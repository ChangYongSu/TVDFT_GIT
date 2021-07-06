// Gmes.cpp: implementation of the CGmes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datsys.h"
#include "Server_Ctrl.h"
#include "Markup.h"
#include "global.h"
#include "step.h"
#include "parser.h"
#include "PLC_Ctrl.h"

#include "IcmpEcho.h"
#include "JigCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CServer_Ctrl gServer_Ctrl;


extern CDATsysView*			g_pView;
extern CString				m_strWipId;	//Wip ID
extern CEdit			ctrlWipIdEdit;
extern CEdit			ctrlSuffixEdit;
extern CColorStaticST  ctrlPLC_COM_Status;//IDC_PLC_COM_STATUS
extern CColorStaticST  ctrlROBOT_Status;//IDC_ROBOT_STATUS
extern int g_AutoControlProcess;
extern CStringArray g_Divisions;

extern CJigCtrl gJigCtrl;

//ctrlPLC_COM_Status;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServer_Ctrl::CServer_Ctrl()
{
	m_bServer_CtrlCom = FALSE;

	m_sReceive = _T("");
	//m_sTransactionID = _T("");
	//m_sWorkOrderID = _T("");
	//m_sOrganizationID = _T("");
	//m_sLineID = _T("");
	//m_sEquipmentID = _T("");
	//m_sOperationID = _T("");
	//m_sModelID = _T("");

	//m_sModelSuffixName = _T("");
	//m_nOrganizationID = 0;
	//m_nDftSeq = 0;
	m_sErrorMsg = _T("");

	m_bCom_RetryCount = 0;

}

CServer_Ctrl::~CServer_Ctrl()
{

}

BOOL CServer_Ctrl::SoketCom_Creat(CString sIpAdd, int nPort)
{
	SOCKET connt;

	IcmpEcho ping;
	unsigned long ulIP;
	DWORD delay;
	CString strtmp;
	CString strtmpIPAddress;
	delay = 1000;
//#ifdef _NO_CONNECT_DEBUG__MODE
	ulIP = ping.ResolveIP(sIpAdd.GetBuffer());
	delay = ping.PingHostOK(ulIP, PING_TIMEOUT);


	//g_pView->m_pMonitorServerSocketClient.ondis

	if (delay >= 1000)
	{
		g_pView->m_pMonitorServerSocketClient.Close();//
#ifdef _DEBUG
		AddStringToStatus("Monitor Server error!");
#endif
		m_bServer_CtrlCom = FALSE;
		g_pView->m_pMonitorServerSocketClient.m_SerConnected = 0;
	
		return m_bServer_CtrlCom;
	}
//#endif
	if (m_bServer_CtrlCom == 1)
	{
		SoketCom_Close();
	}
	
	g_pView->m_pMonitorServerSocketClient.m_SerVerDeviceID = _SERVER_MONITOR_CONTROL_ID;
	g_pView->m_pMonitorServerSocketClient.Create();	
	//g_pView->m_pMonitorServerSocketClient.m_nTimeOut = 3;//

	//g_pView->m_pMonitorServerSocketClient.SetSockOpt(SO_RCVTIMEO, &timeout, sizeof(int));
	connt = g_pView->m_pMonitorServerSocketClient.Connect(sIpAdd, nPort);				//
//	g_pView->m_pMonitorServerSocketClient.AsyncSelect();


	if(connt){												//
		AddStringToStatus("Monitor Server Connection OK.");
		m_bServer_CtrlCom = TRUE;
		m_bServer_Ctrl_WaitCount = 0;
		m_bServer_Ctrl_RegWaitCount = 0;
		m_bServer_Ctrl_Online = 1;
		g_pView->m_pMonitorServerSocketClient.m_SerConnected = 1;
				
	}
	else{
		g_pView->m_pMonitorServerSocketClient.Close();//
		AddStringToStatus("Monitor Server error!");
		m_bServer_CtrlCom = FALSE;
		g_pView->m_pMonitorServerSocketClient.m_SerConnected = 0;
	}
	return m_bServer_CtrlCom;
}

void CServer_Ctrl::SoketCom_Close()
{
	g_pView->m_pMonitorServerSocketClient.Close();//소켓을 종료한다.
	AddStringToStatus("Server Connection Close");
	m_bServer_CtrlCom = FALSE;
}

BOOL CServer_Ctrl::SendCommString(CString strData)
{
	int nSendLenth;
	int nRtn;

//	if(!m_bGmesCom) {return FALSE;}

//	m_sReceive = _T("");
	nSendLenth = strData.GetLength();
//	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);

	nRtn = g_pView->m_pMonitorServerSocketClient.Send(strData, nSendLenth); //데이타 전송 부분

	if(nRtn != nSendLenth) return FALSE;

//	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);
	return TRUE;
}

BOOL CServer_Ctrl::ReceiveCommString(int nWaitLimit, CString& sReadString)
{
//	char	ReadBuffer[4096];	
	CString strRevText;
	int 		dwRead;		 

	m_sReceive = _T("");
//	memset(ReadBuffer, 0, 4096);
	ZeroMemory(m_sReadBuffer, sizeof(m_sReadBuffer));


	DWORD dwStartTick = GetTickCount();

	while (TRUE)
	{
//		dwRead = g_pView->m_pMonitorServerSocketClient.Receive(m_sReadBuffer, sizeof(m_sReadBuffer));
		dwRead = g_pView->m_pMonitorServerSocketClient.Receive(m_sReadBuffer, 500);

		if (dwRead >= 0) 
		{
			strRevText.Format("%s", m_sReadBuffer);
			m_sReceive = strRevText;
			sReadString = strRevText;
		//	delete [] ReadBuffer;
			m_nReadSize = dwRead;
			return TRUE;
		}

		if((GetTickCount() - dwStartTick) > (UINT)nWaitLimit)		
		{	
			return FALSE;
		}
		
		Sleep(10);
	}
	return TRUE;	
}

void CServer_Ctrl::DisplyComStatus(CString sItem, BOOL bFlag)
{
	if(!CurrentSet->bCommDisplay)
	{
		return;
	}

	CString sMsg;

	if(bFlag){
		sMsg.Format(_T("[RECV(SVR)] : %s"), sItem);
	}
	else{
		sMsg.Format(_T("[SEND(SVR)] : %s"), sItem);
	}

	AddStringToStatus(sMsg);
}

