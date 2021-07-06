#include "stdafx.h"
#include <conio.h>
#include <Afxmt.h>
#include <malloc.h>

#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "PatternGeneratorCtl.h"
#include "AnalogGrabber.h"
#include "TVCommCtrl.h"
#include "AvSwitchBoxCtrl.h"
#include "ImageProc.h"
#include "BaseImage.h"
#include "ProgressCtrlX.h"
#include "UserMsgDlg.h"
#include "CurrentSetting.h"
#include "SoundCard.h"

#include "UsbDio.h"
#include "PciDio.h"

///////////////////////
// External Variable //
///////////////////////

extern CProgressCtrlX	ctrlImgProg;
extern CBaseImage		g_GrabImage;
extern CBaseImage		g_RefImage;
extern CBaseImage		g_MaskImage;
extern CImageProc		g_ImageProc;
extern UINT				m_nSelectedImage;
extern CDATsysView*		g_pView;
//extern int				g_nNoFailedStep;
extern CCurrentSetting	g_CurSetting;
extern BYTE GrabData[1920][1080][3];
extern BOOL  RefImage_DirectLoad(CString szRefFileName,int nUseMaskImage);
extern CColorStaticST		ctrlStepTarget_A;

//extern CSoundCard		g_SoundCard;
extern _InputSource		InputDefined[];

/////////////////////
// Global Variable //
/////////////////////

//UINT		g_GrabSource;
UINT		g_VideoSourceType;
POSITION	pos;


__inline int GetInteger()
{
	Argument* pArg = pCurFunc->m_ArgumentList.GetNext(pos);
	return pArg->Integer;
}

__inline double GetDouble()
{
	Argument* pArg = pCurFunc->m_ArgumentList.GetNext(pos);
	return pArg->Double;
}

__inline char* GetString()
{
	Argument* pArg = pCurFunc->m_ArgumentList.GetNext(pos);
	return pArg->String;
}

__inline BOOL GetBoolean()
{
	Argument* pArg = pCurFunc->m_ArgumentList.GetNext(pos);
	return pArg->Boolean;
}

BOOL _UserMessage()
{
	UINT nDelay = 0;

	CString szMsg = _T("");
	CString szUserMsg = _T("");

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		szMsg  = GetString();
		nDelay = GetInteger();

		ShowSubStepMessage(szMsg, "User Message");
	}

	if(!_Wait(nDelay)) return FALSE;

	return TRUE;
}

BOOL _NotifyMessage()
{

	CString sMsg1;
	CString sMsg2;
	BOOL    bRetrun = TRUE;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
        sMsg1 = GetString();
        sMsg2 = GetString();

		g_pView->m_UserMsgDlg.SetMessage(sMsg1,sMsg2);
		g_pView->m_UserMsgDlg.DoModal();

		bRetrun = g_pView->m_UserMsgDlg.m_bYes;
	
	}

	if(!bRetrun) CurrentSet->bRunAbort = TRUE;
//090408
//	return bRetrun;
	return TRUE;
}

BOOL _Delay()
{
	UINT nDelay = 0;
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nDelay = GetInteger();
		
	}

	if(!_Wait(nDelay)) return FALSE;
	return TRUE;
}

BOOL _Work()
{
	BOOL bResult = FALSE;

	if(g_pView->m_bMakeReferenceMode)
	{
		bResult = Work_MakeRef();
	}
	else
	{
		bResult = Work_Normal();
	}

	return bResult;
}

BOOL Work_Normal()
{
	BOOL bTestResult = FALSE;

	//+chage kwmoon 080402
	double reval = 0;

	CString sMsg	  = _T("");
	CString szPrevMsg = _T("");
	 
	DWORD dwAudioMeasureThreadResult = 0;
	DWORD dwExitCode = 0;

	ShowSubStepMessage(sMsg, "Processing");
	
	if(pCurStep->m_pFunc != NULL)
	{
		reval = (*pCurStep->m_pFunc)();
		CheckValue(reval);
	}

	if(pCurStep->m_bRunVideoTest)
	{ 
		if(pCurStep->GetResult() == FALSE)
		{
			pCurStep->m_bVideoTestResult = FALSE;
		}
		else
		{
			pCurStep->m_bVideoTestResult = TRUE;
		}
	}

	//=================
	// With Audio Test 
	//=================
	if(pCurStep->m_bRunAudioTest == TRUE)
	{
//		if(CurrentSet->bUseAVSwitchBox == TRUE)
		if(pCurStep->m_nTestType != AUDIO_TEST)
		{
			if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX){
				if(!CurrentSet->bUseAVSwitchBox) {
					pCurStep->m_bAudioTestResult = FALSE;
				}
				else {
					if(AvSwitchBoxCtrl.AudioMeasure() == TRUE){
						AvSwitchBoxCtrl.AudioMeasure();
					}
				}
			}
			else {
				if(g_pView->m_SignalEnable == 1)////g_pView->m_nMeasureWait > 0)
				{
					clock_t		start;
					start = clock();

					while (TRUE)
					{
						if(g_pView->AudioMeasure()) {
							break;
						}

						if ((clock() - start)*1000/CLOCKS_PER_SEC > g_pView->m_nMeasureWait)
						{
							break;
						}

						if(CurrentSet->bRunAbort)
						{
							break;
						}
					}

				}
				else{
					if(!g_pView->AudioMeasure()) {
						_Wait(800);
						if(!g_pView->AudioMeasure()){
							_Wait(1500);
							g_pView->AudioMeasure();
						}
					}
				}
			}

		}

		if(((CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX) && (AvSwitchBoxCtrl.m_bResult == FALSE))
		|| ((CurrentSet->nAudioCheckMethod == METHOD_SOUNDCARD)	 && (g_pView->m_bAudio_Result == FALSE)))
		{
			if(!g_pView->m_bAdjSpecMode)
			{
				pCurStep->m_bAudioTestResult = FALSE;
				pCurStep->SetResult(FALSE);

				sMsg.Format("[A/NG]");
			}
			else // if g_pView->m_bAdjSpecMode
			{
				pCurStep->m_bAudioTestResult = TRUE;

				sMsg = _T("");
			}
		}
		else
		{
			pCurStep->m_bAudioTestResult = TRUE;

			if(!g_pView->m_bAdjSpecMode)
			{
				sMsg.Format("[A/OK]");
			}
			else
			{
				sMsg = _T("");
			}
		}

		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			pCurStep->m_nLeftFreq[0] 	 = (int)AvSwitchBoxCtrl.m_fTargetLeftFreq;
			pCurStep->m_nRightFreq[0]	 = (int)AvSwitchBoxCtrl.m_fTargetRightFreq;
			pCurStep->m_nLeftLevel[0]	 = (int)AvSwitchBoxCtrl.m_fTargetLeftLevel;
			pCurStep->m_nRightLevel[0]	 = (int)AvSwitchBoxCtrl.m_fTargetRightLevel;

			pCurStep->m_nLeftFreq[1] 	 = (int)AvSwitchBoxCtrl.m_fMeasuredLeftFreq;
			pCurStep->m_nRightFreq[1]	 = (int)AvSwitchBoxCtrl.m_fMeasuredRightFreq;
			pCurStep->m_nLeftLevel[1]	 = (int)AvSwitchBoxCtrl.m_fMeasuredLeftLevel;
			pCurStep->m_nRightLevel[1]	 = (int)AvSwitchBoxCtrl.m_fMeasuredRightLevel;
		}
		else
		{
			pCurStep->m_nLeftFreq[0] 	 = (int)g_pView->m_fTargetLeftFreq;
			pCurStep->m_nRightFreq[0]	 = (int)g_pView->m_fTargetRightFreq;
			pCurStep->m_nLeftLevel[0]	 = (int)g_pView->m_fTargetLeftLevel;
			pCurStep->m_nRightLevel[0]	 = (int)g_pView->m_fTargetRightLevel;

			pCurStep->m_nLeftFreq[1] 	 = (int)g_pView->m_fMeasuredLeftFreq;
			pCurStep->m_nRightFreq[1]	 = (int)g_pView->m_fMeasuredRightFreq;
			pCurStep->m_nLeftLevel[1]	 = (int)g_pView->m_fMeasuredLeftLevel;
			pCurStep->m_nRightLevel[1]	 = (int)g_pView->m_fMeasuredRightLevel;
		}

		if(g_pView->m_bAdjSpecMode)
		{
			pCurStep->m_nAudioMargin	 = (int)CurrentSet->nFreqMargin;
		}
		else
		{
			if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
			{
				pCurStep->m_nAudioMargin	 = (int)AvSwitchBoxCtrl.m_fFreqMargin;
			}
			else
			{
				pCurStep->m_nAudioMargin	 = (int)g_pView->m_fFreqMargin;
			}
		}

		if(CurrentSet->nDisplayType == DETAILEDGRID)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 

			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " +sMsg);
				pCurStep->m_strMsg = szPrevMsg + " , " + sMsg;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
				pCurStep->m_strMsg = sMsg;
			}
		}
	}


//	if(!pCurStep->m_bVideoTestResult 
//	|| (pCurStep->m_bRunAudioTest && !pCurStep->m_bAudioTestResult))
	if(pCurStep->GetResult() == FALSE)
	{
		//g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpFail);
		return TEST_FAIL;
	}
	else
	{
		g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpGood);
		return TEST_PASS;
	}

}

BOOL Work_MakeRef()
{
	BOOL bTestResult = FALSE;

	//+chage kwmoon 080402
	double reval = 0;

	CString sMsg	  = _T("");
	CString szPrevMsg = _T("");
	 
	DWORD dwAudioMeasureThreadResult = 0;
	DWORD dwExitCode = 0;

	ShowSubStepMessage(sMsg, "Processing");

	//+add kwmoon 071206
	//+add kwmoon 071001
	if(pCurStep->m_nTestType == MOVING_PIC_TEST)
	{
		_Wait(5000);
	}

	//change PSH 090331
	//+add kwmoon 071001
//	if((pCurStep->m_nTestType == AUDIO_TEST) && (pCurStep->m_pFunc != NULL))
	if((pCurStep->m_nTestType >= LAN_TEST) && (pCurStep->m_pFunc != NULL))
	{
		reval = (*pCurStep->m_pFunc)();
		CheckValue(reval);
	}
	
	pCurStep->m_bVideoTestResult = TRUE;

	g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpGood);

	return TRUE;
}

BOOL _PatternGen_Out()
{
	BYTE nCmd = 0;
	int nData = 0;
	int nPatternNo;
	int nModelNo;
	CString sTemp;
	CString sBuf;
	BOOL bReturn = FALSE;

	CString sMsg = _T("");

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


	if (pos!= NULL)
	{
		nCmd = GetInteger();
		nData = GetInteger();
		sMsg = GetString();
	}
	else 
	{
		return FALSE;
	}

//del 090525	
//	if((!pCurStep->m_bStepRetryFlag) && g_CurSetting.IsSamePatternGenNo(nData))
//	{
//		return TRUE;
//	}

	sTemp.Format("%d",nData);
	if(sTemp.GetLength() <= 2) return FALSE;
	sBuf = sTemp.Right(2);
	nPatternNo = atoi(sBuf);

	sBuf = sTemp.Left(sTemp.GetLength() - 2);
	nModelNo = atoi(sBuf);
	
	ShowSubStepMessage(sMsg, "Pattern Gen Out");
	
	bReturn = PatternGeneratorCtrl.SetOut(nModelNo, nPatternNo);

//	PatternGeneratorCtrl.SendRun((BYTE)nCmd, (int)nData);

	return bReturn;
}

/*
BOOL _SetLvdsResolution_2in1()
{
	//+add kwmoon 080402
	int nGrabWidth  = 0;
	int nGrabHeight = 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nGrabWidth = GetInteger();
		nGrabHeight = GetInteger();

		sMsg = GetString();
	}
	else return FALSE;

	//+add kwmoon 070929
	if((!pCurStep->m_bStepRetryFlag) && g_CurSetting.IsSameLvdsResolution(nGrabWidth,nGrabHeight))
	{
		return TRUE;
	}

	ShowSubStepMessage(sMsg, "Set Lvds Resolution");


	CurrentSet->nLvdsWidth  = nGrabWidth;
	CurrentSet->nLvdsHeight = nGrabHeight;

	g_pView->SetGrabInfo(&g_GrabImage);

	return TRUE;
}
*/
BOOL _SetPositionShift()
{
	POINT ptPositionShift;

	//+add kwmoon 080402
	ptPositionShift.x = 0;
	ptPositionShift.y = 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		ptPositionShift.x = GetInteger();
		ptPositionShift.y = GetInteger();

		sMsg = GetString();
	}
	else return FALSE;

	ShowSubStepMessage(sMsg, "Set Position Shift");

	g_ImageProc.SetPositionShift(ptPositionShift);

	return TRUE;
}

BOOL _SetAvgFilter()
{
	int nMaskWidth  = 0;
	int nMaskHeight = 0;
	int nNoRepeat	= 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nMaskWidth  = GetInteger();
		nMaskHeight = GetInteger();
		nNoRepeat	= GetInteger();

		sMsg = GetString();
	}
	else return FALSE;

	ShowSubStepMessage(sMsg, "Set Average Filter");

	g_ImageProc.SetAvgMaskSize(nMaskWidth,nMaskHeight,nNoRepeat);

	return TRUE;
}

BOOL _UseOnlyColorHighBits()
{

	//+change kwmoon 080402
	int nNoUsedColorHighBits = 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nNoUsedColorHighBits	 = GetInteger();

		sMsg = GetString();
	}
	else return FALSE;

	if((!pCurStep->m_bStepRetryFlag) && g_CurSetting.IsSameUseColorBits(nNoUsedColorHighBits))
	{
		return TRUE;
	}
	ShowSubStepMessage(sMsg, "Use Only Color High Bits");
	g_ImageProc.SetUsedNoColorHighBits(nNoUsedColorHighBits);

	return TRUE;
}

BOOL _UseOnlyBrightHighBits()
{
	//+change kwmoon 080402
	int nNoUsedBrightHighBits = 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nNoUsedBrightHighBits	 = GetInteger();

		sMsg = GetString();
	}
	else return FALSE;

	if((!pCurStep->m_bStepRetryFlag) && g_CurSetting.IsSameUseBrightBits(nNoUsedBrightHighBits))
	{
		return TRUE;
	}
	ShowSubStepMessage(sMsg, "Use Only Bright High Bits");

	g_ImageProc.SetUsedNoBrightHighBits(nNoUsedBrightHighBits);
	return TRUE;
}

