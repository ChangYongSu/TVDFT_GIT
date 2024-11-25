
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently



#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
//#ifndef WINVER                  // Allow use of features specific to Windows 95 and Windows NT 4 or later.
//#define WINVER 0x0400           // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#endif
//
//#ifndef _WIN32_WINNT            // Allow use of features specific to Windows NT 4 or later.
//#define _WIN32_WINNT 0x0400     // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#endif
//
//#ifndef _WIN32_WINDOWS          // Allow use of features specific to Windows 98 or later.
//#define _WIN32_WINDOWS 0x0410   // Change this to the appropriate value to target Windows Me or later.
//#endif
//
//#ifndef _WIN32_IE               // Allow use of features specific to IE 4.0 or later.
//#define _WIN32_IE 0x0400        // Change this to the appropriate value to target IE 5.0 or later.
//#endif
//
//#ifndef WINVER                  // Allow use of features specific to Windows 95 and Windows NT 4 or later.
//#define WINVER 0x0500           // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
//#endif

#ifndef WINVER                  // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0601           // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT            // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0601     // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS          // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0601   // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE               // Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0601        // Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS    // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>             // MFC core and standard components
#include <afxext.h>             // MFC extensions

#include <afxdisp.h>            // MFC Automation classes

#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif                          // _AFX_NO_AFXCMN_SUPPORT



//{{AFX Measurement Studio
//#include "NiIncludes.h"
#include <afxcontrolbars.h>

#include "SerialPort.h"


//#include <opencv2\opencv.hpp>

#include "afxwin.h"

//#include <allheaders.h>
//#include <baseapi.h>
#include "afxcmn.h"

#include <afxsock.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>

//using namespace cv;

//#define __OK_CHECK_FAST_MODE__


//#define __COM_DEBUG_MODE__

#define __UPH_SAVE_DEBUG_MODE__
//#define __START_DEBUG_MODE_

#define __G_MES_SET_MODE__

//#define _CHANGE_MODEL_DEBUG__
	

#define WM_COMM_BREAK_DETECTED	  WM_USER+1 
#define WM_COMM_CTS_DETECTED	  WM_USER+2 
#define WM_COMM_DSR_DETECTED	  WM_USER+3 
#define WM_COMM_ERR_DETECTED	  WM_USER+4 
#define WM_COMM_RING_DETECTED	  WM_USER+5
#define WM_COMM_RLSD_DETECTED	  WM_USER+6
#define WM_COMM_RXCHAR			  WM_USER+7
#define WM_COMM_RXFLAG_DETECTED	  WM_USER+8
#define WM_COMM_TXEMPTY_DETECTED  WM_USER+9
#define WM_COMM_TXCHAR            WM_USER+10
#define WM_COMM_DISPLAY           WM_USER+11
#define WM_PROGRESS_MSG           WM_USER+12
#define WM_COMM_RXCHAR_PCB			  WM_USER+13
#define WM_COMM_RXCHAR_LED			  WM_USER+14
#define WM_COMM_RXCHAR_SCAN			  WM_USER+15
#define WM_COMM_RXCHAR_ETHERNET			  WM_USER+16


#define COM_MODE_IDLE		0
#define COM_MODE_WAIT_ACK	1


#define COM_ACK_WAIT		0
#define COM_ACK_OK			1
#define COM_ACK_BUSY		2

//}}AFX Measurement Studio



#define PATH_BASE_DIR "D:\\DFT\\DFT_Server"

#define LINE_CM45 0
#define LINE_CM89 1

typedef struct
{
	CString m_Company;
	CString m_Code;
	int m_Repeat;
	CString m_Name;

}Remocon_DataT;

typedef struct
{
	CString m_ModelName;
	int m_Cnt;
	Remocon_DataT Data[1000];
}Recomon_ModelT;



typedef struct
{
	CString strMesDsn;
	CString strMesUserId;
	CString strMesPassWord;
	bool bCommDisplay;
	CString 	strStepName;
	//int nProvider;
	CString strMesAvn;
	int nOobType;
	int nAvnCommandListID;
	CString strOobFlag;
	CString strProductionLine;
}DB_Set_DataT;

extern DB_Set_DataT CurrentSet;


#define FILE_STEP_PROCESS			1
#define FILE_STEP_TEST1				2
#define FILE_STEP_TEST2				3
#define FILE_STEP_TEST3				4
#define FILE_STEP_TEST4				5
#define FILE_STEP_REMOCON			6
#define FILE_STEP_MES				7
#define FILE_STEP_PRE_PROCESS		8
#define FILE_STEP_CAMERA		9

