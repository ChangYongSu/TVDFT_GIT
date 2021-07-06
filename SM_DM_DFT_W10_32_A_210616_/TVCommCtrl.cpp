#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "TVCommCtrl.h"

CTVCommCtrl				TVCommCtrl;
extern CDATsysView*		g_pView;
extern CString			m_strWipId;	//Wip ID

//+change 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

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

	memset(m_nReceiveData, 0, 256);
	memset(m_nSendData, 0, 5);
	m_nReceiveLength = 0;

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

	m_sCheckCmd			= _T("");		
	m_nWaitLimit = 0;

	m_aMacAdd.RemoveAll();
	m_nIfTest_Data1 = 0;
	m_nIfTest_Data2 = 0;
	m_bCheckReadVersion = 0;

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
	CString sTmp = sComPort;
	sTmp.Delete(0, sTmp.Find("COM")+3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");

//	if( !m_ctrlTVCommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID) )
//	if( !m_ctrlTVCommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID, 8, 0, 0,TRUE) )
//	if( !m_ctrlTVCommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID, 8, NOPARITY, 0,TRUE) )
//	if( !m_ctrlTVCommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID, 8, EVENPARITY, 0,TRUE) )
	if( !m_ctrlTVCommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID, 8, CurrentSet->nTVParity, 0,TRUE) )
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

BOOL CTVCommCtrl::SendCommString(CString str)
{
	m_ctrlTVCommCtrl.m_QueueRead.Clear();

	::ResetEvent(m_hReadEvent);

	m_sReceive = _T("");

	int nRtn = m_ctrlTVCommCtrl.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());
	
	if(nRtn != str.GetLength()) return FALSE;

	return TRUE;
}
BOOL CTVCommCtrl::Send_ResultKey( BOOL bResult) 
{
	int i;
	BYTE nCheckSum = 0x00;
	int nRtn;
	CString szPrevMsg = _T("");
	CString szCmdString;
	m_sReceive = _T("");

	if(CurrentSet->nProcessType > 1){return TRUE;}

	for(int i=0; i<5; i++)
	{
		m_nSendData[i] = 0x00;
	}

	
	m_nSendData[0] = 0xFF;
	m_nSendData[1] = 0x03;
	if(CurrentSet->nProcessType == 0){
		m_nSendData[2] = 0x0A;
	}
	else{
		m_nSendData[2] = 0x0B;
	}

	if(bResult){
		m_nSendData[3] = 0x01;
	}
	else{
		m_nSendData[3] = 0x00;
	}

	nCheckSum	= m_nSendData[0] ^ m_nSendData[1] ^ m_nSendData[2] ^ m_nSendData[3];

	m_nSendData[4] = nCheckSum;


	szCmdString.Format("%02X %02X %02X %02X %02X", m_nSendData[0], m_nSendData[1], m_nSendData[2], m_nSendData[3], m_nSendData[4]);
	if(CurrentSet->bIsRunning && CurrentSet->bCommDisplay)
	{
		AddStringToStatus("DFT>:" + szCmdString);
	}
	::ResetEvent(m_hReadEvent);
	m_ctrlTVCommCtrl.m_QueueRead.Clear();
	memset(m_nReceiveData, 0, 225);

	nRtn = m_ctrlTVCommCtrl.WriteComm(m_nSendData, 5);

	
	if(nRtn != 5) return FALSE;

	return TRUE;
}

