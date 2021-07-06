// XDischargDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DATsys.h"
#include "XDischargDlg.h"
#include <math.h>
#include "global.h"
#include "support.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern g_nRunningProcessNo;
extern CDATsysView*		g_pView;

/////////////////////////////////////////////////////////////////////////////
// CXDischargDlg dialog

UINT XVolMeasThread(LPVOID lParam);


CXDischargDlg::CXDischargDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXDischargDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXDischargDlg)
	//}}AFX_DATA_INIT
}


void CXDischargDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXDischargDlg)
	DDX_Control(pDX, IDC_LEDLABEL_XVA, m_ctlLabel_XVa);
	DDX_Control(pDX, IDC_LEDLABEL_X5V, m_ctlLabel_X5V);
	DDX_Control(pDX, IDC_LEDLABEL_X15V, m_ctlLabel_X15V);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXDischargDlg, CDialog)
	//{{AFX_MSG_MAP(CXDischargDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXDischargDlg message handlers

BOOL CXDischargDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bReadFlag = FALSE;

	m_dVolt1 = 0.0;
	m_dVolt2 = 0.0;
	m_dVolt3 = 0.0;

	m_pMeasureThread = NULL;

	m_ctlLabel_X15V.SetBackColor(BLACK);
	m_ctlLabel_X15V.SetColor(DARKGREEN, LIGHTGREEN);
	
	m_ctlLabel_X5V.SetBackColor(BLACK);
	m_ctlLabel_X5V.SetColor(DARKGREEN, LIGHTGREEN);
	
	m_ctlLabel_XVa.SetBackColor(BLACK);
	m_ctlLabel_XVa.SetColor(DARKGREEN, LIGHTGREEN);

	m_ctlLabel_X5V.SetText(_T("0.0"));
	m_ctlLabel_X15V.SetText(_T("0.0"));
	m_ctlLabel_XVa.SetText(_T("0.0"));

	m_dMeasure = 0.0;
//	m_dLimit = GetHighLimit();

	m_bThredStop = FALSE;
	m_pMeasureThread = AfxBeginThread(XVolMeasThread, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXDischargDlg::OnOK() 
{
//	clock_t		start;
	DWORD dwExitCode = 0;

	m_bThredStop = TRUE;

//	UpdateData(TRUE);
	Sleep(500);

	if(m_pMeasureThread)
	{
		TerminateThread(m_pMeasureThread->m_hThread,dwExitCode);

		m_pMeasureThread = NULL; 
	}

	m_bReadFlag = FALSE;

	if(m_dVolt1 < m_dVolt2){
		m_dMeasure = m_dVolt2;
	}
	else{
		m_dMeasure = m_dVolt1;
	}
	if(m_dMeasure < m_dVolt3){
		m_dMeasure = m_dVolt3;
	}
	
	CDialog::OnOK();
}
void CXDischargDlg::GetVolt()
{
	CString sVolt1 = _T("");
	CString sVolt2 = _T("");
	CString sVolt3 = _T("");
	TCHAR *stopstring;

	CStringArray SplitIt;

	int nCH_S;
	int nCH_E;
	double dVol = 0.0;
	CString sVol = _T("");
	CString sTemp;

	if(m_bThredStop){return;}
	m_bReadFlag = TRUE;


	if(g_nRunningProcessNo == 1)
	{
		nCH_S = 1039;
		nCH_E = 1041;
	}
	else if(g_nRunningProcessNo == 2)
	{
		nCH_S = 2039;
		nCH_E = 2041;
	}
	else
	{
		nCH_S = 3039;
		nCH_E = 3041;
	}

	dVol = g_pView->GetDaqData(DAQ_FUNC_DC_M, nCH_S, nCH_E,sVol);

	StringToken(sVol, ",", SplitIt, TRUE);
	if(SplitIt.GetSize() == 3){ 
		sVolt1 = SplitIt.GetAt(0);
		sVolt2 = SplitIt.GetAt(1);
		sVolt3 = SplitIt.GetAt(2);
		m_dVolt1 = _tcstod((LPCTSTR)sVolt1, &stopstring);
		m_dVolt2 = _tcstod((LPCTSTR)sVolt2, &stopstring);
		m_dVolt3 = _tcstod((LPCTSTR)sVolt3, &stopstring);

		sTemp.Format("%.2f", m_dVolt1);
		m_ctlLabel_X5V.SetText(sTemp);
		sTemp.Format("%.2f", m_dVolt2);
		m_ctlLabel_X15V.SetText(sTemp);
		sTemp.Format("%.2f", m_dVolt3);
		m_ctlLabel_XVa.SetText(sTemp);
	}

//	UpdateData();

	m_bReadFlag = FALSE;
}

UINT XVolMeasThread(LPVOID lParam)
{
//	MSG msg;
	//CString sTemp;
	CString sBuf;
	CXDischargDlg* pOwner = (CXDischargDlg*) lParam;

	if(pOwner == NULL)
	{
		//sTemp.LoadString(IDS_THREAD_ERROR);
		sBuf = "Failed to execute thread of DischargDlg"; 
		AfxMessageBox(sBuf);  //" XDischargDlg의 스레드를 실행 시키는데 실패하였습니다");
		return 0;
	}

	while(!pOwner->m_bThredStop)
	{
	/*	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			if (!IsDialogMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		*/
		pOwner->GetVolt(); 
		if((pOwner->m_dVolt1 < pOwner->m_dLimit) &&
		   (pOwner->m_dVolt2 < pOwner->m_dLimit) &&
		   (pOwner->m_dVolt3 < pOwner->m_dLimit))
		{
			pOwner->m_bThredStop = TRUE;
			pOwner->PostMessage(WM_COMMAND, IDOK);
			break;
		}
		Sleep(0);
	}
	return TRUE;
}
