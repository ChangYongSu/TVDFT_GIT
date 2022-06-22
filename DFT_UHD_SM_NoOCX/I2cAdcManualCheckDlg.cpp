// I2cAdcManualCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "I2cAdcManualCheckDlg.h"
#include "Global.h"
#include "I2cAdcCtrl.h"
#include "PatternGeneratorCtl.h"
#include "AvSwitchBoxCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_READ_DATA_COL_ROW	17


extern CI2cAdcCtrl			I2cAdcCtrl;

/////////////////////////////////////////////////////////////////////////////
// CI2cAdcManualCheckDlg dialog


CI2cAdcManualCheckDlg::CI2cAdcManualCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CI2cAdcManualCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CI2cAdcManualCheckDlg)
	m_sz2abData = "00";
	m_sz2abCmd = "f4";
	m_sz2abReadAdh = "00";
	m_sz2abReadAdl = "00";
	m_nI2cPacketDelay = 0;
	m_nI2cReadByteDelay = 0;
	m_nI2cWriteByteDelay = 0;
	m_sz2bWriteAdr = "00";
	m_sz2bReadAdr = "00";
	m_sz2abCurrentAreaOption = _T("");
	m_sz2abCurrentToolOption = _T("");
	m_sz2abReadAreaOption = _T("");
	m_sz2abReadToolOption = _T("");
	m_sz2abWriteAreaOption = _T("");
	m_sz2abWriteToolOption = _T("");
	m_sz2abReadVersion = _T("");
	m_szEdidDownloadResult = _T("");
	m_sz2abReadVersion2 = _T("");
	//}}AFX_DATA_INIT
	m_bActive = FALSE;
}


