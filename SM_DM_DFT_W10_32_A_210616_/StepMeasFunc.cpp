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

#include <winsock.h>

#include "AnalogGrabber.h"
#include "thermometer.h"
#include "IrCheckerCtrl.h"
#include "Gmes.h"

// Extern
extern CDATsysView*		g_pView;
extern CBaseImage		g_RefImage;
extern CBaseImage		g_GrabImage;
extern CBaseImage		g_MaskImage;
extern CImageProc		g_ImageProc;

extern CAvSwitchBox		AvSwitchBoxCtrl;
extern CProgressCtrlX	ctrlImgProg;
extern BYTE				GrabData[1920][1080][3];
extern CTVCommCtrl		TVCommCtrl;
extern _InputSource		InputDefined[];

//+add kwmoon 080514
//extern CSoundCard g_SoundCard;

/////////////////////////////
// Image Processing Function
/////////////////////////////


BOOL ChekcValMuti(double dValL, double dValR, int nMode)
{
	BOOL bResult1 = TRUE;
	BOOL bResult2 = TRUE;
	double dValMax;
	double dValMin;
	CString sMsg;

	dValMax = pCurStep->GetHighLimit();
	dValMin = pCurStep->GetLowLimit();

	sMsg.Format(_T("L:%.02f, R:%.02f"), dValL, dValR);

	g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
	pCurStep->m_strMsg = sMsg;

	pCurStep->m_fMeasured_L = dValL;
	pCurStep->m_fMeasured_R = dValR;

	if(nMode == 0){
		if(dValL < dValMin || dValL > dValMax){
			bResult1 = FALSE;
		}
		if(dValR < dValMin || dValR > dValMax){
			bResult2 = FALSE;
		}
	}
	else{
		if(dValL >= dValMin){bResult1 = TRUE;}
		else{bResult1 = FALSE;}

		if(dValR >= dValMin){bResult2 = TRUE;}
		else{bResult2 = FALSE;}
	}
	if(bResult1 && bResult2){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

int GetSpecMargin(CString szInputSource)
{
	int nSpecMargin		 = 0;
	int nSpecMarginIndex = -1;

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined[i].pszName) == NULL)
		{
			nSpecMarginIndex = i; break;
		}
	}
	
	if(nSpecMarginIndex != -1)
	{
		nSpecMargin	= CurrentSet->nSrcMargin[nSpecMarginIndex];
	}
	
	return nSpecMargin;
}

