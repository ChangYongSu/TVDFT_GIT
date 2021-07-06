// CSelectGenerateModel.cpp: 구현 파일
//
#include "stdafx.h"
#include "datsys.h"
//#include "pch.h"
#include "DATsys.h"
#include "CSelectGenerateModel.h"
//#include "afxdialogex.h"
#include "Global.h"
#include "CSpreadSheet.h"
#include "AddModelDlg.h"

//#include "XLEzAutomation.h"

//#include "CExcel.h"

//void Read_DFTRegistry();
//CString EnumerateValues(HKEY hKey, CString cstrKey, CString cstrFullPath, CString cstrValueName);
//void FormatDataWithDataType(DWORD dwKeyType, LPBYTE pbbinKeyData, DWORD dwKeyDataLength, CString &cstrOutput);


CTypedPtrList<CPtrList, CModelData*>	ModelList;
CTypedPtrList<CPtrList, CModelData*>	ModelList_temp;

// CSelectGenerateModel 대화 상자

IMPLEMENT_DYNAMIC(CSelectGenerateModel, CDialog)

CSelectGenerateModel::CSelectGenerateModel(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SELECT_GENERATE_MODEL_DLG, pParent)
	, m_szServerPath(_T(""))
	, m_szRefImagePath(_T(""))
	//, m_szModelInfoPath(_T(""))
	, m_szModelListPath(_T(""))
{

}

CSelectGenerateModel::~CSelectGenerateModel()
{

	//XL.~CXLEzAutomation();
}

void CSelectGenerateModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SEQ_PATH, m_szServerPath);
	DDX_Text(pDX, IDC_EDIT_REF_PATH, m_szRefImagePath);
	//DDX_Text(pDX, IDC_EDIT_INFO_PATH, m_szModelInfoPath);
	DDX_Text(pDX, IDC_EDIT_MODEL_LIST_PATH, m_szModelListPath);

	DDX_Text(pDX, IDC_EDIT_MODEL_INFO, m_szModelInfoFilePath);
	DDX_Text(pDX, IDC_EDIT_REF_FOLDER, m_szRefImgFolder);
	DDX_Text(pDX, IDC_EDIT_SEQ, m_szSeqFilePath);
	DDX_Text(pDX, IDC_EDIT_DIVISION, m_szDivision);

	DDX_Control(pDX, IDC_LIST_CHASSIS, m_cListChassis);
	DDX_Control(pDX, IDC_LIST_MODEL, m_cListModel);
}


BEGIN_MESSAGE_MAP(CSelectGenerateModel, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CSelectGenerateModel::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CSelectGenerateModel::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_COPY, &CSelectGenerateModel::OnBnClickedBtnCopy)
	ON_BN_CLICKED(IDC_BTN_DEL, &CSelectGenerateModel::OnBnClickedBtnDel)

	ON_BN_CLICKED(IDOK, &CSelectGenerateModel::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CHANGE_MODEL_PATH, &CSelectGenerateModel::OnBnClickedBtnChangeModelPath)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHASSIS, &CSelectGenerateModel::OnNMClickListChassis)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODEL, &CSelectGenerateModel::OnNMClickListModel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_CHASSIS, &CSelectGenerateModel::OnNMCustomdrawListChassis)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MODEL, &CSelectGenerateModel::OnNMCustomdrawListModel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CSelectGenerateModel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSelectGenerateModel 메시지 처리기

BOOL CSelectGenerateModel::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWinApp* pApp = AfxGetApp();
	// Add "About..." menu item to system menu.

	//// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	CString strAboutMenu;
	//	strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	//// Set the icon for this dialog.  The framework does this automatically
	////  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

//	Read_DFTRegistry();

	//	SS.Creat(sModelListFile, "ModelList"); 
	m_szServerPath = CurrentSet->sServerFolder;
	m_szRefImagePath = CurrentSet->sRefRootFolder;
	m_szModelListPath = CurrentSet->sModelListPath;

	m_ModelListPathOld = CurrentSet->sModelListPath;
	bool brtn = CopyFile( CurrentSet->sModelListPath,"D:\\DFT\\Temp.xls", 0);
	DWORD dw;
	if (brtn == 0)
	{
		 dw = GetLastError();
	}

	//m_szPatternFilePath = CurrentSet->sPatternTitle;
	//m_szRemoteFilePath = CurrentSet->sRemoconTitle;
	//m_szTemplateSeqPath = CurrentSet->sFullSeqPath;
	//m_szTestParamPath = CurrentSet->sTestParamIni;
	
	//m_strServerFolder = CurrentSet->sServerFolder;
	//m_strRefImageFolder = CurrentSet->sRefRootFolder;
	//m_strModelInfoFolder = CurrentSet->sModelInfoFolder;


	ModelListOpen();
	InitGrid();

	m_nChaassis_SelectRow = 0;
	m_nModel_SelectRow = 0;



	UpdateGrid();

	UpdateData(FALSE);

	// TODO: Add extra initialization here
	//SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}




void CSelectGenerateModel::InitGrid()
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


	//m_gridChassis.SetRedraw(FALSE);
	//m_gridChassis.SetAllowBigSelection(FALSE);
	//m_gridChassis.SetExtendLastCol(TRUE);
	//m_gridChassis.SetCols(1);
	//m_gridChassis.SetRows(1);
	//m_gridChassis.SetFixedRows(1);
	//m_gridChassis.SetFixedCols(0);
	//m_gridChassis.SetGridLines(flexGridFlat);
	//m_gridChassis.SetTextMatrix(0, 0, "CHASSIS LIST");
	//m_gridChassis.SetRedraw(TRUE);
	//m_gridChassis.Refresh();

	//m_gridChassis.SetBackColorFixed(RGB(210, 216, 176));
	//m_gridChassis.SetForeColorFixed(RGB(51, 102, 153));
	//m_gridChassis.SetBackColor(RGB(247, 247, 231));
	//m_gridChassis.SetGridColor(RGB(210, 216, 176));
	//m_gridChassis.SetColAlignment(0, flexAlignCenterCenter);


	//m_gridModel.SetRedraw(FALSE);
	//m_gridModel.SetAllowBigSelection(FALSE);
	//m_gridModel.SetExtendLastCol(TRUE);
	//m_gridModel.SetCols(1);
	//m_gridModel.SetRows(1);
	//m_gridModel.SetFixedRows(1);
	//m_gridModel.SetFixedCols(0);
	//m_gridModel.SetGridLines(flexGridFlat);
	//m_gridModel.SetTextMatrix(0, 0, "MODEL LIST");
	//m_gridModel.SetRedraw(TRUE);
	//m_gridModel.Refresh();

	//m_gridModel.SetBackColorFixed(RGB(210, 216, 176));
	//m_gridModel.SetForeColorFixed(RGB(51, 102, 153));
	//m_gridModel.SetBackColor(RGB(247, 247, 231));
	//m_gridModel.SetGridColor(RGB(210, 216, 176));
	//m_gridModel.SetColAlignment(0, flexAlignCenterCenter);

#else
	//m_gridChassis.SetRedraw(FALSE);
	//m_gridChassis.SetAllowBigSelection(FALSE);
	//m_gridChassis.SetExtendLastCol(TRUE);
	//m_gridChassis.SetCols(1);
	//m_gridChassis.SetRows(1);
	//m_gridChassis.SetFixedRows(1);
	//m_gridChassis.SetFixedCols(0);
	//m_gridChassis.SetGridLines(flexGridFlat);
	//m_gridChassis.SetTextMatrix(0, 0, "CHASSIS LIST");
	//m_gridChassis.SetRedraw(TRUE);
	//m_gridChassis.Refresh();

	//m_gridChassis.SetBackColorFixed(RGB(210, 216, 176));
	//m_gridChassis.SetForeColorFixed(RGB(51, 102, 153));
	//m_gridChassis.SetBackColor(RGB(247, 247, 231));
	//m_gridChassis.SetGridColor(RGB(210, 216, 176));
	//m_gridChassis.SetColAlignment(0, flexAlignCenterCenter);


	//m_gridModel.SetRedraw(FALSE);
	//m_gridModel.SetAllowBigSelection(FALSE);
	//m_gridModel.SetExtendLastCol(TRUE);
	//m_gridModel.SetCols(1);
	//m_gridModel.SetRows(1);
	//m_gridModel.SetFixedRows(1);
	//m_gridModel.SetFixedCols(0);
	//m_gridModel.SetGridLines(flexGridFlat);
	//m_gridModel.SetTextMatrix(0, 0, "MODEL LIST");
	//m_gridModel.SetRedraw(TRUE);
	//m_gridModel.Refresh();

	//m_gridModel.SetBackColorFixed(RGB(210, 216, 176));
	//m_gridModel.SetForeColorFixed(RGB(51, 102, 153));
	//m_gridModel.SetBackColor(RGB(247, 247, 231));
	//m_gridModel.SetGridColor(RGB(210, 216, 176));
	//m_gridModel.SetColAlignment(0, flexAlignCenterCenter);

#endif
}

void CSelectGenerateModel::UpdateGrid()
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

	Pos = ModelList.GetHeadPosition();

	while (Pos)
	{
		pModelData = ModelList.GetNext(Pos);

		szChassisName = pModelData->m_szChassisName;

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
		m_cListChassis.SetSelectionMark(m_nChaassis_SelectRow + 1);// GetFirstSelectedItemPosition(); //m_gridChassis.SetRow(m_nChaassis_SelectRow);
		OnClickChassisGrid();
	}
//	SetTimer(1, 300, NULL);


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
BOOL CSelectGenerateModel::IsSameChassis(CString szChassisName)
{
	CString	szRegisteredChassisName = _T("");
	//	POSITION lpos = m_cListChassis.GetFirstSelectedItemPosition();
	//	int	nNoChassis = (int)lpos;   //m_gridChassis.GetRows() - 1;
	int	nNoChassis = m_cListChassis.GetItemCount();
	szChassisName.MakeUpper();

	for (int i = 0; i < nNoChassis; i++)
	{
		szRegisteredChassisName = m_cListChassis.GetItemText(i,0); // .GetTextMatrix(i+1, 0);

		szRegisteredChassisName.MakeUpper();

		if (szChassisName.Compare(szRegisteredChassisName) == NULL)
		{
			return TRUE;
		}
	}
	/*CString	szRegisteredChassisName = _T("");

	int	nNoChassis = m_gridChassis.GetRows() - 1;

	szChassisName.MakeUpper();

	for (int i = 0; i < nNoChassis; i++)
	{
		szRegisteredChassisName = m_gridChassis.GetTextMatrix(i + 1, 0);

		szRegisteredChassisName.MakeUpper();

		if (szChassisName.Compare(szRegisteredChassisName) == NULL)
		{
			return TRUE;
		}
	}*/
	return FALSE;
}


