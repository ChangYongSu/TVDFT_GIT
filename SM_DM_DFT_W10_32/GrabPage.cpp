// GrabPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "GrabPage.h"
#include "ImageTotalDlg.h"
#include "RemoteDlg.h"
#include <mil.h> 
#include <malloc.h>
//+add kwmoon 080618
#include "RealMeas.h"
#include "global.h"
#include "UsbDio.h"
#include "PciDio.h"
#include "IrCheckerCtrl.h"
#include "thermometer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;
extern BYTE GrabData[1920][1080][3];
extern CBaseImage	g_GrabImage;
extern HWND g_hCommWnd;
//+add 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

// CGrabPage dialog

int aAvSwitchBoxCtrlItemNo[] = 
{
	IDC_RDO_AVSWITCH_CMD1,IDC_RDO_AVSWITCH_CMD2,IDC_RDO_AVSWITCH_CMD3,
	IDC_RDO_AVSWITCH_CMD4,IDC_RDO_AVSWITCH_CMD5,IDC_RDO_AVSWITCH_CMD6,
	IDC_RDO_AVSWITCH_CMD7,IDC_RDO_AVSWITCH_CMD8,IDC_RDO_AVSWITCH_CMD9,
	IDC_RDO_AVSWITCH_CMD10,IDC_RDO_AVSWITCH_CMD11,IDC_RDO_AVSWITCH_CMD12,
//	IDC_RDO_AVSWITCH_CMD13,IDC_RDO_AVSWITCH_CMD14,
	IDC_STATIC_AVSWITCH_VALUE_NAME1,IDC_STATIC_AVSWITCH_VALUE_NAME2,
	IDC_CMB_AVSWITCH_VALUE1,IDC_CMB_AVSWITCH_VALUE2,
	IDC_EDIT_AVSWITCH_RETURN,IDC_BTN_SEND_AVSWITCH_CMD,
	IDC_EDIT_AVSWITCH_RESULT,
	IDC_STATIC_AVC_GR,IDC_BTN_AVC_BUF,IDC_BTN_AVC_IR,IDC_EDIT_AVC_OUT,
	IDC_STATIC_TH,IDC_BTN_TH,IDC_EDIT_TH_OUT
};

int aUsbDioItemNo[] =
{
	IDC_STATIC_RELAY, IDC_CHECK_RELAY1, IDC_CHECK_RELAY2,
	IDC_CHECK_RELAY3, IDC_CHECK_RELAY4, IDC_CHECK_SELECT_ALL,
	IDC_BUTTON_SET2
};

int aRemoteItemNo[] =
{
	IDC_BTN_REMOTE_DLG_23, IDC_BTN_REMOTE_DLG_24, IDC_BTN_REMOTE_DLG_25, IDC_BTN_REMOTE_DLG_26,
	IDC_BTN_REMOTE_DLG_27, IDC_BTN_REMOTE_DLG_28, IDC_BTN_REMOTE_DLG_29, IDC_BTN_REMOTE_DLG_30,
	IDC_BTN_REMOTE_DLG_31, IDC_BTN_REMOTE_DLG_32, IDC_BTN_REMOTE_DLG_33, IDC_BTN_REMOTE_DLG_34,
	IDC_BTN_REMOTE_DLG_35, IDC_BTN_REMOTE_DLG_36, IDC_BTN_REMOTE_DLG_37, IDC_BTN_REMOTE_DLG_6,	
	IDC_BTN_REMOTE_DLG_7,  IDC_BTN_REMOTE_DLG_8,  IDC_BTN_REMOTE_DLG_9,  IDC_BTN_REMOTE_DLG_71,
	IDC_BTN_REMOTE_DLG_73, IDC_BTN_REMOTE_DLG_10, IDC_BTN_REMOTE_DLG_70, IDC_BTN_REMOTE_DLG_72,
	IDC_BTN_REMOTE_DLG_66, IDC_BTN_REMOTE_DLG_67, IDC_BTN_REMOTE_DLG_68,
};

CGrabPage::CGrabPage(CWnd* pParent /*=NULL*/)
	: CDialog(CGrabPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGrabPage)
	m_nAnalogSignalType = -1;
	m_szGrabStatus = _T("");
	m_bRelay1 = FALSE;
	m_bRelay2 = FALSE;
	m_bRelay3 = FALSE;
	m_bRelay4 = FALSE;
	//}}AFX_DATA_INIT

	m_hPageGrabThreadKillEvent = NULL;

	m_bThreadRunning    = FALSE;
	m_bStopLVDSThread	= FALSE;
	m_bStopAnalogThread	= FALSE;
//	m_bGrabSingleFrame  = FALSE;

	m_nPatternMode		= 0;	// 0 : offLine,		1 : online
	m_nGrabMode			= 1;	// 0 : Manual Grab,	1 : Auto Grab
	m_nGrabDevice		= 1;	// 0 : LVDS Grab,	1 : VideoOut Grab
	m_nAnalogSignalType = 0;	// 0 : NTSC			1 : PAL
	m_pSystemMessageDlg = NULL;
	//+change 090213(Modification No1)
//	m_hThread			= NULL;
//	m_pGrabImageThread_2in1	= NULL;
//	m_MWnd				= NULL;
	m_nAvSwitchCmd		= -1;


	//+add kwmoon 080710
//	m_nNewBitCount		= 0;
//	m_nNewLvdsWidth		= 0;
//	m_nNewLvdsHeight	= 0;
//	m_nNewLVDSMode		= 0;

	m_nCustomCode		= 0;
	m_nRmoteType		= 0;

}


void CGrabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGrabPage)
	DDX_Control(pDX, IDC_EDIT_TH_OUT, m_ThDataOut);
	DDX_Control(pDX, IDC_CHECK_SELECT_ALL, m_ctrlSelectAll);
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
	DDX_Control(pDX, IDC_BTN_GRAB, m_ctrlGrabBtn);
	DDX_Control(pDX, IDC_CHK_ONLINE, m_ctrlOnLineChk);
	DDX_Control(pDX, IDC_EDIT_MODEL, m_ctrlModelEdit);
	DDX_Control(pDX, IDC_EDIT_PATTERN, m_ctrlPatternEdit);
	DDX_Control(pDX, IDC_EDIT_REMOTE_CODE, m_ctrlRemoteCodeEdit);
	DDX_Control(pDX, IDC_CHK_MANUAL_GRAB, m_ctrlManualGrabChk);
	DDX_Control(pDX, IDC_CHK_AUTO_GRAB, m_ctrlAutoGrabChk);
	DDX_Radio(pDX, IDC_RADIO_NTSC, m_nAnalogSignalType);
	DDX_Text(pDX, IDC_LEDLABEL_GRAB, m_szGrabStatus);
	DDX_Check(pDX, IDC_CHECK_RELAY1, m_bRelay1);
	DDX_Check(pDX, IDC_CHECK_RELAY2, m_bRelay2);
	DDX_Check(pDX, IDC_CHECK_RELAY3, m_bRelay3);
	DDX_Check(pDX, IDC_CHECK_RELAY4, m_bRelay4);
	DDX_Control(pDX, IDC_EDIT_AVC_OUT, m_ctrlIrDataOut);
	//}}AFX_DATA_MAP
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
	ON_BN_CLICKED(IDC_CHK_EDID, OnChkEdid)
	ON_BN_CLICKED(IDC_CHK_HDCP, OnChkHdcp)
	ON_BN_CLICKED(IDC_CHK_ONLINE2, OnChkOnline2)
	ON_BN_CLICKED(IDC_BTN_SEND_MODEL2, OnBtnSendModel2)
	ON_BN_CLICKED(IDC_BTN_SEND_PATTERN2, OnBtnSendPattern2)
	ON_BN_CLICKED(IDC_CHK_DDCLINE, OnChkDdcline)
	ON_BN_CLICKED(IDC_RADIO_SECAM, OnRadioSecam)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL, OnCheckSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SET2, OnButtonSet2)
	ON_BN_CLICKED(IDC_BTN_AVC_BUF, OnBtnAvcBuf)
	ON_BN_CLICKED(IDC_BTN_AVC_IR, OnBtnAvcIr)
	ON_BN_CLICKED(IDC_BTN_TH, OnBtnTh)
	ON_BN_CLICKED(IDC_BTN_AVC_IR2, OnBtnAvcIr2)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_UPDATE_GRAB_STATUS, UpdateGrabStatus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrabPage message handlers

BOOL CGrabPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_hPageGrabThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);
	
	m_ctrlSaveAsBtn.EnableWindow(FALSE);


	//090811
	if(AnalogControl.m_bGrabberInit)
	{
		AnalogControl.SetVideoSourceType(CurrentSet->nAnalogType);
	}

	g_pView->SetGrabInfo(&m_Image1);
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
	
	TabItem.pszText = "PCI(USB) DIO Test";
	m_ctrlControlTab.InsertItem(1,&TabItem);

	ShowAvSwitchBoxCtrlGroup(TRUE);
	ShowUsbDioGroup(FALSE);

	// Lvds Format
/*	if((CurrentSet->nLvdsWidth == 852)		 && (CurrentSet->nLvdsHeight == 480))	m_nLvdsResolutionIndex = W_852_H_480;
	else if((CurrentSet->nLvdsWidth == 1024) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1024_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1280_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 1024))	m_nLvdsResolutionIndex = W_1280_H_1024;
	else if((CurrentSet->nLvdsWidth == 1366) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1366_H_768;
	else if((CurrentSet->nLvdsWidth == 1440) && (CurrentSet->nLvdsHeight == 900))	m_nLvdsResolutionIndex = W_1440_H_900;
	else if((CurrentSet->nLvdsWidth == 1680) && (CurrentSet->nLvdsHeight == 1050))	m_nLvdsResolutionIndex = W_1680_H_1050;
	//+add kwmoon 080922
	else if((CurrentSet->nLvdsWidth == 1364) && (CurrentSet->nLvdsHeight == 768))	m_nLvdsResolutionIndex = W_1364_H_768;
	else m_nLvdsResolutionIndex = -1;


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
	//add psh 090630
	m_bOddDE_Only = CurrentSet->bOddDE_Only;
	//add psh 091214
	m_bImage_Rotation = CurrentSet->nImageRotation;
*/

	BYTE nBuf;
	int nDIO_Out;
	if(gPciDioCtrl.m_bPCIDIO){
		GetDlgItem(IDC_CHECK_RELAY4)->EnableWindow(FALSE);
		nDIO_Out = gPciDioCtrl.DioOut_Read();
		if(g_nRunningProcessNo == 1){
			nBuf =  nDIO_Out & 0x01;
			if(nBuf == 0x01){
				m_bRelay1 = TRUE;
			}
			
			nBuf =  nDIO_Out & 0x02;
			if(nBuf == 0x02){
				m_bRelay2 = TRUE;
			}
			
			nBuf =  nDIO_Out & 0x04;
			if(nBuf == 0x04){
				m_bRelay3 = TRUE;
			}
		}
		else{
			nBuf =  nDIO_Out & 0x10;
			if(nBuf == 0x10){
				m_bRelay1 = TRUE;
			}
			
			nBuf =  nDIO_Out & 0x20;
			if(nBuf == 0x20){
				m_bRelay2 = TRUE;
			}
			
			nBuf =  nDIO_Out & 0x40;
			if(nBuf == 0x40){
				m_bRelay3 = TRUE;
			}
		}
	}
	else{
		GetDlgItem(IDC_CHECK_RELAY4)->EnableWindow(TRUE);

		nBuf =  gUsbDioCtrl.nDIO_Out & 0x01;
		if(nBuf == 0x01){
			m_bRelay1 = TRUE;
		}
		
		nBuf =  gUsbDioCtrl.nDIO_Out & 0x02;
		if(nBuf == 0x02){
			m_bRelay2 = TRUE;
		}
		
		nBuf =  gUsbDioCtrl.nDIO_Out & 0x04;
		if(nBuf == 0x04){
			m_bRelay3 = TRUE;
		}
		
		nBuf =  gUsbDioCtrl.nDIO_Out & 0x08;
		if(nBuf == 0x08){
			m_bRelay1 = TRUE;
		}
	}

	UpdateData(FALSE);

	CRect rect;
	CButton* pButton = NULL;

	for(int i=0; i<NO_USB_DIO_TAB_ITEM;i++)
	{
		pButton = (CButton*)GetDlgItem(aUsbDioItemNo[i]);
		pButton->GetWindowRect(&rect);
		// add & change psh 080701
        ScreenToClient(&rect);
		rect.top	 = rect.top	- 320;
		rect.bottom  = rect.bottom  - 320;
		//-

		pButton->MoveWindow(&rect);
	}

	//+change kwmoon 080628
