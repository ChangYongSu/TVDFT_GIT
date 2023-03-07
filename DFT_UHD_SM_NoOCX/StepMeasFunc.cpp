#include "stdafx.h"
#include "resource.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "TextOutCtrl.h"
#include "BaseImage.h"
#include "ImageProc.h"
#include "TVCommCtrl.h"
#include "ProgressCtrlX.h"
#include "ManualTestResultInputDlg.h"
#include <malloc.h>
#include "AvSwitchBoxCtrl.h"
//+add kwmoon 080514
#include "SoundCard.h"
//+add kwmoon 080625
#include "I2cAdcCtrl.h"

#include <winsock.h>

#include "LnbDetectorCtl.h"
#include "AvConvertorCtrl.h"
#include "VF1000.h"
#include "VFDischargeDlg.h"
#include "JigCtl.h"

// Extern
extern CDATsysView*		g_pView;
extern CBaseImage		g_RefImage;
extern CBaseImage		g_GrabImage;
extern CBaseImage		g_MaskImage;
extern CImageProc		g_ImageProc;

extern CAvSwitchBox		AvSwitchBoxCtrl;
extern CProgressCtrlX	ctrlImgProg;
//extern BYTE				GrabData[2560][1080][3];
extern CTVCommCtrl		TVCommCtrl;
extern _InputSource		InputDefined_FHD[];
extern _InputSource		InputDefined_UHD[];

//+add kwmoon 080625
extern CI2cAdcCtrl		I2cAdcCtrl;

//+add kwmoon 080514
extern CSoundCard g_SoundCard;
extern int g_AutoControlProcess;
extern CJigCtrl gJigCtrl;

/////////////////////////////
// Image Processing Function
/////////////////////////////


double	_Color_Test()
{
	int nResult = TEST_FAIL;
	BOOL bGrabReault = FALSE;
	int nTotalNoErrorPixel	= 0;

	POINT ptResultPosition;
	ptResultPosition.x		= 0;
	ptResultPosition.y		= 0;

	CString sMsg 			= _T("");
	CString szMsg1			= _T("");
	CString szMsg2			= _T("");
	CString szPrevMsg		= _T("");

	float fMaxDifference	= 0.0;

	int   nColorDepth		= 0;
	int   nNoLevel			= 0;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");
	SystemMonitorLog_Save("_Color_Test");// , _T(__FILE__), __LINE__);

	

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-ColorTest");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
	}

	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	
	//if (g_pView->CheckGrabberStatus())
	//{
	//	AddStringToStatus("Grabber Update PASS!");
	//	//sWhiteCount = 0;		
	//}
	//else
	//{
	//	AddStringToStatus("Grabber Update FAIL!");
	//	pCurStep->m_bResult = FALSE;
	//	return 100;
	//}


	//+add kwmoon 080819
	g_ImageProc.m_ptPositionShift.x = 0;
	g_ImageProc.m_ptPositionShift.y = 0;

	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);
	g_ImageProc.SetAvgMaskSize(0,0,0); 
	SystemMonitorLog_Save("_Color_Test1"); 
	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		nResult = g_ImageProc.RGBTest(NORMAL_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
									  ptResultPosition, fMaxDifference,nTotalNoErrorPixel,g_MaskImage.m_bImageLoaded,ctrlImgProg);
	
		SystemMonitorLog_Save("_Color_Test2");
		if(nResult == TEST_FAIL)
		{
			CRect nImageRect;
			CWnd* pImageWnd;

			pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
			pImageWnd->GetWindowRect(&nImageRect);
			
			//UHD
			if(g_nGrabberType == UHD_GRABBER)
			{
				bGrabReault = Grab_Image_UHD(_T(""),0);
			}
			else//FHD
			{
				bGrabReault = Grab_Image_FHD(_T(""),0);
			}

			if(!bGrabReault)
			{
				nResult =  TEST_FAIL;
			}
			else
			{
				g_pView->InvalidateRect(nImageRect, FALSE);
				g_pView->UpdateWindow(); _Wait(10);
			
				nResult = g_ImageProc.RGBTest(NORMAL_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
									  ptResultPosition, fMaxDifference,nTotalNoErrorPixel,g_MaskImage.m_bImageLoaded,ctrlImgProg);
			}
		}
		SystemMonitorLog_Save("_Color_Test3");
		pCurStep->m_nTestType = RGB_TEST;
		pCurStep->m_nTotalNoErrorPixel = nTotalNoErrorPixel;
	
		nColorDepth	 = g_RefImage.m_InfoHeader.biBitCount;

		//+change kwmoon 080722
//		nNoLevel	 = (int)pow(2,(nColorDepth/3));		//+add kwmoon 080716
		nNoLevel	 = (int)pow(2.0,(nColorDepth/3))-1;		//+add kwmoon 080716
		
		//+add kwmoon 080716
		switch(nResult)
		{
			case TEST_PASS  : pCurStep->m_bResult = TRUE;  break;
			case TEST_FAIL  : 
			{
			//	gJigCtrl.Set_Gender_Reset(0);

				pCurStep->m_bResult = FALSE;
				break;
			}
			case TEST_ABORT : 
			{
				pCurStep->m_bResult = FALSE; break;
				pCurStep->m_nTotalNoErrorPixel = -1;

				AfxMessageBox("Failed to run color test");
				return MAX_COLOR_LEVEL;
			}
		}

		g_GrabImage.m_bSaveProcessedImage = TRUE; 

		SystemMonitorLog_Save("_Color_Test4");
		if((CurrentSet->bSaveGrabImg)
		|| (!CurrentSet->bSaveGrabImg && (nResult == TEST_FAIL)))
		{
			//=================
			// Save Grab Image 
			//=================
			if(pCurStep->m_strGrabPath == _T(""))
			{
				pCurStep->m_strGrabPath = g_GrabImage.m_szFilePath;
			}
		//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
			//	g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
				g_GrabImage.SaveJpgImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
			}
		}
		SystemMonitorLog_Save("_Color_Test5");
		//================
		// Save Test Data 
		//================
		if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData)
		{
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++)
				{
					pCurStep->m_aNoDifferentPoint[i][j] = g_ImageProc.m_aNoDifferentPoint[i][j];
				}
			}
			pCurStep->m_nSecondDifferentPointFreePercent = g_ImageProc.m_nSecondDifferentPointFreePercent;
		}

		for(int i=0; i<5; i++)
		{
			pCurStep->m_nNoBandErrorPixel[i]				= g_ImageProc.m_nNoBandErrorPixel[i];
			pCurStep->m_nNoAllowedBandErrorPixel[i]			= g_ImageProc.m_nNoAllowedBandErrorPixel[i];
			pCurStep->m_fBandErrorPixelPercentage[i]		= g_ImageProc.m_fBandErrorPixelPercentage[i];
			pCurStep->m_fAllowedBandErrorPixelPercentage[i]	= g_ImageProc.m_fAllowedBandErrorPixelPercentage[i];
		}
		pCurStep->m_fGoodPixelPercentage = g_ImageProc.m_fGoodPixelPercentage;

		pCurStep->m_nNoRoiPixel		= g_ImageProc.m_nNoRoiPixel;
		pCurStep->m_nNoTargetPixel	= g_ImageProc.m_nNoTargetPixel;
		pCurStep->m_nNoGoodPixel	= g_ImageProc.m_nNoGoodPixel;


		//================
		// Show Test Data
		//================
		szMsg1.Format("Good:%.1f | Error:%.2f/%.2f/%.3f/%.3f/%.3f",
		pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
		pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
		pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);
		
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		

		if(CurrentSet->bSaveProcessingTimeData)
		{
			DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;
	
			dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
			szFuncElapsedTime.Format("  E-ColorTest [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
			AddTimeData(szFuncElapsedTime);
		}
	
		if(nResult == TEST_PASS)
		{
			if((fMaxDifference < 0) && (fMaxDifference < pCurStep->m_fLowLimit))
			{
				return pCurStep->m_fLowLimit;
			}
			
			if((fMaxDifference >= 0) && (fMaxDifference > pCurStep->m_fHighLimit))
			{
				return pCurStep->m_fHighLimit;
			}
		}
		else
		{
			if(fMaxDifference >= 0)
			{
				if(fMaxDifference < 100) fMaxDifference += 1;
			}
			else
			{
				if(fMaxDifference > -100) fMaxDifference -= 1;
			}
		}
		SystemMonitorLog_Save("_Color_Test6");
		return fMaxDifference; 	
	}
	else
	{
		return 100;//MAX_COLOR_LEVEL; 	
	}
}


// Image Processing Function
double	_Signal_Line_Test()
{
	int nResult = TEST_FAIL;


	CString szMsg		= _T("");
	CString szPrevMsg	= _T("");
	CString szColor		= _T("");

	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	// Initialization
	//+change kwmoon 070929
	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);
	g_ImageProc.SetAvgMaskSize(0,0,0); 

	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		g_ImageProc.m_LineBitTestData.InitBitTestData();
		g_ImageProc.m_LineBitTestData.m_pGrabImage	= &g_GrabImage;
		g_ImageProc.m_LineBitTestData.m_pRefImage	= &g_RefImage;
		g_ImageProc.m_LineBitTestData.m_pMaskImage	= &g_MaskImage;

		nResult = g_ImageProc.SignalLineTest(NORMAL_IMAGE_TEST,g_MaskImage.m_bImageLoaded,ctrlImgProg);
	
		// Initialization
		g_ImageProc.SetAvgMaskSize(0,0,0); 
		//+del kwmoon 080618
	//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition);
		
		pCurStep->m_nTestType = SIGNAL_LINE_TEST;

//		pCurStep->m_nTotalNoErrorPixel = nTotalNoErrorPixel;

		if(nResult == TEST_ABORT) return 0.0;

		pCurStep->m_nTotalNoMaskedPixel		= g_ImageProc.m_LineBitTestData.m_nTotalNoMaskedPixel;
		pCurStep->m_nTotalNoUnmaskedPixel	= g_ImageProc.m_LineBitTestData.m_nTotalNoUnmaskedPixel;
		pCurStep->m_nBitCount				= g_ImageProc.m_LineBitTestData.m_nBitCount;

		for(int i=0; i<3; i++)
		{
			for(int j=0; j<MAX_NO_COLOR_BITS; j++)
			{
				pCurStep->m_aRefNoHighBit[i][j]		= g_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][j];
				pCurStep->m_aGrabNoHighBit[i][j]	= g_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][j];
			}
		}

		if(nResult == TEST_PASS)
		{
			return 1.0;
		}
		else
		{
			switch(g_ImageProc.m_LineBitTestData.m_nErrorColor)
			{
				case 0 : szColor = "B"; break;
				case 1 : szColor = "G"; break;
				case 2 : szColor = "R"; break;
			}
			
			szMsg.Format("Faulty Bit Position : %s(%d)",szColor,
						 g_ImageProc.m_LineBitTestData.m_nErrorBitPosition);

			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg);
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
			}
			
			return 0.0;
		}
	}
	else
	{
		return 0.0;	
	}
}

// Image Processing Function
double	_Noise_Test()
{
	int nResult = TEST_FAIL;

	int nMaxDifference		= 0;
	int nPlusMaxDifference	= 0;
	int nMinusMaxDifference = 0;
	int nTotalNoErrorPixel	= 0;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	POINT ptPlusResultPosition;
	ptPlusResultPosition.x = 0;
	ptPlusResultPosition.y = 0;

	POINT ptMinusResultPosition;
	ptMinusResultPosition.x = 0;
	ptMinusResultPosition.y = 0;

//	POINT ptZeroPosition;
//	ptZeroPosition.x = 0;
//	ptZeroPosition.y = 0;

	CString szMsg1 = _T("");
	CString szMsg2 = _T("");
	CString szPrevMsg = _T("");

//	float fNewHighSpec = 0.0;
//	float fNewLowSpec  = 0.0;
	int   nColorDepth  = 0;
	int   nNoLevel	   = 0;

	//+add kwmoon 080124
//	int nSpecMargin	   = 0;

	CString sMsg		  = _T("");
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-NoiseTest");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
	}

	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	//+add kwmoon 080819
	g_ImageProc.m_ptPositionShift.x = 0;
	g_ImageProc.m_ptPositionShift.y = 0;

	// Initialization
	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);
	g_ImageProc.SetAvgMaskSize(0,0,0); 

	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		nResult = g_ImageProc.NoiseTest(NORMAL_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
									  ptPlusResultPosition,ptMinusResultPosition, 
									  nPlusMaxDifference,nMinusMaxDifference,nTotalNoErrorPixel,g_MaskImage.m_bImageLoaded,ctrlImgProg);
	
		// Initialization
		g_ImageProc.SetAvgMaskSize(0,0,0); 
		//+del kwmoon 080618
	//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition);
		
		pCurStep->m_nTestType = RGB_TEST;
		pCurStep->m_nTotalNoErrorPixel = nTotalNoErrorPixel;

		//+add kwmoon 080716
		for(int i=0; i<5; i++)
		{
			pCurStep->m_nNoBandErrorPixel[i]				= g_ImageProc.m_nNoBandErrorPixel[i];
			pCurStep->m_nNoAllowedBandErrorPixel[i]			= g_ImageProc.m_nNoAllowedBandErrorPixel[i];
			pCurStep->m_fBandErrorPixelPercentage[i]		= g_ImageProc.m_fBandErrorPixelPercentage[i];
			pCurStep->m_fAllowedBandErrorPixelPercentage[i]	= g_ImageProc.m_fAllowedBandErrorPixelPercentage[i];
		}
		pCurStep->m_fGoodPixelPercentage = g_ImageProc.m_fGoodPixelPercentage;

		pCurStep->m_nNoRoiPixel		= g_ImageProc.m_nNoRoiPixel;
		pCurStep->m_nNoTargetPixel	= g_ImageProc.m_nNoTargetPixel;
		pCurStep->m_nNoGoodPixel	= g_ImageProc.m_nNoGoodPixel;


		if(nResult == TEST_ABORT)
		{
			//+add kwmoon 080201
			AfxMessageBox("Failed to run noise test");

			return MAX_COLOR_LEVEL;
		}

		nColorDepth	 = g_RefImage.m_InfoHeader.biBitCount;

		//+change kwmoon 080722
