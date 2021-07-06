// Gmes.cpp: implementation of the CGmes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datsys.h"
#include "PLC_Ctrl.h"
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

CPLC_Ctrl gPLC_Ctrl;

extern CDATsysView*			g_pView;
extern CString				m_strWipId;	//Wip ID

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPLC_Ctrl::CPLC_Ctrl()
{
	m_bPLCCom = FALSE;
	m_bPCB_Onload_JIG = FALSE;
	m_sReceive = _T("");
	//m_sTransactionID = _T("");
	//m_sWorkOrderID = _T("");
	//m_sOrganizationID = _T("");
	//m_sLineID = _T("");
	//m_sEquipmentID = _T("");
	//m_sOperationID = _T("");
	//m_sModelID = _T("");
	m_sModelName = _T("");
	//m_sModelSuffixName = _T("");
	//m_nOrganizationID = 0;
	//m_nDftSeq = 0;
	m_sErrorMsg = _T("");

}

CPLC_Ctrl::~CPLC_Ctrl()
{

}

BOOL CPLC_Ctrl::SoketCom_Creat(CString sIpAdd, int nPort)
{
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


	g_pView->m_pPLCSocketClient.m_SerVerDeviceID = _SERVER_PLC_ID;
	g_pView->m_pPLCSocketClient.Create();										//클라이언트 소켓 생성
	connt = g_pView->m_pPLCSocketClient.Connect(sIpAdd, nPort);					//서버에 연결을 시도 한다.

	if(connt){												//오류 발생시
		AddStringToStatus("PLC Connection OK.");
		m_bPLCCom = TRUE;
		m_bPLC_WaitCount = 0;
		m_bPLC_RegWaitCount = 0;
		m_bPLC_Online = 1;

		
	}
	else{
		g_pView->m_pPLCSocketClient.Close();//소켓을 종료한다.
		AddStringToStatus("PLC Connection error!");
	//	AfxMessageBox("PLC Connection error!");
		m_bPLCCom = FALSE;
	}
	return m_bPLCCom;
}
void CPLC_Ctrl::SoketCom_Close()
{
	g_pView->m_pPLCSocketClient.Close();//소켓을 종료한다.
	AddStringToStatus("PLC Connection Close");
	m_bPLCCom = FALSE;
}

BOOL CPLC_Ctrl::SendCommString(CString strData)
{
	int nSendLenth;
	int nRtn;

//	if(!m_bGmesCom) {return FALSE;}

//	m_sReceive = _T("");
	nSendLenth = strData.GetLength();
//	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);

	nRtn = g_pView->m_pPLCSocketClient.Send(strData, nSendLenth); //데이타 전송 부분

	if(nRtn != nSendLenth) return FALSE;

//	GmesLog_Save(g_pView->m_szExePath, strData, FALSE);
	return TRUE;
}

BOOL CPLC_Ctrl::ReceiveCommString(int nWaitLimit, CString& sReadString)
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
		dwRead = g_pView->m_pPLCSocketClient.Receive(m_sReadBuffer, sizeof(m_sReadBuffer));

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

void CPLC_Ctrl::DisplyComStatus(CString sItem, BOOL bFlag)
{
	if(!CurrentSet->bCommDisplay)
	{
		return;
	}

	CString sMsg;

	if(bFlag){
		sMsg.Format(_T("[RECV(PLC)] : %s"), sItem);
	}
	else{
		sMsg.Format(_T("[SEND(PLC)] : %s"), sItem);
	}

	AddStringToStatus(sMsg);
}

