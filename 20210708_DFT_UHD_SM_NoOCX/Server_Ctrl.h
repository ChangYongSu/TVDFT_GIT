// Server_Ctrl.h: interface for the CServer_Ctrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SERVER_MONITOR_H_INCLUDED_)
#define SERVER_MONITOR_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define PLC_NG_MODE		0
//#define PLC_OK_MODE		1
//#define PLC_CHECK_MODE	2
//#define PLC_INIT_MODE	99
typedef struct {
	
	CString 	strHeader;
	CString 	strCaption;
	CString		strForeColor;
	CString		strBackColor;
}DispalyInfo_T;

class CServer_Ctrl
{
public:
	CServer_Ctrl();
	virtual ~CServer_Ctrl();

	CSocket m_pClient;


	BOOL	m_bServer_CtrlCom;
	CString	m_sReceive;
	BOOL	m_bServer_Ctrl_Online;
	int	m_bServer_Ctrl_WaitCount;
	int	m_bServer_Ctrl_RegWaitCount;
	//BOOL	m_bPCB_Onload_JIG;
	
	BOOL	m_cResetControl;
	int	m_bCom_NGCount;
	int	m_bCom_RetryCount;

	UINT	m_bCom_SndCount;
	UINT	m_bCom_RcvCount;
	int		m_ServerControlCommand;
	CString m_ServerControlData;
	   
	CString	m_sErrorMsg;

//	char	m_sReadBuffer[4096];	
	char	m_sReadBuffer[8192];	
	int		m_nReadSize;
	CString m_strMesSend;


	BOOL    SoketCom_Creat(CString sIpAdd, int nPort);
	void	SoketCom_Close();
	BOOL	SendCommString(CString str);
	BOOL	ReceiveCommString(int nWaitLimit, CString& sReadString);

	void	DisplyComStatus(CString sItem, BOOL bFlag);

	int WriteSend(CString strData);
	void LinkTestResponse(); //S2F4 
	void ControlCommandReceive(CString lstrCommand, CString lstrData);//S2F103
	void ControlCommandAck();//S2F104
	void LinkTestSend(); //S2F3
	void SendStatus(); //S2F101
	void SendMsgCtrl(int ServerCmdID);


	DispalyInfo_T m_DisplayInfo[30];
	CString SetDisplayString();
	CString SetComDisplayString();
	void SendModelList();
	void ServerChangeModelAck(int lOk);
	void ServerReportTestResult(int lOk);
	void ReportPCB_Clear();//
	void ReportPCB_Check();

	CString m_szSeqFilePath;
	CString m_szModelInfoFilePath;
	CString m_szRefImgFolder;

	CString m_sSelChassis;
	CString m_sSelModel;
		   	  
};

extern CServer_Ctrl gServer_Ctrl;

#endif // !defined(PLC_H_INCLUDED_)