void CSelectGenerateModel::OnBnClickedBtnAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnBtnAdd();

	
}


void CSelectGenerateModel::OnBnClickedBtnEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBtnEdit();
}


void CSelectGenerateModel::OnBnClickedBtnCopy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBtnCopy();
	

	
}


void CSelectGenerateModel::OnBnClickedBtnDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBtnDel();
}



void CSelectGenerateModel::OnClickChassisGrid()
{

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

	Pos = ModelList.GetHeadPosition();
	m_cListModel.DeleteAllItems();
	while (Pos)
	{
		pModelData = ModelList.GetNext(Pos);

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

}

void CSelectGenerateModel::OnClickModelGrid()
{

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
		m_cListModel.SetSelectionMark(m_cListModel.GetItemCount()-1);//m_gridModel.SetRow(m_gridModel.GetRows() - 1); 
		nModelRow = m_cListModel.GetItemCount() - 1;
	}

	szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);// m_gridChassis.GetTextMatrix(nChassisRow, 0);
	szModelName = m_cListModel.GetItemText(nModelRow, 0);//m_gridModel.GetTextMatrix(nModelRow,0);
	m_sSelModel = szModelName;

	Pos = ModelList.GetHeadPosition();

	while (Pos)
	{
		pModelData = ModelList.GetNext(Pos);

		if (pModelData->m_szModelName != _T(""))
		{
			if ((pModelData->m_szChassisName.Compare(szChassisName) == NULL) &&
				(pModelData->m_szModelName.Compare(szModelName) == NULL))
			{
				m_szSeqFilePath = pModelData->m_szSeqFilePath;
				m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
				m_szRefImgFolder = pModelData->m_szRefImgFolder;
				m_szDivision = pModelData->m_szDivision;

				break;
			}
		}
	}

	UpdateData(FALSE);
	m_nModel_SelectRow = nModelRow;
	ChangeRowColor(nModelRow, MODEL_GRID);

	m_cListModel.Invalidate();
	//POSITION	Pos = NULL;
	//CModelData* pModelData = NULL;
	//int			nModelRow = m_gridModel.GetRow();
	//int			nChassisRow = m_gridChassis.GetRow();

	//CString szChassisName = _T("");
	//CString szModelName = _T("");

	//if ((nModelRow <= 0) && (m_gridModel.GetRows() > 1))
	//{
	//	m_gridModel.SetRow(1);
	//	nModelRow = 1;
	//}
	//else if ((nModelRow >= m_gridModel.GetRows()) && (m_gridModel.GetRows() > 1))
	//{
	//	m_gridModel.SetRow(m_gridModel.GetRows() - 1);
	//	nModelRow = m_gridModel.GetRows() - 1;
	//}

	//szChassisName = m_gridChassis.GetTextMatrix(nChassisRow, 0);
	//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);
	//m_sSelModel = szModelName;

	//Pos = ModelList.GetHeadPosition();

	//while (Pos)
	//{
	//	pModelData = ModelList.GetNext(Pos);

	//	if (pModelData->m_szModelName != _T(""))
	//	{
	//		if ((pModelData->m_szChassisName.Compare(szChassisName) == NULL) &&
	//			(pModelData->m_szModelName.Compare(szModelName) == NULL))
	//		{
	//			m_szSeqFilePath = pModelData->m_szSeqFilePath;
	//			m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
	//			m_szRefImgFolder = pModelData->m_szRefImgFolder;
	//			m_szDivision = pModelData->m_szDivision;

	//			break;
	//		}
	//	}
	//}

	//UpdateData(FALSE);
	//m_nModel_SelectRow = nModelRow;
	//ChangeRowColor(nModelRow, MODEL_GRID);
}
void CSelectGenerateModel::ChangeRowColor(int nRow, int nGridType)
{
	////===============
	//// Chassis Grid
	////===============
	//if (nGridType == CHASSIS_GRID)
	//{
	//	for (int nTotalRow = 1; nTotalRow < m_gridChassis.GetRows(); nTotalRow++)
	//	{
	//		m_gridChassis.SetRow(nTotalRow);
	//		m_gridChassis.SetCol(0);
	//		m_gridChassis.SetCellBackColor(RGB(255, 255, 255));
	//		m_gridChassis.SetCellForeColor(RGB(0, 0, 0));
	//	}

	//	m_gridChassis.SetRow(nRow);
	//	m_gridChassis.SetCol(0);

	//	//		m_gridChassis.SetCellBackColor(RGB(10, 36, 106 ));
	//	m_gridChassis.SetCellBackColor(RGB(255, 0, 255));
	//	//		m_gridChassis.SetCellForeColor(RGB(255,255,255));
	//}
	////===============
	//// Chassis Grid
	////===============
	//else
	//{
	//	for (int nTotalRow = 1; nTotalRow < m_gridModel.GetRows(); nTotalRow++)
	//	{
	//		m_gridModel.SetRow(nTotalRow);
	//		m_gridModel.SetCol(0);
	//		m_gridModel.SetCellBackColor(RGB(255, 255, 255));
	//		m_gridModel.SetCellForeColor(RGB(0, 0, 0));
	//	}

	//	m_gridModel.SetRow(nRow);
	//	m_gridModel.SetCol(0);

	//	//		m_gridModel.SetCellBackColor(RGB(10, 36,   106 ));
	//	m_gridModel.SetCellBackColor(RGB(255, 0, 255));
	//	//		m_gridModel.SetCellForeColor(RGB(255,255,255));
	//}
}

BOOL CSelectGenerateModel::ModelListOpen()
{

#if 1
	//int lnModel = 0;
	//CStringArray lnDevsionModel;

	//CSelectGenerateModel_ModelListOpen(CurrentSet->sModelListPath, lnModel, lnDevsionModel);

	//
	//m_nChaassis_SelectRow = 0;

	//m_nModel_SelectRow = 0;// //m_gridModel.GetRow();

	CStringArray Rows;
	int i;
	CString lFileName;
	lFileName = CurrentSet->sModelListPath;// +"ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
	ModelList.RemoveAll();

	for (i = 2; i <= SS.GetTotalRows(); i++)
	{
		// Read row
		SS.ReadRow(Rows, i);

		if (Rows.GetAt(0).GetLength() > 2)
		{
			CModelData* pModel = new CModelData;

			pModel->m_szDivision = Rows.GetAt(0);
			pModel->m_szChassisName = Rows.GetAt(1);
			pModel->m_szModelName = Rows.GetAt(2);
			pModel->m_szSeqFilePath = Rows.GetAt(3);
			pModel->m_szModelInfoFilePath = Rows.GetAt(4);
			pModel->m_szRefImgFolder = Rows.GetAt(5);

			ModelList.AddTail(pModel);
		}
		else
		{
			continue;
		}

	}

#else


	CStringArray Rows;
	int i;
	CString lFileName;
	lFileName = sModelInfoFolder + "ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
	ModelList.RemoveAll();

	for (i = 2; i <= SS.GetTotalRows(); i++)
	{
		// Read row
		SS.ReadRow(Rows, i);

		if (Rows.GetAt(0).GetLength() > 2)
		{
			CModelData* pModel = new CModelData;

			pModel->m_szDivision = Rows.GetAt(0);
			pModel->m_szChassisName = Rows.GetAt(1);
			pModel->m_szModelName = Rows.GetAt(2);
			pModel->m_szSeqFilePath = Rows.GetAt(3);
			pModel->m_szModelInfoFilePath = Rows.GetAt(4);
			pModel->m_szRefImgFolder = Rows.GetAt(5);

			ModelList.AddTail(pModel);
		}
		else
		{
			continue;
		}

	}
#endif
	return TRUE;
}


BOOL CSelectGenerateModel::CheckModel(CString sChassis, CString sModel)
{
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	/*int			nChassisRow = m_gridChassis.GetRow();
	int			nModelRow = m_gridModel.GetRow();*/

	int			nChassisRow = m_nChaassis_SelectRow;
	int			nModelRow = m_nModel_SelectRow;
		
	CString szChassisName = _T("");
	CString szModelName = _T("");

	


	if ((nChassisRow < 0) || (nChassisRow >= m_cListChassis.GetItemCount())) {
		szChassisName = _T("");
	}
	else {
		szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);

	}

	if ((nModelRow < 0) || (nModelRow >= m_cListModel.GetItemCount())) {
		szModelName = _T("");
	}
	else {
		szModelName = m_cListModel.GetItemText(nModelRow, 0);
	}

	Pos = ModelList.GetHeadPosition();
	while (Pos)
	{
		pModelData = ModelList.GetNext(Pos);

		//		if(pModelData == NULL) continue;
		if ((pModelData->m_szChassisName != _T("")) && (pModelData->m_szModelName != _T("")))
		{
			if ((sChassis.Compare(pModelData->m_szChassisName) == NULL)
				&& (sModel.Compare(pModelData->m_szModelName) == NULL))
			{
				//				delete pModelData;
				return FALSE;
			}
		}
	}
	if ((szChassisName != _T("")) && (szModelName != _T("")))
	{
		if ((sChassis.Compare(szChassisName) == NULL) && (sModel.Compare(szModelName) == NULL))
		{
			return FALSE;
		}
	}

	return TRUE;
}


