// OptEtcPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptEtcPage.h"
#include "global.h"
#include "PLC_Ctrl.h"
#include "SoundCard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////

extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// COptEtcPage property page

IMPLEMENT_DYNCREATE(COptEtcPage, CPropertyPage)

COptEtcPage::COptEtcPage() : CPropertyPage(COptEtcPage::IDD)

, m_nAudioForce_HP_R(0)
, m_nAudioForce_HP_L(0)
, m_nAudioForce_Analog_L(0)
, m_nAudioForce_HDMI_L(0)
, m_nAudioForce_RGB_L(0)
, m_nAudioForce_SCART_L(0)
, m_nAudioForce_Analog_R(0)
, m_nAudioForce_HDMI_R(0)
, m_nAudioForce_RGB_R(0)
, m_nAudioForce_SCART_R(0)

, m_bCheckGrabACPower(FALSE)
, m_bCheckIFPackReset(FALSE)
, m_bCheckGenderReset(FALSE)
, m_bRobotSkipFFC(FALSE)
, m_bCheckUsePcbSensor(FALSE)
, m_bCheckRotateReset(FALSE)
, m_bCheckImageFullReset(FALSE)
, m_bCheckOledReset(FALSE)
, m_bCheckBatteryCheck(FALSE)
, m_bEpiPAckReset(FALSE)
, m_bGrabBaseReset(FALSE)
, m_bCheckGrabberOnOffCheck(FALSE)
, m_bFHDUHD_OLED_Pack_Reset(FALSE)
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
	m_bNgCountCheck = FALSE;
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
	DDV_MinMaxDouble(pDX, m_fAllowedErrorPixelPercentage, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_NO_FRAME, m_nNoFrameForRefImageCreation);
	DDV_MinMaxUInt(pDX, m_nNoFrameForRefImageCreation, 1, 10);
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
	DDV_MinMaxDouble(pDX, m_fAllowedErrorPixelPercentageForOsdTest, 0.f, 1.f);
	DDX_Text(pDX, IDC_EDIT_NONBLACK_PIXEL, m_nNonBlackPixel);
	DDV_MinMaxUInt(pDX, m_nNonBlackPixel, 1, 100);
	DDX_Text(pDX, IDC_EDIT_PIXEL_IN_MIDDLE_RANGE, m_nPixelInMiddleRange);
	DDV_MinMaxUInt(pDX, m_nPixelInMiddleRange, 1, 100);
	DDX_Radio(pDX, IDC_RADIO_SAVE_NG_IMAGE, m_bSaveGrabImg);
	DDX_Check(pDX, IDC_CHK_SAVE_PROCESSING_TIME_DATA, m_bSaveProcessingTimeData);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_1, m_fAllowedBand1ErrorPixelPercentage);
	DDV_MinMaxDouble(pDX, m_fAllowedBand1ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_2, m_fAllowedBand2ErrorPixelPercentage);
	DDV_MinMaxDouble(pDX, m_fAllowedBand2ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_3, m_fAllowedBand3ErrorPixelPercentage);
	DDV_MinMaxDouble(pDX, m_fAllowedBand3ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_4, m_fAllowedBand4ErrorPixelPercentage);
	DDV_MinMaxDouble(pDX, m_fAllowedBand4ErrorPixelPercentage, 0.f, 10.f);
	DDX_Text(pDX, IDC_EDIT_ALLOWED_ERROR_PIXELS_5, m_fAllowedBand5ErrorPixelPercentage);
	DDV_MinMaxDouble(pDX, m_fAllowedBand5ErrorPixelPercentage, 0.f, 10.f);
	DDX_Check(pDX, IDC_CHK_SAVE_REVIEW_DATA, m_bSaveReviewData);
	DDX_Check(pDX, IDC_CHK_SAVE_DETAIL_RESULT_DATA, m_bSaveDetailResultData);
	DDX_Text(pDX, IDC_EDIT_LEVEL_MARGIN, m_nLevelMargin);
	DDV_MinMaxInt(pDX, m_nLevelMargin, 0, 100);
	DDX_Text(pDX, IDC_EDIT_NO_USER_STOP, m_nNoUserStop);
	DDX_Text(pDX, IDC_EDIT_NO_TESTED_BOARD, m_nNoTestedBoard);
	DDX_Text(pDX, IDC_EDIT_NO_FAILED_BOARD, m_nNoFailedBoard);
	DDX_Text(pDX, IDC_EDIT_NO_COLORS_USED, m_nNoUsedColors);
	DDX_Check(pDX, IDC_CHECK_NGCOUNT, m_bNgCountCheck);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDIT_FORCE_HP_R, m_nAudioForce_HP_R);
	DDV_MinMaxInt(pDX, m_nAudioForce_HP_R, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_HP_L, m_nAudioForce_HP_L);
	DDV_MinMaxInt(pDX, m_nAudioForce_HP_L, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_COMPO_L, m_nAudioForce_Analog_L);
	DDV_MinMaxInt(pDX, m_nAudioForce_Analog_L, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_HDMI_L, m_nAudioForce_HDMI_L);
	DDV_MinMaxInt(pDX, m_nAudioForce_HDMI_L, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_RGB_L, m_nAudioForce_RGB_L);
	DDV_MinMaxInt(pDX, m_nAudioForce_RGB_L, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_SCART_L, m_nAudioForce_SCART_L);
	DDV_MinMaxInt(pDX, m_nAudioForce_SCART_L, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_COMPO_R, m_nAudioForce_Analog_R);
	DDV_MinMaxInt(pDX, m_nAudioForce_Analog_R, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_HDMI_R, m_nAudioForce_HDMI_R);
	DDV_MinMaxInt(pDX, m_nAudioForce_HDMI_R, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_RGB_R, m_nAudioForce_RGB_R);
	DDV_MinMaxInt(pDX, m_nAudioForce_RGB_R, 0, 5000);
	DDX_Text(pDX, IDC_EDIT_FORCE_SCART_R, m_nAudioForce_SCART_R);
	DDV_MinMaxInt(pDX, m_nAudioForce_SCART_R, 0, 5000);

	DDX_Control(pDX, IDC_COMBO_HDMI_ARC_PORT_NUMBER, m_nARC_Force_Port_Number);
	DDX_Control(pDX, IDC_COMBO_HDMI_ARC_ONOFF_CHANNEL, m_nARC_Force_OnOffChannel);
	DDX_Check(pDX, IDC_CHECK_GRAB_AC_POWER, m_bCheckGrabACPower);
	DDX_Check(pDX, IDC_CHECK_IF_PACK_RESET, m_bCheckIFPackReset);
	DDX_Check(pDX, IDC_CHECK_GENDER_RESET, m_bCheckGenderReset);
	DDX_Check(pDX, IDC_CHECK_SKIP_FFC, m_bRobotSkipFFC);
	DDX_Check(pDX, IDC_CHECK_PCB_SENSOR, m_bCheckUsePcbSensor);
	DDX_Check(pDX, IDC_CHECK_ROTATE_RESET, m_bCheckRotateReset);
	DDX_Check(pDX, IDC_CHECK_FULL_RESET, m_bCheckImageFullReset);
	DDX_Check(pDX, IDC_CHECK_OLED_RESET, m_bCheckOledReset);
	DDX_Check(pDX, IDC_CHECK_BAT_VER_OPTION, m_bCheckBatteryCheck);
	DDX_Check(pDX, IDC_CHK_EPI_PACK_RESET2, m_bEpiPAckReset);
	DDX_Check(pDX, IDC_CHK_GRAB_BASE_RESET2, m_bGrabBaseReset);
	DDX_Check(pDX, IDC_CHECK_GRABBER_ONOFF_OPTION, m_bCheckGrabberOnOffCheck);
	DDX_Check(pDX, IDC_CHECK_FUHD_OLED_RESET, m_bFHDUHD_OLED_Pack_Reset);
	DDX_Control(pDX, IDC_COMBO_AUDIO_CARD, m_cComboAudioCard);
	DDX_Control(pDX, IDC_COMBO_USB_AUDIO_CARD, m_cComboUSBAudioCard);
}


