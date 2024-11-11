/***********************************************************

FileName : lgdserver_def.h

Provider : NaenTech, Inc.
Version 1.0
Date :
2015.12.22
2015.12.28

Supported O.S. :
- MS Windows : above Windows XP(SP3)
- Linux : above Linux kernel 3.x

***********************************************************/

#if !defined(__LGDSERVER_DEFINE_H)
#define  __LGDSERVER_DEFINE_H

#pragma once

#include "../windlib/osconfig.h"
#include "../windlib/windlib.h"
#include "../windlib/windlib.ctrl.h"

#include <fcntl.h>
#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
#	include <io.h>
#else
#	include <unistd.h>
#	include <sys/ioctl.h>
#endif

/////////////////////////////////////////////////
//
//

#define SZ_LGDSERVER_VERSION	"0.9.20160520"		//!< The version of lgdserver

#define	N_DEF_PORT_NUM			5050	//!< Port number for lgdserver protocol
#define	N_DEF_SERVER_BACKLOG	5		//!< Default BackLog Count for lgdserver

#define	N_DEF_COMM_TIMEDOUT				1000	//!< Default timedout milli-second for tcp comm. 1 sec
#define	N_DEF_COMM_IMAGE_TIMEDOUT		20000	//!< Default timedout milli-second for seding image data. 20 sec


/*! \brief Define the kind of Detector Type	*/
#define	DXD_TYPE_17X17		0	//!< Define the Type of Detector 17" x 17". \arg resolution = 3072 x 3072, \arg SCREEN_SIZE = 6144
#define	DXD_TYPE_14X17		1	//!< Define the Type of Detector 17" x 14". \arg resolution = 3328 x 2816, \arg SCREEN_SIZE = 6656

#define	CUR_DXD_TYPE		DXD_TYPE_14X17	//!< Define the current Detector Type

#if (CUR_DXD_TYPE == DXD_TYPE_17X17)
#	define N_ROIC_COUNT				12
#	define N_ROIC_SIZE				256	
#	define N_GATE_COUNT				6
#	define N_GATE_BUFFER_SIZE		512
#elif (CUR_DXD_TYPE == DXD_TYPE_14X17)
#	define N_ROIC_COUNT				13		//!< The Count of ROIC  at detector
#	define N_ROIC_SIZE				256		//!< The Size of ROIC buffer
#	define N_GATE_COUNT				11		//!< The Count of GATE at detector
#	define N_GATE_BUFFER_SIZE		256		//!< The Size of GATE Buffer
#else
#	error 'Detector Type is not defined.'
#endif

#define N_DEF_COL_LINE	(N_ROIC_COUNT * N_ROIC_SIZE)		//!< Define the Column count of image data
#define N_DEF_ROW_LINE	(N_GATE_COUNT * N_GATE_BUFFER_SIZE)	//!< Define the Row count of image data

#define	N_DEF_IMAGE_BPP			14	//!< Define the Bit Per Pixel of image data


#define	SZ_APPDATA_PATH				"data"				//!< {AppPath}/data
#define SZ_DET_PARAM_FILENAME		"detparam.db"		//!< {AppPath}/data/detparam.db
#define SZ_FACTORY_PARAM_FILENAME	"factory_param.db"	//!< {AppPath}/data/factory_param.db
#define SZ_MONITORING_DATA_FILENAME	"mondata.db"		//!< {AppPath}/data/mondata.db
#define SZ_SYSDATA_FILENAME			"sysdata.db"		//!< {AppPath}/data/sysdata.db
#define SZ_HISTORY_FILENAME			"shot_history.db"	//!< {AppPath}/data/shot_history.db
#define	SZ_CALDATA_FILENAME			"caldata.db"		//!< {AppPath}/data/caldata.db


/*!	\brief	The kind of Exposure Mode	*/
enum LGD_ExposureMode
{
	EXP_MODE_AUTO = 0,	//!< Auto Exposure Mode
	EXP_MODE_MANUAL = 1,	//!< Manual Mode
};

#define	WINDOW_TIME_TO_MSEC(wTime)		((wTime) * 100)	//!< Change Window-Time to Milli-Second.

#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
#	define	PACK_ATTR(n)
#else
#	define	PACK_ATTR(n)		__attribute__((aligned(n))) 	//!< Macro of Structure align
#endif

//	Structure align for 4 Bytes
#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
#	pragma pack(push, 4)
#endif


/*!	\brief	Definition of RANGE */
typedef struct
{
	LONG	from;
	LONG	to;

#if !defined(DOXYZEN)
} PACK_ATTR(4) RANGE;
#else
} RANGE;
#endif

/*!	\brief	Definition of RANGE */
typedef struct
{
	float	from;
	float	to;

#if !defined(DOXYZEN)
} PACK_ATTR(4) RANGE_F;
#else
} RANGE_F;
#endif
#if 1

typedef struct

{

           DWORD dwImgRow;                  

           DWORD dwImgCol;                   

           DWORD dwImgBPP;                   

           DWORD dwExposureMode;          

           DWORD dwSensitivity;     

           DWORD dwTriggerSensitivity;       

           DWORD dwImagingMode; 

           DWORD dwREF_INT;                  

           DWORD dwREF_TFT;                  

           DWORD dwWindowTime;  

           DWORD dwLineTime;                 

           DWORD dwGateOnTime;  

           DWORD dwIntTime;                   

           DWORD dwResetTime;     

           DWORD dwGain;                      

           float     fTargetGain;       

           float     fGainMargin;      

           DWORD dwOffsetMargin; 

           DWORD dwStdMargin;    

           DWORD dwSurMargin;    

           DWORD dwRefSatValue;   

           DWORD dwCutEdge_T;    

           DWORD dwCutEdge_B;    

           DWORD dwCutEdge_L;    

           DWORD dwCutEdge_R;    

           DWORD dwDummy1;

           DWORD dwDummy2;

           DWORD dwDummy3;

           DWORD dwDummy4;

} DETECTOR_PARAM;

 

 

// LDCMD_GET_PARAMETER2

//!        \brief   Definition of Detector parameter2
#if 0
typedef struct

