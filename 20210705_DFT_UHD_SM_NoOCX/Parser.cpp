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

CTypedPtrList<CPtrList, CStep*> ForcedAutoStepList;

CStep*		pCurStep;
CVFTestInfo *pCurStep_EM;

CFunction*	pCurFunc;

CTypedPtrList<CPtrList, CFunction*> CurFunc;	//Current Sub Step List

extern CDATsysView*  g_pView;

//+change 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

POSITION	PosStep;
POSITION	PosFunc;

CString		sParserMsg;
CString		cFuncName;
char		achCurString[256];
char*		pWalker;

int			nCurLine;
int			cMeasFuncType;

//int			nSoundOK_Count;

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
	_T("i2c_chk_maxmin2"),	I2C_CHK_MAXMIN2,	INTEGER, 
	_T("i2c_chk_limit2") ,	I2C_CHK_LIMIT2,	INTEGER, 
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
	_T("checkaudio_level_nofreq")		 ,	15,			INTEGER,

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
	_T("tvcomm_test"),		_TVComm_Test,			MEAS_BOOL,
	_T("signal_line_test"),	_Signal_Line_Test,		MEAS_BOOL,
	_T("audio_test"),		_Audio_Test,			MEAS_BOOL,
	_T("moving_pic_test"),	_Moving_Pic_Test,		MEAS_BOOL,
	_T("manual_test"),		_Manual_Test,			MEAS_BOOL,
	_T("adc_test"),			_Adc_Test,				MEAS_BOOL,
//	_T("adc_adjcheck"),		_ADC_AdjConfirmation,	MEAS_BOOL,
	_T("adc_result_check"),	_ADC_AdjResultCheck,	MEAS_BOOL,
//	_T("edid_resul_tcheck"),	_EDID_DownloadResultCheck,	MEAS_DOUBLE,

	_T("color_bias_test"),		_Color_Bias_Test,		MEAS_DOUBLE,
	_T("version_check"),		_Version_Check,			MEAS_BOOL,
	_T("mnt_version_check"),	_MNT_Version_Check,		MEAS_BOOL,
	_T("option_check"),			_Option_Check,			MEAS_BOOL,
	_T("option_checkall"),		_Option_CheckAll,		MEAS_BOOL,

	_T("areacode_check"),			_AreaCode_Check,		MEAS_BOOL,
	_T("countrygr_check"),			_CountryGr_Check,		MEAS_BOOL,

	_T("module_voltage_test"),	_Module_Voltage_Test,		MEAS_DOUBLE,
	_T("edid_download"),		_EDID_Download,				MEAS_BOOL,
	_T("lan_port_check"),		_LAN_Port_Check,			MEAS_BOOL,
	_T("esn_download_check"),	_ESN_Download_Check,		MEAS_BOOL,
	_T("dimming_check"),		_Domming_Check,				MEAS_BOOL,
	_T("ci_key_check"),			_CI_Key_Check,				MEAS_BOOL,
	_T("ci_card_check"),		_CI_Card_Check,				MEAS_BOOL,
	_T("marlin_download_check"), _Marlin_Download_Check,	MEAS_BOOL,
	_T("builtin_wifi_check"),	_BuiltIn_Wifi_Check,		MEAS_BOOL,
	_T("rjp_port_check"),		_RJP_Port_Check,			MEAS_BOOL,
	_T("magic_remocon_check"),	_Magic_Remocon_Check,		MEAS_BOOL,
	_T("wireless_ready_check"),	_Wireless_Ready_Check,		MEAS_BOOL,
	_T("dmm_measuredc"),		_DMM_MeasureDC,				MEAS_DOUBLE,
	_T("dmm_measureac"),		_DMM_MeasureAC,				MEAS_DOUBLE,
	_T("dmm_measureresistance"),_DMM_MeasureResistance,		MEAS_DOUBLE,
	_T("dmm_measurefrequency"),	_DMM_MeasureFrequency,		MEAS_DOUBLE,
	_T("dmm_dischargecheck"),	_DMM_DischargCheck,			MEAS_DOUBLE,

	_T("vf_measurevolt"),		_VF_MeasureVolt,			MEAS_DOUBLE,
	_T("vf_measurefreq"),		_VF_MeasureFreq,			MEAS_DOUBLE,
	_T("vf_measurevolt_minmax"),_VF_MeasureVolt_MinMax,	MEAS_BOOL,
	_T("vf_measurefreq_minmax"),_VF_MeasureFreq_MinMax,	MEAS_BOOL,
	_T("vf_dischargecheck"),	_VF_DischargCheck,		MEAS_BOOL,

	_T("widevine_download_check"), _Widevine_Download_Check,MEAS_BOOL,
	_T("rf_emitter_check"),		_RF_Emitter_Check,			MEAS_BOOL,
	_T("mac_add_check"),		_MAC_Address_Check,			MEAS_BOOL,
	_T("usb_Highspeed_check"),	_USB_HighSpeed_Check,		MEAS_BOOL,
	_T("bcm_chip_check"),		_BCM_Chip_Check,			MEAS_BOOL,
	_T("key1_check"),			_Key1_Check,				MEAS_BOOL,
	_T("key2_check"),			_Key2_Check,				MEAS_BOOL,
	_T("eyeq_check"),			_EyeQ_Check,				MEAS_BOOL,
	_T("cntv_key_dl_check"),	_CNTV_Key_DL_Check,			MEAS_BOOL,
	_T("hdcp_tx_key_dl_check"),	_HDCP_Tx_Key_DL_Check,		MEAS_BOOL,
	_T("hdcp_rx_key_dl_check"),	_HDCP_Rx_Key_DL_Check,		MEAS_BOOL,
	_T("dtcp_rx_key_dl_check"),	_DTCP_Rx_Key_DL_Check,		MEAS_BOOL,
	_T("atsc30_key_dl_check"),	_ATSC_30_Key_DL_Check,		MEAS_BOOL,
	_T("mfi_key_dl_check"),		_MFi_Key_DL_Check,			MEAS_BOOL,
	_T("countrygr_br_write"),	_CountryGr_BR_Write,		MEAS_BOOL,
	_T("countrygr_kr_write"),	_CountryGr_KR_Write,		MEAS_BOOL,
	_T("countrygr_ja_write"),	_CountryGr_JA_Write,		MEAS_BOOL,
	_T("google_key_dl_check"),	_Google_Key_DL_Check,		MEAS_BOOL,
//	_T("hdcpkey_download"),		_HdcpKey_Download,			MEAS_BOOL,
//	_T("hdcpkey_check"),		_HdcpKey_Download_Check,	MEAS_BOOL,
	_T("lnb_voltage_check"),	_LNB_MeasureVoltage,		MEAS_DOUBLE,
	_T("lnb_frequency_check"),	_LNB_MeasureFrequency,		MEAS_DOUBLE,
	_T("lnb2_voltage_check"),	_LNB_MeasureVoltage2,		MEAS_DOUBLE,
	_T("lnb2_frequency_check"),	_LNB_MeasureFrequency2,		MEAS_DOUBLE,
	_T("input_detect_check"),	_Input_Detect_Check,		MEAS_BOOL,
	_T("avc_irdata_check"),		_AVC_IRDataCheck,		    MEAS_BOOL,
	_T("avc_irblaster_check"),	_AVC_IRBlasterDataCheck,    MEAS_BOOL,
	_T("avc_comdata_check"),	_AVC_ComDataCheck,		    MEAS_BOOL,
	_T("drm_key_dl_check"),		_Drm_Key_DL_Check,		    MEAS_BOOL,
	_T("input_detect_allcheck"),_Input_Detect_AllCheck,	    MEAS_BOOL,
	_T("vf_test_resultcheck")	,_VF_Test_ResultCheck,	    MEAS_BOOL,
	_T("vf_measure_check"),		_VF_Measure_Check,	    MEAS_BOOL,
#ifdef SM_MODIFY_CODE__
	_T("hp_disable_check"),		_HP_Disable_Check,	    MEAS_BOOL,	
	_T("port_auto_test"),		_Port_Auto_Check,	    MEAS_BOOL,
