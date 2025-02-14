// OptGrabberPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "Defines.h"
#include "OptGrabberPage.h"
#include "Global.h"
#include "SetDateDlg.h"
#include "UserMsgDlg.h"

//+add 090420(Modification No1)
#include "I2cAdcCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;
extern CBaseImage	g_GrabDisplayImage;
extern CBaseImage	g_GrabImage;

//+add 090420(Modification No1)
extern CI2cAdcCtrl	 I2cAdcCtrl;

/////////////////////////////////////////////////////////////////////////////
// COptGrabberPage property page

IMPLEMENT_DYNCREATE(COptGrabberPage, CPropertyPage)

COptGrabberPage::COptGrabberPage() : CPropertyPage(COptGrabberPage::IDD)
, m_bRedBlueSwap(FALSE)
, m_bDeHsyncEnable(FALSE)
, m_bHyncEdgeEnable(FALSE)
//, m_bEpiPAckReset(FALSE)
//, m_bGrabBaseReset(FALSE)
, m_nHdmiEdidControl(FALSE)
, m_nHdmiHdcpControl(FALSE)
{
	//{{AFX_DATA_INIT(COptGrabberPage)
	m_nAnalogFormat = -1;
	m_nLvdsFormat = -1;
	m_nUranousMode = -1;
	m_nBitMode = -1;
	m_nAudioMeasureMethod = 1;
	m_nGrabCheckArea = -1;
	m_bAutoDataDelete = FALSE;
	m_bOddDE_Only = FALSE;
	m_bImage_Rotation = FALSE;
	m_nHDMI_Resolution = -1;
	m_n3DMode = -1;
	m_nHdmiOutControl = -1;
	m_nHdmiCecControl = -1;
	m_nCompResolution = -1;
	m_bMaskOsd = FALSE;
	//}}AFX_DATA_INIT
}

COptGrabberPage::~COptGrabberPage()
{
}

void COptGrabberPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptGrabberPage)
	DDX_Control(pDX, IDC_COMBO_LVDSMODE, m_ctrlLvdsMode);
	DDX_Control(pDX, IDC_COMBO_HDMI7, m_ctrlHdmi3Model);
	DDX_Control(pDX, IDC_COMBO_HDMI6, m_ctrlHdmi2Model);
	DDX_Control(pDX, IDC_COMBO_HDMI8, m_ctrlHdmi4Model);
	DDX_Control(pDX, IDC_COMBO_HDMI5, m_ctrlHdmi1Model);
	DDX_Control(pDX, IDC_COMBO_GRABDELAY, m_ctrlGrabDelay);
	DDX_Control(pDX, IDC_COMBO_HDMITYPE, m_ctrlHdmiGrabResolution);
	DDX_Control(pDX, IDC_COMBO_HDMIBITSHIFT, m_ctrlHdmiBitShift);
	DDX_Control(pDX, IDC_COMBO_UHDTYPE, m_ctrlUhdType);
	DDX_Control(pDX, IDC_COMBO_GRABMODE, m_ctrlGrabMode);
	DDX_Control(pDX, IDC_COMBO_BITSHIFT, m_ctrlBitShift);
	DDX_Control(pDX, IDC_COMBO_HDMI4, m_ctrlHdmi4Port);
	DDX_Control(pDX, IDC_COMBO_HDMI3, m_ctrlHdmi3Port);
	DDX_Control(pDX, IDC_COMBO_HDMI2, m_ctrlHdmi2Port);
	DDX_Control(pDX, IDC_COMBO_HDMI1, m_ctrlHdmi1Port);
	DDX_Control(pDX, IDC_HDMI_BIT_SPEC, m_ctrlHDMI_BitSpec);
	DDX_Control(pDX, IDC_CHK_ODD_DE_ONLY, m_ctrlOddDE_Only);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_ctrlPeriod);
	DDX_Radio(pDX, IDC_RDO_ANALOG_FORMAT1, m_nAnalogFormat);
	//DDX_Radio(pDX, IDC_RDO_LVDS_FORMAT1, m_nLvdsFormat);
	DDX_Radio(pDX, IDC_RDO_URANOUS_MODE1, m_nUranousMode);
	DDX_Radio(pDX, IDC_RDO_RGB_RES_1, m_nBitMode);
	DDX_Radio(pDX, IDC_RDO_AVSWITCHBOX, m_nAudioMeasureMethod);
	DDX_Radio(pDX, IDC_GRAB_CHECK_AREA1, m_nGrabCheckArea);
	DDX_Check(pDX, IDC_CHK_AUTO_DATA_DELETE, m_bAutoDataDelete);
	DDX_Check(pDX, IDC_CHK_ODD_DE_ONLY, m_bOddDE_Only);
	DDX_Check(pDX, IDC_CHK_IMAGE_ROTATION, m_bImage_Rotation);
	DDX_Radio(pDX, IDC_HDMI_RESOLUTION1, m_nHDMI_Resolution);
	DDX_Radio(pDX, IDC_RDO_3D_MODE1, m_n3DMode);
	DDX_Radio(pDX, IDC_RADIO_HDMI_OUT_CONTROL, m_nHdmiOutControl);
	DDX_Radio(pDX, IDC_RADIO_HDMI_CEC_CONTROL1, m_nHdmiCecControl);
	DDX_Radio(pDX, IDC_RADIO_COMP_RESOLUTION1, m_nCompResolution);
	DDX_Check(pDX, IDC_CHECK_MASK_OSDREGION, m_bMaskOsd);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_RB_SWAP, m_bRedBlueSwap);
	DDX_Check(pDX, IDC_CHK_DE_HSYNC, m_bDeHsyncEnable);
	DDX_Check(pDX, IDC_CHK_HSYNC_EDGE, m_bHyncEdgeEnable);
	DDX_Control(pDX, IDC_COMBO_Y20_SW_MODE2, m_cComboY20SwMode);
	//DDX_Check(pDX, IDC_CHK_EPI_PACK_RESET, m_bEpiPAckReset);
	//DDX_Check(pDX, IDC_CHK_GRAB_BASE_RESET, m_bGrabBaseReset);
	DDX_Radio(pDX, IDC_RADIO_HDMI_EDID_CONTROL1, m_nHdmiEdidControl);
	DDX_Radio(pDX, IDC_RADIO_HDMI_HDCP_CONTROL1, m_nHdmiHdcpControl);
	DDX_Control(pDX, IDC_COMBO_LVDS_RESOLUTION, m_cComboLvdsFormat);
}