int CServer_Ctrl::WriteSend(CString strData)
{
	m_bCom_NGCount++;
	m_bCom_SndCount++;
	

	if (m_bServer_CtrlCom == 0)
	{
		return 0;
	}

	int iLen = strData.GetLength();
	if (iLen <= 0)
		return 0;

	int iSent = g_pView->m_pMonitorServerSocketClient.Send(LPCSTR(strData), iLen);
	if (iSent != iLen)
	{
		return 0;
	}
	// Were we able to send it?
	if (iSent == SOCKET_ERROR)
	{
		return 0;
	}
	else
	{
		ServerMonitorLog_Save(g_pView->m_szExePath, strData, FALSE);
		
	}

	m_bCom_NGCount = 0;
	return TRUE;
}



void CServer_Ctrl::LinkTestResponse() //S2F4 void CServer_Ctrl::LinkTestSend(int  lEQP_Index, int  lSet_Index) //S2F3
{
	//	<EIFVERSION="1.8"ID="S2F4"NAME="LinkTestResponse">
	//<ELEMENT>
	//<EQPID></EQPID>
	//</ELEMENT>
	//</EIF>
		//EquipmentStatusSend
	CString str1;
	
	if  (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F4\" NAME=\"Link Test Request\">\r\n");
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <EQPID>");
		str1.Format(_T("DFT_%d"), CurrentSet->nDFT_No);
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</EQPID>\r\n");
		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);

	//	str1 = "[SEND(SVR)] : S2F4";
	//	AddStringToStatus(str1);
	}
	
}


void CServer_Ctrl::ControlCommandReceive(CString lstrCommand, CString lstrData)//S2F103() //S2F4 void CServer_Ctrl::LinkTestSend(int  lEQP_Index, int  lSet_Index) //S2F3
{
	//m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F3\" NAME=\"Link Test Request\">\r\n");
	//m_strMesSend += _T("  <ELEMENT>\r\n");
	//m_strMesSend += _T("    <EQPID>");
	//m_strMesSend += gSaveInfo[i].strEQPID;
	//m_strMesSend += _T("</EQPID>\r\n");
	//m_strMesSend += _T("  </ELEMENT>\r\n");
	//m_strMesSend += _T("</EIF>\r\n");
	//gServer_Ctrl.ControlCommandAck();

	CString str1;

	if (lstrCommand == "START")
	{
		//#define		_SERVER_CTRL_IDLE		0
//#define		_SERVER_CTRL_START		1
//#define		_SERVER_CTRL_PAUSE		2
//#define		_SERVER_CTRL_STOP		3
		
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_START;

	}
	else if (lstrCommand == "PAUSE")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_PAUSE;


	}
	else if (lstrCommand == "STOP")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_STOP;


	}
	else if (lstrCommand == "GET TOOL OPTION")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_GETTOOL;

		gServer_Ctrl.m_ServerControlData = lstrData;
	}
	else if (lstrCommand == "GET TOOL OPTION CLOSE")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_GETTOOL_CLOSE;
		if (g_pView->m_GetToolOptiondlg.m_bActivate)
		{
			::SendMessage(g_pView->m_GetToolOptiondlg, UM_CLOSE_TOOL_OPTION, 0, 0);
		}		
	}	
	else if (lstrCommand == "GET MODEL LIST")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_GET_MODEL_LIST;

		//gServer_Ctrl.m_ServerControlData = lstrData;
	}	
	else if (lstrCommand == "CHANGE_MODEL")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_CHANGE_MODEL;

		gServer_Ctrl.m_ServerControlData = lstrData;
	}
	else if (lstrCommand == "JIGUP")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_JIG_UP;

	}
	else if (lstrCommand == "JIGDOWN")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_JIG_DOWN;
	}
	else if (lstrCommand == "LOAD")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_LOAD;

	}
	else if (lstrCommand == "TESTPASS")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_TEST_PASS;

	}
	else if (lstrCommand == "TESTNG")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_TEST_NG;
	}
	else if (lstrCommand == "CLEAR")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_TEST_CLEAR;
	}
	else if (lstrCommand == "READ")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_TEST_READ;
	}
	else if (lstrCommand == "AC_ON")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_AC_ON;
	}
	else if (lstrCommand == "AC_OFF")
	{
		gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_AC_OFF;
	}
	
}


void CServer_Ctrl::ControlCommandAck() //S2F4 void CServer_Ctrl::LinkTestSend(int  lEQP_Index, int  lSet_Index) //S2F3
{
	//m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F3\" NAME=\"Link Test Request\">\r\n");
	//m_strMesSend += _T("  <ELEMENT>\r\n");
	//m_strMesSend += _T("    <EQPID>");
	//m_strMesSend += gSaveInfo[i].strEQPID;
	//m_strMesSend += _T("</EQPID>\r\n");
	//m_strMesSend += _T("  </ELEMENT>\r\n");
	//m_strMesSend += _T("</EIF>\r\n");

	//gServer_Ctrl.ControlCommandAck();
	CString str1;
	
	if  (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F104\" NAME=\"COMMAND CONTROL ACK\">\r\n");
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <EQPID>");
		str1.Format(_T("DFT_%d"), CurrentSet->nDFT_No);
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</EQPID>\r\n");
		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);

	//	str1 = "[SEND(SVR)] : S2F4";
	//	AddStringToStatus(str1);
	}
	
}

void CServer_Ctrl::LinkTestSend() //S2F3
{
	//	<EIFVERSION="1.8"ID="S2F4"NAME="LinkTestResponse">
	//<ELEMENT>
	//<EQPID></EQPID>
	//</ELEMENT>
	//</EIF>
		//EquipmentStatusSend
	CString str1;

	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F3\" NAME=\"LinkTestResponse\">\r\n");
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <EQPID>");
		str1.Format(_T("DFT_%d"), CurrentSet->nDFT_No);
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</EQPID>\r\n");

	/*	m_strMesSend += _T("    <SETID>");
		str1.Format(_T("%d"), lSet_Index);
		m_strMesSend += str1;
		m_strMesSend += _T("</SETID>\r\n");
		m_strMesSend += _T("    <COMMAND>");
		str1 = "RESTART";
		m_strMesSend += str1;
		m_strMesSend += _T("</COMMAND>\r\n");*/

		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);
	}
}


