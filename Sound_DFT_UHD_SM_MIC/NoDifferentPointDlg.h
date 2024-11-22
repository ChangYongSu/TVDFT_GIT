//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_NODIFFERENTPOINTDLG_H__D311E430_0509_4C08_9CDC_2CAA6D5121F6__INCLUDED_)
#define AFX_NODIFFERENTPOINTDLG_H__D311E430_0509_4C08_9CDC_2CAA6D5121F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoDifferentPointDlg.h : header file
//
#include "ReviewPage.h"

//class CReviewPage;

/////////////////////////////////////////////////////////////////////////////
// CNoDifferentPointDlg dialog

class CNoDifferentPointDlg : public CDialog
{
// Construction
public:
	CNoDifferentPointDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNoDifferentPointDlg)
	enum { IDD = IDD_NO_DIFFERENT_POINT_DLG };
	CComboBox	m_ctrlCmbStep;
	CVSFlexGrid	m_ctrlGrid;
	//}}AFX_DATA

public:
	void SetParentWnd(CReviewPage* pWnd);
	void InitGrid();
	void UpdateGrid();

	//+add kwmoon 070807
	int m_aNoDifferentPoint[3][NO_DIFFERENT_POINT_BLOCK];
	
	CReviewPage* m_pParentWnd;
	
	CRect m_dlgRect;

//	int m_nNoColorBit;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoDifferentPointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNoDifferentPointDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSelchangeComboStep();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODIFFERENTPOINTDLG_H__D311E430_0509_4C08_9CDC_2CAA6D5121F6__INCLUDED_)
