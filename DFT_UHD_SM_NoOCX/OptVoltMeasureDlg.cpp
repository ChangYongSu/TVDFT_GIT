// OptVoltMeasureDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DATsys.h"
#include "OptVoltMeasureDlg.h"
#include "afxdialogex.h"
#include "Global.h"
#include "VF1000.h"
#include "TVCommCtrl.h"

extern CDATsysView*  g_pView;

// COptVoltMeasureDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COptVoltMeasureDlg, CPropertyPage)

COptVoltMeasureDlg::COptVoltMeasureDlg()
	: CPropertyPage(COptVoltMeasureDlg::IDD)
	, m_szStrCmd(_T(""))
{
	m_strConfigFile = CurrentSet->sVFTestIni;
	bTestAll = FALSE;
}

COptVoltMeasureDlg::~COptVoltMeasureDlg()
{
}

void COptVoltMeasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VOLT_SPEC_GRID, m_ctrlVoltSpecGrid);
	DDX_Control(pDX, IDC_VOLT_SPEC_GRID2, m_ctrlVoltSpecGrid2);
	DDX_Control(pDX, IDC_VOLT_SPEC_GRID3, m_ctrlVoltSpecGrid3);
	DDX_Control(pDX, IDC_COMBO_TIME, m_ctrlComboTime);
	DDX_Text(pDX, IDC_EDIT_RS232C_STRCMD, m_szStrCmd);
	DDX_Control(pDX, IDC_LIST_VOLT_SPEC, m_ctrlVoltSpecList);
	DDX_Control(pDX, IDC_LIST_VOLT_SPEC2, m_ctrlVoltSpecList2);
	DDX_Control(pDX, IDC_LIST_VOLT_SPEC3, m_ctrlVoltSpecList3);
}

BEGIN_MESSAGE_MAP(COptVoltMeasureDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_SAVE, &COptVoltMeasureDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BUTTON_TESTALL, &COptVoltMeasureDlg::OnBnClickedButtonTestall)
	ON_BN_CLICKED(IDC_BUTTON_TESTCLS, &COptVoltMeasureDlg::OnBnClickedButtonTestcls)
	ON_BN_CLICKED(IDC_BTN_RESULTCOPY, &COptVoltMeasureDlg::OnBnClickedBtnResultcopy)
	ON_BN_CLICKED(IDC_BUTTON_SELECTALL, &COptVoltMeasureDlg::OnBnClickedButtonSelectall)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECTALL, &COptVoltMeasureDlg::OnBnClickedButtonUnselectall)
	ON_BN_CLICKED(IDC_BTN_GRABSTOP, &COptVoltMeasureDlg::OnBnClickedBtbGrabstop)
	ON_BN_CLICKED(IDC_BTN_GRABSTART, &COptVoltMeasureDlg::OnBnClickedBtbGrabstart)
	ON_BN_CLICKED(IDC_BTN_SEND_COMMAND2, &COptVoltMeasureDlg::OnBnClickedBtnSendCommand2)
	ON_BN_CLICKED(IDC_BTN_SEND_READY_COMMAND, &COptVoltMeasureDlg::OnBnClickedBtnSendReadyCommand)
END_MESSAGE_MAP()


// COptVoltMeasureDlg 메시지 처리기입니다.