//+add kwmoon 071004
BOOL _SetMatchRate()
{
	float fMatchRate = 0.0f;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{ 
		fMatchRate = (float)GetDouble();
		sMsg	   = GetString();
	}
	else return FALSE;

	//+add kwmoon 070929
	if((!pCurStep->m_bStepRetryFlag) && g_CurSetting.IsSameMatchRate(fMatchRate))
	{
		return TRUE;
	}

	ShowSubStepMessage(sMsg, "Set Match Rate");

	g_ImageProc.SetMatchRate(fMatchRate);
	return TRUE;
}


BOOL _Remote_Out()
{
	//+change kwmoon 080402
	CString sCmd = _T("");
	
	int nClickNum = 0;
	int nDelay	  = 0;

	CString sMsg = _T("");
	CString sCustom = _T("LG");

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCmd	  = GetString();
		nClickNum = GetInteger();
		sMsg	  = GetString();
		nDelay    = GetInteger();
	}
	else 
	{
		return FALSE;
	}

	switch(CurrentSet->nRemoteCustomCode)
	{
		case REMOCON_HT:
		case REMOCON_MINI:
		case REMOCON_BD:
			sCustom = "LG";
			break;

		case REMOCON_PN:
			sCustom = "PN";
			break;

		case REMOCON_LO:
			sCustom = "LO";
			break;

		case REMOCON_NA:
		case REMOCON_CM:
		case REMOCON_GS:
		case REMOCON_TB:
		case REMOCON_SH:
		case REMOCON_SI:
			sCustom = "NA";
			break;

		case REMOCON_SO:
			sCustom = "SO";
			break;

		case REMOCON_JV:
			sCustom = "JV";
			break;

	}


	//+add kwmoon 080318
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag))
	{
		CString szMsg;

		szMsg.Format("%d.Remote_Out(%s) is skipped", pCurStep->m_nStep, sCmd, 1);
		AddStringToRemoteComLog(szMsg);
		return TRUE;
		
	/*	if((CurrentSet->nRemoteCustomCode == 0) && (sCmd.Compare("AA") == NULL)) 
		{
			szMsg.Format("%d.Remote_Out(AA) is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(szMsg);
			return TRUE;
		}
		else if((CurrentSet->nRemoteCustomCode == 1) && (sCmd.Compare("05") == NULL)) 
		{
			szMsg.Format("%d.Remote_Out(05) is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(szMsg);
			return TRUE;
		}
		else if((CurrentSet->nRemoteCustomCode == 2) && (sCmd.Compare("58") == NULL)) 
		{
			szMsg.Format("%d.Remote_Out(58) is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(szMsg);
			return TRUE;
		}
	*/
	}


	ShowSubStepMessage(sMsg, "Remote Out");
	//+ 2007.12.4 Add BY USY
	if(CurrentSet->bUseAVSwitchBox)
	{
		if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
		{
			if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
			{
				AvSwitchBoxCtrl.PortClose();
				if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
				{
					//+add kwmoon 080313
					CString szErrMsg;
					szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
					AfxMessageBox(szErrMsg);
					return FALSE;
				}
			}
		}
		for(int nTmp = 1; nTmp <= nClickNum; nTmp++)
		{
			// change 090408 
		//	if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sCmd))
		//	{
		//		return FALSE;
		//	}
		//	if(!_Wait(nDelay)) return FALSE;
			AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, 1);
			_Wait(nDelay);
		}
		AvSwitchBoxCtrl.m_sRemoteCode = sCmd;
		AvSwitchBoxCtrl.m_nRemoteDelay = nDelay;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL _SetTVCommand()
{
	CString sCmd1 = _T("");
	int nWaitLimit = 0;
	int nResult = TEST_FAIL;
	BYTE nCmd;
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCmd1	  = GetString();
		nWaitLimit= GetInteger();
	}
	else 
	{
		return FALSE;
	}

	
//	TVCommCtrl.m_aCmd1.Add(sCmd1);
//	TVCommCtrl.m_aWaitLimit.Add(nWaitLimit);
	TVCommCtrl.m_sCheckCmd = sCmd1;
	TVCommCtrl.m_nWaitLimit = nWaitLimit;

	nCmd = (BYTE)hexCstr2decNum(TVCommCtrl.m_sCheckCmd);

	//	nResult = TVCommCtrl.IfTest(nCmd, TVCommCtrl.m_aWaitLimit.GetAt(i));
	nResult = TVCommCtrl.IfTest(nCmd, TVCommCtrl.m_nWaitLimit);

	if (nResult != TEST_PASS)
	{
		//	TVCommCtrl.ResetArray();

		return 0.0;
	}
	//		_Wait(200);
	//	}

	//	TVCommCtrl.ResetArray();

	return 1.0;

	return TRUE;	
}

//+ 2007.11.28 Add BY USY
BOOL _CheckAdcResult()
{
	//+chage kwmoon 080402
//	CString sCmd1, sCmd2, sData, sTargetAvgData;
	CString sCmd1 = _T("");
	CString sCmd2 = _T("");
	CString sData = _T("");
	CString sTargetAvgData = _T("");
	
	CString sMsg = _T("");

	int nSetId		= 0;
	int nWaitLimit	= 0;
	int nCheckType	= 0;
	int nTargetMargin = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCmd1	  = GetString();
		sCmd2	  = GetString();

		nSetId	  = GetInteger();
		sData	  = GetString();
		nWaitLimit= GetInteger();
		nCheckType = GetInteger();
		sTargetAvgData = GetString();
		nTargetMargin = GetInteger();

		sMsg	  = GetString();
	}
	else 
	{
		return FALSE;
	}

	//+add kwmoon 080724
	CurrentSet->nAdcType = ADC_TYPE_RS232C;

	//+add kwmoon 080509
	ShowSubStepMessage(sMsg, "Check ADC Result");
	
	TVCommCtrl.m_aCmd1.Add(sCmd1);
	TVCommCtrl.m_aCmd2.Add(sCmd2);
	TVCommCtrl.m_aSetId.Add(nSetId);
	TVCommCtrl.m_aData.Add(sData);
	TVCommCtrl.m_aWaitLimit.Add(nWaitLimit);
	// change 090417
	TVCommCtrl.m_aCheckType.Add(nCheckType);
	TVCommCtrl.m_aCheckOption.Add(0);
	TVCommCtrl.m_strTargetAvg = sTargetAvgData;
	TVCommCtrl.m_nAvgMargin = nTargetMargin;
	//+ 2007.12.5 Add BY USY
	pCurStep->m_bRunAdcTest = TRUE;
	//-
	TVCommCtrl.m_nResultCheckType = 0;
	return TRUE;	
}

//add 090417
BOOL _CheckAdcResult2()
{
	int nTargetData1 = 0;
	int nTargetData2 = 0;
	int nTargetData3 = 0;
	int nTargetMargin = 0;
	int nWaitLimit	= 0;
	int nCheckType	= 0;
	int nCheckOption = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nCheckType = GetInteger();
		nTargetData1 = GetInteger();
		nTargetData2 = GetInteger();
		nTargetData3 = GetInteger();
		nTargetMargin = GetInteger();
		nCheckOption = GetInteger();
		nWaitLimit = GetInteger();
	}
	else 
	{
		return FALSE;
	}

	//+add kwmoon 080724
	CurrentSet->nAdcType = ADC_TYPE_RS232C;

	//+add kwmoon 080509
	ShowSubStepMessage("", "Check ADC Result");
	
	TVCommCtrl.m_aCmd1.Add("a");
	TVCommCtrl.m_aCmd2.Add("d");
	TVCommCtrl.m_aSetId.Add(0);
	TVCommCtrl.m_aData.Add("20");
	TVCommCtrl.m_aWaitLimit.Add(nWaitLimit);
	TVCommCtrl.m_aCheckType.Add(nCheckType);
	TVCommCtrl.m_aCheckOption.Add(nCheckOption);
	pCurStep->m_bRunAdcTest = TRUE;

	switch(nCheckType)
	{
		case 1:
		case 4:
			TVCommCtrl.m_nResultTarget[0] = nTargetData1;
			TVCommCtrl.m_nResultTarget[1] = nTargetData2;
			TVCommCtrl.m_nResultTarget[2] = nTargetData3;
			TVCommCtrl.m_nResultMargin[0] = nTargetMargin;
			break;
	
		case 2:
		case 5:
			TVCommCtrl.m_nResultTarget[3] = nTargetData1;
			TVCommCtrl.m_nResultTarget[4] = nTargetData2;
			TVCommCtrl.m_nResultTarget[5] = nTargetData3;
			TVCommCtrl.m_nResultMargin[1] = nTargetMargin;
			break;

		case 3:
		case 6:
			TVCommCtrl.m_nResultTarget[6] = nTargetData1;
			TVCommCtrl.m_nResultTarget[7] = nTargetData2;
			TVCommCtrl.m_nResultTarget[8] = nTargetData3;
			TVCommCtrl.m_nResultMargin[2] = nTargetMargin;
			break;
	}

	TVCommCtrl.m_nResultCheckType = 1;

	return TRUE;	
}

BOOL _SendTVCommand()
{
	CString sCmd1 = _T("");
	
	CString sMsg = _T("");

	int nWaitLimit = 0;
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCmd1	  = GetString();

		nWaitLimit= GetInteger();
	}
	else 
	{
		return FALSE;
	}


	int nResult = TVCommCtrl.IfTest((BYTE)hexCstr2decNum(sCmd1),nWaitLimit, FALSE);

	if(nResult != TEST_PASS)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL _SendTVCommand2()
{
	CString sCmd1 = _T("");
	BOOL bResult = FALSE;
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCmd1	  = GetString();

	}
	else 
	{
		return FALSE;
	}


	bResult = TVCommCtrl.SendCommand((BYTE)hexCstr2decNum(sCmd1));

	return bResult;
}

BOOL _SetTvCommPortEcho()
{
	CString sMsg = _T("");

	//+chage kwmoon 080402
	int nTvCommPortEcho = 0;
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nTvCommPortEcho	  = GetInteger();
	}
	else 
	{
		return FALSE;
	}

	//+2007.11.21 Add BY USY
	ShowSubStepMessage(sMsg, "TVCommPort Echo");
	
	if(nTvCommPortEcho == 1)
	{
		CurrentSet->bTVCommPortEcho = TRUE;
	}
	else if(nTvCommPortEcho == 0)
	{
		CurrentSet->bTVCommPortEcho = FALSE;
	}

	return TRUE;
}

//-
//+add kwmoon 071201
BOOL _SetAnalogVideoSource()
{
	int nVideoSource	= 1; // ANALOG
	int nVideoRfType	= 0;
	int nVideoAvChannel = 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nVideoRfType	= GetInteger();
		nVideoAvChannel = GetInteger();
		sMsg			= GetString();
	}
	else return FALSE;

	if(AvSwitchBoxCtrl.m_bPortOpen == TRUE)
	{
		AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  nVideoAvChannel, 0);
	}
	else
	{
		if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
		{
			AvSwitchBoxCtrl.PortClose();

			if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
		AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  nVideoAvChannel, 0);
	}

	ShowSubStepMessage(sMsg, "Set Video Source");
	

//	g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1

	switch(nVideoRfType)
	{
		case 0 : g_VideoSourceType = V_SRC_NTSC; break; // V_SRC_NTSC : 0
		case 1 : g_VideoSourceType = V_SRC_PAL;  break; // V_SRC_PAL  : 1
	}

	return TRUE;
}

BOOL _SetRoi()
{
	POINT ptStartPosition;
	ptStartPosition.x = 0;
	ptStartPosition.y = 0;
	
	POINT ptEndPosition;
	ptEndPosition.x = 0;
	ptEndPosition.y = 0;

	int nWidth  = 0;
	int nHeight = 0;

	CString sMsg = _T("");

	//add 090520
	if(g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		ptStartPosition.x = GetInteger();
		ptStartPosition.y = GetInteger();
		ptEndPosition.x   = GetInteger();
		ptEndPosition.y   = GetInteger(); 
		
		sMsg = GetString();
	}
	else return FALSE;

	//+change kwmoon 080624
	CString szErrMsg = _T("");

	//+add kwmoon 080722
	if(ptEndPosition.x <= ptStartPosition.x)
	{
		szErrMsg.LoadString(ERROR_ROI_X_RANGE);
		AfxMessageBox(szErrMsg);
		return FALSE;
	}

	if(ptEndPosition.y <= ptStartPosition.y)
	{
		szErrMsg.LoadString(ERROR_ROI_X_RANGE);
		AfxMessageBox(szErrMsg);
		return FALSE;
	}
	
	if((ptStartPosition.x < 0) || (ptEndPosition.x > (int)(CurrentSet->nAnalogWidth-1)))
	{
		szErrMsg.Format("Roi must be in the range of 0 to %d",CurrentSet->nAnalogWidth-1);
		AfxMessageBox(szErrMsg);
		return FALSE;
	}

	if((ptStartPosition.y < 0) || (ptEndPosition.y > (int)(CurrentSet->nAnalogHeight-1)))
	{
		szErrMsg.Format("Roi must be in the range of 0 to %d",CurrentSet->nAnalogHeight-1);
		AfxMessageBox(szErrMsg);
		return FALSE;
	}

	//+add kwmoon 071227
	g_CurSetting.m_nRoiX1 = g_ImageProc.m_ptRoiPosition.x;
	g_CurSetting.m_nRoiY1 = g_ImageProc.m_ptRoiPosition.y;
	g_CurSetting.m_nRoiX2 = g_CurSetting.m_nRoiX1 + g_ImageProc.m_nRoiWidth;
	g_CurSetting.m_nRoiY2 = g_CurSetting.m_nRoiY1 + g_ImageProc.m_nRoiHeight;


	//+del kwmoon 080617
	//+add kwmoon 070929
//	if((!pCurStep->m_bStepRetryFlag) && g_CurSetting.IsSameRoi(ptStartPosition.x,ptStartPosition.y,ptEndPosition.x,ptEndPosition.y))
//	{
//		return TRUE;
//	}

	ShowSubStepMessage(sMsg, "Set Roi");

	g_ImageProc.SetRoi(ptStartPosition,ptEndPosition);

	AnalogControl.OverlayDraw(g_ImageProc.m_ptRoiPosition.x, g_ImageProc.m_ptRoiPosition.y, g_ImageProc.m_nRoiWidth,g_ImageProc.m_nRoiHeight);
	//+add kwmoon 080617
	CRect nImageRect;
	CWnd* pImageWnd;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);
	
	g_pView->InvalidateRect(nImageRect, FALSE);
