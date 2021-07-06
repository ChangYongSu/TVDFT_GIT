// ImageTotalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ImageTotalDlg.h"
#include "AvSwitchBoxCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Global.h"

extern CDATsysView*  g_pView;

extern int g_nRunningProcessNo;

/////////////////////////////////////////////////////////////////////////////
// CImageTotalDlg dialog

CImageTotalDlg::CImageTotalDlg(CWnd* pParent /*=NULL*/)

	: CDialog(CImageTotalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageTotalDlg)
	m_nRadioAudioMeasureStart = 0;
	m_nRadioAudioOutL = -1;
	m_nRadioAudioOutR = -1;
	//}}AFX_DATA_INIT
}


void CImageTotalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageTotalDlg)
	DDX_Control(pDX, IDC_CMB_SEL_IMAGE, m_ctrlSelImageCmb);
	DDX_Control(pDX, IDC_CHK_MASK_IMG, m_ctrlMaskImgChk);
	DDX_Control(pDX, IDC_CHK_AVERAGE_PAGE, m_ctrlAveragePageChk);
	DDX_Control(pDX, IDC_STATIC_Y_POS, m_ctrlYPosStatic);
	DDX_Control(pDX, IDC_STATIC_X_POS, m_ctrlXPosStatic);
	DDX_Control(pDX, IDC_STATIC_B_VAL, m_ctrlBValStatic);
	DDX_Control(pDX, IDC_STATIC_G_VAL, m_ctrlGValStatic);
	DDX_Control(pDX, IDC_STATIC_R_VAL, m_ctrlRValStatic);
	DDX_Control(pDX, IDC_PROGRESS_ZOOM, m_ctrlProgress);
	DDX_Control(pDX, IDC_CMB_ZOOM, m_ctrlZoomCmb);
	DDX_Control(pDX, IDC_CHK_PROC_IMG, m_ctrlProcImgChk);
	DDX_Control(pDX, IDC_CHK_REF_IMG, m_ctrlRefImgChk);
	DDX_Control(pDX, IDC_CHK_GRAB_IMG, m_ctrlGrabImgChk);
	DDX_Control(pDX, IDC_IMG_V_SCROLL, m_ctrlImgVScroll);
	DDX_Control(pDX, IDC_IMG_H_SCROLL, m_ctrlImgHScroll);
	DDX_Control(pDX, IDC_CHK_PARM_PAGE, m_ctrlParmPageChk);
	DDX_Control(pDX, IDC_CHK_REVIEW_PAGE, m_ctrlReviewPageChk);
	DDX_Control(pDX, IDC_CHK_CROP_PAGE, m_ctrlCropPageChk);
	DDX_Control(pDX, IDC_CHK_GRAB_PAGE, m_ctrlGrabPageChk);
	DDX_Radio(pDX, IDC_RADIO_MEASURE_START, m_nRadioAudioMeasureStart);
	//DDX_Radio(pDX, IDC_RADIO_WAVEOUTL_START, m_nRadioAudioOutL);
	//DDX_Radio(pDX, IDC_RADIO_WAVEOUTR_START, m_nRadioAudioOutR);
	//}}AFX_DATA_MAP
  //  DDX_Control(pDX, IDC_DISPLAY1, m_ActiveMILDisplay);
}


BEGIN_MESSAGE_MAP(CImageTotalDlg, CDialog)
	//{{AFX_MSG_MAP(CImageTotalDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHK_GRAB_PAGE, OnChkGrabPage)
	ON_BN_CLICKED(IDC_CHK_CROP_PAGE, OnChkCropPage)
	ON_BN_CLICKED(IDC_CHK_REVIEW_PAGE, OnChkReviewPage)
	ON_BN_CLICKED(IDC_CHK_PARM_PAGE, OnChkParmPage)
	ON_BN_CLICKED(IDC_CHK_GRAB_IMG, OnChkGrabImg)
	ON_BN_CLICKED(IDC_CHK_REF_IMG, OnChkRefImg)
	ON_BN_CLICKED(IDC_CHK_PROC_IMG, OnChkProcImg)
	ON_CBN_SELCHANGE(IDC_CMB_ZOOM, OnSelchangeCmbZoom)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHK_MASK_IMG, OnChkMaskImg)
	ON_BN_CLICKED(IDC_CHK_AVERAGE_PAGE, OnChkAveragePage)
	ON_CBN_SELCHANGE(IDC_CMB_SEL_IMAGE, OnSelchangeCmbSelImage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_MEASURE_START, OnRadioMeasure)
	ON_BN_CLICKED(IDC_RADIO_MEASURE_STOP, OnRadioMeasure)
//	ON_BN_CLICKED(IDC_RADIO_WAVEOUTL_START, OnRadioWaveoutL)
//	ON_BN_CLICKED(IDC_RADIO_WAVEOUTL_STOP, OnRadioWaveoutL)
//	ON_BN_CLICKED(IDC_RADIO_WAVEOUTR_START, OnRadioWaveoutR)
//	ON_BN_CLICKED(IDC_RADIO_WAVEOUTR_STOP, OnRadioWaveoutR)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_UPDATE_IMAGE_VIEW_DISPLAY, UpdateImageViewDisplay)
	ON_MESSAGE(UM_CALC_MOVE_SUB_WINDOW, CalcAndMoveSubWindow)
	ON_MESSAGE(UM_CHANGE_ACTIVE_VIEW_TAB, ChangeActiveViewTab)
	ON_MESSAGE(UM_UPDATE_ALL_VIEW_TAB, UpdateAllViewTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageTotalDlg message handlers

//+2007.3.15 Add BY USY
void CImageTotalDlg::SetChkBtn(int nTabType, int nImgType)
{
	switch(nTabType) 
	{
		case P_GRAB:
			m_ctrlGrabPageChk.SetCheck(TRUE);
			m_ctrlCropPageChk.SetCheck(FALSE);
			m_ctrlReviewPageChk.SetCheck(FALSE);
			m_ctrlParmPageChk.SetCheck(FALSE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			break;
		case P_CROP:
			m_ctrlGrabPageChk.SetCheck(FALSE);
			m_ctrlCropPageChk.SetCheck(TRUE);
			m_ctrlReviewPageChk.SetCheck(FALSE);
			m_ctrlParmPageChk.SetCheck(FALSE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			break;
		case P_REVIEW:
			m_ctrlGrabPageChk.SetCheck(FALSE);
			m_ctrlCropPageChk.SetCheck(FALSE);
			m_ctrlReviewPageChk.SetCheck(TRUE);
			m_ctrlParmPageChk.SetCheck(FALSE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			break;
		case P_PARM:
			m_ctrlGrabPageChk.SetCheck(FALSE);
			m_ctrlCropPageChk.SetCheck(FALSE);
			m_ctrlReviewPageChk.SetCheck(FALSE);
			m_ctrlParmPageChk.SetCheck(TRUE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			break;
		//+ 2007.9.19 Del BY USY
//		case P_AVERAGE:
//			m_ctrlGrabPageChk.SetCheck(FALSE);
//			m_ctrlCropPageChk.SetCheck(FALSE);
//			m_ctrlReviewPageChk.SetCheck(FALSE);
//			m_ctrlParmPageChk.SetCheck(FALSE);
//			m_ctrlAveragePageChk.SetCheck(TRUE);
//			break;
		//-
	}

	SetPage(nTabType, nImgType);
}

void CImageTotalDlg::SetImgChkBtn(int nTabType, int nImgType)
{
	//+ 2007.10.30 Mod BY USY
	m_ctrlSelImageCmb.SetCurSel(nImgType);
//	switch(nImgType) 
//	{
//		case I_NONE:
//			m_ctrlGrabImgChk.SetCheck(FALSE);
//			m_ctrlRefImgChk.SetCheck(FALSE);
//			m_ctrlProcImgChk.SetCheck(FALSE);
//			m_ctrlMaskImgChk.SetCheck(FALSE);
//			break;
//		case I_GRAB:
//			m_ctrlGrabImgChk.SetCheck(TRUE);
//			m_ctrlRefImgChk.SetCheck(FALSE);
//			m_ctrlProcImgChk.SetCheck(FALSE);
//			m_ctrlMaskImgChk.SetCheck(FALSE);
//			break;
//		case I_REF:
//			m_ctrlGrabImgChk.SetCheck(FALSE);
//			m_ctrlRefImgChk.SetCheck(TRUE);
//			m_ctrlProcImgChk.SetCheck(FALSE);
//			m_ctrlMaskImgChk.SetCheck(FALSE);
//			break;
//		case I_PROC:
//			m_ctrlGrabImgChk.SetCheck(FALSE);
//			m_ctrlRefImgChk.SetCheck(FALSE);
//			m_ctrlProcImgChk.SetCheck(TRUE);
//			m_ctrlMaskImgChk.SetCheck(FALSE);
//			break;
//		case I_MASK:
//			m_ctrlGrabImgChk.SetCheck(FALSE);
//			m_ctrlRefImgChk.SetCheck(FALSE);
//			m_ctrlProcImgChk.SetCheck(FALSE);
//			m_ctrlMaskImgChk.SetCheck(TRUE);
//			break;
//	}
	//-
}

void CImageTotalDlg::SetPage(int nTabType, int nImgType)
{
	switch(nTabType) 
	{
		case P_GRAB:
		{	
			m_GrabPage.ShowWindow(SW_SHOW);
			m_ReviewPage.ShowWindow(SW_HIDE);
			m_ParmPage.ShowWindow(SW_HIDE);
			//+ 2007.10.16 Add BY USY
			GetDlgItem(IDC_STATIC_PAGE)->ShowWindow(SW_HIDE);
			//-
			SetImgPanel(SW_HIDE);
			SetPageItem(nTabType, nImgType);

			if(!m_GrabPage.m_bThreadRunning){
				VideoDispSelectWindow();
				if(m_GrabPage.m_nGrabMode == AUTO_GRAB){
					m_GrabPage.GrabStart();
				}

			}
		}	
		break;
		case P_REVIEW:
		{	
			m_GrabPage.GrabStop();
			VideoDispDeselectWindow();
			
			m_GrabPage.ShowWindow(SW_HIDE);
			m_ReviewPage.ShowWindow(SW_SHOW);
			m_ParmPage.ShowWindow(SW_HIDE);
			//+ 2007.10.16 Add BY USY
			GetDlgItem(IDC_STATIC_PAGE)->ShowWindow(SW_SHOW);
			//-
			SetImgPanel(SW_SHOW);
			SetImgChkBtn(nTabType, nImgType);
			SetPageItem(nTabType, nImgType);
		}	
		break;
		case P_PARM:
		{
			m_GrabPage.GrabStop();
			VideoDispDeselectWindow();
			
			m_GrabPage.ShowWindow(SW_HIDE);
			m_ReviewPage.ShowWindow(SW_HIDE);
			m_ParmPage.ShowWindow(SW_SHOW);
			//+ 2007.10.16 Add BY USY
			GetDlgItem(IDC_STATIC_PAGE)->ShowWindow(SW_SHOW);
			//-
			SetImgPanel(SW_SHOW);
			SetImgChkBtn(nTabType, nImgType);
			SetPageItem(nTabType, nImgType);
		}
		break;
		//+ 2007.9.19 Del BY USY
//		case P_AVERAGE:
//		{
//			m_GrabPage.ShowWindow(SW_HIDE);
//			m_ReviewPage.ShowWindow(SW_HIDE);
//			m_ParmPage.ShowWindow(SW_HIDE);
//			SetImgPanel(SW_HIDE);
//			SetPageItem(nTabType, nImgType);
//		}
//		break;
		//-
	}
}

void CImageTotalDlg::SetPageItem(int nTabType, int nImgType)
{
	int nTmp;
	switch(nTabType) 
	{
	case P_GRAB:
		m_Image[0]					= &m_GrabPage.m_Image1;
		m_GrabPage.m_ctrlProgress	= &m_ctrlProgress;
		m_ZImgInfo					= NULL;
		m_GrabPage.m_MWnd			= this;
		break;
	case P_CROP:
		break;
	case P_REVIEW:
		SetZoomCmb(nTabType, nImgType);

		for(nTmp = I_GRAB; nTmp <= I_MASK; nTmp++)
		{
			m_Image[nTmp] = &m_ReviewPage.m_Image[nTmp];
		}

		m_ReviewPage.m_nTabType		= m_nTabType;
		m_ReviewPage.m_nImgType		= &m_nImgType;
		m_ReviewPage.m_ctrlProgress	= &m_ctrlProgress;
		m_ReviewPage.m_MWnd			= this;
		m_ZImgInfo = NULL;
		break;
	case P_PARM:
		SetZoomCmb(nTabType, nImgType);

		for(nTmp = I_GRAB; nTmp <= I_MASK; nTmp++)
		{
			m_Image[nTmp] = &m_ParmPage.m_Image[nTmp];
		}

		m_ParmPage.m_nTabType		= m_nTabType;
		m_ParmPage.m_nImgType		= &m_nImgType;
		m_ParmPage.m_ctrlZoomCmb	= &m_ctrlZoomCmb;
		m_ParmPage.m_ctrlProgress	= &m_ctrlProgress;
		m_ParmPage.m_MWnd			= this;
		m_ZImgInfo = NULL;
		break;
	}
}

void CImageTotalDlg::SetImgPanel(BOOL bShow)
{
	//Group Box
	GetDlgItem(IDC_STATIC_GROUP_1)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_GROUP_2)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_GROUP_3)->ShowWindow(bShow);
	
	//Image Select
	//+ 2007.10.30 Mod BY USY
	GetDlgItem(IDC_STATIC_SEL_IMAGE)->ShowWindow(bShow);
	GetDlgItem(IDC_CMB_SEL_IMAGE)->ShowWindow(bShow);
//	GetDlgItem(IDC_CHK_GRAB_IMG)->ShowWindow(bShow);
//	GetDlgItem(IDC_CHK_REF_IMG)->ShowWindow(bShow);
//	GetDlgItem(IDC_CHK_PROC_IMG)->ShowWindow(bShow);
//	GetDlgItem(IDC_CHK_MASK_IMG)->ShowWindow(bShow);
	//-
	//Zoom
	GetDlgItem(IDC_STATIC_ZOOM)->ShowWindow(bShow);
	GetDlgItem(IDC_CMB_ZOOM)->ShowWindow(bShow);
	GetDlgItem(IDC_PROGRESS_ZOOM)->ShowWindow(bShow);
	
	//X,Y,R,G,B
	GetDlgItem(IDC_STATIC_X)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_X_POS)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_Y)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_Y_POS)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_R)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_R_VAL)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_G)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_G_VAL)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_B)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_B_VAL)->ShowWindow(bShow);
}

