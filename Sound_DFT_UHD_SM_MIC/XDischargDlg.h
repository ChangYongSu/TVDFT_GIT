//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_XDISCHARGDLG_H__5D88ADB3_3AF3_11D5_8D8E_006008A2DB00__INCLUDED_)
#define AFX_XDISCHARGDLG_H__5D88ADB3_3AF3_11D5_8D8E_006008A2DB00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XDischargDlg.h : header file
//
#include "Digistring.h"

/////////////////////////////////////////////////////////////////////////////
// CXDischargDlg dialog

class CXDischargDlg : public CDialog
{
// Construction
public:
	CXDischargDlg(CWnd* pParent = NULL);   // standard constructor

	void GetVolt();

	BOOL	m_bReadFlag;
	double	m_dVolt1;
	double	m_dVolt2;
	double	m_dVolt3;
	double	m_dMeasure;
	double	m_dLimit;

	CWinThread*		m_pMeasureThread;

// Dialog Data
	//{{AFX_DATA(CXDischargDlg)
	enum { IDD = IDD_X_DISCHARG_DLG };
	CDigistring	m_ctlLabel_XVa;
	CDigistring	m_ctlLabel_X5V;
	CDigistring	m_ctlLabel_X15V;
	BOOL			m_bThredStop;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXDischargDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXDischargDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XDISCHARGDLG_H__5D88ADB3_3AF3_11D5_8D8E_006008A2DB00__INCLUDED_)