BEGIN_MESSAGE_MAP(COptEtcPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptEtcPage)
	ON_BN_CLICKED(IDC_BTN_OPEN_VIEWER, OnBtnOpenViewer)
	ON_BN_CLICKED(IDC_BTN_ETC_OPT_APPLY, OnBtnEtcOptApply)
	ON_BN_CLICKED(IDC_BTN_ETC_OPT_CLEAR_TOTAL_COUNT, OnBtnEtcOptClearTotalCount)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_CHK_SAVE_CAPTION_TEST_IMAGE, OnChkSaveCaptionTestImage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_SKIP_FFC, &COptEtcPage::OnBnClickedCheckSkipFfc)
	ON_BN_CLICKED(IDC_CHK_EPI_PACK_RESET2, &COptEtcPage::OnBnClickedChkEpiPackReset2)
	ON_BN_CLICKED(IDC_CHK_GRAB_BASE_RESET2, &COptEtcPage::OnBnClickedChkGrabBaseReset2)
	ON_CBN_SELCHANGE(IDC_COMBO_USB_AUDIO_CARD, &COptEtcPage::OnCbnSelchangeComboUsbAudioCard)
	ON_BN_CLICKED(IDC_BTN_SOUND_CARD_REFRESH, &COptEtcPage::OnBnClickedBtnSoundCardRefresh)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COptEtcPage message handlers

