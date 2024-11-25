// DMMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DATsys.h"
//#include "dmm.h"
#include "VfMeasureDlg.h"
#include "global.h"
#include "VF1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern g_nRunningProcessNo;
extern CDATsysView*		g_pView;

/////////////////////////////////////////////////////////////////////////////
// CVfMeasureDlg dialog


CVfMeasureDlg::CVfMeasureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVfMeasureDlg::IDD, pParent)
{
	m_bPause = FALSE;
	//{{AFX_DATA_INIT(CVfMeasureDlg)
	m_nFunction = 0;
	m_fMeasured = 0.0;
	m_sUnit = _T("");
	m_nChannel = 0;
	//}}AFX_DATA_INIT
}


void CVfMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVfMeasureDlg)
	DDX_Control(pDX, IDOK, m_ctrlOK);
	DDX_Control(pDX, IDC_PAUSE, m_ctrlPause);
	DDX_Control(pDX, IDC_MEASURE, m_ctrlMeasure);
	DDX_CBIndex(pDX, IDC_FUNCTION, m_nFunction);
	DDX_Text(pDX, IDC_MEASURED, m_fMeasured);
	DDX_Text(pDX, IDC_MEASURED_UNIT, m_sUnit);
	DDX_CBIndex(pDX, IDC_MEASURE_CHANNEL, m_nChannel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MEASURE_CHANNEL, m_ctrlCh);
}


BEGIN_MESSAGE_MAP(CVfMeasureDlg, CDialog)
	//{{AFX_MSG_MAP(CVfMeasureDlg)
	ON_CBN_SELCHANGE(IDC_FUNCTION, OnSelchangeFunction)
	ON_BN_CLICKED(IDC_MEASURE, OnMeasure)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_CBN_SELCHANGE(IDC_MEASURE_CHANNEL, OnSelchangeMeasureChannel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVfMeasureDlg message handlers

void CVfMeasureDlg::OnSelchangeFunction() 
{
	UpdateData();
}

void CVfMeasureDlg::OnMeasure() 
{
	MSG msg;
	double dVol;
	CString sVol;
	int nCH;

	m_bPause = FALSE;
	m_ctrlMeasure.EnableWindow(FALSE);
	m_ctrlOK.EnableWindow(FALSE);
	m_ctrlPause.EnableWindow(TRUE);
	m_ctrlPause.SetFocus();

	nCH = m_nChannel + 1;

	if((nCH == 10) ||  (nCH == 20) || (nCH == 30))
	{
		return;
	}
	while (1) 
	{ 
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			if (!IsDialogMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		if((nCH == 10) ||  (nCH == 20) || (nCH == 30))
		{
			continue;
		}

		if (m_nFunction == 0)
		{
			gVF1000Ctrl.GetVoltageData(nCH,dVol);
			if(fabs(dVol) < 0.01)
			{
				m_fMeasured = dVol * 1000;
				m_sUnit = _T("mV");
			}
			else
			{
				m_fMeasured = dVol;
				m_sUnit = _T("V");
			}
		}
		else if(m_nFunction == 1)
		{ 
			gVF1000Ctrl.GetFrequencyData(nCH,dVol);
			if(dVol > 1000)
			{
				m_fMeasured = dVol / 1000;
				m_sUnit = _T("KHz");
			}
			else if(dVol > 1000000)
			{
				m_fMeasured = dVol / 1000000;
				m_sUnit = _T("MHz");
			}
			else
			{
				m_fMeasured = dVol;
				m_sUnit = _T("Hz");
			}
		}

		UpdateData(FALSE);
		if (m_bPause) break;
		Sleep(500);
	}
}

void CVfMeasureDlg::OnPause() 
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

BOOL CVfMeasureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nChannel = 0; 
	m_nFunction = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVfMeasureDlg::OnSelchangeMeasureChannel() 
{
	UpdateData();
	
}

void CVfMeasureDlg::OnOK() 
{
//	Sleep(100);
//	DMM.SendString(_T("*RST"));
//	DMM.SendString(_T("ROUT:MON:STAT OFF"));
//	DMM.SendString(_T("ROUT:SCAN (@)"));
	
	CDialog::OnOK();
}