void CServer_Ctrl::SendStatus() //S2F101
{
	//	<EIFVERSION="1.8"ID="S2F4"NAME="LinkTestResponse">
	//<ELEMENT>
	//<EQPID></EQPID>
	//</ELEMENT>
	//</EIF>
		//EquipmentStatusSend

	//MODEL 
	//ctrlWipIdEdit.SetWindowText(m_strWipId); //SN
	//ctrlSuffixEdit.SetWindowText(_T(""));

	//ctrlPLC_COM_Status
	//ctrlROBOT_Status

	//int m_cCheck_PLC_Auto_Mode;//:0
	//int m_cCheck_PLC_Manual_Mode;//:1
	//int m_cCheck_PLC_Start_Run;//:2
	//int m_cCheck_PLC_Pause;//:3
	//int m_cCheck_PLC_STOP;	//:4
	//int m_cCheck_PLC_Warn;	//:5
	//int m_cCheck_PLC_Alarm;//:6
	//int m_cCheckSkipFFC;	
	//int m_cCheck_PLC_Data_PCB_Inside;//:10
	//int m_cCheckJigUpStatus;
	//int m_cCheckJigDownStatus;

	//LIST
	//# No// NAME//Color// Result//COLOR
	//# No// NAME//Color// Result//COLOR
	//# No// NAME//Color// Result//COLOR
	//# No// NAME//Color// Result//COLOR
	//LOG ADD
	// Sring Add
	//LOG CLEAR
	CString str1;
	CString str2;

	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F101\" NAME=\"DFT Status Send\">\r\n");
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <EQPID>");
		str1.Format(_T("DFT_%d"), CurrentSet->nDFT_No);
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</EQPID>\r\n");


		ctrlWipIdEdit.GetWindowText(str1);
		m_strMesSend += _T("    <SETID>");		
		m_strMesSend += str1;
		m_strMesSend += _T("</SETID>\r\n");

		g_pView->m_stcSeqName.GetWindowText(str1);
		m_strMesSend += _T("    <MODEL NAME>");
		m_strMesSend += str1;
		m_strMesSend += _T("</MODEL NAME>\r\n");


		ctrlSuffixEdit.GetWindowText(str1);
		m_strMesSend += _T("    <MODEL SUFFIX>");
		m_strMesSend += str1;
		m_strMesSend += _T("</MODEL SUFFIX>\r\n");


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

		m_strMesSend += _T("    <ROBOT MAIN MODE>");
		str1.Format(_T("%d"), g_AutoControlProcess);
		m_strMesSend += str1;
		m_strMesSend += _T("</ROBOT MAIN MODE>\r\n");


	//	m_strMesSend += _T("    <ROBOT RUN PROCESS MODE>");
	////	str1 = "#Command:ADD//";
	//	m_strMesSend += gPLC_Ctrl.m_sPLCProcessMsg;
	//	m_strMesSend += _T("</ROBOT RUN PROCESS MODE>\r\n");

		
		m_strMesSend += _T("    <ROBOT CTRL>");
		str2 = "#PLC_COM_Status//";		
		//COLORREF
		str1.Format(_T("%d//"), ctrlPLC_COM_Status.GetTextColor());
		str2 += str1;
		//COLORREF
		str1.Format(_T("%d//"), ctrlPLC_COM_Status.GetBkColor());
		str2 += str1;
		ctrlPLC_COM_Status.GetWindowText(str1);
		str2 += str1;
		m_strMesSend += str2;

		str2 = "#PCB_Status//";
		//COLORREF
		str1.Format(_T("%d//"), ctrlROBOT_Status.GetTextColor());
		str2 += str1;
		//COLORREF
		str1.Format(_T("%d//"), ctrlROBOT_Status.GetBkColor());
		str2 += str1;
		ctrlROBOT_Status.GetWindowText(str1);
		str2 += str1;
		m_strMesSend += str2;
		m_strMesSend += _T("</ROBOT CTRL>\r\n");


			
		m_strMesSend += _T("    <PLC DATA>");
		str1.Format(_T("#Auto//%d//"), gPLC_Ctrl.m_cCheck_PLC_Auto_Mode);//:0
		m_strMesSend += str1;

		str1.Format(_T("#Manual//%d//"), gPLC_Ctrl.m_cCheck_PLC_Manual_Mode);//:0
		m_strMesSend += str1;

		str1.Format(_T("#Start//%d//"), gPLC_Ctrl.m_cCheck_PLC_Start_Run);//:0
		m_strMesSend += str1;

		str1.Format(_T("#Pause//%d//"), gPLC_Ctrl.m_cCheck_PLC_Pause);//:0
		m_strMesSend += str1;

		str1.Format(_T("#Stop//%d//"), gPLC_Ctrl.m_cCheck_PLC_STOP);//:0
		m_strMesSend += str1;

		str1.Format(_T("#Warn//%d//"), gPLC_Ctrl.m_cCheck_PLC_Warn);//:0
		m_strMesSend += str1;
		str1.Format(_T("#Alarm//%d//"), gPLC_Ctrl.m_cCheck_PLC_Alarm);//:0
		m_strMesSend += str1;
		str1.Format(_T("#SkipFFC//%d//"), gPLC_Ctrl.m_cCheckSkipFFC);//:0
		m_strMesSend += str1;
		str1.Format(_T("#PCBIn//%d//"), gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside);//:0
		m_strMesSend += str1;
		str1.Format(_T("#JigUp//%d//"), gPLC_Ctrl.m_cCheckJigUpStatus);//:0
		m_strMesSend += str1;
		str1.Format(_T("#JigDown//%d//"), gPLC_Ctrl.m_cCheckJigDownStatus);//:0
		m_strMesSend += str1;
		str1.Format(_T("#ACOnOff//%d//"), gJigCtrl.m_bAcPowerOnOff);//:0
		m_strMesSend += str1;

		m_strMesSend += _T("</PLC DATA>\r\n");




		m_strMesSend += _T("    <SEQ LIST>");
		int nStepCount;
		long lcheck;
		int lCurSel;
		nStepCount = StepList.GetCount();
		if (nStepCount > 0)
		{
			 // lCurSel = g_pView->m_ctrlSummaryGrid.GetRow();
			
			for (int i = 0; i < nStepCount; i++)
			{
				//g_pView->m_ctrlSummaryGrid.SetRow(i+1);
				//g_pView->m_ctrlSummaryGrid.SetCol(0);
				lcheck = g_pView->m_CtrlListMainProcess.GetCheck(i);// lcheck = g_pView->m_ctrlSummaryGrid.GetCellChecked();

				str1.Format(_T("#%d//%d//"),i , lcheck);//:0
				m_strMesSend += str1;
				
				//g_pView->m_ctrlSummaryGrid.SetCol(1);
				str1 = g_pView->m_CtrlListMainProcess.GetItemText(i, 0);//str1 = g_pView->m_ctrlSummaryGrid.GetText();
				str1 += "//";
				m_strMesSend += str1;
	/*			str1.Format(_T("%d//"), g_pView->m_ctrlSummaryGrid.GetCellBackColor());
				m_strMesSend += str1;*/


				//g_pView->m_ctrlSummaryGrid.SetCol(2);
				str1 = g_pView->m_CtrlListMainProcess.GetItemText(i, 1);////str1 = g_pView->m_ctrlSummaryGrid.GetText();
				str1 += "//";
				m_strMesSend += str1;

			
				//str1.Format(_T("%d//"), g_pView->m_ctrlSummaryGrid.GetCellBackColor());
				//m_strMesSend += str1;

				//g_pView->m_ctrlSummaryGrid.SetCol(3);
				str1 = g_pView->m_CtrlListMainProcess.GetItemText(i, 2);//str1 = g_pView->m_ctrlSummaryGrid.GetText();
				str1 += "//";
				m_strMesSend += str1;
				
				str1.Format(_T("%d//"), g_pView->m_CtrlListMainProcessEx.nSelBackColor[i][2]);////str1.Format(_T("%d//"), g_pView->m_ctrlSummaryGrid.GetCellBackColor());
				m_strMesSend += str1;
				str1.Format(_T("%d//"), g_pView->m_CtrlListMainProcessEx.nSelForeColor[i][2]);////str1.Format(_T("%d//"), g_pView->m_ctrlSummaryGrid.GetCellForeColor());
				m_strMesSend += str1;

//# No// NAME//Color// Result//COLOR



			}
			//g_pView->m_ctrlSummaryGrid.SetRow(lCurSel);

#ifdef	DEBUG_MD5_CODE__
			static int sCurSel = 0;
			sCurSel++;
			if (sCurSel > 30)
				sCurSel = 0;
			str1.Format(_T("#SEL//%d//"), sCurSel);//:0
			m_strMesSend += str1;
#else
			str1.Format(_T("#SEL//%d//"), lCurSel);//:0
			m_strMesSend += str1;
#endif
		}
		

		m_strMesSend += _T("</SEQ LIST>\r\n");
		//LIST
	
	//# No// NAME//Color// Result//COLOR
	//# No// NAME//Color// Result//COLOR
	//# No// NAME//Color// Result//COLOR
	//LOG ADD
	// Sring Add
	//LOG CLEAR


		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);
	}
}

