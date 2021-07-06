// OptMesPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptMesPage.h"
#include "Gmes.h"
#include "PLC_Ctrl.h"
#include "JigCtl.h""
#include "global.h"
#include "Server_Ctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//+add kwmoon 080522
extern CDATsysView*  g_pView;
//extern CMesDb		m_MesDb;
extern CColorStaticST	ctrlPLC_COM_Status;

/////////////////////////////////////////////////////////////////////////////
// COptMesPage property page

IMPLEMENT_DYNCREATE(COptMesPage, CPropertyPage)

COptMesPage::COptMesPage() : CPropertyPage(COptMesPage::IDD)
{
	//{{AFX_DATA_INIT(COptMesPage)

	//}}AFX_DATA_INIT
}

COptMesPage::~COptMesPage()
{
}

void COptMesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptMesPage)
	DDX_Control(pDX, IDC_CHK_USE_GMES, m_ctrlUseGmesChk);
	DDX_Control(pDX, IDC_EDIT_MES_HOSP_IP, m_ctrlHostIP);
	DDX_Control(pDX, IDC_EDIT_MES_HOST_PORT, m_ctrlHostPort);
	DDX_Control(pDX, IDC_CHK_UPLOAD_DATA_MES, m_ctrlUploadMesChk);
	DDX_Control(pDX, IDC_CHK_AUTO_UPLOAD_MES, m_ctrlAutoUploadMesChk);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_PLC_HOSP_IP, m_ctrlPLCHostIP);
	DDX_Control(pDX, IDC_EDIT_PLC_HOST_PORT, m_ctrlPLCHostPort);
	DDX_Control(pDX, IDC_COMBO_DFT_SEL, m_cComboDFTSel);
//	DDX_Control(pDX, IDC_CHK_AUTO_PLC_ROBOT, m_cCheckAutoPlc);
	DDX_Control(pDX, IDC_EDIT_MONITOR_SERVER_IP, m_ctrlMonitorServerIP);
	DDX_Control(pDX, IDC_EDIT_MONITOR_SERVER_PORT, m_ctrlMonitorServerPort);
	DDX_Control(pDX, IDC_CHK_SERVER_CTRL_CONNECT, m_cCheckServerCtrlConnect);
}


BEGIN_MESSAGE_MAP(COptMesPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptMesPage)
	ON_BN_CLICKED(IDC_BTN_MES_CONNECT_TEST, OnBtnMesConnectTest)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_CHK_USE_GMES, OnChkUseGmes)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_PLC_CONNECT_TEST, &COptMesPage::OnBnClickedBtnPlcConnectTest)
//	ON_BN_CLICKED(IDC_CHK_AUTO_PLC_ROBOT, &COptMesPage::OnBnClickedChkAutoPlcRobot)
	ON_CBN_SELCHANGE(IDC_COMBO_DFT_SEL, &COptMesPage::OnCbnSelchangeComboDftSel)
	ON_BN_CLICKED(IDC_BTN_MONITOR_SERVER_CONNECT_TEST, &COptMesPage::OnBnClickedBtnMonitorServerConnectTest)
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
	//else{
	//	sRtn.LoadString(IDS_MES_MSG2);
	//}
