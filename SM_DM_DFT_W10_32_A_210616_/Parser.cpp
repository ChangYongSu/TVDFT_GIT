#include "stdafx.h"
#include "resource.h"

#include "parser.h"
#include "step.h"
#include "global.h"
#include "realmeas.h"
#include "StepMeasFunc.h"
#include "progdlg.h"
#include "StringTokenizer.h"

#include <ctype.h>
#include <windowsx.h>
#include <sys\stat.h>
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Global Variables
CTypedPtrList<CPtrList, CStep*> StepList;
CTypedPtrList<CPtrList, CStep*> FullSeq;

CStep*		pCurStep;
CFunction*	pCurFunc;

CTypedPtrList<CPtrList, CFunction*> CurFunc;	//Current Sub Step List

extern CDATsysView*  g_pView;

CString g_CommantArray;
CString g_CommantFuncArray;

//+change 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

POSITION	PosStep;
POSITION	PosFunc;

CString		sMsg;
CString		cFuncName;
char		achCurString[256];
char*		pWalker;

int			nCurLine;
int			cMeasFuncType;

Token*		pToken;
LPSTR		UserFile[MAXLINE];
BOOL		bCompiled;
LPSTR		FullSeqFile[MAXLINE];

CEnvironmentData *CurrentSet;

double (*cMeasFunc)();

BOOL  MeasFunc_SyntexChecking(char* achFunc);

union
{
    int nNo;
    char achName[40];
} Parsing;

///////////////////////////////////////////////////////////////

_NameTable NameDefined[] = 
{
	_T("i2c_type")		 ,	I2C_TYPE,		INTEGER, 
	_T("i2c_cmd")		 ,	I2C_CMD,		INTEGER, 
	_T("i2c_read")		 ,	I2C_READ,		INTEGER, 
	_T("i2c_chk_value")	 ,	I2C_CHK_VALUE,	INTEGER, 
	_T("i2c_chk_maxmin") ,	I2C_CHK_MAXMIN,	INTEGER, 
	_T("i2c_chk_limit")	 ,	I2C_CHK_LIMIT,	INTEGER, 
	//+add psh 090612
	_T("i2c_chk_ver")	 ,	I2C_CHK_VER,	INTEGER, 
	_T("i2c_write_tool_option")	 ,	I2C_WRITE_TOOL_OPTION,	INTEGER,
	
	_T("ddc_2ab")		 ,	DDC_2AB,		INTEGER, 
	//+add kwmoon 080804
	_T("ddc_2b")		 ,	DDC_2B,			INTEGER, 
	_T("i2c_channel")	 ,	I2C_CHANNEL,	INTEGER, 

	//+add kwmoon 080911
	_T("i2c_chk_ver")		 ,	DDC_2B,			INTEGER, 
	
	_T("rs232c")		 ,	0,				INTEGER, 
	_T("i2c")		     ,	1,				INTEGER, 

	//+add kwmoon 080625
	_T("i2c_2b_read")		 ,	I2C_2B_READ,		INTEGER, 
	_T("i2c_2b_write")		 ,	I2C_2B_WRITE,		INTEGER, 

	//+add PSH 080417
	_T("cpu_ver")		 ,	1,				INTEGER,
	_T("micom_ver")		 ,	2,				INTEGER,
	_T("usb_ver")		 ,	3,				INTEGER,

	//+add kwmoon 080418
	_T("dio_on_off")	 ,	1,				INTEGER,
	_T("video_ch")		 ,	2,				INTEGER,
	_T("audio_ch")		 ,	3,				INTEGER,
	_T("comp_ch")		 ,	4,				INTEGER,
	_T("cvbs_ch")		 ,	5,				INTEGER,
	_T("dc_out_level")	 ,	6,				INTEGER,
	_T("ir_level")		 ,	7,				INTEGER,
	_T("remote_type")	 ,	8,				INTEGER,
	_T("check_audio_level")	 ,	9,			INTEGER,
	_T("check_audio_freq")	 ,	10,			INTEGER,
	_T("check_status")		 ,	11,			INTEGER,
	_T("remote_code")		 ,	12,			INTEGER,
	_T("remote_repeat_no")	 ,	13,			INTEGER,
	
	//+add kwmoon 080929
	_T("rgb_sound_on")		 ,	14,			INTEGER,

	//+add kwmoon 080418
	_T("off")				 ,	0,			INTEGER,
	_T("on")				 ,	1,			INTEGER,

	//+add kwmoon 080418
	_T("rf_ntsc")			 ,	0,			INTEGER,
	_T("rf_pal")			 ,	1,			INTEGER,

	//+add PSH 20100623
	_T("vdc")				,	1,			INTEGER,
	_T("vac")				,	2,			INTEGER,
	_T("res")				,	3,			INTEGER,
	_T("fre")				,	4,			INTEGER,
	_T("per")				,	5,			INTEGER,

	NULL, NULL, NULL
};

///////////////////////////////////////////////////////////////

_MeasureFunction MeasDefined[] =
{

	_T("color_test"),		_Color_Test,			MEAS_DOUBLE,
	_T("bright_test"),		_Bright_Test,			MEAS_DOUBLE,
	_T("noise_test"),		_Noise_Test,			MEAS_DOUBLE,
	_T("osd_test"),			_Osd_Test,				MEAS_DOUBLE,
	_T("caption_test"),		_Caption_Test,			MEAS_DOUBLE,
	_T("digitaleye_test"),	_Digital_Eye_Test,		MEAS_DOUBLE,
	_T("position_test"),	_Position_Test,			MEAS_BOOL,
	_T("size_test"),		_Size_Test,				MEAS_BOOL,
	_T("lan_test"),			_Lan_Test,				MEAS_BOOL,
	_T("signal_line_test"),	_Signal_Line_Test,		MEAS_BOOL,
	_T("audio_test"),		_Audio_Test,			MEAS_BOOL,
	_T("moving_pic_test"),	_Moving_Pic_Test,		MEAS_BOOL,
	_T("manual_test"),		_Manual_Test,			MEAS_BOOL,

	_T("if_test"),			_If_Test,				MEAS_BOOL,
	_T("if_test2"),			_If_Test2,				MEAS_DOUBLE,
	_T("if_test2_m"),		_If_Test2_m,			MEAS_DOUBLE,
	_T("version_check"),		_Version_Check,		MEAS_BOOL,

	_T("color_bias_test"),	_Color_Bias_Test,		MEAS_DOUBLE,

	_T("wifi_mac_add_check"),	_Wifi_MAC_Add_Check,	MEAS_BOOL,
	_T("lan_mac_add_check"),	_Lan_MAC_Add_Check,		MEAS_BOOL,
	_T("serial_number_check"),	_Serial_Number_Check,	MEAS_BOOL,
	_T("checktemperature"),		_CheckTemperature,		MEAS_DOUBLE,
	_T("irdata_check"),			_IRDataCheck,		    MEAS_BOOL,
	_T("mac_add_checkgmes"),	_MAC_Add_CheckGmes,		MEAS_BOOL,
	_T("mic_diff_level_check"),	_Mic_Diff_Level_Check,	MEAS_BOOL,


	NULL, NULL, NULL
};
///////////////////////////////////////////////////////////////

