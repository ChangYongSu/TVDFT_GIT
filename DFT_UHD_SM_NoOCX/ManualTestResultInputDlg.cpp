// ManualTestResultInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ManualTestResultInputDlg.h"
#include "DATsysView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#include "Global.h"

extern HWND g_hCommWnd;

extern CDATsysView * g_pView;

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CManualTestResultInputDlg dialog

//+ 2007.11.4 Add BY USY
long CManualTestResultInputDlg::OnProcessMessage(WPARAM wParam, LPARAM lParam)
{
	if(g_nRunningProcessNo == 1)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
				//break;
			case MSG_STOP_BTN1:
			{
				m_bTestResult = TRUE;
				m_intTestResult = 4;
				PostMessage(WM_COMMAND, IDC_BTN_PASS);
				break;
			}
			case MSG_RETRY_BTN1:
			{
				m_bTestResult = TRUE;
				m_intTestResult = 3;
				PostMessage(WM_COMMAND, IDC_BTN_PASS);
				break;
			}
			
			case MSG_OK_BTN1:
				{
					m_bTestResult = TRUE;
					m_intTestResult = 2;
					PostMessage(WM_COMMAND, IDC_BTN_PASS);
				}
				break;
			case MSG_NG_BTN1:
				{
					m_bTestResult = FALSE;
					m_intTestResult = 1;
					SendMessage(WM_COMMAND, IDC_BTN_NG);
				}
				break;
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_OK_BTN2:
			case MSG_NG_BTN2:
			case MSG_START_BTN3:
			case MSG_STOP_BTN3:
			case MSG_RETRY_BTN3:
			case MSG_OK_BTN3:
			case MSG_NG_BTN3:
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
						
			break;
			case MSG_OK_BTN1:
			case MSG_NG_BTN1:
				break;
			case MSG_START_BTN2:
				//
			case MSG_STOP_BTN2:
			{
				m_bTestResult = TRUE;
				m_intTestResult = 4;
				PostMessage(WM_COMMAND, IDC_BTN_PASS);
				break;
			}
			case MSG_RETRY_BTN2:
			{
				m_bTestResult = TRUE;
				m_intTestResult = 3;
				PostMessage(WM_COMMAND, IDC_BTN_PASS);
				break;
			}
			case MSG_START_BTN3:
			case MSG_STOP_BTN3:
			case MSG_RETRY_BTN3:
			case MSG_OK_BTN3:
			case MSG_NG_BTN3:
				break;

			case MSG_OK_BTN2:
				{
					m_bTestResult = TRUE;
					m_intTestResult = 2;
					PostMessage(WM_COMMAND, IDC_BTN_PASS);
				}
				break;
			case MSG_NG_BTN2:
				{
					m_bTestResult = FALSE;
					m_intTestResult = 1;
					PostMessage(WM_COMMAND, IDC_BTN_NG);
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
				break;
			case MSG_START_BTN3:
				
			case MSG_STOP_BTN3:
			{
				m_bTestResult = TRUE;
				m_intTestResult = 4;
				PostMessage(WM_COMMAND, IDC_BTN_PASS);
				break;
			}
			case MSG_RETRY_BTN3:
			{
				m_bTestResult = TRUE;
				m_intTestResult = 3;
				PostMessage(WM_COMMAND, IDC_BTN_PASS);
				break;
			}
			

			case MSG_OK_BTN3:
				{
					m_bTestResult = TRUE;
					m_intTestResult = 2;
					PostMessage(WM_COMMAND, IDC_BTN_PASS);
				}
				break;
			case MSG_NG_BTN3:
				{
					m_bTestResult = FALSE;
					m_intTestResult = 1;
					PostMessage(WM_COMMAND, IDC_BTN_NG);
				}
				break;
		}
	}
	return 1;
}
	//-

CManualTestResultInputDlg::CManualTestResultInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManualTestResultInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManualTestResultInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//+ 2007.11.5 Add BY USY
	m_bActivate = FALSE;
	//-
}


void CManualTestResultInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManualTestResultInputDlg)
	DDX_Control(pDX, IDC_MSG_GRID, m_ctrlMsgGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManualTestResultInputDlg, CDialog)
	//{{AFX_MSG_MAP(CManualTestResultInputDlg)
	ON_MESSAGE(WM_JOYSTICK_MSG, OnProcessMessage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PASS, OnBtnPass)
	ON_BN_CLICKED(IDC_BTN_NG, OnBtnNg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManualTestResultInputDlg message handlers

BEGIN_EVENTSINK_MAP(CManualTestResultInputDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CManualTestResultInputDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CManualTestResultInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	m_bTestResult = FALSE;
	m_intTestResult = 1;
	
	//+add kwmoon 080229
	CRect rect;
	GetWindowRect(&rect);
	
	int nLeft   = g_pView->m_FormViewRect.left + (g_pView->m_FormViewRect.Width()/2)  - (rect.Width()/2);
	int nTop    = (g_pView->m_FormViewRect.Height()/2) + 210; // - (rect.Height()/2);
	int nWidth  = rect.Width();
	int nHeight = rect.Height();
	
//	SetWindowPos(&wndTopMost,nLeft,nTop,nWidth,nHeight,SWP_NOSIZE | SWP_NOMOVE);
	MoveWindow(nLeft,nTop,nWidth,nHeight);
	//-
	InitMsgGrid();
	//+2007.10.15 Mod BY USY
	if(m_szStepName.IsEmpty())
	{
		m_szStepName.Format("Manual Test");
	}
	if(m_szMsg1.IsEmpty())
	{
		m_szMsg1.Format("Is Video Signal OK?");
	//	m_szMsg2.Format("Is Video Signal OK?");
	}
	m_ctrlMsgGrid.SetTextMatrix(0, 0, m_szStepName);
	m_ctrlMsgGrid.SetTextMatrix(1, 0, m_szMsg1);
	m_ctrlMsgGrid.SetTextMatrix(2, 0, m_szMsg2);

	
	g_pView->PlayWavSound(IDR_WAVE_RETRY);

	//+ 2007.11.5 Add BY USY
	m_bActivate = TRUE;
	//-

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CManualTestResultInputDlg::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}


void CManualTestResultInputDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	//+ 2007.11.5 Add BY USY
	m_bActivate = FALSE;
	//-
}

void CManualTestResultInputDlg::ClearInfoMessage() 
{
	m_szStepName = _T("");
	m_szMsg1	 = _T("");
	m_szMsg2	 = _T("");
}

void CManualTestResultInputDlg::SetInfoMessage(CString szStepName,CString szMsg1, CString szMsg2) 
{
	if(szStepName != _T("")) m_szStepName = szStepName; 
	if(szMsg1 != _T(""))	 m_szMsg1 = szMsg1; 
	if(szMsg2 != _T(""))	 m_szMsg2 = szMsg2;
}



void CManualTestResultInputDlg::OnBtnPass() 
{
	m_bTestResult = TRUE;
	OnOK();	
	
}

void CManualTestResultInputDlg::OnBtnNg() 
{
	m_bTestResult = FALSE;
	OnOK();	
	
}
void CManualTestResultInputDlg::InitMsgGrid()
{
	CString sTmp;
//	int nCol;
	int nRow;

	m_ctrlMsgGrid.SetRedraw(FALSE);
	m_ctrlMsgGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlMsgGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlMsgGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlMsgGrid.SetBackColor(RGB(247,247,231));
//	m_ctrlMsgGrid.SetGridColor(RGB(200,200,176));

	m_ctrlMsgGrid.SetFixedRows(1);
	m_ctrlMsgGrid.SetFixedCols(0);

	m_ctrlMsgGrid.SetFillStyle(flexFillRepeat);
	m_ctrlMsgGrid.SetFontName("Arial");
//	m_ctrlMsgGrid.SetFontName("System");
	m_ctrlMsgGrid.SetFontSize(14);
	m_ctrlMsgGrid.SetFontBold(TRUE);

	m_ctrlMsgGrid.SetCols(1);
	m_ctrlMsgGrid.SetRows(3);
	m_ctrlMsgGrid.SetExtendLastCol(TRUE);

	m_ctrlMsgGrid.SetColWidth(0, 2500); //500

	for(nRow = 0; nRow < 3 ; nRow++)
	{
		m_ctrlMsgGrid.SetRow(nRow);
		m_ctrlMsgGrid.SetCol(0);
	
		m_ctrlMsgGrid.SetRowHeight(nRow, 600);

		m_ctrlMsgGrid.SetFixedAlignment(0,flexAlignCenterCenter);
		m_ctrlMsgGrid.SetColAlignment(0,flexAlignCenterCenter);		
	}
	m_ctrlMsgGrid.SetTextMatrix(0, 0, _T("Manual Test"));

	m_ctrlMsgGrid.SetCol(-1);
	m_ctrlMsgGrid.SetRow(-1);

	m_ctrlMsgGrid.SetRedraw(TRUE);
	m_ctrlMsgGrid.Refresh();

}
