// SelectChassisModelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SelectChassisModelDlg.h"
#include "Global.h"
#include "AddModelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CStringArray g_Divisions;

//+change 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

/////////////////////////////////////////////////////////////////////////////
// CSelectChassisModelDlg dialog


CSelectChassisModelDlg::CSelectChassisModelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectChassisModelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectChassisModelDlg)
	m_szCurrentSeqFilePath = _T("");
	m_szModelInfoFilePath = _T("");
	m_szRefImgFolder = _T("");
	m_szSeqFilePath = _T("");
	m_szCurrentRefFilePath = _T("");
	//}}AFX_DATA_INIT
}


void CSelectChassisModelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectChassisModelDlg)
	DDX_Control(pDX, IDC_COMBO1, m_ctrlDivisionCmb);
	DDX_Control(pDX, IDC_CHASSIS_GRID, m_gridChassis);
	DDX_Control(pDX, IDC_MODEL_GRID, m_gridModel);
	DDX_Text(pDX, IDC_EDIT_CURRENT_SEQ, m_szCurrentSeqFilePath);
	DDX_Text(pDX, IDC_EDIT_MODEL_INFO, m_szModelInfoFilePath);
	DDX_Text(pDX, IDC_EDIT_REF_FOLDER, m_szRefImgFolder);
	DDX_Text(pDX, IDC_EDIT_SEQ, m_szSeqFilePath);
	DDX_Text(pDX, IDC_EDIT_CURRENT_REF, m_szCurrentRefFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectChassisModelDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectChassisModelDlg)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectChassisModelDlg message handlers

