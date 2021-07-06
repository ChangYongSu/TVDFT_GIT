// SeqEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SeqEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
extern _MeasureFunction		MeasDefined[];
extern _InternalFunction	Predefined[];
extern _InputSource			InputDefined[];

#define STEPTYPE_CMB	0
#define TEST_CMB		1
#define CURSRC_CMB		2
#define TARGET_CMB		3
#define UNIT_CMB		4

//+change kwmoon 080724
//#define MAINSTEP_MAX_COL		12
#define MAINSTEP_MAX_COL		13

#define SUBSTEP_MAX_COL			11


#include "SeqRoiDlg.h"
#include "SeqRemoteDlg.h"

#include "FullSeqDlg.h"

#include "SetAudioSpecDlg.h"
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CSeqEditorDlg dialog


CSeqEditorDlg::CSeqEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeqEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeqEditorDlg)
	//}}AFX_DATA_INIT
}


void CSeqEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeqEditorDlg)
	DDX_Control(pDX, IDC_TEMPLATE_SEQ_PATH_EDIT, m_ctrlTemplatePathEdit);
	DDX_Control(pDX, IDC_SEQ_PATH_EDIT, m_ctrlSeqPathEdit);
	//	DDX_Control(pDX, IDC_MAIN_STEP_GRID, m_ctrlMainGrid);
	//DDX_Control(pDX, IDC_SUB_STEP_GRID, m_ctrlSubGrid);
	//DDX_Control(pDX, IDC_TEMPLATE_SEQ_GRID, m_ctrlTemplateGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_MAIN_STEP_LIST, m_ctrlStepMainList);
	DDX_Control(pDX, IDC_TEMPLATE_SEQ_LIST, m_ctrlTemplateList);
	DDX_Control(pDX, IDC_SUB_STEP_LIST, m_ctrlSubList);
	DDX_Control(pDX, IDC_COMBO_SEQ_EDITOR, m_cComboSeqEditor);
	DDX_Control(pDX, IDC_COMBO_SEQ_SUB_EDITOR, m_cComboSeqSubEditor);
}


