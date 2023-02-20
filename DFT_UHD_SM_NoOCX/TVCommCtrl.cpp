#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "TVCommCtrl.h"
#include "AvSwitchBoxCtrl.h"
CTVCommCtrl				TVCommCtrl;
extern CDATsysView*		g_pView;
extern int g_ID_Check;

CTVCommCtrl::CTVCommCtrl()
{
	m_bPortOpen				= FALSE;
	m_nPortID				= -1;
	//+change kwmoon 080904
//	m_nResult				= T_NOTEST;
	m_nResult				= TEST_NOTEST;
	m_strErrMsg				= _T("");
	
	m_bThreadRunning		= FALSE;
	m_bExitThread			= FALSE;

	//+change 090213(Modification No1)
//	m_hThread				= NULL;
	m_pAdcThread			= NULL;

//	m_hAdcThreadKillEvent	= NULL;
	m_hReadEvent            = NULL;
	m_strErrMsg				= _T("");	

	memset(m_nReceiveData, 0, 2048);
	m_nReceiveLength = 0;
	m_sReceive			= _T("");

	m_aCmd1.RemoveAll();
	m_aCmd2.RemoveAll();
	m_aSetId.RemoveAll();
	m_aData.RemoveAll();
	m_aWaitLimit.RemoveAll();
	
	m_nCheckType			= 0;	
	m_strTargetAvg			= _T("");	
	m_nAvgMargin			= 0;	

	memset(m_nAdcI2cData,0,256);

	// add 090417
	m_nResultCheckType      = 0;
	memset(m_nResultTarget, 0, 9);
	memset(m_nResultData, 0, 9);
	memset(m_nResultMargin, 0, 3);
	m_bResultRead = FALSE;
	m_aCheckType.RemoveAll();
	m_aCheckOption.RemoveAll();

	//add 110215
	m_aMacAdd.RemoveAll();
	m_aMacAdd2.RemoveAll();
	m_nHighSpeedCheckUSB = 0;

	m_bStopBCM_TestThread = FALSE;

	m_nInputDetectItem = JACK_AV1; // 0
	m_nDetectMode = 1;
}

CTVCommCtrl::~CTVCommCtrl()
{
	if(m_hReadEvent != NULL)
	{
		CloseHandle(m_hReadEvent); m_hReadEvent = NULL;
	}

}


BOOL CTVCommCtrl::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");

//	if( !m_ctrlTVCommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID) )
	if( !m_ctrlTVCommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID, 8, 0, 0,TRUE) )
	{
		m_strErrMsg.Format("[TV CommPort] \n%s Init Failed!", sComPort);
		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	_Wait(150);

//	m_hAdcThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);
	m_hReadEvent = ::CreateEvent(NULL,false,false,NULL);
	
	return TRUE;
}

void CTVCommCtrl::PortClose()
{
	m_ctrlTVCommCtrl.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CTVCommCtrl::Init()
{
//	memset(m_strReceive, 0, 225);
	m_sReceive = _T("");
	return TRUE;
}

//+del kwmoon 080320
/*
BOOL CTVCommCtrl::ReceiveCommString(int nRev, CString& sReadString)
{
//SG msg;
	BOOL		bDoing;
    clock_t		start;
	int			i;
	BYTE		buf;
	DWORD		dwRead;	 // 읽은 바이트수.
	char		buff[2048];	 // 읽기 버퍼

	if(!m_bPortOpen) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();
	m_ctrlTVCommCtrl.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{
			dwRead = m_ctrlTVCommCtrl.ReadComm( buff, 2048);
				
			if (BUFF_SIZE - m_ctrlTVCommCtrl.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlTVCommCtrl.m_QueueRead.PutByte(buff[i]);
			}
			else
				AfxMessageBox("m_QueueRead FULL!");
			  
		} while (dwRead); 

		int rrv = m_ctrlTVCommCtrl.m_QueueRead.GetSize();

		if (rrv == nRev)
		{
			for(i=0; i<nRev; i++)
			{
				m_ctrlTVCommCtrl.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}
			sReadString.Format("%s", m_strReceive);
//			if(CurrentSet->bCommDisplay)
//			{
//				CString s;
//				s.Format("TV Rs232->Pc : %s",m_strReceive);
//				AddStringToStatus(s);
//			}
			return TRUE;
		}
		

        if ((clock() - start)/CLOCKS_PER_SEC >5) 
        {
			return FALSE;
		}
	}
	
	return TRUE;	
}
*/

//+change kwmoon 080326
BOOL CTVCommCtrl::ReceiveCommString(int nRev, CString sWaitLimit, CString& sReadString)
{
	unsigned	nWaitLimit;
	int			nReadQueueSize = 0;

	DWORD		dwRead;		 
	BYTE		Buffer;
	BYTE		ReadBuffer[2048];	
	BYTE		strReceive[2048];

	if(!m_bPortOpen) return FALSE;

	memset(ReadBuffer,   0, 2048);
//	memset(m_strReceive, 0, 2048);
	m_sReceive = _T("");
	memset(strReceive, 0, 2048);

	nWaitLimit = atoi(sWaitLimit);
	
	DWORD dwStartTick = GetTickCount();

	m_ctrlTVCommCtrl.m_QueueRead.Clear();

	while (TRUE)
	{
//		do {	
		while(TRUE)
		{
			dwRead = m_ctrlTVCommCtrl.ReadComm( ReadBuffer, 2048);

			if (dwRead == 0) break;
				
			nReadQueueSize = m_ctrlTVCommCtrl.m_QueueRead.GetSize();

			if (BUFF_SIZE - nReadQueueSize > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
				{
					m_ctrlTVCommCtrl.m_QueueRead.PutByte(ReadBuffer[i]);
				}
			}
			else
			{
				AfxMessageBox("m_QueueRead FULL!"); return FALSE;
			}

			if((GetTickCount() - dwStartTick) > nWaitLimit)		
			{
				m_ctrlTVCommCtrl.m_QueueRead.Clear();
				return FALSE;
			}

			if(CurrentSet->bRunAbort)
			{
				m_ctrlTVCommCtrl.m_QueueRead.Clear();
				return FALSE;
			}

			if(m_ctrlTVCommCtrl.m_QueueRead.GetSize() >= nRev)
			{
				break;			
			}
			else
			{
				int debug = 1;
			}
			Sleep(10);
		}
//		} while (dwRead); 

		int rrv = m_ctrlTVCommCtrl.m_QueueRead.GetSize();
		
		//+change kwmoon 080326
		if((rrv >0) && (rrv <= nRev))
		{
			for(int i=0; i<nRev; i++)
			{
				m_ctrlTVCommCtrl.m_QueueRead.GetByte(&Buffer);
				strReceive[i] = Buffer;
			}
			sReadString.Format("%s", strReceive);

			if(CurrentSet->bCommDisplay)
			{
				CString s;
				s.Format("Receive : %s",strReceive);
				AddStringToStatus(s);
			}
			return TRUE;
		}

		if((GetTickCount() - dwStartTick) > nWaitLimit)		
		{	
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			return FALSE;
		}
		
		if(CurrentSet->bRunAbort)
		{
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			return FALSE;
		}
		Sleep(10);
	}
	return TRUE;	
}


BOOL CTVCommCtrl::SendByte(BYTE Data)
{
	SendCommString(&Data);
	return TRUE;
}

BOOL CTVCommCtrl::SendCommString(CString str)
{
	CString sTemp = _T("");
	m_RcvTimeOut = 0;
	m_ctrlTVCommCtrl.m_QueueRead.Clear();

	::ResetEvent(m_hReadEvent);

	m_sReceive = _T("");
	memset(m_nReceiveData, 0, 2048);
	m_nReceiveLength = 0;
	int nRtn = m_ctrlTVCommCtrl.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());

	if(CurrentSet->bIsRunning)
	{
		sTemp = m_strSendStr;
		sTemp.TrimRight(0x0a);
		sTemp.TrimRight(0x0d);

		SaveComLog(sTemp);
	}
	
	if(nRtn != str.GetLength()) return FALSE;

	return TRUE;
}

BOOL CTVCommCtrl::SendRun(CString sData)
{
	CString szCmdString;
	m_sReceive = _T("");

	m_strSendStr.Format(">04%s\r", sData);
	SendCommString(m_strSendStr);

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	return TRUE;
}


BOOL CTVCommCtrl::SendRun(CString sCmd1, CString sCmd2, int nSetId, CString sData)
{
	m_sReceive = _T("");
	_Wait(100);
		
	m_strSendStr.Format("%s%s %02d %s%c", sCmd1, sCmd2, nSetId, sData, 0x0d);

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	
	BOOL bRtn = TRUE;
	bRtn = SendCommString(m_strSendStr);
	return bRtn;
}

BOOL CTVCommCtrl::ReceiveCommString(int nRev, int nWaitLimit, CString& sReadString)
{
	BOOL		bDoing;
    clock_t		start;
	CString		sTemp = _T("");
	CString     sSendData = _T("");
	int         nPos = -1;
	int         nPos1 = -1;
//	int         nLoop = 0;
	int         nTemp;

	MSG msg;
	DWORD dwEventResult = 0;

	if (!m_bPortOpen)
	{
		AddStringToStatus("TVCommCtrl Port Not Open");
		return FALSE;
	}

//	m_sReceive = _T("");


	if(m_strSendStr.GetLength() > 5)
	{
		sSendData = m_strSendStr.Left(4) ;
		sSendData.Delete(0); 
	}


	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   

		dwEventResult = WaitForSingleObject(m_hReadEvent, 500);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(m_hReadEvent);
		}
#ifdef DEBUG_MD5_CODE__
		//m_sReceive = "a 01 TO91x";//"a 01 NG91x";// 
