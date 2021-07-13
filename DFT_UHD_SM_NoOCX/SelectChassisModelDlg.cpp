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
	, m_szModelSuffix(_T(""))
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
//	DDX_Control(pDX, IDC_CHASSIS_GRID, m_gridChassis);
//	DDX_Control(pDX, IDC_MODEL_GRID, m_gridModel);
	DDX_Text(pDX, IDC_EDIT_CURRENT_SEQ, m_szCurrentSeqFilePath);
	DDX_Text(pDX, IDC_EDIT_MODEL_INFO, m_szModelInfoFilePath);
	DDX_Text(pDX, IDC_EDIT_REF_FOLDER, m_szRefImgFolder);
	DDX_Text(pDX, IDC_EDIT_SEQ, m_szSeqFilePath);
	DDX_Text(pDX, IDC_EDIT_CURRENT_REF, m_szCurrentRefFilePath);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_MODEL_SUFFIX, m_szModelSuffix);
	DDX_Control(pDX, IDC_LIST_SEL_CHASSIS, m_cListChassis);
	DDX_Control(pDX, IDC_LIST_SEL_MODEL, m_cListModel);
}


BEGIN_MESSAGE_MAP(CSelectChassisModelDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectChassisModelDlg)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBtnEdit)
	ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SERARCH_MODEL, &CSelectChassisModelDlg::OnBnClickedButtonSerarchModel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SEL_CHASSIS, &CSelectChassisModelDlg::OnNMClickListChassis)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SEL_MODEL, &CSelectChassisModelDlg::OnNMClickListModel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SEL_CHASSIS, &CSelectChassisModelDlg::OnNMCustomdrawListChassis)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SEL_MODEL, &CSelectChassisModelDlg::OnNMCustomdrawListModel)
	ON_MESSAGE(UM_UPDATE_SUFFIX_SCAN, GetModelSuffix)

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
#if 1
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;

	//int			nChassisRow = (int)m_cListChassis.GetFirstSelectedItemPosition()- 1; //  m_gridChassis.GetRow();
	//int			nCurModelRow = (int)m_cListModel.GetFirstSelectedItemPosition()-1;// //m_gridModel.GetRow();
	int			nChassisRow = m_nChaassis_SelectRow;
	int			nCurModelRow = m_nModel_SelectRow;// //m_gridModel.GetRow();

	int			nModelRow = 0;

	CString szChassisName = _T("");
	CString szModelName = _T("");

	if ((nChassisRow <= 0) && (m_cListChassis.GetItemCount() > 1))
	{
		//m_gridChassis.SetRow(1); 
		nChassisRow = 0;
	}
	else if ((nChassisRow >= m_cListChassis.GetItemCount()) && (m_cListChassis.GetItemCount() > 1))
	{
		//m_gridChassis.SetRow(m_gridChassis.GetRows() - 1); 
		nChassisRow = m_cListChassis.GetItemCount() - 1;
	}


	szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);// m_gridChassis.GetTextMatrix(nChassisRow,0);

	Pos = CurrentSet->ModelList.GetHeadPosition();
	m_cListModel.DeleteAllItems();
	while (Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if (pModelData->m_szChassisName != _T(""))
		{
			if (pModelData->m_szChassisName.Compare(szChassisName) == NULL)
			{
				szModelName = pModelData->m_szModelName;

				//m_gridModel.SetRows(nModelRow + 1);
				m_cListModel.InsertItem(nModelRow++, szModelName); //m_cListModel.SetItemText(nModelRow++, 0, szModelName);// //m_gridModel.SetTextMatrix(nModelRow++, 0, szModelName);
				//SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
			}
		}
	}
	ChangeRowColor(nChassisRow, CHASSIS_GRID);
	if (m_sSelChassis != szChassisName)
	{
		m_sSelChassis = szChassisName;
		m_cListModel.SetSelectionMark(0);//m_cListModelEx.SetCellBackColor(); //m_gridModel.SetRow(1);
	}
	else {
		if (nCurModelRow <= 0) {
			m_cListModel.SetSelectionMark(0);//m_gridModel.SetRow(1);
		}
		else if (nCurModelRow >= m_cListModel.GetItemCount())//m_gridModel.GetRows())
		{
			m_cListModel.SetSelectionMark(m_cListModel.GetItemCount() - 1);//mm_gridModel.SetRow(m_gridModel.GetRows() -1);
		}
		else {
			m_cListModel.SetSelectionMark(m_nModel_SelectRow);//m_gridModel.SetRow(m_nModel_SelectRow);
}
	}
	m_nChaassis_SelectRow = nChassisRow;
	m_cListChassis.Invalidate();
	OnClickModelGrid();
