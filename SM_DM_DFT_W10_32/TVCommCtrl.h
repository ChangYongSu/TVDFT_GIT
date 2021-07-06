
#ifndef	__TVCOMMCTRL_H_
#define __TVCOMMCTRL_H_

#include "CommThread.h"

#define	STX		0x02
#define	ETX		0x03

#define	START_C		0x3e		// '>'
#define ZERO_C		0x0
#define ADDR_C		0x04		//Address
#define	END_C		0x0d		//Enter

//+change PSH 080416
//#define T_NOTEST	-1
//#define T_PASS		1
//#define T_FAIL		2
//#define T_ABORT		3
//#define T_TIMEOUT	4

//+del kwmoon 080904
/*
#define T_NOTEST	-1
#define T_FAIL		0
#define T_PASS		1
#define T_ABORT		2
#define T_TIMEOUT	3
*/
//-

//ADC CMD TYPE
#define ADC_MODE_IN			0x0
#define ADC_START			0x1
#define ADC_MODE_OUT		0x2
#define ADC_RESULT_READ		0x3
#define NORMAL_TVCMD		0x4

//ADC RESULT TYPE
#define ADC_OFFSET1			0x1
#define ADC_OFFSET2			0x2
#define ADC_GAIN			0x3

#define BE_VER			1
#define FE_VER			2
#define MICOM_VER		3
#define DSP_VER			4
#define SUB_DSP_VER		5
#define EQ_VER			6
#define MEQ_VER			7
#define TOUCH_VER		8
#define DEMO_VER		9
#define DJ_SOUND_VER	10
#define WIRELESS_TX_VER	11
#define WIRELESS_RX_VER	12
#define BT_VER	13
#define HDMI_VER	14
#define CHECKSUM_VER	15


//+add kwmoon 080318
class CTvCommI2CData
{
public:

	CString m_szInputSource;
    CString m_szAddress;		// Low Address of EEPROM
	CString	m_szResultAddress;
    CString m_szExpectedData;	// Expected Data

//	CTvCommI2CData* m_pNext;

public:
	CTvCommI2CData()
	{
		m_szInputSource		= _T("");
		m_szAddress			= _T("");	// High Address of EEPROM
		m_szResultAddress	= _T("");
		m_szExpectedData	= _T("");	// Expected Data
	}

	~CTvCommI2CData(){};
};

/*
class CTvCommI2C
{

public:

	int m_nNoItem;

	CTvCommI2CData* m_pData;

public:

	CTvCommI2C()
	{
		m_nNoItem = 0;
		m_pData	  = NULL;	
	}

	~CTvCommI2C(){};

	void Add(int nLowAddress,int nHighAddress,int nExpectedData);
	void GetData(int nIndex,int* nLowAddress,int* nHighAddress,int* nExpectedData);
	void RemoveAll();
};
*/

class CTVCommCtrl : public CObject
{

public : 

	CCommThread		m_ctrlTVCommCtrl;
	
	BYTE			m_nReceiveData[256];
	CString			m_sReceive;
	int				m_nReceiveLength;

	BYTE			m_nRevData[256];
	int				m_nRevSize;
	int				m_nRevLength;

	BYTE			m_nSendData[5];
	CString			m_strSendStr;
	
	CString			m_strCmd1;
	CString			m_strCmd2;
	CString			m_strData;
	
	int				m_nSetId;
//	int 			m_nWaitLimit;
	int				m_nPortID;
	int				m_nResult;

	BOOL			m_bPortOpen;
	BOOL			m_bThreadRunning;
	BOOL			m_bExitThread;

//	HANDLE			m_hAdcThreadKillEvent;
	HANDLE			m_hReadEvent;
	
	//+change 090213(Modification No1)
//	HANDLE			m_hThread;
	CWinThread*		m_pAdcThread;

	CString			m_strErrMsg;		// TVCommPort Test Error Message
	
	CString			m_sCheckCmd;		
	UINT			m_nWaitLimit;
	// RS232C Command Array
	CStringArray	m_aCmd1;
	CStringArray	m_aCmd2;
	CUIntArray		m_aSetId;
	CStringArray	m_aData;
	CUIntArray		m_aWaitLimit;
	//add 090417
	CUIntArray		m_aCheckType;      // 1 : Offset1 Check, 2 : Offset2 Check, 3 : Gain Check
	CUIntArray		m_aCheckOption;    // 0 : Spec Check, 1 : MinMax Check, 2 : All Check

	UINT			m_nCheckType;		// 1 : Offset1 Check, 2 : Offset2 Check, 3 : Gain Check
	UINT			m_nAvgMargin;		// Avg Data Margin(+-)
	CString			m_strTargetAvg;		// Avg Data Target Value

	CTypedPtrList<CPtrList, CTvCommI2CData*>	m_TvCommI2CData;
	
	BYTE			m_nAdcI2cData[256];
	//add 090417
	UINT			m_nResultCheckType;		// 0 : Old_Type, 1 : New Type
	UINT			m_nResultTarget[9];
	UINT			m_nResultMargin[3];		// ADC Data Margin(+-)
	BOOL			m_bResultRead;
	UINT			m_nResultData[9];

	CStringArray	m_aMacAdd;

	int				m_nIfTest_Data1;
	int				m_nIfTest_Data2;

	int				m_bCheckReadVersion;
public :

	CTVCommCtrl();
	~CTVCommCtrl();
	
	BOOL Create(CString sComPort, DWORD nBaudRate);
	BOOL SendCommand(BYTE nCmd);
	BOOL Init();
	BOOL SendCommString(CString str);
	BOOL ReceiveCommString(int nWaitLimit);

	void PortClose();
	void ResetArray();

	UINT IfTest(BYTE sCmd1, int nMaxDelay, BOOL bIfCheck = TRUE);

	BOOL Firmware_VersionCheck(int nIndex, CString sVersion);
	BOOL Firmware_VersionRead(int nIndex, CString &sVersion);
	
	BOOL Send_TVControl_Command(BYTE nCommand,int nWait);


		//+add PSH 081204
	BOOL  InputSourceChange(BYTE nCode);
	BOOL PortClear();

	BOOL Set_Mic_OnOff(BOOL bFlag, int nWait);
	//+add PSH 090417
	BOOL Check_Value(int nData, int nTarget, int nMargin);
	void Set_MAC_Address(CString sData);
	BOOL MAC_AddressCheck(BOOL bWifi);
	BOOL MAC_AddressRead(CString &sMac_add, BOOL bWifi);
	BOOL Serial_NumberCheck();

	BOOL Send_ResultKey( BOOL bResult) ;

	BOOL Check_MicDiffLevel();

private:

	void Delay();
};


extern CTVCommCtrl TVCommCtrl;

#endif	// __TVCOMMCTRL_H_