BEGIN_EVENTSINK_MAP(CSelectChassisModelDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSelectChassisModelDlg)
	ON_EVENT(CSelectChassisModelDlg, IDC_CHASSIS_GRID, -600 /* Click */, OnClickChassisGrid, VTS_NONE)
	ON_EVENT(CSelectChassisModelDlg, IDC_MODEL_GRID, -600 /* Click */, OnClickModelGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSelectChassisModelDlg::OnClickChassisGrid() 
{
	POSITION	Pos				= NULL;
	CModelData* pModelData		= NULL;
	int			nChassisRow		= m_gridChassis.GetRow();
	int			nCurModelRow	= m_gridModel.GetRow();
	int			nModelRow		= 1;

	CString szChassisName = _T("");
	CString szModelName	  = _T("");

	if((nChassisRow <= 0) &&  (m_gridChassis.GetRows() > 1))
	{
		m_gridChassis.SetRow(1); 
		nChassisRow = 1;
	}
	else if( (nChassisRow >= m_gridChassis.GetRows()) && (m_gridChassis.GetRows() > 1))
	{
		m_gridChassis.SetRow(m_gridChassis.GetRows() - 1); 
		nChassisRow = m_gridChassis.GetRows() - 1;
	}

	szChassisName = m_gridChassis.GetTextMatrix(nChassisRow,0);

	Pos = CurrentSet->ModelList.GetHeadPosition();

	while(Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if(pModelData->m_szChassisName != _T(""))
		{
			if(pModelData->m_szChassisName.Compare(szChassisName) == NULL)
			{
				szModelName = pModelData->m_szModelName;
				m_gridModel.SetRows(nModelRow + 1);
				m_gridModel.SetTextMatrix(nModelRow++, 0, szModelName);
			}
		}
	}
	ChangeRowColor(nChassisRow,CHASSIS_GRID);
	if(m_sSelChassis != szChassisName)
	{
		m_sSelChassis = szChassisName;
		m_gridModel.SetRow(1);
	}
	else{
		if(nCurModelRow <= 0) {
			m_gridModel.SetRow(1);
		}
		else if( nCurModelRow >= m_gridModel.GetRows()){
			m_gridModel.SetRow(m_gridModel.GetRows() -1);
		}
		else{
			m_gridModel.SetRow(m_nModel_SelectRow);
		}
	}
	m_nChaassis_SelectRow = nChassisRow;
	OnClickModelGrid();
}

void CSelectChassisModelDlg::OnClickModelGrid() 
{
	POSITION	Pos				= NULL;
	CModelData* pModelData		= NULL;
	int			nModelRow		= m_gridModel.GetRow();
	int			nChassisRow		= m_gridChassis.GetRow();

	CString szChassisName = _T("");
	CString szModelName	  = _T("");

	if((nModelRow <= 0) &&  (m_gridModel.GetRows() > 1))
	{
		m_gridModel.SetRow(1); 
		nModelRow = 1;
	}
	else if( (nModelRow >= m_gridModel.GetRows()) && (m_gridModel.GetRows() > 1))
	{
		m_gridModel.SetRow(m_gridModel.GetRows() - 1); 
		nModelRow = m_gridModel.GetRows() - 1;
	}

	szChassisName = m_gridChassis.GetTextMatrix(nChassisRow,0);
	szModelName = m_gridModel.GetTextMatrix(nModelRow,0);
	m_sSelModel = szModelName;

	Pos = CurrentSet->ModelList.GetHeadPosition();

	while(Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if(pModelData->m_szModelName != _T(""))
		{
			if((pModelData->m_szChassisName.Compare(szChassisName) == NULL) &&
			   (pModelData->m_szModelName.Compare(szModelName) == NULL))
			{
				m_szSeqFilePath		  = pModelData->m_szSeqFilePath;
				m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				m_szRefImgFolder	  = pModelData->m_szRefImgFolder;

				break;
			}
		}
	}

	UpdateData(FALSE);
	m_nModel_SelectRow = nModelRow; 
	ChangeRowColor(nModelRow,MODEL_GRID);
//	delete pModelData;
}

BOOL CSelectChassisModelDlg::OnInitDialog() 
{
	//+add 090219(Modification No2)
	CString szLog = "B:OnInitDialog\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	CDialog::OnInitDialog();
	
	CString sTemp;
	// TODO: Add extra initialization here

	m_szCurrentSeqFilePath = CurrentSet->sServerFolder;
	m_szCurrentRefFilePath = CurrentSet->sRefRootFolder;

	UpdateData(FALSE);

//	if(OpenModelListFile(CurrentSet->sModelListPath,m_nNoModel) == FALSE) return;	
	
	InitGrid();

//	m_ctrlDivisionCmb.ResetContent();
	m_ctrlDivisionCmb.AddString("ALL");
	for(int i=0; i < g_Divisions.GetSize(); i++)
	{
		sTemp = g_Divisions.GetAt(i); 
		sTemp.Left(4);
		m_ctrlDivisionCmb.AddString(sTemp);
	}
	m_ctrlDivisionCmb.SetCurSel(0);
//	m_nSelDivision = 0;
	m_nDivision_Sel = 0;
	m_nChaassis_SelectRow = 1;
	m_nModel_SelectRow = 1;
	UpdateGrid();

	if(m_gridChassis.GetRows() > 1)
	{
		m_gridChassis.SetRow(1);
		OnClickChassisGrid();
	}
//	else{
//		OnBtnAdd();
//	}

	ShowWindow(SW_SHOW);
	
	//+add 090219(Modification No2)
	szLog = "E:OnInitDialog\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectChassisModelDlg::InitGrid()
{
	m_gridChassis.SetRedraw(FALSE);
	m_gridChassis.SetAllowBigSelection(FALSE);
	m_gridChassis.SetExtendLastCol(TRUE);
	m_gridChassis.SetCols(1);
	m_gridChassis.SetRows(1);
	m_gridChassis.SetFixedRows(1);
	m_gridChassis.SetFixedCols(0);
	m_gridChassis.SetGridLines(flexGridFlat);
	m_gridChassis.SetTextMatrix(0, 0, "CHASSIS LIST");
	m_gridChassis.SetRedraw(TRUE);
	m_gridChassis.Refresh();
	
	m_gridChassis.SetBackColorFixed(RGB(210,216,176));
	m_gridChassis.SetForeColorFixed(RGB(51,102,153));
	m_gridChassis.SetBackColor(RGB(247,247,231));
	m_gridChassis.SetGridColor(RGB(210,216,176));
	m_gridChassis.SetColAlignment(0, flexAlignCenterCenter);
	

	m_gridModel.SetRedraw(FALSE);
	m_gridModel.SetAllowBigSelection(FALSE);
	m_gridModel.SetExtendLastCol(TRUE);
	m_gridModel.SetCols(1);
	m_gridModel.SetRows(1);
	m_gridModel.SetFixedRows(1);
	m_gridModel.SetFixedCols(0);
	m_gridModel.SetGridLines(flexGridFlat);
	m_gridModel.SetTextMatrix(0, 0, "MODEL LIST");
	m_gridModel.SetRedraw(TRUE);
	m_gridModel.Refresh();

	m_gridModel.SetBackColorFixed(RGB(210,216,176));
	m_gridModel.SetForeColorFixed(RGB(51,102,153));
	m_gridModel.SetBackColor(RGB(247,247,231));
	m_gridModel.SetGridColor(RGB(210,216,176));
	m_gridModel.SetColAlignment(0, flexAlignCenterCenter);
}

void CSelectChassisModelDlg::UpdateGrid()
{
	POSITION	Pos				= NULL;
	CModelData* pModelData		= NULL;
	CString		szChassisName	= _T("");
	int			nChassisRow		= 1;
	CString sTemp;

	m_gridChassis.SetRedraw(FALSE);
	m_gridChassis.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	Pos = CurrentSet->ModelList.GetHeadPosition();

	while(Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		szChassisName = pModelData->m_szChassisName;
		
		if(IsSameChassis(szChassisName) != TRUE)
		{
			if(m_nDivision_Sel == 0)
			{
				m_gridChassis.SetRows(nChassisRow + 1);
				m_gridChassis.SetTextMatrix(nChassisRow++, 0, szChassisName);
			}
			else{
				m_ctrlDivisionCmb.GetLBText(m_nDivision_Sel,sTemp);
				if(sTemp == pModelData->m_szDivision)
				{
					m_gridChassis.SetRows(nChassisRow + 1);
					m_gridChassis.SetTextMatrix(nChassisRow++, 0, szChassisName);
				}
			}
		}
	}
	
	m_gridChassis.SetRedraw(TRUE);
	m_gridChassis.Refresh();

	if(m_gridChassis.GetRows() > 1)
	{
		m_gridChassis.SetRow(m_nChaassis_SelectRow);
		OnClickChassisGrid();
	}
}

BOOL CSelectChassisModelDlg::IsSameChassis(CString szChassisName)
{
	CString	szRegisteredChassisName	= _T("");
	
	int	nNoChassis	= m_gridChassis.GetRows() - 1;

	szChassisName.MakeUpper();

	for(int i=0; i<nNoChassis; i++)
	{
		 szRegisteredChassisName = m_gridChassis.GetTextMatrix(i+1, 0);

		 szRegisteredChassisName.MakeUpper();

		 if(szChassisName.Compare(szRegisteredChassisName) == NULL)
		 {
			 return TRUE;
		 }
	}
	return FALSE;
}

void CSelectChassisModelDlg::ChangeRowColor(int nRow, int nGridType)
{
	//===============
	// Chassis Grid
	//===============
	if(nGridType == CHASSIS_GRID)
	{
		for(int nTotalRow = 1; nTotalRow < m_gridChassis.GetRows(); nTotalRow++)
		{
			m_gridChassis.SetRow(nTotalRow);
			m_gridChassis.SetCol(0);
			m_gridChassis.SetCellBackColor(RGB(255,255,255));
			m_gridChassis.SetCellForeColor(RGB(0,0,0));
		}
		
		m_gridChassis.SetRow(nRow);
		m_gridChassis.SetCol(0);

//		m_gridChassis.SetCellBackColor(RGB(10, 36, 106 ));
		m_gridChassis.SetCellBackColor(RGB(255, 0, 255 ));
//		m_gridChassis.SetCellForeColor(RGB(255,255,255));
	}
	//===============
	// Chassis Grid
	//===============
	else 
	{
		for(int nTotalRow = 1; nTotalRow < m_gridModel.GetRows(); nTotalRow++)
		{
			m_gridModel.SetRow(nTotalRow);
			m_gridModel.SetCol(0);
			m_gridModel.SetCellBackColor(RGB(255,255,255));
			m_gridModel.SetCellForeColor(RGB(0,0,0));
		}
		
		m_gridModel.SetRow(nRow);
		m_gridModel.SetCol(0);

//		m_gridModel.SetCellBackColor(RGB(10, 36,   106 ));
		m_gridModel.SetCellBackColor(RGB(255, 0,   255 ));
//		m_gridModel.SetCellForeColor(RGB(255,255,255));
	}
}

void CSelectChassisModelDlg::OnBtnAdd() 
{
	
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	int			nModelRow		= m_gridModel.GetRow();

	CAddModelDlg dlg;

	GetWindowRect(&dlg.m_ParentWndRect);

	if(dlg.DoModal() == IDOK)
	{
		CModelData* pModel = new CModelData;

		pModel->m_szChassisName		  = dlg.m_szChassisName;
		pModel->m_szModelName		  = dlg.m_szModelName;
		pModel->m_szSeqFilePath		  = dlg.m_szSeqPath;
		pModel->m_szModelInfoFilePath = dlg.m_szModelInfoPath;
		pModel->m_szRefImgFolder	  = dlg.m_szRefFolderPath;

		if(CheckModel(pModel->m_szChassisName, 	pModel->m_szModelName))
		{
			CurrentSet->ModelList.AddTail(pModel);
			UpdateGrid();

			SaveModelListFile();
		}
		else{
			CString sTmp = _T("Error : ChassisName & ModelName");
			AfxMessageBox(sTmp); 
		}
	
//		delete pModel;
	}
}

void CSelectChassisModelDlg::OnBtnDel() 
{
	// TODO: Add your control notification handler code here
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	
	int	nChassisRow	= m_gridChassis.GetRow();
	int	nModelRow	= m_gridModel.GetRow();

	if((nChassisRow <= 0) || (nChassisRow >= m_gridChassis.GetRows())) return;
	if((nModelRow <= 0)	  || (nModelRow >= m_gridModel.GetRows())) return;

	POSITION	Pos				= NULL;
	POSITION	CurrentPos		= NULL;
	CModelData* pModelData		= NULL;
	CString		szChassisName	= _T("");
	CString		szModelName		= _T("");

	szChassisName	= m_gridChassis.GetTextMatrix(nChassisRow, 0);
	szModelName		= m_gridModel.GetTextMatrix(nModelRow, 0);

	Pos = CurrentSet->ModelList.GetHeadPosition();

	while(Pos)
	{
		CurrentPos = Pos;
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if((szChassisName.Compare(pModelData->m_szChassisName) == NULL)
		&& (szModelName.Compare(pModelData->m_szModelName) == NULL))
		{
			CurrentSet->ModelList.RemoveAt(CurrentPos);
			delete pModelData;
			break;
		}
	}

	UpdateGrid();

	SaveModelListFile();
}

void CSelectChassisModelDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	// TODO: Add extra validation here
	if(m_szSeqFilePath == _T(""))
	{
		AfxMessageBox("Incorrect Sequence File"); return;
	}

	if(m_szModelInfoFilePath == _T(""))
	{
		AfxMessageBox("Incorrect Model Info File"); return;
	}

	if(m_szRefImgFolder == _T(""))
	{
		AfxMessageBox("Incorrect Reference Folder "); return;
	}

	CDialog::OnOK();
}

void CSelectChassisModelDlg::OnBtnEdit() 
{
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}

	POSITION	Pos				= NULL;
	POSITION	CurrentPos		= NULL;
	CModelData* pModelData		= NULL;
	int			nChassisRow		= m_gridChassis.GetRow();
	int			nModelRow		= m_gridModel.GetRow();
	CString szChassisName = _T("");
	CString szModelName	  = _T("");
	
	if((nChassisRow <= 0) || (nChassisRow >= m_gridChassis.GetRows())) return;

	szChassisName = m_gridChassis.GetTextMatrix(nChassisRow,0);


	if((nModelRow <= 0) || (nModelRow >= m_gridModel.GetRows())) return;

	szModelName = m_gridModel.GetTextMatrix(nModelRow,0);

	Pos = CurrentSet->ModelList.GetHeadPosition();
	while(Pos)
	{
		CurrentPos = Pos;
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if((szChassisName.Compare(pModelData->m_szChassisName) == NULL)
		&& (szModelName.Compare(pModelData->m_szModelName) == NULL))
		{
//			delete pModelData;
			break;
		}
	}


	CAddModelDlg* pDlg = new CAddModelDlg;

	GetWindowRect(&pDlg->m_ParentWndRect);

	pDlg->m_sWinText  = _T("Edit Model");
	pDlg->m_szChassisName = szChassisName;
	pDlg->m_szModelName = szModelName;
	pDlg->m_szSeqPath = m_szSeqFilePath;
	pDlg->m_szModelInfoPath = m_szModelInfoFilePath;
	pDlg->m_szRefFolderPath = m_szRefImgFolder;

	if(pDlg->DoModal() == IDOK) 
	{
		CModelData* pModel = new CModelData;

		pModel->m_szChassisName		  = pDlg->m_szChassisName;
		pModel->m_szModelName		  = pDlg->m_szModelName;
		pModel->m_szSeqFilePath		  = pDlg->m_szSeqPath;
		pModel->m_szModelInfoFilePath = pDlg->m_szModelInfoPath;
		pModel->m_szRefImgFolder	  = pDlg->m_szRefFolderPath;
		

		CurrentSet->ModelList.SetAt(CurrentPos, pModel);

		UpdateGrid();

		SaveModelListFile(); 
	//	delete pModel;
	}
	delete pDlg;
}