#else
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

#endif
}

void CSelectChassisModelDlg::OnClickModelGrid() 
{

#if 1

	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;

	//int			nChassisRow = (int)m_cListChassis.GetFirstSelectedItemPosition() - 1; //  m_gridChassis.GetRow();
	//int			nModelRow = (int)m_cListModel.GetFirstSelectedItemPosition() - 1;// //m_gridModel.GetRow();
	int			nChassisRow = m_nChaassis_SelectRow;
	int			nModelRow = m_nModel_SelectRow;// //m_gridModel.GetRow();

	CString szChassisName = _T("");
	CString szModelName = _T("");

	//	if((nModelRow <= 0) &&  (m_gridModel.GetRows() > 1))
	if ((nModelRow <= 0) && (m_cListModel.GetItemCount() > 1))//.GetRows() > 1))
	{
		//m_gridModel.SetRow(1); 
		m_cListModel.SetSelectionMark(0);
		nModelRow = 0;
	}
	else if ((nModelRow >= m_cListModel.GetItemCount()) && (m_cListModel.GetItemCount() > 1))
	{
		m_cListModel.SetSelectionMark(m_cListModel.GetItemCount() - 1);//m_gridModel.SetRow(m_gridModel.GetRows() - 1); 
		nModelRow = m_cListModel.GetItemCount() - 1;
	}

	szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);// m_gridChassis.GetTextMatrix(nChassisRow, 0);
	szModelName = m_cListModel.GetItemText(nModelRow, 0);//m_gridModel.GetTextMatrix(nModelRow,0);
	m_sSelModel = szModelName;

	Pos = CurrentSet->ModelList.GetHeadPosition();

	while (Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if (pModelData->m_szModelName != _T(""))
		{
			if ((pModelData->m_szChassisName.Compare(szChassisName) == NULL) &&
				(pModelData->m_szModelName.Compare(szModelName) == NULL))
			{
				m_szSeqFilePath = pModelData->m_szSeqFilePath;
				m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				m_szRefImgFolder = pModelData->m_szRefImgFolder;
				//m_szDivision = pModelData->m_szDivision;
				m_szModelSuffix = pModelData->m_szModelSuffix;

				break;
			}
		}
	}

	UpdateData(FALSE);
	m_nModel_SelectRow = nModelRow;
	ChangeRowColor(nModelRow, MODEL_GRID);

	m_cListModel.Invalidate();
//	AfxMessageBox("Open");
#else


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
				m_szModelSuffix =  pModelData->m_szModelSuffix;
				break;
			}
		}
	}

	UpdateData(FALSE);
	m_nModel_SelectRow = nModelRow; 
	ChangeRowColor(nModelRow,MODEL_GRID);
//	delete pModelData;

#endif
}

