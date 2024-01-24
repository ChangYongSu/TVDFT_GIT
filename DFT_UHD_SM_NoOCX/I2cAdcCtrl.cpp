#include "stdafx.h"
#include "DATsys.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "support.h"
#include "CommThread.h"
#include "I2cAdcCtrl.h"

CI2cAdcCtrl I2cAdcCtrl;

extern CDATsysView * g_pView;
extern CColorStaticST		ctrlStepMeasur_A;

//+add 090219(Modification No2)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

CI2cAdcCtrl::CI2cAdcCtrl()
{
	m_bPortOpen				= FALSE;
	m_nPortID				= -1;
	m_strErrMsg				= _T("");
	m_szI2cAdcReadString	= _T("");
	m_nI2cAdcSeqNo			= 0;
	m_nData1				= 0;
	m_nData2				= 0;
	m_nData3				= 0;

	//+add kwmoon 080804
	m_nReadByte				= 0;

	//+add kwmoon 080826
	memset(m_aReadData,256,0);
	memset(m_aWriteData,256,0);

	m_szCurrentStatusMsg = _T("");
}

CI2cAdcCtrl::~CI2cAdcCtrl()
{
}

void CI2cAdcCtrl::ClearI2cAdcData()
{
	m_szI2cAdcReadString	= _T("");
	m_nI2cAdcSeqNo			= 0;
	m_nData1				= 0;
	m_nData2				= 0;
	m_nData3				= 0;
}

BOOL CI2cAdcCtrl::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");

	if( !m_ctrlI2cAdcCtrl.OpenPort( sComPort, nBaudRate, m_nPortID) )
	{
		m_strErrMsg.Format("[I2C ADC CommPort] \n%s Init Failed!", sComPort);
		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	_Wait(150);

	return TRUE;
}

void CI2cAdcCtrl::PortClose()
{
	m_ctrlI2cAdcCtrl.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CI2cAdcCtrl::Init()
{
	int nResult = TEST_PASS;

	memset(m_strReceive, 0, 1024);

	if(m_bPortOpen == FALSE)
	{
		if(!Create(CurrentSet->sI2cAdcComPort, CurrentSet->wI2cAdcBaudRate)) {return FALSE;}
	}

	if(m_bPortOpen)
	{

		if((nResult = SetI2cClkSpeed(CurrentSet->nI2cClkSpeed)) != TEST_PASS)
		{
			return FALSE;
		}
		
		if((nResult = SetI2cPacketDelay(CurrentSet->nI2cPacketDelay)) != TEST_PASS)
		{
			return FALSE;
		}

		if((nResult = SetI2cReadByteDelay(CurrentSet->nI2cReadByteDelay)) != TEST_PASS)
		{
			return FALSE;
		}

		if((nResult = SetI2cWritByteDelay(CurrentSet->nI2cWriteByteDelay)) != TEST_PASS)
		{
			return FALSE;
		}

	}

	return TRUE;
}

//+change kwmoon 080804
int CI2cAdcCtrl::ReceiveCommString(int nRev, unsigned nWaitLimit, CString& sReadString)
{
	if(m_bPortOpen == FALSE)
	{
		return ERROR_PORT_NOT_OPEN;
	}

	int			nReadQueueSize = 0;

	DWORD		dwRead;		 
	BYTE		Buffer;
	BYTE		ReadBuffer[2048];	

	CString szString = _T("");

	if(!m_bPortOpen) return FALSE;

	memset(ReadBuffer,   0, 2048);
	//+change kwmoon 080919
//	memset(m_strReceive, 0, 225);
	memset(m_strReceive, 0, 1024);

//	nWaitLimit = atoi(sWaitLimit);
	
	DWORD dwStartTick = GetTickCount();

	m_ctrlI2cAdcCtrl.m_QueueRead.Clear();

	while (TRUE)
	{
		while(TRUE)
		{
			dwRead = m_ctrlI2cAdcCtrl.ReadComm( ReadBuffer, 2048);

			if (dwRead == 0){
				Sleep(100);
				dwRead = m_ctrlI2cAdcCtrl.ReadComm( ReadBuffer, 2048);
			}
			if (dwRead == 0) break;
				
			nReadQueueSize = m_ctrlI2cAdcCtrl.m_QueueRead.GetSize();

			if (BUFF_SIZE - nReadQueueSize > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
				{
					m_ctrlI2cAdcCtrl.m_QueueRead.PutByte(ReadBuffer[i]);
				}
			}
			else
			{
				return ERROR_I2C_READ_BUFFER_FULL;
			}

			if((GetTickCount() - dwStartTick) > nWaitLimit)		
			{
				m_ctrlI2cAdcCtrl.m_QueueRead.Clear();
				return ERROR_I2C_READ_TIMEOUT;
			}

			if(CurrentSet->bRunAbort)
			{
				m_ctrlI2cAdcCtrl.m_QueueRead.Clear();
				return ERROR_I2C_USER_STOP;
			}

			if(m_ctrlI2cAdcCtrl.m_QueueRead.GetSize() >= nRev)
			{
				Sleep(10);
				dwRead = m_ctrlI2cAdcCtrl.ReadComm(ReadBuffer, 2048);
				
				break;			
			}
			Sleep(10);
		}

		int rrv = m_ctrlI2cAdcCtrl.m_QueueRead.GetSize();

#if 1 //CYS 20220601
		if ((rrv > 0) && (rrv >= nRev))
		{
			for (int i = 0; i < rrv; i++)
			{
				m_ctrlI2cAdcCtrl.m_QueueRead.GetByte(&Buffer);
				m_strReceive[i] = Buffer;
			}
			sReadString.Format("%s", m_strReceive);

			if (CurrentSet->bCommDisplay)
			{
				szString.Format("Receive :[ %s", m_strReceive);
				szString += "]";
				m_szCurrentStatusMsg.Format("=> Receive : %s", m_strReceive);
				AddStringToStatus(szString);
			}
			//+change kwmoon 080804
		//	return TRUE;
			return TEST_PASS;
		}
#else
		if((rrv >0) && (rrv <= nRev))
		{
			for(int i=0; i<nRev; i++)
			{
				m_ctrlI2cAdcCtrl.m_QueueRead.GetByte(&Buffer);
				m_strReceive[i] = Buffer;
			}
			sReadString.Format("%s", m_strReceive);

			if(CurrentSet->bCommDisplay)
			{
				szString.Format("Receive : %s",m_strReceive);
				m_szCurrentStatusMsg.Format("=> Receive : %s",m_strReceive);
				AddStringToStatus(szString);
			}
			//+change kwmoon 080804
		//	return TRUE;
			return TEST_PASS;
		}
#endif
		if((GetTickCount() - dwStartTick) > nWaitLimit)		
		{	
			m_ctrlI2cAdcCtrl.m_QueueRead.Clear();
			return ERROR_I2C_READ_TIMEOUT;
		}
		
		if(CurrentSet->bRunAbort)
		{
			m_ctrlI2cAdcCtrl.m_QueueRead.Clear();
			return ERROR_I2C_USER_STOP;
		}
		Sleep(10);
	}

	//+change kwmoon 080804
//	return TRUE;	
	return TEST_PASS;
}

int CI2cAdcCtrl::SendI2cCmd(CString szCmdString)
{
	if(m_bPortOpen == FALSE)
	{
		return ERROR_PORT_NOT_OPEN;
	}

	if(!SendCommString(szCmdString))
	{
		if(!SendCommString(szCmdString))
		{
			return ERROR_I2C_SEND;
		}
	}

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus("I2C-ADC : " + szCmdString);
	}

	return TEST_PASS;
}

BOOL CI2cAdcCtrl::SendCommString(CString str)
{
	m_ctrlI2cAdcCtrl.m_QueueRead.Clear();

	int nRtn = 0;
	
	nRtn = m_ctrlI2cAdcCtrl.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());
	
	if(nRtn != str.GetLength()) return FALSE;

	return TRUE;
}

