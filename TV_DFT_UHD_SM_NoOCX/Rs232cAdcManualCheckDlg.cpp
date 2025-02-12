// Rs232cAdcManualCheckDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "Rs232cAdcManualCheckDlg.h"
#include "TVCommCtrl.h"
#include "Global.h"
#include "PatternGeneratorCtl.h"
#include "AvSwitchBoxCtrl.h"
#include "LnbDetectorCtl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;
extern CTVCommCtrl	TVCommCtrl;
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CRs232cAdcManualCheckDlg dialog


CRs232cAdcManualCheckDlg::CRs232cAdcManualCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRs232cAdcManualCheckDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRs232cAdcManualCheckDlg)
	m_szCmd1 = _T("");
	m_szCmd2 = _T("");
	m_nSetId = 0;
	m_szData = _T("");
	m_nEchoOn = 1;
	m_nMaxWait = 3000;
	m_szStrCmd = _T("");
	m_szLNBCmd = _T("");
	//}}AFX_DATA_INIT
	m_bActive = FALSE;
}


void CRs232cAdcManualCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRs232cAdcManualCheckDlg)
	DDX_Control(pDX, IDC_RS232C_COM_LOG, m_ctrlRs232cAdcComLog);
	DDX_Control(pDX, IDC_EDIT_PATTERN, m_ctrlPatternEdit);
	DDX_Control(pDX, IDC_EDIT_MODEL, m_ctrlModelEdit);
	DDX_Control(pDX, IDC_COMBO_RS232C_BAUD_RATE, m_ctrlBaudRate);
	DDX_Control(pDX, IDC_COMBO_RS232C_COM_PORT, m_ctrlComPort);
	DDX_Text(pDX, IDC_EDIT_RS232C_CMD1, m_szCmd1);
	DDX_Text(pDX, IDC_EDIT_RS232C_CMD2, m_szCmd2);
	DDX_Text(pDX, IDC_EDIT_RS232C_SETID, m_nSetId);
	DDX_Text(pDX, IDC_EDIT_RS232C_DATA, m_szData);
	DDX_Radio(pDX, IDC_ECHO_ON_RADIO, m_nEchoOn);
	DDX_Text(pDX, IDC_EDIT_RS232C_MAX_WAIT, m_nMaxWait);
	DDX_Text(pDX, IDC_EDIT_RS232C_STRCMD, m_szStrCmd);
	DDX_Text(pDX, IDC_EDIT_LNB_STRCMD, m_szLNBCmd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRs232cAdcManualCheckDlg, CDialog)
	//{{AFX_MSG_MAP(CRs232cAdcManualCheckDlg)
	ON_BN_CLICKED(IDC_BTN_SET_COM_PORT, OnBtnSetComPort)
	ON_BN_CLICKED(IDC_BTN_SET_ECHO_MODE, OnBtnSetEchoMode)
	ON_BN_CLICKED(IDC_BTN_SEND_COMMAND, OnBtnSendCommand)
	ON_BN_CLICKED(IDC_BTN_CLEAR_RS232C_COM_LOG_GRID, OnBtnClearRs232cComLogGrid)
	ON_CBN_SELCHANGE(IDC_COMBO_RS232C_COM_PORT, OnSelchangeComboRs232cComPort)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_SEND_MODEL, OnBtnSendModel)
	ON_BN_CLICKED(IDC_BTN_SEND_PATTERN, OnBtnSendPattern)
	ON_BN_CLICKED(IDC_BTN_SEND_COMMAND2, OnBtnSendCommand2)
	ON_BN_CLICKED(IDC_BTN_REMOTE_RGB_R, OnBtnRemoteRgbR)
	ON_BN_CLICKED(IDC_BTN_REMOTE_COMP1_R, OnBtnRemoteComp1R)
	ON_BN_CLICKED(IDC_BTN_REMOTE_TV_R, OnBtnRemoteTvR)
	ON_BN_CLICKED(IDC_BTN_REMOTE_AV1_R, OnBtnRemoteAv1R)
	ON_BN_CLICKED(IDC_BTN_REMOTE_INSTART_R, OnBtnRemoteInstartR)
	ON_BN_CLICKED(IDC_BTN_REMOTE_ADJ_R, OnBtnRemoteAdjR)
	ON_BN_CLICKED(IDC_BTN_REMOTE_UP_R, OnBtnRemoteUpR)
	ON_BN_CLICKED(IDC_BTN_REMOTE_RIGHT_R, OnBtnRemoteRightR)
	ON_BN_CLICKED(IDC_BTN_REMOTE_DOWN_R, OnBtnRemoteDownR)
	ON_BN_CLICKED(IDC_BTN_REMOTE_EXIT_R, OnBtnRemoteExitR)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_LNB_V, OnBtnLnbV)
	ON_BN_CLICKED(IDC_BTN_LNB_F, OnBtnLnbF)
	ON_BN_CLICKED(IDC_BTN_SEND_COMMAND3, OnBtnSendCommand3)
	ON_BN_CLICKED(IDC_BTN_KEYDL_CHECK, OnBtnKeydlCheck)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SEND_COMMAND4, &CRs232cAdcManualCheckDlg::OnBnClickedBtnSendCommand4)
	ON_BN_CLICKED(IDC_BTN_GRAB_STOP, &CRs232cAdcManualCheckDlg::OnBnClickedBtnGrabStop)
	ON_BN_CLICKED(IDC_BTN_GRAB_START, &CRs232cAdcManualCheckDlg::OnBnClickedBtnGrabStart)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRs232cAdcManualCheckDlg message handlers