void CI2cAdcManualCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CI2cAdcManualCheckDlg)
	DDX_Control(pDX, IDC_EDIT_MODEL, m_ctrlModelEdit);
	DDX_Control(pDX, IDC_EDIT_PATTERN, m_ctrlPatternEdit);
	DDX_Control(pDX, IDC_CMB_AREA_OPTION_NO, m_ctrlI2cAreaOptionNo);
	DDX_Control(pDX, IDC_CMB_TOOL_OPTION_NO, m_ctrlI2cToolOptionNo);
	DDX_Control(pDX, IDC_2B_WRITE_DATA, m_ctrl2bWriteDataList);
	DDX_Control(pDX, IDC_READ_PROGRESS, m_ctrlReadDataProgress);
	DDX_Control(pDX, IDC_2AB_READ_BYTE_COMBO, m_ctrl2abReadByte);
	DDX_Control(pDX, IDC_2B_READ_BYTE_COMBO, m_ctrl2bReadByte);
	DDX_Control(pDX, IDC_CMB_I2C_CHANNEL_NO, m_ctrlI2cChannelNo);
	DDX_Control(pDX, IDC_CMB_I2C_CLK_SPEED, m_ctrlI2cClkSpeed);
	DDX_Control(pDX, IDC_READ_DATA_GRID, m_ctrlReadDataGrid);
	DDX_Control(pDX, IDC_I2C_ADC_COM_LOG_GRID, m_ctrlI2cAdcComLogGrid);
	DDX_Text(pDX, IDC_EDIT_2AB_DATA, m_sz2abData);
	DDX_Text(pDX, IDC_EDIT_2AB_CMD, m_sz2abCmd);
	DDX_Text(pDX, IDC_EDIT_2AB_READ_ADH, m_sz2abReadAdh);
	DDX_Text(pDX, IDC_EDIT_2AB_READ_ADL, m_sz2abReadAdl);
	DDX_Text(pDX, IDC_EDIT_I2C_PACKET_DELAY, m_nI2cPacketDelay);
	DDX_Text(pDX, IDC_EDIT_I2C_READ_BYTE_DELAY, m_nI2cReadByteDelay);
	DDX_Text(pDX, IDC_EDIT_I2C_WRITE_BYTE_DELAY, m_nI2cWriteByteDelay);
	DDX_Text(pDX, IDC_EDIT_2B_WRITE_ADR, m_sz2bWriteAdr);
	DDX_Text(pDX, IDC_EDIT_2B_READ_ADR, m_sz2bReadAdr);
	DDX_Text(pDX, IDC_EDIT_CURRENT_AREA_OPTION, m_sz2abCurrentAreaOption);
	DDX_Text(pDX, IDC_EDIT_CURRENT_TOOL_OPTION, m_sz2abCurrentToolOption);
	DDX_Text(pDX, IDC_EDIT_READ_AREA_OPTION, m_sz2abReadAreaOption);
	DDX_Text(pDX, IDC_EDIT_READ_TOOL_OPTION, m_sz2abReadToolOption);
	DDX_Text(pDX, IDC_EDIT_WRITE_AREA_OPTION, m_sz2abWriteAreaOption);
	DDX_Text(pDX, IDC_EDIT_WRITE_TOOL_OPTION, m_sz2abWriteToolOption);
	DDX_Text(pDX, IDC_EDIT_READ_VERSION, m_sz2abReadVersion);
	DDX_Text(pDX, IDC_EDIT_EDID_DOWNLOAD, m_szEdidDownloadResult);
	DDX_Text(pDX, IDC_EDIT_READ_VERSION2, m_sz2abReadVersion2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CI2cAdcManualCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CI2cAdcManualCheckDlg)
	ON_BN_CLICKED(IDC_BTN_SET_I2C_CLK_DELAY, OnBtnSetI2cClkDelay)
	ON_BN_CLICKED(IDC_BTN_SET_I2C_CHANNEL_NO, OnBtnSetI2cChannelNo)
	ON_BN_CLICKED(IDC_BTN_SEND_2AB_CMD, OnBtnSend2abCmd)
	ON_BN_CLICKED(IDC_BTN_2AB_WAKEUP, OnBtn2abWakeup)
	ON_BN_CLICKED(IDC_BTN_READ_2AB_DATA_BYTE, OnBtnRead2abDataByte)
	ON_BN_CLICKED(IDC_BTN_READ_2AB_DATA_PAGE, OnBtnRead2abDataPage)
	ON_BN_CLICKED(IDC_BTN_READ_2B_DATA_BYTE, OnBtnRead2bDataByte)
	ON_BN_CLICKED(IDC_BTN_READ_2B_DATA_PAGE, OnBtnRead2bDataPage)
	ON_BN_CLICKED(IDC_BTN_WRITE_2B_DATA, OnBtnWrite2bData)
	ON_BN_CLICKED(IDC_BTN_2AB_SET_TYPE, OnBtn2abSetType)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CLEAR_COM_LOG_GRID, OnBtnClearComLogGrid)
	ON_BN_CLICKED(IDC_BTN_CLEAR_READ_DATA, OnBtnClearReadData)
	ON_BN_CLICKED(IDC_BTN_2B_WAKEUP, OnBtn2bWakeup)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_2AB_TOOL_OPTION_READ, OnBtn2abToolOptionRead)
	ON_BN_CLICKED(IDC_BTN_2AB_READ_VERSION, OnBtn2abReadVersion)
	ON_BN_CLICKED(IDC_BTN_2AB_AREA_OPTION_READ, OnBtn2abAreaOptionRead)
	ON_CBN_SELCHANGE(IDC_CMB_TOOL_OPTION_NO, OnSelchangeCmbToolOptionNo)
	ON_CBN_SELCHANGE(IDC_CMB_AREA_OPTION_NO, OnSelchangeCmbAreaOptionNo)
	ON_BN_CLICKED(IDC_BTN_2AB_TOOL_OPTION_WRITE, OnBtn2abToolOptionWrite)
	ON_BN_CLICKED(IDC_BTN_2AB_AREA_OPTION_WRITE, OnBtn2abAreaOptionWrite)
	ON_BN_CLICKED(IDC_BTN_SEND_MODEL, OnBtnSendModel)
	ON_BN_CLICKED(IDC_BTN_SEND_PATTERN, OnBtnSendPattern)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_29, OnBtnRemoteDlg29)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_31, OnBtnRemoteDlg31)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_30, OnBtnRemoteDlg30)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_33, OnBtnRemoteDlg33)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_81, OnBtnRemoteDlg81)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_71, OnBtnRemoteDlg71)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_32, OnBtnRemoteDlg32)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_14, OnBtnRemoteDlg14)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_34, OnBtnRemoteDlg34)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DLG_73, OnBtnRemoteDlg73)
	ON_BN_CLICKED(IDC_BTN_2AB_EDID_DOWNLOAD, OnBtn2abEdidDownload)
	ON_BN_CLICKED(IDC_BTN_2AB_READ_VERSION2, OnBtn2abReadVersion2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CI2cAdcManualCheckDlg message handlers

BOOL CI2cAdcManualCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ctrlI2cClkSpeed.SetCurSel(CurrentSet->nI2cClkSpeed);
	m_ctrlI2cChannelNo.SetCurSel(0);

	m_nI2cPacketDelay	 = CurrentSet->nI2cPacketDelay;
	m_nI2cReadByteDelay  = CurrentSet->nI2cReadByteDelay;
	m_nI2cWriteByteDelay = CurrentSet->nI2cWriteByteDelay;
	
	InitReadDataGrid();
	
	InitComLogGrid();

	m_ctrlReadDataProgress.SetRange(0,100);
	m_ctrlReadDataProgress.SetPos(0);

	//+add kwmoon 080910
	m_ctrl2abReadByte.SetCurSel(0);
	m_ctrl2bReadByte.SetCurSel(0);

	//+add kwmoon 080925
	m_ctrlI2cToolOptionNo.SetCurSel(0);
	m_ctrlI2cAreaOptionNo.SetCurSel(0);
	m_sz2abCurrentToolOption = CurrentSet->sToolOption1;
	m_sz2abCurrentAreaOption = CurrentSet->sAreaOption1;

	UpdateData(FALSE);

	I2cAdcCtrl.m_nI2cAdcSeqNo = 0;

	m_bActive = TRUE;

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CI2cAdcManualCheckDlg::InitReadDataGrid()
{
	CString sTmp;
	UINT nTmp;
	char *sHeader1[] = {"00","01","02", "03","04", "05", "06", "07", "08", "09", "0A", "0B","0C", "0D", "0E", "0F"};
	char *sHeader2[] = {"00","10","20", "30","40", "50", "60", "70", "80", "90", "A0", "B0","C0", "D0", "E0", "F0"};

	m_ctrlReadDataGrid.SetRedraw(FALSE);
	m_ctrlReadDataGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); 

	m_ctrlReadDataGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlReadDataGrid.SetForeColorFixed(RGB(51,102,153));

	m_ctrlReadDataGrid.SetBackColorBkg(RGB(255,255,255));
	m_ctrlReadDataGrid.SetBackColor(RGB(255,255,255));

	m_ctrlReadDataGrid.SetGridColor(RGB(210,216,176));
	m_ctrlReadDataGrid.SetAllowSelection(TRUE);
	m_ctrlReadDataGrid.SetFixedRows(1);
	m_ctrlReadDataGrid.SetFixedCols(1);
	
	m_ctrlReadDataGrid.SetCols(MAX_READ_DATA_COL_ROW);
	m_ctrlReadDataGrid.SetRows(MAX_READ_DATA_COL_ROW);

	m_ctrlReadDataGrid.SetRow(0);

	//========
	// Header
	//========
	m_ctrlReadDataGrid.SetCol(0);
	m_ctrlReadDataGrid.SetColWidth(0, 600);

	for(nTmp = 1; nTmp < MAX_READ_DATA_COL_ROW ; nTmp++)
	{
		m_ctrlReadDataGrid.SetCol(nTmp);
		m_ctrlReadDataGrid.SetFixedAlignment(nTmp, flexAlignCenterCenter);
		m_ctrlReadDataGrid.SetFillStyle(flexFillRepeat);
		m_ctrlReadDataGrid.SetFontName("Arial");
		m_ctrlReadDataGrid.SetFontSize(10);
		m_ctrlReadDataGrid.SetExtendLastCol(FALSE);
		m_ctrlReadDataGrid.SetText(sHeader1[nTmp-1]);
		m_ctrlReadDataGrid.SetEditable(flexEDNone);
		m_ctrlReadDataGrid.SetColWidth(nTmp, 540);		
	}

	m_ctrlReadDataGrid.SetCol(0);

	for(nTmp = 1; nTmp < MAX_READ_DATA_COL_ROW ; nTmp++)
	{
		m_ctrlReadDataGrid.SetRow(nTmp);
		m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
		m_ctrlReadDataGrid.SetFillStyle(flexFillRepeat);
		m_ctrlReadDataGrid.SetFontName("Arial");
		m_ctrlReadDataGrid.SetFontSize(10);
		m_ctrlReadDataGrid.SetText(sHeader2[nTmp-1]);
		m_ctrlReadDataGrid.SetEditable(flexEDNone);
	}

	m_ctrlReadDataGrid.SetRow(0);
	m_ctrlReadDataGrid.SetCol(0);
	m_ctrlReadDataGrid.SetRedraw(TRUE);
	m_ctrlReadDataGrid.Refresh();
}

