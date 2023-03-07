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
//#include "HwCtrl\AnalogGrabber.h"
#include "TVCommCtrl.h"
#include "AvSwitchBoxCtrl.h"
#include "ImageProc.h"
#include "BaseImage.h"
#include "ProgressCtrlX.h"
#include "UserMsgDlg.h"
#include "I2cAdcCtrl.h"
#include "CurrentSetting.h"
#include "SoundCard.h"
#include "AvConvertorCtrl.h"
#include "JigCtl.h"
#include "VF1000.h"
#include "PLC_Ctrl.h"


///////////////////////
// External Variable //
///////////////////////

extern CProgressCtrlX	ctrlImgProg;
extern CBaseImage	g_GrabDisplayImage;
extern CBaseImage		g_GrabImage;
extern CBaseImage		g_RefImage;
extern CBaseImage		g_MaskImage;
extern CImageProc		g_ImageProc;
extern UINT				m_nSelectedImage;
extern CDATsysView*		g_pView;
extern CCurrentSetting	g_CurSetting;
//extern BYTE GrabData[2560][1080][3];
extern BOOL RefImage_DirectLoad(CString szRefFileName,int nUseMaskImage);
extern CColorStaticST		ctrlStepTarget_A;

extern CSoundCard		g_SoundCard;
extern _InputSource		InputDefined_FHD[];
extern _InputSource		InputDefined_UHD[];
extern CString				m_strWipId;	//Wip ID


/////////////////////
// Global Variable //
/////////////////////
int g_AutoVideoRetry;
UINT		g_GrabSource;
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

	if (pCurStep->m_nTestType == MODULE_TEST)
	{
		if (pCurStep->GetResult() == FALSE)
		{
			gPLC_Ctrl.m_nFirstVideoNG = 1;
		}
		else
		{
			gPLC_Ctrl.m_nFirstVideoNG = 0;
		}
	}


	if(pCurStep->m_bRunVideoTest)
	{ 
		if(pCurStep->GetResult() == FALSE)
		{
			pCurStep->m_bVideoTestResult = FALSE;
			gPLC_Ctrl.m_nCurrentVideoNG = 1;
			if (gPLC_Ctrl.m_nTestVideoCount == 0)
			{
				gPLC_Ctrl.m_nFirstVideoNG = 1;
			}
		}
		else
		{
			gPLC_Ctrl.m_nCurrentVideoNG = 0;
			pCurStep->m_bVideoTestResult = TRUE;
			gPLC_Ctrl.m_nTestVideoCount++;//gPLC_Ctrl.m_nFirstVideoNG == 1)//m_nTestVideoCount
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
			if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)	
			{
				if(!CurrentSet->bUseAVSwitchBox)
				{
					pCurStep->m_bAudioTestResult = FALSE;
				}
				else
				{
					if(AvSwitchBoxCtrl.AudioMeasure() == TRUE)
					{
						AvSwitchBoxCtrl.AudioMeasure();
					}
				}
			}
			else
			{
				if(!g_SoundCard.AudioMeasure())
				{
					_Wait(800);
					if(!g_SoundCard.AudioMeasure())
					{
					//	if((g_SoundCard.m_fMeasuredLeftLevel < (double)MIN_AUDIO_LEVEL)
					//	|| (g_SoundCard.m_fMeasuredRightLevel < (double)MIN_AUDIO_LEVEL))
					//	{
					//		g_SoundCard.WaveRead_Stop();  _Wait(1000); 
					//		g_SoundCard.WaveRead_Start(); _Wait(1500);
					//	}
					//	else{
							_Wait(1500);
					//	}
						g_SoundCard.AudioMeasure();
					}
				}
			}

		}

		if(((CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX) && (AvSwitchBoxCtrl.m_bResult == FALSE))
		|| ((CurrentSet->nAudioCheckMethod == METHOD_SOUNDCARD)	 && (g_SoundCard.m_bResult == FALSE)))
		{
			pCurStep->m_bAudioTestResult = FALSE;
			pCurStep->SetResult(FALSE);

			sMsg.Format("[A/NG]");
		}
		else
		{
			pCurStep->m_bAudioTestResult = TRUE;

			sMsg.Format("[A/OK]");
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
			pCurStep->m_nLeftFreq[0] 	 = (int)g_SoundCard.m_fTargetLeftFreq;
			pCurStep->m_nRightFreq[0]	 = (int)g_SoundCard.m_fTargetRightFreq;
			pCurStep->m_nLeftLevel[0]	 = (int)g_SoundCard.m_fTargetLeftLevel;
			pCurStep->m_nRightLevel[0]	 = (int)g_SoundCard.m_fTargetRightLevel;

			pCurStep->m_nLeftFreq[1] 	 = (int)g_SoundCard.m_fMeasuredLeftFreq;
			pCurStep->m_nRightFreq[1]	 = (int)g_SoundCard.m_fMeasuredRightFreq;
			pCurStep->m_nLeftLevel[1]	 = (int)g_SoundCard.m_fMeasuredLeftLevel;
			pCurStep->m_nRightLevel[1]	 = (int)g_SoundCard.m_fMeasuredRightLevel;
		}

		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			pCurStep->m_nAudioMargin	 = (int)AvSwitchBoxCtrl.m_fFreqMargin;
		}
		else
		{
			pCurStep->m_nAudioMargin	 = (int)g_SoundCard.m_fFreqMargin;
		}

		if(CurrentSet->bSaveProcessingTimeData)
		{
			DWORD dwFuncElapsedTime = GetTickCount()-pCurStep->m_dwCheckAudioTickCount;
			DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;
			
			CString szFuncElapsedTime;
			szFuncElapsedTime.Format("  E-CheckAudio [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
			AddTimeData(szFuncElapsedTime);
		}

		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 

		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " +sMsg);
			pCurStep->m_strMsg = szPrevMsg + "; " + sMsg;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
			pCurStep->m_strMsg = sMsg;
		}
	}

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;
		
		CString szFuncElapsedTime;
		szFuncElapsedTime.Format("  E-Work (%dms)",dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

//	if(!pCurStep->m_bVideoTestResult 
//	|| (pCurStep->m_bRunAudioTest && !pCurStep->m_bAudioTestResult))
	if(pCurStep->GetResult() == FALSE)
	{
	//	g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpFail);
		g_pView->m_cStepResultNew.SetWindowText(_T("FAIL"));
		g_pView->m_cStepResultNew.SetBkColor(RGB(255, 0, 0));
		return TEST_FAIL;
	}
	else
	{
	//	g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpGood);
		g_pView->m_cStepResultNew.SetWindowText(_T("PASS"));
		g_pView->m_cStepResultNew.SetBkColor(RGB(0, 255, 0));
	//	gPLC_Ctrl.m_nUserRetry = 0;
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

	//g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpGood);
	g_pView->m_cStepResultNew.SetWindowText(_T("PASS"));
	g_pView->m_cStepResultNew.SetBkColor(RGB(0, 255, 0));

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

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}

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

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("  PatternGenOut [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	return bReturn;
}

//+add kwmoon 071015 : [2in1]
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

	if(g_nGrabberType == UHD_GRABBER)
	{
		g_pView->StopLVDSGrabThread();
	}

	CurrentSet->nLvdsWidth  = nGrabWidth;
	CurrentSet->nLvdsHeight = nGrabHeight;
	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&g_GrabImage);
	}//
	else{
		Sleep(100);
		g_pView->StartLVDSGrabThread();
	}

	return TRUE;
}
//+add PSH 101029
BOOL _Set_3D_Grab()
{
	int nGrabMode = 0;
	int nTmp;

	if(g_nGrabberType == UHD_GRABBER)
	{
		return TRUE;
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nGrabMode = GetInteger();

	}
	else return FALSE;

//	ShowSubStepMessage(sMsg, "Set Lvds Resolution");

	if(nGrabMode < 0 || nGrabMode > 2)
		return FALSE;

	nTmp = (CurrentSet->nUranousMode) & 0xDF; 
	switch(nGrabMode) 
	{
		case 0:   break;
		case 1:	  nTmp = nTmp | 0x20; break;
		case 2:	  nTmp = nTmp | 0x120; break;
		default : break;
	}
	if((g_pView->m_nGrabber_FWVer < 0x10102301) ||	(CurrentSet->nFullHd120Hz ==1))
	{
		AddStringToStatus("Set 3D Mode Grab Error");
		return FALSE;
	}
	CurrentSet->nUranousMode = nTmp; 
	CurrentSet->n3DGrabbingMode = nGrabMode;

	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&g_GrabImage);
	}
	return TRUE;
}

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

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}
	
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

	//+add kwmoon 080318
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag))
	{
		if(sCmd.Compare("44") == NULL)
		{
			if(CurrentSet->bSaveProcessingTimeData)
			{
				CString szMsg;
				szMsg.Format("   Remote_Out(44) is skipped");
				AddTimeData(szMsg);
			}

			CString szMsg;
			szMsg.Format("%d.Remote_Out(44) is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(szMsg);

			return TRUE;
		}
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
			AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sCmd);
			_Wait(nDelay);
		}
	}
	else
	{
		return FALSE;
	}

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   RemoteOut(%s) Delay(%d) [%dms] (%dms)",sCmd,nDelay,dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	return TRUE;
}

BOOL _SetTVCommand()
{
	//+chage kwmoon 080402
//	CString sCmd1, sCmd2, sData;
	CString sCmd1 = _T("");
	CString sCmd2 = _T("");
	CString sData = _T("");
	CString sMsg  = _T("");

	int nSetId	   = 0;
	int nWaitLimit = 0;
	
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCmd1	  = GetString();
		sCmd2	  = GetString();

		nSetId	  = GetInteger();
		sData	  = GetString();
		nWaitLimit= GetInteger();
		sMsg	  = GetString();
	}
	else 
	{
		return FALSE;
	}

	//+add kwmoon 080724
	CurrentSet->nAdcType = ADC_TYPE_RS232C;

	//+add kwmoon 080509
	ShowSubStepMessage(sMsg, "Send ADC Command");
	
	TVCommCtrl.m_aCmd1.Add(sCmd1);
	TVCommCtrl.m_aCmd2.Add(sCmd2);
	TVCommCtrl.m_aSetId.Add(nSetId);
	TVCommCtrl.m_aData.Add(sData);
	TVCommCtrl.m_aWaitLimit.Add(nWaitLimit);
	// add 090417
	TVCommCtrl.m_aCheckType.Add(0);
	TVCommCtrl.m_aCheckOption.Add(0);
	//+ 2007.12.5 Add BY USY
	pCurStep->m_bRunAdcTest = TRUE;
	//-

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   SetTvCommand [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

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
	//+chage kwmoon 080402
//	CString sCmd1, sCmd2, sData;
	CString sCmd1 = _T("");
	CString sCmd2 = _T("");
	CString sData = _T("");
	CString szErrMsg; //PSH 20080410
	
	CString sMsg = _T("");

	int nSetId = 0;
	int nWaitLimit = 0;
	
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCmd1	  = GetString();
		sCmd2	  = GetString();

		nSetId	  = GetInteger();
		sData	  = GetString();
		nWaitLimit= GetInteger();
	}
	else 
	{
		return FALSE;
	}

	//+add kwmoon 080724
	CurrentSet->nAdcType = ADC_TYPE_RS232C;

	//+2007.11.21 Add BY USY
	ShowSubStepMessage(sMsg, "Adc Test");

	TVCommCtrl.m_strCmd1 = sCmd1;
	TVCommCtrl.m_strCmd2 = sCmd2;
	TVCommCtrl.m_nSetId  = nSetId;
	TVCommCtrl.m_strData = sData;
	TVCommCtrl.m_nWaitLimit = nWaitLimit;
	//+ 2007.12.5 Add BY USY
	pCurStep->m_bRunAdcTest = TRUE;
	//-
	g_pView->m_bAdcRunning = TRUE;

	//+ Modify PSH 20080410
	if(!CurrentSet->bUseTVCommPort)
	{
		szErrMsg.LoadString(IDS_COMMERROR);
		szErrMsg = szErrMsg + " (TV COM Port)";
		AfxMessageBox(szErrMsg);
		return FALSE;
	}
		if(TVCommCtrl.m_bPortOpen == FALSE)
		{
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				TVCommCtrl.PortClose();
				if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
				{
					//+add kwmoon 080313
//				CString szErrMsg;
					szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
					AfxMessageBox(szErrMsg);
					return FALSE;
				}
			}
		}

	int nResult = TVCommCtrl.AdcTest(TVCommCtrl.m_strCmd1,
									  TVCommCtrl.m_strCmd2,
									  TVCommCtrl.m_nSetId,
									  TVCommCtrl.m_strData, 0,
									  TVCommCtrl.m_nWaitLimit);
	if(nResult == TEST_TIMEOUT)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC TIMEOUT");
	//	TVCommCtrl.SendRun("a", "d", 00, "90");
	//	TVCommCtrl.ResetArray();
		g_pView->m_bAdcRunning = FALSE;
		return FALSE;
	}
	else if(nResult == TEST_FAIL)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC FAIL");
	//	TVCommCtrl.SendRun("a", "d", 00, "90");
	//	TVCommCtrl.ResetArray();
		g_pView->m_bAdcRunning = FALSE;
		return FALSE;
	}
	else if(nResult == TEST_PASS)
	{
		
	}

	g_pView->m_bAdcRunning = FALSE;

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   SendTvCommnad [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	return TRUE;
}

