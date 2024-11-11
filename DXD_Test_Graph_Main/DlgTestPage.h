#pragma once
#include "afxwin.h"
#include "textbox_okng.h"
#include "label_text.h"
#include "afxcmn.h"
//#include "cwboolean_do1.h"
#include "AcqVoltageSamples_IntClkDlg.h"

#include <devguid.h>								// GUID_DEVCLASS_PORTS

#include <setupapi.h>					// ::SetupDi********* 
//#include "afxwin.h"
#pragma comment(lib, "setupapi.lib")
#include <vector>

// CDlgTestPage 대화 상자입니다.

class CDlgTestPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTestPage)

public:
	CDlgTestPage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTestPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEST_PAGE };
public:
	BOOL InitPage();
	void UpdateRead();
	CString m_IPaddressGmes;

	int		mbaud_List[10];

	
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
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
//	CComboBox m_ComboRmType;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	

	//afx_msg void OnBnClickedCheckDo1();
	//afx_msg void OnBnClickedCheckDo2();
	//afx_msg void OnBnClickedCheckDo3();
	//afx_msg void OnBnClickedCheckDo4();
	//BOOL m_CheckDO_1;
	//BOOL m_CheckDO_2;
	//BOOL m_CheckDO_3;
	//BOOL m_CheckDO_4;
	//BOOL m_Check_DI_1;
	//BOOL m_Check_DI_2;
	//BOOL m_Check_DI_3;
	//BOOL m_Check_DI_4;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	CComboBox m_cComboScanBaud;
	CComboBox m_cComboScanParity;
	CComboBox m_cComboScanPort;



	//afx_msg void OnBnClickedButtonFreq1();
	//afx_msg void OnBnClickedButtonFreq2();
	//afx_msg void OnBnClickedButtonFreq3();
	//afx_msg void OnBnClickedButtonFreq4();
	//afx_msg void OnBnClickedButtonFreq5();
	//afx_msg void OnBnClickedButtonFreq6();
	//afx_msg void OnBnClickedButtonFreq7();
	//afx_msg void OnBnClickedButtonFreq8();
	//afx_msg void OnBnClickedButtonFreq9();
	//afx_msg void OnBnClickedButtonFreq10();



	//afx_msg void OnBnClickedButtonFreqSet();
	//afx_msg void OnBnClickedButtonFreqRead();
	//CComboBox m_cComboSignalCarrier;
	//CComboBox m_cComboRmTest[12] ;
	/*CComboBox m_cComboRmTest2 ;
	CComboBox m_cComboRmTest3 ;
	CComboBox m_cComboRmTest4 ;
	CComboBox m_cComboRmTest5 ;
	CComboBox m_cComboRmTest6 ;
	CComboBox m_cComboRmTest7 ;
	CComboBox m_cComboRmTest8 ;
	CComboBox m_cComboRmTest9 ;
	CComboBox m_cComboRmTest10;
	CComboBox m_cComboRmTest11;
	CComboBox m_cComboRmTest12;*/
	//afx_msg void OnBnClickedButtonRmtest1();
	//afx_msg void OnBnClickedButtonRmtest2();
	//afx_msg void OnBnClickedButtonRmtest3();
	//afx_msg void OnBnClickedButtonRmtest4();
	//afx_msg void OnBnClickedButtonRmtest5();
	//afx_msg void OnBnClickedButtonRmtest6();
	//afx_msg void OnBnClickedButtonRmtest7();
	//afx_msg void OnBnClickedButtonRmtest8();
	//afx_msg void OnBnClickedButtonRmtest9();
	//afx_msg void OnBnClickedButtonRmtest10();
	//afx_msg void OnBnClickedButtonRmtest11();
	//afx_msg void OnBnClickedButtonRmtest12();
	//afx_msg void OnCbnSelchangeComboSelectRmType();
//	CTextbox_okng m_cTestPageMessage;
//	DECLARE_EVENTSINK_MAP()
//	void ClickButtonTestConfig();
//	CLabel_text m_cTpgMessage;
	//CComboBox m_cComboRmtSelect;


	//CComboBox m_ComboPcbType;


	//afx_msg void OnBnClickedButtonMic1On();
	//afx_msg void OnBnClickedButtonMic2On();
	//afx_msg void OnBnDoubleclickedButtonMic1On();
	//afx_msg void OnBnDoubleclickedButtonMic2On();
	afx_msg void OnBnClickedOk();
	
	CString m_ServerPortNumber;
	CString m_SetPortNumber;
	afx_msg void OnBnClickedButtonGeswver();
	CString m_EditFPGAVer;
	CString m_EditDXD_ServerVer;
	CString m_EditMicomVer;
	CString m_EditAVR_Ver;
	afx_msg void OnBnClickedButtonSave();

	afx_msg void OnBnClickedButtonScannerPortCheck();
	afx_msg void OnCbnSelchangeComboComportScan();
//	BOOL m_Radio_HK_HQ;
	//int m_Radio_HK_HQ;
	//CComboBox m_cComboMicomHKHQ;
	//afx_msg void OnCbnSelchangeComboMicomHkhqType();
	afx_msg void OnBnClickedCancel();

	CString m_EditRoicMinValue[16];
	CString m_EditRoicMaxValue[16];

};