void CSelectGenerateModel::OnBtnAdd()
{
#if 1
	CStringArray Rows;
	CString lFileName;
	lFileName = CurrentSet->sModelListPath; // +"ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");// sModelInfoFolder

	//int			nModelRow = m_gridModel.GetRow();
	int nDivision;

	CAddModelDlg dlg;
	CTime Time = CTime::GetCurrentTime();

	nDivision = Time.GetYear();
	GetWindowRect(&dlg.m_ParentWndRect);

	if (nDivision >= 2007) nDivision = nDivision - 2007;
	dlg.m_nDivision = nDivision;

	if (dlg.DoModal() == IDOK)
	{
		CModelData* pModel = new CModelData;

		pModel->m_szChassisName = dlg.m_szChassisName;
		pModel->m_szModelName = dlg.m_szModelName;
		pModel->m_szSeqFilePath = dlg.m_szSeqPath;
		pModel->m_szModelInfoFilePath = dlg.m_szModelInfoPath;
		//pModel->m_szVFConfigPath = dlg.m_szVFConfigPath;
	//	pModel-> = dlg.m_szVFConfigPath;
		pModel->m_szRefImgFolder = dlg.m_szRefFolderPath;
		pModel->m_szDivision = dlg.m_sDivision;

		Rows.Add(pModel->m_szDivision);
		Rows.Add(pModel->m_szChassisName);
		Rows.Add(pModel->m_szModelName);
		Rows.Add(pModel->m_szSeqFilePath);
		Rows.Add(pModel->m_szModelInfoFilePath);
		Rows.Add(pModel->m_szRefImgFolder);

		if (CheckModel(pModel->m_szChassisName, pModel->m_szModelName))
		{
			ModelList.AddTail(pModel);
			UpdateGrid();
			int lnRows = SS.GetTotalRows();
			if (lnRows == 0)
			{
				CStringArray sampleArray;

				sampleArray.Add("Division");
				sampleArray.Add("Chassis_Name");
				sampleArray.Add("Model_Name");
				sampleArray.Add("Sequence");
				sampleArray.Add("ModelInfo");
				sampleArray.Add("RefImgFolder");

				SS.AddHeaders(sampleArray, true);
			}

			SS.AddRow(Rows); // append test row to spreadsheet
			//SaveModelListFile(); //CSelectGenerateModel_SaveModelListFile(CurrentSet->sModelListPath);
		}
		else {
			CString sTmp = _T("Error : ChassisName & ModelName");
			AfxMessageBox(sTmp);
		}
	}
#endif
	//CStringArray Rows;
	//CString lFileName;
	//lFileName = sModelInfoFolder + "ModelList.xls";
	//CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder

	//int			nModelRow = m_gridModel.GetRow();
	//int nDivision;

	//CAddModelDlg dlg;
	//CTime Time = CTime::GetCurrentTime();

	//nDivision = Time.GetYear();
	//GetWindowRect(&dlg.m_ParentWndRect);

	//if (nDivision >= 2007) nDivision = nDivision - 2007;
	//dlg.m_nDivision = nDivision;

	//if (dlg.DoModal() == IDOK)
	//{
	//	CModelData* pModel = new CModelData;

	//	pModel->m_szChassisName = dlg.m_szChassisName;
	//	pModel->m_szModelName = dlg.m_szModelName;
	//	pModel->m_szSeqFilePath = dlg.m_szSeqPath;
	//	pModel->m_szModelInfoFilePath = dlg.m_szModelInfoPath;
	//	pModel->m_szVFConfigPath = dlg.m_szVFConfigPath;
	//	pModel->m_szRefImgFolder = dlg.m_szRefFolderPath;
	//	pModel->m_szDivision = dlg.m_sDivision;

	//	Rows.Add(pModel->m_szDivision);
	//	Rows.Add(pModel->m_szChassisName);
	//	Rows.Add(pModel->m_szModelName);
	//	Rows.Add(pModel->m_szSeqFilePath);
	//	Rows.Add(pModel->m_szModelInfoFilePath);
	//	Rows.Add(pModel->m_szRefImgFolder);

	//	if (CheckModel(pModel->m_szChassisName, pModel->m_szModelName))
	//	{
	//		ModelList.AddTail(pModel);
	//		UpdateGrid();

	//		if (SS.GetTotalRows() == 0)
	//		{
	//			CStringArray sampleArray;

	//			sampleArray.Add("Division");
	//			sampleArray.Add("Chassis_Name");
	//			sampleArray.Add("Model_Name");
	//			sampleArray.Add("Sequence");
	//			sampleArray.Add("ModelInfo");
	//			sampleArray.Add("RefImgFolder");

	//			SS.AddHeaders(sampleArray, true);
	//		}

	//		SS.AddRow(Rows); // append test row to spreadsheet

	//	}
	//	else {
	//		CString sTmp = _T("Error : ChassisName & ModelName");
	//		AfxMessageBox(sTmp);
	//	}
	//}
}

void CSelectGenerateModel::OnBtnEdit()
{
	POSITION	Pos = NULL;
	POSITION	CurrentPos = NULL;
	CModelData* pModelData = NULL;
	//int			nChassisRow = (int) m_cListChassis.GetFirstSelectedItemPosition()-1; //m_gridChassis.GetRow();
	//int			nModelRow = (int)m_cListModel.GetFirstSelectedItemPosition()-1;//m_gridModel.GetRow();

	int			nChassisRow = m_nChaassis_SelectRow;
	int			nModelRow = m_nModel_SelectRow;//

		//CListCtrl m_cListChassis;
	//CListCtrl m_cListModel;

	CString szChassisName = _T("");
	CString szModelName = _T("");
	CStringArray NewRowValues;
	CStringArray Rows;
	int nDivision;

	CString lFileName;
	lFileName = CurrentSet->sModelListPath; // +"ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");// sModelInfoFolder

	if ((nChassisRow < 0) || (nChassisRow >= m_cListChassis.GetItemCount())) return;

	szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);


	if ((nModelRow < 0) || (nModelRow >= m_cListModel.GetItemCount())) return;

	szModelName = m_cListModel.GetItemText(nModelRow, 0);

	Pos = ModelList.GetHeadPosition();
	while (Pos)
	{
		CurrentPos = Pos;
		pModelData = ModelList.GetNext(Pos);

		if ((szChassisName.Compare(pModelData->m_szChassisName) == NULL)
			&& (szModelName.Compare(pModelData->m_szModelName) == NULL))
		{
			//			delete pModelData;
			break;
		}
	}


	CAddModelDlg lDlg;

	GetWindowRect(&lDlg.m_ParentWndRect);

	//pDlg->m_sWinText = _T("Edit Model");
	lDlg.m_szChassisName = szChassisName;
	lDlg.m_szModelName = szModelName;
	lDlg.m_szSeqPath = m_szSeqFilePath;
	lDlg.m_szModelInfoPath = m_szModelInfoFilePath;
	lDlg.m_szRefFolderPath = m_szRefImgFolder;
	nDivision = atoi(m_szDivision);
	if (nDivision >= 2007) nDivision = nDivision - 2007;
	lDlg.m_nDivision = nDivision;
	lDlg.m_sDivision = m_szDivision;
	lDlg.m_EditMode = 1;

	if (lDlg.DoModal() == IDOK)
	{
		CModelData* pModel = new CModelData;

		pModel->m_szChassisName = lDlg.m_szChassisName;
		pModel->m_szModelName = lDlg.m_szModelName;
		pModel->m_szSeqFilePath = lDlg.m_szSeqPath;
		pModel->m_szModelInfoFilePath = lDlg.m_szModelInfoPath;
		pModel->m_szRefImgFolder = lDlg.m_szRefFolderPath;
		pModel->m_szDivision = lDlg.m_sDivision;



		ModelList.SetAt(CurrentPos, pModel);
		UpdateGrid();

		NewRowValues.Add(pModel->m_szDivision);
		NewRowValues.Add(pModel->m_szChassisName);
		NewRowValues.Add(pModel->m_szModelName);
		NewRowValues.Add(pModel->m_szSeqFilePath);
		NewRowValues.Add(pModel->m_szModelInfoFilePath);
		NewRowValues.Add(pModel->m_szRefImgFolder);

		//for (int i = 2; i <= SS.GetTotalRows(); i++)
		//{
		//	// Read row
		//	SS.ReadRow(Rows, i);

		//	if (Rows.GetAt(0).GetLength() > 2)
		//	{
		//		

		//		if((m_szDivision == Rows.GetAt(0))
		//		&&(szChassisName == Rows.GetAt(1))
		//			&& (szModelName == Rows.GetAt(2)))
		//		{
		//			//pModel->m_szSeqFilePath = Rows.GetAt(3);
		//			//pModel->m_szModelInfoFilePath = Rows.GetAt(4);
		//			//pModel->m_szRefImgFolder = Rows.GetAt(5);

		//			SS.ReplaceRows2(NewRowValues); //ModelList.AddTail(pModel);
		//			break;
		//		}
		//	}
		//
		//}

		SS.ReplaceRows2(NewRowValues);
		//SaveModelListFile();;//CSelectGenerateModel_SaveModelListFile(CurrentSet->sModelListPath);
			//	delete pModel;
	}
//	delete pDlg;
}

