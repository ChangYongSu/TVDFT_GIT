// NoDifferentPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "NoDifferentPointDlg.h"
#include "Global.h"
#include "ReviewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NO_ROW	4
#define NO_COL	21
/////////////////////////////////////////////////////////////////////////////
// CNoDifferentPointDlg dialog


CNoDifferentPointDlg::CNoDifferentPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoDifferentPointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoDifferentPointDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pParentWnd = (CReviewPage*)pParent;

	for(int i=0; i<3; i++)
		for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++) m_aNoDifferentPoint[i][j] = 0;

}


void CNoDifferentPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoDifferentPointDlg)
	DDX_Control(pDX, IDC_COMBO_STEP, m_ctrlCmbStep);
	DDX_Control(pDX, IDC_GRID, m_ctrlGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNoDifferentPointDlg, CDialog)
	//{{AFX_MSG_MAP(CNoDifferentPointDlg)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_STEP, OnSelchangeComboStep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoDifferentPointDlg message handlers

BOOL CNoDifferentPointDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetWindowRect(&m_dlgRect);
	
	InitGrid();
	UpdateGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNoDifferentPointDlg::InitGrid()
{
	int nColumn = 1;

	CString szHeader = _T("");

	m_ctrlGrid.SetRedraw(FALSE);
	m_ctrlGrid.SetAllowBigSelection(FALSE);

	//Rows & Cols Number
	m_ctrlGrid.SetCols(NO_DIFFERENT_POINT_BLOCK+1);
	m_ctrlGrid.SetRows(NO_ROW);
	
	//Fixed Rows & Cols
	m_ctrlGrid.SetFixedRows(1);
	m_ctrlGrid.SetFixedCols(1);

	//Grid Line Setting
	m_ctrlGrid.SetGridLines(flexGridFlat);
	
	//Header
	m_ctrlGrid.SetTextMatrix(0, 0, "NO");
	m_ctrlGrid.SetTextMatrix(1, 0, "R");
	m_ctrlGrid.SetTextMatrix(2, 0, "G");
	m_ctrlGrid.SetTextMatrix(3, 0, "B");

	m_ctrlCmbStep.AddString("1%");
	m_ctrlCmbStep.AddString("5%");
	m_ctrlCmbStep.AddString("10%");
	m_ctrlCmbStep.SetCurSel(0);

	for(int i=0,j=0; i<NO_DIFFERENT_POINT_BLOCK; i++,j++)
	{
		szHeader.Format("%d%%",i);
		m_ctrlGrid.SetTextMatrix(0, j+1, szHeader);
		m_ctrlGrid.SetColWidth(i+1, 1000);
	}
	
	m_ctrlGrid.SetColWidth(0, 400);
	m_ctrlGrid.SetCellAlignment(flexAlignCenterCenter);
	m_ctrlGrid.SetRedraw(TRUE);

	m_ctrlGrid.Refresh();
}

void CNoDifferentPointDlg::UpdateGrid()
{
	CString sTmp	 = _T("");
	CString szHeader = _T("");

	int nPartialSum = 0;
	int nStepType	= 0;
	int	nIncrement	= 0;
	int nNoColumn	= 0;

	m_ctrlGrid.SetRedraw(FALSE);
	m_ctrlGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	nNoColumn = m_ctrlGrid.GetCols();

	// Clear Header
	for(int k=1; k<nNoColumn;k++)
	{
		szHeader.Format("");
		m_ctrlGrid.SetTextMatrix(0, k, szHeader);
	}
	
	nStepType = m_ctrlCmbStep.GetCurSel();
	
	switch(nStepType)
	{
		case 0 : // by 1%
		{
			nIncrement = 1;
			nNoColumn  = NO_DIFFERENT_POINT_BLOCK / nIncrement;

			m_ctrlGrid.SetCols(nNoColumn+1);
		}
		break;

		case 1 : // by 5%
		{
			nIncrement = 5;
			nNoColumn  = NO_DIFFERENT_POINT_BLOCK / nIncrement;

			m_ctrlGrid.SetCols(nNoColumn+1);
		}
		break;

		case 2 : // by 10%
		{
			nIncrement = 10;
			nNoColumn  = NO_DIFFERENT_POINT_BLOCK / nIncrement;

			m_ctrlGrid.SetCols(nNoColumn+1);
		}
		break;

		default : // by 1%
		{
			nIncrement = 1;
			nNoColumn  = NO_DIFFERENT_POINT_BLOCK / nIncrement;

			m_ctrlGrid.SetCols(nNoColumn+1);
		}
		break;
	}
/*
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<NO_DIFFERENT_POINT_BLOCK; j++)
		{
			sTmp.Format("%d", m_aNoDifferentPoint[i][j]);
			m_ctrlGrid.SetTextMatrix(3-i, j+1, sTmp);
		}
	}
*/

	
	// Set Header 
	for(int i=0,j=0; i<NO_DIFFERENT_POINT_BLOCK; i=i+nIncrement,j++)
	{
		szHeader.Format("%d%%",i);
		m_ctrlGrid.SetTextMatrix(0, j+1, szHeader);
	}
	
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<nNoColumn; j++)
		{
			nPartialSum = 0;

			for(int k = 0; k<nIncrement; k++)
			{
				nPartialSum += m_aNoDifferentPoint[i][j*(nIncrement)+k];
			}

			sTmp.Format("%d", nPartialSum);
			m_ctrlGrid.SetTextMatrix(3-i, j+1, sTmp);
		}
	}

	m_ctrlGrid.SetRedraw(TRUE);
	m_ctrlGrid.Refresh();
}

void CNoDifferentPointDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CNoDifferentPointDlg::SetParentWnd(CReviewPage *pWnd)
{
	m_pParentWnd = pWnd;
}

void CNoDifferentPointDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pParentWnd->m_pNoDifferentPointDlg = NULL;

	CDialog::OnClose();
}

void CNoDifferentPointDlg::OnSelchangeComboStep() 
{
	// TODO: Add your control notification handler code here
	
	UpdateGrid();
}
