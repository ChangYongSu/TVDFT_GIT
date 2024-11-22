#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "AvConvertorCtrl.h"
#include "AvSwitchBoxCtrl.h"
#include "TVCommCtrl.h"

extern CDATsysView*  g_pView;

CAvConvertor gAvConvertorCtrl;

CAvConvertor::CAvConvertor()
{
	m_bPortOpen = FALSE;

	m_nPortID = -1;
	m_nAvSwitchCmd = -1;

//	m_bResult				= FALSE;
	

	m_strErrMsg = _T("");
}

CAvConvertor::~CAvConvertor()
{
	if(m_bPortOpen)
	{
		PortClose();
	}

}

// Open Com Port
BOOL CAvConvertor::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlAvConvertor.OpenPort( sComPort, nBaudRate, m_nPortID))
	{
		m_strErrMsg.Format("[AV Convertor] Failed to open %s port!", sComPort);
		m_bPortOpen = FALSE;
		
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	return TRUE;
}

void CAvConvertor::PortClose()
{
	m_ctrlAvConvertor.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CAvConvertor::Init()
{
	memset(m_strReceive, 0, 225);
//	CheckBufferClear();
	return TRUE;
}

BOOL CAvConvertor::SendCommString(CString str)
{
	int nRtn = 0;

	m_ctrlAvConvertor.m_QueueRead.Clear();
	nRtn = m_ctrlAvConvertor.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());
	if(nRtn != str.GetLength()) return FALSE;
	else return TRUE;
}

UINT CAvConvertor::CheckReceiveMsg(int nCmd, int nWaitLimit, int& nLength, CString& sValue)
{
	UINT nRtn = 0; // 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : AvSwitch Busy
	UINT nSize = 0; // Return Msg Size
	CString sRst = _T("");	
	CString sTemp = _T("");	
	CString sBuf = _T("");	
	int nDataNo = 0;

	if((nCmd == READ_COM_DATA) || (nCmd == READ_IR_DATA)) nSize = 20;
	else nSize = 10;

	nLength = 0;

	if(!ReceiveCommString(nSize, nWaitLimit))
	{
		nRtn = 3;
	}
	else 
	{
		if((nCmd == READ_COM_DATA) || (nCmd == READ_IR_DATA))
		{
			sRst.Format("%c%c%c%c%c%c", m_strReceive[1], m_strReceive[2],m_strReceive[3],m_strReceive[4], m_strReceive[5], m_strReceive[6]);
			
			sTemp.Format("%c%c%c%c%c%c", m_strReceive[11], m_strReceive[12],m_strReceive[13],m_strReceive[14], m_strReceive[15], m_strReceive[16]);
			//sTemp.TrimLeft("TCOM");
			nDataNo = atoi(sTemp.Right(2));
			nLength = nDataNo;
			if(nDataNo > 0){
				for(int i = 0; i < nDataNo; i++){
					sTemp.Format("%c", m_strReceive[17 + i]);
					sBuf = sBuf + sTemp;
				}
				sValue = sBuf;
			}
		}
		else{
			sRst.Format("%c%c%c%c%c%c", m_strReceive[1], m_strReceive[2],m_strReceive[3],m_strReceive[4], m_strReceive[5], m_strReceive[6]);
			sRst.MakeUpper();
		}
	}
	if(sRst == "DREADY") 
	{
		nRtn =  1;	
	}
	else nRtn = 2;

	return nRtn;
}

//==============
// AvConvertor Cmd 
//==============
// 1st parameter(Cmd Type)		/ 2nd parameter (nValue1) 				
// 01. SET_INPUR_SEL			/ Input Port(00 : VGA, 01 : HDMI, 02 : DP, 03 : DVI)		
// 02. READ_COM_DATA			/ Don't Care
// 03. READ_IR_DATA				/ Don't Care
// 04. BUFFER_CLR				/ Don't Care