void CSelectChassisModelDlg::OnBtnCopy() 
{
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}

	POSITION	Pos				= NULL;
	POSITION	CurrentPos		= NULL;
	CModelData* pModelData		= NULL;
	int			nChassisRow		= m_gridChassis.GetRow();
	int			nModelRow		= m_gridModel.GetRow();
	CString szChassisName = _T("");
	CString szModelName	  = _T("");
	
	if((nChassisRow <= 0) || (nChassisRow >= m_gridChassis.GetRows())) return;

	szChassisName = m_gridChassis.GetTextMatrix(nChassisRow,0);


	if((nModelRow <= 0) || (nModelRow >= m_gridModel.GetRows())) return;

	szModelName = m_gridModel.GetTextMatrix(nModelRow,0);


	CAddModelDlg* pDlg = new CAddModelDlg;

	GetWindowRect(&pDlg->m_ParentWndRect);

	pDlg->m_sWinText  = _T("Edit Model");
	pDlg->m_szChassisName = szChassisName;
	pDlg->m_szModelName = szModelName;
	pDlg->m_szSeqPath = m_szSeqFilePath;
	pDlg->m_szModelInfoPath = m_szModelInfoFilePath;
	pDlg->m_szRefFolderPath = m_szRefImgFolder;

	if(pDlg->DoModal() == IDOK) 
	{
		CModelData* pModel = new CModelData;

		pModel->m_szChassisName		  = pDlg->m_szChassisName;
		pModel->m_szModelName		  = pDlg->m_szModelName;
		pModel->m_szSeqFilePath		  = pDlg->m_szSeqPath;
		pModel->m_szModelInfoFilePath = pDlg->m_szModelInfoPath;
		pModel->m_szRefImgFolder	  = pDlg->m_szRefFolderPath;
		

		if(CheckModel(pModel->m_szChassisName, 	pModel->m_szModelName))
		{
			CurrentSet->ModelList.AddTail(pModel);

			UpdateGrid();

			SaveModelListFile();
		}
		else{
			CString sTmp = _T("Error : ChassisName & ModelName");
			AfxMessageBox(sTmp); 
		}
	//	delete pModel;
	}
	delete pDlg;	
}

