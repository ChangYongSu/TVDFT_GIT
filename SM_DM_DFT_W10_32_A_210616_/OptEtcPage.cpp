// OptEtcPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptEtcPage.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

extern _InputSource	InputDefined[];

extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// COptEtcPage property page

IMPLEMENT_DYNCREATE(COptEtcPage, CPropertyPage)

COptEtcPage::COptEtcPage() : CPropertyPage(COptEtcPage::IDD)
{
	//{{AFX_DATA_INIT(COptEtcPage)
	m_bSaveProcImg = TRUE; // FALSE;
	m_strViewerPath = _T("");
	m_bResultSaveCsv = FALSE;
	m_bResultSaveHtml = TRUE;
	m_bResultSaveText = FALSE;
	m_nDisplayType = 0;
	m_nNoRepeatExecution = 1;
	m_fAllowedErrorPixelPercentage = 0.0;
	m_nNoFrameForRefImageCreation = 1;
	m_nNoRetry = 0;
	m_nFreqMargin = 15;
	m_nDelayPercentage = 100;
	m_bSaveDistributionTable = FALSE;
	m_bSaveIntermediateImage = FALSE;
	m_bSaveCaptionTestImg = FALSE;
	m_bSaveMovingPicTestImg = FALSE;
	m_fAllowedErrorPixelPercentageForOsdTest = 0.0;
	m_nNonBlackPixel = 30;
	m_nPixelInMiddleRange = 85;
	m_bSaveGrabImg = 0;
	m_bSaveProcessingTimeData = FALSE;
	m_fAllowedBand1ErrorPixelPercentage = 0.0f;
	m_fAllowedBand2ErrorPixelPercentage = 0.0f;
	m_fAllowedBand3ErrorPixelPercentage = 0.0f;
	m_fAllowedBand4ErrorPixelPercentage = 0.0f;
	m_fAllowedBand5ErrorPixelPercentage = 0.0f;
	m_bSaveReviewData = FALSE;
	m_bSaveDetailResultData = FALSE;
	m_nLevelMargin = 45;
	m_nNoUserStop = 0;
	m_nNoTestedBoard = 0;
	m_nNoFailedBoard = 0;
	m_nNoUsedColors = 5000;
	m_bRelayOnOff = FALSE;
	m_bFixtureIdCheck = FALSE;
	//}}AFX_DATA_INIT
}

COptEtcPage::~COptEtcPage()
{
}

void COptEtcPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptEtcPage)
	DDX_Control(pDX, IDC_SPIN_AUDIO_LEVEL_SPEC_MARGIN, m_ctrlAudioLevelSpecMargin);
	DDX_Control(pDX, IDC_SPIN_NONBLACK_PIXEL, m_ctrlNonBlackPixel);
	DDX_Control(pDX, IDC_SPIN_PIXEL_IN_MIDDLE_RANGE, m_ctrlPixelInMiddleRange);
	DDX_Control(pDX, IDC_SPIN_NO_ALLOWED_ERROR_PIXEL_OSDTEST, m_ctrlNoAllowedErrorPixelForOsdTest);
	DDX_Control(pDX, IDC_SPIN_DELAY, m_ctrlDelayPercentage);
	DDX_Control(pDX, IDC_SPIN_AUDIO_FREQ_SPEC_MARGIN, m_ctrlAudioFreqSpecMargin);
	DDX_Control(pDX, IDC_SPIN_RETRY, m_ctrlNoRetry);
	DDX_Control(pDX, IDC_SPIN_FRAME, m_ctrlNoFrame);
	DDX_Control(pDX, IDC_SPIN_NO_ALLOWED_ERROR_PIXEL, m_ctrlNoAllowedErrorPixel);
	DDX_Control(pDX, IDC_SPIN_REPEAT, m_ctrlNoRepeatExecution);
	DDX_Check(pDX, IDC_CHK_SAVE_PROCESSED_IMAGE, m_bSaveProcImg);
	DDX_Text(pDX, IDC_EDIT_VIEWER_PATH, m_strViewerPath);
	DDX_Check(pDX, IDC_CHK_RESULT_SAVE_CSV, m_bResultSaveCsv);
	DDX_Check(pDX, IDC_CHK_RESULT_SAVE_HTML, m_bResultSaveHtml);
	DDX_Check(pDX, IDC_CHK_RESULT_SAVE_TEXT, m_bResultSaveText);
	DDX_Radio(pDX, IDC_RDO_DISPLAY_TYPE1, m_nDisplayType);
	DDX_Text(pDX, IDC_EDIT_NO_REPEAT, m_nNoRepeatExecution);
	DDV_MinMaxUInt(pDX, m_nNoRepeatExecution, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_NO_ALLOWED_ERROR_PIXELS, m_fAllowedErrorPixelPercentage);
	DDV_MinMaxFloat(pDX, m_fAllowedErrorPixelPercentage, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_NO_FRAME, m_nNoFrameForRefImageCreation);
	DDV_MinMaxUInt(pDX, m_nNoFrameForRefImageCreation, 1, 10);
	//DDX_Control(pDX, IDC_SPEC_ADJ_GRID, m_ctrlSpecAdjGrid);
	DDX_Text(pDX, IDC_EDIT_NO_RETRY, m_nNoRetry);
	DDV_MinMaxUInt(pDX, m_nNoRetry, 0, 100);
	DDX_Text(pDX, IDC_EDIT_AUDIO_SPEC_MARGIN, m_nFreqMargin);
	DDV_MinMaxUInt(pDX, m_nFreqMargin, 0, 100);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nDelayPercentage);
	DDV_MinMaxUInt(pDX, m_nDelayPercentage, 1, 100);
	DDX_Check(pDX, IDC_CHK_SAVE_DISTRIBUTION_TABLE, m_bSaveDistributionTable);
	DDX_Check(pDX, IDC_CHK_SAVE_INTERMEDIATE_IMAGE, m_bSaveIntermediateImage);
	DDX_Check(pDX, IDC_CHK_SAVE_CAPTION_TEST_IMAGE, m_bSaveCaptionTestImg);
	DDX_Check(pDX, IDC_CHK_SAVE_MOVING_PIC_TEST_IMAGE, m_bSaveMovingPicTestImg);
	DDX_Text(pDX, IDC_EDIT_NO_ALLOWED_ERROR_PIXELS_OSDTEST, m_fAllowedErrorPixelPercentageForOsdTest);
	DDV_MinMaxFloat(pDX, m_fAllowedErrorPixelPercentageForOsdTest, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_NONBLACK_PIXEL, m_nNonBlackPixel);
	DDV_MinMaxUInt(pDX, m_nNonBlackPixel, 1, 100);
	DDX_Text(pDX, IDC_EDIT_PIXEL_IN_MIDDLE_RANGE, m_nPixelInMiddleRange);
	DDV_MinMaxUInt(pDX, m_nPixelInMiddleRange, 1, 100);
	DDX_Radio(pDX, IDC_RADIO_SAVE_NG_IMAGE, m_bSaveGrabImg);
	DDX_Check(pDX, IDC_CHK_SAVE_PROCESSING_TIME_DATA, m_bSaveProcessingTimeData);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_1, m_fAllowedBand1ErrorPixelPercentage);
	DDV_MinMaxFloat(pDX, m_fAllowedBand1ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_2, m_fAllowedBand2ErrorPixelPercentage);
	DDV_MinMaxFloat(pDX, m_fAllowedBand2ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_3, m_fAllowedBand3ErrorPixelPercentage);
	DDV_MinMaxFloat(pDX, m_fAllowedBand3ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_4, m_fAllowedBand4ErrorPixelPercentage);
	DDV_MinMaxFloat(pDX, m_fAllowedBand4ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_5, m_fAllowedBand5ErrorPixelPercentage);
	DDV_MinMaxFloat(pDX, m_fAllowedBand5ErrorPixelPercentage, 0.f, 10.f);
	DDX_Check(pDX, IDC_CHK_SAVE_REVIEW_DATA, m_bSaveReviewData);
	DDX_Check(pDX, IDC_CHK_SAVE_DETAIL_RESULT_DATA, m_bSaveDetailResultData);
	DDX_Text(pDX, IDC_EDIT_LEVEL_MARGIN, m_nLevelMargin);
	DDV_MinMaxInt(pDX, m_nLevelMargin, 0, 100);
	DDX_Text(pDX, IDC_EDIT_NO_USER_STOP, m_nNoUserStop);
	DDX_Text(pDX, IDC_EDIT_NO_TESTED_BOARD, m_nNoTestedBoard);
	DDX_Text(pDX, IDC_EDIT_NO_FAILED_BOARD, m_nNoFailedBoard);
	DDX_Text(pDX, IDC_EDIT_NO_COLORS_USED, m_nNoUsedColors);
	DDX_Check(pDX, IDC_CHK_RELAY_CONROL, m_bRelayOnOff);
	DDX_Check(pDX, IDC_CHK_FIXTURE_CHECK, m_bFixtureIdCheck);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_SPEC_ADJ_LIST, m_ctrlSpecAdjList);
	DDX_Control(pDX, IDC_COMBO_MARGIN2, m_cComboMargin);
	DDX_Control(pDX, IDC_COMBO_SOUND_L, m_cComboSoundLeft);
}


