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
#include "AcqVoltageSamples_IntClkDlg.h"
#include "DlgTestPage.h"


//#include "DlgModelSetup.h"
#include "MesDb.h"
#include "FileControl.h"
#include "TlHelp32.h"
#include <direct.h>
#include "DlgModelSetupSeq.h"

#include "IcmpEcho.h"
//#include "ping.h"

//#include "DlgImage.h"
#include "time.h" 


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

extern CAcqVoltageSamples_IntClkDlg *m_P_Main;

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

 /*   m_colors[0] = CNiColor(0, 255, 0);
    m_colors[1] = CNiColor(255, 0, 0);
    m_colors[2] = CNiColor(0, 0, 255);
    m_colors[3] = CNiColor(255, 255, 0);
    m_colors[4] = CNiColor(0, 255, 255);
    m_colors[5] = CNiColor(255, 0, 255);
    m_colors[6] = CNiColor(128, 128, 128);
    m_colors[7] = CNiColor(255, 255, 255);*/
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
		
	m_pUDPSocket  = NULL;

	m_strCameraCheckIMGfile =  _T("btok.jpg");

	m_CheckDelayTime = 100;
	g_MesEnable = 1;

}


//CAcqVoltageSamples_IntClkApp g_theApp;

void CAcqVoltageSamples_IntClkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);



	DDX_Control(pDX, IDC_LABEL_MESSAGE, m_MessageDisplay);

	DDX_Control(pDX, IDC_START_BUTTON, m_cBtnStart);

	DDX_Control(pDX, IDC_SETUP_BUTTON, m_cSetupButton);
	DDX_Control(pDX, IDC_STOP_BUTTON, m_cStopButton);
	DDX_Control(pDX, IDC_TEST_BUTTON, m_cTestButton);
	DDX_Control(pDX, IDOK, m_cExitButton);

	//	DDX_Control(pDX, IDC_TEXTBOX_MODEL, m_TextModel);

	DDX_Control(pDX, IDC_LABEL_BARCODE, m_BarCode);
	DDX_Control(pDX, IDC_LABEL_BARCODE_MARK, m_BarCodeMark);
	DDX_Control(pDX, IDC_LABEL_MODEL, m_cLbModel);
	DDX_Control(pDX, IDC_LABEL__OKNG, m_cLbOkNg);
	//DDX_Control(pDX, IDC_LABEL__OKNG1, m_cLb_Test1);
	//DDX_Control(pDX, IDC_LABEL__OKNG2, m_cLb_Test2);
	//DDX_Control(pDX, IDC_LABEL__OKNG3, m_cLb_Test3);

	//	DDX_Control(pDX, IDC_LABEL_OK_LAN, m_cLb_Lan);
	//	DDX_Control(pDX, IDC_LABEL_USB1, m_cLb_Usb1);
	//	DDX_Control(pDX, IDC_LABEL_USB2, m_cLb_Usb2);

	//	DDX_Control(pDX, IDC_LABEL_LAN_NAME, m_LanLabelName);
	//	DDX_Control(pDX, IDC_LABEL_USB1_NAME, m_Usb1LabelName);
	//	DDX_Control(pDX, IDC_LABEL_USB2_NAME, m_Usb2LabelName);

	//GetDlgItem(IDC_LABEL_CHECK_TIME)->SetWindowPos(NULL,1236, 690, 80,15,SWP_ASYNCWINDOWPOS);
	//GetDlgItem(IDC_LABEL_CYCLE_TIME)->SetWindowPos(NULL,1236, 705, 80,15,SWP_ASYNCWINDOWPOS);

	DDX_Control(pDX, IDC_LABEL_CHECK_TIME, m_CheckTimeLabel);
	DDX_Control(pDX, IDC_LABEL_CYCLE_TIME ,m_CycleTimeLabel);

	DDX_Control(pDX, IDC_LABEL_CHECK_TIME_NAME, m_CheckTimeLabelName);
	DDX_Control(pDX, IDC_LABEL_CYCLE_TIME_NAME ,m_CycleTimeLabelName);
	//DDX_Control(pDX, IDC_LABEL_HZ1, m_clbHz);
	/*
	CLabel_text ;
	CLabel_text ;
	CLabel_text m_OKCntLabelName;
	CLabel_text m_NGLabelName;//
	*/
	DDX_Control(pDX, IDC_LABEL_OK_CNT, m_OKCntLabel);
	DDX_Control(pDX, IDC_LABEL_NG_CNT ,m_NGCntLabel);

	DDX_Control(pDX, IDC_LABEL_OK_CNT_NAME, m_OKCntLabelName);
	DDX_Control(pDX, IDC_LABEL_NG_CNT_NAME ,m_NGCntLabelName);
	DDX_Control(pDX, IDC_LIST_MAIN_PROCESS, m_CtrlListMainProcess);
	//	DDX_Control(pDX, IDC_LABEL_CAMREA_NAME, m_CameraLabelName);
	//	DDX_Control(pDX, IDC_LABEL_CAMERA, m_cLb_Camera);
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	//DDX_Control(pDX, IDC_COMBO_NAME_1, //m_cComboName[0]);
	//DDX_Control(pDX, IDC_COMBO_NAME_2, //m_cComboName[1]);
	//DDX_Control(pDX, IDC_COMBO_NAME_3, //m_cComboName[2]);
	//DDX_Control(pDX, IDC_COMBO_NAME_4, //m_cComboName[3]);
	//DDX_Control(pDX, IDC_COMBO_NAME_5, //m_cComboName[4]);
	//DDX_Control(pDX, IDC_COMBO_NAME_6, //m_cComboName[5]);
	//DDX_Control(pDX, IDC_COMBO_NAME_7, //m_cComboName[6]);
	//DDX_Control(pDX, IDC_COMBO_NAME_8, //m_cComboName[7]);

	//	DDX_Control(pDX, IDC_LABEL_COM_MAIN, m_cLb_ComMain);
	DDX_Control(pDX, IDC_LABEL_COM_SET, m_cLb_ComSet);
	//	DDX_Control(pDX, IDC_LABEL_COM_LED, m_cLb_ComLed);
	DDX_Control(pDX, IDC_LABEL_COM_GMES, m_cLb_ComMainGmes);
	//	DDX_Control(pDX, IDC_LABEL_MANAGER, m_cLabelCableManager);
	//	DDX_Control(pDX, IDC_LABEL_MASSAGE, m_cLabelCableMessage);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Control(pDX, IDC_LABEL_GET_SW, m_cLb_GetSW_Version);
	DDX_Control(pDX, IDC_LABEL_WIFI_CHECK, m_cLb_WifiCheck);
	DDX_Control(pDX, IDC_LABEL_GET_AED_SENSOR, m_cLb_GetAEDSensor);
	DDX_Control(pDX, IDC_LABEL_GET_DARK, m_cLb_GetDark);
	DDX_Control(pDX, IDC_LABEL_GET_BRIGHT, m_cLb_GetBright);
	DDX_Control(pDX, IDC_LABEL_SW_VERSION1, m_cLb_Version1);
	DDX_Control(pDX, IDC_LABEL_SW_VERSION2, m_cLb_Version2);
	DDX_Control(pDX, IDC_LABEL_SW_VERSION3, m_cLb_Version3);
	DDX_Control(pDX, IDC_LABEL_SW_VERSION4, m_cLb_Version4);
	DDX_Control(pDX, IDC_LABEL_WIFICHECK, m_cLb_WIFI);
	DDX_Control(pDX, IDC_LABEL_AED_SENSOR, m_cLb_AED_Sensor[0]);
	DDX_Control(pDX, IDC_LABEL_AED_SENSOR2, m_cLb_AED_Sensor[1]);
	DDX_Control(pDX, IDC_LABEL_AED_SENSOR3, m_cLb_AED_Sensor[2]);
	DDX_Control(pDX, IDC_LABEL_AED_SENSOR4, m_cLb_AED_Sensor[3]);
	DDX_Control(pDX, IDC_LABEL_AED_SENSOR5, m_cLb_AED_Sensor[4]);
	DDX_Control(pDX, IDC_LABEL_AED_SENSOR6, m_cLb_AED_Sensor[5]);
	DDX_Control(pDX, IDC_LABEL_AED_SENSOR7, m_cLb_AED_Sensor[6]);
	DDX_Control(pDX, IDC_LABEL_EXPOSURE, m_cLb_Exposure);
	DDX_Control(pDX, IDC_LABEL_IMAGE_WIDTH, m_cLb_ImageWidth);
	DDX_Control(pDX, IDC_LABEL_IMAGE_HEIGHT, m_cLb_ImageHeight);
	DDX_Control(pDX, IDC_LABEL_MAC_CHECK, m_cLb_MAC);
}


BEGIN_MESSAGE_MAP(CAcqVoltageSamples_IntClkDlg, CDialog)
    //{{ AFX_MSG_MAP
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}} AFX_MSG_MAP
    ON_BN_CLICKED(IDC_START_BUTTON, OnBnClickedStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CAcqVoltageSamples_IntClkDlg::OnBnClickedStopButton)
	ON_BN_CLICKED(IDC_SETUP_BUTTON, &CAcqVoltageSamples_IntClkDlg::OnBnClickedSetupButton)
	ON_BN_CLICKED(IDC_TEST_BUTTON, &CAcqVoltageSamples_IntClkDlg::OnBnClickedTestButton)
	//ON_MESSAGE(WM_COMM_RXCHAR,OnReceiveData)
	//ON_MESSAGE(WM_COMM_RXCHAR_PCB,OnReceiveDataPCB)
	//ON_MESSAGE(WM_COMM_RXCHAR_LED,OnReceiveDataLED)
	ON_MESSAGE(WM_COMM_RXCHAR_ETHERNET, OnReceiveDataEthernet)
	ON_MESSAGE(WM_COMM_RXCHAR_SCAN,OnReceiveDataSCAN)

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CAcqVoltageSamples_IntClkDlg::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CAcqVoltageSamples_IntClkDlg::OnHdnItemclickListMainProcess)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAIN_PROCESS, &CAcqVoltageSamples_IntClkDlg::OnNMClickListMainProcess)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MAIN_PROCESS, &CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListMainProcess)
	ON_STN_DBLCLK(IDC_STATIC_PIC, &CAcqVoltageSamples_IntClkDlg::OnStnDblclickStaticPic)
	ON_STN_CLICKED(IDC_STATIC_PIC, &CAcqVoltageSamples_IntClkDlg::OnStnClickedStaticPic)
	ON_WM_RBUTTONDBLCLK()
//	ON_CBN_SELCHANGE(IDC_COMBO_NAME_1, &CAcqVoltageSamples_IntClkDlg::OnCbnSelchangeComboName1)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
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
			if(m_BarcodeString.GetLength() > 6)
			{
				m_WipID = m_BarcodeString;
			}
			OnBnClickedStart();//ClickCbtnStart();
			m_BarcodeString = _T("");
			return TRUE;
		}
	}		
	else if(pMsg->message == WM_CHAR)
	{
		m_BarcodeString = m_BarcodeString  + (char)(pMsg->wParam);
		m_BarcodeString.MakeUpper();
		m_BarCode.put_Caption(m_BarcodeString);
		return TRUE;		
	}	
	else if(pMsg->message == WM_LBUTTONDOWN)
	{
		if(m_TaskRun != TASK_RUN_IDLE)
		{
			RECT lRect;
			m_CtrlListMainProcess.GetWindowRect(&lRect);
		
			if((lRect.bottom > pMsg->pt.y)&&(lRect.top < pMsg->pt.y)
				&&(lRect.left < pMsg->pt.x)&&(lRect.right > pMsg->pt.x))
			{
				return TRUE;
			}
		}
	}
	
	
	return CDialog::PreTranslateMessage(pMsg);
}


#define _MANUAL_SCOPE_ADDMODE
// CAcqVoltageSamples_IntClkDlg message handlers

BOOL CAcqVoltageSamples_IntClkDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
	m_P_Main = this;
    // Set the icon for this dialog.  The framework does this automatically
    // when the application's main window is not a dialog.
    SetIcon(m_hIcon, TRUE);     // Set big icon
    SetIcon(m_hIcon, FALSE);    // Set small icon

	Beep(1200, 200);
	Beep(1200, 200);
	//CString lfname = _T("MultiAudioTester.exe");
	//if( GetProcessList(lfname, 1024) > 1)
	//{
	//	MessageBox("MultiAudioTester.exe Program Running Detected!!");
	//	OnCancel();
	//	
	//} 
//		CTime lCurTime;
//		lCurTime = CTime::GetcurrentTime()
////	int lYear = lCurTime.getcu
#ifdef __START_DEBUG_MODE_
	MessageBox("1");
#endif

//	m_WaitPCB_Ack_Limit = 30;
	int lMakeflag = 0;

	CString str, BaseStr;
	BaseStr = _T(PATH_BASE_DIR);
	
	str = _T("D:\\SM Digital");
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
	
	//str = BaseStr + _T("\\Remote");

	//if(_mkdir(str.GetBuffer())==0)
	//{
	//	lMakeflag = 1;//MessageBox("Make New Directory => [d:\\nagra\\data]");
	//}		
	
	str = BaseStr + _T("\\Model");

	if(_mkdir(str.GetBuffer())==0)
	{
		lMakeflag = 1;//MessageBox("Make New Directory => [d:\\nagra\\data]");
	}	
	str = BaseStr + _T("\\Image");

	if(_mkdir(str.GetBuffer())==0)
	{
		lMakeflag = 1;//MessageBox("Make New Directory => [d:\\nagra\\data]");
	}	

	str = BaseStr + _T("\\Sound");

	if(_mkdir(str.GetBuffer())==0)
	{
		lMakeflag = 1;//MessageBox("Make New Directory => [d:\\nagra\\data]");
	}	



	m_cSetupButton.LoadBitmaps(IDB_BITMAP_MODEL_ON, IDB_BITMAP_MODEL_OFF);
	m_cBtnStart.LoadBitmaps(IDB_BITMAP_START_ON, IDB_BITMAP_START_OFF);//m_cBtnStart.LoadBitmaps(IDB_BITMAP_START_ON, IDB_BITMAP_START_OFF);
	m_cStopButton.LoadBitmaps(IDB_BITMAP_STOP_ON, IDB_BITMAP_STOP_OFF);	
	m_cTestButton.LoadBitmaps(IDB_BITMAP_TEST_ON, IDB_BITMAP_TEST_OFF);
	m_cExitButton.LoadBitmaps(IDB_BITMAP_EXIT_ON, IDB_BITMAP_EXIT_OFF);


	float lfval = 0.0;
	str.Format(_T("%.1f (s)"), lfval);	
	m_CheckTimeLabel.put_Caption(str);

	lfval = 0.0;
	str.Format(_T("%.1f (s)"), lfval);	
	m_CycleTimeLabel.put_Caption(str);

	CFileControl lFile;
	lFile.ReadCountData();

	str.Format(_T("%d"), g_nOKCnt);	
	m_OKCntLabel.put_Caption(str);

	str.Format(_T("%d"), g_nNGCnt);	
	m_NGCntLabel.put_Caption(str);


	if(lFile.ReadCableCountData() != 0)
	{
		lFile.SaveCableCountData();
	}
	



#ifdef __START_DEBUG_MODE_
	MessageBox("2");
#endif
/////////////////////////////////////////////////////

#ifdef __G_MES_SET_MODE__
	if(lFile.Read_ConfigSeqData())
	{
		lFile.SaveConfigSeqData();
	}
#else

	if(lFile.Read_ConfigData())
	{
		lFile.SaveConfigData();
	}

#endif

		
	OpenComportSCAN();
//
	
	if(m_ComPortScanner.mPort_Opened == FALSE)
	{	
		str.Format(_T("SCANNER COM Port %d Open FAIL !!"),  g_nPort_Scanner, g_nBaud_Scanner);		
		MessageBox(str);
	}

#ifdef __START_DEBUG_MODE_
	MessageBox("3");
#endif


	lFile.Read_IniModel();//gTestSeqData.strModelName
	lFile.OpenModel(gTestSeqData.strModelName);
	LoadTestData(gTestSeqData.strSeqFileName);

	SetListControl();
	DisplayTestData();
		
//	CheckDigitalPort();

//	GetRmtCodeList(gTestSeqData.strRemoconFileName);

	str = gTestSeqData.strModelName;
	m_cLbModel.put_Caption((LPCSTR)str);

	m_DO_Writedata = 0;

	m_DIportOpen = 1;
	m_DOportOpen = 1;

	m_NewSystemCheck = 0;
//	m_RecvCnt = 0;
	//m_MainPAGE_COM_PROCESS = 0;
	m_ComAck = 0;
	m_ResendCnt = 0;
	m_ComWaitCnt = 0;
	m_DigitalPort = DIG_OPTICAL_SELECT;

//	m_PrecodeWait = 0;

	#ifdef __START_DEBUG_MODE_
	MessageBox("4");
#endif



#ifdef __G_MES_SET_MODE__

	//Send Are You There
	

	//CreateEthernetSocket();

//	if(gTestSeqData.bMesEnable)
	if(g_MesEnable)
	{

		m_EthernetMode =  ETHERNET_MODE_CREATE_SOCKET;
		m_EthernetWaitCnt = 0;
	}


#else //#endif
		
	

#endif
//	MessageBox("5");

	//for(int i = 0; i < 8; i++)
	//{
	//	m_graph[i].PlotTemplate.LineWidth = 1;
	//	m_graph[i].Axes.Item("YAxis-1").Maximum = 1.2;
	//	m_graph[i].Axes.Item("YAxis-1").Minimum = -1.2;
	//}

	#ifdef __START_DEBUG_MODE_
	MessageBox("5");
#endif

//	SetChannelData(0);
//	InitChannel();
	if(m_InitAiChannel == 0)
	{
		MessageBox("Audio Capture Card INIT FAIL!!");
	//	g_NIAuioBoardFail = 1;//AUDIO Board Fail
	}
#ifdef __START_DEBUG_MODE_
	MessageBox("6");
#endif
	CheckTimeOn = 0;
	CycleTimeOn = 0;
	
	CString lStrName[18] = {
		_T("  HDMI_R"),		_T("  HDMI_L"),
		_T("  OPTICAL_R"),	_T("  OPTICAL_L"),
		_T("  FRONT R"),	_T("  FRONT L"),
		_T("  REAR R"),		_T("  REAR L"),
		_T("  CENTER"),		_T("  WOOFER"),
		_T("  SPEAKER_4R"),	_T("  SPEAKER_4L"),
		_T("  AUX_R"),		_T("  AUX_L"),
		_T("  PORTABLE_R"), _T("  PORTABLE_L"), 
		_T("  COAX_R"),		_T("  COAX_L")};

	
	font = new CFont;
////	font->CreateFontA( 16,                          // nHeight
//	font->CreateFontA( 25,                          // nHeight
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


	font->CreateFontA( 20,                          // nHeight
					   0,                          // nWidth
					   0,                          // nEscapement
					   0,                          // nOrientation
					   600,                           // nWeight
					   0,                          // bItalic
					   0,                          // bUnderline 
					   0,                          // cStrikeOut 
					   0,                          // nCharSet
					   0,           // nOutPrecision 
					   0,                          // nClipPrecision 
					 PROOF_QUALITY,              // nQuality
					 0,  // nPitchAndFamily 
					 _T("Arial"));                     // lpszFacename
	//font->CreatePointFont(40,_T("Arial"));   
	

#ifdef __START_DEBUG_MODE_
	MessageBox("7");
#endif

	for(int i = 0 ;i < 8 ;i++)
	{
		for(int j =0; j < 18; j++)
		{
			//m_cComboName[i].AddString(lStrName[j]); 
		}
		//m_cComboName[i].SetCurSel(g_ManualId[i]);
		//m_cComboName[i].SetFont(font);
	}
	delete font;

	#ifdef __START_DEBUG_MODE_
	MessageBox("8");
#endif

//	CPing	m_ping;
	//MessageBox("6");
	
	IcmpEcho ping;
	unsigned long ulIP;
	DWORD delay;
	CString strtmp;
	CString strtmpIPAddress;
	delay = 1000;	

	
	//if(gTestSeqData.bMesEnable)
	if(g_MesEnable)
	{
		ulIP = ping.ResolveIP( g_nRemoteIPAddress.GetBuffer() );
		delay = ping.PingHostOK( ulIP, PING_TIMEOUT);


		if(delay > 100)
		{
			m_cLb_ComMainGmes.put_BackColor(0x000000FF);
			MessageBox("GMES NOT Connected!");
			g_MesEnable = 0;//gTestSeqData.bMesEnable = 0;
			m_EthernetMode =  ETHERNET_MODE_IDLE;
		}


	}


#ifdef __START_DEBUG_MODE_
	MessageBox("11");
#endif
//	bCapture = false;




	SetTimer(1,10,NULL);
	SetTimer(2,1000,NULL);

	//OnBnClickedButtonFreqSet();

	mpDlgResult = new CDlgResult;	
	mpDlgResult->Create(CDlgResult::IDD, this);
	mpDlgResult->ShowWindow(SW_HIDE);//SetWindowPos(this,rectP.left+ 15,rectP.top+160,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);
	mpDlgResult->pDlg = this;
//	mpDlgResult->m_pMainDlg = this;

	mpDlgBarcode = new CDlgBarcode;	
	mpDlgBarcode->Create(CDlgBarcode::IDD, this);
	mpDlgBarcode->ShowWindow(SW_HIDE);//SetWindowPos(this,rectP.left+ 15,rectP.top+160,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);
	//mpDlgResult->pMainWnd = this;

	mpDlgSimpleInform = new CDlgSimpleInform;	
	mpDlgSimpleInform->Create(CDlgSimpleInform::IDD, this);
	mpDlgSimpleInform->ShowWindow(SW_HIDE);//SetWindowPos(this,rectP.left+ 15,rectP.top+160,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);
	//#include "DlgSimpleInform.h"
#ifdef __START_DEBUG_MODE_
	MessageBox("12");
#endif

	//NG_Display("rorouerqrpotq\r\nuuhrthert\r\njhhguoyt");
	// Return TRUE unless you set the focus to a control.
    return TRUE;
}


void CAcqVoltageSamples_IntClkDlg::OpenComportSCAN()//long CAcqVoltageSamples_IntClkDlg::OnReceiveData(WPARAM wParam, LPARAM lParam )
{

	CString str;
	char lParityChar[3] = {'N', 'E', 'O'};

	if(g_nParity_Scanner > 2)
	{
		g_nParity_Scanner = 0;
	}
	if(m_ComPortScanner.mPort_Opened)
	{
		m_ComPortScanner.ClosePort();
	}
//	if(m_ComPortLED.InitPort(this, g_nPort_LED, g_nBaud_LED, 'N', 8, 1, EV_RXCHAR, 4096 ))
	if(m_ComPortScanner.InitPort(this, g_nPort_Scanner, g_nBaud_Scanner, lParityChar[g_nParity_Scanner], 8, 1, EV_RXCHAR, 4096 ))
	{		
		m_ComPortScanner.StartMonitoring();
	}
	else
	{
		str.Format(_T("SCANNER  COM Port %d Open FAIL !!"),  g_nPort_Scanner, g_nBaud_Scanner);
		//MessageBox(str);
		MessageDisplay(1, str);
		/*StatusDisplayMsg(str, 1 );*/
		//MessageBox(str);
	}
	m_ComPortScanner.m_RxMessageID = WM_COMM_RXCHAR_SCAN;
}


long CAcqVoltageSamples_IntClkDlg::OnReceiveDataSCAN(WPARAM wParam, LPARAM lParam )
{
	BYTE lbuf[4096];
	int lcnt;
	lcnt = (int)wParam;

	if((m_RecvCntSCAN < 0)||(m_RecvCntSCAN > 4095))
		m_RecvCntSCAN = 0;

	memcpy(lbuf,(void *)lParam,lcnt);

	for(int i = 0; i < lcnt; i++)
	{
		m_RecvBufSCAN[m_RecvCntSCAN++] = lbuf[i];
		if(m_RecvCntSCAN > 4095)
			m_RecvCntSCAN = 0;
	}
	

	if(m_RecvCntSCAN > 3)
	{
		for(int i = 0; i < m_RecvCntSCAN; i++)
		{
			if((m_RecvBufSCAN[i] == 0x0d)|| (m_RecvBufSCAN[i] == 0x0a))
			{
				m_RecvBufSCAN[i] = 0;
				if((i > 6)&&(i < 30))
				{
					m_RecvBufSCAN[m_RecvCntSCAN] = 0;
					m_BarcodeString = m_RecvBufSCAN;			
					m_BarcodeString.MakeUpper();
					m_BarCode.put_Caption(m_BarcodeString);

					m_RecvCntSCAN = 0;	
				//	UpdateData(FALSE);

					m_WipID = m_BarcodeString;
			
					OnBnClickedStart();
					m_BarcodeString = _T("");

				}
				else
				{
					MessageDisplay(1, "BARCODE Scan Error !");
					m_RecvCntSCAN = 0;
					//UpdateData(FALSE);
					m_BarcodeString = _T("");
				}
			}
		}
		
		if(m_RecvCntSCAN >= 30)
		{
			MessageDisplay(1, "BARCODE Scan Count Error !");
			m_RecvCntSCAN = 0;
		//	UpdateData(FALSE);
			m_BarcodeString = _T("");
		}		
	}
	

	return 0;
}




BOOL CAcqVoltageSamples_IntClkDlg::CreateEthernetSocket() 
{
	BOOL lresult = 0;
	if (m_pUDPSocket) 
	{
		AfxMessageBox("Socket already created, \r\n Close and Reopen!");
		//lresult = m_pUDPSocket->Connect(g_nRemoteIPAddress, g_nRemotePortNumber);
		//if(lresult == 0)
		//{
		//	if (GetLastError() != WSAEWOULDBLOCK) 
		//	{
				m_pUDPSocket->Close();
				delete m_pUDPSocket;
				m_pUDPSocket = NULL;
				AfxMessageBox (m_szError);
		//		return FALSE;
		//	}
		//}
	}
//	else
	{
		if (g_nLocalPortNumber == 0)
		{
			AfxMessageBox ("Please enter a local port number");
			return FALSE;
		}
		if ((m_pUDPSocket = new CUdpAsySk(this)) == NULL) 
		{
			AfxMessageBox ("Failed to allocate UDP socket! Close and restart app.");
			return FALSE;
		}
		m_pUDPSocket->m_sendBuffer = "";   //for async send
		m_pUDPSocket->m_nBytesSent = 0;
		m_pUDPSocket->m_nBytesBufferSize = 0;


		g_nLocalPortNumber++;
		if(g_nLocalPortNumber > 7500)
		{
			g_nLocalPortNumber = 7000;
		}
	//	if (!m_pUDPSocket->Create(g_nLocalPortNumber, SOCK_DGRAM))
		if (!m_pUDPSocket->Create(g_nLocalPortNumber, SOCK_STREAM))
		{
			wsprintf(m_szError, "Failed to create TCP socket: %d! Close and restart app.", m_pUDPSocket->GetLastError());
			delete m_pUDPSocket;
			m_pUDPSocket = NULL;
			AfxMessageBox (m_szError);
		
			return FALSE;
		}
		lresult = m_pUDPSocket->Connect(g_nRemoteIPAddress, g_nRemotePortNumber);
		if(lresult == 0)
		{
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				m_pUDPSocket->Close();
				delete m_pUDPSocket;
				m_pUDPSocket = NULL;
				AfxMessageBox (m_szError);
				return FALSE;
			}
		}
	}

		
	

	return TRUE;
}


BOOL CAcqVoltageSamples_IntClkDlg::ConnectSocket() 
{
	BOOL lresult = 0;
	if (m_pUDPSocket) 
	{
	
		lresult = m_pUDPSocket->Connect(g_nRemoteIPAddress, g_nRemotePortNumber);
		if(lresult == 0)
		{
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				m_pUDPSocket->Close();
				delete m_pUDPSocket;
				m_pUDPSocket = NULL;
				AfxMessageBox (m_szError);
				return FALSE;
			}
		}
	}
	else
	{
		if (g_nLocalPortNumber == 0)
		{
			AfxMessageBox ("Please enter a local port number");
			return FALSE;
		}
		if ((m_pUDPSocket = new CUdpAsySk(this)) == NULL) 
		{
			AfxMessageBox ("Failed to allocate UDP socket! Close and restart app.");
			return FALSE;
		}
		m_pUDPSocket->m_sendBuffer = "";   //for async send
		m_pUDPSocket->m_nBytesSent = 0;
		m_pUDPSocket->m_nBytesBufferSize = 0;

		g_nLocalPortNumber++;
		if(g_nLocalPortNumber > 7500)
		{
			g_nLocalPortNumber = 7000;
		}
	//	if (!m_pUDPSocket->Create(g_nLocalPortNumber, SOCK_DGRAM))
		if (!m_pUDPSocket->Create(g_nLocalPortNumber, SOCK_STREAM))
		{
			wsprintf(m_szError, "Failed to create TCP socket: %d! Close and restart app.", m_pUDPSocket->GetLastError());
			delete m_pUDPSocket;
			m_pUDPSocket = NULL;
			AfxMessageBox (m_szError);
		
			return FALSE;
		}
		lresult = m_pUDPSocket->Connect(g_nRemoteIPAddress, g_nRemotePortNumber);
		if(lresult == 0)
		{
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				m_pUDPSocket->Close();
				delete m_pUDPSocket;
				m_pUDPSocket = NULL;
				AfxMessageBox (m_szError);
				return FALSE;
			}
		}
	}

		
	

	return TRUE;
}

//UINT g_nRemotePortNumber = 8000;
//UINT g_nLocalPortNumber = 7000;
//CString g_nRemoteIPAddress = _T("192.168.1.20");

void CAcqVoltageSamples_IntClkDlg::CloseEthernetScocket() 
{

	if (m_pUDPSocket)
	{
		m_pUDPSocket->Close();
		delete m_pUDPSocket;

		m_pUDPSocket  = NULL;

	}	
	m_EthernetMode =  ETHERNET_MODE_IDLE;	
	g_MesEnable = 0;
	m_cLb_ComMainGmes.put_BackColor(0x000001FF);
}

BOOL CAcqVoltageSamples_IntClkDlg::OnCmdGmesSend() 
{
	
	if (!m_pUDPSocket)
	{
		m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		AfxMessageBox("GMES Please create socket first.");
		m_EthernetMode =  ETHERNET_MODE_IDLE;
		//MessageBox("GMES Socket Create Fail!");
		g_MesEnable = 0;
		
		return FALSE;
	}
	if (m_strMesSend.GetLength() == 0) 
	{
		MessageDisplay(1, "GMES Data Strings ERROR to Send");
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


	if(m_pUDPSocket->m_Connected == 0)
	{
		m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		MessageDisplay(1, "GMES Socket Not Connected!!");
		m_EthernetMode =  ETHERNET_MODE_IDLE;
		g_MesEnable = 0;
		AfxMessageBox("GMES Socket Not Connected.");
		CloseEthernetScocket() ;
		return FALSE;	
	}

	m_pUDPSocket->m_nBytesSent = 0;
/////////////////
/*	
	TCHAR szSend[65467];
	for (int i=0; i<65467; i++)
		szSend[i] = 'c';
	szSend[65466] = 0;
	m_pUDPSocket->m_sendBuffer = CString(szSend);
	m_pUDPSocket->m_nBytesBufferSize = m_pUDPSocket->m_sendBuffer.GetLength() + 1;
*/
////////////////
	m_pUDPSocket->m_sendBuffer = m_strMesSend;
	m_pUDPSocket->m_nBytesBufferSize = m_strMesSend.GetLength() + 1;

	
	if(m_pUDPSocket->DoAsyncSendBuff() == 1)
	{
		m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		MessageBox("GMES Server Connect FAIL! \r\nGMES DATA Send ERROR","ERROR");
		//if(IDYES == MessageBox("GMES Server Connect FAIL! \r\nGMES DATA Send ERROR","ERROR"))
		//{
		//	CloseEthernetScocket() ;
		//	m_EthernetMode = ETHERNET_MODE_CREATE_SOCKET;			
		//}
		//else
		//{
		//	m_EthernetMode =  ETHERNET_MODE_IDLE;
		//}
		g_MesEnable = 0;
		CloseEthernetScocket() ;
		return FALSE;
	}
	//m_strMesSend = "";
	//m_EthernetMode = ETHERNET_MODE_WAIT;
	AddGmesLog(m_strMesSend);
	return TRUE;
}

void CAcqVoltageSamples_IntClkDlg::GMESAreYouThereRequest() //S1F1
{
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S1F1\" NAME=\"Are You There Request\">\r\n");
	m_strMesSend += _T("</EIF>");
	if(OnCmdGmesSend() == 0)
	{
		m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		MessageDisplay(1, "GMES INIT Fail!");
		g_MesEnable = 0;
	}
}



void CAcqVoltageSamples_IntClkDlg::GMESEquipmentStatusSend(int lStatus) //S1F3
{
	//EquipmentStatusSend
	CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S1F3\" NAME=\"Equipment Status Send\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");

	m_strMesSend += _T("  <ITEM>\r\n");
	m_strMesSend += _T("    <EQPSTATUS>");
	if(lStatus == 1)
	{
		m_strMesSend +=  _T("R");
	}
	else
	{
		m_strMesSend +=  _T("D");
	}
	m_strMesSend += _T("</EQPSTATUS>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");
	m_strMesSend += _T("</EIF>\r\n");	
	
	OnCmdGmesSend();
}

void CAcqVoltageSamples_IntClkDlg::GMESLinkLoopbackTest() //S2F4
{
//<EIFVERSION="1.8"ID="S2F25"NAME="LoopbackdiagnosticsTestRequest">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<AS></AS>
//</ITEM>
//</EIF>
	//EquipmentStatusSend
	//CString str1;
	m_strMesSend = _T("<EIFVERSION=\"1.4\"ID=\"S2F25\"NAME=\"LoopbackdiagnosticsTestRequest\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	
	m_strMesSend += _T("<ITEM>\r\n");
	m_strMesSend += _T("<AS>HZ_AUDIO</AS>\r\n");
	m_strMesSend += _T("</ITEM>\r\n");
	m_strMesSend += _T("</EIF>\r\n");	
	
	OnCmdGmesSend();
}

void CAcqVoltageSamples_IntClkDlg::GMESLinkLoopbackResponce(CString strAS_) //S2F4	void GMESLinkLoopbackResponce();
{
//<EIFVERSION="1.8"ID="S2F26"NAME="LoopbackdiagnosticsTestAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<AS></AS>
//</ITEM></EIF >
	//EquipmentStatusSend
	//CString str1;
	m_strMesSend = _T("<EIFVERSION=\"1.4\"ID=\"S2F26\"NAME=\"LoopbackdiagnosticsTestAcknowledge\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	
	m_strMesSend += _T("<ITEM>\r\n");
	m_strMesSend += _T("<AS>");
	m_strMesSend += strAS_;
	m_strMesSend += _T("</AS>\r\n");
	m_strMesSend += _T("</ITEM>\r\n");
	m_strMesSend += _T("</EIF>\r\n");	
	
	OnCmdGmesSend();
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
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F4\" NAME=\"Link Test Response\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	
	m_strMesSend += _T("</EIF>\r\n");	
	
	OnCmdGmesSend();
}

void CAcqVoltageSamples_IntClkDlg::GMESDateandTimeData() //S1F3
{
//<EIFVERSION="1.8"ID="S2F18"NAME="DateandTimeData">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<TIME></TIME>
//</ITEM>
//</EIF>
	//EquipmentStatusSend
	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);		

	CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F18\" NAME=\"Date and Time Data\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	
	m_strMesSend += _T("  <ITEM>\r\n");
	m_strMesSend += _T("    <TIME>");
	str1.Format(_T("%04d%02d%02d%02d%02d%02d"), t->tm_year,  t->tm_mon,   t->tm_mday, t->tm_hour,t->tm_min, t->tm_sec);
	//“YYYYMMDDhhmmss”
	m_strMesSend += str1;
	m_strMesSend += _T("</TIME>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");
	m_strMesSend += _T("</EIF>\r\n");	
	
	OnCmdGmesSend();
}

void CAcqVoltageSamples_IntClkDlg::GMESLoopbackdiagnosticsTestAcknowledge(CString lLoopBackString) //S2F26
{
//<EIF VERSION="1.8"ID="S2F26"NAME="LoopbackdiagnosticsTestAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<AS></AS>
//</ITEM></EIF >

	//CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F26\" NAME=\"Loopback diagnostics Test Acknowledge\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	
	m_strMesSend += _T("  <ITEM>\r\n");
	m_strMesSend += _T("    <AS>");
	m_strMesSend += lLoopBackString;
	m_strMesSend += _T("</AS>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");
	m_strMesSend += _T("</EIF>\r\n");	
	
	OnCmdGmesSend();
}


void CAcqVoltageSamples_IntClkDlg::GMESDataandTimeAcknowledge(int lAck) //S2F32
{
//<EIFVERSION="1.8"ID="S2F32"NAME="DataandTimeAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<ACK></ACK>
//</ITEM>
//</EIF>
//Notice
//<EQPID>EquipmentID
//<ACK>AcknowledgeCode
//0=Accepted
//1=NotAccepted

	//CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F32\" NAME=\"Data and Time Acknowledge\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	
	m_strMesSend += _T("  <ITEM>\r\n");
	m_strMesSend += _T("     <ACK>");
	if(lAck == 0)
		m_strMesSend += _T("1");
	else
		m_strMesSend += _T("0");

	m_strMesSend += _T("</ACK>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");
	m_strMesSend += _T("</EIF>");	
	
	OnCmdGmesSend();
}

//int			m_AlarmNumber;
//int			m_AlarmCode[100];
//CString		m_AlarmName[100];
//CString		m_AlarmMessage[100];


void CAcqVoltageSamples_IntClkDlg::GMESAlarmSetReportSend() //S5F1
{
	
//<EIFVERSION="1.8"ID="S5F1"NAME="AlarmSetReportSend">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<SUBITEMLISTCOUNT="n">
//<ALCD></ALCD>
//<ALNAME></ALNAME>
//<ALTX></ALTX>
//</SUBITEMLIST>
//</ITEM>
//</EIF>
//Notice
//<EQPID>EquipmentID
//<SUBITEMLISTCOUNT=“n”>Countof<ALCD>,<ALNAME>,<ALTX>Pair
//<ALCD>AlarmCode
//<ALNAME>AlarmName
//<ALTX>AlarmMessage
//GMES에해당Equipment의ALCD/ALNAME/ALTX가정의되어있는경우
//ALCD만전송함.

	CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S5F1\" NAME=\"Alarm Set Report Send\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	


	m_strMesSend += _T("  <ITEM>\r\n");
//	m_strMesSend += _T("<SUBITEMLISTCOUNT=\"n\">");
	str1.Format(    _T("    <SUBITEMLIST COUNT=\"%d\">\r\n"), m_AlarmNumber);
	m_strMesSend += str1;

	for(int i = 0; i < m_AlarmNumber; i++)
	{
		m_strMesSend += _T("      <ALCD>");//<ALCD>AlarmCode
		str1.Format(_T("%d"), m_AlarmCode[i]);
		m_strMesSend += _T("</ALCD>\r\n");
		m_strMesSend += _T("      <ALNAME>");//<ALNAME>AlarmName
		m_strMesSend += m_AlarmName[i];
		m_strMesSend += _T("</ALNAME>\r\n");
		m_strMesSend += _T("      <ALTX>");//<ALTX>AlarmMessage
		m_strMesSend += m_AlarmMessage[i];
		m_strMesSend += _T("</ALTX>\r\n");
	}
	m_strMesSend += _T("    </SUBITEMLIST>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");
	m_strMesSend += _T("</EIF>");	
	
	OnCmdGmesSend();
}


void CAcqVoltageSamples_IntClkDlg::GMES_Scan_InformSend() //S6F11
{
	if(m_GMES_Skipped == 1)
	{
		return;
	}

	CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S6F11\" NAME=\"Event Report Send\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");
	m_strMesSend += _T("    <CEID>");
	m_strMesSend += g_strCEID;
	m_strMesSend += _T("</CEID>\r\n");
	m_strMesSend += _T("    <RPTID>");
	m_strMesSend += g_strRPTID;
	m_strMesSend += _T("</RPTID>\r\n");
	m_strMesSend += _T("  </ELEMENT>\r\n");	


	m_strMesSend += _T("  <ITEM>\r\n");
	m_strMesSend += _T("    <SUBITEMLIST COUNT=\"1\">\r\n");
	m_strMesSend += _T("      <NAME>PCBAID</NAME>\r\n");
	m_strMesSend += _T("      <VALUE>");
	m_strMesSend += g_strSetID;
	m_strMesSend += _T("</VALUE>\r\n");


	m_strMesSend += _T("    </SUBITEMLIST>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");
	m_strMesSend += _T("</EIF>");	
	
	OnCmdGmesSend();//g_strSetID
	m_EthernetResendCnt = 0;
	m_EthernetAck = 0;
	m_EthernetMode = ETHERNET_MODE_WAIT;
}

