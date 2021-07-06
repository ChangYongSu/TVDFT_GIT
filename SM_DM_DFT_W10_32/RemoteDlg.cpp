// RemoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "RemoteDlg.h"
#include "ImageTotalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg dialog


CRemoteDlg::CRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRemoteDlg)
	//}}AFX_DATA_INIT
}


void CRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRemoteDlg)
	DDX_Control(pDX, IDC_CMB_REMOTE_CODE, m_ctrlRemoteCodeCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRemoteDlg, CDialog)
	//{{AFX_MSG_MAP(CRemoteDlg)
	ON_COMMAND_RANGE(IDC_BTN_REMOTE_DLG_1, IDC_BTN_REMOTE_DLG_75,OnRemoteButtonClick)
	ON_BN_CLICKED(IDC_BTN_SEND_REMOTE_CODE, OnBtnSendRemoteCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CRemoteDlg::OnRemoteButtonClick(UINT nButtonID)
{
//	int nButton = nButtonID - IDC_BTN_REMOTE_DLG_1;
	CString sTmp = _T("");
	UINT nVal1 = 0;
	CString strName, sName, sCode, sCustomCode, sDesc;
	strName = sName = sCode = sCustomCode = sDesc = _T("");
	int nByte =1;

	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(_T(""));


	GetDlgItem(nButtonID)->GetWindowText(strName);

	if(!FindButtonKey(strName, sName, sCustomCode, sCode, sDesc, nByte)){
		GetDlgItem(nButtonID)->EnableWindow(FALSE);
		return;
	}
	else
	{
		if(CurrentSet->bUseAVSwitchBox)
		{
			GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(sCode);
			GetDlgItem(IDC_EDIT_DESC)->SetWindowText(sDesc);

			if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, m_nRmoteType, sCustomCode, sCode, nByte))
			{
				GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
			}
			else 
			{
				GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
			}
		}
	}
}

BOOL CRemoteDlg::FindButtonKey(CString sBtnName, CString& strName, CString& strCustomCode, CString& strCode, CString& strDesc, int& nByte)
{
	POSITION Pos = NULL;
	CRemoteCode* pRemote = NULL;
	CString sName, sCode;
	BOOL bFind = FALSE;

	Pos = CurrentSet->RemoteList.GetHeadPosition();

	while(Pos)
	{
		pRemote = CurrentSet->RemoteList.GetNext(Pos);
		sName = pRemote->sName;
		sName.MakeUpper();
		sBtnName.MakeUpper();

		if(sName == sBtnName)
		{
			strName = pRemote->sName;
			strCode = pRemote->sCode;
			strCustomCode = pRemote->sCustomCode;
			strDesc = pRemote->sDesc;
			nByte = pRemote->nByte;
			bFind = TRUE;
			return TRUE;
		}
	}

	if(bFind == FALSE) 
	{
		return FALSE;
	}
	return TRUE;
}


void CRemoteDlg::InitRemoteCodeCmb()
{
	POSITION Pos = NULL;
	CRemoteCode* pRemote = NULL;
	CString sName, sCode;

	Pos = CurrentSet->RemoteList.GetHeadPosition();
	m_ctrlRemoteCodeCmb.ResetContent();

	while(Pos)
	{
		pRemote = CurrentSet->RemoteList.GetNext(Pos);
		sName = pRemote->sName;
		sName.MakeUpper();
	//	if(pRemote->bUse != TRUE)
	//	{
			m_ctrlRemoteCodeCmb.AddString(sName);
	//	}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CRemoteDlg message handlers

BOOL CRemoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(_T(""));

	//////////////////////////////////////////////////////////////////////////
	CString sTmp; 
	sTmp.Format("Remote Control Dialog  - %s", CurrentSet->sModelFolder);
	SetWindowText(sTmp);
	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);
	//////////////////////////////////////////////////////////////////////////
	

	GetWindowRect(&m_dlgRect);

	int nLeft   = m_MWnd->m_PageScreenRect.left;
	int nTop    = m_MWnd->m_PageScreenRect.bottom - m_dlgRect.Height();

	SetWindowPos(&wndTopMost,nLeft,nTop,0,0,SWP_NOSIZE);
	ShowWindow(SW_SHOW);
	UpdateWindow();

	InitRemoteCodeCmb();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CRemoteDlg::OnBtnSendRemoteCode() 
{
	CString sTmp = _T("");
	UINT nVal1 = 0;
	CString strName, sName, sCode, sCustomCode, sDesc;
	strName = sName = sCode = sCustomCode = sDesc = _T("");
	int nByte =1;

	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(_T(""));

	m_ctrlRemoteCodeCmb.GetLBText(m_ctrlRemoteCodeCmb.GetCurSel(),strName);


	if(!FindButtonKey(strName, sName, sCustomCode, sCode, sDesc, nByte)) return;
	else
	{
		if(CurrentSet->bUseAVSwitchBox)
		{
			GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(sCode);
			GetDlgItem(IDC_EDIT_DESC)->SetWindowText(sDesc);

			if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, m_nRmoteType, sCustomCode, sCode, nByte))
			{
				GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
			}
			else 
			{
				GetDlgItem(IDC_EDIT_RESULT)->SetWindowText(AvSwitchBoxCtrl.m_strErrMsg);
			}
		}
	}
}