//	g_pView->UpdateWindow();
	//-
	
	//+add kwmoon 080618
	g_pView->SendMessage(UM_UPDATE_DISPLAY,ALL_VIEW, 0);
	
	return TRUE;
}

BOOL _Wait(int nMillisecond)
{
	DWORD dwStartTick = 0, dwCurrentTick = 0;

	//+add kwmoon 080618
//	MSG msg;

	dwStartTick = GetTickCount();

	//+del kwmoon 080809
//	if(nMillisecond <= 100)
//	{
//		Sleep(nMillisecond); return TRUE;
//	}
	
	while(TRUE)
	{
		
	/*	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
		}
	*/	


		if(CurrentSet->bRunAbort)
		{
			return FALSE;
		}

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMillisecond) break;

		//+del kwmoon 080904
/*		//+add kwmoon 080809
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			GetMessage(&msg,NULL,NULL,NULL);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//-
*/
		Sleep(10);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//AvSwitch Cmd Version 1
// 1st parameter : Cmd Type -> 1 : VIDEO_CH_SELECT,    2 : VIDEO_SIGNAL_ON_OFF
//							   3 : AUDIO_CH_SELECT,    4 : AUDIO_SIGNAL_ON_OFF
//							   5 : SET_DIO_SIGNAL
//							   6 : SET_DC_OUT_SIGNAL   
//							   7: COMPONENT_CH_SELECT
//							   8: CVBS_CH_SELECT	
// 2nd parameter : Value1    -> VIDEO_CH_SELECT (1~4),   VIDEO_SIGNAL_ON_OFF(0:OFF, 1:ON)
//							 -> AUDIO_CH_SELECT (1~8),   AUDIO_SIGNAL_ON_OFF(0:OFF, 1:ON)
//							 -> SET_DIO_SIGNAL(port : 1~8)
//							 -> SET_DC_OUT_SIGNAL(port : 1~2)
//							 -> COMPONENT_CH_SELECT (1~2)
//							 -> CVBS_CH_SELECT (1~2)
// 3rd parameter : Value2    -> VIDEO_CH_SELECT (0),   VIDEO_SIGNAL_ON_OFF(0)
//							 -> AUDIO_CH_SELECT (0),   AUDIO_SIGNAL_ON_OFF(0)
//  						 -> SET_DIO_SIGNAL(0:OFF, 1:ON)
// 							 -> SET_DC_OUT_SIGNALDC_OUT_VOLTAGE(1:1.0V, 2:2.0V, 3:2.5V, 4:3.0V)
//							 -> COMPONENT_CH_SELECT(0)
//							 -> CVBS_CH_SELECT(0)

//	SetAvSwitchBoxSignal(CmdType,Value1,,Value2,"")

/////////////////////////////////////////////////////////////////////////////
//AvSwitch Cmd Version 2
// 1st parameter : Cmd Type -> 3 : SET_AUDIO_CH,		4 : SET_AUDIO_ON_OFF
//							   5 : SET_DIGIT_OUT,		6 : SET_ANALOG_VOLT
//							   7 : SET_COMP_CH,			8 : SET_CVBS_CH  
//							   9 : CHK_AUDIO_LVL		10 : CHK_AUDIO_FRQ   
//							   11 : SEND_REMOTE_CODE	12 : PULLUP_AVSWITCH
//							   13 : SET_VIDEO_CH		14 : REMOTE_TYPE_SEL
//+add kwmoon 080312
//							   15 : SET_REMOTE_REPEAT_NO		

// 2nd parameter : nValue1    -> Audio Channel  (1 ~ 8),				AUDIO_ON_OFF(0:Off, 1:On)
//							 -> Analog Port (1 : Port 1, 2 : Port 2),   
//							 ->	Digital Out Port(00, 01,02, 03~)
//							 -> COMPONENT Channel ( 1 : Ch 1, 2 : Ch 2),CVBS Channel ( 1 : Ch 1, 2 : Ch 2)
//							 -> Audio Volt ( 0 : Left, 1 : Right),		Audio Freq ( 0 : Left, 1 : Right)
//							 -> Remote Code(Integer Type -> Change Hex),PullUp Volt ( 3 : 3.3V, 5 : 5V )
//							 -> Video Channel(1 : CH1, 2 : Ch2)		   ,Remote Type( 1: ,2 : )

// 3rd parameter : nValue2    -> Don't Care,								Don't Care
//							 -> Analog Volt ( 00 : 0V, 01 : 1V, 02 : 2V, 03 : 2.5V, 04 : 3.3V,   
//							 -> Digital Out On / Off( 0 : Off , 1 : On)
//							 -> Don't Care,								Don't Care,
//							 -> Don't Care,								Don't Care,
//							 -> Don't Care,	
//							 -> Don't Care,								Don't Care
/////////////////////////////////////////////////////////////////////////////


BOOL _SetAvSwitchBoxSignal()
{
	int nSwitchButtonNo		= 0;
	int nSwitchButtonValue1	= 0;
	int nSwitchButtonValue2	= 0;
	int nDelay				= 0;

	CString sMsg = _T("");

	BOOL bReturn = FALSE;
	
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nSwitchButtonNo		= GetInteger();
		nSwitchButtonValue1 = GetInteger();
		nSwitchButtonValue2 = GetInteger();
		nDelay				= GetInteger();
		sMsg = GetString();
	}
	else return FALSE;
	
	//+add kwmoon 070929
	if((!pCurStep->m_bStepRetryFlag) && g_CurSetting.IsSameAvSwitchBoxSignal(nSwitchButtonNo,nSwitchButtonValue1,nSwitchButtonValue2))
	{
		return TRUE;
	}

	ShowSubStepMessage(sMsg, "Set AvSwitchBox");
	
	if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
	{
		if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
		{
			AvSwitchBoxCtrl.PortClose();
			
			if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}

	//+2007.12.26 Mod BY USY
	if(!AvSwitchBoxCtrl.SetAvSwitch(nSwitchButtonNo, MAX_AVSWITCH_WAIT_DELAY, nSwitchButtonValue1, nSwitchButtonValue2))
	{
		//+add kwmoon 080514
		CString szErrMsg;
		szErrMsg.Format("Failed to send AvSwitchBox command");
		AfxMessageBox(szErrMsg);
		return FALSE;
	}
	bReturn = TRUE;

	_Wait(nDelay);

	
	return bReturn;
}

BOOL _Grab_N_Load_Image()
{
	BOOL bResult = FALSE;

	if(g_pView->m_bMakeReferenceMode)
	{
		bResult = Grab_N_Load_Image_MakeRef_2in1();
	}
	else
	{
		bResult = Grab_N_Load_Image_Normal_2in1();
	}
	return bResult;
}

UINT Grab_N_Load_Image_Normal_2in1()
{
//	int nWidth				= 0;
//	int nHeight				= 0;
//	int nImageSize			= 0;
//	int nErrorCode			= 0;
//	int nImageBufferSize	= 0;
//	int nRowWidth			= 0;
	int nUseMaskImage		= 0;	// 0 : Dont't Use, 1 : Use
//	int nGrabResult			= GRAB_FAIL;

	CString sRefTmp		= _T("");
	CString sGrabPath	= _T("");
	CString sRefPath	= _T("");
	CString sMsg		= _T("");
	CString szFileName	= _T("");
	CString szErrorMsg	= _T("");


//	CRect nImageRect;
//	CWnd* pImageWnd;

//	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
//	pImageWnd->GetWindowRect(&nImageRect);

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sRefTmp			= GetString();
		nUseMaskImage	= GetInteger();
		sMsg			= GetString();
	}

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	//================
	// Skip GrabNload
	//================
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag))
	{
		//================
		// Audio-NG Retry
		//================
		if((pCurStep->m_bAudioTestResult == FALSE) && (pCurStep->m_bVideoTestResult == TRUE))
		{

			CString szMsg;
			szMsg.Format("%d.GrabNLoad is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(szMsg);

			//+add kwmoon 080710
			// Load a Reference Image
			if(sRefTmp != _T(""))
			{
				if(!RefImage_DirectLoad(sRefTmp,nUseMaskImage)) return TEST_ABORT;
			}
			return TEST_PASS;
		}
	}
	//+add kwmoon 080807 : add <else statement>	
	else
	{
	//+chage kwmoon 080807	
//	if((!pCurStep->m_bStepRetryFlag) && (!pCurStep->m_bStepInternalRetryFlag)
//	&& (g_CurSetting.IsSameGrabRefImage(sRefTmp) == TRUE))

		//================
		// Same Ref Image
		//================
		if(g_CurSetting.IsSameGrabRefImage(sRefTmp) == TRUE)
		{
			//+add kwmoon 080710
			// Load a Reference Image
			if(sRefTmp != _T(""))
			{
				if(!RefImage_DirectLoad(sRefTmp,nUseMaskImage)) return TEST_ABORT;
			}
			return TEST_PASS;
		}
	}

	ShowSubStepMessage(sMsg, "Grab Image");

	// Intiailize ROI
//	POINT ptZeroPosition;
//	ptZeroPosition.x = 0;
//	ptZeroPosition.y = 0;


	//+del kwmoon 080618
//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	ctrlImgProg.SetPos(60);

	if(!Grab_Image_2in1(sRefTmp,nUseMaskImage, TRUE))
	{
		return TEST_ABORT;
	}

	ctrlImgProg.SetPos(80);

	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);
//	sGrabPath.Format("%s\\S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);
	
	pCurStep->m_strGrabPath = sGrabPath;
	pCurStep->m_strProcPath = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";

	if(CurrentSet->bSaveGrabImg)
	{
		g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
	}
	g_GrabImage.m_szFilePath = sGrabPath;

	ctrlImgProg.SetPos(100);

//	g_pView->InvalidateRect(nImageRect, FALSE);
//	g_pView->UpdateWindow();

	return TEST_PASS;
}


UINT Grab_N_Load_Image_MakeRef_2in1()
{
	//+add 090219(Modification No2)
	CString szLog = "B:Grab_N_Load_Image_MakeRef_2in1\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
	
	int nWidth				= 0;
	int nHeight				= 0;
	int nImageSize			= 0;
	int nErrorCode			= 0;
	int nImageBufferSize	= 0;
	int nRowWidth			= 0;
	int nUseMaskImage		= 0;	// 0 : Dont't Use, 1 : Use
	int nGrabResult			= GRAB_FAIL;

	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");
	CString sMsg					= _T("");
	CString szFileName				= _T("");
	CString szErrorMsg				= _T("");
	CString szIntermediateFileName	= _T("");
	CString szIntermediateFilePath	= _T("");
	CString szAvgFileName			= _T("");
	CString szAvgFilePath			= _T("");
	CString szMaskFileName			= _T("");
	CString szMaskFilePath			= _T("");

	CRect nImageRect;
	CWnd* pImageWnd;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sRefTmp			= GetString();
		nUseMaskImage	= GetInteger();
		sMsg			= GetString();
	}

	//==================
	// Blur Filter Size
	//==================
	int nXMask		= 3;
	int nYMask		= 3;
	int nYcXMask	= 5;
	int nYcYMask	= 5;
	int nMaskWidth  = 3;
	int nNoAvgFiltering = 5;

	DWORD dwTickCount	= GetTickCount();
	DWORD dwElapsedTime = 0;

	ShowSubStepMessage(sMsg, "Grab Image");

	//+del kwmoon 080818
	// Intiailize ROI
//	POINT ptZeroPosition;
//	ptZeroPosition.x = 0;
//	ptZeroPosition.y = 0;

	//+del kwmoon 080618