void CServer_Ctrl::SendMsgCtrl(int ServerCmdID) //_SERVER_COM_RUNMSG_ID
{
	
	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected != 1)
	{
		return;
	}

	CString str1;
	CString str2;


		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F105\" NAME=\"DFT Status Send\">\r\n");
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <EQPID>");
		str1.Format(_T("DFT_%d"), CurrentSet->nDFT_No);
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</EQPID>\r\n");

	switch (ServerCmdID)
	{
	case  _SERVER_COM_COMMAND_ID:
		
		break;
	case  _SERVER_COM_RUNMSG_ID:
		m_strMesSend += _T("    <TEST RUN PROCESS MODE>");
		str1 = "#Command:";
		str1 += gPLC_Ctrl.m_sRunProcessMsgCommand;
		str1 += "//";
		m_strMesSend += str1;
		m_strMesSend += gPLC_Ctrl.m_sRunProcessMsg;
		m_strMesSend += _T("</TEST RUN PROCESS MODE>\r\n");

		break;

	case  _SERVER_COM_ROBOTMSG_ID:
		break;

	default:
		break;

	}
	

	m_strMesSend += _T("  </ELEMENT>\r\n");
	m_strMesSend += _T("</EIF>\r\n");

	WriteSend(m_strMesSend);
	
}

