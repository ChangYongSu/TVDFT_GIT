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
#include "DPGenerator.h"
#include <vector>
#include <algorithm>

using namespace std;

#define PERI_REMO_ADDR	0x04

class CStep;
class CFunction;

extern CString gStrTmpLog;
extern int g_nResetGrabMode;
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
	CString sDesc;		//Remote Control Button Description
	BOOL	bUse;		//Remote Control Button Use or Not Use
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
	CString m_szModelSuffix;
	CString m_szSeqFilePath;
	CString m_szModelInfoFilePath;
	CString m_szRefImgFolder;
	CString m_szDivision;
	CString m_szSeqFilePath2;
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

//-------------------
// V-F Measure Structure
//-------------------
class CVFTestInfo:public CObject
{
public:
	CVFTestInfo()
	{
		bRun		= FALSE;		
		nChNo		= 0;		
		strTestName = _T("");
		nCheckType	= 0;	
		dTarget		= 0.0;	
		dLowerLimit = 0.0;
		dUpperLimit = 0.0;
		strUnit		= _T("");	
		nTime		= 0;		
		dMaesure_Min = 0.0;
		dMaesure_Max = 0.0;
		bResult = FALSE;
	}
	~CVFTestInfo()
	{
	}

	// Variable
	BOOL		bRun;		// false : Skip, true : Run
	UINT		nChNo;		
	CString		strTestName;
	UINT		nCheckType;	// OK,NG/A,NG/V, NG/AV
	double		dTarget;	// Target Value
	double		dLowerLimit;// Lower Limit
	double		dUpperLimit;// Upper Limit
	UINT		nMarginType;// Upper Limit
	CString		strUnit;	// Unit : Level , mV, %
	UINT		nTime;		// Elapsed Time

	double		dMaesure_Min;
	double		dMaesure_Max;
	BOOL		bResult;

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
	int			nRunStartType;					// 
	int			nPrintType;						// Current Print Out type
	int			nStepKey;						// StepKey
	int			nDisplayType;					// Current Display type => 0 : Main Grid -> Normal Grid
	int			nJigStatus;						// JigAC UP/Down ≫oAA
	int			nJigUpType;						// °E≫c A¾·aEA JIG≫o½A ¿E¼C
												//						   1 : Main Grid -> Detailed Grid
	//+del kwmoon 080715
//	int			nNoAllowedErrorPixel;				
//	int			nTotalNoTest;
//	int			nTotalNoNg;
	
	//+add kwmoon 081004
//	int			nTotalNoUserStop;	

	int			nNoFrameForRefImageCreation;

	int			nPixelInMiddleRange;
	int			nNonBlackPixel;

	//+add 090218(Modification No1)
	int			nNoUsedColors;

	//+del kwmoon 080715
//	int			nNoAllowedErrorPixelForOsdTest;				
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

	//+del kwmoon 080716
//	BOOL		bSaveDistributionTable;

	//+add kwmoon 080710
	BOOL		bSaveProcessingTimeData;

	//+add kwmoon 080716
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
	CString		sSeqPath_EM;						// Sequence File 
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

	CString		sVFTestIni;						// Model Ini Path
	CString		sEpiCfgIni;						// Model Ini Path
	
	//+add kwmoon 080904
	CString		sServerFolder;
	//+add PSH 081029
	CString		sRefRootFolder;
	CString		sChassisName;
	CString		sModelName;
	//+add PSH 081211
	CString		sModelInfoFolder;
#ifdef SM_MODIFY_CODE__
	//CYS 2018.11.27
	CString		sModelVFConfigFolder;
#endif
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
	CString		sAvSwitchBoxComPort;
	CString		sScannerComPort;
	CString		sStartBoxComPort;
	CString		sI2cAdcComPort;
	CString		sLNBComPort;
	CString		sJigComPort;
	CString		sVfmComPort;
	CString		sDpgComPort;

	DWORD		wPatternBaudRate;
	DWORD		wHDMIBaudRate;
	DWORD		wAvSwitchBoxBaudRate;
	DWORD		wScannerBaudRate;
	DWORD		wStartBoxBaudRate;
	DWORD		wI2cAdcBaudRate;
	DWORD		wLNBBaudRate;
	DWORD		wJigBaudRate;
	DWORD		wVfmBaudRate;
	DWORD		wDpgBaudRate;

	CString		sHDMIRoiShape;