//		nNoLevel	 = (int)pow(2,(nColorDepth/3));		//+add kwmoon 080716
		nNoLevel	 = (int)pow(2.0,(nColorDepth/3))-1;		//+add kwmoon 080716
		
		//+change kwmoon 080715
		g_GrabImage.m_bSaveProcessedImage = TRUE; // CurrentSet->bSaveProcImg;
		
		//+add kwmoon 080715 : if statement 
		if((CurrentSet->bSaveGrabImg)
		|| (!CurrentSet->bSaveGrabImg && (nResult == TEST_FAIL)))
		{
			//=================
			// Save Grab Image 
			//=================
			if(pCurStep->m_strGrabPath == _T(""))
			{
				pCurStep->m_strGrabPath = g_GrabImage.m_szFilePath;
			}
		//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
			//	g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
				g_GrabImage.SaveJpgImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
			}
		}

		//+add kwmoon 070807
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++)
			{
				pCurStep->m_aNoDifferentPoint[i][j] = g_ImageProc.m_aNoDifferentPoint[i][j];
			}
		}
		pCurStep->m_nSecondDifferentPointFreePercent = g_ImageProc.m_nSecondDifferentPointFreePercent;

		if(nPlusMaxDifference >= abs(nMinusMaxDifference))
		{
			nMaxDifference		= nPlusMaxDifference;
			ptResultPosition.x  = ptPlusResultPosition.x;
			ptResultPosition.y  = ptPlusResultPosition.y;
		}
		else
		{
			nMaxDifference		= nMinusMaxDifference;
			ptResultPosition.x  = ptMinusResultPosition.x;
			ptResultPosition.y  = ptMinusResultPosition.y;
		}

		//+del kwmoon 080716
		szMsg1.Format("Good:%.1f | Error : %.2f/%.2f/%.3f/%.3f/%.3f",
		pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
		pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
		pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);
	

		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		
		
		if(CurrentSet->bSaveProcessingTimeData)
		{
			DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

			dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
			szFuncElapsedTime.Format("  E-NoiseTest [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
			AddTimeData(szFuncElapsedTime);
		}

		float fReturnPercentageValue = g_ImageProc.Level2Percentage(nMaxDifference,nNoLevel);

		if(nResult == TEST_PASS)
		{
			if((fReturnPercentageValue < 0) && (fReturnPercentageValue < pCurStep->m_fLowLimit))
			{
				return pCurStep->m_fLowLimit;
			}
			
			if((fReturnPercentageValue >= 0) && (fReturnPercentageValue > pCurStep->m_fHighLimit))
			{
				return pCurStep->m_fHighLimit;
			}
		}
		else
		{
			if(fReturnPercentageValue >= 0)
			{
				if(fReturnPercentageValue < 100) fReturnPercentageValue += 1;
			}
			else
			{
				if(fReturnPercentageValue > -100) fReturnPercentageValue -= 1;
			}
		}
	
		return fReturnPercentageValue; 	
	}
	else
	{
		return MAX_COLOR_LEVEL; 	
	}
}

// Image Processing Function
double	_Bright_Test()
{
	int nResult = TEST_FAIL;

	int nMaxDifference = 0;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	CString szMsg;
	CString szPrevMsg;

	int nBrightMarginLevel = (int)pCurStep->m_fHighLimit;
	//090324
	pCurStep->m_bRunVideoTest = TRUE;

	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		g_ImageProc.SetBrightMargin(nBrightMarginLevel);

		nResult = g_ImageProc.BrightnessTest(g_GrabImage, g_RefImage,g_MaskImage,ptResultPosition,nMaxDifference,ctrlImgProg);

		// Initialization
		g_ImageProc.SetAvgMaskSize(0,0,0); 

		pCurStep->m_nTestType = BRIGHT_TEST;

		if(nResult == TEST_ABORT) return MAX_COLOR_LEVEL;
		
		//+change kwmoon 080715
		g_GrabImage.m_bSaveProcessedImage = TRUE; //CurrentSet->bSaveProcImg;

		//+add kwmoon 080715 : if statement 
		if((CurrentSet->bSaveGrabImg)
		|| (!CurrentSet->bSaveGrabImg && (nResult == TEST_FAIL)))
		{
			//=================
			// Save Grab Image 
			//=================
			if(pCurStep->m_strGrabPath == _T(""))
			{
				pCurStep->m_strGrabPath = g_GrabImage.m_szFilePath;
			}
		//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
			//	g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
				g_GrabImage.SaveJpgImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
			}
		}

		szMsg.Format("Diff : %d(%d,%d)",nMaxDifference,ptResultPosition.x,ptResultPosition.y);

		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " +szMsg);
				//+ 2007.7.23 Add BY USY
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg;
			//-
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
			//+ 2007.7.23 Add BY USY
			pCurStep->m_strMsg = szMsg;
			//-
		}		
		return nMaxDifference;
	}
	else
	{
		return MAX_COLOR_LEVEL; 	
	}	
}

// Image Processing Function
int	_Dark_Test()
{
	int nResult = TEST_FAIL;

	int nMaxDifference = 0;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	CString szMsg;
	CString szPrevMsg;

	SystemMonitorLog_Save("_Dark_Test");// , _T(__FILE__), __LINE__);

	if (g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		//g_ImageProc.SetBrightMargin(nBrightMarginLevel);

		nResult = g_ImageProc.DarknessTest(g_GrabImage, g_MaskImage);

		// Initialization
		szMsg.Format("_Dark_Test Result: %d", nResult);
		SystemMonitorLog_Save(szMsg);
		return nResult;
	}
	else
	{
		szMsg.Format("_Dark_Test Result: No Test", nResult);
		SystemMonitorLog_Save(szMsg);
		return nResult;
	}
}

int	_White_Test()
{
	int nResult = TEST_FAIL;

	int nMaxDifference = 0;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	CString szMsg;
	CString szPrevMsg;

	SystemMonitorLog_Save("_White_Test");// , _T(__FILE__), __LINE__);

	if (g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		//g_ImageProc.SetBrightMargin(nBrightMarginLevel);

		nResult = g_ImageProc.WhitenessTest(g_GrabImage, g_MaskImage);

		// Initialization
		szMsg.Format("_White_Test Result: %d", nResult);
		SystemMonitorLog_Save(szMsg);
		return nResult;
	}
	else
	{
		szMsg.Format("_White_Test Result: No Test", nResult);
		SystemMonitorLog_Save(szMsg);
		return nResult;
	}
}

// Image Processing Function
double	_Position_Test()
{
	int nResult = TEST_FAIL;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	POINT ptPositionShift;
	ptPositionShift.x = 0;
	ptPositionShift.y = 0;

//	POINT ptZeroPosition;
//	ptZeroPosition.x = 0;
//	ptZeroPosition.y = 0;

	float fMatchRate = 0.0f;

	CString szMsg;
	CString szPrevMsg;
	//090324
	pCurStep->m_bRunVideoTest = TRUE;

	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		// Initialize position shift
		// In case of position test, set position shift 0
		g_ImageProc.SetPositionShift(ptPositionShift);

		nResult = g_ImageProc.PatternMatching(POSITION_TEST,g_GrabImage,g_RefImage,ptResultPosition,fMatchRate,ctrlImgProg);

		pCurStep->m_nTestType = POSITION_TEST;

		if(nResult == TEST_ABORT) return 0.0;

		szMsg.Format("Match : %.1f%% (%d,%d)",fMatchRate,ptResultPosition.x,ptResultPosition.y);

		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " +szMsg);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
			pCurStep->m_strMsg = szMsg;
		}		

		if(nResult == TEST_PASS)
		{
			if((g_ImageProc.m_nRoiHeight > 0) && (g_ImageProc.m_nRoiWidth > 0)) // In case of applying ROI
			{
				g_ImageProc.m_ptPositionShift.x = ptResultPosition.x - g_ImageProc.m_ptRoiPosition.x;
				g_ImageProc.m_ptPositionShift.y = ptResultPosition.y - g_ImageProc.m_ptRoiPosition.y;
			}
			else
			{
				g_ImageProc.m_ptPositionShift = ptResultPosition;
			}
		}
		else
		{
				g_ImageProc.m_ptPositionShift.x = 0;
				g_ImageProc.m_ptPositionShift.y = 0;
		}
	}

	// Initialize Roi variable
	//+del kwmoon 080618
//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	if(nResult == TEST_PASS) return 1.0;
	else return 0.0;
}

//===========================
// Image Processing Function
//===========================

double	_Osd_Test()
{
	int nResult				= TEST_FAIL;

	int nTotalNoErrorPixel	= 0;

	POINT ptResultPosition;
	ptResultPosition.x		= 0;
	ptResultPosition.y		= 0;

	CString sMsg			= _T("");
	CString szMsg			= _T("");
	CString szMsg1			= _T("");
	CString szMsg2			= _T("");
	CString szPrevMsg		= _T("");

	float fMatchRate		= 0.0;
	float fMaxDifference	= 0.0;

	BOOL  bOsdTestAfterPatternMatch = FALSE;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-OsdTest");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
	}

	//+add kwmoon 080819
	g_ImageProc.m_ptPositionShift.x = 0;
	g_ImageProc.m_ptPositionShift.y = 0;

	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
	

START_OSD_TEST :

		g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);
		g_ImageProc.SetAvgMaskSize(0,0,0); 
		
		nResult = g_ImageProc.RGBTest(OSD_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
								  ptResultPosition,fMaxDifference,nTotalNoErrorPixel,g_MaskImage.m_bImageLoaded,ctrlImgProg);

		// Initialization
		g_ImageProc.SetAvgMaskSize(0,0,0); 
	
		pCurStep->m_nTotalNoErrorPixel = nTotalNoErrorPixel;
		
		if(nResult == TEST_ABORT)
		{
			pCurStep->m_nTotalNoErrorPixel = -1;
			AfxMessageBox("Failed to run osd test");
			return 100;
		}
		
		g_GrabImage.m_bSaveProcessedImage = TRUE; 

		if((CurrentSet->bSaveGrabImg)
		|| (!CurrentSet->bSaveGrabImg && (nResult == TEST_FAIL)))
		{
			//=================
			// Save Grab Image 
			//=================
			if(pCurStep->m_strGrabPath == _T(""))
			{
				pCurStep->m_strGrabPath = g_GrabImage.m_szFilePath;
			}
		//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
			//	g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
				g_GrabImage.SaveJpgImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
			}
		}

		//================
		// Save Test Data 
		//================
		if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData)
		{
			for(int i=0; i<3; i++)
			{
				for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++)
				{
					pCurStep->m_aNoDifferentPoint[i][j] = g_ImageProc.m_aNoDifferentPoint[i][j];
				}
			}
			pCurStep->m_nSecondDifferentPointFreePercent = g_ImageProc.m_nSecondDifferentPointFreePercent;
		}

		for(int i=0; i<5; i++)
		{
			pCurStep->m_nNoBandErrorPixel[i]				= g_ImageProc.m_nNoBandErrorPixel[i];
			pCurStep->m_nNoAllowedBandErrorPixel[i]			= g_ImageProc.m_nNoAllowedBandErrorPixel[i];
			pCurStep->m_fBandErrorPixelPercentage[i]		= g_ImageProc.m_fBandErrorPixelPercentage[i];
			pCurStep->m_fAllowedBandErrorPixelPercentage[i]	= g_ImageProc.m_fAllowedBandErrorPixelPercentage[i];
		}
		pCurStep->m_fGoodPixelPercentage = g_ImageProc.m_fGoodPixelPercentage;

		pCurStep->m_nNoRoiPixel		= g_ImageProc.m_nNoRoiPixel;
		pCurStep->m_nNoTargetPixel	= g_ImageProc.m_nNoTargetPixel;
		pCurStep->m_nNoGoodPixel	= g_ImageProc.m_nNoGoodPixel;

		//================
		// Show Test Data
		//================
		szMsg1.Format("Good:%.1f | Error : %.2f/%.2f/%.3f/%.3f/%.3f",
		pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
		pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
		pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);

		
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		

		if(CurrentSet->bSaveProcessingTimeData)
		{
			DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;

			DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
			szFuncElapsedTime.Format("  E-OsdTest [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
			AddTimeData(szFuncElapsedTime);
		}

		//================
		// NORMAL_PROCESS 
		//================
		if(pCurStep->GetTestParm() == NORMAL_PROCESS)
		{
			if(nResult == TEST_PASS)
			{
				if((fMaxDifference < 0) && (fMaxDifference < pCurStep->m_fLowLimit))
				{
					return pCurStep->m_fLowLimit;
				}

				if((fMaxDifference >= 0) && (fMaxDifference > pCurStep->m_fHighLimit))
				{
					return pCurStep->m_fHighLimit;
				}
				return fMaxDifference;
			}
			else
			{
				if(bOsdTestAfterPatternMatch)
				{
					return 100;
				}

				//==================
				// Pattern Matching 
				//==================

				nResult = g_ImageProc.PatternMatching(OSD_TEST,g_GrabImage,g_RefImage,ptResultPosition,fMatchRate,ctrlImgProg);
			
				if(nResult == TEST_ABORT)
				{
					return 100;
				}
				
				szMsg.Format("Match : %.1f%% (%d,%d)",fMatchRate,ptResultPosition.x,ptResultPosition.y);

				if(nResult == TEST_PASS)
				{
					if((g_ImageProc.m_nRoiHeight > 0) && (g_ImageProc.m_nRoiWidth > 0)) 
					{
						g_ImageProc.m_ptPositionShift.x = ptResultPosition.x - g_ImageProc.m_ptRoiPosition.x;
						g_ImageProc.m_ptPositionShift.y = ptResultPosition.y - g_ImageProc.m_ptRoiPosition.y;
					}
					else
					{
						g_ImageProc.m_ptPositionShift = ptResultPosition;
					}
				}
				else
				{
						g_ImageProc.m_ptPositionShift.x = 0;
						g_ImageProc.m_ptPositionShift.y = 0;
				}
			
				g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
				
				if(szPrevMsg != _T(""))
				{
						g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " +szMsg);
						pCurStep->m_strMsg = szPrevMsg + "; " + szMsg;
				}
				else
				{
					g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
					pCurStep->m_strMsg = szMsg;
				}		

				if(nResult == TEST_PASS)
				{
					bOsdTestAfterPatternMatch = TRUE; goto START_OSD_TEST;
				}
				else
				{
					return 100;
				}
			}
		}
		//=================
		// REVERSE_PROCESS 
		//=================
		else
		{
			if(nResult == TEST_FAIL)
			{
				if(CurrentSet->bSaveProcessingTimeData)
				{
					DWORD dwStepElapsedTime = GetTickCount()-g_pView->m_dwStepElapsedTime;
			
					DWORD dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
					szFuncElapsedTime.Format("  E-OsdTest [%dms] (%dms)",dwFuncElapsedTime,dwStepElapsedTime);
					AddTimeData(szFuncElapsedTime);
				}
				
				//+add kwmoon 080131
				pCurStep->SetTestParm(NORMAL_PROCESS);

				return fMaxDifference;
			}
			else
			{
				return 100;
			}

		}
	}
	else
	{
		AfxMessageBox("Failed to load grab or reference image");
		return 100;
	}
}

