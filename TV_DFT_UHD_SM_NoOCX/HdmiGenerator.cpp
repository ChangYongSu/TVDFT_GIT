// HdmiGenerator.cpp: implementation of the CHdmiGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datsys.h"
#include "HdmiGenerator.h"

#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CDATsysView*  g_pView;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHdmiGenerator HDMIGeneratorCtrl;

CHdmiGenerator::CHdmiGenerator()
{
	m_bPortOpen = FALSE;
	m_strErrMsg = _T("");
	m_nPortID	= -1;
	m_nModelNo = -1;
	m_nPatternNo = -1;
//	m_nRemoteMode;	//0 : offLine, 1 : online
	m_bHDCPMode = FALSE;	//0 : off,     1 : one
	m_bEDIDMode = FALSE;	//0 : Pass,    1 : Check
	m_bDDCLineMode = FALSE;	//0 : Open,    1 : Close
	m_bCECMode =FALSE;
	m_nOutPort = 0;
	m_bMhlMode = FALSE;

}

CHdmiGenerator::~CHdmiGenerator()
{

}
// Open Com Port
BOOL CHdmiGenerator::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlPatternGenerator.OpenPort( sComPort, nBaudRate, m_nPortID,8,0,0))
	{
		m_strErrMsg.Format("[HDMI GEN.] Failed to open %s port!", sComPort);

		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	_Wait(150);

	return TRUE;
}

void CHdmiGenerator::PortClose()
{
	m_ctrlPatternGenerator.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CHdmiGenerator::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}


BOOL CHdmiGenerator::SendByte(BYTE Data)
{
	SendCommString(&Data);
	return TRUE;
}

BOOL CHdmiGenerator::SendCommString(CString str)
{
	TCHAR *command;

	command = (TCHAR *)(LPCTSTR)str;

	m_ctrlPatternGenerator.m_QueueRead.Clear();
	m_ctrlPatternGenerator.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());

//	for(int  i =0; i<str.GetLength(); i++)
//	{
//		m_ctrlPatternGenerator.WriteComm(command+i, 1);
//		_Wait(10);
//	}

	return TRUE;
}
BOOL CHdmiGenerator::ReceiveCommString(int nWaitLimit)
{
//	MSG msg;
	BOOL		bDoing;
    clock_t		start;
	int			i/*, nWaitLimit*/;
	BYTE		buf;
	DWORD		dwRead;			// Read Byte Count
	BYTE		buff[2048];		// Read Buffer
	CString		sTmp;
	CString		sTmpBuf;

	if (m_bPortOpen == FALSE)
	{	
		AddStringToStatus("m_bPortOpen == FALSE");
		return FALSE;
	}

	memset(m_strReceive, 0, 225);
	m_nReceiveCount = 0;

	bDoing = TRUE;
	start = clock();

	m_ctrlPatternGenerator.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{

			dwRead = m_ctrlPatternGenerator.ReadComm( buff, 2048);
			if (dwRead == 0){
				Sleep(100);
				dwRead = m_ctrlPatternGenerator.ReadComm( buff, 2048);
			}
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlPatternGenerator.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlPatternGenerator.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				AddStringToStatus("BUFF_SIZE - ..Size() > (int)dwRead)");
				return FALSE;
			}

			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				AddStringToStatus("//Over Limit Time.....False Return");
				int rrv1 = m_ctrlPatternGenerator.m_QueueRead.GetSize();
				sTmpBuf = "Data:";
				for (i = 0; i < rrv1; i++)
				{
					m_ctrlPatternGenerator.m_QueueRead.GetByte(&buf);
					sTmp.Format("0x02X ", buf);
					sTmpBuf += sTmp;
				}

				AddStringToStatus(sTmpBuf);
				m_ctrlPatternGenerator.m_QueueRead.Clear();
				return FALSE;
			}			  
		} while (dwRead); 

		int rrv = m_ctrlPatternGenerator.m_QueueRead.GetSize();

		if (rrv > 0)
		{
			for(i=0; i<rrv; i++)
			{
				m_ctrlPatternGenerator.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}
			m_nReceiveCount = rrv;

			sTmpBuf = "Data:";
			for (i = 0; i < rrv; i++)
			{
				sTmp.Format("0x%02X ", m_strReceive[i]);
				sTmpBuf += sTmp;
			}
				
			//AddStringToStatus(sTmpBuf);
			m_ctrlPatternGenerator.m_QueueRead.Clear();
			if(m_strReceive[0] == 0x06)
			{
				//AddStringToStatus("if(m_strReceive[0] == 0x06)");				
				return TRUE;
			}
			else{
				return FALSE;
			}
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			AddStringToStatus("//Over Limit Time.....No Data");
			m_ctrlPatternGenerator.m_QueueRead.Clear();
			return FALSE;
		}
	}
	AddStringToStatus("End return TRUE;");
	return TRUE;	
}

