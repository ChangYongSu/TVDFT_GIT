#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "CurrentSetting.h"
#include "LnbDetectorCtl.h"

extern CCurrentSetting	g_CurSetting;

CLnbDetector gLNBDetectorCtrl;

CLnbDetector::CLnbDetector()
{
	m_bPortOpen = FALSE;
	m_strErrMsg = _T("");
	m_nPortID	= -1;
}

CLnbDetector::~CLnbDetector()
{

}

// Open Com Port
BOOL CLnbDetector::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlLnbDetector.OpenPort( sComPort, nBaudRate, m_nPortID))
	{
		m_strErrMsg.Format("[LNB] Failed to open %s port!", sComPort);

		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	Sleep(150);

	return TRUE;
}

void CLnbDetector::PortClose()
{
	m_ctrlLnbDetector.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CLnbDetector::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}


BOOL CLnbDetector::SendByte(BYTE Data)
{
	SendCommString(&Data);
	return TRUE;
}

BOOL CLnbDetector::SendCommString(CString str)
{
	m_ctrlLnbDetector.m_QueueRead.Clear();
	m_ctrlLnbDetector.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());

	return TRUE;
}
BOOL CLnbDetector::ReceiveString(CString &str, int nWaitLimit)
{
//	MSG msg;
	BOOL		bDoing;
    clock_t		start;
	int			i/*, nWaitLimit*/;
	BYTE		buf;
	DWORD		dwRead;			// Read Byte Count
	BYTE		buff[2048];		// Read Buffer
	CString		sTmp;
	int idx1;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();

	m_ctrlLnbDetector.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{

			dwRead = m_ctrlLnbDetector.ReadComm( buff, 2048);
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlLnbDetector.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlLnbDetector.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				return FALSE;
			}

			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				m_ctrlLnbDetector.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_ctrlLnbDetector.m_QueueRead.GetSize();

		if (rrv > 0)
		{
			for(i=0; i<rrv; i++)
			{
				m_ctrlLnbDetector.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}

			sTmp.Format("%s",m_strReceive);
			str += sTmp;
			
			Sleep(50);

			idx1 = str.Find(_T("\r\n"));
			if(idx1 >= 0){
				m_ctrlLnbDetector.m_QueueRead.Clear();
				return TRUE;
			}
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			m_ctrlLnbDetector.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
	return TRUE;	
}


BOOL CLnbDetector::GetVoltageData(double &dData1, double &dData2)
{
//	CString szCmdString;
	CString szData;
	CString szTemp;
	double dRevData1 = 0.0;
	double dRevData2 = 0.0;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlLnbDetector.ClearPort();
	
//	szCmdString.Format("%c%c%s%c",STX,CALL_MODEL,szData,ETX);
	
	if(!SendCommString(">LV\n"))
	{
		return FALSE;
	}
	if(!ReceiveString(szData, 3000))
	{
		SendCommString(">LV\n");
		if(!ReceiveString(szData,3000))
		{
	        AddStringToStatus(_T("LNB Detector Com. Error"));
			return FALSE;
		}
	}
//	szCmdString.Format("LNB : %s V", szData);
//	AddStringToStatus(szCmdString);
	
	szData.MakeUpper();
	if((szData.Find("CH1-V") != -1) || (szData.Find("CH2-V") != -1)) 
	{
		szData.TrimRight(0x0a);
		szData.TrimRight(0x0d);
		szTemp = szData.Left(szData.Find(",", 0));
	//	szTemp.TrimLeft("CH1-V : ");
		szTemp.Delete(0, szTemp.Find(":", 0) + 1);
		szTemp.TrimLeft(" ");
		szTemp.TrimRight("V");
		dRevData1 = atof(szTemp);

		szData.Delete(0, szData.Find(",", 0) + 1);
	//	szData.TrimLeft("CH2-V : ");
		szData.Delete(0, szData.Find(":", 0) + 1);
		szData.TrimLeft(" ");
		szData.TrimRight("V");
		dRevData2 = atof(szData);

	}
	else{
		szData.TrimRight(0x0a);
		szData.TrimRight(0x0d);
		szData.TrimRight(" V");
		
		dRevData1 = atof(szData);
		dRevData2 = 0.0;
	}
	dData1 = dRevData1;
	dData2 = dRevData2;

	return TRUE;
}

BOOL CLnbDetector::GetFrequencyData(double &dData1, double &dData2)
{
//	CString szCmdString;
	CString szData;
	CString szTemp;
	double dRevData1 = 0.0;
	double dRevData2 = 0.0;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlLnbDetector.ClearPort();
	
//	szCmdString.Format("%c%c%s%c",STX,CALL_MODEL,szData,ETX);
	
	if(!SendCommString(">LF\n"))
	{
		return FALSE;
	}
	if(!ReceiveString(szData, 3000))
	{
		SendCommString(">LF\n");
		if(!ReceiveString(szData,3000))
		{
	        AddStringToStatus(_T("LNB Detector Com. Error"));
			return FALSE;
		}
	}
//	szCmdString.Format("LNB : %s KHz", szData);
//	AddStringToStatus(szCmdString);
	szData.MakeUpper();

	if((szData.Find("CH1-F") != -1) || (szData.Find("CH2-F") != -1)) 
	{
		szData.TrimRight(0x0a);
		szData.TrimRight(0x0d);
		szTemp = szData.Left(szData.Find(",", 0));
	//	szTemp.TrimLeft("CH1-F : ");
		szTemp.Delete(0, szTemp.Find(":", 0) + 1);
		szTemp.TrimLeft(" ");
		szTemp.TrimRight("KHZ");
		dRevData1 = atof(szTemp);

		szData.Delete(0, szData.Find(",", 0) + 1);
	//	szData.TrimLeft("CH2-F : ");
		szData.Delete(0, szData.Find(":", 0) + 1);
		szData.TrimLeft(" ");
		szData.TrimRight("KHZ");
		dRevData2 = atof(szData);

	}
	else{
		szData.TrimRight(0x0a);
		szData.TrimRight(0x0d);
		szData.TrimRight(" KHZ");
		
		dRevData1 = atof(szData);
		dRevData2 = 0.0;
	}

	dData1 = dRevData1;
	dData2 = dRevData2;

	return TRUE;
	
}