//+del kwmoon 080131

// Image Processing Function
// The result of position test affects a size test on position shift.
double	_Size_Test()
{
	int nResult = TEST_FAIL;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

//	POINT ptZeroPosition;
//	ptZeroPosition.x = 0;
//	ptZeroPosition.y = 0;
	
	float fMatchRate = 0;
	
	CString szMsg;
	CString szPrevMsg;

	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		nResult = g_ImageProc.PatternMatching(SIZE_TEST,g_GrabImage,g_RefImage,ptResultPosition,fMatchRate,ctrlImgProg);

		pCurStep->m_nTestType = SIZE_TEST;
	
		if(nResult == TEST_ABORT) return 0.0;	

		szMsg.Format("Match : %.1f%% (%d,%d)",fMatchRate,ptResultPosition.x,ptResultPosition.y);
	
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " +szMsg);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
			pCurStep->m_strMsg = szMsg;
		}		
	}

	//+del kwmoon 080618
//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	if(nResult == TEST_PASS) return 1.0;
	else return 0.0;
}

#include <winsock.h>
#include <windowsx.h>
#include "ipexport.h"
#include "icmpdefs.h"

typedef HANDLE (WINAPI* pfnHV)(VOID);
typedef BOOL   (WINAPI* pfnBH)(HANDLE);
typedef DWORD  (WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD);

int PingTest()
{
	CString szResultMessage;

	DWORD dwStatus = 0;
	PIP_ECHO_REPLY pIpe = NULL;
    
	char acPingBuffer[64];
    memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));

	HINSTANCE hIcmp = NULL;

	HANDLE hIP = NULL;

	struct hostent* phe;

	int nReturn = 0;

	// Function Pointers
    pfnHV			pIcmpCreateFile  = NULL;
    pfnBH			pIcmpCloseHandle = NULL;
    pfnDHDPWPipPDD	pIcmpSendEcho    = NULL;


    // Load the ICMP.DLL
    hIcmp = LoadLibrary("ICMP.DLL");
   
	if(hIcmp == 0) 
	{
        OkMessage("Unable to locate ICMP.DLL!"); goto END_POSITION;
    }

    // Look up an IP address for the given host name
    if((phe = gethostbyname("12.12.2.2")) == 0) 
	{
        OkMessage("Could not find IP address for 12.12.2.2"); goto END_POSITION;
    }

    // Get handles to the functions inside ICMP.DLL that we'll need
    pIcmpCreateFile  = (pfnHV)GetProcAddress(hIcmp,"IcmpCreateFile");;
    pIcmpCloseHandle = (pfnBH)GetProcAddress(hIcmp,"IcmpCloseHandle");;
    pIcmpSendEcho    = (pfnDHDPWPipPDD)GetProcAddress(hIcmp,"IcmpSendEcho");;

    if((pIcmpCreateFile == 0) || (pIcmpCloseHandle == 0) || (pIcmpSendEcho == 0)) 
	{
        OkMessage("Failed to get proc addr for function"); goto END_POSITION;;
    }

    // Open the ping service
    hIP = pIcmpCreateFile();

    if(hIP == INVALID_HANDLE_VALUE) 
	{
        OkMessage("Unable to open ping service"); goto END_POSITION;
    }
   
    // Build ping packet
    pIpe = (PIP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,
				           sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
    
	if(pIpe == 0) 
	{
        OkMessage("Failed to allocate global ping packet buffer"); goto END_POSITION;
    }

    pIpe->Data		= acPingBuffer;
    pIpe->DataSize	= sizeof(acPingBuffer);      


    // Send the ping packet
    dwStatus = pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), 
					 acPingBuffer, sizeof(acPingBuffer), NULL, pIpe, 
					 sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 5000);
/*
    if(dwStatus != 0) 
	{
		szResultMessage.Format("Addr: %d.%d.%d.%d RTT: %d ms, TTL : %d",
                int(LOBYTE(LOWORD(pIpe->Address))),
                int(HIBYTE(LOWORD(pIpe->Address))),
                int(LOBYTE(HIWORD(pIpe->Address))),
                int(HIBYTE(HIWORD(pIpe->Address))),
                int(pIpe->RoundTripTime),
                int(pIpe->Options.Ttl));

		OkMessage(szResultMessage);
    }
    else 
	{
		OkMessage("Error obtaining info from ping packet.");
    }
*/

END_POSITION :

    // Shut down
	if(pIpe  != NULL) GlobalFree(pIpe);
    if(hIcmp != NULL) FreeLibrary(hIcmp);

	if(dwStatus != 0) return TEST_PASS;
	else return TEST_FAIL;
}

double	_Lan_Test()
{
	int nResult = TEST_FAIL;

	CString szMsg;

    WSAData wsaData;

	// WSAStartup : Initiates use of WS2_32.DLL
    if(WSAStartup(MAKEWORD(1, 1),&wsaData) != 0) 
	{
        return 0.0; // TEST_FAIL
    }

    nResult = PingTest();

    WSACleanup();
  
	if(nResult == TEST_PASS) return 1.0;
	else return 0.0;
}

//+ 2007.5.8 Add BY USY
double _TVComm_Test()
{
	int nResult = TEST_FAIL;
	CString sMsg = _T("");
	CString sReadStr = _T("");

	//Port Open Check
	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
			if(TVCommCtrl.m_bPortOpen == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0; 
			}
		}
	}

	if(TVCommCtrl.SendRun(TVCommCtrl.m_strCmd1, TVCommCtrl.m_strCmd2, TVCommCtrl.m_nSetId, TVCommCtrl.m_strData ))
	{
		if(TVCommCtrl.ReceiveCommString(20, TVCommCtrl.m_nWaitLimit, sReadStr))
		{
			sMsg.Format("Receive : %s", sReadStr);
			AddStringToStatus(sMsg);
		}
		else
		{
			sMsg.Format("Read String From TV CommPort Fail!");
			AddStringToStatus(sMsg);
		}

	}
	else
	{
		sMsg.Format("Send Command Fail!");
		AddStringToStatus(sMsg);
	}

	return 1.0;
}

double	_Audio_Test()
{
	int nResult = TEST_FAIL;

	pCurStep->m_nTestType	  = AUDIO_TEST;
	pCurStep->m_bRunAudioTest = TRUE;

	if(g_pView->m_bMakeReferenceMode)
	{
		pCurStep->m_bResult = TRUE; return 1.0;
	}

	//---------------------------------
	// Measure audio with av-switchbox
	//---------------------------------
	if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
	{
		if(!CurrentSet->bUseAVSwitchBox)
		{
			pCurStep->m_bResult = FALSE; return 1.0;
		}

		//change PSH 081104
//		AvSwitchBoxCtrl.StartAudioMeasureThread(&AvSwitchBoxCtrl);

//		if(AvSwitchBoxCtrl.m_bResult == TRUE)
		if(AvSwitchBoxCtrl.AudioMeasure() == TRUE)
		{
			pCurStep->m_bResult = TRUE;
		}
		else
		{
//			pCurStep->m_bResult = FALSE;
			pCurStep->m_bResult = AvSwitchBoxCtrl.AudioMeasure();
		}
	}
	//-------------------------------
	// Measure audio with sound card
	//-------------------------------
	else
	{
		if(g_SoundCard.AudioMeasure() == TRUE)
		{
			pCurStep->m_bResult = TRUE;
		}
		else
		{
			_Wait(800);

			if(g_SoundCard.AudioMeasure() == TRUE)
			{
				pCurStep->m_bResult = TRUE;
			}
			else
			{
				_Wait(1500);

				pCurStep->m_bResult = g_SoundCard.AudioMeasure();
			}
		}
	}
	return 1.0;
}

//+add kwmoon 071001
double _Manual_Test()
{
	int nResult = TEST_FAIL;
	CString szMsg;

	g_pView->m_ManualTestDlg.DoModal();
	g_pView->m_ManualTestDlg.ClearInfoMessage();
	pCurStep->m_nTestType = MANUAL_TEST;

	if(g_pView->m_ManualTestDlg.m_bTestResult) nResult = TEST_PASS;
	else nResult = TEST_FAIL;


	if(nResult == TEST_PASS) return 1.0;
	else return 0.0;
}

BOOL Grab_LvdsImage_2in1()
{
	CString sGrabPath	= _T("");

	int	nNoRetry		= 0;
	int nGrabResult = GRAB_FAIL;

	if(CurrentSet->bRunAbort)
	{
		g_pView->m_nNoCurrentRepeatExecution = 1; return FALSE;
	}

	int nWidth		= 0;
	int nHeight		= 0;
	int nImageSize	= 0;
//	int nErrorCode	= 0;
//	int nImageBufferSize = 0;
	int nBitCount	= 0;
	int nRowWidth	= 0;

	CString szFileName = _T("");
	CString szErrorMsg = _T("");

	BYTE* pImageBuffer = NULL;

	nGrabResult = GRAB_FAIL;

	CRect nImageRect;
	CWnd* pImageWnd;
	pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	nWidth		= CurrentSet->nLvdsWidth;
	nHeight		= CurrentSet->nLvdsHeight;
	nBitCount	= CurrentSet->nBitCount;
	nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
	nImageSize  = nRowWidth * nHeight;

//	g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(!g_pView->Grab_UHD())
		{
			szErrorMsg = "No LVDS input image!";
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);

			g_GrabImage.m_bImageLoaded = TRUE;
		}
	}
	else//FHD
	{
		if((nGrabResult = g_pView->Grab_FHD(GRAB_WITH_RETRY)) != GRAB_PASS)
		{
			szErrorMsg = "No LVDS input image!";
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);

			g_GrabImage.m_bImageLoaded = TRUE;
		}
	}


	ctrlImgProg.SetPos(40);

	g_GrabImage.m_bImageLoaded = TRUE;

	g_pView->InvalidateRect(nImageRect, FALSE);

	g_pView->UpdateWindow(); _Wait(10);	
	
	ctrlImgProg.SetPos(100);

	return TRUE;
}

#define NO_GRAB_FOR_MOVING_PIC_TEST		15
double _Moving_Pic_Test()
{
	int nResult		   = TEST_FAIL;
	int nNoGrab		   = NO_GRAB_FOR_MOVING_PIC_TEST;
	int nNoBlackScreen = 0;

	int nVideoMovingResult = TEST_FAIL;
	int nFreqMovingResult = TEST_FAIL;

	//+add 090218(Modification No1)
	int nNoMpicColor = 0;
	int nMpicColorThreshold = CurrentSet->nNoUsedColors;

	CString szString		 = _T("");
	CString szMovingPicImage = _T("");
	CString sGrabPath		 = _T("");
	CString sGrabFileName	 = "MovingPic.bmp";
	
#ifndef _ABORT_DEBUG__MODE
	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sGrabFileName);
	
	pCurStep->m_strGrabPath  = sGrabPath;
	pCurStep->m_strProcPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";
	g_GrabImage.m_szFilePath = sGrabPath;

	pCurStep->m_bRunVideoTest = TRUE;
	//090324
	
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	



	float fLeftFreqMax = 0;
	float fLeftFreqMin = 50000;
	float fRightFreqMax = 0;
	float fRightFreqMin = 50000;
	_AudioMeasureReset();// g_pView->AudioMeasureStart();

	float fRightFreqArry[50];
	float fLeftFreqArry[50];