BEGIN_MESSAGE_MAP(COptEtcPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptEtcPage)
	ON_BN_CLICKED(IDC_BTN_OPEN_VIEWER, OnBtnOpenViewer)
	ON_BN_CLICKED(IDC_BTN_ETC_OPT_APPLY, OnBtnEtcOptApply)
	ON_BN_CLICKED(IDC_BTN_ETC_OPT_CLEAR_TOTAL_COUNT, OnBtnEtcOptClearTotalCount)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_CHK_SAVE_CAPTION_TEST_IMAGE, OnChkSaveCaptionTestImage)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPEC_ADJ_LIST, &COptEtcPage::OnNMDblclkListSpecAdjList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SPEC_ADJ_LIST, &COptEtcPage::OnNMRClickListSpecAdjList)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPEC_ADJ_LIST, &COptEtcPage::OnNMClickListSpecAdjList)
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_NOTIFY(LVN_ENDSCROLL, IDC_LIST_SPEC_ADJ_LIST, &COptEtcPage::OnLvnEndScrollListSpecAdjList)
	ON_NOTIFY(NM_HOVER, IDC_LIST_SPEC_ADJ_LIST, &COptEtcPage::OnNMHoverListSpecAdjList)
	ON_NOTIFY(LVN_BEGINSCROLL, IDC_LIST_SPEC_ADJ_LIST, &COptEtcPage::OnLvnBeginScrollListSpecAdjList)
	ON_CBN_SELCHANGE(IDC_COMBO_MARGIN2, &COptEtcPage::OnCbnSelchangeComboMargin2)
	ON_CBN_SELCHANGE(IDC_COMBO_SOUND_L, &COptEtcPage::OnCbnSelchangeComboSoundL)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
//+add kwmoon 070930
void COptEtcPage::InitSpecAdjGrid()
{
	//
	CString sTmp;
	//CStringArray strData;
	//strData.RemoveAll();
	for(int nTmp = 0; nTmp <= 100; nTmp++)
	{
		sTmp.Format("        %d    ", nTmp);
		m_cComboMargin.AddString(sTmp);
		//strData.Add(sTmp);
	}
	
	
	m_ctrlSpecAdjList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );// 리스트 컨트롤 초기화: 열 추가
	m_ctrlSpecAdjList.InsertColumn(0, "Source", LVCFMT_CENTER, 80);
	m_ctrlSpecAdjList.InsertColumn(1, "Margin", LVCFMT_CENTER, 80);



	//+add kwmoon 080804
	int nNoSourceType = 0;

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		nNoSourceType++;
	}


	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		m_ctrlSpecAdjList.InsertItem(i, InputDefined[i].pszName);   // // m_ctrlSpecAdjList.InsertColumn(1, "Margin", LVCFMT_CENTER, 100); //m_ctrlSpecAdjGrid.SetTextMatrix(i + 1,0, InputDefined[i].pszName );
		//m_ctrlSpecAdjList.SetItem(i, 1, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); 
	}

	m_ctrlSpecAdjList.Invalidate();

}

