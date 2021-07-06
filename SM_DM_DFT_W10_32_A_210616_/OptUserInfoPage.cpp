// OptUserInfoPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptUserInfoPage.h"
#include "Global.h"
#include "Defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_nRunningProcessNo;

//+add 090219(Modification No2)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

//+add kwmoon 080522
extern CDATsysView*  g_pView;

//+add kwmoon 080523
extern _LineID LindIDList[];

/////////////////////////////////////////////////////////////////////////////
// COptUserInfoPage property page

IMPLEMENT_DYNCREATE(COptUserInfoPage, CPropertyPage)

COptUserInfoPage::COptUserInfoPage() : CPropertyPage(COptUserInfoPage::IDD)
{
	//{{AFX_DATA_INIT(COptUserInfoPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptUserInfoPage::~COptUserInfoPage()
{
}

void COptUserInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptUserInfoPage)
	DDX_Control(pDX, IDC_EDIT_NEW_PASSWORD, m_ctrlNewPasswordEdit);
	DDX_Control(pDX, IDC_EDIT_CURRENT_PASSWORD, m_ctrlCurrentPasswordEdit);
	DDX_Control(pDX, IDC_EDIT_CONFIRM_PASSWORD, m_ctrlConfirmPasswordEdit);
	DDX_Control(pDX, IDC_CMB_SYSTEM_NO, m_ctrlSystemNoCmb);
	//+del kwmoon 080523
//	DDX_Control(pDX, IDC_CMB_LOCATION, m_ctrlLocationCmb);
	DDX_Control(pDX, IDC_CMB_LINE_NO, m_ctrlLineNoCmb);
	DDX_Control(pDX, IDC_CMB_AUTHORITY, m_ctrlAuthorityCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptUserInfoPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptUserInfoPage)
	ON_BN_CLICKED(IDC_BTN_CHG_PASSWORD, OnBtnChgPassword)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_CBN_SELCHANGE(IDC_CMB_AUTHORITY, OnSelchangeCmbAuthority)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void COptUserInfoPage::InitControl()
{
	//+add 090219(Modification No2)
	CString szLog = "B:COptUserInfoPage-InitControl\n";
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
		//+del kwmoon 080523
	//	m_ctrlLineNoCmb.AddString(sTmp);
		m_ctrlSystemNoCmb.AddString(sTmp);
	}
	
	//+add kwmoon 080523
	if((CurrentSet->nSystemNo > 0)
	&& (CurrentSet->nSystemNo <= 50))
	{
		m_ctrlSystemNoCmb.SetCurSel(CurrentSet->nSystemNo-1);
	}
	else
	{
		m_ctrlSystemNoCmb.SetCurSel(0);
	}

	//+add kwmoon 080523
