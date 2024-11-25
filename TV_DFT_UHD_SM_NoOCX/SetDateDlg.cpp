// SetDateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SetDateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDateDlg dialog


CSetDateDlg::CSetDateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetDateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDateDlg)
	DDX_Control(pDX, IDC_MONTHCALENDAR, m_ctrlCalendar);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDateDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDateDlg message handlers

BOOL CSetDateDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

BOOL CSetDateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CTime time;
	time = CTime::GetCurrentTime();

	m_ctrlCalendar.SetToday(&time);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetDateDlg::OnOK() 
{
	// TODO: Add extra validation here

    m_ctrlCalendar.GetCurSel(&m_systemTime);

	CDialog::OnOK();
}
