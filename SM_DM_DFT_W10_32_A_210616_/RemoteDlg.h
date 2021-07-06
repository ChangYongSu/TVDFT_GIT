#if !defined(AFX_REMOTEDLG_H__1E9D14E9_95E2_4E15_B3FF_D4471B3AC434__INCLUDED_)
#define AFX_REMOTEDLG_H__1E9D14E9_95E2_4E15_B3FF_D4471B3AC434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RemoteDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#include "Global.h"

#define UM_CALC_REMOTE_WINDOW				WM_USER + 0x320
/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg dialog
class CImageTotalDlg;


class CRemoteDlg : public CDialog
{
// Construction
public:
	CRemoteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRemoteDlg)
	enum { IDD = IDD_REMOTE_DLG };
	CComboBox	m_ctrlRemoteCodeCmb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRemoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRemoteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRemoteButtonClick(UINT nButtonID);
	afx_msg void OnBtnSendRemoteCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	CRect		m_dlgRect;
	CImageTotalDlg*	m_MWnd;
	int			m_nRmoteType;

	//Function
//	BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCode, CString& strDesc, int& nByte);
	BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCustomCode, CString& strCode, CString& strDesc, int& nByte);
	void InitRemoteCodeCmb();
/////////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REMOTEDLG_H__1E9D14E9_95E2_4E15_B3FF_D4471B3AC434__INCLUDED_)
