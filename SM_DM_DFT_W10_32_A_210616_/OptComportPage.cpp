// OptComportPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptComportPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
#include "Global.h"
#include "TVCommCtrl.h"
#include "AvSwitchBoxCtrl.h"
#include "PatternGeneratorCtl.h"
#include "ScannerCtrl.h"

#include "IrCheckerCtrl.h"
#include "thermometer.h"

extern CTVCommCtrl TVCommCtrl;
extern CAvSwitchBox AvSwitchBoxCtrl;
extern CPatternGenerator PatternGeneratorCtrl;
extern CScannerCtrl ScannerCtrl;


extern CDATsysView*  g_pView;
//////////////////////////////////////////////////////////////////////////
// COptComportPage property page

IMPLEMENT_DYNCREATE(COptComportPage, CPropertyPage)

COptComportPage::COptComportPage() : CPropertyPage(COptComportPage::IDD)
{
	//{{AFX_DATA_INIT(COptComportPage)
	m_szAvSwitchBoxSettingValueTitle = _T("");
	m_ctlTVControlMode = -1;
	m_bAutoSourceControl = FALSE;
	m_bAutoPatternControl = FALSE;
	//}}AFX_DATA_INIT
}

COptComportPage::~COptComportPage()
{
}

void COptComportPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptComportPage)
	DDX_Control(pDX, IDC_COMBO_PROCESS_TYPE, m_ctrlProcessTypeCmb);
	DDX_Control(pDX, IDC_COMBO_CHIP_TYPE1, m_ctrlChipTypeCmb1);
	DDX_Control(pDX, IDC_COMBO_CHIP_TYPE2, m_ctrlChipTypeCmb2);
	DDX_Control(pDX, IDC_CHK_USE_TH, m_ctrlUseThChk);
	DDX_Control(pDX, IDC_CMB_COMPORT_TH, m_ctrlThComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_TH, m_ctrlThBaudCmb);
	DDX_Control(pDX, IDC_CMB_COMPORT_HDMI, m_ctrlMdmiComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_HDMI, m_ctrlHdmiBaudCmb);
	DDX_Control(pDX, IDC_CHK_USE_HDMIGEN, m_ctrlUseHdmiGenChk);
	DDX_Control(pDX, IDC_CMB_COMPORT_IR_CHK, m_ctrlIrChkComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_IR_CHK, m_ctrlIrChkBaudCmb);
	DDX_Control(pDX, IDC_CHK_USE_IR_CHK, m_ctrlUseIrChkChk);
	DDX_Control(pDX, IDC_CMB_COMPORT_SCANNER, m_ctrlScannerComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_SCANNER, m_ctrlScannerBaudCmb);
	DDX_Control(pDX, IDC_CHK_USE_SCANNER, m_ctrlUseScannerChk);
	//DDX_Control(pDX, IDC_CHK_USE_TVCOMMPORT, m_ctrlUseTVCommPortChk);
	DDX_Control(pDX, IDC_CHK_USE_PATTERNGEN, m_ctrlUsePatternGenChk);
	DDX_Control(pDX, IDC_CHK_USE_AVSWITCHBOX, m_ctrlUseAVSwitchBoxChk);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_AV_SWITCHBOX, m_ctrlSwitchBoxBaudCmb);
	DDX_Control(pDX, IDC_CMB_COMPORT_AV_SWITCHBOX, m_ctrlAvSwitchBoxComportCmb);
	DDX_Control(pDX, IDC_CMB_COMPORT_TV, m_ctrlTVComportCmb);
	//DDX_Control(pDX, IDC_CMB_BAUDRATE_TV, m_ctrlTVBaudCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_TVCOM1, m_ctrlTVBaudCmb1);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_TVCOM2, m_ctrlTVBaudCmb2);
	DDX_Control(pDX, IDC_CMB_COMPORT_PATTERN, m_ctrlPatternComportCmb);
	DDX_Control(pDX, IDC_CMB_BAUDRATE_PATTERN, m_ctrlPatternBaudCmb);
	DDX_Radio(pDX, IDC_RADIO_TV_CONTROL_RS232C, m_ctlTVControlMode);
	DDX_Check(pDX, IDC_CHK_AUTO_SOURCE_CONTROL, m_bAutoSourceControl);
	DDX_Check(pDX, IDC_CHK_AUTO_PATTERN_CONTROL, m_bAutoPatternControl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptComportPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptComportPage)
	ON_BN_CLICKED(IDC_BTN_COMPORT_OPT_APPLY, OnBtnComportOptApply)
	ON_BN_CLICKED(IDC_CHK_USE_AVSWITCHBOX, OnChkUseAvswitchbox)
	ON_BN_CLICKED(IDC_CHK_USE_PATTERNGEN, OnChkUsePatterngen)
	//ON_BN_CLICKED(IDC_CHK_USE_TVCOMMPORT, OnChkUseTvcommport)
	ON_BN_CLICKED(IDC_CHK_USE_SCANNER, OnChkUseScanner)
	ON_BN_CLICKED(IDC_BTN_INIT_JOYSTICK, OnBtnInitJoystick)
	ON_BN_CLICKED(IDC_CHK_USE_IR_CHK, OnChkUseIrChk)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_CHK_USE_HDMIGEN, OnChkUseHdmigen)
	ON_BN_CLICKED(IDC_CHK_USE_TH, OnChkUseTh)
	//}}AFX_MSG_MAP
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
	GetDlgItem(IDC_STATIC_TVCOMMPORT_COMPORT2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_TV1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_TV2)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TVCOMMPORT_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TVCOMMPORT_PARITY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_TVCOM1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_PARITY_TV1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_TVCOM2)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_PARITY_TV2)->EnableWindow(bEnable);
}

