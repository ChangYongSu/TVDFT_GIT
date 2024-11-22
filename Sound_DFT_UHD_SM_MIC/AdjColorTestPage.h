#if !defined(AFX_ADJCOLORTESTPAGE_H__42AB15C8_2CC1_4399_82CC_5126467EBA57__INCLUDED_)
#define AFX_ADJCOLORTESTPAGE_H__42AB15C8_2CC1_4399_82CC_5126467EBA57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjColorTestPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjColorTestPage dialog

class CAdjColorTestPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAdjColorTestPage)

// Construction
public:
	CAdjColorTestPage();
	~CAdjColorTestPage();

// Dialog Data
	//{{AFX_DATA(CAdjColorTestPage)
	enum { IDD = IDD_PARM_ADJ_PAGE_COLOR_TEST };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAdjColorTestPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAdjColorTestPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJCOLORTESTPAGE_H__42AB15C8_2CC1_4399_82CC_5126467EBA57__INCLUDED_)