void CRs232cAdcManualCheckDlg::OnBtnSetComPort() 
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	
	CString sErrMsg     = _T("");
	CString sTVComPort  = GetComPortVal(IDC_COMBO_RS232C_COM_PORT);
	DWORD	wTVBaudRate = GetBaudRateVal(IDC_COMBO_RS232C_BAUD_RATE);

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
	else
	{
		TVCommCtrl.PortClose();

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
}

void CRs232cAdcManualCheckDlg::OnBtnSetEchoMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_nEchoOn == 0) 
	{
		CurrentSet->bTVCommPortEcho = TRUE;	

		WriteLog("\r\nEcho Mode is ON");
	}
	else
	{
		CurrentSet->bTVCommPortEcho = FALSE;	

		WriteLog("\r\nEcho Mode is OFF");
	}
}

void CRs232cAdcManualCheckDlg::OnBtnSendCommand() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	CString sCommand;

/*	TVCommCtrl.m_strCmd1	= m_szCmd1;
	TVCommCtrl.m_strCmd2	= m_szCmd2;
	TVCommCtrl.m_nSetId		= m_nSetId;
	TVCommCtrl.m_strData	= m_szData;
	TVCommCtrl.m_nWaitLimit = m_nMaxWait;
*/


	CString sErrMsg		= _T("");
	CString sTVComPort  = GetComPortVal(IDC_COMBO_RS232C_COM_PORT);
	DWORD	wTVBaudRate = GetBaudRateVal(IDC_COMBO_RS232C_BAUD_RATE);

	sCommand.Format("%s%s %02d %s", m_szCmd1, m_szCmd2, m_nSetId, m_szData);

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

	int nResult = TVCommCtrl.Send_TVControl_Command (sCommand,m_nMaxWait);

	switch(nResult)
	{
		case TEST_PASS :
		{
		}
		break;
		case TEST_FAIL :
		{
		}
		break;
		case TEST_ABORT :
		{
		}
		break;
		case TEST_TIMEOUT :
		{
		}
		break;
		case TEST_NOTEST :
		{
		}
		break;
	}
}

void CRs232cAdcManualCheckDlg::OnBtnClearRs232cComLogGrid() 
{
	// TODO: Add your control notification handler code here
//	InitComLogGrid();
	HWND hEdit = m_ctrlRs232cAdcComLog.m_hWnd;

	BOOL bReadOnly = ::GetWindowLong( hEdit, GWL_STYLE ) & ES_READONLY;
	if( bReadOnly )
		::SendMessage( hEdit, EM_SETREADONLY, FALSE, 0 );
	::SendMessage( hEdit, EM_SETSEL, 0, -1 );
	::SendMessage( hEdit, WM_CLEAR, 0, 0 );
	if( bReadOnly )
		::SendMessage( hEdit, EM_SETREADONLY, TRUE, 0 );
}

