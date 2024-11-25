// VF1000.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DATsys.h"
#include "VF1000.h"
#include "global.h"

extern CDATsysView*  g_pView;
// CVF1000
CVF1000 gVF1000Ctrl;


CVF1000::CVF1000()
{
	m_bPortOpen = FALSE;
	m_strErrMsg = _T("");
	m_nPortID	= -1;

	m_nChNo = 1;		
	m_nCheckTime = 1500;	
	m_dSpec1 = 0.0;
	m_dSpec2 = 0.0;

	m_bSystemMixType = _VF_ONLY_TYPE;

}

CVF1000::~CVF1000()
{
}


// CVF1000 멤버 함수
BOOL CVF1000::Create(CString sComPort, DWORD nBaudRate)
{
	int nIndex;

	CString sTmp = sComPort;
	nIndex = sTmp.Find("COM");
	sTmp.Delete(0, nIndex + 3);
	m_nPortID = atoi(sTmp);

	m_strErrMsg = _T("");
	
	if(!m_ctrlVF1000.OpenPort( sComPort, nBaudRate, m_nPortID))
	{
		m_strErrMsg.Format("[VF1000] Failed to open %s port!", sComPort);

		m_bPortOpen = FALSE;
		return FALSE;
	}
	else m_bPortOpen = TRUE;

	Sleep(150);

	return TRUE;
}

void CVF1000::PortClose()
{
	m_ctrlVF1000.ClosePort();
	m_bPortOpen = FALSE;
}

BOOL CVF1000::Init()
{
	memset(m_strReceive, 0, 225);
	return TRUE;
}


BOOL CVF1000::SendByte(BYTE Data)
{
	SendCommString(&Data);
	return TRUE;
}

BOOL CVF1000::SendCommString(CString str)
{
	CString lStemp;
	CString lStempSend;
	if (m_bSystemMixType == _IR_MIX_TYPE)// _VF_ONLY_TYPE
	{
		if ((str.Find("IR_") > 0)||(str.Find("LAN") > 0))
		{
			lStemp.Format("%cM%02d[01:", STX, str.GetLength());
			lStempSend = lStemp;
			lStempSend += str;
			lStemp = "]\r\n";
			lStempSend += lStemp;
		}
		else
		{
			lStemp.Format("%cM%02d[02:", STX, str.GetLength());
			lStempSend = lStemp;
			lStempSend += str;
			lStemp = "]\r\n";
			lStempSend += lStemp;
		}
		m_ctrlVF1000.m_QueueRead.Clear();
		m_ctrlVF1000.WriteComm((BYTE*)(LPCTSTR)lStempSend, lStempSend.GetLength());

	}
	else
	{
		m_ctrlVF1000.m_QueueRead.Clear();
		m_ctrlVF1000.WriteComm((BYTE*)(LPCTSTR)str, str.GetLength());
	}


	return TRUE;
}
BOOL CVF1000::ReceiveString(CString &str, int nWaitLimit)
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

	m_ctrlVF1000.m_QueueRead.Clear();

	while (bDoing)
	{
		do	
		{

			dwRead = m_ctrlVF1000.ReadComm( buff, 2048);
			if (dwRead == 0) break;		
			if (BUFF_SIZE - m_ctrlVF1000.m_QueueRead.GetSize() > (int)dwRead)
			{
				for ( WORD i = 0; i < dwRead; i++)
					m_ctrlVF1000.m_QueueRead.PutByte(buff[i]);
			}
			else
			{
				return FALSE;
			}

			if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
			{//Over Limit Time.....False Return
				m_ctrlVF1000.m_QueueRead.Clear();
				return FALSE;
			}
			  
		} while (dwRead); 

		int rrv = m_ctrlVF1000.m_QueueRead.GetSize();

		if (rrv > 0)
		{
			for(i=0; i<rrv; i++)
			{
				m_ctrlVF1000.m_QueueRead.GetByte(&buf);
				m_strReceive[i] = buf;
			}

			sTmp.Format("%s",m_strReceive);
			str += sTmp;
			
			Sleep(50);
			if (m_bSystemMixType == _IR_MIX_TYPE)// _VF_ONLY_TYPE
			{
				idx1 = str.Find(_T("]\r\n"));
				if (idx1 >= 0) 
				{
						sTmp = str.Left(idx1);
						str = sTmp.Mid(8);
						idx1 = str.Find(_T("\r\n"));
						if (idx1 >= 0) {
							m_ctrlVF1000.m_QueueRead.Clear();
							return TRUE;
						}
				}

			}
			else
			{
				idx1 = str.Find(_T("\r\n"));
				if (idx1 >= 0) {
					m_ctrlVF1000.m_QueueRead.Clear();
					return TRUE;
				}
			}
		}
		
        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {//Over Limit Time.....False Return
			m_ctrlVF1000.m_QueueRead.Clear();
			return FALSE;
		}
	}
	
	return TRUE;	
}


