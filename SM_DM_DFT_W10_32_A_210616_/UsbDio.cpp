// UsbDio.cpp: implementation of the CUsbDio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datsys.h"
#include "UsbDio.h"
#include "global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern int g_nRunningProcessNo;

CUsbDio gUsbDioCtrl;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUsbDio::CUsbDio()
{
	m_bUsbDio = FALSE;
	nDIO_Out = 0;
	m_ftHandle = NULL;
}

CUsbDio::~CUsbDio()
{
	Close();
	m_bUsbDio = FALSE;

}

BOOL CUsbDio::GetNumDevices(int& nNum)
{
	FT_STATUS ftStatus; 
	DWORD numDevs;
	CString sTemp;

//	FT_DEVICE_LIST_INFO_NODE *devInfo; 
	
	//Get the number of devices currently connected
	ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY); 
	if (ftStatus == FT_OK) { 
		// FT_ListDevices OK, number of devices connected is in numDevs
		sTemp.Format("USB_DIO : FT_ListDevices OK- %d devices connected", numDevs);
		AddStringToStatus(sTemp);
		nNum = (int)numDevs;
		return TRUE;
	} 
	else 
	{ 
		// FT_ListDevices failed 
		AddStringToStatus("USB_DIO : FT_ListDevices failed");
		nNum = 0;
		return FALSE;
	}
}




//****************************************************************************************
BOOL CUsbDio::Create()
{
	FT_STATUS ftStatus; 
	int iDevice;
	int nNum = 0;
//	char *Buffer[3]; // more than enough room! 
//	DWORD numDevs;

	m_bUsbDio = FALSE;

	Close();
	if(!GetNumDevices(nNum)){return FALSE;}

	if(g_nRunningProcessNo == 1)
	{
		if(nNum > 0)
		{
			iDevice = 0;
		}
		else{
			return FALSE;
		}
	}
	else{
		if(nNum > 1)
		{
			iDevice = 1;
		}
		else{
			return FALSE;
		}
	}
	if(nNum == 1)
	{
		if(g_nRunningProcessNo == 2){	return FALSE; }

	/*	Buffer[0] = new char[64];
		Buffer[1] = NULL;
		ftStatus = FT_ListDevices("USB <-> Serial", &nNum, FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION); 
		if (ftStatus != FT_OK) {	return FALSE; }
	*/
		ftStatus = FT_OpenEx((PVOID)(LPCTSTR)"USB <-> Serial", FT_OPEN_BY_DESCRIPTION, &m_ftHandle);
	}
	else{
		ftStatus = FT_Open(iDevice, &m_ftHandle);
	}
	if (ftStatus == FT_OK) 
	{ 
		// FT_Open OK, use ftHandle to access device 
		AddStringToStatus("USB_DIO : Devices Open OK");
	} 
	else 
	{ 
		// FT_Open failed 
		AddStringToStatus("USB_DIO : Devices Open failed");
		return FALSE;
	}
	ftStatus = FT_SetBitMode(m_ftHandle, 0xFF, 0x01);
	if (ftStatus != FT_OK) 
	{ 
		// FT_SetBitMode FAILED!
		AddStringToStatus("USB_DIO : SetBitMode FAILED!");
		return FALSE;
	}
	ftStatus = FT_SetBaudRate(m_ftHandle, 9600);
	if (ftStatus != FT_OK) 
	{ 
		// FT_SetBaudRate Failed		
		AddStringToStatus("USB_DIO : SetBaudRate Failed!");
		return FALSE;
	}
	ftStatus = FT_SetDataCharacteristics(m_ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
	if (ftStatus != FT_OK) 
	{ 
		AddStringToStatus("USB_DIO : SetDataCharacteristics Failed!");
		return FALSE;
	}

	ftStatus = FT_SetFlowControl(m_ftHandle, FT_FLOW_NONE, 0, 0);
	if (ftStatus != FT_OK) 
	{ 
		AddStringToStatus("USB_DIO : SetFlowControl Failed!");
		return FALSE;
	}

	ftStatus = FT_SetTimeouts(m_ftHandle, 2000, 0);
	if (ftStatus != FT_OK) 
	{ 
		AddStringToStatus("USB_DIO : SetTimeouts Failed!");
		return FALSE;
	}

 	m_bUsbDio = TRUE;

	return TRUE;
}

BOOL CUsbDio::Close()
{
	if(FT_Close(m_ftHandle) != FT_OK)
	{
	//	AddStringToStatus("USB_DIO : Close Failed");
		return FALSE;
	}

	m_bUsbDio = FALSE;
	
	return TRUE;
}

BOOL CUsbDio::WriteByte(BYTE nValue)
{
	FT_STATUS ftStatus; 
	DWORD BytesWritten; 

	if(!m_bUsbDio) return FALSE;

	ftStatus = FT_Write(m_ftHandle, &nValue, 1, &BytesWritten); 
	if (ftStatus != FT_OK) 
	{ 
		// FT_Write Failed 
		AddStringToStatus("USB_DIO : FT_Write Failed");
		return FALSE;
	}
	
	nDIO_Out = nValue;

	return TRUE;
}

BOOL CUsbDio::Set_Relay(int nNo, BOOL bOnOff)
{
	BYTE nValue;
	BOOL bRet;

	if(!m_bUsbDio) return FALSE;
	
	if(nNo == 1){
		if(bOnOff){
			nValue = nDIO_Out | 0x01;
		}
		else{
			nValue = nDIO_Out & 0xfe;
		}
	}
	else if(nNo == 2){
		if(bOnOff){
			nValue = nDIO_Out | 0x02;
		}
		else{
			nValue = nDIO_Out & 0xfd;
		}
	}
	else if(nNo == 3){
		if(bOnOff){
			nValue = nDIO_Out | 0x04;
		}
		else{
			nValue = nDIO_Out & 0xfb;
		}
	}
	else if(nNo == 4){
		if(bOnOff){
			nValue = nDIO_Out | 0x08;
		}
		else{
			nValue = nDIO_Out & 0xf7;
		}
	}
	else{
		return FALSE;
	}

	bRet = WriteByte(nValue);

	return bRet;
}
BOOL CUsbDio::Set_RelayOnOff(int nWait)
{
	BYTE nValue;
	BOOL bRet;

	if(!m_bUsbDio) return FALSE;
	
	nValue = nDIO_Out | 0x0f;
	bRet = WriteByte(nValue);
	Sleep(nWait);

	nValue = nDIO_Out & 0xf0;
	bRet = WriteByte(nValue);

	return bRet;
}