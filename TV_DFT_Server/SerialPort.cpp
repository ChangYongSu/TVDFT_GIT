#include "stdafx.h"
#include "SerialPort.h"
#include <assert.h>


	extern HWND g_hViewWnd;//gethandletest

//*****************************************************************************//
// Constructor
CSerialPort::CSerialPort()
{
	total = 0 ;
	m_hComm = NULL;
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;
	//m_szWriteBuffer = NULL;
	m_bThreadAlive = FALSE;
	mPort_Opened = FALSE;

	m_RxMessageID = WM_COMM_RXCHAR;
}

//*****************************************************************************//
// Delete dynamic memory
CSerialPort::~CSerialPort()
{
	do
	{
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);
	TRACE("Thread ended\n");
	//delete [] m_szWriteBuffer;
}

//*****************************************************************************//
// Initialize the port. This can be port 1 to 4.
BOOL CSerialPort::InitPort(CWnd* pPortOwner,      // the owner (CWnd) of the port (receives message)	
						   UINT  portnr,	      // portnumber (1..4)
						   UINT  baud,	          // baudrate
						   char  parity,	      // parity 
						   UINT  databits,	      // databits 
						   UINT  stopbits,	      // stopbits 
						   DWORD dwCommEvents,    // EV_RXCHAR, EV_CTS etc
						   UINT  writebuffersize) // size to the writebuffer 
{
	//assert(portnr > 0 && portnr < 10);
	assert(pPortOwner != NULL);


//	g_hViewWnd = pPortOwner->m_hWnd;

	if (m_bThreadAlive) 
	{
		//do
		//{
			SetEvent(m_hShutdownEvent);
		//RACE("Thread ended\n");

		m_bThreadAlive = FALSE;
	}

	if (m_ov.hEvent != NULL) ResetEvent(m_ov.hEvent);  
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hWriteEvent != NULL) ResetEvent(m_hWriteEvent);
	m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hShutdownEvent != NULL) ResetEvent(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//Initialize the event objects
	m_hEventArray[0] = m_hShutdownEvent; // highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	InitializeCriticalSection(&m_csCommunicationSync) ;
	
	// set buffersize for writing and save the owner	
	m_pOwner = pPortOwner;  
	//if (m_szWriteBuffer != NULL) delete [] m_szWriteBuffer;
	//m_szWriteBuffer = new char[writebuffersize];

	m_nPortNr = portnr;
	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;
	
	BOOL bResult = FALSE;
	//char *szPort = new char[50];
	//char *szBaud = new char[50];
	CString strport;
	CString szBaud;
	
	//now it critical
	EnterCriticalSection(&m_csCommunicationSync) ;

	//if the port is already opened: close it
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	//sprintf_s(szPort, 50, "COM%d", portnr); // prepare port strings
	//"\\\\.\\COM10",     
	strport.Format(_T("\\\\.\\COM%d"),portnr);
	szBaud.Format(_T("baud=%d parity=%c data=%d stop=%d"), baud, parity, databits, stopbits);
	////////////////////////////////////////////////////
	//get a handle to the port
	m_hComm = CreateFile(strport,//(LPCWSTR)szPort,		                // communication port string (COMX)				
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,			                    // comm devices must be opened with exclusive access
					     NULL,                          // no security attributes
					     OPEN_EXISTING,               	// comm devices must use OPEN_EXISTING
						 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// Async I/O
					     0);	                        // template must be 0 for comm devices		
	if (m_hComm == INVALID_HANDLE_VALUE)                // port not found
	{
		//delete [] szPort;
		//delete [] szBaud;
		return FALSE;
	}

	//set the timeout values
	m_CommTimeouts.ReadIntervalTimeout = 0xffffffff ;    //1000; 
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 0 ;      //1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 0 ;        //1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 2*baud ;//1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 0 ;       //1000;

	//Configure
	if(SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		if(SetCommMask(m_hComm, dwCommEvents))
		{
			if(GetCommState(m_hComm, &m_dcb))
			{
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE ;
				if(BuildCommDCB(szBaud, &m_dcb))
				{
					if(SetCommState(m_hComm, &m_dcb))
						;//normal operation.. ocntinue
					else ProcessErrorMessage("SetCommState()") ;
				}
				else ProcessErrorMessage("BuildCommDCB()") ;
			}
			else ProcessErrorMessage("GetCommState()") ;
		}
		else ProcessErrorMessage("SetCommMask()") ;
	}
	else ProcessErrorMessage("SetCommTimeOuts()") ;
	/*
	SetCommTimeouts(m_hComm, &m_CommTimeouts) ;
	SetCommMask(m_hComm, dwCommEvents) ;
	GetCommState(m_hComm, &m_dcb) ;
	m_dcb.fRtsControl = RTS_CONTROL_ENABLE;
	BuildCommDCB(szBaud, &m_dcb) ;
	SetCommState(m_hComm, &m_dcb) ;
	*/					;

//	delete [] szPort;
//	delete [] szBaud;

	//Flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);// flush the port
	
	//release critical section
	LeaveCriticalSection(&m_csCommunicationSync) ;
	TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", portnr);

	mPort_Opened = TRUE;
	return TRUE;
}

