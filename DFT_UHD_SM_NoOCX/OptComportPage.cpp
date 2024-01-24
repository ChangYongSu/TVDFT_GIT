// OptComportPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptComportPage.h"
//////////////////////////////////////////////////////////////////////////
#include "Global.h"
#include "TVCommCtrl.h"
#include "AvSwitchBoxCtrl.h"
#include "PatternGeneratorCtl.h"
#include "ScannerCtrl.h"
#include "AvConvertorCtrl.h"

//+add kwmoon 080625
#include "I2cAdcCtrl.h"
//#include "Dmm.h"
#include "LnbDetectorCtl.h"
#include "JigCtl.h"
#include "VF1000.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CTVCommCtrl TVCommCtrl;
extern CAvSwitchBox AvSwitchBoxCtrl;
extern CPatternGenerator PatternGeneratorCtrl;
extern CScannerCtrl ScannerCtrl;

//+add kwmoon 080625
extern CI2cAdcCtrl I2cAdcCtrl;

extern CDATsysView*  g_pView;
//////////////////////////////////////////////////////////////////////////
// COptComportPage property page

IMPLEMENT_DYNCREATE(COptComportPage, CPropertyPage)

COptComportPage::COptComportPage() : CPropertyPage(COptComportPage::IDD)
, m_bScanNotUse(FALSE)
, m_nDP_TimeSel(FALSE)
{
	//{{AFX_DATA_INIT(COptComportPage)
	m_szAvSwitchBoxSettingValueTitle = _T("");
	m_ctlTVControlMode = -1;
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
	m_nHdmiType = -1;
	//}}AFX_DATA_INIT
}

COptComportPage::~COptComportPage()
{
}

void COptComportPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptComportPage)
	DDX_Control(pDX, IDC_CHK_USE_JIG, m_ctrlUseJigChk);
	DDX_Control(pDX, IDC_CHK_USE_COUNT_JIG, m_ctrlUseCountJigChk);
	DDX_Control(pDX, IDC_CHK_USE_DPMS, m_ctrlUseDPMSChk);
	DDX_Control(pDX, IDC_CHK_USE_LNB, m_ctrlUseLnbChk);
	DDX_Control(pDX, IDC_CHK_USE_AVC, m_ctrlUseAvcChk);
	DDX_Control(pDX, IDC_CMB_COMPORT_AVC, m_ctrlAvcComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_AVC, m_ctrlAvcBaudCmb);
	DDX_Control(pDX, IDC_CMB_COMPORT_HDMI, m_ctrlMdmiComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_HDMI, m_ctrlHdmiBaudCmb);
	DDX_Control(pDX, IDC_CHK_USE_HDMIGEN, m_ctrlUseHdmiGenChk);
	DDX_Control(pDX, IDC_CMB_COMPORT_I2C_ADC, m_ctrlI2cAdcComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_I2C_ADC, m_ctrlI2cAdcBaudCmb);
	DDX_Control(pDX, IDC_CHK_USE_I2C_ADC, m_ctrlUseI2cAdcChk);
	DDX_Control(pDX, IDC_CMB_COMPORT_SCANNER, m_ctrlScannerComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_SCANNER, m_ctrlScannerBaudCmb);
	DDX_Control(pDX, IDC_CHK_USE_SCANNER, m_ctrlUseScannerChk);
	DDX_Control(pDX, IDC_CHK_USE_TVCOMMPORT, m_ctrlUseTVCommPortChk);
	DDX_Control(pDX, IDC_CHK_USE_PATTERNGEN, m_ctrlUsePatternGenChk);
	DDX_Control(pDX, IDC_CHK_USE_AVSWITCHBOX, m_ctrlUseAVSwitchBoxChk);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_AV_SWITCHBOX, m_ctrlSwitchBoxBaudCmb);
	DDX_Control(pDX, IDC_CMB_COMPORT_AV_SWITCHBOX, m_ctrlAvSwitchBoxComportCmb);
	DDX_Control(pDX, IDC_CMB_COMPORT_TV, m_ctrlTVComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_TV, m_ctrlTVBaudCmb);
	DDX_Control(pDX, IDC_CMB_COMPORT_PATTERN, m_ctrlPatternComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_PATTERN, m_ctrlPatternBaudCmb);
	DDX_Radio(pDX, IDC_RADIO_TV_CONTROL_RS232C, m_ctlTVControlMode);
	DDX_Check(pDX, IDC_CHK_AUTO_SOURCE_CONTROL, m_bAutoSourceControl);
	DDX_Check(pDX, IDC_CHK_AUTO_PATTERN_CONTROL, m_bAutoPatternControl);
	DDX_Check(pDX, IDC_CHK_AUTO_AVSWBOX_CONTROL, m_bAutoAVSwitchBoxControl);
	DDX_Radio(pDX, IDC_RADIO_HDMI_TYPE1, m_nHdmiType);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHK_USE_VFM, m_ctrlUseVfmChk);
	DDX_Check(pDX, IDC_CHK_SCAN_NO_USE, m_bScanNotUse);
	DDX_Control(pDX, IDC_CHK_USE_START_BOX, m_ctrlUseStartBoxChk);
	DDX_Radio(pDX, IDC_RADIO_DP_TIME60, m_nDP_TimeSel);
	DDX_Control(pDX, IDC_CHK_USE_DPGEN, m_cCheckDP_PortEnable);
}

