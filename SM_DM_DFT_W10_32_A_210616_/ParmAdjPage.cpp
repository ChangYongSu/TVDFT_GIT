// ParmAdjPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ParmAdjPage.h"
#include "SignalTestResultDlg.h"
#include "ImageTotalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//+2007.3.20 Add BY USY
#include "Global.h"
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CParmAdjPage dialog


CParmAdjPage::CParmAdjPage(CWnd* pParent /*=NULL*/)
	: CDialog(CParmAdjPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParmAdjPage)
	m_nBinaryType = 0;
	m_nRegionType = 0;
	//}}AFX_DATA_INIT

	m_pSignalTestResultDlg = NULL;
}


void CParmAdjPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParmAdjPage)
	DDX_Control(pDX, IDC_EDIT_PARM_NO_SKIPPED_LOW_BITS, m_ctrlNoSkippedSignalLineLowBits);
	DDX_Control(pDX, IDC_MASK_WIDTH_CMB, m_ctrlMaskWidthCmb);
	DDX_Control(pDX, IDC_EDIT_PARM_MASK_IMG_PATH, m_ctrlMaskImgPath);
	DDX_Control(pDX, IDC_CHK_PARM_USE_MASK_IMAGE, m_ctrlUseMaskImageChk);
	DDX_Control(pDX, IDC_EDIT_PARM_ROI_WIDTH, m_ctrlRoiWidthEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_ROI_HEIGHT, m_ctrlRoiHeightEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_ROI_Y, m_ctrlRoiYEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_ROI_X, m_ctrlRoiXEdit);
	DDX_Control(pDX, IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS, m_ctrlColMagHighbitsChk);
	DDX_Control(pDX, IDC_CHK_PARM_COLOR_MARGIN_LEVEL, m_ctrlColMagLevelChk);
	DDX_Control(pDX, IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS, m_ctrlColMagHighbitsEdit);
	//+del kwmoon 080122
	//	DDX_Control(pDX, IDC_EDIT_PARM_COLOR_MARGIN_LEVEL, m_ctrlColMagLevelEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_POSITION_SHIFT_X, m_ctrlPosShiftXEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_POSITION_SHIFT_Y, m_ctrlPosShiftYEdit);
	DDX_Control(pDX, IDC_STATIC_PARM_REF_B_VAL, m_ctrlRefBValStatic);
	DDX_Control(pDX, IDC_STATIC_PARM_REF_G_VAL, m_ctrlRefGValStatic);
	DDX_Control(pDX, IDC_STATIC_PARM_REF_R_VAL, m_ctrlRefRValStatic);
	DDX_Control(pDX, IDC_STATIC_PARM_GRAB_B_VAL, m_ctrlGrabBValStatic);
	DDX_Control(pDX, IDC_STATIC_PARM_GRAB_G_VAL, m_ctrlGrabGValStatic);
	DDX_Control(pDX, IDC_STATIC_PARM_GRAB_R_VAL, m_ctrlGrabRValStatic);
	DDX_Control(pDX, IDC_EDIT_PARM_Y1_POS, m_ctrlYPosEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_X1_POS, m_ctrlXPosEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_THRESHOLD, m_ctrlThresholdEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_REF_IMG_PATH, m_ctrlRefImgPath);
	DDX_Control(pDX, IDC_EDIT_PARM_GRAB_IMG_PATH, m_ctrlGrabImgEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_MASK_Y, m_ctrlMaskYEdit);
	DDX_Control(pDX, IDC_EDIT_PARM_MASK_X, m_ctrlMaskXEdit);
	DDX_Radio(pDX, IDC_RADIO_NORMAL, m_nBinaryType);
//	DDX_Control(pDX, IDC_RESULT_GRID, m_ctrlResultGrid);
	DDX_Radio(pDX, IDC_RADIO_ROI, m_nRegionType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_RESULT_LIST, m_ctrlResultList);
}


BEGIN_MESSAGE_MAP(CParmAdjPage, CDialog)
	//{{AFX_MSG_MAP(CParmAdjPage)
	ON_BN_CLICKED(IDC_BTN_PARM_FILTER, OnBtnParmFilter)
	ON_BN_CLICKED(IDC_BTN_PARM_BINARY, OnBtnParmBinary)
	ON_BN_CLICKED(IDC_BTN_PARM_PATTERN_MATCH, OnBtnParmPatternMatch)
	ON_BN_CLICKED(IDC_BTN_PARM_COLOR_TEST, OnBtnParmColorTest)
	ON_BN_CLICKED(IDC_BTN_PARM_OPEN_GRAB_IMG, OnBtnParmOpenGrabImg)
	ON_BN_CLICKED(IDC_BTN_PARM_OPEN_REF_IMG, OnBtnParmOpenRefImg)
	ON_BN_CLICKED(IDC_BTN_PARM_RELOAD, OnBtnParmReload)
	ON_BN_CLICKED(IDC_BTN_PARM_GET_RGB_VALUE, OnBtnParmGetRgbValue)
	ON_BN_CLICKED(IDC_CHK_PARM_COLOR_MARGIN_LEVEL, OnChkParmColorMarginLevel)
	ON_BN_CLICKED(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS, OnChkParmColorMarginHighBits)
	ON_BN_CLICKED(IDC_BTN_PARM_MAKE_MASK_IMAGE, OnBtnParmMakeMaskImage)
	ON_BN_CLICKED(IDC_BTN_PARM_SAVE_AS, OnBtnParmSaveAs)
	ON_BN_CLICKED(IDC_BTN_PARM_OPEN_MASK_IMG, OnBtnParmOpenMaskImg)
	ON_BN_CLICKED(IDC_CHK_PARM_USE_MASK_IMAGE, OnChkParmUseMask)
	ON_BN_CLICKED(IDC_RADIO_NORMAL, OnRadioNormal)
	ON_BN_CLICKED(IDC_RADIO_REVERSE, OnRadioReverse)
	ON_BN_CLICKED(IDC_BTN_PARM_SIGNAL_LINE_TEST, OnBtnParmSignalLineTest)
	ON_BN_CLICKED(IDC_BTN_PARM_LABEL, OnBtnParmLabel)
	ON_BN_CLICKED(IDC_BTN_PARM_REPAINT, OnBtnParmRepaint)
	ON_BN_CLICKED(IDC_BTN_PARM_ZOOM_OUT, OnBtnParmZoomOut)
	ON_BN_CLICKED(IDC_BTN_PARM_NOISE_TEST, OnBtnParmNoiseTest)
	ON_BN_CLICKED(IDC_RADIO_OSD_REGION, OnRadioOsdRegion)
	ON_BN_CLICKED(IDC_RADIO_ROI, OnRadioRoi)
	ON_BN_CLICKED(IDC_BTN_PARM_MOVING_PIC_TEST, OnBtnParmMovingPicTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmAdjPage message handlers

BOOL CParmAdjPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
 	InitParmValue();	

	for(int nTmp = I_GRAB; nTmp <= I_MASK; nTmp++)
	{
		m_bFirstOpen[nTmp]			 = FALSE;
		m_bRefresh[nTmp]			 = FALSE;
		m_Image[nTmp].m_bImageLoaded = FALSE;
	}

	m_bZoomAllReset = FALSE;

	// Full Path (Path+FileName)
	m_strGrabPath	= _T("");
	m_strRefPath	= _T("");
	m_strMaskPath	= _T("");

	// File Name
	m_strGrabFileName = _T("");
	m_strRefFileName  = _T("");
	m_strMaskFileName = _T("");

	// Folder Name
	m_strGrabFolder	= _T("");
	m_strRefFolder	= _T("");
	m_strMaskFolder	= _T("");

	m_ctrlMaskWidthCmb.AddString("1");
	m_ctrlMaskWidthCmb.AddString("2");
	m_ctrlMaskWidthCmb.AddString("3");
	m_ctrlMaskWidthCmb.AddString("4");
	m_ctrlMaskWidthCmb.AddString("5");
	m_ctrlMaskWidthCmb.AddString("6");
	m_ctrlMaskWidthCmb.AddString("7");
	m_ctrlMaskWidthCmb.AddString("8");
	m_ctrlMaskWidthCmb.AddString("9");
	m_ctrlMaskWidthCmb.AddString("10");
	m_ctrlMaskWidthCmb.SetCurSel(0);
	
	InitResultGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CParmAdjPage::OnBtnParmFilter() 
{
//	if(m_nTabType != P_PARM) return;

 	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_MASK_X);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_MASK_Y);

	CString sTmp = _T("");
	int nXMask, nYMask;

	pEdit1->GetWindowText(sTmp); nXMask = atoi(sTmp);
	pEdit2->GetWindowText(sTmp); nYMask = atoi(sTmp);

	m_bZoomAllReset = TRUE;
	
	//+change kwmoon 080820
//	m_ImageProc.Average(nXMask, nYMask, m_Image[*m_nImgType], TRUE);
	m_ImageProc.Average(nXMask, nYMask, m_Image[*m_nImgType]);

	m_MWnd->Invalidate(FALSE);	
}

void CParmAdjPage::OnBtnParmBinary() 
{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_THRESHOLD);

	CString sTmp = _T("");

	int nThresHold = 0;

	pEdit1->GetWindowText(sTmp); 
	
	nThresHold = atoi(sTmp);

	m_bZoomAllReset = TRUE;

	m_ImageProc.BinaryForColorImage(m_nBinaryType,m_Image[*m_nImgType],nThresHold,TRUE);

	m_MWnd->Invalidate(FALSE);			
}