BOOL CHdmiGenerator::SetOnOffLine(BOOL bFlag)
{
	BOOL bReturn;
	CString szCmdString = _T("");

	if(bFlag){
		szCmdString.Format("%c",START_TERMINAL_MODE);
		m_nRemoteMode = ONLINE;
	}
	else{
		szCmdString.Format("%c",END_TERMINAL_MODE);
		m_nRemoteMode = OFFLINE;

	}

	// Port Open
	if(m_bPortOpen == FALSE)
	{
		if(Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate) == FALSE)
		{
			PortClose();
			Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
		}
	}

	bReturn = SendCommString(szCmdString);  

	return bReturn;
}

BOOL CHdmiGenerator::SetModel(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;


	if(!CurrentSet->bUseHDMIGen) return TRUE;
	
	if(m_nModelNo == nData) return TRUE;

//	SetOnOffLine(TRUE);
	
	m_ctrlPatternGenerator.ClearPort();
	
	if(nData < 1 || nData > 999) return FALSE;
	
	szData.Format("%03d", nData);
	szTemp.Format("Set Model(HDMI) %d", nData);
	szCmdString.Format("%c%c%s%c",STX,CALL_MODEL,szData,ETX);
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	m_nModelNo = nData;
	
	return TRUE;
}

BOOL CHdmiGenerator::SetPattern(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	int lreturn = 0;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(m_nPatternNo == nData) return TRUE;

	szData.Format("%03d", nData);
	szTemp.Format("Set Pattern(HDMI) %d", nData);
	szCmdString.Format("%c%c%s%c",STX,CALL_PATTERN,szData,ETX);

//	SetOnOffLine(TRUE);
	
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))		
	{
		lreturn = FALSE;
		return lreturn;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			lreturn = FALSE;
			return lreturn;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	m_nPatternNo = nData;
	
	return TRUE;
}

BOOL CHdmiGenerator::SetModel_Pattern(int nModel, int nPattern)
{
	CString szCmdString;
	CString szData;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;

//	if(!SetPattern(nPattern)) return FALSE;
//	Sleep(500);
//	if(!SetModel(nModel))return FALSE;

/*	if(nModel < 1 || nModel > 56)
	{
		if(nPattern >= 1 || nPattern <= 27) SetPattern(nPattern);
	
		return FALSE;
	}
	if(nPattern < 1 || nPattern > 27)
	{
		if(nModel >= 1 || nModel <= 56) SetModel(nPattern);
	
		return FALSE;
	}
*/
	if(CurrentSet->nHDMIGenType == 0){
		szData.Format("%03d%03d", nModel, nPattern);
	}
	else{
		szData.Format("%03d%02d", nModel, nPattern);
	}
	szTemp.Format("Set Model&Pattern(HDMI) %d, %d", nModel, nPattern);
	szCmdString.Format("%c%c%s%c",STX,CALL_MODEL_PATTERN,szData,ETX);
	if(SendCommString(szCmdString))
	{
		if(CurrentSet->bCommDisplay)
		{
			AddStringToStatus(szTemp);
		}
		m_nModelNo = nModel;
		m_nPatternNo = nPattern;
		return TRUE;
	}
	else return FALSE;

	return TRUE;
}
BOOL CHdmiGenerator::SetEDID_PassCheck(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;
	

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(bFlag)
	{
		szTemp = "Set HDMI_EDID Check";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x51,0x31,ETX);
		if(CurrentSet->nHDMIGenType == 0){
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x51,0x31,ETX);
		}
		else{
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x51,0x30,ETX);
		}
	}
	else
	{
		szTemp = "Set HDMI_EDID Pass";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x51,0x30,ETX);
		if(CurrentSet->nHDMIGenType == 0){
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x51,0x30,ETX);
		}
		else{
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x51,0x31,ETX);
		}
	}

