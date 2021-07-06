// ReviewPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ReviewPage.h"
#include "NoDifferentPointDlg.h"
#include "SignalTestResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//+2007.3.20 Add BY USY
#include "Global.h"
#include "ImageTotalDlg.h"

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CReviewPage dialog


CReviewPage::CReviewPage(CWnd* pParent /*=NULL*/)
	: CDialog(CReviewPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReviewPage)
	//}}AFX_DATA_INIT
}

//m_ctrlReviewGrid
void CReviewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReviewPage)
	DDX_Control(pDX, IDC_STATIC_REVIEW_REF_B_VAL, m_ctrlRefBValStatic);
	DDX_Control(pDX, IDC_STATIC_REVIEW_REF_R_VAL, m_ctrlRefRValStatic);
	DDX_Control(pDX, IDC_STATIC_REVIEW_REF_G_VAL, m_ctrlRefGValStatic);
	DDX_Control(pDX, IDC_STATIC_REVIEW_GRAB_B_VAL, m_ctrlGrabBValStatic);
	DDX_Control(pDX, IDC_STATIC_REVIEW_GRAB_G_VAL, m_ctrlGrabGValStatic);
	DDX_Control(pDX, IDC_STATIC_REVIEW_GRAB_R_VAL, m_ctrlGrabRValStatic);
	DDX_Control(pDX, IDC_EDIT_REVIEW_Y1_POS, m_ctrlYPosEdit);
	DDX_Control(pDX, IDC_EDIT_REVIEW_X1_POS, m_ctrlXPosEdit);
	DDX_Control(pDX, IDC_EDIT_REVIEW_RST_PATH, m_ctrlRstPathEdit);
	DDX_Control(pDX, IDC_EDIT_REVIEW_REF_IMG_PATH, m_ctrlRefImgEdit);
	DDX_Control(pDX, IDC_EDIT_REVIEW_PROC_IMG_PATH, m_ctrlProcImgEdit);
	DDX_Control(pDX, IDC_EDIT_REVIEW_GRAB_IMG_PATH, m_ctrlGrabImgEdit);
	//DDX_Control(pDX, IDC_REVIEW_GRID, m_ctrlReviewGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_REVIEW_LIST, m_ctrlReviewList);
}


BEGIN_MESSAGE_MAP(CReviewPage, CDialog)
	//{{AFX_MSG_MAP(CReviewPage)
	ON_BN_CLICKED(IDC_BTN_REVIEW_RST_FILE_OPEN, OnBtnReviewRstFileOpen)
	ON_BN_CLICKED(IDC_BTN_REVIEW_GET_RGB_VAL, OnBtnReviewGetRgbVal)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//+2007.3.20 Add BY USY
BOOL CReviewPage::GetRgbValue(int nImgType, CBaseImage& Image, int nXPos, int nYPos, int& nRvalue,int& nGvalue,int& nBvalue)
{
	int nBytesInRow = 0;

	CString szErrMsg;

	if(!Image.m_bImageLoaded)
	{
		if(nImgType == I_GRAB) 
		{
			AfxMessageBox("Grab image is not loaded!"); 
			return FALSE;
		}
		else if(nImgType == I_REF) 
		{
			AfxMessageBox("Reference image is not loaded!"); 
			return FALSE;
		}
//		else if(nImgType == I_PROC) 
//		{
//			AfxMessageBox("Processed image is not loaded!"); 
//			return FALSE;
//		}
	}

	if(nXPos >= Image.m_InfoHeader.biWidth)
	{
		szErrMsg.Format("Out of X resolution (%d)",Image.m_InfoHeader.biWidth);
		AfxMessageBox(szErrMsg); return FALSE;
	}
	else if(nYPos >= Image.m_InfoHeader.biHeight)
	{
		szErrMsg.Format("Out of Y resolution (%d)",Image.m_InfoHeader.biHeight);
		AfxMessageBox(szErrMsg); return FALSE;
	}

	nBytesInRow  = WIDTHBYTES(Image.m_InfoHeader.biBitCount * Image.m_InfoHeader.biWidth);
	nRvalue = Image.m_pImageData[(Image.m_InfoHeader.biHeight-nYPos -1 )*nBytesInRow+3*nXPos+2];   
	nGvalue = Image.m_pImageData[(Image.m_InfoHeader.biHeight-nYPos -1 )*nBytesInRow+3*nXPos+1]; 
	nBvalue = Image.m_pImageData[(Image.m_InfoHeader.biHeight-nYPos -1 )*nBytesInRow+3*nXPos]; 
	return TRUE;
}

