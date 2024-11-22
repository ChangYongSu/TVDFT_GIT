// SetAudioSpecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SetAudioSpecDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetAudioSpecDlg dialog


CSetAudioSpecDlg::CSetAudioSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAudioSpecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetAudioSpecDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nAudioSource = 0;
	m_nLeftFreq = 0;
	m_nRightFreq = 0;
	m_nLeftLevel = 0;
	m_nRightLevel = 0;
	m_nAudioMargin = 0;
}


void CSetAudioSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetAudioSpecDlg)
	DDX_Control(pDX, IDC_EDIT_R_LEVEL, m_ctrlRightLevelEdit);
	DDX_Control(pDX, IDC_EDIT_R_FREQ, m_ctrlRightFreqEdit);
	DDX_Control(pDX, IDC_EDIT_L_LEVEL, m_ctrlLeftLevelEdit);
	DDX_Control(pDX, IDC_EDIT_L_FREQ, m_ctrlLeftFreqEdit);
	DDX_Control(pDX, IDC_EDIT_AUDIO_MARGIN, m_ctrlAudioMarginEdit);
	DDX_Control(pDX, IDC_CMB_AUDIO_SOURCE, m_ctrlAudioSrcCmb);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetAudioSpecDlg, CDialog)
	//{{AFX_MSG_MAP(CSetAudioSpecDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetAudioSpecDlg message handlers

BOOL CSetAudioSpecDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString sTmp;
	//Audio Source Combo Init
	char* sSource[] = {"0. Not USe", "1. SPEAKER", "2. AV1", "3. AV2", "4. AV3", "5. VARIABLE OUT", "6. HEADPHONE", "7. WOOFER", "8. SPDIF", "9. OPTIC"};

	m_ctrlAudioSrcCmb.ResetContent();
	for(int nTmp = 0; nTmp < 10; nTmp++)
	{
		sTmp.Format("%s", sSource[nTmp]);
		m_ctrlAudioSrcCmb.AddString(sTmp);
	}
	//Init Value
	m_ctrlAudioSrcCmb.SetCurSel(m_nAudioSource );
	sTmp.Format("%d", m_nLeftFreq);
	m_ctrlLeftFreqEdit.SetWindowText(sTmp);
	sTmp.Format("%d", m_nRightFreq);
	m_ctrlRightFreqEdit.SetWindowText(sTmp);
	sTmp.Format("%d", m_nLeftLevel);
	m_ctrlLeftLevelEdit.SetWindowText(sTmp);
	sTmp.Format("%d", m_nRightLevel);
	m_ctrlRightLevelEdit.SetWindowText(sTmp);
	sTmp.Format("%d", m_nAudioMargin);
	m_ctrlAudioMarginEdit.SetWindowText(sTmp);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetAudioSpecDlg::OnOK() 
{
	CString sTmp;
	m_nAudioSource = m_ctrlAudioSrcCmb.GetCurSel() ;

	m_ctrlLeftFreqEdit.GetWindowText(sTmp);
	m_nLeftFreq = atoi(sTmp);
	
	m_ctrlRightFreqEdit.GetWindowText(sTmp);
	m_nRightFreq = atoi(sTmp);

	m_ctrlLeftLevelEdit.GetWindowText(sTmp);
	m_nLeftLevel = atoi(sTmp);

	m_ctrlRightLevelEdit.GetWindowText(sTmp);
	m_nRightLevel = atoi(sTmp);

	m_ctrlAudioMarginEdit.GetWindowText(sTmp);
	m_nAudioMargin = atoi(sTmp);

	CDialog::OnOK();
}

void CSetAudioSpecDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