	BOOL		bUsePatternGen;
	BOOL		bUseTVCommPort;
	BOOL		bUseAVSwitchBox;
	BOOL		bUseScanner;
	BOOL		bUseStartBox;
	BOOL		bUseI2cAdc;
	BOOL		bUseHDMIGen;
	BOOL		bUseLNB;
	BOOL		bUseJig;
	BOOL		bUseVfm;
	BOOL		bUseDpg;

// add 20100622
	BOOL		bUseAVC;
	DWORD		wAvcBaudRate;
	CString		sAvcComPort;
//-

	UINT		nAdcType;

	CTypedPtrList<CPtrList, CRemoteCode*>	RemoteList;
	CTypedPtrList<CPtrList, CPatternInfo*>	PatternList;

	//+add kwmoon 081024
	CTypedPtrList<CPtrList, CModelData*>	ModelList;
	//+add kwmoon 081024
	CTypedPtrList<CPtrList, CResultData*>	MES_DataList;
	CTypedPtrList<CPtrList, CStepResultData*>	MES_StepDataList;

	CTypedPtrList<CPtrList, CVFTestInfo*>	VF_TestInfo;


	UINT		nAudioSource;					// Audio Input Source 0: LineIn, 1: DigitalIn
	UINT		nNoRetry;						// Internal Retry Number
	BOOL		bTVCommPortEcho;				// TVCommPort Echo


	//------------------
	// MES Data
	//------------------
	CString		sLineCode;
	UINT		nLineID_GLZ;
	UINT		nLineID_GPZ;
	UINT		nLineID_GMZ;
	UINT		nLineNo;		
	UINT		nSystemNo;		
	UINT		nAuthority;		// Authority 0 : Developer, 1 : Administrator, 2 : Operator
	CString		strPassword;	// Password
	CString		strUserId;		// User ID (differ strMesUserId)
//	CString		strMesDsn;		// MES DSN		MES_DEV
//	CString		strMesUserId;	// MES USERID	DFT
//	CString		strMesPassWord;	// MES PASSWORD	DFT
	CString		strGmesIP;		// GMES Host IP Add
	UINT		nGmesPort;		// GMES Host Port

	CString		strPLC_IP;		// GMES Host IP Add
	UINT		nPLC_Port;		// GMES Host Port
	UINT		nDFT_No;		// GMES Host Port

	CString		strMonitorServer_IP;		// GMES Host IP Add
	UINT		nMonitorServer_Port;		// GMES Host Port


//2013.08.22 추가
// GMES관련 Host에서 D/L되는 정부
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
	int			nDftNgCount;
	int			nDftOkCount;
//----------

	BOOL		bUseGmes;
	BOOL		bUsePLCRobot;
	BOOL		bUseServerCtrlConnect;
	BOOL		bAutoUploadMes;	// TRUE : Auto Upload Mes Data, FALSE : Manual Upload Mes Data
	BOOL		bUploadMes;		// TRUE : Upload Mes Data, FALSE : Don't Upload Mes Data

	BOOL		bGMES_Connection;
	BOOL		bPLC_Connection;
	BOOL		bMonitorServer_Connection;
	//+add PSH 080318
	UINT		nADCAdjType;	// 0 : RS232C, 1 : I2C

	int			nAudioCheckMethod;

	// 090120 External DFT
//	int			nLvdsGrabberType;
	
//=========================
// Saved in Model.ini file 
//=========================

	UINT		nLvdsWidth;						// Uranous Grabber Width
	UINT		nLvdsHeight;					// Uranous Grabber Height
	UINT		nBitCount;						// Color Bit Count
	UINT		nAnalogWidth;					// Matrox Analog Grabber(Cronos+) Width
	UINT		nAnalogHeight;					// Matrox Analog Grabber(Cronos+) Height
	BOOL		bRobotSkipFFC;
	UINT		nUranousMode;					// Uranous Grabber Mode// 0 : JEIDA_DUAL(0x81), 1 : JEIDA_SINGLE(0x01), 2 : NS_DUAL(0x80), 3 : NS_SINGLE(0x00)
	UINT		nLVDS_Mode;						// 0 : JEIDA, 1 : VESA, 2 : OLED(JEIDA)
	//+add 090630(Modification No1)
	BOOL		bOddDE_Only;

	//+add 090420(Modification No1)
	UINT		nAnalogType;					// 0 : NTSC, 1 : PAL, 2 : SECAM 
	int			nGrabSource;
	CString		sToolOption1;
	CString		sToolOption2;
	CString		sToolOption3;
	CString		sToolOption4;
	CString		sToolOption5;
	CString		sToolOption6;
	CString		sToolOption7;
	CString		sToolOption8;
	CString		sToolCRC;

	CString		sAreaOption1;
	CString		sCountryGroup;
	CString		sCountry;

	CString		sDipStatus;
	BOOL		bToolOptionKeyBlank;

