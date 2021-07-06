#include "stdafx.h"
#include <afxmt.h>
#include "CommThread.h"
#include "global.h"

// 메세지를 받을 윈도우 핸들, 부모 윈도우에서 HWND hCommWnd= this->m_hWnd로
// 설정해 준다.
extern HWND g_hCommWnd;

//extern CCriticalSection critic;

// CQueue 구현부========================================= 

// Queue의 생성자
CQueue::CQueue()
{
	Clear();
}

// Initialize Queue
void CQueue::Clear()
{
	m_iHead = m_iTail = 0;
	memset(buff, 0, BUFF_SIZE);
}

// Queue에 들어 있는 자료 개수.
int CQueue::GetSize()
{
	return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
}

// Queue에 1 byte 넣음.
BOOL CQueue::PutByte(BYTE b)
{
	if (GetSize() == (BUFF_SIZE-1)) return FALSE;
	buff[m_iHead++] = b;
	m_iHead %= BUFF_SIZE;
	return TRUE;
}

// Queue에서 1 byte 꺼냄.
BOOL CQueue::GetByte(BYTE* pb)
{
	if (GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= BUFF_SIZE;
	return TRUE;
}

CCommThread::CCommThread( )
{
//	hReadEnd = CreateEvent(NULL, TRUE/*bManualReset*/, FALSE /*bInitialState*/, NULL);
	m_bConnected = FALSE;
	m_hThreadWatchComm= NULL;
	m_osRead.hEvent = NULL;
	m_osWrite.hEvent = NULL;

}
CCommThread::~CCommThread( )
{

	ClosePort();
}


//+ 2007.9.28 Add BY USY
BOOL CCommThread::OpenPort(CString sPortName, DWORD dwBaud, WORD wPortID, BYTE bByteSize, BYTE bParity, BYTE bStopBits,BOOL bWatch)
//BOOL CCommThread::OpenPort(CString sPortName, DWORD dwBaud, WORD wPortID)
//-
{
	// Local 변수.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	// Initialize variables?
	m_bConnected = FALSE;
	m_wPortID	= wPortID; // COM1-> 0, COM2->1,,,,,

	// Initialize overlapped structure
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;

	if (! (m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))	return FALSE;

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) return FALSE;
	
	// Open COM port?
	m_sPortName = sPortName;

	m_hComm = CreateFile( m_sPortName, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);
	
	if (m_hComm == (HANDLE) -1) return FALSE;

	// 포트 상태 설정.

	// EV_RXCHAR event 설정
	SetCommMask( m_hComm, EV_RXCHAR);	

	//+change 090218(Modification No2)
	// InQueue, OutQueue 크기 설정.
//	SetupComm( m_hComm, 2048, 2048);	
	SetupComm( m_hComm, 1024*32, 1024*32); // 32KB	

	// 포트 비우기.
	PurgeComm( m_hComm,					
		PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout 설정.
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / (dwBaud+1);
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts( m_hComm, &timeouts);

	// dcb 설정
	dcb.DCBlength = sizeof(DCB);
	GetCommState( m_hComm, &dcb);	// 예전 값을 읽음.
	dcb.BaudRate = dwBaud;
	dcb.ByteSize = bByteSize; //8;
	dcb.Parity = bParity; //0;
	dcb.StopBits = bStopBits; //0;
	dcb.fInX = dcb.fOutX = 0 ; //XON/XOFF 관한것
//	dcb.XonChar = ASCII_XON ;
//	dcb.XoffChar = ASCII_XOFF ;

//	dcb.fInX = dcb.fOutX = 0;		// Xon, Xoff 사용.
//	dcb.XonChar = ASCII_XON;
//	dcb.XoffChar = ASCII_XOFF;
//	dcb.XonChar = 0x02;
//	dcb.XoffChar = 0x03;

//	dcb.XonLim = 100;
//	dcb.XoffLim = 100;

	if (! SetCommState( m_hComm, &dcb))	return FALSE;

	// 포트 감시 쓰레드 생성.
	m_bConnected = TRUE;

	//+ 2007.9.28 Mod BY USY
	if(bWatch == TRUE)
	{
		m_hThreadWatchComm = CreateThread( NULL, 0, 
		(LPTHREAD_START_ROUTINE)ThreadWatchComm, this, 0, &dwThreadID);
		if (! m_hThreadWatchComm)
		{
			ClosePort();
			return FALSE;
		}
	}

	return TRUE;
}
	
// 포트를 닫는다.
void CCommThread::ClosePort()
{
	DWORD dwExitCode = 0;
	//+add 090228(Modification No2)
	if(m_bConnected)
	{
		m_bConnected = FALSE;
		if (m_hThreadWatchComm)
		{
			WaitForSingleObject(m_hThreadWatchComm, 2000);
			if(m_hThreadWatchComm){
				TerminateThread(m_hThreadWatchComm,dwExitCode);
				CloseHandle(m_hThreadWatchComm);         
				m_hThreadWatchComm = NULL;
			}
		}

		SetCommMask( m_hComm, 0);
    
		EscapeCommFunction( m_hComm, CLRDTR ) ;

		PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
		CloseHandle( m_hComm);
	}
		//090919
	if(m_osRead.hEvent != NULL)
	{
		CloseHandle(m_osRead.hEvent); 
		m_osRead.hEvent = NULL;
	}

	if(m_osWrite.hEvent != NULL)
	{
		CloseHandle(m_osWrite.hEvent); 
		m_osWrite.hEvent = NULL;
	}
}

DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	DWORD   Error;
	COMSTAT	comstat;

	if (! WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		//+2007.12.11 Add BY USY
		Error = GetLastError();

		if(Error == ERROR_IO_PENDING)
		{
//			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE))
			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, FALSE))
			{
				dwError = GetLastError();

				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}

	return dwWritten;
}

