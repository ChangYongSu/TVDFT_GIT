// AddModelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "AddModelDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

/////////////////////////////////////////////////////////////////////////////
// CAddModelDlg dialog


CAddModelDlg::CAddModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddModelDlg)
	m_szChassisName = _T("");
	m_szModelInfoPath = _T("");
	m_szModelName = _T("");
	m_szRefFolderPath = _T("");
	m_szSeqPath = _T("");
	//}}AFX_DATA_INIT

	m_sWinText = _T("Add Model");
	m_EditMode = 0;
}


void CAddModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddModelDlg)
	DDX_Text(pDX, IDC_EDIT_CHASSIS_NAME, m_szChassisName);
	DDX_Text(pDX, IDC_EDIT_MODEL_INFO_FILE, m_szModelInfoPath);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME, m_szModelName);
	DDX_Text(pDX, IDC_EDIT_REF_FOLDER_NAME, m_szRefFolderPath);
	DDX_Text(pDX, IDC_EDIT_SEQ_FILE, m_szSeqPath);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_DIVISION1, m_cComboDivision);
}


BEGIN_MESSAGE_MAP(CAddModelDlg, CDialog)
	//{{AFX_MSG_MAP(CAddModelDlg)
	ON_BN_CLICKED(IDC_BTN_SEQ, OnBtnSeq)
	ON_BN_CLICKED(IDC_BTN_MODEL_INFO, OnBtnModelInfo)
	ON_BN_CLICKED(IDC_BTN_REF_FOLDER, OnBtnRefFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddModelDlg message handlers

void CAddModelDlg::OnBtnSeq() 
{
	CString sTemp;

	UpdateData(TRUE);

	CFileDialog dlg(TRUE, "seq", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Seq File(*.seq)|*.seq|",NULL);

	CString sTmp = CurrentSet->sServerFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		sTemp = dlg.GetPathName();
//		sTemp.TrimLeft(CurrentSet->sServerFolder); 
		sTemp.Delete(0, CurrentSet->sServerFolder.GetLength()); 
		m_szSeqPath = sTemp;
	}	

	UpdateData(FALSE);
}

void CAddModelDlg::OnBtnModelInfo() 
{
	CString sTemp;

	UpdateData(TRUE);

	CFileDialog dlg(TRUE, "ini", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Model Info File(*.ini)|*.ini|",NULL);

	CString sTmp = CurrentSet->sModelInfoFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		sTemp = dlg.GetPathName();
//		sTemp.TrimLeft(CurrentSet->sServerFolder); 
		sTemp.Delete(0, CurrentSet->sModelInfoFolder.GetLength()); 
		m_szModelInfoPath = sTemp;
	}
	
	UpdateData(FALSE);
}

void CAddModelDlg::OnBtnRefFolder() 
{
	CString sTemp;

	UpdateData(TRUE);

	CString strFolderPath;

//	TCHAR buffer[_MAX_PATH];

	if (GetFolder(&strFolderPath, "Select Reference Image Foler.", this->m_hWnd, NULL, (const char*)CurrentSet->sRefRootFolder))
	{
		if (!strFolderPath.IsEmpty())
		{
			m_szRefFolderPath = strFolderPath;
			sTemp = strFolderPath;
		//	sTemp.TrimLeft(CurrentSet->sRefRootFolder); 
			sTemp.Delete(0, CurrentSet->sRefRootFolder.GetLength()); 
			m_szRefFolderPath = sTemp;
		}
        else
        {
            MessageBox(_T("Invalid Folder Name."), _T("lol"), MB_OKCANCEL|MB_ICONASTERISK ); 
        }
		UpdateData(FALSE);
	}
 }

void CAddModelDlg::OnOK() 
{
	UpdateData(TRUE);

	// TODO: Add extra validation here
	if(m_szChassisName == _T(""))
	{
		AfxMessageBox("Incorrect Chassis Name"); return;
	}

	if(m_szModelName == _T(""))
	{
		AfxMessageBox("Incorrect Model Name"); return;
	}

	//if(m_szModelInfoPath == _T(""))
	//{
	//	AfxMessageBox("Incorrect Model Info File"); return;
	//}

	//if(m_szRefFolderPath == _T(""))
	//{
	//	AfxMessageBox("Incorrect Reference Folder"); return;
	//}

	if(m_szSeqPath == _T(""))
	{
		AfxMessageBox("Incorrect Sequence File"); return;
	}

	m_szChassisName.TrimLeft(" ");
	m_szChassisName.TrimRight(" ");
	m_szModelName.TrimLeft(" ");
	m_szModelName.TrimRight(" ");

	m_cComboDivision.GetLBText(m_cComboDivision.GetCurSel(), m_sDivision);

	CString sTemp2, sTemp;
	sTemp2 = m_sDivision.Right(2);

	sTemp.Format("\\Ref_%s\\%s\\%s\\Ref", sTemp2, m_szChassisName, m_szModelName);
	m_szRefFolderPath = sTemp;

	//	sSeqFile = m_szSeqPath.Right(m_szSeqPath.GetLength() - m_szSeqPath.ReverseFind('\\')-1); 
	//	sSeqFile = sSeqFile.Left(sSeqFile.ReverseFind('.'));

	//	sTemp.Format("\\Ref_%s\\%s\\%s\\%s.ini", sTemp2, m_szChassisName, m_szModelName, sSeqFile);
	sTemp.Format("\\Ref_%s\\%s\\%s\\%s.ini", sTemp2, m_szChassisName, m_szModelName, m_szModelName);
	m_szModelInfoPath = sTemp;

	sTemp.Format("\\Ref_%s\\%s\\%s\\%s_VF.cfg", sTemp2, m_szChassisName, m_szModelName, m_szModelName);
	m_szVFConfigPath = sTemp;

	UpdateData(FALSE);

	CDialog::OnOK();
}

BOOL CAddModelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetWindowRect(&rect);

	CTime Time = CTime::GetCurrentTime();

	int nDivision = Time.GetYear();
	CString sTemp;
	for (int iDivision = 2007; iDivision <= nDivision + 1; iDivision++)
	{
		sTemp.Format(_T("%d"), iDivision);
		m_cComboDivision.AddString(sTemp);
	}
	m_cComboDivision.SetCurSel(m_nDivision);

	int nLeft   = m_ParentWndRect.left + (m_ParentWndRect.Width()/2)  - (rect.Width()/2);
	int nTop    = (m_ParentWndRect.Height()/2) + 200; 
	int nWidth  = rect.Width();
	int nHeight = rect.Height();
	
	SetWindowText(m_sWinText); 
	MoveWindow(nLeft,nTop,nWidth,nHeight);

	if (m_EditMode == 1)
	{
		GetDlgItem(IDC_EDIT_CHASSIS_NAME)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_MODEL_NAME)->EnableWindow(0);
	

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