#endif
		nTemp = m_sReceive.GetLength();
		if(nTemp > 0)
		{	

			if (nTemp > 50)
			{
				
				AddStringToStatus("TV-Com->PC : ERROR");
				AddStringToStatus(m_sReceive);
				m_ctrlTVCommCtrl.m_QueueRead.Clear();
				m_strSendStr = _T("");
				return FALSE;
			}


			if((nPos == -1) && (nPos1 == -1))
			{
				nPos = m_sReceive.Find('x');
				if(nPos != -1)
				{
					nPos1 = m_sReceive.Find(nPos + 1,'x');

					if((nPos != -1) && (nPos < (nTemp-3)))
					{
						if((m_sReceive.Find("OK") == -1) && (m_sReceive.Find("NG") == -1) && (m_sReceive.Find("NO") == -1) && (m_sReceive.Find("NULL") == -1))
						{
							if ((clock() - start)*1000/CLOCKS_PER_SEC <= nWaitLimit) 
								continue;
						}
					}
				}
			}
			
			if((nPos != -1) && (nPos1 == -1))
			{
				nPos1 = m_sReceive.Find(nPos + 1,'x');
				if(nPos < (nTemp-3))
				{
					if((m_sReceive.Find("OK") == -1) && (m_sReceive.Find("NG") == -1) && (m_sReceive.Find("NO") == -1) && (m_sReceive.Find("NULL") == -1))
					{
						if ((clock() - start)*1000/CLOCKS_PER_SEC <= nWaitLimit) 
							continue;
					}
				}
			}
			
			if((nPos != -1) && (nPos1 != -1))
			{
				
				nPos  = nPos1;
				nPos1 = m_sReceive.Find(nPos + 1,'x');
			}


//			if (nPos != -1)
			if ((nPos > 0) && (nPos1 == -1))
			{
		//		nPos1 = nPos;
		//		sTemp = m_sReceive.Left(nPos + 1);
				sTemp = m_sReceive;

				nPos1 = sTemp.Find(m_strSendStr);
				if (nPos1 >= 0)
				{
					sTemp.Delete(0, nPos1 + m_strSendStr.GetLength());

				}

				if((sTemp.Find("OK") != -1) || (sTemp.Find("NG") != -1) || (sTemp.Find("TO") != -1) || (sTemp.Find("NO") != -1) ||(sTemp.Find("NULL") != -1))
				{
					if(sSendData != _T(""))
					{
						nPos1 = sTemp.Find(sSendData);
						if (nPos1 > 0) sTemp.Delete(0, nPos1);
					}
				}

				sTemp.TrimLeft(0x0a);
				sTemp.TrimLeft(0x0d);
				sTemp.TrimRight(0x0a);
				sTemp.TrimRight(0x0d);

				sReadString.Format("%s", sTemp);
				if(CurrentSet->bCommDisplay && !g_pView->m_Rs232cAdcManualCheckDlg.m_bActive)
				{
					CString s;
#ifdef TEXT_MSG_EXE_DEBUG_CODE__
					AddStringToStatus("1");//DEBUG
					if (sTemp.GetLength() > 16)
					{
						AddStringToStatus("ERROR");
					}
#endif
					s.Format("TV-Com->PC 1: %s",sTemp);
					AddStringToStatus(s);
				}
				if(CurrentSet->bIsRunning)
				{
					SaveComLog(sTemp);
				}

				m_ctrlTVCommCtrl.m_QueueRead.Clear();
				m_strSendStr = _T("");

				return TRUE;
			}
			else
			{

				if(nPos1 != -1)
				{
					sTemp = m_sReceive.Left(nPos1 + 1);
					sTemp.TrimLeft(0x0a);
					sTemp.TrimLeft(0x0d);
					sTemp.TrimRight(0x0a);
					sTemp.TrimRight(0x0d);
					sReadString.Format("%s", sTemp);
					if(CurrentSet->bCommDisplay && !g_pView->m_Rs232cAdcManualCheckDlg.m_bActive)
					{
						CString s;
#ifdef TEXT_MSG_EXE_DEBUG_CODE__
						AddStringToStatus("2");//DEBUG
#endif
						s.Format("TV-Com->PC 2: %s",sTemp);
						AddStringToStatus(s);
					}
					if(CurrentSet->bIsRunning)
					{
						SaveComLog(sTemp);
					}

					m_ctrlTVCommCtrl.m_QueueRead.Clear();
					m_strSendStr = _T("");

					return TRUE;
				}
				else
				{
					if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
					{
						m_sReceive.TrimLeft(0x0a);
						m_sReceive.TrimLeft(0x0d);
						m_sReceive.TrimRight(0x0a);
						m_sReceive.TrimRight(0x0d);
						sReadString.Format("%s", m_sReceive);
						if(CurrentSet->bCommDisplay && !g_pView->m_Rs232cAdcManualCheckDlg.m_bActive)
						{
							CString s;
#ifdef TEXT_MSG_EXE_DEBUG_CODE__
							AddStringToStatus("3");//DEBUG
#endif
							s.Format("TV-Com->PC 3: %s",m_sReceive);
							AddStringToStatus(s);
						}
						if(CurrentSet->bIsRunning)
						{
							SaveComLog(m_sReceive);
						}

						m_ctrlTVCommCtrl.m_QueueRead.Clear();
						m_strSendStr = _T("");

						return TRUE;
					}
				}
			}
		}	

        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			m_strSendStr = _T("");
			AddStringToStatus("Rcv Time Out");
			m_RcvTimeOut = 1;
			BOOL lRtn = FALSE;
			return lRtn; //FALSE;
		}

		if(CurrentSet->bRunAbort)
		{
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			m_strSendStr = _T("");
			return FALSE;
		}
	}
	return TRUE;	
}

BOOL CTVCommCtrl::ReceiveCommString_BCM(int nRev, int nWaitLimit, CString& sReadString)
{

	BOOL		bDoing;
    clock_t		start;
	CString		sTemp;
	CString     sSendData = _T("");
	int         nPos = -1;
	int         nPos1 = -1;
//	int         nLoop = 0;
	int         nTemp;

	MSG msg;
	DWORD dwEventResult = 0;

	if(!m_bPortOpen) return FALSE;

	m_sReceive = _T("");

	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   

		dwEventResult = WaitForSingleObject(m_hReadEvent, 500);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(m_hReadEvent);
		}
		nTemp = m_sReceive.GetLength();
		if(nTemp > 0)
		{	
//			if((nPos == -1) && (nPos1 == -1))
			sTemp = m_sReceive;
			sTemp.TrimLeft(0x0d);
			sTemp.TrimLeft(0x0a);
			if((nPos == -1))
			{
				nPos = sTemp.Find(0x0a);
			}

			if ((nPos > 1))
			{
			//	sTemp = m_sReceive;

			//	sTemp.TrimLeft(0x0a);
			//	sTemp.TrimLeft(0x0d);
				sTemp.TrimRight(0x0a);
				sTemp.TrimRight(0x0d);

				sReadString.Format("%s", sTemp);
				if(CurrentSet->bCommDisplay && !g_pView->m_Rs232cAdcManualCheckDlg.m_bActive)
				{
					CString s;
					s.Format("TV-Com->PC : %s",sTemp);
					AddStringToStatus(s);
				}

				m_ctrlTVCommCtrl.m_QueueRead.Clear();
				m_strSendStr = _T("");

				return TRUE;
			}
		}	

        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			m_strSendStr = _T("");
			return FALSE;
		}

		if(CurrentSet->bRunAbort)
		{
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			m_strSendStr = _T("");
			return FALSE;
		}
	}
	return TRUE;	
}
BOOL CTVCommCtrl::ReceiveCommString_SN(int nRev, int nWaitLimit, CString& sReadString)
{

	BOOL		bDoing;
    clock_t		start;
	CString		sTemp = _T("");
	CString     sSendData = _T("");
	int         nPos = -1;

	MSG msg;
	DWORD dwEventResult = 0;

	if(!m_bPortOpen) return FALSE;

	m_sReceive = _T("");

	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   

		dwEventResult = WaitForSingleObject(m_hReadEvent, 500);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(m_hReadEvent);
			//sTemp = m_sReceive;
			for(int i = 0; i< m_nReceiveLength; i++){
				if(m_nReceiveData[i] == 0x0d){
					nPos = i;
				}
			}

			if(nPos >= (nRev-1)){
				for(int j = nPos - (nRev-1); j<= nPos; j++)
				{
					sTemp.Format("%02x",m_nReceiveData[j]);
					sSendData = sSendData + sTemp;
				}
				sReadString.Format("%s", m_nReceiveData);
				if(CurrentSet->bCommDisplay && !g_pView->m_Rs232cAdcManualCheckDlg.m_bActive)
				{
					AddStringToStatus(sReadString);
					AddStringToStatus("TV-Com->PC:" + sSendData);
				}
				if(CurrentSet->bIsRunning)
				{
					SaveComLog(sSendData);
				}
				
				m_ctrlTVCommCtrl.m_QueueRead.Clear();

				return TRUE;
			}
			else{
				if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
				{
					for(int j = 0; j< m_nReceiveLength; j++)
					{
						sTemp.Format("%02x",m_nReceiveData[j]);
						sSendData = sSendData + sTemp;
					}
					sReadString.Format("%s", m_sReceive);
					if(CurrentSet->bCommDisplay && !g_pView->m_Rs232cAdcManualCheckDlg.m_bActive)
					{
						AddStringToStatus(sReadString);
						AddStringToStatus("TV-Com->PC:" + sSendData);
					}
					if(CurrentSet->bIsRunning)
					{
						SaveComLog(sTemp);
					}

					m_ctrlTVCommCtrl.m_QueueRead.Clear();
					m_strSendStr = _T("");

					return FALSE;
				}
			}

		}	

        if (((clock() - start)*1000/CLOCKS_PER_SEC) > nWaitLimit) 
        {
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			m_strSendStr = _T("");
			return FALSE;
		}

		if(CurrentSet->bRunAbort)
		{
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			m_strSendStr = _T("");
			return FALSE;
		}
	}
	return TRUE;	
}


//+add kwmoon 071015 : [2in1]
UINT CTVCommCtrl::BCM_TestThread(LPVOID pParam)
{
	BOOL bGrabbing = FALSE;

	CTVCommCtrl* pTVCommCtrl = (CTVCommCtrl*)pParam;

	BOOL bResult	= FALSE;

	CString szFileName = _T("");
	CString szErrorMsg = _T("");

	BYTE* pImageBuffer = NULL;

//	pTVCommCtrl->m_ctrlTVCommCtrl.WriteComm((char*)(LPCTSTR)"ka 00 01", 8);

	while(TRUE)
	{
		pTVCommCtrl->m_ctrlTVCommCtrl.WriteComm((BYTE*)(LPCTSTR)"~", 1);
		Sleep(25);
		if(pTVCommCtrl->m_bStopBCM_TestThread)
		{
			break;
		}
	}

	return TRUE;
}

