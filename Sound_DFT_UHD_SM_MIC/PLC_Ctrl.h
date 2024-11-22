// Gmes.h: interface for the CGmes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(PLC_H_INCLUDED_)
#define PLC_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLC_NG_MODE		0
#define PLC_OK_MODE		1
#define PLC_CHECK_MODE	2
#define PLC_INIT_MODE	99

class CPLC_Ctrl  
{
public:
	CPLC_Ctrl();
	virtual ~CPLC_Ctrl();

	CSocket m_pClient;


	BOOL	m_bPLCCom;
	CString	m_sReceive;
	BOOL	m_bPLC_Online;
	BOOL	m_bPLC_WaitCount;
	BOOL	m_bPLC_RegWaitCount;
	BOOL	m_bPCB_Onload_JIG;
	
	BOOL	m_cResetControl;


	//CString	m_sTransactionID;
	//CString	m_sWorkOrderID;
	//CString	m_sOrganizationID;
	//CString	m_sLineID;
	//CString	m_sEquipmentID;
	//CString	m_sOperationID;
	//CString	m_sModelID;
	//CString	m_sModelSuffixName;
	//int		m_nOrganizationID;
	//int		m_nDftSeq;
	CString	m_sPLCProcessMsg;
	CString	m_sRunProcessMsg;
	CString	m_sRunProcessMsgCommand;


	CString	m_sErrorMsg;

	CString	m_sModelName;
	int m_cCheckLoad;
	int m_cCheckUnLoad;
	int m_cCheckSkipFFC;
	int m_cCheckPCBOnLoadSensor;
	int m_cCheckTestPass;
	int m_cCheckTestNg;
	int m_cCheckReconnect;
	int m_cCheckLoadFinish;
	int m_cCheckUnLoadFinish;
	int m_cCheckReconnectFinish;

	int m_cCheckSetAlarm;

	int m_cCheck_PLC_Auto_Mode;//:0
	int m_cCheck_PLC_Manual_Mode;//:1
	int m_cCheck_PLC_Start_Run;//:2
	int m_cCheck_PLC_Pause;//:3
	int m_cCheck_PLC_STOP;	//:4
	int m_cCheck_PLC_Warn;	//:5
	int m_cCheck_PLC_Alarm;//:6

	int m_cCheckUnLoadConfirmSet;
	int m_cCheck_LoadFinish_NG_ConfirmSet;
	int m_cCheckRobotMoving;//_PLC_ADDRESS_ROBOT_MOVING_STATUS
	int m_cCheckJigDownStatus;
	int m_cCheckJigUpStatus;
	int m_cCheck3TimesErrorStatus;
	   
	int m_cCheck_PLC_Data_PCB_Inside;//:10
	int m_cCheck_PLC_Data_DummyCut;//:11
	int m_cCheck_PLC_Data_OK;//:12
	int m_cCheck_PLC_Data_NG;//:13


	int m_nUserRetry;
	int m_nCheckAbortRetry;
	int m_nTestResult;
	int m_nFirstVideoNG;
	int m_nCurrentVideoNG;
	int m_nTestVideoCount;
	int m_NG_ConfirmSet;

	int m_NG_StepNo;
	int m_NG_StepNoOld;
	int m_NG_StepNo3Times;

	int m_nTestStepNGCount;
	int m_nTestStepNGCountList[100];

//	char	m_sReadBuffer[4096];	
	char	m_sReadBuffer[8192];	
	int		m_nReadSize;


	BOOL    SoketCom_Creat(CString sIpAdd, int nPort);
	void	SoketCom_Close();
	BOOL	SendCommString(CString str);
	BOOL	ReceiveCommString(int nWaitLimit, CString& sReadString);

	void	DisplyComStatus(CString sItem, BOOL bFlag);
	
	BOOL	Paser_ReceiveData(CString sReceiveData);

	int WriteFullData(UINT Address, char* Data, int lDataLen);
	int WriteCommand(UINT Address, char* Data, int lDataLen);	
	int ReadCommand(UINT Address, int lDataLen);
	int ReadRegData(UINT Address, int lDataLen);


	void CommandModelName(CString l_strEditModelName);
	void CommandClear();
	void CommandLoad();
	void CommandPassUnload();
	void CommandNgUnload();
	void CommandLoadFast();
	void CommandPassUnloadFast();
	void CommandNgUnloadFast();

	void CommandSkipFFC();
	void CommandConnectFFC();

	void CommandRetryUnload();
	void CommandFfcRetry();
	void CommandRead();
	void CommandRegRead();
	void Command3TimesError();
	void Command3TimesErrorClear();
	   	  
};

extern CPLC_Ctrl gPLC_Ctrl;

#endif // !defined(PLC_H_INCLUDED_)
