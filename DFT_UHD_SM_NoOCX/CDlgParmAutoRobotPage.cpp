// CDlgParmAutoRobotPage.cpp: 구현 파일
//

#include "stdafx.h"
#include "DATsys.h"
#include "CDlgParmAutoRobotPage.h"
#include "afxdialogex.h"
#include "PLC_Ctrl.h"
#include "global.h"
#include "JigCtl.h"

extern CDATsysView*  g_pView;
// CDlgParmAutoRobotPage 대화 상자

IMPLEMENT_DYNAMIC(CDlgParmAutoRobotPage, CDialogEx)

CDlgParmAutoRobotPage::CDlgParmAutoRobotPage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARM_AUTO_ROBOT_PAGE, pParent)
	, m_strEditModelName(_T(""))
	, m_bRelay1(FALSE)
	, m_bGrabPower(FALSE)
	, m_bCheckHdmiUsbCyl(FALSE)
{

}

CDlgParmAutoRobotPage::~CDlgParmAutoRobotPage()
{
}

void CDlgParmAutoRobotPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODEL_NAME, m_strEditModelName);
	DDX_Control(pDX, IDC_CHECK_COMMAND_LOAD, m_cCheckLoad);
	DDX_Control(pDX, IDC_CHECK_COMMAND_UNLOAD, m_cCheckUnLoad);
	DDX_Control(pDX, IDC_CHECK_COMMAND_PASS, m_cCheckTestPass);
	DDX_Control(pDX, IDC_CHECK_COMMAND_NG, m_cCheckTestNg);
	DDX_Control(pDX, IDC_CHECK_FFC, m_cCheckReconnect);
	DDX_Control(pDX, IDC_CHECK_LOAD_FINISH, m_cCheckLoadFinish);
	DDX_Control(pDX, IDC_CHECK_UNLOAD_FINISH, m_cCheckUnLoadFinish);
	DDX_Control(pDX, IDC_CHECK_FFC_FINISH, m_cCheckReconnectFinish);
	DDX_Control(pDX, IDC_CHECK_RELAY1, m_ctrlRelay1Chk);
	DDX_Check(pDX, IDC_CHECK_RELAY1, m_bRelay1);
	DDX_Control(pDX, IDC_CHECK_GRAB_POWER, m_ctrlGrabPower);
	DDX_Check(pDX, IDC_CHECK_GRAB_POWER, m_bGrabPower);
	DDX_Control(pDX, IDC_CHECK_HDMI_USB_CYL, m_cCheckHdmiUsbCyl);
	DDX_Check(pDX, IDC_CHECK_HDMI_USB_CYL, m_bCheckHdmiUsbCyl);
	DDX_Control(pDX, IDC_CHECK_EARPHONE, m_cCheckEarphone);
	DDX_Control(pDX, IDC_CHECK_COMP, m_cCheckComp);
	DDX_Control(pDX, IDC_CHECK_STATUS_AUTO, m_cCheck_PLC_Auto_Mode);
	DDX_Control(pDX, IDC_CHECK_STATUS_MANUAL, m_cCheck_PLC_Manual_Mode);
	DDX_Control(pDX, IDC_CHECK_STATUS_START, m_cCheck_PLC_Start_Run);
	DDX_Control(pDX, IDC_CHECK_STATUS_PAUSE, m_cCheck_PLC_Pause);
	DDX_Control(pDX, IDC_CHECK_STATUS_STOP, m_cCheck_PLC_STOP);
	DDX_Control(pDX, IDC_CHECK_STATUS_WARNING, m_cCheck_PLC_Warn);
	DDX_Control(pDX, IDC_CHECK_STATUS_ALARM, m_cCheck_PLC_Alarm);
	DDX_Control(pDX, IDC_CHECK_PCB_IN, m_cCheck_PLC_Data_PCB_Inside);
	DDX_Control(pDX, IDC_CHECK_PCB_DUMMY, m_cCheck_PLC_Data_DummyCut);
	DDX_Control(pDX, IDC_CHECK_DATA_PCB_OK, m_cCheck_PLC_Data_OK);
	DDX_Control(pDX, IDC_CHECK_DATA_PCB_NG, m_cCheck_PLC_Data_NG);
	//DDX_Control(pDX, IDC_CHECK_COMMAND_LOAD_FAST, m_cCheckLoadFast);
	//DDX_Control(pDX, IDC_CHECK_COMMAND_UNLOAD_FAST, m_cCheckUnLoadFast);
	DDX_Control(pDX, IDC_CHECK_COMMAND_SKIP_FFC, m_cCheckSkipFFC);
	DDX_Control(pDX, IDC_CHECK_PCB_ONLOAD_SENSOR, m_cCheckPCBOnLoadSensor);
	DDX_Control(pDX, IDC_CHECK_ROBOT_RUNNING, m_cCheckRobotMoving);
	DDX_Control(pDX, IDC_CHECK_JIG_UP, m_cCheckJigUpStatus);
	DDX_Control(pDX, IDC_CHECK_JIG_DOWN, m_cCheckJigDownStatus);
	DDX_Control(pDX, IDC_CHECK_STATUS_SET_ALARM, m_cCheckSetAlarm);
}