BOOL CTVCommCtrl::BCMTest(int nWaitLimit)
{
	CString sReadData;
	CString sTemp;

	BOOL bResult = FALSE;
	BOOL		bDoing;
    clock_t		start;
	MSG msg;

	m_bStopBCM_TestThread = FALSE;
	::ResetEvent(m_hReadEvent);

	AfxBeginThread(BCM_TestThread,this);
	Sleep(1000);
//	Send_TVControl_Command("ka 00 01", 500);
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "C4");

	if(!ReceiveCommString_BCM(13, 10000, sReadData))
	{

		m_bStopBCM_TestThread = TRUE;
		return FALSE;
	}
	m_bStopBCM_TestThread = TRUE;
	if(sReadData.Find("Running") == -1)
	{
		return FALSE;
	}
	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		
		if(ReceiveCommString_BCM(13, 1000, sReadData))
		{
			if(sReadData.Find("PASSED") != -1)
			{
				bResult = TRUE;
				break;
			}
			else if(sReadData.Find("FAILED") != -1)
			{
				bResult = FALSE;
				break;
			}

			if(sReadData.Find("Now reboot") != -1) break;

		}


        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			return FALSE;
		}

		if(CurrentSet->bRunAbort)
		{
			m_ctrlTVCommCtrl.m_QueueRead.Clear();
			return FALSE;
		}

	}

	return bResult;
}


UINT CTVCommCtrl::StartAdcThread(LPVOID pParam)
{
	CTVCommCtrl* pTVCommCtrl =(CTVCommCtrl* )pParam;

	CString szMsg, sTmp, sCmd1, sCmd2, sData, sReadData, sOkMsg, sNgMsg, szLog;
	
	int nNoAdcTestRetry		= 0;
	int nSetId, nAdcType, nReturnSize, nMaxDelay, nTmp;
	int nNoRetry = 0;

	pTVCommCtrl->m_strCmd1.MakeLower();
	pTVCommCtrl->m_strCmd2.MakeLower();
	pTVCommCtrl->m_strData.MakeLower();
	
	sCmd1.Format("%s", pTVCommCtrl->m_strCmd1);
	sCmd2.Format("%s", pTVCommCtrl->m_strCmd2);
	sData.Format("%s", pTVCommCtrl->m_strData);

	nSetId = pTVCommCtrl->m_nSetId;
	nMaxDelay = pTVCommCtrl->m_nWaitLimit;

	//+change kwmoon 080904
//	pTVCommCtrl->m_nResult = T_NOTEST;
	pTVCommCtrl->m_nResult = TEST_NOTEST;
	
//ADC_START : 


	if(nNoAdcTestRetry >= MAX_NO_ADC_RETRY)
	{
		goto END_THREAD;
	}

	if(pTVCommCtrl->m_bExitThread)
	{
		goto END_THREAD;
	}

	if((sCmd1.Compare("a") == 0)&&(sCmd2.Compare("d") == 0))
	{
		if(sData.Compare("00") == 0) 
		{
			nReturnSize = 21;
			nAdcType = ADC_MODE_IN;
		}
		else if(sData.Compare("10") == 0) 
		{
			nReturnSize = 15;
			nAdcType = ADC_START;
		}
		else if(sData.Compare("90") == 0) 
		{
			nReturnSize = 21;
			nAdcType = ADC_MODE_OUT;
		}
		else 
		{
			nReturnSize = 21;
			nAdcType = NORMAL_TVCMD;
		}
	}
	else if((sCmd1.Compare("k") == 0) && (sCmd2.Compare("b"))) 
	{
		nReturnSize = 21;
		nAdcType = NORMAL_TVCMD;

	}
	pTVCommCtrl->PortClear();
	pTVCommCtrl->SendRun(sCmd1, sCmd2, nSetId, sData);
	
	do 
	{
		if(pTVCommCtrl->ReceiveCommString(nReturnSize, nMaxDelay, sReadData))
		{
			goto ADC_ANALYSE;
		}
		else
		{
			nNoAdcTestRetry++;
		}
	} 
	while(nNoAdcTestRetry == MAX_NO_ADC_RETRY);

	//+change kwmoon 080904
//	pTVCommCtrl->m_nResult = T_TIMEOUT;
	pTVCommCtrl->m_nResult = TEST_TIMEOUT;
	goto END_THREAD;


ADC_ANALYSE :
	
	if((nAdcType == ADC_MODE_IN) ||
	   (nAdcType == ADC_MODE_OUT) ||
 	   (nAdcType == NORMAL_TVCMD))
	{
		sOkMsg.Format("%s %02d OK%sx", sCmd2, nSetId, sData);
		sNgMsg.Format("%s %02d NG%sx", sCmd2, nSetId, sData);
	}
	else if(nAdcType == ADC_START)
	{
		sOkMsg.Format("OKx");
		sNgMsg.Format("NGx");
	}

	if(sReadData.Compare(sOkMsg) == 0) pTVCommCtrl->m_nResult = TEST_PASS;
	else if(sReadData.Compare(sNgMsg) == 0) pTVCommCtrl->m_nResult = TEST_FAIL;
	else pTVCommCtrl->m_nResult = TEST_FAIL;


	BYTE Tmp[100];
	BYTE Tmp1[100];

	for( nTmp = 0; nTmp < sReadData.GetLength(); nTmp++)
	{
		Tmp[nTmp] = sReadData.GetAt(nTmp);
	}
	for(nTmp = 0; nTmp < sOkMsg.GetLength(); nTmp++)
	{
		Tmp1[nTmp] = sOkMsg.GetAt(nTmp);
	}

END_THREAD :


	pTVCommCtrl->m_bThreadRunning = FALSE;

	return 0;
}

void CTVCommCtrl::RunAdc()
{
	{
		StartAdcThread(this);
		m_bThreadRunning = TRUE;
	}
}
//-