BOOL _SendAdcCheck()
{
	//+chage kwmoon 080402
//	CString sCmd1, sCmd2, sData, sTargetAvgData;
	CString sCmd1 = _T("");
	CString sCmd2 = _T("");
	CString sData = _T("");
	CString sTargetAvgData = _T("");
	
	CString sMsg = _T("");
	CString szErrMsg; //PSH

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

	//+2007.11.21 Add BY USY
	ShowSubStepMessage(sMsg, "Adc Test");

	TVCommCtrl.m_strCmd1 = sCmd1;
	TVCommCtrl.m_strCmd2 = sCmd2;
	TVCommCtrl.m_nSetId  = nSetId;
	TVCommCtrl.m_strData = sData;
	TVCommCtrl.m_nWaitLimit = nWaitLimit;
	TVCommCtrl.m_nCheckType = nCheckType;
	TVCommCtrl.m_strTargetAvg = sTargetAvgData;
	TVCommCtrl.m_nAvgMargin = nTargetMargin;
	
	//+ 2007.12.5 Add BY USY
	pCurStep->m_bRunAdcTest = TRUE;
	//-

	//+ Modify PSH 20080410
	if(!CurrentSet->bUseTVCommPort)
	{
		szErrMsg.LoadString(IDS_COMMERROR);
		szErrMsg = szErrMsg + " (TV COM Port)";
		AfxMessageBox(szErrMsg);
		return FALSE;
	}

		if(TVCommCtrl.m_bPortOpen == FALSE)
		{
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				TVCommCtrl.PortClose();
				if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
				{
					//+add kwmoon 080313
					szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
					AfxMessageBox(szErrMsg);
					return FALSE;
				}
			}
		}

	int nResult = TVCommCtrl.AdcTest(TVCommCtrl.m_strCmd1,
									  TVCommCtrl.m_strCmd2,
									  TVCommCtrl.m_nSetId,
									  TVCommCtrl.m_strData,
									  TVCommCtrl.m_nCheckType,
									  TVCommCtrl.m_nWaitLimit);

	if(nResult == TEST_TIMEOUT)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC TIMEOUT");
	//	TVCommCtrl.SendRun("a", "d", 00, "90");
	//	TVCommCtrl.ResetArray();
		g_pView->m_bAdcRunning = FALSE;
		return FALSE;
	}
	else if(nResult == TEST_FAIL)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC FAIL");
	//	TVCommCtrl.SendRun("a", "d", 00, "90");
	//	TVCommCtrl.ResetArray();
		g_pView->m_bAdcRunning = FALSE;
		return TRUE;
	}
	else if(nResult == TEST_PASS)
	{
		
	}

	return TRUE;
}
//-
//+2007.12.11 Add BY USY
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
	

	g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1

	switch(nVideoRfType)
	{
		case 0 : g_VideoSourceType = V_SRC_NTSC; break; // V_SRC_NTSC : 0
		case 1 : g_VideoSourceType = V_SRC_PAL;  break; // V_SRC_PAL  : 1
	}
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_pView->StopLVDSGrabThread();
		Sleep(100);
		g_pView->StartLVDSGrabThread();
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
	
	if(g_GrabSource == LVDS_GRABBER)
	{
		if((ptStartPosition.x < 0) || (ptEndPosition.x > (int)(CurrentSet->nLvdsWidth-1)))
		{
			szErrMsg.Format("Roi must be in the X range of 0 to %d",CurrentSet->nLvdsWidth-1);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}

		if((ptStartPosition.y < 0) || (ptEndPosition.y > (int)(CurrentSet->nLvdsHeight-1)))
		{
			szErrMsg.Format("Roi must be in the Y range of 0 to %d",CurrentSet->nLvdsHeight-1);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}
	else if (g_GrabSource == HDMI_GRABBER) {
		//nWidth = CurrentSet->nHdmiWidth;
		//nHeight = CurrentSet->nHdmiHeight;
		if ((ptStartPosition.x < 0) || (ptEndPosition.x > (int)(CurrentSet->nHdmiWidth - 1)))
		{
			szErrMsg.Format("Roi must be in the X range of 0 to %d", CurrentSet->nHdmiWidth - 1);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}

		if ((ptStartPosition.y < 0) || (ptEndPosition.y > (int)(CurrentSet->nHdmiHeight - 1)))
		{
			szErrMsg.Format("Roi must be in the Y range of 0 to %d", CurrentSet->nHdmiHeight - 1);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}
	else // ANALOG_GRABBER
	{
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
	}
	//-

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

	if(nMillisecond == 0){ return TRUE;}

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

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("   SetAvSwitchBox");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
	}
	
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

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   SetAvSwitchBox [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}
	
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
	int nUseMaskImage		= 0;	// 0 : Dont't Use, 1 : Use

	CString sRefTmp		= _T("");
	CString sGrabPath	= _T("");
	CString sRefPath	= _T("");
	CString sMsg		= _T("");
	CString szFileName	= _T("");
	CString szErrorMsg	= _T("");


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

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-GrabNload");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
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
			if(CurrentSet->bSaveProcessingTimeData)
			{
				szFuncElapsedTime.Format("  E-GrabNload is skipped");
				AddTimeData(szFuncElapsedTime);
			}

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
	else
	{

		//================
		// Same Ref Image
		//================
		if(g_CurSetting.IsSameGrabRefImage(sRefTmp) == TRUE)
		{
			if(CurrentSet->bSaveProcessingTimeData)
			{
				szFuncElapsedTime.Format("  E-GrabNload is skipped");
				AddTimeData(szFuncElapsedTime);
			}

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


	ctrlImgProg.SetPos(60);

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(!Grab_Image_UHD(sRefTmp,nUseMaskImage))
		{
			return TEST_ABORT;
		}
	}
	else//FHD
	{
		if(!Grab_Image_FHD(sRefTmp,nUseMaskImage))
		{
			return TEST_ABORT;
		}
	}
	ctrlImgProg.SetPos(80);

	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);
	
//	pCurStep->m_strGrabPath = sGrabPath;
	pCurStep->m_strGrabPath = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".jpg";
	pCurStep->m_strProcPath = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";

	if(CurrentSet->bSaveGrabImg)
	{
	//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
		g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
	}
//	g_GrabImage.m_szFilePath = sGrabPath;
	g_GrabImage.m_szFilePath = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".jpg";

	ctrlImgProg.SetPos(100);

	g_pView->InvalidateRect(nImageRect, FALSE);
	g_pView->UpdateWindow();

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;
		
		dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("  E-GrabNload [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}
	return TEST_PASS;
}


UINT Grab_N_Load_Image_MakeRef_2in1()
{
	int nUseMaskImage		= 0;	// 0 : Dont't Use, 1 : Use
	CString sRefTmp					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");
	CString sMsg					= _T("");
	CString szFileName				= _T("");
	CString szIntermediateFileName	= _T("");
	CString szIntermediateFilePath	= _T("");
	CString szAvgFileName			= _T("");
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

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			if(!Grab_Image_UHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}
		else//FHD
		{
			if(!Grab_Image_FHD(_T(""),0))
			{
				return TEST_ABORT;
			}
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
		else if(pCurStep->GetCurrentSource().Left(3).Compare("AVC")  == NULL)
		{
			if(CurrentSet->bHdmiGrab_MaskOsd){
				if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,TRUE))
				{
					AfxMessageBox("Failed to make Mask image!"); 
					return TEST_ABORT;
				}
			}
			else{
				if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,FALSE))
				{
					AfxMessageBox("Failed to make Mask image!");
					return TEST_ABORT;
				}
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

	}

	g_pView->InvalidateRect(nImageRect, FALSE);
	g_pView->UpdateWindow();


	return TEST_PASS;			
}

BOOL _Grab_Image_Check()
{
	BOOL bResult = FALSE;

	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&g_GrabImage);
	}
	if(g_nGrabberType == FHD_GRABBER)
	{
		if(g_pView->m_bContinuousLVDSGrabRunning)
		{
			g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
		}
	}

	if(g_pView->m_bMakeReferenceMode)
	{
		bResult = Grab_Image_Check_MakeRef_2in1();
	}
	else
	{
		bResult = Grab_Image_Check_Normal_2in1();
	}

	if(g_nGrabberType == FHD_GRABBER)
	{
		if((pCurStep->m_nStepType == PROCESS_FUNCTION) && (pCurStep->m_bTest))				
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
	}

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
	int nTotalNoErrorPixel	= 0;

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

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

	// Compare Images
	int nResult = TEST_FAIL;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;
	
	float fMaxDifference  = 0.0;


	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-GrabImageCheck");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
	}
	
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
	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);

//	pCurStep->m_strGrabPath  = sGrabPath;
	pCurStep->m_strGrabPath = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".jpg";
	pCurStep->m_strProcPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";
//	g_GrabImage.m_szFilePath = sGrabPath;
	g_GrabImage.m_szFilePath = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".jpg";


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
	
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(!Grab_Image_UHD(sRefTmp,nUseMaskImage))
		{
			return TEST_ABORT;
		}
	}
	else//FHD
	{
		if(!Grab_Image_FHD(sRefTmp,nUseMaskImage))
		{
			return TEST_ABORT;
		}
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

	//+add kwmoon 080125
	g_pView->InvalidateRect(nImageRect, FALSE);
	g_pView->UpdateWindow(); _Wait(10);

	if(nResult == TEST_ABORT)
	{
		if(CurrentSet->bSaveGrabImg)
		{
		//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
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
			//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
				g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			}
			g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 
			return TEST_ABORT;
		}
		
		if(dwElapsedTime < (DWORD)nMaxDelay) goto GRAB_START;
	}

	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 
	
	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("  E-Grab_Image_Check [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	if(CurrentSet->bSaveGrabImg)
	{
	//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
		g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
	}

	return TEST_PASS;
}

//+add PSH 080412
UINT Grab_Image_Check_OSD_2in1()
{
	int nMaxDelay		 = 0;
	int nUseMaskImage	 = 0;	// 0 : Dont't Use, 1 : Use


	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");

	CRect nImageRect;
	CWnd* pImageWnd	= NULL;

	DWORD dwElapsedTime				= 0;
	DWORD dwTickCount				= GetTickCount();

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	// Compare Images
	int nResult = TEST_FAIL;

	int nTotalNoErrorPixel  = 0;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	float fMaxDifference = 0.0;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sRefTmp			= GetString();
		nUseMaskImage	= GetInteger();
		nMaxDelay		= GetInteger();
		sMsg			= GetString();
	}

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}

	//+add kwmoon 080305
	// if NG/A
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
	//-

GRAB_START :

	sMsg1 = sMsg;
	sMsg2 = _T("");

	if(CurrentSet->bRunAbort)
	{
		g_pView->m_nNoCurrentRepeatExecution = 1; 

		//+del kwmoon 080724
		//+add kwmoon 080704
	//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 
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
	

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(!Grab_Image_UHD(sRefTmp,nUseMaskImage))
		{
			return TEST_ABORT;
		}
	}
	else//FHD
	{
		if(!Grab_Image_FHD(sRefTmp,nUseMaskImage))
		{
			return TEST_ABORT;
		}
	}

	g_CurSetting.m_szGrabRefImageName = sRefTmp;

	ctrlImgProg.SetPos(80);

	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sRefTmp);

//	pCurStep->m_strGrabPath  = sGrabPath;
	pCurStep->m_strGrabPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".jpg";
	pCurStep->m_strProcPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";
//	g_GrabImage.m_szFilePath = sGrabPath;
	g_GrabImage.m_szFilePath = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".jpg";

//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
	g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
	ctrlImgProg.SetPos(100);


	//+add kwmoon 080819
	g_ImageProc.m_ptPositionShift.x = 0;
	g_ImageProc.m_ptPositionShift.y = 0;

	//+change kwmoon 080108
//	g_ImageProc.SetColorLimit(-30.0,30.0);
	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);

	g_ImageProc.SetAvgMaskSize(0,0,0); 

	nResult = g_ImageProc.RGBTest(OSD_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
								  ptResultPosition,fMaxDifference,nTotalNoErrorPixel,nUseMaskImage,ctrlImgProg);

	g_ImageProc.SetAvgMaskSize(0,0,0); 
	//+del kwmoon 080618
//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	//+add kwmoon 080125
	g_pView->InvalidateRect(nImageRect, FALSE);
	g_pView->UpdateWindow(); _Wait(10);

	if(nResult == TEST_ABORT)
	{
		//+del kwmoon 080724
		//+add kwmoon 080704
	//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 
		return TEST_ABORT;
	}

	dwElapsedTime = GetTickCount()-dwTickCount;
 
	if(nResult == TEST_FAIL)
	{
		if(CurrentSet->bRunAbort)
		{
			//+del kwmoon 080724
			//+add kwmoon 080704
		//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 
			return TEST_ABORT;
		}

		if(dwElapsedTime < (DWORD)nMaxDelay)
		{
			goto GRAB_START;
		}
	}
	
	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   Grab_Image_Check_OSD [%dms]",dwFuncElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	//+del kwmoon 080724
	//+add kwmoon 080704
//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	return TEST_PASS;
}

BOOL Grab_Image_UHD(CString sRefImagePath, int nUseMaskImage)
{
	int nWidth		= 0;
	int nHeight		= 0;
	int nBitCount	= 0;
	int nRowWidth	= 0;
	int nImageSize	= 0;
	int nGrabResult = GRAB_FAIL;

	CString szErrorMsg	= _T("");

	CRect nImageRect;
	CWnd* pImageWnd	= NULL;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	CString sMsg		  = _T("");
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


	/////////////////////
	// Grab LVDS Image //
	/////////////////////
	if(g_GrabSource == LVDS_GRABBER)
	{
		nWidth		= CurrentSet->nLvdsWidth;
		nHeight		= CurrentSet->nLvdsHeight;
		nBitCount	= CurrentSet->nBitCount;
		nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
		nImageSize  = nRowWidth * nHeight;

		// Load a Reference Image
		if(sRefImagePath != _T(""))
		{
			if(!RefImage_DirectLoad(sRefImagePath,nUseMaskImage)) return FALSE;
		}

		if((nGrabResult = g_pView->Grab_UHD()) != GRAB_PASS)
		{
			szErrorMsg = "No LVDS input image!";
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);
			g_GrabImage.m_bImageLoaded = TRUE;

			return TRUE;
		}
	}
	///////////////////////
	// Grab Analog Image //
	///////////////////////
	else
	{
		if(sRefImagePath != _T(""))
		{
			if(!RefImage_DirectLoad(sRefImagePath,nUseMaskImage)) return FALSE;
		}

		nGrabResult = g_pView->Grab_UHD();
		
		if(nGrabResult != GRAB_PASS)		
		{
			if(nGrabResult == GRAB_FAIL)
			{
				szErrorMsg = "No Analog input image!";
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);
			}
			g_GrabImage.m_bImageLoaded = TRUE;
		}

		nWidth		= CurrentSet->nAnalogWidth;
		nHeight		= CurrentSet->nAnalogHeight;
		nBitCount	= 24;
		nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
		nImageSize  = nRowWidth * nHeight;

//		g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

		nRowWidth = WIDTHBYTES(g_GrabImage.m_BmInfo->bmiHeader.biBitCount*g_GrabImage.m_BmInfo->bmiHeader.biWidth);

		if(nGrabResult != GRAB_PASS)
		{
			memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);

			if(nGrabResult == GRAB_ABORT) return FALSE;
		} 
	//	return FALSE;
	}
	g_GrabImage.m_bImageLoaded = TRUE;


	return TRUE;
}

BOOL Grab_Image_FHD(CString sRefImagePath, int nUseMaskImage)
{
	int nWidth		= 0;
	int nHeight		= 0;
	int nBitCount	= 0;
	int nRowWidth	= 0;
	int nImageSize	= 0;
	int nGrabResult = GRAB_FAIL;

	CString szErrorMsg	= _T("");

	CRect nImageRect;
	CWnd* pImageWnd	= NULL;

	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	CString sMsg		  = _T("");
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


	/////////////////////
	// Grab LVDS Image //
	/////////////////////
	if(g_GrabSource == LVDS_GRABBER)
	{
		nWidth		= CurrentSet->nLvdsWidth;
		nHeight		= CurrentSet->nLvdsHeight;
		nBitCount	= CurrentSet->nBitCount;
		nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
		nImageSize  = nRowWidth * nHeight;

		g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);
	
		// Load a Reference Image
		if(sRefImagePath != _T(""))
		{
			if(!RefImage_DirectLoad(sRefImagePath,nUseMaskImage)) return FALSE;
		}

		if((nGrabResult = g_pView->Grab_FHD(GRAB_WITH_RETRY)) != GRAB_PASS)
		{
			szErrorMsg = "No LVDS input image!";
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);
			g_GrabImage.m_bImageLoaded = TRUE;

			return TRUE;
		}
	}
	///////////////////////
	// Grab Analog Image //
	///////////////////////
	else
	{
		if(sRefImagePath != _T(""))
		{
			if(!RefImage_DirectLoad(sRefImagePath,nUseMaskImage)) return FALSE;
		}

		nGrabResult = g_pView->Grab_FHD(GRAB_WITH_NO_RETRY,EXT_ANALOG_GRABBER);
		
		if(nGrabResult != GRAB_PASS)		
		{
			if(nGrabResult == GRAB_FAIL)
			{
				szErrorMsg = "No Analog input image!";
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);
			}
			g_GrabImage.m_bImageLoaded = TRUE;
		}

		nWidth		= CurrentSet->nAnalogWidth;
		nHeight		= CurrentSet->nAnalogHeight;
		nBitCount	= 24;
		nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
		nImageSize  = nRowWidth * nHeight;

		g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

		nRowWidth = WIDTHBYTES(g_GrabImage.m_BmInfo->bmiHeader.biBitCount*g_GrabImage.m_BmInfo->bmiHeader.biWidth);

		if(nGrabResult != GRAB_PASS)
		{
			memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);

			if(nGrabResult == GRAB_ABORT) return FALSE;
		}
	}
	g_GrabImage.m_bImageLoaded = TRUE;


	return TRUE;
}