void COptVoltMeasureDlg::InitVoltSpecGrid()
{
	CString sTmp;

	char *sHeader1[] = {"V", "NO(CH)","JIG POINT(Volt.)", "TYPE", "TARGET1", "TARGET2", "TEST" , "MARGIN" };
	char *sHeader3[] = {"F", "NO(CH)","JIG POINT(Freq.)", "TYPE", "TARGET1", "TARGET2", "TEST" , "MARGIN" };
	char *sHeader2[] = {"POWER 24V,20V,19V", "POWER 13.2V,12V","POWER 3.5V", "POWER_ON", "EXT 5V",
						"EXT 12V", "Spare1"/*"INV_ON"*/, "DPC ON", "OPT VCC", "GND",
		                "LVDS B+/EPI LS_VGL", "LVDS B+/Inverter", "IR 3.5V_ST", "KEY1 3.3V", "KEY2 3.3V", 
						"Spare2", "EPI VCC12", "EPI VCC18", "EPI HVDD", "GND",
	                    "EPI VDD", "EPI VCOM1", "EPI VCOM2", "EPI P_VGL", "EPI VST", 
						"EPI LS_VGL", "EPI VGH_EVEN", "EPIVGH_ODD", "EPI VGH","GND",

						"P-DIM1", "P-DIM2", "Spare3","LNB1(BNC)", "LNB2(BNC)",
						"LNB3(BNC)", "Spare4", "Spare5", "Spare6", "Spare7", 
						"INV_ON", "Battery", "GND", "GND"};

	LPSTR sType     =  "#1;Nomal|"
		"#2;Min_Max|"
		"#3;Min|"
		"#4;Max";
//	LPSTR sType2     =  "#1;Normal|#2;Min_Max|#3;Min|#4;Max";
	LPSTR sType2     =  "#1;Normal|#2;Min_Max";
	LPSTR sTMargin     =  "#0;None|#1;10%|#2;20%|#3;30%|#4;40%";

	m_ctrlVoltSpecGrid.SetRedraw(FALSE);
	m_ctrlVoltSpecGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlVoltSpecGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlVoltSpecGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlVoltSpecGrid.SetBackColor(RGB(247,247,231));
	m_ctrlVoltSpecGrid.SetGridColor(RGB(200,200,176));

	m_ctrlVoltSpecGrid.SetFixedRows(1);
	m_ctrlVoltSpecGrid.SetFixedCols(0);
//	m_ctrlVoltSpecGrid.SetFrozenRows(1);

	m_ctrlVoltSpecGrid.SetFillStyle(flexFillRepeat);
	m_ctrlVoltSpecGrid.SetFontName("Arial");
	m_ctrlVoltSpecGrid.SetFontSize(8);

	m_ctrlVoltSpecGrid.SetCols(8); //m_ctrlVoltSpecGrid.SetCols(7);
	m_ctrlVoltSpecGrid.SetRows(31);

	m_ctrlVoltSpecGrid.SetExtendLastCol(TRUE);
	m_ctrlVoltSpecGrid.SetFrozenCols(2);	
    m_ctrlVoltSpecGrid.SetEditable(TRUE);

	m_ctrlVoltSpecGrid2.SetRedraw(FALSE);
	m_ctrlVoltSpecGrid2.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlVoltSpecGrid2.SetBackColorFixed(RGB(210,216,176));
	m_ctrlVoltSpecGrid2.SetForeColorFixed(RGB(51,102,153));
	m_ctrlVoltSpecGrid2.SetBackColor(RGB(247,247,231));
	m_ctrlVoltSpecGrid2.SetGridColor(RGB(200,200,176));

	m_ctrlVoltSpecGrid2.SetFixedRows(1);
	m_ctrlVoltSpecGrid2.SetFixedCols(0);
//	m_ctrlVoltSpecGrid2.SetFrozenRows(1);

	m_ctrlVoltSpecGrid2.SetFillStyle(flexFillRepeat);
	m_ctrlVoltSpecGrid2.SetFontName("Arial");
	m_ctrlVoltSpecGrid2.SetFontSize(8);

	m_ctrlVoltSpecGrid2.SetCols(8); //m_ctrlVoltSpecGrid2.SetCols(7);
	m_ctrlVoltSpecGrid2.SetRows(15);

	m_ctrlVoltSpecGrid2.SetExtendLastCol(TRUE);
	m_ctrlVoltSpecGrid2.SetFrozenCols(2);	
    m_ctrlVoltSpecGrid2.SetEditable(TRUE);

	m_ctrlVoltSpecGrid3.SetRedraw(FALSE);
	m_ctrlVoltSpecGrid3.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlVoltSpecGrid3.SetBackColorFixed(RGB(210,216,176));
	m_ctrlVoltSpecGrid3.SetForeColorFixed(RGB(51,102,153));
	m_ctrlVoltSpecGrid3.SetBackColor(RGB(247,247,231));
	m_ctrlVoltSpecGrid3.SetGridColor(RGB(200,200,176));

	m_ctrlVoltSpecGrid3.SetFixedRows(1);
	m_ctrlVoltSpecGrid2.SetFixedCols(0);
//	m_ctrlVoltSpecGrid2.SetFrozenRows(1);

	m_ctrlVoltSpecGrid3.SetFillStyle(flexFillRepeat);
	m_ctrlVoltSpecGrid3.SetFontName("Arial");
	m_ctrlVoltSpecGrid3.SetFontSize(8);

	m_ctrlVoltSpecGrid3.SetCols(8); //m_ctrlVoltSpecGrid3.SetCols(7);
	m_ctrlVoltSpecGrid3.SetRows(7);

	m_ctrlVoltSpecGrid3.SetExtendLastCol(TRUE);
	m_ctrlVoltSpecGrid3.SetFrozenCols(2);	
    m_ctrlVoltSpecGrid3.SetEditable(TRUE);


	// Set Column Alignment & Column Title
	for(int nCol = 0; nCol < 8 ; nCol++)
	{
		m_ctrlVoltSpecGrid.SetRow(0);
		m_ctrlVoltSpecGrid.SetCol(nCol);
		m_ctrlVoltSpecGrid.SetText(sHeader1[nCol]);
		m_ctrlVoltSpecGrid.SetFixedAlignment(nCol,flexAlignCenterCenter);

		m_ctrlVoltSpecGrid2.SetRow(0);
		m_ctrlVoltSpecGrid2.SetCol(nCol);
		m_ctrlVoltSpecGrid2.SetText(sHeader1[nCol]);
		m_ctrlVoltSpecGrid2.SetFixedAlignment(nCol,flexAlignCenterCenter);

		m_ctrlVoltSpecGrid3.SetRow(0);
		m_ctrlVoltSpecGrid3.SetCol(nCol);
		m_ctrlVoltSpecGrid3.SetText(sHeader3[nCol]);
		m_ctrlVoltSpecGrid3.SetFixedAlignment(nCol,flexAlignCenterCenter);

		switch(nCol)
		{
			case 0 : // Check
						m_ctrlVoltSpecGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVoltSpecGrid.SetColWidth(nCol, 300); //300
						m_ctrlVoltSpecGrid.SetColDataType(nCol, flexDTBoolean);
						//m_ctrlVoltSpecGrid.SetCellChecked(flexChecked);

						m_ctrlVoltSpecGrid2.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVoltSpecGrid2.SetColWidth(nCol, 300); //300
						m_ctrlVoltSpecGrid2.SetColDataType(nCol, flexDTBoolean);
						//m_ctrlVoltSpecGrid2.SetCellChecked(flexChecked);


						m_ctrlVoltSpecGrid3.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVoltSpecGrid3.SetColWidth(nCol, 300); //300
						m_ctrlVoltSpecGrid3.SetColDataType(nCol, flexDTBoolean);
						//m_ctrlVoltSpecGrid3.SetCellChecked(flexChecked);
						break;

			case 1 : // No
						m_ctrlVoltSpecGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVoltSpecGrid.SetColWidth(nCol, 650); //500

						m_ctrlVoltSpecGrid2.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVoltSpecGrid2.SetColWidth(nCol, 650); //500

						m_ctrlVoltSpecGrid3.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVoltSpecGrid3.SetColWidth(nCol, 650); //500
						break;

			case 2 : // Test Name
						m_ctrlVoltSpecGrid.SetColAlignment(nCol,flexAlignLeftCenter);
	 					m_ctrlVoltSpecGrid.SetColWidth(nCol, 1800);//3100

						m_ctrlVoltSpecGrid2.SetColAlignment(nCol,flexAlignLeftCenter);
	 					m_ctrlVoltSpecGrid2.SetColWidth(nCol, 1800);//3100

						m_ctrlVoltSpecGrid3.SetColAlignment(nCol,flexAlignLeftCenter);
	 					m_ctrlVoltSpecGrid3.SetColWidth(nCol, 1800);//3100
				  		break;

			case 3 : // Type
						m_ctrlVoltSpecGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					m_ctrlVoltSpecGrid.SetColWidth(nCol, 900);
						m_ctrlVoltSpecGrid.SetColComboList(nCol, sType2);

						m_ctrlVoltSpecGrid2.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					m_ctrlVoltSpecGrid2.SetColWidth(nCol, 900);
						m_ctrlVoltSpecGrid2.SetColComboList(nCol, sType2);

						m_ctrlVoltSpecGrid3.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					m_ctrlVoltSpecGrid3.SetColWidth(nCol, 900);
						m_ctrlVoltSpecGrid3.SetColComboList(nCol, sType2);
						break;

			case 4 : // Spec1
						m_ctrlVoltSpecGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid.SetColWidth(nCol, 800);
						m_ctrlVoltSpecGrid.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid.SetColFormat(nCol, "##0.#");

						m_ctrlVoltSpecGrid2.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid2.SetColWidth(nCol, 800);
						m_ctrlVoltSpecGrid2.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid2.SetColFormat(nCol, "##0.#");

						m_ctrlVoltSpecGrid3.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid3.SetColWidth(nCol, 800);
						m_ctrlVoltSpecGrid3.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid3.SetColFormat(nCol, "##0.#");
				  		break;

			case 5 : // Spec2
						m_ctrlVoltSpecGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid.SetColWidth(nCol, 800);
						m_ctrlVoltSpecGrid.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid.SetColFormat(nCol, "##0.#");

						m_ctrlVoltSpecGrid2.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid2.SetColWidth(nCol, 800);
						m_ctrlVoltSpecGrid2.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid2.SetColFormat(nCol, "##0.#");

						m_ctrlVoltSpecGrid3.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid3.SetColWidth(nCol, 800);
						m_ctrlVoltSpecGrid3.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid3.SetColFormat(nCol, "##0.#");
				  		break;

			case 6 : // TEST
						m_ctrlVoltSpecGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid.SetColWidth(nCol, 1100);
						m_ctrlVoltSpecGrid.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid.SetColFormat(nCol, "##0.#");

						m_ctrlVoltSpecGrid2.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid2.SetColWidth(nCol, 1100);
						m_ctrlVoltSpecGrid2.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid2.SetColFormat(nCol, "##0.#");

						m_ctrlVoltSpecGrid3.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVoltSpecGrid3.SetColWidth(nCol, 1100);
						m_ctrlVoltSpecGrid3.SetColDataType(nCol, flexDTDouble);
						m_ctrlVoltSpecGrid3.SetColFormat(nCol, "##0.#");
				  		break;
			case 7: // Margin
				m_ctrlVoltSpecGrid.SetColAlignment(nCol, flexAlignCenterCenter);
				m_ctrlVoltSpecGrid.SetColWidth(nCol, 600);				
				m_ctrlVoltSpecGrid.SetColComboList(nCol, sTMargin);

				m_ctrlVoltSpecGrid2.SetColAlignment(nCol, flexAlignCenterCenter);
				m_ctrlVoltSpecGrid2.SetColWidth(nCol, 600);
				m_ctrlVoltSpecGrid2.SetColComboList(nCol, sTMargin);

				m_ctrlVoltSpecGrid3.SetColAlignment(nCol, flexAlignCenterCenter);
				m_ctrlVoltSpecGrid3.SetColWidth(nCol, 600);
				m_ctrlVoltSpecGrid3.SetColComboList(nCol, sTMargin);
				break;
		}
	}

	for(int nRol = 1; nRol < 31 ; nRol++)
	{

		m_ctrlVoltSpecGrid.SetRow(nRol);

		m_ctrlVoltSpecGrid.SetCol(0);
		if(nRol == 10 || nRol==20 || nRol== 30){
			
			m_ctrlVoltSpecGrid.SetCellChecked(flexNoCheckbox);
		}
		else
		{
			m_ctrlVoltSpecGrid.SetCellChecked(flexChecked);
		}

		m_ctrlVoltSpecGrid.SetCol(1);
		sTmp.Format(_T("%d"), nRol);
		m_ctrlVoltSpecGrid.SetText(sTmp);

		m_ctrlVoltSpecGrid.SetCol(2);
		m_ctrlVoltSpecGrid.SetText(sHeader2[nRol-1]);

		m_ctrlVoltSpecGrid.SetCol(3);
//		m_ctrlVoltSpecGrid.SetComboIndex(1);
		m_ctrlVoltSpecGrid.SetTextMatrix(nRol, 3, "1");

		

		if(nRol == 10 || nRol==20 || nRol== 30){
			m_ctrlVoltSpecGrid.SetTextMatrix(nRol,3, "");
			for(int nCol = 0; nCol < 8; nCol++){
				m_ctrlVoltSpecGrid.SetCol(nCol);
				m_ctrlVoltSpecGrid.SetCellBackColor(COLOR_GREEN_1);
			}
		}
		else
		{
			m_ctrlVoltSpecGrid.SetTextMatrix(nRol, 7, "2");
		}
	}

	for(int nRol2 = 1; nRol2 < 15 ; nRol2++)
	{
		m_ctrlVoltSpecGrid2.SetRow(nRol2);
			m_ctrlVoltSpecGrid2.SetCol(0);

		if (nRol2 == 13 || nRol2 == 14) {

			m_ctrlVoltSpecGrid2.SetCellChecked(flexNoCheckbox);
		}
		else
		{
			m_ctrlVoltSpecGrid2.SetCellChecked(flexChecked);
		}

		m_ctrlVoltSpecGrid2.SetCol(1);
		sTmp.Format(_T("%d"), nRol2 + 30);
		m_ctrlVoltSpecGrid2.SetText(sTmp);

		m_ctrlVoltSpecGrid2.SetCol(2);
		m_ctrlVoltSpecGrid2.SetText(sHeader2[nRol2 + 29]);

		m_ctrlVoltSpecGrid2.SetCol(3);
//		m_ctrlVoltSpecGrid2.SetComboIndex(1);
		m_ctrlVoltSpecGrid2.SetTextMatrix(nRol2,3, "1");
		
		if(nRol2 == 13 || nRol2 == 14){
			m_ctrlVoltSpecGrid2.SetTextMatrix(nRol2,3, "");
			
			for(int nCol = 0; nCol < 8; nCol++){
				m_ctrlVoltSpecGrid2.SetCol(nCol);
				m_ctrlVoltSpecGrid2.SetCellBackColor(COLOR_GREEN_1);
			}
		}
		else
		{
			m_ctrlVoltSpecGrid2.SetTextMatrix(nRol2, 7, "2");
			
		}

	
	}

	for(int nRol3 = 1; nRol3 < 7 ; nRol3++)
	{
		m_ctrlVoltSpecGrid3.SetRow(nRol3);

		m_ctrlVoltSpecGrid3.SetCol(0);
		m_ctrlVoltSpecGrid3.SetCellChecked(flexChecked);


		m_ctrlVoltSpecGrid3.SetCol(1);
		sTmp.Format(_T("%d"), nRol3 + 30);
		m_ctrlVoltSpecGrid3.SetText(sTmp);

		m_ctrlVoltSpecGrid3.SetCol(2);
		m_ctrlVoltSpecGrid3.SetText(sHeader2[nRol3 + 29]);

		m_ctrlVoltSpecGrid3.SetCol(3);
//		m_ctrlVoltSpecGrid3.SetComboIndex(1);
		m_ctrlVoltSpecGrid3.SetTextMatrix(nRol3,3, "1");
		m_ctrlVoltSpecGrid3.SetTextMatrix(nRol3, 7, "2");
	}

	m_ctrlVoltSpecGrid.SetRow(-1);
	m_ctrlVoltSpecGrid.SetCol(-1);
	m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid.Refresh();

	m_ctrlVoltSpecGrid2.SetRow(-1);
	m_ctrlVoltSpecGrid2.SetCol(-1);
	m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid2.Refresh();

	m_ctrlVoltSpecGrid3.SetRow(-1);
	m_ctrlVoltSpecGrid3.SetCol(-1);
	m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid3.Refresh();

}