CString CServer_Ctrl::SetDisplayString()//CString CServer_Ctrl::SetComDisplayString()
{
	CString str1, str2;

	//ctrlPLC_COM_Status

	//CLabel_text *m_pcLB_Display[12];
	//m_pcLB_Display[0] = &m_cLB_Select[lIndex];
	//m_pcLB_Display[1] = &m_cLB_BarCode[lIndex];
	//m_pcLB_Display[2] = &m_cLB_Cnt_FM[lIndex];
	//m_pcLB_Display[3] = &m_cLB_Cnt_Time[lIndex];
	//m_pcLB_Display[4] = &m_cLB_SetStatus[lIndex];
	//m_pcLB_Display[5] = &m_cLB_AC_Power[lIndex];
	//m_pcLB_Display[6] = &m_cLB_Lan_Status[lIndex];
	//m_pcLB_Display[7] = &m_cLB_USB1_Status[lIndex];
	//m_pcLB_Display[8] = &m_cLB_USB2_Status[lIndex];
	//m_pcLB_Display[9] = &m_cLB_PowerStatus[lIndex];
	//m_pcLB_Display[10] = &m_cLB_OpenStatus[lIndex];
	//m_pcLB_Display[11] = &m_TotalAgingTimeLabel[lIndex];

	//CString str_head[12];

	//str_head[0] = "#Select//";
	//str_head[1] = "#BarCode//";
	//str_head[2] = "#Cnt_FM//";
	//str_head[3] = "#Cnt_Time//";
	//str_head[4] = "#SetStatus//";
	//str_head[5] = "#AC_Power//";
	//str_head[6] = "#Lan_Status//";
	//str_head[7] = "#USB1_Status//";
	//str_head[8] = "#USB2_Status//";
	//str_head[9] = "#PowerStatus//";
	//str_head[10] = "#OpenStatus//";
	//str_head[11] = "#AgingTimeLabel//";


	
	//long lColor = 0;

	//for (int i = 0; i < 12; i++)
	//{
	//	str1 += str_head[i];
	//	lColor = m_pcLB_Display[i]->get_ForeColor();
	//	str2.Format(_T("%8d"), lColor);
	//	str1 += str2;

	//	/*if(lColor == 0x00FF0000)
	//	{
	//		str1 += "BL";
	//	}
	//	else if(lColor == 0x0000FF00)
	//	{
	//		str1 += "GN";
	//	}
	//	else if(lColor == 0x000000FF)
	//	{
	//		str1 += "RD";
	//	}
	//	else if(lColor == 0x0000FFFF)
	//	{
	//		str1 += "YL";
	//	}
	//	else if(lColor == 0x00000000)
	//	{
	//		str1 += "BK";
	//	}
	//	else
	//	{
	//		str1 += "GR";
	//	}*/

	//	lColor = m_pcLB_Display[i]->get_BackColor();
	//	str2.Format(_T("%8d//"), lColor);
	//	str1 += str2;
	//	//if(lColor == 0x00FF0000)
	//	//{
	//	//	str1 += "BL//";
	//	//}
	//	//else if(lColor == 0x0000FF00)
	//	//{
	//	//	str1 += "GN//";
	//	//}
	//	//else if(lColor == 0x000000FF)
	//	//{
	//	//	str1 += "RD//";
	//	//}
	//	//else if(lColor == 0x0000FFFF)
	//	//{
	//	//	str1 += "YL//";
	//	//}
	//	//else if(lColor == 0x00000000)
	//	//{
	//	//	str1 += "BK//";
	//	//}
	//	//else 
	//	//{
	//	//	str1 += "GR//";
	//	//}

	//	str2 = m_pcLB_Display[i]->get_Caption();
	//	str1 += str2;
	//}
	return str1;
}

CString CServer_Ctrl::SetComDisplayString()
{

	CString str1, str2;


	//MODEL 
	//ctrlWipIdEdit.SetWindowText(m_strWipId); //SN

	//ctrlPLC_COM_Status
	//ctrlROBOT_Status
	//int m_cCheck_PLC_Auto_Mode;//:0
	//int m_cCheck_PLC_Manual_Mode;//:1
	//int m_cCheck_PLC_Start_Run;//:2
	//int m_cCheck_PLC_Pause;//:3
	//int m_cCheck_PLC_STOP;	//:4
	//int m_cCheck_PLC_Warn;	//:5
	//int m_cCheck_PLC_Alarm;//:6
	//int m_cCheckSkipFFC;	
	//int m_cCheck_PLC_Data_PCB_Inside;//:10
	//int m_cCheckJigUpStatus;
	//int m_cCheckJigDownStatus;

	





	//CString str_head[3];

	//str_head[0] = "#ComMain//";
	//str_head[1] = "#ComMainGmes//";
	//str_head[2] = "#ComMainMonitor//";




	//long lColor = 0;

	//for (int i = 0; i < 3; i++)
	//{
	//	str1 += str_head[i];
	//	lColor = m_pcLB_Display[i]->get_ForeColor();
	//	str2.Format(_T("%8d"), lColor);
	//	str1 += str2;



	//	lColor = m_pcLB_Display[i]->get_BackColor();
	//	str2.Format(_T("%8d//"), lColor);
	//	str1 += str2;


	//	str2 = m_pcLB_Display[i]->get_Caption();
	//	str1 += str2;
	//}
	return str1;
}



void CServer_Ctrl::SendModelList() //S2F101
{
	
	CString str1;
	CString str2;
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;

	INT_PTR lnModel = CurrentSet->ModelList.GetSize();
	if (lnModel <= 0)
		return;

	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F107\" NAME=\"DFT Model List Send\">\r\n");
		
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <MODEL COUNT>");
		str1.Format(_T("%d"), lnModel);
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</MODEL COUNT>\r\n");
	
		m_strMesSend += _T("    <MODEL LIST DATA>");		
		
		Pos = CurrentSet->ModelList.GetHeadPosition();

		while (Pos)
		{
			pModelData = CurrentSet->ModelList.GetNext(Pos);
			if (pModelData->m_szModelName != _T(""))
			{
				str1 = "#/m_szChassisName//";				
				str1 += pModelData->m_szChassisName;
				str1 += "#/m_szModelName//";
				str1 += pModelData->m_szModelName; 
				str1 += "#/m_szSeqFilePath//";
				str1 += pModelData->m_szSeqFilePath;
				str1 += "#/m_szModelInfoFilePath//";
				str1 += pModelData->m_szModelInfoFilePath;
				str1 += "#/m_szRefImgFolder//";
				str1 += pModelData->m_szRefImgFolder;
				str1 += "#/m_szDivision//";
				str1 += pModelData->m_szDivision;
				str1 += "#/m_szSeqFilePath2//";
				str1 += pModelData->m_szSeqFilePath2;			
				m_strMesSend += str1;
			}
		}
		str1 = "#/END";		
		m_strMesSend += str1;		
		for (int i = 0; i < g_Divisions.GetSize(); i++)
		{
			str1 = "#/g_Divisions//";
			str1 += g_Divisions.GetAt(i);
			m_strMesSend += str1;
		}
		str1 = "#/END";
		m_strMesSend += str1;

		m_strMesSend += _T("</MODEL LIST DATA>\r\n");

		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);
	}
}