UINT Grab_Image_Check_MakeRef_2in1()
{
	int	nMaxDelay		 = 0;
	int nUseMaskImage	 = 0;	// 0 : Dont't Use, 1 : Use

	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");
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
	while(TRUE)
	{
		if(CurrentSet->bRunAbort)
		{
			g_pView->m_nNoCurrentRepeatExecution = 1; 
			return TEST_ABORT;
		}

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			if(!Grab_Image_UHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}
		else//FHD
		{
			if(!Grab_Image_FHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMaxDelay) break;
	}	

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

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			if(!Grab_Image_UHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}
		else//FHD
		{
			if(!Grab_Image_FHD(_T(""),0))
			{
				return TEST_ABORT;
			}
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
		else if(pCurStep->GetCurrentSource().Left(3).Compare("AVC")  == NULL)
		{
			if(CurrentSet->bHdmiGrab_MaskOsd){
				if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,TRUE))
				{
					AfxMessageBox("Failed to make Mask image!"); 
					return TEST_ABORT;
				}
			}
			else{
				if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,FALSE))
				{
					AfxMessageBox("Failed to make Mask image!");
					return TEST_ABORT;
				}
			}
		}
		//==========================================
		// LVDS Image : Put white box at OSD region
		//==========================================
		else
		{
			if((pCurStep->GetCurrentSource().Left(2).Compare("AV")	== NULL)
			|| (pCurStep->GetCurrentSource().Left(3).Compare("COMP")  == NULL))
			{
				nMaskWidth = 5;
			}

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

	}

	pCurStep->m_bMakeRefDone = TRUE;
	
	g_pView->InvalidateRect(nImageRect, FALSE);
	g_pView->UpdateWindow();


	return TEST_PASS;
}

//+add PSH 080412
UINT Grab_Image_Check_OSD_MakeRef_2in1()
{
	int	nMaxDelay		 = 0;
	int nUseMaskImage	 = 0;	// 0 : Dont't Use, 1 : Use

	CString sRefTmp					= _T("");
	CString sGrabPath				= _T("");
	CString sRefPath				= _T("");
	CString sMsg					= _T("");
	CString sMsg1					= _T("");
	CString sMsg2					= _T("");
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
			
			//+change kwmoon 080319
		//	return FALSE;
			return TEST_ABORT;
		}

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			if(!Grab_Image_UHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}
		else//FHD
		{
			if(!Grab_Image_FHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

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

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			if(!Grab_Image_UHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}
		else//FHD
		{
			if(!Grab_Image_FHD(_T(""),0))
			{
				return TEST_ABORT;
			}
		}

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

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
	//	if(pCurStep->m_strCurSrc.Left(4).Compare("VOUT") == NULL)
	//	|| (pCurStep->GetCurrentSource().Left(3).Compare("AVC")  == NULL))
		if((pCurStep->GetCurrentSource().Left(4).Compare("VOUT")	== NULL)
		|| (pCurStep->GetCurrentSource().Left(6).Compare("AV-OUT")  == NULL)
		|| (pCurStep->GetCurrentSource().Left(7).Compare("MNT-OUT") == NULL)
		|| (pCurStep->GetCurrentSource().Left(6).Compare("TV-OUT")  == NULL))
		{
			if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,FALSE))
			{
				AfxMessageBox("Failed to make Mask image!"); 
				//+change kwmoon 080319
			//	return FALSE;
				return TEST_ABORT;
			}
		}
		else if(pCurStep->GetCurrentSource().Left(3).Compare("AVC")  == NULL)
		{
			if(CurrentSet->bHdmiGrab_MaskOsd){
				if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,TRUE))
				{
					AfxMessageBox("Failed to make Mask image!"); 
					return TEST_ABORT;
				}
			}
			else{
				if(!g_ImageProc.MakeMaskImg(nMaskWidth, g_pView->m_Image[I_AVG], g_pView->m_Image[I_MASK], ctrlImgProg,FALSE))
				{
					AfxMessageBox("Failed to make Mask image!");
					return TEST_ABORT;
				}
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

	int nAudioSource = 0;
	int nAudioCHchangeDelay	= 0;
	int nTemp  = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-CheckAudio");
		AddTimeData(sMsg);
		pCurStep->m_dwCheckAudioTickCount = GetTickCount();
	}

	//+add PSH 080816
//	g_SoundCard.m_bMeasureAudio = FALSE;

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

			nTemp								= GetInteger();
			AvSwitchBoxCtrl.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;
			
			//+add kwmoon 080724
			AvSwitchBoxCtrl.m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
		}
		else
		{
			//+add PSH 090305
			g_SoundCard.m_bMeasureAudioType = FREQ_LEVEL_CHECK;
			g_SoundCard.m_fTargetLeftFreq	= (float)GetInteger();
			g_SoundCard.m_fTargetRightFreq	= (float)GetInteger();
			g_SoundCard.m_fTargetLeftLevel	= (float)GetInteger();
			g_SoundCard.m_fTargetRightLevel	= (float)GetInteger();

			nTemp							= GetInteger();
			g_SoundCard.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;			

			//+add kwmoon 080724
			g_SoundCard.m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
		}

		sMsg			= GetString();
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
		g_pView->AudioMeasureStop();
		//_Wait(150);
		g_pView->AudioMeasureStart();
		
		//_Wait(1000);
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
	

//	g_SoundCard.m_bMeasureAudio = FALSE;

	if (pos!= NULL)
	{
		nAudioSource	= GetInteger();

		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = LEVEL_CHECK;
			AvSwitchBoxCtrl.m_fTargetLeftLevel	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightLevel	= (float)GetInteger();
			AvSwitchBoxCtrl.m_nCheckTimeLimit = 0;

			AvSwitchBoxCtrl.m_fLevelMargin = (float)CurrentSet->nLevelMargin;
			
		}
		else
		{
			//+add PSH 090305
			g_SoundCard.m_bMeasureAudioType = LEVEL_CHECK;
			g_SoundCard.m_fTargetLeftLevel	= (float)GetInteger();
			g_SoundCard.m_fTargetRightLevel	= (float)GetInteger();
			g_SoundCard.m_nCheckTimeLimit = 0;

			g_SoundCard.m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
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
		g_pView->AudioMeasureStop();
		//_Wait(150);
		g_pView->AudioMeasureStart();
		
		//_Wait(1000);
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
	

//	g_SoundCard.m_bMeasureAudio = FALSE;

	if (pos!= NULL)
	{
		nAudioSource	= GetInteger();

		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = FREQUENCY_CHECK;
			AvSwitchBoxCtrl.m_fTargetLeftFreq	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightFreq	= (float)GetInteger();
			AvSwitchBoxCtrl.m_nCheckTimeLimit = 0;

			AvSwitchBoxCtrl.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;
		}
		else
		{
			//+add PSH 090305
			g_SoundCard.m_bMeasureAudioType = FREQUENCY_CHECK;
			g_SoundCard.m_fTargetLeftFreq	= (float)GetInteger();
			g_SoundCard.m_fTargetRightFreq	= (float)GetInteger();
			g_SoundCard.m_nCheckTimeLimit = 0;

			g_SoundCard.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;			
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
		
		g_pView->AudioMeasureStop();
		//_Wait(150);
		g_pView->AudioMeasureStart();
		//_Wait(1000);
	}


//	pCurStep->m_bRunAudioTest = TRUE;

	return TRUE;
}

//+add 090305
BOOL _CheckAudio_Time_Level()
{
	CString sCmd = _T("");
	CString sMsg = _T("");

	int nAudioSource = 0;
	int nAudioCHchangeDelay = 0;
	int nTemp = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();


	//	g_SoundCard.m_bMeasureAudio = FALSE;

	if (pos != NULL)
	{
		nAudioSource = GetInteger();

		if (CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = LEVEL_CHECK;
			AvSwitchBoxCtrl.m_fTargetLeftLevel = (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightLevel = (float)GetInteger();
			AvSwitchBoxCtrl.m_nCheckTimeLimit = GetInteger();

			AvSwitchBoxCtrl.m_fLevelMargin = (float)CurrentSet->nLevelMargin;
		}
		else
		{
			//+add PSH 090305
			g_SoundCard.m_bMeasureAudioType = LEVEL_CHECK;
			g_SoundCard.m_fTargetLeftLevel = (float)GetInteger();
			g_SoundCard.m_fTargetRightLevel = (float)GetInteger();
			g_SoundCard.m_nCheckTimeLimit = GetInteger();

			g_SoundCard.m_fLevelMargin = (float)CurrentSet->nLevelMargin;
		}
	}
	else
	{
		return FALSE;
	}

	//+add kwmoon 080507
	if (g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}

	//+add kwmoon 070929
	if ((pCurStep->m_bStepRetryFlag) || !g_CurSetting.IsSameAudioSource(nAudioSource))
	{
		SetAudioSource(nAudioSource);
		g_pView->AudioMeasureStop();
		//_Wait(150);
		g_pView->AudioMeasureStart();

		//_Wait(1000);
	}


	//	pCurStep->m_bRunAudioTest = TRUE;

	return TRUE;
}

//+add 220222
BOOL _CheckAudio_Time_Frequency()
{
	CString sCmd = _T("");
	CString sMsg = _T("");

	int nAudioSource = 0;
	int nAudioCHchangeDelay = 0;
	int nTemp = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();


	//	g_SoundCard.m_bMeasureAudio = FALSE;

	if (pos != NULL)
	{
		nAudioSource = GetInteger();

		if (CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = FREQUENCY_CHECK;
			AvSwitchBoxCtrl.m_fTargetLeftFreq = (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightFreq = (float)GetInteger();
			AvSwitchBoxCtrl.m_nCheckTimeLimit = GetInteger();

			AvSwitchBoxCtrl.m_fFreqMargin = (float)CurrentSet->nFreqMargin;
		}
		else
		{
			//+add PSH 090305
			g_SoundCard.m_bMeasureAudioType = FREQUENCY_CHECK;
			g_SoundCard.m_fTargetLeftFreq = (float)GetInteger();
			g_SoundCard.m_fTargetRightFreq = (float)GetInteger();
			g_SoundCard.m_nCheckTimeLimit = GetInteger();

			g_SoundCard.m_fFreqMargin = (float)CurrentSet->nFreqMargin;
		}
	}
	else
	{
		return FALSE;
	}

	//+add kwmoon 080507
	if (g_pView->m_bMakeReferenceMode)
	{
		return TRUE;
	}

	//+add kwmoon 070929
	if ((pCurStep->m_bStepRetryFlag) || !g_CurSetting.IsSameAudioSource(nAudioSource))
	{
		SetAudioSource(nAudioSource);

		g_pView->AudioMeasureStop();
		//_Wait(150);
		g_pView->AudioMeasureStart();
		//_Wait(1000);
	}


	//	pCurStep->m_bRunAudioTest = TRUE;

	return TRUE;
}

BOOL _CheckAudio_Level_NoFrequency()
{
	CString sCmd = _T("");
	CString sMsg = _T("");

	int nAudioSource = 0;
	int nAudioCHchangeDelay	= 0;
	int nTemp  = 0;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-CheckAudio");
		AddTimeData(sMsg);
		pCurStep->m_dwCheckAudioTickCount = GetTickCount();
	}

	//+add PSH 080816
//	g_SoundCard.m_bMeasureAudio = FALSE;

	if (pos!= NULL)
	{
		nAudioSource	= GetInteger();

		//+add kwmoon 080523
		if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			//+add PSH 090305
			AvSwitchBoxCtrl.m_bMeasureAudioType = FREQ_LEVEL_CHECK_MOVING;
			AvSwitchBoxCtrl.m_fTargetLeftFreq	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightFreq	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetLeftLevel	= (float)GetInteger();
			AvSwitchBoxCtrl.m_fTargetRightLevel	= (float)GetInteger();

			nTemp								= GetInteger();
			AvSwitchBoxCtrl.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;
			
			//+add kwmoon 080724
			AvSwitchBoxCtrl.m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
		}
		else
		{
			//+add PSH 090305
			g_SoundCard.m_bMeasureAudioType = FREQ_LEVEL_CHECK_MOVING;
			g_SoundCard.m_fTargetLeftFreq	= (float)GetInteger();
			g_SoundCard.m_fTargetRightFreq	= (float)GetInteger();
			g_SoundCard.m_fTargetLeftLevel	= (float)GetInteger();
			g_SoundCard.m_fTargetRightLevel	= (float)GetInteger();

			nTemp							= GetInteger();
			g_SoundCard.m_fFreqMargin		= (float)CurrentSet->nFreqMargin;			

			//+add kwmoon 080724
			g_SoundCard.m_fLevelMargin		= (float)CurrentSet->nLevelMargin;
		}

		sMsg			= GetString();
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
		g_pView->AudioMeasureStop();
		//_Wait(150);
		g_pView->AudioMeasureStart();
		
		//_Wait(1000);
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

BOOL _SetInput()
{
	CString sMsg			= _T("");
	CString sInputSource	= _T("");
	CString sInputSourceKey = _T("");
//	CString sString			= _T("");
	
	int nSourceKeyDelay	= 0;
	int nTotalNoErrorPixel	= 0;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	CString szMsg  = _T("");

	int	  nUseMaskImage = 0;
	//+add kwmoon 080716
	float fMaxDifference = 0.0;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
//	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("   SetInput");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
	}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sInputSource	= GetString();
		sInputSourceKey	= GetString();
		nSourceKeyDelay	= GetInteger();
		sMsg			= GetString();
	}
	else 
	{
		return FALSE;
	}

	if(g_CurSetting.m_nNotSkipSameSourceChannel == 0)
	{
		if(g_CurSetting.IsSameInputSource(sInputSourceKey))
		{
			//+add kwmoon 080222
			if(pCurStep->m_bStepRetryFlag || pCurStep->m_bStepInternalRetryFlag)
			{

				//+add kwmoon 080305
				// if NG/A
				if((pCurStep->m_bAudioTestResult == FALSE) && (pCurStep->m_bVideoTestResult == TRUE))
				{
					szMsg.Format("%d.Set Input %s is skipped",pCurStep->m_nStep,sInputSource);
					AddStringToRemoteComLog(szMsg);

					return TRUE;
				}
				//-

				if((pCurStep->GetCurrentSource().Compare("RF") == NULL))
				{
					CStep* pPrevStep = NULL;
				
					POSITION PosCurrentStep = StepList.FindIndex(pCurStep->m_nStep-1);

					if(PosCurrentStep != StepList.GetHeadPosition())
					{
						pPrevStep = StepList.GetPrev(PosCurrentStep);
					}

						//+add kwmoon 080819
					g_ImageProc.m_ptPositionShift.x = 0;
					g_ImageProc.m_ptPositionShift.y = 0;

					g_ImageProc.SetColorLimit((float)pPrevStep->m_fLowLimit,(float)pPrevStep->m_fHighLimit);
					g_ImageProc.SetAvgMaskSize(0,0,0); 

					// Load a ref-image & mask-image of a previous step.
					if(pPrevStep->m_strGrabPath != _T(""))
					{
						g_RefImage.LoadImage(pPrevStep->m_strGrabPath,ctrlImgProg);
					}

					if(pPrevStep->m_strMaskPath != _T(""))
					{
						g_MaskImage.LoadImage(pPrevStep->m_strMaskPath,ctrlImgProg); nUseMaskImage = 1;
					}

					int nResult = g_ImageProc.RGBTest(NORMAL_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
												  ptResultPosition,fMaxDifference,nTotalNoErrorPixel,nUseMaskImage,ctrlImgProg);

					g_ImageProc.SetAvgMaskSize(0,0,0); 
					
					//+del kwmoon 080618
				//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition);
					
					if(nResult != TEST_PASS)
					{
					//	CString szMsg;
						szMsg.Format("%d.Set Input %s is skipped",pCurStep->m_nStep,sInputSource);
						AddStringToRemoteComLog(szMsg);

						return TRUE;
					}
				}
			}

			//+del kwmoon 080131
			if((pCurStep->GetCurrentSource().Compare("RF") == NULL))
			{
			//	CString szMsg;
				szMsg.Format("%d.Set Input %s is skipped",pCurStep->m_nStep,sInputSource);
				AddStringToRemoteComLog(szMsg);

				return TRUE;
			}
		}
	}
	
	ShowSubStepMessage(sMsg, "Set Input Source");

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
				//	CString szErrMsg;
					szMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
					AfxMessageBox(szMsg);
					return FALSE;
				}
			}
		}
		//+2007.12.26 Mod BY USY
		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sInputSourceKey))
		{
			return FALSE;
		}
		if(!_Wait(nSourceKeyDelay)) return FALSE;
	}

	//+add kwmoon 071205