BEGIN_MESSAGE_MAP(COptGrabberPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptGrabberPage)
	ON_BN_CLICKED(IDC_BTN_GRABBER_OPT_APPLY, OnBtnGrabberOptApply)
	ON_BN_CLICKED(IDC_BTN_GRABBER_OPT_DELETE, OnBtnGrabberOptDelete)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_CBN_SELCHANGE(IDC_COMBO_PERIOD, OnSelchangeComboPeriod)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION1, OnHdmiResolution1)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION2, OnHdmiResolution2)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION3, OnHdmiResolution3)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION4, OnHdmiResolution4)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_RB_SWAP, &COptGrabberPage::OnBnClickedChkRbSwap)
	ON_BN_CLICKED(IDC_BUTTON_170_WRITE, &COptGrabberPage::OnBnClickedButton170Write)
	ON_BN_CLICKED(IDC_BUTTON_170_READ, &COptGrabberPage::OnBnClickedButton170Read)
	ON_CBN_SELCHANGE(IDC_COMBO_GRABMODE, &COptGrabberPage::OnCbnSelchangeComboGrabmode)
	ON_CBN_SELCHANGE(IDC_COMBO_UHDTYPE, &COptGrabberPage::OnCbnSelchangeComboUhdtype)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptGrabberPage message handlers