BEGIN_MESSAGE_MAP(COptComportPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptComportPage)
	ON_BN_CLICKED(IDC_BTN_COMPORT_OPT_APPLY, OnBtnComportOptApply)
	ON_BN_CLICKED(IDC_CHK_USE_AVSWITCHBOX, OnChkUseAvswitchbox)
	ON_BN_CLICKED(IDC_CHK_USE_PATTERNGEN, OnChkUsePatterngen)
	ON_BN_CLICKED(IDC_CHK_USE_TVCOMMPORT, OnChkUseTvcommport)
	ON_BN_CLICKED(IDC_CHK_USE_SCANNER, OnChkUseScanner)
	ON_BN_CLICKED(IDC_BTN_INIT_JOYSTICK, OnBtnInitJoystick)
	ON_BN_CLICKED(IDC_CHK_USE_I2C_ADC, OnChkUseI2cAdc)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_CHK_USE_HDMIGEN, OnChkUseHdmigen)
	ON_BN_CLICKED(IDC_CHK_USE_AVC, OnChkUseAvc)
	ON_BN_CLICKED(IDC_CHK_USE_LNB, OnChkUseLnb)
	ON_BN_CLICKED(IDC_CHK_USE_JIG, OnChkUseJig)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_USE_VFM, &COptComportPage::OnBnClickedChkUseVfm)
	ON_BN_CLICKED(IDC_CHK_USE_START_BOX, &COptComportPage::OnBnClickedChkUseStartBox)
	ON_BN_CLICKED(IDC_CHK_USE_DPGEN, &COptComportPage::OnBnClickedChkUseDpgen)
	ON_BN_CLICKED(IDC_RADIO_DP_TIME60, &COptComportPage::OnBnClickedRadioDpTime60)
	ON_BN_CLICKED(IDC_RADIO_DP_TIME30, &COptComportPage::OnBnClickedRadioDpTime30)
	ON_BN_CLICKED(IDC_CHK_USE_COUNT_JIG, &COptComportPage::OnBnClickedChkUseCountJig)
	ON_BN_CLICKED(IDC_CHK_USE_DPMS, &COptComportPage::OnBnClickedChkUseDpms)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptComportPage message handlers
//////////////////////////////////////////////////////////////////////////
//+ 2007.5.2 Add BY USY
void COptComportPage::InitComPort(CString sComPort, DWORD wBaudRate, int nComportCmbID, int nBaudRateCmbID)
{
	CComboBox* pComport = (CComboBox*)GetDlgItem(nComportCmbID);
	CComboBox* pBaudRate = (CComboBox*)GetDlgItem(nBaudRateCmbID);

	if(sComPort == "COM1") pComport->SetCurSel(0);
	else if(sComPort == "COM2") pComport->SetCurSel(1);
	else if(sComPort == "COM3") pComport->SetCurSel(2);
	else if(sComPort == "COM4") pComport->SetCurSel(3);
	else if(sComPort == "COM5") pComport->SetCurSel(4);
	else if(sComPort == "COM6") pComport->SetCurSel(5);
	else if(sComPort == "COM7") pComport->SetCurSel(6);
	else if(sComPort == "COM8") pComport->SetCurSel(7);
	else if(sComPort == "COM9") pComport->SetCurSel(8);
	else if(sComPort == "\\\\.\\COM10") pComport->SetCurSel(9);
	else if(sComPort == "\\\\.\\COM11") pComport->SetCurSel(10);
	else if(sComPort == "\\\\.\\COM12") pComport->SetCurSel(11);
	else if(sComPort == "\\\\.\\COM13") pComport->SetCurSel(12);
	else if(sComPort == "\\\\.\\COM14") pComport->SetCurSel(13);
	else if(sComPort == "\\\\.\\COM15") pComport->SetCurSel(14);
	else if(sComPort == "\\\\.\\COM16") pComport->SetCurSel(15);
	else if(sComPort == "\\\\.\\COM17") pComport->SetCurSel(16);
	else if(sComPort == "\\\\.\\COM18") pComport->SetCurSel(17);
	else if(sComPort == "\\\\.\\COM19") pComport->SetCurSel(18);
	else if(sComPort == "\\\\.\\COM20") pComport->SetCurSel(19);
	else if(sComPort == "\\\\.\\COM21") pComport->SetCurSel(20);
	else if(sComPort == "\\\\.\\COM22") pComport->SetCurSel(21);
	else if(sComPort == "\\\\.\\COM23") pComport->SetCurSel(22);
	else if(sComPort == "\\\\.\\COM24") pComport->SetCurSel(23);
	else pComport->SetCurSel(-1);

	if(wBaudRate == CBR_9600) pBaudRate->SetCurSel(0);
	else if(wBaudRate == CBR_19200) pBaudRate->SetCurSel(1);
	else if(wBaudRate == CBR_38400) pBaudRate->SetCurSel(2);
	else if(wBaudRate == CBR_57600) pBaudRate->SetCurSel(3);
	else if(wBaudRate == CBR_115200) pBaudRate->SetCurSel(4);
	else pBaudRate->SetCurSel(-1);


}


CString COptComportPage::GetComPortVal(int nComportCmbID)
{
	CString sComPort;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nComportCmbID);
	int nCurSel = pCombo->GetCurSel();
	
	switch(nCurSel) 
	{
		case 0 : sComPort.Format("COM1"); break;
		case 1 : sComPort.Format("COM2"); break;
		case 2 : sComPort.Format("COM3"); break;
		case 3 : sComPort.Format("COM4"); break;
		case 4 : sComPort.Format("COM5"); break;
		case 5 : sComPort.Format("COM6"); break;
		case 6 : sComPort.Format("COM7"); break;
		case 7 : sComPort.Format("COM8"); break;
		case 8 : sComPort.Format("COM9"); break;
		case 9 : sComPort.Format("\\\\.\\COM10"); break;
		case 10 : sComPort.Format("\\\\.\\COM11"); break;
		case 11 : sComPort.Format("\\\\.\\COM12"); break;
		case 12 : sComPort.Format("\\\\.\\COM13"); break;
		case 13 : sComPort.Format("\\\\.\\COM14"); break;
		case 14 : sComPort.Format("\\\\.\\COM15"); break;
		case 15 : sComPort.Format("\\\\.\\COM16"); break;
		case 16 : sComPort.Format("\\\\.\\COM17"); break;
		case 17 : sComPort.Format("\\\\.\\COM18"); break;
		case 18 : sComPort.Format("\\\\.\\COM19"); break;
		case 19 : sComPort.Format("\\\\.\\COM20"); break;
		case 20 : sComPort.Format("\\\\.\\COM21"); break;
		case 21 : sComPort.Format("\\\\.\\COM22"); break;
		case 22 : sComPort.Format("\\\\.\\COM23"); break;
		case 23 : sComPort.Format("\\\\.\\COM24"); break;
		default : sComPort.Format("COM1"); break;
	}
	return sComPort;
}
DWORD COptComportPage::GetBaudRateVal(int nBaudRateCmbID)
{
	DWORD wBaudRate;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(nBaudRateCmbID);
	int nCurSel = pCombo->GetCurSel();
	switch(nCurSel) 
	{
		case 0: wBaudRate = CBR_9600;	break;
		case 1: wBaudRate = CBR_19200;	break;
		case 2: wBaudRate = CBR_38400;	break;
		case 3: wBaudRate = CBR_57600;	break;
		case 4: wBaudRate = CBR_115200;	break;
		default: wBaudRate = CBR_9600; break;
	}
	return wBaudRate;
}