BEGIN_MESSAGE_MAP(CDlgParmAutoRobotPage, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_COMMAND_MODEL_NAME, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandModelName)
	ON_BN_CLICKED(IDC_BTN_COMMAND_LOAD, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandLoad)
	ON_BN_CLICKED(IDC_BTN_COMMAND_PASS_UNLOAD, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandPassUnload)
	ON_BN_CLICKED(IDC_BTN_COMMAND_NG_LOAD, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandNgUnload)
//	ON_BN_CLICKED(IDC_BTN_COMMAND_NG_LOAD2, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandNgLoad2)
	ON_BN_CLICKED(IDC_BTN_COMMAND_FFC_RETRY, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandFfcRetry)
	ON_BN_CLICKED(IDC_BTN_COMMAND_READ, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandRead)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_MAIN_JIG_UP, &CDlgParmAutoRobotPage::OnBnClickedBtnMainJigUp)
	ON_BN_CLICKED(IDC_BTN_MAIN_JIG_DOWN, &CDlgParmAutoRobotPage::OnBnClickedBtnMainJigDown)
	ON_BN_CLICKED(IDC_CHECK_GRAB_POWER, &CDlgParmAutoRobotPage::OnBnClickedCheckGrabPower)
	ON_BN_CLICKED(IDC_BTN_RESET_IFPACK, &CDlgParmAutoRobotPage::OnBnClickedBtnResetIfpack)
	ON_BN_CLICKED(IDC_BTN_RESET_GENDER, &CDlgParmAutoRobotPage::OnBnClickedBtnResetGender)
	ON_BN_CLICKED(IDC_CHECK_HDMI_USB_CYL, &CDlgParmAutoRobotPage::OnBnClickedCheckHdmiUsbCyl)
	ON_BN_CLICKED(IDC_CHECK_EARPHONE, &CDlgParmAutoRobotPage::OnBnClickedCheckEarphone)
	ON_BN_CLICKED(IDC_CHECK_COMP, &CDlgParmAutoRobotPage::OnBnClickedCheckComp)
	ON_BN_CLICKED(IDC_CHECK_RELAY1, &CDlgParmAutoRobotPage::OnBnClickedCheckRelay1)
	//ON_BN_CLICKED(IDC_BTN_COMMAND_LOAD_FAST, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandLoadFast)
	//ON_BN_CLICKED(IDC_BTN_COMMAND_PASS_UNLOAD_FAST, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandPassUnloadFast)
	//ON_BN_CLICKED(IDC_BTN_COMMAND_NG_LOAD_FAST, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandNgLoadFast)
	ON_BN_CLICKED(IDC_BTN_COMMAND_SKIPFFC, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandSkipffc)
	ON_BN_CLICKED(IDC_BTN_COMMAND_CLEAR_COMMAND, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandClearCommand)
	ON_BN_CLICKED(IDC_BTN_COMMAND_ALARM, &CDlgParmAutoRobotPage::OnBnClickedBtnCommandAlarm)
END_MESSAGE_MAP()