//*****************************************************************************//

UINT CSerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to a pointer of CSerialPort class	
	CSerialPort *port = (CSerialPort*)pParam;

	//Set the status variable in the dialog class to TRUE to indicate the thread is running
	port->m_bThreadAlive = TRUE;	

	//Misc.variables
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;

	//Clear comm buffers at startup
	if (port->m_hComm)// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	
	// begin forever loop.  This loop will run as long as the thread is alive.
	for (;;)
	{ 
		//Make a call to WaitCommEvent().
		//This call will return immediately because our port was created as an async port
		//(FILE_FLAG_OVERLAPPED and m_OverlappedStructlapped structure specified).
		//This call will cause the m_OverlappedStructlapped element m_OverlappedStruct.hEvent,
		//which is part is part of the m_hEventArray, to be placed in a non-signal state
		//if there are no bytes available to be read,
		//or to a signaled state if there are bytes available.
		//If this event handle is set to the non-signaled state, it will be set to signaled
		//when a character arrives at the port.
		//We do this for each port!
		if(port->mPort_Opened == FALSE)
		{
			//Kill this thread. Break is not needed, but makes me feel better.
			AfxEndThread(100);
			break;
		}
		if(port->m_bThreadAlive == FALSE)
		{
			//Kill this thread. Break is not needed, but makes me feel better.
			AfxEndThread(100);
			break;
		}
		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);
		if (!bResult)  
		{
			//If WaitCommEvent() returns FALSE, process the last error to determin the reason..
			switch(dwError=GetLastError())
			{
			case ERROR_IO_PENDING :
				{
					//This is a normal return value if there are no bytes
					//to read at the port. Do nothing and continue
					break ;
				}
			case 87:
				{
					//Under Windows NT, this value is returned for somm reason.
					//I have not investigated why, but it is also a valid reply.
					//Also do nothing and continue.
					break ;
				}
			default :
				{
					//All other error codes indicate a serious error has occured.
					//Process this error
					port->ProcessErrorMessage("WaitCommEvent()") ;
					break ;
				}
			}
		}
		else
		{
			//If WaitCommEvent() returns TRUE, check to be sure there are 
			//actually bytes in the buffer to read.
			//If you are reading more than one byte at a time from the buffer
			//(which this program does not do)you will have the situation occur
			//where the first byte to arrive will cause the WaitForMultipleObjects() function
			//to stop waiting. The WaitForMultipleObjects() function resets
			//the event handle in m_OverlappedStruct.hEvent to the non-signaled state
			//as it returns.
			//If in the time between the reset of this event and the call to 
			//ReadFile() more byte arrive,
			//the m_OverlappedStruct.hEvent handle will be set again to the signaled state.
			//When the call to ReadFile() occures, it will read all of the bytes from the buffer,
			//and the program will loop back around to WaitCommEvent().
			//At this point, you will be in the situation where m_OverlappedStruct.hEvent is set,
			//but there are no bytes available to read. If you proceed and call ReadFile(),
			//it will return immediately due to the async port setup,
			//but GetOverlappedResults() will not return until the next character arrives.
			//It is not desirable for the GetOverlappedResults() function to be in this state.
			//The thread shutdown event(Event 0) and the WriteFile() event(Event2) will not work
			//if the thread is blocked by GetOverlappedResults().
			//The solution to this is to check the buffer with a call to ClearCommError().
			//This call will reset the event handle, and if there are no bytes to read,
			//we can loop back through WaitCommEvent() again, then proceed.
			//If there are really bytes to read, do nothing and proceed.
			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
			if (comstat.cbInQue == 0)
				continue;
		}

		//Main wait function. This function will normally block the thread
		//until one of nine events occur that require action.
		Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);
		switch (Event)
		{
		case 0: 
			{
				//Shutdown event. This is enent 0 so it will be the highest priority
				//and be serviced first.
			 	port->m_bThreadAlive = FALSE;

				//Kill this thread. Break is not needed, but makes me feel better.
				AfxEndThread(100);
				break;
			}
		case 1:
			{
				GetCommMask(port->m_hComm, &CommEvent);
				if (CommEvent & EV_CTS)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_RXFLAG)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_BREAK)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_ERR)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_RING)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_TXEMPTY)
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_RXCHAR)//Receive character event from port.
					ReceiveChar(port);
					//ReceiveChar(port, comstat);
				break;
			}  
		case 2:// write event
			{
				WriteChar(port);// Write character event from port
				break;
			}

		}// end switch
		Sleep(10) ;
	}// close forever loop
	return 0;
}


