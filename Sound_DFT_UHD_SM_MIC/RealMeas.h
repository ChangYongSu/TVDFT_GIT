#ifndef __REALMEAS_H__
#define __REALMEAS_H__

#define ROI_USER_DEFINE		10
#define ROI_FULL_SCREEN		0
#define ROI_HALF_UP_SCREEN	1
#define ROI_HALF_DN_SCREEN	2
#define ROI_LEFT_UP_SCREEN	3
#define ROI_LEFT_DN_SCREEN	4
#define ROI_RIGHT_UP_SCREEN	5
#define ROI_RIGHT_DN_SCREEN	6
#define ROI_CENTER_SCREEN	7



BOOL _UserMessage();
BOOL _NotifyMessage();
BOOL _Delay();
BOOL _Check_Delay();
BOOL _Check_Again_Delay();
BOOL _Work();
BOOL _Wait(int nMillisecond);
BOOL _PatternGen_Out();
BOOL _Remote_Out();
BOOL _SetInputSource();
BOOL _SetInput();
BOOL _SetRf();
BOOL _SetRfChannel();
BOOL _SetTVCommand();
BOOL _SendAdcCheck();
BOOL _SendTVCommand();
BOOL _CheckAdcResult();
BOOL _SetTvCommPortEcho();
BOOL _Power_On_Check();
BOOL _Power_Off_Check();
BOOL _Audio_Open();
BOOL _Grab_N_Load_Image();		
BOOL _Grab_Image_Check();
BOOL _Grab_FullImage_Check();
BOOL _SetLvdsResolution_2in1();
//+add PSH 080412
BOOL _Grab_Image_OSD_Check();

BOOL _SendI2cCommand();
//+add PSH 101029
BOOL _Set_3D_Grab();

BOOL _DMM_SetMeasureChannel();
BOOL _DMM_SetMultiMeasureChannel();

BOOL _VF_SetMeasureChannel();
BOOL _VF_SetMeasure_MinMax();

//==================
// I2C ADC Function
//==================

///+del kwmoon 080804
/*
BOOL CheckI2cValue(int nData1);
BOOL CheckI2cMaxMin(int nData1);
BOOL CheckI2cLimit(int nData1);
BOOL ReadI2cData();
BOOL ReadI2c2bData();
BOOL WriteI2c2bData();
*/


//+change kwmoon 080321
BOOL _SetToolOption();
BOOL _SetAreaOption();

BOOL _SetManualTestMessage();
BOOL _SetCaptionSize();
BOOL _SetCaptionKey();
BOOL _SetCaptionCheckTime();
BOOL _SetDigitalEyeDelay();
BOOL _NotSkipSameSourceChannel();
BOOL _SetPositionShift();
BOOL _UseOnlyColorHighBits();
BOOL _UseOnlyBrightHighBits();
BOOL _SetMatchRate();
BOOL _SetRefPositon();
BOOL _SetAvgFilter();
BOOL _SetAnalogVideoSource();
BOOL _SetRoi();	
BOOL _SetAvSwitchBoxSignal();
BOOL _CheckAudio();
BOOL _AuthenticationKey();
BOOL _SkippedSignalLineLowBits();

BOOL _ToolOption_Write();
BOOL ToolOption_WriteCheck();
BOOL _AreaOption_Write();
BOOL _ToolOption_WriteAll();

//+add 090415
BOOL _TV_Mic_On();
BOOL _TV_Mic_Off();
//+add 101108
BOOL _TV_LNB_On();
BOOL _TV_LNB_Off();
//+add 101124
BOOL _TV_3DMode_On();
BOOL _TV_3DMode_Off();

//+add 090305
BOOL _CheckAudio_Level();
BOOL _CheckAudio_Frequency();
BOOL _CheckAudio_Level_NoFrequency();
BOOL _CheckAudio_Time_Level();
BOOL _CheckAudio_Time_Frequency();

//+add 110610
BOOL _TV_HeadPhone_On();
BOOL _TV_HeadPhone_Off();

//=================
// Global Function
//=================
BOOL RefImage_InternalLoad(CString szFileName,int nUseMaskImage, CString sMsg);
BOOL SetAudioSource(int nAudioSource);
BOOL Grab_Image_Check_Normal();
BOOL Grab_Image_Check_MakeRef();
BOOL Grab_N_Load_Image_Normal();		// Grab Image & Load Reference Image
BOOL Grab_N_Load_Image_MakeRef();		// Grab Image & Load Reference Image
BOOL Work_Normal();
BOOL Work_MakeRef();
BOOL GetCurrentPatternNo(int* pPatternNo);

