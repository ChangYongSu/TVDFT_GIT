/******************************************************************************
*
* Example program:
*   AcqVoltageSamples_IntClk
*
* Category:
*   AI
*
* Description:
*   This example demonstrates how to acquire a finite amount of data using an
*   internal clock.
*
* Instructions for running:
*   1.  Select the physical channel corresponding to the input signal on the DAQ
*       device.
*   2.  Enter the minimum and maximum voltage ranges.
*       Note:  For better accuracy, try to match the input ranges to the expected
*       voltage level of the measured signal.
*   3.  Select the number of samples per channel to acquire.
*   4.  Set the rate in Hz for the internal clock.
*       Note:  The rate should be at least twice as fast as the maximum frequency
*       component of the signal being acquired.
*
* Steps:
*   1.  Create a new task and an analog input voltage channel.
*   2.  Configure the task to use the internal clock.
*   3.  Create a CNiDAQmxAnalogMultiChannelReader and associate it with the task
*       by using the task's stream.
*   4.  Call CNiDAQmxAnalogMultiChannelReader::ReadMultiSample to acquire and
*       display the data.
*   5.  Destroy the CNiDAQmxTask object to clean-up any resources associated
*       with the task.
*   6.  Handle any CNiDAQmxExceptions, if they occur.
*
* I/O Connections Overview:
*   Make sure your signal input terminals match the physical channel text box. 
*   For more information on the input and output terminals for your device, open
*   the NI-DAQmx Help, and refer to the NI-DAQmx Device Terminals and Device
*   Considerations books in the table of contents.
*
* Microsoft Windows Vista User Account Control
*   Running certain applications on Microsoft Windows Vista requires
*   administrator privileges, because the application name contains keywords
*   such as setup, update, or install. To avoid this problem, you must add an
*   additional manifest to the application that specifies the privileges
*   required to run the application. Some Measurement Studio NI-DAQmx examples
*   for Visual Studio include these keywords. Therefore, all examples for Visual
*   Studio are shipped with an additional manifest file that you must embed in
*   the example executable. The manifest file is named
*   [ExampleName].exe.manifest, where [ExampleName] is the NI-provided example
*   name. For information on how to embed the manifest file, refer to http://msdn2.microsoft.com/en-us/library/bb756929.aspx.
*   
*   Note: The manifest file is not provided with examples for Visual Studio .NET
*   2003.
*
******************************************************************************/

// AcqVoltageSamples_IntClkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgTestPage.h"

#include "AcqVoltageSamples_IntClkDlg.h"
//#include "DlgModelSetup.h"
//#include "MesDb.h"
#include "FileControl.h"
#include "TlHelp32.h"
#include <direct.h>
//#include "DlgModelSetupSeq.h"

#include "IcmpEcho.h"
//#include "ping.h"

//#include "DlgImage.h"
#include "time.h" 
#include "DlgConfig.h"
#include "ModelSelectDlg.h"
#include "CEditDlg.h"
//#include "CableChangeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define 	TASK_RUN_IDLE				0
#define 	TASK_RUN_START				1
#define 	TASK_RUN_PROCESS			2
#define 	TASK_WAIT_SELECT_PROCESS	3
#define 	TASK_FINISH_PROCESS			4
#define 	TASK_WAIT_BARCODE_PROCESS	5

#define DIG_OPTICAL_SELECT	0
#define DIG_COAX_SELECT		1



CStringArray g_Divisions;

//
//void LoadStringVectorInComboBox(CComboBox& comboBox, const CNiStringVector& strings)
//{
//    for (unsigned long i = 0; i < strings.GetSize(); ++i)
//        comboBox.AddString(strings[i]);
//}

// CAcqVoltageSamples_IntClkDlg dialog
CAcqVoltageSamples_IntClkDlg::CAcqVoltageSamples_IntClkDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CAcqVoltageSamples_IntClkDlg::IDD, pParent)
 ///   m_minimum(_T("-5.0")),
  //  m_maximum(_T("5.0")),
  //  m_samplesPerChannel(_T("3000")),
 //   m_rate(_T("100000.0")
	
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    
	/*m_ChnnelName[0] = _T("");
	m_ChnnelName[1] = _T("");
	m_ChnnelName[2] = _T("");
	m_ChnnelName[3] = _T("");
	m_ChnnelName[4] = _T("");
	m_ChnnelName[5] = _T("");*/
	

	m_TaskRun = 0;
	m_RunCounter = 0;

	m_PreRunCounter = 0;
	m_PreTaskRun = 0;
	m_nPreCurrentNumber = 0;

//SERVER	m_pUDPSocket  = NULL;

		
	//for(int i = 0; i < 10; i++)
	//{
	//	m_pUDPSocketServer[i]  = NULL;
	//}

	m_strCameraCheckIMGfile =  _T("btok.jpg");

	m_CheckDelayTime = 100;

	m_SelectedMonitoringEquipID = 0;

	m_AlarmCount = 0;
	m_bPLC_WaitCount = 0;
	m_nPLCReadSize = 0;


}
CAcqVoltageSamples_IntClkDlg::~CAcqVoltageSamples_IntClkDlg()
{
}


void CAcqVoltageSamples_IntClkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PHYSICAL_CHANNEL_COMBO, m_physicalChannel);
	//DDX_Text(pDX, IDC_MINIMUM_EDIT, m_minimum);
	//DDX_Text(pDX, IDC_MAXIMUM_EDIT, m_maximum);
	//DDX_Text(pDX, IDC_SAMPLES_PER_CHANNEL_EDIT, m_samplesPerChannel);
	//DDX_Text(pDX, IDC_RATE_EDIT, m_rate);
	//	DDX_Control(pDX, IDC_START_BUTTON, m_startButton);
	//	DDX_Control(pDX, IDC_LIST_MAIN_PROCESS, m_CtrlListMainProcess);
	//	DDX_Control(pDX, IDC_LIST_MAIN_PROCESS_SCAN, m_CtrlListMainProcessScan);

	//DDX_Control(pDX, IDC_LABEL__OKNG, m_cLabelResult);
	////DDX_Control(pDX, IDC_LABEL_BARCODE, m_Barcode);

	DDX_Control(pDX, IDC_LIST_OQA_MONITOR_SELECT, m_cListCtrl_OQA_Monitor_Select);

	DDX_Control(pDX, IDC_LIST_OQA_MONITOR_1, m_cListCtrl_OQA_Monitor[0]);
	DDX_Control(pDX, IDC_LIST_OQA_MONITOR_2, m_cListCtrl_OQA_Monitor[1]);
	DDX_Control(pDX, IDC_LIST_OQA_MONITOR_3, m_cListCtrl_OQA_Monitor[2]);
	DDX_Control(pDX, IDC_LIST_OQA_MONITOR_4, m_cListCtrl_OQA_Monitor[3]);
	DDX_Control(pDX, IDC_LIST_OQA_MONITOR_5, m_cListCtrl_OQA_Monitor[4]);
	DDX_Control(pDX, IDC_LIST_OQA_MONITOR_6, m_cListCtrl_OQA_Monitor[5]);






	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_1 ,m_TotalAgingTimeLabel[0]);
	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_2 ,m_TotalAgingTimeLabel[1]);
	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_3 ,m_TotalAgingTimeLabel[2]);
	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_4 ,m_TotalAgingTimeLabel[3]);
	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_5 ,m_TotalAgingTimeLabel[4]);
	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_6 ,m_TotalAgingTimeLabel[5]);
	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_7 ,m_TotalAgingTimeLabel[6]);
	//DDX_Control(pDX, IDC_LABEL_TOTAL_AGING_TIME_8 ,m_TotalAgingTimeLabel[7]);


	//DDX_Control(pDX, IDC_LABEL_SELECT1, m_cLB_Select[0]);
	//DDX_Control(pDX, IDC_LABEL_SELECT2, m_cLB_Select[1]);
	//DDX_Control(pDX, IDC_LABEL_SELECT3, m_cLB_Select[2]);
	//DDX_Control(pDX, IDC_LABEL_SELECT4, m_cLB_Select[3]);
	//DDX_Control(pDX, IDC_LABEL_SELECT5, m_cLB_Select[4]);
	//DDX_Control(pDX, IDC_LABEL_SELECT6, m_cLB_Select[5]);
	//DDX_Control(pDX, IDC_LABEL_SELECT7, m_cLB_Select[6]);
	//DDX_Control(pDX, IDC_LABEL_SELECT8, m_cLB_Select[7]);
	/*DDX_Control(pDX, IDC_LABEL_BARCODE1, m_cLB_BarCode[0]);
	DDX_Control(pDX, IDC_LABEL_BARCODE2, m_cLB_BarCode[1]);
	DDX_Control(pDX, IDC_LABEL_BARCODE3, m_cLB_BarCode[2]);
	DDX_Control(pDX, IDC_LABEL_BARCODE4, m_cLB_BarCode[3]);
	DDX_Control(pDX, IDC_LABEL_BARCODE5, m_cLB_BarCode[4]);
	DDX_Control(pDX, IDC_LABEL_BARCODE6, m_cLB_BarCode[5]);
	DDX_Control(pDX, IDC_LABEL_BARCODE7, m_cLB_BarCode[6]);
	DDX_Control(pDX, IDC_LABEL_BARCODE8, m_cLB_BarCode[7]);

	DDX_Control(pDX, IDC_LABEL_CNT_FM_1, m_cLB_Cnt_FM[0]);
	DDX_Control(pDX, IDC_LABEL_CNT_FM_2, m_cLB_Cnt_FM[1]);
	DDX_Control(pDX, IDC_LABEL_CNT_FM_3, m_cLB_Cnt_FM[2]);
	DDX_Control(pDX, IDC_LABEL_CNT_FM_4, m_cLB_Cnt_FM[3]);
	DDX_Control(pDX, IDC_LABEL_CNT_FM_5, m_cLB_Cnt_FM[4]);
	DDX_Control(pDX, IDC_LABEL_CNT_FM_6, m_cLB_Cnt_FM[5]);
	DDX_Control(pDX, IDC_LABEL_CNT_FM_7, m_cLB_Cnt_FM[6]);
	DDX_Control(pDX, IDC_LABEL_CNT_FM_8, m_cLB_Cnt_FM[7]);

	DDX_Control(pDX, IDC_LABEL_TIME_1, m_cLB_Cnt_Time[0]);
	DDX_Control(pDX, IDC_LABEL_TIME_2, m_cLB_Cnt_Time[1]);
	DDX_Control(pDX, IDC_LABEL_TIME_3, m_cLB_Cnt_Time[2]);
	DDX_Control(pDX, IDC_LABEL_TIME_4, m_cLB_Cnt_Time[3]);
	DDX_Control(pDX, IDC_LABEL_TIME_5, m_cLB_Cnt_Time[4]);
	DDX_Control(pDX, IDC_LABEL_TIME_6, m_cLB_Cnt_Time[5]);
	DDX_Control(pDX, IDC_LABEL_TIME_7, m_cLB_Cnt_Time[6]);
	DDX_Control(pDX, IDC_LABEL_TIME_8, m_cLB_Cnt_Time[7]);
	*/

	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_1, m_cLB_SetStatus[0]);
	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_2, m_cLB_SetStatus[1]);
	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_3, m_cLB_SetStatus[2]);
	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_4, m_cLB_SetStatus[3]);
	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_5, m_cLB_SetStatus[4]);
	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_6, m_cLB_SetStatus[5]);
	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_7, m_cLB_SetStatus[6]);
	//DDX_Control(pDX, IDC_LABEL_SET_STATUS_8, m_cLB_SetStatus[7]);

	//DDX_Control(pDX, IDC_LABEL_LAN_1, m_cLB_Lan_Status[0]);
	//DDX_Control(pDX, IDC_LABEL_LAN_2, m_cLB_Lan_Status[1]);
	//DDX_Control(pDX, IDC_LABEL_LAN_3, m_cLB_Lan_Status[2]);
	//DDX_Control(pDX, IDC_LABEL_LAN_4, m_cLB_Lan_Status[3]);
	//DDX_Control(pDX, IDC_LABEL_LAN_5, m_cLB_Lan_Status[4]);
	//DDX_Control(pDX, IDC_LABEL_LAN_6, m_cLB_Lan_Status[5]);
	//DDX_Control(pDX, IDC_LABEL_LAN_7, m_cLB_Lan_Status[6]);
	//DDX_Control(pDX, IDC_LABEL_LAN_8, m_cLB_Lan_Status[7]);

	//DDX_Control(pDX, IDC_LABEL_1_USB_1, m_cLB_USB1_Status[0]);
	//DDX_Control(pDX, IDC_LABEL_1_USB_2, m_cLB_USB1_Status[1]);
	//DDX_Control(pDX, IDC_LABEL_1_USB_3, m_cLB_USB1_Status[2]);
	//DDX_Control(pDX, IDC_LABEL_1_USB_4, m_cLB_USB1_Status[3]);
	//DDX_Control(pDX, IDC_LABEL_1_USB_5, m_cLB_USB1_Status[4]);
	//DDX_Control(pDX, IDC_LABEL_1_USB_6, m_cLB_USB1_Status[5]);
	//DDX_Control(pDX, IDC_LABEL_1_USB_7, m_cLB_USB1_Status[6]);
	//DDX_Control(pDX, IDC_LABEL_1_USB_8, m_cLB_USB1_Status[7]);

	//DDX_Control(pDX, IDC_LABEL_2_USB_1, m_cLB_USB2_Status[0]);
	//DDX_Control(pDX, IDC_LABEL_2_USB_2, m_cLB_USB2_Status[1]);
	//DDX_Control(pDX, IDC_LABEL_2_USB_3, m_cLB_USB2_Status[2]);
	//DDX_Control(pDX, IDC_LABEL_2_USB_4, m_cLB_USB2_Status[3]);
	//DDX_Control(pDX, IDC_LABEL_2_USB_5, m_cLB_USB2_Status[4]);
	//DDX_Control(pDX, IDC_LABEL_2_USB_6, m_cLB_USB2_Status[5]);
	//DDX_Control(pDX, IDC_LABEL_2_USB_7, m_cLB_USB2_Status[6]);
	//DDX_Control(pDX, IDC_LABEL_2_USB_8, m_cLB_USB2_Status[7]);

	//DDX_Control(pDX, IDC_LABEL_POWER1, m_cLB_PowerStatus[0]);
	//DDX_Control(pDX, IDC_LABEL_POWER2, m_cLB_PowerStatus[1]);
	//DDX_Control(pDX, IDC_LABEL_POWER3, m_cLB_PowerStatus[2]);
	//DDX_Control(pDX, IDC_LABEL_POWER4, m_cLB_PowerStatus[3]);
	//DDX_Control(pDX, IDC_LABEL_POWER5, m_cLB_PowerStatus[4]);
	//DDX_Control(pDX, IDC_LABEL_POWER6, m_cLB_PowerStatus[5]);
	//DDX_Control(pDX, IDC_LABEL_POWER7, m_cLB_PowerStatus[6]);
	//DDX_Control(pDX, IDC_LABEL_POWER8, m_cLB_PowerStatus[7]);

	//DDX_Control(pDX, IDC_LABEL_OPEN1, m_cLB_OpenStatus[0]);
	//DDX_Control(pDX, IDC_LABEL_OPEN2, m_cLB_OpenStatus[1]);
	//DDX_Control(pDX, IDC_LABEL_OPEN3, m_cLB_OpenStatus[2]);
	//DDX_Control(pDX, IDC_LABEL_OPEN4, m_cLB_OpenStatus[3]);
	//DDX_Control(pDX, IDC_LABEL_OPEN5, m_cLB_OpenStatus[4]);
	//DDX_Control(pDX, IDC_LABEL_OPEN6, m_cLB_OpenStatus[5]);
	//DDX_Control(pDX, IDC_LABEL_OPEN7, m_cLB_OpenStatus[6]);
	//DDX_Control(pDX, IDC_LABEL_OPEN8, m_cLB_OpenStatus[7]);






	//DDX_Control(pDX, IDC_LABEL_AC_1, m_cLB_AC_Power[0]);
	//DDX_Control(pDX, IDC_LABEL_AC_2, m_cLB_AC_Power[1]);
	//DDX_Control(pDX, IDC_LABEL_AC_3, m_cLB_AC_Power[2]);
	//DDX_Control(pDX, IDC_LABEL_AC_4, m_cLB_AC_Power[3]);
	//DDX_Control(pDX, IDC_LABEL_AC_5, m_cLB_AC_Power[4]);
	//DDX_Control(pDX, IDC_LABEL_AC_6, m_cLB_AC_Power[5]);
	//DDX_Control(pDX, IDC_LABEL_AC_7, m_cLB_AC_Power[6]);
	//DDX_Control(pDX, IDC_LABEL_AC_8, m_cLB_AC_Power[7]);

	//DDX_Control(pDX, IDC_BUTTON_EXIT, m_cBtnExit);

	DDX_Control(pDX, IDC_LABEL_EQUIP_DFT_SELECT, m_cLbEquipSelect);
	DDX_Control(pDX, IDC_LABEL_EQUIP_1, m_cLbEquip[0]);
	DDX_Control(pDX, IDC_LABEL_EQUIP_2, m_cLbEquip[1]);
	DDX_Control(pDX, IDC_LABEL_EQUIP_3, m_cLbEquip[2]);
	DDX_Control(pDX, IDC_LABEL_EQUIP_4, m_cLbEquip[3]);
	DDX_Control(pDX, IDC_LABEL_EQUIP_5, m_cLbEquip[4]);
	DDX_Control(pDX, IDC_LABEL_EQUIP_6, m_cLbEquip[5]);


	//DDX_Control(pDX, IDC_LABEL_COM_MAIN1, m_cLb_ComMain);
	//DDX_Control(pDX, IDC_LABEL_COM_GMES1, m_cLb_ComMainGmes);
	//DDX_Control(pDX, IDC_LABEL_COM_MONITOR1, m_cLb_ComMainMonitor);
	DDX_Control(pDX, IDC_LABEL_SERVER, m_cLbServerLabel);

	DDX_Control(pDX, IDC_LABEL_PCBA_SN_DFT1, m_cLabelPCBASN[0]);
	DDX_Control(pDX, IDC_LABEL_PCBA_SN_DFT2, m_cLabelPCBASN[1]);
	DDX_Control(pDX, IDC_LABEL_PCBA_SN_DFT3, m_cLabelPCBASN[2]);
	DDX_Control(pDX, IDC_LABEL_PCBA_SN_DFT4, m_cLabelPCBASN[3]);
	DDX_Control(pDX, IDC_LABEL_PCBA_SN_DFT5, m_cLabelPCBASN[4]);
	DDX_Control(pDX, IDC_LABEL_PCBA_SN_DFT6, m_cLabelPCBASN[5]);

	DDX_Control(pDX, IDC_LABEL_PLC_ROBOT_STATUS_DFT1, m_cLabelCtrlStatus[0]);
	DDX_Control(pDX, IDC_LABEL_PLC_ROBOT_STATUS_DFT2, m_cLabelCtrlStatus[1]);
	DDX_Control(pDX, IDC_LABEL_PLC_ROBOT_STATUS_DFT3, m_cLabelCtrlStatus[2]);
	DDX_Control(pDX, IDC_LABEL_PLC_ROBOT_STATUS_DFT4, m_cLabelCtrlStatus[3]);
	DDX_Control(pDX, IDC_LABEL_PLC_ROBOT_STATUS_DFT5, m_cLabelCtrlStatus[4]);
	DDX_Control(pDX, IDC_LABEL_PLC_ROBOT_STATUS_DFT6, m_cLabelCtrlStatus[5]);
	//CLabel_text m_cLabelCtrlStatus[6];//IDC_LABEL_PLC_ROBOT_STATUS_DFT1

	DDX_Control(pDX, IDC_LABEL_PCB_IN_STATUS_DFT1, m_cLabelPcbStatus[0]);
	DDX_Control(pDX, IDC_LABEL_PCB_IN_STATUS_DFT2, m_cLabelPcbStatus[1]);
	DDX_Control(pDX, IDC_LABEL_PCB_IN_STATUS_DFT3, m_cLabelPcbStatus[2]);
	DDX_Control(pDX, IDC_LABEL_PCB_IN_STATUS_DFT4, m_cLabelPcbStatus[3]);
	DDX_Control(pDX, IDC_LABEL_PCB_IN_STATUS_DFT5, m_cLabelPcbStatus[4]);
	DDX_Control(pDX, IDC_LABEL_PCB_IN_STATUS_DFT6, m_cLabelPcbStatus[5]);
	//CLabel_text m_cLabelPcbStatus[6];//IDC_LABEL_PCB_IN_STATUS_DFT1

	DDX_Control(pDX, IDC_LABEL_PROCESS_STATUS_DFT1, m_cLabelProcessStatus[0]);
	DDX_Control(pDX, IDC_LABEL_PROCESS_STATUS_DFT2, m_cLabelProcessStatus[1]);
	DDX_Control(pDX, IDC_LABEL_PROCESS_STATUS_DFT3, m_cLabelProcessStatus[2]);
	DDX_Control(pDX, IDC_LABEL_PROCESS_STATUS_DFT4, m_cLabelProcessStatus[3]);
	DDX_Control(pDX, IDC_LABEL_PROCESS_STATUS_DFT5, m_cLabelProcessStatus[4]);
	DDX_Control(pDX, IDC_LABEL_PROCESS_STATUS_DFT6, m_cLabelProcessStatus[5]);
	//CLabel_text m_cLabelProcessStatus[6];//IDC_LABEL_PROCESS_STATUS_DFT1

	DDX_Control(pDX, IDC_LABEL_BTN_START_DFT1, m_cLabelBtnStart[0]);
	DDX_Control(pDX, IDC_LABEL_BTN_START_DFT2, m_cLabelBtnStart[1]);
	DDX_Control(pDX, IDC_LABEL_BTN_START_DFT3, m_cLabelBtnStart[2]);
	DDX_Control(pDX, IDC_LABEL_BTN_START_DFT4, m_cLabelBtnStart[3]);
	DDX_Control(pDX, IDC_LABEL_BTN_START_DFT5, m_cLabelBtnStart[4]);
	DDX_Control(pDX, IDC_LABEL_BTN_START_DFT6, m_cLabelBtnStart[5]);
	//CLabel_text m_cLabelBtnStart[6];//IDC_LABEL_BTN_START_DFT1


	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_DFT1, m_cLabelBtnPause[0]);
	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_DFT2, m_cLabelBtnPause[1]);
	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_DFT3, m_cLabelBtnPause[2]);
	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_DFT4, m_cLabelBtnPause[3]);
	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_DFT5, m_cLabelBtnPause[4]);
	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_DFT6, m_cLabelBtnPause[5]);
	//CLabel_text m_cLabelBtnPause[6];//IDC_LABEL_BTN_PAUSE_DFT1	


	DDX_Control(pDX, IDC_LABEL_BTN_STOP_DFT1, m_cLabelBtnStop[0]);
	DDX_Control(pDX, IDC_LABEL_BTN_STOP_DFT2, m_cLabelBtnStop[1]);
	DDX_Control(pDX, IDC_LABEL_BTN_STOP_DFT3, m_cLabelBtnStop[2]);
	DDX_Control(pDX, IDC_LABEL_BTN_STOP_DFT4, m_cLabelBtnStop[3]);
	DDX_Control(pDX, IDC_LABEL_BTN_STOP_DFT5, m_cLabelBtnStop[4]);
	DDX_Control(pDX, IDC_LABEL_BTN_STOP_DFT6, m_cLabelBtnStop[5]);
	//CLabel_text m_cLabelBtnStop[6];//IDC_LABEL_BTN_STOP_DFT1


	DDX_Control(pDX, IDC_LABEL_MODEL_NAME_VALUE, m_cLB_ModelName);
	DDX_Control(pDX, IDC_LABEL_SUFFIX_VALUE, m_cLB_Suffix);
	DDX_Control(pDX, IDC_LABEL_BTN_START_ALL, m_cLB_BtnStartAll);
	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_ALL, m_cLB_BtnPauseAll);
	DDX_Control(pDX, IDC_LABEL_BTN_STOP_ALL, m_cLB_BtnStopAll);

	DDX_Control(pDX, IDC_LABEL_BTN_START_SELECT, m_cLB_BtnStartSelect);
	DDX_Control(pDX, IDC_LABEL_BTN_PAUSE_SELECT, m_cLB_BtnPauseSelect);
	DDX_Control(pDX, IDC_LABEL_BTN_STOP_SELECT, m_cLB_BtnStopSelect);

	DDX_Control(pDX, IDC_LABEL_BTN_CHANGE_MODEL, m_cLB_BtnChangeModel);
	DDX_Control(pDX, IDC_LABEL_BTN_SCAN_ON, m_cLB_BtnScanOn);
	DDX_Control(pDX, IDC_LABEL_BTN_GET_TOOL_OPTION, m_cLB_BtnGetToolOption);
	DDX_Control(pDX, IDC_LABEL_BTN_GET_TOOL_CLOSE, m_cLB_BtnGetToolOptionClose);
	DDX_Control(pDX, IDC_LABEL_BTN_SELECT_DFT, m_cLB_BtnSelectDFT);
	
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_AUTO, m_cLB_PLC_Data_Auto);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_MAN, m_cLB_PLC_Data_Manual);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_START, m_cLB_PLC_Data_Start);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_PAUSE, m_cLB_PLC_Data_Pause);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_STOP, m_cLB_PLC_Data_Stop);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_WARN, m_cLB_PLC_Data_Warn);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_ALARM, m_cLB_PLC_Data_Alarm);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_SKIP_FFC, m_cLB_PLC_Data_SkipFFC);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_PCB_SENSOR, m_cLB_PLC_Data_PcbSensor);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_JIG_UP, m_cLB_PLC_Data_JigUp);
	DDX_Control(pDX, IDC_LABEL_PLC_DATA_JIG_DOWN, m_cLB_PLC_Data_JigDown);

	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_JIGUP, m_cLB_PLC_Ctrl_JigUp);
	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_JIG_DOWN, m_cLB_PLC_Ctrl_JigDown);
	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_LOAD, m_cLB_PLC_Ctrl_Load);
	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_TEST_PASS, m_cLB_PLC_Ctrl_TestPass);
	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_TEST_NG, m_cLB_PLC_Ctrl_TestNG);
	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_CLEAR, m_cLB_PLC_Ctrl_Clear);
	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_READ, m_cLB_PLC_Ctrl_Read);
	DDX_Control(pDX, IDC_LABEL_BTN_CTRL_AC_ON_OFF, m_cLB_PLC_Ctrl_AC_ONOFF);
	
	

	//DDX_Control(pDX, IDC_LIST_MAIN_PROCESS_LOG, m_cListCtrlMainLog);
	DDX_Control(pDX, IDC_LIST_PROCESS_LOG, m_cListMainProcessLog);
	DDX_Control(pDX, IDC_LABEL_COM_MONITOR_SCAN, m_cLabelComScan);
	DDX_Control(pDX, IDC_LABEL_COUNT_DISPLAY, m_cLabelCountDisplay);
	
}


