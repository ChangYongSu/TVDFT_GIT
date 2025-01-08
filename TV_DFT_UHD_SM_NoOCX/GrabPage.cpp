// GrabPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "GrabPage.h"
#include "ImageTotalDlg.h"
#include "RemoteDlg.h"
//#include <mil.h> 
#include <malloc.h>
//+add kwmoon 080618
#include "RealMeas.h"
#include "global.h"
#include "AvConvertorCtrl.h"
#include "TVCommCtrl.h"
#include "JigCtl.h"
#include "PLC_Ctrl.h"
#include "VF1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;
//extern BYTE GrabData[2560][1080][3];
extern CBaseImage	g_GrabDisplayImage;
extern CBaseImage	g_GrabImage;
extern HWND g_hCommWnd;
extern CImageProc		g_ImageProc;


#define _DPMSWAIT_ID 5
//////////////////////////////////////////////////////////////////////////
#define MANUAL_GRAB		0
#define AUTO_GRAB		1

#define GRAB_STOP		0
#define GRAB_START		1

#define NO_AVSWITCHBOX_TAB_ITEM	21  //19
#define NO_AVCONVERTOR_TAB_ITEM	7 // 7  //19
#define NO_LAN_TAB_ITEM	4 // 7  //19


//+change kwmoon 080923
// #define NO_GRAB_OPTION_TAB_ITEM 21
//+change psh 091214
//#define NO_GRAB_OPTION_TAB_ITEM 23
//#define NO_GRAB_OPTION_TAB_ITEM 24
//#define NO_GRAB_OPTION_TAB_ITEM 28
//#define NO_GRAB_OPTION_TAB_ITEM 41
//#define NO_GRAB_OPTION_TAB_ITEM 48
#define NO_GRAB_OPTION_TAB_ITEM 35 // 54 //53 // 52 //51 // 50

#define NO_USB_DIO_TAB_ITEM 28//26//21//15

/////////////////////////////////////////////////////////////////////////////
// CGrabPage dialog

int aAvSwitchBoxCtrlItemNo[] = 
{
	IDC_RDO_AVSWITCH_CMD1,IDC_RDO_AVSWITCH_CMD2,IDC_RDO_AVSWITCH_CMD3,
	IDC_RDO_AVSWITCH_CMD4,IDC_RDO_AVSWITCH_CMD5,IDC_RDO_AVSWITCH_CMD6,
	IDC_RDO_AVSWITCH_CMD7,IDC_RDO_AVSWITCH_CMD8,IDC_RDO_AVSWITCH_CMD9,
	IDC_RDO_AVSWITCH_CMD10,IDC_RDO_AVSWITCH_CMD11,IDC_RDO_AVSWITCH_CMD12,
	IDC_RDO_AVSWITCH_CMD13,IDC_RDO_AVSWITCH_CMD14,
	IDC_STATIC_AVSWITCH_VALUE_NAME1,IDC_STATIC_AVSWITCH_VALUE_NAME2,
	IDC_CMB_AVSWITCH_VALUE1,IDC_CMB_AVSWITCH_VALUE2,
	IDC_EDIT_AVSWITCH_RETURN,IDC_BTN_SEND_AVSWITCH_CMD,
	IDC_EDIT_AVSWITCH_RESULT
};

int aAvConvertorCtrlItemNo[] = 
{
	IDC_STATIC_AVC_GR,IDC_STATIC_AVC_INPUT,IDC_CMB_AVC_INPUT,
	IDC_BTN_AVC_COM,IDC_BTN_AVC_IR,IDC_BTN_AVC_BUF,IDC_EDIT_AVC_OUT,
	IDC_STATIC_LAN_GR,IDC_EDIT_LAN_OUT,IDC_BTN_LAN_TEST,IDC_BTN_LAN_CLEAR
};
int aLAN_CheckerItemNo[] =
{
	IDC_STATIC_LAN_GR,IDC_EDIT_LAN_OUT,IDC_BTN_LAN_TEST,IDC_BTN_LAN_CLEAR
};
int aGrabOptionItemNo[] =
{
	IDC_STATIC_RESOLUTION, 		/*IDC_RDO_LVDS_RESOLUTION1,	IDC_RDO_LVDS_RESOLUTION2, 
	IDC_RDO_LVDS_RESOLUTION3,	IDC_RDO_LVDS_RESOLUTION4,	IDC_RDO_LVDS_RESOLUTION5, 
	IDC_RDO_LVDS_RESOLUTION6,	IDC_RDO_LVDS_RESOLUTION7,	IDC_RDO_LVDS_RESOLUTION8, 
	IDC_RDO_LVDS_RESOLUTION9,	IDC_RDO_LVDS_RESOLUTION10,	IDC_RDO_LVDS_RESOLUTION11, 
	IDC_RDO_LVDS_RESOLUTION12,	IDC_RDO_LVDS_RESOLUTION13,	IDC_RDO_LVDS_RESOLUTION14,
	IDC_RDO_LVDS_RESOLUTION15,	IDC_RDO_LVDS_RESOLUTION16,  IDC_RDO_LVDS_RESOLUTION17,
	IDC_RDO_LVDS_RESOLUTION18,  IDC_RDO_LVDS_RESOLUTION19,	IDC_RDO_LVDS_RESOLUTION20,*/
	IDC_COMBO_LVDS_RESOLUTION2,

	IDC_STATIC_LVDS_TYPE,
	IDC_RDO_LVDS_MODE1,			IDC_RDO_LVDS_MODE2,			IDC_RDO_LVDS_MODE3,
	IDC_RDO_LVDS_MODE4,			IDC_STATIC_COLOR_DEPTH,		IDC_RDO_COLOR_DEPTH1,
	IDC_RDO_COLOR_DEPTH2,		IDC_RDO_COLOR_DEPTH3,		IDC_RDO_COLOR_DEPTH4,
	IDC_STATIC_3D_MODE,			IDC_RDO_3D_MODE1,			IDC_RDO_3D_MODE2,
	IDC_RDO_3D_MODE3,			IDC_CHK_ODD_DE_ONLY,		IDC_CHK_IMAGE_ROTATION,
	IDC_BTN_GRAB_OPTION_APPLY,	

	IDC_STATIC_UHD_OPTION,		IDC_STATIC_UHD_BITSHIFT,	IDC_COMBO_BITSHIFT2,		
	IDC_STATIC_GRABMODE,		IDC_COMBO_GRABMODE2,		IDC_STATIC_Y20_SW_MODE, 
	IDC_COMBO_Y20_SW_MODE,		IDC_STATIC_UHD_TYPE,
	IDC_COMBO_UHDTYPE2,			IDC_STATIC_HDMI_OPTION,		IDC_STATIC_HDMI_TYPE,
	IDC_COMBO_HDMITYPE2, 		IDC_STATIC_HDMI_BITSHIFT,	IDC_COMBO_HDMIBITSHIFT2,
	IDC_STATIC_LVDSMODE,		IDC_COMBO_LVDSMODE
};

int aUsbDioItemNo[NO_USB_DIO_TAB_ITEM] =
{
	IDC_STATIC_RELAY, IDC_CHECK_RELAY1, IDC_STATIC_MHL, IDC_RADIO_MHL, IDC_RADIO_MHL2,IDC_STATIC_LABEL_MHL,
	IDC_STATIC_LABEL_MHL2, IDC_CHECK_MHL_LINE, IDC_STATIC_ARC, IDC_CHECK_ARC, IDC_COMBO_OUT_PORT2,
	IDC_STATIC_JIG_UP_DOWN,IDC_BTN_MAIN_JIG_UP,IDC_BTN_MAIN_JIG_DOWN,
	IDC_STATIC_HDMI_USB_CYL, IDC_CHECK_HDMI_USB_CYL, IDC_STATIC_EARPHONE, IDC_CHECK_EARPHONE, IDC_CHECK_COMP, IDC_STATIC_COMP,
	IDC_STATIC_GRAB_POWER,IDC_CHECK_GRAB_POWER,IDC_STATIC_IFPACK_RESET,IDC_BTN_RESET_IFPACK,IDC_BTN_RESET_GRAB,IDC_BTN_RESET_GENDER,
	IDC_BTN_DPMS_CHECK,IDC_STATIC_DPMS_VALUE
};

#define NO_PLC_ROBOT_DIO_TAB_ITEM 20 

int aPLC_RoboItemNo[NO_PLC_ROBOT_DIO_TAB_ITEM] =
{
	IDC_EDIT_MODEL_NAME,
	IDC_CHECK_COMMAND_LOAD, 
	IDC_CHECK_COMMAND_UNLOAD, 
	IDC_CHECK_COMMAND_PASS, 
	IDC_CHECK_COMMAND_NG, 
	IDC_CHECK_FFC, 
	IDC_CHECK_LOAD_FINISH, 
	IDC_CHECK_UNLOAD_FINISH, 
	IDC_CHECK_FFC_FINISH, 
	IDC_CHECK_STATUS_AUTO, 
	IDC_CHECK_STATUS_MANUAL, 
	IDC_CHECK_STATUS_START, 
	IDC_CHECK_STATUS_PAUSE,
	IDC_CHECK_STATUS_STOP,
	IDC_CHECK_STATUS_WARNING,
	IDC_CHECK_STATUS_ALARM, 
	IDC_CHECK_PCB_IN, 
	IDC_CHECK_PCB_DUMMY, 
	IDC_CHECK_DATA_PCB_OK, 
	IDC_CHECK_DATA_PCB_NG
};

CGrabPage::CGrabPage(CWnd* pParent /*=NULL*/)
	: CDialog(CGrabPage::IDD, pParent)
	, m_bGrabPower(FALSE)
	, m_bCheckHdmiUsbCyl(FALSE)
	, m_bRedBlueSwap(FALSE)
	, m_bDeHsyncEnable(FALSE)
	, m_bHyncEdgeEnable(FALSE)
{
	//{{AFX_DATA_INIT(CGrabPage)
	m_nAnalogSignalType = -1;
	m_szGrabStatus = _T("");
	m_nLvdsResolutionIndex = -1;
	m_nUranousMode = -1;
	m_nBitMode = -1;
	m_bOddDE_Only = FALSE;
	m_bImage_Rotation = FALSE;
	m_n3DMode = -1;
	m_bRelay1 = FALSE;
	m_nMhlMode = -1;
	m_bHdmiGenArc = FALSE;
	m_bMhlDetect = FALSE;
	//}}AFX_DATA_INIT

	m_hPageGrabThreadKillEvent = NULL;

	m_bThreadRunning    = FALSE;
	m_bStopGrabThread	= FALSE;
//	m_bStopHDMIThread	= FALSE;
//	m_bStopAnalogThread	= FALSE;
	m_bGrabSingleFrame  = FALSE;

	m_nPatternMode		= 0;	// 0 : offLine,		1 : online
	m_nGrabMode			= 1;	// 0 : Manual Grab,	1 : Auto Grab
	m_nGrabDevice		= 0;	// 0 : LVDS Grab,	1 : VideoOut Grab
	m_nAnalogSignalType = 0;	// 0 : NTSC			1 : PAL
	m_pSystemMessageDlg = NULL;
	//+change 090213(Modification No1)
//	m_hThread			= NULL;
	m_pGrabImageThread_UHD	= NULL;
	m_pGrabImageThread_FHD	= NULL;
	m_MWnd				= NULL;
	m_nAvSwitchCmd		= -1;


	//+add kwmoon 080710
	m_nNewBitCount		= 0;
	m_nNewLvdsWidth		= 0;
	m_nNewLvdsHeight	= 0;
	m_nNewLVDSMode		= 0;

}


void CGrabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrabPage)
	DDX_Control(pDX, IDC_CHECK_COMP, m_ctrlComp);
	DDX_Control(pDX, IDC_CHECK_EARPHONE, m_ctrlEarphone);
	DDX_Control(pDX, IDC_COMBO_LVDSMODE, m_ctrlLvdsModeCmb);
	DDX_Control(pDX, IDC_CMB_AVC_INPUT, m_ctrlAncInputCmb);
	DDX_Control(pDX, IDC_EDIT_AVC_OUT, m_ctrlAvcInputSelectCmb);
	DDX_Control(pDX, IDC_CHK_HDMI_GRAB, m_ctrlHdmiGrabChk);
	DDX_Control(pDX, IDC_COMBO_HDMITYPE2, m_ctrlHdmiResolution);
	DDX_Control(pDX, IDC_COMBO_HDMIBITSHIFT2, m_ctrlHdmiBitShift);
	DDX_Control(pDX, IDC_COMBO_UHDTYPE2, m_ctrlUhdType);
	DDX_Control(pDX, IDC_COMBO_GRABMODE2, m_ctrlGrabMode);
	DDX_Control(pDX, IDC_COMBO_BITSHIFT2, m_ctrlBitShift);
	DDX_Control(pDX, IDC_CHECK_MHL_LINE, m_ctrlMhlDetChk);
	DDX_Control(pDX, IDC_COMBO_OUT_PORT2, m_ctrlHdmiOutPortCmb2);
	DDX_Control(pDX, IDC_CHECK_ARC, m_ctrlArcChk);
	DDX_Control(pDX, IDC_COMBO_OUT_PORT, m_ctrlHdmiOutPortCmb);
	DDX_Control(pDX, IDC_CHK_ODD_DE_ONLY, m_ctrlOddDE_Only);
	DDX_Control(pDX, IDC_CHK_DDCLINE, m_ctrlDDCLineChk);
	DDX_Control(pDX, IDC_CHK_HDCP, m_ctrlHdcpChk);
	DDX_Control(pDX, IDC_CHK_EDID, m_ctrlEdidChk);
	DDX_Control(pDX, IDC_EDIT_PATTERN2, m_ctrlPatternEdit2);
	DDX_Control(pDX, IDC_CHK_ONLINE2, m_ctrlOnlineChk2);
	DDX_Control(pDX, IDC_EDIT_MODEL2, m_ctrlModelEdit2);
	DDX_Control(pDX, IDC_LEDLABEL_GRAB, m_ctrlGrabStatus);
	DDX_Control(pDX, IDC_CONTROL_TAB, m_ctrlControlTab);
	DDX_Control(pDX, IDC_EDIT_SEND_REMOTE_RETURN, m_ctrlSendRemoteRtnEdit);
	DDX_Control(pDX, IDC_EDIT_AVSWITCH_RETURN, m_ctrlAvSwitchRtnEdit);
	DDX_Control(pDX, IDC_EDIT_AVSWITCH_RESULT, m_ctrlAvSwitchRstEdit);
	DDX_Control(pDX, IDC_STATIC_AVSWITCH_VALUE_NAME2, m_ctrlAvSwitchVal2Name);
	DDX_Control(pDX, IDC_STATIC_AVSWITCH_VALUE_NAME1, m_ctrlAvSwitchVal1Name);
	DDX_Control(pDX, IDC_CMB_AVSWITCH_VALUE2, m_ctrlAvSwitchVal2Cmb);
	DDX_Control(pDX, IDC_CMB_AVSWITCH_VALUE1, m_ctrlAvSwitchVal1Cmb);
	DDX_Control(pDX, IDC_BTN_SAVE_BMP, m_ctrlSaveAsBtn);
	DDX_Control(pDX, IDC_CHK_VIDEO_OUT_GRAB, m_ctrlVideoOutGrabChk);
	DDX_Control(pDX, IDC_CHK_LVDS_GRAB, m_ctrlLvdsGrabChk);
	DDX_Control(pDX, IDC_BTN_GRAB, m_ctrlGrabBtn);
	DDX_Control(pDX, IDC_CHK_ONLINE, m_ctrlOnLineChk);
	DDX_Control(pDX, IDC_EDIT_MODEL, m_ctrlModelEdit);
	DDX_Control(pDX, IDC_EDIT_PATTERN, m_ctrlPatternEdit);
	DDX_Control(pDX, IDC_EDIT_REMOTE_CODE, m_ctrlRemoteCodeEdit);
	DDX_Control(pDX, IDC_CHK_MANUAL_GRAB, m_ctrlManualGrabChk);
	DDX_Control(pDX, IDC_CHK_AUTO_GRAB, m_ctrlAutoGrabChk);
	DDX_Radio(pDX, IDC_RADIO_NTSC, m_nAnalogSignalType);
	DDX_Text(pDX, IDC_LEDLABEL_GRAB, m_szGrabStatus);
	//DDX_Radio(pDX, IDC_RDO_LVDS_RESOLUTION1, m_nLvdsResolutionIndex);
	DDX_Radio(pDX, IDC_RDO_LVDS_MODE1, m_nUranousMode);
	DDX_Radio(pDX, IDC_RDO_COLOR_DEPTH1, m_nBitMode);
	DDX_Check(pDX, IDC_CHK_ODD_DE_ONLY, m_bOddDE_Only);
	DDX_Check(pDX, IDC_CHK_IMAGE_ROTATION, m_bImage_Rotation);
	DDX_Radio(pDX, IDC_RDO_3D_MODE1, m_n3DMode);
	DDX_Control(pDX, IDC_CHECK_RELAY1, m_ctrlRelay1Chk);
	DDX_Check(pDX, IDC_CHECK_RELAY1, m_bRelay1);
	DDX_Radio(pDX, IDC_RADIO_MHL, m_nMhlMode);
	DDX_Check(pDX, IDC_CHECK_ARC, m_bHdmiGenArc);
	DDX_Check(pDX, IDC_CHECK_MHL_LINE, m_bMhlDetect);
	DDX_Check(pDX, IDC_CHECK_EARPHONE, m_bEarphone);
	DDX_Check(pDX, IDC_CHECK_COMP, m_bComp);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHK_CEC, m_ctrlCECChk);
	DDX_Control(pDX, IDC_CHECK_GRAB_POWER, m_ctrlGrabPower);
	DDX_Check(pDX, IDC_CHECK_GRAB_POWER, m_bGrabPower);
	DDX_Control(pDX, IDC_CHECK_HDMI_USB_CYL, m_cCheckHdmiUsbCyl);
	DDX_Check(pDX, IDC_CHECK_HDMI_USB_CYL, m_bCheckHdmiUsbCyl);

	DDX_Text(pDX, IDC_EDIT_MODEL_NAME, m_strEditModelName);
	DDX_Control(pDX, IDC_CHECK_COMMAND_LOAD, m_cCheckLoad);
	DDX_Control(pDX, IDC_CHECK_COMMAND_UNLOAD, m_cCheckUnLoad);
	DDX_Control(pDX, IDC_CHECK_COMMAND_PASS, m_cCheckTestPass);
	DDX_Control(pDX, IDC_CHECK_COMMAND_NG, m_cCheckTestNg);
	DDX_Control(pDX, IDC_CHECK_FFC, m_cCheckReconnect);
	DDX_Control(pDX, IDC_CHECK_LOAD_FINISH, m_cCheckLoadFinish);
	DDX_Control(pDX, IDC_CHECK_UNLOAD_FINISH, m_cCheckUnLoadFinish);
	DDX_Control(pDX, IDC_CHECK_FFC_FINISH, m_cCheckReconnectFinish);
	DDX_Control(pDX, IDC_CHECK_STATUS_AUTO, m_cCheck_PLC_Auto_Mode);
	DDX_Control(pDX, IDC_CHECK_STATUS_MANUAL, m_cCheck_PLC_Manual_Mode);
	DDX_Control(pDX, IDC_CHECK_STATUS_START, m_cCheck_PLC_Start_Run);
	DDX_Control(pDX, IDC_CHECK_STATUS_PAUSE, m_cCheck_PLC_Pause);
	DDX_Control(pDX, IDC_CHECK_STATUS_STOP, m_cCheck_PLC_STOP);
	DDX_Control(pDX, IDC_CHECK_STATUS_WARNING, m_cCheck_PLC_Warn);
	DDX_Control(pDX, IDC_CHECK_STATUS_ALARM, m_cCheck_PLC_Alarm);
	DDX_Control(pDX, IDC_CHECK_PCB_IN, m_cCheck_PLC_Data_PCB_Inside);
	DDX_Control(pDX, IDC_CHECK_PCB_DUMMY, m_cCheck_PLC_Data_DummyCut);
	DDX_Control(pDX, IDC_CHECK_DATA_PCB_OK, m_cCheck_PLC_Data_OK);
	DDX_Control(pDX, IDC_CHECK_DATA_PCB_NG, m_cCheck_PLC_Data_NG);
	DDX_Check(pDX, IDC_CHK_RB_SWAP_GRAB, m_bRedBlueSwap);
	DDX_Check(pDX, IDC_CHK_DE_HSYNC_GRAB, m_bDeHsyncEnable);
	DDX_Check(pDX, IDC_CHK_HSYNC_EDGE_GRAB, m_bHyncEdgeEnable);
	DDX_Control(pDX, IDC_COMBO_Y20_SW_MODE, m_cComboY20SwMode);
	DDX_Control(pDX, IDC_COMBO_LVDS_RESOLUTION2, m_cComboLvdsFormat);
}
BEGIN_MESSAGE_MAP(CGrabPage, CDialog)
	//{{AFX_MSG_MAP(CGrabPage)
	ON_BN_CLICKED(IDC_CHK_AUTO_GRAB, OnChkAutoGrab)
	ON_BN_CLICKED(IDC_CHK_MANUAL_GRAB, OnChkManualGrab)
	ON_BN_CLICKED(IDC_BTN_GRAB, OnBtnManualGrab)
	ON_BN_CLICKED(IDC_BTN_SEND_REMOTE_CODE, OnBtnSendRemoteCode)
	ON_BN_CLICKED(IDC_BTN_OPEN_REMOTE_DLG, OnBtnOpenRemoteDlg)
	ON_BN_CLICKED(IDC_CHK_ONLINE, OnChkOnline)
	ON_BN_CLICKED(IDC_BTN_SEND_PATTERN, OnBtnSendPattern)
	ON_BN_CLICKED(IDC_BTN_SEND_MODEL, OnBtnSendModel)
	ON_BN_CLICKED(IDC_BTN_SAVE_BMP, OnBtnSaveBmp)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHK_LVDS_GRAB, OnChkLvdsGrab)
	ON_BN_CLICKED(IDC_CHK_HDMI_GRAB, OnChkHdmiGrab)
	ON_BN_CLICKED(IDC_CHK_VIDEO_OUT_GRAB, OnChkVideoOutGrab)
	ON_BN_CLICKED(IDC_RADIO_NTSC, OnRadioNtsc)
	ON_BN_CLICKED(IDC_RADIO_PAL, OnRadioPal)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD1, OnRdoAvswitchCmd1)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD2, OnRdoAvswitchCmd2)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD3, OnRdoAvswitchCmd3)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD4, OnRdoAvswitchCmd4)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD5, OnRdoAvswitchCmd5)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD6, OnRdoAvswitchCmd6)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD7, OnRdoAvswitchCmd7)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD8, OnRdoAvswitchCmd8)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD9, OnRdoAvswitchCmd9)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD10, OnRdoAvswitchCmd10)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD11, OnRdoAvswitchCmd11)
	ON_BN_CLICKED(IDC_BTN_SEND_AVSWITCH_CMD, OnBtnSendAvswitchCmd)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CMB_AVSWITCH_VALUE1, OnSelchangeCmbAvswitchValue1)
	ON_CBN_SELCHANGE(IDC_CMB_AVSWITCH_VALUE2, OnSelchangeCmbAvswitchValue2)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD12, OnRdoAvswitchCmd12)
	ON_WM_TIMER()
	ON_COMMAND_RANGE(IDC_BTN_REMOTE_DLG_1, IDC_BTN_REMOTE_DLG_73,OnRemoteButtonClick)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CONTROL_TAB, OnSelchangeControlTab)
	ON_BN_CLICKED(IDC_BTN_GRAB_OPTION_APPLY, OnBtnGrabOptionApply)
	//ON_COMMAND_RANGE(IDC_RDO_LVDS_RESOLUTION1, IDC_RDO_LVDS_RESOLUTION20,OnRdoLvdsResolution1)
	ON_COMMAND_RANGE(IDC_RDO_COLOR_DEPTH1, IDC_RDO_COLOR_DEPTH4,OnRdoColorDepth1)
	ON_COMMAND_RANGE(IDC_RDO_3D_MODE1,IDC_RDO_3D_MODE3, OnRdo3dMode1)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD13, OnRdoAvswitchCmd13)
	ON_BN_CLICKED(IDC_CHK_EDID, OnChkEdid)
	ON_BN_CLICKED(IDC_CHK_HDCP, OnChkHdcp)
	ON_BN_CLICKED(IDC_CHK_ONLINE2, OnChkOnline2)
	ON_BN_CLICKED(IDC_BTN_SEND_MODEL2, OnBtnSendModel2)
	ON_BN_CLICKED(IDC_BTN_SEND_PATTERN2, OnBtnSendPattern2)
	ON_BN_CLICKED(IDC_CHK_DDCLINE, OnChkDdcline)
	ON_BN_CLICKED(IDC_RADIO_SECAM, OnRadioSecam)
	ON_BN_CLICKED(IDC_CHK_ODD_DE_ONLY, OnChkOddDeOnly)
	ON_BN_CLICKED(IDC_CHK_IMAGE_ROTATION, OnChkImageRotation)
	ON_BN_CLICKED(IDC_RDO_AVSWITCH_CMD14, OnRdoAvswitchCmd14)
	ON_CBN_SELCHANGE(IDC_COMBO_OUT_PORT, OnSelchangeComboOutPort)
	ON_BN_CLICKED(IDC_CHECK_RELAY1, OnCheckRelay1)
	ON_COMMAND_RANGE(IDC_RADIO_MHL,IDC_RADIO_MHL2, OnRadioMhl)
	ON_BN_CLICKED(IDC_CHECK_ARC, OnCheckArc)
	ON_BN_CLICKED(IDC_CHECK_MHL_LINE, OnCheckMhlLine)
	ON_BN_CLICKED(IDC_BTN_AVC_COM, OnBtnAvcCom)
	ON_CBN_SELCHANGE(IDC_CMB_AVC_INPUT, OnSelchangeCmbAvcInput)
	ON_BN_CLICKED(IDC_BTN_AVC_IR, OnBtnAvcIr)
	ON_BN_CLICKED(IDC_BTN_AVC_BUF, OnBtnAvcBuf)
	ON_BN_CLICKED(IDC_CHECK_EARPHONE, OnCheckEarphone)
	ON_BN_CLICKED(IDC_CHECK_COMP, OnCheckComp)
	//}}AFX_MSG_MAP

	ON_MESSAGE(UM_UPDATE_GRAB_STATUS, UpdateGrabStatus)

	ON_BN_CLICKED(IDC_CHK_CEC, &CGrabPage::OnBnClickedChkCec)
	ON_BN_CLICKED(IDC_CHECK_GRAB_POWER, &CGrabPage::OnBnClickedCheckGrabPower)
	ON_BN_CLICKED(IDC_BTN_RESET_IFPACK, &CGrabPage::OnBnClickedBtnResetIfpack)
	ON_BN_CLICKED(IDC_BTN_RESET_GENDER, &CGrabPage::OnBnClickedBtnResetGender)
	ON_BN_CLICKED(IDC_CHECK_HDMI_USB_CYL, &CGrabPage::OnBnClickedCheckHdmiUsbCyl)
	ON_BN_CLICKED(IDC_BTN_MAIN_JIG_UP, &CGrabPage::OnBnClickedBtnMainJigUp)
	ON_BN_CLICKED(IDC_BTN_MAIN_JIG_DOWN, &CGrabPage::OnBnClickedBtnMainJigDown)


	ON_BN_CLICKED(IDC_BTN_COMMAND_MODEL_NAME, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandModelName)
	ON_BN_CLICKED(IDC_BTN_COMMAND_LOAD, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandLoad)
	ON_BN_CLICKED(IDC_BTN_COMMAND_PASS_UNLOAD, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandPassUnload)
	ON_BN_CLICKED(IDC_BTN_COMMAND_NG_LOAD, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandNgUnload)
	//	ON_BN_CLICKED(IDC_BTN_COMMAND_NG_LOAD2, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandNgLoad2)
	ON_BN_CLICKED(IDC_BTN_COMMAND_FFC_RETRY, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandFfcRetry)
	ON_BN_CLICKED(IDC_BTN_COMMAND_READ, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandRead)


	ON_BN_CLICKED(IDC_CHK_RB_SWAP_GRAB, &CGrabPage::OnBnClickedChkRbSwapGrab)
	ON_BN_CLICKED(IDC_CHK_DE_HSYNC_GRAB, &CGrabPage::OnBnClickedChkDeHsyncGrab)
	ON_BN_CLICKED(IDC_CHK_HSYNC_EDGE_GRAB, &CGrabPage::OnBnClickedChkHsyncEdgeGrab)
	ON_BN_CLICKED(IDC_BTN_RESET_GRAB, &CGrabPage::OnBnClickedBtnResetGrab)
	ON_CBN_SELCHANGE(IDC_COMBO_GRABMODE2, &CGrabPage::OnCbnSelchangeComboGrabmode2)
	ON_CBN_SELCHANGE(IDC_COMBO_UHDTYPE2, &CGrabPage::OnCbnSelchangeComboUhdtype2)
	ON_BN_CLICKED(IDC_BTN_LAN_TEST, &CGrabPage::OnBnClickedBtnLanTest)
	ON_BN_CLICKED(IDC_BTN_LAN_CLEAR, &CGrabPage::OnBnClickedBtnLanClear)
	ON_BN_CLICKED(IDC_BTN_DPMS_CHECK, &CGrabPage::OnBnClickedBtnDpmsCheck)
	ON_CBN_SELCHANGE(IDC_COMBO_LVDS_RESOLUTION2, &CGrabPage::OnCbnSelchangeComboLvdsResolution2)
END_MESSAGE_MAP()

//	ON_COMMAND_RANGE(IDC_RADIO_MHL,IDC_RADIO_MHL2, OnRadioMhl)
//	ON_BN_CLICKED(IDC_RADIO_MHL, OnRadioMhl)
/////////////////////////////////////////////////////////////////////////////
// CGrabPage message handlers

BOOL CGrabPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_hPageGrabThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);
	
