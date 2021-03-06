// ModelSelectDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "ModelSelectDlg.h"
#include "afxdialogex.h"


// ModelSelectDlg 대화 상자

IMPLEMENT_DYNAMIC(ModelSelectDlg, CDialogEx)

ModelSelectDlg::ModelSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MODEL_SELECT, pParent)
	, m_szSeqFilePath(_T(""))
	, m_szModelInfoFilePath(_T(""))
	, m_szRefImgFolder(_T(""))
{

}

ModelSelectDlg::~ModelSelectDlg()
{
}

void ModelSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODEL_LIST_DISPLAY, m_cListModelListDisplay);
	DDX_Control(pDX, IDC_LIST_CHASSIS_LIST_DISPLAY, m_cListCtrlChassisDisplay);
	DDX_Control(pDX, IDC_COMBO_YEAR_DEVISION, m_cComboYearDivision);
	DDX_Text(pDX, IDC_EDIT_SEQ_FILE, m_szSeqFilePath);
	DDX_Text(pDX, IDC_EDIT_MODEL_INFO_FILE, m_szModelInfoFilePath);
	DDX_Text(pDX, IDC_EDIT_REF_FOLDER, m_szRefImgFolder);
}


BEGIN_MESSAGE_MAP(ModelSelectDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHASSIS_LIST_DISPLAY, &ModelSelectDlg::OnNMClickListChassisListDisplay)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MODEL_LIST_DISPLAY, &ModelSelectDlg::OnNMClickListModelListDisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_YEAR_DEVISION, &ModelSelectDlg::OnCbnSelchangeComboYearDevision)
END_MESSAGE_MAP()



BOOL ModelSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CString sTemp;
	SetListControl();

	m_cComboYearDivision.AddString("ALL");
	for (int i = 0; i < g_Divisions.GetSize(); i++)
	{
		sTemp = g_Divisions.GetAt(i);
		m_cComboYearDivision.AddString(sTemp);
	}
	m_cComboYearDivision.SetCurSel(0);
	

	m_nDivision_Sel = 0;
	m_nChaassis_SelectRow = 0;
	m_nModel_SelectRow = 0;

	UpdateGrid();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


// ModelSelectDlg 메시지 처리기
void ModelSelectDlg::SetListControl()
{
	font = new CFont;
	font->CreateFontA(16,                          // nHeight
		0,                          // nWidth
		0,                          // nEscapement
		0,                          // nOrientation
		400,                           // nWeight
		0,                          // bItalic
		0,                          // bUnderline 
		0,                          // cStrikeOut 
		0,                          // nCharSet
		0,           // nOutPrecision 
		0,                          // nClipPrecision 
		PROOF_QUALITY,              // nQuality
		0,  // nPitchAndFamily 
		_T("Arial"));                     // lpszFacename

	CString strTemp;

		m_cListCtrlChassisDisplay.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
		m_cListCtrlChassisDisplay.InsertColumn(0, _T("CHASSIS NAME"), LVCFMT_CENTER, 200);
		m_cListCtrlChassisDisplay.SetFont(font);

		m_cListModelListDisplay.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );// 리스트 컨트롤 초기화: 열 추가
		m_cListModelListDisplay.InsertColumn(0, _T("MODEL NAME"), LVCFMT_CENTER, 200);
		m_cListModelListDisplay.SetFont(font);
	

		//}
	

	delete font;



}


void ModelSelectDlg::UpdateGrid()
{
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	CString		szChassisName = _T("");
	int			nChassisRow = 0;
	CString sTemp;


	Pos = ModelList.GetHeadPosition();
	m_cListCtrlChassisDisplay.DeleteAllItems();

	while (Pos)
	{
		pModelData = ModelList.GetNext(Pos);

		szChassisName = pModelData->m_szChassisName;

		if (IsSameChassis(szChassisName) != TRUE)
		{
			if (m_nDivision_Sel == 0)
			{
				//m_gridChassis.SetRows(nChassisRow + 1);
				//m_gridChassis.SetTextMatrix(nChassisRow++, 0, szChassisName);
				//m_cListCtrlChassisDisplay.
				m_cListCtrlChassisDisplay.InsertItem(nChassisRow, szChassisName);
				nChassisRow++;
			}
			else {
				m_cComboYearDivision.GetLBText(m_nDivision_Sel, sTemp);
				if (sTemp == pModelData->m_szDivision)
				{
				//	m_gridChassis.SetRows(nChassisRow + 1);
				//	m_gridChassis.SetTextMatrix(nChassisRow++, 0, szChassisName);
					m_cListCtrlChassisDisplay.InsertItem(nChassisRow, szChassisName);
					nChassisRow++;
				}
			}
		}
	}

//	m_gridChassis.SetRedraw(TRUE);
//	m_gridChassis.Refresh();

	if (m_cListCtrlChassisDisplay.GetItemCount() > 1)
	{
		m_cListCtrlChassisDisplay.SetSelectionMark(m_nChaassis_SelectRow);
	//	m_cListCtrlChassisDisplay.SetItem(m_nChaassis_SelectRow);
		OnClickChassisGrid(m_nChaassis_SelectRow);
	}
}