void COptComportPage::SetAVSwitchBoxGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_AVSWITCHBOX_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_AV_SWITCHBOX)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_AVSWITCHBOX_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_AV_SWITCHBOX)->EnableWindow(bEnable);
}

void COptComportPage::SetScannerGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_SCANNER_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_SCANNER)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SCANNER_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_SCANNER)->EnableWindow(bEnable);
}

void COptComportPage::SetIrChkGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_IR_CHK_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_IR_CHK)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_IR_CHK_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_IR_CHK)->EnableWindow(bEnable);
	
}
void COptComportPage::SetHDMIGenGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_HDMIGEN_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_HDMI)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_HDMIGEN_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_HDMI)->EnableWindow(bEnable);
}
void COptComportPage::SetThGroup(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_TH_COMPORT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_COMPORT_TH)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TH_BAUDRATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMB_BAUDRATE_TH)->EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////

BOOL COptComportPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

 	//=======================
	// Comport No & BaudRate
 	//=======================
	InitComPort(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate,IDC_CMB_COMPORT_PATTERN, IDC_CMB_BAUDRATE_PATTERN );
	InitComPort(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate, IDC_CMB_COMPORT_TV1, IDC_CMB_BAUDRATE_TVCOM1);
	//InitComPort(CurrentSet->sTVComPort2, CurrentSet->wTVBaudRate2, IDC_CMB_COMPORT_TV2, IDC_CMB_BAUDRATE_TVCOM2);
	InitComPort(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate,IDC_CMB_COMPORT_AV_SWITCHBOX, IDC_CMB_BAUDRATE_AV_SWITCHBOX );
	InitComPort(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate, IDC_CMB_COMPORT_SCANNER, IDC_CMB_BAUDRATE_SCANNER);
	InitComPort(CurrentSet->sIrChkComPort, CurrentSet->wIrChkBaudRate,IDC_CMB_COMPORT_IR_CHK, IDC_CMB_BAUDRATE_IR_CHK );
	InitComPort(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate,IDC_CMB_COMPORT_HDMI, IDC_CMB_BAUDRATE_HDMI );
	InitComPort(CurrentSet->sThComPort, CurrentSet->wThBaudRate,IDC_CMB_COMPORT_TH, IDC_CMB_BAUDRATE_TH );

	CComboBox* pTVParity1 = (CComboBox*)GetDlgItem(IDC_CMB_PARITY_TV1);
	pTVParity1->SetCurSel(CurrentSet->nTVParity);

	//CComboBox* pTVParity2 = (CComboBox*)GetDlgItem(IDC_CMB_PARITY_TV2);
	//pTVParity2->SetCurSel(CurrentSet->nTVParity2);

	m_ctrlUsePatternGenChk.SetCheck(CurrentSet->bUsePatternGen);
	SetPatternGenGroup(CurrentSet->bUsePatternGen);

	//m_ctrlUseTVCommPortChk.SetCheck(CurrentSet->bUseTVCommPort);
	//SetTVCommPortGroup(CurrentSet->bUseTVCommPort);

	m_ctrlUseAVSwitchBoxChk.SetCheck(CurrentSet->bUseAVSwitchBox);
	SetAVSwitchBoxGroup(CurrentSet->bUseAVSwitchBox);
	
	m_ctrlUseScannerChk.SetCheck(CurrentSet->bUseScanner);
	SetScannerGroup(CurrentSet->bUseScanner);

	m_ctrlUseIrChkChk.SetCheck(CurrentSet->bUseIrChk);
	SetIrChkGroup(CurrentSet->bUseIrChk);

	//CurrentSet->bUseHDMIGen = 0;
	m_ctrlUseHdmiGenChk.SetCheck(CurrentSet->bUseHDMIGen);
	SetHDMIGenGroup(CurrentSet->bUseHDMIGen);

	m_ctrlUseThChk.SetCheck(CurrentSet->bUseTh);
	SetThGroup(CurrentSet->bUseTh);

