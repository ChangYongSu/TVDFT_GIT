#ifndef	__I2C_ADC_CTRL_H_
#define __I2C_ADC_CTRL_H_

#include "CommThread.h"

#define	STX			0x02
#define	ETX			0x03
#define	START_C		0x3e		// '>'
#define ZERO_C		0x0
#define ADDR_C		0x04		//Address
#define	END_C		0x0d		//Enter
#define CMD_LENGTH	14

class CI2cAdcCtrl : public CObject
{
public : 

	BOOL			m_bPortOpen;
	CCommThread		m_ctrlI2cAdcCtrl;
	//+change kwmoon 080919
//	BYTE			m_strReceive[225];
	BYTE			m_strReceive[1024];
	int				m_nPortID;
	CString			m_strErrMsg;		
	CString			m_szI2cAdcReadString;
	UINT			m_nI2cAdcSeqNo;
	int				m_nData1;
	int				m_nData2;
	int				m_nData3;

	//+add kwmoon 080804
	int				m_nReadByte;

	//+add kwmoon 080826
	int				m_aReadData[256];
	int				m_aWriteData[256];
	CString			m_szCurrentStatusMsg;
public :

	CI2cAdcCtrl();
	~CI2cAdcCtrl();
	
	BOOL Create(CString sComPort, DWORD nBaudRate);
	void PortClose();
	BOOL Init();
	BOOL SendCommString(CString str);
	BOOL ReceiveCommString(int nRev, unsigned nWaitLimit, CString& sReadString);
	BOOL SendI2cCmd(CString szCmdString);
	void ClearI2cAdcData();

	//+add kwmoon 080724
	int SetI2cClkSpeed(int nClkSpeed);
	int SetI2cPacketDelay(int nDelay);
	int SetI2cReadByteDelay(int nDelay);
	int SetI2cWritByteDelay(int nDelay);

	//+add kwmoon 080804
	int CheckI2cValue(int nData1);
	int CheckI2cMaxMin(int nTarget,int nMargin);
	int CheckI2cLimit(int nTarget,int nMargin);
	int ReadI2cData();

	int ReadI2c2bData();
	int WriteI2c2bData();

	//+add kwmoon 080807
	int SetDDC2AB();
	int SetChannel(CString szData1);
	int SendCmd(CString szData1,CString szData2);

	//+add kwmoon 080911
//	int CheckVersion(CString szData1,CString szData2, int nVersion);

	//+add kwmoon 080819
	int WakeUp();

	//+add kwmoon 080826
	int ReadI2cNbyteData(int nByte, CProgressCtrl& Progress);
	int ReadI2c2bNbyteData(int nByte, CProgressCtrl& Progress);
	int WriteI2c2bNbyteData(int nByte, CProgressCtrl& Progress);

	//+add kwmoon 080923
	int ReadOption(int nType, int nOptionNo);
	int WriteOption(int nType, int nOptionNo,int nOptionValue);
	
	//+add kwmoon 080923
	int ReadVersion();

	//+add PSH 081204
	BOOL  InputSourceChange(BYTE nCode);

	//+add 080216(Modification No1)
	BOOL GetModuleVoltage(float& dVolt);

	//+add 090420(Modification No1)
	int	  SetExtAnalogGrabberMode(int nAnalogGrabberMode);

	//+add PSH 090604
	BOOL Check_Option(int nIndex, int nOptionValue);
	//+add PSH 090611
	int EDID_Download();
	int EDID_Download_ResultCheck();
	// add 100101
	BOOL SetIRcode(BYTE nCode);
};


extern CI2cAdcCtrl I2cAdcCtrl;

#endif	// __I2C_ADC_CTRL_H_