BEGIN_MESSAGE_MAP(CAcqVoltageSamples_IntClkDlg, CDialog)
    //{{ AFX_MSG_MAP
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}} AFX_MSG_MAP
  //  ON_BN_CLICKED(IDC_START_BUTTON, OnBnClickedStart)
	ON_WM_TIMER()
	//ON_BN_CLICKED(IDC_STOP_BUTTON, &CAcqVoltageSamples_IntClkDlg::OnBnClickedStopButton)
	//ON_BN_CLICKED(IDC_SETUP_BUTTON, &CAcqVoltageSamples_IntClkDlg::OnBnClickedSetupButton)
	//ON_BN_CLICKED(IDC_TEST_BUTTON, &CAcqVoltageSamples_IntClkDlg::OnBnClickedTestButton)
	//ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveData)
	//ON_MESSAGE(WM_COMM_RXCHAR_PCB,OnReceiveDataPCB)
	//ON_MESSAGE(WM_COMM_RXCHAR_LED,OnReceiveDataLED)
	//ON_MESSAGE(WM_COMM_RXCHAR_ETHERNET, OnReceiveDataEthernet)
	//ON_MESSAGE(WM_COMM_RXCHAR_SCAN,OnReceiveDataSCAN)

	//ON_WM_DESTROY()
	//ON_BN_CLICKED(IDOK, &CAcqVoltageSamples_IntClkDlg::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//ON_NOTIFY(HDN_ITEMCLICK, 0, &CAcqVoltageSamples_IntClkDlg::OnHdnItemclickListMainProcess)
	//ON_NOTIFY(NM_CLICK, IDC_LIST_MAIN_PROCESS, &CAcqVoltageSamples_IntClkDlg::OnNMClickListMainProcess)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MAIN_PROCESS, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListMainProcess)
//	ON_STN_DBLCLK(IDC_STATIC_PIC, &CAcqVoltageSamples_IntClkDlg::OnStnDblclickStaticPic)
//	ON_STN_CLICKED(IDC_STATIC_PIC, &CAcqVoltageSamples_IntClkDlg::OnStnClickedStaticPic)
	ON_WM_RBUTTONDBLCLK()
//	ON_CBN_SELCHANGE(IDC_COMBO_NAME_1, &CAcqVoltageSamples_IntClkDlg::OnCbnSelchangeComboName1)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//ON_BN_CLICKED(IDC_BUTTON_EXIT, &CAcqVoltageSamples_IntClkDlg::OnBnClickedButtonExit)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_SELECT, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitorSelect)

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_1, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_2, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_3, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_4, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_5, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_6, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor6)
	ON_MESSAGE(WM_COMM_RXCHAR_SCAN, OnReceiveDataSCAN)
//	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_7, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor7)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_8, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor8)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_9, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor9)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_10, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor10)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_11, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor11)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OQA_MONITOR_12, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor12)

	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int GetProcessList(CString lcurname, int MaxNum)
{
	CString lfname;
    int Index = 0;
    HANDLE         hProcessSnap = NULL;
    DWORD          Return       = FALSE;
    PROCESSENTRY32 pe32         = {0};

    //프로세스의 스냅샷을 생성한다.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return 0; 

    pe32.dwSize = sizeof(PROCESSENTRY32);

    //시스템의 모든 프로세스를 가져와 processName와 이름이 같은 것의 카운트를 리턴.
    if (Process32First(hProcessSnap, &pe32))
    {
        DWORD Code = 0;
        DWORD         dwPriorityClass;        

        do
        {
            HANDLE hProcess; 

            // Get the actual priority class.
            hProcess = OpenProcess (PROCESS_ALL_ACCESS,
                FALSE, pe32.th32ProcessID);
            dwPriorityClass = GetPriorityClass (hProcess);              

            if (Index < MaxNum)
            {
                //pProcessList[Index].ProcessName = pe32.szExeFile;
                lfname = pe32.szExeFile;
				if(lfname == lcurname)
				{
					Index++;
				}
            }

            CloseHandle (hProcess);      

        }
        while (Process32Next(hProcessSnap, &pe32));
    } 

    else
        return 0;
           // could not walk the list of processes 

    // Do not forget to clean up the snapshot object.
    CloseHandle (hProcessSnap);

    return Index;

}

BOOL CAcqVoltageSamples_IntClkDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			//ClickCbtnStart();
			//if(m_BarcodeString.GetLength() > 6)
			//{
			//	CheckOKNG(m_BarcodeString);
			//	//m_WipID = m_BarcodeString;
			//}
			//m_BarcodeString = _T("");
			m_SuffixTmpString = _T("");

			return TRUE;
		}
	}		
	else if(pMsg->message == WM_CHAR)
	{
		//m_BarcodeString = m_BarcodeString  + (char)(pMsg->wParam);
		//m_BarcodeString.MakeUpper();
		//m_Barcode.put_Caption(m_BarcodeString);

		if (m_ScanEnable == 1)
		{
			if ((char)(pMsg->wParam) == VK_BACK)
			{
				m_SuffixTmpString = m_SuffixTmpString.Left(m_SuffixTmpString.GetLength()-1);
				m_SuffixTmpString.MakeUpper();
				m_cLB_Suffix.put_Caption(m_SuffixTmpString);
			}
			else if (((char)(pMsg->wParam) >= VK_SPACE)&&((char)(pMsg->wParam) < 0x7F))
			{
				m_SuffixTmpString = m_SuffixTmpString + (char)(pMsg->wParam);
				m_SuffixTmpString.MakeUpper();
				m_cLB_Suffix.put_Caption(m_SuffixTmpString);
			}
		}

		return TRUE;		
	}	
	else if(pMsg->message == WM_LBUTTONDOWN)
	{
		if(m_TaskRun != TASK_RUN_IDLE)
		{
//			RECT lRect;
			//m_CtrlListMainProcess.GetWindowRect(&lRect);		
			//if((lRect.bottom > pMsg->pt.y)&&(lRect.top < pMsg->pt.y)
			//	&&(lRect.left < pMsg->pt.x)&&(lRect.right > pMsg->pt.x))
			//{
			//	return TRUE;
			//}
		}
	}
	
	
	return CDialog::PreTranslateMessage(pMsg);
}


#define _MANUAL_SCOPE_ADDMODE
// CAcqVoltageSamples_IntClkDlg message handlers

BOOL CAcqVoltageSamples_IntClkDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    // Set the icon for this dialog.  The framework does this automatically
    // when the application's main window is not a dialog.
    SetIcon(m_hIcon, TRUE);     // Set big icon
    SetIcon(m_hIcon, FALSE);    // Set small icon


#ifdef __START_DEBUG_MODE_
	MessageBox("1");
#endif
	m_RecvCntSCAN = 0;
	m_WaitPCB_Ack_Limit = 30;
	int lMakeflag = 0;

	CString str, BaseStr;
	BaseStr = _T(PATH_BASE_DIR);
	
	str = _T("D:\\DFT");
	if(_mkdir(str.GetBuffer())==0)
	{
		lMakeflag = 1;
	}	
	
	if(_mkdir(BaseStr.GetBuffer())==0)
	{
		lMakeflag = 1;
	}

	//str = _T(PATH_BASE_Back_DIR);
	str = BaseStr + _T("\\Log");
	
	if(_mkdir(str.GetBuffer())==0)
	{
		lMakeflag = 1;//MessageBox("Make New Directory => [d:\\nagra\\log]");
	}		
	
	

	str = BaseStr + _T("\\Sound");

	if(_mkdir(str.GetBuffer())==0)
	{
		lMakeflag = 1;//MessageBox("Make New Directory => [d:\\nagra\\data]");
	}	



//	CString str;

	float lfval = 0.0;
	//str.Format(_T("%.1f (s)"), lfval);	
	//m_CheckTimeLabel.put_Caption(str);

	//lfval = 0.0;
	//str.Format(_T("%.1f (s)"), lfval);	
	//m_CycleTimeLabel.put_Caption(str);

	CFileControl lFile;
	lFile.ReadCountData();

	lFile.LoadData();

	


	if(lFile.ReadCableCountData() != 0)
	{
		lFile.SaveCableCountData();
	}
	


#ifdef __START_DEBUG_MODE_
	MessageBox("2");
#endif
/////////////////////////////////////////////////////


	if(lFile.Read_ConfigSeqData())
	{
		lFile.SaveConfigSeqData();
	}

	lFile.ReadCountData();

	DisplayCountData();

#ifdef __START_DEBUG_MODE_
	MessageBox("3");
#endif


	SetOQAListControl();

	for(int i = 0 ; i < 6 ; i++)
	{
		m_sListenSocket[i].SetParent(this);
		m_pUDPSocketServer[i].SetParent(this);
		m_pPLCSocketClient.SetParent(this);
			
		gSaveInfo[i].strSystemStatus = 3;			
	}
	
	CreateEthernetSocket();

	//gPLC_Ctrl.
		
	IcmpEcho ping;
//	unsigned long ulIP;
	DWORD delay;
	CString strtmp;
	CString strtmpIPAddress;
	delay = 1000;	

	OpenComportSCAN();



	SetTimer(1,30,NULL);
	SetTimer(2,1000,NULL);
	SetTimer(3, 500, NULL);
	SetTimer(5, 2000, NULL);

	//OnBnClickedButtonFreqSet();

	mpDlgResult = new CDlgResult;	
	mpDlgResult->Create(CDlgResult::IDD, this);
	mpDlgResult->ShowWindow(SW_HIDE);//SetWindowPos(this,rectP.left+ 15,rectP.top+160,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);
	mpDlgResult->pDlg = this;

	m_SelectDFT = DFT_SELECT_ALL;


#ifdef __START_DEBUG_MODE_
	MessageBox("12");
#endif
//	SavePLC_UPH();
	//NG_Display("rorouerqrpotq\r\nuuhrthert\r\njhhguoyt");
	// Return TRUE unless you set the focus to a control.
    return TRUE;
}

BOOL CAcqVoltageSamples_IntClkDlg::CheckOKNG(CString l_BarcodeString)//SaveLogScan(SaveLogDataT lSaveData )
{
	//gLoadDataInfo[100000];
	int i = 0;
	int lFoundFlag = 0;
	unsigned int lFoundNo = 0;
	int lResult = 0;
	CFileControl lFile;
	m_cLabelResult.put_Caption("WAIT");
	m_cLabelResult.put_BackColor(0x00FFFF);

	//for( i = 0; i < gDataCount; i++)
	//{
	//	if(gLoadDataInfo[i].strWipID_SetSN.Compare(l_BarcodeString) == 0)
	//	{
	//		lFoundFlag = 1;
	//		lFoundNo = i;
	//		
	//		if(gLoadDataInfo[i].strTestResult.Compare("OK") == 0)
	//		{
	//			lResult = 1;
	//			break;
	//		}
	//		else //if(gLoadDataInfo[i].strTestResult.Compare("NG") == 0)
	//		{
	//			lResult = 2;
	//			//break;
	//		}
	//	}
	//}

	if(lFoundFlag == 1)
	{
		if(lResult == 1)
		{
			m_cLabelResult.put_Caption("PASS");
			m_cLabelResult.put_BackColor(0x00FF00);
			lFile.SaveLogScan(gLoadDataInfo[lFoundNo] );
		}
		else// if(lResult == 2)
		{
			m_cLabelResult.put_Caption("NG");
			m_cLabelResult.put_BackColor(0x0000FF);
			lFile.SaveLogScan(gLoadDataInfo[lFoundNo] );
		}

	//	SetSubListControlScan(gLoadDataInfo[lFoundNo]);
	}
	else
	{
			m_cLabelResult.put_Caption("FAIL");
			m_cLabelResult.put_BackColor(0x00FFFF);
			//lFile.SaveLogScan(gLoadDataInfo[lFoundNo] );

	}
		CString m_NgWaveFile  = _T(PATH_BASE_DIR);
	m_NgWaveFile  += _T("\\Sound\\OK_CHECK.wav");//NG_CHECK.wav");//OK_CHECK.wav

	//sndPlaySound( m_NgWaveFile , SND_ASYNC);
	return 0;
}



BOOL CAcqVoltageSamples_IntClkDlg::CreateEthernetSocketSvr(int lIndex) 
{
	BOOL lresult = 0;
	//SERVER
	if(m_sListenSocket[lIndex].m_SocketOpen != 0)
	{
		m_sListenSocket[lIndex].Close();
	}
	lresult = m_sListenSocket[lIndex].Create(g_nLocalPortNumber[lIndex]);
	lresult = m_sListenSocket[lIndex].Listen();
	m_sListenSocket[lIndex].m_SocketID = lIndex;
	//if(lresult == 1)
	m_sListenSocket[lIndex].m_SocketOpen = 1;
	return TRUE;
}


BOOL CAcqVoltageSamples_IntClkDlg::CreateEthernetSocketPLC(CString sIpAdd, int nPort)
{
#if 1
	BOOL l_bPLCCom = 0;
	SOCKET connt;


	IcmpEcho ping;
	unsigned long ulIP;
	DWORD delay;
	CString strtmp;
	CString strtmpIPAddress;
	delay = 1000;
#ifdef _NO_CONNECT_DEBUG__MODE
	ulIP = ping.ResolveIP(sIpAdd.GetBuffer());
	delay = ping.PingHostOK(ulIP, PING_TIMEOUT);


	if (delay > 2000)
	{
		g_pView->m_pPLCSocketClient.Close();//소켓을 종료한다.
		AddStringToStatus("PLC Connection error!");
		//	AfxMessageBox("PLC Connection error!");
		m_bPLCCom = FALSE;

		return m_bPLCCom;
	}

#endif
	if (m_pPLCSocketClient.m_SerConnected == 1)
	{
		m_pPLCSocketClient.Close();
		l_bPLCCom = FALSE;
	}
	

	m_pPLCSocketClient.m_SerVerDeviceID = PLC_SOCKET_ID;
	m_pPLCSocketClient.Create();										//클라이언트 소켓 생성
	connt = m_pPLCSocketClient.Connect(sIpAdd, nPort);					//서버에 연결을 시도 한다.

	if (connt) {												//오류 발생시
		//AddStringToStatus("PLC Connection OK.");
		l_bPLCCom = TRUE;
		m_bPLC_WaitCount = 0;
		//m_bPLC_RegWaitCount = 0;
		m_bPLC_Online = 1;
		m_pPLCSocketClient.m_SerConnected = 1;

		AddPLCLog("PLC Connection OK.");
		MessageDisplay(0, "PLC Connection OK!");

	}
	else {
		m_pPLCSocketClient.Close();//소켓을 종료한다.
		//AddStringToStatus("PLC Connection error!");
		//	AfxMessageBox("PLC Connection error!");
		l_bPLCCom = FALSE;
		m_pPLCSocketClient.m_SerConnected = 0;
		AddPLCLog("PLC Connection error!");
		MessageDisplay(1, "PLC Connection error!");
	}
	return l_bPLCCom;
}
#else
	BOOL lresult = 0;
	//SERVER
	if (m_sListenSocket[lIndex].m_SocketOpen != 0)
	{
		m_sListenSocket[lIndex].Close();
	}
	lresult = m_sListenSocket[lIndex].Create(g_nLocalPortNumber[lIndex]);
	lresult = m_sListenSocket[lIndex].Listen();
	m_sListenSocket[lIndex].m_SocketID = lIndex;
	//if(lresult == 1)
	m_sListenSocket[lIndex].m_SocketOpen = 1;
	return TRUE;
#endif
//}

BOOL CAcqVoltageSamples_IntClkDlg::CreateEthernetSocket() 
{
	BOOL lresult = 0;


	for(int i = 0; i< 6; i++)
	{
		CreateEthernetSocketSvr(i) ;
	}

	CreateEthernetSocketPLC(g_strPLCIPAddress ,	g_nPLCPortNumber);
	

	return TRUE;
}


BOOL CAcqVoltageSamples_IntClkDlg::ConnectSocket() 
{
	BOOL lresult = 0;

		
	

	return TRUE;
}

//UINT g_nRemotePortNumber = 8000;
//UINT g_nLocalPortNumber = 7000;
//CString g_nRemoteIPAddress = _T("192.168.1.20");

void CAcqVoltageSamples_IntClkDlg::CloseEthernetScocketSvr(int lIndex) 
{

}


BOOL CAcqVoltageSamples_IntClkDlg::OnCmdGmesSendSvr(int lIndex) 
{

	return TRUE;
}


BOOL CAcqVoltageSamples_IntClkDlg::OnCmdGmesSend(int lIndex)
{

	if (!m_pUDPSocketServer[lIndex])//if(m_pUDPSocketServer[lIndex].m_SocketOpen == 0)
	{
		//	m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		AfxMessageBox(" Please create socket first.");
		//	m_EthernetMode =  ETHERNET_MODE_IDLE;
			//MessageBox("GMES Socket Create Fail!");
		gTestSeqData.bMesEnable = 0;

		return FALSE;
	}
	if (m_strMesSend.GetLength() == 0)
	{
		MessageDisplay(1, " Data Strings ERROR to Send");
		//gTestSeqData.bMesEnable = 0;
		//m_EthernetMode =  ETHERNET_MODE_IDLE;
		return FALSE;
	}

	//if (g_nRemoteIPAddress.GetLength() == 0)
	//{
	//	MessageDisplay(1, "Please Type the remote IPaddress to Send to");
	//	m_EthernetMode =  ETHERNET_MODE_IDLE;
	//	gTestSeqData.bMesEnable = 0;
	//	return FALSE;
	//}


	if (m_pUDPSocketServer[lIndex].m_SocketOpen == 0)
	{
		//	m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		MessageDisplay(1, " Socket Not Connected!!");
		m_EthernetMode = ETHERNET_MODE_IDLE;
		gTestSeqData.bMesEnable = 0;
		AfxMessageBox(" Socket Not Connected.");
		CloseEthernetScocketSvr(lIndex);
		return FALSE;
	}

	int iLen = m_strMesSend.GetLength();
	int iSent = m_pUDPSocketServer[lIndex].Send(LPCSTR(m_strMesSend), iLen);

	// Were we able to send it?
	if (iSent == SOCKET_ERROR)
	{
		CString str;
		str.Format(_T("Port:%d Send Error"), lIndex);
		MessageBox(str);

	}


	AddGmesLog(m_strMesSend);
	return TRUE;
}

int CAcqVoltageSamples_IntClkDlg::OnCmdPLCSend(int lIndex, const void* lpBuf, int nBufLen)
{

	if (!m_pPLCSocketClient)//if(m_pUDPSocketServer[lIndex].m_SocketOpen == 0)
	{
		AddPLCLog(" Please create socket first.");
		//	m_EthernetMode =  ETHERNET_MODE_IDLE;
		//MessageBox("GMES Socket Create Fail!");
		gTestSeqData.bMesEnable = 0;
		return FALSE;
	}

	if (m_pPLCSocketClient.m_SerConnected == 0)
	{
		//	m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		MessageDisplay(1, " Socket Not Connected!!");
		m_EthernetMode = ETHERNET_MODE_IDLE;
		gTestSeqData.bMesEnable = 0;
		//AfxMessageBox(" Socket Not Connected.");
		AddPLCLog(" Socket Not Connected!!");
		CloseEthernetScocketSvr(lIndex);
		return FALSE;
	}
	
	int iSent = m_pPLCSocketClient.Send(lpBuf, nBufLen);

	// Were we able to send it?
	if (iSent == SOCKET_ERROR)
	{
		CString str;
		str.Format(_T("Port:%d Send Error"), lIndex);
		AddPLCLog(str);
//	MessageBox(str);
	}

	//AddGmesLog(m_strMesSend);
	return iSent;
}


void CAcqVoltageSamples_IntClkDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);      // Device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CAcqVoltageSamples_IntClkDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CAcqVoltageSamples_IntClkDlg::OnBnClickedStart()
{


}





void CAcqVoltageSamples_IntClkDlg::OnCancel()
{
	gbShutdown = true;
	Sleep(200);
    ShowWindow(SW_HIDE);
  

//	g_MesDb.Close();

	if(mpDlgResult != NULL)
		delete mpDlgResult;


	//if(mpDlgSimpleInform != NULL)
	//	delete mpDlgSimpleInform;

    CDialog::OnCancel();
}

/*

0 : HDMI_ANA_AUD_OUT_R
1 : HDMI_ANA_AUD_OUT_L
2 : DIG_2_ANA_AUD_R
3 : DIG_2_ANA_AUD_L
4 : 7.1_AUDIO_OUT_R1
5 : 7.1_AUDIO_OUT_L1
6 : 7.1_AUDIO_OUT_R2
7 : 7.1_AUDIO_OUT_L2
8 : 7.1_AUDIO_OUT_R3
9 : 7.1_AUDIO_OUT_L3
10 : 7.1_AUDIO_OUT_R4
11 : 7.1_AUDIO_OUT_L4
12 : ANA_AUDIO_OUT_R
13 : ANA_AUDIO_OUT_L
14 : P_ANA_AUDIO_OUT_R
15 : P_ANA_AUDIO_OUT_L
16 : P_ANA_AUDIO_OUT_R
17 : P_ANA_AUDIO_OUT_L

*/

//
//void CAcqVoltageSamples_IntClkDlg::ReadChannel()
//{
//   if(m_InitAiChannel == 0)
//   {
//	   return;
//   }
//
//	if(m_CheckEnable[lch] == 0)
//		return;
//
//
//    try
//    {
//		// Create multi-channel reader
//		m_reader[lch] = std::auto_ptr<CNiDAQmxAnalogMultiChannelReader>(new CNiDAQmxAnalogMultiChannelReader(m_task[lch]->Stream));
//
//		// Start asynchronous read
//		m_reader[lch]->ReadMultiSample(samplesPerChannel, m_data[lch]);
//
//    }
//    catch (CException* e)
//    {
//        e->ReportError();
//        e->Delete();
//		m_InitAiChannel = 0;
//     //   m_startButton.EnableWindow(true);
//     }
//}