void CServer_Ctrl::ServerChangeModelAck(int lOk)//S2F101
{	
	CString str1;
	CString str2;	

	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F110\" NAME=\"DFT Model Change Ack\">\r\n");
		
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <RESULT>");
		if (lOk)
			str1 = "OK";
		else
			str1 = "NG";
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</RESULT>\r\n");	

		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);
	}
}
void CServer_Ctrl::ServerReportTestResult(int lOk)//S2F101
{	
	CString str1;
	CString str2;	

	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F111\" NAME=\"Report Test Result\">\r\n");
		
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <RESULT>");
		if (lOk)
			str1 = "OK";
		else
			str1 = "NG";
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</RESULT>\r\n");	

		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);
	}
}
void CServer_Ctrl::ReportPCB_Check()//S2F101
{	
	CString str1;
	CString str2;	
	//void ReportPCB_Clear();//
	//void ReportPCB_Check();

	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F113\" NAME=\"Report PCB Inside\">\r\n");
		
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <PCB_CHECK>");
		//if (lOk)
		//	str1 = "OK";
		//else
		str1 = "ERROR";
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</PCB_CHECK>\r\n");	

		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);
	}
}
void CServer_Ctrl::ReportPCB_Clear()//S2F101
{	
	CString str1;
	CString str2;	
	//void ReportPCB_Clear();//
	//void ReportPCB_Check();

	if (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 1)
	{
		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S2F113\" NAME=\"Report PCB Inside\">\r\n");
		
		m_strMesSend += _T("  <ELEMENT>\r\n");
		m_strMesSend += _T("    <PCB_CHECK>");
	
		str1 = "CLEAR";
		
		m_strMesSend += str1;//  gSaveInfo[i].strEQPID;//
		m_strMesSend += _T("</PCB_CHECK>\r\n");	

		m_strMesSend += _T("  </ELEMENT>\r\n");
		m_strMesSend += _T("</EIF>\r\n");

		WriteSend(m_strMesSend);
	}
}