//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	//================
	// Skip GrabNLoad
	//================
	szFileName = CurrentSet->sRefFolder + "\\" + sRefTmp;

	//+add kwmoon 071211
	if((pCurStep->m_strRefPath.Compare(szFileName) == NULL) 
	&& (pCurStep->m_bMakeRefDone))
	{
		return TEST_PASS;	
	}

	//======================================
	// Clear Temporary Reference Image List
	//======================================
	g_pView->m_aRefIntFileArray.RemoveAll();

	//======================================================
	// Grab Multiple Reference Images to make Average-Image
	//======================================================
	for(int nLoop = 0; nLoop < CurrentSet->nNoFrameForRefImageCreation; nLoop++)
	{
		sMsg1.Format("Grab #%d image",nLoop+1);
		ShowSubStepMessage(sMsg1, "Grab Image");

		if(!Grab_Image_2in1(_T(""),0, TRUE))
		{
			return TEST_ABORT;
		}

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

		//=========================
		// Save Intermediate Image
		//=========================
		szIntermediateFileName.Format("%s_%d.bmp",sRefTmp.Left(sRefTmp.ReverseFind('.')),nLoop);
		szIntermediateFilePath = g_pView->m_szMakeRefPath + "\\" + szIntermediateFileName;

		g_GrabImage.SaveImage(szIntermediateFilePath, SNAP_IMAGE,ctrlImgProg);

		//===========================================
		// Add an intermediate file name to the List
		//===========================================
		g_pView->m_aRefIntFileArray.Add(szIntermediateFilePath);

		//======================================================
		// In case of HDMI, Skip an averaging process
		//======================================================
		if(pCurStep->GetCurrentSource().Left(4).Compare("HDMI") == NULL)
		{
			break;
		}
	} 

	//====================
	// Make Average Image
	//====================
	if(g_pView->m_aRefIntFileArray.GetSize() > 1)
	{
		if(g_ImageProc.MakeAvgImg(g_pView->m_aRefIntFileArray, g_pView->m_Image[I_AVG], ctrlImgProg) == TEST_ABORT)
		{
			AfxMessageBox("Failed to make average reference file!");
			return TEST_ABORT;			
		}

		//========================================
		// In case of RGB_TEST, Apply Blur Filter 
		//========================================
		if(pCurStep->m_nTestType == RGB_TEST) 
		{
			//+change kwmoon 080820
		//	g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
			g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
		}

		szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

		g_pView->m_Image[I_AVG].SaveImage(szAvgFileName, SNAP_IMAGE,ctrlImgProg);
	}
	//=============================
	// Save Single Reference Image
	//=============================
	else
	{
		g_pView->m_Image[I_AVG].LoadImage(szIntermediateFilePath,ctrlImgProg); 

		//========================================
		// In case of RGB_TEST, Apply Blur Filter 
		//========================================
		if(pCurStep->m_nTestType == RGB_TEST) 
		{
			//+change kwmoon 080820
		//	g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
			g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
		}

		szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

		g_pView->m_Image[I_AVG].SaveImage(szAvgFileName, SNAP_IMAGE,ctrlImgProg);
	}
	
	//============================
	// Delete Intermediate Images
	//============================
	CString szFilePath = _T("");

	int nNoImage = g_pView->m_aRefIntFileArray.GetSize();

	if(CurrentSet->bSaveIntermediateImage == FALSE)
	{
		for(int i=0; i<nNoImage; i++)
		{
			szFilePath = g_pView->m_aRefIntFileArray.GetAt(i);

			DeleteFile(szFilePath);
		}
	}

	//===================
	// Make a Mask Image
	//===================
	if(nUseMaskImage)
	{
		sMsg1.Format("Make Mask Image");
		ShowSubStepMessage(sMsg1, "Grab Image");
		
		//===================
		// Apply Blur Filter
		//===================
		if(pCurStep->m_nTestType == RGB_TEST)
		{
			//+change kwmoon 080818
/*
			if((pCurStep->GetCurrentSource().Compare("RF") == NULL)
			|| (pCurStep->GetCurrentSource().Left(2).Compare("AV") == NULL))
			{
				//+del kwmoon 080129
			//	for(int i=0; i<2; i++)
			//	{
			//		g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
			//	}
			}
			else if(pCurStep->GetCurrentSource().Left(2).Compare("YC") == NULL)
			{
				//+change kwmoon 080129
			//	for(int i=0; i<4; i++)
				for(int i=0; i<2; i++)
				{
					g_ImageProc.Average(nYcXMask, nYcYMask, g_pView->m_Image[I_AVG], TRUE);
				}
			}
			else
			{
				//+change kwmoon 080129
			//	for(int i=0; i<5; i++)
				for(int i=0; i<2; i++)
				{
					g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
				}
			}
*/
			//===================
			// Input Source : YC
			//===================
			if(pCurStep->GetCurrentSource().Left(2).Compare("YC") == NULL)
			{
				for(int i=0; i<2; i++)
				{
					//+change kwmoon 080820
				//	g_ImageProc.Average(nYcXMask, nYcYMask, g_pView->m_Image[I_AVG], TRUE);
					g_ImageProc.Average(nYcXMask, nYcYMask, g_pView->m_Image[I_AVG]);
				}
			}
			else
			{
				//================================
				// All Souces except YC,RF and AV
				//================================
				if((pCurStep->GetCurrentSource().Compare("RF") != NULL)
				&& (pCurStep->GetCurrentSource().Left(2).Compare("AV") != NULL))
				{
					for(int i=0; i<2; i++)
					{
						//+change kwmoon 080820
					//	g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
						g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
					}
				}
			}
		}

		//=====================================================
		// Analog Image : Skip putting white box at OSD region
		//=====================================================
		//+change kwmoon 080818
	//	if(pCurStep->m_strCurSrc.Left(4).Compare("VOUT") == NULL)
		if((pCurStep->GetCurrentSource().Left(4).Compare("VOUT")	== NULL)
		|| (pCurStep->GetCurrentSource().Left(6).Compare("AV-OUT")  == NULL)
		|| (pCurStep->GetCurrentSource().Left(7).Compare("MNT-OUT") == NULL)
		|| (pCurStep->GetCurrentSource().Left(6).Compare("TV-OUT")  == NULL))
		{
			if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,FALSE))
			{
				AfxMessageBox("Failed to make Mask image!"); 
				return TEST_ABORT;
			}
		}
		//==========================================
		// LVDS Image : Put white box at OSD region
		//==========================================
		else
		{
			if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,TRUE))
			{
				AfxMessageBox("Failed to make Mask image!");
				return TEST_ABORT;
			}
		}

		//=================
		// Save Mask Image
		//=================
		szMaskFilePath = g_pView->m_szMakeRefPath + "\\Mask\\";
		szMaskFileName = szMaskFilePath + sRefTmp;

		if(!FileExists(szMaskFilePath))
		{
			CreateFullPath(szMaskFilePath);
		}

		g_pView->m_Image[I_MASK].SaveImage(szMaskFileName, MASK_IMAGE,ctrlImgProg);

/*		//=========================
		// Repaint Reference Image
		//=========================
		int nCurrentPatternNo	= 0;
		int nFilterSize			= 0;
		int nNoFiltering		= 0;
		int nRepaint			= 0;
		CString szRoiShape		= _T("");
	
		BOOL bSkipRepaint = FALSE;

		if(GetCurrentPatternNo(&nCurrentPatternNo) == TRUE)
		{
			if(FindPatternInfo(nCurrentPatternNo,&nFilterSize,&nNoFiltering,&nRepaint,&szRoiShape) == TRUE)
			{
				if(nRepaint == 0)
				{
					bSkipRepaint = TRUE;
				}
			}
		}
		
		if(!bSkipRepaint)
		{
			//=====================================================================
			// In case of RGB_TEST, Repaint Reference Image except for RF and HDMI
			//=====================================================================
			if((pCurStep->m_nTestType == RGB_TEST)
			&& (pCurStep->GetCurrentSource().Left(4).Compare("HDMI") != NULL)
			//+change 090219(Modification No1)
		//	&& (pCurStep->GetCurrentSource().Left(2).Compare("RF")   != NULL))
			&& (pCurStep->GetCurrentSource().Left(2).Compare("RF")   != NULL)
			&& (pCurStep->GetCurrentSource().Left(3).Compare("ATV")  != NULL))
			{
				sMsg1.Format("Make Repainted Image");
				ShowSubStepMessage(sMsg1, "Grab Image");

				if(g_ImageProc.Repaint(g_pView->m_Image[I_AVG],g_pView->m_Image[I_MASK],g_pView->m_Image[I_PROC], ctrlImgProg) == TEST_ABORT)
				{
					AfxMessageBox("Failed to make repainted average reference file!"); 
					return TEST_ABORT;
				}
				g_pView->m_Image[I_PROC].m_bImageLoaded = TRUE;

				szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

				g_pView->m_Image[I_PROC].SaveImage(szAvgFileName, PROC_IMAGE,ctrlImgProg);
			}
		} */
	}

	g_pView->InvalidateRect(nImageRect, FALSE);
	g_pView->UpdateWindow();

	//+add 090219(Modification No2)
	szLog = "E:Grab_N_Load_Image_MakeRef_2in1\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TEST_PASS;			
}

BOOL _Grab_Image_Check()
{
	BOOL bResult = FALSE;

//	g_pView->SetGrabInfo(&g_GrabImage);

/*	if(g_pView->m_bContinuousLVDSGrabRunning)
	{
		g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
	}
*/
	if(g_pView->m_bMakeReferenceMode)
	{
		bResult = Grab_Image_Check_MakeRef_2in1();
	}
	else
	{
		bResult = Grab_Image_Check_Normal_2in1();
	}

/*	if((pCurStep->m_nStepType == PROCESS_FUNCTION) && (pCurStep->m_bTest))				
	{
		if(!g_pView->m_bContinuousLVDSGrabRunning)
		{
			g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
		}
	}
	
	if((pCurStep->m_nTestType == ADC_TEST) && (pCurStep->m_bTest))
	{
		if(!g_pView->m_bContinuousLVDSGrabRunning)
		{
			g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
		}
	}
*/
	return bResult;
}

//+add PSH 080412
BOOL _Grab_Image_OSD_Check()
{
	BOOL bResult = FALSE;

	if(g_pView->m_bMakeReferenceMode)
	{
		bResult = Grab_Image_Check_OSD_MakeRef_2in1();
	}
	else
	{
		bResult = Grab_Image_Check_OSD_2in1();
	}

	return bResult;
}


UINT Grab_Image_Check_Normal_2in1()
{
	int nMaxDelay		 = 0;
	int nUseMaskImage	 = 0;	// 0 : Dont't Use, 1 : Use
//	int nWidth			 = 0;
//	int nHeight			 = 0;
//	int nBitCount		 = 0;
//	int nRowWidth		 = 0;
//	int nImageSize		 = 0;
//	int nErrorCode		 = 0;
//	int nImageBufferSize = 0;
//	int nGrabResult		 = GRAB_FAIL;
//	int nRoiType		 = ROI_USER_DEFINE;
	int nRetry = 0;
	BOOL bRemocon = FALSE;
	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");
	CString sMsg3					= _T("");
	CString szFileName				= _T("");
	CString szErrorMsg				= _T("");

	DWORD dwElapsedTime				= 0;
	DWORD dwTickCount				= GetTickCount();

	int nResult = TEST_FAIL;
	int nTotalNoErrorPixel  = 0;
	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;
	float fMaxDifference  = 0.0;

	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sRefTmp			= GetString();
		nUseMaskImage	= GetInteger();
		nMaxDelay		= GetInteger();
		sMsg			= GetString();
	}

	//================
	// Skip GrabNload
	//================
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag))
	{
		//================
		// Audio-NG Retry
		//================
		if((pCurStep->m_bAudioTestResult == FALSE) && (pCurStep->m_bVideoTestResult == TRUE))
		{
			sMsg3.Format("%d.GrabImageCheck is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(sMsg3);

			if(sRefTmp != _T(""))
			{
				if(!RefImage_DirectLoad(sRefTmp,nUseMaskImage)) return TEST_ABORT;
			}
			return TEST_PASS;
		}
	}

	if((g_ImageProc.m_nRoiWidth == 0)
	&& (g_ImageProc.m_nRoiHeight == 0))
	{
	//	nRoiType = ROI_HALF_DN_SCREEN;
		Set_Grab_Image_Check_Roi(CurrentSet->nGrabCheckArea);
	}

	g_CurSetting.m_szGrabRefImageName = sRefTmp;
//	sGrabPath.Format("%s\\S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);
	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);

	pCurStep->m_strGrabPath  = sGrabPath;
	pCurStep->m_strProcPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";
	g_GrabImage.m_szFilePath = sGrabPath;

	if(sMsg != _T(""))
	{
		sMsg1 = sMsg;
		sMsg1.MakeUpper();
		if(sMsg1 == "REMOTE")
		{
			bRemocon = TRUE;
		}
	}

GRAB_START :

	sMsg1 = sMsg;
	sMsg2 = _T("");

	if(CurrentSet->bRunAbort)
	{
		g_pView->m_nNoCurrentRepeatExecution = 1; 
		return TEST_ABORT;
	}
	
	if(sMsg1 == _T(""))
	{
		sMsg1.Format("Wait %dms",nMaxDelay);
	}
	else
	{
		sMsg2.Format("%s (Wait %dms)",sMsg1,nMaxDelay);
		sMsg1 = sMsg2;
	}
	ShowSubStepMessage(sMsg1, "Grab Image");
	
//	nWidth				= 0;
//	nHeight				= 0;
//	nImageSize			= 0;
//	nErrorCode			= 0;
//	nImageBufferSize	= 0;
//	nRowWidth			= 0;
//	nGrabResult			= GRAB_FAIL;

	if(!Grab_Image_2in1(sRefTmp,nUseMaskImage, TRUE))
	{
		return TEST_ABORT;
	}
	
	ctrlImgProg.SetPos(80);

	ctrlImgProg.SetPos(100);


	if((pCurStep->GetFuncType() == MEAS_BOOL) || (pCurStep->m_nStepType == PROCESS_FUNCTION))
	{
		pCurStep->m_fLowLimit  = -30.0f;
		pCurStep->m_fHighLimit = 30.0f;
	}

	g_ImageProc.m_ptPositionShift.x = 0;
	g_ImageProc.m_ptPositionShift.y = 0;

	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);
	g_ImageProc.SetAvgMaskSize(0,0,0); 

	nResult = g_ImageProc.RGBTest(GRAB_CHECK_TEST,g_GrabImage,g_RefImage,g_MaskImage,
								  ptResultPosition,fMaxDifference,nTotalNoErrorPixel,nUseMaskImage,ctrlImgProg);

	g_ImageProc.SetAvgMaskSize(0,0,0); 


	if(nResult == TEST_ABORT)
	{
		if(CurrentSet->bSaveGrabImg)
		{
			g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
		}
		g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition);
		return TEST_ABORT;
	}

	dwElapsedTime = GetTickCount()-dwTickCount;
 
	if(nResult == TEST_FAIL)
	{
		if(CurrentSet->bRunAbort)
		{
			if(CurrentSet->bSaveGrabImg)
			{
				g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			}
			g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition);
			AvSwitchBoxCtrl.m_nNoRemoteRetry = 1;
			return TEST_ABORT;
		}
		if(dwElapsedTime < (DWORD)nMaxDelay)
		{
			goto GRAB_START;
		}
		else
		{
			if((bRemocon) && (nRetry < AvSwitchBoxCtrl.m_nNoRemoteRetry))
			{
				AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, AvSwitchBoxCtrl.m_sRemoteCustom, 
					                          AvSwitchBoxCtrl.m_sRemoteCode, AvSwitchBoxCtrl.m_nRemoteByte);
				sMsg.Format("Remote_Out : %s", AvSwitchBoxCtrl.m_sRemoteName);
				AddStringToRemoteComLog(sMsg);

				_Wait(AvSwitchBoxCtrl.m_nRemoteDelay);

				dwTickCount	= GetTickCount();
				nRetry++;
				goto GRAB_START;
			}
		}
	}
	
	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition);

	if(CurrentSet->bSaveGrabImg)
	{
		g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
	}

	AvSwitchBoxCtrl.m_nNoRemoteRetry = 1;
	return TEST_PASS;
}

