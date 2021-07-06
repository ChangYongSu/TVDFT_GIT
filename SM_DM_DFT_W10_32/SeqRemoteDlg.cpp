// SeqRemoteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SeqRemoteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeqRemoteDlg dialog


CSeqRemoteDlg::CSeqRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeqRemoteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeqRemoteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSeqRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeqRemoteDlg)
	DDX_Control(pDX, IDC_CMB_REMOTE_CODE, m_ctrlRemoteCodeCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeqRemoteDlg, CDialog)
	//{{AFX_MSG_MAP(CSeqRemoteDlg)
	ON_COMMAND_RANGE(IDC_BTN_REMOTE_DLG_1, IDC_BTN_REMOTE_DLG_75,OnRemoteButtonClick)
	ON_CBN_SELCHANGE(IDC_CMB_REMOTE_CODE, OnSelchangeCmbRemoteCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CSeqRemoteDlg::OnRemoteButtonClick(UINT nButtonID)
{
//	int nButton = nButtonID - IDC_BTN_REMOTE_DLG_1;
	CString sTmp = _T("");
	CString strName, sName, sCode, sCustomCode, sDesc;
	strName = sName = sCode = sCustomCode = sDesc = _T("");
	int nByte =1;


	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));

	GetDlgItem(nButtonID)->GetWindowText(strName);

	if(!FindButtonKey(strName, sName, sCustomCode, sCode, sDesc, nByte)){
		GetDlgItem(nButtonID)->EnableWindow(FALSE);
		return;
	}
	else
	{
		GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(sCode);
		GetDlgItem(IDC_EDIT_DESC)->SetWindowText(sDesc);
		m_strKeyCode.Format("%s", sCode);
	}
}
BOOL CSeqRemoteDlg::FindButtonKey(CString sBtnName, CString& strName, CString& strCustomCode, CString& strCode, CString& strDesc, int& nByte)
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

void CSeqRemoteDlg::InitRemoteCodeCmb()
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
// CSeqRemoteDlg message handlers

BOOL CSeqRemoteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));

	//////////////////////////////////////////////////////////////////////////
	CString sTmp; 
	sTmp.Format("Remote Control Dialog  - %s", CurrentSet->sModelFolder);
	SetWindowText(sTmp);
	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);
	//////////////////////////////////////////////////////////////////////////

	InitRemoteCodeCmb();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeqRemoteDlg::OnSelchangeCmbRemoteCode() 
{
	CString sTmp = _T("");
	CString strName, sName, sCode, sCustomCode, sDesc;
	strName = sName = sCode = sCustomCode = sDesc = _T("");
	int nByte =1;

	GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DESC)->SetWindowText(_T(""));

	m_ctrlRemoteCodeCmb.GetLBText(m_ctrlRemoteCodeCmb.GetCurSel(),strName);

	if(!FindButtonKey(strName, sName, sCustomCode, sCode, sDesc, nByte)) return;
	else
	{
		GetDlgItem(IDC_EDIT_KEYCODE)->SetWindowText(sCode);
		GetDlgItem(IDC_EDIT_DESC)->SetWindowText(sDesc);
		m_strKeyCode.Format("%s", sCode);
	}	
}
