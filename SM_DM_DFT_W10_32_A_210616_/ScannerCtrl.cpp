
#include "stdafx.h"
#include "DATsys.h"

#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "ScannerCtrl.h"

CScannerCtrl ScannerCtrl;


CScannerCtrl::CScannerCtrl()
{
	m_bPortOpen = FALSE;
	m_nPortID = -1;
	m_strSendStr = _T("");
	m_strErrMsg = _T("");
}

CScannerCtrl::~CScannerCtrl()
{
}

//+Add BY USY
BOOL CScannerCtrl::Create(CString sComPort, DWORD nBaudRate)
{
	CString sTmp = sComPort;
	sTmp.Delete(0, sTmp.Find("COM")+3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");

	if( !m_ctrlScannerCtrl.OpenPort( sComPort, nBaudRate, m_nPortID, 8, 0, 0,TRUE) )
	{
		m_strErrMsg.Format("[Scanner] Failed to open %s port!", sComPort);

		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	_Wait(150);
	return TRUE;
}
//-

void CScannerCtrl::PortClose()
{
	m_ctrlScannerCtrl.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CScannerCtrl::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}


BOOL CScannerCtrl::ReceiveCommString(int nRev, CString& sReadString)
{
//SG msg;
	BOOL		bDoing;
    clock_t		start;
	int			i;
	BYTE		buf;
	DWORD		dwRead;	 // 읽은 바이트수.
	BYTE		buff[2048];	 // 읽기 버퍼

	if(!m_bPortOpen) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();
	m_ctrlScannerCtrl.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{
			dwRead = m_ctrlScannerCtrl.ReadComm( buff, 2048);
				
			if (BUFF_SIZE - m_ctrlScannerCtrl.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlScannerCtrl.m_QueueRead.PutByte(buff[i]);
			}
			else
				AfxMessageBox("m_QueueRead FULL!");
			  
		} while (dwRead); 

		int rrv = m_ctrlScannerCtrl.m_QueueRead.GetSize();

		if (rrv == nRev)
		{
			for(int i=0; i<nRev; i++)
			{
				m_ctrlScannerCtrl.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}
			sReadString.Format("%s", m_strReceive);
//			if(CurrentSet->bCommDisplay)
//			{
//				CString s;
//				s.Format("TV Rs232->Pc : %s",m_strReceive);
//				AddStringToStatus(s);
//			}
			return TRUE;
		}
		

        if ((clock() - start)/CLOCKS_PER_SEC >5) 
        {
			return FALSE;
		}
	}
	
	return TRUE;	
}
//+ 2007.5.7 Add BY USY
BOOL CScannerCtrl::ReceiveCommString(int nRev, CString sWaitLimit, CString& sReadString)
{
//SG msg;
	BOOL		bDoing;
    clock_t		start;
	int			i, nWaitLimit;
	BYTE		buf;
	DWORD		dwRead;	 // Read Byte Count
	BYTE		buff[2048];	 // Read Buffer

	if(!m_bPortOpen) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();
	nWaitLimit = atoi(sWaitLimit);
	m_ctrlScannerCtrl.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{
			dwRead = m_ctrlScannerCtrl.ReadComm( buff, 2048);
				
			if (BUFF_SIZE - m_ctrlScannerCtrl.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlScannerCtrl.m_QueueRead.PutByte(buff[i]);
			}
			else
				AfxMessageBox("m_QueueRead FULL!");
			  
		} while (dwRead); 

		int rrv = m_ctrlScannerCtrl.m_QueueRead.GetSize();

		if (rrv <= nRev)
		{
			for(int i=0; i<nRev; i++)
			{
				m_ctrlScannerCtrl.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}
			sReadString.Format("%s", m_strReceive);
			if(CurrentSet->bCommDisplay)
			{
				CString s;
				s.Format("Scanner->Pc : %s",m_strReceive);
				AddStringToStatus(s);
			}
			return TRUE;
		}
		

        if ((clock() - start)/100000 > nWaitLimit) 
        {//Over Limit Time.....False Return
			return FALSE;
		}
	}
	
	return TRUE;	
}

//-


BOOL CScannerCtrl::SendByte(BYTE Data)
{
	SendCommString(&Data);
	return TRUE;
}

BOOL CScannerCtrl::SendCommString(CString str)
{
	m_ctrlScannerCtrl.m_QueueRead.Clear();
	m_ctrlScannerCtrl.WriteComm((BYTE *)(LPCTSTR)str, str.GetLength());

	return TRUE;
}


BOOL CScannerCtrl::ReceiveCommString(int nRev, int nWaitLimit, CString& sReadString)
{
//SG msg;
	BOOL		bDoing;
    clock_t		start;
	int			i/*, nWaitLimit*/;
	BYTE		buf;
	DWORD		dwRead;	 // Read Byte Count
	BYTE		buff[2048];	 // Read Buffer
	CString		sTmp;
//	int			nValue;
	

	if(!m_bPortOpen) return FALSE;

	memset(m_strReceive, 0, 225);
	bDoing = TRUE;
	start = clock();
//	nWaitLimit = atoi(sWaitLimit);
	m_ctrlScannerCtrl.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{
			dwRead = m_ctrlScannerCtrl.ReadComm( buff, 2048);
				
			if (BUFF_SIZE - m_ctrlScannerCtrl.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlScannerCtrl.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				//sReadString.Format("m_QueueRead FULL!");
				return FALSE;
				//AfxMessageBox("m_QueueRead FULL!");
			}

			//+add kwmoon 071030
			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				m_ctrlScannerCtrl.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_ctrlScannerCtrl.m_QueueRead.GetSize();

		if (rrv >= nRev)
		{
			for(int i=0; i<rrv; i++)
			{
				m_ctrlScannerCtrl.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}
			sTmp.Format("%s", m_strReceive);
			sTmp.Delete(0, sTmp.Find(0x0a, 0) + 1);
			sTmp.TrimRight(0x0a);
			sTmp.TrimRight(0x0d);
			sReadString.Format("%s", sTmp);

			if(CurrentSet->bCommDisplay)
			{
				CString s;
				s.Format("Scanner->Pc : %s",sTmp);
				AddStringToStatus(s);
			}
			m_ctrlScannerCtrl.m_QueueRead.Clear();

			return TRUE;
		}
		
		//+change kwmoon 071030
	//	if ((clock() - start)/100000 > nWaitLimit) 
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			m_ctrlScannerCtrl.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
	return TRUE;	
}
//-