void CI2cAdcManualCheckDlg::InitComLogGrid()
{
	//////////////////////////
	// Remote Com Log
	//////////////////////////

	m_ctrlI2cAdcComLogGrid.SetRedraw(FALSE);
	m_ctrlI2cAdcComLogGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); 

	m_ctrlI2cAdcComLogGrid.SetAllowBigSelection(FALSE);
	m_ctrlI2cAdcComLogGrid.SetExtendLastCol(TRUE);

	m_ctrlI2cAdcComLogGrid.SetBackColorBkg(COLOR_WHITE);
	m_ctrlI2cAdcComLogGrid.SetBackColor(RGB(247,247,231));
	
	m_ctrlI2cAdcComLogGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlI2cAdcComLogGrid.SetGridColor(RGB(210,216,176));

	m_ctrlI2cAdcComLogGrid.SetFixedRows(0);
	m_ctrlI2cAdcComLogGrid.SetFixedCols(0);

	m_ctrlI2cAdcComLogGrid.SetGridLines(flexGridFlat);

	m_ctrlI2cAdcComLogGrid.SetColWidth(0, 6000);
	m_ctrlI2cAdcComLogGrid.SetColAlignment(0,flexAlignLeftCenter);

	m_ctrlI2cAdcComLogGrid.SetCols(1);
	m_ctrlI2cAdcComLogGrid.SetRows(0);
	
	m_ctrlI2cAdcComLogGrid.SetRedraw(TRUE);
	m_ctrlI2cAdcComLogGrid.Refresh();
}

