#if !defined(AFX_ADDMODELDLG_H__41098862_26BE_41B0_ADEC_C6CF225246FF__INCLUDED_)
#define AFX_ADDMODELDLG_H__41098862_26BE_41B0_ADEC_C6CF225246FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddModelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddModelDlg dialog

class CAddModelDlg : public CDialog
{
// Construction
public:
	CAddModelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddModelDlg)
	enum { IDD = IDD_ADD_MODEL_DLG };
	CString	m_szChassisName;
	//CString	m_szModelInfoPath;
	CString	m_szModelName;
	//CString	m_szRefFolderPath;
	CString	m_szSeqPath;

	
//	CString	m_szSeqPath;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddModelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	int m_EditMode;
	CRect m_ParentWndRect;
	CString m_sWinText;
	int      m_nDivision;
	CString  m_sDivision;

	CString	m_szModelInfoPath;
	CString	m_szVFConfigPath;
	CString	m_szRefFolderPath;
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddModelDlg)
	afx_msg void OnBtnSeq();
	afx_msg void OnBtnModelInfo();
	afx_msg void OnBtnRefFolder();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cComboDivision;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDMODELDLG_H__41098862_26BE_41B0_ADEC_C6CF225246FF__INCLUDED_)
