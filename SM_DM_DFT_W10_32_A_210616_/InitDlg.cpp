// InitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "InitDlg.h"
#include "Defines.h"
#include "Global.h"
#include "SoundCard.h"

extern CDATsysView*  g_pView;

//+add 090219(Modification No2)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_nRunningProcessNo;
extern CEdit ctrlWipIdEdit;	

//+add kwmoon 080523
_LineID LindIDList[] = 
{
	0,	"MMK1",	763,
	1,	"MMK2", 764,
	2,	"MMK3",	751,
	3,	"MMK4",	752,
	4,	"MJS1",	761,
	5,	"MJS2",	762,
	6,	"MJS3",	759,
	7,	"MSG1",	757,
	8,	"MSG2",	729,
	9,	"MSG3", 731,
	10,	"MSG4", 733,
	11,	"MSG5", 701,
	12, "TSW",  689,
	13,	 NULL,  0
};

/////////////////////////////////////////////////////////////////////////////
// CInitDlg dialog


CInitDlg::CInitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInitDlg)
	//}}AFX_DATA_INIT
}


void CInitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_ctrlMsgList);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_ctrlPasswordEdit);
	DDX_Control(pDX, IDC_CMB_SYSTEM_NO, m_ctrlSystemNoCmb);
	DDX_Control(pDX, IDC_CMB_LINE_NO, m_ctrlLineNoCmb);
	DDX_Control(pDX, IDC_CMB_AUTHORITY, m_ctrlAuthorityCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInitDlg, CDialog)
	//{{AFX_MSG_MAP(CInitDlg)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_CBN_SELCHANGE(IDC_CMB_AUTHORITY, OnSelchangeCmbAuthority)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CInitDlg::InitControl()
{
	//+add 090219(Modification No2)
	CString szLog = "B:CInitDlg-InitControl\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
	
	CString sTmp = _T("");
	CString sTemp;
	CString sBuf;
	CString sBuf2;
	CStringArray SplitIt;
	int nNoList;

	//Init Control
	int nSelLineNo	 = 0;

	//Authority
	m_ctrlAuthorityCmb.ResetContent();
	m_ctrlAuthorityCmb.AddString(_T("Developer"));
	m_ctrlAuthorityCmb.AddString(_T("Admin"));
	m_ctrlAuthorityCmb.AddString(_T("Operator"));
	
	m_ctrlAuthorityCmb.SetCurSel(CurrentSet->nAuthority);

	//Line No, System No
	m_ctrlLineNoCmb.ResetContent();
	m_ctrlSystemNoCmb.ResetContent();

	for(int nTmp = 0; nTmp < 50; nTmp++)
	{
		sTmp.Format("%d", nTmp + 1);
		m_ctrlSystemNoCmb.AddString(sTmp);
	}

	if((CurrentSet->nSystemNo > 0)
	&& (CurrentSet->nSystemNo <= 50))
	{
		m_ctrlSystemNoCmb.SetCurSel(CurrentSet->nSystemNo-1);
	}
	else
	{
		m_ctrlSystemNoCmb.SetCurSel(0);
	}

	//+change 090206(Modification No2)
//	for(int i = 0 ; i < m_saLineList.GetSize() ; i++)
	int nLineListSize = m_saLineList.GetSize();

	if(m_saLineList.GetSize() > 0)
	{
		for(int i = 0 ; i < m_saLineList.GetSize() ; i++)
		{
			sTemp = m_saLineList.GetAt(i);
			SplitIt.RemoveAll();
			
			StringToken(sTemp, ",", SplitIt, TRUE);

			sBuf = SplitIt.GetAt(1);
			sBuf.TrimLeft(' ');
			sBuf.TrimRight(' ');

			sBuf2 = SplitIt.GetAt(2);
			sBuf2.TrimLeft(' ');
			sBuf2.TrimRight(' ');
			nNoList = _ttoi((const TCHAR*)sBuf2);

			m_ctrlLineNoCmb.AddString(sBuf);

			if(CurrentSet->nLineNo == nNoList) 
			{
				nSelLineNo	 = i;
				m_ctrlLineNoCmb.SetCurSel(nSelLineNo);
			}
		}
	}
	m_ctrlLineNoCmb.SetCurSel(nSelLineNo);

	//+add 090203(Modification No5)
	m_ctrlProgress.SetRange(1,100);

	//Password
	m_ctrlPasswordEdit.SetWindowText(_T(""));
	if(m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_OP)
	{
		m_ctrlPasswordEdit.EnableWindow(FALSE);
	}
	else m_ctrlPasswordEdit.EnableWindow(TRUE);

	//+add 090219(Modification No2)
	szLog = "E:CInitDlg-InitControl\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
}