BEGIN_MESSAGE_MAP(CSeqEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CSeqEditorDlg)
	ON_BN_CLICKED(IDC_BTN_NEW_SEQ, OnBtnNewSeq)
	ON_BN_CLICKED(IDC_BTN_OPEN_SEQ, OnBtnOpenSeq)
	ON_BN_CLICKED(IDC_BTN_SAVE_SEQ, OnBtnSaveSeq)
	ON_BN_CLICKED(IDC_BTN_SAVE_AS_SEQ, OnBtnSaveAsSeq)
	ON_BN_CLICKED(IDC_BTN_INSERT_MAIN_STEP, OnBtnInsertMainStep)
	ON_BN_CLICKED(IDC_BTN_UP_MAIN_STEP, OnBtnUpMainStep)
	ON_BN_CLICKED(IDC_BTN_DOWN_MAIN_STEP, OnBtnDownMainStep)
	ON_BN_CLICKED(IDC_BTN_DELETE_MAIN_STEP, OnBtnDeleteMainStep)
	ON_BN_CLICKED(IDC_BTN_CLEAR_MAIN_STEP, OnBtnClearMainStep)
	ON_BN_CLICKED(IDC_BTN_INSERT_SUB_STEP, OnBtnInsertSubStep)
	ON_BN_CLICKED(IDC_BTN_UP_SUB_STEP, OnBtnUpSubStep)
	ON_BN_CLICKED(IDC_BTN_DOWN_SUB_STEP, OnBtnDownSubStep)
	ON_BN_CLICKED(IDC_BTN_DELETE_SUB_STEP, OnBtnDeleteSubStep)
	ON_BN_CLICKED(IDC_BTN_CLEAR_SUB_STEP, OnBtnClearSubStep)
	ON_BN_CLICKED(IDC_BTN_UPLOAD_SUB_STEP, OnBtnUploadSubStep)
	ON_BN_CLICKED(IDC_BTN_COPY_MAIN_STEP, OnBtnCopyMainStep)
	ON_BN_CLICKED(IDC_BTN_COPY_SUB_STEP, OnBtnCopySubStep)
	ON_BN_CLICKED(IDC_BTN_INSERT_FROM_FULLSEQ, OnBtnInsertFromFullseq)
	ON_BN_CLICKED(IDC_BTN_ADD_ALL, OnBtnAddAll)
	ON_BN_CLICKED(IDC_BTN_BROWSE_SEQ_FILE, OnBtnBrowseSeqFile)
	ON_BN_CLICKED(IDC_BTN_BROWSE_TEMPLATE_FILE, OnBtnBrowseTemplateFile)
	ON_BN_CLICKED(IDC_BTN_SELECT_ALL, OnBtnSelectAll)
	ON_BN_CLICKED(IDC_BTN_DESELECT_ALL, OnBtnDeselectAll)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAIN_STEP_LIST, &CSeqEditorDlg::OnNMClickListMainStepList)
	ON_NOTIFY(NM_CLICK, IDC_SUB_STEP_LIST, &CSeqEditorDlg::OnNMClickSubStepList)
	ON_NOTIFY(NM_DBLCLK, IDC_SUB_STEP_LIST, &CSeqEditorDlg::OnNMDblclkSubStepList)
	ON_NOTIFY(NM_CLICK, IDC_TEMPLATE_SEQ_LIST, &CSeqEditorDlg::OnNMClickTemplateSeqList)
	ON_NOTIFY(NM_DBLCLK, IDC_TEMPLATE_SEQ_LIST, &CSeqEditorDlg::OnNMDblclkTemplateSeqList)
	ON_EN_CHANGE(IDC_EDIT_SEQ_EDITOR, &CSeqEditorDlg::OnEnChangeEditSeqEditor)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_SEQ_EDITOR, &CSeqEditorDlg::OnCbnSelchangeComboSeqEditor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CSeqEditorDlg::InitMainGrid()
{
	CString sTmp;

	//+change kwmoon 080724
//	char *sHeader[] = {"NO","STEP NAME", "STEP TYPE", "TEST", "SOURCE", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT", "A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "A-MARGIN"};
	char *sHeader[] = {"NO","SOURCE","STEP NAME", "AUDIO","TYPE", "TEST", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT", "A-SOURCE", "AUD(FREQ,LVL)","A-MARGIN"};
//	char *sHeader[] = {"NO","STEP NAME", "STEP TYPE", "TEST", "SOURCE", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT"};
	int nWidth[] = {  40, 70, 160, 55, 70, 130, 90, 80, 80, 60, 80, 220, 120 };
	//-

	//m_ctrlMainGrid.SetRedraw(flexRDNone);
	//m_ctrlMainGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	//m_ctrlMainGrid.SetBackColorFixed(RGB(210,216,176));
	//m_ctrlMainGrid.SetForeColorFixed(RGB(51,102,153));

	////+2008.02.12 Mod BY USY
	//m_ctrlMainGrid.SetBackColorBkg(RGB(255,255,255));
	//m_ctrlMainGrid.SetBackColor(RGB(255,255,255));

	//
	////m_ctrlMainGrid.SetBackColorBkg(RGB(247,247,231));
	////m_ctrlMainGrid.SetBackColor(RGB(247,247,231));
	////-

	//m_ctrlMainGrid.SetGridColor(RGB(210,216,176));

	//m_ctrlMainGrid.SetAllowSelection(TRUE);

	//m_ctrlMainGrid.SetFixedRows(1);
	//m_ctrlMainGrid.SetFixedCols(0);
	//m_ctrlMainGrid.SetFrozenCols(3);
	//

	//m_ctrlMainGrid.SetCols(MAINSTEP_MAX_COL);
	//m_ctrlMainGrid.SetRows(10);
	//m_ctrlMainGrid.SetRow(0);

	m_ctrlStepMainList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
//	m_ctrlSpecAdjList.InsertColumn(0, "Source", LVCFMT_CENTER, 80);
//	m_ctrlSpecAdjList.InsertColumn(1, "Margin", LVCFMT_CENTER, 80);

	//========
	// Header
	//========
	for(UINT nTmp = 0; nTmp < MAINSTEP_MAX_COL ; nTmp++)
	{
		m_ctrlStepMainList.InsertColumn(nTmp, sHeader[nTmp], LVCFMT_CENTER, nWidth[nTmp]);//m_ctrlMainGrid.SetCol(nTmp);

		//m_ctrlMainGrid.SetFixedAlignment(nTmp, flexAlignCenterCenter);
		//m_ctrlMainGrid.SetFillStyle(flexFillRepeat);
		//m_ctrlMainGrid.SetFontName("Arial");
		//m_ctrlMainGrid.SetFontSize(10);
		//m_ctrlMainGrid.SetExtendLastCol(FALSE);
		//m_ctrlMainGrid.SetText(sHeader[nTmp]);
		//m_ctrlMainGrid.SetEditable(flexEDNone);
		
		//if(nTmp == 0) m_ctrlMainGrid.SetColWidth(nTmp, 700);		// NO
		//else if(nTmp == 1) m_ctrlMainGrid.SetColWidth(nTmp, 1100);	// Source
		//else if(nTmp == 2) m_ctrlMainGrid.SetColWidth(nTmp, 2400);	// Step Name
		//else if(nTmp == 3) m_ctrlMainGrid.SetColWidth(nTmp, 800);	// U=Limit
		//else if(nTmp == 4) m_ctrlMainGrid.SetColWidth(nTmp, 1200);	// Step Type
		//else if(nTmp == 5) m_ctrlMainGrid.SetColWidth(nTmp, 1500);	// Test
		//else if(nTmp == 6) m_ctrlMainGrid.SetColWidth(nTmp, 900);	// Target
		//else if(nTmp == 7) m_ctrlMainGrid.SetColWidth(nTmp, 800);	// L-Limit
		//else if(nTmp == 8) m_ctrlMainGrid.SetColWidth(nTmp, 800);	// U=Limit
		//else if(nTmp == 9) m_ctrlMainGrid.SetColWidth(nTmp, 600);	// Unit
		//else if(nTmp == 10) m_ctrlMainGrid.SetColWidth(nTmp, 1300);	// A-Source
		//else if(nTmp == 11) m_ctrlMainGrid.SetColWidth(nTmp, 2200);	// A-Target
		//else if(nTmp == 12) m_ctrlMainGrid.SetColWidth(nTmp, 1200);	// A-Margin
	}
	//m_ctrlMainGrid.SetRow(0);
	//m_ctrlMainGrid.SetCol(0);
	//m_ctrlMainGrid.SetRedraw(-1);
	//m_ctrlMainGrid.Refresh();
	m_ctrlStepMainList.Invalidate();
}

void CSeqEditorDlg::UpdateMainGrid()
{
	//m_ctrlMainGrid.SetRedraw(flexRDNone);
	//m_ctrlMainGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearText	))); 

	//m_ctrlMainGrid.SetRows(StepList.GetCount() + 1);
	m_ctrlStepMainList.DeleteAllItems();
	int nTmp = 0;
	
	CString sTmp;
	POSITION Position = StepList.GetHeadPosition();

	if(StepList.GetCount() > 0)
	{
		nTmp = 0;

		while (Position != NULL) 
		{
			CStep* pStep = StepList.GetNext(Position);

		//	m_ctrlMainGrid.SetRow(nTmp);

			//===========
			// 1.Step No
			//===========
			sTmp.Format("%d", pStep->GetStepNo());
	/*		m_ctrlMainGrid.SetCol(0);
			m_ctrlMainGrid.SetColDataType(0,flexDTString);
			m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);*/
			//m_ctrlMainGrid.SetTextMatrix(nTmp,0,sTmp);
			m_ctrlStepMainList.InsertItem(nTmp, sTmp);   // // m_ctrlSpecAdjList.InsertColumn(1, "Margin", LVCFMT_CENTER, 100); //m_ctrlSpecAdjGrid.SetTextMatrix(i + 1,0, InputDefined[i].pszName );
		//m_ctrlStepMainList.SetItem(i, 1, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); 
		
			//==========
			// 2.Source
			//==========
			//if(pStep->m_nStepType == CHECK_FUNCTION)
			//{
			//	m_ctrlMainGrid.SetCol(1);
			//	m_ctrlMainGrid.SetColDataType(1,flexDTString);
			//	m_ctrlMainGrid.SetCellAlignment(flexAlignLeftCenter);
			//	m_ctrlMainGrid.SetTextMatrix(nTmp,1,pStep->GetCurrentSource());
			//
			//	ChangeColumnColor(MAIN_GRID,pStep->GetCurrentSource());
			//}
			m_ctrlStepMainList.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetCurrentSource(), NULL, NULL, NULL, NULL); // m_ctrlMainGrid.SetTextMatrix(nTmp,2,pStep->GetStepName());


			//=============
			// 3.Step Name
			//=============
	/*		m_ctrlMainGrid.SetCol(2);
			m_ctrlMainGrid.SetColDataType(2,flexDTString);
			m_ctrlMainGrid.SetCellAlignment(flexAlignLeftCenter);*/
			m_ctrlStepMainList.SetItem(nTmp, 2,  LVIF_TEXT, pStep->GetStepName(), NULL, NULL, NULL, NULL); // m_ctrlMainGrid.SetTextMatrix(nTmp,2,pStep->GetStepName());
			

			//==============
			// 4.Audio Test
			//==============
			//m_ctrlMainGrid.SetCol(3);
			//m_ctrlMainGrid.SetColDataType(3,flexDTString);
			//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
			
			if(pStep->m_bRunAudioTest)
			{
				m_ctrlStepMainList.SetItem(nTmp, 3, LVIF_TEXT, "O", NULL, NULL, NULL, NULL); // m_ctrlMainGrid.SetTextMatrix(nTmp,3,"O");
			}
			else
			{
				m_ctrlStepMainList.SetItem(nTmp, 3, LVIF_TEXT, "X", NULL, NULL, NULL, NULL); // m_ctrlMainGrid.SetTextMatrix(nTmp,3,"X");
			}


			//=============
			// 4.Step Type
			//=============
			if(pStep->m_nStepType == CHECK_FUNCTION)
			{
				sTmp.Format("CHECK");
			}
			else if(pStep->m_nStepType == PROCESS_FUNCTION)
			{
				sTmp.Format("PROCESS");
			}
			else
			{
				sTmp.Format(_T(""));
			}

			//m_ctrlMainGrid.SetCol(4);
			//m_ctrlMainGrid.SetColDataType(4,flexDTString);
			//m_ctrlMainGrid.SetCellAlignment(flexAlignLeftCenter);
			m_ctrlStepMainList.SetItem(nTmp, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,4,sTmp);
			
			if(pStep->m_nStepType == CHECK_FUNCTION)
			{
				//=================
				// 5.Function Name
				//=================
				//m_ctrlMainGrid.SetCol(5);
				//m_ctrlMainGrid.SetColDataType(5,flexDTString);
				//m_ctrlMainGrid.SetCellAlignment(flexAlignLeftCenter);
				m_ctrlStepMainList.SetItem(nTmp, 5, LVIF_TEXT, pStep->m_strFuncName, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp ,5,pStep->m_strFuncName);

				
				//================
				// 6.Target Value
				//================
				if(pStep->GetFuncType() == MEAS_BOOL)
				{						
					if(pStep->GetNominal_Bool() == FALSE)
					{
						sTmp.Format(_T("%s"),"FALSE");
					}
					else
					{
						sTmp.Format(_T("%s"),"TRUE");
					}
				}
				else if(pStep->GetFuncType() == MEAS_DOUBLE)
				{
					sTmp.Format("%4.1f",pStep->GetNominal_float());
				}


				//m_ctrlMainGrid.SetCol(6);
				//m_ctrlMainGrid.SetColDataType(6,flexDTString);
				//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
				m_ctrlStepMainList.SetItem(nTmp, 6, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,6,sTmp);

				if(pStep->GetFuncType() != MEAS_BOOL)
				{
					//===============
					// 7.Lower Limit
					//===============
					sTmp.Format("%4.1f", pStep->GetLowLimit());
	/*				m_ctrlMainGrid.SetCol(7);
					m_ctrlMainGrid.SetColDataType(7,flexDTString);
					m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);*/
					m_ctrlStepMainList.SetItem(nTmp, 7, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,7,sTmp);
					
					//===============
					// 8.Upper Limit
					//===============
					sTmp.Format("%4.1f", pStep->GetHighLimit());
					//m_ctrlMainGrid.SetCol(8);
					//m_ctrlMainGrid.SetColDataType(8,flexDTString);
					//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
					m_ctrlStepMainList.SetItem(nTmp, 8, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,8,sTmp);

					//========
					// 9.Unit
					//========
					sTmp.Format(_T("%s"), pStep->GetUnit());
					//m_ctrlMainGrid.SetCol(9);
					//m_ctrlMainGrid.SetColDataType(9,flexDTString);
					//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
					m_ctrlStepMainList.SetItem(nTmp, 9, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,9,sTmp);
				}

				if(pStep->m_bRunAudioTest)
				{
					//=================
					// 10.Audio Source 
					//=================
					sTmp.Format("%d", pStep->m_nAudioSource);
					//m_ctrlMainGrid.SetCol(10);
					//m_ctrlMainGrid.SetColDataType(10,flexDTString);
					//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
					m_ctrlStepMainList.SetItem(nTmp, 10, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,10,sTmp);

					//=========
					// 11.Audio 
					//=========
					if(pStep->m_nMeasureAudioType == LEVEL_CHECK) {
						sTmp.Format("%d, %d", pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
						//m_ctrlMainGrid.SetCol(11);
						//m_ctrlMainGrid.SetColDataType(11,flexDTString);
						//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
						m_ctrlStepMainList.SetItem(nTmp, 11, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,11,sTmp);
					}
					else if(pStep->m_nMeasureAudioType == FREQUENCY_CHECK) {
						sTmp.Format("%d, %d", pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
						//m_ctrlMainGrid.SetCol(11);
						//m_ctrlMainGrid.SetColDataType(11,flexDTString);
						//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
						m_ctrlStepMainList.SetItem(nTmp, 11, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,11,sTmp);
					}
					else {
						sTmp.Format("%d, %d, %d, %d", pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], 
									  pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
						//m_ctrlMainGrid.SetCol(11);
						//m_ctrlMainGrid.SetColDataType(11,flexDTString);
						//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
						m_ctrlStepMainList.SetItem(nTmp, 11, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,11,sTmp);
						
						//=================
						// 12.Audio Margin 
						//=================
						sTmp.Format("%d", pStep->m_nAudioMargin);
						//m_ctrlMainGrid.SetCol(12);
						//m_ctrlMainGrid.SetColDataType(12,flexDTString);
						//m_ctrlMainGrid.SetCellAlignment(flexAlignCenterCenter);
						m_ctrlStepMainList.SetItem(nTmp, 12, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlMainGrid.SetTextMatrix(nTmp,12,sTmp);
					}
				}

			}
			nTmp++;
		}
	}
	//m_ctrlMainGrid.SetRow(-1);
	//m_ctrlMainGrid.SetCol(-1);
	//m_ctrlMainGrid.SetRedraw(-1);
	//m_ctrlMainGrid.Refresh();
	m_ctrlStepMainList.Invalidate();
}

void CSeqEditorDlg::SetMainGrid4Edit(int nRow, int nCol)
{
//	m_ctrlMainGrid.SetRow(nRow);
//	m_ctrlMainGrid.SetCol(nCol);
//	m_ctrlMainGrid.SetEditable(flexEDNone);
//	m_ctrlMainGrid.SetColComboList(nCol, _T(""));
//	
	switch(nCol) 
	{
		//==========
		// 1.Source
		//==========
		case 1:
			if(CheckMainStepType(nRow) == PROCESS_FUNCTION)
			{
				//m_ctrlMainGrid.SetRow(nRow);
				//m_ctrlMainGrid.SetCol(nCol);
				//m_ctrlMainGrid.SetEditable(flexEDNone);
				//m_ctrlMainGrid.SetColComboList(nCol, _T(""));
				//GetDlgItem(IDC_EDIT_OPT_VERSION)->SetWindowPos(NULL, x + 120, y + nItem * 16 + 20, 250, 20, SWP_ASYNCWINDOWPOS);
				SetEditPos(nRow, nCol+500, 250, 20 );
				SetComboPos(nRow, nCol + 500, 250, 20,"");
			}
			else if(CheckMainStepType(nRow) == CHECK_FUNCTION)
			{
				//m_ctrlMainGrid.SetRow(nRow);
				//m_ctrlMainGrid.SetCol(nCol);
				//m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlMainGrid.SetColComboList(nCol, MakeMainStepComboList(CURSRC_CMB));
				//CpmboBox
				//m_cComboSeqEditor->SetWindowPos(NULL, nRow + 120, y + nItem * 16 + 20, 250, 20, SWP_ASYNCWINDOWPOS);
				//GetDlgItem(IDC_EDIT_OPT_VERSION)->SetWindowPos(NULL, x + 120, y + nItem * 16 + 20, 250, 20, SWP_ASYNCWINDOWPOS);
				SetEditPos(nRow, nCol + 500, 250, 20);
				SetComboPos(nRow, nCol, 250, 20, MakeMainStepComboList(CURSRC_CMB));
			}
			break;

		//=============
		// 2.Step Name
		//=============
		case 2:
			//m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
			//EditBox
			SetEditPos(nRow, nCol , 250, 20);
			SetComboPos(nRow, nCol + 500, 250, 20, "");
			break;

		//=============
		// 4.Step Type
		//=============
		case 4:
			//m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
			//m_ctrlMainGrid.SetColComboList(nCol, MakeMainStepComboList(STEPTYPE_CMB));	
			//ComboBox
			SetEditPos(nRow, nCol + 500, 250, 20);
			SetComboPos(nRow, nCol , 250, 20, MakeMainStepComboList(STEPTYPE_CMB));
			break;
		
		//=================
		// 5.Function Name
		//=================
		case 5:
			if(CheckMainStepType(nRow) == PROCESS_FUNCTION)
			{
			/*	m_ctrlMainGrid.SetRow(nRow);
				m_ctrlMainGrid.SetCol(nCol);
				m_ctrlMainGrid.SetEditable(flexEDNone);
				m_ctrlMainGrid.SetColComboList(nCol, _T(""));*/
				SetEditPos(nRow, nCol+250, 250, 20);
				SetComboPos(nRow, nCol + 500, 250, 20, "");
			}
			else if(CheckMainStepType(nRow) == CHECK_FUNCTION)
			{
	/*			m_ctrlMainGrid.SetRow(nRow);
				m_ctrlMainGrid.SetCol(nCol);
				m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
				m_ctrlMainGrid.SetColComboList(nCol, MakeMainStepComboList(TEST_CMB));*/
				//ComboBox
				SetEditPos(nRow, nCol + 250, 250, 20);
				SetComboPos(nRow, nCol, 250, 20, MakeMainStepComboList(TEST_CMB));
			}			
			break;

		//================
		// 6.Target Value
		//================
		case 6:
			if(CheckMainStepType(nRow) == PROCESS_FUNCTION)
			{
				//m_ctrlMainGrid.SetRow(nRow);
				//m_ctrlMainGrid.SetCol(nCol);
				//m_ctrlMainGrid.SetEditable(flexEDNone);
				SetEditPos(nRow, nCol + 250, 250, 20);
				SetComboPos(nRow, nCol + 500, 250, 20, "");
			}
			else if(CheckMainStepType(nRow) == CHECK_FUNCTION)
			{
				if(CheckMainTestType(nRow) == MEAS_BOOL)
				{
					//m_ctrlMainGrid.SetRow(nRow);
					//m_ctrlMainGrid.SetCol(nCol);
					//m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
					//m_ctrlMainGrid.SetColComboList(nCol, MakeMainStepComboList(TARGET_CMB));
					//ComboBox
					SetEditPos(nRow, nCol + 250, 250, 20);
					SetComboPos(nRow, nCol , 250, 20, MakeMainStepComboList(TARGET_CMB));
				}
				else if(CheckMainTestType(nRow) == MEAS_DOUBLE)
				{
//					m_ctrlMainGrid.SetRow(nRow);
//					m_ctrlMainGrid.SetCol(nCol);
////					m_ctrlMainGrid.SetTextMatrix(nRow, nCol, _T(""));
//					m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
//					m_ctrlMainGrid.SetColComboList(nCol, _T(""));
					//EditBox
					SetEditPos(nRow, nCol, 250, 20);
					SetComboPos(nRow, nCol + 500, 250, 20, "");
				}
			}
			break;
		//===============
		// 7.Lower Limit
		//===============
		case 7: 
		
		//===============
		// 8.Upper Limit
		//===============
		case 8: 
			if (CheckMainStepType(nRow) == PROCESS_FUNCTION)
			{
				//				m_ctrlMainGrid.SetRow(nRow);
				//				m_ctrlMainGrid.SetCol(nCol);
				////				m_ctrlMainGrid.SetTextMatrix(nRow, nCol, _T(""));
				//				m_ctrlMainGrid.SetEditable(flexEDNone);
				SetEditPos(nRow, nCol + 250, 250, 20);
				SetComboPos(nRow, nCol + 500, 250, 20, "");
			}
			else if (CheckMainStepType(nRow) == CHECK_FUNCTION)
			{
				if (CheckMainTestType(nRow) == MEAS_DOUBLE)
				{
					//					m_ctrlMainGrid.SetRow(nRow);
					//					m_ctrlMainGrid.SetCol(nCol);
					//					m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
					////					m_ctrlMainGrid.SetTextMatrix(nRow, nCol, _T(""));
					//					m_ctrlMainGrid.SetColComboList(nCol, MakeMainStepComboList(UNIT_CMB));
										//EditBox
										//ComboBox
					SetEditPos(nRow, nCol, 250, 20);
					SetComboPos(nRow, nCol + 500, 250, 20, "");
				}
			}
			break;
		//========
		// 9.Unit
		//========
		case 9:
			if(CheckMainStepType(nRow) == PROCESS_FUNCTION)
			{
//				m_ctrlMainGrid.SetRow(nRow);
//				m_ctrlMainGrid.SetCol(nCol);
////				m_ctrlMainGrid.SetTextMatrix(nRow, nCol, _T(""));
//				m_ctrlMainGrid.SetEditable(flexEDNone);
				SetEditPos(nRow, nCol + 250, 250, 20);
				SetComboPos(nRow, nCol + 500, 250, 20, "");
			}
			else if(CheckMainStepType(nRow) == CHECK_FUNCTION)
			{
				if(CheckMainTestType(nRow) == MEAS_BOOL)
				{
					//m_ctrlMainGrid.SetRow(nRow);
					//m_ctrlMainGrid.SetCol(nCol);
					//m_ctrlMainGrid.SetEditable(flexEDNone);
					//m_ctrlMainGrid.SetColComboList(nCol, _T(""));
					SetEditPos(nRow, nCol + 250, 250, 20);
					SetComboPos(nRow, nCol + 500, 250, 20, "");
				}
				else if(CheckMainTestType(nRow) == MEAS_DOUBLE)
				{
//					m_ctrlMainGrid.SetRow(nRow);
//					m_ctrlMainGrid.SetCol(nCol);
//					m_ctrlMainGrid.SetEditable(flexEDKbdMouse);
////					m_ctrlMainGrid.SetTextMatrix(nRow, nCol, _T(""));
//					m_ctrlMainGrid.SetColComboList(nCol, MakeMainStepComboList(UNIT_CMB));
					SetEditPos(nRow, nCol + 250, 250, 20);
					SetComboPos(nRow, nCol, 250, 20, MakeMainStepComboList(UNIT_CMB));
					//EditBox
					//ComboBox
				}
			}
			break;

		//=========
		// 10.Audio 
		//=========
		case 10:
			if(CheckMainStepType(nRow) == CHECK_FUNCTION)
			{
				SetAudioSpec(nRow, nCol);
				SetEditPos(nRow, nCol + 250, 250, 20);
				SetComboPos(nRow, nCol + 500, 250, 20, "");
			}
			break;

		default :
		//	m_ctrlMainGrid.SetRow(-1);
		//	m_ctrlMainGrid.SetCol(-1);
			break;
	}
}

void CSeqEditorDlg::InitSubGrid()
{
	CString sTmp;

	char *sHeader[] = {"STEP NAME", "DATA1", "DATA2", "DATA3", "DATA4", "DATA5", "DATA6", "DATA7", "DATA8", "DATA9", "DATA10" , "COMMENT" };
	int nWidth[] = { 150, 120, 120, 120, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80 };
	

	

	m_ctrlSubList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가

	for (UINT nTmp = 0; nTmp < 12; nTmp++)
	{
		m_ctrlSubList.InsertColumn(nTmp, sHeader[nTmp], LVCFMT_CENTER, nWidth[nTmp]);//m_ctrlMainGrid.SetCol(nTmp);
	}
	m_ctrlSubList.Invalidate();


	//m_ctrlSubGrid.SetRedraw(flexRDNone);
	//m_ctrlSubGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearText))); //flex , flexClearEverything

	//m_ctrlSubGrid.SetBackColorFixed(RGB(210,216,176));
	//m_ctrlSubGrid.SetForeColorFixed(RGB(51,102,153));

	////+2008.02.12 Mod BY USY
	//m_ctrlSubGrid.SetBackColorBkg(RGB(255,255,255));
	//m_ctrlSubGrid.SetBackColor(RGB(255,255,255));
	////m_ctrlSubGrid.SetBackColorBkg(RGB(247,247,231));
	////m_ctrlSubGrid.SetBackColor(RGB(247,247,231));
	////-

	//m_ctrlSubGrid.SetGridColor(RGB(210,216,176));
	//

	//m_ctrlSubGrid.SetFixedRows(1);
	//m_ctrlSubGrid.SetFixedCols(0);

	//m_ctrlSubGrid.SetCols(11);
	//m_ctrlSubGrid.SetRows(1);
	//m_ctrlSubGrid.SetRow(0);
	//Header
	//for(UINT nTmp = 0; nTmp < 11 ; nTmp++)
	//{
	//	m_ctrlSubGrid.SetCol(nTmp);
	//	m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);

	//	//+add kwmoon 080724
	//	m_ctrlSubGrid.SetFixedAlignment(nTmp, flexAlignCenterCenter);

	//	m_ctrlSubGrid.SetFillStyle(flexFillRepeat);
	//	m_ctrlSubGrid.SetFontName("Arial");
	//	m_ctrlSubGrid.SetFontSize(10);
	//	m_ctrlSubGrid.SetExtendLastCol(TRUE);
	//	m_ctrlSubGrid.SetText(sHeader[nTmp]);
	//	
	//	if(nTmp == 0) m_ctrlSubGrid.SetColWidth(nTmp, 2000);
	//	else if(nTmp == 1) m_ctrlSubGrid.SetColWidth(nTmp, 2500);
	//	else if(nTmp == 2) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//	else if(nTmp == 3) m_ctrlSubGrid.SetColWidth(nTmp, 2500);
	//	else if(nTmp == 4) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//	else if(nTmp == 5) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//	else if(nTmp == 6) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//	else if(nTmp == 7) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//	else if(nTmp == 8) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//	else if(nTmp == 9) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//	else if(nTmp == 10) m_ctrlSubGrid.SetColWidth(nTmp, 1500);
	//}
	//m_ctrlSubGrid.SetRow(0);
	//m_ctrlSubGrid.SetCol(0);
	//m_ctrlSubGrid.SetRedraw(-1);
	//m_ctrlSubGrid.Refresh();
	return;	
}

void CSeqEditorDlg::UpdateSubGrid()
{
	//m_ctrlSubGrid.SetRedraw(flexRDNone);
//m_ctrlSubGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

//Row 
//m_ctrlSubGrid.SetRows(m_CurFunc.GetCount() + 1);
	m_ctrlSubList.DeleteAllItems();
	CString sTmp, sNo, sData, sData1;
	int nTmp, nSelNo;

	if(m_CurFunc.GetCount() > 0)
	{
		POSITION CurPos = m_CurFunc.GetHeadPosition();
		nTmp = 0;
		while (CurPos) 
		{
			CFunction* pFunc = m_CurFunc.GetNext(CurPos);
			nSelNo = CheckSubStepType(pFunc->m_pFunc);
			sTmp.Format("%s", CheckSubStepName(pFunc->m_pFunc));

			//+2007.10.11 Add BY USY
			if((nSelNo == -1) && (sTmp == _T(""))) continue;
			//-.

			if(sTmp.Compare("send_i2c_command") == NULL)
			{
				int nDebug = 1;
			}

			//m_ctrlSubGrid.SetRow(nTmp);

			//m_ctrlSubGrid.SetCol(0);
			//m_ctrlSubGrid.SetColDataType(0, flexDTString);
			//m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
			//m_ctrlSubGrid.SetTextMatrix(nTmp , 0, sTmp);

			m_ctrlSubList.InsertItem(nTmp, sTmp);

			m_ctrlSubList.SetItemText(nTmp, 11, pFunc->m_strCommant);
			//	m_ctrlStepMainList.InsertItem(nTmp, sTmp);   // // m_ctrlSpecAdjList.InsertColumn(1, "Margin", LVCFMT_CENTER, 100); //m_ctrlSpecAdjGrid.SetTextMatrix(i + 1,0, InputDefined[i].pszName );
			//m_ctrlStepMainList.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetCurrentSource(), NULL, NULL, NULL, NULL); // m_ctrlMainGrid.SetTextMatrix(nTmp,2,pStep->GetStepName());
		
			int nTmp2 = 0;
			int nCol = 1;
			sData = _T("");
			sData1 = _T("");

			//+add kwmoon 080809
			int nArgumentNo		= 0;
			int nI2CfirstArgument  = 0;

			POSITION ArgPos = pFunc->m_ArgumentList.GetHeadPosition();

			Argument* pArgument ;

			if(ArgPos  != NULL)  pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
			do 
			{
				//+add kwmoon 080809
				nArgumentNo++;

				if(Predefined[nSelNo].achPara[nTmp2] == INTEGER)
				{
					sData1.Format("%d", pArgument->Integer);
				
					//+add kwmoon 080809
					if(sTmp.Compare("send_i2c_command") == NULL)
					{
						if(nArgumentNo == 2)
						{
							switch(pArgument->Integer)
							{
								case 101 : sData1 = "I2C_TYPE";			break;
								case 102 : sData1 = "I2C_CMD";			break;
								case 103 : sData1 = "I2C_READ";			break;
								case 104 : sData1 = "I2C_CHK_VALUE";	break;
								case 105 : sData1 = "I2C_CHK_MAXMIN";	break;
								case 106 : sData1 = "I2C_CHK_LIMIT";	break;
								case 107 : sData1 = "I2C_2B_READ";		break;
								case 108 : sData1 = "I2C_2B_WRITE";		break;
								case 109 : sData1 = "I2C_CHANNEL";		break;
								//+add kwmoon 080911
								case 112 : sData1 = "I2C_CHK_VER";		break;
						}
							nI2CfirstArgument = pArgument->Integer;
						}
						else if(nArgumentNo == 4)
						{
							if(nI2CfirstArgument == I2C_TYPE)
							{
								switch(pArgument->Integer)
								{
									case 110 : sData1 = "DDC_2AB";			break;
									case 111 : sData1 = "DDC_2B";			break;
								}
							}
							else
							{
								sData1.Format("0x%02x ", pArgument->Integer);
							}
						}
						else if(nArgumentNo == 6)
						{
							if(nI2CfirstArgument != I2C_CHK_VALUE)
							{
								sData1.Format("0x%02x ", pArgument->Integer);
							}
						}
					}
					//-
						//+add psh 090703
					if(sTmp.Compare("set_dimmingtest") == NULL)
					{
						if(nArgumentNo == 4)
						{
							sData1.Format("0x%02x ", pArgument->Integer);
						}
						else if(nArgumentNo == 6)
						{
							sData1.Format("0x%02x ", pArgument->Integer);
						}
					}//-

					//m_ctrlSubGrid.SetCol(nCol);
					//m_ctrlSubGrid.SetColDataType(nCol, flexDTString);
					//m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
					//m_ctrlSubGrid.SetTextMatrix(nTmp , nCol, sData1);
					//m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
					m_ctrlSubList.SetItem(nTmp, nCol, LVIF_TEXT, sData1, NULL, NULL, NULL, NULL); // m_ctrlMainGrid.SetTextMatrix(nTmp,2,pStep->GetStepName());

					nCol++;

					if(ArgPos  == NULL) break;
					else pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
				}
				else if((Predefined[nSelNo].achPara[nTmp2] == STRING))
				{
					sData1.Format("\"%s\"", pArgument->String);

					//m_ctrlSubGrid.SetCol(nCol);
					//m_ctrlSubGrid.SetColDataType(nCol, flexDTString);
					//m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
					//m_ctrlSubGrid.SetTextMatrix(nTmp , nCol, sData1);
					//m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
					m_ctrlSubList.SetItem(nTmp, nCol, LVIF_TEXT, sData1, NULL, NULL, NULL, NULL); //
					nCol++;

					if(ArgPos  == NULL) break;
					else pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
				}
				//+ 2007.10.10 Add BY USY
				else if((Predefined[nSelNo].achPara[nTmp2] == REAL))
				{
					sData1.Format("%.1f", float(pArgument->Double));

					//m_ctrlSubGrid.SetCol(nCol);
					//m_ctrlSubGrid.SetColDataType(nCol, flexDTString);
					//m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
					//m_ctrlSubGrid.SetTextMatrix(nTmp , nCol, sData1);
					//m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));

					m_ctrlSubList.SetItem(nTmp, nCol, LVIF_TEXT, sData1, NULL, NULL, NULL, NULL); //
					nCol++;

					if(ArgPos  == NULL) break;
					else pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
				}
				//-
				nTmp2++;
			}while((Predefined[nSelNo].achPara[nTmp2] != TNULL) /*&& (ArgPos != NULL)*/);

			nTmp++;
		}
	}
//m_ctrlSubGrid.SetRow(0);
//m_ctrlSubGrid.SetCol(0);
//m_ctrlSubGrid.SetRedraw(-1);
//m_ctrlSubGrid.Refresh();
	//m_ctrlSubGrid.SetRedraw(flexRDNone);
	//m_ctrlSubGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	////Row 
	//m_ctrlSubGrid.SetRows(m_CurFunc.GetCount() + 1);

	//CString sTmp, sNo, sData, sData1;
	//int nTmp, nSelNo;
	//
	//if(m_CurFunc.GetCount() > 0)
	//{
	//	POSITION CurPos = m_CurFunc.GetHeadPosition();
	//	nTmp = 1;
	//	while (CurPos) 
	//	{
	//		CFunction* pFunc = m_CurFunc.GetNext(CurPos);
	//		nSelNo = CheckSubStepType(pFunc->m_pFunc);
	//		sTmp.Format("%s", CheckSubStepName(pFunc->m_pFunc));

	//		//+2007.10.11 Add BY USY
	//		if((nSelNo == -1) && (sTmp == _T(""))) continue;
	//		//-.

	//		if(sTmp.Compare("send_i2c_command") == NULL)
	//		{
	//			int nDebug = 1;
	//		}

	//		m_ctrlSubGrid.SetRow(nTmp);

	//		m_ctrlSubGrid.SetCol(0);
	//		m_ctrlSubGrid.SetColDataType(0, flexDTString);
	//		m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
	//		m_ctrlSubGrid.SetTextMatrix(nTmp , 0, sTmp);
	//		
	//		int nTmp2 = 0;
	//		int nCol = 1;
	//		sData = _T("");
	//		sData1 = _T("");

	//		//+add kwmoon 080809
	//		int nArgumentNo		= 0;
	//		int nI2CfirstArgument  = 0;

	//		POSITION ArgPos = pFunc->m_ArgumentList.GetHeadPosition();

	//		Argument* pArgument ;

	//		if(ArgPos  != NULL)  pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
	//		do 
	//		{
	//			//+add kwmoon 080809
	//			nArgumentNo++;

	//			if(Predefined[nSelNo].achPara[nTmp2] == INTEGER)
	//			{
	//				sData1.Format("%d", pArgument->Integer);
	//				
	//				//+add kwmoon 080809
	//				if(sTmp.Compare("send_i2c_command") == NULL)
	//				{
	//					if(nArgumentNo == 2)
	//					{
	//						switch(pArgument->Integer)
	//						{
	//							case 101 : sData1 = "I2C_TYPE";			break;
	//							case 102 : sData1 = "I2C_CMD";			break;
	//							case 103 : sData1 = "I2C_READ";			break;
	//							case 104 : sData1 = "I2C_CHK_VALUE";	break;
	//							case 105 : sData1 = "I2C_CHK_MAXMIN";	break;
	//							case 106 : sData1 = "I2C_CHK_LIMIT";	break;
	//							case 107 : sData1 = "I2C_2B_READ";		break;
	//							case 108 : sData1 = "I2C_2B_WRITE";		break;
	//							case 109 : sData1 = "I2C_CHANNEL";		break;
	//							//+add kwmoon 080911
	//							case 112 : sData1 = "I2C_CHK_VER";		break;
	//					}
	//						nI2CfirstArgument = pArgument->Integer;
	//					}
	//					else if(nArgumentNo == 4)
	//					{
	//						if(nI2CfirstArgument == I2C_TYPE)
	//						{
	//							switch(pArgument->Integer)
	//							{
	//								case 110 : sData1 = "DDC_2AB";			break;
	//								case 111 : sData1 = "DDC_2B";			break;
	//							}
	//						}
	//						else
	//						{
	//							sData1.Format("0x%02x ", pArgument->Integer);
	//						}
	//					}
	//					else if(nArgumentNo == 6)
	//					{
	//						if(nI2CfirstArgument != I2C_CHK_VALUE)
	//						{
	//							sData1.Format("0x%02x ", pArgument->Integer);
	//						}
	//					}
	//				}
	//				//-
	//					//+add psh 090703
	//				if(sTmp.Compare("set_dimmingtest") == NULL)
	//				{
	//					if(nArgumentNo == 4)
	//					{
	//						sData1.Format("0x%02x ", pArgument->Integer);
	//					}
	//					else if(nArgumentNo == 6)
	//					{
	//						sData1.Format("0x%02x ", pArgument->Integer);
	//					}
	//				}//-

	//				m_ctrlSubGrid.SetCol(nCol);
	//				m_ctrlSubGrid.SetColDataType(nCol, flexDTString);
	//				m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
	//				m_ctrlSubGrid.SetTextMatrix(nTmp , nCol, sData1);
	//				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
	//				nCol++;

	//				if(ArgPos  == NULL) break;
	//				else pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
	//			}
	//			else if((Predefined[nSelNo].achPara[nTmp2] == STRING))
	//			{
	//				sData1.Format("\"%s\"", pArgument->String);
	//
	//				m_ctrlSubGrid.SetCol(nCol);
	//				m_ctrlSubGrid.SetColDataType(nCol, flexDTString);
	//				m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
	//				m_ctrlSubGrid.SetTextMatrix(nTmp , nCol, sData1);
	//				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
	//				nCol++;

	//				if(ArgPos  == NULL) break;
	//				else pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
	//			}
	//			//+ 2007.10.10 Add BY USY
	//			else if((Predefined[nSelNo].achPara[nTmp2] == REAL))
	//			{
	//				sData1.Format("%.1f", float(pArgument->Double));
	//
	//				m_ctrlSubGrid.SetCol(nCol);
	//				m_ctrlSubGrid.SetColDataType(nCol, flexDTString);
	//				m_ctrlSubGrid.SetCellAlignment(flexAlignLeftCenter);
	//				m_ctrlSubGrid.SetTextMatrix(nTmp , nCol, sData1);
	//				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
	//				nCol++;

	//				if(ArgPos  == NULL) break;
	//				else pArgument = pFunc->m_ArgumentList.GetNext(ArgPos);
	//			}
	//			//-
	//			nTmp2++;
	//		}while((Predefined[nSelNo].achPara[nTmp2] != TNULL) /*&& (ArgPos != NULL)*/);

	//		nTmp++;
	//	}
	//}
	//m_ctrlSubGrid.SetRow(0);
	//m_ctrlSubGrid.SetCol(0);
	//m_ctrlSubGrid.SetRedraw(-1);
	//m_ctrlSubGrid.Refresh();
}

void CSeqEditorDlg::SetSubGridHeader(int nRow)
{
//	int nSelNo = -1;
//	
//		CString sMean;
//	for(int nTmp = 1; nTmp < 10; nTmp++)
//	{
//		sMean.Format("Data%d", nTmp);
//		m_ctrlSubGrid.SetTextMatrix(0, nTmp, sMean);
//	}
//
//	if(nRow < 1) return;
//	CString sTmp = m_ctrlSubGrid.GetTextMatrix(nRow, 0);
//
//	sTmp.MakeLower();
//	sTmp.TrimRight();
//	sTmp.TrimLeft();
//
//int nTmp = 0;
//	if(!sTmp.IsEmpty())
//	{
//		for( nTmp = 0; Predefined[nTmp].pszFunc != NULL;nTmp++)
//		{
//			if(sTmp == Predefined[nTmp].pszFunc)
//			{
//				nSelNo = nTmp;
//				break;
//			}
//		}
//		if(nSelNo == -1) return;
//		
//		nTmp = 1;
//		for(int nTmp2 = 0; Predefined[nSelNo].achPara[nTmp2] != NULL; nTmp2++)
//		{
//			//+ 2007.10.10 Mod BY USY
//			if(((Predefined[nSelNo].achPara[nTmp2] == STRING)) ||
//			   ((Predefined[nSelNo].achPara[nTmp2] == INTEGER)) ||
//			   ((Predefined[nSelNo].achPara[nTmp2] == REAL)))
//			{
//				m_ctrlSubGrid.SetTextMatrix(0 , nTmp, Predefined[nSelNo].pszMean[nTmp -1] );
//				nTmp++;
//			}
////			if((Predefined[nSelNo].achPara[nTmp2] == STRING))
////			{
////				m_ctrlSubGrid.SetTextMatrix(0 , nTmp, Predefined[nSelNo].pszMean[nTmp -1] );
////				nTmp++;
////			}
////			else if((Predefined[nSelNo].achPara[nTmp2] == INTEGER))
////			{
////				m_ctrlSubGrid.SetTextMatrix(0 , nTmp, Predefined[nSelNo].pszMean[nTmp -1] );
////				nTmp++;
////			}
//			//-
//		}
//	}	
}

void CSeqEditorDlg::SetEditPos(int nRow, int nCol, int Width, int Height)
{
	
	CString strTemp;
	RECT lRect;
	RECT lMainRect;
	
	this->GetWindowRect(&lMainRect);
	m_ctrlStepMainList.GetWindowRect(&lRect);
	int leftStart = lRect.left - lMainRect.left;
	int topStart = lRect.top - lMainRect.top + 72;
	int lscroll = m_ctrlStepMainList.GetScrollPos(0);
	leftStart -= lscroll;

	switch (nCol )
	{
	case 2:

		
		GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, leftStart + 100, topStart + nRow * 17 - 84, 160, 22, SWP_ASYNCWINDOWPOS);
		//GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, lRect.left + 20, lRect.top + nRow * 16 + 20, 50, 20, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow, nCol);
		SetDlgItemText(IDC_EDIT_SEQ_EDITOR, strTemp);

		//int m_Main_Edit_Enable;
		break;
	case 7:
		GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, leftStart + 610, topStart + nRow * 17 - 84, 70, 22, SWP_ASYNCWINDOWPOS);
		//GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, lRect.left + 20, lRect.top + nRow * 16 + 20, 50, 20, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow, nCol);
		SetDlgItemText(IDC_EDIT_SEQ_EDITOR, strTemp);

		//int m_Main_Edit_Enable;
		break;
	case 8:


		GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, leftStart + 690, topStart + nRow * 17 - 84, 70, 22, SWP_ASYNCWINDOWPOS);
		//GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, lRect.left + 20, lRect.top + nRow * 16 + 20, 50, 20, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow, nCol);
		SetDlgItemText(IDC_EDIT_SEQ_EDITOR, strTemp);

		//int m_Main_Edit_Enable;
		break;
	default:
		GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL,  2500,   85, 80, 24, SWP_ASYNCWINDOWPOS);//m_cComboSeqEditor.SetWindowPos(NULL, lRect.left + 2500, lRect.top + nRow * 17 - 85, 80, 10, SWP_ASYNCWINDOWPOS);
		break;
	}
	
	
}
void CSeqEditorDlg::SetComboPos(int nRow, int nCol, int Width, int Height, CString Data)
{
	RECT lRect;
	RECT lMainRect;
	CString strTemp;
	this->GetWindowRect(&lMainRect);
	m_ctrlStepMainList.GetWindowRect(&lRect);
	int leftStart = lRect.left - lMainRect.left;
	int topStart = lRect.top - lMainRect.top+72;
	int lscroll = m_ctrlStepMainList.GetScrollPos(0);
	leftStart -= lscroll;
	//	m_ctrlStepMainList.GetClientRect(&lRect);
	switch (nCol )
	{
	case 1:

		m_cComboSeqEditor.SetWindowPos(NULL, leftStart + 30, topStart + nRow * 17 - 85, 80, 10, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow,  nCol);
		m_Main_Combo_Enable = 1;
		for (int i = 0; i < m_ComboDataList.GetCount(); i++)
		{
			if (strTemp.Compare(m_ComboDataList.GetAt(i)) == 0)
			{
				m_cComboSeqEditor.SetCurSel(i);
				break;
			}
		}
		break;
	case 4:

		m_cComboSeqEditor.SetWindowPos(NULL, leftStart + 310, topStart + nRow * 17 - 85, 80, 10, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow, nCol);
		m_Main_Combo_Enable = 1;
		for (int i = 0; i < m_ComboDataList.GetCount(); i++)
		{
			if (strTemp.Compare(m_ComboDataList.GetAt(i)) == 0)
			{
				m_cComboSeqEditor.SetCurSel(i);
				break;
			}
		}
		break;
	case 5:

		m_cComboSeqEditor.SetWindowPos(NULL, leftStart + 390, topStart + nRow * 17 - 85, 130, 10, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow, nCol);
		m_Main_Combo_Enable = 1;
		for (int i = 0; i < m_ComboDataList.GetCount(); i++)
		{
			if (strTemp.Compare(m_ComboDataList.GetAt(i)) == 0)
			{
				m_cComboSeqEditor.SetCurSel(i);
				break;
			}
		}
		break;
	case 6:

		m_cComboSeqEditor.SetWindowPos(NULL, leftStart + 525, topStart + nRow * 17 - 85, 75, 10, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow, nCol);
		m_Main_Combo_Enable = 1;
		for (int i = 0; i < m_ComboDataList.GetCount(); i++)
		{
			if (strTemp.Compare(m_ComboDataList.GetAt(i)) == 0)
			{
				m_cComboSeqEditor.SetCurSel(i);
				break;
			}
		}
		break;
	case 9:

		m_cComboSeqEditor.SetWindowPos(NULL, leftStart + 770, topStart + nRow * 17 - 85, 75, 10, SWP_ASYNCWINDOWPOS);
		strTemp = m_ctrlStepMainList.GetItemText(nRow, nCol);
		m_Main_Combo_Enable = 1;
		for (int i = 0; i < m_ComboDataList.GetCount(); i++)
		{
			if (strTemp.Compare(m_ComboDataList.GetAt(i)) == 0)
			{
				m_cComboSeqEditor.SetCurSel(i);
				break;
			}
		}
		break;	
	
	default:
		m_cComboSeqEditor.SetWindowPos(NULL, lRect.left + 2500, topStart + nRow * 17 - 85, 80, 10, SWP_ASYNCWINDOWPOS);
		m_Main_Combo_Enable = 0;
		break;
	}

	//
}