double	_Color_Test()
{
	int nResult = TEST_FAIL;

	int nMaxDifference		= 0;
	int nPlusMaxDifference	= 0;
	int nMinusMaxDifference = 0;
	int nTotalNoErrorPixel	= 0;

	POINT ptResultPosition;
	ptResultPosition.x		= 0;
	ptResultPosition.y		= 0;

	POINT ptZeroPosition;
	ptZeroPosition.x		= 0;
	ptZeroPosition.y		= 0;

	CString sMsg 			= _T("");
	CString szMsg1			= _T("");
	CString szMsg2			= _T("");
	CString szPrevMsg		= _T("");

//	float fNewHighSpec		= 0.0;
//	float fNewLowSpec		= 0.0;
	float fMaxDifference	= 0.0;

	int   nColorDepth		= 0;
	int   nNoLevel			= 0;
//	int   nSpecMargin		= 0;


	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


	//090324
	pCurStep->m_bRunVideoTest = TRUE;
	//+add kwmoon 080819
	g_ImageProc.m_ptPositionShift.x = 0;
	g_ImageProc.m_ptPositionShift.y = 0;

	g_ImageProc.SetColorLimit((float)pCurStep->m_fLowLimit,(float)pCurStep->m_fHighLimit);
	g_ImageProc.SetAvgMaskSize(0,0,0); 

	if(g_GrabImage.m_bImageLoaded && g_RefImage.m_bImageLoaded)
	{
		nResult = g_ImageProc.RGBTest(NORMAL_IMAGE_TEST,g_GrabImage,g_RefImage,g_MaskImage,
									  ptResultPosition, fMaxDifference,nTotalNoErrorPixel,g_MaskImage.m_bImageLoaded,ctrlImgProg);
	

		if(nResult == TEST_FAIL)
		{
			CRect nImageRect;
			CWnd* pImageWnd;

			pImageWnd = g_pView->GetDlgItem(IDC_VIDEOVIEW);
			pImageWnd->GetWindowRect(&nImageRect);
			
			if(!Grab_Image_2in1(_T(""),0, TRUE))
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

		pCurStep->m_nTestType = RGB_TEST;
		pCurStep->m_nTotalNoErrorPixel = nTotalNoErrorPixel;
	
		nColorDepth	 = g_RefImage.m_InfoHeader.biBitCount;

		//+change kwmoon 080722
//		nNoLevel	 = (int)pow(2,(nColorDepth/3));		//+add kwmoon 080716
		nNoLevel	 = (int)pow(2,(nColorDepth/3))-1;		//+add kwmoon 080716
		
		//+add kwmoon 080716
		switch(nResult)
		{
			case TEST_PASS  : pCurStep->m_bResult = TRUE;  break;
			case TEST_FAIL  : pCurStep->m_bResult = FALSE; break;
			case TEST_ABORT : 
			{
				pCurStep->m_bResult = FALSE; break;
				pCurStep->m_nTotalNoErrorPixel = -1;

				AfxMessageBox("Failed to run color test");
				return MAX_COLOR_LEVEL;
			}
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
			g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
				g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
			}
		}

		//================
		// Save Test Data 
		//================
		if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData || g_pView->m_bAdjSpecMode)
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
		szMsg1.Format("Good:%.1f | Error:%.2f_%.2f_%.3f_%.3f_%.3f",
		pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
		pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
		pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);
		
		if((nResult == TEST_PASS) && g_pView->m_bAdjSpecMode)
		{
			if(g_pView->m_bAdjSpecMode && pCurStep->m_nSecondDifferentPointFreePercent != -1)
			{
//090519
//				nSpecMargin  = GetSpecMargin(pCurStep->m_strCurSrc);
//				fNewHighSpec = (float)pCurStep->m_nSecondDifferentPointFreePercent+nSpecMargin;
//				fNewLowSpec  = (float)(-1)*(pCurStep->m_nSecondDifferentPointFreePercent+nSpecMargin);

//				szMsg2.Format("Spec : L(%.1f>%.1f) H(%.1f>%.1f)",
//							   pCurStep->GetLowLimit(),fNewLowSpec,
//							   pCurStep->GetHighLimit(),fNewHighSpec);

				szMsg2.Format("Spec : L(%.1f) H(%.1f)",pCurStep->GetLowLimit(), pCurStep->GetHighLimit());

//Skip PSH 080816
//				pCurStep->SetHighLimit(fNewHighSpec);
//				pCurStep->SetLowLimit(fNewLowSpec);
				pCurStep->m_nAudioMargin = CurrentSet->nFreqMargin;

				szMsg1 = szMsg2 + " , " + szMsg1;
			}
		}
		
		if(CurrentSet->nDisplayType == DETAILEDGRID)
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

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

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

			// Show Message
			if(CurrentSet->nDisplayType == DETAILEDGRID)
			{
				g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
				
				if(szPrevMsg != _T(""))
				{
					g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg);
				}
				else
				{
					g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
				}		
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

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

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
		nNoLevel	 = (int)pow(2,(nColorDepth/3))-1;		//+add kwmoon 080716
		
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
			g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
				g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
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
		szMsg1.Format("Good:%.1f | Error : %.2f_%.2f_%.3f_%.3f_%.3f",
		pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
		pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
		pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);
	
		if((nResult == TEST_PASS) && g_pView->m_bAdjSpecMode)
		{
			if(g_pView->m_bAdjSpecMode && pCurStep->m_nSecondDifferentPointFreePercent != -1)
			{
//090519
//				nSpecMargin  = GetSpecMargin(pCurStep->m_strCurSrc);
//				fNewHighSpec = (float)pCurStep->m_nSecondDifferentPointFreePercent+nSpecMargin;
//				fNewLowSpec  = (float)(-1)*(pCurStep->m_nSecondDifferentPointFreePercent+nSpecMargin);

//				szMsg2.Format("Spec : L(%.1f>%.1f) H(%.1f>%.1f)",
//							   pCurStep->GetLowLimit(),fNewLowSpec,
//							   pCurStep->GetHighLimit(),fNewHighSpec);

				szMsg2.Format("Spec : L(%.1f) H(%.1f)",pCurStep->GetLowLimit(), pCurStep->GetHighLimit());

//Skip PSH 080816
//				pCurStep->SetHighLimit(fNewHighSpec);
//				pCurStep->SetLowLimit(fNewLowSpec);

				//+add kwmoon 080123
				pCurStep->m_nAudioMargin = CurrentSet->nFreqMargin;

				szMsg1 = szMsg2 + " , " + szMsg1;
			}
		}

		if(CurrentSet->nDisplayType == DETAILEDGRID)
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
			g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
				g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
			}
		}

		szMsg.Format("Diff : %d(%d,%d)",nMaxDifference,ptResultPosition.x,ptResultPosition.y);

		if(CurrentSet->nDisplayType == DETAILEDGRID)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " +szMsg);
				//+ 2007.7.23 Add BY USY
				pCurStep->m_strMsg = szPrevMsg + " , " + szMsg;
				//-
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
				//+ 2007.7.23 Add BY USY
				pCurStep->m_strMsg = szMsg;
				//-
			}		
		}
		return nMaxDifference;
	}
	else
	{
		return MAX_COLOR_LEVEL; 	
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

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

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

		if(CurrentSet->nDisplayType == DETAILEDGRID)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " +szMsg);
				pCurStep->m_strMsg = szPrevMsg + " , " + szMsg;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
				pCurStep->m_strMsg = szMsg;
			}		
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
			g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);

			//======================
			// Save Processed Image 
			//======================
			if(g_GrabImage.m_bProcessedImageLoaded && g_GrabImage.m_bSaveProcessedImage)
			{
				if(pCurStep->m_strProcPath == _T(""))
				{
					pCurStep->m_strProcPath = g_GrabImage.m_szFilePath.Left(g_GrabImage.m_szFilePath.ReverseFind('.')) + ".rst";
				}
				g_GrabImage.SaveImage(pCurStep->m_strProcPath, INTERNAL_PROC_IMAGE,ctrlImgProg);
			}
		}

		//================
		// Save Test Data 
		//================
		if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData || g_pView->m_bAdjSpecMode)
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
		szMsg1.Format("Good:%.1f | Error : %.2f_%.2f_%.3f_%.3f_%.3f",
		pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
		pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
		pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);

		//=======================
		// Display Adjusted Spec 
		//=======================
		if((nResult == TEST_PASS) && g_pView->m_bAdjSpecMode)
		{
			if(g_pView->m_bAdjSpecMode && pCurStep->m_nSecondDifferentPointFreePercent != -1)
			{

				szMsg2.Format("Spec : L(%.1f) H(%.1f)",pCurStep->GetLowLimit(), pCurStep->GetHighLimit());

				pCurStep->m_nAudioMargin = CurrentSet->nFreqMargin;

				szMsg1 = szMsg2 + " , " + szMsg1;
			}
		}
		
		if(CurrentSet->nDisplayType == DETAILEDGRID)
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
			
				if(CurrentSet->nDisplayType == DETAILEDGRID)
				{
					g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
					
					if(szPrevMsg != _T(""))
					{
						g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " +szMsg);
						pCurStep->m_strMsg = szPrevMsg + " , " + szMsg;
					}
					else
					{
						g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
						pCurStep->m_strMsg = szMsg;
					}		
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

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;
	
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
	
		if(CurrentSet->nDisplayType == DETAILEDGRID)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " +szMsg);
				pCurStep->m_strMsg = szPrevMsg + " , " + szMsg;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg);
				pCurStep->m_strMsg = szMsg;
			}		
		}
	}

	//+del kwmoon 080618
