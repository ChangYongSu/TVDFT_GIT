#if !defined(AFX_MODELINFOCREATE_H__B9B0B7EB_86D4_476E_9EE2_6834DFD0DB8D__INCLUDED_)
#define AFX_MODELINFOCREATE_H__B9B0B7EB_86D4_476E_9EE2_6834DFD0DB8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelInfoCreate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate dialog

class CModelInfoCreate : public CDialog
{
// Construction
public:
	CModelInfoCreate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelInfoCreate)
	enum { IDD = IDD_MODELINFO_CREATE };
	CString	m_szInfoFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelInfoCreate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	CString m_sModelInfo_FileName;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelInfoCreate)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELINFOCREATE_H__B9B0B7EB_86D4_476E_9EE2_6834DFD0DB8D__INCLUDED_)
