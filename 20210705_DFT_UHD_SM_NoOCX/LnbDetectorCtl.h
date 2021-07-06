#ifndef	__LNB_DETECTOR_CTL_H_
#define __LNB_DETECTOR_CTL_H_

#include "CommThread.h"

#define	STX		0x02
#define	ETX		0x03

#define END_TERMINAL_MODE		0x04
#define START_TERMINAL_MODE		0x05

class CLnbDetector : public CObject
{
public : 

	//Variable
	BOOL			m_bPortOpen;
	CCommThread		m_ctrlLnbDetector;
	BYTE			m_strReceive[225];
	CString			m_strErrMsg;
	int				m_nPortID;
	
	//Function
	CLnbDetector();
	~CLnbDetector();
	BOOL Create(CString sComPort, DWORD nBaudRate);
	void PortClose();
	BOOL Init();
	BOOL SendByte(BYTE Data);
	BOOL SendCommString(CString str);
	BOOL GetVoltageData(double &dData1, double &dData2);
	BOOL GetFrequencyData(double &dData1, double &dData2);
	BOOL ReceiveString(CString &str, int nWaitLimit);
private:
	void Delay();
};


extern CLnbDetector gLNBDetectorCtrl;

#endif	// __LNB_DETECTOR_CTL_H__