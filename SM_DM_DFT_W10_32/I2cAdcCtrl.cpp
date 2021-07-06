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
	CString sTmp = sComPort;
	sTmp.Delete(0, sTmp.Find("COM")+3);
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
	//+change kwmoon 080919
//	memset(m_strReceive, 0, 225);
	memset(m_strReceive, 0, 1024);
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
				break;			
			}
			Sleep(10);
		}

		int rrv = m_ctrlI2cAdcCtrl.m_QueueRead.GetSize();
		
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

				//+add kwmoon 080826
				m_szCurrentStatusMsg.Format("=> Receive : %s",m_strReceive);

				AddStringToStatus(szString);
			}
			//+change kwmoon 080804
		//	return TRUE;
			return TEST_PASS;
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

	szCmd.Format(">%02x4002%02x0000%c%c",m_nI2cAdcSeqNo++,nClkSpeed+1,CR,LF);

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
		if(sReadString.Mid(1,6).Compare(szCmd.Mid(1,6)) != NULL)
		{
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

	szCmd.Format(">%02x4001%04x00%c%c",m_nI2cAdcSeqNo++,nDelay,CR,LF);

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
		if(sReadString.Mid(1,10).Compare(szCmd.Mid(1,10)) != NULL)
		{
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
	szCmd.Format(">%02x400300%02x00%c%c",m_nI2cAdcSeqNo++,nDelay,CR,LF);

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
		if(sReadString.Mid(1,10).Compare(szCmd.Mid(1,10)) != NULL)
		{
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

	szCmd.Format(">%02x400400%02x00%c%c",m_nI2cAdcSeqNo++,nDelay,CR,LF);

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
		if(sReadString.Mid(1,10).Compare(szCmd.Mid(1,10)) != NULL)
		{
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

	szCmd.Format(">%02x12A0%s%s00%c%c",m_nI2cAdcSeqNo++,szData1,szData2,CR,LF);

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

		szCmd.Format(">%02x12A0%s%s00%c%c",m_nI2cAdcSeqNo++,szData1,szData2,CR,LF);

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

	szCmd.Format(">%02x22A1%s0000%c%c",m_nI2cAdcSeqNo++,szData1,CR,LF);

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

		szCmd.Format(">%02x22A1%s0000%c%c",m_nI2cAdcSeqNo++,szData1,CR,LF);

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

		szData1.Format("%02x",m_nData1+i+1);

		if(i > 0) Progress.SetPos((int)(((float)i*100/(float)(nByte-1))));
	}

 	//+add 090219(Modification No2)
	szLog = "E:ReadI2c2bNbyteData\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;
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

int CI2cAdcCtrl::CheckI2cMaxMin(int nTarget,int nMargin)
{
	//+add 090219(Modification No2)
	CString szLog = "B:CheckI2cMaxMin\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	if(m_szI2cAdcReadString == _T(""))
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_EMPTY_READ_STRING;
	}
	
	if(m_szI2cAdcReadString.GetLength() < 14)
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_SHORT_READ_STRING;
	}
	
	int nReadData[4]   = {0,0,0,0};
	CString szReadData = _T("");
	CString sTemp;
	
//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(5),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(6));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH),m_szI2cAdcReadString.GetAt(CMD_LENGTH+1));
	nReadData[0] = hexCstr2decNum(szReadData);

//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(7),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(8));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+2),m_szI2cAdcReadString.GetAt(CMD_LENGTH+3));
	nReadData[1] = hexCstr2decNum(szReadData);

//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(9),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(10));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+4),m_szI2cAdcReadString.GetAt(CMD_LENGTH+5));
	nReadData[2] = hexCstr2decNum(szReadData);
	
	int nMaxValue = max(max(nReadData[0],nReadData[1]),nReadData[2]);
	int nMinValue = min(min(nReadData[0],nReadData[1]),nReadData[2]);
	int nDiff	  = nMaxValue - nMinValue; 

	//+add kwmoon 081013
	pCurStep->m_szAdcValue.Format("R%d,G%d,B%d",nReadData[0],nReadData[1],nReadData[2]);

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

int CI2cAdcCtrl::CheckI2cLimit(int nTarget,int nMargin)
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
	
	if(m_szI2cAdcReadString.GetLength() < 14)
	{
		pCurStep->m_bAdcTestResult = FALSE;
		return ERROR_I2C_SHORT_READ_STRING;
	}

	int nReadData[4]   = {0,0,0,0};
	CString szReadData = _T("");

	
//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(5),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(6));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH),m_szI2cAdcReadString.GetAt(CMD_LENGTH+1));
	nReadData[0] = hexCstr2decNum(szReadData);

//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(7),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(8));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+2),m_szI2cAdcReadString.GetAt(CMD_LENGTH+3));
	nReadData[1] = hexCstr2decNum(szReadData);

//	szReadData.Format("%c%c",I2cAdcCtrl.m_szI2cAdcReadString.GetAt(9),I2cAdcCtrl.m_szI2cAdcReadString.GetAt(10));
	szReadData.Format("%c%c",m_szI2cAdcReadString.GetAt(CMD_LENGTH+4),m_szI2cAdcReadString.GetAt(CMD_LENGTH+5));
	nReadData[2] = hexCstr2decNum(szReadData);

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
	if((m_nReadByte < 1) || (m_nReadByte > 4))
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

	//==================
	// Set Byte Address
	//==================

	//+del kwmoon 080918
/*
	int nResultAddress[4] = {0,0,0,0};
	
	for(int i=0; i<m_nReadByte; i++)
	{
		nResultAddress[i] = m_nData2 + i;
	}
*/
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//=============================
	// Read 4Byte Data From Buffer
	//=============================

	//+change kwmoon 080918
//	szCmd.Format(">%02x30%02x%02x%02x%02x%c%c",m_nI2cAdcSeqNo++,
//				  nResultAddress[0], nResultAddress[1],
//				  nResultAddress[2], nResultAddress[3],CR,LF);

	//+add kwmoon 080918
	int nReadByte = 4;

	szCmd.Format(">%02x30%02x%02x0000%c%c",m_nI2cAdcSeqNo++,m_nData2,(nReadByte-1),CR,LF);

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

int CI2cAdcCtrl::ReadI2cNbyteData(int nByte, CProgressCtrl& Progress)
{
	//+add 090219(Modification No2)
	CString szLog = "B:ReadI2cNbyteData\n";
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
	szCmd.Format(">%02x2000000000%c%c",m_nI2cAdcSeqNo++,CR,LF);

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
	//+add kwmoon 080910
	memset(m_aReadData,256,0);

	//+del kwmoon 080918
/*
	for(int i=0; i<(nByte/4); i++)
	{
		//==================
		// Set Byte Address
		//==================
		int nResultAddress[4] = {0,0,0,0};

		for(int j=0; j<4; j++)
		{
			nResultAddress[j] = m_nData2 + j+i*4;
		}

		if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

		//=============================
		// Read 4Byte Data From Buffer
		//=============================
		szCmd.Format(">%02x30%02x%02x%02x%02x%c%c",m_nI2cAdcSeqNo++,
					  nResultAddress[0], nResultAddress[1],
					  nResultAddress[2], nResultAddress[3],CR,LF);

		//+add kwmoon 080826
		m_szCurrentStatusMsg.Format("[2AB] Read : %02x,%02x,%02x,%02x \t\t Send :  %s",
					 nResultAddress[0],nResultAddress[1],nResultAddress[2],nResultAddress[3],szCmd);
	
		if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
		{
			return nResult;
		}	

		m_szI2cAdcReadString = _T("");

		if((nResult = ReceiveCommString(14,3000,m_szI2cAdcReadString)) != TEST_PASS)
		{
			return nResult;
		}

		m_aReadData[i*4]   = hexCstr2decNum(m_szI2cAdcReadString.Mid(5,2));
		m_aReadData[i*4+1] = hexCstr2decNum(m_szI2cAdcReadString.Mid(7,2));
		m_aReadData[i*4+2] = hexCstr2decNum(m_szI2cAdcReadString.Mid(9,2));
		m_aReadData[i*4+3] = hexCstr2decNum(m_szI2cAdcReadString.Mid(11,2));

		if(i > 0) Progress.SetPos((int)((float)i*100/(float)(nByte/4-1)));
	}
*/

	//+add kwmoon 080918
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;

	//==============================
	// Read N Byte Data From Buffer
	//==============================
	szCmd.Format(">%02x30%02x%02x0000%c%c",m_nI2cAdcSeqNo++,
				  m_nData2,(nByte-1),CR,LF);

	m_szCurrentStatusMsg.Format("[2AB] Read : %02x + %02x bytes \t\t Send :  %s",
				 m_nData2,nByte,szCmd);

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
	szCmd.Format(">%02x60%s000000%c%c",m_nI2cAdcSeqNo++,szData1,CR,LF);
		
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

int CI2cAdcCtrl::SendCmd(CString szData1,CString szData2)
{
	//+add 090219(Modification No2)
	CString szLog = "B:SendCmd\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int nResult = TEST_FAIL;

	CString szCmd		= _T("");
	CString sReadString = _T("");
	
	//================
	// SetReg4 ~ Reg7
	//================
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x02%s00%s00%c%c",m_nI2cAdcSeqNo++,szData1,szData2,CR,LF);
	
	//+add kwmoon 080826
	m_szCurrentStatusMsg.Format("[2AB] Set Command : %s, %s \t Send :  %s",szData1,szData2,szCmd);
	
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
	m_szCurrentStatusMsg.Format("[2AB] Send Command : %s, %s \t Send :  %s",szData1,szData2,szCmd);

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

		
	//+add 090219(Modification No2)
	szLog = "E:SendCmd\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return nResult;
}

int CI2cAdcCtrl::WakeUp()
{
	//+add 090219(Modification No2)
	CString szLog = "B:WakeUp\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	int nResult = TEST_FAIL;

	CString szCmd		= _T("");
	CString sReadString = _T("");
	
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	szCmd.Format(">%02x1100000000%c%c",m_nI2cAdcSeqNo++,CR,LF);
	
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
		
	//+add 090219(Modification No2)
	szLog = "E:WakeUp\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return nResult;
}

#define SET_I2C_REG0_3_CMD				0x01
#define SET_I2C_REG4_7_CMD				0x02
#define SET_I2C_WRITE_OPTION_CMD		0x14
#define SET_I2C_WRITE_OPTION_SUB_CMD	0x0A
#define SET_I2C_SEND_CMD				0x10
#define SET_I2C_READ_CMD				0x24
#define SET_I2C_TRANSFER_DATA_CMD		0x30
#define I2C_PACKET_LENGTH				0x84

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

	//change 090604
	if(nType == TOOL_OPTION)
	{
		if((nOptionNo < 1) || (nOptionNo > 5))
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
	//091016
	if(nOptionNo == 5){
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

	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,SET_I2C_REG0_3_CMD,CR,LF);
	
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
	if(nOptionNo == 5){
		szCmd.Format(">%02x%02xF5%02x00%02x%c%c",m_nI2cAdcSeqNo++,SET_I2C_REG4_7_CMD,(nSubCmd+nOptionNo), nCheckSum,CR,LF);
	}
	else{
		szCmd.Format(">%02x%02xF5%02x00%02x%c%c",m_nI2cAdcSeqNo++,SET_I2C_REG4_7_CMD,(nSubCmd+(nOptionNo-1)), nCheckSum,CR,LF);
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
	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	
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
	szCmd.Format(">%02x%02x00%02x0000%c%c",m_nI2cAdcSeqNo++,SET_I2C_TRANSFER_DATA_CMD,(nReadByte-1),CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] %s \t Send :  %s",szOptionType,szCmd);

	if((nResult = SendI2cCmd(szCmd)) != TEST_PASS)
	{
		return nResult;
	}


	if((nResult = ReceiveCommString(CMD_LENGTH+(nReadByte*2)+1,5000,m_szI2cAdcReadString)) != TEST_PASS)
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
		if((nOptionNo < 1) || (nOptionNo > 5))
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
	if(nOptionNo == 5){
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
	if(nOptionNo == 5){
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

//+add kwmoon 080923
int CI2cAdcCtrl::ReadVersion()
{
	//+add 090219(Modification No2)
	CString szLog = "B:ReadVersion\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

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
	WakeUp();

	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,SET_I2C_REG0_3_CMD,CR,LF);
	
	m_szCurrentStatusMsg.Format("[2AB] Read Version \t Send :  %s",szCmd);

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
	szCmd.Format(">%02x%02xF50400%02x%c%c",m_nI2cAdcSeqNo++,SET_I2C_REG4_7_CMD,nCheckSum,CR,LF);
	
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
	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	
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
	
	//+add 090219(Modification No2)
	szLog = "E:ReadVersion\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return nResult;
}


//+add 090512(Modification No2)
BOOL CI2cAdcCtrl::GetModuleVoltage(float& dVolt)
{
	CString szCmd		= _T("");
	CString sReadString = _T("");
	int nResult			= TEST_FAIL;
	
	int nMeasureValue  = 0;
	
	float fMeasuredValue = 0.0;

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

	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
		
		if(szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
			pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1;
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

BOOL CI2cAdcCtrl::InputSourceChange(BYTE nCode)
{
	CString szCode = _T("");

	m_ctrlI2cAdcCtrl.ClearPort();
	if(m_nI2cAdcSeqNo >= 0xff) m_nI2cAdcSeqNo = 0;
	SetChannel("00");
	WakeUp();

	szCode.Format("%02x",nCode); 
	if(SendCmd("F4",szCode) != TEST_PASS)
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
	if(SendCmd("60",szCode) != TEST_PASS)
	{
		return FALSE;
	}	
	return TRUE; 	
}

//add 090611
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

	szCmd.Format(">%02x%02x6E508403%c%c",m_nI2cAdcSeqNo++,SET_I2C_REG0_3_CMD,CR,LF);
	
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
	szCmd.Format(">%02x%02xF50A00%02x%c%c",m_nI2cAdcSeqNo++,SET_I2C_REG4_7_CMD,nCheckSum,CR,LF);
	
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
	szCmd.Format(">%02x%02x00000000%c%c",m_nI2cAdcSeqNo++,SET_I2C_READ_CMD,CR,LF);
	
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
	SetChannel("00");
	WakeUp();

	sTemp.Format("I2C EDID Download Start");
	ShowSubStepMessage(sTemp, _T(""));
	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sTemp);
	}
	if((nResult = SendCmd("F6","0A")) != TEST_PASS)
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