void CAcqVoltageSamples_IntClkDlg::GMESProductDataAcknowledge() //S6F6
{
	if(m_GMES_Skipped == 1)
	{
		return;
	}
//	<EIFVERSION="1.8"ID="S6F6"NAME="ProductDataAcknowledge">
//<ELEMENT>




	CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S6F6\" NAME=\"Product Data Acknowledge\">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	//<WOID></WOID>
//<ORGID></ORGID>
//<LINEID></LINEID>
	m_strMesSend += _T("    <EQPID>");//<EQPID></EQPID>
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");

	m_strMesSend += _T("    <PROCID>");
	m_strMesSend += g_strProcID;
	m_strMesSend += _T("</PROCID>\r\n");
	m_strMesSend += _T("    <SETID>");
	m_strMesSend += g_strSetID;
	m_strMesSend += _T("</SETID>\r\n");

	m_strMesSend += _T("    <MODEL>\r\n");//<MODEL>

	m_strMesSend += _T("    <ID>");
	m_strMesSend += g_strModelID;
	m_strMesSend += _T("</ID>\r\n");

	m_strMesSend += _T("    <NAME>");
	m_strMesSend += g_strModelName;
	m_strMesSend += _T("</NAME>\r\n");

	m_strMesSend += _T("    <SUFFIX>");
	m_strMesSend += g_strModelSuffix;
	m_strMesSend += _T("</SUFFIX>\r\n");

	m_strMesSend += _T("    </MODEL>\r\n");//</MODEL>
	m_strMesSend += _T("  </ELEMENT>\r\n");	//</ELEMENT>

	m_strMesSend += _T("  <ITEM>\r\n");//<ITEM>

	m_strMesSend += _T("    <ACK>0</ACK>\r\n");
	m_strMesSend += _T("    <REASON>");
	//m_strMesSend += g_strSetID;
	m_strMesSend += _T("</REASON>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");//</ITEM>
	m_strMesSend += _T("</EIF>");//</EIF>	
	
	OnCmdGmesSend();//g_strSetID
}

void CAcqVoltageSamples_IntClkDlg::GMES_Result_ReportSend() //S6F1
{
	if(m_GMES_Skipped == 1)
	{
		return;
	}

	CString str1;
	m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S6F1\" NAME=\"Inspection(Processing) Data Send \">\r\n");
	m_strMesSend += _T("  <ELEMENT>\r\n");
	m_strMesSend += _T("    <TID>Null</TID>\r\n");	
	m_strMesSend += _T("    <WOID>");
	m_strMesSend += g_strWOID;
	m_strMesSend += _T("</WOID>\r\n");

	m_strMesSend += _T("    <ORGID>");
	m_strMesSend += g_strORGID;
	m_strMesSend += _T("</ORGID>\r\n");


	m_strMesSend += _T("    <LINEID>");
	m_strMesSend += g_strLineID;
	m_strMesSend += _T("</LINEID>\r\n");


	m_strMesSend += _T("    <EQPID>");
	m_strMesSend += g_strEQPID;
	m_strMesSend += _T("</EQPID>\r\n");

	m_strMesSend += _T("    <PROCID>");
	m_strMesSend += g_strProcID;
	m_strMesSend += _T("</PROCID>\r\n");


	m_strMesSend += _T("    <SETID>");
	m_strMesSend += g_strSetID;
	m_strMesSend += _T("</SETID>\r\n");

	
	m_strMesSend += _T("    <MODEL>\r\n");//<MODEL>

	m_strMesSend += _T("    <ID>");
	m_strMesSend += g_strModelID;
	m_strMesSend += _T("</ID>\r\n");

	m_strMesSend += _T("    <NAME>");
	m_strMesSend += g_strModelName;
	m_strMesSend += _T("</NAME>\r\n");

	m_strMesSend += _T("    <SUFFIX>");
	m_strMesSend += g_strModelSuffix;
	m_strMesSend += _T("</SUFFIX>\r\n");

	m_strMesSend += _T("    </MODEL>\r\n");//</MODEL>

	m_strMesSend += _T("  </ELEMENT>\r\n");	


	m_strMesSend += _T("  <ITEM>\r\n");
//	m_strMesSend += _T("<SUBITEMLISTCOUNT=\"n\">");
//	str1.Format(    _T("    <SUBITEMLIST COUNT=\"%d\">\r\n"), m_AlarmNumber);
	str1 =  _T("    <SUBITEMLIST COUNT=\"1\">\r\n");
	m_strMesSend += str1;

	m_strMesSend += _T("      <NAME>RESULT</NAME>\r\n");
	if(m_TEST_Result == 1)
	{
		m_strMesSend += _T("      <VALUE>OK</VALUE>\r\n");
	}
	else
	{
		m_strMesSend += _T("      <VALUE>NG</VALUE>\r\n");
	}

	m_strMesSend += _T("      <NAME>TIME</NAME>\r\n");
	m_strMesSend += _T("      <VALUE>");
	str1 = m_CheckTimeLabel.get_Caption();
	str1 = str1.Left(str1.Find(" (s)"));	
	m_strMesSend += str1;
	m_strMesSend += _T("</VALUE>\r\n");

	
	m_strMesSend += _T("      <NAME>TOTAL_TIME</NAME>\r\n");
	m_strMesSend += _T("      <VALUE>");
	str1 = m_CycleTimeLabel.get_Caption();
	str1 = str1.Left(str1.Find(" (s)"));	
	m_strMesSend += str1;
	m_strMesSend += _T("</VALUE>\r\n");


	if(m_TEST_Result == 0)
	{
		m_strMesSend += _T("      <NAME>NG_ITEM</NAME>\r\n");
		m_strMesSend += _T("      <VALUE>");
		str1 = m_GMES_ErrorName;//m_GMES_ErrorValue;
		m_strMesSend += str1;
		m_strMesSend += _T("</VALUE>\r\n");

		m_strMesSend += _T("      <NAME>NG_VALUE</NAME>\r\n");
		m_strMesSend += _T("      <VALUE>");
		str1 = m_GMES_ErrorValue;
		m_strMesSend += str1;
		m_strMesSend += _T("</VALUE>\r\n");



	}


	m_strMesSend += _T("    </SUBITEMLIST>\r\n");
	m_strMesSend += _T("  </ITEM>\r\n");
	m_strMesSend += _T("</EIF>");	
//	<SUBITEMLIST COUNT=“2">
//<NAME>RESULT</NAME>
//<VALUE>NG</VALUE>
//<NAME>TOTAL_TIME</NAME>
//<VALUE> 37.3</VALUE>
//<NAME>NG_ITEM</NAME>
//<VALUE> AUX_FREQUENCY</VALUE>
//<NAME>NG_VALUE</NAME>
//<VALUE> 0</VALUE>
//</SUBITEMLIST>

	OnCmdGmesSend();
	m_EthernetAck = 0;

	m_EthernetMode = ETHERNET_MODE_WAIT;
	m_EthernetResendCnt = 0;
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
//#define ORA_USING_TNS_NAME _T("DAVMDEV")
//#define ORA_USING_USER_ID _T("oraspc")
//#define ORA_USING_PASSWRD _T("spcora00")
/*
//dev
BYOMINN
BYOMOUT
BYOMT1
BYOMT2
BYOMT3
BYOMT4
BYOMT5
BYOMT6
BYOMT7
BYOMT8
BYOMT9
BYOMT10

//
AGING_LOW
BYOMOUT
BYOMT1
BYOMT10
BYOMT2
BYOMT3
BYOMT4
BYOMT5
BYOMT6
BYOMT7
BYOMT8
BYOMT9
SET_ON_CUR

*/
//
//void CAcqVoltageSamples_IntClkDlg::MesStart()
//{
//	CString sMsg;
//
//	CurrentSet.strMesDsn = g_strMesTns;
//	CurrentSet.strMesUserId = g_strMesUserID;
//	CurrentSet.strMesPassWord = g_strMesPassword;//g_strMesAvn
//	CurrentSet.strMesAvn = g_strMesAvn;
//	CurrentSet.strProductionLine = g_strMesLine;
//	CurrentSet.strStepName = g_strMesInspStep;
//
//	g_MesDb.m_strWipId = m_WipID;
//	m_WipID = _T("");
//
//	if(g_MesDb.m_strWipId.GetLength() < 6)
//	{
//		MessageDisplay(1, _T("BAR CODE EMPTY!! "));
//		m_bStartMES = FALSE;
//		g_MesDb.m_strWipId  = _T("");
//	}
//	else
//	{
//		/*int lresult = g_MesDb.StepCheck();
//		if(lresult == 1)
//		{
//			MessageDisplay(0, g_MesDb.m_strMessage);
//		}
//		else
//		{
//			MessageDisplay(1, g_MesDb.m_strMessage);
//			int lok = MessageBox(_T("MES CHECK FAIL!! TEST CONTINUE??"),  _T("MES FAIL"), MB_YESNO | MB_DEFBUTTON2);
//			if(IDYES != lok)
//			{
//				m_TaskRun = TASK_RUN_IDLE;
//				MessageDisplay(1, _T(" TEST STOP!!"));
//			}
//			
//		}*/
//
//		int lresult =  g_MesDb.Mes_Open_Test(CurrentSet.strMesDsn, CurrentSet.strMesUserId, CurrentSet.strMesPassWord, sMsg);
//		if(lresult == 1)
//		{
//			MessageDisplay(0, sMsg);
//			m_bStartMES = TRUE;
//		}
//		else
//		{
//			MessageDisplay(1, sMsg);
//			m_bStartMES = TRUE;
//		}
//		
//	}	
//	
//}
//
//
//int CAcqVoltageSamples_IntClkDlg::MesTestReport()
//{
//	//CurrentSet.strMesDsn = ORA_USING_TNS_NAME;
//	//CurrentSet.strMesUserId = ORA_USING_USER_ID;
//	//CurrentSet.strMesPassWord = ORA_USING_PASSWRD;
//	//CurrentSet.bCommDisplay = 1;
//	//CurrentSet.strStepName =_T("BYOMT1");
//	int lresult = 0;
//
//#ifdef __G_MES_SET_MODE__
//
//#else //#endif
//	if(gTestSeqData.bMesEnable)
//	{
//		if(m_bStartMES)
//		{
//			if(m_TEST_Result == 1)
//			{
//				lresult = g_MesDb.StepComplete(TRUE);
//			}
//			else
//			{
//				lresult = g_MesDb.StepComplete(FALSE);
//			}
//		}
//	}
//
//#endif
//	//gSaveInfo.bMesSts = lresult;
//	if(lresult == 1)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//	
//
//	//m_bStartMES = TRUE;
//	//OnBnClickedStart();
//}
//


void CAcqVoltageSamples_IntClkDlg::OnBnClickedStart()
{

	CLOSE_Result_Display();
	DisplayClear();
	RefreshTimeOn = 0;

	m_cLbOkNg.put_Caption(_T("WAIT"));
	m_cLbOkNg.put_BackColor(0x0000FFFF);

	m_GMES_CommError = 0;
	//m_WipID = m_BarCode.get_Caption();
	if(gbManualScopeMode)
	{
		MessageBox("MANUAL SCOPE MODE!!");
		return;

	}
		
	if((m_TaskRun != TASK_RUN_IDLE) && (m_TaskRun != TASK_WAIT_SELECT_PROCESS)&& (m_TaskRun != TASK_WAIT_BARCODE_PROCESS))
	{
		return;
	}
	AddLogData("****************START***************");
	if(g_MesEnable)
	{
		if(m_WipID.GetLength() > 5)
		{
			m_GMES_Skipped = 0;
			g_strSetID = m_WipID;
			GMES_Scan_InformSend();//m_WipID

		}
		else
		{
			m_BarCode.put_Caption("");
			MessageDisplay(1, _T("BAR CODE EMPTY!! "));
			//MessageBox("Barcode Not Read !\r\n GMES not Availbele!");
			BarcodeErrorDisplay();
			m_TaskRun = TASK_WAIT_BARCODE_PROCESS;
			return;
			/*if(IDYES != MessageBox("Barcode Not Read !\r\n Contonue test?", "Barcode Error", MB_YESNO))
			{
				MessageDisplay(1, _T("TEST STOP !! "));
					
			}
			else
			{
				m_GMES_Skipped = 1;
			}*/
		}
	}
	else
	{
		m_GMES_Skipped = 1;
		if(m_WipID.GetLength() > 5)
		{
			
			MessageDisplay(1, _T("GMES NOT ENABLE !! "));
		}
		else
		{
			m_BarCode.put_Caption("");
			MessageDisplay(1, _T("BAR CODE EMPTY!! "));
		}
		
		g_strSetID = m_WipID;
		
	}

//	CLOSE_Result_Display();


	//m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//	m_LED_COM_PROCESS = COM_MODE_IDLE;
//	m_PCB_COM_PROCESS = COM_MODE_IDLE;

	
	SetTimeCheck(4);
	SetTimeCheck(6);

	CString str;
	unsigned long lCheck_Time = GetTimeCheck(4);

	str.Format(_T("%.1f (s)"), lCheck_Time/1000.0);
	m_CheckTimeLabel.put_Caption(str);

	lCheck_Time = GetTimeCheck(5);
	if(lCheck_Time > 1000000)
		lCheck_Time = 1000000;

//	g_CycleTime = 
	str.Format(_T("%.1f (s)"), lCheck_Time/1000.0);
	m_CycleTimeLabel.put_Caption(str);
	SetTimeCheck(5);

	CheckTimeOn = 1;
	CycleTimeOn = 1;


//	SetChannelData(0);
//	InitChannel();	
#ifdef __G_MES_SET_MODE__

#else //#endif
	if(gTestSeqData.bMesEnable)
	{
		MesStart();
	}
	else
	{
		m_bStartMES = FALSE;
	}
#endif
	m_TaskRun = TASK_RUN_START;
	m_RunCounter = 0;
	//SetTimer(1,10,NULL);	
	//m_cLb_Test1.put_Caption(_T("WAIT"));
	//m_cLb_Test1.put_BackColor(0x0000FFFF);
	//
	//m_cLb_Test2.put_Caption(_T("WAIT"));
	//m_cLb_Test2.put_BackColor(0x0000FFFF);
	//
	//m_cLb_Test3.put_Caption(_T("WAIT"));
	//m_cLb_Test3.put_BackColor(0x0000FFFF);

	


	//m_OkNgLabel.put_Text(_T("WAIT"));
	//m_OkNgLabel.put_BackColor(0x0000FFFF);
	
	//m_cLbOkNg.put_Caption(_T("WAIT"));//m_BarCode
	//m_cLbOkNg.put_BackColor(0x0000FFFF);


	//	
	//m_cLb_Lan.put_Caption(_T("WAIT"));
	//m_cLb_Lan.put_BackColor(0x0000FFFF);
	//m_cLb_Usb1.put_Caption(_T("WAIT"));
	//m_cLb_Usb1.put_BackColor(0x0000FFFF);
	//m_cLb_Usb2.put_Caption(_T("WAIT"));
	//m_cLb_Usb2.put_BackColor(0x0000FFFF);


	
	for(int  i = 0; i < 6; i++)
	{
		m_CheckEnable[i] = 0;
	}

	for(int  i = 0; i < gTestSeqData.TestCnt; i++)
	{		

		m_CtrlListMainProcess.SetItem(i, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);   
		m_CtrlListMainProcess.SetItem(i, 3, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   	
	//	m_CtrlListTestProcess.SetItem(i, 9, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);   

		if(m_CtrlListMainProcess.GetCheck(i))
		{
			gTestSeqData.TestStepList[i].bEnable = 1;			
		}
		else
		{
			if(g_MesEnable)
			{
				gTestSeqData.TestStepList[i].bEnable = 1;	
				m_CtrlListMainProcess.SetCheck(i);
			}
			else
			{
				gTestSeqData.TestStepList[i].bEnable = 0;
			}
		}
		m_CtrlListMainProcess.Update(i);

		gTestSeqData.TestStepList[i].m_Result = 1;
		
	}
	m_strButtonMessage1 = "Ready 버튼을 누르시오.";
	m_strButtonMessage2 = "Exposure 버튼을 누르시오.";


	gTestSeqData.m_TotalResult = 3;

	m_nCurrentNumber = 0;
	m_nCurrentProcess = 0;
	for(int i = m_nCurrentNumber;i < gTestSeqData.TestCnt; i++)
	{	
	
		m_nCurrentNumber = i;
		if(gTestSeqData.TestStepList[i].bEnable == 0)
		{
		}
		else
		{
			
			break;
		}
	}
	m_CtrlListMainProcess.Invalidate();

	//m_CtrlListMainProcess.click

	
	m_NG_Retry_Count = 0;
	m_NG_WaitFlag = 0;

	SetTimeCheck(1);
}



void CAcqVoltageSamples_IntClkDlg::CallProcess()
{
	//if(m_TEST_Result == 0)//NG
	//{
	//	TestFinish();
	//	return;
	//}
	
	switch(m_nCurrentStep)
	{
	case 0:
		m_nCurrentStep = 1;
		m_InterProcess = 0;
		SetFunction();
		m_Deisplay_ErrorName = "";					
		m_Deisplay_ErrorDetail = "";
		break;
	case 1:
		RunFunction();
		break;
	case 2:
		//GetFunction();m_nCurrentNumber

		m_nCurrentStep = 0;
		m_nCurrentProcess++;
		if(( m_nCurrentProcess >= gTestSeqData.TestStepList[m_nCurrentNumber].nCount )||(gTestSeqData.TestStepList[m_nCurrentNumber].m_Result == 0))
		{
		
			
			if(gTestSeqData.TestStepList[m_nCurrentNumber].m_Result == 1) 
			{
				m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
			}
			else
			{
				m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "NG", NULL, NULL, NULL, NULL);

				
			//	if(IDYES == MessageBox("NG RETRY?" , "NG" , MB_YESNO))
			//	{
			//		m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);
			//		//m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
			//		m_nCurrentProcess = 0;
			////		if(GetTimeCheck(1) > m_CheckDelayTime)
		
			//	}
			//	else
			//	{
			//		TestFinish();
			//	}
				NG_Display(m_GMES_ErrorName);
				m_TaskRun = TASK_WAIT_SELECT_PROCESS;
				
				//m_NG_Retry_Count++;//m_NG_WaitFlag = 0;
				
				break;
			}

			unsigned long lCheck_Time = GetTimeCheck(6);
			CString str;
			str.Format(_T("%.1f"), lCheck_Time/1000.0);
			m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 3, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
			SetTimeCheck(6);

			m_nCurrentNumber++;
	
			int i;
			for( i = m_nCurrentNumber;i < gTestSeqData.TestCnt; i++)
			{
				if(gTestSeqData.TestStepList[i].bEnable == 0)
				{
				}
				else
				{
					break;
				}
			}
			m_nCurrentNumber = i;


			if(m_nCurrentNumber >= gTestSeqData.TestCnt)
			{
				TestFinishGMES();//TestFinish();
			}
			else
			{
				m_nCurrentProcess = 0;		
				for(int  i = 0; i < 6; i++)
				{
					m_CheckEnable[i] = 0;
				}
			}
			m_CtrlListMainProcess.Invalidate();

			m_NG_Retry_Count = 0;
			m_NG_WaitFlag = 0;
		}
		break;
	default:
		break;
	}

	//m_OKcnt = 0;
	//SetTimeCheck(1);
}

	//void CAcqVoltageSamples_IntClkDlg::SetFunction()
	//void CAcqVoltageSamples_IntClkDlg::RunFunction()

void CAcqVoltageSamples_IntClkDlg::SetFunction()
{
	CString lFunctionString;
	CString strtemp;
	
	m_FunctionType = TEST_SEQ_TYPE_END;

	lFunctionString = gTestSeqData.TestStepList[m_nCurrentNumber].FunctionCommand[m_nCurrentProcess];
	int lpos, lendpos;
	lpos = lFunctionString.Find("(");
	lendpos = lFunctionString.Find(")");
	lFunctionString = lFunctionString.Left(lendpos);
	m_FunctionName = lFunctionString.Left(lpos);
	m_FunctionName = m_FunctionName.MakeLower();
	m_FunctionParameter = lFunctionString.Mid(lpos+1);

	if(m_FunctionName.Compare("delay") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_DELAY;
		m_CheckDelayTime = _ttoi(m_FunctionParameter);
		if(m_CheckDelayTime < 0)
		{
			m_CheckDelayTime = 1000;
		}
	}//TEST_SEQ_TYPE_BEEP
	else if(m_FunctionName.Compare("beep") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_BEEP;

		int lpos, lendpos;
		DWORD lFreq,lDelay;


		//lpos = m_FunctionParameter.Find(",");	

		//CString str1 = m_FunctionParameter.Left(lpos);
		//CString str2 = m_FunctionParameter.Mid(lpos+1);
		////
		////lFreq = _ttoi(str1);
		//lDelay = _ttoi(str2);
		////if(lFreq > 2000)
		////{
		////	lFreq = 800;
		////}
		////if(lDelay > 5000)
		////{
		////	lDelay = 300;
		////}
		//Beep(1200, 1000);
		//mpDlgSimpleInform->m_DisplayString = str1;
		//mpDlgSimpleInform->m_Count = lDelay;
		//CRect rectP;
		//CRect rect;		
		//
		//this->GetWindowRect(&rectP);
		//mpDlgSimpleInform->GetWindowRect(&rect);

		//mpDlgSimpleInform->ShowWindow(SW_SHOW);
		//mpDlgSimpleInform->SetWindowPos(this,rectP.left+ 300,rectP.top+350,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);
	
		//Beep(1200, 1000);
		//m_nCurrentStep = 2;
	}
	else if(m_FunctionName.Compare("user_message") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_CHECK_MESSAGE_MANUAL;
		int lpos, lendpos;

		lpos = m_FunctionParameter.Find("\"");	

		CString str1 = m_FunctionParameter.Mid(lpos+1);
		lpos = str1.Find("\"");
		CString str2 = str1.Left(lpos);
		m_strUserMessage = str2; 



		str2 = str1.Mid(lpos+1);		
		lpos = str2.Find(",");
		str2 = str2.Mid(lpos+1);

		m_intUserMessageTime = _ttoi(str2);

		//m_intUserMessageTime = lDelay;
		//lpos = m_FunctionParameter.Find(",");
		//strtemp = m_FunctionParameter.Mid(lpos+1);
		//
		//m_strImageName = strtemp;

		if(m_intUserMessageTime < 50)
		{
			m_intUserMessageTime = 1000;
		}

		if(m_CheckDelayTime < 50)
		{
			m_CheckDelayTime = 1000;
		}
	}
	else if(m_FunctionName.Compare("get_sw_version") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_SW_VERSION;
		int lpos, lendpos;
		

		strtemp = m_FunctionParameter;//	str1 = m_FunctionParameter.Left(lpos);
		m_CheckDelayTime = _ttoi(strtemp);

		if(m_CheckDelayTime < 50)
		{
			m_CheckDelayTime = 1000;
		}
	}
	else if(m_FunctionName.Compare("wifi_module_check") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_CHECK_WIFI;
		int lpos, lendpos;
		
		
	
		strtemp = m_FunctionParameter;//	str1 = m_FunctionParameter.Left(lpos);
		m_CheckDelayTime = _ttoi(strtemp);

		////////////////////////////////////////////////
		if(m_CheckDelayTime < 50)
		{
			m_CheckDelayTime = 1000;
		}
	}	

	else if(m_FunctionName.Compare("get_dark_image") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_GET_DARK_IMAGE;
		int lpos, lendpos;
	
		lpos = m_FunctionParameter.Find(",");	

		CString str1 = m_FunctionParameter.Left(lpos);
		CString str2 = m_FunctionParameter.Mid(lpos+1);
		m_ImageMin = _ttoi(str1);

		lpos = str2.Find(",");	
		str1 = str2.Left(lpos);
		str2 = str2.Mid(lpos+1);
		m_ImageMax  = _ttoi(str1);
		strtemp = str2;//str1 = m_FunctionParameter.Left(lpos);

		m_CheckDelayTime = _ttoi(strtemp);
		if(m_CheckDelayTime < 50)
		{
			m_CheckDelayTime = 1000;
		}
	}
	else if(m_FunctionName.Compare("get_bright_image") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_GET_BRIGHT_IMAGE;
		int lpos, lendpos;
	
		lpos = m_FunctionParameter.Find(",");	

		CString str1 = m_FunctionParameter.Left(lpos);
		CString str2 = m_FunctionParameter.Mid(lpos+1);
		m_ImageMin = _ttoi(str1);

		lpos = str2.Find(",");	
		str1 = str2.Left(lpos);
		str2 = str2.Mid(lpos+1);
		m_ImageMax  = _ttoi(str1);
		strtemp = str2;//str1 = m_FunctionParameter.Left(lpos);

		m_CheckDelayTime = _ttoi(strtemp);
		if(m_CheckDelayTime < 50)
		{
			m_CheckDelayTime = 1000;
		}
	}
	else if(m_FunctionName.Compare("get_mac_address") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_GET_MACADDRESS;
		int lpos, lendpos;

		lpos = m_FunctionParameter.Find(",");	

		CString str1 = m_FunctionParameter.Left(lpos);
		CString str2 = m_FunctionParameter.Mid(lpos+1);
		m_RefMacAddress = str1;

	
		strtemp = str2;//str1 = m_FunctionParameter.Left(lpos);

		m_CheckDelayTime = _ttoi(strtemp);
		if(m_CheckDelayTime < 50)
		{
			m_CheckDelayTime = 1000;
		}
	}
	else if(m_FunctionName.Compare("get_aed_value") == 0)
	{


		m_FunctionType = TEST_SEQ_TYPE_GET_AED_VALUE;
		int lpos, lendpos;

#if 1	
		CString str1;
		CString str2;
		str2 = m_FunctionParameter;

		
		for(int i =  0; i < 7; i++)
		{
			lpos = str2.Find(",");	
			str1 = str2.Left(lpos);
			str2 = str2.Mid(lpos+1);
			m_AED_SensorMin[i] = _ttoi(str1);

			lpos = str2.Find(",");	
			str1 = str2.Left(lpos);
			str2 = str2.Mid(lpos+1);
			m_AED_SensorMax[i]  = _ttoi(str1);
		}

		//lpos = str2.Find(",");	
		//str1 = str2.Left(lpos);
		//str2 = str2.Mid(lpos+1);
		//m_AED_SensorMax  = _ttoi(str1);


		strtemp = str2;//str1 = m_FunctionParameter.Left(lpos);
#else
		lpos = m_FunctionParameter.Find(",");	

		CString str1 = m_FunctionParameter.Left(lpos);
		CString str2 = m_FunctionParameter.Mid(lpos+1);
		m_AED_SensorMin = _ttoi(str1);

		lpos = str2.Find(",");	
		str1 = str2.Left(lpos);
		str2 = str2.Mid(lpos+1);
		m_AED_SensorMax  = _ttoi(str1);
		strtemp = str2;//str1 = m_FunctionParameter.Left(lpos);
#endif
	
		m_CheckDelayTime = _ttoi(strtemp);
		if(m_CheckDelayTime < 50)
		{
			m_CheckDelayTime = 1000;
		}
	}
	//		TEST_SEQ_TYPE_SET_MANUAL_EXPOSURE_MODE	34
//#define TEST_SEQ_TYPE_FACTORY_RESET				35
	else if(m_FunctionName.Compare("set_manual_exposure") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_SET_MANUAL_EXPOSURE_MODE;
		m_CheckDelayTime = _ttoi(m_FunctionParameter);
		if(m_CheckDelayTime < 0)
		{
			m_CheckDelayTime = 1000;
		}
	}//TEST_SEQ_TYPE_BEEP
	else if(m_FunctionName.Compare("set_auto_exposure") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_SET_AUTO_EXPOSURE_MODE;
		m_CheckDelayTime = _ttoi(m_FunctionParameter);
		if(m_CheckDelayTime < 0)
		{
			m_CheckDelayTime = 1000;
		}
	}//TEST_SEQ_TYPE_BEEP
	else if(m_FunctionName.Compare("set_button_message1") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_SET_MESSAGE_1;
		int lpos, lendpos;

		lpos = m_FunctionParameter.Find("\"");

		CString str1 = m_FunctionParameter.Mid(lpos+1);
		lpos = str1.Find("\"");
		CString str2 = str1.Left(lpos);

		m_strButtonMessage1 = str2;//		
	}	
	else if(m_FunctionName.Compare("set_button_message2") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_SET_MESSAGE_2;
		int lpos, lendpos;

		lpos = m_FunctionParameter.Find("\"");

		CString str1 = m_FunctionParameter.Mid(lpos+1);
		lpos = str1.Find("\"");
		CString str2 = str1.Left(lpos);

		m_strButtonMessage2 = str2;//	
	}
	else if(m_FunctionName.Compare("factory_reset") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_FACTORY_RESET;
		m_CheckDelayTime = _ttoi(m_FunctionParameter);
		if(m_CheckDelayTime < 0)
		{
			m_CheckDelayTime = 1000;
		}
	}//TEST_SEQ_TYPE_BEEP	
	else if (m_FunctionName.Compare("set_model_name") == 0)
	{
		m_FunctionType = TEST_SEQ_TYPE_SET_MODEL_NAME;
		m_CheckDelayTime = _ttoi(m_FunctionParameter);
		if (m_CheckDelayTime < 0)
		{
			m_CheckDelayTime = 1000;
		}
	}//TEST_SEQ_TYPE_BEEP	


	m_OKcnt = 0;
	SetTimeCheck(1);
}

void CAcqVoltageSamples_IntClkDlg::MessageInformDisplay( CString lMessage, int lTime)//void CAcqVoltageSamples_IntClkDlg::MessageInformClose()
{
	CRect rectP;
	CRect rect;		
	mpDlgSimpleInform->m_DisplayString = lMessage;
	mpDlgSimpleInform->m_Count = lTime;
		
		
	this->GetWindowRect(&rectP);
	mpDlgSimpleInform->GetWindowRect(&rect);

	mpDlgSimpleInform->m_Result = 0;

	mpDlgSimpleInform->ShowWindow(SW_SHOW);
	mpDlgSimpleInform->SetWindowPos(this,rectP.left+ 300,rectP.top+350,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);

}

void CAcqVoltageSamples_IntClkDlg::MessageInformClose()
{
			//	CRect rectP;
	//CRect rect;		
			

	mpDlgSimpleInform->ShowWindow(SW_HIDE);
	mpDlgSimpleInform->m_DisplayString = "";
	mpDlgSimpleInform->m_Count = 0;
		

}


void CAcqVoltageSamples_IntClkDlg::RunFunction()
{
	CRect rectP;
	CRect rect;		

	static int sWaitCnt = 0;
	if(sWaitCnt > 0)
		sWaitCnt--;
	//if(m_TEST_Result == 0)//NG
	//{
	//	TestFinish();
	//	return;
	//}
	switch(m_FunctionType)//m_nCurrentNumber
	{
	case TEST_SEQ_TYPE_DELAY:
		if(GetTimeCheck(1) > m_CheckDelayTime)
		{
			SetTimeCheck(1);
			m_nCurrentStep = 2;
			
		}

		break;


	case  TEST_SEQ_TYPE_BEEP:
		

		Beep(1200, 1000);
		m_nCurrentStep = 2;
	
		break;

	case  TEST_SEQ_TYPE_CHECK_MESSAGE_MANUAL:
		
//		MessageDisplay(0, "TEST_SEQ_TYPE_MP_KEY!!");
//		SendIR_Code(m_strMP_Key, 3);

		if(m_InterProcess == 0)
		{

			//	CRect rectP;
	//CRect rect;		
			mpDlgSimpleInform->m_DisplayString = m_strUserMessage;
			mpDlgSimpleInform->m_Count = m_intUserMessageTime;
		
		
			this->GetWindowRect(&rectP);
			mpDlgSimpleInform->GetWindowRect(&rect);

			mpDlgSimpleInform->m_Result = 0;

			mpDlgSimpleInform->ShowWindow(SW_SHOW);
			mpDlgSimpleInform->SetWindowPos(this,rectP.left+ 300,rectP.top+350,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);
			m_InterProcess = 1;

		}
		else if(m_InterProcess == 1)
		{

			if(mpDlgSimpleInform->m_Result == 1)
			{
				gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;
				m_nCurrentStep = 2;
			}
			else if(mpDlgSimpleInform->m_Result == 2)
			{
				m_nCurrentStep = 2;
				m_Deisplay_ErrorName = "USER STOP";
				//m_Deisplay_ErrorName += "\r\nFPGA VER ERROR";
				m_Deisplay_ErrorDetail = "";
				gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
			}
			else
			{
				if(GetTimeCheck(1) > m_intUserMessageTime)
				{
					mpDlgSimpleInform->ShowWindow(SW_HIDE);
					m_nCurrentStep = 2;
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;
					m_Deisplay_ErrorName = "OK";					
					m_Deisplay_ErrorDetail = "";
				}
			}
		}

		
	
		break;
		
	case   TEST_SEQ_TYPE_SW_VERSION:

		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(GetDetParam() )
				{

					g_DSERVER_Ver_String.Format(_T("%d.%d%d.%02d"),
						g_DSERVER_Ver_event, g_DSERVER_Ver_major, g_DSERVER_Ver_minor, g_DSERVER_Ver_release);
					m_DSERVER_Ver_String.Format(_T("%d.%d%d.%02d"),
						m_DSERVER_Ver_event, m_DSERVER_Ver_major, m_DSERVER_Ver_minor, m_DSERVER_Ver_release);
					if((g_FPGA_Ver_event ==		m_FPGA_Ver_event)	
						&&(g_FPGA_Ver_major == 		m_FPGA_Ver_major)
						&&(g_FPGA_Ver_minor == 		m_FPGA_Ver_minor)	
						&& (g_DSERVER_Ver_String == m_DSERVER_Ver_String)
						/*&&(g_DSERVER_Ver_event == 	m_DSERVER_Ver_event)
						&&(g_DSERVER_Ver_major == 	m_DSERVER_Ver_major)
						&&(g_DSERVER_Ver_minor == 	m_DSERVER_Ver_minor)
						&&(g_DSERVER_Ver_release == m_DSERVER_Ver_release)*/					
						&&(g_Microchip_Ver_event == m_Microchip_Ver_event)
						&&(g_Microchip_Ver_major == m_Microchip_Ver_major)
						&&(g_Microchip_Ver_minor == m_Microchip_Ver_minor)					
						&&(g_AVR_Ver_event ==		m_AVR_Ver_event)
						&&(g_AVR_Ver_major == 		m_AVR_Ver_major)
						&&(g_AVR_Ver_minor == 		m_AVR_Ver_minor))
					{
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;
					}
					else
					{
						if((g_FPGA_Ver_event !=		m_FPGA_Ver_event)	
						||(g_FPGA_Ver_major != 		m_FPGA_Ver_major)
						||(g_FPGA_Ver_minor != 		m_FPGA_Ver_minor)	)
						{
							m_Deisplay_ErrorName = "GET SW VER ERROR";
							m_Deisplay_ErrorName += "\r\nFPGA VER ERROR";
							m_Deisplay_ErrorDetail.Format(_T("FPGA VER: REF=>(%d.%d.%d) READ=>(%d.%d.%d)"), 
								g_FPGA_Ver_event, g_FPGA_Ver_major, g_FPGA_Ver_minor, 
								m_FPGA_Ver_event, m_FPGA_Ver_major, m_FPGA_Ver_minor);
						}
					/*	else if((g_DSERVER_Ver_event != 	m_DSERVER_Ver_event)
						||(g_DSERVER_Ver_major != 	m_DSERVER_Ver_major)
						||(g_DSERVER_Ver_minor != 	m_DSERVER_Ver_minor)
						||(g_DSERVER_Ver_release != m_DSERVER_Ver_release))*/
						else if(g_DSERVER_Ver_String == m_DSERVER_Ver_String)
						{
							m_Deisplay_ErrorName = "GET SW VER ERROR";
							m_Deisplay_ErrorName += "\r\nDXD SERVER VER ERROR";
							m_Deisplay_ErrorDetail.Format(_T("DXD SERVER VER: REF=>(%d.%d%d.%02d) READ=>(%d.%d%d.%02d)"), 
								g_DSERVER_Ver_event, g_DSERVER_Ver_major, g_DSERVER_Ver_minor,g_DSERVER_Ver_release, 
								m_DSERVER_Ver_event, m_DSERVER_Ver_major, m_DSERVER_Ver_minor, m_DSERVER_Ver_release);
						}
						else if((g_Microchip_Ver_event != m_Microchip_Ver_event)
						||(g_Microchip_Ver_major != m_Microchip_Ver_major)
						||(g_Microchip_Ver_minor != m_Microchip_Ver_minor))
						{
							m_Deisplay_ErrorName = "GET SW VER ERROR";
							m_Deisplay_ErrorName += "\r\nMicrochip VER ERROR";
							m_Deisplay_ErrorDetail.Format(_T("Microchip VER: REF=>(%d.%d.%d) READ=>(%d.%d.%d)"), 
								g_Microchip_Ver_event, g_Microchip_Ver_major, g_Microchip_Ver_minor, 
								m_Microchip_Ver_event, m_Microchip_Ver_major, m_Microchip_Ver_minor);
						}
						else if((g_AVR_Ver_event !=		m_AVR_Ver_event)
						||(g_AVR_Ver_major != 		m_AVR_Ver_major)
						||(g_AVR_Ver_minor != 		m_AVR_Ver_minor))
						{
							m_Deisplay_ErrorName = "GET SW VER ERROR";
							m_Deisplay_ErrorName += "\r\nAVR VER ERROR";
							m_Deisplay_ErrorDetail.Format(_T("AVR VER: REF=>(%d.%d.%d) READ=>(%d.%d.%d)"), 
								g_AVR_Ver_event, g_AVR_Ver_major, g_AVR_Ver_minor, 
								m_AVR_Ver_event, m_AVR_Ver_major, m_AVR_Ver_minor);
						}
						else
						{
							m_Deisplay_ErrorName = "GET SW VER ERROR";
							m_Deisplay_ErrorName += "\r\nOTHERS VER ERROR";
							m_Deisplay_ErrorDetail = "";
						}
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
					}
					
					
					m_nCurrentStep = 2;
					
				}
				else
				{
					if(GetTimeCheck(1) > m_CheckDelayTime)
					{
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
						m_Deisplay_ErrorName = "GET SW VER ERROR";
						m_Deisplay_ErrorName += "\r\nTIME OUT";
						m_Deisplay_ErrorDetail = "";
					}
					else
					{
						sWaitCnt = 100;
					}					
				
				}
			}		
		}

		

		break;

	case   TEST_SEQ_TYPE_CHECK_WIFI:
	//	SetChannelDataRangeSeq(m_nChannel,m_TmpVoltageMin,m_TmpVoltageMax,m_TmpFrequencyMin,m_TmpFrequencyMax);
		
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(WifiCheck() )
				{
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;			
					
					
					m_nCurrentStep = 2;
					//m_Deisplay_ErrorName = "OK";
					////	m_Deisplay_ErrorName += "\r\nTIME OUT";
					//	m_Deisplay_ErrorDetail = "";
					
				}
				else
				{
					if(GetTimeCheck(1) > m_CheckDelayTime)
					{
						m_Deisplay_ErrorName = "WiFi Module Check ERROR";
						m_Deisplay_ErrorName += "\r\nDisconnected";
						m_Deisplay_ErrorDetail = "Disconnected";
					
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
					}
					else
					{
						sWaitCnt = 100;
					}					
				
				}
			}		
		}

		

		
		

		break;