//#define MAX_TEST_SEQ_NUM	12


#define START_BARCODE_SCAN		0
#define START_START_SW_ON		1
#define START_SENSOR_START_SW	2
//#define START_START_ERROR		3
//
//#define TEST_SEQ_TYPE_END		0
//#define TEST_SEQ_TYPE_DELAY		1
//#define TEST_SEQ_TYPE_MP_KEY	2
//#define TEST_SEQ_TYPE_PRE_KEY	3
//#define TEST_SEQ_TYPE_TEST_1	4
//#define TEST_SEQ_TYPE_TEST_2	5
//#define TEST_SEQ_TYPE_TEST_3	6
//#define TEST_SEQ_TYPE_TEST_4	7

#define TEST_SEQ_TYPE_END					0
#define TEST_SEQ_TYPE_DELAY					1
#define TEST_SEQ_TYPE_MP_KEY				2
#define TEST_SEQ_TYPE_PRE_KEY				3
#define TEST_SEQ_TYPE_TEST_2				4
#define TEST_SEQ_TYPE_TEST_3				5
#define TEST_SEQ_TYPE_MP_REPEAT_KEY			6
#define TEST_SEQ_TYPE_PRE_REPEAT_KEY		7
#define TEST_SEQ_TYPE_TEST_LAN				8
#define TEST_SEQ_TYPE_TEST_USB1				9
#define TEST_SEQ_TYPE_TEST_USB2				10
#define TEST_SEQ_TYPE_SET_AUDIO				11
#define TEST_SEQ_TYPE_CHECK_AUDIO			12
#define TEST_SEQ_TYPE_CHECK_CAMERA_OCR		13
#define TEST_SEQ_TYPE_CHECK_CAMERA_IMAGE	14
#define TEST_SEQ_TYPE_CHECK_AUDIO_WORK		15
#define TEST_SEQ_TYPE_CHECK_UART_COMMAND	16
#define TEST_SEQ_TYPE_CLEAR_LED				17
#define TEST_SEQ_TYPE_CHECK_LED				18
#define TEST_SEQ_TYPE_SET_MIC1				19
#define TEST_SEQ_TYPE_SET_MIC2				20
#define TEST_SEQ_TYPE_MP_REPEAT10_KEY		21
#define TEST_SEQ_TYPE_PRE_REPEAT20_KEY		22
#define TEST_SEQ_TYPE_CHECK_SUM_COMMAND		23
#define TEST_SEQ_TYPE_SET_DISPLAY			24
#define TEST_SEQ_TYPE_BEEP					25
#define TEST_SEQ_TYPE_CHECK_RL_BALANCE					26




#define ETHERNET_MODE_IDLE					0
#define ETHERNET_MODE_CREATE_SOCKET			1
#define ETHERNET_MODE_HOST_CHECK			2
#define ETHERNET_MODE_TIME_SEND				3
#define ETHERNET_MODE_EQIP_CONNECT			4
#define ETHERNET_MODE_RUN					5
#define ETHERNET_MODE_WAIT					6

#define __COMMAND_CHECKMODE_IDLE			0
#define __COMMAND_CHECKMODE_SELECT			1




#define PLC_SOCKET_ID		6

#define PLC_NG_MODE		0
#define PLC_OK_MODE		1
#define PLC_CHECK_MODE	2
#define PLC_INIT_MODE	99

#define PLC_ALARM_UPH_ADDRESS	600