#if 0
	for (int i = 0; i < 50; i++)
	{
		if (CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			if (!CurrentSet->bUseAVSwitchBox)
			{
				nFreqMovingResult = FALSE;
				break;
			}
			else
			{
				AvSwitchBoxCtrl.AudioMeasure();
				if (AvSwitchBoxCtrl.m_fMeasuredLeftFreq > 20)
				{
					fLeftFreqMax = max(fLeftFreqMax, AvSwitchBoxCtrl.m_fMeasuredLeftFreq);
					fLeftFreqMin = min(fLeftFreqMin, AvSwitchBoxCtrl.m_fMeasuredLeftFreq);
				}

				if (AvSwitchBoxCtrl.m_fMeasuredRightFreq > 20)
				{
					fRightFreqMax = max(fRightFreqMax, AvSwitchBoxCtrl.m_fMeasuredRightFreq);
					fRightFreqMin = min(fRightFreqMin, AvSwitchBoxCtrl.m_fMeasuredRightFreq);

				}

				if (((fLeftFreqMin < AvSwitchBoxCtrl.m_fMeasuredLeftFreq)
					&& (AvSwitchBoxCtrl.m_fMeasuredLeftFreq < fLeftFreqMax))
					|| ((fRightFreqMin < AvSwitchBoxCtrl.m_fMeasuredRightFreq)
						&& (AvSwitchBoxCtrl.m_fMeasuredRightFreq < fRightFreqMax))
					&& (10 < (fLeftFreqMax - fLeftFreqMin))
					&& (10 < (fRightFreqMax - fRightFreqMin)))
				{
					nFreqMovingResult = TRUE;
					break;
				}
			}
		}
		else
		{
			g_SoundCard.AudioMeasure();

			if (g_SoundCard.m_fMeasuredLeftFreq > 20)
			{
				fLeftFreqMax = max(fLeftFreqMax, g_SoundCard.m_fMeasuredLeftFreq);
				fLeftFreqMin = min(fLeftFreqMin, g_SoundCard.m_fMeasuredLeftFreq);
			}

			if (g_SoundCard.m_fMeasuredRightFreq > 20)
			{
				fRightFreqMax = max(fRightFreqMax, g_SoundCard.m_fMeasuredRightFreq);
				fRightFreqMin = min(fRightFreqMin, g_SoundCard.m_fMeasuredRightFreq);
			}

			fRightFreqArry[i] = g_SoundCard.m_fMeasuredRightFreq;
			fLeftFreqArry[i] = g_SoundCard.m_fMeasuredLeftFreq;

			//if (((fLeftFreqMin < g_SoundCard.m_fMeasuredLeftFreq) 
			//	&& (g_SoundCard.m_fMeasuredLeftFreq < fLeftFreqMax))
			//	|| ((fRightFreqMin < g_SoundCard.m_fMeasuredRightFreq) 
			//	&& (g_SoundCard.m_fMeasuredRightFreq < fRightFreqMax))
			//	&& (10 < (fLeftFreqMax - fLeftFreqMin))
			//	&& (10 < (fRightFreqMax - fRightFreqMin)))
			if (((fLeftFreqMin <= g_SoundCard.m_fMeasuredLeftFreq)
				&& (g_SoundCard.m_fMeasuredLeftFreq < fLeftFreqMax))
				|| ((fRightFreqMin <= g_SoundCard.m_fMeasuredRightFreq)
					&& (g_SoundCard.m_fMeasuredRightFreq < fRightFreqMax))
				&& (10 < (fLeftFreqMax - fLeftFreqMin))
				&& (10 < (fRightFreqMax - fRightFreqMin)))
			{
				nFreqMovingResult = TRUE;
				break;
			}
		}
		_Wait(100);
	}

	if (nFreqMovingResult == TEST_FAIL)
	{
		szMsg1.Format("Freq Moving : Left(Min %2.0f, Max %2.0f) Right (Min %2.0f, Max %2.0f)", fLeftFreqMin, fLeftFreqMax, fRightFreqMin, fRightFreqMax);

		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);

		if (szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}
		//return 0.0;
	}
#else
	nFreqMovingResult = TRUE;
#endif	

	for (int i = 0; i < nNoGrab; i++)
	{
		Grab_LvdsImage_2in1();

		if (g_GrabImage.m_bImageLoaded)
		{
			//+change 090218(Modification No1)
		//	nResult = g_ImageProc.NoVideoSignalTest(g_GrabImage, ctrlImgProg);
			nResult = g_ImageProc.NoVideoSignalTest2(g_GrabImage, nMpicColorThreshold, &nNoMpicColor);

			if (CurrentSet->bSaveMovingPicTestImg)
			{
				if (g_GrabImage.m_szFilePath != _T(""))
				{
					szString = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.'));
					szMovingPicImage.Format("%s_MP%d.bmp", szString, i);

					//	g_GrabImage.SaveImage(szMovingPicImage,SNAP_IMAGE,ctrlImgProg);
					g_GrabImage.SaveJpgImage(szMovingPicImage, SNAP_IMAGE, ctrlImgProg);
				}
			}

			if (nResult == TEST_ABORT)
			return 0.0;

			if (nResult == TEST_PASS)
			{
				nNoBlackScreen++;
			}

			//+change 
			if (nNoBlackScreen >= 2)
			{
				nVideoMovingResult = TEST_PASS;
				break;// return 1.0;
			}
		}
		_Wait(100);
	}



	if (nVideoMovingResult == TEST_FAIL)
	{
		//+add 090218(Modification No1)
		szMsg1.Format("No of color used : %d (Threshold : %d)", nNoMpicColor, nMpicColorThreshold);
		AddStringToStatus(szMsg1);
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);

		if (szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}

		if (g_GrabImage.m_szFilePath != _T(""))
		{
			//	g_GrabImage.SaveImage(sGrabPath,SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(sGrabPath, SNAP_IMAGE, ctrlImgProg);
		}

		//return 0.0;
	}
	else
	{
		szMsg1.Format("[PASS]No of color : %d (Threshold : %d)", nNoMpicColor, nMpicColorThreshold);
		AddStringToStatus(szMsg1);
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);

		if (szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}

		if (g_GrabImage.m_szFilePath != _T(""))
		{
			//	g_GrabImage.SaveImage(sGrabPath,SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(sGrabPath, SNAP_IMAGE, ctrlImgProg);
		}
	}
#else
	nVideoMovingResult = TRUE;
#endif


	//if (pCurStep->m_bRunAudioTest == TRUE)
	//{
	//	nFreqMovingResult = TRUE;
	//}
	//else

	

//	int nImageWidth  = g_GrabImage.m_InfoHeader.biWidth;
//	int nImageHeight = g_GrabImage.m_InfoHeader.biHeight;
//	int nNoPixel	 = nImageWidth * nImageHeight - (g_ImageProc.m_nOsdWidth * g_ImageProc.m_nOsdHeight);


	
	

	if ((nVideoMovingResult == TEST_FAIL) || (nFreqMovingResult == TEST_FAIL))
	{
		return 0.0;
	}
	return 1.0;
}


/*
double _Moving_Pic_Test()
{
	int nResult		   = TEST_FAIL;
	int nResult2	   = TEST_FAIL;
	int nNoGrab		   = NO_GRAB_FOR_MOVING_PIC_TEST;
	int nNoBlackScreen = 0;

	CBaseImage TmpImage;

	//+add 090218(Modification No1)
	int nNoMpicColor = 0;
	int nMpicColorThreshold = CurrentSet->nNoUsedColors;

	CString szString		 = _T("");
	CString szMovingPicImage = _T("");
	CString sGrabPath		 = _T("");
	CString sGrabFileName	 = "MovingPic.bmp";

	sGrabPath.Format("%s_S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sGrabFileName);
	
	pCurStep->m_strGrabPath  = sGrabPath;
	pCurStep->m_strProcPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";
	g_GrabImage.m_szFilePath = sGrabPath;

	//090324
	pCurStep->m_bRunVideoTest = TRUE;

	TmpImage.m_pImageData = (BYTE *)malloc(g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
	memset(TmpImage.m_pImageData,0,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
	
	Grab_LvdsImage_2in1();
	
	g_ImageProc.CopyImage(g_GrabImage, TmpImage);

	
	for(int i=0; i<nNoGrab; i++)
	{
		Grab_LvdsImage_2in1();

		if(g_GrabImage.m_bImageLoaded)
		{
			//+change 090218(Modification No1)
		//	nResult = g_ImageProc.NoVideoSignalTest(g_GrabImage, ctrlImgProg);
			nResult = g_ImageProc.NoVideoSignalTest2(g_GrabImage, nMpicColorThreshold, &nNoMpicColor);	
	
			if(CurrentSet->bSaveMovingPicTestImg)
			{
				if(g_GrabImage.m_szFilePath != _T(""))
				{
					szString =  g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.'));
					szMovingPicImage.Format("%s_MP%d.bmp",szString,i);

				//	g_GrabImage.SaveImage(szMovingPicImage,SNAP_IMAGE,ctrlImgProg);
					g_GrabImage.SaveJpgImage(szMovingPicImage,SNAP_IMAGE,ctrlImgProg);
				}
			}

			if(nResult == TEST_ABORT){
				g_ImageProc.InitMovingPicTestMargin();
				return 0.0;
			}
			
			if(nResult == TEST_PASS)
			{
				//				nResult2 = g_ImageProc.CheckImageDiff(g_GrabImage, TmpImage, -20, 20); 
				nResult2 = g_ImageProc.CheckImageDiff(g_GrabImage, TmpImage, g_ImageProc.m_nDiff_LowerLimit, g_ImageProc.m_nDiff_UpperLimit); 
				if(nResult2 == TEST_PASS){
					nNoBlackScreen++;
				}
			}
			if(nNoBlackScreen > 3){
				g_ImageProc.InitMovingPicTestMargin();
				return 1.0;
			}
		}
		g_ImageProc.CopyImage(g_GrabImage, TmpImage);
		//	_Wait(100);
	}
	g_ImageProc.InitMovingPicTestMargin();

	CString szMsg1    = _T("");
	CString szPrevMsg = _T("");


	szMsg1.Format("No of color used : %d (Threshold : %d)",nNoMpicColor, nMpicColorThreshold);

	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
	
	if(szPrevMsg != _T(""))
	{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
		pCurStep->m_strMsg = szMsg1;
	}		

	if(g_GrabImage.m_szFilePath != _T(""))
	{
	//	g_GrabImage.SaveImage(sGrabPath,SNAP_IMAGE,ctrlImgProg);
		g_GrabImage.SaveJpgImage(sGrabPath,SNAP_IMAGE,ctrlImgProg);
	}

	return 0.0;
}
*/

//+add kwmoon 071108
#define NO_GRAB_FOR_CAPTION_TEST		15

double _Caption_Test()
{
	int nResult			= TEST_FAIL;
	int nNoGrab			= NO_GRAB_FOR_CAPTION_TEST;
	int nNoBlackScreen	= 0;

	int nCaptionKeyDelay		= 0;
	int nCaptionEnterKeyDelay	= 0;
	int nCaptionCheckTime		= 0;
	int nNoOnKeyPress			= 0;
	int nNoOffKeyPress			= 0;

	CString szString			= _T("");
	CString szCaptionImage		= _T("");
	CString szCaptionProcImage	= _T("");

	int nNoLevel	= 255;
	int nLowerLimit = g_ImageProc.Percentage2Level((float)pCurStep->m_fLowLimit,nNoLevel);
	int nUpperLimit = g_ImageProc.Percentage2Level((float)pCurStep->m_fHighLimit,nNoLevel);

	//+del kwmoon 080129
//	if(g_GrabImage.m_bImageLoaded == FALSE) 
//	{
//		AfxMessageBox("Grab Image is not loaded!"); return 100;
//	}
	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	
	if(g_RefImage.m_bImageLoaded == FALSE) 
	{
		AfxMessageBox("Reference Image is not loaded!"); return 100;
	}

	if(g_MaskImage.m_bImageLoaded == FALSE) 
	{
		AfxMessageBox("Mask Image is not loaded!"); return 100;
	}
	
	//+add kwmoon 071224
	nCaptionKeyDelay	  = g_ImageProc.m_nCaptionKeyDelay;
	nCaptionEnterKeyDelay = g_ImageProc.m_nCaptionEnterKeyDelay;
	nCaptionCheckTime	  = g_ImageProc.m_nCaptionCheckTime;	

	//+change kwmoon 080124
	//+add kwmoon 080110
	nNoOnKeyPress			  = g_ImageProc.m_nNoOnKeyPress;
	nNoOffKeyPress			  = g_ImageProc.m_nNoOffKeyPress;
		
	if(pCurStep->m_bStepInternalRetryFlag == FALSE)
	{
		//+change kwmoon 080124
		//+change kwmoon 080110
		// Set Caption Mode On
	//	for(int i=0; i<2; i++)
		for(int i=0; i<nNoOnKeyPress; i++)
		{
			//+2007.12.26 Mod BY USY
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "39"))
			{
				return FALSE;
			}
//			AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum("39"));
			//-

			if(!_Wait(nCaptionKeyDelay)) return 100;
		}

		//+2007.12.26 Mod BY USY
		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "44"))
		{
			return FALSE;
		}
//		AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum("44"));
		//-
		if(!_Wait(nCaptionEnterKeyDelay)) return 100;
	}

	//+add kwmoon 071224
	DWORD dwStartTick	= GetTickCount();
	DWORD dwElapsedTime = 0;
	
	//+del kwmoon 071224