void CI2cAdcManualCheckDlg::OnBtnSetI2cClkDelay() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CurrentSet->nI2cClkSpeed		= m_ctrlI2cClkSpeed.GetCurSel();
	CurrentSet->nI2cPacketDelay		= m_nI2cPacketDelay;
	CurrentSet->nI2cReadByteDelay	= m_nI2cReadByteDelay;
	CurrentSet->nI2cWriteByteDelay	= m_nI2cWriteByteDelay;

	//+add kwmoon 080804
	int nResult = TEST_PASS;
	CString szErrMsg = _T("");

	if(I2cAdcCtrl.m_bPortOpen == FALSE)
	{
		if(!I2cAdcCtrl.Create(CurrentSet->sI2cAdcComPort, CurrentSet->wI2cAdcBaudRate))
		{
			szErrMsg.LoadString(ERROR_I2C_COMPORT);
			AfxMessageBox(szErrMsg); return;
		}
	}

	if((nResult = I2cAdcCtrl.SetI2cClkSpeed(CurrentSet->nI2cClkSpeed)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}

	if((nResult = I2cAdcCtrl.SetI2cPacketDelay(CurrentSet->nI2cPacketDelay)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}

	if((nResult = I2cAdcCtrl.SetI2cReadByteDelay(CurrentSet->nI2cReadByteDelay)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}

	if((nResult = I2cAdcCtrl.SetI2cWritByteDelay(CurrentSet->nI2cWriteByteDelay)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}

//	AfxMessageBox("I2C Clock and Delay are set successfully"); 
}

void CI2cAdcManualCheckDlg::OnBtnSetI2cChannelNo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nChannelNo = m_ctrlI2cChannelNo.GetCurSel();

	int nResult = TEST_PASS;
	CString szErrMsg  = _T("");
	CString	szChannel = _T("");
	
	szChannel.Format("%02x",nChannelNo);

	if(I2cAdcCtrl.m_bPortOpen == FALSE)
	{
		if(!I2cAdcCtrl.Create(CurrentSet->sI2cAdcComPort, CurrentSet->wI2cAdcBaudRate))
		{
			szErrMsg.LoadString(ERROR_I2C_COMPORT);
			AfxMessageBox(szErrMsg); return;
		}
	}

	if((nResult = I2cAdcCtrl.SetChannel(szChannel)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}

//	AfxMessageBox("I2C Channel is set successfully"); 
}

void CI2cAdcManualCheckDlg::OnBtnSend2abCmd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	CString szErrMsg = _T("");

	if((nResult = I2cAdcCtrl.SendCmd(m_sz2abCmd,m_sz2abData)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}	

//	AfxMessageBox("I2C Command is sent successfully"); 
}

void CI2cAdcManualCheckDlg::OnBtn2abWakeup() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	CString szErrMsg = _T("");

	if((nResult = I2cAdcCtrl.WakeUp()) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}
//	AfxMessageBox("WakeUp was successful.");
}

void CI2cAdcManualCheckDlg::OnBtnRead2abDataByte() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int nSelect	  = m_ctrl2abReadByte.GetCurSel();
	int nReadByte = 0;

	switch(nSelect)
	{
		case 0 : nReadByte = 1; break;
		case 1 : nReadByte = 2; break;
		case 2 : nReadByte = 4;   break;
		case 3 : nReadByte = 8;   break;
		case 4 : nReadByte = 16;  break;
		case 5 : nReadByte = 32;  break;
		case 6 : nReadByte = 64;  break;
		case 7 : nReadByte = 128; break;
//		case 8 : nReadByte = 256; break;
		
		default : AfxMessageBox("Choose 4,8,16,32,64,128 or 256"); return;
	}
	
	int nResult = TEST_PASS;
	int	nIndex  = 0;

	CString szData   = _T("");
	CString szErrMsg = _T("");

	I2cAdcCtrl.m_nData1 = hexCstr2decNum(m_sz2abReadAdl); // Page Address
	I2cAdcCtrl.m_nData2 = hexCstr2decNum(m_sz2abReadAdh); // Data Address
	I2cAdcCtrl.m_nData3 = nReadByte; 

	if(I2cAdcCtrl.m_nData2 + nReadByte > 256)
	{
		nReadByte = 256 - I2cAdcCtrl.m_nData2;
		nReadByte = nReadByte - nReadByte%4;
	}

	if(CurrentSet->nAvrFW_Ver == 2){
		nResult = I2cAdcCtrl.ReadI2cNbyteData2(nReadByte);
	}
	else{
		nResult = I2cAdcCtrl.ReadI2cNbyteData(nReadByte);
	}
	if(nResult == TEST_PASS)
	{
		nIndex = 0;
		
		for(int i=0; i<nReadByte; i++)
		{
			szData.Format("%02x",I2cAdcCtrl.m_aReadData[i]);
			szData.MakeUpper();

			nIndex = I2cAdcCtrl.m_nData2 + i;
			
			m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
			m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
			m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlReadDataGrid.SetText(szData);
		}

//		AfxMessageBox("Read Data was successful"); 
	}
	else
	{
		AfxMessageBox("Read Data failed"); 
	}
}

void CI2cAdcManualCheckDlg::OnBtnRead2abDataPage() 
{
	// TODO: Add your control notification handler code here
	//+add kwmoon 080910
	InitReadDataGrid();
	
	UpdateData(TRUE);
	
	int nReadByte = 256;

	int nResult = TEST_PASS;
	int	nIndex  = 0;

	CString szData   = _T("");
	CString szErrMsg = _T("");

	I2cAdcCtrl.m_nData1 = hexCstr2decNum(m_sz2abReadAdl); // Page Address
	I2cAdcCtrl.m_nData2 = hexCstr2decNum("00");			 // Data Address
	I2cAdcCtrl.m_nData3 = nReadByte; 

	if(I2cAdcCtrl.ReadI2cNbyteData(nReadByte) == TEST_PASS)
	{
		nIndex = 0;
		
		for(int i=0; i<nReadByte; i++)
		{
			szData.Format("%02x",I2cAdcCtrl.m_aReadData[i]);
			szData.MakeUpper();

			nIndex = I2cAdcCtrl.m_nData2 + i;
			
			m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
			m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
			m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlReadDataGrid.SetText(szData);
		}

//		AfxMessageBox("Read Page was successful"); 
	}
	else
	{
		AfxMessageBox("Read Page failed"); 
	}
}


void CI2cAdcManualCheckDlg::OnBtnRead2bDataByte() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int nSelect	  = m_ctrl2bReadByte.GetCurSel();
	int nReadByte = 0;

	switch(nSelect)
	{
		case 0 : nReadByte = 4;   break;
		case 1 : nReadByte = 8;   break;
		case 2 : nReadByte = 16;  break;
		case 3 : nReadByte = 32;  break;
		case 4 : nReadByte = 64;  break;
		case 5 : nReadByte = 128; break;
		case 6 : nReadByte = 256; break;
		
		default : AfxMessageBox("Choose 4,8,16,32,64,128 or 256"); return;
	}
	
	int nResult = TEST_PASS;
	int nIndex  = 0;

	CString szErrMsg = _T("");
	CString szData	 = _T("");

	//+add kwmoon 080408
	I2cAdcCtrl.m_nData1 = hexCstr2decNum(m_sz2bReadAdr);
	I2cAdcCtrl.m_nData2 = 0;

	if(CurrentSet->nAvrFW_Ver == 2){
		nResult = I2cAdcCtrl.ReadI2c2bNbyteData2(I2cAdcCtrl.m_nData1, nReadByte);
	}
	else{
		nResult = I2cAdcCtrl.ReadI2c2bNbyteData(nReadByte,m_ctrlReadDataProgress);
	}

	if(nResult == TEST_PASS)
	{
		nIndex = 0;
		
		for(int i=0; i<nReadByte; i++)
		{
			szData.Format("%02x",I2cAdcCtrl.m_aReadData[i]);
			szData.MakeUpper();

			nIndex = I2cAdcCtrl.m_nData1 + i;
			
			m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
			m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
			m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlReadDataGrid.SetText(szData);
		}

//		AfxMessageBox("Read Data was successful"); 
	}
	else
	{
		AfxMessageBox("Read Data failed"); 
	}
}

