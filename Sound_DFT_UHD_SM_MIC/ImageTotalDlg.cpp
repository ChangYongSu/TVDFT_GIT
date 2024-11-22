// ImageTotalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ImageTotalDlg.h"
#include "AvSwitchBoxCtrl.h"
#include "FFT_SM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//+2007.3.20 Add BY USY
#include "Global.h"

#include "PLC_Ctrl.h"


extern CDATsysView*  g_pView;
//-
/////////////////////////////////////////////////////////////////////////////
// CImageTotalDlg dialog

CImageTotalDlg::CImageTotalDlg(CWnd* pParent /*=NULL*/)

	: CDialog(CImageTotalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageTotalDlg)
	m_nRadioAudioMeasureStart = 0;
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
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHK_AUTO_ROBOT_PAGE, m_ctrlAutoRobotPageChk);
	DDX_Control(pDX, IDC_COMBO_MIC_TYPE, m_cComboMIC_Type);
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
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_CMB_SEL_IMAGE, OnSelchangeCmbSelImage)
	ON_BN_CLICKED(IDC_RADIO_MEASURE_START, OnRadioMeasure)
	ON_BN_CLICKED(IDC_RADIO_MEASURE_STOP, OnRadioMeasure)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_UPDATE_IMAGE_VIEW_DISPLAY, UpdateImageViewDisplay)
	ON_MESSAGE(UM_CALC_MOVE_SUB_WINDOW, CalcAndMoveSubWindow)
	ON_MESSAGE(UM_CHANGE_ACTIVE_VIEW_TAB, ChangeActiveViewTab)
	ON_MESSAGE(UM_UPDATE_ALL_VIEW_TAB, UpdateAllViewTab)
	ON_BN_CLICKED(IDC_CHK_AUTO_ROBOT_PAGE, &CImageTotalDlg::OnBnClickedChkAutoRobotPage)
	//ON_BN_CLICKED(IDOK, &CImageTotalDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_MIC_TYPE, &CImageTotalDlg::OnCbnSelchangeComboMicType)
	ON_MESSAGE(WM_GRAPH_UPDATE, OnUpdateSound)
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
			m_ctrlAutoRobotPageChk.SetCheck(FALSE);
			break;
		case P_CROP:
			m_ctrlGrabPageChk.SetCheck(FALSE);
			m_ctrlCropPageChk.SetCheck(TRUE);
			m_ctrlReviewPageChk.SetCheck(FALSE);
			m_ctrlParmPageChk.SetCheck(FALSE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			m_ctrlAutoRobotPageChk.SetCheck(FALSE);
			break;
		case P_REVIEW:
			m_ctrlGrabPageChk.SetCheck(FALSE);
			m_ctrlCropPageChk.SetCheck(FALSE);
			m_ctrlReviewPageChk.SetCheck(TRUE);
			m_ctrlParmPageChk.SetCheck(FALSE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			m_ctrlAutoRobotPageChk.SetCheck(FALSE);
			break;
		case P_PARM:
			m_ctrlGrabPageChk.SetCheck(FALSE);
			m_ctrlCropPageChk.SetCheck(FALSE);
			m_ctrlReviewPageChk.SetCheck(FALSE);
			m_ctrlParmPageChk.SetCheck(TRUE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			m_ctrlAutoRobotPageChk.SetCheck(FALSE);
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
		case P_AUTO_ROBOT:
			m_ctrlGrabPageChk.SetCheck(FALSE);
			m_ctrlCropPageChk.SetCheck(FALSE);
			m_ctrlReviewPageChk.SetCheck(FALSE);
			m_ctrlParmPageChk.SetCheck(FALSE);
			m_ctrlAveragePageChk.SetCheck(FALSE);
			m_ctrlAutoRobotPageChk.SetCheck(TRUE);
			break;
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
			m_ParmAutoRobotPage.ShowWindow(SW_HIDE);
			//+ 2007.10.16 Add BY USY
			GetDlgItem(IDC_STATIC_PAGE)->ShowWindow(SW_HIDE);
			//-
			SetImgPanel(SW_HIDE);
			SetPageItem(nTabType, nImgType);
		}	
		break;
		case P_REVIEW:
		{	
			m_GrabPage.ShowWindow(SW_HIDE);
			m_ReviewPage.ShowWindow(SW_SHOW);
			m_ParmPage.ShowWindow(SW_HIDE);
			m_ParmAutoRobotPage.ShowWindow(SW_HIDE);
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
			m_GrabPage.ShowWindow(SW_HIDE);
			m_ReviewPage.ShowWindow(SW_HIDE);
			m_ParmPage.ShowWindow(SW_SHOW);
			m_ParmAutoRobotPage.ShowWindow(SW_HIDE);
			//+ 2007.10.16 Add BY USY
			GetDlgItem(IDC_STATIC_PAGE)->ShowWindow(SW_SHOW);
			//-
			SetImgPanel(SW_SHOW);
			SetImgChkBtn(nTabType, nImgType);
			SetPageItem(nTabType, nImgType);
		}
		break;

		case P_AUTO_ROBOT:
		{
			m_GrabPage.ShowWindow(SW_HIDE);
			m_ReviewPage.ShowWindow(SW_HIDE);
			m_ParmPage.ShowWindow(SW_HIDE);
			m_ParmAutoRobotPage.ShowWindow(SW_SHOW);
			
			GetDlgItem(IDC_STATIC_PAGE)->ShowWindow(SW_HIDE);			
			SetImgPanel(SW_HIDE);
		//	SetImgChkBtn(nTabType, nImgType);
		//	SetPageItem(nTabType, nImgType);
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

	case P_AUTO_ROBOT:
		//m_Image[0] = &m_GrabPage.m_Image1;
		//m_GrabPage.m_ctrlProgress = &m_ctrlProgress;
		//m_ZImgInfo = NULL;
		//m_GrabPage.m_MWnd = this;
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

	m_pImgViewWnd = GetDlgItem(IDC_STATIC_IMAGE_VIEW);
	m_pPageWnd    = GetDlgItem(IDC_STATIC_PAGE);

	m_GrabPage.Create(IDD_GRAB_PAGE, this);
	m_ReviewPage.Create(IDD_REVIEW_PAGE, this);
	m_ParmPage.Create(IDD_PARM_ADJ_PAGE, this);
	m_ParmAutoRobotPage.Create(IDD_PARM_AUTO_ROBOT_PAGE, this);

	//IDD_PARM_AUTO_ROBOT_PAGE

	//+add kwmoon 071106
	CString strMsg;
	if(CurrentSet->sRemoconTitle.IsEmpty())
	{
		strMsg.LoadString(IDS_ERR_REMOTE);
		AfxMessageBox(IDS_ERR_REMOTE);
	}
	else
	{
		OpenRemoteFile(CurrentSet->sRemoconTitle);
	}

	//+add kwmoon 071214
	if(CurrentSet->sPatternTitle.IsEmpty())
	{
		strMsg.LoadString(IDS_ERR_PATTERN);
		AfxMessageBox(strMsg);
	}
	else
	{
		OpenPatternFile(CurrentSet->sPatternTitle);
	}

	
	//+recover kwmoon 080724
	InitAudioMeasurement();

	UpdateData(FALSE);
	PostMessage(UM_CALC_MOVE_SUB_WINDOW);

	ShowWindow(SW_SHOW);

	//m_cComboMIC_Type.

	SetTimer(1,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//+add kwmoon 080724
void CImageTotalDlg::InitAudioMeasurement() 
{
//	m_ctrlSoundCard.Creat(this);
//	m_ctrlSoundCard.m_nWinFlag = 1;

	////////////////////////////////
//OPEN GL

// Get a handle to the Picture Control
	CWnd* pWnd1 = GetDlgItem(IDC_STATIC_IMAGE_OGL_DISPLAY1);
	m_hDC1 = ::GetDC(pWnd1->GetSafeHwnd());

	// Set the pixel format for the DC
	PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0,
		PFD_MAIN_PLANE, 0, 0, 0, 0 };

	int format1 = ChoosePixelFormat(m_hDC1, &pfd);
	SetPixelFormat(m_hDC1, format1, &pfd);

	// Create the OpenGL rendering context
	m_hGLRC1 = wglCreateContext(m_hDC1);
	wglMakeCurrent(m_hDC1, m_hGLRC1);
	///////////////////////////////////////////////

		// Get a handle to the Picture Control
	CWnd* pWnd2 = GetDlgItem(IDC_STATIC_IMAGE_OGL_DISPLAY2);
	m_hDC2 = ::GetDC(pWnd2->GetSafeHwnd());

	// Set the pixel format for the DC
	pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1,
	   PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
	   PFD_TYPE_RGBA, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0,
	   PFD_MAIN_PLANE, 0, 0, 0, 0 };

	int format2 = ChoosePixelFormat(m_hDC2, &pfd);
	SetPixelFormat(m_hDC2, format2, &pfd);

	// Create the OpenGL rendering context
	m_hGLRC2 = wglCreateContext(m_hDC2);
	wglMakeCurrent(m_hDC2, m_hGLRC2);
	///////////////////////////////////////////
	m_cLeftFreqDisplay.SubclassDlgItem(IDC_LEFT_IMAGE_FREQ_DISPLAY, this);
	m_cLeftFreqDisplay.SetWindowText(_T("0000Hz"));//IDC_ROBOT_STATUS
	m_cLeftFreqDisplay.SetBkColor(RGB(0, 0, 0));
	m_cLeftFreqDisplay.SetTextColor(RGB(0, 255, 0));

	m_cLeftLevelDisplay.SubclassDlgItem(IDC_LEFT_IMAGE_LEVEL_DISPLAY, this);
	m_cLeftLevelDisplay.SetWindowText(_T("0000mV"));//IDC_ROBOT_STATUS
	m_cLeftLevelDisplay.SetBkColor(RGB(0, 0, 0));
	m_cLeftLevelDisplay.SetTextColor(RGB(0, 255, 0));

	m_cRightFreqDisplay.SubclassDlgItem(IDC_RIGHT_IMAGE_FREQ_DISPLAY, this);
	m_cRightFreqDisplay.SetWindowText(_T("0000Hz"));//IDC_ROBOT_STATUS
	m_cRightFreqDisplay.SetBkColor(RGB(0, 0, 0));
	m_cRightFreqDisplay.SetTextColor(RGB(0, 255, 0));

	m_cRightLevelDisplay.SubclassDlgItem(IDC_RIGHT_IMAGE_LEVEL_DISPLAY, this);
	m_cRightLevelDisplay.SetWindowText(_T("0000mV"));//IDC_ROBOT_STATUS
	m_cRightLevelDisplay.SetBkColor(RGB(0, 0, 0));
	m_cRightLevelDisplay.SetTextColor(RGB(0, 255, 0));

	m_cComboMIC_Type.ResetContent();
	int numDevice = g_SoundCard.strDeviceNameArry.GetSize();
	CString strDeviceName;
	int l_DeviceNumSel = 0;
	for (int i = 0; i < numDevice; i++)
	{
		strDeviceName = g_SoundCard.strDeviceNameArry.GetAt(i);
		m_cComboMIC_Type.AddString(strDeviceName);
		strDeviceName.MakeUpper();

		if ((strDeviceName.Find(CurrentSet->sMainMIC_Name) >= 0) && (CurrentSet->sMainMIC_Name.GetLength() > 10))
		{
			l_DeviceNumSel = i;
		}

	}

	m_cComboMIC_Type.SetCurSel(l_DeviceNumSel);// g_SoundCard.m_nDeviceID);
	OnCbnSelchangeComboMicType();
	//if (g_nRunningProcessNo == 1) {
	//	//+add PSH 080527
	//	if (numDevice >= 1) {
	//		m_cComboMIC_Type.SetCurSel(0);
	//	}
	//	
	//}
	//else if (g_nRunningProcessNo == 2) {
	//	if (numDevice >= 2) {
	//		m_cComboMIC_Type.SetCurSel(1);
	//	}
	//}
	//else {
	//	if (numDevice >= 3) {
	//		m_cComboMIC_Type.SetCurSel(2);
	//	}
	//}


	
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
	wglMakeCurrent(m_hDC1, m_hGLRC1);
	m_oglWindow.DrawLeftScene();
	//// Clear the screen
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Set up the scene (replace this with any OpenGL drawing code)
	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex2f(-0.5f, -0.5f);
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex2f(0.5f, -0.5f);
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex2f(0.0f, 0.5f);
	//glEnd();

	// Swap the buffers to display the frame
	SwapBuffers(m_hDC1);



	wglMakeCurrent(m_hDC2, m_hGLRC2);

	m_oglWindow.DrawRightScene();
	//// Clear the screen
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Set up the scene (replace this with any OpenGL drawing code)
	//  // Draw a simple rectangle
	//glBegin(GL_QUADS);
	//glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
	//glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
	//glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
	//glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
	//glEnd();

	// Swap the buffers to display the frame
	SwapBuffers(m_hDC2);



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


LRESULT CImageTotalDlg::OnUpdateSound(WPARAM wParam, LPARAM lParam)
{
	static int sWaitCount = 0;
	//static int s_InputEnableCount = 0;
	CFFT_SM lfft;
	//	COMPLEX XData[m_Sample_Buffer_Max];
	COMPLEX XData[10000];

	//CNiReal64Matrix Check_data;
	//CNiReal64Matrix Check_data_1;

	int len;
	char lbBuffer[100000];
	short lSBuffer[100000];
	///	double lDbuffer[m_Sample_Buffer_Max];
	double lDbuffer[10000];
	double lDbufferR[10000];
	double lDbufferL[10000];

	double lDbufferDisplayR[2000];
	double lDbufferDisplayL[2000];

	//double lmax = 0, lmin = 1000;
	CString szString;
	CString sTemp;
	len = (int)wParam;


	if (sWaitCount > 0)
		sWaitCount--;

	if (m_InputEnableCount > 10)
	{
		if (m_SignalEnable == 0)
		{
			m_SignalEnable = 1;
		}
	}
	else
	{
		m_InputEnableCount++;
	}

	//	int m_Sample_Byte_Max;
	//int m_Sample_Buffer_Max;
	//int m_Sample_Rate_Per_Second;
	if (len != SAMPLE_BUFFER_MAX * 4)
	{
		len = SAMPLE_BUFFER_MAX * 4;
		return 1;
	}
	memcpy(lbBuffer, (char *)lParam, len);
	lbBuffer[len] = 0;
	memcpy(lSBuffer, lbBuffer, len);
	//5/65535
	//Check_data.SetSize(1, len / 2);
	for (int i = 0; i < SAMPLE_BUFFER_MAX; i++)
	{
		lDbuffer[i] = (double)lSBuffer[i * 2] / 3277.;
		//lDbuffer[i] = lDbuffer[i] / 3277.;//65536;
		lDbufferL[i] = (double)lSBuffer[i * 2] / 3277.;
		lDbufferR[i] = (double)lSBuffer[i * 2 + 1] / 3277.;

	}

	double lFreq_Left = 0;
	double lVolume_Left = 0;
	double lFreq_Right = 0;
	double lVolume_Right = 0;

	static double sFreq_Left = 0;
	static double sVolume_Left = 0;
	static double sFreq_Right = 0;
	static double sVolume_Right = 0;
	static int sDisplayCount = 0;

	double lFreq_Left_Sum = 0;
	double lFreq_Right_Sum = 0;
	double lFreq_Left_Avg = 0;
	double lFreq_Right_Avg = 0;


	static double sFreq_Left_Array[30];
	static double sFreq_Right_Array[30];

	double lAvgDbufferR[10000];
	double lAvgDbufferL[10000];
	double lSumDbufferR = 0;
	double lSumDbufferL = 0;

#if 0

	for (int i = 0; i < 2048; i++)
	{

		lAvgDbufferR[i] = lDbufferR[i];
		lAvgDbufferL[i] = lDbufferL[i];

	}
#else

	for (int i = 0; i < 9; i++)
	{
		lSumDbufferR += lDbufferR[i];
		lSumDbufferL += lDbufferL[i];
	}

	for (int i = 0; i < 2038; i++)
	{
		lSumDbufferR += lDbufferR[i + 10];
		lSumDbufferL += lDbufferL[i + 10];
		lAvgDbufferR[i] = lSumDbufferR / 10.0;
		lAvgDbufferL[i] = lSumDbufferL / 10.0;

		lSumDbufferR -= lDbufferR[i];
		lSumDbufferL -= lDbufferL[i];
	}

#endif
#if 1

	lfft.CheckData(lAvgDbufferR, lDbufferDisplayR, &lFreq_Right, &lVolume_Right);
	lfft.CheckData(lAvgDbufferL, lDbufferDisplayL, &lFreq_Left, &lVolume_Left);




#else
	//lfft.CheckData(lDbufferR, lDbufferDisplayR, &lFreq_Right, &lVolume_Right);
	//lfft.CheckData(lDbufferL, lDbufferDisplayL, &lFreq_Left, &lVolume_Left);
	//
	//
	//for (int i = 0; i < 1000; i++)
	//{
	//	m_PlotDataR[i].Data = lDbufferDisplayR[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//	m_PlotDataL[i].Data = lDbufferDisplayL[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//}
#endif





	for (int i = 0; i < 9; i++)
	{
		sFreq_Left_Array[i] = sFreq_Left_Array[i + 1];
		lFreq_Left_Sum += sFreq_Left_Array[i];

	}
	sFreq_Left_Array[9] = lFreq_Left;
	lFreq_Left_Sum += sFreq_Left_Array[9];
	lFreq_Left_Avg = lFreq_Left_Sum / 10.0;


	for (int i = 0; i < 9; i++)
	{
		sFreq_Right_Array[i] = sFreq_Right_Array[i + 1];
		lFreq_Right_Sum += sFreq_Right_Array[i];

	}
	sFreq_Right_Array[9] = lFreq_Right;
	lFreq_Right_Sum += sFreq_Right_Array[9];
	lFreq_Right_Avg = lFreq_Right_Sum / 10.0;


	if (lVolume_Left < 20)
		lFreq_Left = 0;

	if (lVolume_Right < 20)
		lFreq_Right = 0;

	m_Frequency_Right = lFreq_Right;
	m_Frequency_Left = lFreq_Left;


	double sDisplayScaleRateLeft = 1;
	double sDisplayScaleRateRight = 1;
#ifdef __NEW_WAVE_DISPLAY	
	m_Voltage_Left = lVolume_Left * 2.5;
	m_Voltage_Right = lVolume_Right * 2.5;
#else
	double sSacleRate[] = {
	0.008,//0
	0.021,//1
	0.037,//2
	0.056,//3
	0.078,//4
	0.101,//5
	0.132,//6
	0.164,//7
	0.201,//8
	0.244,//9
	0.292,//10
	0.343,//11
	0.403,//12
	0.471,//13
	0.545,//14
	0.630,//15
	};
	int lRateID = int((m_Frequency_Right - 400) / 100);

	if ((m_Frequency_Right > 500) && (m_Frequency_Right < 2000) && lRateID > 0)
	{
		sDisplayScaleRateRight = (1 + sSacleRate[lRateID - 1] + (sSacleRate[lRateID] - sSacleRate[lRateID - 1])* (m_Frequency_Right - (lRateID * 100 + 400)) / 100);
		lVolume_Right = lVolume_Right * sDisplayScaleRateRight;// (1 + sSacleRate[lRateID - 1] + (sSacleRate[lRateID] - sSacleRate[lRateID - 1]) * (m_Frequency_Right - (lRateID * 100 + 400)) / 100);
	}
	else if (m_Frequency_Right >= 2000)
	{
		sDisplayScaleRateRight = (1 + sSacleRate[14] + (sSacleRate[15] - sSacleRate[14]) * (m_Frequency_Right - (14 * 100 + 400)) / 100);
		lVolume_Right = lVolume_Right * sDisplayScaleRateRight;// (1 + sSacleRate[14] + (sSacleRate[15] - sSacleRate[14]) * (m_Frequency_Right - (14 * 100 + 400)) / 100);
	}
	lRateID = int((m_Frequency_Left - 400) / 100);
	if ((m_Frequency_Left > 500) && (m_Frequency_Left < 2000) && lRateID > 0)
	{
		sDisplayScaleRateLeft = (1 + sSacleRate[lRateID - 1] + (sSacleRate[lRateID] - sSacleRate[lRateID - 1])* (m_Frequency_Left - (lRateID * 100 + 400)) / 100);
		lVolume_Left = lVolume_Left * sDisplayScaleRateLeft;//			(1 + sSacleRate[lRateID-1] +( sSacleRate[lRateID] - sSacleRate[lRateID - 1] )* (m_Frequency_Right - (lRateID * 100 + 400))/100);
	}
	else if (m_Frequency_Left >= 2000)
	{
		sDisplayScaleRateLeft = (1 + sSacleRate[14] + (sSacleRate[15] - sSacleRate[14]) * (m_Frequency_Left - (14 * 100 + 400)) / 100);
		lVolume_Left = lVolume_Left * sDisplayScaleRateLeft;//			(1 + sSacleRate[14] + (sSacleRate[15] - sSacleRate[14]) * (m_Frequency_Right - (14 * 100 + 400)) / 100);
	}

	//m_Frequency_Left = lFreq_Left;

	m_Voltage_Left = lVolume_Left;
	m_Voltage_Right = lVolume_Right;
#endif



	double lDisplayAdjustDbufferR[10000];
	double lDisplayAdjustDbufferL[10000];

	memcpy(lDisplayAdjustDbufferR, lDbufferDisplayR, sizeof(PlotData_T) * 1000);
	memcpy(lDisplayAdjustDbufferL, lDbufferDisplayL, sizeof(PlotData_T) * 1000);

	sDisplayScaleRateLeft = sDisplayScaleRateLeft / 10.0;
	sDisplayScaleRateRight = sDisplayScaleRateRight / 10.0;

	sDisplayScaleRateLeft = m_Voltage_Left < 1500 ? sDisplayScaleRateLeft * 1.5 : sDisplayScaleRateLeft * 1.5*(1500 / (m_Voltage_Left));
	sDisplayScaleRateRight = m_Voltage_Right < 1500 ? sDisplayScaleRateRight * 1.5 : sDisplayScaleRateRight * 1.5*(1500 / (m_Voltage_Right));
	sDisplayScaleRateLeft = sDisplayScaleRateLeft < 0.09 ? 0.09 : sDisplayScaleRateLeft;
	sDisplayScaleRateRight = sDisplayScaleRateRight < 0.09 ? 0.09 : sDisplayScaleRateRight;


	for (int i = 0; i < 1000; i++)
	{
		m_PlotDataR[i].Data = lDisplayAdjustDbufferR[i] * sDisplayScaleRateRight;// / 10; //(0.9 + 0.5 / lDbufferDisplayR[i] * 9) * lDbufferDisplayR[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
		m_PlotDataL[i].Data = lDisplayAdjustDbufferL[i] * sDisplayScaleRateLeft;// / 10; //(0.9 + 0.5 / lDbufferDisplayL[i] * 9) * lDbufferDisplayL[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	}

	if (1)// m_Sound_Display_Enable == 1)
	{
		//	memcpy(m_poglWindow->m_PlotDataL, m_PlotDataL, sizeof(PlotData_T) * 1000);
		//	memcpy(m_poglWindow->m_PlotDataR, m_PlotDataR, sizeof(PlotData_T) * 1000);
		memcpy(m_oglWindow.m_PlotDataL, m_PlotDataL, sizeof(PlotData_T) * 1000);
		memcpy(m_oglWindow.m_PlotDataR, m_PlotDataR, sizeof(PlotData_T) * 1000);

	}

	static int sAudioMeasureCount = 0;
	//if (m_RunAudioMeasure == 1)
	//{
	//	sAudioMeasureCount++;
	//	if (sAudioMeasureCount < 3)
	//	{
	//		if (AudioMeasure() == 1)
	//			m_RunAudioMeasure = 0;
	//	}
	//	else
	//	{

	//		m_RunAudioMeasure = 0;
	//	}
	//}
	//else
	//{
	//	sAudioMeasureCount = 0;
	//}


	if (1) // m_Sound_Display_Enable == 1)
	{
		sDisplayCount++;
		if (sDisplayCount > 10)
		{
			sDisplayCount = 0;

			if (sFreq_Left != m_Frequency_Left)
			{
				sFreq_Left = m_Frequency_Left;
				sTemp.Format(_T(" %4.0f Hz "), m_Frequency_Left);
				m_cLeftFreqDisplay.SetWindowText(sTemp);
			}

			if (sVolume_Left != m_Voltage_Left)
			{
				sVolume_Left = m_Voltage_Left;
				sTemp.Format(_T(" %4.0f mV "), m_Voltage_Left);
				m_cLeftLevelDisplay.SetWindowText(sTemp);

			}

			if (sFreq_Right != m_Frequency_Right)
			{
				sFreq_Right = m_Frequency_Right;

				sTemp.Format(_T(" %4.0f Hz "), m_Frequency_Right);
				m_cRightFreqDisplay.SetWindowText(sTemp);
			}

			if (sVolume_Right != m_Voltage_Right)
			{
				sVolume_Right = m_Voltage_Right;

				sTemp.Format(_T(" %4.0f mV "), m_Voltage_Right);
				m_cRightLevelDisplay.SetWindowText(sTemp);

			}
		}
	}
	else
	{
		if (sFreq_Left != 0)
		{
			sFreq_Left = 0;
			sTemp.Format(_T(" %4.0f Hz "), sFreq_Left);
			m_cLeftFreqDisplay.SetWindowText(sTemp);
		}

		if (sVolume_Left != 0)
		{
			sVolume_Left = 0;
			sTemp.Format(_T(" %4.0f mV "), sVolume_Left);
			m_cLeftLevelDisplay.SetWindowText(sTemp);
		}

		if (sFreq_Right != 0)
		{
			sFreq_Right = 0;
			sTemp.Format(_T(" %4.0f Hz "), sFreq_Right);
			m_cRightFreqDisplay.SetWindowText(sTemp);
		}

		if (sVolume_Right != 0)
		{
			sVolume_Right = 0;
			sTemp.Format(_T(" %4.0f mV "), sVolume_Right);
			m_cRightLevelDisplay.SetWindowText(sTemp);
		}
	}



	//	lmax = ((int)(lmax * 100)) / 100.0 + 0.1;
		//m_GraphMainWave.Axes.Item("YAxis-1").Maximum = lmax;
		//m_GraphMainWave.Axes.Item("YAxis-1").Minimum = -lmax;

		//m_GraphMainWave.Plots.RemoveAll();


		//for (unsigned int i = 0; i < 8; i++)
		//{
		//	m_GraphMainWave.Plots.Add();
		//	m_GraphMainWave.Plots.Item(i + 1).LineColor = m_colors[0];
		//}

		//m_GraphMainWave.PlotY(Check_data);



	double lFreq = 0;
	double lVolume = 0;
	int lReturn = lfft.CheckFreqVol(lDbuffer, &lFreq, &lVolume);

	CString sTmp;
	sTmp.Format(_T("Freq: %4.2f, Vol: %4.2f  "), lFreq, lVolume);
	//	SetDlgItemText(IDC_STATIC_FREQ_VOL, sTmp);


	////////////////////////////////////
	////////FFT

	for (int i = 0; i < SAMPLE_BUFFER_MAX; i++)
	{
		XData[i].real = lDbuffer[i];
		//Check_data(0,i) = lDbuffer[i];
	}
	int m = lfft.log2(SAMPLE_BUFFER_MAX);
	int fft_length;
	fft_length = 1 << m;   //bit shift
	lfft.fft(XData, m);
	//////////////////////////////////////////////////
	double SpectrumMag[10000];;
	double tempflt;
	double tempMax = 0;
	int MaxPt = 0;

	double m_Lodata[10000];

	//double m_Lodata[SAMPLE_BUFFER_MAX];
	//SpectrumMag = (double *)calloc(fft_length, sizeof(double));
//	Check_data.SetSize(1, fft_length / 2 - 1);
	//double lDbuffer[10000];
	for (int i = 1; i < fft_length / 2; i++)
	{
		tempflt = XData[i].real * XData[i].real;
		tempflt += XData[i].imag * XData[i].imag;
		tempflt = sqrt(tempflt);
		tempflt = MAX(tempflt, 1.e-14);
		if (tempMax < tempflt)
		{
			MaxPt = i;
		}
		tempMax = MAX(tempflt, tempMax);
		m_Lodata[i] = tempflt;
		SpectrumMag[i] = 10 * log10(tempflt);

	}




	const int cAvgCount = 20;
	double lSum = 0;
	double lAvgDbuffer[4096];
	for (int i = 1; i < cAvgCount; i++)
	{
		lSum += SpectrumMag[i];
	}

	for (int i = 1; i < fft_length / 2; i++)
	{
		if (i < fft_length / 2 - cAvgCount)
		{
			lSum += SpectrumMag[i + cAvgCount];
			lAvgDbuffer[i - 1] = lSum / cAvgCount;
		}
		else
		{
			lSum += SpectrumMag[fft_length / 2 - 1];
			lAvgDbuffer[i - 1] = lSum / cAvgCount;
		}
		lSum -= SpectrumMag[i];
	}



	///////////////////////////////////////////////////////
	//
	//lAvgDbuffer[0] = Check_data(0, 0);
	//for (int i = 1; i < fft_length / 2; i++)
	//{
	//	lAvgDbuffer[i] = Check_data(0, i - 1);
	//}

	CString strTmp;
	int lRtn;
#if 1
	//for (int i = 0; i < 500; i++)
	//{
	//	//	m_oglWindow.m_PlotData[i].Data = m_Lodata[i] / (1.e+6 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//	m_oglWindow.m_PlotData[i].Data = lDbuffer[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//	m_oglWindow.m_PlotData[i].bNG = lfft.m_CheckError[i];
	//	//m_dataBuffer[i] = Check_data(0, i);
	//}
#else
	//	if (m_LogDisplayOn != 1)
	{

		for (int i = 0; i < fft_length / 2 - 1; i++)
		{
			//	m_oglWindow.m_PlotData[i].Data = m_Lodata[i] / (1.e+6 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
			m_oglWindow.m_PlotData[i].Data = lDbuffer[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
			m_oglWindow.m_PlotData[i].bNG = lfft.m_CheckError[i];
			//m_dataBuffer[i] = Check_data(0, i);
		}
	}
	//else
	//{
	//	for (int i = 0; i < fft_length / 2 - 1; i++)
	//	{
	//		m_oglWindow.m_PlotData[i].Data = (Check_data(0, i) - 50.0) / 50.0;
	//		m_oglWindow.m_PlotData[i].bNG = lfft.m_CheckError[i];
	//		//m_dataBuffer[i] = Check_data(0, i);
	//	}
	//}

#endif

	return 1;
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

void CImageTotalDlg::OnBnClickedChkAutoRobotPage()
{

	// TODO: ¿©±â¿¡ ÄÁÆ®·Ñ ¾Ë¸² Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	m_nOldImgType[P_GRAB] = m_nImgType;
	m_nTabType = P_AUTO_ROBOT;


	SetChkBtn(m_nTabType, m_nOldImgType[P_GRAB]);
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
	// TODO: Add extra validation here
	int cnt = 0;

	if (g_SoundCard.wIn_Flag)
	{
		g_SoundCard.WaveRead_Stop();
	}

	if (m_GrabPage.m_bThreadRunning)
	{
		m_GrabPage.GrabStop();
		while (m_GrabPage.m_bThreadRunning)
		{
			Sleep(100);
			cnt++;
			if (cnt > 100)
			{
				MessageBox("Grab Not Close");

			}
		}
	}

	gPLC_Ctrl.CommandClear();

	m_GrabPage.SendMessage(WM_CLOSE);		//Grab Page
	Sleep(100);
	m_ReviewPage.SendMessage(WM_CLOSE);		//Review Page
	Sleep(100);
	m_ParmPage.SendMessage(WM_CLOSE);		//Parm Adj Page
	Sleep(100);

	CDialog::OnOK();
}

void CImageTotalDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	int cnt = 0;
	if (m_GrabPage.m_bThreadRunning)
	{
		m_GrabPage.GrabStop();
		while (m_GrabPage.m_bThreadRunning)
		{
			Sleep(100);
			cnt++;
			if (cnt > 100)
			{
				MessageBox("Grab Not Close");

			}
		}
	}

	m_GrabPage.SendMessage(WM_CLOSE);		//Grab Page
	m_ReviewPage.SendMessage(WM_CLOSE);		//Review Page
	m_ParmPage.SendMessage(WM_CLOSE);		//Parm Adj Page

	

	
	//+recover kwmoon 080724
	if(g_SoundCard.wIn_Flag)
	{
		g_SoundCard.WaveRead_Stop();
	}

	CDialog::OnClose();
}

//+recover kwmoon 080724
#define TIMER_CHECK_AUDIO_IN	100

//+recover kwmoon 080724
void CImageTotalDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	double nLevel_L = 0;
	double nLevel_R = 0;

	double freq = 0, amp = 0, dbamp = 0;
	double nCorrectionValue = 0.97;
	static int sEnableCount = 0;

	static double sFreq_Left = 0;
	static double sVolume_Left = 0;
	static double sFreq_Right = 0;
	static double sVolume_Right = 0;
	static int sDisplayCount = 0;
	CString sTemp;

	switch(nIDEvent)
	{
		case TIMER_CHECK_AUDIO_IN :
		{
			if(g_SoundCard.wIn_Flag)
			{
				// LEFT
				//m_ctrlSoundCard.GetWavePeak_L(&freq, &amp, &dbamp);
				//m_ctrlSoundCard.GetPeackDetectorPeak_L(&nLevel_L);
				//m_ctrlSoundCard.SetLedValue_inL((long)freq);
				//m_ctrlSoundCard.SetLedValue_inL_Level((long)(nLevel_L/(2*1.4142)*0.1*nCorrectionValue));

				//// RIGHT
				//m_ctrlSoundCard.GetWavePeak_R(&freq, &amp, &dbamp);
				//m_ctrlSoundCard.GetPeackDetectorPeak_R(&nLevel_R);
				//m_ctrlSoundCard.SetLedValue_inR((long)freq);
				//m_ctrlSoundCard.SetLedValue_inR_Level((long)(nLevel_R/(2*1.4142)*0.1*nCorrectionValue));
			}
			
			if (sEnableCount != g_pView->m_InputEnableCount)
			{
				sEnableCount != g_pView->m_InputEnableCount;

				//for (int i = 0; i < 1000; i++)
				//{
				//	m_PlotDataR[i].Data = lDisplayAdjustDbufferR[i] * sDisplayScaleRateRight;// / 10; //(0.9 + 0.5 / lDbufferDisplayR[i] * 9) * lDbufferDisplayR[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
				//	m_PlotDataL[i].Data = lDisplayAdjustDbufferL[i] * sDisplayScaleRateLeft;// / 10; //(0.9 + 0.5 / lDbufferDisplayL[i] * 9) * lDbufferDisplayL[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
				//}

				if (1)// m_Sound_Display_Enable == 1)
				{
					//	memcpy(m_poglWindow->m_PlotDataL, m_PlotDataL, sizeof(PlotData_T) * 1000);
					//	memcpy(m_poglWindow->m_PlotDataR, m_PlotDataR, sizeof(PlotData_T) * 1000);
					memcpy(m_oglWindow.m_PlotDataL, g_pView->m_PlotDataL, sizeof(PlotData_T) * 1000);
					memcpy(m_oglWindow.m_PlotDataR, g_pView->m_PlotDataR, sizeof(PlotData_T) * 1000);

				}

				static int sAudioMeasureCount = 0;
				


				//if (1) // m_Sound_Display_Enable == 1)
				//{
				sDisplayCount++;
				if (sDisplayCount > 10)
				{
					sDisplayCount = 0;

					if (sFreq_Left != g_pView->m_Frequency_Left)
					{
						sFreq_Left = g_pView->m_Frequency_Left;
						sTemp.Format(_T(" %4.0f Hz "), g_pView->m_Frequency_Left);
						m_cLeftFreqDisplay.SetWindowText(sTemp);
					}

					if (sVolume_Left != g_pView->m_Voltage_Left)
					{
						sVolume_Left = g_pView->m_Voltage_Left;
						sTemp.Format(_T(" %4.0f mV "), g_pView->m_Voltage_Left);
						m_cLeftLevelDisplay.SetWindowText(sTemp);

					}

					if (sFreq_Right != g_pView->m_Frequency_Right)
					{
						sFreq_Right = g_pView->m_Frequency_Right;

						sTemp.Format(_T(" %4.0f Hz "), g_pView->m_Frequency_Right);
						m_cRightFreqDisplay.SetWindowText(sTemp);
					}

					if (sVolume_Right != g_pView->m_Voltage_Right)
					{
						sVolume_Right = g_pView->m_Voltage_Right;

						sTemp.Format(_T(" %4.0f mV "), g_pView->m_Voltage_Right);
						m_cRightLevelDisplay.SetWindowText(sTemp);

					}
				}
			}
			break;
		}
		case 1:
		{
			KillTimer(nIDEvent);

			m_nRadioAudioMeasureStart = AUDIO_MEASURE_START;
		//	UpdateData();
			OnRadioMeasure();
			break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// +recover kwmoon 080724
void CImageTotalDlg::OnRadioMeasure() 
{
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	if(m_nRadioAudioMeasureStart == AUDIO_MEASURE_START)
	{
		//+ 2007.10.16 Mod BY USY
	//	AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_ON_OFF,1,0); // ON
//		AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_ON,1,0); // ON
		//-

	//	WaveRead_StartUSB
		//if(!m_ctrlSoundCard.wIn_Flag) m_ctrlSoundCard.WaveRead_Start();	

		int lID = m_cComboMIC_Type.GetCurSel();
		m_cComboMIC_Type.ResetContent();
		int numDevice = g_SoundCard.strDeviceNameArry.GetSize();
		for (int i = 0; i < numDevice; i++)
		{
			m_cComboMIC_Type.AddString(g_SoundCard.strDeviceNameArry.GetAt(i));
		}

		m_cComboMIC_Type.SetCurSel(g_SoundCard.m_nDeviceID);
		SetTimer(TIMER_CHECK_AUDIO_IN,100,NULL);
	}
	else // m_nRadioAudioMeasureStart == AUDIO_MEASURE_STOP : 
	{
		//+ 2007.10.16 Mod BY USY
	//	AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_ON_OFF,0,0); // OFF
//		AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_OFF,0,0); // OFF
		//-
		int lID = m_cComboMIC_Type.GetCurSel();
		m_cComboMIC_Type.ResetContent();
		int numDevice = g_SoundCard.strDeviceNameArry.GetSize();
		for (int i = 0; i < numDevice; i++)
		{
			m_cComboMIC_Type.AddString(g_SoundCard.strDeviceNameArry.GetAt(i));
		}
		
		m_cComboMIC_Type.SetCurSel(lID);

		if(g_SoundCard.wIn_Flag) g_SoundCard.WaveRead_Stop();

		KillTimer(TIMER_CHECK_AUDIO_IN);
	}
}

//void CImageTotalDlg::OnRadioMeasure()
//{
//	UpdateData(TRUE);
//
//	// TODO: Add your control notification handler code here
//	if (m_nRadioAudioMeasureStart == AUDIO_MEASURE_START)
//	{
//		//+ 2007.10.16 Mod BY USY
//	//	AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_ON_OFF,1,0); // ON
////		AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_ON,1,0); // ON
//		//-
//
//
//		if (!m_ctrlSoundCard.wIn_Flag) m_ctrlSoundCard.WaveRead_Start();
//
//		SetTimer(TIMER_CHECK_AUDIO_IN, 500, NULL);
//	}
//	else // m_nRadioAudioMeasureStart == AUDIO_MEASURE_STOP : 
//	{
//		//+ 2007.10.16 Mod BY USY
//	//	AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_ON_OFF,0,0); // OFF
////		AvSwitchBoxCtrl.SendRun((BYTE)AUDIO_SIGNAL_OFF,0,0); // OFF
//		//-
//
//		if (m_ctrlSoundCard.wIn_Flag) m_ctrlSoundCard.WaveRead_Stop();
//
//		KillTimer(TIMER_CHECK_AUDIO_IN);
//	}
//}
//


void CImageTotalDlg::OnChkAveragePage() 
{
	m_nOldImgType[m_nTabType] = m_nImgType;
	m_nTabType = P_AVERAGE;
	
	SetChkBtn(m_nTabType, m_nOldImgType[m_nTabType]);
	Invalidate(TRUE);
}

LRESULT  CImageTotalDlg::UpdateImageViewDisplay(WPARAM wParam, LPARAM lParam)
{
	//+change kwmoon 080709
//	InvalidateRect(&m_ImgViewRect,FALSE);
	InvalidateRect(&m_ImgViewRect,TRUE);
	//+add kwmoon 080628
	UpdateWindow();

	return 0;
}

LRESULT  CImageTotalDlg::UpdateAllViewTab(WPARAM wParam, LPARAM lParam)
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
int CImageTotalDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DoModal();
}

LRESULT  CImageTotalDlg::CalcAndMoveSubWindow(WPARAM wParam, LPARAM lParam) 
{
	m_pImgViewWnd->GetWindowRect(&m_ImgViewRect);
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
	m_GrabPage.SetWindowPos(&wndTop,	m_PageRect.left, m_PageRect.top - 50,  m_PageRect.Width() + 200  , m_nPage2Height + 20 ,	SWP_HIDEWINDOW);
	m_ReviewPage.SetWindowPos(&wndTop,	m_PageRect.left, m_PageRect.top,  m_PageRect.Width(), m_PageRect.Height(),	SWP_HIDEWINDOW);
	m_ParmPage.SetWindowPos(&wndTop, m_PageRect.left, m_PageRect.top, m_PageRect.Width(), m_PageRect.Height(), SWP_HIDEWINDOW);
	m_ParmAutoRobotPage.SetWindowPos(&wndTop, m_PageRect.left, m_PageRect.top, m_PageRect.Width(), m_PageRect.Height(), SWP_HIDEWINDOW);

	InitImgPanel();

	SetPage(m_nTabType, m_nImgType);
	
	CString sTmp; 
	sTmp.Format("Image Total Dialog  - %s", CurrentSet->sModelFolder);
	SetWindowText(sTmp);
	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);

	OnChkGrabPage(); 

	//+add kwmoon 080419
	m_GrabPage.ChkLVDSAutoGrab();

	return 0;
}


LRESULT  CImageTotalDlg::ChangeActiveViewTab(WPARAM wParam, LPARAM lParam) 
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






void CImageTotalDlg::OnCbnSelchangeComboMicType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_cComboMIC_Type.ResetContent();
	//int numDevice = m_ctrlSoundCard.strDeviceNameArry.GetSize();
	//for (int i = 0; i < numDevice; i++)
	//{
	//	m_cComboMIC_Type.AddString(m_ctrlSoundCard.strDeviceNameArry.GetAt(i));
	//}
	int lSel_Id = m_cComboMIC_Type.GetCurSel();
	CString strDevName;
	m_cComboMIC_Type.GetLBText(lSel_Id, strDevName);
	
	if (g_SoundCard.wIn_Flag) 
		g_SoundCard.WaveRead_Stop();
	g_SoundCard.WaveRead_StartUSB(strDevName, 1);

	//if (!m_ctrlSoundCard.wIn_Flag)
	//	m_ctrlSoundCard.WaveRead_StartUSB(strDevName,1);
}