void CSeqEditorDlg::SetSubGrid4Edit(int nFuncNo, int nRow, int nCol)
{
	if(nCol == 0)
	{
		//RECT lRect;
		//RECT lMainRect;
		//CString strTemp;
		//CString strTempCombo;
		//this->GetWindowRect(&lMainRect);
		//m_ctrlSubList.GetWindowRect(&lRect);
		//int leftStart = lRect.left - lMainRect.left;
		//int topStart = lRect.top - lMainRect.top + 72;
		//int lscroll = m_cComboSeqSubEditor.GetScrollPos(0);
		//leftStart -= lscroll;
		////m_ctrlSubGrid.SetRow(nRow);
		////m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
		////m_ctrlSubGrid.SetColComboList(0, MakeSubStepFuncList());
		////COMBO BOX
		//MakeSubStepFuncList();
		//m_cComboSeqSubEditor.SetWindowPos(NULL, leftStart-10 , topStart+ nRow*17-85, 150, 10, SWP_ASYNCWINDOWPOS);

		//strTemp = m_ctrlSubList.GetItemText(nRow, nCol);
		//m_Sub_Combo_Enable = 1;
		//for (int i = 0; i < m_cComboSeqSubEditor.GetCount(); i++)
		//{
		//	//strTempCombo = m_cComboSeqSubEditor.GetDlgItemTextA
		//	if (strTemp.Compare(m_ComboSubDataList.GetAt(i)) == 0)
		//	{
		//		m_cComboSeqSubEditor.SetCurSel(i);
		//		break;
		//	}
		//}

		//m_cComboSeqSubEditor.SetFocus();
		return;
	}
	//+ 2007.10.11 Add BY USY
	if((nFuncNo >= 0)&&(Predefined[nFuncNo].argType[nCol - 1] != NULL_TYPE))
	{
		SetSubDlg4SubGrid(Predefined[nFuncNo].argType[nCol - 1], nRow, nCol);
	}
	else
	{
		SetSubDlg4SubGrid(NO_EDIT_TYPE, nRow, nCol);
	}
	//-

	
}

void CSeqEditorDlg::SetDefaultData2SubStepGrid(int nRow, int nCol)
{
//	int nSelNo = -1;
//	CString sTmp = m_ctrlSubGrid.GetTextMatrix(nRow, nCol);
//	CString sMean;
//	sTmp.MakeLower();
//	sTmp.TrimRight();
//	sTmp.TrimLeft();
//	int nTmp = 0;
//	if(!sTmp.IsEmpty())
//	{
//		for( nTmp = 0; Predefined[nTmp].pszFunc != NULL;nTmp++)
//		{
//			if(sTmp == Predefined[nTmp].pszFunc)
//			{
//				nSelNo = nTmp;
//				break;
//			}
//		}
//		if(nSelNo == -1) return;
//
//		for(nTmp = 1; nTmp < 10; nTmp++)
//		{
//			m_ctrlSubGrid.SetRow(nRow);
//			m_ctrlSubGrid.SetCol(nTmp);
//			m_ctrlSubGrid.SetCellBackColor(RGB(247,247,231));
//			m_ctrlSubGrid.SetTextMatrix(nRow, nTmp, _T(""));
//			sMean.Format("Data%d", nTmp);
//			m_ctrlSubGrid.SetTextMatrix(0, nTmp, sMean);
//		}
//		
//		nTmp = 1;
//		for(int nTmp2 = 0; Predefined[nSelNo].achPara[nTmp2] != NULL; nTmp2++)
//		{
//			if((Predefined[nSelNo].achPara[nTmp2] == STRING))
//			{
//				m_ctrlSubGrid.SetCol(nTmp);
//				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
//				m_ctrlSubGrid.SetTextMatrix(nRow , nTmp, _T("\"\""));
//				m_ctrlSubGrid.SetTextMatrix(0 , nTmp, Predefined[nSelNo].pszMean[nTmp -1] );
//				nTmp++;
//			}
//			else if(((Predefined[nSelNo].achPara[nTmp2] == INTEGER)) ||
//				    ((Predefined[nSelNo].achPara[nTmp2] == REAL)))
//			{
//				m_ctrlSubGrid.SetCol(nTmp);
//				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
//				m_ctrlSubGrid.SetTextMatrix(nRow , nTmp, _T(""));
//				m_ctrlSubGrid.SetTextMatrix(0 , nTmp, Predefined[nSelNo].pszMean[nTmp -1] );
//				nTmp++;
//			}
////			if((Predefined[nSelNo].achPara[nTmp2] == STRING))
////			{
////				m_ctrlSubGrid.SetCol(nTmp);
////				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
////				m_ctrlSubGrid.SetTextMatrix(nRow , nTmp, _T("\"\""));
////				m_ctrlSubGrid.SetTextMatrix(0 , nTmp, Predefined[nSelNo].pszMean[nTmp -1] );
////				nTmp++;
////			}
////			else if((Predefined[nSelNo].achPara[nTmp2] == INTEGER))
////			{
////				m_ctrlSubGrid.SetCol(nTmp);
////				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,128));
////				m_ctrlSubGrid.SetTextMatrix(nRow , nTmp, _T(""));
////				m_ctrlSubGrid.SetTextMatrix(0 , nTmp, Predefined[nSelNo].pszMean[nTmp -1] );
////				nTmp++;
////			}
//			//-
//		}
//	}
}


void CSeqEditorDlg::SetSubDlg4SubGrid(int nType, int nRow, int nCol)
{
	//m_ctrlSubGrid.SetRow(nRow);
	//m_ctrlSubGrid.SetCol(nCol);
	//m_ctrlSubGrid.SetEditable(FALSE);
	//m_ctrlSubGrid.SetColComboList(nCol, _T(""));
	switch(nType) 
	{
		case NO_EDIT_TYPE:
			break;
		case NORMAL_EDIT_TYPE:
			{
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetCol(nCol);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
			//EDIT BOX
			}
			break;

		case FILE_SEL_TYPE:
			{
				//+2007.12.05 Mod BY USY
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetCol(nCol);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
			//EDIT BOX
			//-
			}
			break;
		case REMOTE_SEL_TYPE:
			{
				CSeqRemoteDlg dlg;
				if(dlg.DoModal() == IDOK)
				{
					CString sTmp;
					sTmp.Format("\"%s\"", dlg.m_strKeyCode);
					//m_ctrlSubGrid.SetTextMatrix(nRow, nCol, sTmp);
					m_ctrlSubList.SetItem(nRow, nCol, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
				}
			}
			break;
		case ROI_SEL_TYPE:
			{
				CSeqRoiDlg dlg;
				//change PSH 081105
//				dlg.m_strModelFolder = CurrentSet->sModelFolder + "\\ref\\";
				dlg.m_strModelFolder = CurrentSet->sRefFolder + "\\";

				dlg.m_nPrevRoiX1 = atoi(m_ctrlSubList.GetItemText(nRow, 1));// .m_ctrlSubGrid.GetTextMatrix(nRow, 1));
				dlg.m_nPrevRoiY1 = atoi(m_ctrlSubList.GetItemText(nRow, 2));//m_ctrlSubGrid.GetTextMatrix(nRow, 2));
				dlg.m_nPrevRoiX2 = atoi(m_ctrlSubList.GetItemText(nRow, 3));//m_ctrlSubGrid.GetTextMatrix(nRow, 3));
				dlg.m_nPrevRoiY2 = atoi(m_ctrlSubList.GetItemText(nRow, 4));//m_ctrlSubGrid.GetTextMatrix(nRow, 4));

				if(dlg.DoModal() == IDOK)
				{
					CString sStartX, sStartY, sEndX, sEndY;
					sStartX.Format("%d", dlg.m_nRoiX1);
					sStartY.Format("%d", dlg.m_nRoiY1);
					sEndX.Format("%d", dlg.m_nRoiX1 + dlg.m_nRoiWidth);
					sEndY.Format("%d", dlg.m_nRoiY1 + dlg.m_nRoiHeight);

					m_ctrlSubList.SetItem(nRow, 1, LVIF_TEXT, sStartX, NULL, NULL, NULL, NULL); //m_ctrlSubGrid.SetTextMatrix(nRow, 1, `sStartX);
					m_ctrlSubList.SetItem(nRow, 2, LVIF_TEXT, sStartY, NULL, NULL, NULL, NULL); //m_ctrlSubGrid.SetTextMatrix(nRow, 2, sStartY);
					m_ctrlSubList.SetItem(nRow, 3, LVIF_TEXT, sEndX, NULL, NULL, NULL, NULL); //m_ctrlSubGrid.SetTextMatrix(nRow, 3, sEndX);
					m_ctrlSubList.SetItem(nRow, 4, LVIF_TEXT, sEndY, NULL, NULL, NULL, NULL); //m_ctrlSubGrid.SetTextMatrix(nRow, 4, sEndY);
				}			
			}
			break;
		case AUDIO_SRC_SEL_TYPE:
			{
				CString sTmp;
				sTmp.Format("|1\tSpeaker|2\tAv1|3\tAv2|4\tVariable Out|5\tHeadPhone|6\tWoofer|8\tSPDIF|9\tOptic");
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sTmp);
				//COMBO BOX
			
			}
			break;
		case GRABBER_SEL_TYPE:
			{
				CString sTmp;
				sTmp.Format("|0\tLVDS Grabber|1\tAnalog Grabber");
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sTmp);
				//COMBO BOX
			}
			break;
		case RF_SEL_TYPE:
			{
				CString sTmp;
				sTmp.Format("|0\tNTSC|1\tPal");
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sTmp);
				//COMBO BOX
			}
			break;
		case VIDEO_SRC_SEL_TYPE:
			{
				CString sTmp, sList;
				for(int i = 0; InputDefined[i].pszName != NULL; i++)
				{
					sTmp.Format("|\"%s\"", InputDefined[i].pszName);
					sList = sList + sTmp;
				}
//				sList.Format("|RF|AV|AV_YC|AV_1|AV1_YC1|AV2|AV2_YC2|AV3|AV3_YC3|COMP|COMP1|COMP2|RGB|HDMI|HDMI1|HDMI2|HDMI3");
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sList);
			}
			break;
		case USE_SEL_TYPE:
			{
				CString sTmp;
				sTmp.Format("|0\tNo Use|1\tUse");
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sTmp);
			}
			break;
		case PATTERN_CMD_SEL_TYPE:
			{
				CString sTmp;
				sTmp.Format("|7\tModel Change|8\tPattern Change|9\tModel PatternChange");
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sTmp);

			}
			break;
		
		//AvSwitch Cmd 
		//1st parameter(Cmd Type)		/ 2nd parameter (nValue1) 				/ 3rd Parameter (nValue2)
		//01. SET_DIGIT_OUT				/ Digital Out Port(00, 01,02, 03~)		/ Digit Out On / Off( 0 : Off , 1 : On)
  		//02. SET_VIDEO_CH				/ Video Channel(1 : CH1, 2 : Ch2)		/ Don't Care
		//03. SET_AUDIO_CH				/ Audio Channel  (1 ~ 8)				/ Don't Care
		//04. SET_COMP_CH				/ COMP Channel ( 1 : Ch 1, 2 : Ch 2)	/ Don't Care
		//05. SET_CVBS_CH				/ CVBS Channel ( 1 : Ch 1, 2 : Ch 2)	/ Don't Care
		//06. SET_ANALOG_VOLT			/ Port (1 : Port 1, 2 : Port 2)			/ Analog Volt ( 00 : 0V, 01 : 1V, 02 : 2V, 03 : 2.5V, 04 : 3.3V)
		//07. PULLUP_AVSWITCH			/ PullUp Volt ( 3 : 3.3V, 5 : 5V )		/ Don't Care
		//08. REMOTE_TYPE_SEL			/ Remote Type(1:Codeless IR ,2 : Code IR)/ Don't Care
		//09. CHK_AUDIO_LVL				/ Audio Level(0 : Left, 1 : Right)		/ Don't Care
		//10. CHK_AUDIO_FRQ				/ Audio Freq(0 : Left, 1 : Right)		/ Don't Care
		//11. CHECK_STATUS				/ Don't Care							/ Don't Care
		//12. SEND_REMOTE_CODE			/
		case AVSWITCH_CMD_SEL_TYPE:
			{
				CString sTmp;
				if(nCol == 1)
				{
					//+change kwmoon 080312
				//	sTmp.Format("|1\tSet Digit Out|2\tSet Video Ch|3\tSet Audio Ch|4\tSet Comp Ch|5\tSet Cvbs Ch|6\tSet Analog Volt|7\tPullUp AvSwitch|8\tRemote Type Sel");	
					sTmp.Format("|1\tSet Digit Out|2\tSet Video Ch|3\tSet Audio Ch|4\tSet Comp Ch|5\tSet Cvbs Ch|6\tSet Analog Volt|7\tPullUp AvSwitch|8\tRemote Type Sel|9\tRemote Repeat No");	
					//m_ctrlSubGrid.SetRow(nRow);
					//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//m_ctrlSubGrid.SetColComboList(nCol, sTmp);
				}

				else if(nCol == 2)
				{
					//switch(atoi(m_ctrlSubGrid.GetTextMatrix(nRow, 1)))
					//{
					//case SET_DIGIT_OUT:
					//	sTmp.Format("|0\tDIO Port0|1\tDIO Port1|2\tDIO Port2|3\tDIO Port3|4\tDIO Port4|5\tDIO Port5|6\tDIO Port6|7\tDIO Port7");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//case SET_VIDEO_CH:
					//	sTmp.Format("|1\tVIDEO CH1|2\tVIDEO CH2");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//case SET_AUDIO_CH:
					//	sTmp.Format("|1\tCH1|2\tCH2|3\tCH3|4\tCH4|5\tCH5|6\tCH6|7\tCH7|8\tCH8");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//case SET_COMP_CH:
					//	sTmp.Format("|1\tCOMP CH1|2\tCOMP CH2");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//case SET_CVBS_CH:
					//	sTmp.Format("|1\tCOMP CH1|2\tCOMP CH2");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//case SET_ANALOG_VOLT:
					//	sTmp.Format("|1\tPort 1|2\tPort 2");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//case PULLUP_AVSWITCH:
					//	sTmp.Format("|1\t3.3V|2\t5V");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//case REMOTE_TYPE_SEL:
					//	sTmp.Format("|1\tCodeless IR|2\tCode IR");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					////+add kwmoon 080312
					//case SET_REMOTE_REPEAT_NO:
					//	sTmp.Format("|1\t0|2\t1|2\t2|3\t2|4\t3");	
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
					//	m_ctrlSubGrid.SetColComboList(nCol, sTmp);
					//	break;
					//default:
					//	m_ctrlSubGrid.SetRow(nRow);
					//	m_ctrlSubGrid.SetEditable(flexEDNone);
					//	break;
					//}
				}
				else if(nCol == 3)
				{
				/*	switch(atoi(m_ctrlSubGrid.GetTextMatrix(nRow, 1)))
					{
					case SET_DIGIT_OUT:
						sTmp.Format("|0\t Off|1\t On");	
						m_ctrlSubGrid.SetRow(nRow);
						m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
						m_ctrlSubGrid.SetColComboList(nCol, sTmp);
						break;
					case SET_ANALOG_VOLT:
						sTmp.Format("|0\t0V|1\t1V|2\t2V|3\t2.5V|4\t3.3V");
						m_ctrlSubGrid.SetRow(nRow);
						m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
						m_ctrlSubGrid.SetColComboList(nCol, sTmp);
						break;
					default:
						m_ctrlSubGrid.SetRow(nRow);
						m_ctrlSubGrid.SetEditable(flexEDNone);
						m_ctrlSubGrid.SetTextMatrix(nRow, nCol, "0");
						break;
					}*/
				}
				else
				{

				}
			}
			break;
		case NUM_SEL_TYPE:
			{
				CString sTmp, sList;
				//for(int nTmp = 1; nTmp <= 20; nTmp++)
				//{
				//	sTmp.Format("|%d", nTmp);
				//	sList = sList + sTmp;
				//}
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sList);

			}
			break;
		case ECHO_SEL_TYPE:
			{
				CString sTmp;
				//sTmp.Format("|0\tEcho Off|1\tEcho On");
				//m_ctrlSubGrid.SetRow(nRow);
				//m_ctrlSubGrid.SetEditable(flexEDKbdMouse);
				//m_ctrlSubGrid.SetColComboList(nCol, sTmp);

			}
			break;
		//-

	}
