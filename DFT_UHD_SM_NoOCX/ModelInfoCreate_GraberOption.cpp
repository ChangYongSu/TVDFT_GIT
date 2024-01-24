// ModelInfoCreate_GraberOption.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ModelInfoCreate_GraberOption.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate_GraberOption dialog
int aUHD_ItemNo[] = 
{
	IDC_STATIC_UHD_OPTION,	IDC_STATIC_LVDSMODE2,	IDC_COMBO_LVDSMODE,
	IDC_STATICUHD_TYPE,		IDC_COMBO_UHDTYPE,		IDC_STATIC_BIT_SHIFT,
	IDC_COMBO_BITSHIFT,		IDC_STATIC_GRAB_MODE,	IDC_COMBO_GRABMODE,
	IDC_STATIC_HDMI_OPTION,	IDC_STATIC_HDMI_RESOLUTION,	IDC_COMBO_HDMITYPE,
	IDC_STATIC_HDMIBITSHIFT,IDC_COMBO_HDMIBITSHIFT,	IDC_STATIC_HDMI_GEN,
	IDC_HDMI_RESOLUTION1,	IDC_STATIC_HDMI_SPEC,	IDC_HDMI_BIT_SPEC,
	IDC_STATIC_HDMI_SPEC2,	IDC_HDMI_RESOLUTION2,	IDC_HDMI_RESOLUTION3,
	IDC_HDMI_RESOLUTION4,	IDC_STATIC_HDMI5,		IDC_COMBO_HDMI5,
	IDC_STATIC_HDMI6,		IDC_COMBO_HDMI6,		IDC_STATIC_HDMI7,
	IDC_COMBO_HDMI7,		IDC_STATIC_HDMI8,		IDC_COMBO_HDMI8
};

int aFHD_ItemNo[] = 
{
	IDC_STATIC_LVDS_MODE,	IDC_RDO_URANOUS_MODE1,	IDC_RDO_URANOUS_MODE2,
	IDC_RDO_URANOUS_MODE3,	IDC_RDO_URANOUS_MODE4,	IDC_CHK_ODD_DE_ONLY,
	IDC_STATIC_3D_MODE,		IDC_RDO_3D_MODE1,		IDC_RDO_3D_MODE2,
	IDC_RDO_3D_MODE3,		IDC_STATIC_COLOR_DEPTH,	IDC_RDO_RGB_RES_1,
	IDC_RDO_RGB_RES_2,		IDC_RDO_RGB_RES_3,		IDC_RDO_RGB_RES_4
};


CModelInfoCreate_GraberOption::CModelInfoCreate_GraberOption(CWnd* pParent /*=NULL*/)
	: CDialog(CModelInfoCreate_GraberOption::IDD, pParent)
	, m_bScanNotUse(FALSE)
	, m_bRedBlueSwap(FALSE)
	, m_bDeHsyncEnable(FALSE)
	, m_bHyncEdgeEnable(FALSE)
{
	//{{AFX_DATA_INIT(CModelInfoCreate_GraberOption)
	m_nUranousMode = -1;
	m_nLvdsFormat = -1;
	m_nAnalogFormat = -1;
	m_nBitMode = -1;
	m_ctlTVControlMode = 0;
#ifdef SM_MODIFY_CODE__
	m_bAutoSourceControl = TRUE;
	m_bAutoPatternControl = TRUE;
	m_bAutoAVSwitchBoxControl = TRUE;
#else
	m_bAutoSourceControl = FALSE;
	m_bAutoPatternControl = FALSE;
	m_bAutoAVSwitchBoxControl = FALSE;

#endif
	m_bScanNotUse = FALSE;
	m_bImage_Rotation = FALSE;
	m_bOddDE_Only = FALSE;
	
	m_n3DMode = -1;
	m_nHDMI_Resolution = -1;
	m_nCompResolution = -1;
	//}}AFX_DATA_INIT
}