//	m_ctrlSaveAsBtn.EnableWindow(FALSE);

	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&m_Image1);
	}
	m_nAnalogSignalType = CurrentSet->nAnalogType;  

	m_ctrlAvSwitchVal1Cmb.ShowWindow(FALSE);
	m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
	m_ctrlAvSwitchVal1Name.SetWindowText("");
	m_ctrlAvSwitchVal2Name.SetWindowText("");
	m_ctrlAvSwitchRstEdit.ShowWindow(FALSE);
	m_ctrlAvSwitchRtnEdit.ShowWindow(FALSE);
	m_ctrlSendRemoteRtnEdit.SetWindowText("");

	//+add kwmoon 080628
	TC_ITEM TabItem;

	TabItem.mask = TCIF_TEXT;

	TabItem.pszText = "AvSwitchBox";
	m_ctrlControlTab.InsertItem(0,&TabItem);
	
	TabItem.pszText = "Grabber Option";
	m_ctrlControlTab.InsertItem(1,&TabItem);

	TabItem.pszText = "USB DIO Test";
	m_ctrlControlTab.InsertItem(2,&TabItem);

	ShowAvSwitchBoxCtrlGroup(TRUE);
	ShowGrabberOptionGroup(FALSE);
	ShowUsbDioGroup(FALSE);
	
	m_ctrlAncInputCmb.SetCurSel(0);
	// Lvds Format
	if((CurrentSet->nLvdsWidth == 1024) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1024_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1280_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 1024))	m_nLvdsResolutionIndex = W_1280_H_1024;
	else if((CurrentSet->nLvdsWidth == 1364) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1364_H_768;
	else if((CurrentSet->nLvdsWidth == 1366) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1366_H_768;
	else if((CurrentSet->nLvdsWidth == 1440) && (CurrentSet->nLvdsHeight == 900))	m_nLvdsResolutionIndex = W_1440_H_900;
	else if((CurrentSet->nLvdsWidth == 1600) && (CurrentSet->nLvdsHeight == 900))   m_nLvdsResolutionIndex = W_1600_H_900;
	else if((CurrentSet->nLvdsWidth == 1680) && (CurrentSet->nLvdsHeight == 1050))	m_nLvdsResolutionIndex = W_1680_H_1050;
	else if((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 1080) && (CurrentSet->nFullHd120Hz == 0))
	{
		m_nLvdsResolutionIndex = W_1920_H_1080_60;
	}
	else if((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 1080) && (CurrentSet->nFullHd120Hz == 1))
	{
		m_nLvdsResolutionIndex = W_1920_H_1080_120;
	}
	else if((CurrentSet->nLvdsWidth == 2560) && (CurrentSet->nLvdsHeight == 1080) && (CurrentSet->nFullHd120Hz == 1)) 
	{
		m_nLvdsResolutionIndex = W_2560_H_1080;
	}
	else if((CurrentSet->nLvdsWidth == 2560) && (CurrentSet->nLvdsHeight == 1440) && (CurrentSet->nFullHd120Hz == 1)) 
	{
		m_nLvdsResolutionIndex = W_2560_H_1440;
	}
	else if((CurrentSet->nLvdsWidth == 3440) && (CurrentSet->nLvdsHeight == 1440)){	m_nLvdsResolutionIndex = W_3440_H_1440;}
	else if((CurrentSet->nLvdsWidth == 3840) && (CurrentSet->nLvdsHeight == 2160)){	m_nLvdsResolutionIndex = W_3840_H_2160;}
	else if((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 300)) {	m_nLvdsResolutionIndex = W_1920_H_300;}
	else if((CurrentSet->nLvdsWidth == 3840) && (CurrentSet->nLvdsHeight == 600)){	m_nLvdsResolutionIndex = W_3840_H_600;}
	else if ((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 540)) { m_nLvdsResolutionIndex = W_1920_H_540; }
	else if ((CurrentSet->nLvdsWidth == 1920) && (CurrentSet->nLvdsHeight == 1200)) { m_nLvdsResolutionIndex = W_1920_H_1200; }
	else if ((CurrentSet->nLvdsWidth == 2048) && (CurrentSet->nLvdsHeight == 2560)) { m_nLvdsResolutionIndex = W_2048_H_2560; }
	else if ((CurrentSet->nLvdsWidth == 3584) && (CurrentSet->nLvdsHeight == 720))
	{ 
		m_nLvdsResolutionIndex = W_3584_H_720;
	}
	else if ((CurrentSet->nLvdsWidth == 3328) && (CurrentSet->nLvdsHeight == 1440)) { m_nLvdsResolutionIndex = W_3328_H_1440; }
	else m_nLvdsResolutionIndex = -1;

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
	m_cComboLvdsFormat.AddString("3584x 720(3440x1440)");	//#define		3584x 740(3440x1440) 	19
	m_cComboLvdsFormat.AddString("3328 x 1440         ");	//#define		W_3328_H_1440			20
	m_cComboLvdsFormat.SetCurSel(m_nLvdsResolutionIndex);

	if(g_nGrabberType == FHD_GRABBER)
	{
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
	}

	//add psh 090630
	m_bOddDE_Only = CurrentSet->bOddDE_Only;
	//add psh 091214
	m_bImage_Rotation = CurrentSet->nImageRotation;
	m_bRedBlueSwap = CurrentSet->nRedBlueSwap;
	m_bDeHsyncEnable = CurrentSet->nDeHsyncEnable;
	m_bHyncEdgeEnable = CurrentSet->nHyncEdgeEnable;



	// add 101029
	m_n3DMode = CurrentSet->n3DGrabbingMode;

	m_ctrlLvdsModeCmb.SetCurSel(CurrentSet->nLVDS_Mode);

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
	m_ctrlGrabMode.AddString(" 0. Type1 : UHD - Non Division");
	m_ctrlGrabMode.AddString(" 1. Type2 : UHD - 2 Division");
	m_ctrlGrabMode.AddString(" 2. Type3 : UHD - 4 Division");
	m_ctrlGrabMode.AddString(" 3. Type4 : UHD - 8 Division");
	m_ctrlGrabMode.AddString(" 4. Type5 : 16Y_EPI(80P)");
	m_ctrlGrabMode.AddString(" 5. Type6 : FHD(120), etc.");
	m_ctrlGrabMode.AddString(" 6. Type7 : ..");
	m_ctrlGrabMode.AddString(" 7. Type8 : HD, FHD(60), etc.");
	m_ctrlGrabMode.AddString(" 8. Type9 : 16Y_EPI(60P)");
	m_ctrlGrabMode.AddString(" 9. Type10 : 16Y_EPI(60P_White)");
	m_ctrlGrabMode.AddString(" 10.Type11 : 18YEPI(60P / RGBW / 6Nane)");
	m_ctrlGrabMode.AddString(" 11. Type12 : 20YEPI 60P CEDS");
	m_ctrlGrabMode.AddString(" 12. Type13 : 21Y50 68P HKC CEDS UPQO VA");
	m_ctrlGrabMode.AddString(" 13. Type14 : 21Y70 68P SharpCEDS DPT UPQO");
	m_ctrlGrabMode.AddString(" 14. Type15 : HKC 966 Pixel");
	m_ctrlGrabMode.AddString(" 15. Type16 : CSOT 68P 55UP77");//
	m_ctrlGrabMode.AddString(" 16. Type17 : 8K T-CONLESS");
	m_ctrlGrabMode.AddString(" 17. Type18 : LM21A HKC 220307");
	m_ctrlGrabMode.AddString(" 18. Type19 : QHD_FULL_SPEED");
	m_ctrlGrabMode.AddString(" 19. Type20 : Shuffle 4");
	m_ctrlGrabMode.AddString(" 20. Type21 : 32ch Vx MNT Pack");
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


	m_ctrlHdmiResolution.SetCurSel(CurrentSet->nHDMI_Grab_Resolution);
	m_ctrlHdmiBitShift.SetCurSel(CurrentSet->nHDMI_Grab_BitShift);

	UpdateData(FALSE);
	if(g_nGrabberType == UHD_GRABBER)
	{
		m_Image1.m_pImageData = new unsigned char	[10000*8000];
	}

	CRect rect;
	CButton* pButton = NULL;

	for(int i=0; i<NO_AVSWITCHBOX_TAB_ITEM;i++)
	{
		pButton = (CButton*)GetDlgItem(aAvSwitchBoxCtrlItemNo[i]);
		pButton->GetWindowRect(&rect);
		// add & change psh 080701
        ScreenToClient(&rect);
		rect.top	 = rect.top	- 320;
		rect.bottom  = rect.bottom  - 320;
		//-

		pButton->MoveWindow(&rect);
	}
	for(int i=0; i<NO_AVCONVERTOR_TAB_ITEM;i++)
	{
		pButton = (CButton*)GetDlgItem(aAvConvertorCtrlItemNo[i]);
		pButton->GetWindowRect(&rect);
		// add & change psh 080701
        ScreenToClient(&rect);
		rect.top	 = rect.top	- 340;
		rect.bottom  = rect.bottom  - 340;

		pButton->MoveWindow(&rect);
		if (aAvConvertorCtrlItemNo[i] == IDC_BTN_AVC_IR)
		{
			pButton->EnableWindow(TRUE);
		}
		else
		{
			if (!gAvConvertorCtrl.m_bPortOpen) {
				pButton->EnableWindow(FALSE);
			}
		}
	}

	for (int i = 0; i < NO_LAN_TAB_ITEM; i++)
	{
		pButton = (CButton*)GetDlgItem(aLAN_CheckerItemNo[i]);
		pButton->GetWindowRect(&rect);
		// add & change psh 080701
		ScreenToClient(&rect);
		rect.top = rect.top - 340;
		rect.bottom = rect.bottom - 340;

		pButton->MoveWindow(&rect);
		
		if (!gVF1000Ctrl.m_bPortOpen) {
			pButton->EnableWindow(FALSE);
		}
		
	}



	for(int i=0; i<NO_USB_DIO_TAB_ITEM;i++)
	{
		pButton = (CButton*)GetDlgItem(aUsbDioItemNo[i]);
		pButton->GetWindowRect(&rect);
        ScreenToClient(&rect);
//		rect.top	 = rect.top	- 450;
//		rect.bottom  = rect.bottom  - 450;
		rect.top	 = rect.top	- 360;
		rect.bottom  = rect.bottom  - 360;
		rect.left	 = rect.left	+ 290;
		rect.right  = rect.right  + 290;

		pButton->MoveWindow(&rect);
	}

	m_nMhlMode = g_pView->m_bMhlMode;

	m_szGrabStatus = "-READY-";
//	m_ctrlGrabStatus.SetWindowText("-READY-");


	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(!InitPatternGenerator()) return FALSE;
	}

	if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
	{
		if(InitAvSwitchController() == FALSE) return FALSE;
	}

	SetHDMIGenGroup(CurrentSet->bUseHDMIGen);
	if(CurrentSet->bUseHDMIGen && HDMIGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(!InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate)) return FALSE;
		m_ctrlOnlineChk2.SetCheck(TRUE);

	}
	m_ctrlHdmiOutPortCmb.SetCurSel(HDMIGeneratorCtrl.m_nOutPort);
	//+add 090206(Modification No3)
	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&m_Image1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////
// Init Grabber
/////////////////
BOOL CGrabPage::InitGrabber(int nGrabberType)
{
	CString szErrorMsg;

	//+del kwmoon 080318
/*	if(nGrabberType == LVDS_GRABBER)
	{
		if(PeriControl.m_bGrabberInit == FALSE)
		{
			if(!PeriControl.OnPciCardOpen())
			{
				AfxMessageBox("[LVDS Grabber]\nInitialization failed!"); 
				return FALSE;
			}

			if(PeriControl.m_bGrabberInit == FALSE)
			{
				AfxMessageBox("[LVDS Grabber]\nInitialization failed!"); 
				return FALSE;
			}
		}
	}
*/	
	//+add 090131(수정항목5)
/*	if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 0))
	{
		if(nGrabberType == ANALOG_GRABBER) // ANALOG GRAB
		{
			if(AnalogControl.m_bGrabberInit == FALSE)
			{
				AnalogControl.OnPciCardOpen(m_ctrlProgress,80); 

				if(AnalogControl.m_bGrabberInit == FALSE)
				{
					AfxMessageBox("[Analog Grabber]\nInitialization failed!"); 
					return FALSE;
				}
			}
		}
	} */
	return TRUE;
}

/////////////////////////
// Init Pattern Generator
/////////////////////////
BOOL CGrabPage::InitPatternGenerator()
{
	//Pattern Generator Remote On
	CString szCmdString;
	szCmdString.Format("%c",START_TERMINAL_MODE);
	
	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString); 
	m_nPatternMode = ONLINE;
	m_ctrlOnLineChk.SetCheck(TRUE);
//	m_ctrlOffLineChk.SetCheck(FALSE);

	return TRUE;
}

////////////////////
// Init AvSwitchBox
////////////////////
BOOL CGrabPage::InitAvSwitchController()
{
	if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
	{
		if(AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort , CurrentSet->wAvSwitchBoxBaudRate ) == FALSE)
		{
			AvSwitchBoxCtrl.PortClose();
			AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate);

			if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	return TRUE;
}

UINT CGrabPage::GrabImageThread_UHD(LPVOID pParam)
{
	BOOL bGrabbing = FALSE;

	CGrabPage* pGrabPage = (CGrabPage*)pParam;

	int nWidth		= 0;
	int nHeight		= 0;
	int nImageSize	= 0;
	int nErrorCode	= 0;
	int nBitCount	= 0;
//	int nRowWidth   = 0;
	int	nNewWidth, nNewHeight;

	BYTE* pImgBuf8 = NULL;
//	BYTE* pImgBuf8_2 = NULL;
	unsigned char *bufTmp;

	int	nCurIdx, nPrvIdx = 0;
	int	nFirst;
//	int	i; //, j, k;
	int nNoGrabCount = 0;
	int nGrabResult = GRAB_FAIL;
	BOOL bResult	= FALSE;

	CString szFileName = _T("");
	CString szErrorMsg = _T("");

	::ResetEvent(pGrabPage->m_hPageGrabThreadKillEvent);

//	int bpl;
//	BYTE* pBits	= NULL;
//	UINT tSize;

	pGrabPage->m_bThreadRunning = TRUE;

	nBitCount	= 24;
	if(pGrabPage->m_nGrabDevice == LVDS_GRABBER){
		if(CurrentSet->nUHD_Type == PACK_TYPE_UHDEPI60){
			nWidth		 = 3072*2; // 3840;
		}
		else{
			nWidth		 = CurrentSet->nLvdsWidth; // 3840;
		}
		nNewWidth		= CurrentSet->nLvdsWidth;
		nHeight		= CurrentSet->nLvdsHeight;
		nImageSize  = WIDTHBYTES(nBitCount*nNewWidth) * nHeight;
		pGrabPage->m_Image1.InitHeaders(COLOR_24BIT,nNewWidth,nHeight,nImageSize);
	}
	else if(pGrabPage->m_nGrabDevice == HDMI_GRABBER){
		nWidth		= CurrentSet->nHdmiWidth;
		nHeight		= CurrentSet->nHdmiHeight;
		nImageSize  = WIDTHBYTES(nBitCount*nWidth) * nHeight;
		pGrabPage->m_Image1.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);
	}
	else{
		if(CurrentSet->nAnalogType == 0){nWidth = 768;	nHeight	= 624;	} //NTSC
		else{nWidth	= 768;	nHeight	= 630;}  //PAL, SECAM

		nNewWidth	= CurrentSet->nAnalogWidth;
		nNewHeight	= CurrentSet->nAnalogHeight;
		nImageSize  = WIDTHBYTES(nBitCount*nNewWidth) * nNewHeight;
		pGrabPage->m_Image1.InitHeaders(COLOR_24BIT,nNewWidth,nNewHeight,nImageSize);
	} 


	bufTmp = (unsigned char *) malloc (nWidth * nHeight * 3);
	memset(pGrabPage->m_Image1.m_pImageData,0x00,nImageSize);

	nFirst		=	1;
//	pImgBuf8_2	=	new unsigned char [nWidth*nHeight*3];

	while(TRUE)
	{
		if(pGrabPage->m_bStopGrabThread)//pGrabPage->m_bThreadRunning
		{
			pGrabPage->m_szGrabStatus = "GRAB-NG!";
			pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
			pGrabPage->m_bStopGrabThread = FALSE; goto END_POSITION;
		}

		pGrabPage->m_Image1.m_bImageLoaded = FALSE;

		g_pView->m_clsPCI.CM_ChkGrabDone(&nCurIdx);
		if (nFirst==1) {
			nCurIdx = 0;
			nFirst = 0;
		}

		if (nCurIdx > nPrvIdx) {
			g_pView->m_clsPCI.CM_ImageCopySafe(nCurIdx, nWidth*3*nHeight, (int**) &pImgBuf8);


			if(pGrabPage->m_nGrabDevice == LVDS_GRABBER)
			{
				if (CurrentSet->nUHD_Type == PACK_TYPE_UHDEPI) {		// EPI Mode
					//frame->m_clsPCI.DFT3_EPIPuzzle(frame->m_nImgMode, imgBuf8, /*frame->m_pDoc->m_pImage*/frame->m_pEPIRGBImage, frame->m_pEPIWImage, nWidth, nHeight, frame->m_bRotate180);	// 150311 rotate추가
					g_pView->m_clsPCI.DFT3_EPIPuzzle(pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, 0);
					//memcpy(frame->m_pDoc->m_pImage, frame->m_pEPIRGBImage, nWidth*nHeight*3* sizeof(unsigned char)  );
				}
				else if (CurrentSet->nUHD_Type == PACK_TYPE_CMVIEW) {		// CM VIEW
					g_pView->m_clsPCI.DFT3_CMVPuzzle(pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation);
				}
				else {

					if (CurrentSet->nRedBlueSwap)
					{
						g_ImageProc.RedBlueSwap(pImgBuf8, nWidth, nHeight, CurrentSet->nRedBlueSwap);
					}
					



 					if (CurrentSet->nUHD_Grab_Mode == 9) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(CurrentSet->nUHD_Grab_Mode, pImgBuf8, bufTmp, nWidth, nHeight, CurrentSet->nImageRotation);	
 						g_pView->m_clsPCI.DFT3_WhiteMerge(CurrentSet->nUHD_Grab_Mode, bufTmp, pGrabPage->m_Image1.m_pImageData, nWidth/2, nHeight);//
 					}
					else if (CurrentSet->nUHD_Grab_Mode == 11) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P60_CEDS20Y_Mapping(bufTmp, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);
					//	lRotateProcess = 1;
					}
					else if ((CurrentSet->nUHD_Grab_Mode == 12)|| (CurrentSet->nUHD_Grab_Mode == 14)) //( (CurrentSet->nUHD_Grab_Mode >= 14)&&(CurrentSet->nUHD_Grab_Mode <= 17))) //201106 else if (CurrentSet->nUHD_Grab_Mode == 12)
					{
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P68_50_UPQO_VA_CEDS_HKC(bufTmp, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);
					
					}
					else if (CurrentSet->nUHD_Grab_Mode == 13) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P68_70_UPQO_Sharp_CEDS_DPT(bufTmp, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);
						//	lRotateProcess = 1;
					}				
					else if (CurrentSet->nUHD_Grab_Mode == 15) //( (CurrentSet->nUHD_Grab_Mode >= 14)&&(CurrentSet->nUHD_Grab_Mode <= 17))) //201106 else if (CurrentSet->nUHD_Grab_Mode == 12)
					{
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P68_55UP77_CSOT(bufTmp, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);					
					}		
					else if (CurrentSet->nUHD_Grab_Mode == 16) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P22Y8K_TCONLESS(bufTmp, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);
						
					}
					else if (CurrentSet->nUHD_Grab_Mode == 17) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.LM21A_HKC_220307(bufTmp, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);
					}
					else if (CurrentSet->nUHD_Grab_Mode == 18) {

						g_ImageProc.DFT3_UHDPuzzleLocal(CurrentSet->nUHD_Grab_Mode, pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation);
					}
					else if (CurrentSet->nUHD_Grab_Mode == 19) {

						g_ImageProc.DFT3_UHDPuzzleLocal(CurrentSet->nUHD_Grab_Mode, pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation);
					}
					else if ((CurrentSet->nUHD_Grab_Mode == 2) && (CurrentSet->nUHD_Type == PACK_TYPE_STANDBYME))//(CurrentSet->nUHD_Grab_Mode == 2)// 
					{
						g_ImageProc.DFT3_UHDPuzzleLocal(CurrentSet->nUHD_Grab_Mode, pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation, CurrentSet->nUHD_Type);
					}
					else if (CurrentSet->nUHD_Grab_Mode == 20) 
					{
						g_ImageProc.DFT3_UHDPuzzleLocal(CurrentSet->nUHD_Grab_Mode, pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation, CurrentSet->nUHD_Type);
					}
					else {

#ifdef SM_MODIFY_CODE__	
						g_pView->m_clsPCI.DFT3_UHDPuzzle(CurrentSet->nUHD_Grab_Mode, pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, 0);

						g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);

#else
						g_pView->m_clsPCI.DFT3_UHDPuzzle(CurrentSet->nUHD_Grab_Mode, pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation);