//	m_szGrabStatus = "-READY-";

//	if(AnalogControl.m_bGrabberInit == FALSE)
//	{
//		InitGrabber(ANALOG_GRABBER);
//	}
	
	SetPatternGenGroup(CurrentSet->bUsePatternGen);
	SetHDMIGenGroup(CurrentSet->bUseHDMIGen);
	if((CurrentSet->bUseAVSwitchBox) && (AvSwitchBoxCtrl.m_bPortOpen == FALSE))
	{
		if(InitAvSwitchController() == FALSE) return FALSE;
	}

	if((CurrentSet->bUsePatternGen) && (PatternGeneratorCtrl.m_bPortOpen == FALSE))
	{
		if(!InitPatternGenerator()) return FALSE;
	}

	if((CurrentSet->bUseHDMIGen) && (HDMIGeneratorCtrl.m_bPortOpen == FALSE))
	{
		if(!InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate)) return FALSE;
		m_ctrlOnlineChk2.SetCheck(TRUE);

	}

	m_nCustomCode = CurrentSet->nRemoteCustomCode;
	m_nRmoteType  = CurrentSet->nRemoteType;

//	g_pView->SetGrabInfo(&m_Image1);

	return TRUE;  
}


/////////////////
// Init Grabber
/////////////////
BOOL CGrabPage::InitGrabber(int nGrabberType)
{
	CString szErrorMsg;

	if(nGrabberType == ANALOG_GRABBER) // ANALOG GRAB
	{
		if(AnalogControl.m_bGrabberInit == FALSE)
		{
			AnalogControl.OnPciCardOpen(); 

			if(AnalogControl.m_bGrabberInit == FALSE)
			{
				AfxMessageBox("[Analog Grabber]\nInitialization failed!"); 
				return FALSE;
			}
		}
	}

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

BOOL CGrabPage::GrabImage_2in1()
{
	BOOL bGrabbing = FALSE;

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

	m_bThreadRunning = TRUE;

	nWidth		= AnalogControl.m_nWidth;
	nHeight		= AnalogControl.m_nHeight;
	nBitCount	= AnalogControl.m_nBitCount;
	nImageSize  = AnalogControl.m_nImageSize;

//	g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);
	m_Image1.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

	nGrabResult = AnalogControl.OnGrabStart(GRAB_WITH_NO_RETRY, "", 0, FALSE);

	if(nGrabResult != GRAB_PASS)
	{
//		memset(g_GrabImage.m_pImageData,0xff,nImageSize);
//		m_Image1.m_pImageData = g_GrabImage.m_pImageData;
		memset(m_Image1.m_pImageData,0xff,nImageSize);
		m_Image1.m_bImageLoaded = TRUE;

		m_szGrabStatus = "GRAB-NG!";
		PostMessage(UM_UPDATE_GRAB_STATUS);
	}
	else
	{
		m_szGrabStatus = "GRAB-OK!";
		PostMessage(UM_UPDATE_GRAB_STATUS);

		nRowWidth = WIDTHBYTES(m_Image1.m_BmInfo->bmiHeader.biBitCount * m_Image1.m_BmInfo->bmiHeader.biWidth);

		m_Image1.m_bImageLoaded = FALSE;

		for(int i=0; i<nHeight; i++)
		{
			for(int j=0; j<nWidth; j++) 
			{   
//				g_GrabImage.m_pImageData[i*nRowWidth+3*j+2] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j+2]); // R
//				g_GrabImage.m_pImageData[i*nRowWidth+3*j+1] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j+1]); // G
//				g_GrabImage.m_pImageData[i*nRowWidth+3*j  ] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j  ]); // B
				m_Image1.m_pImageData[i*nRowWidth+3*j+2] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j+2]); // R
				m_Image1.m_pImageData[i*nRowWidth+3*j+1] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j+1]); // G
				m_Image1.m_pImageData[i*nRowWidth+3*j  ] = (BYTE) (AnalogControl.m_pImageData[(nHeight-i-1)*nRowWidth+3*j  ]); // B

			}
		}

	//	m_Image1.m_pImageData = g_GrabImage.m_pImageData;
		m_Image1.m_bImageLoaded = TRUE;
	}
	m_bThreadRunning = FALSE;

	return TRUE;
}

void CGrabPage::GrabStart()
{
	if(AnalogControl.m_bGrabberInit == FALSE) return;
	if(!m_bThreadRunning){
		MdigGrabContinuous(AnalogControl.m_MilDigitizer, AnalogControl.m_MilImage);
		m_bThreadRunning = TRUE;
	}
}

void CGrabPage::GrabStop()
{
	if(AnalogControl.m_bGrabberInit == FALSE) return;
	if(m_bThreadRunning){
		MdigHalt(AnalogControl.m_MilDigitizer);	
		m_bThreadRunning = FALSE;
	}


}

