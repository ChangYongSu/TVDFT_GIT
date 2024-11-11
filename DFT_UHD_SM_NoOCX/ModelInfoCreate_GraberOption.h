#if !defined(AFX_MODELINFOCREATE_GRABEROPTION_H__9CDD4521_1064_4D75_9F21_5764B38E31FF__INCLUDED_)
#define AFX_MODELINFOCREATE_GRABEROPTION_H__9CDD4521_1064_4D75_9F21_5764B38E31FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelInfoCreate_GraberOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate_GraberOption dialog

class CModelInfoCreate_GraberOption : public CDialog
{
// Construction
public:
	CModelInfoCreate_GraberOption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelInfoCreate_GraberOption)
	enum { IDD = IDD_MODELINFO_OPTION_DLG };
	CComboBox	m_ctrlLvdsMode;
	CComboBox	m_ctrlHdmi4Model;
	CComboBox	m_ctrlHdmi3Model;
	CComboBox	m_ctrlHdmi2Model;
	CComboBox	m_ctrlHdmi1Model;
	CComboBox	m_ctrlHDMI_BitSpec;
	CComboBox	m_ctrlHdmiBitShift;
	CComboBox	m_ctrlHdmiResolution;
	CComboBox	m_ctrlTVBaudCmb;
	CComboBox	m_ctrlUhdType;
	CComboBox	m_ctrlGrabMode;
	CComboBox	m_ctrlBitShift;
	int		m_nUranousMode;
	int		m_nLvdsFormat;
	int		m_nAnalogFormat;
	int		m_nBitMode;
	int		m_ctlTVControlMode;
	BOOL	m_bAutoSourceControl;
	BOOL	m_bAutoPatternControl;
	BOOL	m_bImage_Rotation;
	BOOL	m_bOddDE_Only;
	BOOL	m_bAutoAVSwitchBoxControl;
	int		m_n3DMode;
	int		m_nHDMI_Resolution;
	int		m_nCompResolution;

	///m_bScanNotUse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelInfoCreate_GraberOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetShowFHD_Item(BOOL bFlage);
	void SetShowUHD_Item(BOOL bFlage);

	// Generated message map functions
	//{{AFX_MSG(CModelInfoCreate_GraberOption)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHdmiResolution1();
	afx_msg void OnHdmiResolution2();
	afx_msg void OnHdmiResolution3();
	afx_msg void OnHdmiResolution4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bScanNotUse;
	CComboBox m_nARC_Force_Port_Number;
	BOOL m_bRedBlueSwap;
	BOOL m_bDeHsyncEnable;
	BOOL m_bHyncEdgeEnable;
	CComboBox m_cComboLvdsFormat;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELINFOCREATE_GRABEROPTION_H__9CDD4521_1064_4D75_9F21_5764B38E31FF__INCLUDED_)