//	InitComPort(CurrentSet->sDmmComPort, CurrentSet->wDmmBaudRate,IDC_CMB_COMPORT_DMM, IDC_CMB_BAUDRATE_DMM );
//	m_ctrlUseDmmChk.SetCheck(CurrentSet->bUseDmm);
//	SetDmmGroup(CurrentSet->bUseDmm);

	m_ctlTVControlMode		 = CurrentSet->nTVControlType;
	m_bAutoSourceControl	 = CurrentSet->bAutoSourceControl;
	m_bAutoPatternControl	 = CurrentSet->bAutoPatternControl;

	m_ctrlChipTypeCmb1.SetCurSel(CurrentSet->nChipType - 1);
	//m_ctrlChipTypeCmb2.SetCurSel(CurrentSet->nChipType2 - 1);
	m_ctrlProcessTypeCmb.SetCurSel(CurrentSet->nProcessType);

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

	if(PatternGeneratorCtrl.m_bPortOpen)	PatternGeneratorCtrl.PortClose();
	if(TVCommCtrl.m_bPortOpen)				TVCommCtrl.PortClose();
	if(AvSwitchBoxCtrl.m_bPortOpen)			AvSwitchBoxCtrl.PortClose();
	if(ScannerCtrl.m_bPortOpen)				ScannerCtrl.PortClose();
	if(gIrCheckerCtrl.m_bPortOpen)			gIrCheckerCtrl.PortClose();
	if(HDMIGeneratorCtrl.m_bPortOpen)		HDMIGeneratorCtrl.PortClose();
	if(gThermometerCtrl.m_bPortOpen)		gThermometerCtrl.CloseComm();

	// Comport

	CurrentSet->sPatternComPort.Format(GetComPortVal(IDC_CMB_COMPORT_PATTERN));
	

	//CurrentSet->bUseTVCommPort = m_ctrlUseTVCommPortChk.GetCheck();


	//CurrentSet->sTVComPort1.Format(GetComPortVal(IDC_CMB_COMPORT_TV1));
	//CurrentSet->sTVComPort2.Format(GetComPortVal(IDC_CMB_COMPORT_TV2));
	//CurrentSet->nChipType1 = m_ctrlChipTypeCmb1.GetCurSel() + 1;
	//CurrentSet->nChipType2 = m_ctrlChipTypeCmb2.GetCurSel() + 1;
	//CurrentSet->wTVBaudRate1 = GetBaudRateVal(IDC_CMB_BAUDRATE_TVCOM1);
	//CurrentSet->wTVBaudRate2 = GetBaudRateVal(IDC_CMB_BAUDRATE_TVCOM2);
	//CComboBox* pTVParity1 = (CComboBox*)GetDlgItem(IDC_CMB_PARITY_TV1);
	//CurrentSet->nTVParity1 = pTVParity1->GetCurSel();
	//CComboBox* pTVParity2 = (CComboBox*)GetDlgItem(IDC_CMB_PARITY_TV2);
	//CurrentSet->nTVParity2 = pTVParity2->GetCurSel();

	CurrentSet->sTVComPort.Format(GetComPortVal(IDC_CMB_COMPORT_TV1));
	CurrentSet->nChipType = m_ctrlChipTypeCmb1.GetCurSel() + 1;
	CurrentSet->wTVBaudRate = GetBaudRateVal(IDC_CMB_BAUDRATE_TVCOM1);
	CComboBox* pTVParity = (CComboBox*)GetDlgItem(IDC_CMB_PARITY_TV1);
	CurrentSet->nTVParity = pTVParity->GetCurSel();
	
	if (g_nRunningProcessNo == 1)
	{
		//CurrentSet->sTVComPort1.Format(GetComPortVal(IDC_CMB_COMPORT_TV1));
		////CurrentSet->sTVComPort2.Format(GetComPortVal(IDC_CMB_COMPORT_TV2));
		//CurrentSet->nChipType1 = m_ctrlChipTypeCmb1.GetCurSel() + 1;
		////CurrentSet->nChipType2 = m_ctrlChipTypeCmb2.GetCurSel() + 1;
		//CurrentSet->wTVBaudRate1 = GetBaudRateVal(IDC_CMB_BAUDRATE_TVCOM1);
		////CurrentSet->wTVBaudRate2 = GetBaudRateVal(IDC_CMB_BAUDRATE_TVCOM2);
		//CComboBox* pTVParity1 = (CComboBox*)GetDlgItem(IDC_CMB_PARITY_TV1);
		//CurrentSet->nTVParity1 = pTVParity1->GetCurSel();
		////CComboBox* pTVParity2 = (CComboBox*)GetDlgItem(IDC_CMB_PARITY_TV2);
		////CurrentSet->nTVParity2 = pTVParity2->GetCurSel();

		//CurrentSet->sTVComPort = CurrentSet->sTVComPort1;
		//CurrentSet->nChipType = CurrentSet->nChipType1;
		//CurrentSet->wTVBaudRate = CurrentSet->wTVBaudRate1;
		//CurrentSet->nTVParity = CurrentSet->nTVParity1;

	}
	else
	{
		//CurrentSet->sTVComPort = CurrentSet->sTVComPort2;
		//CurrentSet->sTVComPort = CurrentSet->sTVComPort2;
		//CurrentSet->nChipType = CurrentSet->nChipType2;
		//CurrentSet->wTVBaudRate = CurrentSet->wTVBaudRate2;
		//CurrentSet->nTVParity = CurrentSet->nTVParity2;

	}
	
	
	CurrentSet->sAvSwitchBoxComPort.Format(GetComPortVal(IDC_CMB_COMPORT_AV_SWITCHBOX));
	CurrentSet->sScannerComPort.Format(GetComPortVal(IDC_CMB_COMPORT_SCANNER));
	CurrentSet->sIrChkComPort.Format(GetComPortVal(IDC_CMB_COMPORT_IR_CHK));
	CurrentSet->sHDMIComPort.Format(GetComPortVal(IDC_CMB_COMPORT_HDMI));
	CurrentSet->sThComPort.Format(GetComPortVal(IDC_CMB_COMPORT_TH));

	// BaudRate
	CurrentSet->wPatternBaudRate		= GetBaudRateVal(IDC_CMB_BAUDRATE_PATTERN);

	CurrentSet->wAvSwitchBoxBaudRate	= GetBaudRateVal(IDC_CMB_BAUDRATE_AV_SWITCHBOX);
	CurrentSet->wScannerBaudRate		= GetBaudRateVal(IDC_CMB_BAUDRATE_SCANNER);
	CurrentSet->wIrChkBaudRate			= GetBaudRateVal(IDC_CMB_BAUDRATE_IR_CHK);
	CurrentSet->wHDMIBaudRate			= GetBaudRateVal(IDC_CMB_BAUDRATE_HDMI);
	CurrentSet->wThBaudRate				= GetBaudRateVal(IDC_CMB_BAUDRATE_TH);



	// Comport Use
	CurrentSet->bUsePatternGen			= m_ctrlUsePatternGenChk.GetCheck();
	CurrentSet->bUseAVSwitchBox			= m_ctrlUseAVSwitchBoxChk.GetCheck();
	CurrentSet->bUseScanner				= m_ctrlUseScannerChk.GetCheck();
	CurrentSet->bUseIrChk				= m_ctrlUseIrChkChk.GetCheck();
	CurrentSet->bUseHDMIGen =  m_ctrlUseHdmiGenChk.GetCheck();
	CurrentSet->bUseTh					= m_ctrlUseThChk.GetCheck();