int CI2cAdcCtrl::SetI2cClkSpeed(int nClkSpeed)
{
	CString szCmd		= _T("");
	CString sReadString = _T("");
	int nResult			= TEST_FAIL;

	if((nClkSpeed < 0) || (nClkSpeed > 3))
	{
		return ERROR_I2C_CLK_RANGE;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02x02%02x0000%c%c",m_nI2cAdcSeqNo++, HM_IIC_CTRL_SET_REG, nClkSpeed+1,CR,LF);

	m_szCurrentStatusMsg.Format("Set I2C Clock : Type %d \t\t Send :  %s",nClkSpeed+1,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return ERROR_I2C_RECEIVE;
	}
	else
	{
		if(sReadString.GetLength() >= 7){
			if(sReadString.Mid(1,6).Compare(szCmd.Mid(1,6)) != NULL)
			{
				return ERROR_I2C_RESPONSE;
			}
		}
		else{
			return ERROR_I2C_RESPONSE;
		}

	}
	return TEST_PASS;
}

int CI2cAdcCtrl::SetI2cPacketDelay(int nDelay)
{
	CString szCmd		= _T("");
	CString sReadString = _T("");
	int nResult			= TEST_FAIL;

	if((nDelay < 0) || (nDelay > 500))
	{
		return ERROR_I2C_PACKET_DELAY_RANGE;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02x01%04x00%c%c",m_nI2cAdcSeqNo++, HM_IIC_CTRL_SET_REG, nDelay,CR,LF);

	m_szCurrentStatusMsg.Format("Set Packet Delay : %d \t\t Send :  %s",nDelay,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}
	else
	{
		if(sReadString.GetLength() >= 11){
			if(sReadString.Mid(1,10).Compare(szCmd.Mid(1,10)) != NULL)
			{
				return ERROR_I2C_RESPONSE;
			}
		}
		else{
			return ERROR_I2C_RESPONSE;
		}
	}
	return TEST_PASS;
}

int CI2cAdcCtrl::SetI2cReadByteDelay(int nDelay)
{
	CString szCmd		= _T("");
	CString sReadString = _T("");

	int nResult			= TEST_FAIL;
	
	if((nDelay < 1) || (nDelay > 100))
	{
		return ERROR_I2C_DELAY_RANGE;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x0300%02x00%c%c",m_nI2cAdcSeqNo++, HM_IIC_CTRL_SET_REG, nDelay,CR,LF);

	m_szCurrentStatusMsg.Format("Set Read Delay : %3d \t\t Send :  %s",nDelay,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}
	else
	{
		if(sReadString.GetLength() >= 11){
			if(sReadString.Mid(1,10).Compare(szCmd.Mid(1,10)) != NULL)
			{
				return ERROR_I2C_RESPONSE;
			}
		}
		else{
			return ERROR_I2C_RESPONSE;
		}
	}
	return TEST_PASS;
}

int CI2cAdcCtrl::SetI2cWritByteDelay(int nDelay)
{
	CString szCmd		= _T("");
	CString sReadString = _T("");

	int nResult			= TEST_FAIL;

	if((nDelay < 1) || (nDelay > 100))
	{
		return ERROR_I2C_DELAY_RANGE;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02x0400%02x00%c%c",m_nI2cAdcSeqNo++, HM_IIC_CTRL_SET_REG, nDelay,CR,LF);

	m_szCurrentStatusMsg.Format("Set Write Delay : %3d \t\t Send :  %s",nDelay,szCmd);
	
	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}
	else
	{
		if(sReadString.GetLength() >= 11){
			if(sReadString.Mid(1,10).Compare(szCmd.Mid(1,10)) != NULL)
			{
				return ERROR_I2C_RESPONSE;
			}
		}
		else{
			return ERROR_I2C_RESPONSE;
		}
	}
	return TEST_PASS;
}

int CI2cAdcCtrl::WriteI2c2bData()
{
 	//+add 090219(Modification No2)
	CString szLog = "B:WriteI2c2bData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int		nCmdType	= 0;
	int		nDelay		= 0;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szData1, szData2;
	szData1.Format("%02x",m_nData1);
	szData2.Format("%02x",m_nData2);
	
	int		nReadByte = m_nData3;
	int		nResult	  = TEST_FAIL;

	m_szI2cAdcReadString = _T("");

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02xA0%s%s00%c%c",m_nI2cAdcSeqNo++, HM_DDC2B_WR_BYTE, szData1,szData2,CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	//+del kwmoon 080809
	Sleep(100);

	if((nResult = ReceiveCommString(14,5000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//+add kwmoon 080910
	if(m_szI2cAdcReadString == _T(""))
	{
		return ERROR_I2C_RESPONSE;
	}

	if(m_szI2cAdcReadString.GetLength() >= 7)
	{
		if((m_szI2cAdcReadString.GetAt(5) != '0') 
		|| (m_szI2cAdcReadString.GetAt(6) != '1'))
		{
			return ERROR_I2C_RESPONSE;
		}	
	}
	else{
			return ERROR_I2C_RESPONSE;
	}
 	//+add 090219(Modification No2)
	szLog = "E:WriteI2c2bData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TEST_PASS;
}

int CI2cAdcCtrl::WriteI2c2b_PageData(int nLAdd, CString sData)
{
	int		nCmdType	= 0;
	int		nDelay		= 0;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	int		nResult	  = TEST_FAIL;

	m_szI2cAdcReadString = _T("");

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02xA0%02x0000%c%c",m_nI2cAdcSeqNo++, HM_DDC2B_WR_PAGE, nLAdd,CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	if((SendI2cCmd(sData)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,3000,m_szI2cAdcReadString)) != TEST_PASS){
		return nResult;
	}

	if(m_szI2cAdcReadString == _T(""))
	{
		return ERROR_I2C_RESPONSE;
	}

	if(m_szI2cAdcReadString.GetLength() >= 7)
	{
		if((m_szI2cAdcReadString.GetAt(5) != '0') || (m_szI2cAdcReadString.GetAt(6) != '1'))
		{
			return ERROR_I2C_RESPONSE; 
		}
	}
	else{
			return ERROR_I2C_RESPONSE; 
	}
	
	return TEST_PASS;
}

int CI2cAdcCtrl::WriteI2c2bNbyteData(int nByte, CProgressCtrl& Progress)
{
 	//+add 090219(Modification No2)
	CString szLog = "B:WriteI2c2bNbyteData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int		nCmdType	= 0;
	int		nDelay		= 0;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szData1, szData2;
	szData1.Format("%02x",m_nData1);
	szData2.Format("%02x",m_nData2);
	
	int		nReadByte = m_nData3;
	int		nResult	  = TEST_FAIL;

	for(int i=0; i<nByte; i++)
	{
		nResult	  = TEST_FAIL;

		m_szI2cAdcReadString = _T("");

		if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

		szData1.Format("%02x",m_nData1+i);
		szData2.Format("%02x",m_aWriteData[i]);

		szCmd.Format(">%02x%02xA0%s%s00%c%c",m_nI2cAdcSeqNo++, HM_DDC2B_WR_BYTE, szData1,szData2,CR,LF);

		//+add kwmoon 080826
		m_szCurrentStatusMsg.Format("[2B] Write : Addr:%s Data:%s \t Send :  %s",szData1,szData2,szCmd);

		if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
		{
			return nResult;
		}
		
		//+del kwmoon 080809
	//	Sleep(100);

		if((nResult = ReceiveCommString(14,5000,m_szI2cAdcReadString)) != TEST_PASS)
		{
			return nResult;
		}

		//+add kwmoon 080910
		if(m_szI2cAdcReadString == _T(""))
		{
			return ERROR_I2C_RESPONSE;
		}		
		
		if(m_szI2cAdcReadString.GetLength() >= 7)
		{
			if((m_szI2cAdcReadString.GetAt(5) != '0') 
			|| (m_szI2cAdcReadString.GetAt(6) != '1'))
			{
				return ERROR_I2C_RESPONSE;
			}	
		}
		else{
				return ERROR_I2C_RESPONSE;
		}
		if(i > 0) Progress.SetPos((int)(((float)i*100/(float)(nByte-1))));
	}

 	//+add 090219(Modification No2)
	szLog = "E:WriteI2c2bNbyteData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TEST_PASS;
}

//+add kwmoon 080804
int CI2cAdcCtrl::ReadI2c2bData()
{
 	//+add 090219(Modification No2)
	CString szLog = "B:ReadI2c2bData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int		nCmdType	= 0;
	int		nDelay		= 0;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szData1, szData2;
	szData1.Format("%02x",m_nData1);
	szData2.Format("%02x",m_nData2);
	
	int	nResult	  = TEST_FAIL;

	m_szI2cAdcReadString = _T("");

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02xA1%s0000%c%c",m_nI2cAdcSeqNo++, HM_DDC2B_RD_BYTE, szData1,CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	//+del kwmoon 080809
//	Sleep(100);
	
	if((nResult = ReceiveCommString(14,3000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//================
	// Check Response 
	//================
	//+add kwmoon 080910
	if(m_szI2cAdcReadString == _T(""))
	{
		return ERROR_I2C_RESPONSE;
	}

	if(m_szI2cAdcReadString.GetLength() >= 7)
	{
		if((m_szI2cAdcReadString.GetAt(5) != '0') 
		|| (m_szI2cAdcReadString.GetAt(6) != '1'))
		{
			return ERROR_I2C_RESPONSE;
		}
	}
	else{
			return ERROR_I2C_RESPONSE;
	}

	//=======================================
	// Compare Read Data with Expected Value 
	//=======================================
	szData2.MakeUpper();
	m_szI2cAdcReadString.MakeUpper();

	//+add kwmoon 080910
	if(m_szI2cAdcReadString == _T(""))
	{
		return ERROR_I2C_RESPONSE;
	}
	if(m_szI2cAdcReadString.GetLength() >= 7)
	{
		sTemp.Format("A:%s, V:%c%c",szData1,szData2,m_szI2cAdcReadString.GetAt(7),m_szI2cAdcReadString.GetAt(8));
		ctrlStepMeasur_A.SetWindowText(sTemp); 
		ctrlStepMeasur_A.Invalidate();
		ctrlStepMeasur_A.UpdateWindow();
		
		if((m_szI2cAdcReadString.GetAt(7) != szData2.GetAt(0)) 
		|| (m_szI2cAdcReadString.GetAt(8) != szData2.GetAt(1)))
		{
			pCurStep->m_bAdcTestResult = FALSE;
			return ERROR_I2C_RESPONSE;
		}
	}
	else{
			return ERROR_I2C_RESPONSE;
	}
	pCurStep->m_bAdcTestResult = TRUE;
 	
	//+add 090219(Modification No2)
	szLog = "E:ReadI2c2bData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;
}

//+add kwmoon 080826
int CI2cAdcCtrl::ReadI2c2bNbyteData(int nByte, CProgressCtrl& Progress)
{
 	//+add 090219(Modification No2)
	CString szLog = "B:ReadI2c2bNbyteData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int		nCmdType	= 0;
	int		nDelay		= 0;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szData1, szData2;
	szData1.Format("%02x",m_nData1);
	szData2.Format("%02x",m_nData2);
	
	int	nResult	  = TEST_FAIL;

	//+add kwmoon 080910
	memset(m_aReadData,256,0);
	
	for(int i=0; i<nByte; i++)
	{
		nResult	  = TEST_FAIL;

		m_szI2cAdcReadString = _T("");

		if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

		szData1.Format("%02x",m_nData1+i);

		szCmd.Format(">%02x%02xA1%s0000%c%c",m_nI2cAdcSeqNo++, HM_DDC2B_RD_BYTE, szData1,CR,LF);

		//+add kwmoon 080826
		m_szCurrentStatusMsg.Format("[2B] Read : Addr:%s \t\t Send :  %s",szData1,szCmd);

		if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
		{
			return nResult;
		}

		if((nResult = ReceiveCommString(14,3000,m_szI2cAdcReadString)) != TEST_PASS)
		{
			return nResult;
		}
	
		//+add kwmoon 080910
		if(m_szI2cAdcReadString == _T(""))
		{
			return ERROR_I2C_RESPONSE;
		}

		//+add kwmoon 080910
		if(m_szI2cAdcReadString == _T(""))
		{
			return ERROR_I2C_RESPONSE;
		}
		
		if(m_szI2cAdcReadString.GetLength() >= 7)
		{
			if((m_szI2cAdcReadString.GetAt(5) != '0') 
			|| (m_szI2cAdcReadString.GetAt(6) != '1'))
			{
				return ERROR_I2C_RESPONSE;
			}	
		}
		else{
				return ERROR_I2C_RESPONSE;
		}

		m_aReadData[i] = hexCstr2decNum(m_szI2cAdcReadString.Mid(7,2));

		if(i > 0) Progress.SetPos((int)(((float)i*100/(float)(nByte-1))));
	}

 	//+add 090219(Modification No2)
	szLog = "E:ReadI2c2bNbyteData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;
}

int CI2cAdcCtrl::ReadI2c2bNbyteData2(int nAdd, int nByte)
{
	int		nCmdType	= 0;
	int		nDelay		= 0;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szData1, szData2;
	szData1.Format("%02x",m_nData1);
	szData2.Format("%02x",m_nData2);
	
	int	nResult	  = TEST_FAIL;

	memset(m_aReadData,256,0);
	
	nResult	  = TEST_FAIL;

	m_szI2cAdcReadString = _T("");

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02xA1%02x%02x00%c%c",m_nI2cAdcSeqNo++, HM_DDC2B_RD_CMD, nAdd, nByte,CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//+add kwmoon 080910
	if(m_szI2cAdcReadString == _T(""))
	{
		return ERROR_I2C_RESPONSE;
	}

	//+add kwmoon 080910
	if(m_szI2cAdcReadString == _T(""))
	{
		return ERROR_I2C_RESPONSE;
	}
	
	if(m_szI2cAdcReadString.GetLength() >= 7)
	{
		if((m_szI2cAdcReadString.GetAt(5) != '0') 
		|| (m_szI2cAdcReadString.GetAt(6) != '1'))
		{
			return ERROR_I2C_RESPONSE;
		}	
	}
	else{
			return ERROR_I2C_RESPONSE;
	}


	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//==============================
	// Read N Byte Data From Buffer
	//==============================
	szCmd.Format(">%02x%02x%02x%02x0000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, 0, (nByte-1),CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}	

	m_szI2cAdcReadString = _T("");

	if((nResult = ReceiveCommString(CMD_LENGTH+nByte*2+1,3000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	for(int i=0; i<nByte; i++)
	{
		m_aReadData[i]   = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+i*2,2));
	}

	return TEST_PASS;
}

int CI2cAdcCtrl::CheckI2cValue(int nData1)
{
	//+add 090219(Modification No2)
	CString szLog = "B:CheckI2cValue\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int		nCmdType	= 0;
	int		nDelay		= 0;

	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");
	CString sTargetData = _T("");

	//+add kwmoon 080408
	CString szData1;
	szData1.Format("%02x",nData1);
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	if(m_szI2cAdcReadString == _T(""))
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_EMPTY_READ_STRING;
	}

	szData1.MakeUpper();
	m_szI2cAdcReadString.MakeUpper();

	//+add kwmoon 081013
	sTargetData.Format("%c%c",szData1.GetAt(0),szData1.GetAt(1));

	for(int i=0; i<m_nReadByte; i++)
	{
		//+add kwmoon 081013
		sTemp.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+i),m_szI2cAdcReadString.GetAt(CMD_LENGTH+1+i));
		sReadString += sTemp;

//		if((m_szI2cAdcReadString.GetAt(5+i) != szData1.GetAt(0)) 
//		|| (m_szI2cAdcReadString.GetAt(6+i) != szData1.GetAt(1)))
		if((m_szI2cAdcReadString.GetAt(CMD_LENGTH+i) != szData1.GetAt(0)) 
		|| (m_szI2cAdcReadString.GetAt(CMD_LENGTH+1+i) != szData1.GetAt(1)))
		{
			pCurStep->m_bAdcTestResult = FALSE;

			//+add kwmoon 081013
			pCurStep->m_szAdcValue.Format("Read:%s Exp:%s",sReadString,sTargetData);
			
			return ERROR_I2C_RESPONSE;
		}
	}

	//+add kwmoon 081013
	pCurStep->m_szAdcValue.Format("Read:%s Exp:%s",sReadString,sTargetData);

	pCurStep->m_bAdcTestResult = TRUE;

	sTemp.Format("Read:%s",sReadString);
	ctrlStepMeasur_A.SetWindowText(sReadString); 
	ctrlStepMeasur_A.Invalidate();
	ctrlStepMeasur_A.UpdateWindow();

	//+add 090219(Modification No2)
	szLog = "E:CheckI2cValue\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TEST_PASS;
}

int CI2cAdcCtrl::CheckI2cMaxMin(int nTarget,int nMargin, int nByte)
{
	//+add 090219(Modification No2)
	CString szLog = "B:CheckI2cMaxMin\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	if(m_szI2cAdcReadString == _T(""))
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_EMPTY_READ_STRING;
	}
	
//	if(m_szI2cAdcReadString.GetLength() < 14)
	if(m_szI2cAdcReadString.GetLength() < (CMD_LENGTH + m_nReadByte *2))
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_SHORT_READ_STRING;
	}
	
	int nReadData[4]   = {0,0,0,0};
	CString szReadData = _T("");
	CString sTemp;
/*	
//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(5),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(6));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH),m_szI2cAdcReadString.GetAt(CMD_LENGTH+1));
	nReadData[0] = hexCstr2decNum(szReadData);

//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(7),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(8));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+2),m_szI2cAdcReadString.GetAt(CMD_LENGTH+3));
	nReadData[1] = hexCstr2decNum(szReadData);

//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(9),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(10));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+4),m_szI2cAdcReadString.GetAt(CMD_LENGTH+5));
	nReadData[2] = hexCstr2decNum(szReadData);
*/
	if(nByte == 1){
		//szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH),m_szI2cAdcReadString.GetAt(CMD_LENGTH+1));
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH, 2);
		nReadData[0] = hexCstr2decNum(szReadData);

		//szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+2),m_szI2cAdcReadString.GetAt(CMD_LENGTH+3));
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 2, 2);
		nReadData[1] = hexCstr2decNum(szReadData);

		//szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+4),m_szI2cAdcReadString.GetAt(CMD_LENGTH+5));
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 4, 2);
		nReadData[2] = hexCstr2decNum(szReadData);
	}
	else if(nByte == 2){
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH, 4);
		nReadData[0] = hexCstr2decNum(szReadData);

		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 4, 4);
		nReadData[1] = hexCstr2decNum(szReadData);

		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 8, 4);
		nReadData[2] = hexCstr2decNum(szReadData);
	}
	else{
		return ERROR_I2C_SHORT_READ_STRING;
	}
	

	int nMaxValue = max(max(nReadData[0],nReadData[1]),nReadData[2]);
	int nMinValue = min(min(nReadData[0],nReadData[1]),nReadData[2]);
	int nDiff	  = nMaxValue - nMinValue; 

	//+add kwmoon 081013
	pCurStep->m_szAdcValue.Format("R%d,G%d,B%d",nReadData[0],nReadData[1],nReadData[2]);

	g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, pCurStep->m_szAdcValue);
	pCurStep->m_strMsg = pCurStep->m_szAdcValue;

	sTemp.Format("R%d,G%d,B%d",nReadData[0],nReadData[1],nReadData[2]);
	ctrlStepMeasur_A.SetWindowText(sTemp); 
	ctrlStepMeasur_A.Invalidate();
	ctrlStepMeasur_A.UpdateWindow();

	//+add 090219(Modification No2)
	szLog = "E:CheckI2cMaxMin\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	if((nDiff >= (nTarget-nMargin))
	&& (nDiff <= (nTarget+nMargin)))
	{
		pCurStep->m_bAdcTestResult = TRUE;
		return TEST_PASS;
	}
	else
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return TEST_FAIL;
	}
}