//	m_ctrlSubGrid.Refresh();
}

CString CSeqEditorDlg::MakeMainStepComboList(int nType)
{
	CString sTmp  = _T("");
	CString sList = _T("");
	
	int nTmp = 0;

	switch(nType) 
	{
		case STEPTYPE_CMB:
			sList.Format("|CHECK|PROCESS");	
			m_ComboDataList.RemoveAll();
			m_ComboDataList.Add("CHECK");
			m_ComboDataList.Add("PROCESS");
			m_cComboSeqEditor.ResetContent();
			m_cComboSeqEditor.AddString("CHECK");
			m_cComboSeqEditor.AddString("PROCESS");
			break;

		case TEST_CMB:
			m_cComboSeqEditor.ResetContent();
			m_ComboDataList.RemoveAll();
			
			for(nTmp = 0; MeasDefined[nTmp].pszFunc != NULL; nTmp++)
			{
				m_ComboDataList.Add(MeasDefined[nTmp].pszFunc);
				m_cComboSeqEditor.AddString(MeasDefined[nTmp].pszFunc);
				sTmp.Format("|%s", MeasDefined[nTmp].pszFunc);
				sList = sList + sTmp;
			}
			break;

		case CURSRC_CMB:
			//m_ctrlStepMainList
			m_cComboSeqEditor.ResetContent();
			m_ComboDataList.RemoveAll();
			for(nTmp = 0; InputDefined[nTmp].pszName != NULL; nTmp++)
			{
				m_ComboDataList.Add(InputDefined[nTmp].pszName);
				m_cComboSeqEditor.AddString(InputDefined[nTmp].pszName);
				sTmp.Format("|%s", InputDefined[nTmp].pszName);
				sList = sList + sTmp;
			}
			break;

		case TARGET_CMB:
			sList.Format("|TRUE|FALSE");
			m_cComboSeqEditor.ResetContent();
			m_ComboDataList.RemoveAll();
			m_ComboDataList.Add("TRUE");
			m_ComboDataList.Add("FALSE");
			m_cComboSeqEditor.AddString("TRUE");
			m_cComboSeqEditor.AddString("FALSE");
			break;

		case UNIT_CMB:
			sList.Format("|%%|Level");	
			m_cComboSeqEditor.ResetContent();
			m_ComboDataList.RemoveAll();
			m_ComboDataList.Add("%");
			m_ComboDataList.Add("Level");
			m_cComboSeqEditor.AddString("%");
			m_cComboSeqEditor.AddString("Level");
			break;
	}

	return sList;
}

CString	CSeqEditorDlg::MakeSubStepFuncList()
{
	CString sList = _T("");
	CString sTmp;
	m_cComboSeqEditor.ResetContent();
	m_ComboSubDataList.RemoveAll();
	for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
	{
		m_cComboSeqSubEditor.AddString(Predefined[nTmp].pszFunc);
		m_ComboSubDataList.Add(Predefined[nTmp].pszFunc);
	//	sTmp.Format("|%s", Predefined[nTmp].pszFunc);
		sList = sList + sTmp;
	}
	return sList;
}

UINT CSeqEditorDlg::CheckMainStepType(int nRow)
{
	CString sTmp = _T("");
	int nType = PROCESS_FUNCTION;
	
	//m_ctrlMainGrid.Refresh();

	//+change kwmoon 080809
//	sTmp = m_ctrlMainGrid.GetTextMatrix(nRow, 4);
	sTmp = m_ctrlStepMainList.GetItemText(nRow, MAIN_STEP_TYPE_COL);//m_ctrlMainGrid.GetTextMatrix(nRow, MAIN_STEP_TYPE_COL);

	sTmp.MakeUpper();
	sTmp.TrimLeft();
	sTmp.TrimRight();

	if(sTmp.Compare("CHECK") == 0)
	{
		nType = CHECK_FUNCTION;
	}
	else if(sTmp.Compare("PROCESS") == 0)
	{
		nType = PROCESS_FUNCTION;
	}
	return nType;
}

UINT CSeqEditorDlg::CheckMainTestType(int nRow)
{
	CString sTmp;
	int nSelNo  = -1;
	//+change kwmoon 080809
//	sTmp = m_ctrlMainGrid.GetTextMatrix(nRow, 5);
	sTmp = m_ctrlStepMainList.GetItemText(nRow, MAIN_TEST_COL);//m_ctrlMainGrid.GetTextMatrix(nRow, MAIN_TEST_COL);
	sTmp.MakeLower();
	sTmp.TrimLeft();
	sTmp.TrimRight();

	for(int nTmp = 0; MeasDefined[nTmp].pszFunc != NULL; nTmp++)
	{
		if(sTmp == MeasDefined[nTmp].pszFunc)
		{
			nSelNo = nTmp;
			break;
		}
	}
	if(nSelNo <0) return MEAS_TYPEERROR;
	else return MeasDefined[nSelNo].FuncType;
}

BOOL CSeqEditorDlg::UpdateMainStep(int nStepNo)
{
	if(nStepNo < 0 ) return FALSE; 

	POSITION Pos	= StepList.GetHeadPosition();
	POSITION CurPos = StepList.FindIndex(nStepNo - 1);
	CString sTmp;

	//if(CurPos != NULL)
	//{
	//	CStep* pStep = StepList.GetAt(CurPos);
	//	//+change kwmoon 080809
	////	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, 1));
	//	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_NAME_COL));

	//	//+change kwmoon 080809
	////	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 2);
	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_TYPE_COL);
	//	sTmp.MakeUpper();
	//	sTmp.TrimRight();
	//	sTmp.TrimLeft();
	//	if(sTmp.Compare("CHECK") == 0)
	//	{
	//		pStep->m_nStepType = CHECK_FUNCTION;

	//		//+change kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 4);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_SOURCE_COL);
	//		sTmp.MakeUpper();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		pStep->SetCurrentSource(sTmp);

	//		//+change kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 3);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TEST_COL);
	//		sTmp.MakeLower();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		if(sTmp != _T(""))
	//		{
	//			pStep->m_strFuncName = sTmp;

	//			if(CheckMainTestType(nStepNo) == MEAS_BOOL)
	//			{
	//				pStep->SetFuncType(MEAS_BOOL);
	//				
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("TRUE") == 0)
	//				{
	//					pStep->SetNominal(TRUE);
	//				}
	//				else if(sTmp.Compare("FALSE") == 0)
	//				{
	//					pStep->SetNominal(FALSE);
	//				}
	//				else 
	//				{
	//					sTmp.Format("Please Select Target of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}						
	//			}
	//			else if(CheckMainTestType(nStepNo) == MEAS_DOUBLE)
	//			{
	//				pStep->SetFuncType(MEAS_DOUBLE);

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("") == 0)
	//				{
	//					sTmp.Format("Please Insert Target Value of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}
	//				else
	//				{
	//					pStep->SetNominal(atof(sTmp));
	//				}
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 6);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_L_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("") == 0)
	//				{
	//					sTmp.Format("Please Insert L-Limit Value of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}
	//				else
	//				{
	//					pStep->SetLowLimit(atof(sTmp));
	//				}
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 7);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_U_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("") == 0)
	//				{
	//					sTmp.Format("Please Insert H-Limit Value of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}
	//				else
	//				{
	//					pStep->SetHighLimit(atof(sTmp));
	//				}
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 8);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_UNIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				pStep->SetUnit(sTmp);
	//			}
	//			//+ 2007.10.10 Add BY USY
	//			//+change kwmoon 080809
	//		//	if((m_ctrlMainGrid.GetTextMatrix(nStepNo, 9) != _T(""))||
	//		//	   (m_ctrlMainGrid.GetTextMatrix(nStepNo, 9) != _T("0")))
	//			if((m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SOURCE_COL) != _T(""))||
	//			   (m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SOURCE_COL) != _T("0")))
	//			{
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 9);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SOURCE_COL);
	//				pStep->m_nAudioSource = atoi(sTmp);

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 10);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SPEC_COL);
	//				
	//				pStep->m_nLeftFreq[0] = atoi(sTmp.Left(sTmp.Find(",", 0)));
	//				sTmp.Delete(0, sTmp.Find(",", 0)  + 1);

	//				pStep->m_nRightFreq[0] = atoi(sTmp.Left(sTmp.Find(",", 0)));
	//				sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	//				
	//				pStep->m_nLeftLevel[0] = atoi(sTmp.Left(sTmp.Find(",", 0)));
	//				sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	//		
	//				pStep->m_nRightLevel[0] = atoi(sTmp);

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 11);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_MARGIN_COL);
	//				pStep->m_nAudioMargin = atoi(sTmp);
	//			}
	//			//-
	//		}

	//	}
	//	else if(sTmp.Compare("PROCESS") == 0)
	//	{
	//		pStep->m_nStepType = PROCESS_FUNCTION;
	//		
	//	}
	//	else
	//	{
	//		sTmp.Format("Please Select Step Type of Step %d", nStepNo );
	//		AfxMessageBox(sTmp);
	//		return FALSE;
	//	}
	//	StepList.SetAt(CurPos, pStep);
	//}

	//else if(CurPos == NULL)
	//{
	//	CStep* pStep = new CStep();

	//	//+add kwmoon 080809
	////	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, 1));
	//	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_NAME_COL));
	//
	//	//+add kwmoon 080809
	////	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 2);
	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_TYPE_COL);
	//	sTmp.MakeUpper();
	//	sTmp.TrimRight();
	//	sTmp.TrimLeft();
	//	if(sTmp.Compare("CHECK") == 0)
	//	{
	//		pStep->m_nStepType = CHECK_FUNCTION;
	//		//+add kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 4);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_SOURCE_COL);
	//		sTmp.MakeUpper();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		pStep->SetCurrentSource(sTmp);

	//		//+add kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 3);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TEST_COL);
	//		sTmp.MakeLower();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		if(sTmp != _T(""))
	//		{
	//			if(CheckMainTestType(nStepNo) == MEAS_BOOL)
	//			{
	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("TRUE") == 0)
	//				{
	//					pStep->SetNominal(TRUE);
	//				}
	//				else if(sTmp.Compare("FALSE") == 0)
	//				{
	//					pStep->SetNominal(FALSE);
	//				}
	//				else 
	//				{
	//					sTmp.Format("Please Select Target of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}						
	//			}
	//			else if(CheckMainTestType(nStepNo) == MEAS_DOUBLE)
	//			{
	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("") == 0)
	//				{
	//					sTmp.Format("Please Insert Target Value of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}
	//				else
	//				{
	//					pStep->SetNominal(atof(sTmp));
	//				}
	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 6);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_L_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("") == 0)
	//				{
	//					sTmp.Format("Please Insert L-Limit Value of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}
	//				else
	//				{
	//					pStep->SetLowLimit(atof(sTmp));
	//				}
	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 7);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_U_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("") == 0)
	//				{
	//					sTmp.Format("Please Insert H-Limit Value of Step %d", nStepNo );
	//					AfxMessageBox(sTmp);
	//					return FALSE;
	//				}
	//				else
	//				{
	//					pStep->SetHighLimit(atof(sTmp));
	//				}
	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 8);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_UNIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				pStep->SetUnit(sTmp);
	//			}
	//			//+ 2007.10.10 Add BY USY
	//			//+add kwmoon 080809
	//		//	if((m_ctrlMainGrid.GetTextMatrix(nStepNo, 9) != _T(""))||
	//		//	   (m_ctrlMainGrid.GetTextMatrix(nStepNo, 9) != _T("0")))
	//			if((m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SOURCE_COL) != _T(""))||
	//			   (m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SOURCE_COL) != _T("0")))
	//			{
	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 9);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SOURCE_COL);
	//				pStep->m_nAudioSource = atoi(sTmp);

	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 10);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_SPEC_COL);

	//				pStep->m_nLeftFreq[0] = atoi(sTmp.Left(sTmp.Find(",", 0)));
	//				sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	//				pStep->m_nRightFreq[0] = atoi(sTmp.Left(sTmp.Find(",", 0)));
	//				sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	//				pStep->m_nLeftLevel[0] = atoi(sTmp.Left(sTmp.Find(",", 0)));
	//				sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	//				pStep->m_nRightLevel[0] = atoi(sTmp);

	//				//+add kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 11);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_AUD_MARGIN_COL);
	//				pStep->m_nAudioMargin = atoi(sTmp);
	//			}
	//			//-
	//		}

	//	}
	//	else if(sTmp.Compare("PROCESS") == 0)
	//	{
	//		pStep->m_nStepType = PROCESS_FUNCTION;
	//		
	//	}
	//	else
	//	{
	//		sTmp.Format("Please Select Step Type of Step %d", nStepNo );
	//		AfxMessageBox(sTmp);
	//		return FALSE;
	//	}
	//	StepList.AddTail(pStep);
	//}  

	return TRUE;
}

//+2007.10.12 Mod BY USY
int CSeqEditorDlg::CheckSubStepType(BOOL (*m_pFunc)())
{
	int nStepType = -1;
//UINT CSeqEditorDlg::CheckSubStepType(BOOL (*m_pFunc)())
//{
//	UINT nStepType;
//-
	for(int nTmp1 = 0; Predefined[nTmp1].pszFunc != NULL; nTmp1++)
	{
		if(m_pFunc == Predefined[nTmp1].pFunc)
		{
			nStepType = nTmp1;
			break;
		}
	}	
	return nStepType;
}


CString CSeqEditorDlg::CheckSubStepName(BOOL (*m_pFunc)())
{
	CString sName;
	for(int nTmp1 = 0; Predefined[nTmp1].pszFunc != NULL; nTmp1++)
	{
		if(m_pFunc == Predefined[nTmp1].pFunc)
		{
			sName.Format("%s", Predefined[nTmp1].pszFunc);
			break;
		}
	}	
	return sName;
}

BOOL CSeqEditorDlg::UpdateSubStep()
{

	//	m_ctrlSubGrid.Refresh();
	
	UINT nRow = 0;
	CString sTmp	 = _T("");;
	//+add kwmoon 080819
	CString sSubStep   = _T("");
	CString sErrorMsg1 = _T("");
	CString sErrorMsg2 = _T("");
    char *stop;             

	int nSelNo = -1;

	//+add kwmoon 080809
	int nArgumentNo	  	   = 0;
	int nI2CfirstArgument  = 0;

    //+change PSH 080902
//	int nCount = m_CurFunc.GetCount();
	int nCount = m_ctrlSubList.GetItemCount(); //m_ctrlSubGrid.GetRows() - 1;
	DeleteFunctionList();

	for(int nCnt = 0; nCnt < nCount; nCnt++)
	{
		CFunction* pFunc = new CFunction();
		
		//+change kwmoon 080819
	//	sTmp = m_ctrlSubGrid.GetTextMatrix(nRow, 0);
	//	sTmp.TrimLeft(); sTmp.TrimRight(); sTmp.MakeLower();
	//	if(sTmp.IsEmpty()) return FALSE;

		sSubStep = m_ctrlSubList.GetItemText(nRow, 0);
		sSubStep.TrimLeft(); sSubStep.TrimRight(); sSubStep.MakeLower();
		if(sSubStep.IsEmpty()) return FALSE;

		for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
		{
			//+change kwmoon 080502
		//	if(sTmp == Predefined[nTmp].pszFunc)
			//+change kwmoon 080819
		//	if(sTmp.Compare(Predefined[nTmp].pszFunc) == NULL)
			if(sSubStep.Compare(Predefined[nTmp].pszFunc) == NULL)
			{
				nSelNo = nTmp;
				break;
			}
		}	
		if(nSelNo == -1) return FALSE;

		pFunc->m_pFunc = Predefined[nSelNo].pFunc;
		
		pFunc->m_strCommant = m_ctrlSubList.GetItemText(nRow, 11); //m_ctrlSubList.SetItemText(nTmp, 11, pFunc->m_strCommant);
		int nCol = 1;
		POSITION Pos = pFunc->m_ArgumentList.GetHeadPosition();
		
		//+add kwmoon 080809
		nArgumentNo	  	   = 0;
		nI2CfirstArgument  = 0;
				
		for(int nTmp =0; Predefined[nSelNo].achPara[nTmp] != TNULL; nTmp++)
		{
			//+add kwmoon 080809
			nArgumentNo++;
			
			if(Predefined[nSelNo].achPara[nTmp] == INTEGER)
			{
				Argument* pArgument = new Argument;
				sTmp = m_ctrlSubList.GetItemText(nRow, nCol); //m_ctrlSubGrid.GetTextMatrix(nRow,nCol );
				sTmp.TrimLeft();
				sTmp.TrimRight();

			    //+add & change PSH 080902
				sTmp.MakeLower(); 
				if(sTmp.Find("0x") != -1)
				{
					pArgument->Integer = (int)strtol(sTmp, &stop, 16);

				}
				else
				{
					pArgument->Integer = atoi(sTmp);
				}
				
				//+add kwmoon 080819
				if(sSubStep.Compare("send_i2c_command") == NULL)
				{
					if(nArgumentNo == 2)
					{
						if(sTmp.Compare("i2c_type")			   == NULL) pArgument->Integer = 101;
						else if(sTmp.Compare("i2c_cmd")		   == NULL) pArgument->Integer = 102;
						else if(sTmp.Compare("i2c_read")	   == NULL) pArgument->Integer = 103;
						else if(sTmp.Compare("i2c_chk_value")  == NULL) pArgument->Integer = 104;
						else if(sTmp.Compare("i2c_chk_maxmin") == NULL) pArgument->Integer = 105;
						else if(sTmp.Compare("i2c_chk_limit")  == NULL) pArgument->Integer = 106;
						else if(sTmp.Compare("i2c_2b_read")	   == NULL) pArgument->Integer = 107;
						else if(sTmp.Compare("i2c_2b_write")   == NULL) pArgument->Integer = 108;
						else if(sTmp.Compare("i2c_channel")    == NULL) pArgument->Integer = 109;
						//+add kwmoon 080911
						else if(sTmp.Compare("i2c_chk_ver")    == NULL) pArgument->Integer = 112;
						else
						{
							sErrorMsg1.LoadString(ERROR_I2C_KEYWORD);
							sErrorMsg2.Format("%s (%s)",sErrorMsg1,sTmp);
							AfxMessageBox(sErrorMsg2); return FALSE;
						}
						nI2CfirstArgument = pArgument->Integer;
					}
					else if((nArgumentNo == 4) && (nI2CfirstArgument == 101))
					{
						if(sTmp.Compare("ddc_2ab")		   == NULL) pArgument->Integer = 110;
						else if(sTmp.Compare("ddc_2b")	   == NULL) pArgument->Integer = 111;
						else
						{
							sErrorMsg1.LoadString(ERROR_I2C_KEYWORD);
							sErrorMsg2.Format("%s (%s)",sErrorMsg1,sTmp);
							AfxMessageBox(sErrorMsg2); return FALSE;
						}
					}
				}
				//-
				
				pFunc->m_ArgumentList.AddTail(pArgument);
				nCol++;
			}
			else if((Predefined[nSelNo].achPara[nTmp] == STRING))
			{
				Argument* pArgument = new Argument;
				sTmp = m_ctrlSubList.GetItemText(nRow, nCol); //m_ctrlSubGrid.GetTextMatrix(nRow, nCol);
				sTmp.TrimLeft();
				sTmp.TrimLeft();
				if(sTmp.GetAt(0) == '"') sTmp.Delete(0,1);
				if(sTmp.GetAt(sTmp.GetLength() - 1) == '"')sTmp.Delete(sTmp.GetLength() - 1, 1);
			
				strcpy(pArgument->String, sTmp);
				pFunc->m_ArgumentList.AddTail(pArgument);
				nCol++;
			}
			//+ 2007.10.10 Add BY USY
			else if(Predefined[nSelNo].achPara[nTmp] == REAL)
			{
				Argument* pArgument = new Argument;
				sTmp = m_ctrlSubList.GetItemText(nRow, nCol); //m_ctrlSubGrid.GetTextMatrix(nRow,nCol );
				sTmp.TrimLeft();
				sTmp.TrimRight();
				pArgument->Double = atof(sTmp);
				pFunc->m_ArgumentList.AddTail(pArgument);
				nCol++;
			}
			//-
		}
			m_CurFunc.AddTail(pFunc);
			nRow++;
	}

//	m_ctrlSubGrid.Refresh();
//	
//	UINT nRow = 1;
//	CString sTmp	 = _T("");;
//	//+add kwmoon 080819
//	CString sSubStep   = _T("");
//	CString sErrorMsg1 = _T("");
//	CString sErrorMsg2 = _T("");
//    char *stop;             
//
//	int nSelNo = -1;
//
//	//+add kwmoon 080809
//	int nArgumentNo	  	   = 0;
//	int nI2CfirstArgument  = 0;
//
//    //+change PSH 080902
////	int nCount = m_CurFunc.GetCount();
//	int nCount = m_ctrlSubGrid.GetRows() - 1;
//	DeleteFunctionList();
//
//	for(int nCnt = 0; nCnt < nCount; nCnt++)
//	{
//		CFunction* pFunc = new CFunction();
//		
//		//+change kwmoon 080819
//	//	sTmp = m_ctrlSubGrid.GetTextMatrix(nRow, 0);
//	//	sTmp.TrimLeft(); sTmp.TrimRight(); sTmp.MakeLower();
//	//	if(sTmp.IsEmpty()) return FALSE;
//
//		sSubStep = m_ctrlSubGrid.GetTextMatrix(nRow, 0);
//		sSubStep.TrimLeft(); sSubStep.TrimRight(); sSubStep.MakeLower();
//		if(sSubStep.IsEmpty()) return FALSE;
//
//		for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
//		{
//			//+change kwmoon 080502
//		//	if(sTmp == Predefined[nTmp].pszFunc)
//			//+change kwmoon 080819
//		//	if(sTmp.Compare(Predefined[nTmp].pszFunc) == NULL)
//			if(sSubStep.Compare(Predefined[nTmp].pszFunc) == NULL)
//			{
//				nSelNo = nTmp;
//				break;
//			}
//		}	
//		if(nSelNo == -1) return FALSE;
//
//		pFunc->m_pFunc = Predefined[nSelNo].pFunc;
//		
//		int nCol = 1;
//		POSITION Pos = pFunc->m_ArgumentList.GetHeadPosition();
//		
//		//+add kwmoon 080809
//		nArgumentNo	  	   = 0;
//		nI2CfirstArgument  = 0;
//				
//		for(int nTmp =0; Predefined[nSelNo].achPara[nTmp] != TNULL; nTmp++)
//		{
//			//+add kwmoon 080809
//			nArgumentNo++;
//			
//			if(Predefined[nSelNo].achPara[nTmp] == INTEGER)
//			{
//				Argument* pArgument = new Argument;
//				sTmp = m_ctrlSubGrid.GetTextMatrix(nRow,nCol );
//				sTmp.TrimLeft();
//				sTmp.TrimRight();
//
//			    //+add & change PSH 080902
//				sTmp.MakeLower(); 
//				if(sTmp.Find("0x") != -1)
//				{
//					pArgument->Integer = (int)strtol(sTmp, &stop, 16);
//
//				}
//				else
//				{
//					pArgument->Integer = atoi(sTmp);
//				}
//				
//				//+add kwmoon 080819
//				if(sSubStep.Compare("send_i2c_command") == NULL)
//				{
//					if(nArgumentNo == 2)
//					{
//						if(sTmp.Compare("i2c_type")			   == NULL) pArgument->Integer = 101;
//						else if(sTmp.Compare("i2c_cmd")		   == NULL) pArgument->Integer = 102;
//						else if(sTmp.Compare("i2c_read")	   == NULL) pArgument->Integer = 103;
//						else if(sTmp.Compare("i2c_chk_value")  == NULL) pArgument->Integer = 104;
//						else if(sTmp.Compare("i2c_chk_maxmin") == NULL) pArgument->Integer = 105;
//						else if(sTmp.Compare("i2c_chk_limit")  == NULL) pArgument->Integer = 106;
//						else if(sTmp.Compare("i2c_2b_read")	   == NULL) pArgument->Integer = 107;
//						else if(sTmp.Compare("i2c_2b_write")   == NULL) pArgument->Integer = 108;
//						else if(sTmp.Compare("i2c_channel")    == NULL) pArgument->Integer = 109;
//						//+add kwmoon 080911
//						else if(sTmp.Compare("i2c_chk_ver")    == NULL) pArgument->Integer = 112;
//						else
//						{
//							sErrorMsg1.LoadString(ERROR_I2C_KEYWORD);
//							sErrorMsg2.Format("%s (%s)",sErrorMsg1,sTmp);
//							AfxMessageBox(sErrorMsg2); return FALSE;
//						}
//						nI2CfirstArgument = pArgument->Integer;
//					}
//					else if((nArgumentNo == 4) && (nI2CfirstArgument == 101))
//					{
//						if(sTmp.Compare("ddc_2ab")		   == NULL) pArgument->Integer = 110;
//						else if(sTmp.Compare("ddc_2b")	   == NULL) pArgument->Integer = 111;
//						else
//						{
//							sErrorMsg1.LoadString(ERROR_I2C_KEYWORD);
//							sErrorMsg2.Format("%s (%s)",sErrorMsg1,sTmp);
//							AfxMessageBox(sErrorMsg2); return FALSE;
//						}
//					}
//				}
//				//-
//				
//				pFunc->m_ArgumentList.AddTail(pArgument);
//				nCol++;
//			}
//			else if((Predefined[nSelNo].achPara[nTmp] == STRING))
//			{
//				Argument* pArgument = new Argument;
//				sTmp = m_ctrlSubGrid.GetTextMatrix(nRow, nCol);
//				sTmp.TrimLeft();
//				sTmp.TrimLeft();
//				if(sTmp.GetAt(0) == '"') sTmp.Delete(0,1);
//				if(sTmp.GetAt(sTmp.GetLength() - 1) == '"')sTmp.Delete(sTmp.GetLength() - 1, 1);
//			
//				strcpy(pArgument->String, sTmp);
//				pFunc->m_ArgumentList.AddTail(pArgument);
//				nCol++;
//			}
//			//+ 2007.10.10 Add BY USY
//			else if(Predefined[nSelNo].achPara[nTmp] == REAL)
//			{
//				Argument* pArgument = new Argument;
//				sTmp = m_ctrlSubGrid.GetTextMatrix(nRow,nCol );
//				sTmp.TrimLeft();
//				sTmp.TrimRight();
//				pArgument->Double = atof(sTmp);
//				pFunc->m_ArgumentList.AddTail(pArgument);
//				nCol++;
//			}
//			//-
//		}
//			m_CurFunc.AddTail(pFunc);
//			nRow++;
//	}
	return TRUE;
}