//	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	if(nResult == TEST_PASS) return 1.0;
	else return 0.0;
}

#include <winsock.h>
#include <windowsx.h>
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

double	_Audio_Test()
{
	BOOL bResult = FALSE;
//	MSG msg;

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
		if(g_pView->m_nMeasureWait > 0){
			clock_t		start;
			start = clock();

			while (TRUE)
			{
				bResult = g_pView->AudioMeasure();
				pCurStep->m_bResult = bResult;
				if(bResult) { break; }

				if ((clock() - start)*1000/CLOCKS_PER_SEC > g_pView->m_nMeasureWait){
					break;
				}

				if(CurrentSet->bRunAbort) {	break; }
				Sleep(100);
			}
		}
		else{
			if(g_pView->AudioMeasure() == TRUE)
			{
				pCurStep->m_bResult = TRUE;
			}
			else
			{
				_Wait(800);

				if(g_pView->AudioMeasure() == TRUE)
				{
					pCurStep->m_bResult = TRUE;
				}
				else
				{
					_Wait(1500);

					pCurStep->m_bResult = g_pView->AudioMeasure();
				}
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
/*
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
	int nErrorCode	= 0;
	int nImageBufferSize = 0;
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

	g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

	if((nGrabResult = g_pView->Grab_2in1(GRAB_WITH_RETRY)) != GRAB_PASS)
	{
		if(CurrentSet->nDisplayType == DETAILEDGRID)
		{
			szErrorMsg = "No LVDS input image!";
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szErrorMsg);
		}

		g_GrabImage.m_bImageLoaded = TRUE;
	}

	ctrlImgProg.SetPos(40);

	g_GrabImage.m_bImageLoaded = TRUE;

	g_pView->InvalidateRect(nImageRect, FALSE);

	g_pView->UpdateWindow(); _Wait(10);	
	
	ctrlImgProg.SetPos(100);

	return TRUE;
}
*/

#define NO_GRAB_FOR_MOVING_PIC_TEST		5

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
//	sGrabPath.Format("%s\\S%d_%s",CurrentSet->sGrabFolder, pCurStep->m_nStep, sGrabFileName);
	
	pCurStep->m_strGrabPath  = sGrabPath;
	pCurStep->m_strProcPath  = sGrabPath.Left(sGrabPath.ReverseFind('.')) + ".rst";
	g_GrabImage.m_szFilePath = sGrabPath;

	//090324
	pCurStep->m_bRunVideoTest = TRUE;


	TmpImage.m_pImageData = (BYTE *)malloc(AnalogControl.m_nImageSize);
	memset(TmpImage.m_pImageData,0,AnalogControl.m_nImageSize);

	Grab_Image_2in1(_T(""),0, FALSE);

	g_ImageProc.CopyImage(g_GrabImage, TmpImage);

	for(int i=0; i<nNoGrab; i++)
	{
		//Grab_LvdsImage_2in1();
		Grab_Image_2in1(_T(""),0, FALSE);
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

					g_GrabImage.SaveImage(szMovingPicImage,SNAP_IMAGE,ctrlImgProg);
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

		}
		g_ImageProc.CopyImage(g_GrabImage, TmpImage);
	//	_Wait(100);
	}
	//+change 
	if(nNoBlackScreen > 3){
		g_ImageProc.InitMovingPicTestMargin();
		return 1.0;
	}
	else{
		g_ImageProc.InitMovingPicTestMargin();
		return 0.0;
	}
}


//+add kwmoon 071108
#define NO_GRAB_FOR_CAPTION_TEST		15

double _Caption_Test()
{
	int nResult			= TEST_FAIL;
	int nNoGrab			= NO_GRAB_FOR_CAPTION_TEST;
	int nNoBlackScreen	= 0;
/*
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
		for(int i=0; i<nNoOnKeyPress; i++)
		{
			if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "39"))
			{
				return FALSE;
			}
			//-

			if(!_Wait(nCaptionKeyDelay)) return 100;
		}

		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "44"))
		{
			return FALSE;
		}
		if(!_Wait(nCaptionEnterKeyDelay)) return 100;
	}

	DWORD dwStartTick	= GetTickCount();
	DWORD dwElapsedTime = 0;
	
	int nFrameNo = 0;

	while(TRUE)
	{
		//Grab_LvdsImage_2in1();
		Grab_Image_2in1(_T(""),0, FALSE);

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

					g_GrabImage.SaveImage(szCaptionImage,SNAP_IMAGE,ctrlImgProg);
					g_GrabImage.SaveImage(szCaptionProcImage,INTERNAL_PROC_IMAGE,ctrlImgProg);
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

	for(int i=0; i<nNoOffKeyPress; i++)
	{
		if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "39"))
		{
			return FALSE;
		}

		if(!_Wait(nCaptionKeyDelay)) return 100;
	}

	if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "44"))
	{
		return FALSE;
	}
	//-
	if(!_Wait(nCaptionEnterKeyDelay)) return 100;
*/
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
	//Grab_LvdsImage_2in1();
	Grab_Image_2in1(_T(""),0, FALSE);

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


double _If_Test()
{
	int nResult = TEST_FAIL;
//	int nNoTest = TVCommCtrl.m_aCmd1.GetSize();
	BYTE nCmd;
	return 1.0;
//	for(int i=0; i<nNoTest; i++)
//	{
	//	nCmd = (BYTE)hexCstr2decNum(TVCommCtrl.m_aCmd1.GetAt(i));
		nCmd = (BYTE)hexCstr2decNum(TVCommCtrl.m_sCheckCmd);
	
	//	nResult = TVCommCtrl.IfTest(nCmd, TVCommCtrl.m_aWaitLimit.GetAt(i));
		nResult = TVCommCtrl.IfTest(nCmd, TVCommCtrl.m_nWaitLimit);

		if(nResult != TEST_PASS)
		{
		//	TVCommCtrl.ResetArray();
			
			return 0.0;
		}
//		_Wait(200);
//	}

//	TVCommCtrl.ResetArray();
	
	return 1.0;
}

double _If_Test2()
{
	int nResult = TEST_FAIL;
	BYTE nCmd;
	double dRev = 0.0;

	dRev = (double)TVCommCtrl.m_nIfTest_Data1;

	return  dRev;

	//return 1.0;
	/*
	nCmd = (BYTE)hexCstr2decNum(TVCommCtrl.m_sCheckCmd);
	
	nResult = TVCommCtrl.IfTest(nCmd, TVCommCtrl.m_nWaitLimit,FALSE);
	
	if(nResult == TEST_PASS){
		dRev = (double)TVCommCtrl.m_nIfTest_Data1;

		return  dRev;
	}
	else{
		return 1000.0;
	}
	*/
}
double _If_Test2_m()
{
	int nResult = TEST_FAIL;
	BYTE nCmd;
	double dRev1 = 0.0;
	double dRev2 = 0.0;
	return 1.0;
	nCmd = (BYTE)hexCstr2decNum(TVCommCtrl.m_sCheckCmd);
	
	nResult = TVCommCtrl.IfTest(nCmd, TVCommCtrl.m_nWaitLimit,FALSE);

	if(nResult == TEST_PASS){
		dRev1 = (double)TVCommCtrl.m_nIfTest_Data1;
		dRev2 = (double)TVCommCtrl.m_nIfTest_Data2;

		if(ChekcValMuti(dRev1, dRev2, 0)){
			pCurStep->SetResult(TRUE);
			return 1.0;
		}
		else{
			pCurStep->SetResult(FALSE);
			return 0.0;
		}
	}
	else{
		pCurStep->SetResult(FALSE); 
		return 0.0;	
	}
}

double _CheckTemperature()
{
	int nResult = TEST_FAIL;
	BYTE nCmd;
	double dRev = 0.0;
	double dSetTemp = 0.0;
	double dMeasurTemp = 0.0;
	CString szPrevMsg = _T("");
	CString sMsg	  = _T("");

	dMeasurTemp = gThermometerCtrl.MeasureTh();

	nCmd = 0xB3;
	nResult = TVCommCtrl.IfTest(nCmd, 10000,FALSE);

	if(nResult == TEST_PASS){
		dSetTemp = (double)TVCommCtrl.m_nIfTest_Data1;

		dRev = abs(dMeasurTemp - dSetTemp);

		sMsg.Format("(T:%.1f, M:%.1f)", dMeasurTemp,dSetTemp);
	}
	else{
		dRev =  1000.0;
		sMsg.Format("(T:%.1f, M:error)", dMeasurTemp);
	}

	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
	
	if(szPrevMsg != _T(""))
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + sMsg);
		pCurStep->m_strMsg = szPrevMsg + " , " + sMsg;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
		pCurStep->m_strMsg = sMsg;
	}		

	return  dRev;
}

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
			g_GrabImage.SaveImage(pCurStep->m_strGrabPath, SNAP_IMAGE,ctrlImgProg);
		}
		
		if(CurrentSet->nDisplayType == DETAILEDGRID)
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
	BOOL bResult5 = TRUE;
	BOOL bResult6 = TRUE;
	BOOL bResult7 = TRUE;
	BOOL bResult8 = TRUE;
	BOOL bResult9 = TRUE;
	BOOL bResult10 = TRUE;
	BOOL bResult11 = TRUE;
	BOOL bResult12 = TRUE;
	BOOL bResult13 = TRUE;
	BOOL bResult14 = TRUE;
	BOOL bResult15 = TRUE;
	//	int  nI2cResult;
	int i;
	int nData[3]   = {0,0,0};