//	pCurStep->SetCurrentSource(sInputSource);
	g_CurSetting.m_szInputSource = sInputSourceKey;
	//-

	//+add kwmoon 071206
	szMsg.Format("%d.Set Input Source %s",pCurStep->m_nStep,sInputSource);
	AddStringToRemoteComLog(szMsg);

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwElapsedTime = GetTickCount()-dwFuncTickCount;
		szMsg.Format("   SetInput [%dms]",dwFuncElapsedTime);
		AddTimeData(szMsg);
	}
	
	return TRUE;
}
 
BOOL _SetInputSource()
{
	//+add kwmoon 071108
	DWORD dwTickCount = GetTickCount();

	CString sMsg			= _T("");
	CString sInputSource	= _T("");
	CString sInputSourceKey = _T("");
	
	int nSourceKeyDelay	= 0;
	int nRemoteDelay	= 0;
	int nSendExit		= 0;	// 1 : Send Exit Key 

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sInputSource	= GetString();
		sInputSourceKey	= GetString();
		nSourceKeyDelay	= GetInteger();
		nRemoteDelay	= GetInteger();
		nSendExit		= GetInteger();
		sMsg			= GetString();
	}
	else 
	{
		return FALSE;
	}

	if(g_CurSetting.m_nNotSkipSameSourceChannel == 0)
	{
		//+change kwmoon 071224
	//	if(g_CurSetting.IsSameInputSource(sInputSourceKey))
		if((!pCurStep->m_bStepRetryFlag) && (!pCurStep->m_bStepInternalRetryFlag)
		&& (g_CurSetting.IsSameInputSource(sInputSourceKey)))
		{
			if((pCurStep->GetCurrentSource().Compare("RF") == NULL))
			{
				return TRUE;
			}
		}
	}
	
	ShowSubStepMessage(sMsg, "Set Input Source");

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
		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sInputSourceKey))
		{
			return FALSE;
		}
		if(!_Wait(nSourceKeyDelay)) return FALSE;

		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY,"44")) return FALSE;
		if(!_Wait(nRemoteDelay)) return FALSE;
	}

	if(nSendExit)
	{
		if(CurrentSet->bUseAVSwitchBox)
		{
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")) return FALSE;
			if(!_Wait(nRemoteDelay)) return FALSE;
		}
	}

//	pCurStep->SetCurrentSource(sInputSource);
	g_CurSetting.m_szInputSource = sInputSourceKey;

	AddStringToRemoteComLog("Set Input Souce " + sInputSource);
	return TRUE;
}

//+add kwmoon 071217
BOOL _SetRf()
{
	CString sMsg			= _T("");
	CString sErrorMsg		= _T("");
	CString sString			= _T("");

	int nChannel		 = 0;
	int nRemoteDelay	 = 0;
	int nNoDigit		 = 1;

	//+add kwmoon 071226
	int nRetryDelay		 = 0;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sString.Format("   SetRF");
		AddTimeData(sString);
		dwFuncTickCount = GetTickCount();
	}
	//+add kwmoon 080201
	float fMesuredValue = (float)pCurStep->GetMeasured_float();

	//===================
	// Retry
	//===================
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag))
	{
		//=========
		// NG/A
		//=========
		if((pCurStep->m_bVideoTestResult == TRUE) && (pCurStep->m_bAudioTestResult == FALSE))
		{

			if(CurrentSet->bSaveProcessingTimeData)
			{
				sString.Format("%d.SetRF is skipped",pCurStep->m_nStep);
				AddTimeData(sString);
			}

			sString.Format("%d.SetRF is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(sString);

			return TRUE;
		}

		//+add kwmoon 080201
		// In case of NG/V or NG/AV
		if(pCurStep->m_bVideoTestResult == FALSE)
		{
			//+add kwmoon 080229
			if((pCurStep->m_nTestType == RGB_TEST) || (pCurStep->m_nTestType == BRIGHT_TEST)
			|| (pCurStep->m_nTestType == NOISE_TEST))
			{
				// Spec Out
				if((fMesuredValue > -50.0) && (fMesuredValue < 50.0)) 
				{

					if(CurrentSet->bSaveProcessingTimeData)
					{
						sString.Format("   SetRF is skipped");
						AddTimeData(sString);
					}

					sString.Format("%d.SetRF is skipped",pCurStep->m_nStep);
					AddStringToRemoteComLog(sString);

					return TRUE;
				}
			}
		}
	}
	//-
			
	char szChannel[6];
	memset(szChannel,NULL,6);

	char szChannelDigit[5];
	memset(szChannelDigit,NULL,5);

	szChannelDigit[0] = '1';
	szChannelDigit[1] = '0';
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nChannel		 = GetInteger();
		nRemoteDelay	 = GetInteger();
		nRetryDelay		 = GetInteger();
		sMsg			 = GetString();
	}
	else 
	{
		return FALSE;
	}

	if(g_CurSetting.m_nNotSkipSameSourceChannel == 0)
	{
		if((!pCurStep->m_bStepRetryFlag) && (!pCurStep->m_bStepInternalRetryFlag)
		&& (g_CurSetting.IsSameChannel(nChannel)))
		{
			sString.Format("%d.SetRF is skipped",pCurStep->m_nStep);
			AddStringToRemoteComLog(sString);
			return TRUE;
		}
	}

	ShowSubStepMessage(sMsg, "Set Rf Channel");

	_itoa_s(nChannel,szChannel,10);

	if((nChannel > 0) && (nChannel < 10))
	{
		nNoDigit = 1;
	}
	else if((nChannel > 9) && (nChannel < 100))
	{
		nNoDigit = 2;
	}
	//+change kwmoon 080307
//	else if(nChannel >= 100)
	else if((nChannel > 99) && (nChannel < 1000))
	{
		nNoDigit = 3;
	}
	//+add kwmoon 080307
	else if((nChannel > 999) && (nChannel < 10000))
	{
		nNoDigit = 4;
	}
	//+add kwmoon 080307
	else if((nChannel > 9999) && (nChannel < 100000))
	{
		nNoDigit = 5;
	}
	else
	{
		sErrorMsg.Format("Wrong Channel No! (CH:%d)",nChannel);
		AfxMessageBox(sErrorMsg);
	}

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
					sString.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
					AfxMessageBox(sString);
					return FALSE;
				}
			}
		}

		//+add 080201 : Add if statement
		// Failed to change RF channel
		
		//+add kwmoon 080312
		if((pCurStep->m_nTestType == RGB_TEST) || (pCurStep->m_nTestType == BRIGHT_TEST)
		|| (pCurStep->m_nTestType == NOISE_TEST))
		{
			//+add kwmoon 080229
			if((fMesuredValue <= -50.0) || (fMesuredValue >= 50.0)) 
			{
				//+add kwmoon 071224
				if((pCurStep->m_bStepInternalRetryFlag) || (pCurStep->m_bStepRetryFlag))
				{
					if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "1a")) return FALSE;
					if(!_Wait(nRetryDelay)) return FALSE;

					if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")) return FALSE;
					if(!_Wait(nRemoteDelay)) return FALSE;

					if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")) return FALSE;
					if(!_Wait(nRemoteDelay)) return FALSE;
				}
			}
		}

		//+add kwmoon 080312
		if(pCurStep->m_nTestType == MOVING_PIC_TEST)
		{
			//+add kwmoon 071224
			if((pCurStep->m_bStepInternalRetryFlag) || (pCurStep->m_bStepRetryFlag))
			{
				if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "1a")) return FALSE;
				if(!_Wait(nRetryDelay)) return FALSE;

				if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")) return FALSE;
				if(!_Wait(nRemoteDelay)) return FALSE;

				if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")) return FALSE;
				if(!_Wait(nRemoteDelay)) return FALSE;
			}
		}

		for(int nTmp = 0; nTmp < nNoDigit; nTmp++)
		{
			szChannelDigit[1] = szChannel[nTmp];
			//+2007.12.26 Mod BY USY
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, szChannelDigit))
			{
				return FALSE;
			}
//			AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum(szChannelDigit));
			//-
			if(!_Wait(nRemoteDelay)) return FALSE;
		}
	}

	//+add kwmoon 071205
	g_CurSetting.m_nChannel = nChannel;
	//-

	//+add kwmoon 071206
	//+add kwmoon 071206
	sString.Format("%d.Set RF Channel %d",pCurStep->m_nStep,nChannel);
	AddStringToRemoteComLog(sString);

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   SetRF [%dms]",dwFuncElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}
	
	return TRUE;
}

BOOL _SetRfChannel()
{
	CString sMsg			= _T("");
	CString sRefFileName	= _T("");
	CString sErrorMsg		= _T("");
	
	int nChannel		 = 0;
	int nChannelKeyDelay = 0;
	int nRemoteDelay	 = 0;
	int nSendExit		 = 0;	// 1 : Send Exit Key 
//	int nClickNum		 = 1;
	int nNoDigit		 = 1;
	//+add kwmoon 071226
	int nRetryDelay		 = 0;

	char szChannel[4];
	memset(szChannel,NULL,4);

	char szChannelDigit[3];
	memset(szChannelDigit,NULL,3);

	szChannelDigit[0] = '1';
	szChannelDigit[1] = '0';
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nChannel		 = GetInteger();
		nChannelKeyDelay = GetInteger();
		nRemoteDelay	 = GetInteger();
		//+add kwmoon 071226
		nRetryDelay		 = GetInteger();
		nSendExit		 = GetInteger();
		sMsg			 = GetString();
	}
	else 
	{
		return FALSE;
	}

	if(g_CurSetting.m_nNotSkipSameSourceChannel == 0)
	{
		//+change kwmoon 071224
	//	if(g_CurSettingIsSameChannel(nChannel))
		if((!pCurStep->m_bStepRetryFlag) && (!pCurStep->m_bStepInternalRetryFlag)
      		&&  (g_CurSetting.IsSameChannel(nChannel)))
		{
			return TRUE;
		}
	}

	ShowSubStepMessage(sMsg, "Set Rf Channel");

	_itoa_s(nChannel,szChannel,10);

	if((nChannel > 0) && (nChannel < 10))
	{
		nNoDigit = 1;
	}
	else if((nChannel > 9) && (nChannel < 100))
	{
		nNoDigit = 2;
	}
	else if(nChannel >= 100)
	{
		nNoDigit = 3;
	}
	else
	{
		sErrorMsg.Format("Wrong Channel No! (CH:%d)",nChannel);
		AfxMessageBox(sErrorMsg);
	}

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

		//+add kwmoon 071224
		if((pCurStep->m_bStepInternalRetryFlag) || (pCurStep->m_bStepRetryFlag))
		{
			//+2007.12.26 Mod BY USY
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "1a"))
			{
				return FALSE;
			}
//			AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum("1a"));
			//-
			if(!_Wait(nRetryDelay)) return FALSE;

			//+2007.12.26 Mod BY USY
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b"))
			{
				return FALSE;
			}
//			AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum("5b"));
			//-
			if(!_Wait(nRemoteDelay)) return FALSE;

			//+2007.12.26 Mod BY USY
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b"))
			{
				return FALSE;
			}
//			AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum("5b"));
			//-

			//+change kwmoon 071226
			if(!_Wait(nRemoteDelay)) return FALSE;
		}

		for(int nTmp = 0; nTmp < nNoDigit; nTmp++)
		{
			szChannelDigit[1] = szChannel[nTmp];
			//+2007.12.26 Mod BY USY
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, szChannelDigit))
			{
				return FALSE;
			}
