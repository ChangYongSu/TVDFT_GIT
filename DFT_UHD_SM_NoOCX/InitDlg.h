#if !defined(AFX_INITDLG_H__C1D2841A_C2BA_4D8A_B293_CD39F0AEA229__INCLUDED_)
#define AFX_INITDLG_H__C1D2841A_C2BA_4D8A_B293_CD39F0AEA229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InitDlg.h : header file
//
#include "resource.h"
#include "ProgressCtrlX.h"

class CDATsysView;
/////////////////////////////////////////////////////////////////////////////
// CInitDlg dialog

class CInitDlg : public CDialog
{
// Construction
public:
	CInitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInitDlg)
	enum { IDD = IDD_INIT_DIALOG };
	CProgressCtrlX	m_ctrlProgress;
	CListBox	m_ctrlMsgList;
	CEdit	m_ctrlPasswordEdit;
	CComboBox	m_ctrlSystemNoCmb;
	CComboBox	m_ctrlLineNoCmb;
	CComboBox	m_ctrlAuthorityCmb;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInitDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnExit();
	afx_msg void OnSelchangeCmbAuthority();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	CRect			m_dlgRect;
	CDATsysView*	m_MWnd;
	CStringArray    m_saLineList;
	
	//+add 090220(Modification No4)
	BOOL			m_bInitComplete;

	//Fuction
	void	InitControl();
	void	AddString2List(CString strString);
	void	UpdateString2List(CString strString);

	//+del kwmoon 080523
//	CString MakeUserID();
	BOOL	CheckPassword();
	BOOL    LineList_Read();



/////////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITDLG_H__C1D2841A_C2BA_4D8A_B293_CD39F0AEA229__INCLUDED_)
