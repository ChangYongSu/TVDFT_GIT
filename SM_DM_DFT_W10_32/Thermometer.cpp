// 명    칭 : AIAS(Automatic Inline Alignment System)
// 창작일자 : 1997. 2. 21
// 소    속 : LG전자(LG생산기술원 MI Gr. 오구환)
// File  명 : Dmm.cpp

// DMM.cpp : implementation file
//
 

#include "stdafx.h"
#include "DATsys.h"
#include "thermometer.h"
#include "global.h"
#include "realmeas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CThermometer gThermometerCtrl;

CThermometer::CThermometer()
{
	m_nPortID = -1;
	m_bPortOpen = FALSE;
	m_strErrMsg				= _T("");
	m_bCommError = FALSE;
};

CThermometer::~CThermometer()
{

};

BOOL CThermometer::CreateComm(CString sComPort, DWORD nBaudRate)
{
	CString sTmp = sComPort;
	sTmp.Delete(0, sTmp.Find("COM")+3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");

	if( !m_CommCtrl.OpenPort( sComPort, nBaudRate, m_nPortID, 8, NOPARITY, ONESTOPBIT,FALSE) )
	{
		m_strErrMsg.Format("[Thermometer CommPort] \n%s Init Failed!", sComPort);
		m_bPortOpen = FALSE;
		m_bCommError = TRUE;
		return FALSE;
	}
	
	m_bPortOpen = TRUE;
	m_bCommError = FALSE;

	_Wait(150);

	return TRUE;
}
void CThermometer::CloseComm()
{
	m_CommCtrl.ClosePort();
	m_bPortOpen = FALSE;
}
BOOL CThermometer::ReceiveString(CString &str, int nCount)
{
	BOOL		bDoing;
    clock_t		start;
	int			i/*, nWaitLimit*/;
	BYTE		buf;
	DWORD		dwRead;			// Read Byte Count
	BYTE		buff[2048];		// Read Buffer
	CString		sTmp;
	int idx1 = 0;

	if(m_bCommError) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();

	m_CommCtrl.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{

			dwRead = m_CommCtrl.ReadComm( buff, 2048);
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_CommCtrl.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_CommCtrl.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				return FALSE;
			}

			if ((clock() - start)*1000/CLOCKS_PER_SEC > 3000) 
			{//Over Limit Time.....False Return
				m_CommCtrl.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_CommCtrl.m_QueueRead.GetSize();

		if (rrv > 0)
		{
			for(int i=0; i<rrv; i++)
			{
				m_CommCtrl.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}

			sTmp.Format("%s",m_strReceive);
			str += sTmp;
			
			idx1 = idx1 + rrv;

			//idx1 = str.Find(0x03);
			if(idx1 >= 10){
				m_CommCtrl.m_QueueRead.Clear();
				return TRUE;
			}
			Sleep(50);
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > 3000) 
        {//Over Limit Time.....False Return
			m_CommCtrl.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
	return TRUE;	
}


BOOL CThermometer::SendString(CString str)
{
	BYTE *command;
	
	if(m_bCommError) return FALSE;
	
	command = (BYTE*)(LPCTSTR)str;
	
	for (int i = 0; i < (int)_tcslen((char*)command) ; i++) {
		m_CommCtrl.WriteComm(command + i, 1);
	}
	m_CommCtrl.WriteComm((BYTE*)"\r", 1);
	m_CommCtrl.WriteComm((BYTE*)"\n", 1);
	
	return TRUE;
}

double CThermometer::MeasureTh()
{
	CString sReadData;
	CString sValue;
	CString sTemp;
	double	dValue = 0.0;
	int		nValue;
	BYTE	nConfig1;
	BYTE	nConfig2;

	if(!m_bPortOpen) return 0.0;

//	m_strReceive = "";
	m_CommCtrl.m_QueueRead.Clear();
	SendString(_T("A"));

	////20210120 Time //Sleep(500);
	if(ReceiveString(sReadData, 100))
	{
		AddStringToStatus(sReadData);
		nConfig1 = m_strReceive[1];
		nConfig2 = m_strReceive[2];
		sValue.Format("%02x%02x",m_strReceive[3],m_strReceive[4]);
		nValue =  atoi(sValue);
		if((nConfig2 & 0x04) == 0x04){
			dValue = (double)nValue;
		}
		else{
			dValue = (double)nValue/10.0;
		}

		if((nConfig2 & 0x02) == 0x02){
			dValue = dValue *(-1);
		}

		if((nConfig1 & 0x80) != 0x80){
			dValue = (dValue - 32)/1.8;
		}
	}
	sTemp.Format("Measur Temp : %.1f",dValue);
	AddStringToStatus(sTemp);

	return dValue;
}