BOOL CVF1000::GetVoltageData(int nCH, double &dData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	double dRevData = 0.0;
	int nFind = -1;
	int nR_CH = 0;
	int nR_CH2 = 0;
	BOOL bRev = TRUE;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlVF1000.ClearPort();
	
	szCmdString.Format("%cCREADV_%02d%c\r\n",STX, nCH,ETX);
	
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveString(szData, 1000))
	{
		SendCommString(szCmdString);
		if(!ReceiveString(szData,1000))
		{
			SendCommString(szCmdString);
			if (!ReceiveString(szData, 1000))
			{
				if (g_pView-> m_InitDialogEnd == 1)
				{
					AddStringToStatus(_T("VF1000 Com. Error"));

				}
				return FALSE;
			}
		}
	}
	
	szData.MakeUpper();
//	nFind = szData.Find("DV_");
	nFind = szData.Find("DV");
	if(nFind != -1) 
	{
		szData.Delete(0, nFind + 2);
		szData.TrimRight(0x0a);
		szData.TrimRight(0x0d);
		szData.TrimRight(0x03);

		szTemp = szData.Left(2);
		nR_CH = atoi(szTemp);
		/*if(nCH < 10){
			nR_CH2 = nR_CH;
		}
		else if(nCH >10 && nCH < 20){
			nR_CH2 = nR_CH + 1;
		}
		else if(nCH >20 && nCH < 30){
			nR_CH2 = nR_CH + 2;
		}
		else if(nCH >30 && nCH < 37){
			nR_CH2 = nR_CH - 3;
		}
		else {
			nR_CH2 = nR_CH + 9;
		} */

	//	if(nCH != nR_CH2){
		if(nCH != nR_CH){
			bRev = FALSE;
		}

		szData.Delete(0, 2);
		szData.TrimLeft(0x20);;
		
		dRevData = atoi(szData) * 0.1;
//		dRevData = (m_strReceive[6] - 0x30)*100 + (m_strReceive[7] - 0x30)*10 + (m_strReceive[8] - 0x30)*1 + (m_strReceive[9] - 0x30)*0.1;
//		dRevData = (szData.GetAt(0) - 0x30)*100 + (szData.GetAt(1) - 0x30)*10 + (szData.GetAt(2) - 0x30)*1 + (szData.GetAt(3) - 0x30)*0.1;
	}
	else{
		dRevData =  0.0;
		bRev = FALSE; 
	}
	dData = dRevData;

	return bRev;
}


BOOL CVF1000::GetFrequencyData(int nCH, double &dData)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	double dRevData = 0.0;
	int nFind = -1;
	int nR_CH = 0;
	int nR_CH2 = 0;
	BOOL bRev = TRUE;

	if(!m_bPortOpen) return FALSE;
	
	m_ctrlVF1000.ClearPort();
	
	szCmdString.Format("%cCREADF_%02d%c\r\n",STX, nCH,ETX);
	
	if(!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if(!ReceiveString(szData, 1000))
	{
		SendCommString(szCmdString);
		if(!ReceiveString(szData,1000))
		{
			SendCommString(szCmdString);
			if (!ReceiveString(szData, 1000))
			{
				AddStringToStatus(_T("VF1000 Com. Error"));
				return FALSE;
			}
		}
	}
	
	szData.MakeUpper();
	nFind = szData.Find("DF_");
	if(nFind != -1) 
	{
		szData.Delete(0, nFind + 3);
		szData.TrimRight(0x0a);
		szData.TrimRight(0x0d);
		szData.TrimRight(0x03);

		szTemp = szData.Left(1);
		nR_CH = atoi(szTemp);
	/*	if(nCH < 10){
			nR_CH2 = nR_CH;
		}
		else if(nCH >10 && nCH < 20){
			nR_CH2 = nR_CH + 1;
		}
		else if(nCH >20 && nCH < 30){
			nR_CH2 = nR_CH + 2;
		}
		else {
			nR_CH2 = nR_CH + 3;
		}
		*/
		if(nCH != nR_CH){
			bRev = FALSE;
		}

		szData.Delete(0, 1);
		szTemp = szData.Right(1);
		if(szTemp == "H"){
			szData.TrimRight('H');
			dRevData = atoi(szData);
		}
		if(szTemp == "K"){
			szData.TrimRight('K');
			dRevData = atof(szData) * 1000;
		}
	}
	else{
		dRevData =  0.0;
		bRev = FALSE;
	}
	dData = dRevData;

	return bRev;
}