void CImageTotalDlg::InitImgPanel()
{
	//Img Chk Btn Init
	//+ 2007.10.30 Mod BY USY
	m_ctrlSelImageCmb.ResetContent();
	m_ctrlSelImageCmb.AddString("Grab");
	m_ctrlSelImageCmb.AddString("Reference");
	m_ctrlSelImageCmb.AddString("Processed");
	m_ctrlSelImageCmb.AddString("Mask");
	m_ctrlSelImageCmb.SetCurSel(-1);
	

//	m_ctrlGrabImgChk.SetCheck(FALSE);
//	m_ctrlRefImgChk.SetCheck(FALSE);
//	m_ctrlProcImgChk.SetCheck(FALSE);
//	m_ctrlMaskImgChk.SetCheck(FALSE);
	//-
	//Img Type Init
	m_nImgType = I_NONE;

	//Scroll Hide
	m_ctrlImgVScroll.ShowScrollBar(FALSE);
	m_ctrlImgHScroll.ShowScrollBar(FALSE);

	//Zoom Combo Init
	m_ctrlZoomCmb.SetCurSel(0);

	//Progress Init
	m_ctrlProgress.SetPos(0);

	//X/Y/R/G/B Val Init
	m_ctrlXPosStatic.SetWindowText(_T(""));
	m_ctrlYPosStatic.SetWindowText(_T(""));
	m_ctrlRValStatic.SetWindowText(_T(""));
	m_ctrlGValStatic.SetWindowText(_T(""));
	m_ctrlBValStatic.SetWindowText(_T(""));

	//////////////////////////////////////////////////////////////////////////
	//Zoom Factor, Scroll Value Init
	for(int nTmp1 = 0; nTmp1 < 5; nTmp1++)
	{
		m_nOldImgType[nTmp1] = I_NONE; 

		for(int nTmp2 = I_GRAB; nTmp2 <= I_MASK; nTmp2++)
		{
			//Zoom Factor Init
			m_fZFactor[nTmp1][nTmp2] = 1.0f;

			//Scroll Value Init
			m_nScrHCur[nTmp1][nTmp2] = 0;
			m_nScrVCur[nTmp1][nTmp2] = 0;
			m_nScrHMax[nTmp1][nTmp2] = 0;
			m_nScrVMax[nTmp1][nTmp2] = 0;
			m_nStartX[nTmp1][nTmp2]	 = 0;
			m_nStartY[nTmp1][nTmp2]  = 0;
		}	
	}
}

void CImageTotalDlg::SetZoomCmb(int nTabType, int nImgType)
{
	int nCurSel;

	switch((int)m_fZFactor[nTabType][nImgType]) 
	{
		case 1:
			nCurSel = 0;
			break;
		case 2:
			nCurSel = 1;
			break;
		case 4:
			nCurSel = 2;
			break;
	}
	
	m_ctrlZoomCmb.SetCurSel(nCurSel);
}

