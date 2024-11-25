#pragma once


// CDlgParmAutoRobotPage 대화 상자

class CDlgParmAutoRobotPage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParmAutoRobotPage)

public:
	CDlgParmAutoRobotPage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgParmAutoRobotPage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARM_AUTO_ROBOT_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCommandModelName();
	afx_msg void OnBnClickedBtnCommandLoad();
	afx_msg void OnBnClickedBtnCommandPassUnload();
	afx_msg void OnBnClickedBtnCommandNgUnload();
//	afx_msg void OnBnClickedBtnCommandNgLoad2();
	afx_msg void OnBnClickedBtnCommandFfcRetry();
	afx_msg void OnBnClickedBtnCommandRead();
	CString m_strEditModelName;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CButton m_cCheckLoad;
	CButton m_cCheckUnLoad;
	CButton m_cCheckTestPass;
	CButton m_cCheckTestNg;
	//CButton m_cCheckLoadFast;
	//CButton m_cCheckUnLoadFast;
	CButton m_cCheckReconnect;
	CButton m_cCheckLoadFinish;
	CButton m_cCheckUnLoadFinish;
	CButton m_cCheckReconnectFinish;
	afx_msg void OnBnClickedBtnMainJigUp();
	afx_msg void OnBnClickedBtnMainJigDown();
	afx_msg void OnBnClickedCheckGrabPower();
	afx_msg void OnBnClickedBtnResetIfpack();
	afx_msg void OnBnClickedBtnResetGender();
	afx_msg void OnBnClickedCheckHdmiUsbCyl();
	afx_msg void OnBnClickedCheckEarphone();
	afx_msg void OnBnClickedCheckComp();
	afx_msg void OnBnClickedCheckRelay1();
	CButton m_ctrlRelay1Chk;
	BOOL m_bRelay1;
	CButton m_ctrlGrabPower;
	BOOL m_bGrabPower;
	CButton m_cCheckHdmiUsbCyl;
	BOOL m_bCheckHdmiUsbCyl;
	CButton m_cCheckEarphone;
	CButton m_cCheckComp;
	CButton m_cCheck_PLC_Auto_Mode;
	CButton m_cCheck_PLC_Manual_Mode;
	CButton m_cCheck_PLC_Start_Run;
	CButton m_cCheck_PLC_Pause;
	CButton m_cCheck_PLC_STOP;
	CButton m_cCheck_PLC_Warn;
	CButton m_cCheck_PLC_Alarm;
	CButton m_cCheck_PLC_Data_PCB_Inside;
	CButton m_cCheck_PLC_Data_DummyCut;
	CButton m_cCheck_PLC_Data_OK;
	CButton m_cCheck_PLC_Data_NG;
	afx_msg void OnBnClickedBtnCommandLoadFast();
	afx_msg void OnBnClickedBtnCommandPassUnloadFast();
	afx_msg void OnBnClickedBtnCommandNgLoadFast();

	CButton m_cCheckSkipFFC;
	CButton m_cCheckPCBOnLoadSensor;
	afx_msg void OnBnClickedBtnCommandSkipffc();
	afx_msg void OnBnClickedBtnCommandClearCommand();
	CButton m_cCheckRobotMoving;
	CButton m_cCheckJigUpStatus;
	CButton m_cCheckJigDownStatus;
	CButton m_cCheckSetAlarm;
	afx_msg void OnBnClickedBtnCommandAlarm();
};
