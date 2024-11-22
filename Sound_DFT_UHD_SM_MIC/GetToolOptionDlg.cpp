// GetToolOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "GetToolOptionDlg.h"
#include "Global.h"
//#include "MesDb.h"
#include "Gmes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern CMesDb		m_MesDb;
extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// GetToolOptionDlg dialog


GetToolOptionDlg::GetToolOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GetToolOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(GetToolOptionDlg)
	m_sModelSuffix = _T("");
	//}}AFX_DATA_INIT
}


void GetToolOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetToolOptionDlg)
	DDX_Text(pDX, IDC_EDIT_MODEL_SUFFIX, m_sModelSuffix);
	DDX_Control(pDX, IDC_VSFLEXGRIDL1, m_ctrlSpecGrid1);
	DDX_Control(pDX, IDC_VSFLEXGRIDL2, m_ctrlOptionGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_INDEX_OPTION, m_CtrlListIndexOption);
}


BEGIN_MESSAGE_MAP(GetToolOptionDlg, CDialog)
	//{{AFX_MSG_MAP(GetToolOptionDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_UPDATE_TOOL_OPTION, GetToolOption)
	ON_MESSAGE(UM_CLOSE_TOOL_OPTION, CloseToolOption)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_OPT_GET, &GetToolOptionDlg::OnBnClickedButtonOptGet)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &GetToolOptionDlg::OnBnClickedButtonUpdate)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INDEX_OPTION, &GetToolOptionDlg::OnNMClickListIndexOption)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_INDEX_OPTION, &GetToolOptionDlg::OnNMCustomdrawListIndexOption)
	ON_BN_CLICKED(IDOK, &GetToolOptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetToolOptionDlg message handlers

BOOL GetToolOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nRetryCount = 0;
	m_bActivate = TRUE;
	m_sModelSuffix = "";
	InitSpecGrid();

	InitIndexGrid();
	UpdateData(FALSE);
	
	if (g_nSysType == AUTO_SYS) {
		if (CurrentSet->sModelSuffixName.Find(".") >= 0)
		{
			SetTimer(1, 500, NULL);
		}
	}

	if ((g_nSysType == AUTO_SYS) && (g_nUseNoScanType == FALSE))
	{
		GetDlgItem(IDC_BUTTON_OPT_GET)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_OPT_GET)->EnableWindow(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GetToolOptionDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_bActivate = FALSE;
	
}


BOOL GetToolOptionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)//m_sModelSuffix
	{
		if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam == VK_RETURN)
		{
			//ClickCbtnStart();
			int lDel = 0;
			for (int i = 0; i < m_sModelSuffix.GetLength(); i++)
			{
				if ((m_sModelSuffix.GetAt(i) >= 0x20) && (m_sModelSuffix.GetAt(i) <= 0x60))
				{
				}
				else
				{
					lDel = 1;
					break;
				}
			}
			if ((m_sModelSuffix.Find(".") > 0)&&(lDel == 0))
			{
				CurrentSet->sModelSuffixName = m_sModelSuffix;
				SendMessage(UM_UPDATE_TOOL_OPTION, 0, 0);
			}
			else
			{
				m_sModelSuffix = m_sTempModelSuffix = "";
				UpdateData(FALSE);
			}
		
			
			m_sTempModelSuffix = _T("");
			return TRUE;
		}
	}
	else if (pMsg->message == WM_CHAR)
	{
		m_sTempModelSuffix = m_sTempModelSuffix + (char)(pMsg->wParam);
		m_sTempModelSuffix.MakeUpper();
		m_sModelSuffix = m_sTempModelSuffix;
		//m_sModelSuffix.MakeUpper();
		UpdateData(FALSE);
		//m_sModelSuffix.MakeUpper();
		//m_BarCode.put_Caption(m_BarcodeString);
		return TRUE;
	}
	

	return CDialog::PreTranslateMessage(pMsg);
}


