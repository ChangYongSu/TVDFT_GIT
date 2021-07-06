// SignalTestResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SignalTestResultDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NO_ROW		7
#define NO_COL		13

/////////////////////////////////////////////////////////////////////////////
// CSignalTestResultDlg dialog


CSignalTestResultDlg::CSignalTestResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignalTestResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSignalTestResultDlg)
	//}}AFX_DATA_INIT

	m_pParentWnd = (CReviewPage*)pParent;

	m_pRefNoHighBit			= NULL;
	m_pGrabNoHighBit		= NULL;
	
}


void CSignalTestResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSignalTestResultDlg)
	DDX_Control(pDX, IDC_EDIT_NO_MASKED_PIXEL, m_ctrlNoMaskedPixel);
	DDX_Control(pDX, IDC_EDIT_NO_PIXEL, m_ctrlNoPixel);
	DDX_Control(pDX, IDC_NO_HIGH_BIT_GRID, m_ctrlNoHighBitGrid);
	DDX_Control(pDX, IDC_LABEL_TAB_TITLE1, m_ctrlTabTitle1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSignalTestResultDlg, CDialog)
	//{{AFX_MSG_MAP(CSignalTestResultDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignalTestResultDlg message handlers

BOOL CSignalTestResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	GetWindowRect(&m_dlgRect);

	m_ctrlTabTitle1.SetCaption("No of Low/High Bits");
	//

	CString szString;
	
	szString.Format("%d",m_nTotalNoUnmaskedPixel);
	m_ctrlNoPixel.SetWindowText(szString);

	szString.Format("%d",m_nTotalNoMaskedPixel);
	m_ctrlNoMaskedPixel.SetWindowText(szString);

	InitGrid();

	UpdateGrid();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSignalTestResultDlg::InitGrid()
{
	int nColumn = 1;

	CString szHeader = _T("");

	m_ctrlNoHighBitGrid.SetRedraw(FALSE);
	m_ctrlNoHighBitGrid.SetAllowBigSelection(FALSE);


	// Set Rows & Cols
	m_ctrlNoHighBitGrid.SetCols(NO_COL);
	m_ctrlNoHighBitGrid.SetRows(NO_ROW);
	


	// Set Fixed Row & Col
	m_ctrlNoHighBitGrid.SetFixedRows(1);
	m_ctrlNoHighBitGrid.SetFixedCols(1);



	// Set Grid Line Type
	m_ctrlNoHighBitGrid.SetGridLines(flexGridFlat);
	m_ctrlNoHighBitGrid.SetCellAlignment(flexAlignCenterCenter);
	


	// Set Row Title
	m_ctrlNoHighBitGrid.SetTextMatrix(1, 0, "REF-R");
	m_ctrlNoHighBitGrid.SetTextMatrix(2, 0, "GRAB-R");
	m_ctrlNoHighBitGrid.SetTextMatrix(3, 0, "REF-G");
	m_ctrlNoHighBitGrid.SetTextMatrix(4, 0, "GRAB-G");
	m_ctrlNoHighBitGrid.SetTextMatrix(5, 0, "REF-B");
	m_ctrlNoHighBitGrid.SetTextMatrix(6, 0, "GRAB-B");

	// Set Column Title
	for(int i=0; i<(NO_COL-1); i++)
	{
		szHeader.Format("%d",i);

		m_ctrlNoHighBitGrid.SetTextMatrix(0, i+1, szHeader);
		m_ctrlNoHighBitGrid.SetColWidth(i+1, 1800);
	}
	// Refresh
	m_ctrlNoHighBitGrid.Refresh();


}

void CSignalTestResultDlg::UpdateGrid()
{
	CString sTmp	 = _T("");
	CString szHeader = _T("");

	m_ctrlNoHighBitGrid.SetRedraw(FALSE);
	m_ctrlNoHighBitGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	CString sNoRefHighBit = _T("");
	CString sNoGrabHighBit = _T("");

	CString szString;
	
	szString.Format("%d",m_nTotalNoUnmaskedPixel);
	m_ctrlNoPixel.SetWindowText(szString);

	szString.Format("%d",m_nTotalNoMaskedPixel);
	m_ctrlNoMaskedPixel.SetWindowText(szString);

	int nNoRefHighBit	= 0;
	int nNoGrabHighBit	= 0;

	for(int i = 2; i>=0; i--)
	{
		for(int j = 0; j<(NO_COL-1); j++)
		{
			nNoRefHighBit  = *(m_pRefNoHighBit+(i*(NO_COL-1)+j));
			nNoGrabHighBit = *(m_pGrabNoHighBit+(i*(NO_COL-1)+j));

			// High Bit
			if(j < (m_nBitCount/3))
			{
				sNoRefHighBit.Format("%8d_%8d",m_nTotalNoMaskedPixel-nNoRefHighBit,nNoRefHighBit);
				sNoGrabHighBit.Format("%8d_%8d",m_nTotalNoMaskedPixel-nNoGrabHighBit,nNoGrabHighBit);

				m_ctrlNoHighBitGrid.SetTextMatrix((3-i)*2-1, j+1, sNoRefHighBit);
				m_ctrlNoHighBitGrid.SetTextMatrix((3-i)*2, j+1, sNoGrabHighBit);
			}
			else
			{
				m_ctrlNoHighBitGrid.SetTextMatrix((3-i)*2-1, j+1, "-");
				m_ctrlNoHighBitGrid.SetTextMatrix((3-i)*2, j+1, "-");
			}
		}
	}

	m_ctrlNoHighBitGrid.SetRedraw(TRUE);
	m_ctrlNoHighBitGrid.Refresh();
}

void CSignalTestResultDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CSignalTestResultDlg::SetParentWnd(CReviewPage *pWnd)
{
	m_pParentWnd = pWnd;
}

void CSignalTestResultDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pParentWnd->m_pSignalTestResultDlg = NULL;
	
	CDialog::OnClose();
}