#pragma once


#include "CommThread.h"

#define	STX		0x02
#define	ETX		0x03

#define _VF_ONLY_TYPE	0
#define _IR_MIX_TYPE	1

// CVF1000 명령 대상입니다.

class CVF1000 : public CObject
{
public:

	//Variable
	BOOL			m_bPortOpen;
	CCommThread		m_ctrlVF1000;
	BYTE			m_strReceive[225];
	CString			m_strErrMsg;
	int				m_nPortID;

	BOOL			m_bSystemMixType;


	UINT		m_nChNo;		
	UINT		m_nCheckTime;	
	double		m_dSpec1;
	double		m_dSpec2;

	CVF1000();
	virtual ~CVF1000();

	BOOL Create(CString sComPort, DWORD nBaudRate);
	void PortClose();
	BOOL Init();
	BOOL SendByte(BYTE Data);
	BOOL SendCommString(CString str);
	BOOL GetVoltageData(int nCH, double &dData);
	BOOL GetVoltageData2(int nCH, int nWaitLimit, double &dData1, double &dData2);
	BOOL GetFrequencyData(int nCH, double &dData);
	BOOL GetFrequencyData2(int nCH, int nWaitLimit, double &dData1, double &dData2);
	BOOL ReceiveString(CString &str, int nWaitLimit);

	BOOL GetIRBlasterData(CString &str, int nWaitLimit);
	BOOL CheckLanHDCP(CString &str, int nWaitLimit);
	BOOL CheckLanClear(CString &str, int nWaitLimit);
	int GetVF_Count();

private:
	void Delay();

};

extern CVF1000 gVF1000Ctrl;

