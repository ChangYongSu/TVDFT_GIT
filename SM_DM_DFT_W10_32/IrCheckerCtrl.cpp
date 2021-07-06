#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "IrCheckerCtrl.h"
#include "AvSwitchBoxCtrl.h"
#include "TVCommCtrl.h"

extern CDATsysView*  g_pView;

CIrChecker gIrCheckerCtrl;

CIrChecker::CIrChecker()
{
	m_bPortOpen = FALSE;

	m_nPortID = -1;
	m_nAvSwitchCmd = -1;

//	m_bResult				= FALSE;
	

	m_strErrMsg = _T("");
}

CIrChecker::~CIrChecker()
{
	if(m_bPortOpen)
	{
		PortClose();
	}

}

// Open Com Port
BOOL CIrChecker::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlIrChecker.OpenPort( sComPort, nBaudRate, m_nPortID))
	{
		m_strErrMsg.Format("[AV Convertor] Failed to open %s port!", sComPort);
		m_bPortOpen = FALSE;
		
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	return TRUE;
}

void CIrChecker::PortClose()
{
	m_ctrlIrChecker.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CIrChecker::Init()
{
	memset(m_strReceive, 0, 225);
//	CheckBufferClear();
	return TRUE;
}

BOOL CIrChecker::SendCommString(CString str)
{
	int nRtn = 0;

	m_ctrlIrChecker.m_QueueRead.Clear();
	nRtn = m_ctrlIrChecker.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());
	if(nRtn != str.GetLength()) return FALSE;
	else return TRUE;
}

UINT CIrChecker::CheckReceiveMsg(int nCmd, int nWaitLimit, CString& sValue)
{
	UINT nRtn = 0; // 0 : NO Test, 1 : Receive Pass, 2 : Receive Fail, 3 : Receive TimeOut, 4 : AvSwitch Busy
	UINT nSize = 0; // Return Msg Size
	CString sRst = _T("");	
	CString sTemp = _T("");	
	CString sBuf = _T("");	
	int nDataNo = 0;

	if((nCmd == READ_IR0_DATA) || (nCmd == READ_IR1_DATA)) nSize = 16;
	else nSize = 10;


	if(!ReceiveCommString(nSize, nWaitLimit))
	{
		nRtn = 3;
	}
	else 
	{
		if((nCmd == READ_IR0_DATA) || (nCmd == READ_IR1_DATA))
		{
			sRst.Format("%c%c%c%c", m_strReceive[1], m_strReceive[2],m_strReceive[3],m_strReceive[4]);
			
			sTemp.Format("%c%c%c%c%c%c%c%c", m_strReceive[5], m_strReceive[6],m_strReceive[7],m_strReceive[8], m_strReceive[9], m_strReceive[10], m_strReceive[11], m_strReceive[12]);
			sValue = sTemp;

			sRst.MakeUpper();
			if(sRst.Find("DIR") != -1){nRtn =  1;	}
			else{ nRtn = 2;	}
		}
		else{
			sRst.Format("%c%c%c%c%c%c", m_strReceive[1], m_strReceive[2],m_strReceive[3],m_strReceive[4], m_strReceive[5], m_strReceive[6]);
			sRst.MakeUpper();
			if(sRst == "DREADY"){nRtn =  1;	}
			else{ nRtn = 2;	}
		}
	}

	return nRtn;
}

//==============
// IrChecker Cmd 
//==============
// 1st parameter(Cmd Type)		/ 2nd parameter (nValue1) 				
// 01. READ_IR0_DATA			/ Input Port(00 : VGA, 01 : HDMI, 02 : DP, 03 : DVI)		
// 02. READ_COM_DATA			/ Don't Care
// 03. READ_IR_DATA				/ Don't Care
// 04. BUFFER_CLR				/ Don't Care