void CGrabPage::ShowSystemMsg() 
{
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

void CGrabPage::CloseSystemMsg() 
{
	if((m_pSystemMessageDlg != NULL)
	&& (m_pSystemMessageDlg->m_bShowDlg))
	{
		m_pSystemMessageDlg->m_bShowDlg = FALSE;
		m_pSystemMessageDlg->DestroyWindow();
		m_pSystemMessageDlg = NULL;
	}
}

void CGrabPage::OnChkAutoGrab() 
{
	
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
	
	m_ctrlSaveAsBtn.EnableWindow(FALSE);

//	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NTSC);
//	pButton->EnableWindow(FALSE);

//	pButton = (CButton*)GetDlgItem(IDC_RADIO_PAL);
//	pButton->EnableWindow(FALSE);

	//+add 090420(Modification No1)
//	pButton = (CButton*)GetDlgItem(IDC_RADIO_SECAM);
//	pButton->EnableWindow(FALSE);

	GrabStart();	
}

void CGrabPage::OnChkManualGrab() 
{
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
//	m_ctrlLvdsGrabChk.EnableWindow(TRUE);

	if(m_MWnd)
	{	
		//+change kwmoon 080628
		m_MWnd->SendMessage(UM_UPDATE_IMAGE_VIEW_DISPLAY);	
	}
}

void CGrabPage::OnBtnManualGrab() 
{
	DWORD dwThreadResult = 0;
	DWORD dwExitCode = 0;

	CButton* pButton = (CButton*)GetDlgItem(IDC_BTN_GRAB);
	pButton->EnableWindow(FALSE);
	
	//+add kwmoon 080419
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

	//	MdigGrab(AnalogControl.m_MilDigitizer, AnalogControl.m_MilImage);
		GrabImage_2in1();

		m_bThreadRunning = FALSE;

		m_ctrlSaveAsBtn.EnableWindow(TRUE);
	}
	else
	{
		AfxMessageBox("First,Select Manual Check Button");
	}
	pButton->EnableWindow(TRUE);
}
/*
void CGrabPage::OnChkLvdsGrab() 
{
	// TODO: Add your control notification handler code here
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NTSC);
	pButton->EnableWindow(FALSE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_PAL);
	pButton->EnableWindow(FALSE);

	//+add 090420(Modification No1)
	pButton = (CButton*)GetDlgItem(IDC_RADIO_SECAM);
	pButton->EnableWindow(FALSE);


	//+add kwmoon 080419
	if(m_bThreadRunning == TRUE)
	{
		if(m_nGrabMode != MANUAL_GRAB)
		{
			if(m_nGrabDevice == LVDS_GRABBER)
			{
				GrabStop(LVDS_GRABBER);
			}
			else
			{
				GrabStop(ANALOG_GRABBER);
			}
		}
	}
	
	m_nGrabDevice = LVDS_GRABBER;

	m_ctrlVideoOutGrabChk.SetCheck(FALSE);
	m_ctrlLvdsGrabChk.SetCheck(TRUE);

	//+add kwmoon 080419
	if(m_bThreadRunning != TRUE)
	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
	}
}	
*/
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
	m_nGrabDevice = ANALOG_GRABBER;

	m_ctrlVideoOutGrabChk.SetCheck(TRUE);
//	m_ctrlLvdsGrabChk.SetCheck(FALSE);

	//+add kwmoon 080419
//	if(m_bThreadRunning != TRUE)
//	{
		if(m_nGrabMode == AUTO_GRAB)
		{
			OnChkAutoGrab();
		}
//	}
}

void CGrabPage::OnRadioNtsc() 
{

	if(CurrentSet->bUseAVSwitchBox)
	{
		if(AvSwitchBoxCtrl.SetVideoOutType(V_SRC_NTSC))
		{
			GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
		}
		else 
		{
			GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
		}
	}
	if(CurrentSet->nAnalogType == V_SRC_NTSC) return;

	if(AnalogControl.m_bGrabberInit)
	{
		CurrentSet->nAnalogType   = V_SRC_NTSC;
	/*	g_pView->StopGrabThread();
		Sleep(500);	*/

		m_MWnd->AudioMeasureStop();

		
		GrabStop();
		Sleep(2000);

		m_MWnd->VideoDispDeselectWindow();	
		Sleep(500);


		AnalogControl.SetVideoSourceType(V_SRC_NTSC);
		
		m_MWnd->VideoDispSelectWindow();
		if(m_nGrabMode == AUTO_GRAB){
			GrabStart();
		}
		Sleep(500);
		//g_pView->StartGrabThread();
		m_MWnd->AudioMeasureStart();

		CurrentSet->nAnalogWidth  = AnalogControl.m_nWidth; 
		CurrentSet->nAnalogHeight = AnalogControl.m_nHeight;
		
		g_pView->SetGrabInfo(&m_Image1);

		UpdateData(TRUE);
	}
}

void CGrabPage::OnRadioPal() 
{
	
	if(CurrentSet->bUseAVSwitchBox)
	{
		if(AvSwitchBoxCtrl.SetVideoOutType(V_SRC_PAL))
		{
			GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
		}
		else 
		{
			GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
		}
	}

	if(CurrentSet->nAnalogType == V_SRC_PAL) return;

#if 1
	if (AnalogControl.m_bGrabberInit)
	{
		CurrentSet->nAnalogType = V_SRC_PAL;
		//g_pView->StopGrabThread();
		//Sleep(500);

		m_MWnd->AudioMeasureStop();
		GrabStop();
		Sleep(2000);

		m_MWnd->VideoDispDeselectWindow();
		Sleep(500);
		AnalogControl.SetVideoSourceType(V_SRC_PAL);
		Sleep(500);
		
	//	g_pView->StartGrabThread();


		

		m_MWnd->VideoDispSelectWindow();
		if (m_nGrabMode == AUTO_GRAB) {
			GrabStart();
		}
		m_MWnd->AudioMeasureStart();

		CurrentSet->nAnalogWidth = AnalogControl.m_nWidth;
		CurrentSet->nAnalogHeight = AnalogControl.m_nHeight;


		g_pView->SetGrabInfo(&m_Image1);

		UpdateData(TRUE);
	}
#else
	if(AnalogControl.m_bGrabberInit)
	{
		CurrentSet->nAnalogType   = V_SRC_PAL;

		m_MWnd->AudioMeasureStop();
		GrabStop();
		Sleep(2000);

		m_MWnd->VideoDispDeselectWindow();	
		Sleep(500);

		AnalogControl.SetVideoSourceType(V_SRC_PAL);

		m_MWnd->VideoDispSelectWindow();
		if(m_nGrabMode == AUTO_GRAB){
			GrabStart();
		}
		m_MWnd->AudioMeasureStart();

		CurrentSet->nAnalogWidth  = AnalogControl.m_nWidth; 
		CurrentSet->nAnalogHeight = AnalogControl.m_nHeight;
		

		g_pView->SetGrabInfo(&m_Image1);

		UpdateData(TRUE);
	}
#endif
}

