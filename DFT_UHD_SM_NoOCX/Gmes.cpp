// Gmes.cpp: implementation of the CGmes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datsys.h"
#include "Gmes.h"
#include "Markup.h"
#include "global.h"
#include "step.h"
#include "parser.h"

#include "IcmpEcho.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CGmes gGmesCtrl;

extern CDATsysView*			g_pView;
extern CString				m_strWipId;	//Wip ID

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGmes::CGmes()
{
	m_bGmesCom = FALSE;
	m_sReceive = _T("");
	m_sTransactionID = _T("");
	m_sWorkOrderID = _T("");
	m_sOrganizationID = _T("");
	m_sLineID = _T("");
	m_sEquipmentID = _T("");
	m_sOperationID = _T("");
	m_sModelID = _T("");
	m_sModelName = _T("");
	m_sModelSuffixName = _T("");
	m_nOrganizationID = 0;
	m_nDftSeq = 0;
	m_sErrorMsg = _T("");
}

CGmes::~CGmes()
{

}

BOOL CGmes::SoketCom_Creat(CString sIpAdd, int nPort)
{
	SOCKET connt;

	
	IcmpEcho ping;
		unsigned long ulIP;
	DWORD delay;
	CString strtmp;
	CString strtmpIPAddress;
	delay = 1000;

	#ifdef _NO_CONNECT_DEBUG__MODE
	ulIP = ping.ResolveIP(sIpAdd.GetBuffer() );
	delay = ping.PingHostOK( ulIP, PING_TIMEOUT);


	if(delay > 2000)
	{
		g_pView->m_pSocketClient.Close();//소켓을 종료한다.
		AddStringToStatus("GMES Connection error!");
		//	AfxMessageBox("GMES Connection error!");
		m_bGmesCom = FALSE;
		g_pView->m_pSocketClient.m_SerConnected = 0;
		return m_bGmesCom;
	}

#endif
	

	g_pView->m_pSocketClient.m_SerVerDeviceID = _SERVER_GMES_ID;
	g_pView->m_pSocketClient.Create();										//클라이언트 소켓 생성
	connt = g_pView->m_pSocketClient.Connect(sIpAdd, nPort);					//서버에 연결을 시도 한다.

	if(connt){												//오류 발생시
		AddStringToStatus("GMES Connection OK.");
		m_bGmesCom = TRUE;
		g_pView->m_pSocketClient.m_SerConnected = 1;
	}
	else{
		g_pView->m_pSocketClient.Close();//소켓을 종료한다.
		AddStringToStatus("GMES Connection error!");
	//	AfxMessageBox("GMES Connection error!");
		m_bGmesCom = FALSE;
		g_pView->m_pSocketClient.m_SerConnected = 0;
	}
	return m_bGmesCom;
}
void CGmes::SoketCom_Close()
{
	g_pView->m_pSocketClient.Close();//소켓을 종료한다.
	AddStringToStatus("GMES Connection Close");
	m_bGmesCom = FALSE;
}

BOOL CGmes::SendCommString(CString strData)
{
	int nSendLenth;
	int nRtn;

	CString strTmp;

//	if(!m_bGmesCom) {return FALSE;}

//	m_sReceive = _T("");
	nSendLenth = strData.GetLength();
//	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);

	nRtn = g_pView->m_pSocketClient.Send(strData, nSendLenth); //데이타 전송 부분

	if (nRtn != nSendLenth)
	{
		//SoketCom_Close();
		//if (SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort))
		//{
		//	nSendLenth = strData.GetLength();
		//	//	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);

		//	nRtn = g_pView->m_pSocketClient.Send(strData, nSendLenth);
		//}
		nRtn = g_pView->m_pSocketClient.Send(strData, nSendLenth);
		if (nRtn != nSendLenth)
		{
			strTmp = "GMES RETRY!!";
			
			DisplyComStatus(strTmp, FALSE);
			strTmp += strData;
			
			GmesLog_Save(g_pView->m_szExePath, strTmp, FALSE);
			Gmes_Connection();

			nRtn = g_pView->m_pSocketClient.Send(strData, nSendLenth);
			if (nRtn != nSendLenth)
			{
				
				nRtn = g_pView->m_pSocketClient.Send(strData, nSendLenth);
				if (nRtn != nSendLenth)
				{
					strTmp = "GMES FAIL!!";
					DisplyComStatus(strTmp, FALSE);
					strTmp += strData;
					
					GmesLog_Save(g_pView->m_szExePath, strTmp, FALSE);

					return FALSE;
				}
			}
			//return FALSE;
		}
	}

	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);
	return TRUE;
}