void COptComportPage::SetPatternGenGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_PATTERNGEN_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_PATTERN)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_PATTERNGEN_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_PATTERN)->EnableWindow(bEnable);
}

void COptComportPage::SetTVCommPortGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_TVCOMMPORT_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_TV)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TVCOMMPORT_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_TV)->EnableWindow(bEnable);
}

void COptComportPage::SetAVSwitchBoxGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_AVSWITCHBOX_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_AV_SWITCHBOX)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_AVSWITCHBOX_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_AV_SWITCHBOX)->EnableWindow(bEnable);
}

//+ 2007.11.30 Add BY USY
void COptComportPage::SetScannerGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_SCANNER_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_SCANNER)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SCANNER_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_SCANNER)->EnableWindow(bEnable);
}
void COptComportPage::SetStartBoxGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_START_BOX_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_START_BOX)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_START_BOX_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_START_BOX)->EnableWindow(bEnable);
}

void COptComportPage::SetI2cAdcGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_I2C_ADC_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_I2C_ADC)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_I2C_ADC_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_I2C_ADC)->EnableWindow(bEnable);
	
}
void COptComportPage::SetHDMIGenGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_HDMIGEN_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_HDMI)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_HDMIGEN_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_HDMI)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_HDMI_TYPE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_HDMI_TYPE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_HDMI_TYPE3)->EnableWindow(bEnable);
}
void COptComportPage::SetLnbGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_LNB_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_LNB)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_LNB_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_LNB)->EnableWindow(bEnable);
}

// add 20100622
void COptComportPage::SetAvcGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_AVC_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_AVC)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_AVC_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_AVC)->EnableWindow(bEnable);
}

void COptComportPage::SetJigGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_JIG_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_JIG)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_JIG_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_JIG)->EnableWindow(bEnable);	
}

void COptComportPage::SetCountJigGroup(BOOL bEnable)
{	
	GetDlgItem(IDC_STATIC_COUNT_JIG_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_COUNT_JIG)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_COUNT_JIG_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_COUNT_JIG)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_COUNT_JIG)->EnableWindow(bEnable);
}

void COptComportPage::SetDPMSGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_DPMS_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_DPMS)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_DPMS_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_DPMS)->EnableWindow(bEnable);
}


void COptComportPage::SetVfmGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_VFM_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_VFM)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_VFM_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_VFM)->EnableWindow(bEnable);
}

void COptComportPage::SetDPGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_DP_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_DPGEN)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_DP_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_DPGEN)->EnableWindow(bEnable); 
	GetDlgItem(IDC_STATIC_DP_TIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_DP_TIME60)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_DP_TIME30)->EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////

BOOL COptComportPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

		UpdateData();

 	//=======================
	// Comport No & BaudRate
 	//=======================
	InitComPort(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate,IDC_CMB_COMPORT_PATTERN, IDC_CMB_BAUDRATE_PATTERN );
	InitComPort(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate,IDC_CMB_COMPORT_TV, IDC_CMB_BAUDRATE_TV );
	InitComPort(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate,IDC_CMB_COMPORT_AV_SWITCHBOX, IDC_CMB_BAUDRATE_AV_SWITCHBOX );
	InitComPort(CurrentSet->sI2cAdcComPort, CurrentSet->wI2cAdcBaudRate,IDC_CMB_COMPORT_I2C_ADC, IDC_CMB_BAUDRATE_I2C_ADC );
	InitComPort(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate,IDC_CMB_COMPORT_HDMI, IDC_CMB_BAUDRATE_HDMI );
	InitComPort(CurrentSet->sLNBComPort, CurrentSet->wLNBBaudRate,IDC_CMB_COMPORT_LNB, IDC_CMB_BAUDRATE_LNB );
//	InitComPort(CurrentSet->sMHLComPort, CurrentSet->wMHLBaudRate,IDC_CMB_COMPORT_MHL, IDC_CMB_BAUDRATE_MHL );

	m_ctrlUsePatternGenChk.SetCheck(CurrentSet->bUsePatternGen);
	SetPatternGenGroup(CurrentSet->bUsePatternGen);

	m_ctrlUseTVCommPortChk.SetCheck(CurrentSet->bUseTVCommPort);
	SetTVCommPortGroup(CurrentSet->bUseTVCommPort);
		
	m_ctrlUseAVSwitchBoxChk.SetCheck(CurrentSet->bUseAVSwitchBox);
	SetAVSwitchBoxGroup(CurrentSet->bUseAVSwitchBox);
	
	m_ctrlUseI2cAdcChk.SetCheck(CurrentSet->bUseI2cAdc);
	SetI2cAdcGroup(CurrentSet->bUseI2cAdc);

	m_ctrlUseHdmiGenChk.SetCheck(CurrentSet->bUseHDMIGen);
	m_nHdmiType = CurrentSet->nHDMIGenType;// m_nDP_TimeSel
	SetHDMIGenGroup(CurrentSet->bUseHDMIGen);

	m_ctrlUseLnbChk.SetCheck(CurrentSet->bUseLNB);
	SetLnbGroup(CurrentSet->bUseLNB);

	InitComPort(CurrentSet->sAvcComPort, CurrentSet->wAvcBaudRate,IDC_CMB_COMPORT_AVC, IDC_CMB_BAUDRATE_AVC );
	m_ctrlUseAvcChk.SetCheck(CurrentSet->bUseAVC);
	SetAvcGroup(CurrentSet->bUseAVC);