void COptGrabberPage::OnBtnGrabberOptApply() 
{
	UpdateData(TRUE);
	
	CString sMsg = _T("");

	int nTmp = 0;
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_pView->StopLVDSGrabThread();
		Sleep(100);
	}
	
	if(g_nGrabberType == FHD_GRABBER)
	{
		switch(m_nUranousMode) 
		{
			//0 : JEIDA_DUAL(0x81), 1 : JEIDA_SINGLE(0x01), 2 : NS_DUAL(0x80), 3 : NS_SINGLE(0x00)
			case 0 :  nTmp = 0x84; break; //JEIDA_DUAL
			case 1 :  nTmp = 0x04; break; //JEIDA_SINGLE
			case 2 :  nTmp = 0x80; break; //NS_DUAL
			case 3 :  nTmp = 0x00; break; //NS_SINGLE
			default : nTmp = 0x00; break; //NS_SINGLE
		}
	}

	CurrentSet->nFullHd120Hz = 0;
	CurrentSet->nImageRotation = 0;
	CurrentSet->nRedBlueSwap = 0;
	m_nLvdsFormat = m_cComboLvdsFormat.GetCurSel();
	switch(m_nLvdsFormat) 
	{
		case W_1024_H_768: 
			CurrentSet->nLvdsWidth = 1024; 
			CurrentSet->nLvdsHeight = 768;			
			break;

		case W_1280_H_768: 
			CurrentSet->nLvdsWidth = 1280; 
			CurrentSet->nLvdsHeight = 768;			
			break;

		case W_1280_H_1024: 
			CurrentSet->nLvdsWidth = 1280; 
			CurrentSet->nLvdsHeight = 1024;			
			break;

		case W_1364_H_768: 
			CurrentSet->nLvdsWidth = 1364; 
			CurrentSet->nLvdsHeight = 768;	
			break;

		case W_1366_H_768: 
			CurrentSet->nLvdsWidth = 1366; 
			CurrentSet->nLvdsHeight = 768;			
			break;

		case W_1440_H_900: 
			CurrentSet->nLvdsWidth = 1440; 
			CurrentSet->nLvdsHeight = 900;			
			break;

		case W_1600_H_900: 
			CurrentSet->nLvdsWidth = 1600; 
			CurrentSet->nLvdsHeight = 900;			
			break;

		case W_1680_H_1050: 
			CurrentSet->nLvdsWidth = 1680; 
			CurrentSet->nLvdsHeight = 1050;			
			break;
		
		case W_1920_H_1080_60: 
			CurrentSet->nLvdsWidth = 1920; 
			CurrentSet->nLvdsHeight = 1080;			
			CurrentSet->nFullHd120Hz = 0;
			if(g_nGrabberType == FHD_GRABBER)//FHD
			{
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if(m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			}
			break;
		
		case W_1920_H_1080_120: 
			CurrentSet->nLvdsWidth = 1920; 
			CurrentSet->nLvdsHeight = 1080;			
			CurrentSet->nFullHd120Hz = 1;
			if(g_nGrabberType == FHD_GRABBER)//FHD
			{
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if(m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			}
			break;
		
		case W_2560_H_1080: 
			CurrentSet->nLvdsWidth = 2560; 
			CurrentSet->nLvdsHeight = 1080;			
			CurrentSet->nFullHd120Hz = 1;
			if(g_nGrabberType == FHD_GRABBER)//FHD
			{
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if(m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			}
			break;

		case W_2560_H_1440: 
			CurrentSet->nLvdsWidth = 2560; 
			CurrentSet->nLvdsHeight = 1440;			
			CurrentSet->nFullHd120Hz = 1;
			if(g_nGrabberType == FHD_GRABBER)//FHD
			{
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if(m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			}
			break;

		case W_3440_H_1440: 
			CurrentSet->nLvdsWidth = 3440; 
			CurrentSet->nLvdsHeight = 1440;			
			break;

		case W_3840_H_2160: 
			CurrentSet->nLvdsWidth = 3840; 
			CurrentSet->nLvdsHeight = 2160;			
			break;

		case W_1920_H_300: 
			CurrentSet->nLvdsWidth = 1920; 
			CurrentSet->nLvdsHeight = 300;			
			break;
			
		case W_3840_H_600: 
			CurrentSet->nLvdsWidth = 3840; 
			CurrentSet->nLvdsHeight = 600;			
			break;
		case W_1920_H_540:
			CurrentSet->nLvdsWidth = 1920;
			CurrentSet->nLvdsHeight = 540;
			break;
		case W_1920_H_1200:
			CurrentSet->nLvdsWidth = 1920;
			CurrentSet->nLvdsHeight = 1200;
			break;
		case W_2048_H_2560:
			CurrentSet->nLvdsWidth = 2048;
			CurrentSet->nLvdsHeight = 2560;
			break;
		case W_3584_H_720:
			CurrentSet->nLvdsWidth = 3584;
			CurrentSet->nLvdsHeight = 720;
			break;
		case W_3328_H_1440:
			CurrentSet->nLvdsWidth = 3328;
			CurrentSet->nLvdsHeight = 1440;
			break;

			
		default : CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
	}
	switch(m_nAnalogFormat) 
	{
		case 0:  CurrentSet->nAnalogWidth = 640; CurrentSet->nAnalogHeight = 480; CurrentSet->nAnalogType = V_SRC_NTSC;	 break;
		case 1: CurrentSet->nAnalogWidth = 704; CurrentSet->nAnalogHeight = 572; CurrentSet->nAnalogType = V_SRC_PAL;	break;
		case 2: CurrentSet->nAnalogWidth = 704; CurrentSet->nAnalogHeight = 572; CurrentSet->nAnalogType = V_SRC_SECAM;	break;
//		case 1: CurrentSet->nAnalogWidth = 768; CurrentSet->nAnalogHeight = 576; CurrentSet->nAnalogType = V_SRC_PAL;	break;
//		case 2: CurrentSet->nAnalogWidth = 768; CurrentSet->nAnalogHeight = 576; CurrentSet->nAnalogType = V_SRC_SECAM;	break;
		default: CurrentSet->nAnalogWidth = 640; CurrentSet->nAnalogHeight = 480;	  CurrentSet->nAnalogType = V_SRC_NTSC;	 break;
	}

	if(g_nGrabberType == FHD_GRABBER)//FHD
	{
		if((g_pView->m_nGrabber_FWVer >= 0x10102301) &&	(CurrentSet->nFullHd120Hz ==0))
		{
			// add 101029
			switch(m_n3DMode) 
			{
				case 0:   break;
				case 1:	  nTmp = nTmp | 0x20; break;
				case 2:	  nTmp = nTmp | 0x120; break;
				default : break;
			}
			CurrentSet->n3DGrabbingMode = m_n3DMode;
		}

		switch(m_nBitMode) 
		{
			case 0:   CurrentSet->nUranousMode = nTmp;		   CurrentSet->nBitCount = 6*3;	 break;
			case 1:	  CurrentSet->nUranousMode = (nTmp | 0x1); CurrentSet->nBitCount = 8*3;  break;
			case 2:	  CurrentSet->nUranousMode = (nTmp | 0x2); CurrentSet->nBitCount = 10*3; break;
			case 3:	  CurrentSet->nUranousMode = (nTmp | 0x3); CurrentSet->nBitCount = 12*3; break;
			default : CurrentSet->nUranousMode = (nTmp | 0x3); CurrentSet->nBitCount = 8*3;	 break;
		}
	}

	CurrentSet->nGrabCheckArea = m_nGrabCheckArea;
	CurrentSet->nImageRotation = m_bImage_Rotation;
	CurrentSet->nRedBlueSwap = m_bRedBlueSwap;

	CurrentSet->nDeHsyncEnable = m_bDeHsyncEnable;
	CurrentSet->nHyncEdgeEnable = m_bHyncEdgeEnable;

	CString szErrorMsg   = _T("");
	CString szOptionData = _T("");


//	m_pProgressMsgDlg	= NULL;

//	if(m_nAudioMeasureMethod == METHOD_AVSWITCHBOX)
//	{
//		CurrentSet->nAudioCheckMethod = METHOD_AVSWITCHBOX;
//	}
//	else
//	{
		CurrentSet->nAudioCheckMethod = METHOD_SOUNDCARD;
//	}
	//+add psh 090401
	CurrentSet->nDataSavePeriod = m_nPeriod;	
	CurrentSet->bAutoDataDelete = m_bAutoDataDelete;

	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&g_GrabImage);
	}//

	CurrentSet->nHDMIResolution = m_nHDMI_Resolution;
	CurrentSet->nHDMIBitSpec = m_ctrlHDMI_BitSpec.GetCurSel();

	CurrentSet->bHdmiOutControl = 0;//  m_nHdmiOutControl;

	if(g_nGrabberType == UHD_GRABBER)//UHD
	{
		CurrentSet->nHDMI1_Model = m_ctrlHdmi1Model.GetCurSel();
		CurrentSet->nHDMI2_Model = m_ctrlHdmi2Model.GetCurSel();
		CurrentSet->nHDMI3_Model = m_ctrlHdmi3Model.GetCurSel();
		CurrentSet->nHDMI4_Model = m_ctrlHdmi4Model.GetCurSel();
	}

	CurrentSet->nHDMI1_Port = 1;// m_ctrlHdmi1Port.GetCurSel() + 1;
	CurrentSet->nHDMI2_Port = 2;// m_ctrlHdmi2Port.GetCurSel() + 1;
	CurrentSet->nHDMI3_Port = 3;// m_ctrlHdmi3Port.GetCurSel() + 1;
	CurrentSet->nHDMI4_Port = 4;// m_ctrlHdmi4Port.GetCurSel() + 1;

	CurrentSet->bHdmiCecControl = m_nHdmiCecControl;
	HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);

	CurrentSet->bHdmiEdidControl = m_nHdmiEdidControl;
	HDMIGeneratorCtrl.SetEDID_PassCheck(CurrentSet->bHdmiEdidControl);

	CurrentSet->bHdmiHdcpControl = m_nHdmiHdcpControl;
	HDMIGeneratorCtrl.SetHDCP_OnOff(CurrentSet->bHdmiHdcpControl);

	CurrentSet->nCompResolution = m_nCompResolution;

	if(g_nGrabberType == UHD_GRABBER)//UHD
	{
		CurrentSet->nLVDS_Mode = m_ctrlLvdsMode.GetCurSel();
		CurrentSet->nUHD_Type = m_ctrlUhdType.GetCurSel();
		CurrentSet->nUHD_Grab_BitShift = m_ctrlBitShift.GetCurSel();
		CurrentSet->nUHD_Grab_Mode = m_ctrlGrabMode.GetCurSel();

		CurrentSet->nUHD_Y20_SW_Mode = m_cComboY20SwMode.GetCurSel();
		

		CurrentSet->nUHD_Grab_Delay = m_ctrlGrabDelay.GetCurSel();

		CurrentSet->nHDMI_Grab_Resolution = m_ctrlHdmiGrabResolution.GetCurSel();
		CurrentSet->nHDMI_Grab_BitShift = m_ctrlHdmiBitShift.GetCurSel();

		switch(CurrentSet->nHDMI_Grab_Resolution) 
		{
			case 0:  CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
			case 1:  CurrentSet->nHdmiWidth = 1366; CurrentSet->nHdmiHeight = 768; break;
			case 2:  CurrentSet->nHdmiWidth = 1280; CurrentSet->nHdmiHeight = 1024; break;
			case 3:  CurrentSet->nHdmiWidth = 1920; CurrentSet->nHdmiHeight = 1080; break;
			case 4:  CurrentSet->nHdmiWidth = 1280; CurrentSet->nHdmiHeight = 540; break;
			default: CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
		}
		
		CurrentSet->bHdmiGrab_MaskOsd = m_bMaskOsd;
	}
	//CurrentSet->bEpiPAckReset = m_bEpiPAckReset;
	//CurrentSet->bGrabBaseReset = m_bGrabBaseReset;


	if(!CurrentSet->bHdmiOutControl){
		HDMIGeneratorCtrl.SetOutPort(0);
	}

//	g_pView->StopLVDSGrabThread();
//	g_pView->SetGrabInfo(&g_GrabImage);
//	Sleep(100);
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_pView->StartLVDSGrabThread();
	}

}

