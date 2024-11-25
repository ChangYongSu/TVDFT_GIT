#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <devguid.h>								// GUID_DEVCLASS_PORTS

#include <setupapi.h>					// ::SetupDi********* 
//#include "afxwin.h"
#pragma comment(lib, "setupapi.lib")
#include <vector>


#define PORTCHEK_MODE_IDLE__		0
#define PORTCHEK_MODE_MAIN__		1
#define PORTCHEK_MODE_PCBSET__		2
#define PORTCHEK_MODE_LEDCHECK__	3
#define PORTCHEK_MODE_SCANCHECK__	4


// CDlgConfig 대화 상자입니다.

class CDlgConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgConfig();


	enum { IDD = IDD_DIALOG_CONFIG };

	

	int		mbaud_List[10];

	int m_WaitCnt;

	int		m_nPortScan;
	int		m_nBaudScan;
	int		m_nParityScan;


	void ComportSearch();


	void GetDeviceInfo();
	CString		strPort[50];
	UINT		nNumberPort[50];
	int			m_PortCnt;
	int			m_PortCntCurrent;
	std::vector<SP_INTERFACE_DEVICE_DATA> interfaces;
	std::vector<SP_INTERFACE_DEVICE_DETAIL_DATA*> devicePaths;






protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	
	//afx_msg long OnReceiveData(WPARAM wParam, LPARAM lParam);
	//afx_msg long OnReceiveDataPCB(WPARAM wParam, LPARAM lParam);
	//afx_msg long OnReceiveDataLED(WPARAM wParam, LPARAM lParam);
	//afx_msg long OnReceiveDataSCAN(WPARAM wParam, LPARAM lParam);


	//CString m_EditPortString;
	//CComboBox m_cComboBaud;

	//CString m_EditMesTnsTimeout;
	//CString m_EditMesAvnReplyTimeout;
	//CString m_EditMesUserID_CEID;
	//CString m_EditMesPswrdRPTID;
	//CString m_EditMesLine;
	//CString m_EditMesInspStep;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();


	afx_msg void OnBnClickedButtonPortCheck();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();


//	afx_msg void OnCbnSelchangeComboComportScan();
	//CString m_Edit_RX_Data;
	//CString m_ServerPortNumberCM45;
	CString m_ServerPortNumber[6];
	//CString m_ServerPortNumberCM89;
	CComboBox m_cComboScanPort;
	CComboBox m_cComboScanParity;
	CComboBox m_cComboScanBaud;
	CString m_EditPLC_IP_Address;
	CString m_EditPLC_Port;
};
