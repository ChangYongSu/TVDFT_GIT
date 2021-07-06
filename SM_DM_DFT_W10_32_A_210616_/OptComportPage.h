#if !defined(AFX_OPTCOMPORTPAGE_H__8B1F77F7_19D6_4615_922C_32F27BD54066__INCLUDED_)
#define AFX_OPTCOMPORTPAGE_H__8B1F77F7_19D6_4615_922C_32F27BD54066__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptComportPage.h : header file
//


#define PATTERN_GENERATOR_COM_TEST			0
#define REMOTE_CONTROLLER_COM_TEST			1
#define TV_SERVICE_COM_TEST					2
#define AV_SWITCHBOX_COM_TEST				3
#define KEYINPUTBOX_COM_TEST				4

/////////////////////////////////////////////////////////////////////////////
// COptComportPage dialog

class COptComportPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptComportPage)

// Construction
public:
	COptComportPage();
	~COptComportPage();

// Dialog Data
	//{{AFX_DATA(COptComportPage)
	enum { IDD = IDD_OPT_COMPORT_PAGE };
	CComboBox	m_ctrlProcessTypeCmb;
	CComboBox	m_ctrlChipTypeCmb1;
	CComboBox	m_ctrlChipTypeCmb2;
	CButton	m_ctrlUseThChk;
	CComboBox	m_ctrlThComportCmb;
	CComboBox	m_ctrlThBaudCmb;
	CComboBox	m_ctrlMdmiComportCmb;
	CComboBox	m_ctrlHdmiBaudCmb;
	CButton	m_ctrlUseHdmiGenChk;
	CComboBox	m_ctrlIrChkComportCmb;
	CComboBox	m_ctrlIrChkBaudCmb;
	CButton	m_ctrlUseIrChkChk;
	CComboBox	m_ctrlScannerComportCmb;
	CComboBox	m_ctrlScannerBaudCmb;
	CButton	m_ctrlUseScannerChk;
	//CButton	m_ctrlUseTVCommPortChk;
	CButton	m_ctrlUsePatternGenChk;
	CButton	m_ctrlUseAVSwitchBoxChk;
	CComboBox	m_ctrlSwitchBoxBaudCmb;
	CComboBox	m_ctrlAvSwitchBoxComportCmb;
	CComboBox	m_ctrlTVComportCmb;
	//CComboBox	m_ctrlTVBaudCmb;
	CComboBox	m_ctrlTVBaudCmb1;
	CComboBox	m_ctrlTVBaudCmb2;
	CComboBox	m_ctrlPatternComportCmb;
	CComboBox	m_ctrlPatternBaudCmb;
	CString	m_szAvSwitchBoxSettingValueTitle;
	int		m_ctlTVControlMode;
	BOOL	m_bAutoSourceControl;
	BOOL	m_bAutoPatternControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptComportPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptComportPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnComportOptApply();
	afx_msg void OnChkUseAvswitchbox();
	afx_msg void OnChkUsePatterngen();
	afx_msg void OnChkUseTvcommport();
	afx_msg void OnChkUseScanner();
	afx_msg void OnBtnInitJoystick();
	afx_msg void OnChkUseIrChk();
	afx_msg void OnBtnSave();
	afx_msg void OnChkUseHdmigen();
	afx_msg void OnChkUseTh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//+ 2007.5.2 Add BY USY
public:
	//Variable
	BOOL		m_bConnect;	//TVComm Connection

	

	//Function
	void		InitComPort(CString sComPort, DWORD wBaudRate, int nComportCmbID, int nBaudRateCmbID);
	CString		GetComPortVal(int nComportCmbID);
	DWORD		GetBaudRateVal(int nBaudRateCmbID);

	void		SetPatternGenGroup(BOOL bEnable);
	void		SetTVCommPortGroup(BOOL bEnable);
	void		SetAVSwitchBoxGroup(BOOL bEnable);
	void		SetScannerGroup(BOOL bEnable);
	void		SetHDMIGenGroup(BOOL bEnable);
// add 20100622
	void		SetThGroup(BOOL bEnable);

	//+add kwmoon 080625
	void		SetIrChkGroup(BOOL bEnable);
	
//////////////////////////////////////////////////////////////////////////

		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTCOMPORTPAGE_H__8B1F77F7_19D6_4615_922C_32F27BD54066__INCLUDED_)