// 포트로부터 pBuff에 nToWrite만큼 읽는다.
// 실제로 읽혀진 Byte수를 리턴한다.
DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//----------------- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);
	dwRead = comstat.cbInQue;
	
	if (dwRead > 0)
	{
		if (! ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts에 정해준 시간만큼 기다려준다.
				while (! GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}

	return dwRead;
}
/*
//데이타를 읽고 데이타를 읽었다는
//메세지를 리턴한다.
void CCommThread::SetReadData(LPTSTR data)
{

	lstrcpy((LPTSTR)abIn,(LPCTSTR)data);
   //ConverData
   //설정된 윈도우에 WM_RECEIVEDATA메세지를 날려주어 현제 데이타가 들어왔다는것을
   //알려준다.

//	::SendMessage(g_hCommWnd, WM_RECEIVEDATA, pComm->m_wPortID, 0 );//CSerialComDlg로 데이터가 들어왔다는 메시지를 보냄
	::SendMessage(g_hCommWnd,WM_RECEIVEDATA, m_wPortID, 0);
}


DWORD	ThreadWatchComm(CCommThread* pComm)
{
	DWORD		dwEvent;
	OVERLAPPED	os;
	BOOL		bOk = TRUE;
	char		buff[2049];	 // 읽기 버퍼
	DWORD		dwRead;	 // 읽은 바이트수.

	// Event, OS 설정.
	memset( &os, 0, sizeof(OVERLAPPED));
	if (! (os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)))
		bOk = FALSE;
	if (! SetCommMask( pComm->m_hComm, EV_RXCHAR))
		bOk = FALSE;
	if (! bOk)
	{
		AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
	}

	// 포트를 감시하는 루프.
	while (pComm->m_bConnected)
	{
		dwEvent = 0;
		
		// 포트에 읽을 거리가 올때까지 기다린다.
		//+2007.10.17 Mod BY MKW
//		WaitCommEvent( pComm->m_hComm, &dwEvent, &os);
		WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);		//-

		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{

			do
		   {
			   memset(buff,0, 2048);
				if (dwRead = pComm->ReadComm( buff, 2048))
				{
					pComm->SetReadData(buff);
				}
		   }
		   while ( dwRead > 0 ) ;
		}
		Sleep(0); //ResetEvent(os.hEvent);

		
			//	읽어 가도록 메시지를 보낸다.
//			if(dwRead > 0) ::PostMessage(g_hCommWnd,WM_COMM_READ, pComm->m_wPortID, 0);
		//	::SendMessage(g_hCommWnd,WM_COMM_READ, pComm->m_wPortID, 0);
		//	::PostMessage(g_hCommWnd,WM_COMM_READ, pComm->m_wPortID, 0);
			//
			//-
		//	SetEvent(pComm->hReadEnd);

	}	
	
	// 포트가 ClosePort에 의해 닫히면 m_bConnected 가 FALSE가 되어 종료.

	CloseHandle( os.hEvent);
	CloseHandle(pComm->m_hThreadWatchComm);
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;
}
*/

DWORD	ThreadWatchComm(CCommThread* pComm)
{
   DWORD           dwEvent;
   OVERLAPPED      os;
   BOOL            bOk = TRUE;
   BYTE            buff[2048];      // 읽기 버퍼
   DWORD           dwRead;  // 읽은 바이트수.
 

   // Event, OS 설정.
   memset( &os, 0, sizeof(OVERLAPPED));
   
   //--> 이벤트 설정..
   if( !(os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)) )
   {
		bOk = FALSE;
   }

   //--> 이벤트 마스크..
   if( !SetCommMask( pComm->m_hComm, EV_RXCHAR) )
   {
	   bOk = FALSE;
   }

   //--> 이벤트나..마스크 설정에 실패함..
   if( !bOk )
   {
		AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
   }
  
   while (pComm ->m_bConnected)//포트가 연결되면 무한 루프에 들어감
   {
 		dwEvent = 0;
	
        // 포트에 읽을 거리가 올때까지 기다린다.
        WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);
	
	
		//--> 데이터가 수신되었다는 메세지가 발생하면..
        if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
        {
            // 포트에서 읽을 수 있는 만큼 읽는다.
				//--> buff 에 받아놓고..
			do
			{
				dwRead = pComm->ReadComm( buff, 2048); //들어온 데이터 읽어 오기 
				if(BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for( WORD i = 0; i < dwRead; i++ )
					{
						pComm->m_QueueRead.PutByte(buff[i]);//큐 버퍼에  들어온 데이터 넣기 
					}
				}
				else
				{
					AfxMessageBox("buff full"); //큐버퍼의 크기를 초과하면 경고 메시지 보냄
					PurgeComm(pComm->m_hComm,
						PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
				}
			}while(dwRead);
//			::PostMessage(g_hCommWnd, WM_COMM_READ, pComm->m_wPortID, 0 );//CSerialComDlg로 데이터가 들어왔다는 메시지를 보냄
			::SendMessage(g_hCommWnd, WM_COMM_READ, pComm->m_wPortID, 0 );//CSerialComDlg로 데이터가 들어왔다는 메시지를 보냄
		}
		Sleep(0);	// 받은 데이터를 화면에 보여줄 시간을 벌기 위해.
					// 데이터를 연속으로 받으면 cpu점유율이 100%가 되어 화면에 뿌려주는 작업이 잘 안되고. 결과적으로 
					// 큐 버퍼에 데이터가 쌓이게 됨
   }
   
	CloseHandle( os.hEvent);

   //--> 쓰레드 종료가 되겠죠?
	CloseHandle(pComm->m_hThreadWatchComm);
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;

}
//+ 2007.11.22 Add BY USY
int CCommThread::ClearPort()
{
	int nResult = 0;
	// 포트 비우기.
	nResult = PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	return nResult;
}

void CCommThread::SetComPort(int port,DWORD rate,BYTE bytesize,BYTE stop,BYTE parity)
{
   bPort=port;
   dwBaudRate=rate;
   bByteSize=bytesize;
   bStopBits=stop;
   bParity=parity;
}