BOOL CTVCommCtrl::SendCommand(BYTE nCmd)
{
	int i;
	BYTE nCheckSum = 0x00;
	int nRtn;
	CString szPrevMsg = _T("");
	CString szCmdString;
	m_sReceive = _T("");

	for(int i=0; i<5; i++)
	{
		m_nSendData[i] = 0x00;
	}

	
	m_nSendData[0] = 0xAA;
	m_nSendData[1] = 0x03;
	m_nSendData[2] = CurrentSet->nChipType;
	m_nSendData[3] = nCmd;

	nCheckSum	= m_nSendData[0] ^ m_nSendData[1] ^ m_nSendData[2] ^ m_nSendData[3];

	m_nSendData[4] = nCheckSum;


	szCmdString.Format("%02X %02X %02X %02X %02X", m_nSendData[0], m_nSendData[1], m_nSendData[2], m_nSendData[3], m_nSendData[4]);
	if(CurrentSet->bIsRunning && CurrentSet->bCommDisplay)
	{
		AddStringToStatus("DFT>:" + szCmdString);
	}
	if((g_pView->m_Rs232cAdcManualCheckDlg.GetSafeHwnd() != NULL)
	&& (g_pView->m_Rs232cAdcManualCheckDlg.m_bActive == TRUE))
	{
		g_pView->m_Rs232cAdcManualCheckDlg.WriteLog("DFT>:" + szCmdString);
	}

	::ResetEvent(m_hReadEvent);
//	m_ctrlTVCommCtrl.ClearPort();
	m_ctrlTVCommCtrl.m_QueueRead.Clear();
	memset(m_nReceiveData, 0, 225);

	nRtn = m_ctrlTVCommCtrl.WriteComm(m_nSendData, 5);

	if(CurrentSet->bIsRunning)
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szCmdString);
			pCurStep->m_strMsg = szPrevMsg + " , " + szCmdString;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szCmdString);
			pCurStep->m_strMsg = szCmdString;
		}		
	}
	
	if(nRtn != 5) return FALSE;

	return TRUE;
}