//	for(int i=0; i<nNoGrab; i++)
	int nFrameNo = 0;

	while(TRUE)
	{
		Grab_LvdsImage_2in1();

		if(g_GrabImage.m_bImageLoaded)
		{
			nResult = g_ImageProc.CaptionTest(g_GrabImage,g_RefImage,g_MaskImage,nLowerLimit,nUpperLimit,ctrlImgProg);
			
			//+add kwmoon 080125 : Add If statement
			if(CurrentSet->bSaveCaptionTestImg)
			{
				if(g_GrabImage.m_szFilePath != _T(""))
				{
					szString =  g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.'));
					szCaptionImage.Format("%s_CAP%d.bmp",szString,nFrameNo);
					szCaptionProcImage.Format("%s_CAP_PROC%d.bmp",szString,nFrameNo);

				//	g_GrabImage.SaveImage(szCaptionImage,SNAP_IMAGE,ctrlImgProg);
				//	g_GrabImage.SaveImage(szCaptionProcImage,INTERNAL_PROC_IMAGE,ctrlImgProg);
					g_GrabImage.SaveJpgImage(szCaptionImage,SNAP_IMAGE,ctrlImgProg);
					g_GrabImage.SaveJpgImage(szCaptionProcImage,INTERNAL_PROC_IMAGE,ctrlImgProg);
				}
			}

			//+change kwmoon 071224
		//	if(nResult == TEST_ABORT) return 100;
			if(nResult == TEST_ABORT) break;
			
			if(nResult == TEST_PASS)
			{
				nNoBlackScreen++;
			}

			//+change kwmoon 071224
		//	if(nNoBlackScreen == 2) return 0.0;
			if(nNoBlackScreen == 2) break;
		}
		_Wait(200);
		
		dwElapsedTime = GetTickCount()-dwStartTick;

		if(dwElapsedTime > (DWORD)nCaptionCheckTime)
		{
			break;
		}

		nFrameNo++;
	}

	//+change kwmoon 080110
	//+add kwmoon 071224
	// Set Caption Mode Off
//	for(int i=0; i<2; i++)
	for(int i=0; i<nNoOffKeyPress; i++)
	{
		//+2007.12.26 Mod BY USY
		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "39"))
		{
			return FALSE;
		}
//		AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum("39"));
		//-

		if(!_Wait(nCaptionKeyDelay)) return 100;
	}

	//+2007.12.26 Mod BY USY
	if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "44"))
	{
		return FALSE;
	}
//	AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE,hexCstr2decNum("44"));
	//-
	if(!_Wait(nCaptionEnterKeyDelay)) return 100;

	//+change kwmoon 071224
//	return 100;
	if(nNoBlackScreen == 2)
	{
		return 0.0;
	}
	else
	{
		return 100;
	}
}

//+add kwmoon 071108
#define NO_GRAB_FOR_DIGITAL_EYE_TEST		15

double _Digital_Eye_Test()
{
	BOOL bReturn = FALSE;

	CString szString = _T("");
	CString szDigitalEyeZeroImage = _T("");
	CString szDigitalEyeZeroProcImage = _T("");
	CString szDigitalEyeNormalImage = _T("");
	CString szDigitalEyeNormalProcImage = _T("");

	int nNoLevel		= 255;
	int nLowerLimit		= g_ImageProc.Percentage2Level((float)pCurStep->m_fLowLimit,nNoLevel);
	int nUpperLimit		= g_ImageProc.Percentage2Level((float)pCurStep->m_fHighLimit,nNoLevel);

	double dDifference	= 0.0f;

	POINT ptRoi1Position;
	ptRoi1Position.x =  g_ImageProc.m_ptRoiPosition.x;
	ptRoi1Position.y =  g_ImageProc.m_ptRoiPosition.y;

	POINT ptRoi2Position;
	ptRoi2Position.x =  g_ImageProc.m_ptRoiPosition.x +  g_ImageProc.m_nRoiWidth;
	ptRoi2Position.y =  g_ImageProc.m_ptRoiPosition.y +  g_ImageProc.m_nRoiHeight;

	
	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	//==============
	// Set DIO Port
	//==============
	if(CurrentSet->bUseAVSwitchBox == TRUE)
	{
		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_DIGIT_OUT, MAX_AVSWITCH_WAIT_DELAY, 8, 0);

		//+change kwmoon 080724
	//	if(!bReturn) return 0.0;
		if(!bReturn) return 100; // Fail
	}

	//==================================
	// 1st Osd Test for DigitalEye Zero
	//==================================
	g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position);

	pCurStep->SetTestParm(NORMAL_PROCESS);

	dDifference = _Osd_Test();

	_Wait(g_ImageProc.m_nDigitalEyeDelay);

	if((dDifference < nLowerLimit) || (dDifference > nUpperLimit))
	{
		return dDifference; // Fail
	}

	//==================================
	// 2nd Osd Test for DigitalEye On
	//==================================
	Grab_LvdsImage_2in1();

	g_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position);

	pCurStep->SetTestParm(REVERSE_PROCESS);

	dDifference = _Osd_Test();

	if((dDifference < nLowerLimit) || (dDifference > nUpperLimit))	
	{
		return 0.0; // Pass
	}	
	else
	{
		return 100; // Fail
	}
}


//+ 2007.11.28 Add BY USY
double _Adc_Test()
{
	int nResult = TEST_FAIL;
	int nNoTest = TVCommCtrl.m_aCmd1.GetSize();

	CString szErrMsg;  //PSH
	CString szString = _T("");
	g_pView->m_bAdcRunning = TRUE;

	//==============
	// Using RS232C  
	//==============
	if(CurrentSet->nAdcType == ADC_TYPE_RS232C)
	{
		if(!CurrentSet->bUseTVCommPort)
		{
			szErrMsg.LoadString(IDS_COMMERROR);
			szErrMsg = szErrMsg + " (TV COM Port)";
			AfxMessageBox(szErrMsg);
			return 0.0;
		}

		if(TVCommCtrl.m_bPortOpen == FALSE)
		{
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				TVCommCtrl.PortClose();
				if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
				{
					szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
					AfxMessageBox(szErrMsg);
					return 0.0;
				}
			}
		}  

		for(int i=0; i<nNoTest; i++)
		{
			nResult = TVCommCtrl.AdcTest(TVCommCtrl.m_aCmd1.GetAt(i),
										 TVCommCtrl.m_aCmd2.GetAt(i),
										 TVCommCtrl.m_aSetId.GetAt(i),
										 TVCommCtrl.m_aData.GetAt(i),
										 TVCommCtrl.m_aCheckType.GetAt(i),
										 TVCommCtrl.m_aWaitLimit.GetAt(i),
										 TVCommCtrl.m_aCheckOption.GetAt(i));

			if(nResult == TEST_TIMEOUT)
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC TIMEOUT");
				TVCommCtrl.ResetArray();
				g_pView->m_bAdcRunning = FALSE;
				CurrentSet->bRunAbort = 1;
				return 0.0;
			}
			else if(nResult == TEST_FAIL)
			{
			//	InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC FAIL");
				TVCommCtrl.ResetArray();
				g_pView->m_bAdcRunning = FALSE;
				
				return 0.0;
			}
			_Wait(200);
		}

		TVCommCtrl.ResetArray();
		g_pView->m_bAdcRunning = FALSE;
	//	InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC_PASS");
		
		return 1.0;
	}
	//===========
	// Using I2C 
	//===========
	else
	{
		if(!pCurStep->m_bAdcTestResult)
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC FAIL");
			g_pView->m_bAdcRunning = FALSE;
			
			return 0.0;
		}
		_Wait(200);	

		g_pView->m_bAdcRunning = FALSE;
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "ADC_PASS");
		
		return 1.0;	
	}
}

//+add PSH 080417
/*
double _ADC_AdjConfirmation()
{
	if(TVCommCtrl.ADC_AdjConfirmation())
	{
		return 1.0;	
	}
	else{
		return 0.0;	
	}
}
*/
//+change PSH 091124
double _ADC_AdjResultCheck()
{

	if(TVCommCtrl.ADC_Adj_ResultCheck(5000))
	{
		return 1.0;	
	}
	else{
		return 0.0;	
	}
}
/*
double _EDID_DownloadResultCheck()
{
	int nAdjItem;

	TVCommCtrl.EDID_Download_ResultCheck(nAdjItem);

	return (double)nAdjItem;	

}
*/
//+add kwmoon 080722
double _Color_Bias_Test()
{
	int nResult = TEST_FAIL;

	CString szString  = _T("");
	CString szPrevMsg = _T("");
	CString szMsg1	  = _T("");
	
	CString sMsg	  = _T("");
	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	int   nColorDepth  = 0;
	int   nNoLevel	   = 0;
	int	  nMargin	   = 0;
	int	  nDifference  = 0;
	float fReturnPercentageValue = 0;

	if(CurrentSet->bSaveProcessingTimeData)
	{
		sMsg.Format("  B-ColorBiasTest");
		AddTimeData(sMsg);
		dwFuncTickCount = GetTickCount();
	}
	//090324
	pCurStep->m_bRunVideoTest = TRUE;

	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);

	if(g_GrabImage.m_bImageLoaded)
	{
		nResult = g_ImageProc.ColorBiasTest(g_GrabImage,nDifference,szMsg1,ctrlImgProg);
		
		pCurStep->m_nTestType = BIAS_TEST;
	
		nColorDepth	 = g_GrabImage.m_InfoHeader.biBitCount;

		//+change kwmoon 080722
	//	nNoLevel	 = (int)pnow(2,(nColorDepth/3));
	//	nNoLevel	 = (int)pow(2,(nColorDepth/3))-1;

		//+change kwmoon 080411
		if(CurrentSet->bSaveGrabImg
		||(!CurrentSet->bSaveGrabImg && (nResult == TEST_FAIL)))
		{
			if(pCurStep->m_strGrabPath == _T(""))
			{
				pCurStep->m_strGrabPath = g_GrabImage.m_szFilePath;
			}
		//	g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
			g_GrabImage.SaveJpgImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
		}
		
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		

		fReturnPercentageValue = (float)nDifference;//g_ImageProc.Level2Percentage(nDifference,nNoLevel);

		if(nResult == TEST_ABORT) return 255;//MAX_COLOR_LEVEL;
	}
	else
	{
		AfxMessageBox("Failed to load grab image"); return 255; 
	}

	return fReturnPercentageValue; 	
}

double	_Version_Check()
{
	CString sCpuVersion   = _T("");
	CString sMicomVersion = _T("");
	CString sUsbVersion	  = _T("");
	CString sFrcVersion   = _T("");
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResult3 = TRUE;
	BOOL bResult4 = TRUE;
	int  nI2cResult;
	int i;
	int nData[3]   = {0,0,0};

	if(CurrentSet->nTVControlType)
	{
		for(i= 0; i<5; i++)
		{
			nI2cResult = I2cAdcCtrl.ReadVersion();
			if(nI2cResult == TEST_PASS) break;
		}

		if(nI2cResult != TEST_PASS)
		{
			return FALSE;
		}
		else
		{
			// change psh 100310
			nData[0] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH,2));
			nData[1] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 2,2));
			nData[2] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 4,2));
			if(nData[2] > 99)
			{					
				sCpuVersion.Format("%02d.%02d.%02X", nData[0], nData[1], nData[2]);
			}
			else{
				sCpuVersion.Format("%02d.%02d.%02d", nData[0], nData[1], nData[2]);
			}
		//	sCpuVersion = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH,6);
		//	sCpuVersion.Insert(2, '.');
		//	sCpuVersion.Insert(5, '.');

			nData[0] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 6,2));
			nData[1] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 8,2));
			nData[2] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 10,2));
			if(nData[2] > 99)
			{					
				sMicomVersion.Format("%02d.%02d.%02X", nData[0], nData[1], nData[2]);
			}
			else{
				sMicomVersion.Format("%02d.%02d.%02d", nData[0], nData[1], nData[2]);
			}

		//	sMicomVersion = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 6,6);
		//	sMicomVersion.Insert(2, '.');
		//	sMicomVersion.Insert(5, '.');

			nData[0] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 12,2));
			nData[1] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 14,2));
			nData[2] = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 16,2));
			if(nData[2] > 99)
			{					
				sUsbVersion.Format("%02d.%02d.%02X", nData[0], nData[1], nData[2]);
			}
			else{
				sUsbVersion.Format("%02d.%02d.%02d", nData[0], nData[1], nData[2]);
			}
		//	sUsbVersion = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 12,6);
		//	sUsbVersion.Insert(2, '.');
		//	sUsbVersion.Insert(5, '.');

//			sFrcVersion = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH + 18,6);
//			sFrcVersion.Insert(2, '.');
//			sFrcVersion.Insert(5, '.');
		}
		sCpuVersion.MakeUpper();
		
		if(CurrentSet->sCPUVersion.Left(1) != "0"){
			if(sCpuVersion.Left(1) == "0")
			{
				sCpuVersion.TrimLeft('0');
			}
		}

		if(sCpuVersion.Compare(CurrentSet->sCPUVersion) != 0){
			bResult1 = FALSE;
		}

		if(CurrentSet->sMicomVersion != "")
		{ 
			sMicomVersion.MakeUpper();

			if(CurrentSet->sMicomVersion.Left(1) != "0"){
				if(sMicomVersion.Left(1) == "0")
				{
					sMicomVersion.TrimLeft('0');
				}
			}
			
			if(sMicomVersion.Compare(CurrentSet->sMicomVersion) != 0){
				bResult2 = FALSE;
			}
		}
		if(CurrentSet->sUSBVersion != "")
		{ 
			sUsbVersion.MakeUpper(); 
			if(CurrentSet->sUSBVersion.Left(1) != "0"){
				if(sUsbVersion.Left(1) == "0")
				{
					sUsbVersion.TrimLeft('0');
				}
			}
			if(sUsbVersion.Compare(CurrentSet->sUSBVersion) != 0){
				bResult3 = FALSE;
			}
		}

	}
	else{
		bResult1 = TVCommCtrl.Firmware_VersionCheck(CPU_VER, CurrentSet->sCPUVersion);

		if(CurrentSet->sMicomVersion != "")
		{ 
			bResult2 = TVCommCtrl.Firmware_VersionCheck(MICOM_VER, CurrentSet->sMicomVersion);
		}

		if(CurrentSet->sUSBVersion != "")
		{ 
			bResult3 = TVCommCtrl.Firmware_VersionCheck(USB_VER, CurrentSet->sUSBVersion);
		}

		if(CurrentSet->sMNTVersion != "")
		{ 
			bResult4 = TVCommCtrl.Firmware_VersionCheck(MNT_VER, CurrentSet->sMNTVersion);
		}
	}
	
	if(bResult1 && bResult2 && bResult3 && bResult4)
	{
		return 1.0;	
	}
	else
	{
		return 0.0;	
	}
}