#endif
					}

					//g_ImageProc.Rotate(pGrabPage->m_Image1, (float)CurrentSet->nImageRotation);

					//if (CurrentSet->nRedBlueSwap)
					//{
					//	g_ImageProc.RedBlueSwap(g_GrabDisplayImage.m_pImageData, nWidth, nHeight, CurrentSet->nRedBlueSwap);
					//}
					//if (CurrentSet->nRedBlueSwap)
					//{
					//	g_ImageProc.RedBlueSwap(pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nRedBlueSwap);
					//}

					if (CurrentSet->nUHD_Y20_SW_Mode == 3)
					{
						g_ImageProc.SW_MODE_Mapping(pGrabPage->m_Image1.m_pImageData, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, CurrentSet->nUHD_Y20_SW_Mode);
					}


					//if (CurrentSet->nDeHsyncEnable || CurrentSet->nHyncEdgeEnable)
					//{
					//	g_ImageProc.HsyncModeChange(CurrentSet->nDeHsyncEnable, CurrentSet->nHyncEdgeEnable);
					//}
				}

			}
			else if(pGrabPage->m_nGrabDevice == HDMI_GRABBER)
			{
				g_pView->m_clsPCI.DFT3_HDMIPuzzle(pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, 0);
			}
			else{
				g_pView->m_clsPCI.DFT3_AnalogPuzzle(CurrentSet->nAnalogType, pImgBuf8, pGrabPage->m_Image1.m_pImageData, nWidth, nHeight, &nNewWidth, &nNewHeight, 0);//17,280, 484);
			}

			pGrabPage->m_szGrabStatus = "GRAB-OK!";
			pGrabPage->m_Image1.m_bImageLoaded = TRUE;
			
			nNoGrabCount = 0;
			pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
			
			nPrvIdx = nCurIdx;
			Sleep(10);
		}
		else {

			if (pGrabPage == NULL)
			{
				goto END_POSITION;
			}
			if(nNoGrabCount < 500){nNoGrabCount++;}
			if(nNoGrabCount > 30){
				memset(pGrabPage->m_Image1.m_pImageData,0xff,nImageSize);
				if (pGrabPage == NULL)
				{
					goto END_POSITION;
				}
				pGrabPage->m_Image1.m_bImageLoaded = TRUE;
				if (pGrabPage == NULL)
				{
					goto END_POSITION;
				}
				pGrabPage->m_szGrabStatus = "GRAB-NG!";
				pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
			}
			if (pGrabPage == NULL)
			{
				goto END_POSITION;
			}
			nPrvIdx = nCurIdx;
			Sleep(50);
		}
	}

END_POSITION : 
	
	free(bufTmp);

	pGrabPage->m_bThreadRunning = FALSE;

	::SetEvent(pGrabPage->m_hPageGrabThreadKillEvent);

	return TRUE;
}
UINT CGrabPage::GrabImageThread_FHD(LPVOID pParam)
{
	BOOL bGrabbing = FALSE;

	CGrabPage* pGrabPage = (CGrabPage*)pParam;

	int nWidth		= 0;
	int nHeight		= 0;
	int nImageSize	= 0;
	int nErrorCode	= 0;
	int nBitCount	= 0;
	int nRowWidth   = 0;
	int nImageBufferSize = 0;
	int nGrabResult = GRAB_FAIL;

	BOOL bResult	= FALSE;

	CString szFileName = _T("");
	CString szErrorMsg = _T("");

	BYTE* pImageBuffer = NULL;

	::ResetEvent(pGrabPage->m_hPageGrabThreadKillEvent);

	//////////////////
	// LVDS GRABBER //
	//////////////////
	if(pGrabPage->m_nGrabDevice == LVDS_GRABBER)
	{
		pGrabPage->m_bThreadRunning = TRUE;

		while(TRUE)
		{
			if(pGrabPage->m_bStopGrabThread)
			{
				pGrabPage->m_szGrabStatus = "GRAB-NG!";
				pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
				pGrabPage->m_bStopGrabThread = FALSE; goto END_POSITION;
			}

			nWidth		= CurrentSet->nLvdsWidth;
			nHeight		= CurrentSet->nLvdsHeight;
			nBitCount	= CurrentSet->nBitCount;
			nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
			nImageSize  = nRowWidth * nHeight;

			pGrabPage->m_Image1.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

			pGrabPage->m_Image1.m_bImageLoaded = FALSE;
			
			if((nGrabResult = g_pView->Grab_FHD(GRAB_WITH_NO_RETRY)) != GRAB_PASS)
			{
				pGrabPage->m_Image1.m_pImageData = g_GrabImage.m_pImageData;
				pGrabPage->m_Image1.m_bImageLoaded = TRUE;
				
				pGrabPage->m_szGrabStatus = "GRAB-NG!";
				pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);

				if(!pGrabPage->m_bGrabSingleFrame)
				{
					if(pGrabPage->m_bStopGrabThread)
					{
						pGrabPage->m_szGrabStatus = "GRAB-NG!";
						pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
						pGrabPage->m_bStopGrabThread = FALSE; goto END_POSITION;
					}

					// Essential delay for a screen update 
					Sleep(100); continue;
				}
				else goto END_POSITION;
			}
			else
			{
				if(pGrabPage->m_bStopGrabThread)
				{
					pGrabPage->m_szGrabStatus = "GRAB-NG!";
					pGrabPage->m_bStopGrabThread = FALSE; goto END_POSITION;
				}

				pGrabPage->m_szGrabStatus = "GRAB-OK!";
				pGrabPage->m_Image1.m_pImageData = g_GrabImage.m_pImageData;
				pGrabPage->m_Image1.m_bImageLoaded = TRUE;

				pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);

				// Essential delay for a screen update
				Sleep(100);
			}

			if(pGrabPage->m_bGrabSingleFrame) break;
		}
	}
	////////////////////
	// ANALOG GRABBER //
	////////////////////
	else 
	{
		pGrabPage->m_bThreadRunning = TRUE;

		while(TRUE)
		{
			if(pGrabPage->m_bStopGrabThread)
			{
				pGrabPage->m_szGrabStatus = "GRAB-NG!";
				pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
				pGrabPage->m_bStopGrabThread = FALSE; goto END_POSITION;
			}



			nWidth		= CurrentSet->nAnalogWidth;
			nHeight		= CurrentSet->nAnalogHeight;
			nBitCount	= 24;
			nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
			nImageSize  = nRowWidth * nHeight;
	
			// 090120 External DFT

			pGrabPage->m_Image1.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

			// 090120 External DFT
			nGrabResult = g_pView->Grab_FHD(GRAB_WITH_NO_RETRY,EXT_ANALOG_GRABBER);

			if(nGrabResult != GRAB_PASS)
			{
				memset(g_GrabImage.m_pImageData,0xff,nImageSize);
				pGrabPage->m_Image1.m_pImageData = g_GrabImage.m_pImageData;
				pGrabPage->m_Image1.m_bImageLoaded = TRUE;

				pGrabPage->m_szGrabStatus = "GRAB-NG!";
				pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
				
				if(!pGrabPage->m_bGrabSingleFrame)
				{
					if(pGrabPage->m_bStopGrabThread)
					{
						pGrabPage->m_szGrabStatus = "GRAB-NG!";
						pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
						pGrabPage->m_bStopGrabThread = FALSE; 
						goto END_POSITION;
					}
	
					Sleep(100);	continue;
				}
				else goto END_POSITION;
				
			}
			else
			{
				if(pGrabPage->m_bStopGrabThread)
				{
					pGrabPage->m_szGrabStatus = "GRAB-NG!";
					pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
					pGrabPage->m_bStopGrabThread = FALSE; 
					goto END_POSITION;
				}
				
				pGrabPage->m_szGrabStatus = "GRAB-OK!";
				pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);
				Sleep(100);
			}

			nRowWidth = WIDTHBYTES(pGrabPage->m_Image1.m_BmInfo->bmiHeader.biBitCount * pGrabPage->m_Image1.m_BmInfo->bmiHeader.biWidth);

			pGrabPage->m_Image1.m_bImageLoaded = FALSE;

			pGrabPage->m_Image1.m_pImageData = g_GrabImage.m_pImageData;
			pGrabPage->m_Image1.m_bImageLoaded = TRUE;

			pGrabPage->PostMessage(UM_UPDATE_GRAB_STATUS);

			if(pGrabPage->m_bGrabSingleFrame) break;

			Sleep(100);
		}
	}

END_POSITION : 
	
	pGrabPage->m_bThreadRunning = FALSE;

	::SetEvent(pGrabPage->m_hPageGrabThreadKillEvent);

	return TRUE;
}
void CGrabPage::GrabStart(int nGrabMode)
{
	BOOL bFlag = FALSE;
	int	lRet;
	int nModel;
	int nWidth;
	int nHeight;
	int nGrabSize;
	int nShiftVal;
	int nImageOffset;
	int	nLvdsType = 0;

	if (g_nCommandOnlyType == TRUE)
		return;

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if (!g_pView->m_bGrabConnection)
		{
			if (g_nSysRobotType == AUTO_SYS)//AUTO_SYS
			{
				AddStringToStatus("Grabber  Not Connect...");
			}
			else
			{

				
				AfxMessageBox("Grabber  Not Connect...");
			}
			return;
		}

	}

	if(m_bThreadRunning){
		return;
	}

	// AUTO GRAB
	if(nGrabMode == AUTO_GRAB)
	{
		m_bGrabSingleFrame = FALSE;
	}
	else // MANUAL GRAB
	{
		m_bGrabSingleFrame = TRUE;
	}

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_pView->GrabberReset();
		if(m_nGrabDevice == LVDS_GRABBER)
		{
			nModel		 = CurrentSet->nUHD_Type;
			if(nModel == 8){
				nWidth		 = 3072*2; // 3840;
			}
			else{
				nWidth		 = CurrentSet->nLvdsWidth; // 3840;
			}
			nHeight		 = CurrentSet->nLvdsHeight; // 2160;
			nGrabSize	 = ((nWidth * nHeight * 3) + 7) & 0xfffffff8;
			nShiftVal	 = CurrentSet->nUHD_Grab_BitShift;
			nLvdsType	 = CurrentSet->nLVDS_Mode;
			nImageOffset = nWidth * 3;
		}
		else if(m_nGrabDevice == HDMI_GRABBER)
		{
			nWidth		 = CurrentSet->nHdmiWidth; 
			nHeight		 = CurrentSet->nHdmiHeight; 
			nGrabSize	 = ((nWidth * nHeight * 3) + 7) & 0xfffffff8;
			nShiftVal	 = CurrentSet->nHDMI_Grab_BitShift;
			nModel		 = CurrentSet->nHDMI_Grab_Resolution;
			nImageOffset = nWidth * 3;
			lRet			=	g_pView->m_clsPCI.CM_RegisterWrite(0x00110, nGrabSize);
			lRet			=	g_pView->m_clsPCI.CM_MemInit(nGrabSize, 5000);
		}
		else{
			if(CurrentSet->nAnalogType == 0){nWidth = 768;	nHeight	= 624;	} //NTSC
			else{nWidth	= 768;	nHeight	= 630;}  //PAL, SECAM
			nGrabSize	 = ((nWidth * nHeight * 3) + 7) & 0xfffffff8;
			lRet			=	g_pView->m_clsPCI.CM_RegisterWrite(0x00110, nGrabSize);
			lRet			=	g_pView->m_clsPCI.CM_MemInit(nGrabSize, 5000);
		}

		//=========================
		// Start Auto-Grab Thread
		//=========================
		m_pGrabImageThread_UHD = AfxBeginThread(GrabImageThread_UHD,this);
	
		if(m_nGrabDevice == LVDS_GRABBER)
		{
			if ((nModel == PACK_TYPE_5_Rev_2019)
				|| (nModel == PACK_TYPE_5_Rev_2020)
				||(nModel == PACK_TYPE_UHDFULL_LOWSPEED)
				|| (nModel == PACK_TYPE_QHD_FULL_SPEED)
				|| (nModel == PACK_TYPE_120HZ_LOW_SPEED)
				|| (nModel == PACK_TYPE_STANDBYME))
			{
				lRet = g_pView->DFT3_UHDGrabStartLocal(nModel, nShiftVal, nWidth, nHeight, nImageOffset, 50, nLvdsType);
			}
			else
			{
				lRet = g_pView->m_clsPCI.DFT3_UHDGrabStart(nModel, nShiftVal, nWidth, nHeight, nImageOffset, 50, nLvdsType);
			}

			//if (CurrentSet->nUHD_Grab_Mode == 14) //if ((CurrentSet->nUHD_Grab_Mode >= 14) && (CurrentSet->nUHD_Grab_Mode <= 17)) //201109 HKC_966_PIXEL
			{
				__int64 SW_Mode_Sel;

				if (CurrentSet->nUHD_Y20_SW_Mode >= 8)
				{
					SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode - 8;
				}
				else
				{
					SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode;
				}

				if ((SW_Mode_Sel >= 0) && (SW_Mode_Sel <= 7)) // Y20_SW_Mode_01
					SW_Mode_Sel = SW_Mode_Sel;
				else
					SW_Mode_Sel = 0x00000000;

				g_pView->m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
				Sleep(1); // SW_Mode_Sel
			}
			//if ((CurrentSet->nUHD_Grab_Mode >= 14)&&(CurrentSet->nUHD_Grab_Mode <= 17)) //201109 HKC_966_PIXEL
			//{
			//	__int64 SW_Mode_Sel;
			//	if (CurrentSet->nUHD_Grab_Mode == 14) // Y20_SW_Mode_01
			//		SW_Mode_Sel = 0x00000001;
			//	else if (CurrentSet->nUHD_Grab_Mode == 15) // Y20_SW_Mode_00
			//		SW_Mode_Sel = 0x00000000;
			//	else if (CurrentSet->nUHD_Grab_Mode == 16) // Y20_SW_Mode_02
			//		SW_Mode_Sel = 0x00000002;
			//	else if (CurrentSet->nUHD_Grab_Mode == 17) // Y20_SW_Mode_03
			//		SW_Mode_Sel = 0x00000003;
			//	else 
			//		SW_Mode_Sel = 0x00000001;

			//	g_pView->m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
			//	Sleep(1); // SW_Mode_Sel
			//}
			//lRet = g_pView->m_clsPCI.DFT3_UHDGrabStart(nModel, nShiftVal, nWidth, nHeight, nImageOffset, 50, nLvdsType);
			//if (nModel == PACK_TYPE_5_Rev_2019)
			//{
			//	Sleep(1);
			//	long long n64BitSpeed = ((nModel == PACK_TYPE_UHDFULL) | (nModel == PACK_TYPE_UHDNORMAL)) ? 0x1120 :
			//		((nModel == PACK_TYPE_UHDEPI)) ? 0x525 : 0x36;	// lvds speed gbps
			//	g_pView->m_clsPCI.CM_RegisterWrite(0x0160, n64BitSpeed | (nLvdsType << 20));		Sleep(1);//n64BitSpeed |= 0x113f0000;
			//}
		}
		else if(m_nGrabDevice == HDMI_GRABBER)
		{
			lRet = g_pView->m_clsPCI.DFT3_HDMIGrabStart(nModel, nWidth, nHeight, nShiftVal);
		}
		else{
			lRet = g_pView->m_clsPCI.DFT3_AnalogGrabStart(0, nWidth, nHeight);
		}

	
	}
	else//FHD
	{
		m_pGrabImageThread_FHD = AfxBeginThread(GrabImageThread_FHD,this);
	}	
	
	if (CurrentSet->nDeHsyncEnable || CurrentSet->nHyncEdgeEnable)
	{
		g_ImageProc.HsyncModeChange(CurrentSet->nDeHsyncEnable, CurrentSet->nHyncEdgeEnable);
	}
}

void CGrabPage::GrabStop()
{
	DWORD dwThreadResult = 0;
	DWORD dwExitCode = 0;

	if (g_nCommandOnlyType == TRUE)
		return;


	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_pView->m_clsPCI.DFT3_UHDGrabStop();
	}

	if(m_bThreadRunning)
	{
		m_bStopGrabThread = TRUE; 

		ShowSystemMsg(); 

		dwThreadResult = WaitForSingleObject(m_hPageGrabThreadKillEvent,10000);


		if(dwThreadResult != WAIT_OBJECT_0)
		{
			AfxMessageBox("Failed to stop grab process!");
		
			//UHD
			if(g_nGrabberType == UHD_GRABBER)
			{
				if(m_pGrabImageThread_UHD) TerminateThread(m_pGrabImageThread_UHD->m_hThread,dwExitCode);
			}
			else//FHD
			{
				if(m_pGrabImageThread_FHD) TerminateThread(m_pGrabImageThread_FHD->m_hThread,dwExitCode);
			}
			
		}

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			m_pGrabImageThread_UHD = NULL; 
		}
		else//FHD
		{
			m_pGrabImageThread_FHD = NULL; 
		}

		CloseSystemMsg(); 
		m_bThreadRunning = FALSE;
	}
}

void CGrabPage::ShowSystemMsg() 
{
	return;
	// Modeless Dialog
	if(m_pSystemMessageDlg == NULL)
	{
		m_pSystemMessageDlg = new CStopProcessingDlg(this);
		m_pSystemMessageDlg->Create(IDD_STOP_PROCESSING_DLG);
		m_pSystemMessageDlg->SetMessage(STOP_GRAB_PROCESSING_MSG);
		m_pSystemMessageDlg->ShowWindow(SW_SHOW);
		m_pSystemMessageDlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
		m_pSystemMessageDlg->UpdateWindow();
		m_pSystemMessageDlg->SetShowDlg(TRUE);
	}
}

void CGrabPage::CloseSystemMsg() ///Godtech
{
	return;
	if((m_pSystemMessageDlg != NULL)
	&& (m_pSystemMessageDlg->m_bShowDlg))
	{
		m_pSystemMessageDlg->m_bShowDlg = FALSE;
		m_pSystemMessageDlg->DestroyWindow();
		delete m_pSystemMessageDlg;
		//m_pSystemMessageDlg = NULL;
	}
}

void CGrabPage::OnChkAutoGrab() 
{
	if(m_MWnd)
	{	
		m_MWnd->SendMessage(UM_UPDATE_IMAGE_VIEW_DISPLAY);	
	}
	else
	{
		return;
	}
	
	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != AUTO_GRAB)
		{
			GrabStop();
		}
	}

	m_nGrabMode = AUTO_GRAB;

	m_ctrlManualGrabChk.SetCheck(FALSE);
	m_ctrlGrabBtn.EnableWindow(FALSE);
	m_ctrlAutoGrabChk.SetCheck(TRUE);

	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		m_ctrlSaveAsBtn.EnableWindow(FALSE);

		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NTSC);
		pButton->EnableWindow(FALSE);

		pButton = (CButton*)GetDlgItem(IDC_RADIO_PAL);
		pButton->EnableWindow(FALSE);

		pButton = (CButton*)GetDlgItem(IDC_RADIO_SECAM);
		pButton->EnableWindow(FALSE);
	}
	GrabStart(m_nGrabMode);	
}