{

           DWORD dwImageBitMode;

           DWORD dwRefADC;

           DWORD dwLPF;

           DWORD dwCDS1;

           DWORD dwCDS2;

           DWORD dwFA1;

           DWORD dwFA2;

           DWORD dwPeriod_ACLK3_ACLK4;

           DWORD dwGateOnDelay;

           DWORD dwACLK0;

           DWORD dwACLK7;

           DWORD dwBackBias;

           DWORD dwPowerMode;

           DWORD dwStandbyMode;

           DWORD dwInitDummy;

           DWORD dwCleanDummy;

           DWORD dwDarkFlush;

           DWORD dwBrightFlush;

 

           DWORD dwFPGAVersion;

           DWORD dwDSERVERVersion;

 

           DWORD dwFreeGating;              

           DWORD dwFreeGatingCount;       

           DWORD dwFreeGatingMode;       

           DWORD dwFreeGatingEnable;      

           DWORD dwFlushMode;              

           DWORD dwFreeGatingBackBias;    

           DWORD dwDarkBackBias;            

           DWORD dwBrightBackBias;          

           DWORD dwDarkFrontFlush;         

           DWORD dwDarkBackFlush;          

           DWORD dwAEDSensorMode;       

           DWORD dwAEDGateOnSelect;      

           DWORD dwAEDStartLine;            

           DWORD dwAEDLineCount;                     

           DWORD dwAEDThreshold;                     

           DWORD dwAEDTriggermode;       

 

           DWORD dwMicrochipVersion;                 

           DWORD dwAVRVersion;              

           DWORD dwAutoReadyTime;                   

           DWORD dwAutoExposureTime;                

           DWORD dwCleanBackBias;                     

           DWORD dwCleanBackBiasCount;              

           DWORD dwBackBiasStandardMode;                    

           DWORD dwAutoAEDExposureTime;                     

           DWORD dwFlushLineTime;           

           DWORD dwBackbiasOnTime;                   

           DWORD dwFreeGatingReadyToExposure;              

           DWORD dwCleanSecondBackBias;            

           DWORD dwFreeGatingFlushLineTime;                   

           DWORD dwCleanLineTime;                     

           DWORD dwCleanFlushMode;                  

           DWORD dwCleanLineTime2;                    

           DWORD dwCleanFlushMode2;                 

           DWORD dwBackBiasSetupTime;               

           DWORD dwBackBiasFallingTime;              

           DWORD dwCleanFlushGateRisingTime_1st;             

           DWORD dwCleanFlushGateFallingTime_1st;            

           DWORD dwFlushType12GateRisingTime;                

           DWORD dwFlushType12GateFallingTime;               

           DWORD dwFGFlushGateRisingTime;                     

           DWORD dwFGFlushGateFallingTime;                    

           DWORD dwCleanFlushGateRisingTime_2nd;            

           DWORD dwCleanFlushGateFallingTime_2nd;           

           DWORD dwAEDTriggerThreshold;             

           DWORD dwADCBase1;               

           DWORD dwADCBase2;               

           DWORD dwADCBase3;               

           DWORD dwADCBase4;               

           DWORD dwADCBase5;               

           DWORD dwADCBase6;               

           DWORD dwADCBase7;               

           DWORD dwAVRTriggerCount;       

           DWORD dwMeanDark;               

           DWORD dwTriggerSensitivity2;      

           DWORD dwXonEnableTime;                    

           DWORD dwAgingMode;             

           DWORD dwAEDScanmode;          

           DWORD dwAEDLineTime;            

           DWORD dwAEDGateOnTime;        

           DWORD dwAEDResetTime;                     

           DWORD dwAEDCDS1;                

           DWORD dwAEDCDS2;                

           DWORD dwAEDFA1;                  

           DWORD dwAEDFA2;                  

           DWORD dwAEDPeriodACLK3ACLK4;          

           DWORD dwAEDGateOnDelay;       

           DWORD dwAEDACLK0;               

           DWORD dwAEDACLK7Position;     

           DWORD dwAEDPeriodACLK6ACLK7;          

           DWORD dwAEDPowerMode;        

           DWORD dwAEDKeepScanForce;    

           DWORD dwTriggerSensitivity3;      

           DWORD dwTriggerSensitivity4;      

           DWORD dwTriggerSensitivity5;      

           DWORD dwTriggerSensitivity6;      

           DWORD dwTriggerSensitivity7;      

           DWORD dwEnableADC1;                        

           DWORD dwEnableADC2;                        

           DWORD dwEnableADC3;                        

           DWORD dwEnableADC4;                        

           DWORD dwEnableADC5;                        

           DWORD dwEnableADC6;                        

           DWORD dwEnableADC7;                        

           DWORD dwGetBrightADCCount;

           DWORD dwGetBrightADCMode;    

           DWORD dwDummy[171];             

} DETECTOR_PARAM2;

#endif