LRESULT GetToolOptionDlg::GetToolOption(WPARAM wParam, LPARAM lParam)
{
	BOOL bRetryResult = 0;
	BOOL bRssult = FALSE;
	CString strRtn;
	CString sTemp;
	CString sMsg;
	DWORD dwEventResult = 0;
    clock_t		start;
	MSG msg;

	CurrentSet->sToolOption1 = "";
	CurrentSet->sToolOption2 = "";
	CurrentSet->sToolOption3 = "";
	CurrentSet->sToolOption4 = "";
	CurrentSet->sToolOption5 = "";
	CurrentSet->sToolOption6 = "";
	CurrentSet->sToolOption7 = "";
	CurrentSet->sToolOption8 = "";
	CurrentSet->sCommercialOption1 = "";
	CurrentSet->sToolCRC	= "";
	
	CurrentSet->sAreaOption1 = "";
	CurrentSet->sCountryGroup = "";
	CurrentSet->sCountry = "";
	
	CurrentSet->sCPUVersion = "";
	CurrentSet->sMicomVersion = "";
	CurrentSet->sUSBVersion = "";
	CurrentSet->sMNTVersion = "";
	
	CurrentSet->sEDID_HDMI1		= "";  	
	CurrentSet->sEDID_HDMI2		= "";  	
	CurrentSet->sEDID_HDMI3		= "";  	
	CurrentSet->sEDID_HDMI4		= "";  	
	CurrentSet->sEDID_HDMI5		= "";  	
	CurrentSet->sEDID_DP		= "";  	        
	CurrentSet->sEDID_DP2		= "";  	        
	CurrentSet->sEDID_DVI		= "";  	        
	CurrentSet->sEDID_PC		= "";  	        
	CurrentSet->sEDID_RGB		= "";  	        
	
	CurrentSet->sCI				= ""; 		
	CurrentSet->sCNTV			= ""; 		
	CurrentSet->sDVBS			= ""; 		
	CurrentSet->sDVBS2			= ""; 		
	CurrentSet->sESN			= ""; 		
	CurrentSet->sDTCP_RX		= ""; 	        
	CurrentSet->sHDCP_RX		= ""; 	        
	CurrentSet->sHDCP_RX_14		= ""; 	
	CurrentSet->sHDCP_TX		= ""; 	        
	CurrentSet->sWASU			= ""; 		
	CurrentSet->sADC			= ""; 		
	CurrentSet->sWIDEVINE		= ""; 	        
	CurrentSet->sMARINKEY		= ""; 	        
	CurrentSet->sBUILT_WIFI		= ""; 	
	CurrentSet->sJAPAN_LNB		= ""; 	
	CurrentSet->sMAC_ADDRESS	= ""; 	
	CurrentSet->sCA				= ""; 		
	CurrentSet->sMODEL_NAME		= ""; 	
	CurrentSet->sSERIAL_NO = "";
	CurrentSet->sASTC30_Key = "";
	CurrentSet->sMfi_Key = "";
	CurrentSet->sCI_CEP_Key = "";

	CurrentSet->sModelID = "";
	CurrentSet->sModelSuffix = "";
	CurrentSet->sModelName_GMES = "";

	m_sModelSuffix = CurrentSet->sModelSuffixName;
	UpdateData(FALSE);

	g_pView->ShowSystemMsg(TOOL_OPTION_DOWNLOAD_MSG);

	strRtn = gGmesCtrl.MakeElem_S6F11_1(CurrentSet->sEquipmentID, m_sModelSuffix, TRUE);
	::ResetEvent(g_pView->m_hReadEvent_S6F12);
	::ResetEvent(g_pView->m_hReadEvent_S6F5);
#ifndef	_PLC_COM_SIM_DEBUG__MODE
	if(gGmesCtrl.SendCommString(strRtn))
	{
		gGmesCtrl.DisplyComStatus("S6F11", FALSE);
	}
	else{
		AfxMessageBox("SendCommString - Fail");
		return 0;
	}
#endif
	start = clock();
	while (TRUE)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   
		dwEventResult = WaitForSingleObject(g_pView->m_hReadEvent_S6F12, 500);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(g_pView->m_hReadEvent_S6F12);
			if(!g_pView->m_bResult_S6F12){
				sMsg.Format(_T("GMES Tool Option D/L Error\nError Code : %s"), gGmesCtrl.m_sErrorMsg);
				if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
				{
					AddStringToStatus(sMsg);
					bRetryResult = 1;
				}
				else
				{
					AfxMessageBox(sMsg);
				}
				//
			}
			bRssult = g_pView->m_bResult_S6F12;
			break;
		}
		if ((clock() - start)*1000/CLOCKS_PER_SEC > 35000){ 
			sMsg.Format(_T("GMES S6F12(Tool Option) D/L Time Out."));
			if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
			{
				AddStringToStatus(sMsg);
				bRetryResult = 1;
			}
			else
			{
				AfxMessageBox(sMsg);
			}
			Gmes_Connection();

			bRssult = FALSE;
			break;
		}
	}
	if(bRssult){
		start = clock();
		while (TRUE)
		{
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					CurrentSet->bIsRunning = FALSE;
					::PostQuitMessage(0);
					break;
				}
				if (!AfxGetApp()->PreTranslateMessage(&msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}   

			dwEventResult = WaitForSingleObject(g_pView->m_hReadEvent_S6F5, 500);
				
			if(dwEventResult == WAIT_OBJECT_0 )
			{
				::ResetEvent(g_pView->m_hReadEvent_S6F5);
				if(!g_pView->m_bResult_S6F5){
					sMsg.Format(_T("GMES Tool Option D/L Error\nParsing Error.."));
					if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
					{
						AddStringToStatus(sMsg);
						bRetryResult = 1;
					}
					else
					{
						AfxMessageBox(sMsg);
					}
				}
				break;
			}
			if ((clock() - start)*1000/CLOCKS_PER_SEC > 5000){ 
				sMsg.Format(_T("GMES S6F5(Tool Option) D/L Time Out."));
				if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
				{
					AddStringToStatus(sMsg);
					bRetryResult = 1;
				}
				else
				{
					AfxMessageBox(sMsg);
				}
				Gmes_Connection();
				break;
			}
		}
	}
	if (bRetryResult == 0)
	{

		sTemp.Format("%s.%s(%s)", CurrentSet->sModelID, CurrentSet->sModelSuffix, CurrentSet->sModelName_GMES);
		m_sModelSuffix = sTemp;

		sTemp.Format("%s.%s", CurrentSet->sModelID, CurrentSet->sModelSuffix);
		CurrentSet->sModelSuffixName = sTemp;

		m_ctrlOptionGrid.SetTextMatrix(0, 1, CurrentSet->sToolOption1);
		m_ctrlOptionGrid.SetTextMatrix(1, 1, CurrentSet->sToolOption2);
		m_ctrlOptionGrid.SetTextMatrix(2, 1, CurrentSet->sToolOption3);
		m_ctrlOptionGrid.SetTextMatrix(3, 1, CurrentSet->sToolOption4);
		m_ctrlOptionGrid.SetTextMatrix(4, 1, CurrentSet->sToolOption5);
		m_ctrlOptionGrid.SetTextMatrix(5, 1, CurrentSet->sToolOption6);
		m_ctrlOptionGrid.SetTextMatrix(6, 1, CurrentSet->sToolOption7);
		m_ctrlOptionGrid.SetTextMatrix(7, 1, CurrentSet->sCommercialOption1);
		m_ctrlOptionGrid.SetTextMatrix(8, 1, CurrentSet->sToolOption8);
		m_ctrlOptionGrid.SetTextMatrix(9, 1, CurrentSet->sToolCRC);

		m_ctrlOptionGrid.SetTextMatrix(10, 1, CurrentSet->sAreaOption1);
		m_ctrlOptionGrid.SetTextMatrix(11, 1, CurrentSet->sCountryGroup);
		m_ctrlOptionGrid.SetTextMatrix(12, 1, CurrentSet->sCountry);
		m_ctrlOptionGrid.SetTextMatrix(13, 1, CurrentSet->sCPUVersion);
		m_ctrlOptionGrid.SetTextMatrix(14, 1, CurrentSet->sMicomVersion);


		m_ctrlSpecGrid1.SetTextMatrix(0, 1, CurrentSet->sEDID_HDMI1);
		m_ctrlSpecGrid1.SetTextMatrix(1, 1, CurrentSet->sEDID_HDMI2);
		m_ctrlSpecGrid1.SetTextMatrix(2, 1, CurrentSet->sEDID_HDMI3);
		m_ctrlSpecGrid1.SetTextMatrix(3, 1, CurrentSet->sEDID_HDMI4);
		m_ctrlSpecGrid1.SetTextMatrix(4, 1, CurrentSet->sEDID_HDMI5);
		m_ctrlSpecGrid1.SetTextMatrix(5, 1, CurrentSet->sEDID_DP);
		m_ctrlSpecGrid1.SetTextMatrix(6, 1, CurrentSet->sEDID_DP2);
		m_ctrlSpecGrid1.SetTextMatrix(7, 1, CurrentSet->sEDID_DVI);
		m_ctrlSpecGrid1.SetTextMatrix(8, 1, CurrentSet->sEDID_PC);
		m_ctrlSpecGrid1.SetTextMatrix(9, 1, CurrentSet->sEDID_RGB);

		m_ctrlSpecGrid1.SetTextMatrix(10, 1, CurrentSet->sCI);
		m_ctrlSpecGrid1.SetTextMatrix(11, 1, CurrentSet->sCNTV);
		m_ctrlSpecGrid1.SetTextMatrix(12, 1, CurrentSet->sDVBS);
		m_ctrlSpecGrid1.SetTextMatrix(13, 1, CurrentSet->sDVBS2);
		m_ctrlSpecGrid1.SetTextMatrix(14, 1, CurrentSet->sESN);
		m_ctrlSpecGrid1.SetTextMatrix(15, 1, CurrentSet->sDTCP_RX);
		m_ctrlSpecGrid1.SetTextMatrix(16, 1, CurrentSet->sHDCP_RX);
		m_ctrlSpecGrid1.SetTextMatrix(17, 1, CurrentSet->sHDCP_RX_14);
		m_ctrlSpecGrid1.SetTextMatrix(18, 1, CurrentSet->sHDCP_TX);
		m_ctrlSpecGrid1.SetTextMatrix(19, 1, CurrentSet->sWASU);
		m_ctrlSpecGrid1.SetTextMatrix(20, 1, CurrentSet->sADC);
		m_ctrlSpecGrid1.SetTextMatrix(21, 1, CurrentSet->sWIDEVINE);
		m_ctrlSpecGrid1.SetTextMatrix(22, 1, CurrentSet->sMARINKEY);
		m_ctrlSpecGrid1.SetTextMatrix(23, 1, CurrentSet->sBUILT_WIFI);
		m_ctrlSpecGrid1.SetTextMatrix(24, 1, CurrentSet->sJAPAN_LNB);
		m_ctrlSpecGrid1.SetTextMatrix(25, 1, CurrentSet->sMAC_ADDRESS);
		m_ctrlSpecGrid1.SetTextMatrix(26, 1, CurrentSet->sASTC30_Key);
		m_ctrlSpecGrid1.SetTextMatrix(27, 1, CurrentSet->sMfi_Key);
		m_ctrlSpecGrid1.SetTextMatrix(28, 1, CurrentSet->sCI_CEP_Key);
		m_ctrlSpecGrid1.SetTextMatrix(29, 1, CurrentSet->sCA);
		m_ctrlSpecGrid1.SetTextMatrix(30, 1, CurrentSet->sMODEL_NAME);
		m_ctrlSpecGrid1.SetTextMatrix(31, 1, CurrentSet->sSERIAL_NO);


		UpdateData(FALSE);
	}

	g_pView->CloseSystemMsg();
	if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
	{
		if (bRetryResult == 1)
		{
			m_nRetryCount++;
			if (m_nRetryCount <= 2)
			{
				SetTimer(1, 500, NULL);
			}
			else
			{
				AfxMessageBox("GetToolOption RETRY Error!!");//AddStringToStatus("GetToolOption RETRY Error!!");//	
			}

		}
	}

	return 0;
}

