// UserMsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "UserMsgDlg.h"
#include "DATsysView.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;

//+add 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;
/////////////////////////////////////////////////////////////////////////////
// CUserMsgDlg dialog


CUserMsgDlg::CUserMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserMsgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserMsgDlg)
	m_szMessage1 = _T("");
	m_szMessage2 = _T("");
	//}}AFX_DATA_INIT
}


void CUserMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserMsgDlg)
	DDX_Text(pDX, IDC_STATIC_MESSAGE1, m_szMessage1);
	DDX_Text(pDX, IDC_STATIC_MESSAGE2, m_szMessage2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserMsgDlg, CDialog)
	//{{AFX_MSG_MAP(CUserMsgDlg)
	ON_BN_CLICKED(IDC_BTN_YES, OnBtnYes)
	ON_BN_CLICKED(IDC_BTN_NO, OnBtnNo)
	ON_MESSAGE (WM_JOYSTICK_MSG, OnProcessMessage)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserMsgDlg message handlers
//+ 2009.09.02 Add BY PSH
long CUserMsgDlg::OnProcessMessage(WPARAM wParam, LPARAM lParam)
{
	if(g_nRunningProcessNo == 1)
	{
		switch(lParam) 
		{
			case MSG_OK_BTN1:
				{
					m_bYes	= TRUE;
					PostMessage(WM_COMMAND, IDOK);	
				}
				break;
			case MSG_START_BTN1:
			case MSG_STOP_BTN1:
			case MSG_RETRY_BTN1:
			case MSG_NG_BTN1:
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_OK_BTN2:
			case MSG_NG_BTN2:
				break;
		}
	}
	else if(g_nRunningProcessNo == 2)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
			case MSG_STOP_BTN1: 
			case MSG_RETRY_BTN1:
			case MSG_OK_BTN1:
			case MSG_NG_BTN1:
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_NG_BTN2:
				break;

			case MSG_OK_BTN2:
				{
					m_bYes	= TRUE;
					PostMessage(WM_COMMAND, IDOK);	
				}
				break;
		}
	}
	else if(g_nRunningProcessNo == 3)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
			case MSG_STOP_BTN1: 
			case MSG_RETRY_BTN1:
			case MSG_OK_BTN1:
			case MSG_NG_BTN1:
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_OK_BTN2:
			case MSG_NG_BTN2:
			case MSG_START_BTN3:
			case MSG_STOP_BTN3:
			case MSG_RETRY_BTN3:
			case MSG_NG_BTN3:
				break;

			case MSG_OK_BTN3:
				{
					m_bYes	= TRUE;
					PostMessage(WM_COMMAND, IDOK);	
				}
				break;
		}
	}
	return 1;
}
	//-

BOOL CUserMsgDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetWindowRect(&m_dlgRect);

	int nLeft   = g_pView->m_FormViewRect.left + (g_pView->m_FormViewRect.Width()/2)  - (m_dlgRect.Width()/2);
	int nTop    = (g_pView->m_FormViewRect.Height()/2) + 210; // - (m_dlgRect.Height()/2);
	int nWidth  = m_dlgRect.Width();
	int nHeight = m_dlgRect.Height();
	
	MoveWindow(nLeft,nTop,nWidth,nHeight);


	ShowMessage1(m_szMsg1);
	ShowMessage2(m_szMsg2);
	
	UpdateData(FALSE);
	//+add kwmoon 080508
	m_bYes	= TRUE;

	g_pView->PlayWavSound(IDR_WAVE_RETRY);

	
	//+ add 090902
	m_bActivate = TRUE;
	//-
	if (m_bAutoClose == 1)
	{
		SetTimer(1, 3000, NULL);
	}
	//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
	CString szString;
	szString.Format("UserKeyCheck %x\n",m_pUserKeyCheck->m_nThreadID);
//	if(g_LogFileOpen) g_LogFile.WriteString(szString);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CUserMsgDlg::ShowMessage1(CString szMessage)
{
	m_szMessage1 = szMessage;
	//m_ctrlMsg1.SetWindowText(szMessage);
	//m_ctrlMsg1.Invalidate();
	//m_ctrlMsg1.UpdateWindow();
}

void CUserMsgDlg::ShowMessage2(CString szMessage)
{
	m_szMessage2 = szMessage;
//	m_ctrlMsg2.SetWindowText(szMessage);
//	m_ctrlMsg2.Invalidate();
//	m_ctrlMsg2.UpdateWindow();
}

void CUserMsgDlg::SetMessage(CString szMessage1, CString szMessage2)
{
	m_szMsg1 = szMessage1;
	m_szMsg2 = szMessage2;
}

BEGIN_EVENTSINK_MAP(CUserMsgDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUserMsgDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CUserMsgDlg::OnBtnYes() 
{
	// TODO: Add your control notification handler code here
	m_bYes	= TRUE;

	//+ add 090902
	m_bActivate = FALSE;
	//-

	OnOK();
	
}

void CUserMsgDlg::OnBtnNo() 
{
	// TODO: Add your control notification handler code here
	m_bYes	= FALSE;

	//+ add 090902
	m_bActivate = FALSE;
	//-

	OnOK();
	
}




void CUserMsgDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_bActivate = FALSE;
	
}


void CUserMsgDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	OnBtnYes();
	m_bAutoClose = 0;
	CDialog::OnTimer(nIDEvent);
}