void CSeqEditorDlg::DeleteFunctionList()
{
	POSITION Pos = m_CurFunc.GetHeadPosition();
	while (Pos != NULL) 
	{
		delete m_CurFunc.GetNext(Pos);
	}
	m_CurFunc.RemoveAll();

}

void CSeqEditorDlg::SortSubStep(CStep* pStep)
{
	if(pStep == NULL) return;
	
	POSITION pos = m_CurFunc.GetHeadPosition();
	while(pos != NULL)
	{       
		delete m_CurFunc.GetNext(pos);
	}
	m_CurFunc.RemoveAll();

	POSITION FuncPos = pStep->m_InstructionList.GetHeadPosition();
	
	while (FuncPos != NULL) 
	{
		CFunction* pOrgFunc = pStep->m_InstructionList.GetNext(FuncPos);
		CFunction* pCopyFunc = new CFunction();

		pCopyFunc->m_pFunc = pOrgFunc->m_pFunc;
		pCopyFunc->m_strCommant = pOrgFunc->m_strCommant;

		POSITION ArgPos = pOrgFunc->m_ArgumentList.GetHeadPosition();

		while(ArgPos != NULL)
		{
			Argument* pOrgArgument = pOrgFunc->m_ArgumentList.GetNext(ArgPos);
			Argument* pCopyArgument = new Argument;

			memcpy(pCopyArgument->String, pOrgArgument->String, sizeof(pOrgArgument->String));
			pCopyFunc->m_ArgumentList.AddTail(pCopyArgument);
		}
		m_CurFunc.AddTail(pCopyFunc);
	}
}

//+ 2007.9.13 Add BY USY
void CSeqEditorDlg::CopyMainStep(CStep* pOrgStep, CStep* pCopyStep, int nTargetStepNo)
{
	//Copy Main Step Value
	pCopyStep->SetStepNo(nTargetStepNo);
	pCopyStep->SetStepName(pOrgStep->GetStepName());
	pCopyStep->SetCurrentSource(pOrgStep->GetCurrentSource());
	pCopyStep->SetFuncType(pOrgStep->GetFuncType());
	pCopyStep->SetTestType(pOrgStep->GetTestType());
	pCopyStep->SetStepType(pOrgStep->GetStepType());
	if(pCopyStep->GetStepType() == CHECK_FUNCTION)
	{
		pCopyStep->SetFuncName(pOrgStep->GetFuncName());
		pCopyStep->SetFunction(pOrgStep->GetFunction());
		if(pCopyStep->GetFuncType() == MEAS_BOOL)
		{
			pCopyStep->SetNominal(pOrgStep->GetNominal_Bool());
		}
		else if(pCopyStep->GetFuncType() == MEAS_DOUBLE)
		{
			pCopyStep->SetNominal(pOrgStep->GetNominal_float());
			pCopyStep->SetLowLimit(pOrgStep->GetLowLimit());
			pCopyStep->SetHighLimit(pOrgStep->GetHighLimit());
			pCopyStep->SetUnit(pOrgStep->GetUnit());
		}
		//+ 2007.10.10 Add BY USY
		pCopyStep->m_nAudioSource = pOrgStep->m_nAudioSource;
		//-
		for(int nTmp = 0; nTmp < 2; nTmp++)
		{
			pCopyStep->m_nLeftFreq[nTmp] = pOrgStep->m_nLeftFreq[nTmp];
			pCopyStep->m_nRightFreq[nTmp] = pOrgStep->m_nRightFreq[nTmp];
			pCopyStep->m_nLeftLevel[nTmp] = pOrgStep->m_nLeftLevel[nTmp];
			pCopyStep->m_nRightLevel[nTmp] = pOrgStep->m_nRightLevel[nTmp];
		}
		pCopyStep->m_nAudioMargin = pOrgStep->m_nAudioMargin;		
	}
	POSITION OrgPos = pOrgStep->m_InstructionList.GetHeadPosition();
	while (OrgPos != NULL) 
	{
		CFunction* pOrgFunc = pOrgStep->m_InstructionList.GetNext(OrgPos);
		CFunction* pCopyFunc = new CFunction();

		pCopyFunc->m_pFunc = pOrgFunc->m_pFunc;

		POSITION ArgPos = pOrgFunc->m_ArgumentList.GetHeadPosition();

		while(ArgPos != NULL)
		{
			Argument* pOrgArgument = pOrgFunc->m_ArgumentList.GetNext(ArgPos);
			Argument* pCopyArgument = new Argument;

			memcpy(pCopyArgument->String, pOrgArgument->String, sizeof(pOrgArgument->String));
			pCopyFunc->m_ArgumentList.AddTail(pCopyArgument);
		}
		pCopyStep->m_InstructionList.AddTail(pCopyFunc);
	}
}
void CSeqEditorDlg::CopySubStep(CFunction* pOrgFunc, CFunction* pCopyFunc)
{
	pCopyFunc->m_pFunc = pOrgFunc->m_pFunc;

	POSITION ArgPos = pOrgFunc->m_ArgumentList.GetHeadPosition();
	while(ArgPos != NULL)
	{
		Argument* pOrgArgument = pOrgFunc->m_ArgumentList.GetNext(ArgPos);
		Argument* pCopyArgument = new Argument;
		memcpy(pCopyArgument->String, pOrgArgument->String, sizeof(pOrgArgument->String));
		pCopyFunc->m_ArgumentList.AddTail(pCopyArgument);
	}
}
//-

BOOL CSeqEditorDlg::SaveSeqFile(CString sSeqPath)
{
	int nTotalCnt = StepList.GetCount();
	if(nTotalCnt <1) 
	{
		AfxMessageBox("Add Step First!");
		return FALSE;
	}

	CStdioFile pFile;
	CFileException FileExcept;
	CString sTmp;
	CStringArray aTmp;
	aTmp.RemoveAll();

	if(FileExists(sSeqPath)) ::DeleteFile(sSeqPath);

	//+change kwmoon 080910
//	if(!pFile.Open(sSeqPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate |CFile::shareExclusive   , &FileExcept)) 
	if(!pFile.Open(sSeqPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate |CFile::shareDenyNone   , &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	
	POSITION Position = NULL;
//	CStep* pStep = new CStep();
	int nTmp;

	Position = StepList.GetHeadPosition();
	if(StepList.GetCount() > 0)
	{
		nTmp = 1;
		while (Position) 
		{
			CStep* pStep = StepList.GetNext(Position);
			pFile.WriteString(pStep->m_strCommant);

			//Step Header
			sTmp = MakeStepHeader(pStep);
			pFile.WriteString(sTmp);
			//Sub Step
			MakeSubStep(pStep, aTmp);
			for(int nTmp1 = 0;nTmp1 <aTmp.GetSize(); nTmp1++ )
			{
				sTmp = aTmp.GetAt(nTmp1);
				if ((sTmp.Find("//") < 0)&&(nTmp1 > 0))
					pFile.WriteString("\n");
				pFile.WriteString(sTmp);
			}

			if (pStep->m_strInCommant.GetLength() < 2)
			{
				pFile.WriteString("\n");
			}
			else
			{
				pFile.WriteString(pStep->m_strInCommant);
			}
			//Step Tail
			sTmp = MakeStepTail(pStep);
			pFile.WriteString(sTmp);

			nTmp++;
		}
	}
	pFile.Close();
	return TRUE;
}

CString CSeqEditorDlg::MakeStepHeader(CStep* pStep)
{
	CString sHeader = _T("");
	CString sProcess, sSource,sFunc, sTarget, sLowlinit, sHighlinit,sUnit;

	

	if(pStep->m_nStepType == PROCESS_FUNCTION)
	{
		sHeader.Format("step(%d, \"%s\"):process()\n{\n", 
									pStep->m_nStep,
									pStep->m_sName	);
	}
	else if(pStep->m_nStepType == CHECK_FUNCTION)
	{
		//Criterion
		if(pStep->m_nFuncType == MEAS_BOOL)
		{
			if(pStep->GetNominal_Bool() == TRUE)	sTarget = "TRUE";
			else if(pStep->GetNominal_Bool() == FALSE) sTarget = "FALSE";
			sLowlinit = sHighlinit = sUnit = "";
		}
		else if(pStep->m_nFuncType != MEAS_BOOL)
		{
			sTarget.Format("%5.1f", pStep->m_fNominal);
			sLowlinit.Format("%5.1f", pStep->m_fLowLimit);
			sHighlinit.Format("%5.1f", pStep->m_fHighLimit);
			sUnit.Format("%s", pStep->m_sUnit);
		}
		sHeader.Format("step(%d, \"%s\"):check(\"%s\", %s, %s, %s, %s,\"%s\")\n{\n", 
//		sHeader.Format("step(%d, \"%s\"):check(\"%s\",\"%s\", %s, %s, %s, %s,\"%s\")\n{\n", 
									pStep->m_nStep,
									pStep->m_sName,
									pStep->m_strCurSrc,
								//	pStep->m_strNextSrc,
									pStep->m_strFuncName,
									sTarget,
									sLowlinit,
									sHighlinit,
									sUnit);
	}

	return sHeader;
}

BOOL CSeqEditorDlg::MakeSubStep(CStep* pStep, CStringArray& aArray)
{
	CString sTmp, sSubStep;
	int nSelNo = 0;

	//+add kwmoon 080809
	int nArgumentNo	  	   = 0;
	int nI2CfirstArgument  = 0;
	
	sSubStep = _T("");
	POSITION Pos = pStep->m_InstructionList.GetHeadPosition();
	SortSubStep(pStep);
	aArray.RemoveAll();

	CString strTmp;

	if(pStep->m_nStepType == PROCESS_FUNCTION)
	{
		Pos = m_CurFunc.GetHeadPosition();
		while(Pos != NULL)
		{
			CFunction* pCurFunc = m_CurFunc.GetNext(Pos);
			for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
			{
				if(pCurFunc->m_pFunc == Predefined[nTmp].pFunc)
				{
					nSelNo = nTmp;
					break;
				}
			}
			if(nSelNo == -1) return FALSE;
			POSITION ArgPos = pCurFunc->m_ArgumentList.GetHeadPosition();
			
			//sSubStep.Format("\t %s", Predefined[nSelNo].pszFunc);
			sSubStep = pCurFunc->m_strCommant;
			//strTmp.Format("\t %s", Predefined[nSelNo].pszFunc);
			strTmp.Format("\t %s", Predefined[nSelNo].pszFunc);
			sSubStep += strTmp;


			nArgumentNo	  	   = 0;
			nI2CfirstArgument  = 0;

			for(int nTmp = 0; Predefined[nSelNo].achPara[nTmp] != TNULL; nTmp++)
			{
				nArgumentNo++;
				if(Predefined[nSelNo].achPara[nTmp] == LP)
				{
					sSubStep += "(";
				}
				else if(Predefined[nSelNo].achPara[nTmp] == RP)
				{
					sSubStep += ")";
				}
				else if(Predefined[nSelNo].achPara[nTmp] == COMMA)
				{
					sSubStep += ",";
				}

				else if(Predefined[nSelNo].achPara[nTmp] == INTEGER)
				{
					
					if(ArgPos  == NULL) break;
					else 
					{
//						Argument* pArgument = new Argument;
						Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);	
						sTmp.Format("%d ", pArgument->Integer);
						//+add psh 090702
						if(sSubStep.Mid(2,16).Compare("send_i2c_command") == NULL)
						{
							if(nArgumentNo == 2)
							{
								switch(pArgument->Integer)
								{
									case 101 : sTmp = "I2C_TYPE";		break;
									case 102 : sTmp = "I2C_CMD";		break;
									case 103 : sTmp = "I2C_READ";		break;
									case 104 : sTmp = "I2C_CHK_VALUE";	break;
									case 105 : sTmp = "I2C_CHK_MAXMIN";	break;
									case 106 : sTmp = "I2C_CHK_LIMIT";	break;
									case 107 : sTmp = "I2C_2B_READ";	break;
									case 108 : sTmp = "I2C_2B_WRITE";	break;
									case 109 : sTmp = "I2C_CHANNEL";	break;
									//+add kwmoon 080911
									case 112 : sTmp = "I2C_CHK_VER";	break;
								}
								nI2CfirstArgument = pArgument->Integer;
							}
							else if(nArgumentNo == 4)
							{
								if(nI2CfirstArgument == I2C_TYPE)
								{
									switch(pArgument->Integer)
									{
										case 110 : sTmp = "DDC_2AB";			break;
										case 111 : sTmp = "DDC_2B";			break;
									}
								}
								else
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
							}
							else if(nArgumentNo == 6)
							{
								if(nI2CfirstArgument != I2C_CHK_VALUE)
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
							}
						}
						//-

						sSubStep += sTmp;
					}
				}
				else if((Predefined[nSelNo].achPara[nTmp] == STRING))
				{
					if(ArgPos  == NULL) break;
					else
					{
//						Argument* pArgument = new Argument;
						Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
						sTmp.Format("\"%s\" ", pArgument->String);
						sSubStep += sTmp;
					}
				}
				//+ 2007.10.10 Add BY USY
				else if((Predefined[nSelNo].achPara[nTmp] == REAL))
				{
					if(ArgPos  == NULL) break;
					else
					{
//						Argument* pArgument = new Argument;
						Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
						sTmp.Format("%.1f ", pArgument->Double);
						sSubStep += sTmp;
					}
				}
				//-
			}
			aArray.Add(sSubStep);
			
			
		} 
		
	}
	else if(pStep->m_nStepType == CHECK_FUNCTION)
	{
		Pos = m_CurFunc.GetHeadPosition();

/*		if(nTmp == 3)
		{
			if(pStep->m_nAudioSource > 0)
			{
				sTmp.Format("\t check_audio (%d ,%d , %d , %d , %d, %d , \"\" )\n\n",pStep->m_nAudioSource,
						pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0],
						pStep->m_nAudioMargin);
				aArray.Add(sTmp);
			}
		}
		else
		{
*/			while(Pos != NULL)
			{
				CFunction* pCurFunc = m_CurFunc.GetNext(Pos);
				for(int nTmp1 = 0; Predefined[nTmp1].pszFunc != NULL; nTmp1++)
				{
					if(pCurFunc->m_pFunc == Predefined[nTmp1].pFunc)
					{
						nSelNo = nTmp1;
						break;
					}
				}
				POSITION ArgPos = pCurFunc->m_ArgumentList.GetHeadPosition();
				
				sSubStep = pCurFunc->m_strCommant;
				//strTmp.Format("\t %s", Predefined[nSelNo].pszFunc);
				strTmp.Format("\t %s", Predefined[nSelNo].pszFunc);
				sSubStep += strTmp;
				//sSubStep.Format("\t %s", Predefined[nSelNo].pszFunc);
			
				//+add kwmoon 080809
				nArgumentNo	  	   = 0;
				nI2CfirstArgument  = 0;
				
				for(int nTmp1 = 0; Predefined[nSelNo].achPara[nTmp1] != TNULL; nTmp1++)
				{
					nArgumentNo++;

//					Argument* pArgument = new Argument;
					if(Predefined[nSelNo].achPara[nTmp1] == LP)
					{
						sSubStep += "(";
					}
					else if(Predefined[nSelNo].achPara[nTmp1] == RP)
					{
						sSubStep += ")";
					}
					else if(Predefined[nSelNo].achPara[nTmp1] == COMMA)
					{
						sSubStep += ",";
					}
					else if(Predefined[nSelNo].achPara[nTmp1] == INTEGER)
					{
						if(ArgPos  == NULL) break;
						else 
						{
							Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);	
							sTmp.Format("%d ", pArgument->Integer);

							//+add kwmoon 080809
							if(sSubStep.Mid(2,16).Compare("send_i2c_command") == NULL)
							{
								if(nArgumentNo == 2)
								{
									switch(pArgument->Integer)
									{
										case 101 : sTmp = "I2C_TYPE";		break;
										case 102 : sTmp = "I2C_CMD";		break;
										case 103 : sTmp = "I2C_READ";		break;
										case 104 : sTmp = "I2C_CHK_VALUE";	break;
										case 105 : sTmp = "I2C_CHK_MAXMIN";	break;
										case 106 : sTmp = "I2C_CHK_LIMIT";	break;
										case 107 : sTmp = "I2C_2B_READ";	break;
										case 108 : sTmp = "I2C_2B_WRITE";	break;
										case 109 : sTmp = "I2C_CHANNEL";	break;
										//+add kwmoon 080911
										case 112 : sTmp = "I2C_CHK_VER";	break;
									}
									nI2CfirstArgument = pArgument->Integer;
								}
								else if(nArgumentNo == 4)
								{
									if(nI2CfirstArgument == I2C_TYPE)
									{
										switch(pArgument->Integer)
										{
											case 110 : sTmp = "DDC_2AB";			break;
											case 111 : sTmp = "DDC_2B";			break;
										}
									}
									else
									{
										sTmp.Format("0x%02x ", pArgument->Integer);
									}
								}
								else if(nArgumentNo == 6)
								{
									if(nI2CfirstArgument != I2C_CHK_VALUE)
									{
										sTmp.Format("0x%02x ", pArgument->Integer);
									}
								}
							}
							//-
							//+add psh 090703
							if(sSubStep.Mid(2,15).Compare("set_dimmingtest") == NULL)
							{
								if(nArgumentNo == 4)
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
								else if(nArgumentNo == 6)
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
							}//-

							sSubStep += sTmp;
						}
					}
					else if((Predefined[nSelNo].achPara[nTmp1] == STRING))
					{
						if(ArgPos  == NULL) break;
						else 
						{
							Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
							sTmp.Format("\"%s\" ", pArgument->String);
							sSubStep += sTmp;
						}
					}
					//+ 2007.10.10 Add BY USY
					else if((Predefined[nSelNo].achPara[nTmp1] == REAL))
					{
						if(ArgPos  == NULL) break;
						else 
						{
							Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
							sTmp.Format("%.1f ", pArgument->Double);
							sSubStep += sTmp;
						}
					}
					//-
				}
				aArray.Add(sSubStep);
			} 

//		}

	}
	return TRUE;
}