/*! \brief Definition of Detector parameter - 40 Byte */
typedef struct
{
	DWORD dwImageBitMode;  //!< \arg Default is 2
	DWORD dwRefADC;  //!< \arg Default is 41
	DWORD dwLPF;  //!< \arg Default is 1
	DWORD dwCDS1;  //!< \arg Default is 400
	DWORD dwCDS2; //!< \arg Default is 400
	DWORD dwFA1; //!< \arg Default is 111
	DWORD dwFA2; //!< \arg Default is 111
	DWORD dwPeriod_ACLK3_ACLK4; //!< \arg Default is 350
	DWORD dwGateOnDelay;  //!< \arg Default is 5
	DWORD dwACLK0;  //!< \arg Default is 11
	DWORD dwACLK7;  //!< \arg Default is 11
	DWORD dwBackBias;  //!< \arg Default is 11
	DWORD dwPowerMode;  //!< \arg Default is 11
	DWORD dwStandbyMode;  //!< \arg Default is 11

	DWORD dwInitFlushCnt;  //!< \arg Default is 11
	DWORD dwCleanDummy;  //!< \arg Default is 11
	DWORD dwDarkFlush;  //!< \arg Default is 11
	DWORD dwBrightFlush;  //!< \arg Default is 11

	DWORD dwFPGAVersion;  //!< \FPGA FW version
	DWORD dwDSERVERVersion; //!< \Dserver version

	DWORD dwFreeGatingInterval; //!< \arg Default is 3
	DWORD dwFreeGatingCount; //!< \arg Default is 1
	DWORD dwFreeGatingMode; //!< \arg Default is 2
	DWORD dwFreeGatingEnable; //!< \arg Default is enable
	DWORD dwFlushMode;  //!< \arg Default is 0
	DWORD dwFreeGatingBackBias; //!< \arg Default is 0
	DWORD dwDarkBackBias;  //!< \arg Default is 0
	DWORD dwBrightBackBias; //!< \arg Default is 0
	DWORD dwDarkFrontFlush; //!< \arg Default is 1
	DWORD dwDarkBackFlush; //!< \arg Default is 1

	DWORD dwAEDSensorMode; //!< \arg Default is 0
	DWORD dwPanelSenTrigROICNo; //!< \arg Default is 5
	DWORD dwPanelSen_Line1;  //!< \arg Default is 0
	DWORD dwAEDLineCount;  //!< \arg Default is 8
	DWORD dwPanelSenThreshold1;  //!< \arg Default is 30
	DWORD dwAEDTriggermode; //!< \arg Default is 0

	DWORD dwMicrochipVersion; //!< \ roic micom version
	DWORD dwAVRVersion;  //!< \Aed Micom version

	DWORD dwAutoReadyTime; //!< \ dummy
	DWORD dwAutoExposureTime;  //!< \ dummy
	DWORD dwCleanBackBias;  //!< \ dummy
	DWORD dwCleanBackBiasCount;  //!< \ dummy
	DWORD dwBackBiasStandbyMode;  //!< \ default 0(-1.2v)
	DWORD dwAutoAEDExposureTime;  //!< \ dummy
	DWORD dwFlushLineTime;  //!< \arg Default is 0
	DWORD dwBackBiasOnTime;  //!< \ default 300
	DWORD dwFreeGatingReadyToExposure;  //!< \ default enable
	DWORD dwCleanSecondBackBias;  //!< \ dummy
	DWORD dwFreeGatingFlushLineTime;  //!< \ dummy
	DWORD dwCleanLineTime;  //!< \ dummy
	DWORD dwCleanFlushMode;  //!< \ dummy
	DWORD dwCleanLineTime2;  //!< \ dummy
	DWORD dwCleanFlushMode2;  //!< \ dummy
	DWORD dwBackBiasSetupTime;  //!< \ dummy
	DWORD dwBackBiasFallingTime;  //!< \ dummy
	DWORD dwCleanFlushGateRisingTime_1st;  //!< \ CLEANFLUSHGATERISINGTIME_1ST
	DWORD dwCleanFlushGateFallingTime_1st;  //!< \ dummy
	DWORD dwImageFlushGateRisingTime;  //!< \ FPGA_REG_FLUSHTYPE12GATERISINGTIME
	DWORD dwImageFlushGateFallingTime;  //!< \ dummy
	DWORD dwFGFlushGateRisingTime;  //!< \ FPGA_REG_FGFLUSHGATERISINGTIME
	DWORD dwFGFlushGateFallingTime;  //!< \ dummy
	DWORD dwCleanFlushGateRisingTime_2nd;  //!< \ CLEANFLUSHGATERISINGTIME_2ST
	DWORD dwCleanFlushGateFallingTime_2nd;  //!< \ dummy
	DWORD dwAEDTriggerThreshold;  //!< \ AED thrshold
	DWORD dwADCBase1;  //!< \ dummy
	DWORD dwADCBase2;  //!< \ dummy
	DWORD dwADCBase3;  //!< \ dummy
	DWORD dwADCBase4;  //!< \ dummy
	DWORD dwADCBase5;  //!< \ dummy
	DWORD dwADCBase6;  //!< \ dummy
	DWORD dwADCBase7;  //!< \ dummy
	DWORD dwAVRTriggerCount; //!< \ dummy
	DWORD dwMeanDark;  //!< \ dummy
	DWORD dwAEDSenThreshold_ch2; //!< Sensitivity value of Trigger
	DWORD dwXonEnableTime;  //!< \ dummy
	DWORD dwAgingMode;  //!< \ dummy
	DWORD dwAEDScanmode; //!< \arg Default is 0
	DWORD dwAEDLineTime;  //!< \arg Default is 10200
	DWORD dwAEDGateOnTime; //!< \arg Default is 1000
	DWORD dwAEDResetTime;  //!< \arg Default is 264
	DWORD dwAEDCDS1;  //!< \arg Default is 2000
	DWORD dwAEDCDS2;  //!< \arg Default is 2000
	DWORD dwAEDFA1;  //!< \arg Default is 111
	DWORD dwAEDFA2;  //!< \arg Default is 111
	DWORD dwAEDPeriodACLK3ACLK4; //!< \arg Default is 1100
	DWORD dwAEDGateOnDelay; //!< \arg Default is 5
	DWORD dwAEDACLK0;  //!< \arg Default is 11
	DWORD dwAEDACLK7Position; //!< \arg Default is 1
	DWORD dwAEDPeriodACLK6ACLK7; //!< \arg Default is 50
	DWORD dwPanelSenPwrModeOn; //!< \arg Default is 0
	DWORD dwAEDKeepScanForce; //!< \arg Default is 0
	DWORD dwAEDSenThreshold_ch3; //!< Sensitivity value of Trigger
	DWORD dwAEDSenThreshold_ch4; //!< Sensitivity value of Trigger
	DWORD dwAEDSenThreshold_ch5; //!< Sensitivity value of Trigger
	DWORD dwAEDSenThreshold_ch6; //!< Sensitivity value of Trigger
	DWORD dwAEDSenThreshold_ch7; //!< Sensitivity value of Trigger
	DWORD dwEnableADC1;   //!< enable adc
	DWORD dwEnableADC2;   //!< enable adc
	DWORD dwEnableADC3;   //!< enable adc
	DWORD dwEnableADC4;   //!< enable adc
	DWORD dwEnableADC5;   //!< enable adc
	DWORD dwEnableADC6;   //!< enable adc
	DWORD dwEnableADC7;   //!< enable adc
	DWORD dwGetBrightADCCount;   //!< adc count
	DWORD dwGetBrightADCMode;   //!< upper 30% or total
	DWORD dwKernelVersion;  //!< kernel version
	DWORD dwSleepTest;  //!< Sleep Test
	DWORD dwGateClockType;  //!< Gate clock type
	DWORD dwAEDTimeout;  //!< Aed timeout 30s;1min, 2min,3min, 4min, 5min
	DWORD dwADCPreThreshold;  //!< ADC pre threshold, default 20
	DWORD dwADCPreThresholdTimeout;  //!< ADC pre threshold timeout, default 100ms
	DWORD dwImageAckWaitTime;  // ack wait time for aed image
	DWORD dwXonSignalInterval[5];  // xon signal interval
	DWORD dwAEDPreThresholdCheck;  //!< AED pre threshold check, default 0
	DWORD dwXONRefDAC;  //!< Xon Ref. DAC
	DWORD dwXONOffOrder0;  //!< Xon off order, each use 4 bit, 0000 0000 0000 0000 gate3 gate2 gate1 gate0
	DWORD dwXONOffOrder1;  //!< Xon off order, each use 4 bit, 0000 0000 0000 0000 0000 0000 gate5 gate4
	DWORD dwXONRefDACMode;  //!< Xon Ref. DAC on/off mode
	DWORD dw9SectionMean[9];  // !< 9 section mean
	DWORD dwFlushCountManual;  // !< pre flush count for manual
	DWORD dwFlushCountAED;  // !< pre flush count for aed
	DWORD dwXONALLDarkWaitingTime;  // !< Dark waiting time for XONALL mode
	DWORD dwSystemSleepTimeout;  // !< System sleep timeout
	DWORD dwFactoryMode;  // !< set factory/user mode
	DWORD dwPowerModeLocation;  // !< power mode location
	DWORD dwPowerModeTime;  // !< power mode time
	DWORD dwBrightDarkInterval;  // dummy
	DWORD dwAutoExposureInterval;  // dummy
	DWORD dwXONUseStopSignal;  // dummy
	DWORD dwBrightDarkGatingInterval;  // dummy
	DWORD dwBrightDarkWindowTime;  // dummy
	DWORD dwBrightDarkGatingTime;  // dummy
	DWORD dwFreegatingIntervalMode;  // 0 - mili second, 1 micro second
	DWORD dwFreegatingMicroSecInterval;  // dummy
	DWORD dwStableTime;
	DWORD dwStableBaseWaitTime;
	DWORD dwStableBrightWaitTime;
	DWORD dwUnstableBaseWaitTime;
	DWORD dwUnstableBrightWaitTime;
	DWORD dwWaitAfterBrightTime;
	DWORD dwDynamicOffsetMode;
	DWORD dwPostDarkAtBackgroundDark;
	DWORD dwRefreshMeanDarkAfterPostDark;
	DWORD dwRefreshMeanDarkInterval;
	DWORD dwPostDarkIntervalAfterBright;
	DWORD dwPreDarkIntervalBeforeBright;
	DWORD dwMaxCriteriaValue;
	DWORD dwPanelSenROICPwrPreOnTime;
	DWORD dwPanelSenROICPwrCtrl;
	DWORD dw16SectionMean[16];
	DWORD dwPanelSensingLineSel;
	DWORD dwPanelSenThreshold2;
	DWORD dwPanelSenThreshold3;
	DWORD dwPanelSen_Line2;
	DWORD dwPanelSen_Line3;
	DWORD dwCleanStartDelay;
	DWORD dwCleanCommandInterval;
	DWORD dwCleanCommandCount;
	DWORD dwBootDarkCount;
	DWORD dwBootDarkInterval;
	DWORD dwCleanAfterDark;
	DWORD dwNetworkMode;             // 0:ethernet, 1:WiFi, 2:AP
	DWORD dwRunDarkAfterBright;
	DWORD dwDarkCountAfterBright;
	DWORD dwPanelSenROICPwrOffDelay;
	DWORD dwPwrLevel_BackBias; //2020_1207_yspark_1 : add parameter for the detector_param2
	DWORD dwPwrLevel_VEE;
	DWORD dwPwrLevel_VGG;
	DWORD dwIsSubDarkForBriImg; //2020_1218_yspark_1 : add parameter for the option of bright image (dwIsSubDarkForBriImg)
	DWORD dwImgAcquisitionMode;
	DWORD dwAutoSaveMode;
	DWORD dwBoardDebugLEDOnOff;
	DWORD dwPanelSenPwrModeOff;
	DWORD dwROICPipeLineMode;
	DWORD dwROICDebugOnOff; //2021_0120_yspark_1 : add paramete2 to support roic midom debug on/off
	DWORD dwDropMicomVersion;
	DWORD dwPlatformType; //0-NXP 1-Zync
	DWORD dwEnableBrightWithoutXray; //2021_0224_yspark_2 : add parameters dwEnableBrightWithoutXray
	DWORD dwExcuteCleanAfterBright; //2021_0224_yspark_3 : add parameters dwExcuteCleanAfterBright
	DWORD dwBinningMode;
	DWORD dwROICDoutMode;
	DWORD dwFPGADebugEnable;
	DWORD dwImgAckEnInHotSwapMode; //2021_0405_yspark_1 : add parameter2 dwImgAckEnInHotSwapMode
	DWORD dwManualTimeout;
	DWORD dwPwrLevel_Clean_IsUse;
	DWORD dwPwrLevel_Clean_BackBias;
	DWORD dwPwrLevel_Clean_VEE;
	DWORD dwPwrLevel_Clean_VGG;
	DWORD dwDynamic_FrameInterval;
	DWORD dwDynamic_ExposureTime;
	DWORD dwDynamic_LineTime;
	DWORD dwDynamic_GateonTime;
	DWORD dwDynamic_ResetTime;
	DWORD dwDynamic_CDS1;
	DWORD dwDynamic_CDS2;
	DWORD dwDynamic_FA1;
	DWORD dwDynamic_FA2;
	DWORD dwDynamic_Period_ACLK3_ACLK4;
	DWORD dwDynamic_GateOnDelay;
	DWORD dwDynamic_ACLK0;
	DWORD dwDynamic_ACLK7_Pos;
	DWORD dwDynamic_Period_ACLK6_ACLK7;
	DWORD dwDynamic_ACLK8;
	DWORD dwClean_FG_Mode;
	DWORD dwClean_FG_Count;
	DWORD dwClean_FG_LineTime;
	DWORD dwClean_FG_RisingTime;
	DWORD dwClean_FG_FallingTime;
	DWORD dwClean_FG_OnTime;
	DWORD dwClean_CleanAndPwrSetInterval; //Call FPGA_CMD_GEN_CLEAN ------ Delay(CleanAndPwrSetInterval) ------ Set PwrCtrl(BackBias and VEE) //2021_0416_yspark_4 : Add param2 for clean 
	DWORD dwClean_CleanDoneReadyDelay; //After Clean DONE ---- delay(CleanDoneReadyDelay) -- Exit(Set Detector as READY)
	DWORD dwLogSave_AutoSave;
	DWORD dwLogSave_GetDark;
	DWORD dwLogSave_GetBrightAuto;
	DWORD dwLogSave_GetBrightManual;
	DWORD dwLogSave_GetImgDynamic;
	DWORD dwLogSave_KeyProcess;
	DWORD dwLogSave_Network;
	DWORD dwLogSave_ControlBox;
	DWORD dwLogSave_Drop;
	DWORD dwLogSave_AutoSleep;
	DWORD dwLogSave_WebServer;
	DWORD dwCpuClk_Speed;  // 0 - (300MHz 299999) 1 (400Mhz 399999), 2(600Mhz 599999), 3(1.2GHz 1199999)
	DWORD dwWebServerVersion;
	DWORD dwExcuteCleanAfterCancel; //2021_0817_yspark_3 : add excuteCleanAfterCancel in the param2
	DWORD dwClean_CleanTypeFPGAorSoC; //0 - Excute clean by FPGA, 1 - Excute Clean by SoC
	DWORD dwExpAckDelayManualMode; //2021_0927_yspark_3_1 : add parameter for the delay exp_ack for the manual mode
	DWORD dwDarkAgingInterval;     //2021_1001_yspark_3 : Add param2 the interval of dark aging (step - sec)
	DWORD dwAutoSaveExitDelay;     //2021_1001_yspark_4 : Add param2 the delay time for auto save exit
	DWORD dwDebugLEDTestMode;      //2021_1018_yspark_2 : Add param2 for the led test mode
	DWORD dwAECField;
	DWORD dwAECKvP;
	DWORD dwAECmAs;
	DWORD dwAECMode;
	DWORD dwAECGainAttenuation;
	DWORD dwAECPotentioMeter1; //Ch1  Ch0  for AEC1
	DWORD dwAECPotentioMeter2; //Ch1  Ch0  for AEC2
	DWORD dwAECPotentioMeter3; //Ch1  Ch0  for AEC3
	DWORD dwDummy[11];  // dummy

//#if !defined(DOXYZEN)
//} PACK_ATTR(4) DETECTOR_PARAM2;
}DETECTOR_PARAM2;

