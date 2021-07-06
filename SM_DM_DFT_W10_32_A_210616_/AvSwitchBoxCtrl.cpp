// Modified by kwmoon 07.04.06
////////////////////////////////////////////

#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "AvSwitchBoxCtrl.h"

extern CDATsysView*  g_pView;

//+change 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;
extern CColorStaticST		ctrlStepMeasur_A;

CAvSwitchBox AvSwitchBoxCtrl;

CAvSwitchBox::CAvSwitchBox()
{
	m_bPortOpen = FALSE;

	m_nPortID = -1;
	m_nAvSwitchCmd = -1;

	m_fTargetLeftFreq		= 0;
	m_fTargetRightFreq		= 0;
	
	m_fTargetLeftLevel		= 0;
	m_fTargetRightLevel		= 0;

	//+change kwmoon 080724
//	m_fPercentMargin		= 0;
	m_fFreqMargin			= 0;

	//+add kwmoon 080724
	m_fLevelMargin			= 0;

	m_fMeasuredLeftFreq		= 0;
	m_fMeasuredRightFreq	= 0;

	m_fMeasuredLeftLevel	= 0;
	m_fMeasuredRightLevel	= 0;

	m_fLeftFreqLowerLimit	= 0;
	m_fLeftFreqUpperLimit	= 0;
	m_fRightFreqLowerLimit	= 0;
	m_fRightFreqUpperLimit	= 0;

	m_fLeftLevelLowerLimit	= 0;
	m_fLeftLevelUpperLimit	= 0;
	m_fRightLevelLowerLimit	= 0;
	m_fRightLevelUpperLimit	= 0;

	m_bResult				= FALSE;
	
	m_bThreadRunning		= FALSE;
	m_bExitThread			= FALSE;

	//+change 090213(Modification No1)
//	m_hThread	 		  = NULL;
	m_pAudioMeasureThread = NULL;
	m_hAudioMeasureThreadKillEvent = NULL;

	m_strErrMsg = _T("");
	m_sRemoteName = _T("");;
	m_sRemoteCustom = _T("");;
	m_sRemoteCode = _T("");
	m_nRemoteDelay = 0;

	m_nNoRemoteRetry = 1;

	m_nVideoSourceType = -1;

	m_nAudioCh = 0;
	m_nVideoCh = 0;

}

CAvSwitchBox::~CAvSwitchBox()
{
	if(m_hAudioMeasureThreadKillEvent != NULL)
	{
		CloseHandle(m_hAudioMeasureThreadKillEvent); m_hAudioMeasureThreadKillEvent = NULL;
	}

}

// Open Com Port
BOOL CAvSwitchBox::Create(CString sComPort, DWORD nBaudRate)
{
	CString sTmp = sComPort;

	sTmp.Delete(0, sTmp.Find("COM")+3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlAvSwitchBox.OpenPort( sComPort, nBaudRate, m_nPortID))
	{
		m_strErrMsg.Format("[AvSwitchBox] Failed to open %s port!", sComPort);
		m_bPortOpen = FALSE;
		
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	m_hAudioMeasureThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);

	return TRUE;
}

void CAvSwitchBox::PortClose()
{
	m_ctrlAvSwitchBox.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CAvSwitchBox::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}

BOOL CAvSwitchBox::SendCommString(CString str)
{
	int nRtn = 0;
	if(m_bPortOpen == FALSE) return FALSE;

	m_ctrlAvSwitchBox.m_QueueRead.Clear();
	nRtn = m_ctrlAvSwitchBox.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());
	if(nRtn != str.GetLength()) return FALSE;
	else return TRUE;
}

//+change kwmoon 080325
UINT CAvSwitchBox::StartAudioMeasureThread(LPVOID pParam)