void CParmAdjPage::OnBtnParmPatternMatch() 
{
	UpdateData(TRUE);
	
	if(m_strGrabPath == _T("") || m_strRefPath == _T("")) return;

//	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_MARGIN_X);
//	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_MARGIN_Y);

	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_X);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_Y);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_WIDTH);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_HEIGHT);
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_X);
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_Y);

	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_MATCH_RATE);

	
	CString sTmp = _T("");

	int	nRoiX1,nRoiY1,nRoiX2,nRoiY2,nPositionShiftX,nPositionShiftY;

	//+add kwmoon 070111
	float fMatchRate = 0.0f;

//	pEdit1->GetWindowText(sTmp); nXMargin = atoi(sTmp);
//	pEdit2->GetWindowText(sTmp); nYMargin = atoi(sTmp);

	pEdit5->GetWindowText(sTmp); nRoiX1 = atoi(sTmp);
	pEdit6->GetWindowText(sTmp); nRoiY1 = atoi(sTmp);
	pEdit7->GetWindowText(sTmp); nRoiX2 = atoi(sTmp);
	pEdit8->GetWindowText(sTmp); nRoiY2 = atoi(sTmp);
	pEdit9->GetWindowText(sTmp); nPositionShiftX = atoi(sTmp);
	pEdit10->GetWindowText(sTmp); nPositionShiftY = atoi(sTmp);

	pEdit11->GetWindowText(sTmp); fMatchRate = (float)atof(sTmp);

	
	int nResult = TEST_FAIL;

	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	POINT ptPositionShift;
	ptPositionShift.x = nPositionShiftX;
	ptPositionShift.y = nPositionShiftY;

	POINT ptRoi1Position;
	ptRoi1Position.x = nRoiX1;
	ptRoi1Position.y = nRoiY1;

	POINT ptRoi2Position;
	ptRoi2Position.x = nRoiX2;
	ptRoi2Position.y = nRoiY2;

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;


	CString szMsg;

	m_ImageProc.SetPositionShift(ptPositionShift);
	m_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position);
	m_ImageProc.SetMatchRate(fMatchRate);

	nResult = m_ImageProc.PatternMatching(POSITION_TEST,m_Image[I_GRAB],m_Image[I_REF],ptResultPosition,fMatchRate,*m_ctrlProgress);

	if(nResult == TEST_PASS)
	{
		szMsg.Format("[Pattern Match]-PASS!");
	}
	else if(nResult == TEST_FAIL)
	{
		szMsg.Format("[Pattern Match]-FAIL!");
	}
	else
	{
		szMsg.Format("[Pattern Match]-ABORT!");
	}
	AddStringToResultGrid(szMsg);

	szMsg.Format("Match : %.1f%% (%d,%d)",fMatchRate,ptResultPosition.x,ptResultPosition.y);
	AddStringToResultGrid(szMsg);

	// Initialize Roi variable
	m_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	m_bZoomAllReset = TRUE;
	m_MWnd->Invalidate(FALSE);		
}

//+change kwmoon 070626
void CParmAdjPage::OnBtnParmColorTest() 
{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_X);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_Y);

	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS);
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_X);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_Y);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_WIDTH);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_HEIGHT);

	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_LOWER_LIMIT);
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_UPPER_LIMIT);


	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_LEVEL); 
	CButton* pCheck2 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS);
	CButton* pCheck3 = (CButton*)GetDlgItem(IDC_CHK_PARM_USE_MASK_IMAGE);


	CString sTmp = _T("");

	int nXPosShift, nYPosShift, nLevel, nHighbits,nRoiX1,nRoiY1,nRoiX2,nRoiY2;
	float fLowerLimit, fUpperLimit;
	
	pEdit1->GetWindowText(sTmp); nXPosShift = atoi(sTmp);
	pEdit2->GetWindowText(sTmp); nYPosShift = atoi(sTmp);

	UpdateData(TRUE);

	//+change kwmoon 080122
	if(m_nRegionType == ROI_COORDINATES)
	{
		pEdit5->GetWindowText(sTmp); nRoiX1 = atoi(sTmp);
		pEdit6->GetWindowText(sTmp); nRoiY1 = atoi(sTmp);
		pEdit7->GetWindowText(sTmp); nRoiX2 = atoi(sTmp);
		pEdit8->GetWindowText(sTmp); nRoiY2 = atoi(sTmp);
	}
	else
	{
		nRoiX1	= 0;
		nRoiY1	= 0;
		nRoiX2  = 0;
		nRoiY2	= 0;
	}
	//-

	//+change kwmoon 070929
	pEdit9->GetWindowText(sTmp); fLowerLimit = (float)atof(sTmp);
	pEdit10->GetWindowText(sTmp); fUpperLimit = (float)atof(sTmp);


	int nResult = TEST_FAIL;
	int nMaxDifference = 0;
	int nMaxVariation	= 0;
	int nPlusMaxDifference = 0;
	int nMinusMaxDifference = 0;
	int nTotalNoErrorPixel = 0;

	// Variable for Position Shift
	POINT ptPositionShift;
	ptPositionShift.x = nXPosShift;
	ptPositionShift.y = nYPosShift;

	// Variable for Max Difference Position
	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	//+del kwmoon 080716
//	POINT ptPlusResultPosition;
//	ptPlusResultPosition.x = 0;
//	ptPlusResultPosition.y = 0;

	//+del kwmoon 080716
//	POINT ptMinusResultPosition;
//	ptMinusResultPosition.x = 0;
//	ptMinusResultPosition.y = 0;

	POINT ptRoi1Position;
	ptRoi1Position.x = nRoiX1;
	ptRoi1Position.y = nRoiY1;

	POINT ptRoi2Position;
	ptRoi2Position.x = nRoiX2;
	ptRoi2Position.y = nRoiY2;

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

	//+add kwmoon 080716
	float fMaxDifference = 0;

	CString szMsg;

	m_ImageProc.SetPositionShift(ptPositionShift);
	m_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position);

	if(pCheck3->GetCheck() == 1) // Use Mask Image
	{
		m_bUseMaskImage = TRUE;
	}

	if(pCheck1->GetCheck() == 1) // Use Margin
	{
		//+add kwmoon 070205[#1]
		nHighbits = 0;

		//+change kwmoon 070929
		pEdit9->GetWindowText(sTmp); fLowerLimit = (float)atof(sTmp);
		pEdit10->GetWindowText(sTmp); fUpperLimit = (float)atof(sTmp);

		pEdit4->SetWindowText("0");
	}
	else // Use High Bits
	{
		//+add kwmoon 070205[#1]
		nLevel = 0;
		pEdit4->GetWindowText(sTmp); nHighbits = atoi(sTmp);
	}

	m_ImageProc.SetColorLimit(fLowerLimit,fUpperLimit);
	m_ImageProc.SetUsedNoBrightHighBits(nHighbits);
	m_ImageProc.SetUsedNoColorHighBits(nHighbits);

	//+change kwmoon 080716
//	nResult = m_ImageProc.RGBTest(NORMAL_IMAGE_TEST,m_Image[I_GRAB], m_Image[I_REF],m_Image[I_MASK],
//								  ptPlusResultPosition,ptMinusResultPosition, 
//								  nPlusMaxDifference,nMinusMaxDifference,nTotalNoErrorPixel,m_bUseMaskImage,*m_ctrlProgress);
	nResult = m_ImageProc.RGBTest(NORMAL_IMAGE_TEST,m_Image[I_GRAB], m_Image[I_REF],m_Image[I_MASK],
								  ptResultPosition,fMaxDifference,nTotalNoErrorPixel,m_bUseMaskImage,*m_ctrlProgress);

	m_ImageProc.SetAvgMaskSize(0,0,0); // Initialize
	m_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	if(nResult == TEST_ABORT) return;

	if(m_Image[I_GRAB].m_bProcessedImageLoaded)
	{
		for(int i=0; i<(int)m_Image[I_GRAB].m_InfoHeader.biSizeImage; i++)
		{
			m_Image[I_PROC].m_pImageData[i]			 = m_Image[I_GRAB].m_pProcessedImageData[i];
		}
	}

	//+del kwmoon 080716
/*
	// Display Max Difference in an absolute value
	if(nPlusMaxDifference >= abs(nMinusMaxDifference))
	{
		nMaxDifference		= nPlusMaxDifference;
		ptResultPosition.x  = ptPlusResultPosition.x;
		ptResultPosition.y  = ptPlusResultPosition.y;
	}
	else
	{
		nMaxDifference		= abs(nMinusMaxDifference);
		ptResultPosition.x  = ptMinusResultPosition.x;
		ptResultPosition.y  = ptMinusResultPosition.y;
	}
*/
	if(nResult == TEST_PASS)
	{
		szMsg.Format("[Color Test]-PASS!");
	}
	else
	{
		szMsg.Format("[Color Test]-FAIL!");
	}
	AddStringToResultGrid(szMsg);

	if(nResult == TEST_PASS)
	{
		szMsg.Format("        Max Level Difference : %.1f",fMaxDifference);
	}
	else
	{
		szMsg.Format("        Max Level Difference : %.1f (%d,%d),(%d Pixel)",
							  fMaxDifference,ptResultPosition.x,ptResultPosition.y,nTotalNoErrorPixel);
	}
	AddStringToResultGrid(szMsg);

	//+del kwmoon 080716
