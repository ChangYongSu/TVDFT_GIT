// FullSeqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "FullSeqDlg.h"

//+add kwmoon 080508
#include "SeqEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFullSeqDlg dialog


CFullSeqDlg::CFullSeqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFullSeqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFullSeqDlg)
	//}}AFX_DATA_INIT

	//+add kwmoon 080508
	m_pParentDlg = NULL;
}


void CFullSeqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFullSeqDlg)
	DDX_Control(pDX, IDC_FULL_SEQ_PATH_EDIT, m_ctrlFullSeqPathEdit);
	DDX_Control(pDX, IDC_FULL_SEQ_GRID, m_ctrlFullSeqGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFullSeqDlg, CDialog)
	//{{AFX_MSG_MAP(CFullSeqDlg)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CFullSeqDlg::InitFullSeqGrid()
{
	CString sTmp;

	char *sHeader[] = {"NO","STEP NAME", "STEP TYPE", "TEST", "SOURCE", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT"};

	m_ctrlFullSeqGrid.SetRedraw(flexRDNone);
	m_ctrlFullSeqGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlFullSeqGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlFullSeqGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlFullSeqGrid.SetBackColorBkg(RGB(247,247,231));
	m_ctrlFullSeqGrid.SetBackColor(RGB(247,247,231));

//	m_ctrlFullSeqGrid.SetBackColorSel(RGB(255,255,128));
//	m_ctrlFullSeqGrid.SetForeColorSel(RGB(0,0,0));

	m_ctrlFullSeqGrid.SetAllowSelection(FALSE);

	m_ctrlFullSeqGrid.SetFixedRows(1);
	m_ctrlFullSeqGrid.SetFixedCols(0);

	m_ctrlFullSeqGrid.SetCols(9);
	m_ctrlFullSeqGrid.SetRows(10);
	m_ctrlFullSeqGrid.SetRow(0);

	//========
	// Header
	//========
	for(UINT nTmp = 0; nTmp < 9 ; nTmp++)
	{
		m_ctrlFullSeqGrid.SetCol(nTmp);
		m_ctrlFullSeqGrid.SetFixedAlignment(nTmp, flexAlignCenterCenter);
		m_ctrlFullSeqGrid.SetFillStyle(flexFillRepeat);
		m_ctrlFullSeqGrid.SetFontName("Arial");
		m_ctrlFullSeqGrid.SetFontSize(10);
		m_ctrlFullSeqGrid.SetExtendLastCol(TRUE);
		m_ctrlFullSeqGrid.SetText(sHeader[nTmp]);
		m_ctrlFullSeqGrid.SetEditable(flexEDNone);
		m_ctrlFullSeqGrid.SetFocusRect(flexFocusNone);
		m_ctrlFullSeqGrid.SetAllowSelection(FALSE);
		
		if(nTmp == 0) m_ctrlFullSeqGrid.SetColWidth(nTmp, 500);
		else if(nTmp == 1) m_ctrlFullSeqGrid.SetColWidth(nTmp, 4000);
		else if(nTmp == 2) m_ctrlFullSeqGrid.SetColWidth(nTmp, 1800);
		else if(nTmp == 3) m_ctrlFullSeqGrid.SetColWidth(nTmp, 2500);
		else if(nTmp == 4) m_ctrlFullSeqGrid.SetColWidth(nTmp, 1800);
		else if(nTmp == 5) m_ctrlFullSeqGrid.SetColWidth(nTmp, 1000);
		else if(nTmp == 6) m_ctrlFullSeqGrid.SetColWidth(nTmp, 1000);
		else if(nTmp == 7) m_ctrlFullSeqGrid.SetColWidth(nTmp, 1000);
		else if(nTmp == 8) m_ctrlFullSeqGrid.SetColWidth(nTmp, 1000);
	}

	m_ctrlFullSeqGrid.SetRow(0);
	m_ctrlFullSeqGrid.SetCol(0);
	m_ctrlFullSeqGrid.SetRedraw(-1);
	m_ctrlFullSeqGrid.Refresh();
}

void CFullSeqDlg::UpdateFullSeqGrid()
{
	m_ctrlFullSeqGrid.SetRedraw(flexRDNone);
	m_ctrlFullSeqGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearText	))); 

	m_ctrlFullSeqGrid.SetRows(FullSeq.GetCount() + 1);

	//+Insert Step Default Data
	int nTmp = 0;

	CString sTmp;
	POSITION Position = FullSeq.GetHeadPosition();

	if(FullSeq.GetCount() > 0)
	{
		nTmp = 1;

		while (Position != NULL) 
		{
			//CStep* pStep = new CStep();
			CStep* pStep = FullSeq.GetNext(Position);

			m_ctrlFullSeqGrid.SetRow(nTmp);

			//Step No
			sTmp.Format("%d", pStep->GetStepNo());
			m_ctrlFullSeqGrid.SetCol(0);
			m_ctrlFullSeqGrid.SetColDataType(0, flexDTString);
			m_ctrlFullSeqGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 0, sTmp);
		
			//Step Name
			m_ctrlFullSeqGrid.SetCol(1);
			m_ctrlFullSeqGrid.SetColDataType(1, flexDTString);
			m_ctrlFullSeqGrid.SetCellAlignment(flexAlignLeftCenter);
			m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 1, pStep->GetStepName());
			
			//+add kwmoon 080724
			if(pStep->m_strCurSrc.Left(2).Compare("RF") == NULL)
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,100));
			}
			else if(pStep->m_strCurSrc.Left(2).Compare("AV") == NULL)
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,120));
			}
			else if(pStep->m_strCurSrc.Left(4).Compare("COMP") == NULL)
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,140));
			}
			else if(pStep->m_strCurSrc.Left(3).Compare("RGB") == NULL)
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,160));
			}
			else if(pStep->m_strCurSrc.Left(2).Compare("YC") == NULL)
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,180));
			}
			else if(pStep->m_strCurSrc.Left(4).Compare("VOUT") == NULL)
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,200));
			}
			else if(pStep->m_strCurSrc.Left(4).Compare("HDMI") == NULL)
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,220));
			}
			else
			{
				m_ctrlFullSeqGrid.SetCellBackColor(RGB(200,200,240));
			}

			//Step Type
			if(pStep->m_nStepType == CHECK_FUNCTION) sTmp.Format("CHECK");
			else if(pStep->m_nStepType == PROCESS_FUNCTION) sTmp.Format("PROCESS");
			else sTmp.Format(_T(""));

			m_ctrlFullSeqGrid.SetCol(2);
			m_ctrlFullSeqGrid.SetColDataType(2, flexDTString);
			m_ctrlFullSeqGrid.SetCellAlignment(flexAlignLeftCenter);
			m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 2, sTmp);
			
			if(pStep->m_nStepType == CHECK_FUNCTION)
			{
				//Main Step Test Function Name
				m_ctrlFullSeqGrid.SetCol(3);
				m_ctrlFullSeqGrid.SetColDataType(3, flexDTString);
				m_ctrlFullSeqGrid.SetCellAlignment(flexAlignLeftCenter);
				m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 3, pStep->m_strFuncName);
				

				//Current Source
				m_ctrlFullSeqGrid.SetCol(4);
				m_ctrlFullSeqGrid.SetColDataType(4, flexDTString);
				m_ctrlFullSeqGrid.SetCellAlignment(flexAlignLeftCenter);
				m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 4, pStep->m_strCurSrc);

			