//
//
//BOOL CAcqVoltageSamples_IntClkDlg::SetD_OUT(ULONG lSetVal)
//{
//	if(m_InitAiChannel == 0)
//	{
//		return 0;
//	}
//
//	if(m_DOportOpen == 0)
//		return 0;
//
//
//	m_DO_Writedata = lSetVal;
//
//	 try
//    {
//        // Create the task
//        CNiDAQmxTask digitalWriteTask;
//
//        //  Create an Digital Output channel and name it.
//        digitalWriteTask.DOChannels.CreateChannel(
//            "Dev1/port1",
//            "",
//            DAQmxOneChannelForAllLines);
//
//        //  Write digital port data. WriteDigitalSingChanSingSampPort writes a single sample
//        //  of digital data on demand, so no timeout is necessary.
//        CNiDAQmxDigitalSingleChannelWriter writer(digitalWriteTask.Stream);
//        
//        //CNiString(m_dataToWrite) >> dataToWrite;
//        writer.WriteSingleSamplePort(true, lSetVal);
//    }
//    catch (CNiDAQmxException *e)
//    {
//        e->ReportError();
//        e->Delete();
//		m_DOportOpen = 0;
//
//		return 0;
//    }
//
//	return 1;
//
//}
//
//
//BOOL CAcqVoltageSamples_IntClkDlg::ReadD_IN(unsigned long *lReadData)
//{
//	if(m_InitAiChannel == 0)
//	{
//		return 0;
//	}
//
//	if(	m_DIportOpen == 0)
//		return 0;
//
//
//	try
//	{       
//		CNiDAQmxTask digitalReadTask;
//        //Create the reader
//				
//		digitalReadTask.DIChannels.CreateChannel(
//		    "Dev1/port0",
//		    "",
//		    DAQmxOneChannelForAllLines);
//		CNiDAQmxDigitalSingleChannelReader reader(digitalReadTask.Stream);
//
//        // Read the data
//		//unsigned long data = reader.ReadSingleSamplePortUInt32();
//		*lReadData = reader.ReadSingleSamplePortUInt32();
//
//        // Display the data
//        //m_hexData.Format(_T("0x%X"),data);//0x3F		
//		//UpdateData(FALSE);		
//	}
//	catch(CNiDAQmxException *exception)
//	{
//		exception->ReportError();
//		exception->Delete();
//			m_DIportOpen = 0;
//		return 0;
//	}	
//
//	return 1;
//}
//
////#define TEMP_CHANGE_PORT__
//
//void CAcqVoltageSamples_IntClkDlg::CheckControlInput()
//{
//	unsigned long lReadData;
//	int lStart = 0;
//	int lSensor = 0;
//	int lTouch = 0;
//	int lReserved = 0;
//	static int sOnCnt[4] = {0,0,0,0};
//	static int sOffCnt[4] = {0,0,0,0};
//	static int sOffFlag[4] = {0,0,0,0};
//	static int sPressCnt[4] = {0,0,0,0};
//
//	static int sPressWarning = 0;
//
//	CAcqVoltageSamples_IntClkDlg::ReadD_IN(&lReadData);
//
//	if((lReadData & 0x01) == 1)
//	{		
//		if(sOnCnt[0] > 1)
//		{
//			if(sOffFlag[0])
//			{
//				//SendPreIR_Code(gTestSeqData.TestProcess.strPre_Key);
//				sOffFlag[0] = 0; 
//				//m_PrecodeWait = 5;
//				
//				m_nPreCurrentStep = 0;	
//				m_nPreCurrentProcess = 0;
//				m_nPreCurrentNumber = 0;
//				if(gTestSeqData.PreTestCnt > 0)
//				{
//					m_PreTaskRun = TASK_RUN_START;
//					MessageDisplay(2, "Preprocess Start");
//				}
//			}
//		}
//		else
//		{
//			sOnCnt[0]++;
//		}
//		
//		sOffCnt[0] = 0;
//		
//	}
//	else
//	{
//		if(sOffCnt[0] > 5)
//		{
//			sOffFlag[0] = 1; 
//		}
//		else
//		{
//			sOffFlag[0] = 0; 
//			sOffCnt[0]++;
//		}
//
//		sOnCnt[0] = 0;
//	}
//
//#ifdef TEMP_CHANGE_PORT__
//	if((lReadData & 0x08) == 0)
//#else
//	if((lReadData & 0x02) == 0)
//#endif
//	
//	{		
//		if(sOnCnt[1] > 1)
//		{
//			if(sOffFlag[1])
//			{
//				if(m_TaskRun == TASK_WAIT_SELECT_PROCESS)
//				{
//
//					m_NG_Retry_Count++;
//					//if(m_NG_WaitFlag
//					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
//					m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);
//					//m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
//					m_nCurrentProcess = 0;
//					m_nCurrentStep = 0;
//					m_NG_WaitFlag = 1;
//
//					//return;
//
//					m_TaskRun = TASK_RUN_PROCESS;
//					CLOSE_Result_Display();
//						
//					m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//					m_LED_COM_PROCESS = COM_MODE_IDLE;
//					m_PCB_COM_PROCESS = COM_MODE_IDLE;
//
//			
//
//
//
//				}
//				else
//				{
//					CLOSE_Result_Display();
//					OnBnClickedStart();
//				}
//				sOffFlag[1] = 0; 
//			}
//		}
//		else
//		{
//
//			
//			sOnCnt[1]++;
//		}
//		sOffCnt[1] = 0;		
//		if(m_TaskRun == TASK_RUN_IDLE)
//		{
//			if((sPressCnt[1] > 400)&&((sPressCnt[1] % 500) == 99))
//			{
//				MessageDisplay(1, "Start Button Pressed Long Time!");
//				sPressWarning = 1;
//			}
//			if(gbManualScopeMode != 1)
//			{
//				sPressCnt[1]++;
//			}
//		}
//		
//	}
//	else
//	{
//		if(sOffCnt[1] > 5)
//		{
//			sOffFlag[1] = 1; 
//		}
//		else
//		{
//			sOffFlag[1] = 0; 
//			sOffCnt[1]++;
//		}
//
//		sOnCnt[1] = 0;
//
//		if(sPressWarning)
//		{
//			MessageDisplay(2, "Start Button Released!");
//			sPressWarning = 0;
//		}
//	}
//	
//
//	if((lReadData & 0x04) == 0)
//	{			
//		if(sOnCnt[2] > 1)
//		{
//			if(sOffFlag[2])
//			{				
//				
//				if(m_TaskRun == TASK_WAIT_SELECT_PROCESS)
//				{
//					if(m_GMES_CommError == 1)
//					{
//						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
//						m_NG_WaitFlag = 0;
//						TestFinishGMES();//TestFinish();
//					}
//					else
//					{
//						m_NG_Retry_Count++;
//
//						//if(m_NG_WaitFlag
//						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
//						m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);
//						//m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
//						m_nCurrentProcess = 0;
//						m_nCurrentStep = 0;
//						//m_NG_WaitFlag = 1;
//
//						//return;
//
//						m_TaskRun = TASK_RUN_PROCESS;
//						CLOSE_Result_Display();
//						m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//						m_LED_COM_PROCESS = COM_MODE_IDLE;
//						m_PCB_COM_PROCESS = COM_MODE_IDLE;//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
//
//					}
//
//
//				}
//			}
//		}
//
//		else
//		{
//			sOnCnt[2]++;
//		}
//
//		sOffCnt[2] = 0;
//		
//	}
//	else
//	{
//		if(sOffCnt[2] > 5)
//		{
//			sOffFlag[2] = 1; 
//		}
//		else
//		{
//			sOffFlag[2] = 0; 
//			sOffCnt[2]++;
//		}
//		sOnCnt[2] = 0;
//	}
//
//
//	if((lReadData & 0x08) == 0)
//	{			
//		if(sOnCnt[3] > 1)
//		{
//			if(sOffFlag[3])
//			{				
//				
//				if(m_TaskRun != TASK_RUN_IDLE)
//				{
//					if(m_TaskRun == TASK_WAIT_SELECT_PROCESS)
//					{
//				
//						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
//						m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, _T("NG"), NULL, NULL, NULL, NULL);
//				
//						////m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
//						//m_nCurrentProcess = 0;
//						//m_nCurrentStep = 0;
//						////m_NG_WaitFlag = 1;
//
//						////return;
//
//						//m_TaskRun = TASK_RUN_PROCESS;
//						//CLOSE_Result_Display();
//						m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//						m_LED_COM_PROCESS = COM_MODE_IDLE;
//						m_PCB_COM_PROCESS = COM_MODE_IDLE;//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
//			
//						m_NG_WaitFlag = 0;
//						TestFinishGMES();//TestFinish();
//					}
//					else if(m_TaskRun == TASK_WAIT_BARCODE_PROCESS)
//					{
//					//	m_TaskRun = TASK_RUN_IDLE;
//						CLOSE_Result_Display();
//						m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//						m_LED_COM_PROCESS = COM_MODE_IDLE;
//						m_PCB_COM_PROCESS = COM_MODE_IDLE;
//
//						m_TaskRun = TASK_RUN_IDLE;
//						m_cLbOkNg.put_Caption(_T("STOP"));//m_BarCode
//						m_cLbOkNg.put_BackColor(0x0000FFFF);
//					}
//					else
//					{
//						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
//						m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, _T("NG"), NULL, NULL, NULL, NULL);
//				
//						////m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
//						//m_nCurrentProcess = 0;
//						//m_nCurrentStep = 0;
//						////m_NG_WaitFlag = 1;
//
//						////return;
//
//						//m_TaskRun = TASK_RUN_PROCESS;
//						//CLOSE_Result_Display();
//						m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//						m_LED_COM_PROCESS = COM_MODE_IDLE;
//						m_PCB_COM_PROCESS = COM_MODE_IDLE;//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
//						//m_EthernetMode = 
//
//						m_NG_WaitFlag = 0;
//						//TestFinishGMES();//
//						TestFinish();
//					}
//						
//
//					mpDlgBarcode->ShowWindow(SW_HIDE);
//
//
//				}
//			}
//		}
//
//		else
//		{
//			sOnCnt[3]++;
//		}
//
//		sOffCnt[3] = 0;
//		
//	}
//	else
//	{
//		if(sOffCnt[3] > 5)
//		{
//			sOffFlag[3] = 1; 
//		}
//		else
//		{
//			sOffFlag[3] = 0; 
//			sOffCnt[3]++;
//		}
//		sOnCnt[3] = 0;
//	}
//	/*
//
//	unsigned long lCycle_Time = GetTimeCheck(5);
//	str.Format(_T("Cycle Time: %.2f"), lCycle_Time/1000.0);
//	
//	SetDlgItemText(IDC_STATIC_TOTALTIME,str);
//
//	*/
//
//
//}
//
//



void CAcqVoltageSamples_IntClkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//double m_avg_PPVoltage[6][3];
	//double m_avg_Freg[6][3];

	CString str;
//	unsigned long lCheck_Time;
	static unsigned int sCount = 0;
	static unsigned int sAlramCount = 0;
	int lAlarmDetect = 0;

	if(nIDEvent == 1)
	{	
		KillTimer(1);
		for(int i = 0; i < 6; i++)
		{
			m_GMES_Alive_WaitCnt[i]++;
			if(m_GMES_Alive_WaitCnt[i] > 300)
			{
				m_GMES_Alive_WaitCnt[i] = 0;
				gSaveInfo[i].strSystemStatus = 3;
				m_DFT_Online[i] = 0;
			}
		}

		sAlramCount++;
		if(sAlramCount > 10000)
		{
			sAlramCount = 0;
		}

		if((sAlramCount % 5) == 0)
		{
			for(int i = 0; i < 6; i++)
			{

				if(m_DFT_Online[i] == 0)//if(gSaveInfo[i].strSystemStatus == 3)
				{	
					if(m_cLbEquip[i].get_BackColor() != 0x00707070)
					{
						m_cLbEquip[i].put_BackColor( 0x00707070);
					}	
					
					if(m_cLbEquip[i].get_ForeColor() != 0x00505050)
					{
						m_cLbEquip[i].put_ForeColor( 0x00505050);
					}

					if (m_SelectedMonitoringEquipID == i)
					{
						if (m_cLbEquipSelect.get_BackColor() != 0x00707070)
						{
							m_cLbEquipSelect.put_BackColor(0x00707070);
						}

						if (m_cLbEquipSelect.get_ForeColor() != 0x00505050)
						{
							m_cLbEquipSelect.put_ForeColor(0x00505050);
						}
					}
				}
				else
				{

					if(m_cLbEquip[i].get_ForeColor() != 0x000FFFF)
					{
						m_cLbEquip[i].put_ForeColor( 0x000FFFF);
					}

					if (m_cLbEquip[i].get_BackColor() != 0x00000000)
					{
						m_cLbEquip[i].put_BackColor(0x00000000);
					}

					if (m_SelectedMonitoringEquipID == i)
					{
						
						if (m_cLbEquipSelect.get_ForeColor() != 0x000FFFF)
						{
							m_cLbEquipSelect.put_ForeColor(0x000FFFF);
						}

						if (m_cLbEquipSelect.get_BackColor() != 0x00000000)
						{
							m_cLbEquipSelect.put_BackColor(0x00000000);
						}
					}
					
	/*				if(gSaveInfo[i].strSystemStatus == 0)
					{
						if(m_AlarmNumberOn[i] == 1)
						{
							if(m_cLbEquip[i].get_BackColor() == 0x00000000)
							{
								m_cLbEquip[i].put_BackColor( 0x000000FF);
							}
							else
							{
								m_cLbEquip[i].put_BackColor( 0x00000000);
							}
							lAlarmDetect = 1;
						}
						else
						{
							if(m_cLbEquip[i].get_BackColor() != 0x000000FF)
							{
								m_cLbEquip[i].put_BackColor( 0x000000FF);
							}
						}
					}
					else
					{
						if(gSaveInfo[i].strRunStatus.Find("RUN") >= 0)
						{
							if(m_cLbEquip[i].get_BackColor() != 0x00000000)
							{
								m_cLbEquip[i].put_BackColor( 0x00000000);
							}
						}
						else
						{
							
							if(m_cLbEquip[i].get_BackColor() != 0x00707070)
							{
								m_cLbEquip[i].put_BackColor( 0x00707070);
							}
						}
					}*/
				}
				if (m_PCB_IN_Error[i] == 1)
				{
					if ((sAlramCount % 10) == 0)
					{

						if (m_cLabelProcessStatus[i].get_BackColor() == 0x00000000)
						{
							m_cLabelProcessStatus[i].put_BackColor(0x000000FF);

							m_cLabelProcessStatus[i].put_Caption("PCB CHECK ERROR");
						}
						else
						{
							m_cLabelProcessStatus[i].put_BackColor(0x00000000);
						}
					}

				}
				else
				{
					if (m_cLabelProcessStatus[i].get_BackColor() != 0x404040)
					{
						m_cLabelProcessStatus[i].put_BackColor(0x404040);
					}
				}
			}
			//if(((sAlramCount % 100) == 0)&&(lAlarmDetect == 1))
			//{
			//	//Beep(500, 100);
			//	//Beep(500, 100);
			////	sAlramCount += 4;
			//	CString m_NgWaveFile  = _T(PATH_BASE_DIR);
			//	m_NgWaveFile  += _T("\\Sound\\OK_CHECK.wav");//NG_CHECK.wav");//OK_CHECK.wav

			////	sndPlaySound( m_NgWaveFile , SND_ASYNC);

			//}

	/*		if((sAlramCount % (10)) == 0)
			{
				m_cListCtrl_OQA_Monitor[(sAlramCount / 10)%6].Invalidate();
			}*/

			
			
		}


		SetTimer(1,50,NULL);

	}
	else if(nIDEvent == 2)
	{
		KillTimer(2);

		sCount++;

		GMESLinkTestResponse();
		
	
		SetTimer(2,10000,NULL);
	}
	else if (nIDEvent == 3)
	{


	
		ModelSelectDlg lDlg;
		KillTimer(3);
		//m_DFT_CommandCheck = __MODE_MODEL_SELECT;
		switch (m_DFT_CommandCheck)
		{
		case __COMMAND_CHECKMODE_IDLE:
		
			break;
		case __COMMAND_CHECKMODE_SELECT:
			
			if (lDlg.DoModal() == IDOK)
			{
				str = "#/m_szSeqFilePath//";
				str += lDlg.m_szSeqFilePath;
				str += "#/m_szModelInfoFilePath//";
				str += lDlg.m_szModelInfoFilePath;
				str += "#/m_szRefImgFolder//";
				str += lDlg.m_szRefImgFolder;

				str += "#/m_sSelChassis//";
				str += lDlg.m_sSelChassis;
				str += "#/m_sSelModel//";
				str += lDlg.m_sSelModel;
				str += "#/END";


				//for (int i = 0; i < 6; i++)
				//{
				//	CommandDFT_ModelChange(i, str); //CommandDFT_ModelChange(m_SelectedMonitoringEquipID, str);
				//}
				if (m_SelectDFT == DFT_SELECT_123)
				{
					for (int i = 0; i < 3; i++)
					{
						CommandDFT_ModelChange(i, str);
					}
				}
				else if (m_SelectDFT == DFT_SELECT_456)
				{
					for (int i = 4; i < 6; i++)
					{
						CommandDFT_ModelChange(i, str);
					}
				}
				else
				{
					for (int i = 0; i < 6; i++)
					{
						CommandDFT_ModelChange(i, str);
					}
				}
			}

			m_DFT_CommandCheck = __COMMAND_CHECKMODE_IDLE;
			break;

		default:
			m_DFT_CommandCheck = __COMMAND_CHECKMODE_IDLE;
			break;
		}

		SetTimer(3, 500, NULL);
	}	
	else if (nIDEvent == 4)
	{
		if (m_BtnClearCnt > 0)
		{
			m_BtnClearCnt--;
		}
		else
		{
			if (m_cLB_BtnChangeModel.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_BtnChangeModel.put_BackColor(0xA0A0A0);
			}

			if (m_cLB_BtnGetToolOptionClose.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_BtnGetToolOptionClose.put_BackColor(0xA0A0A0);
			}

			if (m_cLB_BtnGetToolOption.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_BtnGetToolOption.put_BackColor(0xA0A0A0);
			}

			if (m_cLB_BtnSelectDFT.get_BackColor() != 0x00FF00)
			{
				m_cLB_BtnSelectDFT.put_BackColor(0x00FF00);
			}

			if (m_cLB_BtnStartAll.get_BackColor() != 0x00F0F0)
			{
				m_cLB_BtnStartAll.put_BackColor(0x00F0F0);
			}
			if (m_cLB_BtnPauseAll.get_BackColor() != 0x00F0F0)
			{
				m_cLB_BtnPauseAll.put_BackColor(0x00F0F0);
			}
			if (m_cLB_BtnStopAll.get_BackColor() != 0x00F0F0)
			{
				m_cLB_BtnStopAll.put_BackColor(0x00F0F0);
			}

			if (m_cLB_BtnStartSelect.get_BackColor() != 0x00F0F0)
			{
				m_cLB_BtnStartSelect.put_BackColor(0x00F0F0);
			}

			if (m_cLB_BtnPauseSelect.get_BackColor() != 0x00F0F0)
			{
				m_cLB_BtnPauseSelect.put_BackColor(0x00F0F0);
			}

			if (m_cLB_BtnStopSelect.get_BackColor() != 0x00F0F0)
			{
				m_cLB_BtnStopSelect.put_BackColor(0x00F0F0);
			}

			for (int i = 0; i < 6; i++)
			{

				if (m_cLabelBtnStart[i].get_BackColor() != 0x00F0F0)
				{
					m_cLabelBtnStart[i].put_BackColor(0x00F0F0);
				}

				if (m_cLabelBtnPause[i].get_BackColor() != 0x00F0F0)
				{
					m_cLabelBtnPause[i].put_BackColor(0x00F0F0);
				}

				if (m_cLabelBtnStop[i].get_BackColor() != 0x00F0F0)
				{
					m_cLabelBtnStop[i].put_BackColor(0x00F0F0);
				}
			}

	
			if (m_cLB_PLC_Ctrl_JigUp.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_JigUp.put_BackColor(0xA0A0A0);
			}
			if (m_cLB_PLC_Ctrl_JigDown.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_JigDown.put_BackColor(0xA0A0A0);
			}
			if (m_cLB_PLC_Ctrl_Load.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_Load.put_BackColor(0xA0A0A0);
			}
			if (m_cLB_PLC_Ctrl_TestPass.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_TestPass.put_BackColor(0xA0A0A0);
			}
			if (m_cLB_PLC_Ctrl_TestNG.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_TestNG.put_BackColor(0xA0A0A0);
			}
			if (m_cLB_PLC_Ctrl_Clear.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_Clear.put_BackColor(0xA0A0A0);
			}
			if (m_cLB_PLC_Ctrl_Read.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_Read.put_BackColor(0xA0A0A0);
			}


			SetTimer(4, 1000, NULL);
		}		
	}
	else if (nIDEvent == 5)
	{
		KillTimer(5);
		PLC_AlarmCheck();
		m_bPLC_WaitCount++;
		if  (m_bPLC_Online != 1)
		{
			if (m_cLabelComScan.get_BackColor() != 0x0000FF)
			{
				m_cLabelComScan.put_BackColor(0x0000FF);
			}
		}
		else if(m_bPLC_WaitCount > 2)
		{
			if (m_cLabelComScan.get_BackColor() != 0x808080)
			{
				m_cLabelComScan.put_BackColor(0x808080);
			}
		}
		else
		{
			if (m_cLabelComScan.get_BackColor() != 0x00FF00)
			{
				m_cLabelComScan.put_BackColor(0x00FF00);
			}
		}
	
		SetTimer(5, 1000, NULL);
	}


	CDialog::OnTimer(nIDEvent);
}




void CAcqVoltageSamples_IntClkDlg::OnBnClickedSetupButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedTestButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

}

void CAcqVoltageSamples_IntClkDlg::MessageDisplay(int lError, CString sMessage)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_cLbServerLabel.put_Caption(sMessage);

	if(lError == 0)
	{
		m_cLbServerLabel.put_ForeColor(0x00FF0000);
	}
	else if(lError == 1)
	{
		m_cLbServerLabel.put_ForeColor(0x000000FF);
	}
	else if(lError == 2)
	{
		m_cLbServerLabel.put_ForeColor(0x0000FFFF);
	}

}

void CAcqVoltageSamples_IntClkDlg::GMESLinkTestResponse() //S2F4
{
//	<EIFVERSION="1.8"ID="S2F4"NAME="LinkTestResponse">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//</EIF>
	//EquipmentStatusSend
	//CString str1;
	for(int  i = 0; i < 6; i++)
	{
		if((m_pUDPSocketServer[i]) && (m_pUDPSocketServer[i].m_SocketOpen == 1))
		{
			m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F3\" NAME=\"Link Test Request\">\r\n");
			m_strMesSend += _T("  <ELEMENT>\r\n");
			m_strMesSend += _T("    <EQPID>");
			m_strMesSend += gSaveInfo[i].strEQPID;
			m_strMesSend += _T("</EQPID>\r\n");
			m_strMesSend += _T("  </ELEMENT>\r\n");	
			m_strMesSend += _T("</EIF>\r\n");	
	
			OnCmdGmesSend(i);
		}
	}
}


void CAcqVoltageSamples_IntClkDlg::GMES_TestCtrl(int  lEQP_Index,  CString   lstrCommand) //S2F103
{
//	<EIFVERSION="1.8"ID="S2F4"NAME="LinkTestResponse">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//</EIF>

	//EquipmentStatusSend
	CString str1;

		if((m_pUDPSocketServer[lEQP_Index]) && (m_pUDPSocketServer[lEQP_Index].m_SocketOpen == 1))
		{
			m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F103\" NAME=\"COMMAND CONTROL\">\r\n");

			m_strMesSend += _T("  <ELEMENT>\r\n");
			m_strMesSend += _T("    <EQPID>");
			m_strMesSend += gSaveInfo[lEQP_Index].strEQPID;
			m_strMesSend += _T("</EQPID>\r\n");
					   
			m_strMesSend += _T("    <COMMAND>");
			//str1 = "RESTART";
			m_strMesSend += lstrCommand;
			m_strMesSend += _T("</COMMAND>\r\n");

		/*	m_strMesSend += _T("    <COMMAND DATA>");			
			m_strMesSend += lstrData;
			m_strMesSend += _T("</COMMAND DATA>\r\n");*/


			m_strMesSend += _T("  </ELEMENT>\r\n");	
			m_strMesSend += _T("</EIF>\r\n");	
	
			OnCmdGmesSend(lEQP_Index);
		}
	
}


void CAcqVoltageSamples_IntClkDlg::GMES_TestCtrl(int  lEQP_Index, CString   lstrCommand, CString   lstrData) //S2F103
{
	//	<EIFVERSION="1.8"ID="S2F4"NAME="LinkTestResponse">
	//<ELEMENT>
	//<EQPID></EQPID>
	//</ELEMENT>
	//</EIF>

		//EquipmentStatusSend
	CString str1;

	if ((m_pUDPSocketServer[lEQP_Index]) && (m_pUDPSocketServer[lEQP_Index].m_SocketOpen == 1))
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F103\" NAME=\"COMMAND CONTROL\">\r\n");

		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <EQPID>");
		m_strMesSend += gSaveInfo[lEQP_Index].strEQPID;
		m_strMesSend += _T("</EQPID>\r\n");

		m_strMesSend += _T("    <COMMAND>");
		//str1 = "RESTART";
		m_strMesSend += lstrCommand;
		m_strMesSend += _T("</COMMAND>\r\n");

		m_strMesSend += _T("    <COMMAND DATA>");
		m_strMesSend += lstrData;
		m_strMesSend += _T("</COMMAND DATA>\r\n");


		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		OnCmdGmesSend(lEQP_Index);
	}

}


