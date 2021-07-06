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

}

CHdmiGenerator::~CHdmiGenerator()
{

}
// Open Com Port
BOOL CHdmiGenerator::Create(CString sComPort, DWORD nBaudRate)
{
	CString sTmp = sComPort;
	sTmp.Delete(0, sTmp.Find("COM")+3);
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
	BYTE *command;

	command = (BYTE *)(LPCTSTR)str;

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

	if(m_bPortOpen == FALSE) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();

	m_ctrlPatternGenerator.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{

			dwRead = m_ctrlPatternGenerator.ReadComm( buff, 2048);
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlPatternGenerator.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlPatternGenerator.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				return FALSE;
			}

			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				m_ctrlPatternGenerator.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_ctrlPatternGenerator.m_QueueRead.GetSize();

		if (rrv > 0)
		{
			for(int i=0; i<rrv; i++)
			{
				m_ctrlPatternGenerator.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}

			m_ctrlPatternGenerator.m_QueueRead.Clear();
			if(m_strReceive[0] == 0x06){
				return TRUE;
			}
			else{
				return FALSE;
			}
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			m_ctrlPatternGenerator.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
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
	
	if(nData < 1 || nData > 56) return FALSE;
	
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

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(m_nPatternNo == nData) return TRUE;

	szData.Format("%03d", nData);
	szTemp.Format("Set Pattern(HDMI) %d", nData);
	szCmdString.Format("%c%c%s%c",STX,CALL_PATTERN,szData,ETX);

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
	m_nPatternNo = nData;
	
	return TRUE;
}

BOOL CHdmiGenerator::SetModel_Pattern(int nModel, int nPattern)
{
	CString szCmdString;
	CString szData;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;

	if(!SetModel(nModel))return FALSE;

	Sleep(100);
	
	if(!SetPattern(nPattern)) return FALSE;
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
	szData.Format("%03d %02d", nModel, nPattern);
	szTemp.Format("Set Model&Pattern(HDMI) %d %d", nModel, nPattern);
	szCmdString.Format("%c %c %s %c",STX,CALL_MODEL_PATTERN,szData,ETX);
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
*/
	return TRUE;
}
BOOL CHdmiGenerator::SetEDID_PassCheck(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(bFlag)
	{
		szTemp = "Set HDMI_EDID Pass";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x51,0x31,ETX);
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x51,0x31,ETX);
	}
	else
	{
		szTemp = "Set HDMI_EDID Check";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x51,0x30,ETX);
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x51,0x30,ETX);
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
	m_bEDIDMode = !bFlag;
	return TRUE;

}
BOOL CHdmiGenerator::SetHDCP_OnOff(BOOL bFlag)
{
	CString szCmdString;
	CString szTemp;

	if(!CurrentSet->bUseHDMIGen) return TRUE;
	if(bFlag)
	{
		szTemp = "Set HDMI_HDCP Off";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x50,0x31,ETX);
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x50,0x31,ETX);
	}
	else
	{
		szTemp = "Set HDMI_HDCP On";
//		szCmdString.Format("%c %c %c %c %c %c",STX,0x1c,0x50,0x30,ETX);
		szCmdString.Format("%c%c%c%c%c%c",STX,0x1c,0x50,0x30,ETX);
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
	m_bHDCPMode = !bFlag;
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

	if(!CurrentSet->bUseHDMIGen) return TRUE;

	if(!m_bPortOpen)
	{
		if(Create(CurrentSet->sPatternComPort , CurrentSet->wPatternBaudRate) == FALSE)
		{
			PortClose();
			Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	if((m_nModelNo == nModel) && (m_nPatternNo != nPattern))
	{
		bReturn = SetPattern(nPattern); 
		_Wait(500);
	}
	else if((m_nModelNo != nModel) && (m_nPatternNo == nPattern))
	{
		bReturn = SetModel(nModel); 
		_Wait(500);
	}
	else
	{
		bReturn = SetModel_Pattern(nModel, nPattern); 
		_Wait(500);
	}

	return bReturn;
}