// LDCMD_GET_WIFI_MODULE

//!        \brief   Definition of WiFi module

typedef struct

{

           char strWiFiModule[512];

} WIFI_PARAM;

 

// LDCMD_GET_DARK_IMAGE

// LDCMD_GET_BRIGHT_IMAGE

// LDCMD_GET_PARAMETER 호출 후 row, column 값으 ㄹ가지고 memory 할당 후 사용

// ptrImage = (unsigned short *)malloc(sizeof(unsigned short)*detParam.dwImgRow* detParam.dwImgCol);

//

//!        \brief   Definition of dark image

typedef struct

{

    unsigned short *ptrImage;

} IMAGE_PARAM;

 

// LDCMD_GET_AVR_COMMAND

//!        \brief   Definition of avr command

typedef struct

{

           char head1;

           char head2;

           char addr;

           char cmd;

           char data0_low;

           char data0_high;

           char data1_low;

           char data1_high;

           char data2_low;

           char data2_high;

           char data3_low;

           char data3_high;

           char data4_low;

           char data4_high;

           char data5_low;

           char data5_high;

           char data6_low;

           char data6_high;

           char data7_low;

           char data7_high;

           char data8_low;

           char data8_high;

           char data9_low;

           char data9_high;

} AVR_PARAM;

#else