//	if(m_MesDb.m_bDBconnection) m_MesDb.m_pConn->Close();
//	bRtn = m_pDb.Mes_Open_Test(m_strMesDbDsn, m_strMesDbUserID, m_strMesDbPassWD, sRtn);
//	bRtn = m_pDb.Mes_Open_Test(m_strGmesIP, nPort, sRtn);


	//m_ctrlPLCHostIP.GetWindowText(m_strPLCIP);
	//m_ctrlPLCHostPort.GetWindowText(sMsg);
	//nPort = _ttoi((LPCTSTR)sMsg);						//CString 을 숫자로 변환

	//if (gPLC_Ctrl.m_bPLCCom) {
	//	gPLC_Ctrl.SoketCom_Close();
	//}

	////	if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
	//if (gPLC_Ctrl.SoketCom_Creat(m_strPLCIP, nPort)) {
	//	sRtn.LoadString(IDS_MES_MSG1);
	//}


	//AfxMessageBox(sRtn,MB_OK|MB_ICONINFORMATION);
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

	m_ctrlUploadMesChk.SetCheck(CurrentSet->bUploadMes);
	m_ctrlUseGmesChk.SetCheck(CurrentSet->bUseGmes);


	sPort.Format("%d", CurrentSet->nPLC_Port);
	m_ctrlPLCHostIP.SetWindowText(CurrentSet->strPLC_IP);
	m_ctrlPLCHostPort.SetWindowText(sPort);


	m_cComboDFTSel.SetCurSel(CurrentSet->nDFT_No);
	 
	//m_cCheckAutoPlc.SetCheck(CurrentSet->bUsePLCRobot);
	m_cCheckServerCtrlConnect.SetCheck(CurrentSet->bUseServerCtrlConnect);

	sPort.Format("%d", CurrentSet->nMonitorServer_Port);
	m_ctrlMonitorServerIP.SetWindowText(CurrentSet->strMonitorServer_IP);
	m_ctrlMonitorServerPort.SetWindowText(sPort);

	//strPL_CIP;		// GMES Host IP Add
	//UINT		nPLC_Port;
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
	CurrentSet->bUseGmes			= m_ctrlUseGmesChk.GetCheck();

	if(CurrentSet->bUploadMes)
	{
		if(gGmesCtrl.m_bGmesCom){
			gGmesCtrl.SoketCom_Close();
			CurrentSet->bGMES_Connection = FALSE;
		}
		
		Sleep(500);
		if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
			sTmp = gGmesCtrl.MakeElem_S1F1();
			CurrentSet->bGMES_Connection = TRUE;
			if(gGmesCtrl.SendCommString(sTmp))
			{
				gGmesCtrl.DisplyComStatus("S1F1", FALSE);
			}
			g_pView->UpdateInfo(LED_STATE,"");
		}
		else{
			sRtn.LoadString(IDS_MES_MSG2);
			AfxMessageBox(sRtn,MB_OK|MB_ICONINFORMATION);
		}	//+add kwmoon 080522
	}



	m_ctrlPLCHostIP.GetWindowText(sTmp);
	sTmp.MakeUpper();
	CurrentSet->strPLC_IP = sTmp;
	m_ctrlPLCHostPort.GetWindowText(sTmp);
	port = _ttoi((LPCTSTR)sTmp);						//CString 을 숫자로 변환
	CurrentSet->nPLC_Port = port;
	CurrentSet->nDFT_No = m_cComboDFTSel.GetCurSel();
	//CurrentSet->bUsePLCRobot = m_cCheckAutoPlc.GetCheck();

	if (gPLC_Ctrl.m_bPLCCom) {
		gPLC_Ctrl.SoketCom_Close();
		//CurrentSet->bGMES_Connection = FALSE;
	}
	if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == 1))
	{

		Sleep(500);
		if (gPLC_Ctrl.SoketCom_Creat(CurrentSet->strPLC_IP, CurrentSet->nPLC_Port))
		{
			ctrlPLC_COM_Status.SetWindowText(_T("PLC ON"));
			//if (CurrentSet->bUsePLCRobot)
			{
				ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
				ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
			}
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_PLC_ON;
		}
		else
		{
			ctrlPLC_COM_Status.SetWindowText(_T("PLC OFF"));
			ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
			ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
			//	sRtn.LoadString(IDS_MES_MSG2);
			AfxMessageBox("PLC Connect Fail", MB_OK | MB_ICONINFORMATION);
		}	//+add kwmoon 080522


	}

	CurrentSet->bUseServerCtrlConnect = m_cCheckServerCtrlConnect.GetCheck();
	
	m_ctrlMonitorServerIP.GetWindowText(sTmp);
	sTmp.MakeUpper();
	CurrentSet->strMonitorServer_IP = sTmp;
	m_ctrlMonitorServerPort.GetWindowText(sTmp);
	port = _ttoi((LPCTSTR)sTmp);						//CString 을 숫자로 변환
	CurrentSet->nMonitorServer_Port = port;

	if (gServer_Ctrl.m_bServer_CtrlCom) {
		gServer_Ctrl.SoketCom_Close();
	}
	if (CurrentSet->bUseServerCtrlConnect == 1)
	{
		if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port)) {
			
			CurrentSet->bMonitorServer_Connection = TRUE;
		}
		else
		{
			CurrentSet->bMonitorServer_Connection = FALSE;
		}
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

