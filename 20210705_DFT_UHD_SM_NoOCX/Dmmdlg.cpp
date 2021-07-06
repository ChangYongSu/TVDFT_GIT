// File  Έν : DmmDlg.cpp

// DMMDlg.cpp : implementation file
//


#include "stdafx.h"
#include "DATsys.h"
//#include "dmm.h"
#include "DMMDlg.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*		g_pView;

/////////////////////////////////////////////////////////////////////////////
// CDMMDlg dialog


CDMMDlg::CDMMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMMDlg::IDD, pParent)
{
	m_bPause = FALSE;
	//{{AFX_DATA_INIT(CDMMDlg)
	m_nFunction = 0;
	m_fMeasured = 0.0;
	m_sUnit = _T("");
	m_nChannel = 0;
	//}}AFX_DATA_INIT
}


void CDMMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMMDlg)
	DDX_Control(pDX, IDOK, m_ctrlOK);
	DDX_Control(pDX, IDC_PAUSE, m_ctrlPause);
	DDX_Control(pDX, IDC_MEASURE, m_ctrlMeasure);
	DDX_CBIndex(pDX, IDC_FUNCTION, m_nFunction);
	DDX_Text(pDX, IDC_MEASURED, m_fMeasured);
	DDX_Text(pDX, IDC_MEASURED_UNIT, m_sUnit);
	DDX_CBIndex(pDX, IDC_MEASURE_CHANNEL, m_nChannel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDMMDlg, CDialog)
	//{{AFX_MSG_MAP(CDMMDlg)
	ON_CBN_SELCHANGE(IDC_FUNCTION, OnSelchangeFunction)
//	ON_BN_CLICKED(IDC_MEASURE, OnMeasure)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_CBN_SELCHANGE(IDC_MEASURE_CHANNEL, OnSelchangeMeasureChannel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMMDlg message handlers

void CDMMDlg::OnSelchangeFunction() 
{
	UpdateData();
}

//void CDMMDlg::OnMeasure() 
//{
//	MSG msg;
//	double dVol;
//	CString sVol;
//	int nCH;
//
//	m_bPause = FALSE;
//	m_ctrlMeasure.EnableWindow(FALSE);
//	m_ctrlOK.EnableWindow(FALSE);
//	m_ctrlPause.EnableWindow(TRUE);
//	m_ctrlPause.SetFocus();
//
//	if(g_nRunningProcessNo == 1)
//	{
//		nCH = m_nChannel + 1001;
//	}
//	else if(g_nRunningProcessNo == 2)
//	{
//		nCH = m_nChannel + 2001;
//	}
//	else
//	{
//		nCH = m_nChannel + 3001;
//	}
//
//
////	dVol = g_pView->GetDaqData(m_nFunction + 1, nCH, nCH,sVol);
////	Sleep(200);
//	while (1) 
//	{ 
//		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
//		{ 
//			if (!IsDialogMessage(&msg))
//			{
//				::TranslateMessage(&msg);
//				::DispatchMessage(&msg);
//			}
//		}
//
//		dVol = g_pView->GetDaqData(m_nFunction + 1, nCH, nCH,sVol);
//		if (m_nFunction == 0)
//		{
//			if(fabs(dVol) < 0.01)
//			{
//				m_fMeasured = dVol * 1000;
//				m_sUnit = _T("mV AC");
//			}
//			else
//			{
//				m_fMeasured = dVol;
//				m_sUnit = _T("V AC");
//			}
//		}
//		else if(m_nFunction == 1)
//		{ 
//			if(fabs(dVol) < 0.01)
//			{
//				m_fMeasured = dVol * 1000;
//				m_sUnit = _T("mV DC");
//			}
//			else
//			{
//				m_fMeasured = dVol;
//				m_sUnit = _T("V DC");
//			}
//		}
//		else if(m_nFunction == 2)
//		{	
//			if(fabs(dVol) > 1000)
//			{
//				m_fMeasured = dVol / 1000;
//				m_sUnit = _T("K-Ohm");
//			}
//			else if(fabs(dVol) > 1000000)
//			{
//				m_fMeasured = dVol / 1000000;
//				m_sUnit = _T("M-Ohm");
//			}
//			else
//			{
//				m_fMeasured = dVol;
//				m_sUnit = _T("Ohm");
//			}
//		}
//		else if(m_nFunction == 3)
//		{	
//			if(dVol > 1000)
//			{
//				m_fMeasured = dVol / 1000;
//				m_sUnit = _T("KHz");
//			}
//			else if(dVol > 1000000)
//			{
//				m_fMeasured = dVol / 1000000;
//				m_sUnit = _T("MHz");
//			}
//			else
//			{
//				m_fMeasured = dVol;
//				m_sUnit = _T("Hz");
//			}
//		}
//
//		UpdateData(FALSE);
//		if (m_bPause) break;
//		Sleep(500);
//	}
//}

void CDMMDlg::OnPause() 
{
	m_bPause = TRUE;

	m_ctrlMeasure.EnableWindow();
	m_ctrlOK.EnableWindow();
	m_ctrlPause.EnableWindow(FALSE);
	m_ctrlOK.SetFocus();

//	Sleep(100);
//	DMM.SendString(_T("*RST"));
//	DMM.SendString(_T("ROUT:MON:STAT OFF"));
//	DMM.SendString(_T("ROUT:SCAN (@)"));
	
}

BOOL CDMMDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nChannel = 0; 
	m_nFunction = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMMDlg::OnSelchangeMeasureChannel() 
{
	UpdateData();
	
}

void CDMMDlg::OnOK() 
{
//	Sleep(100);
//	DMM.SendString(_T("*RST"));
//	DMM.SendString(_T("ROUT:MON:STAT OFF"));
//	DMM.SendString(_T("ROUT:SCAN (@)"));
	
	CDialog::OnOK();
}


