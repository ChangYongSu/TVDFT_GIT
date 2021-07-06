// OptMesPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptMesPage.h"
#include "Gmes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//+add kwmoon 080522
extern CDATsysView*  g_pView;
//extern CMesDb		m_MesDb;

/////////////////////////////////////////////////////////////////////////////
// COptMesPage property page

IMPLEMENT_DYNCREATE(COptMesPage, CPropertyPage)

COptMesPage::COptMesPage() : CPropertyPage(COptMesPage::IDD)
{
	//{{AFX_DATA_INIT(COptMesPage)
	m_nModelInfo_Check = -1;
	//}}AFX_DATA_INIT
}

COptMesPage::~COptMesPage()
{
}

void COptMesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptMesPage)
	DDX_Control(pDX, IDC_EDIT_MES_HOSP_IP, m_ctrlHostIP);
	DDX_Control(pDX, IDC_EDIT_MES_HOST_PORT, m_ctrlHostPort);
	DDX_Control(pDX, IDC_CHK_UPLOAD_DATA_MES, m_ctrlUploadMesChk);
	DDX_Control(pDX, IDC_CHK_AUTO_UPLOAD_MES, m_ctrlAutoUploadMesChk);
	DDX_Radio(pDX, IDC_RDO_MODELCHECK_TYPE1, m_nModelInfo_Check);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHK_AUTO_CHANGE_MODEL, m_ctrlAutoGMES_ChangeModel);
}


BEGIN_MESSAGE_MAP(COptMesPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptMesPage)
	ON_BN_CLICKED(IDC_BTN_MES_CONNECT_TEST, OnBtnMesConnectTest)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptMesPage message handlers

void COptMesPage::OnBtnMesConnectTest() 
{
//	bool bRtn;
	CString sMsg, sRtn;
	CString sTmp;
	int nPort;

	m_ctrlHostIP.GetWindowText(m_strGmesIP);
	m_ctrlHostPort.GetWindowText(sMsg);
	nPort = _ttoi((LPCTSTR)sMsg);						//CString 을 숫자로 변환

	if(gGmesCtrl.m_bGmesCom){
		gGmesCtrl.SoketCom_Close();
	}

//	if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
	if(gGmesCtrl.SoketCom_Creat(m_strGmesIP, nPort)){
		sRtn.LoadString(IDS_MES_MSG1);
	}
	else{
		sRtn.LoadString(IDS_MES_MSG2);
	}
//	if(m_MesDb.m_bDBconnection) m_MesDb.m_pConn->Close();
//	bRtn = m_pDb.Mes_Open_Test(m_strMesDbDsn, m_strMesDbUserID, m_strMesDbPassWD, sRtn);
//	bRtn = m_pDb.Mes_Open_Test(m_strGmesIP, nPort, sRtn);

	AfxMessageBox(sRtn,MB_OK|MB_ICONINFORMATION);
}
/////////////////////////////////////////////////////////////////////////////
BOOL COptMesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString sPort;

	sPort.Format("%d", CurrentSet->nGmesPort);
	m_ctrlHostIP.SetWindowText(CurrentSet->strGmesIP);
	m_ctrlHostPort.SetWindowText(sPort);
	m_ctrlAutoUploadMesChk.SetCheck(CurrentSet->bAutoUploadMes);
	m_ctrlAutoGMES_ChangeModel.SetCheck(CurrentSet->bAutoGMES_ChangeModel);
	

	m_ctrlUploadMesChk.SetCheck(CurrentSet->bUploadMes);

	m_nModelInfo_Check = CurrentSet->nModelInfo_Check;

	UpdateData(FALSE);

/*	if(CurrentSet->bUseScanner)
	{
		m_ctrlAutoUploadMesChk.EnableWindow();
		m_ctrlUploadMesChk.EnableWindow();
	}
	else
	{
		m_ctrlAutoUploadMesChk.EnableWindow(FALSE);
		m_ctrlUploadMesChk.EnableWindow(FALSE);
	}
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptMesPage::OnBtnApply() 
{
	CString sTmp = _T("");
	CString sRtn;
	
	m_ctrlHostIP.GetWindowText(sTmp);
	sTmp.MakeUpper();
	CurrentSet->strGmesIP = sTmp;
	m_ctrlHostPort.GetWindowText(sTmp);
	int port = _ttoi((LPCTSTR)sTmp);						//CString 을 숫자로 변환
	CurrentSet->nGmesPort = port;
	
	CurrentSet->bAutoUploadMes  = m_ctrlAutoUploadMesChk.GetCheck();
	CurrentSet->bUploadMes		= m_ctrlUploadMesChk.GetCheck();
	CurrentSet->bAutoGMES_ChangeModel = m_ctrlAutoGMES_ChangeModel.GetCheck();

	CurrentSet->nModelInfo_Check = m_nModelInfo_Check;

	if(CurrentSet->bUploadMes)
	{
		if(gGmesCtrl.m_bGmesCom){
			gGmesCtrl.SoketCom_Close();
		}

		if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
			sTmp = gGmesCtrl.MakeElem_S1F1();
			if(gGmesCtrl.SendCommString(sTmp))
			{
				gGmesCtrl.DisplyComStatus("S1F1", FALSE);
			}
		}
		else{
			sRtn.LoadString(IDS_MES_MSG2);
			AfxMessageBox(sRtn,MB_OK|MB_ICONINFORMATION);
		}	//+add kwmoon 080522
	}
	g_pView->SaveRegistrySetting();
}

//+add kwmoon 080502
void COptMesPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnApply() ;

	CPropertyPage::OnOK();
}

BOOL COptMesPage::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
            return TRUE;
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}
