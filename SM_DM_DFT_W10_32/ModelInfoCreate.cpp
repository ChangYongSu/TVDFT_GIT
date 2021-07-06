// ModelInfoCreate.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ModelInfoCreate.h"
#include "global.h"
#include "ModelInfoCreate_GraberOption.h"
#include "AnalogGrabber.h"
#include "AvSwitchBoxCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*			g_pView;
extern CBaseImage	g_GrabImage;
extern CAvSwitchBox			AvSwitchBoxCtrl;

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
	BOOL bGrabFlag;

	CFileDialog dlg(TRUE, "ini", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Model Info File(*.ini)|*.ini|",NULL);

	dlg.m_ofn.lpstrInitialDir = CurrentSet->sModelInfoFolder;

	if(dlg.DoModal() == IDOK) 
	{
		sTemp = dlg.GetPathName();

		OpenModelIniFile(sTemp);

		AvSwitchBoxCtrl.SetVideoOutType(CurrentSet->nAnalogType);
		
		bGrabFlag = g_pView->m_bGrabThreadRunning;
		g_pView->StopGrabThread();
		Sleep(500);
		AnalogControl.SetVideoSourceType(CurrentSet->nAnalogType);
		Sleep(500);
		if(bGrabFlag) g_pView->StartGrabThread();

		g_pView->SetGrabInfo(&g_GrabImage);

		CreateModelIniFile(CurrentSet->sModelIni);
	
		CDialog::OnOK();
	}
}

void CModelInfoCreate::OnCancel() 
{
	BOOL bGrabFlag;
	CModelInfoCreate_GraberOption dlg;
				
	dlg.DoModal();

	AvSwitchBoxCtrl.SetVideoOutType(CurrentSet->nAnalogType);
	
	bGrabFlag = g_pView->m_bGrabThreadRunning;
	g_pView->StopGrabThread();
	Sleep(500);
	AnalogControl.SetVideoSourceType(CurrentSet->nAnalogType);
	Sleep(500);
	if(bGrabFlag) g_pView->StartGrabThread();

	g_pView->SetGrabInfo(&g_GrabImage);

	//+add psh 100310
	CurrentSet->nHDMIResolution = 0;
	CurrentSet->nHDMIBitSpec = 0;
	//-
	CurrentSet->sBE_Version		  = "";
	CurrentSet->sFE_Version		  = "";
	CurrentSet->sMicom_Version	  = "";
	CurrentSet->sDSP_Version		  = "";
	CurrentSet->sSubDSP_Version	  = "";
	CurrentSet->sEQ_Version	      = "";
	CurrentSet->sMEQ_Version	      = "";
	CurrentSet->sTouch_Version      = "";
	CurrentSet->sDemo_Version	      = "";
	CurrentSet->sDJSound_Version    = "";
	CurrentSet->sWirelessTx_Version = "";
	CurrentSet->sWirelessRx_Version = "";
	CurrentSet->sBT_Version = "";
	CurrentSet->sHDMI_Version = "";
	CurrentSet->sChecksum	          = "";

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