void COptVoltMeasureDlg::InitVoltSpecList()
{
	CString sTmp;

	//char *sHeader1[6] = { "NO(CH)","JIG POINT(Volt.)", "TYPE", "TARGET1", "TARGET2", "TEST" };
	char *sHeader1[7] = { "CH","   JIG POINT(Volt.)", "TYPE", "TARGET1", "TARGET2", "TEST", "MARGIN" };
	//	int sWidth[6] = { 65, 180, 90, 80, 80, 60 };
	int sWidth[7] = { 30, 140, 80, 70, 70, 50 , 70};
	int sFormat[7] = { LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER , LVCFMT_CENTER};
	char *sHeader3[6] = { "NO(CH)","  JIG POINT(Freq.)", "TYPE", "TARGET1", "TARGET2", "TEST"};
	char *sHeader2[44] = {"POWER 24V,20V,19V", "POWER 13.2V,12V","POWER 3.5V", "POWER_ON", "EXT 5V", "EXT 12V", "Spare1"/*"INV_ON"*/, "DPC ON", "OPT VCC", "GND",
		                "LVDS B+/EPI LS_VGL", "LVDS B+/Inverter", "IR 3.5V_ST", "KEY1 3.3V", "KEY2 3.3V", "Spare2", "EPI VCC12", "EPI VCC18", "EPI HVDD", "GND",
	                    "EPI VDD", "EPI VCOM1", "EPI VCOM2", "EPI P_VGL", "EPI VST", "EPI LS_VGL", "EPI VGH_EVEN", "EPIVGH_ODD", "EPI VGH","GND",
						"P-DIM1", "P-DIM2", "Spare3", "LNB1(BNC)", "LNB2(BNC)", "LNB3(BNC)", "Spare4", "Spare5", "Spare6", "Spare7", 
						"INV_ON", "Battery", "GND", "GND"};

	LPSTR sType     =  "#1;Nomal|"
		"#2;Min_Max|"
		"#3;Min|"
		"#4;Max";
//	LPSTR sType2     =  "#1;Normal|#2;Min_Max|#3;Min|#4;Max";
	LPSTR sType2     =  "#1;Normal|#2;Min_Max";
	LPSTR sTMargin = "#0;None|#1;10%|#2;20%|#3;30%|#4;40%";

	//m_ctrlVoltSpecList
	//	m_CtrlListTestProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_ctrlVoltSpecList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_ctrlVoltSpecList2.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_ctrlVoltSpecList3.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	for (int i = 0; i < 6; i++)
	{
		m_ctrlVoltSpecList.InsertColumn(i, sHeader1[i], sFormat[i], sWidth[i]);
		m_ctrlVoltSpecList2.InsertColumn(i, sHeader1[i], sFormat[i], sWidth[i]);
		m_ctrlVoltSpecList3.InsertColumn(i, sHeader1[i], sFormat[i], sWidth[i]);
	}
//	m_ctrlVoltSpecList.InsertColumn(1, sHeader1[1], LVCFMT_LEFT, sWidth[1]);

	for (int i = 0; i < 30; i++)
	{
		sTmp.Format(_T("%d"), i + 1);
		m_ctrlVoltSpecList.InsertItem(i, sTmp);   // 첫째행(0), 첫째열에 삽입		
		m_ctrlVoltSpecList.SetItem(i, 1, LVIF_TEXT, _T(sHeader2[i]), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		if ((i == 9) || (i == 19) || (i == 29))
		{
			for (int nCol = 0; nCol < 8; nCol++) {			
				m_ctrlVoltSpecListColor.SetCellBackColor(i, nCol, COLOR_GREEN_1);//
			}
			continue;
		}

		m_ctrlVoltSpecList.SetCheck(i,1);

		m_ctrlVoltSpecList.SetItem(0, 2, LVIF_TEXT, _T("Normal"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		m_ctrlVoltSpecList.SetItem(0, 3, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//m_ctrlVoltSpecList.SetItem(0, 4, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//m_ctrlVoltSpecList.SetItem(0, 5, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		m_ctrlVoltSpecList.SetItem(0, 6, LVIF_TEXT, _T("20%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	}
	for (int i = 0; i < 14; i++)
	{

		sTmp.Format(_T("%d"), i + 31);
		m_ctrlVoltSpecList.InsertItem(i, sTmp);   // 첫째행(0), 첫째열에 삽입		
		m_ctrlVoltSpecList.SetItem(i, 1, LVIF_TEXT, _T(sHeader2[i+30]), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		if ((i == 13) || (i == 14))
		{
			continue;
		}
		
		m_ctrlVoltSpecList.SetItem(0, 2, LVIF_TEXT, _T("Normal"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		m_ctrlVoltSpecList.SetItem(0, 3, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//m_ctrlVoltSpecList.SetItem(0, 4, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//m_ctrlVoltSpecList.SetItem(0, 5, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		m_ctrlVoltSpecList.SetItem(0, 6, LVIF_TEXT, _T("20%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	}

	for (int i = 0; i < 6; i++)
	{
		sTmp.Format(_T("%d"), i + 30);
		m_ctrlVoltSpecList.InsertItem(i, sTmp);   // 첫째행(0), 첫째열에 삽입		
		m_ctrlVoltSpecList.SetItem(i, 1, LVIF_TEXT, _T(sHeader2[i + 30]), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	

		m_ctrlVoltSpecList.SetItem(0, 2, LVIF_TEXT, _T("Normal"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		m_ctrlVoltSpecList.SetItem(0, 3, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//m_ctrlVoltSpecList.SetItem(0, 4, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//m_ctrlVoltSpecList.SetItem(0, 5, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		m_ctrlVoltSpecList.SetItem(0, 6, LVIF_TEXT, _T("20%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 

	}

	HFONT hNewFont;
	hNewFont = CreateFont(14, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_MODERN, "Arial");
	m_ctrlVoltSpecList.SendMessage(WM_SETFONT, (WPARAM)hNewFont, (LPARAM)TRUE);
	m_ctrlVoltSpecList2.SendMessage(WM_SETFONT, (WPARAM)hNewFont, (LPARAM)TRUE);
	m_ctrlVoltSpecList3.SendMessage(WM_SETFONT, (WPARAM)hNewFont, (LPARAM)TRUE);



}

void COptVoltMeasureDlg::ConfigDisplay()
{

	POSITION Pos = NULL;
	CVFTestInfo* pVFTestInfo = NULL;
	BOOL		bRun;
	UINT		nChNo;
	CString		strTestName;
	UINT		nCheckType;
	UINT		nMarginType;
	//	double		dTarget;	
	double		dLowerLimit;
	double		dUpperLimit;
	UINT		nTime = 0;
	CString sTmp;
#if 0
	for (int nRol = 27; nRol < 29; nRol++)
	{
		if (nRol == 10 || nRol == 20 || nRol == 30) {
			continue;
		}
		m_ctrlVoltSpecGrid.SetTextMatrix(nRol, 2, _T(""));
	}

	for (int nRol2 = 1; nRol2 < 3; nRol2++)
	{
		m_ctrlVoltSpecGrid2.SetTextMatrix(nRol2, 2, _T(""));
	}
#else
	for (int nRol = 1; nRol < 31; nRol++)
	{
		if (nRol == 10 || nRol == 20 || nRol == 30) {
			m_ctrlVoltSpecGrid.SetTextMatrix(nRol, 4, _T(""));
			m_ctrlVoltSpecGrid.SetRow(nRol);
			m_ctrlVoltSpecGrid.SetCol(0);
			m_ctrlVoltSpecGrid.SetCellChecked(flexUnchecked);
			continue;
		}

		if ( nRol == 27 || nRol == 28) {

			m_ctrlVoltSpecGrid.SetTextMatrix(nRol, 4, _T(""));
			m_ctrlVoltSpecGrid.SetRow(nRol);
			m_ctrlVoltSpecGrid.SetCol(0);
			m_ctrlVoltSpecGrid.SetCellChecked(flexUnchecked);
		}
		else
		{
			m_ctrlVoltSpecGrid.SetTextMatrix(nRol, 4, _T("0"));
		}
		
	}

	for (int nRol2 = 1; nRol2 < 13; nRol2++)
	{
		if (nRol2 == 1 || nRol2 == 2 ) {
			m_ctrlVoltSpecGrid2.SetTextMatrix(nRol2, 4, _T(""));
			m_ctrlVoltSpecGrid2.SetRow(nRol2);
			m_ctrlVoltSpecGrid2.SetCol(0);
			m_ctrlVoltSpecGrid2.SetCellChecked(flexUnchecked);
		}
		else
		{
			m_ctrlVoltSpecGrid2.SetTextMatrix(nRol2, 4, _T("0"));
		}		
	}

	for (int nRol3 = 1; nRol3 < 7; nRol3++)
	{
		m_ctrlVoltSpecGrid3.SetTextMatrix(nRol3, 4, _T("0"));
	}
#endif
	Pos = CurrentSet->VF_TestInfo.GetHeadPosition();

	while (Pos)
	{
		pVFTestInfo = CurrentSet->VF_TestInfo.GetNext(Pos);
		bRun = pVFTestInfo->bRun;
		nChNo = pVFTestInfo->nChNo;
		strTestName = pVFTestInfo->strTestName;
		nCheckType = pVFTestInfo->nCheckType;
		dLowerLimit = pVFTestInfo->dLowerLimit;
		dUpperLimit = pVFTestInfo->dUpperLimit;		
		nMarginType = pVFTestInfo->nMarginType;
		
		nTime = pVFTestInfo->nTime;

		if (nCheckType == 2) {
			m_ctrlComboTime.SetCurSel(nTime);
		}

		if (nChNo == 10 || nChNo == 20 || nChNo == 30 || nChNo == 43 || nChNo == 44)
		{
			continue;
		}

		if (nChNo <= 30) {
			m_ctrlVoltSpecGrid.SetRow(nChNo);

			m_ctrlVoltSpecGrid.SetCol(0);

			if (bRun) {
				m_ctrlVoltSpecGrid.SetCellChecked(flexChecked);	
				m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 2, strTestName);

				sTmp.Format(_T("%d"), nCheckType);
				m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 3, sTmp);


				sTmp.Format(_T("%.1f"), dLowerLimit);
				m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 4, sTmp);

				if (nCheckType == 1) {
					if (nMarginType == 0)
						nMarginType = 2;
					sTmp.Format(_T("%d"), nMarginType);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 7, sTmp);
				}
				else if (nCheckType == 2) {
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 7, "0");
					sTmp.Format(_T("%.1f"), dUpperLimit);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 5, sTmp);
				}
			}
			else
			{
				if (nChNo == 27 || nChNo == 28)
				{
					m_ctrlVoltSpecGrid.SetCellChecked(flexUnchecked);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 2, strTestName);
				
					sTmp.Format(_T("%d"), nCheckType);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 3, sTmp);


					sTmp.Format(_T("%.1f"), dLowerLimit);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 4, sTmp);

					if (nCheckType == 1) {
						//sTmp.Format(_T("%d"), nMarginType);
						m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 7, "0");
					}
					else if (nCheckType == 2) {
						sTmp.Format(_T("%.1f"), dUpperLimit);
						m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 5, sTmp);
					}
				}
				else
				{
					m_ctrlVoltSpecGrid.SetCellChecked(flexChecked);
				
				
					//m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 2, strTestName);
					nCheckType = 1;
					sTmp.Format(_T("%d"), nCheckType);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 3, sTmp);

					dLowerLimit = 0.0;
					sTmp.Format(_T("%.1f"), dLowerLimit);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 4, sTmp);

					nMarginType = 2;
					sTmp.Format(_T("%d"), nMarginType);
					m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 7, sTmp);
				}

			}


		}
		else if (nChNo > 30 && nChNo <= 42) {
			m_ctrlVoltSpecGrid2.SetRow(nChNo - 30);

			m_ctrlVoltSpecGrid2.SetCol(0);

			if (bRun) {
				m_ctrlVoltSpecGrid2.SetCellChecked(flexChecked);
				m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 2, strTestName);

				sTmp.Format(_T("%d"), nCheckType);
				m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 3, sTmp);


				sTmp.Format(_T("%.1f"), dLowerLimit);
				m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 4, sTmp);

				if (nCheckType == 1) {
					if (nMarginType == 0)
						nMarginType = 2;
					sTmp.Format(_T("%d"), nMarginType);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo-30, 7, sTmp);
				}
				else if (nCheckType == 2) {
					sTmp.Format(_T("%.1f"), dUpperLimit);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 5, sTmp);
				}
			}
			else
			{
				if (nChNo == 31 || nChNo == 32)
				{
					m_ctrlVoltSpecGrid2.SetCellChecked(flexUnchecked);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo-30, 2, strTestName);

					sTmp.Format(_T("%d"), nCheckType);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo-30, 3, sTmp);


					sTmp.Format(_T("%.1f"), dLowerLimit);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo-30, 4, sTmp);

					if (nCheckType == 1) {
						if (nMarginType == 0)
							nMarginType = 2;
						sTmp.Format(_T("%d"), nMarginType);
						m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo-30, 7, sTmp);
					}
					else if (nCheckType == 2) {
						sTmp.Format(_T("%.1f"), dUpperLimit);
						m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo-30, 5, sTmp);
					}
				}
				else
				{
					m_ctrlVoltSpecGrid2.SetCellChecked(flexChecked);
				

					//m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 2, strTestName);
					nCheckType = 1;
					sTmp.Format(_T("%d"), nCheckType);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 3, sTmp);

					dLowerLimit = 0.0;
					sTmp.Format(_T("%.1f"), dLowerLimit);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 4, sTmp);
					nMarginType = 2;
					sTmp.Format(_T("%d"), nMarginType);
					m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 7, sTmp);
				}
			}			
		}
		else if (nChNo > 100) {
			m_ctrlVoltSpecGrid3.SetRow(nChNo - 100);

			m_ctrlVoltSpecGrid3.SetCol(0);

			if (bRun) {
				m_ctrlVoltSpecGrid3.SetCellChecked(flexChecked);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 2, strTestName);

				sTmp.Format(_T("%d"), nCheckType);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 3, sTmp);


				sTmp.Format(_T("%.1f"), dLowerLimit);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 4, sTmp);

				if (nCheckType == 1) {
					if (nMarginType == 0)
					{
						nMarginType = 2;
					}
					sTmp.Format(_T("%d"), nMarginType);
					m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 7, sTmp);
				}
				else if (nCheckType == 2) {
					sTmp.Format(_T("%.1f"), dUpperLimit);
					m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 5, sTmp);
				}
			}
			else
			{
				m_ctrlVoltSpecGrid3.SetCellChecked(flexChecked);


				//m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 2, strTestName);
				nCheckType = 1;
				sTmp.Format(_T("%d"), nCheckType);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 3, sTmp);

				dLowerLimit = 0.0;
				sTmp.Format(_T("%.1f"), dLowerLimit);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 4, sTmp);

				nMarginType = 2;
				sTmp.Format(_T("%d"), nMarginType);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 7, "2");
			}
		}
	}

	m_ctrlVoltSpecGrid.SetRow(-1);
	m_ctrlVoltSpecGrid.SetCol(-1);
	m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid.Refresh();

	m_ctrlVoltSpecGrid2.SetRow(-1);
	m_ctrlVoltSpecGrid2.SetCol(-1);
	m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid2.Refresh();

	m_ctrlVoltSpecGrid3.SetRow(-1);
	m_ctrlVoltSpecGrid3.SetCol(-1);
	m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid3.Refresh();
}