	//add 20110610
	CString		sCommercialOption1;

	CString		sCPUVersionRead;
	CString		sCPUVersion;
	CString		sMicomVersion;
	CString		sUSBVersion;
	CString		sMNTVersion;

	UINT		nToolOptionCheck_Delay;					 

	CString		sEDID_HDMI1;
	CString		sEDID_HDMI2;
	CString		sEDID_HDMI3;
	CString		sEDID_HDMI4;
	CString		sEDID_HDMI5;
	CString		sEDID_DP;
	CString		sEDID_DP2;
	CString		sEDID_DVI;
	CString		sEDID_PC;
	CString		sEDID_RGB;

	CString		sCI;
	CString		sCNTV;
	CString		sDTCP_RX;
	CString		sDVBS;
	CString		sDVBS2;
	CString		sESN;
	CString		sHDCP_RX;
	CString		sHDCP_RX_14;
	CString		sHDCP_TX;
	CString		sJAPAN_LNB;
	CString		sMAC_ADDRESS;
	CString		sMARINKEY;
	CString		sMODEL_NAME;
	CString		sSERIAL_NO;
	CString		sWASU;
	CString		sWIDEVINE;
	CString		sADC;
	CString		sBUILT_WIFI;
	CString		sCA;
	CString		sASTC30_Key;
	CString		sMfi_Key;
	CString		sCI_CEP_Key;

	BOOL        bDrmKey;
	int         nDrmKeyVal;

	DWORD		wTVBaudRate;
	//+add PSH 081029
	UINT		nGrabCheckArea;

	//+add PSH 081029
	UINT		nCountryGroup ;

//	UINT		nSrcMargin[NO_SOURCE_TYPE];		// Input Source Spec Margin (Depend on InputSource Defined )

	//+add kwmoon 080715
	double		fAllowedBandErrorPixelPercentage[5];
	int			nFullHd120Hz;

	//+add kwmoon 080716
	int			nLevelMargin;
	int			nFreqMargin;				// In Spec Adjust Mode, Image test spec margin

	int			nAudioForce_HP_R;
	int			nAudioForce_HP_L;
	int			nAudioForce_Analog_L;
	int			nAudioForce_Analog_R;
	int			nAudioForce_HDMI_L;
	int			nAudioForce_HDMI_R;
	int			nAudioForce_RGB_L;
	int			nAudioForce_RGB_R;
	int			nAudioForce_SCART_L;
	int			nAudioForce_SCART_R;

	int			nARC_Force_Port_Number;
	int			nARC_Force_OnOffChannel;
	
	//+add kwmoon 080724
	// I2C Setting Value
	int			nI2cClkSpeed;
	int			nI2cPacketDelay;
	int			nI2cReadByteDelay;
	int			nI2cWriteByteDelay;

	UINT		nTVControlType;
	BOOL        bAutoSourceControl;
	BOOL        bAutoPatternControl;
	BOOL        nAutoAVSwitchBoxControl;
	//+add 090402
	BOOL        bAutoDataDelete;

	//+add psh 091214
	int			nImageRotation;

	int nRedBlueSwap;
	int nDeHsyncEnable;
	int nHyncEdgeEnable;

//=============================
// Saved in TestParam.ini file 
//=============================

//	UINT		nAudioSpec[NO_SOURCE_TYPE][5];	
	
	//+add psh 090401
	int			nDataSavePeriod;


	//+add psh 100310
	UINT		nHDMIGenType;
	UINT		nHDMIResolution;
	UINT		nHDMIBitSpec;
	UINT		nCompResolution;
	UINT		nHDMI1_Model;
	UINT		nHDMI2_Model;
	UINT		nHDMI3_Model;
	UINT		nHDMI4_Model;
	//+add psh 101029
	int			n3DGrabbingMode;
	int			nToolOptionSetMethod;
	CString		sModelSuffixName;
	int			nOrganization_ID;
	//HDCP-Key DOwnload
	UINT		nHDCP_Start_ADH;
	UINT		nHDCP_Start_ADL;
	UINT		nHDCP_Data_Size;
	UINT		nHDCP_Key_Class;
	CString		sHDCP_Key_Class;

	CString		sHdcpKeyListPath;					
	CString		sHdcpKeyFileName;					
	BOOL		bHdmiOutControl;
	UINT		nHDMI1_Port;
	UINT		nHDMI2_Port;
	UINT		nHDMI3_Port;
	UINT		nHDMI4_Port;
	BOOL		bHdmiCecControl;
	BOOL		bHdmiHdcpControl;
	BOOL		bHdmiEdidControl;
	int			nAvrFW_Ver;
//	BOOL		bS6F5DL;
	UINT		nUHD_Type;
	UINT		nUHD_Grab_BitShift;
	UINT		nUHD_Grab_Mode;
	UINT		nUHD_Grab_Delay;
	UINT		nUHD_Y20_SW_Mode;

