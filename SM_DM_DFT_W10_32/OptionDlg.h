#if !defined(AFX_OPTIONDLG_H__1F236338_C1BB_44E8_AE54_14B0B3A5DBE7__INCLUDED_)
#define AFX_OPTIONDLG_H__1F236338_C1BB_44E8_AE54_14B0B3A5DBE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDlg.h : header file
//
//////////////////////////////////////////////////////////////////////////
//#include "CommPage.h"
#include "OptComportPage.h"
#include "OptEtcPage.h"
#include "OptGrabberPage.h"
#include "OptMesPage.h"
#include "OptUserInfoPage.h"

//+add kwmoon 080724
#include "OptSpecPage.h"

//+add kwmoon 081024
#include "OptFolderPage.h"

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COptionDlg

class COptionDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionDlg)

// Construction
public:
	COptionDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
//+del 090817
//	CFont				m_editBoxFont;
//	CFont*				m_pOldFont;

//	CCommPage			m_CommPage;
//-
	COptComportPage		m_ComPortPage;
	COptEtcPage			m_EtcPage;
	COptGrabberPage		m_GrabberPage;
	COptMesPage			m_MesPage;
	COptUserInfoPage	m_UserInfoPage;

	//+add kwmoon 080724
	COptSpecPage		m_SpecPage;

	//+add kwmoon 081024
	COptFolderPage		m_FolderPage;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__1F236338_C1BB_44E8_AE54_14B0B3A5DBE7__INCLUDED_)