BOOL COptEtcPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


	if (g_nRunningProcessNo != 1) {
		GetDlgItem(IDC_CHECK_DFT2_AUTORUN)->EnableWindow(FALSE);
	}
	else {
		CheckDlgButton(IDC_CHECK_DFT2_AUTORUN, CurrentSet->bAutoRun_DFT2);
	}

	CheckDlgButton(IDC_CHECK_PJT_NO_GRAB, CurrentSet->bPJT_GrabDisable);
	
	m_bNgCountCheck			 = CurrentSet->bNgCountCheck;

	m_bCheckGrabACPower = CurrentSet->bCheckGrabACPower;
	m_bCheckIFPackReset = CurrentSet->bCheckIFPackReset;
	m_bCheckGenderReset = CurrentSet->bCheckGenderReset;
	m_bCheckRotateReset = CurrentSet->bCheckRotateReset;
	m_bCheckImageFullReset = CurrentSet->bCheckImageFullReset;
	m_bCheckOledReset = CurrentSet->bCheckOledReset;

	m_bRobotSkipFFC = CurrentSet->bRobotSkipFFC ? 0:1;

	m_bCheckUsePcbSensor = CurrentSet->bCheckUsePcbSensor;
	m_bCheckBatteryCheck = CurrentSet->bCheckBatteryCheck;
	m_bCheckGrabberOnOffCheck = CurrentSet->bCheckGrabberOnOffCheck;
	
	m_bEpiPAckReset = CurrentSet->bEpiPAckReset;
	m_bGrabBaseReset = CurrentSet->bGrabBaseReset;

	m_bFHDUHD_OLED_Pack_Reset = CurrentSet->bFHDUHD_OLED_Pack_Reset;

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
	m_bSaveReviewData = CurrentSet->bSaveReviewData;
	m_bSaveDetailResultData = CurrentSet->bSaveDetailResultData;

	//+ 2007.11.27 Mod BY USY
	m_nDisplayType = DETAILEDGRID;
	//	m_nDisplayType			= CurrentSet->nDisplayType;
		//-

		//+change kwmoon 080715
	m_bResultSaveText = CurrentSet->bSaveResult2Txt;
	m_bResultSaveHtml = CurrentSet->bSaveResult2Html;
	m_bResultSaveCsv = CurrentSet->bSaveResult2Csv;
	//-

	m_nNoRepeatExecution = CurrentSet->nNoRepeatExecution;

	//+del kwmoon 080715
	//	m_fAllowedErrorPixelPercentage  = CurrentSet->nNoAllowedErrorPixel;
	m_nNoFrameForRefImageCreation = CurrentSet->nNoFrameForRefImageCreation;
	m_nNonBlackPixel = CurrentSet->nNonBlackPixel;
	m_nPixelInMiddleRange = CurrentSet->nPixelInMiddleRange;
	m_nDelayPercentage = CurrentSet->nDelayPercentage;
	m_nNoRetry = CurrentSet->nNoRetry;

	//+add 090218(Modification No1)
	m_nNoUsedColors = CurrentSet->nNoUsedColors;

	//+del kwmoon 080715
	//	m_fAllowedErrorPixelPercentageForOsdTest  = CurrentSet->nNoAllowedErrorPixelForOsdTest;

		//+del kwmoon 080125
		//+add kwmoon 071227
	//	m_nSpecMargin			= CurrentSet->nSpecMargin;

		//+add kwmoon 080123
	m_nFreqMargin = CurrentSet->nFreqMargin;

	m_ctrlNoRepeatExecution.SetRange(1, 1000);
	//+del kwmoon 080715
	//	m_ctrlNoAllowedErrorPixel.SetRange(0,10000);
	m_ctrlNoFrame.SetRange(1, 10);

	m_ctrlNonBlackPixel.SetRange(1, 100);
	m_ctrlPixelInMiddleRange.SetRange(1, 100);

	//+del kwmoon 080715
	//	m_ctrlNoAllowedErrorPixelForOsdTest.SetRange(0,10000);

	m_ctrlNoRetry.SetRange(0, 100);
	m_ctrlDelayPercentage.SetRange(1, 100);
	m_ctrlAudioFreqSpecMargin.SetRange(0, 100);
	//+add kwmoon 080819
	m_ctrlAudioLevelSpecMargin.SetRange(0, 100);

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


	//+add kwmoon 081004
	m_nNoTestedBoard = g_nNoTested;
	m_nNoFailedBoard = g_nFailed;
	m_nNoUserStop = g_nUserStop;

	m_nAudioForce_HP_R = CurrentSet->nAudioForce_HP_R;
	m_nAudioForce_HP_L = CurrentSet->nAudioForce_HP_L;
	m_nAudioForce_Analog_L = CurrentSet->nAudioForce_Analog_L;
	m_nAudioForce_Analog_R = CurrentSet->nAudioForce_Analog_R;
	m_nAudioForce_HDMI_L = CurrentSet->nAudioForce_HDMI_L;
	m_nAudioForce_HDMI_R = CurrentSet->nAudioForce_HDMI_R;
	m_nAudioForce_RGB_L = CurrentSet->nAudioForce_RGB_L;
	m_nAudioForce_RGB_R = CurrentSet->nAudioForce_RGB_R;
	m_nAudioForce_SCART_L = CurrentSet->nAudioForce_SCART_L;
	m_nAudioForce_SCART_R = CurrentSet->nAudioForce_SCART_R;


	m_nARC_Force_Port_Number.SetCurSel(CurrentSet->nARC_Force_Port_Number);
	m_nARC_Force_OnOffChannel.SetCurSel(CurrentSet->nARC_Force_OnOffChannel);
	//g_SoundCard.SoundInMainSet();



	g_SoundCard.RefreshDeviceArray();

	for (int i = 0; i < g_SoundCard.strHWDeviceLocationArry.GetCount(); i++)
	{
		CString str_SoundName = g_SoundCard.strHWDeviceLocationArry.GetAt(i);
		
		if (CurrentSet->sUSBMIC_HW_ID.Find(str_SoundName) >= 0)
		{
			CurrentSet->sUSBMIC_HW_Name = g_SoundCard.strHWDeviceNameArry.GetAt(i);
		}
	}


	SetDlgItemText(IDC_STATIC_USB_HW_ID, CurrentSet->sUSBMIC_HW_ID);
	SetDlgItemText(IDC_STATIC_USB_HW_NAME, CurrentSet->sUSBMIC_HW_Name);
	for (int i = 0; i < g_SoundCard.strDeviceNameArry.GetCount(); i++)
	{
		CString str_SoundName = g_SoundCard.strDeviceNameArry.GetAt(i);
		m_cComboAudioCard.AddString(str_SoundName);
		m_cComboUSBAudioCard.AddString(str_SoundName);
		if (CurrentSet->sMainMIC_Name.Find(str_SoundName) >= 0)
		{
			m_cComboAudioCard.SetCurSel(i);
		}
		if (CurrentSet->sUSBMIC_HW_Name.Find(str_SoundName) >= 0)
		{
			m_cComboUSBAudioCard.SetCurSel(i);
			
		}
	}
	m_cComboUSBAudioCard.AddString("Not Found");

	
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

	CurrentSet->bAutoRun_DFT2 = IsDlgButtonChecked(IDC_CHECK_DFT2_AUTORUN);
	CurrentSet->bPJT_GrabDisable = IsDlgButtonChecked(IDC_CHECK_PJT_NO_GRAB);

	CurrentSet->bNgCountCheck			= m_bNgCountCheck;
	CurrentSet->bCheckGrabACPower = m_bCheckGrabACPower ;
	CurrentSet->bCheckIFPackReset = m_bCheckIFPackReset ;
	CurrentSet->bCheckGenderReset = m_bCheckGenderReset ;
	CurrentSet->bCheckRotateReset = m_bCheckRotateReset;
	CurrentSet->bCheckImageFullReset = m_bCheckImageFullReset;
	CurrentSet->bCheckOledReset = m_bCheckOledReset;

	CurrentSet->bCheckBatteryCheck = m_bCheckBatteryCheck;
	CurrentSet->bCheckGrabberOnOffCheck = m_bCheckGrabberOnOffCheck;
	CurrentSet->bFHDUHD_OLED_Pack_Reset = m_bFHDUHD_OLED_Pack_Reset;

	CurrentSet->bEpiPAckReset = m_bEpiPAckReset;
	CurrentSet->bGrabBaseReset = m_bGrabBaseReset;
	
	CurrentSet->bRobotSkipFFC = 1;  //cys 200824 m_bRobotSkipFFC ? 0 : 1;
	CurrentSet->bCheckUsePcbSensor = 1; //cys 200824 m_bCheckUsePcbSensor;

	if ((g_nSysRobotType == AUTO_SYS) && (g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
	{

		if (gPLC_Ctrl.m_cCheckSkipFFC == 0)
		{
			gPLC_Ctrl.CommandSkipFFC();
		}		
	}
	


	CurrentSet->bSaveGrabImg			= m_bSaveGrabImg;
	CurrentSet->bSaveProcImg			= TRUE; // m_bSaveProcImg;
	CurrentSet->bSaveCaptionTestImg		= m_bSaveCaptionTestImg;
	CurrentSet->bSaveMovingPicTestImg	= m_bSaveMovingPicTestImg;
	CurrentSet->bSaveIntermediateImage	= m_bSaveIntermediateImage;
	
	//+add kwmoon 080710
	CurrentSet->bSaveProcessingTimeData	= m_bSaveProcessingTimeData;
	CurrentSet->bSaveReviewData			= m_bSaveReviewData;
	CurrentSet->bSaveDetailResultData	= m_bSaveDetailResultData;
	CurrentSet->sViewerPath				= m_strViewerPath;
	
	//+change kwmoon 080715
	CurrentSet->bSaveResult2Txt			= m_bResultSaveText;
	CurrentSet->bSaveResult2Html		= m_bResultSaveHtml;
	CurrentSet->bSaveResult2Csv			= m_bResultSaveCsv;

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
	
	CurrentSet->nFreqMargin		= m_nFreqMargin;

	CurrentSet->fAllowedBandErrorPixelPercentage[0] = m_fAllowedBand1ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[1] = m_fAllowedBand2ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[2] = m_fAllowedBand3ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[3] = m_fAllowedBand4ErrorPixelPercentage;
	CurrentSet->fAllowedBandErrorPixelPercentage[4] = m_fAllowedBand5ErrorPixelPercentage;
	//-
	//+add kwmoon 080716
	CurrentSet->nLevelMargin = m_nLevelMargin;

	m_cComboAudioCard.GetLBText(m_cComboAudioCard.GetCurSel(), CurrentSet->sMainMIC_Name);
	CString strTmp;
	GetDlgItemText(IDC_STATIC_USB_HW_ID, strTmp);
	if (strTmp.GetLength() > 15)
	{
		CurrentSet->sUSBMIC_HW_ID = strTmp;
	}

	GetDlgItemText(IDC_STATIC_USB_HW_NAME, strTmp);	
	if (strTmp.GetLength() > 15)
	{
		CurrentSet->sUSBMIC_HW_Name = strTmp;
	}


	//m_cComboUSBAudioCard.GetLBText(m_cComboUSBAudioCard.GetCurSel(), CurrentSet->sUSBMIC_HW_Name);	
	//if (CurrentSet->sUSBMIC_HW_Name.GetLength() > 15)
	//{
	//	for (int i = 0; i < g_SoundCard.strHWDeviceNameArry.GetCount(); i++)
	//	{
	//		if (g_SoundCard.strHWDeviceNameArry.GetAt(i).Find(CurrentSet->sUSBMIC_HW_Name) >= 0)
	//		{
	//			CurrentSet->sUSBMIC_HW_ID = g_SoundCard.strHWDeviceIDArry.GetAt(i);
	//			CurrentSet->sUSBMIC_HW_Name = g_SoundCard.strHWDeviceNameArry.GetAt(i);
	//			break;
	//		}
	//	}
	//}


	//+add kwmoon 071227
	g_pView->SaveRegistrySetting();

	CurrentSet->nAudioForce_HP_R	= m_nAudioForce_HP_R;
	CurrentSet->nAudioForce_HP_L	= m_nAudioForce_HP_L;
	CurrentSet->nAudioForce_Analog_L= m_nAudioForce_Analog_L;
	CurrentSet->nAudioForce_Analog_R= m_nAudioForce_Analog_R;
	CurrentSet->nAudioForce_HDMI_L	= m_nAudioForce_HDMI_L;
	CurrentSet->nAudioForce_HDMI_R	= m_nAudioForce_HDMI_R;
	CurrentSet->nAudioForce_RGB_L	= m_nAudioForce_RGB_L;
	CurrentSet->nAudioForce_RGB_R	= m_nAudioForce_RGB_R;
	CurrentSet->nAudioForce_SCART_L	= m_nAudioForce_SCART_L;
	CurrentSet->nAudioForce_SCART_R	= m_nAudioForce_SCART_R;

	CurrentSet->nARC_Force_Port_Number = m_nARC_Force_Port_Number.GetCurSel();

	CurrentSet->nARC_Force_OnOffChannel = m_nARC_Force_OnOffChannel.GetCurSel();


	

//	m_nARC_Force_OnOffChannel.SetCurSel(CurrentSet->nARC_Force_OnOffChannel);

	//+move kwmoon 080904
/*	//+add kwmoon 080716
	//+add kwmoon 080821 : Add FileExists Check
	if(FileExists(CurrentSet->sModelIni))
	{
		SaveModelIniFile(CurrentSet->sModelIni);
	}
	else
	{
		sMsg.Format("Failed to save file.\n[%s]",CurrentSet->sModelIni);
		AfxMessageBox(sMsg);
	}

	//+change kwmoon 080724
	SaveTestParamIniFile(CurrentSet->sTestParamIni);	
*/
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
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


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


void COptEtcPage::OnBnClickedCheckSkipFfc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void COptEtcPage::OnBnClickedChkEpiPackReset2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void COptEtcPage::OnBnClickedChkGrabBaseReset2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void COptEtcPage::OnCbnSelchangeComboUsbAudioCard()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int sel_id = m_cComboUSBAudioCard.GetCurSel();
	if (g_SoundCard.strDeviceNameArry.GetCount() <= sel_id)
	{
		return;
	}

	CString str_SoundName = g_SoundCard.strDeviceNameArry.GetAt(sel_id);

	for (int i = 0; i < g_SoundCard.strHWDeviceNameArry.GetCount(); i++)
	{
		CString str = g_SoundCard.strHWDeviceNameArry.GetAt(i);
		if (str.Find(str_SoundName) >= 0)
		{
			CString str = g_SoundCard.strHWDeviceLocationArry.GetAt(i);
			SetDlgItemText(IDC_STATIC_USB_HW_ID, g_SoundCard.strHWDeviceLocationArry.GetAt(i));
			SetDlgItemText(IDC_STATIC_USB_HW_NAME, str_SoundName);
			break;
		}
	}
}


void COptEtcPage::OnBnClickedBtnSoundCardRefresh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	g_SoundCard.RefreshDeviceArray();

	for (int i = 0; i < g_SoundCard.strHWDeviceLocationArry.GetCount(); i++)
	{
		CString str_SoundName = g_SoundCard.strHWDeviceLocationArry.GetAt(i);

		if (CurrentSet->sUSBMIC_HW_ID.Find(str_SoundName) >= 0)
		{
			CurrentSet->sUSBMIC_HW_Name = g_SoundCard.strHWDeviceNameArry.GetAt(i);
		}
	}


	SetDlgItemText(IDC_STATIC_USB_HW_ID, CurrentSet->sUSBMIC_HW_ID);
	SetDlgItemText(IDC_STATIC_USB_HW_NAME, CurrentSet->sUSBMIC_HW_Name);

	m_cComboAudioCard.ResetContent();
	m_cComboUSBAudioCard.ResetContent();


	for (int i = 0; i < g_SoundCard.strDeviceNameArry.GetCount(); i++)
	{
		CString str_SoundName = g_SoundCard.strDeviceNameArry.GetAt(i);
		m_cComboAudioCard.AddString(str_SoundName);
		m_cComboUSBAudioCard.AddString(str_SoundName);
		if (CurrentSet->sMainMIC_Name.Find(str_SoundName) >= 0)
		{
			m_cComboAudioCard.SetCurSel(i);
		}
		if (CurrentSet->sUSBMIC_HW_Name.Find(str_SoundName) >= 0)
		{
			m_cComboUSBAudioCard.SetCurSel(i);
			SetDlgItemText(IDC_STATIC_USB_HW_ID, g_SoundCard.strHWDeviceLocationArry.GetAt(i));
		}
	}
	m_cComboUSBAudioCard.AddString("Not Found");
}