//	SetOnOffLine(TRUE);
	
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	m_bEDIDMode = bFlag;
	return TRUE;

}
BOOL CHdmiGenerator::SetHDCP_OnOff(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(bFlag)
	{
		szTemp = "Set HDMI_HDCP On";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x50,0x31,ETX);
		if(CurrentSet->nHDMIGenType == 0){
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x50,0x31,ETX);
		}
		else{
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x50,0x30,ETX);
		}
	}
	else
	{
		szTemp = "Set HDMI_HDCP Off";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x50,0x30,ETX);
		if(CurrentSet->nHDMIGenType == 0){
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x50,0x30,ETX);
		}
		else{
			szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x50,0x31,ETX);
		}
	}

//	SetOnOffLine(TRUE);
	
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	m_bHDCPMode = bFlag;
	return TRUE;

}
BOOL CHdmiGenerator::SetDDCLine_OpenClose(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(bFlag)
	{
		szTemp = "Set HDMI_DDCLine Open";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x52,0x31,ETX);
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x52,0x31,ETX);
	}
	else
	{
		szTemp = "Set HDMI_DDCLine Close";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x52,0x30,ETX);
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x52,0x30,ETX);
	}
	
//	SetOnOffLine(TRUE);
	
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	m_bDDCLineMode = !bFlag;
	return TRUE;

}

