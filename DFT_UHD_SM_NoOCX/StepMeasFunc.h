#ifndef __STEPMEASFUNC_H__
#define __STEPMEASFUNC_H__

double	_Color_Test();
double	_Bright_Test();
int	_Dark_Test();
int _White_Test();
double	_Position_Test();
double	_Osd_Test();
double	_Size_Test();
double	_Noise_Test();
double  _Lan_Test();
double  _TVComm_Test();
double	_Signal_Line_Test();
double	_Audio_Test();
double  _Moving_Pic_Test();
double  _Manual_Test();
double  _Caption_Test();
double  _Digital_Eye_Test();
double  _Adc_Test();
//double  _ADC_AdjConfirmation();
double  _ADC_AdjResultCheck();
//double  _EDID_DownloadResultCheck();
double  _Color_Bias_Test();
double	_Version_Check();
double	_MNT_Version_Check();
BOOL	Check_ToolOption(int nOptionNo, int nOptionValue);
double	_Option_Check();
double	_Option_CheckAll();

double	_AreaCode_Check();
double	_CountryGr_Check();

//+add 080216(Modification No1)
double _Module_Voltage_Test();
//+add 090304(Modification No1)
double _EDID_Download();
//+add 090325(Modification No1)
double _LAN_Port_Check();
double _ESN_Download_Check();
//add 090527
double _Domming_Check();
//add 091118
double _CI_Key_Check();
//add 101011
double _CI_Card_Check();
double _Marlin_Download_Check(); 
double _BuiltIn_Wifi_Check(); 

//add 091124
double _RJP_Port_Check();
double _Magic_Remocon_Check();
double _Wireless_Ready_Check();

double _DMM_MeasureDC();
double _DMM_MeasureAC();
double _DMM_MeasureResistance();
//double _DMM_GetMonitoringData();
double _DMM_MeasureFrequency();
//double _DMM_MeasurePeriod();
double _DMM_DischargCheck();

double _VF_MeasureVolt();
double _VF_MeasureFreq();
double _VF_MeasureVolt_MinMax();
double _VF_MeasureFreq_MinMax();
double _VF_DischargCheck();



//add 101108
double _Widevine_Download_Check(); 
//add 101115
double _RF_Emitter_Check();
//add 110215
double _MAC_Address_Check();
double _USB_HighSpeed_Check();
double _BCM_Chip_Check();
double _Key1_Check();
double _Key2_Check();
double _EyeQ_Check();
double _BAT_Ver_Check();
double _CNTV_Key_DL_Check();
double _HDCP_Tx_Key_DL_Check();
double _HDCP_Rx_Key_DL_Check();
double _DTCP_Rx_Key_DL_Check();
double _ATSC_30_Key_DL_Check();
double _MFi_Key_DL_Check();
double _CountryGr_BR_Write();
double _CountryGr_KR_Write();
double _CountryGr_JA_Write();
double _Google_Key_DL_Check();
//double _HdcpKey_Download(); 
//double _HdcpKey_Download_Check(); 

double _LNB_MeasureVoltage();
double _LNB_MeasureFrequency();
double _LNB_MeasureVoltage2();
double _LNB_MeasureFrequency2();
double _Input_Detect_Check();
double _AVC_ComDataCheck();
double _AVC_IRDataCheck();
double _AVC_IRBlasterDataCheck();
double _Drm_Key_DL_Check();
double _Input_Detect_AllCheck();
double _VF_Test_ResultCheck();
double _VF_Measure_Check();
#ifdef SM_MODIFY_CODE__
double _HP_Disable_Check();
double _Port_Auto_Check();
double _IR_Blaster_Check();
double _Read_A_CAS_ID();
#endif

#endif	// __REALMEAS_H__