void CI2cAdcManualCheckDlg::OnBtnRead2bDataPage() 
{
	// TODO: Add your control notification handler code here
	//+add kwmoon 080910
	InitReadDataGrid();
	
	UpdateData(TRUE);
	
	int nSelect	  = m_ctrl2bReadByte.GetCurSel();
	int nReadByte = 256;
	
	int nResult = TEST_PASS;
	int nIndex  = 0;

	CString szErrMsg = _T("");
	CString szData	 = _T("");

	//+add kwmoon 080408
	I2cAdcCtrl.m_nData1 = 0;
	I2cAdcCtrl.m_nData2 = 0;

	if(CurrentSet->nAvrFW_Ver == 2){
		if(I2cAdcCtrl.ReadI2c2bNbyteData2(0x00, 128) == TEST_PASS)
		{
			for(int i=0; i<128; i++)
			{
				szData.Format("%02x",I2cAdcCtrl.m_aReadData[i]);
				szData.MakeUpper();

				nIndex = i;
				
				m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
				m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
				m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
				m_ctrlReadDataGrid.SetText(szData);
			}
		}
		else
		{
			AfxMessageBox("Read Page failed"); 
			return; 
		}
		if(I2cAdcCtrl.ReadI2c2bNbyteData2(0x80, 128) == TEST_PASS)
		{
			for(int i=0; i<128; i++)
			{
				szData.Format("%02x",I2cAdcCtrl.m_aReadData[i]);
				szData.MakeUpper();

				nIndex = 128 + i;
				
				m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
				m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
				m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
				m_ctrlReadDataGrid.SetText(szData);
			}
		}
		else
		{
			AfxMessageBox("Read Page failed"); 
			return; 
		}
	}
	else{
		if(I2cAdcCtrl.ReadI2c2bNbyteData(nReadByte,m_ctrlReadDataProgress) == TEST_PASS)
		{
			nIndex = 0;
			
			for(int i=0; i<nReadByte; i++)
			{
				szData.Format("%02x",I2cAdcCtrl.m_aReadData[i]);
				szData.MakeUpper();

				nIndex = I2cAdcCtrl.m_nData2 + i;
				
				m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
				m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
				m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
				m_ctrlReadDataGrid.SetText(szData);
			}

	//		AfxMessageBox("Read Page was successful"); 
		}
		else
		{
			AfxMessageBox("Read Page failed"); 
		}
	}
}