/*!	\brief	Definition of Detector parameter - 100 Byte */
typedef struct
{
	DWORD	dwImgRow;		//!< Row count of image. Default is 2816. See \ref N_DEF_ROW_LINE.
	DWORD	dwImgCol;		//!< Column count of image. Default is 3328. See \ref N_DEF_COL_LINE.
	DWORD	dwImgBPP;		//!< Bit Per Pixel. Default is 14
	DWORD	dwExposureMode;	//!< Exposure Mode. See \ref LGD_ExposureMode.
	DWORD	dwSensitivity;	//!< Sensitivity
	DWORD	dwTriggerSensitivity;	//!< Sensitivity value of Trigger
	DWORD	dwImagingMode;	//!< Image Mode. \arg 0 - Portrait, \arg 1 - Landscape
	DWORD	dwREF_INT;		//!< \arg Default is 32.
	DWORD	dwREF_TFT;		//!< \arg Default is 32
	DWORD	dwWindowTime;	//!< \arg Default is 15. \arg 100 ms unit. \arg 10 Window Time = 1000 msec = 1 sec.
	DWORD	dwLineTime;		//!< \arg Default is 4950.
	DWORD	dwGateOnTime;	//!< \arg Default is 594
	DWORD	dwIntTime;		//!< \arg Default is 825
	DWORD	dwResetTime;	//!< \arg Default is 264
	DWORD	dwGain;			//!< The gain value for calibration. \arg The Range is 0 to 256. \arg Default is 15
	float	fTargetGain;	//!< The Target gain value for calibration. \arg Default is 1.0
	float	fGainMargin;	//!< The Gain Margin value for calibration. \arg The Range is -1.0 to 1.0. \arg Default is 0.3
	DWORD	dwOffsetMargin;	//!< The Offset Margin value value for calibration. \arg Default is 45
	DWORD	dwStdMargin;	//!< The Target gain value for calibration. \arg Default is 20
	DWORD	dwSurMargin;	//!< The Target gain value for calibration. \arg Default is 25
	DWORD	dwRefSatValue;	//!< The Reference saturation value for calibration. \arg Default is 13000
	DWORD	dwCutEdge_T;	//!< The top area of Cutting Edge for calibration. \arg Default is 0
	DWORD	dwCutEdge_B;	//!< The bottom area of Cutting Edge for calibration. \arg Default is 0
	DWORD	dwCutEdge_L;	//!< The left area of Cutting Edge for calibration. \arg Default is 0
	DWORD	dwCutEdge_R;	//!< The right area of Cutting Edge for calibration. \arg Default is 0
	DWORD	dwDummy1;
	DWORD	dwDummy2;
	DWORD	dwDummy3;
	DWORD	dwDummy4;

#if !defined(DOXYZEN)
} PACK_ATTR(4) DETECTOR_PARAM;
#else
} DETECTOR_PARAM;
#endif

