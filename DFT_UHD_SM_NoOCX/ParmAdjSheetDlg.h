#if !defined(AFX_PARMADJSHEETDLG_H__4B34EBAA_2A65_4629_9A19_600CFF764B64__INCLUDED_)
#define AFX_PARMADJSHEETDLG_H__4B34EBAA_2A65_4629_9A19_600CFF764B64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParmAdjSheetDlg.h : header file
//

#include "AdjColorTestPage.h"
#include "AdjPatternMatchTestPage.h"
#include "AdjSignalLineTestPage.h"

/////////////////////////////////////////////////////////////////////////////
// CParmAdjSheetDlg

class CParmAdjSheetDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CParmAdjSheetDlg)

// Construction
public:
	CParmAdjSheetDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CParmAdjSheetDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CAdjColorTestPage			m_AdjColorTestPage;
	CAdjPatternMatchTestPage	m_AdjPatternMatchTestPage;
	CAdjSignalLineTestPage		m_AdjSignalLineTestPage;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParmAdjSheetDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CParmAdjSheetDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CParmAdjSheetDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMADJSHEETDLG_H__4B34EBAA_2A65_4629_9A19_600CFF764B64__INCLUDED_)