//				//Next Source
//				m_ctrlFullSeqGrid.SetCol(5);
//				m_ctrlFullSeqGrid.SetColDataType(5, flexDTString);
//				m_ctrlFullSeqGrid.SetCellAlignment(flexAlignLeftCenter);
//				m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 5, pStep->m_strNextSrc);
				
				
				if(pStep->GetFuncType() == MEAS_BOOL)
				{						
					if(pStep->GetNominal_Bool() == FALSE) sTmp.Format(_T("%s"), "FALSE");
					else sTmp.Format(_T("%s"), "TRUE");
				}
				else if(pStep->GetFuncType() == MEAS_DOUBLE)
				{
					sTmp.Format("%4.1f  ", pStep->GetNominal_float());
				}

				m_ctrlFullSeqGrid.SetCol(5);
				m_ctrlFullSeqGrid.SetColDataType(5, flexDTString);
				m_ctrlFullSeqGrid.SetCellAlignment(flexAlignCenterCenter);
				m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 5, sTmp);

				//Lower Limit, Upper Limit
				if(pStep->GetFuncType() != MEAS_BOOL)
				{
					//Lower Limit
					sTmp.Format("%4.1f", pStep->GetLowLimit());
					m_ctrlFullSeqGrid.SetCol(6);
					m_ctrlFullSeqGrid.SetColDataType(6, flexDTString);
					m_ctrlFullSeqGrid.SetCellAlignment(flexAlignCenterCenter);
					m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 6, sTmp);
					//High Limit
					sTmp.Format("%4.1f", pStep->GetHighLimit());
					m_ctrlFullSeqGrid.SetCol(7);
					m_ctrlFullSeqGrid.SetColDataType(7, flexDTString);
					m_ctrlFullSeqGrid.SetCellAlignment(flexAlignCenterCenter);
					m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 7, sTmp);
					//Unit
					sTmp.Format(_T("%s"), pStep->GetUnit());
					m_ctrlFullSeqGrid.SetCol(8);
					m_ctrlFullSeqGrid.SetColDataType(8, flexDTString);
					m_ctrlFullSeqGrid.SetCellAlignment(flexAlignCenterCenter);
					m_ctrlFullSeqGrid.SetTextMatrix(nTmp , 8, sTmp);
				}
			}
			nTmp++;
		}
	}

	m_ctrlFullSeqGrid.SetRow(0);
	m_ctrlFullSeqGrid.SetCol(0);
	m_ctrlFullSeqGrid.SetRedraw(-1);
	m_ctrlFullSeqGrid.Refresh();
}