double	_MNT_Version_Check()
{
	CString sCpuVersion   = _T("");
	BOOL bResult1 = TRUE;
	int  nI2cResult;
	int i;
//	int nData[3]   = {0,0,0};

	if(!CurrentSet->nTVControlType)
	{
			return FALSE;
	}
	for(i= 0; i<5; i++)
	{
		nI2cResult = I2cAdcCtrl.MNT_ReadVersion();
		if(nI2cResult == TEST_PASS) break;
	}

	if(nI2cResult != TEST_PASS)
	{
		return FALSE;
	}
	else
	{
		CString szTemp;
		CString szData;
		szTemp = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH);
		if (szTemp.Find("82") == 0)
		{
			int lhighVersion = hex2dec(szTemp.GetAt(3));
			int lMidVersion = hex2dec(szTemp.GetAt(4));
			szData.Format("%02d%02d", lhighVersion, lMidVersion);
			szData.MakeUpper();
			sCpuVersion = szData;
		}
		else
		{
			sCpuVersion = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH, 4);
		}

		
		sCpuVersion.Insert(2, '.');
	}

	sCpuVersion.MakeUpper(); 

	if(sCpuVersion.Compare(CurrentSet->sCPUVersion) != 0){
		bResult1 = FALSE;
	}

	if(bResult1)
	{
		return 1.0;	
	}
	else
	{
		return 0.0;	
	}
}

double	_MNT_EDID_Check()
{
	CString sCpuVersion   = _T("");
	BOOL bResult1 = TRUE;
	int  nI2cResult;
	int i;
//	int nData[3]   = {0,0,0};

	if(!CurrentSet->nTVControlType)
	{
			return FALSE;
	}

	for(i= 0; i<5; i++)
	{
		nI2cResult = I2cAdcCtrl.MNT_EDID_Check();
		if(nI2cResult == TEST_PASS) break;
	}

	if(nI2cResult != TEST_PASS)
	{
		return FALSE;
	}
	else
	{
		bResult1 = TRUE;
	}

	

	if(bResult1)
	{
		return 1.0;	
	}
	else
	{
		return 0.0;	
	}
}


double	_MNT_HDCP_Check()
{
	CString sCpuVersion   = _T("");
	BOOL bResult1 = TRUE;
	int  nI2cResult;
	int i;
//	int nData[3]   = {0,0,0};

	if(!CurrentSet->nTVControlType)
	{
			return FALSE;
	}

	for(i= 0; i<5; i++)
	{
		nI2cResult = I2cAdcCtrl.MNT_HDCP_Check();
		if(nI2cResult == TEST_PASS) break;
	}

	if(nI2cResult != TEST_PASS)
	{
		return FALSE;
	}
	else
	{
		bResult1 = TRUE;
	}

	if(bResult1)
	{
		return 1.0;	
	}
	else
	{
		return 0.0;	
	}
}

#ifdef SM_MODIFY_CODE__
//2018.11.26 CYS
BOOL Check_ToolOption(int nOptionNo, int nToolOption)
{
	BOOL bResult = TRUE;
	//BOOL bResultTemp = TRUE;
	int  nI2cResult;
	int  i;

		
	if (CurrentSet->nTVControlType) {
		_Wait(100);
		bResult = I2cAdcCtrl.Check_Option(nOptionNo, nToolOption);
	}
	else {
		_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
		for (i = 0; i < 3; i++)
		{
			bResult = TVCommCtrl.Check_Option(nOptionNo, nToolOption);
			if (bResult) break;
			_Wait(200);
		}
	}
		
	
	return bResult;
}



#else
BOOL Check_ToolOption(int nOptionNo, int nToolOption)
{
	BOOL bResult = TRUE;
	BOOL bResultTemp;
	int  nI2cResult;
	int  i;

	if(CurrentSet->nTVControlType){
		_Wait(100);
		bResult = I2cAdcCtrl.Check_Option(nOptionNo, nToolOption);
	}
	else{
		_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
		for(i=0; i<3; i++)
		{
			bResult = TVCommCtrl.Check_Option(nOptionNo, nToolOption);
			if(bResult) break;
			_Wait(200);
		}
	}
	if(!bResult){
		if(CurrentSet->nTVControlType){
			for(i=0; i<5; i++)
			{
				nI2cResult = I2cAdcCtrl.WriteOption(nOptionNo, 1, nToolOption);
				if(nI2cResult == TEST_PASS){bResultTemp = TRUE; break;}
				_Wait(100);
			}
			if(nI2cResult != TEST_PASS)
			{
				bResultTemp = FALSE;
			}
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResultTemp = TVCommCtrl.Write_Option(nOptionNo, nToolOption);
				if(bResultTemp) break;
				_Wait(200);
			}
		}
		if(bResultTemp)
		{
			if(CurrentSet->nTVControlType){
				_Wait(100);
				bResult = I2cAdcCtrl.Check_Option(nOptionNo, nToolOption);
			}
			else{
				_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
				for(i=0; i<3; i++)
				{
					bResult = TVCommCtrl.Check_Option(nOptionNo, nToolOption);
					if(bResult) break;
					_Wait(200);
				}
			}
		}
	}
	return bResult;
}



#endif



double	_Option_Check()
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
	int nToolOption;

	if(CurrentSet->sToolOption1 != ""){
		nToolOption = atoi(CurrentSet->sToolOption1);
		bResult1 = Check_ToolOption(TOOL_OPTION1, nToolOption);
	}

	if(CurrentSet->sToolOption2 != ""){
		nToolOption = atoi(CurrentSet->sToolOption2);
		bResult2 = Check_ToolOption(TOOL_OPTION2, nToolOption);
	}
	
	if(CurrentSet->sToolOption3 != ""){
		nToolOption = atoi(CurrentSet->sToolOption3);
		bResult3 = Check_ToolOption(TOOL_OPTION3, nToolOption);
	}

	if(CurrentSet->sToolOption4 != ""){
		nToolOption = atoi(CurrentSet->sToolOption4);
		bResult4 = Check_ToolOption(TOOL_OPTION4, nToolOption);
	}

	if(CurrentSet->sToolOption5 != ""){
		nToolOption = atoi(CurrentSet->sToolOption5);
		bResult5 = Check_ToolOption(TOOL_OPTION5, nToolOption);
	}

	if(CurrentSet->sToolOption6 != ""){
		nToolOption = atoi(CurrentSet->sToolOption6);
		bResult6 = Check_ToolOption(TOOL_OPTION6, nToolOption);
	}

	if(CurrentSet->sToolOption7 != ""){
		nToolOption = atoi(CurrentSet->sToolOption7);
		bResult7 = Check_ToolOption(TOOL_OPTION7, nToolOption);
	}

	if(CurrentSet->sToolOption8 != ""){
		nToolOption = atoi(CurrentSet->sToolOption8);
		bResult8 = Check_ToolOption(TOOL_OPTION8, nToolOption);
	}
	if(CurrentSet->sCommercialOption1 != ""){
		nToolOption = atoi(CurrentSet->sCommercialOption1);
		bResult9 = Check_ToolOption(COMMERCIAL_OPTION1, nToolOption);
	}

	if(bResult1 && bResult2 && bResult3  && bResult4  && bResult5  && bResult6  && bResult7 && bResult8 && bResult9)
	{
		return 1.0;	

	}
	else{
		return 0.0;	
	}

}

double	_Option_CheckAll()
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	int nToolOption1 = 0;
	int nToolOption2 = 0;
	int nToolOption3 = 0;
	int nToolOption4 = 0;
	int nToolOption5 = 0;
	int nToolOption6 = 0;
	int nToolOption7 = 0;
	int nAreaOption;

	if(CurrentSet->sToolOption1 != ""){
		nToolOption1 = atoi(CurrentSet->sToolOption1);
	}

	if(CurrentSet->sToolOption2 != ""){
		nToolOption2 = atoi(CurrentSet->sToolOption2);
	}
	
	if(CurrentSet->sToolOption3 != ""){
		nToolOption3 = atoi(CurrentSet->sToolOption3);
	}

	if(CurrentSet->sToolOption4 != ""){
		nToolOption4 = atoi(CurrentSet->sToolOption3);
	}

	if(CurrentSet->sToolOption5 != ""){
		nToolOption5 = atoi(CurrentSet->sToolOption5);
	}

	if(CurrentSet->sToolOption6 != ""){
		nToolOption6 = atoi(CurrentSet->sToolOption6);
	}

	if(CurrentSet->sToolOption7 != ""){
		nToolOption7 = atoi(CurrentSet->sToolOption7);
	}

	if(CurrentSet->nTVControlType){
		bResult1 = I2cAdcCtrl.Check_OptionAll(nToolOption1, nToolOption2, nToolOption3, nToolOption4, nToolOption5, nToolOption6);
	}
	else{
		bResult1 = TVCommCtrl.Check_OptionAll(nToolOption1, nToolOption2, nToolOption3, nToolOption4, nToolOption5, nToolOption6, nToolOption7);
	}


	if(CurrentSet->sAreaOption1 != ""){
		nAreaOption = atoi(CurrentSet->sAreaOption1);
		if(CurrentSet->nTVControlType){
			bResult2 = I2cAdcCtrl.Check_Option(AREA_OPTION1, nAreaOption);
		}
		else{
			bResult2 = TVCommCtrl.Check_Option(AREA_OPTION1, nAreaOption);
		}
	}

	if(bResult1 && bResult2)
	{
		return 1.0;	

	}
	else{
		return 0.0;	
	}

}

double	_AreaCode_Check()
{
	BOOL bResult = TRUE;
	BOOL bResultTemp;
	int  nI2cResult;
	int  i;

	int nToolOption;

	if(CurrentSet->sAreaOption1 != ""){
		nToolOption = atoi(CurrentSet->sAreaOption1);
		if(CurrentSet->nTVControlType){
			_Wait(100);
			bResult = I2cAdcCtrl.Check_Option(AREA_OPTION1, nToolOption);
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult = TVCommCtrl.Check_Option(AREA_OPTION1, nToolOption);
				if(bResult) break;
				_Wait(200);
			}
		}
		if(!bResult){
			if(CurrentSet->nTVControlType){
				for(i=0; i<5; i++)
				{
					nI2cResult = I2cAdcCtrl.WriteOption(AREA_OPTION, 1, nToolOption);
					if(nI2cResult == TEST_PASS){bResultTemp = TRUE; break;}
					_Wait(100);
				}
				if(nI2cResult != TEST_PASS)
				{
					bResultTemp = FALSE;
				}
			}
			else{
				_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
				for(i=0; i<3; i++)
				{
					bResultTemp = TVCommCtrl.Write_Option(AREA_OPTION1, nToolOption);
					if(bResultTemp) break;
					_Wait(200);
				}
			}
			if(bResultTemp)
			{
				if(CurrentSet->nTVControlType){
					_Wait(100);
					bResult = I2cAdcCtrl.Check_Option(AREA_OPTION1, nToolOption);
				}
				else{
					_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
					for(i=0; i<3; i++)
					{
						bResult = TVCommCtrl.Check_Option(AREA_OPTION1, nToolOption);
						if(bResult) break;
						_Wait(200);
					}
				}
			}
		}
	}

	if(bResult)
	{
		return 1.0;	

	}
	else{
		return 0.0;	
	}

}

double	_CountryGr_Check()
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	BOOL bResultTemp;
//	int  nI2cResult;
//	int  i;
	CString szErrMsg;
	CString sData;
	int nData;
	int nCountryGr;
	int nCountry;
	int i;

	if(CurrentSet->sCountryGroup != ""){
		nCountryGr = Get_CountryGroup_Code(CurrentSet->sCountryGroup);

		if(nCountryGr == 0){
			szErrMsg.Format("Failed CountryGroup (%s) - Code Not",CurrentSet->sCountryGroup);
			AfxMessageBox(szErrMsg);
			return 0.0;
		}
		if(CurrentSet->nTVControlType){
			bResult1 = FALSE;
		}
		else{
			for(i=0; i<3; i++)
			{
				bResult1 = TVCommCtrl.Check_Option(COUNTRY_GROUP, nCountryGr);
				if(bResult1) break;
				_Wait(200);
			}
		}
		if(!bResult1){
			if(CurrentSet->nTVControlType){
				bResultTemp = FALSE;
			}
			else{
				_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
				for(i=0; i<3; i++)
				{
					bResultTemp = TVCommCtrl.Write_Option(COUNTRY_GROUP, nCountryGr);
					if(bResultTemp) break;
					_Wait(200);
				}
			}
			if(bResultTemp)
			{
				if(CurrentSet->nTVControlType){
					bResult1 = FALSE;
				}
				else{
					_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
					for(i=0; i<3; i++)
					{
						bResult1 = TVCommCtrl.Check_Option(COUNTRY_GROUP, nCountryGr);
						if(bResult1) break;
						_Wait(200);
					}
				}
			}
		}
	}

	if(CurrentSet->sCountry != ""){
		_Wait(300);
		nCountry = Get_Country_Code(CurrentSet->sCountry);
		if(nCountry == 0){
			szErrMsg.Format("Failed Country (%s) - Code Not",CurrentSet->sCountry);
			AfxMessageBox(szErrMsg);
			return 0.0;
		}
	
		sData.Format("%02x%06x", nCountryGr, nCountry);

		nData = hexCstr2decNum(sData);

		if(CurrentSet->nTVControlType){
			bResult2 = FALSE;
		}
		else{
			_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
			for(i=0; i<3; i++)
			{
				bResult2 = TVCommCtrl.Check_Option(COUNTRY, nData);
				if(bResult2) break;
				_Wait(200);
			}
		}
		if(!bResult2){
			if(CurrentSet->nTVControlType){
				bResultTemp = FALSE;
			}
			else{
				_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
				for(i=0; i<3; i++)
				{
					bResultTemp = TVCommCtrl.Write_Option(COUNTRY, nData);
					if(bResultTemp) break;
					_Wait(200);
				}
			}
			if(bResultTemp)
			{
				if(CurrentSet->nTVControlType){
					bResult2 = FALSE;
				}
				else{
					_Wait(CurrentSet->nToolOptionCheck_Delay * 100);
					for(i=0; i<3; i++)
					{
						bResult2 = TVCommCtrl.Check_Option(COUNTRY, nData);
						if(bResult2) break;
						_Wait(200);
					}
				}
			}
		}
	}

	if(bResult1 && bResult2)
	{
		return 1.0;	

	}
	else{
		return 0.0;	
	}

}