BOOL CSelectChassisModelDlg::OnInitDialog() 
{
	//+add 090219(Modification No2)
	CString szLog = "B:OnInitDialog\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	CDialog::OnInitDialog();
	
	m_bActivate = 1;
	CString sTemp;
	// TODO: Add extra initialization here

	m_szCurrentSeqFilePath = CurrentSet->sServerFolder;
	m_szCurrentRefFilePath = CurrentSet->sRefRootFolder;

	UpdateData(FALSE);

//	if(OpenModelListFile(CurrentSet->sModelListPath,m_nNoModel) == FALSE) return;	
	
	InitGrid();
	//AfxMessageBox("InitGrid();");
//	m_ctrlDivisionCmb.ResetContent();
	m_ctrlDivisionCmb.AddString("ALL");
	for(int i=0; i < g_Divisions.GetSize(); i++)
	{
		sTemp = g_Divisions.GetAt(i); 
		sTemp.Left(4);
		m_ctrlDivisionCmb.AddString(sTemp);
	}
	//AfxMessageBox("m_ctrlDivisionCmb.AddString(sTemp);");
	m_ctrlDivisionCmb.SetCurSel(0);
//	m_nSelDivision = 0;
	m_nDivision_Sel = 0;
	m_nChaassis_SelectRow = 0;
	m_nModel_SelectRow = 0;
	UpdateGrid();
	//AfxMessageBox("UpdateGrid();");
	if(m_cListChassis.GetItemCount() > 1)
	{
		//m_gridChassis.SetRow(1);
		OnClickChassisGrid();
	}
//	else{
//		OnBtnAdd();
//	}
	//AfxMessageBox("OnClickChassisGrid();");
	ShowWindow(SW_SHOW);
	
	//+add 090219(Modification No2)
	szLog = "E:OnInitDialog\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectChassisModelDlg::InitGrid()
{
#if 1
	m_cListChassis.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가

	m_cListChassis.InsertColumn(0, _T("CHASSIS LIST"), LVCFMT_CENTER, 255);
	m_cListChassis.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
	m_cListChassis.Invalidate();

	m_cListModel.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가

	m_cListModel.InsertColumn(0, _T("MODEL LIST"), LVCFMT_CENTER, 270);
	m_cListModel.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
	m_cListModel.Invalidate();

#else
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

#endif
}

void CSelectChassisModelDlg::UpdateGrid()
{
#if 1
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	CString		szChassisName = _T("");
	int			nChassisRow = 0;
	CString sTemp;

	//m_cListChassis);	
	//m_cListModel);
	m_cListChassis.DeleteAllItems();//  m_gridChassis.SetRedraw(FALSE);
	//m_gridChassis.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	Pos = CurrentSet->ModelList.GetHeadPosition();

	while (Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		szChassisName = pModelData->m_szChassisName;
	//	AfxMessageBox(szChassisName);
		if (IsSameChassis(szChassisName) != TRUE)
		{

			//m_gridChassis.SetRows(nChassisRow + 1);
			//m_gridChassis.SetTextMatrix(nChassisRow++, 0, szChassisName);
			m_cListChassis.InsertItem(nChassisRow++, szChassisName);   // 첫째행(0), 첫째열에 삽입

			//if (nChassisRow > 50)
			//	break;
		}
	}

	m_cListChassis.Invalidate();//m_gridChassis.SetRedraw(TRUE);
	//m_gridChassis.Refresh();
	if (m_cListChassis.GetItemCount() >= 1)
	{
		//AfxMessageBox("(m_cListChassis.GetItemCount() >= 1)");
		m_cListChassis.SetSelectionMark(m_nChaassis_SelectRow + 1);// GetFirstSelectedItemPosition(); //m_gridChassis.SetRow(m_nChaassis_SelectRow);
		OnClickChassisGrid();
	}
	//else
	//{
	//	AfxMessageBox("(m_cListChassis.GetItemCount() == 0)");
	//}
	//	SetTimer(1, 300, NULL);


#else
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

#endif
}
void CSelectChassisModelDlg::UpdateGridLast()
{
	KillTimer(1);
	return;
#if 1
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	CString		szChassisName = _T("");
	int			nChassisRow = 0;
	CString sTemp;
	int lFlagBreak = 0;

	//m_cListChassis);	
	//m_cListModel);
	//m_cListChassis.DeleteAllItems();//  m_gridChassis.SetRedraw(FALSE);
	//m_gridChassis.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	Pos = CurrentSet->ModelList.GetHeadPosition();
	int lPos = m_cListChassis.GetItemCount();
	int lMpos = CurrentSet->ModelList.GetCount();

	if (lPos >= lMpos)
	{
		KillTimer(1);
	}

	while (Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		szChassisName = pModelData->m_szChassisName;
		if (nChassisRow < lPos)
		{
			nChassisRow++;
		}
		else
		{
			if (IsSameChassis(szChassisName) != TRUE)
			{
				//m_gridChassis.SetRows(nChassisRow + 1);
				//m_gridChassis.SetTextMatrix(nChassisRow++, 0, szChassisName);
				m_cListChassis.InsertItem(nChassisRow++, szChassisName);   // 첫째행(0), 첫째열에 삽입

				if (nChassisRow > 50 + lPos)
				{
					lFlagBreak = 1;
					break;
				}
			}
		}
	}

	if (lFlagBreak)
	{

	}
	else
	{
		KillTimer(1);
	}

	m_cListChassis.Invalidate();//m_gridChassis.SetRedraw(TRUE);
	//m_gridChassis.Refresh();
	//if (m_cListChassis.GetItemCount() > 1)
	//{
	//	m_cListChassis.SetSelectionMark(m_nChaassis_SelectRow + 1);// GetFirstSelectedItemPosition(); //m_gridChassis.SetRow(m_nChaassis_SelectRow);
	//	OnClickChassisGrid();
	//}



#else
	//POSITION	Pos = NULL;
	//CModelData* pModelData = NULL;
	//CString		szChassisName = _T("");
	//int			nChassisRow = 1;
	//long        nRows;
	//m_gridChassis.SetRedraw(FALSE);
	//m_gridChassis.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything)));

	//Pos = ModelList.GetHeadPosition();

	//while (Pos)
	//{
	//	pModelData = ModelList.GetNext(Pos);

	//	szChassisName = pModelData->m_szChassisName;

	//	if (IsSameChassis(szChassisName) != TRUE)
	//	{
	//		m_gridChassis.SetRows(nChassisRow + 1);
	//		m_gridChassis.SetTextMatrix(nChassisRow++, 0, szChassisName);
	//	}
	//}
	//if (nChassisRow == 1)
	//{
	//	m_gridChassis.SetRows(1);
	//	m_gridModel.SetRows(1);
	//}
	//m_gridChassis.SetRedraw(TRUE);
	//m_gridChassis.Refresh();

	//nRows = m_gridChassis.GetRows();
	//if (nRows > 1)
	//{
	//	if (m_nChaassis_SelectRow >= nRows) m_nChaassis_SelectRow = nRows - 1;

	//	m_gridChassis.SetRow(m_nChaassis_SelectRow);
	//	OnClickChassisGrid();
	//}

#endif
}

BOOL CSelectChassisModelDlg::IsSameChassis(CString szChassisName)
{

#if 1
	CString	szRegisteredChassisName = _T("");
	//	POSITION lpos = m_cListChassis.GetFirstSelectedItemPosition();
	//	int	nNoChassis = (int)lpos;   //m_gridChassis.GetRows() - 1;
	int	nNoChassis = m_cListChassis.GetItemCount();
	szChassisName.MakeUpper();

	for (int i = 0; i < nNoChassis; i++)
	{
		szRegisteredChassisName = m_cListChassis.GetItemText(i, 0); // .GetTextMatrix(i+1, 0);

		szRegisteredChassisName.MakeUpper();

		if (szChassisName.Compare(szRegisteredChassisName) == NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
#else

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

#endif
}

void CSelectChassisModelDlg::ChangeRowColor(int nRow, int nGridType)
{

#if 0
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

#endif
}

void CSelectChassisModelDlg::OnBtnAdd() 
{
#if 0
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

#endif
}

void CSelectChassisModelDlg::OnBtnDel() 
{
#if 0
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
#endif
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
#if 0
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

#endif
}

void CSelectChassisModelDlg::OnBtnCopy() 
{

#if 0
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
#endif
}

BOOL CSelectChassisModelDlg::CheckModel(CString sChassis, CString sModel)
{
#if 0
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
#endif
	return TRUE;
}


void CSelectChassisModelDlg::OnSelchangeCombo1() 
{
	m_nDivision_Sel = m_ctrlDivisionCmb.GetCurSel();
	UpdateGrid();	
}


void CSelectChassisModelDlg::OnBnClickedButtonSerarchModel()
{

#if 1
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;

	int			nModelRow = 0;// m_gridModel.GetRow();
	int			nChassisRow = 0;// m_gridChassis.GetRow();
	int			nChassisRowNum = m_cListChassis.GetItemCount();
//	CListCtrl m_cListChassis;
//	CListCtrl m_cListModel;

	CString lfind_szModelSuffix;
	CString lfind_szSeqFilePath;;
	CString lfind_szModelInfoFilePath;
	CString lfind_szRefImgFolder;
	CString lfind_szChassisName;
	CString lfind_szModelName;

	CString szChassisName = _T("");
	CString szModelName = _T("");



	UpdateData(TRUE);


	Pos = CurrentSet->ModelList.GetHeadPosition();

	while (Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if (pModelData->m_szModelName != _T(""))
		{
			if (pModelData->m_szModelSuffix.Compare(m_szModelSuffix) == NULL)
			{
				//m_szSeqFilePath = pModelData->m_szSeqFilePath;
				//m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				//m_szRefImgFolder = pModelData->m_szRefImgFolder;

				// szChassisName = pModelData->m_szChassisName;
				// szModelName = pModelData->m_szModelName;
				//m_szModelSuffix = pModelData->m_szModelSuffix;

				lfind_szSeqFilePath = pModelData->m_szSeqFilePath;
				lfind_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				lfind_szRefImgFolder = pModelData->m_szRefImgFolder;

				lfind_szChassisName = pModelData->m_szChassisName;
				lfind_szModelName = pModelData->m_szModelName;
				lfind_szModelSuffix = pModelData->m_szModelSuffix;

				//UpdateData(FALSE);
				break;
			}
		}
	}
	CString szReadChassisName;

	if (lfind_szChassisName.GetLength() > 0)
		for (int i = 0; i < nChassisRowNum; i++)
		{
			szReadChassisName = m_cListChassis.GetItemText(i , 0);
			//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);
				//if (szReadChassisName.Compare(szChassisName) == NULL)
			if (szReadChassisName == lfind_szChassisName)
			{
				nChassisRow = i ;
				//m_gridChassis.SetRow(nChassisRow);
				//ChangeRowColor(nChassisRow, CHASSIS_GRID);
				m_nChaassis_SelectRow = nChassisRow;
				//OnClickModelGrid();
				ChangeRowColor(nChassisRow, CHASSIS_GRID);
				break;
			}
		}

	if (m_nChaassis_SelectRow >= 0)
	{
		//GetItemText(nChassisRow, 0);//m_gridChassis.SetRow(m_nChaassis_SelectRow);
		OnClickChassisGrid();
	}
	//Pos = CurrentSet->ModelList.GetHeadPosition();
	//nModelRow = 0;
	//while (Pos)
	//{
	//	pModelData = CurrentSet->ModelList.GetNext(Pos);

	//	if (pModelData->m_szChassisName != _T(""))
	//	{
	//		if (pModelData->m_szChassisName == szChassisName)
	//		{
	//			szModelName = pModelData->m_szModelName;
	//			m_gridModel.SetRows(nModelRow + 1);
	//			m_gridModel.SetTextMatrix(nModelRow++, 0, szModelName);
	//		}
	//	}
	//}

	CString szReadModelName;
	int			nModelRowNum = m_cListModel.GetItemCount();
	for (int i = 0; i < nModelRowNum; i++)
	{
		szReadModelName = m_cListModel.GetItemText(i, 0);
		//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);
		if (szReadModelName == lfind_szModelName)
		{
			nModelRow = i ;
			//m_gridModel.SetRow(nModelRow);
			//ChangeRowColor(nChassisRow, CHASSIS_GRID);
			m_nModel_SelectRow = nModelRow;
			//OnClickModelGrid();
			ChangeRowColor(nModelRow, MODEL_GRID);
			break;
		}
	}

	//m_szSeqFilePath = pModelData->m_szSeqFilePath;
				//m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				//m_szRefImgFolder = pModelData->m_szRefImgFolder;

				// szChassisName = pModelData->m_szChassisName;
				// szModelName = pModelData->m_szModelName;
				//m_szModelSuffix = pModelData->m_szModelSuffix;

	m_szSeqFilePath = lfind_szSeqFilePath;
	m_szModelInfoFilePath = lfind_szModelInfoFilePath;
	m_szRefImgFolder = lfind_szRefImgFolder;

	m_sSelChassis = lfind_szChassisName;
	m_sSelModel = lfind_szModelName;
	m_szModelSuffix = lfind_szModelSuffix;

	UpdateData(FALSE);

#else
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;

	int			nModelRow = m_gridModel.GetRow();
	int			nChassisRow = m_gridChassis.GetRow();
	int			nChassisRowNum = m_gridChassis.GetRows();


	CString lfind_szModelSuffix;
	CString lfind_szSeqFilePath;;
	CString lfind_szModelInfoFilePath;
	CString lfind_szRefImgFolder;
	CString lfind_szChassisName;
	CString lfind_szModelName;

	CString szChassisName = _T("");
	CString szModelName = _T("");

	

	UpdateData(TRUE);


	Pos = CurrentSet->ModelList.GetHeadPosition();

	while (Pos)
	{
		pModelData = CurrentSet->ModelList.GetNext(Pos);

		if (pModelData->m_szModelName != _T(""))
		{
			if (pModelData->m_szModelSuffix.Compare(m_szModelSuffix) == NULL)
			{
				//m_szSeqFilePath = pModelData->m_szSeqFilePath;
				//m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				//m_szRefImgFolder = pModelData->m_szRefImgFolder;

				// szChassisName = pModelData->m_szChassisName;
				// szModelName = pModelData->m_szModelName;
				//m_szModelSuffix = pModelData->m_szModelSuffix;

				lfind_szSeqFilePath = pModelData->m_szSeqFilePath;
				lfind_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				lfind_szRefImgFolder = pModelData->m_szRefImgFolder;

				lfind_szChassisName = pModelData->m_szChassisName;
				lfind_szModelName = pModelData->m_szModelName;
				lfind_szModelSuffix = pModelData->m_szModelSuffix;

			//UpdateData(FALSE);
				break;
			}
		}
	}
	CString szReadChassisName;

	if(lfind_szChassisName.GetLength() > 0)
	for (int i = 0; i < nChassisRowNum; i++)
	{
		szReadChassisName = m_gridChassis.GetTextMatrix(i+1, 0);
	//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);
		//if (szReadChassisName.Compare(szChassisName) == NULL)
		if (szReadChassisName == lfind_szChassisName)
		{
			nChassisRow = i + 1;
			m_gridChassis.SetRow(nChassisRow);
			//ChangeRowColor(nChassisRow, CHASSIS_GRID);
			m_nChaassis_SelectRow = nChassisRow;
			//OnClickModelGrid();
			ChangeRowColor(nChassisRow, CHASSIS_GRID);
			break;
		}
	}
	
	if (m_gridChassis.GetRows() > 1)
	{
		m_gridChassis.SetRow(m_nChaassis_SelectRow);
		OnClickChassisGrid();
	}
	//Pos = CurrentSet->ModelList.GetHeadPosition();
	//nModelRow = 0;
	//while (Pos)
	//{
	//	pModelData = CurrentSet->ModelList.GetNext(Pos);

	//	if (pModelData->m_szChassisName != _T(""))
	//	{
	//		if (pModelData->m_szChassisName == szChassisName)
	//		{
	//			szModelName = pModelData->m_szModelName;
	//			m_gridModel.SetRows(nModelRow + 1);
	//			m_gridModel.SetTextMatrix(nModelRow++, 0, szModelName);
	//		}
	//	}
	//}

	CString szReadModelName;
	int			nModelRowNum = m_gridModel.GetRows();
	for (int i = 0; i < nModelRowNum; i++)
	{
		szReadModelName = m_gridModel.GetTextMatrix(i + 1, 0);
		//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);
		if (szReadModelName == lfind_szModelName)
		{
			nModelRow = i + 1;
			m_gridModel.SetRow(nModelRow);
			//ChangeRowColor(nChassisRow, CHASSIS_GRID);
			//m_nChaassis_SelectRow = nModelRow;
			//OnClickModelGrid();
			ChangeRowColor(nModelRow, MODEL_GRID);
			break;
		}
	}
	
	//m_szSeqFilePath = pModelData->m_szSeqFilePath;
				//m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				//m_szRefImgFolder = pModelData->m_szRefImgFolder;

				// szChassisName = pModelData->m_szChassisName;
				// szModelName = pModelData->m_szModelName;
				//m_szModelSuffix = pModelData->m_szModelSuffix;

	m_szSeqFilePath = lfind_szSeqFilePath;
	m_szModelInfoFilePath = lfind_szModelInfoFilePath;
	m_szRefImgFolder = lfind_szRefImgFolder;

	m_sSelChassis = lfind_szChassisName;
	m_sSelModel = lfind_szModelName ;
	m_szModelSuffix = lfind_szModelSuffix ;

	UpdateData(FALSE);
#endif
}

void CSelectChassisModelDlg::OnNMClickListChassis(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nChaassis_SelectRow = (int)m_cListChassis.GetFirstSelectedItemPosition() - 1; //  m_gridChassis.GetRow();
	//int			nModelRow = (int)m_cListModel.GetFirstSelectedItemPosition() - 1;// //m_gridModel.GetRow();
	//int			nChassisRow = m_nChaassis_SelectRow;
	//int			nModelRow = m_nModel_SelectRow;// //m_gridModel.GetRow();
	m_cListChassis.Invalidate();
	OnClickChassisGrid();
	*pResult = 0;
}


void CSelectChassisModelDlg::OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	m_nChaassis_SelectRow = (int)m_cListChassis.GetFirstSelectedItemPosition() - 1; //  m_gridChassis.GetRow();
	m_nModel_SelectRow = (int)m_cListModel.GetFirstSelectedItemPosition() - 1;// //m_gridModel.GetRow();
	m_cListModel.Invalidate();
	//int			nChassisRow = m_nChaassis_SelectRow;
	//int			nModelRow = m_nModel_SelectRow;// //m_gridModel.GetRow();
	OnClickModelGrid();
	*pResult = 0;
}


void CSelectChassisModelDlg::OnNMCustomdrawListChassis(NMHDR *pNMHDR, LRESULT *pResult)
{
	//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	//int nRow=0;
	//int nCol=0;
	CString strCol = _T("");



	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}
	case CDDS_ITEMPREPAINT:
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		return;
	}
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
	{
		COLORREF crText, crBkgnd;

		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
		//	nRow= m_CtrlListMainProcess.GetItemCount();

		int l_nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
		int l_nSubItem = (int)(pLVCD->iSubItem);


		if (l_nItem == m_nChaassis_SelectRow)
		{
			crText = RGB(0, 0, 0); ////m_cListChassisEx.nSelForeColor[l_nItem][l_nSubItem];// RGB(0, 0, 0); //글자색
			crBkgnd = RGB(255, 0, 255); ////m_cListChassisEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
		}
		else
		{
			crText = RGB(0, 0, 0); ////m_cListChassisEx.nSelForeColor[l_nItem][l_nSubItem];// RGB(0, 0, 0); //글자색
			crBkgnd = RGB(255, 255, 255); ////m_cListChassisEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
		}


		//		if (2 == (int)(pLVCD->iSubItem))
		//		{
		//			strCol = m_CtrlListMainProcess.GetItemText(pLVCD->nmcd.dwItemSpec, 2);
		//			if (strCol == "NG") // 두번째열의 값이 "10"일 때, 그 값 부분만			
		//			{
		//				crText = RGB(0, 0, 0); //글자색
		//				crBkgnd = RGB(255, 0, 0); //배경색으로 한다    
		//			}
		//			else if (2 == (int)(pLVCD->iSubItem) && strCol == "OK")
		//			{
		//				crText = RGB(0, 0, 0);
		//				crBkgnd = RGB(0, 255, 0);
		//			}
		//			else
		//			{
		//				if ((nItem == m_nCurrentNumber) && (m_TaskRun != TASK_RUN_IDLE))//else if(nItem == m_Process_Index)          
		//				{
		//					crText = RGB(0, 0, 0);
		//					crBkgnd = RGB(255, 255, 100);
		//				}
		//				else
		//				{
		//					crText = RGB(0, 0, 0);
		//					crBkgnd = RGB(255, 255, 255);
		//				}
		//			}
		//
		//		}
		//		else
		//		{
		//			//int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
		//			//m_TaskRun;m_Process_Index
		////int		m_nCurrentNumber;
		//			if ((nItem == m_nCurrentNumber) && (m_TaskRun != TASK_RUN_IDLE))
		//			{
		//				//if(pLVCD->iSubItem >= 1)
		//				{
		//					crText = RGB(0, 0, 0);
		//					crBkgnd = RGB(255, 255, 100);
		//				}
		//
		//			}
		//			else
		//			{
		//
		//				crText = RGB(0, 0, 0);
		//				crBkgnd = RGB(255, 255, 255);
		//			}
		//		}
		//
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;


		*pResult = CDRF_DODEFAULT;
		return;
	}
	}
	*pResult = 0;
}