void CFullSeqDlg::ChangeRowColor(int nRow)
{
//	int nTotalRow,  nTotalCol;

	if(nRow > 0)
	{
		//Repaint All Main Grid
//		for(nTotalRow = 1; nTotalRow < m_ctrlFullSeqGrid.GetRows(); nTotalRow++)
//		{
//			m_ctrlFullSeqGrid.Select(1, 0, COleVariant(long(m_ctrlFullSeqGrid.GetRows() - 1)), COleVariant(long(m_ctrlFullSeqGrid.GetCols() - 1)));
//			m_ctrlFullSeqGrid.SetRow(nTotalRow);
//			for(nTotalCol = 0; nTotalCol < m_ctrlFullSeqGrid.GetCols(); nTotalCol++)
//			{
//				m_ctrlFullSeqGrid.SetCol(nTotalCol);
//				m_ctrlFullSeqGrid.SetCellBackColor(RGB(247,247,231));
//			}
//		}
//		m_ctrlFullSeqGrid.SetRow(nRow);
//		for(nTotalCol = 0; nTotalCol < m_ctrlFullSeqGrid.GetCols(); nTotalCol++)
//		{
//			m_ctrlFullSeqGrid.SetCol(nTotalCol);
			m_ctrlFullSeqGrid.Select(nRow, 0, COleVariant(long(nRow)), COleVariant(long(8)));
//			m_ctrlFullSeqGrid.SetCellBackColor(RGB(128,182,0));
//		}
//		m_ctrlFullSeqGrid.SetCol(-1);
//		m_ctrlFullSeqGrid.SetRow(-1);
	}
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CFullSeqDlg message handlers

BOOL CFullSeqDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nSelRow = -1;

	InitFullSeqGrid();

	if(!CurrentSet->sFullSeqPath.IsEmpty())
	{
		m_ctrlFullSeqPathEdit.SetWindowText(CurrentSet->sFullSeqPath);
		UpdateFullSeqGrid();
	}
	else
	{
		m_ctrlFullSeqPathEdit.SetWindowText(_T(""));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CFullSeqDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFullSeqDlg)
	ON_EVENT(CFullSeqDlg, IDC_FULL_SEQ_GRID, -600 /* Click */, OnClickFullSeqGrid, VTS_NONE)
	ON_EVENT(CFullSeqDlg, IDC_FULL_SEQ_GRID, -601 /* DblClick */, OnDblClickFullSeqGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CFullSeqDlg::OnClickFullSeqGrid() 
{
	m_nSelRow = m_ctrlFullSeqGrid.GetRow();
	if((m_nSelRow < 1)) return;
	ChangeRowColor(m_nSelRow);
}

void CFullSeqDlg::OnDblClickFullSeqGrid() 
{
	m_nSelRow = m_ctrlFullSeqGrid.GetRow();
	if((m_nSelRow < 1)) return;
	ChangeRowColor(m_nSelRow);

	//+del kwmoon 080508
//	OnOK();

	//+add kwmoon 080508
	m_pParentDlg->InsertFromFullseq(m_nSelRow);
}

void CFullSeqDlg::OnOK() 
{
	CDialog::OnOK();
}

void CFullSeqDlg::OnCancel() 
{
	CDialog::OnCancel();
}

//+add kwmoon 080508
void CFullSeqDlg::SetParentDlg(CSeqEditorDlg* pParentDlg)
{
	m_pParentDlg = pParentDlg;
}

//+add kwmoon 080508
void CFullSeqDlg::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	OnDblClickFullSeqGrid();	
}