void CInitDlg::AddString2List(CString strString)
{
	m_ctrlMsgList.AddString(strString);
	m_ctrlMsgList.SetCurSel(m_ctrlMsgList.GetCurSel() + 1);
	UpdateData(TRUE);
}
void CInitDlg::UpdateString2List(CString strString)
{
	m_ctrlMsgList.InsertString( m_ctrlMsgList.GetCurSel(), strString);
	UpdateData(TRUE);
}

//+del kwmoon 080523
/*
CString CInitDlg::MakeUserID()
{
	CString sTmp, sUserId;
	switch(m_ctrlLocationCmb.GetCurSel())
	{
		case 0: sTmp = _T("PR");	break;
		case 1:	sTmp = _T("DD");	break;
		case 2: sTmp = _T("JS");	break;
		case 3:	sTmp = _T("SG");	break;
		default: sTmp = _T("DD");	break;
	}
	sUserId.Format("%s%d%02d%d", sTmp, (m_ctrlLineNoCmb.GetCurSel() + 1), (m_ctrlSystemNoCmb.GetCurSel() + 1), g_nRunningProcessNo);
	return sUserId;
}
*/

BOOL CInitDlg::CheckPassword()
{
	CString sTmp;
	m_ctrlPasswordEdit.GetWindowText(sTmp);
	sTmp.MakeUpper(); sTmp.TrimLeft(); sTmp.TrimRight();

	if(m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_DEV)
	{
		if(sTmp != DEV_PASSWORD) 
		{
			AfxMessageBox("Password is Wrong!");
			return FALSE;
		}
		else return TRUE;
	}
	else if(m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_ADMIN)
	{
		if(sTmp != CurrentSet->strPassword)
		{
			AfxMessageBox("Password is Wrong!");
			return FALSE;
		}
		else return TRUE;
	}
	else if(m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_OP) return TRUE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CInitDlg message handlers

BOOL CInitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetWindowRect(&m_dlgRect);

	//+change 090206(Modification No2)
//	LineList_Read();
	if(!LineList_Read())
	{
		AfxMessageBox("Failed to open PCBA_LineList.ini!\r\nApplication will be closed"); 
		
		//+add 090220(Modification No4)
		m_bInitComplete = FALSE;

	//	AfxGetMainWnd()->PostMessage(WM_CLOSE);
	}
	else
	{
		//+add 090220(Modification No4)
		m_bInitComplete = TRUE;

		InitControl();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInitDlg::OnOK() 
{
	CString sTemp;
	CString sBuf;
	CString sBuf2;
	CStringArray SplitIt;
	int nNoList;

	if(!CheckPassword()) return;
	else
	{
		CurrentSet->nAuthority  = m_ctrlAuthorityCmb.GetCurSel();

		//+change kwmoon 080523
	//	CurrentSet->nLineNo		= m_ctrlLineNoCmb.GetCurSel();
		int nCurSel = m_ctrlLineNoCmb.GetCurSel();

		CString szLineID = _T("");
		m_ctrlLineNoCmb.GetLBText(nCurSel,szLineID);

		//+add kwmoon 080523
/*		int nTmp = 0;

		while(LindIDList[nTmp].pszLineName != NULL)
		{
			if(szLineID.Compare(LindIDList[nTmp].pszLineName) == NULL)
			{
				CurrentSet->nLineNo	= LindIDList[nTmp].nLineNo; break;
			}
			nTmp++;
		} */

		//+change 090206(Modification No2)
	//	for(int i = 0 ; i < m_saLineList.GetSize() ; i++)
		int nLineListSize = m_saLineList.GetSize();

		if(m_saLineList.GetSize() > 0)
		{
			for(int i = 0 ; i < m_saLineList.GetSize() ; i++)
			{
				sTemp = m_saLineList.GetAt(i);
				SplitIt.RemoveAll();

				StringToken(sTemp, ",", SplitIt, TRUE);

				sBuf = SplitIt.GetAt(1);
				sBuf.TrimLeft(' ');
				sBuf.TrimRight(' ');

				sBuf2 = SplitIt.GetAt(2);
				sBuf2.TrimLeft(' ');
				sBuf2.TrimRight(' ');
				nNoList = _ttoi((const TCHAR*)sBuf2);

				if(szLineID.Compare(sBuf) == NULL)
				{
					CurrentSet->nLineNo	= nNoList; break;
				}
			}
		}

		//+change kwmoon 080523
		CurrentSet->nSystemNo   = m_ctrlSystemNoCmb.GetCurSel()+1;

		//+del kwmoon 080523
	//	CurrentSet->nLocation   = m_ctrlLocationCmb.GetCurSel();
	//	CurrentSet->strUserId		= MakeUserID();

		//+add kwmoon 080524
		// if user is operator, wipid must be input by scanner.
	//	if(CurrentSet->nAuthority == AUTHORITY_OP)
	//	{
	//		ctrlWipIdEdit.SetReadOnly();
	//	}
		//+add PSH 080527
		if(CurrentSet->bUseScanner)
		{
			ctrlWipIdEdit.SetReadOnly(TRUE);
			
		}
		else
		{
			ctrlWipIdEdit.SetReadOnly(FALSE);
		}
		//+add kwmoon 080522
		g_pView->SaveRegistrySetting();
	}
//	g_pView->StartLVDSGrabThread();

	g_pView->SetTimer(1, 1000, NULL);
//	g_pView->SetTimer(2, 1500, NULL);


	CDialog::OnOK();

}

void CInitDlg::OnBtnExit() 
{
//	m_MWnd->m_pCheckDeviceDlg = NULL;
	m_MWnd->StopGrabThread();
	m_MWnd->PostMessage(WM_CLOSE);
	PostNcDestroy();
}

void CInitDlg::OnSelchangeCmbAuthority() 
{
	m_ctrlPasswordEdit.EnableWindow(TRUE);
	switch(m_ctrlAuthorityCmb.GetCurSel())
	{
		case AUTHORITY_DEV:
		case AUTHORITY_ADMIN:
			m_ctrlPasswordEdit.EnableWindow(TRUE);
			break;
		case AUTHORITY_OP:
			m_ctrlPasswordEdit.EnableWindow(FALSE);
			break;
	}
}

void CInitDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
//	delete this;
}

BOOL CInitDlg::LineList_Read()
{
 	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sBuf;
	CString filename;
	CString sModel;

	CString sDay;
	CString sMonth;
	CString sTemp2;
	CStringArray SplitIt;

	filename.Format(_T("%s\\PCBA_LineList.ini"),g_pView->m_szExePath); 
	m_saLineList.RemoveAll();

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	hFind = FindFirstFile(filename, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		return FALSE;
	}
	
	FindClose(hFind);

	if( !stdFile.Open(filename, CFile::modeRead, &fileExcept ) )
    {
		fileExcept.ReportError();
		return FALSE;
	}
	while(stdFile.ReadString(sTemp)){
		m_saLineList.Add(sTemp);
	}
	stdFile.Close();
	
	return TRUE;
}
