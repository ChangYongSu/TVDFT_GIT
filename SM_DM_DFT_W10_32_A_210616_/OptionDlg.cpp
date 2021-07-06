// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg

IMPLEMENT_DYNAMIC(COptionDlg, CPropertySheet)

COptionDlg::COptionDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_GrabberPage);
	AddPage(&m_EtcPage);
	AddPage(&m_ComPortPage);
	AddPage(&m_SpecPage);
	
	AddPage(&m_MesPage);
	AddPage(&m_UserInfoPage);
	AddPage(&m_FolderPage);
}

COptionDlg::COptionDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_GrabberPage);
	AddPage(&m_EtcPage);
	AddPage(&m_ComPortPage);
	AddPage(&m_SpecPage);
	
	AddPage(&m_MesPage);
	AddPage(&m_UserInfoPage);

	AddPage(&m_FolderPage);
}

COptionDlg::~COptionDlg()
{
}


BEGIN_MESSAGE_MAP(COptionDlg, CPropertySheet)
	//{{AFX_MSG_MAP(COptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//del 090817
/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers
/*
int COptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Font
    CClientDC dc(this);  

	m_editBoxFont.CreateFont(22,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,
						DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
					    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
					    DEFAULT_PITCH,_T("Arial"));

    m_pOldFont = (CFont*) dc.SelectObject(&m_editBoxFont);

//	m_CommPage.m_pFont		= &m_editBoxFont;
	return 0;
}
*/