/*	nTmp = 0;

	while(LindIDList[nTmp].pszLineName != NULL)
	{
		sTmp.Format("%s",LindIDList[nTmp].pszLineName);
		m_ctrlLineNoCmb.AddString(sTmp);

		if(CurrentSet->nLineNo == LindIDList[nTmp].nLineNo) 
		{
			nSelLineNo	 = LindIDList[nTmp].nSeq;
			m_ctrlLineNoCmb.SetCurSel(nSelLineNo);
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

			m_ctrlLineNoCmb.AddString(sBuf);

			if(CurrentSet->nLineNo == nNoList) 
			{
				nSelLineNo	 = i;
				m_ctrlLineNoCmb.SetCurSel(nSelLineNo);
			}
		}
	}

	//+add kwmoon 080523
	m_ctrlLineNoCmb.SetCurSel(nSelLineNo);

	//+change kwmoon 080523
//	m_ctrlLineNoCmb.SetCurSel(CurrentSet->nLineNo);
//	m_ctrlSystemNoCmb.SetCurSel(CurrentSet->nSystemNo);

	//Location 
	//+del kwmoon 080523
//	m_ctrlLocationCmb.ResetContent();
//	m_ctrlLocationCmb.AddString(_T("LG-PRI"));
//	m_ctrlLocationCmb.AddString(_T("LG-DD"));
//	m_ctrlLocationCmb.AddString(_T("JIN SUNG"));
//	m_ctrlLocationCmb.AddString(_T("SG TECH"));

//	m_ctrlLocationCmb.SetCurSel(CurrentSet->nLocation);

	//Password
	m_ctrlCurrentPasswordEdit.SetWindowText(_T(""));
	if((m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_DEV) || 
		(m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_OP))
	{
		m_ctrlCurrentPasswordEdit.EnableWindow(FALSE);
		m_ctrlNewPasswordEdit.EnableWindow(FALSE);
		m_ctrlConfirmPasswordEdit.EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CHG_PASSWORD)->EnableWindow(FALSE);
	}
	else 
	{
		m_ctrlCurrentPasswordEdit.EnableWindow(TRUE);
		m_ctrlNewPasswordEdit.EnableWindow(TRUE);
		m_ctrlConfirmPasswordEdit.EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CHG_PASSWORD)->EnableWindow(TRUE);
	}

	//+add 090219(Modification No2)
	szLog = "E:COptUserInfoPage-InitControl\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
}

//+del kwmoon 080523
/*
CString COptUserInfoPage::MakeUserID()
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
BOOL COptUserInfoPage::CheckPassword()
{
	CString sTmp;
	m_ctrlCurrentPasswordEdit.GetWindowText(sTmp);
	sTmp.MakeUpper(); sTmp.TrimLeft(); sTmp.TrimRight();

	if((m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_DEV) ||
		(m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_OP)) return TRUE;

	else if(m_ctrlAuthorityCmb.GetCurSel() == AUTHORITY_ADMIN)
	{
		if(sTmp != CurrentSet->strPassword)
		{
			AfxMessageBox("Password is Wrong!");
			return FALSE;
		}
		else return TRUE;
	}
	else  

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COptUserInfoPage message handlers

void COptUserInfoPage::OnBtnChgPassword() 
{
	CString sTmp1, sTmp2;

	if(CheckPassword())
	{
		m_ctrlNewPasswordEdit.GetWindowText(sTmp1);
		m_ctrlConfirmPasswordEdit.GetWindowText(sTmp2);
		sTmp1.MakeUpper(); sTmp1.TrimRight(); sTmp1.TrimLeft();
		sTmp2.MakeUpper(); sTmp2.TrimRight(); sTmp2.TrimLeft();
		if(sTmp1 == sTmp2)
		{
			AfxMessageBox("Password Changing is Success!");
			CurrentSet->strPassword.Format("%s", sTmp1);
		}
		else 
		{
			AfxMessageBox("New Password & Confirm Password is not Equal!");
		}
	}
	else 
	{
		AfxMessageBox("Current Password is Wrong!");
		return;
	}
}

void COptUserInfoPage::OnBtnApply() 
{
	//+add 090219(Modification No2)
	CString szLog = "B:OnBtnApply\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
	
	CString sTemp;
	CString sBuf;
	CString sBuf2;
	CStringArray SplitIt;
	int nNoList;

	CurrentSet->nAuthority  = m_ctrlAuthorityCmb.GetCurSel();

	//+change kwmoon 080523
//	CurrentSet->nLineNo		= m_ctrlLineNoCmb.GetCurSel();
	int nCurSel = m_ctrlLineNoCmb.GetCurSel();
	
	CString szLineID = _T("");
	m_ctrlLineNoCmb.GetLBText(nCurSel,szLineID);

	//+add kwmoon 080523
/*	int nTmp = 0;

	while(LindIDList[nTmp].pszLineName != NULL)
	{
		if(szLineID.Compare(LindIDList[nTmp].pszLineName) == NULL)
		{
			CurrentSet->nLineNo	= LindIDList[nTmp].nLineNo; break;
		}
		nTmp++;
	}
*/
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

	CurrentSet->nSystemNo   = m_ctrlSystemNoCmb.GetCurSel()+1;
	g_pView->SaveRegistrySetting();

	//+add 090219(Modification No2)
	szLog = "E:OnBtnApply\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
}

BOOL COptUserInfoPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	//+change 090206(Modification No2)
//	LineList_Read();
	if(!LineList_Read())
	{
		AfxMessageBox("Failed to open PCBA_LineList.ini!\r\nApplication will be closed"); 

		AfxGetMainWnd()->PostMessage(WM_CLOSE);
	}
	else
	{
		InitControl();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptUserInfoPage::OnSelchangeCmbAuthority() 
{
	m_ctrlCurrentPasswordEdit.EnableWindow(TRUE);
	m_ctrlNewPasswordEdit.EnableWindow(TRUE);
	m_ctrlConfirmPasswordEdit.EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CHG_PASSWORD)->EnableWindow(TRUE);

	switch(m_ctrlAuthorityCmb.GetCurSel())
	{
		
		case AUTHORITY_ADMIN:
			{
				m_ctrlCurrentPasswordEdit.EnableWindow(TRUE);
				m_ctrlCurrentPasswordEdit.SetWindowText(CurrentSet->strPassword);
				m_ctrlNewPasswordEdit.EnableWindow(TRUE);
				m_ctrlConfirmPasswordEdit.EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_CHG_PASSWORD)->EnableWindow(TRUE);
			}
			break;
		case AUTHORITY_DEV:
		case AUTHORITY_OP:
			{
				m_ctrlCurrentPasswordEdit.EnableWindow(FALSE);
				m_ctrlNewPasswordEdit.EnableWindow(FALSE);
				m_ctrlConfirmPasswordEdit.EnableWindow(FALSE);
				GetDlgItem(IDC_BTN_CHG_PASSWORD)->EnableWindow(FALSE);
			}
			break;
	}
}

//+add kwmoon 080502
void COptUserInfoPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnApply();

	CPropertyPage::OnOK();
}


BOOL COptUserInfoPage::LineList_Read()
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