CString CAvConvertor::MakeCmdString(int nCmd, CString& szSwitchBoxMsg, int nValue1)
{
	CString szCmdString	= _T("");
	CString sTmp		= _T("");
	int nVolt = 0;
	szSwitchBoxMsg	= _T("");

	switch(nCmd)
	{
		case SET_INPUR_SEL : 
		{				
			switch(nValue1) 
			{
				case 00 : 
					szSwitchBoxMsg = _T("Set InPut VGA"); break;
				case 01 : 
					szSwitchBoxMsg = _T("Set InPut DVI"); break;
				case 02 : 
					szSwitchBoxMsg = _T("Set InPut Display Port"); break;
				case 03 : 
					szSwitchBoxMsg = _T("Set InPut HDMI"); break;
			}
			szCmdString.Format(_T("%cCV_SEL_%02d%c%c%c"), STX, nValue1, ETX, CR, LF);
		}
		break;

		case READ_COM_DATA :
		{	
			szCmdString.Format(_T("%cCREAD_COM%c%c%c"), STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Read Com Data");
		}
		break;
		
		case READ_IR_DATA :
		{	
			szCmdString.Format(_T("%cCREAD_IRS%c%c%c"), STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Read IR Data");
		}
		break;

		case BUFFER_CLR :
		{	
			szCmdString.Format(_T("%cCBUFF_CLR%c%c%c"), STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Buffer Clear");
		}
		break;
	}
	return szCmdString;
}
BOOL CAvConvertor::ReceiveCommString(int nRev, int nWaitLimit)
{
//	MSG msg;
	BOOL		bDoing;
    clock_t		start;
	int			i/*, nWaitLimit*/;
	BYTE		buf;
	DWORD		dwRead;			// Read Byte Count
	BYTE		buff[2048];		// Read Buffer
	CString		sTmp;

	if(m_bPortOpen == FALSE) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();

	m_ctrlAvConvertor.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{
			//+add kwmoon 080621
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
				if (msg.message == 45557 || msg.message == 45559)break;
				//CString s;
				//s.Format("msg.message= %d",msg.message); 
				//AddStringToStatus(s);
			}
		*/
			//-

			dwRead = m_ctrlAvConvertor.ReadComm( buff, 2048);
			if (dwRead == 0){
				Sleep(100);
				dwRead = m_ctrlAvConvertor.ReadComm( buff, 2048);
			}
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlAvConvertor.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlAvConvertor.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				return FALSE;
			}

			//+add kwmoon 071030
			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				m_ctrlAvConvertor.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_ctrlAvConvertor.m_QueueRead.GetSize();

		if (rrv >= nRev)
		{
			for(i=0; i<rrv; i++)
			{
				m_ctrlAvConvertor.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}

			if(CurrentSet->bCommDisplay)
			{
				CString s;
				s.Format("Response : %s",m_strReceive);
				AddStringToStatus(s);
			}
			m_ctrlAvConvertor.m_QueueRead.Clear();

			return TRUE;
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			m_ctrlAvConvertor.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
	return TRUE;	
}

/*
BOOL CAvConvertor::SendCmd(int nCmd, int nWaitLimit, CString& sValue , int nValue1)
{
	// 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : Cmd Send Retry, 5 : Send Cmd Fail
	CString szCmdString	= _T("");
	CString szSwitchBoxMsg = _T("");
	UINT nRtn = 0;
	m_strErrMsg = _T("");

	m_nAvSwitchCmd = nCmd;

	szCmdString = MakeCmdString(nCmd, szSwitchBoxMsg, nValue1);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(nCmd, nWaitLimit, sValue);

		if(nRtn == 1) m_strErrMsg = _T("PASS");
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
	}
	
	if(CurrentSet->bCommDisplay)
	{
		if(szSwitchBoxMsg != _T("")) AddStringToStatus(szSwitchBoxMsg + " : " + m_strErrMsg);
	}

	if(nRtn == 1 ) return TRUE;
	else return FALSE;
}
*/
BOOL CAvConvertor::SetInputSignal(int nInput)
{
	// 00 : VGA, 01 : DVI, 02 : Display Port, 03 : HDMI
	CString szCmdString	= _T("");
	CString szSwitchBoxMsg = _T("");
	CString sValue = _T("");
	int nDataLength;
	UINT nRtn = 0;
	m_strErrMsg = _T("");

	CString		sTmp = _T("");

	if(!m_bPortOpen){return FALSE;}

	m_nAvSwitchCmd = SET_INPUR_SEL;
	if(nInput < 0 || nInput > 3) nInput = 0;

	szCmdString	   = MakeCmdString(SET_INPUR_SEL,szSwitchBoxMsg,  nInput);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(SET_INPUR_SEL, 3000, nDataLength, sValue);

		if(nRtn == 1) m_strErrMsg = _T("PASS");
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
	}
	if(CurrentSet->bCommDisplay)
	{
		sTmp.Format("%s : %s", szSwitchBoxMsg, m_strErrMsg);
		AddStringToStatus(sTmp);
	}

	if(nRtn == 1 ) return TRUE;
	else return FALSE;
}

BOOL CAvConvertor::CheckRemoteCmd(int& nLength, CString& sValue)
{
	CString szCmdString		= _T("");
	CString szSwitchBoxMsg	= _T("");
	CString sData			= _T("");
	int nDataLength;
	CString		sTmp = _T("");

	UINT nRtn		= 0;
	UINT nValue		= 0;
	m_strErrMsg		= _T("");

	if(!m_bPortOpen){return FALSE;}

	m_nAvSwitchCmd = READ_IR_DATA;
	szCmdString	   = MakeCmdString(READ_IR_DATA, szSwitchBoxMsg, 0);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(READ_IR_DATA, 3000, nDataLength, sData);

		if(nRtn == 1){
			m_strErrMsg = _T("PASS");
		}
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
	}
	

	if(CurrentSet->bCommDisplay)
	{
		sTmp.Format("%s : %s", szSwitchBoxMsg, m_strErrMsg);
		AddStringToStatus(sTmp);
	}

	nLength = nDataLength;
	sValue = sData;

	if(nRtn == 1 ) return TRUE;
	else return FALSE;
}

BOOL CAvConvertor::CheckComData(int& nLength, CString& sValue)
{
	CString szCmdString		= _T("");
	CString szSwitchBoxMsg	= _T("");
	CString sData			= _T("");
	int nDataLength;
	CString		sTmp = _T("");

	UINT nRtn		= 0;
	UINT nValue		= 0;
	m_strErrMsg		= _T("");

	if(!m_bPortOpen){return FALSE;}

	m_nAvSwitchCmd = READ_COM_DATA;
	szCmdString	   = MakeCmdString(READ_COM_DATA, szSwitchBoxMsg, 0);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		nRtn = CheckReceiveMsg(READ_COM_DATA, 3000, nDataLength, sData);

		if(nRtn == 1){
			m_strErrMsg = _T("PASS");

		}
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
	}
	

	if(CurrentSet->bCommDisplay)
	{
		sTmp.Format("%s : %s", szSwitchBoxMsg, m_strErrMsg);
		AddStringToStatus(sTmp);
	}

	sData.TrimRight(0x0a);
	sData.TrimRight(0x0d);
	nLength = nDataLength;
	sValue = sData;

	if(nRtn == 1 ) return TRUE;
	else return FALSE;
}