BOOL CTVCommCtrl::ReceiveCommString(int nWaitLimit)
{
	BOOL	bDoing;
    clock_t	start;
	CString	sTemp;
	int     nPos = -1;
	int     nTemp;
	int     nTemp2;
	int		i, j;
	BYTE    nHeader = 0x00;
//	int		nRevLength = 0;
//	char	nReceiveData[256];
	MSG		msg;
	DWORD dwEventResult = 0;

	if(!m_bPortOpen) return FALSE;

	memset(m_nRevData, 0, 225);
	m_nRevLength = 0;

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

		dwEventResult = WaitForSingleObject(m_hReadEvent, nWaitLimit);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(m_hReadEvent);
		}

		if(m_nReceiveLength > 0)
		{
			if(m_nRevLength == 0)
			{
				for(nPos =0; nPos <256; nPos++)
				{
					nHeader = m_nReceiveData[nPos]; 
					if( nHeader == 0xbb){
						break;
					}
				}

				if(nPos < 255){
					nTemp = m_nReceiveData[nPos + 1];
				}
				else
				{
					return FALSE;
				}
				nTemp2 = nPos + nTemp + 1;


				if(nTemp2 > 255)
				{
					for( i= 0, j= nPos; j < 256; i++, j++)
					{
						m_nRevData[i] = m_nReceiveData[j];
					}
					m_nRevLength = i;

					if ((clock() - start)*1000/CLOCKS_PER_SEC <= nWaitLimit) 
					{	continue; }
				}
				else{
					for( i= 0, j= nPos; j <= nTemp2; i++, j++)
					{
						m_nRevData[i] = m_nReceiveData[j];
					}
					m_nRevLength = i;

					m_ctrlTVCommCtrl.m_QueueRead.Clear();
					m_strSendStr = _T("");

					return TRUE;
				}
			}			
			else{
				nTemp2 = nTemp + 2 - m_nRevLength;
				if((nTemp + 2) <= (m_nRevLength + m_nReceiveLength)){
					for( i= m_nRevLength, j= 0; j < nTemp2; i++, j++)
					{
						m_nRevData[i] = m_nReceiveData[j];
					}
					m_nRevLength = m_nRevLength  + nTemp2;

					m_ctrlTVCommCtrl.m_QueueRead.Clear();
					m_strSendStr = _T("");

					return TRUE;
				}
				else{
					for( i= m_nRevLength, j= 0; j < m_nReceiveLength; i++, j++)
					{
						m_nRevData[i] = m_nReceiveData[j];
					}
					m_nRevLength = m_nRevLength + m_nReceiveLength;

					if ((clock() - start)*1000/CLOCKS_PER_SEC <= nWaitLimit) 
					{	continue; }
				}
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


UINT CTVCommCtrl::IfTest(BYTE nCmd, int nMaxDelay, BOOL bIfCheck)
{
	CString sReadData = _T("");
	BYTE nCheckSum  = 0x00;
	int i;
	int nFind;
	CString szPrevMsg = _T("");
	CString szMsg1 = _T("");
	CString sData1 = _T("");
	CString sData2 = _T("");
	CString sTemp = _T("");

	m_nResult = TEST_PASS;

	m_nIfTest_Data1 = 1000;
	m_nIfTest_Data2 = 1000;
	//==============
	// Send Command
	//==============
	if(!SendCommand(nCmd))
	{
		m_nResult = TEST_FAIL;
		szMsg1 = "Send Fail";
		goto END_THREAD;
	}

	//20210120 Time //Sleep(1000);
	//==================
	// Receive Response
	//==================
	if(!ReceiveCommString(nMaxDelay))
	{
		m_nResult = TEST_TIMEOUT;
		szMsg1 = "Read TimeOut";
		goto END_THREAD;
	}

	for(i =0; i < m_nRevLength; i++)
	{
		sTemp.Format("%02X ", m_nRevData[i]);
		szMsg1 = szMsg1 + sTemp;
	}
	for(i =0; i < m_nRevLength - 1; i++)
	{
		nCheckSum = nCheckSum ^ m_nRevData[i];
	}

	if(nCheckSum != m_nRevData[m_nRevLength - 1]){
		szMsg1 = szMsg1 + ",CheckSum Fail";
		m_nResult = TEST_CHKSUM_FAIL;
		goto END_THREAD;
	}
	if(nCmd != m_nRevData[3]){
		szMsg1 = szMsg1 + ",Command Fail";
		m_nResult = TEST_COMMAND_FAIL;
		goto END_THREAD;
	}

	if(bIfCheck){
		if(m_nRevData[4] == 0x01){		
			m_nResult = TEST_PASS;
			szMsg1 = szMsg1 + "(OK)";
		}
		else{  
			m_nResult = TEST_FAIL;
			szMsg1 = szMsg1 + "(NG)";
		}
	}

	if(m_nRevData[1] > 0x04)
	{
		for(i = 5; i < m_nRevLength - 1; i++)
		{
			sTemp.Format("%c", m_nRevData[i]);
			sReadData = sReadData + sTemp;
		}

		nFind = sReadData.Find(',');
		if(nFind == -1){
			m_nIfTest_Data1 = atoi(sReadData);
		}
		else{
			sData1 = sReadData.Left(nFind);
			sData2 = sReadData.Right(sReadData.GetLength( ) -( nFind + 1));

			m_nIfTest_Data1 = atoi(sData1);
			m_nIfTest_Data2 = atoi(sData2);
		}

	}

END_THREAD :
	if(CurrentSet->bIsRunning)
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		
	}

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
	BOOL bHexType = FALSE;
	BYTE nCmd;
	CString sReadData = _T("");
	CString sTemp;
	CString sVerName;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BYTE nCheckSum1  = 0x00;
	BYTE nCheckSum2  = 0x00;
	int i;
	BOOL bCheckSum_Check = TRUE;

	m_bCheckReadVersion = 0;

	switch(nIndex)
	{
		case BE_VER:		
			nCmd = 0x21;
			sVerName = "BE_Ver:";
			break;

		case FE_VER:		
			nCmd = 0x22;
			sVerName = "FE_Ver:";
			break;

		case MICOM_VER:	
			nCmd = 0x23;
			sVerName = "MICOM_Ver:";
			break;

		case DSP_VER:		
			nCmd = 0x24;
			sVerName = "DSP_Ver:";
			break;

		case SUB_DSP_VER:	
			nCmd = 0x25;
			sVerName = "SUB_DSP_Ver:";
			break;

		case EQ_VER:		
			nCmd = 0x26;
			sVerName = "EQ_Ver:";
			break;

		case MEQ_VER:		
			nCmd = 0x27;
			sVerName = "MEQ_Ver:";
			break;

		case TOUCH_VER:	
			nCmd = 0x28;
			sVerName = "TOUCH_Ver:";
			break;

		case DEMO_VER:	
			nCmd = 0x29;
			sVerName = "DEMO_Ver:";
			break;


		case DJ_SOUND_VER:	
			nCmd = 0x2A;
			sVerName = "DJ_SOUND_Ver:";
			break;

		case WIRELESS_TX_VER:	
			nCmd = 0x2B;
			if(CurrentSet->nChipType == CHIP_MCS)
			{
				bHexType = TRUE;
			}
			sVerName = "WirelessTx_Ver:";
			break;

		case WIRELESS_RX_VER:	
			nCmd = 0x2C;
			if(CurrentSet->nChipType == CHIP_MCS)
			{
				bHexType = TRUE;
			}
			sVerName = "WirelessRx_Ver:";
			break;
		case BT_VER:
			nCmd = 0x2F;
			if (CurrentSet->nChipType == CHIP_MCS)
			{
				bHexType = TRUE;
			}
			sVerName = "BT_Ver:";
			break;
		case HDMI_VER:
			nCmd = 0x2E;
			if (CurrentSet->nChipType == CHIP_MCS)
			{
				bHexType = TRUE;
			}
			sVerName = "HDMI_Ver:";
			break;


		case CHECKSUM_VER:	
			nCmd = 0x2D;
			if(CurrentSet->nChipType == CHIP_MCS)
			{
				bHexType = TRUE;
			}
			sVerName = "CheckSum_Ver:";
			break;


		default :
			return FALSE;
			break;
	}

	if(!SendCommand(nCmd))
	{
		return FALSE;
	}
	//20210120 Time //_Wait(200);

	if(!ReceiveCommString(5000))
	{
		return FALSE;
	}

	for(i =0; i < m_nRevLength; i++)
	{
		sTemp.Format("%02X ", m_nRevData[i]);
		szMsg1 = szMsg1 + sTemp;
	}
	for(i =0; i < m_nRevLength - 1; i++)
	{
		nCheckSum1 = nCheckSum1 ^ m_nRevData[i];
	}

	nCheckSum2 = m_nRevData[m_nRevLength - 1];
	if(nCheckSum1 != nCheckSum2){
		sTemp.Format(",CheckSum Fail(%02X,%02X)", nCheckSum1, nCheckSum2);
		//szMsg1 = szMsg1 + sTemp;
		bCheckSum_Check = FALSE;
		AddStringToStatus(sTemp);
	}
	
	for(i =4; i < m_nRevLength - 1; i++)
	{
		if(bHexType){
			sTemp.Format("%02X", m_nRevData[i]);
		}
		else{
			sTemp.Format("%c", m_nRevData[i]);
		}
		sReadData = sReadData + sTemp;

	}
	sTemp.Format("(%s%s)", sVerName, sReadData); 
	if(CurrentSet->bIsRunning && CurrentSet->bCommDisplay)
	{
		AddStringToStatus(sTemp);
	}

	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1 + sTemp;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1 + sTemp;
		}		
	}

	if(!bCheckSum_Check){return FALSE;}