void CImageTotalDlg::DrawImage(int nTabType, int nImageType,int nDlgItemNo,CBaseImage& ImageData, int nStartXPos, int nStartYPos, float fZoomFactor)
{
	CWnd *pWnd = (CWnd*)GetDlgItem(nDlgItemNo);

	if(!pWnd->GetSafeHwnd())
	{
		AfxMessageBox("Failed to Show Image"); return;
	}

	CDC* pDC = pWnd->GetDC();

	CRect rect;
	pWnd->GetWindowRect(&rect);
	pWnd->ScreenToClient(&rect);

	// Palette 
	HPALETTE hPalette	 = NULL;          
	HPALETTE hOldPalette = NULL;       

	if(ImageData.cPalette != NULL)
	{	
		hPalette	= (HPALETTE)ImageData.cPalette;
		hOldPalette = ::SelectPalette(pDC->GetSafeHdc(), hPalette, TRUE);
		RealizePalette(pDC->GetSafeHdc());
	}
	else
	{
		if(ImageData.SetSystemPalette(pDC->GetSafeHdc()))
		{
			RealizePalette(pDC->GetSafeHdc());
		}
	}

	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);

	if(nTabType == P_GRAB)
	{
		StretchDIBits(pDC->GetSafeHdc()		// handle to DC	
					  
					  ,0//1 change kwmoon 080709	// x-coord of destination upper-left corner
					  ,0//1	change kwmoon 080709	// y-coord of destination upper-left corner
					  ,rect.Width()			// width of destination rectangle
					  ,rect.Height()		// height of destination rectangle
					  ,0					// x-coord of source upper-left corner
					  ,0					// y-coord of source upper-left corner
					  ,ImageData.m_BmInfo->bmiHeader.biWidth	// width of source rectangle
					  ,ImageData.m_BmInfo->bmiHeader.biHeight	// height of source rectangle
					  ,ImageData.m_pImageData					// bitmap bits
					  ,ImageData.m_BmInfo						// bitmap data
					  ,DIB_RGB_COLORS							 // usage options
					  ,SRCCOPY);								// raster operation code
	}
	else if(nTabType == P_CROP)
	{
		int nWidth, nHeight;

		if(ImageData.m_BmInfo->bmiHeader.biWidth < rect.Width())
		{
			nWidth = ImageData.m_BmInfo->bmiHeader.biWidth;
		}
		else nWidth = rect.Width();

		if(ImageData.m_BmInfo->bmiHeader.biHeight < rect.Height())
		{
			nHeight = ImageData.m_BmInfo->bmiHeader.biHeight;
		}
		else nHeight = rect.Height();

		SetDIBitsToDevice(pDC->GetSafeHdc()			
						  ,1						
						  ,1						
						  ,nWidth					
						  ,nHeight					
						  ,nStartXPos				
						  ,nStartYPos				
						  ,0						
						  ,ImageData.m_BmInfo->bmiHeader.biHeight
						  ,ImageData.m_pImageData	
						  ,ImageData.m_BmInfo		
						  ,DIB_RGB_COLORS);			
	
	}
	else if(nTabType == P_REVIEW)
	{
		int nNewImgLowerLeftYpos = 0;
		int nWidth, nHeight;

		if(ImageData.m_BmInfo->bmiHeader.biWidth*(int)fZoomFactor < rect.Width())
		{
			nWidth = ImageData.m_BmInfo->bmiHeader.biWidth*(int)fZoomFactor;
		}
		else nWidth = rect.Width() ;

		if(ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor < rect.Height())
		{
			nHeight = ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor;
		}
		else nHeight = rect.Height();

		if(fZoomFactor == 1.0)
		{
			SetDIBitsToDevice(pDC->GetSafeHdc()
							  ,1
							  ,1
							  ,nWidth
							  ,nHeight
							  ,nStartXPos
							  ,nStartYPos
							  ,0
							  ,ImageData.m_BmInfo->bmiHeader.biHeight
							  ,ImageData.m_pImageData
							  ,ImageData.m_BmInfo
							  ,DIB_RGB_COLORS);
		}
		else
		{
			nNewImgLowerLeftYpos = ( nStartYPos * (int)fZoomFactor ) + (int)((fZoomFactor - 1) * rect.Height()) ;
			
			SetDIBitsToDevice(pDC->GetSafeHdc()
							  ,1
							  ,1
							  ,nWidth
							  ,nHeight
							  ,nStartXPos * (int)fZoomFactor
							  ,nStartYPos
							  ,0
							  ,ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor
							  ,ImageData.m_pProcessedImageData
							  ,m_ZImgInfo
							  ,DIB_RGB_COLORS);
		}
	}
	else if(nTabType == P_PARM)
	{
		int nNewImgLowerLeftYpos = 0;
		int nWidth, nHeight;

		if(ImageData.m_BmInfo->bmiHeader.biWidth*(int)fZoomFactor < rect.Width())
		{
			nWidth = ImageData.m_BmInfo->bmiHeader.biWidth*(int)fZoomFactor;
		}
		else nWidth = rect.Width();

		if(ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor < rect.Height())
		{
			nHeight = ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor;
		}
		else nHeight = rect.Height();

		if(fZoomFactor == 1.0)
		{
			SetDIBitsToDevice(pDC->GetSafeHdc()
							  ,1
							  ,1
							  ,nWidth
							  ,nHeight
							  ,nStartXPos
							  ,nStartYPos
							  ,0
							  ,ImageData.m_BmInfo->bmiHeader.biHeight
							  ,ImageData.m_pImageData
							  ,ImageData.m_BmInfo
							  ,DIB_RGB_COLORS);
		}
		else
		{
			nNewImgLowerLeftYpos = (nStartYPos) * (int)fZoomFactor + (int)((fZoomFactor - 1) * rect.Height()) ;//-1;
			
			SetDIBitsToDevice(pDC->GetSafeHdc()
				              ,1
							  ,1
							  ,nWidth
							  ,nHeight
							  ,nStartXPos*(int)fZoomFactor
							  ,nStartYPos
							  ,0
							  ,ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor
							  ,ImageData.m_pProcessedImageData
							  ,m_ZImgInfo
							  ,DIB_RGB_COLORS);
		}
	}
	//+ 2007.9.19 Del BY USY
//	else if(nTabType == P_AVERAGE)
//	{
//		int nNewImgLowerLeftYpos = 0;
//		int nWidth, nHeight;
//
//		if(ImageData.m_BmInfo->bmiHeader.biWidth*(int)fZoomFactor < rect.Width())
//		{
//			nWidth = ImageData.m_BmInfo->bmiHeader.biWidth*(int)fZoomFactor;
//		}
//		else nWidth = rect.Width();
//
//		if(ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor < rect.Height())
//		{
//			nHeight = ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor;
//		}
//		else nHeight = rect.Height();
//
//		if(fZoomFactor == 1.0)
//		{
//			SetDIBitsToDevice(pDC->GetSafeHdc()
//							  ,1
//							  ,1
//							  ,nWidth
//							  ,nHeight
//							  ,nStartXPos
//							  ,nStartYPos
//							  ,0
//							  ,ImageData.m_BmInfo->bmiHeader.biHeight
//							  ,ImageData.m_pImageData
//							  ,ImageData.m_BmInfo
//							  ,DIB_RGB_COLORS);
//		}
//		else
//		{
//			nNewImgLowerLeftYpos = (nStartYPos) * (int)fZoomFactor + (int)((fZoomFactor - 1) * rect.Height()) ;//-1;
//			
//			SetDIBitsToDevice(pDC->GetSafeHdc()
//				              ,1
//							  ,1
//							  ,nWidth
//							  ,nHeight
//							  ,nStartXPos*(int)fZoomFactor
//							  ,nStartYPos
//							  ,0
//							  ,ImageData.m_BmInfo->bmiHeader.biHeight*(int)fZoomFactor
//							  ,ImageData.m_pProcessedImageData
//							  ,m_ZImgInfo
//							  ,DIB_RGB_COLORS);
//		}
//	}
	//-
	if(hOldPalette != NULL)
	{
		::SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
	}
	pWnd->ReleaseDC(pDC);
}

void CImageTotalDlg::ZoomIn(int nTabType, int nImageType,CBaseImage& ImageData)
{
	int height = ImageData.m_InfoHeader.biHeight;
	int width  = ImageData.m_InfoHeader.biWidth;

	int new_height	=(int)(height * m_fZFactor[nTabType][nImageType]);
	int new_width	=(int)(width  * m_fZFactor[nTabType][nImageType]);

	int nProgress = 0;
	int nOrgRowWidth = WIDTHBYTES(ImageData.m_BmInfo->bmiHeader.biBitCount*width);
	int nRowWidth	 = WIDTHBYTES(ImageData.m_BmInfo->bmiHeader.biBitCount*new_width);

	BYTE *pZoomImg = new BYTE[new_height*nRowWidth];

	int nZoomFactor = (int)m_fZFactor[nTabType][m_nImgType];

	int nRvalue = 0;
	int nGvalue = 0;
	int nBvalue = 0;

	m_ctrlProgress.SetPos(0);

	for(int r=0;r<height;r++)
	{
		for(int c=0;c<width;c++)
		{
			nRvalue = ImageData.m_pImageData[r*nOrgRowWidth+c*3+2];
			nGvalue = ImageData.m_pImageData[r*nOrgRowWidth+c*3+1];
			nBvalue = ImageData.m_pImageData[r*nOrgRowWidth+c*3];

			for(int j=0; j<nZoomFactor;j++)
			{
				for(int k=0; k<nZoomFactor;k++)
				{
					pZoomImg[((r*nZoomFactor+j))*nRowWidth+(c*nZoomFactor+k)*3+2]	= nRvalue;
					pZoomImg[((r*nZoomFactor+j))*nRowWidth+(c*nZoomFactor+k)*3+1]	= nGvalue;
					pZoomImg[((r*nZoomFactor+j))*nRowWidth+(c*nZoomFactor+k)*3]		= nBvalue;
				}
			}
		} 
		nProgress = (int)((float)r*90/(float)(height-1));
		m_ctrlProgress.SetPos(nProgress);
	}
	
	if(ImageData.m_pProcessedImageData != NULL)
	{ 
		free(ImageData.m_pProcessedImageData);
		ImageData.m_pProcessedImageData = NULL;
	}

	ImageData.m_bProcessedImageLoaded = FALSE;

	ImageData.m_pProcessedImageData = (BYTE *)malloc(new_height*nRowWidth); 
	memset(ImageData.m_pProcessedImageData,0,new_height*nRowWidth);

	for(int i=0;i<(new_height*nRowWidth);i++)
	{
		ImageData.m_pProcessedImageData[i] = pZoomImg[i];
	}

	delete [] pZoomImg;

	// Set BITMAPINFO
	if(m_ZImgInfo != NULL) free(m_ZImgInfo); m_ZImgInfo = NULL;

	m_ZImgInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO));

	m_ZImgInfo->bmiHeader.biBitCount		= ImageData.m_InfoHeader.biBitCount;
	m_ZImgInfo->bmiHeader.biClrImportant	= ImageData.m_InfoHeader.biClrImportant;
	m_ZImgInfo->bmiHeader.biClrUsed			= ImageData.m_InfoHeader.biClrUsed;
	m_ZImgInfo->bmiHeader.biCompression		= ImageData.m_InfoHeader.biCompression;
	m_ZImgInfo->bmiHeader.biHeight			= new_height;
	m_ZImgInfo->bmiHeader.biPlanes			= ImageData.m_InfoHeader.biPlanes;
	m_ZImgInfo->bmiHeader.biSize			= ImageData.m_InfoHeader.biSize;
	m_ZImgInfo->bmiHeader.biSizeImage		= nRowWidth * new_height;
	m_ZImgInfo->bmiHeader.biWidth			= new_width;
	m_ZImgInfo->bmiHeader.biXPelsPerMeter	= ImageData.m_InfoHeader.biXPelsPerMeter;
	m_ZImgInfo->bmiHeader.biYPelsPerMeter	= ImageData.m_InfoHeader.biYPelsPerMeter;

	ImageData.m_bProcessedImageLoaded = TRUE;

