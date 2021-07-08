// AdjColorTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "AdjColorTestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjColorTestPage property page

IMPLEMENT_DYNCREATE(CAdjColorTestPage, CPropertyPage)

CAdjColorTestPage::CAdjColorTestPage() : CPropertyPage(CAdjColorTestPage::IDD)
{
	//{{AFX_DATA_INIT(CAdjColorTestPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAdjColorTestPage::~CAdjColorTestPage()
{
}

void CAdjColorTestPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjColorTestPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjColorTestPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAdjColorTestPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CAdjColorTestPage message handlers