//	#define TEST_SEQ_TYPE_GET_DARK_IMAGE			30
//#define TEST_SEQ_TYPE_GET_BRIGHT_IMAGE			31
//#define TEST_SEQ_TYPE_GET_AED_VALUE				32

	case   TEST_SEQ_TYPE_GET_DARK_IMAGE:
				
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(GetdarkCheck(m_ImageMin, m_ImageMax))//
				{
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;
					
					
					//m_Deisplay_ErrorName = "OK";
					////	m_Deisplay_ErrorName += "\r\nTIME OUT";
					//	m_Deisplay_ErrorDetail = "";
					m_nCurrentStep = 2;
					AddLogData("Get Dark Check OK");
					m_Deisplay_ErrorDetail = "ROIC:";
					AddLogData(m_Deisplay_ErrorDetail);
					CString sTemp;
					for (int i = 0; i < gNoRoic; i++)
					{
						sTemp.Format(" %d:%d[%d~%d]", i, g_RoicMean[i], m_ImageMin, m_ImageMax);
						AddLogData(sTemp);
						m_Deisplay_ErrorDetail += sTemp;

					}
					AddLogData(m_Deisplay_ErrorDetail);

					m_Deisplay_ErrorDetail = "GATE:";
					AddLogData(m_Deisplay_ErrorDetail);
					for (int i = 0; i < gNoGate; i++)
					{
						sTemp.Format(" %d:%d[%d~%d]", i, g_GateMean[i], m_ImageMin, m_ImageMax);
						AddLogData(sTemp);
						m_Deisplay_ErrorDetail += sTemp;
					}
					AddLogData(m_Deisplay_ErrorDetail);
					
				}
				else
				{
					//if(GetTimeCheck(1) > m_CheckDelayTime)
					{
						m_Deisplay_ErrorName = "Get Dark Check ERROR";
						m_Deisplay_ErrorName += "\r\nGet Dark Image Fail";
						m_Deisplay_ErrorDetail = "Get Dark Image Fail";
					
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
					}
					CString sTemp ;
					m_Deisplay_ErrorDetail += "ROIC:";
					AddLogData("ROIC");
					for (int i = 0; i < gNoRoic; i++)
					{
						if (m_RoicAvgNG[i] == 1)
						{
							sTemp.Format(" %d:NG %d[%d~%d]", i, g_RoicMean[i], m_ImageMin, m_ImageMax);
							AddLogData(sTemp);
							m_Deisplay_ErrorDetail += sTemp;
						}
						/*else
						{
							sTemp.Format(" %d:OK %d[%d~%d]", i, g_RoicMean[i], m_ImageMin, m_ImageMax);
						}*/
						

					}
					m_Deisplay_ErrorDetail += "\nGATE:";
					AddLogData("GATE");
					for (int i = 0; i < gNoGate; i++)
					{
						//sTemp.Format(" %d:%d[%d~%d]", i, g_GateMean[i], m_ImageMin, m_ImageMax);
						if (m_GateAvgNG[i] == 1)
						{
							sTemp.Format(" %d:NG %d[%d~%d]", i, g_GateMean[i], m_ImageMin, m_ImageMax);
							AddLogData(sTemp);
							m_Deisplay_ErrorDetail += sTemp;
						}
				/*		else
						{
							sTemp.Format(" %d:OK %d[%d~%d]", i, g_GateMean[i], m_ImageMin, m_ImageMax);
						}*/
						
					}

					//AddLogData(m_Deisplay_ErrorDetail);
					AddLogData(m_Deisplay_ErrorName);
					AddLogData(m_Deisplay_ErrorDetail);
				}
			}		
		}


		break;

	case   TEST_SEQ_TYPE_GET_BRIGHT_IMAGE:
	//	OnBnClickedBtnGetbright()
	//	m_ImageAvg
		if(m_InterProcess == 0)
		{
		//	MessageInformDisplay("S I Z E T E S T 1 2 3 4 5 6 7 8 9 0", 10000);
			sWaitCnt = 50;
			m_InterProcess = 1;
			m_SetComError = 0;
			m_SetComFinished = 0;
			//m_ImageAvg = 0;
		}	
		else if(m_InterProcess == 1)
		{	
			if(sWaitCnt == 0)
			{	
				if (m_detParam.dwExposureMode != EXP_MODE_AUTO)
				{
					sWaitCnt = 50;
					m_InterProcess = 2;
				}
				else
				{
					if (GetAedSensor())
					{
						int lNg = 0;
						for (int i = 0; i < 7; i++)
						{

							if (m_AED_Sensor[i] <= 100)
							{
								m_cLb_AED_Sensor[i].put_ForeColor(0x00FF00);
							}
							else
							{
								lNg = 1;
								m_cLb_AED_Sensor[i].put_ForeColor(0x0000FF);
							}
						}
						if (lNg)
						{
							m_Deisplay_ErrorName = "Get Bright Check ERROR";
							m_Deisplay_ErrorName += "\r\nAED Sensor Check Value Over";
							CString sTemp;
							m_Deisplay_ErrorDetail = "AED READ:";
							for (int i = 0; i < 7; i++)
							{
								sTemp.Format("[%d]:%d,", i, m_AED_Sensor[i]);
								m_Deisplay_ErrorDetail += sTemp;
							}
							//AddLogData("");
							gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
							m_nCurrentStep = 2;

						}
						else
						{
							sWaitCnt = 50;
							m_InterProcess = 2;
						}
					}
					else
					{
						m_Deisplay_ErrorName = "Get Bright Check ERROR";
						m_Deisplay_ErrorName += "\r\nAED Sensor Check Error";
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
					}
				}
			}
		}
		else if(m_InterProcess == 2)
		{
			
			if(sWaitCnt == 0)
			{
				sWaitCnt = 50;
				m_InterProcess = 3;
				OnBnClickedBtnGetbright();
			}
		}
		else if(m_InterProcess == 3)
		{
			if(sWaitCnt == 0)
			{
				
				if(m_SetComFinished == 1)//int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
				{					
					//if((m_SetComError == 0)&&(m_ImageAvg > m_ImageMin)&&(m_ImageAvg < m_ImageMax))//int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
					if((m_SetComError == 0)&&(m_TotalNG == 0))//int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
					{
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;
						m_nCurrentStep = 2;
						
						AddLogData("Get Bright Check OK");
						//m_Deisplay_ErrorDetail.Format("m_ImageAvg:%d[Min:%d, Max:%d]", m_ImageAvg, m_ImageMin, m_ImageMax);
						//
						CString sTemp;
						for (int i = 0; i < gNoRoic; i++)
						{
							sTemp.Format(" %d:%d[%d~%d]", i, g_RoicMean[i], g_RefMin_buf[i], g_RefMax_buf[i]);
							m_Deisplay_ErrorDetail += sTemp;
							
						}
						AddLogData(m_Deisplay_ErrorDetail);
					}
					else
					{
						OnBnClickedBtnCancel2();
						m_Deisplay_ErrorName = "Get Bright Check ERROR";
						if (m_SetComError == 1)
						{
							m_Deisplay_ErrorName += "\r\nSet Communition Fail";
							//m_Deisplay_ErrorDetail = "Get Bright Image Fail";
						}
						else if (m_TotalNG > 0) // (m_ImageAvg > m_ImageMin) && (m_ImageAvg < m_ImageMax))
						{
							m_Deisplay_ErrorName += "\r\nGet Bright Image Average Fail";
							m_Deisplay_ErrorDetail = "CHECK :";
							CString sTemp;
							for (int i = 0; i < gNoRoic; i++)
							{
								if (m_RoicAvgNG[i] == 1)
								{
									sTemp.Format("ROIC %d:NG %d[%d~%d]", i, g_RoicMean[i], g_RefMin_buf[i], g_RefMax_buf[i]);
									m_Deisplay_ErrorDetail += sTemp;
								}
								else
								{
								//	sTemp.Format(" %d:OK %d[%d~%d]", i, g_RoicMean[i], g_RefMin_buf[i], g_RefMax_buf[i]);
								}
							}
							
							for (int i = 0; i < gNoGate; i++)
							{
								if (m_GateAvgNG[i] == 1)
								{
									sTemp.Format("GATE %d:NG %d[%d~%d]", i, g_GateMean[i], g_RefMin_buf[i], g_RefMax_buf[i]);
									m_Deisplay_ErrorDetail += sTemp;
								}
								else
								{
								//	sTemp.Format(" %d:OK %d[%d~%d]", i, g_RoicMean[i], g_RefMin_buf[i], g_RefMax_buf[i]);
								}
							}						
						}
					
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;

						AddLogData(m_Deisplay_ErrorName);
						AddLogData(m_Deisplay_ErrorDetail);
					}
					
				}
				else
				{
					if(GetTimeCheck(1) > m_CheckDelayTime)
					{
						OnBnClickedBtnCancel2();
						m_Deisplay_ErrorName = "Get Bright Check ERROR";
						m_Deisplay_ErrorName += "\r\nGet Bright Image Fail";
						m_Deisplay_ErrorDetail = "Get Bright Image Time Out";
					
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
						AddLogData(m_Deisplay_ErrorName);
						AddLogData(m_Deisplay_ErrorDetail);

					}
					else
					{
						sWaitCnt = 100;
					}					
				
				}
			}		
		}

		break;	

	case   TEST_SEQ_TYPE_GET_AED_VALUE:
		
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(GetAedSensor() )
				{
					int lNg = 0;
					for(int i = 0; i < 7; i++)
					{
#if 1
						if(((m_AED_Sensor[i]  >=	m_AED_SensorMin[i] )&&(m_AED_Sensor[i]  <=	m_AED_SensorMax[i] ))
							||(m_AED_SensorMin[i] ==	m_AED_SensorMax[i] ))
						{
							m_cLb_AED_Sensor[i].put_ForeColor(0x00FF00);
						}
						else
						{
							lNg = 1;
							m_cLb_AED_Sensor[i].put_ForeColor(0x0000FF);
						}

#else
						if((m_AED_Sensor[i]  <	m_AED_SensorMin[i] )||(m_AED_Sensor[i]  >	m_AED_SensorMax[i] ))
						{
							lNg = 1;
							m_cLb_AED_Sensor[i].put_ForeColor(0x0000FF);
						}
						else
						{
							m_cLb_AED_Sensor[i].put_ForeColor(0x00FF00);
						}

#endif
					}
					if(lNg)
					{
						m_Deisplay_ErrorName = "AED SENSOR Check ERROR";
						m_Deisplay_ErrorName += "\r\nAED SENSOR Value";
						m_Deisplay_ErrorDetail = "AED SENSOR Value Error";
					
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
						//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;	
					}
					else
					{
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;	
						m_nCurrentStep = 2;
					
					
					}
					

				}
				else
				{
					//if(GetTimeCheck(1) > m_CheckDelayTime)
					{
						m_Deisplay_ErrorName = "AED SENSOR Check ERROR";
						m_Deisplay_ErrorName += "\r\nAED SENSOR Value";
						m_Deisplay_ErrorDetail = "AED SENSOR Value Error";
					
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
					}
				/*	else
					{
						sWaitCnt = 100;
					}	*/				
				
				}
			}		
		}
		break;	
	case   TEST_SEQ_TYPE_GET_MACADDRESS:
		
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(CheckMacRead() )
				{
					//int lNg = 0;
					
					m_RefMacAddress.MakeUpper();
					m_ReadMacAddress.MakeUpper();
					//if((m_RefMacAddress[0].Compare(m_ReadMacAddress)  == 0)
					//	||(m_RefMacAddress[1].Compare(m_ReadMacAddress)  == 0)
					//	||(m_RefMacAddress[2].Compare(m_ReadMacAddress)  == 0))
					if(m_RefMacAddress.Compare(m_ReadMacAddress)  == 0)
					{
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;	
						m_nCurrentStep = 2;
						//m_Deisplay_ErrorName = "OK";
						//m_Deisplay_ErrorName += "\r\nTIME OUT";
						//m_Deisplay_ErrorDetail = "";
					}
					else
					{
						m_Deisplay_ErrorName = "MAC ADDRESS Check ERROR";
						m_Deisplay_ErrorName += "\r\nMAC ADDRESS Value";
						m_Deisplay_ErrorDetail = "MAC ADDRESS Value Error";
					
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_nCurrentStep = 2;
						//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;	
					}			

				}
				else
				{
					m_Deisplay_ErrorName = "MAC ADDRESS Check ERROR";
					m_Deisplay_ErrorName += "\r\nMAC ADDRESS Value";
					m_Deisplay_ErrorDetail = "MAC ADDRESS Value Error";
					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
					m_nCurrentStep = 2;		
				
				}
			}		
		}

		break;	
//		TEST_SEQ_TYPE_SET_MANUAL_EXPOSURE_MODE	34
//#define TEST_SEQ_TYPE_FACTORY_RESET				35
		case   TEST_SEQ_TYPE_SET_MANUAL_EXPOSURE_MODE:
		
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(SetManualExposureMode() )
				{					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;	
					m_nCurrentStep = 2;	
				}
				else
				{
					m_Deisplay_ErrorName = "\r\nMANUAL EXOSURE MODE ERROR";
					//m_Deisplay_ErrorName += "MAC ADDRESS Value";
					m_Deisplay_ErrorDetail = "Manual Exposure Mode Error";
					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
					m_nCurrentStep = 2;		
				
				}
			}		
		}

		break;


		case   TEST_SEQ_TYPE_SET_AUTO_EXPOSURE_MODE:
		
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(SetAutoExposureMode() )
				{					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;	
					m_nCurrentStep = 2;	
				}
				else
				{
					m_Deisplay_ErrorName = "\r\nAUO EXOSURE MODE ERROR";
					//m_Deisplay_ErrorName += "MAC ADDRESS Value";
					m_Deisplay_ErrorDetail = "AUTO Exposure Mode Error";
					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
					m_nCurrentStep = 2;		
				
				}
			}		
		}

		break;

		case   TEST_SEQ_TYPE_FACTORY_RESET:
		
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(FactoryReset() )
				{					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;	
					m_nCurrentStep = 2;	
				}
				else
				{
					m_Deisplay_ErrorName = "\r\nFACTORY RESET ERROR";
					//m_Deisplay_ErrorName += "MAC ADDRESS Value";
					m_Deisplay_ErrorDetail = "Factory Reset Error";
					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
					m_nCurrentStep = 2;		
				
				}
			}		
		}

		break;	
		case   TEST_SEQ_TYPE_SET_MODEL_NAME:
		
		if(m_InterProcess == 0)
		{
			sWaitCnt = 50;
			m_InterProcess = 1;
		}
		else if(m_InterProcess == 1)
		{
			if(sWaitCnt == 0)
			{
				if(Set_ModelName() )
				{					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 1;	
					m_nCurrentStep = 2;	
				}
				else
				{
					m_Deisplay_ErrorName = "\r\nSET_MODEL_NAME";
					//m_Deisplay_ErrorName += "MAC ADDRESS Value";
					m_Deisplay_ErrorDetail = "SET_MODEL_NAME Error";
					
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
					m_nCurrentStep = 2;		
				
				}
			}		
		}

		break;	
	default:
		m_nCurrentStep = 2;
		break;

	}

}

//
//void CAcqVoltageSamples_IntClkDlg::CallPreProcess(int lCurrentProcess)
//{
//	//int		m_PreRunCounter;
//	//int		m_PreTaskRun;
//	//int		m_nPreCurrentNumber = 0;
//	//m_nPreCurrentProcess
//	if(gTestSeqData.PreTestCnt == 0)
//	{
//		m_PreTaskRun = TASK_RUN_IDLE;
//	}
//
//	
//	switch(m_nPreCurrentStep)
//	{
//	case 0:
//		PreSetFunction();
//		m_nPreCurrentStep = 1;
//		break;
//	case 1:
//		PreRunFunction();
//		break;
//	case 2:
//		//GetFunction();m_nCurrentNumber
//
//		m_nPreCurrentStep = 0;
//		m_nPreCurrentProcess++;
//		if( m_nPreCurrentProcess >= gTestSeqData.PreStepList[m_nPreCurrentNumber].nCount )
//		{
//			m_nPreCurrentNumber++;
//			if(m_nPreCurrentNumber >= gTestSeqData.PreTestCnt)
//			{
//				m_PreTaskRun = TASK_RUN_IDLE;
//			}
//			else
//			{
//				m_nPreCurrentProcess = 0;
//			}
//		}
//		break;
//	default:
//		break;
//	}
//
//	
//}
//
//void CAcqVoltageSamples_IntClkDlg::PreSetFunction()//PreRunFunction
//{
//	CString lFunctionString;
//
//	/*lFunctionString = gTestSeqData.PreStepList[m_nPreCurrentNumber].FunctionCommand[m_nPreCurrentProcess];
//	int lpos, lendpos;
//	lpos = lFunctionString.Find("(");
//	lendpos = lFunctionString.Find(")");
//	m_PreFunctionName = lFunctionString.Left(lpos);
//	m_PreFunctionParameter = lFunctionString.Mid(lpos+1,lendpos);*/
//	
//
//	m_PreFunctionType = TEST_SEQ_TYPE_END;
//
//	lFunctionString = gTestSeqData.PreStepList[m_nPreCurrentNumber].FunctionCommand[m_nPreCurrentProcess];
//	int lpos, lendpos;
//	lpos = lFunctionString.Find("(");
//	lendpos = lFunctionString.Find(")");
//	lFunctionString = lFunctionString.Left(lendpos);
//	m_PreFunctionName = lFunctionString.Left(lpos);
//	m_PreFunctionParameter = lFunctionString.Mid(lpos+1);
//
//	if(m_PreFunctionName.Compare("delay") == 0)
//	{
//		m_PreFunctionType = TEST_SEQ_TYPE_DELAY;
//		m_PreCheckDelayTime = _ttoi(m_PreFunctionParameter);
//		if(m_PreCheckDelayTime < 0)
//		{
//			m_PreCheckDelayTime = 1000;
//		}
//		SetTimeCheck(2);
//
//
//		
//	}
//	else if((m_PreFunctionName.Compare("remocon_out") == 0)||(m_PreFunctionName.Compare("remocon_out1") == 0))
//	{
//		m_PreFunctionType = TEST_SEQ_TYPE_MP_KEY;
//		int lpos, lendpos;
//		lpos = m_PreFunctionParameter.Find("\"");
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1);
//		lendpos = strtemp.Find("\"");
//
//		strtemp = strtemp.Left(lendpos);
//		m_PrestrMP_Key = strtemp;
//
//
//		if(m_PreCheckDelayTime < 50)
//		{
//			m_PreCheckDelayTime = 1000;
//		}
//	}
//	else if(m_PreFunctionName.Compare("remocon_out2") == 0)
//	{
//		m_PreFunctionType = TEST_SEQ_TYPE_PRE_KEY;
//		int lpos, lendpos;
//	/*	lpos = m_PreFunctionParameter.Find("\"");
//		lendpos = m_PreFunctionParameter.Find("\"");
//
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1,lendpos);
//		m_strMP_Key = strtemp;*/
//		
//		lpos = m_PreFunctionParameter.Find("\"");
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1);
//		lendpos = strtemp.Find("\"");
//
//		strtemp = strtemp.Left(lendpos);
//		m_PrestrMP_Key = strtemp;
//		if(m_PreCheckDelayTime < 50)
//		{
//			m_PreCheckDelayTime = 1000;
//		}
//	}
//	else if(m_PreFunctionName.Compare("remocon_repeat") == 0)
//	{
//		m_PreFunctionType = TEST_SEQ_TYPE_MP_REPEAT_KEY;
//		int lpos, lendpos;
//		/*lpos = m_PreFunctionParameter.Find("\"");
//		lendpos = m_PreFunctionParameter.Find("\"");
//
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1,lendpos);*/
//		lpos = m_PreFunctionParameter.Find("\"");
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1);
//		lendpos = strtemp.Find("\"");
//
//		strtemp = strtemp.Left(lendpos);
//		m_PrestrMP_Key = strtemp;
//		if(m_PreCheckDelayTime < 50)
//		{
//			m_PreCheckDelayTime = 1000;
//		}
//	}		
//	else if(m_PreFunctionName.Compare("remocon_repeat10") == 0)
//	{
//		m_PreFunctionType = TEST_SEQ_TYPE_MP_REPEAT10_KEY;
//		int lpos, lendpos;
//		/*lpos = m_PreFunctionParameter.Find("\"");
//		lendpos = m_PreFunctionParameter.Find("\"");
//
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1,lendpos);*/
//		lpos = m_PreFunctionParameter.Find("\"");
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1);
//		lendpos = strtemp.Find("\"");
//
//		strtemp = strtemp.Left(lendpos);
//		m_PrestrMP_Key = strtemp;
//		if(m_PreCheckDelayTime < 50)
//		{
//			m_PreCheckDelayTime = 1000;
//		}
//	}
//	else if(m_PreFunctionName.Compare("remocon_repeat20") == 0)
//	{
//		m_PreFunctionType = TEST_SEQ_TYPE_PRE_REPEAT20_KEY;
//		int lpos, lendpos;
//		/*lpos = m_PreFunctionParameter.Find("\"");
//		lendpos = m_PreFunctionParameter.Find("\"");
//
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1,lendpos);*/
//		lpos = m_PreFunctionParameter.Find("\"");
//		CString strtemp = m_PreFunctionParameter.Mid(lpos+1);
//		lendpos = strtemp.Find("\"");
//
//		strtemp = strtemp.Left(lendpos);
//		m_PrestrMP_Key = strtemp;
//		if(m_PreCheckDelayTime < 50)
//		{
//			m_PreCheckDelayTime = 1000;
//		}
//	}
//	
//	
//	
//	m_OKcnt = 0;
//	SetTimeCheck(2);
//}
//
//void CAcqVoltageSamples_IntClkDlg::PreRunFunction()
//{
//	//if(m_TEST_Result == 0)//NG
//	//{
//	//	TestFinish();
//	//	return;
//	//}
//	switch(m_PreFunctionType)
//	{
//	case TEST_SEQ_TYPE_DELAY:
//		if(GetTimeCheck(2) > m_PreCheckDelayTime)
//		{
//			SetTimeCheck(2);
//			m_nPreCurrentStep = 2;
//		}
//		
//		break;
//
//	case  TEST_SEQ_TYPE_PRE_KEY:
//		
//		MessageDisplay(0, "TEST_SEQ_TYPE_PRE_KEY!!");
//	//	SendPreIR_Code(m_PrestrMP_Key, 0);
//		m_nPreCurrentStep = 2;
//	
//		break;
//		
//	case  TEST_SEQ_TYPE_PRE_REPEAT_KEY:
//		
//		MessageDisplay(0, "TEST_SEQ_TYPE_PRE_REPEAT_KEY!!");
//	//	SendPreIR_Code(m_PrestrMP_Key, 1);
//		m_nPreCurrentStep = 2;
//	
//		break;
//
//
//		
//	case  TEST_SEQ_TYPE_PRE_REPEAT20_KEY:
//		
//		MessageDisplay(0, "TEST_SEQ_TYPE_PRE_REPEAT_KEY!!");
//	//	SendPreIR_Code(m_PrestrMP_Key, 3);
//		m_nPreCurrentStep = 2;
//	
//		break;
//
//	default:
//		m_nPreCurrentStep = 2;
//		break;
//
//	}
//
//}
//




////SetMic
//void CAcqVoltageSamples_IntClkDlg::SetFreqSend(UINT lPort, UINT lFreq )//MIC : 4
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	BYTE lbuf[128]; 
//	int lcnt;
//	CString str;
//	//UINT m_EditFreq1;
//	
//
//	lbuf[0] = 0x02;
//	
//	//sprintf((char *)&lbuf[1], "CFREQ%02d%03d", 
//	if(lFreq == 0)
//	{
//		sprintf((char *)&lbuf[1], "CFREQ%02d000       ", lPort);
//	}
//	else if(lFreq == 1)
//	{
//		sprintf((char *)&lbuf[1], "CFREQ%02d001       ", lPort);
//	}
//	else
//	{
//		sprintf((char *)&lbuf[1], "CFREQ%02d%03d       ", lPort, lFreq/10);
//	}
//	
//	lbuf[18] = 0x03;
//	lbuf[19] = 0x0d;
//	lbuf[20] = 0x0a;
//	//ReceiveData[18] == 0x03) && (ReceiveData[19] == 0x0d) && (ReceiveData[20] == 0x0a
//	SendData(lbuf, 21);	
//	
//	//MessageDisplay(2, "Frequency Set !");
//
//
//}

//void CAcqVoltageSamples_IntClkDlg::GetRmtCodeList(CString lRmtFile)//gTestSeqData.strRemoconFileName;
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	
//	FILE *m_fp;
//	CString str;
//	CString Filename;
//	char lbuf[1024];
//	char lTemp_Company[64];
//	char lTemp_Code[64];
//	char lTemp_Repeat[64];
//	char lTemp_Name[64];
//	char lTemp_Discription[129];
//
//	
//
//	str = PATH_BASE_DIR;
//	str += _T("\\Remote\\");
//	str += lRmtFile;//gTestSeqData.strRemoconFileName;
//	
//	// 파일경로 가져오기    
//	 
//	Filename = str;
//		
//	fopen_s(&m_fp, (LPCSTR)Filename, "r");
//	if(m_fp)
//	{
//		m_RemoconModel.m_Cnt = 0;
//		//memset(&m_RemoconModel, 0, sizeof(Recomon_ModelT));
//		while(fgets(lbuf, 1024, m_fp) != NULL) 
//		{
//			if(lbuf[0] == '[')
//			{
//				continue;
//			}
//			if(strlen(lbuf) < 16)
//			{
//				break;
//			}
//			sscanf_s(lbuf, "%s %s %s %s", lTemp_Company,32, lTemp_Code,32,lTemp_Repeat,32, lTemp_Name,32);
//			if(lTemp_Company[strlen(lTemp_Company)-1] == ',')
//			{
//				lTemp_Company[strlen(lTemp_Company)-1] = 0 ;
//			}
//				
//			if(lTemp_Code[strlen(lTemp_Code)-1] == ',')
//			{
//				lTemp_Code[strlen(lTemp_Code)-1] = 0 ;
//			}
//				
//			if(lTemp_Repeat[strlen(lTemp_Repeat)-1] == ',')
//			{
//				lTemp_Repeat[strlen(lTemp_Repeat)-1] = 0; 
//			}
//
//			if(lTemp_Name[strlen(lTemp_Name)-1] == ',')
//			{
//				lTemp_Name[strlen(lTemp_Name)-1] = 0;
//			}
//
//			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Company.Format(_T("%s"),lTemp_Company);
//			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Code.Format(_T("%s"),lTemp_Code); 
//			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Repeat = atoi(lTemp_Repeat);
//			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Name.Format(_T("%s"),lTemp_Name); 
//			m_RemoconModel.m_Cnt++;		
//			if(m_RemoconModel.m_Cnt >= 200)
//			{
//				break;
//			}
//		}
//					
//
//		fclose(m_fp);	
//		//SetDlgItemText(IDC_STATIC_RMT_FILE, Filename);
//		//for(int id = 0; id < 12; id++)
//		//{
//		//	m_cComboRmTest[id].ResetContent();
//		//	for(int i = 0; i < m_RemoconModel.m_Cnt; i++)
//		//	{
//		//		str = m_RemoconModel.Data[i].m_Company;
//		//		str += _T(" : "); 
//		//		str += m_RemoconModel.Data[i].m_Name;
//		//		m_cComboRmTest[id].AddString(str);
//		//	}
//		//	if(m_RemoconModel.m_Cnt > id)
//		//	{
//		//		m_cComboRmTest[id].SetCurSel(id);
//		//	}
//		//	else
//		//	{
//		//		m_cComboRmTest[id].SetCurSel(0);
//		//	}
//		//}
//	} 
//}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedStopButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_TaskRun = false;
	m_TaskRun = TASK_RUN_IDLE;
	//m_startButton.EnableWindow(true);
	CycleTimeOn = 0;
	//TestFinish();///test

	RefreshTimeOn = 1;
}
//
//void CAcqVoltageSamples_IntClkDlg::CleanUpIfNecessary()
//{
//	for(int i = 0; i < 16; i++)
//	{
//		delete m_reader[i].release();
//		delete m_task[i].release();
//	}
//}
//
//void CAcqVoltageSamples_IntClkDlg::RemoveEventHandler()
//{
//	for(int i = 0; i < 16; i++)
//	{
//		if (m_reader[i].get())
//		{
//			// Remove our event handler
//			while(!m_reader[i]->RemoveAllEventHandlers())
//			{
//				::Sleep(100);
//
//				MSG msg;
//				while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//				{
//					TranslateMessage(&msg);
//					DispatchMessage(&msg);
//				}
//			}
//		}
//	}
//}

void CAcqVoltageSamples_IntClkDlg::OnCancel()
{
	//gbShutdown = true;
	Sleep(200);
    ShowWindow(SW_HIDE);
 //   RemoveEventHandler();
	//for(int i = 0; i < 16; i++)
	//{
	//	if (m_task[i].get() != NULL)
	//	{
	//		m_task[i]->Control(DAQmxTaskAbort);
	//	}
	//}
 //   CleanUpIfNecessary();

	//g_MesDb.Close();

	if(mpDlgResult != NULL)
		delete mpDlgResult;

	if(mpDlgBarcode != NULL)
		delete mpDlgBarcode;

	if(mpDlgSimpleInform != NULL)
		delete mpDlgSimpleInform;

    CDialog::OnCancel();
}




//#define TEMP_CHANGE_PORT__





void CAcqVoltageSamples_IntClkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//double m_avg_PPVoltage[6][3];
	//double m_avg_Freg[6][3];

	CString str;
	unsigned long lCheck_Time;
	//static int sEthernetWaitCnt = 0;

	
	if(nIDEvent == 2)
	{
		KillTimer(2);

		if(g_nPortReopen)
		{
			MessageDisplay(2, "GMES Port Change!");
			CloseEthernetScocket() ;
			m_EthernetMode =  ETHERNET_MODE_CREATE_SOCKET;
			m_EthernetWaitCnt = 0;
		
			g_MesEnable = 1;
			g_nPortReopen = 0;
		}

		if(g_MesEnable == 0)
		{
			m_EthernetMode = ETHERNET_MODE_IDLE;
		}

		switch(m_EthernetMode)
		{	
		case ETHERNET_MODE_CREATE_SOCKET:

			if(m_EthernetWaitCnt > 1)
			{
				if(CreateEthernetSocket())
				{
					m_EthernetMode = ETHERNET_MODE_HOST_CHECK;
					MessageDisplay(2, "GMES Socket Create!");

					m_cLb_ComMainGmes.put_BackColor(0x0000FFFF);
				}
				else
				{
					m_EthernetMode = ETHERNET_MODE_IDLE;
					MessageDisplay(1, "GMES Socket Create Fail!");
					m_cLb_ComMainGmes.put_BackColor(0x000000FF);

					MessageBox("GMES Socket Create Fail!");
					g_MesEnable = 0;

				}

				m_EthernetWaitCnt = 0;
			}
			else
			{
				m_EthernetWaitCnt++;
			}
			break;
		case ETHERNET_MODE_HOST_CHECK:
			//Send Are You There
			//=> Are You There Data		
			
			if(m_EthernetWaitCnt > 5)
			{
				m_EthernetWaitCnt = 0;
				GMESAreYouThereRequest();
			}
			else
			{
				m_EthernetWaitCnt++;
			}
			break;		
		
		case ETHERNET_MODE_TIME_SEND:
			//Equipment Status Send
			//=>Equipment Status Acknowledge
			
			if(m_EthernetWaitCnt > 10)
			{
				m_EthernetWaitCnt = 0;
				GMESDateandTimeData();
			}
			else
			{
				m_EthernetWaitCnt++;
			}
			break;

		case ETHERNET_MODE_EQIP_CONNECT:
			//Equipment Status Send
			//=>Equipment Status Acknowledge
			
			if(m_EthernetWaitCnt > 10)
			{
				GMESEquipmentStatusSend(1);
			}
			else
			{
				m_EthernetWaitCnt++;
			}
			break;

		case ETHERNET_MODE_WAIT:
			//Equipment Status Send
			//=>Equipment Status Acknowledge
			if(m_EthernetAck == 1)
			{
				m_EthernetWaitCnt = 0;
				m_EthernetMode = ETHERNET_MODE_RUN;//ETHERNET_MODE_WAIT
			}
			else if(m_EthernetWaitCnt > 100)
			{
				m_EthernetResendCnt++;
				if(m_EthernetResendCnt > 1)
				{
					m_EthernetWaitCnt = 0;
					m_cLb_ComMainGmes.put_BackColor(0x000000FF);
					MessageDisplay(1, "GMES Ack Time Out");
					m_GMES_CommError = 1;
					m_TaskRun = TASK_WAIT_SELECT_PROCESS;
					m_EthernetMode = ETHERNET_MODE_RUN;
					//m_NG_Retry_Count++;
				//	
					
					m_Deisplay_ErrorName = "GMES Ack Time Out";
					m_Deisplay_ErrorName += "\r\nGMES COM NG";
					m_Deisplay_ErrorDetail = "GMES Ack Time Out";
					NG_Display("GMES Ack Time Out");
				}
				else
				{
					m_EthernetWaitCnt = 0;
					OnCmdGmesSend();
				}
			}
			else
			{
				m_EthernetWaitCnt++;
			}
			break;
		case ETHERNET_MODE_RUN://OnCmdGmesSend();

			m_GMES_Alive_WaitCnt++;
			if(m_GMES_Alive_WaitCnt > 2500)
			{
				g_MesEnable = 0;
				m_GMES_Alive_WaitCnt = 2;
				MessageBox( "GMES Link Time Out");
				CloseEthernetScocket();
				m_GMES_NGCNT ++;
				MessageDisplay(1, "GMES Link Time Out");
				m_cLb_ComMainGmes.put_BackColor(0x000000FF);
		
				m_EthernetWaitCnt = 0;
			}
			else
			{
				if(m_GMES_Alive_WaitCnt == 20)
				{
					g_MesEnable = 1;
					m_cLb_ComMainGmes.put_BackColor(0x0000FF00);
				//m_cLb_ComMainGmes.put_BackColor(0x0000FF00);
				}
				else if(m_GMES_Alive_WaitCnt == 200)
				{
					m_cLb_ComMainGmes.put_BackColor(0x0000FFFF);
			//		GMESLinkLoopbackTest();
				}

			}
		
			//SetTimer(2,20,NULL);

			break;
		default:

			if(m_EthernetWaitCnt > 100)
			{
				m_EthernetWaitCnt = 0;
				m_cLb_ComMainGmes.put_BackColor(0x000000FF);
				

			}
			else
			{
				m_EthernetWaitCnt++;
			}
			//if(gTestSeqData.bMesEnable)
			//{
			////	m_EthernetMode = ETHERNET_MODE_CREATE_SOCKET;
			////	m_EthernetWaitCnt = 0;

			//}
			break;
		}

		SetTimer(2,20,NULL);
	}
	else if(nIDEvent == 1)
	{
		KillTimer(1);
	


		if(m_EthernetMode  != ETHERNET_MODE_WAIT)
		{

		
			switch(m_TaskRun)
			{	
			case TASK_RUN_IDLE:
			


				if(GetTimeCheck(3) > 500)
				{
					if(m_DO_Writedata &  0x01)
					//if(m_DO_Writedata &  0x03) //NG 50ms
					{
						//SetD_OUT(0x00);
					}
				}

				CheckTimeOn = 0;

				if(RefreshTimeOn == 1)
				{
					
					if(GetTimeCheck(_TIME_ID_RESFRESH_) > 5000)
					{
						RefreshTimeOn = 0;
						if (m_TEST_Result == 1)
						{
							CLOSE_Result_Display();
							DisplayClear();

							m_cLbOkNg.put_Caption(_T("STOP"));
							m_cLbOkNg.put_BackColor(0x0000FFFF);

							m_GMES_CommError = 0;
							for (int i = 0; i < gTestSeqData.TestCnt; i++)
							{
								m_CtrlListMainProcess.SetItem(i, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);
								m_CtrlListMainProcess.SetItem(i, 3, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);
					
								m_CtrlListMainProcess.Update(i);

								gTestSeqData.TestStepList[i].m_Result = 1;

							}
						}
					}
				}
				break;

		
			case TASK_WAIT_SELECT_PROCESS:
				if(m_NG_Retry_Count < 10)
				{
					//m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);
					////m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
					//m_nCurrentProcess = 0;
					//m_nCurrentStep = 0;
					//m_NG_WaitFlag = 1;
					if(mpDlgResult->m_ReturnFlag == 1)
					{
						m_NG_Retry_Count++;
						if(m_GMES_CommError == 1)
						{
							gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
							m_NG_WaitFlag = 0;
							TestFinishGMES();//TestFinish();
						}
						else
						{
							gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
							m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL);
							////m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL);
							m_nCurrentProcess = 0;
							m_nCurrentStep = 0;
							CLOSE_Result_Display();
							m_TaskRun = TASK_RUN_PROCESS;
						}

					}
					else if(mpDlgResult->m_ReturnFlag == 2)//||((m_NG_WaitFlag == 1)&&(m_NG_Retry_Count >= 2)))
					{
						gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
						m_NG_WaitFlag = 0;
						TestFinishGMES();//TestFinish();

					}
		
		
				}
				else 
				{
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;
					m_NG_WaitFlag = 0;
					TestFinishGMES();//TestFinish();
				}

			//m_NG_WaitFlag = 0;
		
				break;		
			
			case TASK_WAIT_BARCODE_PROCESS:
		
				if(mpDlgBarcode->m_ReturnFlag == 1)
				{
					m_TaskRun = TASK_RUN_IDLE;
					m_BarCode.put_Caption(mpDlgBarcode->m_Barcode);
					OnBnClickedStart();

				}
				else if(mpDlgBarcode->m_ReturnFlag == 2)
				{
					m_TaskRun = TASK_RUN_IDLE;
					m_cLbOkNg.put_Caption(_T("STOP"));//m_BarCode
					m_cLbOkNg.put_BackColor(0x0000FFFF);

				}
		
		
				

			//m_NG_WaitFlag = 0;
		
				break;

			case TASK_RUN_START:

				//SetD_OUT(0x00);//OK DO OUT CONTROL 

	//			m_nCurrentNumber = 0;//gTestSeqData.TestProcess.aSeqData[0].nSeqType;
	//			m_nCurrentProcess = 0;
				m_nCurrentStep = 0;
				SetTimeCheck(1);//m_WaitCnt = 0;
				m_TEST_Result = 3;
				m_TaskRun = TASK_RUN_PROCESS;
				MessageDisplay(0, "START");


				break;

			case TASK_RUN_PROCESS://m_TaskRun

			
				//lCheck_Time = GetTimeCheck(6);			
			
				//str.Format(_T("%.1f"),  lCheck_Time/1000.0);
				//m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 3, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
		

				CallProcess();

		


				break;
			case TASK_FINISH_PROCESS:
			
					TestFinish();
					m_TaskRun = TASK_RUN_IDLE;
						
				break;

			default:

				m_TaskRun = TASK_RUN_IDLE;
				break;		
			}
		}

		DisplayTimeLap();
		if(m_TaskRun == TASK_RUN_PROCESS)
		{
			lCheck_Time = GetTimeCheck(6);					
			str.Format(_T("%.1f"),  lCheck_Time/1000.0);
			m_CtrlListMainProcess.SetItem(m_nCurrentNumber, 3, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
		}

		//str.Format(_T("CNT : %d"), m_RunCounter);
		//SetDlgItemText(IDC_STATIC_CNT, str);
		SetTimer(1,5,NULL);
	}


	CDialog::OnTimer(nIDEvent);
}