//change 090417
UINT CTVCommCtrl::AdcTest(CString sCmd1, CString sCmd2, int nSetId, CString sData,int nCheckType, int nMaxDelay, int nCheckOption)
{
	CString sReadData,sTmp;

//	int nData[9];
	int nAdcType	 =0;
	int nReturnSize	= 0;
//	int nNoRetry; 
	int nRedData	= 0;
	int nGreenData  = 0;
	int nBlueData	= 0;
	int nTarget     = 0;
	int nTargetData1 = 0;
	int nTargetData2 = 0;
	int nTargetData3 = 0;
	int nTargetMargin = 0;
	int nCnt        = 0;
	int nTmp        = 0;
	BOOL bTmp1		= FALSE;
	BOOL bTmp2		= FALSE;
	BOOL bTmp3		= FALSE;
	BOOL bTmp4		= FALSE;
	int nMaxValue   = 0;
	int nMinValue   = 0;
	int nDiff	    = 0; 
	int nCheckItem  = 0;

	CString szPrevMsg = _T("");
	CString szMsg1 = _T("");

	m_nResult = TEST_NOTEST;

	if(CurrentSet->bRunAbort)
	{
		m_nResult = TEST_ABORT; goto END_THREAD;
	}

	//=================================
	// ADC Command (Cmd1='a',Cmd2='d') 
	//=================================
	if((sCmd1.Compare("a") == 0) && (sCmd2.Compare("d") == 0))
	{
		if(sData.Compare("00") == 0) 
		{
			nReturnSize = 22;
			nAdcType = ADC_MODE_IN;
		}
		else if(sData.Compare("10") == 0) 
		{
			nReturnSize = 12;
			nAdcType = ADC_START;
		}
		else if(sData.Compare("20") == 0)
		{
			nReturnSize = 37;
			nAdcType = ADC_RESULT_READ;
		}
		else if(sData.Compare("90") == 0) 
		{
			nReturnSize = 22;
			nAdcType = ADC_MODE_OUT;
		}
		else 
		{
			nReturnSize = 22;
			nAdcType = NORMAL_TVCMD;
		}
	}
	else
	{
		nReturnSize = 21;
		nAdcType = NORMAL_TVCMD;
	}

	if(!CurrentSet->bTVCommPortEcho)
	{
		nReturnSize -= 9;
	}

	if(m_bResultRead && (nAdcType == ADC_RESULT_READ))
	{
		goto ADC_RESULT_CHECK;
	}

	PortClear();

	_Wait(100);

	//==============
	// Send Command
	//==============
	if(!SendRun(sCmd1, sCmd2, nSetId, sData))
	{
		m_nResult = TEST_FAIL; goto END_THREAD;
	}
//	_Wait(100);

	//==================
	// Receive Response
	//==================
	if(!ReceiveCommString(nReturnSize, nMaxDelay, sReadData))
	{
		m_nResult = TEST_TIMEOUT; goto END_THREAD;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}
	
	if(nAdcType == ADC_RESULT_READ)
	{
		//add 090423
		pCurStep->m_bAdcValueCheckStep = TRUE;
		goto ADC_RESULT_CHECK;
	}

	sReadData.MakeUpper();
	//if (sReadData.Find("NO") >= 0)
	//{
	//	AddStringToStatus("ACK [NO] ");
	//}
	//else if (sReadData.Find("OK") >= 0)
	//{
	if (sReadData.Find("OK") != -1)
	{

		m_nResult = TEST_PASS;
	}
	else if (sReadData.Find("TO") != -1) 
	{
		m_nResult = TEST_PASS;
		AddStringToStatus("ACK [TO] ");
	}	
	else if  (sReadData.Find("NO") != -1)
	{
		m_nResult = TEST_PASS;
		AddStringToStatus("ACK [NO] ");
	}
	else
		m_nResult = TEST_FAIL;

	goto END_THREAD;


ADC_RESULT_CHECK :

	//090508
	if(nCheckType <= 3){
		nCheckItem = nCheckType;
	}
	else{
		nCheckItem = nCheckType - 3;
	}
	//================
	// Check Response
	//================
	if(!m_bResultRead)
	{
		if(sReadData.GetLength() < 36)
		{
			m_nResult = TEST_FAIL;
			goto END_THREAD;
		}

		nCnt = 0;
		for(nTmp = 0; nTmp < 9; nTmp++)
		{
			m_nResultData[nTmp] = 0;

			sTmp.Format("%c%c%c%c", sReadData.GetAt(nCnt),
									sReadData.GetAt(nCnt + 1),
									sReadData.GetAt(nCnt + 2),
									sReadData.GetAt(nCnt + 3));
			//090508
			if(nCheckType <= 3){
				m_nResultData[nTmp] = hexCstr2decNum(sTmp);
			}
			else{
				m_nResultData[nTmp] = atoi(sTmp);
			}
			nCnt += 4;
		}
		m_bResultRead = TRUE;
	}
	

	if(nCheckItem == ADC_OFFSET1)
	{
		nRedData = m_nResultData[0]; nGreenData = m_nResultData[1]; nBlueData = m_nResultData[2];
		nTargetData1 = m_nResultTarget[0];
		nTargetData2 = m_nResultTarget[1];
		nTargetData3 = m_nResultTarget[2];
		nTargetMargin = m_nResultMargin[0];
		
	}
	else if(nCheckItem == ADC_OFFSET2)
	{
		nRedData = m_nResultData[3]; nGreenData = m_nResultData[4]; nBlueData = m_nResultData[5];
		nTargetData1 = m_nResultTarget[3];
		nTargetData2 = m_nResultTarget[4];
		nTargetData3 = m_nResultTarget[5];
		nTargetMargin = m_nResultMargin[1];
	}
	else if(nCheckItem == ADC_GAIN)
	{
		nRedData = m_nResultData[6]; nGreenData = m_nResultData[7]; nBlueData = m_nResultData[8];
		nTargetData1 = m_nResultTarget[6];
		nTargetData2 = m_nResultTarget[7];
		nTargetData3 = m_nResultTarget[8];
		nTargetMargin = m_nResultMargin[2];
	}

	if(m_nResultCheckType == 0)
	{
		//090508
		if(nCheckType <= 3){
			nTarget = hexCstr2decNum(m_strTargetAvg);
		}
		else{
			nTarget = atoi(m_strTargetAvg);
		}

		bTmp1 = Check_Value(nRedData, nTarget, m_nAvgMargin);
		bTmp2 = Check_Value(nGreenData, nTarget, m_nAvgMargin);
		bTmp3 = Check_Value(nBlueData, nTarget, m_nAvgMargin);


		if(bTmp1 && bTmp2 && bTmp3)
		{
			m_nResult = TEST_PASS;
		}
		else
		{
			m_nResult = TEST_FAIL;
		}
			//add 090423
		pCurStep->m_szAdcValue.Format("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,x,x,%d",m_nResultData[0],m_nResultData[1],m_nResultData[2],m_nResultData[3],
	                                  m_nResultData[4],m_nResultData[5],m_nResultData[6],m_nResultData[7],m_nResultData[8], nTarget, m_nAvgMargin);
	}
	else
	{
		bTmp1 = Check_Value(nRedData, nTargetData1, nTargetMargin);
		bTmp2 = Check_Value(nGreenData, nTargetData2, nTargetMargin);
		bTmp3 = Check_Value(nBlueData, nTargetData3, nTargetMargin);

		nMaxValue = max(max(nRedData,nGreenData),nBlueData);
		nMinValue = min(min(nRedData,nGreenData),nBlueData);
		nDiff	  = nMaxValue - nMinValue; 

		bTmp4 = Check_Value(nDiff, 0, nTargetMargin);

		if(nCheckOption == 0){
			if(bTmp1 && bTmp2 && bTmp3) {
				m_nResult = TEST_PASS;
			}
			else{
				m_nResult = TEST_FAIL;
			}
		}
		else if(nCheckOption == 1){
			if(bTmp4){
				m_nResult = TEST_PASS;
			}
			else {
				m_nResult = TEST_FAIL;
			}
		}
		else{
			if(bTmp1 && bTmp2 && bTmp3 && bTmp4) {
				m_nResult = TEST_PASS;
			}
			else{
				m_nResult = TEST_FAIL;
			}
		}
			//add 090423
		pCurStep->m_szAdcValue.Format("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",m_nResultData[0],m_nResultData[1],m_nResultData[2],m_nResultData[3],
	                                  m_nResultData[4],m_nResultData[5],m_nResultData[6],m_nResultData[7],m_nResultData[8],
									  nTargetData1, nTargetData2, nTargetData3, nTargetMargin);
	}

END_THREAD :

	m_bThreadRunning = FALSE;


	return m_nResult;
}

void CTVCommCtrl::ResetArray()
{
	m_strErrMsg		= _T("");	
	m_aCmd1.RemoveAll();
	m_aCmd2.RemoveAll();
	m_aSetId.RemoveAll();
	m_aData.RemoveAll();
	m_aWaitLimit.RemoveAll();
	
	m_nCheckType	= 0;	
	m_strTargetAvg  = _T("");	
	m_nAvgMargin    = 0;	
	
	// add 090417
	m_nResultCheckType      = 0;
	memset(m_nResultTarget, 0, 9);
	memset(m_nResultMargin, 0, 3);
	memset(m_nResultData, 0, 9);
	m_bResultRead = FALSE;
	m_aCheckType.RemoveAll();
	m_aCheckOption.RemoveAll();
}


