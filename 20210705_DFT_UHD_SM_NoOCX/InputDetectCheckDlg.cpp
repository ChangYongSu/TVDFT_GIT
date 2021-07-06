// InputDetectCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "InputDetectCheckDlg.h"
#include "Global.h"
#include "TvCommCtrl.h"
#include "step.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// CInputDetectCheckDlg dialog


CInputDetectCheckDlg::CInputDetectCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDetectCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDetectCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInputCheckResult = FALSE;
	m_bActivate = FALSE;
	m_bTestRunning = FALSE;
}


void CInputDetectCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDetectCheckDlg)
	DDX_Control(pDX, IDC_INPUTCHECK_GRID, m_ctrlInputCheckGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDetectCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDetectCheckDlg)
	ON_MESSAGE (WM_JOYSTICK_MSG, OnProcessMessage)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ENTER, OnBtnEnter)
	ON_BN_CLICKED(IDC_BTN_RECHECK, OnBtnRecheck)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_STOP, &CInputDetectCheckDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDetectCheckDlg message handlers
long CInputDetectCheckDlg::OnProcessMessage(WPARAM wParam, LPARAM lParam)
{
	if(g_nRunningProcessNo == 1)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
				break;
			case MSG_STOP_BTN1:
				//CurrentSet->bRunAbort = TRUE;
				//PostMessage(WM_COMMAND,IDOK);
				PostMessage(WM_COMMAND, IDC_BTN_STOP);
				break;
				
			case MSG_RETRY_BTN1:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDC_BTN_RECHECK);
				}
				break;

			case MSG_OK_BTN1:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;

			case MSG_NG_BTN1:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
				
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_OK_BTN2:
			case MSG_NG_BTN2:
				break;
		}
	}
	else if(g_nRunningProcessNo == 2)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
			case MSG_STOP_BTN1: 
			case MSG_RETRY_BTN1:
			case MSG_OK_BTN1:
			case MSG_NG_BTN1:
			case MSG_START_BTN2:
				break;
			case MSG_STOP_BTN2:
				CurrentSet->bRunAbort = TRUE;
				PostMessage(WM_COMMAND,IDOK);
				break;

			case MSG_RETRY_BTN2:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDC_BTN_RECHECK);
				}
				break;
			case MSG_OK_BTN2:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
			case MSG_NG_BTN2:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
		}
	}
	else if(g_nRunningProcessNo == 3)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
			case MSG_STOP_BTN1: 
			case MSG_RETRY_BTN1:
			case MSG_OK_BTN1:
			case MSG_NG_BTN1:
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_OK_BTN2:
			case MSG_NG_BTN2:
			case MSG_START_BTN3:
				break;
			case MSG_STOP_BTN3:
				CurrentSet->bRunAbort = TRUE;
				PostMessage(WM_COMMAND,IDOK);
				break;

			case MSG_RETRY_BTN3:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDC_BTN_RECHECK);
				}
				break;
			case MSG_OK_BTN3:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
			case MSG_NG_BTN3:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
		}
	}	return 1;
}
	//-