void COptVoltMeasureDlg::ConfigDisplayList()
{

	POSITION Pos = NULL;
	CVFTestInfo* pVFTestInfo = NULL;
	BOOL		bRun;
	UINT		nChNo;
	CString		strTestName;
	UINT		nCheckType;
	//	double		dTarget;	
	double		dLowerLimit;
	double		dUpperLimit;
	UINT		nTime = 0;
	CString sTmp;
	int nRol;
	for ( nRol = 0; nRol < 30; nRol++)
	{
		if (nRol == 9 || nRol == 19 || nRol == 29) {
			continue;
		}
		//m_ctrlVoltSpecList.SetTextMatrix(nRol, 2, _T(""));
		m_ctrlVoltSpecList.SetItem(nRol , 1, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
	}

	/*for (int nRol2 = 1; nRol2 < 13; nRol2++)
	{
		m_ctrlVoltSpecGrid2.SetTextMatrix(nRol2, 2, _T(""));
	}

	for (int nRol3 = 1; nRol3 < 7; nRol3++)
	{
		m_ctrlVoltSpecGrid3.SetTextMatrix(nRol3, 2, _T(""));
	}*/

	Pos = CurrentSet->VF_TestInfo.GetHeadPosition();

	while (Pos)
	{
		pVFTestInfo = CurrentSet->VF_TestInfo.GetNext(Pos);
		bRun = pVFTestInfo->bRun;
		nChNo = pVFTestInfo->nChNo;
		strTestName = pVFTestInfo->strTestName;
		nCheckType = pVFTestInfo->nCheckType;
		dLowerLimit = pVFTestInfo->dLowerLimit;
		dUpperLimit = pVFTestInfo->dUpperLimit;
		nTime = pVFTestInfo->nTime;

		if (nCheckType == 2) {
			m_ctrlComboTime.SetCurSel(nTime);
		}

		if ((nChNo >= 1)&&(nChNo <= 30)) {
			//m_ctrlVoltSpecGrid.SetRow(nChNo);

		//	m_ctrlVoltSpecGrid.SetCol(0);
			nRol = nChNo - 1;

			if (bRun) {
				m_ctrlVoltSpecList.SetCheck(nRol);
				m_ctrlVoltSpecList.SetItem(nRol, 1, LVIF_TEXT, strTestName, NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 2, strTestName);

				sTmp.Format(_T("%d"), nCheckType);
				m_ctrlVoltSpecList.SetItem(nRol, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 3, sTmp);


				sTmp.Format(_T("%.1f"), dLowerLimit);
				m_ctrlVoltSpecList.SetItem(nRol, 3, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 4, sTmp);

				if (nCheckType == 2) {
					sTmp.Format(_T("%.1f"), dUpperLimit);
					m_ctrlVoltSpecList.SetItem(nRol, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 5, sTmp);
				}
			}
			else
			{
				m_ctrlVoltSpecList.SetCheck(0);
				m_ctrlVoltSpecList.SetItem(nRol, 1, LVIF_TEXT, "", NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 2, strTestName);

				m_ctrlVoltSpecList.SetItem(nRol, 2, LVIF_TEXT, "", NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 3, sTmp);

				m_ctrlVoltSpecList.SetItem(nRol, 3, LVIF_TEXT, "", NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 4, sTmp);
				m_ctrlVoltSpecList.SetItem(nRol, 4, LVIF_TEXT, "", NULL, NULL, NULL, NULL); //m_ctrlVoltSpecGrid.SetTextMatrix(nChNo, 5, sTmp);
			}


		}
		else if (nChNo > 30 && nChNo <= 42) {
			/*m_ctrlVoltSpecGrid2.SetRow(nChNo - 30);

			m_ctrlVoltSpecGrid2.SetCol(0);

			if (bRun) {
				m_ctrlVoltSpecGrid2.SetCellChecked(flexChecked);
			}

			m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 2, strTestName);

			sTmp.Format(_T("%d"), nCheckType);
			m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 3, sTmp);


			sTmp.Format(_T("%.1f"), dLowerLimit);
			m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 4, sTmp);

			if (nCheckType == 2) {
				sTmp.Format(_T("%.1f"), dUpperLimit);
				m_ctrlVoltSpecGrid2.SetTextMatrix(nChNo - 30, 5, sTmp);
			}*/
		}
		else if (nChNo > 100) {
			/*m_ctrlVoltSpecGrid3.SetRow(nChNo - 100);

			m_ctrlVoltSpecGrid3.SetCol(0);

			if (bRun) {
				m_ctrlVoltSpecGrid3.SetCellChecked(flexChecked);
			}

			m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 2, strTestName);

			sTmp.Format(_T("%d"), nCheckType);
			m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 3, sTmp);


			sTmp.Format(_T("%.1f"), dLowerLimit);
			m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 4, sTmp);

			if (nCheckType == 2) {
				sTmp.Format(_T("%.1f"), dUpperLimit);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nChNo - 100, 5, sTmp);
			}*/
		}

	}

	//m_ctrlVoltSpecGrid.SetRow(-1);
	//m_ctrlVoltSpecGrid.SetCol(-1);
	//m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	//m_ctrlVoltSpecGrid.Refresh();

	//m_ctrlVoltSpecGrid2.SetRow(-1);
	//m_ctrlVoltSpecGrid2.SetCol(-1);
	//m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	//m_ctrlVoltSpecGrid2.Refresh();

	//m_ctrlVoltSpecGrid3.SetRow(-1);
	//m_ctrlVoltSpecGrid3.SetCol(-1);
	//m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	//m_ctrlVoltSpecGrid3.Refresh();
}



BOOL COptVoltMeasureDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	InitVoltSpecGrid();

	InitVoltSpecList();

	m_ctrlComboTime.SetCurSel(1);
	if(FileExists(CurrentSet->sVFTestIni))
	{
		ConfigDisplay();
		ConfigDisplayList();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COptVoltMeasureDlg::OnBnClickedBtnSave()
{
	CString sMsg = _T("");

		if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sMsg = _T("");
		sMsg.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sMsg); return;
	}
	WriteConfig(m_strConfigFile);
	OpenVFTestFile(CurrentSet->sVFTestIni);

}
BEGIN_EVENTSINK_MAP(COptVoltMeasureDlg, CPropertyPage)
	ON_EVENT(COptVoltMeasureDlg, IDC_VOLT_SPEC_GRID, DISPID_CLICK, ClickVoltSpecGrid, VTS_NONE)
	ON_EVENT(COptVoltMeasureDlg, IDC_VOLT_SPEC_GRID2, DISPID_CLICK, COptVoltMeasureDlg::ClickVoltSpecGrid2, VTS_NONE)
	ON_EVENT(COptVoltMeasureDlg, IDC_VOLT_SPEC_GRID3, DISPID_CLICK, COptVoltMeasureDlg::ClickVoltSpecGrid3, VTS_NONE)


	ON_EVENT(COptVoltMeasureDlg, IDC_VOLT_SPEC_GRID, DISPID_DBLCLICK, COptVoltMeasureDlg::DblClickVoltSpecGrid, VTS_NONE)
	ON_EVENT(COptVoltMeasureDlg, IDC_VOLT_SPEC_GRID2, DISPID_DBLCLICK, COptVoltMeasureDlg::DblClickVoltSpecGrid2, VTS_NONE)
	ON_EVENT(COptVoltMeasureDlg, IDC_VOLT_SPEC_GRID3, DISPID_DBLCLICK, COptVoltMeasureDlg::DblClickVoltSpecGrid3, VTS_NONE)
