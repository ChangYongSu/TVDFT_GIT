//{{AFX_INCLUDES()
#include "labelcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_PROGRESSMSGDLG_H__292FE6B2_9801_4A8D_8434_3A357608A2D1__INCLUDED_)
#define AFX_PROGRESSMSGDLG_H__292FE6B2_9801_4A8D_8434_3A357608A2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressMsgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressMsgDlg dialog

class CProgressMsgDlg : public CDialog
{
// Construction
public:
	void ShowMessage2(CString szMsg);
	void ShowMessage1(CString szMsg);
	CProgressMsgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressMsgDlg)
	enum { IDD = IDD_PROGRESS_MSG_DLG };
	CProgressCtrl	m_ctrlProgress;
	CLabelControl	m_ctrlMsg1;
	CLabelControl	m_ctrlMsg2;
	//}}AFX_DATA

public:

	void SetProgressPos(int nPos);
	void SetMessage(CString szMsg1, CString szMsh2);

	CString m_szMsg1;
	CString m_szMsg2;

	CRect m_dlgRect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressMsgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSMSGDLG_H__292FE6B2_9801_4A8D_8434_3A357608A2D1__INCLUDED_)