// add 20100622
//	CurrentSet->bUseDmm			= m_ctrlUseDmmChk.GetCheck();
//	CurrentSet->wDmmBaudRate	= GetBaudRateVal(IDC_CMB_BAUDRATE_DMM);
//	CurrentSet->sDmmComPort.Format(GetComPortVal(IDC_CMB_COMPORT_DMM));
//-


	CurrentSet->nTVControlType = m_ctlTVControlMode;
	CurrentSet->bAutoSourceControl = m_bAutoSourceControl;
	CurrentSet->bAutoPatternControl = m_bAutoPatternControl;

	//========================
	// Init Pattern Generator
	//========================
	if(CurrentSet->bUsePatternGen)
	{
		InitPatternGen(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
	}
	else
	{
		if(PatternGeneratorCtrl.m_bPortOpen) PatternGeneratorCtrl.PortClose();
	}
	
	//====================
	// Init TV Tuner Port
	//====================
	if(1)//CurrentSet->bUseTVCommPort)
	{
		InitTVCommPort(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
	}
	else
	{
		if(TVCommCtrl.m_bPortOpen) TVCommCtrl.PortClose();
	}
	
	//=======================
	// Init AvSwitchBox Port
	//=======================
	if(CurrentSet->bUseAVSwitchBox)
	{
		InitAvSwitchBox(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate);
	}
	else
	{
		if(AvSwitchBoxCtrl.m_bPortOpen) AvSwitchBoxCtrl.PortClose();
	}

	//===================
	// Init Scanner Port
	//===================
	if(CurrentSet->bUseScanner)
	{
		g_pView->InitScanner(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate);
	}
	else
	{
		if(ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();
	}

	if(CurrentSet->bUseIrChk)
	{
		InitIrChkCommPort(CurrentSet->sIrChkComPort, CurrentSet->wIrChkBaudRate);
	}
	else
	{
		gIrCheckerCtrl.PortClose();
	}

	//========================
	// Init HDMI Generator
	//========================
	if(CurrentSet->bUseHDMIGen)
	{
		InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
	}
	else
	{
		if(HDMIGeneratorCtrl.m_bPortOpen) HDMIGeneratorCtrl.PortClose();
	}

	//========================
	// Init Thermometer
	//========================
	if(CurrentSet->bUseTh)
	{
		InitThermometer(CurrentSet->sThComPort, CurrentSet->wThBaudRate);
	}
	else
	{
		if(gThermometerCtrl.m_bPortOpen)		gThermometerCtrl.CloseComm();
	}



	CurrentSet->nProcessType = m_ctrlProcessTypeCmb.GetCurSel();

	g_pView->SaveRegistrySetting();

	//+move kwmoon 080904
/*	//+add kwmoon 080821 : Add FileExists Check
	if(FileExists(CurrentSet->sModelIni))
	{
		SaveModelIniFile(CurrentSet->sModelIni);
	}
	else
	{
		sMsg.Format("Failed to save file.\n[%s]",CurrentSet->sModelIni);
		AfxMessageBox(sMsg);
	}
*/
}

void COptComportPage::OnChkUseAvswitchbox() 
{
	SetAVSwitchBoxGroup(m_ctrlUseAVSwitchBoxChk.GetCheck());
}

void COptComportPage::OnChkUsePatterngen() 
{
	SetPatternGenGroup(m_ctrlUsePatternGenChk.GetCheck());
}

//
//void COptComportPage::OnChkUseTvcommport() 
//{
//	SetTVCommPortGroup(m_ctrlUseTVCommPortChk.GetCheck());
//}


void COptComportPage::OnChkUseScanner() 
{
	SetScannerGroup(m_ctrlUseScannerChk.GetCheck());
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
void COptComportPage::OnChkUseIrChk() 
{
	// TODO: Add your control notification handler code here
	SetIrChkGroup(m_ctrlUseIrChkChk.GetCheck());
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
void COptComportPage::OnChkUseTh() 
{
	SetThGroup(m_ctrlUseThChk.GetCheck());
}