//+add 080216(Modification No1)
double _Module_Voltage_Test()
{
	double fMeasuredValue = 0.0;
	int nIndex;
	CString sTemp;
	BOOL bResult = FALSE;

	for(nIndex = 0; nIndex < 10; nIndex++)
	{
		
		bResult = I2cAdcCtrl.GetModuleVoltage(fMeasuredValue);

		if (pCurStep->GetHighLimit() < fMeasuredValue || pCurStep->GetLowLimit() > fMeasuredValue) 
		{
			_Wait(1000);
		}
		else{
			break;
		}
		//090511
		if(CurrentSet->bRunAbort)
		{
			break;
		}
	}

	pCurStep->m_nTestType = MODULE_TEST;//gPLC_Ctrl.m_nFirstVideoNG == 1

	if (pCurStep->GetHighLimit() < fMeasuredValue || pCurStep->GetLowLimit() > fMeasuredValue)
	{
		if(bResult){
			sTemp.Format("T:%f,  M:%f", pCurStep->GetNominal_float(), fMeasuredValue);
		}
		else{
			sTemp.Format("T:%f,  M: Error-I2C Response Data", pCurStep->GetNominal_float());
		}
		pCurStep->m_strMsg = sTemp;
	}
	return fMeasuredValue; 	
}

//+add 090304(Modification No1)
double _EDID_Download()
{
	CString szErrMsg;  //PSH

	if(CurrentSet->nHDMIGenType == 1){
		if(!HDMIGeneratorCtrl.SetDDCLine_OpenClose(TRUE)){
			InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
			if(!HDMIGeneratorCtrl.SetDDCLine_OpenClose(TRUE)){
				return 0.0;
			}
		}
		_Wait(500);
	}

	if(CurrentSet->nTVControlType){
		if(I2cAdcCtrl.EDID_Download() != TEST_PASS)
		{
			return 0.0;
		}
	}
	else{
		if(TVCommCtrl.m_bPortOpen == FALSE)
		{
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				TVCommCtrl.PortClose();
				if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
				{
					szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
					AfxMessageBox(szErrMsg);
					return 0.0;
				}
			}
		} 
		
		if(!TVCommCtrl.EDID_Download(8000)){
			return 0.0;
		}
	}

	if(CurrentSet->nHDMIGenType == 1){
		if(!HDMIGeneratorCtrl.SetDDCLine_OpenClose(FALSE)){
			InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
			if(!HDMIGeneratorCtrl.SetDDCLine_OpenClose(FALSE)){
				return 0.0;
			}
		}
		_Wait(500);

		if(!HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE)){
			InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
			if(!HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE)){
				return 0.0;
			}
		}

		_Wait(500);
		if(!HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE)){
			InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
			if(!HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE)){
				return 0.0;
			}
		}
	}
	
	return 1.0;
}

//+add 090325(Modification No1)
double _LAN_Port_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.LAN_Port_Check(8000)){
		return 0.0;
	}
	
	return 1.0;
}
double _ESN_Download_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.ESN_Download_Check(5000)){
		return 0.0;
	}
	
	return 1.0;
}

//add 090527
double _Domming_Check()
{
	CString szString  = _T("");
	CString szPrevMsg = _T("");
	CString szMsg1	  = _T("");
	int nReturn;

	nReturn = g_pView->LocalDimmingTest();
	
	switch(nReturn)
	{
		case 0:
			szMsg1.Format("Dimming Check : OK");
			break;
		case 1:	
			szMsg1.Format("Dimming Check : Address mis-match");
			break;
		case 2:	
			szMsg1.Format("Dimming Check : Command mis-match");
			break;
		case 3:
			szMsg1.Format("Dimming Check : Checksum mis-match");
			break;
		case -1:
			szMsg1.Format("Dimming Check : Error");
			break;
	}

	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
	
	if(szPrevMsg != _T(""))
	{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
		pCurStep->m_strMsg = szMsg1;
	}		


	if(nReturn == 0)
	{
		return 1.0;
	}
	else
	{
		return 0.0;
	}
}

//add 091118
double _CI_Key_Check()
{
	CString szErrMsg;  //PSH
	BOOL bResult1;
//	BOOL bResult2;

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	bResult1 = TVCommCtrl.CI_Download_Check1(3000);
// del 20101108
//	bResult2 = TVCommCtrl.CI_Download_Check2(3000);

//	if(bResult1 && bResult2){
	if(bResult1){
		return 1.0;
	}
	else{
		return 0.0;
	}
}
//add 101011
double _CI_Card_Check()
{
	CString szErrMsg;  //PSH
	BOOL bResult;

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	bResult = TVCommCtrl.CI_Card_Check(3000);

	if(bResult){
		return 1.0;
	}
	else{
		return 0.0;
	}
}
double _Marlin_Download_Check()
{
	CString szErrMsg;  //PSH
	BOOL bResult;

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	bResult = TVCommCtrl.Marlin_Download_Check(3000);

	if(bResult){
		return 1.0;
	}
	else{
		return 0.0;
	}
}
double _BuiltIn_Wifi_Check()
{
	CString szErrMsg;  //PSH
	BOOL bResult;

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	bResult = TVCommCtrl.BuiltIn_Wifi_Check(3000);

	if(bResult){
		return 1.0;
	}
	else{
		return 0.0;
	}
}

//+add 091124(Modification No1)
double _RJP_Port_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.RJP_Port_Check(4000)){
		return 0.0;
	}
	
	return 1.0;
}

//+add 091124(Modification No1)
double _Magic_Remocon_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.Magic_Remocon_Check(2000)){
		return 0.0;
	}
	
	return 1.0;
}

//+add 091124(Modification No1)
double _Wireless_Ready_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.Wireless_Ready_Check(8000)){
		return 0.0;
	}
	
	return 1.0;
}

double _DMM_MeasureDC()
{
	double dVol;
	//CString sVol;
	//CString sUnit;

	//dVol = g_pView->GetDaqData(DAQ_FUNC_DC, CurrentSet->nDmmMeasureCh_S, CurrentSet->nDmmMeasureCh_S,sVol);

	//sUnit = pCurStep->GetUnit();
	//sUnit.TrimLeft();
	//sUnit.TrimRight();
	//sUnit.MakeUpper();
	//if(sUnit == "MV")
	//	dVol = dVol*1000.;

	return dVol;
}

double _DMM_MeasureAC()
{
	double dVol;
	//CString sVol;
	//CString sUnit;

	//dVol = g_pView->GetDaqData(DAQ_FUNC_AC, CurrentSet->nDmmMeasureCh_S, CurrentSet->nDmmMeasureCh_S,sVol);

	//sUnit = pCurStep->GetUnit();
	//sUnit.TrimLeft();
	//sUnit.TrimRight();
	//sUnit.MakeUpper();
	//if(sUnit == "MV")
	//	dVol = dVol*1000.;

	return dVol;
}

double _DMM_MeasureResistance()
{
	double dVol;
	CString sVol;
	CString sUnit;

	dVol = g_pView->GetDaqData(DAQ_FUNC_RES, CurrentSet->nDmmMeasureCh_S, CurrentSet->nDmmMeasureCh_S,sVol);

	sUnit = pCurStep->GetUnit();
	sUnit.TrimLeft();
	sUnit.TrimRight();
	sUnit.MakeUpper();

	if(sUnit == "KOHM")
		dVol = dVol/1000.;
	else if(sUnit == "MOHM")
		dVol = dVol/1000000.;

	return dVol;
}

double _DMM_MeasureFrequency()
{
	double dVol;
	CString sVol;
	CString sUnit;

	dVol = g_pView->GetDaqData(DAQ_FUNC_FREQ, CurrentSet->nDmmMeasureCh_S, CurrentSet->nDmmMeasureCh_S, sVol);

	sUnit = pCurStep->GetUnit();
	sUnit.TrimLeft();
	sUnit.TrimRight();
	sUnit.MakeUpper();

	if(sUnit == "KHZ")
		dVol = dVol/1000.;
	else if(sUnit == "MHZ")
		dVol = dVol/1000000.;

	return dVol;
}

double _DMM_DischargCheck()
{
	double dVol;
	CString sVol;

	dVol = g_pView->GetDaqData(DAQ_FUNC_DC, CurrentSet->nDmmMeasureCh_S, CurrentSet->nDmmMeasureCh_S,sVol);

	for(int i = 0; i < 40; i ++)
	{
		if(dVol < GetHighLimit()){break;}
		Sleep(100);
		dVol = g_pView->GetDaqData(DAQ_FUNC_DC, CurrentSet->nDmmMeasureCh_S, CurrentSet->nDmmMeasureCh_S,sVol);
	}
	return dVol;
}

double _VF_MeasureVolt()
{
	BOOL bRev = FALSE;
	double dVol = 0.0;

	bRev = gVF1000Ctrl.GetVoltageData(gVF1000Ctrl.m_nChNo, dVol);
	if(!bRev){
		return 0.0;
	}

	return dVol;
}

double _VF_MeasureFreq()
{
	BOOL bRev = FALSE;
	double dVol = 0.0;
	CString sUnit;

	bRev = gVF1000Ctrl.GetFrequencyData(gVF1000Ctrl.m_nChNo, dVol);
	if(!bRev){
		return 0.0;
	}

	sUnit = pCurStep->GetUnit();
	sUnit.TrimLeft();
	sUnit.TrimRight();
	sUnit.MakeUpper();

	if(sUnit == "KHZ"){
		dVol = dVol/1000.;
	}

	return dVol;
}

double _VF_MeasureVolt_MinMax()
{
	BOOL		bResult = FALSE;
	BOOL		bRev = FALSE;
	UINT		nChNo;		
	double		dSpec1;
	double		dSpec2;
	double		dLowerLimit1;
	double		dUpperLimit1;
	double		dLowerLimit2;
	double		dUpperLimit2;
	double		dMargin = 0.2;

	double dVol = 0.0;
	double dVol2 = 0.0;
	int nTime = 0;
	CString sMeasur;

	nChNo = gVF1000Ctrl.m_nChNo;
	dSpec1 = gVF1000Ctrl.m_dSpec1;
	dSpec2 = gVF1000Ctrl.m_dSpec2;
	nTime = gVF1000Ctrl.m_nCheckTime;

//	if(nChNo == 34 || nChNo ==35 || nChNo ==36 || nChNo ==101 || nChNo ==102 || nChNo ==103){
//	if(nChNo == 34 || nChNo ==35 || nChNo ==36){
//		dMargin = 0.1;
//	}
//	else{
//		dMargin = 0.2;
//	}

	if(nChNo == 34 || nChNo ==35 || nChNo ==36 || nChNo ==104 || nChNo ==105 || nChNo ==106){
		dMargin = 0.1;
	}
	else if(nChNo ==101 || nChNo ==102 || nChNo ==103){
		dMargin = 0.3;
	}
	else{
		dMargin = 0.2;
	}

	dLowerLimit1 = dSpec1 - (abs(dSpec1) * dMargin);
	dUpperLimit1 = dSpec1 + (abs(dSpec1) * dMargin);
	dLowerLimit2 = dSpec2 - (abs(dSpec2) * dMargin);
	dUpperLimit2 = dSpec2 + (abs(dSpec2) * dMargin);


	bRev = gVF1000Ctrl.GetVoltageData2(nChNo, nTime, dVol, dVol2);
	if(bRev){
//		if((dVol >= dLowerLimit1 && dVol <= dUpperLimit1) && (dVol2 >= dLowerLimit2 && dVol2 <= dUpperLimit2)){
		if((dVol <= dSpec1) && (dVol2 >= dSpec2)){
			bResult = TRUE;
		}
		else{
			bResult = FALSE;
		}
		sMeasur.Format(_T("%.1f,%.1f"), dVol, dVol2);
	}
	else{
		bResult = FALSE;
		sMeasur.Format(_T("error"));
	}

	CString szPrevMsg = _T("");

	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
	if(szPrevMsg != _T(""))
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + sMeasur);
		pCurStep->m_strMsg = szPrevMsg + "; " + sMeasur;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMeasur);
		pCurStep->m_strMsg = sMeasur;
	}
	if(bResult){
		return 1.0;
	}
	else{
		return 0.0;
	}
}

