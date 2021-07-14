// DlgTestPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgTestPage.h"
#include "afxdialogex.h"
#include "FileControl.h"
#include "DlgPassword.h"
#include "DlgConfig.h"

#include <InitGuid.h>
extern CAcqVoltageSamples_IntClkDlg *m_P_Main;
// CDlgTestPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTestPage, CDialogEx)

CDlgTestPage::CDlgTestPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTestPage::IDD, pParent)
	//, m_CheckDO_1(FALSE)
	//, m_CheckDO_2(FALSE)
	//, m_CheckDO_3(FALSE)
	//, m_CheckDO_4(FALSE)

	//, m_CheckDOLED_1(FALSE)
	//, m_CheckDOLED_2(FALSE)
	//, m_CheckDOLED_3(FALSE)
	//, m_CheckDOLED_4(FALSE)
	, m_ServerPortNumber(_T(""))
	, m_SetPortNumber(_T(""))
	, m_EditFPGAVer(_T(""))
	, m_EditDXD_ServerVer(_T(""))
	, m_EditMicomVer(_T(""))
	, m_EditAVR_Ver(_T(""))
{

}

CDlgTestPage::~CDlgTestPage()
{
}

void CDlgTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_SELECT_RM_TYPE, m_ComboRmType);

	//DDX_Check(pDX, IDC_CHECK_DO_1, m_CheckDO_1);
	//DDX_Check(pDX, IDC_CHECK_DO_2, m_CheckDO_2);
	//DDX_Check(pDX, IDC_CHECK_DO_3, m_CheckDO_3);
	//DDX_Check(pDX, IDC_CHECK_DO_4, m_CheckDO_4);

	//DDX_Check(pDX, IDC_CHECK_DI_1, m_Check_DI_1);
	//DDX_Check(pDX, IDC_CHECK_DI_2, m_Check_DI_2);
	//DDX_Check(pDX, IDC_CHECK_DI_3, m_Check_DI_3);
	//DDX_Check(pDX, IDC_CHECK_DI_4, m_Check_DI_4);

	/*
	CLabel_text m_LedLAN;
	CLabel_text m_LedUSB1;
	CLabel_text m_LedUSB2;
	*/



	//	DDX_Control(pDX, IDC_COMBO_SIGNAL, m_cComboSignalCarrier);

	//DDX_Control(pDX, IDC_COMBO_RM_TEST1 , m_cComboRmTest[0]);	
	//DDX_Control(pDX, IDC_COMBO_RM_TEST2 , m_cComboRmTest[1] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST3 , m_cComboRmTest[2] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST4 , m_cComboRmTest[3] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST5 , m_cComboRmTest[4] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST6 , m_cComboRmTest[5] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST7 , m_cComboRmTest[6] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST8 , m_cComboRmTest[7] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST9 , m_cComboRmTest[8] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST10, m_cComboRmTest[9] );
	//DDX_Control(pDX, IDC_COMBO_RM_TEST11, m_cComboRmTest[10]);
	//DDX_Control(pDX, IDC_COMBO_RM_TEST12, m_cComboRmTest[11]);

	//DDX_Control(pDX, IDC_TEXTBOX_TPG_MESSAGE, m_cTestPageMessage);
	//DDX_Control(pDX, IDC_LABEL_TPG_MESSAGE, m_cTpgMessage);
	//	DDX_Control(pDX, IDC_COMBO_RMT_OUT, m_cComboRmtSelect);

	//DDX_Control(pDX, IDC_COMBO_PCB_TYPE, m_ComboPcbType);
		
	DDX_Control(pDX, IDC_COMBO_BAUD4, m_cComboScanBaud);
	DDX_Control(pDX, IDC_COMBO_PARITY4, m_cComboScanParity);
	DDX_Control(pDX, IDC_COMBO_COMPORT_SCAN, m_cComboScanPort);

	//	DDX_Control(pDX, IDC_IPADDRESS_GMES1, m_IPaddressGmes);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT1, m_ServerPortNumber);
	DDX_Text(pDX, IDC_EDIT_SET_PORT, m_SetPortNumber);
	DDX_Text(pDX, IDC_EDIT_FPGA_VER, m_EditFPGAVer);
	DDX_Text(pDX, IDC_EDIT_DXD_SERVER_VER, m_EditDXD_ServerVer);
	DDX_Text(pDX, IDC_EDIT_MICOM_VER, m_EditMicomVer);
	DDX_Text(pDX, IDC_EDIT_AVR_VER, m_EditAVR_Ver);
}


