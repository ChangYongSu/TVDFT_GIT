#if !defined(AFX_DEFINES_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_)
#define AFX_DEFINES_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#ifdef _DEBUG
//#define ERR_DEBUG
#endif

#define _TVCOM_FLAG_SN				0x1		
#define _TVCOM_FLAG_MODEL				0x00		


//===========
// Pass/Fail
//===========

//+add kwmoon 080904
#define TEST_NOTEST				-1

#define TEST_FAIL				0x0		
#define TEST_PASS				0x1		
#define TEST_ABORT				0x2
#define TEST_TIMEOUT			0x3
#define GRAB_PASS				0x1
#define GRAB_FAIL				0x2
#define GRAB_ABORT				0x3

#define MONO_256COLOR			1
#define COLOR_24BIT				2

//#define WIDTHBYTES(bits)		(((bits)+31)/32*4) 
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

//============
// Image Type
//============
#define SNAP_IMAGE				0
#define LOAD_IMAGE				1
#define PROC_IMAGE				2
#define MASK_IMAGE				3
#define INTERNAL_PROC_IMAGE		4
#define HDMI_IMAGE				5


//===============
// Main View Type
//===============
#define _MAIN_VIEW_GRAB_IMAGE				0
#define _MAIN_VIEW_LOAD_IMAGE				1
#define _MAIN_VIEW_PROC_IMAGE				2
#define _MAIN_VIEW_HDMI_IMAGE				3


//===========
// Test Type
//===========
#define POSITION_TEST			1
#define SIZE_TEST				2
#define RGB_TEST				3
#define BRIGHT_TEST				4
#define OSD_TEST				5
#define NOISE_TEST				6
#define SIGNAL_LINE_TEST		7
#define MOVING_PIC_TEST			8
#define CAPTION_TEST			9
#define DIGITAL_EYE_TEST		10
#define BIAS_TEST				11
#define LAN_TEST				12
#define AUDIO_TEST				13
#define MANUAL_TEST				14
#define TV_COMM_TEST			15
#define ADC_TEST				16
//+add 090216(Modification No1)
#define MODULE_TEST				18
#define KEY_DL_TEST				19
#define BCM_CHIP_TEST			20
#define INPUT_DETECT_TEST		21
#define VF_MEASURE_TEST			22
#define AUTO_PORT_TEST			23
#define IR_BLASTER_TEST			24




#define NORMAL_BINARY			0
#define REVERSE_BINARY			1
	
#define NORMAL_PROCESS			0
#define REVERSE_PROCESS			1

#define LVDS_GRABBER			0
#define ANALOG_GRABBER			1
#define HDMI_GRABBER			2

#define NO_LOOP_FOR_AVERAGING	10 // No Loop for creation of average reference files

#define HD_WIDTH				1366
#define HD_HEIGHT				768
#define FHD_WIDTH				1920
#define FHD_HEIGHT				1080

#define ROI_COORDINATES			0
#define OSD_REGION_COORDINATES	1

//#define _TIME_ANALYSIS

struct ImageInfoStruct
{
	int nLVDSHeight;
	int nLVDSWidth;
	int nAnalogHeight;
	int nAnalogWidth;
	int nBitCount;
	int nGrabberMode;
	
	int nGrabImageLoaded;
	int nGrabSucceed;

	int nFullHd120Hz;

	int nHbackPorch;
	int nVbackPorch;
	int nUseExtAnalogGrabber;

	int nImageRotation;
	int nGrabber_FWVer;
};

// add 090526
struct DimmingTestStruct
{
	int nChannel;
	int nW_Add;
	int nW_Command;
	int nR_Add;
	int nR_Command;
	int nR_CheckSum;
	int nResult;
	int nResultValue;
};

struct UsbDioStruct
{
	int nChannel;
	int nCtrlData;
	int nResult;
};

struct MhlControlStruct
{
	int nFunction;
	int nChannel;
	int nCtrlData;
	int nResult;
};
struct BarcodeStruct
{
	int	 nSize;
	char sData[25];
};


struct USB_MIC_Struct
{
	int nMicCount;

	int nNewMicRequest;
	char sSystem1_MIC[25];
	char sSystem2_MIC[25];
	char sSystem3_MIC[25];

	char sSystem1_USBMIC[25];
	char sSystem2_USBMIC[25];
	char sSystem3_USBMIC[25];

};

//struct DaqStruct
//{
//	int	 nFuncIndex;
//	int  nStartChannel;
//	int  nEndChannel;
//	double dRetValue;
//	char sRetValue[25];
//};
typedef struct ProcessInformationStructure 
{
	char update_flag;
	char name[MAX_PATH];
	unsigned int id;
	unsigned int thread_counts;
	unsigned int parent_id;
	unsigned int priority;
	int cpu_usage;
	LONGLONG cpu_old_value;
	LARGE_INTEGER old_perf_time_100nsec;
} PIS;