BOOL CHdmiGenerator::SetOut(int nModel, int nPattern)
{
	BOOL bReturn;

	if (!CurrentSet->bUseHDMIGen) return TRUE;

	if (!m_bPortOpen)
	{
		if (Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PortClose();
			Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	if (m_bResetPatternModel == 1)//HDMIGeneratorCtrl
	{
		bReturn = SetModel_Pattern(nModel, nPattern);
		if (bReturn == TRUE)
		{
			m_bResetPatternModel = 0;
		}		
	}
	else
	{

		if ((m_nModelNo == nModel) && (m_nPatternNo != nPattern))
		{
			bReturn = SetPattern(nPattern);
			_Wait(500);
		}
		else if ((m_nModelNo != nModel) && (m_nPatternNo == nPattern))
		{
			bReturn = SetModel(nModel);
			_Wait(500);
		}
		else if ((m_nModelNo != nModel) && (m_nPatternNo != nPattern))
		{
			bReturn = SetModel_Pattern(nModel, nPattern);
			_Wait(500);
		}
	}

	return bReturn;
}


BOOL CHdmiGenerator::SetOut(int nModel, int nPattern, int Reset)
{
	BOOL bReturn;

	if (!CurrentSet->bUseHDMIGen) return TRUE;

	if (!m_bPortOpen)
	{
		if (Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate) == FALSE)
		{
			PortClose();
			Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	//if ((m_nModelNo == nModel) && (m_nPatternNo != nPattern))
	//{
	//	bReturn = SetPattern(nPattern);
	//	_Wait(500);
	//}
	//else if ((m_nModelNo != nModel) && (m_nPatternNo == nPattern))
	//{
	//	bReturn = SetModel(nModel);
	//	_Wait(500);
	//}
	//else
	{
		bReturn = SetModel_Pattern(nModel, nPattern);
		_Wait(500);
	}

	return bReturn;
}


BOOL CHdmiGenerator::SetOutPort(int nPort)
{
//	BOOL bReturn; 
	CString szCmdString;
	CString szData;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;

	if(!m_bPortOpen)
	{
		if(Create(CurrentSet->sPatternComPort , CurrentSet->wPatternBaudRate) == FALSE)
		{
			PortClose();
			Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	m_ctrlPatternGenerator.ClearPort();
	
	if(nPort < 0 || nPort > 5) return FALSE;
	
	szData.Format("%d", nPort);
	szTemp.Format("Set HDMI Out Port: %d", nPort);
//	if(CurrentSet->nHDMIGenType == 0){
//		szCmdString.Format("%c%c%c%c%c%c",STX,SET_OUT_PORT, 0x53,szData,0x30,ETX);
//	}
//	else{
		szCmdString.Format("%c%c%c%s%c",STX,SET_OUT_PORT, 0x53,szData,ETX);
//	}

	
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	m_nOutPort = nPort;

	return TRUE;
}
BOOL CHdmiGenerator::SetCEC_OnOff(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(bFlag)
	{
		szTemp = "Set CEC On";
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x54,0x31,ETX);
	}
	else
	{
		szTemp = "Set CEC Off";
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x54,0x30,ETX);
	}
		
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}

	m_bCECMode = bFlag;

	return TRUE;


}

BOOL CHdmiGenerator::SetARC_OnOff(int nPort, BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;
	CString szData;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(nPort < 0 || nPort > 5) return FALSE;

	szData.Format("%d", nPort);
//	szCmdString.Format("%c%c%c%s%c",STX,SET_OUT_PORT, 0x53,szData,ETX);

	if(bFlag)
	{
		szTemp.Format("Set ARC On : Port %d", nPort);
		szCmdString.Format("%c%c%c%s%c%c",STX, 0x1c, 0x55, szData, 0x31, ETX);
	}
	else
	{
		szTemp.Format("Set ARC Off : Port %d", nPort);
		szCmdString.Format("%c%c%c%s%c%c",STX, 0x1c, 0x55, szData, 0x30, ETX);
	}
		
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	return TRUE;


}

BOOL CHdmiGenerator::SetARC_Control(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;
	CString szData;

	if(!CurrentSet->bUseHDMIGen) return TRUE;

	if(bFlag)
	{
		szTemp = _T("Set ARC On : All Portd");
		szCmdString.Format("%c%c%c%c%c%c",STX, 0x1c, 0x55, 0x30, 0x31, ETX);
	}
	else
	{
		szTemp = _T("Set ARC Off : All Port");
		szCmdString.Format("%c%c%c%c%c%c",STX, 0x1c, 0x55, 0x30, 0x30, ETX);
	}
		
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	return TRUE;


}
BOOL CHdmiGenerator::SetMHL_OnOff(int nPort, BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;
	CString szData;

	if(!CurrentSet->bUseHDMIGen) return TRUE;

	if(nPort == 0){ return SetMHL_Control(bFlag);}

	if(nPort < 3 || nPort > 5) return FALSE;

	szData.Format("%d", nPort);

	if(bFlag)
	{
		szTemp.Format("Set MHL On : Port %d", nPort);
		szCmdString.Format("%c%c%c%s%c%c",STX, 0x1c, 0x60, szData, 0x31, ETX);
	}
	else
	{
		szTemp.Format("Set MHL Off : Port %d", nPort);
		szCmdString.Format("%c%c%c%s%c%c",STX, 0x1c, 0x60, szData, 0x30, ETX);
	}
		
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}

	g_pView->m_bMhlMode = bFlag;

	return TRUE;


}
BOOL CHdmiGenerator::SetMHL_Control(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;
	CString szData;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(bFlag)
	{
		szTemp = _T("Set MHL On: All Port");
		szCmdString.Format("%c%c%c%c%c%c",STX, 0x1c, 0x60, 0x30, 0x31, ETX);
	}
	else
	{
		szTemp = _T("Set MHL Off: All Port");
		szCmdString.Format("%c%c%c%c%c%c",STX, 0x1c, 0x60, 0x30, 0x30, ETX);
	}
		
	m_ctrlPatternGenerator.ClearPort();
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if(!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}

	m_bMhlMode = bFlag;

	return TRUE;


}

BOOL CHdmiGenerator::SetDFT_Mode(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	int lresult = 0;

	if (!CurrentSet->bUseHDMIGen) return TRUE;
	

	
	//szTemp.Format("Set DFT BOOT(HDMI) OK");
	//if (nData == 1)
	{
		szCmdString.Format("%c%c%c%c%c", STX, SET_OUT_PORT, 0x57, 0x031, ETX);
	}
	//else
	//{
	//	szCmdString.Format("%c%c%c%s%c", STX, SET_OUT_PORT, 0x57, 0x030, ETX);
	//}

	//	SetOnOffLine(TRUE);

	m_ctrlPatternGenerator.ClearPort();
	if (!SendCommString(szCmdString))
	{

		lresult = FALSE;
		return lresult;
	}
	if (!ReceiveCommString(2500))
	{
		SetOnOffLine(TRUE);
		SendCommString(szCmdString);
		if (!ReceiveCommString(2500))
		{
			AddStringToStatus(_T("HDMIGen Com. Error"));
			lresult = FALSE;
			return lresult;
		}
	}

	if (CurrentSet->bCommDisplay)
	{
		AddStringToStatus("Set DFT BOOT(HDMI) OK");
	}


	return TRUE;
}



BOOL CHdmiGenerator::CheckVer()
{
	CString szCmdString;
	CString szTemp;
	CString szData;
	if (g_nCommandOnlyType == TRUE)
	{
		return TRUE;
	}

	if (g_nGrabberType == FHD_GRABBER)
		return TRUE;


	szTemp = "Check HDMI Ver";
	AddStringToStatus(szTemp);
	m_FW_Ver = "";
#ifdef _DEBUG
	InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
	InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
	m_FW_Ver = "220516";
	return TRUE;
#endif
	if (!CurrentSet->bUseHDMIGen) return TRUE;


	
	szCmdString.Format("%c%c%c", STX, CHECK_FW_VER, ETX);


	if (!SendCommString(szCmdString))
	{
		AddStringToStatus(_T("HDMI Gen Ver. Error"));
		return FALSE;
	}
	if (!ReceiveCommString(500))
	{

		//0x02 0x1A 0x32 0x31 0x31 0x31 0x30 0x31 0x03 
		if (m_nReceiveCount >= 9)
		{
			if ((m_strReceive[0] == 0x02) && (m_strReceive[1] == CHECK_FW_VER) && (m_strReceive[8] == 0x03))
			{
				m_strReceive[8] = 0x00;
				m_FW_Ver = &(m_strReceive[2]);
				AddStringToStatus(m_FW_Ver);
				return TRUE;
			}
		}
		szTemp= "RCV Error:";
		for (int i = 0; i < m_nReceiveCount; i++)
		{
			szData.Format("0x%02X",m_strReceive[i]);
			szTemp += szData;
		}
		AddStringToStatus(szTemp);
		AddStringToStatus("Retry..");

		SendCommString(szCmdString);
		if (!ReceiveCommString(1500))
		{

			//0x02 0x1A 0x32 0x31 0x31 0x31 0x30 0x31 0x03 
			if (m_nReceiveCount >= 9)
			{
				if ((m_strReceive[0] == 0x02) && (m_strReceive[1] == CHECK_FW_VER) && (m_strReceive[8] == 0x03))
				{
					m_strReceive[8] = 0x00;
					m_FW_Ver = &(m_strReceive[2]);
					AddStringToStatus(m_FW_Ver);
					return TRUE;
				}
			}

		}
		szTemp = "Retry RCV Error:";
		for (int i = 0; i < m_nReceiveCount; i++)
		{
			szData.Format("0x%02X", m_strReceive[i]);
			szTemp += szData;
		}
		AddStringToStatus(szTemp);
	}
	AddStringToStatus(_T("HDMI Gen Ver Check. Error"));

	//if (CurrentSet->bCommDisplay)
	//{
	//	AddStringToStatus(szTemp);
	//}

	return FALSE;




}