BEGIN_MESSAGE_MAP(CDlgTestPage, CDialogEx)
	//ON_BN_CLICKED(IDC_CHECK_DO_1, &CDlgTestPage::OnBnClickedCheckDo1)
	//ON_BN_CLICKED(IDC_CHECK_DO_2, &CDlgTestPage::OnBnClickedCheckDo2)
	//ON_BN_CLICKED(IDC_CHECK_DO_3, &CDlgTestPage::OnBnClickedCheckDo3)
	//ON_BN_CLICKED(IDC_CHECK_DO_4, &CDlgTestPage::OnBnClickedCheckDo4)
	ON_WM_TIMER()
	ON_WM_DESTROY()

	//ON_BN_CLICKED(IDC_BUTTON_FREQ2 , &CDlgTestPage::OnBnClickedButtonFreq2 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ3 , &CDlgTestPage::OnBnClickedButtonFreq3 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ4 , &CDlgTestPage::OnBnClickedButtonFreq4 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ5 , &CDlgTestPage::OnBnClickedButtonFreq5 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ6 , &CDlgTestPage::OnBnClickedButtonFreq6 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ7 , &CDlgTestPage::OnBnClickedButtonFreq7 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ8 , &CDlgTestPage::OnBnClickedButtonFreq8 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ9 , &CDlgTestPage::OnBnClickedButtonFreq9 )
	//ON_BN_CLICKED(IDC_BUTTON_FREQ10, &CDlgTestPage::OnBnClickedButtonFreq10)

	//ON_BN_CLICKED(IDC_BUTTON_FREQ_SET, &CDlgTestPage::OnBnClickedButtonFreqSet)
	//ON_BN_CLICKED(IDC_BUTTON_FREQ_READ, &CDlgTestPage::OnBnClickedButtonFreqRead)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_1, &CDlgTestPage::OnBnClickedButtonRmtest1)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_2, &CDlgTestPage::OnBnClickedButtonRmtest2)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_3, &CDlgTestPage::OnBnClickedButtonRmtest3)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_4, &CDlgTestPage::OnBnClickedButtonRmtest4)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_5, &CDlgTestPage::OnBnClickedButtonRmtest5)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_6, &CDlgTestPage::OnBnClickedButtonRmtest6)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_7, &CDlgTestPage::OnBnClickedButtonRmtest7)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_8, &CDlgTestPage::OnBnClickedButtonRmtest8)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_9, &CDlgTestPage::OnBnClickedButtonRmtest9)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_10, &CDlgTestPage::OnBnClickedButtonRmtest10)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_11, &CDlgTestPage::OnBnClickedButtonRmtest11)
	//ON_BN_CLICKED(IDC_BUTTON_RMTEST_12, &CDlgTestPage::OnBnClickedButtonRmtest12)
	//ON_CBN_SELCHANGE(IDC_COMBO_SELECT_RM_TYPE, &CDlgTestPage::OnCbnSelchangeComboSelectRmType)

	//ON_BN_CLICKED(IDC_BUTTON_MIC1_ON, &CDlgTestPage::OnBnClickedButtonMic1On)
	//ON_BN_CLICKED(IDC_BUTTON_MIC2_ON, &CDlgTestPage::OnBnClickedButtonMic2On)
	//ON_BN_DOUBLECLICKED(IDC_BUTTON_MIC1_ON, &CDlgTestPage::OnBnDoubleclickedButtonMic1On)
	//ON_BN_DOUBLECLICKED(IDC_BUTTON_MIC2_ON, &CDlgTestPage::OnBnDoubleclickedButtonMic2On)
	ON_BN_CLICKED(IDOK, &CDlgTestPage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_GESWVER, &CDlgTestPage::OnBnClickedButtonGeswver)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgTestPage::OnBnClickedButtonSave)

	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT_SCAN, &CDlgTestPage::OnCbnSelchangeComboComportScan)
	ON_BN_CLICKED(IDC_BUTTON_SCANNER_PORT_CHECK, &CDlgTestPage::OnBnClickedButtonScannerPortCheck)