BOOL CSelectChassisModelDlg::CheckModel(CString sChassis, CString sModel)
{
	POSITION	Pos				= NULL;
	CModelData* pModelData		= NULL;
	int			nChassisRow		= m_gridChassis.GetRow();
	int			nModelRow		= m_gridModel.GetRow();
	CString szChassisName = _T("");
	CString szModelName	  = _T("");
	
	if((nChassisRow <= 0) || (nChassisRow >= m_gridChassis.GetRows())){
		szChassisName = _T("");
	}
	else{
		szChassisName = m_gridChassis.GetTextMatrix(nChassisRow,0);

	}

	if((nModelRow <= 0) || (nModelRow >= m_gridModel.GetRows())){
		szModelName	  = _T("");
	}
	else{
		szModelName = m_gridModel.GetTextMatrix(nModelRow,0);
	}

	Pos = CurrentSet->ModelList.GetHeadPosition();
	while(Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);
		
//		if(pModelData == NULL) continue;
		if((pModelData->m_szChassisName != _T("")) && (pModelData->m_szModelName != _T("")))
		{
			if((sChassis.Compare(pModelData->m_szChassisName) == NULL)
			&& (sModel.Compare(pModelData->m_szModelName) == NULL))
			{
//				delete pModelData;
				return FALSE;
			}
		}
	}
	if((szChassisName != _T("")) && (szModelName != _T("")))
	{
			if((sChassis.Compare(szChassisName) == NULL) && (sModel.Compare(szModelName) == NULL))
			{
				return FALSE;
			}
	}

	return TRUE;
}


void CSelectChassisModelDlg::OnSelchangeCombo1() 
{
	m_nDivision_Sel = m_ctrlDivisionCmb.GetCurSel();
	UpdateGrid();	
}