// CDlgParmAutoRobotPage 메시지 처리기


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandModelName()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData();
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_MODEL_NAME;

	gPLC_Ctrl.WriteFullData(Address, m_strEditModelName.GetBuffer(), m_strEditModelName.GetLength());
}


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gPLC_Ctrl.CommandLoad();
	gPLC_Ctrl.CommandConnectFFC();

	//UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	//UINT Address = l_PLC_BaseAddress +  _PLC_ADDRESS_LOAD_COMMAND;
	//char Buff[256];

	//if (gPLC_Ctrl.m_cCheckLoad == 1)
	//{
	//	gPLC_Ctrl.m_cCheckLoad = 0;
	//}
	//else
	//{
	//	gPLC_Ctrl.m_cCheckLoad = 1;
	//}
	//Buff[0] = gPLC_Ctrl.m_cCheckLoad;


	//gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandPassUnload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gPLC_Ctrl.CommandPassUnload();
	gPLC_Ctrl.CommandConnectFFC();
	//UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	//UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_COMMAND;
	//char Buff[256];

	//if (gPLC_Ctrl.m_cCheckUnLoad == 1)
	//{
	//	gPLC_Ctrl.m_cCheckUnLoad = 0;
	//	Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;

	//	Buff[1] = 0; //OK
	//	Buff[2] = 0; //NG
	//}
	//else
	//{
	//	gPLC_Ctrl.m_cCheckUnLoad = 1;
	//	Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;
	//
	//	Buff[1] = 1; //OK
	//	Buff[2] = 0; //NG
	//}

	//gPLC_Ctrl.WriteCommand(Address, Buff, 3);
}


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandNgUnload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gPLC_Ctrl.CommandNgUnload();
	gPLC_Ctrl.CommandConnectFFC();
	//UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	//UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_COMMAND;
	//char Buff[256];

	//if (gPLC_Ctrl.m_cCheckUnLoad == 1)
	//{
	//	gPLC_Ctrl.m_cCheckUnLoad = 0;
	//	Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;
	//	Buff[1] = 0; //OK
	//	Buff[2] = 0; //NG
	//}
	//else
	//{
	//	gPLC_Ctrl.m_cCheckUnLoad = 1;
	//	
	//	Buff[0] = gPLC_Ctrl.m_cCheckUnLoad;
	//	Buff[1] = 0; //OK
	//	Buff[2] = 1; //NG
	//}

	//gPLC_Ctrl.WriteCommand(Address, Buff, 3);
}



void CDlgParmAutoRobotPage::OnBnClickedBtnCommandFfcRetry()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	return; //cys 200824
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_FFC_RETRY_COMMAND;
	char Buff[256];

	if (gPLC_Ctrl.m_cCheckReconnect == 1)
	{
		gPLC_Ctrl.m_cCheckReconnect = 0;
	}
	else
	{
		gPLC_Ctrl.m_cCheckReconnect = 1;
	}
	Buff[0] = gPLC_Ctrl.m_cCheckReconnect;
	//Buff[0] = 1;
	//	Buff[1] = 0; //OK
	//	Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int lSel = m_cComboDFT_SEL.GetCurSel();
	
	//UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;
	//gPLC_Ctrl.ReadCommand(l_PLC_BaseAddress, 100);
	gPLC_Ctrl.CommandRegRead();
}



void CDlgParmAutoRobotPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static int sCount = 0;
	if (nIDEvent == 1)
	{
		CString str;
		GetDlgItemText(IDC_STATIC_PLC_MODEL_NAME, str);
		if (gPLC_Ctrl.m_sModelName != str)
		{
			SetDlgItemText(IDC_STATIC_PLC_MODEL_NAME, gPLC_Ctrl.m_sModelName);
		}

		int l_cCheckLoad = m_cCheckLoad.GetCheck();			
		if (gPLC_Ctrl.m_cCheckLoad != l_cCheckLoad)
		{
			m_cCheckLoad.SetCheck(gPLC_Ctrl.m_cCheckLoad);
			if(gPLC_Ctrl.m_cCheckLoad)
				GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(0);
		}

		int l_cCheckUnLoad = m_cCheckUnLoad.GetCheck();
		if (gPLC_Ctrl.m_cCheckUnLoad != l_cCheckUnLoad)
		{
			m_cCheckUnLoad.SetCheck(gPLC_Ctrl.m_cCheckUnLoad);
			if (gPLC_Ctrl.m_cCheckUnLoad)
				GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(0);
		}



		int l_cCheck = (m_cCheckSkipFFC.GetCheck() ? 0:1);
		if (gPLC_Ctrl.m_cCheckSkipFFC != l_cCheck)
		{
			m_cCheckSkipFFC.SetCheck(gPLC_Ctrl.m_cCheckSkipFFC ? 0 : 1);
		}

		l_cCheck = m_cCheckPCBOnLoadSensor.GetCheck();
		if (gPLC_Ctrl.m_cCheckPCBOnLoadSensor != l_cCheck)
		{
			m_cCheckPCBOnLoadSensor.SetCheck(gPLC_Ctrl.m_cCheckPCBOnLoadSensor);
		}

	
		l_cCheck = m_cCheckRobotMoving.GetCheck();
		if (gPLC_Ctrl.m_cCheckRobotMoving != l_cCheck)
		{
			m_cCheckRobotMoving.SetCheck(gPLC_Ctrl.m_cCheckRobotMoving);
			if (gPLC_Ctrl.m_cCheckRobotMoving)
			{
				GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(0);
				
			}
			else
			{
				GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(1);
			}
		}

		l_cCheck = m_cCheckJigUpStatus.GetCheck();
		if (gPLC_Ctrl.m_cCheckJigUpStatus != l_cCheck)
		{
			m_cCheckJigUpStatus.SetCheck(gPLC_Ctrl.m_cCheckJigUpStatus);
		}
		l_cCheck = m_cCheckJigDownStatus.GetCheck();
		if (gPLC_Ctrl.m_cCheckJigDownStatus != l_cCheck)
		{
			m_cCheckJigDownStatus.SetCheck(gPLC_Ctrl.m_cCheckJigDownStatus);
		}

		int l_cCheckTestPass = m_cCheckTestPass.GetCheck();
		if (gPLC_Ctrl.m_cCheckTestPass != l_cCheckTestPass)
		{
			m_cCheckTestPass.SetCheck(gPLC_Ctrl.m_cCheckTestPass);
		}

		int l_cCheckTestNg = m_cCheckTestNg.GetCheck();
		if (gPLC_Ctrl.m_cCheckTestNg != l_cCheckTestNg)
		{
			m_cCheckTestNg.SetCheck(gPLC_Ctrl.m_cCheckTestNg);
		}
		int l_cCheckReconnect = m_cCheckReconnect.GetCheck();
		if (gPLC_Ctrl.m_cCheckReconnect != l_cCheckReconnect)
		{
			m_cCheckReconnect.SetCheck(gPLC_Ctrl.m_cCheckReconnect);
			if (gPLC_Ctrl.m_cCheckReconnect)
				GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(0);
		}

		int l_cCheckLoadFinish = m_cCheckLoadFinish.GetCheck();
		if (gPLC_Ctrl.m_cCheckLoadFinish != l_cCheckLoadFinish)
		{
			m_cCheckLoadFinish.SetCheck(gPLC_Ctrl.m_cCheckLoadFinish);
			
		}
	


		int l_cCheckUnLoadFinish = m_cCheckUnLoadFinish.GetCheck();
		if (gPLC_Ctrl.m_cCheckUnLoadFinish != l_cCheckUnLoadFinish)
		{
			m_cCheckUnLoadFinish.SetCheck(gPLC_Ctrl.m_cCheckUnLoadFinish);
		
		}
		
			   

		int l_cCheckReconnectFinish = m_cCheckReconnectFinish.GetCheck();
		if (gPLC_Ctrl.m_cCheckReconnectFinish != l_cCheckReconnectFinish)
		{
			m_cCheckReconnectFinish.SetCheck(gPLC_Ctrl.m_cCheckReconnectFinish);
			
		}


		int l_cCheckSetAlarm = m_cCheckSetAlarm.GetCheck();
		if (gPLC_Ctrl.m_cCheckSetAlarm != l_cCheckSetAlarm)
		{
			m_cCheckSetAlarm.SetCheck(gPLC_Ctrl.m_cCheckSetAlarm);
			
		}
			   		 	  	
		int l_cCheck_PLC_Auto_Mode = m_cCheck_PLC_Auto_Mode.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Auto_Mode != l_cCheck_PLC_Auto_Mode)
		{
			m_cCheck_PLC_Auto_Mode.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Auto_Mode);
		}

		int l_cCheck_PLC_Manual_Mode = m_cCheck_PLC_Manual_Mode.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode != l_cCheck_PLC_Manual_Mode)
		{
			m_cCheck_PLC_Manual_Mode.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Manual_Mode);
		}

		int l_cCheck_PLC_Start_Run = m_cCheck_PLC_Start_Run.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Start_Run != l_cCheck_PLC_Start_Run)
		{
			m_cCheck_PLC_Start_Run.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Start_Run);
		}

		int l_cCheck_PLC_Pause = m_cCheck_PLC_Pause.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Pause != l_cCheck_PLC_Pause)
		{
			m_cCheck_PLC_Pause.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Pause);
		}
		int l_cCheck_PLC_STOP = m_cCheck_PLC_STOP.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_STOP != l_cCheck_PLC_STOP)
		{
			m_cCheck_PLC_STOP.SetCheck(gPLC_Ctrl.m_cCheck_PLC_STOP);
		}

		int l_cCheck_PLC_Warn = m_cCheck_PLC_Warn.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Warn != l_cCheck_PLC_Warn)
		{
			m_cCheck_PLC_Warn.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Warn);
		}
		int l_cCheck_PLC_Alarm = m_cCheck_PLC_Alarm.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Alarm != l_cCheck_PLC_Alarm)
		{
			m_cCheck_PLC_Alarm.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Alarm);
		}

	

		int l_cCheck_PLC_Data_PCB_Inside = m_cCheck_PLC_Data_PCB_Inside.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside != l_cCheck_PLC_Data_PCB_Inside)
		{
			m_cCheck_PLC_Data_PCB_Inside.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside);
		}

		int l_cCheck_PLC_Data_DummyCut = m_cCheck_PLC_Data_DummyCut.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Data_DummyCut != l_cCheck_PLC_Data_DummyCut)
		{
			m_cCheck_PLC_Data_DummyCut.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Data_DummyCut);
		}

		int l_cCheck_PLC_Data_OK = m_cCheck_PLC_Data_OK.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Data_OK != l_cCheck_PLC_Data_OK)
		{
			m_cCheck_PLC_Data_OK.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Data_OK);
		}
		int l_cCheck_PLC_Data_NG = m_cCheck_PLC_Data_NG.GetCheck();
		if (gPLC_Ctrl.m_cCheck_PLC_Data_NG != l_cCheck_PLC_Data_NG)
		{
			m_cCheck_PLC_Data_NG.SetCheck(gPLC_Ctrl.m_cCheck_PLC_Data_NG);
		}

		sCount++;
		if (sCount > 20)
		{
			gPLC_Ctrl.CommandRead();// OnBnClickedBtnCommandRead();
			sCount = 0;
			if (gPLC_Ctrl.m_cCheckLoadFinish == 1)
			{
				if (gPLC_Ctrl.m_cCheckLoad == 1)
				{
					gPLC_Ctrl.CommandClear();
					if (gPLC_Ctrl.m_cCheckLoadFinish)
						GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(1);
				}

			}
			else if (gPLC_Ctrl.m_cCheckUnLoadFinish == 1)
			{
				if (gPLC_Ctrl.m_cCheckUnLoad == 1)
				{
					gPLC_Ctrl.CommandClear();
					if (gPLC_Ctrl.m_cCheckLoadFinish)
						GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(1);
				}
			}
			else if (gPLC_Ctrl.m_cCheckReconnectFinish == 1)
			{
				if (gPLC_Ctrl.m_cCheckReconnect == 1)
				{
					gPLC_Ctrl.CommandClear();
					if (gPLC_Ctrl.m_cCheckLoadFinish)
						GetDlgItem(IDC_BTN_MAIN_JIG_DOWN)->EnableWindow(1);
				}
			}
			
		}	
		else if (sCount == 10)
		{

			gPLC_Ctrl.CommandRegRead();// OnBnClickedBtnCommandRead();

			if (gPLC_Ctrl.m_cCheckSkipFFC == 0)
			{
				gPLC_Ctrl.CommandSkipFFC();
			}
			//gPLC_Ctrl.CommandRegRead();// OnBnClickedBtnCommandRead();
			////sCount = 0;
			//if (CurrentSet->bRobotSkipFFC == 1)
			//{
			//	if (gPLC_Ctrl.m_cCheckSkipFFC == 0)
			//	{
			//		gPLC_Ctrl.CommandSkipFFC();
			//	}
			//}
			//else
			//{
			//	if (gPLC_Ctrl.m_cCheckSkipFFC == 1)
			//	{
			//		gPLC_Ctrl.CommandConnectFFC();
			//	}
			//}
		}
		
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgParmAutoRobotPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		CString str;
		str.Format(_T("DFT_%d :Port %d. IP: "), CurrentSet->nDFT_No+1, CurrentSet->nPLC_Port);
		str += CurrentSet->strPLC_IP;

		SetDlgItemText(IDC_STATIC_PLC_IP_ADDRESS_PORT, str);
		
		SetTimer(1, 10, NULL);
		if (CurrentSet->nRunStartType != MANUAL_START)
		{
			if (IDYES == MessageBox("Robot Auto Process Running \r\n Change to Manual MODE?", "Process Warning!!", MB_YESNO))
			{
				CurrentSet->nRunStartType = MANUAL_START;
			}
		}

	}
	else
	{
		KillTimer(1);
	}
}


