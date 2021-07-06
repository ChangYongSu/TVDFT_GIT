#ifndef	__JIG_CTL_H_
#define __JIG_CTL_H_

#include "CommThread.h"

#define	STX		0x02
#define	ETX		0x03

#define ERAPHONE_SIGNAL		0x01
#define COMPONENT_SIGNAL	0x02
#define HDMI_USB_SIGNAL		0x04
extern int g_AutoControlProcess;
extern int g_Pause_AutoControlProcess;
extern int g_AutoControlProcessNoTEST;
extern int g_AutoControlProcessWaitTime;

class CJigCtrl : public CObject
{
public : 

	//Variable
	BOOL			m_bPortOpen;
	CCommThread		m_ctrlJigCom;
	BYTE			m_strReceive[225];
	BYTE			m_strSend[125];
	CString			m_sReceive;
	CString			m_strErrMsg;
	int				m_nPortID;
	HANDLE			m_hReadEvent;
	BYTE			m_nCylinder;

	BOOL			m_bGet_JigReady;
	BOOL			m_bAcPowerOnOff;

	BOOL			m_bJigUpStatus;
	BOOL			m_bJigDownStatus;

	
	//Function
	CJigCtrl();
	~CJigCtrl();
	BOOL Create(CString sComPort, DWORD nBaudRate);
	void PortClose();
	BOOL Init();
	BOOL SendByte(BYTE Data);
	BOOL SendCommString(BYTE* str, int nLength);
	BOOL ReceiveString(CString &str, int nWaitLimit);
	BOOL Set_ACPower(BYTE Data);
	BOOL Set_TestStatus(BYTE Data);
	BOOL Set_TestResult(BYTE Data);
	BOOL Set_CylinderDOWN();
	BOOL Set_CylinderUP();
	BOOL Set_CylinderHDMI_USB(BOOL bHdmiUsb);
	BOOL Set_CylinderEarphone(BOOL bEarPhone);
	BOOL Set_CylinderComp(BOOL bComp);
	BOOL Set_Cylinder(BOOL bEarPhone, BOOL bComp);
	BOOL Get_JigReady();
	BOOL Set_Grabber_Power(BOOL bGrabPower);
	BOOL Set_IF_Pack_Reset(BOOL bReset);
	BOOL Set_Gender_Reset(BOOL bReset);
private:
	void Delay();
};


extern CJigCtrl gJigCtrl;

#endif	// __JIG_CTL_H__