void CGrabPage::OnChkManualGrab() 
{
	if(m_MWnd)
	{	
		m_MWnd->SendMessage(UM_UPDATE_IMAGE_VIEW_DISPLAY);	
	}
	else
	{
		return;
	}

	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			GrabStop();
		}
	}

	m_szGrabStatus = "-READY-";

	UpdateData(FALSE);

	m_nGrabMode = MANUAL_GRAB;

	m_ctrlManualGrabChk.SetCheck(TRUE);
	m_ctrlGrabBtn.EnableWindow(TRUE);
	m_ctrlAutoGrabChk.SetCheck(FALSE);

	m_ctrlVideoOutGrabChk.EnableWindow(TRUE);
	m_ctrlLvdsGrabChk.EnableWindow(TRUE);

	if(m_MWnd)
	{	
		m_MWnd->SendMessage(UM_UPDATE_IMAGE_VIEW_DISPLAY);	
	}
}

void CGrabPage::OnBtnManualGrab() 
{
	DWORD dwThreadResult = 0;
	DWORD dwExitCode = 0;

	if (g_nCommandOnlyType == TRUE)
		return;


	CButton* pButton = (CButton*)GetDlgItem(IDC_BTN_GRAB);
	pButton->EnableWindow(FALSE);
	
	if(m_MWnd)
	{	
		m_MWnd->SendMessage(UM_UPDATE_IMAGE_VIEW_DISPLAY);	
	}
	else
	{
		pButton->EnableWindow(TRUE);
		return;
	}	
	
	if(m_nGrabMode == MANUAL_GRAB)
	{
		m_szGrabStatus = "GRABBING!";
		UpdateData(FALSE);

		GrabStart(m_nGrabMode); // _Wait(500);

		dwThreadResult = WaitForSingleObject(m_hPageGrabThreadKillEvent,1000);

		if(dwThreadResult != WAIT_OBJECT_0)
		{
		//	AfxMessageBox("Failed to grab");

			//UHD
			if(g_nGrabberType == UHD_GRABBER)
			{
				if(m_pGrabImageThread_UHD) TerminateThread(m_pGrabImageThread_UHD->m_hThread,dwExitCode);
			}
			else//FHD
			{
				if(m_pGrabImageThread_FHD) TerminateThread(m_pGrabImageThread_FHD->m_hThread,dwExitCode);
			}
			m_szGrabStatus = "GRAB-NG!";
			PostMessage(UM_UPDATE_GRAB_STATUS);
		}

		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			m_pGrabImageThread_UHD = NULL; 
		}
		else//FHD
		{
			m_pGrabImageThread_FHD = NULL; 
			m_ctrlSaveAsBtn.EnableWindow(TRUE);
		}

		m_bThreadRunning = FALSE;
	}
	else
	{
		AfxMessageBox("First,Select Manual Check Button");
	}
	pButton->EnableWindow(TRUE);
}

void CGrabPage::OnChkLvdsGrab() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NTSC);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_PAL);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_SECAM);
	pButton->EnableWindow(FALSE);


	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			GrabStop();
		}
	}
	
	m_nGrabDevice = LVDS_GRABBER;

	m_ctrlVideoOutGrabChk.SetCheck(FALSE);
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		m_ctrlHdmiGrabChk.SetCheck(FALSE);
	}
	m_ctrlLvdsGrabChk.SetCheck(TRUE);

	if(m_bThreadRunning != TRUE)
	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
	}
}	
void CGrabPage::OnChkHdmiGrab() 
{
	//FHD
	if(g_nGrabberType == FHD_GRABBER){
		return;
	}
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NTSC);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_PAL);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_SECAM);
	pButton->EnableWindow(FALSE);


	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			GrabStop();
		}
	}
	
	m_nGrabDevice = HDMI_GRABBER;

	m_ctrlVideoOutGrabChk.SetCheck(FALSE);
	m_ctrlLvdsGrabChk.SetCheck(FALSE);
	m_ctrlHdmiGrabChk.SetCheck(TRUE);

	if(m_bThreadRunning != TRUE)
	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
	}
}	

void CGrabPage::OnChkVideoOutGrab() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NTSC);
	pButton->EnableWindow(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_PAL);
	pButton->EnableWindow(TRUE);
	
	//+add 090420(Modification No1)
	pButton = (CButton*)GetDlgItem(IDC_RADIO_SECAM);
	pButton->EnableWindow(TRUE);
	UpdateData(FALSE);

	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			GrabStop();
		}
	}
	
	m_nGrabDevice = ANALOG_GRABBER;

	m_ctrlVideoOutGrabChk.SetCheck(TRUE);
	m_ctrlLvdsGrabChk.SetCheck(FALSE);
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		m_ctrlHdmiGrabChk.SetCheck(FALSE);
	}

	if(m_bThreadRunning != TRUE)
	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
	}
}

void CGrabPage::OnRadioNtsc() 
{
	//+add 090206(Modification No3)
	CurrentSet->nAnalogWidth  = 640; 
	CurrentSet->nAnalogHeight = 480;
	
	//+add 090420(Modification No3)
	m_nGrabDevice = ANALOG_GRABBER;
	CurrentSet->nAnalogType   = V_SRC_NTSC;

	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			GrabStop();
		}
	}
	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&m_Image1);
	}

	if(m_bThreadRunning != TRUE)
	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
	}


	UpdateData(TRUE);
}

void CGrabPage::OnRadioPal() 
{
	CurrentSet->nAnalogWidth  = 704;
	CurrentSet->nAnalogHeight = 572;
//	CurrentSet->nAnalogWidth  = 768; 
//	CurrentSet->nAnalogHeight = 576;

	//+add 090420(Modification No3)
	m_nGrabDevice = ANALOG_GRABBER;
	CurrentSet->nAnalogType   = V_SRC_PAL;

	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			GrabStop();
		}
	}
	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&m_Image1);
	}
	if(m_bThreadRunning != TRUE)
	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
	}

	UpdateData(TRUE);
}

void CGrabPage::OnBtnSaveBmp() 
{
	DWORD dwThreadResult = 0;

	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		if(m_bThreadRunning == TRUE)
		{
			GrabStop();		
		}
	}
	CString sTmp = _T("");
	CBaseImage* pBaseImage = NULL;
	
	CString szFileName = _T("");
	CString szFilter = "BMP (*.bmp) | *.bmp||";

	pBaseImage = &m_Image1;

	if(pBaseImage->m_bImageLoaded != TRUE)
	{
		AfxMessageBox("No Image is snapped yet!"); return;
	}
	
//	sTmp = CurrentSet->sModelFolder + "\\ref\\";  
	sTmp = CurrentSet->sResultFolder;
	SetCurrentDirectory(sTmp);
	CFileDialog dlg(FALSE,".bmp",NULL,OFN_HIDEREADONLY,szFilter);
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK)
	{
		szFileName = dlg.GetPathName();

		if(!pBaseImage->SaveImage(szFileName, SNAP_IMAGE, *m_ctrlProgress)){ return;}
	}
}

void CGrabPage::OnBtnSendRemoteCode() 
{
	//+ 2007.10.16 Mod BY USY
	CString sTmp;
	m_ctrlRemoteCodeEdit.GetWindowText(sTmp);
	//+2007.12.25 Mod BY USY
	m_ctrlSendRemoteRtnEdit.SetWindowText("");
	//-

	//+ 2007.12.4 Mod BY USY
	if(CurrentSet->bUseAVSwitchBox)
	{
		if(!InitAvSwitchController()) return;
		else
		{
			m_ctrlRemoteCodeEdit.GetWindowText(sTmp);
			
			//+2007.12.25 Mod BY USY
			AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sTmp);
			m_ctrlSendRemoteRtnEdit.SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
			//AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE, hexCstr2decNum(sTmp));
			//-
		}
	}
}

void CGrabPage::OnBtnOpenRemoteDlg() 
{
	//+2008.02.11 Add BY USY
	CRemoteDlg dlg(m_MWnd);
//	CRemoteControlDlg dlg(m_MWnd);
	//-
	dlg.m_MWnd = m_MWnd;

	if(dlg.DoModal() != IDOK) return;
}

void CGrabPage::OnChkOnline() 
{
	if(m_ctrlOnLineChk.GetCheck()) 
	{
		CString szCmdString = _T("");
		szCmdString.Format("%c",START_TERMINAL_MODE);

		// Send On-Line Mode On Cmd
		PatternGeneratorCtrl.SendCommString(szCmdString);  

		m_nPatternMode = ONLINE;
	}
	else
	{
		CString szCmdString = _T("");
		szCmdString.Format("%c",END_TERMINAL_MODE);
		
		// Send On-Line Mode On Cmd
		PatternGeneratorCtrl.SendCommString(szCmdString);  

		PatternGeneratorCtrl.PortClose();

		m_nPatternMode = OFFLINE;
	}
}
/*
void CGrabPage::OnChkOffline() 
{
	if(m_nPatternMode != ONLINE)
	{
		CString szCmdString = _T("");
		szCmdString.Format("%c",START_TERMINAL_MODE);

		// Send On-Line Mode On Cmd
		PatternGeneratorCtrl.SendCommString(szCmdString);  
		m_nPatternMode = ONLINE;

		// Before sending another command, following delay should be applied.
		_Wait(500);
	}
	
	if(m_nPatternMode == ONLINE)
	{
		CString szCmdString = _T("");
		szCmdString.Format("%c",END_TERMINAL_MODE);
		
		// Send On-Line Mode On Cmd
		PatternGeneratorCtrl.SendCommString(szCmdString);  

		PatternGeneratorCtrl.PortClose();

		m_nPatternMode = OFFLINE;
		m_ctrlOnLineChk.SetCheck(FALSE);
		m_ctrlOffLineChk.SetCheck(TRUE);
	}
}
*/
void CGrabPage::OnBtnSendPattern() 
{
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	CString sTmp = _T("");
	UINT nNumber = 0;
	
	m_ctrlPatternEdit.GetWindowText(sTmp);
	nNumber = atoi(sTmp);
//	PatternGeneratorCtrl.SendRun((BYTE)0x08, (int)nNumber);
	if(PatternGeneratorCtrl.SetPattern((int)nNumber))
	{
		m_nPatternMode = ONLINE;
		m_ctrlOnLineChk.SetCheck( TRUE);
	}
	else
	{
		AfxMessageBox(_T("Failed to Set Pattern[P/G]"));
		m_nPatternMode = 0;
		m_ctrlOnLineChk.SetCheck( FALSE);
	}
}

void CGrabPage::OnBtnSendModel() 
{
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	CString sTmp = _T("");
	UINT nNumber = 0;

	m_ctrlModelEdit.GetWindowText(sTmp);
	nNumber = atoi(sTmp);
	
//	PatternGeneratorCtrl.SendRun((BYTE)0x07, (int)nNumber);
	if(PatternGeneratorCtrl.SetModel((int)nNumber))
	{
		m_nPatternMode = ONLINE;
		m_ctrlOnLineChk.SetCheck( TRUE);
	}
	else
	{
		AfxMessageBox(_T("Failed to Set Model[P/G]"));
		m_nPatternMode = 0;
		m_ctrlOnLineChk.SetCheck( FALSE);
	}
}

void CGrabPage::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bThreadRunning == TRUE)
	{
		GrabStop();		
	}

	//+add 090131(수정항목1)
	DWORD dwExitCode = 0;

	//UHD
	if(g_nGrabberType == UHD_GRABBER){
		if(m_pGrabImageThread_UHD)
		{
			TerminateThread(m_pGrabImageThread_UHD->m_hThread,dwExitCode);

			//+add 090224(Modification No2)
			m_pGrabImageThread_UHD = NULL; 

			if(m_Image1.m_pImageData != NULL)
			{
					delete	m_Image1.m_pImageData; 
					m_Image1.m_pImageData = NULL;
			}
		}
	}
	else{//FHD
		if(m_pGrabImageThread_FHD)
		{
			TerminateThread(m_pGrabImageThread_FHD->m_hThread,dwExitCode);

			m_pGrabImageThread_FHD = NULL; 
		}

		m_Image1.m_pImageData = NULL;
	}

	CDialog::OnClose();
}


BOOL CGrabPage::PreTranslateMessage(MSG* pMsg) 
{
	//+ 2007.5.4 Add BY USY
	//Protect Dialog close By Enter Key or ESC Key
	if(pMsg->message == WM_KEYDOWN)
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
            return TRUE;
	//-

	return CDialog::PreTranslateMessage(pMsg);
}


///////////////////////////////////////////////////////////////////////////////////////////////



void CGrabPage::OnRdoAvswitchCmd1() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_DIGIT_OUT;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd2() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_VIDEO_CH;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd3() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_AUDIO_CH;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd4() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_COMP_CH;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd5() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_CVBS_CH;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd6() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_ANALOG_VOLT;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd7() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = PULLUP_AVSWITCH;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd8() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = REMOTE_TYPE_SEL;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

void CGrabPage::OnRdoAvswitchCmd9() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = CHK_AUDIO_LVL;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}
void CGrabPage::OnRdoAvswitchCmd10() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = CHK_AUDIO_FRQ;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}
void CGrabPage::OnRdoAvswitchCmd11() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = CHECK_STATUS;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

///////////////////////////////////////////////////////////////////////////////////////////////

BOOL CGrabPage::FindSourceKey(int nSource, CString& strName, CString& strCode, CString& strDesc)
{
	CString sSrc, sMsg;
	switch(nSource) 
	{
		case INPUT_RF: sSrc.Format("TV");	break;
		case INPUT_AV: 
		case INPUT_AV1: sSrc.Format("AV1");	break;
		case INPUT_AV2: sSrc.Format("AV2");	break;
		case INPUT_AV3: sSrc.Format("AV3");	break;
		case INPUT_COMP:
		case INPUT_COMP1: sSrc.Format("COMP1");	break;
		case INPUT_COMP2: sSrc.Format("COMP2");	break;
		case INPUT_RGB: sSrc.Format("RGB");	break;
		case INPUT_HDMI: sSrc.Format("HDMI");	break;
		case INPUT_HDMI1: sSrc.Format("HDMI1");	break;
		case INPUT_HDMI2: sSrc.Format("HDMI2");	break;
		case INPUT_HDMI3: sSrc.Format("HDMI3");	break;
	}

	POSITION Pos = NULL;
	CRemoteCode* pRemote = NULL;
	CString sName, sCode;
	BOOL bFind = FALSE;

	Pos = CurrentSet->RemoteList.GetHeadPosition();

	while(Pos)
	{
		pRemote = CurrentSet->RemoteList.GetNext(Pos);
		sName = pRemote->sName;
		sName.MakeUpper();
		if(sName == sSrc)
		{
			strName = pRemote->sName;
			strCode = pRemote->sCode;
			strDesc = pRemote->sDesc;
			bFind = TRUE;
			return TRUE;
		}
	}
	if(bFind == FALSE) 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGrabPage::SendInputKey(int nSource)
{
	CString sName, sCode, sDesc;
	CString sTmp;
	
	if(!FindSourceKey(nSource, sName, sCode, sDesc)) return FALSE;
	else
	{
		//+ 2007.12.4 Mod BY USY
		if(CurrentSet->bUseAVSwitchBox)
		{
			if(!InitAvSwitchController()) return FALSE;
			else 
			{
				//+2007.12.25 Mod BY USY
				if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sCode))
				{
//						m_ctrlSendKeyRtnEdit.SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
					return TRUE;
				}
				else 
				{
//						m_ctrlSendKeyRtnEdit.SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
					return FALSE;
				}
//					AvSwitchBoxCtrl.SendRun2(SEND_REMOTE_CODE, hexCstr2decNum(sCode));
//					return TRUE;
				//-
			}
		}
	}
	return TRUE;
}