//+ 2007.10.5 Add BY USY
void COptEtcPage::UpdateSpecAdjGrid()
{
	///m_ctrlSpecAdjGrid.SetRedraw(flexRDNone);
	//m_ctrlSpecAdjGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	CString sTmp;

	m_ctrlSpecAdjList.DeleteAllItems();

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		m_ctrlSpecAdjList.InsertItem(i, InputDefined[i].pszName);  //m_ctrlSpecAdjGrid.SetTextMatrix(i + 1,0, InputDefined[i].pszName );
		sTmp.Format("%d", m_nSrcMargin[i]);
		m_ctrlSpecAdjList.SetItem(i, 1, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);   // // m_ctrlSpecAdjGrid.SetTextMatrix(i + 1,1, sTmp);
	}

	//m_ctrlSpecAdjGrid.SetCol(-1);
	//m_ctrlSpecAdjGrid.SetRow(-1);
	//m_ctrlSpecAdjGrid.SetRedraw(-1);
	//m_ctrlSpecAdjGrid.Refresh();
	m_ctrlSpecAdjList.Invalidate();
}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COptEtcPage message handlers

BOOL COptEtcPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString sTmp;

	m_strViewerPath			 = CurrentSet->sViewerPath;
	m_bSaveGrabImg			 = CurrentSet->bSaveGrabImg;

	//+change kwmoon 080715
	m_bSaveProcImg			 = TRUE; // CurrentSet->bSaveProcImg;

	m_bSaveCaptionTestImg	 = CurrentSet->bSaveCaptionTestImg;
	m_bSaveMovingPicTestImg	 = CurrentSet->bSaveMovingPicTestImg;
	m_bSaveIntermediateImage = CurrentSet->bSaveIntermediateImage;

	//+del kwmoon 080716
//	m_bSaveDistributionTable = CurrentSet->bSaveDistributionTable;
	m_bSaveProcessingTimeData = CurrentSet->bSaveProcessingTimeData;

	//+add kwmoon 080716
	m_bSaveReviewData		= CurrentSet->bSaveReviewData;
	m_bSaveDetailResultData	= CurrentSet->bSaveDetailResultData;

//	m_nDisplayType			= DETAILEDGRID;
	m_nDisplayType			= CurrentSet->nUseUserRetry;
	m_bRelayOnOff			= CurrentSet->bVersionCheckOnly;

	//+change kwmoon 080715
	m_bResultSaveText		=  CurrentSet->bSaveResult2Txt;
	m_bResultSaveHtml		=  CurrentSet->bSaveResult2Html;
	m_bResultSaveCsv		=  CurrentSet->bSaveResult2Csv;
	//-
	
	m_nNoRepeatExecution	= CurrentSet->nNoRepeatExecution;

	//+del kwmoon 080715
//	m_fAllowedErrorPixelPercentage  = CurrentSet->nNoAllowedErrorPixel;
	m_nNoFrameForRefImageCreation = CurrentSet->nNoFrameForRefImageCreation;
	m_nNonBlackPixel  		= CurrentSet->nNonBlackPixel;
	m_nPixelInMiddleRange	= CurrentSet->nPixelInMiddleRange;
	m_nDelayPercentage		= CurrentSet->nDelayPercentage;
	m_nNoRetry				= CurrentSet->nNoRetry;

	//+add 090218(Modification No1)
	m_nNoUsedColors  		= CurrentSet->nNoUsedColors;

	//+del kwmoon 080715
//	m_fAllowedErrorPixelPercentageForOsdTest  = CurrentSet->nNoAllowedErrorPixelForOsdTest;

	//+del kwmoon 080125
	//+add kwmoon 071227
//	m_nSpecMargin			= CurrentSet->nSpecMargin;
	
	//+add kwmoon 080123
	m_nFreqMargin		= CurrentSet->nFreqMargin;

	m_ctrlNoRepeatExecution.SetRange(1,1000);
	//+del kwmoon 080715
//	m_ctrlNoAllowedErrorPixel.SetRange(0,10000);
	m_ctrlNoFrame.SetRange(1,10);

	m_ctrlNonBlackPixel.SetRange(1,100);
	m_ctrlPixelInMiddleRange.SetRange(1,100);

	//+del kwmoon 080715