/*
	// Display Max variation positions and values
	nMaxVariation = nPlusMaxDifference - nMinusMaxDifference;

	szMsg.Format("        Max Level Variation : %d => %d(%d,%d), %d(%d,%d)",nMaxVariation,
				 nPlusMaxDifference,ptPlusResultPosition.x,ptPlusResultPosition.y,
				 nMinusMaxDifference,ptMinusResultPosition.x,ptMinusResultPosition.y );

	AddStringToResultGrid(szMsg);
*/
	m_bZoomAllReset = FALSE;

	m_MWnd->SendMessage(UM_UPDATE_ALL_VIEW_TAB);
	m_MWnd->SendMessage(UM_CHANGE_ACTIVE_VIEW_TAB,I_PROC);
}

void CParmAdjPage::OnBtnParmOpenGrabImg() 
{
	CFileDialog dlg(TRUE, "bmp", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Grab Image Files(*.bmp)|*.bmp|",NULL);

	CString sTmp = _T("");

	if(CurrentSet->sParmAdjGrabFolder.IsEmpty())
	{
//		sTmp = CurrentSet->sModelFolder;
		sTmp = CurrentSet->sRefFolder;
	}
	else
	{
		sTmp = CurrentSet->sParmAdjGrabFolder;
	}

	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strGrabPath	  = dlg.GetPathName();
		m_strGrabFileName = dlg.GetFileName();
		m_strGrabFolder	  = m_strGrabPath.Left(m_strGrabPath.ReverseFind('\\'));

		// Save Grab Folder
		CurrentSet->sParmAdjGrabFolder = m_strGrabFolder;
	
		OpenImgFile(m_strGrabPath, I_GRAB);
		OpenImgFile(m_strGrabPath, I_PROC);

		*m_nImgType = I_GRAB;
	}
}

void CParmAdjPage::OnBtnParmOpenRefImg() 
{
	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_USE_MASK_IMAGE); 

	CFileDialog dlg(TRUE, "bmp", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Reference Image Files(*.bmp)|*.bmp|",NULL);

	CString sTmp = _T("");

	if(CurrentSet->sParmAdjRefFolder.IsEmpty())
	{
//		sTmp = CurrentSet->sModelFolder;
		sTmp = CurrentSet->sRefFolder;
	}
	else
	{
		sTmp = CurrentSet->sParmAdjRefFolder;
	}

	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		//  Open Reference Image
		m_strRefPath	 = dlg.GetPathName();
		m_strRefFileName = dlg.GetFileName();
		m_strRefFolder	 = m_strRefPath.Left(m_strRefPath.ReverseFind('\\'));

		// Save Ref Folder
		CurrentSet->sParmAdjRefFolder = m_strRefFolder;

		OpenImgFile(m_strRefPath, I_REF);
		OpenImgFile(m_strRefPath, I_PROC);

		//  Open Mask Image
		m_strMaskPath	 = m_strRefPath.Left(m_strRefPath.ReverseFind('\\'))+"\\Mask\\" + m_strRefFileName;
		m_strMaskFolder	 = m_strMaskPath.Left(m_strMaskPath.ReverseFind('\\'));
			   
		// Save Mask Folder
		CurrentSet->sParmAdjMaskFolder = m_strMaskFolder;

		if(OpenImgFile(m_strMaskPath, I_MASK) == FALSE)
		{
			pCheck1->SetCheck(FALSE);
			pCheck1->EnableWindow(FALSE);
			m_ctrlMaskImgPath.SetWindowText("");
		}
		else
		{
			pCheck1->EnableWindow(TRUE);
		}

		*m_nImgType = I_REF;
	}
}


void CParmAdjPage::OnBtnParmReload() 
{
	if(!m_strMaskPath.IsEmpty()) OpenImgFile(m_strMaskPath, I_MASK);
	if(!m_strGrabPath.IsEmpty()) OpenImgFile(m_strGrabPath, I_GRAB);	
	if(!m_strGrabPath.IsEmpty()) OpenImgFile(m_strGrabPath, I_PROC);
	if(!m_strRefPath.IsEmpty())  OpenImgFile(m_strRefPath, I_REF);
}

void CParmAdjPage::OnBtnParmGetRgbValue() 
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
//////////////////////////////////////////////////////////////////////////
//+2007.3.20 Add BY USY
BOOL CParmAdjPage::GetRgbValue(int nImgType, CBaseImage& Image, int nXPos, int nYPos, int& nRvalue,int& nGvalue,int& nBvalue)
{
	int nBytesInRow = 0;

	CString szErrMsg;

	if(!Image.m_bImageLoaded)
	{
		if(nImgType == 0) 
		{
			AfxMessageBox("Grab image is not loaded!"); 
			return FALSE;
		}
		else if(nImgType == 1) 
		{
			AfxMessageBox("Reference image is not loaded!"); 
			return FALSE;
		}
//		else if(nImgType == 2) 
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

void CParmAdjPage::InitParmValue()
{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_MASK_X);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_MASK_Y);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_THRESHOLD);
//	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_MARGIN_X);
//	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_MARGIN_Y);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_X);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_Y);
//	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_CROP_X);
//	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_CROP_Y);
	//+del kwmoon 080122
//	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_LEVEL);
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS);

	CEdit* pEdit12 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_X1_POS);
	CEdit* pEdit13 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_Y1_POS);
	CEdit* pEdit14 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_X);
	CEdit* pEdit15 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_Y);
	CEdit* pEdit16 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_WIDTH);
	CEdit* pEdit17 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_HEIGHT);
	

	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_LEVEL); 
	CButton* pCheck2 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS);
	CButton* pCheck3 = (CButton*)GetDlgItem(IDC_CHK_PARM_USE_MASK_IMAGE);
	
	CString sTmp = "0";

	pEdit1->SetWindowText(sTmp);
	pEdit2->SetWindowText(sTmp);
	pEdit3->SetWindowText(sTmp);
//	pEdit4->SetWindowText(sTmp);
//	pEdit5->SetWindowText(sTmp);
	pEdit6->SetWindowText(sTmp);
	pEdit7->SetWindowText(sTmp);
//	pEdit8->SetWindowText(sTmp);
//	pEdit9->SetWindowText(sTmp);
	//+del kwmoon 080122
//	pEdit10->SetWindowText(sTmp);
	pEdit11->SetWindowText(sTmp);
	pEdit12->SetWindowText(sTmp);
	pEdit13->SetWindowText(sTmp);
	pEdit14->SetWindowText(sTmp);
	pEdit15->SetWindowText(sTmp);
	pEdit16->SetWindowText(sTmp);
	pEdit17->SetWindowText(sTmp);

//	pList->ResetContent();

	pCheck1->SetCheck(TRUE);
	//+del kwmoon 080122
//	pEdit10->EnableWindow(TRUE);
	pCheck2->SetCheck(FALSE);
	pEdit11->EnableWindow(FALSE);

	m_bUseMaskImage = FALSE;
	pCheck3->SetCheck(FALSE);
}

BOOL CParmAdjPage::OpenImgFile(CString sImgPath, int nImgType)
{
	CString sTmp;
	if(!FileExists(sImgPath))
	{
		sTmp.Format("%s\n File doesn't Exist!", sImgPath);
		AfxMessageBox(sTmp);
		return FALSE;
	}

	if(!m_Image[nImgType].LoadImage(sImgPath, *m_ctrlProgress)) return FALSE;
	*m_nImgType = nImgType;
	m_bFirstOpen[nImgType]	= TRUE;
	m_bZoomAllReset			= TRUE;
	m_bRefresh[nImgType]	= FALSE;
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_GRAB_IMG_PATH);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_REF_IMG_PATH);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_MASK_IMG_PATH);

	int nFileNamePos = sImgPath.ReverseFind('\\');

	CString strImgFileName = sImgPath.Mid(nFileNamePos+1,sImgPath.GetLength()-nFileNamePos-1); 

	switch(nImgType) 
	{
		case I_GRAB:
			pEdit1->SetWindowText(strImgFileName);
			break;
		case I_REF:
			pEdit2->SetWindowText(strImgFileName);
			break;
		//+ 2007.5.1 Add BY USY
		case I_MASK:
			pEdit3->SetWindowText(strImgFileName);
			break;
		//-
	}
	m_MWnd->Invalidate(TRUE);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

//+change kwmoon 060726
void CParmAdjPage::OnChkParmColorMarginLevel() 
{
	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_LEVEL); 
	CButton* pCheck2 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS);

	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_LOWER_LIMIT);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_UPPER_LIMIT);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS);

	pCheck1->SetCheck(TRUE);
	pCheck2->SetCheck(FALSE);

	pEdit1->EnableWindow(TRUE);
	pEdit2->EnableWindow(TRUE);
	pEdit3->EnableWindow(FALSE);
}

//+change kwmoon 060726
void CParmAdjPage::OnChkParmColorMarginHighBits() 
{
	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_LEVEL); 
	CButton* pCheck2 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS);

	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_LOWER_LIMIT);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_UPPER_LIMIT);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS);


	pCheck1->SetCheck(FALSE);
	pCheck2->SetCheck(TRUE);

	pEdit1->EnableWindow(FALSE);
	pEdit2->EnableWindow(FALSE);
	pEdit3->EnableWindow(TRUE);
}