void CRs232cAdcManualCheckDlg::InitComPort(CString sComPort, DWORD wBaudRate, int nComportCmbID, int nBaudRateCmbID)
{

	CComboBox* pComport  = (CComboBox*)GetDlgItem(nComportCmbID);
	CComboBox* pBaudRate = (CComboBox*)GetDlgItem(nBaudRateCmbID);

	if(sComPort == "COM1")				pComport->SetCurSel(0);
	else if(sComPort == "COM2")			pComport->SetCurSel(1);
	else if(sComPort == "COM3")			pComport->SetCurSel(2);
	else if(sComPort == "COM4")			pComport->SetCurSel(3);
	else if(sComPort == "COM5")			pComport->SetCurSel(4);
	else if(sComPort == "COM6")			pComport->SetCurSel(5);
	else if(sComPort == "COM7")			pComport->SetCurSel(6);
	else if(sComPort == "COM8")			pComport->SetCurSel(7);
	else if(sComPort == "COM9")			pComport->SetCurSel(8);
	else if(sComPort == "\\\\.\\COM10") pComport->SetCurSel(9);
	else if(sComPort == "\\\\.\\COM11") pComport->SetCurSel(10);
	else if(sComPort == "\\\\.\\COM12") pComport->SetCurSel(11);
	else if(sComPort == "\\\\.\\COM13") pComport->SetCurSel(12);
	else if(sComPort == "\\\\.\\COM14") pComport->SetCurSel(13);
	else if(sComPort == "\\\\.\\COM15") pComport->SetCurSel(14);
	else if(sComPort == "\\\\.\\COM16") pComport->SetCurSel(15);
	else pComport->SetCurSel(-1);

	if(wBaudRate == CBR_9600)		 pBaudRate->SetCurSel(0);
	else if(wBaudRate == CBR_19200)  pBaudRate->SetCurSel(1);
	else if(wBaudRate == CBR_38400)  pBaudRate->SetCurSel(2);
	else if(wBaudRate == CBR_57600)  pBaudRate->SetCurSel(3);
	else if(wBaudRate == CBR_115200) pBaudRate->SetCurSel(4);
	else pBaudRate->SetCurSel(-1);
}

CString CRs232cAdcManualCheckDlg::GetComPortVal(int nComportCmbID)
{
	CString sErrMsg  = _T("");
	CString sComPort = _T("");
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nComportCmbID);
	int nCurSel = pCombo->GetCurSel();
	
	switch(nCurSel) 
	{
		case 0 : sComPort.Format("COM1"); break;
		case 1 : sComPort.Format("COM2"); break;
		case 2 : sComPort.Format("COM3"); break;
		case 3 : sComPort.Format("COM4"); break;
		case 4 : sComPort.Format("COM5"); break;
		case 5 : sComPort.Format("COM6"); break;
		case 6 : sComPort.Format("COM7"); break;
		case 7 : sComPort.Format("COM8"); break;
		case 8 : sComPort.Format("COM9"); break;
		case 9 : sComPort.Format("\\\\.\\COM10"); break;
		case 10 : sComPort.Format("\\\\.\\COM11"); break;
		case 11 : sComPort.Format("\\\\.\\COM12"); break;
		case 12 : sComPort.Format("\\\\.\\COM13"); break;
		case 13 : sComPort.Format("\\\\.\\COM14"); break;
		case 14 : sComPort.Format("\\\\.\\COM15"); break;
		case 15 : sComPort.Format("\\\\.\\COM16"); break;
		case 16 : sComPort.Format("\\\\.\\COM17"); break;
		case 17 : sComPort.Format("\\\\.\\COM18"); break;
		case 18 : sComPort.Format("\\\\.\\COM19"); break;
		case 19 : sComPort.Format("\\\\.\\COM20"); break;
		case 20 : sComPort.Format("\\\\.\\COM21"); break;
		case 21 : sComPort.Format("\\\\.\\COM22"); break;
		case 22 : sComPort.Format("\\\\.\\COM23"); break;
		case 23 : sComPort.Format("\\\\.\\COM24"); break;
		default : sComPort.Format("COM1"); break;
	}
	return sComPort;
}

DWORD CRs232cAdcManualCheckDlg::GetBaudRateVal(int nBaudRateCmbID)
{
	DWORD wBaudRate;

	CComboBox* pCombo = (CComboBox*)GetDlgItem(nBaudRateCmbID);
	int nCurSel = pCombo->GetCurSel();

	switch(nCurSel) 
	{
		case 0:  wBaudRate = CBR_9600;		break;
		case 1:  wBaudRate = CBR_19200;		break;
		case 2:  wBaudRate = CBR_38400;		break;
		case 3:  wBaudRate = CBR_57600;		break;
		case 4:  wBaudRate = CBR_115200;	break;
		default: wBaudRate = CBR_9600;		break;
	}
	return wBaudRate;
}

BOOL CRs232cAdcManualCheckDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	//=======================
	// Set ComPort, BaudRate
	//=======================
 	InitComPort(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate,
				IDC_COMBO_RS232C_COM_PORT, IDC_COMBO_RS232C_BAUD_RATE);