BOOL CGmes::ReceiveCommString(int nWaitLimit, CString& sReadString)
{
//	char	ReadBuffer[4096];	
	CString strRevText;
	DWORD		dwRead;		 

	m_sReceive = _T("");
//	memset(ReadBuffer, 0, 4096);
	ZeroMemory(m_sReadBuffer, sizeof(m_sReadBuffer));


	DWORD dwStartTick = GetTickCount();

	while (TRUE)
	{
		dwRead = g_pView->m_pSocketClient.Receive(m_sReadBuffer, sizeof(m_sReadBuffer));

		if (dwRead >= 0) 
		{
			strRevText.Format("%s", m_sReadBuffer);
			m_sReceive = strRevText;
			sReadString = strRevText;
		//	delete [] ReadBuffer;
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

void CGmes::DisplyComStatus(CString sItem, BOOL bFlag)
{
	if(!CurrentSet->bCommDisplay)
	{
		return;
	}

	CString sMsg;

	if(bFlag){
		sMsg.Format(_T("[RECV(GMES)] : %s"), sItem);
	}
	else{
		sMsg.Format(_T("[SEND(GMES)] : %s"), sItem);
	}

	AddStringToStatus(sMsg);
}
BOOL CGmes::ChangeSystemTime(CString sTimeInfo)
{
	if(sTimeInfo.GetLength() < 14){ return FALSE;}
	
	SYSTEMTIME stm;
	BOOL bRev;

	int nY = atoi(sTimeInfo.Left(4));
	int nM = atoi(sTimeInfo.Mid(4,2));
	int nD = atoi(sTimeInfo.Mid(6,2));
	int nH = atoi(sTimeInfo.Mid(8,2));
	int nMin = atoi(sTimeInfo.Mid(10,2));
	int nS = atoi(sTimeInfo.Mid(12,2));

	stm.wYear = nY;
	stm.wMonth = nM;
	stm.wDay = nD;
	stm.wHour = nH;
	stm.wMinute = nMin;
	stm.wSecond = nS;
	stm.wMilliseconds = 0;

	bRev = SetLocalTime(&stm);

	return bRev;
}

CString CGmes::MakeElem_S1F1()
{
	CString sXmlDoc;
	CMarkup xmlTemp;

//	<EIF VERSION="1.00" ID="S1F1" NAME="Are You There Request">
//	</EIF>


//	xmlTemp.AddElem("Temp");
	xmlTemp.AddElem("EIF");
//	xmlTemp.IntoElem();
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S1F1");
	xmlTemp.AddAttrib("NAME", "Are You There Request");
	xmlTemp.AddChildElem("Temp");
	xmlTemp.RemoveChildElem();
//	xmlTemp.OutOfElem();

	sXmlDoc = xmlTemp.GetDoc();

	return sXmlDoc;
}
CString CGmes::MakeElem_S2F4(CString sEquipmentID)
{
	CString sXmlDoc;
	CMarkup xmlTemp;

//	<EIF VERSION="1.00" ID="S2F4" NAME="Link Test Response">
//		<ELEMENT>
//			<EQPID>"Equipment ID"</EQPID>
//		</ELEMENT>
//	</EIF>

	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S2F4");
	xmlTemp.AddAttrib("NAME", "Link Test Response");
	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("EQPID", sEquipmentID);
	xmlTemp.OutOfElem();

	sXmlDoc = xmlTemp.GetDoc();

	return sXmlDoc;
}

CString CGmes::MakeElem_S2F32(CString sEquipmentID, int nFlagD)
{
	CString sXmlDoc;
	CMarkup xmlTemp;
	CString sAckFlag;

//	<EIF VERSION="1.00" ID="S2F32" NAME="Data and Time Acknowledge">
//		<ELEMENT>
//			<EQPID>GLZINS002</EQPID>
//		</ELEMENT>
//		<ITEM>
//			<ACK>0</ACK><!--0=Accepted/1=Not Accepted-->
//		</ITEM>	
//	</EIF>

	sAckFlag.Format("%d", nFlagD);

	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S2F32");
	xmlTemp.AddAttrib("NAME", "Data and Time Acknowledge");
	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("EQPID", sEquipmentID);
	xmlTemp.OutOfElem();
	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("ACK", sAckFlag);
	xmlTemp.OutOfElem();


	sXmlDoc = xmlTemp.GetDoc();

	return sXmlDoc;
}

CString CGmes::MakeElem_S6F11(CString sEquipmentID, CString sPcbID)
{
	CString sXmlDoc;
	CMarkup xmlTemp;

	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S6F11");
	xmlTemp.AddAttrib("NAME", "Event Report Send");
	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("EQPID", sEquipmentID);
	xmlTemp.AddChildElem("CEID", "40600");
	xmlTemp.AddChildElem("RPTID", "40600");
	xmlTemp.OutOfElem();
	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("SUBITEMLIST");
	xmlTemp.AddChildAttrib("COUNT", "1");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("NAME", "PCBAID");
	xmlTemp.AddChildElem("VALUE", sPcbID);
	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();

	sXmlDoc = xmlTemp.GetDoc();

	return sXmlDoc;
}

CString CGmes::MakeElem_S6F11_1(CString sEquipmentID, CString sPcbID, BOOL bModelSuffix)
{
	CString sXmlDoc;
	CMarkup xmlTemp;

//	<EIF VERSION="1.00" ID="S6F11" NAME="Event Report Send">
//	  <ELEMENT>
//		<EQPID>GLZINS004</EQPID>
//		<CEID>20100</CEID>
//		<RPTID>20101</RPTID>
//	  </ELEMENT>
//	  <ITEM>
//		<SUBITEMLIST COUNT="1">
//		  <NAME>SETID</NAME>
//		  <VALUE>TS372101FL</VALUE>
//		</SUBITEMLIST>
//	  </ITEM>
//	</EIF>

	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S6F11");
	xmlTemp.AddAttrib("NAME", "Event Report Send");
	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("EQPID", sEquipmentID);
	if(bModelSuffix){
		xmlTemp.AddChildElem("CEID", "40200");
		xmlTemp.AddChildElem("RPTID", "40200");
	}
	else{
		xmlTemp.AddChildElem("CEID", "40100");
		xmlTemp.AddChildElem("RPTID", "40100");
	}
	xmlTemp.OutOfElem();
	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("SUBITEMLIST");
	xmlTemp.AddChildAttrib("COUNT", "1");
	xmlTemp.IntoElem();
	if(bModelSuffix){
		xmlTemp.AddChildElem("NAME", "MODEL_SUFFIX");
	}
	else{
		xmlTemp.AddChildElem("NAME", "PCBAID");
	}

	xmlTemp.AddChildElem("VALUE", sPcbID);
	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();


	sXmlDoc = xmlTemp.GetDoc();

	return sXmlDoc;
}

CString CGmes::MakeElem_S6F11_2(CString sEquipmentID, CString sPcbID)
{
	CString sXmlDoc;
	CMarkup xmlTemp;
	
	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S6F11");
	xmlTemp.AddAttrib("NAME", "Event Report Send");
	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("EQPID", sEquipmentID);

	if (CurrentSet->bNgCountCheck) {
		xmlTemp.AddChildElem("CEID", "40600");
		xmlTemp.AddChildElem("RPTID", "40600");
	}
	else {
		xmlTemp.AddChildElem("CEID", "41600");
		xmlTemp.AddChildElem("RPTID", "41600");
	}
	xmlTemp.OutOfElem();
	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("SUBITEMLIST");
	xmlTemp.AddChildAttrib("COUNT", "1");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("NAME", "PCBAID");
	xmlTemp.AddChildElem("VALUE", sPcbID);
	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();
	
	
	sXmlDoc = xmlTemp.GetDoc();
	
	return sXmlDoc;
}
CString CGmes::MakeElem_S6F11_3(CString sEquipmentID, CString sPcbID)
{
	CString sXmlDoc;
	CMarkup xmlTemp;

	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.7");
	xmlTemp.AddAttrib("ID", "S6F11");
	xmlTemp.AddAttrib("NAME", "Event Report Send");
	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("EQPID", sEquipmentID);
	
	xmlTemp.AddChildElem("CEID", "40104");
	xmlTemp.AddChildElem("RPTID", "40104");

	xmlTemp.OutOfElem();
	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("SUBITEMLIST");
	xmlTemp.AddChildAttrib("COUNT", "2");
	xmlTemp.IntoElem();

	xmlTemp.AddChildElem("NAME", "SETID");
	xmlTemp.AddChildElem("VALUE", sPcbID);

	xmlTemp.AddChildElem("NAME", "DEFECT_CODE");
	xmlTemp.AddChildElem("VALUE", "0W1F7C");

	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();


	sXmlDoc = xmlTemp.GetDoc();

	return sXmlDoc;
}


BOOL CGmes::Paser_ReceiveData(CString sReceiveData)
{
	CMarkup xmlTemp;
	BOOL bRev = TRUE;
	CString tempId;
	CString sFindElem;
	CString sXmlData;
	CString sSetID;

	xmlTemp.SetDoc(sReceiveData);
	xmlTemp.ResetPos();

	while (xmlTemp.FindElem("EIF")) {
		tempId = xmlTemp.GetAttrib("ID");
		sFindElem = xmlTemp.GetSubDoc();
		if (tempId == "S2F3") {
			if(Paser_S2F3(sFindElem))
			{
				sXmlData = MakeElem_S2F4(CurrentSet->sEquipmentID);
				if(SendCommString(sXmlData))
				{
					DisplyComStatus("S2F4", FALSE);
				}
			}
		}
		else if(tempId == "S1F2") {
			Paser_S1F2(sFindElem);
		}
		else if(tempId == "S2F31") {
			if(Paser_S2F31(sFindElem)){
				sXmlData = MakeElem_S2F32(CurrentSet->sEquipmentID, 0);
			}
			else{
				sXmlData = MakeElem_S2F32(CurrentSet->sEquipmentID, 1);
			}

			if(SendCommString(sXmlData))
			{
				DisplyComStatus("S2F31", FALSE);
			}
		}
		else if(tempId == "S6F12") {
			Paser_S6F12(sFindElem);
		}
		else if(tempId == "S6F5") {
			if(Paser_S6F5(sFindElem, sSetID)){
				sXmlData = MakeElem_S6F6(CurrentSet->sEquipmentID, sSetID, 0);
				g_pView->Display_GMESInfo(TRUE);
			}
			else{
				bRev = FALSE;
				sXmlData = MakeElem_S6F6(CurrentSet->sEquipmentID, sSetID, 1);
			}

			if(SendCommString(sXmlData))
			{
				DisplyComStatus("S6F5", FALSE);
			}
		}
		else{
			bRev = FALSE;
		}

	}

	return bRev;
}

BOOL CGmes::Paser_S2F3(CString sReceiveData)
{
	CMarkup xmlTemp;
	BOOL bRev = FALSE;
	CString tempId;

	xmlTemp.SetDoc(sReceiveData);
	xmlTemp.ResetPos();

	while (xmlTemp.FindElem("EIF")) {
		tempId = xmlTemp.GetAttrib("ID");
		if (tempId == "S2F3") {
			bRev = TRUE;
			DisplyComStatus("S2F3", TRUE);
		}

	}

	return bRev;
}

BOOL CGmes::Paser_S1F2(CString sReceiveData)
{
	CMarkup xmlTemp;
	BOOL bRev = TRUE;

	xmlTemp.SetDoc(sReceiveData);
	xmlTemp.ResetPos();

	xmlTemp.FindElem("EIF");
	xmlTemp.IntoElem();
	xmlTemp.FindElem("ELEMENT");
//	xmlTemp.IntoElem();
//	xmlTemp.FindElem();
	if(xmlTemp.FindChildElem("EQPID")) {
		m_sEquipmentID = xmlTemp.GetChildData();
		CurrentSet->sEquipmentID = m_sEquipmentID;
	}   

//	xmlTemp.OutOfElem();						//현재의 노드에서 상위노드로 이동한다.
	xmlTemp.OutOfElem();

	DisplyComStatus("S1F2", TRUE);

	return bRev;
}

BOOL CGmes::Paser_S2F31(CString sReceiveData)
{
	CMarkup xmlTemp;
	CString sTemp;
	CString sEQPID = "";
	CString sTIME = "";
	BOOL bRev = FALSE;

	xmlTemp.SetDoc(sReceiveData);
	xmlTemp.ResetPos();

	xmlTemp.FindElem("EIF");
	xmlTemp.IntoElem();

	xmlTemp.FindElem("ELEMENT");
	if(xmlTemp.FindChildElem("EQPID")) {
		sEQPID = xmlTemp.GetChildData();
	}   

	xmlTemp.FindElem("ITEM");
//	xmlTemp.IntoElem();
//	xmlTemp.FindElem();
	if( xmlTemp.FindChildElem("TIME")) { 
		sTIME = xmlTemp.GetChildData();
		ChangeSystemTime(sTIME);
		bRev = TRUE;
	}   

//	xmlTemp.OutOfElem();						//현재의 노드에서 상위노드로 이동한다.
	xmlTemp.OutOfElem();
	DisplyComStatus("S2F31", TRUE);

	if(bRev){
		sTemp.Format("EQPID = %s, TIME = %s", sEQPID, sTIME);
		AddStringToStatus(sTemp);
	}

	return bRev;
}

BOOL CGmes::Paser_S6F12(CString sReceiveData)
{
	CMarkup xmlTemp;
	CString sTemp;
	CString sEQPID = "";
	CString sCEID = "";
	CString sRPTID = "";
	int		nAck = -1;

	m_sErrorMsg = _T("");

	xmlTemp.SetDoc(sReceiveData);
	xmlTemp.ResetPos();

	xmlTemp.FindElem("EIF");
	xmlTemp.IntoElem();
	xmlTemp.FindElem("ELEMENT");
//	xmlTemp.IntoElem();
	if(xmlTemp.FindChildElem("EQPID")) { sEQPID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("CEID")) { sCEID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("RPTID")) { sRPTID = xmlTemp.GetChildData(); }   
//	xmlTemp.OutOfElem();						//현재의 노드에서 상위노드로 이동한다.
	
	xmlTemp.FindElem("ITEM");
//	xmlTemp.IntoElem();
	if(xmlTemp.FindChildElem("ACK")) { 
		sTemp = xmlTemp.GetChildData();
		nAck = atoi(sTemp);
	}
//	xmlTemp.OutOfElem();						
	
	if(nAck == 0){
		DisplyComStatus("S6F12:ACK-OK", TRUE);
	}
	else{
		if(xmlTemp.FindChildElem("REASON")) { 
			m_sErrorMsg = xmlTemp.GetChildData();
		//	AfxMessageBox(m_sErrorMsg);
		}
		DisplyComStatus("S6F12:ACK-NG => " + m_sErrorMsg, TRUE);
		return FALSE;
	}

	return TRUE;
}

BOOL CGmes::Paser_S6F5(CString sReceiveData, CString& sSetId)
{
	BOOL bRev = TRUE;
	CMarkup xmlTemp;
	CString sTemp;
	CString sEQPID = "";
	CString sSetId_Temp = "";
//	CString sRPTID = "";
	int		nAck = -1;
	int nCount = 0;
	CString sName;
	CString sValue;

	xmlTemp.SetDoc(sReceiveData);
	xmlTemp.ResetPos();

	xmlTemp.FindElem("EIF");
	xmlTemp.IntoElem();
	xmlTemp.FindElem("ELEMENT");
//	xmlTemp.IntoElem();

	if(xmlTemp.FindChildElem("TID")) { 
		m_sTransactionID = xmlTemp.GetChildData();
		CurrentSet->sTransactionID = m_sTransactionID;
	}   
	if(xmlTemp.FindChildElem("WOID")) { 
		m_sWorkOrderID = xmlTemp.GetChildData();
		CurrentSet->sWorkOrderID = m_sWorkOrderID;
	}   
	else {bRev = FALSE;}

	if(xmlTemp.FindChildElem("ORGID")) { 
		m_sOrganizationID = xmlTemp.GetChildData();
		m_nOrganizationID = atoi(m_sOrganizationID);
		CurrentSet->sOrganizationID = m_sOrganizationID;
	}
	else {bRev = FALSE;}
	
	if(xmlTemp.FindChildElem("LINEID")) { 
		m_sLineID = xmlTemp.GetChildData(); 
		CurrentSet->sLineID = m_sLineID;
	}   
	else {bRev = FALSE;}

	if(xmlTemp.FindChildElem("EQPID")) { sEQPID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("PROCID")) { 
		m_sOperationID = xmlTemp.GetChildData(); 
		CurrentSet->sOperationID = m_sOperationID;
	}   
	else {bRev = FALSE;}

	if(xmlTemp.FindChildElem("SETID")) { 
		sSetId_Temp = xmlTemp.GetChildData();    
		sSetId = sSetId_Temp;
	}
	if(xmlTemp.FindChildElem("MODEL")){
		xmlTemp.IntoElem();
		if(xmlTemp.FindChildElem("ID")) { 
			m_sModelID = xmlTemp.GetChildData(); 
			CurrentSet->sModelID = m_sModelID;
		}   
		if(xmlTemp.FindChildElem("NAME")) { 
			m_sModelName = xmlTemp.GetChildData(); 
			CurrentSet->sModelName_GMES = m_sModelName;
		}  
		if(xmlTemp.FindChildElem("SUFFIX")) { 
			m_sModelSuffixName = xmlTemp.GetChildData(); 
			CurrentSet->sModelSuffix = m_sModelSuffixName;
		}   
		xmlTemp.OutOfElem();
	//	xmlTemp.OutOfElem();

		sTemp.Format(_T("%s.%s"), CurrentSet->sModelID, CurrentSet->sModelSuffix);
		CurrentSet->sModelSuffixName = sTemp;
	}
	else {bRev = FALSE;}

	xmlTemp.FindElem("ITEM");
	xmlTemp.IntoElem();
	if(xmlTemp.FindElem("SUBITEMLIST")){ 
		sTemp = xmlTemp.GetAttrib("COUNT");
		nCount = atoi(sTemp);
		bRev = FALSE;
		for(int i =0; i< nCount; i++){
			sName = _T("");
			sValue = _T("");
			if(xmlTemp.FindChildElem("NAME")) { sName = xmlTemp.GetChildData(); }   
			if(xmlTemp.FindChildElem("VALUE")) { sValue = xmlTemp.GetChildData(); }
//			if((sName.Compare("COMPONENT_REMARK") == 0) || (sName.Compare("COMPONENT_REMARKS") == 0))
			if((sName == "COMPONENT_REMARK") || (sName == "COMPONENT_REMARKS"))
			{
				sValue.MakeUpper();
				if(Get_ToolOption(sValue)) { 
					bRev = TRUE;
				}
				g_pView->UpdateToolOptionInfo(TRUE);
			//	UpDateInfo_ToolOption();
			}
			else if(sName == "DFT_SEQ")
			{
				m_nDftSeq = atoi(sValue);
				if(m_nDftSeq < 0){
					bRev = FALSE;
				}
				else{
					CurrentSet->nDftSeq = m_nDftSeq;
				}
			}
			else if(sName == "TOOL_OPTION1"){CurrentSet->sToolOption1 = sValue;}
			else if(sName == "TOOL_OPTION2"){CurrentSet->sToolOption2 = sValue;}
			else if(sName == "TOOL_OPTION3"){CurrentSet->sToolOption3 = sValue;}
			else if(sName == "TOOL_OPTION4"){CurrentSet->sToolOption4 = sValue;}
			else if(sName == "TOOL_OPTION5"){CurrentSet->sToolOption5 = sValue;}
			else if(sName == "TOOL_OPTION6"){CurrentSet->sToolOption6 = sValue;}
			else if(sName == "TOOL_OPTION7"){CurrentSet->sToolOption7 = sValue;}
			else if(sName == "TOOL_OPTION8"){CurrentSet->sCommercialOption1 = sValue;}
			else if(sName == "TOOL_OPTION9"){CurrentSet->sToolOption8 = sValue;}
			else if(sName == "TOOLCRC"){CurrentSet->sToolCRC = sValue;}
			else if(sName == "AREA_CODE"){	CurrentSet->sAreaOption1 = sValue;}
			else if(sName == "COUNTRY_CODE"){CurrentSet->sCountryGroup = sValue;}
			else if(sName == "COUNTRY"){CurrentSet->sCountry = sValue;}
			else if(sName == "SW_VERSION"){CurrentSet->sCPUVersion = sValue;}
			else if(sName == "MICOM_SW_VERSION"){CurrentSet->sMicomVersion = sValue;}
			else if(sName == "EDID_HDMI1"){CurrentSet->sEDID_HDMI1 = sValue;}
			else if(sName == "EDID_HDMI2"){CurrentSet->sEDID_HDMI2 = sValue;}
			else if(sName == "EDID_HDMI3"){CurrentSet->sEDID_HDMI3 = sValue;}
			else if(sName == "EDID_HDMI4"){CurrentSet->sEDID_HDMI4 = sValue;}
			else if(sName == "EDID_HDMI5"){CurrentSet->sEDID_HDMI5 = sValue;}
			else if(sName == "EDID_DP"){CurrentSet->sEDID_DP = sValue;}
			else if(sName == "EDID_DP2"){CurrentSet->sEDID_DP2 = sValue;}
			else if(sName == "EDID_DVI"){CurrentSet->sEDID_DVI = sValue;}
			else if(sName == "EDID_PC"){CurrentSet->sEDID_PC = sValue;}
			else if(sName == "EDID_RGB"){CurrentSet->sEDID_RGB = sValue;}
			else if(sName == "CI"){CurrentSet->sCI = sValue;}
			else if(sName == "CNTV"){CurrentSet->sCNTV = sValue;}
			else if(sName == "DTCP_RX"){CurrentSet->sDTCP_RX = sValue;}
			else if(sName == "DVBS"){CurrentSet->sDVBS = sValue;}
			else if(sName == "DVBS2"){CurrentSet->sDVBS2 = sValue;}
			else if(sName == "ESN"){CurrentSet->sESN = sValue;}
			else if(sName == "HDCP_RX"){CurrentSet->sHDCP_RX = sValue;}
			else if(sName == "HDCP_RX_14"){CurrentSet->sHDCP_RX_14 = sValue;}
			else if (sName == "HDCP_TX") { CurrentSet->sHDCP_TX = sValue; }
			else if(sName == "JAPAN_LNB"){CurrentSet->sJAPAN_LNB = sValue;}
			else if(sName == "MAC_ADDRESS"){CurrentSet->sMAC_ADDRESS = sValue;}
			else if(sName == "MARINKEY"){CurrentSet->sMARINKEY = sValue;}
			else if(sName == "MODEL_NAME"){CurrentSet->sMODEL_NAME = sValue;}
			else if(sName == "SERIAL_NO"){CurrentSet->sSERIAL_NO = sValue;}
			else if(sName == "WASU"){CurrentSet->sWASU = sValue;}
			else if(sName == "WIDEVINE"){CurrentSet->sWIDEVINE = sValue;}
			else if(sName == "ADC"){CurrentSet->sADC = sValue;}
			else if(sName == "BUILT_WIFI"){CurrentSet->sBUILT_WIFI = sValue;}
			else if(sName == "CA"){CurrentSet->sCA = sValue;}
			else if(sName == "TOTAL_SUM"){CurrentSet->nDftSeq = atoi(sValue);}
			else if(sName == "OK_SUM"){CurrentSet->nDftOkCount = atoi(sValue);}
			else if (sName == "NG_SUM") { CurrentSet->nDftNgCount = atoi(sValue); }
			else if (sName == "ATSC3.0") { CurrentSet->sASTC30_Key = sValue; }
			else if (sName == "MFi") { CurrentSet->sMfi_Key = sValue; }
			else if (sName == "CI+ECP") { CurrentSet->sCI_CEP_Key = sValue; }
			/*else if (sName == "NG_SUM") { CurrentSet->sUHDCP_Key = atoi(sValue); }
			else if (sName == "NG_SUM") { CurrentSet->nDftNgCount = atoi(sValue); }*/
		}
		if(nCount >= 40){bRev = TRUE;}
		if(nCount == 3){bRev = TRUE;}
		if (nCount == 0) { bRev = TRUE; }

	}

	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();

	if(bRev){
		DisplyComStatus("S6F5 : OK", TRUE);
	}
	else{
		DisplyComStatus("S6F5 : NG", TRUE);
	}

	if(nCount >= 40){
		CurrentSet->bDrmKey = ChekDrmKey();
	}
	if (nCount == 3) {
		sTemp.Format("%s : Total_sum - %d, OK_sum - %d, NG_sum - %d", sSetId_Temp, CurrentSet->nDftSeq,CurrentSet->nDftOkCount, CurrentSet->nDftNgCount);
		AddStringToStatus(sTemp);
	}

	return bRev;
}

CString CGmes::MakeElem_S6F6(CString sEquipmentID, CString sPcbID, int nFlag)
{
	CString sXmlDoc;
	CMarkup xmlTemp;
	CString sAckFlag;

//	<EIF VERSION="1.00" ID="S6F6" NAME="Product Data Acknowledge">
//		<ELEMENT>
//			<TID>"Transaction ID - Mobile Only"</TID>
//			<EQPID>"Equipment ID"</EQPID>
//			<PROCID>"Operation ID"</PROCID>
//			<SETID>"SET ID"</SETID>
//			<MODEL>
//				<ID>"ID"</ID>
//				<NAME>"Inspection(Processing) Data Name"</NAME>
//				<SUFFIX>"SUFFIX"</SUFFIX>
//			</MODEL>
//		</ELEMENT>
//		<ITEM>
//			<ACK>"Acknowledge Code"</ACK><!--0=Accepted/1=Not Accepted/2-63=Reserved-->
//		</ITEM>
//	</EIF>

	sAckFlag.Format("%d", nFlag);

	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S6F6");
	xmlTemp.AddAttrib("NAME", "Product Data Acknowledge");
	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("TID", CurrentSet->sTransactionID);
	xmlTemp.AddChildElem("WOID", CurrentSet->sWorkOrderID);
	xmlTemp.AddChildElem("ORGID", CurrentSet->sOrganizationID);
	xmlTemp.AddChildElem("LINEID", CurrentSet->sLineID);
	xmlTemp.AddChildElem("EQPID", CurrentSet->sEquipmentID);
	xmlTemp.AddChildElem("PROCID", CurrentSet->sOperationID);
	xmlTemp.AddChildElem("SETID", sPcbID);
	xmlTemp.AddChildElem("MODEL");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("ID", CurrentSet->sModelID);
	xmlTemp.AddChildElem("NAME", CurrentSet->sModelName_GMES);
	xmlTemp.AddChildElem("SUFFIX", CurrentSet->sModelSuffix);
	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();

	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("ACK", sAckFlag);
	xmlTemp.OutOfElem();


	sXmlDoc = xmlTemp.GetDoc();

	return sXmlDoc;
}

CString CGmes::MakeElem_S6F1(CString sEquipmentID, CString sPcbID, BOOL bResult, CString sTotalTime, BOOL bFailOnly )
{
	CString sXmlDoc;
	CMarkup xmlTemp;
//	CString sResult		= _T("");
//	CString sTotalTime	= _T("");
	int		nSeqNo		= 1;
	CString sSeqNo		= _T("");
	int		nItemCount	= 0;
	CString sItemCount	= _T("");
	CString sSystemNo	= _T("");
	int		nSPEC_SEQ	= 0;
	CString sSPEC_SEQ	= _T("");

	CString sStepNo			= _T("");
	CString sRun			= _T("");
	CString sResult			= _T("");
	CString sAudioTarget	= _T("");
	CString sAudioMeasure	= _T("");
	CString sMeasured		= _T("");
	CString sNominal		= _T("");
	CString sLowLimit		= _T("");
	CString sHighLimit		= _T("");
	CString sElapsedTime	= _T("");
	CString sMsg	= _T("");
	CString sKeyDL_Log = _T("");

	nSeqNo = CurrentSet->nDftSeq + 1;
	sSeqNo.Format("%d", nSeqNo);

	if(CurrentSet->bDrmKey){
		sKeyDL_Log.Format("KEY%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
	}
	else{	
		sKeyDL_Log.Format("DFT%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
	}

	if(bResult){
		nItemCount = 6;
	}
	else{
		nItemCount = 6 + (FindResult2() * 12);
	}
	sItemCount.Format("%d", nItemCount);

	if(bResult == TRUE){sResult = "OK";}
	else{sResult = "NG";}
//	sTotalTime.Format("%.1f", fTime);

	sSystemNo.Format("%d", CurrentSet->nSystemNo);

	xmlTemp.AddElem("EIF");
	xmlTemp.AddAttrib("VERSION", "1.4");
	xmlTemp.AddAttrib("ID", "S6F1");
	xmlTemp.AddAttrib("NAME", "Inspection(Processing) Data Send");

	xmlTemp.AddChildElem("ELEMENT");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("TID", CurrentSet->sTransactionID);
	xmlTemp.AddChildElem("WOID", CurrentSet->sWorkOrderID);
	xmlTemp.AddChildElem("ORGID", CurrentSet->sOrganizationID); //"205416"
	xmlTemp.AddChildElem("LINEID", CurrentSet->sLineID); //"1333058"
	xmlTemp.AddChildElem("EQPID", CurrentSet->sEquipmentID);
	xmlTemp.AddChildElem("PROCID", CurrentSet->sOperationID);
	xmlTemp.AddChildElem("SETID", sPcbID);

	xmlTemp.AddChildElem("MODEL");
	xmlTemp.IntoElem();
	if(CurrentSet->nToolOptionSetMethod == 2){
		xmlTemp.AddChildElem("ID", "");
		xmlTemp.AddChildElem("NAME", "");
		xmlTemp.AddChildElem("SUFFIX", "");
	}
	else{
		xmlTemp.AddChildElem("ID", CurrentSet->sModelID);
		xmlTemp.AddChildElem("NAME", CurrentSet->sModelName_GMES);
		xmlTemp.AddChildElem("SUFFIX", CurrentSet->sModelSuffix);
	}
	xmlTemp.OutOfElem();

	xmlTemp.OutOfElem();

	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("SUBITEMLIST");
	xmlTemp.AddChildAttrib("COUNT", sItemCount);
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("NAME", "RESULT");  //5개 
	xmlTemp.AddChildElem("VALUE", sResult);

	xmlTemp.AddChildElem("NAME", "KEYCHECK_LOG");
	xmlTemp.AddChildElem("VALUE", sKeyDL_Log);

	xmlTemp.AddChildElem("NAME", "SEQ");
	xmlTemp.AddChildElem("VALUE", sSeqNo);
	xmlTemp.AddChildElem("NAME", "MACHINE_NO");
	xmlTemp.AddChildElem("VALUE", sSystemNo);
	xmlTemp.AddChildElem("NAME", "TOTAL_TIME");
	xmlTemp.AddChildElem("VALUE", sTotalTime);
	xmlTemp.AddChildElem("NAME", "CHASSIS");
	xmlTemp.AddChildElem("VALUE", CurrentSet->sChassisName);
	xmlTemp.AddChildElem("NAME", "A_CAS_ID");
	xmlTemp.AddChildElem("VALUE", g_pView->m_A_CAS_ID_Check);

	xmlTemp.AddChildElem("NAME", "HDMI_GEN_VER");
	xmlTemp.AddChildElem("VALUE", HDMIGeneratorCtrl.m_FW_Ver);// 

	if(!bResult){
		POSITION Position = StepList.GetHeadPosition();
		while (Position) 
		{
			CStep* pStep = StepList.GetNext(Position);

			if(pStep->m_sName == _T(""))  pStep->m_sName  = _T("-");
			if(pStep->m_strMsg == _T("")) pStep->m_strMsg = _T("-");
			sAudioTarget	= _T("-");
			sAudioMeasure	= _T("-");
			sMeasured		= _T("-");
			sNominal		= _T("-");
			sLowLimit		= _T("-");
			sHighLimit		= _T("-");
			sElapsedTime	= _T("-");
			sResult			= _T("-");

			if(pStep->m_bTest){
				sRun = _T("1");
				if (pStep->m_bResult){ sResult = _T("OK"); }
				else{ 
					sResult = _T("NG");
					if(pStep->m_bRunAudioTest && pStep->m_bRunVideoTest){
						if(pStep->m_bVideoTestResult == FALSE){ sResult = _T("NG_V");}
						if(pStep->m_bAudioTestResult == FALSE){	sResult = _T("NG_A"); }

						if((pStep->m_bVideoTestResult == FALSE) && (pStep->m_bAudioTestResult == FALSE)) {
							sResult = _T("NG_AV");
						}
					}
					else if(!pStep->m_bRunAudioTest && pStep->m_bRunVideoTest){
						if(pStep->m_bVideoTestResult == FALSE){	sResult = _T("NG_V"); }
					}
					else if(pStep->m_bRunAudioTest && !pStep->m_bRunVideoTest){
						if(pStep->m_bAudioTestResult == FALSE){	sResult = _T("NG_A"); }
					}
					else{
						sResult = _T("NG");
					}
				}
				sElapsedTime.Format(_T("%.1f"), pStep->m_fElapsedTime);
			}
			else{ sResult = _T("SKIP"); sRun = _T("0");}

			if(bFailOnly){
				if(pStep->m_bResult || !pStep->m_bTest) continue;
			}

			if(pStep->m_bRunAudioTest)
			{
				if(pStep->m_nMeasureAudioType == LEVEL_CHECK){
					sAudioTarget.Format(_T("X, X, %d, %d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				else if(pStep->m_nMeasureAudioType == FREQUENCY_CHECK){
					sAudioTarget.Format(_T("%d, %d, X, X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
				}
				else{
					sAudioTarget.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				// Audio Measure
				sAudioMeasure.Format(_T("%d,%d,%d,%d"), pStep->m_nLeftFreq[1], pStep->m_nRightFreq[1], pStep->m_nLeftLevel[1], pStep->m_nRightLevel[1]);
			}

			if(pStep->m_bAdcValueCheckStep)
			{
				sAudioMeasure = pStep->m_szAdcValue;
			}
			if(pStep->m_nFuncType == MEAS_DOUBLE)
			{
				sMeasured.Format(_T("%.1f"), pStep->m_fMeasured);
				sNominal.Format(_T("%.1f"), pStep->m_fNominal);
				sLowLimit.Format(_T("%.1f"), pStep->m_fLowLimit);
				sHighLimit.Format(_T("%.1f"), pStep->m_fHighLimit);

			}
			sStepNo.Format("%d", pStep->m_nStep);

			nSPEC_SEQ ++;
			sSPEC_SEQ.Format("%d", nSPEC_SEQ);

			xmlTemp.AddChildElem("NAME", "SPEC_SEQ");  //12개
			xmlTemp.AddChildElem("VALUE", sSPEC_SEQ);

			xmlTemp.AddChildElem("NAME", "RUN");
			xmlTemp.AddChildElem("VALUE", sRun);

			xmlTemp.AddChildElem("NAME", "STEP_NO");
			xmlTemp.AddChildElem("VALUE", sStepNo);

			xmlTemp.AddChildElem("NAME", "STEP_NAME");
			xmlTemp.AddChildElem("VALUE", pStep->m_sName);
			xmlTemp.IntoElem();
			xmlTemp.AddChildElem("JUDGE", sResult);
			xmlTemp.OutOfElem();

			xmlTemp.AddChildElem("NAME", "MEASURE");
			xmlTemp.AddChildElem("VALUE", sMeasured);

			xmlTemp.AddChildElem("NAME", "TARGET");
			xmlTemp.AddChildElem("VALUE", sNominal);

			xmlTemp.AddChildElem("NAME", "LOWER_LIMIT");
			xmlTemp.AddChildElem("VALUE", sLowLimit);

			xmlTemp.AddChildElem("NAME", "UPPER_LIMIT");
			xmlTemp.AddChildElem("VALUE", sHighLimit);

			xmlTemp.AddChildElem("NAME", "A_TARGET");
			xmlTemp.AddChildElem("VALUE", sAudioTarget);

			xmlTemp.AddChildElem("NAME", "A_MEASURE");
			xmlTemp.AddChildElem("VALUE", sAudioMeasure);

			xmlTemp.AddChildElem("NAME", "TIME");
			xmlTemp.AddChildElem("VALUE", sElapsedTime);

			sMsg.Format(_T("%s"), pStep->m_strMsg);
			xmlTemp.AddChildElem("NAME", "MESSAGE");
			xmlTemp.AddChildElem("VALUE", sMsg);
		}
	}


	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();



	sXmlDoc = xmlTemp.GetDoc();



	return sXmlDoc;
}

BOOL CGmes::Paser_S6F2(CString sReceiveData)
{
	CMarkup xmlTemp;
	CString sTemp;
	CString sWOID = "";
	CString sORGID = "";
	CString sLINEID = "";
	CString sEQPID = "";
	CString sPROCID = "";
	CString sSETID = "";
	int		nAck = -1;

	m_sErrorMsg = _T("");

	xmlTemp.SetDoc(sReceiveData);
	xmlTemp.ResetPos();

	xmlTemp.FindElem("EIF");
	xmlTemp.IntoElem();
	xmlTemp.FindElem("ELEMENT");
	if(xmlTemp.FindChildElem("WOID")) { sWOID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("ORGID")) { sORGID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("LINEID")) { sLINEID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("EQPID")) { sEQPID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("PROCID")) { sPROCID = xmlTemp.GetChildData(); }   
	if(xmlTemp.FindChildElem("SETID")) { sSETID = xmlTemp.GetChildData(); }   
	
	xmlTemp.FindElem("ITEM");
	if(xmlTemp.FindChildElem("ACK")) { 
		sTemp = xmlTemp.GetChildData();
		nAck = atoi(sTemp);
	}

	if(nAck == 0){
		DisplyComStatus("S6F2:ACK-OK", TRUE);
	}
	else{
		if(xmlTemp.FindChildElem("REASON")) { 
			m_sErrorMsg = xmlTemp.GetChildData();
		//	AfxMessageBox(m_sErrorMsg);
		}
		DisplyComStatus("S6F2:ACK-NG => " + m_sErrorMsg, TRUE);
		xmlTemp.OutOfElem();
		return FALSE;
	}
	xmlTemp.OutOfElem();

	return TRUE;
}