void CSelectGenerateModel::OnBtnCopy()
{
	POSITION	Pos = NULL;
	POSITION	CurrentPos = NULL;
	CModelData* pModelData = NULL;
	//int			nChassisRow = m_gridChassis.GetRow();
	//int			nModelRow = m_gridModel.GetRow();
	//int			nChassisRow = (int)m_cListChassis.GetFirstSelectedItemPosition(); //m_gridChassis.GetRow();
	//int			nModelRow = (int)m_cListModel.GetFirstSelectedItemPosition();//m_gridModel.GetRow();

	CString lFileName;
	lFileName = CurrentSet->sModelListPath; // +"ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");// sModelInfoFolder


	int			nChassisRow = m_nChaassis_SelectRow;
	int			nModelRow = m_nModel_SelectRow;//


	CString szChassisName = _T("");
	CString szModelName = _T("");
	CStringArray NewRowValues;
	int nDivision;

	//CString lFileName ;
	//lFileName = sModelInfoFolder + "ModelList.xls";
	//CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder

	//if ((nChassisRow <= 0) || (nChassisRow >= m_gridChassis.GetRows())) return;
	//szChassisName = m_gridChassis.GetTextMatrix(nChassisRow, 0);
	//if ((nModelRow <= 0) || (nModelRow >= m_gridModel.GetRows())) return;
	//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);

	if ((nChassisRow < 0) || (nChassisRow >= m_cListChassis.GetItemCount())) return;
	szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);
	
	if ((nModelRow < 0) || (nModelRow >= m_cListModel.GetItemCount())) return;
	szModelName = m_cListModel.GetItemText(nModelRow, 0);


	CAddModelDlg* pDlg = new CAddModelDlg;

	GetWindowRect(&pDlg->m_ParentWndRect);

	pDlg->m_sWinText = _T("Edit Model(Copy)");
	pDlg->m_szChassisName = szChassisName;
	pDlg->m_szModelName = szModelName;
	pDlg->m_szSeqPath = m_szSeqFilePath;
	pDlg->m_szModelInfoPath = m_szModelInfoFilePath;
	pDlg->m_szRefFolderPath = m_szRefImgFolder;
	nDivision = atoi(m_szDivision);
	if (nDivision >= 2007) nDivision = nDivision - 2007;
	pDlg->m_nDivision = nDivision;

	if (pDlg->DoModal() == IDOK)
	{
		CModelData* pModel = new CModelData;

		pModel->m_szChassisName = pDlg->m_szChassisName;
		pModel->m_szModelName = pDlg->m_szModelName;
		pModel->m_szSeqFilePath = pDlg->m_szSeqPath;
		pModel->m_szModelInfoFilePath = pDlg->m_szModelInfoPath;
		pModel->m_szRefImgFolder = pDlg->m_szRefFolderPath;
		pModel->m_szDivision = pDlg->m_sDivision;


		if (CheckModel(pModel->m_szChassisName, pModel->m_szModelName))
		{
			ModelList.AddTail(pModel);

			UpdateGrid();

			NewRowValues.Add(pModel->m_szDivision);
			NewRowValues.Add(pModel->m_szChassisName);
			NewRowValues.Add(pModel->m_szModelName);
			NewRowValues.Add(pModel->m_szSeqFilePath);
			NewRowValues.Add(pModel->m_szModelInfoFilePath);
			NewRowValues.Add(pModel->m_szRefImgFolder);

			SS.AddRow(NewRowValues);
			//SaveModelListFile();
		}
		else {
			CString sTmp = _T("Error : ChassisName & ModelName");
			AfxMessageBox(sTmp);
		}
		//	delete pModel;
	}
	delete pDlg;
}


BOOL CSelectGenerateModel::SaveModelListFile()
{
	return 0;
	//CStringArray FieldNames, Rows;
	//POSITION	Pos = NULL;
	//CModelData* pModelData = NULL;
	//CString lFileName;
	//lFileName = CurrentSet->sModelListPath; // sModelInfoFolder + "ModelList.xls";
	//CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder

	//SS.GetFieldNames(FieldNames);
	//SS.DeleteSheet();

	//SS.BeginTransaction();
	//SS.RollBack();

	//SS.AddHeaders(FieldNames);

	//Pos = ModelList.GetHeadPosition();
	//while (Pos)
	//{
	//	Rows.RemoveAll();
	//	pModelData = ModelList.GetNext(Pos);

	//	Rows.Add(pModelData->m_szChassisName);
	//	Rows.Add(pModelData->m_szModelName);
	//	Rows.Add(pModelData->m_szSeqFilePath);
	//	Rows.Add(pModelData->m_szModelInfoFilePath);
	//	Rows.Add(pModelData->m_szRefImgFolder);

	//	SS.AddRow(Rows); // append test row to spreadsheet
	//}

	//SS.Commit();

	//CString sTemp;
	//CString sSeqFolder;
	//CIniAccess		m_Ini;				// IniAccess Class
	//CString sSection;
	//int nNoModel;
	//POSITION	Pos = NULL;
	//CStringArray Rows;

	////CFileDialog dlg(TRUE, "Model List", NULL,
	////	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY,
	////	"ModelList Text File(*.ini)|*.ini|", NULL);

	////sSeqFolder = "D:\\DFT";
	////dlg.m_ofn.lpstrInitialDir = sSeqFolder;

	////if (dlg.DoModal() != IDOK)
	////{
	////	return;
	////}
	//sTemp = dlg.GetPathName();
	//m_Ini.SetIniFile(sTemp);

	//sSection = "GENERAL_INFO";
	//nNoModel = m_Ini.GetProfileInt(sSection, "No_Model");

	//ModelList_temp.RemoveAll();
	//if (SS.GetTotalRows() == 0)
	//{
	//	CStringArray sampleArray;

	//	sampleArray.Add("Division");
	//	sampleArray.Add("Chassis_Name");
	//	sampleArray.Add("Model_Name");
	//	sampleArray.Add("Sequence");
	//	sampleArray.Add("ModelInfo");
	//	sampleArray.Add("RefImgFolder");

	//	SS.AddHeaders(sampleArray, true);
	//}

	//Pos = ModelList_temp.GetHeadPosition();

	//for (int i = 0; i < nNoModel; i++)
	//{
	//	CModelData* pModel = new CModelData;

	//	Rows.RemoveAll();

	//	sSection.Format("MODEL_%d", i + 1);

	//	pModel->m_szChassisName = m_Ini.GetProfileString(sSection, "Chassis_Name");
	//	pModel->m_szModelName = m_Ini.GetProfileString(sSection, "Model_Name");
	//	pModel->m_szSeqFilePath = m_Ini.GetProfileString(sSection, "Sequence");
	//	pModel->m_szModelInfoFilePath = m_Ini.GetProfileString(sSection, "ModelInfo");
	//	pModel->m_szRefImgFolder = m_Ini.GetProfileString(sSection, "RefImgFolder");
	//	pModel->m_szDivision = _T("2008");

	//	Rows.Add(pModel->m_szDivision);
	//	Rows.Add(pModel->m_szChassisName);
	//	Rows.Add(pModel->m_szModelName);
	//	Rows.Add(pModel->m_szSeqFilePath);
	//	Rows.Add(pModel->m_szModelInfoFilePath);
	//	Rows.Add(pModel->m_szRefImgFolder);

	//	if (CheckModel(pModel->m_szChassisName, pModel->m_szModelName))
	//	{
	//		ModelList.AddTail(pModel);
	//		SS.AddRow(Rows);
	//	}
	//	else {
	//		if (m_nSaveOption == 0) SS.ReplaceRows2(Rows);
	//	}
	//}
	//SS.RollBack();
	//ModelListOpen();
	//UpdateGrid();

#if 0

	CString sTempShell;
	CString sTempShellValue;

	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	int lModel_Id = 1;

	
	CXLEzAutomation XL(FALSE);
	
	CString TempFile = "D:\\DFT\\Temp.xls";	
	CString TempNewFile = "D:\\DFT\\TempNew.xls";
	DeleteFile(TempNewFile);
	CopyFile(CurrentSet->sModelListPath, TempFile, 1);

	//XL.OpenExcelFile(CurrentSet->sModelListPath);
	XL.OpenExcelFile(TempFile);
	
	
	for (int i = ModelList.GetCount()-1; i > 1 ; i--)
	{
		XL.DeleteRow(i);
	}

	//XL.SetCellValue(1, 0, _T();
	XL.SetCellValue(1, 1, _T("Division"));
	XL.SetCellValue(2, 1, _T("Chassis_Name"));
	XL.SetCellValue(3, 1, _T("Model_Name"));
	XL.SetCellValue(4, 1, _T("Sequence"));
	XL.SetCellValue(5, 1, _T("ModelInfo"));
	XL.SetCellValue(6, 1, _T("RefImgFolder"));
	XL.SetCellValue(7, 1, _T("Sequence2"));


	Pos = ModelList.GetHeadPosition();
	while (Pos)
	{
				
		pModelData = ModelList.GetNext(Pos);
		lModel_Id++;

		XL.SetCellValue(1, lModel_Id, pModelData->m_szDivision);
		XL.SetCellValue(2, lModel_Id, pModelData->m_szChassisName);
		XL.SetCellValue(3, lModel_Id, pModelData->m_szModelName);
		XL.SetCellValue(4, lModel_Id, pModelData->m_szSeqFilePath);
		XL.SetCellValue(5, lModel_Id, pModelData->m_szModelInfoFilePath);
		XL.SetCellValue(6, lModel_Id, pModelData->m_szRefImgFolder);		
	}

	XL.SaveFileAs(TempNewFile);

	int lResult = XL.ReleaseExcel();  // tempSS.GetDivisions(Divisions);
	 // 
	CopyFile(TempNewFile, CurrentSet->sModelListPath,  0);
	DeleteFile(TempNewFile);

	//CExcel excel;
	//CString sTempShell;
	//CString sTempShellValue;

	//POSITION	Pos = NULL;
	//CModelData* pModelData = NULL;
	//int lModel_Id = 1;
	//CString TempFile = "D:\\DFT\\Temp.xls";
	//CString TempNewFile = "D:\\DFT\\TempNew.xls";
	//DeleteFile(TempNewFile);
	//CopyFile(CurrentSet->sModelListPath, TempFile, 1);
	////CopyFile( TempFile, TempNewFile, 0 );


	////CopyFile(TempNewFile,CurrentSet->sModelListPath, 0 );

	//if (excel.InitExcel( TempFile, TempNewFile))
	//{
	//	if (excel.SelectSheet(1))
	//	{
	//		//Division	Chassis_Name	Model_Name	Sequence	ModelInfo	RefImgFolder	Sequence2


	//		excel.SetCellValue(COleVariant("A1"), COleVariant("Division"));
	//		excel.SetCellValue(COleVariant("B1"), COleVariant("Chassis_Name"));
	//		excel.SetCellValue(COleVariant("C1"), COleVariant("Model_Name"));
	//		excel.SetCellValue(COleVariant("D1"), COleVariant("Sequence"));
	//		excel.SetCellValue(COleVariant("E1"), COleVariant("ModelInfo"));
	//		excel.SetCellValue(COleVariant("F1"), COleVariant("RefImgFolder"));
	//		excel.SetCellValue(COleVariant("G1"), COleVariant("Sequence2"));


	//		Pos = ModelList.GetHeadPosition();
	//		while (Pos)
	//		{
	//			
	//			pModelData = ModelList.GetNext(Pos);
	//			lModel_Id++;
	//			sTempShell.Format(_T("A%d"), lModel_Id);
	//			excel.SetCellValue(COleVariant(sTempShell), COleVariant(pModelData->m_szDivision));

	//			sTempShell.Format(_T("B%d"), lModel_Id);
	//			excel.SetCellValue(COleVariant(sTempShell), COleVariant(pModelData->m_szChassisName));

	//			sTempShell.Format(_T("C%d"), lModel_Id);
	//			excel.SetCellValue(COleVariant(sTempShell), COleVariant(pModelData->m_szModelName));

	//			sTempShell.Format(_T("D%d"), lModel_Id);
	//			excel.SetCellValue(COleVariant(sTempShell), COleVariant(pModelData->m_szSeqFilePath));

	//			sTempShell.Format(_T("E%d"), lModel_Id);
	//			excel.SetCellValue(COleVariant(sTempShell), COleVariant(pModelData->m_szModelInfoFilePath));

	//			sTempShell.Format(_T("F%d"), lModel_Id);
	//			excel.SetCellValue(COleVariant(sTempShell), COleVariant(pModelData->m_szRefImgFolder));
	//							
	//			
	//		}
	//	}
	//	excel.CloseExcel(FALSE, TRUE);
	//}

	//CopyFile(TempNewFile, CurrentSet->sModelListPath, 0);
#else
//	CSelectGenerateModel_SaveModelListFile(CurrentSet->sModelListPath);
#endif



	CStringArray FieldNames, Rows, sampleArray;
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	CString lFileName;
	lFileName = CurrentSet->sModelListPath;// +"ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
	//CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
#if  1

	SS.GetFieldNames(FieldNames);
	SS.DeleteSheet();

	SS.BeginTransaction();
	SS.RollBack();
#endif //  0
//	FieldNames.Add
	
	SS.AddHeaders(FieldNames);

	Rows.RemoveAll();
	sampleArray.Add("Division");
	sampleArray.Add("Chassis_Name");
	sampleArray.Add("Model_Name");
	sampleArray.Add("Sequence");
	sampleArray.Add("ModelInfo");
	sampleArray.Add("RefImgFolder");

	SS.AddHeaders(sampleArray, true);


	Pos = ModelList.GetHeadPosition();
	while (Pos)
	{
		Rows.RemoveAll();
		pModelData = ModelList.GetNext(Pos);

		Rows.Add(pModelData->m_szDivision);
		Rows.Add(pModelData->m_szChassisName);
		Rows.Add(pModelData->m_szModelName);
		Rows.Add(pModelData->m_szSeqFilePath);
		Rows.Add(pModelData->m_szModelInfoFilePath);
		Rows.Add(pModelData->m_szRefImgFolder);

		SS.AddRow(Rows); // append test row to spreadsheet
	}
	SS.RollBack();
	SS.Commit();
//
	return TRUE;
}

