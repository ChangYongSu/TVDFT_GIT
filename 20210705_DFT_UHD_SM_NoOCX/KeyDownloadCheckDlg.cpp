// KeyDownloadCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "KeyDownloadCheckDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView * g_pView;

/////////////////////////////////////////////////////////////////////////////
// CKeyDownloadCheckDlg dialog


CKeyDownloadCheckDlg::CKeyDownloadCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyDownloadCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyDownloadCheckDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKeyDownloadCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyDownloadCheckDlg)
	DDX_Control(pDX, IDC_KEYCHECK_GRID, m_ctrlKeyCheckGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyDownloadCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyDownloadCheckDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyDownloadCheckDlg message handlers

BOOL CKeyDownloadCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetWindowRect(&m_dlgRect);
	
	InitKeyCheckGrid();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CKeyDownloadCheckDlg::InitKeyCheckGrid()
{
	CString sTmp;
	int nKeyItem;
	int nRow;

	char *sHeader1[] = {"Option", "MAC_ADDRESS", "ESN", "WIDEVINE", "HDCP_RX_1.x", "HDCP_RX_2.x", "Google TV", "MARINKEY", "CI", "CNTV", "DTCP_RX"  , "ATSC_3.0" , "MFI", "CI+ECP" };
	char *sHeader2[] = {"KEY D/L List", "Status", "Check Result"};


	m_ctrlKeyCheckGrid.SetRedraw(FALSE);
	m_ctrlKeyCheckGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlKeyCheckGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlKeyCheckGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlKeyCheckGrid.SetBackColor(RGB(247,247,231));
	m_ctrlKeyCheckGrid.SetGridColor(RGB(200,200,176));

	m_ctrlKeyCheckGrid.SetFixedRows(1);
	m_ctrlKeyCheckGrid.SetFixedCols(1);

	m_ctrlKeyCheckGrid.SetFillStyle(flexFillRepeat);
	m_ctrlKeyCheckGrid.SetFontName("Arial");
	m_ctrlKeyCheckGrid.SetFontSize(10);

	m_ctrlKeyCheckGrid.SetCols(3);
	m_ctrlKeyCheckGrid.SetRows(15);
	m_ctrlKeyCheckGrid.SetExtendLastCol(TRUE);

	m_ctrlKeyCheckGrid.SetColWidth(0, 2000); //500
	m_ctrlKeyCheckGrid.SetColWidth(1, 800); //500
	m_ctrlKeyCheckGrid.SetColWidth(1, 800); //500
	m_ctrlKeyCheckGrid.SetFixedAlignment(0,flexAlignCenterCenter);
	m_ctrlKeyCheckGrid.SetColAlignment(1,flexAlignCenterCenter);		
	m_ctrlKeyCheckGrid.SetColAlignment(2,flexAlignCenterCenter);		

	m_ctrlKeyCheckGrid.SetRowHeight(0, 300);

	m_ctrlKeyCheckGrid.SetTextMatrix(0, 0, "KEY ITEM");
	m_ctrlKeyCheckGrid.SetTextMatrix(0, 1, "STATUS");
	m_ctrlKeyCheckGrid.SetTextMatrix(0, 2, "CHECK RESULT");

	m_ctrlKeyCheckGrid.SetCol(0);
	for(nRow = 0; nRow < 14 ; nRow++)
	{
		m_ctrlKeyCheckGrid.SetRow(nRow + 1);
	
		m_ctrlKeyCheckGrid.SetRowHeight(nRow + 1, 400);
		m_ctrlKeyCheckGrid.SetText(sHeader1[nRow]);
	}

	if(!CurrentSet->bDrmKey){
		m_ctrlKeyCheckGrid.SetTextMatrix(1, 1, "YES");
		for(nRow = 2; nRow < 14 ; nRow++)
		{
			m_ctrlKeyCheckGrid.SetTextMatrix(nRow, 1, "NO");
		}
	}
	else{
		//nKeyItem = CurrentSet->nDrmKeyVal;
		nKeyItem = CheckKeyGmesVal();
		for(nRow = 0; nRow < 14 ; nRow++)
		{
			if(nKeyItem & 0x01)
			{
				m_ctrlKeyCheckGrid.SetTextMatrix(nRow + 1, 1, "YES");
			}
			else{
				m_ctrlKeyCheckGrid.SetTextMatrix(nRow + 1, 1, "NO");
			}
			nKeyItem = nKeyItem >> 1;
		}
	}

	m_ctrlKeyCheckGrid.SetRedraw(TRUE);
	m_ctrlKeyCheckGrid.Refresh();

}
void CKeyDownloadCheckDlg::UpdateResult(int nIndex, int nResult)
{
	CString sTmp;

	m_ctrlKeyCheckGrid.SetRow(nIndex);
	m_ctrlKeyCheckGrid.SetCol(2);
	m_ctrlKeyCheckGrid.SetColDataType(2, flexDTString);

	switch(nResult){
		case 0:
			sTmp = _T("NG");
			m_ctrlKeyCheckGrid.SetCellBackColor(RGB(255,0,0));
			m_ctrlKeyCheckGrid.SetCellForeColor(RGB(255,255,255));
			m_ctrlKeyCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			break;

		case 1:
			sTmp = _T("OK");
			m_ctrlKeyCheckGrid.SetCellBackColor(RGB(0,128,64));
			m_ctrlKeyCheckGrid.SetCellForeColor(RGB(255,255,255));
			m_ctrlKeyCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			g_pView->m_bKeyDLCheckResultList[nIndex] = 1;
			break;
		case 2:
			sTmp = _T("SKIP");
			m_ctrlKeyCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			break;
		default:
			sTmp = _T("SKIP");
			m_ctrlKeyCheckGrid.SetTextMatrix(nIndex, 2, sTmp);
			break;

	}

	m_ctrlKeyCheckGrid.Refresh();
}
void CKeyDownloadCheckDlg::SaveResult2File()
{
	CString sFolderPath = _T("");
	sFolderPath  = CurrentSet->sResultFolder+ "_KEY.html";  

	SaveGridToHTML(&m_ctrlKeyCheckGrid, (char*)(LPCTSTR)sFolderPath);
}