CString CSeqEditorDlg::MakeStepTail(CStep* pStep)
{
	CString sTail = _T("");
	sTail.Format("}\n\n");
	return sTail;
}

void CSeqEditorDlg::ChangeRowColor(int nRow, int nGridType)
{
	int nTotalRow,  nTotalCol;

	CString szStepType		= _T("");
	CString szCurrentSource = _T("");

	////===============
	//// MainStep Grid
	////===============
	//if(nGridType == 0)
	//{
	//	if(nRow > 0)
	//	{
	//		//======================
	//		//	Clear Selected Rows
	//		//======================
	//		for(nTotalRow = 1; nTotalRow < m_ctrlMainGrid.GetRows(); nTotalRow++)
	//		{
	//			m_ctrlMainGrid.SetRow(nTotalRow);

	//			for(nTotalCol = 0; nTotalCol < m_ctrlMainGrid.GetCols(); nTotalCol++)
	//			{
	//				m_ctrlMainGrid.SetCol(nTotalCol);
	//				
	//				szStepType		=  m_ctrlMainGrid.GetTextMatrix(nTotalRow,MAIN_STEP_TYPE_COL);
	//				
	//				if(szStepType.Compare("CHECK") == NULL)
	//				{
	//					szCurrentSource =  m_ctrlMainGrid.GetTextMatrix(nTotalRow,MAIN_SOURCE_COL);
	//					
	//					if(nTotalCol == 1)
	//					{
	//						ChangeColumnColor(MAIN_GRID,szCurrentSource);
	//					}
	//					else
	//					{
	//						m_ctrlMainGrid.SetCellBackColor(RGB(255,255,255));
	//					}
	//				}
	//				else
	//				{
	//					m_ctrlMainGrid.SetCellBackColor(RGB(255,255,255));
	//				}
	//				m_ctrlMainGrid.SetCellForeColor(RGB(0,0,0));
	//			}
	//		}

	//		//========================
	//		//	Highlight Current Row
	//		//========================
	//		m_ctrlMainGrid.SetRow(nRow);
	//		
	//		for(nTotalCol = 0; nTotalCol < m_ctrlMainGrid.GetCols(); nTotalCol++)
	//		{
	//			m_ctrlMainGrid.SetCol(nTotalCol);
	//			m_ctrlMainGrid.SetCellBackColor(RGB(0, 99,   147 ));
	//			m_ctrlMainGrid.SetCellForeColor(RGB(255,255,255));
	//		}
	//	}
	//}
	////===============
	//// Sub Step Grid
	////===============
	//else if(nGridType == 1)
	//{
	//	if(nRow > 0)
	//	{
	//		//======================
	//		//	Clear Selected Rows
	//		//======================
	//		for(nTotalRow = 1; nTotalRow < m_ctrlSubGrid.GetRows(); nTotalRow++)
	//		{
	//			m_ctrlSubGrid.SetRow(nTotalRow);

	//			for(nTotalCol = 0; nTotalCol < 1; nTotalCol++)
	//			{
	//				m_ctrlSubGrid.SetCol(nTotalCol);
	//				m_ctrlSubGrid.SetCellBackColor(RGB(255,255,255));
	//				m_ctrlSubGrid.SetCellForeColor(RGB(0,0,0));
	//			}
	//		}

	//		//========================
	//		//	Highlight Current Row
	//		//========================
	//		m_ctrlSubGrid.SetRow(nRow);
	//		
	//		for(nTotalCol = 0; nTotalCol < 1; nTotalCol++)
	//		{
	//			m_ctrlSubGrid.SetCol(nTotalCol);
	//			m_ctrlSubGrid.SetCellBackColor(RGB(0, 99,   147 ));
	//			m_ctrlSubGrid.SetCellForeColor(RGB(255,255,255));
	//		}
	//	}
	//}
}

void CSeqEditorDlg::SetAudioSpec(int nRow, int nCol)
{
	//+change kwmoon 080809
//	if((nCol != 9)&&(nCol != 10)&&(nCol != 11)) return;
	if((nCol != 10) && (nCol != 11)) return;

	CSetAudioSpecDlg dlg;
	CString sTmp;

	//+change kwmoon 080809
//	dlg.m_nAudioSource = (int)m_ctrlMainGrid.GetValueMatrix(nRow, 9);
	//dlg.m_nAudioSource = (int)m_ctrlMainGrid.GetValueMatrix(nRow, MAIN_AUD_SOURCE_COL);

	sTmp = m_ctrlStepMainList.GetItemText(nRow, MAIN_AUD_SOURCE_COL);
	dlg.m_nAudioSource = _ttoi(sTmp);
	//+change kwmoon 080809
//	dlg.m_nAudioMargin = (int)m_ctrlMainGrid.GetValueMatrix(nRow, 11);
	//dlg.m_nAudioMargin = (int)m_ctrlMainGrid.GetValueMatrix(nRow, MAIN_AUD_MARGIN_COL);
	sTmp = m_ctrlStepMainList.GetItemText(nRow, MAIN_AUD_MARGIN_COL);
	dlg.m_nAudioMargin = _ttoi(sTmp);

	//+change kwmoon 080809
//	sTmp = m_ctrlMainGrid.GetTextMatrix(nRow, 10);
	//sTmp = m_ctrlMainGrid.GetTextMatrix(nRow, MAIN_AUD_SPEC_COL);
	sTmp = m_ctrlStepMainList.GetItemText(nRow, MAIN_AUD_SPEC_COL);	
	dlg.m_nLeftFreq = atoi(sTmp.Left(sTmp.Find(",", 0)));

	sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	dlg.m_nRightFreq = atoi(sTmp.Left(sTmp.Find(",", 0)));
	
	sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	dlg.m_nLeftLevel = atoi(sTmp.Left(sTmp.Find(",", 0)));
	
	sTmp.Delete(0, sTmp.Find(",", 0)  + 1);
	dlg.m_nRightLevel = atoi(sTmp);
	
	if(dlg.DoModal() == IDOK)
	{
		//+change kwmoon 080809
		sTmp.Format("%d", dlg.m_nAudioSource);
		//+change kwmoon 080809
	//	m_ctrlMainGrid.SetTextMatrix(nRow, 9, sTmp);
		m_ctrlStepMainList.SetItem(nRow, MAIN_AUD_SOURCE_COL, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //
		//m_ctrlMainGrid.SetTextMatrix(nRow, MAIN_AUD_SOURCE_COL, sTmp);

		sTmp.Format("%d", dlg.m_nAudioMargin);
		//+change kwmoon 080809
	//	m_ctrlMainGrid.SetTextMatrix(nRow, 11, sTmp);
		m_ctrlStepMainList.SetItem(nRow, MAIN_AUD_MARGIN_COL, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //
		//m_ctrlMainGrid.SetTextMatrix(nRow, MAIN_AUD_MARGIN_COL, sTmp);

		sTmp.Format("%d, %d, %d, %d", dlg.m_nLeftFreq, dlg.m_nRightFreq, dlg.m_nLeftLevel, dlg.m_nRightLevel );
		//+change kwmoon 080809
	//	m_ctrlMainGrid.SetTextMatrix(nRow, 10, sTmp);
		m_ctrlStepMainList.SetItem(nRow, MAIN_AUD_SPEC_COL, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //
		//m_ctrlMainGrid.SetTextMatrix(nRow, MAIN_AUD_SPEC_COL, sTmp);
	}
}
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CSeqEditorDlg message handlers

BOOL CSeqEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//======================
	// Display Model Folder
	//======================
	CString sTmp;
	sTmp.Format("Sequence Editor Dialog  - %s", CurrentSet->sModelFolder);

	SetWindowText(sTmp);
	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);
	
	//===============
	// Set File Path
	//===============
	if(!CurrentSet->sSeqPath.IsEmpty())
	{
		m_strSeqPath.Format("%s", CurrentSet->sSeqPath);
		m_ctrlSeqPathEdit.SetWindowText(m_strSeqPath);
	}

	if(!CurrentSet->sFullSeqPath.IsEmpty())
	{
		m_strTemplatePath.Format("%s", CurrentSet->sFullSeqPath);
		m_ctrlTemplatePathEdit.SetWindowText(m_strTemplatePath);
	}
	
	//=================
	// Init Step Table
	//=================
	InitMainGrid();
	UpdateMainGrid();

	//=====================
	// Init Sub Step Table
	//=====================
	InitSubGrid();

	//=====================
	// Init Template Table
	//=====================
	InitTemplateGrid();
	UpdateTemplateGrid();

	DeleteFunctionList();
	GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, 2500, 200, 150, 23, SWP_ASYNCWINDOWPOS);
	m_cComboSeqEditor.SetWindowPos(NULL, 25100, 100, 250, 20, SWP_ASYNCWINDOWPOS);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeqEditorDlg::OnBtnNewSeq() 
{
	//Seq Path
	m_strSeqPath.Format(_T(""));
	m_ctrlSeqPathEdit.SetWindowText(m_strSeqPath);
	
	//Step List Clear
	DeleteStepList();

	DeleteFunctionList();

	//Grid Control
	InitMainGrid();
	UpdateMainGrid();

	InitSubGrid();
}

void CSeqEditorDlg::OnBtnOpenSeq() 
{
	CFileDialog dlg(TRUE, "seq", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Sequence File(*.seq)|*.seq|",NULL);

	CString sTmp = CurrentSet->sModelFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		if(Prescan(dlg.GetPathName()))
		{
			m_strSeqPath.Format("%s",dlg.GetPathName());
		}
		else m_strSeqPath.Format(_T(""));
		m_ctrlSeqPathEdit.SetWindowText(m_strSeqPath);
		
	
		//Grid Control
		InitMainGrid();
		UpdateMainGrid();	

		InitSubGrid();
	}
}

void CSeqEditorDlg::OnBtnSaveSeq() 
{
	//+add kwmoon 080910
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	
	OnBtnUploadSubStep();
	//-	
	//+change kwmoon 080508
//	OnDblClickMainStepGrid();
	OnClickMainStepGrid();



	m_ctrlSeqPathEdit.GetWindowText(m_strSeqPath);
	if(m_strSeqPath.IsEmpty())
	{
		CFileDialog dlg(FALSE, "seq", NULL, 
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
		"Sequence File(*.seq)|*.seq|",NULL);

		CString sTmp = CurrentSet->sModelFolder;
		dlg.m_ofn.lpstrInitialDir = sTmp;
		m_strSeqPath.Format("%s", dlg.GetPathName());
		m_ctrlSeqPathEdit.SetWindowText(m_strSeqPath);
	}
	for(int nTmp = 0; nTmp < StepList.GetCount(); nTmp++)
	{
		UpdateMainStep(nTmp + 1);
	}
	SaveSeqFile(m_strSeqPath);
}

void CSeqEditorDlg::OnBtnSaveAsSeq() 
{
	//+add kwmoon 080910
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}

	
	//+change kwmoon 080508
//	OnDblClickMainStepGrid();
	OnClickMainStepGrid();
	
	//+add kwmoon 071210
	OnBtnUploadSubStep();
	//-

	CFileDialog dlg(FALSE, "seq", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Sequence File(*.seq)|*.seq|",NULL);

	CString sTmp = CurrentSet->sModelFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK)
	{
		m_strSeqPath.Format("%s", dlg.GetPathName());
		m_ctrlSeqPathEdit.SetWindowText(m_strSeqPath);

		for(int nTmp = 0; nTmp < StepList.GetCount(); nTmp++)
		{
			UpdateMainStep(nTmp + 1);
		}
		SaveSeqFile(m_strSeqPath);
	}
	
}

void CSeqEditorDlg::OnBtnInsertMainStep() 
{
	//int nRow = m_ctrlMainGrid.GetRow();
	//int nCol = m_ctrlMainGrid.GetCol();

	//if(nRow < 1)
	//{
	//	CStep* pStep = new CStep();
	//	pStep->SetStepNo(StepList.GetCount() + 1);
	//	StepList.AddTail(pStep);
	//}
	//else 
	//{
	//	CStep* pStep = new CStep();
	//	pStep->SetStepNo(nRow);
	//	POSITION Pos = StepList.GetHeadPosition();
	//	Pos = StepList.FindIndex(nRow - 1);
	//	StepList.InsertAfter(Pos, pStep);

	//	//Change Step No After Delete Step
	//	for(int nTmp = nRow; nTmp <= StepList.GetCount(); nTmp++)
	//	{
	//		POSITION Pos = StepList.FindIndex(nTmp);
	//		if(Pos != NULL)
	//		{
	//			CStep* pStep = StepList.GetAt(Pos);
	//			pStep->SetStepNo(nTmp + 1);
	//			StepList.SetAt(Pos, pStep);
	//		}
	//	}
	//}

	//UpdateMainGrid();		

	////+ 2007.10.11 Add BY USY
	//if(nRow < 1) m_ctrlMainGrid.SetTopRow(StepList.GetCount());
	//else m_ctrlMainGrid.SetTopRow(nRow );
	////-
}

void CSeqEditorDlg::OnBtnUpMainStep() 
{
	//int nRow = m_ctrlMainGrid.GetRow();
	//if(nRow < 2) return;
	//else
	//{
	//	POSITION Pos = StepList.GetHeadPosition();
	//	POSITION CurPos = StepList.FindIndex(nRow - 1);
	//	POSITION BeforePos = StepList.FindIndex(nRow - 2);
	//	if((CurPos != NULL) && (BeforePos != NULL))
	//	{
	//		CStep* pCurStep = StepList.GetAt(CurPos);
	//		CStep* pBeforeStep = StepList.GetAt(BeforePos);

	//		pCurStep->SetStepNo(nRow - 1);
	//		pBeforeStep->SetStepNo(nRow);
	//		StepList.SetAt(CurPos, pBeforeStep);
	//		StepList.SetAt(BeforePos, pCurStep);
	//	}
	//	UpdateMainGrid();
	//}
	//
	////+ 2007.10.11 Add BY USY
	//m_ctrlMainGrid.SetTopRow(nRow - 1);
	//ChangeRowColor(nRow - 1, 0);
	////-

}

void CSeqEditorDlg::OnBtnDownMainStep() 
{
	//int nRow = m_ctrlMainGrid.GetRow();
	//if(nRow < 1) return;
	//else
	//{
	//	POSITION Pos = StepList.GetHeadPosition();
	//	POSITION CurPos = StepList.FindIndex(nRow - 1);
	//	POSITION AfterPos= StepList.FindIndex(nRow);
	//	if((CurPos != NULL) && (AfterPos != NULL))
	//	{
	//		CStep* pCurStep = StepList.GetAt(CurPos);
	//		CStep* pAfterStep = StepList.GetAt(AfterPos);

	//		pCurStep->SetStepNo(nRow + 1);
	//		pAfterStep->SetStepNo(nRow);
	//		StepList.SetAt(CurPos, pAfterStep);
	//		StepList.SetAt(AfterPos, pCurStep);
	//	}
	//	UpdateMainGrid();
	//}
	////+ 2007.10.11 Add BY USY
	//m_ctrlMainGrid.SetTopRow(nRow + 1);
	//ChangeRowColor(nRow + 1, 0);
	////-
}

void CSeqEditorDlg::OnBtnDeleteMainStep() 
{
	//int nRow = m_ctrlMainGrid.GetRow();
	//if(nRow < 1) return;
	//else
	//{
	//	POSITION Pos = StepList.GetHeadPosition();
	//	POSITION CurPos = StepList.FindIndex(nRow - 1);
	//	if(CurPos != NULL)
	//	{
	//		StepList.RemoveAt(CurPos);

	//		//Change Step No After Delete Step
	//		for(int nTmp = nRow - 1; nTmp <= StepList.GetCount(); nTmp++)
	//		{
	//			POSITION Pos = StepList.FindIndex(nTmp);
	//			if(Pos != NULL)
	//			{
	//				CStep* pStep = StepList.GetAt(Pos);
	//				pStep->SetStepNo(nTmp + 1);
	//				StepList.SetAt(Pos, pStep);
	//			}
	//		}
	//		CStep* pStep = FindStep(nRow);
	//		SortSubStep(pStep);
	//		UpdateSubGrid();
	//		
	//	}
	//	UpdateMainGrid();
	//}
}

void CSeqEditorDlg::OnBtnClearMainStep() 
{
	DeleteStepList();
	DeleteFunctionList();

	//Grid Control
	InitMainGrid();
	UpdateMainGrid();

	InitSubGrid();
}

