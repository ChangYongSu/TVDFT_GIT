//{{AFX_INCLUDES()
#include "labelcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_RESULTDLG_H__C0A9596F_0266_4480_91E7_05929552B8F0__INCLUDED_)
#define AFX_RESULTDLG_H__C0A9596F_0266_4480_91E7_05929552B8F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultDlg.h : header file
//
#include "ColorStaticST.h"	
/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog

class CResultDlg : public CDialog
{
// Construction
public:
	CResultDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CResultDlg)
	enum { IDD = IDD_RESULT_DLG };
	CStatic	m_ctrlResultImage;
	//CLabelControl	m_ctrlTestResultInfo1;
	//CLabelControl	m_ctrlTestResultInfo2;
	//CLabelControl	m_ctrlTestResultInfo3;
	//}}AFX_DATA

	CBitmap	m_pBitmap;

	CString m_szMsg1;
	CString m_szMsg2;


	//+add kwmoon 071121
	int m_nImageID;

	//+ 2007.11.5 Add BY USY
	BOOL	m_bActivate;
	//-

	//+add kwmoon 071210
	BOOL	m_bContinueTest;


//	CColorStaticST m_stcStatus;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public :
	void SetTestResultInfo(CString szResultMsg1,CString szResultMsg2);
	void SetImage(int nImageID);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CResultDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnContinue();
	afx_msg void OnBtnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTDLG_H__C0A9596F_0266_4480_91E7_05929552B8F0__INCLUDED_)