//			AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum(szChannelDigit));
			//-
			if(nTmp < (nNoDigit-1))
			{
				if(!_Wait(nRemoteDelay)) return FALSE;
			}
		}
		if(!_Wait(nChannelKeyDelay)) return FALSE;
		
	}

	if(CurrentSet->bUseAVSwitchBox)
	{
		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "44")) return FALSE;
		if(!_Wait(nRemoteDelay)) return FALSE;
	}

	if(nSendExit)
	{
		if(CurrentSet->bUseAVSwitchBox)
		{
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")) return FALSE;
			if(!_Wait(nRemoteDelay)) return FALSE;
		}
	}

	//+add kwmoon 071205
	g_CurSetting.m_nChannel = nChannel;
	//-

	//+add kwmoon 071206
	CString szTemp;
	szTemp.Format("Set RF Channel %d",nChannel);
	AddStringToRemoteComLog(szTemp);
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
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY, nAudioSource, 0))
		{
			return FALSE;
		}
	}

	return TRUE;
}
//+add kwmoon 080321
BOOL _SetToolOption()
{
	CString sMsg		  = _T("");
	CString szTemp		  = _T("");
	CString szOption	  = _T("");

	int nRemoteDelay	= 0;
	int nNoKeys			= 0;
	int nOptionNo		= 0;
	
	char szDigit[3];
	memset(szDigit,NULL,3);

	szDigit[0] = '1';
	szDigit[1] = '0';

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nOptionNo			= GetInteger();
		nRemoteDelay		= GetInteger();
		sMsg				= GetString();
	}
	else 
	{
		return FALSE;
	}

	szTemp.Format("Set Tool Option%d",nOptionNo);
	ShowSubStepMessage(sMsg,szTemp);

	// Tool Option
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

		switch(nOptionNo)
		{
			case 1 : szOption = CurrentSet->sToolOption1; break;
			case 2 : szOption = CurrentSet->sToolOption2; break;
			case 3 : szOption = CurrentSet->sToolOption3; break;
		
			default :
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Unsupported Tool-Option Number : %d",nOptionNo);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}

		if(szOption.Compare("NULL") == 0)
		{
			//+add kwmoon 080313
			CString szErrMsg;
			szErrMsg.Format("Tool-Option%d value is not specified!",nOptionNo);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}

		// Tool Option
		nNoKeys = szOption.GetLength();

		for(int i = 0; i < nNoKeys; i++)
		{
			szDigit[1] = szOption.GetAt(i);
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, szDigit)) return FALSE;
			if(!_Wait(nRemoteDelay)) return FALSE;
		}
	}

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   SetToolOption [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	return TRUE;
}

//+add kwmoon 080321
BOOL _SetAreaOption()
{
	CString sMsg		= _T("");
	CString szTemp		= _T("");
	CString szOption	= _T("");

	int nRemoteDelay	= 0;
	int nNoKeys			= 0;
	int nOptionNo		= 0;
	
	char szDigit[3];
	memset(szDigit,NULL,3);

	szDigit[0] = '1';
	szDigit[1] = '0';

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nOptionNo			= GetInteger();
		nRemoteDelay		= GetInteger();
		sMsg				= GetString();
	}
	else 
	{
		return FALSE;
	}

	szTemp.Format("Set Area Option%d",nOptionNo);
	ShowSubStepMessage(sMsg,szTemp);

	// Tool Option
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

		switch(nOptionNo)
		{
			case 1 : szOption = CurrentSet->sAreaOption1; break;
//			case 2 : szOption = CurrentSet->sAreaOption2; break;
		
			default :
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Unsupported Area-Option Number : %d",nOptionNo);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}


		if(szOption.Compare("NULL") == 0)
		{
			//+add kwmoon 080313
			CString szErrMsg;
			szErrMsg.Format("Area-Option%d value is not specified!",nOptionNo);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
		
		// Area Option
		nNoKeys = szOption.GetLength();

		for(int i = 0; i < nNoKeys; i++)
		{
			szDigit[1] = szOption.GetAt(i);
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, szDigit)) return FALSE;
			if(!_Wait(nRemoteDelay)) return FALSE;
		}
	}

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   SetAreaOption [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
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

	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0); _Wait(500);
	}
	
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}

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

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			if(!Grab_Image_UHD(_T(""),0))
			{
				_Wait(1000);
				break;
			}
		}
		else//FHD
		{
			if(!Grab_Image_FHD(_T(""),0))
			{
				_Wait(1000);
				break;
			}
		}

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMaxDelay) break;
	}	

	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
	}
	
	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   PowerOnCheck [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

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

	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0); _Wait(500);
	}
	
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}


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

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			if(!Grab_Image_UHD(_T(""),0))
			{
				break;
			}
		}
		else//FHD
		{
			if(!Grab_Image_FHD(_T(""),0))
			{
				break;
			}
		}

		g_pView->InvalidateRect(nImageRect, FALSE);
		g_pView->UpdateWindow(); _Wait(10);

		dwCurrentTick = GetTickCount();

		if((int)(dwCurrentTick-dwStartTick) > nMaxDelay) break;
	}	

	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
	}
	
	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   PowerOffCheck [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

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


//+add kwmoon 080329
BOOL _SendI2cCommand()
{
	int		nCmdType	 = 0;
	int		nDelay		 = 0;
	int		nData1		 = 0;
	int		nData2		 = 0;
	int		nData3		 = 0;
	int		nResult		 = TEST_PASS;
	int		nOptionValue = 0;

	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");
	CString szData1		= _T("");
	CString szData2		= _T("");
	CString szData3		= _T("");
	CString szErrMsg	= _T("");

	DWORD dwStartTick		  = 0;
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nCmdType	= GetInteger();
		nData1		= GetInteger();
		nData2		= GetInteger();
		nData3		= GetInteger();
		nDelay		= GetInteger();
		sMsg		= GetString();
	}
	
	I2cAdcCtrl.m_ctrlI2cAdcCtrl.ClearPort();
	
	if(I2cAdcCtrl.m_nI2cAdcSeqNo >= 0xff) I2cAdcCtrl.m_nI2cAdcSeqNo = 0;

	switch(nCmdType)
	{
		case I2C_TYPE :
		{
			I2cAdcCtrl.ClearI2cAdcData();

			if(nData1 == DDC_2AB)
			{
				ShowSubStepMessage("I2C) Set Type DDC-2AB", _T(""));
		
				if((nResult = I2cAdcCtrl.SetDDC2AB()) != TEST_PASS)
				{
					goto END_POSITION;
				}
			}
			else
			{
				ShowSubStepMessage("I2C) Set Type DDC-2B", _T(""));
			}
		}
		break;

		case I2C_CHANNEL :
		{
			szData1.Format("%02x",nData1);

			sTemp.Format("I2C) Set Channel(%s)",szData1);
			ShowSubStepMessage(sTemp, _T(""));

			if((nResult = I2cAdcCtrl.SetChannel(szData1)) != TEST_PASS)
			{
				goto END_POSITION;
			}
		}
		break;

		case I2C_CMD :
		{
			// I2C ADC Start Command : 0xf1
			//+del kwmoon 080923
		//	if(nData1 == 0xf12) 
			{
				CurrentSet->nAdcType = ADC_TYPE_I2C;
			}

			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);

			sTemp.Format("I2C) Send Command(%s)(%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			for(int j = 0; j < 5; j++)
			{
				nResult = I2cAdcCtrl.WakeUp();
				if(nResult == TEST_PASS) break;
				Sleep(1);
			}

//			if((nResult = I2cAdcCtrl.WakeUp()) != TEST_PASS)
//			{
//				goto END_POSITION;
//			}

			if((nResult = I2cAdcCtrl.SendCmd(szData1,"00",szData2)) != TEST_PASS)
			{
				goto END_POSITION;
			}

			_Wait(nDelay);
		}
		break;

		//+add kwmoon 080923
		case I2C_CHK_VER :
		{
			// I2C ADC Start Command : 0xf1
			//+del kwmoon 080923
		//	if(nData1 == 0xf12) 
			{
				CurrentSet->nAdcType = ADC_TYPE_I2C;
			}

			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);

			sTemp.Format("I2C) Send Command(%s)(%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			for(int j = 0; j < 5; j++)
			{
				nResult = I2cAdcCtrl.WakeUp();
				if(nResult == TEST_PASS) break;
				Sleep(1);
			}

//			if((nResult = I2cAdcCtrl.WakeUp()) != TEST_PASS)
//			{
//				goto END_POSITION;
//			}

			if((nResult = I2cAdcCtrl.SendCmd(szData1, "00", szData2)) != TEST_PASS)
			{
				goto END_POSITION;
			}

			_Wait(nDelay);
		}
		break;
		
		//+add kwmoon 080923
		case I2C_WRITE_TOOL_OPTION :
		{
			sTemp.Format("I2C) Tool Option%d Write",nData1);
			ShowSubStepMessage(sTemp, _T(""));

			if((nData1 != 1) && (nData1 != 2) && (nData1 != 3))
			{
				szErrMsg.LoadString(ERROR_TOOL_OPTION_NO);
				AfxMessageBox(szErrMsg); return TEST_FAIL;
			}
			
			switch(nData1)
			{
				case 1 :
				{
					if(CurrentSet->sToolOption1 == _T(""))
					{
						szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
						AfxMessageBox(szErrMsg); return TEST_FAIL;
					}					
					nOptionValue = atoi(CurrentSet->sToolOption1); 
				}
				break;

				case 2 :
				{
					if(CurrentSet->sToolOption2 == _T(""))
					{
						szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
						AfxMessageBox(szErrMsg); return TEST_FAIL;
					}					
					nOptionValue = atoi(CurrentSet->sToolOption2); 
				}
				break;

				case 3 :
				{
					if(CurrentSet->sToolOption3 == _T(""))
					{
						szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
						AfxMessageBox(szErrMsg); return TEST_FAIL;
					}					
					nOptionValue = atoi(CurrentSet->sToolOption3); 
				}
				break;

				case 4 :
				{
					if(CurrentSet->sToolOption4 == _T(""))
					{
						szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
						AfxMessageBox(szErrMsg); return TEST_FAIL;
					}					
					nOptionValue = atoi(CurrentSet->sToolOption4); 
				}
				break;

				case 5 :
				{
					if(CurrentSet->sToolOption5 == _T(""))
					{
						szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
						AfxMessageBox(szErrMsg); return TEST_FAIL;
					}					
					nOptionValue = atoi(CurrentSet->sToolOption5); 
				}
				break;

				case 6 :
				{
					if(CurrentSet->sToolOption6 == _T(""))
					{
						szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
						AfxMessageBox(szErrMsg); return TEST_FAIL;
					}					
					nOptionValue = atoi(CurrentSet->sToolOption6); 
				}
				break;

				case 7 :
				{
					if(CurrentSet->sToolOption7 == _T(""))
					{
						szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
						AfxMessageBox(szErrMsg); return TEST_FAIL;
					}					
					nOptionValue = atoi(CurrentSet->sToolOption7); 
				}
				break;
			}

			if((nResult = I2cAdcCtrl.WriteOption(TOOL_OPTION,nData1,nOptionValue)) != TEST_PASS)
			{
				goto END_POSITION;
			}
		}
		break;
		
		case I2C_READ :
		{
			//+add kwmoon 080408
			I2cAdcCtrl.m_nData1 = nData1; // Page Address
			I2cAdcCtrl.m_nData2 = nData2; // Data Address
			I2cAdcCtrl.m_nData3 = nData3; // Number of data to read

			//+add kwmoon 080408
			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);

			sTemp.Format("I2C) Read(Addr:%s,%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));


			//+change kwmoon 080826
		//	nResult = I2cAdcCtrl.ReadI2cData();
			if(CurrentSet->nAvrFW_Ver == 2){
				I2cAdcCtrl.ReadI2cNbyteData2(I2cAdcCtrl.m_nData3);
			}
			else{
				I2cAdcCtrl.ReadI2cData();
			}
			nResult = TEST_PASS;
		}
		break;

		case I2C_CHK_VALUE :
		{
			szData1.Format("%02x",nData1);

			sTemp.Format("I2C) Check Value(%s)",szData1);
			ShowSubStepMessage(sTemp, _T(""));

			sTemp.Format("Value:%s",szData1);
			ctrlStepTarget_A.SetWindowText(sTemp); 
			ctrlStepTarget_A.Invalidate();
			ctrlStepTarget_A.UpdateWindow();
			// nData1 : Check Value
			// nData2 : Number of check

			//+change kwmoon 080804
		//	int nNoCheck = nData2;
			int  nMaxCheckTime = nData2;

			//+add kwmoon 081013
			pCurStep->m_bAdcValueCheckStep = TRUE;

			dwStartTick = GetTickCount();
			
			if((nResult = I2cAdcCtrl.CheckI2cValue(nData1)) != TEST_PASS) 
			{
				//+change kwmoon 080804
			/*	for(int i=0; i<nNoCheck; i++)
				{
					if(!ReadI2cData()) return FALSE;

					if(CheckI2cValue(nData1)) goto END_POSITION;

					_Wait(10);
				}
			
				return FALSE;
			*/	
				while(TRUE)
				{
					if(CurrentSet->nAvrFW_Ver == 2){
						I2cAdcCtrl.ReadI2cNbyteData2(I2cAdcCtrl.m_nData3);
					}
					else{
						I2cAdcCtrl.ReadI2cData();
					}
				
					if((nResult = I2cAdcCtrl.CheckI2cValue(nData1)) == TEST_PASS)
					{
						break;
					}

					if(GetTickCount() - dwStartTick > (UINT)nMaxCheckTime)
					{
						nResult = ERROR_I2C_READ_TIMEOUT; break;
					}

					//+change kwmoon 080826
				//	_Wait(10);
					_Wait(100);
				}
			}
			//+del kwmoon 080710
		//	else
		//	{
		//		return TRUE;
		//	}
		}
		break;

		case I2C_CHK_MAXMIN :
		{
			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);

			sTemp.Format("I2C) Check Max/Min(Tgt:%s/Mrg:%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			sTemp.Format("Tgt:%s/Mrg:%s",szData1,szData2);
			ctrlStepTarget_A.SetWindowText(sTemp); 
			ctrlStepTarget_A.Invalidate();
			ctrlStepTarget_A.UpdateWindow();
			
			//+change kwmoon 080408
			int nTarget	 = nData1;
			int nMargin	 = nData2;
	
			//+change kwmoon 080804
		//	int nNoCheck = nData3;
			int  nMaxCheckTime = nData3;

			//+add kwmoon 081013
			pCurStep->m_bAdcValueCheckStep = TRUE;

			dwStartTick = GetTickCount();
			
			if((nResult = I2cAdcCtrl.CheckI2cMaxMin(nTarget,nMargin, 1)) != TEST_PASS)
			{
			/*	for(int i=0; i<nNoCheck; i++)
				{
				//	if(!I2cAdcCtrl.ReadI2cData()) return FALSE;

					if((nResult = I2cAdcCtrl.ReadI2cData()) != TEST_PASS)
					{
					//	return FALSE;
						return nResult;
					}

					if(I2cAdcCtrl.CheckI2cMaxMin(nTarget,nMargin, 1)) goto END_POSITION;

					_Wait(10);
				}
				return FALSE;
			*/
				while(TRUE)
				{
					if(CurrentSet->nAvrFW_Ver == 2){
						I2cAdcCtrl.ReadI2cNbyteData2(I2cAdcCtrl.m_nData3);
					}
					else{
						I2cAdcCtrl.ReadI2cData();
					}

					if((nResult = I2cAdcCtrl.CheckI2cMaxMin(nTarget,nMargin, 1)) == TEST_PASS)
					{
						break; 
					}

					if(GetTickCount() - dwStartTick > (UINT)nMaxCheckTime)
					{
						nResult = ERROR_I2C_READ_TIMEOUT; break;
					}

					_Wait(10);
				}

			}
			//+del kwmoon 080710
		//	else
		//	{
		//		return TRUE;
		//	}
		}
		break;

		case I2C_CHK_MAXMIN2 :
		{
			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);

			sTemp.Format("I2C) Check Max/Min(Tgt:%s/Mrg:%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			sTemp.Format("Tgt:%s/Mrg:%s",szData1,szData2);
			ctrlStepTarget_A.SetWindowText(sTemp); 
			ctrlStepTarget_A.Invalidate();
			ctrlStepTarget_A.UpdateWindow();
			
			int nTarget	 = nData1;
			int nMargin	 = nData2;
	
			int  nMaxCheckTime = nData3;

			pCurStep->m_bAdcValueCheckStep = TRUE;

			dwStartTick = GetTickCount();
			
			if((nResult = I2cAdcCtrl.CheckI2cMaxMin(nTarget,nMargin, 2)) != TEST_PASS)
			{
				while(TRUE)
				{
					if(CurrentSet->nAvrFW_Ver == 2){
						I2cAdcCtrl.ReadI2cNbyteData2(I2cAdcCtrl.m_nData3);
					}
					else{
						I2cAdcCtrl.ReadI2cData();
					}

					if((nResult = I2cAdcCtrl.CheckI2cMaxMin(nTarget,nMargin, 2)) == TEST_PASS)
					{
						break; 
					}

					if(GetTickCount() - dwStartTick > (UINT)nMaxCheckTime)
					{
						nResult = ERROR_I2C_READ_TIMEOUT; break;
					}

					_Wait(10);
				}

			}
		}
		break;

		case I2C_CHK_LIMIT :
		{
			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);

			sTemp.Format("I2C) Check Limit(Tgt:%s/Mrg:%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			//+change kwmoon 080408
			int nTarget	 = nData1;
			int nMargin	 = nData2;

			//+change kwmoon 080804
		//	int nNoCheck = nData3;
			int  nMaxCheckTime = nData3;

			//+add kwmoon 081013
			pCurStep->m_bAdcValueCheckStep = TRUE;

			dwStartTick = GetTickCount();
		
			if((nResult = I2cAdcCtrl.CheckI2cLimit(nTarget,nMargin, 1)) != TEST_PASS)
			{
				/*
				for(int i=0; i<nNoCheck; i++)
				{
					if(!I2cAdcCtrl.I2cAdcCtrl.eadI2cData()) return FALSE;

					if(I2cAdcCtrl.CheckI2cLimit(nTarget,nMargin, 1)) goto END_POSITION;

					_Wait(10);
				}
				return FALSE;
				*/
				while(TRUE)
				{
					if(CurrentSet->nAvrFW_Ver == 2){
						I2cAdcCtrl.ReadI2cNbyteData2(I2cAdcCtrl.m_nData3);
					}
					else{
						I2cAdcCtrl.ReadI2cData();
					}
				
					if((nResult = I2cAdcCtrl.CheckI2cLimit(nTarget,nMargin, 1)) == TEST_PASS)
					{
						break;
					}

					if(GetTickCount() - dwStartTick > (UINT)nMaxCheckTime)
					{
						nResult = ERROR_I2C_READ_TIMEOUT; break;
					}

					_Wait(10);
				}
			}
			//+del kwmoon 080710
		//	else
		//	{
		//		return TRUE;
		//	}
		}
		break;

		case I2C_CHK_LIMIT2 :
		{
			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);

			sTemp.Format("I2C) Check Limit(Tgt:%s/Mrg:%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			int nTarget	 = nData1;
			int nMargin	 = nData2;

			int  nMaxCheckTime = nData3;

			pCurStep->m_bAdcValueCheckStep = TRUE;

			dwStartTick = GetTickCount();
		
			if((nResult = I2cAdcCtrl.CheckI2cLimit(nTarget,nMargin, 2)) != TEST_PASS)
			{
				while(TRUE)
				{
					if(CurrentSet->nAvrFW_Ver == 2){
						I2cAdcCtrl.ReadI2cNbyteData2(I2cAdcCtrl.m_nData3);
					}
					else{
						I2cAdcCtrl.ReadI2cData();
					}
				
					if((nResult = I2cAdcCtrl.CheckI2cLimit(nTarget,nMargin, 2)) == TEST_PASS)
					{
						break;
					}

					if(GetTickCount() - dwStartTick > (UINT)nMaxCheckTime)
					{
						nResult = ERROR_I2C_READ_TIMEOUT; break;
					}

					_Wait(10);
				}
			}
		}
		break;
		
		//+add kwmoon 080625
		case I2C_2B_READ :
		{
			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);
	
			sTemp.Format("I2C-2B) Read(Addr:%s,Value:%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			//+add kwmoon 080408
			I2cAdcCtrl.m_nData1 = nData1;
			I2cAdcCtrl.m_nData2 = nData2;

			nResult = I2cAdcCtrl.ReadI2c2bData();
		}
		break;
		
		//+add kwmoon 080625
		case I2C_2B_WRITE :
		{
			szData1.Format("%02x",nData1);
			szData2.Format("%02x",nData2);
	
			sTemp.Format("I2C-2B) Write(Addr:%s,Value:%s)",szData1,szData2);
			ShowSubStepMessage(sTemp, _T(""));

			sTemp.Format("A:%s,V:%s",szData1,szData2);
			ctrlStepTarget_A.SetWindowText(sTemp); 
			ctrlStepTarget_A.Invalidate();
			ctrlStepTarget_A.UpdateWindow();

			//+add kwmoon 080408
			I2cAdcCtrl.m_nData1 = nData1;
			I2cAdcCtrl.m_nData2 = nData2;

			nResult = I2cAdcCtrl.WriteI2c2bData();
		}
		break;

		default : nResult = TEST_FAIL;//FALSE;
	}

END_POSITION :

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   SendI2cCommand [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	//+change kwmoon 080804
//	return TRUE;
	return nResult;
}

//+add PSH 080516

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


//+add PSH 080522
BOOL _TVLink_SetChannel_PAL()
{
	BOOL bReturn;
	int nRFSource;
	int nCH;
	int nWait;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nRFSource = GetInteger();
		nCH = GetInteger();
		nWait = GetInteger();
	}	

	bReturn = TVCommCtrl.TVLink_SetChannel_PAL(nRFSource, nCH, nWait);
	if(!bReturn)
	{
		_Wait(200);
		bReturn = TVCommCtrl.TVLink_SetChannel_PAL(nRFSource, nCH, nWait);
		if(!bReturn)
		{
			_Wait(500);
			bReturn = TVCommCtrl.TVLink_SetChannel_PAL(nRFSource, nCH, nWait);
		}
	}

	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   TvLinkSetChannelPAL [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	return bReturn;
}

BOOL _RS232C_TVControl_Command()
{
	BOOL bReturn;
	CString sCommand;
	int nWait;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncTickCount = GetTickCount();
	}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		sCommand	= GetString();
		nWait = GetInteger();
	}	

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				//+add kwmoon 080313
			//	szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
			//	AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}

	bReturn = TVCommCtrl.Send_TVControl_Command(sCommand,nWait);

	if(!bReturn)
	{
		_Wait(200);
		bReturn = TVCommCtrl.Send_TVControl_Command(sCommand,nWait);

		if(!bReturn)
		{
			TVCommCtrl.PortClose();
			_Wait(500);

			TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
			bReturn = TVCommCtrl.Send_TVControl_Command(sCommand,nWait);
		}
	}


	if(CurrentSet->bSaveProcessingTimeData)
	{
		DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

		DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("   Rs232cTvControl [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
		AddTimeData(szFuncElapsedTime);
	}

	return bReturn;
}