/*!	\brief	Definition of Detector parameter - 100 Byte */
typedef struct
{
	DWORD	dwImageBitMode;
	DWORD	dwRefADC;
	DWORD	dwLPF;
	DWORD	dwCDS1;
	DWORD	dwCDS2;
	DWORD	dwFA1;
	DWORD	dwFA2;
	DWORD	dwPeriod_ACLK3_ACLK4;
	DWORD	dwGateOnDelay;
	DWORD	dwACLK0;
	DWORD	dwACLK7;
	DWORD	dwBackBias;
	DWORD	dwPowerMode;
	DWORD	dwStandbyMode;
	DWORD	dwInitDummy;
	DWORD	dwCleanDummy;
	DWORD	dwDarkFlush;
	DWORD	dwBrightFlush;

	DWORD	dwFPGAVersion;
	DWORD	dwDSERVERVersion;

	DWORD	dwFreeGating;		//!< \arg Default is 4
	DWORD	dwFreeGatingCount;	//!< \arg Default is 1
	DWORD	dwFreeGatingMode;	//!< \arg Default is 2
	DWORD	dwFreeGatingEnable;	//!< \arg Default is disable
	DWORD	dwFlushMode;		//!< \arg Default is 0
	DWORD	dwFreeGatingBackBias;	//!< \arg Default is 0
	DWORD	dwDarkBackBias;		//!< \arg Default is 0
	DWORD	dwBrightBackBias;	//!< \arg Default is 0
	DWORD	dwDarkFrontFlush;	//!< \arg Default is 1
	DWORD	dwDarkBackFlush;	//!< \arg Default is 1
	DWORD	dwAEDSensorMode;	//!< \arg Default is 0
	DWORD	dwAEDGateOnSelect;	//!< \arg Default is 5
	DWORD	dwAEDStartLine;		//!< \arg Default is 0
	DWORD	dwAEDLineCount;		//!< \arg Default is 8
	DWORD	dwAEDThreshold;		//!< \arg Default is 30
	DWORD	dwAEDTriggermode;	//!< \arg Default is 0

	DWORD	dwMicrochipVersion;		//!< \ dummy
	DWORD	dwAVRVersion;		//!< \ dummy
	DWORD	dwAutoReadyTime;		//!< \ dummy
	DWORD	dwAutoExposureTime;		//!< \ dummy
	DWORD	dwCleanBackBias;		//!< \ dummy
	DWORD	dwCleanBackBiasCount;		//!< \ dummy
	DWORD	dwDummy7;		//!< \ dummy
	DWORD	dwDummy8;		//!< \ dummy
	DWORD	dwDummy9;		//!< \ dummy
	DWORD	dwDummy10;		//!< \ dummy
	DWORD	dwDummy11;		//!< \ dummy
	DWORD	dwDummy12;		//!< \ dummy
	DWORD	dwDummy13;		//!< \ dummy
	DWORD	dwDummy14;		//!< \ dummy
	DWORD	dwDummy15;		//!< \ dummy
	DWORD	dwDummy16;		//!< \ dummy
	DWORD	dwDummy17;		//!< \ dummy
	DWORD	dwDummy18;		//!< \ dummy
	DWORD	dwDummy19;		//!< \ dummy
	DWORD	dwDummy20;		//!< \ dummy
	DWORD	dwDummy21;		//!< \ dummy
	DWORD	dwDummy22;		//!< \ dummy
	DWORD	dwDummy23;		//!< \ dummy
	DWORD	dwDummy24;		//!< \ dummy
	DWORD	dwDummy25;		//!< \ dummy
	DWORD	dwDummy26;		//!< \ dummy
	DWORD	dwDummy27;		//!< \ dummy
	DWORD	dwDummy28;		//!< \ dummy
	DWORD	dwDummy29;		//!< \ dummy
	DWORD	dwDummy30;		//!< \ dummy
	DWORD	dwDummy31;		//!< \ dummy
	DWORD	dwDummy32;		//!< \ dummy
	DWORD	dwDummy33;		//!< \ dummy
	DWORD	dwDummy34;		//!< \ dummy
	DWORD	dwDummy35;		//!< \ dummy
	DWORD	dwDummy36;		//!< \ dummy
	DWORD	dwDummy37;		//!< \ dummy
	DWORD	dwDummy38;		//!< \ dummy
	DWORD	dwDummy39;		//!< \ dummy
	DWORD	dwDummy40;		//!< \ dummy

#if !defined(DOXYZEN)
} PACK_ATTR(4) DETECTOR_PARAM2;
#else
} DETECTOR_PARAM2;
#endif
#endif
/*!	\brief	Definition of Detector parameter Range - 144 Byte */
typedef struct
{
	RANGE	rangeImgRow;		//!< The Min/Max value of Row count of image.
	RANGE	rangeImgCol;		//!< The Min value of Column count of image.
	RANGE	rangeSensitivity;	//!< The Min/Max value of Sensitivity.
	RANGE	rangeTriggerSensitivity;	//!< The Min/Max value of Trigger Sensitivity.
	RANGE	rangeWindowTime;	//!< The Min/Max value of Window Time.
	RANGE	rangeGain;			//!< The Min/Max value of gain value for calibration.
	RANGE_F	rangefTargetGain;	//!< The Min/Max value of Target gain value for calibration.
	RANGE_F	rangefGainMargin;	//!< The Min/Max value of Gain Margin value for calibration.
	RANGE	rangeOffsetMargin;	//!< The Min/Max value of Offset Margin value value for calibration.
	RANGE	rangeStdMargin;		//!< The Min/Max value of Std Margin for calibration.
	RANGE	rangeSurMargin;		//!< The Min/Max value of Surround Margin for calibration.
	RANGE	rangeRefSatValue;	//!< The Min/Max value of Reference saturation value for calibration.
	RANGE	rangeCutEdge_T;		//!< The Min/Max value of top area of Cutting Edge for calibration.
	RANGE	rangeCutEdge_B;		//!< The Min/Max value of bottom area of Cutting Edge for calibration.
	RANGE	rangeCutEdge_L;		//!< The Min/Max value of left area of Cutting Edge for calibration.
	RANGE	rangeCutEdge_R;		//!< The Min/Max value of right area of Cutting Edge for calibration.
	RANGE	rangeDummy1;
	RANGE	rangeDummy2;
	RANGE	rangeDummy3;
	RANGE	rangeDummy4;
#if !defined(DOXYZEN)
} PACK_ATTR(4) DETECTOR_PARAM_RANGE;
#else
} DETECTOR_PARAM;
#endif

/*!	\brief	Definition of Detector parameter - 100 Byte */
typedef struct
{
	DWORD	dwROIC0;
	DWORD	dwROIC1;
	DWORD	dwROIC2;
	DWORD	dwROIC3;
	DWORD	dwROIC4;
	DWORD	dwROIC5;
	DWORD	dwROIC6;
	DWORD	dwROIC7;

#if !defined(DOXYZEN)
} PACK_ATTR(4) DETECTOR_ROIC;
#else
} DETECTOR_ROIC;
#endif


