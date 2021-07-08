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

#define		HM_IIC_WR_REG1 		0x1
#define		HM_IIC_WR_REG2 		0x2

#define		HM_DDC2B_WR_BYTE	0x12
#define		HM_DDC2B_WR_PAGE	0x13
#define		HM_IIC_WR_CMD1 		0x10
#define		HM_IIC_WR_CMD2 		0x11
#define		HM_IIC_WR_CMD3 		0x14
#define		HM_IIC_RD_CMD1 		0x20
#define		HM_IIC_RD_CMD2 		0x21
#define		HM_DDC2B_RD_BYTE	0x22
#define		HM_DDC2B_RD_CMD		0x23
#define		HM_IIC_RD_CMD3 		0x24
#define		HM_IIC_RD_BUF	    0x30
#define		HM_IIC_CTRL_SET_REG	0x40
#define		HM_IIC_CH_SEL		0x60
#define		HM_GET_VER 			0x80

#define SET_I2C_WRITE_OPTION_CMD		0x14
#define SET_I2C_WRITE_OPTION_SUB_CMD	0x0A
#define SET_I2C_SEND_CMD				0x10
#define SET_I2C_READ_CMD				0x24
#define SET_I2C_TRANSFER_DATA_CMD		0x30
#define I2C_PACKET_LENGTH				0x84

#define HOST_ADD0	0x50		//Address
#define HOST_ADD1	0x51		//Address

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
	BYTE			m_nHdcpKeyWriteData[400];
	BYTE			m_nHdcpKeyReadData[400];
	BYTE			m_nEdidData_A[256];
	BYTE			m_nEdidData_D[256];
	BYTE			m_nEdidData_H[256];
	BYTE			m_nEdidData_I[256];
	BYTE			m_nEdidReadData[256];
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
	int CheckI2cMaxMin(int nTarget,int nMargin, int nByte);
	int CheckI2cLimit(int nTarget,int nMargin, int nByte);
	int ReadI2cData();

	int ReadI2c2bData();
	int WriteI2c2bData();

	//+add kwmoon 080807
	int SetDDC2AB();
	int SetChannel(CString szData1);
	int SendCmd(CString szData1,CString szData2);
	int SendCmd2(int nCmd, int nData);

	//+add kwmoon 080911
//	int CheckVersion(CString szData1,CString szData2, int nVersion);

	//+add kwmoon 080819
	int WakeUp();

	//+add kwmoon 080826
	int ReadI2cNbyteData(int nByte);
	int ReadI2cNbyteData2(int nByte);
	int ReadI2c2bNbyteData(int nByte, CProgressCtrl& Progress);
	int ReadI2c2bNbyteData2(int nAdd, int nByte);
	int WriteI2c2bNbyteData(int nByte, CProgressCtrl& Progress);
	int WriteI2c2b_PageData(int nLAdd, CString sData);

	int ReadOption(int nType, int nOptionNo);
	int WriteOption(int nType, int nOptionNo,int nOptionValue);
	int ReadOptionAll();
	int WriteOptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6);
	
	//+add kwmoon 080923
	int ReadVersion();
	int MNT_ReadVersion();

	//+add PSH 081204
	BOOL  InputSourceChange(BYTE nCode);

	//+add 080216(Modification No1)
	BOOL GetModuleVoltage(double& dVolt);

	//+add 090420(Modification No1)
	int	  SetExtAnalogGrabberMode(int nAnalogGrabberMode);

	
	BOOL Check_Option(int nIndex, int nOptionValue);
	BOOL Check_OptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6);
	int EDID_Download_ResultCheck();
	int EDID_Download();


 	BOOL SetIRcode(BYTE nCode);
	BOOL WriteI2c_Data(CString sData, int nWriteByte);
	BOOL WriteHdcpKey();
	BOOL ReadHdcpKey();
/*	BOOL WriteEdid_2AB(int nIndex);
	BOOL WriteEdid_2B(int nIndex);
	BOOL ReadEdid_2AB(int nIndex);
	BOOL ReadEdid_2B(int nIndex);
*/
	int GetFWVersion();
};


extern CI2cAdcCtrl I2cAdcCtrl;

#endif	// __I2C_ADC_CTRL_H_