BOOL CGrabPage::FindButtonKey(CString sBtnName, CString& strName, CString& strCode, CString& strDesc)
{
	POSITION Pos = NULL;
	CRemoteCode* pRemote = NULL;
	CString sName, sCode;
	BOOL bFind = FALSE;

	Pos = CurrentSet->RemoteList.GetHeadPosition();

	while(Pos)
	{
		pRemote = CurrentSet->RemoteList.GetNext(Pos);
		sName = pRemote->sName;
		sName.MakeUpper();
		sBtnName.MakeUpper();

		if(sName == sBtnName)
		{
			strName = pRemote->sName;
			strCode = pRemote->sCode;
			strDesc = pRemote->sDesc;
			bFind = TRUE;
			return TRUE;
		}
	}

	if(bFind == FALSE) 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGrabPage::SendButtonKey(CString strName)
{
	CString sName, sCode, sDesc;
	
	if(!FindButtonKey(strName, sName, sCode, sDesc)) return FALSE;
	else
	{
		//+ 2007.12.4 Mod BY USY
		if(CurrentSet->bUseAVSwitchBox)
		{
			if(!InitAvSwitchController()) return FALSE;
			else 
			{
				//+2007.12.25 Mod BY USY
				if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sCode))
				{
					return TRUE;
				}
				else 
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}


void CGrabPage::SetAvSwitchValueCmb(int nCmdType)
{
	CString sTmp;
	int nTmp = 0;

	m_ctrlAvSwitchVal1Cmb.EnableWindow(TRUE);
	m_ctrlAvSwitchVal2Cmb.EnableWindow(TRUE);

	m_ctrlAvSwitchVal1Cmb.ShowWindow(TRUE);
	m_ctrlAvSwitchVal2Cmb.ShowWindow(TRUE);

	m_ctrlAvSwitchVal1Cmb.ResetContent();
	m_ctrlAvSwitchVal2Cmb.ResetContent();
	
	//+2007.10.22 Add BY USY
	m_ctrlAvSwitchRstEdit.SetWindowText("");
	m_ctrlAvSwitchRstEdit.ShowWindow(FALSE);
	//-
	//+2007.12.25 Add BY USY
	m_ctrlAvSwitchRtnEdit.SetWindowText("");
	m_ctrlAvSwitchRtnEdit.ShowWindow(TRUE);
	//-

	switch(nCmdType) 
	{
		case SET_AUDIO_CH:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			for(nTmp = 0; nTmp < 9; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);

		}
		break;
		case SET_ANALOG_VOLT:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			m_ctrlAvSwitchVal1Cmb.AddString("1");
			m_ctrlAvSwitchVal1Cmb.AddString("2");

			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("Voltage");
			m_ctrlAvSwitchVal2Cmb.AddString("0V");
			m_ctrlAvSwitchVal2Cmb.AddString("1.0V");
			m_ctrlAvSwitchVal2Cmb.AddString("2.0V");
			m_ctrlAvSwitchVal2Cmb.AddString("2.5V");
			m_ctrlAvSwitchVal2Cmb.AddString("3.3V");

			m_ctrlAvSwitchVal2Cmb.SetCurSel(0);
		}
		break;
		case SET_DIGIT_OUT:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("PORT");
			for(nTmp = 0; nTmp < MAX_DIO_PORT; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);
		
			m_ctrlAvSwitchVal2Name.SetWindowText("Set");
			m_ctrlAvSwitchVal2Cmb.AddString("OFF");
			m_ctrlAvSwitchVal2Cmb.AddString("ON");

			m_ctrlAvSwitchVal2Cmb.SetCurSel(0);
		}
		break;
		case SET_COMP_CH:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			for(nTmp = 0; nTmp < MAX_COMPONENT_CH; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		case SET_CVBS_CH:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			for(nTmp = 0; nTmp < MAX_CVBS_CH; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		case CHK_AUDIO_LVL:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			m_ctrlAvSwitchVal1Cmb.AddString("Left");
			m_ctrlAvSwitchVal1Cmb.AddString("Right");

			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("Result");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
			//+2007.10.22 Add BY USY
			m_ctrlAvSwitchRstEdit.SetWindowText("");
			m_ctrlAvSwitchRstEdit.ShowWindow(TRUE);	
			//-
		}
		break;
		case CHK_AUDIO_FRQ:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			m_ctrlAvSwitchVal1Cmb.AddString("Left");
			m_ctrlAvSwitchVal1Cmb.AddString("Right");

			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("Result");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
			//+2007.10.22 Add BY USY
			m_ctrlAvSwitchRstEdit.SetWindowText("");
			m_ctrlAvSwitchRstEdit.ShowWindow(TRUE);	
			//-
		}
		break;
		case PULLUP_AVSWITCH:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("Voltage");
			m_ctrlAvSwitchVal1Cmb.AddString("3.3V");
			m_ctrlAvSwitchVal1Cmb.AddString("5V");

			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		//+2007.12.05 Add BY USY
		case SET_VIDEO_CH:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			for(nTmp = 0; nTmp < MAX_VIDEO_CH; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		case REMOTE_TYPE_SEL:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("Remote Type");

			m_ctrlAvSwitchVal1Cmb.AddString("1 : Codeless IR");
			m_ctrlAvSwitchVal1Cmb.AddString("2 : Code IR");
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		//-
		case CHECK_STATUS:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("");
			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal1Cmb.ShowWindow(FALSE);
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		
		//+add kwmoon 080312
		case SET_REMOTE_REPEAT_NO:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("Rmt Repeat");
			for(nTmp = 0; nTmp < MAX_REMOTE_REPEAT; nTmp++)
			{
				sTmp.Format("%d", nTmp);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
	
		//+add kwmoon 080929
		case SET_RGB_SOUND:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("");
			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal1Cmb.ShowWindow(FALSE);
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;

		//+add PSh 100310
		case SET_COMP_CVBS_CH:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("CH");
			for(nTmp = 0; nTmp < MAX_COMPONENT_CH; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;

	}
}

void CGrabPage::OnBtnSendAvswitchCmd() 
{
	UpdateData(TRUE);
	int nVal1, nVal2, nValue;
	nVal1 = nVal2  = nValue = 0;

	CString sTmp, sValue;
	sTmp = _T("");
	sValue = _T("");
	
	UINT nRtn = 0;

	m_ctrlAvSwitchRtnEdit.SetWindowText("");

	switch(m_nAvSwitchCmd) 
	{
		// Channel : 1 ~
		case SET_AUDIO_CH:
		{
			nVal1 = m_ctrlAvSwitchVal1Cmb.GetCurSel() + 1;		
		}
		break;

		case SET_COMP_CH: case SET_CVBS_CH:
		case SET_VIDEO_CH: case REMOTE_TYPE_SEL:
		case SET_COMP_CVBS_CH:  //+add PSh 100415
		{
			nVal1 = m_ctrlAvSwitchVal1Cmb.GetCurSel()+1;		
		}
		break;
		//+add kwmoon 080312
		case SET_REMOTE_REPEAT_NO :
		{
			nVal1 = m_ctrlAvSwitchVal1Cmb.GetCurSel();		
		}
		break;

		case SET_ANALOG_VOLT: 
		{
			nVal1 = m_ctrlAvSwitchVal1Cmb.GetCurSel() + 1;		
			nVal2 = m_ctrlAvSwitchVal2Cmb.GetCurSel();  
		}
		break;
		
		case SET_DIGIT_OUT:
		{
			nVal1 = m_ctrlAvSwitchVal1Cmb.GetCurSel() + 1; // Port	
			nVal2 = m_ctrlAvSwitchVal2Cmb.GetCurSel();   // On/Off
		}
		break;

		case CHK_AUDIO_LVL: case CHK_AUDIO_FRQ:
		{
			m_ctrlAvSwitchRstEdit.SetWindowText(_T(""));
			nVal1 = m_ctrlAvSwitchVal1Cmb.GetCurSel();		
		}
		break;
		
		case PULLUP_AVSWITCH:
		{
			if(m_ctrlAvSwitchVal1Cmb.GetCurSel() == 0)
			{
				nVal1 = 3;
			}
			else if(m_ctrlAvSwitchVal1Cmb.GetCurSel() == 1)
			{
				nVal1 = 5;
			}
		}
		break;
		case CHECK_STATUS:
		{
			
		}
		break;
	}

	if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
	{
		if(InitAvSwitchController() == FALSE) return;
	}
	
	if(AvSwitchBoxCtrl.SendCmd( m_nAvSwitchCmd ,MAX_AVSWITCH_WAIT_DELAY, sValue, nVal1, nVal2))
	{	
		if((m_nAvSwitchCmd == CHK_AUDIO_LVL) || (m_nAvSwitchCmd == CHK_AUDIO_FRQ))
		{
			if(m_nAvSwitchCmd == CHK_AUDIO_LVL)
			{
				nValue = (int)(atof(sValue) * 1000);
				sTmp.Format("%d mv", nValue);
			}
			else if(m_nAvSwitchCmd == CHK_AUDIO_FRQ)
			{
				nValue = atoi(sValue);
				sTmp.Format("%d hz", nValue);
			}
			
			m_ctrlAvSwitchRstEdit.SetWindowText(sTmp);
		}
		m_ctrlAvSwitchRtnEdit.SetWindowText("PASS");
	}
	else m_ctrlAvSwitchRtnEdit.SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
}



//+add kwmoon 071015 : [2in1]
LRESULT  CGrabPage::UpdateGrabStatus(WPARAM wParam, LPARAM lParam)
{
	//+change kwmoon 080628
	UpdateData(FALSE);
//	m_ctrlGrabStatus.SetWindowText(m_szGrabStatus);

	//+change kwmoon 080709
	//+add kwmoon 080419
//	if(m_MWnd)
//	{
//		//+change kwmoon 080628
//		m_MWnd->SendMessage(UM_UPDATE_IMAGE_VIEW_DISPLAY);	
//	}
	
	m_MWnd->InvalidateRect(&m_MWnd->m_ImgViewRect,FALSE);
	//+add kwmoon 080628
	m_MWnd->UpdateWindow();

	return 0;
}

void CGrabPage::OnDestroy() 
{
	//090919
	if(m_hPageGrabThreadKillEvent != NULL)
	{
		CloseHandle(m_hPageGrabThreadKillEvent); 
		m_hPageGrabThreadKillEvent = NULL;
	}

	CDialog::OnDestroy();
}

void CGrabPage::OnSelchangeCmbAvswitchValue1() 
{
	m_ctrlAvSwitchRtnEdit.SetWindowText("");	
}

void CGrabPage::OnSelchangeCmbAvswitchValue2() 
{
	m_ctrlAvSwitchRtnEdit.SetWindowText("");	
}

//+add kwmoon 080312
void CGrabPage::OnRdoAvswitchCmd12() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_REMOTE_REPEAT_NO;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);	
}

//+add kwmoon 080929
void CGrabPage::OnRdoAvswitchCmd13() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_RGB_SOUND;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);	
}

//+add kwmoon 080327
void CGrabPage::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int sDPMS_Count = 0;
	double lReadVal;
	CString StrMsg;
	CString sTmp;
	CString strReadData2 = _T("DPMS>> PC ");


	switch (nIDEvent)
	{
	case _DPMSWAIT_ID:
		if (gJigCtrl.m_nDPMS_ReceiveCnt >= 7)
		{			
			if ((gJigCtrl.m_strDPMS_Receive[0] == 0x02)
				&& (gJigCtrl.m_strDPMS_Receive[1] == 0x03))
				//&&(m_strDPMS_Receive[2] == 0x02))
			{
				lReadVal = (gJigCtrl.m_strDPMS_Receive[3] * 256. + gJigCtrl.m_strDPMS_Receive[4]) / 10.0;
				gJigCtrl.m_fDPMS_WattData = lReadVal;
				for (int idx1 = 0; idx1 < gJigCtrl.m_nDPMS_ReceiveCnt; idx1++)
				{
					//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);

					sTmp.Format("%02X ", gJigCtrl.m_strDPMS_Receive[idx1]);
					strReadData2 = strReadData2 + sTmp;
				}
				//strReadData2 += str;
				AddStringToStatus(strReadData2);
				StrMsg.Format("%2.1f W", lReadVal);
				SetDlgItemText(IDC_STATIC_DPMS_VALUE, StrMsg);
				KillTimer(_DPMSWAIT_ID);
				sDPMS_Count = 0;
			}
			else
			{
				sDPMS_Count = 0;
				SetDlgItemText(IDC_STATIC_DPMS_VALUE, "Fail!");
				KillTimer(_DPMSWAIT_ID);
			}
		}
		else
		{
			sDPMS_Count++;
			if (sDPMS_Count > 50)
			{
				sDPMS_Count = 0;
				SetDlgItemText(IDC_STATIC_DPMS_VALUE, "Time Out!");
				KillTimer(_DPMSWAIT_ID);
			}
			else
			{
				StrMsg.Format("Rcv Wait %d", sDPMS_Count/10+1);
				SetDlgItemText(IDC_STATIC_DPMS_VALUE, StrMsg);
			}
			
			
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

//+add kwmoon 080419
void CGrabPage::ChkLVDSAutoGrab() 
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NTSC);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_PAL);
	pButton->EnableWindow(FALSE);

	//+add 090420(Modification No1)
	pButton = (CButton*)GetDlgItem(IDC_RADIO_SECAM);
	pButton->EnableWindow(FALSE);

	m_nGrabDevice = LVDS_GRABBER;

	m_ctrlVideoOutGrabChk.SetCheck(FALSE);
	m_ctrlLvdsGrabChk.SetCheck(TRUE);

	OnChkAutoGrab();
}

//+add kwmoon 080417
void CGrabPage::OnRemoteButtonClick(UINT nButtonID)
{
	UINT nButton = nButtonID - IDC_BTN_REMOTE_DLG_1;
	CString sTmp = _T("");
	CString strName, sName, sCode, sDesc;
	strName = sName = sCode = sDesc = _T("");

//	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
//	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));
//	GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(_T(""));


	GetDlgItem(nButtonID)->GetWindowText(strName);

	if(!FindButtonKey(strName, sName, sCode, sDesc)) return;
	else
	{
		if(CurrentSet->bUseAVSwitchBox)
		{
			GetDlgItem(IDC_EDIT_REMOTE_CODE)->SetWindowText(sCode);
			//GetDlgItem(IDC_EDIT_DESC)->SetWindowText(sDesc);

			AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, sCode);

			m_ctrlSendRemoteRtnEdit.SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
		}
	}

	return;
}

//+add kwmoon 080628
void CGrabPage::ShowAvSwitchBoxCtrlGroup(BOOL bShow)
{
	CButton* pButton = NULL;

	for(int i=0; i<NO_AVSWITCHBOX_TAB_ITEM;i++)
	{
		pButton = (CButton*)GetDlgItem(aAvSwitchBoxCtrlItemNo[i]);
		pButton->ShowWindow(bShow);
	}
}

//+add kwmoon 080628
void CGrabPage::ShowGrabberOptionGroup(BOOL bShow)
{
	CButton* pButton = NULL;
	int i;


	if(g_nGrabberType == UHD_GRABBER){
		for (i = 0; i < 2; i++)//for (i = 0; i < 21; i++)
		{
			pButton = (CButton*)GetDlgItem(aGrabOptionItemNo[i]);
			pButton->ShowWindow(bShow);
		} 

		for(i = 18; i<NO_GRAB_OPTION_TAB_ITEM; i++)
		{
			pButton = (CButton*)GetDlgItem(aGrabOptionItemNo[i]);
			pButton->ShowWindow(bShow);
		}
		for (i = 2; i < 18; i++)//for (i = 21; i < 37; i++)
		{
			pButton = (CButton*)GetDlgItem(aGrabOptionItemNo[i]);
			pButton->ShowWindow(FALSE);
		} 

	}
	else{
		for (i = 0; i < 18; i++)//for (i = 0; i < 37; i++)
		{
			pButton = (CButton*)GetDlgItem(aGrabOptionItemNo[i]);
			pButton->ShowWindow(bShow);
		} 
		for(i = 18; i<NO_GRAB_OPTION_TAB_ITEM; i++)
		{
			pButton = (CButton*)GetDlgItem(aGrabOptionItemNo[i]);
			pButton->ShowWindow(FALSE);
		}
	}
//	if(g_pView->m_nGrabber_FWVer < 0x10102301)
//	{
//		GetDlgItem(IDC_STATIC_3D_MODE)->EnableWindow(FALSE);
//		GetDlgItem(IDC_RDO_3D_MODE1)->EnableWindow(FALSE);
//		GetDlgItem(IDC_RDO_3D_MODE2)->EnableWindow(FALSE);
//		GetDlgItem(IDC_RDO_3D_MODE3)->EnableWindow(FALSE);
//	}
}

void CGrabPage::ShowUsbDioGroup(BOOL bShow)
{
	CButton* pButton = NULL;

	for(int i=0; i<NO_USB_DIO_TAB_ITEM;i++)
	{
		pButton = (CButton*)GetDlgItem(aUsbDioItemNo[i]);
		pButton->ShowWindow(bShow);

	}
	if(g_nSysType != AUTO_SYS){
		GetDlgItem(IDC_STATIC_EARPHONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_EARPHONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_COMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_COMP)->EnableWindow(FALSE);

		//GetDlgItem(IDC_STATIC_GRAB_POWER)->EnableWindow(FALSE);
		//GetDlgItem(IDC_CHECK_GRAB_POWER)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_STATIC_IFPACK_RESET)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_BTN_RESET_IFPACK)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC_GENDER_RESET)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BTN_RESET_GENDER)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC_JIG_UP_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MAIN_JIG_UP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_HDMI_USB_CYL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_HDMI_USB_CYL)->EnableWindow(FALSE);
	
		
	}
}

void CGrabPage::OnSelchangeControlTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nTabNo = m_ctrlControlTab.GetCurSel();
	
	switch(nTabNo)
	{
		case AVSWITCHBOX_CONTROL_TAB :
		{
			ShowAvSwitchBoxCtrlGroup(TRUE);
			ShowGrabberOptionGroup(FALSE);
			ShowUsbDioGroup(FALSE);
		}
		break;

		case GRABBER_OPTION_TAB :
		{
			ShowAvSwitchBoxCtrlGroup(FALSE);
			ShowGrabberOptionGroup(TRUE);
			ShowUsbDioGroup(FALSE);
		}
		break;
		case USBDIO_CONTROL_TAB :
		{
			ShowAvSwitchBoxCtrlGroup(FALSE);
			ShowGrabberOptionGroup(FALSE);
			ShowUsbDioGroup(TRUE);
		}
		break;

	}	
	*pResult = 0;
}