//long CAcqVoltageSamples_IntClkDlg::OnReceiveDataEthernet(WPARAM wParam, LPARAM lParam )
//{
//
//
//	BYTE lbuf[4096];
//	int lcnt;
//	lcnt = (int)wParam;
//
//	return 0;
//}
//

long CAcqVoltageSamples_IntClkDlg::OnParseDataEthernet(int lEQP_Index )//m_strEthernetReceive
{
//	BYTE lbuf[4096];
	int lcnt;
	int lpos;
	int lendpos;
	CString l_Receivestr;
	CString l_ReceivestrReserved;
	CString str1;
	CString str_read1;
	CString str2;
	bool lReadFlag = 0;
//	int lEQP_Index = 0;

	if((lEQP_Index >= 0)&&(lEQP_Index < 12))
	{
	}
	else
	{
		return 1;
	}
	lcnt = m_strEthernetReceive[lEQP_Index].GetLength();

	lpos = m_strEthernetReceive[lEQP_Index].Find("<EIF");
	if(lpos < 0)
	{
		m_strEthernetReceive[lEQP_Index] = "";
	}
	else
	{
		lendpos = m_strEthernetReceive[lEQP_Index].Find("</EIF>");
		
	}

	//if ((m_strEthernetReceive[lEQP_Index].Find("<EIF") >= 0) && (m_strEthernetReceive[lEQP_Index].Find("</EIF>") >= 0))
		
	while ((m_strEthernetReceive[lEQP_Index].Find("<EIF") >= 0) && (m_strEthernetReceive[lEQP_Index].Find("</EIF>") >= 0))
	{
		lpos = m_strEthernetReceive[lEQP_Index].Find("<EIF");
		if(lpos > 0)
		{
			m_strEthernetReceive[lEQP_Index] = m_strEthernetReceive[lEQP_Index].Mid(lpos);
		}
		m_GMES_NGCNT = 0;
		AddGmesLog(m_strEthernetReceive[lEQP_Index], 1);

		m_GMES_Alive_WaitCnt[lEQP_Index] = 0;
		m_DFT_Online[lEQP_Index] = 1;
//		m_cLb_ComMainGmes.put_BackColor(0x0000FF00);
		//l_Receivestr = m_strEthernetReceive;
		lendpos = m_strEthernetReceive[lEQP_Index].Find("</EIF>");
		l_Receivestr = m_strEthernetReceive[lEQP_Index].Left(lendpos+6);
		int len1 = l_Receivestr.GetLength();
		l_ReceivestrReserved = m_strEthernetReceive[lEQP_Index].Mid(lendpos+6);
		int len2 = l_ReceivestrReserved.GetLength();
		if (l_ReceivestrReserved.Find("<EIF") >= 0)
		{
			m_strEthernetReceive[lEQP_Index] = l_ReceivestrReserved;
		}
		else
		{
			m_strEthernetReceive[lEQP_Index] = "";
		}
	//	m_strEthernetReceive[lEQP_Index] = "";		

		SaveLogDataT lSaveReceivedData;

		if (l_Receivestr.Find("\"S2F101\"") >= 0)
		{
			lReadFlag = 1;

			lpos = l_Receivestr.Find("<EQPID>");	//</EQPID>	
			str1 = l_Receivestr.Mid(lpos + strlen("<EQPID>"));
			lpos = str1.Find("</EQPID>");	//</EQPID>	
			str1 = str1.Left(lpos);
			lSaveReceivedData.strEQPID = str1;//g_strEQPID



		//	ctrlWipIdEdit.GetWindowText(str1);
	/*		m_strMesSend += _T("    <SETID>");
			m_strMesSend += str1;
			m_strMesSend += _T("</SETID>\r\n");*/

			lpos = l_Receivestr.Find("<SETID>");	//</EQPID>	
			str1 = l_Receivestr.Mid(lpos + strlen("<SETID>"));
			lpos = str1.Find("</SETID>");	//</EQPID>	
			str1 = str1.Left(lpos);
			lSaveReceivedData.strWipID_SetSN = str1;//g_strEQPID

			str_read1 = m_cLabelPCBASN[lEQP_Index].get_Caption();
			if (str1.Compare(str_read1) != 0)
			{
				m_cLabelPCBASN[lEQP_Index].put_Caption(str1);
			}


			lpos = l_Receivestr.Find("<MODEL NAME>");	//</EQPID>	
			str1 = l_Receivestr.Mid(lpos + strlen("<MODEL NAME>"));
			lpos = str1.Find("</MODEL NAME>");	//</EQPID>	
			str1 = str1.Left(lpos);
			lSaveReceivedData.strModelName = str1;//g_strEQPID

			if (m_SelectedMonitoringEquipID == lEQP_Index)
			{
				str_read1 = m_cLB_ModelName.get_Caption();
				if (str1.Compare(str_read1) != 0)
				{
					m_cLB_ModelName.put_Caption(str1);
				}
			}

			if (m_ScanEnable != 1)
			{
				lpos = l_Receivestr.Find("<MODEL SUFFIX>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos + strlen("<MODEL SUFFIX>"));
				lpos = str1.Find("</MODEL SUFFIX>");	//</EQPID>	
				str1 = str1.Left(lpos);
				lSaveReceivedData.strModelSuffix = str1;//g_strEQPID

				if (m_SelectedMonitoringEquipID == lEQP_Index)
				{
					str_read1 = m_cLB_Suffix.get_Caption();
					if (str1.Compare(str_read1) != 0)//if (str_read1.GetLength() < 5)					
					{
						//m_cLB_Suffix.put_Caption(str1);
					}
				}
			}

				//#define _AUTO_ROBOT_CTRL_IDLE			0
				//#define _AUTO_ROBOT_CTRL_INIT			1
				//#define _AUTO_ROBOT_CTRL_JIGUP			2
				//#define _AUTO_ROBOT_CTRL_LOAD			3
				//#define _AUTO_ROBOT_CTRL_JIG_DOWN		4
				//#define _AUTO_ROBOT_CTRL_TEST_RUN		5
				//#define _AUTO_ROBOT_CTRL_JIGFINISH_UP	6
				//#define _AUTO_ROBOT_CTRL_UNLOAD			7
				//#define _AUTO_ROBOT_CTRL_TEST_OK		8
				//#define _AUTO_ROBOT_CTRL_TEST_NG		9
				//#define _AUTO_ROBOT_CTRL_PAUSE			10
				//#define _AUTO_ROBOT_CTRL_OFFLINE		11
				//#define _AUTO_ROBOT_CTRL_NOACK			12
				//#define _AUTO_ROBOT_CTRL_MANUAL			13
				//#define _AUTO_ROBOT_CTRL_ALARM			14
				//#define _AUTO_ROBOT_CTRL_PCB_IN_ERROR	15
				//#define _AUTO_ROBOT_CTRL_TIMEOUT		16
				//#define _AUTO_ROBOT_CTRL_FFC_RETRY		17
				//#define _AUTO_ROBOT_CTRL_UPDOWN_RETRY	18
				//#define _AUTO_ROBOT_CTRL_NG_3_STOP		98
				//#define _AUTO_ROBOT_CTRL_ERROR			99
				//#define _AUTO_ROBOT_CTRL_PLC_ON			100

				//m_strMesSend += _T("    <ROBOT MAIN MODE>");
				//str1.Format(_T("%d"), g_AutoControlProcess);
				//m_strMesSend += str1;
				//m_strMesSend += _T("</ROBOT MAIN MODE>\r\n");

				lpos = l_Receivestr.Find("<ROBOT MAIN MODE>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos + strlen("<ROBOT MAIN MODE>"));
				lpos = str1.Find("</ROBOT MAIN MODE>");	//</EQPID>	
				str1 = str1.Left(lpos);
				lSaveReceivedData.strRobotMainMode = str1;//g_strEQPID							

				lpos = l_Receivestr.Find("<ROBOT CTRL>");	//	
				str1 = l_Receivestr.Mid(lpos + strlen("<ROBOT CTRL>"));
				lpos = str1.Find("</ROBOT CTRL>");	//
				str1 = str1.Left(lpos);
				SetRobotStatusDisplay(lEQP_Index, str1);

				lpos = l_Receivestr.Find("<PLC DATA>");	//	
				str1 = l_Receivestr.Mid(lpos + strlen("<PLC DATA>"));
				lpos = str1.Find("</PLC DATA>");	//
				str1 = str1.Left(lpos);
				SetPLC_DataDisplay(lEQP_Index, str1);		

				lpos = l_Receivestr.Find("<SEQ LIST>");	//	
				str1 = l_Receivestr.Mid(lpos + strlen("<SEQ LIST>"));
				lpos = str1.Find("</SEQ LIST>");	//
				str1 = str1.Left(lpos);
				SetSeq_DataDisplay(lEQP_Index, str1);


				m_strMesSend += _T("  </ELEMENT>\r\n");
				m_strMesSend += _T("</EIF>\r\n");


///////////////////////////////////////////////

				m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F102\" NAME=\"Inspection(Processing) Data Ack\">\r\n");
				m_strMesSend += _T("    <EQPID>");
				m_strMesSend += lSaveReceivedData.strEQPID;
				m_strMesSend += _T("</EQPID>\r\n");

				m_strMesSend += _T("    <ACK>");
				m_strMesSend +=  _T("0");
				m_strMesSend += _T("</ACK>\r\n");
				m_strMesSend += _T("    <REASON>");
				m_strMesSend +=  _T("0");
				m_strMesSend += _T("</REASON>\r\n");
				m_strMesSend += _T("</EIF>");
							
				OnCmdGmesSend( lEQP_Index) ;
				
		}
		else if (l_Receivestr.Find("\"S2F105\"") >= 0)
		{
			lpos = l_Receivestr.Find("<TEST RUN PROCESS MODE>");	//	
			str1 = l_Receivestr.Mid(lpos + strlen("<TEST RUN PROCESS MODE>"));
			lpos = str1.Find("</TEST RUN PROCESS MODE>");	//
			str1 = str1.Left(lpos);

			lpos = str1.Find("#Command:");
			str1 = str1.Mid(lpos + strlen("#Command:"));
			lpos = str1.Find("//");
			str2 = str1.Left(lpos);

			if (str2.Find("ADD") >= 0)
			{
				str2 = str1.Mid(lpos + 2);
				int nIndex = m_cListMainProcessLog.AddString(str2);
				m_cListMainProcessLog.SetCurSel(nIndex);
			}
			else if(str2.Find("CLEAR") >= 0)
			{			
				m_cListMainProcessLog.ResetContent();
			}

	/*	
		m_strMesSend += _T("    <TEST RUN PROCESS MODE>");
		str1 = "#Command:";
		str1 += gPLC_Ctrl.m_sRunProcessMsgCommand;
		str1 += "//";
		m_strMesSend += gPLC_Ctrl.m_sRunProcessMsg;
		m_strMesSend += _T("</TEST RUN PROCESS MODE>\r\n");
	*/
		}
		else if (l_Receivestr.Find("\"S2F107\"") >= 0)
		{

			POSITION	Pos = NULL;
			//CModelData pModelData;
			
		//	CString StrAry[7];

			CString PosString[7] = {
				"#/m_szChassisName//",
				"#/m_szModelName//",
				"#/m_szSeqFilePath//",
				"#/m_szModelInfoFilePath//",
				"#/m_szRefImgFolder//",
				"#/m_szDivision//",
				"#/m_szSeqFilePath2//" };

			lpos = l_Receivestr.Find("<MODEL COUNT>");	//	
			str1 = l_Receivestr.Mid(lpos + strlen("<MODEL COUNT>"));
			lpos = str1.Find("</MODEL COUNT>");	//
			str1 = str1.Left(lpos);

			m_ModelListCount = _ttoi(str1);


			
			lpos = l_Receivestr.Find("<MODEL LIST DATA>");	//	
			str1 = l_Receivestr.Mid(lpos + strlen("<MODEL LIST DATA>"));
			lpos = str1.Find("</MODEL LIST DATA>");	//
			str1 = str1.Left(lpos);

			ModelList.RemoveAll();
			for (int i = 0; i < m_ModelListCount; i++)
			{
				CModelData* pModel = new CModelData;
				for (int j = 0; j < 6; j++)
				{
					lpos = str1.Find(PosString[j]);
					str2 = str1.Mid(lpos + strlen(PosString[j]));
					str1 = str2;
					lpos = str2.Find("#/");
					str2 = str2.Left(lpos);
				//	StrAry[j] = str2;
					switch (j)
					{
					case 0:
						pModel->m_szChassisName = str2;
						break;
					case 1:
						pModel->m_szModelName = str2;
						break;
					case 2:
						pModel->m_szSeqFilePath = str2;
						break;
					case 3:
						pModel->m_szModelInfoFilePath = str2;
						break;
					case 4:
						pModel->m_szRefImgFolder = str2;
						break;
					case 5:
						pModel->m_szDivision = str2;
						break;
					case 6:
						pModel->m_szSeqFilePath2 = str2;
						break;
					default:
						break;

					}
				}
				ModelList.AddTail(pModel);
			}

			lpos = str1.Find("#/g_Divisions//");			
			g_Divisions.RemoveAll();
			while (lpos > 0)
			{				
				str2 = str1.Mid(lpos + strlen("#/g_Divisions//"));
				lpos = str2.Find("#/");
				str1 = str2;
				str2 = str2.Left(lpos);
				g_Divisions.Add(str2);
				lpos = str1.Find("#/g_Divisions//");
			}
		
			m_DFT_CommandCheck = __COMMAND_CHECKMODE_SELECT;
		}
		else if (l_Receivestr.Find("\"S2F111\"") >= 0)
		{
			lpos = l_Receivestr.Find("<RESULT>");	//	
			str1 = l_Receivestr.Mid(lpos + strlen("<RESULT>"));
			lpos = str1.Find("</RESULT>");	//
			str1 = str1.Left(lpos);
			if (str1.Find("OK") >= 0)
			{
				OK_Display(lEQP_Index);
				g_nOKCnt++; 
				CFileControl lFile;
				lFile.SaveCountData();
				DisplayCountData();
			}
			else
			{
				NG_Display(lEQP_Index);
				g_nNGCnt++;
				CFileControl lFile;
				lFile.SaveCountData();
				DisplayCountData();
			}
		}
		else if (l_Receivestr.Find("\"S2F113\"") >= 0)
		{
			lpos = l_Receivestr.Find("<PCB_CHECK>");	//	
			str1 = l_Receivestr.Mid(lpos + strlen("<PCB_CHECK>"));
			lpos = str1.Find("</PCB_CHECK>");	//
			str1 = str1.Left(lpos);
			if (str1.Find("CLEAR") >= 0)
			{
				m_PCB_IN_Error[lEQP_Index] = 0;
			}
			else
			{
				m_PCB_IN_Error[lEQP_Index] = 1;
			}

			//m_strMesSend += _T("  <ELEMENT>\r\n");
			//m_strMesSend += _T("    <PCB_CHECK>");

			//str1 = "CLEAR";

			//m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
			//m_strMesSend += _T("</PCB_CHECK>\r\n");
		}
		else if (l_Receivestr.Find("\"S1F1\"") >= 0)
		{

			lpos = l_Receivestr.Find("<EQPID>");	//</EQPID>	
			str1 = l_Receivestr.Mid(lpos+7);
			lpos = str1.Find("</EQPID>");	//</EQPID>	
			str1 = str1.Left(lpos);
			lSaveReceivedData.strEQPID = str1;//g_strEQPID

			
		

			m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S1F2\" NAME=\"Are You There Data Send \">\r\n");
			m_strMesSend += _T("    <EQPID>");
			m_strMesSend += lSaveReceivedData.strEQPID;
			m_strMesSend += _T("</EQPID>\r\n");
			m_strMesSend += _T("</EIF>");

			OnCmdGmesSend( lEQP_Index) ;		

		}
		else if (l_Receivestr.Find("\"S2F4\"") >= 0)
		{

		;

		}
		
	}
	return 0;
}

void CAcqVoltageSamples_IntClkDlg::SetRobotStatusDisplay(int lEQP_index, CString RcvStr)
{
	CString str1, str2, str3;
	long lColor = 0;
	int lpos;
	CLabel_text *m_pcLB_Display[12];


	CString str_head[12];

	str_head[0] = "#PLC_COM_Status//";
	str_head[1] = "#PCB_Status//";


	//m_strMesSend += _T("    <ROBOT CTRL>");
	//str2 = "#PLC_COM_Status//";
	////COLORREF
	//str1.Format(_T("%d//"), ctrlPLC_COM_Status.GetTextColor());
	//str2 += str1;
	////COLORREF
	//str1.Format(_T("%d//"), ctrlPLC_COM_Status.GetBkColor());
	//str2 += str1;
	//ctrlPLC_COM_Status.GetWindowText(str1);
	//str2 += str1;
	//m_strMesSend += str2;

	//str2 = "#PCB_Status//";
	////COLORREF
	//str1.Format(_T("%d//"), ctrlROBOT_Status.GetTextColor());
	//str2 += str1;
	////COLORREF
	//str1.Format(_T("%d//"), ctrlROBOT_Status.GetBkColor());
	//str2 += str1;
	//ctrlROBOT_Status.GetWindowText(str1);
	//str2 += str1;
	//m_strMesSend += str2;
	//m_strMesSend += _T("</ROBOT CTRL>\r\n");

	//CLabel_text m_cLabelCtrlStatus[6];//IDC_LABEL_PLC_ROBOT_STATUS_DFT1
	//CLabel_text m_cLabelPcbStatus[6];//IDC_LABEL_PCB_IN_STATUS_DFT1
	m_pcLB_Display[0] = &m_cLabelCtrlStatus[lEQP_index];
	m_pcLB_Display[1] = &m_cLabelPcbStatus[lEQP_index];
	//m_pcLB_Display[2] = &m_cLb_ComMainMonitor;

	str1 = RcvStr;////str1 = gSaveInfo[lEQP_index].strComStatus;

	for (int j = 0; j < 2; j++)
	{

		lpos = str1.Find(str_head[j]);
		if (lpos < 0)
			break;

		str1 = str1.Mid(lpos + str_head[j].GetLength());
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);
		str3 = str2.Left(8);
		lColor = _ttoi(str3);
		if (m_pcLB_Display[j]->get_ForeColor() != lColor)
		{
			m_pcLB_Display[j]->put_ForeColor(lColor);
		}

		lpos = str1.Find("//");
		str1 = str1.Mid(lpos + 2);
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);
		str3 = str2.Left(8);
		lColor = _ttoi(str3);
		if (m_pcLB_Display[j]->get_BackColor() != lColor)
		{
			m_pcLB_Display[j]->put_BackColor(lColor);
		}

		lpos = str1.Find("//");
		str1 = str1.Mid(lpos + 2);
		lpos = str1.Find("#");

		if (lpos < 1)
		{
			str2 = str1;
			str1 = "";
		}
		else
		{
			str2 = str1.Left(lpos);
			str1 = str1.Mid(lpos);
		}

		str3 = m_pcLB_Display[j]->get_Caption();

		if (str2.Compare(str3) != 0)
		{
			m_pcLB_Display[j]->put_Caption(str2);
		}

	}
}

void CAcqVoltageSamples_IntClkDlg::SetPLC_DataDisplay(int lEQP_index, CString RcvStr)
{
	CString str1, str2, str3;
	long lColor = 0;
	int lpos;
	int *m_pcLB_Display[12];


	CString str_head[12];


	str_head[0] = "#Auto//";
	str_head[1] = "#Manual//";
	str_head[2] = "#Start//";
	str_head[3] = "#Pause//";
	str_head[4] = "#Stop//";
	str_head[5] = "#Warn//";
	str_head[6] = "#Alarm//";
	str_head[7] = "#SkipFFC//";
	str_head[8] = "#PCBIn//";
	str_head[9] = "#JigUp//";
	str_head[10] = "#JigDown//";
	str_head[11] = "#ACOnOff//";



	
	str1 = RcvStr;//gSaveInfo[lEQP_index].strComStatus;
	
	m_pcLB_Display[0] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Auto_Mode;
	m_pcLB_Display[1] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Manual_Mode;
	m_pcLB_Display[2] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Start_Run;
	m_pcLB_Display[3] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Pause;
	m_pcLB_Display[4] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_STOP;
	m_pcLB_Display[5] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Warn;
	m_pcLB_Display[6] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Alarm;
	m_pcLB_Display[7] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckSkipFFC;
	m_pcLB_Display[8] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Data_PCB_Inside;
	m_pcLB_Display[9] = &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckJigUpStatus;
	m_pcLB_Display[10] =  &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckJigDownStatus;//:0
	m_pcLB_Display[11] =  &gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckACPowerStatus;//:0



	for (int j = 0; j < 12; j++)
	{

		lpos = str1.Find(str_head[j]);
		if (lpos < 0)
			break;

		str1 = str1.Mid(lpos + str_head[j].GetLength());
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);		
		str1 = str1.Mid(lpos + 2); 

		int lOn = _ttoi(str2);
		*m_pcLB_Display[j] = lOn;		
	}

	
	

	if (lEQP_index == m_SelectedMonitoringEquipID)
	{
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Auto_Mode == 1)
		{
			if (m_cLB_PLC_Data_Auto.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_Auto.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_Auto.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_Auto.put_BackColor(0x606060);
			}
		}
		
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Manual_Mode == 1)
		{
			if (m_cLB_PLC_Data_Manual.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_Manual.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_Manual.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_Manual.put_BackColor(0x606060);
			}
		}
		
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Start_Run == 1)
		{
			if (m_cLB_PLC_Data_Start.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_Start.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_Start.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_Start.put_BackColor(0x606060);
			}
		}
		
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Pause == 1)
		{
			if (m_cLB_PLC_Data_Pause.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_Pause.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_Pause.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_Pause.put_BackColor(0x606060);
			}
		}	
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_STOP == 1)
		{
			if (m_cLB_PLC_Data_Stop.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_Stop.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_Stop.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_Stop.put_BackColor(0x606060);
			}
		}
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Warn == 1)
		{
			if (m_cLB_PLC_Data_Warn.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_Warn.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_Warn.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_Warn.put_BackColor(0x606060);
			}
		}
	
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Alarm == 1)
		{
			if (m_cLB_PLC_Data_Alarm.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_Alarm.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_Alarm.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_Alarm.put_BackColor(0x606060);
			}
		}		

		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckSkipFFC == 1)
		{
			if (m_cLB_PLC_Data_SkipFFC.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_SkipFFC.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_SkipFFC.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_SkipFFC.put_BackColor(0x606060);
			}
		}




		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheck_PLC_Data_PCB_Inside == 1)
		{
			if (m_cLB_PLC_Data_PcbSensor.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_PcbSensor.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_PcbSensor.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_PcbSensor.put_BackColor(0x606060);
			}
		}	
		



		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckJigUpStatus == 1)
		{
			if (m_cLB_PLC_Data_JigUp.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_JigUp.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_JigUp.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_JigUp.put_BackColor(0x606060);
			}
		}	
		
	
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckJigDownStatus == 1)
		{
			if (m_cLB_PLC_Data_JigDown.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Data_JigDown.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Data_JigDown.get_BackColor() != 0x606060)
			{
				m_cLB_PLC_Data_JigDown.put_BackColor(0x606060);
			}
		}
//
		if (gSaveInfo[lEQP_index].m_PLC_Data.m_cCheckACPowerStatus == 1)
		{
			if (m_cLB_PLC_Ctrl_AC_ONOFF.get_BackColor() != 0x00F0F0)
			{
				m_cLB_PLC_Ctrl_AC_ONOFF.put_BackColor(0x00F0F0);
			}
		}
		else
		{
			if (m_cLB_PLC_Ctrl_AC_ONOFF.get_BackColor() != 0xA0A0A0)
			{
				m_cLB_PLC_Ctrl_AC_ONOFF.put_BackColor(0xA0A0A0);
			}
		}
	}
}