END_EVENTSINK_MAP()


void COptVoltMeasureDlg::ClickVoltSpecGrid()
{
	if(CurrentSet->bIsRunning) return;

 	int nCol = m_ctrlVoltSpecGrid.GetCol();
	int nRow = m_ctrlVoltSpecGrid.GetRow();
	if(nCol > m_ctrlVoltSpecGrid.GetCols()) return;
	if(nRow > m_ctrlVoltSpecGrid.GetRows()) return;


	if((nRow > 0) && (nCol == 0))
	{
		if(nRow == 10 || nRow==20 || nRow== 30 || nRow == 43 || nRow == 44){
			m_ctrlVoltSpecGrid.SetCol(nCol);
			m_ctrlVoltSpecGrid.SetRow(nRow);
			if(m_ctrlVoltSpecGrid.GetCellChecked() == flexChecked){
				m_ctrlVoltSpecGrid.SetCellChecked(flexNoCheckbox);
			}
		}
		else if (nRow == 27 || nRow == 28 ) {
			
		}
		else 
		{
			m_ctrlVoltSpecGrid.SetCol(nCol);
			m_ctrlVoltSpecGrid.SetRow(nRow);
			if (m_ctrlVoltSpecGrid.GetCellChecked() != flexChecked) {
				m_ctrlVoltSpecGrid.SetCellChecked(flexChecked);
			}
		}

	}
	else if((nRow > 0) && (nCol == 3))
	{
		return;
	}
}
void COptVoltMeasureDlg::ClickVoltSpecGrid2()
{
	if(CurrentSet->bIsRunning) return;

 	int nCol = m_ctrlVoltSpecGrid2.GetCol();
	int nRow = m_ctrlVoltSpecGrid2.GetRow();
	if(nCol > m_ctrlVoltSpecGrid2.GetCols()) return;
	if(nRow > m_ctrlVoltSpecGrid2.GetRows()) return;


	if((nRow > 0) && (nCol == 0))
	{
		if( nRow == 13 || nRow == 14){
			m_ctrlVoltSpecGrid2.SetCol(nCol);
			m_ctrlVoltSpecGrid2.SetRow(nRow);
			if(m_ctrlVoltSpecGrid2.GetCellChecked() == flexChecked){
				m_ctrlVoltSpecGrid2.SetCellChecked(flexNoCheckbox);
			}
		}
		else if (nRow == 1 || nRow == 2) {

		}
		else
		{
			m_ctrlVoltSpecGrid2.SetCol(nCol);
			m_ctrlVoltSpecGrid2.SetRow(nRow);
			if (m_ctrlVoltSpecGrid2.GetCellChecked() != flexChecked) {
				m_ctrlVoltSpecGrid2.SetCellChecked(flexChecked);
			}
		}
	}
	else if((nRow > 0) && (nCol == 3))
	{
		return;
	}
}
void COptVoltMeasureDlg::ClickVoltSpecGrid3()
{
	if(CurrentSet->bIsRunning) return;

 	int nCol = m_ctrlVoltSpecGrid3.GetCol();
	int nRow = m_ctrlVoltSpecGrid3.GetRow();
	if(nCol > m_ctrlVoltSpecGrid3.GetCols()) return;
	if(nRow > m_ctrlVoltSpecGrid3.GetRows()) return;


	if((nRow > 0) && (nCol == 0))
	{
		
		m_ctrlVoltSpecGrid3.SetCol(nCol);
		m_ctrlVoltSpecGrid3.SetRow(nRow);
		if (m_ctrlVoltSpecGrid3.GetCellChecked() != flexChecked) {
			m_ctrlVoltSpecGrid3.SetCellChecked(flexChecked);
		}
		
	}
	
}