#define	ALARM_FLAG_ADDRESS	0//
#define	ALARM_1_WARN_NUMBER_ADDRESS	3// 1st Alarm / Warning Number	3
#define	ALARM_2_WARN_NUMBER_ADDRESS	4// 2nd Alarm / Warning Number	4
#define	ALARM_3_WARN_NUMBER_ADDRESS	5// 3rd Alarm / Warning Number	5
#define	ALARM_4_WARN_NUMBER_ADDRESS	6// 4th Alarm / Warning Number	6
#define	ALARM_5_WARN_NUMBER_ADDRESS	7// 5th Alarm / Warning Number	7
//#define	8//								8
//#define	9//								9
#define	TOTAL_QTY_1_ADDRESS		10// TOTAL  Quantity				10
#define	TOTAL_QTY_2_ADDRESS		11// TOTAL  Quantity				11
#define	OK_QTY_ADDRESS			12// OK  Quantity					12
#define	NG_QTY_ADDRESS			13// NG  Quantity					13
#define	OK_1_QTY_ADDRESS		14// DFT#1 OK  Quantity			14
#define	NG_1_QTY_ADDRESS		15// DFT#1 NG  Quantity			15
#define	OK_2_QTY_ADDRESS		16// DFT#2 OK  Quantity			16
#define	NG_2_QTY_ADDRESS		17// DFT#2 NG  Quantity			17
#define	OK_3_QTY_ADDRESS		18// DFT#3 OK  Quantity			18
#define	NG_3_QTY_ADDRESS		19// DFT#3 NG  Quantity			19
#define	OK_4_QTY_ADDRESS		20// DFT#4 OK  Quantity			20
#define	NG_4_QTY_ADDRESS		21// DFT#4 NG  Quantity			21
#define	OK_5_QTY_ADDRESS		22// DFT#5 OK  Quantity			22
#define	NG_5_QTY_ADDRESS		23// DFT#5 NG  Quantity			23
#define	OK_6_QTY_ADDRESS		24// DFT#6 OK  Quantity			24
#define	NG_6_QTY_ADDRESS		25// DFT#6 NG  Quantity			25




typedef struct{
	int		nSeqType;
	int		nDelay;
	CString strMP_Key;
	
	int		nTestNumber;

}TestSeq_T;

typedef struct{
	int			nStartType;
	CString		strPre_Key;
	TestSeq_T	aSeqData[20];

}TestProcess_T;

//TestProcess_T g_TestProcess;



//CString lCheckName[6]= {_T("HDMI AUD_R"), _T("HDMI AUD_L"), _T("OPTICAL_R"), _T("OPTICAL_L"), _T("POTABLE_R"), _T("POTABLE_L")};


typedef struct{
	BOOL		nEnable;
	int			nChannel;
	int			nSourceInput;
	double		dRefVoltageMin;
	double		dRefVoltageMax;
	double		dFrequencyMin;
	double		dFrequencyMax;

}TestPort_T;

//TestPort_T SetPortTest_1[6];
//TestPort_T SetPortTest_2[6];


#define IR_REMOTE_RF 0
#define IR_REMOTE_CA 1

//
//typedef struct{
//	CString 		strModelName;
//	TestProcess_T	TestProcess;
//	TestProcess_T	PreTestProcess;
//	TestPort_T		SetPortTest_1[6];
//	TestPort_T		SetPortTest_2[6];
//	TestPort_T		SetPortTest_3[6];
//	TestPort_T		SetPortTest_4[6];
//	bool			bLevelTestSkip1;
//	bool			bLevelTestSkip2;
//	bool			bLevelTestSkip3;
//	bool			bLevelTestSkip4;
//	int				nRemoconSigType;
//	int				nTestTimes;
//	CString			strRemoconFileName;
//	bool			bMesEnable;
//} TestModelDataT;

//int gRemoconSigType;
//extern TestModelDataT	gTestSeqData;



typedef struct{
	
	CString			strStepName;	
	CString			strStepWork;	
	CString			FunctionCommand[100];
	int				nCount;
	int				bEnable;
	int				m_Result;
} StepDataT;

typedef struct{
	CString 		strModelName;
	CString 		strSeqFileName;
	CString 		strRemoconFileName;
	UINT			Frequency[10];

	int				nRemoconSigType;
	bool			bMesEnable;
	unsigned char	CheckSum[2];
	CString 		strCameraIPAddress;
	int				PreTestCnt;
	StepDataT		PreStepList[50];	
	int				TestCnt;
	StepDataT		TestStepList[100];	

	int				m_TotalResult;

}TestSeqDataT;


extern TestSeqDataT	gTestSeqData;


typedef struct {

	int				nEnable;
	int				nCheck;
	CString			strStepNo;
	CString			strStepName;
	CString			strResult;
	ULONG			nBackColor;
	ULONG			nForeColor;
} SeqStepListDataT;

typedef struct{

	int m_cCheck_PLC_Auto_Mode;
	int m_cCheck_PLC_Manual_Mode;
	int m_cCheck_PLC_Start_Run;
	int m_cCheck_PLC_Pause;
	int m_cCheck_PLC_STOP;
	int m_cCheck_PLC_Warn;
	int m_cCheck_PLC_Alarm;
	int m_cCheckSkipFFC;
	int m_cCheck_PLC_Data_PCB_Inside;
	int m_cCheckJigUpStatus;
	int m_cCheckJigDownStatus;//:0
	int m_cCheckACPowerStatus;//:0

} SubPLC_DataT;