BOOL COptGrabberPage::MakeOptionString(CString& szOptionData,UINT nNoDigit)
{
	CString szZeroData = _T("");

	UINT nLength = szOptionData.GetLength();

	if(nLength <= nNoDigit)
	{
		for(int i=0; i<(int)(nNoDigit-nLength); i++)
		{
			szZeroData += '0';
		}
		szOptionData = szZeroData + szOptionData;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL COptGrabberPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// Lvds Format
	if((CurrentSet->nLvdsWidth == 1024) && (CurrentSet->nLvdsHeight == 768))  m_nLvdsFormat = W_1024_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 768))  m_nLvdsFormat = W_1280_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 1024)) m_nLvdsFormat = W_1280_H_1024;
	else if((CurrentSet->nLvdsWidth == 1364) && (CurrentSet->nLvdsHeight == 768))  m_nLvdsFormat = W_1364_H_768;
	else if((CurrentSet->nLvdsWidth == 1366) && (CurrentSet->nLvdsHeight == 768))  m_nLvdsFormat = W_1366_H_768;
	else if((CurrentSet->nLvdsWidth == 1440) && (CurrentSet->nLvdsHeight == 900))  m_nLvdsFormat = W_1440_H_900;
	else if((CurrentSet->nLvdsWidth == 1600) && (CurrentSet->nLvdsHeight == 900))  m_nLvdsFormat = W_1600_H_900;
	else if((CurrentSet->nLvdsWidth == 1680) && (CurrentSet->nLvdsHeight == 1050)) m_nLvdsFormat = W_1680_H_1050;
	else if((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 1080) && (CurrentSet->nFullHd120Hz == 0)) 
	{
		m_nLvdsFormat = W_1920_H_1080_60;
	}
	else if((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 1080) && (CurrentSet->nFullHd120Hz == 1)) 
	{
		m_nLvdsFormat = W_1920_H_1080_120;
	}
	else if((CurrentSet->nLvdsWidth == 2560) && (CurrentSet->nLvdsHeight == 1080) && (CurrentSet->nFullHd120Hz == 1)) 
	{
		m_nLvdsFormat = W_2560_H_1080;
	}
	else if((CurrentSet->nLvdsWidth == 2560) && (CurrentSet->nLvdsHeight == 1440) && (CurrentSet->nFullHd120Hz == 1)) 
	{
		m_nLvdsFormat = W_2560_H_1440;
	}
	else if((CurrentSet->nLvdsWidth == 3440) && (CurrentSet->nLvdsHeight == 1440)) m_nLvdsFormat = W_3440_H_1440;
	else if((CurrentSet->nLvdsWidth == 3840) && (CurrentSet->nLvdsHeight == 2160)) m_nLvdsFormat = W_3840_H_2160;
	else if((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 300))  m_nLvdsFormat = W_1920_H_300;
	else if((CurrentSet->nLvdsWidth == 3840) && (CurrentSet->nLvdsHeight == 600))  m_nLvdsFormat = W_3840_H_600;
	else if ((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 540))  m_nLvdsFormat = W_1920_H_540;
	else if ((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 1200))  m_nLvdsFormat = W_1920_H_1200;
	else if ((CurrentSet->nLvdsWidth == 2048) && (CurrentSet->nLvdsHeight == 2560))  m_nLvdsFormat = W_2048_H_2560;
	else if ((CurrentSet->nLvdsWidth == 3584) && (CurrentSet->nLvdsHeight == 740))
	{
		m_nLvdsFormat = W_3584_H_720;
	}
	else if ((CurrentSet->nLvdsWidth == 3328) && (CurrentSet->nLvdsHeight == 1440))  m_nLvdsFormat = W_3328_H_1440;
	else m_nLvdsFormat = -1;
	
	m_cComboLvdsFormat.AddString("1024 x 768(XGA)     ");	//#define		W_1024_H_768			0
	m_cComboLvdsFormat.AddString("1280 x 768(WXGA)    ");	//#define		W_1280_H_768			1
	m_cComboLvdsFormat.AddString("1280 x 1024         ");	//#define		W_1280_H_1024			2
	m_cComboLvdsFormat.AddString("1364 x 768          ");	//#define		W_1364_H_768			3
	m_cComboLvdsFormat.AddString("1366 x 768(HD)      ");	//#define		W_1366_H_768			4
	m_cComboLvdsFormat.AddString("1440 x 900          ");	//#define		W_1440_H_900			5
	m_cComboLvdsFormat.AddString("1600 x 900          ");	//#define		W_1600_H_900			6
	m_cComboLvdsFormat.AddString("1680 x 1050         ");	//#define		W_1680_H_1050			7
	m_cComboLvdsFormat.AddString("1920 x 1080(FHD,60Hz)");	//#define		W_1920_H_1080_60		8
	m_cComboLvdsFormat.AddString("1920 x 1080(FHD,120Hz)");	//#define		W_1920_H_1080_120		9
	m_cComboLvdsFormat.AddString("2560 x 1080(120Hz)  ");	//#define		W_2560_H_1080			10
	m_cComboLvdsFormat.AddString("2560 x 1440         ");	//#define		W_2560_H_1440			11
	m_cComboLvdsFormat.AddString("3440 x 1440         ");	//#define		W_3440_H_1440			12
	m_cComboLvdsFormat.AddString("3840 x 2160(UHD)    ");	//#define		W_3840_H_2160			13
	m_cComboLvdsFormat.AddString("1920 x 300          ");	//#define		W_1920_H_300			14
	m_cComboLvdsFormat.AddString("3840 x 600          ");	//#define		W_3840_H_600			15
	m_cComboLvdsFormat.AddString("1920 x 540          ");	//#define		W_1920_H_540			16
	m_cComboLvdsFormat.AddString("1920 x 1200         ");	//#define		W_1920_H_1200			17
	m_cComboLvdsFormat.AddString("2048 x 2560         ");	//#define		W_2048_H_2560			18
	m_cComboLvdsFormat.AddString("3584x 740(3440x1440)");	//#define		3584x 740(3440x1440) 	19
	m_cComboLvdsFormat.AddString("3328 x 1440         ");	//#define		W_3328_H_1440			20
	m_cComboLvdsFormat.SetCurSel(m_nLvdsFormat);
	m_nAnalogFormat = CurrentSet->nAnalogType;

	m_nGrabCheckArea = CurrentSet->nGrabCheckArea;
	m_bImage_Rotation = CurrentSet->nImageRotation;
	m_bRedBlueSwap = CurrentSet->nRedBlueSwap;

	m_bDeHsyncEnable = CurrentSet->nDeHsyncEnable;
	m_bHyncEdgeEnable = CurrentSet->nHyncEdgeEnable;

	
	m_bAutoDataDelete	 = CurrentSet->bAutoDataDelete;
	m_nPeriod  = CurrentSet->nDataSavePeriod;
	if(m_nPeriod <15){
		m_nPeriod = 15;
	}
	m_ctrlPeriod.SetCurSel(m_nPeriod/15 -1);
	//-

	m_nAudioMeasureMethod = CurrentSet->nAudioCheckMethod;

	m_nHDMI_Resolution = CurrentSet->nHDMIResolution;
	m_ctrlHDMI_BitSpec.SetCurSel(CurrentSet->nHDMIBitSpec);

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		InitItem(TRUE);

		if(CurrentSet->nHDMIResolution == 0)
		{
			GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(FALSE);
		
			GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(FALSE);

		}
		else if(CurrentSet->nHDMIResolution == 3)
		{
			GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(FALSE);

			GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(TRUE);
		
			GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(TRUE);

		}
		else{
			GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(FALSE);

			GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(FALSE);
		
			GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(FALSE);
		}
		m_ctrlHdmi1Model.SetCurSel(CurrentSet->nHDMI1_Model);
		m_ctrlHdmi2Model.SetCurSel(CurrentSet->nHDMI2_Model);
		m_ctrlHdmi3Model.SetCurSel(CurrentSet->nHDMI3_Model);
		m_ctrlHdmi4Model.SetCurSel(CurrentSet->nHDMI4_Model);
	
		m_ctrlLvdsMode.SetCurSel(CurrentSet->nLVDS_Mode);

		m_ctrlUhdType.ResetContent();
		m_ctrlUhdType.AddString("0. UHD Full Speed");
		m_ctrlUhdType.AddString("1. UHD Normal Speed");
		m_ctrlUhdType.AddString("2. FHD(60Hz) - Pack1(51p), Pack10");
		m_ctrlUhdType.AddString("3. FHD(120Hz) - Pack1(51p + 41p), EPI");
		m_ctrlUhdType.AddString("4. FHD");
		m_ctrlUhdType.AddString("5. No Use");
		m_ctrlUhdType.AddString("6. HD - Pack5 / 8");
		m_ctrlUhdType.AddString("7. EPI_UHD");
		m_ctrlUhdType.AddString("8. EPI_UHD(16Y_60P)");
		m_ctrlUhdType.AddString("9. CMVIEW");
		m_ctrlUhdType.AddString("10. 5_HD_Rev");
		m_ctrlUhdType.AddString("11. 5_Rev_2019");
		m_ctrlUhdType.AddString("12. 5_Rev_2020");
		m_ctrlUhdType.AddString("13. UHD Low Speed");
		m_ctrlUhdType.AddString("14. QHD Full Speed");
		m_ctrlUhdType.AddString("15. FHD(120Hz) Low Speed");
		m_ctrlUhdType.AddString("16. PACK_TYPE_STANDBYME");

		m_ctrlUhdType.SetCurSel(CurrentSet->nUHD_Type);

		m_ctrlBitShift.SetCurSel(CurrentSet->nUHD_Grab_BitShift);

		m_ctrlGrabMode.ResetContent();
		m_ctrlGrabMode.AddString("0. Type1 : UHD - Non Division");
		m_ctrlGrabMode.AddString("1. Type2 : UHD - 2 Division");
		m_ctrlGrabMode.AddString("2. Type3 : UHD - 4 Division");
		m_ctrlGrabMode.AddString("3. Type4 : UHD - 8 Division");
		m_ctrlGrabMode.AddString("4. Type5 : 16Y_EPI(80P)");
		m_ctrlGrabMode.AddString("5. Type6 : FHD(120), etc.");
		m_ctrlGrabMode.AddString("6. Type7 : ..");
		m_ctrlGrabMode.AddString("7. Type8 : HD, FHD(60), etc.");
		m_ctrlGrabMode.AddString("8. Type9 : 16Y_EPI(60P)");
		m_ctrlGrabMode.AddString("9. Type10 : 16Y_EPI(60P_White)");
		m_ctrlGrabMode.AddString("10.Type11 : 18YEPI(60P / RGBW / 6Nane)");
		m_ctrlGrabMode.AddString("11. Type12 : 20YEPI 60P CEDS");
		m_ctrlGrabMode.AddString("12. Type13 : 21Y50 68P HKC CEDS UPQO VA");
		m_ctrlGrabMode.AddString("13. Type14 : 21Y70 68P SharpCEDS DPT UPQO");		
		m_ctrlGrabMode.AddString("14. Type15 : HKC 966 Pixel");
		m_ctrlGrabMode.AddString("15. Type16 : CSOT 68P 55UP77");
		m_ctrlGrabMode.AddString("16. Type17 : 8K T-CONLESS");
		m_ctrlGrabMode.AddString("17. Type18 : LM21A HKC 220307");
		m_ctrlGrabMode.AddString("18. Type19 : QHD_FULL_SPEED");
		m_ctrlGrabMode.AddString("19. Type20 : Shuffle 4");
		m_ctrlGrabMode.AddString("20. Type21 : 32ch Vx1 MNT Pack");
		m_ctrlGrabMode.AddString("21. Type22 : 32ch Vx1 MNT Pack2");

		//m_ctrlGrabMode.AddString(" 16. Type17 : Y20_SW_Mode_02");
		//m_ctrlGrabMode.AddString(" 17. Type18 : Y20_SW_Mode_03");
		m_ctrlGrabMode.SetCurSel(CurrentSet->nUHD_Grab_Mode);

		CString Stemp;
		m_cComboY20SwMode.ResetContent();
		for (int i = 0; i < 8; i++)
		{
			Stemp.Format(_T("SW_Mode_%02d"), i);
			m_cComboY20SwMode.AddString(Stemp);
		}
		for (int i = 0; i < 8; i++)
		{
			Stemp.Format(_T("SW_170_%02d"), i);
			m_cComboY20SwMode.AddString(Stemp);
		}
		m_cComboY20SwMode.SetCurSel(CurrentSet->nUHD_Y20_SW_Mode);




		m_ctrlGrabDelay.SetCurSel(CurrentSet->nUHD_Grab_Delay);
		m_ctrlHdmiGrabResolution.SetCurSel(CurrentSet->nHDMI_Grab_Resolution);
		m_ctrlHdmiBitShift.SetCurSel(CurrentSet->nHDMI_Grab_BitShift);
		m_bMaskOsd = CurrentSet->bHdmiGrab_MaskOsd;
	}
	else{//FHD
		InitItem(FALSE);

		switch((CurrentSet->nUranousMode) & 0xCF) 
		{
			case JEIDA_DUAL6	: m_nUranousMode = 0; m_nBitMode = 0;	break;
			case JEIDA_DUAL8	: m_nUranousMode = 0; m_nBitMode = 1;	break;
			case JEIDA_DUAL10	: m_nUranousMode = 0; m_nBitMode = 2;	break;
			case JEIDA_DUAL12	: m_nUranousMode = 0; m_nBitMode = 3;	break;

			case JEIDA_SINGLE6	: m_nUranousMode = 1; m_nBitMode = 0;   break;
			case JEIDA_SINGLE8	: m_nUranousMode = 1; m_nBitMode = 1;	break;
			case JEIDA_SINGLE10 : m_nUranousMode = 1; m_nBitMode = 2;	break;
			case JEIDA_SINGLE12 : m_nUranousMode = 1; m_nBitMode = 3;	break;
		
			case NS_DUAL6		: m_nUranousMode = 2; m_nBitMode = 0;	break;
			case NS_DUAL8		: m_nUranousMode = 2; m_nBitMode = 1;	break;
			case NS_DUAL10		: m_nUranousMode = 2; m_nBitMode = 2;	break;
			case NS_DUAL12		: m_nUranousMode = 2; m_nBitMode = 3;	break;

			case NS_SINGLE6		: m_nUranousMode = 3; m_nBitMode = 0;	break;
			case NS_SINGLE8		: m_nUranousMode = 3; m_nBitMode = 1;	break;
			case NS_SINGLE10	: m_nUranousMode = 3; m_nBitMode = 2;	break;
			case NS_SINGLE12	: m_nUranousMode = 3; m_nBitMode = 3;	break;
		}

		m_bOddDE_Only = CurrentSet->bOddDE_Only;
		m_n3DMode = CurrentSet->n3DGrabbingMode;
		if(g_pView->m_nGrabber_FWVer < 0x10102301)
		{
			GetDlgItem(IDC_STATIC_3D_MODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RDO_3D_MODE1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RDO_3D_MODE2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RDO_3D_MODE3)->EnableWindow(FALSE);
		}	
	}

	m_nHdmiOutControl = 0;// CurrentSet->bHdmiOutControl;
	m_ctrlHdmi1Port.SetCurSel(CurrentSet->nHDMI1_Port -1);
	m_ctrlHdmi2Port.SetCurSel(CurrentSet->nHDMI2_Port -1);
	m_ctrlHdmi3Port.SetCurSel(CurrentSet->nHDMI3_Port -1);
	m_ctrlHdmi4Port.SetCurSel(CurrentSet->nHDMI4_Port -1);

	m_nHdmiCecControl = CurrentSet->bHdmiCecControl;
	m_nHdmiEdidControl = CurrentSet->bHdmiEdidControl;
	m_nHdmiHdcpControl = CurrentSet->bHdmiHdcpControl;

	m_nCompResolution = CurrentSet->nCompResolution;
	//m_bEpiPAckReset = CurrentSet->bEpiPAckReset;
	//m_bGrabBaseReset = CurrentSet->bGrabBaseReset;

	UpdateData(FALSE);