BOOL CTVCommCtrl::Firmware_VersionCheck(int nIndex, CString sVersion)
{
	CString sData;
	CString sReadData;
	CString sTemp;
	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	switch(nIndex)
	{
		case CPU_VER:
			CurrentSet->sCPUVersionRead = "";
			sData = "00";
			break;

		case MICOM_VER:
			sData = "01";
			break;
		case USB_VER:
			sData = "02";
			break;

		case MNT_VER:
			sData = "03";
			break;

		default :
			return FALSE;
			break;
	}

	PortClear();
	if(!SendRun("a", "v", 0, sData))
	{
		return FALSE;
	}
	_Wait(100);

	if(!ReceiveCommString(14, 1500, sReadData))
	{
		return FALSE;
	}
	//090522
	int nI;
	nI = sReadData.Find(0x0d);
	if(nI != -1)
	{
		szMsg1 = sReadData.Left(nI + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	if(sReadData.Find("NG") != -1)
	{
		return FALSE;
	}
	nTemp = sReadData.Find("OK")+2 ;
	sReadData.Delete(0, nTemp);
	sReadData.TrimRight('X'); 
//	sReadData.TrimLeft('0'); 

	// change 091208
	if(sReadData.GetLength() > 2){
		sReadData.Insert(2, '.');
	}

	if(sReadData.GetLength() > 5){
		sReadData.Insert(5, '.');
	}

	if(sReadData.GetLength() > 8){
		sReadData.Insert(8, '.');
	}

/*	if(sVersion.Left(1).Compare("0") != 0){
		if(sReadData.Left(1).Compare("0") == 0)
		{
			sReadData.TrimLeft('0');
		}
	}
	if(nIndex == CPU_VER)
	{
		if(sVersion.GetLength() <= 8)
		{
			sTemp = sReadData.Left(sVersion.GetLength());
			sReadData = sTemp;
		}
	}

	if(nIndex == MICOM_VER)
	{
		sTemp = sVersion.Right(sVersion.GetLength()-sVersion.ReverseFind('.')-1);
		if(sTemp.GetLength() == 3)
		{
			nTemp = sReadData.ReverseFind('.') + 1 ;
			sReadData.Insert(nTemp, '0');
		}
	}
*/
//-
	CurrentSet->sCPUVersionRead = sReadData;
	sVersion.MakeUpper(); 
//	if(sReadData.Compare(sVersion) == 0){
	if(Check_Version(sReadData, sVersion)){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

BOOL CTVCommCtrl::ADC_Adj_ResultCheck(int nWait)
{
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	CString sTemp1;
	CString sTemp2;
	int nIndex;
	int nAdjItem = 0;
	int nOkItem =0;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC Adj Result Check");
	}

	PortClear();
	if(!SendRun("a", "d", 0, "99"))
	{
		return FALSE;
	}

	if(!ReceiveCommString(14, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );

	if(sReadData.Find("OK") == -1)
	{
		bResult1 = FALSE;
	}
	sReadData.TrimRight('X'); 
	
	sTemp2 = sReadData.Right(5);
	sTemp1 = sReadData.Right(2);

	nOkItem = hexCstr2decNum(sTemp1);
	sTemp1 = sTemp2.Left(2);
	nAdjItem = hexCstr2decNum(sTemp1);

	if(nOkItem != nAdjItem)
	{
		bResult2 = FALSE;
	}

	return (bResult1 && bResult2);
}

BOOL CTVCommCtrl::EDID_Download(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "EDID_Download Start");
	}

	PortClear();
	sIndex.Format("%02x", 0x10);
	if(!SendRun("a", "e", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//+add 090325
BOOL CTVCommCtrl::LAN_Port_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "MAC Address/Ping Test");
	}

	PortClear();
	sIndex.Format("%02x", 0x10);
	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::ESN_Download_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ESN Download Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x10);
	if(!SendRun("a", "s", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

	//+add PSH 080520
BOOL CTVCommCtrl::TVLink_SetChannel_PAL(int nRFSource, int nCH, int nWait)
{
	CString sTemp1;
	CString sTemp2;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	sTemp1.Format("%04x", nCH);
	sTemp2 = sTemp1.Right(4);
	sTemp1 = sTemp2.Left(2);
	sTemp2 = sTemp2.Right(2); 
	m_strSendStr.Format("ma 00 %s %s %d0%c", sTemp1, sTemp2, nRFSource, 0x0d);

	PortClear();
	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}

	if(!ReceiveCommString(11, nWait, sReadData))
	{
		return FALSE;
	}
	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") == -1)
	{
		return FALSE;
	}
	
	return TRUE;

}

BOOL CTVCommCtrl::Send_TVControl_ACAS_ID(CString sCommand, CString &sRevString, int nWait)
{
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	CString sCheckStr = _T("");

	m_strSendStr.Format("%s%c", sCommand, 0x0d);

	PortClear();


	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	if(!SendCommString(m_strSendStr))
	{
		
		AddStringToStatus("Fail to read ACAS ID: Send Fail");
		
		return FALSE;
	}
	sCheckStr = m_strSendStr;

	

	if(!ReceiveCommString(27, nWait, sReadData))
	{
			AddStringToStatus("Fail to read ACAS ID: Receive Fail");
		
		return FALSE;
	}
	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	sReadData.MakeUpper();

	if(sReadData.Find("OK") == -1)
	{
		AddStringToStatus("ACAS_ID Data Error: Not OK: "+ sReadData);
		return FALSE;
	}
	else
	{
		sRevString = sReadData;
	}
	
	
	return TRUE;
}
BOOL CTVCommCtrl::Send_TVControl_Command(CString sCommand, int nWait)
{
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	CString sCheckStr = _T("");

	m_strSendStr.Format("%s%c", sCommand, 0x0d);

	PortClear();


	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	if(!SendCommString(m_strSendStr))
	{
		if (sCheckStr.Find("aa 00 00") >= 0)
		{
			if (g_ID_Check == 0)
			{

				AddStringToStatus("Fail to Ready PCB ID: Send Fail");
			}
		}
		return FALSE;
	}
	sCheckStr = m_strSendStr;

	

	if(!ReceiveCommString(11, nWait, sReadData))
	{
		if (sCheckStr.Find("aa 00 00") >= 0)
		{
			if (g_ID_Check == 0)
			{

				AddStringToStatus("Fail to Ready PCB ID: Receive Fail");
			}
		}
		return FALSE;
	}


	if (sCheckStr.Find("aa 00 00") >= 0)
	{
		g_ID_Check = 1;
		/*if (g_ID_Check == 0)
		{
			
			AddStringToStatus("Ready to Ready PCB ID");
		}*/
	}	
	
	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	sReadData.MakeUpper();

	if(sReadData.Find("NO") >= 0)
	{
		AddStringToStatus("ACK [NO] ");
	}
	else if (sReadData.Find("OK") >= 0)
	{
		if (g_ID_Check == 1)
		{
			g_ID_Check = 2;
			AddStringToStatus("Ready to Read PCB ID");
		}		
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CTVCommCtrl::Write_Option(int nIndex, int nOptionValue)
{
	CString sCmd;
	CString sData;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	CString sTemp1;
	CString sTemp2;
	int nI;
	int nReturnOptionValue = 0;
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResult3 = TRUE;
	BOOL bCheckCrc = FALSE;

	switch(nIndex)
	{
		case TOOL_OPTION1:
			sCmd = "ab";
			break;

		case TOOL_OPTION2:
			sCmd = "ac";
			break;
		case TOOL_OPTION3:
			sCmd = "af";
			break;
		case TOOL_OPTION4:
			sCmd = "ag";
			break;
		case TOOL_OPTION5:   //091016
			sCmd = "aj";
			break;
		case TOOL_OPTION6:   //091016
			sCmd = "ap";
			break;
		case TOOL_OPTION7:   //091016
			sCmd = "aq";
			break;
		case TOOL_OPTION8:   //091016
			sCmd = "ay";
			break;
		case AREA_OPTION1:
			sCmd = "ah";
			break;
		case COMMERCIAL_OPTION1:
			sCmd = "ca";
			break;

		case COUNTRY_GROUP:
			sCmd = "ak";
			break;

		case COUNTRY:
			sCmd = "an";
			break;

		default :
			return FALSE;
			break;
	}

	if(nIndex == COUNTRY_GROUP){
		sData.Format("%02x", nOptionValue);
	}
	else if(nIndex == COUNTRY){
		sData.Format("%08x", nOptionValue);
		sData.Insert(2, ' ');
		sData.Insert(5, ' ');
		sData.Insert(8, ' ');
	}
	else{
		if(nOptionValue > 0xFFFF){
			bCheckCrc = TRUE;
			sData.Format("%08x", nOptionValue);
			sData.Insert(2, ' ');
			sData.Insert(5, ' ');
			sData.Insert(8, ' ');
		}
		else{
			sData.Format("%04x", nOptionValue);
			sData.Insert(2, ' ');
		}
	}

	m_strSendStr.Format("%s 00 %s%c", sCmd, sData, 0x0d); 

	PortClear();
	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}

	if(!ReceiveCommString(13, 3000, sReadData))
	{
		return FALSE;
	}
	nI = sReadData.Find(0x0d);
	if(nI != -1)
	{
		szMsg1 = sReadData.Left(nI + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	if(sReadData.Find("OK") == -1)
	{
		bResult1 = FALSE;
	}

	sTemp1 = sCmd.Right(1);
	sTemp1.MakeUpper();
	sTemp2 = sReadData.Left(1);
	sTemp2.MakeUpper();
	if(sTemp1 != sTemp2){
		bResult2 = FALSE;
	}
	if(nIndex == COUNTRY_GROUP){
		if(sReadData.GetLength() < 9){return FALSE;}
		sTemp1 = sReadData.Mid(7,2);
	}
	else if(nIndex == COUNTRY){
		if(sReadData.GetLength() < 15){return FALSE;}
		sTemp1 = sReadData.Mid(7,8);
	}
	else{
		if(bCheckCrc){
			if(sReadData.GetLength() < 15){return FALSE;}
			sTemp1 = sReadData.Mid(7,8);
		}
		else{
			if(sReadData.GetLength() < 11){return FALSE;}
			sTemp1 = sReadData.Mid(7,4);
		}
	}

	nReturnOptionValue = hexCstr2decNum(sTemp1);
//	nReturnOptionValue = atoi(sTemp1);
	if(nReturnOptionValue != nOptionValue){
		bResult3 = FALSE;
	}
	if(bResult1 && bResult2 && bResult3){
		return TRUE;
	}
	else{
		return FALSE;
	}
}
BOOL CTVCommCtrl::Write_OptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6, int nOption7)
{
	CString sCmd;
	CString sData1;
	CString sData2;
	CString sData3;
	CString sData4;
	CString sData5;
	CString sData6;
	CString sData7;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nI;

	sCmd = "at";

	sData1.Format("%04x", nOption1);
	sData1.Insert(2, ' ');
	sData2.Format("%04x", nOption2);
	sData2.Insert(2, ' ');
	sData3.Format("%04x", nOption3);
	sData3.Insert(2, ' ');
	sData4.Format("%04x", nOption4);
	sData4.Insert(2, ' ');
	sData5.Format("%04x", nOption5);
	sData5.Insert(2, ' ');
	sData6.Format("%04x", nOption6);
	sData6.Insert(2, ' ');
	sData7.Format("%04x", nOption7);
	sData7.Insert(2, ' ');

	m_strSendStr.Format("%s 00 %s %s %s %s %s %s %s%c", sCmd, sData1, sData2, sData3, sData4, sData5, sData6, sData7, 0x0d); 

	PortClear();
	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}

	if(!ReceiveCommString(13, 3000, sReadData))
	{
		return FALSE;
	}
	nI = sReadData.Find(0x0d);
	if(nI != -1)
	{
		szMsg1 = sReadData.Left(nI + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") == -1)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CTVCommCtrl::Check_Option(int nIndex, int nOptionValue)
{
	CString sCmd;
	CString sData;
	CString sReadData;
//	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	CString sTemp1;
	CString sTemp2;
//	int nCountryGr;
	int nI;
	int nReturnOptionValue = 0;
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResult3 = TRUE;
	BOOL bCheckCrc = FALSE;

	switch(nIndex)
	{
		case TOOL_OPTION1:
			sCmd = "ab";
			break;

		case TOOL_OPTION2:
			sCmd = "ac";
			break;
		case TOOL_OPTION3:
			sCmd = "af";
			break;
		case TOOL_OPTION4:
			sCmd = "ag";
			break;
		case TOOL_OPTION5:
			sCmd = "aj";
			break;
		case TOOL_OPTION6:
			sCmd = "ap";
			break;
		case TOOL_OPTION7:
			sCmd = "aq";
			break;
		case TOOL_OPTION8:
			sCmd = "ay";
			break;
		case AREA_OPTION1:
			sCmd = "ah";
			break;
		case COMMERCIAL_OPTION1:
			sCmd = "ca";
			break;

		case COUNTRY_GROUP:
			sCmd = "ak";
			break;

		case COUNTRY:
			sCmd = "an";
			break;

		default :
			return FALSE;
			break;
	}

	if(nIndex == COUNTRY_GROUP){
		sData.Format("%02x", nOptionValue);
	}
	else if(nIndex == COUNTRY){
		
		sData.Format("%08x", nOptionValue);
	}
	else{
		if(nOptionValue > 0xFFFF){
			bCheckCrc = TRUE;
			sData.Format("%08x", nOptionValue);
		}
		else{
			sData.Format("%04x", nOptionValue);
		}
	}

//	sData.Format("%04X", nOptionValue);
//	sData.Insert(2, ' ');

	m_strSendStr.Format("%s 00 ff%c", sCmd, 0x0d); 

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}

	PortClear();
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}

	if(!ReceiveCommString(13, 3000, sReadData))
	{
		return FALSE;
	}
	//090522
	nI = sReadData.Find(0x0d);
	if(nI != -1)
	{
		szMsg1 = sReadData.Left(nI + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	sData.MakeUpper(); 
	AddStringToStatus("Com: " + sReadData + "," + sData);

	sReadData.MakeUpper( );
	if(sReadData.Find("NG") != -1)
	{
		bResult1 = FALSE;
	}

	sTemp1 = sCmd.Right(1);
	sTemp1.MakeUpper();
	sTemp2 = sReadData.Left(1);
	sTemp2.MakeUpper();

	if(sTemp1 != sTemp2){
		bResult2 = FALSE;
	}
	if(nIndex == COUNTRY_GROUP){
		if(sReadData.GetLength() < 9){return FALSE;}
		sTemp1 = sReadData.Mid(7,2);
	}
	else if(nIndex == COUNTRY){
		if(sReadData.GetLength() < 15){return FALSE;}
		sTemp1 = sReadData.Mid(7,8);
	}
	else{
		if(bCheckCrc){
			if(sReadData.GetLength() < 15){return FALSE;}
			sTemp1 = sReadData.Mid(7,8);
		}
		else{
			if(sReadData.GetLength() < 11){return FALSE;}
			sTemp1 = sReadData.Mid(7,4);
		}
	}

//	nReturnOptionValue = atoi(sTemp1);
	nReturnOptionValue = hexCstr2decNum(sTemp1);
	if(nReturnOptionValue != nOptionValue){
		bResult3 = FALSE;
	}
	if(bResult1 && bResult2 && bResult3){
		return TRUE;
	}
	else{
		return FALSE;
	}
}
BOOL CTVCommCtrl:: Input_Detect_Check(int nIndex, int nOptionValue)
{
	CString sCmd;
	CString sData;
	CString sReadData;
//	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	CString sTemp1;
	CString sTemp2;
//	int nCountryGr;
	int nI;
	int nData = 0;
	int nReData = 0;
	int nReturnOptionValue = 0;
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResult3 = TRUE;
	BOOL bResult4 = TRUE;

	sCmd = "ai";

	switch(nIndex)
	{
		case  JACK_AV1:
			nData = 0x61;
			break;
			
		case  JACK_AV2 :
		 	nData = 0x62;
			break;
			
		case  JACK_SCART :
		 	nData = 0x63;
			break;
			
		case  JACK_COMP1 :
		 	nData = 0x64;
			break;
			
		case  JACK_COMP2 :
		 	nData = 0x65;
			break;
			
		case  JACK_USB1 :
		 	nData = 0x66;
			break;
			
		case  JACK_USB2 :
		 	nData = 0x67;
			break;
			
		case  JACK_USB3 :
		 	nData = 0x68;
			break;
			
		case  JACK_USB4 :
		 	nData = 0x69;
			break;
			
		case  JACK_HEAD_PHONE :
		 	nData = 0x6A;
			break;
			
		case  JACK_HDMI1 :
		 	nData = 0x6B;
			break;
			
		case  JACK_HDMI2 :
		 	nData = 0x6C;
			break;
			
		case  JACK_HDMI3 :
		 	nData = 0x6D;
			break;
			
		case  JACK_HDMI4 :
			nData = 0x6E;
			break;

		case  JACK_RGB:
			nData = 0x51;
			break;
			
		case  JACK_DVI :
		 	nData = 0x52;
			break;
			
		case  JACK_DP :
		 	nData = 0x53;
			break;
			
		case  JACK_SMHL :
		 	nData = 0x54;
			break;

		default :
			return FALSE;
			break;
	}

	m_strSendStr.Format("%s 00 %02x%c", sCmd, nData, 0x0d); 

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}

	PortClear();
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}

	if(!ReceiveCommString(13, 3000, sReadData))
	{
		return FALSE;
	}
	//090522
	nI = sReadData.Find(0x0d);
	if(nI != -1)
	{
		szMsg1 = sReadData.Left(nI + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	sData.MakeUpper(); 
	AddStringToStatus("Com: " + sReadData + "," + sData);

	sReadData.MakeUpper( );
	if(sReadData.Find("NG") != -1)
	{
		bResult1 = FALSE;
	}

	if(sReadData.GetLength() < 11){return FALSE;}

	sTemp1 = sCmd.Right(1);
	sTemp1.MakeUpper();
	sTemp2 = sReadData.Left(1);
	sTemp2.MakeUpper();

	if(sTemp1 != sTemp2){
		bResult2 = FALSE;
	}

	sTemp1 = sReadData.Mid(7,2);

	nReData = hexCstr2decNum(sTemp1);
	if(nReData != nData){
		bResult3 = FALSE;
	}

	sTemp1 = sReadData.Mid(9,2);
	nReturnOptionValue = atoi(sTemp1);
	if(nReturnOptionValue != nOptionValue){
		bResult4 = FALSE;
	}
	if(bResult1 && bResult2 && bResult3 && bResult4){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::Auto_Port_Detect(int nIndex)
{
	CString sCmd;
	CString sData;
	CString sReadData;
//	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	CString sTemp1;
	CString sTemp2;
//	int nCountryGr;
	int nI;
	int nData = 0;
	int nReData = 0;
	int nReturnOptionValue = 0;
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResult3 = TRUE;
	BOOL bResult4 = TRUE;

	//BYTE lCmdData[AUTO_COUNT_ID_MAX] = { 0x05, 0x06, 0x02, 0x03, 0x04, 0x01, 0x08, 0x07 };
	BYTE lCmdData[AUTO_COUNT_ID_MAX] = { 0x05, 0x06, 0x02, 0x03, 0x04, 0x01 };
	//#define AUTO_COUNT_ID_HEADPHONE	0	// 0x05,
//#define AUTO_COUNT_ID_GREENEYE	1	//0x06,
//#define AUTO_COUNT_ID_COMPONENT	2	//0x02,
//#define AUTO_COUNT_ID_AV		3		//0x03,
//#define AUTO_COUNT_ID_USB		4		//0x04, 
//#define AUTO_COUNT_ID_HDMI		5	//0x01,
//#define AUTO_COUNT_ID_RGB		6		//0x08
//#define AUTO_COUNT_ID_SCART		7	//0x07,

	sCmd = "XC";

	if (nIndex >= AUTO_COUNT_ID_MAX)
	{
		for(int i = 0; i < AUTO_COUNT_ID_MAX; i++)
		{
			m_strSendStr.Format("XC 00 %02x%c", lCmdData[i], 0x0d);

			if (CurrentSet->bCommDisplay)
			{
				AddStringToStatus("TV-Com : " + m_strSendStr);
			}

			PortClear();
			if (!SendCommString(m_strSendStr))
			{
				return FALSE;
			}

			if (!ReceiveCommString(10, 3000, sReadData))
			{
				return FALSE;
			}
			//090522
			nI = sReadData.Find("x");
			if (nI != -1)
			{
				szMsg1 = sReadData.Left(nI + 1);
			}
			else {
				szMsg1 = sReadData;
			}


			sData.MakeUpper();
			AddStringToStatus("Com: " + sReadData + "," + sData);

			sReadData.MakeUpper();
			int lpos = sReadData.Find("OK");
			if (lpos == -1)
			{
				return FALSE;
			}

			if (sReadData.GetLength() < 6) { return FALSE; }

			m_AutoCountPort[i] = _ttoi(sReadData.Mid(lpos + 2, 2));
		}

	}
	else if (nIndex >= 0)
	{
		//
		m_strSendStr.Format("XC 00 %02x%c",  lCmdData[nIndex], 0x0d);

		if (CurrentSet->bCommDisplay)
		{
			AddStringToStatus("TV-Com : " + m_strSendStr);
		}

		PortClear();
		if (!SendCommString(m_strSendStr))
		{
			return FALSE;
		}

		if (!ReceiveCommString(10, 3000, sReadData))
		{
			return FALSE;
		}
		//090522
		nI = sReadData.Find("x");
		if (nI != -1)
		{
			szMsg1 = sReadData.Left(nI + 1);
		}
		else {
			szMsg1 = sReadData;
		}


		sData.MakeUpper();
		AddStringToStatus("Com: " + sReadData + "," + sData);

		sReadData.MakeUpper();
		int lpos = sReadData.Find("OK");
		if(lpos == -1)
		{
			return FALSE;
		}

		if (sReadData.GetLength() < 6) { return FALSE; }

		m_AutoCountPort[nIndex] = _ttoi(sReadData.Mid(lpos+2, 2));
	}
	return TRUE;
	

}


BOOL CTVCommCtrl::Check_OptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6, int nOption7)
{
	CString sCmd;
	CString sData;
	CString sBuf;
	CString sReadData;
	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	sCmd = "at";

	sData.Format("%04x%04x%04x%04x%04x%04x%04x", nOption1, nOption2, nOption3, nOption4, nOption5, nOption6, nOption7);

	m_strSendStr.Format("%s 00 ff%c", sCmd, 0x0d); 

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}

	PortClear();
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}

	if(!ReceiveCommString(13, 3000, sReadData))
	{
		return FALSE;
	}
	//090522
	int nI;
	nI = sReadData.Find(0x0d);
	if(nI != -1)
	{
		szMsg1 = sReadData.Left(nI + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	if(sReadData.Find("NG") != -1)
	{
		return FALSE;
	}
	nTemp = sReadData.Find("OK")+2 ;
	sReadData.Delete(0, nTemp);
	sReadData.TrimRight('X'); 

	sReadData.TrimRight(' '); 
	sReadData.TrimLeft(' '); 

	sData.MakeUpper(); 
	if(sReadData.Compare(sData) == 0){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

BOOL CTVCommCtrl::InputSourceChange(BYTE nCode)
{
	CString sCmd;
	CString sData;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	m_strSendStr.Format("xb 00 %02x%c", nCode, 0x0d); 

	PortClear();
	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}

	if(!ReceiveCommString(10, 3000, sReadData))
	{
		return FALSE;
	}
	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	if(sReadData.Find("NG") != -1)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CTVCommCtrl::PortClear()
{
	int nRev;
//	CString sTVComPort;
//	int nPortNo;

	nRev = m_ctrlTVCommCtrl.ClearPort();
	
/*	nPortNo = m_ctrlTVCommCtrl.m_wPortID;

	if((nPortNo >= 1) && (nPortNo <= 9))
	{
		sTVComPort.Format("COM%d",nPortNo);
	}
	else if((nPortNo >= 10) && (nPortNo <= 16))
	{
		sTVComPort.Format("\\\\.\\COM%d",nPortNo);
	}
	else
		return FALSE;

	if(nRev == 0)
	{
		PortClose();
		
		_Wait(500);
		if(Create(sTVComPort,m_ctrlTVCommCtrl.dwBaudRate) == FALSE)
		{
			PortClose();

			if(Create(sTVComPort,m_ctrlTVCommCtrl.dwBaudRate) == FALSE)
			{
				return FALSE;
			}
		}
	}	
*/
	return TRUE;
}

//+add 090415
BOOL CTVCommCtrl::Set_Mic_OnOff(BOOL bFlag, int nWait)
{
	CString sIndex;
	CString sReadData;
	CString sTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(bFlag)
	{
		sTemp = _T("Mic On");
		sIndex.Format("%02x", 0x10);
	}
	else
	{
		sTemp = _T("Mic Off");
		sIndex.Format("%02x", 0x20);
	}

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sTemp);
	}

	PortClear();
	if(!SendRun("a", "m", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}
	//+add PSH 090417
BOOL CTVCommCtrl::Check_Value(int nData, int nTarget, int nMargin)
{
	int nMin = nTarget - nMargin;
	int nMax = nTarget + nMargin;

	if(	(nMin <= nData) && (nData <= nMax))
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}


	//+add PSH 091118
BOOL CTVCommCtrl::CI_Download_Check1(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "CI+Key Download Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x10);
	if(!SendRun("c", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else{
		Result = FALSE;
	}

	return Result;
}

	//+add PSH 091118
BOOL CTVCommCtrl::CI_Download_Check2(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;
	BOOL Result2;
	CString sTemp;

	PortClear();
	sIndex.Format("%02x", 0x20);
	if(!SendRun("c", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(14, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	sReadData.TrimRight('X'); 

	if(sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else{
		Result = FALSE;
	}
	
	sTemp = sReadData.Right(16);

	if(sTemp == "0000000000000000")
	{
		Result2 = FALSE;
	}
	else{
		Result2 = TRUE;
	}

	if(Result && Result2)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}
//+add PSH 101011
BOOL CTVCommCtrl::CI_Card_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;
	CString sTemp;

	PortClear();
	sIndex.Format("%02x", 0x30);
	if (!SendRun("c", "i", 0, sIndex))
	{
		return FALSE;
	}
	if (!ReceiveCommString(14, nWait, sReadData))
	{
		return FALSE;
	}

	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if (nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else {
		szMsg1 = sReadData;
	}


	sReadData.MakeUpper();
	sReadData.TrimRight('X');

	if (sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else {
		Result = FALSE;
	}

	return Result;
}



BOOL CTVCommCtrl::CI_ECP_Download_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;
	CString sTemp;

	PortClear();
	sIndex.Format("%02x", 0x40);
	if (!SendRun("c", "i", 0, sIndex))
	{
		return FALSE;
	}
	if (!ReceiveCommString(14, nWait, sReadData))
	{
		return FALSE;
	}

	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if (nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else {
		szMsg1 = sReadData;
	}
	
	sReadData.MakeUpper();
	sReadData.TrimRight('X');

	if (sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else 
	{
		Result = FALSE;
	}

	return Result;
}



BOOL CTVCommCtrl::Marlin_Download_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;
	CString sTemp;

	PortClear();
	sIndex.Format("%02x", 0x20);
	if(!SendRun("a", "s", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(14, nWait, sReadData))
	{
		return FALSE;
	}

	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	sReadData.TrimRight('X'); 

	if(sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else{
		Result = FALSE;
	}
	
	return Result;
}
BOOL CTVCommCtrl::BuiltIn_Wifi_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;
	CString sTemp;

	PortClear();
	sIndex.Format("%02x", 0x20);
	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(14, nWait, sReadData))
	{
		return FALSE;
	}

	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	sReadData.MakeUpper( );
	sReadData.TrimRight('X'); 

	if(sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else{
		Result = FALSE;
	}
	
	return Result;
}

//+add 091124
BOOL CTVCommCtrl::RJP_Port_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "RJP Port Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x10);
	if(!SendRun("a", "r", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}
//+add 091124
BOOL CTVCommCtrl::Magic_Remocon_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "Magic Remocon Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x30);
	if(!SendRun("a", "m", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//+add 091124
BOOL CTVCommCtrl::Wireless_Ready_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "Wireless Ready Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x10);
	if(!SendRun("a", "w", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//+add 101108
BOOL CTVCommCtrl::Set_LNB_OnOff(BOOL bFlag, int nWait)
{
	CString sIndex;
	CString sReadData;
	CString sTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(bFlag)
	{
		sTemp = _T("LNB On");
		sIndex.Format("%02x", 0x30);
	}
	else
	{
		sTemp = _T("LNB Off");
		sIndex.Format("%02x", 0x40);
	}

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sTemp);
	}

	PortClear();
	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::Widevine_Download_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;
	CString sTemp;

	PortClear();
	sIndex.Format("%02x", 0x81);
	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(14, nWait, sReadData))
	{
		return FALSE;
	}

	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	sReadData.MakeUpper( );
	sReadData.TrimRight('X'); 

	if(sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else{
		Result = FALSE;
	}
	
	return Result;
}
//+add 101115
BOOL CTVCommCtrl::RF_Emitter_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "3D RF Emitter Module Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x40);
	if(!SendRun("a", "m", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//+add 101124
BOOL CTVCommCtrl::Set_3DMode_OnOff(BOOL bFlag, int nWait)
{
	CString sIndex;
	CString sReadData;
	CString sTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

	if(bFlag)
	{
		sTemp = _T("3DMode On");
		sIndex.Format("%02x", 0x50);
	}
	else
	{
		sTemp = _T("3DMode Off");
		sIndex.Format("%02x", 0x60);
	}

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sTemp);
	}

	PortClear();
	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

void CTVCommCtrl::Set_MAC_Address(CString sData)
{
	sData.MakeUpper();

	m_aMacAdd.Add(sData);
	
}

BOOL CTVCommCtrl::MAC_AddressCheck(int nWait)
{
	CString sData;
	CString sReadData;
	CString sMAC_Add;
	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL bResult = TRUE;
//	int nNoTest = m_aMacAdd.GetSize();
	int nNoTest = m_aMacAdd2.GetSize();
	m_ErrorType = "";

	sData = "70";
	PortClear();
	if(!SendRun("a", "i", 0, sData))
	{
		AddStringToStatus("Send Error"); //m_ErrorType = "Send Error";//godtech 180603
		return FALSE;
	}
	_Wait(100);

	if(!ReceiveCommString(14, 1500, sReadData))
	{
		AddStringToStatus("Rcv Error"); //m_ErrorType = "Send Error";//godtech 180603
		return FALSE;
	}
	//090522
	int nI;
	nI = sReadData.Find(0x0d);
	if(nI != -1)
	{
		szMsg1 = sReadData.Left(nI + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	if(sReadData.Find("NG") != -1)
	{
		AddStringToStatus("Rcv Data NG"); ////godtech 180603
		return FALSE;
	}
	nTemp = sReadData.Find("OK")+2 ;
	sReadData.Delete(0, nTemp);
	sReadData.TrimRight('X'); 
//	sReadData.TrimLeft('0'); 

	// change 091208
	if(sReadData.GetLength() > 2){
		sReadData.Insert(2, ':');
	}

	if(sReadData.GetLength() > 5){
		sReadData.Insert(5, ':');
	}

	if(sReadData.GetLength() > 8){
		sReadData.Insert(8, ':');
	}

	if(sReadData.GetLength() > 11){
		sReadData.Insert(11, ':');
	}
	
	if(sReadData.GetLength() > 14){
		sReadData.Insert(14, ':');
	}
//-

	if(sReadData.Find("00:00:00:00:00:00") != -1)
	{
		AddStringToStatus("Rcv Data Error: 00:00:00:00:00:00");//godtech 180603
		bResult = FALSE;
	}
	else if(sReadData.Find("FF:FF:FF:FF:FF:FF") != -1)
	{
		AddStringToStatus("Rcv Data Error: FF:FF:FF:FF:FF:FF");//godtech 180603
		bResult = FALSE;
	}
	else if(sReadData.Find("00:10:18:26:08:E0") != -1)
	{
		AddStringToStatus("Rcv Data Error: 00:10:18:26:08:E0");//godtech 180603
		bResult = FALSE;
	}
	else if(sReadData.Find("00:F0:FF:00:F0:FF") != -1)
	{
		AddStringToStatus("Rcv Data Error: 00:F0:FF:00:F0:FF");//godtech 180603
		bResult = FALSE;
	}
	if(!bResult){
		return FALSE;
	}

	sMAC_Add = sReadData.Left(8);
	if (nNoTest == 0)
	{
		AddStringToStatus("MAC nNoTest == 0");//godtech 180603
		return FALSE; 
	}
		
	bResult = FALSE;

	for(int i=0; i<nNoTest; i++)
	{
		//if (sMAC_Add.Compare(m_aMacAdd.GetAt(i)) == 0) {
		if(sMAC_Add.Compare(m_aMacAdd2.GetAt(i)) == 0){
			bResult = TRUE;
		}
	}
	if (!bResult)
	{

		AddStringToStatus("Not Match MAC");//godtech 180603
		AddStringToStatus(sMAC_Add);//godtech 180603
	//	m_aMacAdd.RemoveAll();
	}

	return bResult;
}

void CTVCommCtrl::Set_Chgeck_USB(UINT nUSBData)
{
	m_nHighSpeedCheckUSB = nUSBData;
}

BOOL CTVCommCtrl::USB_HighSpeedCheck(int nWait)
{
	CString sData;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BOOL Result;
	CString sTemp;

	PortClear();
	switch(m_nHighSpeedCheckUSB)
	{
		case 1:
			sData = "84";
			break;

		case 2:
			sData = "85";
			break;
		case 3:
			sData = "86";
			break;
		case 4:
			sData = "87";
			break;

		default :
			return FALSE;
			break;
	}

	if(!SendRun("a", "i", 0, sData))
	{
		return FALSE;
	}
	if(!ReceiveCommString(14, nWait, sReadData))
	{
		return FALSE;
	}

	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	sReadData.MakeUpper( );
	sReadData.TrimRight('X'); 

	if(sReadData.Find("OK") != -1)
	{
		Result = TRUE;
	}
	else{
		Result = FALSE;
	}
	
	return Result;
}


//+add 090415
BOOL CTVCommCtrl::Set_HeadPhone_OnOff(BOOL bFlag, int nWait)
{
	CString sIndex;
	CString sReadData;
	CString sTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(bFlag)
	{
		sTemp = _T("Head Phone On");
		sIndex.Format("%02x", 0x50);
	}
	else
	{
		sTemp = _T("Head Phone Off");
		sIndex.Format("%02x", 0x60);
	}

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sTemp);
	}

	PortClear();
	if(!SendRun("e", "s", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	//090522
	int nIndex;
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

//+add 110715
BOOL CTVCommCtrl::Key_Check(int nFlag)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "Key Check");
	}

	PortClear();
	if(nFlag ==1){
		sIndex.Format("%02x", 0x91);
	}
	else if(nFlag ==2){
		sIndex.Format("%02x", 0x92);
	}
	else{
		return FALSE;
	}

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}
//+add 110715
BOOL CTVCommCtrl::EyeQ_Check(int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "EyeQ Sensor Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x93);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}
BOOL CTVCommCtrl::BAT_Ver_Check(CString sBatVersion,int nWait)
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	//if(CurrentSet->bIsRunning)
	//{
	//	g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "EyeQ Sensor Check");
	//}

	PortClear();
	sIndex.Format("%02x", 0x31);

	if(!SendRun("a", "v", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, nWait, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}
#ifdef _NO_CONNECT_DEBUG__MODE
	sReadData = "OK 00 31x";
#endif
	if((sReadData.Find("OK") != -1)&&(sReadData.Find(CurrentSet->sBatVersion) != -1))
	{
		
		return TRUE;
		
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::CNTV_Key_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "CNTV Key D/L Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x85);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::HDCP_Tx_20_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "HDCP Tx Key D/L Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x86);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}


	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::HDCP_Rx_20_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "HDCP Rx Key D/L Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x87);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}
BOOL CTVCommCtrl::HDCP_Rx_14_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;
	
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "HDCP Rx Key D/L Check");
	}
	
	PortClear();
	sIndex.Format("%02x", 0x73);
	
	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}
	
	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}
	
	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::DTCP_Rx_Key_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "DTCP Rx Key D/L Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x88);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::ATSC_30_Key_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "DTCP Rx Key D/L Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x60);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}


BOOL CTVCommCtrl::MFi_Key_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "DTCP Rx Key D/L Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x50);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else if (sReadData.Find("NULL") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}


//BOOL CTVCommCtrl::MFi_Key_DL_Check()
//{
//	CString sIndex;
//	CString sReadData;
//	CString szMsg1 = _T("");
//	CString szPrevMsg = _T("");
//	int nIndex;
//
//	//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
//	if (CurrentSet->bIsRunning)
//	{
//		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "DTCP Rx Key D/L Check");
//	}
//
//	PortClear();
//	sIndex.Format("%02x", 0x50);
//
//	if (!SendRun("a", "i", 0, sIndex))
//	{
//		return FALSE;
//	}
//	if (!ReceiveCommString(4, 3000, sReadData))
//	{
//		return FALSE;
//	}
//
//	nIndex = sReadData.Find(0x0d);
//	if (nIndex != -1)
//	{
//		szMsg1 = sReadData.Left(nIndex + 1);
//	}
//	else {
//		szMsg1 = sReadData;
//	}
//
//	if (sReadData.Find("OK") != -1)
//	{
//		return TRUE;
//	}
//	else {
//		return FALSE;
//	}
//}



BOOL CTVCommCtrl::Google_Key_DL_Check()
{
	CString sIndex;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nIndex;

//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	if(CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "Google Key D/L Check");
	}

	PortClear();
	sIndex.Format("%02x", 0x89);

	if(!SendRun("a", "i", 0, sIndex))
	{
		return FALSE;
	}
	if(!ReceiveCommString(4, 3000, sReadData))
	{
		return FALSE;
	}

	nIndex = sReadData.Find(0x0d);
	if(nIndex != -1)
	{
		szMsg1 = sReadData.Left(nIndex + 1);
	}
	else{
		szMsg1 = sReadData;
	}

	if(sReadData.Find("OK") != -1)
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CTVCommCtrl::Check_Version(CString sReadData, CString sTarget)
{
	CStringArray SplitIt_Read;
	CStringArray SplitIt_Target;
	int nSize_M;
	int nSize_T;
	int nValue_M;
	int nValue_T;
	BOOL bResult = TRUE;

	StringToken(sReadData, ".", SplitIt_Read, TRUE);
	StringToken(sTarget, ".", SplitIt_Target, TRUE);

	nSize_M = SplitIt_Read.GetSize();
	nSize_T = SplitIt_Target.GetSize();

	if(nSize_T ==0 || nSize_M == 0){return FALSE;}
	if(nSize_T > nSize_M){return FALSE;}

	for(int i = 0; i < nSize_T; i++)
	{
		nValue_M = _ttoi((const TCHAR*)SplitIt_Read.GetAt(i));
		nValue_T = _ttoi((const TCHAR*)SplitIt_Target.GetAt(i));

		if(nValue_M != nValue_T){
			bResult = FALSE;
		}
	}

	return bResult;
}

void CTVCommCtrl::SaveComLog(CString sComData)
{
	CString szPrevMsg = _T("");

	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
	if(szPrevMsg != _T(""))
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + sComData);
		pCurStep->m_strMsg = szPrevMsg + "; " + sComData;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sComData);
		pCurStep->m_strMsg = sComData;
	}		
}


//---------------------------------------------------------------------------
BOOL CTVCommCtrl:: ModelSNo_Write(CString sData, BOOL bFlag) // 모델명 또는 S/N 데이터를 송신
{
	CString sReadData;
	CString sWriteData;
	CString sTemp1;
	int nI;
	BOOL bREv = FALSE;

//	m_strSendStr.Format("%c%c%s%c", 0xa0, 0x8f, sData, 0x0d);
	if(bFlag){
		nI =  sData.GetLength();
		if(nI < 10) {return FALSE;}
		else if(nI > 10){sWriteData = sData.Left(10);}
		else{sWriteData = sData;}

//		m_strSendStr.Format("%c%c%c%c%s%c", 0xa0, 0x8f, 's', 'a', sData, 0x0d); 
		m_strSendStr.Format("%c%c%c%c%sFF%c", 0xa0, 0x90, 's', 'a', sWriteData, 0x0d); 
	}
	else{
		m_strSendStr.Format("%c%c%c%c%s%c", 0xa0, 0x8f, 's', 'b', sData, 0x0d); 
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	
	PortClear();
	if(!SendCommString(m_strSendStr))
	{
		return FALSE;
	}
//	Sleep(100);
	if(!ReceiveCommString_SN(2, 3000, sReadData))
	{
		return FALSE;
	}

	for(int i = 0; i< m_nReceiveLength; i++)
	{
		if(	m_nReceiveData[i] == 0xa0){
			bREv = TRUE;
		}
	}


	return bREv;
}
//---------------------------------------------------------------------------
int CTVCommCtrl:: ModelSNo_Read(int &nKeyVal, BOOL bFlag) // 모델명 또는 S/N 데이터를 수신
{
	CString sData = _T("");
	CString sReadData;
	CString sBuf;
	CString sTemp1;
	int nI;
	
//	m_strSendStr.Format("%c%c%c%c%c", 0xa1, 0x8f, 's', 'a', 0x0d); 
	if(bFlag){
		m_strSendStr.Format("%c%c%c%c%c", 0xa1, 0x8f, 's', 'a', 0x0d); 
	}
	else{
		m_strSendStr.Format("%c%c%c%c%c", 0xa1, 0x8f, 's', 'b', 0x0d); 
	}
	
	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}
	
	PortClear();
	if(!SendCommString(m_strSendStr))
	{
		return 0;
	}
	
	if(!ReceiveCommString_SN(12, 3000, sReadData))
	{
		return 0;
	}

//	nI = sReadData.Find(0x0d);
	nI = sReadData.Find("KEY");
	if(nI != -1)
	{
		sData = sReadData.Mid(nI, 11);
		sTemp1 = sData.Right(4);
		
		nKeyVal = hexCstr2decNum(sTemp1);

		return 1;
	}
	else{
		return 2;
	}

}


int CTVCommCtrl::ModelSNo_Read(int &nKeyVal, CString &sRtnData, BOOL bFlag) // 모델명 또는 S/N 데이터를 수신
{
	CString sData = _T("");
	CString sReadData;
	CString sBuf;
	CString sTemp1;
	int nI;

	//	m_strSendStr.Format("%c%c%c%c%c", 0xa1, 0x8f, 's', 'a', 0x0d); 
	if (bFlag) {
		m_strSendStr.Format("%c%c%c%c%c", 0xa1, 0x8f, 's', 'a', 0x0d);
	}
	else {
		m_strSendStr.Format("%c%c%c%c%c", 0xa1, 0x8f, 's', 'b', 0x0d);
	}

	if (CurrentSet->bCommDisplay)
	{
		AddStringToStatus("TV-Com : " + m_strSendStr);
	}

//////////////////////////////

	_Wait(700);
////////////////////////////////
	PortClear();
	if (!SendCommString(m_strSendStr))
	{
		return 0;
	}

	if (!ReceiveCommString_SN(12, 3000, sReadData))
	{
		return 0;
	}

	sRtnData = sReadData;

	//	nI = sReadData.Find(0x0d);
	nI = sReadData.Find("KEY");
	if (nI != -1)
	{
		sData = sReadData.Mid(nI, 11);
		sTemp1 = sData.Right(4);

		nKeyVal = hexCstr2decNum(sTemp1);

		return 1;
	}
	else {
		return 2;
	}

}

BOOL CTVCommCtrl::Open_MacAdd_List()
{
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sBuf;
	CString filename;
	CString sModel;

	CString sDay;
	CString sMonth;
	CString sTemp2;
	CStringArray SplitIt;

	filename.Format(_T("%s\\Config_Mac_Add.INI"), g_pView->m_szExePath);
	m_aMacAdd2.RemoveAll();

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	hFind = FindFirstFile(filename, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		sTemp.Format("%s\n File doesn't Exist!", filename);
		AfxMessageBox(sTemp);
		return FALSE;
	}

	FindClose(hFind);

	if (!stdFile.Open(filename, CFile::modeRead, &fileExcept))
	{
		fileExcept.ReportError();
		return FALSE;
	}
 	while (stdFile.ReadString(sTemp)) {
		sTemp.TrimLeft(' ');
		sTemp.TrimRight(' ');
		if (sTemp.Find("//") != -1)
		{
			continue;
		}
		sTemp.MakeUpper();
		m_aMacAdd2.Add(sTemp);
	}
	stdFile.Close();

	return TRUE;
}
//---------------------------------------------------------------------------