typedef struct{
	CString				strEQPID;//

	CString 			strModelName;	
	CString 			strModelSuffix;
	CString 			strRobotMainMode;
	CString 			strWipID_SetSN;
	
	int 				strSystemStatus;
	CString 			strRunStatus;	
	CString 			strComStatus;	

	SubPLC_DataT		m_PLC_Data;
	int					SeqStepCount;
	int					nSeqSel;
	SeqStepListDataT	 m_SeqList[500];

} SaveLogDataT;

extern SaveLogDataT gSaveInfo[12];
extern SaveLogDataT gLoadDataInfo[12];
extern unsigned int gDataCount;

typedef struct
{
	UINT nUSB_CableCountMax;
	UINT nUSB_CableCount;
	UINT nPort_CableCountMax;
	UINT nPort_CableCount;
	UINT nCM4CheckerCountMax;
	UINT nCM5CheckerCountMax;
	UINT nCM4CheckerCount;
	UINT nCM5CheckerCount;
	UINT nMIC_CableCountMax;
	UINT nMIC_CableCount;
}CableManagerT;

extern CableManagerT gCableManager;
//extern int  g_Result;
class CModelData :public CObject
{

public:

	CModelData()
	{
		/*		m_szChassisName			= _T("");
				m_szModelName			= _T("");
				m_szSeqFilePath			= _T("");
				m_szModelInfoFilePath	= _T("");
				m_szRefImgFolder		= _T("");*/
	}

	~CModelData()
	{
	}

	// Variable
	CString m_szChassisName;
	CString m_szModelName;
	CString m_szSeqFilePath;
	CString m_szModelInfoFilePath;
	CString m_szRefImgFolder;
	CString m_szDivision;
	CString m_szSeqFilePath2;
};

extern int g_nRemoconSelNum;//

extern bool gbManualScopeMode;

extern int g_ManualId[8];

extern UINT g_NIAuioBoardFail;

//extern UINT g_nPort_Set;
//extern UINT g_nBaud_Set;
//extern UINT g_nParity_Set;
//
//extern UINT g_nPort_Pcb;
//extern UINT g_nBaud_Pcb ;
//extern UINT g_nParity_Pcb;
//
//extern UINT g_nPort_LED ;
//extern UINT g_nBaud_LED ;
//extern UINT g_nParity_LED ;



extern CSerialPort m_ComPortScanner;
extern UINT g_nPort_Scanner;
extern UINT g_nBaud_Scanner;
extern UINT g_nParity_Scanner;





//extern CString g_strPassword;
//extern CString g_strMesTns;
//extern CString g_strMesAvn;
//extern CString g_strMesUserID;
//extern CString g_strMesPassword;
//extern CString g_strMesLine;
//extern CString g_strMesInspStep;

extern UINT g_nPortReopen[6];
extern UINT g_nReplyTimeOut;
extern UINT g_nTransTimeOut;
extern UINT g_nLocalPortNumber[6];
//extern UINT		g_nRemotePortNumber;
//extern UINT		g_nRemotePortNumberCM45;
//extern UINT		g_nRemotePortNumberCM89;
//extern CString	g_nRemoteIPAddress;
//extern CString g_strEQPID;
//extern CString g_strEQPName;
//extern CString g_strCEID;
//extern CString g_strRPTID;
//
//extern CString g_strProcID;
//
//extern CString g_strWOID;
//extern CString g_strORGID;
//extern CString g_strLineID;
//extern CString g_strSetID;
//extern CString g_strModelID;
//extern CString g_strModelName;
//extern CString g_strModelSuffix;
extern int g_nTargetCnt;

extern int g_nOKCnt;
extern int g_nNGCnt;
//extern cv::VideoCapture vcap; 
//extern cv::Mat gCuptureimage;
extern bool gbShutdown;
extern int g_CameraFound;
extern bool gbCapture;
extern int g_CameraCount;




//extern UINT g_nRemotePortType;// = LINE_CM45;

extern UINT g_nNetPortNumber[6];
extern CString g_nRemoteIPAddress[6];

extern CString		g_strPLCIPAddress;
extern UINT		g_nPLCPortNumber;


extern CTypedPtrList<CPtrList, CModelData*>	ModelList;
extern CStringArray g_Divisions;
extern CString strAlarmList[500];