//#if 1
//void CAcqVoltageSamples_IntClkDlg::CheckData(int CheckID)
//{
//	double     lbuf[4000];
//	double lMinCmp = 0, lmaxCmp = 0;
//
//	double lMinSum = 0, lmaxSum = 0,  lAverage = 0; 
//	double lMin[300], lmax[300];
//	int lMinCnt = 0, lmaxCnt = 0;
//	double PPVoltage = 0;
//	double Frequency = 0;
//
//	int lStartNum = 0,	lEndNum = 0,	lCheckCrossCnt = 0;
//	int lSignalError = 0;
//	int i;
//	int lmaxEnable = 0;
//	int lminEnable = 0;
//
//	
//
//	double CmpCoefFwd = 0, CmpCoefBak = 0;
////////////////////////////////////////////////////
//	//
//#ifdef TESTMODE_LEVEL_CHANGE	
//	m_data[CheckID].SetSize(1,6000);
//	for(i = 0; i < 6000; i++)
//	{
//		m_data[CheckID](0,i) = 1.0*sin(3.141592 / 500 * i)*(CheckID/12.0+1) ;
//	}
//#endif
////////////////////////////////////////
//	lmaxSum = 0;
//	for(i = 0; i < NI_SAMPLE_COUNT; i++)
//	{
//		lmaxSum += abs(m_data[CheckID](0,i));
//	}
//	lAverage = lmaxSum/NI_SAMPLE_COUNT;
//	
//	//CmpCoef1 = data[CheckID][0];
//	memset(lMin, 0, sizeof(lMin));
//	memset(lmax, 0, sizeof(lmax));
//	//lMinCmp = -0.2;
//	//lmaxCmp = 0.2;
//	//m_data
//	lMinSum = 0;
//	lmaxSum = 0;
//	lminEnable  = 1;
//	lmaxEnable =1;
//
//	double lchecklimit = lAverage;//*0.5;
//	if(lchecklimit < 0.01)
//		lchecklimit = 0.011;
//
//	lchecklimit = lchecklimit*2;
//
//	for(i = 0; i < NI_SAMPLE_COUNT-3; i++)
//	{
//		CmpCoefBak = CmpCoefFwd;
//		CmpCoefFwd = m_data[CheckID](0,i)+m_data[CheckID](0,i+1)+m_data[CheckID](0,i+2)+m_data[CheckID](0,i+3);
//	//	if((-0.00 > CmpCoefFwd)&&(lmaxCmp >= lAverage*0.1)&&(lminEnable && lmaxEnable))
//	//	if((-0.00 > CmpCoefFwd)&&(lmaxCmp >= lAverage*0.5)&&(lminEnable && lmaxEnable))
//		if((-0.00 > CmpCoefFwd)&&(lmaxCmp >= lchecklimit)&&(lminEnable && lmaxEnable))
//		{
//			if(lStartNum == 0)
//			{
//				lMinCnt = 0;
//				lmaxCnt = 0;
//				lStartNum = i;
//			}
//			else //if(lminEnable && lmaxEnable)
//			{
//				lMin[lMinCnt] = lMinCmp;
//				lmax[lmaxCnt] = lmaxCmp;
//				lMinCnt++;
//				lmaxCnt++;
//			}
//
//			//if(lminEnable && lmaxEnable)
//			{
//				lCheckCrossCnt++;
//				if(lCheckCrossCnt >= 200)
//				{
//					lSignalError = 1;
//					break;
//				}
//				lEndNum = i;
//			}
//			lMinCmp = 0;
//			lmaxCmp = 0;
//				
//			lmaxEnable = 0;	
//			lminEnable = 0;
//		}
//		else
//		{
//		//	if((lMinCmp > CmpCoefFwd)&&(CmpCoefFwd <= -lAverage*2))
//			if((lMinCmp > CmpCoefFwd)&&(CmpCoefFwd < 0))
//			{
//				lMinCmp = CmpCoefFwd;
//				lminEnable = 1;
//			}
//	//		if((lmaxCmp < CmpCoefFwd)&&(CmpCoefFwd >= lAverage*2))
//			if((lmaxCmp < CmpCoefFwd)&&(CmpCoefFwd > 0))
//			{
//				lmaxCmp = CmpCoefFwd;
//				lmaxEnable = 1;
//			}
//			
//		}
//	}
//	if(lCheckCrossCnt < 2)
//	{
//		lSignalError = 1;
//		PPVoltage =  lmaxCmp - lMinCmp;
//	}
//	else
//	{
//		for(i = 0; i < lMinCnt ; i++)
//		{			
//			//lmaxSum += lmax[i];
//			lMinSum += lMin[i];
//		}
//
//		for(i = 0; i < lmaxCnt ; i++)
//		{			
//			lmaxSum += lmax[i];
//			//lMinSum += lMin[i];
//		}
//
//		PPVoltage = (lmaxSum/lmaxCnt - lMinSum/lMinCnt)*1000 /4;
//	}
//
//	if(lSignalError)
//	{
//		Frequency = 0;
//	}
//	else
//	{
//	//	Frequency = 100000.0 / (lEndNum - lStartNum) * (lCheckCrossCnt-1);
//		Frequency = NI_SAMPLE_RATE / (lEndNum - lStartNum) * (lCheckCrossCnt-1);
//	
//	}
///*	NI_SAMPLE_RATE		
// NI_SAMPLE_COUNT	*/	
//	m_PPVoltage[CheckID] = PPVoltage;	
//	m_Frequency[CheckID] = Frequency;
//
////	M_Vol[CheckID].Format(_T("P%02d : %4.0fmV"), CheckID, PPVoltage);
////	M_Frq[CheckID].Format(_T("F%02d : %4.0fHz"), CheckID, Frequency);
//	UpdateData(FALSE);
//
//	if((lStartNum < 0)||(lStartNum >= NI_SAMPLE_COUNT-NI_CHECK_COUNT-50))
//	{
//		lStartNum = 0;
//	}
//
//	int j;
//	
//	j = 0;
//	lmaxSum = 0;
//
//
//	Check_data[CheckID].SetSize(1,NI_CHECK_COUNT);
//
//#if 0
//	//for(i = lStartNum; i < lStartNum+NI_CHECK_COUNT; i++)
//	//{
//	//	Check_data[CheckID](0,j) = m_data[CheckID](0,i);
//	//	
//	//	j++;			
//	//}
//
//	for(i = lStartNum; i < lStartNum+NI_CHECK_COUNT; i++)
//	{
//		lmaxSum = 0;
//		for(int k = 0; k < 40; k++)
//		{
//			lmaxSum += m_data[CheckID](0,i+k);
//		}
//
//		{
//			Check_data[CheckID](0,j) = lmaxSum / 40;
//		}
//		j++;			
//	}
//#else
//	for(i = lStartNum; i < lStartNum+NI_CHECK_COUNT; i++)
//	{
//		lmaxSum = 0;
//		for(int k = 0; k < 20; k++)
//		{
//			lmaxSum += m_data[CheckID](0,i+k);
//		}
//
//		{
//			Check_data[CheckID](0,j) = lmaxSum / 20;
//		}
//		j++;			
//	}
//
//
//	lmaxSum = 0;
//	for(i = 0; i < NI_CHECK_COUNT; i++)
//	{
//		if(lmaxSum < Check_data[CheckID](0,i))//m_Frequency[CheckID] > 25000)
//		{
//			lmaxSum = Check_data[CheckID](0,i);
//		}
//
//		if(lMinSum > Check_data[CheckID](0,i))//m_Frequency[CheckID] > 25000)
//		{
//			lMinSum = Check_data[CheckID](0,i);
//		}
//	}
//	if(((lmaxSum - m_PPVoltage[CheckID] / 2) >  m_PPVoltage[CheckID] / 2/5)&&((lMinSum - m_PPVoltage[CheckID] / 2) >  m_PPVoltage[CheckID] / 2/5))
//	{
//		m_NoiseFlag[CheckID] = 1;
//	}
//	else
//	{
//		m_NoiseFlag[CheckID] = 0;
//	}
//
//	if((lmaxSum < m_PPVoltage[CheckID]/2000)&&(m_PPVoltage[CheckID] > 20)&&(m_Frequency[CheckID] > 1500))
//	{
//		double lScaleCoef = 0;
//		lScaleCoef = m_PPVoltage[CheckID]/lmaxSum/2000;
//
//		for(i = 0; i < NI_CHECK_COUNT; i++)
//		{
//			Check_data[CheckID](0,i) = Check_data[CheckID](0,i) * lScaleCoef;
//		}
//	}	
//#endif
//}
//#else
//
//void CAcqVoltageSamples_IntClkDlg::CheckData(int CheckID)
//{
//	double     lbuf[2000];
//	double lMinCmp = 0, lmaxCmp = 0;
//
//	double lMinSum = 0, lmaxSum = 0; 
//	double lMin[300], lmax[300];
//	int lMinCnt = 0, lmaxCnt = 0;
//	double PPVoltage = 0;
//	double Frequency = 0;
//
//	int lStartNum = 0,	lEndNum = 0,	lCheckCrossCnt = 0;
//	int lSignalError = 0;
//	int i;
//	int lmaxEnable = 0;
//	int lminEnable = 0;
//
//	
//
//	double CmpCoefFwd = 0, CmpCoefBak = 0;
//	
//	//CmpCoef1 = data[CheckID][0];
//	memset(lMin, 0, sizeof(lMin));
//	memset(lmax, 0, sizeof(lmax));
//	//lMinCmp = -0.2;
//	//lmaxCmp = 0.2;
//	//m_data
//	for(i = 0; i < 2999; i++)
//	{
//		CmpCoefBak = CmpCoefFwd;
//		CmpCoefFwd = m_data[CheckID](0,i)+m_data[CheckID](0,i+1);
//		if((-0.01 < CmpCoefFwd)&&(0.01 > CmpCoefBak)&&(lMinCmp <= -0.05))//&& (lmaxCmp >= 0.15))
//		{
//			if(lStartNum == 0)
//			{
//				lMinCnt = 0;
//				lmaxCnt = 0;
//				lStartNum = i;
//			}
//			else //if(lminEnable && lmaxEnable)
//			{
//				lMin[lMinCnt] = lMinCmp;
//				lmax[lmaxCnt] = lmaxCmp;
//				lMinCnt++;
//				lmaxCnt++;
//			}
//
//			//if(lminEnable && lmaxEnable)
//			{
//				lCheckCrossCnt++;
//				if(lCheckCrossCnt >= 200)
//				{
//					lSignalError = 1;
//					break;
//				}
//				lEndNum = i;
//			}
//			lMinCmp = 0;
//			lmaxCmp = 0;
//				
//			lmaxEnable = 0;	
//			lminEnable = 0;
//		}
//		else
//		{
//			if((lMinCmp > CmpCoefFwd))
//			{
//				lMinCmp = CmpCoefFwd;
//				lminEnable = 1;
//			}
//			if((lmaxCmp < CmpCoefFwd))
//			{
//				lmaxCmp = CmpCoefFwd;
//				lmaxEnable = 1;
//			}
//			/*
//			if((lMinCmp > CmpCoefFwd)&&(CmpCoefFwd < -0.001))
//			{
//				lMinCmp = CmpCoefFwd;
//				lminEnable = 1;
//			}
//			if((lmaxCmp < CmpCoefFwd)&&(CmpCoefFwd > 0.001))
//			{
//				lmaxCmp = CmpCoefFwd;
//				lmaxEnable = 1;
//			}*/
//		}
//	}
//	if(lCheckCrossCnt < 2)
//	{
//		lSignalError = 1;
//		PPVoltage =  lmaxCmp - lMinCmp;
//	}
//	else
//	{
//		for(i = 0; i < lMinCnt ; i++)
//		{			
//			//lmaxSum += lmax[i];
//			lMinSum += lMin[i];
//		}
//
//		for(i = 0; i < lmaxCnt ; i++)
//		{			
//			lmaxSum += lmax[i];
//			//lMinSum += lMin[i];
//		}
//
//		PPVoltage = (lmaxSum/lmaxCnt - lMinSum/lMinCnt)*1000 /2;
//	}
//
//	if(lSignalError)
//	{
//		Frequency = 0;
//	}
//	else
//	{
//		Frequency = 100000.0 / (lEndNum - lStartNum) * (lCheckCrossCnt-1);
//	
//	}
//		
//	m_PPVoltage[CheckID] = PPVoltage;	
//	m_Frequency[CheckID] = Frequency;
//
////	M_Vol[CheckID].Format(_T("P%02d : %4.0fmV"), CheckID, PPVoltage);
////	M_Frq[CheckID].Format(_T("F%02d : %4.0fHz"), CheckID, Frequency);
//	UpdateData(FALSE);
//
//	if((lStartNum < 1)||(lStartNum >= 1000))
//	{
//		lStartNum = 0;
//	}
//
//	int j;
//	j = 0;
//
//	Check_data[CheckID].SetSize(1,2000);
//	for(i = lStartNum; i < lStartNum+2000; i++)
//	{
//		
//		Check_data[CheckID](0,j) = m_data[CheckID](0,i);
//		j++;			
//	}
//}
//
//#endif

void CAcqVoltageSamples_IntClkDlg::CheckOK()
{
	 int lOldchOk[6];

	
}
void CAcqVoltageSamples_IntClkDlg::TestResultDisplay()
{
	int lOldchOk[6];

	for(int i = 0; i < 6; i++)
	{
		if(m_CheckEnable[i])
		{
			if(m_ChannelOK[i] != 1)
			{
				m_ChannelOK[i] = 0;
				if((m_VoltageOK[i] ==  0)||(m_BalanceOK[i] == 0))
				{
					//m_VoltLabel[i].put_ForeColor(0x000000ff);
				}
				if(m_FrequencyOK[i] == 0)
				{
					//m_FreqLabel[i].put_ForeColor(0x000000ff);
				}

			}
		}
	}

	if((m_CheckEnable[0] )||(m_CheckEnable[1] ))
	{
		if(m_ChannelOK[0] && m_ChannelOK[1])
		{
		//	m_cLb_Test1.put_Caption(_T("O K"));
		/*	if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
			{
				m_cLb_Test1.put_Caption(_T("O K 1"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
			{
				m_cLb_Test1.put_Caption(_T("O K 2"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
			{
				m_cLb_Test1.put_Caption(_T("O K 3"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
			{
				m_cLb_Test1.put_Caption(_T("O K 4"));
			}*/
		//	m_cLb_Test1.put_BackColor(0x0000FF00);
		}
		else
		{
		//	m_cLb_Test1.put_Caption(_T("N G"));
		/*	if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
			{
				m_cLb_Test1.put_Caption(_T("N G 1"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
			{
				m_cLb_Test1.put_Caption(_T("N G 2"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
			{
				m_cLb_Test1.put_Caption(_T("N G 3"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
			{
				m_cLb_Test1.put_Caption(_T("N G 4"));
			}*/
		//	m_cLb_Test1.put_BackColor(0x000000FF);
		}
		
	}
	if((m_CheckEnable[2] )||(m_CheckEnable[3] ))
	{
		
		if(m_ChannelOK[2] && m_ChannelOK[3])
		{
		//	m_cLb_Test2.put_Caption(_T("O K"));
	/*		if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
			{
				m_cLb_Test2.put_Caption(_T("O K 1"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
			{
				m_cLb_Test2.put_Caption(_T("O K 2"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
			{
				m_cLb_Test2.put_Caption(_T("O K 3"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
			{
				m_cLb_Test2.put_Caption(_T("O K 4"));
			}*/
			//m_cLb_Test2.put_BackColor(0x0000FF00);
		}
		else
		{
		//	m_cLb_Test2.put_Caption(_T("N G"));
		/*	if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
			{
				m_cLb_Test2.put_Caption(_T("N G 1"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
			{
				m_cLb_Test2.put_Caption(_T("N G 2"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
			{
				m_cLb_Test2.put_Caption(_T("N G 3"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
			{
				m_cLb_Test2.put_Caption(_T("N G 4"));
			}*/
		//	m_cLb_Test2.put_BackColor(0x000000FF);
		}
		
	}

	if((m_CheckEnable[4] )||(m_CheckEnable[5] ))
	{
		
		if(m_ChannelOK[4] && m_ChannelOK[5])
		{
		//	m_cLb_Test3.put_Caption(_T("O K"));
	/*		if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
			{
				m_cLb_Test3.put_Caption(_T("O K 1"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
			{
				m_cLb_Test3.put_Caption(_T("O K 2"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
			{
				m_cLb_Test3.put_Caption(_T("O K 3"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
			{
				m_cLb_Test3.put_Caption(_T("O K 4"));
			}*/
		//	m_cLb_Test3.put_BackColor(0x0000FF00);
		}
		else
		{
		//	m_cLb_Test3.put_Caption(_T("N G"));
	/*		if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
			{
				m_cLb_Test3.put_Caption(_T("N G 1"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
			{
				m_cLb_Test3.put_Caption(_T("N G 2"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
			{
				m_cLb_Test3.put_Caption(_T("N G 3"));
			}
			else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
			{
				m_cLb_Test3.put_Caption(_T("N G 4"));
			}*/
		//	m_cLb_Test3.put_BackColor(0x000000FF);
		}
		
	}
}

void CAcqVoltageSamples_IntClkDlg::TestFinishGMES()
{
	//ULONG lSetVal = 0x00;
	//CFileControl lFile;//SAVE LOG
	//lFile.ReadCountData();

	
	for(int i = 0; i < gTestSeqData.TestCnt; i++)
	{
		if(gTestSeqData.TestStepList[i].m_Result == 0)
		{
			m_TEST_Result = 0;
			break;
		}
		else
		{
			m_TEST_Result = 1;
		}
	}

	gSaveInfo.bMesSts = 0;
	if(m_GMES_CommError == 0)
	{
		GMES_Result_ReportSend();
		//gSaveInfo.bMesSts = 1;
	}
	else
	{
	}
	m_TaskRun = TASK_FINISH_PROCESS;
		
}

void CAcqVoltageSamples_IntClkDlg::TestFinish()
{
	ULONG lSetVal = 0x00;
	CFileControl lFile;//SAVE LOG
	lFile.ReadCountData();
//	CLOSE_Result_Display();
	
	for(int i = 0; i < gTestSeqData.TestCnt; i++)
	{
		if(gTestSeqData.TestStepList[i].m_Result == 0)
		{
			m_TEST_Result = 0;
			break;
		}
		else
		{
			m_TEST_Result = 1;
		}
	}

	if(m_GMES_CommError == 1)
	{
		m_TEST_Result = 0;
	}

	if(m_TEST_Result == 1)
	{
		lSetVal = 0x01;
		MessageDisplay(0, "SEND OK SIGNAL!!");
		gSaveInfo.strTestResult = _T("OK");
		g_nOKCnt++;
		m_cLbOkNg.put_Caption(_T("O K"));
		m_cLbOkNg.put_BackColor(0x0000FF00);	

		SendOkSignalToJig();//Back Jig
		CycleTimeOn = 0;
		OK_Display();
		//Beep(800, 300);
		
		//CString m_NgWaveFile  = _T(PATH_BASE_DIR);
		//m_NgWaveFile  += _T("\\Sound\\OK_CHECK.wav");//OK_CHECK.wav
		//sndPlaySound( m_NgWaveFile , SND_ASYNC);

		
	
	}
	else
	{
		lSetVal = 0x02;
		MessageDisplay(1, "SEND NG SIGNAL!!");
		gSaveInfo.strTestResult = _T("NG");
		g_nNGCnt++;
				
						
		m_cLbOkNg.put_Caption(_T("N G"));
		m_cLbOkNg.put_BackColor(0x000000FF);	

		CycleTimeOn = 0;
		SendOkSignalToJig();//Back Jig

		//NG_Display();
		//CLOSE_Result_Display();
		
	}

	CString str;
	unsigned long lCheck_Time = GetTimeCheck(4);
	str.Format(_T("%.1f (s)"), lCheck_Time/1000.0);
	m_CheckTimeLabel.put_Caption(str);

	CString str1;
	str1 = m_CheckTimeLabel.get_Caption();
	str1 = str1.Left(str1.Find(" (s)"));	
	gSaveInfo.CheckTime = _ttof(str1);

	str1 = m_CycleTimeLabel.get_Caption();
	str1 = str1.Left(str1.Find(" (s)"));		
	gSaveInfo.CycleTime  = _ttof(str1);
	//if(m_GMES_CommError == 0)
	//{
	//	GMES_Result_ReportSend();
	//	gSaveInfo.bMesSts = 1;
	//}
	//else
	//{
	//	gSaveInfo.bMesSts = 0;
	//}
//	SetD_OUT(lSetVal);

	SetTimeCheck(3);

	gSaveInfo.strModelName = gTestSeqData.strModelName;
//	gSaveInfo.bMesSts = MesTestReport();
	//gSaveInfo.strTestResult = _T("OK");
	gSaveInfo.strWipID_SetSN = g_strSetID;
	
	//gCableManager.nUSB_CableCount
	
	lFile.SaveLog();


	
	//m_Check_Time = lCheck_Time/1000.0;

	CheckTimeOn = 0;
	//CycleTimeOn = 1;

	str.Format(_T("%d"), g_nOKCnt);	
	m_OKCntLabel.put_Caption(str);

	str.Format(_T("%d"), g_nNGCnt);	
	m_NGCntLabel.put_Caption(str);
	lFile.SaveCountData();

	 m_BarCode.put_Caption("");
	
	m_TaskRun = TASK_RUN_IDLE;
}

//m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//						m_LED_COM_PROCESS = COM_MODE_IDLE;
//						m_PCB_COM_PROCESS = COM_MODE_IDLE;//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
//						//m_EthernetMode = 
//
//						m_NG_WaitFlag = 0;
//						//TestFinishGMES();//
//						TestFinish();

void CAcqVoltageSamples_IntClkDlg::CheckTotalOK()
{
	
	if(m_TotalOK == 1)
	{
	/*	if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
		{
			m_cLbOkNg.put_Caption(_T("O K 1"));
		}
		else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
		{
			m_cLbOkNg.put_Caption(_T("O K 2"));
		}
		else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
		{
			m_cLbOkNg.put_Caption(_T("O K 3"));
		}
		else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
		{
			m_cLbOkNg.put_Caption(_T("O K 4"));
		}*/
	
		//m_cLbOkNg.put_Caption(_T("O K"));
		m_cLbOkNg.put_BackColor(0x0000FF00);

		//m_TEST_Result = 1;
	}
	else
	{
		/*if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_1)
		{
			m_cLbOkNg.put_Caption(_T("N G 1"));
			m_NG_OK_Num = 1;
		}
		else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_2)
		{
			m_cLbOkNg.put_Caption(_T("N G 2"));
			m_NG_OK_Num = 2;
		}
		else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_3)
		{
			m_cLbOkNg.put_Caption(_T("N G 3"));
			m_NG_OK_Num = 3;
		}
		else if(gTestSeqData.TestProcess.aSeqData[m_nCurrentNumber].nSeqType == TEST_SEQ_TYPE_TEST_4)
		{
			m_cLbOkNg.put_Caption(_T("N G 4"));
			m_NG_OK_Num = 4;
		}*/
		//m_cLbOkNg.put_Caption(_T("N G"));
		m_cLbOkNg.put_BackColor(0x000000FF);	
	//	m_TEST_Result = 0;
		//TestFinish();

		
	}

	TestResultDisplay();
	

}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedSetupButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if(m_TaskRun != TASK_RUN_IDLE) 
	{
		MessageBox("TEST PROCESSING");
		return;
	}

	CString str = _T("");
	DlgModelSetupSeq dlg;
//	CDlgModelSetup dlg;
	

	int lrtn = dlg.DoModal();
	if((lrtn == IDOK)||(lrtn == IDCANCEL))
	{

		LoadTestData(gTestSeqData.strSeqFileName);

		//SetListControl();
		DisplayTestData();
		
		//m_TextModel.put_Text(gTestSeqData.strModelName);	

		CFileControl lFile;
		lFile.SaveINIModel();
		lFile.OpenModel(gTestSeqData.strModelName);

	//	CheckDigitalPort();
//		GetRmtCodeList(gTestSeqData.strRemoconFileName);

		////str = _T("MODEL : ");
		str = gTestSeqData.strModelName;
		//m_TextModel.put_Text(str);
		m_cLbModel.put_Caption((LPCSTR)str);
		m_DO_Writedata = 0;
		if(gbManualScopeMode == 0)
		{
		//	SetChannelData(0);
		//	InitChannel();
		}

		if(g_MesEnable)
		{			
			CloseEthernetScocket() ;
			m_EthernetMode =  ETHERNET_MODE_CREATE_SOCKET;
			m_EthernetWaitCnt = 0;
			g_nPortReopen = 0;
			g_MesEnable = 1;			
		}
	}

	
	

}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedTestButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_TaskRun != TASK_RUN_IDLE) 
	{
		MessageBox("TEST PROCESSING");
		return;
	}
//	m_Camera_Capture_Flag = 0;
	CDlgTestPage dlg;

//	dlg.m_P_Main = this;

	if(m_ComPortScanner.mPort_Opened == TRUE)
		m_ComPortScanner.ClosePort();
	
	dlg.m_nPortScan  = g_nPort_Scanner;
	dlg.m_nBaudScan  = g_nBaud_Scanner;
	dlg.m_nParityScan =  g_nParity_Scanner;

	if(dlg.DoModal() == IDOK)
	{
	}

	CString str;

	//if(g_CameraFound == 1)
	//	m_Camera_Capture_Flag = 1;
	//for(int i = 0; i< 6; i++)
	//			m_graph[i].Plots.RemoveAll();
	if(gbManualScopeMode == 0)
	{
//		InitClearChannel();
	}
	

	OpenComportSCAN();
}

void CAcqVoltageSamples_IntClkDlg::MessageDisplay(int lError, CString sMessage)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_MessageDisplay.put_Caption(sMessage);

	if(lError == 0)
	{
		m_MessageDisplay.put_ForeColor(0x00FF0000);
	}
	else if(lError == 1)
	{
		m_MessageDisplay.put_ForeColor(0x000000FF);
	}
	else if(lError == 2)
	{
		m_MessageDisplay.put_ForeColor(0x0000FFFF);
	}

}



long CAcqVoltageSamples_IntClkDlg::OnReceiveDataEthernet(WPARAM wParam, LPARAM lParam )
{


	BYTE lbuf[4096];
	int lcnt;
	lcnt = (int)wParam;

	return 0;
}