//-

	InitComPort(CurrentSet->sJigComPort, CurrentSet->wJigBaudRate, IDC_CMB_COMPORT_JIG, IDC_CMB_BAUDRATE_JIG);
	SetJigGroup(CurrentSet->bUseJig);
	m_ctrlUseJigChk.SetCheck(CurrentSet->bUseJig);

	InitComPort(CurrentSet->sCountJigComPort, CurrentSet->wCountJigBaudRate,IDC_CMB_COMPORT_COUNT_JIG, IDC_CMB_BAUDRATE_COUNT_JIG); 
	SetCountJigGroup(CurrentSet->bUseCountIDJig);
	m_ctrlUseCountJigChk.SetCheck(CurrentSet->bUseCountIDJig);
	CString sMsg;
	sMsg.Format("%d", CurrentSet->wJig_ID_Count_MAX);
	SetDlgItemText(IDC_EDIT_COUNT_JIG, sMsg);
	

	InitComPort(CurrentSet->sDPMSComPort, CurrentSet->wDPMSBaudRate, IDC_CMB_COMPORT_DPMS, IDC_CMB_BAUDRATE_DPMS);
	SetDPMSGroup(CurrentSet->bUseDPMS);
	m_ctrlUseDPMSChk.SetCheck(CurrentSet->bUseDPMS);

	//if (gJigCtrl.m_bID_Ctrl_PortOpen)
		//	gJigCtrl.ID_Ctrl_PortClose();
		//CurrentSet->bUseCountIDJig = m_ctrlUseCountJigChk.GetCheck();
		//CurrentSet->wCountJigBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_COUNT_JIG);
		//CurrentSet->sCountJigComPort.Format(GetComPortVal(IDC_CMB_COMPORT_COUNT_JIG));

		//if (CurrentSet->bUseCountIDJig)
		//{

		//	if (!gJigCtrl.Create(CurrentSet->sCountJigComPort, CurrentSet->wCountJigBaudRate)) {
		//		sMsg.Format("Failed to open COM port (%s)", CurrentSet->sCountJigComPort);
		//		AfxMessageBox(sMsg);
		//	}
		//}

		//if (gJigCtrl.m_bDPMS_PortOpen)
		//	gJigCtrl.DPMS_PortClose();
		//CurrentSet->bUseDPMS = m_ctrlUseDPMSChk.GetCheck();
		//CurrentSet->wDPMSBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_DPMS);
		//CurrentSet->sDPMSComPort.Format(GetComPortVal(IDC_CMB_COMPORT_DPMS));

		//if (CurrentSet->bUseDPMS)
		//{

		//	if (!gJigCtrl.Create(CurrentSet->sDPMSComPort, CurrentSet->wDPMSBaudRate)) {
		//		sMsg.Format("Failed to open COM port (%s)", CurrentSet->sDPMSComPort);
		//		AfxMessageBox(sMsg);
		//	}
		//}




		InitComPort(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate, IDC_CMB_COMPORT_SCANNER, IDC_CMB_BAUDRATE_SCANNER);
		//SetScannerGroup(FALSE);
		GetDlgItem(IDC_CHK_USE_SCANNER)->EnableWindow(FALSE);

		InitComPort(CurrentSet->sStartBoxComPort, CurrentSet->wStartBoxBaudRate, IDC_CMB_COMPORT_START_BOX, IDC_CMB_BAUDRATE_START_BOX);
		//SetStartBoxGroup(FALSE);
		GetDlgItem(IDC_CHK_USE_START_BOX)->EnableWindow(FALSE);
		if (CurrentSet->bUsePLCRobot == TRUE)
		{
			SetScannerGroup(TRUE);
			SetStartBoxGroup(FALSE);

		}
		else
		{
			m_ctrlUseScannerChk.SetCheck(CurrentSet->bUseScanner);
			if (CurrentSet->bUseScanner == FALSE)
			{
				CurrentSet->bUseStartBox = TRUE;
			}
			else
			{
				CurrentSet->bUseStartBox = FALSE;
			}
			m_ctrlUseStartBoxChk.SetCheck(CurrentSet->bUseStartBox);
		}

