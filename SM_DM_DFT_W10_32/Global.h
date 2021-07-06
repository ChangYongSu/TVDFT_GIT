#if !defined(AFX_GLOBAL_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_)
#define AFX_GLOBAL_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <time.h>
#include <math.h>

#include "IniAccess.h"
#include "resource.h"
#include "MainFrm.h"
#include "DATsysDoc.h"
#include "DATsysView.h"
#include "FlexUtil.h"
#include "HdmiGenerator.h"

#define PERI_REMO_ADDR	0x04

class CStep;
class CFunction;

//-----------------------
// Remote Code Structure
//-----------------------
class CRemoteCode:public CObject
{
public:
	CRemoteCode()
	{
	}
	~CRemoteCode()
	{
	}

	// Variable
	CString sName;		//Remote Control Button Name
	CString sCode;		//Remote Control Button Code
	CString sCustomCode;		//Remote Control Button Code
	CString sDesc;		//Remote Control Button Description
	int		nByte;		//Remote Control Button Use or Not Use
};

//-------------------
// Pattern Structure
//-------------------
class CPatternInfo:public CObject
{
public:
	CPatternInfo()
	{
	}
	~CPatternInfo()
	{
	}

	// Variable
	int		nPatternNo;
	int		nFilterSize;	
	int		nNoFiltering;	
	BOOL	bRepaint;	
	CString szRoiShape;
};

//+add kwmoon 081024
//----------------------
// Model Data Structure
//----------------------
class CModelData:public CObject
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
};

//+add PSH 081219
//----------------------
// Result Data Structure
//----------------------
class CResultData:public CObject
{

public:

	CResultData()
	{
		m_szTime	= _T("");
		m_szWipId	= _T("");
		m_szResult	= _T("");
		m_szData	= _T("");
	}

	~CResultData()
	{
	}

	// Variable
	CString m_szTime;
	CString m_szWipId;
	CString m_szResult;
	CString m_szData;
};

//----------------------
// StepResult Data Structure
//----------------------
class CStepResultData:public CObject
{

public:

	CStepResultData()
	{
		m_szTime	= _T("");
		m_szWipId	= _T("");
		m_szData	= _T("");
	}

	~CStepResultData()
	{
	}

	// Variable
	CString m_szTime;
	CString m_szWipId;
	CString m_szData;
};


typedef struct tag_DFT_DATA
{
//	CString		strWid;		// WIP ID
//	CString		strUid;		// User ID
	bool		bRun;		// false : Skip, true : Run
	UINT		nStepNo;	// Step Number;
	CString		strStepName;// Step Name
	CString		strResult;	// OK,NG/A,NG/V, NG/AV
	double		dTarget;	// Target Value
	double		dLowerLimit;// Lower Limit
	double		dUpperLimit;// Upper Limit
	CString		strUnit;	// Unit : Level , mV, %
	double		dTime;		// Elapsed Time
    CString		strMsg;		// Message
	
	//MDB Data
	CString		strGrabPath;
	CString		strRefPath;
	CString		strProcPath;
	CString		strAudioTarget;
	CString		strAudioMeasure;
	CString		strAudioMargin;

} DFTDATA;

class CEnvironmentData
{
public:
	CEnvironmentData();
	~CEnvironmentData(){};

public:
   
    int			nTotalLine, nTotalStep; 		// TOTAL LINES & TOTAL STEPS
    int			nTotalProduct, nOKProduct;      // Total Products & OK Product
    int			nRunType;              			// Current Run Mode 
	int			nPrintType;						// Current Print Out type
	int			nStepKey;						// StepKey
	int			nDisplayType;					// Current Display type => 0 : Main Grid -> Normal Grid
												//						   1 : Main Grid -> Detailed Grid
	int			nUseUserRetry; // 0: on,1:off
	
	int			nNoFrameForRefImageCreation;

	int			nPixelInMiddleRange;
	int			nNonBlackPixel;

	int			nNoUsedColors;

	int			nDelayPercentage;

    long		lTime;                 			// Last Compile Time
	BOOL		bIsRunning;						// Running state
	BOOL		bRunAbort;						// User abort key
    BOOL		bSystemBreak;                   // System break
	BOOL		bCompiled;						// Parsing result
	BOOL		bCommDisplay;					// rs232 
	BOOL		bIsRunMsg;						// thread 
	//+add PSH 081212
	BOOL		bMesUploadCheckTimer;
	
	//------------
	// Save Image
	//------------
	BOOL		bSaveGrabImg;					// TRUE : Auto Save Grab Image
	BOOL		bSaveProcImg;					// TRUE : Auto Save Processed Image
	BOOL		bSaveCaptionTestImg;
	BOOL		bSaveMovingPicTestImg;
	BOOL		bSaveIntermediateImage;

	BOOL		bSaveProcessingTimeData;