//*****************************************************************************//
// start comm watching
BOOL CSerialPort::StartMonitoring()
{
	if (!(m_Thread = AfxBeginThread(CommThread, this)))
		return FALSE;
	TRACE("Thread started\n");
	return TRUE;	
}

//*****************************************************************************//
// Restart the comm thread
BOOL CSerialPort::RestartMonitoring()
{
	TRACE("Thread resumed\n");
	m_Thread->ResumeThread();
	return TRUE;	
}

//*****************************************************************************//
// Suspend the comm thread
BOOL CSerialPort::StopMonitoring()
{
	TRACE("Thread suspended\n");
	m_Thread->SuspendThread(); 
	return TRUE;	
}

//*****************************************************************************//
// If there is a error, give the right message
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	ClosePort();
	//sprintf_s(Temp, 200, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr); 
	//MessageBox(NULL, (LPCSTR)Temp, _T("Application Error"), MB_ICONSTOP);
	LocalFree(lpMsgBuf);
	delete[] Temp;
}

unsigned long length;
//*****************************************************************************//
// Write a character.
void CSerialPort::WriteChar(CSerialPort* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;
	DWORD BytesSent = 0;
	ResetEvent(port->m_hWriteEvent);
	// Gain ownership of the critical section
	
	EnterCriticalSection(&port->m_csCommunicationSync);

	if (bWrite)
	{
		port->m_ov.Offset = 0;// Initailize variables min
		port->m_ov.OffsetHigh = 0;
		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR |
								 PURGE_RXABORT | PURGE_TXABORT);
		
		//실제 데이터를 보내기에 앞서 일단 데이터를 송신한다는 메세지를 한번 쏜다...
		//::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_TXCHAR, (WPARAM)BytesSent, (LPARAM) 0 ) ;
		//실제 데이터를 보낸다. WriteFile
		bResult = WriteFile(port->m_hComm,		    // Handle to COMM Port					
							port->m_szWriteBuffer,	// Pointer to message buffer in calling finction				
							length,	                // Length of message to send
							&BytesSent,	            // Where to store the number of bytes sent							
							&port->m_ov);           // Overlapped structure

		
		//여기서부터 거의 모든 시간이 소요된다.
		// deal with any error codes
		if (!bResult)
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						//continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						port->ProcessErrorMessage("WriteFile()");
					}
			}
		} 
		else
		{

		}
	}// end if(bWrite)
	if (!bWrite)
	{
		bWrite = TRUE;
		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port  
									  &port->m_ov,      // Overlapped structure
									  &BytesSent,       // Stores number of bytes sent
									  TRUE);            // Wait flag
		
		LeaveCriticalSection(&port->m_csCommunicationSync) ;
		
		//deal with the error code
		if (!bResult)  
		{
			port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	}

	//Verity that the data size send equals what we tried to send
	/*
	if (BytesSent != strlen((char*)port->m_szWriteBuffer))
	{
		TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
	}
	*/

	//port->total = BytesSent ;
	//::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_TXCHAR, (WPARAM) BytesSent, (LPARAM) 0 ) ;
	::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_TXCHAR, (WPARAM) BytesSent, (LPARAM) 0 ) ;
	
	//Sleep(50) ;
	//위의 루틴에서 모든 데이터가 송신되면 송신완료되었다는 메세지를 한번 쏜다...
	//::PostMessage(port->m_pOwner->m_hWnd, WM_COMM_DISPLAY, (WPARAM) 0, (LPARAM) 0 ) ;
}