void COptMesPage::OnChkUseGmes() 
{
	if(CurrentSet->bUseGmes){
		CurrentSet->bUseGmes = FALSE;
	}
	else{
		CurrentSet->bUseGmes = TRUE;
	}
	m_ctrlUseGmesChk.SetCheck(CurrentSet->bUseGmes);
	
}


void COptMesPage::OnBnClickedBtnPlcConnectTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sMsg, sRtn;
	CString sTmp;
	int nPort;

	

	m_ctrlPLCHostIP.GetWindowText(m_strPLCIP);
	m_ctrlPLCHostPort.GetWindowText(sMsg);
	nPort = _ttoi((LPCTSTR)sMsg);						//CString 을 숫자로 변환

	if (gPLC_Ctrl.m_bPLCCom) {
		gPLC_Ctrl.SoketCom_Close();
	}

	//	if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
	if (gPLC_Ctrl.SoketCom_Creat(m_strPLCIP, nPort)) {
		//sRtn.LoadString(IDS_MES_MSG1);
	}


	//AfxMessageBox(sRtn, MB_OK | MB_ICONINFORMATION);
}


//void COptMesPage::OnBnClickedChkAutoPlcRobot()
//{
//	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//if (CurrentSet->bUsePLCRobot == TRUE)
//	//{
//	//	CurrentSet->bUsePLCRobot = FALSE;
//	//}
//	//else
//	//{
//	//	CurrentSet->bUsePLCRobot = TRUE;
//	//}
//
//	//m_cCheckAutoPlc.SetCheck(CurrentSet->bUsePLCRobot);
//	//m_cCheckServerCtrlConnect.SetCheck(CurrentSet->bUseServerCtrlConnect);
//}
//

void COptMesPage::OnCbnSelchangeComboDftSel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*m_ctrlPLCHostIP.GetWindowText(sTmp);
	sTmp.MakeUpper();*/
	//CurrentSet->strPLC_IP = sTmp;
	//m_ctrlPLCHostPort.SetWindowText(sTmp);
	//port = _ttoi((LPCTSTR)sTmp);						//CString 을 숫자로 변환
	//CurrentSet->nPLC_Port = port;
	//CurrentSet->nDFT_No = 
	//CurrentSet->bUsePLCRobot = m_cCheckAutoPlc.GetCheck();
	int lDftNo = m_cComboDFTSel.GetCurSel();
	CString sPort;
	sPort.Format("%d", 2000+ lDftNo);	
	m_ctrlPLCHostPort.SetWindowText(sPort);
}


void COptMesPage::OnBnClickedBtnMonitorServerConnectTest()
{
	CString sMsg, sRtn;
	CString sTmp;
	int nPort;

	//CString	m_strServerIP;
	//UINT	m_nServerPort;


	m_ctrlMonitorServerIP.GetWindowText(m_strServerIP);
	m_ctrlMonitorServerPort.GetWindowText(sMsg);
	nPort = _ttoi((LPCTSTR)sMsg);						//CString 을 숫자로 변환

	if (gServer_Ctrl.m_bServer_CtrlCom) {
		gServer_Ctrl.SoketCom_Close();
	}

	

	if (gServer_Ctrl.SoketCom_Creat(m_strServerIP, nPort)) {
		//sMsg += "PASS";
		////	sTmp = gPLC_Ctrl.MakeElem_S1F1();
		//CurrentSet->bMonitorServer_Connection = TRUE;
	}

}