//	sVersion.MakeUpper(); 
	if(sReadData.Compare(sVersion) == 0){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

BOOL CTVCommCtrl::Firmware_VersionRead(int nIndex, CString &sVersion)
{
	BOOL bHexType = FALSE;
	BYTE nCmd;
	CString sReadData = _T("");
	CString sTemp;
	CString sVerName;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BYTE nCheckSum1 = 0x00;
	BYTE nCheckSum2 = 0x00;
	int i;
	BOOL bCheckSum_Check = TRUE;

	switch (nIndex)
	{
	case BE_VER:
		nCmd = 0x21;
		sVerName = "BE_Ver:";
		break;

	case FE_VER:
		nCmd = 0x22;
		sVerName = "FE_Ver:";
		break;

	case MICOM_VER:
		nCmd = 0x23;
		sVerName = "MICOM_Ver:";
		break;

	case DSP_VER:
		nCmd = 0x24;
		sVerName = "DSP_Ver:";
		break;

	case SUB_DSP_VER:
		nCmd = 0x25;
		sVerName = "SUB_DSP_Ver:";
		break;

	case EQ_VER:
		nCmd = 0x26;
		sVerName = "EQ_Ver:";
		break;

	case MEQ_VER:
		nCmd = 0x27;
		sVerName = "MEQ_Ver:";
		break;

	case TOUCH_VER:
		nCmd = 0x28;
		sVerName = "TOUCH_Ver:";
		break;

	case DEMO_VER:
		nCmd = 0x29;
		sVerName = "DEMO_Ver:";
		break;


	case DJ_SOUND_VER:
		nCmd = 0x2A;
		sVerName = "DJ_SOUND_Ver:";
		break;

	case WIRELESS_TX_VER:
		nCmd = 0x2B;
		if (CurrentSet->nChipType == CHIP_MCS)
		{
			bHexType = TRUE;
		}
		sVerName = "WirelessTx_Ver:";
		break;

	case WIRELESS_RX_VER:
		nCmd = 0x2C;
		if (CurrentSet->nChipType == CHIP_MCS)
		{
			bHexType = TRUE;
		}
		sVerName = "WirelessRx_Ver:";
		break;
	case BT_VER:
		nCmd = 0x2F;
		if (CurrentSet->nChipType == CHIP_MCS)
		{
			bHexType = TRUE;
		}
		sVerName = "BT_Ver:";
		break;
	case HDMI_VER:
		nCmd = 0x2E;
		if (CurrentSet->nChipType == CHIP_MCS)
		{
			bHexType = TRUE;
		}
		sVerName = "HDMI_Ver:";
		break;

	case CHECKSUM_VER:
		nCmd = 0x2D;
		if (CurrentSet->nChipType == CHIP_MCS)
		{
			bHexType = TRUE;
		}
		sVerName = "CheckSum_Ver:";
		break;


	default:
		return FALSE;
		break;
	}

	if (!SendCommand(nCmd))
	{
		return FALSE;
	}
	//20210120 Time //_Wait(200);

	if (!ReceiveCommString(5000))
	{
		return FALSE;
	}

	for (i = 0; i < m_nRevLength; i++)
	{
		sTemp.Format("%02X ", m_nRevData[i]);
		szMsg1 = szMsg1 + sTemp;
	}
	for (i = 0; i < m_nRevLength - 1; i++)
	{
		nCheckSum1 = nCheckSum1 ^ m_nRevData[i];
	}

	nCheckSum2 = m_nRevData[m_nRevLength - 1];
	if (nCheckSum1 != nCheckSum2) {
		sTemp.Format(",CheckSum Fail(%02X,%02X)", nCheckSum1, nCheckSum2);
		//szMsg1 = szMsg1 + sTemp;
		bCheckSum_Check = FALSE;
		AddStringToStatus(sTemp);
	}

	for (i = 4; i < m_nRevLength - 1; i++)
	{
		if (bHexType) {
			sTemp.Format("%02X", m_nRevData[i]);
		}
		else {
			sTemp.Format("%c", m_nRevData[i]);
		}
		sReadData = sReadData + sTemp;

	}
	sTemp.Format("(%s%s)", sVerName, sReadData);
	if (CurrentSet->bIsRunning && CurrentSet->bCommDisplay)
	{
		AddStringToStatus(sTemp);
	}

	/*if ((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);

		if (szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1 + sTemp;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1 + sTemp;
		}
	}*/

	if (!bCheckSum_Check) { return FALSE; }

	sVersion = sReadData;

	////	sVersion.MakeUpper(); 
	//if (sReadData.Compare(sVersion) == 0) {
	//	return TRUE;
	//}
	//else {
	//	return FALSE;
	//}

}


BOOL CTVCommCtrl::Send_TVControl_Command(BYTE nCommand, int nWait)
{
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");


	PortClear();

	if(!SendCommand(nCommand))
	{
		return FALSE;
	}

	//==================
	// Receive Response
	//==================

	if ((g_pView->m_Rs232cAdcManualCheckDlg.GetSafeHwnd() != NULL)
		&& (g_pView->m_Rs232cAdcManualCheckDlg.m_bActive == TRUE))
	{

	}
	else
	{
		if (!ReceiveCommString(nWait))
		{
			return FALSE;
		}
	}
	return TRUE;
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

	if(!ReceiveCommString(3000))
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

	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		
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
	CString sTVComPort;
	int nPortNo;

	nRev = m_ctrlTVCommCtrl.ClearPort();
	
	nPortNo = m_ctrlTVCommCtrl.m_wPortID;

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

	return TRUE;
}

//+add 090415
BOOL CTVCommCtrl::Set_Mic_OnOff(BOOL bFlag, int nWait)
{
/*	CString sIndex;
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

	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
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
	*/
	return TRUE;

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
void CTVCommCtrl::Set_MAC_Address(CString sData)
{
	sData.MakeUpper();

	m_aMacAdd.Add(sData);
	
}

BOOL CTVCommCtrl::MAC_AddressCheck(BOOL bWifi)
{
	BYTE nCmd;
//	CString sData;
	CString sReadData;
	CString sMAC_Add;
	CString sTemp;
//	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BYTE nCheckSum1  = 0x00;
	BYTE nCheckSum2  = 0x00;
	int i;
	BOOL bCheckSum_Check = TRUE;

	BOOL bResult = TRUE;
	int nNoTest = m_aMacAdd.GetSize();

	if(bWifi){ nCmd = 0x46;}
	else{ nCmd = 0x45;}

	if(!SendCommand(nCmd))
	{
		return FALSE;
	}
	//20210120 Time //_Wait(100);

	if(!ReceiveCommString(5000))
	{
		return FALSE;
	}

	for(i =0; i < m_nRevLength; i++)
	{
		sTemp.Format("%02X ", m_nRevData[i]);
		szMsg1 = szMsg1 + sTemp;
	}
	for(i =0; i < m_nRevLength - 1; i++)
	{
		nCheckSum1 = nCheckSum1 ^ m_nRevData[i];
	}

	nCheckSum2 = m_nRevData[m_nRevLength - 1];
	if(nCheckSum1 != nCheckSum2){
		sTemp.Format(",CheckSum Fail(%02X,%02X)", nCheckSum1, nCheckSum2);
		szMsg1 = szMsg1 + sTemp;
		bCheckSum_Check = FALSE;
	}
	
	for(i =4; i < m_nRevLength - 1; i++)
	{
		sTemp.Format("%c", m_nRevData[i]);
		sReadData = sReadData + sTemp;
	}
	sTemp.Format("MAC Add : %s)", sReadData); 
	if(CurrentSet->bIsRunning && CurrentSet->bCommDisplay)
	{
		AddStringToStatus(sTemp);
	}

	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1 + sTemp;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1 + sTemp;
		}		
	}
	if(!bCheckSum_Check){return FALSE;}

	sReadData.MakeUpper();

	if(sReadData.Find("00:00:00:00:00:00") != -1)
	{
		bResult = FALSE;
	}
	else if(sReadData.Find("FF:FF:FF:FF:FF:FF") != -1)
	{
		bResult = FALSE;
	}
	else if(sReadData.Find("00:10:18:26:08:E0") != -1)
	{
		bResult = FALSE;
	}
	else if(sReadData.Find("00:F0:FF:00:F0:FF") != -1)
	{
		bResult = FALSE;
	}
	if(!bResult){
		return FALSE;
	}

	sMAC_Add = sReadData.Left(8);
	if(nNoTest == 0) return FALSE; 
	bResult = FALSE;

	for(int i=0; i<nNoTest; i++)
	{
		if(sMAC_Add.Compare(m_aMacAdd.GetAt(i)) == 0){
			bResult = TRUE;
		}
	}
	
	m_aMacAdd.RemoveAll();

	return bResult;
}