void CSelectGenerateModel::OnBtnDel()
{


#if 0

	POSITION	Pos = NULL;
	POSITION	CurrentPos = NULL;
	CModelData* pModelData = NULL;
	//int			nChassisRow = m_gridChassis.GetRow();
	//int			nModelRow = m_gridModel.GetRow();
	int			nChassisRow = m_nChaassis_SelectRow;
	int			nModelRow = m_nModel_SelectRow;
	CString szChassisName = _T("");
	CString szModelName = _T("");
	CStringArray NewRowValues;
	int nDivision;

	//if ((nChassisRow <= 0) || (nChassisRow >= m_gridChassis.GetRows())) return;

	//szChassisName = m_gridChassis.GetTextMatrix(nChassisRow, 0);


	//if ((nModelRow <= 0) || (nModelRow >= m_gridModel.GetRows())) return;

	//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);
	if ((nChassisRow < 0) || (nChassisRow >= m_cListChassis.GetItemCount())) return;
	if ((nModelRow < 0) || (nModelRow >= m_cListModel.GetItemCount())) return;


	if ((nChassisRow < 0) || (nChassisRow >= m_cListChassis.GetItemCount())) return;
	szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);

	if ((nModelRow < 0) || (nModelRow >= m_cListModel.GetItemCount())) return;
	szModelName = m_cListModel.GetItemText(nModelRow, 0);

	CString lFileName;
	lFileName = CurrentSet->sModelListPath; // +"ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");// sModelInfoFolder


	Pos = ModelList.GetHeadPosition();
	while (Pos)
	{
		CurrentPos = Pos;
		pModelData = ModelList.GetNext(Pos);

		if ((szChassisName.Compare(pModelData->m_szChassisName) == NULL)
			&& (szModelName.Compare(pModelData->m_szModelName) == NULL))
		{
			//			delete pModelData;
			break;
		}
	}


	CAddModelDlg* pDlg = new CAddModelDlg;

	GetWindowRect(&pDlg->m_ParentWndRect);

	pDlg->m_sWinText = _T("Edit Model");
	pDlg->m_szChassisName = szChassisName;
	pDlg->m_szModelName = szModelName;
	pDlg->m_szSeqPath = m_szSeqFilePath;
	pDlg->m_szModelInfoPath = m_szModelInfoFilePath;
	pDlg->m_szRefFolderPath = m_szRefImgFolder;
	nDivision = atoi(m_szDivision);
	if (nDivision >= 2007) nDivision = nDivision - 2007;
	pDlg->m_nDivision = nDivision;

	if (pDlg->DoModal() == IDOK)
	{
		CModelData* pModel = new CModelData;

		pModel->m_szChassisName = pDlg->m_szChassisName;
		pModel->m_szModelName = pDlg->m_szModelName;
		pModel->m_szSeqFilePath = pDlg->m_szSeqPath;
		pModel->m_szModelInfoFilePath = pDlg->m_szModelInfoPath;
		pModel->m_szRefImgFolder = pDlg->m_szRefFolderPath;
		pModel->m_szDivision = pDlg->m_sDivision;



		ModelList.SetAt(CurrentPos, pModel);
		UpdateGrid();

		NewRowValues.Add(pModel->m_szDivision);
		NewRowValues.Add(pModel->m_szChassisName);
		NewRowValues.Add(pModel->m_szModelName);
		NewRowValues.Add(pModel->m_szSeqFilePath);
		NewRowValues.Add(pModel->m_szModelInfoFilePath);
		NewRowValues.Add(pModel->m_szRefImgFolder);

		SS.ReplaceRows2(NewRowValues);
		//	delete pModel;
}


#else
	//int	nChassisRow = m_gridChassis.GetRow();
	//int	nModelRow = m_gridModel.GetRow();

	int			nChassisRow = m_nChaassis_SelectRow;
	int			nModelRow = m_nModel_SelectRow;

	CStringArray Rows;
	
	if ((nChassisRow < 0) || (nChassisRow >= m_cListChassis.GetItemCount())) return;
	if ((nModelRow < 0) || (nModelRow >= m_cListModel.GetItemCount())) return;
	//if ((nChassisRow < 0) || (nChassisRow >= m_gridChassis.GetRows())) return;
	//if ((nModelRow < 0) || (nModelRow >= m_gridModel.GetRows())) return;

	POSITION	Pos = NULL;
	POSITION	CurrentPos = NULL;
	CModelData* pModelData = NULL;
	CString		szChassisName = _T("");
	CString		szModelName = _T("");

	CString lFileName;
	lFileName = CurrentSet->sModelListPath; // +"ModelList.xls";
	CSpreadSheet SS(lFileName, "ModelList");// sModelInfoFolder


	if ((nChassisRow < 0) || (nChassisRow >= m_cListChassis.GetItemCount())) return;
	szChassisName = m_cListChassis.GetItemText(nChassisRow, 0);

	if ((nModelRow < 0) || (nModelRow >= m_cListModel.GetItemCount())) return;
	szModelName = m_cListModel.GetItemText(nModelRow, 0);

	//szChassisName = m_gridChassis.GetTextMatrix(nChassisRow, 0);
	//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);

	Pos = ModelList.GetHeadPosition();

	while (Pos)
	{
		CurrentPos = Pos;
		pModelData = ModelList.GetNext(Pos);

		if ((szChassisName.Compare(pModelData->m_szChassisName) == NULL)
			&& (szModelName.Compare(pModelData->m_szModelName) == NULL))
		{
			Rows.Add(pModelData->m_szDivision);
			Rows.Add(pModelData->m_szChassisName);
			Rows.Add(pModelData->m_szModelName);
			Rows.Add(pModelData->m_szSeqFilePath);
			Rows.Add(pModelData->m_szModelInfoFilePath);
			Rows.Add(pModelData->m_szRefImgFolder);

			ModelList.RemoveAt(CurrentPos);
			delete pModelData;
			break;
		}
	}
	//SaveModelListFile(); //CSelectGenerateModel_SaveModelListFile(CurrentSet->sModelListPath);
	SS.DeleteRow(Rows);

	UpdateGrid();

#endif
}


void CSelectGenerateModel::OnImportText()
{
	////CString lFileName ;
	////	lFileName = sModelInfoFolder + "ModelList.xls";
	////	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
	////
	//CString sTemp;
	//CString sTemp2;
	//CString sTempNewFolder;
	//CString sSeqFolder;
	//CIniAccess		m_Ini;				// IniAccess Class
	//CString sSection;
	//int nNoModel;
	//CStringArray Rows;

	//CFileDialog dlg(TRUE, "Model List", NULL,
	//	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY,
	//	"ModelList Excel File(*.xls)|*.xls|", NULL);

	//sSeqFolder = sModelInfoFolder;//"D:\\DFT";
	//dlg.m_ofn.lpstrInitialDir = sSeqFolder;

	//if (dlg.DoModal() != IDOK)
	//{
	//	return;
	//}
	//sTemp = dlg.GetPathName();
	//sTemp2 = sTemp.Left(sTemp.Find("ModelList.xls"));
	//sTempNewFolder = sTemp2;
	//CSpreadSheet tempSS(sTemp, "ModelList");

	////if(SS.GetTotalRows() == 0)
	////{
	////	CStringArray sampleArray;

	////	sampleArray.Add("Division");
	////	sampleArray.Add("Chassis_Name");
	////	sampleArray.Add("Model_Name");
	////	sampleArray.Add("Sequence");
	////	sampleArray.Add("ModelInfo");
	////	sampleArray.Add("RefImgFolder");
	////	
	////	SS.AddHeaders(sampleArray,true);
	////}
	//nNoModel = tempSS.GetTotalRows();
	//if (nNoModel > 1)
	//{

	//	sModelInfoFolder = sTempNewFolder;
	//	for (int i = 2; i <= nNoModel; i++)
	//	{
	//		// Read row
	//		tempSS.ReadRow(Rows, i);

	//		CModelData* pModel = new CModelData;

	//		pModel->m_szDivision = Rows.GetAt(0);
	//		pModel->m_szChassisName = Rows.GetAt(1);
	//		pModel->m_szModelName = Rows.GetAt(2);
	//		pModel->m_szSeqFilePath = Rows.GetAt(3);
	//		pModel->m_szModelInfoFilePath = Rows.GetAt(4);
	//		pModel->m_szRefImgFolder = Rows.GetAt(5);

	//		//if(CheckModel(pModel->m_szChassisName, 	pModel->m_szModelName))
	//		{
	//			ModelList.AddTail(pModel);

	//		}


	//	}

	//	ModelListOpen();
	//	UpdateGrid();

	//	//CWinApp* pApp		= AfxGetApp();
	//	//pApp->WriteProfileString(_T("Folder"), _T("Current Model Info  Root Foler"),		sModelInfoFolder);
	//	Write_DFTRegistry();
	//	m_szModelInfoPath = sModelInfoFolder;
	//	UpdateData(FALSE);
	//}

	////	SS.RollBack(); 	

}