void CAcqVoltageSamples_IntClkDlg::SetSeq_DataDisplay(int lEQP_index, CString RcvStr)
{
	CString str1, str2, str3;
	ULONG lColor = 0;
	int lpos;
	int lCheck;

	int m_pcLB_DisplayText[12];	
	CString str_head;

	
	str1 = RcvStr;
	
	for (int i = 0; i < 100; i++)
	{
		str_head.Format(_T("#%d//"), i);
		lpos = str1.Find(str_head);
		gSaveInfo[lEQP_index].m_SeqList[i].nEnable = 0;
		
		if (lpos < 0)
		{
			gSaveInfo[lEQP_index].SeqStepCount = i;
			break;
		}
		gSaveInfo[lEQP_index].m_SeqList[i].nEnable = 1;

/////////////////////////////////////Check
		str1 = str1.Mid(lpos + str_head.GetLength());
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);
		lCheck = _ttoi(str2);
		gSaveInfo[lEQP_index].m_SeqList[i].nCheck = lCheck;
		str1 = str1.Mid(lpos + 2);

//////////////////////////////////////Step No
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);
		gSaveInfo[lEQP_index].m_SeqList[i].strStepNo = str2;
		str1 = str1.Mid(lpos + 2);	

//////////////////////////////////////////Step Name
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);
		gSaveInfo[lEQP_index].m_SeqList[i].strStepName = str2;
		str1 = str1.Mid(lpos + 2);		


//////////////////////////////////////////Step Result
		
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);

		gSaveInfo[lEQP_index].m_SeqList[i].strResult = str2;
		str1 = str1.Mid(lpos + 2);


//////////////////////////////////////////Step Back Color
		
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);

		lColor = _ttoi(str2);
		gSaveInfo[lEQP_index].m_SeqList[i].nBackColor = lColor;

		str1 = str1.Mid(lpos + 2);		
		
//////////////////////////////////////////Step Fore Color	
		
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);
		lColor = _ttoi(str2);
		gSaveInfo[lEQP_index].m_SeqList[i].nForeColor = lColor;

		str1 = str1.Mid(lpos + 2);	
		
	}

	CString strTmp;
	strTmp = "#SEL//";
	lpos = str1.Find(strTmp);
	str1 = str1.Mid(lpos + strTmp.GetLength());
	lpos = str1.Find("//");
	str2 = str1.Left(lpos);
	
	gSaveInfo[lEQP_index].nSeqSel =   _ttoi(str2) - 1;
	

	//CListCtrl m_cListCtrl_OQA_Monitor[6];
	//CListCtrl m_cListCtrl_OQA_Monitor_Select;
	//CListCtrl m_cListCtrlMainLog;
	if (gSaveInfo[lEQP_index].SeqStepCount != m_cListCtrl_OQA_Monitor[lEQP_index].GetItemCount())
	{

		m_cListCtrl_OQA_Monitor[lEQP_index].DeleteAllItems();

		if (lEQP_index == m_SelectedMonitoringEquipID)
			m_cListCtrl_OQA_Monitor_Select.DeleteAllItems();
		for (int i = 0; i < gSaveInfo[lEQP_index].SeqStepCount; i++)
		{
			m_cListCtrl_OQA_Monitor[lEQP_index].InsertItem(i, "");
			
			if (lEQP_index == m_SelectedMonitoringEquipID)
				m_cListCtrl_OQA_Monitor_Select.InsertItem(i, "");

			gLoadDataInfo[lEQP_index].m_SeqList[i].nCheck = 0;
			gLoadDataInfo[lEQP_index].m_SeqList[i].strStepNo = "";
			gLoadDataInfo[lEQP_index].m_SeqList[i].strStepName = "";
			gLoadDataInfo[lEQP_index].m_SeqList[i].strResult = "";
			gLoadDataInfo[lEQP_index].m_SeqList[i].nForeColor = 0;
			gLoadDataInfo[lEQP_index].m_SeqList[i].nBackColor = 0xFFFFFF;
					   
		}

	}


	for (int i = 0; i < gSaveInfo[lEQP_index].SeqStepCount; i++)
	{
		if (gSaveInfo[lEQP_index].m_SeqList[i].nEnable == 1)
		{
			if ((gSaveInfo[lEQP_index].m_SeqList[i].nCheck != m_cListCtrl_OQA_Monitor[lEQP_index].GetCheck(i))
				||(gSaveInfo[lEQP_index].m_SeqList[i].nCheck != m_cListCtrl_OQA_Monitor_Select.GetCheck(i)))
			{
				m_cListCtrl_OQA_Monitor[lEQP_index].SetCheck(i, gSaveInfo[lEQP_index].m_SeqList[i].nCheck);
				if(lEQP_index == m_SelectedMonitoringEquipID)
					m_cListCtrl_OQA_Monitor_Select.SetCheck(i, gSaveInfo[lEQP_index].m_SeqList[i].nCheck);
			}		
			
			if (gSaveInfo[lEQP_index].m_SeqList[i].strStepNo != gLoadDataInfo[lEQP_index].m_SeqList[i].strStepNo)
			{
				m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 0, LVIF_TEXT, 
					gSaveInfo[lEQP_index].m_SeqList[i].strStepNo, NULL, NULL, NULL, NULL);
				if (lEQP_index == m_SelectedMonitoringEquipID)
					m_cListCtrl_OQA_Monitor_Select.SetItem(i, 0, LVIF_TEXT,
						gSaveInfo[lEQP_index].m_SeqList[i].strStepNo, NULL, NULL, NULL, NULL);

			}			
			
			if (gSaveInfo[lEQP_index].m_SeqList[i].strStepName != gLoadDataInfo[lEQP_index].m_SeqList[i].strStepName)
			{
				m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 1, LVIF_TEXT,
					gSaveInfo[lEQP_index].m_SeqList[i].strStepName, NULL, NULL, NULL, NULL);
				if (lEQP_index == m_SelectedMonitoringEquipID)
					m_cListCtrl_OQA_Monitor_Select.SetItem(i, 1, LVIF_TEXT,
						gSaveInfo[lEQP_index].m_SeqList[i].strStepName, NULL, NULL, NULL, NULL);


			}		
			
			if (gSaveInfo[lEQP_index].m_SeqList[i].strResult != gLoadDataInfo[lEQP_index].m_SeqList[i].strResult)
			{
				m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 2, LVIF_TEXT,
					gSaveInfo[lEQP_index].m_SeqList[i].strResult, NULL, NULL, NULL, NULL);
				if (lEQP_index == m_SelectedMonitoringEquipID)
					m_cListCtrl_OQA_Monitor_Select.SetItem(i, 2, LVIF_TEXT,
						gSaveInfo[lEQP_index].m_SeqList[i].strResult, NULL, NULL, NULL, NULL);


			}	
			if (gSaveInfo[lEQP_index].nSeqSel != gLoadDataInfo[lEQP_index].nSeqSel)
			{
				gLoadDataInfo[lEQP_index].nSeqSel = gSaveInfo[lEQP_index].nSeqSel;
				m_cListCtrl_OQA_Monitor[lEQP_index].SetSelectionMark(gLoadDataInfo[lEQP_index].nSeqSel);
				//if (gLoadDataInfo[lEQP_index].nSeqSel > 8)
				{
				//	m_cListCtrl_OQA_Monitor[lEQP_index].Scroll((gLoadDataInfo[lEQP_index].nSeqSel-8)*10);
					SetScroll(lEQP_index, (gLoadDataInfo[lEQP_index].nSeqSel - 8));
				}
				//if (lEQP_index == m_SelectedMonitoringEquipID)
				//{
				//	m_cListCtrl_OQA_Monitor_Select.SetSelectionMark(gLoadDataInfo[lEQP_index].nSeqSel);
				//	if (gLoadDataInfo[lEQP_index].nSeqSel > 22)
				//	{
				////		m_cListCtrl_OQA_Monitor_Select.Scroll((gLoadDataInfo[lEQP_index].nSeqSel - 8) * 10);
				//		SetScroll(lEQP_index, (gLoadDataInfo[lEQP_index].nSeqSel - 22) * 10);
				//	}
				//}
				
				
			}
		}
		else
		{
			if (gLoadDataInfo[lEQP_index].m_SeqList[i].nEnable == 1)
			{
				m_cListCtrl_OQA_Monitor[lEQP_index].SetCheck(i, 0);


				m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 0, LVIF_TEXT,
					"", NULL, NULL, NULL, NULL);
				m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 1, LVIF_TEXT,
					"", NULL, NULL, NULL, NULL);

				m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 2, LVIF_TEXT,
					"", NULL, NULL, NULL, NULL);

				if (lEQP_index == m_SelectedMonitoringEquipID)
				{
					m_cListCtrl_OQA_Monitor[lEQP_index].SetCheck(i, 0);


					m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 0, LVIF_TEXT,
						"", NULL, NULL, NULL, NULL);
					m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 1, LVIF_TEXT,
						"", NULL, NULL, NULL, NULL);

					m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(i, 2, LVIF_TEXT,
						"", NULL, NULL, NULL, NULL);
				}

			}
		}
		gLoadDataInfo[lEQP_index].m_SeqList[i] = gSaveInfo[lEQP_index].m_SeqList[i];
	}
		

}


void CAcqVoltageSamples_IntClkDlg::OnDestroy()
{
	CDialog::OnDestroy();
	gbShutdown = true;
	Sleep(200);

	ShowWindow(SW_HIDE);

		

	//CloseEthernetScocket();
	for(int i = 0 ;i < 6; i++)
	{
		m_pUDPSocketServer[i].Close();
		//if (m_pUDPSocketServer[i].m)
		//{
		//	m_pUDPSocketServer[i]->Close();
		//	delete m_pUDPSocketServer[i];

		//	m_pUDPSocketServer[i]  = NULL;

		//}	
	}
		   

	if(mpDlgResult != NULL)
		delete mpDlgResult;

	
	//if(mpDlgSimpleInform != NULL)
	//	delete mpDlgSimpleInform;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gbShutdown = true;

	Sleep(200);


	//Sleep(10);
	//if( vcap.isOpened() )
	//{

	//	vcap.release();
	//	
	//}


	CDialog::OnOK();
}


void CAcqVoltageSamples_IntClkDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonDown(nFlags, point);
}


void CAcqVoltageSamples_IntClkDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static int sId = 0;
	sId++;
	if (sId > 15)
		sId = 0;

	m_cListCtrl_OQA_Monitor[0].SetSelectionMark(sId);
	//SetScroll(0, sId * 10);
	CDialog::OnLButtonUp(nFlags, point);
}


void CAcqVoltageSamples_IntClkDlg::SetScroll(int  lIndex, int lScroll)
{
	int lyPos;
	int lDiffScroll;


	CSize lSize;
	
	//NG_Display(sId); //OK_Display(sId);
	lyPos = m_cListCtrl_OQA_Monitor[0].GetScrollPos(1);
	lDiffScroll = (lScroll - lyPos)*10;
	lSize.SetSize(0, lDiffScroll);
	m_cListCtrl_OQA_Monitor[0].Scroll(lSize);

	lyPos = m_cListCtrl_OQA_Monitor_Select.GetScrollPos(1);

	lDiffScroll = (lScroll - lyPos-14) * 10;
	lSize.SetSize(0, lDiffScroll);
	m_cListCtrl_OQA_Monitor_Select.Scroll(lSize);
}

void CAcqVoltageSamples_IntClkDlg::SetTimeCheck(int  lIndex)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	//MessageDisplay(0, _T("Model Mouse Down!!"));

	/*time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);		
	char sDate[32];
	sprintf(sDate, "%4d/%02d/%02d %02dh %02dm %02ds",
		t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	static CTime sOldTime;

	CTime m_CurrentTime;
	

	m_CurrentTime = m_CurrentTime.GetCurrentTime();*/

	
	if((mstartT_1 == 0) || (lIndex == 1))
	{
		mstartT_1 = GetTickCount();

	}

	if((mstartT_2 == 0) || (lIndex == 2))
	{
		mstartT_2 = GetTickCount();

	}	
	
	if((mstartT_3 == 0) || (lIndex == 3))
	{
		mstartT_3 = GetTickCount();

	}	
	
	if((mstartT_4 == 0) || (lIndex == 4))
	{
		mstartT_4 = GetTickCount();

	}
	
	if((mstartT_5 == 0) || (lIndex == 5))
	{
		mstartT_5 = GetTickCount();

	}	
	
	if((mstartT_6 == 0) || (lIndex == 6))
	{
		mstartT_6 = GetTickCount();

	}
	
}


unsigned long  CAcqVoltageSamples_IntClkDlg::GetTimeCheck(int  lIndex)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//MessageDisplay(0, _T("Model Mouse Up!!"));

	/*	static unsigned long mstartT_1 = 0;
	static unsigned long mstartT_2 = 0;
	static unsigned long mstartT_3 = 0;*/

	unsigned long lCheckTime = GetTickCount();
	unsigned long lCheckDelayTime = 0;

	if(lIndex == 1)
	{
		lCheckDelayTime = lCheckTime - mstartT_1;

	}
	else if(lIndex == 2)
	{
		lCheckDelayTime = lCheckTime - mstartT_2 ;

	}
	else if(lIndex == 3)
	{
		lCheckDelayTime = lCheckTime - mstartT_3;

	}
	else if(lIndex == 4)
	{
		lCheckDelayTime = lCheckTime - mstartT_4;

	}
	else if(lIndex == 5)
	{
		lCheckDelayTime = lCheckTime - mstartT_5;

	}	
	else if(lIndex == 6)
	{
		lCheckDelayTime = lCheckTime - mstartT_6;

	}

	return lCheckDelayTime;

}


void  CAcqVoltageSamples_IntClkDlg::DisplayTimeLap()
{
	CString str;
//	unsigned long lCheck_Time ;
	//if(CheckTimeOn)
	//{
	//	lCheck_Time = GetTimeCheck(4);
	//	str.Format(_T("%.1f (s)"), lCheck_Time/1000.0);
	//	m_CheckTimeLabel.put_Caption(str);
	//}


	//if(CycleTimeOn)
	//{
	//	lCheck_Time = GetTimeCheck(5);
	//	str.Format(_T("%.1f (s)"), lCheck_Time/1000.0);
	//	m_CycleTimeLabel.put_Caption(str);

	//	if(lCheck_Time > 100000)
	//	{
	//		CycleTimeOn = 0;
	//	}
	//}
}


void CAcqVoltageSamples_IntClkDlg::LoadTestData(CString lstrModelname)
{

	//lstrModelname  = _T("TEST");
	CFileControl lFile;
	lFile.OpenModelSeq(lstrModelname);

	
}

void CAcqVoltageSamples_IntClkDlg::DisplayTestData()
{
	//SetSubListControl(SaveLogDataT lSaveReceivedData);
	
}


//
//void CAcqVoltageSamples_IntClkDlg::SetListControl()
//{
//	/*
//	
//	m_CtrlListMainProcess.InsertItem(0, lSaveReceivedData.strWipID_SetSN);
//	m_CtrlListMainProcess.SetItem(0,  1, LVIF_TEXT, lSaveReceivedData.strTestResult, NULL, NULL, NULL, NULL);
//	m_CtrlListMainProcess.SetItem(0,  2, LVIF_TEXT, lSaveReceivedData.strEQPID, NULL, NULL, NULL, NULL);
//	m_CtrlListMainProcess.SetItem(0,  3, LVIF_TEXT, lSaveReceivedData.strProcID, NULL, NULL, NULL, NULL);
//	m_CtrlListMainProcess.SetItem(0,  4, LVIF_TEXT, lSaveReceivedData.strTestDateTime, NULL, NULL, NULL, NULL);
//	m_CtrlListMainProcess.SetItem(0,  5, LVIF_TEXT, lSaveReceivedData.strTestDiscription, NULL, NULL, NULL, NULL);
//
//	*/
////	m_CtrlListTestProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
////	m_CtrlListMainProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
//	m_CtrlListMainProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
//	m_CtrlListMainProcess.InsertColumn(0, _T("SN"), LVCFMT_CENTER, 120); 
//	m_CtrlListMainProcess.InsertColumn(1, _T("RESULT"), LVCFMT_CENTER, 50);// 리스트 행 추가
//	m_CtrlListMainProcess.InsertColumn(2, _T("EQUIP"), LVCFMT_CENTER, 100);// 리스트 행 추가
//	m_CtrlListMainProcess.InsertColumn(3, _T("JIG No."), LVCFMT_CENTER, 20);// 리스트 행 추가
//	m_CtrlListMainProcess.InsertColumn(4, _T("Time"), LVCFMT_CENTER, 300);// 리스트 행 추가
//	m_CtrlListMainProcess.InsertColumn(5, _T("Discription"), LVCFMT_CENTER, 500);// 리스트 행 추가
//	//m_CtrlListTestProcess.InsertColumn(6, _T("Ref2"), LVCFMT_CENTER, 60);// 리스트 행 추가
//	//m_CtrlListTestProcess.InsertColumn(7, _T("Value"), LVCFMT_CENTER, 60);// 리스트 행 추가
//	//m_CtrlListTestProcess.InsertColumn(8, _T("OK/NG"), LVCFMT_CENTER, 60);// 리스트 행 추가
//	//m_CtrlListTestProcess.InsertColumn(9, _T("T/T"), LVCFMT_CENTER, 50);// 리스트 행 추가
//
//	//m_CtrlListMainProcess.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
//	//m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T("FM TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	//m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	////m_CtrlListTestProcess.SetItem(0, 3, LVIF_TEXT, _T("2.5"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	////m_CtrlListTestProcess.SetItem(0, 4, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	////m_CtrlListTestProcess.SetItem(0, 5, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	////m_CtrlListTestProcess.SetItem(0, 6, LVIF_TEXT, _T("80%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	////m_CtrlListTestProcess.SetItem(0, 7, LVIF_TEXT, _T("90%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	////m_CtrlListTestProcess.SetItem(0, 8, LVIF_TEXT, _T("OK"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//
//	//
//	//m_CtrlListMainProcess.InsertItem(0, _T("2"));   // 첫째행(0), 첫째열에 삽입
//	//m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T("SECOND TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	//m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//
//
//	
//	
//	font = new CFont;
//	font->CreateFontA( 16,                          // nHeight
//					0,                          // nWidth
//					0,                          // nEscapement
//					0,                          // nOrientation
//					400,                           // nWeight
//					0,                          // bItalic
//					0,                          // bUnderline 
//					0,                          // cStrikeOut 
//					0,                          // nCharSet
//					0,           // nOutPrecision 
//					0,                          // nClipPrecision 
//					PROOF_QUALITY,              // nQuality
//					0,  // nPitchAndFamily 
//					_T("Arial"));                     // lpszFacename
//
//
//	//font->CreateFontA( 20,                          // nHeight
//	//				   0,                          // nWidth
//	//				   0,                          // nEscapement
//	//				   0,                          // nOrientation
//	//				   400,                           // nWeight
//	//				   0,                          // bItalic
//	//				   0,                          // bUnderline 
//	//				   0,                          // cStrikeOut 
//	//				   0,                          // nCharSet
//	//				   0,           // nOutPrecision 
//	//				   0,                          // nClipPrecision 
//	//				 PROOF_QUALITY,              // nQuality
//	//				 0,  // nPitchAndFamily 
//	//				 _T("Arial"));                     // lpszFacename
//	//font->CreatePointFont(20,_T("Arial"));   
//	m_CtrlListMainProcess.SetFont(font);
//
//	delete font;
//	
//}

void CAcqVoltageSamples_IntClkDlg::SetOQAListControl()
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
	for(int i = 0 ; i < 6; i++)
	{		
		m_cListCtrl_OQA_Monitor[i].SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
		m_cListCtrl_OQA_Monitor[i].InsertColumn(0, _T("NO"), LVCFMT_CENTER, 40); 
		m_cListCtrl_OQA_Monitor[i].InsertColumn(1, _T("STEP"), LVCFMT_CENTER, 147);// 리스트 행 추가
		m_cListCtrl_OQA_Monitor[i].InsertColumn(2, _T("RESULT"), LVCFMT_CENTER, 80);// 리스트 행 추가


		for (int j = 0; j < 100; j++)
		{
			strTemp.Format(_T("%d"), j+1);
			m_cListCtrl_OQA_Monitor[i].InsertItem(j, strTemp);
			m_cListCtrl_OQA_Monitor[i].SetItem(j,  1, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);
			m_cListCtrl_OQA_Monitor[i].SetItem(j,  2, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);
			m_cListCtrl_OQA_Monitor[i].SetItem(j,  3, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);	
			m_cListCtrl_OQA_Monitor[i].SetItem(j,  4, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);	
		}
		m_cListCtrl_OQA_Monitor[i].SetFont(font);		
	}