//	else {
//		InitComPort(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate, IDC_CMB_COMPORT_SCANNER, IDC_CMB_BAUDRATE_SCANNER);
//		InitComPort(CurrentSet->sStartBoxComPort, CurrentSet->wStartBoxBaudRate, IDC_CMB_COMPORT_START_BOX, IDC_CMB_BAUDRATE_START_BOX);
//
////		GetDlgItem(IDC_CHK_USE_JIG)->EnableWindow(FALSE);
////		SetJigGroup(FALSE);
//		InitComPort(CurrentSet->sJigComPort, CurrentSet->wJigBaudRate, IDC_CMB_COMPORT_JIG, IDC_CMB_BAUDRATE_JIG);
//		SetJigGroup(CurrentSet->bUseJig);
//		m_ctrlUseJigChk.SetCheck(CurrentSet->bUseJig);
//
//
//		m_ctrlUseScannerChk.SetCheck(CurrentSet->bUseScanner);
//		SetScannerGroup(CurrentSet->bUseScanner);
//		if (CurrentSet->bUseScanner == FALSE)
//		{
//			CurrentSet->bUseStartBox = TRUE;
//		}
//		else
//		{
//			CurrentSet->bUseStartBox = FALSE;
//		}
//		m_ctrlUseStartBoxChk.SetCheck(CurrentSet->bUseStartBox);
//		SetStartBoxGroup(CurrentSet->bUseStartBox);
//	}

	InitComPort(CurrentSet->sVfmComPort, CurrentSet->wVfmBaudRate,IDC_CMB_COMPORT_VFM, IDC_CMB_BAUDRATE_VFM );
	m_ctrlUseVfmChk.SetCheck(CurrentSet->bUseVfm);
	SetVfmGroup(CurrentSet->bUseVfm);

	InitComPort(CurrentSet->sDpgComPort, CurrentSet->wDpgBaudRate, IDC_CMB_COMPORT_DPGEN, IDC_CMB_BAUDRATE_DPGEN);
	m_cCheckDP_PortEnable.SetCheck(CurrentSet->bUseDpg);
	m_nDP_TimeSel = CurrentSet->nDP_TimeSel;// 
	if (m_nDP_TimeSel != 1)
		m_nDP_TimeSel = 0;
	SetDPGroup(CurrentSet->bUseDpg);


	m_ctlTVControlMode		 = CurrentSet->nTVControlType;
	m_bAutoSourceControl	 = CurrentSet->bAutoSourceControl;
	m_bAutoPatternControl	 = CurrentSet->bAutoPatternControl;
	m_bScanNotUse	 = CurrentSet->bScanNotUse;

	if(CurrentSet->nAutoAVSwitchBoxControl == 1){
		m_bAutoAVSwitchBoxControl = TRUE;
	}
	else{
		m_bAutoAVSwitchBoxControl = FALSE;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//======================
// Save Comport Setting 
//======================
void COptComportPage::OnBtnComportOptApply() 
{
	UpdateData(TRUE);

	CString sMsg = _T("");

	CEnvironmentData OldSet;

	OldSet.sPatternComPort	= CurrentSet->sPatternComPort	;
	OldSet.wPatternBaudRate	= CurrentSet->wPatternBaudRate	;
	OldSet.bUsePatternGen	= CurrentSet->bUsePatternGen	;

	CurrentSet->sPatternComPort.Format(GetComPortVal(IDC_CMB_COMPORT_PATTERN));
	CurrentSet->wPatternBaudRate		= GetBaudRateVal(IDC_CMB_BAUDRATE_PATTERN);
	CurrentSet->bUsePatternGen			= m_ctrlUsePatternGenChk.GetCheck();

	
	//========================
	// Init Pattern Generator
	//========================
	if (CurrentSet->bUsePatternGen)
	{
		if ((CurrentSet->bUsePatternGen != OldSet.bUsePatternGen)
			|| (CurrentSet->sPatternComPort != OldSet.sPatternComPort)
			|| (CurrentSet->wPatternBaudRate != OldSet.wPatternBaudRate))
		{
		
			if (PatternGeneratorCtrl.m_bPortOpen)	PatternGeneratorCtrl.PortClose();
			InitPatternGen(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}
	else
	{
		if (PatternGeneratorCtrl.m_bPortOpen) PatternGeneratorCtrl.PortClose();
	}


	//====================
	// Init TV Tuner Port
	//====================
	OldSet.sTVComPort		= CurrentSet->sTVComPort;
	OldSet.wTVBaudRate		= CurrentSet->wTVBaudRate;
	OldSet.bUseTVCommPort	= CurrentSet->bUseTVCommPort;
	//if(TVCommCtrl.m_bPortOpen)				TVCommCtrl.PortClose();
	CurrentSet->sTVComPort.Format(GetComPortVal(IDC_CMB_COMPORT_TV));	
	CurrentSet->wTVBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_TV);
	CurrentSet->bUseTVCommPort = m_ctrlUseTVCommPortChk.GetCheck();

	if (CurrentSet->bUseTVCommPort)
	{
		if ((CurrentSet->bUseTVCommPort != OldSet.bUseTVCommPort)
			|| (CurrentSet->sTVComPort != OldSet.sTVComPort)
			|| (CurrentSet->wTVBaudRate != OldSet.wTVBaudRate))
		{
			if (TVCommCtrl.m_bPortOpen) TVCommCtrl.PortClose();
			InitTVCommPort(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
		}
	}
	else
	{
		if (TVCommCtrl.m_bPortOpen) TVCommCtrl.PortClose();
	}


	//=======================
	// Init AvSwitchBox Port
	//=======================
	OldSet.sAvSwitchBoxComPort	= CurrentSet->sAvSwitchBoxComPort;
	OldSet.wAvSwitchBoxBaudRate = CurrentSet->wAvSwitchBoxBaudRate;
	OldSet.bUseAVSwitchBox = CurrentSet->bUseAVSwitchBox;

	CurrentSet->sAvSwitchBoxComPort.Format(GetComPortVal(IDC_CMB_COMPORT_AV_SWITCHBOX));
	CurrentSet->wAvSwitchBoxBaudRate	= GetBaudRateVal(IDC_CMB_BAUDRATE_AV_SWITCHBOX);
	CurrentSet->bUseAVSwitchBox			= m_ctrlUseAVSwitchBoxChk.GetCheck();


	if (CurrentSet->bUseAVSwitchBox)
	{
		if ((CurrentSet->bUseAVSwitchBox != OldSet.bUseAVSwitchBox)
			|| (CurrentSet->sAvSwitchBoxComPort != OldSet.sAvSwitchBoxComPort)
			|| (CurrentSet->wAvSwitchBoxBaudRate != OldSet.wAvSwitchBoxBaudRate))
		{
			if (AvSwitchBoxCtrl.m_bPortOpen)			AvSwitchBoxCtrl.PortClose();
			InitAvSwitchBox(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate);
		}
	}
	else
	{
		if (AvSwitchBoxCtrl.m_bPortOpen) AvSwitchBoxCtrl.PortClose();
	}
	//

//===================
// Init Scanner Port
//===================

	OldSet.sScannerComPort = CurrentSet->sScannerComPort;
	OldSet.wScannerBaudRate = CurrentSet->wScannerBaudRate;
	OldSet.bUseScanner = CurrentSet->bUseScanner;

	OldSet.sStartBoxComPort		= CurrentSet->sStartBoxComPort;
	OldSet.wStartBoxBaudRate = CurrentSet->wStartBoxBaudRate;
	OldSet.bUseStartBox = CurrentSet->bUseStartBox;


	//if(ScannerCtrl.m_bPortOpen)				ScannerCtrl.PortClose();
	CurrentSet->sScannerComPort.Format(GetComPortVal(IDC_CMB_COMPORT_SCANNER));
	CurrentSet->wScannerBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_SCANNER);
	CurrentSet->bUseScanner		= m_ctrlUseScannerChk.GetCheck();

	CurrentSet->sStartBoxComPort.Format(GetComPortVal(IDC_CMB_COMPORT_START_BOX));
	CurrentSet->wStartBoxBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_START_BOX);
	CurrentSet->bUseStartBox      = m_ctrlUseStartBoxChk.GetCheck();
	if (g_nSysType != AUTO_SYS)
	{
		if (CurrentSet->bUseScanner)
		{
			if ((CurrentSet->bUseScanner != OldSet.bUseScanner)
				|| (CurrentSet->sScannerComPort != OldSet.sScannerComPort)
				|| (CurrentSet->wScannerBaudRate != OldSet.wScannerBaudRate))
			{
				g_pView->InitScanner(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate);
			}
		}
		else
		{
			//if(ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();
			CurrentSet->bUseStartBox = 1;
			if ((CurrentSet->bUseStartBox != OldSet.bUseStartBox)
				|| (CurrentSet->sStartBoxComPort != OldSet.sStartBoxComPort)
				|| (CurrentSet->wStartBoxBaudRate != OldSet.wStartBoxBaudRate))
			{
				g_pView->InitScanner(CurrentSet->sStartBoxComPort, CurrentSet->wStartBoxBaudRate);
			}


			////===================
			//// Init StartBox Port
			////===================
			//if(CurrentSet->bUseStartBox)
			//{
			//	
			//}
			//else
			//{
			//	if(ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();
		}

	}
	else if (CurrentSet->bUsePLCRobot == TRUE)
	{
		CurrentSet->bUseScanner = 0;
		CurrentSet->bUseStartBox = 0;
		if (ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();
	}




	//===============
	// Init I2C Port
	//===============
	OldSet.sI2cAdcComPort  = CurrentSet->sI2cAdcComPort;
	OldSet.wI2cAdcBaudRate = CurrentSet->wI2cAdcBaudRate;
	OldSet.bUseI2cAdc = CurrentSet->bUseI2cAdc;

	CurrentSet->sI2cAdcComPort .Format(GetComPortVal(IDC_CMB_COMPORT_I2C_ADC));
	CurrentSet->wI2cAdcBaudRate			= GetBaudRateVal(IDC_CMB_BAUDRATE_I2C_ADC);
	CurrentSet->bUseI2cAdc				= m_ctrlUseI2cAdcChk.GetCheck();
	
	if (CurrentSet->bUseI2cAdc)
	{
		if ((CurrentSet->bUseI2cAdc != OldSet.bUseI2cAdc)
			|| (CurrentSet->sI2cAdcComPort != OldSet.sI2cAdcComPort)
			|| (CurrentSet->wI2cAdcBaudRate != OldSet.wI2cAdcBaudRate))
		{
			if (I2cAdcCtrl.m_bPortOpen)				I2cAdcCtrl.PortClose();
			InitI2cAdcCommPort(CurrentSet->sI2cAdcComPort, CurrentSet->wI2cAdcBaudRate);
		}
	}
	else
	{
		if (I2cAdcCtrl.m_bPortOpen)		I2cAdcCtrl.PortClose(); //I2cAdcCtrl.PortClose();
	}




	//========================
	// Init HDMI Generator
	//========================

	OldSet.bUseHDMIGen = CurrentSet->bUseHDMIGen;
	OldSet.sHDMIComPort = CurrentSet->sHDMIComPort;
	OldSet.wHDMIBaudRate = CurrentSet->wHDMIBaudRate;


	CurrentSet->sHDMIComPort.Format(GetComPortVal(IDC_CMB_COMPORT_HDMI));
	CurrentSet->wHDMIBaudRate			= GetBaudRateVal(IDC_CMB_BAUDRATE_HDMI);
	CurrentSet->bUseHDMIGen				= m_ctrlUseHdmiGenChk.GetCheck();
	if (CurrentSet->bUseHDMIGen)
	{
		if ((   CurrentSet->bUseHDMIGen   != OldSet.bUseHDMIGen)
			|| (CurrentSet->sHDMIComPort  != OldSet.sHDMIComPort)
			|| (CurrentSet->wHDMIBaudRate != OldSet.wHDMIBaudRate))
		{
			if (HDMIGeneratorCtrl.m_bPortOpen)		HDMIGeneratorCtrl.PortClose();
			InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}
	else
	{
		if (HDMIGeneratorCtrl.m_bPortOpen) HDMIGeneratorCtrl.PortClose();
	}



	//CurrentSet->nHDMIGenType = m_nHdmiType;




//========================
// Init AV Convertor
//========================

	OldSet.bUseAVC      = CurrentSet->bUseAVC;
	OldSet.sAvcComPort = CurrentSet->sAvcComPort;
	OldSet.wAvcBaudRate = CurrentSet->wAvcBaudRate;


	CurrentSet->bUseAVC		= m_ctrlUseAvcChk.GetCheck();
	CurrentSet->wAvcBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_AVC);
	CurrentSet->sAvcComPort.Format(GetComPortVal(IDC_CMB_COMPORT_AVC));


	if (CurrentSet->bUseAVC)
	{

		if ((   CurrentSet->bUseAVC      != OldSet.bUseAVC)
			|| (CurrentSet->sAvcComPort  != OldSet.sAvcComPort)
			|| (CurrentSet->wAvcBaudRate != OldSet.wAvcBaudRate))
		{
			if (gAvConvertorCtrl.m_bPortOpen)	
				gAvConvertorCtrl.PortClose();
			InitAVC(CurrentSet->sAvcComPort, CurrentSet->wAvcBaudRate);
		}
	}
	else
	{
		if (gAvConvertorCtrl.m_bPortOpen) gAvConvertorCtrl.PortClose();
	}


	//========================
	// Init LNB Detector
	//========================

	OldSet.bUseLNB = CurrentSet->bUseLNB;
	OldSet.sLNBComPort = CurrentSet->sLNBComPort;
	OldSet.wLNBBaudRate = CurrentSet->wLNBBaudRate;

	CurrentSet->bUseLNB = m_ctrlUseLnbChk.GetCheck();
	CurrentSet->wLNBBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_LNB);
	CurrentSet->sLNBComPort.Format(GetComPortVal(IDC_CMB_COMPORT_LNB));
	if (CurrentSet->bUseLNB)
	{
		if ((   CurrentSet->bUseLNB      != OldSet.bUseLNB)
			|| (CurrentSet->sLNBComPort  != OldSet.sLNBComPort)
			|| (CurrentSet->wLNBBaudRate != OldSet.wLNBBaudRate))
		{
			if (gLNBDetectorCtrl.m_bPortOpen)		
				gLNBDetectorCtrl.PortClose();
			InitLNB_Detector(CurrentSet->sLNBComPort, CurrentSet->wLNBBaudRate);
		}
	}
	else
	{
		if (gLNBDetectorCtrl.m_bPortOpen)		gLNBDetectorCtrl.PortClose();
	}


//========================
// Init Jig 
//========================
//AUTO
	//-
	OldSet.bUseJig = CurrentSet->bUseJig;
	OldSet.sJigComPort = CurrentSet->sJigComPort;
	OldSet.wJigBaudRate = CurrentSet->wJigBaudRate;
	
	CurrentSet->bUseJig = m_ctrlUseJigChk.GetCheck();
	CurrentSet->wJigBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_JIG);
	CurrentSet->sJigComPort.Format(GetComPortVal(IDC_CMB_COMPORT_JIG));
	
	if (CurrentSet->bUseJig)
	{
		if ((CurrentSet->bUseJig != OldSet.bUseJig)
			|| (CurrentSet->sJigComPort != OldSet.sJigComPort)
			|| (CurrentSet->wJigBaudRate != OldSet.wJigBaudRate))
		{
			if (gJigCtrl.m_bPortOpen)
				gJigCtrl.PortClose();
			//if (gJigCtrl.m_bPortOpen == FALSE)
			{
				if (!gJigCtrl.Create(CurrentSet->sJigComPort, CurrentSet->wJigBaudRate)) {
					sMsg.Format("Failed to open COM port (%s)", CurrentSet->sJigComPort);
					AfxMessageBox(sMsg);
				}
			}

			if (gJigCtrl.m_bPortOpen == TRUE)
			{
				gJigCtrl.Init();
			}
		}


	}
	else
	{
		if (gJigCtrl.m_bPortOpen)				gJigCtrl.PortClose();
	}
	
	//OldSet.bUseJig = CurrentSet->bUseJig;
	//OldSet.sJigComPort = CurrentSet->sJigComPort;
	//OldSet.wJigBaudRate = CurrentSet->wJigBaudRate;
////////////////////////////////////////////////////////////
	if (gJigCtrl.m_bID_Ctrl_PortOpen)
		gJigCtrl.ID_Ctrl_PortClose();
	CurrentSet->bUseCountIDJig = m_ctrlUseCountJigChk.GetCheck();
	CurrentSet->wCountJigBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_COUNT_JIG);
	CurrentSet->sCountJigComPort.Format(GetComPortVal(IDC_CMB_COMPORT_COUNT_JIG));
	
	
		
	GetDlgItemText(IDC_EDIT_COUNT_JIG, sMsg);
	CurrentSet->wJig_ID_Count_MAX = _ttoi(sMsg);

	if (CurrentSet->bUseCountIDJig)
	{
		
		if (!gJigCtrl.ID_Ctrl_Create(CurrentSet->sCountJigComPort, CurrentSet->wCountJigBaudRate)) {
			sMsg.Format("Failed to open COM port (%s)", CurrentSet->sCountJigComPort);
			AfxMessageBox(sMsg);
		}
	}