	UINT		nHDMI_Grab_Resolution;
	UINT		nHdmiWidth;					// Hdmi Grab Width
	UINT		nHdmiHeight;				// Hdmi Grab Height
	UINT		nHDMI_Grab_BitShift;
	UINT		nHDMI_Grab_Input;
	int			nDmmMeasureCh_S;
	int			nDmmMeasureCh_E;
	BOOL        bNgCountCheck;
	BOOL        bHdmiGrab_MaskOsd;

	BOOL        bCheckGrabACPower;
	BOOL        bCheckIFPackReset;
	BOOL        bCheckGenderReset;
	BOOL        bCheckRotateReset;
	BOOL        bCheckUsePcbSensor;
	BOOL		bCheckImageFullReset;
	BOOL		bCheckOledReset;
	BOOL        bCheckBatteryCheck;


	CString		sJACK_AV1;
	CString		sJACK_AV2;
	CString		sJACK_SCART;
	CString		sJACK_COMP1;
	CString		sJACK_COMP2;
	CString		sJACK_USB1;
	CString		sJACK_USB2;
	CString		sJACK_USB3;
	CString		sJACK_USB4;
	CString		sJACK_HEAD_PHONE;
	CString		sJACK_HDMI1;
	CString		sJACK_HDMI2;
	CString		sJACK_HDMI3;
	CString		sJACK_HDMI4;
	CString		sJACK_RGB;
	CString		sJACK_DVI;
	CString		sJACK_DP;
	CString		sJACK_SMHL;

//	UINT		nSystemType;		
	UINT        nDrmKeyResultVal;
	UINT        nInputCheckResultVal;
	int			nAcPowerOffType;
	int			nAuto_Grab_Reset;

	BOOL        bTestTimeCount;
	BOOL        bAutoRun_DFT2;
	BOOL		bPJT_GrabDisable;
	CString     sData_MD5;

	BOOL        bScanNotUse;
	CString		sBatVersion;
	BOOL		bEpiPAckReset;	
	BOOL		bGrabBaseReset;

	CString		sMNT_Edid_Source;
	int			bPCBID_Rewrite;
	CString		sPCBID_Scan;
	CString		sPCBID_Read;
	UINT		nDP_TimeSel;

};

enum GrabberType
{
	UHD_GRABBER = 0, FHD_GRABBER
};
enum SystemType
{
	MANUAL_SYS = 0, AUTO_SYS
};


enum RunType
{
	CONT = 0, ERRORSTEP, STOP, STEP, AUTO
};

enum StartType
{
	MANUAL_START = 0, AUTO_START
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

enum 
{
   INPUT_ERROR =0, IN_JIGUP, IN_JIGDN, IN_TEST_START, IN_TEST_STOP, IN_TEST_ENTER,
	   IN_EMG
};

enum RectIndex
{
	//+change kwmoon 071015 : [2in1]
//	MODEL = 0, REMOCON, TESTED, STATUS, FAILED, TIME, RATE, MODE
	MODEL = 0, CHASSIS, REF, REMOCON, TESTED, STATUS, FAILED, TIME, RATE, MODE, PROG_NO,
	AUDIO_L_FREQ, AUDIO_R_FREQ, AUDIO_L_LEVEL, AUDIO_R_LEVEL, SEQ,

	//+add kwmoon 080321
	TOOL_OPTION1, TOOL_OPTION2, TOOL_OPTION3, TOOL_OPTION4, TOOL_OPTION5, TOOL_OPTION6, TOOL_OPTION7, TOOL_OPTION8, TOOL_CRC,AREA_OPTION1,  
	COMMERCIAL_OPTION1, CPU_VERSION, MICOM_VERSION, USB_VERSION, COUNTRAY_OPTION, COUNTRY_GROUP, COUNTRY,  MNT_VERSION, LED_STATE,