void CI2cAdcManualCheckDlg::OnBtnWrite2bData() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	int nWriteByte = 0;
	
	int nResult = TEST_PASS;
	int nIndex  = 0;
	int nLength = 0;

	CString szErrMsg = _T("");
//	CString szTemp	 = _T("");
	
	char szData[10];
	memset(szData,NULL,10);
	
	I2cAdcCtrl.m_nData1 = hexCstr2decNum(m_sz2bWriteAdr);;
	I2cAdcCtrl.m_nData2 = 0;

	nWriteByte = m_ctrl2bWriteDataList.GetLineCount();

	for(int i=0; i<nWriteByte;i++)
	{
		memset(szData,NULL,10);

		nLength = m_ctrl2bWriteDataList.GetLine(i,szData,10);

		szData[nLength] = NULL;

		if(nLength > 2) 
		{
			AfxMessageBox("Data length cannot exceed two digits."); return;
		}

		if(nLength == 0) 
		{
			AfxMessageBox("Data length cannot be zero."); return;
		}
			
		I2cAdcCtrl.m_aWriteData[i] = hexCstr2decNum(szData);
	}

	if(I2cAdcCtrl.WriteI2c2bNbyteData(nWriteByte,m_ctrlReadDataProgress) == TEST_PASS)
	{
//		AfxMessageBox("Write Data was successful"); 
	}
	else
	{
		AfxMessageBox("Write Data failed"); 
	}
}

void CI2cAdcManualCheckDlg::OnBtn2abSetType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	CString szErrMsg = _T("");

	if((nResult = I2cAdcCtrl.SetDDC2AB()) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}
//	AfxMessageBox("SetType was successful."); 
}

void CI2cAdcManualCheckDlg::WriteLog(CString szString)
{
	int nRow = m_ctrlI2cAdcComLogGrid.GetRows();

	if(nRow > 500)
	{
		InitComLogGrid();
		nRow = 0;
	}

	m_ctrlI2cAdcComLogGrid.SetRedraw(FALSE);
	m_ctrlI2cAdcComLogGrid.SetRows(nRow + 1);
	m_ctrlI2cAdcComLogGrid.SetTextMatrix(nRow , 0, szString);
	//+del kwmoon 080826
//	m_ctrlI2cAdcComLogGrid.SetTopRow(nRow);
	m_ctrlI2cAdcComLogGrid.SetRedraw(TRUE);
	m_ctrlI2cAdcComLogGrid.Refresh();
	m_ctrlI2cAdcComLogGrid.SetTopRow(nRow);
}


void CI2cAdcManualCheckDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	m_bActive = FALSE;

	CDialog::OnClose();
}

void CI2cAdcManualCheckDlg::OnBtnClearComLogGrid() 
{
	// TODO: Add your control notification handler code here
	InitComLogGrid();
}

void CI2cAdcManualCheckDlg::OnBtnClearReadData() 
{
	// TODO: Add your control notification handler code here
	InitReadDataGrid();
}

void CI2cAdcManualCheckDlg::OnBtn2bWakeup() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	CString szErrMsg = _T("");

	if((nResult = I2cAdcCtrl.WakeUp()) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}
}

void CI2cAdcManualCheckDlg::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CI2cAdcManualCheckDlg::OnBtn2abToolOptionRead() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	int nReadToolOption = 0;
	int nReadByte		= 2;
	int nIndex			= 0;
	
	CString szErrMsg = _T("");
	CString szTemp	 = _T("");
	CString szData   = _T("");

	m_sz2abReadToolOption = _T("");

	int nToolOptionNo = m_ctrlI2cToolOptionNo.GetCurSel() + 1;

	if((nResult = I2cAdcCtrl.ReadOption(TOOL_OPTION,nToolOptionNo)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}
	else
	{
		nReadToolOption = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH,4));

		m_sz2abReadToolOption.Format("%d",nReadToolOption);
	
		InitReadDataGrid();
		
		for(int i=0; i<nReadByte; i++)
		{
			szData.Format("%02x",hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH+2*i,2)));
			szData.MakeUpper();

		//	nIndex = I2cAdcCtrl.m_nData2 + i;
			
			m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
			m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
			m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlReadDataGrid.SetText(szData);

			nIndex++;
		}	
	}

	UpdateData(FALSE);
}