END_MESSAGE_MAP()



BOOL CDlgTestPage::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			//ClickCbtnStart();		
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

// CDlgTestPage 메시지 처리기입니다.
BOOL CDlgTestPage::OnInitDialog()
{
    CDialog::OnInitDialog();


	InitPage();


	
	mbaud_List[0] = 9600;
	mbaud_List[1] = 19200;
	mbaud_List[2] = 38400;
	mbaud_List[3] = 57600;
	mbaud_List[4] = 115200;

	
//	m_EditLEDPortString.Format(_T("%d"),m_nPortLED );

	
		
	m_cComboScanBaud.AddString("9600");
	m_cComboScanBaud.AddString("19200");
	m_cComboScanBaud.AddString("38400");
	m_cComboScanBaud.AddString("57600");
	m_cComboScanBaud.AddString("115200");
	m_cComboScanBaud.SetCurSel(0);
	for(int i = 0; i < 5; i++)
	{
		if(m_nBaudScan == mbaud_List[i])
		{
			m_cComboScanBaud.SetCurSel(i);
		}
	}

	
	m_cComboScanParity.AddString("NONE");
	m_cComboScanParity.AddString("EVEN");
	m_cComboScanParity.AddString("ODD");
	m_cComboScanParity.SetCurSel(m_nParityScan);

	ComportSearch();


	//g_nRemotePortNumber = _ttoi(dlg.m_ServerPortNumber);]


	m_SetPortNumber.Format(_T("%d"),g_nSetPortNumber);
	SetDlgItemText(IDC_IPADDRESS_SET, g_nSetIPAddress);


	m_ServerPortNumber.Format(_T("%d"),g_nRemotePortNumber);
	SetDlgItemText(IDC_IPADDRESS_GMES1, g_nRemoteIPAddress);


	m_EditFPGAVer.Format(_T("%d.%d.%d"), 
			g_FPGA_Ver_event, 
			g_FPGA_Ver_major ,
			g_FPGA_Ver_minor);

<<<<<<< HEAD
	m_EditDXD_ServerVer.Format( _T("%d.%d.%d.%02d"), 
=======
	m_EditDXD_ServerVer.Format( _T("%d.%d%d.%02d"), 
>>>>>>> 349ccb8b749484336f4564a18cb267dd5110eddb
		g_DSERVER_Ver_event, 
		g_DSERVER_Ver_major ,
		g_DSERVER_Ver_minor, 
		g_DSERVER_Ver_release);

	m_EditMicomVer.Format( _T("%d.%d.%d"), 
		g_Microchip_Ver_event, 
		g_Microchip_Ver_major ,
		g_Microchip_Ver_minor);

	m_EditAVR_Ver.Format( _T("%d.%d.%d"), 
		g_AVR_Ver_event, 
		g_AVR_Ver_major ,
		g_AVR_Ver_minor);

	UpdateData(FALSE);

	SetTimer(1, 30, NULL);

	
    return TRUE;
}

BOOL CDlgTestPage::InitPage()
{

	return 0;
}


void CDlgTestPage::UpdateRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static int lOldDi[4] = {3,3,3,3};

	static int lOldLan = 0;
	static int lOldUsb1 = 0;
	static int lOldUsb2 = 0;


	CString str;


	
}



void CDlgTestPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	KillTimer(1);


	//UpdateRead();



	
	SetTimer(1, 30, NULL);

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgTestPage::OnDestroy()
{
	CDialogEx::OnDestroy();
	
}