void CSelectChassisModelDlg::OnNMCustomdrawListModel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	//int nRow=0;
	//int nCol=0;
	CString strCol = _T("");



	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}
	case CDDS_ITEMPREPAINT:
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		return;
	}
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
	{
		COLORREF crText, crBkgnd;



		int l_nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
		int l_nSubItem = (int)(pLVCD->iSubItem);


		if (l_nItem == m_nModel_SelectRow)
		{
			crText = RGB(0, 0, 0); ////m_cListChassisEx.nSelForeColor[l_nItem][l_nSubItem];// RGB(0, 0, 0); //글자색
			crBkgnd = RGB(255, 0, 255); ////m_cListChassisEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
		}
		else
		{
			crText = RGB(0, 0, 0); ////m_cListChassisEx.nSelForeColor[l_nItem][l_nSubItem];// RGB(0, 0, 0); //글자색
			crBkgnd = RGB(255, 255, 255); ////m_cListChassisEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
		}


		//if ((l_nItem >= 0) && (l_nItem < 500) && (l_nSubItem >= 0) && (l_nSubItem < 20))
		//{
		//	crText = m_cListModelEx.nSelForeColor[l_nItem][l_nSubItem];// RGB(0, 0, 0); //글자색
		//	crBkgnd = m_cListModelEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
		//}


		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;


		*pResult = CDRF_DODEFAULT;
		return;
	}
	}


	*pResult = 0;
}



LRESULT CSelectChassisModelDlg::GetModelSuffix(WPARAM wParam, LPARAM lParam)
{


	

	m_szModelSuffix = CurrentSet->sModelSuffixName;
	UpdateData(FALSE);

	OnBnClickedButtonSerarchModel();

	return 0;
}