//====================================
// Event Between DFT.exe and HwIF.exe
//====================================

#define EVENT_USBDIO_CTRL1		0
#define EVENT_USBDIO_CTRL2		1
#define EVENT_USBDIO_CTRL3		2

#define EVENT_MHL_CTRL1			3
#define EVENT_MHL_CTRL2			4
#define EVENT_MHL_CTRL3			5

#define EVENT_USBDIO_CTRL1_COMPLETE	0
#define EVENT_USBDIO_CTRL2_COMPLETE	1
#define EVENT_USBDIO_CTRL3_COMPLETE	2

#define EVENT_MHL_CTRL1_COMPLETE	3
#define EVENT_MHL_CTRL2_COMPLETE	4
#define EVENT_MHL_CTRL3_COMPLETE	5

#define EVENT_GRAB1_INFO		0
#define EVENT_GRAB2_INFO		1
#define EVENT_GRAB3_INFO		2
// LVDS
#define EVENT_GRAB1_IMG			3
#define EVENT_GRAB2_IMG			4
#define EVENT_GRAB3_IMG			5
// Analog
#define EVENT_GRAB4_IMG			6
#define EVENT_GRAB5_IMG			7
#define EVENT_GRAB6_IMG			8

#define EVENT_GRABBER1_FW		9
#define EVENT_GRABBER2_FW		10
#define EVENT_GRABBER3_FW		11


#define EVENT_GRAB1_COMPLETE		0
#define EVENT_GRAB2_COMPLETE		1
#define EVENT_GRAB3_COMPLETE		2
#define EVENT_GRAB4_COMPLETE		3
#define EVENT_GRAB5_COMPLETE		4
#define EVENT_GRAB6_COMPLETE		5
#define EVENT_GRABBER1_FW_COMPLETE	6
#define EVENT_GRABBER2_FW_COMPLETE	7
#define EVENT_GRABBER3_FW_COMPLETE	8

#define EVENT_BARCODE1		0
#define EVENT_BARCODE2		1
#define EVENT_BARCODE3		2

#define EVENT_DAQ1			3
#define EVENT_DAQ2			4
#define EVENT_DAQ3			5

#define EVENT_BARCODE1_COMPLETE	0
#define EVENT_BARCODE2_COMPLETE	1
#define EVENT_BARCODE3_COMPLETE	2

#define EVENT_DAQ1_COMPLETE		3
#define EVENT_DAQ2_COMPLETE		4
#define EVENT_DAQ3_COMPLETE		5

#define DAQ_FUNC_AC		1
#define DAQ_FUNC_DC		2
#define DAQ_FUNC_RES	3
#define DAQ_FUNC_FREQ	4
#define DAQ_FUNC_DC_M	5


//=====================
// User Window Message
//=====================

#define	WM_MASSAGE_LOG_DISPLAY			(WM_USER+10)	

#define UM_UPDATE_DISPLAY				WM_USER + 0x110
#define UM_CLOSE_STOP_PROCESSING_DLG	WM_USER + 0x120
#define UM_RESULT_DISPLAY				WM_USER + 0x130
#define UM_RUN_TEST						WM_USER + 0x140
#define UM_INIT_PERIPHERAL_DEVICE		WM_USER + 0x150
#define UM_INIT_DIALOG					WM_USER + 0x155
#define UM_CONTINUOUS_LVDS_GRAB			WM_USER + 0x160
#define UM_UPDATE_IMAGE_VIEW_DISPLAY	WM_USER + 0x170
#define UM_CALC_MOVE_SUB_WINDOW			WM_USER + 0x180
#define UM_CHANGE_ACTIVE_VIEW_TAB		WM_USER + 0x190
#define UM_UPDATE_ALL_VIEW_TAB			WM_USER + 0x200
#define UM_UPDATE_GRAB_STATUS			WM_USER + 0x210
#define UM_UPLOAD_MES_DATA				WM_USER + 0x220
#define UM_CLOSE_RESULT_DLG				WM_USER + 0x230
//+add 090220(Modification No3)
#define UM_START_LVDS_GRAB_THREAD		WM_USER + 0x240
//+add 090902
#define WM_KEYSTROKE					(WM_USER + 755)//This message is recieved when key is down/up
#define UM_UPDATE_TOOL_OPTION			WM_USER + 0x250
#define UM_CLOSE_TOOL_OPTION			WM_USER + 0x251
#define UM_MESSAGE_CLEAR			WM_USER + 0x252
#define UM_UPDATE_SUFFIX_SCAN			WM_USER + 0x253


