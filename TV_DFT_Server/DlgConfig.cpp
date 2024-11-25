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
//	, m_Edit_RX_Data(_T(""))
	//, m_ServerPortNumber(_T(""))
	, m_EditPLC_IP_Address(_T(""))
	, m_EditPLC_Port(_T(""))
{


}

CDlgConfig::~CDlgConfig()
{
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_COMPORT, m_EditPortString);
	//DDX_Control(pDX, IDC_COMBO_BAUD, m_cComboBaud);


	//DDX_Text(pDX, IDC_EDIT_MES_TNS, m_EditMesTnsTimeout);
	//DDX_Text(pDX, IDC_EDIT_MES_AVN, m_EditMesAvnReplyTimeout);
	//DDX_Text(pDX, IDC_EDIT_MES_USERID, m_EditMesUserID_CEID);
	//DDX_Text(pDX, IDC_EDIT_MES_PSWRD, m_EditMesPswrdRPTID);
	//DDX_Text(pDX, IDC_EDIT_MES_LINE, m_EditMesLine);
	//DDX_Text(pDX, IDC_EDIT_MES_INSP_STEP, m_EditMesInspStep);


	//DDX_Text(pDX, IDC_EDIT_COMPORT2, m_EditSetPortString);
	//DDX_Text(pDX, IDC_EDIT_COMPORT3, m_EditLEDPortString);

	//	DDX_Control(pDX, IDC_IPADDRESS_GMES, m_CtrlIpaddressGmes);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT1, m_ServerPortNumber[0]);//IDC_EDIT_SERVER_PORT1
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT2, m_ServerPortNumber[1]);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT3, m_ServerPortNumber[2]);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT4, m_ServerPortNumber[3]);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT5, m_ServerPortNumber[4]);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT6, m_ServerPortNumber[5]);


	//DDX_Text(pDX, IDC_EDIT_SERVER_PORT2, m_ServerPortNumberCM89);



	//	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_ServerPortNumber);
	DDX_Control(pDX, IDC_COMBO_SCAN_PORT, m_cComboScanPort);
	DDX_Control(pDX, IDC_COMBO_SCAN_PARITY, m_cComboScanParity);
	DDX_Control(pDX, IDC_COMBO_SCAN_BAUD, m_cComboScanBaud);
	DDX_Text(pDX, IDC_EDIT_PLC_IP_ADDRESS, m_EditPLC_IP_Address);
	DDX_Text(pDX, IDC_EDIT_PLC_IP_PORT, m_EditPLC_Port);
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PORT_CHECK, &CDlgConfig::OnBnClickedButtonPortCheck)
	ON_WM_TIMER()
	ON_WM_DESTROY()
		


END_MESSAGE_MAP()


// CDlgConfig 메시지 처리기입니다.

BOOL CDlgConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	mbaud_List[0] = 9600;
	mbaud_List[1] = 19200;
	mbaud_List[2] = 38400;
	mbaud_List[3] = 57600;
	mbaud_List[4] = 115200;

	m_cComboScanBaud.AddString("9600");
	m_cComboScanBaud.AddString("19200");
	m_cComboScanBaud.AddString("38400");
	m_cComboScanBaud.AddString("57600");
	m_cComboScanBaud.AddString("115200");
	m_cComboScanBaud.SetCurSel(0);
	for (int i = 0; i < 5; i++)
	{
		if (m_nBaudScan == mbaud_List[i])
		{
			m_cComboScanBaud.SetCurSel(i);
		}
	}


	m_cComboScanParity.AddString("NONE");
	m_cComboScanParity.AddString("EVEN");
	m_cComboScanParity.AddString("ODD");
	m_cComboScanParity.SetCurSel(m_nParityScan);
	


	UpdateData(FALSE);


	ComportSearch();


	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgConfig::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//nNumberPort[50];
	//int			m_PortCnt;
	UpdateData(TRUE);
	int l_port = 0;
	int lBaud = 0;
	int lParity = 0;

	if (m_PortCnt <= 0)
	{
		//	return;
	}
	else
	{
		

		l_port = nNumberPort[m_cComboScanPort.GetCurSel()];
		if ((l_port > 0) && (l_port < 128))
		{
			m_nPortScan = l_port;
		}
	}




	lBaud = mbaud_List[m_cComboScanBaud.GetCurSel()];
	if ((lBaud >= 9600) && (lBaud <= 115200))
	{
		m_nBaudScan = lBaud;
	}
	lParity = m_cComboScanParity.GetCurSel();
	if ((lParity >= 0) && (lParity <= 2))
	{
		m_nParityScan = lParity;
	}

	UpdateData(TRUE);
	
	//m_PortNumber;		
	//m_IPaddressGmes;//m_CtrlIpaddressGmes

	CDialogEx::OnOK();
}





void CDlgConfig::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// PORTCHEK_MODE_IDLE__
//#define PORTCHEK_MODE_MAIN__		1
//#define PORTCHEK_MODE_PCBSET__		2
//#define PORTCHEK_MODE_LEDCHECK__	3
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



void CDlgConfig::OnBnClickedButtonPortCheck()
{
	int i = 0;
}


