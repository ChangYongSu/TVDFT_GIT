// SetModelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SetModelDlg.h"
#include <direct.h>

/////////////////////////////////////////////////////////////////////////////
//#include "FolderDlg.h"
#include "Global.h"
//#include "FolderSelDlg.h"
/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//+add kwmoon 080326
extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// CSetModelDlg dialog


CSetModelDlg::CSetModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetModelDlg)
	//}}AFX_DATA_INIT
}


void CSetModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetModelDlg)
	DDX_Control(pDX, IDC_EDIT_REF_IMAGE_FOLDER_PATH, m_ctrlRefImageFolderEdit);
	DDX_Control(pDX, IDC_EDIT_TEST_PARAM_FILE_PATH, m_ctrlTestParamFileEdit);
	DDX_Control(pDX, IDC_EDIT_FULL_REMOTE_FILE_PATH, m_ctrlRemoteFileEdit);
	DDX_Control(pDX, IDC_EDIT_PATTERN_FILE_PATH, m_ctrlPatternFileEdit);
	DDX_Control(pDX, IDC_EDIT_FULL_SEQ_FILE_PATH, m_ctrlFullSeqFileEdit);
	DDX_Control(pDX, IDC_EDIT_SEQ_FILE_PATH, m_ctrlSeqFileEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetModelDlg, CDialog)
	//{{AFX_MSG_MAP(CSetModelDlg)
	ON_BN_CLICKED(IDC_BTN_SEL_SEQ_FILE, OnBtnSelSeqFile)
	ON_BN_CLICKED(IDC_BTN_SEL_FULL_SEQ_FILE, OnBtnSelFullSeqFile)
	ON_BN_CLICKED(IDC_BTN_SEL_PATTERN_FILE, OnBtnSelPatternFile)
	ON_BN_CLICKED(IDC_BTN_SEL_REMOTE_FILE, OnBtnSelRemoteFile)
	ON_BN_CLICKED(IDC_BTN_SEL_TEST_PARAM_FILE, OnBtnSelTestParamFile)
	ON_BN_CLICKED(IDC_BTN_SEL_REF_IMAGE_FOLDER, OnBtnSelRefImageFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetModelDlg message handlers

BOOL CSetModelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// Local PC
	//+add kwmoon 080904
	m_strRefImageFolder		= CurrentSet->sRefFolder;

	// Server
	//+add kwmoon 080904
	m_strServerFolder		= CurrentSet->sServerFolder;

	// Server
	m_strModelFolder		= CurrentSet->sModelFolder;
	m_strSeqFilePath		= CurrentSet->sSeqPath;

	// Server
	m_strFullSeqFilePath	= CurrentSet->sFullSeqPath;
	m_strPatternFilePath	= CurrentSet->sPatternTitle;
	m_strRemoteFilePath		= CurrentSet->sRemoconTitle;
	m_strTestParamFilePath	= CurrentSet->sTestParamIni;

	m_ctrlSeqFileEdit.SetWindowText(m_strSeqFilePath);
	m_ctrlFullSeqFileEdit.SetWindowText(m_strFullSeqFilePath);
	m_ctrlPatternFileEdit.SetWindowText(m_strPatternFilePath);
	m_ctrlRemoteFileEdit.SetWindowText(m_strRemoteFilePath);
	m_ctrlTestParamFileEdit.SetWindowText(m_strTestParamFilePath);

	//+add kwmoon 080904
	m_ctrlRefImageFolderEdit.SetWindowText(m_strRefImageFolder);

	CString sTmp;
	sTmp.Format("Select Model Folder & Sequence File");
	SetWindowText(sTmp);

	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetModelDlg::OnOK() 
{
	//+del kwmoon 080904
/*	g_pView->LoadRegistrySetting(CurrentSet);

	//+del kwmoon 080818
	if(CurrentSet->sModelIni.IsEmpty() == FALSE)
	{
		//+add kwmoon 080821 : Add FileExists Check
		if(FileExists(CurrentSet->sModelIni))
		{
			OpenModelIniFile(CurrentSet->sModelIni);
		}
	}
*/
	//+move kwmoon 080818
/*
	//+add kwmoon 080502
	CurrentSet->sModelFolder	= m_strModelFolder;
	CurrentSet->sSeqPath		= m_strSeqFilePath;
	CurrentSet->sFullSeqPath	= m_strFullSeqFilePath;
	CurrentSet->sPatternTitle	= m_strPatternFilePath;
	CurrentSet->sRemoconTitle	= m_strRemoteFilePath;

	//+add kwmoon 080724
	CurrentSet->sTestParamIni	= m_strTestParamFilePath;

	//+add kwmoon 080326
	g_pView->SaveRegistrySetting();
*/
	CDialog::OnOK();
}

void CSetModelDlg::OnCancel() 
{
	//+add kwmoon 080904
	m_strServerFolder		= _T("");

	m_strModelFolder		= _T("");
	m_strSeqFilePath		= _T("");
	m_strFullSeqFilePath	= _T("");
	m_strPatternFilePath	= _T("");
	m_strRemoteFilePath		= _T("");

	CDialog::OnCancel();
}

//=============================
// Load Sequence File (Server)
//=============================
void CSetModelDlg::OnBtnSelSeqFile() 
{
	CFileDialog dlg(TRUE, "seq", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Sequence File(*.seq)|*.seq|",NULL);

	//+change kwmoon 080904
//	CString sTmp = m_strModelFolder;
	CString sTmp = m_strServerFolder;

	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strSeqFilePath = dlg.GetPathName();
		m_ctrlSeqFileEdit.SetWindowText(m_strSeqFilePath);
		m_strModelFolder = m_strSeqFilePath.Left(m_strSeqFilePath.ReverseFind('\\'));
	}
}

//======================================
// Load Template Sequence File (Server)
//======================================
void CSetModelDlg::OnBtnSelFullSeqFile() 
{
	CFileDialog dlg(TRUE, "seq", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Full Sequence File(*.seq)|*.seq|",NULL);

	//+change kwmoon 080904
//	TCHAR szModulePath[_MAX_PATH] = _T("");
//	GetModuleFileName(NULL,szModulePath,_MAX_PATH);
//	CString sTmp = szModulePath;
//	sTmp = sTmp.Left(sTmp.ReverseFind('\\'));
//	CString sTmp = m_strModelFolder;

	CString sTmp = m_strServerFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strFullSeqFilePath = dlg.GetPathName();
		m_ctrlFullSeqFileEdit.SetWindowText(m_strFullSeqFilePath);
	}
}

//============================
// Load Pattern File (Server)
//============================
void CSetModelDlg::OnBtnSelPatternFile() 
{
	CFileDialog dlg(TRUE, "pat", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Pattern Info File(*.pat)|*.pat|",NULL);

	//+change kwmoon 080904
//	TCHAR szModulePath[_MAX_PATH] = _T("");
//	GetModuleFileName(NULL,szModulePath,_MAX_PATH);
//	CString sTmp = szModulePath;
//	sTmp = sTmp.Left(sTmp.ReverseFind('\\'));
//	CString sTmp = m_strModelFolder;

	CString sTmp = m_strServerFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strPatternFilePath = dlg.GetPathName();
		m_ctrlPatternFileEdit.SetWindowText(m_strPatternFilePath);
	}	
}

//===========================
// Load Remote File (Server)
//===========================
void CSetModelDlg::OnBtnSelRemoteFile() 
{
	CFileDialog dlg(TRUE, "rmt", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Remote Control Info File(*.rmt)|*.rmt|",NULL);

	//+change kwmoon 080904
//	TCHAR szModulePath[_MAX_PATH] = _T("");
//	GetModuleFileName(NULL,szModulePath,_MAX_PATH);
//	CString sTmp = szModulePath;
//	sTmp = sTmp.Left(sTmp.ReverseFind('\\'));
//	CString sTmp = m_strModelFolder;

	CString sTmp = m_strServerFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strRemoteFilePath = dlg.GetPathName();
		m_ctrlRemoteFileEdit.SetWindowText(m_strRemoteFilePath);
	}	
}

//===================================
// Load Test Parameter File (Server)
//===================================
void CSetModelDlg::OnBtnSelTestParamFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "ini", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Test Param File(*.ini)|*.ini|",NULL);

	//+change kwmoon 080904
//	TCHAR szModulePath[_MAX_PATH] = _T("");
//	GetModuleFileName(NULL,szModulePath,_MAX_PATH);
//	CString sTmp = szModulePath;
//	sTmp = sTmp.Left(sTmp.ReverseFind('\\'));
//	CString sTmp = m_strModelFolder;

	CString sTmp = m_strServerFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		m_strTestParamFilePath = dlg.GetPathName();
		m_ctrlTestParamFileEdit.SetWindowText(m_strTestParamFilePath);
	}	
}



void CSetModelDlg::OnBtnSelRefImageFolder() 
{
	CString strFolderPath;

//	TCHAR buffer[_MAX_PATH];

	if (GetFolder(&strFolderPath, "Select Reference Image Foler.", this->m_hWnd, NULL, (const char*)m_strRefImageFolder))
	{
		if (!strFolderPath.IsEmpty()){
			m_strRefImageFolder = strFolderPath;
			m_ctrlRefImageFolderEdit.SetWindowText(m_strRefImageFolder);
			UpdateData(FALSE);
		}
        else
        {
            MessageBox(_T("Invalid Folder Name."), _T("lol"), MB_OKCANCEL|MB_ICONASTERISK ); 
        }
	}
 }
