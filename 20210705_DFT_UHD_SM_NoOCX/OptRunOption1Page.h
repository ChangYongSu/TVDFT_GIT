#if !defined(AFX_OPTRUNOPTION1PAGE_H__9418295F_1EEB_474F_B01E_E47E3180712A__INCLUDED_)
#define AFX_OPTRUNOPTION1PAGE_H__9418295F_1EEB_474F_B01E_E47E3180712A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptRunOption1Page.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptRunOption1Page

class COptRunOption1Page : public CPropertySheet
{
	DECLARE_DYNAMIC(COptRunOption1Page)

// Construction
public:
	COptRunOption1Page(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptRunOption1Page(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptRunOption1Page)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptRunOption1Page();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptRunOption1Page)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTRUNOPTION1PAGE_H__9418295F_1EEB_474F_B01E_E47E3180712A__INCLUDED_)
