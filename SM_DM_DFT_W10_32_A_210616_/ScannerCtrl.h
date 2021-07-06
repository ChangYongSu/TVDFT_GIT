
#ifndef	__SCANNERCTRL_H_
#define __SCANNERCTRL_H_

#include "CommThread.h"

#define	STX		0x02
#define	ETX		0x03

#define	START_C		0x3e		// '>'
#define ZERO_C		0x0
#define ADDR_C		0x04		//Address
#define	END_C		0x0d		//Enter


class CScannerCtrl : public CObject
{
public : 

	//Variable
	BOOL			m_bPortOpen;
	int				m_nPortID;
	CCommThread		m_ctrlScannerCtrl;
	BYTE			m_strReceive[225];
	CString 		m_strSendStr;
	CString			m_strErrMsg;

	//Function
	CScannerCtrl();
	~CScannerCtrl();
	BOOL Create(CString sComPort, DWORD nBaudRate);
	void PortClose();
	BOOL Init();
	BOOL SendByte(BYTE Data);
	BOOL SendCommString(CString str);
	BOOL ReceiveCommString(int nRev, CString& sReadString);
	BOOL ReceiveCommString(int nRev, CString sWaitLimit, CString& sReadString);
	BOOL ReceiveCommString(int nRev, int nWaitLimit, CString& sReadString);

private:
	void Delay();
};


extern CScannerCtrl ScannerCtrl;

#endif	// __SCANNERCTRL_H_