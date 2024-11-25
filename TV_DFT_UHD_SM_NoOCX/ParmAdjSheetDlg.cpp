// ParmAdjSheetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ParmAdjSheetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParmAdjSheetDlg

IMPLEMENT_DYNAMIC(CParmAdjSheetDlg, CPropertySheet)

CParmAdjSheetDlg::CParmAdjSheetDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_AdjColorTestPage);
	AddPage(&m_AdjPatternMatchTestPage);
	AddPage(&m_AdjSignalLineTestPage);
}

CParmAdjSheetDlg::CParmAdjSheetDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_AdjColorTestPage);
	AddPage(&m_AdjPatternMatchTestPage);
	AddPage(&m_AdjSignalLineTestPage);
}

CParmAdjSheetDlg::~CParmAdjSheetDlg()
{
}


BEGIN_MESSAGE_MAP(CParmAdjSheetDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CParmAdjSheetDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParmAdjSheetDlg message handlers