void CSelectGenerateModel::OnImportExcel()
{
	//CString sTemp;
	//CString sSeqFolder;
	//CIniAccess		m_Ini;				// IniAccess Class
	//CString sSection;
	//int nNoModel;
	//CStringArray Rows;

	//CString lFileName;
	//lFileName = sModelInfoFolder + "ModelList.xls";
	//CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder


	//CFileDialog dlg(TRUE, "Model List", NULL,
	//	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY,
	//	"ModelList Excel File(*.xls)|*.xls|", NULL);

	//sSeqFolder = "D:\\DFT";
	//dlg.m_ofn.lpstrInitialDir = sSeqFolder;

	//if (dlg.DoModal() != IDOK)
	//{
	//	return;
	//}
	//sTemp = dlg.GetPathName();
	//CSpreadSheet tempSS(sTemp, "ModelList");

	//if (SS.GetTotalRows() == 0)
	//{
	//	CStringArray sampleArray;

	//	sampleArray.Add("Division");
	//	sampleArray.Add("Chassis_Name");
	//	sampleArray.Add("Model_Name");
	//	sampleArray.Add("Sequence");
	//	sampleArray.Add("ModelInfo");
	//	sampleArray.Add("RefImgFolder");

	//	SS.AddHeaders(sampleArray, true);
	//}
	//nNoModel = tempSS.GetTotalRows();
	//for (int i = 2; i <= nNoModel; i++)
	//{
	//	// Read row
	//	tempSS.ReadRow(Rows, i);

	//	CModelData* pModel = new CModelData;

	//	pModel->m_szDivision = Rows.GetAt(0);
	//	pModel->m_szChassisName = Rows.GetAt(1);
	//	pModel->m_szModelName = Rows.GetAt(2);
	//	pModel->m_szSeqFilePath = Rows.GetAt(3);
	//	pModel->m_szModelInfoFilePath = Rows.GetAt(4);
	//	pModel->m_szRefImgFolder = Rows.GetAt(5);

	//	if (CheckModel(pModel->m_szChassisName, pModel->m_szModelName))
	//	{
	//		ModelList.AddTail(pModel);
	//		SS.AddRow(Rows);
	//	}
	//	else {
	//		if (m_nSaveOption == 0) SS.ReplaceRows2(Rows);
	//	}

	//}

	//SS.RollBack();
	//ModelListOpen();
	//UpdateGrid();
}

//BOOL CSelectGenerateModel::GetFolder(CString* strSelectedFolder,
//	const char* lpszTitle,
//	const HWND hwndOwner,
//	const char* strRootFolder,
//	const char* strStartFolder)
//{
//	char pszDisplayName[MAX_PATH];
//	LPITEMIDLIST lpID;
//	BROWSEINFOA bi;
//
//	bi.hwndOwner = hwndOwner;
//	if (strRootFolder == NULL) {
//		bi.pidlRoot = NULL;
//	}
//	else {
//		LPITEMIDLIST  pIdl = NULL;
//		IShellFolder* pDesktopFolder;
//		char          szPath[MAX_PATH];
//		OLECHAR       olePath[MAX_PATH];
//		ULONG         chEaten;
//		ULONG         dwAttributes;
//
//		strcpy(szPath, (LPCTSTR)strRootFolder);
//		if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
//		{
//			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
//			pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
//			pDesktopFolder->Release();
//		}
//		bi.pidlRoot = pIdl;
//	}
//	bi.pszDisplayName = pszDisplayName;
//	bi.lpszTitle = lpszTitle;
//	//	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
//	bi.ulFlags = BIF_RETURNONLYFSDIRS | 0x0040 | BIF_EDITBOX;
//	bi.lpfn = BrowseCallbackProc;
//	if (strStartFolder == NULL) {
//		bi.lParam = FALSE;
//	}
//	else {
//		strTmpPath.Format("%s", strStartFolder);
//		bi.lParam = TRUE;
//	}
//	bi.iImage = NULL;
//	lpID = SHBrowseForFolderA(&bi);
//	if (lpID != NULL) {
//		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
//		if (b == TRUE) {
//			strSelectedFolder->Format("%s", pszDisplayName);
//			return TRUE;
//		}
//	}
//	else {
//		strSelectedFolder->Empty();
//	}
//	return FALSE;
//}