void CDlgTestPage::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;
	CString strTemp1;
	CString strTemp2;
	CString strTemp3;
	CString strTemp4;
	int lPos = 0;
	int lNumber = 0;


	UpdateData();
	g_nSetPortNumber = _ttoi(m_SetPortNumber);
	//m_SetPortNumber.Format(_T("%d"),g_nSetPortNumber);
	GetDlgItemText(IDC_IPADDRESS_SET, g_nSetIPAddress);


	g_nRemotePortNumber = _ttoi(m_ServerPortNumber);
	//m_ServerPortNumber.Format(_T("%d"),g_nRemotePortNumber);
	GetDlgItemText(IDC_IPADDRESS_GMES1, g_nRemoteIPAddress);


	lPos = m_EditFPGAVer.Find(".");
	strTemp1 = m_EditFPGAVer.Left(lPos);
	strTemp = m_EditFPGAVer.Mid(lPos+1);
	strTemp2 = strTemp.Left(lPos);
	lPos = strTemp.Find(".");
	strTemp = strTemp.Mid(lPos+1);
	strTemp3 = strTemp;

		
	g_FPGA_Ver_event = _ttoi(strTemp1); 
	g_FPGA_Ver_major  = _ttoi(strTemp2); 
	g_FPGA_Ver_minor = _ttoi(strTemp3); 



	
	lPos = m_EditDXD_ServerVer.Find(".");
	strTemp = m_EditDXD_ServerVer.Mid(lPos+1);
	strTemp1 = m_EditDXD_ServerVer.Left(lPos);

	lPos = strTemp.Find(".");
	strTemp2 = strTemp.Left(lPos);
	strTemp = strTemp.Mid(lPos+1);
<<<<<<< HEAD
	lPos = strTemp.Find(".");
	strTemp3 = strTemp.Left(lPos);
	strTemp = strTemp.Mid(lPos+1);
	strTemp4 = strTemp;


	g_DSERVER_Ver_event = _ttoi(strTemp1);  
	g_DSERVER_Ver_major = _ttoi(strTemp2); 
	g_DSERVER_Ver_minor = _ttoi(strTemp3);
	g_DSERVER_Ver_release	= _ttoi(strTemp4); 
=======
	strTemp3 = strTemp;
	g_DSERVER_Ver_event = _ttoi(strTemp1);  
	lNumber = _ttoi(strTemp2); 
	g_DSERVER_Ver_major = lNumber/10; 
	g_DSERVER_Ver_minor = lNumber%10;  
	g_DSERVER_Ver_release	= _ttoi(strTemp3); 
>>>>>>> 349ccb8b749484336f4564a18cb267dd5110eddb
		

	lPos = m_EditMicomVer.Find(".");
	strTemp = m_EditMicomVer.Mid(lPos+1);
	strTemp1 = m_EditMicomVer.Left(lPos);
	strTemp2 = strTemp.Left(lPos);
	lPos = strTemp.Find(".");
	strTemp = strTemp.Mid(lPos+1);
	strTemp3 = strTemp;

		
	g_Microchip_Ver_event  = _ttoi(strTemp1); 
	g_Microchip_Ver_major   = _ttoi(strTemp2); 
	g_Microchip_Ver_minor  = _ttoi(strTemp3); 

	lPos = m_EditAVR_Ver.Find(".");
	strTemp = m_EditAVR_Ver.Mid(lPos+1);
	strTemp1 = m_EditAVR_Ver.Left(lPos);
	strTemp2 = strTemp.Left(lPos);
	lPos = strTemp.Find(".");
	strTemp = strTemp.Mid(lPos+1);
	strTemp3 = strTemp;

		
	g_AVR_Ver_event = _ttoi(strTemp1); 
	g_AVR_Ver_major = _ttoi(strTemp2); 
	g_AVR_Ver_minor = _ttoi(strTemp3); 


	
		int l_port = 0;
	int lBaud = 0;
	int lParity = 0;

	if(m_PortCnt > 0)
	{
		l_port = nNumberPort[m_cComboScanPort.GetCurSel()];	
		if((l_port > 0)&&(l_port < 128))
		{
			g_nPort_Scanner = l_port;	
		}
	}
	lBaud =  mbaud_List[m_cComboScanBaud.GetCurSel()];
	if((lBaud >= 9600)&&(lBaud <= 115200))
	{
		g_nBaud_Scanner = lBaud;	
	}
	lParity = m_cComboScanParity.GetCurSel();
	if((lParity >= 0)&&(lParity <= 2))
	{
		g_nParity_Scanner = lParity;	
	}
	

	CFileControl ldlg;
	ldlg.SaveConfigSeqData();

	CDialogEx::OnOK();
}

	