BOOL CPLC_Ctrl::Paser_ReceiveData(CString sReceiveData)
{
	
	BOOL bRev = TRUE;	
	CString sSetID;

	//xmlTemp.SetDoc(sReceiveData);
	//void CSockDlg::OnReceive()
	//{
	//	byte pBuf[4096];
	//	int iBufSize = 4096;
	//	int iRcvd;
	//	CString strRecvd;
	//	byte RcvData[256];

	//	// Receive the message
	//	iRcvd = m_sConnectSocket.Receive(pBuf, iBufSize);
	//	strRecvd = _T("1:");
	//	if (iRcvd == SOCKET_ERROR)
	//	{
	//		iRcvd = m_sConnectSocket2.Receive(pBuf, iBufSize);
	//		strRecvd = _T("2:");
	//	}
	//	// Did we receive anything?
	//	if (iRcvd == SOCKET_ERROR)
	//	{
	//	}
	//	else
	//	{
	//		// Truncate the end of the message
	//		pBuf[iRcvd] = NULL;
	//		//strRecvd += pBuf;
	//		//////////////////////////////////////////////////////////////
	//		//Parcing 
	//		if (iRcvd >= 11)
	//		{
	//			if ((pBuf[0] == 0xD0) && (pBuf[1] == 0x00))
	//			{

	//			}
	//			else
	//			{
	//				strRecvd += "Head Error";
	//				strRecvd += (char *)pBuf;
	//				m_ctlRecvd.AddString(strRecvd);

	//			}

	//			int ReceiveSize = pBuf[8];// + pBuf[7];
	//			ReceiveSize = ReceiveSize << 8;
	//			ReceiveSize += pBuf[7] + 9;
	//			if (iRcvd >= ReceiveSize)
	//			{

	//				int lDataLen;// = pBuf[20];
	//			/*	lDataLen = lDataLen<<8;
	//				lDataLen  +=  pBuf[19];*/


	//				if ((pBuf[9] == 0x00) && (pBuf[10] == 0x00))//End Code
	//				{
	//					//	strRecvd += "ACK";
	//						//strRecvd += (char *)pBuf;
	//					//	m_ctlRecvd.AddString(strRecvd);
	//						//return ;

	//				}
	//				else
	//				{
	//					strRecvd.Format(_T("Error Code: 0x%02X%02X"), pBuf[10], pBuf[9]);
	//					//	strRecvd += (char *)pBuf;
	//					m_ctlRecvd.AddString(strRecvd);
	//					return;
	//				}
	//				lDataLen = ReceiveSize - 11;
	//				lDataLen = lDataLen / 2;
	//				if (lDataLen >= 100)
	//				{
	//				}
	//				else
	//				{
	//					strRecvd += "ACK";
	//					//	strRecvd += (char *)pBuf;
	//					m_ctlRecvd.AddString(strRecvd);
	//					return;
	//				}

	//				//IDC_STATIC_MODEL_NAME
	//				char lModelBuf[64];
	//				lModelBuf[40] = 0;
	//				for (int i = 0; i < 20; i++)
	//				{
	//					lModelBuf[i * 2] = pBuf[11 + (i * 2)];
	//					lModelBuf[i * 2 + 1] = pBuf[11 + (i * 2) + 1];
	//				}
	//				SetDlgItemText(IDC_STATIC_MODEL_NAME, lModelBuf);

	//				for (int i = 0; i < lDataLen; i++)
	//				{
	//					RcvData[i] = pBuf[11 + (i * 2)];
	//				}


	//				/*	if(RcvData[_PLC_ADDRESS_MODEL_NAME])
	//					{
	//						m_cCheckLoad.SetCheck(1);
	//					}
	//					else
	//					{
	//						m_cCheckLoad.SetCheck(0);
	//					}*/

	//				if (RcvData[_PLC_ADDRESS_LOAD_COMMAND])
	//				{
	//					m_cCheckLoad.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckLoad.SetCheck(0);
	//				}

	//				if (RcvData[_PLC_ADDRESS_UNLOAD_COMMAND])
	//				{
	//					m_cCheckUnLoad.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckUnLoad.SetCheck(0);
	//				}

	//				if (RcvData[_PLC_ADDRESS_TEST_PASS])
	//				{
	//					m_cCheckTestPass.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckTestPass.SetCheck(0);
	//				}

	//				if (RcvData[_PLC_ADDRESS_TEST_NG])
	//				{
	//					m_cCheckTestNg.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckTestNg.SetCheck(0);
	//				}

	//				if (RcvData[_PLC_ADDRESS_FFC_RETRY_COMMAND])
	//				{
	//					m_cCheckReconnect.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckReconnect.SetCheck(0);
	//				}

	//				//if(RcvData[_PLC_ADDRESS_MODEL_NAME])
	//				//{
	//				//	m_cCheckLoad.SetCheck(1);
	//				//}
	//				//else
	//				//{
	//				//	m_cCheckLoad.SetCheck(0);
	//				//}

	//				if (RcvData[_PLC_ADDRESS_LOAD_FINISH])
	//				{
	//					m_cCheckLoadFinish.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckLoadFinish.SetCheck(0);
	//				}

	//				if (RcvData[_PLC_ADDRESS_UNLOAD_FINISH])
	//				{
	//					m_cCheckUnLoadFinish.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckUnLoadFinish.SetCheck(0);
	//				}

	//				if (RcvData[_PLC_ADDRESS_FFC_RETRY_COMMAND])
	//				{
	//					m_cCheckReconnect.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckReconnect.SetCheck(0);
	//				}

	//				if (RcvData[_PLC_ADDRESS_FFC_RETRY_FINISH])
	//				{
	//					m_cCheckReconnectFinish.SetCheck(1);
	//				}
	//				else
	//				{
	//					m_cCheckReconnectFinish.SetCheck(0);
	//				}


	//				//	CButton m_cCheckLoadFinish;
	//				//	CButton m_cCheckUnLoad;
	//				//	CButton m_cCheckUnLoadFinish;
	//				//	CButton m_cCheckTestPass;
	//				//	CButton m_cCheckTestNg;
	//				//	CButton m_cCheckReconnect;
	//				//	CButton m_cCheckReconnectFinish;


	//			}
	//			else
	//			{
	//				strRecvd += "Size Error";
	//				strRecvd += (char *)pBuf;
	//				m_ctlRecvd.AddString(strRecvd);

	//			}

	//		}

	//		// Copy the message to a CString
	//	//	strRecvd += pBuf;

	//		// Add the message to the received list box
	//	//	m_ctlRecvd.AddString(strRecvd);


	//		// Sync the variables with the controls
	//		UpdateData(TRUE);

	//	}
	//}


	return bRev;
}