	BOOL		bSaveReviewData;
	BOOL		bSaveDetailResultData;

	BOOL		bSaveResult2Txt;				// Save Result to Text File	(default)
	BOOL		bSaveResult2Html;				// Save Result to Html File
	BOOL		bSaveResult2Csv;				// Save Result to Csv File

	UINT		nNoRepeatExecution;				// Save Number of repeat execution

	//------------------
	// File/Folder Path
	//------------------
	CString		sSeqPath;						// Sequence File 
	CString		sFullSeqPath;					// Full Sequence Path
    CString		sRemoconTitle;   				// REMOCON FILE NAME
    CString		sPatternTitle;   				// PATTERN FILE NAME
	CString		sViewerPath;					// File Viewer Program Path
	CString		sTestParamIni;
 	CString		sModelIni;						// Model Ini Path
	CString		sModelFolder;					// Model Folder
	CString		sGrabFolder;					// Grab Image Folder
	CString		sRefFolder;						// Ref Image Folder
	CString		sMaskFolder;					// Mask Image Folder
	CString		sResultFolder;					// Test Result Image Folder
	CString		sResultPath;					// Test Result File Path
	
	//+add kwmoon 080904
	CString		sServerFolder;
	//+add PSH 081029
	CString		sRefRootFolder;
	CString		sChassisName;
	CString		sModelName;
	//+add PSH 081211
	CString		sModelInfoFolder;

	//+add kwmoon 080716
	CString		sDetailResultDataPath;

	//+add kwmoon 080918
	CString		sResultSummaryPath;

	CString		sParmAdjGrabFolder;				
	CString		sParmAdjRefFolder;				
	CString		sParmAdjMaskFolder;				

	//+add kwmoon 081024
	CString		sModelListPath;					

	//------------------
	// COM Port
	//------------------
	CString		sPatternComPort;
	CString		sHDMIComPort;
	CString		sTVComPort;
	//CString		sTVComPort1;
	//CString		sTVComPort2;
	CString		sAvSwitchBoxComPort;
	CString		sScannerComPort;
	CString		sIrChkComPort;

	DWORD		wPatternBaudRate;
	DWORD		wHDMIBaudRate;
	DWORD		wAvSwitchBoxBaudRate;
	DWORD		wScannerBaudRate;
	DWORD		wIrChkBaudRate;

	CString		sHDMIRoiShape;

	BOOL		bUsePatternGen;
	BOOL		bUseTVCommPort;
	BOOL		bUseAVSwitchBox;
	BOOL		bUseScanner;
	BOOL		bUseIrChk;
	BOOL		bUseHDMIGen;

	BOOL		bUseTh;
	DWORD		wThBaudRate;
	CString		sThComPort;
//-

	UINT		nAdcType;

	CTypedPtrList<CPtrList, CRemoteCode*>	RemoteList;
	CTypedPtrList<CPtrList, CPatternInfo*>	PatternList;

	//+add kwmoon 081024
	CTypedPtrList<CPtrList, CModelData*>	ModelList;
	//+add kwmoon 081024
	CTypedPtrList<CPtrList, CResultData*>	MES_DataList;
	CTypedPtrList<CPtrList, CStepResultData*>	MES_StepDataList;

	UINT		nAudioSource;					// Audio Input Source 0: LineIn, 1: DigitalIn
	UINT		nNoRetry;						// Internal Retry Number
	BOOL		bTVCommPortEcho;				// TVCommPort Echo


	//------------------
	// MES Data
	//------------------
	UINT		nLocation;		// Location 0 : LG-PRI, 1 : LG-DD, 2 : JINSUNG, 3 : SG TECH
	UINT		nLineNo;		// Line Number 1 ~ 10
	UINT		nSystemNo;		// System Number 1 ~ 10
	UINT		nAuthority;		// Authority 0 : Developer, 1 : Administrator, 2 : Operator
	CString		strPassword;	// Password
	CString		strUserId;		// User ID (differ strMesUserId)
//	CString		strMesDsn;		// MES DSN		MES_DEV
//	CString		strMesUserId;	// MES USERID	DFT
//	CString		strMesPassWord;	// MES PASSWORD	DFT
	CString		strGmesIP;		// GMES Host IP Add
	UINT		nGmesPort;		// GMES Host Port
	BOOL		bGMES_Connection;		
//2013.08.22 추가
// GMES관련 Host에서 D/L되는 정보
	CString		sTransactionID;
	CString		sWorkOrderID;
	CString		sOrganizationID;
	CString		sLineID;
	CString		sEquipmentID;
	CString		sOperationID;
	CString		sModelID;
	CString		sModelName_GMES;
	CString		sModelSuffix;
	int			nDftSeq;
//----------

