#if !defined(AFX_ADJSIGNALLINETESTPAGE_H__0D3D0330_3DA6_46AF_922D_A982CB9B82CF__INCLUDED_)
#define AFX_ADJSIGNALLINETESTPAGE_H__0D3D0330_3DA6_46AF_922D_A982CB9B82CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdjSignalLineTestPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdjSignalLineTestPage dialog

class CAdjSignalLineTestPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAdjSignalLineTestPage)

// Construction
public:
	CAdjSignalLineTestPage();
	~CAdjSignalLineTestPage();

// Dialog Data
	//{{AFX_DATA(CAdjSignalLineTestPage)
	enum { IDD = IDD_PARM_ADJ_PAGE_SIGNAL_TEST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdjSignalLineTestPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdjSignalLineTestPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADJSIGNALLINETESTPAGE_H__0D3D0330_3DA6_46AF_922D_A982CB9B82CF__INCLUDED_)