//	InitScrollBar(nTabType, nImageType);
	m_nScrHCur[m_nTabType][m_nImgType] = 0;
	m_nScrVCur[m_nTabType][m_nImgType] = 0;
	SetHScrollBar(nTabType, nImageType);
	SetVScrollBar(nTabType, nImageType);
	m_ctrlProgress.SetPos(100);
}

void CImageTotalDlg::InitScrollBar(int nTabType, int nImgType)
{
	//Scroll Value & StarX/Y Value
	
	//H-Value
	m_nScrHMax[m_nTabType][m_nImgType] = m_Image[m_nImgType]->m_InfoHeader.biWidth - m_nImgViewWidth;
	m_nStartX[m_nTabType][m_nImgType] = m_nScrHCur[m_nTabType][m_nImgType] = 0;

	//Control
	if(m_nScrHMax[m_nTabType][m_nImgType] > 0)
	{
		m_ctrlImgHScroll.SetScrollRange(0, m_nScrHMax[m_nTabType][m_nImgType]);
		m_ctrlImgHScroll.ShowScrollBar(TRUE);
		m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
		m_ctrlImgHScroll.SetScrollPos(m_nScrHCur[m_nTabType][m_nImgType], TRUE);
	}
	else
	{
		m_ctrlImgHScroll.ShowScrollBar(FALSE);
	}
	
	//V-Value
	m_nScrVMax[m_nTabType][m_nImgType] = m_Image[m_nImgType]->m_InfoHeader.biHeight - m_nImgViewHeight;
	m_nScrVCur[m_nTabType][m_nImgType] = 0;
	
	//Control
	if(m_nScrVMax[m_nTabType][m_nImgType] > 0)
	{
		m_ctrlImgVScroll.SetScrollRange(0, m_nScrVMax[m_nTabType][m_nImgType]);
		m_ctrlImgVScroll.ShowScrollBar(TRUE);
		m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
		m_ctrlImgVScroll.SetScrollPos(m_nScrVCur[m_nTabType][m_nImgType], TRUE);
		m_nStartY[m_nTabType][m_nImgType] = (m_Image[m_nImgType]->m_InfoHeader.biHeight - m_nScrVCur[m_nTabType][m_nImgType] - 1) - m_ImgViewRect.Height() + 1;
	}
	else
	{
		m_ctrlImgVScroll.ShowScrollBar(FALSE);
		m_nStartY[m_nTabType][m_nImgType] = 0;
	}
}
void CImageTotalDlg::SetVScrollBar(int nTabType, int nImgType)
{
	if(m_fZFactor[nTabType][nImgType] == 1.0)
	{
		m_nScrVMax[nTabType][nImgType] = m_Image[nImgType]->m_InfoHeader.biHeight - m_nImgViewHeight;
		if(m_nScrVMax[nTabType][nImgType] > 0)
		{
			m_ctrlImgVScroll.SetScrollRange(0, m_nScrVMax[m_nTabType][m_nImgType]);
			m_ctrlImgVScroll.ShowScrollBar(TRUE);
			m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgVScroll.SetScrollPos(m_nScrVCur[m_nTabType][m_nImgType], TRUE);

			m_nStartY[nTabType][nImgType] = (m_Image[nImgType]->m_InfoHeader.biHeight - m_nScrVCur[nTabType][nImgType] - 1) - m_ImgViewRect.Height()+ 1;
		}
		else
		{
			m_ctrlImgVScroll.SetScrollRange(0, 0);
			m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgVScroll.SetScrollPos(0, TRUE);
			m_ctrlImgVScroll.ShowScrollBar(FALSE);
			
		}
	}
	else 
	{	
		m_nScrVMax[nTabType][nImgType] = m_ZImgInfo->bmiHeader.biHeight - m_nImgViewHeight;
		if(m_nScrVMax[nTabType][nImgType] > 0)
		{
			m_ctrlImgVScroll.SetScrollRange(0, m_nScrVMax[m_nTabType][m_nImgType]);
			m_ctrlImgVScroll.ShowScrollBar(TRUE);
			m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgVScroll.SetScrollPos(m_nScrVCur[m_nTabType][m_nImgType], TRUE);

			m_nStartY[nTabType][nImgType] = (m_ZImgInfo->bmiHeader.biHeight - m_nScrVCur[nTabType][nImgType] - 1) -  m_ImgViewRect.Height()+ 1;
		}
		else
		{
			m_ctrlImgVScroll.SetScrollRange(0, 0);
			m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgVScroll.SetScrollPos(0, TRUE);
			m_ctrlImgVScroll.ShowScrollBar(FALSE);
			
		}
	}
}
void CImageTotalDlg::SetHScrollBar(int nTabType, int nImgType)
{
	if(m_fZFactor[m_nTabType][m_nImgType] == 1.0)
	{
		m_nScrHMax[m_nTabType][m_nImgType] = m_Image[m_nImgType]->m_InfoHeader.biWidth - m_nImgViewWidth;
		if(m_nScrHMax[m_nTabType][m_nImgType] > 0)
		{
			m_ctrlImgHScroll.SetScrollRange(0, m_nScrHMax[m_nTabType][m_nImgType]);
			m_ctrlImgHScroll.ShowScrollBar(TRUE);
			m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgHScroll.SetScrollPos(m_nScrHCur[m_nTabType][m_nImgType], TRUE);

			m_nStartX[m_nTabType][m_nImgType] = m_nScrHCur[m_nTabType][m_nImgType];
		}
		else
		{
			m_ctrlImgHScroll.SetScrollRange(0, 0);
			m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgHScroll.SetScrollPos(0, TRUE);
			m_ctrlImgHScroll.ShowScrollBar(FALSE);
			
		}
	}
	else 
	{
		m_nScrHMax[m_nTabType][m_nImgType] = m_ZImgInfo->bmiHeader.biWidth - m_nImgViewWidth;
		if(m_nScrHMax[m_nTabType][m_nImgType] > 0)
		{
			m_ctrlImgHScroll.SetScrollRange(0, m_nScrHMax[m_nTabType][m_nImgType]);
			m_ctrlImgHScroll.ShowScrollBar(TRUE);
			m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgHScroll.SetScrollPos(m_nScrHCur[m_nTabType][m_nImgType], TRUE);

			m_nStartX[m_nTabType][m_nImgType] = m_nScrHCur[m_nTabType][m_nImgType] / (int)m_fZFactor[m_nTabType][m_nImgType];
		}
		else
		{
			m_ctrlImgHScroll.SetScrollRange(0, 0);
			m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgHScroll.SetScrollPos(0, TRUE);
			m_ctrlImgHScroll.ShowScrollBar(FALSE);
			
		}
	}
}
//-