	BOOL		bAutoUploadMes;	// TRUE : Auto Upload Mes Data, FALSE : Manual Upload Mes Data
	BOOL		bUploadMes;		// TRUE : Upload Mes Data, FALSE : Don't Upload Mes Data
	BOOL		bAutoGMES_ChangeModel;


	UINT		nADCAdjType;	// 0 : RS232C, 1 : I2C

	int			nAudioCheckMethod;

	
//=========================
// Saved in Model.ini file 
//=========================

	//del 0831
//	UINT		nLvdsWidth;						// Uranous Grabber Width
//	UINT		nLvdsHeight;					// Uranous Grabber Height
//	UINT		nBitCount;						// Color Bit Count
	UINT		nAnalogWidth;					// Matrox Analog Grabber(Cronos+) Width
	UINT		nAnalogHeight;					// Matrox Analog Grabber(Cronos+) Height
//	UINT		nUranousMode;					// Uranous Grabber Mode
												// 0 : JEIDA_DUAL(0x81), 1 : JEIDA_SINGLE(0x01), 2 : NS_DUAL(0x80), 3 : NS_SINGLE(0x00)
	//+add 090630(Modification No1)
//	BOOL		bOddDE_Only;

	//+add 090420(Modification No1)
	UINT		nAnalogType;					// 0 : NTSC, 1 : PAL, 2 : SECAM 

//	CString		sToolOption1;
//	CString		sToolOption2;
//	CString		sToolOption3;
//	CString		sToolOption4;
//	CString		sToolOption5;
//	CString		sAreaOption1;

	CString		sBE_Version;
	CString		sFE_Version;
	CString		sMicom_Version;
	CString		sDSP_Version;
	CString		sSubDSP_Version;
	CString		sEQ_Version;
	CString		sMEQ_Version;
	CString		sTouch_Version;
	CString		sDemo_Version;
	CString		sDJSound_Version;
	CString		sWirelessTx_Version;
	CString		sWirelessRx_Version;
	CString		sBT_Version;
	CString		sHDMI_Version;
	CString		sChecksum;
	int			bVerChecked[15];


	UINT		nProcessType;

	UINT		nChipType;
//	UINT		nChipType1;
//	UINT		nChipType2;
	DWORD		wTVBaudRate;
//	DWORD		wTVBaudRate1;
//	DWORD		wTVBaudRate2;
	UINT		nTVParity;
//	UINT		nTVParity1;
//	UINT		nTVParity2;
	UINT		nGrabCheckArea;

	UINT		nSrcMargin[NO_SOURCE_TYPE];		// Input Source Spec Margin (Depend on InputSource Defined )

	float		fAllowedBandErrorPixelPercentage[5];

	int			nLevelMargin;
	int			nFreqMargin;				// In Spec Adjust Mode, Image test spec margin

	//+add kwmoon 080724
	// I2C Setting Value
	int			nI2cClkSpeed;
	int			nI2cPacketDelay;
	int			nI2cReadByteDelay;
	int			nI2cWriteByteDelay;

	UINT		nTVControlType;
	BOOL        bAutoSourceControl;
	BOOL        bAutoPatternControl;
	BOOL        bAutoDataDelete;

	//+add psh 091214
//	int			nImageRotation;
//=============================
// Saved in TestParam.ini file 
//=============================

	//+add kwmoon 080724
	UINT		nAudioSpec[NO_SOURCE_TYPE][5];	
	
	//+add psh 090401
	int			nDataSavePeriod;


	//+add psh 100310
	UINT		nHDMIResolution;
	UINT		nHDMIBitSpec;
	//+add psh 110226
	UINT		nRemoteCustomCode;
	UINT		nRemoteType;

	BOOL		bAvSwitchBoxBuzzerMute;
	//+add psh 110520
	UINT		nAvSwitchBoxInitVideoCh;
	CString		sModelSuffixName;
	int			nOrganization_ID;

	BOOL		bVersionCheckOnly;

	CString		sFixtureId;
	CString		sMacAdd;
	BOOL		bMacAddCheck;

	UINT		nModelInfo_Check;
	BOOL		bFixtureIdCheck;

	int		nJigUpType;							// 검사 종료후 JIG상승 옵션
	int		nJigStatus;							// Jig의 UP/Down 상태
	int		nRunStartType;						// 검사 시작 방법

	int		nSoundInDeviceID;
};


enum RunType
{
	CONT = 0, ERRORSTEP, STOP, STEP, AUTO
};

enum DisplayType
{
	NORMALGRID = 0, DETAILEDGRID
};

enum PrintType
{
	ALL = 0, ERRORVIEW, NONE
};

enum StepKey
{
	EMPTY, NEXT, PREV, JUMP
};

enum StartType
{
	MANUAL_START = 0, AUTO_START
};