void CSelectGenerateModel::Write_DFTRegistry()
{
//#if 1
//	HKEY hKeyRootName = HKEY_CURRENT_USER;
//	CString cstrKeyRootName = _T("HKEY_CURRENT_USER");
//	CString cstrKeyName = _T("Software\\DFT Program1\\DFT(Digital Function Test) System");
//
//	CString cstrFullPathStr = _T("");
//	CString sTemp;
//	int iDepth = 0;
//
//	HKEY hParentKey[100];
//	TCHAR strParentString[1024][100];
//	DWORD dwParentIndex[100];
//
//	HKEY hCurKey, hPrevKey;
//	TCHAR strCurString[1024], strPrevString[1024];
//	DWORD dwCurIndex = 0, dwPrevIndex = 0;
//	LONG lResult;
//
//	lResult = RegOpenKeyEx(hKeyRootName, cstrKeyName, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);
//	cstrFullPathStr = cstrKeyRootName + _T("\\") + cstrKeyName;
//
//	do
//	{
//		lResult = RegEnumKey(hCurKey, dwCurIndex, strCurString, sizeof(strCurString));
//
//		///// assign current to previous to traverse back...
//		hPrevKey = hCurKey;
//		dwPrevIndex = dwCurIndex;
//		_tcscpy(strPrevString, strCurString);
//
//		///// sometimes we maynot be able to open the registry key and it may return ERROR_INVALID_HANDLE, 
//		///// in that case you just ignore and proceed ...
//		if ((lResult == ERROR_NO_MORE_ITEMS) || (lResult == ERROR_INVALID_HANDLE))
//		{
//			iDepth--;
//			if (iDepth < 0)
//				break;
//
//
//			////// take the values for the current depth...
//			hCurKey = hParentKey[iDepth];
//			dwCurIndex = dwParentIndex[iDepth] + 1;
//			_tcscpy(strCurString, strParentString[iDepth]);
//
//			int i = cstrFullPathStr.ReverseFind(_T('\\'));
//			CString cstrTemp;
//			cstrTemp = cstrFullPathStr.Left(i);
//			cstrFullPathStr = cstrTemp;
//
//			continue;
//		}
//		else
//		{
//			//// Copy values to parent
//			//// make current as a parent and traverse down...
//			hParentKey[iDepth] = hCurKey;
//			dwParentIndex[iDepth] = dwCurIndex;
//			_tcscpy(strParentString[iDepth], strCurString);
//
//			cstrFullPathStr += _T("\\");
//			cstrFullPathStr += strCurString;
//			sTemp.Format("%s", strCurString);
//			//if(sTemp == "File"){
//			//	sModelListFile = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current ModelList");
//			//}
//
//			if (sTemp == "Folder") {
//				//sServerFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Server Folder");
//				//sRefRootFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Reference Image Root Folder");
//				//sModelInfoFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Model Info  Root Foler");
//
//				lResult = RegReplaceKey(
//					hCurKey, // handle to open key
//					"Current Model Info  Root Foler1", // subkey name
//					sModelInfoFolder.GetBuffer(), // data file
//					NULL // backup file
//				);
//			}
//
//			lResult = RegOpenKeyEx(hCurKey, strCurString, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);
//
//			iDepth++;
//			dwCurIndex = 0;
//		}
//
//	} while (TRUE);
//
//
//
//	//
//	//	HKEY hKeyRootName = HKEY_CURRENT_USER;
//	//	CString cstrKeyRootName = _T("HKEY_CURRENT_USER");
//	//	CString cstrKeyName = _T("Software\\DFT Program1\\DFT(Digital Function Test) System");
//	//
//	//	CString cstrFullPathStr = _T("");
//	//	CString sTemp;
//	//	int iDepth = 0;
//	//
//	//	HKEY hParentKey[100];
//	//	TCHAR strParentString[1024][100];
//	//	DWORD dwParentIndex[100];
//	//
//	//	HKEY hCurKey, hPrevKey;
//	//	TCHAR strCurString[1024], strPrevString[1024];
//	//	DWORD dwCurIndex = 0, dwPrevIndex = 0;
//	//	LONG lResult;
//	//
//	//	cstrFullPathStr = cstrKeyRootName + _T("\\") + cstrKeyName + _T("\\Folder") ;
//	//
//	//	lResult = RegOpenKeyEx(hKeyRootName, cstrKeyRootName, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);
//	////	lResult = RegOpenKeyEx(hKeyRootName, cstrFullPathStr, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);
//	//	cstrFullPathStr = cstrKeyRootName + _T("\\") + cstrKeyName + _T("\\Folder") ;
//	////	LSTATUS RegReplaceKeyA(
//	////  HKEY   hKey,
//	////  LPCSTR lpSubKey,
//	////  LPCSTR lpNewFile,
//	////  LPCSTR lpOldFile
//	////);
//	////	RegSetValueEx(hCurKey,_T("Current Model Info  Root Foler"), 0, REG_SZ, (BYTE*) sModelInfoFolder.GetBuffer(),sModelInfoFolder.GetLength());
//	//
//	//	RegSetValueEx(hCurKey,_T("Current Model Info  Root Foler"), 0, REG_SZ, (BYTE*) sModelInfoFolder.GetBuffer(),sModelInfoFolder.GetLength());
//	//
//
//		// start enumetating all keys under HKLM
//		//HKEY hKey = HKEY_LOCAL_MACHINE;
//		//while( (rc=RegEnumKeyEx(
//		//	hKey,
//		//	dwSubKeyIndex,
//		//	szSubKey,
//		//	&dwSubKeyLength,
//		//	NULL,
//		//	NULL,
//		//	NULL,
//		//	NULL)
//		//	) != ERROR_NO_MORE_ITEMS) 
//		//{ 
//		//
//		//	if(rc == ERROR_SUCCESS)
//		//	{
//		//		HKEY hKeyToRestore; // Handle of subkey to save
//		//		DWORD dwDisposition;
//
//		//		if((rc=RegCreateKeyEx(hKey,
//		//		szSubKey, // Name of subkey to open
//		//		0,
//		//		NULL,
//		//		REG_OPTION_BACKUP_RESTORE, // in winnt.h
//		//		KEY_WRITE, 
//		//		NULL,
//		//		&hKeyToRestore,
//		//		&dwDisposition)
//		//		) == ERROR_SUCCESS)
//		//		{
//		//			// Restore registry subkey..... 
//		//			char path[256];
//		//			memset(path,0,256);
//		//			strcpy(path,"D:\\tests\\"); 
//		//			strcat(path,szSubKey);
//
//		//			rc = RegReplaceKey(
//		//			hKeyToSave, // handle to open key
//		//			szSubKey, // subkey name
//		//			path, // data file
//		//			NULL // backup file
//		//			);
//
//		//			if(rc != ERROR_SUCCESS)
//		//			{
//		//				strcat(szSubKey," NOT Restored");
//		//				ErrorHandler(szSubKey); //Display error
//		//			}
//
//		//			// close registry key we just tried to restore
//		//			RegCloseKey(hKeyToRestore);
//		//		}
//		//		// increment index into the key
//		//		dwSubKeyIndex++;
//
//		//		// reset buffer size
//		//		dwSubKeyLength=_MAX_FNAME;
//
//		//		continue;
//		//	}
//	///////////////////////////////////////////////////
//
//	//	LONG lResult;
//	//char buffer[100];
//	//HKEY hKey;
//	//DWORD dwDesc;
//	////char *path = "c:winntsystem32
//	////otepad.exe"; // 노트 패드 경로
//	// 
//	//// 레지스트리 열고
//	//lResult =  RegOpenKeyEx(HKEY_CURRENT_USER,
//	//    "Software\\DFT Program1\\DFT(Digital Function Test) System",
//	//    0, KEY_ENUMERATE_SUB_KEYS, &hKey);
//	// 
//	////// Set Registry Key & Value
//	////lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
//	////    "SOFTWAREMicrosoftWindowsCurrentVersionRun",
//	////    0, buffer, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
//	////    NULL, &hKey, &dwDesc);
//	// 
//	//if(lResult == ERROR_SUCCESS)
//	//{
//	//    RegSetValueEx(hKey, "Folder1", 0, REG_SZ, (BYTE*) sModelInfoFolder.GetBuffer(),sModelInfoFolder.GetLength());
//	//}
//	// 
//	////레지스트리 닫고
//	//RegCloseKey(hKey);//	CWinApp* pApp		= AfxGetApp();
//
//#else
//
//
//	HKEY hKeyRootName = HKEY_CURRENT_USER;
//	CString cstrKeyRootName = _T("HKEY_CURRENT_USER");
//	CString cstrKeyName = _T("Software\\DFT Program1\\DFT(Digital Function Test) System");
//
//	CString cstrFullPathStr = _T("");
//	CString sTemp;
//	int iDepth = 0;
//
//	HKEY hParentKey[100];
//	TCHAR strParentString[1024][100];
//	DWORD dwParentIndex[100];
//
//	HKEY hCurKey, hPrevKey;
//	TCHAR strCurString[1024], strPrevString[1024];
//	DWORD dwCurIndex = 0, dwPrevIndex = 0;
//	LONG lResult;
//
//	lResult = RegOpenKeyEx(hKeyRootName, cstrKeyName, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);
//	cstrFullPathStr = cstrKeyRootName + _T("\\") + cstrKeyName;
//
//
//	//	RegSetValueEx(hKey, "abc", 0, REG_SZ, (BYTE*)path, strlen(path));
//
//		//ret = RegOpenKeyEx(  
//		//        HKEY_LOCAL_MACHINE ,                            // 키 값  
//		//        "SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2",// 서브 키값  
//		//        0,                                              // 옵션 항상 0  
//		//        KEY_SET_VALUE,                                  // 접근 권한  
//		//        &hKey                                           // 키 핸들  
//		//        );  
//
//	if (lResult == ERROR_SUCCESS)
//	{
//		//memset( data_buffer, 0, sizeof( data_buffer ) );  
//		//sprintf( data_buffer, "/cygdrive" );             
//		//data_size = strlen( data_buffer ) + 1;               
//		RegSetValueEx(
//			hCurKey,                           // RegOpenKeyEx에 얻어진 키 핸들  
//			"Current Model Info  Root Foler",              // 키 안에 값 이름  
//			0,                              // 옵션 항상 0  
//			REG_SZ,                         // 데이터 타입  
//			(BYTE*)sModelInfoFolder.GetBuffer(),// (BYTE*) data_buffer,            // 써 넣을 데이터  
//			sModelInfoFolder.GetLength()//  data_size                       // 데이터 크기  
//		);
//		RegCloseKey(hCurKey);
//	}
//#endif

	//strCurString, cstrFullPathStr, "Current Model Info  Root Foler"


	//do
	//{
	//	lResult = RegEnumKey(hCurKey, dwCurIndex, strCurString, sizeof(strCurString));

	//	///// assign current to previous to traverse back...
	//	hPrevKey = hCurKey;
	//	dwPrevIndex = dwCurIndex;
	//	_tcscpy(strPrevString, strCurString);

	//	///// sometimes we maynot be able to open the registry key and it may return ERROR_INVALID_HANDLE, 
	//	///// in that case you just ignore and proceed ...
	//	if((lResult == ERROR_NO_MORE_ITEMS) || (lResult == ERROR_INVALID_HANDLE))
	//	{
	//		iDepth--;
	//		if(iDepth < 0)
	//			break;


	//		////// take the values for the current depth...
	//		hCurKey = hParentKey[iDepth];
	//		dwCurIndex = dwParentIndex[iDepth] + 1;
	//		_tcscpy(strCurString, strParentString[iDepth]);

	//		int i = cstrFullPathStr.ReverseFind(_T('\\'));
	//		CString cstrTemp;
	//		cstrTemp = cstrFullPathStr.Left(i);
	//		cstrFullPathStr = cstrTemp;

	//		continue;
	//	}
	//	else
	//	{
	//		//// Copy values to parent
	//		//// make current as a parent and traverse down...
	//		hParentKey[iDepth] = hCurKey;
	//		dwParentIndex[iDepth] = dwCurIndex;
	//		_tcscpy(strParentString[iDepth], strCurString);

	//		cstrFullPathStr += _T("\\");
	//		cstrFullPathStr += strCurString;
	//		sTemp.Format("%s", strCurString); 
	//		if(sTemp == "File"){
	//			sModelListFile = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current ModelList");
	//		}

	//		if(sTemp == "Folder"){
	//			sServerFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Server Folder");
	//			sRefRootFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Reference Image Root Folder");
	//			sModelInfoFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Model Info  Root Foler");
	//		}

	//		lResult = RegOpenKeyEx(hCurKey, strCurString, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);

	//		iDepth ++;
	//		dwCurIndex = 0;
	//	}

	//}while(TRUE);

}


//void Read_DFTRegistry()
//{
//
//	HKEY hKeyRootName = HKEY_CURRENT_USER;
//	CString cstrKeyRootName = _T("HKEY_CURRENT_USER");
//	CString cstrKeyName = _T("Software\\DFT Program1\\DFT(Digital Function Test) System");
//
//	CString cstrFullPathStr = _T("");
//	CString sTemp;
//	int iDepth = 0;
//
//	HKEY hParentKey[100];
//	TCHAR strParentString[1024][100];
//	DWORD dwParentIndex[100];
//
//	HKEY hCurKey, hPrevKey;
//	TCHAR strCurString[1024], strPrevString[1024];
//	DWORD dwCurIndex = 0, dwPrevIndex = 0;
//	LONG lResult;
//
//	lResult = RegOpenKeyEx(hKeyRootName, cstrKeyName, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);
//	cstrFullPathStr = cstrKeyRootName + _T("\\") + cstrKeyName;
//
//	do
//	{
//		lResult = RegEnumKey(hCurKey, dwCurIndex, strCurString, sizeof(strCurString));
//
//		///// assign current to previous to traverse back...
//		hPrevKey = hCurKey;
//		dwPrevIndex = dwCurIndex;
//		_tcscpy(strPrevString, strCurString);
//
//		///// sometimes we maynot be able to open the registry key and it may return ERROR_INVALID_HANDLE, 
//		///// in that case you just ignore and proceed ...
//		if ((lResult == ERROR_NO_MORE_ITEMS) || (lResult == ERROR_INVALID_HANDLE))
//		{
//			iDepth--;
//			if (iDepth < 0)
//				break;
//
//
//			////// take the values for the current depth...
//			hCurKey = hParentKey[iDepth];
//			dwCurIndex = dwParentIndex[iDepth] + 1;
//			_tcscpy(strCurString, strParentString[iDepth]);
//
//			int i = cstrFullPathStr.ReverseFind(_T('\\'));
//			CString cstrTemp;
//			cstrTemp = cstrFullPathStr.Left(i);
//			cstrFullPathStr = cstrTemp;
//
//			continue;
//		}
//		else
//		{
//			//// Copy values to parent
//			//// make current as a parent and traverse down...
//			hParentKey[iDepth] = hCurKey;
//			dwParentIndex[iDepth] = dwCurIndex;
//			_tcscpy(strParentString[iDepth], strCurString);
//
//			cstrFullPathStr += _T("\\");
//			cstrFullPathStr += strCurString;
//			sTemp.Format("%s", strCurString);
//			if (sTemp == "File") {
//				CurrentSet->sModelListPath = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current ModelList");
//			}
//
//			if (sTemp == "Folder") {
//				CurrentSet->sServerFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Server Folder");
//				CurrentSet->sRefRootFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Reference Image Root Folder");
//				CurrentSet->sModelInfoFolder = EnumerateValues(hCurKey, strCurString, cstrFullPathStr, "Current Model Info  Root Foler");
//			}
//
//			lResult = RegOpenKeyEx(hCurKey, strCurString, 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);
//
//			iDepth++;
//			dwCurIndex = 0;
//		}
//
//	} while (TRUE);
//
//}