double _VF_MeasureFreq_MinMax()
{
	BOOL		bResult = FALSE;
	BOOL		bRev = FALSE;
	UINT		nChNo;		
	double		dSpec1;
	double		dSpec2;
	double		dLowerLimit1;
	double		dUpperLimit1;
	double		dLowerLimit2;
	double		dUpperLimit2;
	double		dMargin = 0.2;

	double dVol = 0.0;
	double dVol2 = 0.0;
	int nTime = 0;
	CString sMeasur;

	nChNo = gVF1000Ctrl.m_nChNo;
	dSpec1 = gVF1000Ctrl.m_dSpec1;
	dSpec2 = gVF1000Ctrl.m_dSpec2;
	nTime = gVF1000Ctrl.m_nCheckTime;

//	if(nChNo == 34 || nChNo ==35 || nChNo ==36 || nChNo ==101 || nChNo ==102 || nChNo ==103){
//	if(nChNo == 34 || nChNo ==35 || nChNo ==36){
//		dMargin = 0.1;
//	}
//	else{
//		dMargin = 0.2;
//	}
	if(nChNo == 34 || nChNo ==35 || nChNo ==36 || nChNo ==104 || nChNo ==105 || nChNo ==106){
		dMargin = 0.1;
	}
	else if(nChNo ==101 || nChNo ==102 || nChNo ==103){
		dMargin = 0.3;
	}
	else{
		dMargin = 0.2;
	}

	dLowerLimit1 = dSpec1 - (abs(dSpec1) * dMargin);
	dUpperLimit1 = dSpec1 + (abs(dSpec1) * dMargin);
	dLowerLimit2 = dSpec2 - (abs(dSpec2) * dMargin);
	dUpperLimit2 = dSpec2 + (abs(dSpec2) * dMargin);


	bRev = gVF1000Ctrl.GetFrequencyData2(nChNo, nTime, dVol, dVol2);
	if(bRev){
	//	if((dVol >= dLowerLimit1 && dVol <= dUpperLimit1) && (dVol2 >= dLowerLimit2 && dVol2 <= dUpperLimit2)){
		if((dVol <= dSpec1) && (dVol2 >= dSpec2)){
		bResult = TRUE;
		}
		else{
			bResult = FALSE;
		}
		sMeasur.Format(_T("%.1f,%.1f"), dVol, dVol2);
	}
	else{
		bResult = FALSE;
		sMeasur.Format(_T("error"));
	}

	CString szPrevMsg = _T("");

	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
	if(szPrevMsg != _T(""))
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + sMeasur);
		pCurStep->m_strMsg = szPrevMsg + "; " + sMeasur;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMeasur);
		pCurStep->m_strMsg = sMeasur;
	}
	if(bResult){
		return 1.0;
	}
	else{
		return 0.0;
	}
}

double _VF_DischargCheck()
{
	int nNoFailed = 0;

	CVFDischargeDlg *pDlg = new CVFDischargeDlg;

	pDlg->DoModal();

	nNoFailed = pDlg->m_nNoFailed;

	delete pDlg;

	if (nNoFailed == 0) {
		return 1.0;
	}
	else {
		return 0.0;
	}
}


double _Widevine_Download_Check()
{
	CString szErrMsg;  //PSH
	BOOL bResult;

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	bResult = TVCommCtrl.Widevine_Download_Check(3000);

	if(bResult){
		return 1.0;
	}
	else{
		return 0.0;
	}
}

//add 101115
double _RF_Emitter_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.RF_Emitter_Check(2000)){
		return 0.0;
	}
	
	return 1.0;
}
//add 1110215
double _MAC_Address_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.MAC_AddressCheck(3000)){
		return 0.0;
	}
	
	return 1.0;
}
double _USB_HighSpeed_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.USB_HighSpeedCheck(3000)){
		return 0.0;
	}
	
	return 1.0;
}


double _BCM_Chip_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.BCMTest(90000)){
		return 1.0;
	}
	else{
		return 0.0;
	}
}
double _Key1_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.Key_Check(1)){
		return 0.0;
	}
	
	return 1.0;
}
double _Key2_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.Key_Check(2)){
		return 0.0;
	}
	
	return 1.0;
}
double _EyeQ_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.EyeQ_Check(3000)){
		return 0.0;
	}
	
	return 1.0;
}


double _BAT_Ver_Check()
{
	CString szErrMsg;  //PSH

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return 0.0;
			}
		}
	}  
	if(!TVCommCtrl.BAT_Ver_Check(CurrentSet->sBatVersion, 3000)){//if(!TVCommCtrl.EyeQ_Check(3000)){
		return 0.0;
	}
	
	return 1.0;
}
double _CNTV_Key_DL_Check()
{
	if(!TVCommCtrl.CNTV_Key_DL_Check()){
		return 0.0;
	}
	
	return 1.0;
}
double _HDCP_Tx_Key_DL_Check()
{
	if(!TVCommCtrl.HDCP_Tx_20_DL_Check()){
		return 0.0;
	}
	
	return 1.0;
}
double _HDCP_Rx_Key_DL_Check()
{
	if(!TVCommCtrl.HDCP_Rx_20_DL_Check()){
		return 0.0;
	}
	
	return 1.0;
}

double _DTCP_Rx_Key_DL_Check()
{
	if(!TVCommCtrl.DTCP_Rx_Key_DL_Check()){
		return 0.0;
	}
	
	return 1.0;
}



double _ATSC_30_Key_DL_Check()
{
	if(!TVCommCtrl.ATSC_30_Key_DL_Check()){
		return 0.0;
	}
	
	return 1.0;
}


double _MFi_Key_DL_Check()
{
	if(!TVCommCtrl.MFi_Key_DL_Check()){
		return 0.0;
	}
	
	return 1.0;
}



double _Google_Key_DL_Check()
{
	if(!TVCommCtrl.Google_Key_DL_Check()){
		return 0.0;
	}
	
	return 1.0;
}

double	_CountryGr_BR_Write()
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	bResult1 = TVCommCtrl.Write_Option(COUNTRY_GROUP, 0x03);
	if(!bResult1){
		bResult1 = TVCommCtrl.Write_Option(COUNTRY_GROUP, 0x03);
	}
	if(!bResult1){
		return 0.0;
	}
	bResult2 = TVCommCtrl.Check_Option(COUNTRY_GROUP, 0x03);
	if(!bResult2){
		return 0.0;
	}

	return 1.0;

}

double	_CountryGr_KR_Write()
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	bResult1 = TVCommCtrl.Write_Option(COUNTRY_GROUP, 0x01);
	if(!bResult1){
		bResult1 = TVCommCtrl.Write_Option(COUNTRY_GROUP, 0x01);
	}
	if(!bResult1){
		return 0.0;
	}
	bResult2 = TVCommCtrl.Check_Option(COUNTRY_GROUP, 0x01);
	if(!bResult2){
		return 0.0;
	}

	return 1.0;

}
double	_CountryGr_JA_Write()
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;

	bResult1 = TVCommCtrl.Write_Option(COUNTRY_GROUP, 0x14);
	if(!bResult1){
		bResult1 = TVCommCtrl.Write_Option(COUNTRY_GROUP, 0x14);
	}
	if(!bResult1){
		return 0.0;
	}
	bResult2 = TVCommCtrl.Check_Option(COUNTRY_GROUP, 0x14);
	if(!bResult2){
		return 0.0;
	}

	return 1.0;

}

/*
double _HdcpKey_Download() 
{
	int i;

	if(GetHdcpKeyData(CurrentSet->sHdcpKeyListPath, TRUE) == 0)
	{
		AddStringToStatus("HDCP-Key	File Open Error");
		return 0.0;
	}

	AddStringToStatus("HDCP-Key Downloading....");
	if(I2cAdcCtrl.WriteHdcpKey())
	{
		AddStringToStatus("HDCP-Key Download..OK");
	}
	else{
		AddStringToStatus("HDCP-Key Download..NG");
		return 0.0;
	}
	AddStringToStatus("HDCP-Key Reading....");
	if(I2cAdcCtrl.ReadHdcpKey())
	{
		AddStringToStatus("HDCP-Key Read..OK");
	}
	else{
		AddStringToStatus("HDCP-Key Read..NG");
		return 0.0;
	}
	for(i = 0; i < CurrentSet->nHDCP_Data_Size; i++)
	{
		if(I2cAdcCtrl.m_nHdcpKeyWriteData[i] != I2cAdcCtrl.m_nHdcpKeyReadData[i])
		{
			AddStringToStatus("HDCP-Key verify..NG");
			return 0.0;
		}
	}
	AddStringToStatus("HDCP-Key verify..OK");

	return 1.0;
}
double _HdcpKey_Download_Check() 
{
	int i;

	AddStringToStatus("HDCP-Key Reading....");
	if(I2cAdcCtrl.ReadHdcpKey())
	{
		AddStringToStatus("HDCP-Key Read..OK");
	}
	else{
		AddStringToStatus("HDCP-Key Read..NG");
		return 0.0;
	}
	for(i = 0; i < CurrentSet->nHDCP_Data_Size; i++)
	{
		if(I2cAdcCtrl.m_nHdcpKeyWriteData[i] != I2cAdcCtrl.m_nHdcpKeyReadData[i])
		{
			AddStringToStatus("HDCP-Key verify..NG");
			return 0.0;
		}
	}
	AddStringToStatus("HDCP-Key verify..OK");
	return 1.0;
}
*/
double _LNB_MeasureFrequency()
{
//	CString s;
	double dLnbF1 = 0.0;
	double dLnbF2 = 0.0;

	gLNBDetectorCtrl.GetFrequencyData(dLnbF1, dLnbF2);

	return dLnbF1;
}

double _LNB_MeasureFrequency2()
{
//	CString s;
	double dLnbF1 = 0.0;
	double dLnbF2 = 0.0;

	gLNBDetectorCtrl.GetFrequencyData(dLnbF1, dLnbF2);

	return dLnbF2;
}

double _LNB_MeasureVoltage()
{
//	CString s;
	double dLnbV1 = 0.0;
	double dLnbV2 = 0.0;

	gLNBDetectorCtrl.GetVoltageData(dLnbV1, dLnbV2);

	return dLnbV1;
}

double _LNB_MeasureVoltage2()
{
//	CString s;
	double dLnbV1 = 0.0;
	double dLnbV2 = 0.0;

	gLNBDetectorCtrl.GetVoltageData(dLnbV1, dLnbV2);

	return dLnbV2;
}
double _Input_Detect_Check()
{
	BOOL bResult = TRUE;

	bResult = TVCommCtrl.Input_Detect_Check(TVCommCtrl.m_nInputDetectItem, TVCommCtrl.m_nDetectMode);

	if(!bResult){
		return 0.0;
	}

	return 1.0;

}
double _AVC_IRDataCheck()
{
	BOOL bResult = TRUE;

	bResult = gAvConvertorCtrl.IRDataCheck()	;

	if(!bResult){
		return 0.0;
	}

	return 1.0;
}
double _AVC_IRBlasterDataCheck()
{
	BOOL bResult = TRUE;

	bResult = gAvConvertorCtrl.IRBlasterDataCheck()	;

	if(!bResult){
		return 0.0;
	}

	return 1.0;
}


double _AVC_ComDataCheck()
{
	BOOL bResult = TRUE;

	bResult = gAvConvertorCtrl.ComDataCheck()	;

	if(!bResult){
		return 0.0;
	}

	return 1.0;
}

double _Drm_Key_DL_Check()
{
	if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_TEST_RUN)
	{
		memset(g_pView->m_bKeyDLCheckResultList, 0, sizeof(g_pView->m_bKeyDLCheckResultList));
		for (int i = 0; i < 3; i++)
		{
			if (g_pView->Check_KeyDownload())
			{
				return 1.0;
			}
			//else
			//{
			//	TVCommCtrl.Send_TVControl_Command("aa 00 00", 700);
			//	TVCommCtrl.Send_TVControl_Command("ai 00 30", 700);
			//	TVCommCtrl.Send_TVControl_Command("aa 00 20", 700);
			//	TVCommCtrl.Send_TVControl_Command("ar 00 30", 700);
			//}
		}

	}
	else
	{
		memset(g_pView->m_bKeyDLCheckResultList, 0, sizeof(g_pView->m_bKeyDLCheckResultList));
		if(g_pView->Check_KeyDownload())
		{
			return 1.0;
		}
	

	}
	
	return 0.0;
}
double _Input_Detect_AllCheck()
{
	if(!g_pView->Check_InputDetect()){
		return 0.0;
	}
	
	return 1.0;
}

double _VF_Test_ResultCheck()
{
	DWORD dwThreadResult = 0;
	DWORD dwExitCode = 0;

	if(g_pView->m_bVFTestThreadRunning)
	{
		dwThreadResult = WaitForSingleObject(g_pView->m_hVfThreadKillEvent,10000);

		if(dwThreadResult != WAIT_OBJECT_0)
		{
			g_pView->m_bStopVFTestThread = TRUE;
			Sleep(500);
			if(g_pView->m_bVFTestThreadRunning) TerminateThread(g_pView->m_pVFTestThread->m_hThread,dwExitCode);
			g_pView->m_pVFTestThread = NULL; 
			g_pView->m_bVFTestThreadRunning = FALSE;

			return 0.0;
		}

	}
	::ResetEvent(g_pView->m_hVfThreadKillEvent);

	if(g_nNoFailedStep_EM > 0){
		return 0.0;
	}
	else{
		return 1.0;
	}
}

double _VF_Measure_Check()
{
	if(!g_pView->Check_VfMeasure()){
		return 0.0;
	}
	
	return 1.0;
}
#ifdef SM_MODIFY_CODE__


double _HP_Disable_Check()
{
	if (!g_pView->Check_HP_Disable()) {
		return 0.0;
	}

	return 1.0;
}

double _Port_Auto_Check()
{
	if (!g_pView->Loop_Auto_Check()) {
		return 0.0;
	}

	return 1.0;
}
#endif

double _IR_Blaster_Check()
{
	double ldReturn;

	ldReturn = g_pView->Check_IR_Blaster();
		

	return ldReturn;
	//if (!g_pView->Check_IR_Blaster()) {
	//	return 0.0;
	//}

	//return 1.0;
}

double _Read_A_CAS_ID()
{
	double ldReturn;

	ldReturn = g_pView->Read_A_CAS_ID();
		

	return ldReturn;
	
}