BOOL CAvConvertor::CheckBufferClear()
{
	CString szCmdString	= _T("");
	CString szSwitchBoxMsg = _T("");
	CString sValue			= _T("");
	int nDataLength;
	UINT nRtn = 0;
	m_strErrMsg = _T("");

	CString		sTmp = _T("");

	if(!m_bPortOpen){return FALSE;}
	m_nAvSwitchCmd = BUFFER_CLR;

	szCmdString	   = MakeCmdString(BUFFER_CLR, szSwitchBoxMsg, 0);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		//Sleep(1500);
		nRtn = CheckReceiveMsg(BUFFER_CLR, 5000, nDataLength, sValue);

		if(nRtn == 1) m_strErrMsg = _T("PASS");
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
	}
	if(CurrentSet->bCommDisplay)
	{
		sTmp.Format("%s : %s", szSwitchBoxMsg, m_strErrMsg);
		AddStringToStatus(sTmp);
	}

	if(nRtn == 1 ) return TRUE;
	else return FALSE;
}
BOOL CAvConvertor::IRDataCheck()
{
	CString sTemp;
	int nLength;
	CString sValue;
	int nValue = 0;

//	CheckBufferClear();
//	if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")){
//		return FALSE;
//	}
//	Sleep(500);
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b");

	if(CheckRemoteCmd(nLength, sValue)){
		sTemp.Format(_T("%s : %d - %s"), m_strErrMsg, nLength, sValue);
		AddStringToStatus(sTemp);
	}
	else{
		AddStringToStatus(m_strErrMsg);
		return FALSE;
	}
	if(nLength == 0){ return FALSE;}
	nValue = hexCstr2decNum(sValue.Right(2));
	if(nValue == 0x5b){ return TRUE;}
	else{ return FALSE;}
}
BOOL CAvConvertor::IRBlasterDataCheck()
{
	CString sTemp;
	int nLength;
	CString sValue;
	int nValue = 0;

	//	CheckBufferClear();
	//	if(AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5b")){
	//		return FALSE;
	//	}
	//	Sleep(500);

	if (TVCommCtrl.Send_TVControl_Command("av 00 25", 700) == FALSE)
	{
//		return 0;
	}
	

	CheckRemoteCmd(nLength, sValue);
	for (int i = 0; i < 5; i++)
	{
		Sleep(500);
		if (CheckRemoteCmd(nLength, sValue)) {
			sTemp.Format(_T("%s : %d - %s"), m_strErrMsg, nLength, sValue);
			AddStringToStatus(sTemp);
			sValue.MakeUpper();
			if (sValue.Find("DAC048B7") >= 0)
			{
				TVCommCtrl.Send_TVControl_Command("av 00 26", 700);
				return TRUE;
			}
		}
		else
		{
			break;
		}		
	}
	
	AddStringToStatus(m_strErrMsg);
	TVCommCtrl.Send_TVControl_Command("av 00 26", 700);
	return FALSE;

	
}

BOOL CAvConvertor::ComDataCheck()
{
	CString sTemp;
	int nLength;
	CString sValue;
	int nValue = 0;

//	CheckBufferClear();

	TVCommCtrl.Send_TVControl_Command ("mc 00 44", 2000);

	Sleep(100);

	if(CheckComData(nLength, sValue)){
		sValue.TrimRight(0x0a);
		sValue.TrimRight(0x0d);

		sTemp.Format(_T("%s : %d - %s"), m_strErrMsg, nLength, sValue);
		AddStringToStatus(sTemp);
	}
	else{
		AddStringToStatus(m_strErrMsg);
		return FALSE;
	}
	if(nLength == 0){ return FALSE;}
	sValue.MakeLower();

	if(sValue.Compare("mc 00 44") == 0){ return TRUE;}
	else{ return FALSE;}
}