int CPLC_Ctrl::WriteFullData(UINT Address, char* Data, int lDataLen)
{
	byte packet[256];
	if (lDataLen <= 0)
		return 0;

	byte lData40[64];
	lData40[40] = 0;
	memset(lData40, 0x20, 40);
	memcpy(lData40, Data, lDataLen);

	int writeSize = 12 + 40;//lDataLen;

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
	packet[12] = 0x14; //READ Command 0401 HIGH


	packet[13] = 0x00; ////READ Command SUB 0000 LOW
	packet[14] = 0x00; //READ Command SUB 0000 HIGH

	packet[15] = (byte)(Address & 0x000000FF);           // Low  : 선두 디바이스
	packet[16] = (byte)((Address & 0x0000FF00) >> 8);    // 
	packet[17] = (byte)((Address & 0x00FF0000) >> 16);   // High

	packet[18] = 0xA8; //D* A8H = Data register

	int lWordLen = 20;
	packet[19] = (byte)(lWordLen & 0xFF);     // Low    
	packet[20] = (byte)(lWordLen >> 8);       // High

	for (int i = 0; i < lWordLen; i++)
	{
		packet[21 + (i * 2)] = (byte)(lData40[i * 2]);     // Low
		packet[22 + (i * 2)] = (byte)(lData40[i * 2 + 1]);       // High
	}

	int iLen = 21 + 40;//(lDataLen);//m_strMessage.GetLength();

	//nRtn = g_pView->m_pPLCSocketClient.Send(strData, nSendLenth);
	int iSent = g_pView->m_pPLCSocketClient.Send(packet, iLen);

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
}

int CPLC_Ctrl::WriteCommand(UINT Address, char* Data, int lDataLen)
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
	int iSent = g_pView->m_pPLCSocketClient.Send(packet, iLen);
	

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

int CPLC_Ctrl::ReadCommand(UINT Address, int lDataLen)
{
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


	int iSent = g_pView->m_pPLCSocketClient.Send(packet, iLen);

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
}


int CPLC_Ctrl::ReadRegData(UINT Address, int lDataLen)
{
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


	int iSent = g_pView->m_pPLCSocketClient.Send(packet, iLen);

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
}



void CPLC_Ctrl::CommandModelName(CString l_strEditModelName)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_MODEL_NAME;

	gPLC_Ctrl.WriteFullData(Address, l_strEditModelName.GetBuffer(), l_strEditModelName.GetLength());
}
//
//#define _PLC_ADDRESS_LOAD_COMMAND		50 
//#define _PLC_ADDRESS_UNLOAD_COMMAND		51 
//#define _PLC_ADDRESS_TEST_PASS			52 
//#define _PLC_ADDRESS_TEST_NG			53 
//#define _PLC_ADDRESS_FFC_RETRY_COMMAND	54 
void CPLC_Ctrl::CommandClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_LOAD_COMMAND;
	char Buff[256];
	Buff[0] = 0;
	Buff[1] = 0;
	Buff[2] = 0;
	Buff[3] = 0;
	Buff[4] = 0;

	gPLC_Ctrl.WriteCommand(Address, Buff, 5);

	Sleep(50);
	Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_SET_RETRY;
	//char Buff[256];
	Buff[0] = 0;
	//Buff[1] = 0; //OK
	//Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	Sleep(50);
	//Address = l_PLC_BaseAddress + _PLC_ADDRESS_LOAD_FAST_COMMAND;
	////char Buff[256];
	//Buff[1] = 0; //LOAD FAST
	//Buff[2] = 0; //UNLOAD FAST
	//gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	Command3TimesErrorClear();

	gStrTmpLog = "CommandClear()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
}

void CPLC_Ctrl::CommandLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_LOAD_COMMAND;
	char Buff[256];
	Buff[0] = 1;

	gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	gStrTmpLog = "CommandLoad()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

}