void CGrabPage::OnBtnSaveBmp() 
{
	DWORD dwThreadResult = 0;

	if(m_bThreadRunning == TRUE)
	{
		GrabStop();		
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
	CString sTmp;
	CString sCustom = "LG";

	m_ctrlRemoteCodeEdit.GetWindowText(sTmp);
	m_ctrlSendRemoteRtnEdit.SetWindowText("");

	if(CurrentSet->bUseAVSwitchBox)
	{
		if(!InitAvSwitchController()) return;
		else
		{
			m_ctrlRemoteCodeEdit.GetWindowText(sTmp);
			
			switch(m_nCustomCode)
			{
				case REMOCON_HT:
				case REMOCON_MINI:
				case REMOCON_BD:
					sCustom = "LG";
					break;

				case REMOCON_PN:
					sCustom = "PN";
					break;

				case REMOCON_LO:
					sCustom = "LO";
					break;

				case REMOCON_NA:
				case REMOCON_CM:
				case REMOCON_GS:
				case REMOCON_TB:
				case REMOCON_SH:
				case REMOCON_SI:
					sCustom = "NA";
					break;

				case REMOCON_SO:
					sCustom = "SO";
					break;
			}
			sTmp.TrimLeft();
			sTmp.TrimRight();
			if(sTmp.GetLength() == 4){
				AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, m_nRmoteType, sCustom, sTmp, 1);
			}
			else if(sTmp.GetLength() == 8){
				AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, m_nRmoteType, sCustom, sTmp, 2);
			}
			else{
				m_ctrlSendRemoteRtnEdit.SetWindowText("ERROR");
			}
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
	dlg.m_nRmoteType = m_nRmoteType;

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
	CString sMsg;
/*	// TODO: Add your message handler code here and/or call default
	if(m_bThreadRunning == TRUE)
	{
		GrabStop(m_nGrabDevice);		
	}

	//+add 090131(수정항목1)
	DWORD dwExitCode = 0;
	if(m_pGrabImageThread_2in1)
	{
		TerminateThread(m_pGrabImageThread_2in1->m_hThread,dwExitCode);

		//+add 090224(Modification No2)
		m_pGrabImageThread_2in1 = NULL; 
	}
*/
	//+add kwmoon 071015 : [2in1]
	m_Image1.m_pImageData = NULL;

//	MdigHalt(AnalogControl.m_MilDigitizer1);
//	MdispDeselect(AnalogControl.m_MilDisplay1, AnalogControl.m_MilImage1);
//	MdispFree(AnalogControl.m_MilDisplay1);
//	AnalogControl.m_MilDisplay1 = M_NULL;
	if(CurrentSet->nRemoteCustomCode != m_nCustomCode)
	{
		if(FileExists(CurrentSet->sRemoconTitle))
		{
			OpenRemoteFile(CurrentSet->sRemoconTitle);
		//	AvSwitchBoxCtrl.SetAvSwitch(SET_CUSTOM_CODE, MAX_AVSWITCH_WAIT_DELAY,  nVal1, 0);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sRemoconTitle);
			AfxMessageBox(sMsg);
		}
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
//	m_nAvSwitchCmd = CHECK_STATUS;
	m_nAvSwitchCmd = SET_CUSTOM_CODE;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);
}

///////////////////////////////////////////////////////////////////////////////////////////////

BOOL CGrabPage::FindSourceKey(int nSource, CString& strName, CString& strCode, CString& strDesc, int& nByte)
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
			nByte = pRemote->nByte;
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


BOOL CGrabPage::FindButtonKey(CString sBtnName, CString& strName, CString& strCustomCode, CString& strCode, CString& strDesc, int& nByte)
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
			strCustomCode = pRemote->sCustomCode;
			strDesc = pRemote->sDesc;
			nByte = pRemote->nByte;
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
	CString sName, sCode, sCustom, sDesc;
	int nByte =1;
	
	if(!FindButtonKey(strName, sName, sCustom, sCode, sDesc, nByte)) return FALSE;
	else
	{
		//+ 2007.12.4 Mod BY USY
		if(CurrentSet->bUseAVSwitchBox)
		{
			if(!InitAvSwitchController()) return FALSE;
			else 
			{
				if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, m_nRmoteType, sCustom, sCode, nByte))
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
		/*	for(nTmp = 0; nTmp < MAX_AUDIO_CH; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
		*/
			m_ctrlAvSwitchVal1Cmb.AddString("1 : OPTICAL OUT");
			m_ctrlAvSwitchVal1Cmb.AddString("2 : COAX OUT");
			m_ctrlAvSwitchVal1Cmb.AddString("3 : SPK(F-L/R)");
			m_ctrlAvSwitchVal1Cmb.AddString("4 : SPK(R-L/R)");
			m_ctrlAvSwitchVal1Cmb.AddString("5 : SPK(C/W)");
			m_ctrlAvSwitchVal1Cmb.AddString("6 : SPK(S-L/R)");
			m_ctrlAvSwitchVal1Cmb.AddString("7 : AUX OUT");
			m_ctrlAvSwitchVal1Cmb.AddString("8 : SCART OUT");
			m_ctrlAvSwitchVal1Cmb.AddString("9 : HDMI OUT");

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
		/*	for(nTmp = 0; nTmp < MAX_VIDEO_CH; nTmp++)
			{
				sTmp.Format("%d", nTmp + 1);
				m_ctrlAvSwitchVal1Cmb.AddString(sTmp);
			}
		*/
			m_ctrlAvSwitchVal1Cmb.AddString("1 : HDMI");
			m_ctrlAvSwitchVal1Cmb.AddString("2 : RGB");
			m_ctrlAvSwitchVal1Cmb.AddString("3 : COMPONENT");
			m_ctrlAvSwitchVal1Cmb.AddString("4 : SCART VIDEO");
			m_ctrlAvSwitchVal1Cmb.AddString("5 : SCART S-VIDEO");
			m_ctrlAvSwitchVal1Cmb.AddString("6 : SCART RGB");
			m_ctrlAvSwitchVal1Cmb.AddString("7 : S-VIDEO");
			m_ctrlAvSwitchVal1Cmb.AddString("8 : CVBS1");
			m_ctrlAvSwitchVal1Cmb.AddString("9 : CVBS2");
			m_ctrlAvSwitchVal1Cmb.SetCurSel(0);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		case REMOTE_TYPE_SEL:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("Remote Type");

			m_ctrlAvSwitchVal1Cmb.AddString("0 : Code IR");
			m_ctrlAvSwitchVal1Cmb.AddString("1 : Codeless IR");
			m_ctrlAvSwitchVal1Cmb.SetCurSel(m_nRmoteType);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
		//-
/*		case CHECK_STATUS:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("");
			m_ctrlAvSwitchVal2Name.SetWindowText("");
			m_ctrlAvSwitchVal1Cmb.ShowWindow(FALSE);
			m_ctrlAvSwitchVal2Cmb.ShowWindow(FALSE);
		}
		break;
*/

		case SET_CUSTOM_CODE:
		{
			m_ctrlAvSwitchVal1Name.SetWindowText("Custom Code");

			m_ctrlAvSwitchVal1Cmb.AddString(" 1 : Home Theater");
			m_ctrlAvSwitchVal1Cmb.AddString(" 2 : MINI/MICRO");
			m_ctrlAvSwitchVal1Cmb.AddString(" 3 : BD Player");
			m_ctrlAvSwitchVal1Cmb.AddString(" 4 : PIONEER(HT)");
			m_ctrlAvSwitchVal1Cmb.AddString(" 5 : LOEWE(BD Player)");
			m_ctrlAvSwitchVal1Cmb.AddString(" 6 : NAD(BD Player)");
			m_ctrlAvSwitchVal1Cmb.AddString(" 7 : SONY(HT)");
			m_ctrlAvSwitchVal1Cmb.AddString(" 8 : LSC_CM(STB)");
			m_ctrlAvSwitchVal1Cmb.AddString(" 9 : LSC_GS(STB)");
			m_ctrlAvSwitchVal1Cmb.AddString("10 : LSC_TB(STB)");
			m_ctrlAvSwitchVal1Cmb.AddString("11 : LSH(STB)");
			m_ctrlAvSwitchVal1Cmb.AddString("12 : LSI(STB)");
			m_ctrlAvSwitchVal1Cmb.AddString("13 : JVC");
			m_ctrlAvSwitchVal1Cmb.SetCurSel(m_nCustomCode);

			m_ctrlAvSwitchVal2Name.SetWindowText("");
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
		break;
	
		//+add kwmoon 080929
/*		case SET_RGB_SOUND:
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
*/
	}
}

void CGrabPage::OnBtnSendAvswitchCmd() 
{
	UpdateData(TRUE);
	int nVal1, nVal2, nValue;
	nVal1 = nVal2  = nValue = 0;
	CString sRemoconTitle;
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
		case SET_VIDEO_CH: 
		case SET_COMP_CVBS_CH:  //+add PSh 100415
		{
			nVal1 = m_ctrlAvSwitchVal1Cmb.GetCurSel()+1;		
		}
		break;

		case REMOTE_TYPE_SEL:
		{
			m_nRmoteType  = m_ctrlAvSwitchVal1Cmb.GetCurSel();
			return;
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
		case SET_CUSTOM_CODE:
		{
			m_nCustomCode = m_ctrlAvSwitchVal1Cmb.GetCurSel();
			switch(m_nCustomCode)
			{
				case REMOCON_HT:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
					break;

				case REMOCON_MINI:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_MINI_REMOTE.rmt";
					break;

				case REMOCON_BD:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_BD_REMOTE.rmt";
					break;

				case REMOCON_PN:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_PN_REMOTE.rmt";
					break;

				case REMOCON_LO:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LO_REMOTE.rmt";
					break;

				case REMOCON_NA:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_NA_REMOTE.rmt";
				//	nVal1 = 0x10;
					break;

				case REMOCON_SO:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_SO_REMOTE.rmt";
				//	nVal1 = 0x2D;
					break;

				case REMOCON_CM:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSC_CM_REMOTE.rmt";
					break;

				case REMOCON_GS:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSC_GS_REMOTE.rmt";
					break;

				case REMOCON_TB:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSC_TB_REMOTE.rmt";
					break;

				case REMOCON_SH:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSH_REMOTE.rmt";
					break;

				case REMOCON_SI:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSI_REMOTE.rmt";
					break;

				default:
					sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
					break;
			}


			if(FileExists(sRemoconTitle))
			{
				OpenRemoteFile(sRemoconTitle);
				InitRemoteButtonGroup();
			}
			else
			{
				sTmp.Format("Failed to load file.\n[%s]",sRemoconTitle);
				AfxMessageBox(sTmp);
			}
			return;
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
LRESULT CGrabPage::UpdateGrabStatus(WPARAM wParam, LPARAM lParam)
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
/*void CGrabPage::OnRdoAvswitchCmd13() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_RGB_SOUND;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);	
}
*/
//+add kwmoon 080327
void CGrabPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

/*
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
*/
void CGrabPage::ChkVideoAutoGrab() 
{
	OnChkVideoOutGrab();
}

//+add kwmoon 080417
void CGrabPage::OnRemoteButtonClick(UINT nButtonID)
{
//	int nButton = nButtonID - IDC_BTN_REMOTE_DLG_1;
	CString sTmp = _T("");
	UINT nVal1 = 0;
	CString strName, sName, sCode, sCustomCode, sDesc;
	strName = sName = sCode = sCustomCode = sDesc = _T("");
	int nByte =1;

//	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
//	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));
//	GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(_T(""));


	GetDlgItem(nButtonID)->GetWindowText(strName);

	if(!FindButtonKey(strName, sName, sCustomCode, sCode, sDesc, nByte)){
		GetDlgItem(nButtonID)->EnableWindow(FALSE);
		return;
	}
	else
	{
		if(CurrentSet->bUseAVSwitchBox)
		{
			if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, m_nRmoteType, sCustomCode, sCode, nByte))
			{
				GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
			}
			else 
			{
				GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
			}
		}
	}
}

//+add kwmoon 080628
void CGrabPage::ShowAvSwitchBoxCtrlGroup(BOOL bShow)
{
	CButton* pButton = NULL;

	for(int i=0; i<NO_AVSWITCHBOX_TAB_ITEM;i++)
	{
	//	GetDlgItem(aAvSwitchBoxCtrlItemNo[i])->EnableWindow(CurrentSet->bUseAVSwitchBox);
		pButton = (CButton*)GetDlgItem(aAvSwitchBoxCtrlItemNo[i]);
		pButton->ShowWindow(bShow);
	}
}

//+add kwmoon 080628
void CGrabPage::ShowUsbDioGroup(BOOL bShow)
{
	CButton* pButton = NULL;

	for(int i=0; i<NO_USB_DIO_TAB_ITEM;i++)
	{
		pButton = (CButton*)GetDlgItem(aUsbDioItemNo[i]);
		pButton->ShowWindow(bShow);
	}
/*	if(bShow){
		if(gPciDioCtrl.m_bPCIDIO){
			GetDlgItem(IDC_CHECK_RELAY4)->ShowWindow(FALSE);
		}
		else{
			GetDlgItem(IDC_CHECK_RELAY4)->ShowWindow(TRUE);
		}
	} */
}

void CGrabPage::InitRemoteButtonGroup()
{
	CButton* pButton = NULL;

	for(int i=0; i<27;i++)
	{
		pButton = (CButton*)GetDlgItem(aRemoteItemNo[i]);
		pButton->EnableWindow(TRUE);
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
			ShowUsbDioGroup(FALSE);
		}
		break;

		case USB_DIO_TEST_TAB :
		{
			ShowAvSwitchBoxCtrlGroup(FALSE);
			ShowUsbDioGroup(TRUE);
		}
		break; 
	}	
	*pResult = 0;
}
/*
void CGrabPage::OnBtnGrabOptionApply() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString sMsg = _T("");

	int nTmp = 0;

	switch(m_nUranousMode) 
	{
		//0 : JEIDA_DUAL(0x81), 1 : JEIDA_SINGLE(0x01), 2 : NS_DUAL(0x80), 3 : NS_SINGLE(0x00)

		case 0 :  nTmp = 0x84; break; // JEIDA_DUAL
		case 1 :  nTmp = 0x04; break; // JEIDA_SINGLE
		case 2 :  nTmp = 0x80; break; // NS_DUAL
		case 3 :  nTmp = 0x00; break; // NS_SINGLE
	
		default : nTmp = 0x00; break; // NS_SINGLE
	}
	
	// add 091214
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
			CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
		case 5: 
			CurrentSet->nLvdsWidth = 1440; CurrentSet->nLvdsHeight = 900;		break;
		case 6: 
			CurrentSet->nLvdsWidth = 1680; CurrentSet->nLvdsHeight = 1050;		break;
	
		case 7: 
			CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;	
			// change psh 090630
			CurrentSet->bOddDE_Only = m_bOddDE_Only;
		break;

		case 8: 
			CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;	
			// change psh 090630
			CurrentSet->bOddDE_Only = m_bOddDE_Only;
		break;

		//+change kwmoon 080922
		case 9: 
			CurrentSet->nLvdsWidth = 1364; CurrentSet->nLvdsHeight = 768;
			// change psh 090630
//			if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 0))
//			{
				nTmp = nTmp | 0x10;
//			}
		break;

		default :
			CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
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

	//+del kwmoon 080910
	g_pView->SetGrabInfo(&m_Image1);

}

//+add kwmoon 080709
void CGrabPage::OnRdoLvdsResolution1(int nButtonID)
{
	switch(nButtonID) 
	{
		case IDC_RDO_LVDS_RESOLUTION1: 
			m_nLvdsResolutionIndex	= 0;		
			m_nNewLvdsWidth			= 852; 
			m_nNewLvdsHeight		= 480;   
		break;
		
		case IDC_RDO_LVDS_RESOLUTION2: 
			m_nLvdsResolutionIndex	 = 1;	
			m_nNewLvdsWidth			= 1024; 
			m_nNewLvdsHeight		= 768;  
		break;
		
		case IDC_RDO_LVDS_RESOLUTION3:
			m_nLvdsResolutionIndex	= 2;	
			m_nNewLvdsWidth			= 1280; 
			m_nNewLvdsHeight		= 768;  
			break;

		case IDC_RDO_LVDS_RESOLUTION4: 
			m_nLvdsResolutionIndex	= 3;	
			m_nNewLvdsWidth			= 1280; 
			m_nNewLvdsHeight		= 1024; 
			break;

		case IDC_RDO_LVDS_RESOLUTION5: 
			m_nLvdsResolutionIndex	= 4;	
			m_nNewLvdsWidth			= 1366; 
			m_nNewLvdsHeight		= 768;  
			break;

		case IDC_RDO_LVDS_RESOLUTION6: 
			m_nLvdsResolutionIndex	= 5;	
			m_nNewLvdsWidth			= 1440; 
			m_nNewLvdsHeight		= 900;  
			break;

		case IDC_RDO_LVDS_RESOLUTION7: 
			m_nLvdsResolutionIndex	= 6;	
			m_nNewLvdsWidth			= 1680; 
			m_nNewLvdsHeight		= 1050; 
			break;

		case IDC_RDO_LVDS_RESOLUTION8: 
			m_nLvdsResolutionIndex	= 7;
			m_nNewLvdsWidth			= 1920; 
			m_nNewLvdsHeight		= 1080; 
			break;

		case IDC_RDO_LVDS_RESOLUTION9: 
			m_nLvdsResolutionIndex	= 8;
			m_nNewLvdsWidth			= 1920; 
			m_nNewLvdsHeight		= 1080; 
			break;

		///+add kwmoon 080923
		case IDC_RDO_LVDS_RESOLUTION10: 
			m_nLvdsResolutionIndex	= 9;
			m_nNewLvdsWidth			= 1364; 
			m_nNewLvdsHeight		= 768; 
			break;

		default : 
			m_nLvdsResolutionIndex	= 4;
			m_nNewLvdsWidth			= 1366; 
			m_nNewLvdsHeight		= 768;	
			break;
	}
	
}

void CGrabPage::OnRdoColorDepth1(int nButtonID)
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

void CGrabPage::OnChkEdid() 
{
	BOOL bReturn;

	if(m_ctrlEdidChk.GetCheck()) 
	{
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;  
	}
	else
	{
		bReturn = HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;  
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
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE) ;  
	}
	else
	{
		bReturn = HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE) ;  
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

//+add 090420(Modification No1)
void CGrabPage::OnRadioSecam() 
{
	if(CurrentSet->nAnalogType == V_SRC_SECAM) return;

	if(AnalogControl.m_bGrabberInit)
	{
		GrabStop();

		m_MWnd->VideoDispDeselectWindow();	

		AnalogControl.SetVideoSourceType(V_SRC_SECAM);

		m_MWnd->VideoDispSelectWindow();
		if(m_nGrabMode == AUTO_GRAB){
			GrabStart();
		}

		CurrentSet->nAnalogWidth  = AnalogControl.m_nWidth; 
		CurrentSet->nAnalogHeight = AnalogControl.m_nHeight;
		
		CurrentSet->nAnalogType   = V_SRC_SECAM;

		g_pView->SetGrabInfo(&m_Image1);

		UpdateData(TRUE);
	}
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

/*
void CGrabPage::OnRdoAvswitchCmd14() 
{
	UpdateData(TRUE);
	m_nAvSwitchCmd = SET_COMP_CVBS_CH;
	SetAvSwitchValueCmb(m_nAvSwitchCmd);	
}
*/
void CGrabPage::SetPatternGenGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_GRAB_GROUP_4)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_MODEL_NUMBER)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_MODEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEND_MODEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_PATTERN_NUMBER)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_PATTERN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_SEND_PATTERN)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHK_ONLINE)->EnableWindow(bEnable);
}