long CAcqVoltageSamples_IntClkDlg::OnParseDataEthernet( )
{
	BYTE lbuf[4096];
	int lcnt;
	int lpos;
	int lendpos;
	CString l_Receivestr; 
	CString str1; 
	CString str2; 
	bool lReadFlag = 0;

	lcnt = m_strEthernetReceive.GetLength();




	if((m_strEthernetReceive.Find("<EIF") >= 0)&&(m_strEthernetReceive.Find("</EIF>") >= 0))
	{
		m_GMES_NGCNT = 0;
		AddGmesLog(m_strEthernetReceive, 1);

		m_GMES_Alive_WaitCnt = 0;
		m_cLb_ComMainGmes.put_BackColor(0x0000FF00);
		l_Receivestr = m_strEthernetReceive;

		m_strEthernetReceive = "";
		switch(m_EthernetMode)
		{
		case ETHERNET_MODE_HOST_CHECK:
			
			//Send Are You There
			//=> Are You There Data		
			if(l_Receivestr.Find("\"S1F2\"")>=0)
			{
				lReadFlag = 1;
//				m_EthernetMode = ETHERNET_MODE_EQIP_CONNECT;
				m_EthernetMode = ETHERNET_MODE_RUN;//ETHERNET_MODE_TIME_SEND;
				MessageDisplay(0, "GMES Socket OPEN!");

				//g_strEQPID						
				lpos = l_Receivestr.Find("<EQPID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+7);
				lpos = str1.Find("</EQPID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strEQPID = str1;


				lpos = l_Receivestr.Find("<EQPNAME>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<EQPNAME>"));
				lpos = str1.Find("</EQPNAME>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strEQPName = str1;

				lpos = l_Receivestr.Find("<PROCID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<PROCID>"));
				lpos = str1.Find("</PROCID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strProcID = str1;


				lpos = l_Receivestr.Find("<ORGID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<ORGID>"));
				lpos = str1.Find("</ORGID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strORGID = str1;


				lpos = l_Receivestr.Find("<LINEID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<LINEID>"));
				lpos = str1.Find("</LINEID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strLineID = str1;

//<EIF VERSION="1.8" ID="S1F1" NAME="AreYouThereRequest">
//</EIF>

//<EIFVERSION="1.8"ID="S1F2"NAME="AreYouThereData">
//<ELEMENT>
//<EQPID></EQPID>
//<EQPNAME></EQPNAME>
//<PROCID></PROCID>
//<ORGID></ORGID>
//<LINEID></LINEID>
//</ELEMENT>
//</EIF>
//				CString g_strEQPID;
//CString g_strEQPName;
//CString g_strProcID;
//CString g_strORGID;
//CString g_strLineID;
//lpos = str1.Find("ID=");
//str1 = str1.Mid(lpos);
//lpos = str1.Find("\"");
//str1 = str1.Mid(lpos+1);
//lpos = str1.Find("\"");
//str1 = str1.Left(lpos);

			}			
			break;

		case  ETHERNET_MODE_TIME_SEND:
//			if(l_Receivestr.Find("S2F32") >= 0)
//			{
//
//				#include "time.h" 
//
//SYSTEMTIME datetime; 
//datetime.wYear = 2012; 
//datetime.wMonth = 11; 
//datetime.wDay = 10; 
//
//datetime.wHour = 9; 
//datetime.wMinute = 8; 
//datetime.wSecond = 7; 
////datetime.wMilliseconds = 0; 
//SetSystemTime(&datetime); 
//
////E=>H
////<EIFVERSION="1.8"ID="S2F31"NAME="DataandTimeSend">
////<ELEMENT>
////<EQPID></EQPID>
////</ELEMENT>
////<ITEM>
////<TIME></TIME>
////</ITEM>
////</EIF>
//
//				
////H=>E
////<EIFVERSION="1.8"ID="S2F32"NAME="DataandTimeAcknowledge">
////<ELEMENT>
////<EQPID></EQPID>
////</ELEMENT>
////<ITEM>
////<ACK></ACK>
////</ITEM>
////</EIF>
//
//			}

			break;

//		case ETHERNET_MODE_EQIP_CONNECT:
//
////<EIFVERSION="1.8"ID="S1F4"NAME="EquipmentStatusAcknowledge">
////<ELEMENT>
////<EQPID></EQPID>
////</ELEMENT>
////<ITEM>
////<ACK></ACK>
////</ITEM>
////</EIF>
//			
//
//
//			break;

		default:
			break;

		}



		if(lReadFlag == 0)
		{
			
			
			if(l_Receivestr.Find("\"S1F4\"")>=0)
			{
				lReadFlag = 0;
				//m_EthernetMode = ETHERNET_MODE_RUN;

			}
			else if(l_Receivestr.Find("\"S2F3\"")>=0)
			{

				GMESLinkTestResponse();
				
//<EIFVERSION="1.8"ID="S2F3"NAME="LinkTestRequest">
//</EIF>


//<EIFVERSION="1.8"ID="S2F4"NAME="LinkTestResponse">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//</EIF>
//Notice
//<EQPID>EquipmentID

			}				
			
			else if(l_Receivestr.Find("\"S2F17\"")>=0)
			{
				GMESDateandTimeData();
//<EIFVERSION="1.8"ID="S2F17"NAME="DateandTimeRequest">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//</EIF>

//<EIFVERSION="1.8"ID="S2F18"NAME="DateandTimeData">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<TIME></TIME>
//</ITEM>
//</EIF>

			}
			else if(l_Receivestr.Find("\"S2F25\"")>=0)
			{

//<EIFVERSION="1.8"ID="S2F25"NAME="LoopbackdiagnosticsTestRequest">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<AS></AS>
//</ITEM>
//</EIF>

//<EIFVERSION="1.8"ID="S2F26"NAME="LoopbackdiagnosticsTestAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<AS></AS>
//</ITEM></EIF >

				lpos = l_Receivestr.Find("<AS>");
				str1 = l_Receivestr.Mid(lpos+strlen("<AS>"));
				lpos = str1.Find("</AS>");
				str1 = str1.Left(lpos);

				GMESLinkLoopbackResponce(str1);

			}
			else if(l_Receivestr.Find("\"S2F26\"")>=0)
			{

				m_GMES_Alive_WaitCnt = 0;
//
//				<EIFVERSION="1.8"ID="S2F26"NAME="LoopbackdiagnosticsTestAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<AS></AS>
//</ITEM></EIF >
			}
			else if(l_Receivestr.Find("\"S2F31\"") >= 0)
			{

//#include "time.h" 
		
				SYSTEMTIME datetime; 
				GetLocalTime(&datetime);

				

				int lAck =1;
				int litemp = 0;
					//YYYYMMDDhhmmss						
				lpos = l_Receivestr.Find("<TIME>");
				str1 = l_Receivestr.Mid(lpos+strlen("<TIME>"));
				lpos = str1.Find("</TIME>");
				str1 = str1.Left(lpos);

				str2 = str1.Left(4);
				datetime.wYear = _ttoi(str2);
				if(datetime.wYear < 2015)
					lAck = 0;


				str2 = str1.Mid(4, 2);
				datetime.wMonth = _ttoi(str2);
				if((datetime.wMonth < 0)||(datetime.wMonth > 12))
					lAck = 0;

				str2 = str1.Mid(6,2);
				datetime.wDay = _ttoi(str2); 
				if((datetime.wDay < 0)||(datetime.wDay > 31))
					lAck = 0;

				str2 = str1.Mid(8,2);
				datetime.wHour = _ttoi(str2); 
				if((datetime.wHour < 0)||(datetime.wHour > 24))
					lAck = 0;

				str2 = str1.Mid(10,2);
				datetime.wMinute = _ttoi(str2); 
				if((datetime.wMinute < 0)||(datetime.wMinute > 60))
					lAck = 0;
				
				str2 = str1.Mid(12,2);
				datetime.wSecond = _ttoi(str2); 
				if((datetime.wSecond < 0)||(datetime.wSecond > 60))
					lAck = 0;

				//datetime.wMilliseconds = 0; 
				if(lAck == 0)
					GetLocalTime(&datetime);//GetSystemTime(&datetime);

				SetSystemTime(&datetime); 
				//SetLocalTime(&datetime); 

				GMESDataandTimeAcknowledge(lAck);

//H=>E
//<EIFVERSION="1.8"ID="S2F31"NAME="DataandTimeSend">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<TIME></TIME>
//</ITEM>
//</EIF>
			

				
//E=>H
//<EIFVERSION="1.8"ID="S2F32"NAME="DataandTimeAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<ACK></ACK>
//</ITEM>
//</EIF>

			}			
			else if(l_Receivestr.Find("\"S5F2\"")>=0)
			{
//E => H
//<EIFVERSION="1.8"ID="S5F1"NAME="AlarmSetReportSend">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<SUBITEMLISTCOUNT="n">
//<ALCD></ALCD>
//<ALNAME></ALNAME>
//<ALTX></ALTX>
//</SUBITEMLIST>
//</ITEM>
//</EIF>

//H => E
//<EIFVERSION="1.8"ID="S5F2"NAME="AlarmSetReportAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<ACK></ACK>
//</ITEM>
//</EIF>

			}
			else if(l_Receivestr.Find("\"S5F4\"")>=0)
			{

//E => H
//<EIFVERSION="1.8"ID="S5F3"NAME="AlarmResetReportSend">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<SUBITEMLISTCOUNT="n">
//<ALCD></ALCD>
//<ALNAME></ALNAME>
//<ALTX></ALTX>
//</SUBITEMLIST>
//</ITEM>
//</EIF>

//H => E
//<EIFVERSION="1.8"ID="S5F4"NAME="AlarmResetReportAcknowledge">
//<ELEMENT>
//<EQPID></EQPID>
//</ELEMENT>
//<ITEM>
//<ACK></ACK>
//</ITEM>
//</EIF>

			}	
			else if(l_Receivestr.Find("\"S6F2\"")>=0)
			{




				lpos = l_Receivestr.Find("<SETID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<SETID>"));
				lpos = str1.Find("</SETID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				
				if(g_strSetID.Compare(str1) != 0)
				{
					MessageDisplay(1, "Set ID Mismatch!");
				}


		//lpos = l_Receivestr.Find("<MODEL>");	//</EQPID>	
		//		str1 = l_Receivestr.Mid(lpos+strlen("<MODEL>"));
		//		lpos = str1.Find("</MODEL>");	//</EQPID>	
		//		str1 = str1.Left(lpos);

		//lpos = l_Receivestr.Find("<ID>");	//</EQPID>	
		//		str1 = l_Receivestr.Mid(lpos+strlen("<ID>"));
		//		lpos = str1.Find("</ID>");	//</EQPID>	
		//		str1 = str1.Left(lpos);
		//		g_strModelID = str1;

		
				lpos = l_Receivestr.Find("<ACK>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<ACK>"));
				lpos = str1.Find("</ACK>");	//</EQPID>	
				str1 = str1.Left(lpos);
				if(str1.Compare("0") == 0)
				{
					MessageDisplay(0, "GEMS OK");
					gSaveInfo.bMesSts = 1;
				}
				else
				{
					lpos = l_Receivestr.Find("<REASON>");	//</EQPID>	
					str1 = l_Receivestr.Mid(lpos+strlen("<REASON>"));
					lpos = str1.Find("</REASON>");	//</EQPID>	
					str1 = str1.Left(lpos);
					MessageDisplay(1, str1.GetBuffer());

					str1 += "  [GMES]";
					m_Deisplay_ErrorName = str1;
					NG_Display(str1);
					m_TaskRun = TASK_WAIT_SELECT_PROCESS;
					m_GMES_CommError = 1;
					//MessageBox(str1);
					gSaveInfo.bMesSts = 0;
				}

				m_EthernetAck = 1;
				
			}		
			else if(l_Receivestr.Find("\"S6F5\"")>=0)
			{




				lpos = l_Receivestr.Find("<WOID>");	//<WOID></WOID>
				str1 = l_Receivestr.Mid(lpos+strlen("<WOID>"));
				lpos = str1.Find("</WOID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strEQPID = str1;

				lpos = l_Receivestr.Find("<ORGID>");	//<WOID></WOID>
				str1 = l_Receivestr.Mid(lpos+strlen("<ORGID>"));
				lpos = str1.Find("</ORGID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strORGID = str1;

				lpos = l_Receivestr.Find("<LINEID>");	//<WOID></WOID>
				str1 = l_Receivestr.Mid(lpos+strlen("<LINEID>"));
				lpos = str1.Find("</LINEID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strLineID = str1;

				lpos = l_Receivestr.Find("<EQPID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+7);
				lpos = str1.Find("</EQPID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strEQPID = str1;


				lpos = l_Receivestr.Find("<PROCID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<PROCID>"));
				lpos = str1.Find("</PROCID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strProcID = str1;


				lpos = l_Receivestr.Find("<SETID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<SETID>"));
				lpos = str1.Find("</SETID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strSetID = str1;


				lpos = l_Receivestr.Find("<MODEL>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<MODEL>"));
				lpos = str1.Find("</MODEL>");	//</EQPID>	
				str1 = str1.Left(lpos);

				lpos = l_Receivestr.Find("<ID>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<ID>"));
				lpos = str1.Find("</ID>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strModelID = str1;

				lpos = l_Receivestr.Find("<NAME>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<NAME>"));
				lpos = str1.Find("</NAME>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strModelName = str1;

				lpos = l_Receivestr.Find("<SUFFIX>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<SUFFIX>"));
				lpos = str1.Find("</SUFFIX>");	//</EQPID>	
				str1 = str1.Left(lpos);
				g_strModelSuffix = str1;

				str1 = g_strModelName;
				str1 += ".";
				str1 += g_strModelSuffix;

				m_EthernetAck = 1;
				GMESProductDataAcknowledge();//S6F6		
				
				//if(gTestSeqData.strModelName.Find(str1) >= 0)
				//{
				//}
				//else
				//{
				//	ChangeModel(str1);
				//}

				
			}		
			else if(l_Receivestr.Find("\"S6F12\"")>=0)
			{

				

				lpos = l_Receivestr.Find("<ACK>");	//</EQPID>	
				str1 = l_Receivestr.Mid(lpos+strlen("<ACK>"));
				lpos = str1.Find("</ACK>");	//</EQPID>	
				str1 = str1.Left(lpos);
				if(str1.Compare("0") == 0)
				{
					MessageDisplay(0, "GEMS OK");
				}
				else
				{
					lpos = l_Receivestr.Find("<REASON>");	//</EQPID>	
					str1 = l_Receivestr.Mid(lpos+strlen("<REASON>"));
					lpos = str1.Find("</REASON>");	//</EQPID>	
					str1 = str1.Left(lpos);
					MessageDisplay(1, str1.GetBuffer());
					gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 0;

					str1 += "  [GMES]";

					m_Deisplay_ErrorName = str1;
					NG_Display(str1);
					m_TaskRun = TASK_WAIT_SELECT_PROCESS;
					m_GMES_CommError = 1;

					m_EthernetAck = 1;//
					//MessageBox(str1);
				}
			}
			else if(l_Receivestr.Find("\"S9F3\"")>=0)
			{
/*<EIFVERSION="1.8"ID="S9F3"NAME="UnrecognizedStreamType">
<ELEMENT>
<EQPID></EQPID>
</ELEMENT>
</EIF>*/
			}
		}

		m_strEthernetReceive = "";


	}


	return 0;
}

void CAcqVoltageSamples_IntClkDlg::OnDestroy()
{
	CDialog::OnDestroy();
	//gbShutdown = true;
	Sleep(200);

	ShowWindow(SW_HIDE);
 //   RemoveEventHandler();
	//for(int i = 0; i < 16; i++)
	//{
	//	if (m_task[i].get() != NULL)
	//	{
	//		m_task[i]->Control(DAQmxTaskAbort);
	//	}
	//}
 //   CleanUpIfNecessary();


	

#ifdef __G_MES_SET_MODE__
	//CloseEthernetScocket();
	if (m_pUDPSocket)
	{
		m_pUDPSocket->Close();
		delete m_pUDPSocket;

		m_pUDPSocket  = NULL;

	}	
#else
	//g_MesDb.Close();
#endif





	//
	//if( vcap.isOpened() )
	//{

	//	vcap.release();
	//	
	//}

	if(mpDlgResult != NULL)
		delete mpDlgResult;

	if(mpDlgBarcode != NULL)
		delete mpDlgBarcode;

	if(mpDlgSimpleInform != NULL)
		delete mpDlgSimpleInform;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//gbShutdown = true;

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

	CDialog::OnLButtonUp(nFlags, point);
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

	
	for(int i = 0 ; i < 64; i++)
	{
		//mstartT[i]
		if((mstartT[i] == 0) || (lIndex == i))
		{
			mstartT[i] = GetTickCount();
		}
	}
	
}


unsigned long  CAcqVoltageSamples_IntClkDlg::GetTimeCheck(int  lIndex)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//MessageDisplay(0, _T("Model Mouse Up!!"));

	/*	static unsigned long mstartT_1 = 0;
	static unsigned long mstartT_2 = 0;
	static unsigned long mstartT_3 = 0;*/

	//unsigned long lCheckTime = GetTickCount();
	//unsigned long lCheckDelayTime = 0;

	//if(lIndex == 1)
	//{
	//	lCheckDelayTime = lCheckTime - mstartT_1;

	//}
	//else if(lIndex == 2)
	//{
	//	lCheckDelayTime = lCheckTime - mstartT_2 ;

	//}
	//else if(lIndex == 3)
	//{
	//	lCheckDelayTime = lCheckTime - mstartT_3;

	//}
	//else if(lIndex == 4)
	//{
	//	lCheckDelayTime = lCheckTime - mstartT_4;

	//}
	//else if(lIndex == 5)
	//{
	//	lCheckDelayTime = lCheckTime - mstartT_5;

	//}	
	//else if(lIndex == 6)
	//{
	//	lCheckDelayTime = lCheckTime - mstartT_6;

	//}
	
	unsigned long lCheckTime = GetTickCount();
	unsigned long lCheckDelayTime = 0;
	for(int i = 0 ; i < 64; i++)
	{

		if(lIndex == i)
		{
			lCheckDelayTime = lCheckTime - mstartT[i];
		}
	}



	return lCheckDelayTime;
	

}


void  CAcqVoltageSamples_IntClkDlg::DisplayTimeLap()
{
	CString str;
	unsigned long lCheck_Time ;
	if(CheckTimeOn)
	{
		lCheck_Time = GetTimeCheck(4);
		str.Format(_T("%.1f (s)"), lCheck_Time/1000.0);
		m_CheckTimeLabel.put_Caption(str);
	}


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
	SetSubListControl();
	
}



void CAcqVoltageSamples_IntClkDlg::SetListControl()
{
	
//	m_CtrlListTestProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListMainProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListMainProcess.InsertColumn(0, _T("No."), LVCFMT_CENTER, 40); 
	m_CtrlListMainProcess.InsertColumn(1, _T("STEP"), LVCFMT_CENTER, 205);// 리스트 행 추가
	m_CtrlListMainProcess.InsertColumn(2, _T("RESULT"), LVCFMT_CENTER, 80);// 리스트 행 추가
	m_CtrlListMainProcess.InsertColumn(3, _T("T/T"), LVCFMT_CENTER, 50);// 리스트 행 추가
	//m_CtrlListMainProcess.InsertColumn(4, _T("Discription"), LVCFMT_LEFT, 350);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(3, _T("Wait"), LVCFMT_LEFT, 50);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(4, _T("Time"), LVCFMT_CENTER, 70);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(5, _T("Ref1"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(6, _T("Ref2"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(7, _T("Value"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(8, _T("OK/NG"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(9, _T("T/T"), LVCFMT_CENTER, 50);// 리스트 행 추가

	m_CtrlListMainProcess.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
	m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T("FM TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 3, LVIF_TEXT, _T("2.5"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 4, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 5, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 6, LVIF_TEXT, _T("80%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 7, LVIF_TEXT, _T("90%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 8, LVIF_TEXT, _T("OK"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 

	
	
	font = new CFont;
	font->CreateFontA( 16,                          // nHeight
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


	//font->CreateFontA( 20,                          // nHeight
	//				   0,                          // nWidth
	//				   0,                          // nEscapement
	//				   0,                          // nOrientation
	//				   400,                           // nWeight
	//				   0,                          // bItalic
	//				   0,                          // bUnderline 
	//				   0,                          // cStrikeOut 
	//				   0,                          // nCharSet
	//				   0,           // nOutPrecision 
	//				   0,                          // nClipPrecision 
	//				 PROOF_QUALITY,              // nQuality
	//				 0,  // nPitchAndFamily 
	//				 _T("Arial"));                     // lpszFacename
	//font->CreatePointFont(20,_T("Arial"));   
	m_CtrlListMainProcess.SetFont(font);

	delete font;
	
}


void CAcqVoltageSamples_IntClkDlg::SetSubListControl()
{
	
	CSize lSize;
	CString strTemp;

	m_CtrlListMainProcess.DeleteAllItems();

	mCheckAll = 1;
	
	for(int  i = 0; i < gTestSeqData.TestCnt; i++)
	{
		strTemp.Format(_T("%d"), i+1);
		m_CtrlListMainProcess.InsertItem(i, strTemp);   


		strTemp = gTestSeqData.TestStepList[i].strStepName;
		m_CtrlListMainProcess.SetItem(i, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
		m_CtrlListMainProcess.SetItem(i, 2, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
		m_CtrlListMainProcess.SetItem(i, 3, LVIF_TEXT, "0", NULL, NULL, NULL, NULL);
		strTemp = gTestSeqData.TestStepList[i].strStepWork;
//		m_CtrlListMainProcess.SetItem(i, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		gTestSeqData.TestStepList[i].bEnable = 1;
		if(gTestSeqData.TestStepList[i].bEnable == 1)
		{
			m_CtrlListMainProcess.SetCheck(i,1);
		}
		else
		{
			m_CtrlListMainProcess.SetCheck(i,0);
		}			
	}

	m_CtrlListMainProcess.Invalidate();
}



void CAcqVoltageSamples_IntClkDlg::OnHdnItemclickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

		
	int lint  = phdr->iItem;
	int i,lcnt;
	if(lint == 0)
	{
		if(mCheckAll)
		{
			mCheckAll = 0;
			lcnt = m_CtrlListMainProcess.GetItemCount();
			for(i = 0; i < lcnt; i++)
			{
				m_CtrlListMainProcess.SetCheck(i, 0);
			}
		}
		else
		{
			mCheckAll = 1;
			lcnt = m_CtrlListMainProcess.GetItemCount();
			for(i = 0; i < lcnt; i++)
			{
				m_CtrlListMainProcess.SetCheck(i, 1);
			}
		}
	}
	*pResult = 0;

}


void CAcqVoltageSamples_IntClkDlg::OnNMClickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	*pResult = 0;
	/*
	int i,lcnt;	
	lcnt = m_CtrlListMainProcess.GetItemCount();
		
	for(i = 0; i < lcnt; i++)
	{
		if(gTestSeqData.TestStepList[i].bEnable == 1)
		{
			m_CtrlListMainProcess.SetCheck(i, 1);
		}
		else
		{
			m_CtrlListMainProcess.SetCheck(i, 0);
		}
	}	*/
}


void CAcqVoltageSamples_IntClkDlg::OnNMCustomdrawListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
		
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	//int nRow=0;
	//int nCol=0;
	CString strCol=_T("");



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



			if(2==(int)(pLVCD->iSubItem) )
			{
				strCol= m_CtrlListMainProcess.GetItemText(pLVCD->nmcd.dwItemSpec,2);
				if( strCol=="NG") // 두번째열의 값이 "10"일 때, 그 값 부분만			
				{
					crText = RGB(0,0,0); //글자색
					crBkgnd = RGB(255,0,0); //배경색으로 한다    
				}
				else if(2==(int)(pLVCD->iSubItem) && strCol=="OK")          
				{
					crText = RGB(0,0,0);
					crBkgnd = RGB(0,255,0);
				}	
				else
				{
					if((nItem == m_nCurrentNumber)&&(m_TaskRun != TASK_RUN_IDLE))//else if(nItem == m_Process_Index)          
					{
						crText = RGB(0,0,0);
						crBkgnd = RGB(255,255,100);
					}
					else 
					{
						crText = RGB(0,0,0);
						crBkgnd = RGB(255,255,255);
					}
				}
		
			}
			else
			{
				//int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
				//m_TaskRun;m_Process_Index
	//int		m_nCurrentNumber;
				if((nItem == m_nCurrentNumber)&&(m_TaskRun != TASK_RUN_IDLE))
				{
					//if(pLVCD->iSubItem >= 1)
					{
						crText = RGB(0,0,0);
						crBkgnd = RGB(255,255,100);
					}
			
				}
				else
				{

					crText = RGB(0,0,0);
					crBkgnd = RGB(255,255,255);
				}
			}

			pLVCD->clrText = crText;
			pLVCD->clrTextBk= crBkgnd;			


			*pResult = CDRF_DODEFAULT; 
			return;
		}
	}
	
	*pResult = 0;
}


void CAcqVoltageSamples_IntClkDlg::OnStnDblclickStaticPic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}


void CAcqVoltageSamples_IntClkDlg::OnStnClickedStaticPic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDlgImage ldlg;
//	ldlg.DoModal();
}
BEGIN_EVENTSINK_MAP(CAcqVoltageSamples_IntClkDlg, CDialog)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_CAMREA_NAME, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelCamreaName, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabel, VTS_DISPATCH)
//	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG1, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelOK1, VTS_DISPATCH)
//	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG2, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelOK2, VTS_DISPATCH)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_COM_GMES, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelComGmes, VTS_DISPATCH)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_COM_MAIN, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelComMain, VTS_DISPATCH)
//	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG1, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelOK1, VTS_NONE)
//	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL__OKNG2, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelOK2, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_CHECK_TIME, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelCheckTime, VTS_DISPATCH)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_NG_CNT, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelNgCnt, VTS_DISPATCH)
	//ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_MANAGER, DISPID_DBLCLICK, CAcqVoltageSamples_IntClkDlg::DblClickLabelManager, VTS_DISPATCH)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_GET_SW, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelGetSw, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_GET_DARK, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelGetDark, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_GET_BRIGHT, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelGetBright, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_WIFI_CHECK, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelWifiCheck, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_GET_AED_SENSOR, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelAEDSensor, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_MAC_CHECK_MARK, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelMacCheckMark, VTS_NONE)
	ON_EVENT(CAcqVoltageSamples_IntClkDlg, IDC_LABEL_BARCODE_MARK, DISPID_CLICK, CAcqVoltageSamples_IntClkDlg::ClickLabelBarcodeMark, VTS_NONE)
END_EVENTSINK_MAP()


void CAcqVoltageSamples_IntClkDlg::ClickLabelCamreaName()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(g_CameraFound == 0)
	{
		return;
	}
	if(gbManualScopeMode)
	{
		MessageBox("MANUAL SCOPE MODE!!");
		return;

	}
	if(m_TaskRun != TASK_RUN_IDLE)
	{	
		
		return;
	}
	//CDlgImage ldlg;
	//if(g_CameraFound == 1)
	//{
	//	ldlg.m_ImageFileName = m_strCameraCheckIMGfile;
	//	ldlg.DoModal();
	//}
	//else
	//{
	//	MessageBox("Camrea Not Connected!");
	//}
		
}


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
	m_EthernetMode = ETHERNET_MODE_CREATE_SOCKET;
	CDialog::OnRButtonDblClk(nFlags, point);
}

void CAcqVoltageSamples_IntClkDlg::SendOkSignalToJig()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	BYTE lbuf[128]; 
	//int lcnt;
	//UINT m_EditFreq1;

	lbuf[0] = 0x02;	
	
	sprintf((char *)&lbuf[1], "CSENDOK01");
  //sprintf((char *)&lbuf[1], "CMAC_READ");
	
	
	lbuf[10] = 0x03;
	lbuf[11] = 0x0d;
	lbuf[12] = 0x0a;
	


	
}


void CAcqVoltageSamples_IntClkDlg::NG_Display(CString lstr)
{

	mpDlgSimpleInform->ShowWindow(SW_HIDE);

	gSaveInfo.strTestDiscription = m_GMES_ErrorName;
	gSaveInfo.strTestDiscription += m_GMES_ErrorValue;


	mpDlgResult->m_DisplayString = m_Deisplay_ErrorName;//m_GMES_ErrorName;
	mpDlgResult->m_DetailMessage = m_Deisplay_ErrorDetail;//m_GMES_ErrorValue;
	CString m_NgWaveFile  = _T(PATH_BASE_DIR);
	m_NgWaveFile  += _T("\\Sound\\NG_CHECK.wav");//OK_CHECK.wav

//	sndPlaySound( m_NgWaveFile , SND_ASYNC);
//	Beep(1200, 300);
	/*	if(ldlg.DoModal() == IDOK)
		{
		}*/

		mpDlgResult->m_Barcode = "";
		mpDlgResult->m_Result = 0;
		mpDlgResult->m_ReturnFlag = 0;
		CRect rectP;
		CRect rect;		
		
		this->GetWindowRect(&rectP);
		mpDlgResult->GetWindowRect(&rect);

		mpDlgResult->ShowWindow(SW_SHOW);
		mpDlgResult->SetWindowPos(this,rectP.left+ 270,rectP.top+150,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);


}

void CAcqVoltageSamples_IntClkDlg::OK_Display()
{

	mpDlgSimpleInform->ShowWindow(SW_HIDE);
		//CDlgResult ldlg;
		//ldlg.m_Result = 1;
		//Beep(800, 300);
		//if(ldlg.DoModal() == IDOK)
		//{
		//}
		mpDlgResult->m_Barcode = "";
		
		mpDlgResult->m_Result = 1;
		mpDlgResult->m_ReturnFlag = 0;
		mpDlgResult->m_DisplayString = "NO ERROR";//m_GMES_ErrorName;
		mpDlgResult->m_DetailMessage = "";
		CRect rectP;
		CRect rect;		
		
		this->GetWindowRect(&rectP);
		mpDlgResult->GetWindowRect(&rect);

		mpDlgResult->ShowWindow(SW_SHOW);
		mpDlgResult->SetWindowPos(this,rectP.left+ 270,rectP.top+150,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);

		gSaveInfo.strTestDiscription = "CHECK PASS";

}

void CAcqVoltageSamples_IntClkDlg::BarcodeErrorDisplay()
{
		//CDlgResult ldlg;
		//ldlg.m_Result = 1;
		//Beep(800, 300);
		//if(ldlg.DoModal() == IDOK)
		//{
		//}

		
		mpDlgBarcode->m_Barcode = "";
		mpDlgBarcode->m_ReturnFlag = 0;
		CRect rectP;
		CRect rect;		
		
		this->GetWindowRect(&rectP);
		mpDlgBarcode->GetWindowRect(&rect);

		mpDlgBarcode->ShowWindow(SW_SHOW);
		mpDlgBarcode->SetWindowPos(this,rectP.left+ 270,rectP.top+400,rect.Width(),rect.Height(),SWP_ASYNCWINDOWPOS);

}


void CAcqVoltageSamples_IntClkDlg::CLOSE_Result_Display()
{
	if(mpDlgResult->IsWindowEnabled())
	{
		if(mpDlgResult->m_Barcode.GetLength() > 6)
		{
			m_BarCode.put_Caption(mpDlgResult->m_Barcode);
		}
	}
	mpDlgResult->ShowWindow(SW_HIDE);
	mpDlgBarcode->ShowWindow(SW_HIDE);

	mpDlgSimpleInform->ShowWindow(SW_HIDE);
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

void CAcqVoltageSamples_IntClkDlg::DblClickLabel(LPDISPATCH Cancel)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	return;
	if(m_TaskRun != TASK_RUN_IDLE) 
	{
		MessageBox("TEST PROCESSING");
		return;
	}

	if(gbManualScopeMode == 1)		
	{

		gbManualScopeMode = 0;
		//SetGuiPos(gbManualScopeMode);

		//SetChannelData(0);
		
		//InitChannel();
		if(m_InitAiChannel == 0)
		{
			MessageBox("Audio Capture Card INIT FAIL!!");
			//g_NIAuioBoardFail = 1;//AUDIO Board Fail
		}

		m_cLbOkNg.put_Caption(_T("WAIT"));
		m_cLbOkNg.put_BackColor(0x0000FFFF);
		Invalidate();
	}
	else
	{
		gbManualScopeMode = 1;
		//SetGuiPos(gbManualScopeMode);	
		
		
	//	SetChannelDataManual(0);
		
	//	InitChannel();
		if(m_InitAiChannel == 0)
		{
			MessageBox("Audio Capture Card INIT FAIL!!");
			//g_NIAuioBoardFail = 1;//AUDIO Board Fail
		}
		m_cLbOkNg.put_Caption(_T("SCOPE MODE"));
		m_cLbOkNg.put_BackColor(0x0000FFFF);
		
		//m_cLb_Test1.put_Caption(_T("MIC1"));
		//m_cLb_Test1.put_BackColor(0x0000FFFF);
	
		//m_cLb_Test2.put_Caption(_T("MIC2"));
		//m_cLb_Test2.put_BackColor(0x0000FFFF);
	


		Invalidate();
	}
}

//
//void CAcqVoltageSamples_IntClkDlg::SetGuiPos(int IsScopeMode)
//{
//	if(IsScopeMode)
//	{
//
//
//		int Xoffset = -20;
//	//	GetDlgItem(IDC_LABEL__NAME1)->SetWindowPos(NULL,60, 105, 160,60,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[0].SetWindowPos(NULL,35+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[0].SetWindowPos(NULL,1600+35+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[0].SetWindowPos(NULL,155+10+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[0].SetWindowPos(NULL,245+45+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH0)->SetWindowPos(NULL,35+Xoffset, 180, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_cComboName[1].SetWindowPos(NULL,35+Xoffset,			345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[1].SetWindowPos(NULL,1600+35+Xoffset,			345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[1].SetWindowPos(NULL,155+10+Xoffset,		345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[1].SetWindowPos(NULL,245+45+Xoffset,		345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH1)->SetWindowPos(NULL,35+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test1.SetWindowPos(NULL,235+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//		//m_cLb_Test1.SetWindowPos(NULL,1600+35+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//		//m_cLb_Test1.put_Caption("MIC1");
//		//m_cLb_Test1.put_BackColor(0x0000FFFF);
//	//	m_cLb_Test1.Invalidate();
//
//		/////////////
//		//m_cComboName[2].SetWindowPos(NULL,345+80+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[2].SetWindowPos(NULL,1600+345+80+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[2].SetWindowPos(NULL,465+80+10+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[2].SetWindowPos(NULL,555+80+45+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH2)->SetWindowPos(NULL,345+80+Xoffset, 180, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_cComboName[3].SetWindowPos(NULL,345+80+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[3].SetWindowPos(NULL,1600+345+80+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[3].SetWindowPos(NULL,465+80+10+Xoffset, 345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[3].SetWindowPos(NULL,555+80+45+Xoffset, 345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH3)->SetWindowPos(NULL,345+80+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//		//m_cLb_Test2.SetWindowPos(NULL,1600+345+80+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test2.SetWindowPos(NULL,545+80+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//		//m_cLb_Test2.put_Caption("MIC2");
//		//m_cLb_Test2.put_BackColor(0x0000FFFF);
//	/*	RECT lRect;
//		lRect.top = 540+95;
//		lRect.bottom = 540+95+30;
//		lRect.left = 35+Xoffset;
//		lRect.top = 35+Xoffset +345+ 80+300+80 ;
//		InvalidateRect(&lRect);*/
//	//	m_cLb_Test2.Invalidate();
//
//		//m_cComboName[4].SetWindowPos(NULL,655+80*2+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[4].SetWindowPos(NULL,1600+655+80*2+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[4].SetWindowPos(NULL,775+80*2+10+Xoffset, 150,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[4].SetWindowPos(NULL,865+80*2+45+Xoffset, 150,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH4)->SetWindowPos(NULL,655+80*2+Xoffset, 180, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_cComboName[5].SetWindowPos(NULL,655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[5].SetWindowPos(NULL,1600+655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[5].SetWindowPos(NULL,775+80*2+10+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[5].SetWindowPos(NULL,865+80*2+45+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH5)->SetWindowPos(NULL,655+80*2+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test3.SetWindowPos(NULL,1600+655+80*2+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//	//m_cLb_Test3.SetWindowPos(NULL,655+80*2+Xoffset, 540+95, 100,30,SWP_ASYNCWINDOWPOS);
//
//		//m_cComboName[6].SetWindowPos(NULL,965+80*3+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[6].SetWindowPos(NULL,1600+965+80*3+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[6].SetWindowPos(NULL,1085+80*3+10+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[6].SetWindowPos(NULL,1175+80*3+45+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH6)->SetWindowPos(NULL,965+80*3+Xoffset, 180, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_cComboName[7].SetWindowPos(NULL,965+80*3+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[7].SetWindowPos(NULL,1600+965+80*3+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[7].SetWindowPos(NULL,1085+80*3+10+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[7].SetWindowPos(NULL,1175+80*3+45+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH7)->SetWindowPos(NULL,965+80*3+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//		m_CtrlListMainProcess.SetWindowPos(NULL,1600+1205+Xoffset, 150, 380, 420+95,SWP_ASYNCWINDOWPOS);//1015
//		GetDlgItem(IDC_STATIC_PIC)->SetWindowPos(NULL,1600+1205+Xoffset, 575+95, 380, 110,SWP_ASYNCWINDOWPOS);
//
//		
//		m_cLabelCableManager.SetWindowPos(NULL,1205+Xoffset, 575+95, 220, 30,SWP_ASYNCWINDOWPOS);
//		m_cLabelCableMessage.SetWindowPos(NULL,1205+Xoffset, 575+125, 220, 80,SWP_ASYNCWINDOWPOS);
//
//	
//		//965
//	//	m_cLbOkNg.SetWindowPos(NULL,35, 575, 1290,110,SWP_ASYNCWINDOWPOS);
//	//	m_cLbOkNg.SetWindowPos(NULL,35, 575, 960+130,110,SWP_ASYNCWINDOWPOS);//1125
//
//	//	m_MessageDisplay.SetWindowPos(NULL,35, 690, 1090,35,SWP_ASYNCWINDOWPOS);
//		m_MessageDisplay.SetWindowPos(NULL,165+Xoffset, 785, 960+255,35,SWP_ASYNCWINDOWPOS);
//
//		//95
//
//
//		m_OKCntLabelName.SetWindowPos(NULL,35+Xoffset, 688+95, 50,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabelName.SetWindowPos(NULL,35+Xoffset, 707+95, 50,18,SWP_ASYNCWINDOWPOS);
//		m_OKCntLabel.SetWindowPos(NULL,85+Xoffset, 688+95, 75,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabel.SetWindowPos(NULL,85+Xoffset, 707+95, 75,18,SWP_ASYNCWINDOWPOS);
//		/*	
//		m_OKCntLabelName.SetWindowPos(NULL,1130, 650, 50,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabelName.SetWindowPos(NULL,1130, 669, 50,18,SWP_ASYNCWINDOWPOS);
//		m_OKCntLabel.SetWindowPos(NULL,1240, 650, 75,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabel.SetWindowPos(NULL,1240, 669, 75,18,SWP_ASYNCWINDOWPOS);
//	*/
//		m_CheckTimeLabelName.SetWindowPos(NULL,1130+255+Xoffset, 688+95, 110,18,SWP_ASYNCWINDOWPOS);
//		m_CycleTimeLabelName.SetWindowPos(NULL,1130+255+Xoffset, 707+95, 110,18,SWP_ASYNCWINDOWPOS);
//		m_CheckTimeLabel.SetWindowPos(NULL,1240+255+Xoffset, 688+95, 85,18,SWP_ASYNCWINDOWPOS);
//		m_CycleTimeLabel.SetWindowPos(NULL,1240+255+Xoffset, 707+95, 85,18,SWP_ASYNCWINDOWPOS);
//
//	
//
//		m_cLbOkNg.SetWindowPos(NULL,235+Xoffset, 575+95, 720+240,110,SWP_ASYNCWINDOWPOS);
//
//	
//		m_LanLabelName.SetWindowPos(NULL,35+Xoffset, 575+95, 80,26,SWP_ASYNCWINDOWPOS);
//		m_Usb1LabelName.SetWindowPos(NULL,35+Xoffset, 603+95, 80,26,SWP_ASYNCWINDOWPOS);
//		m_Usb2LabelName.SetWindowPos(NULL,35+Xoffset, 631+95, 80,26,SWP_ASYNCWINDOWPOS);
//		m_CameraLabelName.SetWindowPos(NULL,35+Xoffset, 659+95, 80,26,SWP_ASYNCWINDOWPOS);
//
//		m_cLb_Lan.SetWindowPos(NULL,115+Xoffset, 575+95, 110,26,SWP_ASYNCWINDOWPOS);
//		m_cLb_Usb1.SetWindowPos(NULL,115+Xoffset, 603+95, 110,26,SWP_ASYNCWINDOWPOS);
//		m_cLb_Usb2.SetWindowPos(NULL,115+Xoffset, 631+95, 110,26,SWP_ASYNCWINDOWPOS);
//		m_cLb_Camera.SetWindowPos(NULL,115+Xoffset, 659+95, 110,26,SWP_ASYNCWINDOWPOS);
//
//	}
//	else
//	{
//		int Xoffset = -20;
//	//	GetDlgItem(IDC_LABEL__NAME1)->SetWindowPos(NULL,60, 105, 160,60,SWP_ASYNCWINDOWPOS);
//		//m_NameLabel[0].SetWindowPos(NULL,35+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[0].SetWindowPos(NULL,1600+35+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[0].SetWindowPos(NULL,155+10+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[0].SetWindowPos(NULL,245+45+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH0)->SetWindowPos(NULL,35+Xoffset, 180, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_NameLabel[1].SetWindowPos(NULL,35+Xoffset,			345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[1].SetWindowPos(NULL,1600+35+Xoffset,			345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[1].SetWindowPos(NULL,155+10+Xoffset,		345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[1].SetWindowPos(NULL,245+45+Xoffset,		345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH1)->SetWindowPos(NULL,35+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test1.SetWindowPos(NULL,35+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test1.put_Caption("WAIT");
//		/////////////
//		//m_NameLabel[2].SetWindowPos(NULL,345+80+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[2].SetWindowPos(NULL,1600+345+80+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[2].SetWindowPos(NULL,465+80+10+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[2].SetWindowPos(NULL,555+80+45+Xoffset, 150, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH2)->SetWindowPos(NULL,345+80+Xoffset, 180, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_NameLabel[3].SetWindowPos(NULL,345+80+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[3].SetWindowPos(NULL,1600+345+80+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[3].SetWindowPos(NULL,465+80+10+Xoffset, 345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[3].SetWindowPos(NULL,555+80+45+Xoffset, 345+45,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH3)->SetWindowPos(NULL,345+80+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test2.SetWindowPos(NULL,345+80+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test2.put_Caption("WAIT");
//
//		//m_NameLabel[4].SetWindowPos(NULL,655+80*2+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[4].SetWindowPos(NULL,1600+655+80*2+Xoffset, 150, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[4].SetWindowPos(NULL,775+80*2+10+Xoffset, 150,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[4].SetWindowPos(NULL,865+80*2+45+Xoffset, 150,  90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH4)->SetWindowPos(NULL,655+80*2+Xoffset, 180, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_NameLabel[5].SetWindowPos(NULL,655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[5].SetWindowPos(NULL,1600+655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[5].SetWindowPos(NULL,775+80*2+10+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[5].SetWindowPos(NULL,865+80*2+45+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH5)->SetWindowPos(NULL,655+80*2+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//		m_cLb_Test3.SetWindowPos(NULL,655+80*2+Xoffset, 540+95, 300+80,30,SWP_ASYNCWINDOWPOS);
//
//		//m_NameLabel[6].SetWindowPos(NULL,1600+655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[6].SetWindowPos(NULL,1600+655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[6].SetWindowPos(NULL,1600+775+80*2+10+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[6].SetWindowPos(NULL,1600+865+80*2+45+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH6)->SetWindowPos(NULL,1600+655+80*2+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//
//		//m_NameLabel[7].SetWindowPos(NULL,1600+655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_cComboName[7].SetWindowPos(NULL,1600+655+80*2+Xoffset, 345+45, 120+10, 30,SWP_ASYNCWINDOWPOS);
//		//m_FreqLabel[7].SetWindowPos(NULL,1600+775+80*2+10+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		//m_VoltLabel[7].SetWindowPos(NULL,1600+865+80*2+45+Xoffset, 345+45, 90+35, 30,SWP_ASYNCWINDOWPOS);
//		GetDlgItem(IDC_CWGRAPH7)->SetWindowPos(NULL,1600+655+80*2+Xoffset, 375+45, 300+80,160+40,SWP_ASYNCWINDOWPOS);
//
//		m_CtrlListMainProcess.SetWindowPos(NULL,1205+Xoffset, 150, 380, 420+95,SWP_ASYNCWINDOWPOS);//1015
//		GetDlgItem(IDC_STATIC_PIC)->SetWindowPos(NULL,1600+1205+Xoffset, 575+95, 380, 110,SWP_ASYNCWINDOWPOS);
//
//		m_cLabelCableManager.SetWindowPos(NULL,1205+Xoffset, 575+95, 220, 30,SWP_ASYNCWINDOWPOS);
//		m_cLabelCableMessage.SetWindowPos(NULL,1205+Xoffset, 575+125, 220, 80,SWP_ASYNCWINDOWPOS);
//
//	
//		//965
//	//	m_cLbOkNg.SetWindowPos(NULL,35, 575, 1290,110,SWP_ASYNCWINDOWPOS);
//	//	m_cLbOkNg.SetWindowPos(NULL,35, 575, 960+130,110,SWP_ASYNCWINDOWPOS);//1125
//
//	//	m_MessageDisplay.SetWindowPos(NULL,35, 690, 1090,35,SWP_ASYNCWINDOWPOS);
//		m_MessageDisplay.SetWindowPos(NULL,165+Xoffset, 785, 960+255,35,SWP_ASYNCWINDOWPOS);
//
//		//95
//
//
//		m_OKCntLabelName.SetWindowPos(NULL,35+Xoffset, 688+95, 50,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabelName.SetWindowPos(NULL,35+Xoffset, 707+95, 50,18,SWP_ASYNCWINDOWPOS);
//		m_OKCntLabel.SetWindowPos(NULL,85+Xoffset, 688+95, 75,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabel.SetWindowPos(NULL,85+Xoffset, 707+95, 75,18,SWP_ASYNCWINDOWPOS);
//		/*	
//		m_OKCntLabelName.SetWindowPos(NULL,1130, 650, 50,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabelName.SetWindowPos(NULL,1130, 669, 50,18,SWP_ASYNCWINDOWPOS);
//		m_OKCntLabel.SetWindowPos(NULL,1240, 650, 75,18,SWP_ASYNCWINDOWPOS);
//		m_NGCntLabel.SetWindowPos(NULL,1240, 669, 75,18,SWP_ASYNCWINDOWPOS);
//	*/
//		m_CheckTimeLabelName.SetWindowPos(NULL,1130+255+Xoffset, 688+95, 110,18,SWP_ASYNCWINDOWPOS);
//		m_CycleTimeLabelName.SetWindowPos(NULL,1130+255+Xoffset, 707+95, 110,18,SWP_ASYNCWINDOWPOS);
//		m_CheckTimeLabel.SetWindowPos(NULL,1240+255+Xoffset, 688+95, 85,18,SWP_ASYNCWINDOWPOS);
//		m_CycleTimeLabel.SetWindowPos(NULL,1240+255+Xoffset, 707+95, 85,18,SWP_ASYNCWINDOWPOS);
//
//	
//
//		m_cLbOkNg.SetWindowPos(NULL,235+Xoffset, 575+95, 720+240,110,SWP_ASYNCWINDOWPOS);
//
//	
//		m_LanLabelName.SetWindowPos(NULL,35+Xoffset, 575+95, 80,26,SWP_ASYNCWINDOWPOS);
//		m_Usb1LabelName.SetWindowPos(NULL,35+Xoffset, 603+95, 80,26,SWP_ASYNCWINDOWPOS);
//		m_Usb2LabelName.SetWindowPos(NULL,35+Xoffset, 631+95, 80,26,SWP_ASYNCWINDOWPOS);
//		m_CameraLabelName.SetWindowPos(NULL,35+Xoffset, 659+95, 80,26,SWP_ASYNCWINDOWPOS);
//
//		m_cLb_Lan.SetWindowPos(NULL,115+Xoffset, 575+95, 110,26,SWP_ASYNCWINDOWPOS);
//		m_cLb_Usb1.SetWindowPos(NULL,115+Xoffset, 603+95, 110,26,SWP_ASYNCWINDOWPOS);
//		m_cLb_Usb2.SetWindowPos(NULL,115+Xoffset, 631+95, 110,26,SWP_ASYNCWINDOWPOS);
//		m_cLb_Camera.SetWindowPos(NULL,115+Xoffset, 659+95, 110,26,SWP_ASYNCWINDOWPOS);
//	}
//}


