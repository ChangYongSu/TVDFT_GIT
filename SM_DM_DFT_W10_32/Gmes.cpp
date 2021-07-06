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
	m_sSetID = _T("");
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

	g_pView->m_pSocketClient.Create();										//클라이언트 소켓 생성
	connt = g_pView->m_pSocketClient.Connect(sIpAdd, nPort);					//서버에 연결을 시도 한다.

	if(connt){												//오류 발생시
		AddStringToStatus("GMES Connection OK.");
		m_bGmesCom = TRUE;
	}
	else{
		g_pView->m_pSocketClient.Close();//소켓을 종료한다.
		AddStringToStatus("GMES Connection error!");
		AfxMessageBox("GMES Connection error!");
		m_bGmesCom = FALSE;
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

//	if(!m_bGmesCom) {return FALSE;}

//	m_sReceive = _T("");
	nSendLenth = strData.GetLength();
//	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);

	nRtn = g_pView->m_pSocketClient.Send(strData, nSendLenth); //데이타 전송 부분

	if(nRtn != nSendLenth) return FALSE;

	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);
	return TRUE;
}

BOOL CGmes::ReceiveCommString(int nWaitLimit, CString& sReadString)
{
	char	ReadBuffer[2048];	
	CString strRevText;
	DWORD		dwRead;		 

	m_sReceive = _T("");
	memset(ReadBuffer, 0, 2048);


	DWORD dwStartTick = GetTickCount();

	while (TRUE)
	{
		dwRead = g_pView->m_pSocketClient.Receive(ReadBuffer, 2048);

		if (dwRead >= 0) 
		{
			m_sReceive = ReadBuffer;
			sReadString = ReadBuffer;
			return TRUE;
		}

		if((GetTickCount() - dwStartTick) > nWaitLimit)		
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
	
	if(CurrentSet->nProcessType == 2){
		xmlTemp.AddChildElem("CEID", "41800");
		xmlTemp.AddChildElem("RPTID", "41800");
	}
	else if(CurrentSet->nProcessType == 3){
		xmlTemp.AddChildElem("CEID", "20100");
		xmlTemp.AddChildElem("RPTID", "20100");
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
	if((CurrentSet->nProcessType == 2) || (CurrentSet->nProcessType == 3))
	{
		xmlTemp.AddChildElem("NAME", "SETID");
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

CString CGmes::MakeElem_S6F11_2(CString sEquipmentID, CString sPcbID, CString sMac_Add)
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
	
	xmlTemp.AddChildElem("CEID", "40106");
	xmlTemp.AddChildElem("RPTID", "40106");

	xmlTemp.OutOfElem();
	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("SUBITEMLIST");
	xmlTemp.AddChildAttrib("COUNT", "2");
	xmlTemp.IntoElem();

	xmlTemp.AddChildElem("NAME", "PCBAID");
	xmlTemp.AddChildElem("VALUE", sPcbID);

	xmlTemp.AddChildElem("NAME", "MAC_ADDRESS");
	xmlTemp.AddChildElem("VALUE", sMac_Add);

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

////////////////////////////////////////////////////////////////////////////
		//CurrentSet->ModelList
		if (CurrentSet->bAutoGMES_ChangeModel == 1)
		{
			if (CurrentSet->sModelName.Find(CurrentSet->sModelSuffixName) >= 0)
			{
			}
			else
			{
				g_pView->SendMessage(WM_CHANGE_MODEL, 0, 0);
				//ChangeModel(CurrentSet->sModelSuffixName);
			}
		}
///////////////////////////////////////////////////////////////
	}
	else {bRev = FALSE;}

	xmlTemp.FindElem("ITEM");
	xmlTemp.IntoElem();
	if(xmlTemp.FindElem("SUBITEMLIST")){ 
		sTemp = xmlTemp.GetAttrib("COUNT");
		nCount = atoi(sTemp);
		m_nDftSeq = 0;
	//	bRev = FALSE;
		for(int i =0; i< nCount; i++){
			sName = _T("");
			sValue = _T("");
			if(xmlTemp.FindChildElem("NAME")) { sName = xmlTemp.GetChildData(); }   
			if(xmlTemp.FindChildElem("VALUE")) { sValue = xmlTemp.GetChildData(); }
			/*if((sName.Compare("COMPONENT_REMARK") == 0) || (sName.Compare("COMPONENT_REMARKS") == 0))
			{
				sValue.MakeUpper();
				if(Get_ToolOption(sValue)) { 
					bRev = TRUE;
				}
			} */
			if(sName == "DFT_SEQ")
			{
				m_nDftSeq = atoi(sValue);
				if(m_nDftSeq < 0){
					bRev = FALSE;
				}
				else{
				//	bRev = TRUE;
					CurrentSet->nDftSeq = m_nDftSeq;
				}
			}
		}
	}

	xmlTemp.OutOfElem();
	xmlTemp.OutOfElem();

	g_pView->UpdateToolOptionInfo();

	if(bRev){
		DisplyComStatus("S6F5 : OK", TRUE);
	}
	else{
		DisplyComStatus("S6F5 : NG", TRUE);
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

	nSeqNo = CurrentSet->nDftSeq + 1;
	sSeqNo.Format("%d", nSeqNo);

	if(CurrentSet->bMacAddCheck){
		nItemCount = 6;
	}
	else{
		nItemCount = 5;
	}

	if(!bResult){
		nItemCount = nItemCount + (FindResult2() * 12);
	}
	sItemCount.Format("%d", nItemCount);

	if(bResult == TRUE){sResult = "OK";}
	else{sResult = "NG";}
//	sTotalTime.Format("%.1f", fTime);

	if(CurrentSet->bFixtureIdCheck){
		sSystemNo.Format("%d,%s", CurrentSet->nSystemNo, CurrentSet->sFixtureId);
	}
	else{
		sSystemNo.Format("%d", CurrentSet->nSystemNo);
	}

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
	xmlTemp.AddChildElem("ID", CurrentSet->sModelID);
	xmlTemp.AddChildElem("NAME", CurrentSet->sModelName_GMES);
	xmlTemp.AddChildElem("SUFFIX", CurrentSet->sModelSuffix);
	xmlTemp.OutOfElem();

	xmlTemp.OutOfElem();

	xmlTemp.AddChildElem("ITEM");
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("SUBITEMLIST");
	xmlTemp.AddChildAttrib("COUNT", sItemCount);
	xmlTemp.IntoElem();
	xmlTemp.AddChildElem("NAME", "RESULT");  //5개 
	xmlTemp.AddChildElem("VALUE", sResult);
	xmlTemp.AddChildElem("NAME", "SEQ");
	xmlTemp.AddChildElem("VALUE", sSeqNo);
	xmlTemp.AddChildElem("NAME", "MACHINE_NO");
	xmlTemp.AddChildElem("VALUE", sSystemNo);
	xmlTemp.AddChildElem("NAME", "TOTAL_TIME");
	xmlTemp.AddChildElem("VALUE", sTotalTime);
	xmlTemp.AddChildElem("NAME", "CHASSIS");
	xmlTemp.AddChildElem("VALUE", CurrentSet->sChassisName);
	if(CurrentSet->bMacAddCheck){
		xmlTemp.AddChildElem("NAME", "MAC_ADDRESS");
		xmlTemp.AddChildElem("VALUE", CurrentSet->sMacAdd);
	}

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
				if(pStep->m_nTestType == MULTICHECL_TEST){
					sMeasured.Format(_T("%.02f, %.02f"), pStep->m_fMeasured_L,pStep->m_fMeasured_R);
				}
				else{
					sMeasured.Format(_T("%.1f"), pStep->m_fMeasured);
				}
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

			//sMsg.Format(_T("%s"), pStep->m_strMsg);
			if(pStep->m_strMsg.GetLength() > 200){
				sMsg = pStep->m_strMsg.Left(200);
			}
			else{
				sMsg = pStep->m_strMsg;
			}

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