//+add PSH 080412
UINT Grab_Image_Check_OSD_2in1()
{
	int nMaxDelay		 = 0;
	int nUseMaskImage	 = 0;	// 0 : Dont't Use, 1 : Use
	int nRetry = 0;
	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");

	BOOL bRemocon = FALSE;
	int nResult = TEST_FAIL;
	int nTotalNoErrorPixel  = 0;
	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;
	float fMaxDifference = 0.0;

	DWORD dwElapsedTime				= 0;
	DWORD dwTickCount				= 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sRefTmp			= GetString();
		nUseMaskImage	= GetInteger();
		nMaxDelay		= GetInteger();
		sMsg			= GetString();
	}


	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag))
	{
		if((pCurStep->m_bAudioTestResult == FALSE) && (pCurStep->m_bVideoTestResult == TRUE))
		{
			CString szMsg;
			szMsg.Format("%d.GrabImageCheck is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(szMsg);

			return TEST_PASS;
		}
	}

	if(sMsg != _T(""))
	{
		sMsg1 = sMsg;
		sMsg1.MakeUpper();
		if(sMsg1 == "REMOTE")
		{
			bRemocon = TRUE;
		}
	}

	dwTickCount				= GetTickCount();

GRAB_START :

	sMsg1 = sMsg;
	sMsg2 = _T("");

	if(CurrentSet->bRunAbort)
	{
		g_pView->m_nNoCurrentRepeatExecution = 1; 

		return TEST_ABORT;
	}
	
	if(sMsg1 == _T(""))
	{
		sMsg1.Format("Wait %dms",nMaxDelay);
	}
	else
	{
		sMsg2.Format("%s (Wait %dms)",sMsg1,nMaxDelay);
		sMsg1 = sMsg2;
	}
	
	ShowSubStepMessage(sMsg1, "Grab Image");
	

	//+change kwmoon 071207
	if(!Grab_Image_2in1(sRefTmp,nUseMaskImage, TRUE))
	{
		return TEST_ABORT;
	}

	g_CurSetting.m_szGrabRefImageName = sRefTmp;

	ctrlImgProg.SetPos(80);

	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);
//	sGrabPath.Format("%s\\S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);

	pCurStep->m_strGrabPath  = sGrabPath;
	pCurStep->m_strProcPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";
	g_GrabImage.m_szFilePath = sGrabPath;

//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
	
	ctrlImgProg.SetPos(100);

	if((pCurStep->GetFuncType() == MEAS_BOOL) || (pCurStep->m_nStepType == PROCESS_FUNCTION))
	{
		pCurStep->m_fLowLimit  = -30.0f;
		pCurStep->m_fHighLimit = 30.0f;
	}
	//+add kwmoon 080819
	g_ImageProc.m_ptPositionShift.x = 0;
	g_ImageProc.m_ptPositionShift.y = 0;

	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);

	g_ImageProc.SetAvgMaskSize(0,0,0); 

	nResult = g_ImageProc.RGBTest(OSD_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
								  ptResultPosition,fMaxDifference,nTotalNoErrorPixel,nUseMaskImage,ctrlImgProg);

	g_ImageProc.SetAvgMaskSize(0,0,0); 


	if(nResult == TEST_ABORT)
	{
		return TEST_ABORT;
	}

	dwElapsedTime = GetTickCount()-dwTickCount;
 
	if(nResult == TEST_FAIL)
	{
		if(CurrentSet->bRunAbort)
		{
			AvSwitchBoxCtrl.m_nNoRemoteRetry = 1;
			return TEST_ABORT;
		}

		if(dwElapsedTime < (DWORD)nMaxDelay)
		{
			goto GRAB_START;
		}
		else
		{
			if((bRemocon) && (nRetry < AvSwitchBoxCtrl.m_nNoRemoteRetry))
			{
				AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, AvSwitchBoxCtrl.m_sRemoteCustom, 
					                          AvSwitchBoxCtrl.m_sRemoteCode, AvSwitchBoxCtrl.m_nRemoteByte);
				sMsg.Format("Remote_Out : %s", AvSwitchBoxCtrl.m_sRemoteName);
				AddStringToRemoteComLog(sMsg);

				_Wait(AvSwitchBoxCtrl.m_nRemoteDelay);

				dwTickCount	= GetTickCount();
				nRetry++;
				goto GRAB_START;
			}
		}
	}

	AvSwitchBoxCtrl.m_nNoRemoteRetry = 1;
	
	return TEST_PASS;
}

BOOL Grab_Image_2in1(CString sRefImagePath, int nUseMaskImage, BOOL bMultiGrab)
{
	int nWidth		= 0;
	int nHeight		= 0;
	int nBitCount	= 0;
	int nRowWidth	= 0;
	int nImageSize	= 0;
	int nGrabResult = GRAB_FAIL;

	CString szErrorMsg	= _T("");
	CString sMsg		  = _T("");
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");



	nWidth		= AnalogControl.m_nWidth;
	nHeight		= AnalogControl.m_nHeight;
	nImageSize  = AnalogControl.m_nImageSize;

	g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

	nGrabResult = AnalogControl.OnGrabStart(GRAB_WITH_RETRY,sRefImagePath,nUseMaskImage, bMultiGrab);
	if(nGrabResult != GRAB_PASS)		
	{
		if(CurrentSet->nDisplayType == DETAILEDGRID)
		{
			if(nGrabResult == GRAB_FAIL)
			{
				szErrorMsg = "No Analog input image!";
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);
			}
		}
		g_GrabImage.m_bImageLoaded = TRUE;
	}


	nRowWidth = WIDTHBYTES(g_GrabImage.m_BmInfo->bmiHeader.biBitCount*g_GrabImage.m_BmInfo->bmiHeader.biWidth);

	if(nGrabResult == GRAB_PASS)
	{
		// Copy Image Data
		for(int i=0; i<nHeight; i++)
		{
			for(int j=0; j<nWidth; j++) 
			{   
				g_GrabImage.m_pImageData[i*nRowWidth+3*j+2] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j+2]);  // R
				g_GrabImage.m_pImageData[i*nRowWidth+3*j+1] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j+1]); // G
				g_GrabImage.m_pImageData[i*nRowWidth+3*j  ] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j  ]); // B
			}
		}
	}
	else
	{
		memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);

		if(nGrabResult == GRAB_ABORT) return FALSE;
	}
	g_GrabImage.m_bImageLoaded = TRUE;

	return TRUE;
}

UINT Grab_Image_Check_MakeRef_2in1()
{
	//+add 090219(Modification No2)
	CString szLog = "B:Grab_Image_Check_MakeRef_2in1\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int	nMaxDelay		 = 0;
	int nUseMaskImage	 = 0;	// 0 : Dont't Use, 1 : Use
	int nWidth			 = 0;
	int nHeight			 = 0;
	int nBitCount		 = 0;
	int nRowWidth		 = 0;
	int nImageSize		 = 0;
	int nErrorCode		 = 0;
	int nImageBufferSize = 0;
	int nGrabResult = GRAB_FAIL;

	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");
	CString szFileName				= _T("");
	CString szErrorMsg				= _T("");
	CString szIntermediateFileName	= _T("");
	CString szIntermediateFilePath	= _T("");
	CString szAvgFileName			= _T("");
	CString szMaskFileName			= _T("");
	CString szMaskFilePath			= _T("");

	CRect nImageRect;
	CWnd* pImageWnd	= NULL;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sRefTmp			= GetString();
		nUseMaskImage	= GetInteger();
		nMaxDelay		= GetInteger();
		sMsg1			= GetString();
	}

	//==================
	// Blur Filter Size
	//==================
	int nXMask		= 3;
	int nYMask		= 3;
	int nYcXMask	= 5;
	int nYcYMask	= 5;
	int nMaskWidth  = 3;
	int nNoAvgFiltering = 5;

	nMaxDelay = (int)((float)nMaxDelay * (float)(CurrentSet->nDelayPercentage * 0.01));

	DWORD dwTickCount	= GetTickCount();
	DWORD dwElapsedTime = 0;

	if(sMsg1 == _T(""))
	{
		sMsg1.Format("Wait %dms",nMaxDelay);
	}
	else
	{
		sMsg2.Format("%s (Wait %dms)",sMsg1,nMaxDelay);
		sMsg1 = sMsg2;
	}

	ShowSubStepMessage(sMsg1, "Grab Image");
	
	DWORD dwStartTick = 0, dwCurrentTick = 0;

	dwStartTick = GetTickCount();

	//======================
	// Grab Reference Image
	//======================
/*	while(TRUE)
	{
		if(CurrentSet->bRunAbort)
		{
			g_pView->m_nNoCurrentRepeatExecution = 1; 
			return TEST_ABORT;
		}

		if(!Grab_Image_2in1(_T(""),0, TRUE))
		{
			return TEST_ABORT;
		}

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMaxDelay) break;
	}	
*/
	Sleep(nMaxDelay);

	pCurStep->m_bMakeRefDone = FALSE;
	pCurStep->m_strRefPath	 =  CurrentSet->sRefFolder + "\\" +sRefTmp;
	
	//======================================
	// Clear Temporary Reference Image List
	//======================================
	g_pView->m_aRefIntFileArray.RemoveAll();

	//======================================================
	// Grab Multiple Reference Images to make Average-Image
	//======================================================
	for(int nLoop = 0; nLoop < CurrentSet->nNoFrameForRefImageCreation; nLoop++)
	{
		sMsg1.Format("Grab #%d image",nLoop+1);
		ShowSubStepMessage(sMsg1, "Grab Image");

		if(!Grab_Image_2in1(_T(""),0, TRUE))
		{
			return TEST_ABORT;
		}

//		g_pView->InvalidateRect(nImageRect, FALSE);
//		g_pView->UpdateWindow(); _Wait(10);

		//=========================
		// Save Intermediate Image
		//=========================
		szIntermediateFileName.Format("%s_%d.bmp",sRefTmp.Left(sRefTmp.ReverseFind('.')),nLoop);
		szIntermediateFilePath = g_pView->m_szMakeRefPath + "\\" + szIntermediateFileName;

		g_GrabImage.SaveImage(szIntermediateFilePath, SNAP_IMAGE,ctrlImgProg);

		//===========================================
		// Add an intermediate file name to the List
		//===========================================
		g_pView->m_aRefIntFileArray.Add(szIntermediateFilePath);

		//======================================================
		// In case of HDMI, Skip an averaging process
		//======================================================
		if(pCurStep->GetCurrentSource().Left(4).Compare("HDMI") == NULL)
		{
			break;
		}
	} 
	
	//====================
	// Make Average Image
	//====================
	if(g_pView->m_aRefIntFileArray.GetSize() > 1)
	{
		if(g_ImageProc.MakeAvgImg(g_pView->m_aRefIntFileArray, g_pView->m_Image[I_AVG], ctrlImgProg) == TEST_ABORT)
		{
			AfxMessageBox("Failed to make average reference file!"); 
			return TEST_ABORT;
		}

		//========================================
		// In case of RGB_TEST, Apply Blur Filter 
		//========================================
	/*	if(pCurStep->m_nTestType == RGB_TEST) 
		{
			g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
		}
	*/
		szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

		g_pView->m_Image[I_AVG].SaveImage(szAvgFileName, SNAP_IMAGE,ctrlImgProg);
	}
	//=============================
	// Save Single Reference Image
	//=============================
	else
	{
		g_pView->m_Image[I_AVG].LoadImage(szIntermediateFilePath,ctrlImgProg); 

		//========================================
		// In case of RGB_TEST, Apply Blur Filter 
		//========================================
	/*	if(pCurStep->m_nTestType == RGB_TEST) 
		{
			g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
		}
	*/
		szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

		g_pView->m_Image[I_AVG].SaveImage(szAvgFileName, SNAP_IMAGE,ctrlImgProg);
	}

	//============================
	// Delete Intermediate Images
	//============================
	CString szFilePath = _T("");

	int nNoImage = g_pView->m_aRefIntFileArray.GetSize();

	if(CurrentSet->bSaveIntermediateImage == FALSE)
	{
		for(int i=0; i<nNoImage; i++)
		{
			szFilePath = g_pView->m_aRefIntFileArray.GetAt(i);

			DeleteFile(szFilePath);
		}
	}
	
	//===================
	// Make a Mask Image
	//===================
	if(nUseMaskImage)
	{
		sMsg1.Format("Make Mask Image");
		ShowSubStepMessage(sMsg1, "Grab Image");

		//===================
		// Apply Blur Filter
		//===================
		if(pCurStep->m_nTestType == RGB_TEST)
		{
		//	for(int i=0; i<2; i++)
		//	{
				g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
		//	}
		}

		if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,TRUE))
		{
			AfxMessageBox("Failed to make Mask image!");
			return TEST_ABORT;
		}
		
		//=================
		// Save Mask Image
		//=================
		szMaskFilePath = g_pView->m_szMakeRefPath + "\\Mask\\";
		szMaskFileName = szMaskFilePath + sRefTmp;

		if(!FileExists(szMaskFilePath))
		{
			CreateFullPath(szMaskFilePath);
		}

		g_pView->m_Image[I_MASK].SaveImage(szMaskFileName, MASK_IMAGE,ctrlImgProg);

	}

	pCurStep->m_bMakeRefDone = TRUE;
	
	return TEST_PASS;
}