//void CAcqVoltageSamples_IntClkDlg::OnCbnSelchangeComboName1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//
//	for(int  i = 0; i < 8; i++)
//	{
//		g_ManualId[i] = //m_cComboName[i].GetCurSel();
//	}
//
//	CFileControl lFile;
//
//	lFile.SaveINIModel();
//	SetChannelDataManual(0);
//
//
//}

void CAcqVoltageSamples_IntClkDlg::AddGmesLog(CString LogStr, int Recv)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
	CFileControl lFile;

	lFile.SaveGmesLOG(LogStr, Recv);



}


void CAcqVoltageSamples_IntClkDlg::DblClickLabelComGmes(LPDISPATCH Cancel)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CFileControl lFile;
	if(g_MesEnable)
	{
		m_cLb_ComMainGmes.put_BackColor(0x000001FF);
		g_MesEnable = 0;
		CloseEthernetScocket() ;

		


		lFile.SaveModel(gTestSeqData.strModelName);
		
	}
	else
	{
		CloseEthernetScocket() ;
		m_EthernetMode =  ETHERNET_MODE_CREATE_SOCKET;
		m_EthernetWaitCnt = 0;
		g_nPortReopen = 0;
		g_MesEnable = 1;
		lFile.SaveModel(gTestSeqData.strModelName);
	}
}


void CAcqVoltageSamples_IntClkDlg::DblClickLabelComMain(LPDISPATCH Cancel)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_nPreCurrentStep = 0;	
	m_nPreCurrentProcess = 0;
	m_nPreCurrentNumber = 0;
	m_PreTaskRun = TASK_RUN_START;
		
	MessageDisplay(2, "Preprocess Start");
}

//void CAcqVoltageSamples_IntClkDlg::OnBnClickedButtonFreqSet()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	BYTE lbuf[128]; 
//	int lcnt;
//	//UINT m_EditFreq1;
//	
//
//	lbuf[0] = 0x02;
//	
//	//sprintf((char *)&lbuf[1], "CFREQ%02d%03d", 
//	sprintf((char *)&lbuf[1], "CFSET%03d%03d%03d%03d%03d%03d%03d%03d%03d%03d", 
//		gTestSeqData.Frequency[0]/10,
//		gTestSeqData.Frequency[1]/10,
//		gTestSeqData.Frequency[2]/10,
//		gTestSeqData.Frequency[3]/10,
//		gTestSeqData.Frequency[4]/10,
//		gTestSeqData.Frequency[5]/10,
//		gTestSeqData.Frequency[6]/10,
//		gTestSeqData.Frequency[7]/10,
//		gTestSeqData.Frequency[8]/10,
//		gTestSeqData.Frequency[9]/10
//		);
//	
//	lbuf[36] = 0x03;
//	lbuf[37] = 0x0d;
//	lbuf[38] = 0x0a;
//	//ReceiveData[18] == 0x03) && (ReceiveData[19] == 0x0d) && (ReceiveData[20] == 0x0a
//	SendData(lbuf, 39);//ComSend(lbuf, 39);
//	
//	//m_TESTPAGE_COM_PROCESS = COM_MODE_WAIT_ACK;
//	// m_ComAck =COM_ACK_WAIT;
//	// m_WaitCnt = 0;
//	// m_ResendCnt = 0;
//	// MessageDisplay(2, "Frequency Set ALL!");
//}


void CAcqVoltageSamples_IntClkDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialog::OnRButtonDown(nFlags, point);
}


void CAcqVoltageSamples_IntClkDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialog::OnRButtonUp(nFlags, point);
}

//
//void CAcqVoltageSamples_IntClkDlg::ClickLabelOK1()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	SetFreqSend(4, 1 );
//}
//
//
//void CAcqVoltageSamples_IntClkDlg::ClickLabelOK2()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	SetFreqSend(5, 1 );
//}
//
//void CAcqVoltageSamples_IntClkDlg::DblClickLabelOK1(LPDISPATCH Cancel)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	SetFreqSend(4, 0);	
//}
//
//void CAcqVoltageSamples_IntClkDlg::DblClickLabelOK2(LPDISPATCH Cancel)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	SetFreqSend(5, 0);	
//}

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
	char lBuf[2048];
	char ltmpBuf[2048];


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
//
//	int lGMES_Opened = 0;
//	int lGMES_Port = 0;
//	int lOldPortType = g_nRemotePortType;
////	if(g_nRemotePortType == LINE_CM89)
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
//	//m_MainPAGE_COM_PROCESS = COM_MODE_IDLE;
//	//m_LED_COM_PROCESS = COM_MODE_IDLE;
//	//m_PCB_COM_PROCESS = COM_MODE_IDLE;//gTestSeqData.TestStepList[m_nCurrentNumber].m_Result = 3;
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
//	//OnBnClickedButtonFreqSet();

}


void CAcqVoltageSamples_IntClkDlg::DblClickLabelCheckTime(LPDISPATCH Cancel)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(m_EnableCount == 1)
	{
		m_EnableCount = 0;
	}
	else
	{
		m_EnableCount = 1;
		m_RoopCnt = 0;
	}
}


void CAcqVoltageSamples_IntClkDlg::DblClickLabelNgCnt(LPDISPATCH Cancel)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	g_nNGCnt = 0;
	CFileControl lFile;
	lFile.SaveCountData();
	
	CString str;
	str.Format(_T("%d"), g_nOKCnt);	
	m_OKCntLabel.put_Caption(str);

	str.Format(_T("%d"), g_nNGCnt);	
	m_NGCntLabel.put_Caption(str);
}

//
//void CAcqVoltageSamples_IntClkDlg::DblClickLabelManager(LPDISPATCH Cancel)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	CCableChangeDlg ldlg;
//
//	ldlg.DoModal();
//
//	
//	int lcountError = 0;
//	CString str = "";
//	if(gCableManager.nUSB_CableCount >= gCableManager.nUSB_CableCountMax)
//	{
//		str = _T("Change USB Cable \r\n");
//	//	m_cLabelCableMessage.put_BackColor(0x00008FFF);
//		lcountError++;
//	}
//
//	if(gCableManager.nPort_CableCount >= gCableManager.nPort_CableCountMax)
//	{
//		str += _T("Change Portable Cable \r\n");
//	//	m_cLabelCableMessage.put_BackColor(0x00008FFF);
//		lcountError++;
//	}
//
//
//	if(gCableManager.nCM4CheckerCount >= gCableManager.nCM4CheckerCountMax)
//	{
//		str += _T("Change CM4 Checker Pin \r\n");
//	//	m_cLabelCableMessage.put_BackColor(0x00008FFF);
//		lcountError++;
//	}	
//	if(gCableManager.nCM5CheckerCount >= gCableManager.nCM5CheckerCountMax)
//	{
//		str += _T("Change CM5 Checker Pin \r\n");
//	//	m_cLabelCableMessage.put_BackColor(0x00008FFF);
//		lcountError++;
//	}
//	if(gCableManager.nMIC_CableCount >= gCableManager.nMIC_CableCountMax)
//	{
//		str += _T("Change MIC Cable \r\n");
//	//	m_cLabelCableMessage.put_BackColor(0x00008FFF);
//		lcountError++;
//	}		
//		
//	if(lcountError > 0)
//	{
//	}
//	else
//	{
//	//	m_cLabelCableMessage.put_BackColor(0x0008FF00);
//		str += _T("\r\n\r\nCheck Cable OK\r\n");
//	}
//	//m_cLabelCableMessage.put_Caption(str);
//}


void CAcqVoltageSamples_IntClkDlg::ClickLabelGetSw()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
GetDetParam();

	//CWTCPClient	tcpCli;
	//CCommPacket	pktRecv, pktSend;

	//// det param 2

	//if (!ConnectToDetector(tcpCli))  
	//	return;

	//pktSend.SetHeaderData(LDCMD_GET_PARAMETER, 0, 0);

	////send
	//if (!SendPacket(tcpCli, pktSend))
	//{
	//	DisconnectToDetector(tcpCli);
	//	return;
	//}

	////recv header
	//if (!RecvPacket(tcpCli, pktRecv))
	//{
	//	DisconnectToDetector(tcpCli);
	//	return;
	//}

	//DisconnectToDetector(tcpCli);

	//if (pktRecv.dwResponseCode != LDRET_SUCCESS)
	//	AfxMessageBox(_T("The detector returned not success."));
	//else {
	//	memcpy(&m_detParam2, pktRecv.m_pDataBuf, pktRecv.dwDataSize);

	//	TCHAR str[80];
	//	
	//	DWORD version;
	//	DWORD version_event;
	//	DWORD version_major;
	//	DWORD version_minor;
	//	DWORD version_release;
	//	version = m_detParam2.dwFPGAVersion;
	//	version_minor = version & 0x0ff;
	//	version >>= 8;
	//	version_major = version & 0xf;
	//	version >>= 4;
	//	version_event = version & 0x0f;
	//	wsprintf(str, _T("FPGA ver. %d.%d.%d"), version_event, version_major, version_minor);
	//	//m_staticFPGAVersion.SetWindowText(str);
	//	m_cLb_Version1.put_Caption(str);

	//	version = m_detParam2.dwDSERVERVersion;
	//	version_release = version & 0x0ff;
	//	version >>= 8;
	//	version_minor = version & 0x0ff;
	//	version >>= 8;
	//	version_major = version & 0x0ff;
	//	version >>= 8;
	//	version_event = version & 0x0ff;
	//	wsprintf(str, _T("DXD server ver. %d.%d%d.%d"), version_event, version_major, version_minor, version_release);
	//	//m_staticDSERVERVersion.SetWindowText(str);
	//	m_cLb_Version2.put_Caption(str);

	//	version = m_detParam2.dwMicrochipVersion;
	//	version_minor = version & 0x0ff;
	//	version >>= 8;
	//	version_major = version & 0x0ff;
	//	version >>= 8;
	//	version_event = version & 0x0ff;
	//	wsprintf(str, _T("Microchip ver. %d.%d.%d"), version_event, version_major, version_minor);
	//	//m_staticMICROCHIPVersion.SetWindowText(str);
	//	m_cLb_Version3.put_Caption(str);

	//	version = m_detParam2.dwAVRVersion;
	//	version_minor = version & 0x0ff;
	//	version >>= 8;
	//	version_major = version & 0x0ff;
	//	version >>= 8;
	//	version_event = version & 0x0ff;
	//	wsprintf(str, _T("AVR ver. %d.%d.%d"), version_event, version_major, version_minor);
	//	//m_staticAVRVersion.SetWindowText(str);
	//	m_cLb_Version4.put_Caption(str);

	//}
}

int CAcqVoltageSamples_IntClkDlg::GetDetParam()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	// det param
	if (!ConnectToDetector(tcpCli))
		return 0;

	pktSend.SetHeaderData(LDCMD_GET_PARAMETER, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
		AfxMessageBox(_T("The detector returned not success."));
	else {
		//TCHAR str[80];
		CString strTemp;

		memcpy(&m_detParam, pktRecv.m_pDataBuf, pktRecv.dwDataSize);
		//strTemp.Format(_T("%d"), m_detParam.dwExposureMode);// wsprintf(str, _T("%d"), m_detParam.dwExposureMode);
		if(m_detParam.dwExposureMode == 0)
		{
			m_cLb_Exposure.put_Caption("AUTO");//
		}
		else
		{
			m_cLb_Exposure.put_Caption("MAN");//, m_comboExposure.SetCurSel(_ttoi(str));
		}

		m_nExposure = m_detParam.dwExposureMode;

		m_nPanelWidth = m_detParam.dwImgCol;
		m_nPanelHeight = m_detParam.dwImgRow;

		gNoRoic = m_nPanelWidth / 256;
		m_RoicWidthEnd = 256;
		if (m_nPanelWidth % 256 != 0)
		{
			unsigned int lRoicWidthRsv = m_nPanelWidth - gNoRoic * 256;
			if (lRoicWidthRsv > 0)
			{
				m_RoicWidthEnd = lRoicWidthRsv;// (lRoicWidthRsv + 256) / 2;
				gNoRoic += 1;
			}
		}

		gNoGate = m_nPanelHeight / 512;
		m_GateHeightEnd = 512;
		if (m_nPanelHeight % 512 != 0)
		{
			unsigned int lGateHeightRsv = m_nPanelHeight - gNoGate * 512;
			if (lGateHeightRsv > 0)
			{
				m_GateHeightEnd = lGateHeightRsv;// (lRoicHeightRsv + 512) / 2;
				gNoGate += 1;
			}
		}


		//wsprintf(str, _T("%d"), m_detParam.dwImgCol);
		//m_staticPanelWidth.SetWindowText(str);
		//wsprintf(str, _T("%d"), m_detParam.dwImgRow);
		//m_staticPanelHeight.SetWindowText(str);
		strTemp.Format(_T("%d"), m_detParam.dwImgCol);
		m_cLb_ImageWidth.put_Caption(strTemp);
		strTemp.Format(_T("%d"), m_detParam.dwImgRow);
		m_cLb_ImageHeight.put_Caption(strTemp);
	}

	// det param 2

	if (!ConnectToDetector(tcpCli))  
		return 0;

	pktSend.SetHeaderData(LDCMD_GET_PARAMETER2, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
		AfxMessageBox(_T("The detector returned not success."));
	else {
		memcpy(&m_detParam2, pktRecv.m_pDataBuf, pktRecv.dwDataSize);

		TCHAR str[80];
		
	



		DWORD version;
		DWORD version_event;
		DWORD version_major;
		DWORD version_minor;
		DWORD version_release;
		version = m_detParam2.dwFPGAVersion;
		version_minor = version & 0x0ff;
		version >>= 8;
		version_major = version & 0xf;
		version >>= 4;
		version_event = version & 0x0f;
		wsprintf(str, _T("%d.%d.%d"), version_event, version_major, version_minor);
		m_cLb_Version1.put_Caption(str); //m_staticFPGAVersion.SetWindowText(str);

		m_FPGA_Ver_event = version_event;
		m_FPGA_Ver_major = version_major;
		m_FPGA_Ver_minor = version_minor;



		version = m_detParam2.dwDSERVERVersion;
		version_release = version & 0x0ff;
		version >>= 8;
		version_minor = version & 0x0ff;
		version >>= 8;
		version_major = version & 0x0ff;
		version >>= 8;
		version_event = version & 0x0ff;
		wsprintf(str, _T("%d.%d%d.%02d"), version_event, version_major, version_minor, version_release);
		m_cLb_Version2.put_Caption(str); //m_staticDSERVERVersion.SetWindowText(str);
		m_DSERVER_Ver_event = version_event;
		m_DSERVER_Ver_major = version_major;
		m_DSERVER_Ver_minor = version_minor;
		m_DSERVER_Ver_release = version_release;

		m_dwDSERVERVersion = m_detParam2.dwDSERVERVersion;
	


		version = m_detParam2.dwMicrochipVersion;
		version_minor = version & 0x0ff;
		version >>= 8;
		version_major = version & 0x0ff;
		version >>= 8;
		version_event = version & 0x0ff;
		wsprintf(str, _T("%d.%d.%d"), version_event, version_major, version_minor);
		m_cLb_Version3.put_Caption(str); // m_staticMICROCHIPVersion.SetWindowText(str);
		m_Microchip_Ver_event = version_event;
		m_Microchip_Ver_major = version_major;
		m_Microchip_Ver_minor = version_minor;

		if (g_AVR_Type_HK_HQ != m_detParam2.dwPlatformType)
		{
			//CString 
			if (m_detParam2.dwPlatformType == MICOM_TYPE_HK)
				MessageBox("Change HK Platform \r\n Check SET MODEL NAME ","DETECT HK Platform");
			else
				MessageBox("Change HQ Platform \r\n Check SET MODEL NAME ", "DETECT HQ Platform");

		}
		g_AVR_Type_HK_HQ = m_detParam2.dwPlatformType;

		if (g_AVR_Type_HK_HQ == MICOM_TYPE_HK)
		{
			version = m_detParam2.dwAVRVersion;
		}
		else
		{
			version = m_detParam2.dwDropMicomVersion;
		}

		version_minor = version & 0x0ff;
		version >>= 8;
		version_major = version & 0x0ff;
		version >>= 8;
		version_event = version & 0x0ff;
		wsprintf(str, _T("%d.%d.%d"), version_event, version_major, version_minor);
		m_cLb_Version4.put_Caption(str); // m_staticAVRVersion.SetWindowText(str);

		m_AVR_Ver_event = version_event;
		m_AVR_Ver_major = version_major;
		m_AVR_Ver_minor = version_minor;

		return 1;
	}

}


void CAcqVoltageSamples_IntClkDlg::ClickLabelGetDark()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedBtnGetdark();
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelGetBright()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedBtnGetbright();

}


void CAcqVoltageSamples_IntClkDlg::ClickLabelWifiCheck()
{

	WifiCheck();
	//// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CWTCPClient	tcpCli;
	//CCommPacket	pktRecv, pktSend;
	//DWORD		bWiFiModule = FALSE;
	//char		recvBuffer[512];

	//if (!ConnectToDetector(tcpCli))
	//	return;

	//pktSend.SetHeaderData(LDCMD_GET_WIFI_MODULE, 0, 0);

	////send
	//if (!SendPacket(tcpCli, pktSend))
	//{
	//	DisconnectToDetector(tcpCli);
	//	return;
	//}

	////recv header
	//if (!RecvPacket(tcpCli, pktRecv))
	//{
	//	DisconnectToDetector(tcpCli);
	//	return;
	//}

	//DisconnectToDetector(tcpCli);

	//if (pktRecv.dwResponseCode != LDRET_SUCCESS)
	//	AfxMessageBox(_T("The detector returned not success."));
	//else {
	//	memcpy(recvBuffer, pktRecv.m_pDataBuf, pktRecv.dwDataSize);
	//}

	//char str[512];
	//int idx = 0;
	//memset(str, 0, sizeof(str));
	//for (int i = 0; i < strlen(recvBuffer); i++)
	//{
	//	if (recvBuffer[i] != 0x0a)
	//		str[idx++] = recvBuffer[i];
	//	else {
	//		str[idx++] = 0x0d;
	//		str[idx++] = 0x0a;
	//	}
	//}
	//if (strlen(recvBuffer) < 50)
	//{
	//	//m_staticWiFiModule.SetWindowText(_T("Disconnected"));
	//	m_cLb_WIFI.put_Caption(_T("Disconnected"));
	//}
	//else
	//{
	//	m_cLb_WIFI.put_Caption(_T("Connected"));
	//}

	////m_editWiFiInfo.SetWindowText(CString(str));
	//AddLogData(str);
}

int CAcqVoltageSamples_IntClkDlg::WifiCheck()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;
	DWORD		bWiFiModule = FALSE;
	char		recvBuffer[512];

	if (!ConnectToDetector(tcpCli))
		return 0;

	pktSend.SetHeaderData(LDCMD_GET_WIFI_MODULE, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
		AfxMessageBox(_T("The detector returned not success."));
	else {
		memcpy(recvBuffer, pktRecv.m_pDataBuf, pktRecv.dwDataSize);
	}

	char str[512];
	int idx = 0;
	memset(str, 0, sizeof(str));
	for (int i = 0; i < strlen(recvBuffer); i++)
	{
		if (recvBuffer[i] != 0x0a)
			str[idx++] = recvBuffer[i];
		else {
			str[idx++] = 0x0d;
			str[idx++] = 0x0a;
		}
	}
	if (strlen(recvBuffer) < 50)
	{
		//m_staticWiFiModule.SetWindowText(_T("Disconnected"));
		m_cLb_WIFI.put_Caption(_T("Disconnected"));
		return 0;
	}
	else
	{
		m_cLb_WIFI.put_Caption(_T("Connected"));
		return 1;
	}

	//m_editWiFiInfo.SetWindowText(CString(str));
	AddLogData(str);
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelAEDSensor()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	OnBnClickedBtnGetAedSensor();
}



BOOL CAcqVoltageSamples_IntClkDlg::ConnectToDetector(CWTCPClient & tcpCli)
{
	int		nPort;
	CString	strAddr, strPort, strMesg;

	//m_editIPAddr.GetWindowText(strAddr);
	strAddr = g_nSetIPAddress;	
	nPort = g_nSetPortNumber;//5050;

	strMesg.Format(_T("Connect to detector : [%s:%d]"), (LPCTSTR) strAddr, nPort);
	AddLogData(strMesg);

	if (tcpCli.Connect(CStringA(strAddr), nPort, 1000) != 0)
	{
		AddLogData(_T("     Fail"));
		return FALSE;
	}
	
	AddLogData(_T("     Success"));

	return TRUE;
}

BOOL CAcqVoltageSamples_IntClkDlg::DisconnectToDetector(CWTCPClient & tcpCli)
{
	AddLogData(_T("Disconnect to detector"));
	AddLogData(_T("----------------------"));
	AddLogData(_T(""));

	tcpCli.CloseSocket();

	return TRUE;
}

BOOL CAcqVoltageSamples_IntClkDlg::SendPacket(CWTCPClient & tcpCli, CCommPacket & pktSend)
{
	AddLogData(pktSend, TRUE);

	CWError	wErr;

	wErr = pktSend.SendPacket(tcpCli, false);
	if (wErr.IsSuccess())
		AddLogData(_T("     Success"));
	else
		AddLogData(_T("     Fail"));

	return wErr.IsSuccess();
}

BOOL CAcqVoltageSamples_IntClkDlg::RecvPacket(CWTCPClient & tcpCli, CCommPacket & pktRecv)
{
	CWError	wErr;

	wErr = pktRecv.RecvPacket(tcpCli);
	if (! wErr.IsSuccess())
		AddLogData(_T("Receving data Fail..."));
	else
		AddLogData(pktRecv, FALSE);

	return wErr.IsSuccess();
}

BOOL CAcqVoltageSamples_IntClkDlg::SendACK(CWTCPClient & tcpCli)
{
	AddLogHandShake(LGD_COMM_ACK, TRUE);

	DWORD	dwCode = LGD_COMM_ACK;

	if (tcpCli.Send(& dwCode, 4) != 0)
	{
		AddLogData(_T("     Fail"));
		return FALSE;
	}

	AddLogData(_T("     Success"));
	return TRUE;
}

BOOL CAcqVoltageSamples_IntClkDlg::RecvHandShake(CWTCPClient & tcpCli, DWORD & dwCode)
{
	dwCode = 0;
	if (tcpCli.RecvExact(& dwCode, 4) != 0)
	{
		AddLogData(_T("Receving data Fail..."));
		return FALSE;
	}

	AddLogHandShake(dwCode, FALSE);
	return TRUE;
}

void CAcqVoltageSamples_IntClkDlg::AddLogData(LPCTSTR pszMesg)
{
	if (! ::IsWindow(m_listLog.m_hWnd))
		return;

	SYSTEMTIME	st;
	::GetLocalTime(& st);

	CString	str;
	str.Format(_T("[%02d:%02d:%03d] %s"), st.wMinute, st.wSecond, st.wMilliseconds, pszMesg);
	int		nIdx = m_listLog.AddString(str);
	m_listLog.SetCurSel(nIdx);

	SaveFileLog(str);

	//m_listLog.AddString(_T("1"));
	//m_listLog.AddString(_T("2"));
	//m_listLog.AddString(_T("3"));
}

void CAcqVoltageSamples_IntClkDlg::AddLogData(const CCommPacket & pktCommm, BOOL bSend)
{
	if (! ::IsWindow(m_listLog.m_hWnd))
		return;

	if (bSend)
		AddLogData(_T("Sending Packet"));
	else
		AddLogData(_T("Received Packet"));

	CString	str;
	char	szCmdName[128], szResName[128];

	CCommPacket::GetCommandName((PacketCommand) pktCommm.dwCmd, szCmdName, 128);
	CCommPacket::GetResponseCodeName((ResponseCode) pktCommm.dwResponseCode, szResName, 128);

	str.Format(_T("     Cmd : 0x%04x (%s)"), pktCommm.dwCmd, (LPCTSTR) CString(szCmdName));
	AddLogData(str);

	str.Format(_T("     ResponseCode : 0x%04x (%s)"), pktCommm.dwResponseCode, (LPCTSTR) CString(szResName));
	AddLogData(str);

	str.Format(_T("     DataSize : %d"), pktCommm.dwDataSize);
	AddLogData(str);

	SaveFileLogData(pktCommm, bSend);


}

void CAcqVoltageSamples_IntClkDlg::SaveFileLog(CString	str)
{
	//if (! ::IsWindow(m_listLog.m_hWnd))
	//	return;


//	CString	str;
	char	szCmdName[128], szResName[128];



		SYSTEMTIME	st;
	GetLocalTime(&st);//PATH_BASE_DIR

//	::CreateDirectory(g_theApp.m_strSavePath, NULL);
	CString m_strPath;
	m_strPath = PATH_BASE_DIR;
	m_strPath += "\\Log";

	TCHAR	szMesg[4096], szPath[256];
	int lSize = 0;


	_stprintf(szPath, _T("%s\\%s_%04d%02d%02d_%02d.logdbg"),
		(LPCTSTR)m_strPath,
		m_WipID,
		st.wYear, st.wMonth, st.wDay, st.wHour);

	FILE*	fp;
	if ((fp = _tfopen(szPath, _T("a+"))) != NULL)
	{		
		
			lSize = _stprintf(szMesg,_T("%s\r\n"), str.GetBuffer());

			fwrite(szMesg, lSize, 1, fp);

		
			fclose(fp);
	}	
}

void CAcqVoltageSamples_IntClkDlg::SaveFileLogData(const CCommPacket & pktCommm, BOOL bSend)
{
	//if (! ::IsWindow(m_listLog.m_hWnd))
	//	return;


	CString	str;
	char	szCmdName[128], szResName[128];


	CCommPacket::GetCommandName((PacketCommand) pktCommm.dwCmd, szCmdName, 128);
	CCommPacket::GetResponseCodeName((ResponseCode) pktCommm.dwResponseCode, szResName, 128);
		SYSTEMTIME	st;
	GetLocalTime(&st);//PATH_BASE_DIR

//	::CreateDirectory(g_theApp.m_strSavePath, NULL);
	CString m_strPath;
	m_strPath = PATH_BASE_DIR;
	m_strPath += "\\Log";

	TCHAR	szMesg[4096], szPath[256];
	int lSize = 0;


	_stprintf(szPath, _T("%s\\%s_%04d%02d%02d_%02d%02d%02d.logdbg"),
		(LPCTSTR)m_strPath,
		szCmdName,
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	FILE*	fp;
	if ((fp = _tfopen(szPath, _T("wb"))) != NULL)
	{		
		if(bSend)
			lSize = _stprintf(szMesg,_T("Sending Packet\r\n"));
		else
			lSize = _stprintf(szMesg,_T("Received Packet\r\n"));

		fwrite(szMesg, lSize, 1, fp);

		lSize = _stprintf(szMesg, _T(" Cmd : 0x%04x (%s)\r\n"), pktCommm.dwCmd, (LPCTSTR) CString(szCmdName));
		fwrite(szMesg, lSize, 1, fp);

		lSize = _stprintf(szMesg, _T(" ResponseCode : 0x%04x (%s)\r\n"), pktCommm.dwResponseCode, (LPCTSTR) CString(szResName));
		fwrite(szMesg, lSize, 1, fp);

		lSize = _stprintf(szMesg, _T(" DataSize : %d\r\n"), pktCommm.dwDataSize);
		fwrite(szMesg, lSize, 1, fp);

		if(pktCommm.dwDataSize < 4096)
		{
			for(int i = 0; i < pktCommm.dwDataSize; i += 16)
			{
				if(pktCommm.dwDataSize > i+16)
				{
					lSize = _stprintf(szMesg, _T("%06d: %02x %02x %02x %02x %02x %02x %02x %02x   "), 
						pktCommm.m_pDataBuf[i+0], pktCommm.m_pDataBuf[i+1], pktCommm.m_pDataBuf[i+2], pktCommm.m_pDataBuf[i+3],
						pktCommm.m_pDataBuf[i+4], pktCommm.m_pDataBuf[i+5], pktCommm.m_pDataBuf[i+6], pktCommm.m_pDataBuf[i+7],i);
					fwrite(szMesg, lSize, 1, fp);

					lSize = _stprintf(szMesg, _T("%02x %02x %02x %02x %02x %02x %02x %02x\r\n"), 
						pktCommm.m_pDataBuf[i+8], pktCommm.m_pDataBuf[i+9], pktCommm.m_pDataBuf[i+10], pktCommm.m_pDataBuf[i+11],
						pktCommm.m_pDataBuf[i+12], pktCommm.m_pDataBuf[i+13], pktCommm.m_pDataBuf[i+14], pktCommm.m_pDataBuf[i+15]);
					fwrite(szMesg, lSize, 1, fp);
				}
				else
				{
					lSize = _stprintf(szMesg, _T("%06d: "), i);
					fwrite(szMesg, lSize, 1, fp);

					for( ; i < pktCommm.dwDataSize; i++)
					{
						if(i%16 == 8)
						{
							lSize = _stprintf(szMesg, _T("  %02x "),pktCommm.m_pDataBuf[i]);
						}
						else
						{
							lSize = _stprintf(szMesg, _T("%02x "),pktCommm.m_pDataBuf[i]);
						}
						fwrite(szMesg, lSize, 1, fp);
					}

					lSize = _stprintf(szMesg, _T("\r\n"));
					fwrite(szMesg, lSize, 1, fp);
					
				}

			}
		}	
		fclose(fp);	
	}	
}

void CAcqVoltageSamples_IntClkDlg::AddLogHandShake(DWORD dwCode, BOOL bSend)
{
	if (! ::IsWindow(m_listLog.m_hWnd))
		return;

	CString	str;
	char	szResName[128];

	CCommPacket::GetResponseCodeName((ResponseCode) dwCode, szResName, 128);

	if (bSend)
		str.Format(_T("     Sending H/S : 0x%04x (%s)"), dwCode, (LPCTSTR) CString(szResName));
	else
		str.Format(_T("     Received H/S : 0x%04x (%s)"), dwCode, (LPCTSTR) CString(szResName));
	AddLogData(str);
}





//void CAcqVoltageSamples_IntClkDlg::OnBnClickedBtnWifimodulecheck()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CWTCPClient	tcpCli;
//	CCommPacket	pktRecv, pktSend;
//	DWORD		bWiFiModule = FALSE;
//	char		recvBuffer[512];
//
//	if (!ConnectToDetector(tcpCli))
//		return;
//
//	pktSend.SetHeaderData(LDCMD_GET_WIFI_MODULE, 0, 0);
//
//	//send
//	if (!SendPacket(tcpCli, pktSend))
//	{
//		DisconnectToDetector(tcpCli);
//		return;
//	}
//
//	//recv header
//	if (!RecvPacket(tcpCli, pktRecv))
//	{
//		DisconnectToDetector(tcpCli);
//		return;
//	}
//
//	DisconnectToDetector(tcpCli);
//
//	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
//		AfxMessageBox(_T("The detector returned not success."));
//	else {
//		memcpy(recvBuffer, pktRecv.m_pDataBuf, pktRecv.dwDataSize);
//	}
//
//	char str[512];
//	int idx = 0;
//	memset(str, 0, sizeof(str));
//	for (int i = 0; i < strlen(recvBuffer); i++)
//	{
//		if (recvBuffer[i] != 0x0a)
//			str[idx++] = recvBuffer[i];
//		else {
//			str[idx++] = 0x0d;
//			str[idx++] = 0x0a;
//		}
//	}
//	if (strlen(recvBuffer) < 50)
//		m_staticWiFiModule.SetWindowText(_T("Disconnected"));
//	else
//		m_staticWiFiModule.SetWindowText(_T("Connected"));
//	m_editWiFiInfo.SetWindowText(CString(str));
//
//}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedBtnGetdark()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	ClearImage();

	if (!ConnectToDetector(tcpCli))
		return;

	pktSend.SetHeaderData(LDCMD_GET_DARK_IMAGE, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return;
	}

	DisconnectToDetector(tcpCli);

//	CheckImage(packet.m_pDataBuf);//packet.m_pDataBuf + 8, packet.dwDataSize - 8

	SaveImage(pktRecv, TRUE);
}

int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int lResult = 0;
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	ClearImage();

	if (!ConnectToDetector(tcpCli))
		return 0;

	pktSend.SetHeaderData(LDCMD_GET_DARK_IMAGE, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);
	m_dwDataSize = pktRecv.dwDataSize;
	SaveImage(pktRecv, TRUE);
//	CheckImage((char *)pktRecv.m_pDataBuf, lMin, lMax);

	if (CheckDarkImage((char *)pktRecv.m_pDataBuf, lMin, lMax))	//if (CheckDarkImage((char *)&pktRecv.m_pDataBuf[8], lMin, lMax))//if (CheckDarkImage((char *)pktRecv.m_pDataBuf, lMin, lMax))	
	{
		//packet.m_pDataBuf + 8, packet.dwDataSize - 8
		lResult = 1;
	}
	else
	{
		lResult = 0;
	}
	DisplayCheckErrorImage((char *)&pktRecv.m_pDataBuf[8]); //DisplayCheckErrorImage(pktRecv);
	
	return lResult;

	
}
void CAcqVoltageSamples_IntClkDlg::GetMinMaxFromImgbuf(unsigned short *img_buf, int img_width, int img_height)
{
	int MaxCount[10], MinCount[10];
	double MaxAry[10], MinAry[10];
	for (int i = 0; i < 10; i++) {
		MaxCount[i] = 0;
		MinCount[i] = 0;
		MaxAry[i] = 0;
		MinAry[i] = 65535;
	}

	for (int y = 1; y < img_height; y++)
	{
		for (int i = 1; i < img_width-1; i++)
		{
			if ((g_TotalMin > img_buf[y*img_width + i])&&(30 < img_buf[y*img_width + i]))
				g_TotalMin = img_buf[y*img_width + i];

			if ((g_TotalMax < img_buf[y*img_width + i]) && (65000 > img_buf[y*img_width + i]))
			{
				g_TotalMax = (img_buf[y*img_width + i]- g_TotalMax)/100.+ g_TotalMax;

				//for (int m = 0; m < 10; m++) {
				//	
				//	if (MaxAry[m] < img_buf[y*img_width + i]  )
				//	{
				//		MaxCount[m]++;
				//		if (MaxCount[m] > 10)
				//		{
				//			MaxAry[m] = img_buf[y*img_width + i];
				//			g_TotalMax = MaxAry[m];
				//		}
				//	}
				//	else
				//	{
				//		//MaxAry[m] = img_buf[y*img_width + i];
				//		break;
				//	}
				//}
			}
		}
	}
}
//5000~15000
void CAcqVoltageSamples_IntClkDlg::UpdateMeanMedianFromImgbuf(unsigned short *img_buf, double* mean_buf, unsigned int* median_buf, int img_width, int img_height)
{
	int x_offset = 0, y_offset = 0;

	//*(mean_buf) = getMean(img_buf, x_offset, 0, img_height, m_RoicWidthEnd, img_width);
	//*(median_buf) = GetMedian(img_buf, x_offset, 0, img_height, m_RoicWidthEnd, img_width);

	//x_offset = m_RoicWidthEnd;
	for (int i_y = 0; i_y < gNoGate - 1; i_y++)
	{
		
	//	*(mean_buf + i) = getMean(img_buf, 0, y_offset, 512, img_width, img_width);
	//	*(median_buf + i) = GetMedian(img_buf, 0, y_offset, 512, img_width, img_width);
	
		x_offset = 0;

		for (int i = 0; i < gNoRoic - 1; i++) {

			*(mean_buf + i_y* gNoRoic + i) = getMean(img_buf, x_offset, y_offset, 512, 256, img_width);
			*(median_buf + i_y * gNoRoic + i) = GetMedian(img_buf, x_offset, y_offset, 512, 256, img_width);
			x_offset = 256 * (i + 1);
			TRACE("Mean:%d \n", i_y* gNoRoic + i);
		}

		*(mean_buf + i_y * gNoRoic+gNoRoic - 1) = getMean(img_buf, x_offset, y_offset, 512,  m_RoicWidthEnd, img_width);
		*(median_buf + i_y * gNoRoic + gNoRoic - 1) = GetMedian(img_buf, x_offset, y_offset, 512,  m_RoicWidthEnd, img_width);
		y_offset = 512 * (i_y + 1);
		TRACE("Mean:%d \n", i_y * gNoRoic + gNoRoic - 1);
	}

	//*(mean_buf + gNoGate - 1) = getMean(img_buf, 0, y_offset, m_RoicHeightEnd, img_width, img_width);
	//*(median_buf + gNoGate - 1) = GetMedian(img_buf, 0, y_offset, m_RoicHeightEnd, img_width, img_width);

	for (int i = 0; i < gNoRoic ; i++) {

		*(mean_buf + (gNoGate - 1) * gNoRoic + i) = getMean(img_buf, x_offset, y_offset, m_GateHeightEnd, 256, img_width);
		*(median_buf + (gNoGate - 1) * gNoRoic + i) = GetMedian(img_buf, x_offset, y_offset, m_GateHeightEnd, 256, img_width);
		x_offset = 256 * (i + 1);
		TRACE("Mean:%d \n", (gNoGate - 1) * gNoRoic + i);
	}

	/**(mean_buf + (gNoGate) * gNoRoic + gNoRoic - 1) = getMean(img_buf, x_offset, y_offset, m_GateHeightEnd, m_RoicWidthEnd, img_width);
	*(median_buf + (gNoGate ) * gNoRoic + gNoRoic - 1) = GetMedian(img_buf, x_offset, y_offset, m_GateHeightEnd, m_RoicWidthEnd, img_width);
	TRACE("Mean:%d \n", (gNoGate)* gNoRoic + gNoRoic - 1);*/
}