//	OnRadioHdmiOutControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptGrabberPage::OnBtnGrabberOptDelete() 
{
	UpdateData(TRUE);

	g_pView->ShowSystemMsg(DATA_DELETE_PROCESSING_MSG);

	ResultData_Delete((LONG)m_nPeriod);

	g_pView->CloseSystemMsg();
}

//+add kwmoon 080415
void COptGrabberPage::MakeModelFolderList(CString szModuleFolder,CStringArray& aModelFoler)
{
	CFileFind find;

	BOOL bFind		 = FALSE;
	CString szErrMsg = _T("");
	CString szModelFolderName = szModuleFolder + "\\*.*";

	if(!(bFind = find.FindFile(szModelFolderName)))
	{
		szErrMsg.Format("Failed to find <%d> folder!",szModuleFolder); 
		AfxMessageBox(szErrMsg); return;
	}  

	while(bFind)
	{
		bFind = find.FindNextFile();

		szModelFolderName = find.GetFileName();
		
		if (find.IsDirectory())
		{
			if (!find.IsDots())
			{
				szModelFolderName = find.GetFileName();

				aModelFoler.Add(szModelFolderName);
			}
		}
	}
	find.Close();
}

//+add kwmoon 080415
BOOL COptGrabberPage::DeleteFolder(CString szGrabFolderPath) 
{
	CFileFind find;

	BOOL bFind			 = FALSE;
	BOOL bSuccess		 = FALSE;
	
	CString szErrMsg	 = _T("");
	CString szFileName	 = _T("");
	CString szFilePath	 = _T("");

	CString szFolderName = _T("");
	CString szFolderPath = _T("");
	CString szGrabFolderFile = szGrabFolderPath + "\\*.*";

	DWORD dwErrCode		= 0;

//	szGrabFolderPath	+= "\\";

	if((bFind = find.FindFile(szGrabFolderFile)))
	{	
		while(bFind)
		{
			bFind = find.FindNextFile();
		
			szFileName = find.GetFileName();
			szFilePath = szGrabFolderPath + "\\" + szFileName;
	
			if(!find.IsDirectory())
			{
			//	m_pProgressMsgDlg->ShowMessage2(szFileName);

				bSuccess = ::DeleteFile((LPCTSTR)szFilePath);
			}
			else
			{
				if((szFileName != ".") && (szFileName != ".."))
				{
					DeleteFolder(szFilePath); 
				}
			}
		}	
		find.Close();

		if(!(bSuccess = ::RemoveDirectory((LPCTSTR)szGrabFolderPath)))
		{
			dwErrCode = GetLastError();
		}
		return TRUE;
	}  
	find.Close();
	return FALSE;
}

