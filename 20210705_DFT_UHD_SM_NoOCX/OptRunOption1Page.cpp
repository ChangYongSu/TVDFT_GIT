// OptRunOption1Page.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptRunOption1Page.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptRunOption1Page

IMPLEMENT_DYNAMIC(COptRunOption1Page, CPropertySheet)

COptRunOption1Page::COptRunOption1Page(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

COptRunOption1Page::COptRunOption1Page(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

COptRunOption1Page::~COptRunOption1Page()
{
}


BEGIN_MESSAGE_MAP(COptRunOption1Page, CPropertySheet)
	//{{AFX_MSG_MAP(COptRunOption1Page)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptRunOption1Page message handlers