//+del kwmoon 080122
/*
void CParmAdjPage::OnBtnParmBrightTest() 
{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_X);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_Y);
	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_LEVEL);
	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS);

	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_LEVEL); 
	CButton* pCheck2 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS);


	CString sTmp = _T("");

	int nXPosShift, nYPosShift, nLevel, nHighbits;
	pEdit1->GetWindowText(sTmp); nXPosShift = atoi(sTmp);
	pEdit2->GetWindowText(sTmp); nYPosShift = atoi(sTmp);


	int nResult = TEST_FAIL;
	int nMaxDifference = 0;

	// Variable for Position Shift
	POINT ptPositionShift;
	ptPositionShift.x = nXPosShift;
	ptPositionShift.y = nYPosShift;

	// Variable for Max Difference Position
	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	CString szMsg;

	//+add kwmoon 070201[#1]
	m_ImageProc.SetPositionShift(ptPositionShift);


	//+add kwmoon 070205[#1]
	if(pCheck1->GetCheck() == 1) // Use Margin
	{
		//+add kwmoon 070205[#1]
		nHighbits = 0;
		pEdit3->GetWindowText(sTmp); nLevel = atoi(sTmp);
		pEdit4->SetWindowText("0");
	}
	else // Use High Bits
	{
		//+add kwmoon 070205[#1]
		nLevel = 0;
		pEdit4->GetWindowText(sTmp); nHighbits = atoi(sTmp);
		pEdit3->SetWindowText("0");
	}

	//+add kwmoon 070115
	m_ImageProc.SetColorMargin(nLevel);

	//+add kwmoon 070203[#3]
	m_ImageProc.SetUsedNoBrightHighBits(nHighbits);
	m_ImageProc.SetUsedNoColorHighBits(nHighbits);
	//-

	//+ 2007.4.28 Mod BY USY
	nResult = m_ImageProc.BrightnessTest(m_Image[I_GRAB], m_Image[I_REF],m_Image[I_MASK],ptResultPosition,nMaxDifference,*m_ctrlProgress);
//	nResult = m_ImageProc.BrightnessTest(m_Image[I_GRAB], m_Image[I_PROC],m_MaskImage,ptResultPosition,nMaxDifference,*m_ctrlProgress);
	//-

	//+change kwmoon 070205[#2]
	//+add kwmoon 070201[#3]
	m_ImageProc.SetAvgMaskSize(0,0,0); // Initialize

	for(int i=0; i<(int)m_Image[I_GRAB].m_InfoHeader.biSizeImage; i++)
	{
		m_Image[I_PROC].m_pImageData[i] = m_Image[I_GRAB].m_pProcessedImageData[i];
	}

	if(nResult == TEST_PASS)
	{
		szMsg.Format("[Brightness Test]-PASS!");
	}
	else
	{
		szMsg.Format("[Brightness Test]-FAIL!");
	}
	AddStringToResultGrid(szMsg);
	
	szMsg.Format("        Max Level Difference : %d(%d,%d)",nMaxDifference,ptResultPosition.x,ptResultPosition.y);
	AddStringToResultGrid(szMsg);

	m_bZoomAllReset = FALSE;


	m_MWnd->Invalidate(FALSE);	
}
*/

//+change kwmoon 070912
void CParmAdjPage::OnBtnParmMakeMaskImage() 
{
	int nMaskWidth = m_ctrlMaskWidthCmb.GetCurSel();

	//+add kwmoon 080122
	POINT ptStartPosition;
	POINT ptEndPosition;

	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_X);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_Y);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_WIDTH);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_HEIGHT);

	CString sTmp = _T("");

	int	nRoiX1 = 0,nRoiY1 = 0,nRoiX2 = 0,nRoiY2 = 0;

	UpdateData(TRUE);

	if(m_nRegionType == OSD_REGION_COORDINATES)
	{
		pEdit5->GetWindowText(sTmp); nRoiX1 = atoi(sTmp);
		pEdit6->GetWindowText(sTmp); nRoiY1 = atoi(sTmp);
		pEdit7->GetWindowText(sTmp); nRoiX2 = atoi(sTmp);
		pEdit8->GetWindowText(sTmp); nRoiY2 = atoi(sTmp);

		ptStartPosition.x = nRoiX1;
		ptStartPosition.y = nRoiY1;
		ptEndPosition.x   = nRoiX2;
		ptEndPosition.y   = nRoiY2; 

		m_ImageProc.SetOsdRegion(ptStartPosition,ptEndPosition);

		m_ImageProc.MakeMaskImg(nMaskWidth+1, m_Image[*m_nImgType], m_Image[I_MASK], *m_ctrlProgress,TRUE);
	}
	else
	{
		m_ImageProc.MakeMaskImg(nMaskWidth+1, m_Image[*m_nImgType], m_Image[I_MASK], *m_ctrlProgress,FALSE);
	}
	//-


	m_MWnd->SendMessage(UM_UPDATE_ALL_VIEW_TAB);
	m_MWnd->SendMessage(UM_CHANGE_ACTIVE_VIEW_TAB,I_MASK);
}

void CParmAdjPage::OnBtnParmSaveAs() 
{
	CString szInitFolder;
	CString szInitFile;
	CString sBuf;	
//	char	szInitFileName[100];
//	ZeroMemory(szInitFileName,100);
	
	CBaseImage* pBaseImage = NULL;

	int nFileNamePos = 0;

	sBuf = CurrentSet->sRefFolder;
	sBuf.MakeLower(); 
	sBuf.TrimRight("\\ref"); 
		

	switch(*m_nImgType)
	{
		case I_GRAB		 :
		{
			pBaseImage	 = &m_Image[I_GRAB];	
			szInitFolder = sBuf + "\\grab\\";  

			nFileNamePos = m_strGrabPath.ReverseFind('\\');
			szInitFile	 = m_strGrabPath.Mid(nFileNamePos+1,m_strGrabPath.GetLength()-nFileNamePos-1); 
		}
		break;
		
		case I_REF		 : 
		{
			//+change kwmoon 070314[#2]
			pBaseImage	 = &m_Image[I_REF];	
		//	szInitFolder = CurrentSet->sModelFolder + "\\ref\\";  		
			szInitFolder = CurrentSet->sRefFolder + "\\";  		

			nFileNamePos = m_strRefPath.ReverseFind('\\');
			szInitFile	 = m_strRefPath.Mid(nFileNamePos+1,m_strRefPath.GetLength()-nFileNamePos-1); 
		}
		break;
		case I_PROC		 :
		{
			// In case of I_PROC, Use m_Image[I_GRAB] instead of m_Image[I_PROC]
			pBaseImage	 = &m_Image[I_GRAB];	
			szInitFolder = sBuf + "\\grab\\";  

			nFileNamePos = m_strGrabPath.ReverseFind('\\');
			szInitFile	 = m_strGrabPath.Mid(nFileNamePos+1,m_strGrabPath.GetLength()-nFileNamePos-1); 
		}
		break;
		//+ 2007.4.28 Add BY USY
		case I_MASK		 :
		{
			pBaseImage = &m_Image[I_MASK];
			//szInitFolder = CurrentSet->sModelFolder + "\\ref\\mask\\";
			szInitFolder = CurrentSet->sRefFolder + "\\mask\\";  		

			nFileNamePos = m_strMaskPath.ReverseFind('\\');
			szInitFile	 = m_strMaskPath.Mid(nFileNamePos+1,m_strMaskPath.GetLength()-nFileNamePos-1); 
		}
		break;
		//-
	}
	//-
	
	// Load할 BMP파일 선택
	CString szFileName;
	CString szFilter = "BMP (*.bmp) | *.bmp||";

	if(pBaseImage->m_bImageLoaded != TRUE)
	{
		AfxMessageBox("No Image is snapped yet!"); return;
	}
	
//	szInitFolder = CurrentSet->sModelFolder + "\\ref\\";  
	SetCurrentDirectory(szInitFolder);

//	strcpy(szInitFileName,(LPTSTR)szInitFile.GetBuffer(0));

	CFileDialog dlg(FALSE,".bmp",NULL,OFN_HIDEREADONLY,szFilter);
	dlg.m_ofn.lpstrInitialDir = szInitFolder;
//	dlg.m_ofn.lpstrFile = szInitFileName;

	m_ctrlProgress->SetPos(0);

	if(dlg.DoModal() == IDOK)
	{
//		szFileName = dlg.GetFileName();
		szFileName = dlg.GetPathName();

		if(!pBaseImage->SaveImage(szFileName,*m_nImgType,*m_ctrlProgress)){ return;}
		
	}
	m_ctrlProgress->SetPos(100);
//	SetGrabType(GRAB_START);
}

void CParmAdjPage::OnBtnParmOpenMaskImg() 
{
	CFileDialog dlg(TRUE, "bmp", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Mask Image Files(*.bmp)|*.bmp|",NULL);

	CString sTmp = CurrentSet->sRefFolder + "\\mask\\";
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strMaskPath = dlg.GetPathName();
		OpenImgFile(m_strMaskPath, I_MASK);
	}
	
//	InitParmValue();
}
BOOL CParmAdjPage::PreTranslateMessage(MSG* pMsg) 
{
	//+ 2007.5.4 Add BY USY
	//Protect Dialog close By Enter Key or ESC Key
	if(pMsg->message == WM_KEYDOWN)
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
            return TRUE;
	//-
	return CDialog::PreTranslateMessage(pMsg);
}

