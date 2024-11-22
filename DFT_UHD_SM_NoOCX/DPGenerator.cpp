// HdmiGenerator.cpp: implementation of the CHdmiGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datsys.h"
#include "DPGenerator.h"

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
CDPGenerator DPGeneratorCtrl;

CDPGenerator::CDPGenerator()
{
	m_bPortOpen = FALSE;
	m_strErrMsg = _T("");
	m_nPortID	= -1;
	m_nModelNo = -1;
	m_nPatternNo = -1;
//	m_nRemoteMode;	//0 : offLine, 1 : online
	

}

CDPGenerator::~CDPGenerator()
{

}
// Open Com Port
BOOL CDPGenerator::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlDPGenerator.OpenPort( sComPort, nBaudRate, m_nPortID,8,0,0))
	{
		m_strErrMsg.Format("[DP GEN.] Failed to open %s port!", sComPort);

		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	_Wait(150);

	return TRUE;
}

void CDPGenerator::PortClose()
{
	m_ctrlDPGenerator.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CDPGenerator::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}


BOOL CDPGenerator::SendByte(BYTE Data)
{
	SendCommString(&Data);
	return TRUE;
}

BOOL CDPGenerator::SendCommString(CString str)
{
	TCHAR *command;

	command = (TCHAR *)(LPCTSTR)str;

	m_ctrlDPGenerator.m_QueueRead.Clear();
	m_ctrlDPGenerator.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());

//	for(int  i =0; i<str.GetLength(); i++)
//	{
//		m_ctrlPatternGenerator.WriteComm(command+i, 1);
//		_Wait(10);
//	}

	return TRUE;
}
BOOL CDPGenerator::ReceiveCommString(int nWaitLimit)
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

	m_ctrlDPGenerator.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{

			dwRead = m_ctrlDPGenerator.ReadComm( buff, 2048);
			if (dwRead == 0){
				Sleep(100);
				dwRead = m_ctrlDPGenerator.ReadComm( buff, 2048);
			}
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlDPGenerator.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlDPGenerator.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				AddStringToStatus("BUFF_SIZE - ..Size() > (int)dwRead)");
				return FALSE;
			}

			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				AddStringToStatus("//Over Limit Time.....False Return");
				int rrv1 = m_ctrlDPGenerator.m_QueueRead.GetSize();
				sTmpBuf = "Data:";
				for (i = 0; i < rrv1; i++)
				{
					m_ctrlDPGenerator.m_QueueRead.GetByte(&buf);
					sTmp.Format("0x02X ", buf);
					sTmpBuf += sTmp;
				}

				AddStringToStatus(sTmpBuf);
				m_ctrlDPGenerator.m_QueueRead.Clear();
				return FALSE;
			}			  
		} while (dwRead); 

		int rrv = m_ctrlDPGenerator.m_QueueRead.GetSize();

		if (rrv > 0)
		{
			for(i=0; i<rrv; i++)
			{
				m_ctrlDPGenerator.m_QueueRead.GetByte(&buf);
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
			m_ctrlDPGenerator.m_QueueRead.Clear();
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
			m_ctrlDPGenerator.m_QueueRead.Clear();
			return FALSE;
		}
	}
	AddStringToStatus("End return TRUE;");
	return TRUE;	
}

BOOL CDPGenerator::SetOnOffLine(BOOL bFlag)
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
		if(Create(CurrentSet->sDpgComPort, CurrentSet->wDpgBaudRate) == FALSE)
		{
			PortClose();
			Create(CurrentSet->sDpgComPort, CurrentSet->wDpgBaudRate);
		}
	}

	bReturn = SendCommString(szCmdString);  

	return bReturn;
}

BOOL CDPGenerator::SetModel(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;


	if(!CurrentSet->bUseDpg) return TRUE;
	
	if(m_nModelNo == nData) return TRUE;

//	SetOnOffLine(TRUE);
	
	m_ctrlDPGenerator.ClearPort();
	
	if(nData < 1 || nData > 999) return FALSE;
	
	szData.Format("%03d", nData);
	szTemp.Format("Set Model(DP) %d", nData);
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
			AddStringToStatus(_T("DP Gen Com. Error"));
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

BOOL CDPGenerator::SetPattern(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	int lreturn = 0;

	if(!CurrentSet->bUseDpg) return TRUE;
	if(m_nPatternNo == nData) return TRUE;

	szData.Format("%03d", nData);
	szTemp.Format("Set Pattern(HDMI) %d", nData);
	szCmdString.Format("%c%c%s%c",STX,CALL_PATTERN,szData,ETX);

//	SetOnOffLine(TRUE);
	
	m_ctrlDPGenerator.ClearPort();
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
			AddStringToStatus(_T("DP Gen Com. Error"));
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

BOOL CDPGenerator::SetModel_Pattern(int nModel, int nPattern)
{
	CString szCmdString;
	CString szData;
	CString szTemp;

	if(!CurrentSet->bUseDpg) return TRUE;

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
		szData.Format("%03d%03d", nModel, nPattern);
	
	szTemp.Format("Set Model&Pattern(DP) %d, %d", nModel, nPattern);
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

BOOL CDPGenerator::SetOut(int nModel, int nPattern)
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


BOOL CDPGenerator::SetOut(int nModel, int nPattern, int Reset)
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


BOOL CDPGenerator::SetDFT_Mode(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	int lresult = 0;

	if (!CurrentSet->bUseDpg) return TRUE;
	

	
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

	m_ctrlDPGenerator.ClearPort();
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

#define DP_60HZ_TIME 0
#define DP_30HZ_TIME 1

BOOL CDPGenerator::SetTime_Control(int nOutPutTime)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	int lresult = 0;
	int nTime = 126; 
	int nPattern = 65;
//Time: 126 - 1920 * 1080, 60Hz

//	Time : 376 - 1920 * 1080, 30Hz
	if (!CurrentSet->bUseDpg) return TRUE;

	SetOnOffLine(TRUE);


	if (nOutPutTime == DP_30HZ_TIME)
	{
		nTime = 376;
		
	}
	else
	{
		nTime = 126;
	}
	
	if (SetModel_Pattern(nTime, nPattern) == 0)//if (SetModel(nTime) == 0)
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CDPGenerator::CheckVer()
{
	CString szCmdString;
	CString szTemp;
	CString szData;

	if (g_nGrabberType == FHD_GRABBER)
		return TRUE;


	szTemp = "Check DP Ver";
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
		AddStringToStatus(_T("DP Gen Ver. Error"));
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
	AddStringToStatus(_T("DP Gen Ver Check. Error"));

	//if (CurrentSet->bCommDisplay)
	//{
	//	AddStringToStatus(szTemp);
	//}

	return FALSE;




}