enum RectIndex
{
	//+change kwmoon 071015 : [2in1]
//	MODEL = 0, REMOCON, TESTED, STATUS, FAILED, TIME, RATE, MODE
	MODEL = 0, CHASSIS, REF, REMOCON, TESTED, STATUS, FAILED, TIME, RATE, MODE, PROG_NO,
	AUDIO_L_FREQ, AUDIO_R_FREQ, AUDIO_L_LEVEL, AUDIO_R_LEVEL, SEQ, LED_STATE,

	//+add kwmoon 080321
	TOOL_OPTION1, TOOL_OPTION2, TOOL_OPTION3, TOOL_OPTION4, TOOL_OPTION5, AREA_OPTION1, AREA_OPTION2, 
	CPU_VERSION, MICOM_VERSION, USB_VERSION,

	//+add kwmoon 080618
	ALL_VIEW
};

//+change kwmoon 080522
//#define MES_DFT_MASTER_TABLE	"MES.TB_DFT_MASTER"
//#define MES_DFT_TABLE			"MES.TB_DFT"
#define MES_DFT_MASTER_TABLE	"MES.TB_WP_DFT_MASTER"
#define MES_DFT_DETAIL_TABLE	"MES.TB_WP_DFT_DETAIL"

#define MAMES_DFT_MASTER_TABLE	"MAMES.TB_WP_DFT_MASTER"
#define MAMES_DFT_DETAIL_TABLE	"MAMES.TB_WP_DFT_DETAIL"

//#define MDB_DFT_MASTER_TABLE	"DFTDB.TB_DFT_MASTER"
//#define MDB_DFT_TABLE			"DFTDB.TB_DFT"

#define DEV_PASSWORD		"5555"

#define AUTHORITY_DEV		0x0
#define AUTHORITY_ADMIN		0x1
#define AUTHORITY_OP		0x2


//INI
#define GENERAL_S			"GENERAL INFO"
#define SPEC_S				"SPEC"
#define GRAB_S				"GRABBER OPTION"
#define TOOL_AREA_S			"TOOL_AREA OPTION"
#define SRCMARGIN_S			"SOURCE MARGIN"

//+add kwmoon 080715
#define ALLOWED_ERROR_PIXEL_S	"ALLOWED ERROR PIXEL"

//+add kwmoon 080716
#define AUDIO_MARGIN_S		"AUDIO MARGIN"

//+add kwmoon 080724
#define AUDIO_SPEC_S		"AUDIO SPEC"

//+add kwmoon 080724
#define I2C_SETTING_S		"I2C SETTING"
//-
#define TV_CONTROL_S		"TV CONTROL"
#define VERSION_CHECK_S		"VERSION CHECK"

//+add PSH 100310
#define HDMI_FORMAT_S		"HDMI FORMAT"
//-

//+add PSH 120226
#define REMOCON_CUSTOM_S		"REMOCON CUSTOM"
//-

//Tab Page
#define P_GRAB			0
#define P_CROP			1
#define P_REVIEW		2
#define P_PARM			3
#define P_AVERAGE		4

#define I_ORG			0
#define I_CROP			1

//Image
#define I_NONE			-1
#define I_GRAB			0
#define I_REF			1
#define I_PROC			2
#define I_MASK			3
#define I_AVG			4

//Sequence Input Source
#define INPUT_RF		0
#define INPUT_AV		1
#define INPUT_AV1		2
#define INPUT_AV2		3
#define INPUT_AV3		4
#define INPUT_COMP		5
#define INPUT_COMP1		6
#define INPUT_COMP2		7
#define INPUT_RGB		8
#define INPUT_HDMI		9
#define INPUT_HDMI1		10
#define INPUT_HDMI2		11
#define INPUT_HDMI3		12
#define INPUT_HDMI4		13


#define AUDIO_SOURCE_TYPE_SPEAKER		1
#define AUDIO_SOURCE_TYPE_AV1			2
#define AUDIO_SOURCE_TYPE_AV2			3
#define AUDIO_SOURCE_VAR_OUT			4
#define AUDIO_SOURCE_TYPE_HEAD_PHONE	5
#define AUDIO_SOURCE_TYPE_WOOFER		6
#define AUDIO_SOURCE_TYPE_SPDIF			7	
#define AUDIO_SOURCE_TYPE_OPTIC			8	

#define IMG_SEL			0
#define IMG_AVR			1

#define NO_AV_SWITCHBOX_CMD					9
#define NO_AV_SWITCHBOX_CHANNEL				8
#define NO_AV_SWITCHBOX_ADC_VOLTAGE_TYPE	4
#define NO_AV_SWITCHBOX_DIO_PORT			16
#define NO_AV_SWITCHBOX_COMPONENT			2

//Remocon Custom Code
#define REMOCON_HT		0
#define REMOCON_MINI	1
#define REMOCON_BD		2
#define REMOCON_PN		3
#define REMOCON_LO		4
#define REMOCON_NA		5
#define REMOCON_SO		6