void CSeqEditorDlg::OnOK() 
{
	POSITION Pos = m_CurFunc.GetHeadPosition();
	while (Pos != NULL) 
	{
		delete m_CurFunc.GetNext(Pos);
	}
	m_CurFunc.RemoveAll();

	DeleteStepList();
	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CSeqEditorDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSeqEditorDlg)
	ON_EVENT(CSeqEditorDlg, IDC_SUB_STEP_GRID, -601 /* DblClick */, OnDblClickSubStepGrid, VTS_NONE)
	ON_EVENT(CSeqEditorDlg, IDC_SUB_STEP_GRID, 43 /* CellChanged */, OnCellChangedSubStepGrid, VTS_I4 VTS_I4)
	ON_EVENT(CSeqEditorDlg, IDC_MAIN_STEP_GRID, 23 /* ValidateEdit */, OnValidateEditMainStepGrid, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CSeqEditorDlg, IDC_MAIN_STEP_GRID, 24 /* AfterEdit */, OnAfterEditMainStepGrid, VTS_I4 VTS_I4)
	ON_EVENT(CSeqEditorDlg, IDC_SUB_STEP_GRID, 24 /* AfterEdit */, OnAfterEditSubStepGrid, VTS_I4 VTS_I4)
	ON_EVENT(CSeqEditorDlg, IDC_SUB_STEP_GRID, -600 /* Click */, OnClickSubStepGrid, VTS_NONE)
	ON_EVENT(CSeqEditorDlg, IDC_SUB_STEP_GRID, 21 /* BeforeEdit */, OnBeforeEditSubStepGrid, VTS_I4 VTS_I4 VTS_PBOOL)
	ON_EVENT(CSeqEditorDlg, IDC_MAIN_STEP_GRID, -600 /* Click */, OnClickMainStepGrid, VTS_NONE)
	ON_EVENT(CSeqEditorDlg, IDC_TEMPLATE_SEQ_GRID, -600 /* Click */, OnClickTemplateSeqGrid, VTS_NONE)
	ON_EVENT(CSeqEditorDlg, IDC_TEMPLATE_SEQ_GRID, -601 /* DblClick */, OnDblClickTemplateSeqGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//+del kwmoon 080508
/*
void CSeqEditorDlg::OnDblClickMainStepGrid() 
{
	int nRow = m_ctrlMainGrid.GetRow();
	int nCol = m_ctrlMainGrid.GetCol();
	if((nRow < 1) || (nCol < 1)) return;

	ChangeRowColor(nRow, 0);
	SetMainGrid4Edit(nRow, nCol);

	CStep* pStep = FindStep(nRow);
	SortSubStep(pStep);
	UpdateSubGrid();
}
*/

void CSeqEditorDlg::OnDblClickSubStepGrid() 
{
	//int nMainRow = m_ctrlMainGrid.GetRow();
	//m_Sub_Click_nItem = nItem;
	//m_Sub_Click_nShell = nShell;
	int nRow = m_Sub_Click_nItem;
	int nCol = m_Sub_Click_nShell;

	CString sName = _T("");
	CString sTmp = m_ctrlSubList.GetItemText(nRow, 0);//m_ctrlSubGrid.GetTextMatrix(nRow, 0);
	sTmp.TrimLeft(); sTmp.TrimRight(); sTmp.MakeLower();
	
	if((nCol != 0) && (sTmp.IsEmpty())) return;

	UINT nStepType = -1;
	for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
	{
		sName.Format("%s", Predefined[nTmp].pszFunc);
		sName.TrimLeft(); sName.TrimRight();  sName.MakeLower();

		if(sTmp == sName)
		{
			nStepType = nTmp;
			break;
		}
	}	
	//+ 2007.10.11 Add BY USY
	//m_ctrlSubGrid.SetTopRow(nRow);
	//ChangeRowColor(nRow , 1);
	//-
	if (nStepType >= 0)
	{
		SetSubGrid4Edit(nStepType, nRow, nCol);
	}
}


void CSeqEditorDlg::OnBtnInsertSubStep() 
{
	//int nRow = m_ctrlSubGrid.GetRow();
	//int nCol = m_ctrlSubGrid.GetCol();

	//if(nRow < 1)
	//{
	//	CFunction* pFunction = new CFunction();
	//	m_CurFunc.AddTail(pFunction);
	//}
	//else 
	//{
	//	CFunction* pFunction = new CFunction();
	//	POSITION Pos = m_CurFunc.GetHeadPosition();
	//	Pos = m_CurFunc.FindIndex(nRow - 1);
	//	m_CurFunc.InsertAfter(Pos, pFunction);
	//}
	//UpdateSubGrid();

	////+ 2007.10.11 Add BY USY
	//m_ctrlSubGrid.SetTopRow(nRow);
	//ChangeRowColor(nRow , 1);
	////-
}

void CSeqEditorDlg::OnBtnUpSubStep() 
{
	//int nRow = m_ctrlSubGrid.GetRow();

	//if(nRow < 2) return;
	//else
	//{
	//	POSITION Pos = m_CurFunc.GetHeadPosition();
	//	POSITION CurPos = m_CurFunc.FindIndex(nRow - 1);
	//	POSITION BeforePos = m_CurFunc.FindIndex(nRow - 2);
	//	if((CurPos != NULL) && (BeforePos != NULL))
	//	{
	//		CFunction* pCurFunc = m_CurFunc.GetAt(CurPos);
	//		CFunction* pBeforeFunc = m_CurFunc.GetAt(BeforePos);

	//		m_CurFunc.SetAt(CurPos, pBeforeFunc);
	//		m_CurFunc.SetAt(BeforePos, pCurFunc);
	//	}
	//	UpdateSubGrid();
	//}
	////+ 2007.10.11 Add BY USY
	//m_ctrlSubGrid.SetTopRow(nRow - 1);
	//ChangeRowColor(nRow - 1 , 1);
	////-
}

void CSeqEditorDlg::OnBtnDownSubStep() 
{
	//int nRow = m_ctrlSubGrid.GetRow();

	//if( nRow < 1) return;
	//else
	//{
	//	POSITION Pos = m_CurFunc.GetHeadPosition();
	//	POSITION CurPos = m_CurFunc.FindIndex(nRow - 1);
	//	POSITION AfterPos= m_CurFunc.FindIndex(nRow);
	//	if((CurPos != NULL) && (AfterPos != NULL))
	//	{
	//		CFunction* pCurFunc = m_CurFunc.GetAt(CurPos);
	//		CFunction* pAfterFunc = m_CurFunc.GetAt(AfterPos);

	//		m_CurFunc.SetAt(CurPos, pAfterFunc);
	//		m_CurFunc.SetAt(AfterPos, pCurFunc);
	//	}
	//	UpdateSubGrid();
	//}
	////+ 2007.10.11 Add BY USY
	//m_ctrlSubGrid.SetTopRow(nRow + 1);
	//ChangeRowColor(nRow + 1 , 1);
	////-
}

void CSeqEditorDlg::OnBtnDeleteSubStep() 
{
	//int nRow = m_ctrlSubGrid.GetRow();

	//if((nRow <= m_CurFunc.GetCount())&&(nRow > 0))
	//{
	//	POSITION Pos = m_CurFunc.FindIndex(nRow - 1);
	//	if(Pos != NULL)
	//	{
	//		m_CurFunc.RemoveAt(Pos);
	//	}
	//	UpdateSubGrid();
	//}


}

void CSeqEditorDlg::OnBtnClearSubStep() 
{
	//int nRow = m_ctrlSubGrid.GetRow();

	//if((nRow <= m_CurFunc.GetCount())&&(nRow > 0))
	//{
	//	DeleteFunctionList();
	//	UpdateSubGrid();
	//}
}

void CSeqEditorDlg::OnCellChangedSubStepGrid(long Row, long Col) 
{
	if((Col == 0) &&  (Row > 0))
	{
		SetDefaultData2SubStepGrid(Row, Col);
	}
}

void CSeqEditorDlg::OnBtnUploadSubStep() 
{

	UpdateSubStep();

	int nMainRow = m_nMCurRow; //m_ctrlMainGrid.GetRow();
	
	POSITION Pos = StepList.GetHeadPosition();
	POSITION CurPos = StepList.FindIndex(nMainRow - 1);
	if(CurPos != NULL)
	{
		CStep* pStep = StepList.GetAt(CurPos);
		POSITION FuncPos = pStep->m_InstructionList.GetHeadPosition();
		while(FuncPos != NULL)
		{
			delete pStep->m_InstructionList.GetNext(FuncPos);
		}
		pStep->m_InstructionList.RemoveAll();

		FuncPos = m_CurFunc.GetHeadPosition();
		while (FuncPos != NULL) 
		{
			CFunction* pCopyFunc = new CFunction;
			CFunction* pOrgFunc = m_CurFunc.GetNext(FuncPos);
			
			pCopyFunc->m_pFunc = pOrgFunc->m_pFunc;
			pCopyFunc->m_strCommant = pOrgFunc->m_strCommant;
			POSITION ArgPos = pOrgFunc->m_ArgumentList.GetHeadPosition();
			while(ArgPos != NULL)
			{
				Argument* pCopyArgument = new Argument;
				Argument* pOrgArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
				memcpy(pCopyArgument->String, pOrgArgument->String, sizeof(pOrgArgument->String));
				pCopyFunc->m_ArgumentList.AddTail(pCopyArgument);
			}
			pStep->m_InstructionList.AddTail(pCopyFunc);

		}
		StepList.SetAt(CurPos, pStep);
	}

	//UpdateSubStep();

	//int nMainRow = m_ctrlMainGrid.GetRow();
	//
	//POSITION Pos = StepList.GetHeadPosition();
	//POSITION CurPos = StepList.FindIndex(nMainRow - 1);
	//if(CurPos != NULL)
	//{
	//	CStep* pStep = StepList.GetAt(CurPos);
	//	POSITION FuncPos = pStep->m_InstructionList.GetHeadPosition();
	//	while(FuncPos != NULL)
	//	{
	//		delete pStep->m_InstructionList.GetNext(FuncPos);
	//	}
	//	pStep->m_InstructionList.RemoveAll();

	//	FuncPos = m_CurFunc.GetHeadPosition();
	//	while (FuncPos != NULL) 
	//	{
	//		CFunction* pCopyFunc = new CFunction;
	//		CFunction* pOrgFunc = m_CurFunc.GetNext(FuncPos);
	//		
	//			pCopyFunc->m_pFunc = pOrgFunc->m_pFunc;
	//		POSITION ArgPos = pOrgFunc->m_ArgumentList.GetHeadPosition();
	//		while(ArgPos != NULL)
	//		{
	//			Argument* pCopyArgument = new Argument;
	//			Argument* pOrgArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
	//			memcpy(pCopyArgument->String, pOrgArgument->String, sizeof(pOrgArgument->String));
	//			pCopyFunc->m_ArgumentList.AddTail(pCopyArgument);
	//		}
	//		pStep->m_InstructionList.AddTail(pCopyFunc);

	//	}
	//	StepList.SetAt(CurPos, pStep);
	//}
}

void CSeqEditorDlg::OnValidateEditMainStepGrid(long Row, long Col, BOOL FAR* Cancel) 
{
	
}

void CSeqEditorDlg::OnAfterEditMainStepGrid(long Row, long Col) 
{
	//m_ctrlMainGrid.Refresh();
	//int nStepNo = m_ctrlMainGrid.GetRow();

	//if(nStepNo < 0 ) return; 

	//POSITION Pos	= StepList.GetHeadPosition();
	//POSITION CurPos = StepList.FindIndex(nStepNo - 1);
	//CString sTmp;

	//if(CurPos != NULL)
	//{
	//	CStep* pStep = StepList.GetAt(CurPos);
	//	
	//	//+change kwmoon 080809
	////	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, 1));
	//	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_NAME_COL));

	//	//+change kwmoon 080809
	////	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 2);
	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_TYPE_COL);
	//	sTmp.MakeUpper();
	//	sTmp.TrimRight();
	//	sTmp.TrimLeft();
	//	if(sTmp.Compare("CHECK") == 0)
	//	{
	//		pStep->m_nStepType = CHECK_FUNCTION;

	//		//+change kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 4);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_SOURCE_COL);
	//		sTmp.MakeUpper();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		pStep->SetCurrentSource(sTmp);

	//		//+change kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 3);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TEST_COL);
	//		
	//		sTmp.MakeLower();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		if(sTmp != _T(""))
	//		{
	//			pStep->m_strFuncName = sTmp;

	//			if(CheckMainTestType(nStepNo) == MEAS_BOOL)
	//			{
	//				pStep->SetFuncType(MEAS_BOOL);
	//				
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("TRUE") == 0)
	//				{
	//					pStep->SetNominal(TRUE);
	//				}
	//				else if(sTmp.Compare("FALSE") == 0)
	//				{
	//					pStep->SetNominal(FALSE);
	//				}
	//			}
	//			else if(CheckMainTestType(nStepNo) == MEAS_DOUBLE)
	//			{
	//				pStep->SetFuncType(MEAS_DOUBLE);
	//				
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//			
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();

	//				pStep->SetNominal(atof(sTmp));

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 6);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_L_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();

	//				pStep->SetLowLimit(atof(sTmp));

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 7);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_U_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();

	//				pStep->SetHighLimit(atof(sTmp));

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 8);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_UNIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				pStep->SetUnit(sTmp);
	//			}
	//		}

	//	}
	//	else if(sTmp.Compare("PROCESS") == 0)
	//	{
	//		pStep->m_nStepType = PROCESS_FUNCTION;
	//		
	//	}
	//	StepList.SetAt(CurPos, pStep);
	//}

	//else if(CurPos == NULL)
	//{
	//	CStep* pStep = new CStep();

	//	//+change kwmoon 080809
	////	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, 1));
	//	pStep->SetStepName(m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_NAME_COL));

	//	//+change kwmoon 080809
	////	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 2);
	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_STEP_TYPE_COL);
	//	sTmp.MakeUpper();
	//	sTmp.TrimRight();
	//	sTmp.TrimLeft();
	//	if(sTmp.Compare("CHECK") == 0)
	//	{
	//		pStep->m_nStepType = CHECK_FUNCTION;
	//		//+change kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 4);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_SOURCE_COL);
	//		sTmp.MakeUpper();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		pStep->SetCurrentSource(sTmp);

	//		//+change kwmoon 080809
	//	//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 3);
	//		sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TEST_COL);
	//		sTmp.MakeLower();
	//		sTmp.TrimRight();
	//		sTmp.TrimLeft();
	//		if(sTmp != _T(""))
	//		{
	//			if(CheckMainTestType(nStepNo) == MEAS_BOOL)
	//			{
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				if(sTmp.Compare("TRUE") == 0)
	//				{
	//					pStep->SetNominal(TRUE);
	//				}
	//				else if(sTmp.Compare("FALSE") == 0)
	//				{
	//					pStep->SetNominal(FALSE);
	//				}
	//			}
	//			else if(CheckMainTestType(nStepNo) == MEAS_DOUBLE)
	//			{
	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 5);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_TARGET_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();

	//				pStep->SetNominal(atof(sTmp));

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 6);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_L_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();

	//				pStep->SetLowLimit(atof(sTmp));

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 7);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_U_LIMIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();

	//				pStep->SetHighLimit(atof(sTmp));

	//				//+change kwmoon 080809
	//			//	sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, 8);
	//				sTmp = m_ctrlMainGrid.GetTextMatrix(nStepNo, MAIN_UNIT_COL);
	//				sTmp.MakeUpper();
	//				sTmp.TrimRight();
	//				sTmp.TrimLeft();
	//				pStep->SetUnit(sTmp);
	//			}
	//		}

	//	}
	//	else if(sTmp.Compare("PROCESS") == 0)
	//	{
	//		pStep->m_nStepType = PROCESS_FUNCTION;
	//		
	//	}
	//	StepList.AddTail(pStep);
	//}
}

void CSeqEditorDlg::OnAfterEditSubStepGrid(long Row, long Col) 
{
	//m_ctrlSubGrid.Refresh();
	//UpdateSubStep();
}

BOOL CSeqEditorDlg::PreTranslateMessage(MSG* pMsg) 
{
	//+ 2007.5.4 Add BY USY
	//Protect Dialog close By Enter Key or ESC Key
	if(pMsg->message == WM_KEYDOWN)
        if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
            return TRUE;
	//-

	return CDialog::PreTranslateMessage(pMsg);
}

void CSeqEditorDlg::OnClickSubStepGrid() 
{
//	int nRow = m_ctrlSubGrid.GetRow();
////	int nCol = m_ctrlSubGrid.GetCol();
//
//	SetSubGridHeader( nRow);
}

void CSeqEditorDlg::OnBeforeEditSubStepGrid(long Row, long Col, BOOL FAR* Cancel) 
{
//	int nMainRow = m_ctrlMainGrid.GetRow();
//
//	if((Row < 1) || (Col < 1)) return;
//
//	CString sName = _T("");
//	CString sTmp = m_ctrlSubGrid.GetTextMatrix(Row, 0);
//	sTmp.TrimLeft(); sTmp.TrimRight(); sTmp.MakeLower();
//	
//	UINT nStepType;
//	for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
//	{
//		sName.Format("%s", Predefined[nTmp].pszFunc);
//		sName.TrimLeft(); sName.TrimRight();  sName.MakeLower();
//
//		if(sTmp == sName)
//		{
//			nStepType = nTmp;
//			break;
//		}
//	}	
//	
//	SetSubGrid4Edit(nStepType, Row, Col);
}

void CSeqEditorDlg::OnBtnCopyMainStep() 
{
	//int nRow = m_ctrlMainGrid.GetRow();
	//int nCol = m_ctrlMainGrid.GetCol();

	//if(nRow < 1)
	//{
	//	return;
	//}
	//else 
	//{
	//	POSITION Pos = StepList.GetHeadPosition();
	//	POSITION OrgPos = StepList.FindIndex(nRow - 1);
	//	
	//	if(OrgPos != NULL)
	//	{
	//		CStep* pCopyStep = new CStep();
	//		CStep* pOrgStep = StepList.GetAt(OrgPos);
	//		CopyMainStep(pOrgStep, pCopyStep,pOrgStep->GetStepNo() + 1);

	//		StepList.InsertAfter(OrgPos, pCopyStep);
	//	}
	//	//Change Step No After Insert Step
	//	for(int nTmp = nRow + 1; nTmp <= StepList.GetCount(); nTmp++)
	//	{
	//		POSITION Pos = StepList.FindIndex(nTmp);
	//		if(Pos != NULL)
	//		{
	//			CStep* pStep = StepList.GetAt(Pos);
	//			pStep->SetStepNo(nTmp + 1);
	//			StepList.SetAt(Pos, pStep);
	//		}
	//	}
	//	UpdateMainGrid();

	//	CStep* pStep = FindStep(nRow + 1);
	//	SortSubStep(pStep);
	//	UpdateSubGrid();
	//}
	////+ 2007.10.11 Add BY USY
	//m_ctrlMainGrid.SetTopRow(nRow + 1);
	//ChangeRowColor(nRow + 1, 0);
	////-
}

void CSeqEditorDlg::OnBtnCopySubStep() 
{
	//int nRow = m_ctrlSubGrid.GetRow();

	//if(nRow < 1) return;
	//else
	//{
	//	POSITION Pos = m_CurFunc.GetHeadPosition();
	//	POSITION OrgPos = m_CurFunc.FindIndex(nRow - 1);

	//	if(OrgPos != NULL) 
	//	{
	//		CFunction* pOrgFunc = m_CurFunc.GetAt(OrgPos);
	//		CFunction* pCopyFunc = new CFunction();

	//		CopySubStep(pOrgFunc, pCopyFunc);

	//		m_CurFunc.InsertAfter(OrgPos, pCopyFunc);
	//	}
	//	UpdateSubGrid();
	//}
	////+ 2007.10.11 Add BY USY
	//m_ctrlSubGrid.SetTopRow(nRow );
	//ChangeRowColor(nRow , 1);
	////-
}

void CSeqEditorDlg::OnBtnInsertFromFullseq() 
{
	//int nRow = m_ctrlMainGrid.GetRow();

	//CFullSeqDlg dlg;
	//dlg.SetParentDlg(this);
	//dlg.DoModal();


}

//+add kwmoon 080508
void CSeqEditorDlg::InsertFromFullseq(int nSelRow) 
{
	int nRow = m_Main_Click_nItem;// m_ctrlMainGrid.GetRow();

//	CFullSeqDlg dlg;
//	if(dlg.DoModal() == IDOK)
//	{
		POSITION Pos = FullSeq.GetHeadPosition();
		POSITION OrgPos = FullSeq.FindIndex(nSelRow - 1);
		
		if(OrgPos != NULL)
		{
			CStep* pCopyStep = new CStep();
			CStep* pOrgStep = FullSeq.GetAt(OrgPos);

			if(nRow < 1)
			{
				CopyMainStep(pOrgStep, pCopyStep, StepList.GetCount() + 1);
				StepList.AddTail(pCopyStep);
			}
			else 
			{
				POSITION CopyPos = StepList.FindIndex(nRow - 1);
				CopyMainStep(pOrgStep, pCopyStep, nRow + 1);
				StepList.InsertAfter(CopyPos, pCopyStep);
				
				//Change Step No After Insert Step
				for(int nTmp = nRow + 1; nTmp <= StepList.GetCount(); nTmp++)
				{
					POSITION Pos = StepList.FindIndex(nTmp);
					if(Pos != NULL)
					{
						CStep* pStep = StepList.GetAt(Pos);
						pStep->SetStepNo(nTmp + 1);
						StepList.SetAt(Pos, pStep);
					}
				}
			}
			UpdateMainGrid();

			CStep* pStep = FindStep(nRow + 1);
			SortSubStep(pStep);
			UpdateSubGrid();
		
		}
//	}
	//+ 2007.10.11 Add BY USY
//	m_ctrlMainGrid.SetTopRow(nRow + 1);
//	ChangeRowColor(nRow + 1, 0);
	//-
}

//+add kwmoon 080508
void CSeqEditorDlg::OnClickMainStepGrid() 
{


	int nRow = m_Main_Click_nItem+1;// (int)m_ctrlStepMainList.GetFirstSelectedItemPosition();// 0;// m_ctrlMainGrid.GetRow();
	int nCol = m_Main_Click_nShell;// m_ctrlMainGrid.GetCol();
	if((nRow < 0) || (nCol < 0)) return;

	m_nMCurRow = nRow;
	//ChangeRowColor(nRow, 0);
	//
//	SetMainGrid4Edit(m_Main_Click_nItem, nCol);

	CStep* pStep = FindStep(nRow);
	SortSubStep(pStep);
	UpdateSubGrid();

}

//+add kwmoon 080805
void CSeqEditorDlg::InitTemplateGrid()
{
//	CString sTmp;
//
	//m_ctrlTemplateList.getco

	char *sHeader[] = {"NO","SOURCE","STEP NAME","AUDIO","STEP TYPE","TEST","TARGET","L-LIMIT","U-LIMIT","UNIT",""};
	int nWidth[] = { 40, 70, 160, 55, 70, 130, 90, 80, 80, 60, 80, 220, 120 };

	m_ctrlTemplateList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가

	//========
	// Header
	//========
	for (UINT nTmp = 0; nTmp < 10; nTmp++)
	{
		m_ctrlTemplateList.InsertColumn(nTmp, sHeader[nTmp], LVCFMT_CENTER, nWidth[nTmp]);//m_ctrlMainGrid.SetCol(nTmp);
			
	}
	
	m_ctrlTemplateList.Invalidate();


//	m_ctrlTemplateGrid.SetRedraw(flexRDNone);
//	m_ctrlTemplateGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything
//
////	m_ctrlTemplateGrid.SetBackColor(RGB(210,216,176));
////	m_ctrlTemplateGrid.SetForeColor(RGB(51,102,153));
//	m_ctrlTemplateGrid.SetBackColorBkg(RGB(247,247,231));
//	m_ctrlTemplateGrid.SetGridColor(RGB(210,216,176));
//
////	m_ctrlTemplateGrid.SetBackColorSel(RGB(255,255,128));
////	m_ctrlTemplateGrid.SetForeColorSel(RGB(0,0,0));
//
////	m_ctrlTemplateGrid.SetAllowSelection(FALSE);
//
//	m_ctrlTemplateGrid.SetFixedRows(0);
//	m_ctrlTemplateGrid.SetFixedCols(0);
//
//
//	m_ctrlTemplateGrid.SetFillStyle(flexFillRepeat);
//	m_ctrlTemplateGrid.SetFontName("Arial");
//	m_ctrlTemplateGrid.SetFontSize(10);
//
//	m_ctrlTemplateGrid.SetCols(11);
//	m_ctrlTemplateGrid.SetRows(1);
//
//	m_ctrlTemplateGrid.SetFrozenRows(1);
//	m_ctrlTemplateGrid.SetRow(0);
//
//	//========
//	// Header
//	//========
//	for(UINT nCol = 0; nCol < 11 ; nCol++)
//	{
//	
//		m_ctrlTemplateGrid.SetRow(0);
//		m_ctrlTemplateGrid.SetCol(nCol);
//		m_ctrlTemplateGrid.SetText(sHeader[nCol]);
//		m_ctrlTemplateGrid.SetFixedAlignment(nCol, flexAlignCenterCenter);
//		m_ctrlTemplateGrid.SetCellBackColor(RGB(210,216,176));
//		m_ctrlTemplateGrid.SetCellForeColor(RGB(51,102,153));
//	
//		switch(nCol)
//		{
//			case 0: m_ctrlTemplateGrid.SetColWidth(nCol, 300);
//					m_ctrlTemplateGrid.SetColDataType(nCol,flexDTBoolean);
//					m_ctrlTemplateGrid.SetCellChecked(flexUnchecked);
//					break;
//			case 1: m_ctrlTemplateGrid.SetColWidth(nCol, 700);	break;
//			case 2: m_ctrlTemplateGrid.SetColWidth(nCol, 1100);	break;
//			case 3: m_ctrlTemplateGrid.SetColWidth(nCol, 2400);	break;
//			case 4: m_ctrlTemplateGrid.SetColWidth(nCol, 800);
//					break;
//			case 5: m_ctrlTemplateGrid.SetColWidth(nCol, 1200);	break;
//			case 6: m_ctrlTemplateGrid.SetColWidth(nCol, 1800);	break;
//			case 7: m_ctrlTemplateGrid.SetColWidth(nCol, 1000);	break;
//			case 8: m_ctrlTemplateGrid.SetColWidth(nCol, 1000);	break;
//			case 9: m_ctrlTemplateGrid.SetColWidth(nCol, 1000);	break;
//			case 10: m_ctrlTemplateGrid.SetColWidth(nCol, 700);	break;
//		}
//	}
//
//	m_ctrlTemplateGrid.SetRow(0);
//	m_ctrlTemplateGrid.SetCol(0);
//	m_ctrlTemplateGrid.SetRedraw(-1);
//	m_ctrlTemplateGrid.Refresh();
//
//	m_nTemplateSelectedRow = -1;
}