//+add kwmoon 080621
void Set_Grab_Image_Check_Roi(int nType)
{
	POINT ptRoi1Position;
	POINT ptRoi2Position;

	int nImageWidth;
	int nImageHeight;

	if(g_GrabSource == LVDS_GRABBER)
	{
		nImageWidth = CurrentSet->nLvdsWidth;
		nImageHeight     = CurrentSet->nLvdsHeight;
	}
	else{
		nImageWidth = CurrentSet->nAnalogWidth;
		nImageHeight     = CurrentSet->nAnalogHeight;
	}

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
#ifdef SM_MODIFY_CODE__
//2018.12.19 CYS
BOOL _ToolOption_Write()
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResult3 = TRUE;
	BOOL bResult4 = TRUE;
	BOOL bResult5 = TRUE;
	BOOL bResult6 = TRUE;
	BOOL bResult7 = TRUE;
	BOOL bResult8 = TRUE;
	BOOL bResult9 = TRUE;

	int  nI2cResult;
	int  i;
	int nToolOption;

	if(CurrentSet->sToolOption1 != ""){
		nToolOption = atoi(CurrentSet->sToolOption1);
		if(CurrentSet->nTVControlType){
			_Wait(500);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 1, nToolOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult1 = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult1 = TVCommCtrl.Write_Option(TOOL_OPTION1, nToolOption);
				if(bResult1) break;
				_Wait(200);
			}
		}
	}

	if(CurrentSet->sToolOption2 != ""){
		nToolOption = atoi(CurrentSet->sToolOption2);
		if(CurrentSet->nTVControlType){
			_Wait(500);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 2, nToolOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult2 = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult2 = TVCommCtrl.Write_Option(TOOL_OPTION2, nToolOption);
				if(bResult2) break;
				_Wait(200);
			}
		}
	}
	
	if(CurrentSet->sToolOption3 != ""){
		nToolOption = atoi(CurrentSet->sToolOption3);
		if(CurrentSet->nTVControlType){
			_Wait(500);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 3, nToolOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult3 = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult3 = TVCommCtrl.Write_Option(TOOL_OPTION3, nToolOption);
				if(bResult3) break;
				_Wait(200);
			}
		}
	}

	if(CurrentSet->sToolOption4 != ""){
		nToolOption = atoi(CurrentSet->sToolOption4);
		if(CurrentSet->nTVControlType){
			_Wait(500);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 4, nToolOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult4 = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult4 = TVCommCtrl.Write_Option(TOOL_OPTION4, nToolOption);
				if(bResult4) break;
				_Wait(200);
			}
		}
	}
	//091016
	if(CurrentSet->sToolOption5 != ""){
		nToolOption = atoi(CurrentSet->sToolOption5);
		if(CurrentSet->nTVControlType){
			_Wait(500);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 5, nToolOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult5 = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult5 = TVCommCtrl.Write_Option(TOOL_OPTION5, nToolOption);
				if(bResult5) break;
				_Wait(200);
			}
		}
	}
	//101011
	if(CurrentSet->sToolOption6 != ""){
		nToolOption = atoi(CurrentSet->sToolOption6);
		if(CurrentSet->nTVControlType){
			_Wait(500);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 6, nToolOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult6 = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult6 = TVCommCtrl.Write_Option(TOOL_OPTION6, nToolOption);
				if(bResult6) break;
				_Wait(200);
			}
		}
	}

	if(CurrentSet->sToolOption7 != ""){
		nToolOption = atoi(CurrentSet->sToolOption7);
		if(CurrentSet->nTVControlType){
			_Wait(500);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 7, nToolOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult7 = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult7 = TVCommCtrl.Write_Option(TOOL_OPTION7, nToolOption);
				if(bResult7) break;
				_Wait(200);
			}
		}
	}
	if(CurrentSet->sToolOption8 != ""){
		nToolOption = atoi(CurrentSet->sToolOption8);
		if(CurrentSet->nTVControlType){
			bResult8 = FALSE;
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult8 = TVCommCtrl.Write_Option(TOOL_OPTION8, nToolOption);
				if(bResult8) break;
				_Wait(200);
			}
		}
	}
	if (CurrentSet->sCommercialOption1 != "") {
		nToolOption = atoi(CurrentSet->sCommercialOption1);
		if (CurrentSet->nTVControlType) {
			bResult9 = FALSE;
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult9 = TVCommCtrl.Write_Option(COMMERCIAL_OPTION1, nToolOption);
				if (bResult9) break;
				_Wait(200);
			}
		}
	}
	

	
	if (bResult1 && bResult2 && bResult3  && bResult4  && bResult5  && bResult6  && bResult7 && bResult8 && bResult9)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}

}
#else
BOOL _ToolOption_Write()
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResult3 = TRUE;
	BOOL bResult4 = TRUE;
	BOOL bResult5 = TRUE;
	BOOL bResult6 = TRUE;
	BOOL bResult7 = TRUE;
	BOOL bResult8 = TRUE;

	int  nI2cResult;
	int  i;
	int nToolOption;

	if (CurrentSet->sToolOption1 != "") {
		nToolOption = atoi(CurrentSet->sToolOption1);
		if (CurrentSet->nTVControlType) {
			_Wait(500);
			for (i = 0; i < 5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 1, nToolOption);
				if (nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if (nI2cResult != TEST_PASS)
			{
				bResult1 = FALSE;
			}
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult1 = TVCommCtrl.Write_Option(TOOL_OPTION1, nToolOption);
				if (bResult1) break;
				_Wait(200);
			}
		}
	}

	if (CurrentSet->sToolOption2 != "") {
		nToolOption = atoi(CurrentSet->sToolOption2);
		if (CurrentSet->nTVControlType) {
			_Wait(500);
			for (i = 0; i < 5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 2, nToolOption);
				if (nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if (nI2cResult != TEST_PASS)
			{
				bResult2 = FALSE;
			}
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult2 = TVCommCtrl.Write_Option(TOOL_OPTION2, nToolOption);
				if (bResult2) break;
				_Wait(200);
			}
		}
	}

	if (CurrentSet->sToolOption3 != "") {
		nToolOption = atoi(CurrentSet->sToolOption3);
		if (CurrentSet->nTVControlType) {
			_Wait(500);
			for (i = 0; i < 5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 3, nToolOption);
				if (nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if (nI2cResult != TEST_PASS)
			{
				bResult3 = FALSE;
			}
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult3 = TVCommCtrl.Write_Option(TOOL_OPTION3, nToolOption);
				if (bResult3) break;
				_Wait(200);
			}
		}
	}

	if (CurrentSet->sToolOption4 != "") {
		nToolOption = atoi(CurrentSet->sToolOption4);
		if (CurrentSet->nTVControlType) {
			_Wait(500);
			for (i = 0; i < 5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 4, nToolOption);
				if (nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if (nI2cResult != TEST_PASS)
			{
				bResult4 = FALSE;
			}
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult4 = TVCommCtrl.Write_Option(TOOL_OPTION4, nToolOption);
				if (bResult4) break;
				_Wait(200);
			}
		}
	}
	//091016
	if (CurrentSet->sToolOption5 != "") {
		nToolOption = atoi(CurrentSet->sToolOption5);
		if (CurrentSet->nTVControlType) {
			_Wait(500);
			for (i = 0; i < 5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 5, nToolOption);
				if (nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if (nI2cResult != TEST_PASS)
			{
				bResult5 = FALSE;
			}
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult5 = TVCommCtrl.Write_Option(TOOL_OPTION5, nToolOption);
				if (bResult5) break;
				_Wait(200);
			}
		}
	}
	//101011
	if (CurrentSet->sToolOption6 != "") {
		nToolOption = atoi(CurrentSet->sToolOption6);
		if (CurrentSet->nTVControlType) {
			_Wait(500);
			for (i = 0; i < 5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 6, nToolOption);
				if (nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if (nI2cResult != TEST_PASS)
			{
				bResult6 = FALSE;
			}
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult6 = TVCommCtrl.Write_Option(TOOL_OPTION6, nToolOption);
				if (bResult6) break;
				_Wait(200);
			}
		}
	}

	if (CurrentSet->sToolOption7 != "") {
		nToolOption = atoi(CurrentSet->sToolOption7);
		if (CurrentSet->nTVControlType) {
			_Wait(500);
			for (i = 0; i < 5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(TOOL_OPTION, 7, nToolOption);
				if (nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if (nI2cResult != TEST_PASS)
			{
				bResult7 = FALSE;
			}
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult7 = TVCommCtrl.Write_Option(TOOL_OPTION7, nToolOption);
				if (bResult7) break;
				_Wait(200);
			}
		}
	}
	if (CurrentSet->sToolOption8 != "") {
		nToolOption = atoi(CurrentSet->sToolOption8);
		if (CurrentSet->nTVControlType) {
			bResult8 = FALSE;
		}
		else {
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for (i = 0; i < 3; i++)
			{
				bResult8 = TVCommCtrl.Write_Option(TOOL_OPTION8, nToolOption);
				if (bResult8) break;
				_Wait(200);
			}
		}
	}

	if (bResult1 && bResult2 && bResult3 && bResult4 && bResult5 && bResult6 && bResult7 && bResult8)
	{
		return TRUE;

	}
	else {
		return FALSE;
	}

}
#endif