void CParmAdjPage::OnChkParmUseMask() 
{
	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_USE_MASK_IMAGE); 
	
	if(m_bUseMaskImage)
	{
		m_bUseMaskImage = FALSE;
		pCheck1->SetCheck(FALSE);
	}
	else
	{
		m_bUseMaskImage = TRUE;
		pCheck1->SetCheck(TRUE);
	}
}
void CParmAdjPage::OnRadioNormal() 
{
	// TODO: Add your control notification handler code here
	m_nBinaryType	= NORMAL_BINARY;
}

void CParmAdjPage::OnRadioReverse() 
{
	// TODO: Add your control notification handler code here
	m_nBinaryType	= REVERSE_BINARY;
}

void CParmAdjPage::OnBtnParmSignalLineTest() 
{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_X);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_Y);
	
//	CEdit* pEdit3 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_LEVEL);

	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS);
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_X);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_Y);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_WIDTH);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_HEIGHT);

	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_LOWER_LIMIT);
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_UPPER_LIMIT);
	CEdit* pEdit11 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_NO_SKIPPED_LOW_BITS);


	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_LEVEL); 
	CButton* pCheck2 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS);
	CButton* pCheck3 = (CButton*)GetDlgItem(IDC_CHK_PARM_USE_MASK_IMAGE);


	CString sTmp = _T("");

	int nXPosShift, nYPosShift, nLevel, nHighbits,nRoiX1,nRoiY1,nRoiX2,nRoiY2, nNoSkippedSignalLineLowBits;
	float fLowerLimit, fUpperLimit;
	
	pEdit1->GetWindowText(sTmp); nXPosShift = atoi(sTmp);
	pEdit2->GetWindowText(sTmp); nYPosShift = atoi(sTmp);
	pEdit5->GetWindowText(sTmp); nRoiX1 = atoi(sTmp);
	pEdit6->GetWindowText(sTmp); nRoiY1 = atoi(sTmp);
	pEdit7->GetWindowText(sTmp); nRoiX2 = atoi(sTmp);
	pEdit8->GetWindowText(sTmp); nRoiY2 = atoi(sTmp);

	//+change kwmoon 070929
	pEdit9->GetWindowText(sTmp); fLowerLimit = (float)atof(sTmp);
	pEdit10->GetWindowText(sTmp); fUpperLimit = (float)atof(sTmp);
	
	pEdit11->GetWindowText(sTmp); nNoSkippedSignalLineLowBits = atoi(sTmp);


	int nResult = TEST_FAIL;
	int nMaxDifference = 0;
	int nMaxVariation	= 0;
	int nPlusMaxDifference = 0;
	int nMinusMaxDifference = 0;
	int nTotalNoErrorPixel = 0;

	// Variable for Position Shift
	POINT ptPositionShift;
	ptPositionShift.x = nXPosShift;
	ptPositionShift.y = nYPosShift;

	// Variable for Max Difference Position
	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	POINT ptPlusResultPosition;
	ptPlusResultPosition.x = 0;
	ptPlusResultPosition.y = 0;

	POINT ptMinusResultPosition;
	ptMinusResultPosition.x = 0;
	ptMinusResultPosition.y = 0;

	POINT ptRoi1Position;
	ptRoi1Position.x = nRoiX1;
	ptRoi1Position.y = nRoiY1;

	POINT ptRoi2Position;
	ptRoi2Position.x = nRoiX2;
	ptRoi2Position.y = nRoiY2;

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

	CString szMsg   = _T("");
	CString szColor = _T("");

	m_ImageProc.SetPositionShift(ptPositionShift);
	m_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position);

	if(pCheck3->GetCheck() == 1) // Use Mask Image
	{
		m_bUseMaskImage = TRUE;
	}


	if(pCheck1->GetCheck() == 1) // Use Margin
	{
		//+add kwmoon 070205[#1]
		nHighbits = 0;

	//	pEdit3->GetWindowText(sTmp); nLevel = atoi(sTmp);

		//+change kwmoon 070929
		pEdit9->GetWindowText(sTmp); fLowerLimit = (float)atof(sTmp);
		pEdit10->GetWindowText(sTmp); fUpperLimit = (float)atof(sTmp);

		pEdit4->SetWindowText("0");
	}
	else // Use High Bits
	{
		//+add kwmoon 070205[#1]
		nLevel = 0;
		pEdit4->GetWindowText(sTmp); nHighbits = atoi(sTmp);
//		pEdit3->SetWindowText("0");
	}

//	m_ImageProc.SetColorMargin(nLevel);
	m_ImageProc.SetColorLimit(fLowerLimit,fUpperLimit);

	m_ImageProc.SetUsedNoBrightHighBits(nHighbits);
	m_ImageProc.SetUsedNoColorHighBits(nHighbits);

	//+2007.3.26 Mod BY USY
	m_ImageProc.m_LineBitTestData.InitBitTestData();
	m_ImageProc.m_LineBitTestData.m_pGrabImage	= &m_Image[I_GRAB];
	m_ImageProc.m_LineBitTestData.m_pRefImage	= &m_Image[I_REF];
	m_ImageProc.m_LineBitTestData.m_pMaskImage	= &m_Image[I_MASK];
	m_ImageProc.m_LineBitTestData.m_nNoSkippedSignalLineLowBits = nNoSkippedSignalLineLowBits;

	nResult = m_ImageProc.SignalLineTest(NORMAL_IMAGE_TEST,m_bUseMaskImage,*m_ctrlProgress);

	m_ImageProc.SetAvgMaskSize(0,0,0); // Initialize
	m_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	if(nResult == TEST_ABORT) return;

	if(m_Image[I_GRAB].m_bProcessedImageLoaded)
	{
		for(int i=0; i<(int)m_Image[I_GRAB].m_InfoHeader.biSizeImage; i++)
		{
			m_Image[I_PROC].m_pImageData[i]			 = m_Image[I_GRAB].m_pProcessedImageData[i];
		}
	}

/*	
	// Display Max Difference in an absolute value
	if(nPlusMaxDifference >= abs(nMinusMaxDifference))
	{
		nMaxDifference		= nPlusMaxDifference;
		ptResultPosition.x  = ptPlusResultPosition.x;
		ptResultPosition.y  = ptPlusResultPosition.y;
	}
	else
	{
		nMaxDifference		= abs(nMinusMaxDifference);
		ptResultPosition.x  = ptMinusResultPosition.x;
		ptResultPosition.y  = ptMinusResultPosition.y;
	}
*/


	if(nResult == TEST_PASS)
	{
		szMsg.Format("[Line Test]-PASS!");
	
		AddStringToResultGrid(szMsg);
	}
	else
	{
		szMsg.Format("[Line Test]-FAIL!");

		AddStringToResultGrid(szMsg);

		switch(m_ImageProc.m_LineBitTestData.m_nErrorColor)
		{
			case 0 : szColor = "B"; break;
			case 1 : szColor = "G"; break;
			case 2 : szColor = "R"; break;
		}
		
		szMsg.Format("Faulty Bit Position : %s(%d)",szColor,
					 m_ImageProc.m_LineBitTestData.m_nErrorBitPosition);

		AddStringToResultGrid(szMsg);
	}