//	m_cListCtrl_OQA_Monitor_Select.SetCaretPos()
	m_cListCtrl_OQA_Monitor_Select.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_cListCtrl_OQA_Monitor_Select.InsertColumn(0, _T("NO"), LVCFMT_CENTER, 40); 
	m_cListCtrl_OQA_Monitor_Select.InsertColumn(1, _T("STEP"), LVCFMT_CENTER, 224);// 리스트 행 추가
	m_cListCtrl_OQA_Monitor_Select.InsertColumn(2, _T("RESULT"), LVCFMT_CENTER, 115);// 리스트 행 추가
	for (int j = 0; j < 100; j++)
	{
		strTemp.Format(_T("%d"), j + 1);
		m_cListCtrl_OQA_Monitor_Select.InsertItem(j, strTemp);
		m_cListCtrl_OQA_Monitor_Select.SetItem(j, 1, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
		m_cListCtrl_OQA_Monitor_Select.SetItem(j, 2, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
		m_cListCtrl_OQA_Monitor_Select.SetItem(j, 3, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
		m_cListCtrl_OQA_Monitor_Select.SetItem(j, 4, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
	}
	delete font;
	font = new CFont;
	font->CreateFontA(19,                          // nHeight
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

	m_cListCtrl_OQA_Monitor_Select.SetFont(font);		



	delete font;	


	
}

//
//void CAcqVoltageSamples_IntClkDlg::SetSubListControl(int lEQP_index, SaveLogDataT lSaveReceivedData)
//{
//	CSize lSize;
//	CString strTemp;
//	
//	for(int j = 0; j < 100; j++)
//	{
//	//	strTemp.Format(_T("%d"), j+1);
//		m_cListCtrl_OQA_Monitor[lEQP_index].InsertItem(j, "");
//		m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(j,  1, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);
//		m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(j,  2, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);
//		m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(j,  3, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);	
//		m_cListCtrl_OQA_Monitor[lEQP_index].SetItem(j,  4, LVIF_TEXT,  "", NULL, NULL, NULL, NULL);	
//	}
//
//	
//
//}

void CAcqVoltageSamples_IntClkDlg::SetSubDetailDisplay(int lEQP_index)
{
	CString str1,str2,str3;	
	long lColor = 0;
	int lpos;
//	CLabel_text *m_pcLB_Display[12];


	str1.Format(_T("DFT No %d"), lEQP_index + 1);
	m_cLbEquipSelect.put_Caption(str1);

	//str1.Format( _T("SELECT RACK %d ["), lEQP_index+1);
	//str1 += m_cLbEquip[lEQP_index].get_Caption();
	//str1 += "]";
	//m_cLbServerLabel.put_Caption(str1);



	CString str_head[12];


	str_head[0]  = "#Select//";
	str_head[1]  = "#BarCode//";
	str_head[2]  = "#Cnt_FM//";
	str_head[3]  = "#Cnt_Time//";
	str_head[4]  = "#SetStatus//";
	str_head[5]  = "#AC_Power//";
	str_head[6]  = "#Lan_Status//";
	str_head[7]  = "#USB1_Status//";
	str_head[8]  = "#USB2_Status//";
	str_head[9]  = "#PowerStatus//";
	str_head[10] = "#OpenStatus//";
	str_head[11] = "#AgingTimeLabel//";
			
		
	//for(int i = 0; i < 8; i++)
	//{
	//	m_pcLB_Display[0] = &m_cLB_Select[i];	
	//	m_pcLB_Display[1] = &m_cLB_BarCode[i];
	//	m_pcLB_Display[2] = &m_cLB_Cnt_FM[i];
	//	m_pcLB_Display[3] = &m_cLB_Cnt_Time[i];
	//	m_pcLB_Display[4] = &m_cLB_SetStatus[i];
	//	m_pcLB_Display[5] = &m_cLB_AC_Power[i];
	//	m_pcLB_Display[6] = &m_cLB_Lan_Status[i];
	//	m_pcLB_Display[7] = &m_cLB_USB1_Status[i];
	//	m_pcLB_Display[8] = &m_cLB_USB2_Status[i];
	//	m_pcLB_Display[9] = &m_cLB_PowerStatus[i];
	//	m_pcLB_Display[10] = &m_cLB_OpenStatus[i];
	//	m_pcLB_Display[11] = &m_TotalAgingTimeLabel[i];
	//	str1 = gSaveInfo[lEQP_index].TestSetData[i].strTestDiscription;

	//	for(int j = 0; j < 12; j++)
	//	{
	//
	//		lpos = str1.Find(str_head[j]);
	//		if(lpos < 0)
	//			break;

	//		str1 = str1.Mid(lpos + str_head[j].GetLength());
	//		lpos = str1.Find("//");
	//		str2 = str1.Left(lpos);
	//		str3 = str2.Left(8);
	//		lColor = _ttoi(str3);

	//		if(m_pcLB_Display[j]->get_ForeColor() != lColor)
	//		{
	//			m_pcLB_Display[j]->put_ForeColor(lColor);
	//		}

	//		str3 = str2.Mid(8,8);
	//		lColor = _ttoi(str3);

	//		if(m_pcLB_Display[j]->get_BackColor() != lColor)
	//		{			
	//			m_pcLB_Display[j]->put_BackColor(lColor);
	//		}

	//		
	//		lpos = str1.Find("//");
	//		str1 = str1.Mid(lpos+2);
	//		lpos = str1.Find("#");
	//		
	//		if(lpos < 1)
	//		{
	//			str2 = str1;
	//			str1 = "";
	//		}
	//		else
	//		{
	//			str2 = str1.Left(lpos);
	//			str1 = str1.Mid(lpos);
	//		}

	//		str3 = m_pcLB_Display[j]->get_Caption();
	//		
	//		if(str2.Compare(str3) != 0)
	//		{
	//			m_pcLB_Display[j]->put_Caption(str2);
	//		}

	//	}	
	//}

	SetSubComDetailDisplay(lEQP_index);
}


void CAcqVoltageSamples_IntClkDlg::SetSubComDetailDisplay(int lEQP_index)
{
	CString str1, str2, str3;
	long lColor = 0;
	int lpos;
	CLabel_text *m_pcLB_Display[12];


	CString str_head[12];

	str_head[0] = "#ComMain//";
	str_head[1] = "#ComMainGmes//";
	str_head[2] = "#ComMainMonitor//";


	//m_pcLB_Display[0] = &m_cLb_ComMain;	
	//m_pcLB_Display[1] = &m_cLb_ComMainGmes;
	//m_pcLB_Display[2] = &m_cLb_ComMainMonitor;

	str1 = gSaveInfo[lEQP_index].strComStatus;

	for (int j = 0; j < 3; j++)
	{

		lpos = str1.Find(str_head[j]);
		if (lpos < 0)
			break;

		str1 = str1.Mid(lpos + str_head[j].GetLength());
		lpos = str1.Find("//");
		str2 = str1.Left(lpos);
		str3 = str2.Left(8);
		lColor = _ttoi(str3);
		if (m_pcLB_Display[j]->get_ForeColor() != lColor)
		{
			m_pcLB_Display[j]->put_ForeColor(lColor);
		}

		str3 = str2.Mid(8, 8);
		lColor = _ttoi(str3);
		if (m_pcLB_Display[j]->get_BackColor() != lColor)
		{
			m_pcLB_Display[j]->put_BackColor(lColor);
		}

		lpos = str1.Find("//");
		str1 = str1.Mid(lpos + 2);
		lpos = str1.Find("#");

		if (lpos < 1)
		{
			str2 = str1;
			str1 = "";
		}
		else
		{
			str2 = str1.Left(lpos);
			str1 = str1.Mid(lpos);
		}

		str3 = m_pcLB_Display[j]->get_Caption();

		if (str2.Compare(str3) != 0)
		{
			m_pcLB_Display[j]->put_Caption(str2);
		}

	}
}





//
//void CAcqVoltageSamples_IntClkDlg::SetSubListControlScan(SaveLogDataT lSaveReceivedData)
//{
//	
//	CSize lSize;
//	CString strTemp;
//
//	//m_CtrlListMainProcessScan.InsertItem(0, lSaveReceivedData.strWipID_SetSN);
//	//m_CtrlListMainProcessScan.SetItem(0,  1, LVIF_TEXT, lSaveReceivedData.strTestResult, NULL, NULL, NULL, NULL);
//	//m_CtrlListMainProcessScan.SetItem(0,  2, LVIF_TEXT, lSaveReceivedData.strEQPID, NULL, NULL, NULL, NULL);
//	//m_CtrlListMainProcessScan.SetItem(0,  3, LVIF_TEXT, lSaveReceivedData.strProcID, NULL, NULL, NULL, NULL);
//	//m_CtrlListMainProcessScan.SetItem(0,  4, LVIF_TEXT, lSaveReceivedData.strTestDateTime, NULL, NULL, NULL, NULL);
//	//m_CtrlListMainProcessScan.SetItem(0,  5, LVIF_TEXT, lSaveReceivedData.strTestDiscription, NULL, NULL, NULL, NULL);
//
//	//if(m_CtrlListMainProcess.GetItemCount() > 10)
//	//{
//	//	m_CtrlListMainProcess.DeleteItem(10);
//	//}
//
//
//}
//

//
//
//void CAcqVoltageSamples_IntClkDlg::OnHdnItemclickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//		
//	int lint  = phdr->iItem;
//	int i,lcnt;
//	if(lint == 0)
//	{
//		if(mCheckAll)
//		{
//			mCheckAll = 0;
//			lcnt = m_CtrlListMainProcess.GetItemCount();
//			for(i = 0; i < lcnt; i++)
//			{
//				m_CtrlListMainProcess.SetCheck(i, 0);
//			}
//		}
//		else
//		{
//			mCheckAll = 1;
//			lcnt = m_CtrlListMainProcess.GetItemCount();
//			for(i = 0; i < lcnt; i++)
//			{
//				m_CtrlListMainProcess.SetCheck(i, 1);
//			}
//		}
//	}
//	*pResult = 0;
//
//}
//
//
//void CAcqVoltageSamples_IntClkDlg::OnNMClickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	*pResult = 0;
//	/*
//	int i,lcnt;	
//	lcnt = m_CtrlListMainProcess.GetItemCount();
//		
//	for(i = 0; i < lcnt; i++)
//	{
//		if(gTestSeqData.TestStepList[i].bEnable == 1)
//		{
//			m_CtrlListMainProcess.SetCheck(i, 1);
//		}
//		else
//		{
//			m_CtrlListMainProcess.SetCheck(i, 0);
//		}
//	}	*/
//}
//
//
//void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
//{
////	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//		
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	*pResult = 0;
//
//	//int nRow=0;
//	//int nCol=0;
//	CString strCol=_T("");
//
//
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//
//
//
//			if(2==(int)(pLVCD->iSubItem) )
//			{
//				strCol= m_CtrlListMainProcess.GetItemText(pLVCD->nmcd.dwItemSpec,2);
//				if( strCol=="NG") // 두번째열의 값이 "10"일 때, 그 값 부분만			
//				{
//					crText = RGB(0,0,0); //글자색
//					crBkgnd = RGB(255,0,0); //배경색으로 한다    
//				}
//				else if(2==(int)(pLVCD->iSubItem) && strCol=="OK")          
//				{
//					crText = RGB(0,0,0);
//					crBkgnd = RGB(0,255,0);
//				}	
//				else
//				{
//					if((nItem == m_nCurrentNumber)&&(m_TaskRun != TASK_RUN_IDLE))//else if(nItem == m_Process_Index)          
//					{
//						crText = RGB(0,0,0);
//						crBkgnd = RGB(255,255,100);
//					}
//					else 
//					{
//						crText = RGB(0,0,0);
//						crBkgnd = RGB(255,255,255);
//					}
//				}
//		
//			}
//			else
//			{
//				//int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//				//m_TaskRun;m_Process_Index
//	//int		m_nCurrentNumber;
//				if((nItem == m_nCurrentNumber)&&(m_TaskRun != TASK_RUN_IDLE))
//				{
//					//if(pLVCD->iSubItem >= 1)
//					{
//						crText = RGB(0,0,0);
//						crBkgnd = RGB(255,255,100);
//					}
//			
//				}
//				else
//				{
//
//					crText = RGB(0,0,0);
//					crBkgnd = RGB(255,255,255);
//				}
//			}
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//}
//
//



BEGIN_EVENTSINK_MAP(CAcqVoltageSamples_IntClkDlg, CDialog)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_CAMREA_NAME, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelCamreaName, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabel, VTS_DISPATCH)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG1, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelOK1, VTS_DISPATCH)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG2, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelOK2, VTS_DISPATCH)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_COM_GMES, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelComGmes, VTS_DISPATCH)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_COM_MAIN, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelComMain, VTS_DISPATCH)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG1, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelOK1, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG2, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelOK2, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_CHECK_TIME, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelCheckTime, VTS_DISPATCH)
//	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_NG_CNT, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelNgCnt, VTS_DISPATCH)
//	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_MANAGER, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelManager, VTS_DISPATCH)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SERVER, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelServer, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_1 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip1 , VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_2 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip2 , VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_3 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip3 , VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_4 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip4 , VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_5 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip5 , VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_6 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip6 , VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_7 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip7 , VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_8 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip8 , VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_9 , DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip9 , VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_10, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip10, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_11, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip11, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EQUIP_12, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelEquip12, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT1, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect1, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT2, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect2, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT3, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect3, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT4, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect4, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT5, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect5, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT6, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect6, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT7, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect7, VTS_NONE)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_SELECT8, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelSelect8, VTS_NONE)

	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_CONFIG_SET, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelConfigSet, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_EXIT, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelExit, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_ALL, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartAll, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_ALL, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseAll, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_ALL, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopAll, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_DFT1, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft1, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_DFT2, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft2, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_DFT3, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft3, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_DFT4, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft4, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_DFT5, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft5, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_DFT6, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft6, VTS_NONE)

	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_DFT1, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft1, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_DFT2, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft2, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_DFT3, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft3, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_DFT4, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft4, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_DFT5, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft5, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_DFT6, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft6, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_DFT1, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft1, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_DFT2, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft2, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_DFT3, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft3, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_DFT4, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft4, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_DFT5, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft5, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_DFT6, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft6, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CHANGE_MODEL, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnChangeModel, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_SCAN_ON, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnScanOn, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_GET_TOOL_OPTION, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnGetToolOption, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_START_SELECT, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartSelect, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_PAUSE_SELECT, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseSelect, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_STOP_SELECT, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopSelect, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_GET_TOOL_CLOSE, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnGetToolClose, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_COUNT_SET, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelCountSet, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_COUNT_RESET, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelCountReset, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_JIGUP, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlJigup, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_JIG_DOWN, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlJigDown, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_LOAD, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlLoad, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_TEST_PASS, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlTestPass, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_TEST_NG, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlTestNg, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_CLEAR, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlClear, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_READ, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlRead, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_CTRL_AC_ON_OFF, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlAcOnOff, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BTN_SELECT_DFT, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBtnSelectDft, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_COM_MONITOR_SCAN, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelComMonitorScan, VTS_NONE)
END_EVENTSINK_MAP()




void CAcqVoltageSamples_IntClkDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//SetChannelData(0);
	//InitChannel();
	//if(m_InitAiChannel == 0)
	//{
	//	MessageBox("Audio Capture Card INIT FAIL!!");
	//	g_NIAuioBoardFail = 1;//AUDIO Board Fail
	//}
//	m_EthernetMode = ETHERNET_MODE_CREATE_SOCKET;
	ModelSelectDlg lDlg;
	//lDlg.DoModal();
	m_ModelListCount = 50;
	ModelList.RemoveAll();
	CString str2;
	for (int i = 0; i < m_ModelListCount; i++)
	{
		CModelData* pModel = new CModelData;
		for (int j = 0; j < 6; j++)
		{
			
			str2.Format("Model%d_%d", i,j);
			//	StrAry[j] = str2;
			switch (j)
			{
			case 0:
				pModel->m_szChassisName = str2;
				break;
			case 1:
				pModel->m_szModelName = str2;
				break;
			case 2:
				pModel->m_szSeqFilePath = str2;
				break;
			case 3:
				pModel->m_szModelInfoFilePath = str2;
				break;
			case 4:
				pModel->m_szRefImgFolder = str2;
				break;
			case 5:
				pModel->m_szDivision = str2;
				break;
			case 6:
				pModel->m_szSeqFilePath2 = str2;
				break;
			default:
				break;

			}
		}
		ModelList.AddTail(pModel);
	}
	if (lDlg.DoModal() == IDOK)
	{
		CString str = lDlg.m_szSeqFilePath;
	}

	CDialog::OnRButtonDblClk(nFlags, point);
}


void CAcqVoltageSamples_IntClkDlg::NG_Display(int l_ID, CString lstr)
{

	//mpDlgSimpleInform->ShowWindow(SW_HIDE);



	//mpDlgResult->m_DisplayString = m_Deisplay_ErrorName;//m_GMES_ErrorName;
	//mpDlgResult->m_DetailMessage = m_Deisplay_ErrorDetail;//m_GMES_ErrorValue;
	//CString m_NgWaveFile  = _T(PATH_BASE_DIR);
	//m_NgWaveFile  += _T("\\Sound\\NG_CHECK.wav");//OK_CHECK.wav

	//sndPlaySound( m_NgWaveFile , SND_ASYNC);
	//Beep(1200, 300);

	
	mpDlgResult->m_Result = 0;
	mpDlgResult->m_ReturnFlag = 0;
	CRect rectP;
	CRect rect;		
		
	this->GetWindowRect(&rectP);
	mpDlgResult->GetWindowRect(&rect);

	mpDlgResult->ShowWindow(SW_SHOW);
	mpDlgResult->SetWindowPos(this, rectP.left + 930 + 325 * (l_ID / 2), rectP.top + 250 + 430 * (l_ID % 2), rect.Width(), rect.Height(), SWP_ASYNCWINDOWPOS);

}

void CAcqVoltageSamples_IntClkDlg::OK_Display(int l_ID)
{

	//mpDlgSimpleInform->ShowWindow(SW_HIDE);
		//CDlgResult ldlg;
		//ldlg.m_Result = 1;
		//Beep(800, 300);
		//if(ldlg.DoModal() == IDOK)
		//{
		//}
	
		
		mpDlgResult->m_Result = 1;
		mpDlgResult->m_ReturnFlag = 0;
		CRect rectP;
		CRect rect;		
		
		this->GetWindowRect(&rectP);
		mpDlgResult->GetWindowRect(&rect);

		mpDlgResult->ShowWindow(SW_SHOW);
		mpDlgResult->SetWindowPos(this,rectP.left+ 930 + 325*(l_ID / 2),rectP.top+250 + 430 * (l_ID%2),rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);

	//	gSaveInfo.strTestDiscription = "CHECK PASS";

}

void CAcqVoltageSamples_IntClkDlg::BarcodeErrorDisplay()
{
		//CDlgResult ldlg;
		//ldlg.m_Result = 1;
		//Beep(800, 300);
		//if(ldlg.DoModal() == IDOK)
		//{
		//}

		
	


}


void CAcqVoltageSamples_IntClkDlg::CLOSE_Result_Display()
{
	//if(mpDlgResult->IsWindowEnabled())
	//{
	//	if(mpDlgResult->m_Barcode.GetLength() > 6)
	//	{
	//		m_Barcode.put_Caption(mpDlgResult->m_Barcode);
	//	}
	//}
	mpDlgResult->ShowWindow(SW_HIDE);

	//mpDlgSimpleInform->ShowWindow(SW_HIDE);
		//CDlgResult ldlg;
		//ldlg.m_Result = 1;
		//Beep(800, 500);
		//if(ldlg.DoModal() == IDOK)
		//{
		//}

}

CString CAcqVoltageSamples_IntClkDlg::GetMyIPAddress()
{
    char        chName[255];
    CString        sAddress;
    PHOSTENT    pHostEntry;
    IN_ADDR        inAddr;

    if( gethostname( chName, 255 ) != 0 ) return "";
    else
    {
        if( ( pHostEntry = gethostbyname( chName ) ) == NULL ) return "";
        else
        {
            memcpy( &inAddr, pHostEntry->h_addr, 4 );
            sAddress.Format( "%s", inet_ntoa( inAddr ) );
            return sAddress;
        }
    }
}



void CAcqVoltageSamples_IntClkDlg::AddGmesLog(CString LogStr, int Recv)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
	CFileControl lFile;

	lFile.SaveGmesLOG(LogStr, Recv);



}


void CAcqVoltageSamples_IntClkDlg::AddPLCLog(CString LogStr, int Recv)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
	CFileControl lFile;

	lFile.SavePLCLOG(LogStr, Recv);



}


void CAcqVoltageSamples_IntClkDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialog::OnRButtonDown(nFlags, point);
}


void CAcqVoltageSamples_IntClkDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	m_PCB_IN_Error[0] = 1;
	CDialog::OnRButtonUp(nFlags, point);
}



int CAcqVoltageSamples_IntClkDlg::MessageWindow(CString str)
{
	CDlgMessage dlg;
	dlg.m_EditMessage = str;
	int rtn  = dlg.DoModal();
	return rtn;
}

CString CAcqVoltageSamples_IntClkDlg::GetModelList(CString NewName)
{
	BOOL rtnValue = 0;
	
	CString pathName;
	CString fileName;

//	int lFileRename = 0;
	//CString PathNameList[200];
	CString fileNameList[2000];
	
	//CString titleNameList[200];
	//CString  accpathName;
	//CString  ukpathName;
	//CString  chkpathName;
//	char lBuf[2048];
//	char ltmpBuf[2048];


	int nFileCnt = 0;
	CString str;

	CFileFind finder;
	// build a string with wildcards   
	CString strWildcard;
		

	//int nIndexPos = 0;
	BOOL bFileFound = 0;
	//BOOL rtnValue = 0;
	CFileException ex;

   
	strWildcard = PATH_BASE_DIR;
	strWildcard += _T("\\Model\\*.mdl");//"C:\\Program Files\\SM Digital\\nagra\\HDCPKey"
  
	BOOL bWorking = finder.FindFile(strWildcard);
	
	nFileCnt = 0;
	bFileFound = 0;
	while (bWorking)
	{   
		bWorking = finder.FindNextFile();
		
		// skip . and .. files; otherwise, we'd
		// recur infinitely!!
		fileName = finder.GetFileName();
		//str = (LPCTSTR)finder.GetRoot();
		//str = finder.GetFileTitle();
		pathName = finder.GetFilePath();
		//str = finder.GetFileURL();
		//str = finder.GetFileName();

	//	PathNameList[nFileCnt] = pathName;
		fileNameList[nFileCnt] = fileName;
	//	titleNameList[nFileCnt] = finder.GetFileTitle();
			
		nFileCnt++;
		if(nFileCnt >= 1000)
		{
				
			break;
		}
		bFileFound = 1;
				
	}


	finder.Close();

	CString m_strModelNameList[1000];
	CString lCheckModelName;

	if(bFileFound)
	{
		int l_nModelSelNum = 0;
		
		for(int i = 0 ; i < nFileCnt; i++)
		{
			m_strModelNameList[i] = fileNameList[i].Left(fileNameList[i].GetLength() - 4);

		
			if(m_strModelNameList[i].Compare(NewName) == 0)
			{
				lCheckModelName = m_strModelNameList[i];
				return lCheckModelName;
			}
		}

		if(lCheckModelName.GetLength() < 2)
		{		
			MessageDisplay(1, "Model Test File(*.mdl) Not Found!!");
			MessageBox("Model Test File(*.mdl) Not Found!!");
		}
	}
	else
	{
		MessageDisplay(1, "Model Test File(*.mdl) Not Found!!");
		MessageBox("Model Test File(*.mdl) Not Found!!");
	}
	return lCheckModelName;
}


void CAcqVoltageSamples_IntClkDlg::ChangeModel(CString lModel)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//int lGMES_Opened = 0;
	//int lGMES_Port = 0;
	//int lOldPortType = g_nRemotePortType;
//	if(g_nRemotePortType == LINE_CM89)
//
//	lGMES_Opened = gTestSeqData.bMesEnable;
//
//
//	CString str;
//	CString lFindName;		
//	CFileControl lFile;
//	str = "Check S/N is";
//	str += lModel;
//	str += " MODEL. \r\n \r\n";
//	str += "  Do You Change MODEL ?";
//
//	m_TaskRun = TASK_RUN_IDLE;
//
//	m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//	m_LED_COM_PROCESS = COM_MODE_IDLE;
//	m_PCB_COM_PROCESS = COM_MODE_IDLE;//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
//						//m_EthernetMode = 
//
//						//m_NG_WaitFlag = 0;
//						//TestFinishGMES();//
//						//TestFinish();
//
////	if(IDOK == MessageWindow(str))
//	if(IDYES == MessageBox(str,"New Model Detected", MB_YESNO))
//	{
//
//#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("1");
//#endif
//		lFindName =	GetModelList(lModel);
//		if(lFindName.GetLength() > 3)
//		{
//#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("1");
//#endif
//			if(lFile.OpenModel(lFindName))
//			{
//				MessageDisplay(2, "MODEL  FILE NOT FOUND !");
//				MessageBox("MODEL  FILE NOT FOUND !");
//				return;
//			}
//
//			if(lFile.OpenModelSeq(gTestSeqData.strSeqFileName))
//			{
//				lFile.OpenModel(gTestSeqData.strModelName);
//				MessageDisplay(2, "MODEL SEQUENCE FILE NOT FOUND !");
//				MessageBox("MODEL SEQUENCE FILE NOT FOUND !");
//				return;
//			}
//		}
//		else
//		{
//			return;
//		}
//
//#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("2");
//#endif
//
//		gTestSeqData.strModelName = lFindName;
//		lFile.SaveINIModel();
//
//		LoadTestData(gTestSeqData.strSeqFileName);
//#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("3");
//#endif
//		//SetListControl();
//		DisplayTestData();		
//#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("4");
//#endif
//		GetRmtCodeList(gTestSeqData.strRemoconFileName);
//
//#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("5");
//#endif
//		str = gTestSeqData.strModelName;
//		m_cLbModel.put_Caption((LPCSTR)str);
//		m_DO_Writedata = 0;	
//#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("6");
//#endif
//		if(gTestSeqData.bMesEnable)
//		{			
//			if(lGMES_Opened)
//			{
//				if(lOldPortType != g_nRemotePortType)
//				{
//					CloseEthernetScocket() ;
//					m_EthernetMode =  ETHERNET_MODE_IDLE;
//					m_EthernetWaitCnt = 0;
//					g_nPortReopen = 0;
//					gTestSeqData.bMesEnable = 0;	
//
//					m_cLb_ComMainGmes.put_BackColor(0x000000FF);
//				}
//			}
//			else
//			{
//				//CloseEthernetScocket() ;
//				m_EthernetMode =  ETHERNET_MODE_CREATE_SOCKET;
//				m_EthernetWaitCnt = 0;
//				g_nPortReopen = 0;
//				gTestSeqData.bMesEnable = 1;	
//
//				m_cLb_ComMainGmes.put_BackColor(0x000000FF);
//			}
//		}
//		else
//		{
//			if(lGMES_Opened)
//			{				
//				CloseEthernetScocket() ;
//				m_EthernetMode =  ETHERNET_MODE_IDLE;
//				m_EthernetWaitCnt = 0;
//				g_nPortReopen = 0;
//			//	gTestSeqData.bMesEnable = 1;	
//
//				m_cLb_ComMainGmes.put_BackColor(0x000000FF);
//				
//			}
//		}
//		#ifdef _CHANGE_MODEL_DEBUG__
//		MessageBox("7");
//#endif
//	}
//
//	OnBnClickedButtonFreqSet();
//
}


void CAcqVoltageSamples_IntClkDlg::OnShockAccept(int lIndex)
{
#if 1
	bool lConnected = 0;
	bool lResult = 0;
	if(lIndex < 6)
	{
		if(m_pUDPSocketServer[lIndex].m_SocketOpen == 0)
		{
			lResult = m_sListenSocket[lIndex].Accept(m_pUDPSocketServer[lIndex]);
			m_pUDPSocketServer[lIndex].m_SocketOpen = 1;
			m_pUDPSocketServer[lIndex].m_SocketID = lIndex;
			lConnected = 1;
		
		}
	}
	if(lConnected != 1)
	{
		MessageBox(" Reject Socket Connect!!");
	}
#else
	
	}
#endif
}

void CAcqVoltageSamples_IntClkDlg::OnShockConnect(int lIndex)
{
	// Enable the text and message controls
	//GetDlgItem(IDC_EMSG)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BSEND)->EnableWindow(TRUE);
	//GetDlgItem(IDC_STATICMSG)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BCLOSE)->EnableWindow(TRUE);

}

void CAcqVoltageSamples_IntClkDlg::OnShockSend(int lIndex)
{

}

