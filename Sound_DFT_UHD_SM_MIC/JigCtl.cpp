#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "CurrentSetting.h"
#include "JigCtl.h"
#include "PLC_Ctrl.h"

extern CCurrentSetting	g_CurSetting;

CJigCtrl gJigCtrl;

CJigCtrl::CJigCtrl()
{
	m_bPortOpen = FALSE;
	m_strErrMsg = _T("");
	m_nPortID	= -1;
	m_nCylinder = 0x03;
	m_bGet_JigReady = FALSE;
	m_bAcPowerOnOff = FALSE;
	m_nDPMS_5CheckFlag = 0;
}

CJigCtrl::~CJigCtrl()
{
}

// Open Com Port
BOOL CJigCtrl::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");

	if( !m_ctrlJigCom.OpenPort( sComPort, nBaudRate, m_nPortID, 8, 0, 0,TRUE) )
	{
		m_strErrMsg.Format("[Jig CommPort] \n%s Init Failed!", sComPort);
		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	Sleep(150);

	m_hReadEvent = ::CreateEvent(NULL,false,false,NULL);
	
	return TRUE;
}

void CJigCtrl::PortClose()
{
	m_ctrlJigCom.ClosePort();
	m_bPortOpen = FALSE;
	if(m_hReadEvent != NULL)
	{
		CloseHandle(m_hReadEvent); m_hReadEvent = NULL;
	}
}

BOOL CJigCtrl::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}


BOOL CJigCtrl::SendByte(BYTE Data)
{
	SendCommString(&Data, 1);
	return TRUE;
}

BOOL CJigCtrl::SendCommString(BYTE* str, int nLength)
{
	m_ctrlJigCom.m_QueueRead.Clear();
	m_ctrlJigCom.WriteComm(str, nLength);

	CString sTmp;
	CString strReadData2 = _T("PC>>Jig ");
	for (int idx1 = 0; idx1 < nLength; idx1++)
	{
		//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);  //
		gJigCtrl.m_strReceive[idx1] = str[idx1];
		sTmp.Format("%02X ", str[idx1]);
		strReadData2 = strReadData2 + sTmp;
	}

	AddStringToStatus(strReadData2);

	return TRUE;
}
BOOL CJigCtrl::ReceiveString(CString &str, int nWaitLimit)
{
	BOOL		bDoing;
    clock_t		start;
	CString		sTemp;
	CString     sSendData = _T("");
	int         nPos = -1;
	int         nPos1 = -1;
//	int         nLoop = 0;
	int         nTemp;

	MSG msg;
	DWORD dwEventResult = 0;

//	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();

	while (bDoing)
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

		dwEventResult = WaitForSingleObject(m_hReadEvent, 500);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(m_hReadEvent);
		}

		nTemp = m_sReceive.GetLength();
		if(nTemp > 3)
		{	
		//	nPos = m_sReceive.Find("03");
			if(m_strReceive[3] == 0x03)
			{
				str = m_sReceive;

				CString sTmp;
				CString strReadData2 = _T("Jig>> PC ");
				//for (int idx1 = 0; idx1 < nTemp; idx1++)
				//{
				//	//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);  //
				//	gJigCtrl.m_strReceive[idx1] = str[idx1];
				//	sTmp.Format("%02X ", str[idx1]);
				//	strReadData2 = strReadData2 + sTmp;
				//}
				strReadData2 += str;
				AddStringToStatus(strReadData2);
				break;
			}
		}

        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {
			m_ctrlJigCom.m_QueueRead.Clear();
			return FALSE;
		}

		if(CurrentSet->bRunAbort)
		{
			m_ctrlJigCom.m_QueueRead.Clear();
			return FALSE;
		}
	}
	return TRUE;	
}


BOOL CJigCtrl::Set_ACPower(BYTE nData)
{
	CString szCmdString;
	CString		szData;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);
	
	m_strSend[0] = 0x02;
	m_strSend[1] = 0x41;
	m_strSend[2] = nData;
	m_strSend[3] = 0x03;

//	szCmdString.Format("%cA%c%c",STX,nData,ETX);