void CI2cAdcManualCheckDlg::OnBtn2abReadVersion() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	int nCpuVersion   = 0;
	int nMicomVersion   = 0;
	int nUsbVersion		= 0;
	int nReadByte		= 12;
	int nIndex			= 0;
	
	CString szErrMsg = _T("");
	CString szTemp	 = _T("");
	CString szData   = _T("");

	m_sz2abReadToolOption = _T("");

	if((nResult = I2cAdcCtrl.ReadVersion()) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}
	else
	{
		InitReadDataGrid();
		
	
		for(int i=0; i<nReadByte; i++)
		{
			szData.Format("%02x",hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH+2*i,2)));
			szData.MakeUpper();

		//	nIndex = I2cAdcCtrl.m_nData2 + i;
			
			m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
			m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
			m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlReadDataGrid.SetText(szData);

			nIndex++;
		}

		m_sz2abReadVersion = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH,24);
	//	nCpuVersion	  = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH+2,6));
	//	nMicomVersion = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH+8,6));
	//	nUsbVersion   = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH+14,6));
	}

	UpdateData(FALSE);
}

void CI2cAdcManualCheckDlg::OnBtn2abAreaOptionRead() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	int nReadAreaOption = 0;
	int nReadByte		= 2;
	int nIndex			= 0;
	
	CString szErrMsg = _T("");
	CString szTemp	 = _T("");
	CString szData   = _T("");

	m_sz2abReadAreaOption = _T("");

	int nAreaOptionNo = m_ctrlI2cAreaOptionNo.GetCurSel() + 1;

	if((nResult = I2cAdcCtrl.ReadOption(AREA_OPTION,nAreaOptionNo)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}
	else
	{
		nReadAreaOption = hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH,4));

		m_sz2abReadAreaOption.Format("%d",nReadAreaOption);
	
		InitReadDataGrid();
	
		for(int i=0; i<nReadByte; i++)
		{
			szData.Format("%02x",hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH+2*i,2)));
			szData.MakeUpper();

		//	nIndex = I2cAdcCtrl.m_nData2 + i;
			
			m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
			m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
			m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlReadDataGrid.SetText(szData);

			nIndex++;
		}	
	}

	UpdateData(FALSE);
}

void CI2cAdcManualCheckDlg::OnSelchangeCmbToolOptionNo() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ctrlI2cToolOptionNo.GetCurSel();
	
	switch(nCurSel)
	{
		case 0 : 	m_sz2abCurrentToolOption = CurrentSet->sToolOption1; break;
		case 1 : 	m_sz2abCurrentToolOption = CurrentSet->sToolOption2; break;
		case 2 : 	m_sz2abCurrentToolOption = CurrentSet->sToolOption3; break;
	}
	UpdateData(FALSE);
}

void CI2cAdcManualCheckDlg::OnSelchangeCmbAreaOptionNo() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ctrlI2cAreaOptionNo.GetCurSel();
	
	switch(nCurSel)
	{
		case 0 : 	m_sz2abCurrentAreaOption = CurrentSet->sAreaOption1; break;
//		case 1 : 	m_sz2abCurrentAreaOption = CurrentSet->sAreaOption2; break;
	}
	UpdateData(FALSE);	
}


void CI2cAdcManualCheckDlg::OnBtn2abToolOptionWrite() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	int nWriteToolOption = 0;
	int nWriteByte		= 2;
	int nIndex			= 0;
	int nOptionValue	= 0;
	
	CString szErrMsg = _T("");
	CString szTemp	 = _T("");
	CString szData   = _T("");

	int nToolOptionNo = m_ctrlI2cToolOptionNo.GetCurSel() + 1;

	if(m_sz2abWriteToolOption == _T(""))
	{
		szErrMsg.LoadString(ERROR_TOOL_OPTION_NULL);
		AfxMessageBox(szErrMsg); return;
	}

	nOptionValue = atoi(m_sz2abWriteToolOption);

	if((nResult = I2cAdcCtrl.WriteOption(TOOL_OPTION,nToolOptionNo,nOptionValue)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}

	UpdateData(FALSE);
}

void CI2cAdcManualCheckDlg::OnBtn2abAreaOptionWrite() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	int nWriteAreaOption = 0;
	int nWriteByte		= 2;
	int nIndex			= 0;
	int nOptionValue	= 0;
	
	CString szErrMsg = _T("");
	CString szTemp	 = _T("");
	CString szData   = _T("");

	int nAreaOptionNo = m_ctrlI2cAreaOptionNo.GetCurSel() + 1;

	if(m_sz2abWriteAreaOption == _T(""))
	{
		szErrMsg.LoadString(ERROR_AREA_OPTION_NULL);
		AfxMessageBox(szErrMsg); return;
	}

	nOptionValue = atoi(m_sz2abWriteAreaOption);

	if((nResult = I2cAdcCtrl.WriteOption(AREA_OPTION,nAreaOptionNo,nOptionValue)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}

	UpdateData(FALSE);
}

void CI2cAdcManualCheckDlg::OnBtnSendModel() 
{
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	CString sTmp = _T("");
	UINT nNumber = 0;

	m_ctrlModelEdit.GetWindowText(sTmp);
	nNumber = atoi(sTmp);
	
	PatternGeneratorCtrl.SendRun((BYTE)0x07, (int)nNumber);
	

}