void CReviewPage::OpenRstFile(CString sRstPath)
{
	int nTCount = 0;
	CString sSection, sTmp;

	m_Ini.SetIniFile(sRstPath);
	sSection.Format("Step Count");
	nTCount = m_Ini.GetProfileInt(sSection,"Total Step");
	POSITION Pos = m_StepList.GetHeadPosition();
	while(Pos != NULL)
	{
		delete m_StepList.GetNext(Pos);
	}
	m_StepList.RemoveAll();


	//+add kwmoon 070807
	CString sDifferentPointValue = _T("");
	int nFirstIndex = 0, nLastIndex = 0;
	//-

	//Position Initialize
	for(int nTmp = 0; nTmp < nTCount; nTmp++)
	{
		CStep* pStep = new CStep();

		sSection.Format("Step %d", nTmp+1);
		pStep->SetStepNo(nTmp+1);
		pStep->SetStepName(m_Ini.GetProfileString(sSection, "Step Name"));
		pStep->m_nStepType = m_Ini.GetProfileInt(sSection, "Step Type");
		pStep->m_nTestType = m_Ini.GetProfileInt(sSection, "Test Type");
		if(pStep->m_nStepType == PROCESS_FUNCTION)
		{
			//pStep->SetElapsedTime(atof(m_Ini.GetProfileString(sSection, "Time(s)")));
			pStep->m_fElapsedTime = atof(m_Ini.GetProfileString(sSection, "Time(s)"));
			m_StepList.AddTail(pStep);	
			continue;
		}
		else
		{
			//Audio Test Check
			sTmp = m_Ini.GetProfileString(sSection, "AudioTest");
			if(sTmp.Compare("TRUE") == 0) pStep->m_bRunAudioTest = TRUE;
			else if(sTmp.Compare("FALSE") == 0) pStep->m_bRunAudioTest = FALSE;

			//+ 2007.9.12 Add BY USY
			//Audio Target, Measure , Margin
			pStep->m_nLeftFreq[0] = m_Ini.GetProfileInt(sSection, "Audio Left Freq Target");
			pStep->m_nRightFreq[0] = m_Ini.GetProfileInt(sSection, "Audio Right Freq Target");
			pStep->m_nLeftFreq[1] = m_Ini.GetProfileInt(sSection, "Audio Left Freq Measure");
			pStep->m_nRightFreq[1] = m_Ini.GetProfileInt(sSection, "Audio Right Freq Measure");

			pStep->m_nLeftLevel[0] = m_Ini.GetProfileInt(sSection, "Audio Left Level Target");
			pStep->m_nRightLevel[0] = m_Ini.GetProfileInt(sSection, "Audio Right Level Target");
			pStep->m_nLeftLevel[1] = m_Ini.GetProfileInt(sSection, "Audio Left Level Measure");
			pStep->m_nRightLevel[1] = m_Ini.GetProfileInt(sSection, "Audio Right Level Measure");

			pStep->m_nAudioMargin = m_Ini.GetProfileInt(sSection, "Audio Left Freq Target");
			//-

			//Result Chekc
			sTmp = m_Ini.GetProfileString(sSection, "Result");
			if(sTmp.Compare("OK") == 0)
			{
				pStep->m_bTest = TRUE;
				pStep->SetResult(TRUE);
				pStep->m_bVideoTestResult = TRUE;
				if(pStep->m_bRunAudioTest) pStep->m_bAudioTestResult = TRUE;
				else pStep->m_bAudioTestResult = FALSE;
			}
			else if(sTmp.Compare("SKIP") ==0)
			{
				pStep->m_bTest = FALSE;
				pStep->SetResult(FALSE);
				pStep->m_bVideoTestResult = FALSE;
				pStep->m_bAudioTestResult = FALSE;
			}
			else
			{
				pStep->m_bTest = TRUE;
				pStep->SetResult(FALSE);

				//+change kwmoon 080305
				if(sTmp.Compare("NG/V") ==0)
				{
					pStep->m_bVideoTestResult = FALSE;
					if(pStep->m_bRunAudioTest) pStep->m_bAudioTestResult = TRUE;
					else pStep->m_bAudioTestResult = FALSE;
				}
				else if(sTmp.Compare("NG/A") ==0)
				{
					if(pStep->m_bRunVideoTest) pStep->m_bVideoTestResult = TRUE;
					else pStep->m_bVideoTestResult = FALSE;
					pStep->m_bAudioTestResult = FALSE;
				}
			//	if(sTmp.Compare("NG/AV") ==0)
				else
				{
					pStep->m_bVideoTestResult = FALSE;
					pStep->m_bAudioTestResult = FALSE;
				}
			}
			
			
//			if(sTmp.Compare("OK") == 0)
//			{
//				pStep->SetResult(TRUE);
//			}
//			else if((sTmp.Compare("NG/AV") == 0)
//				 || (sTmp.Compare("NG/V")  == 0)
//				 || (sTmp.Compare("NG/A")  == 0))
//			{
//				pStep->SetResult(FALSE);
//			}
			
			pStep->m_nFuncType = m_Ini.GetProfileInt(sSection, "Function Type");

			if(pStep->m_nFuncType == MEAS_BOOL)
			{
				sTmp = m_Ini.GetProfileString(sSection, "Measure Value");
				if(sTmp.Compare("TRUE") == 0)
				{
					pStep->m_bMeasured = TRUE;
				}
				else if(sTmp.Compare("FALSE") == 0)
				{
					pStep->m_bMeasured = FALSE;
				}
	
				// TARGET VALUE
				sTmp = m_Ini.GetProfileString(sSection, "Target Value");
				if(sTmp.Compare("TRUE") == 0)
				{
					pStep->m_bNominal = TRUE;
				}
				else if(sTmp.Compare("FALSE") == 0)
				{
					pStep->m_bNominal = FALSE;
				}

			}
			else 
			{	
				pStep->m_fMeasured	= atof(m_Ini.GetProfileString(sSection, "Measure Value"));
				pStep->m_fNominal	= atof(m_Ini.GetProfileString(sSection, "Target Value"));
				pStep->m_fLowLimit	= atof(m_Ini.GetProfileString(sSection, "Low Limit"));
				pStep->m_fHighLimit = atof(m_Ini.GetProfileString(sSection, "High Limit"));
				pStep->SetUnit(m_Ini.GetProfileString(sSection, "Unit"));
			}

			pStep->m_fElapsedTime	= atof(m_Ini.GetProfileString(sSection, "Time(s)"));
			pStep->m_strGrabPath	= m_Ini.GetProfileString(sSection, "Grab Image");
			pStep->m_strRefPath		= m_Ini.GetProfileString(sSection, "Ref Image");
			pStep->m_strProcPath	= m_Ini.GetProfileString(sSection, "Proc Image");
			pStep->m_strMaskPath	= m_Ini.GetProfileString(sSection, "Mask Image");

			//+2007.10.15 Add BY USY
			pStep->m_strMsg = m_Ini.GetProfileString(sSection, "Message");
			//-
			//+add kwmoon 070807
			nFirstIndex = 0;
			nLastIndex  = 0;

			sTmp = m_Ini.GetProfileString(sSection, "R-Diff");
	
			if(sTmp.Compare("NULL") != 0)
			{
				for(int i=0; i<NO_DIFFERENT_POINT_BLOCK-1; i++)
				{
					nLastIndex = sTmp.Find(',',nFirstIndex);
					
					if(nLastIndex == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:R-Diff]"); return;
					}

					sDifferentPointValue = sTmp.Mid(nFirstIndex,nLastIndex-nFirstIndex);
					pStep->m_aNoDifferentPoint[R_DATA][i] = atoi(sDifferentPointValue);
					nFirstIndex = nLastIndex+1;
				}
				sDifferentPointValue = sTmp.Right(sTmp.GetLength()-nFirstIndex);
				pStep->m_aNoDifferentPoint[R_DATA][NO_DIFFERENT_POINT_BLOCK-1] = atoi(sDifferentPointValue);
			}
			else
			{
				for(int i=0; i<NO_DIFFERENT_POINT_BLOCK; i++)
					pStep->m_aNoDifferentPoint[R_DATA][i] = -1;
			}

			nFirstIndex = 0;
			nLastIndex  = 0;

			sTmp = m_Ini.GetProfileString(sSection, "G-Diff");
			
			if(sTmp.Compare("NULL") != 0)
			{
				for(int i=0; i<NO_DIFFERENT_POINT_BLOCK-1; i++)
				{
					nLastIndex = sTmp.Find(',',nFirstIndex);
					
					if(nLastIndex == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:G-Diff]"); return;
					}

					sDifferentPointValue = sTmp.Mid(nFirstIndex,nLastIndex-nFirstIndex);
					pStep->m_aNoDifferentPoint[G_DATA][i] = atoi(sDifferentPointValue);
					nFirstIndex = nLastIndex+1;
				}
				sDifferentPointValue = sTmp.Right(sTmp.GetLength()-nFirstIndex);
				pStep->m_aNoDifferentPoint[G_DATA][NO_DIFFERENT_POINT_BLOCK-1] = atoi(sDifferentPointValue);
			}
			else
			{
				for(int i=0; i<NO_DIFFERENT_POINT_BLOCK; i++)
					pStep->m_aNoDifferentPoint[G_DATA][i] = -1;
			}

			nFirstIndex = 0;
			nLastIndex  = 0;

			sTmp = m_Ini.GetProfileString(sSection, "B-Diff");
			
			if(sTmp.Compare("NULL") != 0)
			{
				for(int i=0; i<NO_DIFFERENT_POINT_BLOCK-1; i++)
				{
					nLastIndex = sTmp.Find(',',nFirstIndex);
					
					if(nLastIndex == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:B-Diff]"); return;
					}

					sDifferentPointValue = sTmp.Mid(nFirstIndex,nLastIndex-nFirstIndex);
					pStep->m_aNoDifferentPoint[B_DATA][i] = atoi(sDifferentPointValue);
					nFirstIndex = nLastIndex+1;
				}
				sDifferentPointValue = sTmp.Right(sTmp.GetLength()-nFirstIndex);
				pStep->m_aNoDifferentPoint[B_DATA][NO_DIFFERENT_POINT_BLOCK-1] = atoi(sDifferentPointValue);
			}
			else
			{
				for(int i=0; i<NO_DIFFERENT_POINT_BLOCK; i++)
					pStep->m_aNoDifferentPoint[B_DATA][i] = -1;
			}
			
			//----------------------//
			// No of Ref High Bit
			//----------------------//

			CString sBitValueR = _T("");
			CString sBitValueG = _T("");
			CString sBitValueB = _T("");

			int nFirstIndexR = 0;
			int nFirstIndexG = 0;
			int nFirstIndexB = 0;
			int nLastIndexR  = 0;
			int nLastIndexG  = 0;
			int nLastIndexB  = 0;
			
			CString sDataStringR;
			CString sDataStringG;
			CString sDataStringB;

			sDataStringR = m_Ini.GetProfileString(sSection, "REF-R-HighBit");
			sDataStringG = m_Ini.GetProfileString(sSection, "REF-G-HighBit");
			sDataStringB = m_Ini.GetProfileString(sSection, "REF-B-HighBit");
	
			if((sDataStringR.Compare("NULL") != 0)
			&& (sDataStringG.Compare("NULL") != 0)
			&& (sDataStringB.Compare("NULL") != 0))
			{
				for(int i=0; i<MAX_NO_COLOR_BITS-1; i++)
				{
					nLastIndexR = sDataStringR.Find(',',nFirstIndexR);
					nLastIndexG = sDataStringG.Find(',',nFirstIndexG);
					nLastIndexB = sDataStringB.Find(',',nFirstIndexB);
					
					if(nLastIndexR == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-R-HighBit]"); return;
					}
					if(nLastIndexG == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-G-HighBit]"); return;
					}
					if(nLastIndexB == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-B-HighBit]"); return;
					}

					sBitValueR = sDataStringR.Mid(nFirstIndexR,nLastIndexR-nFirstIndexR);
					sBitValueG = sDataStringG.Mid(nFirstIndexG,nLastIndexG-nFirstIndexG);
					sBitValueB = sDataStringB.Mid(nFirstIndexB,nLastIndexB-nFirstIndexB);
					
					pStep->m_aRefNoHighBit[R_DATA][i] = atoi(sBitValueR);
					pStep->m_aRefNoHighBit[G_DATA][i] = atoi(sBitValueG);
					pStep->m_aRefNoHighBit[B_DATA][i] = atoi(sBitValueB);

					nFirstIndexR = nLastIndexR+1;
					nFirstIndexG = nLastIndexG+1;
					nFirstIndexB = nLastIndexB+1;
				}
				sBitValueR = sDataStringR.Right(sDataStringR.GetLength()-nFirstIndexR);
				sBitValueG = sDataStringG.Right(sDataStringG.GetLength()-nFirstIndexG);
				sBitValueB = sDataStringB.Right(sDataStringB.GetLength()-nFirstIndexB);

				pStep->m_aRefNoHighBit[R_DATA][MAX_NO_COLOR_BITS-1] = atoi(sBitValueR);
				pStep->m_aRefNoHighBit[G_DATA][MAX_NO_COLOR_BITS-1] = atoi(sBitValueG);
				pStep->m_aRefNoHighBit[B_DATA][MAX_NO_COLOR_BITS-1] = atoi(sBitValueB);
			}
			else
			{
				for(int i=0; i<MAX_NO_COLOR_BITS; i++)
				{
					pStep->m_aRefNoHighBit[R_DATA][i] = -1;
					pStep->m_aRefNoHighBit[G_DATA][i] = -1;
					pStep->m_aRefNoHighBit[B_DATA][i] = -1;
				}
			}

			//----------------------//
			// No of Grab High Bit
			//----------------------//

			sBitValueR = _T("");
			sBitValueG = _T("");
			sBitValueB = _T("");

			nFirstIndexR = 0;
			nFirstIndexG = 0;
			nFirstIndexB = 0;
			nLastIndexR  = 0;
			nLastIndexG  = 0;
			nLastIndexB  = 0;
			
			sDataStringR = m_Ini.GetProfileString(sSection, "GRAB-R-HighBit");
			sDataStringG = m_Ini.GetProfileString(sSection, "GRAB-G-HighBit");
			sDataStringB = m_Ini.GetProfileString(sSection, "GRAB-B-HighBit");
	
			if((sDataStringR.Compare("NULL") != 0)
			&& (sDataStringG.Compare("NULL") != 0)
			&& (sDataStringB.Compare("NULL") != 0))
			{
				for(int i=0; i<MAX_NO_COLOR_BITS-1; i++)
				{
					nLastIndexR = sDataStringR.Find(',',nFirstIndexR);
					nLastIndexG = sDataStringG.Find(',',nFirstIndexG);
					nLastIndexB = sDataStringB.Find(',',nFirstIndexB);
					
					if(nLastIndexR == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-R-HighBit]"); return;
					}
					if(nLastIndexG == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-G-HighBit]"); return;
					}
					if(nLastIndexB == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-B-HighBit]"); return;
					}

					sBitValueR = sDataStringR.Mid(nFirstIndexR,nLastIndexR-nFirstIndexR);
					sBitValueG = sDataStringG.Mid(nFirstIndexG,nLastIndexG-nFirstIndexG);
					sBitValueB = sDataStringB.Mid(nFirstIndexB,nLastIndexB-nFirstIndexB);

					pStep->m_aGrabNoHighBit[R_DATA][i] = atoi(sBitValueR);
					pStep->m_aGrabNoHighBit[R_DATA][i] = atoi(sBitValueG);
					pStep->m_aGrabNoHighBit[R_DATA][i] = atoi(sBitValueB);

					nFirstIndexR = nLastIndexR+1;
					nFirstIndexG = nLastIndexG+1;
					nFirstIndexB = nLastIndexB+1;
				}
				sBitValueR = sDataStringR.Right(sTmp.GetLength()-nFirstIndexR);
				sBitValueG = sDataStringG.Right(sTmp.GetLength()-nFirstIndexG);
				sBitValueB = sDataStringB.Right(sTmp.GetLength()-nFirstIndexB);

				pStep->m_aGrabNoHighBit[R_DATA][MAX_NO_COLOR_BITS-1] = atoi(sBitValueR);
				pStep->m_aGrabNoHighBit[G_DATA][MAX_NO_COLOR_BITS-1] = atoi(sBitValueG);
				pStep->m_aGrabNoHighBit[B_DATA][MAX_NO_COLOR_BITS-1] = atoi(sBitValueB);
			}
			else
			{
				for(int i=0; i<MAX_NO_COLOR_BITS; i++)
				{
					pStep->m_aGrabNoHighBit[R_DATA][i] = -1;
					pStep->m_aGrabNoHighBit[G_DATA][i] = -1;
					pStep->m_aGrabNoHighBit[B_DATA][i] = -1;
				}
			}

			//----------------------//
			// No of Ref 0-Pair Bit
			//----------------------//

			sBitValueR = _T("");
			sBitValueG = _T("");
			sBitValueB = _T("");

			nFirstIndexR = 0;
			nFirstIndexG = 0;
			nFirstIndexB = 0;
			nLastIndexR  = 0;
			nLastIndexG  = 0;
			nLastIndexB  = 0;
			
			sDataStringR = m_Ini.GetProfileString(sSection, "GRAB-R-LowBitPair");
			sDataStringG = m_Ini.GetProfileString(sSection, "GRAB-G-LowBitPair");
			sDataStringB = m_Ini.GetProfileString(sSection, "GRAB-B-LowBitPair");
	
			if((sDataStringR.Compare("NULL") != 0)
			&& (sDataStringG.Compare("NULL") != 0)
			&& (sDataStringB.Compare("NULL") != 0))
			{
				for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
				{
					nLastIndexR = sDataStringR.Find(',',nFirstIndexR);
					nLastIndexG = sDataStringG.Find(',',nFirstIndexG);
					nLastIndexB = sDataStringB.Find(',',nFirstIndexB);
					
					if(nLastIndexR == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-R-LowBitPair]"); return;
					}
					if(nLastIndexG == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-G-LowBitPair]"); return;
					}
					if(nLastIndexB == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-B-LowBitPair]"); return;
					}

					sBitValueR = sDataStringR.Mid(nFirstIndexR,nLastIndexR-nFirstIndexR);
					sBitValueG = sDataStringG.Mid(nFirstIndexG,nLastIndexG-nFirstIndexG);
					sBitValueB = sDataStringB.Mid(nFirstIndexB,nLastIndexB-nFirstIndexB);

					pStep->m_aGrabNoLowBitPair[R_DATA][i] = atoi(sBitValueR);
					pStep->m_aGrabNoLowBitPair[G_DATA][i] = atoi(sBitValueG);
					pStep->m_aGrabNoLowBitPair[B_DATA][i] = atoi(sBitValueB);

					nFirstIndexR = nLastIndexR+1;
					nFirstIndexG = nLastIndexG+1;
					nFirstIndexB = nLastIndexB+1;
				}
				sBitValueR = sDataStringR.Right(sDataStringR.GetLength()-nFirstIndexR);
				sBitValueG = sDataStringG.Right(sDataStringG.GetLength()-nFirstIndexG);
				sBitValueB = sDataStringB.Right(sDataStringB.GetLength()-nFirstIndexB);

				pStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueR);
				pStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueG);
				pStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueB);
			}
			else
			{
				for(int i=0; i<MAX_NO_COLOR_BITS; i++)
				{
					pStep->m_aGrabNoLowBitPair[R_DATA][i] = -1;
					pStep->m_aGrabNoLowBitPair[G_DATA][i] = -1;
					pStep->m_aGrabNoLowBitPair[B_DATA][i] = -1;
				}
			}

			//----------------------//
			// No of Grab 0-Pair Bit
			//----------------------//

			sBitValueR = _T("");
			sBitValueG = _T("");
			sBitValueB = _T("");

			nFirstIndexR = 0;
			nFirstIndexG = 0;
			nFirstIndexB = 0;
			nLastIndexR  = 0;
			nLastIndexG  = 0;
			nLastIndexB  = 0;
			
			sDataStringR = m_Ini.GetProfileString(sSection, "GRAB-R-LowBitPair");
			sDataStringG = m_Ini.GetProfileString(sSection, "GRAB-G-LowBitPair");
			sDataStringB = m_Ini.GetProfileString(sSection, "GRAB-B-LowBitPair");
	
			if((sDataStringR.Compare("NULL") != 0)
			&& (sDataStringG.Compare("NULL") != 0)
			&& (sDataStringB.Compare("NULL") != 0))
			{
				for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
				{
					nLastIndexR = sDataStringR.Find(',',nFirstIndexR);
					nLastIndexG = sDataStringG.Find(',',nFirstIndexG);
					nLastIndexB = sDataStringB.Find(',',nFirstIndexB);
					
					if(nLastIndexR == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-R-LowBitPair]"); return;
					}
					if(nLastIndexG == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-G-LowBitPair]"); return;
					}
					if(nLastIndexB == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-B-LowBitPair]"); return;
					}

					sBitValueR = sDataStringR.Mid(nFirstIndexR,nLastIndexR-nFirstIndexR);
					sBitValueG = sDataStringG.Mid(nFirstIndexG,nLastIndexG-nFirstIndexG);
					sBitValueB = sDataStringB.Mid(nFirstIndexB,nLastIndexB-nFirstIndexB);

					pStep->m_aGrabNoLowBitPair[R_DATA][i] = atoi(sBitValueR);
					pStep->m_aGrabNoLowBitPair[G_DATA][i] = atoi(sBitValueG);
					pStep->m_aGrabNoLowBitPair[B_DATA][i] = atoi(sBitValueB);

					nFirstIndexR = nLastIndexR+1;
					nFirstIndexG = nLastIndexG+1;
					nFirstIndexB = nLastIndexB+1;
				}
				sBitValueR = sDataStringR.Right(sDataStringR.GetLength()-nFirstIndexR);
				sBitValueG = sDataStringG.Right(sDataStringG.GetLength()-nFirstIndexG);
				sBitValueB = sDataStringB.Right(sDataStringB.GetLength()-nFirstIndexB);

				pStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueR);
				pStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueG);
				pStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueB);
			}
			else
			{
				for(int i=0; i<MAX_NO_COLOR_BITS; i++)
				{
					pStep->m_aGrabNoLowBitPair[R_DATA][i] = -1;
					pStep->m_aGrabNoLowBitPair[G_DATA][i] = -1;
					pStep->m_aGrabNoLowBitPair[B_DATA][i] = -1;
				}
			}

			//----------------------//
			// No of Ref 1-Pair Bit
			//----------------------//

			sBitValueR = _T("");
			sBitValueG = _T("");
			sBitValueB = _T("");

			nFirstIndexR = 0;
			nFirstIndexG = 0;
			nFirstIndexB = 0;
			nLastIndexR  = 0;
			nLastIndexG  = 0;
			nLastIndexB  = 0;
			
			sDataStringR = m_Ini.GetProfileString(sSection, "REF-R-HighBitPair");
			sDataStringG = m_Ini.GetProfileString(sSection, "REF-G-HighBitPair");
			sDataStringB = m_Ini.GetProfileString(sSection, "REF-B-HighBitPair");
	
			if((sDataStringR.Compare("NULL") != 0)
			&& (sDataStringG.Compare("NULL") != 0)
			&& (sDataStringB.Compare("NULL") != 0))
			{
				for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
				{
					nLastIndexR = sDataStringR.Find(',',nFirstIndexR);
					nLastIndexG = sDataStringG.Find(',',nFirstIndexG);
					nLastIndexB = sDataStringB.Find(',',nFirstIndexB);
					
					if(nLastIndexR == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-R-HighBitPair]"); return;
					}
					if(nLastIndexG == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-G-HighBitPair]"); return;
					}
					if(nLastIndexB == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:REF-B-HighBitPair]"); return;
					}

					sBitValueR = sDataStringR.Mid(nFirstIndexR,nLastIndexR-nFirstIndexR);
					sBitValueG = sDataStringG.Mid(nFirstIndexG,nLastIndexG-nFirstIndexG);
					sBitValueB = sDataStringB.Mid(nFirstIndexB,nLastIndexB-nFirstIndexB);

					pStep->m_aGrabNoHighBitPair[R_DATA][i] = atoi(sBitValueR);
					pStep->m_aGrabNoHighBitPair[G_DATA][i] = atoi(sBitValueG);
					pStep->m_aGrabNoHighBitPair[B_DATA][i] = atoi(sBitValueB);

					nFirstIndexR = nLastIndexR+1;
					nFirstIndexG = nLastIndexG+1;
					nFirstIndexB = nLastIndexB+1;
				}
				sBitValueR = sDataStringR.Right(sDataStringR.GetLength()-nFirstIndexR);
				sBitValueG = sDataStringG.Right(sDataStringG.GetLength()-nFirstIndexG);
				sBitValueB = sDataStringB.Right(sDataStringB.GetLength()-nFirstIndexB);

				pStep->m_aGrabNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueR);
				pStep->m_aGrabNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueG);
				pStep->m_aGrabNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueB);
			}
			else
			{
				for(int i=0; i<MAX_NO_COLOR_BITS; i++)
				{
					pStep->m_aGrabNoHighBitPair[R_DATA][i] = -1;
					pStep->m_aGrabNoHighBitPair[G_DATA][i] = -1;
					pStep->m_aGrabNoHighBitPair[B_DATA][i] = -1;
				}
			}

			//----------------------//
			// No of Grab 1-Pair Bit
			//----------------------//

			sBitValueR = _T("");
			sBitValueG = _T("");
			sBitValueB = _T("");

			nFirstIndexR = 0;
			nFirstIndexG = 0;
			nFirstIndexB = 0;
			nLastIndexR  = 0;
			nLastIndexG  = 0;
			nLastIndexB  = 0;
			
			sDataStringR = m_Ini.GetProfileString(sSection, "GRAB-R-HighBitPair");
			sDataStringG = m_Ini.GetProfileString(sSection, "GRAB-G-HighBitPair");
			sDataStringB = m_Ini.GetProfileString(sSection, "GRAB-B-HighBitPair");
	
			if((sDataStringR.Compare("NULL") != 0)
			&& (sDataStringG.Compare("NULL") != 0)
			&& (sDataStringB.Compare("NULL") != 0))
			{
				for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
				{
					nLastIndexR = sDataStringR.Find(',',nFirstIndexR);
					nLastIndexG = sDataStringG.Find(',',nFirstIndexG);
					nLastIndexB = sDataStringB.Find(',',nFirstIndexB);
					
					if(nLastIndexR == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-R-HighBitPair]"); return;
					}
					if(nLastIndexG == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-G-HighBitPair]"); return;
					}
					if(nLastIndexB == -1)
					{
						AfxMessageBox("Syntax error in a Rst file [Section:GRAB-B-HighBitPair]"); return;
					}

					sBitValueR = sDataStringR.Mid(nFirstIndexR,nLastIndexR-nFirstIndexR);
					sBitValueG = sDataStringG.Mid(nFirstIndexG,nLastIndexG-nFirstIndexG);
					sBitValueB = sDataStringB.Mid(nFirstIndexB,nLastIndexB-nFirstIndexB);

					pStep->m_aGrabNoHighBitPair[R_DATA][i] = atoi(sBitValueR);
					pStep->m_aGrabNoHighBitPair[G_DATA][i] = atoi(sBitValueG);
					pStep->m_aGrabNoHighBitPair[B_DATA][i] = atoi(sBitValueB);

					nFirstIndexR = nLastIndexR+1;
					nFirstIndexG = nLastIndexG+1;
					nFirstIndexB = nLastIndexB+1;
				}
				sBitValueR = sDataStringR.Right(sDataStringR.GetLength()-nFirstIndexR);
				sBitValueG = sDataStringG.Right(sDataStringG.GetLength()-nFirstIndexG);
				sBitValueB = sDataStringB.Right(sDataStringB.GetLength()-nFirstIndexB);

				pStep->m_aGrabNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueR);
				pStep->m_aGrabNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueG);
				pStep->m_aGrabNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2] = atoi(sBitValueB);
			}
			else
			{
				for(int i=0; i<MAX_NO_COLOR_BITS; i++)
				{
					pStep->m_aGrabNoHighBitPair[R_DATA][i] = -1;
					pStep->m_aGrabNoHighBitPair[G_DATA][i] = -1;
					pStep->m_aGrabNoHighBitPair[B_DATA][i] = -1;
				}
			}
		}
		m_StepList.AddTail(pStep);	
	}
}

