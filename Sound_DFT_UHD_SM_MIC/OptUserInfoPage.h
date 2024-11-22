#if !defined(AFX_OPTUSERINFOPAGE_H__F44E42A6_9449_4183_BEF6_94557BEEE146__INCLUDED_)
#define AFX_OPTUSERINFOPAGE_H__F44E42A6_9449_4183_BEF6_94557BEEE146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptUserInfoPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptUserInfoPage dialog

class COptUserInfoPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptUserInfoPage)

// Construction
public:
	COptUserInfoPage();
	~COptUserInfoPage();

// Dialog Data
	//{{AFX_DATA(COptUserInfoPage)
	enum { IDD = IDD_OPT_USERINFO_PAGE };
	CEdit	m_ctrlNewPasswordEdit;
	CEdit	m_ctrlCurrentPasswordEdit;
	CEdit	m_ctrlConfirmPasswordEdit;
	CComboBox	m_ctrlSystemNoCmb;
//	CComboBox	m_ctrlLocationCmb;
	CComboBox	m_ctrlLineNoCmb;
	CComboBox	m_ctrlAuthorityCmb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptUserInfoPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptUserInfoPage)
	afx_msg void OnBtnChgPassword();
	afx_msg void OnBtnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbAuthority();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
public:
	//Variable

	CStringArray    m_saLineList;

	//Fuction
	void			InitControl();
	CString			MakeUserID();
	BOOL			CheckPassword();
	BOOL    LineList_Read();
/////////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTUSERINFOPAGE_H__F44E42A6_9449_4183_BEF6_94557BEEE146__INCLUDED_)