{
	CAvSwitchBox* pAvSwitch = (CAvSwitchBox* )pParam;

	int aResult[4] = {0,0,0,0};
	int nNoMeasureRepeat = 0;

	float fLowerMargin	 = (float)(1 - (pAvSwitch->m_fFreqMargin * 0.01));
	float fUpperMargin	 = (float)(1 + (pAvSwitch->m_fFreqMargin * 0.01));

	//+add kwmoon 080716
	float fLevelMargin	 = (float)(1 - (pAvSwitch->m_fLevelMargin * 0.01));

	pAvSwitch->m_fMeasuredLeftFreq		= 0;
	pAvSwitch->m_fMeasuredRightFreq		= 0;
	
	pAvSwitch->m_fMeasuredLeftLevel		= 0;
	pAvSwitch->m_fMeasuredRightLevel	= 0;

	pAvSwitch->m_bResult		= FALSE;
	pAvSwitch->m_bExitThread	= FALSE;

	//+change kwmoon 080716
	pAvSwitch->m_fLeftFreqLowerLimit	= pAvSwitch->m_fTargetLeftFreq * fLowerMargin;
	pAvSwitch->m_fLeftFreqUpperLimit	= pAvSwitch->m_fTargetLeftFreq * fUpperMargin;

	pAvSwitch->m_fRightFreqLowerLimit	= pAvSwitch->m_fTargetRightFreq * fLowerMargin;
	pAvSwitch->m_fRightFreqUpperLimit	= pAvSwitch->m_fTargetRightFreq * fUpperMargin;

	//+change kwmoon 080716
//	pAvSwitch->m_fLeftLevelLowerLimit	= pAvSwitch->m_fTargetLeftLevel * fLowerMargin;
	pAvSwitch->m_fLeftLevelLowerLimit	= pAvSwitch->m_fTargetLeftLevel * fLevelMargin;
	//+del kwmoon 080716
//	pAvSwitch->m_fLeftLevelUpperLimit	= pAvSwitch->m_fTargetLeftLevel * fUpperMargin;
	
	//+change kwmoon 080716
//	pAvSwitch->m_fRightLevelLowerLimit	= pAvSwitch->m_fTargetRightLevel * fLowerMargin;
	pAvSwitch->m_fRightLevelLowerLimit	= pAvSwitch->m_fTargetRightLevel * fLevelMargin;
	//+del kwmoon 080716
//	pAvSwitch->m_fRightLevelUpperLimit	= pAvSwitch->m_fTargetRightLevel * fUpperMargin;

	::ResetEvent(pAvSwitch->m_hAudioMeasureThreadKillEvent);

	double Avgfreq_L,Avgfreq_R;

	int nNoMeasureAudioFreqRetry		= 0;
	int nNoMeasureAudioLevelRetry		= 0;
	int nNoMeasureAudioResetRetry		= 0;

	double AvgLevel_L,AvgLevel_R;

	CString szMsg;

	CString szLeftFreq, szRightFreq, szLeftLevel, szRightLevel;
	int nNoRetry = 0;

	CString sTmp;

	nNoMeasureAudioResetRetry = 0;

MEASURE_LEVEL_START : 


	
	AvgLevel_L			= 0;
	AvgLevel_R			= 0;

	if(pAvSwitch->SendCmd( CHK_AUDIO_LVL ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 0, 0))
	{
		AvgLevel_L = (atof(sTmp) * 1000);
	}

	if(pAvSwitch->SendCmd( CHK_AUDIO_LVL ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 1, 0))
	{
		AvgLevel_R = (atof(sTmp) * 1000);
	}


	pAvSwitch->m_fMeasuredLeftLevel  = (float)AvgLevel_L;
	pAvSwitch->m_fMeasuredRightLevel = (float)AvgLevel_R;

	if(pAvSwitch->m_fMeasuredLeftLevel >= (pAvSwitch->m_fLeftLevelLowerLimit))
	{
		aResult[2] = TEST_PASS;
	}
	else
	{
		aResult[2] = TEST_FAIL;
	}

//	if((pAvSwitch->m_fMeasuredRightLevel >= (pAvSwitch->m_fRightLevelLowerLimit))
//	&& (pAvSwitch->m_fMeasuredRightLevel <= (pAvSwitch->m_fRightLevelUpperLimit)))
	if(pAvSwitch->m_fMeasuredRightLevel >= (pAvSwitch->m_fRightLevelLowerLimit))
	{
		aResult[3] = TEST_PASS;
	}
	else
	{
		aResult[3] = TEST_FAIL;
	}

	//+add kwmoon 080326
	if((aResult[2] == TEST_FAIL) || (aResult[3] == TEST_FAIL))
	{
		if(nNoMeasureAudioLevelRetry++ < MAX_NO_MEASURE_AUDIO_RETRY)
		{
			szMsg.Format("Retry Check Level (%d_%d)",nNoMeasureAudioLevelRetry,MAX_NO_MEASURE_AUDIO_RETRY);
			ShowSubStepMessage(szMsg, "Retry Audio Level Check");	
			
			//+change kwmoon 080618
		//	_Wait(500);
			_Wait(200);

			
			goto MEASURE_LEVEL_START;
		}
	}

MEASURE_FREQ_START : 


	Avgfreq_L	= 0.0;
	Avgfreq_R	= 0.0;

	if(pAvSwitch->SendCmd( CHK_AUDIO_FRQ ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 0, 0))
	{
		Avgfreq_L = atol(sTmp);
	}
	
	if(pAvSwitch->SendCmd( CHK_AUDIO_FRQ ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 1, 0))
	{
 		Avgfreq_R = atol(sTmp);
	}



	pAvSwitch->m_fMeasuredLeftFreq  = (float)Avgfreq_L;
	pAvSwitch->m_fMeasuredRightFreq = (float)Avgfreq_R;
		
	if((pAvSwitch->m_fMeasuredLeftFreq >= (pAvSwitch->m_fLeftFreqLowerLimit))
	&& (pAvSwitch->m_fMeasuredLeftFreq <= (pAvSwitch->m_fLeftFreqUpperLimit)))
	{
		aResult[0] = TEST_PASS;
	}
	else
	{
		aResult[0] = TEST_FAIL;
	
	}

	if((pAvSwitch->m_fMeasuredRightFreq >= (pAvSwitch->m_fRightFreqLowerLimit))
	&& (pAvSwitch->m_fMeasuredRightFreq <= (pAvSwitch->m_fRightFreqUpperLimit)))
	{
		aResult[1] = TEST_PASS;
	}
	else
	{
		aResult[1] = TEST_FAIL;
		
		//+del kwmoon 080326
	//	nNoRetry = ++nNoMeasureAudioFreqRetry;

	//	szMsg.Format("Retry Check Audio Freq (%d/%d)",nNoRetry,MAX_NO_MEASURE_AUDIO_RETRY);
	//	ShowSubStepMessage(szMsg, "Retry Audio Freq Check");	
		
	//	goto MEASURE_FREQ_START;
	
	}
	
	//+add kwmoon 080326
	if((aResult[0] == TEST_FAIL) || (aResult[1] == TEST_FAIL))
	{
		if(nNoMeasureAudioFreqRetry++ < MAX_NO_MEASURE_AUDIO_RETRY)
		{
			szMsg.Format("Retry Check Freq (%d_%d)",nNoMeasureAudioFreqRetry,MAX_NO_MEASURE_AUDIO_RETRY);
			ShowSubStepMessage(szMsg, "Retry Audio Freq Check");	
			//+change kwmoon 080618
		//	_Wait(500); 
			_Wait(200); 

			
			goto MEASURE_FREQ_START;
		}
	}

//END_THREAD :

	// If all of the 4 audio test are passed, set the result TRUE;
	if((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS)
	&& (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
	{
		pAvSwitch->m_bResult = TRUE;
	}

	//+change kwmoon 080523
//	g_pView->UpdateInfo(AUDIO_L_FREQ,szLeftFreq);
//	g_pView->UpdateInfo(AUDIO_R_FREQ,szRightFreq);
//	g_pView->UpdateInfo(AUDIO_L_LEVEL,szLeftLevel);
//	g_pView->UpdateInfo(AUDIO_R_LEVEL,szRightLevel);
	
	pAvSwitch->m_bThreadRunning = FALSE;

	::SetEvent(pAvSwitch->m_hAudioMeasureThreadKillEvent);

	return 0;
}



void CAvSwitchBox::RunAudioMeasure()
{
	if(!m_bThreadRunning)
	{
		//+change 090213(Modification No1)
		m_pAudioMeasureThread = AfxBeginThread(StartAudioMeasureThread,this);
		
		//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
		CString szString;
		szString.Format("StartAudioMeasureThread %x\n",m_pAudioMeasureThread->m_nThreadID);
//		if(g_LogFileOpen) g_LogFile.WriteString(szString);
#endif
		m_bThreadRunning = TRUE;
	}
}
//+2007.12.25 Add BY USY

UINT CAvSwitchBox::CheckReceiveMsg(int nCmd, int nWaitLimit, CString& sValue)
{
	UINT nRtn = 0; // 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : AvSwitch Busy
	UINT nSize = 0; // Return Msg Size
	CString sRst = _T("");	

	if(m_bPortOpen == FALSE) return FALSE;

	if((nCmd == CHK_AUDIO_LVL) || (nCmd == CHK_AUDIO_FRQ)) nSize = 22;
	else nSize = 9;

	if(!ReceiveCommString(nSize, nWaitLimit))
	{
		nRtn = 3;
	}
	else 
	{
		if((nCmd == CHK_AUDIO_LVL) || (nCmd == CHK_AUDIO_FRQ))
		{
			sRst.Format("%c%c%c%c%c", m_strReceive[14], m_strReceive[15],m_strReceive[16],m_strReceive[17], m_strReceive[18]);
			
			sValue.Format("%c%c%c%c%c%c%c%c", m_strReceive[2],m_strReceive[3],m_strReceive[4],m_strReceive[5],
											  m_strReceive[6],m_strReceive[7],m_strReceive[8],m_strReceive[9]);			
			
		}
		else
		{ 
		//	sRst.Format("%c%c%c%c%c", m_strReceive[1], m_strReceive[2],m_strReceive[3],m_strReceive[4], m_strReceive[5]);
			sRst.Format("%s", m_strReceive);
		}
		sRst.MakeUpper();
		sRst.TrimLeft(STX);
//		sRst = sRst.Left(5);
		if(sRst.Find("DREAD") != -1) 
		{
			nRtn =  1;	
		}
		else if(sRst.Find("DREPL") != -1) nRtn =  2;
		else if(sRst.Find("DBUSY")  != -1) nRtn =  4;
		else nRtn = 2;
	}
	return nRtn;
}

//==============
// AvSwitch Cmd 
//==============

// 1st parameter(Cmd Type)		/ 2nd parameter (nValue1) 				/ 3rd Parameter (nValue2)
// 01. SET_DIGIT_OUT			/ Digital Out Port(00, 01,02, 03~)		/ Digit Out On / Off( 0 : Off , 1 : On)
// 02. SET_VIDEO_CH				/ Video Channel(1 : CH1, 2 : Ch2)		/ Don't Care
// 03. SET_AUDIO_CH				/ Audio Channel  (1 ~ 8)				/ Don't Care
// 04. SET_COMP_CH				/ COMP Channel ( 1 : Ch 1, 2 : Ch 2)	/ Don't Care
// 05. SET_CVBS_CH				/ CVBS Channel ( 1 : Ch 1, 2 : Ch 2)	/ Don't Care
// 06. SET_ANALOG_VOLT			/ Port (1 : Port 1, 2 : Port 2)			/ Analog Volt ( 00 : 0V, 01 : 1V, 02 : 2V, 03 : 2.5V, 04 : 3.3V)
// 07. PULLUP_AVSWITCH			/ PullUp Volt ( 3 : 3.3V, 5 : 5V )		/ Don't Care
// 08. REMOTE_TYPE_SEL			/ Remote Type( 1: ,2 : )				/ Don't Care
// 09. CHK_AUDIO_LVL			/ Audio Level(0 : Left, 1 : Right)		/ Don't Care
// 10. CHK_AUDIO_FRQ			/ Audio Freq(0 : Left, 1 : Right)		/ Don't Care
// 11. CHECK_STATUS				/ Don't Care							/ Don't Care
// 12. SEND_REMOTE_CODE			 
//+add kwmoon080312
// 13. SET_REMOTE_REPEAT_NO 
//+add kwmoon 080929
// 14. SET_RGB_SOUND 
//+add PSH 100310
// 14. SET_COMP_CVBS_CH 

CString CAvSwitchBox::MakeCmdString(int nCmd, CString& szSwitchBoxMsg, int nValue1 , int nValue2 )
{
	CString szCmdString	= _T("");
	CString sTmp		= _T("");
	int nVolt = 0;
	szSwitchBoxMsg	= _T("");

	switch(nCmd)
	{
		case SET_DIGIT_OUT : 
		{	//nValue1 => Port(00, 01, 02, .....), nValue2 => 0 : Port off, 1 : Port On  
			if(nValue2 == 0)// Port Off
			{
				szCmdString.Format("%cCDIGI_2%02d%c%c%c",STX,(nValue1 - 1), ETX,CR,LF);
				szSwitchBoxMsg.Format("Digital Out Off");
			}
			else if(nValue2 == 1)// Port On
			{
				szCmdString.Format("%cCDIGI_1%02d%c%c%c",STX,(nValue1 - 1), ETX,CR,LF);
				szSwitchBoxMsg.Format("Digital Out On");
			}
		}
		break;

		case SET_VIDEO_CH :
		{	//nValue1 => Video Channel (1 or 2)
			szCmdString.Format("%cCV_SEL_%02d%c%c%c", STX, (nValue1 - 1), ETX, CR, LF);
			szSwitchBoxMsg.Format("Set VIDEO CH%d",nValue1);
		}
		break;

		case SET_AUDIO_CH : // nValue2 don't care
		{	//nValue1 => Channel Number( 00, 01, 02....)
			szCmdString.Format("%cCA_SEL_%02d%c%c%c",STX,(nValue1 - 1),ETX,CR,LF);
			szSwitchBoxMsg.Format("Set Audio Ch(%d)",nValue1);
		}
 		break;
		
		case SET_COMP_CH :
		{	//nValue1 => COMP Channel (1 or 2)
			szCmdString.Format("%cCCOMPON_%d%c%c%c", STX, nValue1, ETX, CR, LF);
			szSwitchBoxMsg.Format("Set Comp%d",nValue1);
		}
		break;
		
		case SET_CVBS_CH :
		{	// nValue1 => DVBS Channel ( 1 or 2)
			szCmdString.Format("%cCDVBS_%d__%c%c%c", STX, nValue1, ETX, CR, LF);
			szSwitchBoxMsg.Format("Set CVBS%d",nValue1);
		}
		break;

		case SET_ANALOG_VOLT : 
		{	//nValue1 => Channel(1 or 2), nValue2 : Volt(1.1V => 11)
			
			switch(nValue2) 
			{
				case DC_OUT_VOLTAGE_0_0 : nVolt = 00; 
					szSwitchBoxMsg.Format("Set Analog 0V"); break;
				case DC_OUT_VOLTAGE_1_0 : nVolt = 10; 
					szSwitchBoxMsg.Format("Set Analog 1V"); break;
				case DC_OUT_VOLTAGE_2_0 : nVolt = 20;
					szSwitchBoxMsg.Format("Set Analog 2V"); break;
				case DC_OUT_VOLTAGE_2_5 : nVolt = 25;
					szSwitchBoxMsg.Format("Set Analog 2.5V"); break;
				case DC_OUT_VOLTAGE_3_3 : nVolt = 33;
					szSwitchBoxMsg.Format("Set Analog 3.3V"); break;
			}
			szCmdString.Format("%cCVOLT_%d%2d%c%c%c",STX,nValue1 ,nVolt, ETX,CR,LF);
		}
		break;

		case PULLUP_AVSWITCH :
		{	// nValue1 => Voltage( 3 : 3.3V, 5 = 5V)
			szCmdString.Format("%cCPULL_UP%d%c%c%c", STX, nValue1, ETX, CR, LF);
			if(nValue1 == 3) szSwitchBoxMsg.Format("Set Pull Up 3.3V");
			else szSwitchBoxMsg.Format("Set Pull Up 5V");
		}
		break;
		
		case REMOTE_TYPE_SEL:
		{	//nValue1 => Remote Type (1 or 2)
			szCmdString.Format("%cCR_SEL_%02d%c%c%c", STX, nValue1 , ETX, CR, LF);	
			szSwitchBoxMsg.Format("Set REMOTE TYPE%d",nValue1);
		}
		break;

		case CHK_AUDIO_LVL :
		{	
			switch(nValue1) 
			{
				case 0:	// Left Level
					szCmdString.Format("%cCA_LVL_L?%c%c%c", STX, ETX, CR, LF);
					szSwitchBoxMsg.Format("Check Audio Level(L)");
					break;
				case 1: // Right Level
					szCmdString.Format("%cCA_LVL_R?%c%c%c", STX, ETX, CR, LF);
					szSwitchBoxMsg.Format("Check Audio Level(R)");
					break;
			}
		}
		break;

		case CHK_AUDIO_FRQ :
		{
			switch(nValue1) 
			{
				case 0:	//Left Freq
					szCmdString.Format("%cCA_FRQ_L?%c%c%c", STX, ETX, CR, LF);
					szSwitchBoxMsg.Format("Check Audio Freq(L)");
					break;
				case 1: //Right Freq
					szCmdString.Format("%cCA_FRQ_R?%c%c%c", STX, ETX, CR, LF);
					szSwitchBoxMsg.Format("Check Audio Freq(R)");
					break;
			}
		}
		break;
		
/*		case CHECK_STATUS:
		{	// nValue1, nValue2 don't use
			szCmdString.Format("%cCSTATUS_?%c%c%c",STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Check AvSwitch Status");
		}
		break;
*/
		case SET_CUSTOM_CODE:
		{	// nValue1 => Remote Custom Code
			sTmp.Format("%02X", nValue1);

			szCmdString.Format("%cCRECUS_%s%c%c%c",STX, sTmp, ETX, CR, LF);
			szSwitchBoxMsg.Format("Set Remocon Custom Code :%02X",nValue1);
		}
		break;

		case SEND_REMOTE_CODE :
		{	// nValue1 => Remote Code
			sTmp.Format("%02x", nValue1);
			sTmp.MakeUpper();
			szCmdString.Format("%cCREMOT_%s%c%c%c", STX, sTmp, ETX, CR, LF);
		}
		break;

		//+add kwmoon 080312
		case SET_REMOTE_REPEAT_NO :
		{	// nValue1 => Remote Repeat Number
			sTmp.Format("%x", nValue1);
			sTmp.MakeUpper();
			szCmdString.Format("%cCREPEAT_%s%c%c%c", STX, sTmp, ETX, CR, LF);
		}
		break;

/*		//+add kwmoon 080929
		case SET_RGB_SOUND :
		{	
			szCmdString.Format("%cCSOUNDRGB%c%c%c", STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Set RGB Sound On");
		}
		break;

		//+add PSH 100408 change
		case SET_COMP_CVBS_CH :
		{	
			szCmdString.Format("%cCCOMDVB_%d%c%c%c", STX, nValue1, ETX, CR, LF);
			szSwitchBoxMsg.Format("Set COMP_CVBS%d",nValue1);
		}
		break;
*/
	}
	return szCmdString;
}
BOOL CAvSwitchBox::ReceiveCommString(int nRev, int nWaitLimit)
{
//	MSG msg;
	BOOL		bDoing;
    clock_t		start;
//	int			i/*, nWaitLimit*/;
	BYTE		buf;
	DWORD		dwRead;			// Read Byte Count
	BYTE		buff[2048];		// Read Buffer
	CString		sTmp;

	if(m_bPortOpen == FALSE) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();

	m_ctrlAvSwitchBox.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{
			//+add kwmoon 080621
		/*	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
				if (msg.message == 45557 || msg.message == 45559)break;
				//CString s;
				//s.Format("msg.message= %d",msg.message); 
				//AddStringToStatus(s);
			}
		*/
			//-

			dwRead = m_ctrlAvSwitchBox.ReadComm( buff, 2048);
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlAvSwitchBox.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlAvSwitchBox.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				return FALSE;
			}

			//+add kwmoon 071030
			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				m_ctrlAvSwitchBox.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_ctrlAvSwitchBox.m_QueueRead.GetSize();

		if (rrv >= nRev)
		{
			for(int i=0; i<nRev; i++)
			{
				m_ctrlAvSwitchBox.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}

			if(CurrentSet->bCommDisplay)
			{
				CString s;
				s.Format("Response : %s",m_strReceive);
				AddStringToStatus(s);
			}
			m_ctrlAvSwitchBox.m_QueueRead.Clear();

			return TRUE;
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			m_ctrlAvSwitchBox.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
	return TRUE;	
}

BOOL CAvSwitchBox::SendCmd(int nCmd, int nWaitLimit, CString& sValue , int nValue1 , int nValue2 )
{
	// 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : Cmd Send Retry, 5 : Send Cmd Fail
	CString szCmdString	= _T("");
	CString szSwitchBoxMsg = _T("");
	UINT nRtn = 0;
	m_strErrMsg = _T("");

	//+del kwmoon 080325
	// Essential Delay for working properly.
//	_Wait(100);
	if(m_bPortOpen == FALSE) return FALSE;

	m_nAvSwitchCmd = nCmd;

	szCmdString = MakeCmdString(nCmd, szSwitchBoxMsg, nValue1, nValue2);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(nCmd, nWaitLimit, sValue);

		if(nRtn == 1) m_strErrMsg = _T("PASS");
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
		else if(nRtn == 4)
		{
			if(!SendCommString(szCmdString)) 
			{
				m_strErrMsg.Format("SEND FAIL");
				nRtn = 5;
			}
			else
			{
				nRtn = CheckReceiveMsg(nCmd, nWaitLimit, sValue);

				if(nRtn == 1) m_strErrMsg = _T("PASS");
				else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
				else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
				else if(nRtn == 4)
				{
					m_strErrMsg.Format("BUSY");
				}
			}
		}
	}
	
	if(CurrentSet->bCommDisplay)
	{
		if(szSwitchBoxMsg != _T("")) AddStringToStatus(szSwitchBoxMsg + " : " + m_strErrMsg);
	}

	if(nRtn == 1 ) return TRUE;
	else return FALSE;
}

BOOL CAvSwitchBox::SetAvSwitch(int nCmd, int nWaitLimit, int nValue1, int nValue2)
{
	// 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : Cmd Send Retry, 5 : Send Cmd Fail
	CString szCmdString	= _T("");
	CString szSwitchBoxMsg = _T("");
	CString sValue = _T("");
	UINT nRtn = 0;
	m_strErrMsg = _T("");

	if(m_bPortOpen == FALSE) return FALSE;
	//+2008.01.23 Add BY USY
	clock_t		start = clock();
	int			nElapsedTime = 0;
	CString		sTmp = _T("");
	//-

	//+del kwmoon 080325
	// Essential Delay for working properly.
//	_Wait(100);

	if((nCmd == SET_VIDEO_CH) && (m_nVideoCh == nValue1)) 
		return TRUE;
	if((nCmd == SET_AUDIO_CH) && (m_nAudioCh == nValue1)) 
		return TRUE;

	m_nAvSwitchCmd = nCmd;

	szCmdString = MakeCmdString(nCmd, szSwitchBoxMsg, nValue1, nValue2);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(nCmd, nWaitLimit, sValue);

		if(nRtn == 1) m_strErrMsg = _T("PASS");
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
		else if(nRtn == 4)
		{
			if(!SendCommString(szCmdString)) 
			{
				m_strErrMsg.Format("SEND FAIL");
				nRtn = 5;
			}
			else
			{
				nRtn = CheckReceiveMsg(nCmd, nWaitLimit, sValue);

				if(nRtn == 1) m_strErrMsg = _T("PASS");
				else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
				else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
				else if(nRtn == 4)
				{
					m_strErrMsg.Format("BUSY");
				}
			}
		}
	}
	//+2008.01.23 Add BY USY
	nElapsedTime = (clock() - start)*1000/CLOCKS_PER_SEC;
	//-

	if(CurrentSet->bCommDisplay)
	{
		//+2008.01.23 Mod BY USY
		if(szSwitchBoxMsg != _T("")) 
		{
			sTmp.Format("%s : %s[%dms]", szSwitchBoxMsg, m_strErrMsg, nElapsedTime);
			AddStringToStatus(sTmp);
		}
		//if(szSwitchBoxMsg != _T("")) AddStringToStatus(szSwitchBoxMsg + " : "+ m_strErrMsg);
		//-
	}

	if(nRtn == 1 ){
		if(nCmd == SET_VIDEO_CH) m_nVideoCh = nValue1;
		if(nCmd == SET_AUDIO_CH) m_nAudioCh = nValue1;
		return TRUE;
	}
	else return FALSE;
}

BOOL CAvSwitchBox::SendRemoteCmd(int nWaitLimit, int nRemoteType, CString sCustom, CString sCmd, int nByte)
{
	// 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : Cmd Send Retry, 5 : Send Cmd Fail
	CString szCmdString		= _T("");
	CString szSwitchBoxMsg	= _T("");
	CString sRType			= _T("");
	CString sValue			= _T("");
	CString sTmp			= _T("");

	UINT nRtn		= 0;
	UINT nValue		= 0;
	m_strErrMsg		= _T("");

	clock_t		start = clock();
	int			nElapsedTime = 0;

	if(m_bPortOpen == FALSE) return FALSE;

	m_nAvSwitchCmd = SEND_REMOTE_CODE;

	if(nRemoteType == IR_IR){ sRType = "IR";}
	else {sRType = "CA";}

	sTmp.Format("%cCRSND%s%s",STX, sRType, sCustom);

	if(nByte == 1)
	{ 
		if(sCmd.GetLength() == 6){
			szCmdString.Format("%s%s%c%c%c%c%c", sTmp, sCmd, SP, SP, ETX, CR, LF);
		}
		else{
			szCmdString.Format("%s%s%c%c%c%c%c%c%c", sTmp, sCmd, SP, SP, SP, SP, ETX, CR, LF);
		}
	}
	else 
	{
		szCmdString.Format("%s%s%c%c%c", sTmp, sCmd, ETX, CR, LF);;
	}

	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(SEND_REMOTE_CODE, nWaitLimit, sValue);

		if(nRtn == 1)	  {m_strErrMsg = _T("PASS");}
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
		else if(nRtn == 4)
		{
			if(!SendCommString(szCmdString)) 
			{
				m_strErrMsg.Format("SEND FAIL");
				nRtn = 5;
			}
			else
			{
				nRtn = CheckReceiveMsg(SEND_REMOTE_CODE, nWaitLimit, sValue);

				if(nRtn == 1)	  {m_strErrMsg = _T("PASS");}
				else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
				else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
				else if(nRtn == 4)
				{
					m_strErrMsg.Format("BUSY");
				}
			}
		}
	}
	
	nElapsedTime = (clock() - start)*1000/CLOCKS_PER_SEC;
	//-
	
	if(CurrentSet->bCommDisplay)
	{
		sTmp.Format("RemoteKey %s : %s[%dms]", sCmd, m_strErrMsg, nElapsedTime);
		AddStringToRemoteComLog(sTmp);
	}

	if(nRtn == 1 ) return TRUE;
	else 
	{
		AfxMessageBox("Failed to send AvSwitchBox Command!");
		return FALSE;
	}
}