void CGrabPage::OnBtnGrabOptionApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString sMsg = _T("");

	int nTmp = 0;

	if(g_nGrabberType == UHD_GRABBER)
	{
		GrabStop();
		Sleep(100);

		CurrentSet->nFullHd120Hz = 0;
		CurrentSet->nImageRotation = 0;

		switch(m_nLvdsResolutionIndex) 
		{
			case 0: 
				CurrentSet->nLvdsWidth = 1024; CurrentSet->nLvdsHeight = 768;		break;
			case 1: 
				CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 768;		break;
			case 2: 
				CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 1024;		break;
			case 3: 
				CurrentSet->nLvdsWidth = 1364; CurrentSet->nLvdsHeight = 768;		break;
			case 4: 
				CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
			case 5: 
				CurrentSet->nLvdsWidth = 1440; CurrentSet->nLvdsHeight = 900;		break;
			case 6: 
				CurrentSet->nLvdsWidth = 1600; CurrentSet->nLvdsHeight = 900;		break;
			case 7: 
				CurrentSet->nLvdsWidth = 1680; CurrentSet->nLvdsHeight = 1050;		break;
	
			case 8: 
				CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;	
				CurrentSet->nFullHd120Hz = 0;
				break;

			case 9: 
				CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;	
				CurrentSet->nFullHd120Hz = 1;
				break;

			case 10: 
				CurrentSet->nLvdsWidth = 2560; CurrentSet->nLvdsHeight = 1080;	
				CurrentSet->nFullHd120Hz = 1;
				break;

			case 11: 
				CurrentSet->nLvdsWidth = 2560; CurrentSet->nLvdsHeight = 1440;			
				CurrentSet->nFullHd120Hz = 1;
				break;
			case 12: 
				CurrentSet->nLvdsWidth = 3440;  CurrentSet->nLvdsHeight = 1440;		break;

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

			default :
				CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
		}

		CurrentSet->nLVDS_Mode = m_ctrlLvdsModeCmb.GetCurSel();
		CurrentSet->nUHD_Type =	m_ctrlUhdType.GetCurSel();
		CurrentSet->nUHD_Grab_BitShift = m_ctrlBitShift.GetCurSel();
		CurrentSet->nUHD_Grab_Mode = m_ctrlGrabMode.GetCurSel();


		CurrentSet->nUHD_Y20_SW_Mode = m_cComboY20SwMode.GetCurSel();		
	
		//int lSelID = 0;
		//CString strTmp;
		//
		//lSelID = m_cComboY20SwMode.GetCurSel();
		//m_cComboY20SwMode.GetLBText(lSelID, strTmp);

		//if (strTmp.Find("SW_170_") == 0)
		//{
		//	CurrentSet->nUHD_Y20_SW_Mode = _ttoi(strTmp.Mid(7));
		//}
		//else
		//{
		//	CurrentSet->nUHD_Y20_SW_Mode = lSelID;
		//}




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

		CurrentSet->nImageRotation = m_bImage_Rotation;
		CurrentSet->nRedBlueSwap = m_bRedBlueSwap ;
		CurrentSet->nDeHsyncEnable = m_bDeHsyncEnable;
		CurrentSet->nHyncEdgeEnable = m_bHyncEdgeEnable;
	

		GrabStart(m_nGrabMode);	
	}
	else{
		switch(m_nUranousMode) 
		{
			//0 : JEIDA_DUAL(0x81), 1 : JEIDA_SINGLE(0x01), 2 : NS_DUAL(0x80), 3 : NS_SINGLE(0x00)

			case 0 :  nTmp = 0x84; break; // JEIDA_DUAL
			case 1 :  nTmp = 0x04; break; // JEIDA_SINGLE
			case 2 :  nTmp = 0x80; break; // NS_DUAL
			case 3 :  nTmp = 0x00; break; // NS_SINGLE
	
			default : nTmp = 0x00; break; // NS_SINGLE
		}
	
		CurrentSet->nFullHd120Hz = 0;
		CurrentSet->nImageRotation = 0;

		switch(m_nLvdsResolutionIndex) 
		{
			case 0: 
				CurrentSet->nLvdsWidth = 852;  CurrentSet->nLvdsHeight = 480;		break;
			case 1: 
				CurrentSet->nLvdsWidth = 1024; CurrentSet->nLvdsHeight = 768;		break;
			case 2: 
				CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 768;		break;
			case 3: 
				CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 1024;		break;
			case 4: 
				CurrentSet->nLvdsWidth = 1364; CurrentSet->nLvdsHeight = 768;
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 0))
				{
					nTmp = nTmp | 0x10;
				}
			break;
			case 5: 
				CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
			case 6: 
				CurrentSet->nLvdsWidth = 1440; CurrentSet->nLvdsHeight = 900;		break;
			case 7: 
				CurrentSet->nLvdsWidth = 1600; CurrentSet->nLvdsHeight = 900;		break;
			case 8: 
				CurrentSet->nLvdsWidth = 1680; CurrentSet->nLvdsHeight = 1050;		break;
	
			case 9: 
				CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;	
				CurrentSet->nFullHd120Hz = 0;
				// change psh 090630
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			break;

			case 10: 
				CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;	
				CurrentSet->nFullHd120Hz = 1;
				// change psh 090630
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			break;

			case 11: 
				CurrentSet->nLvdsWidth = 2560; CurrentSet->nLvdsHeight = 1080;	
				CurrentSet->nFullHd120Hz = 1;
				// change psh 090630
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
			break;

			case 12: 
				CurrentSet->nLvdsWidth = 2560; CurrentSet->nLvdsHeight = 1440;			
				CurrentSet->nFullHd120Hz = 1;
				CurrentSet->bOddDE_Only = m_bOddDE_Only;
				if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
				{
					nTmp = nTmp | 0x10;
				}
				break;

			default :
				CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
		}

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




		//add psh 091214
		CurrentSet->nImageRotation = m_bImage_Rotation;

		CurrentSet->nRedBlueSwap = m_bRedBlueSwap;

		CurrentSet->nDeHsyncEnable = m_bDeHsyncEnable;
		CurrentSet->nHyncEdgeEnable = m_bHyncEdgeEnable;

		g_pView->SetGrabInfo(&m_Image1);
	} 

}
//
//void CGrabPage::OnRdoLvdsResolution1(UINT nButtonID)
//{
//	switch(nButtonID) 
//	{
//		case IDC_RDO_LVDS_RESOLUTION1: 
//			m_nLvdsResolutionIndex	 = 0;	
//			m_nNewLvdsWidth			= 1024; 
//			m_nNewLvdsHeight		= 768;  
//		break;
//		
//		case IDC_RDO_LVDS_RESOLUTION2:
//			m_nLvdsResolutionIndex	= 1;	
//			m_nNewLvdsWidth			= 1280; 
//			m_nNewLvdsHeight		= 768;  
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION3: 
//			m_nLvdsResolutionIndex	= 2;	
//			m_nNewLvdsWidth			= 1280; 
//			m_nNewLvdsHeight		= 1024; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION4: 
//			m_nLvdsResolutionIndex	= 3;
//			m_nNewLvdsWidth			= 1364; 
//			m_nNewLvdsHeight		= 768; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION5: 
//			m_nLvdsResolutionIndex	= 4;	
//			m_nNewLvdsWidth			= 1366; 
//			m_nNewLvdsHeight		= 768;  
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION6: 
//			m_nLvdsResolutionIndex	= 5;	
//			m_nNewLvdsWidth			= 1440; 
//			m_nNewLvdsHeight		= 900;  
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION7: 
//			m_nLvdsResolutionIndex	= 6;	
//			m_nNewLvdsWidth			= 1600; 
//			m_nNewLvdsHeight		= 900;  
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION8: 
//			m_nLvdsResolutionIndex	= 7;	
//			m_nNewLvdsWidth			= 1680; 
//			m_nNewLvdsHeight		= 1050; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION9: 
//			m_nLvdsResolutionIndex	= 8;
//			m_nNewLvdsWidth			= 1920; 
//			m_nNewLvdsHeight		= 1080; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION10: 
//			m_nLvdsResolutionIndex	= 9;
//			m_nNewLvdsWidth			= 1920; 
//			m_nNewLvdsHeight		= 1080; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION11: 
//			m_nLvdsResolutionIndex	= 10;
//			m_nNewLvdsWidth			= 2560; 
//			m_nNewLvdsHeight		= 1080; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION12: 
//			m_nLvdsResolutionIndex	= 11;
//			m_nNewLvdsWidth			= 2560; 
//			m_nNewLvdsHeight		= 1440; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION13: 
//			m_nLvdsResolutionIndex	= 12;		
//			m_nNewLvdsWidth			= 3440; 
//			m_nNewLvdsHeight		= 1440;   
//		break;
//		
//
//		case IDC_RDO_LVDS_RESOLUTION14: 
//			m_nLvdsResolutionIndex	= 13;
//			m_nNewLvdsWidth			= 3840; 
//			m_nNewLvdsHeight		= 2160; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION15: 
//			m_nLvdsResolutionIndex	= 14;
//			m_nNewLvdsWidth			= 1920; 
//			m_nNewLvdsHeight		= 300; 
//			break;
//
//		case IDC_RDO_LVDS_RESOLUTION16:
//			m_nLvdsResolutionIndex = 15;
//			m_nNewLvdsWidth = 3840;
//			m_nNewLvdsHeight = 600;
//			break;
//		case IDC_RDO_LVDS_RESOLUTION17:
//			m_nLvdsResolutionIndex = 16;
//			m_nNewLvdsWidth = 1920;
//			m_nNewLvdsHeight = 540;
//			break;
//		case IDC_RDO_LVDS_RESOLUTION18:
//			m_nLvdsResolutionIndex = 17;
//			m_nNewLvdsWidth = 1920;
//			m_nNewLvdsHeight = 1200;
//			break;
//		case IDC_RDO_LVDS_RESOLUTION19:
//			m_nLvdsResolutionIndex = 18;//W_2048_H_2560
//			m_nNewLvdsWidth = 2048;
//			m_nNewLvdsHeight = 2560;
//			break;
//		case IDC_RDO_LVDS_RESOLUTION20:
//			m_nLvdsResolutionIndex = W_3328_H_1440;//W_2048_H_2560
//			m_nNewLvdsWidth = 3328;
//			m_nNewLvdsHeight = 1440;
//			break;
//		case IDC_RDO_LVDS_RESOLUTION20:
//			m_nLvdsResolutionIndex = W_3584_H_740;
//			m_nNewLvdsWidth = 3854;
//			m_nNewLvdsHeight = 740;
//			break;
//
//		default : 
//			m_nLvdsResolutionIndex	= 5;
//			m_nNewLvdsWidth			= 1366; 
//			m_nNewLvdsHeight		= 768;	
//			break;
//	}
//}

void CGrabPage::OnRdoColorDepth1(UINT nButtonID)
{
	switch(nButtonID) 
	{
		case IDC_RDO_COLOR_DEPTH1: 
			m_nBitMode	= 0;		
			m_nNewBitCount		= 6*3;
		break;
		
		case IDC_RDO_COLOR_DEPTH2: 
			m_nBitMode	= 1;	
			m_nNewBitCount		= 8*3;
		break;
		
		case IDC_RDO_COLOR_DEPTH3:
			m_nBitMode	= 2;	
			m_nNewBitCount		= 10*3;
			break;

		case IDC_RDO_COLOR_DEPTH4: 
			m_nBitMode	= 3;	
			m_nNewBitCount		= 12*3;
			break;

		default : 
			m_nBitMode	= 1;
			m_nNewBitCount		= 8*3;
			break;
	}
}
/*
void CGrabPage::OnRdoLvdsMode1(int nButtonID) 
{
	int nTmp = 0;

	switch(nButtonID) 
	{
		case IDC_RDO_LVDS_MODE1: 
			m_nUranousMode	= 0;		
			nTmp					= 0x84; //JEIDA_DUAL
		break;
		
		case IDC_RDO_LVDS_MODE2: 
			m_nUranousMode	= 1;	
			nTmp					= 0x04; //JEIDA_SINGLE
		break;
		
		case IDC_RDO_LVDS_MODE3:
			m_nUranousMode	= 2;	
			nTmp					= 0x80; //NS_DUAL
			break;

		case IDC_RDO_LVDS_MODE4: 
			m_nUranousMode	= 3;	
			nTmp					= 0x00; //NS_SINGLE
			break;


		default : 
			m_nUranousMode	= 3;	
			nTmp					= 0x00; //NS_SINGLE
			break;
	}

	switch(m_nBitMode) 
	{
		case 0:   m_nNewLVDSMode = nTmp;  	     break;
		case 1:	  m_nNewLVDSMode = (nTmp | 0x1); break;
		case 2:	  m_nNewLVDSMode = (nTmp | 0x2); break;
		case 3:	  m_nNewLVDSMode = (nTmp | 0x3); break;
		default : m_nNewLVDSMode = (nTmp | 0x3); break;
	}	
}
*/

void CGrabPage::OnRdo3dMode1(UINT nButtonID) 
{
	switch(nButtonID) 
	{
		case IDC_RDO_3D_MODE1: 
			m_n3DMode	= 0;		
		break;
		
		case IDC_RDO_3D_MODE2: 
			m_n3DMode	= 1;		
		break;
		
		case IDC_RDO_3D_MODE3:
			m_n3DMode	= 2;		
			break;

		default : 
			m_n3DMode	= 0;		
			break;
	}
}


void CGrabPage::OnChkEdid() 
{
	BOOL bReturn;

	if(m_ctrlEdidChk.GetCheck()) 
	{
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;  
	}
	else
	{
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;  
	}

	if(!bReturn)
	{
		CString szErrMsg;
		szErrMsg.Format(_T("Failed to Set EDID(HDMI_Gen)"));
		AfxMessageBox(szErrMsg);
	}

}
void CGrabPage::OnChkHdcp() 
{
	BOOL bReturn;

	if(m_ctrlHdcpChk.GetCheck()) 
	{
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE) ;  
	}
	else
	{
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE) ;  
	}
	if(!bReturn)
	{
		CString szErrMsg;
		szErrMsg.Format(_T("Failed to Set HDCP(HDMI_Gen)"));
		AfxMessageBox(szErrMsg);
	}
}

void CGrabPage::OnChkOnline2() 
{
	if(m_ctrlOnlineChk2.GetCheck()) 
	{
		CString szCmdString = _T("");
		szCmdString.Format("%c",START_TERMINAL_MODE);

		// Send On-Line Mode On Cmd
		HDMIGeneratorCtrl.SendCommString(szCmdString);  

		HDMIGeneratorCtrl.m_nRemoteMode = ONLINE;
	}
	else
	{
		CString szCmdString = _T("");
		szCmdString.Format("%c",END_TERMINAL_MODE);
		
		// Send On-Line Mode On Cmd
		HDMIGeneratorCtrl.SendCommString(szCmdString);  

		HDMIGeneratorCtrl.PortClose();

		HDMIGeneratorCtrl.m_nRemoteMode = OFFLINE;
	}
}

void CGrabPage::OnBtnSendModel2() 
{
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);
	BOOL bReturn;

	// Port Open
	if(HDMIGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate) == FALSE)
		{
			HDMIGeneratorCtrl.PortClose();
			HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	HDMIGeneratorCtrl.SendCommString(szCmdString);  

	CString sTmp = _T("");
	UINT nNumber = 0;

	m_ctrlModelEdit2.GetWindowText(sTmp);
	nNumber = atoi(sTmp);
	
	bReturn = HDMIGeneratorCtrl.SetModel((int)nNumber);
	if(!bReturn)
	{
		AfxMessageBox(_T("Failed to Set Model(HDMI_Gen)"));
		HDMIGeneratorCtrl.m_nRemoteMode = 0;
		m_ctrlOnlineChk2.SetCheck( FALSE);
	}
	else{
		HDMIGeneratorCtrl.m_nRemoteMode = ONLINE;
		m_ctrlOnlineChk2.SetCheck( TRUE);
	}
}

void CGrabPage::OnBtnSendPattern2() 
{
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);
	BOOL bReturn;

	// Port Open
	if(HDMIGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate) == FALSE)
		{
			HDMIGeneratorCtrl.PortClose();
			HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	HDMIGeneratorCtrl.SendCommString(szCmdString);  

	CString sTmp = _T("");
	UINT nNumber = 0;
	
	m_ctrlPatternEdit2.GetWindowText(sTmp);
	nNumber = atoi(sTmp);
	bReturn = HDMIGeneratorCtrl.SetPattern((int)nNumber);
	if(!bReturn)
	{
		AfxMessageBox(_T("Failed to Set Pattern(HDMI_Gen)"));
		HDMIGeneratorCtrl.m_nRemoteMode = 0;
		m_ctrlOnlineChk2.SetCheck(FALSE);
	}
	else{
		HDMIGeneratorCtrl.m_nRemoteMode = ONLINE;
		m_ctrlOnlineChk2.SetCheck( TRUE);
	}
}
void CGrabPage::SetHDMIGenGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_GRAB_GROUP_7)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MODEL_NUMBER2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MODEL2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEND_MODEL2)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_PATTERN_NUMBER2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_PATTERN2)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEND_PATTERN2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHK_ONLINE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHK_HDCP)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHK_EDID)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHK_DDCLINE)->EnableWindow(bEnable);
}

void CGrabPage::OnChkDdcline() 
{
	BOOL bReturn;

	if(CurrentSet->nHDMIGenType == 0){ return;}

	if(m_ctrlDDCLineChk.GetCheck()) 
	{
		bReturn = HDMIGeneratorCtrl.SetDDCLine_OpenClose(FALSE) ;  
	}
	else
	{
		bReturn = HDMIGeneratorCtrl.SetDDCLine_OpenClose(TRUE) ;  
	}

	if(!bReturn)
	{
		CString szErrMsg;
		szErrMsg.Format(_T("Failed to Set DDCLine(HDMI_Gen)"));
		AfxMessageBox(szErrMsg);
	}

}