/////////////////////////////////////////////////////
	if (gJigCtrl.m_bDPMS_PortOpen)
		gJigCtrl.DPMS_PortClose();
	CurrentSet->bUseDPMS = m_ctrlUseDPMSChk.GetCheck();
	CurrentSet->wDPMSBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_DPMS);
	CurrentSet->sDPMSComPort.Format(GetComPortVal(IDC_CMB_COMPORT_DPMS));

	if (CurrentSet->bUseDPMS)
	{

		if (!gJigCtrl.DPMS_Create(CurrentSet->sDPMSComPort, CurrentSet->wDPMSBaudRate)) {
			sMsg.Format("Failed to open COM port (%s)", CurrentSet->sDPMSComPort);
			AfxMessageBox(sMsg);
		}
	}



//
	

	

	//========================
	// Init V-F Measurement
	//========================
	OldSet.bUseVfm = CurrentSet->bUseVfm;
	OldSet.sVfmComPort = CurrentSet->sVfmComPort;
	OldSet.wVfmBaudRate = CurrentSet->wVfmBaudRate;

	CurrentSet->bUseVfm			= m_ctrlUseVfmChk.GetCheck();
	CurrentSet->wVfmBaudRate	= GetBaudRateVal(IDC_CMB_BAUDRATE_VFM);
	CurrentSet->sVfmComPort.Format(GetComPortVal(IDC_CMB_COMPORT_VFM));
	if (CurrentSet->bUseVfm)
	{
		if ((   CurrentSet->bUseVfm     != OldSet.bUseVfm)
			|| (CurrentSet->sVfmComPort != OldSet.sVfmComPort)
			|| (CurrentSet->wVfmBaudRate != OldSet.wVfmBaudRate))
		{
			if (gVF1000Ctrl.m_bPortOpen)		gVF1000Ctrl.PortClose();
			InitVfm_Detector(CurrentSet->sVfmComPort, CurrentSet->wVfmBaudRate);
		}
	}
	else
	{
		if (gVF1000Ctrl.m_bPortOpen)		gVF1000Ctrl.PortClose();
	}


	//========================
	// Init DP Generator
	//========================
	OldSet.bUseDpg = CurrentSet->bUseDpg;
	OldSet.sDpgComPort = CurrentSet->sDpgComPort;
	OldSet.wDpgBaudRate = CurrentSet->wDpgBaudRate;
	OldSet.nDP_TimeSel = CurrentSet->nDP_TimeSel;

	CurrentSet->nDP_TimeSel = m_nDP_TimeSel;
	CurrentSet->bUseDpg = m_cCheckDP_PortEnable.GetCheck();
	CurrentSet->wDpgBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_DPGEN);
	CurrentSet->sDpgComPort.Format(GetComPortVal(IDC_CMB_COMPORT_DPGEN));
	if (CurrentSet->bUseDpg)
	{
		if ((CurrentSet->bUseDpg != OldSet.bUseDpg)
			|| (CurrentSet->sDpgComPort != OldSet.sDpgComPort)
			|| (CurrentSet->wDpgBaudRate != OldSet.wDpgBaudRate))
		{
			if (DPGeneratorCtrl.m_bPortOpen)		DPGeneratorCtrl.PortClose();
			InitDPGen(CurrentSet->sDpgComPort, CurrentSet->wDpgBaudRate);
		}
		else if (CurrentSet->nDP_TimeSel != OldSet.nDP_TimeSel)
		{
			DPGeneratorCtrl.SetTime_Control(CurrentSet->nDP_TimeSel);//m_nDP_TimeSel
		}
	}
	else
	{
		if (DPGeneratorCtrl.m_bPortOpen)		DPGeneratorCtrl.PortClose();
	}


	CurrentSet->nTVControlType = m_ctlTVControlMode;
	CurrentSet->bAutoSourceControl = m_bAutoSourceControl;
	CurrentSet->bAutoPatternControl = m_bAutoPatternControl;
	CurrentSet->bScanNotUse = m_bScanNotUse;
	if(m_bAutoAVSwitchBoxControl){
		CurrentSet->nAutoAVSwitchBoxControl = 1;
	}
	else{
		CurrentSet->nAutoAVSwitchBoxControl = 2;
	}


	

	//========================
	// Init MHL Buffer
	//========================