void CDlgParmAutoRobotPage::OnBnClickedBtnMainJigUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	gJigCtrl.Set_TestResult(0x03);
	
}


void CDlgParmAutoRobotPage::OnBnClickedBtnMainJigDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//void CGrabPage::OnBnClickedBtnMainJigDown()
			// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gJigCtrl.Set_CylinderDOWN();
	
}


void CDlgParmAutoRobotPage::OnBnClickedCheckGrabPower()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		m_bGrabPower = m_ctrlGrabPower.GetCheck();
		gJigCtrl.Set_Grabber_Power(m_bGrabPower);


	

}


void CDlgParmAutoRobotPage::OnBnClickedBtnResetIfpack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gJigCtrl.Set_IF_Pack_Reset(0);
}


void CDlgParmAutoRobotPage::OnBnClickedBtnResetGender()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gJigCtrl.Set_Gender_Reset(0);
}


void CDlgParmAutoRobotPage::OnBnClickedCheckHdmiUsbCyl()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bCheckHdmiUsbCyl = m_cCheckHdmiUsbCyl.GetCheck();
	gJigCtrl.Set_CylinderHDMI_USB(m_bCheckHdmiUsbCyl);
}


void CDlgParmAutoRobotPage::OnBnClickedCheckEarphone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int lcheck = m_cCheckEarphone.GetCheck();
	gJigCtrl.Set_CylinderEarphone(lcheck);
}