/*!	\brief	Definition of Monitoring Data. - 72 Byte */
typedef struct
{
	DWORD	dwDarkCount;		//!< The count of dark image
	DWORD	dwBrightCount;		//!< The count of bright image
	DWORD	dwTitleStatus;		//!< Tilt switch on or off. \arg 0 - off, \arg 1 - on
	int		nDetectorAngle;		//!< The Angle of Detector by gSensor. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	gSensorStatus;		//!< The Status of gSensor. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	ControlBoxStatus;	//!< The Status of Control Box. \arg 0 - Disconnected, \arg 1 - Connected
	DWORD	EthernetLinkStatus;	//!< The Status of Ethernet Link. \arg 0 - Disconnected, \arg 1 - Connected
	DWORD	WIFILinkStatus;		//!< The Status of WIFI Link. \arg 0 - Disconnected, \arg 1 - Connected
	DWORD	dwTemperature;		//!< The Temperature value of detector
	DWORD	dwBatteryPower;		//!< The power level of battery. 0xFFFFFFFF if unpluged.
	DWORD	FPGA_Status;		//!< The Status of FPGA. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	PrepTrigger_Status;	//!< The Status of Prepare Trigger. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	ExpTrigger_Status;	//!< The Status of Exposure Trigger. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	AutoTrigger_Status;	//!< The Status of Auto Trigger. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	LED0_Status;		//!< The Status of LED 0. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	LED1_Status;		//!< The Status of LED 1. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	LED2_Status;		//!< The Status of LED 2. \arg 0 - Inactive, \arg 1 -  Active
	DWORD	LED3_Status;		//!< The Status of LED 3. \arg 0 - Inactive, \arg 1 -  Active
#if !defined(DOXYZEN)
} PACK_ATTR(4) MONITORING_DATA;
#else
} MONITORING_DATA;
#endif

/*!	\brief	Definition of SetTime Data. - 24 Byte */
typedef struct
{
	int		nYear;		//!< 
	int		nMonth;		//!< 
	int		nDay;		//!< 
	int		nHour;		//!< 
	int		nMinute;	//!< 
	int		nSecond;	//!< 
#if !defined(DOXYZEN)
} PACK_ATTR(4) SET_TIME_DATA;
#else
} SET_TIME_DATA;
#endif


/*!	\brief	Definition of Get/Set Network Info Data. - 152 Byte */
typedef struct
{
	char	szIPAddr[16];	//!< IP Addr
	char	szNetMask[16];	//!< Network Mask
	char	szGateway[16];	//!< Gateway 

	char	szEth_MACAddr[20];	//!< ETH MAC address. Read Only.
	char	szWLAN_MACAddr[20];	//!< WIFI MAC address. Read Only.

	char	szWLAN_SSID[64];	//!< WIFI SSID
	char	szWLAN_Password[64];	//!< WIFI Password.
	char	szWLAN_CountryCode[8];	//!< WIFI Country Code.
	int		nConnectType;
#if !defined(DOXYZEN)
} PACK_ATTR(4) NETWORK_INFO_DATA;
#else
} NETWORK_INFO_DATA;
#endif


/*!	\brief	Definition of Get/Set System Info - 108 Byte */
typedef struct
{
	char	szProductDate[20];	//!< Product Date, yyyymmddHHMMSS
	char	szModelName[32];	//!< Model Name
	char	szSerialNo[32];		//!< Serial No
	char	szInstallDate[20];	//!< Install Date, yyyymmddHHMMSS
	DWORD	dwEnableWIFIDriver;	//!< Enable/Disable WIFI Driver. \arg 0 - Disable, \arg 1 - Enable
	DWORD	dwTimeFormat;		//!< Display time format. \arg 0 - YYYY/MM/DD hh:mm:ss, \arg 1 - MM/DD/YYYY hh:mm:ss, \arg 2 - DD/MM/YYYY hh:mm:ss
	DWORD	dwVersion_Major;	//!< The Major Version of SOC Packet.
	DWORD	dwVersion_Minor;	//!< The Minor Version of SOC Packet.
	DWORD	dwVersion_Release;	//!< The Release Version of SOC Packet.
	DWORD	dwSleepTime;		//!< Sleep time
	DWORD	dwShutdownTime;		//!< Shutdown time
	DWORD	dwDummy1;			//!< dummy1
	DWORD	dwDummy2;			//!< dummy2
	DWORD	dwDummy3;			//!< dummy3
	DWORD	dwDummy4;			//!< dummy4
	DWORD	dwDummy5;			//!< dummy5
#if !defined(DOXYZEN)
} PACK_ATTR(4) SYSTEM_INFO_DATA;
#else
} SYSTEM_INFO_DATA;
#endif

#define	SZ_UPLOAD_SOC_DATA_MAGIC_STR		"KDMF"	//!< Define of Magic String for struct \ref UPLOAD_SOC_DATA
#define	N_UPLOAD_SOC_DATA_MAGIC_STR_LEN		4		//!< Define of length of Magic String for struct \ref UPLOAD_SOC_DATA

/*!	\brief	Definition of UploadSOC - 32 + nnn Byte */
typedef struct
{
	char	szMagicString[4];	//!< Fixed data. "KDMF"
	DWORD	dwTotalFileSize;	//!< Sum of sizeof(UPLOAD_SOC_DATA) + dwKernel_FileSize + dwDServer_FileSize + dwMiCom_FileSize + dwFPGA_FileSize
	DWORD	dwKernel_FileSize;	//!< The file size fo kernel image
	DWORD	dwDTB_FileSize;		//!< The file size fo DTB
	DWORD	dwDServer_FileSize;	//!< The file size fo DServer image
	DWORD	dwFPGA_FileSize;	//!< The file size fo FPGA image
	DWORD	dwMiCom_FileSize;	//!< The file size fo MiCom image
	DWORD	dwAVR_FileSize;			//!< The file size fo AVR image
	DWORD	dwCheckSum;			//!< CheckSum Data
	BYTE	caPackageVersion[4];			//!< Packet version major(1), minor(1), release(2)
#if !defined(DOXYZEN)
} PACK_ATTR(4) UPLOAD_SOC_DATA;
#else
} UPLOAD_SOC_DATA;
#endif


/*!	\brief	Definition of FPGA regitser set - 8 Byte */
typedef struct
{
	DWORD		addr;		//!< 
	DWORD		data;		//!< 
#if !defined(DOXYZEN)
} PACK_ATTR(4) SET_FPGA_REGISTER;
#else
} SET_FPGA_REGISTER;
#endif

#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
#	pragma pack(pop)
#endif

#endif	//  __LGDSERVER_DEFINE_H