void CGrabPage::OnRadioSecam() 
{
	CurrentSet->nAnalogWidth  = 704;
	CurrentSet->nAnalogHeight = 572;
//	CurrentSet->nAnalogWidth  = 768; 
//	CurrentSet->nAnalogHeight = 576;
	m_nGrabDevice = ANALOG_GRABBER;
	CurrentSet->nAnalogType   = V_SRC_SECAM;

	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			GrabStop();
		}
	}
	if(g_nGrabberType == FHD_GRABBER)
	{
		g_pView->SetGrabInfo(&m_Image1);
	}
	if(m_bThreadRunning != TRUE)
	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
	}

	UpdateData(TRUE);	
}
void CGrabPage::OnChkOddDeOnly() 
{
//	m_bLVDS_Option = m_bOddDE_Only;
//	m_bLVDS_Option = m_ctrlOddDE_Only.GetCheck();
	UpdateData(TRUE);	
	
}

void CGrabPage::OnChkImageRotation() 
{
	UpdateData(TRUE);	
	
}

void CGrabPage::OnRdoAvswitchCmd14() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_COMP_CVBS_CH;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);	
}

void CGrabPage::OnSelchangeComboOutPort() 
{
	int nPort;
	
	UpdateData(TRUE);	
	nPort = m_ctrlHdmiOutPortCmb.GetCurSel();
	HDMIGeneratorCtrl.SetOutPort(nPort);
}


void CGrabPage::OnCheckRelay1() 
{
	m_bRelay1 = m_ctrlRelay1Chk.GetCheck();	
//AUTO
	if(g_nSysType == AUTO_SYS){
		gJigCtrl.Set_ACPower((BYTE)m_bRelay1);
	}
	else{
		g_pView->Set_Relay(m_bRelay1);
	}
}

void CGrabPage::OnRadioMhl(UINT nButtonID) 
{
	int nPort;

	UpdateData(TRUE);	

	switch(nButtonID) 
	{
		case IDC_RADIO_MHL: 
			m_nMhlMode	= 0;		
		break;
		
		case IDC_RADIO_MHL2: 
			m_nMhlMode	= 1;	
		break;
	}
	if(CurrentSet->nHDMIGenType == 1){
	/*	if(m_nMhlMode == 0){
			HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;
			Sleep(100);
			HDMIGeneratorCtrl.SetCEC_OnOff(FALSE);
		}
		else{
			HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;
			Sleep(100);
			HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);
		}  */
		g_pView->Set_Mhl(1, m_nMhlMode);
	}
	else{
		nPort = m_ctrlHdmiOutPortCmb2.GetCurSel();
		if(m_nMhlMode == 0){
			HDMIGeneratorCtrl.SetMHL_OnOff(nPort, TRUE);
		}
		else{
			HDMIGeneratorCtrl.SetMHL_OnOff(nPort, FALSE);
		}

	}
}

void CGrabPage::OnCheckArc() 
{
	int nPort;
	
	UpdateData(TRUE);	
	nPort = m_ctrlHdmiOutPortCmb2.GetCurSel();

	m_bHdmiGenArc = m_ctrlArcChk.GetCheck();
	HDMIGeneratorCtrl.SetARC_OnOff(nPort, m_bHdmiGenArc);	
}

void CGrabPage::OnCheckMhlLine() 
{
	UpdateData(TRUE);	
	m_bMhlDetect = m_ctrlMhlDetChk.GetCheck();
	g_pView->Set_Mhl(2, m_bMhlDetect);
}

void CGrabPage::OnBtnAvcCom() 
{
	CString sTemp;
	int nLength;
	CString sValue;

	m_ctrlAvcInputSelectCmb.SetWindowText(_T(""));

	TVCommCtrl.Send_TVControl_Command ("mc 00 44", 2000);

	if(gAvConvertorCtrl.CheckComData(nLength, sValue)){
		sTemp.Format(_T("%s : %d - %s"), gAvConvertorCtrl.m_strErrMsg, nLength, sValue);
		m_ctrlAvcInputSelectCmb.SetWindowText(sTemp);
	}
	else{
		m_ctrlAvcInputSelectCmb.SetWindowText(gAvConvertorCtrl.m_strErrMsg);
	}
}

void CGrabPage::OnSelchangeCmbAvcInput() 
{
	m_ctrlAvcInputSelectCmb.SetWindowText(_T(""));
	CurrentSet->nHDMI_Grab_Input =	m_ctrlAncInputCmb.GetCurSel();
	gAvConvertorCtrl.SetInputSignal(CurrentSet->nHDMI_Grab_Input);
	m_ctrlAvcInputSelectCmb.SetWindowText(gAvConvertorCtrl.m_strErrMsg);
}

void CGrabPage::OnBtnAvcIr() 
{
	CString sTemp;
	int nLength;
	CString sValue;
	CString strRead, strIR_KeyData, str33Data, strMsg;

	TVCommCtrl.Send_TVControl_Command("av 00 25", 1000);
	if (gVF1000Ctrl.GetIRBlasterData(strRead, 1000))
	{
		Sleep(500);


		strIR_KeyData = strRead.Left(8);
		str33Data = strRead.Mid(8);
		str33Data.Insert(1, '.');

		strMsg = "IR: ";
		strMsg += strIR_KeyData;
		strMsg += " DC: ";
		strMsg += str33Data;
		strMsg += "V";

		AddStringToStatus(strMsg);
		m_ctrlAvcInputSelectCmb.SetWindowText(strMsg);

	}
	else 
	{
		m_ctrlAvcInputSelectCmb.SetWindowText("READ ERROR");
	}
	TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);

	//m_ctrlAvcInputSelectCmb.SetWindowText(_T(""));
	//AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b");
	//if(gAvConvertorCtrl.CheckRemoteCmd(nLength, sValue)){
	//	sTemp.Format(_T("%s : %d - %s"), gAvConvertorCtrl.m_strErrMsg, nLength, sValue);
	//	m_ctrlAvcInputSelectCmb.SetWindowText(sTemp);
	//}
	//else{
	//	m_ctrlAvcInputSelectCmb.SetWindowText(gAvConvertorCtrl.m_strErrMsg);
	//}
	
}

void CGrabPage::OnBtnAvcBuf() 
{
	m_ctrlAvcInputSelectCmb.SetWindowText(_T(""));
	gAvConvertorCtrl.CheckBufferClear();
	m_ctrlAvcInputSelectCmb.SetWindowText(gAvConvertorCtrl.m_strErrMsg);
}
void CGrabPage::OnCheckEarphone() 
{
	m_bEarphone = m_ctrlEarphone.GetCheck();	
	gJigCtrl.Set_CylinderEarphone(m_bEarphone);
	
}
void CGrabPage::OnCheckComp() 
{
	m_bComp = m_ctrlComp.GetCheck();	
	gJigCtrl.Set_CylinderComp(m_bComp);
	
}




void CGrabPage::OnBnClickedChkCec()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);
	BOOL bReturn;

	if (m_ctrlCECChk.GetCheck())
	{
		bReturn = HDMIGeneratorCtrl.SetCEC_OnOff(TRUE);
	}
	else
	{
		bReturn = HDMIGeneratorCtrl.SetCEC_OnOff(FALSE);
	}
	if (!bReturn)
	{
		CString szErrMsg;
		szErrMsg.Format(_T("Failed to Set CEC(HDMI_Gen)"));
		AfxMessageBox(szErrMsg);
	}
}


void CGrabPage::OnBnClickedCheckGrabPower()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bGrabPower = m_ctrlGrabPower.GetCheck();
	gJigCtrl.Set_Grabber_Power(m_bGrabPower);

	
}


void CGrabPage::OnBnClickedBtnResetIfpack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gJigCtrl.Set_IF_Pack_Reset(0);
	//g_pView->IF_Pack_Reset();
}


void CGrabPage::OnBnClickedBtnResetGender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gJigCtrl.Set_Gender_Reset(0);
}


void CGrabPage::OnBnClickedCheckHdmiUsbCyl()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bCheckHdmiUsbCyl = m_cCheckHdmiUsbCyl.GetCheck();
	gJigCtrl.Set_CylinderHDMI_USB(m_bCheckHdmiUsbCyl);
}


void CGrabPage::OnBnClickedBtnMainJigUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gJigCtrl.Set_TestResult(0x03);
}


void CGrabPage::OnBnClickedBtnMainJigDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//if (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside)

	//else
	{
		gJigCtrl.Set_CylinderDOWN();
	}
}



void CGrabPage::OnBnClickedBtnCommandModelName()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData();
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_MODEL_NAME;

	gPLC_Ctrl.WriteFullData(Address, m_strEditModelName.GetBuffer(), m_strEditModelName.GetLength());
}


void CGrabPage::OnBnClickedBtnCommandLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_LOAD_COMMAND;
	char Buff[256];

	if (gPLC_Ctrl.m_cCheckLoad == 1)
	{
		gPLC_Ctrl.m_cCheckLoad = 0;
	}
	else
	{
		gPLC_Ctrl.m_cCheckLoad = 1;
	}
	Buff[0] = gPLC_Ctrl.m_cCheckLoad;


	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}


void CGrabPage::OnBnClickedBtnCommandPassUnload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_COMMAND;
	char Buff[256];

	if (gPLC_Ctrl.m_cCheckUnLoad == 1)
	{
		gPLC_Ctrl.m_cCheckUnLoad = 0;
		Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;

		Buff[1] = 0; //OK
		Buff[2] = 0; //NG
	}
	else
	{
		gPLC_Ctrl.m_cCheckUnLoad = 1;
		Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;

		Buff[1] = 1; //OK
		Buff[2] = 0; //NG
	}

	gPLC_Ctrl.WriteCommand(Address, Buff, 3);
}


void CGrabPage::OnBnClickedBtnCommandNgUnload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_COMMAND;
	char Buff[256];

	if (gPLC_Ctrl.m_cCheckUnLoad == 1)
	{
		gPLC_Ctrl.m_cCheckUnLoad = 0;
		Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;
		Buff[1] = 0; //OK
		Buff[2] = 0; //NG
	}
	else
	{
		gPLC_Ctrl.m_cCheckUnLoad = 1;

		Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;
		Buff[1] = 0; //OK
		Buff[2] = 1; //NG
	}

	gPLC_Ctrl.WriteCommand(Address, Buff, 3);
}



void CGrabPage::OnBnClickedBtnCommandFfcRetry()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_FFC_RETRY_COMMAND;
	char Buff[256];

	if (gPLC_Ctrl.m_cCheckReconnect == 1)
	{
		gPLC_Ctrl.m_cCheckReconnect = 0;
	}
	else
	{
		gPLC_Ctrl.m_cCheckReconnect = 1;
	}
	Buff[0] = gPLC_Ctrl.m_cCheckReconnect;
	//Buff[0] = 1;
	//	Buff[1] = 0; //OK
	//	Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}


void CGrabPage::OnBnClickedBtnCommandRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int lSel = m_cComboDFT_SEL.GetCurSel();

	//UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;
	//gPLC_Ctrl.ReadCommand(l_PLC_BaseAddress, 100);
	gPLC_Ctrl.CommandRegRead();
}


void CGrabPage::OnBnClickedChkRbSwapGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void CGrabPage::OnBnClickedChkDeHsyncGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	 g_ImageProc.HsyncModeChange(m_bDeHsyncEnable, m_bHyncEdgeEnable);
}


void CGrabPage::OnBnClickedChkHsyncEdgeGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	g_ImageProc.HsyncModeChange(m_bDeHsyncEnable, m_bHyncEdgeEnable);
}


void CGrabPage::OnBnClickedBtnResetGrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pView->GrabBaseResetStartThread(); 
}


void CGrabPage::OnCbnSelchangeComboGrabmode2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBtnGrabOptionApply();
}


void CGrabPage::OnCbnSelchangeComboUhdtype2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBtnGrabOptionApply();
}


void CGrabPage::OnBnClickedBtnLanTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strRead;
	if (gVF1000Ctrl.CheckLanHDCP(strRead, 1000) == 0)
	{
		SetDlgItemText(IDC_EDIT_LAN_OUT,"FAIL");//return FALSE;
	};
	if (strRead.Find("OK") >= 0)
	{
		//if (nStepNo == 0)
		//{
		//	sTmp = "HDMI Gen Ver :";
		//	sTmp += HDMIGeneratorCtrl.m_FW_Ver;
		//	m_CtrlListMainProcess.SetItemText(nStepNo, 11, sTmp); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);
		//}
		//g_pView->SaveMessageList("LAN DHCP: OK");
		//AddStringToStatus("LAN DHCP: OK");
		SetDlgItemText(IDC_EDIT_LAN_OUT, "OK");//return TRUE;
	}
	else
	{
		//g_pView->SaveMessageList("LAN DHCP: NG");
		//AddStringToStatus("LAN DHCP: NG");
		SetDlgItemText(IDC_EDIT_LAN_OUT, "NG");//return FALSE;
	}

}


void CGrabPage::OnBnClickedBtnLanClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strRead;
	if (gVF1000Ctrl.CheckLanClear(strRead, 1000) == 0)
	{
		SetDlgItemText(IDC_EDIT_LAN_OUT, "FAIL");//return FALSE;
	}
	else 
	{
		
		SetDlgItemText(IDC_EDIT_LAN_OUT, "PASS");//return TRUE;
	}
	
}


void CGrabPage::OnBnClickedBtnDpmsCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//double ldReturn;
	
	if (gJigCtrl.DPMS_Read_Check(1))
	{
		SetTimer(_DPMSWAIT_ID, 100, NULL);
		SetDlgItemText(IDC_STATIC_DPMS_VALUE, "Rev Wait");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_DPMS_VALUE, "Send Fail");
	}
	//CString StrMsg;
	//StrMsg.Format("%2.1f mW",ldReturn);
	//SetDlgItemText(IDC_EDIT_LAN_OUT, StrMsg);
}


void CGrabPage::OnCbnSelchangeComboLvdsResolution2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
	UINT nButtonID;
	m_nLvdsResolutionIndex = m_cComboLvdsFormat.GetCurSel();
	switch (m_nLvdsResolutionIndex)
	{
	case 0:
		m_nLvdsResolutionIndex = 0;
		m_nNewLvdsWidth = 1024;
		m_nNewLvdsHeight = 768;
		break;

	case 1:
		m_nLvdsResolutionIndex = 1;
		m_nNewLvdsWidth = 1280;
		m_nNewLvdsHeight = 768;
		break;

	case 2:
		m_nLvdsResolutionIndex = 2;
		m_nNewLvdsWidth = 1280;
		m_nNewLvdsHeight = 1024;
		break;

	case 3:
		m_nLvdsResolutionIndex = 3;
		m_nNewLvdsWidth = 1364;
		m_nNewLvdsHeight = 768;
		break;

	case 4:
		m_nLvdsResolutionIndex = 4;
		m_nNewLvdsWidth = 1366;
		m_nNewLvdsHeight = 768;
		break;

	case 5:
		m_nLvdsResolutionIndex = 5;
		m_nNewLvdsWidth = 1440;
		m_nNewLvdsHeight = 900;
		break;

	case 6:
		m_nLvdsResolutionIndex = 6;
		m_nNewLvdsWidth = 1600;
		m_nNewLvdsHeight = 900;
		break;

	case 7:
		m_nLvdsResolutionIndex = 7;
		m_nNewLvdsWidth = 1680;
		m_nNewLvdsHeight = 1050;
		break;

	case 8:
		m_nLvdsResolutionIndex = 8;
		m_nNewLvdsWidth = 1920;
		m_nNewLvdsHeight = 1080;
		break;

	case 9:
		m_nLvdsResolutionIndex = 9;
		m_nNewLvdsWidth = 1920;
		m_nNewLvdsHeight = 1080;
		break;

	case 10:
		m_nLvdsResolutionIndex = 10;
		m_nNewLvdsWidth = 2560;
		m_nNewLvdsHeight = 1080;
		break;

	case 11:
		m_nLvdsResolutionIndex = 11;
		m_nNewLvdsWidth = 2560;
		m_nNewLvdsHeight = 1440;
		break;

	case 12:
		m_nLvdsResolutionIndex = 12;
		m_nNewLvdsWidth = 3440;
		m_nNewLvdsHeight = 1440;
		break;


	case 13:
		m_nLvdsResolutionIndex = 13;
		m_nNewLvdsWidth = 3840;
		m_nNewLvdsHeight = 2160;
		break;

	case 14:
		m_nLvdsResolutionIndex = 14;
		m_nNewLvdsWidth = 1920;
		m_nNewLvdsHeight = 300;
		break;

	case 15:
		m_nLvdsResolutionIndex = 15;
		m_nNewLvdsWidth = 3840;
		m_nNewLvdsHeight = 600;
		break;
	case 16:
		m_nLvdsResolutionIndex = 16;
		m_nNewLvdsWidth = 1920;
		m_nNewLvdsHeight = 540;
		break;
	case 17:
		m_nLvdsResolutionIndex = 17;
		m_nNewLvdsWidth = 1920;
		m_nNewLvdsHeight = 1200;
		break;
	case 18:
		m_nLvdsResolutionIndex = 18;//W_2048_H_2560
		m_nNewLvdsWidth = 2048;
		m_nNewLvdsHeight = 2560;
		break;
	case 19:
		m_nLvdsResolutionIndex = W_3584_H_720;
		m_nNewLvdsWidth = 3854;
		m_nNewLvdsHeight = 720;
		break;
	case 20:
		m_nLvdsResolutionIndex = W_3328_H_1440;//W_2048_H_2560
		m_nNewLvdsWidth = 3328;
		m_nNewLvdsHeight = 1440;
		break;

	default:
		m_nLvdsResolutionIndex = 5;
		m_nNewLvdsWidth = 1366;
		m_nNewLvdsHeight = 768;
		break;
	}
}