BOOL  CVF1000::GetVoltageData2(int nCH, int nWaitLimit, double &dData1, double &dData2)
{

	BOOL		bDoing;
    clock_t		start;
	int         nTemp;
	double dMinVal = 0.0;
	double dMaxVal = 0.0;
	double dBufVal = 0.0;


	MSG msg;

//	if(!m_bPortOpen) return FALSE;

	if(!GetVoltageData(nCH,dMinVal)){ return FALSE;} 
	dMaxVal = dMinVal;

	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
		Sleep(300);
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   

		if(!GetVoltageData(nCH,dBufVal)){
			return FALSE; 
		}

		if(dBufVal > dMaxVal){
			dMaxVal = dBufVal;
		}
		else if(dBufVal < dMinVal){
			dMinVal = dBufVal;
		}


        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {
			bDoing = FALSE;
			break;
		}
		Sleep(100);
	}
	dData1 =  dMinVal;
	dData2 =  dMaxVal;

	return TRUE;
}

BOOL  CVF1000::GetFrequencyData2(int nCH, int nWaitLimit, double &dData1, double &dData2)
{

	BOOL		bDoing;
    clock_t		start;
	int         nTemp;
	double dMinVal = 0.0;
	double dMaxVal = 0.0;
	double dBufVal = 0.0;


	MSG msg;

//	if(!m_bPortOpen) return FALSE;

	if(!GetFrequencyData(nCH,dMinVal)){ return FALSE;} 
	dMaxVal = dMinVal;

	bDoing = TRUE;
	start = clock();

	while (bDoing)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   

		if(!GetFrequencyData(nCH,dBufVal)){return FALSE;} 

		if(dBufVal > dMaxVal){
			dMaxVal = dBufVal;
		}
		else if(dBufVal < dMinVal){
			dMinVal = dBufVal;
		}


        if ((clock() - start)*1000/CLOCKS_PER_SEC > nWaitLimit) 
        {
			bDoing = FALSE;
			break;
		}
		Sleep(100);
	}
	dData1 =  dMinVal;
	dData2 =  dMaxVal;

	return TRUE;
}
int CVF1000::GetVF_Count()
{

	POSITION Pos = NULL;
	CVFTestInfo* pVFTestInfo = NULL;
	BOOL		bRun;
	UINT		nChNo;
	CString		strTestName;
	UINT		nCheckType;
	//	double		dTarget;	
	double		dLowerLimit;
	double		dUpperLimit;
	UINT		nTime = 0;
	CString sTmp;
	int nTestIndex = 0;
	if (CurrentSet->VF_TestInfo.GetCount() == 0) { return 0; }

	Pos = CurrentSet->VF_TestInfo.GetHeadPosition();

	while (Pos)
	{
		pVFTestInfo = CurrentSet->VF_TestInfo.GetNext(Pos);
		bRun = pVFTestInfo->bRun;
		nChNo = pVFTestInfo->nChNo;
		strTestName = pVFTestInfo->strTestName;
		nCheckType = pVFTestInfo->nCheckType;
		dLowerLimit = pVFTestInfo->dLowerLimit;
		dUpperLimit = pVFTestInfo->dUpperLimit;
		nTime = pVFTestInfo->nTime;
		if (bRun)
		{
			if ((nCheckType == 1) && (dLowerLimit >= 0) && (dLowerLimit <= 0.5))
			{
				continue;
			}

			nTestIndex++;
		}
	}
	return nTestIndex;
}

