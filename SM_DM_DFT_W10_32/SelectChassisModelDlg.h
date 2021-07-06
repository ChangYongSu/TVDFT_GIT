//{{AFX_INCLUDES()
//#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_SELECTCHASSISMODELDLG_H__B3DB42EE_231D_4744_AAA5_151C6E741331__INCLUDED_)
#define AFX_SELECTCHASSISMODELDLG_H__B3DB42EE_231D_4744_AAA5_151C6E741331__INCLUDED_

#include "ListCtrlEx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectChassisModelDlg.h : header file
//
#define CHASSIS_GRID	0
#define MODEL_GRID		1

/////////////////////////////////////////////////////////////////////////////
// CSelectChassisModelDlg dialog

class CSelectChassisModelDlg : public CDialog
{
// Construction
public:
	CSelectChassisModelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectChassisModelDlg)
	enum { IDD = IDD_SELECT_CHASSIS_MODEL_DLG };
	CComboBox	m_ctrlDivisionCmb;
//	CVSFlexGrid	m_gridChassis;
//	CVSFlexGrid	m_gridModel;
	CString	m_szCurrentSeqFilePath;
	CString	m_szModelInfoFilePath;
	CString	m_szRefImgFolder;
	CString	m_szSeqFilePath;
	CString	m_szCurrentRefFilePath;
	//}}AFX_DATA

	int		m_nNoModel;
	CString m_sSelChassis;
	CString m_sSelModel;
	int		m_nChaassis_SelectRow;
	int		m_nModel_SelectRow;
	int		m_nDivision_Sel;

	void	InitGrid();
	void	UpdateGrid();
	BOOL	IsSameChassis(CString szChassisName);
	void	ChangeRowColor(int nRow, int nGridType);
	BOOL	CheckModel(CString sChassis, CString sModel);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectChassisModelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectChassisModelDlg)
	 void OnClickChassisGrid();
	 void OnClickModelGrid();

	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	virtual void OnOK();
	afx_msg void OnBtnEdit();
	afx_msg void OnBtnCopy();
	afx_msg void OnSelchangeCombo1();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListChassis;
	CListCtrl m_cListModel;

	CListCtrlEx		m_cListChassisEx;
	CListCtrlEx		m_cListModelEx;

	afx_msg void OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMRClickListChassis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListChassis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListChassis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListModel(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCHASSISMODELDLG_H__B3DB42EE_231D_4744_AAA5_151C6E741331__INCLUDED_)
