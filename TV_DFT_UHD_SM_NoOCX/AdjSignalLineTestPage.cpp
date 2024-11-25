// AdjSignalLineTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "AdjSignalLineTestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdjSignalLineTestPage property page


IMPLEMENT_DYNCREATE(CAdjSignalLineTestPage, CPropertyPage)

CAdjSignalLineTestPage::CAdjSignalLineTestPage() : CPropertyPage(CAdjSignalLineTestPage::IDD)
{
	//{{AFX_DATA_INIT(CAdjSignalLineTestPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAdjSignalLineTestPage::~CAdjSignalLineTestPage()
{
}

void CAdjSignalLineTestPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdjSignalLineTestPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdjSignalLineTestPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAdjSignalLineTestPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAdjSignalLineTestPage message handlers