_InternalFunction Predefined[] =    
{
	_T("usermessage"), _UserMessage,
	{LP, STRING, COMMA, INTEGER, RP, TNULL},
	{_T("Message"), _T("Delay"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("waitmessage"), _NotifyMessage,
	{LP, STRING, COMMA, STRING, RP, TNULL},
	{_T("Message1"),_T("Message2"), _T("")},
	{NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("delay"), _Delay,
	{LP, INTEGER, RP, TNULL},
	{ _T("Delay"), _T("")},
	{NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("work"), _Work,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("pattern_gen_out"), _PatternGen_Out,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Command"), _T("Data"), _T("Message"), _T("")},
	{PATTERN_CMD_SEL_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("remote_out"), _Remote_Out,
	{LP, STRING, COMMA, INTEGER, COMMA, STRING, COMMA, INTEGER, RP, TNULL},
	{_T("Remote Key"), _T("Repeat"), _T("Message"), _T("Delay"), _T("")},
	{REMOTE_SEL_TYPE, NUM_SEL_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("set_if_command"), _SetTVCommand,
	{LP, STRING, COMMA, INTEGER, RP, TNULL},
	{_T("Command"), _T("Wait Limit"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("send_set_command"), _SendTVCommand,
	{LP, STRING, COMMA,INTEGER, RP, TNULL},
	{_T("Command"), _T("Wait Limit"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("send_set_command2"), _SendTVCommand2,
	{LP, STRING, RP, TNULL},
	{_T("Command"), _T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("grab_n_load_image"), _Grab_N_Load_Image,
	{LP,STRING, COMMA, INTEGER, COMMA,STRING,RP, TNULL},
	{_T("Ref File"), _T("Use Mask"),_T("Message"),_T("")},
	{FILE_SEL_TYPE,USE_SEL_TYPE,NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("grab_image_check"), _Grab_Image_Check,
	{LP, STRING, COMMA, INTEGER, COMMA,INTEGER, COMMA,STRING,RP, TNULL},
	{_T("Ref File"), _T("Use Mask"),_T("Max Delay"),_T("Message"),_T("")},
	{FILE_SEL_TYPE,USE_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	//+add PSH 080412
	_T("grab_image_osd_check"), _Grab_Image_OSD_Check,
	{LP, STRING, COMMA, INTEGER, COMMA,INTEGER, COMMA,STRING,RP, TNULL},
	{_T("Ref File"), _T("Use Mask"),_T("Max Delay"),_T("Message"),_T("")},
	{FILE_SEL_TYPE,USE_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("power_on_check"), _Power_On_Check,
	{LP, INTEGER, COMMA,STRING,RP, TNULL},
	{_T("Power On Delay"),_T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("power_off_check"), _Power_Off_Check,
	{LP, INTEGER, COMMA,STRING,RP, TNULL},
	{_T("Power Off Delay"),_T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

/*	_T("set_lvds_resolution"), _SetLvdsResolution_2in1,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Grab Width"),_T("Grab Height"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NULL_TYPE},
*/	
	_T("setpositionshift"), _SetPositionShift,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Position Shift X"),_T("Position Shift Y"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("usedcolorhighbits"), _UseOnlyColorHighBits,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Use Color HighBits"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},
	
	_T("usedbrighthighbits"), _UseOnlyBrightHighBits,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Use Bright HighBits"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("set_match_rate"), _SetMatchRate,
	{LP, REAL, COMMA, STRING, RP, TNULL},
	{_T("Match Rate(%)"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},
	
	_T("set_avg_filter"), _SetAvgFilter,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Mask Width"),_T("Mask Height"), _T("Repeat"),_T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NUM_SEL_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_analog_video_source"), _SetAnalogVideoSource,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{ _T("NTSC or PAL"), _T("AvSwitchBoxChannel"),_T("Message"),_T("")},
	{RF_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_roi"), _SetRoi,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Start X"), _T("Start Y"), _T("End X"),_T("End Y"), _T("Message"), _T("")},
	{ROI_SEL_TYPE,ROI_SEL_TYPE,ROI_SEL_TYPE,ROI_SEL_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_avswitchbox_signal"), _SetAvSwitchBoxSignal,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA,INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Command Type"), _T("Value 1"), _T("Value 2"),_T("Delay"), _T("Message"), _T("")},
	{AVSWITCH_CMD_SEL_TYPE,AVSWITCH_CMD_SEL_TYPE,AVSWITCH_CMD_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("check_audio"), _CheckAudio,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA,INTEGER, COMMA,STRING,RP, TNULL},
	{_T("Audio Source"), _T("Left Freq"), _T("Right Freq"),_T("Left Level"), _T("Right Level"),_T("Margin"),_T("Message"), _T("")},
	{AUDIO_SRC_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("authentication_key"), _AuthenticationKey,
	{LP, INTEGER, RP, TNULL},
	{_T("Key"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("skipped_signalline_low_bits"), _SkippedSignalLineLowBits,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Data"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

/*	_T("set_tool_option"), _SetToolOption,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Option"), _T("Remote Delay"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_area_option"), _SetAreaOption,
	{LP, INTEGER, COMMA,INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Option"), _T("Remote Delay"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},
*/
	_T("set_manual_test_message"), _SetManualTestMessage,
	{LP, STRING, COMMA, STRING, RP, TNULL},
	{_T("Message1"), _T("Message2"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_caption_size"), _SetCaptionSize,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Set Caption Width"),_T("Set Caption Height"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_digitaleye_delay"), _SetDigitalEyeDelay,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Set Delay"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("not_skip_same_source_channel"), _NotSkipSameSourceChannel,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Not Skip Same Source/Channel"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("set_caption_key"), _SetCaptionKey,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Set Caption Key Delay"),_T("Set Enter Key Delay"),_T("No of On-Key Press"),_T("No of Off-Key Press"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NUM_SEL_TYPE,NUM_SEL_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_caption_check_time"), _SetCaptionCheckTime,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Set Caption Check Time"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_osd_region"), _SetOsdRegion,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Start X"), _T("Start Y"), _T("End X"),_T("End Y"), _T("Message"), _T("")},
	{ROI_SEL_TYPE,ROI_SEL_TYPE,ROI_SEL_TYPE,ROI_SEL_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	//+add kwmoon 080826
	_T("set_invalid_region"), _SetInvalidRegion,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Start X"), _T("Start Y"), _T("End X"),_T("End Y"), _T("Message"), _T("")},
	{ROI_SEL_TYPE,ROI_SEL_TYPE,ROI_SEL_TYPE,ROI_SEL_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

//+add PSH 080516
	_T("pattern_gen_hsync_on"), _PatternGenerator_HSync_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("pattern_gen_vsync_on"), _PatternGenerator_VSync_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("pattern_gen_rgb_on"), _PatternGenerator_RGB_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("pattern_gen_hsync_off"), _PatternGenerator_HSync_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("pattern_gen_vsync_off"), _PatternGenerator_VSync_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

		_T("pattern_gen_rgb_off"), _PatternGenerator_RGB_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},
//+add kwmoon 080523
	_T("set_audio_check_method"), _SetAudioCheckMethod,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Method"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("measure_audio"), _MeasureAudio,
	{LP, INTEGER, RP, TNULL},
	{_T("Audio Source"), _T("")},
	{AUDIO_SRC_SEL_TYPE,NULL_TYPE},

/*	_T("tooloption_write"), _ToolOption_Write,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("areaoption_write"), _AreaOption_Write,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},
*/
//+add PSH 090129
	_T("hdmigen_setedid"), _HDMIGen_SetEDID,
	{LP, INTEGER, RP, TNULL},
	{_T("HDMI Gen. EDID Pass/Check")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("hdmigen_sethdcp"), _HDMIGen_SetHDCP,
	{LP, INTEGER, RP, TNULL},
	{_T("HDMI Gen. HDCP Off/On")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("hdmigen_setout"), _HDMIGen_SetOut,
	{LP, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Model No"), _T("Pattern No"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("hdmigen_setddcline"), _HDMIGen_SetDDCLine,
	{LP, INTEGER, RP, TNULL},
	{_T("HDMI Gen. DDCLine Open/Close")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("checkaudio_level"), _CheckAudio_Level,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Audio Source"), _T("Left Level"), _T("Right Level")},
	{AUDIO_SRC_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("checkaudio_freq"), _CheckAudio_Frequency,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Audio Source"), _T("Left Freq."), _T("Right Freq.")},
	{AUDIO_SRC_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("tv_mic_on"), _TV_Mic_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tv_mic_off"), _TV_Mic_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("setremoconoutretry"), _SetRemoconOutRetry,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("remocon_out"), _Remocon_Out,
	{LP, STRING, COMMA, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Remote Key Name"), _T("Repeat"), _T("Delay"), _T("")},
	{REMOTE_SEL_TYPE, NUM_SEL_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},


	_T("setrelay_onoff"), _SetRelay_OnOff,
	{LP, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Relay No."), _T("On/Off Flag")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("audioleft_out"), _AudioLeft_Out,
	{LP, INTEGER, RP, TNULL},
	{_T("On/Off Flag"), _T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("audioright_out"), _AudioRight_Out,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_movingpictest_margin"), _SetMovingPicTestMargin,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, RP, TNULL},
	{_T("Diff_LoLimit"), _T("DIff_HiLimit"), _T("Test Margin(%)")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_macadd"), _Set_MacAddress,
	{LP, STRING, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

    NULL, NULL, { TNULL }
};                      

int CheckAuthentificationKey(FILE *fp)
{
	char achBuf[256];
	memset(achBuf,NULL,256);

	char aAuthentificationKey[11];
	memset(aAuthentificationKey,NULL,11);
    
	int nAuthentificationKey = 0;

	if (fgets(achBuf, 256, fp) == NULL) return -1;

	strncpy_s(aAuthentificationKey, 11, &achBuf[1], 10); //strncpy_s(aAuthentificationKey,&achBuf[1],10);

	nAuthentificationKey = atoi(aAuthentificationKey);

	time_t timerModifiedDate1  = nAuthentificationKey;
	time_t timerModifiedDate2 = CurrentSet->lTime;

	struct tm* tmModifiedDateBlock1 ;	
	struct tm* tmModifiedDateBlock2;

	localtime_s(tmModifiedDateBlock1, &timerModifiedDate1);
	localtime_s(tmModifiedDateBlock2, &timerModifiedDate2);

	DWORD tmModifiedDateSecond1 = tmModifiedDateBlock1->tm_hour * 3600 +
								  tmModifiedDateBlock1->tm_min * 60 +
							 	  tmModifiedDateBlock1->tm_sec;	
		
	DWORD tmModifiedDateSecond2 = tmModifiedDateBlock2->tm_hour * 3600 +
								  tmModifiedDateBlock2->tm_min * 60 +
								  tmModifiedDateBlock2->tm_sec;	

	if((tmModifiedDateBlock1->tm_year != tmModifiedDateBlock2->tm_year)
	|| (tmModifiedDateBlock1->tm_mon  != tmModifiedDateBlock2->tm_mon)
	|| (tmModifiedDateBlock1->tm_mday != tmModifiedDateBlock2->tm_mday)
	|| (abs(long(tmModifiedDateSecond1 - tmModifiedDateSecond2)) > 10))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

//+change kwmoon 080724
/*
_InputSource InputDefined[] = 
{
	_T("RF"),
	_T("AV"),
	_T("YC"),
	_T("VOUT"),
	_T("AV1"),
	_T("YC1"),
	_T("VOUT1"),
	_T("AV2"),
	_T("YC2"),
	_T("VOUT2"),
	_T("AV3"),
	_T("YC3"),
	_T("VOUT3"),
	_T("COMP"),
	_T("COMP1"),
	_T("COMP2"),
	_T("RGB"),
	_T("HDMI"),
	_T("HDMI1"),
	_T("HDMI2"),
	_T("HDMI3"),
	_T("HDMI4"),
	NULL
};
*/

_InputSource InputDefined[] = 
{
	_T("RF"),
	_T("AV"),
	_T("AV1"),
	_T("AV2"),
	_T("AV3"),
	_T("AV4"),
	_T("YC"),
	_T("YC1"),
	_T("YC2"),
	_T("YC3"),
	_T("YC4"),
	_T("COMP"),
	_T("COMP1"),
	_T("COMP2"),
	_T("COMP3"),
	_T("MNT-OUT"),
	_T("TV-OUT"),
	_T("AV-OUT"),
	_T("RGB"),
	_T("HDMI"),
	_T("HDMI1"),
	_T("HDMI2"),
	_T("HDMI3"),
	_T("HDMI4"),
	_T("HDMI5"),
	//+add kwmoon 080804
	_T("VAR"),
	_T("E/P"),
	_T("OPT"),
	_T("VOUT"),
	_T("VOUT1"),
	_T("VOUT2"),
	_T("VOUT3"),
	_T("VOUT4"),
	//+add PSH 081204
	_T("DTV"),
	_T("ATV"),
	_T("RGB-DTV"),
	_T("DVI-DTV"),
	_T("DVI-PC"),
	_T("HDMI-PC"),
	_T("DVD"),
	_T("SPDIF"),
	_T("OSD_TEST"),
	_T("SCART1"),
	_T("SCART2"),
	_T("SCART_RGB"),
	_T("COMP1-AV1"),
	_T("COMP1-AV2"),
	_T("COMP2-AV1"),
	_T("COMP2-AV2"),

	NULL
};
         
int LoadFile(CString sPath)
{
    char achBuf[256];
    FILE *fp;     
    int nLineIndex = 0;
    
	CString szErrMsg = _T("");
	fopen_s(&fp, (LPCSTR)sPath, _T("rt"));
	if (fp == NULL)//if ((fp = fopen_s((LPCSTR)sPath, _T("rt"))) == NULL)    
    {
		szErrMsg.Format("Failed to load file.\n[%s]",sPath);
		AfxMessageBox(szErrMsg, MB_ICONSTOP | MB_OK);
		return -1;
    }
/* del 070728 temoporary	
	//+add kwmoon 070727
	if(!feof(fp))
    {
		if(CheckAuthentificationKey(fp) == -1)
		{
			AfxMessageBox("Invalid sequence file!"); return -1;
		}
	}
	//-   
*/
    while(!feof(fp))
    {
        if (fgets(achBuf, 256, fp) == NULL) break;
 
        UserFile[nLineIndex] = (LPSTR)GlobalAllocPtr(GHND, strlen(achBuf) + 1);
        if (UserFile[nLineIndex] == NULL)
        {
            AfxMessageBox(IDS_ERR_MEMORY, MB_ICONSTOP | MB_OK);
            return -1;
        }

        lstrcpy(UserFile[nLineIndex++], (LPSTR)achBuf);
        
        if (nLineIndex > MAXLINE)
        {
            AfxMessageBox(IDS_ERR_SIZE, MB_ICONSTOP | MB_OK);
            break;
        }
    }             
    fclose(fp);           
    return CurrentSet->nTotalLine = nLineIndex;  
}


BOOL GetToken()
{
    char achBuf[256];
   // register cbIndex;     
	int cbIndex;
    char *stop;             
    int i, ErrorFlag = 0;
    float f;

    pToken->type = TNULL;
    
    while(1)
	{
		if (*pWalker == '\n')
		{   
			while(*pWalker == '\n') 
			{                   
				if (++nCurLine < CurrentSet->nTotalLine)
				{
					lstrcpy((LPSTR)achCurString, UserFile[nCurLine]);
					pWalker = achCurString;
				}    
				else 
				{
					pToken->type = FINISHED;
					return TRUE;
				}
			}
		}
                    
		if (*pWalker == '\0')
		{
			pToken->type = FINISHED;
			pToken->value.c = '\0';
			return TRUE;
		}                                    

		if (strchr(_T("{}"), *pWalker))
		{
			pToken->type = BRACE;
			pToken->value.c = *pWalker;
			pWalker++;
			return TRUE;
		}

		if (strchr(_T("()"), *pWalker))
		{
			if (*pWalker == '(') pToken->type = LP;
			else pToken->type = RP;
			pToken->value.c = *pWalker;
			pWalker++;
			return TRUE;
		}

		if (*pWalker == '/' && *(pWalker + 1) == '/')
		{
			pToken->type = COMMENT;
			strcpy(pToken->value.s, pWalker);
			while(*pWalker != '\n') *pWalker++;
			//continue;	//
			return TRUE;
		}
		// temp
		if (*pWalker == '#' && *(pWalker + 1) == '#')
		{
			pToken->type = COMMENT;
			
			char sGroupNo[3];
			memset(sGroupNo,NULL,3);
			strcpy(pToken->value.s, pWalker + 2);

			while(*pWalker != ' ') *pWalker++;
			strncpy(sGroupNo,pWalker,3);
//			g_CategoryNo = atoi(sGroupNo);


			while(*pWalker != '\n') *pWalker++;
			continue;	//return TRUE;
		}
		//-

		if (strchr(_T("+-"), *pWalker))       
		{                                      
			if (*pWalker == '+' && *(pWalker + 1) == '-') 
			{
				pToken->type = SIGN;
				pToken->value.c = '+' + '-';
				pWalker++; pWalker++;
				return TRUE;
			}                        
			else if (*pWalker == '+')
			{
				pToken->type = SIGN;
				pToken->value.c = '+';
				pWalker++;
				return TRUE;
			}             
			pToken->type = SIGN;
			pToken->value.c = '-';
			pWalker++;
			return TRUE;
		}

		if (strchr(_T(",;:'"), *pWalker))
		{            
			if (*pWalker == ',') pToken->type = COMMA;
			else pToken->type = DELIMITER;
			pToken->value.c = *pWalker;
			pWalker++;
			return TRUE;
		}

		if (*pWalker == '"')        
		{
			pWalker++; 
			for(cbIndex = 0; (*pWalker != '"') && (*pWalker != '\n'); cbIndex++)
			{
				achBuf[cbIndex] = *pWalker++;
			}
    
			if (*pWalker == '\n')
			{                                    
				pToken->value.i = IDS_NOT_STRING;           
				strcpy(pToken->value.s, achBuf);  
				return FALSE;
			}
			pWalker++; achBuf[cbIndex] = '\0';
			pToken->type = STRING;
			strcpy(pToken->value.s, achBuf);
			return TRUE;
		}
    
		if (isdigit(*pWalker))
		{ 
			for(cbIndex = 0; !IsDelimiter(*pWalker); cbIndex++)
				achBuf[cbIndex] = *pWalker++;
			achBuf[cbIndex] = '\0';     
    
			if (strchr(achBuf, '.'))
			{
				f = (float)strtod(achBuf, &stop);     
            
				if (stop == NULL)
				{           
					pToken->value.i = IDS_NOT_NUMBER;    
					return FALSE;
				}
				pToken->type = REAL;
				pToken->value.f = f;
				return TRUE;
			}                      

			if (achBuf[1] == 'x')                   
			{                          
				for(cbIndex = 2; achBuf[cbIndex] != '\0'; cbIndex++)  // "0x" skip
				{
					if (!isxdigit(achBuf[cbIndex])) ErrorFlag++;
				}           
				i = (int)strtol(achBuf, &stop, 16);
			}
    
			else if (achBuf[1] == 'b')      
			{
				for(cbIndex = 2; achBuf[cbIndex] != '\0'; cbIndex++)  // "0b" skip
				{
					if (achBuf[cbIndex] < '0' || achBuf[cbIndex] > '1')
						ErrorFlag++;
				}
				i = (int)strtol(achBuf + 2, &stop, 2);   
			}
    
			else 
			{
				for(cbIndex = 0; achBuf[cbIndex] != '\0'; cbIndex++)  
				{
					if (!isdigit(achBuf[cbIndex]))   ErrorFlag++;
				}
				i = (int)strtol(achBuf, &stop, 10);
			}

			if (ErrorFlag)
			{               
				pToken->value.i = IDS_NOT_NUMBER;
				return FALSE;
			}
			pToken->type = INTEGER;
			pToken->value.i = i;
			return TRUE;
		}       
                                        
		// letter, underscore or digit                                          
		if (__iscsymf(*pWalker))  
		{
			for(cbIndex = 0; !IsDelimiter(*pWalker); cbIndex++)
				achBuf[cbIndex] = *pWalker++;
			achBuf[cbIndex] = '\0';
			pToken->type = KEYWORD; 
			strcpy(pToken->value.s, achBuf);
			return TRUE;
		}           
		if (IsWhite(*pWalker) && *pWalker)
		{
			while(IsWhite(*pWalker) && *pWalker) 
			{
				++pWalker;
			}
			continue;
		}
		return FALSE;
	}
    return FALSE;
}

BOOL IsWhite(char c)
{
    if (c == ' ' || c == '\t') return TRUE;
    return FALSE;
}

BOOL IsDelimiter(char c)
{
    if (strchr(_T(" ;:,()"), c) || c == '\t' || c == '\n' || c == '\0')
        return TRUE;
    return FALSE;
}

void SyntexError(int line)
{
    CString s, temp;
                    
    temp.LoadString(pToken->value.i);
              
	s.Format(_T("Syntax Error : %d Line, %dStep\n%s")
		, line + 1, Parsing.nNo, (LPCSTR)temp);
    AfxMessageBox(s);
}

BOOL ProcessTyep_Step()
{
	return TRUE;
}

BOOL Check_AdjustType_Step()
{
	return TRUE;
}


BOOL ParseStep(int nStepCnt)
{

    CString sStepName, sUnit, sCurSource, sNextSource;

	int nStepLine, nStepNo;
	BOOL bNominal;
	double fNominal, fHigh, fLow;
    FuncionType func_type;

	//g_CommantArray = "";

    if (GetToken() == FALSE) return FALSE;
    if (pToken->value.c != '(')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }
    
    if (GetToken() == FALSE) return FALSE;
    if (pToken->type == INTEGER) nStepNo = pToken->value.i;
    else
    {
        pToken->value.i = IDS_TYPE_MISMATCH;
        return FALSE;
    }   
	
	if(nStepNo != nStepCnt)
	{
		//+ 2007.10.3 Mod BY USY
		nStepNo = nStepCnt;
//		pToken->value.i = IDS_STEPNO_ERROR;
//        return FALSE;
		//-
	}

    Parsing.nNo = nStepNo;
    
    if (GetToken() == FALSE) return FALSE;
    if (pToken->value.c != ',')
    {
        pToken->value.i = IDS_MISSING_COMMA;
        return FALSE;
    }
    
    if (GetToken() == FALSE) return FALSE;
    if (pToken->type == STRING) sStepName = pToken->value.s;
    else
    {
        pToken->value.i = IDS_TYPE_MISMATCH;
        return FALSE;
    }

    if (GetToken() == FALSE) return FALSE;
    if (pToken->value.c != ')')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }

    if (GetToken() == FALSE) return FALSE;
    if (pToken->value.c != ':')
    {
        pToken->value.i = IDS_MISSING_COMMA;  
        return FALSE;
    }
  
	if (GetToken() == FALSE) return FALSE;
    if (pToken->type == KEYWORD)
	{
		func_type = FunctionTypeChecking(pToken->value.s);
		if (func_type == FUNCTION_TYPEERROR) 
		{
			pToken->value.i = IDS_UNDEFINED_WORD;
			return FALSE;
		}
    }
	else
    {
        pToken->value.i = IDS_UNDEFINED_WORD;
        return FALSE;
    }

    if (GetToken() == FALSE) return FALSE;
    if (pToken->value.c != '(')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }

	if(func_type != PROCESS_FUNCTION)
	{
		if (GetToken() == FALSE) return FALSE;
		//+ 2007.7.24 Add BY USY
		if (pToken->type == STRING) 
		{
			//+ 2007.10.4 Mod BY USY
			if(CheckSource(pToken->value.s))
			{
				sCurSource = pToken->value.s;
			}
			else 
			{
				pToken->value.i = IDS_UNDEFINED_SOURCE;
				return FALSE;
			}
//			sCurSource = pToken->value.s;
			//-
		}
//		if (pToken->type == STRING) 
//		{
//			sInputSource = pToken->value.s;
//		}
		//-
		else
		{
			pToken->value.i = IDS_UNDEFINED_WORD;
			return FALSE;
		}
//		//+ 2007.7.24 Add BY USY
//		if (GetToken() == FALSE) return FALSE;
//		if (pToken->value.c != ',')
//		{
//			pToken->value.i = IDS_MISSING_COMMA;
//			return FALSE;
//		}
//		//-
//		if (GetToken() == FALSE) return FALSE;
//		if (pToken->type == STRING) 
//		{
//			sNextSource = pToken->value.s;
//		}
//		else
//		{
//			pToken->value.i = IDS_UNDEFINED_WORD;
//			return FALSE;
//		}
//		//-

		if (GetToken() == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
		
		if (GetToken() == FALSE) return FALSE;
		if (pToken->type == KEYWORD)
		{
			if(!MeasFunc_SyntexChecking(pToken->value.s))
				return FALSE;
		}
		else
		{
			pToken->value.i = IDS_UNDEFINED_WORD;
			return FALSE;
		}

		if (GetToken() == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
		
		if(cMeasFuncType == MEAS_BOOL)
		{
			if (GetToken() == FALSE) return FALSE;
			if (pToken->type == KEYWORD)
			{
				if (!strcmp(_strlwr(pToken->value.s), _T("true"))) bNominal = TRUE;
				else if (!strcmp(_strlwr(pToken->value.s), _T("false"))) bNominal = FALSE;
				else
				{
					pToken->value.i = IDS_UNDEFINED_WORD;
					return FALSE;
				}
			}
			else
			{
				pToken->value.i = IDS_UNDEFINED_WORD;
				return FALSE;
			}
		}
		else
		{
			if (GetFloat() == FALSE)
			{
				pToken->value.i = IDS_TYPE_MISMATCH;
				return FALSE;
			}
			else fNominal = pToken->value.f;
		}


		if (GetToken() == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
    
		if(cMeasFuncType != MEAS_BOOL)
		{
			if (GetFloat() == FALSE)
			{
				pToken->value.i = IDS_TYPE_MISMATCH;
				return FALSE;
			}
			else fLow = pToken->value.f;
		}

		if (GetToken() == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
    
		if(cMeasFuncType != MEAS_BOOL)
		{
			if (GetFloat() == FALSE)
			{
				pToken->value.i = IDS_TYPE_MISMATCH;
				return FALSE;
			}
			else fHigh = pToken->value.f;
		}

		if (GetToken() == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
    
		if (GetToken() == FALSE) return FALSE;
		if (pToken->type == STRING) sUnit = pToken->value.s;
		else
		{
			pToken->value.i = IDS_TYPE_MISMATCH;
			return FALSE;
		}
	}
	if (GetToken() == FALSE) return FALSE;
	if (pToken->value.c != ')')
	{
		pToken->value.i = IDS_MISSING_PAREN;
		return FALSE;
	}   
	//if (GetToken() == FALSE) return FALSE;
	//if (pToken->type == COMMENT)
	//{
	//	g_CommantArray += pToken->value.s;		
	//}
	    
    if (FindStep(nStepNo) != NULL)
    {
        pToken->value.i = IDS_REDEFINITION_STEP;
        return FALSE;
    }       
                                 
    nStepLine = nCurLine;   
      
	CStep* pStep = new CStep();	

    if (ParseBlock(pStep) == FALSE) 
	{
		delete pStep;
		return FALSE;
	}
 
	pStep->m_strCommant = g_CommantArray;
	g_CommantArray = "";
	pStep->m_strInCommant = g_CommantFuncArray;
	g_CommantFuncArray = "";
	//Func TEST Comment

	pStep->SetStepNo(nStepNo);
	pStep->SetStepName(sStepName);
	pStep->m_nStepType = func_type; 

	if(func_type != PROCESS_FUNCTION)
	{
		pStep->SetFunction(cMeasFunc);
		pStep->SetFuncType(cMeasFuncType); 
		pStep->SetFuncName(cFuncName);
		//+ 2007.7.24 Mod BY USY
		pStep->SetCurrentSource(sCurSource);
		pStep->SetNextSource(sNextSource);
//		pStep->SetInputSource(sInputSource);
		//-

		//+add kwmoon 071001
		if(cFuncName.Compare("color_test") == 0)			pStep->SetTestType(RGB_TEST);
		else if(cFuncName.Compare("bright_test") == 0)		pStep->SetTestType(BRIGHT_TEST);
		else if(cFuncName.Compare("noise_test") == 0)		pStep->SetTestType(NOISE_TEST);
		else if(cFuncName.Compare("position_test") == 0)	pStep->SetTestType(POSITION_TEST);
		else if(cFuncName.Compare("osd_test") == 0)			pStep->SetTestType(OSD_TEST);
		else if(cFuncName.Compare("size_test") == 0)		pStep->SetTestType(SIZE_TEST);
		else if(cFuncName.Compare("lan_test") == 0)			pStep->SetTestType(LAN_TEST);
		else if(cFuncName.Compare("tvcomm_test") == 0)		pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("signal_line_test") == 0) pStep->SetTestType(SIGNAL_LINE_TEST);
		else if(cFuncName.Compare("audio_test") == 0)		pStep->SetTestType(AUDIO_TEST);
		else if(cFuncName.Compare("moving_pic_test") == 0)	pStep->SetTestType(MOVING_PIC_TEST);
		else if(cFuncName.Compare("manual_test") == 0)		pStep->SetTestType(MANUAL_TEST);
		else if(cFuncName.Compare("caption_test") == 0)		pStep->SetTestType(CAPTION_TEST);
		else if(cFuncName.Compare("digitaleye_test") == 0)	pStep->SetTestType(DIGITAL_EYE_TEST);
		else if(cFuncName.Compare("adc_test") == 0)			pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("adc_result_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("edid_result_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("edid_download") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("color_bias_test") == 0)	pStep->SetTestType(BIAS_TEST);
		else if(cFuncName.Compare("module_voltage_test") == 0)	pStep->SetTestType(MODULE_TEST);
		else if(cFuncName.Compare("version_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("option_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("lan_port_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("esn_download_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("ci_key_check") == 0)	pStep->SetTestType(TV_COMM_TEST);  //100401 add
		else if(cFuncName.Compare("rjp_Port_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("magic_remocon_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("wireless_ready_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("if_test2_m") == 0)	pStep->SetTestType(MULTICHECL_TEST);

		if(cMeasFuncType == MEAS_BOOL)
		{
			pStep->SetNominal(bNominal);

			//+add kwmoon 080724
			fHigh = 0.0f;
			fLow  = 0.0f;
		}
		else	
		{
			pStep->SetNominal(fNominal);
		}
		pStep->SetHighLimit(fHigh);
		pStep->SetLowLimit(fLow);
		pStep->SetUnit(sUnit);
	}

	//+add kwmoon 080804
	GetAudioValue(pStep);
	
	StepList.AddTail(pStep);

    sMsg.Format(_T("Step(%2d) Function : %s"), nStepNo, sStepName);
    
    return TRUE;
}


BOOL ParseBlock(CStep* pStep)
{
    char achFuncName[31];
    BOOL bFindFlag = FALSE;
	g_CommantFuncArray = "";
    
    if (GetToken() == FALSE) return FALSE;
 
	if (pToken->value.c != '{')
    {
        pToken->value.i = IDS_MISSING_BRACE;
        return FALSE;
    }          

    pToken->type = TNULL;   

	for(; (pToken->type != FINISHED) && (pToken->type != BRACE);)
    {
        if (GetToken() == FALSE) return FALSE;
                                     
        if (pToken->type == KEYWORD)
        {   
            strcpy(achFuncName, pToken->value.s);
            if (SyntexChecking(achFuncName) == FALSE) 
				return FALSE;

			MakeFunctionList(pStep, achFuncName);
        }               
		else if (pToken->type == COMMENT)
		{
			g_CommantFuncArray += pToken->value.s;
			continue;
		}
        else if (pToken->type == BRACE && pToken->value.c == '}')
            continue;
        else 
        {
            pToken->value.i = IDS_UNDEFINED_SYNTEX;
            return FALSE;                  
        }
    }      
	
	//+del kwmoon 080804
	//+ 2007.10.4 Add BY USY
//	GetAudioValue(pStep);
	//-
    return TRUE;
}
        
BOOL  MeasFunc_SyntexChecking(char* achFunc)
{
    int nIndex;
    
    for(nIndex = 0; MeasDefined[nIndex].pszFunc != NULL; nIndex++)
    {                             
        if (!strcmp(MeasDefined[nIndex].pszFunc, _strlwr(achFunc)))
        {       
            cMeasFunc = MeasDefined[nIndex].pFunc;     
			cMeasFuncType = MeasDefined[nIndex].FuncType; 
			cFuncName = MeasDefined[nIndex].pszFunc;
            return TRUE;
        }
    }                        
    pToken->value.i = IDS_UNDEFINED_WORD;
    return FALSE;
}


BOOL Name_SyntexChecking(char* achName)
{
    int nIndex;
    
    for(nIndex = 0; NameDefined[nIndex].pszName != NULL; nIndex++)
    {                             
        if (!strcmp(NameDefined[nIndex].pszName, _strlwr(achName)))
        {       
            pToken->type = NameDefined[nIndex].nType;     
			pToken->value.i = NameDefined[nIndex].nValue; 
            return TRUE;
        }
    }                        
    pToken->value.i = IDS_UNDEFINED_WORD;
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////

BOOL  SyntexChecking(char* achFunc)
{
    int nIndex, cbSyntex;
    BOOL bFindFlag;
	char sign;
	int nvalue;
	float fvalue;

    
	//+ 2007.10.4 Add BY USY
	int	nCheck = 0;	// 0 : no check, 1 : set rf channel check, 2 : set input source check
	//-

    for(nIndex = 0; Predefined[nIndex].pszFunc != NULL; nIndex++)
    {                             
         if (!strcmp(Predefined[nIndex].pszFunc, _strlwr(achFunc)))
        {       
            bFindFlag = TRUE;
            cbSyntex = 0;    
			//+ 2007.10.4 Add BY USY
			if(strcmp(Predefined[nIndex].pszFunc, _T("set_rf_channel")) == 0)
			{
				nCheck = 1;
			}
			else if(strcmp(Predefined[nIndex].pszFunc, _T("set_input_source")) == 0)
			{
				nCheck = 2;
			}
			//-
                    
            while(Predefined[nIndex].achPara[cbSyntex] != TNULL)                  
            {
                if (GetToken() == FALSE) return FALSE;
				if (pToken->type == KEYWORD)
				{
					if(!Name_SyntexChecking(pToken->value.s))
					{
                            cbSyntex++; // ')' skip
                            continue;
					}
				}
                if (pToken->type != Predefined[nIndex].achPara[cbSyntex])
                {
                    if (Predefined[nIndex].achPara[cbSyntex] == SIGN)
                    {
                        if (Predefined[nIndex].achPara[cbSyntex + 1] 
                            == pToken->type)
                        {
                            cbSyntex++; cbSyntex++;
                            continue;
                        }            
                        if (Predefined[nIndex].achPara[cbSyntex + 1] == REAL 
                            && pToken->type == INTEGER)
                        {                           
                            cbSyntex++; cbSyntex++;
                            continue;
                        }
                        pToken->value.i = IDS_TYPE_MISMATCH;
                        return FALSE;
                    }
                    else if (Predefined[nIndex].achPara[cbSyntex] == REAL 
						&& pToken->type == INTEGER)
                    {                           
                        cbSyntex++;
                        continue;
                    }        
					else if (Predefined[nIndex].achPara[cbSyntex] == REAL
						&& pToken->type == SIGN)	
					{
						sign = pToken->value.c;
						if (GetToken() == FALSE) return FALSE;
						if (pToken->type != REAL)
						{
							if (pToken->type != INTEGER)
							{
								pToken->value.i = IDS_TYPE_MISMATCH;
								return FALSE;
							}
						}
						fvalue = pToken->value.f;
						if (sign == '-') pToken->value.f = (float)-fvalue;

						cbSyntex++;
						continue;
					}
					else if (Predefined[nIndex].achPara[cbSyntex] == INTEGER
						&& pToken->type == SIGN)	
					{
						sign = pToken->value.c;
						if (GetToken() == FALSE) return FALSE;

						if (pToken->type != INTEGER)
						{
							if (pToken->type != REAL)
							{
								pToken->value.i = IDS_TYPE_MISMATCH;
								return FALSE;
							}
						}
						nvalue = pToken->value.i;
						if (sign == '-') pToken->value.i = (int)-nvalue;

						cbSyntex++;
						continue;
					}
                    else if (Predefined[nIndex].achPara[cbSyntex] == INTARRAY)
                    {
                        if (pToken->type != INTEGER) 
                        {
                            pToken->value.i = IDS_TYPE_MISMATCH;
                            return FALSE;
                        }
                        if (AnalyzeIntArray() == FALSE)   
                        {
                            pToken->value.i = IDS_TYPE_MISMATCH;
                            return FALSE;
                        }
                        else 
                        {
                            cbSyntex++; // INTARRAY skip
                            cbSyntex++; // ')' skip
                            continue;
                        }
                    }                    
                    pToken->value.i = IDS_TYPE_MISMATCH;
                    return FALSE;
                }
				//+ 2007.10.4 Add BY USY
				if(nCheck > 0)
				{
					if((nCheck == 1)&&(cbSyntex == 5)&&(pToken->type == STRING))
					{
						//+ 2007.10.8 Mod BY USY
						if(pToken->value.s == NULL)
						{
							pToken->value.i = IDS_STRING_NULL_RFCHANNEL;
							nCheck = 0;
							return FALSE;
						}
//						pToken->value.i = IDS_STRING_NULL_RFCHANNEL;
//						nCheck = 0;
//						return FALSE;
						//-
					}
					else if((nCheck == 2)&&(cbSyntex == 9)&&(pToken->type == STRING))
					{
						//+ 2007.10.8 Mod BY USY
						if(pToken->value.s == NULL)
						{
							pToken->value.i = IDS_STRING_NULL_INPUTSOURCE;
							nCheck = 0;
							return FALSE;
						}
//						pToken->value.i = IDS_STRING_NULL_INPUTSOURCE;
//						nCheck = 0;
//						return FALSE;
						//-
					}
				}
				//-
                cbSyntex++;
            }     
            return TRUE;
        }
    }                        
    pToken->value.i = IDS_UNDEFINED_WORD;
    return FALSE;
}

HEADERTYPE HeaderChecking(char* pszString)
{                    
    if (!strcmp(_strlwr(pszString), _T("step"))) return STEPHEADER;
	//+add PSH 080410
    else if (!strcmp(_strlwr(pszString), _T("adc_adjtype"))) return ADC_TYPE;
	//-
	else 
		return HEADERERROR;
}               
 
FuncionType FunctionTypeChecking(char* pszString)
{
    if (!strcmp(_strlwr(pszString), _T("process"))) return PROCESS_FUNCTION;
    else if (!strcmp(_strlwr(pszString), _T("check"))) return CHECK_FUNCTION;
    else if (!strcmp(_strlwr(pszString), _T("adjust"))) return ADJUST_FUNCTION;
    else if (!strcmp(_strlwr(pszString), _T("manual_adjust"))) return MANUAL_ADJUST_FUNCTION;
	else 
		return FUNCTION_TYPEERROR;

}

//+add PSH 080410
BOOL GetADC_AdjType()
{
	int nAdcAdjType;

    if (GetToken() == FALSE) return FALSE;
    if (pToken->value.c != '(')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }
    
    if (GetToken() == FALSE) return FALSE;
    if (pToken->type == INTEGER) nAdcAdjType = pToken->value.i;
	else if(pToken->type == KEYWORD)
	{
		if(Name_SyntexChecking(pToken->value.s))
		{
			if (pToken->type == INTEGER) 
				nAdcAdjType = pToken->value.i;
			else
				return FALSE;
		}
	}
    else
    {
        pToken->value.i = IDS_TYPE_MISMATCH;
        return FALSE;
    }
    if (GetToken() == FALSE) return FALSE;
    if (pToken->value.c != ')')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }
	if(nAdcAdjType == 0 || nAdcAdjType == 1)
	{
		CurrentSet->nADCAdjType = nAdcAdjType;
	}
	else
		return FALSE;

	return TRUE;
}
//-

BOOL Prescan(CString sPath)
{
    long lCompileTime;
    int nLineIndex = 0;
    int nPercentDrawn = 0;    
	HEADERTYPE	header;
	BOOL bCancel = FALSE;
	int nStepCnt = 1;
//	MSG msg;

	pToken = new Token;

    lCompileTime = GetModifyTime(sPath);

	//+add kwmoon 070930
	if(lCompileTime == -1) return FALSE;

	CurrentSet->lTime = lCompileTime;

    if ((lCompileTime == CurrentSet->lTime) && (bCompiled == TRUE))
    {
        AfxMessageBox(IDS_INFO_UPDATED, MB_ICONINFORMATION | MB_OK);
        return TRUE;
    }
    else
    {                   
        CurrentSet->lTime = lCompileTime;
	    if (LoadFile(sPath) == -1) return FALSE;
    }           
                 
	CurrentSet->nADCAdjType = 0;  //add PSH 20080410
    DeleteStepList();
    bCancel = FALSE;            // Intialize cancel flag
    nCurLine = 0;               // Intialize line count
		
    lstrcpy((LPSTR)achCurString, UserFile[nCurLine]);
    pWalker = achCurString;

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetRange(1, CurrentSet->nTotalLine);

	//+change kwmoon 071206
//	StepSize = 0;
	g_nStepSize = 0;
	g_CommantArray = "";

    while((!bCancel) && (nCurLine < CurrentSet->nTotalLine))
    {
		if(dlg.CheckCancelButton())
		{
			CString s;
			s.LoadString(IDS_QUERY_ABORT);
			if(AfxMessageBox(s, MB_YESNO)==IDYES)
			{	
				bCancel = TRUE;
				continue;
			}
		}
		
		// iteration of the processing
		dlg.SetPos(nCurLine + 1);

        if (GetToken() == FALSE) 
			goto SYNTEXERROR;
        else if (pToken->type == KEYWORD)
        {
			header = HeaderChecking(pToken->value.s);
			if (header == STEPHEADER) 
			{
				if (ParseStep(nStepCnt) == FALSE) 
					goto SYNTEXERROR;
				dlg.SetStatus((LPCSTR)sMsg);
				
				//+change kwmoon 071206
			//	StepSize++;
				g_nStepSize++;

				nStepCnt++;
			}
			//+add PSH 080410
			else if(header == ADC_TYPE)
			{
				if (GetADC_AdjType() == FALSE) 
					goto SYNTEXERROR;
			} //-
			else
			{
				pToken->value.i = IDS_UNDEFINED_WORD;
				goto SYNTEXERROR;
			}
        }
		else if (pToken->type == COMMENT)
		{
			//pToken->value.s;
			g_CommantArray += pToken->value.s;
			continue;
		}
        else if (pToken->type == FINISHED) break;
        else if (pToken->type == END_OF_FILE) break;
        else goto SYNTEXERROR;
    }

	FreeUserFile();
    if (bCancel == TRUE) return FALSE;
	if ((nCurLine == CurrentSet->nTotalLine) || (pToken->type == FINISHED))
	{
		delete pToken;
        return TRUE;
	}
	delete pToken;
    return FALSE;
	

SYNTEXERROR : 
	dlg.DestroyWindow();
	SyntexError(nCurLine);    
	FreeUserFile();
    delete pToken;
	DeleteStepList();
    return FALSE;
	
}

//+ 2007.10.2 Add BY USY
BOOL OpenFullSeq(CString sPath)
{
//	long lCompileTime;
    int nLineIndex = 0;
    int nPercentDrawn = 0;    
	HEADERTYPE	header;
	BOOL bCancel = FALSE;
	int nStepCnt = 1;
	int nTotalLine = -1;

	pToken = new Token;

//    lCompileTime = GetModifyTime(sPath);

	//+add kwmoon 070930
//	if(lCompileTime == -1) return FALSE;

//	CurrentSet->lTime = lCompileTime;

//    if ((lCompileTime == CurrentSet->lTime) && (bCompiled == TRUE))
//    {
//        AfxMessageBox(IDS_INFO_UPDATED, MB_ICONINFORMATION | MB_OK);
//        return TRUE;
//    }
//    else
//    {                   
//        CurrentSet->lTime = lCompileTime;
	    if ((nTotalLine = LoadFullSeq(sPath)) == -1) return FALSE;
//    }           
                       
    DeleteFullSeq();
    bCancel = FALSE;            // Intialize cancel flag
    nCurLine = 0;               // Intialize line count
    
    lstrcpy((LPSTR)achCurString, FullSeqFile[nCurLine]);
    pWalker = achCurString;

//	CProgressDlg dlg;
//	dlg.Create();
//	dlg.SetRange(1, nTotalLine);

	//+change kwmoon 071206
//	StepSize = 0;
	g_nStepSize = 0;

    while((!bCancel) && (nCurLine < nTotalLine))
    {
//		if(dlg.CheckCancelButton())
//		{
//			CString s;
//			s.LoadString(IDS_QUERY_ABORT);
//			if(AfxMessageBox(s, MB_YESNO)==IDYES)
//			{	
//				bCancel = TRUE;
//				continue;
//			}
//		}
//		
//		// iteration of the processing
//		dlg.SetPos(nCurLine + 1);

        if (GetTokenFullSeq(nTotalLine) == FALSE) 
			goto SYNTEXERROR;
        else if (pToken->type == KEYWORD)
        {
			header = HeaderChecking(pToken->value.s);
			if (header == STEPHEADER) 
			{
				
				if (ParseFullSeqStep(nStepCnt, nTotalLine) == FALSE) 
					goto SYNTEXERROR;
//				dlg.SetStatus((LPCSTR)sMsg);

				//+change kwmoon 071206
				//	StepSize++;
				g_nStepSize++;

				nStepCnt++;
			}
			//+add PSH 080410
			else if(header == ADC_TYPE)
			{
				if (GetADC_AdjType() == FALSE) 
					goto SYNTEXERROR;
			} //-
			else
			{
				pToken->value.i = IDS_UNDEFINED_WORD;
				goto SYNTEXERROR;
			}
        }
		else if (pToken->type == COMMENT)
		{
		//	g_CommantArray = pToken->value.s;
			continue;
		}
        else if (pToken->type == FINISHED) break;
        else if (pToken->type == END_OF_FILE) break;
        else
			goto SYNTEXERROR;
    }
//	FreeUserFile();
	FreeUserFile(nTotalLine);
//    if (bCancel == TRUE) return FALSE;
	if ((nCurLine == nTotalLine) || (pToken->type == FINISHED))
	{
		delete pToken;
        return TRUE;
	}
	delete pToken;
    return FALSE;
	

SYNTEXERROR : 
	SyntexError(nCurLine);    
	FreeUserFile(nTotalLine);
    delete pToken;
	DeleteFullSeq();
    return FALSE;
}

int LoadFullSeq(CString sPath)
{
    char achBuf[256];
    FILE *fp;     
    int nLineIndex = 0;
    
	//+add kwmoon 070930
	CString szErrMsg = _T("");

    if ((fp = fopen((LPCSTR)sPath, _T("rt"))) == NULL)
    {
		//+change kwmoon 070930
	//	AfxMessageBox(IDS_ERR_FILE, MB_ICONSTOP | MB_OK);
		szErrMsg.Format("Failed to load Template Sequence.\n[%s]",sPath);
		AfxMessageBox(szErrMsg, MB_ICONSTOP | MB_OK);
		return -1;
    }
/* del 070728 temoporary	
	//+add kwmoon 070727
	if(!feof(fp))
    {
		if(CheckAuthentificationKey(fp) == -1)
		{
			AfxMessageBox("Invalid sequence file!"); return -1;
		}
	}
	//-   
*/
    while(!feof(fp))
    {
        if (fgets(achBuf, 256, fp) == NULL) break;
 
        FullSeqFile[nLineIndex] = (LPSTR)GlobalAllocPtr(GHND, strlen(achBuf) + 1);
        if (FullSeqFile[nLineIndex] == NULL)
        {
            AfxMessageBox(IDS_ERR_MEMORY, MB_ICONSTOP | MB_OK);
            return -1;
        }

        lstrcpy(FullSeqFile[nLineIndex++], (LPSTR)achBuf);
        
        if (nLineIndex > MAXLINE)
        {
            AfxMessageBox(IDS_ERR_SIZE, MB_ICONSTOP | MB_OK);
            break;
        }
    }             
    fclose(fp);           
    return nLineIndex;  
}
void DeleteFullSeq()
{
	POSITION pos = FullSeq.GetHeadPosition();
	while(pos != NULL)
    {       
        delete FullSeq.GetNext(pos);
    }
    FullSeq.RemoveAll();
}

void FreeUserFile(int nTotalLine)
{
    for(int n = 0; n < nTotalLine; n++)
        GlobalFreePtr(FullSeqFile[n]);
}

CStep* FindFullSeqStep(int nStepNo)
{
	POSITION pos = FullSeq.GetHeadPosition();
    while(pos != NULL)
	{
		CStep* temp = FullSeq.GetNext(pos);
        if (temp->GetStepNo() == nStepNo) return temp;
    }
    return NULL;  
}
BOOL ParseFullSeqStep(int nStepCnt, int nTotalLine)
{

    CString sStepName, sUnit, sCurSource, sNextSource;

	int nStepLine, nStepNo;
	BOOL bNominal;
	double fNominal = 0, fHigh = 0, fLow = 0;
    FuncionType func_type;

    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->value.c != '(')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }
    
    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->type == INTEGER) nStepNo = pToken->value.i;
    else
    {
        pToken->value.i = IDS_TYPE_MISMATCH;
        return FALSE;
    }   
	
	if(nStepNo != nStepCnt)
	{
		//+change kwmoon 080805
		nStepNo = nStepCnt;
	//	pToken->value.i = IDS_STEPNO_ERROR;
    //  return FALSE;

	}

    Parsing.nNo = nStepNo;
    
    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->value.c != ',')
    {
        pToken->value.i = IDS_MISSING_COMMA;
        return FALSE;
    }
    
    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->type == STRING) sStepName = pToken->value.s;
    else
    {
        pToken->value.i = IDS_TYPE_MISMATCH;
        return FALSE;
    }

    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->value.c != ')')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }

    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->value.c != ':')
    {
        pToken->value.i = IDS_MISSING_COMMA;  
        return FALSE;
    }
  
	if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->type == KEYWORD)
	{
		func_type = FunctionTypeChecking(pToken->value.s);
		if (func_type == FUNCTION_TYPEERROR) 
		{
			pToken->value.i = IDS_UNDEFINED_WORD;
			return FALSE;
		}
    }
	else
    {
        pToken->value.i = IDS_UNDEFINED_WORD;
        return FALSE;
    }

    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
    if (pToken->value.c != '(')
    {
        pToken->value.i = IDS_MISSING_PAREN;
        return FALSE;
    }

	if(func_type != PROCESS_FUNCTION)
	{
		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		//+ 2007.7.24 Add BY USY
		if (pToken->type == STRING) 
		{
			sCurSource = pToken->value.s;
		}
//		if (pToken->type == STRING) 
//		{
//			sInputSource = pToken->value.s;
//		}
		//-
		else
		{
			pToken->value.i = IDS_UNDEFINED_WORD;
			return FALSE;
		}
//		//+ 2007.7.24 Add BY USY
//		if (GetTokenFullSeq(nTotalLine)() == FALSE) return FALSE;
//		if (pToken->value.c != ',')
//		{
//			pToken->value.i = IDS_MISSING_COMMA;
//			return FALSE;
//		}
//		//-
//		if (GetTokenFullSeq(nTotalLine)() == FALSE) return FALSE;
//		if (pToken->type == STRING) 
//		{
//			sNextSource = pToken->value.s;
//		}
//		else
//		{
//			pToken->value.i = IDS_UNDEFINED_WORD;
//			return FALSE;
//		}
//		//-

		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
		
		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		if (pToken->type == KEYWORD)
		{
			if(!MeasFunc_SyntexChecking(pToken->value.s))
				return FALSE;
		}
		else
		{
			pToken->value.i = IDS_UNDEFINED_WORD;
			return FALSE;
		}

		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
		
		if(cMeasFuncType == MEAS_BOOL)
		{
			if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
			if (pToken->type == KEYWORD)
			{
				if (!strcmp(_strlwr(pToken->value.s), _T("true"))) bNominal = TRUE;
				else if (!strcmp(_strlwr(pToken->value.s), _T("false"))) bNominal = FALSE;
				else
				{
					pToken->value.i = IDS_UNDEFINED_WORD;
					return FALSE;
				}
			}
			else
			{
				pToken->value.i = IDS_UNDEFINED_WORD;
				return FALSE;
			}
		}
		else
		{
			if (GetFloat() == FALSE)
			{
				pToken->value.i = IDS_TYPE_MISMATCH;
				return FALSE;
			}
			else fNominal = pToken->value.f;
		}


		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
    
		if(cMeasFuncType != MEAS_BOOL)
		{
			if (GetFloat() == FALSE)
			{
				pToken->value.i = IDS_TYPE_MISMATCH;
				return FALSE;
			}
			else fLow = pToken->value.f;
		}

		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
    
		if(cMeasFuncType != MEAS_BOOL)
		{
			if (GetFloat() == FALSE)
			{
				pToken->value.i = IDS_TYPE_MISMATCH;
				return FALSE;
			}
			else fHigh = pToken->value.f;
		}

		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		if (pToken->value.c != ',')
		{
			pToken->value.i = IDS_MISSING_COMMA;
			return FALSE;
		}
    
		if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
		if (pToken->type == STRING) sUnit = pToken->value.s;
		else
		{
			pToken->value.i = IDS_TYPE_MISMATCH;
			return FALSE;
		}
	}
	if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
	if (pToken->value.c != ')')
	{
		pToken->value.i = IDS_MISSING_PAREN;
		return FALSE;
	}   
 //	if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
	//if (pToken->type = COMMENT)
	//{
	////	g_CommantFuncArray += pToken->value.s ;
	//	return FALSE;
	//}      
    if (FindFullSeqStep(nStepNo) != NULL)
    {
        pToken->value.i = IDS_REDEFINITION_STEP;
        return FALSE;
    }       
                                 
    nStepLine = nCurLine;   
      
	CStep* pStep = new CStep();	
    if (ParseFullSeqBlock(pStep, nTotalLine) == FALSE) 
	{
		delete pStep;
		return FALSE;
	}

 
	//pStep->m_strCommant = g_CommantArray;
	//g_CommantArray = "";
	pStep->SetStepNo(nStepNo);
	pStep->SetStepName(sStepName);
	pStep->m_nStepType = func_type; 

	if(func_type != PROCESS_FUNCTION)
	{
		pStep->SetFunction(cMeasFunc);
		pStep->SetFuncType(cMeasFuncType); 
		pStep->SetFuncName(cFuncName);
		//+ 2007.7.24 Mod BY USY
		pStep->SetCurrentSource(sCurSource);
		pStep->SetNextSource(sNextSource);
//		pStep->SetInputSource(sInputSource);
		//-

		//+add kwmoon 071001
		if(cFuncName.Compare("color_test") == 0)			pStep->SetTestType(RGB_TEST);
		else if(cFuncName.Compare("bright_test") == 0)		pStep->SetTestType(BRIGHT_TEST);
		else if(cFuncName.Compare("noise_test") == 0)		pStep->SetTestType(NOISE_TEST);
		else if(cFuncName.Compare("position_test") == 0)	pStep->SetTestType(POSITION_TEST);
		else if(cFuncName.Compare("osd_test") == 0)			pStep->SetTestType(OSD_TEST);
		else if(cFuncName.Compare("size_test") == 0)		pStep->SetTestType(SIZE_TEST);
		else if(cFuncName.Compare("lan_test") == 0)			pStep->SetTestType(LAN_TEST);
		else if(cFuncName.Compare("tvcomm_test") == 0)		pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("signal_line_test") == 0) pStep->SetTestType(SIGNAL_LINE_TEST);
		else if(cFuncName.Compare("audio_test") == 0)		pStep->SetTestType(AUDIO_TEST);
		else if(cFuncName.Compare("moving_pic_test") == 0)	pStep->SetTestType(MOVING_PIC_TEST);
		else if(cFuncName.Compare("manual_test") == 0)		pStep->SetTestType(MANUAL_TEST);
		//+add kwmoon 071108
		else if(cFuncName.Compare("caption_test") == 0)		pStep->SetTestType(CAPTION_TEST);
		//+add kwmoon 071109
		else if(cFuncName.Compare("digitaleye_test") == 0)	pStep->SetTestType(DIGITAL_EYE_TEST);
		
		if(cMeasFuncType == MEAS_BOOL)
			pStep->SetNominal(bNominal);
		else	
			pStep->SetNominal(fNominal);
		
		pStep->SetHighLimit(fHigh);
		pStep->SetLowLimit(fLow);
		pStep->SetUnit(sUnit);
	}

	//+add kwmoon 080804
	GetAudioValue(pStep);

	FullSeq.AddTail(pStep);

    sMsg.Format(_T("Step(%2d) Function : %s"), nStepNo, sStepName);
    
    return TRUE;
}
BOOL GetTokenFullSeq(int nTotalLine)
{
    char achBuf[256];
   // register cbIndex;     
	int cbIndex;
    char *stop;             
    int i, ErrorFlag = 0;
    float f;

    pToken->type = TNULL;
    
    while(1)
	{
		if (*pWalker == '\n')
		{   
			while(*pWalker == '\n') 
			{                   
				if (++nCurLine < nTotalLine)
				{
					lstrcpy((LPSTR)achCurString, FullSeqFile[nCurLine]);
					pWalker = achCurString;
				}    
				else 
				{
					pToken->type = FINISHED;
					return TRUE;
				}
			}
		}
                    
		if (*pWalker == '\0')
		{
			pToken->type = FINISHED;
			pToken->value.c = '\0';
			return TRUE;
		}                                    

		if (strchr(_T("{}"), *pWalker))
		{
			pToken->type = BRACE;
			pToken->value.c = *pWalker;
			pWalker++;
			return TRUE;
		}

		if (strchr(_T("()"), *pWalker))
		{
			if (*pWalker == '(') pToken->type = LP;
			else pToken->type = RP;
			pToken->value.c = *pWalker;
			pWalker++;
			return TRUE;
		}

		if (*pWalker == '/' && *(pWalker + 1) == '/')
		{
			pToken->type = COMMENT;
			strcpy(pToken->value.s, pWalker );
			while(*pWalker != '\n') *pWalker++;
			//continue;	//
			return TRUE;
		}
		
		// temp
		if (*pWalker == '#' && *(pWalker + 1) == '#')
		{
			pToken->type = COMMENT;
			
			char sGroupNo[3];
			memset(sGroupNo,NULL,3);
			strcpy(pToken->value.s, pWalker + 2);

			while(*pWalker != ' ') *pWalker++;
			strncpy(sGroupNo,pWalker,3);
//			g_CategoryNo = atoi(sGroupNo);


			while(*pWalker != '\n') *pWalker++;
			continue;	//return TRUE;
		}
		//-

		if (strchr(_T("+-"), *pWalker))       
		{                                      
			if (*pWalker == '+' && *(pWalker + 1) == '-') 
			{
				pToken->type = SIGN;
				pToken->value.c = '+' + '-';
				pWalker++; pWalker++;
				return TRUE;
			}                        
			else if (*pWalker == '+')
			{
				pToken->type = SIGN;
				pToken->value.c = '+';
				pWalker++;
				return TRUE;
			}             
			pToken->type = SIGN;
			pToken->value.c = '-';
			pWalker++;
			return TRUE;
		}

		if (strchr(_T(",;:'"), *pWalker))
		{            
			if (*pWalker == ',') pToken->type = COMMA;
			else pToken->type = DELIMITER;
			pToken->value.c = *pWalker;
			pWalker++;
			return TRUE;
		}

		if (*pWalker == '"')        
		{
			pWalker++; 
			for(cbIndex = 0; (*pWalker != '"') && (*pWalker != '\n'); cbIndex++)
			{
				achBuf[cbIndex] = *pWalker++;
			}
    
			if (*pWalker == '\n')
			{                                    
				pToken->value.i = IDS_NOT_STRING;           
				strcpy(pToken->value.s, achBuf);  
				return FALSE;
			}
			pWalker++; achBuf[cbIndex] = '\0';
			pToken->type = STRING;
			strcpy(pToken->value.s, achBuf);
			return TRUE;
		}
    
		if (isdigit(*pWalker))
		{ 
			for(cbIndex = 0; !IsDelimiter(*pWalker); cbIndex++)
				achBuf[cbIndex] = *pWalker++;
			achBuf[cbIndex] = '\0';     
    
			if (strchr(achBuf, '.'))
			{
				f = (float)strtod(achBuf, &stop);     
            
				if (stop == NULL)
				{           
					pToken->value.i = IDS_NOT_NUMBER;    
					return FALSE;
				}
				pToken->type = REAL;
				pToken->value.f = f;
				return TRUE;
			}                      

			if (achBuf[1] == 'x')                   
			{                          
				for(cbIndex = 2; achBuf[cbIndex] != '\0'; cbIndex++)  // "0x" skip
				{
					if (!isxdigit(achBuf[cbIndex])) ErrorFlag++;
				}           
				i = (int)strtol(achBuf, &stop, 16);
			}
    
			else if (achBuf[1] == 'b')      
			{
				for(cbIndex = 2; achBuf[cbIndex] != '\0'; cbIndex++)  // "0b" skip
				{
					if (achBuf[cbIndex] < '0' || achBuf[cbIndex] > '1')
						ErrorFlag++;
				}
				i = (int)strtol(achBuf + 2, &stop, 2);   
			}
    
			else 
			{
				for(cbIndex = 0; achBuf[cbIndex] != '\0'; cbIndex++)  
				{
					if (!isdigit(achBuf[cbIndex]))   ErrorFlag++;
				}
				i = (int)strtol(achBuf, &stop, 10);
			}

			if (ErrorFlag)
			{               
				pToken->value.i = IDS_NOT_NUMBER;
				return FALSE;
			}
			pToken->type = INTEGER;
			pToken->value.i = i;
			return TRUE;
		}       
                                        
		// letter, underscore or digit                                          
		if (__iscsymf(*pWalker))  
		{
			for(cbIndex = 0; !IsDelimiter(*pWalker); cbIndex++)
				achBuf[cbIndex] = *pWalker++;
			achBuf[cbIndex] = '\0';
			pToken->type = KEYWORD; 
			strcpy(pToken->value.s, achBuf);
			return TRUE;
		}           
		if (IsWhite(*pWalker) && *pWalker)
		{
			while(IsWhite(*pWalker) && *pWalker) 
			{
				++pWalker;
			}
			continue;
		}
		return FALSE;
	}
    return FALSE;
}

BOOL ParseFullSeqBlock(CStep* pStep, int nTotalLine)
{
    char achFuncName[31];
    BOOL bFindFlag = FALSE;
    
    if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
 
	if (pToken->value.c != '{')
    {
        pToken->value.i = IDS_MISSING_BRACE;
        return FALSE;
    }          

    pToken->type = TNULL;   

	for(; (pToken->type != FINISHED) && (pToken->type != BRACE);)
    {
        if (GetTokenFullSeq(nTotalLine) == FALSE) return FALSE;
                                     
        if (pToken->type == KEYWORD)
        {   
            strcpy(achFuncName, pToken->value.s);
            if (SyntexChecking(achFuncName) == FALSE) return FALSE;

			MakeFunctionList(pStep, achFuncName);
			
        }               
		else if (pToken->type == COMMENT)
		{
			g_CommantFuncArray += pToken->value.s;
			continue;
		}
        else if (pToken->type == BRACE && pToken->value.c == '}')
            continue;
        else 
        {
            pToken->value.i = IDS_UNDEFINED_SYNTEX;
            return FALSE;                  
        }
    } 

    return TRUE;
}


//+change kwmoon 080804
//+ 2007.10.4 Add BY USY
void GetAudioValue(CStep* pStep)
{
	int nCheckAudioNo	= -1;
	int nCheckAudioNo_Level	= -1;
	int nCheckAudioNo_Freq	= -1;
	int nMeasureAudioNo = -1;

	BOOL bAudioTest = FALSE;

	for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
	{
		if(strcmp(Predefined[nTmp].pszFunc,"check_audio") == NULL)
		{
			nCheckAudioNo = nTmp;
		}

		if(strcmp(Predefined[nTmp].pszFunc,"measure_audio") == NULL)
		{
			nMeasureAudioNo = nTmp;
		}

		if(strcmp(Predefined[nTmp].pszFunc,"checkaudio_level") == NULL)
		{
			nCheckAudioNo_Level = nTmp;
		}
	
		if(strcmp(Predefined[nTmp].pszFunc,"checkaudio_freq") == NULL)
		{
			nCheckAudioNo_Freq = nTmp;
		}
	}


	POSITION Pos = pStep->m_InstructionList.GetHeadPosition();
	
	while(Pos != NULL)
	{	
		CFunction* pFunction = pStep->m_InstructionList.GetNext(Pos);
	
		//==============
		// CheckAudio
		//==============
		if(pFunction->m_pFunc == Predefined[nCheckAudioNo].pFunc)
		{
			bAudioTest = TRUE;
			
			pStep->m_bRunAudioTest = TRUE;
			pStep->m_nMeasureAudioType = FREQ_LEVEL_CHECK;
			POSITION ArgPos = pFunction->m_ArgumentList.GetHeadPosition();
			
			int nTmp = 0;
			
			while(nTmp < 6)
			{
				Argument* pArgument = pFunction->m_ArgumentList.GetNext(ArgPos);
			
				if(nTmp == 0)
				{
					pStep->m_nAudioSource = pArgument->Integer;	 
				}
				else if(nTmp == 1)
				{
					pStep->m_nLeftFreq[0] =  pArgument->Integer;
				}
				else if(nTmp == 2)
				{
					pStep->m_nRightFreq[0] =  pArgument->Integer;
				}
				else if(nTmp == 3)
				{
					pStep->m_nLeftLevel[0] =  pArgument->Integer;
				}
				else if(nTmp == 4)
				{
					pStep->m_nRightLevel[0] =  pArgument->Integer;
				}
				else if(nTmp == 5)
				{
					pStep->m_nAudioMargin = pArgument->Integer;
				}
				nTmp++;
			}
		}
		//==============
		// MeasureAudio
		//==============
		if(pFunction->m_pFunc == Predefined[nMeasureAudioNo].pFunc)
		{
			bAudioTest = TRUE;
			
			pStep->m_bRunAudioTest = TRUE;
			pStep->m_nMeasureAudioType = FREQ_LEVEL_CHECK;

			POSITION ArgPos = pFunction->m_ArgumentList.GetHeadPosition();
			
			Argument* pArgument = pFunction->m_ArgumentList.GetNext(ArgPos);
			
			pStep->m_nAudioSource	= pArgument->Integer;	 

			int  nInputSource = 0;
			BOOL bFindInputSource = FALSE;

			CString szInputSource = pStep->GetCurrentSource();

			for(int i = 0; InputDefined[i].pszName != NULL; i++)
			{
				if(szInputSource.Compare(InputDefined[i].pszName) == NULL)
				{
					bFindInputSource = TRUE; nInputSource = i;	
				}
			}

			if(!bFindInputSource)
			{
				sMsg.Format("Invalid Input Source : %s",szInputSource);
				AfxMessageBox(sMsg); 
			}

			pStep->m_nLeftFreq[0]	= CurrentSet->nAudioSpec[nInputSource][0];
			pStep->m_nRightFreq[0]	= CurrentSet->nAudioSpec[nInputSource][1];
			pStep->m_nAudioMargin	= CurrentSet->nAudioSpec[nInputSource][2];
			pStep->m_nLeftLevel[0]	= CurrentSet->nAudioSpec[nInputSource][3];
			pStep->m_nRightLevel[0] = CurrentSet->nAudioSpec[nInputSource][4];
		}
		//add psh 090306
		//==============
		// CheckAudio_Level
		//==============
		if(pFunction->m_pFunc == Predefined[nCheckAudioNo_Level].pFunc)
		{
			bAudioTest = TRUE;
			
			pStep->m_bRunAudioTest = TRUE;
			pStep->m_nMeasureAudioType = LEVEL_CHECK;
			POSITION ArgPos = pFunction->m_ArgumentList.GetHeadPosition();
			
			int nTmp = 0;
			
			while(nTmp < 3)
			{
				Argument* pArgument = pFunction->m_ArgumentList.GetNext(ArgPos);
			
				if(nTmp == 0)
				{
					pStep->m_nAudioSource = pArgument->Integer;	 
				}
				else if(nTmp == 1)
				{
					pStep->m_nLeftLevel[0] =  pArgument->Integer;
				}
				else if(nTmp == 2)
				{
					pStep->m_nRightLevel[0] =  pArgument->Integer;
				}
				nTmp++;
			}
			pStep->m_nLeftFreq[0]	= 0;
			pStep->m_nRightFreq[0]	= 0;

		}
		//add psh 090306
		//==============
		// CheckAudio_Freq
		//==============
		if(pFunction->m_pFunc == Predefined[nCheckAudioNo_Freq].pFunc)
		{
			bAudioTest = TRUE;
			
			pStep->m_bRunAudioTest = TRUE;
			pStep->m_nMeasureAudioType = FREQUENCY_CHECK;
			POSITION ArgPos = pFunction->m_ArgumentList.GetHeadPosition();
			
			int nTmp = 0;
			
			while(nTmp < 3)
			{
				Argument* pArgument = pFunction->m_ArgumentList.GetNext(ArgPos);
			
				if(nTmp == 0)
				{
					pStep->m_nAudioSource = pArgument->Integer;	 
				}
				else if(nTmp == 1)
				{
					pStep->m_nLeftFreq[0] =  pArgument->Integer;
				}
				else if(nTmp == 2)
				{
					pStep->m_nRightFreq[0] =  pArgument->Integer;
				}
				nTmp++;
			}
			pStep->m_nLeftLevel[0]	= 0;
			pStep->m_nRightLevel[0] = 0;
		}
	}
	
	if(bAudioTest == FALSE)
	{
		pStep->m_bRunAudioTest = FALSE;

		pStep->m_nAudioSource	= 0;
		pStep->m_nLeftFreq[0]	= 0;
		pStep->m_nRightFreq[0]	= 0;
		pStep->m_nLeftLevel[0]	= 0;
		pStep->m_nRightLevel[0] = 0;
		pStep->m_nAudioMargin	= 0;
	}
}

BOOL CheckSource(CString sSource)
{
	CString sTmp;
	sSource.MakeUpper();
	sSource.TrimRight();
	sSource.TrimLeft();
	BOOL bCheck = FALSE;

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		sTmp.Format("%s",InputDefined[i].pszName);
		sTmp.MakeUpper();

		if(sSource.Compare(sTmp) == 0)
		{
			bCheck = TRUE;
			break;
		}
	}
	return bCheck;
}
//-

CStep* FindStep(int nStepNo)
{
	POSITION pos = StepList.GetHeadPosition();
    while(pos != NULL)
	{
		CStep* temp = StepList.GetNext(pos);
        if (temp->GetStepNo() == nStepNo) return temp;
    }
    return NULL;        
}   

void DeleteStepList()                                              
{
	POSITION pos = StepList.GetHeadPosition();
	while(pos != NULL)
    {       
        delete StepList.GetNext(pos);
    }
    StepList.RemoveAll();
}   

void FreeUserFile()
{
    for(int n = 0; n < CurrentSet->nTotalLine; n++)
        GlobalFreePtr(UserFile[n]);
}


BOOL AnalyzeIntArray()
{           
    for(; (pToken->type != FINISHED) && (pToken->type != RP);)
    {      
        if (GetToken() == FALSE) return FALSE;
        if ((pToken->type == COMMA) || (pToken->type == RP))
        {
            if (pToken->type != RP)
            {
                if (GetToken() == FALSE) return FALSE; // ',' skip
                if (pToken->type == INTEGER) continue;
            }
            else continue;
        }
        else return FALSE;  
    }
    if (pToken->type != RP) return FALSE;
    return TRUE;
}


long GetModifyTime(CString sPath)
{
	CFile file;
	CFileStatus status;
	CFileException e;
	CString s, s1;
	
	if(sPath.IsEmpty()) return -1L;

	//+change kwmoon 080910
//	if (!file.Open(sPath, CFile::modeRead , &e))
	if (!file.Open(sPath, CFile::modeRead | CFile::shareDenyNone, &e))
	{
		//+change kwmoon 070930
	//	s = _T("File could not be opened : ");
	//	if (e.m_cause == CFileException::fileNotFound) s1 = _T("The file could not be located.");
	//	else if (e.m_cause == CFileException::badPath) s1 = _T("All or part of the path is invalid.");
	//	else if (e.m_cause == CFileException::accessDenied) s1 = _T("The file could not be accessed.");
	//	else s1 = _T("An unspecified error occurred.");
	//	AfxMessageBox(s + s1);

		if (e.m_cause == CFileException::fileNotFound) s1.Format("Failed to load file!\n[%s]",sPath);
		else if (e.m_cause == CFileException::badPath) s1.Format("Invalid Path!\n[%s]",sPath);
		else if (e.m_cause == CFileException::accessDenied) s1.Format("Failed to access file!\n[%s]",sPath);
		else s1.Format("An unspecified error.\n[%s]",sPath);
		AfxMessageBox(s1);
	
		return -1L;
	}
	file.GetStatus(status);
	file.Close();

	return (long)status.m_mtime.GetTime();
}

void MakeFunctionList(CStep* pStep, char* fun)
{
	char	seps[] = _T("(),\n");
	char*	token;
	char*	endptr;
    char	*stop;             
	int		cbSyntex = 0;
	int		val;
	CString s;

	CFunction* pFunction = new CFunction;
	for(int nIndex = 0; Predefined[nIndex].pszFunc != NULL; nIndex++)
	{                             
		if (!strcmp(Predefined[nIndex].pszFunc, _strlwr(fun)))
        {
            pFunction->m_pFunc = Predefined[nIndex].pFunc;
			CString str;
			str.Format("%s", achCurString);
			CStringTokenizer *analizer = new CStringTokenizer(str);
			val = analizer->NextToken(); 
			while(Predefined[nIndex].achPara[cbSyntex] != TNULL && val != TT_EOF)                  
			{
				if (Predefined[nIndex].achPara[cbSyntex] == LP
					|| Predefined[nIndex].achPara[cbSyntex] == RP
					|| Predefined[nIndex].achPara[cbSyntex] == COMMA)
				{
					cbSyntex++;
					continue;
				}
				
				Argument* pArg = new Argument;
				val = analizer->NextToken(); 
				s = analizer->GetStrValue(); 
				if((val == 40) || (val == 41) || (val == 44))
				{
					val = analizer->NextToken(); 
					s = analizer->GetStrValue();
				}
				
				token = (char*)(LPCTSTR)s; 

				if (Predefined[nIndex].achPara[cbSyntex] == INTEGER)
				{	
					if (isdigit(token[0]))
					{
						if (token[1] == 'x')                   
						{                          
							pArg->Integer = (int)strtol(token, &stop, 16);
						}
 						else if (token[1] == 'b')      
						{
							pArg->Integer = (int)strtol(token + 2, &stop, 2);   
						}
						else 
						{
							pArg->Integer = atoi(token);
						}
					}
					else if((token[0] == '-') || token[0] == '+')
					{
						pArg->Integer = atoi(token);
					}
					else
					{
						for(int nIndex = 0; NameDefined[nIndex].pszName != NULL; nIndex++)
						{                             
							if (!strcmp(NameDefined[nIndex].pszName, _strlwr(token)))
							{       
								pArg->Integer = NameDefined[nIndex].nValue; 
							}
						}                        

					}
				}
				else if (Predefined[nIndex].achPara[cbSyntex] == REAL)
					pArg->Double = strtod(token, &endptr);
				else if (Predefined[nIndex].achPara[cbSyntex] == STRING)
				{
					strcpy(pArg->String, token);
				}
				else if (Predefined[nIndex].achPara[cbSyntex] == SIGN)
				{
					if (strstr(token, _T("+")) != NULL)	
						strcpy(pArg->String, _T("+"));
					else strcpy(pArg->String, _T("-"));
				}
				else AfxMessageBox(_T("Unhandle Parameter is presented."));
				pFunction->m_ArgumentList.AddTail(pArg);
				cbSyntex++;

				continue;
			} 
			delete analizer; analizer = NULL;

			break;
        }
	}
	if (g_CommantFuncArray.Find("//") < 0)
	{
		pFunction->m_strCommant = "\n";
	}
	else
	{
		pFunction->m_strCommant = g_CommantFuncArray;
	}		
	g_CommantFuncArray = "";
	pStep->m_InstructionList.AddTail(pFunction);
}

void DeleteRemoteList()
{
	POSITION Pos = CurrentSet->RemoteList.GetHeadPosition();
	while(Pos != NULL)
	{
		delete CurrentSet->RemoteList.GetNext(Pos);
	}
	CurrentSet->RemoteList.RemoveAll();
}

void DeletePatternList()
{
	POSITION Pos = CurrentSet->PatternList.GetHeadPosition();
	while(Pos != NULL)
	{
		delete CurrentSet->PatternList.GetNext(Pos);
	}
	CurrentSet->PatternList.RemoveAll();
}

BOOL SaveSeqFile(CString sSeqPath)
{
	//+add 090219(Modification No2)
	CString szLog = "B:SaveSeqFile\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
	
	int nTotalCnt = StepList.GetCount();
	if(nTotalCnt <1) 
	{
		AfxMessageBox("No Step!");
		return FALSE;
	}

	CStdioFile pFile;
	CFileException FileExcept;
	CString sTmp;
	CStringArray aTmp;
	aTmp.RemoveAll();

	if(FileExists(sSeqPath)) ::DeleteFile(sSeqPath);

	//+change kwmoon 080910
//	if(!pFile.Open(sSeqPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate   , &FileExcept)) 
	if(!pFile.Open(sSeqPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate| CFile::shareDenyNone   , &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	
	int nTmp;

	POSITION Position = StepList.GetHeadPosition();
	if(StepList.GetCount() > 0)
	{
		nTmp = 1;
		while (Position) 
		{
			CStep* pStep = StepList.GetNext(Position);
			//Step Header
			
			pFile.WriteString(pStep->m_strCommant);
				//pFile.WriteString("\n");
			
			sTmp = MakeStepHeader(pStep);
			pFile.WriteString(sTmp);
			//Sub Step
			MakeSubStep(pStep, aTmp);
			for(int nTmp1 = 0;nTmp1 <aTmp.GetSize(); nTmp1++ )
			{
				sTmp = aTmp.GetAt(nTmp1);
				if(sTmp.Find("//") < 0)
					pFile.WriteString("\n");
				pFile.WriteString(sTmp);

			}
			if (pStep->m_strInCommant.GetLength() < 2)
			{
				pFile.WriteString("\n");
			}
			else
			{
				pFile.WriteString(pStep->m_strInCommant);
			}
			//Step Tail
			sTmp = MakeStepTail(pStep);
			pFile.WriteString(sTmp);

			nTmp++;
		}
	}
	pFile.Close();

	POSITION pos = CurFunc.GetHeadPosition();
	while(pos != NULL)
	{       
		delete CurFunc.GetNext(pos);
	}
	CurFunc.RemoveAll();
	
	//+add 090219(Modification No2)
	szLog = "E:SaveSeqFile\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;
}

CString MakeStepHeader(CStep* pStep)
{
	CString sHeader = _T("");
	CString sProcess, sSource,sFunc, sTarget, sLowlinit, sHighlinit,sUnit;

	

	if(pStep->m_nStepType == PROCESS_FUNCTION)
	{
		sHeader.Format("step(%d, \"%s\"):process()\n{\n", 
									pStep->m_nStep,
									pStep->m_sName	);
	}
	else if(pStep->m_nStepType == CHECK_FUNCTION)
	{
//		sProcess = "check";

		//Criterion
		if(pStep->m_nFuncType == MEAS_BOOL)
		{
			if(pStep->GetNominal_Bool() == TRUE)	sTarget = "TRUE";
			else if(pStep->GetNominal_Bool() == FALSE) sTarget = "FALSE";
			sLowlinit = sHighlinit = sUnit = "";
		}
		else if(pStep->m_nFuncType != MEAS_BOOL)
		{
			sTarget.Format("%5.1f", pStep->m_fNominal);
			sLowlinit.Format("%5.1f", pStep->m_fLowLimit);
			sHighlinit.Format("%5.1f", pStep->m_fHighLimit);
			sUnit.Format("%s", pStep->m_sUnit);
		}
		sHeader.Format("step(%d, \"%s\"):check(\"%s\", %s, %s, %s, %s,\"%s\")\n{\n", 
//		sHeader.Format("step(%d, \"%s\"):check(\"%s\",\"%s\", %s, %s, %s, %s,\"%s\")\n{\n", 
									pStep->m_nStep,
									pStep->m_sName,
									pStep->m_strCurSrc,
								//	pStep->m_strNextSrc,
									pStep->m_strFuncName,
									sTarget,
									sLowlinit,
									sHighlinit,
									sUnit);
	}

	return sHeader;
}

BOOL MakeSubStep(CStep* pStep, CStringArray& aArray)
{
	CString sTmp, sSubStep;
	int nSelNo = 0;
	int nLeftFreq = 0, nLeftLevel = 0, nRightFreq = 0, nRightLevel = 0, nMargin = 0;
	
	BOOL bIsCheckAudio = FALSE;
	int  nCheckAudioType = -1;

		//+add psh 080809
	int nArgumentNo	  	   = 0;
	int nI2CfirstArgument  = 0;

	//+ 2007.9.5 Add BY USY
	if(pStep->m_bRunAudioTest == TRUE)
	{
		//+add kwmoon 080213
		if(g_pView->m_bAdjSpecMode && !pStep->m_bTest)
		{
			nLeftFreq	= pStep->m_nLeftFreq[0];	// [0] : Target Value, [1] : Measured Value
			nRightFreq	= pStep->m_nRightFreq[0];
			nLeftLevel	= pStep->m_nLeftLevel[0];
			nRightLevel = pStep->m_nRightLevel[0];
			nMargin		= pStep->m_nAudioMargin;
		}
		else
		{
			if((pStep->m_nLeftFreq[1]	>= 0)
			&& (pStep->m_nRightFreq[1]  >= 0)
			&& (pStep->m_nLeftLevel[1]	>= 0)
			&& (pStep->m_nRightLevel[1] >= 0)
			&& (pStep->m_nAudioMargin	>= 0))
			{
				nLeftFreq	= pStep->m_nLeftFreq[1];	// [0] : Target Value, [1] : Measured Value
				nRightFreq	= pStep->m_nRightFreq[1];
				nLeftLevel	= pStep->m_nLeftLevel[1];
				nRightLevel = pStep->m_nRightLevel[1];
				nMargin		= pStep->m_nAudioMargin;
			}
		}
	}

	
	sSubStep = _T("");
	POSITION Pos = pStep->m_InstructionList.GetHeadPosition();
	SortSubStep(pStep);
	aArray.RemoveAll();
	CString strTmp;

	if(pStep->m_nStepType == PROCESS_FUNCTION)
	{
		Pos = CurFunc.GetHeadPosition();
//		CFunction* pCurFunc = new CFunction();
		do 
		{
			CFunction* pCurFunc = CurFunc.GetNext(Pos);
			for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
			{
				if(pCurFunc->m_pFunc == Predefined[nTmp].pFunc)
				{
					nSelNo = nTmp;
					break;
				}
			}
			if(nSelNo == -1) return FALSE;

			POSITION ArgPos = pCurFunc->m_ArgumentList.GetHeadPosition();
			
			sSubStep = pCurFunc->m_strCommant;
			//strTmp.Format("\t %s", Predefined[nSelNo].pszFunc);
			strTmp.Format("\t %s", Predefined[nSelNo].pszFunc);
			sSubStep += strTmp;
			nArgumentNo	  	   = 0;
			nI2CfirstArgument  = 0;

			for(int nTmp = 0; Predefined[nSelNo].achPara[nTmp] != TNULL; nTmp++)
			{
				nArgumentNo++;
				if(Predefined[nSelNo].achPara[nTmp] == LP)
				{
					sSubStep += "(";
				}
				else if(Predefined[nSelNo].achPara[nTmp] == RP)
				{
					sSubStep += ")";
				}
				else if(Predefined[nSelNo].achPara[nTmp] == COMMA)
				{
					sSubStep += ",";
				}

				else if(Predefined[nSelNo].achPara[nTmp] == INTEGER)
				{
					
					if(ArgPos  == NULL) break;
					else 
					{
//						Argument* pArgument = new Argument;
						Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);	
						sTmp.Format("%d ", pArgument->Integer);

						//+add psh 090702
						if(sSubStep.Mid(2,16).Compare("send_i2c_command") == NULL)
						{
							if(nArgumentNo == 2)
							{
								switch(pArgument->Integer)
								{
									case 101 : sTmp = "I2C_TYPE";		break;
									case 102 : sTmp = "I2C_CMD";		break;
									case 103 : sTmp = "I2C_READ";		break;
									case 104 : sTmp = "I2C_CHK_VALUE";	break;
									case 105 : sTmp = "I2C_CHK_MAXMIN";	break;
									case 106 : sTmp = "I2C_CHK_LIMIT";	break;
									case 107 : sTmp = "I2C_2B_READ";	break;
									case 108 : sTmp = "I2C_2B_WRITE";	break;
									case 109 : sTmp = "I2C_CHANNEL";	break;
									//+add kwmoon 080911
									case 112 : sTmp = "I2C_CHK_VER";	break;
								}
								nI2CfirstArgument = pArgument->Integer;
							}
							else if(nArgumentNo == 4)
							{
								if(nI2CfirstArgument == I2C_TYPE)
								{
									switch(pArgument->Integer)
									{
										case 110 : sTmp = "DDC_2AB";			break;
										case 111 : sTmp = "DDC_2B";			break;
									}
								}
								else
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
							}
							else if(nArgumentNo == 6)
							{
								if(nI2CfirstArgument != I2C_CHK_VALUE)
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
							}
						}
						//-
						sSubStep += sTmp;
					}
				}
				else if((Predefined[nSelNo].achPara[nTmp] == STRING))
				{
					if(ArgPos  == NULL) break;
					else
					{
//						Argument* pArgument = new Argument;
						Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
						sTmp.Format("\"%s\" ", pArgument->String);
						sSubStep += sTmp;
					}
				}
				//+ 2007.10.10 Add BY USY
				else if((Predefined[nSelNo].achPara[nTmp] == REAL))
				{
					if(ArgPos  == NULL) break;
					else
					{
//						Argument* pArgument = new Argument;
						Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
						sTmp.Format("%.1f ", pArgument->Double);
						sSubStep += sTmp;
					}
				}
				//-
			}
			//sSubStep.Delete(sSubStep.GetLength() - 1, 1);
//			sSubStep += ")\n\n";
			
			//sSubStep += pCurFunc->m_strCommant;
			//	sSubStep += "\n";
			
			aArray.Add(sSubStep);
			//sSubStep.
			
			
		} while(Pos != NULL);
		
	}
	else if(pStep->m_nStepType == CHECK_FUNCTION)
	{
//		CFunction* pCurFunc = new CFunction();

		Pos = CurFunc.GetHeadPosition();

		while(Pos != NULL)
		{
			CFunction* pCurFunc = CurFunc.GetNext(Pos);

			for(int nTmp1 = 0; Predefined[nTmp1].pszFunc != NULL; nTmp1++)
			{
				if(pCurFunc->m_pFunc == Predefined[nTmp1].pFunc)
				{
					nSelNo = nTmp1;

					if(Predefined[nTmp1].pszFunc == _T("check_audio"))
					{
						bIsCheckAudio = TRUE;
						nCheckAudioType = FREQ_LEVEL_CHECK;
					}
					else if(Predefined[nTmp1].pszFunc == _T("checkaudio_level"))
					{
						bIsCheckAudio = TRUE;
						nCheckAudioType = LEVEL_CHECK;
					}
					else if(Predefined[nTmp1].pszFunc == _T("checkaudio_freq"))
					{
						bIsCheckAudio = TRUE;
						nCheckAudioType = FREQUENCY_CHECK;
					}
					break;
				}
			}
			POSITION ArgPos = pCurFunc->m_ArgumentList.GetHeadPosition();
			
			sSubStep.Format("\t %s", Predefined[nSelNo].pszFunc);

			if((bIsCheckAudio == TRUE) && (pStep->m_bRunAudioTest == TRUE))
			{
				//Audio Source
				ArgPos = pCurFunc->m_ArgumentList.FindIndex(0);
				Argument* pArgument = pCurFunc->m_ArgumentList.GetAt(ArgPos);
				sTmp.Format("(%d ,", pArgument->Integer);
				sSubStep += sTmp;

				//Value Change
				if(nCheckAudioType == LEVEL_CHECK){
					sTmp.Format("%d , %d )",  nLeftLevel, nRightLevel);
					sSubStep += sTmp;
				}
				else if(nCheckAudioType == FREQUENCY_CHECK){
					sTmp.Format("%d , %d )", nLeftFreq, nRightFreq);
					sSubStep += sTmp;
				}
				else{
					sTmp.Format("%d , %d , %d , %d, %d ,", 
							 nLeftFreq, nRightFreq, nLeftLevel, nRightLevel, nMargin);
					sSubStep += sTmp;
					//Message
					ArgPos = pCurFunc->m_ArgumentList.FindIndex(6);
					pArgument = pCurFunc->m_ArgumentList.GetAt(ArgPos);
					sTmp.Format("\"%s\" )\n\n", pArgument->String);
					sSubStep += sTmp;
				}
				bIsCheckAudio = FALSE;
				//	
			}
			else 
			{
				//+add psh 080809
				nArgumentNo	  	   = 0;
				nI2CfirstArgument  = 0;

				for(int nTmp1 = 0; Predefined[nSelNo].achPara[nTmp1] != TNULL; nTmp1++)
				{
					nArgumentNo++;

//					Argument* pArgument = new Argument;
					if(Predefined[nSelNo].achPara[nTmp1] == LP)
					{
						sSubStep += "(";
					}
					else if(Predefined[nSelNo].achPara[nTmp1] == RP)
					{
						sSubStep += ")\n\n";
					}
					else if(Predefined[nSelNo].achPara[nTmp1] == COMMA)
					{
						sSubStep += ",";
					}
					else if(Predefined[nSelNo].achPara[nTmp1] == INTEGER)
					{
						if(ArgPos  == NULL) break;
						else 
						{
							Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);	
							sTmp.Format("%d ", pArgument->Integer);
							//+add psh 080829
							if(sSubStep.Mid(2,16).Compare("send_i2c_command") == NULL)
							{
								if(nArgumentNo == 2)
								{
									switch(pArgument->Integer)
									{
										case 101 : sTmp = "I2C_TYPE";		break;
										case 102 : sTmp = "I2C_CMD";		break;
										case 103 : sTmp = "I2C_READ";		break;
										case 104 : sTmp = "I2C_CHK_VALUE";	break;
										case 105 : sTmp = "I2C_CHK_MAXMIN";	break;
										case 106 : sTmp = "I2C_CHK_LIMIT";	break;
										case 107 : sTmp = "I2C_2B_READ";	break;
										case 108 : sTmp = "I2C_2B_WRITE";	break;
										case 109 : sTmp = "I2C_CHANNEL";	break;
										//+add kwmoon 080911
										case 112 : sTmp = "I2C_CHK_VER";	break;
									}
									nI2CfirstArgument = pArgument->Integer;
								}
								else if(nArgumentNo == 4)
								{
									if(nI2CfirstArgument == I2C_TYPE)
									{
										switch(pArgument->Integer)
										{
											case 110 : sTmp = "DDC_2AB";			break;
											case 111 : sTmp = "DDC_2B";			break;
										}
									}
									else
									{
										sTmp.Format("0x%02x ", pArgument->Integer);
									}
								}
								else if(nArgumentNo == 6)
								{
									if(nI2CfirstArgument != I2C_CHK_VALUE)
									{
										sTmp.Format("0x%02x ", pArgument->Integer);
									}
								}
							}
							//-
							//+add psh 090703
							if(sSubStep.Mid(2,15).Compare("set_dimmingtest") == NULL)
							{
								if(nArgumentNo == 4)
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
								else if(nArgumentNo == 6)
								{
									sTmp.Format("0x%02x ", pArgument->Integer);
								}
							}//-

							sSubStep += sTmp;
						}
					}
					else if((Predefined[nSelNo].achPara[nTmp1] == STRING))
					{
						if(ArgPos  == NULL) break;
						else 
						{
							Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
							sTmp.Format("\"%s\" ", pArgument->String);
							sSubStep += sTmp;
						}
					}
					//+ 2007.10.10 Add BY USY
					else if((Predefined[nSelNo].achPara[nTmp1] == REAL))
					{
						if(ArgPos  == NULL) break;
						else
						{
	//						Argument* pArgument = new Argument;
							Argument* pArgument = pCurFunc->m_ArgumentList.GetNext(ArgPos);
							sTmp.Format("%.1f ", pArgument->Double);
							sSubStep += sTmp;
						}
					}
					//-
				}
			}
			
			//sSubStep.Delete(sSubStep.GetLength() - 1, 1);
//				sSubStep += ")\n\n";
			
				sSubStep += pCurFunc->m_strCommant;
				//sSubStep += "\n";
			
			aArray.Add(sSubStep);
		} 

	}
	return TRUE;
}

CString MakeStepTail(CStep* pStep)
{
	CString sTail = _T("");
	sTail.Format("}\n");
	return sTail;
}

void SortSubStep(CStep* pStep)
{
	if(pStep == NULL) return;
	
	POSITION pos = CurFunc.GetHeadPosition();
	while(pos != NULL)
	{       
		delete CurFunc.GetNext(pos);
	}
	CurFunc.RemoveAll();

	POSITION FuncPos = pStep->m_InstructionList.GetHeadPosition();
	
	while (FuncPos != NULL) 
	{
		CFunction* pOrgFunc = pStep->m_InstructionList.GetNext(FuncPos);
		CFunction* pCopyFunc = new CFunction();

		pCopyFunc->m_pFunc = pOrgFunc->m_pFunc;


		POSITION ArgPos = pOrgFunc->m_ArgumentList.GetHeadPosition();

		while(ArgPos != NULL)
		{
			Argument* pOrgArgument = pOrgFunc->m_ArgumentList.GetNext(ArgPos);
			Argument* pCopyArgument = new Argument;

			memcpy(pCopyArgument->String, pOrgArgument->String, sizeof(pOrgArgument->String));
			pCopyFunc->m_ArgumentList.AddTail(pCopyArgument);
		}
		CurFunc.AddTail(pCopyFunc);


	}
}
//-

BOOL GetFloat()
{
	char sign;
	double value;

    if (GetToken() == FALSE) return FALSE;

	if (pToken->type == SIGN) 
	{
		sign = pToken->value.c;
		if (GetToken() == FALSE) return FALSE;
		if (pToken->type == REAL) value = pToken->value.f;
		else if(pToken->type == INTEGER) value = pToken->value.i;
		else return FALSE;

		if (sign == '-') {
		//	if (pToken->type == REAL){ pToken->value.f = (float)-value;
		//	else if(pToken->type == INTEGER) pToken->value.i = (int)-value;
			 value =  value * (-1);
		}
		//return TRUE;
	}
    else if (pToken->type == REAL) value = pToken->value.f;
	else if(pToken->type == INTEGER) value = pToken->value.i;
	else return FALSE;

	pToken->value.f = value;

	return TRUE;
}

//+add kwmoon 081024
void DeleteModelList()
{
	POSITION Pos = CurrentSet->ModelList.GetHeadPosition();
	
	while(Pos != NULL)
	{
		delete CurrentSet->ModelList.GetNext(Pos);
	}
	CurrentSet->ModelList.RemoveAll();
}
