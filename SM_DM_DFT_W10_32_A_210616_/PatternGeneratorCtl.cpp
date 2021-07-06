#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "CurrentSetting.h"
#include "PatternGeneratorCtl.h"

extern CCurrentSetting	g_CurSetting;

CPatternGenerator PatternGeneratorCtrl;

CPatternGenerator::CPatternGenerator()
{
	m_bPortOpen = FALSE;
	m_strErrMsg = _T("");
	m_nPortID	= -1;
	m_nModelNo = -1;
	m_nPatternNo = -1;
}

CPatternGenerator::~CPatternGenerator()
{

}

// Open Com Port
BOOL CPatternGenerator::Create(CString sComPort, DWORD nBaudRate)
{
	CString sTmp = sComPort;
	sTmp.Delete(0, 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlPatternGenerator.OpenPort( sComPort, nBaudRate, m_nPortID))
	{
		m_strErrMsg.Format("[P/G] Failed to open %s port!", sComPort);

		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	_Wait(150);

	return TRUE;
}

void CPatternGenerator::PortClose()
{
	m_ctrlPatternGenerator.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CPatternGenerator::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}


BOOL CPatternGenerator::SendByte(BYTE Data)
{
	SendCommString(&Data);
	return TRUE;
}

BOOL CPatternGenerator::SendCommString(CString str)
{
	m_ctrlPatternGenerator.m_QueueRead.Clear();
	m_ctrlPatternGenerator.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());

	return TRUE;
}

BOOL CPatternGenerator::SendRun(BYTE nCmd, int nData)
{
	CString szCmdString;

	_Wait(100);
		
	char szData[10];
	memset(szData,NULL,10);

	char szModifiedData[10];
	memset(szModifiedData,NULL,10);

	int nDataLength = 0;
	itoa(nData,szData,10);

	switch(nCmd)
	{
		case 0x07 : nDataLength = 3; break;
		case 0x08 : nDataLength = 2; break;
		case 0x09 : nDataLength = 5; break;
	}

	if((nCmd == 0x7) || (nCmd == 0x8) || (nCmd == 0x9))
	{
		memset(szModifiedData,'0',nDataLength);

		for(int i=0; i<(int)strlen(szData); i++)
		{
			szModifiedData[(nDataLength-strlen(szData))+i] = szData[i];
		}

		szCmdString.Format("%c%c%s%c",STX,nCmd,szModifiedData,ETX);
		SendCommString(szCmdString);

		if(CurrentSet->bCommDisplay)
		{
			szCmdString.Format("%s",szData);

			AddStringToStatus("Set Pattern " + szCmdString.Right(2));
		}
		return TRUE;
	}
	else return FALSE;
}

BOOL CPatternGenerator::SetModel(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;


	if(!CurrentSet->bUsePatternGen) return TRUE;
	
	if(m_nModelNo == nData) return TRUE;

	m_ctrlPatternGenerator.ClearPort();
	
	szData.Format("%03d", nData);
	szTemp.Format("Set Model %d", nData);
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
	        AddStringToStatus(_T("PatternGen Com. Error"));
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

BOOL CPatternGenerator::SetPattern(int nData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;

	if(!CurrentSet->bUsePatternGen) return TRUE;
	if(m_nPatternNo == nData) return TRUE;

	szData.Format("%02d", nData);
	szTemp.Format("Set Pattern %d", nData);
	szCmdString.Format("%c%c%s%c",STX,CALL_PATTERN,szData,ETX);

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
	        AddStringToStatus(_T("PatternGen Com. Error"));
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

BOOL CPatternGenerator::SetModel_Pattern(int nModel, int nPattern)
{
	CString szCmdString;
	CString szData;
	CString szTemp;

	if(!CurrentSet->bUsePatternGen) return TRUE;

	szData.Format("%03d%02d", nModel, nPattern);
	szTemp.Format("Set Model&Pattern %d %d", nModel, nPattern);
	szCmdString.Format("%c%c%s%c",STX,CALL_MODEL_PATTERN,szData,ETX);

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
	        AddStringToStatus(_T("PatternGen Com. Error"));
			return FALSE;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(szTemp);
	}
	m_nModelNo = nModel;
	m_nPatternNo = nPattern;

	return TRUE;
}

BOOL CPatternGenerator::SetOut(int nModel, int nPattern)
{
	BOOL bReturn; 
	CString szData;
	int nData;
	
	if(!CurrentSet->bUsePatternGen) return TRUE;

	szData.Format("%03d%02d", nModel, nPattern);
	nData = atoi(szData);

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
	}
	else if((m_nModelNo != nModel) && (m_nPatternNo == nPattern))
	{
		bReturn = SetModel(nModel); 
	}
	else
	{
		bReturn = SetModel_Pattern(nModel, nPattern); 
	}

	return bReturn;
}

BOOL CPatternGenerator::ReceiveCommString(int nWaitLimit)
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
			if((m_strReceive[0] == 0x06) || (m_strReceive[0] == 0x86)){
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

BOOL CPatternGenerator::SetOnOffLine(BOOL bFlag)
{
	BOOL bReturn;
	CString szCmdString = _T("");

	if(bFlag){
		szCmdString.Format("%c",START_TERMINAL_MODE);
	}
	else{
		szCmdString.Format("%c",END_TERMINAL_MODE);
	}

	if(!m_bPortOpen)
	{
		if(Create(CurrentSet->sPatternComPort , CurrentSet->wPatternBaudRate) == FALSE)
		{
			PortClose();
			Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate);
		}
	}

	bReturn = SendCommString(szCmdString);  

	return bReturn;
}