//+add kwmoon 080502
void COptGrabberPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnGrabberOptApply();

	CPropertyPage::OnOK();
}

//+add kwmoon 080904
void COptGrabberPage::OnBtnSave() 
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
	
	OnBtnGrabberOptApply();

	//+move kwmoon 080904
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
}

void COptGrabberPage::OnSelchangeComboPeriod() 
{
	int nTemp;

	nTemp = m_ctrlPeriod.GetCurSel();

	m_nPeriod = (nTemp + 1) * 15;
}

	//+add psh 100310
void COptGrabberPage::OnHdmiResolution1() 
{
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(TRUE);

	GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(FALSE);
}

void COptGrabberPage::OnHdmiResolution2() 
{
//	m_ctrlHDMI_BitSpec.SetCurSel(0);
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(FALSE);

	GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(FALSE);
	}

void COptGrabberPage::OnHdmiResolution3() 
{
//	m_ctrlHDMI_BitSpec.SetCurSel(0);
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(FALSE);

	GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(FALSE);
}

void COptGrabberPage::OnHdmiResolution4() 
{
//	m_ctrlHDMI_BitSpec.SetCurSel(0);
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(FALSE);

	GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(TRUE);
	
	GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(TRUE);
}
void COptGrabberPage::InitItem(BOOL bFlag)
{
	//GetDlgItem(IDC_RDO_LVDS_FORMAT13)->EnableWindow(bFlag);
	//GetDlgItem(IDC_RDO_LVDS_FORMAT14)->EnableWindow(bFlag);
	//GetDlgItem(IDC_RDO_LVDS_FORMAT15)->EnableWindow(bFlag);
	//GetDlgItem(IDC_RDO_LVDS_FORMAT16)->EnableWindow(bFlag);

	GetDlgItem(IDC_COMBO_LVDSMODE)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_UHDTYPE)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_BITSHIFT)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_GRABMODE)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_GRABDELAY)->EnableWindow(bFlag);


	GetDlgItem(IDC_COMBO_HDMITYPE)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_HDMIBITSHIFT)->EnableWindow(bFlag);
	GetDlgItem(IDC_CHECK_MASK_OSDREGION)->EnableWindow(bFlag);