BOOL CTVCommCtrl::MAC_AddressRead(CString &sMac_add, BOOL bWifi)
{
	BYTE nCmd;
	CString sReadData;
	CString sMAC_Add;
	CString sTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BYTE nCheckSum1  = 0x00;
	BYTE nCheckSum2  = 0x00;
	int i;
	BOOL bCheckSum_Check = TRUE;

	BOOL bResult = TRUE;

	if(bWifi){ nCmd = 0x46;}
	else{ nCmd = 0x45;}

	if(!SendCommand(nCmd))
	{
		return FALSE;
	}
	//20210120 Time //_Wait(100);

	if(!ReceiveCommString(5000))
	{
		return FALSE;
	}

	for(i =0; i < m_nRevLength; i++)
	{
		sTemp.Format("%02X ", m_nRevData[i]);
		szMsg1 = szMsg1 + sTemp;
	}
	for(i =0; i < m_nRevLength - 1; i++)
	{
		nCheckSum1 = nCheckSum1 ^ m_nRevData[i];
	}

	nCheckSum2 = m_nRevData[m_nRevLength - 1];
	if(nCheckSum1 != nCheckSum2){
		sTemp.Format(",CheckSum Fail(%02X,%02X)", nCheckSum1, nCheckSum2);
		szMsg1 = szMsg1 + sTemp;
		bCheckSum_Check = FALSE;
	}
	
	for(i =4; i < m_nRevLength - 1; i++)
	{
		sTemp.Format("%c", m_nRevData[i]);
		sReadData = sReadData + sTemp;
	}
	sTemp.Format("MAC Add : %s)", sReadData); 
	if(CurrentSet->bIsRunning && CurrentSet->bCommDisplay)
	{
		AddStringToStatus(sTemp);
	}

	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1 + sTemp;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1 + sTemp;
		}		
	}
	if(!bCheckSum_Check){return FALSE;}

	sReadData.MakeUpper();

	sMac_add = sReadData;

	return TRUE;
}