void CAcqVoltageSamples_IntClkDlg::OnShockReceive(int lIndex)
{
	char* pBuf = new char[70001];
	int iBufSize = 70000;
	int iRcvd;
	CString strRecvd;
	static unsigned int sCount = 0;


#if 1 
	if((lIndex < 0) || (lIndex >= 12))
	{
		return;
	}
	
	
	// Did we receive anything?
	
	if (lIndex == PLC_SOCKET_ID)
	{
		iRcvd = m_pPLCSocketClient.Receive(pBuf, iBufSize);
		if(iRcvd == SOCKET_ERROR)
		{
		}
		else
		{
			
			memcpy(&m_sPLCReadBuffer[m_nPLCReadSize], pBuf, iRcvd);
			m_nPLCReadSize += iRcvd;
			OnPLCSocketReceive();
		}
	}
	else 
	{
		iRcvd = m_pUDPSocketServer[lIndex].Receive(pBuf, iBufSize);
		if(iRcvd == SOCKET_ERROR)
		{
		}
		else
		{
			//m_nPLCReadSize;//  sReceiveData.GetLength();
			////sprintf(pBuf,"%s", sReceiveData.GetBuffer());	pBuf[iRcvd] = NULL;
			//memcpy(pBuf, m_sPLCReadBuffer

			// Truncate the end of the message
			pBuf[iRcvd] = NULL;

			// Copy the message to a CString
			strRecvd = pBuf;

			m_strEthernetReceive[lIndex] += strRecvd;
			OnParseDataEthernet(lIndex);//

			// Add the message to the received list box
			//m_ctlRecvd.AddString(strRecvd);
			CString str;
			str.Format(_T("%2d:%d:"), lIndex, sCount);
			sCount++;
			str += strRecvd;
			SetDlgItemText(IDC_STATIC_RECEIVE, str);


			// Sync the variables with the controls
			//UpdateData(TRUE);

		}

	}

	delete[] pBuf;
#else
	// Receive the message
	iRcvd = m_sConnectSocket.Receive(pBuf, iBufSize);
	strRecvd = _T("1:");
	if(iRcvd == SOCKET_ERROR)
	{	
		iRcvd = m_sConnectSocket2.Receive(pBuf, iBufSize);
		strRecvd = _T("2:");
	}
	// Did we receive anything?
	if(iRcvd == SOCKET_ERROR)
	{
	}

	else

	{
		// Truncate the end of the message
		pBuf[iRcvd] = NULL;

		// Copy the message to a CString
		strRecvd += pBuf;

		// Add the message to the received list box
		m_ctlRecvd.AddString(strRecvd);

		// Sync the variables with the controls
		UpdateData(TRUE);

	}

#endif
}

void CAcqVoltageSamples_IntClkDlg::OnShockClose(int lIndex)
{
	// Close the connected socket
	m_pUDPSocketServer[lIndex].Close();
	m_pUDPSocketServer[lIndex].m_SocketOpen = 0;
	
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelServer()
{
	//// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//if(m_sListenSocket.m_SocketOpen != 0)
	//{
	//	m_sListenSocket.Close();
	//}

	//m_sListenSocket.Create(g_nLocalPortNumber);
	//m_sListenSocket.Listen();
}

//
//void CAcqVoltageSamples_IntClkDlg::OnBnClickedButtonExit()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	OnOK();
//}


void CAcqVoltageSamples_IntClkDlg::ClickLabelEquip1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FFFF);
	m_SelectedMonitoringEquipID = 0;
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FF00);

	
	SetSubDetailDisplay( m_SelectedMonitoringEquipID);
	if(m_AlarmNumberOn[m_SelectedMonitoringEquipID] == 1)
	{
		m_AlarmNumberOn[m_SelectedMonitoringEquipID] = 0;
	}
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelEquip2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FFFF);
	m_SelectedMonitoringEquipID = 1;
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FF00);
	SetSubDetailDisplay( m_SelectedMonitoringEquipID);
	if(m_AlarmNumberOn[m_SelectedMonitoringEquipID] == 1)
	{
		m_AlarmNumberOn[m_SelectedMonitoringEquipID] = 0;
	}
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelEquip3()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FFFF);
	m_SelectedMonitoringEquipID = 2;
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FF00);
	SetSubDetailDisplay( m_SelectedMonitoringEquipID);
	if(m_AlarmNumberOn[m_SelectedMonitoringEquipID] == 1)
	{
		m_AlarmNumberOn[m_SelectedMonitoringEquipID] = 0;
	}
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelEquip4()
{
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FFFF);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_SelectedMonitoringEquipID = 3;
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FF00);
	SetSubDetailDisplay( m_SelectedMonitoringEquipID);
	if(m_AlarmNumberOn[m_SelectedMonitoringEquipID] == 1)
	{
		m_AlarmNumberOn[m_SelectedMonitoringEquipID] = 0;
	}
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelEquip5()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FFFF);
	m_SelectedMonitoringEquipID = 4;
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FF00);
	SetSubDetailDisplay( m_SelectedMonitoringEquipID);
	if(m_AlarmNumberOn[m_SelectedMonitoringEquipID] == 1)
	{
		m_AlarmNumberOn[m_SelectedMonitoringEquipID] = 0;
	}
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelEquip6()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FFFF);
	m_SelectedMonitoringEquipID = 5;
	//m_cLbEquip[m_SelectedMonitoringEquipID].put_ForeColor(0x00FF00);
	SetSubDetailDisplay( m_SelectedMonitoringEquipID);
	if(m_AlarmNumberOn[m_SelectedMonitoringEquipID] == 1)
	{
		m_AlarmNumberOn[m_SelectedMonitoringEquipID] = 0;
	}
}

//
//void CAcqVoltageSamples_IntClkDlg::CustomdrawListOqaMonitor(NMLVCUSTOMDRAW* pLVCD, LRESULT *pResult, int lIndex)
//{
//	
//	CString strCol=_T("");
////	int lIndex = 0;
//
//
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//
//
//
//			if(3==(int)(pLVCD->iSubItem) )
//			{
//				strCol= m_cListCtrl_OQA_Monitor[lIndex].GetItemText(pLVCD->nmcd.dwItemSpec,3);
//				if( strCol=="NG") // 두번째열의 값이 "10"일 때, 그 값 부분만			
//				{
//					crText = RGB(0,0,0); //글자색
//					crBkgnd = RGB(255,0,0); //배경색으로 한다    
//				}
//				else if(strCol=="OK")          
//				{
//					crText = RGB(0,0,0);
//					crBkgnd = RGB(0,255,0);
//				}	
//				else
//				{
//					//if((nItem == m_nCurrentNumber[lIndex])&&(m_TaskRun[lIndex] != TASK_RUN_IDLE))//else if(nItem == m_Process_Index)          
//					//{
//					//	crText = RGB(0,0,0);
//					//	crBkgnd = RGB(255,255,100);
//					//}
//					//else 
//					{
//						crText = RGB(0,0,0);
//						crBkgnd = RGB(255,255,255);
//					}
//				}
//		
//			}
//			else
//			{
//				//int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//				//m_TaskRun[lSetIndex];m_Process_Index
//	//int		m_nCurrentNumber;
//				//if((nItem == m_nCurrentNumber[lIndex])&&(m_TaskRun[lIndex] != TASK_RUN_IDLE))
//				//{
//				//	//if(pLVCD->iSubItem >= 1)
//				//	{
//				//		crText = RGB(0,0,0);
//				//		crBkgnd = RGB(255,255,100);
//				//	}
//			
//				//}
//				//else
//				{
//
//					crText = RGB(0,0,0);
//					crBkgnd = RGB(255,255,255);
//				}
//			}
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//}



void CAcqVoltageSamples_IntClkDlg::CustomdrawListOqaMonitor(int nItem, int nlSubItem, COLORREF *pcrText,COLORREF *pcrBkgnd, int lIndex)
{
	
	CString strCol=_T("");
	COLORREF crText, crBkgnd;
	//if (nItem == gSaveInfo[lIndex].nSeqSel)
	//{
	//	crText = RGB(0, 0, 0);
	//	crBkgnd = RGB(128, 128, 0);
	//}
	//
	//else 
		
	if (nlSubItem == 2)
	{
		crText = (COLORREF)(gSaveInfo[lIndex].m_SeqList[nItem].nForeColor);
		if (gSaveInfo[lIndex].m_SeqList[nItem].nBackColor == 0)
			gSaveInfo[lIndex].m_SeqList[nItem].nBackColor = 0xFFFFFF;
		crBkgnd = (COLORREF)(gSaveInfo[lIndex].m_SeqList[nItem].nBackColor);

	}
	else
	{
		crText = RGB(0, 0, 0);
		crBkgnd = RGB(255, 255, 255);
	}

	
	*pcrText = crText;
	*pcrBkgnd= crBkgnd;			

	return;
		
}
void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitorSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;


	//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	CString strCol=_T("");
	int lIndex = m_SelectedMonitoringEquipID;

	switch(pLVCD->nmcd.dwDrawStage)
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

			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row

			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);



			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	
	*pResult = 0;
}



void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;


	//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	CString strCol=_T("");
	int lIndex = 0;

	switch(pLVCD->nmcd.dwDrawStage)
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

			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row

			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);



			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	
	*pResult = 0;
}





void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	CString strCol=_T("");
	int lIndex = 1;

	switch(pLVCD->nmcd.dwDrawStage)
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

			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row

			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);



			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	*pResult = 0;
}

void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor3(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	CString strCol=_T("");
	int lIndex = 2;

	switch(pLVCD->nmcd.dwDrawStage)
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

			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row

			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);



			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	
	*pResult = 0;
	//*pResult = 0;
}

void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor4(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	CString strCol=_T("");
	int lIndex = 3;

	switch(pLVCD->nmcd.dwDrawStage)
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

			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row

			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);



			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	*pResult = 0;
}

void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor5(NMHDR *pNMHDR, LRESULT *pResult)
{
		NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	CString strCol=_T("");
	int lIndex = 4;

	switch(pLVCD->nmcd.dwDrawStage)
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

			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row

			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);



			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	
	*pResult = 0;
}

void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor6(NMHDR *pNMHDR, LRESULT *pResult)
{
		NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	CString strCol=_T("");
	int lIndex = 5;

	switch(pLVCD->nmcd.dwDrawStage)
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

			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row

			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);



			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	
	*pResult = 0;
}

//void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor7(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	CString strCol=_T("");
//	int lIndex = 6;
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row
//
//			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//
//}
//
//void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor8(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//		NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	CString strCol=_T("");
//	int lIndex = 7;
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row
//
//			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);
//
//
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//}
//
//
//void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor9(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	CString strCol=_T("");
//	int lIndex = 8;
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row
//
//			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);
//
//
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//}
//
//
//void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor10(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	CString strCol=_T("");
//	int lIndex = 9;
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row
//
//			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);
//
//
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//}
//
//
//void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor11(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	CString strCol=_T("");
//	int lIndex = 10;
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row
//
//			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);
//
//
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//}
//
//
//void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListOqaMonitor12(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
//	CString strCol=_T("");
//	int lIndex = 11;
//
//	switch(pLVCD->nmcd.dwDrawStage)
//	{
//		case CDDS_PREPAINT:
//		{
//			*pResult = CDRF_NOTIFYITEMDRAW;
//			return;
//		}
//		case CDDS_ITEMPREPAINT:
//		{
//			*pResult = CDRF_NOTIFYSUBITEMDRAW;
//			return;
//		}
//		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
//		{
//			COLORREF crText, crBkgnd;
//			
//		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
//		//	nRow= m_CtrlListMainProcess.GetItemCount();
//
//			int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
//			int nlSubItem = static_cast<int>( pLVCD->iSubItem ); //row
//
//			CustomdrawListOqaMonitor( nItem,  nlSubItem, &crText, &crBkgnd,  lIndex);
//
//
//
//			pLVCD->clrText = crText;
//			pLVCD->clrTextBk= crBkgnd;			
//
//
//			*pResult = CDRF_DODEFAULT; 
//			return;
//		}
//	}
//	
//	*pResult = 0;
//}
//


//void CAcqVoltageSamples_IntClkDlg::ClickLabelSelect1()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	int lIndex = 0;
//	GMES_TestCtrl(m_SelectedMonitoringEquipID, lIndex);
//	
//	SetTimer(2,10000,NULL);
//}
//
//void CAcqVoltageSamples_IntClkDlg::ClickLabelSelect2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	int lIndex = 1;
//	GMES_TestCtrl(m_SelectedMonitoringEquipID, lIndex);
//}
//
//void CAcqVoltageSamples_IntClkDlg::ClickLabelSelect3()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	int lIndex = 2;
//	GMES_TestCtrl(m_SelectedMonitoringEquipID, lIndex);
//}
//
//void CAcqVoltageSamples_IntClkDlg::ClickLabelSelect4()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	int lIndex = 3;
//
//	GMES_TestCtrl(m_SelectedMonitoringEquipID, lIndex);
//}
//
//void CAcqVoltageSamples_IntClkDlg::ClickLabelSelect5()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	int lIndex = 4;
//
//	GMES_TestCtrl(m_SelectedMonitoringEquipID, lIndex);
//}
//
//void CAcqVoltageSamples_IntClkDlg::ClickLabelSelect6()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	int lIndex = 5;
//
//	GMES_TestCtrl(m_SelectedMonitoringEquipID, lIndex);
//}
//


int CAcqVoltageSamples_IntClkDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelConfigSet()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CDlgConfig dlg;

	if (m_ComPortScanner.mPort_Opened == TRUE)
		m_ComPortScanner.ClosePort();

	for (int i = 0; i < 6; i++)
	{
		dlg.m_ServerPortNumber[i].Format(_T("%d"), g_nLocalPortNumber[i]);
	}
	dlg.m_nPortScan = g_nPort_Scanner;
	dlg.m_nBaudScan = g_nBaud_Scanner;
	dlg.m_nParityScan = g_nParity_Scanner;

	dlg.m_EditPLC_IP_Address = g_strPLCIPAddress;
	dlg.m_EditPLC_Port.Format(_T("%d"), g_nPLCPortNumber);


	if (dlg.DoModal() == IDOK)
	{
		for (int i = 0; i < 6; i++)
		{
			g_nLocalPortNumber[i] = atoi(dlg.m_ServerPortNumber[i]);
		}
		
		g_nPort_Scanner = dlg.m_nPortScan;
		g_nBaud_Scanner = dlg.m_nBaudScan;
		g_nParity_Scanner = dlg.m_nParityScan;

		g_strPLCIPAddress = dlg.m_EditPLC_IP_Address;
		g_nPLCPortNumber = atoi(dlg.m_EditPLC_Port );

		CFileControl lFile;
		lFile.SaveConfigSeqData();

		CreateEthernetSocket();

		OpenComportSCAN();
	}

}


void CAcqVoltageSamples_IntClkDlg::ClickLabelExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (mpDlgResult != NULL)
		delete mpDlgResult;

	OnOK();
}

void CAcqVoltageSamples_IntClkDlg::CommandGetDFT_ModelList(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "GET MODEL LIST";
	GMES_TestCtrl(lIndexd, lstrCmd);
}


void CAcqVoltageSamples_IntClkDlg::CommandDFT_ModelChange(int lIndexd, CString strModelInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "CHANGE_MODEL";
	GMES_TestCtrl(lIndexd, lstrCmd, strModelInfo);
}

void CAcqVoltageSamples_IntClkDlg::CommandStart(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	long lcolor = m_cLabelBtnStart[lIndexd].get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLabelBtnStart[lIndexd].put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);

	CString lstrCmd;
	lstrCmd = "START";
	GMES_TestCtrl(lIndexd, lstrCmd);

	   	  
	//m_cLabelBtnStart[6];//IDC_LABEL_BTN_START_DFT1
	//CLabel_text m_cLabelBtnPause[6];//IDC_LABEL_BTN_PAUSE_DFT1	
	//CLabel_text m_cLabelBtnStop[6];
}


void CAcqVoltageSamples_IntClkDlg::CommandPause(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.


	long lcolor = m_cLabelBtnPause[lIndexd].get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLabelBtnPause[lIndexd].put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);


	CString lstrCmd;
	lstrCmd = "PAUSE";
	GMES_TestCtrl(lIndexd, lstrCmd);
}


void CAcqVoltageSamples_IntClkDlg::CommandStop(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.


	long lcolor = m_cLabelBtnStop[lIndexd].get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLabelBtnStop[lIndexd].put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);


	CString lstrCmd;
	lstrCmd = "STOP";
	GMES_TestCtrl(lIndexd, lstrCmd);
}

void CAcqVoltageSamples_IntClkDlg::CommandJigUp(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "JIGUP";
	GMES_TestCtrl(lIndexd, lstrCmd);

}


void CAcqVoltageSamples_IntClkDlg::CommandJigDown(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "JIGDOWN";
	GMES_TestCtrl(lIndexd, lstrCmd);

}


void CAcqVoltageSamples_IntClkDlg::CommandLoad(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "LOAD";
	GMES_TestCtrl(lIndexd, lstrCmd);

}


void CAcqVoltageSamples_IntClkDlg::CommandTestPass(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "TESTPASS";
	GMES_TestCtrl(lIndexd, lstrCmd);

}


void CAcqVoltageSamples_IntClkDlg::CommandTestNG(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "TESTNG";
	GMES_TestCtrl(lIndexd, lstrCmd);

}


void CAcqVoltageSamples_IntClkDlg::CommandClear(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "CLEAR";
	GMES_TestCtrl(lIndexd, lstrCmd);

}


void CAcqVoltageSamples_IntClkDlg::CommandRead(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "READ";
	GMES_TestCtrl(lIndexd, lstrCmd);
}



void CAcqVoltageSamples_IntClkDlg::CommandACPowerOn(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "AC_ON";
	GMES_TestCtrl(lIndexd, lstrCmd); 
}


void CAcqVoltageSamples_IntClkDlg::CommandACPowerOff(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "AC_OFF";
	GMES_TestCtrl(lIndexd, lstrCmd);
}


void CAcqVoltageSamples_IntClkDlg::CommandGetToolOption(int lIndexd, CString strSuffix)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "GET TOOL OPTION";//"GET TOOL OPTION CLOSE"
	GMES_TestCtrl(lIndexd, lstrCmd, strSuffix);
}