BOOL CImageTotalDlg::OnInitDialog() 
{ 
	CDialog::OnInitDialog();

	m_pImgViewWnd = (CWnd*)(CStatic*)GetDlgItem(IDC_STATIC_IMAGE_VIEW);
	m_pPageWnd    = GetDlgItem(IDC_STATIC_PAGE);

	m_GrabPage.Create(IDD_GRAB_PAGE, this);
	m_ReviewPage.Create(IDD_REVIEW_PAGE, this);
	m_ParmPage.Create(IDD_PARM_ADJ_PAGE, this);

	//+add kwmoon 071106
	if(CurrentSet->sRemoconTitle.IsEmpty())
	{
		AfxMessageBox(IDS_ERR_REMOTE);
	}
	else
	{
		OpenRemoteFile(CurrentSet->sRemoconTitle);
	}

	//+add kwmoon 071214
	if(CurrentSet->sPatternTitle.IsEmpty())
	{
		AfxMessageBox(IDS_ERR_PATTERN);
	}
	else
	{
		OpenPatternFile(CurrentSet->sPatternTitle);
	}


//	long SizeX, SizeY;
	InitAudioMeasurement();

	if(AnalogControl.m_bGrabberInit)
	{
		m_pImgViewWnd = (CWnd*)(CStatic*)GetDlgItem(IDC_STATIC_IMAGE_VIEW);
		
		MdispSelectWindow(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage, m_pImgViewWnd->GetSafeHwnd());
		/*Filled with the selected image buffer using an automatically calculated zoom factor. */
		MdispControl(AnalogControl.m_MilDisplay, M_FILL_DISPLAY , M_ENABLE);
	}
	UpdateData(FALSE);
	PostMessage(UM_CALC_MOVE_SUB_WINDOW);

	ShowWindow(SW_SHOW);

	SetTimer(1,1000,NULL);

	CRect rect;
	//GetDlgItem(IDC_STATIC_INST_DISPLAY_R)->GetWindowRect(rect);
	//ScreenToClient(rect);
	//m_oglWindowRight.oglCreate(rect, this);
	//m_oglWindowRight.m_unpTimer = m_oglWindowRight.SetTimer(1, 100, 0);
	//m_oglWindowRight.m_DisplayPage = __DISPLAY_PAGE_TOTAL_RIGHT;


	GetDlgItem(IDC_STATIC_INST_DISPLAY_L)->GetWindowRect(rect);
	ScreenToClient(rect);
	mp_oglWindowLeft = new COpenGLControlSub();
	mp_oglWindowLeft->oglCreate(rect, this, "OpenGLTotal");	
	mp_oglWindowLeft->m_unpTimer = mp_oglWindowLeft->SetTimer(1, 100, 0);
	mp_oglWindowLeft->m_DisplayPage = __DISPLAY_PAGE_TOTAL_LEFT;

#if 0//ndef AUDIO_DEBUG__
	m_oglWindowLeft.oglCreate(rect, this, "OpenGLTotal");	
	m_oglWindowLeft.m_unpTimer = m_oglWindowLeft.SetTimer(1, 100, 0);
	m_oglWindowLeft.m_DisplayPage = __DISPLAY_PAGE_TOTAL_LEFT;
#endif
	m_cLabelSoundLeftFREQ.SubclassDlgItem(IDC_TOTAL_TEXT_FREQ_LEFT, this);//IDC_TEXT_FREQ_LEFT // ctrlPLC_COM_Status.SubclassDlgItem(IDC_PLC_COM_STATUS,this);
	m_cLabelSoundLeftVolt.SubclassDlgItem(IDC_TEXT_TOTAL_VOLT_LEFT, this);//IDC_TEXT_VOLT_LEFT
	m_cLabelSoundRightFREQ.SubclassDlgItem(IDC_TOTAL_TEXT_FREQ_RIGHT, this);//IDC_TEXT_FREQ_RIGHT
	m_cLabelSoundRightVolt.SubclassDlgItem(IDC_TOTAL_TEXT_VOLT_RIGHT, this);//IDC_TEXT_VOLT_RIGHT

	m_cLabelSoundLeftFREQ.SetWindowText(_T("0 Hz"));
	m_cLabelSoundLeftFREQ.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundLeftFREQ.SetTextColor(RGB(0, 255, 0));

	m_cLabelSoundLeftVolt.SetWindowText(_T("0 mV"));
	m_cLabelSoundLeftVolt.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundLeftVolt.SetTextColor(RGB(0, 255, 0));

	m_cLabelSoundRightFREQ.SetWindowText(_T("0 Hz"));
	m_cLabelSoundRightFREQ.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundRightFREQ.SetTextColor(RGB(0, 255, 0));

	m_cLabelSoundRightVolt.SetWindowText(_T("0 mV"));
	m_cLabelSoundRightVolt.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundRightVolt.SetTextColor(RGB(0, 255, 0));

	LOGFONT logFont5;
	logFont5.lfHeight = 30;
	logFont5.lfWidth = 10;
	logFont5.lfEscapement = 0;
	logFont5.lfOrientation = 0;
	logFont5.lfWeight = FW_BOLD;
	logFont5.lfItalic = 0;
	logFont5.lfUnderline = 0;
	logFont5.lfStrikeOut = 0;
	logFont5.lfOutPrecision = 1;
	logFont5.lfClipPrecision = 2;
	logFont5.lfQuality = 1;
	logFont5.lfPitchAndFamily = 17;
	logFont5.lfCharSet = ANSI_CHARSET;
	strcpy_s(logFont5.lfFaceName, _countof(logFont5.lfFaceName), (LPCTSTR)"Arial");
	//strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Courier New");
	m_AudioFont.CreateFontIndirect(&logFont5);


	//ctrlPLC_COM_Status.SetFont(&m_AudioFont);
	m_cLabelSoundLeftFREQ.SetFont(&m_AudioFont);
	m_cLabelSoundLeftVolt.SetFont(&m_AudioFont);
	m_cLabelSoundRightFREQ.SetFont(&m_AudioFont);
	m_cLabelSoundRightVolt.SetFont(&m_AudioFont);



	SetTimer(2, 300, NULL);



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//+add kwmoon 080724
void CImageTotalDlg::InitAudioMeasurement() 
{
//	m_ctrlSoundCard.Creat(this);
}

void CImageTotalDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int Rvalue = 0;
	int Gvalue = 0;
	int Bvalue = 0;
	int nBytesInRow = 0;

	CString sTmp = _T("");

	if(m_nTabType == P_CROP)
	{

	}
	else if(m_nTabType == P_REVIEW)
	{
		
		//Get Rgb Value From Grab Image
		if(!m_ReviewPage.GetRgbValue(I_GRAB, *m_Image[I_GRAB], m_nXPos, m_nYPos, Rvalue, Gvalue, Bvalue)) return;

		sTmp.Format("%3d", Rvalue);
		m_ReviewPage.m_ctrlGrabRValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Gvalue);
		m_ReviewPage.m_ctrlGrabGValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Bvalue);
		m_ReviewPage.m_ctrlGrabBValStatic.SetWindowText(sTmp);

		//Get Rgb Value From Ref Image
		if(!m_ReviewPage.GetRgbValue(I_REF, *m_Image[I_REF], m_nXPos, m_nYPos, Rvalue, Gvalue, Bvalue)) return;
		sTmp.Format("%3d", Rvalue);
		m_ReviewPage.m_ctrlRefRValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Gvalue);
		m_ReviewPage.m_ctrlRefGValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Bvalue);
		m_ReviewPage.m_ctrlRefBValStatic.SetWindowText(sTmp);

		sTmp.Format("%d", m_nXPos);
		m_ReviewPage.m_ctrlXPosEdit.SetWindowText(sTmp); 	
		sTmp.Format("%d", m_nYPos);
		m_ReviewPage.m_ctrlYPosEdit.SetWindowText(sTmp);		
		
	}
	else if(m_nTabType == P_PARM)
	{
		
		//Get Rgb Value From Grab Image
		if(!m_ParmPage.GetRgbValue(I_GRAB, *m_Image[I_GRAB], m_nXPos, m_nYPos, Rvalue, Gvalue, Bvalue)) return;
		sTmp.Format("%3d", Rvalue);
		m_ParmPage.m_ctrlGrabRValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Gvalue);
		m_ParmPage.m_ctrlGrabGValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Bvalue);
		m_ParmPage.m_ctrlGrabBValStatic.SetWindowText(sTmp);

		//Get Rgb Value From Ref Image
		if(!m_ParmPage.GetRgbValue(I_REF, *m_Image[I_REF], m_nXPos, m_nYPos, Rvalue, Gvalue, Bvalue)) return;
		sTmp.Format("%3d", Rvalue);
		m_ParmPage.m_ctrlRefRValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Gvalue);
		m_ParmPage.m_ctrlRefGValStatic.SetWindowText(sTmp);
		sTmp.Format("%3d", Bvalue);
		m_ParmPage.m_ctrlRefBValStatic.SetWindowText(sTmp);

		sTmp.Format("%3d", m_nXPos);
		m_ParmPage.m_ctrlXPosEdit.SetWindowText(sTmp);
		sTmp.Format("%3d", m_nYPos);
		m_ParmPage.m_ctrlYPosEdit.SetWindowText(sTmp);

	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CImageTotalDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CImageTotalDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_nTabType == P_CROP)
	{
//		if(m_Crop.bMouse == TRUE)
//		{
//			if((point.x < m_MViewRect.left) || (point.x > m_MViewRect.right)) 
//			{
//				Invalidate(FALSE); 
//				return;
//			}
//			if((point.y < m_MViewRect.top) || (point.y > m_MViewRect.bottom)) 
//			{
//				Invalidate(FALSE); 
//				return;
//			}
//			int nPointX = point.x - m_MViewRect.left - 1;
//			int nPointY = point.y - m_MViewRect.top  - 1;
//
//			if((nPointX >= m_Image1.m_InfoHeader.biWidth)
//			|| (nPointY >= m_Image1.m_InfoHeader.biHeight)) 
//			{
//				Invalidate(FALSE);
//				return;
//			}
//
//			m_Crop.rCropRect.right = point.x;
//			m_Crop.rCropRect.bottom = point.y;
//			Invalidate(FALSE);
//	//		InvalidateRect(&m_OrgViewRect); 
//		}
//		else if(m_Crop.bMouse == FALSE)
//		{
//			CStatic* pStatic1 = (CStatic*)GetDlgItem(IDC_STATIC_CROP_X_POS);
//			CStatic* pStatic2 = (CStatic*)GetDlgItem(IDC_STATIC_CROP_Y_POS);
//			CStatic* pStatic3 = (CStatic*)GetDlgItem(IDC_STATIC_CROP_R_VAL);
//			CStatic* pStatic4 = (CStatic*)GetDlgItem(IDC_STATIC_CROP_G_VAL);
//			CStatic* pStatic5 = (CStatic*)GetDlgItem(IDC_STATIC_CROP_B_VAL);
//
//			CString sTmp;
//			if((point.x >  m_MViewRect.left)
//			&& (point.x <= m_MViewRect.right)
//			&& (point.y >  m_MViewRect.top)
//			&& (point.y <= m_MViewRect.bottom))
//			{
//				int nPointX = point.x - m_MViewRect.left - 1;
//				int nPointY = point.y - m_MViewRect.top  - 1;
//
//				int nImageXpos = nPointX + m_Crop.nHCurPos[I_ORG]; 
//				int nImageYpos = nPointY + m_Crop.nVCurPos[I_ORG];  
//
//				//+move kwmoon 070201[#5]
//				//+add kwmoon 070131[#1]
//				if((nImageXpos >= m_Image1.m_InfoHeader.biWidth)
//				|| (nImageYpos >= m_Image1.m_InfoHeader.biHeight)) return;
//				//-
//
//				sTmp.Format("%d",nImageXpos);
//				pStatic1->SetWindowText(sTmp);
//				sTmp.Format("%d",nImageYpos);
//				pStatic2->SetWindowText(sTmp);
//
//				int nBytesInRow, Rvalue, Gvalue, Bvalue;
//				nBytesInRow  = WIDTHBYTES(m_Image1.m_InfoHeader.biBitCount * m_Image1.m_InfoHeader.biWidth);
//				Rvalue = m_Image1.m_pImageData[(m_Image1.m_InfoHeader.biHeight-nImageYpos -1 )*nBytesInRow+3*nImageXpos+2];   
//				Gvalue = m_Image1.m_pImageData[(m_Image1.m_InfoHeader.biHeight-nImageYpos -1 )*nBytesInRow+3*nImageXpos+1]; 
//				Bvalue = m_Image1.m_pImageData[(m_Image1.m_InfoHeader.biHeight-nImageYpos -1 )*nBytesInRow+3*nImageXpos]; 
//
//				sTmp.Format("%3d",Rvalue);
//				pStatic3->SetWindowText(sTmp);
//				sTmp.Format("%3d",Gvalue);
//				pStatic4->SetWindowText(sTmp);
//				sTmp.Format("%3d",Bvalue);
//				pStatic5->SetWindowText(sTmp);
//			}
//		}
	}

	else if((m_nTabType == P_PARM)||(m_nTabType == P_REVIEW))
	{
		//+ 2007.5.1 Add BY USY
		if((m_nImgType <I_GRAB)||(m_nImgType > I_MASK)) return;
//		if((m_nImgType <I_GRAB)||(m_nImgType > I_PROC)) return;
		//-
		CString sTmp;
		if(m_Image[m_nImgType] == FALSE) return;

		if((point.x >  m_ImgViewRect.left)
		&& (point.x <= m_ImgViewRect.right)
		&& (point.y >  m_ImgViewRect.top)
		&& (point.y <= m_ImgViewRect.bottom))
		{
			int nPointX = point.x - m_ImgViewRect.left - 1;
			int nPointY = point.y - m_ImgViewRect.top  - 1;

			m_nXPos = nPointX + m_nScrHCur[m_nTabType][m_nImgType]; 
			m_nYPos = nPointY + m_nScrVCur[m_nTabType][m_nImgType];  

			if((m_nXPos >= m_Image[m_nImgType]->m_InfoHeader.biWidth * (int)m_fZFactor[m_nTabType][m_nImgType])
			||(m_nYPos >= m_Image[m_nImgType]->m_InfoHeader.biHeight * (int)m_fZFactor[m_nTabType][m_nImgType])) return;
			

			//+add kwmoon 070125
			m_nXPos /= (int)m_fZFactor[m_nTabType][m_nImgType];
			m_nYPos /= (int)m_fZFactor[m_nTabType][m_nImgType];
			
			sTmp.Format("%d",m_nXPos);
			m_ctrlXPosStatic.SetWindowText(sTmp);
			sTmp.Format("%d",m_nYPos);
			m_ctrlYPosStatic.SetWindowText(sTmp);

			int nBytesInRow, Rvalue, Gvalue, Bvalue;

			nBytesInRow  = WIDTHBYTES(m_Image[m_nImgType]->m_InfoHeader.biBitCount * m_Image[m_nImgType]->m_InfoHeader.biWidth);
			Rvalue = m_Image[m_nImgType]->m_pImageData[(m_Image[m_nImgType]->m_InfoHeader.biHeight-m_nYPos -1 )*nBytesInRow+3*m_nXPos+2];   
			Gvalue = m_Image[m_nImgType]->m_pImageData[(m_Image[m_nImgType]->m_InfoHeader.biHeight-m_nYPos -1 )*nBytesInRow+3*m_nXPos+1]; 
			Bvalue = m_Image[m_nImgType]->m_pImageData[(m_Image[m_nImgType]->m_InfoHeader.biHeight-m_nYPos -1 )*nBytesInRow+3*m_nXPos]; 

			sTmp.Format("%3d",Rvalue);
			m_ctrlRValStatic.SetWindowText(sTmp);
			sTmp.Format("%3d",Gvalue);
			m_ctrlGValStatic.SetWindowText(sTmp);
			sTmp.Format("%3d",Bvalue);
			m_ctrlBValStatic.SetWindowText(sTmp);
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CImageTotalDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(m_nTabType == P_GRAB)
	{
		if(m_Image[0]->m_bImageLoaded == TRUE)
		{
			DrawImage(P_GRAB, m_nImgType, IDC_STATIC_IMAGE_VIEW, *m_Image[0], 1, 1, 1);
		}
	}
	else if(m_nTabType == P_CROP)
	{

	}
	else if(m_nTabType == P_REVIEW)
	{
		if((m_nImgType >= I_GRAB)&&(m_nImgType <= I_MASK))
		{
			if(m_ReviewPage.m_bZoomAllReset == TRUE)
			{
				for(int nTmp = I_GRAB; nTmp <= I_MASK; nTmp++)
				{
					m_fZFactor[m_nTabType][nTmp] = 1.0f;
				}
				m_ctrlZoomCmb.SetCurSel(0);
				m_ReviewPage.m_bZoomAllReset = FALSE;
			}

			if(m_ReviewPage.m_bFirstOpen[m_nImgType] == TRUE)
			{	
				InitScrollBar(m_nTabType, m_nImgType);
				SetImgChkBtn(m_nTabType, m_nImgType);

				m_ReviewPage.m_bFirstOpen[m_nImgType] = FALSE;
			}
			else if(m_ReviewPage.m_bFirstOpen[m_nImgType] == FALSE)
			{

			}
			
			if(m_Image[m_nImgType]->m_bImageLoaded == TRUE)
			{
				if(m_fZFactor[m_nTabType][m_nImgType] == 1.0)
				{
					DrawImage(P_REVIEW, m_nImgType, IDC_STATIC_IMAGE_VIEW, *m_Image[m_nImgType], m_nStartX[m_nTabType][m_nImgType], m_nStartY[m_nTabType][m_nImgType], m_fZFactor[m_nTabType][m_nImgType]);
				}
				else if((m_fZFactor[m_nTabType][m_nImgType] != 1.0)
				&& (m_Image[m_nImgType]->m_bProcessedImageLoaded == TRUE) && (m_ZImgInfo != NULL))
				{
					DrawImage(P_REVIEW, m_nImgType, IDC_STATIC_IMAGE_VIEW, *m_Image[m_nImgType], m_nStartX[m_nTabType][m_nImgType], m_nStartY[m_nTabType][m_nImgType], m_fZFactor[m_nTabType][m_nImgType]);
				}
				else if((m_fZFactor[m_nTabType][m_nImgType] != 1.0)
				&&(m_Image[m_nImgType]->m_bProcessedImageLoaded == TRUE) && (m_ZImgInfo == NULL))
				{
					ZoomIn(m_nTabType, m_nImgType, *m_Image[m_nImgType]);	
					DrawImage(P_REVIEW, m_nImgType, IDC_STATIC_IMAGE_VIEW, *m_Image[m_nImgType], m_nStartX[m_nTabType][m_nImgType], m_nStartY[m_nTabType][m_nImgType], m_fZFactor[m_nTabType][m_nImgType]);
				}
			}
			SetHScrollBar(m_nTabType, m_nImgType);
			SetVScrollBar(m_nTabType, m_nImgType);
		}
	}
	else if(m_nTabType == P_PARM)
	{
		if((m_nImgType >= I_GRAB)&&(m_nImgType <= I_MASK))
		{
			if(m_ParmPage.m_bZoomAllReset == TRUE)
			{
				for(int nTmp = I_GRAB; nTmp <= I_MASK; nTmp++)
				{
					m_fZFactor[m_nTabType][nTmp] = 1.0f;
				}
				m_ctrlZoomCmb.SetCurSel(0);
				m_ParmPage.m_bZoomAllReset = FALSE;
			}
			if(m_ParmPage.m_bFirstOpen[m_nImgType] == TRUE)
			{	
				InitScrollBar(m_nTabType, m_nImgType);
				SetImgChkBtn(m_nTabType, m_nImgType);

				m_ParmPage.m_bFirstOpen[m_nImgType] = FALSE;
			}
			else if(m_ParmPage.m_bFirstOpen[m_nImgType] == FALSE)
			{

			}
			if(m_Image[m_nImgType]->m_bImageLoaded == TRUE)
			{
				if(m_fZFactor[m_nTabType][m_nImgType] == 1.0)
				{
					DrawImage(P_PARM, m_nImgType, IDC_STATIC_IMAGE_VIEW, *m_Image[m_nImgType], m_nStartX[m_nTabType][m_nImgType], m_nStartY[m_nTabType][m_nImgType], m_fZFactor[m_nTabType][m_nImgType]);
				}
				else if((m_fZFactor[m_nTabType][m_nImgType] != 1.0)&&(m_Image[m_nImgType]->m_bProcessedImageLoaded == TRUE) && (m_ZImgInfo != NULL))
				{
					DrawImage(P_PARM, m_nImgType, IDC_STATIC_IMAGE_VIEW, *m_Image[m_nImgType], m_nStartX[m_nTabType][m_nImgType], m_nStartY[m_nTabType][m_nImgType], m_fZFactor[m_nTabType][m_nImgType]);
				}
				else if((m_fZFactor[m_nTabType][m_nImgType] != 1.0)&&(m_Image[m_nImgType]->m_bProcessedImageLoaded == TRUE) && (m_ZImgInfo == NULL))
				{
					ZoomIn(m_nTabType, m_nImgType, *m_Image[m_nImgType]);
					DrawImage(P_PARM, m_nImgType, IDC_STATIC_IMAGE_VIEW, *m_Image[m_nImgType], m_nStartX[m_nTabType][m_nImgType], m_nStartY[m_nTabType][m_nImgType], m_fZFactor[m_nTabType][m_nImgType]);
				}
			}
			SetHScrollBar(m_nTabType, m_nImgType);
			SetVScrollBar(m_nTabType, m_nImgType);
		}
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CImageTotalDlg::OnChkGrabPage() 
{
	m_nOldImgType[m_nTabType] = m_nImgType;
	m_nTabType = P_GRAB;
	
	SetChkBtn(m_nTabType, m_nOldImgType[m_nTabType]);
	Invalidate(TRUE);
}

void CImageTotalDlg::OnChkCropPage() 
{
	m_nOldImgType[m_nTabType] = m_nImgType;
	m_nTabType = P_CROP;
	SetChkBtn(m_nTabType, m_nOldImgType[m_nTabType]);	
	Invalidate(TRUE);
}

void CImageTotalDlg::OnChkReviewPage() 
{
	m_nOldImgType[m_nTabType] = m_nImgType;
	m_nTabType = P_REVIEW;
	

	SetChkBtn(m_nTabType, m_nOldImgType[m_nTabType]);
	Invalidate(TRUE);
}

void CImageTotalDlg::OnChkParmPage() 
{
	m_nOldImgType[m_nTabType] = m_nImgType;
	m_nTabType = P_PARM;

//	InitImgPanel();
	SetChkBtn(m_nTabType, m_nOldImgType[m_nTabType]);
	Invalidate(TRUE);
}

void CImageTotalDlg::OnChkGrabImg() 
{
	int nOldImgType = m_nImgType;
	m_nImgType = I_GRAB;
	if(m_Image[m_nImgType]->m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Grab Image is not loaded yet!");
		SetImgChkBtn(m_nTabType, nOldImgType);
		m_nImgType = nOldImgType;
		return;
	}
	
	SetImgChkBtn(m_nTabType, m_nImgType);
	SetZoomCmb(m_nTabType, m_nImgType);
	Invalidate(TRUE);
}

void CImageTotalDlg::OnChkRefImg() 
{
	int nOldImgType = m_nImgType;
	m_nImgType = I_REF;
	if(m_Image[m_nImgType]->m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Reference Image is not loaded yet!");
		SetImgChkBtn(m_nTabType, nOldImgType);	
		m_nImgType = nOldImgType;
		return;
	}
	SetImgChkBtn(m_nTabType, m_nImgType);
	SetZoomCmb(m_nTabType, m_nImgType);
	Invalidate(TRUE);

}

void CImageTotalDlg::OnChkProcImg() 
{
	int nOldImgType = m_nImgType;
	m_nImgType = I_PROC;
	if(m_Image[m_nImgType]->m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Processed Image is not loaded yet!");
		SetImgChkBtn(m_nTabType, nOldImgType);	
		m_nImgType = nOldImgType;
		return;
	}
	SetImgChkBtn(m_nTabType, m_nImgType);
	SetZoomCmb(m_nTabType, m_nImgType);
	Invalidate(TRUE);
}

void CImageTotalDlg::OnChkMaskImg() 
{
	int nOldImgType = m_nImgType;
	m_nImgType = I_MASK;
	if(m_Image[m_nImgType]->m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Mask Image is not loaded yet!");
		SetImgChkBtn(m_nTabType, nOldImgType);	
		m_nImgType = nOldImgType;
		return;
	}
	SetImgChkBtn(m_nTabType, m_nImgType);
	SetZoomCmb(m_nTabType, m_nImgType);
	Invalidate(TRUE);
}

void CImageTotalDlg::OnSelchangeCmbZoom()
{
	int nCurSel = m_ctrlZoomCmb.GetCurSel();
	int nZoomInPercentageIndex = m_ctrlZoomCmb.GetCurSel();

	float fPrevZoomFactor = m_fZFactor[m_nTabType][m_nImgType];

	switch(nZoomInPercentageIndex)
	{
		case 0 : m_fZFactor[m_nTabType][m_nImgType] = 1.0; break;
		case 1 : m_fZFactor[m_nTabType][m_nImgType] = 2.0; break;
		case 2 : m_fZFactor[m_nTabType][m_nImgType] = 4.0; break;
	}
	if(m_fZFactor[m_nTabType][m_nImgType] != fPrevZoomFactor)
	{
		if(m_fZFactor[m_nTabType][m_nImgType] == 1.0)
		{
			m_Image[m_nImgType]->m_bProcessedImageLoaded = FALSE;

			InitScrollBar(m_nTabType, m_nImgType);
		}
		else
		{
			m_Image[m_nImgType]->m_bProcessedImageLoaded = TRUE;

			ZoomIn(m_nTabType, m_nImgType, *m_Image[m_nImgType]);
		}		
	}
	Invalidate(FALSE);
}


void CImageTotalDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(pScrollBar == &m_ctrlImgHScroll)
	{
		int nTmp;
		nTmp = pScrollBar->GetScrollPos();
		switch(nSBCode) 
		{
			case SB_LINELEFT:
				if(nTmp > 0) nTmp -= 1;
				else nTmp = 0;
				break;
			case SB_PAGELEFT:
				if(nTmp > 10) nTmp -= 10;
				else nTmp = 0;
				break;
			case SB_LINERIGHT:
				if(nTmp < m_nScrHMax[m_nTabType][m_nImgType]) nTmp+= 1;
				else nTmp = m_nScrHMax[m_nTabType][m_nImgType];
				break;
			case SB_PAGERIGHT:
				if(nTmp < m_nScrHMax[m_nTabType][m_nImgType] - 10) nTmp+= 10;
				else nTmp = m_nScrHMax[m_nTabType][m_nImgType];
				break;
			case SB_THUMBTRACK:
				nTmp = nPos;
				break;
		}
		m_nScrHCur[m_nTabType][m_nImgType] = nTmp;
		pScrollBar->SetScrollPos(nTmp);

		SetHScrollBar(m_nTabType, m_nImgType);
		InvalidateRect(&m_ImgViewRect);
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CImageTotalDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(pScrollBar == &m_ctrlImgVScroll)
	{
		int nTmp;
		nTmp = pScrollBar->GetScrollPos();
		switch(nSBCode) 
		{
			case SB_LINELEFT:
				if(nTmp > 0) nTmp -= 1;
				else nTmp = 0;
				break;
			case SB_PAGELEFT:
				if(nTmp > 10) nTmp -= 10;
				else nTmp = 0;
				break;
			case SB_LINERIGHT:
				if(nTmp < m_nScrVMax[m_nTabType][m_nImgType]) nTmp+= 1;
				else nTmp = m_nScrVMax[m_nTabType][m_nImgType];
				break;
			case SB_PAGERIGHT:
				if(nTmp < m_nScrVMax[m_nTabType][m_nImgType] - 10) nTmp+= 10;
				else nTmp = m_nScrVMax[m_nTabType][m_nImgType];
				break;
			case SB_THUMBTRACK:
				nTmp = nPos;
				break;
		}
		m_nScrVCur[m_nTabType][m_nImgType] = nTmp;
		pScrollBar->SetScrollPos(nTmp);
		
		SetVScrollBar(m_nTabType, m_nImgType);
		
		//Invalidate(FALSE);
		InvalidateRect(&m_ImgViewRect);
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CImageTotalDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnEraseBkgnd(pDC);
}

void CImageTotalDlg::OnOK() 
{
	if(AnalogControl.m_bGrabberInit)
	{
		MdigHalt(AnalogControl.m_MilDigitizer);
		MdispDeselect(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage);
	}
	/*if(m_ctrlSoundCard.wIn_Flag)
	{
		m_ctrlSoundCard.WaveRead_Stop();
	}
	if(m_ctrlSoundCard.wOutL_Flag) m_ctrlSoundCard.WaveOutL_Stop();
	if(m_ctrlSoundCard.wOutR_Flag) m_ctrlSoundCard.WaveOutR_Stop();*/

	m_GrabPage.SendMessage(WM_CLOSE);		//Grab Page
	m_ReviewPage.SendMessage(WM_CLOSE);		//Review Page
	m_ParmPage.SendMessage(WM_CLOSE);		//Parm Adj Page

	CDialog::OnOK();
}

void CImageTotalDlg::OnClose() 
{
	if(AnalogControl.m_bGrabberInit)
	{
		MdigHalt(AnalogControl.m_MilDigitizer);
		MdispDeselect(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage);
	}
	//if(m_ctrlSoundCard.wIn_Flag)
	//{
	//	m_ctrlSoundCard.WaveRead_Stop();
	//}
	//if(m_ctrlSoundCard.wOutL_Flag) m_ctrlSoundCard.WaveOutL_Stop();
	//if(m_ctrlSoundCard.wOutR_Flag) m_ctrlSoundCard.WaveOutR_Stop();

	m_GrabPage.SendMessage(WM_CLOSE);		//Grab Page
	m_ReviewPage.SendMessage(WM_CLOSE);		//Review Page
	m_ParmPage.SendMessage(WM_CLOSE);		//Parm Adj Page

	mp_oglWindowLeft->destroy();
	//m_poglWindow->~COpenGLControl();
	delete mp_oglWindowLeft;

	CDialog::OnClose();
}

//+recover kwmoon 080724
#define TIMER_CHECK_AUDIO_IN	100

//+recover kwmoon 080724
void CImageTotalDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	double nLevel_L = 0;
	double nLevel_R = 0;

	static double sFreq_Left = 0;
	static double sVolume_Left = 0;
	static double sFreq_Right = 0;
	static double sVolume_Right = 0;
	double lFreq_Left = 0;
	double lVolume_Left = 0;
	double lFreq_Right = 0;
	double lVolume_Right = 0;
	CString sTemp;

	double freq = 0, amp = 0, dbamp = 0;
	double nCorrectionValue = 0.97;
	
	switch(nIDEvent)
	{
		//case TIMER_CHECK_AUDIO_IN :
		//{
		//	if(m_ctrlSoundCard.wIn_Flag)
		//	{
		//		// LEFT
		//		m_ctrlSoundCard.GetWavePeak_L(&freq, &amp, &dbamp);
		//		m_ctrlSoundCard.GetPeackDetectorPeak_L(&nLevel_L);
		//		m_ctrlSoundCard.SetLedValue_inL((long)freq);
		//		m_ctrlSoundCard.SetLedValue_inL_Level((long)(nLevel_L/(2*1.4142)*0.1*nCorrectionValue));

		//		// RIGHT
		//		m_ctrlSoundCard.GetWavePeak_R(&freq, &amp, &dbamp);
		//		m_ctrlSoundCard.GetPeackDetectorPeak_R(&nLevel_R);
		//		m_ctrlSoundCard.SetLedValue_inR((long)freq);
		//		m_ctrlSoundCard.SetLedValue_inR_Level((long)(nLevel_R/(2*1.4142)*0.1*nCorrectionValue));
		//	}
		//	break;
		//}
		case 1:
		{
		//	KillTimer(nIDEvent);

		//	m_nRadioAudioMeasureStart = AUDIO_MEASURE_START;
		////	UpdateData();
		//	OnRadioMeasure();
			break;
		}
		case 2:
			lFreq_Left = g_pView->m_Frequency_Left;
			lFreq_Right = g_pView->m_Frequency_Right;
			lVolume_Left = g_pView->m_Voltage_Left ;
			lVolume_Right = g_pView->m_Voltage_Right;
			if (sFreq_Left != lFreq_Left)
			{
				sFreq_Left = lFreq_Left;
				sTemp.Format(_T(" %4.0f Hz "), lFreq_Left);
				m_cLabelSoundLeftFREQ.SetWindowText(sTemp);
			}

			if (sVolume_Left != lVolume_Left)
			{
				sVolume_Left = lVolume_Left;
				sTemp.Format(_T(" %4.0f mV "), lVolume_Left);
				m_cLabelSoundLeftVolt.SetWindowText(sTemp);

			}

			if (sFreq_Right != lFreq_Right)
			{
				sFreq_Right = lFreq_Right;

				sTemp.Format(_T(" %4.0f Hz "), lFreq_Right);
				m_cLabelSoundRightFREQ.SetWindowText(sTemp);
			}

			if (sVolume_Right != lVolume_Right)
			{
				sVolume_Right = lVolume_Right;

				sTemp.Format(_T(" %4.0f mV "), lVolume_Right);
				m_cLabelSoundRightVolt.SetWindowText(sTemp);

			}
#if 0//ndef AUDIO_DEBUG__
			memcpy(m_oglWindowLeft.m_PlotDataL, g_pView->m_PlotDataL, sizeof(PlotData_T) * 1000);
			memcpy(m_oglWindowLeft.m_PlotDataR, g_pView->m_PlotDataR, sizeof(PlotData_T) * 1000);
#endif		
			memcpy(mp_oglWindowLeft->m_PlotDataL, g_pView->m_PlotDataL, sizeof(PlotData_T) * 1000);
			memcpy(mp_oglWindowLeft->m_PlotDataR, g_pView->m_PlotDataR, sizeof(PlotData_T) * 1000);
			//	m_oglWindow.m_PlotDataR[i].Data = lDbufferDisplayR[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
			//m_oglWindow.m_PlotDataL[i].Data = lDbufferDisplayL[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;


			break;

		default:
			break;
	}

	CDialog::OnTimer(nIDEvent);
}
void CImageTotalDlg::AudioMeasureStart()
{
//	if(!m_ctrlSoundCard.wIn_Flag) m_ctrlSoundCard.WaveRead_Start();	
	
	SetTimer(TIMER_CHECK_AUDIO_IN,500,NULL);
		
}
void CImageTotalDlg::AudioMeasureStop()
{
	//if(m_ctrlSoundCard.wIn_Flag) m_ctrlSoundCard.WaveRead_Stop();

	KillTimer(TIMER_CHECK_AUDIO_IN);
}

//+recover kwmoon 080724
void CImageTotalDlg::OnRadioMeasure() 
{
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	if(m_nRadioAudioMeasureStart == AUDIO_MEASURE_START)
	{
		AudioMeasureStart();
	}
	else // m_nRadioAudioMeasureStart == AUDIO_MEASURE_STOP : 
	{
		AudioMeasureStop();
	}
}


void CImageTotalDlg::OnChkAveragePage() 
{
	m_nOldImgType[m_nTabType] = m_nImgType;
	m_nTabType = P_AVERAGE;
	
	SetChkBtn(m_nTabType, m_nOldImgType[m_nTabType]);
	Invalidate(TRUE);
}

 LRESULT CImageTotalDlg::UpdateImageViewDisplay(WPARAM wParam, LPARAM lParam)
{
	//+change kwmoon 080709
//	InvalidateRect(&m_ImgViewRect,FALSE);
	InvalidateRect(&m_ImgViewRect,TRUE);
	//+add kwmoon 080628
	UpdateWindow();
	return 0;
}

 LRESULT CImageTotalDlg::UpdateAllViewTab(WPARAM wParam, LPARAM lParam)
{
	OnChkGrabImg();
	InitScrollBar(P_PARM, I_GRAB);
	
	OnChkRefImg();
	InitScrollBar(P_PARM, I_REF);
	
	OnChkProcImg();
	InitScrollBar(P_PARM, I_PROC);
	
	OnChkMaskImg();
	InitScrollBar(P_PARM, I_MASK);
	return 0;
}
//int CImageTotalDlg::DoModal() 
//{
//	// TODO: Add your specialized code here and/or call the base class
//	return CDialog::DoModal();
//}

 LRESULT CImageTotalDlg::CalcAndMoveSubWindow(WPARAM wParam, LPARAM lParam)
{
	CWnd* pImageWnd;

	pImageWnd = GetDlgItem(IDC_STATIC_IMAGE_VIEW);

//	m_pImgViewWnd->GetWindowRect(&m_ImgViewRect);
	pImageWnd->GetWindowRect(&m_ImgViewRect);
	ScreenToClient(&m_ImgViewRect);
	
	m_nImgViewHeight = m_ImgViewRect.bottom - m_ImgViewRect.top;
	m_nImgViewWidth  = m_ImgViewRect.right  - m_ImgViewRect.left;

	m_pPageWnd->GetWindowRect(&m_PageScreenRect);
	m_pPageWnd->GetWindowRect(&m_PageRect);
	ScreenToClient(&m_PageRect);
	
	m_nPageHeight = m_PageRect.bottom - m_PageRect.top;
	m_nPageWidth  = m_PageRect.right  - m_PageRect.left;

	m_pStaticWnd  = GetDlgItem(IDC_STATIC_GROUP_1);
	m_pStaticWnd->GetWindowRect(&m_StaticRect);
	ScreenToClient(&m_StaticRect);
	
	m_nPage2Height = m_StaticRect.bottom - m_StaticRect.top + m_nPageHeight;
	m_nPage2Width  = m_StaticRect.right - m_StaticRect.left + m_nPageWidth;

	m_pPageWnd->ShowWindow(SW_SHOW);
	
	// Set Position Of Sub Windows
	m_GrabPage.SetWindowPos(&wndTop,	m_PageRect.left, m_PageRect.top - 30,  m_PageRect.Width()  , m_nPage2Height ,	SWP_HIDEWINDOW);
	m_ReviewPage.SetWindowPos(&wndTop,	m_PageRect.left, m_PageRect.top,  m_PageRect.Width(), m_PageRect.Height(),	SWP_HIDEWINDOW);
	m_ParmPage.SetWindowPos(&wndTop,	m_PageRect.left, m_PageRect.top,  m_PageRect.Width(), m_PageRect.Height(),	SWP_HIDEWINDOW);

	InitImgPanel();

	SetPage(m_nTabType, m_nImgType);
	
	CString sTmp; 
	sTmp.Format("Image Total Dialog  - %s", CurrentSet->sModelFolder);
	SetWindowText(sTmp);
	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);

	OnChkGrabPage(); 

	m_GrabPage.ChkVideoAutoGrab();
	return 0;
}


 LRESULT CImageTotalDlg::ChangeActiveViewTab(WPARAM wParam, LPARAM lParam)
{
	int nActiveViewTab = (int)wParam;

	switch(nActiveViewTab)
	{
		case  I_GRAB	: OnChkGrabImg();	InitScrollBar(P_PARM, I_GRAB);break;
		case  I_REF		: OnChkRefImg();	InitScrollBar(P_PARM, I_REF); break;
		case  I_PROC	: OnChkProcImg();	InitScrollBar(P_PARM, I_PROC);break;
		case  I_MASK	: OnChkMaskImg();	InitScrollBar(P_PARM, I_MASK);break;
	}

	return 0;
}

void CImageTotalDlg::OnSelchangeCmbSelImage() 
{
	CString sTmp;
	int nOldImgType = m_nImgType;
	m_nImgType = m_ctrlSelImageCmb.GetCurSel();;
	

	if(m_Image[m_nImgType]->m_bImageLoaded == FALSE)
	{
		switch(m_ctrlSelImageCmb.GetCurSel()) 
		{
			case I_GRAB: sTmp.Format("Grab Image is not loaded yet!");		break;
			case I_REF: sTmp.Format("Reference Image is not loaded yet!");	break;
			case I_PROC: sTmp.Format("Processed Image is not loaded yet!"); break;
			case I_MASK: sTmp.Format("Mask Image is not loaded yet!");		break;
		}
		SetImgChkBtn(m_nTabType, nOldImgType);
		m_nImgType = nOldImgType;
		AfxMessageBox(sTmp);
		return;
	}
	SetImgChkBtn(m_nTabType, m_nImgType);
	SetZoomCmb(m_nTabType, m_nImgType);
	Invalidate(TRUE);
}
void CImageTotalDlg::VideoDispSelectWindow()
{
	if(AnalogControl.m_bGrabberInit == FALSE) return;

	m_pImgViewWnd = (CWnd*)(CStatic*)GetDlgItem(IDC_STATIC_IMAGE_VIEW);
	
	MdispSelectWindow(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage, m_pImgViewWnd->GetSafeHwnd());
	MdispControl(AnalogControl.m_MilDisplay, M_FILL_DISPLAY , M_ENABLE);

}
void CImageTotalDlg::VideoDispDeselectWindow()
{
	if(AnalogControl.m_bGrabberInit == FALSE) return;

	MdispDeselect(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage);

	m_pImgViewWnd = NULL;

}

BEGIN_EVENTSINK_MAP(CImageTotalDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CImageTotalDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CImageTotalDlg::OnRadioWaveoutL() 
{
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	//if(m_nRadioAudioOutL == 0)
	//{
	//	if(!m_ctrlSoundCard.wOutL_Flag) m_ctrlSoundCard.WaveOutL_Start();	
	//}
	//else // m_nRadioAudioMeasureStart == AUDIO_MEASURE_STOP : 
	//{
	//	if(m_ctrlSoundCard.wOutL_Flag) m_ctrlSoundCard.WaveOutL_Stop();
	//}
	
}

void CImageTotalDlg::OnRadioWaveoutR() 
{
	UpdateData(TRUE);

	//// TODO: Add your control notification handler code here
	//if(m_nRadioAudioOutR == 0)
	//{
	//	if(!m_ctrlSoundCard.wOutR_Flag) m_ctrlSoundCard.WaveOutR_Start();	
	//}
	//else // m_nRadioAudioMeasureStart == AUDIO_MEASURE_STOP : 
	//{
	//	if(m_ctrlSoundCard.wOutR_Flag) m_ctrlSoundCard.WaveOutR_Stop();
	//}
	//	
}