//+add PSH 080412 
UINT Grab_Image_Check_OSD_2in1();
UINT Grab_Image_Check_OSD_MakeRef_2in1();
//+change kwmoon 080319 
//BOOL Grab_Image_Check_2in1(CString sGrabTmp,CString sRefTmp,int nUseMaskImage,int nMaxDelay, CString sMsg);
//BOOL Grab_Image_Check_Normal_2in1();
//BOOL Grab_Image_Check_MakeRef_2in1();
UINT Grab_Image_Check_2in1(CString sGrabTmp,CString sRefTmp,int nUseMaskImage,int nMaxDelay, CString sMsg);
UINT Grab_Image_Check_Normal_2in1();
UINT Grab_Image_Full_Check_Normal_2in1();
UINT Grab_Image_Check_MakeRef_2in1();

//+change kwmoon 080319 
// -> change return type
// BOOL Grab_N_Load_Image_Normal_2in1();		// Grab Image & Load Reference Image
// BOOL Grab_N_Load_Image_MakeRef_2in1();		// Grab Image & Load Reference Image
// BOOL Grab_Image_Check_AdjSpec_2in1();
UINT Grab_N_Load_Image_Normal_2in1();		// Grab Image & Load Reference Image
UINT Grab_N_Load_Image_MakeRef_2in1();		// Grab Image & Load Reference Image

//+del kwmoon 080703
//UINT Grab_Image_Check_AdjSpec_2in1();

BOOL Grab_Image_UHD(CString sRefImagePath,int nUseMaskImage);
BOOL Grab_Image_FHD(CString sRefImagePath,int nUseMaskImage);

BOOL _SetOsdRegion();
//+add kwmoon 080826
BOOL _SetInvalidRegion();

//+add kwmoon 080826

//+add PSH 080516
BOOL _PatternGenerator_HSync_On(); 
BOOL _PatternGenerator_VSync_On(); 
BOOL _PatternGenerator_RGB_On(); 
BOOL _PatternGenerator_HSync_Off(); 
BOOL _PatternGenerator_VSync_Off(); 
BOOL _PatternGenerator_RGB_Off(); 

//+add PSH 080522
BOOL _TVLink_SetChannel_PAL();
BOOL _RS232C_TVControl_Command();

//+add kwmoon 080523
BOOL _SetAudioCheckMethod();

//+add kwmoon 080621
void Set_Grab_Image_Check_Roi(int nType);

//+add PSH 090129
BOOL _HDMIGen_SetEDID();
BOOL _HDMIGen_SetHDCP();
BOOL _HDMIGen_SetOut();
BOOL _HDMIGen_SetCEC();
BOOL _HDMIGen_SetARC();
BOOL _HDMIGen_SetMHL();
BOOL _HDMIGen_SetDDCLine();

//+add PSH 090417
BOOL _CheckAdcResult2();
//add 090527
BOOL _Set_DimmingTest();

//add 101221
BOOL _AudioMeasureReset();
BOOL _AudioSelectReset();
BOOL _AudioListSearch();
BOOL _AudioNewMicCheck();

//add 110215
BOOL _Set_MacAddress();
BOOL _Set_USBHighSpeed();
BOOL _Set_CountryGroup();

BOOL _AC_Power_OnOff();
BOOL _MHL_BufferControl();
BOOL _MHL_DetectLineControl();
BOOL _SetDetectCheckSource();
BOOL _Grabber_Reset();
BOOL _Grabber_HW_Reset();
BOOL _AVC_BufferClear();
BOOL _Set_HdmiGrab();
BOOL _Read_KeyItem();
BOOL _Epi_Pack_Reset();
BOOL _Read_PCBAID_READ();
BOOL _Set_CylinderComp();
BOOL _Set_CylinderEarphone();
BOOL _Set_Cylinder();
BOOL _Set_Cylinder3Bit();
BOOL _IsFailePowerOn();
BOOL _SetMovingPicTestMargin();

BOOL _VF_MeasureStart();
BOOL _RotateDisplay();
BOOL _SetTVBuadRate();

BOOL _Mnt_Edid_Source();
BOOL _GrabStop();
BOOL _GrabToggle();
BOOL _LAN_HDCP_Clear();

#endif	// __REALMEAS_H__