#define REMOCON_CM		7
#define REMOCON_GS		8
#define REMOCON_TB		9
#define REMOCON_SH		10
#define REMOCON_SI		11
#define REMOCON_JV		12

//Remocon Type
#define IR_CA 0
#define IR_IR 1

//Micom Chip Type
#define CHIP_MCS 0x01
#define CHIP_MTK 0x02
#define CHIP_BCM 0x03

//////////////////////////////////////////////////////////////////////////
//AVSwitchBox  Original Version
//AVSwitchBox Cmd
/*
#define VIDEO_CH_SELECT			0x01
#define VIDEO_SIGNAL_ON_OFF		0x02
#define AUDIO_CH_SELECT			0x03
#define AUDIO_SIGNAL_ON_OFF		0x04
#define SET_DIO_SIGNAL			0x05
#define SET_DC_OUT_SIGNAL		0x06
#define COMPONENT_CH_SELECT		0x07
#define CVBS_CH_SELECT			0x08
*/

//AVSwitchBox Value1
#define MAX_VIDEO_CH			9	//1~2
#define MAX_AUDIO_CH			9	//1~8
#define MAX_DIO_PORT			8	//1~8
#define MAX_DC_PORT				2	//1~2
#define MAX_COMPONENT_CH		3	//1~2 =>1~3 change PSH 100310
#define MAX_CVBS_CH				3	//1~2
#define MAX_REMOTE_TYPE			2
#define MAX_REMOTE_REPEAT		4	//0~3

/////////////////////////////////////////////////////////////////////////////
//AVSwitchBox 2In1Version
//AVSwitchBox Cmd
#define SET_DIGIT_OUT			0x01
#define SET_VIDEO_CH			0x02
#define SET_AUDIO_CH			0x03
#define SET_COMP_CH				0x04
#define SET_CVBS_CH				0x05
#define SET_ANALOG_VOLT			0x06
#define PULLUP_AVSWITCH			0x07
#define REMOTE_TYPE_SEL			0x08
#define CHK_AUDIO_LVL			0x09
#define CHK_AUDIO_FRQ			0x0a
//#define CHECK_STATUS			0x0b
#define SET_CUSTOM_CODE			0x0b
#define SEND_REMOTE_CODE		0x0c
#define SET_REMOTE_REPEAT_NO	0x0d

//+add kwmoon 080929
#define SET_RGB_SOUND			0x0e
//+add PSH 100310
#define SET_COMP_CVBS_CH		0x0f

//AVConvertor Cmd
#define READ_IR0_DATA			0x01
#define READ_IR1_DATA			0x02
#define BUFFER_CLR				0x03

//////////////////////////////////////////////////////////////////////////
#define DARK_BLACK_COLOR	RGB(128,128,128)
#define LIGHT_BLUE_COLOR	RGB(110,156,244)
#define DARK_BLUE_COLOR		RGB(0,0,255)
#define LIGHT_RED_COLOR		RGB(252,62,201)
#define LIGHT_YELLOW_COLOR	RGB(242,229,164)
#define DARK_YELLOW_COLOR	RGB(255,255,0)

#define COLOR_YELLOW_1		RGB(252,252,182)
#define COLOR_YELLOW_2		RGB(247,247,87)
#define COLOR_YELLOW_3		RGB(210,210,11)
#define COLOR_YELLOW_4		RGB(174,174,9)

#define COLOR_GREEN_1		RGB(196,255,136)
#define COLOR_GREEN_2		RGB(165,255,74)
#define COLOR_GREEN_3		RGB(111,221,0)
#define COLOR_GREEN_4		RGB(73,147,0)

#define COLOR_BLUE_1		RGB(210,255,233)
#define COLOR_BLUE_2		RGB(166,255,210)
#define COLOR_BLUE_3		RGB(108,255,182)
#define COLOR_BLUE_4		RGB(0,196,98)


//======================================================
const COLORREF COLOR_RED         = RGB( 255,   0,   0 );
const COLORREF COLOR_YELLOW      = RGB( 255, 255,   0 );
const COLORREF COLOR_GREEN       = RGB(   0, 255,   0 );
const COLORREF COLOR_CYAN        = RGB(   0, 255, 255 );
const COLORREF COLOR_BLUE        = RGB(   0,   0, 255 );
const COLORREF COLOR_MAGENTA     = RGB( 255,   0, 255 );
const COLORREF COLOR_LIGHTGRAY   = RGB( 192, 192, 192 );
const COLORREF COLOR_WHITE       = RGB( 255, 255, 255 );

const COLORREF COLOR_CLOUDBLUE   = RGB( 128, 184, 223 );