void CModelInfoCreate_GraberOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelInfoCreate_GraberOption)
	DDX_Control(pDX, IDC_COMBO_LVDSMODE, m_ctrlLvdsMode);
	DDX_Control(pDX, IDC_COMBO_HDMI8, m_ctrlHdmi4Model);
	DDX_Control(pDX, IDC_COMBO_HDMI7, m_ctrlHdmi3Model);
	DDX_Control(pDX, IDC_COMBO_HDMI6, m_ctrlHdmi2Model);
	DDX_Control(pDX, IDC_COMBO_HDMI5, m_ctrlHdmi1Model);
	DDX_Control(pDX, IDC_HDMI_BIT_SPEC, m_ctrlHDMI_BitSpec);
	DDX_Control(pDX, IDC_COMBO_HDMIBITSHIFT, m_ctrlHdmiBitShift);
	DDX_Control(pDX, IDC_COMBO_HDMITYPE, m_ctrlHdmiResolution);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_TV2, m_ctrlTVBaudCmb);
	DDX_Control(pDX, IDC_COMBO_UHDTYPE, m_ctrlUhdType);
	DDX_Control(pDX, IDC_COMBO_GRABMODE, m_ctrlGrabMode);
	DDX_Control(pDX, IDC_COMBO_BITSHIFT, m_ctrlBitShift);
	DDX_Radio(pDX, IDC_RDO_URANOUS_MODE1, m_nUranousMode);
	DDX_Radio(pDX, IDC_RDO_LVDS_FORMAT1, m_nLvdsFormat);
	DDX_Radio(pDX, IDC_RDO_ANALOG_FORMAT1, m_nAnalogFormat);
	DDX_Radio(pDX, IDC_RDO_RGB_RES_1, m_nBitMode);
	DDX_Radio(pDX, IDC_RADIO_TV_CONTROL_RS232C, m_ctlTVControlMode);
	DDX_Check(pDX, IDC_CHK_AUTO_SOURCE_CONTROL, m_bAutoSourceControl);
	DDX_Check(pDX, IDC_CHK_AUTO_PATTERN_CONTROL, m_bAutoPatternControl);
	DDX_Check(pDX, IDC_CHK_IMAGE_ROTATION, m_bImage_Rotation);
	DDX_Check(pDX, IDC_CHK_ODD_DE_ONLY, m_bOddDE_Only);
	DDX_Check(pDX, IDC_CHK_AUTO_AVSWBOX_CONTROL, m_bAutoAVSwitchBoxControl);
	DDX_Radio(pDX, IDC_RDO_3D_MODE1, m_n3DMode);
	DDX_Radio(pDX, IDC_HDMI_RESOLUTION1, m_nHDMI_Resolution);
	DDX_Radio(pDX, IDC_RADIO_COMP_RESOLUTION1, m_nCompResolution);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_SCAN_NO_USE, m_bScanNotUse);
	DDX_Control(pDX, IDC_COMBO_HDMI_ARC_PORT_NUMBER, m_nARC_Force_Port_Number);
	DDX_Check(pDX, IDC_CHK_RB_SWAP, m_bRedBlueSwap);
	DDX_Check(pDX, IDC_CHK_DE_HSYNC, m_bDeHsyncEnable);
	DDX_Check(pDX, IDC_CHK_HSYNC_EDGE, m_bHyncEdgeEnable);
}


BEGIN_MESSAGE_MAP(CModelInfoCreate_GraberOption, CDialog)
	//{{AFX_MSG_MAP(CModelInfoCreate_GraberOption)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION1, OnHdmiResolution1)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION2, OnHdmiResolution2)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION3, OnHdmiResolution3)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION4, OnHdmiResolution4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate_GraberOption message handlers

