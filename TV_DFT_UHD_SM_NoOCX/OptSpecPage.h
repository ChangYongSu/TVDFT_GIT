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

	BOOL CheckToolCrc();
	void	InitSpecGrid();
	void	InitSpecGrid2();

// Dialog Data
	//{{AFX_DATA(COptSpecPage)
	enum { IDD = IDD_OPT_SPEC_PAGE };
	CString	m_szToolOption1;
	CString	m_szToolOption2;
	CString	m_szToolOption3;
	CString	m_szToolOption4;
	CString	m_szToolOption5;
	CString	m_szToolOption6;
	CString	m_szToolOption7;
	CString	m_szCommercialOption1;
	CString	m_szToolOption8;
	CString	m_szAreaOption1;
	CString	m_szCountryGroup;
	CString	m_szCountry;
	CString	m_szCpuVersion;
	CString	m_szMicomVersion;
	CString	m_szMntVersion;
	int		m_nToolOptionMethod;
	CComboBox	m_ctrlOptionCheckDelay;
	CString	m_szToolCRC;
	//CVSFlexGrid	m_ctrlSpecGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptSpecPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

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


	//CVSFlexGrid m_ctrlSpecGrid2;
	afx_msg void OnEnChangeEditDrmKey();
//	afx_msg void //OnBnClickedBtnCheckDrm();
	afx_msg void OnBnClickedButtonCheckDrmKey();
	void ChangeEditSpecGrid();
	CListCtrl m_CtrlListSpecIndexOption;
	afx_msg void OnNMClickListSpecIndexOption(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListSpecIndexOption(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_CtrlListSpecGrid;
	afx_msg void OnNMClickListSpecGrid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListSpecGrid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonOpEpiCfgSw();
	CEdit m_CEdit_OP_EPI_CFG_SW;
	afx_msg void OnLvnItemchangedListSpecGrid(NMHDR *pNMHDR, LRESULT *pResult);
	
	BOOL m_bToolOptionWriteFlag;
	CString m_szBoardOption;
	CString m_szCommercialBoardOption;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTSPECPAGE_H__A108E4AB_62C9_47F4_AA02_4B989EEA9878__INCLUDED_)