BOOL CVF1000::GetIRBlasterData(CString &str, int nWaitLimit)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	double dRevData = 0.0;
	int nFind = -1;
	int nR_CH = 0;
	int nR_CH2 = 0;
	BOOL bRev = TRUE;

	if (!m_bPortOpen) return FALSE;

	m_ctrlVF1000.ClearPort();

	szCmdString.Format("%cCIR_READ0%c\r\n", STX,  ETX);

	if (!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 1000))
	{
		SendCommString(szCmdString);
		if (!ReceiveString(szData, 1000))
		{
			SendCommString(szCmdString);
			if (!ReceiveString(szData, 1000))
			{
				AddStringToStatus(_T("IR Com. Error"));
				return FALSE;
			}
		}
	}

	szData.MakeUpper();
	//	nFind = szData.Find("DV_");
	nFind = szData.Find("DIR");
	if (nFind != -1)
	{
		str = szData.Mid(nFind + 4, 11);

		
		//		dRevData = (m_strReceive[6] - 0x30)*100 + (m_strReceive[7] - 0x30)*10 + (m_strReceive[8] - 0x30)*1 + (m_strReceive[9] - 0x30)*0.1;
		//		dRevData = (szData.GetAt(0) - 0x30)*100 + (szData.GetAt(1) - 0x30)*10 + (szData.GetAt(2) - 0x30)*1 + (szData.GetAt(3) - 0x30)*0.1;
	}
	else {
		str = "";
		bRev = FALSE;
	}
	

	return bRev;
}


BOOL CVF1000::CheckLanHDCP(CString &str, int nWaitLimit)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	double dRevData = 0.0;
	int nFind = -1;
	int nR_CH = 0;
	int nR_CH2 = 0;
	BOOL bRev = TRUE;

	if (!m_bPortOpen)
	{
		AddStringToStatus("LANCHECK PORT NOT OPEN");
		return FALSE;
	}
	else
	{
		AddStringToStatus("DHCP LANCHECK");
	}

	m_ctrlVF1000.ClearPort();

	szCmdString.Format("%cCLANCHECK%c\r\n", STX, ETX);

	if (!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 1000))
	{
		SendCommString(szCmdString);
		if (!ReceiveString(szData, 1000))
		{
			SendCommString(szCmdString);
			if (!ReceiveString(szData, 1000))
			{
				AddStringToStatus(_T("LAN HDCP. Error"));
				return FALSE;
			}
		}
	}

	szData.MakeUpper();
	//	nFind = szData.Find("DV_");
	nFind = szData.Find("DLAN");
	AddStringToStatus(szData);
	if (nFind != -1)
	{
		str = szData.Mid(nFind + 4, 11);
		

		//		dRevData = (m_strReceive[6] - 0x30)*100 + (m_strReceive[7] - 0x30)*10 + (m_strReceive[8] - 0x30)*1 + (m_strReceive[9] - 0x30)*0.1;
		//		dRevData = (szData.GetAt(0) - 0x30)*100 + (szData.GetAt(1) - 0x30)*10 + (szData.GetAt(2) - 0x30)*1 + (szData.GetAt(3) - 0x30)*0.1;
	}
	else {
		str = "";
		bRev = FALSE;
	}


	return bRev;
}

BOOL CVF1000::CheckLanClear(CString &str, int nWaitLimit)
{
	CString szCmdString;
	CString szData;
	CString szTemp;
	double dRevData = 0.0;
	int nFind = -1;
	int nR_CH = 0;
	int nR_CH2 = 0;
	BOOL bRev = TRUE;

	if (!m_bPortOpen) return FALSE;

	m_ctrlVF1000.ClearPort();

	szCmdString.Format("%cCLANCLEAR%c\r\n", STX, ETX);

	if (!SendCommString(szCmdString))
	{
		return FALSE;
	}
	if (!ReceiveString(szData, 1000))
	{
		SendCommString(szCmdString);
		if (!ReceiveString(szData, 1000))
		{
			SendCommString(szCmdString);
			if (!ReceiveString(szData, 1000))
			{
				AddStringToStatus(_T("LAN CLEAR. Error"));
				return FALSE;
			}
		}
	}

	szData.MakeUpper();
	//	nFind = szData.Find("DV_");
	nFind = szData.Find("DREADY");
	if (nFind != -1)
	{
		str = szData.Mid(nFind + 4, 11);
		CString szMsg = "LAN CLEAR: PASS";
		AddStringToStatus(szMsg);
		//g_pView->SaveMessageList(szMsg);
	}
	else 
	{
		//g_pView->SaveMessageList("LAN CLEAR: FAIL");
		AddStringToStatus("LAN CLEAR: FAIL");
		str = "";
		bRev = FALSE;
	}

	return bRev;
}