int CI2cAdcCtrl::CheckI2cLimit(int nTarget,int nMargin, int nByte)
{
	//+add 090219(Modification No2)
	CString szLog = "B:CheckI2cLimit\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int nResultValue = 0;

	if(m_szI2cAdcReadString == _T(""))
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_EMPTY_READ_STRING;
	}
	
	if(m_szI2cAdcReadString.GetLength() < (CMD_LENGTH + m_nReadByte *2))
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_SHORT_READ_STRING;
	}

	int nReadData[4]   = {0,0,0,0};
	CString szReadData = _T("");

	if(nByte == 1){
		//szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH),m_szI2cAdcReadString.GetAt(CMD_LENGTH+1));
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH, 2);
		nReadData[0] = hexCstr2decNum(szReadData);

		//szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+2),m_szI2cAdcReadString.GetAt(CMD_LENGTH+3));
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 2, 2);
		nReadData[1] = hexCstr2decNum(szReadData);

		//szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+4),m_szI2cAdcReadString.GetAt(CMD_LENGTH+5));
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 4, 2);
		nReadData[2] = hexCstr2decNum(szReadData);
	}
	else if(nByte == 2){
		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH, 4);
		nReadData[0] = hexCstr2decNum(szReadData);

		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 4, 4);
		nReadData[1] = hexCstr2decNum(szReadData);

		szReadData = m_szI2cAdcReadString.Mid(CMD_LENGTH + 8, 4);
		nReadData[2] = hexCstr2decNum(szReadData);
	}
	else{
		return ERROR_I2C_SHORT_READ_STRING;
	}

	int nMaxValue = max(max(nReadData[0],nReadData[1]),nReadData[2]);
	int nMinValue = min(min(nReadData[0],nReadData[1]),nReadData[2]);

	if(abs(nTarget-nMaxValue) - abs(nTarget-nMinValue) >= 0)
	{
		nResultValue = nMaxValue;
	}
	else
	{
		nResultValue = nMinValue;
	}

	//+add kwmoon 081013
	pCurStep->m_szAdcValue.Format("R%d,G%d,B%d",nReadData[0],nReadData[1],nReadData[2]);
	g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, pCurStep->m_szAdcValue);
	pCurStep->m_strMsg = pCurStep->m_szAdcValue;

	//+add 090219(Modification No2)
	szLog = "E:CheckI2cLimit\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	if((nResultValue >= nTarget-nMargin)
	&& (nResultValue <= nTarget+nMargin))
	{
		pCurStep->m_bAdcTestResult = TRUE;
		return TEST_PASS;
	}
	else
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return TEST_FAIL;
	}
}


