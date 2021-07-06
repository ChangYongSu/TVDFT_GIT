//{{AFX_INCLUDES()
#include "labelcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_USERMSGDLG_H__DAF958DE_D05B_4E16_82CE_FEF912427E0A__INCLUDED_)
#define AFX_USERMSGDLG_H__DAF958DE_D05B_4E16_82CE_FEF912427E0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserMsgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserMsgDlg dialog

class CUserMsgDlg : public CDialog
{
// Construction
public:
	CUserMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserMsgDlg)
	enum { IDD = IDD_USER_MSG_DLG };
//	CLabelControl	m_ctrlMsg1;
//	CLabelControl	m_ctrlMsg2;
//	BOOL		m_bThredStop;
	//}}AFX_DATA

public:
	CRect m_dlgRect;

	CString m_szMsg1;
	CString m_szMsg2;

	//+add kwmoon 080508
	BOOL	m_bYes;

	void ShowMessage2(CString szMsg);
	void ShowMessage1(CString szMsg);

	void SetMessage(CString szMsg1, CString szMsg2);
	
	//+ add 090902
	BOOL		m_bActivate;
	//-

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserMsgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnYes();
	afx_msg void OnBtnNo();
	afx_msg LRESULT OnProcessMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERMSGDLG_H__DAF958DE_D05B_4E16_82CE_FEF912427E0A__INCLUDED_)
