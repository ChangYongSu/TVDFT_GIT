// StopStepDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "StopStepDlg.h"
#include "step.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "Global.h"


extern HWND g_hCommWnd;


//+ 2007.9.28 Add BY USY
extern CDATsysView * g_pView;

//+ 2007.11.4 Add BY USY
extern int g_nRunningProcessNo;

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CStopStepDlg dialog


//+ 2007.11.4 Add BY USY
LRESULT CStopStepDlg::OnProcessMessage(WPARAM wParam, LPARAM lParam)
{
	if(g_nRunningProcessNo == 1)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
				break;
			case MSG_STOP_BTN1:
				{
					m_nRunType = STEP_STOP;
					//m_bThreadStop = TRUE;
					PostMessage(WM_COMMAND,IDOK);
				}
				break;
				
			case MSG_RETRY_BTN1:
				{
					m_nRunType = STEP_RETRY;
					//m_bThreadStop = TRUE;
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
			case MSG_OK_BTN1:
//				if(pCurStep->m_nTestType != ADC_TEST)
				if(!CurrentSet->bUseScanner)
				{
					m_nRunType = STEP_OK;	
					//m_bThreadStop = TRUE;
					PostMessage(WM_COMMAND, IDOK);	
				}
				break;
			case MSG_NG_BTN1:
				m_nRunType = STEP_NG;	
				//m_bThreadStop = TRUE;
				PostMessage(WM_COMMAND, IDOK);
				break;
				
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
				break;
			case MSG_STOP_BTN2:
				{
					m_nRunType = STEP_STOP;
					//m_bThreadStop = TRUE;
					PostMessage(WM_COMMAND,IDOK);
				}
				break;

			case MSG_RETRY_BTN2:
				{
					m_nRunType = STEP_RETRY;
					//m_bThreadStop = TRUE;
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
			case MSG_OK_BTN2:
//				if(pCurStep->m_nTestType != ADC_TEST)
				if(!CurrentSet->bUseScanner)
				{
					m_nRunType = STEP_OK;	
					//m_bThreadStop = TRUE;
					PostMessage(WM_COMMAND, IDOK);	
				}
				break;
			case MSG_NG_BTN2:
				m_nRunType = STEP_NG;	
				//m_bThreadStop = TRUE;
				PostMessage(WM_COMMAND, IDOK);
				break;
		}
	}
	return 1;
}
	//-

CStopStepDlg::CStopStepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStopStepDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStopStepDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//+ 2007.11.5 Add BY USY
	m_bActivate = FALSE;
	//-

	//+add kwmoon 080618
	m_szMsg1	= _T("");
	m_szMsg2	= _T("");
	m_szMsg3	= _T("");
}


void CStopStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStopStepDlg)
	//DDX_Control(pDX, IDC_LABEL_MSG1, m_ctrlTestResultInfo1);
	//DDX_Control(pDX, IDC_LABEL_MSG2, m_ctrlTestResultInfo2);
	//DDX_Control(pDX, IDC_LABEL_MSG3, m_ctrlTestResultInfo3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStopStepDlg, CDialog)
	//{{AFX_MSG_MAP(CStopStepDlg)
	ON_MESSAGE (WM_JOYSTICK_MSG, OnProcessMessage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_STOP2, OnBtnStop2)
	ON_BN_CLICKED(IDC_BTN_RETRY2, OnBtnRetry2)
	ON_BN_CLICKED(IDC_BTN_OK2, OnBtnOk2)
	ON_BN_CLICKED(IDC_BTN_NG2, OnBtnNg2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CStopStepDlg message handlers

BEGIN_EVENTSINK_MAP(CStopStepDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CStopStepDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
/*
void CStopStepDlg::OnClickBtnRetry() 
{
	m_nRunType = STEP_RETRY;
	OnOK();
}

void CStopStepDlg::OnClickBtnStop() 
{
	m_nRunType = STEP_STOP;
	OnOK();
}

void CStopStepDlg::OnClickBtnOk() 
{
	m_nRunType = STEP_OK;
	OnOK();
}

void CStopStepDlg::OnClickBtnNg() 
{
	m_nRunType = STEP_NG;
	OnOK();
}
*/
BOOL CStopStepDlg::PreTranslateMessage(MSG* pMsg) 
{
//	if(pMsg->message == WM_KEYDOWN)
//	{
//		switch(pMsg->wParam)
//		{
//			case VK_F5 : 
//				OnClickBtnContinue();
//			break;
//		}
//	}	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CStopStepDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//+ 2007.11.5 Add BY USY
	m_bActivate = TRUE;
	//-
	if(CurrentSet->bUseScanner)
	{
		GetDlgItem(IDC_BTN_OK2)->EnableWindow(FALSE);
	}

/*	if(pCurStep->m_nTestType == ADC_TEST)
	{
		GetDlgItem(IDC_BTN_OK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_NG2)->EnableWindow(FALSE);
	}
*/
	//+add kwmoon 080229
	CRect rect;
	GetWindowRect(&rect);

	int nLeft   = g_pView->m_FormViewRect.left + (g_pView->m_FormViewRect.Width()/2)  - (rect.Width()/2);
	int nTop    = (g_pView->m_FormViewRect.Height()/2) + 210; // - (rect.Height()/2);
//	int nLeft   = g_pView->m_StatusViewScreenRect.right - rect.Width() +10;
//	int nTop    = g_pView->m_FormViewRect.bottom - rect.Height() - 36;
	int nWidth  = rect.Width();
	int nHeight = rect.Height();
	
//	SetWindowPos(&wndTopMost,nLeft,nTop,nWidth,nHeight,SWP_NOSIZE | SWP_NOMOVE);
	MoveWindow(nLeft,nTop,nWidth,nHeight);
//	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);


	//+add kwmoon 080618
	int nMsgLength = 0;

	if(m_szMsg1 != _T(""))
	{
		//m_ctrlTestResultInfo1.SetCaption(m_szMsg1); 
		SetDlgItemText(IDC_STATIC_INFO1, m_szMsg1);
	}
	
	if(m_szMsg2 != _T(""))
	{
		SetDlgItemText(IDC_STATIC_INFO2, m_szMsg2); //m_ctrlTestResultInfo2.SetCaption(m_szMsg2);
	}

	if(m_szMsg3 != _T(""))
	{
		SetDlgItemText(IDC_STATIC_INFO3, m_szMsg3); //m_ctrlTestResultInfo3.SetCaption(m_szMsg3);
	}

	UpdateData(FALSE);
	//-

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CStopStepDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	//+ 2007.11.5 Add BY USY
	m_bActivate = FALSE;
	//-
}

BOOL CStopStepDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreCreateWindow(cs);
}

//+add kwmoon 080618
void CStopStepDlg::SetTestResultInfo(CString szResultMsg1,CString szResultMsg2, CString szResultMsg3)
{
	m_szMsg1 = szResultMsg1;
	m_szMsg2 = szResultMsg2;
	m_szMsg3 = szResultMsg3;
}

void CStopStepDlg::OnBtnStop2() 
{
	m_nRunType = STEP_STOP;
	OnOK();
}

void CStopStepDlg::OnBtnRetry2() 
{
	m_nRunType = STEP_RETRY;
	OnOK();
}

void CStopStepDlg::OnBtnOk2() 
{
	m_nRunType = STEP_OK;
	OnOK();
}

void CStopStepDlg::OnBtnNg2() 
{
	m_nRunType = STEP_NG;
	OnOK();
}