//	m_ctrlNoAllowedErrorPixelForOsdTest.SetRange(0,10000);

	m_ctrlNoRetry.SetRange(0,100);
	m_ctrlDelayPercentage.SetRange(1,100);
	m_ctrlAudioFreqSpecMargin.SetRange(0,100);
	//+add kwmoon 080819
	m_ctrlAudioLevelSpecMargin.SetRange(0,100);

	//+del kwmoon 080125
	//+add kwmoon 071227
//	m_ctrlSpecMargin.SetRange(1,100);
	

	//+change kwmoon 080724
	//change PSh 080605
//	for(int nTmp = 0; nTmp < 22; nTmp++)
	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		m_nSrcMargin[i] = CurrentSet->nSrcMargin[i];
	}

	//+add kwmoon 080715
	m_fAllowedBand1ErrorPixelPercentage = CurrentSet->fAllowedBandErrorPixelPercentage[0];
	m_fAllowedBand2ErrorPixelPercentage = CurrentSet->fAllowedBandErrorPixelPercentage[1];
	m_fAllowedBand3ErrorPixelPercentage = CurrentSet->fAllowedBandErrorPixelPercentage[2];
	m_fAllowedBand4ErrorPixelPercentage = CurrentSet->fAllowedBandErrorPixelPercentage[3];
	m_fAllowedBand5ErrorPixelPercentage = CurrentSet->fAllowedBandErrorPixelPercentage[4];
	//-

	//+add kwmoon 080716
	m_nLevelMargin = CurrentSet->nLevelMargin;

	//+add kwmoon 080819
	m_nFreqMargin = CurrentSet->nFreqMargin;

	//+add kwmoon 070930
	InitSpecAdjGrid();
	UpdateSpecAdjGrid();


	//+add kwmoon 081004
	m_nNoTestedBoard  = g_nNoTested;
	m_nNoFailedBoard  = g_nFailed;
	m_nNoUserStop	  = g_nUserStop;

	m_bFixtureIdCheck = CurrentSet->bFixtureIdCheck;

	if (g_pView->m_sm_sound_ctrl.m_aStrSoundDevice.GetSize() > 0)
	{
		for (int i = 0; i < g_pView->m_sm_sound_ctrl.m_aStrSoundDevice.GetSize(); i++)
		{
			m_cComboSoundLeft.AddString(g_pView->m_sm_sound_ctrl.m_aStrSoundDevice.GetAt(i));
		}
	}
	if ((g_pView->m_sm_sound_ctrl.m_aStrSoundDevice.GetSize() > CurrentSet->nSoundInDeviceID)
		&& (0 <= CurrentSet->nSoundInDeviceID))
	{
		m_cComboSoundLeft.SetCurSel(CurrentSet->nSoundInDeviceID);

		//g_pView->m_sm_sound_ctrl.StartCapture(this, CurrentSet->nSoundInDeviceID);
	}
	else
	{
		if (g_pView->m_sm_sound_ctrl.m_aStrSoundDevice.GetSize() >= 2)
		{
			if (g_nRunningProcessNo == 1)
			{
				CurrentSet->nSoundInDeviceID = 1;
			}
			else
			{
				CurrentSet->nSoundInDeviceID = 0;
			}
			m_cComboSoundLeft.SetCurSel(CurrentSet->nSoundInDeviceID);

			//g_pView->m_sm_sound_ctrl.StartCapture(this, CurrentSet->nSoundInDeviceID);
		}
		else
		{
			m_cComboSoundLeft.SetCurSel(-1);
		//	sTmp.Format(_T("Sound Card ID(%d) Select Error"), CurrentSet->nSoundInDeviceID);
			MessageBox("Sound Card Select Error");
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void COptEtcPage::OnBtnOpenViewer() 
{
	CFileDialog dlg(TRUE, "exe", NULL, 
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"EXE File(*.exe)|*.exe|", NULL);
	if(dlg.DoModal() == IDOK)
	{
		m_strViewerPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void COptEtcPage::OnBtnEtcOptApply() 
{
	UpdateData(TRUE);

	CString sMsg = _T("");

	CurrentSet->bSaveGrabImg			= m_bSaveGrabImg;
	CurrentSet->bSaveProcImg			= TRUE; // m_bSaveProcImg;
	CurrentSet->bSaveCaptionTestImg		= m_bSaveCaptionTestImg;
	CurrentSet->bSaveMovingPicTestImg	= m_bSaveMovingPicTestImg;
	CurrentSet->bSaveIntermediateImage	= m_bSaveIntermediateImage;
	
	//+del kwmoon 080716
//	CurrentSet->bSaveDistributionTable	= m_bSaveDistributionTable;

	//+add kwmoon 080710
	CurrentSet->bSaveProcessingTimeData	= m_bSaveProcessingTimeData;
	CurrentSet->bSaveReviewData			= m_bSaveReviewData;
	CurrentSet->bSaveDetailResultData	= m_bSaveDetailResultData;
	CurrentSet->sViewerPath				= m_strViewerPath;
	
	//+change kwmoon 080715
	CurrentSet->bSaveResult2Txt			=  m_bResultSaveText;
	CurrentSet->bSaveResult2Html		=  m_bResultSaveHtml;
	CurrentSet->bSaveResult2Csv			=  m_bResultSaveCsv;

	CurrentSet->nNoRepeatExecution		= m_nNoRepeatExecution;
	//+del kwmoon 080715
//	CurrentSet->nNoAllowedErrorPixel	= m_fAllowedErrorPixelPercentage;
	CurrentSet->nNoFrameForRefImageCreation	= m_nNoFrameForRefImageCreation;

	CurrentSet->nNonBlackPixel		= m_nNonBlackPixel;
	CurrentSet->nPixelInMiddleRange	= m_nPixelInMiddleRange;
	CurrentSet->nDelayPercentage		= m_nDelayPercentage;
	CurrentSet->nNoRetry				= m_nNoRetry;

	//+add 090218(Modification No1)
	CurrentSet->nNoUsedColors		= m_nNoUsedColors;
	
	//+del kwmoon 080715
//	CurrentSet->nNoAllowedErrorPixelForOsdTest	= m_fAllowedErrorPixelPercentageForOsdTest;
	
	//+del kwmoon 080125
	//+add kwmoon 071227
//	CurrentSet->nSpecMargin				= m_nSpecMargin;
	
	//+add kwmoon 080123
	CurrentSet->nFreqMargin		= m_nFreqMargin;

	CurrentSet->nUseUserRetry = m_nDisplayType;
	CurrentSet->bVersionCheckOnly =	m_bRelayOnOff;

	//+change kwmoon 080724
	//change PSh 080605
//	for(int nTmp = 0; nTmp < 21; nTmp++)
	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		CurrentSet->nSrcMargin[i] = (UINT)_ttoi(m_ctrlSpecAdjList.GetItemText(i, 1));// .GetValueMatrix(i + 1, 1);
	}

	//+add kwmoon 080715
	CurrentSet->fAllowedBandErrorPixelPercentage[0] = m_fAllowedBand1ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[1] = m_fAllowedBand2ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[2] = m_fAllowedBand3ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[3] = m_fAllowedBand4ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[4] = m_fAllowedBand5ErrorPixelPercentage;
	//-

	//+add kwmoon 080716
	CurrentSet->nLevelMargin = m_nLevelMargin;

	CurrentSet->bFixtureIdCheck = 	m_bFixtureIdCheck;
	CurrentSet->nSoundInDeviceID = m_cComboSoundLeft.GetCurSel();



	//+add kwmoon 071227
	g_pView->SaveRegistrySetting();

}


void COptEtcPage::OnBtnEtcOptClearTotalCount() 
{
	// TODO: Add your control notification handler code here

	g_nNoTested = 0;
	g_nFailed	= 0;
	//+add kwmoon 081004
	g_nUserStop	= 0;

	g_dRate		= 0.0;

	sTested		= _T("0");
	sFailed		= _T("0");
	sRate		= _T("0");
	g_pView->UpdateInfo(TESTED, sTested);
	g_pView->UpdateInfo(FAILED, sTested);
	g_pView->UpdateInfo(RATE, sTested);

	//+add kwmoon 081004
	m_nNoTestedBoard = g_nNoTested;
	m_nNoFailedBoard = g_nFailed;
	m_nNoUserStop	 = g_nUserStop;

	UpdateData(FALSE);
}

BEGIN_EVENTSINK_MAP(COptEtcPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(COptEtcPage)
	ON_EVENT(COptEtcPage, IDC_SPEC_ADJ_GRID, -601 /* DblClick */, OnDblClickSpecAdjGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void COptEtcPage::OnDblClickSpecAdjGrid() 
{
	//int nRow = m_ctrlSpecAdjGrid.GetRow();
	//int nCol = m_ctrlSpecAdjGrid.GetCol();
	//if((nRow < 1)||(nCol != 1)) return;
	//CString sTmp, sList;

	//m_ctrlSpecAdjGrid.SetRow(nRow);
	//m_ctrlSpecAdjGrid.SetCol(nCol);
	//m_ctrlSpecAdjGrid.SetEditable(flexEDNone);
	//m_ctrlSpecAdjGrid.SetColComboList(nCol, _T(""));
	//m_ctrlSpecAdjGrid.SetEditable(flexEDKbdMouse);

	//for(int nTmp = 0; nTmp <= 100; nTmp++)
	//{
	//	sTmp.Format("|%d", nTmp);
	//	sList = sList + sTmp;
	//}
	//
	//m_ctrlSpecAdjGrid.SetColComboList(nCol, sList);
}






//+add kwmoon 080502
void COptEtcPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnEtcOptApply();

	CPropertyPage::OnOK();
}

//+add kwmoon 080904
void COptEtcPage::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	//+add kwmoon 080910
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	
	CString sMsg = _T("");
	
	OnBtnEtcOptApply();

	//+move kwmoon 080904
	//+add kwmoon 080716
	//+add kwmoon 080821 : Add FileExists Check
	if(FileExists(CurrentSet->sModelIni))
	{
		SaveModelIniFile(CurrentSet->sModelIni);
	}
	else
	{
		CreateModelIniFile(CurrentSet->sModelIni);
		sMsg.Format("File is created.\n[%s]",CurrentSet->sModelIni);
		AfxMessageBox(sMsg);
	}

	//+change kwmoon 080724
	if(FileExists(CurrentSet->sTestParamIni))
	{
		SaveTestParamIniFile(CurrentSet->sTestParamIni);
	}
	else
	{
		CreateTestParamIniFile(CurrentSet->sTestParamIni);
		sMsg.Format("File is created.\n[%s]",CurrentSet->sTestParamIni);
		AfxMessageBox(sMsg);
	}	
}


void COptEtcPage::OnChkSaveCaptionTestImage() 
{
	// TODO: Add your control notification handler code here
	
}


void COptEtcPage::OnNMDblclkListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;


	m_EditComboMode = 0;
	SetComboBoxList(nItem, nShell);
	//if (nShell == 1)//m_cEditPinName
	//{
	//	RECT lMainRect, lRect;
	//	this->GetWindowRect(&lMainRect);

	//	m_ctrlSpecAdjList.GetWindowRect(&lRect);

	//	int x = lRect.left - lMainRect.left;
	//	int y = lRect.top - lMainRect.top;

	//	//m_cEditPinName.SetWindowPos(this, lRect.left+70, lRect.top+nItem*30, 70, 30, SWP_ASYNCWINDOWPOS);
	//	GetDlgItem(IDC_COMBO_MARGIN2)->SetWindowPos(NULL, x + 50, y + (nItem+1) * 20, 60, 18, SWP_ASYNCWINDOWPOS);
	////	GetDlgItem(IDC_COMBO_MARGIN3)->SetWindowPos(NULL, x + 30, y + nItem * 20, 60, 18, SWP_ASYNCWINDOWPOS);
	//	m_cComboMargin.ShowWindow(SW_SHOW);
	//	//	m_cEditPinName.SetRect(&lRect);
	//	//SetDlgItemText(IDC_EDIT_PIN_NAME, gTestSeqData.TestStepList[nItem].strStepName);


	//	m_SelectItem = nItem;
	//	m_SelectShell = nShell;

	//	m_EditComboMode = 1;

	//}
	
	
	*pResult = 0;
//	SetTimer(1, 50, NULL);
	//m_cComboMargin.Invalidate();
}


void COptEtcPage::OnNMRClickListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cComboMargin.ShowWindow(SW_HIDE);
	*pResult = 0;
	
	m_EditComboMode = 0;
	
	
}


void COptEtcPage::OnNMClickListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	if (m_EditComboMode != 1)
	{
		return;
	}
	SetComboBoxList(nItem, nShell);
	*pResult = 0;
}
void COptEtcPage::SetComboBoxList(int nItem ,int nShell)
{
	CString Stemp;
	
	if (nShell == 1)//m_cEditPinName
	{
		RECT lMainRect, lRect;
		this->GetWindowRect(&lMainRect);

		m_ctrlSpecAdjList.GetWindowRect(&lRect);

		int x = lRect.left - lMainRect.left;
		int y = lRect.top - lMainRect.top;

		m_ctrlSpecAdjList.GetItemRect(nItem, &lRect, LVIR_BOUNDS);
		//m_cEditPinName.SetWindowPos(this, lRect.left+70, lRect.top+nItem*30, 70, 30, SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_COMBO_MARGIN2)->SetWindowPos(NULL, x + 80, y + (nItem + 1) * 16, 90, 15, SWP_ASYNCWINDOWPOS);
		GetDlgItem(IDC_COMBO_MARGIN2)->SetWindowPos(NULL, x + 80, y + lRect.top, 90, 15, SWP_ASYNCWINDOWPOS);
		//	GetDlgItem(IDC_COMBO_MARGIN3)->SetWindowPos(NULL, x + 30, y + nItem * 20, 60, 18, SWP_ASYNCWINDOWPOS);
		m_cComboMargin.ShowWindow(SW_SHOW);

		
		//	m_cEditPinName.SetRect(&lRect);
		//SetDlgItemText(IDC_EDIT_PIN_NAME, gTestSeqData.TestStepList[nItem].strStepName);


		m_SelectItem = nItem;
		m_SelectShell = nShell;

		m_EditComboMode = 1;

		m_cComboMargin.SetCurSel(_ttoi(m_ctrlSpecAdjList.GetItemText(nItem, nShell)));

	}
	//else if (nShell == 4)
	//{
	//	m_TEST_READ_Mode = 1;
	//	CheckValue(nItem);
	//}
	//else if (nShell == 3)
	//{
	//	m_TEST_READ_Mode = 0;
	//	CheckValue(nItem);
	//}

	
	m_ctrlSpecAdjList.Invalidate();
	SetTimer(1, 50, NULL); //m_cComboMargin.Invalidate();
}


void COptEtcPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_EditComboMode == 1)
	{
		m_cComboMargin.Invalidate();
		SetTimer(1, 1000, NULL);

	}
	CPropertyPage::OnTimer(nIDEvent);
}


void COptEtcPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_EditComboMode = 0;
	m_ctrlSpecAdjList.Invalidate();
	m_cComboMargin.ShowWindow(SW_HIDE);

	SetTimer(1, 50, NULL); //m_cComboMargin.Invalidate();
	


	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void COptEtcPage::OnLvnEndScrollListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 이 기능을 사용하려면 Internet Explorer 5.5 이상이 필요합니다.
	// _WIN32_IE 기호는 0x0560보다 크거나 같아야 합니다.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_EditComboMode = 0;
	m_ctrlSpecAdjList.Invalidate();
	m_cComboMargin.ShowWindow(SW_HIDE);

	SetTimer(1, 50, NULL); //m_cComboMargin.Invalidate();
	*pResult = 0;
}


void COptEtcPage::OnNMHoverListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void COptEtcPage::OnLvnBeginScrollListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 이 기능을 사용하려면 Internet Explorer 5.5 이상이 필요합니다.
	// _WIN32_IE 기호는 0x0560보다 크거나 같아야 합니다.
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void COptEtcPage::OnCbnSelchangeComboMargin2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sTmp;

	int l_ID = m_cComboMargin.GetCurSel();
	sTmp.Format(_T("%d"), l_ID);
	m_ctrlSpecAdjList.SetItem(m_SelectItem, m_SelectShell, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
	SetTimer(1, 50, NULL);
	//m_SelectItem = nItem;
	//m_SelectShell = nShell;
}


void COptEtcPage::OnCbnSelchangeComboSoundL()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nDeviceID = m_cComboSoundLeft.GetCurSel();
	g_pView->SelchangeSoudDevice(nDeviceID);
}
