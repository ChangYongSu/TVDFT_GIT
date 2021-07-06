#if !defined(AFX_SEQREMOTEDLG_H__28B5BA44_76C1_4BBD_823C_374458B2D8C4__INCLUDED_)
#define AFX_SEQREMOTEDLG_H__28B5BA44_76C1_4BBD_823C_374458B2D8C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeqRemoteDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
#include "Global.h"

#define UM_CALC_REMOTE_WINDOW				WM_USER + 0x320
/////////////////////////////////////////////////////////////////////////////
// CSeqRemoteDlg dialog

class CSeqRemoteDlg : public CDialog
{
// Construction
public:
	CSeqRemoteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSeqRemoteDlg)
	enum { IDD = IDD_SEQ_REMOTE_DLG };
	CComboBox	m_ctrlRemoteCodeCmb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeqRemoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeqRemoteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRemoteButtonClick(UINT nButtonID);
	afx_msg void OnSelchangeCmbRemoteCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	CString m_strKeyCode;

	//Function
//	BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCode, CString& strDesc);
//	BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCode, CString& strDesc, int& nByte);
	BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCustomCode, CString& strCode, CString& strDesc, int& nByte);
	void InitRemoteCodeCmb();
/////////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEQREMOTEDLG_H__28B5BA44_76C1_4BBD_823C_374458B2D8C4__INCLUDED_)