void CGrabPage::OnCheckSelectAll() 
{
	if(m_ctrlSelectAll.GetCheck())
	{
		m_bRelay1 = TRUE;
		m_bRelay2 = TRUE;
		m_bRelay3 = TRUE;
		m_bRelay4 = TRUE;
	}
	else{
		m_bRelay1 = FALSE;
		m_bRelay2 = FALSE;
		m_bRelay3 = FALSE;
		m_bRelay4 = FALSE;
	}

	UpdateData(FALSE);
}

void CGrabPage::OnButtonSet2() 
{
	BYTE nBuf = 0;

	UpdateData();
	
	if(gPciDioCtrl.m_bPCIDIO){
		gPciDioCtrl.Set_Relay(1, m_bRelay1);
		Sleep(100);
		gPciDioCtrl.Set_Relay(2, m_bRelay2);
		Sleep(100);
		gPciDioCtrl.Set_Relay(3, m_bRelay3);
	}
	else{
		if(m_bRelay1) nBuf = nBuf + 0x01;
		if(m_bRelay2) nBuf = nBuf + 0x02;
		if(m_bRelay3) nBuf = nBuf + 0x04;
		if(m_bRelay4) nBuf = nBuf + 0x08;
		
		gUsbDioCtrl.WriteByte(nBuf);
	}

}

