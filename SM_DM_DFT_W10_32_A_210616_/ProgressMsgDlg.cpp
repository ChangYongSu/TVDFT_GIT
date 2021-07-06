// ProgressMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ProgressMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressMsgDlg dialog


CProgressMsgDlg::CProgressMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressMsgDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProgressMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressMsgDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_LABEL_MESSAGE1, m_ctrlMsg1);
	DDX_Control(pDX, IDC_LABEL_MESSAGE2, m_ctrlMsg2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressMsgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressMsgDlg message handlers

void CProgressMsgDlg::ShowMessage1(CString szMsg)
{
	m_ctrlMsg1.SetWindowText(szMsg);
	m_ctrlMsg1.Invalidate();
	m_ctrlMsg1.UpdateWindow();
}

void CProgressMsgDlg::ShowMessage2(CString szMsg)
{
	m_ctrlMsg2.SetWindowText(szMsg);
	m_ctrlMsg2.Invalidate();
	m_ctrlMsg2.UpdateWindow();
}

void CProgressMsgDlg::SetMessage(CString szMsg1, CString szMsg2)
{
	m_szMsg1 = szMsg1;
	m_szMsg2 = szMsg2;
}

BOOL CProgressMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_ctrlProgress.SetRange(1,100);
	m_ctrlProgress.SetPos(0);

	GetWindowRect(&m_dlgRect);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressMsgDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CProgressMsgDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CProgressMsgDlg::SetProgressPos(int nPos)
{
	m_ctrlProgress.SetPos(nPos);
}