LRESULT GetToolOptionDlg::CloseToolOption(WPARAM wParam, LPARAM lParam)
{
	OnOK();
	return 0;
}

void GetToolOptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_sTempModelSuffix = "";

	CurrentSet->sJACK_AV1	= m_CtrlListIndexOption.GetItemText(0, 1);//, CurrentSet->sJACK_AV1);
	CurrentSet->sJACK_AV2	= m_CtrlListIndexOption.GetItemText(1, 1);//, CurrentSet->sJACK_AV2);
	CurrentSet->sJACK_SCART	= m_CtrlListIndexOption.GetItemText(2, 1);//, CurrentSet->sJACK_SCART);
	CurrentSet->sJACK_COMP1	= m_CtrlListIndexOption.GetItemText(3, 1);//, CurrentSet->sJACK_COMP1);
	CurrentSet->sJACK_COMP2	= m_CtrlListIndexOption.GetItemText(4, 1);//, CurrentSet->sJACK_COMP2);
	CurrentSet->sJACK_USB1	= m_CtrlListIndexOption.GetItemText(5, 1);//, CurrentSet->sJACK_USB1);
	CurrentSet->sJACK_USB2	= m_CtrlListIndexOption.GetItemText(6, 1);//, CurrentSet->sJACK_USB2);
	CurrentSet->sJACK_USB3	= m_CtrlListIndexOption.GetItemText(7, 1);//, CurrentSet->sJACK_USB3);
	CurrentSet->sJACK_USB4	= m_CtrlListIndexOption.GetItemText(8, 1);//, CurrentSet->sJACK_USB4);
	CurrentSet->sJACK_HEAD_PHONE = m_CtrlListIndexOption.GetItemText(9, 1);// , CurrentSet->sJACK_HEAD_PHONE);
	CurrentSet->sJACK_HDMI1	= m_CtrlListIndexOption.GetItemText(10, 1);///, CurrentSet->sJACK_HDMI1);;
	CurrentSet->sJACK_HDMI2	= m_CtrlListIndexOption.GetItemText(11, 1);///, CurrentSet->sJACK_HDMI2);
	CurrentSet->sJACK_HDMI3	= m_CtrlListIndexOption.GetItemText(12, 1);///, CurrentSet->sJACK_HDMI3);
	CurrentSet->sJACK_HDMI4	= m_CtrlListIndexOption.GetItemText(13, 1);///, CurrentSet->sJACK_HDMI4);
	CurrentSet->sJACK_RGB	= m_CtrlListIndexOption.GetItemText(14, 1);///, CurrentSet->sJACK_RGB);
	CurrentSet->sJACK_DVI	= m_CtrlListIndexOption.GetItemText(15, 1);///, CurrentSet->sJACK_DVI);
	CurrentSet->sJACK_DP	= m_CtrlListIndexOption.GetItemText(16, 1);///, CurrentSet->sJACK_DP);
	CurrentSet->sJACK_SMHL	= m_CtrlListIndexOption.GetItemText(17, 1);///, CurrentSet->sJACK_SMHL);
																	
	CDialog::OnOK();
}

void GetToolOptionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void GetToolOptionDlg::OnButton1() 
{
//	CurrentSet->sModelSuffixName = "32LN630R-NA.AKRYLH";
	CurrentSet->sModelSuffixName = "32LB6500-JA.AJLWLH";
//	CurrentSet->sModelSuffixName = "79UB9800-NA.AKRWLH";
	SendMessage(UM_UPDATE_TOOL_OPTION,0,0);

}
void GetToolOptionDlg::InitSpecGrid()
{
	CString sTmp;
//	int nCol;
	int nRow;

	char *sHeader1[] = {"EDID_HDMI1", "EDID_HDMI2", "EDID_HDMI3", "EDID_HDMI4", "EDID_HDMI5", 
						"EDID_DP", "EDID_DP2", "EDID_DVI", "EDID_PC", "EDID_RGB",
						"CI", "CNTV", "DVBS", "DVBS2", "ESN",
						"DTCP_RX", "HDCP_RX", "HDCP_RX_14", "HDCP_TX", "WASU", 
						"ADC", "WIDEVINE", "MARINKEY",	"BUILT_WIFI", "JAPAN_LNB",
						"MAC_ADDRESS","ATSC3_0_KEY" , "MFI", "CI_CEP", "CA",
						"MODEL_NAME","SERIAL_NO"  };

	char *sHeader2[] = {"TOOL OPTION1", "TOOL OPTION2", "TOOL OPTION3", "TOOL OPTION4", "TOOL OPTION5", "TOOL OPTION6", "TOOL OPTION7", "TOOL OPTION8(C)", 
						"TOOL OPTION9", "TOOL CRC", "AREA CODE", "COUNTRY GROUP", "COUNTRY", "S/W VERSION", "MICOM VERSION"};

	//	char *sHeader2[] = {"CI", "CNTV", "DVBS", "ESN", "DTCP_RX", "HDCP_RX", "HDCP_RX_14", "HDCP_TX", "WASU", "ADC", "WIDEVINE", "MARINKEY",
//						"BUILT_WIFI", "JAPAN_LNB", "MAC_ADDRESS", "MODEL_NAME", "SERIAL_NO"};

	//	char *sHeader3[] = {"BUILT_WIFI", "JAPAN_LNB", "MAC_ADDRESS", "MODEL_NAME", "SERIAL_NO"};
//	char *sHeader2[] = {"CI", "CNTV", "DVBS", "ESN", "DTCP_RX", "HDCP_RX", "HDCP_RX_14", "HDCP_TX", "WASU", "ADC", "WIDEVINE", "MARINKEY"};

	m_ctrlSpecGrid1.SetRedraw(FALSE);
	m_ctrlSpecGrid1.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlSpecGrid1.SetBackColorFixed(RGB(210,216,176));
	m_ctrlSpecGrid1.SetForeColorFixed(RGB(51,102,153));
	m_ctrlSpecGrid1.SetBackColor(RGB(247,247,231));
	m_ctrlSpecGrid1.SetGridColor(RGB(200,200,176));

	m_ctrlSpecGrid1.SetFixedRows(0);
	m_ctrlSpecGrid1.SetFixedCols(1);

	m_ctrlSpecGrid1.SetFillStyle(flexFillRepeat);
	m_ctrlSpecGrid1.SetFontName("Arial");
	m_ctrlSpecGrid1.SetFontSize(8);

	m_ctrlSpecGrid1.SetCols(2);
	m_ctrlSpecGrid1.SetRows(32);
	m_ctrlSpecGrid1.SetExtendLastCol(TRUE);

	m_ctrlSpecGrid1.SetColWidth(0, 1300); //500
	m_ctrlSpecGrid1.SetColWidth(1, 1000); //500
	m_ctrlSpecGrid1.SetColAlignment(1,flexAlignCenterCenter);		
	m_ctrlSpecGrid1.SetFixedAlignment(0,flexAlignRightCenter);

	for(nRow = 0; nRow < 32 ; nRow++)
	{
		m_ctrlSpecGrid1.SetRow(nRow);
		m_ctrlSpecGrid1.SetCol(0);
	
		m_ctrlSpecGrid1.SetRowHeight(nRow, 250);
		m_ctrlSpecGrid1.SetText(sHeader1[nRow]);
	}
	m_ctrlSpecGrid1.SetRedraw(TRUE);
	m_ctrlSpecGrid1.Refresh();


	m_ctrlOptionGrid.SetRedraw(FALSE);
	m_ctrlOptionGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlOptionGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlOptionGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlOptionGrid.SetBackColor(RGB(247,247,231));
	m_ctrlOptionGrid.SetGridColor(RGB(200,200,176));

	m_ctrlOptionGrid.SetFixedRows(0);
	m_ctrlOptionGrid.SetFixedCols(1);

	m_ctrlOptionGrid.SetFillStyle(flexFillRepeat);
//	m_ctrlOptionGrid.SetFontName("Arial");
	m_ctrlOptionGrid.SetFontName("System");
	m_ctrlOptionGrid.SetFontSize(11);
	m_ctrlOptionGrid.SetFontBold(TRUE);

	m_ctrlOptionGrid.SetCols(2);
	m_ctrlOptionGrid.SetRows(15);
	m_ctrlOptionGrid.SetExtendLastCol(TRUE);

	m_ctrlOptionGrid.SetColWidth(0, 2500); //500
	m_ctrlOptionGrid.SetColWidth(1, 1800); //500

	for(nRow = 0; nRow < 15 ; nRow++)
	{
		m_ctrlOptionGrid.SetRow(nRow);
		m_ctrlOptionGrid.SetCol(0);
	
		m_ctrlOptionGrid.SetRowHeight(nRow, 500);
		m_ctrlOptionGrid.SetText(sHeader2[nRow]);

		m_ctrlOptionGrid.SetFixedAlignment(0,flexAlignRightCenter);
		m_ctrlOptionGrid.SetColAlignment(1,flexAlignCenterCenter);		
	}
	m_ctrlOptionGrid.SetRedraw(TRUE);
	m_ctrlOptionGrid.Refresh();

/*
	m_ctrlSpecGrid2.SetRedraw(FALSE);
	m_ctrlSpecGrid2.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlSpecGrid2.SetBackColorFixed(RGB(210,216,176));
	m_ctrlSpecGrid2.SetForeColorFixed(RGB(51,102,153));
	m_ctrlSpecGrid2.SetBackColor(RGB(247,247,231));
	m_ctrlSpecGrid2.SetGridColor(RGB(200,200,176));

	m_ctrlSpecGrid2.SetFixedRows(0);
	m_ctrlSpecGrid2.SetFixedCols(1);

	m_ctrlSpecGrid2.SetFillStyle(flexFillRepeat);
	m_ctrlSpecGrid2.SetFontName("Arial");
	m_ctrlSpecGrid2.SetFontSize(8);

	m_ctrlSpecGrid2.SetCols(2);
	m_ctrlSpecGrid2.SetRows(17);
	m_ctrlSpecGrid2.SetExtendLastCol(TRUE);

	m_ctrlSpecGrid2.SetColAlignment(1,flexAlignCenterCenter);		
	m_ctrlSpecGrid2.SetFixedAlignment(0,flexAlignCenterCenter);

	m_ctrlSpecGrid2.SetColWidth(0, 1300); //500
	m_ctrlSpecGrid2.SetColWidth(1, 800); //500

	for(nRow = 0; nRow < 17 ; nRow++)
	{
		m_ctrlSpecGrid2.SetRow(nRow);
		m_ctrlSpecGrid2.SetCol(0);
	
		m_ctrlSpecGrid2.SetText(sHeader2[nRow]);


	}
	m_ctrlSpecGrid2.SetRedraw(TRUE);
	m_ctrlSpecGrid2.Refresh();
*/
/*	m_ctrlSpecGrid3.SetRedraw(FALSE);
	m_ctrlSpecGrid3.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlSpecGrid3.SetBackColorFixed(RGB(210,216,176));
	m_ctrlSpecGrid3.SetForeColorFixed(RGB(51,102,153));
	m_ctrlSpecGrid3.SetBackColor(RGB(247,247,231));
	m_ctrlSpecGrid3.SetGridColor(RGB(200,200,176));

	m_ctrlSpecGrid3.SetFixedRows(1);
	m_ctrlSpecGrid3.SetFixedCols(0);

	m_ctrlSpecGrid3.SetFillStyle(flexFillRepeat);
	m_ctrlSpecGrid3.SetFontName("Arial");
	m_ctrlSpecGrid3.SetFontSize(8);

	m_ctrlSpecGrid3.SetCols(5);
	m_ctrlSpecGrid3.SetRows(2);
	m_ctrlSpecGrid3.SetExtendLastCol(TRUE);

	for(nCol = 0; nCol < 5 ; nCol++)
	{
		m_ctrlSpecGrid3.SetRow(0);
		m_ctrlSpecGrid3.SetCol(nCol);
	
		m_ctrlSpecGrid3.SetText(sHeader3[nCol]);
		m_ctrlSpecGrid3.SetColAlignment(nCol,flexAlignCenterCenter);		
		//m_ctrlSpecGrid3.SetColWidth(nCol, 1000); //500
//	char *sHeader3[] = {"BUILT_WIFI", "JAPAN_LNB", "MAC_ADDRESS", "MODEL_NAME", "SERIAL_NO"};
		
		m_ctrlSpecGrid3.SetColWidth(nCol, 1300); 

		m_ctrlSpecGrid3.SetFixedAlignment(nCol,flexAlignCenterCenter);

	}
	m_ctrlSpecGrid3.SetRedraw(TRUE);
	m_ctrlSpecGrid3.Refresh();
*/

}


void GetToolOptionDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
		case 1:
		{
			KillTimer(nIDEvent);
			SendMessage(UM_UPDATE_TOOL_OPTION,0,0);

			break;
		}
	
		default:
			break;
	}
	CDialog::OnTimer(nIDEvent);
}


void GetToolOptionDlg::OnBnClickedButtonOptGet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sTmp;
	int nVal;
	if ((g_nSysType == AUTO_SYS) && (g_nUseNoScanType == FALSE))
	{
		nVal = g_pView->GetBarcodeData(sTmp);
			if (nVal == 0) {
				Sleep(500);
					nVal = g_pView->GetBarcodeData(sTmp);
			}
		if (nVal == 0) {
			AfxMessageBox("Barcode Scanning Error!");
				AddStringToStatus("Barcode Scanning Error!");

				//return;
		}
		else
		{
			m_sModelSuffix = sTmp;
			UpdateData(FALSE);
		}
	}
}


void GetToolOptionDlg::OnBnClickedButtonUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_sModelSuffix.Find(".") > 0)
	{

		CurrentSet->sModelSuffixName = m_sModelSuffix;
		SendMessage(UM_UPDATE_TOOL_OPTION, 0, 0);

	}
	else
	{
		MessageBox("Scan or Edit Suffix Name First!!");
	}

}