//	if(CurrentSet->bUseMHL)
//	{
//		InitMHL_Detector(CurrentSet->sMHLComPort, CurrentSet->wMHLBaudRate);
//	}


	g_pView->SaveRegistrySetting();

}

void COptComportPage::OnChkUseAvswitchbox() 
{
	SetAVSwitchBoxGroup(m_ctrlUseAVSwitchBoxChk.GetCheck());
}

void COptComportPage::OnChkUsePatterngen() 
{
	SetPatternGenGroup(m_ctrlUsePatternGenChk.GetCheck());
}


void COptComportPage::OnChkUseTvcommport() 
{
	SetTVCommPortGroup(m_ctrlUseTVCommPortChk.GetCheck());
}


void COptComportPage::OnChkUseScanner() 
{
	SetScannerGroup(m_ctrlUseScannerChk.GetCheck());

	if (m_ctrlUseScannerChk.GetCheck())
	{
		m_ctrlUseStartBoxChk.SetCheck(0);
		SetStartBoxGroup(0);
	}
}


void COptComportPage::OnBtnInitJoystick() 
{
/*	if(!g_pView->UserControlInit(TRUE))
	{
		AfxMessageBox("[User Control Input] Init Fail!");
	}
	else 
	{
		AfxMessageBox("[User Control Input] Init Pass!");
	}
	*/
}

