// UsbDio.h: interface for the CUsbDio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBDIO_H__F081A270_D98D_4F42_927C_D44AA4604753__INCLUDED_)
#define AFX_USBDIO_H__F081A270_D98D_4F42_927C_D44AA4604753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ftd2xx.h"

class CUsbDio  
{
public:
	CUsbDio();
	virtual ~CUsbDio();

	BOOL m_bUsbDio;
	BYTE nDIO_Out;

	BOOL Set_Relay(int nNo, BOOL bOnOff);
	BOOL WriteByte(BYTE nValue);
	BOOL Set_RelayOnOff(int nWait);

	BOOL GetNumDevices(int& nNum);
	
	BOOL Create();
	BOOL Close();	
protected:
	FT_HANDLE m_ftHandle;


};

extern CUsbDio gUsbDioCtrl;

#endif // !defined(AFX_USBDIO_H__F081A270_D98D_4F42_927C_D44AA4604753__INCLUDED_)
