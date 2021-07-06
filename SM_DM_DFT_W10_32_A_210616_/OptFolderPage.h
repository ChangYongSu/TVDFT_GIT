#if !defined(AFX_OPTFOLDERPAGE_H__CBD62FC8_BE16_442E_AB03_55E6821DFBEC__INCLUDED_)
#define AFX_OPTFOLDERPAGE_H__CBD62FC8_BE16_442E_AB03_55E6821DFBEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptFolderPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptFolderPage dialog

class COptFolderPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptFolderPage)

// Construction
public:
	COptFolderPage();
	~COptFolderPage();

// Dialog Data
	//{{AFX_DATA(COptFolderPage)
	enum { IDD = IDD_OPT_FOLDER_PAGE };
	CString	m_szPatternFilePath;
	CString	m_szRemoteFilePath;
	CString	m_szTemplateSeqPath;
	CString	m_szTestParamPath;
	CString	m_szModelListPath;
	CString	m_strServerFolder;
	CString	m_strRefImageFolder;
	CString	m_strModelInfoFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptFolderPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptFolderPage)
	afx_msg void OnBtnOpenPattern();
	afx_msg void OnBtnOpenRemote();
	afx_msg void OnBtnOpenTestParam();
	afx_msg void OnBtnOpenTemplateSeq();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnOpenModelList();
	afx_msg void OnBtnOpenServerFolder();
	afx_msg void OnBtnOpenRefFolder();
	afx_msg void OnBtnOpenModelinfoFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTFOLDERPAGE_H__CBD62FC8_BE16_442E_AB03_55E6821DFBEC__INCLUDED_)