//+add kwmoon 080502
void COptComportPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnComportOptApply();

	CPropertyPage::OnOK();
}

//+add kwmoon 080625
void COptComportPage::OnChkUseI2cAdc() 
{
	// TODO: Add your control notification handler code here
	SetI2cAdcGroup(m_ctrlUseI2cAdcChk.GetCheck());
}


void COptComportPage::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//+add kwmoon 080910
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}

	
	CString sMsg = _T("");
	
	OnBtnComportOptApply();

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
}

void COptComportPage::OnChkUseHdmigen() 
{
	SetHDMIGenGroup(m_ctrlUseHdmiGenChk.GetCheck());
}

// add 20100622
void COptComportPage::OnChkUseAvc() 
{
	SetAvcGroup(m_ctrlUseAvcChk.GetCheck());
}

void COptComportPage::OnChkUseLnb() 
{
	SetLnbGroup(m_ctrlUseLnbChk.GetCheck());
	
}
void COptComportPage::OnChkUseJig() 
{
	SetJigGroup(m_ctrlUseJigChk.GetCheck());
	
}


void COptComportPage::OnBnClickedChkUseVfm()
{
	SetVfmGroup(m_ctrlUseVfmChk.GetCheck());
}


void COptComportPage::OnBnClickedChkUseStartBox()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetStartBoxGroup(m_ctrlUseStartBoxChk.GetCheck());
	if (m_ctrlUseStartBoxChk.GetCheck())
	{
		m_ctrlUseScannerChk.SetCheck(0);
		SetScannerGroup(0);
	}
}


void COptComportPage::OnBnClickedChkUseDpgen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	SetDPGroup(m_cCheckDP_PortEnable.GetCheck());
}


void COptComportPage::OnBnClickedRadioDpTime60()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i = 0;
	CEnvironmentData OldSet;

	UpdateData();
//========================
// Init DP GTenerator
//========================
	OldSet.bUseDpg = CurrentSet->bUseDpg;
	OldSet.sDpgComPort = CurrentSet->sDpgComPort;
	OldSet.wDpgBaudRate = CurrentSet->wDpgBaudRate;
	OldSet.nDP_TimeSel = CurrentSet->nDP_TimeSel;

	CurrentSet->nDP_TimeSel = m_nDP_TimeSel;
	CurrentSet->bUseDpg = m_cCheckDP_PortEnable.GetCheck();
	CurrentSet->wDpgBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_DPGEN);
	CurrentSet->sDpgComPort.Format(GetComPortVal(IDC_CMB_COMPORT_DPGEN));

	if (CurrentSet->bUseDpg)
	{
		if ((CurrentSet->bUseDpg != OldSet.bUseDpg)
			|| (CurrentSet->sDpgComPort != OldSet.sDpgComPort)
			|| (CurrentSet->wDpgBaudRate != OldSet.wDpgBaudRate))
		{
			if (DPGeneratorCtrl.m_bPortOpen)		DPGeneratorCtrl.PortClose();
			InitDPGen(CurrentSet->sDpgComPort, CurrentSet->wDpgBaudRate);
		}
		else if (CurrentSet->nDP_TimeSel != OldSet.nDP_TimeSel)
		{
			DPGeneratorCtrl.SetTime_Control(CurrentSet->nDP_TimeSel);//m_nDP_TimeSel
		}
	}
}


void COptComportPage::OnBnClickedRadioDpTime30()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedRadioDpTime60();
}


void COptComportPage::OnBnClickedChkUseCountJig()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetCountJigGroup(m_ctrlUseCountJigChk.GetCheck());
}


void COptComportPage::OnBnClickedChkUseDpms()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDPMSGroup(m_ctrlUseDPMSChk.GetCheck());
}