void CModelInfoCreate_GraberOption::OnOK() 
{
	UpdateData(TRUE);
	
	CString sMsg = _T("");

	int nTmp = 0;

	CurrentSet->nImageRotation = 0;

	if(g_nGrabberType == FHD_GRABBER){
		CurrentSet->bOddDE_Only = FALSE;
	
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

	switch(m_nLvdsFormat) 
	{
		case W_1024_H_768: 
			CurrentSet->nLvdsWidth = 1024; CurrentSet->nLvdsHeight = 768;			break;
		case W_1280_H_768: 
			CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 768;			break;
		case W_1280_H_1024: 
			CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 1024;			break;
		case W_1366_H_768: 
			CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;			break;
		case W_1440_H_900: 
			CurrentSet->nLvdsWidth = 1440; CurrentSet->nLvdsHeight = 900;			break;
		case W_1600_H_900: 
			CurrentSet->nLvdsWidth = 1600; CurrentSet->nLvdsHeight = 900;			break;
		case W_1680_H_1050: 
			CurrentSet->nLvdsWidth = 1680; CurrentSet->nLvdsHeight = 1050;			break;
		
		case W_1920_H_1080_60: 
		{
			CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;			
			CurrentSet->nFullHd120Hz = 0;
			if(g_nGrabberType == FHD_GRABBER){
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			}
		}
		break;
		
		case W_1920_H_1080_120: 
		{
			CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;			
			CurrentSet->nFullHd120Hz = 1;
			if(g_nGrabberType == FHD_GRABBER){
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				// add psh 110209
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			}
		}
		break;

		case W_2560_H_1080: 
		{
			CurrentSet->nLvdsWidth = 2560; CurrentSet->nLvdsHeight = 1080;			
			CurrentSet->nFullHd120Hz = 1;
			if(g_nGrabberType == FHD_GRABBER){
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			}
		}
		break;
		
		case W_1364_H_768: 
		{	
			CurrentSet->nLvdsWidth = 1364; CurrentSet->nLvdsHeight = 768;	
			if(g_nGrabberType == FHD_GRABBER){
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 0))
				{
					nTmp = nTmp | 0x10;
				}
			}
		}
		break;

		case W_2560_H_1440: 
			CurrentSet->nLvdsWidth = 2560; CurrentSet->nLvdsHeight = 1440;			
			CurrentSet->nFullHd120Hz = 1;
			break;

		case W_3440_H_1440: 
			CurrentSet->nLvdsWidth = 3440; CurrentSet->nLvdsHeight = 1440;			break;

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
		case W_3328_H_1440:
			CurrentSet->nLvdsWidth = 3328;
			CurrentSet->nLvdsHeight = 1440;
			break;

		default : CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
	}

	switch(m_nAnalogFormat) 
	{
		case 0: CurrentSet->nAnalogWidth = 640; CurrentSet->nAnalogHeight = 480; CurrentSet->nAnalogType = V_SRC_NTSC;	break;
		case 1: CurrentSet->nAnalogWidth = 704; CurrentSet->nAnalogHeight = 572; CurrentSet->nAnalogType = V_SRC_PAL;	break;
		case 2: CurrentSet->nAnalogWidth = 704; CurrentSet->nAnalogHeight = 572; CurrentSet->nAnalogType = V_SRC_SECAM;	break;
		default: CurrentSet->nAnalogWidth = 640; CurrentSet->nAnalogHeight = 480;	 CurrentSet->nAnalogType = V_SRC_NTSC;	break;
	}

	if(g_nGrabberType == FHD_GRABBER){
		if((g_pView->m_nGrabber_FWVer >= 0x10102301) &&	(CurrentSet->nFullHd120Hz ==0))
		{
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

	CurrentSet->nImageRotation = m_bImage_Rotation;
	CurrentSet->nRedBlueSwap = m_bRedBlueSwap;
	CurrentSet->nDeHsyncEnable = m_bDeHsyncEnable;
	CurrentSet->nHyncEdgeEnable = m_bHyncEdgeEnable;


	CurrentSet->nGrabCheckArea = 7;
	CurrentSet->nAudioCheckMethod = METHOD_SOUNDCARD;

	//============
	// I2C Option
	//============
	CurrentSet->bUseI2cAdc = 1;
	CurrentSet->nI2cClkSpeed = 1;
	CurrentSet->nI2cPacketDelay = 150;
	CurrentSet->nI2cReadByteDelay =  1;
	CurrentSet->nI2cWriteByteDelay = 1;



	
	//==================
	// Tool/Area Option
	//==================
	CurrentSet->sToolOption1 = "";
	CurrentSet->sToolOption2 = "";
	CurrentSet->sToolOption3 = "";
	CurrentSet->sToolOption4 = "";
	CurrentSet->sToolOption5 = "";
	CurrentSet->sToolOption6 = "";
	CurrentSet->sToolOption7 = "";
	CurrentSet->sToolOption8 = "";
	CurrentSet->sCommercialOption1 = "";
	CurrentSet->sToolCRC	= "";
	
	CurrentSet->sAreaOption1 = "";
	CurrentSet->sCountryGroup = "";
	CurrentSet->sCountry = "";
	
	CurrentSet->sCPUVersion = "";
	CurrentSet->sMicomVersion = "";
	CurrentSet->sUSBVersion = "";
	CurrentSet->sMNTVersion = "";
	
	CurrentSet->sEDID_HDMI1		= "";  	
	CurrentSet->sEDID_HDMI2		= "";  	
	CurrentSet->sEDID_HDMI3		= "";  	
	CurrentSet->sEDID_HDMI4		= "";  	
	CurrentSet->sEDID_HDMI5		= "";  	
	CurrentSet->sEDID_DP		= "";  	        
	CurrentSet->sEDID_DP2		= "";  	        
	CurrentSet->sEDID_DVI		= "";  	        
	CurrentSet->sEDID_PC		= "";  	        
	CurrentSet->sEDID_RGB		= "";  	        
	
	CurrentSet->sCI				= ""; 		
	CurrentSet->sCNTV			= ""; 		
	CurrentSet->sDVBS			= ""; 		
	CurrentSet->sDVBS2			= ""; 		
	CurrentSet->sESN			= ""; 		
	CurrentSet->sDTCP_RX		= ""; 	        
	CurrentSet->sHDCP_RX		= ""; 	        
	CurrentSet->sHDCP_RX_14		= ""; 	
	CurrentSet->sHDCP_TX		= ""; 	        
	CurrentSet->sWASU			= ""; 		
	CurrentSet->sADC			= ""; 		
	CurrentSet->sWIDEVINE		= ""; 	        
	CurrentSet->sMARINKEY		= ""; 	        
	CurrentSet->sBUILT_WIFI		= ""; 	
	CurrentSet->sJAPAN_LNB		= ""; 	
	CurrentSet->sMAC_ADDRESS	= ""; 	
	CurrentSet->sCA				= ""; 		
	CurrentSet->sMODEL_NAME		= ""; 	
	CurrentSet->sSERIAL_NO = "";
	CurrentSet->sASTC30_Key = "";
	CurrentSet->sMfi_Key = "";

	
	DWORD wBaudRate;
	int nCurSel = m_ctrlTVBaudCmb.GetCurSel();
	switch(nCurSel) 
	{
		case 0: wBaudRate = CBR_9600;	break;
		case 1: wBaudRate = CBR_19200;	break;
		case 2: wBaudRate = CBR_38400;	break;
		case 3: wBaudRate = CBR_57600;	break;
		case 4: wBaudRate = CBR_115200;	break;
		default: wBaudRate = CBR_9600; break;
	}
	CurrentSet->wTVBaudRate = wBaudRate;


	//============
	// TV Control
	//============
	CurrentSet->nTVControlType = m_ctlTVControlMode;
	CurrentSet->bAutoSourceControl = m_bAutoSourceControl;
	CurrentSet->bAutoPatternControl = m_bAutoPatternControl;

	CurrentSet->bScanNotUse =   m_bScanNotUse;
	if(m_bAutoAVSwitchBoxControl){
		CurrentSet->nAutoAVSwitchBoxControl = 1;
	}
	else{
		CurrentSet->nAutoAVSwitchBoxControl = 2;
	}

	if(g_nGrabberType == UHD_GRABBER){
		CurrentSet->nLVDS_Mode = m_ctrlLvdsMode.GetCurSel();
		CurrentSet->nUHD_Type = m_ctrlUhdType.GetCurSel();
		CurrentSet->nUHD_Grab_BitShift = m_ctrlBitShift.GetCurSel();
		CurrentSet->nUHD_Grab_Mode = m_ctrlGrabMode.GetCurSel();

		CurrentSet->nHDMI_Grab_Resolution = m_ctrlHdmiResolution.GetCurSel();
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

		CurrentSet->nHDMIResolution = m_nHDMI_Resolution;
		CurrentSet->nHDMIBitSpec = m_ctrlHDMI_BitSpec.GetCurSel();
		CurrentSet->nCompResolution = m_nCompResolution;
		CurrentSet->nHDMI1_Model = m_ctrlHdmi1Model.GetCurSel();
		CurrentSet->nHDMI2_Model = m_ctrlHdmi2Model.GetCurSel();
		CurrentSet->nHDMI3_Model = m_ctrlHdmi3Model.GetCurSel();
		CurrentSet->nHDMI4_Model = m_ctrlHdmi4Model.GetCurSel();
		CurrentSet->bHdmiGrab_MaskOsd = FALSE;

		CurrentSet->nARC_Force_Port_Number = m_nARC_Force_Port_Number.GetCurSel();
		CurrentSet->nARC_Force_OnOffChannel = 2;
	}

	CDialog::OnOK();
}

BOOL CModelInfoCreate_GraberOption::OnInitDialog() 
{
	CDialog::OnInitDialog();

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
	else if((CurrentSet->nLvdsWidth == 2560) && (CurrentSet->nLvdsHeight == 1440) && (CurrentSet->nFullHd120Hz == 0)) 
	{
		m_nLvdsFormat = W_2560_H_1440;
	}
	else if((CurrentSet->nLvdsWidth == 3440) && (CurrentSet->nLvdsHeight == 1440))  m_nLvdsFormat = W_3440_H_1440;
	else if((CurrentSet->nLvdsWidth == 3840) && (CurrentSet->nLvdsHeight == 2160)) m_nLvdsFormat = W_3840_H_2160;
	else if((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 300))  m_nLvdsFormat = W_1920_H_300;
	else if((CurrentSet->nLvdsWidth == 3840) && (CurrentSet->nLvdsHeight == 600))  m_nLvdsFormat = W_3840_H_600;
	else if ((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 540))  m_nLvdsFormat = W_1920_H_540;
	else if ((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 1200))  m_nLvdsFormat = W_1920_H_1200;
	else if ((CurrentSet->nLvdsWidth == 2048) && (CurrentSet->nLvdsHeight == 2560))  m_nLvdsFormat = W_2048_H_2560;
	else if ((CurrentSet->nLvdsWidth == 3328) && (CurrentSet->nLvdsHeight == 1440))  m_nLvdsFormat = W_3328_H_1440;
	else m_nLvdsFormat = -1;

	m_nAnalogFormat = CurrentSet->nAnalogType;
	m_bAutoSourceControl = TRUE;
	m_bAutoPatternControl = TRUE;
	m_bAutoAVSwitchBoxControl = TRUE;	
	m_bScanNotUse = FALSE;
		//+change 090331
	m_ctrlTVBaudCmb.SetCurSel(4);

	if(g_nGrabberType == UHD_GRABBER){
		SetShowFHD_Item(FALSE);
		SetShowUHD_Item(TRUE);


		m_ctrlLvdsMode.SetCurSel(CurrentSet->nLVDS_Mode);
		m_ctrlUhdType.SetCurSel(CurrentSet->nUHD_Type);
		m_ctrlBitShift.SetCurSel(CurrentSet->nUHD_Grab_BitShift);
		m_ctrlGrabMode.SetCurSel(CurrentSet->nUHD_Grab_Mode);

		m_ctrlHdmiResolution.SetCurSel(CurrentSet->nHDMI_Grab_Resolution);
		m_ctrlHdmiBitShift.SetCurSel(CurrentSet->nHDMI_Grab_BitShift);


		m_nHDMI_Resolution = CurrentSet->nHDMIResolution;
		m_ctrlHDMI_BitSpec.SetCurSel(CurrentSet->nHDMIBitSpec);
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
	}
	else{
		SetShowFHD_Item(TRUE);
		SetShowUHD_Item(FALSE);


		switch(CurrentSet->nUranousMode) 
		{
			case JEIDA_DUAL6	: case (JEIDA_DUAL6  | 0x10)	: m_nUranousMode = 0; m_nBitMode = 0;	break;
			case JEIDA_DUAL8	: case (JEIDA_DUAL8  | 0x10)	: m_nUranousMode = 0; m_nBitMode = 1;	break;
			case JEIDA_DUAL10	: case (JEIDA_DUAL10 | 0x10)	: m_nUranousMode = 0; m_nBitMode = 2;	break;
			case JEIDA_DUAL12	: case (JEIDA_DUAL12 | 0x10)	: m_nUranousMode = 0; m_nBitMode = 3;	break;

			case JEIDA_SINGLE6	: case (JEIDA_SINGLE6  | 0x10)	: m_nUranousMode = 1; m_nBitMode = 0;   break;
			case JEIDA_SINGLE8	: case (JEIDA_SINGLE8  | 0x10)	: m_nUranousMode = 1; m_nBitMode = 1;	break;
			case JEIDA_SINGLE10 : case (JEIDA_SINGLE10 | 0x10)	: m_nUranousMode = 1; m_nBitMode = 2;	break;
			case JEIDA_SINGLE12 : case (JEIDA_SINGLE12 | 0x10)	: m_nUranousMode = 1; m_nBitMode = 3;	break;
		
			case NS_DUAL6		: case (NS_DUAL6  | 0x10)	: m_nUranousMode = 2; m_nBitMode = 0;	break;
			case NS_DUAL8		: case (NS_DUAL8  | 0x10)	: m_nUranousMode = 2; m_nBitMode = 1;	break;
			case NS_DUAL10		: case (NS_DUAL10 | 0x10)	: m_nUranousMode = 2; m_nBitMode = 2;	break;
			case NS_DUAL12		: case (NS_DUAL12 | 0x10)	: m_nUranousMode = 2; m_nBitMode = 3;	break;

			case NS_SINGLE6		: case (NS_SINGLE6  | 0x10)	: m_nUranousMode = 3; m_nBitMode = 0;	break;
			case NS_SINGLE8		: case (NS_SINGLE8  | 0x10)	: m_nUranousMode = 3; m_nBitMode = 1;	break;
			case NS_SINGLE10	: case (NS_SINGLE10 | 0x10)	: m_nUranousMode = 3; m_nBitMode = 2;	break;
			case NS_SINGLE12	: case (NS_SINGLE12 | 0x10)	: m_nUranousMode = 3; m_nBitMode = 3;	break;
		}


		// add 110209
		m_n3DMode = 0;
		if(g_pView->m_nGrabber_FWVer < 0x10102301)
		{
			GetDlgItem(IDC_STATIC_3D_MODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RDO_3D_MODE1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RDO_3D_MODE2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RDO_3D_MODE3)->EnableWindow(FALSE);
		}
	}
	m_nCompResolution = CurrentSet->nCompResolution;
	m_nARC_Force_Port_Number.SetCurSel(CurrentSet->nARC_Force_Port_Number);

	UpdateData(FALSE);

	return TRUE;  
}
void CModelInfoCreate_GraberOption::SetShowFHD_Item(BOOL bShow)
{
	CButton* pButton = NULL;
	int i;

	for(i = 0; i<15; i++)
	{
		pButton = (CButton*)GetDlgItem(aFHD_ItemNo[i]);
		pButton->ShowWindow(bShow);
	} 
}

void CModelInfoCreate_GraberOption::SetShowUHD_Item(BOOL bShow)
{
	CButton* pButton = NULL;
	int i;

	for(i = 0; i<30; i++)
	{
		pButton = (CButton*)GetDlgItem(aUHD_ItemNo[i]);
		pButton->ShowWindow(bShow);
	} 
}

void CModelInfoCreate_GraberOption::OnHdmiResolution1() 
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

void CModelInfoCreate_GraberOption::OnHdmiResolution2() 
{
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

void CModelInfoCreate_GraberOption::OnHdmiResolution3() 
{
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);

	GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(FALSE);
}

void CModelInfoCreate_GraberOption::OnHdmiResolution4() 
{
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);

	GetDlgItem(IDC_STATIC_HDMI5)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI6)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI7)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI8)->EnableWindow(TRUE);
	
	GetDlgItem(IDC_COMBO_HDMI5)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_HDMI6)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_HDMI7)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_HDMI8)->EnableWindow(TRUE);
}