//*****************************************************************************//
// Character received. Inform the owner
//void CSerialPort::ReceiveChar(CSerialPort* port, COMSTAT comstat)
	
void CSerialPort::ReceiveChar(CSerialPort* port)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	COMSTAT comstat;
	unsigned char RXBuff[4096];

	for (;;) 
	{ 		
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object.
		if(port->mPort_Opened == FALSE)
		{
			//Kill this thread. Break is not needed, but makes me feel better.
			//AfxEndThread(100);
			break;
		}
		if(port->m_bThreadAlive == FALSE)
		{
			//Kill this thread. Break is not needed, but makes me feel better.
			//AfxEndThread(100);
			break;
		}
		
		EnterCriticalSection(&port->m_csCommunicationSync) ;

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
		LeaveCriticalSection(&port->m_csCommunicationSync);

		// start forever loop.  I use this type of loop because I
		// do not know at runtime how many loops this will have to
		// run. My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample 
		// as it is in my production code, but I have found this 
		// solutiion to be the most efficient way to do this.

		if (comstat.cbInQue == 0)
		{
			// break out when all bytes have been read
			break;
		}

		EnterCriticalSection(&port->m_csCommunicationSync) ;

		if (bRead)
		{
			bResult = ReadFile(port->m_hComm, // Handle to COMM port 
							   RXBuff,	      // RX Buffer Pointer
							   256,             // Read one byte(1:한개씩, 2:두개마다 한개씩)
							   &BytesRead,    // Stores number of bytes read
							   &port->m_ov);  // pointer to the m_ov structure
			//deal with the error code
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							// asynchronous i/o is still in progress 
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{
							// Another error has occured.  Process this error.
							port->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}//close if(bRead)


		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(port->m_hComm, // Handle to COMM port 
										  &port->m_ov,   // Overlapped structure
										  &BytesRead,    // Stores number of bytes read
										  TRUE); 
			if (!bResult)  
			{
				port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// notify parent that a byte was received
		
		//의미없는 행동은 컴파일 최적화를 위해 없애버린다. 고로 실행 안된다.
		//UINT count01 = 5000000 ;
		//for(unsigned long count=0 ; count<5000000 ; count++) count01--;
		//Sleep(2) ;
		//::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
//		::PostMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
		if(BytesRead > 0)
		{
			//memcpy(TransBuff, RXBuff, BytesRead);
			//::SendMessage(g_hViewWnd, WM_COMM_RXCHAR, (WPARAM)BytesRead, (LPARAM)RXBuff );//gethandletest
		//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM)BytesRead, (LPARAM)RXBuff );//gethandletest
			::SendMessage((port->m_pOwner)->m_hWnd, port->m_RxMessageID, (WPARAM)BytesRead, (LPARAM)RXBuff );//gethandletest
			BytesRead = 0;
		}
	}
}


//*****************************************************************************//
// Write a string to the port
void CSerialPort::WriteToPort(unsigned char* string, unsigned long len)
{	
	if(len > 4095)
		length = 4095;
	else
		length = len;
	assert(m_hComm != 0);
//	m_szWriteBuffer = new char [len];
	for(unsigned long i=0; i<len; i++)
		m_szWriteBuffer[i] = string[i];
	SetEvent(m_hWriteEvent);//set event for write
}


//*****************************************************************************//
// Return the device control block
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}


//*****************************************************************************//
// Return the communication event masks
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}


//*****************************************************************************//
// Return the output buffer size
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}

void CSerialPort::ClosePort()//이것만 내가 추가한것..
{


	if(m_bThreadAlive != NULL)
	{
		SetEvent(m_hShutdownEvent) ;
		//WaitForSingleObject(m_Thread, INFINITE) ;
		m_Thread = NULL ;
	}
	if(m_hComm)
	{
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR |
								 PURGE_RXABORT | PURGE_TXABORT);
		CloseHandle(m_hComm) ;
		m_hComm = INVALID_HANDLE_VALUE ;
	}
	//CloseHandle(m_hComm) ;
	//AfxMessageBox("Do you want to close application?") ;
	Sleep(500) ;
	mPort_Opened = FALSE;
}