BOOL ModelSelectDlg::IsSameChassis(CString szChassisName)
{
	CString	szRegisteredChassisName = _T("");

	int	nNoChassis = m_cListCtrlChassisDisplay.GetItemCount();

	szChassisName.MakeUpper();

	for (int i = 0; i < nNoChassis; i++)
	{
		szRegisteredChassisName = m_cListCtrlChassisDisplay.GetItemText(i , 0);

		szRegisteredChassisName.MakeUpper();

		if (szChassisName.Compare(szRegisteredChassisName) == NULL)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void ModelSelectDlg::OnNMClickListChassisListDisplay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	

	int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
	int nlSubItem = static_cast<int>(pLVCD->iSubItem); //row

	int			nCurChassisRow = (int)(m_cListCtrlChassisDisplay.GetFirstSelectedItemPosition())-1;// m_gridModel.GetRow();
	OnClickChassisGrid(nCurChassisRow);
	*pResult = 0;
}

void ModelSelectDlg::OnClickChassisGrid(int			nChassisRow)
{
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	
	int			nCurModelRow =(int)( m_cListModelListDisplay.GetFirstSelectedItemPosition())-1;// m_gridModel.GetRow();
	int			nModelRow = 0;

	CString szChassisName = _T("");
	CString szModelName = _T("");

	if ((nChassisRow <= 0) && (m_cListCtrlChassisDisplay.GetItemCount() > 0))
	{
		m_cListCtrlChassisDisplay.SetSelectionMark(0);
		nChassisRow = 0;
	}
	else if ((nChassisRow >= m_cListCtrlChassisDisplay.GetItemCount()) && (m_cListCtrlChassisDisplay.GetItemCount() > 1))
	{
		m_cListCtrlChassisDisplay.SetSelectionMark(m_cListCtrlChassisDisplay.GetItemCount() - 1);
		nChassisRow = m_cListCtrlChassisDisplay.GetItemCount() - 1;
	}

	szChassisName = m_cListCtrlChassisDisplay.GetItemText(nChassisRow, 0);

	Pos = ModelList.GetHeadPosition();
	m_cListModelListDisplay.DeleteAllItems();
	while (Pos)
	{
		pModelData = ModelList.GetNext(Pos);

		if (pModelData->m_szChassisName != _T(""))
		{
			if (pModelData->m_szChassisName.Compare(szChassisName) == NULL)
			{
				szModelName = pModelData->m_szModelName;
				m_cListModelListDisplay.InsertItem(nModelRow, szModelName);//m_gridModel.SetRows(nModelRow + 1);
				//m_gridModel.SetTextMatrix(nModelRow++, 0, szModelName);
				nModelRow++;
			}
		}
	}
	//ChangeRowColor(nChassisRow, CHASSIS_GRID);
	if (m_sSelChassis != szChassisName)
	{
		m_sSelChassis = szChassisName;
		m_cListModelListDisplay.SetSelectionMark(0);//m_gridModel.SetRow(1);
	}
	else {
		if (nCurModelRow <= 0) {
			m_cListModelListDisplay.SetSelectionMark(0); //m_gridModel.SetRow(1);
		}
		else if (nCurModelRow >= m_cListModelListDisplay.GetItemCount()) {
			m_cListModelListDisplay.SetSelectionMark(m_cListModelListDisplay.GetItemCount() - 1);
		}
		else {
			m_cListModelListDisplay.SetSelectionMark(m_nModel_SelectRow);
		}
	}
	m_nChaassis_SelectRow = nChassisRow;
	OnClickModelGrid((int)m_cListModelListDisplay.GetFirstSelectedItemPosition()-1);
}

void ModelSelectDlg::OnClickModelGrid(int			nModelRow)
{
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	//int			nModelRow = m_gridModel.GetRow();
	//int			nChassisRow = m_gridChassis.GetRow();

	CString szChassisName = _T("");
	CString szModelName = _T("");

	if ((nModelRow <= 0) && (m_cListModelListDisplay.GetItemCount() > 0))
	{
		m_cListModelListDisplay.SetSelectionMark(0);
		nModelRow = 0;
	}
	else if ((nModelRow >= m_cListModelListDisplay.GetItemCount()) && (m_cListModelListDisplay.GetItemCount() > 0))
	{
		m_cListModelListDisplay.SetSelectionMark(m_cListModelListDisplay.GetItemCount() - 1);
		nModelRow = m_cListModelListDisplay.GetItemCount() - 1;
	}

	szChassisName = m_cListCtrlChassisDisplay.GetItemText(m_nChaassis_SelectRow, 0);
	szModelName = m_cListModelListDisplay.GetItemText(nModelRow, 0);
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

				break;
			}
		}
	}

	UpdateData(FALSE);
	m_nModel_SelectRow = nModelRow;
	//ChangeRowColor(nModelRow, MODEL_GRID);
	//	delete pModelData;
}

void ModelSelectDlg::OnNMClickListModelListDisplay(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;


	int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
	int nlSubItem = static_cast<int>(pLVCD->iSubItem); //row
	//OnClickChassisGrid(nItem);
	int			nCurModelRow = (int)(m_cListModelListDisplay.GetFirstSelectedItemPosition())-1;
	OnClickModelGrid(nCurModelRow);


	*pResult = 0;
}


void ModelSelectDlg::OnCbnSelchangeComboYearDevision()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nDivision_Sel = m_cComboYearDivision.GetCurSel();
	UpdateGrid();
}