void CSeqEditorDlg::UpdateTemplateGrid()
{
	//char *sHeader[] = {"","NO","SOURCE","STEP NAME","AUDIO","STEP TYPE","TEST","TARGET","L-LIMIT","U-LIMIT","UNIT"};

	//m_ctrlTemplateGrid.SetRedraw(flexRDNone);
	//m_ctrlTemplateGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearText	))); 

	//m_ctrlTemplateGrid.SetFrozenRows(1);
	//m_ctrlTemplateGrid.SetRow(0);

	////========
	//// Header
	////========
	//for(UINT nCol = 0; nCol < 11 ; nCol++)
	//{
	//
	//	m_ctrlTemplateGrid.SetRow(0);
	//	m_ctrlTemplateGrid.SetCol(nCol);
	//	m_ctrlTemplateGrid.SetText(sHeader[nCol]);
	//	m_ctrlTemplateGrid.SetFixedAlignment(nCol, flexAlignCenterCenter);
	//	m_ctrlTemplateGrid.SetCellBackColor(RGB(210,216,176));
	//	m_ctrlTemplateGrid.SetCellForeColor(RGB(51,102,153));
	//
	//	switch(nCol)
	//	{
	//		case 0: m_ctrlTemplateGrid.SetColWidth(nCol, 300);
	//				m_ctrlTemplateGrid.SetColDataType(nCol,flexDTBoolean);
	//				m_ctrlTemplateGrid.SetCellChecked(flexUnchecked);
	//				break;
	//		case 1: m_ctrlTemplateGrid.SetColWidth(nCol, 700);	break;
	//		case 2: m_ctrlTemplateGrid.SetColWidth(nCol, 1100);	break;
	//		case 3: m_ctrlTemplateGrid.SetColWidth(nCol, 2400);	break;
	//		case 4: m_ctrlTemplateGrid.SetColWidth(nCol, 800);
	//				break;
	//		case 5: m_ctrlTemplateGrid.SetColWidth(nCol, 1200);	break;
	//		case 6: m_ctrlTemplateGrid.SetColWidth(nCol, 1800);	break;
	//		case 7: m_ctrlTemplateGrid.SetColWidth(nCol, 1000);	break;
	//		case 8: m_ctrlTemplateGrid.SetColWidth(nCol, 1000);	break;
	//		case 9: m_ctrlTemplateGrid.SetColWidth(nCol, 1000);	break;
	//		case 10: m_ctrlTemplateGrid.SetColWidth(nCol, 700);	break;
	//	}
	//}
	//
	//m_ctrlTemplateGrid.SetBackColorFixed(RGB(210,216,176));
	//m_ctrlTemplateGrid.SetForeColorFixed(RGB(51,102,153));
	//m_ctrlTemplateGrid.SetBackColorBkg(RGB(247,247,231));
	//m_ctrlTemplateGrid.SetBackColor(RGB(247,247,231));

	//m_ctrlTemplateGrid.SetRows(FullSeq.GetCount() + 1);

	int nTmp = 0;

	CString sTmp;
	POSITION Position = FullSeq.GetHeadPosition();

	if(FullSeq.GetCount() > 0)
	{
		nTmp = 0;

		while (Position != NULL) 
		{
			CStep* pStep = FullSeq.GetNext(Position);

			//m_ctrlTemplateGrid.SetRow(nTmp);

			//===========
			// 1.Step No
			//===========
			sTmp.Format("%d", pStep->GetStepNo());
			//m_ctrlTemplateGrid.SetCol(1);
			//m_ctrlTemplateGrid.SetColDataType(1,flexDTString);
			//m_ctrlTemplateGrid.SetCellAlignment(flexAlignCenterCenter);
			//m_ctrlTemplateGrid.SetTextMatrix(nTmp,1,sTmp);
			m_ctrlTemplateList.InsertItem(nTmp, sTmp);
			//==========
			// 2.Source
			//==========
			if(pStep->m_nStepType == CHECK_FUNCTION)
			{
				//m_ctrlTemplateGrid.SetCol(2);
				//m_ctrlTemplateGrid.SetColDataType(2,flexDTString);
				//m_ctrlTemplateGrid.SetCellAlignment(flexAlignLeftCenter);
				//m_ctrlTemplateGrid.SetTextMatrix(nTmp,2,pStep->GetCurrentSource());
			
				//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());
				m_ctrlTemplateList.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetCurrentSource(), NULL, NULL, NULL, NULL);
			}

			//=============
			// 3.Step Name
			//=============
			//m_ctrlTemplateGrid.SetCol(3);
			//m_ctrlTemplateGrid.SetColDataType(3,flexDTString);
			//m_ctrlTemplateGrid.SetCellAlignment(flexAlignLeftCenter);
			//m_ctrlTemplateGrid.SetTextMatrix(nTmp,3,pStep->GetStepName());
	
			//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());
			m_ctrlTemplateList.SetItem(nTmp, 2, LVIF_TEXT, pStep->GetStepName(), NULL, NULL, NULL, NULL);


			//==============
			// 4.Audio Test
			//==============
			//m_ctrlTemplateGrid.SetCol(4);
			//m_ctrlTemplateGrid.SetColDataType(4,flexDTString);
			//m_ctrlTemplateGrid.SetCellAlignment(flexAlignCenterCenter);
			
			if(pStep->m_bRunAudioTest)
			{
				m_ctrlTemplateList.SetItem(nTmp, 3, LVIF_TEXT, "O", NULL, NULL, NULL, NULL); //m_ctrlTemplateGrid.SetTextMatrix(nTmp,4,"O");
			}
			else
			{
				m_ctrlTemplateList.SetItem(nTmp, 3, LVIF_TEXT, "X", NULL, NULL, NULL, NULL); //m_ctrlTemplateGrid.SetTextMatrix(nTmp,4,"X");
			}
			
		//	ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());
		//	m_ctrlTemplateList.SetItem(nTmp, 2, LVIF_TEXT, pStep->GetStepName(), NULL, NULL, NULL, NULL);

			//=============
			// 4.Step Type
			//=============
			if(pStep->m_nStepType == CHECK_FUNCTION)
			{
				sTmp.Format("CHECK");
			}
			else if(pStep->m_nStepType == PROCESS_FUNCTION)
			{
				sTmp.Format("PROCESS");
			}
			else
			{
				sTmp.Format(_T(""));
			}

			//m_ctrlTemplateGrid.SetCol(5);
			//m_ctrlTemplateGrid.SetColDataType(5, flexDTString);
			//m_ctrlTemplateGrid.SetCellAlignment(flexAlignLeftCenter);
			//m_ctrlTemplateGrid.SetTextMatrix(nTmp,5,sTmp);
			m_ctrlTemplateList.SetItem(nTmp, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);

			//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());
			
			if(pStep->m_nStepType == CHECK_FUNCTION)
			{
				//=================
				// 5.Function Name
				//=================
				//m_ctrlTemplateGrid.SetCol(6);
				//m_ctrlTemplateGrid.SetColDataType(6,flexDTString);
				//m_ctrlTemplateGrid.SetCellAlignment(flexAlignLeftCenter);
				//m_ctrlTemplateGrid.SetTextMatrix(nTmp,6,pStep->GetFuncName());

				//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());

				m_ctrlTemplateList.SetItem(nTmp, 5, LVIF_TEXT, pStep->GetFuncName(), NULL, NULL, NULL, NULL);
				
				//================
				// 6.Target Value
				//================
				if(pStep->GetFuncType() == MEAS_BOOL)
				{						
					if(pStep->GetNominal_Bool() == FALSE)
					{
						sTmp.Format(_T("%s"),"FALSE");
					}
					else
					{
						sTmp.Format(_T("%s"),"TRUE");
					}
				}
				else if(pStep->GetFuncType() == MEAS_DOUBLE)
				{
					sTmp.Format("%4.1f",pStep->GetNominal_float());
				}

				//m_ctrlTemplateGrid.SetCol(7);
				//m_ctrlTemplateGrid.SetColDataType(7, flexDTString);
				//m_ctrlTemplateGrid.SetCellAlignment(flexAlignCenterCenter);
				//m_ctrlTemplateGrid.SetTextMatrix(nTmp,7,sTmp);

				//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());
				m_ctrlTemplateList.SetItem(nTmp, 6, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);

				if(pStep->GetFuncType() != MEAS_BOOL)
				{
					//===============
					// 7.Lower Limit
					//===============
					sTmp.Format("%4.1f", pStep->GetLowLimit());
					//m_ctrlTemplateGrid.SetCol(8);
					//m_ctrlTemplateGrid.SetColDataType(8,flexDTString);
					//m_ctrlTemplateGrid.SetCellAlignment(flexAlignCenterCenter);
					//m_ctrlTemplateGrid.SetTextMatrix(nTmp,8,sTmp);

					//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());

					m_ctrlTemplateList.SetItem(nTmp, 7, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
		
					//===============
					// 8.Upper Limit
					//===============
					sTmp.Format("%4.1f", pStep->GetHighLimit());
					//m_ctrlTemplateGrid.SetCol(9);
					//m_ctrlTemplateGrid.SetColDataType(9,flexDTString);
					//m_ctrlTemplateGrid.SetCellAlignment(flexAlignCenterCenter);
					//m_ctrlTemplateGrid.SetTextMatrix(nTmp,9,sTmp);

					//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());
					m_ctrlTemplateList.SetItem(nTmp, 8, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);

					//========
					// 9.Unit
					//========
					sTmp.Format(_T("%s"), pStep->GetUnit());
					//m_ctrlTemplateGrid.SetCol(10);
					//m_ctrlTemplateGrid.SetColDataType(10,flexDTString);
					//m_ctrlTemplateGrid.SetCellAlignment(flexAlignCenterCenter);
					//m_ctrlTemplateGrid.SetTextMatrix(nTmp,10,sTmp);

					//ChangeColumnColor(TEMPLATE_GRID,pStep->GetCurrentSource());
					m_ctrlTemplateList.SetItem(nTmp, 9, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
				}
			}
			nTmp++;
		}
	}

	m_ctrlTemplateList.Invalidate();

	//m_ctrlTemplateGrid.SetRow(0);
	//m_ctrlTemplateGrid.SetCol(0);
	//m_ctrlTemplateGrid.SetRedraw(-1);
	//m_ctrlTemplateGrid.Refresh();
}

void CSeqEditorDlg::SelectRow(int nRow,int nGridType)
{
	//if(nRow > 0)
	//{
	//	if(nGridType == TEMPLATE_GRID)
	//	{
	//		m_ctrlTemplateGrid.Select(nRow, 0, COleVariant(long(nRow)), COleVariant(long(8)));
	//	}
	//	
	//	if(nGridType == MAIN_GRID)
	//	{
	//		m_ctrlMainGrid.Select(nRow, 0, COleVariant(long(nRow)), COleVariant(long(8)));
	//	}
	//}
}

void CSeqEditorDlg::OnClickTemplateSeqGrid() 
{
	// TODO: Add your control notification handler code here

	//int nRow = m_ctrlTemplateGrid.GetRow();
 //	int nCol = m_ctrlTemplateGrid.GetCol();
	//
	//if((nCol < 0) || (nCol > m_ctrlTemplateGrid.GetCols())) return;
	//if((nRow < 0) || (nRow > m_ctrlTemplateGrid.GetRows())) return;

	//m_nTemplateSelectedRow = nRow;

	////===============================
	//// Change Color of Selected Item 
	////===============================

	//BOOL bCellCheck = FALSE;

	//m_ctrlTemplateGrid.SetRow(nRow);
	//m_ctrlTemplateGrid.SetCol(nCol);

	////=============
	//// Select Item 
	////=============
	//if((nRow > 0) && (nCol == 0))
	//{
	//	if(m_ctrlTemplateGrid.GetCellChecked() == flexChecked)
	//	{
	//		m_ctrlTemplateGrid.SetCellChecked(flexUnchecked);
	//	}
	//	else
	//	{
	//		m_ctrlTemplateGrid.SetCellChecked(flexChecked);
	//	}
	//}
	////==================
	//// Select All Items 
	////==================
	//else if((nRow == 0) && (nCol == 0))
	//{
	//	if(m_ctrlTemplateGrid.GetCellChecked() == flexChecked)
	//	{
	//		bCellCheck = TRUE;
	//	}
	//	else
	//	{
	//		bCellCheck = FALSE;
	//	}
	//	
	//	for(int nTmp = 0; nTmp <= FullSeq.GetCount(); nTmp++)
	//	{
	//		m_ctrlTemplateGrid.SetRow(nTmp);

	//		if(bCellCheck == TRUE)
	//		{
	//			m_ctrlTemplateGrid.SetCellChecked(flexUnchecked);
	//		}
	//		else
	//		{
	//			m_ctrlTemplateGrid.SetCellChecked(flexChecked);
	//		}
	//	}
	//}

	//if(nRow > 0)
	//{
	//	SelectRow(nRow,TEMPLATE_GRID);
	//}
}

void CSeqEditorDlg::OnDblClickTemplateSeqGrid() 
{
	// TODO: Add your control notification handler code here
	m_nTemplateSelectedRow = m_Temp_Click_nItem+1;// m_ctrlTemplateGrid.GetRow();

	if(m_nTemplateSelectedRow < 1) return;
	
	SelectRow(m_nTemplateSelectedRow,TEMPLATE_GRID);

	//+del kwmoon 080508
//	OnOK();

	//+add kwmoon 080508
	InsertFromFullseq(m_nTemplateSelectedRow);	
}

//+add kwmoon 080804
void CSeqEditorDlg::ChangeColumnColor(int nGridType, CString szSource) 
{
	//if(nGridType == MAIN_GRID)
	//{
	//	if(szSource.Left(2).Compare("RF") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_YELLOW_1);
	//	}
	//	else if(szSource.Left(2).Compare("AV") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_YELLOW_2);
	//	}
	//	else if(szSource.Left(4).Compare("COMP") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_BLUE_2);
	//	}
	//	else if(szSource.Left(3).Compare("RGB") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_BLUE_1);
	//	}
	//	else if(szSource.Left(2).Compare("YC") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_GREEN_1);
	//	}
	//	else if(szSource.Left(4).Compare("VOUT") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_GREEN_2);
	//	}
	//	else if(szSource.Left(4).Compare("HDMI") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_GREEN_3);
	//	}
	//	else if(szSource.Left(3).Compare("MNT") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_GREEN_4);
	//	}
	//	else if(szSource.Left(2).Compare("TV") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_YELLOW_4);
	//	}
	//	else if(szSource.Left(3).Compare("OPT") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_YELLOW_3);
	//	}
	//	else if(szSource.Left(2).Compare("E/P") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_BLUE_3);
	//	}
	//	else if(szSource.Left(5).Compare("SPDIF") == NULL)
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(COLOR_BLUE_4);
	//	}
	//	else
	//	{
	//		m_ctrlMainGrid.SetCellBackColor(RGB(255,255,255));
	//	}
	//}
	//
	//
	//if(nGridType == TEMPLATE_GRID)
	//{
	//	if(szSource.Left(2).Compare("RF") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_YELLOW_1);
	//	}
	//	else if(szSource.Left(2).Compare("AV") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_YELLOW_2);
	//	}
	//	else if(szSource.Left(4).Compare("COMP") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_BLUE_2);
	//	}
	//	else if(szSource.Left(3).Compare("RGB") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_BLUE_1);
	//	}
	//	else if(szSource.Left(2).Compare("YC") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_GREEN_1);
	//	}
	//	else if(szSource.Left(4).Compare("VOUT") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_GREEN_2);
	//	}
	//	else if(szSource.Left(4).Compare("HDMI") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_GREEN_3);
	//	}
	//	else if(szSource.Left(3).Compare("MNT") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_GREEN_4);
	//	}
	//	else if(szSource.Left(2).Compare("TV") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_YELLOW_4);
	//	}
	//	else if(szSource.Left(3).Compare("OPT") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_YELLOW_3);
	//	}
	//	else if(szSource.Left(2).Compare("E/P") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_BLUE_3);
	//	}
	//	else if(szSource.Left(5).Compare("SPDIF") == NULL)
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(COLOR_BLUE_4);
	//	}
	//	else
	//	{
	//		m_ctrlTemplateGrid.SetCellBackColor(RGB(255,255,255));
	//	}
	//}
}

void CSeqEditorDlg::OnBtnAddAll() 
{
	// TODO: Add your control notification handler code here

	//m_ctrlTemplateGrid.SetCol(0);

	for(int nTmp = 0; nTmp <= FullSeq.GetCount(); nTmp++)
	{
		

		if(m_ctrlTemplateList.GetCheck(nTmp))
		{
			InsertFromFullseq(nTmp);	
		}
	}
}

void CSeqEditorDlg::OnBtnBrowseSeqFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "seq", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Sequence File(*.seq)|*.seq|",NULL);

	CString sTmp = CurrentSet->sModelFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		if(Prescan(dlg.GetPathName()))
		{
			m_strSeqPath.Format("%s",dlg.GetPathName());
		}
		else
		{
			m_strSeqPath.Format(_T(""));
		}
		m_ctrlSeqPathEdit.SetWindowText(m_strSeqPath);
		
	
		//==================
		// Init Main Grid
		//==================
		InitMainGrid();
		UpdateMainGrid();	

		//==================
		// Init Sub Grid
		//==================
		InitSubGrid();
	}
}
	
void CSeqEditorDlg::OnBtnBrowseTemplateFile() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, "seq", NULL, 
	OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY, 
	"Template File(*.seq)|*.seq|",NULL);

	CString sTmp = CurrentSet->sModelFolder;
	dlg.m_ofn.lpstrInitialDir = sTmp;

	if(dlg.DoModal() == IDOK) 
	{
		if(OpenFullSeq(dlg.GetPathName()))
		{
			CurrentSet->sFullSeqPath = dlg.GetPathName();
			m_strTemplatePath.Format("%s",dlg.GetPathName());
		}
		else
		{
			DeleteFullSeq();
			CurrentSet->sFullSeqPath = _T("");
			m_strTemplatePath.Format(_T(""));
		}
	
		m_ctrlTemplatePathEdit.SetWindowText(m_strTemplatePath);

		//======================
		// Init Template Grid
		//======================
		//InitTemplateGrid();
		UpdateTemplateGrid();	
	}
}

void CSeqEditorDlg::OnBtnSelectAll() 
{
	// TODO: Add your control notification handler code here
	//m_ctrlTemplateGrid.SetCol(0);

	for(int nTmp = 0; nTmp <= FullSeq.GetCount(); nTmp++)
	{
		//m_ctrlTemplateGrid.SetRow(nTmp);
		m_ctrlTemplateList.SetCheck(nTmp,1); //m_ctrlTemplateGrid.SetCellChecked(flexChecked);
	}
}

void CSeqEditorDlg::OnBtnDeselectAll() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//m_ctrlTemplateGrid.SetCol(0);
		
	for(int nTmp = 0; nTmp <= FullSeq.GetCount(); nTmp++)
	{
		//m_ctrlTemplateGrid.SetRow(nTmp);
		//m_ctrlTemplateGrid.SetCellChecked(flexUnchecked);
		m_ctrlTemplateList.SetCheck(nTmp,0);
	}
}


void CSeqEditorDlg::OnNMClickListMainStepList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	m_Main_Click_nItem = nItem;
	m_Main_Click_nShell = nShell;
	//int m_Main_Click_nItem;//  = pNMItemActivate->iItem;
	//int m_Main_Click_nShell;// = pNMItemActivate->iSubItem;
	//int m_Sub_Click_nItem;//  = pNMItemActivate->iItem;
	//int m_Sub_Click_nShell;// = pNMItemActivate->iSubItem;
	//int m_Temp_Click_nItem;//  = pNMItemActivate->iItem;
	//int m_Temp_Click_nShell;// = pNMItemActivate->iSubItem;
	OnClickMainStepGrid();
	
	SetTimer(1, 100, NULL);
	*pResult = 0;
}


void CSeqEditorDlg::OnNMClickSubStepList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;





	*pResult = 0;
}


void CSeqEditorDlg::OnNMDblclkSubStepList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	
	m_Sub_Click_nItem   = nItem;	
	m_Sub_Click_nShell = nShell;
	OnDblClickSubStepGrid();
//	SetTimer(1, 100, NULL);

	*pResult = 0;
}


void CSeqEditorDlg::OnNMClickTemplateSeqList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	*pResult = 0;
}


void CSeqEditorDlg::OnNMDblclkTemplateSeqList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	m_Temp_Click_nItem = nItem;
	OnDblClickTemplateSeqGrid();
	
	 
	*pResult = 0;
}


void CSeqEditorDlg::OnEnChangeEditSeqEditor()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	CString sTmp;
	//m_Main_Click_nItem;//  = pNMItemActivate->iItem;
	//int m_Main_Click_nShell
	if (m_Main_Click_nShell == 2)
	{
		GetDlgItemText(IDC_EDIT_SEQ_EDITOR, sTmp);
		m_ctrlStepMainList.SetItemText(m_Main_Click_nItem, 2, sTmp);
	}
	//GetDlgItem(IDC_EDIT_SEQ_EDITOR)->SetWindowPos(NULL, 2500, 85, 80, 24, SWP_ASYNCWINDOWPOS);//m_cComboSeqEditor.SetWindowPos(NULL, lRect.left + 2500, lRect.top + nRow * 17 - 85, 80, 10, SWP_ASYNCWINDOWPOS);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSeqEditorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1)
	{
		//m_ctrlStepMainList.Invalidate();
		GetDlgItem(IDC_EDIT_SEQ_EDITOR)->Invalidate();
		m_cComboSeqEditor.Invalidate();

		GetDlgItem(IDC_EDIT_SEQ_SUB_EDITOR)->Invalidate();
		m_cComboSeqSubEditor.Invalidate();

		KillTimer(1);
	}

	CDialog::OnTimer(nIDEvent);
}


void CSeqEditorDlg::OnCbnSelchangeComboSeqEditor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sTmp;
	

	//if (m_Main_Click_nShell == 1)
	{
		int lSel = m_cComboSeqEditor.GetCurSel();
		sTmp = m_ComboDataList.GetAt(lSel);
		//m_cComboSeqEditor.GetWindowTextA(sTmp); IDC_COMBO_SEQ_EDITOR
		//GetDlgItemText(IDC_COMBO_SEQ_EDITOR, sTmp);

		m_ctrlStepMainList.SetItemText(m_Main_Click_nItem, m_Main_Click_nShell, sTmp);
	}
	SetTimer(1, 100, NULL);
}