BOOL COptVoltMeasureDlg::WriteConfig( CString strConfigName)
{
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sBuf;
//	CString sDay;
//	CString sMonth;
	CString filename;
//	CString sModel;
//	CString sTemp2;
//	int nPos;

	CStdioFile stdFileSave;
	CStringArray saDif;
	BOOL bCellCheck = FALSE;
	int nCol;
	int nRow;
	CString sJigPint;
	int nType = 1;
	//int nMargin = 1;
	CString sType;
	CString sMargin;
	CString sSpec1;
	CString sSpec2;
	CString sSpec3;
	int nTime = 0;

	for(nRow = 1; nRow < 31; nRow++)
	{
		if(nRow == 10 || nRow == 20 || nRow == 30){continue;}
		m_ctrlVoltSpecGrid.SetRow(nRow);
		m_ctrlVoltSpecGrid.SetCol(0);
		if(m_ctrlVoltSpecGrid.GetCellChecked() == flexChecked	){
			bCellCheck = TRUE;
		}
		else{
		
			bCellCheck = FALSE;
			continue;
			
		}

		sJigPint = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,2);

		m_ctrlVoltSpecGrid.SetCol(3);
		sType = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,3);
		nType = atoi(sType);
		sSpec1 = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,4);
		sSpec2 = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,5);

		sMargin = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,7);
		//nMargin = atoi(sMargin);

		if(sSpec1 == _T("")){
			sTemp.Format(_T("CH%d -TARGET1 : No value entered."), nRow);
			AfxMessageBox(sTemp);  
			return FALSE;
		}

		if(nType == 1){
			nTime = 0;
		}
		else{
			if(sSpec2 == _T("")){
				sTemp.Format(_T("CH%d -TARGET2 : No value entered."), nRow);
				AfxMessageBox(sTemp);  
				return FALSE;
			}
			nTime = m_ctrlComboTime.GetCurSel();

			sMargin = "0";
		}
			
		sBuf.Format(_T("%d:%d:%s:%s:%s:%s:%d:%s"), bCellCheck, nRow, sJigPint, sType, sSpec1, sSpec2, nTime, sMargin);
		saDif.Add(sBuf);
	}
	for(nRow = 1; nRow < 13; nRow++)
	{
		m_ctrlVoltSpecGrid2.SetRow(nRow);
		m_ctrlVoltSpecGrid2.SetCol(0);
		if(m_ctrlVoltSpecGrid2.GetCellChecked() == flexChecked	){
			bCellCheck = TRUE;
		}
		else{
			bCellCheck = FALSE;
			continue;
		}

		sJigPint = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,2);

		m_ctrlVoltSpecGrid2.SetCol(3);
		sType = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,3);
		nType = atoi(sType);
		sSpec1 = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,4);
		sSpec2 = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,5);

		sMargin = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow, 7);
	//	nMargin = atoi(sMargin);


		if(sSpec1 == _T("")){
			sTemp.Format(_T("CH%d -SPEC1 : No value entered."), nRow+30);
			AfxMessageBox(sTemp);  
			return FALSE;
		}
		if(nType == 1){
			nTime = 0;
		}
		else{
			if(sSpec2 == _T("")){
				sTemp.Format(_T("CH%d -SPEC2 : No value entered."), nRow+30);
				AfxMessageBox(sTemp);  
				return FALSE;
			}
			nTime = m_ctrlComboTime.GetCurSel();

			sMargin = "0";
		}

		
		sBuf.Format(_T("%d:%d:%s:%s:%s:%s:%d:%s"), bCellCheck, nRow + 30, sJigPint, sType, sSpec1, sSpec2, nTime, sMargin);
		//sBuf.Format(_T("%d:%d:%s:%s:%s:%s:%d"), bCellCheck, nRow + 30, sJigPint, sType, sSpec1, sSpec2, nTime);
		saDif.Add(sBuf);
	}

	for(nRow = 1; nRow < 7; nRow++)
	{
		m_ctrlVoltSpecGrid3.SetRow(nRow);
		m_ctrlVoltSpecGrid3.SetCol(0);
		if(m_ctrlVoltSpecGrid3.GetCellChecked() == flexChecked	){
			bCellCheck = TRUE;
		}
		else{
			bCellCheck = FALSE;
			continue;
		}

		sJigPint = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,2);

		m_ctrlVoltSpecGrid3.SetCol(3);
		sType = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,3);
		nType = atoi(sType);
		sSpec1 = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,4);
		sSpec2 = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,5);

		sMargin = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow, 7);
	//	nMargin = atoi(sMargin);

		if(sSpec1 == _T("")){
			sTemp.Format(_T("F_CH%d -SPEC1 : No value entered."), nRow+30);
			AfxMessageBox(sTemp);  
			return FALSE;
		}
		if(nType == 1){
			nTime = 0;
		}
		else{
			if(sSpec2 == _T("")){
				sTemp.Format(_T("F_CH%d -SPEC2 : No value entered."), nRow+30);
				AfxMessageBox(sTemp);  
				return FALSE;
			}
			nTime = m_ctrlComboTime.GetCurSel();
			sMargin = "0";
		}

		sBuf.Format(_T("%d:%d:%s:%s:%s:%s:%d:%s"), bCellCheck , nRow+ 100, sJigPint, sType, sSpec1, sSpec2, nTime, sMargin);
		//sBuf.Format(_T("%d:%d:%s:%s:%s:%s:%d"), bCellCheck, nRow + 100, sJigPint, sType, sSpec1, sSpec2, nTime);
		saDif.Add(sBuf);
	}


	filename = strConfigName;

	if(!stdFileSave.Open(filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))
	{
        return FALSE;
    }
	stdFileSave.SeekToBegin(); 

    TRY 
	{
		for(int j = 0 ; j < saDif.GetSize() ; j++)
		{
			stdFileSave.WriteString(saDif.GetAt(j));
			stdFileSave.WriteString(_T("\n"));
		}
		stdFileSave.Close();
    }
    CATCH (CFileException, e) 
	{
		sTemp = _T("Error occurs while saving VF-test information");
        AfxMessageBox(sTemp);  
        e->Delete();
        return FALSE;
    }
    END_CATCH


    return TRUE;
	
}



