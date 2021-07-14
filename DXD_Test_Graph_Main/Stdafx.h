
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

#ifndef WINVER                  // Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0600           // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT            // Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0600     // Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS          // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0610   // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE               // Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0600        // Change this to the appropriate value to target IE 5.0 or later.
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


#include <opencv2\opencv.hpp>

#include "afxwin.h"

//#include <allheaders.h>
//#include <baseapi.h>
#include "afxcmn.h"

#include <afxsock.h>

using namespace cv;

//#define __OK_CHECK_FAST_MODE__


//#define __COM_DEBUG_MODE__

#define __MESSAGE_DEBUG_MODE__
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


//#define PATH_BASE_DIR "C:\\SM Digital\\AudioTester_1100AP"
#define PATH_BASE_DIR "D:\\SM Digital\\DXD_Tester"

//#define LINE_CM45 0
//#define LINE_CM89 1

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
#define TEST_SEQ_TYPE_CHECK_RL_BALANCE		26

#define TEST_SEQ_TYPE_CHECK_MESSAGE_MANUAL		27
#define TEST_SEQ_TYPE_SW_VERSION				28
#define TEST_SEQ_TYPE_CHECK_WIFI				29
#define TEST_SEQ_TYPE_GET_DARK_IMAGE			30
#define TEST_SEQ_TYPE_GET_BRIGHT_IMAGE			31
#define TEST_SEQ_TYPE_GET_AED_VALUE				32
#define TEST_SEQ_TYPE_GET_MACADDRESS			33
#define TEST_SEQ_TYPE_SET_MANUAL_EXPOSURE_MODE	34
#define TEST_SEQ_TYPE_FACTORY_RESET				35





#define ETHERNET_MODE_IDLE					0
#define ETHERNET_MODE_CREATE_SOCKET			1
#define ETHERNET_MODE_HOST_CHECK			2
#define ETHERNET_MODE_TIME_SEND				3
#define ETHERNET_MODE_EQIP_CONNECT			4
#define ETHERNET_MODE_RUN					5
#define ETHERNET_MODE_WAIT					6




#define _TIME_ID_RESFRESH_	10




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
	//CString 		strRemoconFileName;
	//UINT			Frequency[10];

	//int				nRemoconSigType;
	//bool			bMesEnable;
	//unsigned char	CheckSum[2];
	//CString 		strCameraIPAddress;
	//int				PreTestCnt;
	//StepDataT		PreStepList[50];	
	int				TestCnt;
	StepDataT		TestStepList[100];	

	int				m_TotalResult;

}TestSeqDataT;


extern TestSeqDataT	gTestSeqData;


typedef struct{
	CString 		strModelName;
	CString 		strWipID_SetSN;
	CString			strTestResult;
	bool			bMesEnable;	
	bool			bMesSts;
	float				CheckTime;
	float				CycleTime;
	CString 		strTestDiscription;

} SaveLogDataT;

extern SaveLogDataT gSaveInfo;



//extern int  g_Result;

//extern CString g_strModelName;
//extern int g_nModelSelNum;
//extern CString g_strRemoconFileName;
extern int g_nRemoconSelNum;//

extern bool gbManualScopeMode;

extern int g_ManualId[8];

//extern UINT g_NIAuioBoardFail;
//extern UINT g_nPort_Set;
//extern UINT g_nBaud_Set;
//extern UINT g_nParity_Set;
//extern UINT g_nPort_Pcb;
//extern UINT g_nBaud_Pcb ;
//extern UINT g_nParity_Pcb;
//
//extern UINT g_nPort_LED ;
//extern UINT g_nBaud_LED ;
//extern UINT g_nParity_LED ;
//
extern UINT g_nPort_Scanner;
extern UINT g_nBaud_Scanner;
extern UINT g_nParity_Scanner;





extern CString g_strPassword;
extern CString g_strMesTns;
extern CString g_strMesAvn;
extern CString g_strMesUserID;
extern CString g_strMesPassword;
extern CString g_strMesLine;
extern CString g_strMesInspStep;

extern UINT g_nPortReopen;
extern UINT g_nReplyTimeOut;
extern UINT g_nTransTimeOut;
extern UINT g_nLocalPortNumber;
extern UINT		g_nRemotePortNumber;
//extern UINT		g_nRemotePortNumberCM45;
//extern UINT		g_nRemotePortNumberCM89;
extern CString	g_nRemoteIPAddress;
extern CString g_strEQPID;
extern CString g_strEQPName;
extern CString g_strCEID;
extern CString g_strRPTID;

extern CString g_strProcID;

extern CString g_strWOID;
extern CString g_strORGID;
extern CString g_strLineID;
extern CString g_strSetID;
extern CString g_strModelID;
extern CString g_strModelName;
extern CString g_strModelSuffix;

extern int g_nOKCnt;
extern int g_nNGCnt;
//extern cv::VideoCapture vcap; 
//extern cv::Mat gCuptureimage;
//extern bool gbShutdown;
extern int g_CameraFound;
//extern bool gbCapture;
extern int g_CameraCount;


extern CString g_nSetIPAddress;
extern UINT g_nSetPortNumber;

extern UINT	g_FPGA_Ver_event;
extern UINT	g_FPGA_Ver_major;
extern UINT	g_FPGA_Ver_minor;
 		
extern UINT	g_DSERVER_Ver_event;
extern UINT	g_DSERVER_Ver_major;
extern UINT	g_DSERVER_Ver_minor;
extern UINT	g_DSERVER_Ver_release;
 		
extern UINT	g_Microchip_Ver_event;
extern UINT	g_Microchip_Ver_major;
extern UINT	g_Microchip_Ver_minor;
 		
extern UINT	g_AVR_Ver_event;
extern UINT	g_AVR_Ver_major;
extern UINT	g_AVR_Ver_minor;

extern BOOL g_MesEnable;

//extern CSerialPort m_ComPort;
//extern CSerialPort m_ComPortPCB;
//extern CSerialPort m_ComPortLED;
extern CSerialPort m_ComPortScanner;