void CDlgParmAutoRobotPage::OnBnClickedCheckComp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int lcheck = m_cCheckComp.GetCheck();
	gJigCtrl.Set_CylinderComp(lcheck);
}


void CDlgParmAutoRobotPage::OnBnClickedCheckRelay1()//AC POWER
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
		m_bRelay1 = m_ctrlRelay1Chk.GetCheck();
		//AUTO
		if (g_nSysType == AUTO_SYS) {
			gJigCtrl.Set_ACPower((BYTE)m_bRelay1);
		}
		else {
			g_pView->Set_Relay(m_bRelay1);
		}
	
}





//
//void CDlgParmAutoRobotPage::OnBnClickedBtnCommandLoadFast()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	gPLC_Ctrl.CommandLoad();
//	gPLC_Ctrl.CommandSkipFFC();
//}
//
//
//void CDlgParmAutoRobotPage::OnBnClickedBtnCommandPassUnloadFast()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	gPLC_Ctrl.CommandPassUnload();
//	gPLC_Ctrl.CommandSkipFFC();
//
//}
//
//
//void CDlgParmAutoRobotPage::OnBnClickedBtnCommandNgLoadFast()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	gPLC_Ctrl.CommandNgUnload();
//	gPLC_Ctrl.CommandSkipFFC();
//
//}


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandSkipffc()
{
	return;//cys 200824
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (gPLC_Ctrl.m_cCheckSkipFFC == 1)
	{
		gPLC_Ctrl.CommandConnectFFC();
	} 
	else
	{
		gPLC_Ctrl.CommandSkipFFC();
	}
}


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandClearCommand()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gPLC_Ctrl.CommandClear();
}


void CDlgParmAutoRobotPage::OnBnClickedBtnCommandAlarm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	gPLC_Ctrl.Command3TimesError();
}
