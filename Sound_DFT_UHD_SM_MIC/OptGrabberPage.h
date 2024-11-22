#if !defined(AFX_OPTGRABBERPAGE_H__B252E04A_E75C_4741_80ED_744076FEB391__INCLUDED_)
#define AFX_OPTGRABBERPAGE_H__B252E04A_E75C_4741_80ED_744076FEB391__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptGrabberPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptGrabberPage dialog


class COptGrabberPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptGrabberPage)

// Construction
public:
	COptGrabberPage();
	~COptGrabberPage();

// Dialog Data
	//{{AFX_DATA(COptGrabberPage)
	enum { IDD = IDD_OPT_GRABBER_PAGE };
	CComboBox	m_ctrlLvdsMode;
	CComboBox	m_ctrlHdmi3Model;
	CComboBox	m_ctrlHdmi2Model;
	CComboBox	m_ctrlHdmi4Model;
	CComboBox	m_ctrlHdmi1Model;
	CComboBox	m_ctrlGrabDelay;
	CComboBox	m_ctrlHdmiGrabResolution;
	CComboBox	m_ctrlHdmiBitShift;
	CComboBox	m_ctrlUhdType;
	CComboBox	m_ctrlGrabMode;
	CComboBox	m_ctrlBitShift;
	CComboBox	m_ctrlHdmi4Port;
	CComboBox	m_ctrlHdmi3Port;
	CComboBox	m_ctrlHdmi2Port;
	CComboBox	m_ctrlHdmi1Port;
	CComboBox	m_ctrlHDMI_BitSpec;
	CButton	m_ctrlOddDE_Only;
	CComboBox	m_ctrlPeriod;
	int		m_nAnalogFormat;
	int		m_nLvdsFormat;
	int		m_nUranousMode;
	int		m_nBitMode;
	int		m_nAudioMeasureMethod;
	int		m_nGrabCheckArea;
	BOOL	m_bAutoDataDelete;
	BOOL	m_bOddDE_Only;
	BOOL	m_bImage_Rotation;
	int		m_nHDMI_Resolution;
	int		m_n3DMode;
	int		m_nHdmiOutControl;
	int		m_nHdmiCecControl;
	int		m_nCompResolution;
	BOOL	m_bMaskOsd;
	//}}AFX_DATA

public:
	//+add kwmoon 080416
//	CProgressMsgDlg* m_pProgressMsgDlg;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptGrabberPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptGrabberPage)
	afx_msg void OnBtnGrabberOptApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGrabberOptDelete();
	afx_msg void OnBtnSave();
	afx_msg void OnSelchangeComboPeriod();
	afx_msg void OnHdmiResolution1();
	afx_msg void OnHdmiResolution2();
	afx_msg void OnHdmiResolution3();
	afx_msg void OnHdmiResolution4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
//+ 2007.5.2 Add BY USY
public:
	//Variable
	int m_nPeriod;
	//Function
	BOOL MakeOptionString(CString& szOptionData,UINT nNoDigit);
	BOOL DeleteFolder(CString szGrabFolderPath); 
	void MakeModelFolderList(CString szModuleFolder,CStringArray& aModelFoler);
	void InitItem(BOOL bFlag);


//////////////////////////////////////////////////////////////////////////

	BOOL m_bRedBlueSwap;
	afx_msg void OnBnClickedChkRbSwap();
	BOOL m_bDeHsyncEnable;
	BOOL m_bHyncEdgeEnable;
	CComboBox m_cComboY20SwMode;
	afx_msg void OnBnClickedButton170Write();
	afx_msg void OnBnClickedButton170Read();
//	BOOL m_bEpiPAckReset;
//	BOOL m_bGrabBaseReset;
	BOOL m_nHdmiEdidControl;
	BOOL m_nHdmiHdcpControl;
	afx_msg void OnCbnSelchangeComboGrabmode();
	afx_msg void OnCbnSelchangeComboUhdtype();
	CComboBox m_cComboLvdsFormat;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTGRABBERPAGE_H__B252E04A_E75C_4741_80ED_744076FEB391__INCLUDED_)