CString CIrChecker::MakeCmdString(int nCmd, CString& szSwitchBoxMsg)
{
	CString szCmdString	= _T("");
	CString sTmp		= _T("");
	int nVolt = 0;
	szSwitchBoxMsg	= _T("");

	switch(nCmd)
	{
		case READ_IR0_DATA :
		{	
			szCmdString.Format(_T("%cCIR_READ0%c%c%c"), STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Read IR0 Data");
		}
		break;

		case READ_IR1_DATA :
		{	
			szCmdString.Format(_T("%cCIR_READ1%c%c%c"), STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Read IR1 Data");
		}
		break;

		case BUFFER_CLR :
		{	
			szCmdString.Format(_T("%cCIR_CLEAR%c%c%c"), STX, ETX, CR, LF);
			szSwitchBoxMsg.Format("Buffer Clear");
		}
		break;
	}
	return szCmdString;
}
BOOL CIrChecker::ReceiveCommString(int nRev, int nWaitLimit)
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

	m_ctrlIrChecker.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{
			dwRead = m_ctrlIrChecker.ReadComm( buff, 2048);
			if (dwRead == 0){
				Sleep(100);
				dwRead = m_ctrlIrChecker.ReadComm( buff, 2048);
			}
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlIrChecker.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlIrChecker.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				return FALSE;
			}

			//+add kwmoon 071030
			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				m_ctrlIrChecker.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_ctrlIrChecker.m_QueueRead.GetSize();

		if (rrv >= nRev)
		{
			for(int i=0; i<rrv; i++)
			{
				m_ctrlIrChecker.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}

			if(CurrentSet->bCommDisplay)
			{
				CString s;
				s.Format("Response : %s",m_strReceive);
				AddStringToStatus(s);
			}
			m_ctrlIrChecker.m_QueueRead.Clear();

			return TRUE;
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			m_ctrlIrChecker.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
	return TRUE;	
}

BOOL CIrChecker::CheckRemoteCmd(CString& sValue, int nMode)
{
	CString szCmdString		= _T("");
	CString szSwitchBoxMsg	= _T("");
	CString sData			= _T("");
//	int nDataLength;
	CString		sTmp = _T("");

	UINT nRtn		= 0;
	UINT nValue		= 0;
	m_strErrMsg		= _T("");

	if(!m_bPortOpen){return FALSE;}

//	m_nAvSwitchCmd = READ_IR_DATA;
	if(nMode == 0){
		szCmdString	   = MakeCmdString(READ_IR0_DATA, szSwitchBoxMsg);
	}
	else{
		szCmdString	   = MakeCmdString(READ_IR1_DATA, szSwitchBoxMsg);
	}
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		if(nMode == 0){
			nRtn = CheckReceiveMsg(READ_IR0_DATA, 3000, sData);
		}
		else{
			nRtn = CheckReceiveMsg(READ_IR1_DATA, 3000, sData);
		}


		if(nRtn == 1){
			m_strErrMsg = _T("PASS");
		}
		else if(nRtn == 2){m_strErrMsg.Format("FAIL");}
		else if(nRtn == 3){m_strErrMsg.Format("TIMEOUT");}
	}
	

	sTmp.Format("%s : %s", szSwitchBoxMsg, m_strErrMsg);
	AddStringToStatus(sTmp);

	sValue = sData;

	if(nRtn == 1 ) return TRUE;
	else return FALSE;
}

BOOL CIrChecker::CheckBufferClear()
{
	CString szCmdString	= _T("");
	CString szSwitchBoxMsg = _T("");
	CString sValue			= _T("");
//	int nDataLength;
	UINT nRtn = 0;
	m_strErrMsg = _T("");

	CString		sTmp = _T("");

	if(!m_bPortOpen){return FALSE;}
	m_nAvSwitchCmd = BUFFER_CLR;

	szCmdString	   = MakeCmdString(BUFFER_CLR, szSwitchBoxMsg);
	
	if(!SendCommString(szCmdString)) 
	{
		m_strErrMsg.Format("SEND FAIL");
		nRtn = 5;
	}
	else
	{
		//Sleep(1500);
		nRtn = CheckReceiveMsg(BUFFER_CLR, 5000, sValue);

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
BOOL CIrChecker::IRDataCheck()
{
	CString sTemp;
//	int nLength;
	CString sValue;
	int nValue = 0;
	int nValue_ref = 0;
	CString sCmd = _T("");
	CString sName = _T("");
	CString sCustom = _T("");
	int nByte = 1;
	CString sMsg;
	BOOL bRev;

	sName = "ENTER";
	if(!FindRemoconCode(sName, sCustom, sCmd, nByte))
	{
		sMsg.Format("Remocon Code Not Found: %s", sName);
		AddStringToRemoteComLog(sMsg);
	}

	CheckBufferClear();
	////20210120 Time //Sleep(300);
//	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte);	
//	if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, "LG", "5b", 1)){
	if(!AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte)){
		return FALSE;
	}
	//20210120 Time //Sleep(300);
	bRev = CheckRemoteCmd(sValue, 1);
	if(!bRev){
		//20210120 Time //Sleep(500);
		if(!CheckRemoteCmd(sValue), 1){
			AddStringToStatus(m_strErrMsg);
			return FALSE;
		}
	}
	sTemp.Format(_T("%s : %s"), m_strErrMsg, sValue);
	AddStringToStatus(sTemp);

	nValue_ref = hexCstr2decNum(sCmd.Right(2));
	nValue = hexCstr2decNum(sValue.Mid(4,2));
	if(nValue_ref == nValue){ return TRUE;}
	else{ return FALSE;}
}
