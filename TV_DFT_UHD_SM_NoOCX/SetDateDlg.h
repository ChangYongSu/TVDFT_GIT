#if !defined(AFX_SETDATEDLG_H__79F8E974_8598_4B0B_86BE_81EBF0417273__INCLUDED_)
#define AFX_SETDATEDLG_H__79F8E974_8598_4B0B_86BE_81EBF0417273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetDateDlg dialog

class CSetDateDlg : public CDialog
{
// Construction
public:
	CSetDateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDateDlg)
	enum { IDD = IDD_SET_DATE_DLG };
	CMonthCalCtrl	m_ctrlCalendar;
	//}}AFX_DATA

public:
	SYSTEMTIME m_systemTime;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDateDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDATEDLG_H__79F8E974_8598_4B0B_86BE_81EBF0417273__INCLUDED_)