void CDlgTestPage::OnBnClickedButtonGeswver()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_P_Main->GetDetParam() )
	{
		
		m_EditFPGAVer.Format(_T("%d.%d.%d"), 
			m_P_Main->m_FPGA_Ver_event, 
			m_P_Main->m_FPGA_Ver_major ,
			m_P_Main->m_FPGA_Ver_minor);

		m_EditDXD_ServerVer.Format( _T("%d.%d%d.%02d"), 
			m_P_Main->m_DSERVER_Ver_event, 
			m_P_Main->m_DSERVER_Ver_major ,
			m_P_Main->m_DSERVER_Ver_minor, 
			m_P_Main->m_DSERVER_Ver_release);

		m_EditMicomVer.Format( _T("%d.%d.%d"), 
			m_P_Main->m_Microchip_Ver_event, 
			m_P_Main->m_Microchip_Ver_major ,
			m_P_Main->m_Microchip_Ver_minor);

		m_EditAVR_Ver.Format( _T("%d.%d.%d"), 
			m_P_Main->m_AVR_Ver_event, 
			m_P_Main->m_AVR_Ver_major ,
			m_P_Main->m_AVR_Ver_minor);

		UpdateData(FALSE);
	}

}



void CDlgTestPage::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strTemp;
	CString strTemp1;
	CString strTemp2;
	CString strTemp3;
	CString strTemp4;
	int lPos = 0;
	int lNumber = 0;


	UpdateData();
	g_nSetPortNumber = _ttoi(m_SetPortNumber);
	//m_SetPortNumber.Format(_T("%d"),g_nSetPortNumber);
	GetDlgItemText(IDC_IPADDRESS_SET, g_nSetIPAddress);


	g_nRemotePortNumber = _ttoi(m_ServerPortNumber);
	//m_ServerPortNumber.Format(_T("%d"),g_nRemotePortNumber);
	GetDlgItemText(IDC_IPADDRESS_GMES1, g_nRemoteIPAddress);


	lPos = m_EditFPGAVer.Find(".");
	strTemp = m_EditFPGAVer.Mid(lPos+1);
	strTemp1 = m_EditFPGAVer.Left(lPos);
	lPos = strTemp.Find(".");
	strTemp = strTemp.Mid(lPos+1);
	strTemp2 = strTemp.Left(lPos);
	strTemp3 = strTemp;

		
	g_FPGA_Ver_event = _ttoi(strTemp1); 
	g_FPGA_Ver_major  = _ttoi(strTemp2); 
	g_FPGA_Ver_minor = _ttoi(strTemp3); 


	lPos = m_EditDXD_ServerVer.Find(".");
	strTemp = m_EditDXD_ServerVer.Mid(lPos+1);
	strTemp1 = m_EditDXD_ServerVer.Left(lPos);

	lPos = strTemp.Find(".");
	strTemp2 = strTemp.Left(lPos);
	strTemp = strTemp.Mid(lPos+1);
	strTemp3 = strTemp;
	g_DSERVER_Ver_event = _ttoi(strTemp1);  
	lNumber = _ttoi(strTemp2); 
	g_DSERVER_Ver_major = lNumber/10; 
	g_DSERVER_Ver_minor = lNumber%10;  
	g_DSERVER_Ver_release	= _ttoi(strTemp3); 
	

		

	lPos = m_EditMicomVer.Find(".");
	strTemp = m_EditMicomVer.Mid(lPos+1);
	strTemp1 = m_EditMicomVer.Left(lPos);
	strTemp = strTemp.Mid(lPos+1);
	strTemp2 = strTemp.Left(lPos);
	strTemp3 = strTemp;

		
	g_Microchip_Ver_event  = _ttoi(strTemp1); 
	g_Microchip_Ver_major   = _ttoi(strTemp2); 
	g_Microchip_Ver_minor  = _ttoi(strTemp3); 

	lPos = m_EditAVR_Ver.Find(".");
	strTemp = m_EditAVR_Ver.Mid(lPos+1);
	strTemp1 = m_EditAVR_Ver.Left(lPos);
	strTemp = strTemp.Mid(lPos+1);
	strTemp2 = strTemp.Left(lPos);
	strTemp3 = strTemp;

		
	g_AVR_Ver_event = _ttoi(strTemp1); 
	g_AVR_Ver_major = _ttoi(strTemp2); 
	g_AVR_Ver_minor = _ttoi(strTemp3); 


	int l_port = 0;
	int lBaud = 0;
	int lParity = 0;

	if(m_PortCnt > 0)
	{
		l_port = nNumberPort[m_cComboScanPort.GetCurSel()];	
		if((l_port > 0)&&(l_port < 128))
		{
			g_nPort_Scanner = l_port;	
		}
	}
	lBaud =  mbaud_List[m_cComboScanBaud.GetCurSel()];
	if((lBaud >= 9600)&&(lBaud <= 115200))
	{
		g_nBaud_Scanner = lBaud;	
	}
	lParity = m_cComboScanParity.GetCurSel();
	if((lParity >= 0)&&(lParity <= 2))
	{
		g_nParity_Scanner = lParity;	
	}

	CFileControl ldlg;
	ldlg.SaveConfigSeqData();
}




