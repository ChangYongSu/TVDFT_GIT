#if !defined(AFX_OPTMESPAGE_H__7A8898DF_650A_4136_8092_99A777FBF5D1__INCLUDED_)
#define AFX_OPTMESPAGE_H__7A8898DF_650A_4136_8092_99A777FBF5D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptMesPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "MesDb.h"
#include "Global.h"
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// COptMesPage dialog

class COptMesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptMesPage)

// Construction
public:
	COptMesPage();
	~COptMesPage();

// Dialog Data
	//{{AFX_DATA(COptMesPage)
	enum { IDD = IDD_OPT_MES_PAGE };
	CEdit	m_ctrlHostIP;
	CEdit	m_ctrlHostPort;
	CButton	m_ctrlUploadMesChk;
	CButton	m_ctrlAutoUploadMesChk;
	int		m_nModelInfo_Check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptMesPage)
	public:
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptMesPage)
	afx_msg void OnBtnMesConnectTest();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	CString	m_strGmesIP;				//GMES IP Add
	UINT	m_nGmesPort;			//GMES Port

	//Function


/////////////////////////////////////////////////////////////////////////////


	CButton m_ctrlAutoGMES_ChangeModel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTMESPAGE_H__7A8898DF_650A_4136_8092_99A777FBF5D1__INCLUDED_)