void CPLC_Ctrl::CommandLoadFast()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//CommandLoad();

	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_FAST_COMMAND;
	char Buff[256];
	Buff[0] = 1;

	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
	gStrTmpLog = "CommandLoadFast()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
}

void CPLC_Ctrl::CommandSkipFFC()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_FAST_COMMAND;
	char Buff[256];
	Buff[0] = 1;

	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}

void CPLC_Ctrl::CommandConnectFFC()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_FAST_COMMAND;
	char Buff[256];
	Buff[0] = 0;

	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}


void CPLC_Ctrl::CommandPassUnload()
{
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;
	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_SET_RETRY;
	char Buff[256];
	Buff[0] = 0;
	//Buff[1] = 0; //OK
	//Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	Sleep(20);
	 Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_COMMAND;
	//char Buff[256];
	Buff[0] = 1;
	Buff[1] = 1; //OK
	Buff[2] = 0; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 3);

	gStrTmpLog = "CommandPassUnload()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
}


void CPLC_Ctrl::CommandNgUnload()
{
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;
	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_SET_RETRY;
	char Buff[256];
	Buff[0] = 0;
	//Buff[1] = 0; //OK
	//Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	Sleep(20);
	 Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_COMMAND;
	//char Buff[256];
	Buff[0] = 1;
	Buff[1] = 0; //OK
	Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 3);
	gStrTmpLog = "CommandNgUnload()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
}

void CPLC_Ctrl::CommandPassUnloadFast()
{
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;
	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_SET_RETRY;
	char Buff[256];
	Buff[0] = 0;
	//Buff[1] = 0; //OK
	//Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);


	//Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_FAST_COMMAND;
	////char Buff[256];
	//Buff[0] = 1;	
	//gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	Address = l_PLC_BaseAddress + _PLC_ADDRESS_TEST_PASS;
	//char Buff[256];
	Buff[0] = 1;//OK
	Buff[1] = 0; //NG 
	gPLC_Ctrl.WriteCommand(Address, Buff, 2);
	gStrTmpLog = "CommandPassUnloadFast()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
}


void CPLC_Ctrl::CommandNgUnloadFast()
{
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;
	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_SET_RETRY;
	char Buff[256];
	Buff[0] = 0;
	//Buff[1] = 0; //OK
	//Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	//Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_FAST_COMMAND;
	////char Buff[256];
	//Buff[0] = 1;
	//gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	Address = l_PLC_BaseAddress + _PLC_ADDRESS_TEST_PASS;
	//char Buff[256];
	Buff[0] = 0;//OK
	Buff[1] = 1; //NG 
	gPLC_Ctrl.WriteCommand(Address, Buff, 2);
	gStrTmpLog = "CommandNgUnloadFast()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
}

void CPLC_Ctrl::CommandRetryUnload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	
	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_SET_RETRY;
	char Buff[256];
	Buff[0] = 1;
	//Buff[1] = 0; //OK
	//Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);


	Address = l_PLC_BaseAddress + _PLC_ADDRESS_UNLOAD_COMMAND;
	Buff[256];
	Buff[0] = 1;
	Buff[1] = 0; //OK
	Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 3);

	gStrTmpLog = "CommandRetryUnload()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
	
}

void CPLC_Ctrl::CommandFfcRetry()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_FFC_RETRY_COMMAND;
	char Buff[256];
	Buff[0] = 1;
	//	Buff[1] = 0; //OK
	//	Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);

	gStrTmpLog = "CommandFfcRetry()"; //gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName);
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
}


void CPLC_Ctrl::CommandRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int lSel = m_cComboDFT_SEL.GetCurSel();
	//m_DFT_No = lSel;
	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;
	gPLC_Ctrl.ReadCommand(l_PLC_BaseAddress, 100);
}

void CPLC_Ctrl::CommandRegRead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int lSel = m_cComboDFT_SEL.GetCurSel();
	//m_DFT_No = lSel;
	UINT l_PLC_BaseAddress = 1040 + CurrentSet->nDFT_No * 20;
	gPLC_Ctrl.ReadRegData(l_PLC_BaseAddress, 20);
}


void CPLC_Ctrl::Command3TimesError()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_3_TIMES_ERROR;
	char Buff[256];
	Buff[0] = 1;
	//	Buff[1] = 0; //OK
	//	Buff[2] = 1; //NG
	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}

void CPLC_Ctrl::Command3TimesErrorClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UINT l_PLC_BaseAddress = 4000 + CurrentSet->nDFT_No * 100;

	UINT Address = l_PLC_BaseAddress + _PLC_ADDRESS_NG_3_TIMES_ERROR;
	char Buff[256];
	Buff[0] = 0;

	gPLC_Ctrl.WriteCommand(Address, Buff, 1);
}