const COLORREF COLOR_DARKRED     = RGB( 128,   0,   0 );
const COLORREF COLOR_DARKYELLOW  = RGB( 128, 128,   0 );
const COLORREF COLOR_DARKGREEN   = RGB(   0, 128,   0 );
const COLORREF COLOR_DARKCYAN    = RGB(   0, 128, 128 );
const COLORREF COLOR_DARKBLUE    = RGB(   0,   0, 128 );
const COLORREF COLOR_DARKMAGENTA = RGB( 128,   0, 128 );
const COLORREF COLOR_DARKGRAY    = RGB( 128, 128, 128 );
const COLORREF COLOR_BLACK       = RGB(   1,   1,   1 );

//////////////////////////////////////////////////////////////////////////
//Grabber Resolution Define

#define		W_852_H_480				0x0
#define		W_1024_H_768			0x1
#define		W_1280_H_768			0x2
#define		W_1280_H_1024			0x3
#define		W_1366_H_768			0x4
#define		W_1440_H_900			0x5
#define		W_1680_H_1050			0x6
#define		W_1920_H_1080_60		0x7
#define		W_1920_H_1080_120		0x8
//+add kwmoon 080922
#define		W_1364_H_768			0x9

//Mode Register Setting Define
#define		JEIDA_DUAL			0x81
#define		JEIDA_SINGLE		0x01
#define		NS_DUAL				0x80
#define		NS_SINGLE			0x00

//Mode Register Setting Define
#define		JEIDA_DUAL6			0x84
#define		JEIDA_DUAL8			0x85
#define		JEIDA_DUAL10		0x86
#define		JEIDA_DUAL12		0x87
#define		JEIDA_SINGLE6		0x04
#define		JEIDA_SINGLE8		0x05
#define		JEIDA_SINGLE10		0x06
#define		JEIDA_SINGLE12		0x07
#define		NS_DUAL6			0x80
#define		NS_DUAL8			0x81
#define		NS_DUAL10			0x82
#define		NS_DUAL12			0x83
#define		NS_SINGLE6			0x00
#define		NS_SINGLE8			0x01
#define		NS_SINGLE10			0x02
#define		NS_SINGLE12			0x03
//-

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifndef		SAFE_DELETE
	#define SAFE_DELETE( p )	{ if( p ){ delete p; p = NULL; } }
#endif

#ifndef		SAFE_FREE
	#define SAFE_FREE( p )		{ if( p ){ free( (LPVOID)p ); p = NULL; } }
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// PLC
#define SWOUT1		0x310
#define SWOUT2		SWOUT1 + 1

#define KEYSTATUS	0x310
#define OPENALL		0x00

#define RUNSIGNAL		0x001
#define STOPSIGNAL		0x004
#define YESSIGNAL		0x040
#define NOSIGNAL		0x080
#define OKSIGNAL		0x200

//+ 2007.11.2 Add BY USY
#define START_BTN1	0x200
#define STOP_BTN1	0x10
#define RETRY_BTN1	0x20
#define OK_BTN1		0x40
#define NG_BTN1		0x80

#define START_BTN2	0x100
#define STOP_BTN2	0x01
#define RETRY_BTN2	0x02
#define OK_BTN2		0x04
#define NG_BTN2		0x08
//-

//+ 2007.11.4 Add BY USY
#define MSG_START_BTN1	0x0
#define MSG_STOP_BTN1	0x1
#define MSG_RETRY_BTN1	0x2
#define MSG_OK_BTN1		0x3
#define MSG_NG_BTN1		0x4

#define MSG_START_BTN2	0x5
#define MSG_STOP_BTN2	0x6
#define MSG_RETRY_BTN2	0x7
#define MSG_OK_BTN2		0x8
#define MSG_NG_BTN2		0x9

#define NO_USERCONTROL	0x0
#define KEYINPUTBOX_USE 0x1
#define JOYSTICK_USE	0x2

#define MAX_COLOR_LEVEL	100

//+add kwmoon 071004
#define DIFF_PERCENT_100	100

// Pattern Generator
#define PG_TIMING_HDMI	374

#define VK_F5		0x74
#define VK_F6		0x75
#define VK_F8		0x77
#define VK_F9		0x78

//+change kwmoon 070318
//#define MAX_NO_GRAB_RETRY	10
#define MAX_NO_GRAB_RETRY	1
#define MAX_NO_ADC_RETRY	2

#define MAX_NO_MEASURE_AUDIO_RETRY	3
#define MAX_GRAB_DELAY		1000


#define GRAB_WITH_RETRY		0
#define GRAB_WITH_NO_RETRY	1
#define GRAB_CHECK			2

#define MIN_AUDIO_FREQ		10.0	// 10Hz
#define MIN_AUDIO_LEVEL		10.0	// 10mV
#define MAX_AUDIO_FREQ		2000.0	// 2000Hz
#define MAX_AUDIO_LEVEL		10000.0	// 10000mV