//MCS-DSP_VER, MTK-BE_VER

	if(CurrentSet->nChipType == CHIP_MCS)
	{
		for(i = 0; i < 3; i++){
			bResult1 = TVCommCtrl.Firmware_VersionCheck(DSP_VER, CurrentSet->sDSP_Version);
			if(bResult1){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
		if(CurrentSet->sBE_Version != "")
		{ 
			for(i = 0; i < 3; i++){
				bResult2 = TVCommCtrl.Firmware_VersionCheck(BE_VER, CurrentSet->sBE_Version);
				if(bResult2){break;}
				if(CurrentSet->bRunAbort){return 0.0;}
			}
			_Wait(200);
		}
	}	
	else{
		for(i = 0; i < 3; i++){
			bResult1 = TVCommCtrl.Firmware_VersionCheck(BE_VER, CurrentSet->sBE_Version);
			if(bResult1){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);

		if(CurrentSet->sDSP_Version != "")
		{ 
			for(i = 0; i < 3; i++){
				bResult2 = TVCommCtrl.Firmware_VersionCheck(DSP_VER, CurrentSet->sDSP_Version);
				if(bResult2){break;}
				if(CurrentSet->bRunAbort){return 0.0;}
			}
			_Wait(200);
		}
	}	

	if(CurrentSet->sFE_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult3 = TVCommCtrl.Firmware_VersionCheck(FE_VER, CurrentSet->sFE_Version);
			if(bResult3){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}

	if(CurrentSet->sMicom_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult4 = TVCommCtrl.Firmware_VersionCheck(MICOM_VER, CurrentSet->sMicom_Version);
			if(bResult4){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}

	if(CurrentSet->sSubDSP_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult5 = TVCommCtrl.Firmware_VersionCheck(SUB_DSP_VER, CurrentSet->sSubDSP_Version);
			if(bResult5){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}
	
	if(CurrentSet->sEQ_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult6 = TVCommCtrl.Firmware_VersionCheck(EQ_VER, CurrentSet->sEQ_Version);
			if(bResult6){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}
	if(CurrentSet->sMEQ_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult7 = TVCommCtrl.Firmware_VersionCheck(MEQ_VER, CurrentSet->sMEQ_Version);
			if(bResult7){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}
	if(CurrentSet->sTouch_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult8 = TVCommCtrl.Firmware_VersionCheck(TOUCH_VER, CurrentSet->sTouch_Version);
			if(bResult8){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}

	if(CurrentSet->sDemo_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult9 = TVCommCtrl.Firmware_VersionCheck(DEMO_VER, CurrentSet->sDemo_Version);
			if(bResult9){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}

	if(CurrentSet->sDJSound_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult10 = TVCommCtrl.Firmware_VersionCheck(DJ_SOUND_VER, CurrentSet->sDJSound_Version);
			if(bResult10){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}
	if(CurrentSet->sWirelessTx_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult11 = TVCommCtrl.Firmware_VersionCheck(WIRELESS_TX_VER, CurrentSet->sWirelessTx_Version);
			if(bResult11){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}
	if(CurrentSet->sWirelessRx_Version != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult12 = TVCommCtrl.Firmware_VersionCheck(WIRELESS_RX_VER, CurrentSet->sWirelessRx_Version);
			if(bResult12){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
		_Wait(200);
	}
	///////////////////////////////////////////////////
	if (CurrentSet->sBT_Version != "")
	{
		for (i = 0; i < 3; i++) {
			bResult13 = TVCommCtrl.Firmware_VersionCheck(BT_VER, CurrentSet->sBT_Version);
			if (bResult13) { break; }
			if (CurrentSet->bRunAbort) { return 0.0; }
		}
		_Wait(200);
	}
	if (CurrentSet->sHDMI_Version != "")
	{
		for (i = 0; i < 3; i++) {
			bResult14 = TVCommCtrl.Firmware_VersionCheck(HDMI_VER, CurrentSet->sHDMI_Version);
			if (bResult14) { break; }
			if (CurrentSet->bRunAbort) { return 0.0; }
		}
		_Wait(200);
	}

	//////////////////////////////////////////////////
	if(CurrentSet->sChecksum != "")
	{ 
		for(i = 0; i < 3; i++){
			bResult15 = TVCommCtrl.Firmware_VersionCheck(CHECKSUM_VER, CurrentSet->sChecksum);
			if(bResult15){break;}
			if(CurrentSet->bRunAbort){return 0.0;}
		}
	}


	if(bResult1 && bResult2 && bResult3 && bResult4 && bResult5 && bResult6 && bResult7 && bResult8 && bResult9 && bResult10
		 && bResult11 && bResult12 && bResult13 && bResult14 && bResult15)
	{
		return 1.0;	
	}
	else
	{
		return 0.0;	
	}
}

double _Lan_MAC_Add_Check()
{
	if(!TVCommCtrl.MAC_AddressCheck(FALSE)){
		return 0.0;
	}
	
	return 1.0;
}

double _Wifi_MAC_Add_Check()
{

	if(!TVCommCtrl.MAC_AddressCheck(TRUE)){
		return 0.0;
	}
	
	return 1.0;
}
double _Serial_Number_Check()
{
	if(!TVCommCtrl.Serial_NumberCheck()){
		return 0.0;
	}
	
	return 1.0;
}
double _IRDataCheck()
{
	BOOL bResult = TRUE;

	bResult = gIrCheckerCtrl.IRDataCheck()	;

	if(!bResult){
		return 0.0;
	}

	return 1.0;
}
double _MAC_Add_CheckGmes()
{
	CString sMac_Add;

	if(!TVCommCtrl.MAC_AddressRead(sMac_Add, FALSE)){
		CurrentSet->bSystemBreak = TRUE;
		g_pView->m_szResultMsg2.Format("MAC Add Check(GMES): Read Error!");
		return 0.0;
	}

	CurrentSet->sMacAdd = sMac_Add;
	CurrentSet->bMacAddCheck = TRUE;
	if(!g_pView->Check_MacAddress(sMac_Add))
	{
		CurrentSet->bSystemBreak = TRUE;
		g_pView->m_szResultMsg2.Format("MAC Add Check(GMES): %s", gGmesCtrl.m_sErrorMsg);
		return 0.0;
	}

	return 1.0;
}
double _Mic_Diff_Level_Check()
{
	if(!TVCommCtrl.Check_MicDiffLevel()){
		return 0.0;
	}
	
	return 1.0;
}