/*
	if(nResult == TEST_PASS)
	{
//		szMsg.Format("        Max Level Difference : %d",nMaxDifference);
	}
	else
	{
		/////////////////////////////////////////////////////////

		int nBitPosition = 0;
		int nMaxDifference = 0;
		float fDifferencePercentage = 0;
		CString szColor = _T("");

		CString szMsg1 = _T("");
		CString szMsg2 = _T("");
		CString szMsg3 = _T("");
		
		if(m_ImageProc.m_LineBitTestData.fPlusDifferencePercentage >= (m_ImageProc.m_LineBitTestData.fMinusDifferencePercentage*(-1)))
		{
			nMaxDifference			= m_ImageProc.m_LineBitTestData.nPlusMaxDifference;
			nBitPosition			= m_ImageProc.m_LineBitTestData.nPlusBitPosition;
			fDifferencePercentage	= m_ImageProc.m_LineBitTestData.fPlusDifferencePercentage;

			switch(m_ImageProc.m_LineBitTestData.nPlusColorType)
			{
				case 0 : szColor = "B"; break;
				case 1 : szColor = "G"; break;
				case 2 : szColor = "R"; break;
			}
		}
		else
		{
			nMaxDifference			= m_ImageProc.m_LineBitTestData.nMinusMaxDifference;
			nBitPosition			= m_ImageProc.m_LineBitTestData.nMinusBitPosition;
			fDifferencePercentage	= m_ImageProc.m_LineBitTestData.fMinusDifferencePercentage;

			switch(m_ImageProc.m_LineBitTestData.nPlusColorType)
			{
				case 0 : szColor = "B"; break;
				case 1 : szColor = "G"; break;
				case 2 : szColor = "R"; break;
			}
		}

		szMsg.Format("No of Faulty Lines : %d[%s%d(%.1f%%)], ",
				  	  m_ImageProc.m_LineBitTestData.nTotalNoErrorBitLine,
					  szColor,
					  nBitPosition,
					  fDifferencePercentage);
	
		AddStringToResultGrid(szMsg);

		/////////////////////////////////////////////////////////
		nBitPosition = 0;
		nMaxDifference = 0;
		fDifferencePercentage = 0;
		szColor = _T("");

		if(m_ImageProc.m_LineBitTestData.fPlusLowBitPairDifferencePercentage >= (m_ImageProc.m_LineBitTestData.fMinusLowBitPairDifferencePercentage*(-1)))
		{
			nMaxDifference			= m_ImageProc.m_LineBitTestData.nPlusMaxNoLowBitPair;
			nBitPosition			= m_ImageProc.m_LineBitTestData.nPlusLowBitPairBitPosition;
			fDifferencePercentage	= m_ImageProc.m_LineBitTestData.fPlusLowBitPairDifferencePercentage;

			switch(m_ImageProc.m_LineBitTestData.nPlusLowBitPairColorType)
			{
				case 0 : szColor = "B"; break;
				case 1 : szColor = "G"; break;
				case 2 : szColor = "R"; break;
			}
		}
		else
		{
			nMaxDifference			= m_ImageProc.m_LineBitTestData.nMinusMaxNoLowBitPair;
			nBitPosition			= m_ImageProc.m_LineBitTestData.nMinusLowBitPairBitPosition;
			fDifferencePercentage	= m_ImageProc.m_LineBitTestData.fMinusLowBitPairDifferencePercentage;

			switch(m_ImageProc.m_LineBitTestData.nPlusLowBitPairColorType)
			{
				case 0 : szColor = "B"; break;
				case 1 : szColor = "G"; break;
				case 2 : szColor = "R"; break;
			}
		}

		szMsg.Format("No of 0-Pair Lines : %d[%s%d-%s%d(%.1f%%)], ",
				  	  m_ImageProc.m_LineBitTestData.nTotalNoErrorLowBitLine,
					  szColor, nBitPosition,
					  szColor, nBitPosition+1,
					  fDifferencePercentage);

		AddStringToResultGrid(szMsg);

		/////////////////////////////////////////////////////////
		nBitPosition = 0;
		szColor	 = _T("");;
		nMaxDifference = 0;
		fDifferencePercentage = 0;

		if(m_ImageProc.m_LineBitTestData.fPlusHighBitPairDifferencePercentage >= (m_ImageProc.m_LineBitTestData.fMinusHighBitPairDifferencePercentage*(-1)))
		{
			nMaxDifference			= m_ImageProc.m_LineBitTestData.nMaxNoPlusHighBitPair;
			nBitPosition			= m_ImageProc.m_LineBitTestData.nPlusHighBitPairBitPosition;
			fDifferencePercentage	= m_ImageProc.m_LineBitTestData.fPlusHighBitPairDifferencePercentage;

			switch(m_ImageProc.m_LineBitTestData.nPlusHighBitPairColorType)
			{
				case 0 : szColor = "B"; break;
				case 1 : szColor = "G"; break;
				case 2 : szColor = "R"; break;
			}
		}
		else
		{
			nMaxDifference			= m_ImageProc.m_LineBitTestData.nMaxNoMinusHighBitPair;
			nBitPosition			= m_ImageProc.m_LineBitTestData.nMinusHighBitPairBitPosition;
			fDifferencePercentage	= m_ImageProc.m_LineBitTestData.fMinusHighBitPairDifferencePercentage;

			switch(m_ImageProc.m_LineBitTestData.nPlusHighBitPairColorType)
			{
				case 0 : szColor = "B"; break;
				case 1 : szColor = "G"; break;
				case 2 : szColor = "R"; break;
			}
		}

		szMsg.Format("No of 1-Pair Lines : %d[%s%d-%s%d(%.1f%%)],",
				  	  m_ImageProc.m_LineBitTestData.nTotalNoErrorHighBitLine,
					  szColor, nBitPosition,
					  szColor, nBitPosition+1,
					  fDifferencePercentage);

		/////////////////////////////////////////////////////////

//		szMsg = szMsg1 + szMsg2 + szMsg3;
	
		AddStringToResultGrid(szMsg);
	}
*/
	// Display Max variation positions and values
/*	nMaxVariation = nPlusMaxDifference - nMinusMaxDifference;

	szMsg.Format("        Max Level Variation : %d => %d(%d,%d), %d(%d,%d)",nMaxVariation,
				 nPlusMaxDifference,ptPlusResultPosition.x,ptPlusResultPosition.y,
				 nMinusMaxDifference,ptMinusResultPosition.x,ptMinusResultPosition.y );

	AddStringToResultGrid(szMsg);
*/
/*
	// High Bits
	szMsg.Format("[No High Bits]");
	AddStringToResultGrid(szMsg);

	for(int i=2; i>=0; i--)
	{
		switch(i)
		{
			case 0: // B
			{
				szMsg.Format("(R) B : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][10],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][11]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) B : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][10],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][11]);	

				AddStringToResultGrid(szMsg);
			}
			break;

			case 1: // G
			{
				szMsg.Format("(R) G : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][10],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][11]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) G : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][10],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][11]);	

				AddStringToResultGrid(szMsg);
			}
			break;
			
			case 2: // R
			{
				szMsg.Format("(R) R : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][10],m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[i][11]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) R : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][10],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[i][11]);	

				AddStringToResultGrid(szMsg);
			}
			break;
		}
	}

	// Low Bits Pair
	szMsg.Format("[Low Bit Pairs]");
	AddStringToResultGrid(szMsg);

	for(int i=2; i>=0; i--)
	{
		switch(i)
		{
			case 0: // B
			{
				szMsg.Format("(R) B : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][10]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) B : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][10]);	

				AddStringToResultGrid(szMsg);
			}
			break;

			case 1: // G
			{
				szMsg.Format("(R) G : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][10]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) G : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][10]);	

				AddStringToResultGrid(szMsg);
			}
			break;
			
			case 2: // R
			{
				szMsg.Format("(R) R : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[i][10]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) R : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[i][10]);	

				AddStringToResultGrid(szMsg);
			}
			break;
		}
	}

	// High Bits Pair
	szMsg.Format("[High Bit Pairs]");
	AddStringToResultGrid(szMsg);

	for(int i=2; i>=0; i--)
	{
		switch(i)
		{
			case 0: // B
			{
				szMsg.Format("(R) B : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][10]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) B : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][10]);	

				AddStringToResultGrid(szMsg);
			}
			break;

			case 1: // G
			{
				szMsg.Format("(R) G : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][10]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) G : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][10]);	

				AddStringToResultGrid(szMsg);
			}
			break;
			
			case 2: // R
			{
				szMsg.Format("(R) R : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[i][10]);	
			
				AddStringToResultGrid(szMsg);
						
				szMsg.Format("(G) R : %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d %6d",
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][0],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][1],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][2],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][3],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][4],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][5],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][6],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][7],	
					m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][8],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][9],m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[i][10]);	

				AddStringToResultGrid(szMsg);
			}
			break;
		}
	}
*/
	int nLeft=0, nTop=0, nWidth=0, nHeight=0;

	if(m_pSignalTestResultDlg == NULL)
	{

		// Modeless Dialog
		m_pSignalTestResultDlg = new CSignalTestResultDlg(this);

		m_pSignalTestResultDlg->m_pRefNoHighBit			= &m_ImageProc.m_LineBitTestData.m_aRefNoHighBit[0][0];
		m_pSignalTestResultDlg->m_pGrabNoHighBit		= &m_ImageProc.m_LineBitTestData.m_aGrabNoHighBit[0][0];
//		m_pSignalTestResultDlg->m_pRefNoHighBitPair		= &m_ImageProc.m_LineBitTestData.m_aRefNoHighBitPair[0][0];
//		m_pSignalTestResultDlg->m_pGrabNoHighBitPair	= &m_ImageProc.m_LineBitTestData.m_aGrabNoHighBitPair[0][0];
//		m_pSignalTestResultDlg->m_pRefNoLowBitPair		= &m_ImageProc.m_LineBitTestData.m_aRefNoLowBitPair[0][0];
//		m_pSignalTestResultDlg->m_pGrabNoLowBitPair		= &m_ImageProc.m_LineBitTestData.m_aGrabNoLowBitPair[0][0];

		m_pSignalTestResultDlg->m_nTotalNoMaskedPixel		= m_ImageProc.m_LineBitTestData.m_nTotalNoMaskedPixel;
		m_pSignalTestResultDlg->m_nTotalNoUnmaskedPixel		= m_ImageProc.m_LineBitTestData.m_nTotalNoUnmaskedPixel;
		m_pSignalTestResultDlg->m_nBitCount					= m_ImageProc.m_LineBitTestData.m_nBitCount;

		m_pSignalTestResultDlg->Create(IDD_SIGNAL_LINE_TEST_RESULT_DLG);
	}

	m_pSignalTestResultDlg->m_nTotalNoMaskedPixel		= m_ImageProc.m_LineBitTestData.m_nTotalNoMaskedPixel;
	m_pSignalTestResultDlg->m_nTotalNoUnmaskedPixel		= m_ImageProc.m_LineBitTestData.m_nTotalNoUnmaskedPixel;
	m_pSignalTestResultDlg->m_nBitCount					= m_ImageProc.m_LineBitTestData.m_nBitCount;


	nLeft   = (m_MWnd->m_PageRect.right)  - (m_pSignalTestResultDlg->m_dlgRect.Width());
	nTop    = (m_MWnd->m_PageRect.bottom) - (m_pSignalTestResultDlg->m_dlgRect.Height());
	nWidth  = m_pSignalTestResultDlg->m_dlgRect.Width();
	nHeight = m_pSignalTestResultDlg->m_dlgRect.Height();


	m_pSignalTestResultDlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
	m_pSignalTestResultDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
	m_pSignalTestResultDlg->ShowWindow(SW_SHOW);
	m_pSignalTestResultDlg->UpdateWindow();
	m_pSignalTestResultDlg->UpdateGrid();

	m_bZoomAllReset = FALSE;

	m_MWnd->Invalidate(FALSE);	
}