void CDlgTestPage::ComportSearch()//void CDlgTestPage::GetDeviceInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;
	CString strTempCom;

	int ArrPort[256];
	//SearchPort( ArrPort);
	GetDeviceInfo();


//	m_cComboPCBPort.ResetContent();
	/*		m_cComboMainPort.SetCurSel(i);
			m_cComboPCBPort.SetCurSel(i);
				m_cComboLEDPort.SetCurSel(i);
				m_cComboScanPort.SetCurSel(i);*/
		

	if(m_PortCnt > 0)
	{
		for(int i = 0; i < m_PortCnt; i++)
		{		
		//	m_cComboPortSel.AddString(strPort[i]);

			strTemp = strPort[i].Mid(strPort[i].Find("COM")+3);
			strTemp = strTemp.Left(strTemp.Find(")"));

			strTempCom = strPort[i].Mid(strPort[i].Find("COM"));
			strTempCom = strTempCom.Left(strTempCom.Find(")"));
				
		
			m_cComboScanPort.AddString(strTempCom);


		
			nNumberPort[i] = _ttoi(strTemp);

		
			if(m_nPortScan == nNumberPort[i])
			{
				m_cComboScanPort.SetCurSel(i);
			}
					
	
		}
	//	m_cComboPortSel.SetCurSel(0);	

		strTemp.Format(_T("컴포트 %d개가 검색되었습니다."), m_PortCnt);
		MessageBox(strTemp);		
				
	}
	else
	{
		MessageBox("사용가능한 통신 포트가 없습니다.");
	}
}


DEFINE_GUID( GUID_PCIE,                 0x4d36e978L, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18 );

