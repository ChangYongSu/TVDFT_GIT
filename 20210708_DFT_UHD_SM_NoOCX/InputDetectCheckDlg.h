//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_INPUTDETECTCHECKDLG_H__2196D25D_B5B7_4989_B857_BA03F9A7ECA4__INCLUDED_)
#define AFX_INPUTDETECTCHECKDLG_H__2196D25D_B5B7_4989_B857_BA03F9A7ECA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputDetectCheckDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDetectCheckDlg dialog

class CInputDetectCheckDlg : public CDialog
{
// Construction
public:
	CInputDetectCheckDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDetectCheckDlg)
	enum { IDD = IDD_INPUT_DETECT_CHECK };
	CVSFlexGrid	m_ctrlInputCheckGrid;
	//}}AFX_DATA

//	CRect			m_dlgRect;
	BOOL		m_bActivate;
	BOOL		m_bInputCheckResult;
	BOOL		m_bTestRunning;
	BOOL m_bInputCheckWait;
	int  m_nInputCheckCount;

	BOOL		m_bInputDetectResultList[200];

	void InitInputCheckGrid();
	void UpdateResult(int nIndex, int nResult);
	BOOL InputCheck();
	void SaveResult2File();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDetectCheckDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDetectCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg long OnProcessMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnEnter();
	afx_msg void OnBtnRecheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnStop();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDETECTCHECKDLG_H__2196D25D_B5B7_4989_B857_BA03F9A7ECA4__INCLUDED_)