	//+add kwmoon 080618
	ALL_VIEW
};
enum Input_JackIndex
{JACK_AV1 = 0,JACK_AV2, JACK_SCART, JACK_COMP1, JACK_COMP2, JACK_USB1, JACK_USB2, JACK_USB3, JACK_USB4, JACK_HEAD_PHONE, 
JACK_HDMI1, JACK_HDMI2, JACK_HDMI3, JACK_HDMI4, JACK_RGB, JACK_DVI, JACK_DP, JACK_SMHL 
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

#define ALLOWED_ERROR_PIXEL_S	"ALLOWED ERROR PIXEL"
#define AUDIO_MARGIN_S		"AUDIO MARGIN"
#define AUDIO_SPEC_S		"AUDIO SPEC"
#define I2C_SETTING_S		"I2C SETTING"
#define TV_CONTROL_S		"TV CONTROL"
#define VERSION_CHECK_S		"VERSION CHECK"
#define SPEC_CHECK_S		"SPEC CHECK"
#define INPUT_CHECK_S		"INPUT CHECK"
#define FORCE_CHECK_S		"FORCE CHECK"
#define HDMI_FORMAT_S		"HDMI FORMAT"
#define COMP_FORMAT_S		"COMP FORMAT"
#define HDCP_KEYDN_S		"DHCP KEY DOWNLOAD"
#define VIDEO_MARGIN_S		"VIDEO MARGIN"

#define DFT_SYSTEM_INFO_S		"DFT SYSTEM INFO"

//Tab Page
#define P_GRAB			0
#define P_CROP			1
#define P_REVIEW		2
#define P_PARM			3
#define P_AVERAGE		4
#define P_AUTO_ROBOT	5

#define I_ORG			0
#define I_CROP			1

//Image
#define I_NONE			-1
#define I_GRAB			0
#define I_REF			1
#define I_PROC			2
#define I_MASK			3
#define I_AVG			4


#define _PLC_ADDRESS_MODEL_NAME						0 
#define _PLC_ADDRESS_LOAD_COMMAND					50 
#define _PLC_ADDRESS_UNLOAD_COMMAND					51 
#define _PLC_ADDRESS_TEST_PASS						52 
#define _PLC_ADDRESS_TEST_NG						53 
#define _PLC_ADDRESS_FFC_RETRY_COMMAND				54 
#define _PLC_ADDRESS_LOAD_FINISH					55 
#define _PLC_ADDRESS_UNLOAD_FINISH					56 
#define _PLC_ADDRESS_FFC_RETRY_FINISH				57 
#define _PLC_ADDRESS_PLC_STATUS						58 
#define _PLC_ADDRESS_NG_SET_RETRY					59 
#define _PLC_ADDRESS_PLC_PCB_NG_RETRY_STATUS		60 
#define _PLC_ADDRESS_ROBOT_MOVING_STATUS			61
#define _PLC_ADDRESS_PCB_ONLOAD_SENSOR				62 
#define _PLC_ADDRESS_FAST_COMMAND					63 
#define _PLC_ADDRESS_JIG_UP_STATUS					64 
#define _PLC_ADDRESS_JIG_DOWN_STATUS				65 
#define _PLC_ADDRESS_NG_3_TIMES_ERROR				66 



#define _PLC_ADDRESS_PLC_PCB_INSIDE_STATUS			10 
#define _PLC_ADDRESS_PLC_PCB_DUMMY_STATUS			11 
#define _PLC_ADDRESS_PLC_PCB_OK_STATUS				12
#define _PLC_ADDRESS_PLC_PCB_NG_STATUS				13 



#define _SERVER_GMES_ID					0 
#define _SERVER_PLC_ID					1 
#define _SERVER_MONITOR_CONTROL_ID		2


#define _AUTO_ROBOT_CTRL_IDLE			0
#define _AUTO_ROBOT_CTRL_INIT			1
#define _AUTO_ROBOT_CTRL_JIGUP			2
#define _AUTO_ROBOT_CTRL_LOAD			3
#define _AUTO_ROBOT_CTRL_JIG_DOWN		4
#define _AUTO_ROBOT_CTRL_TEST_RUN		5
#define _AUTO_ROBOT_CTRL_JIGFINISH_UP	6
#define _AUTO_ROBOT_CTRL_UNLOAD			7
#define _AUTO_ROBOT_CTRL_TEST_OK		8
#define _AUTO_ROBOT_CTRL_TEST_NG		9
#define _AUTO_ROBOT_CTRL_PAUSE			10
#define _AUTO_ROBOT_CTRL_OFFLINE		11
#define _AUTO_ROBOT_CTRL_NOACK			12
#define _AUTO_ROBOT_CTRL_MANUAL			13
#define _AUTO_ROBOT_CTRL_ALARM			14
#define _AUTO_ROBOT_CTRL_PCB_IN_ERROR	15
#define _AUTO_ROBOT_CTRL_TIMEOUT		16
#define _AUTO_ROBOT_CTRL_FFC_RETRY		17
#define _AUTO_ROBOT_CTRL_UPDOWN_RETRY	18
#define _AUTO_SERVER_CTRL_PAUSE			19
#define _AUTO_ROBOT_CTRL_NG_3_STOP		98
#define _AUTO_ROBOT_CTRL_ERROR			99
#define _AUTO_ROBOT_CTRL_PLC_ON			100

#define _SERVER_COM_COMMAND_ID			1
#define _SERVER_COM_RUNMSG_ID			2
#define _SERVER_COM_ROBOTMSG_ID			3
#define _SERVER_COM_MMMMMM_ID			1

#define		_SERVER_CTRL_IDLE			0
#define		_SERVER_CTRL_START			1
#define		_SERVER_CTRL_PAUSE			2
#define		_SERVER_CTRL_STOP			3
#define		_SERVER_CTRL_GETTOOL		4
#define		_SERVER_CTRL_GETTOOL_CLOSE	5
#define		_SERVER_CTRL_GET_MODEL_LIST	6
#define		_SERVER_CTRL_CHANGE_MODEL	7
#define		_SERVER_CTRL_JIG_UP			8
#define		_SERVER_CTRL_JIG_DOWN		9
#define		_SERVER_CTRL_LOAD			10
#define		_SERVER_CTRL_TEST_PASS		11
#define		_SERVER_CTRL_TEST_NG		12
#define		_SERVER_CTRL_TEST_CLEAR		13
#define		_SERVER_CTRL_TEST_READ		14
#define		_SERVER_CTRL_PLC_DATA_READ	15
#define		_SERVER_CTRL_AC_ON			16
#define		_SERVER_CTRL_AC_OFF			17



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
#define INPUT_USB1		14
#define INPUT_USB2		15
#define INPUT_USB3		16
#define INPUT_USB4		17
#define INPUT_SCART		18

#define AUTO_COUNT_ID_HEADPHONE	0
#define AUTO_COUNT_ID_GREENEYE	1
#define AUTO_COUNT_ID_COMPONENT	2
#define AUTO_COUNT_ID_AV		3
#define AUTO_COUNT_ID_USB		4
#define AUTO_COUNT_ID_HDMI		5
#define AUTO_COUNT_ID_RGB		6
#define AUTO_COUNT_ID_SCART		7
#define AUTO_COUNT_ID_MAX		6 // 8 //200626


#define FORCED_CHECK_ID_HEADPHONE		0
#define FORCED_CHECK_ID_GREENEYE		1
#define FORCED_CHECK_ID_COMPONENT		2
#define FORCED_CHECK_ID_AV				3
#define FORCED_CHECK_ID_USB_1			4
#define FORCED_CHECK_ID_USB_2			5
#define FORCED_CHECK_ID_USB_3			6
#define FORCED_CHECK_ID_HDMI_1			7
#define FORCED_CHECK_ID_HDMI_1_ARC		8
#define FORCED_CHECK_ID_HDMI_2			9
#define FORCED_CHECK_ID_HDMI_2_ARC		10
#define FORCED_CHECK_ID_HDMI_3			11
#define FORCED_CHECK_ID_HDMI_3_ARC		12
#define FORCED_CHECK_ID_HDMI_4			13
#define FORCED_CHECK_ID_HDMI_4_ARC		14
#define FORCED_CHECK_ID_RGB				15
#define FORCED_CHECK_ID_SCART			16
#define FORCED_CHECK_ID_MAX				17

#define FORCE_ARC_HDMI_PORT_1			0
#define FORCE_ARC_HDMI_PORT_2			1
#define FORCE_ARC_HDMI_PORT_3			2
#define FORCE_ARC_HDMI_PORT_4			3
#define FORCE_ARC_HDMI_PORT_NONE		4

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
#define MAX_VIDEO_CH			2	//1~2
#define MAX_AUDIO_CH			8	//1~8
#define MAX_DIO_PORT			12	//1~8
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
#define CHECK_STATUS			0x0b
#define SEND_REMOTE_CODE		0x0c
#define SET_REMOTE_REPEAT_NO	0x0d

//+add kwmoon 080929
#define SET_RGB_SOUND			0x0e
//+add PSH 100310
#define SET_COMP_CVBS_CH		0x0f

//AVConvertor Cmd
#define SET_INPUR_SEL			0x01
#define READ_COM_DATA			0x02
#define READ_IR_DATA			0x03
#define BUFFER_CLR				0x04


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


#define		W_1024_H_768			0
#define		W_1280_H_768			1
#define		W_1280_H_1024			2
#define		W_1364_H_768			3
#define		W_1366_H_768			4
#define		W_1440_H_900			5
#define		W_1600_H_900			6
#define		W_1680_H_1050			7
#define		W_1920_H_1080_60		8
#define		W_1920_H_1080_120		9
#define		W_2560_H_1080			10
#define		W_2560_H_1440			11
#define		W_3440_H_1440			12
#define		W_3840_H_2160			13
#define		W_1920_H_300			14
#define		W_3840_H_600			15
#define		W_1920_H_540			16

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

#define START_BTN	0x10
#define STOP_BTN	0x20
#define RETRY_BTN	0x30
#define OK_BTN		0x40
#define NG_BTN		0x50


#define START_BTN1	0x10
#define STOP_BTN1	0x20
#define RETRY_BTN1	0x30
#define OK_BTN1		0x40
#define NG_BTN1		0x50


#define START_BTN2	0x60
#define STOP_BTN2	0x70
#define RETRY_BTN2	0x80
#define OK_BTN2		0x90
#define NG_BTN2		0xA0

#define START_BTN3	0xB0
#define STOP_BTN3	0xC0
#define RETRY_BTN3	0xD0
#define OK_BTN3		0xE0
#define NG_BTN3		0xF0


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

#define MSG_START_BTN3	0xA
#define MSG_STOP_BTN3	0xB
#define MSG_RETRY_BTN3	0xC
#define MSG_OK_BTN3		0xD
#define MSG_NG_BTN3		0xE


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
#define MAX_NO_GRAB_RETRY	10
//#define MAX_NO_GRAB_RETRY	1
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
#define STOP_TEST_PROCESSING_MSG	0
#define STOP_GRAB_PROCESSING_MSG	1
#define DATA_DELETE_PROCESSING_MSG	2
#define MES_DATA_AUTOUPLOAD_MSG		3
#define TOOL_OPTION_DOWNLOAD_MSG	4
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
#define MAX_REMOTE_WAIT_DELAY		500
#define MAX_AVSWITCH_WAIT_DELAY		500
#define MAX_CHK_AUDIO_WAIT_DELAY	500


//+add 090306
#define FREQ_LEVEL_CHECK	0
#define FREQUENCY_CHECK		1
#define LEVEL_CHECK			2
#define FREQ_LEVEL_CHECK_MOVING	3


#define HDCP_CLASS_MEDIATEK		0
#define HDCP_CLASS_MSTAR		1
#define HDCP_CLASS_NOVATEK		2
#define HDCP_CLASS_REALTEK		3
#define HDCP_CLASS_GENESIS		4
#define HDCP_CLASS_MSTAR60		5
#define HDCP_CLASS_NOVATEK60	6
#define HDCP_CLASS_REALTEK60	7


//	Global Variable
extern CTypedPtrList<CPtrList, CStep*>	StepList;
extern CTypedPtrList<CPtrList, CStep*>	StepList_EM;
extern CTypedPtrList<CPtrList, CStep*>  FullSeq;
extern CTypedPtrList<CPtrList, CStep*>	ForcedAutoStepList;

extern CEnvironmentData*				CurrentSet;
extern CStep*							pCurStep;
extern CVFTestInfo*						pCurStep_EM;
extern CFunction*						pCurFunc;
extern CFunction*						pCurFunc_EM;
extern POSITION							PosStep;
//extern POSITION							PosStep_EM;
extern POSITION							PosFunc;


extern clock_t							Start;
extern clock_t							StepStart;
extern CString							sModel, sSeq, sTested, sStatus, sFailed, sTime, sRate, sMode;
extern int								CurrentDeviceNo;
extern int								g_nStepSize;
extern int								g_nStepSize_EM;
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
extern int				g_nNoFailedStep;
extern int				g_nStepSize;

extern int				g_nStepPlusCount;
extern int				g_nNoFailedStep_EM;

extern UINT			g_nRunningProcessNo;
extern UINT			g_nGrabberType;
extern UINT			g_nSysType;
extern UINT			g_nSysRobotType;
extern UINT			g_nUseNoScanType;

extern CString			g_sData_MD5;


// Global Function
int GetSysStatus();
void StepInforInit();
void StepInfoDisplay();

//+change kwmoon 080804
//BOOL StepRun();
//int StepRun();

//void ResetStep();
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
BOOL FindResult_EM();
void SwControl_Inite();
void SwControl_DTVIN(int sIn, int sOut);
void ResultPrint();
void OkMessage(CString MsgString);

void CreateFullPath(CString sFullPath);

BOOL CreateModelIniFile(CString sModelIni);
BOOL SaveModelIniFile(CString sIniPath);
BOOL OpenModelIniFile(CString sIniPath, CString sTemp);

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

void DeleteVFTestList();
BOOL OpenVFTestFile(CString sVFTestPath);

BOOL OpenRemoteFile(CString sRemotePath);

//+add kwmoon 071214
BOOL OpenPatternFile(CString sPatternPath);
BOOL FindPatternInfo(int nPatternNo, int* pFilterSize, int* pNoFiltering, int* pRepaint,CString* szRoiShape);

//+del kwmoon 080411
// void SetDirPath(CString sModelFolder);

UINT hex2dec(const  char ccHex);
UINT hexCstr2decNum(const  CString csHex);
UINT hexStr2DecNum(char* cpcHex);
//+ 2007.10.19 Add BY USY
BOOL InitPatternGen(CString sComPort, DWORD wBaudRate);
//+ 2007.12.4 Del BY USY
//BOOL InitRemoteCtrl(CString sComPort, DWORD wBaudRate);
//-
BOOL InitTVCommPort(CString sComPort, DWORD wBaudRate);
BOOL InitAvSwitchBox(CString sComPort, DWORD wBaudRate);
BOOL InitLNB_Detector(CString sComPort, DWORD wBaudRate);
BOOL InitVfm_Detector(CString sComPort, DWORD wBaudRate);
//+ 2007.11.30 Add BY USY
//BOOL InitScanner(CString sComPort, DWORD wBaudRate);
//-
//+add kwmoon 071120
//void InitSysInfoGrid();
//-

//+add kwmoon 080625
BOOL InitI2cAdcCommPort(CString sComPort, DWORD wBaudRate);


//+add kwmoon 080710
BOOL CreateTimeDataFile(CString sData, CString sTime);
void CloseTimeDataFile();
void AddTimeData(CString szTimeData);

//+add kwmoon 080716
void SaveDetailTestResult();
//add 090423
BOOL SaveAdcAdjResult();

//+add kwmoon 080918
void SaveResultSummary(CString strWipid, BOOL bResult, CString sTime);

//+add kwmoon 081024
BOOL OpenModelListFile(CString sModelListPath,int& nNoModel,CStringArray &Divisions);
BOOL SaveModelListFile();

//BOOL CSelectGenerateModel_ModelListOpen(CString sModelListPath, int& nNoModel, CStringArray &Divisions);
//BOOL CSelectGenerateModel_SaveModelListFile(CString sModelListPath);

//+add kwmoon 081028
BOOL CreateModelListFile(CString sModelListPath);
BOOL StepInit_AvSwitchBox();
BOOL StepInit_AvSwitchBox_UHD();
BOOL StepInit_AvSwitchBox_FHD();
BOOL StepInit_SourceAutoControl();
BOOL StepInit_SourceAutoControl_UHD();
BOOL StepInit_SourceAutoControl_FHD();
// add PSh 100310
int GetHDMI_Model(int nModelOption);
int GetCOMP_Model();
//+add PSH 0903038
BOOL StepInit_PatternAutoControl();
BOOL StepInit_PatternAutoControl_UHD();
BOOL StepInit_PatternAutoControl_FHD();
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
BOOL InitDPGen(CString sComPort, DWORD wBaudRate);
BOOL InitAVC(CString sComPort, DWORD wBaudRate);
void ResultData_Delete(LONG nPeriodDay); 
void GmesLog_Delete(LONG nPeriodDay); 
//090424
BOOL MesData_AutoUpLoad();
int  GetUpLoadData_Count();
BOOL Get_ToolOption(CString sOption);

BOOL Get_CountryGroup(CString sCountryGroup);
BOOL Get_Country(CString sCountry);
int Get_CountryGroup_Code(CString sCountryGroup);
int Get_Country_Code(CString sCountry);

void SetI2cClkDelay(); 
//int GetHdcpKeyData(CString sHdcpFilePath, BOOL bFlag);
//int GetHdcpKeyNumber(CString sHdcpFilePath);
void DischargCheck(double dLimit);
void UpDateInfo_ToolOption();
BOOL GmesLog_Save(CString sExePath, CString sComData, BOOL bFlag);
BOOL ServerMonitorLog_Save(CString sExePath, CString sComData, BOOL bFlag);
BOOL ToolOptionLog_Save(CString sExePath, CString sComData, BOOL bFlag);
BOOL Gmes_Connection();
vector<int> makeBitArray(unsigned int val);
BOOL ChekDrmKey();
BOOL ChekDrmKeyScanRead();
int CheckKeyGmesVal();
BOOL OpenSystemInfoFile(CString sSysInfoFolder);
BOOL OpenEPI_ConfigFile(CString sIniPath);
BOOL SystemMonitorLog_Save(CString sLogString);
BOOL SystemMonitorLog_Save(CString sLogString, CString sFile, int Line);
BOOL SystemMonitorLog_Write();


#endif // !defined(AFX_GLOBAL_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_)