BOOL CTVCommCtrl::Serial_NumberCheck()
{
	BYTE nCmd;
	CString sTemp;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BYTE nCheckSum1  = 0x00;
	BYTE nCheckSum2  = 0x00;
	int i;
	BOOL bCheckSum_Check = TRUE;

	BOOL bResult = FALSE;

	nCmd = 0xB4;

	if(!SendCommand(nCmd))
	{
		return FALSE;
	}
	//20210120 Time //_Wait(100);

	if(!ReceiveCommString(5000))
	{
		return FALSE;
	}

	for(i =0; i < m_nRevLength; i++)
	{
		sTemp.Format("%02X ", m_nRevData[i]);
		szMsg1 = szMsg1 + sTemp;
	}
	for(i =0; i < m_nRevLength - 1; i++)
	{
		nCheckSum1 = nCheckSum1 ^ m_nRevData[i];
	}

	nCheckSum2 = m_nRevData[m_nRevLength - 1];
	if(nCheckSum1 != nCheckSum2){
		sTemp.Format(",CheckSum Fail(%02X,%02X)", nCheckSum1, nCheckSum2);
		szMsg1 = szMsg1 + sTemp;
		bCheckSum_Check = FALSE;
	}
	
	for(i =4; i < m_nRevLength - 1; i++)
	{
		sTemp.Format("%c", m_nRevData[i]);
		sReadData = sReadData + sTemp;
	}
	sTemp.Format("S/N : %s)", sReadData); 
	if(CurrentSet->bIsRunning && CurrentSet->bCommDisplay)
	{
		AddStringToStatus(sTemp);
	}

	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1 + sTemp;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1 + sTemp;
		}		
	}
	if(!bCheckSum_Check){return FALSE;}

	sReadData.MakeUpper();

	if(sReadData.Compare(m_strWipId) == 0){
			bResult = TRUE;
	}
	
	return bResult;
}