//	GetDlgItem(IDC_HDMI_RESOLUTION1)->EnableWindow(bFlag);
//	GetDlgItem(IDC_HDMI_RESOLUTION2)->EnableWindow(bFlag);
//	GetDlgItem(IDC_HDMI_RESOLUTION3)->EnableWindow(bFlag);
	GetDlgItem(IDC_HDMI_RESOLUTION4)->EnableWindow(bFlag);
//	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(bFlag);

	GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(bFlag);
	GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(bFlag);

	GetDlgItem(IDC_RDO_URANOUS_MODE1)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_URANOUS_MODE2)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_URANOUS_MODE3)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_URANOUS_MODE4)->EnableWindow(!bFlag);

	GetDlgItem(IDC_CHK_ODD_DE_ONLY)->EnableWindow(!bFlag);

	GetDlgItem(IDC_RDO_3D_MODE1)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_3D_MODE2)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_3D_MODE3)->EnableWindow(!bFlag);

	GetDlgItem(IDC_RDO_RGB_RES_1)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_RGB_RES_2)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_RGB_RES_3)->EnableWindow(!bFlag);
	GetDlgItem(IDC_RDO_RGB_RES_4)->EnableWindow(!bFlag);

}


void COptGrabberPage::OnBnClickedChkRbSwap()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

