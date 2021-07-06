// StopProcessingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "StopProcessingDlg.h"
#include "Global.h"
#include "DATsysView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define START_PROGRESS_BAR	100

/////////////////////////////////////////////////////////////////////////////
// CStopProcessingDlg dialog


CStopProcessingDlg::CStopProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStopProcessingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStopProcessingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStopProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStopProcessingDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	//DDX_Control(pDX, IDC_LABEL_SYSTEM_MESSAGE, m_ctrlMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStopProcessingDlg, CDialog)
	//{{AFX_MSG_MAP(CStopProcessingDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStopProcessingDlg message handlers

BOOL CStopProcessingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ctrlProgress.SetBkColor(DARK_BLACK_COLOR);
	m_ctrlProgress.SetGradientColors(LIGHT_BLUE_COLOR,DARK_BLUE_COLOR);
	m_ctrlProgress.SetTextColor(DARK_BLACK_COLOR, DARK_BLACK_COLOR);
//	m_ctrlProgress.SetSnake(TRUE);

	m_nProgress = 0;

	m_bShowDlg = FALSE;

	GetWindowRect(&m_dlgRect);
	
	SetTimer(START_PROGRESS_BAR,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStopProcessingDlg::SetShowDlg(BOOL bShow)
{
	if(bShow)
	{
		m_bShowDlg = TRUE;
	}
	else 
	{
		m_bShowDlg = FALSE;
	}
}

void CStopProcessingDlg::SetMessage(int nMessageType)
{
	CString szMessage = _T("");

	switch(nMessageType)
	{
		case STOP_TEST_PROCESSING_MSG :
		{
			szMessage.Format("Please wait for finishing test stop process");
			SetDlgItemText(IDC_STATIC_INFO1, szMessage);
		//	m_ctrlMessage.SetCaption((LPCTSTR)szMessage); 
		//	m_ctrlMessage.SetCaption("Please wait for finishing test stop process"); 
		}
		break;

		case STOP_GRAB_PROCESSING_MSG :
		{
			szMessage.Format("Please wait for finishing grab stop process");
			SetDlgItemText(IDC_STATIC_INFO1, szMessage);
		//	m_ctrlMessage.SetCaption((LPCTSTR)szMessage); 
		//	m_ctrlMessage.SetCaption("Please wait for finishing grab stop process"); 
		}
		break;
		//090402
		case DATA_DELETE_PROCESSING_MSG :
		{
			szMessage.Format("Please wait for finishing Data Delete process");
			SetDlgItemText(IDC_STATIC_INFO1, szMessage);
			//m_ctrlMessage.SetCaption((LPCTSTR)szMessage); 
		//	m_ctrlMessage.SetCaption("Please wait for finishing grab stop process"); 
		}
		break;

		//090402
		case MES_DATA_AUTOUPLOAD_MSG :
		{
			szMessage.Format("Please wait.. MES Data Uploading.");
			SetDlgItemText(IDC_STATIC_INFO1, szMessage);
			//m_ctrlMessage.SetCaption((LPCTSTR)szMessage); 
		//	m_ctrlMessage.SetCaption("Please wait for finishing grab stop process"); 
		}
		break;
	}
}

void CStopProcessingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	MSG Msg;

	switch(nIDEvent)
	{
		case START_PROGRESS_BAR :
		{
			if(m_nProgress <= 100) m_nProgress += 5;
			else KillTimer(START_PROGRESS_BAR);

			m_ctrlProgress.SetPos(m_nProgress);

			//+add 090409(Modification No2)
			if(::PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CStopProcessingDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::PostNcDestroy();
//	delete this;
}


void CStopProcessingDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(START_PROGRESS_BAR);
	
}
