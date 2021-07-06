#if !defined(AFX_ADJPATTERNMATCHTESTPAGE_H__2B40FDEA_9310_410F_A716_A8CFDA69E639__INCLUDED_)
#define AFX_ADJPATTERNMATCHTESTPAGE_H__2B40FDEA_9310_410F_A716_A8CFDA69E639__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjPatternMatchTestPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjPatternMatchTestPage dialog

class CAdjPatternMatchTestPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAdjPatternMatchTestPage)

// Construction
public:
	CAdjPatternMatchTestPage();
	~CAdjPatternMatchTestPage();

// Dialog Data
	//{{AFX_DATA(CAdjPatternMatchTestPage)
	enum { IDD = IDD_PARM_ADJ_PAGE_PATTERN_MATCH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjPatternMatchTestPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjPatternMatchTestPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJPATTERNMATCHTESTPAGE_H__2B40FDEA_9310_410F_A716_A8CFDA69E639__INCLUDED_)