//BOOL CServer_Ctrl::Paser_ReceiveData(CString sReceiveData)
//{
//	
//	BOOL bRev = TRUE;	
//	CString sSetID;
//
//	//xmlTemp.SetDoc(sReceiveData);
//	//void CSockDlg::OnReceive()
//	//{
//	//	byte pBuf[4096];
//	//	int iBufSize = 4096;
//	//	int iRcvd;
//	//	CString strRecvd;
//	//	byte RcvData[256];
//
//	//	// Receive the message
//	//	iRcvd = m_sConnectSocket.Receive(pBuf, iBufSize);
//	//	strRecvd = _T("1:");
//	//	if (iRcvd == SOCKET_ERROR)
//	//	{
//	//		iRcvd = m_sConnectSocket2.Receive(pBuf, iBufSize);
//	//		strRecvd = _T("2:");
//	//	}
//	//	// Did we receive anything?
//	//	if (iRcvd == SOCKET_ERROR)
//	//	{
//	//	}
//	//	else
//	//	{
//	//		// Truncate the end of the message
//	//		pBuf[iRcvd] = NULL;
//	//		//strRecvd += pBuf;
//	//		//////////////////////////////////////////////////////////////
//	//		//Parcing 
//	//		if (iRcvd >= 11)
//	//		{
//	//			if ((pBuf[0] == 0xD0) && (pBuf[1] == 0x00))
//	//			{
//
//	//			}
//	//			else
//	//			{
//	//				strRecvd += "Head Error";
//	//				strRecvd += (char *)pBuf;
//	//				m_ctlRecvd.AddString(strRecvd);
//
//	//			}
//
//	//			int ReceiveSize = pBuf[8];// + pBuf[7];
//	//			ReceiveSize = ReceiveSize << 8;
//	//			ReceiveSize += pBuf[7] + 9;
//	//			if (iRcvd >= ReceiveSize)
//	//			{
//
//	//				int lDataLen;// = pBuf[20];
//	//			/*	lDataLen = lDataLen<<8;
//	//				lDataLen  +=  pBuf[19];*/
//
//
//	//				if ((pBuf[9] == 0x00) && (pBuf[10] == 0x00))//End Code
//	//				{
//	//					//	strRecvd += "ACK";
//	//						//strRecvd += (char *)pBuf;
//	//					//	m_ctlRecvd.AddString(strRecvd);
//	//						//return ;
//
//	//				}
//	//				else
//	//				{
//	//					strRecvd.Format(_T("Error Code: 0x%02X%02X"), pBuf[10], pBuf[9]);
//	//					//	strRecvd += (char *)pBuf;
//	//					m_ctlRecvd.AddString(strRecvd);
//	//					return;
//	//				}
//	//				lDataLen = ReceiveSize - 11;
//	//				lDataLen = lDataLen / 2;
//	//				if (lDataLen >= 100)
//	//				{
//	//				}
//	//				else
//	//				{
//	//					strRecvd += "ACK";
//	//					//	strRecvd += (char *)pBuf;
//	//					m_ctlRecvd.AddString(strRecvd);
//	//					return;
//	//				}
//
//	//				//IDC_STATIC_MODEL_NAME
//	//				char lModelBuf[64];
//	//				lModelBuf[40] = 0;
//	//				for (int i = 0; i < 20; i++)
//	//				{
//	//					lModelBuf[i * 2] = pBuf[11 + (i * 2)];
//	//					lModelBuf[i * 2 + 1] = pBuf[11 + (i * 2) + 1];
//	//				}
//	//				SetDlgItemText(IDC_STATIC_MODEL_NAME, lModelBuf);
//
//	//				for (int i = 0; i < lDataLen; i++)
//	//				{
//	//					RcvData[i] = pBuf[11 + (i * 2)];
//	//				}
//
//
//	//				/*	if(RcvData[_PLC_ADDRESS_MODEL_NAME])
//	//					{
//	//						m_cCheckLoad.SetCheck(1);
//	//					}
//	//					else
//	//					{
//	//						m_cCheckLoad.SetCheck(0);
//	//					}*/
//
//	//				if (RcvData[_PLC_ADDRESS_LOAD_COMMAND])
//	//				{
//	//					m_cCheckLoad.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckLoad.SetCheck(0);
//	//				}
//
//	//				if (RcvData[_PLC_ADDRESS_UNLOAD_COMMAND])
//	//				{
//	//					m_cCheckUnLoad.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckUnLoad.SetCheck(0);
//	//				}
//
//	//				if (RcvData[_PLC_ADDRESS_TEST_PASS])
//	//				{
//	//					m_cCheckTestPass.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckTestPass.SetCheck(0);
//	//				}
//
//	//				if (RcvData[_PLC_ADDRESS_TEST_NG])
//	//				{
//	//					m_cCheckTestNg.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckTestNg.SetCheck(0);
//	//				}
//
//	//				if (RcvData[_PLC_ADDRESS_FFC_RETRY_COMMAND])
//	//				{
//	//					m_cCheckReconnect.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckReconnect.SetCheck(0);
//	//				}
//
//	//				//if(RcvData[_PLC_ADDRESS_MODEL_NAME])
//	//				//{
//	//				//	m_cCheckLoad.SetCheck(1);
//	//				//}
//	//				//else
//	//				//{
//	//				//	m_cCheckLoad.SetCheck(0);
//	//				//}
//
//	//				if (RcvData[_PLC_ADDRESS_LOAD_FINISH])
//	//				{
//	//					m_cCheckLoadFinish.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckLoadFinish.SetCheck(0);
//	//				}
//
//	//				if (RcvData[_PLC_ADDRESS_UNLOAD_FINISH])
//	//				{
//	//					m_cCheckUnLoadFinish.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckUnLoadFinish.SetCheck(0);
//	//				}
//
//	//				if (RcvData[_PLC_ADDRESS_FFC_RETRY_COMMAND])
//	//				{
//	//					m_cCheckReconnect.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckReconnect.SetCheck(0);
//	//				}
//
//	//				if (RcvData[_PLC_ADDRESS_FFC_RETRY_FINISH])
//	//				{
//	//					m_cCheckReconnectFinish.SetCheck(1);
//	//				}
//	//				else
//	//				{
//	//					m_cCheckReconnectFinish.SetCheck(0);
//	//				}
//
//
//	//				//	CButton m_cCheckLoadFinish;
//	//				//	CButton m_cCheckUnLoad;
//	//				//	CButton m_cCheckUnLoadFinish;
//	//				//	CButton m_cCheckTestPass;
//	//				//	CButton m_cCheckTestNg;
//	//				//	CButton m_cCheckReconnect;
//	//				//	CButton m_cCheckReconnectFinish;
//
//
//	//			}
//	//			else
//	//			{
//	//				strRecvd += "Size Error";
//	//				strRecvd += (char *)pBuf;
//	//				m_ctlRecvd.AddString(strRecvd);
//
//	//			}
//
//	//		}
//
//	//		// Copy the message to a CString
//	//	//	strRecvd += pBuf;
//
//	//		// Add the message to the received list box
//	//	//	m_ctlRecvd.AddString(strRecvd);
//
//
//	//		// Sync the variables with the controls
//	//		UpdateData(TRUE);
//
//	//	}
//	//}
//
//
//	return bRev;
//}
//
//int CServer_Ctrl::WriteFullData(UINT Address, char* Data, int lDataLen)
//{
//	byte packet[256];
//	if (lDataLen <= 0)
//		return 0;
//
//	byte lData40[64];
//	lData40[40] = 0;
//	memset(lData40, 0x20, 40);
//	memcpy(lData40, Data, lDataLen);
//
//	int writeSize = 12 + 40;//lDataLen;
//
//	packet[0] = 0x50;
//
//	packet[1] = 0x00;
//
//	packet[2] = 0x00;// (byte)this._nNetworkNo;                         // Packet - 네트워크 번호   
//	packet[3] = 0xFF;//(byte)this._nPLCNo;                         // Packet - PLC 번호
//
//	packet[4] = 0xFF;//(byte)(this._nIONo & 0x000000FF);           // Packet - IO 번호 Low    
//	packet[5] = 0x03;//(byte)((this._nIONo & 0x0000FF00) >> 8);    // 
//
//	packet[6] = 0x00;//(byte)this._nStationNo;                     // Packet - Station 번호
//
//	packet[7] = (byte)(writeSize & 0x000000FF);                  // data size - Low
//	packet[8] = (byte)((writeSize & 0x0000FF00) >> 8);                   // data size - HIGH
//
//
//	packet[9] = 0x0A; // Monitoring Timer Low
//	packet[10] = 0x00; // Monitoring Timer High
//
//	packet[11] = 0x01; //READ Command 0401 LOW
//	packet[12] = 0x14; //READ Command 0401 HIGH
//
//
//	packet[13] = 0x00; ////READ Command SUB 0000 LOW
//	packet[14] = 0x00; //READ Command SUB 0000 HIGH
//
//	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
//	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
//	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High
//
//	packet[18] = 0xA8; //D* A8H = Data register
//
//	int lWordLen = 20;
//	packet[19] = (byte)(lWordLen & 0xFF);     // Low    
//	packet[20] = (byte)(lWordLen >> 8);       // High
//
//	for (int i = 0; i < lWordLen; i++)
//	{
//		packet[21 + (i * 2)] = (byte)(lData40[i * 2]);     // Low
//		packet[22 + (i * 2)] = (byte)(lData40[i * 2 + 1]);       // High
//	}
//
//	int iLen = 21 + 40;//(lDataLen);//m_strMessage.GetLength();
//
//	//nRtn = g_pView->m_pMonitorServerSocketClient.Send(strData, nSendLenth);
//	int iSent = g_pView->m_pMonitorServerSocketClient.Send(packet, iLen);
//
//	// Were we able to send it?
//	if (iSent == SOCKET_ERROR)
//	{
//
//	}
//	else
//	{
//		// Add the message to the list box.
//		//m_ctlSent.AddString(m_strMessage);
//		//// Sync the variables with the controls
//		//UpdateData(TRUE);
//	}
//}
//
//int CServer_Ctrl::WriteCommand(UINT Address, char* Data, int lDataLen)
//{
//	byte packet[256];
//	if (lDataLen <= 0)
//		return 0;
//
//
//	int writeSize = 12 + lDataLen * 2;
//
//	packet[0] = 0x50;
//
//	packet[1] = 0x00;
//
//	packet[2] = 0x00;// (byte)this._nNetworkNo;                         // Packet - 네트워크 번호   
//	packet[3] = 0xFF;//(byte)this._nPLCNo;                         // Packet - PLC 번호
//
//	packet[4] = 0xFF;//(byte)(this._nIONo & 0x000000FF);           // Packet - IO 번호 Low    
//	packet[5] = 0x03;//(byte)((this._nIONo & 0x0000FF00) >> 8);    // 
//
//	packet[6] = 0x00;//(byte)this._nStationNo;                     // Packet - Station 번호
//
//	packet[7] = (byte)(writeSize & 0x000000FF);                  // data size - Low
//	packet[8] = (byte)((writeSize & 0x0000FF00) >> 8);                   // data size - HIGH
//
//
//	packet[9] = 0x0A; // Monitoring Timer Low
//	packet[10] = 0x00; // Monitoring Timer High
//
//	packet[11] = 0x01; //READ Command 0401 LOW
//	packet[12] = 0x14; //READ Command 0401 HIGH
//
//
//	packet[13] = 0x00; ////READ Command SUB 0000 LOW
//	packet[14] = 0x00; //READ Command SUB 0000 HIGH
//
//	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
//	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
//	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High
//
//	packet[18] = 0xA8; //D* A8H = Data register
//
//	packet[19] = (byte)(lDataLen & 0xFF);     // Low    
//	packet[20] = (byte)(lDataLen >> 8);       // High
//
//	for (int i = 0; i < lDataLen; i++)
//	{
//		packet[21 + (i * 2)] = (byte)(Data[i] & 0xFF);     // Low
//		packet[22 + (i * 2)] = 0x00;//(byte)(sndData[i] >> 8);       // High
//	}
//
//	int iLen = 21 + (lDataLen * 2);//m_strMessage.GetLength();
//
//
//	//int iSent = m_sConnectSocket.Send(packet, iLen);
//	int iSent = g_pView->m_pMonitorServerSocketClient.Send(packet, iLen);
//	
//
//	// Were we able to send it?
//	if (iSent == SOCKET_ERROR)
//	{
//
//	}
//	else
//	{
//		//// Add the message to the list box.
//		//m_ctlSent.AddString(m_strMessage);
//		//// Sync the variables with the controls
//		//UpdateData(TRUE);
//	}
//}
//
//int CServer_Ctrl::ReadCommand(UINT Address, int lDataLen)
//{
//	byte packet[256];
//	if (lDataLen <= 0)
//		return 0;
//
//
//	int writeSize = 12;
//
//	packet[0] = 0x50;
//
//	packet[1] = 0x00;
//
//	packet[2] = 0x00;// (byte)this._nNetworkNo;                         // Packet - 네트워크 번호   
//	packet[3] = 0xFF;//(byte)this._nPLCNo;                         // Packet - PLC 번호
//
//	packet[4] = 0xFF;//(byte)(this._nIONo & 0x000000FF);           // Packet - IO 번호 Low    
//	packet[5] = 0x03;//(byte)((this._nIONo & 0x0000FF00) >> 8);    // 
//
//	packet[6] = 0x00;//(byte)this._nStationNo;                     // Packet - Station 번호
//
//	packet[7] = (byte)(writeSize & 0x000000FF);                  // data size - Low
//	packet[8] = (byte)((writeSize & 0x0000FF00) >> 8);                   // data size - HIGH
//
//
//	packet[9] = 0x0A; // Monitoring Timer Low
//	packet[10] = 0x00; // Monitoring Timer High
//
//	packet[11] = 0x01; //READ Command 0401 LOW
//	packet[12] = 0x04; //READ Command 0401 HIGH
//
//
//	packet[13] = 0x00; ////READ Command SUB 0000 LOW
//	packet[14] = 0x00; //READ Command SUB 0000 HIGH
//
//	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
//	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
//	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High
//
//	packet[18] = 0xA8; //D* A8H = Data register
//
//	packet[19] = (byte)(lDataLen & 0xFF);     // Low    
//	packet[20] = (byte)(lDataLen >> 8);       // High
//
//	int iLen = 21;//m_strMessage.GetLength();
//
//
//	int iSent = g_pView->m_pMonitorServerSocketClient.Send(packet, iLen);
//
//	// Were we able to send it?
//	if (iSent == SOCKET_ERROR)
//	{
//
//	}
//	else
//	{
//		// Add the message to the list box.
//		//m_ctlSent.AddString(m_strMessage);
//		//// Sync the variables with the controls
//		//UpdateData(TRUE);
//	}
//}
//
//
//int CServer_Ctrl::ReadRegData(UINT Address, int lDataLen)
//{
//	byte packet[256];
//	if (lDataLen <= 0)
//		return 0;
//
//
//	int writeSize = 12;
//
//	packet[0] = 0x50;
//
//	packet[1] = 0x00;
//
//	packet[2] = 0x00;// (byte)this._nNetworkNo;                         // Packet - 네트워크 번호   
//	packet[3] = 0xFF;//(byte)this._nPLCNo;                         // Packet - PLC 번호
//
//	packet[4] = 0xFF;//(byte)(this._nIONo & 0x000000FF);           // Packet - IO 번호 Low    
//	packet[5] = 0x03;//(byte)((this._nIONo & 0x0000FF00) >> 8);    // 
//
//	packet[6] = 0x00;//(byte)this._nStationNo;                     // Packet - Station 번호
//
//	packet[7] = (byte)(writeSize & 0x000000FF);                  // data size - Low
//	packet[8] = (byte)((writeSize & 0x0000FF00) >> 8);                   // data size - HIGH
//
//
//	packet[9] = 0x0A; // Monitoring Timer Low
//	packet[10] = 0x00; // Monitoring Timer High
//
//	packet[11] = 0x01; //READ Command 0401 LOW
//	packet[12] = 0x04; //READ Command 0401 HIGH
//
//
//	packet[13] = 0x00; ////READ Command SUB 0000 LOW
//	packet[14] = 0x00; //READ Command SUB 0000 HIGH
//
//	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
//	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
//	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High
//
//	packet[18] = 0xAF;//R* File Register 0xA8; //D* A8H = Data register
//
//	packet[19] = (byte)(lDataLen & 0xFF);     // Low    
//	packet[20] = (byte)(lDataLen >> 8);       // High
//
//	int iLen = 21;//m_strMessage.GetLength();
//
//
//	int iSent = g_pView->m_pMonitorServerSocketClient.Send(packet, iLen);
//
//	// Were we able to send it?
//	if (iSent == SOCKET_ERROR)
//	{
//
//	}
//	else
//	{
//		// Add the message to the list box.
//		//m_ctlSent.AddString(m_strMessage);
//		//// Sync the variables with the controls
//		//UpdateData(TRUE);
//	}
//}
//
