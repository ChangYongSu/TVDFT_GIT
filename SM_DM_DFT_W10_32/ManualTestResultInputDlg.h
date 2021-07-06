//{{AFX_INCLUDES()
#include "labelcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANUALTESTRESULTINPUTDLG_H__679707E2_B3CB_4B3A_8441_784FC7CB4AA2__INCLUDED_)
#define AFX_MANUALTESTRESULTINPUTDLG_H__679707E2_B3CB_4B3A_8441_784FC7CB4AA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManualTestResultInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManualTestResultInputDlg dialog

class CManualTestResultInputDlg : public CDialog
{
// Construction
public:
	CManualTestResultInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManualTestResultInputDlg)
	enum { IDD = IDD_MANUAL_TEST_RESULT_INPUT_DLG };
	CLabelControl	m_ctrlInfoMsg1;
	CLabelControl	m_ctrlInfoMsg2;
	CLabelControl	m_ctrlTitle;
	//}}AFX_DATA

	BOOL m_bTestResult;

	CString m_szStepName; 
	CString m_szMsg1; 
	CString m_szMsg2;

	//+ 2007.11.5 Add BY USY
	BOOL		m_bActivate;
	//-

public:

	void ClearInfoMessage();
	void SetInfoMessage(CString szStepName, CString szMsg1, CString szMsg2); 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManualTestResultInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManualTestResultInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnProcessMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBtnPass();
	afx_msg void OnBtnNg();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANUALTESTRESULTINPUTDLG_H__679707E2_B3CB_4B3A_8441_784FC7CB4AA2__INCLUDED_)
