//{{AFX_INCLUDES()
#include "labelcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_STOPSTEPDLG_H__4DBCADAB_E0DC_4294_8CF2_797EEDB19EB5__INCLUDED_)
#define AFX_STOPSTEPDLG_H__4DBCADAB_E0DC_4294_8CF2_797EEDB19EB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StopStepDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStopStepDlg dialog

class CStopStepDlg : public CDialog
{
// Construction
public:
	CStopStepDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStopStepDlg)
	enum { IDD = IDD_STOP_STEP_DLG };
	//CLabelControl	m_ctrlTestResultInfo1;
	//CLabelControl	m_ctrlTestResultInfo2;
	//CLabelControl	m_ctrlTestResultInfo3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStopStepDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStopStepDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnProcessMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBtnStop2();
	afx_msg void OnBtnRetry2();
	afx_msg void OnBtnOk2();
	afx_msg void OnBtnNg2();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
public:
	//Variable

	UINT	m_nRunType;		

	//+ 2007.11.5 Add BY USY
	BOOL		m_bActivate;
	//-
	
	//+add kwmoon 080618
	CString m_szMsg1;
	CString m_szMsg2;
	CString m_szMsg3;

	//Function
	//+add kwmoon 080618
	void SetTestResultInfo(CString szResultMsg1,CString szResultMsg2,CString szResultMsg3);

/////////////////////////////////////////////////////////////////////////////

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPSTEPDLG_H__4DBCADAB_E0DC_4294_8CF2_797EEDB19EB5__INCLUDED_)
