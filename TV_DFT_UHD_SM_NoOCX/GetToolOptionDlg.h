//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_GETTOOLOPTIONDLG_H__2A4B852D_7D3D_4CAF_82AD_31B279E04467__INCLUDED_)
#define AFX_GETTOOLOPTIONDLG_H__2A4B852D_7D3D_4CAF_82AD_31B279E04467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetToolOptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GetToolOptionDlg dialog

class GetToolOptionDlg : public CDialog
{
// Construction
public:
	GetToolOptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GetToolOptionDlg)
	enum { IDD = IDD_GET_TOOL_OPTION_DLG };
	CString	m_sModelSuffix;
	CString	m_sTempModelSuffix;
	//CVSFlexGrid	m_ctrlSpecGrid1;
	//CVSFlexGrid	m_ctrlOptionGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GetToolOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	BOOL	m_bActivate;
	BOOL	m_nRetryCount;

	LRESULT	GetToolOption(WPARAM wParam, LPARAM lParam);
	LRESULT	CloseToolOption(WPARAM wParam, LPARAM lParam);
	void	InitSpecGrid();
	void	InitIndexGrid();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GetToolOptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonOptGet();
	afx_msg void OnBnClickedButtonUpdate();
	CListCtrl m_CtrlListIndexOption;
	afx_msg void OnNMClickListIndexOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListIndexOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	CListCtrl m_ctrlOptionList;
	CListCtrl m_ctrlSpecList;
	afx_msg void OnNMCustomdrawListToolOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListSpecOption(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETTOOLOPTIONDLG_H__2A4B852D_7D3D_4CAF_82AD_31B279E04467__INCLUDED_)