void COptVoltMeasureDlg::OnBnClickedButtonTestall()
{
	double dVol = 0.0;
	double dVol2 = 0.0;
	CString sVol;
	int nCH;
	int nType;
	CString sType;
	int nCmbIndex;
	int nWait;

	if(CurrentSet->bIsRunning) return;

 	int nCol;
	int nRow;

	TVCommCtrl.Send_TVControl_Command("aa 00 00", 700);
	TVCommCtrl.Send_TVControl_Command("ai 00 30", 700);
	TVCommCtrl.Send_TVControl_Command("aa 00 20", 700);
	TVCommCtrl.Send_TVControl_Command("ar 00 30", 700);

	for(nRow = 1; nRow < 30; nRow++){
		if(nRow == 10 || nRow==20 || nRow== 30){
			continue;
		}
		Sleep(100);
		nCH = nRow;
		m_ctrlVoltSpecGrid.SetRow(nRow);
		m_ctrlVoltSpecGrid.SetCol(0);
		if(m_ctrlVoltSpecGrid.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				if(gVF1000Ctrl.GetVoltageData(nCH,dVol)){
					sVol.Format(_T("%.1f"), dVol);
				}
				else{ 
					m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			else if(nType == 2){
				nCmbIndex = m_ctrlComboTime.GetCurSel();
				nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
				if(gVF1000Ctrl.GetVoltageData2(nCH, nWait, dVol, dVol2)){
					sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
				}
				else{ 
					m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, sVol);
		}
		m_ctrlVoltSpecGrid.SetRedraw(TRUE);
		m_ctrlVoltSpecGrid.Refresh();
	}

	for(nRow = 1; nRow < 13; nRow++){
		Sleep(100);
		nCH = nRow + 30;
		m_ctrlVoltSpecGrid2.SetRow(nRow);
		m_ctrlVoltSpecGrid2.SetCol(0);
		if(m_ctrlVoltSpecGrid2.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				if(gVF1000Ctrl.GetVoltageData(nCH,dVol)){
					sVol.Format(_T("%.1f"), dVol);
				}
				else{ 
					m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			else if(nType == 2){
				nCmbIndex = m_ctrlComboTime.GetCurSel();
				nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
				if(gVF1000Ctrl.GetVoltageData2(nCH, nWait, dVol, dVol2)){
					sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
				}
				else{ 
					m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, sVol);
		}
		m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
		m_ctrlVoltSpecGrid2.Refresh();
	}
	for(nRow = 1; nRow < 7; nRow++){
		Sleep(100);
		nCH = nRow;
		m_ctrlVoltSpecGrid3.SetRow(nRow);
		m_ctrlVoltSpecGrid3.SetCol(0);
		if(m_ctrlVoltSpecGrid3.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				if(gVF1000Ctrl.GetFrequencyData(nCH,dVol)){
					sVol.Format(_T("%.1f"), dVol);
				}
				else{ 
					m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			else if(nType == 2){
				nCmbIndex = m_ctrlComboTime.GetCurSel();
				nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
				if(gVF1000Ctrl.GetFrequencyData2(nCH, nWait, dVol, dVol2)){
					sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
				}
				else{ 
					m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, sVol);
		}
		m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
		m_ctrlVoltSpecGrid3.Refresh();
	}
	m_ctrlVoltSpecGrid.SetRow(-1);
	m_ctrlVoltSpecGrid.SetCol(-1);
	m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid.Refresh();

	m_ctrlVoltSpecGrid2.SetRow(-1);
	m_ctrlVoltSpecGrid2.SetCol(-1);
	m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid2.Refresh();

	m_ctrlVoltSpecGrid3.SetRow(-1);
	m_ctrlVoltSpecGrid3.SetCol(-1);
	m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid3.Refresh();

	bTestAll = TRUE;
}


void COptVoltMeasureDlg::OnBnClickedButtonTestallList()
{
	double dVol = 0.0;
	double dVol2 = 0.0;
	CString sVol;
	int nCH;
	int nType;
	CString sType;
	int nCmbIndex;
	int nWait;

	if(CurrentSet->bIsRunning) return;

 	int nCol;
	int nRow;

	TVCommCtrl.Send_TVControl_Command("aa 00 00", 700);
	TVCommCtrl.Send_TVControl_Command("ai 00 30", 700);
	TVCommCtrl.Send_TVControl_Command("aa 00 20", 700);
	TVCommCtrl.Send_TVControl_Command("ar 00 30", 700);

	for(nRow = 1; nRow < 30; nRow++){
		if(nRow == 10 || nRow==20 || nRow== 30){
			continue;
		}
		Sleep(100);
		nCH = nRow;
		m_ctrlVoltSpecGrid.SetRow(nRow);
		m_ctrlVoltSpecGrid.SetCol(0);
		if(m_ctrlVoltSpecGrid.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				if(gVF1000Ctrl.GetVoltageData(nCH,dVol)){
					sVol.Format(_T("%.1f"), dVol);
				}
				else{ 
					m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			else if(nType == 2){
				nCmbIndex = m_ctrlComboTime.GetCurSel();
				nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
				if(gVF1000Ctrl.GetVoltageData2(nCH, nWait, dVol, dVol2)){
					sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
				}
				else{ 
					m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, sVol);
		}
		m_ctrlVoltSpecGrid.SetRedraw(TRUE);
		m_ctrlVoltSpecGrid.Refresh();
	}

	for(nRow = 1; nRow < 13; nRow++){
		Sleep(100);
		nCH = nRow + 30;
		m_ctrlVoltSpecGrid2.SetRow(nRow);
		m_ctrlVoltSpecGrid2.SetCol(0);
		if(m_ctrlVoltSpecGrid2.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				if(gVF1000Ctrl.GetVoltageData(nCH,dVol)){
					sVol.Format(_T("%.1f"), dVol);
				}
				else{ 
					m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			else if(nType == 2){
				nCmbIndex = m_ctrlComboTime.GetCurSel();
				nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
				if(gVF1000Ctrl.GetVoltageData2(nCH, nWait, dVol, dVol2)){
					sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
				}
				else{ 
					m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, sVol);
		}
		m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
		m_ctrlVoltSpecGrid2.Refresh();
	}
	for(nRow = 1; nRow < 7; nRow++){
		Sleep(100);
		nCH = nRow;
		m_ctrlVoltSpecGrid3.SetRow(nRow);
		m_ctrlVoltSpecGrid3.SetCol(0);
		if(m_ctrlVoltSpecGrid3.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				if(gVF1000Ctrl.GetFrequencyData(nCH,dVol)){
					sVol.Format(_T("%.1f"), dVol);
				}
				else{ 
					m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			else if(nType == 2){
				nCmbIndex = m_ctrlComboTime.GetCurSel();
				nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
				if(gVF1000Ctrl.GetFrequencyData2(nCH, nWait, dVol, dVol2)){
					sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
				}
				else{ 
					m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, "error");
					return;
				}
			}
			m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, sVol);
		}
		m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
		m_ctrlVoltSpecGrid3.Refresh();
	}
	m_ctrlVoltSpecGrid.SetRow(-1);
	m_ctrlVoltSpecGrid.SetCol(-1);
	m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid.Refresh();

	m_ctrlVoltSpecGrid2.SetRow(-1);
	m_ctrlVoltSpecGrid2.SetCol(-1);
	m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid2.Refresh();

	m_ctrlVoltSpecGrid3.SetRow(-1);
	m_ctrlVoltSpecGrid3.SetCol(-1);
	m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid3.Refresh();

	bTestAll = TRUE;
}


void COptVoltMeasureDlg::DblClickVoltSpecGrid()
{
	double dVol = 0.0;
	double dVol2 = 0.0;
	CString sVol;
	int nCH;
	int nType;
	CString sType;
	int nCmbIndex;
	int nWait;
	BOOL bRev = FALSE;

	if(CurrentSet->bIsRunning) return;

 	int nCol = m_ctrlVoltSpecGrid.GetCol();
	int nRow = m_ctrlVoltSpecGrid.GetRow();
	if(nCol > m_ctrlVoltSpecGrid.GetCols()) return;
	if(nRow > m_ctrlVoltSpecGrid.GetRows()) return;


	if((nRow > 0) && (nCol == 6))
	{
		if(nRow == 10 || nRow==20 || nRow== 30){
			return;
		}

		nCH = nRow;
		sType = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,3);
		nType = atoi(sType);

		if(nType == 1){
			bRev = gVF1000Ctrl.GetVoltageData(nCH,dVol);
			if(bRev){
				sVol.Format(_T("%.1f"), dVol);
			}
			else{ sVol = _T("error");}
		}
		else if(nType == 2){
			nCmbIndex = m_ctrlComboTime.GetCurSel();
			nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
			bRev = gVF1000Ctrl.GetVoltageData2(nCH, nWait, dVol, dVol2);
			if(bRev){
				sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
			}
			else{ sVol = _T("error");}
		}
		m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, sVol);

	}
	m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid.Refresh();
}


void COptVoltMeasureDlg::DblClickVoltSpecGrid2()
{
	double dVol = 0.0;
	double dVol2 = 0.0;
	CString sVol;
	int nCH;
	int nType;
	CString sType;
	int nCmbIndex;
	int nWait;
	BOOL bRev = FALSE;

	if(CurrentSet->bIsRunning) return;

 	int nCol = m_ctrlVoltSpecGrid2.GetCol();
	int nRow = m_ctrlVoltSpecGrid2.GetRow();
	if(nCol > m_ctrlVoltSpecGrid2.GetCols()) return;
	if(nRow > m_ctrlVoltSpecGrid2.GetRows()) return;


	if((nRow > 0) && (nCol == 6))
	{
		if(nRow == 13 || nRow==14){
			return;
		}

		nCH = nRow + 30;
		sType = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,3);
		nType = atoi(sType);

		if(nType == 1){
			bRev = gVF1000Ctrl.GetVoltageData(nCH,dVol);
			if(bRev){
				sVol.Format(_T("%.1f"), dVol);
			}
			else{ sVol = _T("error");}
		}
		else if(nType == 2){
			nCmbIndex = m_ctrlComboTime.GetCurSel();
			nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
			bRev = gVF1000Ctrl.GetVoltageData2(nCH, nWait, dVol, dVol2);
			if(bRev){
				sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
			}
			else{ sVol = _T("error");}
		}
		m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, sVol);
	}
	m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid2.Refresh();
}


void COptVoltMeasureDlg::DblClickVoltSpecGrid3()
{
	double dVol = 0.0;
	double dVol2 = 0.0;
	CString sVol;
	int nCH;
	int nType;
	CString sType;
	int nCmbIndex;
	int nWait;
	BOOL bRev = FALSE;

	if(CurrentSet->bIsRunning) return;

 	int nCol = m_ctrlVoltSpecGrid3.GetCol();
	int nRow = m_ctrlVoltSpecGrid3.GetRow();
	if(nCol > m_ctrlVoltSpecGrid3.GetCols()) return;
	if(nRow > m_ctrlVoltSpecGrid3.GetRows()) return;


	if((nRow > 0) && (nCol == 6))
	{
		if(nRow == 13 || nRow==14){
			return;
		}

//		nCH = nRow + 30;
		nCH = nRow;
		sType = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,3);
		nType = atoi(sType);

		if(nType == 1){
			bRev = gVF1000Ctrl.GetFrequencyData(nCH,dVol);
			if(bRev){
				sVol.Format(_T("%.1f"), dVol);
			}
			else{ sVol = _T("error");}
		}
		else if(nType == 2){
			nCmbIndex = m_ctrlComboTime.GetCurSel();
			nWait = ((nCmbIndex + 1) - (nCmbIndex * 0.5)) * 1000;
			bRev = gVF1000Ctrl.GetFrequencyData2(nCH, nWait, dVol, dVol2);
			if(bRev){
				sVol.Format(_T("%.1f,%.1f"), dVol, dVol2);
			}
			else{ sVol = _T("error");}
		}
		m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, sVol);
	}
	m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid3.Refresh();
}


void COptVoltMeasureDlg::OnBnClickedButtonTestcls()
{
	int nRow;

	for(nRow = 1; nRow < 30; nRow++){
		m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 6, _T(""));
	}

	for(nRow = 1; nRow < 13; nRow++){
		m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 6, _T(""));
	}

	for(nRow = 1; nRow < 7; nRow++){
		m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 6, _T(""));
	}

	bTestAll = FALSE;

	m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid.Refresh();

	m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid2.Refresh();

	m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid3.Refresh();
}