#define NORMAL_IMAGE_TEST	0
#define GRAB_CHECK_TEST		1
#define OSD_IMAGE_TEST		2

#define NO_DIFFERENT_POINT_BLOCK	100
#define R_DATA	2
#define G_DATA	1
#define B_DATA	0
#define MAX_NO_COLOR_BITS	12

// Type of system message
#define STOP_TEST_PROCESSING_MSG   0
#define STOP_GRAB_PROCESSING_MSG   1
#define DATA_DELETE_PROCESSING_MSG 2
#define MES_DATA_AUTOUPLOAD_MSG    3

//+ 2007.9.20 Add BY USY
#define INTERNAL_REMOTE_CTRL				0
#define EXTERNAL_REMOTE_CTRL				1
//-

#define STEP_START			0
#define STEP_STOP			1
#define STEP_RETRY			2
#define STEP_OK				3
#define STEP_NG				4

//+ 2007.11.5 Add BY USY
#define WM_JOYSTICK_MSG		0x8000
//-

//+2007.12.25 Add BY USY
#define MAX_REMOTE_WAIT_DELAY		800
#define MAX_AVSWITCH_WAIT_DELAY		800
#define MAX_CHK_AUDIO_WAIT_DELAY	800


//+add 090306
#define FREQ_LEVEL_CHECK	0
#define FREQUENCY_CHECK		1
#define LEVEL_CHECK			2



//	Global Variable
extern CTypedPtrList<CPtrList, CStep*>	StepList;
extern CTypedPtrList<CPtrList, CStep*>  FullSeq;
extern CEnvironmentData*				CurrentSet;
extern CStep*							pCurStep;
extern CFunction*						pCurFunc;
extern POSITION							PosStep;
extern POSITION							PosFunc;


extern clock_t							Start;
extern clock_t							StepStart;
extern CString							sModel, sSeq, sTested, sStatus, sFailed, sTime, sRate, sMode;
extern int								CurrentDeviceNo;
extern int								g_nStepSize;
extern CIniAccess						m_Ini;


extern int				g_nProgressPos;
extern int				g_nNoTested;
extern int				g_nFailed;

//+add kwmoon 081004
extern int				g_nUserStop;

//+add PSH 081218
extern int				g_nRetryStop;
extern int				g_nUserRetry;
extern int				g_nNormalFailed;

extern double			g_dRate;	
//extern int				g_nNoFailedStep;
extern int				g_nStepSize;
extern int		gJigStatus;							// Jig의 UP/Down 상태
extern int g_nRunningProcessNo;
extern CString			g_sData_MD5;

// Global Function
int GetSysStatus();
void StepInforInit();
void StepInfoDisplay();

//int StepRun();

void ResetStep();
void AddStringToStatus(CString s);
//+add kwmoon 071206
void AddStringToRemoteComLog(CString s);

//+del kwmoon 071206
//void AddDoubleToStatus(double d);

//+change kwmoon 071206
//void InitComLogGrid();

//+del kwmoon 071206
//void AddStringToStatusGrid(CString s);
//void AddDoubleToStatusGrid(double d);


BOOL FindResult();
int FindResult2();
void SwControl_Inite();
void SwControl_DTVIN(int sIn, int sOut);
void ResultPrint();
void OkMessage(CString MsgString);

void CreateFullPath(CString sFullPath);

BOOL CreateModelIniFile(CString sModelIni);
BOOL SaveModelIniFile(CString sIniPath);
BOOL OpenModelIniFile(CString sIniPath);

BOOL CreateUserIniFile(CString sIniPath);
BOOL OpenUserIniFile(CString sIniPath);
BOOL SaveUserIniFile(CString sIniPath);

BOOL CreateLogFile(CString sLogPath);
BOOL OpenLogFile(CString sLogPath);
BOOL SaveLogFile(CString sLogPath);

//+change kwmoon 080904
BOOL CreateTestParamIniFile(CString sTestParamFolder);
BOOL SaveTestParamIniFile(CString sTestParamFolder);
BOOL OpenTestParamIniFile(CString sTestParamFolder);


//+del kwmoon 080910
//BOOL UpdateModelIniFile(CString sIniPath, CString sRefPath, CPoint pStart, CPoint pSize);
//BOOL FindInfoFromModelIni(CString sModelIniPath, CString sRefPath, CPoint &pStart, CPoint &pSize );

BOOL GetBmpSize(CString sBmpPath, CPoint &pSize);

//+del kwmoon 080904
//void GetCurrentValue(CString sSeqPath);

BOOL FileExists(const CString csFilePath );
BOOL FileExists(const char * cpcFilePath);

CString SplitDefectData(UINT nNo, CString sRstData);