//+add PSH 080412
UINT Grab_Image_Check_OSD_MakeRef_2in1()
{
	//+add 090219(Modification No2)
	CString szLog = "B:Grab_Image_Check_OSD_MakeRef_2in1\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int	nMaxDelay		 = 0;
	int nUseMaskImage	 = 0;	// 0 : Dont't Use, 1 : Use
	int nWidth			 = 0;
	int nHeight			 = 0;
	int nBitCount		 = 0;
	int nRowWidth		 = 0;
	int nImageSize		 = 0;
	int nErrorCode		 = 0;
	int nImageBufferSize = 0;
	int nGrabResult = GRAB_FAIL;

	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");
	CString szFileName				= _T("");
	CString szErrorMsg				= _T("");
	CString szIntermediateFileName	= _T("");
	CString szIntermediateFilePath	= _T("");
	CString szAvgFileName			= _T("");
	CString szMaskFileName			= _T("");
	CString szMaskFilePath			= _T("");

	CRect nImageRect;
	CWnd* pImageWnd	= NULL;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sRefTmp			= GetString();
		nUseMaskImage	= GetInteger();
		nMaxDelay		= GetInteger();
		sMsg1			= GetString();
	}
	
	//+add kwmoon 080129
	int nXMask = 3;
	int nYMask = 3;

	//+add kwmoon 080124
	nMaxDelay = (int)((float)nMaxDelay * (float)(CurrentSet->nDelayPercentage * 0.01));

	DWORD dwTickCount	= GetTickCount();
	DWORD dwElapsedTime = 0;

	if(sMsg1 == _T(""))
	{
		sMsg1.Format("Wait %dms",nMaxDelay);
	}
	else
	{
		sMsg2.Format("%s (Wait %dms)",sMsg1,nMaxDelay);
		sMsg1 = sMsg2;
	}

	ShowSubStepMessage(sMsg1, "Grab Image");
	
	DWORD dwStartTick = 0, dwCurrentTick = 0;

	dwStartTick = GetTickCount();

	while(TRUE)
	{
		if(CurrentSet->bRunAbort)
		{
			g_pView->m_nNoCurrentRepeatExecution = 1; 
			
			return TEST_ABORT;
		}

		if(!Grab_Image_2in1(_T(""),0, TRUE))
		{
			return TEST_ABORT;
		}

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMaxDelay) break;
	}	

	//+add kwmoon 071211
	pCurStep->m_bMakeRefDone = FALSE;
	pCurStep->m_strRefPath	 =  CurrentSet->sRefFolder + "\\" +sRefTmp;
	
	// Empty a List
	g_pView->m_aRefIntFileArray.RemoveAll();

	for(int nLoop = 0; nLoop < CurrentSet->nNoFrameForRefImageCreation; nLoop++)
	{
		sMsg1.Format("Grab #%d image",nLoop+1);
		ShowSubStepMessage(sMsg1, "Grab Image");

		//+change kwmoon 071207
		if(!Grab_Image_2in1(_T(""),0, TRUE))
		{
			//+change kwmoon 080319
		//	return FALSE;
			return TEST_ABORT;
		}

		//g_pView->InvalidateRect(nImageRect, FALSE);
		//g_pView->UpdateWindow(); _Wait(10);

		// Save Image as an intermediate File
		szIntermediateFileName.Format("%s_%d.bmp",sRefTmp.Left(sRefTmp.ReverseFind('.')),nLoop);
		szIntermediateFilePath = g_pView->m_szMakeRefPath + "\\" + szIntermediateFileName;

		g_GrabImage.SaveImage(szIntermediateFilePath, SNAP_IMAGE,ctrlImgProg);

		// Add an intermediate file name to the List
		g_pView->m_aRefIntFileArray.Add(szIntermediateFilePath);

		// In case of HDMI, do not perform an averaging process.
		if(pCurStep->GetCurrentSource().Left(4).Compare("HDMI") == NULL)
		{
			//+del kwmoon 080129
		//	g_pView->m_Image[I_AVG].LoadImage(szIntermediateFilePath,ctrlImgProg); 
		//	szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;
		//	g_pView->m_Image[I_AVG].SaveImage(szAvgFileName, SNAP_IMAGE,ctrlImgProg); 
			
			break;
		}
	} 
	
	// Perform an averaging process
	if(g_pView->m_aRefIntFileArray.GetSize() > 1)
	{
		if(g_ImageProc.MakeAvgImg(g_pView->m_aRefIntFileArray, g_pView->m_Image[I_AVG], ctrlImgProg) == TEST_ABORT)
		{
			AfxMessageBox("Failed to make average reference file!"); 

			//+change kwmoon 080319
		//	return FALSE;
			return TEST_ABORT;
		}

		//+add kwmoon 080129
		if(pCurStep->m_nTestType == RGB_TEST) 
		{
			//+change kwmoon 080820
		//	g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
			g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
		}

		szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

		g_pView->m_Image[I_AVG].SaveImage(szAvgFileName, SNAP_IMAGE,ctrlImgProg);
	}
	else
	{
		g_pView->m_Image[I_AVG].LoadImage(szIntermediateFilePath,ctrlImgProg); 

		//+add kwmoon 080129
		if(pCurStep->m_nTestType == RGB_TEST) 
		{
			//+change kwmoon 080820
		//	g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
			g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
		}

		szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

		g_pView->m_Image[I_AVG].SaveImage(szAvgFileName, SNAP_IMAGE,ctrlImgProg);
	}

	//+add kwmoon 080124
	CString szFilePath = _T("");

	int nNoImage = g_pView->m_aRefIntFileArray.GetSize();

	if(CurrentSet->bSaveIntermediateImage == FALSE)
	{
		for(int i=0; i<nNoImage; i++)
		{
			szFilePath = g_pView->m_aRefIntFileArray.GetAt(i);

			DeleteFile(szFilePath);
		}
	}
	//-

	//+add kwmoon 071210
	int nYcXMask	= 5;
	int nYcYMask	= 5;

	//+del kwmoon 080129
//	int nXMask		= 3;
//	int nYMask		= 3;
	int nMaskWidth  = 3;

	int nNoAvgFiltering = 5;

	// Make a Mask Image
	if(nUseMaskImage)
	{
		sMsg1.Format("Make Mask Image");
		ShowSubStepMessage(sMsg1, "Grab Image");

		//+change kwmoon 070929
		if(pCurStep->m_nTestType == RGB_TEST)
		{
			if((pCurStep->GetCurrentSource().Compare("RF") == NULL)
			|| (pCurStep->GetCurrentSource().Left(2).Compare("AV") == NULL))
			{
				//+del kwmoon 080129
			//	for(int i=0; i<2; i++)
			//	{
			//		g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
			//	}
			}
			else if(pCurStep->GetCurrentSource().Left(2).Compare("YC") == NULL)
			{
				//+change kwmoon 080129
			//	for(int i=0; i<4; i++)
				for(int i=0; i<2; i++)
				{
					//+change kwmoon 080820
				//	g_ImageProc.Average(nYcXMask, nYcYMask, g_pView->m_Image[I_AVG], TRUE);
					g_ImageProc.Average(nYcXMask, nYcYMask, g_pView->m_Image[I_AVG]);
				}
			}
			else
			{
				//+change kwmoon 080129
			//	for(int i=0; i<5; i++)
				for(int i=0; i<2; i++)
				{
					//+change kwmoon 080820
				//	g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG], TRUE);
					g_ImageProc.Average(nXMask, nYMask, g_pView->m_Image[I_AVG]);
				}
			}
		}

		//+add kwmoon 080125
		if(pCurStep->m_strCurSrc.Left(4).Compare("VOUT") == NULL)
		{
			if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,FALSE))
			{
				AfxMessageBox("Failed to make Mask image!"); 
				//+change kwmoon 080319
			//	return FALSE;
				return TEST_ABORT;
			}
		}
		else
		{
			if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,TRUE))
			{
				AfxMessageBox("Failed to make Mask image!");
				//+change kwmoon 080319
			//	return FALSE;
				return TEST_ABORT;
			}
		}
		//-
		
		szMaskFilePath = g_pView->m_szMakeRefPath + "\\Mask\\";
		szMaskFileName = szMaskFilePath + sRefTmp;

		if(!FileExists(szMaskFilePath))
		{
			CreateFullPath(szMaskFilePath);
		}

		g_pView->m_Image[I_MASK].SaveImage(szMaskFileName, MASK_IMAGE,ctrlImgProg);

		//+add kwmoon 071215
		int nCurrentPatternNo = 0;
		int nFilterSize = 0;
		int nNoFiltering = 0;
		int nRepaint	 = 0;
		CString szRoiShape = _T("");
		
		BOOL bSkipRepaint = FALSE;

		//+add kwmoon 071215 : add if statement
		if(GetCurrentPatternNo(&nCurrentPatternNo) == TRUE)
		{
			if(FindPatternInfo(nCurrentPatternNo,&nFilterSize,&nNoFiltering,&nRepaint,&szRoiShape) == TRUE)
			{
				if(nRepaint == 0)
				{
					bSkipRepaint = TRUE;
				}
			}
		}
		
		// Repainting
		if(!bSkipRepaint)
		{
			if((pCurStep->m_nTestType == RGB_TEST)
			&& (pCurStep->GetCurrentSource().Left(4).Compare("HDMI") != NULL)
			//+change 090219(Modification No1)
		//	&& (pCurStep->GetCurrentSource().Left(2).Compare("RF")   != NULL))
			&& (pCurStep->GetCurrentSource().Left(2).Compare("RF")   != NULL)
			&& (pCurStep->GetCurrentSource().Left(3).Compare("ATV")   != NULL))
			{
				sMsg1.Format("Make Repainted Image");
				ShowSubStepMessage(sMsg1, "Grab Image");

				if(g_ImageProc.Repaint(g_pView->m_Image[I_AVG],g_pView->m_Image[I_MASK],g_pView->m_Image[I_PROC], ctrlImgProg) == TEST_ABORT)
				{
					AfxMessageBox("Failed to make repainted average reference file!"); 
					return TEST_ABORT;
				}
				g_pView->m_Image[I_PROC].m_bImageLoaded = TRUE;

				szAvgFileName = g_pView->m_szMakeRefPath + "\\" +sRefTmp;

				g_pView->m_Image[I_PROC].SaveImage(szAvgFileName, PROC_IMAGE,ctrlImgProg);
			}
		}
	}

	//+add kwmoon 071211
	pCurStep->m_bMakeRefDone = TRUE;
	
	g_pView->InvalidateRect(nImageRect, FALSE);
	g_pView->UpdateWindow();

	//+add 090219(Modification No2)
	szLog = "E:Grab_Image_Check_OSD_MakeRef_2in1\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TEST_PASS;
}


//+add kwmoon 080523
BOOL _SetAudioCheckMethod()
{
	CString sMsg = _T("");

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		CurrentSet->nAudioCheckMethod = GetInteger();
		sMsg						 = GetString();
	}
	else 
	{
		return FALSE;
	}

	ShowSubStepMessage(sMsg, "AudioChkMethod");

	return TRUE;
}

//+add kwmoon 080514
BOOL _CheckAudio()
{
	CString sCmd = _T("");
	CString sMsg = _T("");
	CString sMsg1 = _T("");

	int nAudioSource = 0;
	int nAudioCHchangeDelay	= 0;
	int nTemp  = 0;
	int nIndex;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	


	//+add PSH 080816
//	g_pView->m_bMeasureAudio = FALSE;
	g_pView->m_nMeasureWait = 0;
	if (pos!= NULL)
	{
		nAudioSource	= GetInteger();

		//+add kwmoon 080523
		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = FREQ_LEVEL_CHECK;
			AvSwitchBoxCtrl.m_fTargetLeftFreq	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightFreq	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetLeftLevel	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightLevel	= (float)GetInteger();

//			nTemp								= GetInteger();
//			AvSwitchBoxCtrl.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;
//			AvSwitchBoxCtrl.m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
			AvSwitchBoxCtrl.m_fFreqMargin		= (float)GetInteger();
			AvSwitchBoxCtrl.m_fLevelMargin		= (float)GetInteger();
		}
		else
		{
			//+add PSH 090305
			g_pView->m_bMeasureAudioType = FREQ_LEVEL_CHECK;
			g_pView->m_fTargetLeftFreq	= (float)GetInteger();
			g_pView->m_fTargetRightFreq	= (float)GetInteger();
			g_pView->m_fTargetLeftLevel	= (float)GetInteger();
			g_pView->m_fTargetRightLevel	= (float)GetInteger();

//			nTemp							= GetInteger();
//			g_pView->m_fFreqMargin		= (float)CurrentSet->nFreqMargin;			
//			g_pView->m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
			g_pView->m_fFreqMargin		= (float)(float)GetInteger();		
			g_pView->m_fLevelMargin		= (float)(float)GetInteger();
		}

		sMsg			= GetString();
		if(!sMsg.IsEmpty()){
			nIndex = sMsg.Find(',');
			if(nIndex == -1){
				g_pView->m_nMeasureWait = atoi(sMsg);
			}
			else{
				sMsg1 = sMsg.Left(nIndex);
				g_pView->m_nMeasureWait = atoi(sMsg1);
				sMsg.Delete(0, nIndex +1);
				sMsg.TrimLeft();
				sMsg.TrimRight();
				sMsg.MakeUpper();
				if(sMsg == "REMOTE")
				{
					pCurStep->m_bRemocon = TRUE;
				}
			}
		}
	}
	else 
	{
		return FALSE;
	}

	//+add kwmoon 080507
	if(g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}

	//+add kwmoon 070929
	if((pCurStep->m_bStepRetryFlag) || !g_CurSetting.IsSameAudioSource(nAudioSource))
	{
		SetAudioSource(nAudioSource);
		
		_Wait(1000);
	}


//	pCurStep->m_bRunAudioTest = TRUE;

	return TRUE;
}

//+add 090305
BOOL _CheckAudio_Level()
{
	CString sCmd = _T("");
	CString sMsg = _T("");

	int nAudioSource = 0;
	int nAudioCHchangeDelay	= 0;
	int nTemp  = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	

//	g_pView->m_bMeasureAudio = FALSE;
	g_pView->m_nMeasureWait = 0;

	if (pos!= NULL)
	{
		nAudioSource	= GetInteger();

		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = LEVEL_CHECK;
			AvSwitchBoxCtrl.m_fTargetLeftLevel	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightLevel	= (float)GetInteger();

			AvSwitchBoxCtrl.m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
		}
		else
		{
			//+add PSH 090305
			g_pView->m_bMeasureAudioType = LEVEL_CHECK;
			g_pView->m_fTargetLeftLevel	= (float)GetInteger();
			g_pView->m_fTargetRightLevel	= (float)GetInteger();

			g_pView->m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
		}
	}
	else 
	{
		return FALSE;
	}

	//+add kwmoon 080507
	if(g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}

	//+add kwmoon 070929
	if((pCurStep->m_bStepRetryFlag) || !g_CurSetting.IsSameAudioSource(nAudioSource))
	{
		SetAudioSource(nAudioSource);
		
		_Wait(1000);
	}


//	pCurStep->m_bRunAudioTest = TRUE;

	return TRUE;
}

//+add 090305
BOOL _CheckAudio_Frequency()
{
	CString sCmd = _T("");
	CString sMsg = _T("");

	int nAudioSource = 0;
	int nAudioCHchangeDelay	= 0;
	int nTemp  = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	

//	g_pView->m_bMeasureAudio = FALSE;
//	g_pView->m_nMeasureWait = 0;

	if (pos!= NULL)
	{
		nAudioSource	= GetInteger();

		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = FREQUENCY_CHECK;
			AvSwitchBoxCtrl.m_fTargetLeftFreq	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightFreq	= (float)GetInteger();

			AvSwitchBoxCtrl.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;
		}
		else
		{
			//+add PSH 090305
			g_pView->m_bMeasureAudioType = FREQUENCY_CHECK;
			g_pView->m_fTargetLeftFreq	= (float)GetInteger();
			g_pView->m_fTargetRightFreq	= (float)GetInteger();

			g_pView->m_fFreqMargin		= (float)CurrentSet->nFreqMargin;
		}
	}
	else 
	{
		return FALSE;
	}

	//+add kwmoon 080507
	if(g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}

	//+add kwmoon 070929
	if((pCurStep->m_bStepRetryFlag) || !g_CurSetting.IsSameAudioSource(nAudioSource))
	{
		SetAudioSource(nAudioSource);
		
		_Wait(1000);
	}


