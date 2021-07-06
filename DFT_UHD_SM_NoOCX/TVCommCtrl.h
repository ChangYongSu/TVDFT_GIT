
#ifndef	__TVCOMMCTRL_H_
#define __TVCOMMCTRL_H_

#include "global.h"
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

//+add PSH 080417
#define CPU_VER		1
#define MICOM_VER	2
#define USB_VER		3
#define MNT_VER		4


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
	
	BYTE			m_nReceiveData[2048];
	CString			m_sReceive;
	int				m_nReceiveLength;
	

	CString			m_strCmd1;
	CString			m_strCmd2;
	CString			m_strData;
	
	int				m_nSetId;
	int 			m_nWaitLimit;
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

	CString			m_strSendStr;
	CString			m_strErrMsg;		// TVCommPort Test Error Message
	
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
	UINT			m_nInputDetectItem;		
	UINT			m_nDetectMode;		

	CTypedPtrList<CPtrList, CTvCommI2CData*>	m_TvCommI2CData;
	
	BYTE			m_nAdcI2cData[256];
	//add 090417
	UINT			m_nResultCheckType;		// 0 : Old_Type, 1 : New Type
	UINT			m_nResultTarget[9];
	UINT			m_nResultMargin[3];		// ADC Data Margin(+-)
	BOOL			m_bResultRead;
	UINT			m_nResultData[9];
	
	//add 110215
	CStringArray	m_aMacAdd;
	CStringArray	m_aMacAdd2;
	CString				m_ErrorType;
	UINT			m_nHighSpeedCheckUSB ;

	BOOL			m_bStopBCM_TestThread;

	UINT			m_AutoCountPort[AUTO_COUNT_ID_MAX];

public :

	CTVCommCtrl();
	~CTVCommCtrl();
	

	int m_RcvTimeOut;


	BOOL Create(CString sComPort, DWORD nBaudRate);
	BOOL SendRun(CString sData);
	BOOL Init();
	BOOL SendByte(BYTE Data);
	BOOL SendCommString(CString str);
	BOOL SendRun(CString sCmd1, CString sCmd2, int nSetId, CString sData);
	BOOL ReceiveCommString(int nRev, CString sWaitLimit, CString& sReadString);
	BOOL ReceiveCommString(int nRev, int nWaitLimit, CString& sReadString);
	BOOL ReceiveCommString_BCM(int nRev, int nWaitLimit, CString& sReadString);
	BOOL ReceiveCommString_SN(int nRev, int nWaitLimit, CString& sReadString);

	void RunAdc();
	void PortClose();
	void ResetArray();

	static UINT StartAdcThread(LPVOID pParam); 
	static UINT BCM_TestThread(LPVOID pParam); 
	
	UINT AdcTest(CString sCmd1, CString sCmd2, int nSetId, CString sData,int nCheckType, int nMaxDelay, int nCheckOption = 0);

	//+add PSH 080417
	BOOL ADC_AdjConfirmation();
	BOOL Firmware_VersionCheck(int nIndex, CString sVersion);
	
	//+add PSH 080520
/*	BOOL ADC_Adj_ModeIn();
	BOOL ADC_Adj_Start(int nItem, int nWait);
	BOOL ADC_Adj_ModeOut();
*/
	BOOL ADC_Adj_ResultCheck(int nWait);
//	BOOL EDID_Download_ModeIn();
//	BOOL EDID_Download_Start(int nIdex, int nWait);
	BOOL EDID_Download(int nWait);
//	BOOL EDID_Download_ModeOut();
//	BOOL EDID_Download_ResultCheck(int &nAdjItem);
	BOOL TVLink_SetChannel_PAL(int nRFSource, int nCH, int nWait);
	BOOL Send_TVControl_Command(CString sCommand,int nWait);
	BOOL Send_TVControl_ACAS_ID(CString sCommand, CString &sRevString, int nWait);

		//+add PSH 081119
	BOOL Write_Option(int nIndex, int nOptionValue);
	BOOL Check_Option(int nIndex, int nOptionValue);

		//+add PSH 101011
	BOOL Write_OptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6, int nOption7);
	BOOL Check_OptionAll(int nOption1, int nOption2, int nOption3, int nOption4, int nOption5, int nOption6, int nOption7);

		//+add PSH 081204
	BOOL  InputSourceChange(BYTE nCode);
	BOOL PortClear();

	//+add PSH 090325
	BOOL LAN_Port_Check(int nWait);
	BOOL ESN_Download_Check(int nWait);
	//+add PSH 090415
	BOOL Set_Mic_OnOff(BOOL bFlag, int nWait);
	//+add PSH 090417
	BOOL Check_Value(int nData, int nTarget, int nMargin);
	//+add PSH 091118
	BOOL CI_Download_Check1(int nWait);
	BOOL CI_Download_Check2(int nWait);
	//+add PSH 091124
	BOOL RJP_Port_Check(int nWait);
	BOOL Magic_Remocon_Check(int nWait);
	BOOL Wireless_Ready_Check(int nWait);
	//+add PSH 101011
	BOOL CI_Card_Check(int nWait);
	BOOL CI_ECP_Download_Check(int nWait);
	BOOL Marlin_Download_Check(int nWait); 
	BOOL BuiltIn_Wifi_Check(int nWait); 

	//+add PSH 101108
	BOOL Set_LNB_OnOff(BOOL bFlag, int nWait);
	BOOL Widevine_Download_Check(int nWait); 

	//+add PSH 101115
	BOOL RF_Emitter_Check(int nWait);
	//+add PSH 101124
	BOOL Set_3DMode_OnOff(BOOL bFlag, int nWait);
	//+add PSH 110215
	void Set_MAC_Address(CString sData);
	BOOL MAC_AddressCheck(int nWait);
	BOOL USB_HighSpeedCheck(int nWait);
	void Set_Chgeck_USB(UINT nUSBData);
	BOOL Set_HeadPhone_OnOff(BOOL bFlag, int nWait);

	BOOL BCMTest(int nWaitLimit);
	BOOL EyeQ_Check(int nWait);
	BOOL BAT_Ver_Check(CString sBatVersion, int nWait);
	BOOL Key_Check(int nIndex);
	BOOL CNTV_Key_DL_Check();
	BOOL HDCP_Tx_20_DL_Check();
	BOOL HDCP_Rx_20_DL_Check();
	BOOL HDCP_Rx_14_DL_Check();
	BOOL DTCP_Rx_Key_DL_Check();
	BOOL ATSC_30_Key_DL_Check();
	BOOL MFi_Key_DL_Check();
	BOOL Google_Key_DL_Check();

	BOOL Check_Version(CString sReadData, CString sTarget);

	void SaveComLog(CString sComData);

	BOOL Input_Detect_Check(int nIndex, int nOptionValue);
	BOOL Auto_Port_Detect(int nIndex = AUTO_COUNT_ID_MAX);
	int  ModelSNo_Read(int &nKeyVal, BOOL bFlag = TRUE); // 모델명 또는 S/N 데이터를 수신
	BOOL ModelSNo_Write(CString sData, BOOL bFlag = TRUE); // 모델명 또는 S/N 데이터를 수신
	int ModelSNo_Read(int &nKeyVal, CString &sRtnData, BOOL bFlag = TRUE);
	BOOL Open_MacAdd_List();


private:

	void Delay();
};


extern CTVCommCtrl TVCommCtrl;

#endif	// __TVCOMMCTRL_H_