void COptVoltMeasureDlg::OnBnClickedBtnResultcopy()
{
	double dVol = 0.0;
	double dVol2 = 0.0;
	CString sVol1;
	CString sVol2;
	int nType;
	CString sType;
	CString sTemp;

	if(!bTestAll) return;

 	int nCol;
	int nRow;

	for(nRow = 1; nRow < 30; nRow++){
		if(nRow == 10 || nRow==20 || nRow== 30){
			continue;
		}

		m_ctrlVoltSpecGrid.SetRow(nRow);
		m_ctrlVoltSpecGrid.SetCol(0);
		if(m_ctrlVoltSpecGrid.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				sVol1 = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,6);
				m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 4, sVol1);
			}
			else if(nType == 2){
				sTemp = m_ctrlVoltSpecGrid.GetTextMatrix(nRow,6);
				sVol1 =	sTemp.Left(sTemp.Find(",", 0));
				sTemp.Delete(0, sTemp.Find(",", 0) + 1);
				sVol1.TrimRight(); sVol1.TrimLeft();
				sVol2 = sTemp.Left(sTemp.GetLength());
				sVol2.TrimRight(); sVol2.TrimLeft();
				m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 4, sVol1);
				m_ctrlVoltSpecGrid.SetTextMatrix(nRow, 5, sVol2);
			}
		}

	}

	for(nRow = 1; nRow < 13; nRow++){
		m_ctrlVoltSpecGrid2.SetRow(nRow);
		m_ctrlVoltSpecGrid2.SetCol(0);
		if(m_ctrlVoltSpecGrid2.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				sVol1 = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,6);
				m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 4, sVol1);
			}
			else if(nType == 2){
				sTemp = m_ctrlVoltSpecGrid2.GetTextMatrix(nRow,6);
				sVol1 =	sTemp.Left(sTemp.Find(",", 0));
				sTemp.Delete(0, sTemp.Find(",", 0) + 1);
				sVol1.TrimRight(); sVol1.TrimLeft();
				sVol2 = sTemp.Left(sTemp.GetLength());
				sVol2.TrimRight(); sVol2.TrimLeft();
				m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 4, sVol1);
				m_ctrlVoltSpecGrid2.SetTextMatrix(nRow, 5, sVol2);
			}
		}
	}
	for(nRow = 1; nRow < 7; nRow++){
		m_ctrlVoltSpecGrid3.SetRow(nRow);
		m_ctrlVoltSpecGrid3.SetCol(0);
		if(m_ctrlVoltSpecGrid3.GetCellChecked() == flexChecked	){
			sType = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,3);
			nType = atoi(sType);

			if(nType == 1){
				sVol1 = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,6);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 4, sVol1);
			}
			else if(nType == 2){
				sTemp = m_ctrlVoltSpecGrid3.GetTextMatrix(nRow,6);
				sVol1 =	sTemp.Left(sTemp.Find(",", 0));
				sTemp.Delete(0, sTemp.Find(",", 0) + 1);
				sVol1.TrimRight(); sVol1.TrimLeft();
				sVol2 = sTemp.Left(sTemp.GetLength());
				sVol2.TrimRight(); sVol2.TrimLeft();
				m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 4, sVol1);
				m_ctrlVoltSpecGrid3.SetTextMatrix(nRow, 5, sVol2);
			}
		}
	}
	m_ctrlVoltSpecGrid.SetRow(-1);
	m_ctrlVoltSpecGrid.SetCol(-1);
	m_ctrlVoltSpecGrid.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid.Refresh();

	m_ctrlVoltSpecGrid2.SetRow(-1);
	m_ctrlVoltSpecGrid2.SetCol(-1);
	m_ctrlVoltSpecGrid2.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid2.Refresh();

	m_ctrlVoltSpecGrid3.SetRow(-1);
	m_ctrlVoltSpecGrid3.SetCol(-1);
	m_ctrlVoltSpecGrid3.SetRedraw(TRUE);
	m_ctrlVoltSpecGrid3.Refresh();
}


void COptVoltMeasureDlg::OnBnClickedButtonSelectall()
{
	int nRow;

	for(nRow = 1; nRow < 30; nRow++){
		if(nRow == 10 || nRow==20 || nRow== 30){
			continue;
		}

		m_ctrlVoltSpecGrid.SetCol(0);
		m_ctrlVoltSpecGrid.SetRow(nRow);
		m_ctrlVoltSpecGrid.SetCellChecked(flexChecked);
	}

	for(nRow = 1; nRow < 13; nRow++){
		m_ctrlVoltSpecGrid2.SetCol(0);
		m_ctrlVoltSpecGrid2.SetRow(nRow);
		m_ctrlVoltSpecGrid2.SetCellChecked(flexChecked);
	}

	for(nRow = 1; nRow < 7; nRow++){
		m_ctrlVoltSpecGrid3.SetCol(0);
		m_ctrlVoltSpecGrid3.SetRow(nRow);
		m_ctrlVoltSpecGrid3.SetCellChecked(flexChecked);
	}

}


void COptVoltMeasureDlg::OnBnClickedButtonUnselectall()
{
	int nRow;
#if 1
	for(nRow = 27; nRow < 29; nRow++){
		if(nRow == 10 || nRow==20 || nRow== 30){
			continue;
		}

		m_ctrlVoltSpecGrid.SetCol(0);
		m_ctrlVoltSpecGrid.SetRow(nRow);
		m_ctrlVoltSpecGrid.SetCellChecked(flexUnchecked);
	}

	for(nRow = 1; nRow < 3; nRow++){
		m_ctrlVoltSpecGrid2.SetCol(0);
		m_ctrlVoltSpecGrid2.SetRow(nRow);
		m_ctrlVoltSpecGrid2.SetCellChecked(flexUnchecked);
	}

	//for(nRow = 1; nRow < 7; nRow++){
	//	m_ctrlVoltSpecGrid3.SetCol(0);
	//	m_ctrlVoltSpecGrid3.SetRow(nRow);
	//	m_ctrlVoltSpecGrid3.SetCellChecked(flexUnchecked);
	//}

#else
	for (nRow = 1; nRow < 30; nRow++) {
		if (nRow == 10 || nRow == 20 || nRow == 30) {
			continue;
		}

		m_ctrlVoltSpecGrid.SetCol(0);
		m_ctrlVoltSpecGrid.SetRow(nRow);
		m_ctrlVoltSpecGrid.SetCellChecked(flexUnchecked);
}

	for (nRow = 1; nRow < 13; nRow++) {
		m_ctrlVoltSpecGrid2.SetCol(0);
		m_ctrlVoltSpecGrid2.SetRow(nRow);
		m_ctrlVoltSpecGrid2.SetCellChecked(flexUnchecked);
	}

	for (nRow = 1; nRow < 7; nRow++) {
		m_ctrlVoltSpecGrid3.SetCol(0);
		m_ctrlVoltSpecGrid3.SetRow(nRow);
		m_ctrlVoltSpecGrid3.SetCellChecked(flexUnchecked);
	}
#endif
}


void COptVoltMeasureDlg::OnBnClickedBtbGrabstop()
{
	g_pView->PostMessage(WM_COMMAND, ID_GRAB_STOP);
}


void COptVoltMeasureDlg::OnBnClickedBtbGrabstart()
{
	g_pView->PostMessage(WM_COMMAND, ID_GRAB_START);
}


void COptVoltMeasureDlg::OnBnClickedBtnSendCommand2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	TVCommCtrl.m_nWaitLimit = 3000;

	CString sErrMsg		= _T("");
	
	CString sTVComPort  = CurrentSet->sTVComPort;
	DWORD	wTVBaudRate = CurrentSet->wTVBaudRate;

	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(sTVComPort,wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();

			if(TVCommCtrl.Create(sTVComPort,wTVBaudRate) == FALSE)
			{
				sErrMsg.Format("Failed to open COM port (%s)",sTVComPort);
				AfxMessageBox(sErrMsg);
			}
		}
	}	

	int nResult = TVCommCtrl.Send_TVControl_Command (m_szStrCmd,3000);
}


void COptVoltMeasureDlg::OnBnClickedBtnSendReadyCommand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	TVCommCtrl.Send_TVControl_Command("aa 00 00", 700);
	TVCommCtrl.Send_TVControl_Command("ai 00 30", 700);
	TVCommCtrl.Send_TVControl_Command("aa 00 20", 700);
	TVCommCtrl.Send_TVControl_Command("ar 00 30", 700);
}