void CAcqVoltageSamples_IntClkDlg::UpdateMeanMedianFromImgbufRoic(unsigned short *img_buf, double* mean_buf, unsigned int* median_buf, int img_width, int img_height, int no_roic)
{
	int x_offset = 0, y_offset = 0;

	//*(mean_buf) = getMean(img_buf, x_offset, 0, img_height, m_RoicWidthEnd, img_width);
	//*(median_buf) = GetMedian(img_buf, x_offset, 0, img_height, m_RoicWidthEnd, img_width);

	//x_offset = m_RoicWidthEnd;
	for (int i = 0; i < no_roic - 1; i++) {

		*(mean_buf + i) = getMean(img_buf, x_offset, 0, img_height, 256, img_width);
		*(median_buf + i) = GetMedian(img_buf, x_offset, 0, img_height, 256, img_width);
		x_offset = 256 * (i+1);
	}

	*(mean_buf + no_roic - 1) = getMean(img_buf, x_offset, 0, img_height, m_RoicWidthEnd, img_width);
	*(median_buf + no_roic - 1) = GetMedian(img_buf, x_offset, 0, img_height, m_RoicWidthEnd, img_width);

}
void CAcqVoltageSamples_IntClkDlg::UpdateMeanMedianFromImgbufGate(unsigned short *img_buf, 
	double* mean_buf, unsigned int* median_buf, int img_width, int img_height, int no_gate)
{
	int x_offset = 0, y_offset = 0;

	//*(mean_buf) = getMean(img_buf, 0, y_offset, m_RoicHeightEnd, img_width, img_width  );
	//*(median_buf) = GetMedian(img_buf, 0, y_offset, m_RoicHeightEnd, img_width, img_width);
	
	//y_offset = m_RoicHeightEnd;
	for (int i = 0; i < no_gate - 1; i++) {

		*(mean_buf + i) = getMean(img_buf, 0, y_offset, 512, img_width, img_width);
		*(median_buf + i) = GetMedian(img_buf, 0, y_offset, 512, img_width, img_width);
		y_offset = 512 * (i+1);
	}

	*(mean_buf + no_gate - 1) = getMean(img_buf, 0, y_offset, m_GateHeightEnd, img_width, img_width);
	*(median_buf + no_gate - 1) = GetMedian(img_buf, 0, y_offset, m_GateHeightEnd, img_width, img_width);

}

double CAcqVoltageSamples_IntClkDlg::getMean(unsigned short *img_buf, int start_x, int start_y, int height, int width, int imageWidth) {
	double total = 0;
	double ImgVal = 0;
	double Imean = 0;
	int nNumber = 0;

	for (int y = start_y; y < height + start_y; y++)
	{
		if (y % 100 == 0)
		{
			Imean = 0;
		}
		for (int x = start_x; x < width + start_x; x++)
		{
			 ImgVal = img_buf[y*imageWidth + x];
			 total += ImgVal;
			 nNumber++;
		}
	}
	Imean = total / (height*width);
	return Imean;// total / (height*width);
}

int CAcqVoltageSamples_IntClkDlg::GetMedian(unsigned short *img_buf, int start_x, int start_y, int height, int width, int imageWidth) {
	int median = 0;
	//int index = 0;
	//int size = height * width;// glory

	//WORD *temp = new WORD[size + 1];
	//for (int y = start_y; y < height + start_y; y++)
	//	for (int x = start_x; x < width + start_x; x++)
	//		temp[index++] = img_buf[y*imageWidth + x];
	////temp[index++] = image[y*rCol + x];

 // //memcpy(temp, image, sizeof(WORD)*size); //2019_0523_yspark_1 : remove line (it is error code)
 // //qsort(temp, size, sizeof(WORD), compare_ascending_order);
	//quicksort(temp, 0, size - 1); //2018_0331_yspark_3 : change qsort => quicksort
	//median = temp[size / 2];
	//delete[] temp;

	return median;
}


int CAcqVoltageSamples_IntClkDlg::CheckDarkImage(char *buffer, unsigned int lMin, unsigned int lMax)//int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
{

#if 0
	//#define WIDTH 3328
	//#define HEIGHT 2816
	//	BITMAPINFOHEADER bmiHeader;
		//char *buffer;
	//	FILE *fp;
	unsigned int lWidth, lHeight;
	unsigned int color;
	int i;
	unsigned int r, g, b, gray;
	int len;

	WORD *ptr;

	//HWND hwnd;
	//HDC hdc;
	//RECT rc;


	//buffer = (char *)malloc(WIDTH*HEIGHT * 2);

	//fp = _tfopen(filename, _T("rb"));
	//len = fread(buffer, 1, WIDTH*HEIGHT * 2, fp);
	//fclose(fp);
	if (buffer == NULL)
	{
		MessageBox("Get Dark Image Fail!");
		return 0;
	}
	unsigned long sum = 0;
	int lcnt = 0;
	memcpy(&lWidth, buffer, 4);
	memcpy(&lHeight, &buffer[4], 4);
	ptr = (WORD *)(buffer + 8);
	// change color to grayscale
	for (i = 0; i < lWidth * lHeight; i++)
	{
		color = *ptr;
		//color = (int)(*(buffer + i*2));

		b = color & 0x001f;
		color >>= 5;
		g = color & 0x001f;
		color >>= 5;
		r = color & 0x003f;

		gray = (r + g + b) / 3 + 4;
		//gray = ((float)r*0.3 + (float)g*0.59 + (float)b*0.11);
		//gray = ((float)r*0.299 + (float)g*0.587 + (float)b*0.114) + 4;

		color = gray;
		color <<= 5;
		color |= gray;
		color <<= 5;
		color |= gray;
		//*(buffer + i*2) = color;
		*ptr++ = color;
		lcnt++;

		sum += gray;

	}
#endif
	if (buffer == NULL)
	{
		MessageBox("Get Dark Image Fail!");
		return 0;
	}
	char TempBuf[4096];
	short Tempimg_buf[30000];

	unsigned int lWidth, lHeight;
	//memcpy(&lHeight, buffer, 4);
	//memcpy(&lWidth, &buffer[4], 4);
	//m_dwDataSize
	
	if ((m_nPanelWidth *m_nPanelHeight *2+8) != m_dwDataSize)//if ((m_nPanelWidth != lWidth) || (m_nPanelHeight != lHeight))
	{
	MessageBox("Pannel Size Error! \r\n Ger SWver First!!");
	return 0;
	}

	if ((m_nPanelWidth < 4096) && (m_nPanelHeight < 4096))
	{
		m_WidthRcv = m_nPanelWidth;
		m_HeightRcv = m_nPanelHeight;
		lWidth = m_nPanelWidth; ;
		lHeight = m_nPanelHeight;
	}
	else
	{
		MessageBox("Get Dark Image Size Fail!");
		return 0;
	}
	double  mean_buf[256];
	double   *mean_Roic_buf;
	double   *mean_gate_buf;
	unsigned int    *median_buf;
	unsigned int    *median_Roic_buf;
	unsigned int    *median_Gate_buf;
	unsigned short *img_buf;
	img_buf = new unsigned short[lWidth * lHeight];
	gNoRoic = lWidth / 256;
	m_RoicWidthEnd = 256;
	if (lWidth % 256 != 0)
	{
		unsigned int lRoicWidthRsv = lWidth - gNoRoic * 256;
		if (lRoicWidthRsv > 0)
		{
			m_RoicWidthEnd = lRoicWidthRsv;// (lRoicWidthRsv + 256) / 2;
			gNoRoic += 1;
		}
		//	 m_RoicWidthEnd

	}

	gNoGate = lHeight / 512;
	m_GateHeightEnd = 512;
	if (lHeight % 512 != 0)
	{
		unsigned int lGateHeightRsv = lHeight - gNoGate * 512;
		if (lGateHeightRsv > 0)
		{
			m_GateHeightEnd = lGateHeightRsv;// (lRoicHeightRsv + 512) / 2;
			gNoGate += 1;
		}
	}

	//mean_buf = new double[256];//gNoRoic*gNoGate];
	mean_Roic_buf = new double[gNoRoic];
	mean_gate_buf = new double[gNoGate];
	median_buf = new unsigned int[gNoRoic*gNoGate];
	median_Roic_buf = new unsigned int[gNoRoic];
	median_Gate_buf = new unsigned int[gNoGate];

	//error = ReadImgFromFile_Unsigned(img_buf, cImagePath, img_width, img_height);
	memcpy(img_buf, &buffer[8], lWidth * lHeight * 2);// sizeof(img_buf));

	memcpy(TempBuf, buffer, 4096);// sizeof(img_buf));
	

	g_TotalMin = 0xFFFF;
	g_TotalMax = 0;

	UpdateMeanMedianFromImgbuf((unsigned short *)img_buf, mean_buf, median_buf, lWidth, lHeight);
	UpdateMeanMedianFromImgbufRoic((unsigned short *)img_buf, mean_Roic_buf, median_Roic_buf, lWidth, lHeight, gNoRoic);
	UpdateMeanMedianFromImgbufGate((unsigned short *)img_buf, mean_gate_buf, median_Gate_buf, lWidth, lHeight, gNoGate);

	int lNg[16];
	m_TotalNG = 0;

	for (int i = 0; i < gNoRoic; i++)
	{
		m_RoicAvgNG[i] = 1;
		for (int g_i = 0; g_i < gNoGate; g_i++)
		{

			if ((mean_buf[g_i*gNoRoic + i] > lMin) && (mean_buf[g_i*gNoRoic + i] < lMax))//if ((mean_buf[g_i*gNoRoic + i] > g_RefMin_buf[i]) && (mean_buf[g_i*gNoRoic + i] < g_RefMax_buf[i]))
			{
				m_RoicAvgNG[i] = 0;
				continue;
			}
			else
			{
				m_TotalNG++;

			}
		}
	}

	for (int g_i = 0; g_i < gNoGate; g_i++)
	{
		m_GateAvgNG[g_i] = 1;
		for (int i = 0; i < gNoRoic; i++)
		{
			if ((mean_buf[g_i*gNoRoic + i] > lMin) && (mean_buf[g_i*gNoRoic + i] < lMax))//if ((mean_buf[g_i*gNoRoic + i] > g_RefMin_buf[i]) && (mean_buf[g_i*gNoRoic + i] < g_RefMax_buf[i]))
			{
				m_GateAvgNG[g_i] = 0;
				continue;
			}
			else
			{
				m_TotalNG++;
			}
		}
	}
	for (int i = 0; i < gNoRoic; i++)
	{
		g_RoicMean[i] = mean_Roic_buf[i];
	}

	for (int i = 0; i < gNoGate; i++)
	{
		g_GateMean[i] = mean_gate_buf[i];
	
	}


	//for (int i = 0; i < gNoRoic; i++)
	//{
	//	g_RoicMean[i] = mean_Roic_buf[i];
	//	m_RoicAvgNG[i] = 0;
	////	if ((g_RoicMean[i] > g_RefMin_buf[i]) && (g_RoicMean[i] < g_RefMax_buf[i]))
	//	if ((g_RoicMean[i] > lMin) && (g_RoicMean[i] < lMax))
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		m_TotalNG++;
	//		m_RoicAvgNG[i] = 1;
	//	}
	//}

	//for (int i = 0; i < gNoGate; i++)
	//{
	//	g_GateMean[i] = mean_gate_buf[i];
	//	m_GateAvgNG[i] = 0;
	//	//	if ((g_RoicMean[i] > g_RefMin_buf[i]) && (g_RoicMean[i] < g_RefMax_buf[i]))
	//	if ((g_GateMean[i] > lMin) && (g_GateMean[i] < lMax))
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		m_TotalNG++;
	//		m_GateAvgNG[i] = 1;
	//	}
	//}

	if (m_TotalNG == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	//UINT lavg = (UINT)(sum / (lWidth * lHeight));
	//m_ImageAvg = lavg;
	//if((lavg > lMin)&&(lavg < lMax))
	//{
	//	return 1;
	//}
	//else
	//{
	//	return 0;
	//}
}

int CAcqVoltageSamples_IntClkDlg::CheckDarkImage(char *buffer, unsigned int lWidth,  unsigned int  lHeight, unsigned int lMin, unsigned int lMax)//int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
{


	if (buffer == NULL)
	{
		MessageBox("Get Dark Image Fail!");
		return 0;
	}
	char TempBuf[4096];
	short Tempimg_buf[30000];


	

	if ((lWidth < 4096) && (lHeight < 4096))
	{
		m_WidthRcv = lWidth;
		m_HeightRcv = lHeight;
	}
	else
	{
		MessageBox("Get Dark Image Size Fail!");
		return 0;
	}
	double   mean_buf[256];
	double   *mean_Roic_buf;
	double   *mean_gate_buf;
	unsigned int    *median_buf;
	unsigned int    *median_Roic_buf;
	unsigned int    *median_Gate_buf;
	unsigned short *img_buf;
	img_buf = new unsigned short[lWidth * lHeight];
	gNoRoic = lWidth / 256;
	m_RoicWidthEnd = 256;
	if (lWidth % 256 != 0)
	{
		unsigned int lRoicWidthRsv = lWidth - gNoRoic * 256;
		if (lRoicWidthRsv > 0)
		{
			m_RoicWidthEnd = lRoicWidthRsv;// (lRoicWidthRsv + 256) / 2;
			gNoRoic += 1;
		}
	}

	gNoGate = lHeight / 512;
	m_GateHeightEnd = 512;
	if (lHeight % 512 != 0)
	{
		unsigned int lGateHeightRsv = lHeight - gNoGate * 512;
		if (lGateHeightRsv > 0)
		{
			m_GateHeightEnd = lGateHeightRsv;// (lRoicHeightRsv + 512) / 2;
			gNoGate += 1;
		}
	}

	//mean_buf = new double[gNoRoic*gNoGate];
	mean_Roic_buf = new double[gNoRoic];
	mean_gate_buf = new double[gNoGate];
	median_buf = new unsigned int[gNoRoic*gNoGate];
	median_Roic_buf = new unsigned int[gNoRoic];
	median_Gate_buf = new unsigned int[gNoGate];

	//error = ReadImgFromFile_Unsigned(img_buf, cImagePath, img_width, img_height);
	memcpy(img_buf, buffer, lWidth * lHeight * 2);// sizeof(img_buf));

	//memcpy(TempBuf, buffer, 4096);// sizeof(img_buf));


	g_TotalMin = 0xFFFF;
	g_TotalMax = 0;

	UpdateMeanMedianFromImgbuf((unsigned short *)img_buf, mean_buf, median_buf, lWidth, lHeight);
	UpdateMeanMedianFromImgbufRoic((unsigned short *)img_buf, mean_Roic_buf, median_Roic_buf, lWidth, lHeight, gNoRoic);
	UpdateMeanMedianFromImgbufGate((unsigned short *)img_buf, mean_gate_buf, median_Gate_buf, lWidth, lHeight, gNoGate);

	int lNg[16];
	m_TotalNG = 0;

	for (int i = 0; i < gNoRoic; i++)
	{
		m_RoicAvgNG[i] = 1;
		for (int g_i = 0; g_i < gNoGate; g_i++)
		{

			if ((mean_buf[g_i*gNoRoic + i] > lMin) && (mean_buf[g_i*gNoRoic + i] < lMax))//if ((mean_buf[g_i*gNoRoic + i] > g_RefMin_buf[i]) && (mean_buf[g_i*gNoRoic + i] < g_RefMax_buf[i]))
			{
				m_RoicAvgNG[i] = 0;
				continue;
			}
			else
			{
				m_TotalNG++;

			}
		}
	}

	for (int g_i = 0; g_i < gNoGate; g_i++)
	{
		m_GateAvgNG[g_i] = 1;
		for (int i = 0; i < gNoRoic; i++)
		{
			if ((mean_buf[g_i*gNoRoic + i] > lMin) && (mean_buf[g_i*gNoRoic + i] < lMax))//if ((mean_buf[g_i*gNoRoic + i] > g_RefMin_buf[i]) && (mean_buf[g_i*gNoRoic + i] < g_RefMax_buf[i]))
			{
				m_GateAvgNG[g_i] = 0;
				continue;
			}
			else
			{
				m_TotalNG++;
			}
		}
	}
	for (int i = 0; i < gNoRoic; i++)
	{
		g_RoicMean[i] = mean_Roic_buf[i];
	}

	for (int i = 0; i < gNoGate; i++)
	{
		g_GateMean[i] = mean_gate_buf[i];

	}


	

	if (m_TotalNG == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	
}

int CAcqVoltageSamples_IntClkDlg::CheckImage(char *buffer, unsigned int lMin, unsigned int lMax)//int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
{

#if 0
	//#define WIDTH 3328
	//#define HEIGHT 2816
	//	BITMAPINFOHEADER bmiHeader;
		//char *buffer;
	//	FILE *fp;
	unsigned int lWidth, lHeight;
	unsigned int color;
	int i;
	unsigned int r, g, b, gray;
	int len;

	WORD *ptr;

	//HWND hwnd;
	//HDC hdc;
	//RECT rc;


	//buffer = (char *)malloc(WIDTH*HEIGHT * 2);

	//fp = _tfopen(filename, _T("rb"));
	//len = fread(buffer, 1, WIDTH*HEIGHT * 2, fp);
	//fclose(fp);
	if (buffer == NULL)
	{
		MessageBox("Get Dark Image Fail!");
		return 0;
	}
	unsigned long sum = 0;
	int lcnt = 0;
	memcpy(&lWidth, buffer, 4);
	memcpy(&lHeight, &buffer[4], 4);
	ptr = (WORD *)(buffer + 8);
	// change color to grayscale
	for (i = 0; i < lWidth * lHeight; i++)
	{
		color = *ptr;
		//color = (int)(*(buffer + i*2));

		b = color & 0x001f;
		color >>= 5;
		g = color & 0x001f;
		color >>= 5;
		r = color & 0x003f;

		gray = (r + g + b) / 3 + 4;
		//gray = ((float)r*0.3 + (float)g*0.59 + (float)b*0.11);
		//gray = ((float)r*0.299 + (float)g*0.587 + (float)b*0.114) + 4;

		color = gray;
		color <<= 5;
		color |= gray;
		color <<= 5;
		color |= gray;
		//*(buffer + i*2) = color;
		*ptr++ = color;
		lcnt++;

		sum += gray;

	}
#endif
	if (buffer == NULL)
	{
		MessageBox("Get Bright Image Fail!");
		return 0;
	}
	unsigned int lWidth, lHeight;
	//memcpy(&lWidth, buffer, 4);
	//memcpy(&lHeight, &buffer[4], 4);
	//memcpy(&lHeight, buffer, 4);
	//memcpy(&lWidth, &buffer[4], 4);

	if ((m_nPanelWidth *m_nPanelHeight * 2 + 8) != m_dwDataSize)//if ((m_nPanelWidth != lWidth) || (m_nPanelHeight != lHeight))
	{
		MessageBox("Pannel Size Error! \r\n Ger SW ver First!!");
		return 0;
	}
	if ((m_nPanelWidth < 4096) && (m_nPanelHeight < 4096))
	{
		m_WidthRcv = m_nPanelWidth;
		m_HeightRcv = m_nPanelHeight;
		lWidth = m_nPanelWidth; ;
		lHeight = m_nPanelHeight;
	}
	else
	{
		MessageBox("Get  Image Size Fail!");
		return 0;
	}
	/*if ((lWidth < 4096) && (lHeight < 4096))
	{
		m_WidthRcv = lWidth;
		m_HeightRcv = lHeight;
	}
	else
	{
		MessageBox("Get Bright Image Size Fail!");
		return 0;
	}*/

	double   *mean_buf;
	double   *mean_Roic_buf;
	double   *mean_gate_buf;
	unsigned int    *median_buf;
	unsigned int    *median_Roic_buf;
	unsigned int    *median_Gate_buf;

	//double   *mean_buf;
	//unsigned int    *median_buf;
	unsigned short *img_buf;
	img_buf = new unsigned short[lWidth * lHeight];
	gNoRoic = lWidth / 256;
	
	m_RoicWidthEnd = 256;
	if (lWidth % 256 != 0)
	{
		unsigned int lRoicWidthRsv = lWidth - gNoRoic * 256;
		if (lRoicWidthRsv > 0)
		{
			m_RoicWidthEnd = lRoicWidthRsv;// (lRoicWidthRsv + 256) / 2;
			gNoRoic += 1;
		}
		//	 m_RoicWidthEnd

	}

	gNoGate = lHeight / 512;
	m_GateHeightEnd = 512;
	if (lHeight % 512 != 0)
	{
		unsigned int lGateHeightRsv = lHeight - gNoGate * 512;
		if (lGateHeightRsv > 0)
		{
			m_GateHeightEnd = lGateHeightRsv;// (lRoicHeightRsv + 512) / 2;
			gNoGate += 1;
		}
	}

	 
	mean_buf = new double[gNoRoic*gNoGate];
	mean_Roic_buf = new double[gNoRoic];
	mean_gate_buf = new double[gNoGate];
	median_buf = new unsigned int[gNoRoic*gNoGate];
	median_Roic_buf = new unsigned int[gNoRoic];
	median_Gate_buf = new unsigned int[gNoGate];

	//error = ReadImgFromFile_Unsigned(img_buf, cImagePath, img_width, img_height);
	memcpy(img_buf, &buffer[8], lWidth * lHeight * 2);// sizeof(img_buf));

//	memcpy(TempBuf, buffer, 4096);// sizeof(img_buf));

//	mean_buf = new double[gNoRoic];
//	median_buf = new unsigned int[gNoRoic];
////error = ReadImgFromFile_Unsigned(img_buf, cImagePath, img_width, img_height);
//	memcpy(img_buf, &buffer[8], lWidth * lHeight * 2); //sizeof(img_buf));
//	UpdateMeanMedianFromImgbuf((unsigned short *)img_buf, mean_buf, median_buf, lWidth, lHeight, gNoRoic);
	//char TempBuf[4096];
	//short Tempimg_buf[2048];
	//   
	//memcpy(TempBuf, buffer, 4096);// sizeof(img_buf));
	//memcpy(Tempimg_buf, buffer, 4096);// sizeof(img_buf));
	g_TotalMin = 0xFFFF;
	g_TotalMax = 0;

	UpdateMeanMedianFromImgbuf((unsigned short *)img_buf, mean_buf, median_buf, lWidth, lHeight);
	UpdateMeanMedianFromImgbufRoic((unsigned short *)img_buf, mean_Roic_buf, median_Roic_buf, lWidth, lHeight, gNoRoic);
	UpdateMeanMedianFromImgbufGate((unsigned short *)img_buf, mean_gate_buf, median_Gate_buf, lWidth, lHeight, gNoGate);
	int lNg[16];
	m_TotalNG = 0;
	
	for (int i = 0; i < gNoRoic; i++)
	{	
		m_RoicAvgNG[i] = 1;
		for (int g_i = 0; g_i < gNoGate; g_i++)
		{
			
			if ((mean_buf[g_i*gNoRoic +i] > g_RefMin_buf[i]) && (mean_buf[g_i*gNoRoic + i] < g_RefMax_buf[i]))
			{
				m_RoicAvgNG[i] = 0;
				continue;
			}
			else
			{
				m_TotalNG++;				
				
			}
		}
	}

	for (int g_i = 0; g_i < gNoGate; g_i++)
	{	
		m_GateAvgNG[g_i] = 1;
		for (int i = 0; i < gNoRoic; i++)
		{
			if ((mean_buf[g_i*gNoRoic + i] > g_RefMin_buf[i]) && (mean_buf[g_i*gNoRoic + i] < g_RefMax_buf[i]))
			{
				m_GateAvgNG[g_i] = 0;
				continue;
			}
			else
			{
				m_TotalNG++;
			}
		}
	}

	for (int i = 0; i < gNoRoic; i++)
	{
		g_RoicMean[i] = mean_Roic_buf[i];
	}

	for (int i = 0; i < gNoGate; i++)
	{
		g_GateMean[i] = mean_gate_buf[i];

	}

	//for (int i = 0; i < gNoRoic; i++)
	//{
	//	g_RoicMean[i] = mean_Roic_buf[i];
	//	m_RoicAvgNG[i] = 0;
	//	if ((g_RoicMean[i] > g_RefMin_buf[i]) && (g_RoicMean[i] < g_RefMax_buf[i]))
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		m_TotalNG++;
	//		m_RoicAvgNG[i] = 1;
	//	}
	//}

	//for (int i = 0; i < gNoGate; i++)
	//{
	//	g_GateMean[i] = mean_gate_buf[i];
	//	m_GateAvgNG[i] = 0;
	//	//	if ((g_RoicMean[i] > g_RefMin_buf[i]) && (g_RoicMean[i] < g_RefMax_buf[i]))
	//	if ((g_GateMean[i] > lMin) && (g_GateMean[i] < lMax))
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		m_TotalNG++;
	//		m_GateAvgNG[i] = 1;
	//	}
	//}


	if (m_TotalNG == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	
}



void CAcqVoltageSamples_IntClkDlg::SaveImage(const CCommPacket & packet, BOOL bDark)
{
	SYSTEMTIME	st;
	GetLocalTime(&st);


	TCHAR	szMesg[256], szPath[256];

	_stprintf(szPath, _T("%s\\Image\\%s_%04d%02d%02d_%02d%02d%02d.raw"),
		(LPCTSTR)PATH_BASE_DIR,
		(bDark) ? _T("Dark") : _T("Bright"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	FILE*	fp;
	if ((fp = _tfopen(szPath, _T("wb"))) != NULL)
	{
		fwrite(packet.m_pDataBuf + 8, packet.dwDataSize - 8, 1, fp);
		/*
		// in case of 0 data, fill with 99AA BBCC
		if ((packet.dwDataSize - 8) == 0) {
		int i, j;
		int buffer[3328 * 2816 / 2];
		char *ptr = (char *)buffer;
		for (i = 0; i < 3328*2816/2; i++) {
		for (j = 0; j < 3328; j += 2) {
		buffer[i] = 0xCCBBAA99;
		}
		}
		fwrite(ptr, 3328*2816*2, 1, fp);
		}
		*/
		fclose(fp);

		LPTSTR	ptrName = _tcsrchr(szPath, '\\');

		_stprintf(szMesg, _T("Saveto %s"), ptrName + 1);

		AddLogData(szMesg);
	}
	else
		AddLogData(_T("Save to file fail..."));

	DisplayImage(szPath);
	//DeleteFile(szPath);
}

void CAcqVoltageSamples_IntClkDlg::SaveImage(char *pData, int  dwDataSize, BOOL bDark)
{
	SYSTEMTIME	st;
	GetLocalTime(&st);


	TCHAR	szMesg[256], szPath[256];

	_stprintf(szPath, _T("%s\\Image\\%s_%04d%02d%02d_%02d%02d%02d.raw"),
		(LPCTSTR)PATH_BASE_DIR,
		(bDark) ? _T("Dark") : _T("Bright"),
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	FILE*	fp;
	if ((fp = _tfopen(szPath, _T("wb"))) != NULL)
	{
		fwrite(pData, dwDataSize , 1, fp);
		
		fclose(fp);

		LPTSTR	ptrName = _tcsrchr(szPath, '\\');

		_stprintf(szMesg, _T("Saveto %s"), ptrName + 1);

		AddLogData(szMesg);
	}
	else
		AddLogData(_T("Save to file fail..."));

	DisplayImage(szPath);
	//DeleteFile(szPath);
}

void CAcqVoltageSamples_IntClkDlg::DisplayImage(TCHAR *filename)
{
	BITMAPINFOHEADER bmiHeader;
	char headbuffer[16];
	char *buffer;
	char *bufferDest;
	FILE *fp;
	unsigned int color;
	int i;
	unsigned int r, g, b, gray;
	int len;

	WORD *ptr;
	char *Destptr;

	

	HWND hwnd;
	HDC hdc;
	RECT rc;
	//unsigned int lWidth, lHeight;
	
	fp = _tfopen(filename, _T("rb"));

	fseek(fp, 0, SEEK_SET);
	len = fread(headbuffer, 1, 16, fp);
	ptr = (WORD *)headbuffer;

	m_WidthRcv = *(ptr + 1);
	m_HeightRcv = *(ptr + 2);

	buffer = (char *)malloc(m_WidthRcv*m_HeightRcv * 2);
	bufferDest = (char *)malloc(m_WidthRcv*m_HeightRcv * 3);

	
	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	len = fread(buffer, 1, fileLength, fp);
	fclose(fp);
	
	ptr = (WORD *)buffer;
//
//	
//	//18, 743, 296
//
//
////	buffer = (char *)malloc(WIDTH*HEIGHT * 2);
//
//
//	//fp = _tfopen(filename, _T("rb"));
//	//len = fread(buffer, 1, WIDTH*HEIGHT * 2, fp);
//	//fclose(fp);
//	//fwrite(packet.m_pDataBuf + 8, packet.dwDataSize - 8, 1, fp);
//
//	unsigned int lWidth, lHeight;
//	memcpy(&lHeight, packet.m_pDataBuf, 4);
//	memcpy(&lWidth, &packet.m_pDataBuf[4], 4);
//
//
//	//	memcpy(buffer, (char *)(packet.m_pDataBuf + 8), packet.dwDataSize - 8);// WIDTH*HEIGHT * 2);
//	memcpy(buffer, (char *)(packet.m_pDataBuf + 8), m_WidthRcv*m_HeightRcv * 2);// WIDTH*HEIGHT * 2);
//	ptr = (WORD *)buffer;
	Destptr = bufferDest;
	g_TotalMin = 0;// 0xFFFF;
	g_TotalMax = 16000;
	double lExpandRatio = 1;

//	GetMinMaxFromImgbuf(ptr, m_WidthRcv, m_HeightRcv);
	lExpandRatio = (65536. - 0x0000) / (g_TotalMax - g_TotalMin);
	unsigned int lWidthOffset;
	unsigned int n_Gate;
	unsigned int l_OldColor = 0;
	for (i = 0; i < m_HeightRcv; i++)
	{

		n_Gate = i / 512;
		for (int k = 0; k < m_WidthRcv; k++)
		{

			color = *(ptr + i * m_WidthRcv + k);
#if 1
			if (color < g_TotalMin)
				color = 0;
			else
				color -= g_TotalMin;
			

			gray = (unsigned int)(color *(lExpandRatio) / 256.);
			if (gray >= 255)
				gray = 255;
			if ((abs(int(l_OldColor) - int(gray)) > 30) && ((gray) < 30))
				l_OldColor = gray;

			l_OldColor = gray;
			
#else

			gray = (unsigned int)(color / 256.);
			if (gray >= 255)
				gray = 255;

			if (gray < 0)
				gray = 0;
#endif
			//if ((n_Gate % 2) && (k < 50))
			//{
			//	b = 0;

			//	g = 0;

			//	r = 255;
			//}
			//else if (i < 100)
			//{
			//	b = 0;

			//	g = 255;

			//	r = 0;
			//}
			//else
			{

				//gray = color >> 8;
				b = gray;
				//color >>= 5;
				g = gray;
				//color >>= 5;
				r = gray;
			}

			//*(Destptr + (i * m_WidthRcv + k) * 3) = (char)(b & 0xFF);
			//*(Destptr + (i * m_WidthRcv + k) * 3 + 1) = (char)(g & 0xFF);
			//*(Destptr + (i * m_WidthRcv + k) * 3 + 2) = (char)(r & 0xFF);
			*(Destptr + (i * m_WidthRcv + k) * 3) = (b & 0xFF);
			*(Destptr + (i * m_WidthRcv + k) * 3 + 1) = (g & 0xFF);
			*(Destptr + (i * m_WidthRcv + k) * 3 + 2) = (r & 0xFF);


		}
	}


	memset(&bmiHeader, 0, sizeof(bmiHeader));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = m_WidthRcv;
	bmiHeader.biHeight = -m_HeightRcv;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;//16;
	bmiHeader.biCompression = BI_RGB;
	
	hwnd = m_pic.m_hWnd;
	::GetClientRect(hwnd, &rc);
	hdc = ::GetDC(hwnd);
	//	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, WIDTH, HEIGHT, buffer1, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, m_WidthRcv, m_HeightRcv, bufferDest, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	//StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, buffer, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(hwnd, hdc);

	free(buffer);
	free(bufferDest);
#if 0
	// change color to grayscale
	for (i = 0; i < lWidth * lHeight; i++) {
		color = *ptr;
		//color = (int)(*(buffer + i*2));

		b = color & 0x001f;
		color >>= 5;
		g = color & 0x001f;
		color >>= 5;
		r = color & 0x003f;

		gray = (r + g + b) / 3 + 4;
		//gray = ((float)r*0.3 + (float)g*0.59 + (float)b*0.11);
		//gray = ((float)r*0.299 + (float)g*0.587 + (float)b*0.114) + 4;

		color = gray;
		color <<= 5;
		color |= gray;
		color <<= 5;
		color |= gray;
		//*(buffer + i*2) = color;
		*ptr++ = color;
	}



	memset(&bmiHeader, 0, sizeof(bmiHeader));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = WIDTH;
	bmiHeader.biHeight = -HEIGHT;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 16;
	bmiHeader.biCompression = BI_RGB;
	//bmiHeader.biSizeImage = bmiHeader.biWidth * bmiHeader.biHeight * 2;

	//hwnd = m_staticView.m_hWnd;

//	hwnd = m_PicturView.m_hWnd;
	hwnd = m_pic.m_hWnd;
	::GetClientRect(hwnd, &rc);
	hdc = ::GetDC(hwnd);
	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, WIDTH, HEIGHT, buffer, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	//StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, buffer, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(hwnd, hdc);

	free(buffer);
#endif
}

//
//void CAcqVoltageSamples_IntClkDlg::DisplayImage(TCHAR *filename)
//{
//#define WIDTH 3328
//#define HEIGHT 2816
//	BITMAPINFOHEADER bmiHeader;
//	char *buffer;
//	FILE *fp;
//	unsigned int color;
//	int i;
//	unsigned int r, g, b, gray;
//	int len;
//
//	WORD *ptr;
//
//	HWND hwnd;
//	HDC hdc;
//	RECT rc;
//
//
//	buffer = (char *)malloc(WIDTH*HEIGHT * 2);
//
//	fp = _tfopen(filename, _T("rb"));
//	len = fread(buffer, 1, WIDTH*HEIGHT * 2, fp);
//	fclose(fp);
//
//	ptr = (WORD *)buffer;
//	// change color to grayscale
//	for (i = 0; i < WIDTH * HEIGHT; i++) {
//		color = *ptr;
//		//color = (int)(*(buffer + i*2));
//
//		b = color & 0x001f;
//		color >>= 5;
//		g = color & 0x001f;
//		color >>= 5;
//		r = color & 0x003f;
//
//		gray = (r + g + b) / 3 + 4;
//		//gray = ((float)r*0.3 + (float)g*0.59 + (float)b*0.11);
//		//gray = ((float)r*0.299 + (float)g*0.587 + (float)b*0.114) + 4;
//
//		color = gray;
//		color <<= 5;
//		color |= gray;
//		color <<= 5;
//		color |= gray;
//		//*(buffer + i*2) = color;
//		*ptr++ = color;
//	}
//
//
//
//	memset(&bmiHeader, 0, sizeof(bmiHeader));
//	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	bmiHeader.biWidth = WIDTH;
//	bmiHeader.biHeight = -HEIGHT;
//	bmiHeader.biPlanes = 1;
//	bmiHeader.biBitCount = 16;
//	bmiHeader.biCompression = BI_RGB;
//	//bmiHeader.biSizeImage = bmiHeader.biWidth * bmiHeader.biHeight * 2;
//
//	//hwnd = m_staticView.m_hWnd;
//
////	hwnd = m_PicturView.m_hWnd;
//	hwnd = m_pic.m_hWnd;
//	::GetClientRect(hwnd, &rc);
//	hdc = ::GetDC(hwnd);
//	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, WIDTH, HEIGHT, buffer, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
//	//StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, buffer, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
//	::ReleaseDC(hwnd, hdc);
//
//	free(buffer);
//}


void CAcqVoltageSamples_IntClkDlg::DisplayCheckErrorImage(const CCommPacket & packet)
{

	//m_WidthRcv = lWidth;
	//m_HeightRcv = lHeight;
	//gNoRoic
	if ((m_WidthRcv > 4096) || (m_HeightRcv > 4096))
	{
		AddLogData(_T("Display Error fail..."));
		return;
	}
#define WIDTH 3328
#define HEIGHT 2816

	int lRoicWidth;

	lRoicWidth = 256;
	if (m_WidthRcv > WIDTH)
	{
		lRoicWidth = WIDTH / gNoRoic;
		if (lRoicWidth > 256)
		{

		}
	}


	BITMAPINFOHEADER bmiHeader;
	char *buffer;
	char *bufferDest;
	FILE *fp;
	unsigned int color;
	int i;
	unsigned int r, g, b, gray;
	int len;

	WORD *ptr;
	char *Destptr;

	HWND hwnd;
	HDC hdc;
	RECT rc;
	//18, 743, 296


//	buffer = (char *)malloc(WIDTH*HEIGHT * 2);
	buffer = (char *)malloc(m_WidthRcv*m_HeightRcv * 2);
	bufferDest = (char *)malloc(m_WidthRcv*m_HeightRcv * 3);

	//fp = _tfopen(filename, _T("rb"));
	//len = fread(buffer, 1, WIDTH*HEIGHT * 2, fp);
	//fclose(fp);
	//fwrite(packet.m_pDataBuf + 8, packet.dwDataSize - 8, 1, fp);

	unsigned int lWidth, lHeight;
	memcpy(&lHeight, packet.m_pDataBuf, 4);
	memcpy(&lWidth, &packet.m_pDataBuf[4], 4);


	//	memcpy(buffer, (char *)(packet.m_pDataBuf + 8), packet.dwDataSize - 8);// WIDTH*HEIGHT * 2);
	memcpy(buffer, (char *)(packet.m_pDataBuf + 8), m_WidthRcv*m_HeightRcv * 2);// WIDTH*HEIGHT * 2);
	ptr = (WORD *)buffer;
	Destptr = bufferDest;
	g_TotalMin = 0xFFFF;
	g_TotalMax = 0;
	double lExpandRatio = 1;

	GetMinMaxFromImgbuf(ptr, m_WidthRcv, m_HeightRcv);
	lExpandRatio = (65536. - 0x0000) / (g_TotalMax - g_TotalMin);
	unsigned int lWidthOffset;
	unsigned int n_Gate;
	for (i = 0; i < m_HeightRcv; i++)
	{

		n_Gate = i / 512;
		for (int k = 0; k < gNoRoic; k++)
		{
			int RoicWidth = 256;

			//if (k == 0 )
			//{
			//	RoicWidth = m_RoicWidthEnd;
			//	lWidthOffset = 0;
			//}
			//else 

			if (k == gNoRoic - 1)
			{
				RoicWidth = m_RoicWidthEnd;
				lWidthOffset = (k) * 256;
			}
			else
			{
				RoicWidth = 256;
				lWidthOffset = (k) * 256;
			}


			for (int m = 0; m < RoicWidth; m++)
			{

				//gray = (i / 50 + 70) % 256;// (unsigned int)(color *(lExpandRatio) / 256.);
				//if (gray >= 255)
				//	gray = 255;
				if (m == 0)
				{
					b = 0;
					g = 0;
					r = 0;
					//if (gray % 2)
					//{
					//	b = 0;
					//	g = 0xFF;
					//	r = 0;
					//}
					//else
					//{
					//	b = 0;
					//	g = 0xFF;
					//	r = 0xFF;
					//}
				}
				else if ((m_GateAvgNG[n_Gate] == 1) && (k == 0) && (m <= 50))
				{
					//color = 0x003f;
					//color <<= 5;
					//color |= 0;
					//color <<= 5;
					//color |= 0;

					b = 0;
					g = 0;
					r = 0xFF;


				}
				else if ((m_RoicAvgNG[k] == 1) && (i <= 100))
				{
					//color = 0x003f;
					//color <<= 5;
					//color |= 0;
					//color <<= 5;
					//color |= 0;

					b = 0;
					g = 0;
					r = 0xFF;


				}
				else
				{
					color = *(ptr + i * m_WidthRcv + lWidthOffset + m);
#if 1
					if (color < g_TotalMin)
						color = 0;
					else
						color -= g_TotalMin;



					gray = (unsigned int)(color *(lExpandRatio) / 256.);
					if (gray >= 255)
						gray = 255;
#else

					gray = (unsigned int)(color / 256.);
					if (gray >= 255)
						gray = 255;

					if (gray < 0)
						gray = 0;
#endif
					//gray = color >> 8;
					b = gray;
					//color >>= 5;
					g = gray;
					//color >>= 5;
					r = gray;

					//gray = color ;

					//color = gray>>8;
					//color <<= 8;
					//color |= gray>>8;
					//color <<= 8;
					//color |= gray>>8;


					////b = color & 0x001f;
					////color >>= 5;
					////g = color & 0x001f;
					////color >>= 5;
					////r = color & 0x003f;

					////gray = (r + g + b) / 3 + 4;
					////1111 1111 0000 0000
					//gray = color;
					//color = gray >> 10;
					//color <<= 5;
					//color |= gray >> 11;
					//color <<= 5;
					//color |= gray >> 11;
				}
				*(Destptr + (i * m_WidthRcv + lWidthOffset + m) * 3) = (char)(b & 0xFF);
				*(Destptr + (i * m_WidthRcv + lWidthOffset + m) * 3 + 1) = (char)(g & 0xFF);
				*(Destptr + (i * m_WidthRcv + lWidthOffset + m) * 3 + 2) = (char)(r & 0xFF);

			}
		}
	}
	//for (i = 0; i < HEIGHT  ; i++)
	//{
	//	for (int k = 0; k < gNoRoic; k++)
	//	{
	//		for (int m = 0; m < lRoicWidth; m++)
	//		{
	//			if (m_RoicAvgNG[i] == 1)
	//			{
	//				color = 0x003f;
	//				color <<= 5;
	//				color |= 0;
	//				color <<= 5;
	//				color |= 0;
	//			}
	//			else
	//			{
	//				color = *(ptr + i * m_WidthRcv + k * 256 + m);

	//				b = color & 0x001f;
	//				color >>= 5;
	//				g = color & 0x001f;
	//				color >>= 5;
	//				r = color & 0x003f;

	//				gray = (r + g + b) / 3 + 4;

	//				color = gray;
	//				color <<= 5;
	//				color |= gray;
	//				color <<= 5;
	//				color |= gray;
	//			}
	//			*Destptr++ = color;
	//		}
	//	}
	//}

	//// change color to grayscale
	//for (i = 0; i < WIDTH * HEIGHT; i++) {
	//	color = *ptr;
	//	//color = (int)(*(buffer + i*2));

	//	b = color & 0x001f;
	//	color >>= 5;
	//	g = color & 0x001f;
	//	color >>= 5;
	//	r = color & 0x003f;

	//	gray = (r + g + b) / 3 + 4;
	//	//gray = ((float)r*0.3 + (float)g*0.59 + (float)b*0.11);
	//	//gray = ((float)r*0.299 + (float)g*0.587 + (float)b*0.114) + 4;

	//	color = gray;
	//	color <<= 5;
	//	color |= gray;
	//	color <<= 5;
	//	color |= gray;
	//	//*(buffer + i*2) = color;
	//	*ptr++ = color;
	//}



	memset(&bmiHeader, 0, sizeof(bmiHeader));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = m_WidthRcv;
	bmiHeader.biHeight = -m_HeightRcv;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;//16;
	bmiHeader.biCompression = BI_RGB;
	//bmiHeader.biSizeImage = bmiHeader.biWidth * bmiHeader.biHeight * 2;

	//hwnd = m_staticView.m_hWnd;

//	hwnd = m_PicturView.m_hWnd;
	hwnd = m_pic.m_hWnd;
	::GetClientRect(hwnd, &rc);
	hdc = ::GetDC(hwnd);
	//	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, WIDTH, HEIGHT, buffer1, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, m_WidthRcv, m_HeightRcv, bufferDest, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	//StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, buffer, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(hwnd, hdc);

	free(buffer);
	free(bufferDest);
}

void CAcqVoltageSamples_IntClkDlg::DisplayCheckErrorImage(char *buffer)
{

	//m_WidthRcv = lWidth;
	//m_HeightRcv = lHeight;
	//gNoRoic
	


	BITMAPINFOHEADER bmiHeader;
	//char *buffer;
	char *bufferDest;
	FILE *fp;
	unsigned int color;
	int i;
	unsigned int r, g, b, gray;
	int len;

	WORD *ptr;
	char *Destptr;

	HWND hwnd;
	HDC hdc;
	RECT rc;
	//18, 743, 296


//	buffer = (char *)malloc(WIDTH*HEIGHT * 2);

	//fp = _tfopen(filename, _T("rb"));
	//len = fread(buffer, 1, WIDTH*HEIGHT * 2, fp);
	//fclose(fp);
	//fwrite(packet.m_pDataBuf + 8, packet.dwDataSize - 8, 1, fp);

	//unsigned int lWidth, lHeight;
	//memcpy(&lHeight, packet.m_pDataBuf, 4);
	//memcpy(&lWidth, &packet.m_pDataBuf[4], 4);


	//	memcpy(buffer, (char *)(packet.m_pDataBuf + 8), packet.dwDataSize - 8);// WIDTH*HEIGHT * 2);
	//memcpy(buffer, (char *)(packet.m_pDataBuf + 8), m_WidthRcv*m_HeightRcv * 2);// WIDTH*HEIGHT * 2);
	ptr = (WORD *)buffer;
//	m_WidthRcv = *(ptr + 1);
//	m_HeightRcv = *(ptr + 2);
	//buffer = (char *)malloc(m_WidthRcv*m_HeightRcv * 2);
	bufferDest = (char *)malloc(m_WidthRcv*m_HeightRcv * 3);

	Destptr = bufferDest;
	g_TotalMin = 0;
	g_TotalMax = 16000;
	double lExpandRatio = 1;

	//GetMinMaxFromImgbuf(ptr, m_WidthRcv, m_HeightRcv);
	lExpandRatio = (65536. - 0x0000) / (g_TotalMax - g_TotalMin);
	unsigned int lWidthOffset;
	unsigned int n_Gate;
	unsigned int n_Roic;
	for (i = 0; i < m_HeightRcv; i++)
	{
		//TRACE("Data # %d\n", i);
		n_Gate = i / 512;
		for (int k = 0; k < m_WidthRcv; k++)
		{
			//int RoicWidth = 256;
			n_Roic = k / 256;
			


			
			if (k == 0)
			{
				b = 0;
				g = 0;
				r = 0;
			}
			else if ((m_GateAvgNG[n_Gate] == 1) && (k  <= 50))
			{

				b = 0;
				g = 0;
				r = 0xFF;


			}
			else if ((m_RoicAvgNG[n_Roic] == 1) && (i <= 100))
			{

				b = 0;
				g = 0;
				r = 0xFF;


			}
			else
			{
				color = *(ptr + i * m_WidthRcv + k);
#if 1
				if (color < g_TotalMin)
					color = 0;
				else
					color -= g_TotalMin;



				gray = (unsigned int)(color *(lExpandRatio) / 256.);
				if (gray >= 255)
					gray = 255;
#else

				gray = (unsigned int)(color / 256.);
				if (gray >= 255)
					gray = 255;

				if (gray < 0)
					gray = 0;
#endif
				
				b = gray;				
				g = gray;				
				r = gray;
					
			}
			*(Destptr + (i * m_WidthRcv + k) * 3) = (char)(b & 0xFF);
			*(Destptr + (i * m_WidthRcv + k) * 3 + 1) = (char)(g & 0xFF);
			*(Destptr + (i * m_WidthRcv + k) * 3 + 2) = (char)(r & 0xFF);
			//if (i == 3051)
			//	TRACE("Data # (i%d * m_WidthRcv%d + k%d) * 3\n", i, m_WidthRcv, k);
			//if ((i == 3051)&&(k == 2499))
			//	TRACE("Data # (i%d * m_WidthRcv%d + k%d) * 3\n", i, m_WidthRcv, k);

		}
	}
	



	memset(&bmiHeader, 0, sizeof(bmiHeader));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = m_WidthRcv;
	bmiHeader.biHeight = -m_HeightRcv;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;//16;
	bmiHeader.biCompression = BI_RGB;
	//bmiHeader.biSizeImage = bmiHeader.biWidth * bmiHeader.biHeight * 2;

	//hwnd = m_staticView.m_hWnd;

//	hwnd = m_PicturView.m_hWnd;
	hwnd = m_pic.m_hWnd;
	::GetClientRect(hwnd, &rc);
	hdc = ::GetDC(hwnd);
	//	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, WIDTH, HEIGHT, buffer1, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	StretchDIBits(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, 0, m_WidthRcv, m_HeightRcv, bufferDest, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	//StretchDIBits(hdc, 0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, buffer, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(hwnd, hdc);

	//free(buffer);
	free(bufferDest);
}


void CAcqVoltageSamples_IntClkDlg::ClearImage()
{
	HWND hwnd;
	HDC hdc;
	RECT rc;


//	hwnd = m_staticView.m_hWnd;
	hwnd = m_pic.m_hWnd;
	::GetClientRect(hwnd, &rc);
	hdc = ::GetDC(hwnd);
	::Rectangle(hdc, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top));
	::ReleaseDC(hwnd, hdc);
}

void CAcqVoltageSamples_IntClkDlg::OnBnClickedBtnGetbright()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	 CCommPacket packet;
//	SaveImage( packet, 1);//::CreateDirectory(g_theApp.m_strSavePath, NULL);

	if (m_threadGetBright.IsAlive())
		return;

	m_threadGetBright.Create((WLIB_THREAD_STARTPROC)GetBrightThread, this);
}

DWORD WINAPI CAcqVoltageSamples_IntClkDlg::GetBrightThread(LPVOID pParam)
{
	CAcqVoltageSamples_IntClkDlg*	pThis = reinterpret_cast<CAcqVoltageSamples_IntClkDlg *>(pParam);

	pThis->DoGetBrightThread();

	return 0;
}

void CAcqVoltageSamples_IntClkDlg::DoGetBrightThread()
{
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	ClearImage();

	if (!ConnectToDetector(tcpCli))
	{
		m_SetComError =1;
		m_Deisplay_ErrorDetail = "if (!ConnectToDetector(tcpCli))";
		return;
	}

	pktSend.SetHeaderData(LDCMD_GET_BRIGHT_IMAGE, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		m_SetComError =1;
		m_Deisplay_ErrorDetail = "if (!SendPacket(tcpCli, pktSend))LDCMD_GET_BRIGHT_IMAGE";
		return;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		m_SetComError =1;
		m_Deisplay_ErrorDetail = "if (!RecvPacket(tcpCli, pktRecv))LDCMD_GET_BRIGHT_IMAGE";
		return;
	}

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
	{
		DisconnectToDetector(tcpCli);
		m_SetComError =1;
		m_Deisplay_ErrorDetail = "if (pktRecv.dwResponseCode != LDRET_SUCCESS)LDCMD_GET_BRIGHT_IMAGE";
		return;
	}

	if (m_detParam.dwExposureMode == EXP_MODE_AUTO)
	{
		if(HandShake_Auto(tcpCli) == FALSE)
		{
			m_Deisplay_ErrorDetail = "if(HandShake_Auto(tcpCli) == FALSE)";

			m_SetComError =1;
		}
	}
	else
	{
		//HandShake_Manual(tcpCli);
		if(HandShake_Manual(tcpCli) == FALSE)
		{
			m_Deisplay_ErrorDetail = "if(HandShake_Manual(tcpCli) == FALSE)";
			m_SetComError =1;
		}

	}

	DisconnectToDetector(tcpCli);

	m_SetComFinished = 1;
}

BOOL CAcqVoltageSamples_IntClkDlg::HandShake_Auto(CWTCPClient & tcpCli)
{
	DWORD	dwCode;
MessageInformDisplay(m_strButtonMessage1, 10000);
	Sleep(500);
	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_READY))
	{
		m_SetComError =1;
		return FALSE;
	}
	else
		SendACK(tcpCli);
	

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_WINDOW_OPEN))
	{
		m_SetComError =1;
		return FALSE;
	}
	else
		SendACK(tcpCli);
	MessageInformClose();

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_WINDOW_CLOSED))
		return FALSE;
	else
		SendACK(tcpCli);

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_IMAGE_READY))
	{
		m_SetComError =1;
		return FALSE;
	}
	else
		SendACK(tcpCli);

	MessageInformClose();

	return HandShake_PostProcess(tcpCli);
}