void CAcqVoltageSamples_IntClkDlg::CommandGetToolOptionClose(int lIndexd)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString lstrCmd;
	lstrCmd = "GET TOOL OPTION CLOSE";
	GMES_TestCtrl(lIndexd, lstrCmd);
}
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnGetToolClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	for (int i = 0; i < 6; i++)
	{
		CommandGetToolOptionClose(i);
	}
	long lcolor = m_cLB_BtnGetToolOptionClose.get_BackColor();
	if (lcolor != 0x00F0F0)
	{
		m_cLB_BtnGetToolOptionClose.put_BackColor(0x00F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	
}



void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartAll()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_BtnStartAll.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_BtnStartAll.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);


	for (int i = 0; i < 6; i++)
	{
		CommandStart(i);
	}
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseAll()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	long lcolor = m_cLB_BtnPauseAll.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_BtnPauseAll.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);

	for (int i = 0; i < 6; i++)
	{
		CommandPause(i);
	}
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopAll()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	long lcolor = m_cLB_BtnStopAll.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_BtnStopAll.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);


	for (int i = 0; i < 6; i++)
	{
		CommandStop(i);
	}
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft1() { CommandStart(0); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft2() { CommandStart(1); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft3() { CommandStart(2); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft4() { CommandStart(3); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft5() { CommandStart(4); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartDft6() { CommandStart(5); }

void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft1() { CommandPause(0); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft2() { CommandPause(1); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft3() { CommandPause(2); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft4() { CommandPause(3); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft5() { CommandPause(4); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseDft6() { CommandPause(5); }

void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft1() { CommandStop(0); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft2() { CommandStop(1); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft3() { CommandStop(2); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft4() { CommandStop(3); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft5() { CommandStop(4); }
void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopDft6() { CommandStop(5); }


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnChangeModel()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CommandGetDFT_ModelList(m_SelectedMonitoringEquipID);

	long lcolor = m_cLB_BtnChangeModel.get_BackColor();	
	if (lcolor != 0x00F0F0)
	{
		m_cLB_BtnChangeModel.put_BackColor(0x00F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4,100, NULL);
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnScanOn()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_BtnScanOn.get_BackColor();
	if (m_ScanEnable == 1)
	{
		m_ScanEnable = 0;
		if (lcolor != 0x404040)
		{
			m_cLB_BtnScanOn.put_BackColor(0x404040);
		}
		
	}
	else
	{
		m_ScanEnable = 1;
		if (lcolor != 0x00A0A0)
		{
			m_cLB_BtnScanOn.put_BackColor(0x00F0F0);
		}
		m_cLB_Suffix.put_Caption("");
		m_SuffixTmpString = _T("");
	}
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnGetToolOption()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CString strSuffix;

	long lcolor = m_cLB_BtnGetToolOption.get_BackColor();
	if (lcolor != 0x00F0F0)//0xA0A0A0
	{
		m_cLB_BtnGetToolOption.put_BackColor(0x00F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);

	   
	strSuffix = m_cLB_Suffix.get_Caption();
	if ((strSuffix.GetLength() > 10) && (strSuffix.Find(".") > 1))
	{
		if (m_SelectDFT == DFT_SELECT_123)
		{
			for (int i = 0; i < 3; i++)
			{
				CommandGetToolOption(i, strSuffix);
			}
		}
		else if(m_SelectDFT == DFT_SELECT_456)
		{
			for (int i = 4; i < 6; i++)
			{
				CommandGetToolOption(i, strSuffix);
			}
		}
		else
		{ 		
			for (int i = 0; i < 6; i++)
			{
				CommandGetToolOption(i, strSuffix);
			}
		}

	}
	else
	{
		MessageBox("Suffix Nmae is Error!!");
	}
}

void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStartSelect()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	long lcolor = m_cLB_BtnStartSelect.get_BackColor();
	if (lcolor != 0xF0F0F0)//0xA0A0A0
	{
		m_cLB_BtnStartSelect.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);

	CommandStart(m_SelectedMonitoringEquipID);
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnPauseSelect()
{
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	long lcolor = m_cLB_BtnPauseSelect.get_BackColor();
	if (lcolor != 0xF0F0F0)//0xA0A0A0
	{
		m_cLB_BtnPauseSelect.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);

	CommandPause(m_SelectedMonitoringEquipID);
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnStopSelect()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	long lcolor = m_cLB_BtnStopSelect.get_BackColor();
	if (lcolor != 0xF0F0F0)//0xA0A0A0
	{
		m_cLB_BtnStopSelect.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);

	CommandStop(m_SelectedMonitoringEquipID);
}

void CAcqVoltageSamples_IntClkDlg::OpenComportSCAN()//long CAcqVoltageSamples_IntClkDlg::OnReceiveData(WPARAM wParam, LPARAM lParam )
{

	CString str;
	char lParityChar[3] = { 'N', 'E', 'O' };

	if (g_nPort_Scanner <= 0)
	{
		return;
	}

	if (g_nParity_Scanner > 2)
	{
		g_nParity_Scanner = 0;
	}
	if (m_ComPortScanner.mPort_Opened)
	{
		m_ComPortScanner.ClosePort();
	}
	//	if(m_ComPortLED.InitPort(this, g_nPort_LED, g_nBaud_LED, 'N', 8, 1, EV_RXCHAR, 4096 ))
	if (m_ComPortScanner.InitPort(this, g_nPort_Scanner, g_nBaud_Scanner, lParityChar[g_nParity_Scanner], 8, 1, EV_RXCHAR, 4096))
	{
		m_ComPortScanner.StartMonitoring();
		str.Format(_T("SCANNER  COM Port %d Open OK !!"), g_nPort_Scanner, g_nBaud_Scanner);
	
		MessageDisplay(0, str);

		//m_cLabelComScan.put_BackColor(0x00FFFF);
	}
	else
	{
		str.Format(_T("SCANNER  COM Port %d Open FAIL !!"), g_nPort_Scanner, g_nBaud_Scanner);
		//MessageBox(str);
		MessageDisplay(1, str);
		//m_cLabelComScan.put_BackColor(0x808080);
	}
	m_ComPortScanner.m_RxMessageID = WM_COMM_RXCHAR_SCAN;
}


long CAcqVoltageSamples_IntClkDlg::OnReceiveDataSCAN(WPARAM wParam, LPARAM lParam)
{
	BYTE lbuf[4096];
	int lcnt;
	lcnt = (int)wParam;

	if ((m_RecvCntSCAN < 0) || (m_RecvCntSCAN > 4095))
		m_RecvCntSCAN = 0;

	memcpy(lbuf, (void *)lParam, lcnt);

	for (int i = 0; i < lcnt; i++)
	{
		m_RecvBufSCAN[m_RecvCntSCAN++] = lbuf[i];
		if (m_RecvCntSCAN > 4095)
			m_RecvCntSCAN = 0;
	}


	if (m_RecvCntSCAN > 3)
	{
		if ((m_RecvBufSCAN[m_RecvCntSCAN - 2] == 0x0d) && (m_RecvBufSCAN[m_RecvCntSCAN - 1] == 0x0a))
		{
			m_RecvBufSCAN[m_RecvCntSCAN] = 0;
			//m_BarcodeString = m_RecvBufSCAN;
			//m_BarcodeString.MakeUpper();
			//m_BarCode.put_Caption(m_BarcodeString);
			m_SuffixTmpString = m_RecvBufSCAN;
			m_SuffixTmpString.MakeUpper();
			m_cLB_Suffix.put_Caption(m_SuffixTmpString);

			m_RecvCntSCAN = 0;
			UpdateData(FALSE);
			m_SuffixTmpString = _T("");
		}
		else if (m_RecvCntSCAN >= 30)
		{
			MessageDisplay(1, "BARCODE Scan Error !");
			m_RecvCntSCAN = 0;
			UpdateData(FALSE);
			m_SuffixTmpString = _T("");
		}
	}


	return 0;
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelCountSet()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CEditDlg lDlg;
	if (lDlg.DoModal() == IDOK)
	{
		g_nTargetCnt = lDlg.m_nTargetCount;
	}
	
	DisplayCountData();
	
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelCountReset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	g_nOKCnt = 0;
	g_nNGCnt = 0;
	
	DisplayCountData();
}

void CAcqVoltageSamples_IntClkDlg::DisplayCountData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CFileControl lFile;
	lFile.SaveCountData();
	CString strTmp;
	strTmp.Format(_T("COUNT: %4d / %d"), g_nOKCnt, g_nTargetCnt);
	m_cLabelCountDisplay.put_Caption(strTmp);

	if (g_nOKCnt >= g_nTargetCnt)
	{

		ClickLabelBtnPauseAll();
	}
}


void CAcqVoltageSamples_IntClkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlJigup()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_PLC_Ctrl_JigUp.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_JigUp.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);


	CommandJigUp(m_SelectedMonitoringEquipID);
	

}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlJigDown()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_PLC_Ctrl_JigDown.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_JigDown.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	CommandJigDown(m_SelectedMonitoringEquipID);


	
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlLoad()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_PLC_Ctrl_Load.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_Load.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	CommandLoad(m_SelectedMonitoringEquipID);


}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlTestPass()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_PLC_Ctrl_TestPass.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_TestPass.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	CommandTestPass(m_SelectedMonitoringEquipID);

}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlTestNg()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_PLC_Ctrl_TestNG.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_TestNG.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	CommandTestNG(m_SelectedMonitoringEquipID);


}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlClear()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_PLC_Ctrl_Clear.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_Clear.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	CommandClear(m_SelectedMonitoringEquipID);


}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlRead()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	long lcolor = m_cLB_PLC_Ctrl_Read.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_Read.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	CommandRead(m_SelectedMonitoringEquipID);


}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnCtrlAcOnOff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	long lcolor = m_cLB_PLC_Ctrl_AC_ONOFF.get_BackColor();
	if (lcolor != 0xF0F0F0)
	{
		m_cLB_PLC_Ctrl_AC_ONOFF.put_BackColor(0xF0F0F0);
	}
	m_BtnClearCnt = 2;
	SetTimer(4, 100, NULL);
	if (gSaveInfo[m_SelectedMonitoringEquipID].m_PLC_Data.m_cCheckACPowerStatus)
	{
		CommandACPowerOff(m_SelectedMonitoringEquipID);
	}
	else
	{
		CommandACPowerOn(m_SelectedMonitoringEquipID);

	}
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBtnSelectDft()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	m_cLB_BtnSelectDFT.put_BackColor(0x00FFFF);
	SetTimer(4, 200, NULL);
	if (m_SelectDFT == DFT_SELECT_123)
	{
		m_SelectDFT = DFT_SELECT_456;
		m_cLB_BtnSelectDFT.put_Caption("SELECT 456");
	}
	else if (m_SelectDFT == DFT_SELECT_456)
	{
		m_SelectDFT = DFT_SELECT_ALL;
		m_cLB_BtnSelectDFT.put_Caption("SELECT ALL");
	}
	else 
	{
		m_SelectDFT = DFT_SELECT_123;
		m_cLB_BtnSelectDFT.put_Caption("SELECT 123");
	}
}


int CAcqVoltageSamples_IntClkDlg::WriteCommand(UINT Address, char* Data, int lDataLen)
{
	byte packet[256];
	if (lDataLen <= 0)
		return 0;


	int writeSize = 12 + lDataLen * 2;

	packet[0] = 0x50;

	packet[1] = 0x00;

	packet[2] = 0x00;// (byte)this._nNetworkNo;                         // Packet - 네트워크 번호   
	packet[3] = 0xFF;//(byte)this._nPLCNo;                         // Packet - PLC 번호

	packet[4] = 0xFF;//(byte)(this._nIONo & 0x000000FF);           // Packet - IO 번호 Low    
	packet[5] = 0x03;//(byte)((this._nIONo & 0x0000FF00) >> 8);    // 

	packet[6] = 0x00;//(byte)this._nStationNo;                     // Packet - Station 번호

	packet[7] = (byte)(writeSize & 0x000000FF);                  // data size - Low
	packet[8] = (byte)((writeSize & 0x0000FF00) >> 8);                   // data size - HIGH


	packet[9] = 0x0A; // Monitoring Timer Low
	packet[10] = 0x00; // Monitoring Timer High

	packet[11] = 0x01; //Write Command 0401 LOW
	packet[12] = 0x14; //Write Command 0401 HIGH


	packet[13] = 0x00; ////READ Command SUB 0000 LOW
	packet[14] = 0x00; //READ Command SUB 0000 HIGH

	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High

	packet[18] = 0xA8; //D* A8H = Data register

	packet[19] = (byte)(lDataLen & 0xFF);     // Low    
	packet[20] = (byte)(lDataLen >> 8);       // High

	for (int i = 0; i < lDataLen; i++)
	{
		packet[21 + (i * 2)] = (byte)(Data[i] & 0xFF);     // Low
		packet[22 + (i * 2)] = 0x00;//(byte)(sndData[i] >> 8);       // High
	}

	int iLen = 21 + (lDataLen * 2);//m_strMessage.GetLength();


	//int iSent = m_sConnectSocket.Send(packet, iLen);
	int iSent = OnCmdPLCSend(PLC_SOCKET_ID, packet, iLen); //g_pView->m_pPLCSocketClient.Send(packet, iLen);


	// Were we able to send it?
	if (iSent == SOCKET_ERROR)
	{

	}
	else
	{
		//// Add the message to the list box.
		//m_ctlSent.AddString(m_strMessage);
		//// Sync the variables with the controls
		//UpdateData(TRUE);
	}
}

int CAcqVoltageSamples_IntClkDlg::ReadCommand(UINT Address, int lDataLen)
{
#if 1
	byte packet[256];
	if (lDataLen <= 0)
		return 0;


	int writeSize = 12;

	packet[0] = 0x50;

	packet[1] = 0x00;

	packet[2] = 0x00;// (byte)this._nNetworkNo;                         // Packet - 네트워크 번호   
	packet[3] = 0xFF;//(byte)this._nPLCNo;                         // Packet - PLC 번호

	packet[4] = 0xFF;//(byte)(this._nIONo & 0x000000FF);           // Packet - IO 번호 Low    
	packet[5] = 0x03;//(byte)((this._nIONo & 0x0000FF00) >> 8);    // 

	packet[6] = 0x00;//(byte)this._nStationNo;                     // Packet - Station 번호

	packet[7] = (byte)(writeSize & 0x000000FF);                  // data size - Low
	packet[8] = (byte)((writeSize & 0x0000FF00) >> 8);                   // data size - HIGH


	packet[9] = 0x0A; // Monitoring Timer Low
	packet[10] = 0x00; // Monitoring Timer High

	packet[11] = 0x01; //READ Command 0401 LOW
	packet[12] = 0x04; //READ Command 0401 HIGH


	packet[13] = 0x00; ////READ Command SUB 0000 LOW
	packet[14] = 0x00; //READ Command SUB 0000 HIGH

	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High

	packet[18] = 0xAF;//R* File Register 0xA8; //D* A8H = Data register

	packet[19] = (byte)(lDataLen & 0xFF);     // Low    
	packet[20] = (byte)(lDataLen >> 8);       // High

	int iLen = 21;//m_strMessage.GetLength();


	int iSent = OnCmdPLCSend(PLC_SOCKET_ID, packet, iLen); //int iSent = g_pView->m_pPLCSocketClient.Send(packet, iLen);

	// Were we able to send it?
	if (iSent == SOCKET_ERROR)
	{
		AddPLCLog("Send Read Command Fail");
		return 0;
	}
	else
	{
		// Add the message to the list box.
		//m_ctlSent.AddString(m_strMessage);
		//// Sync the variables with the controls
		AddPLCLog("Send Read Command OK");		
		//UpdateData(TRUE);
		return iSent;
	}
#else
	byte packet[256];
	if (lDataLen <= 0)
		return 0;


	int writeSize = 12;

	packet[0] = 0x50;

	packet[1] = 0x00;

	packet[2] = 0x00;// (byte)this._nNetworkNo;                         // Packet - 네트워크 번호   
	packet[3] = 0xFF;//(byte)this._nPLCNo;                         // Packet - PLC 번호

	packet[4] = 0xFF;//(byte)(this._nIONo & 0x000000FF);           // Packet - IO 번호 Low    
	packet[5] = 0x03;//(byte)((this._nIONo & 0x0000FF00) >> 8);    // 

	packet[6] = 0x00;//(byte)this._nStationNo;                     // Packet - Station 번호

	packet[7] = (byte)(writeSize & 0x000000FF);                  // data size - Low
	packet[8] = (byte)((writeSize & 0x0000FF00) >> 8);                   // data size - HIGH


	packet[9] = 0x0A; // Monitoring Timer Low
	packet[10] = 0x00; // Monitoring Timer High

	packet[11] = 0x01; //READ Command 0401 LOW
	packet[12] = 0x04; //READ Command 0401 HIGH


	packet[13] = 0x00; ////READ Command SUB 0000 LOW
	packet[14] = 0x00; //READ Command SUB 0000 HIGH

	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High

	packet[18] = 0xA8; //D* A8H = Data register

	packet[19] = (byte)(lDataLen & 0xFF);     // Low    
	packet[20] = (byte)(lDataLen >> 8);       // High

	int iLen = 21;//m_strMessage.GetLength();

	//m_strMesSend += _T("</EIF>\r\n");

	int iSent =OnCmdGmesSend(PLC_SOCKET_ID, packet, iLen);
	//int iSent = g_pView->m_pPLCSocketClient.Send(packet, iLen);

	// Were we able to send it?
	if (iSent == SOCKET_ERROR)
	{

	}
	else
	{
		// Add the message to the list box.
		//m_ctlSent.AddString(m_strMessage);
		//// Sync the variables with the controls
		//UpdateData(TRUE);
	}

#endif
}

BOOL CAcqVoltageSamples_IntClkDlg::OnPLCSocketReceive()
{
	CString sReceiveData;
	CString sXmlData;

	BOOL bRev = TRUE;
	CString tempId;
	CString sFindElem;
	CString sSetID;

	//sReceiveData = sRcvData;
	//if (gPLC_Ctrl.ReceiveCommString(500, sReceiveData))
	{
		
		//void CSockDlg::OnReceive()
		//{
		byte pBuf[8192];
		int iBufSize = 4096;
		int iRcvd;
		CString strRecvd;
		byte RcvData[256];
		unsigned short lsRcvData[256];

	/*	static int s_cCheckLoadFinish = 1;
		static int s_cCheckUnLoadFinish = 1;
		static int s_cCheckPCBOnLoadSensor = 1;
		static int s_cCheck_PLC_Data_PCB_Inside = 1;*/
		// Truncate the end of the message
		//strRecvd += pBuf;
		//////////////////////////////////////////////////////////////			

		iRcvd = m_nPLCReadSize;//  sReceiveData.GetLength();

		//sprintf(pBuf,"%s", sReceiveData.GetBuffer());	pBuf[iRcvd] = NULL;
		memcpy(pBuf, m_sPLCReadBuffer, iRcvd);
		if (iRcvd >= 11)
		{
#if 0//def			__UPH_SAVE_DEBUG_MODE__
			

			int ReceiveSize	= 30 * 2 + 11;
			iRcvd =  30 * 2 + 11;
			pBuf[9] = 0x00;
			pBuf[10] = 0x00;
#else		
			
			
			
			
			if ((pBuf[0] == 0xD0) && (pBuf[1] == 0x00))
			{
				m_bPLC_WaitCount = 0;
				m_bPLC_Online = 1;
				strRecvd = "PLC Head OK";
				AddPLCLog(strRecvd, 1);
				
			}
			else
			{
				strRecvd = "PLC Head Error";
				//strRecvd += (char *)pBuf;
				//AddStringToStatus(strRecvd); //m_ctlRecvd.AddString(strRecvd);
				AddPLCLog(strRecvd, 1);
				m_nPLCReadSize = 0;
				return FALSE;
			}


			int ReceiveSize = pBuf[8];// + pBuf[7];
			ReceiveSize = ReceiveSize << 8;
			ReceiveSize += pBuf[7] + 9;
#endif
		
			if (iRcvd >= ReceiveSize)
			{

				int lDataLen;

				if ((pBuf[9] == 0x00) && (pBuf[10] == 0x00))//End Code
				{

				}
				else
				{
					strRecvd.Format(_T("Error Code: 0x%02X%02X"), pBuf[10], pBuf[9]);
					//	strRecvd += (char *)pBuf;
					//AddStringToStatus(strRecvd); //m_ctlRecvd.AddString(strRecvd);
					AddPLCLog(strRecvd,1);
					m_nPLCReadSize = 0;
					return FALSE;
				}
				lDataLen = ReceiveSize - 11;
				lDataLen = lDataLen / 2;
				if (lDataLen >= 30)
				{
					
					for (int i = 0; i < lDataLen; i++)
					{
						lsRcvData[i] = pBuf[(i * 2)+11] + (pBuf[(i * 2)+12]<< 8);
					}

#if 0//def			__UPH_SAVE_DEBUG_MODE__


					
					
					m_PLC_AlarmOn = 1;
					
					for (int i = 0; i < 5; i++)
					{
						m_PLC_AlarmNumber[i] = i%2;
					}

					m_PLC_TotalQty = 356;
					m_PLC_OK_Qty = 25;
					m_PLC_NG_Qty = 15;
					for (int i = 0; i < 6; i++)
					{
						m_PLC_OK_QtyDFT[i] = 11+i;
						m_PLC_NG_QtyDFT[i] = 21 + i;
					}
					


#else


					if (lsRcvData[ALARM_FLAG_ADDRESS] & 0x01)
					{
						m_PLC_AlarmOn = 1;
					}
					else
					{
						m_PLC_AlarmOn = 0;
					}
					for (int i = 0; i < 5; i++)
					{
						m_PLC_AlarmNumber[i] = lsRcvData[ALARM_1_WARN_NUMBER_ADDRESS + i];
					}

					m_PLC_TotalQty = lsRcvData[TOTAL_QTY_1_ADDRESS] + (lsRcvData[TOTAL_QTY_2_ADDRESS] << 16);
					m_PLC_OK_Qty = lsRcvData[OK_QTY_ADDRESS] ;
					m_PLC_NG_Qty = lsRcvData[NG_QTY_ADDRESS] ;
					for (int i = 0; i < 6; i++)
					{
						m_PLC_OK_QtyDFT[i] = lsRcvData[OK_1_QTY_ADDRESS + i * 2];
						m_PLC_NG_QtyDFT[i] = lsRcvData[NG_1_QTY_ADDRESS + i * 2 ];
					}

#endif
				}
				
				SavePLCDataLog();
				m_nPLCReadSize = 0;

		 
			}
			else
			{
				strRecvd += "PLC Size Error";
				//AddStringToStatus(strRecvd); //m_ctlRecvd.AddString(strRecvd);
				AddPLCLog("PLC Size Error",1);
				m_nPLCReadSize = 0;
				return FALSE;

			}

		}
		else
		{
			m_nPLCReadSize = 0;
			return FALSE;
		}


	}
	return TRUE;

}
BOOL CAcqVoltageSamples_IntClkDlg::PLC_AlarmCheck()
{
	if (m_pPLCSocketClient.m_SerConnected == 1)
	{
		UINT l_PLC_BaseAddress = 600;
		int lSend = ReadCommand(l_PLC_BaseAddress, 30);
	}
	return TRUE;

}

void CAcqVoltageSamples_IntClkDlg::SavePLCDataLog()
{
	static int s_PLC_AlarmOn = 0;
	//int		m_bPLC_Online;
	//int		m_bPLC_WaitCount;
	//int		m_PLC_AlarmOn;
	//int		m_PLC_AlarmNumber[5];
	//int		m_PLC_TotalQty;
	//int		m_PLC_OK_Qty;
	//int		m_PLC_NG_Qty;
	//int		m_PLC_OK_QtyDFT[6];
	//int		m_PLC_NG_QtyDFT[6];
//	m_PLC_AlarmOn = 0;
//}
//for (int i = 0; i < 5; i++)
//{
//	m_PLC_AlarmNumber[i]
	if (m_PLC_AlarmOn == 1)
	{
		SavePLCAlarm();		

		s_PLC_AlarmOn = m_PLC_AlarmOn;
	}
	else
	{
		if (s_PLC_AlarmOn == 1)
		{			
			SavePLCAlarm();			
		}

		s_PLC_AlarmOn = m_PLC_AlarmOn;
	}
		
	SavePLC_UPH();
}


void CAcqVoltageSamples_IntClkDlg::SavePLCAlarm()
{
	static int		ls_PLC_AlarmNumber[5] = {0,0,0,0,0};
	int				lNewAlarmFlag = 0;

	for (int i = 0; i < 5; i++)
	{
		if (m_PLC_AlarmNumber[i] != ls_PLC_AlarmNumber[i])
		{
			
			lNewAlarmFlag = 1;
		}
	}
	if (lNewAlarmFlag == 1)
	{
		for (int i = 0; i < 5; i++)
		{
			if (m_PLC_AlarmNumber[i] != ls_PLC_AlarmNumber[i])
			{
				ls_PLC_AlarmNumber[i] = m_PLC_AlarmNumber[i];
				if (m_PLC_AlarmNumber[i] > 0)
				{
					SavePLCAlarm(m_PLC_AlarmNumber[i], 1);
				}
				else
				{
					SavePLCAlarm(ls_PLC_AlarmNumber[i], 0);
				}				
			}
		}
	}
}

int CAcqVoltageSamples_IntClkDlg::SavePLCAlarm(int lAlarmNumber , int lON)//m_AlarmCount
{

	if ((lAlarmNumber > 259)&&(lAlarmNumber <= 0))
		return 0;

	char lmonbuf[12][4] = { "JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC" };
	CFile logFILE;

	int buflen = 0;
	char pbufwrite[4096];
	CString strBufWrite;
	CString StrTemp;
	CFileException ex;


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);
	char sDate[1024];


	CString sTempDir;
	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\PLC_AlarmLog\\");

	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year - 100, t->tm_mon + 1);
	strDir += _T(sDate);
	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//sprintf(sDate, "%02d%02d%02d_%s_GMES_log", t->tm_year-100, t->tm_mon+1, t->tm_mday, gTestSeqData.strModelName.GetBuffer());
	sprintf(sDate, "%02d%02d%02d_ALARM_log", t->tm_year - 100, t->tm_mon + 1, t->tm_mday);

	CString fname = strDir;
	fname += _T("\\");
	fname += _T(sDate);
	fname += ".csv";



	if (!logFILE.Open(fname, CFile::modeReadWrite, &ex))
	{
		if (!logFILE.Open(fname, CFile::modeReadWrite | CFile::modeCreate, &ex))
		{
			//MessageBox("Fail to Save Log !!!");
			return 0;
		}
		//	NO	DATE	ACK	COMMANT	REV

		StrTemp.Format(_T("NO, DATE, ACK, COMMANT, ON/OFF\r\n"));
		strBufWrite = StrTemp;
		logFILE.SeekToEnd();
		logFILE.Write(strBufWrite, strBufWrite.GetLength());
		m_AlarmCount = 0;
	}
	else
	{
		CFileControl lFileCtrl;
		if (m_AlarmCount == 0)
		{
			int lCount = lFileCtrl.ReadAlarmCount(fname);
			if (lCount > 0)
				m_AlarmCount = lCount;
		}
	}

	
	m_AlarmCount++;
	if (lON)
	{
		StrTemp.Format(_T("%4d, %02d-%02d-%02d, %02d:%02d:%02d, %Ts, ON\r\n"),
			m_AlarmCount, t->tm_year - 100, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, strAlarmList[lAlarmNumber]);

	}
	else
	{
		StrTemp.Format(_T("%4d, %02d-%02d-%02d, %02d:%02d:%02d, %Ts, OFF\r\n"), 
			m_AlarmCount, t->tm_year - 100, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, strAlarmList[lAlarmNumber]);

	}
	strBufWrite = StrTemp;
	

	logFILE.SeekToEnd();
	logFILE.Write(strBufWrite, strBufWrite.GetLength());

	logFILE.Close();


	return 1;
}
int CAcqVoltageSamples_IntClkDlg::SavePLC_UPH()
{
	int static  sHour = -1;
	int static  sDay = -1;
	char lmonbuf[12][4] = { "JAN","FEB","MAR","APR","MAY","JUN", "JUL","AUG","SEP","OCT","NOV","DEC" };
	CFile logFILE;

	int buflen = 0;
	char pbufwrite[4096];
	CString strBufWrite;
	CString StrTemp;
	CString sTmp;
	CFileException ex;


	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);
	char sDate[1024];

	if ((sHour == t->tm_hour)&&(sDay = t->tm_mday))
		return 0;
	sHour = t->tm_hour;
	sDay = t->tm_mday;

	CString sTempDir;
	CString strDir;
	strDir = _T(PATH_BASE_DIR);
	strDir += _T("\\PLC_UPH_Log\\");

	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year-100, t->tm_mon+1, t->tm_mday);
	sprintf(sDate, "\\[20%02d-%02d]\\", t->tm_year - 100, t->tm_mon + 1);
	strDir += _T(sDate);
	if (_mkdir(strDir.GetBuffer()) == 0)
	{
	}

	//sprintf(sDate, "%02d%02d%02d_%s_GMES_log", t->tm_year-100, t->tm_mon+1, t->tm_mday, gTestSeqData.strModelName.GetBuffer());
	sprintf(sDate, "%02d%02d%02d_UPH_log", t->tm_year - 100, t->tm_mon + 1, t->tm_mday);

	CString fname = strDir;
	fname += _T("\\");
	fname += _T(sDate);
	fname += ".csv";



	if (!logFILE.Open(fname, CFile::modeReadWrite, &ex))
	{
		if (!logFILE.Open(fname, CFile::modeReadWrite | CFile::modeCreate, &ex))
		{
			//MessageBox("Fail to Save Log !!!");
			return 0;
		}
		//	NO	DATE	TIME	TOTAL  Quantity	OK  Quantity	NG  Quantity	DFT#1 OK  Quantity	DFT#1 NG  Quantity	DFT#2 OK  Quantity	DFT#2 NG  Quantity	DFT#3 OK  Quantity	DFT#3 NG  Quantity	DFT#4 OK  Quantity	DFT#4 NG  Quantity	DFT#5 OK  Quantity	DFT#5 NG  Quantity	DFT#6 OK  Quantity	DFT#6 NG  Quantity


		StrTemp.Format(_T("DATE,	TIME,	TOTAL ,	OK ,	NG ,	\
			DFT#1 OK ,	DFT#1 NG ,	DFT#2 OK ,	DFT#2 NG ,	DFT#3 OK ,	DFT#3 NG ,	\
			DFT#4 OK ,	DFT#4 NG ,	DFT#5 OK ,	DFT#5 NG ,	DFT#6 OK ,	DFT#6 NG \r\n"));
		strBufWrite = StrTemp;
		logFILE.SeekToEnd();
		logFILE.Write(strBufWrite, strBufWrite.GetLength());
		
	}
	/*else
	{
		CFileControl lFileCtrl;
		if (m_AlarmCount == 0)
		{
			m_AlarmCount = lFileCtrl.ReadAlarmCount(fname);
		}
	}*/
	
	StrTemp.Format(_T("%02d-%02d-%02d, %02d:%02d, %d, %d, %d "),
		t->tm_year - 100, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min,
		m_PLC_TotalQty,
		m_PLC_OK_Qty,
		m_PLC_NG_Qty);
	for (int i = 0; i < 6; i++)
	{
		sTmp.Format(_T(", %d, %d"), m_PLC_OK_QtyDFT[i], m_PLC_NG_QtyDFT[i]);
		StrTemp += sTmp;
	}
	
	strBufWrite = StrTemp;
	strBufWrite += _T("\r\n");

	logFILE.SeekToEnd();
	logFILE.Write(strBufWrite, strBufWrite.GetLength());

	logFILE.Close();

	return 1;
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelComMonitorScan()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CreateEthernetSocketPLC(g_strPLCIPAddress, g_nPLCPortNumber);

}