BOOL CAvSwitchBox::CheckAudio(int nCmd, int nWaitLimit, CString& sValue, int nValue1, int nValue2)
{
// 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : Cmd Send Retry, 5 : Send Cmd Fail
	CString szCmdString	= _T("");
	CString szSwitchBoxMsg = _T("");
	CString sTmp = _T("");
	UINT nRtn = 0;
	UINT nValue = 0;
	m_strErrMsg = _T("");


	// Essential Delay for working properly.
//	_Wait(100);

	if(m_bPortOpen == FALSE) return FALSE;

	m_nAvSwitchCmd = nCmd;
	
	szCmdString = MakeCmdString(nCmd, szSwitchBoxMsg, nValue1, nValue2);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(nCmd, nWaitLimit, sValue);

		if(nRtn == 1) m_strErrMsg = _T("PASS");
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
		else if(nRtn == 4)
		{
			if(!SendCommString(szCmdString)) 
			{
				m_strErrMsg.Format("SEND FAIL");
				nRtn = 5;
			}
			else
			{
				nRtn = CheckReceiveMsg(nCmd, nWaitLimit, sValue);

				if(nRtn == 1) m_strErrMsg = _T("PASS");
				else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
				else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
				else if(nRtn == 4)
				{
					m_strErrMsg.Format("BUSY");
				}
			}
		}
	}

	if(nCmd == CHK_AUDIO_LVL)
	{
		nValue = (int)(atof(sValue) * 1000);
		sTmp.Format("%d mv", nValue);
	}
	else if(nCmd == CHK_AUDIO_FRQ)
	{
		nValue = atoi(sValue);
		sTmp.Format("%d hz", nValue);
	}

	if(CurrentSet->bCommDisplay)
	{
		if(szSwitchBoxMsg != _T("")) AddStringToStatus(szSwitchBoxMsg + " : "+ m_strErrMsg);
	}

	if(nRtn == 1) return TRUE;
	else return FALSE;
}
//-