BOOL _ToolOption_WriteAll()
{
	BOOL bResult = TRUE;
	int  nI2cResult;
	int  i;
	int nOption1 = 0 ;
	int nOption2 = 0;
	int nOption3 = 0;
	int nOption4 = 0;
	int nOption5 = 0;
	int nOption6 = 0;
	int nOption7 = 0;

	if(CurrentSet->sToolOption1 != ""){
		nOption1 = atoi(CurrentSet->sToolOption1);
	}

	if(CurrentSet->sToolOption2 != ""){
		nOption2 = atoi(CurrentSet->sToolOption2);
	}
	
	if(CurrentSet->sToolOption3 != ""){
		nOption3 = atoi(CurrentSet->sToolOption3);
	}
	if(CurrentSet->sToolOption4 != ""){
		nOption4 = atoi(CurrentSet->sToolOption4);
	}
	if(CurrentSet->sToolOption5 != ""){
		nOption5 = atoi(CurrentSet->sToolOption5);
	}
	if(CurrentSet->sToolOption6 != ""){
		nOption6 = atoi(CurrentSet->sToolOption6);
	}
	if(CurrentSet->sToolOption7 != ""){
		nOption7 = atoi(CurrentSet->sToolOption7);
	}

	if(CurrentSet->nTVControlType){
		for(i=0; i<5; i++)
		{
			nI2cResult = I2cAdcCtrl.WriteOptionAll(nOption1, nOption2, nOption3, nOption4, nOption5, nOption6);
			if(nI2cResult == TEST_PASS) break;
			_Wait(200);
		}
		if(nI2cResult != TEST_PASS)
		{
			bResult = FALSE;
		}
	}
	else{
		bResult = TVCommCtrl.Write_OptionAll(nOption1, nOption2, nOption3, nOption4, nOption5, nOption6, nOption7);
	}

	return bResult;	
}

//+Change PSH 081119
BOOL _AreaOption_Write()
{
	BOOL bResult = TRUE;
	int  nI2cResult;
	int  i;
	int nOption;

	if(CurrentSet->sAreaOption1 != ""){
		nOption = atoi(CurrentSet->sAreaOption1);
		if(CurrentSet->nTVControlType){
			_Wait(300);
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(AREA_OPTION, 1, nOption);
				if(nI2cResult == TEST_PASS) break;
				_Wait(200);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResult = FALSE;
			}
		}
		else{
			for(i=0; i<3; i++)
			{
				bResult = TVCommCtrl.Write_Option(AREA_OPTION1, nOption);
				if(bResult) break;
				_Wait(200);
			}
		}
	}

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}

//+add PSH 110610
BOOL _CommercialOption_Write()
{
	BOOL bResult = TRUE;
//	int  nI2cResult;
	int  i;
	int nOption;

	if(CurrentSet->sCommercialOption1 != ""){
		nOption = atoi(CurrentSet->sCommercialOption1);
		if(CurrentSet->nTVControlType){
			return FALSE;	
		}
		else{
			for(i=0; i<3; i++)
			{
				bResult = TVCommCtrl.Write_Option(COMMERCIAL_OPTION1, nOption);
				if(bResult) break;
				_Wait(200);
			}
		}
	}

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}

//+add PSH 090415
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

//+add PSH 110610
BOOL _TV_HeadPhone_On()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_HeadPhone_OnOff(TRUE, 3000);

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}

BOOL _TV_HeadPhone_Off()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_HeadPhone_OnOff(FALSE, 3000);

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
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;
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
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE) ;
	}
	
	return bReturn;
}
BOOL _HDMIGen_SetCEC()
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
		bReturn = HDMIGeneratorCtrl.SetCEC_OnOff(FALSE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetCEC_OnOff(TRUE) ;
	}

	return bReturn;
}

BOOL _HDMIGen_SetARC()
{
	BOOL bReturn;

	int nPort;
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
		nPort = GetInteger();
		nFlag = GetInteger();
	}	
	else 
	{
		return FALSE;
	}

	if(nPort < 0 || nPort > 5) return FALSE;

	if(nFlag == 0){
		bReturn = HDMIGeneratorCtrl.SetARC_OnOff(nPort, FALSE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetARC_OnOff(nPort, TRUE) ;
	}

	return bReturn;
}

BOOL _HDMIGen_SetMHL()
{
	BOOL bReturn;

	int nPort;
	int nFlag;

	if(!CurrentSet->bUseHDMIGen){ return TRUE;}

	if(CurrentSet->nHDMIGenType != 0){return FALSE;}
		
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
		nPort = GetInteger();
		nFlag = GetInteger();
	}	
	else 
	{
		return FALSE;
	}

	if(nPort < 0 || nPort > 5) return FALSE;
	if((nPort == 1) || (nPort == 2))return FALSE;

	if(nFlag == 0){
		bReturn = HDMIGeneratorCtrl.SetMHL_OnOff(nPort, FALSE) ;
	}
	else{
		bReturn = HDMIGeneratorCtrl.SetMHL_OnOff(nPort, TRUE) ;
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

	if(CurrentSet->nHDMIGenType == 0){return TRUE;}

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
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE);
		_Wait(500);
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE);
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

	if (HDMIGeneratorCtrl.m_bResetPatternModel == 1)//HDMIGeneratorCtrl
	{
		bReturn = HDMIGeneratorCtrl.SetModel_Pattern(nModel, nPattern);
		if (bReturn == TRUE)
		{
			HDMIGeneratorCtrl.m_bResetPatternModel = 0;
		}
	}
	else if(pCurStep->m_bStepRetryFlag || pCurStep->m_bStepInternalRetryFlag)
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
		else if ((HDMIGeneratorCtrl.m_nModelNo != nModel) && (HDMIGeneratorCtrl.m_nPatternNo != nPattern))
		{
			bReturn = HDMIGeneratorCtrl.SetModel_Pattern(nModel, nPattern); 
		}
	}
	return bReturn;
}

//add 090527
BOOL _Set_DimmingTest()
{
/*	int nCh;
	int nAdd;
	int nCmd;
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nCh = GetInteger();
		nAdd = GetInteger();
		nCmd = GetInteger();
	}
	else 
	{
		return FALSE;
	}

	g_pView->m_pDimmingTest->nChannel   = nCh;
	g_pView->m_pDimmingTest->nW_Add	   = nAdd;
	g_pView->m_pDimmingTest->nW_Command = nCmd;
*/
	return TRUE;
}

BOOL _DMM_SetMeasureChannel()
{
	int	 nChannel;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nChannel = GetInteger();
	}
	else 
		return FALSE;

	if(nChannel < 1) nChannel = 1;
	if(nChannel > 70) nChannel = 70;
	
	if(g_nRunningProcessNo == 1)
	{
		CurrentSet->nDmmMeasureCh_S = nChannel + 1000;
	}
	else if(g_nRunningProcessNo == 2)
	{
		CurrentSet->nDmmMeasureCh_S = nChannel + 2000;
	}
	else
	{
		CurrentSet->nDmmMeasureCh_S = nChannel + 3000;
	}

	return TRUE;
}
BOOL _DMM_SetMultiMeasureChannel()
{
	int	 nChannel_S;
	int	 nChannel_E;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nChannel_S = GetInteger();
		nChannel_E = GetInteger();
	}
	else 
		return FALSE;

	if(nChannel_S < 1) nChannel_S = 1;
	if(nChannel_S > 70) nChannel_S = 70;

	if(nChannel_E < 1) nChannel_E = 1;
	if(nChannel_E > 70) nChannel_E = 70;
	
	if(g_nRunningProcessNo == 1)
	{
		CurrentSet->nDmmMeasureCh_S = nChannel_S + 1000;
		CurrentSet->nDmmMeasureCh_E = nChannel_E + 1000;
	}
	else if(g_nRunningProcessNo == 2)
	{
		CurrentSet->nDmmMeasureCh_S = nChannel_S + 2000;
		CurrentSet->nDmmMeasureCh_E = nChannel_E + 2000;
	}
	else
	{
		CurrentSet->nDmmMeasureCh_S = nChannel_S + 3000;
		CurrentSet->nDmmMeasureCh_E = nChannel_E + 3000;
	}

	return TRUE;
}

BOOL _VF_SetMeasureChannel()
{
	int	 nCh;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nCh = GetInteger();
	}
	else 
		return FALSE;

	if((nCh < 1) || (nCh > 42) || (nCh == 10) || (nCh==20) || (nCh== 30)){
		return FALSE;
	}
			
	
	gVF1000Ctrl.m_nChNo = nCh;

	return TRUE;
}	

BOOL _VF_SetMeasure_MinMax()
{
	int	 nCh;
	double dSpec1;
	double dSpec2;
	int	nWaitTime;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nCh = GetInteger();
		nWaitTime = GetInteger();
		dSpec1 = GetDouble();
		dSpec2 = GetDouble();
	}
	else 
		return FALSE;

	if((nCh < 1) || (nCh > 42) || (nCh == 10) || (nCh==20) || (nCh== 30)){
		return FALSE;
	}
			
	
	gVF1000Ctrl.m_nChNo = nCh;
	gVF1000Ctrl.m_nCheckTime = nWaitTime;
	gVF1000Ctrl.m_dSpec1 = dSpec1;
	gVF1000Ctrl.m_dSpec2 = dSpec2;


	return TRUE;
}

//+add PSH 101108
BOOL _TV_LNB_On()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_LNB_OnOff(TRUE, 3000);

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}

BOOL _TV_LNB_Off()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_LNB_OnOff(FALSE, 3000);

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}
//-
//+add PSH 101124
BOOL _TV_3DMode_On()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_3DMode_OnOff(TRUE, 3000);

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}

BOOL _TV_3DMode_Off()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Set_3DMode_OnOff(FALSE, 3000);

	if(bResult)
	{
		return TRUE;	

	}
	else{
		return FALSE;	
	}
}
//-
BOOL _AudioMeasureReset()
{
	g_pView->AudioMeasureStop();
	//_Wait(150);
	g_pView->AudioMeasureStart();

	return TRUE;
}

BOOL _Set_MacAddress()
{
	CString  sAdd;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		sAdd = GetString();

	//	TVCommCtrl.m_aMacAdd.Add(sAdd);
		TVCommCtrl.Set_MAC_Address(sAdd); 
	}
	else
		return FALSE;

	return TRUE;
}
BOOL _Set_USBHighSpeed()
{
	int  nUSB;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nUSB = GetInteger();

		TVCommCtrl.USB_HighSpeedCheck((UINT)nUSB); 
	}
	else
		return FALSE;

	return TRUE;
}

BOOL _Set_CountryGroup()
{
	int  nGroup;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nGroup = GetInteger();

		CurrentSet->nCountryGroup = (UINT)nGroup; 
	}
	else
		return FALSE;

	return TRUE;
}

BOOL _AC_Power_OnOff()
{
	int nFlag ;
	BOOL bFlag ;
	int nRet;
	BOOL bRet;

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
		bFlag = FALSE;
	}
	else{
		bFlag = TRUE;
	}

//AUTO
	if(g_nSysType == AUTO_SYS){
		bRet = gJigCtrl.Set_ACPower(bFlag);
		SystemMonitorLog_Save("Set_ACPower(bFlag);", _T(__FILE__), __LINE__);

		return bRet;
	}
	else{
		nRet = g_pView->Set_Relay(bFlag);

		if(nRet <= 0)
		{
			return FALSE;
		}
		else{
			return TRUE;
		}
	}
}

BOOL _MHL_BufferControl()
{
	int nFlag ;
	BOOL bFlag ;
	int nRet;

	if(CurrentSet->nHDMIGenType == 0){return TRUE;}

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
		bFlag = FALSE;
		HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;
		Sleep(100);
		HDMIGeneratorCtrl.SetCEC_OnOff(FALSE);
	}
	else{
		bFlag = TRUE;
		HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;
		Sleep(100);
		HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);
	}

	
	nRet = g_pView->Set_Mhl(1, bFlag);

	if(nRet <= 0)
	{
		return FALSE;
	}
	else{
		return TRUE;
	}

}

BOOL _MHL_DetectLineControl()
{
	int nFlag ;
	BOOL bFlag ;
	int nRet;

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
		bFlag = FALSE;
	}
	else{
		bFlag = TRUE;
	}

	
	nRet = g_pView->Set_Mhl(2, bFlag);

	if(nRet <= 0)
	{
		return FALSE;
	}
	else{
		return TRUE;
	}

}
BOOL _SetDetectCheckSource()
{
	int	 nChannel;
	int	 nMode;

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos!= NULL)
	{
		nChannel = GetInteger();
		nMode = GetInteger();
	}
	else 
		return FALSE;

//	if(nChannel < 0) nChannel = 0;
//	if(nChannel > JACK_SMHL) nChannel = 13;
	
	TVCommCtrl.m_nInputDetectItem = nChannel;
	TVCommCtrl.m_nDetectMode = nMode;

	return TRUE;
}

BOOL _Grabber_Reset()
{
	if(g_nGrabberType == FHD_GRABBER){ return TRUE;}
	
	g_pView->StopLVDSGrabThread();
	Sleep(100);
	if (CurrentSet->bGrabBaseReset == 1)
	{
		g_pView->GrabBaseReset();
	}

	g_pView->StartLVDSGrabThread();

	return TRUE;
}