//+2007.05.25 Add BY USY
void CParmAdjPage::InitResultGrid()
{
	m_ctrlResultList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);//
	m_ctrlResultList.InsertColumn(0, "RESULT", LVCFMT_CENTER, 100);
	//m_ctrlResultGrid.SetRedraw(FALSE);
	//m_ctrlResultGrid.SetAllowBigSelection(FALSE);
	//m_ctrlResultGrid.SetExtendLastCol(TRUE);

	//m_ctrlResultGrid.SetBackColorBkg(COLOR_WHITE);
	//m_ctrlResultGrid.SetForeColorFixed(RGB(51,102,153));
	//m_ctrlResultGrid.SetBackColor(RGB(247,247,231));
	//m_ctrlResultGrid.SetGridColor(RGB(210,216,176));

	////Fixed Rows & Cols
	//m_ctrlResultGrid.SetFixedRows(0);
	//m_ctrlResultGrid.SetFixedCols(0);

	////Grid Line Setting
	//m_ctrlResultGrid.SetGridLines(flexGridFlat);

	////Column Width
	//m_ctrlResultGrid.SetColWidth(0, 1000);

	////Alignment
	//m_ctrlResultGrid.SetCellAlignment(flexAlignCenterCenter);

	////Rows & Cols Number
	//m_ctrlResultGrid.SetCols(1);
	//m_ctrlResultGrid.SetRows(0);
	//
	//m_ctrlResultGrid.SetRedraw(TRUE);
	//m_ctrlResultGrid.Refresh();
}

void CParmAdjPage::AddStringToResultGrid(int nRow, int nColumn,CString s)
{
/*	int nRow = m_ctrlResultGrid.GetRows();

	m_ctrlResultGrid.SetRedraw(FALSE);
	m_ctrlResultGrid.SetRows(nRow + 1);
	m_ctrlResultGrid.SetTextMatrix(nRow , nColumn, s);
	m_ctrlResultGrid.SetTopRow(nRow);
	m_ctrlResultGrid.SetRedraw(TRUE);
	m_ctrlResultGrid.Refresh();
*/
}

void CParmAdjPage::AddStringToResultGrid(CString s)
{
	int nRow = m_ctrlResultList.GetItemCount();
	m_ctrlResultList.SetItem(nRow, 0,  LVIF_TEXT, s, NULL, NULL, NULL, NULL);// (nRow, 0, s);
	//int nRow = m_ctrlResultGrid.GetRows();

	//m_ctrlResultGrid.SetRedraw(FALSE);
	//m_ctrlResultGrid.SetRows(nRow + 1);
	//m_ctrlResultGrid.SetTextMatrix(nRow , 0, s);
	//m_ctrlResultGrid.SetTopRow(nRow);
	//m_ctrlResultGrid.SetRedraw(TRUE);
	//m_ctrlResultGrid.Refresh();
}

void CParmAdjPage::OnBtnParmLabel() 
{
	int nResult = TEST_FAIL;

	CString szMsg = _T("");
	CString szLabelImagePath = _T("");
	CString szLabelImageFile = _T("");

	nResult = m_ImageProc.Labeling(m_Image[*m_nImgType], m_Image[I_MASK], m_Image[I_PROC],*m_ctrlProgress);

	if(nResult == TEST_ABORT) return;

	if(nResult == TEST_PASS)
	{
		szMsg.Format("[Labeling]-PASS!");
	
		AddStringToResultGrid(szMsg);
		
	//	szLabelImageFile = m_strRefFileName.Left(m_strRefFileName.ReverseFind('.')) + "_Label.bmp";

	//	szLabelImagePath.Format("%s\\%s",m_strRefFolder,szLabelImageFile);
		
	//	m_Image[I_PROC].SaveImage(szLabelImagePath,PROC_IMAGE,*m_ctrlProgress);
	}
	else
	{
		szMsg.Format("[Labeling]-FAIL!");

		AddStringToResultGrid(szMsg);
	}

	m_MWnd->SendMessage(UM_UPDATE_ALL_VIEW_TAB);
	m_MWnd->SendMessage(UM_CHANGE_ACTIVE_VIEW_TAB,I_PROC);
}

void CParmAdjPage::OnBtnParmRepaint() 
{
	int nResult = TEST_FAIL;

	CString szMsg = _T("");
	CString szLabelImagePath = _T("");
	CString szLabelImageFile = _T("");

	nResult = m_ImageProc.Repaint(m_Image[*m_nImgType], m_Image[I_MASK], m_Image[I_PROC],*m_ctrlProgress);

	if(nResult == TEST_ABORT) return;

	if(nResult == TEST_PASS)
	{
		szMsg.Format("[Repaint]-PASS!");
	
		AddStringToResultGrid(szMsg);
	}
	else
	{
		szMsg.Format("[Repaint]-FAIL!");

		AddStringToResultGrid(szMsg);
	}

	m_MWnd->SendMessage(UM_UPDATE_ALL_VIEW_TAB);
	m_MWnd->SendMessage(UM_CHANGE_ACTIVE_VIEW_TAB,I_PROC);
}

void CParmAdjPage::OnBtnParmZoomOut() 
{
/*	CString szMsg = _T("");

	m_ImageProc.ZoomOut(m_Image[*m_nImgType], 0.4f);

	m_MWnd->SendMessage(UM_UPDATE_ALL_VIEW_TAB);
	m_MWnd->SendMessage(UM_CHANGE_ACTIVE_VIEW_TAB,I_PROC);
*/
	int	nResult2;
	nResult2 = m_ImageProc.CheckImageDiff(m_Image[0], m_Image[1], -20, 20); 

}


void CParmAdjPage::OnBtnParmNoiseTest() 
{
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_X);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_POSITION_SHIFT_Y);

	CEdit* pEdit4 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_COLOR_MARGIN_HIGHBITS);
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_X);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_Y);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_WIDTH);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_HEIGHT);

	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_LOWER_LIMIT);
	CEdit* pEdit10 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_UPPER_LIMIT);


	CButton* pCheck1 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_LEVEL); 
	CButton* pCheck2 = (CButton*)GetDlgItem(IDC_CHK_PARM_COLOR_MARGIN_HIGH_BITS);
	CButton* pCheck3 = (CButton*)GetDlgItem(IDC_CHK_PARM_USE_MASK_IMAGE);

	CString sTmp = _T("");

	int nXPosShift, nYPosShift, nLevel, nHighbits,nRoiX1,nRoiY1,nRoiX2,nRoiY2;
	float fLowerLimit, fUpperLimit;
	
	pEdit1->GetWindowText(sTmp); nXPosShift = atoi(sTmp);
	pEdit2->GetWindowText(sTmp); nYPosShift = atoi(sTmp);

	UpdateData(TRUE);

	//+change kwmoon 080122
	if(m_nRegionType == ROI_COORDINATES)
	{
		pEdit5->GetWindowText(sTmp); nRoiX1 = atoi(sTmp);
		pEdit6->GetWindowText(sTmp); nRoiY1 = atoi(sTmp);
		pEdit7->GetWindowText(sTmp); nRoiX2 = atoi(sTmp);
		pEdit8->GetWindowText(sTmp); nRoiY2 = atoi(sTmp);
	}
	else
	{
		nRoiX1	= 0;
		nRoiY1	= 0;
		nRoiX2  = 0;
		nRoiY2	= 0;
	}
	//-

	//+change kwmoon 070929
	pEdit9->GetWindowText(sTmp); fLowerLimit = (float)atof(sTmp);
	pEdit10->GetWindowText(sTmp); fUpperLimit = (float)atof(sTmp);


	int nResult = TEST_FAIL;
	int nMaxDifference = 0;
	int nMaxVariation	= 0;
	int nPlusMaxDifference = 0;
	int nMinusMaxDifference = 0;
	int nTotalNoErrorPixel = 0;

	// Variable for Position Shift
	POINT ptPositionShift;
	ptPositionShift.x = nXPosShift;
	ptPositionShift.y = nYPosShift;

	// Variable for Max Difference Position
	POINT ptResultPosition;
	ptResultPosition.x = 0;
	ptResultPosition.y = 0;

	POINT ptPlusResultPosition;
	ptPlusResultPosition.x = 0;
	ptPlusResultPosition.y = 0;

	POINT ptMinusResultPosition;
	ptMinusResultPosition.x = 0;
	ptMinusResultPosition.y = 0;

	POINT ptRoi1Position;
	ptRoi1Position.x = nRoiX1;
	ptRoi1Position.y = nRoiY1;

	POINT ptRoi2Position;
	ptRoi2Position.x = nRoiX2;
	ptRoi2Position.y = nRoiY2;

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

	CString szMsg;

	m_ImageProc.SetPositionShift(ptPositionShift);
	m_ImageProc.SetRoi(ptRoi1Position,ptRoi2Position);

	if(pCheck3->GetCheck() == 1) // Use Mask Image
	{
		m_bUseMaskImage = TRUE;
	}


	if(pCheck1->GetCheck() == 1) // Use Margin
	{
		//+add kwmoon 070205[#1]
		nHighbits = 0;

		//+change kwmoon 070929
		pEdit9->GetWindowText(sTmp); fLowerLimit = (float)atof(sTmp);
		pEdit10->GetWindowText(sTmp); fUpperLimit = (float)atof(sTmp);

		pEdit4->SetWindowText("0");
	}
	else // Use High Bits
	{
		//+add kwmoon 070205[#1]
		nLevel = 0;
		pEdit4->GetWindowText(sTmp); nHighbits = atoi(sTmp);
	}

	m_ImageProc.SetColorLimit(fLowerLimit,fUpperLimit);
	m_ImageProc.SetUsedNoBrightHighBits(nHighbits);
	m_ImageProc.SetUsedNoColorHighBits(nHighbits);

	nResult = m_ImageProc.NoiseTest(NORMAL_IMAGE_TEST,m_Image[I_GRAB], m_Image[I_REF],m_Image[I_MASK],
								  ptPlusResultPosition,ptMinusResultPosition, 
								  nPlusMaxDifference,nMinusMaxDifference,nTotalNoErrorPixel,m_bUseMaskImage,*m_ctrlProgress);

	m_ImageProc.SetAvgMaskSize(0,0,0); // Initialize
	m_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	if(nResult == TEST_ABORT) return;

	if(m_Image[I_GRAB].m_bProcessedImageLoaded)
	{
		m_ImageProc.ZoomOut(m_Image[I_PROC],0.5f);

		for(int i=0; i<(int)m_Image[I_GRAB].m_InfoHeader.biSizeImage; i++)
		{
			m_Image[I_PROC].m_pImageData[i]	= m_Image[I_GRAB].m_pProcessedImageData[i];
		}
	}
	
	// Display Max Difference in an absolute value
	if(nPlusMaxDifference >= abs(nMinusMaxDifference))
	{
		nMaxDifference		= nPlusMaxDifference;
		ptResultPosition.x  = ptPlusResultPosition.x;
		ptResultPosition.y  = ptPlusResultPosition.y;
	}
	else
	{
		nMaxDifference		= abs(nMinusMaxDifference);
		ptResultPosition.x  = ptMinusResultPosition.x;
		ptResultPosition.y  = ptMinusResultPosition.y;
	}

	if(nResult == TEST_PASS)
	{
		szMsg.Format("[Noise Test]-PASS!");
	}
	else
	{
		szMsg.Format("[Noise Test]-FAIL!");
	}
	AddStringToResultGrid(szMsg);

	if(nResult == TEST_PASS)
	{
		szMsg.Format("        Max Level Difference : %d",nMaxDifference);
	}
	else
	{
		szMsg.Format("        Max Level Difference : %d (%d,%d),(%d Pixel)",
							  nMaxDifference,ptResultPosition.x,ptResultPosition.y,nTotalNoErrorPixel);
	}
	AddStringToResultGrid(szMsg);

	// Display Max variation positions and values
	nMaxVariation = nPlusMaxDifference - nMinusMaxDifference;

	szMsg.Format("        Max Level Variation : %d => %d(%d,%d), %d(%d,%d)",nMaxVariation,
				 nPlusMaxDifference,ptPlusResultPosition.x,ptPlusResultPosition.y,
				 nMinusMaxDifference,ptMinusResultPosition.x,ptMinusResultPosition.y );

	AddStringToResultGrid(szMsg);

	m_bZoomAllReset = FALSE;

	m_MWnd->SendMessage(UM_UPDATE_ALL_VIEW_TAB);
	m_MWnd->InitScrollBar(P_PARM, I_PROC);
	m_MWnd->Invalidate(FALSE);	
}

