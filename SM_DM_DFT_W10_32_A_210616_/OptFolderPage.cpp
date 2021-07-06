// OptFolderPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptFolderPage.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptFolderPage property page

IMPLEMENT_DYNCREATE(COptFolderPage, CPropertyPage)

COptFolderPage::COptFolderPage() : CPropertyPage(COptFolderPage::IDD)
{
	//{{AFX_DATA_INIT(COptFolderPage)
	m_szPatternFilePath = _T("");
	m_szRemoteFilePath = _T("");
	m_szTemplateSeqPath = _T("");
	m_szTestParamPath = _T("");
	m_szModelListPath = _T("");
	m_strServerFolder = _T("");
	m_strRefImageFolder = _T("");
	m_strModelInfoFolder = _T("");
	//}}AFX_DATA_INIT
}

COptFolderPage::~COptFolderPage()
{
}

void COptFolderPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptFolderPage)
	DDX_Text(pDX, IDC_EDIT_PATTERN_PATH, m_szPatternFilePath);
	DDX_Text(pDX, IDC_EDIT_REMOTE_PATH, m_szRemoteFilePath);
	DDX_Text(pDX, IDC_EDIT_TEMPLATE_SEQ, m_szTemplateSeqPath);
	DDX_Text(pDX, IDC_EDIT_TEST_PARAM_PATH, m_szTestParamPath);
	DDX_Text(pDX, IDC_EDIT_MODEL_LIST, m_szModelListPath);
	DDX_Text(pDX, IDC_EDIT_SERVER_FOLDER, m_strServerFolder);
	DDX_Text(pDX, IDC_EDIT_REFIMAGE_FOLDER, m_strRefImageFolder);
	DDX_Text(pDX, IDC_EDIT_MODELINFO_FOLDER, m_strModelInfoFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptFolderPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptFolderPage)
	ON_BN_CLICKED(IDC_BTN_OPEN_PATTERN, OnBtnOpenPattern)
	ON_BN_CLICKED(IDC_BTN_OPEN_REMOTE, OnBtnOpenRemote)
	ON_BN_CLICKED(IDC_BTN_OPEN_TEST_PARAM, OnBtnOpenTestParam)
	ON_BN_CLICKED(IDC_BTN_OPEN_TEMPLATE_SEQ, OnBtnOpenTemplateSeq)
	ON_BN_CLICKED(IDC_BTN_OPEN_MODEL_LIST, OnBtnOpenModelList)
	ON_BN_CLICKED(IDC_BTN_OPEN_SERVER_FOLDER, OnBtnOpenServerFolder)
	ON_BN_CLICKED(IDC_BTN_OPEN_REF_FOLDER, OnBtnOpenRefFolder)
	ON_BN_CLICKED(IDC_BTN_OPEN_MODELINFO_FOLDER, OnBtnOpenModelinfoFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptFolderPage message handlers

void COptFolderPage::OnBtnOpenPattern() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "pat", NULL, 
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"PAT File(*.pat)|*.pat|", NULL);

	if(dlg.DoModal() == IDOK)
	{
		m_szPatternFilePath		  = dlg.GetPathName();
		CurrentSet->sPatternTitle = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void COptFolderPage::OnBtnOpenRemote() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "rmt", NULL, 
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"RMT File(*.rmt)|*.rmt|", NULL);

	if(dlg.DoModal() == IDOK)
	{
		m_szRemoteFilePath		  = dlg.GetPathName();
		CurrentSet->sRemoconTitle = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void COptFolderPage::OnBtnOpenTestParam() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "ini", NULL, 
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"INI File(*.ini)|*.ini|", NULL);

	if(dlg.DoModal() == IDOK)
	{
		m_szTemplateSeqPath		  = dlg.GetPathName();
		CurrentSet->sTestParamIni = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void COptFolderPage::OnBtnOpenTemplateSeq() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "seq", NULL, 
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"SEQ File(*.seq)|*.seq|", NULL);

	if(dlg.DoModal() == IDOK)
	{
		m_szTemplateSeqPath		 = dlg.GetPathName();
		CurrentSet->sFullSeqPath = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

BOOL COptFolderPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_szPatternFilePath	= CurrentSet->sPatternTitle;
	m_szRemoteFilePath	= CurrentSet->sRemoconTitle;
	m_szTemplateSeqPath = CurrentSet->sFullSeqPath;
	m_szTestParamPath	= CurrentSet->sTestParamIni;
	m_szModelListPath	= CurrentSet->sModelListPath;

	m_strServerFolder = CurrentSet->sServerFolder;
	m_strRefImageFolder = CurrentSet->sRefRootFolder;
	m_strModelInfoFolder = CurrentSet ->sModelInfoFolder;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptFolderPage::OnBtnOpenModelList() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "ini", NULL, 
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"INI File(*.ini)|*.ini|", NULL);

	if(dlg.DoModal() == IDOK)
	{
		m_szModelListPath		   = dlg.GetPathName();
		CurrentSet->sModelListPath = dlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void COptFolderPage::OnBtnOpenServerFolder() 
{
	CString strFolderPath;

	if (GetFolder(&strFolderPath, "Select Sequence Server Foler.", this->m_hWnd, NULL, (const char*)m_strServerFolder))
	{
		if (!strFolderPath.IsEmpty())
		{
			m_strServerFolder = strFolderPath;
			CurrentSet->sServerFolder = strFolderPath;

		}
        else
        {
            MessageBox(_T("Invalid Folder Name."), _T("lol"), MB_OKCANCEL|MB_ICONASTERISK ); 
        }
		UpdateData(FALSE);
	}
}

void COptFolderPage::OnBtnOpenRefFolder() 
{
	CString strFolderPath;

	if (GetFolder(&strFolderPath, "Reference Image Root Foler.", this->m_hWnd, NULL, (const char*)m_strRefImageFolder))
	{
		if (!strFolderPath.IsEmpty())
		{
			m_strRefImageFolder = strFolderPath;
			CurrentSet->sRefRootFolder = strFolderPath;

		}
        else
        {
            MessageBox(_T("Invalid Folder Name."), _T("lol"), MB_OKCANCEL|MB_ICONASTERISK ); 
        }
		UpdateData(FALSE);
	}
}

void COptFolderPage::OnBtnOpenModelinfoFolder() 
{
	CString strFolderPath;

	if (GetFolder(&strFolderPath, "Model Info  Root Foler.", this->m_hWnd, NULL, (const char*)m_strModelInfoFolder))
	{
		if (!strFolderPath.IsEmpty())
		{
			m_strModelInfoFolder = strFolderPath;
			CurrentSet->sModelInfoFolder = strFolderPath;

		}
        else
        {
            MessageBox(_T("Invalid Folder Name."), _T("lol"), MB_OKCANCEL|MB_ICONASTERISK ); 
        }
		UpdateData(FALSE);
	}
}