void CReviewPage::OpenImgFile(CString sImgPath, int nImgType)
{
	CString sTmp;
	if(!FileExists(sImgPath))
	{
		sTmp.Format("%s\n File doesn't Exist!", sImgPath);
		AfxMessageBox(sTmp);
		return;
	}

	if(!m_Image[nImgType].LoadImage(sImgPath, *m_ctrlProgress)) return;
	*m_nImgType = nImgType;
	m_bFirstOpen[nImgType]	= TRUE;
	m_bZoomAllReset			= TRUE;
	m_bRefresh[nImgType]	= FALSE;

	int nFileNamePos = sImgPath.ReverseFind('\\');

	CString strImgFileName = sImgPath.Mid(nFileNamePos+1,sImgPath.GetLength()-nFileNamePos-1); 

	switch(nImgType) 
	{
		case I_GRAB:
			m_ctrlGrabImgEdit.SetWindowText(strImgFileName);
			break;
		case I_REF:
			m_ctrlRefImgEdit.SetWindowText(strImgFileName);
			break;
		case I_PROC:
			m_ctrlProcImgEdit.SetWindowText(strImgFileName);
			break;
		//+ 2007.4.28 Add BY USY
		case I_MASK:
			break;
		//-
	}



	m_MWnd->Invalidate(FALSE);
}