/*
//void InitSummaryGrid();
void InitMainGrid(int nGridType);
void InsertStepData2Grid(int nGridType);
void InsertResultData2Grid(int nGridType, int nStepNo);
void InsertResult2NormalGrid(int nStepNo);
void InsertResult2DetailedGrid(int nStepNo);
void InsertMsg2DetailedGrid(int nStepNo, CString sMsg);

void InsertTotalData2Grid(int nGridType, BOOL bResult, CString sElapsedTime);

//+ 2007.9.21 Add BY USY
void DeleteResultDataFromGrid(int nGridType, int nStepNo);
void DeleteResultDataFromNormalGrid(int nStepNo);
void DeleteResultDataFromDetailedGrid(int nStepNo);
//-

BOOL GetResultFromDetailedGrid(int nStepNo,CString &sMsg);

void GetCheckStep(int nGridType);
*/
void ShowSubStepMessage(CString sMsg, CString sDefaultMsg);
void SaveTestResult(CString sResultPath);

//+del kwmoon 080814
//+ 2007.9.21 Add BY USY
//void ChangeTestResult(CString sResultPath, int nStepNo, int nType);
//-
//void SaveResult2File(CString sRstPath, BOOL bSave2Txt, BOOL bSave2Html, BOOL bSave2Csv);

//+add kwmoon 080123
void SaveRemoteLog2File(CString sRstPath, BOOL bSave2Txt, BOOL bSave2Html, BOOL bSave2Csv);


//+add kwmoon 070823
void SaveLineTestData(CIniAccess* pIni);

//+add kwmoon 080229
//int GetNoCheckedStep(int nDisplayType);

BOOL OpenRemoteFile(CString sRemotePath);

//+add kwmoon 071214
BOOL OpenPatternFile(CString sPatternPath);
BOOL FindPatternInfo(int nPatternNo, int* pFilterSize, int* pNoFiltering, int* pRepaint,CString* szRoiShape);

//+del kwmoon 080411
// void SetDirPath(CString sModelFolder);

UINT hex2dec(const char ccHex);
UINT hexCstr2decNum(const CString csHex);
UINT hexStr2DecNum( char* cpcHex);
//+ 2007.10.19 Add BY USY
BOOL InitPatternGen(CString sComPort, DWORD wBaudRate);
//+ 2007.12.4 Del BY USY
//BOOL InitRemoteCtrl(CString sComPort, DWORD wBaudRate);
//-
BOOL InitTVCommPort(CString sComPort, DWORD wBaudRate);
BOOL InitAvSwitchBox(CString sComPort, DWORD wBaudRate);
//+ 2007.11.30 Add BY USY
//BOOL InitScanner(CString sComPort, DWORD wBaudRate);
//-
//+add kwmoon 071120
//void InitSysInfoGrid();
//-

//+add kwmoon 080625
BOOL InitIrChkCommPort(CString sComPort, DWORD wBaudRate);


//+add kwmoon 080710
BOOL CreateTimeDataFile(CString sData, CString sTime);
void CloseTimeDataFile();
void AddTimeData(CString szTimeData);

//+add kwmoon 080716
void SaveDetailTestResult();
//add 090423
BOOL SaveAdcAdjResult();

//+add kwmoon 080918
void SaveResultSummary();

//+add kwmoon 081024
BOOL OpenModelListFile(CString sModelListPath,int& nNoModel,CStringArray &Divisions);
BOOL SaveModelListFile();
//+add kwmoon 081028
BOOL CreateModelListFile(CString sModelListPath);
BOOL StepInit_AvSwitchBox();
BOOL StepInit_SourceAutoControl();
// add PSh 100310
int GetHDMI_Model();
//+add PSH 0903038
BOOL StepInit_PatternAutoControl();
BOOL GetFolder(CString* strSelectedFolder,
				   const char* lpszTitle,
				   const HWND hwndOwner, 
				   const char* strRootFolder, 
				   const char* strStartFolder);

BOOL MesFailSave(CString sPCBID, CString sError);
BOOL MesData_AutoUpLoad_TimeCheck();
BOOL TestCountSave(CString sExePath);
BOOL TestCountRead(CString sExePath);
void StringToken(CString Source, CString Deliminator, CStringArray& AddIt, BOOL bAddEmpty);

BOOL InitHDMIGen(CString sComPort, DWORD wBaudRate);
BOOL InitThermometer(CString sComPort, DWORD wBaudRate);

void ResultData_Delete(LONG nPeriodDay); 
//090424
BOOL MesData_AutoUpLoad();
int  GetUpLoadData_Count();
BOOL FindRemoconCode(CString strName, CString& strCustom, CString& strCode, int& nByte);
BOOL GmesLog_Save(CString sExePath, CString sComData, BOOL bFlag);
BOOL Get_ToolOption(CString sOption);
BOOL Gmes_Connection();
BOOL Send_ResultMpKey( BOOL bResult); 

#endif // !defined(AFX_GLOBAL_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_)
