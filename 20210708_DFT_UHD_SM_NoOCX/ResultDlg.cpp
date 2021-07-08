// ResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ResultDlg.h"
#include "Global.h"
#include "RealMeas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

extern HWND g_hCommWnd;

extern CDATsysView * g_pView;


/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CResultDlg dialog


CResultDlg::CResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResultDlg)
	m_szTestResultInfo1 = _T("");
	m_szTestResultInfo2 = _T("");
	m_szTestResultInfo3 = _T("");
	//}}AFX_DATA_INIT
	//+ 2007.11.5 Add BY USY
	m_bActivate = FALSE;
	//-
	
	//+add kwmoon 080508
	m_bContinueTest = FALSE;

	//+add kwmoon 071122
	m_nImageID	= IDB_PASS_BMP;
	//-

}


void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultDlg)
	DDX_Control(pDX, IDC_RESULT_IMAGE, m_ctrlResultImage);
	DDX_Text(pDX, IDC_STATIC_MSG1, m_szTestResultInfo1);
	DDX_Text(pDX, IDC_STATIC_MSG2, m_szTestResultInfo2);
	DDX_Text(pDX, IDC_STATIC_MSG3, m_szTestResultInfo3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResultDlg, CDialog)
	//{{AFX_MSG_MAP(CResultDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CONTINUE, OnBtnContinue)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CResultDlg::SetImage(int nImageID)
{
	m_nImageID = nImageID;
}

BOOL CResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//+add kwmoon 071122
	m_pBitmap.DeleteObject();
	m_pBitmap.LoadBitmap(m_nImageID);
	//-

	m_ctrlResultImage.SetBitmap((HBITMAP)m_pBitmap);

	//+add kwmoon 071210
	m_bContinueTest = FALSE;

	int nMsgLength = 0;

	if(m_szMsg1 != _T(""))
	{
	//	m_ctrlTestResultInfo1.SetCaption(m_szMsg1); 
		m_szTestResultInfo1 = m_szMsg1; 
	}
	
	if(m_szMsg2 != _T(""))
	{
		nMsgLength = m_szMsg2.GetLength();
		
		if(nMsgLength <= 40)
		{
			m_szTestResultInfo2 = m_szMsg2; 
		//	m_ctrlTestResultInfo2.SetCaption(m_szMsg2);
		}
		else
		{
			m_szTestResultInfo2 = m_szMsg2.Left(40);
			m_szTestResultInfo3 = m_szMsg2.Mid(40,m_szMsg2.GetLength()-40);
		//	m_ctrlTestResultInfo2.SetCaption(m_szMsg2.Left(40));
		//	m_ctrlTestResultInfo3.SetCaption(m_szMsg2.Mid(40,m_szMsg2.GetLength()-40));
		}
	}

	if(CurrentSet->bUseScanner)
	{
		GetDlgItem(IDC_BTN_CONTINUE)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	//+ 2007.11.5 Add BY USY
	m_bActivate = TRUE;
	//-

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResultDlg::OnBtnContinue() 
{
	m_bContinueTest = TRUE;
	g_pView->m_bResultDlgActivate = FALSE;
	//OnOK();
	DestroyWindow();
	//+add kwmoon 080618
	g_pView->Invalidate();
//	g_pView->UpdateWindow();
	g_pView->PostMessage(UM_UPDATE_DISPLAY,ALL_VIEW, 0);
	//+add kwmoon 080618
	g_pView->AudioMeasureStop();
	g_pView->PostMessage(WM_COMMAND, ID_RUN_RUN);
}

void CResultDlg::OnBtnClose() 
{
	g_pView->m_bResultDlgActivate = FALSE;
	//OnOK();
	DestroyWindow();
	//+add kwmoon 080618
	g_pView->PostMessage(UM_UPDATE_DISPLAY,ALL_VIEW, 0);
}
void CResultDlg::SetContinue()
{
	OnBtnContinue();
}
void CResultDlg::SetClose()
{
	OnBtnClose();
}
BOOL CResultDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE)){
			PostMessage(WM_COMMAND, IDC_BTN_CLOSE);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CResultDlg::SetTestResultInfo(CString szResultMsg1,CString szResultMsg2)
{
	m_szMsg1 = szResultMsg1;
	m_szMsg2 = szResultMsg2;
}

void CResultDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	//+ 2007.11.5 Add BY USY
	m_bActivate = FALSE;
	//+add psh 080603
	g_pView->m_bResultDlgActivate = FALSE;
	PostNcDestroy();
	//-
}

void CResultDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}