//+ 2007.7.11 Add BY USY
void CReviewPage::InitGrid()
{
	CString sTmp;

	//+ 2007.9.12 Add BY USY
	char *sHeader[] = {"NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT", "A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "A-MARGIN", "TIME(s)", "MESSAGE"};
	int Width[13] = { 80,50, 370, 130, 100, 100, 100, 100,80,190,190,120,90 };
	
//
//	m_ctrlReviewGrid.SetRedraw(flexRDNone);
//	m_ctrlReviewGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything
//
//	m_ctrlReviewGrid.SetBackColorFixed(RGB(210,216,176));
//	m_ctrlReviewGrid.SetForeColorFixed(RGB(51,102,153));
//	m_ctrlReviewGrid.SetBackColor(RGB(247,247,231));
//	m_ctrlReviewGrid.SetGridColor(RGB(210,216,176));
//	
//
////	m_ctrlReviewGrid.SetFixedRows(1);
////	m_ctrlReviewGrid.SetFixedCols(0);
//	//+2007.10.15 Add BY USY
//	m_ctrlReviewGrid.SetFrozenCols(1);
//	//-
//
//	m_ctrlReviewGrid.SetCols(13);
//	//+ 2007.9.12 Add BY USY
//	m_ctrlReviewGrid.SetRows(14);
////	m_ctrlReviewGrid.SetRows(11);
//	//-
//	m_ctrlReviewGrid.SetRow(0);

	//m_ctrlSpecAdjList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
	//m_ctrlSpecAdjList.InsertColumn(0, "Source", LVCFMT_CENTER, 80);
	//m_ctrlSpecAdjList.InsertColumn(1, "Margin", LVCFMT_CENTER, 80);



	////+add kwmoon 080804
	//int nNoSourceType = 0;

	//for (int i = 0; InputDefined[i].pszName != NULL; i++)
	//{
	//	nNoSourceType++;
	//}


	//for (int i = 0; InputDefined[i].pszName != NULL; i++)
	//{
	//	m_ctrlSpecAdjList.InsertItem(i, InputDefined[i].pszName);   // // m_ctrlSpecAdjList.InsertColumn(1, "Margin", LVCFMT_CENTER, 100); //m_ctrlSpecAdjGrid.SetTextMatrix(i + 1,0, InputDefined[i].pszName );
	//	//m_ctrlSpecAdjList.SetItem(i, 1, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); 
	//}

	//m_ctrlSpecAdjList.Invalidate();
	m_ctrlReviewList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//
//	//Header
	for(UINT nTmp = 0; nTmp < 13 ; nTmp++)
	{
		m_ctrlReviewList.InsertColumn(nTmp, sHeader[nTmp], LVCFMT_CENTER, Width[nTmp]); //m_ctrlReviewGrid.SetText(sHeader[nTmp]);
		
		
	}
	m_ctrlReviewList.Invalidate();
	return;
}
void CReviewPage::UpdateGrid()
{
//	m_ctrlReviewGrid.SetRedraw(flexRDNone);
//	m_ctrlReviewGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 
	//Row 
	m_ctrlReviewList.DeleteAllItems();
	//m_ctrlSpecAdjList.SetItem(i, 1, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
	//if(m_StepList.GetCount() < 10) m_ctrlReviewGrid.SetRows(11);
	//else m_ctrlReviewGrid.SetRows(m_StepList.GetCount() + 1);


	int nTmp;
	CString sTmp;

	POSITION Position = m_StepList.GetHeadPosition();
	
	if(m_StepList.GetCount() > 0)
	{
		nTmp = 0;
		while (Position) 
		{
			CStep* pStep = m_StepList.GetNext(Position);

		//	m_ctrlReviewGrid.SetRow(nTmp);

			//Step No
			sTmp.Format("%d", pStep->GetStepNo());
			//m_ctrlSpecAdjList.SetItem(i, 1, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp , 0, sTmp);
		
			//Step Name
			m_ctrlReviewList.InsertItem(nTmp, sTmp);// m_ctrlReviewGrid.SetTextMatrix(nTmp, 1, pStep->GetStepName());

		    if(pStep->m_nStepType == PROCESS_FUNCTION)
			{
				//ElapsedTime
				sTmp.Format(_T("%5.1f"), pStep->m_fElapsedTime);
				m_ctrlReviewList.SetItem(nTmp, 11, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //./m_ctrlReviewGrid.SetTextMatrix(nTmp, 11, sTmp);
				//m_ctrlReviewGrid.SetRedraw(-1);
				//m_ctrlReviewGrid.Refresh();

				nTmp++;
				continue;
			}

			else if(pStep->m_nStepType == CHECK_FUNCTION)
			{	//Result
				if(pStep->m_bTest == TRUE)
				{
					if (pStep->m_bResult)
					{
						sTmp = _T("OK");
	/*					m_ctrlReviewGrid.SetCol(2);
						m_ctrlReviewGrid.SetCellBackColor(RGB(0,0,255));
						m_ctrlReviewGrid.SetCellForeColor(RGB(255,255,255));*/
						m_ctrlReviewList.SetItem(nTmp, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp, 2, sTmp);
						
					}
					else // Video Test NG
					{
						//090324
						sTmp = _T("NG");
						if(pStep->m_bRunAudioTest && pStep->m_bRunVideoTest)
						{

							if(pStep->m_bVideoTestResult == FALSE)
							{
								sTmp = _T("NG/V");
							}

							if(pStep->m_bAudioTestResult == FALSE)
							{
								sTmp = _T("NG/A");
							}

							if((pStep->m_bVideoTestResult == FALSE) && (pStep->m_bAudioTestResult == FALSE))
							{
								sTmp = _T("NG/AV");
							}
						}
						else if(!pStep->m_bRunAudioTest && pStep->m_bRunVideoTest)
						{
							if(pStep->m_bVideoTestResult == FALSE)
							{
								sTmp = _T("NG/V");
							}
						}
						else if(pStep->m_bRunAudioTest && !pStep->m_bRunVideoTest)
						{
							if(pStep->m_bAudioTestResult == FALSE)
							{
								sTmp = _T("NG/A");
							}
						}
						else
						{
							sTmp = _T("NG");
						}

						//m_ctrlReviewGrid.SetCol(2);
						//m_ctrlReviewGrid.SetCellBackColor(RGB(255,0,0));
						//m_ctrlReviewGrid.SetCellForeColor(RGB(255,255,255));
						m_ctrlReviewList.SetItem(nTmp, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp, 2, sTmp);
					}
				}
				else if(pStep->m_bTest == FALSE)
				{
					sTmp = _T("SKIP");
					m_ctrlReviewList.SetItem(nTmp, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp, 2, sTmp);
				}
				
				if(pStep->GetFuncType() == MEAS_BOOL)                              
				{	                                                                  
					if(pStep->m_bMeasured == FALSE)                                
						sTmp.Format(_T("%s"), "FALSE");                                 
					else                                                              
						sTmp.Format(_T("%s"), "TRUE");                                  
				}                                                                 
				else                                                                
					sTmp.Format(_T("%5.1f"), pStep->m_fMeasured);   
				m_ctrlReviewList.SetItem(nTmp, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp , 3, sTmp);


				if(pStep->GetFuncType() == MEAS_BOOL)
				{						
					if(pStep->GetNominal_Bool() == FALSE) sTmp.Format(_T("%s"), "FALSE");
					else sTmp.Format(_T("%s"), "TRUE");
				}
				else sTmp.Format("%4.1f  ", pStep->GetNominal_float());

				m_ctrlReviewList.SetItem(nTmp, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp , 4, sTmp);



				//Low Limit, High Limit
				if(pStep->GetFuncType() != MEAS_BOOL)
				{
//					double fGap =  pStep->m_fMeasured - pStep->m_fNominal;      
//                                                                  
//					sTmp.Format(_T("%5.1f"), fGap); 
//					m_ctrlReviewGrid.SetCol(5);
//					m_ctrlReviewGrid.SetColDataType(5, flexDTString);
//					m_ctrlReviewGrid.SetCellAlignment(flexAlignCenterCenter);
//					m_ctrlReviewGrid.SetTextMatrix(nTmp , 5, sTmp);

					//Low Limit
					sTmp.Format("%4.1f", pStep->GetLowLimit());
					m_ctrlReviewList.SetItem(nTmp, 5, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp , 5, sTmp);
					//High Limit
					sTmp.Format("%4.1f", pStep->GetHighLimit());
					m_ctrlReviewList.SetItem(nTmp, 6, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp , 6, sTmp);
					//Unit
					sTmp.Format(_T("%s"), pStep->GetUnit());
					m_ctrlReviewList.SetItem(nTmp, 7, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp ,7, sTmp);
				}
				//Time
				sTmp.Format(_T("%5.1f"), pStep->m_fElapsedTime);
				m_ctrlReviewList.SetItem(nTmp, 11, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp, 11, sTmp);

				//+ 2007.7.23 Add BY USY
				//Message
				m_ctrlReviewList.SetItem(nTmp, 12, LVIF_TEXT, pStep->m_strMsg, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp, 12, pStep->m_strMsg);
				//-

				//+2007.10.15 Add BY USY
				if(pStep->m_bRunAudioTest)
				{
					// (Column 8) Audio Target
					sTmp.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
					m_ctrlReviewList.SetItem(nTmp, 8, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp , 8, sTmp);

					// (Column 9) Audio Measure
					sTmp.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[1], pStep->m_nRightFreq[1], pStep->m_nLeftLevel[1], pStep->m_nRightLevel[1]);
					m_ctrlReviewList.SetItem(nTmp, 9, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp , 9, sTmp);
					
					// (Column 10) Audio Margin
					sTmp.Format(_T("%d"), pStep->m_nAudioMargin);
					m_ctrlReviewList.SetItem(nTmp, 10, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlReviewGrid.SetTextMatrix(nTmp, 10, sTmp);
				}
				//-
			}

			nTmp++;
		}
	}
	//+2007.10.15 Add BY USY
	m_ctrlReviewList.Invalidate();
	//m_ctrlReviewGrid.SetRow(-1);
	//m_ctrlReviewGrid.SetCol(-1);
	////-
	//m_ctrlReviewGrid.SetRedraw(-1);
	//m_ctrlReviewGrid.Refresh();
	return;
}
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CReviewPage message handlers

BOOL CReviewPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Result Grid Init
	//+ 2007.7.11 Mod BY USY
	InitGrid();
//	InitResultGrid();
	//-
	
	m_strRstPath	= _T("");
	m_strGrabPath	= _T("");
	m_strRefPath	= _T("");
	m_strProcPath	= _T("");
	m_strMaskPath	= _T("");

	m_StepList.RemoveAll();
	//+ 2007.4.28 Add BY USY
	for(int nTmp = I_GRAB; nTmp < I_MASK; nTmp++)
//	for(int nTmp = 0; nTmp < 3; nTmp++)
	//-
	{
		m_bFirstOpen[nTmp] = FALSE;
		m_bRefresh[nTmp] = FALSE;
		m_Image[nTmp].m_bImageLoaded = FALSE;
	}
	m_bZoomAllReset = FALSE; 
	//

	//+add kwmoon 070807
	m_pNoDifferentPointDlg = NULL;

	GetWindowRect(&m_dlgRect);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReviewPage::OnBtnReviewRstFileOpen() 
{
	CFileDialog dlg(TRUE, "dat", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Test Result Files(*.dat)|*.dat|",NULL);

	//+ 2007.7.9 Mod BY USY
	CString sTmp = _T("");
	if(CurrentSet->sResultFolder == _T("")) sTmp = CurrentSet->sModelFolder;
	else sTmp = CurrentSet->sResultFolder;
//	CString sTmp = CurrentSet->sModelFolder + "\\Result\\";
	//-
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strRstPath = dlg.GetPathName();
		//+ 2007.4.28 Mod BY USY
		m_ctrlRstPathEdit.SetWindowText(dlg.GetFileName());
//		m_ctrlRstPathEdit.SetWindowText(m_strRstPath);
		//-
		OpenRstFile(m_strRstPath);	
		//+ 2007.7.11 Mod BY USY
		UpdateGrid();
//		UpdateRstGrid();
		//-
	}
}

void CReviewPage::OnBtnReviewGetRgbVal() 
{
	int Rvalue = 0;
	int Gvalue = 0;
	int Bvalue = 0;
	int nXPos, nYPos;
	int nBytesInRow = 0;

	CString sTmp = _T("");
	CString szErrMsg = _T("");
	
	m_ctrlXPosEdit.GetWindowText(sTmp); 	nXPos = atoi(sTmp);
	m_ctrlYPosEdit.GetWindowText(sTmp);		nYPos = atoi(sTmp);

	//Get Rgb Value From Grab Image
	if(!GetRgbValue(I_GRAB, m_Image[I_GRAB], nXPos, nYPos, Rvalue, Gvalue, Bvalue)) return;
	sTmp.Format("%3d", Rvalue);
	m_ctrlGrabRValStatic.SetWindowText(sTmp);
	sTmp.Format("%3d", Gvalue);
	m_ctrlGrabGValStatic.SetWindowText(sTmp);
	sTmp.Format("%3d", Bvalue);
	m_ctrlGrabBValStatic.SetWindowText(sTmp);

	//Get Rgb Value From Ref Image
	if(!GetRgbValue(I_REF, m_Image[I_REF], nXPos, nYPos, Rvalue, Gvalue, Bvalue)) return;
	sTmp.Format("%3d", Rvalue);
	m_ctrlRefRValStatic.SetWindowText(sTmp);
	sTmp.Format("%3d", Gvalue);
	m_ctrlRefGValStatic.SetWindowText(sTmp);
	sTmp.Format("%3d", Bvalue);
	m_ctrlRefBValStatic.SetWindowText(sTmp);
}


BEGIN_EVENTSINK_MAP(CReviewPage, CDialog)
    //{{AFX_EVENTSINK_MAP(CReviewPage)
//	ON_EVENT(CReviewPage, IDC_REVIEW_GRID, -601 /* DblClick */, OnDblClickReviewGrid, VTS_NONE)
	ON_EVENT(CReviewPage, IDC_REVIEW_GRID, -600 /* Click */, OnClickReviewGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CReviewPage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	POSITION pos = m_StepList.GetHeadPosition();
	while(pos != NULL)
    {       
        delete m_StepList.GetNext(pos);
    }
    m_StepList.RemoveAll();

//	delete pStep;
}
BOOL CReviewPage::PreTranslateMessage(MSG* pMsg) 
{
	//+ 2007.5.4 Add BY USY
	//Protect Dialog close By Enter Key or ESC Key
	if(pMsg->message == WM_KEYDOWN)
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
            return TRUE;
	//-
	return CDialog::PreTranslateMessage(pMsg);
}


//void CReviewPage::OnDblClickReviewGrid() 
//{
//	int nCurSel = m_ctrlReviewGrid.GetRow();
//	int nTotalCnt = m_StepList.GetCount();
//	if((nCurSel <1)||(nCurSel > nTotalCnt)) return;
//	
//	POSITION PosStep = m_StepList.FindIndex(nCurSel - 1);
//	CStep* pStep = m_StepList.GetAt(PosStep);
//	if(pStep->m_nStepType == PROCESS_FUNCTION) return;
//	//+ 2007.5.4 Mod BY USY
//	if(pStep->m_strMaskPath.IsEmpty() == 0)OpenImgFile(pStep->m_strMaskPath, I_MASK);
//	if(pStep->m_strProcPath.IsEmpty() == 0)OpenImgFile(pStep->m_strProcPath, I_PROC);
//	if(pStep->m_strGrabPath.IsEmpty() == 0)OpenImgFile(pStep->m_strGrabPath, I_GRAB);
//	if(pStep->m_strRefPath.IsEmpty() == 0)OpenImgFile(pStep->m_strRefPath, I_REF);
//}
void CReviewPage::OnClickReviewGrid() 
{
	// TODO: Add your control notification handler code here

	//int nCurSel = m_ctrlReviewGrid.GetRow();
	int nCurSel = (int)m_ctrlReviewList.GetFirstSelectedItemPosition();
	int nTotalCnt	= m_StepList.GetCount();
	
	int nLeft   = 0;
	int	nTop    = 0;
	int	nWidth  = 0;
	int	nHeight = 0;
	
	if((nCurSel <1)||(nCurSel > nTotalCnt)) return;
	
	POSITION PosStep = m_StepList.FindIndex(nCurSel - 1);

	CStep* pStep = m_StepList.GetAt(PosStep);
	
	if(pStep->m_nStepType == PROCESS_FUNCTION) return;
	//+change kwmoon 080125
	//+ 2007.9.14 Add BY USY
//	if(pStep->m_strMaskPath.IsEmpty() == 0)OpenImgFile(pStep->m_strMaskPath, I_MASK);
//	if(pStep->m_strProcPath.IsEmpty() == 0)OpenImgFile(pStep->m_strProcPath, I_PROC);
//	if(pStep->m_strGrabPath.IsEmpty() == 0)OpenImgFile(pStep->m_strGrabPath, I_GRAB);
//	if(pStep->m_strRefPath.IsEmpty() == 0)OpenImgFile(pStep->m_strRefPath, I_REF);
	if((pStep->m_strMaskPath.IsEmpty() == 0) && (pStep->m_strMaskPath.Compare("NULL") != NULL))
	{
		OpenImgFile(pStep->m_strMaskPath, I_MASK);
	}
	else
	{
		AfxMessageBox("Failed to open a mask image!"); return;
	}
	
	if((pStep->m_strProcPath.IsEmpty() == 0) && (pStep->m_strProcPath.Compare("NULL") != NULL))
	{
		OpenImgFile(pStep->m_strProcPath, I_PROC);
	}
	else
	{
		AfxMessageBox("Failed to open a mask image!"); return;
	}
	
	if((pStep->m_strGrabPath.IsEmpty() == 0) && (pStep->m_strGrabPath.Compare("NULL") != NULL))
	{
		OpenImgFile(pStep->m_strGrabPath, I_GRAB);
	}
	else
	{
		AfxMessageBox("Failed to open a mask image!"); return;
	}
	
	if((pStep->m_strRefPath.IsEmpty()  == 0) && (pStep->m_strRefPath.Compare("NULL") != NULL))
	{
		OpenImgFile(pStep->m_strRefPath, I_REF);
	}
	else
	{
		AfxMessageBox("Failed to open a mask image!"); return;
	}
	//-

	if(pStep->m_nTestType == RGB_TEST) // RGB_TEST : 1
	{

		if(m_pNoDifferentPointDlg == NULL)
		{
			// Modeless Dialog
			m_pNoDifferentPointDlg = new CNoDifferentPointDlg(this);
			m_pNoDifferentPointDlg->Create(IDD_NO_DIFFERENT_POINT_DLG);
		}

		nLeft   = (m_MWnd->m_PageRect.right)  - (m_pNoDifferentPointDlg->m_dlgRect.Width());
		nTop    = (m_MWnd->m_PageRect.bottom) - (m_pNoDifferentPointDlg->m_dlgRect.Height());
		nWidth  = m_pNoDifferentPointDlg->m_dlgRect.Width();
		nHeight = m_pNoDifferentPointDlg->m_dlgRect.Height();
		
		m_pNoDifferentPointDlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
		m_pNoDifferentPointDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
		m_pNoDifferentPointDlg->ShowWindow(SW_SHOW);
		m_pNoDifferentPointDlg->UpdateWindow();

		for(int i=0; i<3; i++)
		{
			for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++) m_pNoDifferentPointDlg->m_aNoDifferentPoint[i][j] = pStep->m_aNoDifferentPoint[i][j];
		}

		m_pNoDifferentPointDlg->UpdateGrid();

	}
	
	if(pStep->m_nTestType == SIGNAL_LINE_TEST) // SIGNAL_LINE_TEST : 7
	{
		// Modeless Dialog
		m_pSignalTestResultDlg = new CSignalTestResultDlg(this);
		
		m_pSignalTestResultDlg->m_pRefNoHighBit			= &pStep->m_aRefNoHighBit[0][0];
		m_pSignalTestResultDlg->m_pGrabNoHighBit		= &pStep->m_aGrabNoHighBit[0][0];
//		m_pSignalTestResultDlg->m_pRefNoHighBitPair		= &pStep->m_aRefNoHighBitPair[0][0];
//		m_pSignalTestResultDlg->m_pGrabNoHighBitPair	= &pStep->m_aGrabNoHighBitPair[0][0];
//		m_pSignalTestResultDlg->m_pRefNoLowBitPair		= &pStep->m_aRefNoLowBitPair[0][0];
//		m_pSignalTestResultDlg->m_pGrabNoLowBitPair		= &pStep->m_aGrabNoLowBitPair[0][0];

		m_pSignalTestResultDlg->Create(IDD_SIGNAL_LINE_TEST_RESULT_DLG);

		nLeft   = (m_MWnd->m_PageRect.right)  - (m_pSignalTestResultDlg->m_dlgRect.Width());
		nTop    = (m_MWnd->m_PageRect.bottom) - (m_pSignalTestResultDlg->m_dlgRect.Height());
		nWidth  = m_pSignalTestResultDlg->m_dlgRect.Width();
		nHeight = m_pSignalTestResultDlg->m_dlgRect.Height();


		m_pSignalTestResultDlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
		m_pSignalTestResultDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
		m_pSignalTestResultDlg->ShowWindow(SW_SHOW);
		m_pSignalTestResultDlg->UpdateWindow();

		m_pSignalTestResultDlg->ShowWindow(SW_SHOW);
		m_pSignalTestResultDlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

		m_pSignalTestResultDlg->UpdateGrid();
	}
}