/////////////////////////////////////////////////////////////////////////////////////////
//// Enumerates through the values of the given key and store the values into the file.
//// Invokes "FormatDataWithDataType" function to format the data
//// 
//// Inputs	:	1. Handle to the key
////			2. Key name
////			3. File pointer
////			4. Full path of the key to store in the file
/////////////////////////////////////////////////////////////////////////////////////////
//CString EnumerateValues(HKEY hKey, CString cstrKey, CString cstrFullPath, CString cstrValueName)
//{
//	static HKEY hLastKey = hKey;
//
//	LONG lResult;
//	DWORD dwIndex = 0;
//
//	HKEY hCurKey = hKey;
//
//	DWORD dwKeyType;
//	DWORD dwKeyDataLength, dwKeyNameLen;
//
//	LPBYTE pbbinKeyData = NULL;
//	TCHAR *tcKeyName = NULL;
//	TCHAR tcDataType[1024] = _T("");
//
//	CString cstrFinalData = _T("");
//	CString cstrTemp;
//
//	lResult = RegOpenKeyEx(hCurKey, cstrKey, 0, KEY_QUERY_VALUE, &hKey);
//	if (lResult != ERROR_SUCCESS)
//		return cstrFinalData;
//
//	DWORD lNoOfValues = 0;
//	DWORD lLongestKeyNameLen = 1;
//	DWORD lLongestDataLen = 1;
//
//	lResult = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &lNoOfValues, &lLongestKeyNameLen, &lLongestDataLen,
//		NULL, NULL);
//
//	if (lResult != ERROR_SUCCESS)
//		return cstrFinalData;
//
//	hLastKey = hKey;
//
//	lLongestKeyNameLen++;
//	lLongestDataLen++;
//
//	tcKeyName = new TCHAR[lLongestKeyNameLen];
//	pbbinKeyData = new BYTE[lLongestDataLen];
//
//	while (TRUE)
//	{
//		memset(pbbinKeyData, 0, lLongestDataLen);
//		memset(tcKeyName, 0, lLongestKeyNameLen);
//		dwKeyType = dwKeyDataLength = dwKeyNameLen = 0;
//
//		dwKeyNameLen = lLongestKeyNameLen;
//		dwKeyDataLength = lLongestDataLen;
//
//		lResult = RegEnumValue(hKey, dwIndex, tcKeyName, &dwKeyNameLen, NULL, &dwKeyType, pbbinKeyData, &dwKeyDataLength);
//		if (lResult == ERROR_NO_MORE_ITEMS)
//			break;
//
//		FormatDataWithDataType(dwKeyType, pbbinKeyData, dwKeyDataLength, cstrFinalData);
//
//		cstrTemp = tcKeyName;
//
//		if (cstrTemp == cstrValueName)
//		{
//			break;
//		}
//		dwIndex++;
//	}
//
//	delete tcKeyName;
//	delete pbbinKeyData;
//
//	return cstrFinalData;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Format the value with the actual data as the same way the REGEDIT does....
//// Inputs: 1. Data type of the key
////		 2. Data to be formatted
////		 3. Length of the data
//// Output: 4. Formatted string
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void FormatDataWithDataType(DWORD dwKeyType, LPBYTE pbbinKeyData, DWORD dwKeyDataLength, CString &cstrOutput)
//{
//	CString cstrTemp, cstrTemp1;
//	switch (dwKeyType)
//	{
//	case REG_SZ:
//	{
//		cstrTemp.Format(_T("%s"), pbbinKeyData);
//		cstrOutput = cstrTemp;
//		break;
//	}
//
//	case REG_DWORD: /// same is for REG_DWORD_LITTLE_ENDIAN
//	{
//		DWORD dwValue;
//		memcpy(&dwValue, pbbinKeyData, sizeof DWORD);
//		cstrTemp.Format(_T("dword:%08x"), dwValue);
//		cstrOutput = cstrTemp;
//		break;
//	}
//
//	case REG_BINARY:
//	case REG_MULTI_SZ:
//	case REG_EXPAND_SZ:
//	case REG_FULL_RESOURCE_DESCRIPTOR:
//	case REG_RESOURCE_LIST:
//	case REG_RESOURCE_REQUIREMENTS_LIST:
//	{
//		if (dwKeyType != REG_BINARY)
//			cstrOutput.Format(_T("hex(%d):"), dwKeyType);
//		else
//			cstrOutput.Format(_T("hex:"));
//
//		for (DWORD dwIndex = 0; dwIndex < dwKeyDataLength; dwIndex++)
//		{
//			cstrTemp1.Format(_T("%02x,"), pbbinKeyData[dwIndex]);
//			cstrTemp += cstrTemp1;
//			if (dwIndex != 0 && (dwIndex % 0x15 == 0))
//				cstrTemp += _T("\\\n");
//		}
//
//		cstrOutput += cstrTemp;
//		break;
//	}
//
//	case REG_NONE:
//	case REG_DWORD_BIG_ENDIAN:
//	case REG_LINK:
//		//// TODO : add code for these types...
//		break;
//
//	}
//}
//




void CSelectGenerateModel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

//	SaveModelListFile();


	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString(_T("File"), _T("Current ModelList"), CurrentSet->sModelListPath);
	CDialog::OnOK();
}


void CSelectGenerateModel::OnBnClickedBtnChangeModelPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString lFileName ;
	//	lFileName = sModelInfoFolder + "ModelList.xls";
	//	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
	//
	CString sTemp;
	CString sTemp2;
	CString sTempNewFolder;
	CString sSeqFolder;
	CIniAccess		m_Ini;				// IniAccess Class
	CString sSection;
	int nNoModel;
	CStringArray Rows;

	CFileDialog dlg(TRUE, "Model List", NULL,
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY,
		"ModelList Excel File(*.xls)|*.xls|", NULL);

	sSeqFolder = CurrentSet->sModelListPath;// sModelInfoFolder;//"D:\\DFT";
	sTempNewFolder = sSeqFolder.Left(sSeqFolder.ReverseFind('\\'));
	dlg.m_ofn.lpstrInitialDir = sTempNewFolder;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	sTemp = dlg.GetPathName();
	//sTemp2 = sTemp.Left(sTemp.Find("ModelList.xls"));
	sTempNewFolder = sTemp;

	CurrentSet->sModelListPath = sTempNewFolder;

	ModelListOpen();
	UpdateGrid();


//	Write_DFTRegistry();
	m_szModelListPath = CurrentSet->sModelListPath;

	UpdateData(FALSE);



//	CSpreadSheet tempSS(sTemp, "ModelList");

	//if(SS.GetTotalRows() == 0)
	//{
	//	CStringArray sampleArray;

	//	sampleArray.Add("Division");
	//	sampleArray.Add("Chassis_Name");
	//	sampleArray.Add("Model_Name");
	//	sampleArray.Add("Sequence");
	//	sampleArray.Add("ModelInfo");
	//	sampleArray.Add("RefImgFolder");
	//	
	//	SS.AddHeaders(sampleArray,true);
	//}
	//nNoModel = tempSS.GetTotalRows();
	//if (nNoModel > 1)
	//{

	//	sModelInfoFolder = sTempNewFolder;
	//	for (int i = 2; i <= nNoModel; i++)
	//	{
	//		 Read row
	//		tempSS.ReadRow(Rows, i);

	//		CModelData* pModel = new CModelData;

	//		pModel->m_szDivision = Rows.GetAt(0);
	//		pModel->m_szChassisName = Rows.GetAt(1);
	//		pModel->m_szModelName = Rows.GetAt(2);
	//		pModel->m_szSeqFilePath = Rows.GetAt(3);
	//		pModel->m_szModelInfoFilePath = Rows.GetAt(4);
	//		pModel->m_szRefImgFolder = Rows.GetAt(5);

	//		if(CheckModel(pModel->m_szChassisName, 	pModel->m_szModelName))
	//		{
	//			ModelList.AddTail(pModel);

	//		}


	//	}

	//	ModelListOpen();
	//	UpdateGrid();

	//	CWinApp* pApp		= AfxGetApp();
	//	pApp->WriteProfileString(_T("Folder"), _T("Current Model Info  Root Foler"),		sModelInfoFolder);
	//	Write_DFTRegistry();
	//	m_szModelInfoPath = sModelInfoFolder;
	//	UpdateData(FALSE);
	//}

	//	SS.RollBack(); 	
}


void CSelectGenerateModel::OnNMClickListChassis(NMHDR *pNMHDR, LRESULT *pResult)
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


void CSelectGenerateModel::OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult)
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


void CSelectGenerateModel::OnNMCustomdrawListChassis(NMHDR *pNMHDR, LRESULT *pResult)
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
			crBkgnd =  RGB(255, 0, 255); ////m_cListChassisEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
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


void CSelectGenerateModel::OnNMCustomdrawListModel(NMHDR *pNMHDR, LRESULT *pResult)
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


void CSelectGenerateModel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateGridLast();
	CDialog::OnTimer(nIDEvent);
}


void CSelectGenerateModel::UpdateGridLast()
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

	Pos = ModelList.GetHeadPosition();
	int lPos = m_cListChassis.GetItemCount();
	int lMpos = ModelList.GetCount();

	if (lPos >= lMpos)
	{
		KillTimer(1);
	}

	while (Pos)
	{
		pModelData = ModelList.GetNext(Pos);

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

void CSelectGenerateModel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CurrentSet->sModelListPath = m_ModelListPathOld;

	CopyFile("D:\\DFT\\Temp.xls", CurrentSet->sModelListPath, 0);
	CDialog::OnCancel();
}
//BOOL CopyFile(
//	LPCTSTR lpExistingFileName,
//	LPCTSTR lpNewFileName,
//	BOOL    bFailIfExists
//);