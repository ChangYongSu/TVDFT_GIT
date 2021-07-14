// DlgConfig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgConfig.h"
#include "afxdialogex.h"

#include <InitGuid.h>
// CDlgConfig 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgConfig, CDialogEx)

CDlgConfig::CDlgConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConfig::IDD, pParent)
//	, m_EditPortString(_T(""))
	//, m_EditMesTnsTimeout(_T(""))
	//, m_EditMesAvnReplyTimeout(_T(""))
	//, m_EditMesUserID_CEID(_T(""))
	//, m_EditMesPswrdRPTID(_T(""))
	//, m_EditMesLine(_T(""))
	//, m_EditMesInspStep(_T(""))
//	, m_EditSetPortString(_T(""))
//	, m_EditLEDPortString(_T(""))
	//, m_PortNumber(_T(""))
	, m_Edit_RX_Data(_T(""))
	//, m_ServerPortNumber(_T(""))
{

	//m_RecvCnt = 0;
	//m_RecvCntPCB = 0;
	//m_RecvCntLED = 0;
}

CDlgConfig::~CDlgConfig()
{
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_COMPORT, m_EditPortString);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_cComboBaud);


	//DDX_Text(pDX, IDC_EDIT_MES_TNS, m_EditMesTnsTimeout);
	//DDX_Text(pDX, IDC_EDIT_MES_AVN, m_EditMesAvnReplyTimeout);
	//DDX_Text(pDX, IDC_EDIT_MES_USERID, m_EditMesUserID_CEID);
	//DDX_Text(pDX, IDC_EDIT_MES_PSWRD, m_EditMesPswrdRPTID);
	//DDX_Text(pDX, IDC_EDIT_MES_LINE, m_EditMesLine);
	//DDX_Text(pDX, IDC_EDIT_MES_INSP_STEP, m_EditMesInspStep);


	//DDX_Text(pDX, IDC_EDIT_COMPORT2, m_EditSetPortString);
	//DDX_Text(pDX, IDC_EDIT_COMPORT3, m_EditLEDPortString);

	//DDX_Control(pDX, IDC_COMBO_BAUD2, m_cComboSetBaud);
	//DDX_Control(pDX, IDC_COMBO_BAUD3, m_cComboLEDBaud);
	//DDX_Control(pDX, IDC_COMBO_PARITY, m_cComboParity);
	//DDX_Control(pDX, IDC_COMBO_PARITY2, m_cComboSetParity);
	//DDX_Control(pDX, IDC_COMBO_PARITY3, m_cComboLedParity);
//	DDX_Control(pDX, IDC_IPADDRESS_GMES, m_CtrlIpaddressGmes);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT1, m_ServerPortNumberCM45);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT2, m_ServerPortNumberCM89);
	


	//DDX_Control(pDX, IDC_COMBO_PORT_SEARCH, m_cComboPortSel);
	//DDX_Control(pDX, IDC_COMBO_BAUD4, m_cComboScanBaud);
	//DDX_Control(pDX, IDC_COMBO_PARITY4, m_cComboScanParity);
	//DDX_Control(pDX, IDC_COMBO_COMPORT_SCAN, m_cComboScanPort);
	//DDX_Control(pDX, IDC_COMBO_COMPORT_LED, m_cComboLEDPort);
	//DDX_Control(pDX, IDC_COMBO_COMPORT_PCB, m_cComboPCBPort);
	//DDX_Control(pDX, IDC_COMBO_COMPORT_MAIN, m_cComboMainPort);
	DDX_Text(pDX, IDC_EDIT_RX_DATA, m_Edit_RX_Data);
//	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_ServerPortNumber);
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgConfig::OnBnClickedOk)
//	ON_BN_CLICKED(IDC_BUTTON_PORT_CHECK, &CDlgConfig::OnBnClickedButtonPortCheck)
	ON_WM_TIMER()
	ON_WM_DESTROY()
		
//	ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveData)
//	ON_MESSAGE(WM_COMM_RXCHAR_PCB,OnReceiveDataPCB)
//	ON_MESSAGE(WM_COMM_RXCHAR_LED,OnReceiveDataLED)
//	ON_MESSAGE(WM_COMM_RXCHAR_SCAN,OnReceiveDataSCAN)
//	ON_BN_CLICKED(IDC_BUTTON_SET_PORT_CHECK, &CDlgConfig::OnBnClickedButtonSetPortCheck)
//	ON_BN_CLICKED(IDC_BUTTON_LED_PORT_CHECK2, &CDlgConfig::OnBnClickedButtonLedPortCheck2)
//	ON_BN_CLICKED(IDC_BUTTON_SCANNER_PORT_CHECK, &CDlgConfig::OnBnClickedButtonScannerPortCheck)
//	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT_SCAN, &CDlgConfig::OnCbnSelchangeComboComportScan)
ON_BN_CLICKED(IDC_BUTTON_SCANNER_PORT_CHECK, &CDlgConfig::OnBnClickedButtonScannerPortCheck)
END_MESSAGE_MAP()


// CDlgConfig 메시지 처리기입니다.

BOOL CDlgConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	//SetDlgItemText(IDC_IPADDRESS_GMES,m_IPaddressGmes);

	/*	
		
int		m_nPortSet;
int		m_nBaudSet;
int		m_nParitySet;

		
int		m_nPortLED;
int		m_nBaudLED;
int		m_nParityLED;	CString m_EditMesLine;
CString m_EditMesInspStep;
CString m_EditSetPortString;
CString m_EditLEDPortString;
CComboBox m_cComboSetBaud;
CComboBox m_cComboLEDBaud;
CComboBox m_cComboParity;
CComboBox m_cComboSetParity;
CComboBox m_cComboLedParity;
		*/


	UpdateData(FALSE);

	//ComportSearch();


	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgConfig::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//nNumberPort[50];
	//int			m_PortCnt;
	UpdateData(TRUE);

	


	//GetDlgItemText(IDC_IPADDRESS_GMES,m_IPaddressGmes);
	
	
	CDialogEx::OnOK();
}



void CDlgConfig::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	CString str;

	KillTimer(1);


	SetTimer(1, 50, NULL);
	CDialogEx::OnTimer(nIDEvent);
}




void CDlgConfig::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.



	
}




void CDlgConfig::OnBnClickedButtonScannerPortCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