void CDlgConfig::ComportSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp;
	CString strTempCom;

	int ArrPort[256];
	
	GetDeviceInfo();

	
	m_cComboScanPort.ResetContent();
	
	/*		m_cComboMainPort.SetCurSel(i);
			m_cComboPCBPort.SetCurSel(i);
				m_cComboLEDPort.SetCurSel(i);
				m_cComboScanPort.SetCurSel(i);*/


	if (m_PortCnt > 0)
	{
		for (int i = 0; i < m_PortCnt; i++)
		{
			
			strTemp = strPort[i].Mid(strPort[i].Find("COM") + 3);
			strTemp = strTemp.Left(strTemp.Find(")"));

			strTempCom = strPort[i].Mid(strPort[i].Find("COM"));
			strTempCom = strTempCom.Left(strTempCom.Find(")"));


			m_cComboScanPort.AddString(strTempCom);



			nNumberPort[i] = _ttoi(strTemp);

		
			if (m_nPortScan == nNumberPort[i])
			{
				m_cComboScanPort.SetCurSel(i);
			}


		}
	
		strTemp.Format(_T("COM PORT %dEA FOUNDED!"), m_cComboScanPort.GetCount());
		MessageBox(strTemp);

	}
	else
	{
		MessageBox("Not Found COM Port.!!");

		if ((m_nPortScan > 0) && (m_nPortScan < 99))
		{
			m_PortCnt = 1;
			nNumberPort[0] = m_nPortScan;
			strTempCom.Format("COM %d");
			m_cComboScanPort.AddString(strTempCom);
			m_cComboScanPort.SetCurSel(0);
			
		}
	}




	//for(int i = 1; i < 64; i++)
	//{
	//	if(mComport.m_bConnected)
	//	{
	//		mComport.ClosePort();
	//	}
	//	if(mComport.InitPort(this, (UINT)i, 115200, 'N', 8, 1, EV_RXCHAR, 4096 ))
	//	{
	//		mComport.ClosePort();
	//		strTemp.Format(_T("COM %d"), i);
	//		m_cComboPortSel.AddString(strTemp);	
	//		if(m_nPort == i)
	//		{
	//			m_cComboPortSel.SetCurSel(m_cComboPortSel.GetCount()-1);
	//		}			
	//	}
	//	m_cProgress_Search.SetPos(i*100/64);

	//}
	//m_cProgress_Search.SetPos(100);
	//if(m_cComboPortSel.GetCount() > 0)
	//{
	//	if(m_cComboPortSel.GetCurSel() <= 0)
	//		m_cComboPortSel.SetCurSel(0);	

	//	strTemp.Format(_T("컴포트 %d개가 검색되었습니다."), m_cComboPortSel.GetCount());
	//	SetDlgItemText(IDC_STATIC_STATUS, strTemp);		
	//}
	//else
	//{
	//	MessageBox("사용가능한 통신 포트가 없습니다.");
	//}	
}



DEFINE_GUID(GUID_PCIE, 0x4d36e978L, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18);

void CDlgConfig::GetDeviceInfo()
{
	m_PortCnt = 0;
	GUID Input_GUID = GUID_PCIE;//GUID_DEVINTERFACE_USB_DEVICE;
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&Input_GUID, 0, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		return;
	}
	DWORD i = 0;

	for (i = 0; true; ++i)
	{
		SP_DEVINFO_DATA devInfo;
		devInfo.cbSize = sizeof(SP_DEVINFO_DATA);
		BOOL succ = SetupDiEnumDeviceInfo(hDevInfo, i, &devInfo);
		if (GetLastError() == ERROR_NO_MORE_ITEMS)
			break;
		if (!succ) continue;

		SP_INTERFACE_DEVICE_DATA ifInfo;

		ifInfo.cbSize = sizeof(SP_INTERFACE_DEVICE_DATA);
		if (TRUE != SetupDiEnumDeviceInterfaces(hDevInfo, &devInfo, &(Input_GUID), 0, &ifInfo))
		{
			if (GetLastError() != ERROR_NO_MORE_ITEMS)
				break;
		}
		interfaces.push_back(ifInfo);


		//	WCHAR  szDesc[MAX_PATH] = {0};
		char  szDesc[MAX_PATH] = { 0 };
		SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfo,
			SPDRP_DEVICEDESC,
			0L,
			(PBYTE)szDesc,
			2048,
			0);
		//	wchar_t  szFriendly[MAX_PATH] = {0};
		char  szFriendly[MAX_PATH] = { 0 };
		SetupDiGetDeviceRegistryProperty(hDevInfo, &devInfo,
			SPDRP_FRIENDLYNAME,
			0L,
			(PBYTE)szFriendly,
			2048,
			0);


		DWORD requiredSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &(interfaces.at(i)), NULL, NULL, &requiredSize, NULL);
		SP_INTERFACE_DEVICE_DETAIL_DATA* data = (SP_INTERFACE_DEVICE_DETAIL_DATA*)malloc(requiredSize);

		data->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

		if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &(interfaces.at(i)), data, requiredSize, NULL, NULL))
		{
			continue;
		}

		//m_List.AddString(_T("Desc	: ") + CString(szDesc) + _T("\r\n"));
		//m_List.AddString(_T("Name	: ") + CString(szFriendly) + _T("\r\n"));
		//m_List.AddString(_T("Path	: ") + CString(data->DevicePath) + _T("\r\n"));
		//m_List.AddString(_T("\r\n"));

		CString str;
		str = szDesc;

		CString str1(szFriendly);
		CString str2(data->DevicePath);
		//strPort[i] = str;
		strPort[i] = str1;
		//strPort[i] += str2;


	//	strPort[i].Format( _T("Desc	: %ws %ws %s") ,(CString)szDesc , (CString)szFriendly , (CString)data->DevicePath);

		devicePaths.push_back(data);

		delete data;
		if (i >= 49)
			break;
	}

	m_PortCnt = i;
	//if(m_Port.InitPort(this, CString(devicePaths[1]->DevicePath), 19200, 'N', 8, 1,  EV_RXCHAR, 4096 ))
	//{		
	//	m_Port.m_hWnd_ToSend = this->m_hWnd;
	//	m_Port.StartMonitoring();
	//}

}