//	InitComLogGrid();
//	m_ctrlRs232cAdcComLog.SetEditCtrl( m_ctrlEdit.m_hWnd );
	
	// Redirect cout to our Edit-Control
//	m_pOldBuf = cout.rdbuf( &m_EditStrBuf );
	// and also the unicode version
//	m_pOldBufW = wcout.rdbuf( &m_EditStrBufW );
	// Check it
//	cout << "cout (ANSI) works :-)" << endl;
//	wcout << L"wcout (UNICODE) works also ;-)" << endl;
//	cout << "\n\n";

//	m_nEchoOn = 1;
	CurrentSet->bTVCommPortEcho = FALSE;	
	UpdateData(TRUE);

	m_bActive = TRUE;

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRs232cAdcManualCheckDlg::OnSelchangeComboRs232cComPort() 
{
	// TODO: Add your control notification handler code here
	
}
int CRs232cAdcManualCheckDlg::AppendToLogAndScroll(CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;
	
	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = m_ctrlRs232cAdcComLog.GetWindowTextLength();
	m_ctrlRs232cAdcComLog.SetSel(nInsertionPoint, -1);
	
	// Set the character format
	m_ctrlRs232cAdcComLog.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing 
	// selected, this will simply insert
	// the string at the current caret position.
	m_ctrlRs232cAdcComLog.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible   = g_pView->GetNumVisibleLines(&m_ctrlRs232cAdcComLog);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (&m_ctrlRs232cAdcComLog != m_ctrlRs232cAdcComLog.GetFocus())
	{
		m_ctrlRs232cAdcComLog.LineScroll(INT_MAX);
		m_ctrlRs232cAdcComLog.LineScroll(1 - nVisible);
	}

	return 0;
}

void CRs232cAdcManualCheckDlg::WriteLog(CString szString)
{
	if(!m_bActive) return;
	AppendToLogAndScroll(szString);
	AppendToLogAndScroll("\r\n");
}

void CRs232cAdcManualCheckDlg::InitComLogGrid()
{
	//////////////////////////
	// Remote Com Log
	//////////////////////////
/*
	m_ctrlRs232cAdcComLogGrid.SetRedraw(FALSE);
	m_ctrlRs232cAdcComLogGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); 

	m_ctrlRs232cAdcComLogGrid.SetAllowBigSelection(FALSE);
	m_ctrlRs232cAdcComLogGrid.SetExtendLastCol(TRUE);

	m_ctrlRs232cAdcComLogGrid.SetBackColorBkg(COLOR_WHITE);
	m_ctrlRs232cAdcComLogGrid.SetBackColor(RGB(247,247,231));
	
	m_ctrlRs232cAdcComLogGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlRs232cAdcComLogGrid.SetGridColor(RGB(210,216,176));

	m_ctrlRs232cAdcComLogGrid.SetFixedRows(0);
	m_ctrlRs232cAdcComLogGrid.SetFixedCols(0);

	m_ctrlRs232cAdcComLogGrid.SetGridLines(flexGridFlat);

	m_ctrlRs232cAdcComLogGrid.SetColWidth(0, 6000);
	m_ctrlRs232cAdcComLogGrid.SetColAlignment(0,flexAlignLeftCenter);

	m_ctrlRs232cAdcComLogGrid.SetCols(1);
	m_ctrlRs232cAdcComLogGrid.SetRows(0);
	
	m_ctrlRs232cAdcComLogGrid.SetRedraw(TRUE);
	m_ctrlRs232cAdcComLogGrid.Refresh(); */
}

void CRs232cAdcManualCheckDlg::OnBtnClose() 
{
	//add 090703
//	cout.rdbuf( m_pOldBuf );
//	wcout.rdbuf( m_pOldBufW );

	OnOK();
	m_bActive = FALSE;
}

void CRs232cAdcManualCheckDlg::OnBtnSendModel() 
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

void CRs232cAdcManualCheckDlg::OnBtnSendPattern() 
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


void CRs232cAdcManualCheckDlg::OnBtnSendCommand2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	TVCommCtrl.m_nWaitLimit = m_nMaxWait;

	CString sErrMsg		= _T("");
	CString sTVComPort  = GetComPortVal(IDC_COMBO_RS232C_COM_PORT);
	DWORD	wTVBaudRate = GetBaudRateVal(IDC_COMBO_RS232C_BAUD_RATE);

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

	int nResult = TVCommCtrl.Send_TVControl_Command (m_szStrCmd,m_nMaxWait);


	switch(nResult)
	{
		case TEST_PASS :
		{
		}
		break;
		case TEST_FAIL :
		{
		}
		break;
		case TEST_ABORT :
		{
		}
		break;
		case TEST_TIMEOUT :
		{
		}
		break;
		case TEST_NOTEST :
		{
		}
		break;
	}	
}