BOOL _Grabber_HW_Reset()//GRABBER_HW_TEST
{
	static int sWhiteCount = 0;
	BOOL bResult = FALSE;
	g_pView->m_WhiteResetFlag = 1;
	if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
	{
		if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_TEST_RUN)
		{

			return 1;
		}
			   
		if (g_pView->m_bMakeReferenceMode)
		{
			
			return 1;
		}
		else
		{
			bResult = Grab_Image_UHD("", 0);// bResult = Grab_Image_Check_Normal_2in1();
		}

		int nResult = g_ImageProc.WhitenessFullTest(g_GrabImage);

		
		if (nResult == TEST_FAIL)//No WhiteScreen
		{
			g_pView->m_WhiteResetCount = 0;
			sWhiteCount = 0;
			return TRUE;
		}
		AddStringToStatus("Grab HW Test Fail");
		if (g_pView->CheckGrabberStatus())
		{
			AddStringToStatus("Grabber Update PASS!");
			sWhiteCount = 0;
			return TRUE;
		}
		//{
		//	gJigCtrl.Set_Gender_Reset(0);
		//	AddStringToStatus("Gender_Reset!!");
		//	Sleep(1000);

		for (int i = 0; i < 2; i++)
		{
//				g_pView->GrabBaseResetStartThread();
			gJigCtrl.Set_Gender_Reset(0);
			AddStringToStatus("Gender_Reset!!");
			Sleep(700);
			if (g_pView->Grab_UHD())
			{
				nResult = g_ImageProc.WhitenessFullTest(g_GrabImage);
				if (nResult == TEST_FAIL)//No WhiteScreen
				{
					g_pView->m_WhiteResetCount = 0;
					sWhiteCount = 0;
					return 1;
				}
				AddStringToStatus("Grab HW Test Fail");
			}
		}

		AddStringToStatus("Grabber Power OFF");
		gJigCtrl.Set_Grabber_Power(0);
		Sleep(700);

		AddStringToStatus("Grabber Power ON");
		gJigCtrl.Set_Grabber_Power(1);
		Sleep(700);

		g_pView->ResetGrabStartThread();
		Sleep(1000);
		if (g_pView->Grab_UHD())
		{
			nResult = g_ImageProc.WhitenessFullTest(g_GrabImage);
			if (nResult == TEST_FAIL)//No WhiteScreen
			{
				g_pView->m_WhiteResetCount = 0;
				sWhiteCount = 0;
				return 1;
			}
			AddStringToStatus("Grab HW Test Fail");
		}
		if (g_pView->CheckGrabberStatus())
		{
			AddStringToStatus("Grabber Update PASS!");
			sWhiteCount = 0;
			return TRUE;
		}
		//gender Reset(2ȸ)->Grabber Off / On->Fixture Up / Donw->AC on->Grabber SW Rest
		gJigCtrl.Set_CylinderUP();
		AddStringToStatus("Jig Up!");
		Sleep(1000);
		for (int i = 0; i < 10; i++)
		{
			if (gJigCtrl.m_bJigUpStatus)
				break;
			Sleep(500);
		}
		gJigCtrl.Set_CylinderDOWN();
		AddStringToStatus("Jig Down!");
		Sleep(1000);
		for (int i = 0; i < 10; i++)
		{
			if (gJigCtrl.m_bJigDownStatus)
				break;
			Sleep(500);
		}
		if (gJigCtrl.m_bJigDownStatus == 0)
		{
			AddStringToStatus("Jig Up Down Fail!");
			
			return 0;
		}
		gJigCtrl.Set_ACPower(1);
		AddStringToStatus("AC ON!");
		g_pView->ResetGrabStartThread();
		AddStringToStatus("Grab Reset!");
		Sleep(1000);
		if (g_pView->Grab_UHD())
		{
			nResult = g_ImageProc.WhitenessFullTest(g_GrabImage);
			if (nResult == TEST_FAIL)//No WhiteScreen
			{
				g_pView->m_WhiteResetCount = 0;
				sWhiteCount = 0;
				return 1;
			}
			AddStringToStatus("Grab HW Test Fail");
		}
		if (g_pView->CheckGrabberStatus())
		{
			AddStringToStatus("Grabber Update PASS!");
			sWhiteCount = 0;
			return TRUE;
		}
		sWhiteCount++;
		//if (sWhiteCount >= 3)
		//{
		//	gPLC_Ctrl.Command3TimesError();			
		//	AfxMessageBox("Grab HW Test Fail !!");
		//	gPLC_Ctrl.Command3TimesErrorClear();
		//	sWhiteCount = 0;
		//	return 1;
		//	int lCount = CurrentSet->nNoRetry+2;//(CurrentSet->nNoRetry > 0)
		//	if (sWhiteCount >= lCount*3)
		//	{
		//		gPLC_Ctrl.Command3TimesError();			
		//		AfxMessageBox("Grab HW Test Fail !!");
		//		gPLC_Ctrl.Command3TimesErrorClear();
		//		sWhiteCount = 0;
		//		return 1;
		//	}
		//}
	}
	else
	{
		g_pView->m_WhiteResetCount = 0;
		sWhiteCount = 0;
		return 1;
	}
	g_pView->m_WhiteResetCount = sWhiteCount;
	return bResult;
}

BOOL _AVC_BufferClear()
{
	BOOL bReturn = FALSE;

	bReturn = gAvConvertorCtrl.CheckBufferClear();

	return bReturn;
}


BOOL _Set_HdmiGrab()
{
	int nInput	= 0;
	int nResolution	= 0;
	int nBitShift	= 0;

	CString sMsg = _T("");

	BOOL bReturn = FALSE;

	if(g_nGrabberType == FHD_GRABBER){ return TRUE;}
	
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();

	if (pos!= NULL)
	{
		nInput		= GetInteger();
		nResolution = GetInteger();
		nBitShift = GetInteger();
	}
	else return FALSE;
	

	ShowSubStepMessage(sMsg, "Set AvConvertor");
	
	if(gAvConvertorCtrl.m_bPortOpen == FALSE){return FALSE;}

	if(!gAvConvertorCtrl.SetInputSignal(nInput))
	{
		CString szErrMsg;
		szErrMsg.Format("Failed to send AvConvertor command");
		AfxMessageBox(szErrMsg);
		return FALSE;
	}
	g_GrabSource = HDMI_GRABBER;
	CurrentSet->nHDMI_Grab_Resolution = nResolution;
	CurrentSet->nHDMI_Grab_BitShift = nBitShift;

	switch(CurrentSet->nHDMI_Grab_Resolution) 
	{
		case 0:  CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
		case 1:  CurrentSet->nHdmiWidth = 1366; CurrentSet->nHdmiHeight = 768; break;
		case 2:  CurrentSet->nHdmiWidth = 1280; CurrentSet->nHdmiHeight = 1024; break;
		case 3:  CurrentSet->nHdmiWidth = 1920; CurrentSet->nHdmiHeight = 1080; break;
		default: CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
	}
	_Grabber_Reset();

	return TRUE;
}

extern CEdit ctrlWipIdEdit;				//Wip ID
BOOL _Read_KeyItem()
{
	int nRev = 0;
	int nKeyItem = 0;
	int nKeySerialItem = 0;
	CString sTmp;
	int nKeyItem_GMES = 0;

	CString sSerialTmp;
	CString sModelnameTmp;


	//#ifdef DEBUG_MD5_CODE__
	//	if ((g_nGrabberType == UHD_GRABBER) && (CurrentSet->bUseScanner == FALSE))
	//#else
	////	if ((g_nGrabberType == UHD_GRABBER) && (g_nSysType == AUTO_SYS) && ((CurrentSet->bUseScanner == FALSE) || (CurrentSet->bUsePLCRobot == TRUE)))
	//	if ((g_nGrabberType == UHD_GRABBER) && ((CurrentSet->bUseScanner == FALSE) || (CurrentSet->bUsePLCRobot == TRUE)))
	//#endif

#if 0
	if (g_nGrabberType == UHD_GRABBER)
	{
		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
		if (nRev == 2)
		{
			sSerialTmp = sSerialTmp.Left(10);
			if (m_strWipId.Find("NO_SCAN") >= 0)
			{

				m_strWipId = sSerialTmp;
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);

			}
			sTmp.Format("PCBA ID Read : ");
			sTmp += sSerialTmp;
			AddStringToStatus(sTmp);

		}
		else if (nRev == 1)
		{

			CurrentSet->bSystemBreak = TRUE;
			sTmp.Format("No Ack PCBA ID Read");
			AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
			return FALSE;
		}
		else
		{
			CurrentSet->bSystemBreak = TRUE;
			sTmp.Format("No Key D/L history");
			AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
			return FALSE;
		}
	}
#endif

	if (!CurrentSet->bDrmKey) {
		CurrentSet->nDrmKeyVal = 0x01;
		return TRUE;
	}


	nRev = TVCommCtrl.ModelSNo_Read(nKeyItem, TRUE);
	if (nRev != 1) {
		nRev = TVCommCtrl.ModelSNo_Read(nKeyItem, FALSE);
	}


	if (nRev == 2) {
		CurrentSet->bSystemBreak = TRUE;
		sTmp.Format("%s : No Key D/L history", m_strWipId);
		AddStringToStatus(sTmp); //AfxMessageBox(sTmp);

		return FALSE;
	}
	else if (nRev == 0) {
		return FALSE;
	}

	CurrentSet->nDrmKeyVal = nKeyItem;
	nKeyItem_GMES = CheckKeyGmesVal();

	nKeyItem_GMES = nKeyItem_GMES & (~0x800); // ATSC 3.0 Bit MASK 
	nKeyItem_GMES = nKeyItem_GMES & (~0x2000); // CI+ECP Bit MASK 

	if (CurrentSet->nDrmKeyVal != nKeyItem_GMES)
	{
		sTmp.Format("DrmKeyVal = %03x, KeyItem_GMES = %03x", CurrentSet->nDrmKeyVal, nKeyItem_GMES);
		AddStringToStatus(sTmp);
		return FALSE;
	}

	return TRUE;

}
BOOL _Epi_Pack_Reset()
{
	g_pView->IF_Pack_Reset();
	return TRUE;
}

BOOL _RotateDisplay()
{
	int nRev = 0;
	int nKeyItem = 0;
	int nKeySerialItem = 0;
	CString sTmp;
	int nKeyItem_GMES = 0;

	CString sSerialTmp;
	CString sModelnameTmp;
	g_pView->PostMessage(WM_COMMAND, ID_IMG_ROTATE);


	return TRUE;

}

BOOL _SetTVBuadRate()
{
	UINT nBaud = 0;
	CString szErrMsg;
	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos != NULL)
	{
		nBaud = GetInteger();
	}
	CurrentSet->wTVBaudRate = nBaud;
	if (TVCommCtrl.m_bPortOpen == TRUE)
	{
		TVCommCtrl.PortClose();
		Sleep(500);
	}	
	
	if (TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
	{
		TVCommCtrl.PortClose();
		if (TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			szErrMsg.Format("Failed to Change Baud Rate(%d) COM port (%s)", CurrentSet->wTVBaudRate, CurrentSet->sTVComPort);
			AddStringToStatus(szErrMsg); //AfxMessageBox(szErrMsg);
			return 0.0;
		}
	}
	

	return TRUE;

}
BOOL _Read_PCBAID_READ()
{
	int nRev = 0;
	int nKeyItem = 0;
	int nKeySerialItem = 0;
	CString sTmp;
	int nKeyItem_GMES = 0;

	CString sSerialTmp;
	CString sModelnameTmp;


	//#ifdef DEBUG_MD5_CODE__
	//	if ((g_nGrabberType == UHD_GRABBER) && (CurrentSet->bUseScanner == FALSE))
	//#else
	////	if ((g_nGrabberType == UHD_GRABBER) && (g_nSysType == AUTO_SYS) && ((CurrentSet->bUseScanner == FALSE) || (CurrentSet->bUsePLCRobot == TRUE)))
	//	if ((g_nGrabberType == UHD_GRABBER) && ((CurrentSet->bUseScanner == FALSE) || (CurrentSet->bUsePLCRobot == TRUE)))
	//#endif	
	if (g_nGrabberType == UHD_GRABBER)
	{
		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
		if (nRev == 2)
		{
			sSerialTmp = sSerialTmp.Left(10);
			if (m_strWipId.Find("NO_SCAN") >= 0)
			{

				m_strWipId = sSerialTmp;
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);

				//if (!FileExists(szTemp1)) CreateFullPath(szTemp1);

				//// change 090410
				//if (CurrentSet->bSaveProcessingTimeData)
				//{
				//	if (!CreateTimeDataFile(sDate, sTime))
				//	{
				//		AfxMessageBox("Failed to creat time data file!");
				//	}
				//	dwFuncTickCount = GetTickCount();
				//}


			}
			sTmp.Format("PCBA ID Read : ");
			sTmp += sSerialTmp;
			AddStringToStatus(sTmp);
			return TRUE;

		}
		else 	
		{
			CurrentSet->bSystemBreak = TRUE;
			sTmp.Format("PCBA ID Read :FAIL");
			AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
			return FALSE;
		}
	}


	return FALSE;

}


BOOL _Set_CylinderComp()
{
	int nFlag ;
	BOOL bRet;

	if(g_nSysType != AUTO_SYS){return FALSE;}

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
		bRet = gJigCtrl.Set_CylinderComp(FALSE);
	}
	else{
		bRet = gJigCtrl.Set_CylinderComp(TRUE);
	}

	
	return bRet;

}

BOOL _Set_CylinderEarphone()
{
	int nFlag ;
	BOOL bRet;

	if(g_nSysType != AUTO_SYS){return FALSE;}

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
		bRet = gJigCtrl.Set_CylinderEarphone(FALSE);
	}
	else{
		bRet = gJigCtrl.Set_CylinderEarphone(TRUE);
	}

	
	return bRet;

}
BOOL _Set_Cylinder()
{
	int nFlag1 ;
	int nFlag2 ;
	BOOL bRet = FALSE;
	BOOL bEarPhone = FALSE;
	BOOL bComp = FALSE;

	if(g_nSysType != AUTO_SYS){return FALSE;}

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	
	if (pos!= NULL)
	{
		nFlag1 = GetInteger();
		nFlag2 = GetInteger();
	}
	else 
	{
		return FALSE;
	}

	if(nFlag1 == 0){
		bEarPhone = FALSE;
	}
	else{
		bEarPhone = TRUE;
	}

	if(nFlag2 == 0){
		bComp = FALSE;
	}
	else{
		bComp = TRUE;
	}

	bRet = gJigCtrl.Set_Cylinder(bEarPhone, bComp);
	
	return bRet;
}
BOOL _IsFailePowerOn()
{
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag)){
		AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "C4");
		_Wait(1500);

	}
	return TRUE;
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

BOOL _VF_MeasureStart()
{
	if((pCurStep->m_bStepRetryFlag) || (pCurStep->m_bStepInternalRetryFlag)){
		return TRUE;
	}

	g_pView->StartVFTestThread();

	return TRUE;

}

BOOL _Mnt_Edid_Source()
{
	

	UINT nDelay = 0;

	CString szMsg = _T("");
	CString szUserMsg = _T("");

	pos = pCurFunc->m_ArgumentList.GetHeadPosition();
	if (pos != NULL)
	{
		szMsg = GetString();
		
		CurrentSet->sMNT_Edid_Source = szMsg;
		
	}

	if (!_Wait(nDelay)) return FALSE;

	return TRUE;
}