void GetToolOptionDlg::InitIndexGrid()
{
	char *sHeader1[] = { " AV1", " AV2", " SCART", " COMP1", " COMP2", " USB1", " USB2", " USB3", " USB4"
		, " HEAD_PHONE", " HDMI1", " HDMI2", " HDMI3", " HDMI4", " RGB", " DVI", " DP", " SMHL" };
	//18
	CString sTmp;
	m_CtrlListIndexOption.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListIndexOption.InsertColumn(0, _T("INDEX"), LVCFMT_CENTER, 125);
	m_CtrlListIndexOption.InsertColumn(1, _T("OPTION"), LVCFMT_CENTER, 100);// 리스트 행 추가
	for (int i = 0; i < 18; i++)
	{
		sTmp = sHeader1[i];
		m_CtrlListIndexOption.InsertItem(i, sTmp);
	}
	
	//m_CtrlListIndexOption.SetItemText(0,1,)
	//m_ctrlStepMainList.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetCurrentSource(), NULL, NULL, NULL, NULL);
	
	m_CtrlListIndexOption.SetItemText(0, 1, CurrentSet->sJACK_AV1);
	m_CtrlListIndexOption.SetItemText(1, 1, CurrentSet->sJACK_AV2);
	m_CtrlListIndexOption.SetItemText(2, 1, CurrentSet->sJACK_SCART);
	m_CtrlListIndexOption.SetItemText(3, 1, CurrentSet->sJACK_COMP1);
	m_CtrlListIndexOption.SetItemText(4, 1, CurrentSet->sJACK_COMP2);
	m_CtrlListIndexOption.SetItemText(5, 1, CurrentSet->sJACK_USB1);
	m_CtrlListIndexOption.SetItemText(6, 1, CurrentSet->sJACK_USB2);
	m_CtrlListIndexOption.SetItemText(7, 1, CurrentSet->sJACK_USB3);
	m_CtrlListIndexOption.SetItemText(8, 1, CurrentSet->sJACK_USB4);
	m_CtrlListIndexOption.SetItemText(9, 1, CurrentSet->sJACK_HEAD_PHONE);
	m_CtrlListIndexOption.SetItemText(10, 1, CurrentSet->sJACK_HDMI1);;
	m_CtrlListIndexOption.SetItemText(11, 1, CurrentSet->sJACK_HDMI2);
	m_CtrlListIndexOption.SetItemText(12, 1, CurrentSet->sJACK_HDMI3);
	m_CtrlListIndexOption.SetItemText(13, 1, CurrentSet->sJACK_HDMI4);
	m_CtrlListIndexOption.SetItemText(14, 1, CurrentSet->sJACK_RGB);
	m_CtrlListIndexOption.SetItemText(15, 1, CurrentSet->sJACK_DVI);
	m_CtrlListIndexOption.SetItemText(16, 1, CurrentSet->sJACK_DP);
	m_CtrlListIndexOption.SetItemText(17, 1, CurrentSet->sJACK_SMHL);
	
}