void CGrabPage::OnBtnAvcBuf() 
{
	m_ctrlIrDataOut.SetWindowText(_T(""));
	gIrCheckerCtrl.CheckBufferClear();
	m_ctrlIrDataOut.SetWindowText(gIrCheckerCtrl.m_strErrMsg);
}

void CGrabPage::OnBtnAvcIr() 
{
	CString sTemp;
	CString sValue;

	m_ctrlIrDataOut.SetWindowText(_T(""));
//	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b");

	if(gIrCheckerCtrl.CheckRemoteCmd(sValue,0)){
		sTemp.Format(_T("%s : %s"), gIrCheckerCtrl.m_strErrMsg, sValue);
		m_ctrlIrDataOut.SetWindowText(sTemp);
	}
	else{
		m_ctrlIrDataOut.SetWindowText(gIrCheckerCtrl.m_strErrMsg);
	}
	
	
}

void CGrabPage::OnBtnTh() 
{
	double dVal =0.0;
	CString sVal;

	dVal  = gThermometerCtrl.MeasureTh();
	
	sVal.Format("%.1f", dVal);
	m_ThDataOut.SetWindowText(sVal);

}

void CGrabPage::OnBtnAvcIr2() 
{
	CString sTemp;
	CString sValue;

	m_ctrlIrDataOut.SetWindowText(_T(""));
//	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b");

	if(gIrCheckerCtrl.CheckRemoteCmd(sValue,1)){
		sTemp.Format(_T("%s : %s"), gIrCheckerCtrl.m_strErrMsg, sValue);
		m_ctrlIrDataOut.SetWindowText(sTemp);
	}
	else{
		m_ctrlIrDataOut.SetWindowText(gIrCheckerCtrl.m_strErrMsg);
	}
	
}