/*
// LDCMD_GET_PARAMETER

//!        \brief   Definition of Detector parameter

typedef struct

{

           DWORD dwImgRow;                  

           DWORD dwImgCol;                   

           DWORD dwImgBPP;                   

           DWORD dwExposureMode;          

           DWORD dwSensitivity;     

           DWORD dwTriggerSensitivity;       

           DWORD dwImagingMode; 

           DWORD dwREF_INT;                  

           DWORD dwREF_TFT;                  

           DWORD dwWindowTime;  

           DWORD dwLineTime;                 

           DWORD dwGateOnTime;  

           DWORD dwIntTime;                   

           DWORD dwResetTime;     

           DWORD dwGain;                      

           float     fTargetGain;       

           float     fGainMargin;      

           DWORD dwOffsetMargin; 

           DWORD dwStdMargin;    

           DWORD dwSurMargin;    

           DWORD dwRefSatValue;   

           DWORD dwCutEdge_T;    

           DWORD dwCutEdge_B;    

           DWORD dwCutEdge_L;    

           DWORD dwCutEdge_R;    

           DWORD dwDummy1;

           DWORD dwDummy2;

           DWORD dwDummy3;

           DWORD dwDummy4;

} DETECTOR_PARAM;

 

 

// LDCMD_GET_PARAMETER2

//!        \brief   Definition of Detector parameter2

typedef struct

{

           DWORD dwImageBitMode;

           DWORD dwRefADC;

           DWORD dwLPF;

           DWORD dwCDS1;

           DWORD dwCDS2;

           DWORD dwFA1;

           DWORD dwFA2;

           DWORD dwPeriod_ACLK3_ACLK4;

           DWORD dwGateOnDelay;

           DWORD dwACLK0;

           DWORD dwACLK7;

           DWORD dwBackBias;

           DWORD dwPowerMode;

           DWORD dwStandbyMode;

           DWORD dwInitDummy;

           DWORD dwCleanDummy;

           DWORD dwDarkFlush;

           DWORD dwBrightFlush;

 

           DWORD dwFPGAVersion;

           DWORD dwDSERVERVersion;

 

           DWORD dwFreeGating;              

           DWORD dwFreeGatingCount;       

           DWORD dwFreeGatingMode;       

           DWORD dwFreeGatingEnable;      

           DWORD dwFlushMode;              

           DWORD dwFreeGatingBackBias;    

           DWORD dwDarkBackBias;            

           DWORD dwBrightBackBias;          

           DWORD dwDarkFrontFlush;         

           DWORD dwDarkBackFlush;          

           DWORD dwAEDSensorMode;       

           DWORD dwAEDGateOnSelect;      

           DWORD dwAEDStartLine;            

           DWORD dwAEDLineCount;                     

           DWORD dwAEDThreshold;                     

           DWORD dwAEDTriggermode;       

 

           DWORD dwMicrochipVersion;                 

           DWORD dwAVRVersion;              

           DWORD dwAutoReadyTime;                   

           DWORD dwAutoExposureTime;                

           DWORD dwCleanBackBias;                     

           DWORD dwCleanBackBiasCount;              

           DWORD dwBackBiasStandardMode;                    

           DWORD dwAutoAEDExposureTime;                     

           DWORD dwFlushLineTime;           

           DWORD dwBackbiasOnTime;                   

           DWORD dwFreeGatingReadyToExposure;              

           DWORD dwCleanSecondBackBias;            

           DWORD dwFreeGatingFlushLineTime;                   

           DWORD dwCleanLineTime;                     

           DWORD dwCleanFlushMode;                  

           DWORD dwCleanLineTime2;                    

           DWORD dwCleanFlushMode2;                 

           DWORD dwBackBiasSetupTime;               

           DWORD dwBackBiasFallingTime;              

           DWORD dwCleanFlushGateRisingTime_1st;             

           DWORD dwCleanFlushGateFallingTime_1st;            

           DWORD dwFlushType12GateRisingTime;                

           DWORD dwFlushType12GateFallingTime;               

           DWORD dwFGFlushGateRisingTime;                     

           DWORD dwFGFlushGateFallingTime;                    

           DWORD dwCleanFlushGateRisingTime_2nd;            

           DWORD dwCleanFlushGateFallingTime_2nd;           

           DWORD dwAEDTriggerThreshold;             

           DWORD dwADCBase1;               

           DWORD dwADCBase2;               

           DWORD dwADCBase3;               

           DWORD dwADCBase4;               

           DWORD dwADCBase5;               

           DWORD dwADCBase6;               

           DWORD dwADCBase7;               

           DWORD dwAVRTriggerCount;       

           DWORD dwMeanDark;               

           DWORD dwTriggerSensitivity2;      

           DWORD dwXonEnableTime;                    

           DWORD dwAgingMode;             

           DWORD dwAEDScanmode;          

           DWORD dwAEDLineTime;            

           DWORD dwAEDGateOnTime;        

           DWORD dwAEDResetTime;                     

           DWORD dwAEDCDS1;                

           DWORD dwAEDCDS2;                

           DWORD dwAEDFA1;                  

           DWORD dwAEDFA2;                  

           DWORD dwAEDPeriodACLK3ACLK4;          

           DWORD dwAEDGateOnDelay;       

           DWORD dwAEDACLK0;               

           DWORD dwAEDACLK7Position;     

           DWORD dwAEDPeriodACLK6ACLK7;          

           DWORD dwAEDPowerMode;        

           DWORD dwAEDKeepScanForce;    

           DWORD dwTriggerSensitivity3;      

           DWORD dwTriggerSensitivity4;      

           DWORD dwTriggerSensitivity5;      

           DWORD dwTriggerSensitivity6;      

           DWORD dwTriggerSensitivity7;      

           DWORD dwEnableADC1;                        

           DWORD dwEnableADC2;                        

           DWORD dwEnableADC3;                        

           DWORD dwEnableADC4;                        

           DWORD dwEnableADC5;                        

           DWORD dwEnableADC6;                        

           DWORD dwEnableADC7;                        

           DWORD dwGetBrightADCCount;

           DWORD dwGetBrightADCMode;    

           DWORD dwDummy[171];             

} DETECTOR_PARAM2;

 

// LDCMD_GET_WIFI_MODULE

//!        \brief   Definition of WiFi module

typedef struct

{

           char strWiFiModule[512];

} WIFI_PARAM;

 

// LDCMD_GET_DARK_IMAGE

// LDCMD_GET_BRIGHT_IMAGE

// LDCMD_GET_PARAMETER 호출 후 row, column 값으 ㄹ가지고 memory 할당 후 사용

// ptrImage = (unsigned short *)malloc(sizeof(unsigned short)*detParam.dwImgRow* detParam.dwImgCol);

//

//!        \brief   Definition of dark image

typedef struct

{

           unsigsned short *ptrImage;

} IMAGE_PARAM;

 

// LDCMD_GET_AVR_COMMAND

//!        \brief   Definition of avr command

typedef struct

{

           char head1;

           char head2;

           char addr;

           char cmd;

           char data0_low;

           char data0_high;

           char data1_low;

           char data1_high;

           char data2_low;

           char data2_high;

           char data3_low;

           char data3_high;

           char data4_low;

           char data4_high;

           char data5_low;

           char data5_high;

           char data6_low;

           char data6_high;

           char data7_low;

           char data7_high;

           char data8_low;

           char data8_high;

           char data9_low;

           char data9_high;

} AVR_PARAM;

*/