BOOL CAvSwitchBox::InitAvSwitch()
{
	CString sTmp = _T("");
	m_strErrMsg = _T("");

	//------------------------//
	// Init AvSwitchBox       //
	//------------------------//

	// AvSwitchBox Ctrl Port Open
	if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
	{
		AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate);
	}

	// Send <Video/Audio-On> Cmd
	if(AvSwitchBoxCtrl.m_bPortOpen != TRUE)
	{
		if(!AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate)) 
		{
			m_strErrMsg.Format("AVSwitchBox Port Open Error!");
			return FALSE;
		}
	}
	
	//AVSwitchBox Init
	
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sTmp.Format("Set Audio CH 1 %s!\n",  AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}
/*	for(int nTmp = 1; nTmp <= 8; nTmp++)
	{
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_DIGIT_OUT, MAX_AVSWITCH_WAIT_DELAY,  nTmp, 0))
		{
			sTmp.Format("Set DIO Port %d Off %s!\n", nTmp, AvSwitchBoxCtrl.m_strErrMsg);
			m_strErrMsg += sTmp;
		}
	}
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  1, DC_OUT_VOLTAGE_1_0))
	{
		sTmp.Format("Set DC OUT CH1 Volt to 1.0V %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  2, DC_OUT_VOLTAGE_1_0))
	{
		sTmp.Format("Set DC OUT CH2 Volt to 1.0V %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set COMP CH 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set DVBS CH 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}
*/
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nAvSwitchBoxInitVideoCh, 0))
	{
		sTmp.Format("Set VIDEO CH 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}
	if(!AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set Remote Type 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}

	//+add kwmoon 080312
/*	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_REMOTE_REPEAT_NO, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sTmp.Format("Set Remote Repeat No %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}

	//+add kwmoon 080929
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_RGB_SOUND, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sTmp.Format("Set RGB Sound On %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		m_strErrMsg += sTmp;
	}
*/
	return TRUE;
}

BOOL CAvSwitchBox::AudioMeasure()
{
	double Avgfreq_L,Avgfreq_R;
	double AvgLevel_L,AvgLevel_R;

	CString sTmp;
	CString sTemp;
	int aResult[4] = {0,0,0,0};

	if(m_bPortOpen == FALSE) return FALSE;

	float fLowerMargin	 = (float)(1 - (m_fFreqMargin * 0.01));
	float fUpperMargin	 = (float)(1 + (m_fFreqMargin * 0.01));
	float fLevelMargin	 = (float)(1 - (m_fLevelMargin * 0.01));

	m_fMeasuredLeftFreq		= 0;
	m_fMeasuredRightFreq	= 0;
	
	m_fMeasuredLeftLevel	= 0;
	m_fMeasuredRightLevel	= 0;

	m_fLeftFreqLowerLimit	= m_fTargetLeftFreq * fLowerMargin;
	m_fLeftFreqUpperLimit	= m_fTargetLeftFreq * fUpperMargin;

	m_fRightFreqLowerLimit	= m_fTargetRightFreq * fLowerMargin;
	m_fRightFreqUpperLimit	= m_fTargetRightFreq * fUpperMargin;

	m_fLeftLevelLowerLimit	= m_fTargetLeftLevel * fLevelMargin;
	m_fRightLevelLowerLimit	= m_fTargetRightLevel * fLevelMargin;

	
	AvgLevel_L	= 0;
	AvgLevel_R	= 0;
	Avgfreq_L	= 0.0;
	Avgfreq_R	= 0.0;

	if(SendCmd( CHK_AUDIO_LVL ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 0, 0))
	{
		AvgLevel_L = (atof(sTmp) * 1000);
	}

	if(SendCmd( CHK_AUDIO_LVL ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 1, 0))
	{
		AvgLevel_R = (atof(sTmp) * 1000);
	}

	m_fMeasuredLeftLevel  = (float)AvgLevel_L;
	m_fMeasuredRightLevel = (float)AvgLevel_R;


	if(SendCmd( CHK_AUDIO_FRQ ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 0, 0))
	{
		Avgfreq_L = atol(sTmp);
	}
	
	if(SendCmd( CHK_AUDIO_FRQ ,MAX_AVSWITCH_WAIT_DELAY, sTmp, 1, 0))
	{
 		Avgfreq_R = atol(sTmp);
	}

	m_fMeasuredLeftFreq  = (float)Avgfreq_L;
	m_fMeasuredRightFreq = (float)Avgfreq_R;

	
	switch(m_bMeasureAudioType)
	{
		case FREQ_LEVEL_CHECK:  
			sTemp.Format(_T("F:%d,%d/ L:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq, (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
			if((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)){
				aResult[0] = TEST_PASS;
			}
			else {
				aResult[0] = TEST_FAIL;
			}

			if((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit)	&& (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
				aResult[1] = TEST_PASS;
			}
			else {
				aResult[1] = TEST_FAIL;
			}

			if(m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
				aResult[2] = TEST_PASS;
			}
			else {
				aResult[2] = TEST_FAIL;
			}

			if(m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
				aResult[3] = TEST_PASS;
			}
			else {
				aResult[3] = TEST_FAIL;
			}

			if((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS) && (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
			{
				m_bResult = TRUE; 
			}
			else {
				m_bResult = FALSE;
			}
			break;

		case FREQUENCY_CHECK:  
			sTemp.Format(_T("F:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq);
			if((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)){
				aResult[0] = TEST_PASS;
			}
			else {
				aResult[0] = TEST_FAIL;
			}

			if((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit)	&& (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
				aResult[1] = TEST_PASS;
			}
			else {
				aResult[1] = TEST_FAIL;
			}

			if((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS))
			{
				m_bResult = TRUE; 
			}
			else {
				m_bResult = FALSE;
			}
			break;

		case LEVEL_CHECK:  
			sTemp.Format(_T("L:%d,%d"), (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
			if(m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
				aResult[2] = TEST_PASS;
			}
			else {
				aResult[2] = TEST_FAIL;
			}

			if(m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
				aResult[3] = TEST_PASS;
			}
			else {
				aResult[3] = TEST_FAIL;
			}

			if((aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
			{
				m_bResult = TRUE; 
			}
			else {
				m_bResult = FALSE;
			}
			break;
	}
	ctrlStepMeasur_A.SetWindowText(sTemp);
	ctrlStepMeasur_A.Invalidate();
	ctrlStepMeasur_A.UpdateWindow();

	return m_bResult;
}

BOOL CAvSwitchBox::SetVideoOutType(int nVideoSourceType)
{
	// 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : Cmd Send Retry, 5 : Send Cmd Fail
	CString szCmdString		= _T("");
	CString sTmp;
	CString sTemp;
	UINT nRtn		= 0;
	CString sValue			= _T("");

	m_strErrMsg		= _T("");

	clock_t		start = clock();
	int			nElapsedTime = 0;

	if(m_bPortOpen == FALSE) return FALSE;

	if(m_nVideoSourceType == nVideoSourceType)  {
		m_strErrMsg = _T("PASS");
		return TRUE;
	}

	if(nVideoSourceType == V_SRC_NTSC)
	{ 
		szCmdString.Format("%cCSET_NTSC%c%c%c", STX, ETX, CR, LF);
		sTemp = "NTSC";
	}
	else 
	{
		szCmdString.Format("%cCSET_PAL %c%c%c", STX, ETX, CR, LF);
		sTemp = "PAL";
	}

	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(SEND_REMOTE_CODE, MAX_AVSWITCH_WAIT_DELAY, sValue);

		if(nRtn == 1)	  {m_strErrMsg = _T("PASS");}
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
		else if(nRtn == 4)
		{
			if(!SendCommString(szCmdString)) 
			{
				m_strErrMsg.Format("SEND FAIL");
				nRtn = 5;
			}
			else
			{
				nRtn = CheckReceiveMsg(SEND_REMOTE_CODE, MAX_AVSWITCH_WAIT_DELAY, sValue);

				if(nRtn == 1)	  {m_strErrMsg = _T("PASS");}
				else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
				else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
				else if(nRtn == 4)
				{
					m_strErrMsg.Format("BUSY");
				}
			}
		}
	}
	
	nElapsedTime = (clock() - start)*1000/CLOCKS_PER_SEC;
	
	if(CurrentSet->bCommDisplay)
	{
		sTmp.Format("Video Out Type - %s : %s[%dms]", sTemp, m_strErrMsg, nElapsedTime);

		AddStringToRemoteComLog(sTmp);
	}

	m_nVideoSourceType = nVideoSourceType;

	if(nRtn == 1 ) return TRUE;
	else 
	{
		//+add kwmoon 080329
		AfxMessageBox("Failed to send AvSwitchBox Command!");
		return FALSE;
	}
}

BOOL CAvSwitchBox::SetBuzzerMute(BOOL bFlag)
{
	// 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : Cmd Send Retry, 5 : Send Cmd Fail
	CString szCmdString		= _T("");
	UINT nRtn		= 0;
	CString sValue			= _T("");

	m_strErrMsg		= _T("");

	if(m_bPortOpen == FALSE) return FALSE;

	if(bFlag)
	{ 
		szCmdString.Format("%cCSET_BUZ%c%c%c%c", STX, MUTE_ON, ETX, CR, LF);
	}
	else 
	{
		szCmdString.Format("%cCSET_BUZ%c%c%c%c", STX, MUTE_OFF, ETX, CR, LF);
	}

	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(SEND_REMOTE_CODE, MAX_AVSWITCH_WAIT_DELAY, sValue);

		if(nRtn == 1)	  {m_strErrMsg = _T("PASS");}
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
		else if(nRtn == 4)
		{
			if(!SendCommString(szCmdString)) 
			{
				m_strErrMsg.Format("SEND FAIL");
				nRtn = 5;
			}
			else
			{
				nRtn = CheckReceiveMsg(SEND_REMOTE_CODE, MAX_AVSWITCH_WAIT_DELAY, sValue);

				if(nRtn == 1)	  {m_strErrMsg = _T("PASS");}
				else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
				else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
				else if(nRtn == 4)
				{
					m_strErrMsg.Format("BUSY");
				}
			}
		}
	}
	
	if(nRtn == 1 ) return TRUE;
	else 
	{
		//+add kwmoon 080329
		AfxMessageBox("Failed to send AvSwitchBox Command!");
		return FALSE;
	}
}
