#if !defined(AFX_STOPPROCESSINGDLG_H__6B49BC3E_34DD_4A04_81A2_1116F5755FBB__INCLUDED_)
#define AFX_STOPPROCESSINGDLG_H__6B49BC3E_34DD_4A04_81A2_1116F5755FBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StopProcessingDlg.h : header file
//

#include "ProgressCtrlX.h"
#include "resource.h"
#include "labelcontrol.h"

/////////////////////////////////////////////////////////////////////////////
// CStopProcessingDlg dialog

class CStopProcessingDlg : public CDialog
{
// Construction
public:
	CStopProcessingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStopProcessingDlg)
	enum { IDD = IDD_STOP_PROCESSING_DLG };
	CProgressCtrlX	m_ctrlProgress;
	//CLabelControl	m_ctrlMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStopProcessingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	CRect m_dlgRect;

	BOOL m_bShowDlg;
	
	int  m_nProgress;

	void SetShowDlg(BOOL bShow);
	void SetMessage(int nMessageType);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStopProcessingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPPROCESSINGDLG_H__6B49BC3E_34DD_4A04_81A2_1116F5755FBB__INCLUDED_)