BOOL CInputDetectCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rect;
	GetWindowRect(&rect);

	int nLeft   = g_pView->m_FormViewRect.left + (g_pView->m_FormViewRect.Width()/2)  - (rect.Width()/2);
	int nTop    = (g_pView->m_FormViewRect.Height()/2) - (rect.Height()/2);
	int nWidth  = rect.Width();
	int nHeight = rect.Height();

	MoveWindow(nLeft,nTop,nWidth,nHeight);
	m_bInputCheckWait = 0;
	m_bInputCheckResult = FALSE;
	InitInputCheckGrid();

	m_bActivate = TRUE;
	m_nInputCheckCount = 0;
	memset(m_bInputDetectResultList, 0, sizeof(m_bInputDetectResultList));

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInputDetectCheckDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}
void CInputDetectCheckDlg::InitInputCheckGrid()
{
	CString sTmp;
//	int nKeyItem;
	int nRow;

	char *sHeader1[] = {"AV1", "AV2", "SCART", "COMP1", "COMP2", "USB1", "USB2", "USB3", "USB4", "HEAD_PHONE", "HDMI1", "HDMI2", "HDMI3", "HDMI4", "RGB", "DVI", "DP", "SMHL"};
	char *sHeader2[] = {"Input List", "Status", "Check Result"};


	m_ctrlInputCheckGrid.SetRedraw(FALSE);
	m_ctrlInputCheckGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlInputCheckGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlInputCheckGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlInputCheckGrid.SetBackColor(RGB(247,247,231));
	m_ctrlInputCheckGrid.SetGridColor(RGB(200,200,176));

	m_ctrlInputCheckGrid.SetFixedRows(1);
	m_ctrlInputCheckGrid.SetFixedCols(1);

	m_ctrlInputCheckGrid.SetFillStyle(flexFillRepeat);
	m_ctrlInputCheckGrid.SetFontName("Arial");
	m_ctrlInputCheckGrid.SetFontSize(10);

	m_ctrlInputCheckGrid.SetCols(3);
	m_ctrlInputCheckGrid.SetRows(19);
	m_ctrlInputCheckGrid.SetExtendLastCol(TRUE);

	m_ctrlInputCheckGrid.SetColWidth(0, 1500); //500
	m_ctrlInputCheckGrid.SetColWidth(1, 1300); //500
	m_ctrlInputCheckGrid.SetColWidth(2, 800); //500
	m_ctrlInputCheckGrid.SetFixedAlignment(0,flexAlignCenterCenter);
	m_ctrlInputCheckGrid.SetColAlignment(1,flexAlignCenterCenter);		
	m_ctrlInputCheckGrid.SetColAlignment(2,flexAlignCenterCenter);		

	m_ctrlInputCheckGrid.SetRowHeight(0, 300);

	m_ctrlInputCheckGrid.SetTextMatrix(0, 0, "INPUT ITEM");
	m_ctrlInputCheckGrid.SetTextMatrix(0, 1, "STATUS");
	m_ctrlInputCheckGrid.SetTextMatrix(0, 2, "CHECK RESULT");

	m_ctrlInputCheckGrid.SetCol(0);
	for(nRow = 0; nRow < 18 ; nRow++)
	{
		m_ctrlInputCheckGrid.SetRow(nRow + 1);
	
		m_ctrlInputCheckGrid.SetRowHeight(nRow + 1, 380);
		m_ctrlInputCheckGrid.SetText(sHeader1[nRow]);
	}

	m_ctrlInputCheckGrid.SetTextMatrix(1, 1, CurrentSet->sJACK_AV1);       
	m_ctrlInputCheckGrid.SetTextMatrix(2, 1, CurrentSet->sJACK_AV2);       
	m_ctrlInputCheckGrid.SetTextMatrix(3, 1, CurrentSet->sJACK_SCART);     
	m_ctrlInputCheckGrid.SetTextMatrix(4, 1, CurrentSet->sJACK_COMP1);     
	m_ctrlInputCheckGrid.SetTextMatrix(5, 1, CurrentSet->sJACK_COMP2);     
	m_ctrlInputCheckGrid.SetTextMatrix(6, 1, CurrentSet->sJACK_USB1);      
	m_ctrlInputCheckGrid.SetTextMatrix(7, 1, CurrentSet->sJACK_USB2);      
	m_ctrlInputCheckGrid.SetTextMatrix(8, 1, CurrentSet->sJACK_USB3);      
	m_ctrlInputCheckGrid.SetTextMatrix(9, 1, CurrentSet->sJACK_USB4);      
	m_ctrlInputCheckGrid.SetTextMatrix(10, 1, CurrentSet->sJACK_HEAD_PHONE);
	m_ctrlInputCheckGrid.SetTextMatrix(11, 1, CurrentSet->sJACK_HDMI1);;    
	m_ctrlInputCheckGrid.SetTextMatrix(12, 1, CurrentSet->sJACK_HDMI2);     
	m_ctrlInputCheckGrid.SetTextMatrix(13, 1, CurrentSet->sJACK_HDMI3);     
	m_ctrlInputCheckGrid.SetTextMatrix(14, 1, CurrentSet->sJACK_HDMI4);     
	m_ctrlInputCheckGrid.SetTextMatrix(15, 1, CurrentSet->sJACK_RGB);       
	m_ctrlInputCheckGrid.SetTextMatrix(16, 1, CurrentSet->sJACK_DVI);       
	m_ctrlInputCheckGrid.SetTextMatrix(17, 1, CurrentSet->sJACK_DP);        
	m_ctrlInputCheckGrid.SetTextMatrix(18, 1, CurrentSet->sJACK_SMHL);      

	m_ctrlInputCheckGrid.SetRedraw(TRUE);
	m_ctrlInputCheckGrid.Refresh();

}
void CInputDetectCheckDlg::UpdateResult(int nIndex, int nResult)
{
	CString sTmp;

	m_ctrlInputCheckGrid.SetRow(nIndex);
	m_ctrlInputCheckGrid.SetCol(2);
	m_ctrlInputCheckGrid.SetColDataType(2, flexDTString);

	switch(nResult){
		case 0:
			sTmp = _T("NG");
			m_ctrlInputCheckGrid.SetCellBackColor(RGB(255,0,0));
			m_ctrlInputCheckGrid.SetCellForeColor(RGB(255,255,255));
			m_ctrlInputCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			break;

		case 1:
			sTmp = _T("OK");
			m_ctrlInputCheckGrid.SetCellBackColor(RGB(0,128,64));
			m_ctrlInputCheckGrid.SetCellForeColor(RGB(255,255,255));
			m_ctrlInputCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			m_bInputDetectResultList[nIndex] = 1;
			break;
		case 2:
			sTmp = _T("SKIP");
			m_ctrlInputCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			break;
		default:
			sTmp = _T("SKIP");
			m_ctrlInputCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			break;

	}


}
BOOL  CInputDetectCheckDlg::InputCheck()
{
	CString sWriteData = _T("");
	CString szPrevMsg = _T("");
	CString sMsg = _T("");
	BOOL bReturn = TRUE;
	BOOL bResult = TRUE;
	CString sTmp = _T("");

	m_bTestRunning = TRUE;
	CurrentSet->nInputCheckResultVal = 0;

	g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, "");

	if(CurrentSet->sJACK_AV1 == "YES"){

		//sTmp = m_ctrlInputCheckGrid.GetTextMatrix(1, 2 );
		if (m_bInputDetectResultList[1] == 1)
		{
			UpdateResult(1, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0001;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_AV1, 1);
			if (bResult) {
				//UpdateResult(1, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0001;
			}
			else {
				UpdateResult(1, 0);
				sMsg = sMsg + "AV1(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}

	if(CurrentSet->sJACK_AV2 == "YES"){
		if (m_bInputDetectResultList[2] == 1)
		{
			//UpdateResult(2, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0001;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_AV2, 1);
			if (bResult) {
				UpdateResult(2, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0001;
			}
			else {
				UpdateResult(2, 0);
				sMsg = sMsg + "AV2(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_SCART == "YES"){

		if (m_bInputDetectResultList[3] == 1)
		{
			//UpdateResult(3, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0002;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_SCART, 1);
			if (bResult) {
				UpdateResult(3, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0002;
			}
			else {
				UpdateResult(3, 0);
				sMsg = sMsg + "SCART(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_COMP1 == "YES"){
		if (m_bInputDetectResultList[4] == 1)
		{
			//UpdateResult(4, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0004;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_COMP1, 1);
			if (bResult) {
				UpdateResult(4, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0004;
			}
			else {
				UpdateResult(4, 0);
				sMsg = sMsg + "COMP1(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_COMP2 == "YES"){
		if (m_bInputDetectResultList[5] == 1)
		{
			//UpdateResult(5, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0004;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_COMP2, 1);
			if (bResult) {
				UpdateResult(5, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0004;
			}
			else {
				UpdateResult(5, 0);
				sMsg = sMsg + "COMP2(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_USB1 == "YES"){
		if (m_bInputDetectResultList[6] == 1)
		{
			//UpdateResult(6, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0008;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_USB1, 1);
			if (bResult) {
				UpdateResult(6, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0008;
			}
			else {
				UpdateResult(6, 0);
				sMsg = sMsg + "USB1(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_USB2 == "YES"){
		if (m_bInputDetectResultList[7] == 1)
		{
			//UpdateResult(7, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0010;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_USB2, 1);
			if (bResult) {
				UpdateResult(7, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0010;
			}
			else {
				UpdateResult(7, 0);
				sMsg = sMsg + "USB2(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_USB3 == "YES"){
		if (m_bInputDetectResultList[8] == 1)
		{
			//UpdateResult(8, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0020;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_USB3, 1);
			if (bResult) {
				UpdateResult(8, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0020;
			}
			else {
				UpdateResult(8, 0);
				sMsg = sMsg + "USB3(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_USB4 == "YES"){
		if (m_bInputDetectResultList[9] == 1)
		{
			//UpdateResult(9, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0040;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_USB4, 1);
			if (bResult) {
				UpdateResult(9, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0040;
			}
			else {
				UpdateResult(9, 0);
				sMsg = sMsg + "USB4(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_HEAD_PHONE == "YES"){
		if (m_bInputDetectResultList[10] == 1)
		{
			//UpdateResult(10, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0080;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_HEAD_PHONE, 1);
			if (bResult) {
				UpdateResult(10, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0080;
			}
			else {
				UpdateResult(10, 0);
				sMsg = sMsg + "HP(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_HDMI1 == "YES"){
		if (m_bInputDetectResultList[11] == 1)
		{
			//UpdateResult(11, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0100;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_HDMI1, 1);
			if (bResult) {
				UpdateResult(11, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0100;
			}
			else {
				UpdateResult(11, 0);
				sMsg = sMsg + "HDMI1(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_HDMI2 == "YES"){
		if (m_bInputDetectResultList[12] == 1)
		{
			//UpdateResult(12, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0200;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_HDMI2, 1);
			if (bResult) {
				UpdateResult(12, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0200;
			}
			else {
				UpdateResult(12, 0);
				sMsg = sMsg + "HDMI2(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_HDMI3 == "YES"){
		if (m_bInputDetectResultList[13] == 1)
		{
			//UpdateResult(13, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0400;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_HDMI3, 1);
			if (bResult) {
				UpdateResult(13, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0400;
			}
			else {
				UpdateResult(13, 0);
				sMsg = sMsg + "HDMI3(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_HDMI4 == "YES"){
		if (m_bInputDetectResultList[14] == 1)
		{
			//UpdateResult(14, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0800;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_HDMI4, 1);
			if (bResult) {
				UpdateResult(14, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x0800;
			}
			else {
				UpdateResult(14, 0);
				sMsg = sMsg + "HDMI4(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_RGB == "YES"){
		if (m_bInputDetectResultList[15] == 1)
		{
			//UpdateResult(15, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x1000;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_RGB, 1);
			if (bResult) {
				UpdateResult(15, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x1000;
			}
			else {
				UpdateResult(15, 0);
				sMsg = sMsg + "RGB(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_DVI == "YES"){
		if (m_bInputDetectResultList[16] == 1)
		{
			//UpdateResult(16, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x2000;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_DVI, 1);
			if (bResult) {
				UpdateResult(16, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x2000;
			}
			else {
				UpdateResult(16, 0);
				sMsg = sMsg + "DVI(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_DP == "YES"){
		if (m_bInputDetectResultList[17] == 1)
		{
			//UpdateResult(17, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x4000;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_DP, 1);
			if (bResult) {
				UpdateResult(17, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x4000;
			}
			else {
				UpdateResult(17, 0);
				sMsg = sMsg + "DP(NG)";
				bReturn = FALSE;
			}
		}
	}
	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}
	if(CurrentSet->sJACK_SMHL == "YES"){
		if (m_bInputDetectResultList[18] == 1)
		{
			//UpdateResult(18, 1);
			CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x8000;
		}
		else
		{
			bResult = TVCommCtrl.Input_Detect_Check(JACK_SMHL, 1);
			if (bResult) {
				UpdateResult(18, 1);
				CurrentSet->nInputCheckResultVal = CurrentSet->nInputCheckResultVal | 0x8000;
			}
			else {
				UpdateResult(18, 0);
				sMsg = sMsg + "SMHL(NG)";
				bReturn = FALSE;
			}
		}
	}

	if(CurrentSet->bRunAbort){ m_bTestRunning = FALSE; return FALSE;}


	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
	if(szPrevMsg != _T(""))
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + sMsg);
		pCurStep->m_strMsg = szPrevMsg + "; " + sMsg;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
		pCurStep->m_strMsg = sMsg;
	}	

	if(bReturn){
		if(CurrentSet->bDrmKey){
			sWriteData.Format("KEY%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
		}
		else{	
			sWriteData.Format("DFT%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
		}
		if(!TVCommCtrl.ModelSNo_Write(sWriteData, FALSE)){
			Sleep(100);
			if(!TVCommCtrl.ModelSNo_Write(sWriteData, FALSE)){
				AddStringToStatus("InputCheckResult(SN Write NG)");
				//bReturn = FALSE;
			}
		}
	}
	else
	{
		m_bInputCheckWait = 1;
	}
	SaveResult2File();
	m_bTestRunning = FALSE;

	//m_bInputCheckResult = bReturn;
	return bReturn;
}

void CInputDetectCheckDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
		case 1:
		{
			KillTimer(nIDEvent);

			m_bInputCheckResult = InputCheck();
			if(m_bInputCheckResult){
				OnOK();	
			}
			break;
		}
	
	}
	CDialog::OnTimer(nIDEvent);
}

void CInputDetectCheckDlg::OnBtnEnter() 
{
	OnOK();	
}

void CInputDetectCheckDlg::OnBtnRecheck() 
{
	InitInputCheckGrid();
	m_bInputCheckResult = InputCheck();
	if(m_bInputCheckResult){
		OnOK();	
	}
}

void CInputDetectCheckDlg::SaveResult2File()
{
	CString sFolderPath = _T("");
	sFolderPath  = CurrentSet->sResultFolder+ "_Detect.html";  

	SaveGridToHTML(&m_ctrlInputCheckGrid, (char*)(LPCTSTR)sFolderPath);
}



void CInputDetectCheckDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ¿©±â¿¡ ¸Þ½ÃÁö Ã³¸®±â ÄÚµå¸¦ Ãß°¡ÇÕ´Ï´Ù.
	m_bActivate = FALSE;
}


void CInputDetectCheckDlg::OnBnClickedBtnStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sMsg;
	if (CurrentSet->bIsRunning)
	{
		CurrentSet->bRunAbort = TRUE;
		sMsg.LoadString(IDS_USER_BREAK);
		AddStringToStatus(sMsg);

		if (!m_bTestRunning) {
			OnOK();
		}
		else {
			Sleep(1500);
			OnOK();
		}
	}
}