//	::ResetEvent(m_hReadEvent);
	if(!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if(!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if(szData == "02580103"){
		if(nData == 0){
			m_bAcPowerOnOff = FALSE;
		}
		else{
			m_bAcPowerOnOff = TRUE;
		}
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CJigCtrl::Set_TestResult(BYTE nData)
{
	CString szCmdString;
	CString		szData;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);
	
	m_strSend[0] = 0x02;
	m_strSend[1] = 0x47;  //G
	m_strSend[2] = nData;
	m_strSend[3] = 0x03;
	
//	szCmdString.Format("%cG%02X%c",STX,nData,ETX);

	::ResetEvent(m_hReadEvent);

	m_bGet_JigReady = TRUE;
	if(!SendCommString(m_strSend, 4))
	{
		m_bGet_JigReady = FALSE;
		return FALSE;
	}
	if(!ReceiveString(szData, 3000))
	{
		m_bGet_JigReady = FALSE;
		return FALSE;
	}

	m_bGet_JigReady = FALSE;
	//if (szData == "02580103") {
	if (szData == "02440003") {
			return TRUE;
	}
	else{
		return FALSE;
	}
}
BOOL CJigCtrl::Set_TestStatus(BYTE nData)
{
	CString szCmdString;
	CString		szData;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);
	
	m_strSend[0] = 0x02;
	m_strSend[1] = 0x52;  //R
	m_strSend[2] = nData;
	m_strSend[3] = 0x03;
	
//	szCmdString.Format("%cR%02X%c",STX,nData,ETX);

//	::ResetEvent(m_hReadEvent);
	if(!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if(!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if(szData == "02580103"){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL CJigCtrl::Set_CylinderUP()
{
	gJigCtrl.Set_TestResult(0x03);
	return 1;
}

BOOL CJigCtrl::Set_CylinderDOWN()
{
	CString szCmdString;
	CString		szData;

	
	
	if (!m_bPortOpen) return FALSE;

	//if (bDown) {
	//	m_nCylinder = m_nCylinder & (~HDMI_USB_SIGNAL);
	//}
	//else {
	//	m_nCylinder = m_nCylinder | HDMI_USB_SIGNAL;
	//}
	if (gPLC_Ctrl.m_cCheckRobotMoving == 1)
	{
		AddStringToStatus("Cannot Down Robot Moving!!");
		return FALSE;
	}

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	m_strSend[1] = 0x44;  //D
	m_strSend[2] = 0x01;
	m_strSend[3] = 0x03;

	//	szCmdString.Format("%cC%02X%c",STX,nData,ETX);

	//	::ResetEvent(m_hReadEvent);
	if (!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if (szData == "02440003") {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CJigCtrl::Set_CylinderHDMI_USB(BOOL bHdmiUsb)
{
	CString szCmdString;
	CString		szData;

	if (!m_bPortOpen) return FALSE;

	if (bHdmiUsb) {
		m_nCylinder = m_nCylinder | HDMI_USB_SIGNAL;
	}
	else {
		m_nCylinder = m_nCylinder & (~HDMI_USB_SIGNAL);
	}

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	m_strSend[1] = 0x43;  //C
	m_strSend[2] = m_nCylinder;
	m_strSend[3] = 0x03;

	//	szCmdString.Format("%cC%02X%c",STX,nData,ETX);

	//	::ResetEvent(m_hReadEvent);
	if (!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if (szData == "02580103") {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CJigCtrl::Set_CylinderEarphone(BOOL bEarPhone)
{
	CString szCmdString;
	CString		szData;

	if (!m_bPortOpen) return FALSE;

	if (bEarPhone) {
		m_nCylinder = m_nCylinder | ERAPHONE_SIGNAL;
	}
	else {
		m_nCylinder = m_nCylinder & (~ERAPHONE_SIGNAL);
	}

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	m_strSend[1] = 0x43;  //C
	m_strSend[2] = m_nCylinder;
	m_strSend[3] = 0x03;

	//	szCmdString.Format("%cC%02X%c",STX,nData,ETX);

	//	::ResetEvent(m_hReadEvent);
	if (!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if (szData == "02580103") {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CJigCtrl::Set_CylinderComp(BOOL bComp)
{
	CString szCmdString;
	CString		szData;

	if(!m_bPortOpen) return FALSE;
	
	if(bComp){
		m_nCylinder = m_nCylinder | COMPONENT_SIGNAL;
	}
	else{
		m_nCylinder = m_nCylinder & (~COMPONENT_SIGNAL);
	}

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);
	
	m_strSend[0] = 0x02;
	m_strSend[1] = 0x43;  //C
	m_strSend[2] = m_nCylinder;
	m_strSend[3] = 0x03;
	
//	szCmdString.Format("%cC%02X%c",STX,nData,ETX);

//	::ResetEvent(m_hReadEvent);
	if(!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if(!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if(szData == "02580103"){
		return TRUE;
	}
	else{
		return FALSE;
	}
}


BOOL CJigCtrl::Set_Cylinder(BOOL bEarPhone, BOOL bComp)
{
	CString szCmdString;
	CString		szData;

	if (!m_bPortOpen) return FALSE;

	if (bEarPhone) {
		//m_nCylinder = m_nCylinder | ERAPHONE_SIGNAL ;
		m_nCylinder = m_nCylinder | ERAPHONE_SIGNAL | HDMI_USB_SIGNAL;
	}
	else {
		m_nCylinder = m_nCylinder & (~ERAPHONE_SIGNAL);
	}

	if (bComp) {
		m_nCylinder = m_nCylinder | COMPONENT_SIGNAL;
	}
	else {
		m_nCylinder = m_nCylinder & (~COMPONENT_SIGNAL);
	}

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	m_strSend[1] = 0x43;  //C
	m_strSend[2] = m_nCylinder;
	m_strSend[3] = 0x03;

	//	szCmdString.Format("%cC%02X%c",STX,nData,ETX);

	//	::ResetEvent(m_hReadEvent);
	if (!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if (szData == "02580103") {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


BOOL CJigCtrl::Set_CylinderAll(BYTE Data)
{
	CString szCmdString;
	CString		szData;

	if (!m_bPortOpen) return FALSE;

	//if (bEarPhone) {
	//	//m_nCylinder = m_nCylinder | ERAPHONE_SIGNAL ;
	//	m_nCylinder = m_nCylinder | ERAPHONE_SIGNAL | HDMI_USB_SIGNAL;
	//}
	//else {
	//	m_nCylinder = m_nCylinder & (~ERAPHONE_SIGNAL);
	//}

	//if (bComp) {
	//	m_nCylinder = m_nCylinder | COMPONENT_SIGNAL;
	//}
	//else {
	//	m_nCylinder = m_nCylinder & (~COMPONENT_SIGNAL);
	//}
	if (m_nCylinder != Data)
		m_nCylinder = Data;
		
	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	m_strSend[1] = 0x43;  //C
	m_strSend[2] = m_nCylinder;
	m_strSend[3] = 0x03;

	//	szCmdString.Format("%cC%02X%c",STX,nData,ETX);

	//	::ResetEvent(m_hReadEvent);
	if (!SendCommString(m_strSend, 4))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
	if (szData.Find("02580103") >= 0) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


BOOL CJigCtrl::Get_JigReady()
{
	CString szCmdString;
	CString		szData;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);
	
	m_strSend[0] = 0x02;
	m_strSend[1] = 0x53;  //S
	m_strSend[2] = 0x00;
	m_strSend[3] = 0x03;
	
//	szCmdString.Format("%cS00%c",STX,ETX);
//	::ResetEvent(m_hReadEvent);

	m_bGet_JigReady = TRUE;

	if(!SendCommString(m_strSend, 4))
	{
		m_bGet_JigReady = FALSE;
		return FALSE;
	}
	if(!ReceiveString(szData, 3000))
	{
		m_bGet_JigReady = FALSE;
		return FALSE;
	}

	m_bGet_JigReady = FALSE;

	if(m_strReceive[2]&0x01){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

BOOL CJigCtrl::Set_Grabber_Power(BOOL bGrabPower)
{
	CString szCmdString;
	CString		szData;


	if (!m_bPortOpen) return FALSE;

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	if(bGrabPower == 1)
	{ 
		sprintf((char *)&m_strSend[1], "CGRAB_PON");
	}
	else
	{
		sprintf((char *)&m_strSend[1], "CGRAB_OFF");
	}
	
	m_strSend[10] = 0x03;
	m_strSend[11] = 0x0d;
	m_strSend[12] = 0x0a;
	
	if (!SendCommString(m_strSend, 13))
	{	
		AddStringToStatus("GRAB_PON SEND FAIL");
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{	
		AddStringToStatus("GRAB_PON RECV FAIL");
		return FALSE;
	}
	
	if (bGrabPower == 1)
	{
		if (strncmp((char *)&m_strReceive[1],"DGRAB_PON",9) == 0)
		{
			AddStringToStatus("GRAB_PON OK");
			return TRUE;
		}
		else {
			AddStringToStatus("GRAB_PON NG");
			return FALSE;
		}		
	}
	else
	{
		if (strncmp((char *)&m_strReceive[1], "DGRAB_OFF", 9) == 0)
		{
			AddStringToStatus("DGRAB_OFF OK");
			return TRUE;
		}
		else {
			AddStringToStatus("DGRAB_OFF NG");
			return FALSE;
		}
	}
}

BOOL CJigCtrl::Set_IF_Pack_Reset(BOOL bReset)
{

	if (g_nCommandOnlyType == TRUE)
		return FALSE;

	CString szCmdString;
	CString		szData;

	if (!m_bPortOpen) return FALSE;

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	//if (bReset == 1)
	//{
	sprintf((char *)&m_strSend[1], "CIFPK_RST");
	//}
	//else
	//{
	//	sprintf((char *)&m_strSend[1], "CGRAB_OFF");
	//}

	m_strSend[10] = 0x03;
	m_strSend[11] = 0x0d;
	m_strSend[12] = 0x0a;


	if (!SendCommString(m_strSend, 13))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{
		return FALSE;
	}


	//if (bReset == 1)
	//{
	if (strncmp((char *)&m_strReceive[1], "DIFPK_RST", 9) == 0)
	{
		return TRUE;
	}
	else {
		return FALSE;
	}

	/*}
	else
	{
		if (strncmp((char *)&m_strReceive[1], "DGRAB_OFF", 9) == 0)
		{
			return TRUE;
		}
		else {
			return FALSE;
		}
	}*/
}

BOOL CJigCtrl::Set_Gender_Reset(BOOL bReset)
{
	CString szCmdString;
	CString		szData;

	if (!m_bPortOpen) return FALSE;

	m_ctrlJigCom.ClearPort();
	memset(m_strSend, 0, 5);

	m_strSend[0] = 0x02;
	
	sprintf((char *)&m_strSend[1], "CGNDR_RST");
	
	m_strSend[10] = 0x03;
	m_strSend[11] = 0x0d;
	m_strSend[12] = 0x0a;


	if (!SendCommString(m_strSend, 13))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 3000))
	{
		return FALSE;
	}
		
	if (strncmp((char *)&m_strReceive[1], "DGNDR_RST", 9) == 0)
	{


		return TRUE;
	}
	else 
	{
		return FALSE;
	}

	
}

////JIG Count Control
//BOOL			m_bID_Ctrl_PortOpen;
//CCommThread		m_ID_Ctrl_Com;
//BYTE			m_strID_Ctrl_Receive[225];
//BYTE			m_strID_Ctrl_Send[125];
//CString			m_sID_Ctrl_Receive;
//int				m_nID_Ctrl_PortID;




// Open Com Port
BOOL CJigCtrl::ID_Ctrl_Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nID_Ctrl_PortID = atoi(sTmp);

	m_strID_Ctrl_ErrMsg = _T("");

	if (!m_ID_Ctrl_Com.OpenPort(sComPort, nBaudRate, m_nID_Ctrl_PortID, 8, 0, 0, TRUE))
	{
		m_strID_Ctrl_ErrMsg.Format("[JIG ID READ CommPort] \n%s Init Failed!", sComPort);
		m_bID_Ctrl_PortOpen = FALSE;
		return FALSE;
	}
	else m_bID_Ctrl_PortOpen = TRUE;

	Sleep(150);

	//m_hReadEvent = ::CreateEvent(NULL, false, false, NULL);

	return TRUE;
}

void CJigCtrl::ID_Ctrl_PortClose()
{
	m_ID_Ctrl_Com.ClosePort();
	m_bID_Ctrl_PortOpen = FALSE;

}

BOOL CJigCtrl::ID_Ctrl_SendCommString(BYTE* str, int nLength)
{
	m_ID_Ctrl_Com.m_QueueRead.Clear();
	m_sID_Ctrl_Receive = "";
	memset(m_strID_Ctrl_Receive, 0, 225);
	m_nID_Ctrl_ReceiveCnt = 0;

	m_ID_Ctrl_Com.WriteComm(str, nLength);

	CString sTmp;
	CString strReadData2 = _T("PC>>JigID ");
	for (int idx1 = 0; idx1 < nLength; idx1++)
	{
		//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);  //
		//gJigCtrl.m_strReceive[idx1] = str[idx1];
		sTmp.Format("%02X ", str[idx1]);
		strReadData2 = strReadData2 + sTmp;
	}

	AddStringToStatus(strReadData2);

	return TRUE;
}


BOOL CJigCtrl::ID_Ctrl_ReceiveString(CString &str, int nWaitLimit)
{
	BOOL		bDoing;
	clock_t		start;
	CString		sTemp;
	CString     sSendData = _T("");
	int         nPos = -1;
	int         nPos1 = -1;
	//	int         nLoop = 0;
	int         nTemp;

	MSG msg;
	DWORD dwEventResult = 0;

	
	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
	/*	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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

		dwEventResult = WaitForSingleObject(m_hReadEvent, 500);

		if (dwEventResult == WAIT_OBJECT_0)
		{
			::ResetEvent(m_hReadEvent);
		}*/

		nTemp = m_sID_Ctrl_Receive.GetLength();
		if (nTemp >= 2)
		{
			
			if (m_sID_Ctrl_Receive.Find('\r')> 1)
			{
				str = m_sID_Ctrl_Receive;

				CString sTmp;
				CString strReadData2 = _T("JigID>> PC ");
				//for (int idx1 = 0; idx1 < nTemp; idx1++)
				//{
				//	//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);  //
				//	gJigCtrl.m_strReceive[idx1] = str[idx1];
				//	sTmp.Format("%02X ", str[idx1]);
				//	strReadData2 = strReadData2 + sTmp;
				//}
				strReadData2 += str;
				AddStringToStatus(strReadData2);
				break;
			}
		}

		if ((clock() - start) * 1000 / CLOCKS_PER_SEC > nWaitLimit)
		{
			m_ID_Ctrl_Com.m_QueueRead.Clear();

			if (m_sID_Ctrl_Receive.Find('\r') == -1)
			{
				CString strReadData2 = _T("JigID>> PC FAIL ");
				
				strReadData2 += str;
				AddStringToStatus(strReadData2);
			}
			return FALSE;
		}

		if (CurrentSet->bRunAbort)
		{
			m_ID_Ctrl_Com.m_QueueRead.Clear();
			return FALSE;
		}
	}
	return TRUE;
}


BOOL CJigCtrl::ID_Ctrl_ReadID(CString &str, int nWaitLimit) 
{
	CString szCmdString;
	CString		szData;
	
	if (!m_bID_Ctrl_PortOpen) return FALSE;

	m_ID_Ctrl_Com.ClearPort();
	memset(m_strID_Ctrl_Send, 0, sizeof(m_strID_Ctrl_Send));
	//Read\n
	
	sprintf((char *)m_strID_Ctrl_Send, "Read\n");
	


	if (!ID_Ctrl_SendCommString(m_strID_Ctrl_Send, 5))
	{
		return FALSE;
	}
	if (!ID_Ctrl_ReceiveString(szData, 3000))
	{
		return FALSE;
	}


	CString strReadData2;
	CString sTemp = m_sID_Ctrl_Receive;
	//	‘Tx CompanyInfo ChassisInfo DateInfo SerialNo CounterData\r’
	//	CString		m_strID_CompanyInfo;
	//CString		m_strID_ChassisInfo;
	//CString		m_strID_DateInfo;
	//CString		m_strID_SerialNo;
	sTemp.Trim();
	int lnPos = sTemp.Find("Tx ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	sTemp = sTemp.Mid(lnPos);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_CompanyInfo = sTemp.Left(lnPos);
	//CompanyInfo ChassisInfo DateInfo SerialNo CounterData
	strReadData2 = _T("CompanyInfo : ");
	strReadData2 += m_strID_CompanyInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos+1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_ChassisInfo = sTemp.Left(lnPos);
	strReadData2 = _T("ChassisInfo : ");
	strReadData2 += m_strID_ChassisInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos+1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_DateInfo = sTemp.Left(lnPos);
	strReadData2 = _T("DateInfo : ");
	strReadData2 += m_strID_DateInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos+1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_SerialNo = sTemp.Left(lnPos);
	strReadData2 = _T("SerialNo : ");
	strReadData2 += m_strID_SerialNo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.GetLength();
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_nJigID_UseCount = _ttoi(sTemp);

	//CompanyInfo ChassisInfo DateInfo SerialNo CounterData
	strReadData2 = _T("CounterData : ");
	strReadData2 += sTemp;
	AddStringToStatus(strReadData2);

	return TRUE;
	

	
}


BOOL CJigCtrl::ID_Ctrl_IncCount()
{
	//‘Inc\n
	CString szCmdString;
	CString		szData;
	CString strReadData2;
	CString sTemp;
	int lnPos;
#ifdef _DEBUG
	sTemp = "Tx LGEKR WBS03 2024/01/03 0001 255\r";;//m_sID_Ctrl_Receive;

	sTemp.Trim();
	 lnPos = sTemp.Find("Tx ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	sTemp = sTemp.Mid(lnPos + 3);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_CompanyInfo = sTemp.Left(lnPos);
	//CompanyInfo ChassisInfo DateInfo SerialNo CounterData
	strReadData2 = _T("CompanyInfo : ");
	strReadData2 += m_strID_CompanyInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_ChassisInfo = sTemp.Left(lnPos);
	strReadData2 = _T("ChassisInfo : ");
	strReadData2 += m_strID_ChassisInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_DateInfo = sTemp.Left(lnPos);
	strReadData2 = _T("DateInfo : ");
	strReadData2 += m_strID_DateInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_SerialNo = sTemp.Left(lnPos);
	strReadData2 = _T("SerialNo : ");
	strReadData2 += m_strID_SerialNo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.GetLength();
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_nJigID_UseCount = _ttoi(sTemp);

	//CompanyInfo ChassisInfo DateInfo SerialNo CounterData
	strReadData2 = _T("CounterData : ");
	strReadData2 += sTemp;
	AddStringToStatus(strReadData2);

	
#endif
	if (!m_bID_Ctrl_PortOpen) return FALSE;

	m_ID_Ctrl_Com.ClearPort();
	memset(m_strID_Ctrl_Send, 0, sizeof(m_strID_Ctrl_Send));
	

	sprintf((char *)m_strID_Ctrl_Send, "Inc\n");

	if (!ID_Ctrl_SendCommString(m_strID_Ctrl_Send, 4))
	{
		return FALSE;
	}
	if (!ID_Ctrl_ReceiveString(szData, 3000))
	{
		return FALSE;
	}

	
	sTemp = m_sID_Ctrl_Receive;
	
	sTemp.Trim();
	 lnPos = sTemp.Find("Tx ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	sTemp = sTemp.Mid(lnPos+3);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_CompanyInfo = sTemp.Left(lnPos);
	//CompanyInfo ChassisInfo DateInfo SerialNo CounterData
	strReadData2 = _T("CompanyInfo : ");
	strReadData2 += m_strID_CompanyInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_ChassisInfo = sTemp.Left(lnPos);
	strReadData2 = _T("ChassisInfo : ");
	strReadData2 += m_strID_ChassisInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_DateInfo = sTemp.Left(lnPos);
	strReadData2 = _T("DateInfo : ");
	strReadData2 += m_strID_DateInfo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.Find(" ");
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_strID_SerialNo = sTemp.Left(lnPos);
	strReadData2 = _T("SerialNo : ");
	strReadData2 += m_strID_SerialNo;
	AddStringToStatus(strReadData2);

	sTemp = sTemp.Mid(lnPos + 1);
	lnPos = sTemp.GetLength();
	if (lnPos < 0)
	{
		return FALSE;
	}
	m_nJigID_UseCount = _ttoi(sTemp);

	//CompanyInfo ChassisInfo DateInfo SerialNo CounterData
	strReadData2 = _T("CounterData : ");
	strReadData2 += sTemp;
	AddStringToStatus(strReadData2);

	m_strID_JigFullInfo = m_strID_CompanyInfo+ " " + m_strID_ChassisInfo + " " + m_strID_DateInfo + " " + m_strID_SerialNo;

	return TRUE;
}


BOOL CJigCtrl::ID_Ctrl_ReadCount(CString &str, int nWaitLimit) 
{
	return TRUE;
}




//// DPMS Open Com Port
//void DPMS_PortClose();
//
////BOOL ID_Ctrl_SendByte(BYTE Data);
//BOOL DPMS_SendCommString(BYTE* str, int nLength);
//BOOL DPMS_ReadID(CString &str, int nWaitLimit);
//BOOL DPMS_ReceiveString(CString &str, int nWaitLimit);

BOOL CJigCtrl::DPMS_Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nDPMS_PortID = atoi(sTmp);

	m_strDPMS_ErrMsg = _T("");

	if (!m_DPMS_Com.OpenPort(sComPort, nBaudRate, m_nDPMS_PortID, 8, 0, 0, TRUE))
	{
		m_strDPMS_ErrMsg.Format("[DPMS READ CommPort] \n%s Init Failed!", sComPort);
		m_bDPMS_PortOpen = FALSE;
		return FALSE;
	}
	else m_bDPMS_PortOpen = TRUE;

	Sleep(150);

	//m_hReadEvent = ::CreateEvent(NULL, false, false, NULL);

	return TRUE;
}

void CJigCtrl::DPMS_PortClose()
{
	m_DPMS_Com.ClosePort();
	m_bDPMS_PortOpen = FALSE;

}
//방식: RS232C(default Option) MODBUS RTU
//	속도 : 9600bps
//
//	1. Power Data 읽기
//
//	요청(HEX Data 8 bytes) : 0x02 0x03 0x00 0x11 0x00 0x01 0xd4 0x3c + 1초이상 wait
//	응답(HEX Data 8 bytes)) :       0x02 0x03 0x00 0x02 MSB LSB CRC1 CRC2
//
//	2.Power 계산
//
//	Watt = (MSB * 256 + LSB) / 10

BOOL CJigCtrl::DPMS_SendCommString(BYTE* str, int nLength)
{
	m_DPMS_Com.m_QueueRead.Clear();
	m_sDPMS_Receive = "";
	memset(m_strDPMS_Receive, 0, 225);
	m_nDPMS_ReceiveCnt = 0;
	
	m_DPMS_Com.WriteComm(str, nLength);

	CString sTmp;
	CString strReadData2 = _T("PC>>DPMS ");
	for (int idx1 = 0; idx1 < nLength; idx1++)
	{
		//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);  //
		//gJigCtrl.m_strReceive[idx1] = str[idx1];
		sTmp.Format("%02X ", str[idx1]);
		strReadData2 = strReadData2 + sTmp;
	}

	AddStringToStatus(strReadData2);

	return TRUE;
}


BOOL CJigCtrl::DPMS_ReceiveString(int nCount, int nWaitLimit)
{
	BOOL		bDoing;
	clock_t		start;
	CString		sTemp;
	CString     sSendData = _T("");
	int         nPos = -1;
	int         nPos1 = -1;
	//	int         nLoop = 0;
	int         nTemp;

	MSG msg;
	DWORD dwEventResult = 0;

	
	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
		Sleep(10);
		//nTemp = m_sDPMS_Receive.GetLength();
		if (m_nDPMS_ReceiveCnt >= nCount)
		{
			//	응답(HEX Data 8 bytes)) :       0x02 0x03 0x00 0x02 MSB LSB CRC1 CRC2
			if ((m_strDPMS_Receive[0] == 0x02)
				&&(m_strDPMS_Receive[1] == 0x03))
				//&&(m_strDPMS_Receive[2] == 0x02))
			{
				
				double lReadVal = (m_strDPMS_Receive[3] * 256. + m_strDPMS_Receive[4])/10.0;
				m_fDPMS_WattData = lReadVal;
				CString sTmp;
				CString strReadData2 = _T("DPMS>> PC ");
				for (int idx1 = 0; idx1 < m_nDPMS_ReceiveCnt; idx1++)
				{
					//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);
					
					sTmp.Format("%02X ", m_strDPMS_Receive[idx1]);
					strReadData2 = strReadData2 + sTmp;
				}
				//strReadData2 += str;
				AddStringToStatus(strReadData2);
				break;
			}
		}

		if ((clock() - start) * 1000 / CLOCKS_PER_SEC > nWaitLimit)
		{
			m_DPMS_Com.m_QueueRead.Clear();
			CString strReadData2 = _T("DPMS>> PC TIME OUT ");

			if(m_nDPMS_ReceiveCnt >0)
			{ 
				
				CString sTmp;
				
				for (int idx1 = 0; idx1 < m_nDPMS_ReceiveCnt; idx1++)
				{
					//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);

					sTmp.Format("%02X ", m_strDPMS_Receive[idx1]);
					strReadData2 = strReadData2 + sTmp;
				}
				//strReadData2 += str;
				AddStringToStatus(strReadData2);
				
			}
			//else
			//{
			//	CString strReadData2 = _T("DPMS>> PC FAIL ");

			//	CString sTmp;

			//	for (int idx1 = 0; idx1 < nTemp; idx1++)
			//	{
			//		//(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&str);

			//		sTmp.Format("%02X ", m_strDPMS_Receive[idx1]);
			//		strReadData2 = strReadData2 + sTmp;
			//	}
			//	//strReadData2 += str;
			//}
			
			AddStringToStatus(strReadData2);

			return FALSE;
		}

		if (CurrentSet->bRunAbort)
		{
			m_DPMS_Com.m_QueueRead.Clear();
			return FALSE;
		}
	}
	return TRUE;
}


double CJigCtrl::DPMS_Read_Check( int Manual )
{
	CString szCmdString;
	CString		szData;
	CString		sMsg;

	if (!m_bDPMS_PortOpen)
	{
		//sMsg.Format("DPMS COM Port Not Open ERROR !! ", CurrentSet->m_nDPMS_5CheckCnt);
		AddStringToStatus("DPMS COM Port Not Open ERROR !! ");
		return -1000;//FALSE;//  FALSE;
	}
	
	//if (gJigCtrl.m_nDPMS_5CheckFlag != 1)
	//{
	//	gJigCtrl.m_nDPMS_5CheckFlag = 1;
	//}
	//if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes )
	//{
	//	if (CurrentSet->m_nDPMS_5CheckCnt % 5 == 0)
	//	{
	//		//m_nDPMS_5CheckCnt;
	//	}
	//	else
	//	{
	//		sMsg.Format("DPMS Check Skip:%d ", CurrentSet->m_nDPMS_5CheckCnt);
	//		AddStringToStatus(sMsg);
	//		return ((pCurStep->GetHighLimit() + pCurStep->GetLowLimit())/2);
	//		
	//	}
	//}


	m_DPMS_Com.ClearPort();
	memset(m_strDPMS_Send, 0, sizeof(m_strDPMS_Send));
	//Read\n
	//
	m_strDPMS_Send[0] = 0x02; 
	m_strDPMS_Send[1] = 0x03;
	m_strDPMS_Send[2] = 0x00;
	m_strDPMS_Send[3] = 0x11;
	m_strDPMS_Send[4] = 0x00;
	m_strDPMS_Send[5] = 0x01;
	m_strDPMS_Send[6] = 0xd4;
	m_strDPMS_Send[7] = 0x3c;
	


	if (!DPMS_SendCommString(m_strDPMS_Send, 8))
	{

		return  -1000;//FALSE;// FALSE;

	}
	if (Manual == 1)
	{
		return 1;
	}

	if (!DPMS_ReceiveString(7, 3000))
	{
		return  -1000;//FALSE;//FALSE;
	}

	//요청(HEX Data 8 bytes) : 0x02 0x03 0x00 0x11 0x00 0x01 0xd4 0x3c + 1초이상 wait
	//	응답(HEX Data 8 bytes)) :       0x02 0x03 0x00 0x02 MSB LSB CRC1 CRC2

	//	2.Power 계산
	if (m_fDPMS_WattData == 0)
		m_fDPMS_WattData = 0.00001;

	//	Watt = (MSB * 256 + LSB) / 10
	if (Manual == 1)
	{

		sMsg.Format("DPMS : %0.1f ", m_fDPMS_WattData );
		AddStringToStatus(sMsg);
		return  m_fDPMS_WattData;
	}
	else
	{
		if ((m_fDPMS_WattData >= pCurStep->m_fLowLimit) && (m_fDPMS_WattData <= pCurStep->m_fHighLimit))
		{
			sMsg.Format("DPMS OK: %0.1f [%0.1f~%0.1f]", m_fDPMS_WattData, pCurStep->m_fLowLimit, pCurStep->m_fHighLimit);
			AddStringToStatus(sMsg);
			return  m_fDPMS_WattData;
			//return TRUE;// m_fDPMS_WattData;
		}
		else
		{
			sMsg.Format("DPMS NG: %0.1f [%0.1f~%0.1f]", m_fDPMS_WattData, pCurStep->m_fLowLimit, pCurStep->m_fHighLimit);
			AddStringToStatus(sMsg);
			return   m_fDPMS_WattData;
			//return FALSE;//  m_fDPMS_WattData;
		}
	}
	//CompanyInfo ChassisInfo DateInfo SerialNo CounterData

}


bool CJigCtrl::DPMS_Count_Check()
{
	CString szCmdString;
	CString		szData;
	CString		sMsg;

	if (CurrentSet->nDPMS_SampleRate == 1)
	{
		return 1;
	}
	
	if (gJigCtrl.m_nDPMS_5CheckFlag != 1)
	{
		gJigCtrl.m_nDPMS_5CheckFlag = 1;
	}
	if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)
	{
		if (CurrentSet->m_nDPMS_5CheckCnt % 5 == 1)
		{
			return 1;
		}
		else
		{
			sMsg.Format("DPMS Check Skip:%d ", CurrentSet->m_nDPMS_5CheckCnt);
			AddStringToStatus(sMsg);
			return 0;

		}
	}
	return 1;




}