BOOL CTVCommCtrl::Check_MicDiffLevel()
{
	BYTE nCmd;
	CString sTemp;
	CString sReadData;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	BYTE nCheckSum  = 0x00;
	int i;
	BOOL bCheckSum_Check = TRUE;

	BOOL bResult = FALSE;

	nCmd = 0xF1;

	//==============
	// Send Command
	//==============
	if(!SendCommand(nCmd))
	{
		szMsg1 = "Send Fail";
		goto END_THREAD;
	}

	//20210120 Time //_Wait(500);
	//==================
	// Receive Response
	//==================
	if(!ReceiveCommString(5000))
	{
		szMsg1 = "Read TimeOut";
		goto END_THREAD;
	}

	for(i =0; i < m_nRevLength; i++)
	{
		sTemp.Format("%02X ", m_nRevData[i]);
		szMsg1 = szMsg1 + sTemp;
	}
	for(i =0; i < m_nRevLength - 1; i++)
	{
		nCheckSum = nCheckSum ^ m_nRevData[i];
	}

	if(nCheckSum != m_nRevData[m_nRevLength - 1]){
		szMsg1 = szMsg1 + ",CheckSum Fail";
		goto END_THREAD;
	}
	if(nCmd != m_nRevData[3]){
		szMsg1 = szMsg1 + ",Command Fail";
		goto END_THREAD;
	}

	if(m_nRevData[4] == 0x01){		
		bResult = TRUE;
		szMsg1 = szMsg1 + "(OK)";
	}
	else{  
		bResult = FALSE;
		szMsg1 = szMsg1 + "(NG)";
	}

END_THREAD :
	if(CurrentSet->bIsRunning)
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		
	}


	
	return bResult;
}