#endif	
	_T("ir_blaster_check"),		_IR_Blaster_Check,	    MEAS_DOUBLE,
	_T("read_a_cas_id"),		_Read_A_CAS_ID,	    MEAS_BOOL,
	_T("bat_ver_check"),			_BAT_Ver_Check,				MEAS_BOOL,
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

	_T("set_input_source"), _SetInputSource,
	{LP, STRING, COMMA, STRING, COMMA,INTEGER, COMMA,INTEGER, COMMA,INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Source"), _T("Remote Key"), _T("SourceKey Delay"), _T("Remote Delay"),_T("Send Exit"), _T("Message"), _T("")},
	{VIDEO_SRC_SEL_TYPE,  REMOTE_SEL_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("set_input"), _SetInput,
	{LP, STRING, COMMA, STRING,COMMA,INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Source"), _T("SourceKey"), _T("Remote Delay"), _T("Message"), _T("")},
	{VIDEO_SRC_SEL_TYPE,  REMOTE_SEL_TYPE, REMOTE_SEL_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("set_rf_channel"), _SetRfChannel,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA,INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Channel"), _T("ChannelKey Delay"), _T("Remote Delay"), _T("Retry Delay"),_T("Send Exit"),_T("Message"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("set_rf"), _SetRf,
	{LP, INTEGER, COMMA,INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Channel"), _T("Remote Delay"), _T("Retry Delay"),_T("Message"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("set_tv_command"), _SetTVCommand,
	{LP, STRING, COMMA, STRING, COMMA,INTEGER, COMMA, STRING, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Command1"), _T("Command2"), _T("Set ID"), _T("Data"), _T("Wait Limit"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("check_adc_result"), _CheckAdcResult,
	{LP, STRING, COMMA, STRING, COMMA,INTEGER, COMMA, STRING, COMMA, INTEGER, COMMA,INTEGER, COMMA,STRING, COMMA, INTEGER, COMMA,  STRING, RP, TNULL},
	{_T("Command1"), _T("Command2"), _T("Set ID"), _T("Data"), _T("Wait Limit"),_T("Check Type"), _T("Target Value"),_T("Target Margin"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

//+add 090417
	_T("check_adc_result2"), _CheckAdcResult2,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Check Type"), _T("R_Target Value"), _T("G_Target Value"), _T("B_Target Value"), _T("Margin"), _T("Wait Limit"),_T("Check Optio"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NULL_TYPE},
//-
	_T("send_tv_command"), _SendTVCommand,
	{LP, STRING, COMMA, STRING, COMMA,INTEGER, COMMA, STRING, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Command1"), _T("Command2"), _T("Set ID"), _T("Data"), _T("Wait Limit"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("send_i2c_command"), _SendI2cCommand,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Command Type"),_T("Data1"),_T("Data2"),_T("Data3"), _T("Delay"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("send_adc_check"), _SendAdcCheck,
	{LP, STRING, COMMA, STRING, COMMA,INTEGER, COMMA, STRING, COMMA, INTEGER, COMMA,INTEGER, COMMA,STRING, COMMA, INTEGER, COMMA,  STRING, RP, TNULL},
	{_T("Command1"), _T("Command2"), _T("Set ID"), _T("Data"), _T("Wait Limit"),_T("Check Type"), _T("Target Value"),_T("Target Margin"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_tvcommport_echo"), _SetTvCommPortEcho,
	{LP, INTEGER, RP, TNULL},
	{_T("Echo Off"), _T("")},
	{ECHO_SEL_TYPE},

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

	_T("set_lvds_resolution"), _SetLvdsResolution_2in1,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Grab Width"),_T("Grab Height"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE, NULL_TYPE},
	
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
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA,INTEGER, COMMA,STRING,RP, TNULL},
	{_T("Audio Source"), _T("Left Freq"), _T("Right Freq"),_T("Left Level"), _T("Right Level"),_T("Margin"),_T("Message"), _T("")},
	{AUDIO_SRC_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("authentication_key"), _AuthenticationKey,
	{LP, INTEGER, RP, TNULL},
	{_T("Key"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("skipped_signalline_low_bits"), _SkippedSignalLineLowBits,
	{LP, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Data"), _T("Message"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_tool_option"), _SetToolOption,
	{LP, INTEGER, COMMA, INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Option"), _T("Remote Delay"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_area_option"), _SetAreaOption,
	{LP, INTEGER, COMMA,INTEGER, COMMA, STRING, RP, TNULL},
	{_T("Option"), _T("Remote Delay"), _T("Message"), _T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

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

	_T("tvlink_setchannel_pal"), _TVLink_SetChannel_PAL,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("TVLink Set PAL Channel"),_T("RF Source"),_T("Channel"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("rs232c_tvcontrol"), _RS232C_TVControl_Command,
	{LP, STRING, COMMA, INTEGER, RP, TNULL},
	{_T("TV Control Command"),_T("Wait")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("tooloption_write"), _ToolOption_Write,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tooloption_writeall"), _ToolOption_WriteAll,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("areaoption_write"), _AreaOption_Write,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

//+add PSH 090129
	_T("hdmigen_setedid"), _HDMIGen_SetEDID,
	{LP, INTEGER, RP, TNULL},
	{_T("HDMI Gen. EDID Pass/Check"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("hdmigen_sethdcp"), _HDMIGen_SetHDCP,
	{LP, INTEGER, RP, TNULL},
	{_T("HDMI Gen. HDCP Off/On"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("hdmigen_setcec"), _HDMIGen_SetCEC,
	{LP, INTEGER, RP, TNULL},
	{_T("HDMI Gen. CEC Off/On"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("hdmigen_setarc"), _HDMIGen_SetARC,
	{LP, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Port No"), _T("HDMI Gen. ARC Off/On"),_T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("hdmigen_setmhl"), _HDMIGen_SetMHL,
	{LP, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Port No"), _T("HDMI Gen. MHL Off/On"),_T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("hdmigen_setout"), _HDMIGen_SetOut,
	{LP, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Model No"), _T("Pattern No"), _T("")},
	{NORMAL_EDIT_TYPE, NORMAL_EDIT_TYPE, NULL_TYPE},

	_T("hdmigen_setddcline"), _HDMIGen_SetDDCLine,
	{LP, INTEGER, RP, TNULL},
	{_T("HDMI Gen. DDCLine Open/Close"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("checkaudio_level"), _CheckAudio_Level,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Audio Source"), _T("Left Level"), _T("Right Level"),_T("")},
	{AUDIO_SRC_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("checkaudio_freq"), _CheckAudio_Frequency,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Audio Source"), _T("Left Freq."), _T("Right Freq."),_T("")},
	{AUDIO_SRC_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("checkaudio_level_nofreq"), _CheckAudio_Level_NoFrequency,
	{ LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA, INTEGER, COMMA,INTEGER,RP, TNULL },
	{ _T("Audio Source"), _T("Left Freq"), _T("Right Freq"),_T("Left Level"), _T("Right Level"),_T("Margin"), _T("") },
	{ AUDIO_SRC_SEL_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE },



	_T("tv_mic_on"), _TV_Mic_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tv_mic_off"), _TV_Mic_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tv_headphone_on"), _TV_HeadPhone_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tv_headphone_off"), _TV_HeadPhone_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},
	//add 090527
	_T("set_dimmingtest"), _Set_DimmingTest,
	{LP, INTEGER, COMMA, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Test Channel"), _T("DimmingTest Address"), _T("DimmingTest Command"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("dmm_setmeasurechannel"), _DMM_SetMeasureChannel,
	{LP, INTEGER, RP, TNULL},
	{_T("Test Dmm Channel"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("vf_setmeasurechannel"), _VF_SetMeasureChannel,
	{LP, INTEGER, RP, TNULL},
	{_T("Test VF Channel"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("vf_setmeasure_minmax"), _VF_SetMeasure_MinMax,
	{LP, INTEGER, COMMA, INTEGER, COMMA, REAL, COMMA, REAL, RP, TNULL},
	{_T("Test Channel"), _T("Test Time"), _T("Min_Spec"), _T("Max_Spec"),_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_3d_grab"), _Set_3D_Grab,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("tv_lnb_on"), _TV_LNB_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tv_lnb_off"), _TV_LNB_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tv_3dmode_on"), _TV_3DMode_On,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("tv_3dmode_off"), _TV_3DMode_Off,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("audiomeasurereset"), _AudioMeasureReset,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("set_macadd"), _Set_MacAddress,
	{LP, STRING, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_usbcheck"), _Set_USBHighSpeed,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_countrygroup"), _Set_CountryGroup,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("ac_power_onoff"), _AC_Power_OnOff,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("mhl_buffercontrol"), _MHL_BufferControl,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("mhl_detectlinecontrol"), _MHL_DetectLineControl,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("set_detectchecksource"), _SetDetectCheckSource,
	{LP, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("Input Detect Check Source"), _T("Detec Mode"),_T("")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("grabber_reset"), _Grabber_Reset,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("avc_bufferclear"), _AVC_BufferClear,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("set_hdmigrab"), _Set_HdmiGrab,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, RP, TNULL},
	{_T("Input"), _T("Resolution"), _T("BitShift"),_T("")},
	{AVC_INPUT, HDMI_GRAB_RESOLUTION, HDMI_GRAB_BITSHIFT,NULL_TYPE},

	_T("read_keyitem"), _Read_KeyItem,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("read_pcbaid"), _Read_PCBAID_READ,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("jig_cylindercompcontrol"), _Set_CylinderComp,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("jig_cylinderearpcontrol"), _Set_CylinderEarphone,
	{LP, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("jig_cylindercontrol"), _Set_Cylinder,
	{LP, INTEGER, COMMA, INTEGER, RP, TNULL},
	{_T("")},
	{NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("isfailepoweron"), _IsFailePowerOn,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},

	_T("set_movingpictest_margin"), _SetMovingPicTestMargin,
	{LP, INTEGER, COMMA, INTEGER, COMMA,INTEGER, RP, TNULL},
	{_T("Diff_LoLimit"), _T("DIff_HiLimit"), _T("Test Margin(%)")},
	{NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NORMAL_EDIT_TYPE,NULL_TYPE},

	_T("vf_measure_start"), _VF_MeasureStart,
	{LP, RP, TNULL},
	{_T("")},
	{NULL_TYPE},
	
	_T("rotate_display"), _RotateDisplay,
	{ LP, RP, TNULL },
	{ _T("") },
	{ NULL_TYPE },

	_T("set_tv_baudrate"), _SetTVBuadRate,
	{ LP, INTEGER, RP, TNULL },
	{ _T("BaudRate"), _T("") },
	{ NORMAL_EDIT_TYPE, NULL_TYPE },


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

	strncpy_s(aAuthentificationKey, 11, &achBuf[1],10);

	nAuthentificationKey = atoi(aAuthentificationKey);

	time_t timerModifiedDate1  = nAuthentificationKey;
	time_t timerModifiedDate2 = CurrentSet->lTime;

	struct tm tmModifiedDateBlock1;
	struct tm tmModifiedDateBlock2;
	
	localtime_s(&tmModifiedDateBlock1, &timerModifiedDate1);	
	localtime_s(&tmModifiedDateBlock2, &timerModifiedDate2);	

	DWORD tmModifiedDateSecond1 = tmModifiedDateBlock1.tm_hour * 3600 +
								  tmModifiedDateBlock1.tm_min * 60 +
							 	  tmModifiedDateBlock1.tm_sec;	
		
	DWORD tmModifiedDateSecond2 = tmModifiedDateBlock2.tm_hour * 3600 +
								  tmModifiedDateBlock2.tm_min * 60 +
								  tmModifiedDateBlock2.tm_sec;	

	int tmTemp = tmModifiedDateSecond1 - tmModifiedDateSecond2;

	if((tmModifiedDateBlock1.tm_year != tmModifiedDateBlock2.tm_year)
	|| (tmModifiedDateBlock1.tm_mon  != tmModifiedDateBlock2.tm_mon)
	|| (tmModifiedDateBlock1.tm_mday != tmModifiedDateBlock2.tm_mday)
	|| (abs(tmTemp) > 10))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

_InputSource InputDefined_FHD[] = 
{
	_T("RF"),   //0
	_T("AV"),
	_T("AV1"),
	_T("AV2"),
	_T("AV3"),  
	_T("AV4"), //5
	_T("YC"),
	_T("YC1"),
	_T("YC2"),
	_T("YC3"), 
	_T("YC4"), //10
	_T("COMP"),
	_T("COMP1"),
	_T("COMP2"),
	_T("COMP3"),  
	_T("MNT-OUT"), //15
	_T("TV-OUT"),
	_T("AV-OUT"),
	_T("RGB"),
	_T("HDMI"), 
	_T("HDMI1"), //20
	_T("HDMI2"),
	_T("HDMI3"),
	_T("HDMI4"),
	_T("HDMI5"),  
	_T("VAR"),   //25
	_T("E/P"),
	_T("OPT"),
	_T("VOUT"),
	_T("VOUT1"),  
	_T("VOUT2"), //30
	_T("VOUT3"),
	_T("VOUT4"),
	_T("DTV"),
	_T("ATV"),
	_T("RGB-DTV"), //35
	_T("DVI-DTV"),
	_T("DVI-PC"),
	_T("HDMI-PC"),
	_T("DVD"),
	_T("SPDIF"),    //40
	_T("OSD_TEST"),
	_T("SCART1"),
	_T("SCART2"),
	_T("SCART_RGB"),
	_T("COMP1-AV1"),  //45
	_T("COMP1-AV2"),
	_T("COMP2-AV1"),
	_T("COMP2-AV2"),
	_T("MHL"),
//	_T("HDMI-DIM"), //50
//	_T("COMP-DIM"),  
//	_T("AV-DIM"),
	NULL
};

_InputSource InputDefined_UHD[] = 
{
	_T("RF"),   //0
	_T("AV"),
	_T("AV1"),
	_T("AV2"),
	_T("YC"),
	_T("YC1"),  //5
	_T("YC2"),
	_T("COMP"),
	_T("COMP1"),
	_T("COMP2"),
	_T("MNT-OUT"), //10
	_T("TV-OUT"),
	_T("AV-OUT"),
	_T("RGB"),
	_T("HDMI"), 
	_T("HDMI1"), //15
	_T("HDMI2"),
	_T("HDMI3"),
	_T("HDMI4"),
	_T("HDMI5"),  
	_T("VAR"),   //20
	_T("E/P"),
	_T("OPT"),
	_T("VOUT"),
	_T("VOUT1"),  
	_T("VOUT2"), //25
	_T("DTV"),
	_T("ATV"),
	_T("RGB-DTV"), 
	_T("DVI-DTV"),
	_T("DVI-PC"),  //30
	_T("HDMI-PC"),
	_T("DVD"),
	_T("SPDIF"),    
	_T("OSD_TEST"),
	_T("SCART1"),   //35
	_T("SCART2"),
	_T("SCART_RGB"),
	_T("COMP1-AV1"),  
	_T("COMP1-AV2"),
	_T("COMP2-AV1"),   //40
	_T("COMP2-AV2"),
	_T("MHL"),
	_T("AVC-RGB"),
	_T("AVC-DVI"),
	_T("AVC-DP"),    //45
	_T("AVC-HDMI"),
	_T("HDMI-DIM"), 
	_T("COMP-DIM"),    
	_T("AV-DIM"),   
	_T("DP"),    //50   
	_T("OPS"),   
	_T("HDMI-GRAB"),//52 //190225 GODTECH
	NULL
};
         
int LoadFile(CString sPath)
{
    char achBuf[256];
    FILE *fp;     
    int nLineIndex = 0;
    
	CString szErrMsg = _T("");

	fopen_s(&fp, (LPCSTR)sPath, _T("rt"));
    if (fp  == NULL)
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
//    register cbIndex;     
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
			strcpy_s(pToken->value.s, _countof(pToken->value.s), pWalker + 2);
			while(*pWalker != '\n') *pWalker++;
			continue;	//return TRUE;
		}
		// temp
		if (*pWalker == '#' && *(pWalker + 1) == '#')
		{
			pToken->type = COMMENT;
			
			char sGroupNo[3];
			memset(sGroupNo,NULL,3);
			strcpy_s(pToken->value.s, _countof(pToken->value.s), pWalker + 2);

			while(*pWalker != ' ') *pWalker++;
			strncpy_s(sGroupNo, sizeof(sGroupNo), pWalker, 3);
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
				strcpy_s(pToken->value.s, _countof(pToken->value.s), achBuf);  
				return FALSE;
			}
			pWalker++; achBuf[cbIndex] = '\0';
			pToken->type = STRING;
			strcpy_s(pToken->value.s, _countof(pToken->value.s), achBuf);
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
			strcpy_s(pToken->value.s, _countof(pToken->value.s), achBuf);
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
				_strlwr_s(pToken->value.s, strlen(pToken->value.s) + 1);
				if (!strcmp(pToken->value.s, _T("true"))) bNominal = TRUE;
				else if (!strcmp(pToken->value.s, _T("false"))) bNominal = FALSE;
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
 

	pStep->SetStepNo(nStepNo);
	pStep->SetStepName(sStepName);
	pStep->m_nStepType = func_type; 

	if (func_type != PROCESS_FUNCTION)
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
		if (cFuncName.Compare("color_test") == 0)
			pStep->SetTestType(RGB_TEST);
		else if (cFuncName.Compare("bright_test") == 0)		pStep->SetTestType(BRIGHT_TEST);
		else if (cFuncName.Compare("noise_test") == 0)		pStep->SetTestType(NOISE_TEST);
		else if (cFuncName.Compare("position_test") == 0)	pStep->SetTestType(POSITION_TEST);
		else if (cFuncName.Compare("osd_test") == 0)			pStep->SetTestType(OSD_TEST);
		else if (cFuncName.Compare("size_test") == 0)		pStep->SetTestType(SIZE_TEST);
		else if (cFuncName.Compare("lan_test") == 0)			pStep->SetTestType(LAN_TEST);
		else if (cFuncName.Compare("tvcomm_test") == 0)		pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("signal_line_test") == 0) pStep->SetTestType(SIGNAL_LINE_TEST);
		else if (cFuncName.Compare("audio_test") == 0)		pStep->SetTestType(AUDIO_TEST);
		else if (cFuncName.Compare("moving_pic_test") == 0)	pStep->SetTestType(MOVING_PIC_TEST);
		else if (cFuncName.Compare("manual_test") == 0)		pStep->SetTestType(MANUAL_TEST);
		else if (cFuncName.Compare("caption_test") == 0)		pStep->SetTestType(CAPTION_TEST);
		else if (cFuncName.Compare("digitaleye_test") == 0)	pStep->SetTestType(DIGITAL_EYE_TEST);
		else if (cFuncName.Compare("adc_test") == 0)
		{
			pStep->SetTestType(TV_COMM_TEST);
		}
		else if (cFuncName.Compare("adc_result_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("edid_result_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("edid_download") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("color_bias_test") == 0)	pStep->SetTestType(BIAS_TEST);
		else if (cFuncName.Compare("module_voltage_test") == 0)	pStep->SetTestType(MODULE_TEST);
		else if (cFuncName.Compare("version_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("option_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("lan_port_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("esn_download_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("ci_key_check") == 0)	pStep->SetTestType(TV_COMM_TEST);  //100401 add
		else if (cFuncName.Compare("rjp_Port_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("magic_remocon_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("wireless_ready_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("widevine_download_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("rf_emitter_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("bcm_chip_check") == 0)	pStep->SetTestType(BCM_CHIP_TEST);
		else if (cFuncName.Compare("key1_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("key2_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("eyeq_check") == 0)
		{		
			pStep->SetTestType(TV_COMM_TEST);
		}
		else if(cFuncName.Compare("cntv_key_dl_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("hdcp_tx_key_dl_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("hdcp_rx_key_dl_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("dtcp_rx_key_dl_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("atsc30_key_dl_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if (cFuncName.Compare("mfi_key_dl_check") == 0)	pStep->SetTestType(TV_COMM_TEST);
		else if(cFuncName.Compare("drm_key_dl_check") == 0)	pStep->SetTestType(KEY_DL_TEST);
		else if(cFuncName.Compare("input_detect_allcheck") == 0)	pStep->SetTestType(INPUT_DETECT_TEST);
		else if(cFuncName.Compare("vf_test_resultcheck") == 0)	pStep->SetTestType(VF_MEASURE_TEST);
		else if(cFuncName.Compare("vf_measure_check") == 0)	pStep->SetTestType(VF_MEASURE_TEST);
		
#ifdef SM_MODIFY_CODE__
		else if(cFuncName.Compare("hp_disable_check") == 0)	pStep->SetTestType(INPUT_DETECT_TEST);		
		else if (cFuncName.Compare("port_auto_test") == 0)
		{
			pStep->SetTestType(AUTO_PORT_TEST);
			//ParseForceInitStep(nStepNo);
			//return TRUE;
		}		
#endif

		else if(cFuncName.Compare("ir_blaster_check") == 0)	
			pStep->SetTestType(VF_MEASURE_TEST);
		//_T("ir_blaster_check"),		_IR_Blaster_Check,	    MEAS_BOOL,

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

		if(!pStep->m_bWorkFunc){
			sParserMsg.Format(_T("Step(%2d) : work function not found"), nStepNo);
			AfxMessageBox(sParserMsg);

			return FALSE;
		}
	}

	GetAudioValue(pStep);

	StepList.AddTail(pStep);

    sParserMsg.Format(_T("Step(%2d) Function : %s"), nStepNo, sStepName);
    
    return TRUE;
}

BOOL ParseForceInsertStep(int nCheckID, POSITION nStepPos)
{
#if 1		

	/////////////////////////////////////////////////////////////
	if (ScanPortCheckStep(nCheckID) == TRUE)
	{
		return TRUE;
	}
	CString strTmp;
	CString strName[FORCED_CHECK_ID_MAX] =
	{
		"AT HEADPHONE TEST#",	//#define FORCED_CHECK_ID_HEADPHONE		0
		"AT GREENEYE TEST#",		//#define FORCED_CHECK_ID_GREENEYE		1
		"AT COMPONENT TEST#",	//#define FORCED_CHECK_ID_COMPONENT		2
		"AT AV TEST#",			//#define FORCED_CHECK_ID_AV				3
		"AT USB_1 TEST#",		//#define FORCED_CHECK_ID_USB_1			4
		"AT USB_2 TEST#",		//#define FORCED_CHECK_ID_USB_2			5
		"AT USB_3 TEST#",		//#define FORCED_CHECK_ID_USB_3			6
		"AT HDMI_1 TEST#",		//#define FORCED_CHECK_ID_HDMI_1			7
		"AT HDMI_1_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_1_ARC		8
		"AT HDMI_2 TEST#",		//#define FORCED_CHECK_ID_HDMI_2			9
		"AT HDMI_2_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_2_ARC		10
		"AT HDMI_3 TEST#",		//#define FORCED_CHECK_ID_HDMI_3			11
		"AT HDMI_3_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_3_ARC		12
		"AT HDMI_4 TEST#",		//#define FORCED_CHECK_ID_HDMI_4			13
		"AT HDMI_4_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_4_ARC		14
		"AT RGB TEST#",			//#define FORCED_CHECK_ID_RGB				15
		"AT SCART TEST#",		//#define FORCED_CHECK_ID_SCART			16
	};
	//ForcedAutoStepList.RemoveAll();
		////	StepList.InsertAfter(PosStep,);
	//for (int i = 0; i < FORCED_CHECK_ID_MAX; i++)
	{

		CStep* pFoceStep = new CStep();
		//	CFunction* pFoceFunc = new CFunction;			
		//	Argument* pForceArg = new Argument;

		switch (nCheckID)
		{
		case FORCED_CHECK_ID_HEADPHONE:
			//step(16, "HEADPHONE CHECK") : check("RF", audio_test, TRUE, , , "")
			//{
			//	rs232c_tvcontrol("es 00 50", 700)
			//		checkaudio_freq(7, 1000, 400)
			//		work()
			//		rs232c_tvcontrol("es 00 60", 700)
			//}

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"es 00 50\", 700)");

			strTmp.Format(_T("checkaudio_freq(7, %d, %d)"), CurrentSet->nAudioForce_HP_L, CurrentSet->nAudioForce_HP_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			MakeForceFunctionList(pFoceStep, "work", "work()");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"es 00 60\", 700)");

			//ForcedAutoStepList.AddTail(pFoceStep);
			//StepList.AddTail(pFoceStep);
			/*StepList.InsertAfter(nStepPos , pFoceStep);
			g_nStepPlusCount++;
			nStepNo++;*/

			break;
		case  FORCED_CHECK_ID_GREENEYE:
			/*step(15, "EYE CHECK") : check("RF", eyeq_check, TRUE, , , "")
			{
				work()
			}*/
			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("eyeq_check") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "work", "work()");

			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;

			break;
		case   FORCED_CHECK_ID_COMPONENT:
			//step(18, "COMP1 CHECK") : check("COMP1", color_test, 0.0, -30.0, 30.0, "%")
			//{
			//	grab_image_check("COMP1.bmp", 1, 7000, "")
			//		checkaudio_freq(1, 400, 400)
			//		work()
			//}

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("COMP1");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"COMP1.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_Analog_L, CurrentSet->nAudioForce_Analog_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;

			break;
		case  FORCED_CHECK_ID_AV:
			//AV1
			/*step(15, "AV1 CHECK") : check("AV1", color_test, 0.0, -40.0, 40.0, "%")
			{
				grab_image_check("AV1.bmp", 1, 10000, "")

					work()


			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("AV1");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"AV1.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_Analog_L, CurrentSet->nAudioForce_Analog_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;

			break;

		case  FORCED_CHECK_ID_USB_1:
			/*
			step(23, "USB1 CHECK"):check("RF", adc_test, TRUE, , ,"")
			{
				 set_tv_command("m" ,"c" ,0 ,"ee" ,700 ,"USB1 TEST" )

				 work()

			}
			*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("adc_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "set_tv_command", "set_tv_command(\"m\" ,\"c\" ,0 ,\"ee\" ,700 ,\"USB1 TEST\" )");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case  FORCED_CHECK_ID_USB_2:
			//step(24, "USB2 CHECK") : check("RF", adc_test, TRUE, , , "")
			//{
			//	set_tv_command("m", "c", 0, "a8", 700, "USB2 TEST")

			//		work()

			//}
			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("adc_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "set_tv_command", "set_tv_command(\"m\" ,\"c\" ,0 ,\"a8\" ,700 ,\"USB2 TEST\" )");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case  FORCED_CHECK_ID_USB_3:
			//step(24, "USB3 CHECK") : check("RF", adc_test, TRUE, , , "")
			//{
			//	set_tv_command("m", "c", 0, "9a", 700, "USB2 TEST")

			//		work()

			//}

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("adc_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "set_tv_command", "set_tv_command(\"m\" ,\"c\" ,0 ,\"9a\" ,700 ,\"USB3 TEST\" )");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_1:
			/*step(36, "HDMI1 CHECK") : check("HDMI1", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI1.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI1");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI1.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;
		case   FORCED_CHECK_ID_HDMI_1_ARC:
			/*			step(38, "HDMI1 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
								{
									rs232c_tvcontrol("aa 00 00", 700)

										hdmigen_setarc(2, 1)

										checkaudio_freq(6, 400, 1000)

										work()

										hdmigen_setarc(2, 0)

								}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
			strTmp.Format(_T("hdmigen_setarc(%d, 1)"), CurrentSet->nARC_Force_OnOffChannel+1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());//nARC_Force_OnOffChannel
			strTmp.Format(_T("checkaudio_freq(6, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 0)");
			//strTmp.Format(_T("hdmigen_setarc(%d, 1)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			//MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());
			strTmp.Format(_T("hdmigen_setarc(%d, 0)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_2:
			/*step(36, "HDMI2 CHECK") : check("HDMI2", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI2.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI2");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI2.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
			//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_2_ARC:
			/*			step(38, "HDMI2 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
			{
				rs232c_tvcontrol("aa 00 00", 700)

					hdmigen_setarc(2, 1)

					checkaudio_freq(6, 400, 1000)

					work()

					hdmigen_setarc(2, 0)

			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
			strTmp.Format(_T("hdmigen_setarc(%d, 1)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());
						
			strTmp.Format(_T("checkaudio_freq(6, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			strTmp.Format(_T("hdmigen_setarc(%d, 0)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());

			//MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 0)");

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;
		case   FORCED_CHECK_ID_HDMI_3:
			/*step(36, "HDMI3 CHECK") : check("HDMI3", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI3.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI3");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI3.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
			//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_3_ARC:
			/*			step(38, "HDMI3 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
			{
				rs232c_tvcontrol("aa 00 00", 700)

					hdmigen_setarc(2, 1)

					checkaudio_freq(6, 400, 1000)

					work()

					hdmigen_setarc(2, 0)

			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
			//MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");

			strTmp.Format(_T("hdmigen_setarc(%d, 1)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());

			strTmp.Format(_T("checkaudio_freq(6, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			strTmp.Format(_T("hdmigen_setarc(%d, 0)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 0)");

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_4:
			/*step(36, "HDMI4 CHECK") : check("HDMI4", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI4.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI4");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI4.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
			//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;
		case   FORCED_CHECK_ID_HDMI_4_ARC:
			/*			step(38, "HDMI4 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
			{
				rs232c_tvcontrol("aa 00 00", 700)

					hdmigen_setarc(2, 1)

					checkaudio_freq(6, 400, 1000)

					work()

					hdmigen_setarc(2, 0)

			}*/

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
		//	MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");
			strTmp.Format(_T("hdmigen_setarc(%d, 1)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());



			strTmp.Format(_T("checkaudio_freq(6, %d, %d)"), CurrentSet->nAudioForce_HDMI_L, CurrentSet->nAudioForce_HDMI_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			strTmp.Format(_T("hdmigen_setarc(%d, 0)"), CurrentSet->nARC_Force_OnOffChannel + 1);
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", strTmp.GetBuffer());
		//	MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 0)");

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case   FORCED_CHECK_ID_RGB:
			//step(18, "RGB CHECK") : check("RGB", color_test, 0.0, -30.0, 30.0, "%")
			//{
			//	grab_image_check("RGB.bmp", 1, 7000, "")
			//		checkaudio_freq(1, 400, 400)
			//		work()
			//}

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RGB");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"RGB.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_RGB_L, CurrentSet->nAudioForce_RGB_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		case   FORCED_CHECK_ID_SCART:
			//step(18, "SCART CHECK") : check("SCART", color_test, 0.0, -30.0, 30.0, "%")
			//{
			//	grab_image_check("SCART.bmp", 1, 7000, "")
			//		checkaudio_freq(1, 400, 400)
			//		work()
			//}

			pFoceStep->SetStepNo(nCheckID);
			pFoceStep->SetStepName(strName[nCheckID]);//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("SCART");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"SCART.bmp\", 1, 7000, \"\")");
			strTmp.Format(_T("checkaudio_freq(1, %d, %d)"), CurrentSet->nAudioForce_SCART_L, CurrentSet->nAudioForce_SCART_R);
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", strTmp.GetBuffer());
			//MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			//StepList.AddTail(pFoceStep);
			//g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			//nStepNo++;
			break;

		default:
			return FALSE;
			break;
		}
		GetAudioValue(pFoceStep);
		//StepList.InsertAfter(nStepPos, pFoceStep);
		StepList.InsertBefore(nStepPos, pFoceStep);
		g_nStepPlusCount++;
		
	}

	return TRUE;
#endif

}



BOOL ScanPortCheckStep(int nCheckID)
{
#if 1	

	if ((nCheckID < 0) || (nCheckID >= FORCED_CHECK_ID_MAX))
	{
		return FALSE;
	}

	CString strName[FORCED_CHECK_ID_MAX] =
	{ 		
		"AT HEADPHONE TEST#",	//#define FORCED_CHECK_ID_HEADPHONE		0
		"AT GREENEYE TEST#",		//#define FORCED_CHECK_ID_GREENEYE		1
		"AT COMPONENT TEST#",	//#define FORCED_CHECK_ID_COMPONENT		2
		"AT AV TEST#",			//#define FORCED_CHECK_ID_AV				3
		"AT USB_1 TEST#",		//#define FORCED_CHECK_ID_USB_1			4
		"AT USB_2 TEST#",		//#define FORCED_CHECK_ID_USB_2			5
		"AT USB_3 TEST#",		//#define FORCED_CHECK_ID_USB_3			6
		"AT HDMI_1 TEST#",		//#define FORCED_CHECK_ID_HDMI_1			7
		"AT HDMI_1_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_1_ARC		8
		"AT HDMI_2 TEST#",		//#define FORCED_CHECK_ID_HDMI_2			9
		"AT HDMI_2_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_2_ARC		10
		"AT HDMI_3 TEST#",		//#define FORCED_CHECK_ID_HDMI_3			11
		"AT HDMI_3_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_3_ARC		12
		"AT HDMI_4 TEST#",		//#define FORCED_CHECK_ID_HDMI_4			13
		"AT HDMI_4_ARC TEST#",	//#define FORCED_CHECK_ID_HDMI_4_ARC		14
		"AT RGB TEST#",			//#define FORCED_CHECK_ID_RGB				15
		"AT SCART TEST#",		//#define FORCED_CHECK_ID_SCART			16
	};

	
		//time_t ltime;
		//time(&ltime);
		//struct tm today;
		//
		//localtime_s(&today, &ltime);
		//today.
		//_tcsftime(tmpbuf, 128, _T("%Y%m%d"), &today);


		//CStep* pFoceStep = new CStep();
	CString strTmp;
	CString strTmpRef;
	POSITION Position = StepList.GetHeadPosition();

	strTmpRef = strName[nCheckID];
	while (Position != NULL)
	{
		CStep* pStep = StepList.GetNext(Position);
		strTmp = pStep->GetStepName();

		if (strTmp.Compare(strTmpRef) == 0)
		{
			return TRUE;
		}

	}

	return FALSE;
		
		
					
					

			
#endif

}


BOOL ParseForceInitStep(int nStepNo)
{
#if 1		
	
	/////////////////////////////////////////////////////////////

	//ForcedAutoStepList.RemoveAll();

	for (int i = 0; i < FORCED_CHECK_ID_MAX; i++)
	{

		CStep* pFoceStep = new CStep();
		//	CFunction* pFoceFunc = new CFunction;			
		//	Argument* pForceArg = new Argument;

		switch (i)
		{
		case FORCED_CHECK_ID_HEADPHONE:
			//step(16, "HEADPHONE CHECK") : check("RF", audio_test, TRUE, , , "")
			//{
			//	rs232c_tvcontrol("es 00 50", 700)
			//		checkaudio_freq(7, 1000, 400)
			//		work()
			//		rs232c_tvcontrol("es 00 60", 700)
			//}

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HEADPHONE CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"es 00 50\", 700)");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 1000, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"es 00 60\", 700)");

			//ForcedAutoStepList.AddTail(pFoceStep);
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++;
			nStepNo++;
		
			break;
		case  FORCED_CHECK_ID_GREENEYE:
			/*step(15, "EYE CHECK") : check("RF", eyeq_check, TRUE, , , "")
			{
				work()
			}*/
			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("EYE CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("eyeq_check") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "work", "work()");

			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;

			break;
		case   FORCED_CHECK_ID_COMPONENT:
			//step(18, "COMP1 CHECK") : check("COMP1", color_test, 0.0, -30.0, 30.0, "%")
			//{
			//	grab_image_check("COMP1.bmp", 1, 7000, "")
			//		checkaudio_freq(1, 400, 400)
			//		work()
			//}

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("COMP1 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("COMP1");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"COMP1.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;

			break;
		case  FORCED_CHECK_ID_AV:
			//AV1
			/*step(15, "AV1 CHECK") : check("AV1", color_test, 0.0, -40.0, 40.0, "%")
			{
				grab_image_check("AV1.bmp", 1, 10000, "")

					work()


			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("AV1 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("AV1");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"AV1.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;

			break;

		case  FORCED_CHECK_ID_USB_1:
			/*
			step(23, "USB1 CHECK"):check("RF", adc_test, TRUE, , ,"")
			{
				 set_tv_command("m" ,"c" ,0 ,"ee" ,700 ,"USB1 TEST" )

				 work()

			}
			*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("USB1 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("adc_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "set_tv_command", "set_tv_command(\"m\" ,\"c\" ,0 ,\"ee\" ,700 ,\"USB1 TEST\" )");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case  FORCED_CHECK_ID_USB_2:
			//step(24, "USB2 CHECK") : check("RF", adc_test, TRUE, , , "")
			//{
			//	set_tv_command("m", "c", 0, "a8", 700, "USB2 TEST")

			//		work()

			//}
			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("USB2 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("adc_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "set_tv_command", "set_tv_command(\"m\" ,\"c\" ,0 ,\"a8\" ,700 ,\"USB2 TEST\" )");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case  FORCED_CHECK_ID_USB_3:
			//step(24, "USB3 CHECK") : check("RF", adc_test, TRUE, , , "")
			//{
			//	set_tv_command("m", "c", 0, "9a", 700, "USB2 TEST")

			//		work()

			//}

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("USB3 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("adc_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(TV_COMM_TEST); //
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "set_tv_command", "set_tv_command(\"m\" ,\"c\" ,0 ,\"9a\" ,700 ,\"USB3 TEST\" )");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_1:
			/*step(36, "HDMI1 CHECK") : check("HDMI1", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI1.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI1 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI1");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI1.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;
		case   FORCED_CHECK_ID_HDMI_1_ARC:
			/*			step(38, "HDMI1 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
								{
									rs232c_tvcontrol("aa 00 00", 700)

										hdmigen_setarc(2, 1)

										checkaudio_freq(6, 400, 1000)

										work()

										hdmigen_setarc(2, 0)

								}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI1 ARC CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_2:
			/*step(36, "HDMI2 CHECK") : check("HDMI2", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI2.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI2 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI2");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI2.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
			//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_2_ARC:
			/*			step(38, "HDMI2 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
			{
				rs232c_tvcontrol("aa 00 00", 700)

					hdmigen_setarc(2, 1)

					checkaudio_freq(6, 400, 1000)

					work()

					hdmigen_setarc(2, 0)

			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI2 ARC CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;
		case   FORCED_CHECK_ID_HDMI_3:
			/*step(36, "HDMI3 CHECK") : check("HDMI3", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI3.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI3 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI3");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI3.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
			//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_3_ARC:
			/*			step(38, "HDMI3 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
			{
				rs232c_tvcontrol("aa 00 00", 700)

					hdmigen_setarc(2, 1)

					checkaudio_freq(6, 400, 1000)

					work()

					hdmigen_setarc(2, 0)

			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI3 ARC CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case   FORCED_CHECK_ID_HDMI_4:
			/*step(36, "HDMI4 CHECK") : check("HDMI4", color_test, 0.0, -20.0, 20.0, "%")
			{
				grab_image_check("HDMI4.bmp", 1, 7000, "")

					checkaudio_freq(1, 400, 1000)

					work()

			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI4 CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("HDMI4");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(20);
			pFoceStep->SetLowLimit(-20);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"HDMI4.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			//pFoceFunc
			//pForceArg
			//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;
		case   FORCED_CHECK_ID_HDMI_4_ARC:
			/*			step(38, "HDMI4 ARC CHECK") :check("RF", audio_test, TRUE, , , "")
			{
				rs232c_tvcontrol("aa 00 00", 700)

					hdmigen_setarc(2, 1)

					checkaudio_freq(6, 400, 1000)

					work()

					hdmigen_setarc(2, 0)

			}*/

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("HDMI4 ARC CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("audio_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RF");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(AUDIO_TEST);
			pFoceStep->SetHighLimit(0);
			pFoceStep->SetLowLimit(0);
			pFoceStep->SetUnit("");

			MakeForceFunctionList(pFoceStep, "rs232c_tvcontrol", "rs232c_tvcontrol(\"aa 00 00\", 700)");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(6, 400, 1000)");
			MakeForceFunctionList(pFoceStep, "work", "work()");
			MakeForceFunctionList(pFoceStep, "hdmigen_setarc", "hdmigen_setarc(2, 1)");

			//pFoceFunc
			//pForceArg
				//pFoceStep->m_InstructionList		
			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case   FORCED_CHECK_ID_RGB:
			//step(18, "RGB CHECK") : check("RGB", color_test, 0.0, -30.0, 30.0, "%")
			//{
			//	grab_image_check("RGB.bmp", 1, 7000, "")
			//		checkaudio_freq(1, 400, 400)
			//		work()
			//}

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("RGB CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("RGB");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"RGB.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		case   FORCED_CHECK_ID_SCART:
			//step(18, "SCART CHECK") : check("SCART", color_test, 0.0, -30.0, 30.0, "%")
			//{
			//	grab_image_check("SCART.bmp", 1, 7000, "")
			//		checkaudio_freq(1, 400, 400)
			//		work()
			//}

			pFoceStep->SetStepNo(nStepNo);
			pFoceStep->SetStepName("SCART CHECK");//
			pFoceStep->m_nStepType = CHECK_FUNCTION;

			if (MeasFunc_SyntexChecking("color_test") == FALSE)
			{
				return FALSE;
			}
			else
			{
				pFoceStep->SetFunction(cMeasFunc);
				pFoceStep->SetFuncType(cMeasFuncType);
				pFoceStep->SetFuncName(cFuncName);
			}

			pFoceStep->SetCurrentSource("SCART");
			pFoceStep->SetNextSource("");
			pFoceStep->SetTestType(RGB_TEST); //
			pFoceStep->SetHighLimit(30);
			pFoceStep->SetLowLimit(-30);
			pFoceStep->SetUnit("%");

			MakeForceFunctionList(pFoceStep, "grab_image_check", "grab_image_check(\"SCART.bmp\", 1, 7000, \"\")");
			MakeForceFunctionList(pFoceStep, "checkaudio_freq", "checkaudio_freq(1, 400, 400)");
			MakeForceFunctionList(pFoceStep, "work", "work()");

			StepList.AddTail(pFoceStep);
			g_nStepPlusCount++; //ForcedAutoStepList.AddTail(pFoceStep);
			nStepNo++;
			break;

		default:
			break;
		}
	}
#endif

}

BOOL ParseBlock(CStep* pStep)
{
    char achFuncName[31];
    BOOL bFindFlag = FALSE;
    
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
            strcpy_s(achFuncName, _countof(achFuncName), pToken->value.s);
            if (SyntexChecking(achFuncName) == FALSE) return FALSE;

			MakeFunctionList(pStep, achFuncName);
        }               
        else if (pToken->type == COMMENT) continue;
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
        
BOOL  MeasFunc_SyntexChecking(char* achFunc)
{
    int nIndex;
    
    for(nIndex = 0; MeasDefined[nIndex].pszFunc != NULL; nIndex++)
    {                             
		_strlwr_s(achFunc, strlen(achFunc) + 1);
        if (!strcmp(MeasDefined[nIndex].pszFunc, achFunc))
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
		_strlwr_s(achName, strlen(achName) + 1);
        if (!strcmp(NameDefined[nIndex].pszName, achName))
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
    
	//+ 2007.10.4 Add BY USY
	int	nCheck = 0;	// 0 : no check, 1 : set rf channel check, 2 : set input source check
	//-

    for(nIndex = 0; Predefined[nIndex].pszFunc != NULL; nIndex++)
    {                             
		_strlwr_s(achFunc, strlen(achFunc) + 1);
         if (!strcmp(Predefined[nIndex].pszFunc,achFunc))
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
						if (GetToken() == FALSE) return FALSE;
						if (pToken->type != REAL)
						{
							if (pToken->type != INTEGER)
							{
								pToken->value.i = IDS_TYPE_MISMATCH;
								return FALSE;
							}
						}
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
	_strlwr_s(pszString, strlen(pszString) + 1);
    if (!strcmp(pszString, _T("step"))) return STEPHEADER;
    else if (!strcmp(pszString, _T("adc_adjtype"))) return ADC_TYPE;
	//-
	else 
		return HEADERERROR;
}               
 
FuncionType FunctionTypeChecking(char* pszString)
{
 	_strlwr_s(pszString, strlen(pszString) + 1);
   if (!strcmp(pszString, _T("process"))) return PROCESS_FUNCTION;
    else if (!strcmp(pszString, _T("check"))) return CHECK_FUNCTION;
    else if (!strcmp(pszString, _T("adjust"))) return ADJUST_FUNCTION;
    else if (!strcmp(pszString, _T("manual_adjust"))) return MANUAL_ADJUST_FUNCTION;
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

        if (GetToken() == FALSE) goto SYNTEXERROR;
        else if (pToken->type == KEYWORD)
        {
			header = HeaderChecking(pToken->value.s);
			if (header == STEPHEADER)
			{

				
				if (ParseStep(nStepCnt) == FALSE) goto SYNTEXERROR;
				dlg.SetStatus((LPCSTR)sParserMsg);

				//+change kwmoon 071206
			//	StepSize++;
				/*if (g_nStepPlusCount > 0)
				{
					g_nStepSize += g_nStepPlusCount;

					nStepCnt += g_nStepPlusCount;
				}
				else
				{*/

					g_nStepSize++;

					nStepCnt++;
				//}
			}
			//+add PSH 080410
			else if(header == ADC_TYPE)
			{
				if (GetADC_AdjType() == FALSE) goto SYNTEXERROR;
			} //-
			else
			{
				pToken->value.i = IDS_UNDEFINED_WORD;
				goto SYNTEXERROR;
			}
        }
        else if (pToken->type == COMMENT) continue;
        else if (pToken->type == FINISHED) break;
        else if (pToken->type == END_OF_FILE) break;
        else goto SYNTEXERROR;
    }

	FreeUserFile();
    if (bCancel == TRUE) return FALSE;
	if ((nCurLine == CurrentSet->nTotalLine) || (pToken->type == FINISHED))
	{
		delete pToken;
///////////////////////////////////////////////////////////////
		/*
		int nTotalCnt = StepList.GetCount();
		int lForcetestFound = 0;
		CStep *lpCheckStep;
		
		POSITION	lPosStep;
		lPosStep = StepList.GetHeadPosition();
		if (StepList.GetCount() > 0)
		{			
			while (lPosStep)
			{
				lpCheckStep = StepList.GetNext(lPosStep);

				if (lpCheckStep->m_nTestType == AUTO_PORT_TEST)
				{
					lForcetestFound = 1;
					break;
				}
			}			
		}
		if (lForcetestFound == 1)
		{
			return TRUE;
		}
		else
		{
			AfxMessageBox("Step [port_auto_test] Not Found !!");
			return FALSE;
		}*/
////////////////////////////////////////////////////////////////		
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

        if (GetTokenFullSeq(nTotalLine) == FALSE) goto SYNTEXERROR;
        else if (pToken->type == KEYWORD)
        {
			header = HeaderChecking(pToken->value.s);
			if (header == STEPHEADER) 
			{
				if (ParseFullSeqStep(nStepCnt, nTotalLine) == FALSE) goto SYNTEXERROR;
//				dlg.SetStatus((LPCSTR)sMsg);

				//+change kwmoon 071206
				//	StepSize++;
				g_nStepSize++;

				nStepCnt++;
			}
			//+add PSH 080410
			else if(header == ADC_TYPE)
			{
				if (GetADC_AdjType() == FALSE) goto SYNTEXERROR;
			} //-
			else
			{
				pToken->value.i = IDS_UNDEFINED_WORD;
				goto SYNTEXERROR;
			}
        }
        else if (pToken->type == COMMENT) continue;
        else if (pToken->type == FINISHED) break;
        else if (pToken->type == END_OF_FILE) break;
        else goto SYNTEXERROR;
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

	fopen_s(&fp, (LPCSTR)sPath, _T("rt"));
    if (fp  == NULL)
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
	BOOL bNominal = 0;
	double fNominal = 16, fHigh = 18, fLow = 15;
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
				_strlwr_s(pToken->value.s, strlen(pToken->value.s) + 1);
				if (!strcmp(pToken->value.s, _T("true"))) bNominal = TRUE;
				else if (!strcmp(pToken->value.s, _T("false"))) bNominal = FALSE;
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
		if(cFuncName.Compare("color_test") == 0)			
			pStep->SetTestType(RGB_TEST);
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

	GetAudioValue(pStep);

	FullSeq.AddTail(pStep);

    sParserMsg.Format(_T("Step(%2d) Function : %s"), nStepNo, sStepName);
    
    return TRUE;
}
BOOL GetTokenFullSeq(int nTotalLine)
{
    char achBuf[256];
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
			strcpy_s(pToken->value.s, _countof(pToken->value.s), pWalker + 2);
			while(*pWalker != '\n') *pWalker++;
			continue;	//return TRUE;
		}
		
		// temp
		if (*pWalker == '#' && *(pWalker + 1) == '#')
		{
			pToken->type = COMMENT;
			
			char sGroupNo[3];
			memset(sGroupNo,NULL,3);
			strcpy_s(pToken->value.s, _countof(pToken->value.s), pWalker + 2);

			while(*pWalker != ' ') *pWalker++;
			strncpy_s(sGroupNo, sizeof(sGroupNo), pWalker, 3);
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
				strcpy_s(pToken->value.s, _countof(pToken->value.s), achBuf);  
				return FALSE;
			}
			pWalker++; achBuf[cbIndex] = '\0';
			pToken->type = STRING;
			strcpy_s(pToken->value.s, _countof(pToken->value.s), achBuf);
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
			strcpy_s(pToken->value.s, _countof(pToken->value.s), achBuf);
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
            strcpy_s(achFuncName, _countof(achFuncName), pToken->value.s);
            if (SyntexChecking(achFuncName) == FALSE) return FALSE;

			MakeFunctionList(pStep, achFuncName);
			
        }               
        else if (pToken->type == COMMENT) continue;
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
void GetAudioValue(CStep* pStep)
{
	int nCheckAudioNo	= -1;
	int nCheckAudioNo_Level	= -1;
	int nCheckAudioNo_Freq	= -1;
	int nCheckAudioNo_Level_NoFreq	= -1;

	BOOL bAudioTest = FALSE;

	for(int nTmp = 0; Predefined[nTmp].pszFunc != NULL; nTmp++)
	{
		if(strcmp(Predefined[nTmp].pszFunc,"check_audio") == NULL)
		{
			nCheckAudioNo = nTmp;
		}

		if(strcmp(Predefined[nTmp].pszFunc,"checkaudio_level") == NULL)
		{
			nCheckAudioNo_Level = nTmp;
		}
	
		if(strcmp(Predefined[nTmp].pszFunc,"checkaudio_freq") == NULL)
		{
			nCheckAudioNo_Freq = nTmp;
		}

		if (strcmp(Predefined[nTmp].pszFunc, "checkaudio_level_nofreq") == NULL)
		{
			nCheckAudioNo_Level_NoFreq = nTmp;
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
		
		if (pFunction->m_pFunc == Predefined[nCheckAudioNo_Level_NoFreq].pFunc)
		{
			bAudioTest = TRUE;

			pStep->m_bRunAudioTest = TRUE;
			pStep->m_nMeasureAudioType = FREQ_LEVEL_CHECK_MOVING;
			POSITION ArgPos = pFunction->m_ArgumentList.GetHeadPosition();

			int nTmp = 0;

			while (nTmp < 6)
			{
				Argument* pArgument = pFunction->m_ArgumentList.GetNext(ArgPos);

				if (nTmp == 0)
				{
					pStep->m_nAudioSource = pArgument->Integer;
				}
				else if (nTmp == 1)
				{
					pStep->m_nLeftFreq[0] = pArgument->Integer;
				}
				else if (nTmp == 2)
				{
					pStep->m_nRightFreq[0] = pArgument->Integer;
				}
				else if (nTmp == 3)
				{
					pStep->m_nLeftLevel[0] = pArgument->Integer;
				}
				else if (nTmp == 4)
				{
					pStep->m_nRightLevel[0] = pArgument->Integer;
				}
				else if (nTmp == 5)
				{
					pStep->m_nAudioMargin = pArgument->Integer;
				}
				nTmp++;
			}
		}//

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
	if(g_nGrabberType == FHD_GRABBER)
	{
		for(int i = 0; InputDefined_FHD[i].pszName != NULL; i++)
		{
			sTmp.Format("%s",InputDefined_FHD[i].pszName);
			sTmp.MakeUpper();

			if(sSource.Compare(sTmp) == 0)
			{
				bCheck = TRUE;
				break;
			}
		}
	}
	else{
		for(int i = 0; InputDefined_UHD[i].pszName != NULL; i++)
		{
			sTmp.Format("%s",InputDefined_UHD[i].pszName);
			sTmp.MakeUpper();

			if(sSource.Compare(sTmp) == 0)
			{
				bCheck = TRUE;
				break;
			}
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
		_strlwr_s(fun, strlen(fun) + 1);
		if (!strcmp(Predefined[nIndex].pszFunc, fun))
        {
  			if(strcmp(Predefined[nIndex].pszFunc, _T("work")) == 0)
			{
				pStep->m_bWorkFunc = TRUE;
			}

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
					else
					{
						for(int nIndex = 0; NameDefined[nIndex].pszName != NULL; nIndex++)
						{                             
							_strlwr_s(token, strlen(token) + 1);
							if (!strcmp(NameDefined[nIndex].pszName, token))
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
					strcpy_s(pArg->String, _countof(pArg->String), token);
				}
				else if (Predefined[nIndex].achPara[cbSyntex] == SIGN)
				{
					if (strstr(token, _T("+")) != NULL)	
						strcpy_s(pArg->String, _countof(pArg->String), _T("+"));
					else strcpy_s(pArg->String, _countof(pArg->String), _T("-"));
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
	pStep->m_InstructionList.AddTail(pFunction);
}

void MakeForceFunctionList(CStep* pStep, char* fun, char *CurString)
{
	char	seps[] = _T("(),\n");
	char*	token;
	char*	endptr;
	char	*stop;
	int		cbSyntex = 0;
	int		val;
	CString s;

	CFunction* pFunction = new CFunction;
	for (int nIndex = 0; Predefined[nIndex].pszFunc != NULL; nIndex++)
	{
		_strlwr_s(fun, strlen(fun) + 1);
		if (!strcmp(Predefined[nIndex].pszFunc, fun))
		{
			if (strcmp(Predefined[nIndex].pszFunc, _T("work")) == 0)
			{
				pStep->m_bWorkFunc = TRUE;
			}

			pFunction->m_pFunc = Predefined[nIndex].pFunc;
			CString str;
			str.Format("%s", CurString);
			CStringTokenizer *analizer = new CStringTokenizer(str);
			val = analizer->NextToken();
			while (Predefined[nIndex].achPara[cbSyntex] != TNULL && val != TT_EOF)
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
				if ((val == 40) || (val == 41) || (val == 44))
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
					else
					{
						for (int nIndex = 0; NameDefined[nIndex].pszName != NULL; nIndex++)
						{
							_strlwr_s(token, strlen(token) + 1);
							if (!strcmp(NameDefined[nIndex].pszName, token))
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
					strcpy_s(pArg->String, _countof(pArg->String), token);
				}
				else if (Predefined[nIndex].achPara[cbSyntex] == SIGN)
				{
					if (strstr(token, _T("+")) != NULL)
						strcpy_s(pArg->String, _countof(pArg->String), _T("+"));
					else strcpy_s(pArg->String, _countof(pArg->String), _T("-"));
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
			sTmp = MakeStepHeader(pStep);
			pFile.WriteString(sTmp);
			//Sub Step
			MakeSubStep(pStep, aTmp);
			if(pStep->m_nFuncType != PROCESS_FUNCTION){
				if(!pStep->m_bWorkFunc){
					sParserMsg.Format(_T("Step(%2d) : work function not found"), pStep->m_nStep);
					AfxMessageBox(sParserMsg);

					return FALSE;
				}
			}
			for(int nTmp1 = 0;nTmp1 <aTmp.GetSize(); nTmp1++ )
			{
				sTmp = aTmp.GetAt(nTmp1);
				pFile.WriteString(sTmp);
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

	
	sSubStep = _T("");
	POSITION Pos = pStep->m_InstructionList.GetHeadPosition();
	SortSubStep(pStep);
	aArray.RemoveAll();

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
			
			sSubStep.Format("\t %s", Predefined[nSelNo].pszFunc);

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
					sSubStep += ")\n\n";
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

					//	if(sSubStep.Mid(2,16).Compare("send_i2c_command") == NULL)
						if((Predefined[nSelNo].pszFunc) == _T("send_i2c_command"))
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
									case 116 : sTmp = "I2C_CHK_MAXMIN2";	break;
									case 117 : sTmp = "I2C_CHK_LIMIT2";	break;
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
			aArray.Add(sSubStep);
			
			
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
					if(Predefined[nTmp1].pszFunc == _T("work"))
					{
						pStep->m_bWorkFunc = TRUE;
					}

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
					else if(Predefined[nTmp1].pszFunc == _T("checkaudio_level_nofreq"))
					{
						bIsCheckAudio = TRUE;
						nCheckAudioType = FREQ_LEVEL_CHECK_MOVING;
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
						//	if(sSubStep.Mid(2,16).Compare("send_i2c_command") == NULL)
							if((Predefined[nSelNo].pszFunc) == _T("send_i2c_command"))
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
										case 116 : sTmp = "I2C_CHK_MAXMIN2";	break;
										case 117 : sTmp = "I2C_CHK_LIMIT2";	break;
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
						//	if(sSubStep.Mid(2,15).Compare("set_dimmingtest") == NULL)
							if((Predefined[nSelNo].pszFunc) == _T("set_dimmingtest"))
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

		if (sign == '-') pToken->value.f = (float)-value;
		return TRUE;
	}
    else if (pToken->type == REAL) value = pToken->value.f;
	else if(pToken->type == INTEGER) value = pToken->value.i;
	else return FALSE;
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