//+add kwmoon 080122
void CParmAdjPage::OnRadioOsdRegion() 
{
	// TODO: Add your control notification handler code here
	m_nRegionType = ROI_COORDINATES;
}

//+add kwmoon 080122
void CParmAdjPage::OnRadioRoi() 
{
	// TODO: Add your control notification handler code here
	m_nRegionType = OSD_REGION_COORDINATES;
}

//+add kwmoon 080228
void CParmAdjPage::OnBtnParmMovingPicTest() 
{
	int nResult = TEST_FAIL;

	CString sTmp  = _T("");
	CEdit* pEdit5 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_X);
	CEdit* pEdit6 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_Y);
	CEdit* pEdit7 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_WIDTH);
	CEdit* pEdit8 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_ROI_HEIGHT);

	//+add 090218(Modification No1)
	CEdit* pEdit9 = (CEdit*)GetDlgItem(IDC_EDIT_PARM_NO_COLOR);
	
	
	UpdateData(TRUE);

	int	nRoiX1	= 0;
	int	nRoiY1	= 0;
	int	nRoiX2  = 0;
	int	nRoiY2	= 0;

	//+add 090218(Modification No1)
	int nNoMpicColor = 0;
	int nMpicColorThreshold = 0;

	POINT ptStartPosition;
	POINT ptEndPosition;

	if(!m_Image[I_GRAB].m_bImageLoaded)
	{
		AfxMessageBox("Grab Image is not loaded!"); return;
	}

	if(m_nRegionType == OSD_REGION_COORDINATES)
	{
		pEdit5->GetWindowText(sTmp); nRoiX1 = atoi(sTmp);
		pEdit6->GetWindowText(sTmp); nRoiY1 = atoi(sTmp);
		pEdit7->GetWindowText(sTmp); nRoiX2 = atoi(sTmp);
		pEdit8->GetWindowText(sTmp); nRoiY2 = atoi(sTmp);
	}
	else
	{
		nRoiX1	= 0;
		nRoiY1	= 0;
		nRoiX2  = 0;
		nRoiY2	= 0;
	}

	//+add 090218(Modification No1)
	pEdit9->GetWindowText(sTmp); nMpicColorThreshold = atoi(sTmp);

	ptStartPosition.x = nRoiX1;
	ptStartPosition.y = nRoiY1;
	ptEndPosition.x   = nRoiX2;
	ptEndPosition.y   = nRoiY2; 

	m_ImageProc.SetOsdRegion(ptStartPosition,ptEndPosition);
	
	CString szMsg = _T("");

	int nImageWidth  = m_Image[I_GRAB].m_InfoHeader.biWidth;
	int nImageHeight = m_Image[I_GRAB].m_InfoHeader.biHeight;
	int nNoPixel	 = nImageWidth * nImageHeight - (m_ImageProc.m_nOsdWidth * m_ImageProc.m_nOsdHeight);

	//+del 090218(Modification No1)
/*	float nPercentagePixelInMiddleRange[3];
	float nPercentageNonBlackPixel[3];

	for(int i=0; i<3; i++)
	{
		m_ImageProc.m_nNoPixelInMiddleRange[i]	= 0;
		m_ImageProc.m_nNoNonBlackPixel[i]		= 0;

		nPercentagePixelInMiddleRange[i] = 0;
		nPercentageNonBlackPixel[i]		 = 0;
	}
*/
	//+change 090218(Modification No1)
//	nResult = m_ImageProc.NoVideoSignalTest(m_Image[I_GRAB], *m_ctrlProgress);
	nResult = m_ImageProc.NoVideoSignalTest2(m_Image[I_GRAB],nMpicColorThreshold, &nNoMpicColor);
			
	if(nResult == TEST_PASS)
	{
		szMsg.Format("[MovingPic Test]-PASS!");
	}
	else
	{
		szMsg.Format("[MovingPic Test]-FAIL!");
	}
	AddStringToResultGrid(szMsg);

	//+del 090218(Modification No1)
/*	nPercentagePixelInMiddleRange[R_DATA] = ((float)(m_ImageProc.m_nNoPixelInMiddleRange[R_DATA]*100)/(float)nNoPixel);
	nPercentagePixelInMiddleRange[G_DATA] = ((float)(m_ImageProc.m_nNoPixelInMiddleRange[G_DATA]*100)/(float)nNoPixel);
	nPercentagePixelInMiddleRange[B_DATA] = ((float)(m_ImageProc.m_nNoPixelInMiddleRange[B_DATA]*100)/(float)nNoPixel);
	
	nPercentageNonBlackPixel[R_DATA] = ((float)(m_ImageProc.m_nNoNonBlackPixel[R_DATA]*100)/(float)nNoPixel);
	nPercentageNonBlackPixel[G_DATA] = ((float)(m_ImageProc.m_nNoNonBlackPixel[G_DATA]*100)/(float)nNoPixel);
	nPercentageNonBlackPixel[B_DATA] = ((float)(m_ImageProc.m_nNoNonBlackPixel[B_DATA]*100)/(float)nNoPixel);

	szMsg.Format("No of Pixels in middle range(more than %d %%) <R:%.2f%%, G:%.2f%%, B:%.2f%%>",CurrentSet->nPixelInMiddleRange,
		nPercentagePixelInMiddleRange[R_DATA], nPercentagePixelInMiddleRange[G_DATA], nPercentagePixelInMiddleRange[B_DATA]);

	AddStringToResultGrid(szMsg);

	szMsg.Format("No of non-black Pixels(more than %d %%) <R:%.2f%%, G:%.2f%%, B:%.2f%%>",CurrentSet->nNonBlackPixel,
		nPercentageNonBlackPixel[R_DATA], nPercentageNonBlackPixel[G_DATA], nPercentageNonBlackPixel[B_DATA]);

	AddStringToResultGrid(szMsg);
*/
	//+add 090218(Modification No1)
	szMsg.Format("No of color used : %d (Threshold : %d)",nNoMpicColor, nMpicColorThreshold);
	AddStringToResultGrid(szMsg);

//	m_MWnd->SendMessage(UM_UPDATE_ALL_VIEW_TAB);
//	m_MWnd->InitScrollBar(P_PARM, I_PROC);
	m_MWnd->Invalidate(FALSE);	
}