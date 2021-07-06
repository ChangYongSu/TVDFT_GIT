// ModelInfoCreate.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ModelInfoCreate.h"
#include "global.h"
#include "ModelInfoCreate_GraberOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*			g_pView;
extern CBaseImage	g_GrabDisplayImage;
extern CBaseImage	g_GrabImage;

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate dialog


CModelInfoCreate::CModelInfoCreate(CWnd* pParent /*=NULL*/)
	: CDialog(CModelInfoCreate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelInfoCreate)
	m_szInfoFile = _T("");
	//}}AFX_DATA_INIT
}


void CModelInfoCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelInfoCreate)
	DDX_Text(pDX, IDC_INFOFILE, m_szInfoFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModelInfoCreate, CDialog)
	//{{AFX_MSG_MAP(CModelInfoCreate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate message handlers

void CModelInfoCreate::OnOK() 
{
	CString sTemp;

	CFileDialog dlg(TRUE, "ini", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Model Info File(*.ini)|*.ini|",NULL);

	dlg.m_ofn.lpstrInitialDir = CurrentSet->sModelInfoFolder;

	if(dlg.DoModal() == IDOK) 
	{
		sTemp = dlg.GetPathName();

		OpenModelIniFile(sTemp, g_pView->m_szExePath);
		if(g_nGrabberType == FHD_GRABBER)
		{
			g_pView->SetGrabInfo(&g_GrabImage);
		}
		CreateModelIniFile(CurrentSet->sModelIni);


	
		CDialog::OnOK();
	}
}

void CModelInfoCreate::OnCancel() 
{
	CModelInfoCreate_GraberOption dlg;
				
	dlg.DoModal();

	if(g_nGrabberType == FHD_GRABBER)
	{

		g_pView->SetGrabInfo(&g_GrabImage);

		CurrentSet->nHDMIResolution = 0;
		CurrentSet->nHDMIBitSpec = 0;
	}

	//	CurrentSet->n3DGrabbingMode = 0;
	CreateModelIniFile(CurrentSet->sModelIni);
	
	CDialog::OnCancel();
}

BOOL CModelInfoCreate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString sTemp;

	UpdateData(TRUE);

	sTemp.Format("[%s]",  m_sModelInfo_FileName);

	m_szInfoFile = sTemp;

	UpdateData(FALSE);
	
	return TRUE;  
}
