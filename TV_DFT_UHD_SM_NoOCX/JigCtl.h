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
	BOOL Set_CylinderAll(BYTE Data);
	BOOL Get_JigReady();
	BOOL Set_Grabber_Power(BOOL bGrabPower);
	BOOL Set_IF_Pack_Reset(BOOL bReset);
	BOOL Set_Gender_Reset(BOOL bReset);


	/////////////////////////////////////////////////////
	//JIG Count Control
	BOOL			m_bID_Ctrl_PortOpen;
	CCommThread		m_ID_Ctrl_Com;
	BYTE			m_strID_Ctrl_Receive[225];
	BYTE			m_strID_Ctrl_Send[125];
	CString			m_strID_Ctrl_ErrMsg;
	CString			m_sID_Ctrl_Receive;
	int				m_nID_Ctrl_PortID;
	int				m_nID_Ctrl_ReceiveCnt;


	CString		m_strID_CompanyInfo;
	CString		m_strID_ChassisInfo;
	CString		m_strID_DateInfo;
	CString		m_strID_SerialNo;
	CString		m_strID_JigFullInfo;
	int			m_nJigID_UseCount;



	BOOL ID_Ctrl_Create(CString sComPort, DWORD nBaudRate);
	void ID_Ctrl_PortClose();
	
	//BOOL ID_Ctrl_SendByte(BYTE Data);
	BOOL ID_Ctrl_SendCommString(BYTE* str, int nLength);
	BOOL ID_Ctrl_IncCount();
	BOOL ID_Ctrl_ReadID(CString &str, int nWaitLimit);
	BOOL ID_Ctrl_ReadCount(CString &str, int nWaitLimit);
	BOOL ID_Ctrl_ReceiveString(CString &str, int nWaitLimit);


	/////////////////////////////////////////////////////
	//DPMS Monitoring
	BOOL			m_bDPMS_PortOpen;
	CCommThread		m_DPMS_Com;
	BYTE			m_strDPMS_Receive[225];
	BYTE			m_strDPMS_Send[125];
	CString			m_strDPMS_ErrMsg;
	CString			m_sDPMS_Receive;
	int				m_nDPMS_PortID;
	int				m_nDPMS_ReceiveCnt;
	int				m_nDPMS_5CheckFlag;
	double		m_fDPMS_WattData;




	BOOL DPMS_Create(CString sComPort, DWORD nBaudRate);
	void DPMS_PortClose();

	//BOOL ID_Ctrl_SendByte(BYTE Data);
	BOOL DPMS_SendCommString(BYTE* str, int nLength);
	double DPMS_Read_Check(int Manual = 0);
	BOOL DPMS_ReceiveString(int nCount,  int nWaitLimit);
	bool CJigCtrl::DPMS_Count_Check();

private:
	void Delay();
};


extern CJigCtrl gJigCtrl;

#endif	// __JIG_CTL_H__