BOOL CAcqVoltageSamples_IntClkDlg::HandShake_Manual(CWTCPClient & tcpCli)
{
	DWORD	dwCode;
	//MessageInformDisplay(CString lMessage, int lTime);
//	GetBright 버튼 누르면 LDRET_STATUS_READY ack 신호를 받고
//
//지그에서 Ready 버튼을 누르면 LDRET_STATUS_PREPARE_SIGNAL ack 신호
//
//지그에서 Exposure 버튼을 누르면 LDRET_STATUS_WINDOW_OPEN ack신호를 받습니다.

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_READY))
		return FALSE;
	else
		SendACK(tcpCli);

//	MessageInformDisplay("Ready 버튼을 누르시오.", 10000);
	MessageInformDisplay(m_strButtonMessage1, 10000);
	Sleep(1000);
	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_PREPARE_SIGNAL))
		return FALSE;
	else
		SendACK(tcpCli);

	MessageInformClose();

	MessageInformDisplay(m_strButtonMessage2, 10000);
	Sleep(1000);
	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_WINDOW_OPEN))
		return FALSE;
	else
		SendACK(tcpCli);

	MessageInformClose();
	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_WINDOW_CLOSED))
		return FALSE;
	else
		SendACK(tcpCli);

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_IMAGE_READY))
		return FALSE;
	else
		SendACK(tcpCli);

	return HandShake_PostProcess(tcpCli);
}

BOOL CAcqVoltageSamples_IntClkDlg::HandShake_PostProcess(CWTCPClient & tcpCli)
{
	DWORD	dwCode;

	CCommPacket	pktRecv;
	pktRecv.SetHeaderData(LDCMD_GET_BRIGHT_IMAGE, 0, 0);

	//image size
	if (tcpCli.RecvExact(&pktRecv.dwDataSize, 4) != 0)
		return FALSE;

	pktRecv.AllocDataBuffer();

	//image data
	if (tcpCli.RecvExact(pktRecv.m_pDataBuf, pktRecv.dwDataSize) != 0)
		return FALSE;

	{
		CString	str;
		str.Format(_T("     Received Image : DataSize : (8 + %d bytes)"), pktRecv.dwDataSize - 8);
		AddLogData(str);
	}

	SendACK(tcpCli);

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_CLEARING))
		return FALSE;
	else
		SendACK(tcpCli);

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_POWER_SAVING))
		return FALSE;
	else
		SendACK(tcpCli);

	if (!RecvHandShake(tcpCli, dwCode) || (dwCode != LDRET_STATUS_READY))
		return FALSE;
	else
		SendACK(tcpCli);

	SaveImage(pktRecv, FALSE);
	m_dwDataSize = pktRecv.dwDataSize;
	CheckImage((char *)pktRecv.m_pDataBuf, 0, 255);
	DisplayCheckErrorImage(pktRecv);// BYTE *lImage);

	return TRUE;
}


void CAcqVoltageSamples_IntClkDlg::OnBnClickedBtnCancel2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	if (!ConnectToDetector(tcpCli))
		return;

	pktSend.SetHeaderData(LDCMD_CANCEL_JOB, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return;
	}

	DisconnectToDetector(tcpCli);
}

/*
void CDxdTestDlg::OnBnClickedBtnCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWTCPClient tcpCli;
	CCommPacket pktRecv, pktSend;
	m_btnGetBright.EnableWindow(TRUE);
	m_btnGetCalMapBright.EnableWindow(TRUE);
	if (!ConnectToDetector(tcpCli))
	{
		if (m_threadGetBright.IsAlive())
		{
			m_threadGetBright.CloseHandle();
			m_btnGetBright.EnableWindow(TRUE);
		}
		return;
	}
	pktSend.SetHeaderData(LDCMD_CANCEL_JOB, 0, 0);
	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return;
	}
	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return;
	}
	DisconnectToDetector(tcpCli);
}
*/

void CAcqVoltageSamples_IntClkDlg::OnBnClickedBtnGetAedSensor()
{
	GetAedSensor();
}


int CAcqVoltageSamples_IntClkDlg::GetAedSensor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

#if 0
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;
	CString str;

	char	avr_command[11];
	unsigned short adc[5];

	if (!ConnectToDetector(tcpCli))
		return 0;

	memset(avr_command, 0, sizeof(avr_command));

	avr_command[0] = 0x01;
	avr_command[1] = 0x02;
	avr_command[2] = 0x01;
	avr_command[3] = 0x11;

	avr_command[10] = avr_command[0];
	for (int i = 1; i < sizeof(avr_command)-1; i++)
		avr_command[10] ^= avr_command[i];

	pktSend.SetHeaderData(LDCMD_SET_AVR_COMMAND, 0, sizeof(avr_command));
	pktSend.AllocDataBuffer();
	memcpy(pktSend.m_pDataBuf, avr_command, sizeof(avr_command));

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
	{
		return 0;
		AfxMessageBox(_T("The detector returned not success."));

	}
	else {
		TCHAR string[80];

		memcpy(adc, pktRecv.m_pDataBuf + 4, sizeof(adc));
		//wsprintf(string, _T("%d"), adc[0]);
		str.Format(_T("%d"), adc[0]);
		m_cLb_AED_Sensor.put_Caption(str);
		//m_staticAVRADC1.SetWindowText(string);
		//wsprintf(string, _T("%d"), adc[1]);
		str.Format(_T("%d"), adc[1]);
		m_cLb_AED_Sensor2.put_Caption(str);
		//m_staticAVRADC2.SetWindowText(string);
		//wsprintf(string, _T("%d"), adc[2]);
		str.Format(_T("%d"), adc[2]);
		m_cLb_AED_Sensor3.put_Caption(str);
		//m_staticAVRADC3.SetWindowText(string);

		str.Format(_T("%d"), adc[3]);
		m_cLb_AED_Sensor4.put_Caption(str);


		str.Format(_T("%d"), adc[4]);
		m_cLb_AED_Sensor5.put_Caption(str);

		for(int i = 0; i < 5; i++)
		{	
			m_AED_Sensor[i] = adc[i];
	//UINT	m_AED_SensorMin;
	//UINT	m_AED_SensorMax;
		}
		return 1;

	}

#else
	///////////////////////////////////////////////////
	int lErrorFlag = 0;
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;
	CString str;

//	CDxdTestDlg* pParent = (CDxdTestDlg*)AfxGetMainWnd();

	char	avr_command[25];
	unsigned short adc[10];


	memset(avr_command, 0, sizeof(avr_command));

	avr_command[0] = 0x01;
	avr_command[1] = 0x02;
	avr_command[2] = 0x01;
	avr_command[3] = 0x11;

	avr_command[24] = avr_command[0];
	for (int i = 1; i < sizeof(avr_command)-1; i++)
		avr_command[24] ^= avr_command[i];

	pktSend.SetHeaderData(LDCMD_SET_AVR_COMMAND, 0, sizeof(avr_command));
	pktSend.AllocDataBuffer();
	memcpy(pktSend.m_pDataBuf, avr_command, sizeof(avr_command));
#if 1
	SaveFileLogData(pktSend, 1);

#endif

	if (!ConnectToDetector(tcpCli))
		return 0;

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
	{
		//AfxMessageBox(_T("The detector returned not success."));
		lErrorFlag = 1;
	}
	else {
		TCHAR string[80];

		memcpy(adc, pktRecv.m_pDataBuf + 4, sizeof(adc));


		//str.Format(_T("%d"), adc[0]);
		//m_cLb_AED_Sensor[0].put_Caption(str);		

		//str.Format(_T("%d"), adc[1]);
		//m_cLb_AED_Sensor[1].put_Caption(str);	

		//str.Format(_T("%d"), adc[2]);
		//m_cLb_AED_Sensor[2].put_Caption(str);		

		//str.Format(_T("%d"), adc[3]);
		//m_cLb_AED_Sensor[3].put_Caption(str);

		//str.Format(_T("%d"), adc[4]);
		//m_cLb_AED_Sensor[4].put_Caption(str);

		for(int i = 0; i < 7; i++)
		{	
			str.Format(_T("%d"), adc[i]);
			m_cLb_AED_Sensor[i].put_Caption(str);
			m_AED_Sensor[i] = adc[i];
		}
	
	
	}

	// get trigger sensitivity
	if (!ConnectToDetector(tcpCli))
		return 0;

	memset(avr_command, 0, sizeof(avr_command));

	avr_command[0] = 0x01;
	avr_command[1] = 0x02;
	avr_command[2] = 0x01;
	avr_command[3] = 0x12;

	avr_command[24] = avr_command[0];
	for (int i = 1; i < sizeof(avr_command)-1; i++)
		avr_command[24] ^= avr_command[i];

	pktSend.SetHeaderData(LDCMD_SET_AVR_COMMAND, 0, sizeof(avr_command));
	pktSend.AllocDataBuffer();
	memcpy(pktSend.m_pDataBuf, avr_command, sizeof(avr_command));

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);

	if(lErrorFlag == 1)
		return 0;
	else
		return 1;

	//if (pktRecv.dwResponseCode != LDRET_SUCCESS)
	//	AfxMessageBox(_T("The detector returned not success."));
	//else {
	//	TCHAR string[80];
	//	unsigned short trigger_sensitivity;

	//	memcpy(&trigger_sensitivity, pktRecv.m_pDataBuf + 4, sizeof(trigger_sensitivity));
	//	wsprintf(string, _T("%d"), trigger_sensitivity & 0x00ffff);
	//	m_staticTriggerSensitivity.SetWindowText(string);
	//}
#endif
}








void CAcqVoltageSamples_IntClkDlg::ClickLabelMacCheckMark()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CheckMacRead();
}

int CAcqVoltageSamples_IntClkDlg::CheckMacRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;
	CString str;
	char buffer[12];
	
	if (!ConnectToDetector(tcpCli))
		return 0;
	pktSend.SetHeaderData(LDCMD_GET_MAC_ADDRESS, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 0;
	}

	DisconnectToDetector(tcpCli);

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
		return 0;//AfxMessageBox(_T("The detector returned not success."));
	else
	{
		TCHAR string[80];
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, pktRecv.m_pDataBuf, pktRecv.dwDataSize);
	//	wsprintf(string, _T("%c%c:%c%c:%c%c:%c%c:%c%c:%c%c"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
	//	m_editMacAddress.SetWindowText(string);
		str.Format(_T("%c%c:%c%c:%c%c:%c%c:%c%c:%c%c"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
		
		m_cLb_MAC.put_Caption(str);

		m_ReadMacAddress.Format(_T("%c%c:%c%c:%c%c"), buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]); 

		return 1;
	}
}



void CAcqVoltageSamples_IntClkDlg::DisplayClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_listLog.SetRedraw(FALSE);
	m_listLog.ResetContent();
	m_listLog.SetRedraw(TRUE);

	/*m_staticFPGAVersion.SetWindowText(NULL);
	m_staticDSERVERVersion.SetWindowText(NULL);
	m_staticMICROCHIPVersion.SetWindowText(NULL);
	m_staticAVRVersion.SetWindowText(NULL);
	m_staticWiFiModule.SetWindowText(NULL);
	m_editWiFiInfo.SetWindowText(NULL);
	m_staticAVRADC1.SetWindowText(NULL);
	m_staticAVRADC2.SetWindowText(NULL);
	m_staticAVRADC3.SetWindowText(NULL);
	m_staticAVRADC4.SetWindowText(NULL);
	m_staticAVRADC5.SetWindowText(NULL);
	m_editMacAddress.SetWindowText(NULL);
*/

	ClearImage();
	for(int i = 0; i < 7; i++)
	{
		m_cLb_AED_Sensor[i].put_ForeColor(0x00FFFF);
						
	}

	//memset(SaveFileName, 0, sizeof(SaveFileName));
}


int CAcqVoltageSamples_IntClkDlg::SetManualExposureMode()
{

	SET_FPGA_REGISTER       rData;
	rData.addr = FPGA_REG_EXPOSURE;
	rData.data = 1;   // 0 – AED mode, 1 – Manual mode           

	m_detParam.dwExposureMode = rData.data; 

	CWTCPClient      tcpCli;
	CCommPacket    pktRecv, pktSend;
 

	if (!ConnectToDetector(tcpCli))
			return FALSE;

	pktSend.SetHeaderData(LDCMD_SET_FPGAREGISTER, 0, sizeof(SET_FPGA_REGISTER));
	pktSend.AllocDataBuffer();
	memcpy(pktSend.m_pDataBuf, &rData, pktSend.dwDataSize);		    

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

 
	//recv header

	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	DisconnectToDetector(tcpCli);

	return TRUE;
}


int CAcqVoltageSamples_IntClkDlg::SetAutoExposureMode()
{

	SET_FPGA_REGISTER       rData;
	rData.addr = FPGA_REG_EXPOSURE;
	rData.data = 0;   // 0 – AED mode, 1 – Manual mode           

	m_detParam.dwExposureMode = rData.data; 

	CWTCPClient      tcpCli;
	CCommPacket    pktRecv, pktSend;
 

	if (!ConnectToDetector(tcpCli))
			return FALSE;

	pktSend.SetHeaderData(LDCMD_SET_FPGAREGISTER, 0, sizeof(SET_FPGA_REGISTER));
	pktSend.AllocDataBuffer();
	memcpy(pktSend.m_pDataBuf, &rData, pktSend.dwDataSize);		    

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

 
	//recv header

	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	DisconnectToDetector(tcpCli);

	return TRUE;
}

 int CAcqVoltageSamples_IntClkDlg::GetExposureMode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	// det param
	if (!ConnectToDetector(tcpCli))
		return 3;

	pktSend.SetHeaderData(LDCMD_GET_PARAMETER, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return 3;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return 3;
	}

	DisconnectToDetector(tcpCli);

	if (pktRecv.dwResponseCode != LDRET_SUCCESS)
	{
		AfxMessageBox(_T("The detector returned not success."));
		return 3;
	}
	else {
		//TCHAR str[80];
		CString strTemp;

		memcpy(&m_detParam, pktRecv.m_pDataBuf, pktRecv.dwDataSize);
		//strTemp.Format(_T("%d"), m_detParam.dwExposureMode);// wsprintf(str, _T("%d"), m_detParam.dwExposureMode);
		if(m_detParam.dwExposureMode == 0)
		{
			m_cLb_Exposure.put_Caption("AUTO");//
		}
		else
		{
			m_cLb_Exposure.put_Caption("MAN");//, m_comboExposure.SetCurSel(_ttoi(str));
		}

		m_nExposure = m_detParam.dwExposureMode;		
	
	
		return m_nExposure;
	}
}


 

int CAcqVoltageSamples_IntClkDlg::FactoryReset()
{

	CWTCPClient      tcpCli;
	CCommPacket    pktRecv, pktSend;
 

	if (!ConnectToDetector(tcpCli))
		return FALSE;
		   
	pktSend.SetHeaderData(LDCMD_FACTORY_RESET, 0, 0); 

	//send

	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	} 

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}		   
	DisconnectToDetector(tcpCli);

	return TRUE;

}

int CAcqVoltageSamples_IntClkDlg::Set_ModelName()
{
	if(!GetDetParam())
		return FALSE;	
	if (g_AVR_Type_HK_HQ == MICOM_TYPE_HQ)
	{
		if (!Set_ModelName_HQ())
			return FALSE;
	}
	else
	{
		if(!SetModelname_HK())
			return FALSE;
	}
	return TRUE;
}

//===================================================
// HQ 모델 (10HQ701G, 14HQ701G, 14HQ901G, 17HQ701G, 17HQ901G)
//===================================================
//#define DXD_MODEL_COUNT	7
//char *modelName[DXD_MODEL_COUNT] = { "10HQ701G", "14HQ701G", "14HQ901G", "17HQ701G", "17HQ901G", "14HQ721G", "17HQ901G_D" };

int  CAcqVoltageSamples_IntClkDlg::Set_ModelName_HQ()//void CAcqVoltageSamples_IntClkDlg::SetModelname_HK()
{
	// TODO: Add your control notification handler code here
	//CDxdTestDlg* pParent = (CDxdTestDlg*)AfxGetMainWnd();
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	if (!ConnectToDetector(tcpCli))
		return  FALSE;

	pktSend.SetHeaderData(LDCMD_GET_SYSTEM_INFO, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return  FALSE;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return  FALSE;
	}

	memcpy(&m_sysInfo, pktRecv.m_pDataBuf, pktRecv.dwDataSize);

	// get current model name
	//int index = m_comboModelName.GetCurSel();
	int lSeL = -1;
	for (int i = 0; i < DXD_MODEL_COUNT; i++)
	{
		
		if (gHQ_ModelNmae[i].Compare(g_SetModelName) == 0)
		{
			lSeL = i;
		}		
	}
	if (lSeL == -1)
	{
		MessageBox("HQ Set Model Name Not Found", "Set Model Name ERROR");

		return  FALSE;
	}

	memset(m_sysInfo.szModelName, 0, strlen(m_sysInfo.szModelName));
	memcpy(m_sysInfo.szModelName, g_SetModelName.GetBuffer(), strlen(g_SetModelName.GetBuffer()));

	// write model name
	pktSend.SetHeaderData(LDCMD_SET_SYSTEM_INFO, 0, sizeof(SYSTEM_INFO_DATA));
	pktSend.AllocDataBuffer();
	memcpy(pktSend.m_pDataBuf, &m_sysInfo, pktSend.dwDataSize);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	DisconnectToDetector(tcpCli);
	return TRUE;
}


//===================================================
// HK 모델 (17HK701G, 17HK700G, 14HK701G)
//===================================================

//#define DXD_HK_MODEL_NUM	3
//char *HK_ModelName[DXD_HK_MODEL_NUM] = { "17HK700G", "14HK701G", "17HK701G" };

int CAcqVoltageSamples_IntClkDlg::SetModelname_HK()
{
	// TODO: Add your control notification handler code here
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	if (!ConnectToDetector(tcpCli))
		return FALSE;

	pktSend.SetHeaderData(LDCMD_GET_SYSTEM_INFO, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	memcpy(&m_sysInfo, pktRecv.m_pDataBuf, pktRecv.dwDataSize);

	// get current model name
//	int index = m_ComboHKModelName.GetCurSel();
	int lSeL = -1;
	for (int i = 0; i < DXD_HK_MODEL_NUM; i++)
	{

		if (gHK_ModelName[i].Compare(g_SetModelName) == 0)
		{
			lSeL = i;
		}
	}
	if (lSeL == -1)
	{
		MessageBox("HK Set Model Name Not Found", "Set Model Name ERROR");
		return  FALSE;
	}

	memset(m_sysInfo.szModelName, 0, strlen(m_sysInfo.szModelName));
	//memcpy(m_sysInfo.szModelName, HK_ModelName[index], strlen(HK_ModelName[index]));
	memcpy(m_sysInfo.szModelName, g_SetModelName.GetBuffer(), strlen(g_SetModelName.GetBuffer()));

	// write model name
	pktSend.SetHeaderData(LDCMD_SET_SYSTEM_INFO, 0, sizeof(SYSTEM_INFO_DATA));
	pktSend.AllocDataBuffer();
	memcpy(pktSend.m_pDataBuf, &m_sysInfo, pktSend.dwDataSize);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	DisconnectToDetector(tcpCli);
	return TRUE;
}



int CAcqVoltageSamples_IntClkDlg::GetModelname(CString &ReadModelName)
{
	// TODO: Add your control notification handler code here
	CWTCPClient	tcpCli;
	CCommPacket	pktRecv, pktSend;

	if (!ConnectToDetector(tcpCli))
		return FALSE;

	pktSend.SetHeaderData(LDCMD_GET_SYSTEM_INFO, 0, 0);

	//send
	if (!SendPacket(tcpCli, pktSend))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	//recv header
	if (!RecvPacket(tcpCli, pktRecv))
	{
		DisconnectToDetector(tcpCli);
		return FALSE;
	}

	memcpy(&m_sysInfo, pktRecv.m_pDataBuf, pktRecv.dwDataSize);
	ReadModelName = m_sysInfo.szModelName;
	//memset(m_sysInfo.szModelName, 0, strlen(m_sysInfo.szModelName));
	

	DisconnectToDetector(tcpCli);
	return TRUE;
}


void CAcqVoltageSamples_IntClkDlg::ClickLabelBarcodeMark()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	
	CFileDialog dlg(TRUE, "Image", NULL,
		OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR | OFN_HIDEREADONLY,
		"Image Raw File(*.raw)|*.raw|", NULL);

	CString sTempNewFolder;
	CString sTemp;
	sTempNewFolder = PATH_BASE_DIR;
	sTempNewFolder += "\\Image\\";

	dlg.m_ofn.lpstrInitialDir = sTempNewFolder;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	sTempNewFolder = dlg.GetPathName();
	sTemp = sTempNewFolder;
	TCHAR	 szPath[512];
	_stprintf(szPath, "%s", sTemp.GetBuffer());

	//memcpy(szPath, sTemp.GetBuffer(), sTemp.GetLength);
	//
	//DisplayImage(szPath);
	BITMAPINFOHEADER bmiHeader;
	char headbuffer[16];
	char *buffer;
	char *bufferDest;
	FILE *fp;
	unsigned int color;
	int i;
	unsigned int r, g, b, gray;
	int len;

	WORD *ptr;
	char *Destptr;



	HWND hwnd;
	HDC hdc;
	RECT rc;
	//unsigned int lWidth, lHeight;

	fp = _tfopen(szPath, _T("rb"));

	fseek(fp, 0, SEEK_SET);
	len = fread(headbuffer, 1, 16, fp);
	ptr = (WORD *)headbuffer;

	m_WidthRcv = *(ptr + 1);
	m_HeightRcv = *(ptr + 2);

	buffer = (char *)malloc(m_WidthRcv*m_HeightRcv * 2);
	bufferDest = (char *)malloc(m_WidthRcv*m_HeightRcv * 3);


	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	len = fread(buffer, 1, fileLength, fp);
	fclose(fp);

	//if (CheckDarkImage(buffer, lMin, lMax));

	CheckDarkImage(buffer, m_WidthRcv,m_HeightRcv , 5000, 13000);//int CAcqVoltageSamples_IntClkDlg::GetdarkCheck(unsigned int lMin, unsigned int lMax)
	DisplayCheckErrorImage(buffer);

	// DisplayImage(szPath);
}