void GetToolOptionDlg::OnNMClickListIndexOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	

	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	CString strTmp;
	
	if (nShell == 1)
	{
		strTmp = m_CtrlListIndexOption.GetItemText(nItem, nShell);
		if (strTmp.Find("YES") >= 0)
		{
			m_CtrlListIndexOption.SetItemText(nItem, 1, "NO");
		}
		else
		{
			m_CtrlListIndexOption.SetItemText(nItem, 1, "YES");
		}
	}
	


	*pResult = 0;
}


void GetToolOptionDlg::OnNMCustomdrawListIndexOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
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

		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row


	


		if (1 == (int)(pLVCD->iSubItem))
		{
		//m_ctrlSpecGrid1.SetBackColorFixed(RGB(210, 216, 176));
		//m_ctrlSpecGrid1.SetForeColorFixed(RGB(51, 102, 153));
		//m_ctrlSpecGrid1.SetBackColor(RGB(247, 247, 231));
		//m_ctrlSpecGrid1.SetGridColor(RGB(200, 200, 176));
			
			crText = RGB(0, 0, 0);
			crBkgnd = RGB(220, 220, 150);
			

		}
		else
		{
		//m_ctrlOptionGrid.SetBackColorFixed(RGB(210, 216, 176));
		//m_ctrlOptionGrid.SetForeColorFixed(RGB(51, 102, 153));
		//m_ctrlOptionGrid.SetBackColor(RGB(247, 247, 231));
		//m_ctrlOptionGrid.SetGridColor(RGB(200, 200, 176));

				crText = RGB(0, 0, 0);
				crBkgnd = RGB(210, 216, 176);
			
		}

		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;


		*pResult = CDRF_DODEFAULT;
		return;
	}
	}

	*pResult = 0;
}


void GetToolOptionDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}