//	pCurStep->m_bRunAudioTest = TRUE;

	return TRUE;
}



// Global Function

BOOL RefImage_DirectLoad(CString szFileName,int nUseMaskImage)
{
	//+chage kwmoon 080402
	CString sTmp = _T("");

	// Load Ref Image
	m_nSelectedImage = LOAD_IMAGE;

	ctrlImgProg.SetPos(0);
	sTmp = CurrentSet->sRefFolder + "\\" + szFileName;
	pCurStep->m_strRefPath = sTmp;

	if(!g_RefImage.LoadImage(sTmp, ctrlImgProg)) return FALSE;

	// Load Mask Image
	g_MaskImage.m_bImageLoaded = FALSE;

	if(nUseMaskImage)
	{
		ctrlImgProg.SetPos(0);

		sTmp = CurrentSet->sMaskFolder + "\\" + szFileName;
	
		pCurStep->m_strMaskPath = sTmp;

		if(!g_MaskImage.LoadImage(sTmp, ctrlImgProg)) return FALSE;
	}
	return TRUE;
}


BOOL _AuthenticationKey()
{
	CString sMsg			= _T("");
	CString sErrorMsg		= _T("");
	
	int nAuthenticationKey	= 0;
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nAuthenticationKey	= GetInteger();
		sMsg				= GetString();
	}
	else 
	{
		return FALSE;
	}

	ShowSubStepMessage(sMsg, "Check Authentication Key");
/*
	|| (GetModifyTime(CurrentSet->sSeqPath) != CurrentSet->lTime))
    {
        int nChoice = AfxMessageBox(IDS_QUERY_COMPILE, MB_YESNO);
        
		switch(nChoice)
        {
            case IDYES :	
			{
				CurrentSet->bCompiled = Prescan(CurrentSet->sSeqPath);
               
				if(CurrentSet->bCompiled == FALSE) goto END_EXIT;
       		}
			break;
		}
    }

*/

	return TRUE;
}

BOOL _SkippedSignalLineLowBits()
{

	int nNoSkippedSignalLineLowBits = 0;

	CString sMsg = _T("");
	CString sDefaultMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nNoSkippedSignalLineLowBits	 = GetInteger();

		sMsg = GetString();
	}
	else return FALSE;

	sDefaultMsg.Format("Low %d bits is excluded in Signal Line Test",nNoSkippedSignalLineLowBits);

	ShowSubStepMessage(sMsg,sDefaultMsg);

	g_ImageProc.m_LineBitTestData.SetNoSkippedSignalLineLowBits(nNoSkippedSignalLineLowBits);

	return TRUE;
}

/////////////////////
// Global Function //
/////////////////////

BOOL SetAudioSource(int nAudioSource)
{
	CString sMsg = _T("");

	int nAvSwitchBoxAudioChannelNo = 0;

	ShowSubStepMessage(sMsg, "Set Audio Source");

	if(AvSwitchBoxCtrl.m_bPortOpen == TRUE)
	{
		//+2007.12.26 Mod BY USY
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY, nAudioSource, 0))
		{
			return FALSE;
		}
//			AvSwitchBoxCtrl.SendRun2(SET_AUDIO_CH,nAudioSource,0);
		//-
	}
	else
	{
	/*	if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
		{
			AvSwitchBoxCtrl.PortClose();

			if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY, nAudioSource, 0))
		{ */
			return FALSE;
	//	}
	}

	return TRUE;
}



BOOL _SetManualTestMessage()
{
	UINT nDelay = 0;

	CString sName  = _T("");
	CString szMsg1 = _T("");
	CString szMsg2 = _T("");

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		szMsg1  = GetString();
		szMsg2  = GetString();
	}

	sName.Format("(%d) %s",pCurStep->m_nStep,pCurStep->m_sName);

	g_pView->m_ManualTestDlg.SetInfoMessage(sName,szMsg1,szMsg2);

	return TRUE;
}

//+add kwmoon 071112
BOOL _SetCaptionSize()
{
	CString sMsg  = _T("");

	int nWidth	= 0;
	int nHeight = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nWidth	= GetInteger();
		nHeight	= GetInteger();
		sMsg    = GetString();
	}

	ShowSubStepMessage(sMsg, "Set Caption Size");

	g_ImageProc.SetCaptionSize(nWidth,nHeight);

	return TRUE;
}

//+change kwmoon 080110
//+add kwmoon 071224
//BOOL _SetCaptionKeyDelay()
BOOL _SetCaptionKey()
{
	CString sMsg  = _T("");

	int nCaptionKeyDelay		= 0;
	int nCaptionEnterKeyDelay   = 0;

	//+change kwmoon 080124
	int nNoOnKeyPress			= 0;
	int nNoOffKeyPress			= 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nCaptionKeyDelay		= GetInteger();
		nCaptionEnterKeyDelay	= GetInteger();

		//+change kwmoon 080124
		nNoOnKeyPress			= GetInteger();
		nNoOffKeyPress			= GetInteger();
		sMsg					= GetString();
	}

	ShowSubStepMessage(sMsg, "Set Caption Key Delay");

	//+change kwmoon 080124
	g_ImageProc.SetCaptionKey(nCaptionKeyDelay,nCaptionEnterKeyDelay,nNoOnKeyPress,nNoOffKeyPress);

	return TRUE;
}

//+add kwmoon 071224
BOOL _SetCaptionCheckTime()
{
	CString sMsg  = _T("");

	int nCaptionCheckTime = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nCaptionCheckTime		= GetInteger();
		sMsg					= GetString();
	}

	ShowSubStepMessage(sMsg, "Set Caption Check Time");

	g_ImageProc.SetCaptionCheckTime(nCaptionCheckTime);

	return TRUE;
}


//+add kwmoon 071127
BOOL _SetDigitalEyeDelay()
{
	CString sMsg  = _T("");

	int nDelay	= 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nDelay	= GetInteger();
		sMsg    = GetString();
	}

	ShowSubStepMessage(sMsg, "Set Digital Eye Delay");

	g_ImageProc.SetDigitalEyeDelay(nDelay);

	return TRUE;
}

//+add kwmoon 071206
BOOL _NotSkipSameSourceChannel()
{
	CString sMsg  = _T("");

	int nNotSkipSameSourceChannel	= 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nNotSkipSameSourceChannel	= GetInteger();
		sMsg						= GetString();
	}

	if(nNotSkipSameSourceChannel)
	{
		ShowSubStepMessage(sMsg, "Not Skip Same Source/Channel");
	}
	else
	{
		ShowSubStepMessage(sMsg, "Skip Same Source/Channel");
	}

	g_CurSetting.m_nNotSkipSameSourceChannel = nNotSkipSameSourceChannel;

	return TRUE;
}

//+add kwmoon 071215
BOOL GetCurrentPatternNo(int* pPatternNo)
{

/*	if(g_CurSetting.m_nPatternGenNo > 0)
	{
		char szCurrentPatternNo[10];
		memset(szCurrentPatternNo,NULL,10);

		itoa(g_CurSetting.m_nPatternGenNo,szCurrentPatternNo,10);
		
		int nLength = strlen(szCurrentPatternNo);
		
		*pPatternNo = atoi(&szCurrentPatternNo[nLength-2]);

		return TRUE;
	} */

	if(PatternGeneratorCtrl.m_nPatternNo > 0)
	{
		
		*pPatternNo = PatternGeneratorCtrl.m_nPatternNo;

		return TRUE;
	}

	return FALSE;
}


//+add kwmoon 080110
BOOL _Power_On_Check()
{
	CString sMsg  = _T("");

	int nMaxDelay	= 0;

	CRect nImageRect;
	CWnd* pImageWnd;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	//+add kwmoon 080819
//	g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0); _Wait(500);

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nMaxDelay	= GetInteger();
		sMsg		= GetString();
	}

	DWORD dwStartTick = 0, dwCurrentTick = 0;

	dwStartTick = GetTickCount();

	while(TRUE)
	{
		if(CurrentSet->bRunAbort)
		{
			g_pView->m_nNoCurrentRepeatExecution = 1; return FALSE;
		}

		if(Grab_Image_2in1(_T(""),0, TRUE))
		{
			_Wait(1000);
			//+change kwmoon 080710
		//	return TRUE;
			break;
		}

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMaxDelay) break;
	}	

	//+add kwmoon 080819
//	g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);


	return TRUE;
}

//+add kwmoon 080110
BOOL _Power_Off_Check()
{
	CString sMsg  = _T("");

	int nMaxDelay	= 0;

	CRect nImageRect;
	CWnd* pImageWnd;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	//+add kwmoon 080819
//	g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0); _Wait(500);

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nMaxDelay	= GetInteger();
		sMsg		= GetString();
	}

	DWORD dwStartTick = 0, dwCurrentTick = 0;

	dwStartTick = GetTickCount();

	while(TRUE)
	{
		if(CurrentSet->bRunAbort)
		{
			g_pView->m_nNoCurrentRepeatExecution = 1; return FALSE;
		}

		if(Grab_Image_2in1(_T(""),0, TRUE) == TEST_ABORT) break;

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMaxDelay) break;
	}	

	//+add kwmoon 080819
//	g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);

	return TRUE;
}

//+add kwmoon 080121
BOOL _SetOsdRegion()
{
	POINT ptStartPosition;
	ptStartPosition.x = 0;
	ptStartPosition.y = 0;
	
	POINT ptEndPosition;
	ptEndPosition.x = 0;
	ptEndPosition.y = 0;

	int nWidth  = 0;
	int nHeight = 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		ptStartPosition.x = GetInteger();
		ptStartPosition.y = GetInteger();
		ptEndPosition.x   = GetInteger();
		ptEndPosition.y   = GetInteger(); 
		
		sMsg = GetString();
	}
	else return FALSE;

	ShowSubStepMessage(sMsg, "Set Osd Region");

	g_ImageProc.SetOsdRegion(ptStartPosition,ptEndPosition);

	return TRUE;
}

//+add kwmoon 080826
BOOL _SetInvalidRegion()
{
	POINT ptStartPosition;
	ptStartPosition.x = 0;
	ptStartPosition.y = 0;
	
	POINT ptEndPosition;
	ptEndPosition.x = 0;
	ptEndPosition.y = 0;

	int nWidth  = 0;
	int nHeight = 0;

	CString sMsg = _T("");
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		ptStartPosition.x = GetInteger();
		ptStartPosition.y = GetInteger();
		ptEndPosition.x   = GetInteger();
		ptEndPosition.y   = GetInteger(); 
		
		sMsg = GetString();
	}
	else return FALSE;

	ShowSubStepMessage(sMsg, "Set Invalid Region");

	g_ImageProc.SetInvalidRegion(ptStartPosition,ptEndPosition);

	g_ImageProc.m_bInvalidateInvalidRegion = TRUE;

	return TRUE;
}

BOOL _PatternGenerator_HSync_On() 
{
	BOOL bReturn;
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	szCmdString.Format("%c%c0%c",STX,HSYNC_ONOFF,ETX);

	bReturn = PatternGeneratorCtrl.SendCommString(szCmdString);  

	return bReturn;
}

BOOL _PatternGenerator_VSync_On() 
{
	BOOL bReturn;
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	szCmdString.Format("%c%c0%c",STX,VSYNC_ONOFF,ETX);
//	szCmdString.Format("%c",VSYNC_ONOFF);

	bReturn = PatternGeneratorCtrl.SendCommString(szCmdString);  

	return bReturn;
}

BOOL _PatternGenerator_RGB_On() 
{
	BOOL bReturn;
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	szCmdString.Format("%c%c0%c",STX,VIDEO_COLOR_CHANGE,ETX);
//	szCmdString.Format("%c",VSYNC_ONOFF);

	bReturn = PatternGeneratorCtrl.SendCommString(szCmdString);  

	return bReturn;
}

//+add PSH 080516

BOOL _PatternGenerator_HSync_Off() 
{
	BOOL bReturn;
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	szCmdString.Format("%c%c1%c",STX,HSYNC_ONOFF,ETX);

	bReturn = PatternGeneratorCtrl.SendCommString(szCmdString);  

	return bReturn;
}

BOOL _PatternGenerator_VSync_Off() 
{
	BOOL bReturn;
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	szCmdString.Format("%c%c1%c",STX,VSYNC_ONOFF,ETX);
//	szCmdString.Format("%c",VSYNC_ONOFF);

	bReturn = PatternGeneratorCtrl.SendCommString(szCmdString);  

	return bReturn;
}

BOOL _PatternGenerator_RGB_Off() 
{
	BOOL bReturn;
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	szCmdString.Format("%c%c7%c",STX,VIDEO_COLOR_CHANGE,ETX);
//	szCmdString.Format("%c",VSYNC_ONOFF);

	bReturn = PatternGeneratorCtrl.SendCommString(szCmdString);  

	return bReturn;
}




