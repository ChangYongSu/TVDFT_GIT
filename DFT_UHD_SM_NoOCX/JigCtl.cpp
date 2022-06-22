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

	if(!m_bPortOpen) return FALSE;
	
	if(bEarPhone){
		//m_nCylinder = m_nCylinder | ERAPHONE_SIGNAL ;
		m_nCylinder = m_nCylinder | ERAPHONE_SIGNAL | HDMI_USB_SIGNAL;
	}
	else{
		m_nCylinder = m_nCylinder & (~ERAPHONE_SIGNAL);
	}

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