void CDlgTestPage::GetDeviceInfo()
{
	m_PortCnt = 0;
	GUID Input_GUID = GUID_PCIE;//GUID_DEVINTERFACE_USB_DEVICE;
	HDEVINFO hDevInfo = SetupDiGetClassDevs( &Input_GUID, 0, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
    if(hDevInfo == INVALID_HANDLE_VALUE)
    {
        return ;
    }
	DWORD i = 0;

    for ( i = 0; true; ++i)
    {
        SP_DEVINFO_DATA devInfo;
        devInfo.cbSize = sizeof(SP_DEVINFO_DATA);
        BOOL succ = SetupDiEnumDeviceInfo(hDevInfo, i, &devInfo);
        if (GetLastError() == ERROR_NO_MORE_ITEMS)
            break;
        if (!succ) continue;

        SP_INTERFACE_DEVICE_DATA ifInfo;
		
        ifInfo.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);
        if (TRUE != SetupDiEnumDeviceInterfaces(hDevInfo, &devInfo,  &(Input_GUID), 0, &ifInfo))
        {
            if (GetLastError() != ERROR_NO_MORE_ITEMS)
                break;
        }
        interfaces.push_back(ifInfo);

		
	//	WCHAR  szDesc[MAX_PATH] = {0};
		char  szDesc[MAX_PATH] = {0};
		SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfo,
													  SPDRP_DEVICEDESC,
													  0L,
													  (PBYTE)szDesc,
													  2048,
													  0);
	//	wchar_t  szFriendly[MAX_PATH] = {0};
		char  szFriendly[MAX_PATH] = {0};
		SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfo,
													  SPDRP_FRIENDLYNAME,
													  0L,
													  (PBYTE)szFriendly,
													  2048,
													  0);
		
        DWORD requiredSize = 0;
        SetupDiGetDeviceInterfaceDetail(hDevInfo, &(interfaces.at(i)), NULL, NULL, &requiredSize, NULL);
        SP_INTERFACE_DEVICE_DETAIL_DATA* data = (SP_INTERFACE_DEVICE_DETAIL_DATA*) malloc(requiredSize);

        data->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

        if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &(interfaces.at(i)), data, requiredSize, NULL, NULL))
        {
            continue;
        }
		

		CString str;
		str = szDesc;

		CString str1(szFriendly);
		CString str2(data->DevicePath);
		//strPort[i] = str;
		strPort[i] = str1;

        devicePaths.push_back(data);

		delete data;
		if(i >= 49)
			break;
    }

	m_PortCnt = i;
	
}



void CDlgTestPage::OnBnClickedButtonScannerPortCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_ComPortScanner.mPort_Opened)
	{
		m_ComPortScanner.ClosePort();
	}			

	CString str;
	if(m_PortCnt > 0)
	{
		m_PortCntCurrent = m_cComboScanPort.GetCurSel();

		
//		if(m_ComPortScanner.InitPort(this, nNumberPort[m_PortCntCurrent], 9600, 'N', 8, 1, EV_RXCHAR, 4096 ))
		char lParityChar[3] = {'N', 'E', 'O'};
		if(m_ComPortScanner.InitPort(this, nNumberPort[m_PortCntCurrent], mbaud_List[m_cComboScanBaud.GetCurSel()], lParityChar[m_cComboScanParity.GetCurSel()], 8, 1, EV_RXCHAR, 4096 ))
		
		{		
			m_ComPortScanner.StartMonitoring();
			m_ComPortScanner.m_RxMessageID = WM_COMM_RXCHAR_SCAN;
			
		}
		else
		{
			str.Format(_T("Scanner COM Port %d Open FAIL !!"),nNumberPort[m_PortCntCurrent] );
			MessageBox(str);
		
		}
	}
	
	//m_Edit_RX_Data = "";
	//m_RecvCntSCAN = 0;	
	UpdateData(FALSE);
	
}


void CDlgTestPage::OnCbnSelchangeComboComportScan()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_Edit_RX_Data = "";
	//m_RecvCntSCAN = 0;	
	UpdateData(FALSE);

	CString str;
	if(m_PortCnt > 0)
	{
		m_PortCntCurrent = m_cComboScanPort.GetCurSel();

		if(m_ComPortScanner.mPort_Opened)
		{
			m_ComPortScanner.ClosePort();
		}
			
		char lParityChar[3] = {'N', 'E', 'O'};
		if(m_ComPortScanner.InitPort(this, nNumberPort[m_PortCntCurrent], mbaud_List[m_cComboScanBaud.GetCurSel()], lParityChar[m_cComboScanParity.GetCurSel()], 8, 1, EV_RXCHAR, 4096 ))
		{		
			m_ComPortScanner.StartMonitoring();
			m_ComPortScanner.m_RxMessageID = WM_COMM_RXCHAR_SCAN;
			
		}
		else
		{
			str.Format(_T("Scanner COM Port %d Open FAIL !!"),nNumberPort[m_PortCntCurrent]);
			MessageBox(str);
			//MessageDisplay(1, str);
			/*StatusDisplayMsg(str, 1 );*/
		}
	}

	//m_Edit_RX_Data = "";
	//m_RecvCntSCAN = 0;	
	UpdateData(FALSE);
}
