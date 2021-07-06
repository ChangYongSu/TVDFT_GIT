//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_OPTSPECPAGE_H__A108E4AB_62C9_47F4_AA02_4B989EEA9878__INCLUDED_)
#define AFX_OPTSPECPAGE_H__A108E4AB_62C9_47F4_AA02_4B989EEA9878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptSpecPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptSpecPage dialog

class COptSpecPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptSpecPage)

// Construction
public:
	COptSpecPage();
	~COptSpecPage();

// Dialog Data
	//{{AFX_DATA(COptSpecPage)
	enum { IDD = IDD_OPT_SPEC_PAGE };
//	CVSFlexGrid	m_ctrlVerSpecGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptSpecPage)
	public:
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	int m_EditMode;
	int m_SelectItem;
	int m_SelectShell;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptSpecPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnOptApply();
	afx_msg void OnBtnSave();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	void InitVerSpecGrid();
	void UpdateVerSpecGrid();

	CListCtrl m_ctrlAudioVerSpecList;
	afx_msg void OnNMDblclkListAudioSpecList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListAudioSpecList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditOptVersion();
	CEdit m_cEditOptVersion;
	afx_msg void OnBnClickedButtonUpdateVersion();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTSPECPAGE_H__A108E4AB_62C9_47F4_AA02_4B989EEA9878__INCLUDED_)