#if 0
void COptGrabberPage::OnBnClickedButton170Write()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int  lRet;
	CurrentSet->nUHD_Grab_Mode = m_ctrlGrabMode.GetCurSel();
	CurrentSet->nUHD_Y20_SW_Mode = m_cComboY20SwMode.GetCurSel();
	__int64 SW_Mode_Sel;
	//if (CurrentSet->nUHD_Grab_Mode == 14) //if ((CurrentSet->nUHD_Grab_Mode >= 14) && (CurrentSet->nUHD_Grab_Mode <= 17)) //201109 HKC_966_PIXEL
	{
		if ((CurrentSet->nUHD_Y20_SW_Mode >= 0) && (CurrentSet->nUHD_Y20_SW_Mode <= 7)) // Y20_SW_Mode_01
			SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode;
		else
			SW_Mode_Sel = 0x00000001;

		lRet = g_pView->m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
		Sleep(1); // SW_Mode_Sel
	}
	if (lRet == 0)
	{
		CString stemp;
		stemp.Format(_T("CM_RegisterWrite 0x0170: 0x%08X"), SW_Mode_Sel);
		MessageBox(stemp);
	}
	else
	{
		MessageBox("Write 0x170: Fail!!");
	}
	//{
	//	MessageBox("nUHD_Grab_Mode != HKC 966 Pixel[14]");
	//}
}
#else
void COptGrabberPage::OnBnClickedButton170Write()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int  lRet;
	__int64 SW_Mode_Sel;
	int lSelID = 0;
	CString strTmp;
	//CurrentSet->nUHD_Grab_Mode = m_ctrlGrabMode.GetCurSel();
	lSelID =  m_cComboY20SwMode.GetCurSel();
//	m_cComboY20SwMode.GetLBText(lSelID, strTmp);

	//if (strTmp.Find("SW_170_") == 0)
	//{
	//	CurrentSet->nUHD_Y20_SW_Mode = _ttoi(strTmp.Mid(7));
	//}
	//else
	//{
	//	CurrentSet->nUHD_Y20_SW_Mode = lSelID;
	//}
	CurrentSet->nUHD_Y20_SW_Mode = lSelID;
	if (CurrentSet->nUHD_Y20_SW_Mode >= 8)
	{
		SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode - 8;
	}
	else
	{
		SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode;
	}
	
	//if (CurrentSet->nUHD_Grab_Mode == 14) //if ((CurrentSet->nUHD_Grab_Mode >= 14) && (CurrentSet->nUHD_Grab_Mode <= 17)) //201109 HKC_966_PIXEL
	{
		if ((SW_Mode_Sel >= 0) && (SW_Mode_Sel <= 7)) // Y20_SW_Mode_01
			SW_Mode_Sel = SW_Mode_Sel;
		else
			SW_Mode_Sel = 0x00000000;

		lRet = g_pView->m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
		Sleep(1); // SW_Mode_Sel
	}
	if (lRet == 0)
	{
		CString stemp;
		stemp.Format(_T("CM_RegisterWrite 0x0170: 0x%08X"), SW_Mode_Sel);
		MessageBox(stemp);
	}
	else
	{
		MessageBox("Write 0x170: Fail!!");
	}
	//{
	//	MessageBox("nUHD_Grab_Mode != HKC 966 Pixel[14]");
	//}
}
#endif

void COptGrabberPage::OnBnClickedButton170Read()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int  lRet;
	__int64 SW_Mode_Sel = -1;
	lRet = g_pView->m_clsPCI.CM_RegisterRead(0x0170, &SW_Mode_Sel);

	if (lRet == 0)
	{
		CString stemp;
		stemp.Format(_T("CM_RegisterRead 0x0170: 0x%08X"), SW_Mode_Sel);
		MessageBox(stemp);
	}
	else
	{
		MessageBox("Read 0x170: Fail!!");
	}
	
}


void COptGrabberPage::OnCbnSelchangeComboGrabmode()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	OnBtnGrabberOptApply();
}


void COptGrabberPage::OnCbnSelchangeComboUhdtype()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnBtnGrabberOptApply();
}