//+add kwmoon 080621
void Set_Grab_Image_Check_Roi(int nType)
{
	POINT ptRoi1Position;
	POINT ptRoi2Position;

	int nImageWidth;
	int nImageHeight;

	nImageWidth = CurrentSet->nAnalogWidth;
	nImageHeight     = CurrentSet->nAnalogHeight;

	CString szMsg;

	switch(nType)
	{
		case ROI_FULL_SCREEN :
		{
			ptRoi1Position.x = 0;
			ptRoi1Position.y = 0;
		
//			ptRoi2Position.x = (int)(g_RefImage.m_InfoHeader.biWidth)-1;
//			ptRoi2Position.y = (int)(g_RefImage.m_InfoHeader.biHeight)-1;
			ptRoi2Position.x = nImageWidth - 1;
			ptRoi2Position.y = nImageHeight -1;

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;

		case ROI_HALF_DN_SCREEN :
		{
			ptRoi1Position.x = 0;
//			ptRoi1Position.y = (int)(g_RefImage.m_InfoHeader.biHeight/2);
			ptRoi1Position.y = (int)(nImageHeight/2);
		
//			ptRoi2Position.x = (int)(g_RefImage.m_InfoHeader.biWidth)-1;
//			ptRoi2Position.y = (int)(g_RefImage.m_InfoHeader.biHeight)-1;
			ptRoi2Position.x = nImageWidth - 1;
			ptRoi2Position.y = nImageHeight - 1;

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;

		case ROI_HALF_UP_SCREEN :
		{
			ptRoi1Position.x = 0;
			ptRoi1Position.y = 0;
		
//			ptRoi2Position.x = (int)(g_RefImage.m_InfoHeader.biWidth)-1;
//			ptRoi2Position.y = (int)(g_RefImage.m_InfoHeader.biHeight/2);
			ptRoi2Position.x = nImageWidth - 1;
			ptRoi2Position.y = (int)(nImageHeight/2);

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;

		case ROI_LEFT_UP_SCREEN :
		{
			ptRoi1Position.x = 0;
			ptRoi1Position.y = 0;
		
			ptRoi2Position.x = (int)(nImageWidth/4);
			ptRoi2Position.y = (int)(nImageHeight/4);

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;

		case ROI_LEFT_DN_SCREEN :
		{
			ptRoi1Position.x = 0;
			ptRoi1Position.y = (int)(nImageHeight*3/4);
		
			ptRoi2Position.x = (int)(nImageWidth/4);
			ptRoi2Position.y =  nImageHeight - 1;

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;

		case ROI_RIGHT_UP_SCREEN :
		{
			ptRoi1Position.x = (int)(nImageWidth*3/4);
			ptRoi1Position.y = 0;
		
			ptRoi2Position.x = nImageWidth - 1;
			ptRoi2Position.y = (int)(nImageHeight/4);

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;

		case ROI_RIGHT_DN_SCREEN :
		{
			ptRoi1Position.x = (int)(nImageWidth*3/4);
			ptRoi1Position.y = (int)(nImageHeight*3/4);
		
			ptRoi2Position.x = nImageWidth - 1;
			ptRoi2Position.y = nImageHeight - 1;

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;

		case ROI_CENTER_SCREEN :
		{
			ptRoi1Position.x = (int)(nImageWidth/3);
			ptRoi1Position.y = (int)(nImageHeight/3);
		
			ptRoi2Position.x = (int)(nImageWidth*2/3);
			ptRoi2Position.y = (int)(nImageHeight*2/3);

			g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position); 
		}
		break;
	}
}

//+add kwmoon 080724
BOOL _MeasureAudio()
{
	CString sCmd = _T("");
	CString sMsg = _T("");

	int nAudioSource = 0;
	int nAudioCHchangeDelay	= 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	
	int nInputSource = 0;
	BOOL bFindInputSource = FALSE;

	CString szInputSource = pCurStep->GetCurrentSource();

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined[i].pszName) == NULL)
		{
			bFindInputSource = TRUE; nInputSource = i;	
		}
	}

	if(!bFindInputSource)
	{
		sMsg.Format("Invalid Input Source : %s",szInputSource);
		AfxMessageBox(sMsg); return FALSE;
	}

	//+add PSH 080816
//	g_pView->m_bMeasureAudio = TRUE;

	if (pos!= NULL)
	{
		nAudioSource = GetInteger();

		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			AvSwitchBoxCtrl.m_fTargetLeftFreq	= (float)CurrentSet->nAudioSpec[nInputSource][0];
			AvSwitchBoxCtrl.m_fTargetRightFreq	= (float)CurrentSet->nAudioSpec[nInputSource][1];
			AvSwitchBoxCtrl.m_fFreqMargin		= (float)CurrentSet->nAudioSpec[nInputSource][2];
			AvSwitchBoxCtrl.m_fTargetLeftLevel	= (float)CurrentSet->nAudioSpec[nInputSource][3];
			AvSwitchBoxCtrl.m_fTargetRightLevel	= (float)CurrentSet->nAudioSpec[nInputSource][4];

			//+add kwmoon 080724
			AvSwitchBoxCtrl.m_fLevelMargin		= 0;
		}
		else
		{
			g_pView->m_fTargetLeftFreq	= (float)CurrentSet->nAudioSpec[nInputSource][0];
			g_pView->m_fTargetRightFreq	= (float)CurrentSet->nAudioSpec[nInputSource][1];
			g_pView->m_fFreqMargin		= (float)CurrentSet->nAudioSpec[nInputSource][2];
			g_pView->m_fTargetLeftLevel	= (float)CurrentSet->nAudioSpec[nInputSource][3];
			g_pView->m_fTargetRightLevel	= (float)CurrentSet->nAudioSpec[nInputSource][4];

			//+add kwmoon 080724
			g_pView->m_fLevelMargin		= 0;
		}
	}
	else 
	{
		return FALSE;
	}

	if(g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}

	if((pCurStep->m_bStepRetryFlag) || !g_CurSetting.IsSameAudioSource(nAudioSource))
	{
		SetAudioSource(nAudioSource); _Wait(1000);
	}


	ShowSubStepMessage(sMsg, "Measure Audio");

//	pCurStep->m_bRunAudioTest = TRUE;

	if(pCurStep->m_nTestType != AUDIO_TEST)
	{
		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			AvSwitchBoxCtrl.RunAudioMeasure();
		}
		else
		{
			g_pView->m_RunAudioMeasure = 1;////RunAudioMeasure();
		}
	}

	return TRUE;
}

BOOL _TV_Mic_On()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_Mic_OnOff(TRUE, 3000);

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}

BOOL _TV_Mic_Off()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_Mic_OnOff(FALSE, 3000);

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}

//+add PSH 090129
BOOL _HDMIGen_SetEDID()
{
	BOOL bReturn;
	int nFlag;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
		
	if(!HDMIGeneratorCtrl.m_bPortOpen)
	{
		if(HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate) == FALSE)
		{
			HDMIGeneratorCtrl.PortClose();
			HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nFlag = GetInteger();
	}	
	else 
	{
		return FALSE;
	}

	if(nFlag == 0){
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;
	}

	return bReturn;
}

BOOL _HDMIGen_SetHDCP()
{
	BOOL bReturn;
	int nFlag;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
		
	if(!HDMIGeneratorCtrl.m_bPortOpen)
	{
		if(HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate) == FALSE)
		{
			HDMIGeneratorCtrl.PortClose();
			HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nFlag = GetInteger();
	}	
	else 
	{
		return FALSE;
	}

	if(nFlag == 0){
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE) ;
	}

	return bReturn;
}
BOOL _HDMIGen_SetDDCLine()
{
	BOOL bReturn;
	int nFlag;
	BOOL bOldDDCLine_Status;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
		
	if(!HDMIGeneratorCtrl.m_bPortOpen)
	{
		if(HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate) == FALSE)
		{
			HDMIGeneratorCtrl.PortClose();
			HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nFlag = GetInteger();
	}	
	else 
	{
		return FALSE;
	}

	bOldDDCLine_Status = HDMIGeneratorCtrl.m_bDDCLineMode;
	
	if(nFlag == 0){
		bReturn = HDMIGeneratorCtrl.SetDDCLine_OpenClose(TRUE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetDDCLine_OpenClose(FALSE) ;
	}

	if(!bOldDDCLine_Status && nFlag && bReturn)
	{
		_Wait(500);
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE);
		_Wait(500);
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE);
	}
	return bReturn;
}

BOOL _HDMIGen_SetOut()
{
	BOOL bReturn;
	int nModel;
	int nPattern;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
		
	if(!HDMIGeneratorCtrl.m_bPortOpen)
	{
		if(HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate) == FALSE)
		{
			HDMIGeneratorCtrl.PortClose();
			HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nModel = GetInteger();
		nPattern = GetInteger();
	}
	else 
	{
		return FALSE;
	}

	if(pCurStep->m_bStepRetryFlag || pCurStep->m_bStepInternalRetryFlag)
	{
		return TRUE;
	}
	else
	{
		if((HDMIGeneratorCtrl.m_nModelNo == nModel) && (HDMIGeneratorCtrl.m_nPatternNo != nPattern))
		{
			bReturn = HDMIGeneratorCtrl.SetPattern(nPattern); 
		}
		else if((HDMIGeneratorCtrl.m_nModelNo != nModel) && (HDMIGeneratorCtrl.m_nPatternNo == nPattern))
		{
			bReturn = HDMIGeneratorCtrl.SetModel(nModel); 
		}
		else
		{
			bReturn = HDMIGeneratorCtrl.SetModel_Pattern(nModel, nPattern); 
		}
	}
	return bReturn;
}


BOOL _SetRemoconOutRetry()
{
	int  nLoop;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nLoop = GetInteger();

		AvSwitchBoxCtrl.m_nNoRemoteRetry = nLoop; 
	}
	else
		return FALSE;

	return TRUE;
}


BOOL _Remocon_Out()
{
	//+change kwmoon 080402
	CString sCmd = _T("");
	CString sName = _T("");
	CString sCustom = _T("");
	
	int nClickNum = 0;
	int nDelay	  = 0;
	int nByte = 1;

	CString sMsg = _T("");

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sName	  = GetString();
		nClickNum = GetInteger();
		nDelay    = GetInteger();
	}
	else 
	{
		return FALSE;
	}

	if(!FindRemoconCode(sName, sCustom, sCmd, nByte))
	{
		sMsg.Format("Remocon Code Not Found: %s", sName);
		AddStringToRemoteComLog(sMsg);
		AfxMessageBox(sMsg);
		//return FALSE;
	}

	//+add kwmoon 080318
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag))
	{
		//================
		// Audio-NG Retry
		//================
		if((pCurStep->m_bRemocon) && (pCurStep->m_bRunAudioTest) &&(pCurStep->m_bAudioTestResult == FALSE))
		{	goto REMOCON_OUT_START;}

		sMsg.Format("%d.Remote_Out(%s) is skipped", pCurStep->m_nStep, sName);
		AddStringToRemoteComLog(sMsg);
		return TRUE;		
	}


REMOCON_OUT_START :

	ShowSubStepMessage(sMsg, "Remocon Out");
	//+ 2007.12.4 Add BY USY
	if(CurrentSet->bUseAVSwitchBox)
	{
		if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
		{
			if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
			{
				AvSwitchBoxCtrl.PortClose();
				if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate) == FALSE)
				{
					//+add kwmoon 080313
					CString szErrMsg;
					szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
					AfxMessageBox(szErrMsg);
					return FALSE;
				}
			}
		}
		for(int nTmp = 1; nTmp <= nClickNum; nTmp++)
		{
			AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte);
			sMsg.Format("Remote_Out : %s", sName);
			AddStringToRemoteComLog(sMsg);
			_Wait(nDelay);
		}

		AvSwitchBoxCtrl.m_sRemoteName = sName;
		AvSwitchBoxCtrl.m_sRemoteCustom = sCustom;
		AvSwitchBoxCtrl.m_sRemoteCode = sCmd;
		AvSwitchBoxCtrl.m_nRemoteDelay = nDelay;
		AvSwitchBoxCtrl.m_nRemoteByte = nByte;
	}
	else
	{
		return FALSE;
	}


	return TRUE;
}

BOOL _AudioLeft_Out()
{
	int nFlag ;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nFlag = GetInteger();
	}
	else 
	{
		return FALSE;
	}
	//if(nFlag == 0){
	//	if(g_pView->wOutL_Flag) g_pView->WaveOutL_Stop();
	//}
	//else{
	//	if(!g_pView->wOutL_Flag) g_pView->WaveOutL_Start();		
	//}

	return TRUE;
}
BOOL _AudioRight_Out()
{
	int nFlag ;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nFlag = GetInteger();
	}
	else 
	{
		return FALSE;
	}
	//if(nFlag == 0){
	//	if(g_pView->wOutR_Flag) g_pView->WaveOutR_Stop();
	//}
	//else{
	//	if(!g_pView->wOutR_Flag) g_pView->WaveOutR_Start();		
	//}

	return TRUE;
}

BOOL _SetRelay_OnOff()
{
	int nRelayNo;
	int nFlag ;
	BOOL bRet;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nRelayNo = GetInteger();
		nFlag = GetInteger();
	}
	else 
	{
		return FALSE;
	}
	if(gPciDioCtrl.m_bPCIDIO){
		if((nRelayNo < 1) || (nRelayNo > 3)){
			return FALSE;
		}
		if(nFlag == 0){
			gPciDioCtrl.Set_Relay(nRelayNo, FALSE);
		}
		else{
			gPciDioCtrl.Set_Relay(nRelayNo, TRUE);
		}
	}
	else{
		if((nRelayNo < 1) || (nRelayNo > 4)){
			return FALSE;
		}
		if(nFlag == 0){
			bRet = gUsbDioCtrl.Set_Relay(nRelayNo, FALSE);
		}
		else{
			bRet = gUsbDioCtrl.Set_Relay(nRelayNo, TRUE);
		}
	}

	return bRet;
}

BOOL _SetMovingPicTestMargin()
{
	CString szErrMsg;
	int nDiff_LoLimit;
	int nDiff_HiLimit;
	int nTest_Limit;

	//add 090520
	if(g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nDiff_LoLimit = GetInteger();
		nDiff_HiLimit = GetInteger();
		nTest_Limit   = GetInteger();
	}
	else return FALSE;

	if((nDiff_LoLimit < -250) || (nDiff_LoLimit > 250))
	{
		szErrMsg.Format("MovingPicTestMargin must be in the range of -250 to 250");
		AfxMessageBox(szErrMsg);
		return FALSE;
	}

	if((nDiff_HiLimit < -250) || (nDiff_HiLimit > 250))
	{
		szErrMsg.Format("MovingPicTestMargin must be in the range of -250 to 250");
		AfxMessageBox(szErrMsg);
		return FALSE;
	}

	g_ImageProc.SetMovingPicTestMargin(nDiff_LoLimit, nDiff_HiLimit, nTest_Limit) ;

	return TRUE;
}
BOOL _Set_MacAddress()
{
	CString  sAdd;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		sAdd = GetString();

		sAdd.MakeUpper();
	//	TVCommCtrl.m_aMacAdd.Add(sAdd);
		TVCommCtrl.Set_MAC_Address(sAdd); 
	}
	else
		return FALSE;

	return TRUE;
}