int CI2cAdcCtrl::ReadI2cData()
{
	//+add 090219(Modification No2)
	CString szLog = "B:ReadI2cData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int		nCmdType	= 0;
	int		nDelay		= 0;
	int		nResult		= TEST_FAIL;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szAddress;
	szAddress.Format("%02x",m_nData1);
	
	m_nReadByte			 = m_nData3;
	m_szI2cAdcReadString = _T("");

	//========================
	// Check No of Read Bytes
	//========================
//	if((m_nReadByte < 1) || (m_nReadByte > 6))
	if(m_nReadByte < 1)
	{
		return ERROR_I2C_NO_READ_BYTE;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//==================
	// Set Page Address
	//==================
	szCmd.Format(">%02x02E7%s0000%c%c",m_nI2cAdcSeqNo++,szAddress,CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//=======================
	// Read EEPROM (256 Byte)
	//=======================
	szCmd.Format(">%02x2000000000%c%c",m_nI2cAdcSeqNo++,CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	if((nResult = ReceiveCommString(14,5000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//================
	// Response Check
	//================
	if(sReadString.GetLength() >= 7)
	{
		if((sReadString.GetAt(5) != '0') 
		|| (sReadString.GetAt(6) != '1'))
		{
			return ERROR_I2C_RESPONSE;
		}
	}
	else{
			return ERROR_I2C_RESPONSE;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	int nReadByte = m_nReadByte;

	szCmd.Format(">%02x%02x%02x%02x0000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, m_nData2,(nReadByte-1),CR,LF);

	//+add 090219(Modification No2)
	szLog = "E:ReadI2cData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}	
	
	m_szI2cAdcReadString = _T("");

	if((nResult = ReceiveCommString(CMD_LENGTH+nReadByte*2+1,3000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	return TEST_PASS;
}

int CI2cAdcCtrl::ReadI2cNbyteData(int nByte)
{
	int		nCmdType	= 0;
	int		nDelay		= 0;
	int		nResult		= TEST_FAIL;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szAddress;
	szAddress.Format("%02X",m_nData1);
	
	m_nReadByte			 = m_nData3;
	m_szI2cAdcReadString = _T("");


	//========================
	// Check No of Read Bytes
	//========================
//	if((m_nReadByte < 1) || (m_nReadByte > 4))
//	{
//		return ERROR_I2C_NO_READ_BYTE;
//	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//==================
	// Set Page Address
	//==================
	szCmd.Format(">%02x02E7%s0000%c%c",m_nI2cAdcSeqNo++,szAddress,CR,LF);

	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("[2AB] Set Page Address : %s \t Send :  %s",szAddress,szCmd);
	
	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//=======================
	// Read EEPROM (256 Byte)
	//=======================
	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_CMD1, CR,LF);

	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("[2AB] Store Data To Buffer \t\t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	if((nResult = ReceiveCommString(14,5000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//================
	// Response Check
	//================
	if(sReadString.GetLength() >= 7)
	{
		if((sReadString.GetAt(5) != '0') 
		|| (sReadString.GetAt(6) != '1'))
		{
	//		return ERROR_I2C_RESPONSE;
		}
	}
	memset(m_aReadData,256,0);

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//==============================
	// Read N Byte Data From Buffer
	//==============================
	szCmd.Format(">%02x%02x%02x%02x0000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, m_nData2,(nByte-1),CR,LF);

	m_szCurrentStatusMsg.Format("[2AB] Read : %02x + %d bytes \t\t Send :  %s",	 m_nData2,nByte,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}	

	m_szI2cAdcReadString = _T("");

//	if ((nResult = ReceiveCommString(CMD_LENGTH + nByte * 2 + 3, 3000, m_szI2cAdcReadString)) != TEST_PASS)
	if ((nResult = ReceiveCommString(CMD_LENGTH + nByte * 2 + 1, 3000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	for(int i=0; i<nByte; i++)
	{
		m_aReadData[i]   = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+i*2,2));
	}

	return TEST_PASS;
}

int CI2cAdcCtrl::ReadI2cNbyteData2(int nByte)
{
	//+add 090219(Modification No2)
	CString szLog = "B:ReadI2cNbyteData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int		nCmdType	= 0;
	int		nDelay		= 0;
	int		nResult		= TEST_FAIL;
	int nCheckSum			= 0;
	
	CString sMsg		= _T("");
	CString sTemp		= _T("");
	CString sReadData	= _T("");
	CString szCmd		= _T("");
	CString sReadString = _T("");

	CString szAddress;
	szAddress.Format("%02X",m_nData1);
	
	m_nReadByte			 = nByte;

	m_szI2cAdcReadString = _T("");

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;


	
	for(int j = 0; j < 5; j++)
	{
		nResult = WakeUp();
		if(nResult == TEST_PASS) break;
		Sleep(1);
	}

	nCheckSum	= 0x6E ^ 0x50 ^ 0x84 ^ 0x03 ^ 0xE7 ^ m_nData1 ^ m_nData2;


	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1,CR,LF);
	
	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02X%02XE7%02X%02X%02X%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG2,m_nData1,m_nData2, nCheckSum,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//=======================
	// Read EEPROM 
	//=======================
	if(CurrentSet->nAvrFW_Ver == 2){
		szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nByte, CR,LF);
	}
	else{
		szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	}

	m_szCurrentStatusMsg.Format("[2AB] Store Data To Buffer \t\t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//================
	// Response Check
	//================
	if(sReadString.GetLength() >= 7)
	{
		if((sReadString.GetAt(5) != '0') 
		|| (sReadString.GetAt(6) != '1'))
		{
	//		return ERROR_I2C_RESPONSE;
		}
	}
	memset(m_aReadData,256,0);

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//==============================
	// Read N Byte Data From Buffer
	//==============================
	szCmd.Format(">%02x30%02x%02x0000%c%c",m_nI2cAdcSeqNo++,
				  0,(nByte-1),CR,LF);

	m_szCurrentStatusMsg.Format("[2AB] Read : %d bytes \t\t Send :  %s", nByte,szCmd);

	//+add 090219(Modification No2)
	szLog = "E:ReadI2cNbyteData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}	

	m_szI2cAdcReadString = _T("");

	if((nResult = ReceiveCommString(CMD_LENGTH+nByte*2+1,3000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	for(int i=0; i<nByte; i++)
	{
		m_aReadData[i]   = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+i*2,2));
	}

	return TEST_PASS;
}

//+add kwmoon 080807
int CI2cAdcCtrl::SetDDC2AB()
{
	int nResult = TEST_FAIL;

	CString szCmd		= _T("");
	CString sReadString = _T("");
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x016E508403%c%c",m_nI2cAdcSeqNo++,CR,LF);
	
	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("[2AB] Set DDC-2AB \t\t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	//+del kwmoon 080809
//	Sleep(100);

	nResult = ReceiveCommString(14,3000,sReadString);

	return nResult;
}

//+add kwmoon 080807
int CI2cAdcCtrl::SetChannel(CString szData1)
{
	int nResult = TEST_FAIL;

	CString szCmd		= _T("");
	CString sReadString = _T("");
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%s000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_CH_SEL, szData1,CR,LF);
		
	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("Set Channel : %s \t\t\t Send :  %s",szData1,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	//+del kwmoon 080809
//	Sleep(100);

	nResult = ReceiveCommString(14,3000,sReadString);

	return nResult;
}

int CI2cAdcCtrl::SendCmd(CString szData1,CString szData2, CString szData3)
{
	int nResult = TEST_FAIL;

	CString szCmd		= _T("");
	CString sReadString = _T("");
	
	SetDDC2AB();

	//================
	// SetReg4 ~ Reg7
	//================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x02%s%s%s00%c%c",m_nI2cAdcSeqNo++,szData1,szData2, szData3,CR,LF);
	
	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("[2AB] Set Command : %s, %s, %s \t Send :  %s",szData1, szData2, szData3, szCmd);
	
	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//==============
	// Send Command
	//==============
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x1000000000%c%c",m_nI2cAdcSeqNo++,CR,LF);
	
	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("[2AB] Send Command : %s, %s, %s \t Send :  %s",szData1, szData2, szData3,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,5000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(sReadString.GetLength() >= 7)
	{
		if((sReadString.GetAt(5) != '0') 
		|| (sReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}

	return nResult;
}

int CI2cAdcCtrl::ReadCmd(CString szData1, CString szData2, CString szData3)
{
	int nResult = TEST_FAIL;
	int nCheckSum = 0;
	int nToolOption = 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd = _T("");
	CString szTemp = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg = _T("");
	int nHostAdd;


	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
	//	if(CurrentSet->nHostAddressType == 0){
	nHostAdd = HOST_ADD0;
	//	}
	//	else{
	//		nHostAdd = HOST_ADD1;
	//	}

	m_szI2cAdcReadString = _T("");
	//6E 50 84 C9 00 00 checksum 
	//nCheckSum = 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xC9 ^ 0x00;//int nCheckSum			= 0;
	//===========================
	// (Reg0-Reg3 : 6E 50 84 03)
	//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//	SetChannel("00");
	for (int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if (nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG1, nHostAdd, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] Read CMD \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%s%s%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, szData1, szData2, szData3, CR, LF);
	//szCmd.Format(">%02x%02xF50400%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] Read CMD \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 5;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] Read CMD \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 5000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (szReadString.GetLength() >= 7)
	{
		if ((szReadString.GetAt(5) != '0')
			&& (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE;
		}
	}
	else {
		nResult = ERROR_I2C_RESPONSE;
	}

	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] Read CMD \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	return nResult;
}

 int CI2cAdcCtrl::SendCmd2(int nCmd, int nData)
{
	int nCheckSum			= 0;
	CString szCmd		= _T("");
	CString sReadString = _T("");
	int nResult = TEST_FAIL;

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02x6E%02x8403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1, HOST_ADD1, CR,LF);
	
	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x00%02x00%c%c",m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCmd, nData, CR,LF);
	
	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}


	//==============
	// Send Command
	//==============
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_WR_CMD1, CR,LF);
	

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(sReadString.GetLength() >= 7)
	{
		if((sReadString.GetAt(5) != '0') 
		|| (sReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}

	return nResult;
}
int CI2cAdcCtrl::WakeUp()
{
	int nResult = TEST_FAIL;

	CString szCmd		= _T("");
	CString sReadString = _T("");
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_WR_CMD2,CR,LF);
	
	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("Send WakeUp \t\t\t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(sReadString.GetLength() >= 7)
	{
		if((sReadString.GetAt(5) != '0') 
		|| (sReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	return nResult;
}


int CI2cAdcCtrl::ReadOption(int nType, int nOptionNo)
{
	//+add 090219(Modification No2)
	CString szLog = "B:ReadOption\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
	int nToolOption			= 0;
	int nReadByte			= 2; // Tool Option Data : 2Byte
	int nSubCmd				= 0;

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szOptionType = _T("");
	CString szErrMsg	 = _T("");

	m_szI2cAdcReadString = _T("");

	if(nType == TOOL_OPTION)
	{
		if((nOptionNo < 1) || (nOptionNo > 6))
		{
			szErrMsg.LoadString(ERROR_TOOL_OPTION_NO);
			AfxMessageBox(szErrMsg); return TEST_FAIL;
		}
		nSubCmd = 0x05;
		szOptionType.Format("Tool Option%d",nOptionNo);
	}
	else // AREA_OPTION
	{
		if(nOptionNo != 1)
		{
			szErrMsg.LoadString(ERROR_AREA_OPTION_NO);
			AfxMessageBox(szErrMsg); return TEST_FAIL;
		}
		nSubCmd = 0x09;
		szOptionType.Format("Area Option%d",nOptionNo);
	}
	if(nOptionNo >= 5){
		nCheckSum	= 0x6E ^ 0x50 ^ 0x84 ^ 0x03 ^ 0xF5 ^ (nSubCmd + nOptionNo);
	}
	else{
		nCheckSum	= 0x6E ^ 0x50 ^ 0x84 ^ 0x03 ^ 0xF5 ^ (nSubCmd + (nOptionNo-1));
	}

	//===========================
	// (Reg0-Reg3 : 6E 50 84 03)
	//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	WakeUp();

	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1,CR,LF);
	
//	m_szCurrentStatusMsg.Format("[2AB] Set Page Address : %s \t Send :  %s",szAddress,szCmd);
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}


	if((nResult = ReceiveCommString(CMD_LENGTH,3000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : F5 05 00 CS)
	//===========================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//091016
	if(nOptionNo >= 5){
		szCmd.Format(">%02x%02xF5%02x00%02x%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG2,(nSubCmd+nOptionNo), nCheckSum,CR,LF);
	}
	else{
		szCmd.Format(">%02x%02xF5%02x00%02x%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG2,(nSubCmd+(nOptionNo-1)), nCheckSum,CR,LF);
	}
	
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	if(CurrentSet->nAvrFW_Ver == 2){
		szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR,LF);
	}
	else{
		szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	}

	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		|| (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
//	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,SET_I2C_TRANSFER_DATA_CMD,(nReadByte-1),CR,LF);
	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,SET_I2C_TRANSFER_DATA_CMD,nReadByte,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}


//	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+1,5000,m_szI2cAdcReadString)) != TEST_PASS)
	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+3,5000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}
	
	//+add 090219(Modification No2)
	szLog = "E:ReadOption\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return nResult;
}
int CI2cAdcCtrl::ReadOptionAll()
{
	//+add 090219(Modification No2)
	CString szLog = "B:ReadOption\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
//	int nToolOption			= 0;
	int nReadByte			= 12; // Tool Option Data : 2Byte
	int nSubCmd				= 0;

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szOptionType = _T("");
	CString szErrMsg	 = _T("");

	m_szI2cAdcReadString = _T("");

	nSubCmd = 0x0C;

	szOptionType.Format("Tool Option All");

	nCheckSum	= 0x6E ^ 0x50 ^ 0x84 ^ 0x03 ^ 0xF5 ^ nSubCmd;

	//===========================
	// (Reg0-Reg3 : 6E 50 84 03)
	//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	WakeUp();

	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1,CR,LF);
	
//	m_szCurrentStatusMsg.Format("[2AB] Set Page Address : %s \t Send :  %s",szAddress,szCmd);
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}


	if((nResult = ReceiveCommString(CMD_LENGTH,3000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : F5 05 00 CS)
	//===========================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02xF5%02x00%02x%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG2,nSubCmd, nCheckSum,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	if(CurrentSet->nAvrFW_Ver == 2){
		szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR,LF);
	}
	else{
		szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	}

	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		|| (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
//	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,SET_I2C_TRANSFER_DATA_CMD,(nReadByte-1),CR,LF);
	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,SET_I2C_TRANSFER_DATA_CMD,nReadByte,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}


//	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+1,5000,m_szI2cAdcReadString)) != TEST_PASS)
	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+3,5000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}
	
	//+add 090219(Modification No2)
	szLog = "E:ReadOption\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return nResult;
}


int CI2cAdcCtrl::WriteOption(int nType, int nOptionNo,int nOptionValue)
{
	//+add 090219(Modification No2)
	CString szLog = "B:WriteOption\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
	int nToolOption			= 0;
	int nReadByte			= 2; // Tool Option Data : 2Byte
	int nSubCmd				= 0;
	int nLength				= I2C_PACKET_LENGTH; // 0x84
	int nToolOptionHighByte = 0;
	int nToolOptionLowByte  = 0;

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szOptionType = _T("");
	CString szErrMsg	 = _T("");

	m_szI2cAdcReadString = _T("");

	//change 090604
	if(nType == TOOL_OPTION)
	{
		if((nOptionNo < 1) || (nOptionNo > 7))
		{
			szErrMsg.LoadString(ERROR_TOOL_OPTION_NO);
			AfxMessageBox(szErrMsg); return TEST_FAIL;
		}
		nSubCmd = 0x05;
		szOptionType.Format("Tool Option%d",nOptionNo);
	}
	else // AREA_OPTION
	{
		if(nOptionNo != 1)
		{
			szErrMsg.LoadString(ERROR_AREA_OPTION_NO);
			AfxMessageBox(szErrMsg); return TEST_FAIL;
		}
		nSubCmd = 0x09;
		szOptionType.Format("Area Option%d",nOptionNo);
	}

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	WakeUp();
	
	nToolOptionHighByte = (nOptionValue >> 8) & 0xFF;
	nToolOptionLowByte  =  nOptionValue & 0xFF;

	//091016
	if(nOptionNo >= 5){
		nCheckSum	= 0x6E ^ 0x50 ^ (I2C_PACKET_LENGTH+2) ^ 0x03 ^ 0xF6 ^ (nSubCmd + nOptionNo) ^ nToolOptionHighByte ^ nToolOptionLowByte;
	}
	else{
		nCheckSum	= 0x6E ^ 0x50 ^ (I2C_PACKET_LENGTH+2) ^ 0x03 ^ 0xF6 ^ (nSubCmd + (nOptionNo-1)) ^ nToolOptionHighByte ^ nToolOptionLowByte;
	}

	//=================================
	// SET_I2C_WRITE_OPTION_CMD (0x14)
	//=================================
	szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_WRITE_OPTION_CMD,SET_I2C_WRITE_OPTION_SUB_CMD,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// Set Option Data
	//===========================
	//091016
	if(nOptionNo >= 5){
		szCmd.Format("=6E50%02x03F600%02x%04x%02x%c%c",I2C_PACKET_LENGTH+2,(nSubCmd+nOptionNo),nOptionValue,nCheckSum,CR,LF);
	}
	else{
		szCmd.Format("=6E50%02x03F600%02x%04x%02x%c%c",I2C_PACKET_LENGTH+2,(nSubCmd+(nOptionNo-1)),nOptionValue,nCheckSum,CR,LF);
	}
	
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}


	if((nResult = ReceiveCommString(CMD_LENGTH,3000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		|| (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//+add 090219(Modification No2)
	szLog = "E:WriteOption\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return nResult;
}
int CI2cAdcCtrl::WriteOptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6)
{
	CString szLog = "B:WriteOption\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
	int nToolOption			= 0;
	int nReadByte			= 2; // Tool Option Data : 2Byte
	int nSubCmd				= 0;
	int nLength				= I2C_PACKET_LENGTH; // 0x84
	int nHighByte = 0;
	int nLowByte  = 0;
	int nWriteByte  = 20;

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szOptionType = _T("");
	CString szErrMsg	 = _T("");

	m_szI2cAdcReadString = _T("");

	nSubCmd = 0x0C;

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	WakeUp();
	
	nHighByte = (nOption1 >> 8) & 0xFF;
	nLowByte  =  nOption1 & 0xFF;

	nCheckSum	= 0x6E ^ 0x50 ^ (I2C_PACKET_LENGTH+12) ^ 0x03 ^ 0xF6 ^ nSubCmd ^ nHighByte ^ nLowByte;

	nHighByte = (nOption2 >> 8) & 0xFF;
	nLowByte  =  nOption2 & 0xFF;
	nCheckSum	= nCheckSum ^ nHighByte ^ nLowByte;

	nHighByte = (nOption3 >> 8) & 0xFF;
	nLowByte  =  nOption3 & 0xFF;
	nCheckSum	= nCheckSum ^ nHighByte ^ nLowByte;

	nHighByte = (nOption4 >> 8) & 0xFF;
	nLowByte  =  nOption4 & 0xFF;
	nCheckSum	= nCheckSum ^ nHighByte ^ nLowByte;

	nHighByte = (nOption5 >> 8) & 0xFF;
	nLowByte  =  nOption5 & 0xFF;
	nCheckSum	= nCheckSum ^ nHighByte ^ nLowByte;

	nHighByte = (nOption6 >> 8) & 0xFF;
	nLowByte  =  nOption6 & 0xFF;
	nCheckSum	= nCheckSum ^ nHighByte ^ nLowByte;


	//=================================
	// SET_I2C_WRITE_OPTION_CMD (0x14)
	//=================================
//	szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_WRITE_OPTION_CMD,SET_I2C_WRITE_OPTION_SUB_CMD,CR,LF);
	szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_WRITE_OPTION_CMD, nWriteByte ,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Tool Option All \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// Set Option Data
	//===========================
	szCmd.Format("=6E50%02x03F600%02x%04x%04x%04x%04x%04x%04x%02x%c%c",I2C_PACKET_LENGTH+12,nSubCmd,
		                          nOption1,nOption2,nOption3,nOption4,nOption5,nOption6,nCheckSum,CR,LF);

	
	m_szCurrentStatusMsg.Format("[2AB] Tool Option All \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}


	if((nResult = ReceiveCommString(CMD_LENGTH,3000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		|| (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//+add 090219(Modification No2)
	szLog = "E:WriteOption\n";

	return nResult;
}


int CI2cAdcCtrl::ReadVersion()
{
	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
	int nToolOption			= 0;
	int nReadByte			= 12; // Version Data : 9Byte

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg	 = _T("");

	m_szI2cAdcReadString = _T("");

	nCheckSum	= 0x6E ^ 0x50 ^ 0x84 ^ 0x03 ^ 0xF5 ^ 0x04;

	//===========================
	// (Reg0-Reg3 : 6E 50 84 03)
	//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	for(int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if(nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte			 = nReadByte;

	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : F5 04 00 CS)
	//===========================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02xF50400%02x%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG2,nCheckSum,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
//	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	if(CurrentSet->nAvrFW_Ver == 2){
		szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR,LF);
	}
	else{
		szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	}

	

	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		|| (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,HM_IIC_RD_BUF,nReadByte,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+3,5000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}
	
	return nResult;
}

int CI2cAdcCtrl::MNT_ReadVersion()
{
	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
	int nToolOption			= 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg	 = _T("");
	int nHostAdd;


	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
//	if(CurrentSet->nHostAddressType == 0){
		nHostAdd = HOST_ADD0;
//	}
//	else{
//		nHostAdd = HOST_ADD1;
//	}

	m_szI2cAdcReadString = _T("");
//6E 50 84 C9 00 00 checksum 
	nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xC9 ^ 0x00;//int nCheckSum			= 0;
	//===========================
	// (Reg0-Reg3 : 6E 50 84 03)
	//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
//	SetChannel("00");
	for(int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if(nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte			 = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1,nHostAdd,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02xC90000%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);
	//szCmd.Format(">%02x%02xF50400%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	//nReadByte = 10;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++,HM_IIC_RD_CMD3, nReadByte,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		&&(szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,HM_IIC_RD_BUF,nReadByte,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}
	
	return nResult;
}


int CI2cAdcCtrl::MNT_EDID_Check()
{
	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
	int nToolOption			= 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg	 = _T("");
	int nHostAdd;
		//60	DSUB
		//61	SDI
		//80	DVI
		//90	HDMI1
		//91	HDMI2
		//92	HDMI3
		//D0	DP
		//D1	USB-C
		//D2	THUNDERBOLT
		//D3	DP2
	CString sSourceHexStr;
	CString szInputSource;
	szInputSource = CurrentSet->sMNT_Edid_Source;// pCurStep->m_strCurSrc;
	//if (szInputSource.Compare("RGB") == NULL) 
	if ((szInputSource.Compare("RGB") == NULL) || (szInputSource.Compare("VGA") == NULL) || (szInputSource.Compare("DSUB") == NULL))
	{
		sSourceHexStr = "60";
	}
	else if((szInputSource.Compare("SDI") == NULL))
	{
		sSourceHexStr = "61";
	}
	else if ((szInputSource.Compare("DVI") == NULL))
	{
		sSourceHexStr = "80";
	}
	else if ((szInputSource.Compare("HDMI") == NULL)||(szInputSource.Compare("HDMI1") == NULL))
	{
		sSourceHexStr = "90";
	}
	else if ((szInputSource.Compare("HDMI2") == NULL))
	{
		sSourceHexStr = "91";
	}
	else if ((szInputSource.Compare("HDMI3") == NULL))
	{
		sSourceHexStr = "92";
	}
	else if ((szInputSource.Compare("DP") == NULL))
	{
		sSourceHexStr = "D0";
	}
	else if ((szInputSource.Compare("USB-C") == NULL))
	{
		sSourceHexStr = "D1";
	}
	else if ((szInputSource.Compare("THUNDERBOLT") == NULL))
	{
		sSourceHexStr = "D2";
	}
	else if ((szInputSource.Compare("DP2") == NULL))
	{
		sSourceHexStr = "D3";
	}
	else
	{
		return TEST_FAIL;
	}
	

	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
//	if(CurrentSet->nHostAddressType == 0){
		nHostAdd = HOST_ADD0;
//	}
//	else{
//		nHostAdd = HOST_ADD1;
//	}

	m_szI2cAdcReadString = _T("");
//6E 50 84 C9 00 00 checksum 
	//nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xC9 ^ 0x00;//int nCheckSum			= 0;
	//===========================
	// (Reg0-Reg3 : 6E 50 84 03)
	//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
//	SetChannel("00");
	for(int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if(nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte			 = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1,nHostAdd,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Edid Check \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x7A00%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2,sSourceHexStr,  CR, LF);
	//szCmd.Format(">%02x%02xF50400%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] Edid Check \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,2000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 10;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++,HM_IIC_RD_CMD3, nReadByte,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Edid Check \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		&&(szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,HM_IIC_RD_BUF,nReadByte,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+3,2000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (m_szI2cAdcReadString.GetAt(CMD_LENGTH) == '8')
	{
		if (m_szI2cAdcReadString.GetAt(CMD_LENGTH + 3) == '1')
		{
			nResult = TEST_PASS;
			CurrentSet->sMNT_Edid_Source = "";
		}
		else
		{
			nResult = TEST_FAIL;
		}
	}
	else
	{
		nResult = TEST_FAIL;
	}


	return nResult;
}


int CI2cAdcCtrl::MNT_HDCP_Check()
{
	int nResult = TEST_FAIL;
	int nCheckSum = 0;
	int nToolOption = 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd = _T("");
	CString szTemp = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg = _T("");
	int nHostAdd;
	//60	DSUB
	//61	SDI
	//80	DVI
	//90	HDMI1
	//91	HDMI2
	//92	HDMI3
	//D0	DP
	//D1	USB-C
	//D2	THUNDERBOLT
	//D3	DP2
	CString sHDCPHexStr;
	CString szInputSource;
	
	//if (szInputSource.Compare("RGB") == NULL) 
	sHDCPHexStr = "01";
	
	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
	//	if(CurrentSet->nHostAddressType == 0){
	nHostAdd = HOST_ADD0;
	//	}
	//	else{
	//		nHostAdd = HOST_ADD1;
	//	}

	m_szI2cAdcReadString = _T("");
	//6E 50 84 C9 00 00 checksum 
		//nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xCD ^ 0x00;//int nCheckSum			= 0;
		//===========================
		// (Reg0-Reg3 : 6E 50 84 03)
		//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//	SetChannel("00");
	for (int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if (nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG1, nHostAdd, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] HDCP Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02xCD00%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, sHDCPHexStr, CR, LF);
	//szCmd.Format(">%02x%02xF50400%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] HDCP Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 10;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] HDCP Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 5000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (szReadString.GetLength() >= 7)
	{
		if ((szReadString.GetAt(5) != '0')
			&& (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE;
		}
	}
	else {
		nResult = ERROR_I2C_RESPONSE;
	}

	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] HDCP Version \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (m_szI2cAdcReadString.GetAt(CMD_LENGTH) == '8')
	{
		if (m_szI2cAdcReadString.GetAt(CMD_LENGTH + 3) == '1')
		{
			nResult = TEST_PASS;
			
		}
		else
		{
			nResult = TEST_FAIL;
		}
	}
	else
	{
		nResult = TEST_FAIL;
	}


	return nResult;
}


int CI2cAdcCtrl::MNT_FAN_Check()
{
	int nResult = TEST_FAIL;
	int nCheckSum = 0;
	int nToolOption = 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd = _T("");
	CString szTemp = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg = _T("");
	int nHostAdd;
	
	CString sFANHexStr;
	CString szInputSource;
	
	//if (szInputSource.Compare("RGB") == NULL) 
	sFANHexStr = "A1";
	
	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
	//	if(CurrentSet->nHostAddressType == 0){
	nHostAdd = HOST_ADD0;
	//	}
	//	else{
	//		nHostAdd = HOST_ADD1;
	//	}

	m_szI2cAdcReadString = _T("");
	//6E 50 84 C9 00 00 checksum 
		//nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xCD ^ 0x00;//int nCheckSum			= 0;
		//===========================
		// (Reg0-Reg3 : 6E 50 84 03)
		//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//	SetChannel("00");
	for (int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if (nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG1, nHostAdd, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//szCmd.Format(">%02x%02xCD00%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, sFANHexStr, CR, LF);
	szCmd.Format(">%02x%02xA10001%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 5;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 5000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (szReadString.GetLength() >= 7)
	{
		if ((szReadString.GetAt(5) != '0')
			&& (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE;
		}
	}
	else {
		nResult = ERROR_I2C_RESPONSE;
	}

	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN REEAD ACK \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (m_szI2cAdcReadString.GetAt(CMD_LENGTH) == '8')
	{
		if (m_szI2cAdcReadString.GetAt(CMD_LENGTH + 3) == '1')
		{
			nResult = TEST_PASS;
			
		}
		else
		{
			nResult = TEST_FAIL;
		}
	}
	else
	{
		nResult = TEST_FAIL;
	}


	return nResult;
}

int CI2cAdcCtrl::APD_ON_Check()
{
		int		nCmdType = 0;
		int		nDelay = 0;
		int		nData1 = 0;
		int		nData2 = 0;
		int		nData3 = 0;
		int		nResult = TEST_PASS;
		int		nOptionValue = 0;

		CString sMsg = _T("");
		CString sTemp = _T("");
		CString sReadData = _T("");
		CString szCmd = _T("");
		CString sReadString = _T("");
		CString szData1 = _T("");
		CString szData2 = _T("");
		CString szData3 = _T("");
		CString szErrMsg = _T("");

		DWORD dwStartTick = 0;
		DWORD dwFuncTickCount = 0;
		DWORD dwFuncElapsedTime = 0;
		CString szFuncElapsedTime = _T("");

		if (CurrentSet->bSaveProcessingTimeData)
		{
			dwFuncTickCount = GetTickCount();
		}


		I2cAdcCtrl.m_ctrlI2cAdcCtrl.ClearPort();

		if (I2cAdcCtrl.m_nI2cAdcSeqNo >= 0xff) I2cAdcCtrl.m_nI2cAdcSeqNo = 0;

	
		CurrentSet->nAdcType = ADC_TYPE_I2C;
		

		szData1 = "F7";
		szData2 = "01";//// .Format("%02x", nData1);
		//szData2.Format("%02x", nData2);

		sTemp.Format("I2C) Send Command(%s)(%s)", szData1, szData2);
		ShowSubStepMessage(sTemp, _T(""));

		for (int j = 0; j < 5; j++)
		{
			nResult = I2cAdcCtrl.WakeUp();
			if (nResult == TEST_PASS) break;
			Sleep(1);
		}

		if ((nResult = I2cAdcCtrl.SendCmd(szData1, "00", szData2)) != TEST_PASS)
		{
			nResult = TEST_FAIL;// goto END_POSITION;
		}

//		nDelay = 3000;
//		_Wait(nDelay);
		
		if (CurrentSet->bSaveProcessingTimeData)
		{
			DWORD dwStepElapsedTime = GetTickCount() - g_pView->m_dwStepElapsedTime;

			DWORD dwFuncElapsedTime = GetTickCount() - dwFuncTickCount;
			szFuncElapsedTime.Format("   SendI2cCommand [%dms] (%dms)", dwFuncElapsedTime, dwStepElapsedTime);
			AddTimeData(szFuncElapsedTime);
		}

	


	return nResult;
}

//13	ALS Sensor Check	BB	00	00 : NG / 01 : OK	ALS Sensor 의 정상 동작 여부 판단 Get 명령(응답 : 00 = NG / 01 = OK)
//14	BLS Sensor Check	BC	00	00 : NG / 01 : OK	BLS Sensor 의 정상 동작 여부 판단 Get 명령(응답 : 00 = NG / 01 = OK)
//15	Front Sensor Check	BD	00	00 : NG / 01 : OK	Front Sensor Sensor 의 정상 동작 여부 판단 Get 명령(응답 : 00 = NG / 01 = OK)
//16	Proximity Check	BE	00	00 : NG / 01 : OK	Proximity Sensor 의 정상 동작 여부 판단 Get 명령(응답 : 00 = NG / 01 = OK)
int CI2cAdcCtrl::MNT_ALS_Sensor_Check()
{
	int nResult = TEST_FAIL;
	int nCheckSum = 0;
	int nToolOption = 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd = _T("");
	CString szTemp = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg = _T("");
	int nHostAdd;

	CString sCommandHexStr;
	CString szInputSource;
	
	sCommandHexStr = "BB";

	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
	//	if(CurrentSet->nHostAddressType == 0){
	nHostAdd = HOST_ADD0;
	//	}
	//	else{
	//		nHostAdd = HOST_ADD1;
	//	}

	m_szI2cAdcReadString = _T("");
	//6E 50 84 C9 00 00 checksum 
		//nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xCD ^ 0x00;//int nCheckSum			= 0;
		//===========================
		// (Reg0-Reg3 : 6E 50 84 03)
		//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//	SetChannel("00");
	for (int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if (nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG1, nHostAdd, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//szCmd.Format(">%02x%02xCD00%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, sFANHexStr, CR, LF);
	szCmd.Format(">%02x%02xA10001%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 5;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 5000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (szReadString.GetLength() >= 7)
	{
		if ((szReadString.GetAt(5) != '0')
			&& (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE;
		}
	}
	else {
		nResult = ERROR_I2C_RESPONSE;
	}

	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN REEAD ACK \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (m_szI2cAdcReadString.GetAt(CMD_LENGTH) == '0')
	{
		if (m_szI2cAdcReadString.GetAt(CMD_LENGTH + 1) == '1')
		{
			nResult = TEST_PASS;

		}
		else
		{
			nResult = TEST_FAIL;
		}
	}
	else
	{
		nResult = TEST_FAIL;
	}


	return nResult;
}
int CI2cAdcCtrl::MNT_BLS_Sensor_Check()
{
	int nResult = TEST_FAIL;
	int nCheckSum = 0;
	int nToolOption = 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd = _T("");
	CString szTemp = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg = _T("");
	int nHostAdd;

	CString sCommandHexStr;
	CString szInputSource;

	//if (szInputSource.Compare("RGB") == NULL) 
	sCommandHexStr = "BC";

	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
	//	if(CurrentSet->nHostAddressType == 0){
	nHostAdd = HOST_ADD0;
	//	}
	//	else{
	//		nHostAdd = HOST_ADD1;
	//	}

	m_szI2cAdcReadString = _T("");
	//6E 50 84 C9 00 00 checksum 
		//nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xCD ^ 0x00;//int nCheckSum			= 0;
		//===========================
		// (Reg0-Reg3 : 6E 50 84 03)
		//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//	SetChannel("00");
	for (int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if (nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG1, nHostAdd, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//szCmd.Format(">%02x%02xCD00%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, sFANHexStr, CR, LF);
	szCmd.Format(">%02x%02xA10001%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 5;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 5000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (szReadString.GetLength() >= 7)
	{
		if ((szReadString.GetAt(5) != '0')
			&& (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE;
		}
	}
	else {
		nResult = ERROR_I2C_RESPONSE;
	}

	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN REEAD ACK \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (m_szI2cAdcReadString.GetAt(CMD_LENGTH) == '0')
	{
		if (m_szI2cAdcReadString.GetAt(CMD_LENGTH + 1) == '1')
		{
			nResult = TEST_PASS;

		}
		else
		{
			nResult = TEST_FAIL;
		}
	}
	else
	{
		nResult = TEST_FAIL;
	}


	return nResult;
}
int CI2cAdcCtrl::MNT_Front_Sensor_Check()
{
	int nResult = TEST_FAIL;
	int nCheckSum = 0;
	int nToolOption = 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd = _T("");
	CString szTemp = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg = _T("");
	int nHostAdd;

	CString sCommandHexStr;
	CString szInputSource;

	//if (szInputSource.Compare("RGB") == NULL) 
	sCommandHexStr = "BD";

	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
	//	if(CurrentSet->nHostAddressType == 0){
	nHostAdd = HOST_ADD0;
	//	}
	//	else{
	//		nHostAdd = HOST_ADD1;
	//	}

	m_szI2cAdcReadString = _T("");
	//6E 50 84 C9 00 00 checksum 
		//nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xCD ^ 0x00;//int nCheckSum			= 0;
		//===========================
		// (Reg0-Reg3 : 6E 50 84 03)
		//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//	SetChannel("00");
	for (int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if (nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG1, nHostAdd, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//szCmd.Format(">%02x%02xCD00%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, sFANHexStr, CR, LF);
	szCmd.Format(">%02x%02xA10001%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 5;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 5000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (szReadString.GetLength() >= 7)
	{
		if ((szReadString.GetAt(5) != '0')
			&& (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE;
		}
	}
	else {
		nResult = ERROR_I2C_RESPONSE;
	}

	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN REEAD ACK \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (m_szI2cAdcReadString.GetAt(CMD_LENGTH) == '0')
	{
		if (m_szI2cAdcReadString.GetAt(CMD_LENGTH + 1) == '1')
		{
			nResult = TEST_PASS;

		}
		else
		{
			nResult = TEST_FAIL;
		}
	}
	else
	{
		nResult = TEST_FAIL;
	}


	return nResult;
}
int CI2cAdcCtrl::MNT_Proximity_Check()
{
	int nResult = TEST_FAIL;
	int nCheckSum = 0;
	int nToolOption = 0;

	//int nReadByte = 2; // Version Data : 9Byte
	int nReadByte = 2; // Version Data : 9Byte

	CString szCmd = _T("");
	CString szTemp = _T("");
	CString szReadString = _T("");
	CString szToolOption = _T("");
	CString szErrMsg = _T("");
	int nHostAdd;

	CString sCommandHexStr;
	CString szInputSource;

	//if (szInputSource.Compare("RGB") == NULL) 
	sCommandHexStr = "BE";
	if (m_MicomVer_Ex >= 1)
	{
		nReadByte = 3;
	}
	
	//	if(CurrentSet->nHostAddressType == 0){
	nHostAdd = HOST_ADD0;
	//	}
	//	else{
	//		nHostAdd = HOST_ADD1;
	//	}

	m_szI2cAdcReadString = _T("");
	//6E 50 84 C9 00 00 checksum 
		//nCheckSum	= 0x6E ^ nHostAdd ^ 0x84 ^ 0x03 ^ 0xCD ^ 0x00;//int nCheckSum			= 0;
		//===========================
		// (Reg0-Reg3 : 6E 50 84 03)
		//===========================
	m_ctrlI2cAdcCtrl.ClearPort();
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//	SetChannel("00");
	for (int i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if (nResult == TEST_PASS) break;
		Sleep(1);
	}

	m_nReadByte = nReadByte;

	szCmd.Format(">%02x%02x6E%02X8403%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG1, nHostAdd, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : C9 00 00 CS)
	//===========================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	//szCmd.Format(">%02x%02xCD00%s00%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, sFANHexStr, CR, LF);
	szCmd.Format(">%02x%02xA10001%02x%c%c", m_nI2cAdcSeqNo++, HM_IIC_WR_REG2, nCheckSum, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 2000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}


	nReadByte = 5;
	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x%02x000000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN Check \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH, 5000, szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (szReadString.GetLength() >= 7)
	{
		if ((szReadString.GetAt(5) != '0')
			&& (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE;
		}
	}
	else {
		nResult = ERROR_I2C_RESPONSE;
	}

	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if (m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c", m_nI2cAdcSeqNo++, HM_IIC_RD_BUF, nReadByte, CR, LF);

	m_szCurrentStatusMsg.Format("[2AB] FAN REEAD ACK \t Send :  %s", szCmd);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(CMD_LENGTH + (nReadByte * 2) + 3, 2000, m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if (m_szI2cAdcReadString.GetAt(CMD_LENGTH) == '0')
	{
		if (m_szI2cAdcReadString.GetAt(CMD_LENGTH + 1) == '1')
		{
			nResult = TEST_PASS;

		}
		else
		{
			nResult = TEST_FAIL;
		}
	}
	else
	{
		nResult = TEST_FAIL;
	}


	return nResult;
}


BOOL CI2cAdcCtrl::GetModuleVoltage(double& dVolt)
{
	CString szCmd		= _T("");
	CString sReadString = _T("");
	int nResult			= TEST_FAIL;
	
	int nMeasureValue  = 0;
	
	double fMeasuredValue = 0.0;

	dVolt = fMeasuredValue; 

	szCmd.Format(">00A800000000%c%c",CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return FALSE;
	}

	if((nResult = ReceiveCommString(14,3000,sReadString)) != TEST_PASS)
	{
		return FALSE;
	}
	if(sReadString.GetLength() >= 7)
	{
		if((sReadString.GetAt(5) != '0') 
		|| (sReadString.GetAt(6) != '1'))
		{
			return FALSE;
		}
	}
	else{
			return FALSE;
	}

	nMeasureValue = (int)(hexCstr2decNum(sReadString.Mid(7,4)));
	fMeasuredValue = nMeasureValue * 0.01;

	dVolt = fMeasuredValue; 

	return TRUE; 
}


//+add 090420(Modification No1)
int CI2cAdcCtrl::SetExtAnalogGrabberMode(int nAnalogGrabberMode)
{
	CString szCmd		= _T("");
	CString sReadString = _T("");

	int nResult	= TEST_FAIL;
	
	switch(nAnalogGrabberMode)
	{
		case V_SRC_NTSC : case V_SRC_PAL :
		{
			szCmd.Format(">00A0480E0700%c%c",CR,LF);
		}
		break;

		case V_SRC_SECAM :
		{
			szCmd.Format(">00A0480E5700%c%c",CR,LF);
		}
		break;
	}

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	nResult = ReceiveCommString(14,3000,sReadString);
	
	return nResult;
}

//+add 090604(Modification No1)
BOOL CI2cAdcCtrl::Check_Option(int nIndex, int nOptionValue)
{
	CString sData;
//	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nType;
	int nOptionNo;
	int nReadToolOption;
	int  nI2cResult;
	int i;

	switch(nIndex)
	{
		case TOOL_OPTION1:
			nType = TOOL_OPTION;
			nOptionNo = 1;
			break;

		case TOOL_OPTION2:
			nType = TOOL_OPTION;
			nOptionNo = 2;
			break;
		case TOOL_OPTION3:
			nType = TOOL_OPTION;
			nOptionNo = 3;
			break;
		case TOOL_OPTION4:
			nType = TOOL_OPTION;
			nOptionNo = 4;
			break;
		case TOOL_OPTION5:
			nType = TOOL_OPTION;
			nOptionNo = 5;
			break;
		case TOOL_OPTION6:
			nType = TOOL_OPTION;
			nOptionNo = 6;
			break;
		case AREA_OPTION1:
			nType = AREA_OPTION;
			nOptionNo = 1;
			break;

		default :
			return FALSE;
			break;
	}
	for(i= 0; i<5; i++)
	{
		nI2cResult = ReadOption(nType,nOptionNo);
		if(nI2cResult == TEST_PASS) break;
		_Wait(200);
	}

	if(nI2cResult != TEST_PASS)
	{
		return FALSE;
	}
	else
	{
		nReadToolOption = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH,4));
	}

	if(nType == TOOL_OPTION){
		sData.Format("Tool Option%d(Read):%d", nOptionNo, nReadToolOption);
		szMsg1.Format("TO%d:%d", nOptionNo, nReadToolOption);
	}
	else{
		sData.Format("Area Option%d(Read):%d", nOptionNo, nReadToolOption);
		szMsg1.Format("AO%d(Read):%d", nOptionNo, nReadToolOption);
	}
	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(sData);
	}

	if(CurrentSet->bIsRunning)
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		
	}

	if(nOptionValue == nReadToolOption){
		return TRUE;
	}
	else{
		return FALSE;
	}

}

BOOL CI2cAdcCtrl::Check_OptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6)
{
	CString sData;
//	int nTemp;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");
	int nROption1;
	int nROption2;
	int nROption3;
	int nROption4;
	int nROption5;
	int nROption6;
	int  nI2cResult;
	int i;

	for(i= 0; i<5; i++)
	{
		nI2cResult = ReadOptionAll();
		if(nI2cResult == TEST_PASS) break;
		_Wait(200);
	}

	if(nI2cResult != TEST_PASS)
	{
		return FALSE;
	}
	else
	{
		nROption1 = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH,4));
		nROption2 = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+4,4));
		nROption3 = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+8,4));
		nROption4 = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+12,4));
		nROption5 = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+16,4));
		nROption6 = hexCstr2decNum(m_szI2cAdcReadString.Mid(CMD_LENGTH+20,4));
	}

	sData.Format("Tool OptionAll(Read):%d %d %d %d %d %d", nROption1, nROption2, nROption3, nROption4, nROption5, nROption6);
	szMsg1.Format("TO::%d %d %d %d %d %d", nROption1, nROption2, nROption3, nROption4, nROption5, nROption6);;

	if(CurrentSet->bCommDisplay)
	{
		AddStringToStatus(sData);
	}

	if (CurrentSet->bIsRunning)
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
			pCurStep->m_strMsg = szMsg1;
		}		
	}

	if((nOption1 == nROption1) && (nOption2 == nROption2) && (nOption3 == nROption3)
		 && (nOption4 == nROption4) && (nOption5 == nROption5) && (nOption6 == nROption6)){
		return TRUE;
	}
	else{
		return FALSE;
	}

}


BOOL CI2cAdcCtrl::InputSourceChange(BYTE nCode)
{
	CString szCode = _T("");

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	WakeUp();

	szCode.Format("%02x",nCode); 
	if(SendCmd("F4","00",szCode) != TEST_PASS)
	{
		return FALSE;
	}	
	return TRUE; 	
}

//add 100101
BOOL CI2cAdcCtrl::SetIRcode(BYTE nCode)
{
	CString szCode = _T("");

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	WakeUp();

	szCode.Format("%02x",nCode); 
	if(SendCmd("60","00",szCode) != TEST_PASS)
	{
		return FALSE;
	}	
	return TRUE; 	
}


BOOL CI2cAdcCtrl::EDID_Download_ResultCheck()
{
	int nResult				=	 TEST_FAIL;
	int nCheckSum			= 0;
	int nReadByte			= 1; 

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");
	CString szErrMsg	 = _T("");

	m_szI2cAdcReadString = _T("");

	nCheckSum	= 0x6E ^ 0x50 ^ 0x84 ^ 0x03 ^ 0xF5 ^ 0x0A;

	//===========================
	// (Reg0-Reg3 : 6E 50 84 03)
	//===========================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG1,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read EDID D/L Check \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,3000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//===========================
	// (Reg4-Reg7 : F5 04 00 CS)
	//===========================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02xF50A00%02x%c%c",m_nI2cAdcSeqNo++,HM_IIC_WR_REG2,nCheckSum,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read EDID D/L Check \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//=================================
	// Tool Option Read Command (0x24)
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	if(CurrentSet->nAvrFW_Ver == 2){
		szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_RD_CMD3, nReadByte, CR,LF);
	}
	else{
		szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	}
	m_szCurrentStatusMsg.Format("[2AB] Read EDID D/L Check \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH,5000,szReadString)) != TEST_PASS)
	{
		return nResult;
	}

	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') 
		|| (szReadString.GetAt(6) != '1'))
		{
			nResult = ERROR_I2C_RESPONSE; 
		}
	}
	else{
			nResult = ERROR_I2C_RESPONSE; 
	}
		
	//=================================
	// Tool Option Read Command (0x30)
	// Read nReadByte
	//=================================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,SET_I2C_TRANSFER_DATA_CMD,(nReadByte-1),CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+3,5000,m_szI2cAdcReadString)) != TEST_PASS)
	{
		return nResult;
	}

	return nResult;
}

//add 090611
int CI2cAdcCtrl::EDID_Download()
{
	CString sTemp;
	int		nResult		 = TEST_PASS;
	CString szData;

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
//	SetChannel("00");
	WakeUp();

	sTemp.Format("I2C EDID Download Start");
	ShowSubStepMessage(sTemp, _T(""));
	if (CurrentSet->bIsRunning)
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sTemp);
	}
	if((nResult = SendCmd("F6", "00", "0A")) != TEST_PASS)
	{
		return nResult;
	}

	_Wait(2000);
	DWORD dwStartTick = GetTickCount();

	while (TRUE)
	{
		nResult = EDID_Download_ResultCheck();
		if(nResult == TEST_PASS)
		{
			szData = m_szI2cAdcReadString.Mid(CMD_LENGTH,2);
			szData.MakeUpper();

			if(CurrentSet->bCommDisplay)
			{
				sTemp.Format("I2C EDID D/L Check : %s",szData);

				AddStringToStatus(sTemp);
			}

			if(szData.Compare("AA") == 0)
			{
				return TEST_PASS;
			}
		}

		if((GetTickCount() - dwStartTick) > 5000)		
		{	
			return ERROR_I2C_READ_TIMEOUT;
		}
		
		if(CurrentSet->bRunAbort)
		{
			return ERROR_I2C_USER_STOP;
		}
		_Wait(200);

	}

	return nResult;
}


BOOL CI2cAdcCtrl::WriteI2c_Data(CString sData, int nWriteByte)
{
	CString szCmd		 = _T("");
	CString szReadString = _T("");

	m_ctrlI2cAdcCtrl.ClearPort();

//	szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_WRITE_OPTION_CMD,SET_I2C_WRITE_OPTION_SUB_CMD,CR,LF);
	szCmd.Format(">%02x%02x%02x000000%c%c",m_nI2cAdcSeqNo++, HM_IIC_WR_CMD3, nWriteByte,CR,LF);
	if((SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return FALSE;
	}

	Sleep(5);

	if((SendI2cCmd(sData)) != TEST_PASS)
	{
		return FALSE;
	}


	if((ReceiveCommString(CMD_LENGTH,1500,szReadString)) != TEST_PASS){
		return FALSE;
	}
	if(szReadString.GetLength() >= 7)
	{
		if((szReadString.GetAt(5) != '0') || (szReadString.GetAt(6) != '1'))
		{
			return FALSE; 
		}
	}
	else{
			return FALSE; 
	}
	
	return TRUE;
}

BOOL CI2cAdcCtrl::WriteHdcpKey()
{
	BOOL bResult			= FALSE;
	int nResult				= TEST_FAIL;
	int nLength				= I2C_PACKET_LENGTH; // 0x84

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");

	BYTE nWriteCmd		= 0xE8;

	BYTE nCheckSum;
	BYTE nADDH1;
	BYTE nADDL1;
	BYTE nADDH2;
	BYTE nADDL2;
	BYTE nADDH3;
	BYTE nADDL3;
	BYTE nDATA;
	int  nSize3 = 0;
	int i,j, k;
	div_t div_result;
	int nHostAdd;

//	if(CurrentSet->nHostAddressType == 0){
		nHostAdd = HOST_ADD0;
//	}
//	else{
//		nHostAdd = HOST_ADD1;
//	}

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
//	SetChannel("00");
	for(i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if(nResult == TEST_PASS) break;
		Sleep(1);
	}	
	nADDH1 = CurrentSet->nHDCP_Start_ADH;
	nADDL1 = CurrentSet->nHDCP_Start_ADL;

	if(nADDL1 == 0x80){
		nADDH2 = nADDH1 + 0x02;
		nADDL2 = 0x00;
		nADDH3 = nADDH2;
		nADDL3 = 0x80;
	}
	else{
		nADDH2 = nADDH1;
		nADDL2 = 0x80;
		nADDH3 = nADDH1 + 0x02;
		nADDL3 = 0x00;
	}
	nSize3 = CurrentSet->nHDCP_Data_Size - 256;
	div_result = div(nSize3, 16);

	for(i = 0; i < 8; i++)
	{
		nCheckSum	= 0x6E ^ nHostAdd ^ (I2C_PACKET_LENGTH+16) ^ 0x03 ^ 0xE8 ^ nADDH1 ^ (nADDL1 + 16* i) ;
		szCmd.Format("=6E%02X%02X03%02X%02X%02X",nHostAdd,I2C_PACKET_LENGTH+16,nWriteCmd, nADDH1, (nADDL1 +16* i));

		for(j = 0; j < 16; j++)
		{
			nDATA = m_nHdcpKeyWriteData[16* i + j];
			nCheckSum = nCheckSum ^ nDATA;
			szTemp.Format("%02X",nDATA);
			szCmd = szCmd + szTemp;
		} 
		szTemp.Format("%02X%c%c",nCheckSum,CR,LF);
		szCmd = szCmd + szTemp;

		for(k =0; k < 5; k++)
		{
			bResult = WriteI2c_Data(szCmd, 24);
			if(bResult) 
			{
				Sleep(50);
				break;
			}
			else{
				Sleep(250);
			}

		}
		if(!bResult){ return FALSE;}
	}

	for(i = 0; i < 8; i++)
	{
		nCheckSum	= 0x6E ^ nHostAdd ^ (I2C_PACKET_LENGTH+16) ^ 0x03 ^ 0xE8 ^ nADDH2 ^ (nADDL2 + 16* i);
		szCmd.Format("=6E%02X%02X03%02X%02X%02X",nHostAdd,I2C_PACKET_LENGTH+16,nWriteCmd, nADDH2, (nADDL2 + 16* i));

		for(j = 0; j < 16; j++)
		{
			nDATA = m_nHdcpKeyWriteData[128+ 16* i + j];
			nCheckSum = nCheckSum ^ nDATA;
			szTemp.Format("%02X",nDATA);
			szCmd = szCmd + szTemp;
		}
		szTemp.Format("%02X%c%c",nCheckSum,CR,LF);
		szCmd = szCmd + szTemp;

		for(k =0; k < 5; k++){
			bResult = WriteI2c_Data(szCmd, 24);
			if(bResult) 
			{
				Sleep(50);
				break;
			}
			else{
				Sleep(250);
			}

		}
		if(!bResult){ return FALSE;}
	}
	for(i = 0; i < div_result.quot; i++)
	{
		nCheckSum	= 0x6E ^ nHostAdd ^ (I2C_PACKET_LENGTH+16) ^ 0x03 ^ 0xE8 ^ nADDH3 ^ (nADDL3 + 16* i);
		szCmd.Format("=6E%02X%02X03%02X%02X%02X",nHostAdd,I2C_PACKET_LENGTH+16,nWriteCmd, nADDH3, (nADDL3 + 16* i));

		for(j = 0; j < 16; j++)
		{
			nDATA = m_nHdcpKeyWriteData[256+ 16* i + j];
			nCheckSum = nCheckSum ^ nDATA;
			szTemp.Format("%02X",nDATA);
			szCmd = szCmd + szTemp;
		}
		szTemp.Format("%02X%c%c",nCheckSum,CR,LF);
		szCmd = szCmd + szTemp;

		for(k =0; k < 5; k++){
			bResult = WriteI2c_Data(szCmd, 24);
			if(bResult) 
			{
				Sleep(50);
				break;
			}
			else{
				Sleep(250);
			}

		}
		if(!bResult){ return FALSE;}
	}
	
	nCheckSum	= 0x6E ^ nHostAdd ^ (I2C_PACKET_LENGTH+div_result.rem) ^ 0x03 ^ 0xE8 ^ nADDH3 ^ (nADDL3 + 16* div_result.quot);
	szCmd.Format("=6E%02X%02X03%02X%02X%02X",nHostAdd,I2C_PACKET_LENGTH+div_result.rem ,nWriteCmd, nADDH3, (nADDL3 + 16* div_result.quot));

	for(j = 0; j < div_result.rem ; j++)
	{
		nDATA = m_nHdcpKeyWriteData[256+ 16* div_result.quot + j];
		nCheckSum = nCheckSum ^ nDATA;
		szTemp.Format("%02X",nDATA);
		szCmd = szCmd + szTemp;
	}
	szTemp.Format("%02X%c%c",nCheckSum,CR,LF);
	szCmd = szCmd + szTemp;

	for(k =0; k < 5; k++){
		bResult = WriteI2c_Data(szCmd, (8 + div_result.rem));
		if(bResult) 
		{
			Sleep(50);
			break;
		}
		else{
			Sleep(250);
		}

	}
	if(!bResult){ return FALSE;}

	return TRUE;
}

int CI2cAdcCtrl::ReadHdcpKey()
{

	int		nIndex		= 0;
	int		nReadByte1;
	int		nReadByte2;
	int		nReadByteEnd;
	
	BYTE nADDH1;
	BYTE nADDL1;
	BYTE nADDH2;
	BYTE nADDL2;


	nADDH1 = CurrentSet->nHDCP_Start_ADH;
	nADDL1 = CurrentSet->nHDCP_Start_ADL;

	memset(I2cAdcCtrl.m_nHdcpKeyReadData,400,0);

	if(nADDL1 == 0x80){
		nReadByte1 = 128;
		m_nData1 = nADDH1; // Page Address
		m_nData2 = nADDL1; // Data Address

		if(ReadI2cNbyteData2(128) == TEST_PASS)
		{	
			for(nIndex = 0; nIndex < 128; nIndex++)
			{
				m_nHdcpKeyReadData[nIndex] = m_aReadData[nIndex];
			}
		}
		else
		{
			Sleep(500);
			if(ReadI2cNbyteData2(128) == TEST_PASS)
			{	
				for(nIndex = 0; nIndex < 128; nIndex++)
				{
					m_nHdcpKeyReadData[nIndex] = m_aReadData[nIndex];
				}
			}
			else{
				return FALSE;
			}
		}
	}
	else{
		nReadByte1 = 256;
		m_nData1 = nADDH1; // Page Address
		m_nData2 = 0x00; // Data Address

		if(ReadI2cNbyteData2(128) == TEST_PASS)
		{	
			for(nIndex = 0; nIndex < 128; nIndex++)
			{
				m_nHdcpKeyReadData[nIndex] = m_aReadData[nIndex];
			}
		}
		else
		{
			Sleep(500);
			if(ReadI2cNbyteData2(128) == TEST_PASS)
			{	
				for(nIndex = 0; nIndex < 128; nIndex++)
				{
					m_nHdcpKeyReadData[nIndex] = m_aReadData[nIndex];
				}
			}
			else
			{
				return FALSE; 
			}
		}

		m_nData2 = 0x80; // Data Address

		if(ReadI2cNbyteData2(128) == TEST_PASS)
		{	
			for(nIndex = 0; nIndex < 128; nIndex++)
			{
				m_nHdcpKeyReadData[128+ nIndex] = m_aReadData[nIndex];
			}
		}
		else
		{
			Sleep(500);
			if(ReadI2cNbyteData2(128) == TEST_PASS)
			{	
				for(nIndex = 0; nIndex < 128; nIndex++)
				{
					m_nHdcpKeyReadData[128+ nIndex] = m_aReadData[nIndex];
				}
			}
			else
			{
				return FALSE; 
			}
		}

	}
	nADDH2 = nADDH1 + 0x02;
	nADDL2 = 0x00;
	nReadByte2 = CurrentSet->nHDCP_Data_Size -  nReadByte1;
	

	if(nReadByte2 > 128)
	{
		m_nData1 = nADDH2; // Page Address
		m_nData2 = nADDL2; // Data Address
		if(ReadI2cNbyteData2(128) == TEST_PASS)
		{	
			for(nIndex = 0; nIndex < 128; nIndex++)
			{
				m_nHdcpKeyReadData[nReadByte1 + nIndex] = m_aReadData[nIndex];
			}
		}
		else
		{
			Sleep(500);
			if(ReadI2cNbyteData2(128) == TEST_PASS)
			{	
				for(nIndex = 0; nIndex < 128; nIndex++)
				{
					m_nHdcpKeyReadData[nReadByte1 + nIndex] = m_aReadData[nIndex];
				}
			}
			else
			{
				return FALSE; 
			}
		}

		m_nData2 = 0x80; // Data Address
		nReadByteEnd = CurrentSet->nHDCP_Data_Size -  nReadByte1 - 128;

		if(ReadI2cNbyteData2(nReadByteEnd) == TEST_PASS)
		{	
			for(nIndex = 0; nIndex < nReadByteEnd; nIndex++)
			{
				m_nHdcpKeyReadData[nReadByte1 + 128 + nIndex] = m_aReadData[nIndex];
			}
		}
		else
		{
			Sleep(500);
			if(ReadI2cNbyteData2(nReadByteEnd) == TEST_PASS)
			{	
				for(nIndex = 0; nIndex < nReadByteEnd; nIndex++)
				{
					m_nHdcpKeyReadData[nReadByte1 + 128 + nIndex] = m_aReadData[nIndex];
				}
			}
			else
			{
				return FALSE; 
			}
		}

	}
	else{
		m_nData1 = nADDH2; // Page Address
		m_nData2 = nADDL2; // Data Address
		nReadByteEnd = nReadByte2;
		if(ReadI2cNbyteData2(nReadByteEnd) == TEST_PASS)
		{	
			for(nIndex = 0; nIndex < nReadByteEnd; nIndex++)
			{
				m_nHdcpKeyReadData[nReadByte1 + nIndex] = m_aReadData[nIndex];
			}
		}
		else
		{
			Sleep(500);
			if(ReadI2cNbyteData2(nReadByteEnd) == TEST_PASS)
			{	
				for(nIndex = 0; nIndex < nReadByteEnd; nIndex++)
				{
					m_nHdcpKeyReadData[nReadByte1 + nIndex] = m_aReadData[nIndex];
				}
			}
			else
			{
				return FALSE; 
			}
		}

	}
	return TRUE;
}
int CI2cAdcCtrl::GetFWVersion()
{
	int		nResult		= TEST_FAIL;
	CString szCmd		= _T("");
	CString sReadString = _T("");
	int nValue;
	int nValueMid;

	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++, HM_GET_VER, CR,LF);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}
	
	if((nResult = ReceiveCommString(14,5000,sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//================
	// Response Check
	//================
	if(sReadString.GetLength() >= 7)
	{
		nValue = (int)(hexCstr2decNum(sReadString.Mid(5,2)));
	}
	else{
			return 0;
	}

	nValue = (int)(hexCstr2decNum(sReadString.Mid(5,2)));
	nValueMid = (int)(hexCstr2decNum(sReadString.Mid(7,2)));
	m_MicomVer_Ex = nValueMid;
	return nValue;
}
int CI2cAdcCtrl::GetSETStS()
{
	int		nResult = TEST_FAIL;
	CString szCmd = _T("");
	CString sReadString = _T("");
	int nValue;

	szCmd.Format(">%02x%02x00000000%c%c", m_nI2cAdcSeqNo++, HM_GET_STS, CR, LF);

	if ((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}

	if ((nResult = ReceiveCommString(14, 5000, sReadString)) != TEST_PASS)
	{
		return nResult;
	}

	//================
	// Response Check
	//================
	if (sReadString.GetLength() >= 7)
	{
		nValue = (int)(hexCstr2decNum(sReadString.Mid(5, 2)));
	}
	else {
		return 0;
	}

	nValue = (int)(hexCstr2decNum(sReadString.Mid(5, 2)));

	return nValue;
}


/*
BOOL CI2cAdcCtrl::WriteEdid_2AB(int nIndex)
{
	int nResult				= TEST_FAIL;
	BOOL bResult			= FALSE;
	int nLength				= I2C_PACKET_LENGTH; // 0x84

	CString szCmd		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");

	BYTE nWriteCmd		= 0xE8;

	int nEdidSum;
	BYTE nCheckSum;
	BYTE nADDH;
	BYTE nADDL1;
	BYTE nADDL2;
	BYTE nDATA;
	int  nSize = 0;
	int i,j,k;

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szTemp.Format("%02x",nIndex);

	if((nResult = SetChannel(szTemp)) != TEST_PASS)
	{
		return FALSE;
	}
	for(i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if(nResult == TEST_PASS) break;
		Sleep(1);
	}	
	memset(m_aWriteData,256,0);
	switch(nIndex)
	{
		case 0:
			nSize = CurrentSet->nEDID_DataSize_A;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_A[i];
			}
			break;

		case 1:
			nSize = CurrentSet->nEDID_DataSize_D;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_D[i];
			}
			break;

		case 2:
			nSize = CurrentSet->nEDID_DataSize_H;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_H[i];
			}
			break;

		case 3:
			nSize = CurrentSet->nEDID_DataSize_I;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_I[i];
			}
			break;

		default:
			return FALSE;
			break;
	}

	m_aWriteData[16] = CurrentSet->nEDID_CurrentWeek;
	m_aWriteData[17] = CurrentSet->nEDID_CurrentYear;

	nCheckSum = 0;
	nEdidSum = 0;
	for(i =0; i < 127; i++)
	{
		nEdidSum = nEdidSum + m_aWriteData[i];
	}
	nCheckSum = BYTE(0x100 - nEdidSum); 
	m_aWriteData[127] = nCheckSum;

	nADDH = 0xA6;
	nADDL1 = 0x00;
	nADDL2 = 0x80;

	for(i = 0; i < 8; i++)
	{
		nCheckSum	= 0x6E ^ 0x50 ^ (I2C_PACKET_LENGTH+16) ^ 0x03 ^ 0xE8 ^ nADDH ^ (nADDL1 + 16* i) ;
		szCmd.Format("=6E50%02X03%02X%02X%02X",I2C_PACKET_LENGTH+16,nWriteCmd, nADDH, (nADDL1 +16* i));

		for(j = 0; j < 16; j++)
		{
			nDATA = m_aWriteData[16* i + j];
			nCheckSum = nCheckSum ^ nDATA;
			szTemp.Format("%02X",nDATA);
			szCmd = szCmd + szTemp;
		} 
		szTemp.Format("%02X%c%c",nCheckSum,CR,LF);
		szCmd = szCmd + szTemp;

		for(k =0; k < 5; k++){
			bResult = WriteI2c_Data(szCmd, 24);
			if(bResult) 
			{
				Sleep(50);
				break;
			}
			else{
				Sleep(250);
			}

		}
		if(!bResult){ return FALSE;}
		
	}

	if(nSize == 256)
	{
		for(i = 0; i < 8; i++)
		{
			nCheckSum	= 0x6E ^ 0x50 ^ (I2C_PACKET_LENGTH+16) ^ 0x03 ^ 0xE8 ^ nADDH ^ (nADDL2 + 16* i);
			szCmd.Format("=6E50%02X03%02X%02X%02X",I2C_PACKET_LENGTH+16,nWriteCmd, nADDH, (nADDL2 + 16* i));

			for(j = 0; j < 16; j++)
			{
				nDATA = m_aWriteData[128+ 16* i + j];
				nCheckSum = nCheckSum ^ nDATA;
				szTemp.Format("%02X",nDATA);
				szCmd = szCmd + szTemp;
			}
			szTemp.Format("%02X%c%c",nCheckSum,CR,LF);
			szCmd = szCmd + szTemp;

			for(k =0; k < 5; k++){
				bResult = WriteI2c_Data(szCmd, 24);
				if(bResult) 
				{
					Sleep(50);
					break;
				}
				else{
					Sleep(250);
				}

			}
			if(!bResult){ return FALSE;}
		}
	}

	return TRUE;
}


BOOL CI2cAdcCtrl::WriteEdid_2B(int nIndex)
{
	int nResult				=	 TEST_FAIL;

	CString szData		 = _T("");
	CString szTemp		 = _T("");
	CString szReadString = _T("");

	int nEdidSum;
	BYTE nCheckSum;
	BYTE nADDL1;
	BYTE nADDL2;
	BYTE nDATA;
	int  nSize = 0;
	int i,j;

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szTemp.Format("%02x",nIndex);

	if((nResult = SetChannel(szTemp)) != TEST_PASS)
	{
		return FALSE;
	}
	WakeUp();
	
	memset(m_aWriteData,256,0);
	switch(nIndex)
	{
		case 0:
			nSize = CurrentSet->nEDID_DataSize_A;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_A[i];
			}
			break;

		case 1:
			nSize = CurrentSet->nEDID_DataSize_D;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_D[i];
			}
			break;

		case 2:
			nSize = CurrentSet->nEDID_DataSize_H;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_H[i];
			}
			break;

		case 3:
			nSize = CurrentSet->nEDID_DataSize_I;
			for(i =0; i < nSize; i++)
			{
				m_aWriteData[i] = m_nEdidData_I[i];
			}
			break;

		default:
			return FALSE;
			break;
	}

	m_aWriteData[16] = CurrentSet->nEDID_CurrentWeek;
	m_aWriteData[17] = CurrentSet->nEDID_CurrentYear;
	nCheckSum = 0;
	nEdidSum = 0;
	for(i =0; i < 127; i++)
	{
		nEdidSum = nEdidSum + m_aWriteData[i];
	}
	nCheckSum = BYTE(0x100 - nEdidSum); 
	m_aWriteData[127] = nCheckSum;

	nADDL1 = 0x00;
	nADDL2 = 0x80;

	for(i = 0; i < 16; i++)
	{
		nADDL1 =  8* i;

		szData = "=";
		for(j = 0; j < 8; j++)
		{
			nDATA = m_aWriteData[ nADDL1 + j];
			szTemp.Format("%02X",nDATA);
			szData = szData + szTemp;
		} 
		szTemp.Format("%c%c",CR,LF);
		szData = szData + szTemp;

		if(WriteI2c2b_PageData(nADDL1, szData) != TEST_PASS)
		{
			if(WriteI2c2b_PageData(nADDL1, szData) != TEST_PASS){ return FALSE;}
		}
	}

	if(nSize == 256)
	{
		for(i = 0; i < 16; i++)
		{
			nADDL2  = 0x80 + 8* i;
			szData = "=";
			for(j = 0; j < 16; j++)
			{
				nDATA = m_aWriteData[nADDL2 + j];
				szTemp.Format("%02X",nDATA);
				szData = szData + szTemp;
			}
			szTemp.Format("%c%c",CR,LF);
			szData = szData + szTemp;

			if(WriteI2c2b_PageData(nADDL2, szData) != TEST_PASS)
			{
				if(WriteI2c2b_PageData(nADDL2, szData) != TEST_PASS){ return FALSE;}
			}
		}
	}

	return TRUE;
}

BOOL CI2cAdcCtrl::ReadEdid_2AB(int nIndex)
{
	int nResult				=	 TEST_FAIL;

	CString szTemp		 = _T("");
	int  nSize = 0;
	int i;
	
	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szTemp.Format("%02x",nIndex);

	if((nResult = SetChannel(szTemp)) != TEST_PASS)
	{
		return FALSE;
	}
	for(i = 0; i < 5; i++)
	{
		nResult = WakeUp();
		if(nResult == TEST_PASS) break;
		Sleep(1);
	}

	memset(m_nEdidReadData,256,0);
	switch(nIndex)
	{
		case 0:
			nSize = CurrentSet->nEDID_DataSize_A;
			break;

		case 1:
			nSize = CurrentSet->nEDID_DataSize_D;
			break;

		case 2:
			nSize = CurrentSet->nEDID_DataSize_H;
			break;

		case 3:
			nSize = CurrentSet->nEDID_DataSize_I;
			break;

		default:
			return FALSE;
			break;
	}		

	if(nSize > 128)
	{
		m_nData1 = 0xA6; // Page Address
		m_nData2 = 0x00; // Data Address
		if(ReadI2cNbyteData2(128) == TEST_PASS)
		{
			for(i = 0; i < 128; i++)
			{
				m_nEdidReadData[i] = m_aReadData[i];
			}
		}
		else
		{
			return FALSE; 
		}
		m_nData1 = 0xA6; // Page Address
		m_nData2 = 0x80; // Data Address

		if(ReadI2cNbyteData2(nSize - 128) == TEST_PASS)
		{
			for(i = 0; i < (nSize - 128); i++)
			{
				m_nEdidReadData[128 + i] = m_aReadData[i];
			}
		}
		else
		{
			return FALSE; 
		}
	}
	else{

		m_nData1 = 0xA6; // Page Address
		m_nData2 = 0x00; // Data Address
		if(ReadI2cNbyteData2(nSize) == TEST_PASS)
		{
			for(i = 0; i < nSize; i++)
			{
				m_nEdidReadData[i] = m_aReadData[i];
			}
		}
		else
		{
			return FALSE; 
		}
	}
	return TRUE;
}

BOOL CI2cAdcCtrl::ReadEdid_2B(int nIndex)
{
	int nResult				=	 TEST_FAIL;

	CString szTemp		 = _T("");
	int  nSize = 0;
	int i;
	
	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	szTemp.Format("%02x",nIndex);

	if((nResult = SetChannel(szTemp)) != TEST_PASS)
	{
		return FALSE;
	}
	WakeUp();


	memset(m_nEdidReadData,256,0);
	switch(nIndex)
	{
		case 0:
			nSize = CurrentSet->nEDID_DataSize_A;
			break;

		case 1:
			nSize = CurrentSet->nEDID_DataSize_D;
			break;

		case 2:
			nSize = CurrentSet->nEDID_DataSize_H;
			break;

		case 3:
			nSize = CurrentSet->nEDID_DataSize_I;
			break;

		default:
			return FALSE;
			break;
	}		

	if(ReadI2c2bNbyteData2(0x00, 128) == TEST_PASS)
	{
		for(i = 0; i < 128; i++)
		{
			m_nEdidReadData[i] = m_aReadData[i];
		}
	}
	else
	{
		return FALSE; 
	}
	if(nSize == 256)
	{
		if(ReadI2c2bNbyteData2(0x80, 128) == TEST_PASS)
		{
			for(i = 0; i < 128; i++)
			{
				m_nEdidReadData[128+ i] = m_aReadData[i];
			}
		}
		else
		{
			return FALSE; 
		}

	}

	return TRUE;
}
*/