//=================================
// Communication Monitoring Window
//=================================
#define REMOTE_COMM_LOG			0
#define SWITCHBOX_COMM_LOG		1

//==========
// ADC TYPE
//==========
#define ADC_TYPE_RS232C			0
#define ADC_TYPE_I2C			1

//==========
// Timer ID 
//==========
#define TIMER_AUTO_CONTROL_ID				0x80
#define TIMER_SERVER_CONTROL_ID				0x81
#define TIMER_SERVER_TIMEOUT_ID				0x82
#define TIMER_GMES_TIMEOUT_ID				0x83
#define TIMER_PLC_TIMEOUT_ID				0x84
#define TIMER_UPDATE_STATUS_INFO			0x100
#define TIMER_MEASURE_AUDIO_OUTPUT			0x200
#define TIMER_MEASURE_AUDIO_NEW_SEARCH		0x201
#define TIMER_CONTINUOUS_LVDS_GRAB			0x300
#define TIMER_UPDATE_ALL_SCREEN				0x400
#define TIMER_START_AUTO_GRAB				0x500	
#define TIMER_MES_DATA_UPLOAD				0x600	
#define TIMER_CHECK_LIST_UPDATE				0x601	
#define TIMER_CHECK_LIST_UPDATEDELAY			0x602
#define TIMER_CHECK_DEBUG_TEST			0x603

//=================
// I2C ADC KEYWORD 
//=================
#define I2C_TYPE			101
#define I2C_CMD				102
#define I2C_READ			103
#define I2C_CHK_VALUE		104
#define I2C_CHK_MAXMIN		105
#define I2C_CHK_LIMIT		106
#define I2C_2B_READ			107
#define I2C_2B_WRITE		108
#define I2C_CHANNEL			109
#define DDC_2AB				110

#define DDC_2B				111

#define I2C_CHK_VER			112

#define I2C_WRITE_TOOL_OPTION	113
#define I2C_WRITE_AREA_OPTION	114
#define I2C_CHK_VALUE2			115
#define I2C_CHK_MAXMIN2			116
#define I2C_CHK_LIMIT2			117
#define I2C_CMD2				118
#define I2C_WRITE				119

	
//===========================
// Start/Stop Continous Grab 
//===========================
#define STOP_CONT_GRAB	0
#define START_CONT_GRAB	1

typedef struct tag_LineID
{
	UINT	   nSeq;
    char*  pszLineName;             
	UINT	   nLineNo;					
} _LineID;

//======================
// Audio Measure Method
//======================
#define METHOD_AVSWITCHBOX	0
#define METHOD_SOUNDCARD	1

//============
// Option Tab
//============
#define AVSWITCHBOX_CONTROL_TAB		0
#define GRABBER_OPTION_TAB			1
#define USBDIO_CONTROL_TAB			2

//+add kwmoon 080724
#define NO_SOURCE_TYPE		50

//==================
// Error Code Table
//==================

//+add kwmoon 080724
#define I2C_COMM_OK							1

//#define ERROR_I2C_SEND					10000
//#define ERROR_I2C_RECEIVE				10001
//#define ERROR_I2C_RESPONSE				10002


//=======================
// Communication Keyword
//=======================
#define	STX		0x02
#define	ETX		0x03
#define CR		0x0d
#define LF		0x0a


//+add kwmoon 080925
#define TOOL_OPTION		1
#define AREA_OPTION		2

//==========
// TV COntrol TYPE
//==========
#define TV_CONTROL_RS232C		0
#define TV_CONTROL_I2C			1

// 090120 External DFT
#define INTERNAL_GRABBER		0
#define EXTERNAL_GRABBER		1
#define NOT_SELECTED			2

//+add 090206(Modification No1)
#define NTSC_DEFAULT_H_BPCH			40
//+change 090414(Modification No1)
//#define NTSC_DEFAULT_V_BPCH			20
#define NTSC_DEFAULT_V_BPCH			1

//+change 090407(Modification No1)
//#define PAL_DEFAULT_H_BPCH			10
//#define PAL_DEFAULT_V_BPCH			10
#define PAL_DEFAULT_H_BPCH			8
#define PAL_DEFAULT_V_BPCH			1

// 090120 External DFT
#define EXT_LVDS_GRABBER		1
#define EXT_ANALOG_GRABBER		2

#define OFFLINE			0
#define ONLINE			1

// Video Source
//+add 090420(Modification No1)
#define V_SRC_NTSC	0
#define V_SRC_PAL	1
#define V_SRC_SECAM 2

#endif // !defined(AFX_DEFINES_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_)