void CRs232cAdcManualCheckDlg::OnBtnRemoteRgbR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "D5");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteComp1R() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "BF");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteTvR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "D6");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteAv1R() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5A");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteInstartR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "FB");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteAdjR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "FF");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteUpR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "40");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteRightR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "06");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteDownR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "41");	
	
}

void CRs232cAdcManualCheckDlg::OnBtnRemoteExitR() 
{
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5B");	
	
}

void CRs232cAdcManualCheckDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	//add 090903
//	cout.rdbuf( m_pOldBuf );
//	wcout.rdbuf( m_pOldBufW );
	
}

void CRs232cAdcManualCheckDlg::OnBtnLnbV() 
{
	CString sTemp;
	double dLnbV1;
	double dLnbV2;

	if(gLNBDetectorCtrl.GetVoltageData(dLnbV1, dLnbV2))
	{
		sTemp.Format("LNB Voltage : %.1f V, %.1f V", dLnbV1, dLnbV2 );
		WriteLog(sTemp);
	}
}


void CRs232cAdcManualCheckDlg::OnBtnLnbF() 
{
	CString sTemp;
	double dLnbF1;
	double dLnbF2;

	if(gLNBDetectorCtrl.GetFrequencyData(dLnbF1, dLnbF2))
	{
		sTemp.Format("LNB Frequency : %.1f Khz, %.1f Khz", dLnbF1, dLnbF2);
		WriteLog(sTemp);
	}
}

void CRs232cAdcManualCheckDlg::OnBtnSendCommand3() 
{
	UpdateData(TRUE);

	CString sTemp;
	CString szCmdString;
	CString szData;

	sTemp.Format("%s", m_szLNBCmd);
	WriteLog(sTemp);
	
	szCmdString.Format("%s\n",m_szLNBCmd);
	
	if(!gLNBDetectorCtrl.SendCommString(szCmdString))
	{
        AddStringToStatus(_T("LNB Detector Com. Error"));
		return;
	}
	if(gLNBDetectorCtrl.ReceiveString(szData, 3000))
	{
		sTemp.Format("%s", szData);
		WriteLog(sTemp);
	}
	else{
        AddStringToStatus(_T("LNB Detector Com. Error"));
	}

}

void CRs232cAdcManualCheckDlg::OnBtnKeydlCheck() 
{
	int nKeyVal = 0;
	CString sTemp, sWriteData;
	int nRev = 0;

	nRev = TVCommCtrl.ModelSNo_Read(nKeyVal, TRUE);
	if(nRev != 1){
		nRev = TVCommCtrl.ModelSNo_Read(nKeyVal, FALSE);
	}

	if((nRev == 2) || (nRev == 0)){
		WriteLog("No Key D/L history\r\n");

		return ;
	}

//	sWriteData.Format("DFT071C001");
//	if(!TVCommCtrl.ModelSNo_Write(sWriteData, FALSE)){
//		AddStringToStatus("InputCheckResult(SN Write NG)");
//	}


	sTemp.Format("Key D/L History Val. : %04x", nKeyVal);
	WriteLog(sTemp);
	
}


void CRs232cAdcManualCheckDlg::OnBnClickedBtnSendCommand4()
{
	UpdateData(TRUE);

	TVCommCtrl.m_nWaitLimit = m_nMaxWait;

	CString sErrMsg = _T("");
	CString sTVComPort = GetComPortVal(IDC_COMBO_RS232C_COM_PORT);
	DWORD	wTVBaudRate = GetBaudRateVal(IDC_COMBO_RS232C_BAUD_RATE);

	if (TVCommCtrl.m_bPortOpen == FALSE)
	{
		if (TVCommCtrl.Create(sTVComPort, wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();

			if (TVCommCtrl.Create(sTVComPort, wTVBaudRate) == FALSE)
			{
				sErrMsg.Format("Failed to open COM port (%s)", sTVComPort);
				AfxMessageBox(sErrMsg);
			}
		}
	}

	int nResult = TVCommCtrl.Send_TVControl_Command("aa 00 00", m_nMaxWait);
}


void CRs232cAdcManualCheckDlg::OnBnClickedBtnGrabStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pView->PostMessage(WM_COMMAND, ID_GRAB_STOP);
}


void CRs232cAdcManualCheckDlg::OnBnClickedBtnGrabStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_pView->PostMessage(WM_COMMAND, ID_GRAB_START);
}