void CI2cAdcManualCheckDlg::OnBtnSendPattern() 
{
	CString szCmdString = _T("");
	szCmdString.Format("%c",START_TERMINAL_MODE);

	// Port Open
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PatternGeneratorCtrl.PortClose();
			PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString);  

	CString sTmp = _T("");
	UINT nNumber = 0;
	
	m_ctrlPatternEdit.GetWindowText(sTmp);
	nNumber = atoi(sTmp);
	PatternGeneratorCtrl.SendRun((BYTE)0x08, (int)nNumber);

}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg29() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "D5");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg31() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "BF");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg30() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "D6");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg33() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5A");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg81() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "FB");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg71() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "FF");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg32() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "40");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg14() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "06");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg34() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "41");	
	
}

void CI2cAdcManualCheckDlg::OnBtnRemoteDlg73() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5B");	
	
}

void CI2cAdcManualCheckDlg::OnBtn2abEdidDownload() 
{
	CString sTemp;
	int		nResult		 = TEST_PASS;
	CString szData;

	UpdateData(TRUE);
	InitReadDataGrid();
	m_szEdidDownloadResult = _T("");

	if((nResult = I2cAdcCtrl.SendCmd("F6","0A")) != TEST_PASS)
	{
		sTemp.Format("I2C EDID D/L Start Fail");
		AddStringToStatus(sTemp);
		return;
	}

	DWORD dwStartTick = GetTickCount();

	while (TRUE)
	{
		nResult = I2cAdcCtrl.EDID_Download_ResultCheck();
		if(nResult == TEST_PASS)
		{
			szData = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH,2);
			szData.MakeUpper();

			m_szEdidDownloadResult.Format("%s" ,szData);
			if(CurrentSet->bCommDisplay)
			{
				sTemp.Format("I2C EDID D/L Check : %s",szData);

				AddStringToStatus(sTemp);
				m_ctrlReadDataGrid.SetRow(1);
				m_ctrlReadDataGrid.SetCol(1);
				m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
				m_ctrlReadDataGrid.SetText(szData);
			}

		}

		if((GetTickCount() - dwStartTick) > 5000)		
		{	
			UpdateData(FALSE);
			return;
		}	
	}
	UpdateData(FALSE);
}



void CI2cAdcManualCheckDlg::OnBtn2abReadVersion2() 
{
	UpdateData(TRUE);

	int nResult = TEST_PASS;
	int nCpuVersion   = 0;
	int nMicomVersion   = 0;
	int nUsbVersion		= 0;
	int nReadByte		= 2;
	int nIndex			= 0;
	
	CString szErrMsg = _T("");
	CString szTemp	 = _T("");
	CString szData   = _T("");
#ifdef	DEBUG_MD5_CODE__
	szTemp = "8203C2";
	if (szTemp.Find("82") == 0)
	{
		nReadByte = 3;
	}
	for (int i = 0; i < nReadByte; i++)
	{
		szData.Format("%02x", hexCstr2decNum(szTemp.Mid( 2 * i, 2)));
		szData.MakeUpper();

		//	nIndex = I2cAdcCtrl.m_nData2 + i;			
		m_ctrlReadDataGrid.SetRow(1 + (nIndex / 16));
		m_ctrlReadDataGrid.SetCol(1 + (nIndex % 16));
		m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
		m_ctrlReadDataGrid.SetText(szData);

		nIndex++;
	}
	if (szTemp.Find("82") == 0)
	{
		int lhighVersion = hex2dec(szTemp.GetAt(3));
		int lMidVersion = hex2dec(szTemp.GetAt(4));
		szData.Format("%02d%02d", lhighVersion, lMidVersion);
		szData.MakeUpper();
		m_sz2abReadVersion2 = szData;
	}
	UpdateData(FALSE);
	nIndex = 0;
	return;
#endif
	m_sz2abReadToolOption = _T("");

	if((nResult = I2cAdcCtrl.MNT_ReadVersion()) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AfxMessageBox(szErrMsg); return;
	}
	else
	{
		InitReadDataGrid();		
		szTemp = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH);
		AddStringToStatus( szTemp);//LOG
		if (szTemp.Find("82") == 0)
		{
			nReadByte = 3;
		}
		for(int i=0; i<nReadByte; i++)
		{
			szData.Format("%02x",hexCstr2decNum(I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH+2*i,2)));
			szData.MakeUpper();

		//	nIndex = I2cAdcCtrl.m_nData2 + i;			
			m_ctrlReadDataGrid.SetRow(1+(nIndex / 16));
			m_ctrlReadDataGrid.SetCol(1+(nIndex % 16));
			m_ctrlReadDataGrid.SetCellAlignment(flexAlignCenterCenter);
			m_ctrlReadDataGrid.SetText(szData);

			nIndex++;
		}

		if (szTemp.Find("82") == 0)
		{
			int lhighVersion = hex2dec(szTemp.GetAt(3));
			int lMidVersion = hex2dec(szTemp.GetAt(4));
			szData.Format("%02d%02d", lhighVersion, lMidVersion);
			szData.MakeUpper();
			m_sz2abReadVersion2 = szData;
		}
		else
		{
			m_sz2abReadVersion2 = I2cAdcCtrl.m_szI2cAdcReadString.Mid(CMD_LENGTH,4);
		}
	}
	UpdateData(FALSE);
}
