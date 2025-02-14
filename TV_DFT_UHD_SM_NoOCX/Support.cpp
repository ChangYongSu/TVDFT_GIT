#include "stdafx.h"
#include "resource.h"
#include "parser.h"
#include "step.h"
#include "global.h"
#include "support.h"		
#include "ColorStaticST.h"	
#include "vsflexgrid.h"
#include "PatternGeneratorCtl.h"
#include "TVCommCtrl.h"
#include "AvSwitchBoxCtrl.h"
#include "ScannerCtrl.h"
#include "RealMeas.h"
#include "I2cAdcCtrl.h"
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
//#include "HwCtrl\AnalogGrabber.h"
#include "CurrentSetting.h"
#include "SoundCard.h"
#include "CSpreadSheet.h"
#include "HdmiGenerator.h"
//#include "MesDb.h"
#include "LnbDetectorCtl.h"
#include "XDischargDlg.h"

#include "AvConvertorCtrl.h"
#include "Gmes.h"
#include "VF1000.h"
#include "PLC_Ctrl.h"



//extern CMesDb		m_MesDb;
//extern CVSFlexGrid			ctrlSummaryGrid;
extern UINT					nDefectCurSel;
extern CColorStaticST		ctrlTestSubStep;
extern CColorStaticST		ctrlStepTarget_V;
extern CColorStaticST		ctrlStepMeasur_V;
extern CColorStaticST		ctrlStepTarget_A;
extern CColorStaticST		ctrlStepMeasur_A;
//extern int					g_nNoFailedStep;
extern CDATsysView*			g_pView;
extern _InputSource			InputDefined_FHD[];
extern _InputSource			InputDefined_UHD[];
extern CPatternGenerator	PatternGeneratorCtrl;
extern CTVCommCtrl			TVCommCtrl;
extern CAvSwitchBox			AvSwitchBoxCtrl;
extern CScannerCtrl			ScannerCtrl;
extern CI2cAdcCtrl			I2cAdcCtrl;
extern CString				m_strWipId;	//Wip ID
extern UINT					g_GrabSource;
extern UINT					g_VideoSourceType;
extern CCurrentSetting		g_CurSetting;
extern CSoundCard			g_SoundCard;
//+add 090210(Modification No1)
extern CBaseImage			g_GrabImage;

extern CRITICAL_SECTION g_cs4;  

extern CStringArray				g_strLogArray;
extern int g_Using;
//int g_Using1;

//+add 090409(Modification No1)
int g_nCurSwitchBoxLogLine;
int g_nCurRemoteLogLine;

int limit;
clock_t t;

int GetStepNo()
{
	return pCurStep->GetStepNo();
}

double GetNominal()
{
	return pCurStep->GetNominal_float();
}

double GetHighLimit()
{
	return pCurStep->GetHighLimit();
}

double GetLowLimit()
{
	return pCurStep->GetLowLimit();
}

BOOL CheckValue(double value)
{
	//----------------//
	// BOOL TYPE STEP //
	//----------------//
	if(pCurStep->GetFuncType() == MEAS_BOOL)
	{
		if(pCurStep->m_nTestType == AUDIO_TEST)
		{
			pCurStep->SetResult(TRUE); return TRUE;
		}

		if(value == 1.0)
		{
			pCurStep->m_bMeasured = TRUE;
		}
		else
		{
			pCurStep->m_bMeasured = FALSE;
		}

		if (TRUE != pCurStep->m_bMeasured)//if (pCurStep->GetNominal_Bool() != pCurStep->m_bMeasured)
		{
			pCurStep->SetResult(FALSE); 
			return FALSE;
		}
		else
		{
			pCurStep->SetResult(TRUE); return TRUE;
		}	
	}
	//----------------//
	// REAL TYPE STEP //
	//----------------//
	else
	{
		pCurStep->m_fMeasured = value;

		//-----------------------//
		// RGB_TEST / NOISE_TEST //
		//-----------------------//
		if((pCurStep->m_nTestType == RGB_TEST) || (pCurStep->m_nTestType == NOISE_TEST))
		{
			//+add kwmoon 080716
			if(pCurStep->m_bResult)
			{
				return TRUE;
			}
			else 
			{
				return FALSE;
			}
		}

		if (pCurStep->GetHighLimit() < value || pCurStep->GetLowLimit() > value) 
		{
			pCurStep->SetResult(FALSE); 
			return FALSE;
		}
		else 
		{
			pCurStep->SetResult(TRUE);
		}
	}

	return TRUE;
}

void AddStringToStatus(CString szString)
{

	
#ifdef _EXIT_DEBUG
		::SendMessage(g_pView->m_hWnd, WM_MASSAGE_LOG_DISPLAY, (WPARAM)szString.GetLength(), (LPARAM)szString.GetBuffer());//WPARAM wParam, LPARAM lParam
	
	//CString stmp;
	//
	////if(g_pView->m_bWriteMassageEnable != 0)
	////{
	//	//if (g_pView->m_LogBackupString.GetLength() > 0)
	//	//{
	//	//	g_pView->m_LogString += g_pView->m_LogBackupString;
	//	//}
	///*	int len = g_pView->m_LogString.GetLength();
	//	if (g_pView->m_LogString.GetLength() > 0)
	//	{
	//		stmp = _T("#/");
	//		szString = szString + stmp;
	//	}
	//	g_pView->m_LogString = g_pView->m_LogString + szString;

	//	if (g_pView->m_LogString.GetLength() > 512)
	//	{
	//		g_pView->m_LogString = "";
	//	}*/
	//	::SendMessage(g_pView->m_hWnd, WM_MASSAGE_LOG_DISPLAY, (WPARAM)szString.GetLength(), (LPARAM)szString.GetBuffer());//WPARAM wParam, LPARAM lParam
	////	g_pView->m_bWriteMassageEnable = 0;
	////}
	//
	////else
	////{
	////	if (g_pView->m_LogBackupString.GetLength() > 0)
	////	{
	////		g_pView->m_LogBackupString += "#/";
	////	}
	////	g_pView->m_LogBackupString += szString;

	////	if (g_pView->m_LogBackupString.GetLength() > 512)
	////	{
	////		g_pView->m_LogBackupString = "";
	////	}
	////	
	////}
	//

#else
	szString.TrimRight(0x0a);
	szString.TrimRight(0x0d);

	g_pView->ComLog_AddString( szString );
	

	if((g_pView->m_I2cAdcManualCheckDlg.GetSafeHwnd() != NULL)
	&& (g_pView->m_I2cAdcManualCheckDlg.m_bActive == TRUE))
	{
		g_pView->m_I2cAdcManualCheckDlg.WriteLog(I2cAdcCtrl.m_szCurrentStatusMsg);
	}

	if((g_pView->m_Rs232cAdcManualCheckDlg.GetSafeHwnd() != NULL)
	&& (g_pView->m_Rs232cAdcManualCheckDlg.m_bActive == TRUE))
	{
		g_pView->m_Rs232cAdcManualCheckDlg.WriteLog(szString);
	}  
#endif
}

void AddStringToRemoteComLog(CString szString)
{
	AddStringToStatus( szString);// //g_pView->ComLog_AddString( szString );

}

/*
void InitComLogGrid()
{
	//////////////////////////
	// Remote Com Log
	//////////////////////////

	ctrlRemoteComLogGrid.SetRedraw(FALSE);
	ctrlRemoteComLogGrid.SetAllowBigSelection(FALSE);
	ctrlRemoteComLogGrid.SetExtendLastCol(TRUE);

	ctrlRemoteComLogGrid.SetBackColorBkg(COLOR_WHITE);
	ctrlRemoteComLogGrid.SetForeColorFixed(RGB(51,102,153));
	ctrlRemoteComLogGrid.SetBackColor(RGB(247,247,231));
	ctrlRemoteComLogGrid.SetGridColor(RGB(210,216,176));

	//Fixed Rows & Cols
	ctrlRemoteComLogGrid.SetFixedRows(0);
	ctrlRemoteComLogGrid.SetFixedCols(0);
	//Grid Line Setting
	ctrlRemoteComLogGrid.SetGridLines(flexGridFlat);
	//Column Width
	ctrlRemoteComLogGrid.SetColWidth(0, 6000);
	//Alignment
	ctrlRemoteComLogGrid.SetColAlignment(0,flexAlignLeftCenter);
	//Rows & Cols Number
	ctrlRemoteComLogGrid.SetCols(1);

	//+change 090409(Modification No1)
//	ctrlRemoteComLogGrid.SetRows(0);
	ctrlRemoteComLogGrid.SetRows(100);
	
	ctrlRemoteComLogGrid.SetRedraw(TRUE);
	ctrlRemoteComLogGrid.Refresh();

	//////////////////////////
	// Av SwitchBox Com Log
	//////////////////////////

	ctrlSwitchBoxComLogGrid.SetRedraw(FALSE);
	ctrlSwitchBoxComLogGrid.SetAllowBigSelection(FALSE);
	ctrlSwitchBoxComLogGrid.SetExtendLastCol(TRUE);

	ctrlSwitchBoxComLogGrid.SetBackColorBkg(COLOR_WHITE);
	ctrlSwitchBoxComLogGrid.SetForeColorFixed(RGB(51,102,153));
	ctrlSwitchBoxComLogGrid.SetBackColor(RGB(247,247,231));
	ctrlSwitchBoxComLogGrid.SetGridColor(RGB(210,216,176));

	//Fixed Rows & Cols
	ctrlSwitchBoxComLogGrid.SetFixedRows(0);
	ctrlSwitchBoxComLogGrid.SetFixedCols(0);
	//Grid Line Setting
	ctrlSwitchBoxComLogGrid.SetGridLines(flexGridFlat);
	//Column Width
	ctrlSwitchBoxComLogGrid.SetColWidth(0, 6000);
	//Alignment
	ctrlSwitchBoxComLogGrid.SetColAlignment(0,flexAlignLeftCenter);
	//Rows & Cols Number
	ctrlSwitchBoxComLogGrid.SetCols(1);

	//+change 090409(Modification No1)
//	ctrlSwitchBoxComLogGrid.SetRows(0);
	ctrlSwitchBoxComLogGrid.SetRows(100);
	
	ctrlSwitchBoxComLogGrid.SetRedraw(TRUE);
	ctrlSwitchBoxComLogGrid.Refresh();

}
*/

void SetTimeLimit(int nTime)
{
	limit = nTime;
	t = clock();
}

BOOL IsTimeOut()
{
	if ((double)(clock() - t) / CLOCKS_PER_SEC > limit) 
	{
		AddStringToStatus(_T("Time Out."));
		return FALSE;
	}
	return TRUE;
}

void CreateFullPath(CString sFullPath)
{
	int nPos2 = 0;

	int nPos = sFullPath.Find('\\');
	CString csTmp = sFullPath.Mid(0, nPos);

	while(1)
	{
		nPos2 = sFullPath.Find('\\', nPos + 1);
		if(nPos2 < 0)
		{
			::CreateDirectory((LPCTSTR)sFullPath, NULL);
			SetFileAttributes(csTmp, FILE_ATTRIBUTE_NORMAL);
			break;
		}
		csTmp = sFullPath.Mid(0, nPos2);
		nPos = nPos2;
		::CreateDirectory((LPCTSTR)csTmp, NULL);
		SetFileAttributes(csTmp, FILE_ATTRIBUTE_NORMAL);
	}
}

BOOL CreateModelIniFile(CString sModelIni)
{
	//+del 081024
//	CurrentSet->sModelIni = sModelFolder + "\\" + "ModelInfo.Ini";
	CString sModelInfoFilePath = sModelIni; 

	CFile pFile;
	CFileException FileExcept;
	CString sTmp;

	//+change kwmoon 081024
//	if(!pFile.Open(CurrentSet->sModelIni, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &FileExcept)) 
	if(!pFile.Open(sModelInfoFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &FileExcept)) 
	{
		char szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	//+change kwmoon 081024
//	m_Ini.SetIniFile(CurrentSet->sModelIni);
	m_Ini.SetIniFile(sModelInfoFilePath);

	//=========================
	// Grabber Resolution Info
	//=========================
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Height",CurrentSet->nLvdsHeight);
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Width" ,CurrentSet->nLvdsWidth);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Height",CurrentSet->nAnalogHeight);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Width",CurrentSet->nAnalogWidth);
	m_Ini.SetProfileInt(GRAB_S, "LVDS 30P Robot Skip FFC",CurrentSet->bRobotSkipFFC);
	CurrentSet->bRobotSkipFFC = 1;
	m_Ini.SetProfileInt(GRAB_S, "Image Full Reset Option",CurrentSet->bCheckImageFullReset);
	m_Ini.SetProfileInt(GRAB_S, "Image OLED Reset Option",CurrentSet->bCheckOledReset);
	
	
	if(g_nGrabberType == FHD_GRABBER){ 
		m_Ini.SetProfileInt(GRAB_S, "Uranous Grabber Mode",CurrentSet->nUranousMode);
	}
	m_Ini.SetProfileInt(GRAB_S, "Color Bit Count",CurrentSet->nBitCount);
	m_Ini.SetProfileInt(GRAB_S, "FullHD120Hz",CurrentSet->nFullHd120Hz);
	m_Ini.SetProfileInt(GRAB_S, "ImageRotation",CurrentSet->nImageRotation);
	m_Ini.SetProfileInt(GRAB_S, "Red Blue Swap", CurrentSet->nRedBlueSwap);

	m_Ini.SetProfileInt(GRAB_S, "DE to HSYNC Enable", CurrentSet->nDeHsyncEnable);
	m_Ini.SetProfileInt(GRAB_S, "HSYNC Edge Enable", CurrentSet->nHyncEdgeEnable);



	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Type",CurrentSet->nAnalogType);

#ifdef SM_MODIFY_CODE__
	m_Ini.SetProfileInt(GRAB_S, "Grab Image Check Area", CurrentSet->nGrabCheckArea);
#endif

	if(g_nGrabberType == UHD_GRABBER){
		m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Mode",CurrentSet->nLVDS_Mode);
		m_Ini.SetProfileInt(GRAB_S, "UHD Type",CurrentSet->nUHD_Type);
		m_Ini.SetProfileInt(GRAB_S, "UHD Grab BitShift",CurrentSet->nUHD_Grab_BitShift);
		m_Ini.SetProfileInt(GRAB_S, "UHD Grab Mode",CurrentSet->nUHD_Grab_Mode);
		m_Ini.SetProfileInt(GRAB_S, "UHD Grab Delay",CurrentSet->nUHD_Grab_Delay);
		m_Ini.SetProfileInt(GRAB_S, "UHD Y20 SW Mode",CurrentSet->nUHD_Y20_SW_Mode);

		m_Ini.SetProfileInt(GRAB_S, "HDMI Grab Resolution",CurrentSet->nHDMI_Grab_Resolution);
		m_Ini.SetProfileInt(GRAB_S, "HDMI Grab BitShift",CurrentSet->nHDMI_Grab_BitShift);
		m_Ini.SetProfileInt(GRAB_S, "HDMI Out Port Control",(UINT)CurrentSet->bHdmiOutControl);
		m_Ini.SetProfileInt(GRAB_S, "HDMI Grab Mask OSD",(UINT)CurrentSet->bHdmiGrab_MaskOsd);
	}


	//==================
	// Tool/Area Option
	//==================
	m_Ini.SetProfileString(TOOL_AREA_S, "Current Model Suffix","");
	m_Ini.SetProfileInt(TOOL_AREA_S, "Organization ID",0);

	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option1",CurrentSet->sToolOption1);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option2",CurrentSet->sToolOption2);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option3",CurrentSet->sToolOption3);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option4",CurrentSet->sToolOption4);
	//091016
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option5",CurrentSet->sToolOption5);
	//101011
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option6",CurrentSet->sToolOption6);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option7",CurrentSet->sToolOption7);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option8",CurrentSet->sToolOption8);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool CRC",CurrentSet->sToolCRC);
	m_Ini.SetProfileInt(TOOL_AREA_S, "ToolOption Write", CurrentSet->bToolOptionWriteFlag);
	
	m_Ini.SetProfileString(TOOL_AREA_S, "Area Option1",CurrentSet->sAreaOption1);

	m_Ini.SetProfileString(TOOL_AREA_S, "Country Group",CurrentSet->sCountryGroup);
	m_Ini.SetProfileString(TOOL_AREA_S, "Country Code",CurrentSet->sCountry);

	m_Ini.SetProfileString(TOOL_AREA_S, "Commercial Option1",CurrentSet->sCommercialOption1);
	m_Ini.SetProfileString(TOOL_AREA_S, "BOARD Option", CurrentSet->sBoardOption);
	m_Ini.SetProfileString(TOOL_AREA_S, "Commercial Board Option", CurrentSet->sCommercialBoardOption);

	m_Ini.SetProfileInt(TOOL_AREA_S, "Tool Option Chek Delay", CurrentSet->nToolOptionCheck_Delay);



	m_Ini.SetProfileString(VERSION_CHECK_S, "CPU Version",CurrentSet->sCPUVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Micom Version",CurrentSet->sMicomVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "USB Version",CurrentSet->sUSBVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "MNT Version",CurrentSet->sMNTVersion);

	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI1",	CurrentSet->sEDID_HDMI1);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI2",	CurrentSet->sEDID_HDMI2);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI3",	CurrentSet->sEDID_HDMI3);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI4",	CurrentSet->sEDID_HDMI4);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI5",	CurrentSet->sEDID_HDMI5);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_DP",		CurrentSet->sEDID_DP);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_DP2",	CurrentSet->sEDID_DP2);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_DVI",	CurrentSet->sEDID_DVI);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_PC",		CurrentSet->sEDID_PC);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_RGB",	CurrentSet->sEDID_RGB);
                                                                                            
	m_Ini.SetProfileString(SPEC_CHECK_S, "CI",		CurrentSet->sCI);
	m_Ini.SetProfileString(SPEC_CHECK_S, "CNTV",		CurrentSet->sCNTV);
	m_Ini.SetProfileString(SPEC_CHECK_S, "DTCP_RX",		CurrentSet->sDTCP_RX);
	m_Ini.SetProfileString(SPEC_CHECK_S, "DVBS",		CurrentSet->sDVBS);
	m_Ini.SetProfileString(SPEC_CHECK_S, "DVBS2",		CurrentSet->sDVBS2);
	m_Ini.SetProfileString(SPEC_CHECK_S, "ESN",		CurrentSet->sESN);
	m_Ini.SetProfileString(SPEC_CHECK_S, "HDCP_RX",		CurrentSet->sHDCP_RX);
	m_Ini.SetProfileString(SPEC_CHECK_S, "HDCP_RX_14",	CurrentSet->sHDCP_RX_14);
	m_Ini.SetProfileString(SPEC_CHECK_S, "HDCP_TX",		CurrentSet->sHDCP_TX);
	m_Ini.SetProfileString(SPEC_CHECK_S, "JAPAN_LNB",	CurrentSet->sJAPAN_LNB);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MAC_ADDRESS",	CurrentSet->sMAC_ADDRESS);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MARINKEY",	CurrentSet->sMARINKEY);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MODEL_NAME",	CurrentSet->sMODEL_NAME);
	m_Ini.SetProfileString(SPEC_CHECK_S, "SERIAL_NO",	CurrentSet->sSERIAL_NO);
	m_Ini.SetProfileString(SPEC_CHECK_S, "WASU",		CurrentSet->sWASU);
	m_Ini.SetProfileString(SPEC_CHECK_S, "WIDEVINE",	CurrentSet->sWIDEVINE);
	m_Ini.SetProfileString(SPEC_CHECK_S, "ADC",		CurrentSet->sADC);
	m_Ini.SetProfileString(SPEC_CHECK_S, "BUILT_WIFI",	CurrentSet->sBUILT_WIFI);
	m_Ini.SetProfileString(SPEC_CHECK_S, "CA", CurrentSet->sCA);
	m_Ini.SetProfileString(SPEC_CHECK_S, "ATSC30_KEY", CurrentSet->sASTC30_Key);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MFI", CurrentSet->sMfi_Key);
	m_Ini.SetProfileString(SPEC_CHECK_S, "CI+ECP", CurrentSet->sCI_CEP_Key);

	//+add psh 080701
	m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate", CurrentSet->wTVBaudRate);

	m_Ini.SetProfileInt(GENERAL_S, "Scanner Skip", CurrentSet->bScanNotUse);
	m_Ini.SetProfileInt(GENERAL_S, "DP Time Set", CurrentSet->nDP_TimeSel);
	m_Ini.SetProfileInt(GENERAL_S, "DPMS Sample Rate", CurrentSet->nDPMS_SampleRate);
	
	
	//============
	// I2C Option
	//============
	m_Ini.SetProfileInt(I2C_SETTING_S, "Use I2C",CurrentSet->bUseI2cAdc);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Clock Speed",CurrentSet->nI2cClkSpeed);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Packet Delay",CurrentSet->nI2cPacketDelay);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Read Byte Delay",CurrentSet->nI2cReadByteDelay);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Write Byte Delay",CurrentSet->nI2cWriteByteDelay);

	//============
	// TV Control
	//============
	m_Ini.SetProfileInt(TV_CONTROL_S, "TV Control Type",CurrentSet->nTVControlType);
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Source Control",CurrentSet->bAutoSourceControl);
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Pattern Control",CurrentSet->bAutoPatternControl);
	//add 20101012
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto AVSwitchBox Control",CurrentSet->nAutoAVSwitchBoxControl);

	//+add psh 100310
	//============
	// HDMI Gen. Control
	//============
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Resolution",CurrentSet->nHDMIResolution);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Color Bit Count",CurrentSet->nHDMIBitSpec);

	m_Ini.SetProfileInt(HDMI_FORMAT_S, "CEC On/Off Control", CurrentSet->bHdmiCecControl);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "EDID On/Off Control", CurrentSet->bHdmiEdidControl);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDCP On/Off Control", CurrentSet->bHdmiHdcpControl);

	// add 101029
	m_Ini.SetProfileInt(GRAB_S, "3D Grabbing Mode",CurrentSet->n3DGrabbingMode);

	//HDCP-Key DOwnload
	m_Ini.SetProfileString(HDCP_KEYDN_S, "Key Class Name",CurrentSet->sHDCP_Key_Class);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Key Class No",CurrentSet->nHDCP_Key_Class);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Data SIze",CurrentSet->nHDCP_Data_Size);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Start ADH",CurrentSet->nHDCP_Start_ADH);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Start ADL",CurrentSet->nHDCP_Start_ADL);

	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_AV1",       CurrentSet->sJACK_AV1);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_AV2",       CurrentSet->sJACK_AV2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_SCART",     CurrentSet->sJACK_SCART);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_COMP1",     CurrentSet->sJACK_COMP1);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_COMP2",     CurrentSet->sJACK_COMP2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB1",      CurrentSet->sJACK_USB1);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB2",      CurrentSet->sJACK_USB2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB3",      CurrentSet->sJACK_USB3);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB4",      CurrentSet->sJACK_USB4);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HEAD_PHONE",CurrentSet->sJACK_HEAD_PHONE);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI1",     CurrentSet->sJACK_HDMI1);;
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI2",     CurrentSet->sJACK_HDMI2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI3",     CurrentSet->sJACK_HDMI3);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI4",     CurrentSet->sJACK_HDMI4);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_RGB",       CurrentSet->sJACK_RGB);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_DVI",       CurrentSet->sJACK_DVI);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_DP",        CurrentSet->sJACK_DP);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_SMHL",      CurrentSet->sJACK_SMHL);

	m_Ini.SetProfileString(SPEC_CHECK_S, "BAT_VERSION",      CurrentSet->sBatVersion);
//	m_Ini.SetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET", CurrentSet->bEpiPAckReset);
//	m_Ini.SetProfileInt(SPEC_CHECK_S, "GRAB_BASE_RESET", CurrentSet->bGrabBaseReset);
	return TRUE;
}

//=======================================
// Save setting values to Model.ini file
//=======================================
BOOL SaveModelIniFile(CString sIniPath)
{
	CFile pFile;
	CFileException FileExcept;
	CString sTmp;

	//+change kwmoon 080910
//	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::modeWrite, &FileExcept)) 
	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone, &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	m_Ini.SetIniFile(sIniPath);

	//=========================
	// Grabber Resolution Info
	//=========================
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Height",CurrentSet->nLvdsHeight);
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Width" ,CurrentSet->nLvdsWidth);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Height",CurrentSet->nAnalogHeight);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Width",CurrentSet->nAnalogWidth);
	m_Ini.SetProfileInt(GRAB_S, "LVDS 30P Robot Skip FFC", CurrentSet->bRobotSkipFFC);
	m_Ini.SetProfileInt(GRAB_S, "Image Full Reset Option", CurrentSet->bCheckImageFullReset);
	m_Ini.SetProfileInt(GRAB_S, "Image OLED Reset Option", CurrentSet->bCheckOledReset);

	if(g_nGrabberType == FHD_GRABBER){
		m_Ini.SetProfileInt(GRAB_S, "Uranous Grabber Mode",CurrentSet->nUranousMode);
	}
	m_Ini.SetProfileInt(GRAB_S, "LVDS OddDE Only",CurrentSet->bOddDE_Only);
	m_Ini.SetProfileInt(GRAB_S, "Color Bit Count",CurrentSet->nBitCount);
	m_Ini.SetProfileInt(GRAB_S, "FullHD120Hz",CurrentSet->nFullHd120Hz);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Type",CurrentSet->nAnalogType);
#ifdef SM_MODIFY_CODE__
	m_Ini.SetProfileInt(GRAB_S, "Grab Image Check Area", CurrentSet->nGrabCheckArea);
#endif


	m_Ini.SetProfileInt(GRAB_S, "ImageRotation",CurrentSet->nImageRotation);
	m_Ini.SetProfileInt(GRAB_S, "Red Blue Swap",CurrentSet->nRedBlueSwap);

	m_Ini.SetProfileInt(GRAB_S, "DE to HSYNC Enable", CurrentSet->nDeHsyncEnable);
	m_Ini.SetProfileInt(GRAB_S, "HSYNC Edge Enable", CurrentSet->nHyncEdgeEnable);

	if(g_nGrabberType == UHD_GRABBER){
		m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Mode",CurrentSet->nLVDS_Mode);
		m_Ini.SetProfileInt(GRAB_S, "UHD Type",CurrentSet->nUHD_Type);
		m_Ini.SetProfileInt(GRAB_S, "UHD Grab BitShift",CurrentSet->nUHD_Grab_BitShift);
		m_Ini.SetProfileInt(GRAB_S, "UHD Grab Mode",CurrentSet->nUHD_Grab_Mode);
		m_Ini.SetProfileInt(GRAB_S, "UHD Grab Delay",CurrentSet->nUHD_Grab_Delay);
		m_Ini.SetProfileInt(GRAB_S, "UHD Y20 SW Mode", CurrentSet->nUHD_Y20_SW_Mode);

		m_Ini.SetProfileInt(GRAB_S, "HDMI Grab Resolution",CurrentSet->nHDMI_Grab_Resolution);
		m_Ini.SetProfileInt(GRAB_S, "HDMI Grab BitShift",CurrentSet->nHDMI_Grab_BitShift);
		m_Ini.SetProfileInt(GRAB_S, "HDMI Out Port Control",(UINT)CurrentSet->bHdmiOutControl);
		m_Ini.SetProfileInt(GRAB_S, "HDMI Grab Mask OSD",(UINT)CurrentSet->bHdmiGrab_MaskOsd);
	}
	//==================
	// Tool/Area Option
	//==================
	m_Ini.SetProfileString(TOOL_AREA_S, "Current Model Suffix",CurrentSet->sModelSuffixName);
	m_Ini.SetProfileInt(TOOL_AREA_S, "Organization ID",CurrentSet->nOrganization_ID);

	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option1",CurrentSet->sToolOption1);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option2",CurrentSet->sToolOption2);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option3",CurrentSet->sToolOption3);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option4",CurrentSet->sToolOption4);
	//091016
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option5",CurrentSet->sToolOption5);
	//101011
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option6",CurrentSet->sToolOption6);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option7",CurrentSet->sToolOption7);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option8",CurrentSet->sToolOption8);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool CRC",CurrentSet->sToolCRC);
	m_Ini.SetProfileInt(TOOL_AREA_S, "ToolOption Write", CurrentSet->bToolOptionWriteFlag);


	m_Ini.SetProfileString(TOOL_AREA_S, "Area Option1",CurrentSet->sAreaOption1);
//	m_Ini.SetProfileString(TOOL_AREA_S, "Area Option2",CurrentSet->sAreaOption2);

	m_Ini.SetProfileString(TOOL_AREA_S, "Country Group",CurrentSet->sCountryGroup);
	m_Ini.SetProfileString(TOOL_AREA_S, "Country Code",CurrentSet->sCountry);

	m_Ini.SetProfileString(TOOL_AREA_S, "Commercial Option1",CurrentSet->sCommercialOption1);
	m_Ini.SetProfileString(TOOL_AREA_S, "BOARD Option", CurrentSet->sBoardOption);
	m_Ini.SetProfileString(TOOL_AREA_S, "Commercial Board Option", CurrentSet->sCommercialBoardOption);


	m_Ini.SetProfileInt(TOOL_AREA_S, "Tool Option Chek Delay" ,CurrentSet->nToolOptionCheck_Delay);

	m_Ini.SetProfileString(VERSION_CHECK_S, "CPU Version",CurrentSet->sCPUVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Micom Version",CurrentSet->sMicomVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "USB Version",CurrentSet->sUSBVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "MNT Version",CurrentSet->sMNTVersion);

	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI1",	CurrentSet->sEDID_HDMI1);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI2",	CurrentSet->sEDID_HDMI2);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI3",	CurrentSet->sEDID_HDMI3);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI4",	CurrentSet->sEDID_HDMI4);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_HDMI5",	CurrentSet->sEDID_HDMI5);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_DP",		CurrentSet->sEDID_DP);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_DP2",	CurrentSet->sEDID_DP2);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_DVI",	CurrentSet->sEDID_DVI);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_PC",		CurrentSet->sEDID_PC);
	m_Ini.SetProfileString(SPEC_CHECK_S, "EDID_RGB",	CurrentSet->sEDID_RGB);
                                                                                            
	m_Ini.SetProfileString(SPEC_CHECK_S, "CI",			CurrentSet->sCI);
	m_Ini.SetProfileString(SPEC_CHECK_S, "CNTV",		CurrentSet->sCNTV);
	m_Ini.SetProfileString(SPEC_CHECK_S, "DTCP_RX",		CurrentSet->sDTCP_RX);
	m_Ini.SetProfileString(SPEC_CHECK_S, "DVBS",		CurrentSet->sDVBS);
	m_Ini.SetProfileString(SPEC_CHECK_S, "DVBS2",		CurrentSet->sDVBS2);
	m_Ini.SetProfileString(SPEC_CHECK_S, "ESN",			CurrentSet->sESN);
	m_Ini.SetProfileString(SPEC_CHECK_S, "HDCP_RX",		CurrentSet->sHDCP_RX);
	m_Ini.SetProfileString(SPEC_CHECK_S, "HDCP_RX_14",	CurrentSet->sHDCP_RX_14);
	m_Ini.SetProfileString(SPEC_CHECK_S, "HDCP_TX",		CurrentSet->sHDCP_TX);
	m_Ini.SetProfileString(SPEC_CHECK_S, "JAPAN_LNB",	CurrentSet->sJAPAN_LNB);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MAC_ADDRESS",	CurrentSet->sMAC_ADDRESS);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MARINKEY",	CurrentSet->sMARINKEY);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MODEL_NAME",	CurrentSet->sMODEL_NAME);
	m_Ini.SetProfileString(SPEC_CHECK_S, "SERIAL_NO",	CurrentSet->sSERIAL_NO);
	m_Ini.SetProfileString(SPEC_CHECK_S, "WASU",		CurrentSet->sWASU);
	m_Ini.SetProfileString(SPEC_CHECK_S, "WIDEVINE",	CurrentSet->sWIDEVINE);
	m_Ini.SetProfileString(SPEC_CHECK_S, "ADC",			CurrentSet->sADC);
	m_Ini.SetProfileString(SPEC_CHECK_S, "BUILT_WIFI",	CurrentSet->sBUILT_WIFI);
	m_Ini.SetProfileString(SPEC_CHECK_S, "CA",		CurrentSet->sCA);
	m_Ini.SetProfileString(SPEC_CHECK_S, "ATSC30_KEY", CurrentSet->sASTC30_Key);
	m_Ini.SetProfileString(SPEC_CHECK_S, "MFI", CurrentSet->sMfi_Key);
	m_Ini.SetProfileString(SPEC_CHECK_S, "CI+ECP", CurrentSet->sCI_CEP_Key);

	//+add psh 080701
	m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate", CurrentSet->wTVBaudRate);

	m_Ini.SetProfileInt(GENERAL_S, "Scanner Skip", CurrentSet->bScanNotUse);
	m_Ini.SetProfileInt(GENERAL_S, "DP Time Set", CurrentSet->nDP_TimeSel);
	m_Ini.SetProfileInt(GENERAL_S, "DPMS Sample Rate", CurrentSet->nDPMS_SampleRate);


	//============
	// I2C Option
	//============
	m_Ini.SetProfileInt(I2C_SETTING_S, "Use I2C",CurrentSet->bUseI2cAdc);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Clock Speed",CurrentSet->nI2cClkSpeed);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Packet Delay",CurrentSet->nI2cPacketDelay);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Read Byte Delay",CurrentSet->nI2cReadByteDelay);
	m_Ini.SetProfileInt(I2C_SETTING_S, "Write Byte Delay",CurrentSet->nI2cWriteByteDelay);

	//============
	// TV Control
	//============
	m_Ini.SetProfileInt(TV_CONTROL_S, "TV Control Type",CurrentSet->nTVControlType);
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Source Control",CurrentSet->bAutoSourceControl);
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Pattern Control",CurrentSet->bAutoPatternControl);
	//add 20101012
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto AVSwitchBox Control",CurrentSet->nAutoAVSwitchBoxControl);

	//+add psh 100310
	//============
	// HDMI Gen. Control
	//============
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Resolution",CurrentSet->nHDMIResolution);
	
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI1 4K Model",CurrentSet->nHDMI1_Model);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI2 4K Model",CurrentSet->nHDMI2_Model);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI3 4K Model",CurrentSet->nHDMI3_Model);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI4 4K Model",CurrentSet->nHDMI4_Model);

	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI1 Port",CurrentSet->nHDMI1_Port);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI2 Port",CurrentSet->nHDMI2_Port);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI3 Port",CurrentSet->nHDMI3_Port);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDMI4 Port",CurrentSet->nHDMI4_Port);


	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Color Bit Count",CurrentSet->nHDMIBitSpec);

	m_Ini.SetProfileInt(HDMI_FORMAT_S, "CEC On/Off Control", CurrentSet->bHdmiCecControl);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "EDID On/Off Control", CurrentSet->bHdmiEdidControl);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "HDCP On/Off Control", CurrentSet->bHdmiHdcpControl);

	m_Ini.SetProfileInt(COMP_FORMAT_S, "Resolution",CurrentSet->nCompResolution);

	// add 101029
	m_Ini.SetProfileInt(GRAB_S, "3D Grabbing Mode",CurrentSet->n3DGrabbingMode);

		//HDCP-Key DOwnload
	m_Ini.SetProfileString(HDCP_KEYDN_S, "Key Class Name",CurrentSet->sHDCP_Key_Class);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Key Class No",CurrentSet->nHDCP_Key_Class);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Data SIze",CurrentSet->nHDCP_Data_Size);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Start ADH",CurrentSet->nHDCP_Start_ADH);
	m_Ini.SetProfileInt(HDCP_KEYDN_S, "Start ADL",CurrentSet->nHDCP_Start_ADL);

	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_AV1",       CurrentSet->sJACK_AV1);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_AV2",       CurrentSet->sJACK_AV2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_SCART",     CurrentSet->sJACK_SCART);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_COMP1",     CurrentSet->sJACK_COMP1);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_COMP2",     CurrentSet->sJACK_COMP2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB1",      CurrentSet->sJACK_USB1);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB2",      CurrentSet->sJACK_USB2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB3",      CurrentSet->sJACK_USB3);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_USB4",      CurrentSet->sJACK_USB4);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HEAD_PHONE",CurrentSet->sJACK_HEAD_PHONE);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI1",     CurrentSet->sJACK_HDMI1);;
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI2",     CurrentSet->sJACK_HDMI2);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI3",     CurrentSet->sJACK_HDMI3);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_HDMI4",     CurrentSet->sJACK_HDMI4);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_RGB",       CurrentSet->sJACK_RGB);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_DVI",       CurrentSet->sJACK_DVI);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_DP",        CurrentSet->sJACK_DP);
	m_Ini.SetProfileString(INPUT_CHECK_S, "JACK_SMHL",      CurrentSet->sJACK_SMHL);

	m_Ini.SetProfileInt(INPUT_CHECK_S, "HDMI_ARC_PORT_NUMBER",      CurrentSet->nARC_Force_Port_Number);
	m_Ini.SetProfileInt(INPUT_CHECK_S, "ARC_ON_OFF_CH_NUMBER",      CurrentSet->nARC_Force_OnOffChannel);

	sTmp.Format("%s:%s:Tool Option(1:%s,2:%s,3:%s,4:%s,5:%s,6:%s,7:%s,8:%s)",CurrentSet->sChassisName,CurrentSet->sModelName,
		            CurrentSet->sToolOption1,CurrentSet->sToolOption2,CurrentSet->sToolOption3,CurrentSet->sToolOption4,
					CurrentSet->sToolOption5,CurrentSet->sToolOption6,CurrentSet->sToolOption7,CurrentSet->sToolOption8);   
	ToolOptionLog_Save(g_pView->m_szExePath, sTmp, FALSE);


	m_Ini.SetProfileString(SPEC_CHECK_S, "BAT_VERSION", CurrentSet->sBatVersion);
	//m_Ini.SetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET",      CurrentSet->bEpiPAckReset);
	//m_Ini.SetProfileInt(SPEC_CHECK_S, "GRAB_BASE_RESET", CurrentSet->bGrabBaseReset);


	return TRUE;
}

BOOL OpenModelIniFile(CString sIniPath, CString sDftPath)
{
	CFile pFile;
	CFileException FileExcept;
	CString sTemp;

	//+change kwmoon 080910
//	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::modeWrite, &FileExcept)) 
	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::shareDenyNone, &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();
	m_Ini.SetIniFile(sIniPath);

	//=========================
	// Grabber Resolution Info
	//=========================
	CurrentSet->nLvdsHeight   = m_Ini.GetProfileInt(GRAB_S, "LVDS Grabber Height");
	CurrentSet->nLvdsWidth    = m_Ini.GetProfileInt(GRAB_S, "LVDS Grabber Width");
	CurrentSet->nAnalogHeight = m_Ini.GetProfileInt(GRAB_S, "Analog Grabber Height");
	CurrentSet->nAnalogWidth  = m_Ini.GetProfileInt(GRAB_S, "Analog Grabber Width");
	CurrentSet->bRobotSkipFFC = 1;//  m_Ini.GetProfileInt(GRAB_S, "LVDS 30P Robot Skip FFC");
	CurrentSet->bCheckImageFullReset = m_Ini.GetProfileInt(GRAB_S, "Image Full Reset Option");
	CurrentSet->bCheckOledReset = m_Ini.GetProfileInt(GRAB_S, "Image OLED Reset Option");
	
	if(g_nGrabberType == FHD_GRABBER){
		CurrentSet->nUranousMode  = m_Ini.GetProfileInt(GRAB_S, "Uranous Grabber Mode");
	}
	CurrentSet->bOddDE_Only	  = m_Ini.GetProfileInt(GRAB_S, "LVDS OddDE Only");
	CurrentSet->nFullHd120Hz  = m_Ini.GetProfileInt(GRAB_S, "FullHD120Hz");
	CurrentSet->nBitCount	  = m_Ini.GetProfileInt(GRAB_S, "Color Bit Count");
	CurrentSet->nAnalogType  = m_Ini.GetProfileInt(GRAB_S, "Analog Grabber Type");


#ifdef SM_MODIFY_CODE__
	CurrentSet->nGrabCheckArea = m_Ini.GetProfileInt(GRAB_S, "Grab Image Check Area"); //m_Ini.SetProfileInt(GRAB_S, "Grab Image Check Area", CurrentSet->nGrabCheckArea);
#endif

	if(CurrentSet->nAnalogWidth == 768){ 
		CurrentSet->nAnalogWidth = 704;
		CurrentSet->nAnalogHeight = 572;
	}

	if((CurrentSet->nAnalogType == 0) && (CurrentSet->nAnalogWidth == 704))
	{
		CurrentSet->nAnalogType = 1;
	}

	if(CurrentSet->nBitCount == 0) CurrentSet->nBitCount = 24;

	CurrentSet->nImageRotation = m_Ini.GetProfileInt(GRAB_S, "ImageRotation");
	CurrentSet->nRedBlueSwap = m_Ini.GetProfileInt(GRAB_S, "Red Blue Swap");

	CurrentSet->nDeHsyncEnable = m_Ini.GetProfileInt(GRAB_S, "DE to HSYNC Enable");
	CurrentSet->nHyncEdgeEnable = m_Ini.GetProfileInt(GRAB_S, "HSYNC Edge Enable");
	
	if(g_nGrabberType == UHD_GRABBER){
		CurrentSet->nLVDS_Mode	  = m_Ini.GetProfileInt(GRAB_S, "LVDS Grabber Mode");
		CurrentSet->nUHD_Type = m_Ini.GetProfileInt(GRAB_S, "UHD Type");
		CurrentSet->nUHD_Grab_BitShift = m_Ini.GetProfileInt(GRAB_S, "UHD Grab BitShift");
		CurrentSet->nUHD_Grab_Mode = m_Ini.GetProfileInt(GRAB_S, "UHD Grab Mode");
		if(CurrentSet->nUHD_Grab_Mode > 21){CurrentSet->nUHD_Grab_Mode = 13;}
		CurrentSet->nUHD_Grab_Delay = m_Ini.GetProfileInt(GRAB_S, "UHD Grab Delay");
		CurrentSet->nUHD_Y20_SW_Mode = m_Ini.GetProfileInt(GRAB_S, "UHD Y20 SW Mode");
		if (CurrentSet->nUHD_Y20_SW_Mode > 16) { CurrentSet->nUHD_Y20_SW_Mode = 1; }

		CurrentSet->nHDMI_Grab_Resolution = m_Ini.GetProfileInt(GRAB_S, "HDMI Grab Resolution");
		CurrentSet->nHDMI_Grab_BitShift = m_Ini.GetProfileInt(GRAB_S, "HDMI Grab BitShift");
		CurrentSet->bHdmiOutControl = 0;// m_Ini.GetProfileInt(GRAB_S, "HDMI Out Port Control");
		CurrentSet->bHdmiGrab_MaskOsd = m_Ini.GetProfileInt(GRAB_S, "HDMI Grab Mask OSD");

		switch(CurrentSet->nHDMI_Grab_Resolution) 
		{
			case 0:  CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
			case 1:  CurrentSet->nHdmiWidth = 1366; CurrentSet->nHdmiHeight = 768; break;
			case 2:  CurrentSet->nHdmiWidth = 1280; CurrentSet->nHdmiHeight = 1024; break;
			case 3:  CurrentSet->nHdmiWidth = 1920; CurrentSet->nHdmiHeight = 1080; break;
			case 4:  CurrentSet->nHdmiWidth = 1280; CurrentSet->nHdmiHeight = 540; break;
			default: CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
		}
	}
	
	//==================
	// Tool/Area Option
	//==================
	CurrentSet->sModelSuffixName = m_Ini.GetProfileString(TOOL_AREA_S, "Current Model Suffix");
	CurrentSet->nOrganization_ID = m_Ini.GetProfileInt(TOOL_AREA_S, "Organization ID");

	CurrentSet->sToolOption1	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option1");
	CurrentSet->sToolOption2	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option2");
	CurrentSet->sToolOption3	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option3");
	CurrentSet->sToolOption4	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option4");
	//091016
	CurrentSet->sToolOption5	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option5");
	CurrentSet->sToolOption6	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option6");
	CurrentSet->sToolOption7	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option7");
	CurrentSet->sToolOption8	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option8");
	CurrentSet->sBoardOption = m_Ini.GetProfileString(TOOL_AREA_S, "BOARD Option");
	CurrentSet->sCommercialBoardOption = m_Ini.GetProfileString(TOOL_AREA_S, "Commercial Board Option");
	CurrentSet->sToolCRC		= m_Ini.GetProfileString(TOOL_AREA_S, "Tool CRC");
	CurrentSet->bToolOptionWriteFlag = m_Ini.GetProfileInt(TOOL_AREA_S, "ToolOption Write");


	CurrentSet->sAreaOption1	= m_Ini.GetProfileString(TOOL_AREA_S, "Area Option1");
//	CurrentSet->sAreaOption2	= m_Ini.GetProfileString(TOOL_AREA_S, "Area Option2");
	CurrentSet->sCommercialOption1	= m_Ini.GetProfileString(TOOL_AREA_S, "Commercial Option1");

	CurrentSet->sCountryGroup	= m_Ini.GetProfileString(TOOL_AREA_S, "Country Group");
	CurrentSet->sCountry		= m_Ini.GetProfileString(TOOL_AREA_S, "Country Code");

	CurrentSet->nToolOptionCheck_Delay	= m_Ini.GetProfileInt(TOOL_AREA_S, "Tool Option Chek Delay");

	CurrentSet->sCPUVersion	  = m_Ini.GetProfileString(VERSION_CHECK_S, "CPU Version");
	CurrentSet->sMicomVersion = m_Ini.GetProfileString(VERSION_CHECK_S, "Micom Version");
	CurrentSet->sUSBVersion	  = m_Ini.GetProfileString(VERSION_CHECK_S, "USB Version");
	CurrentSet->sMNTVersion	  = m_Ini.GetProfileString(VERSION_CHECK_S, "MNT Version");

	CurrentSet->sEDID_HDMI1	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_HDMI1");
	CurrentSet->sEDID_HDMI2	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_HDMI2");
	CurrentSet->sEDID_HDMI3	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_HDMI3");
	CurrentSet->sEDID_HDMI4	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_HDMI4");
	CurrentSet->sEDID_HDMI5	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_HDMI5");
	CurrentSet->sEDID_DP	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_DP");
	CurrentSet->sEDID_DP2	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_DP2");
	CurrentSet->sEDID_DVI	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_DVI");
	CurrentSet->sEDID_PC	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_PC");
	CurrentSet->sEDID_RGB	 = m_Ini.GetProfileString(SPEC_CHECK_S, "EDID_RGB");

	CurrentSet->sCI			 = m_Ini.GetProfileString(SPEC_CHECK_S, "CI");
	CurrentSet->sCNTV		 = m_Ini.GetProfileString(SPEC_CHECK_S, "CNTV");
	CurrentSet->sDTCP_RX	 = m_Ini.GetProfileString(SPEC_CHECK_S, "DTCP_RX");
	CurrentSet->sDVBS		 = m_Ini.GetProfileString(SPEC_CHECK_S, "DVBS");
	CurrentSet->sDVBS2		 = m_Ini.GetProfileString(SPEC_CHECK_S, "DVBS2");
	CurrentSet->sESN		 = m_Ini.GetProfileString(SPEC_CHECK_S, "ESN");
	CurrentSet->sHDCP_RX	 = m_Ini.GetProfileString(SPEC_CHECK_S, "HDCP_RX");
	CurrentSet->sHDCP_RX_14	 = m_Ini.GetProfileString(SPEC_CHECK_S, "HDCP_RX_14");
	CurrentSet->sHDCP_TX	 = m_Ini.GetProfileString(SPEC_CHECK_S, "HDCP_TX");
	CurrentSet->sJAPAN_LNB	 = m_Ini.GetProfileString(SPEC_CHECK_S, "JAPAN_LNB");
	CurrentSet->sMAC_ADDRESS = m_Ini.GetProfileString(SPEC_CHECK_S, "MAC_ADDRESS");
	CurrentSet->sMARINKEY	 = m_Ini.GetProfileString(SPEC_CHECK_S, "MARINKEY");
	CurrentSet->sMODEL_NAME	 = m_Ini.GetProfileString(SPEC_CHECK_S, "MODEL_NAME");
	CurrentSet->sSERIAL_NO	 = m_Ini.GetProfileString(SPEC_CHECK_S, "SERIAL_NO");
	CurrentSet->sWASU		 = m_Ini.GetProfileString(SPEC_CHECK_S, "WASU");
	CurrentSet->sWIDEVINE	 = m_Ini.GetProfileString(SPEC_CHECK_S, "WIDEVINE");
	CurrentSet->sADC		 = m_Ini.GetProfileString(SPEC_CHECK_S, "ADC");
    CurrentSet->sBUILT_WIFI	 = m_Ini.GetProfileString(SPEC_CHECK_S, "BUILT_WIFI");	
	CurrentSet->sASTC30_Key = m_Ini.GetProfileString(SPEC_CHECK_S, "ATSC30_KEY");
	CurrentSet->sMfi_Key = m_Ini.GetProfileString(SPEC_CHECK_S, "MFI");
	CurrentSet->sCI_CEP_Key = m_Ini.GetProfileString(SPEC_CHECK_S, "CI+ECP");
	//m_Ini.SetProfileString(SPEC_CHECK_S, "CI+ECP", CurrentSet->sCI_CEP_Key);
	

	if(CurrentSet->sModelSuffixName == "NULL") CurrentSet->sModelSuffixName = "";
	
	if(CurrentSet->sToolOption1 == "NULL") CurrentSet->sToolOption1 = "";
	if(CurrentSet->sToolOption2 == "NULL") CurrentSet->sToolOption2 = "";
	if(CurrentSet->sToolOption3 == "NULL") CurrentSet->sToolOption3 = "";
	if(CurrentSet->sToolOption4 == "NULL") CurrentSet->sToolOption4 = "";
	//091016
	if(CurrentSet->sToolOption5 == "NULL") CurrentSet->sToolOption5 = "";
	if(CurrentSet->sToolOption6 == "NULL") CurrentSet->sToolOption6 = "";
	if(CurrentSet->sToolOption7 == "NULL") CurrentSet->sToolOption7 = "";
	if(CurrentSet->sCommercialOption1 == "NULL") CurrentSet->sCommercialOption1 = "";
	if (CurrentSet->sToolOption8 == "NULL") CurrentSet->sToolOption8 = "";
	if (CurrentSet->sBoardOption == "NULL") CurrentSet->sBoardOption = "";
	if (CurrentSet->sCommercialBoardOption == "NULL") CurrentSet->sCommercialBoardOption = "";
	if(CurrentSet->sToolCRC == "NULL") CurrentSet->sToolCRC = "";

	if(CurrentSet->sAreaOption1 == "NULL") CurrentSet->sAreaOption1 = "";
	if(CurrentSet->sCountryGroup == "NULL") CurrentSet->sCountryGroup = "";
	if(CurrentSet->sCountry == "NULL") CurrentSet->sCountry = "";

	if(CurrentSet->sCPUVersion == "NULL") CurrentSet->sCPUVersion = "";
	if(CurrentSet->sMicomVersion == "NULL") CurrentSet->sMicomVersion = "";
	if(CurrentSet->sUSBVersion == "NULL") CurrentSet->sUSBVersion = "";
	if(CurrentSet->sMNTVersion == "NULL") CurrentSet->sMNTVersion = "";

	if(CurrentSet->sEDID_HDMI1 == "NULL") CurrentSet->sEDID_HDMI1 = "";
	if(CurrentSet->sEDID_HDMI2 == "NULL") CurrentSet->sEDID_HDMI2 = "";
	if(CurrentSet->sEDID_HDMI3 == "NULL") CurrentSet->sEDID_HDMI3 = "";
	if(CurrentSet->sEDID_HDMI4 == "NULL") CurrentSet->sEDID_HDMI4 = "";
	if(CurrentSet->sEDID_HDMI5 == "NULL") CurrentSet->sEDID_HDMI5 = "";
	if(CurrentSet->sEDID_DP	   == "NULL") CurrentSet->sEDID_DP    = "";
	if(CurrentSet->sEDID_DP2   == "NULL") CurrentSet->sEDID_DP2   = "";
	if(CurrentSet->sEDID_DVI   == "NULL") CurrentSet->sEDID_DVI   = "";
	if(CurrentSet->sEDID_PC	   == "NULL") CurrentSet->sEDID_PC    = "";
	if(CurrentSet->sEDID_RGB   == "NULL") CurrentSet->sEDID_RGB   = "";

	if(CurrentSet->sCI	    == "NULL") CurrentSet->sCI	  	= "";
	if(CurrentSet->sCNTV	    == "NULL") CurrentSet->sCNTV	= "";
	if(CurrentSet->sDTCP_RX	    == "NULL") CurrentSet->sDTCP_RX	= "";
	if(CurrentSet->sDVBS	    == "NULL") CurrentSet->sDVBS	= "";
	if(CurrentSet->sDVBS2	    == "NULL") CurrentSet->sDVBS2	= "";
	if(CurrentSet->sESN	    == "NULL") CurrentSet->sESN	  	= "";
	if(CurrentSet->sHDCP_RX	    == "NULL") CurrentSet->sHDCP_RX	= "";
	if(CurrentSet->sHDCP_RX_14  == "NULL") CurrentSet->sHDCP_RX_14	= "";
	if(CurrentSet->sHDCP_TX	    == "NULL") CurrentSet->sHDCP_TX	= "";
	if(CurrentSet->sJAPAN_LNB   == "NULL") CurrentSet->sJAPAN_LNB	= "";
	if(CurrentSet->sMAC_ADDRESS == "NULL") CurrentSet->sMAC_ADDRESS = "";
	if(CurrentSet->sMARINKEY    == "NULL") CurrentSet->sMARINKEY	= "";
	if(CurrentSet->sMODEL_NAME  == "NULL") CurrentSet->sMODEL_NAME	= "";
	if(CurrentSet->sSERIAL_NO   == "NULL") CurrentSet->sSERIAL_NO	= "";
	if(CurrentSet->sWASU	    == "NULL") CurrentSet->sWASU	= "";
	if(CurrentSet->sWIDEVINE    == "NULL") CurrentSet->sWIDEVINE	= "";
	if(CurrentSet->sADC	    == "NULL") CurrentSet->sADC	  	= "";
	if(CurrentSet->sBUILT_WIFI  == "NULL") CurrentSet->sBUILT_WIFI	= "";
	if (CurrentSet->sCA == "NULL") CurrentSet->sCA = "";
	if (CurrentSet->sASTC30_Key == "NULL") CurrentSet->sASTC30_Key = "";
	if (CurrentSet->sMfi_Key == "NULL") CurrentSet->sMfi_Key = "";
	if (CurrentSet->sCI_CEP_Key == "NULL") CurrentSet->sCI_CEP_Key = "";

	//+add psh 080701
	CurrentSet->wTVBaudRate = m_Ini.GetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate");
	if(CurrentSet->wTVBaudRate == 0) CurrentSet->wTVBaudRate = 115200;

	CurrentSet->bScanNotUse = m_Ini.GetProfileInt(GENERAL_S, "Scanner Skip");
	CurrentSet->nDP_TimeSel = m_Ini.GetProfileInt(GENERAL_S, "DP Time Set",0);
	CurrentSet->nDPMS_SampleRate = m_Ini.GetProfileInt(GENERAL_S, "DPMS Sample Rate",0);
	//if (CurrentSet->nDP_TimeSel == 0) CurrentSet->wTVBaudRate = 115200;


	//============
	// I2C Option
	//============
	CurrentSet->bUseI2cAdc			= m_Ini.GetProfileInt(I2C_SETTING_S, "Use I2C");
	CurrentSet->nI2cClkSpeed		= m_Ini.GetProfileInt(I2C_SETTING_S, "Clock Speed");
	CurrentSet->nI2cPacketDelay		= m_Ini.GetProfileInt(I2C_SETTING_S, "Packet Delay");
	CurrentSet->nI2cReadByteDelay	= m_Ini.GetProfileInt(I2C_SETTING_S, "Read Byte Delay");
	CurrentSet->nI2cWriteByteDelay	= m_Ini.GetProfileInt(I2C_SETTING_S, "Write Byte Delay");

	//+add kwmoon 080820
	if(CurrentSet->nI2cClkSpeed == 0) CurrentSet->nI2cClkSpeed = 1;
	if(CurrentSet->nI2cPacketDelay == 0) CurrentSet->nI2cPacketDelay = 150;
	if(CurrentSet->nI2cReadByteDelay == 0) CurrentSet->nI2cReadByteDelay = 1;
	if(CurrentSet->nI2cWriteByteDelay == 0) CurrentSet->nI2cWriteByteDelay = 1;

	//============
	// TV Control
	//============
	CurrentSet->nTVControlType			= m_Ini.GetProfileInt(TV_CONTROL_S, "TV Control Type");
	CurrentSet->bAutoSourceControl		= m_Ini.GetProfileInt(TV_CONTROL_S, "Auto Source Control");
	CurrentSet->bAutoPatternControl		= m_Ini.GetProfileInt(TV_CONTROL_S, "Auto Pattern Control");
	//add 20101012
	CurrentSet->nAutoAVSwitchBoxControl	= m_Ini.GetProfileInt(TV_CONTROL_S, "Auto AVSwitchBox Control");
	if(CurrentSet->nAutoAVSwitchBoxControl == 0) CurrentSet->nAutoAVSwitchBoxControl = 1;

	//+add psh 100310
	//============
	// HDMI Gen. Control
	//============
	CurrentSet->nHDMIResolution 		= m_Ini.GetProfileInt(HDMI_FORMAT_S, "Resolution");
	CurrentSet->nHDMI1_Model 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI1 4K Model");
	CurrentSet->nHDMI2_Model 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI2 4K Model");
	CurrentSet->nHDMI3_Model 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI3 4K Model");
	CurrentSet->nHDMI4_Model 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI4 4K Model");

	CurrentSet->nHDMI1_Port 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI1 Port", 1);
	CurrentSet->nHDMI2_Port 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI2 Port", 2);
	CurrentSet->nHDMI3_Port 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI3 Port", 3);
	CurrentSet->nHDMI4_Port 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI4 Port", 4);


	CurrentSet->nHDMIBitSpec			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "Color Bit Count");

	CurrentSet->bHdmiCecControl = m_Ini.GetProfileInt(HDMI_FORMAT_S, "CEC On/Off Control");
	CurrentSet->bHdmiEdidControl = m_Ini.GetProfileInt(HDMI_FORMAT_S, "EDID On/Off Control",1);
	CurrentSet->bHdmiHdcpControl = m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDCP On/Off Control",1);

	CurrentSet->nCompResolution 		= m_Ini.GetProfileInt(COMP_FORMAT_S, "Resolution");

	// add 101029
	CurrentSet->n3DGrabbingMode			= m_Ini.GetProfileInt(GRAB_S, "3D Grabbing Mode");

	//HDCP-Key DOwnload
	CurrentSet->sHDCP_Key_Class		= m_Ini.GetProfileString(HDCP_KEYDN_S, "Key Class Name");
	CurrentSet->nHDCP_Key_Class		= m_Ini.GetProfileInt(HDCP_KEYDN_S, "Key Class No");

	CurrentSet->nHDCP_Data_Size		= m_Ini.GetProfileInt(HDCP_KEYDN_S, "Data SIze");
	CurrentSet->nHDCP_Start_ADH		= m_Ini.GetProfileInt(HDCP_KEYDN_S, "Start ADH");
	CurrentSet->nHDCP_Start_ADL		= m_Ini.GetProfileInt(HDCP_KEYDN_S, "Start ADL");

	CurrentSet->bDrmKey = ChekDrmKey();

	CurrentSet->sJACK_AV1	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_AV1");       
	CurrentSet->sJACK_AV2	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_AV2");       
	CurrentSet->sJACK_SCART	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_SCART");     
	CurrentSet->sJACK_COMP1	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_COMP1");     
	CurrentSet->sJACK_COMP2	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_COMP2");     
	CurrentSet->sJACK_USB1	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_USB1");      
	CurrentSet->sJACK_USB2	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_USB2");      
	CurrentSet->sJACK_USB3	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_USB3");      
	CurrentSet->sJACK_USB4	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_USB4");      
	CurrentSet->sJACK_HEAD_PHONE= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_HEAD_PHONE");
	CurrentSet->sJACK_HDMI1	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_HDMI1");     
	CurrentSet->sJACK_HDMI2	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_HDMI2");     
	CurrentSet->sJACK_HDMI3	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_HDMI3");     
	CurrentSet->sJACK_HDMI4	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_HDMI4");     
	CurrentSet->sJACK_RGB	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_RGB");       
	CurrentSet->sJACK_DVI	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_DVI");       
	CurrentSet->sJACK_DP	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_DP");        
    CurrentSet->sJACK_SMHL	 	= m_Ini.GetProfileString(INPUT_CHECK_S, "JACK_SMHL");      
 

	if(CurrentSet->sJACK_AV1	== "NULL") CurrentSet->sJACK_AV1	= _T("");
	if(CurrentSet->sJACK_AV2	== "NULL") CurrentSet->sJACK_AV2	= _T("");
	if(CurrentSet->sJACK_SCART	== "NULL") CurrentSet->sJACK_SCART	= _T("");
	if(CurrentSet->sJACK_COMP1	== "NULL") CurrentSet->sJACK_COMP1	= _T("");
	if(CurrentSet->sJACK_COMP2	== "NULL") CurrentSet->sJACK_COMP2	= _T("");
	if(CurrentSet->sJACK_USB1	== "NULL") CurrentSet->sJACK_USB1	= _T("");
	if(CurrentSet->sJACK_USB2	== "NULL") CurrentSet->sJACK_USB2	= _T("");
	if(CurrentSet->sJACK_USB3	== "NULL") CurrentSet->sJACK_USB3	= _T("");
	if(CurrentSet->sJACK_USB4	== "NULL") CurrentSet->sJACK_USB4	= _T("");
	if(CurrentSet->sJACK_HEAD_PHONE	== "NULL") CurrentSet->sJACK_HEAD_PHONE	= _T("");
	if(CurrentSet->sJACK_HDMI1	== "NULL") CurrentSet->sJACK_HDMI1	= _T("");
	if(CurrentSet->sJACK_HDMI2	== "NULL") CurrentSet->sJACK_HDMI2	= _T("");
	if(CurrentSet->sJACK_HDMI3	== "NULL") CurrentSet->sJACK_HDMI3	= _T("");
	if(CurrentSet->sJACK_HDMI4	== "NULL") CurrentSet->sJACK_HDMI4	= _T("");
	if(CurrentSet->sJACK_RGB	== "NULL") CurrentSet->sJACK_RGB	= _T("");
	if(CurrentSet->sJACK_DVI	== "NULL") CurrentSet->sJACK_DVI	= _T("");
	if(CurrentSet->sJACK_DP		== "NULL") CurrentSet->sJACK_DP		= _T("");
	if(CurrentSet->sJACK_SMHL	== "NULL") CurrentSet->sJACK_SMHL	= _T("");

	 
	CurrentSet->nARC_Force_Port_Number = m_Ini.GetProfileInt(INPUT_CHECK_S, "HDMI_ARC_PORT_NUMBER");
 	CurrentSet->nARC_Force_OnOffChannel = m_Ini.GetProfileInt(INPUT_CHECK_S, "ARC_ON_OFF_CH_NUMBER");
//	m_Ini.SetProfileInt(INPUT_CHECK_S, "ARC_ON_OFF_CH_NUMBER", CurrentSet->nARC_Force_OnOffChannel);

	sTemp.Format("%s:%s:Tool Option(1:%s,2:%s,3:%s,4:%s,5:%s,6:%s,7:%s,8:%s)",CurrentSet->sChassisName,CurrentSet->sModelName,
		            CurrentSet->sToolOption1,CurrentSet->sToolOption2,CurrentSet->sToolOption3,CurrentSet->sToolOption4,
					CurrentSet->sToolOption5,CurrentSet->sToolOption6,CurrentSet->sToolOption7,CurrentSet->sToolOption8);   
	ToolOptionLog_Save(sDftPath, sTemp, TRUE);


	CurrentSet->sBatVersion = m_Ini.GetProfileString(SPEC_CHECK_S, "BAT_VERSION");
	if (CurrentSet->sBatVersion == "NULL")
	{
		CurrentSet->sBatVersion = "31";
	}
	
	//m_Ini.SetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET", CurrentSet->bEpiPAckReset);
	//CurrentSet->nHDMI1_Port 			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "HDMI1 Port", 1);
	//CurrentSet->bEpiPAckReset = m_Ini.GetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET", 0);
	//CurrentSet->bGrabBaseReset = m_Ini.GetProfileInt(SPEC_CHECK_S, "GRAB_BASE_RESET", 0);
	
	memset(gPLC_Ctrl.m_nTestStepNGCountList, 0, sizeof(gPLC_Ctrl.m_nTestStepNGCountList));
	
	return TRUE;
}


BOOL GetBmpSize(CString sBmpPath, CPoint &pSize)
{
	BITMAPFILEHEADER FileHeader;   // Bitmap File Header
	BITMAPINFOHEADER InfoHeader;   // Bitmap Info Header
	FILE*	 pFile = NULL;
	
	// Open Image File
	fopen_s(&pFile, (LPCTSTR)sBmpPath,"rb");
	if(pFile == NULL)
	{
		AfxMessageBox("Failed to Get Info From Image File : "+sBmpPath); return FALSE;
	}

	// Read Bitmap File Header
	fread(&FileHeader,sizeof(BITMAPFILEHEADER),1,pFile);
	
	if(FileHeader.bfType != 0x4D42)
	{
		AfxMessageBox("Unsupported Image File"); 
		return FALSE;
	}

	// Read Bitmap Info Header
	fread(&InfoHeader,sizeof(BITMAPINFOHEADER),1,pFile);

	if(InfoHeader.biBitCount < 8)
	{
		AfxMessageBox("Unsupported Image File"); 
		return FALSE;
	}

	if(InfoHeader.biBitCount<8 ) 
	{ 
		AfxMessageBox("Unsupported Format(Color Depth < 8)!"); 
		return FALSE; 
	}
	pSize.x = InfoHeader.biWidth;
	pSize.y = InfoHeader.biHeight;
	fclose(pFile);
	return TRUE;
}

// File이 존재하는지 여부 확인 
BOOL FileExists(const CString csFilePath )
{
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;  // file handle
	BOOL bReturn;

	hFind = FindFirstFile(csFilePath, &FindFileData);

	if(hFind != INVALID_HANDLE_VALUE){
		bReturn = TRUE;
        FindClose(hFind); 
	}
	else{
		bReturn = FALSE;
	}

	return bReturn;
}

// File이 존재하는지 여부 확인 
BOOL FileExists(const char * cpcFilePath)
{
	CString sFilePath = cpcFilePath;
	return FileExists(sFilePath);
}

CString SplitDefectData(UINT nNo, CString sRstData)
{
	CString sTmp;
	CString sData[7];
	UINT	nVal[3];//RGB
	UINT	nTmp, nTmp1;

	for( nTmp = 0; nTmp < 6 ;nTmp++)
	{
		sData[nTmp] = sRstData.Left(sRstData.Find(','));
		sRstData.Delete(0, sRstData.Find(',') + 1);
	}
	sRstData.TrimLeft();
	sRstData.TrimRight();
	sData[6] = sRstData;
	if(nNo == 1) return sData[0];
	else if(nNo == 2)
	{
		nTmp = atoi(sData[1]);

		//+change kwmoon 070126
		if(nTmp == POSITION_TEST) sTmp.Format(_T("POSITION"));
		else if(nTmp == SIZE_TEST) sTmp.Format(_T("SIZE"));
		else if(nTmp == RGB_TEST) sTmp.Format(_T("COLOR"));
		else if(nTmp == BRIGHT_TEST) sTmp.Format(_T("BRIGHT"));
		else if(nTmp == OSD_TEST) sTmp.Format(_T("OSD"));
		else sTmp.Format("Error");
		return sTmp;
		//-
	}
	else if(nNo == 3) 
	{
		sTmp.Format(sData[2]);
		return sTmp;
	}
	else if(nNo == 4) 
	{
		sTmp.Format(sData[3]);
		return sTmp;
	}
	else if(nNo == 5)
	{
		nTmp = atoi(sData[1]);
		if((nTmp == POSITION_TEST)||(nTmp == SIZE_TEST)||(nTmp == OSD_TEST)) 
		{
			sTmp.Format(_T(sData[4]));
			return sTmp;
		}
		else if((nTmp == RGB_TEST)||(nTmp == BRIGHT_TEST)) 
		{
			nTmp1 = atoi(sData[4]);
			// R
			nVal[0] = (nTmp1 >> 16) & 0x000000FF;

			// G
			nVal[1] = (nTmp1 >> 8) & 0x000000FF;

			//B
			nVal[2] = (nTmp1 >> 0) & 0x000000FF;

			
			sTmp.Format("%d,%d,%d", nVal[0], nVal[1], nVal[2]);
			return sTmp;
		}
	}
	else if(nNo == 6)
	{
		nTmp = atoi(sData[1]);
		if((nTmp == POSITION_TEST)||(nTmp == SIZE_TEST)||(nTmp == OSD_TEST)) 
		{
			sTmp.Format(_T(sData[5]));
			return sTmp;
		}
		else if((nTmp == RGB_TEST)||(nTmp == BRIGHT_TEST)) 
		{
			nTmp1 = atoi(sData[5]);
			
			// R
			nVal[0] = (nTmp1 >> 16) & 0x000000FF;

			// G
			nVal[1] = (nTmp1 >> 8) & 0x000000FF;

			//B
			nVal[2] = (nTmp1 >> 0) & 0x000000FF;

			sTmp.Format("%d,%d,%d", nVal[0], nVal[1], nVal[2]);
			return sTmp;
		}
	}
	else if(nNo == 7)
	{
		sTmp.Format(sData[6]);
		return sTmp;
	}
	else if(nNo == 8)
	{
		nTmp = atoi(sData[1]);
		if((nTmp == POSITION_TEST)||(nTmp == SIZE_TEST)||(nTmp == OSD_TEST)) sTmp.Format(_T("Match %%"));
		else if((nTmp == RGB_TEST)||(nTmp == BRIGHT_TEST)) sTmp.Format(_T("RGB"));
		else sTmp.Format("Error");
		return sTmp;
	}
	sTmp.Format("Error");
	return sTmp;
}

void ShowSubStepMessage(CString sMsg, CString sDefaultMsg)
{
	if(!sMsg.IsEmpty())
	{
		ctrlTestSubStep.SetWindowText(" "+sMsg);
	}
	else
	{
		ctrlTestSubStep.SetWindowText(" "+sDefaultMsg);
	}

	//+change kwmoon 080509
//	if(pCurStep->m_bStepInternalRetryFlag)
	if(pCurStep->m_bStepInternalRetryFlag || pCurStep->m_bStepRetryFlag)
	{
		ctrlTestSubStep.SetTextColor(RGB(255,0,0));  
	}
	else
	{
		ctrlTestSubStep.SetTextColor(RGB(0,0,0));  
	}

	//+add kwmoon 080419
	ctrlTestSubStep.Invalidate();

	//+add kwmoon 070913
	ctrlTestSubStep.UpdateWindow();
}

//+add kwmoon 070823
void SaveLineTestData(CIniAccess* pIni,CString sSection)
{
	CString sRefRdata = _T("");
	CString sRefGdata = _T("");
	CString sRefBdata = _T("");
	CString sGrabRdata = _T("");
	CString sGrabGdata = _T("");
	CString sGrabBdata = _T("");

	CString sRefTempRdata = _T("");
	CString sRefTempGdata = _T("");
	CString sRefTempBdata = _T("");
	CString sGrabTempRdata = _T("");
	CString sGrabTempGdata = _T("");
	CString sGrabTempBdata = _T("");
	
	int i;
	//----------------------//
	// No of High Bit
	//----------------------//
	for(i=0; i<MAX_NO_COLOR_BITS-1; i++)
	{
		// Reference Image
		sRefTempRdata.Format("%d,",pCurStep->m_aRefNoHighBit[R_DATA][i]);
		sRefRdata += sRefTempRdata;
	
		sRefTempGdata.Format("%d,",pCurStep->m_aRefNoHighBit[G_DATA][i]);
		sRefGdata += sRefTempRdata;

		sRefTempBdata.Format("%d,",pCurStep->m_aRefNoHighBit[B_DATA][i]);
		sRefBdata += sRefTempRdata;

		// Grab Image
		sGrabTempRdata.Format("%d,",pCurStep->m_aGrabNoHighBit[R_DATA][i]);
		sGrabRdata += sGrabTempRdata;

		sGrabTempGdata.Format("%d,",pCurStep->m_aGrabNoHighBit[G_DATA][i]);
		sGrabGdata += sGrabTempRdata;

		sGrabTempBdata.Format("%d,",pCurStep->m_aGrabNoHighBit[B_DATA][i]);
		sGrabBdata += sGrabTempRdata;
	}

	// Reference Image
	sRefTempRdata.Format("%d",pCurStep->m_aRefNoHighBit[R_DATA][MAX_NO_COLOR_BITS-1]);
	sRefRdata += sRefTempRdata;

	sRefTempGdata.Format("%d",pCurStep->m_aRefNoHighBit[G_DATA][MAX_NO_COLOR_BITS-1]);
	sRefGdata += sRefTempGdata;
	
	sRefTempBdata.Format("%d",pCurStep->m_aRefNoHighBit[B_DATA][MAX_NO_COLOR_BITS-1]);
	sRefBdata += sRefTempBdata;

	// Grab Image
	sGrabTempRdata.Format("%d",pCurStep->m_aGrabNoHighBit[R_DATA][MAX_NO_COLOR_BITS-1]);
	sGrabRdata += sGrabTempRdata;

	sGrabTempGdata.Format("%d",pCurStep->m_aGrabNoHighBit[G_DATA][MAX_NO_COLOR_BITS-1]);
	sGrabGdata += sGrabTempGdata;
	
	sGrabTempBdata.Format("%d",pCurStep->m_aGrabNoHighBit[B_DATA][MAX_NO_COLOR_BITS-1]);
	sGrabBdata += sGrabTempBdata;

	// Write
	pIni->SetProfileInt(sSection, "NO MASKED PIXEL" ,pCurStep->m_nTotalNoMaskedPixel);
	pIni->SetProfileInt(sSection, "NO MASKED PIXEL" ,pCurStep->m_nTotalNoMaskedPixel);
	pIni->SetProfileInt(sSection, "COLOR BIT COUNT",pCurStep->m_nBitCount);

	pIni->SetProfileString(sSection, "REF-R-HighBit", sRefRdata);
	pIni->SetProfileString(sSection, "REF-G-HighBit", sRefGdata);
	pIni->SetProfileString(sSection, "REF-B-HighBit", sRefBdata);

	pIni->SetProfileString(sSection, "GRAB-R-HighBit", sGrabRdata);
	pIni->SetProfileString(sSection, "GRAB-G-HighBit", sGrabGdata);
	pIni->SetProfileString(sSection, "GRAB-B-HighBit", sGrabBdata);

	//----------------------//
	// No of 0-Pair Bit
	//----------------------//

	sRefRdata = _T("");
	sRefGdata = _T("");
	sRefBdata = _T("");
	sGrabRdata = _T("");
	sGrabGdata = _T("");
	sGrabBdata = _T("");

	sRefTempRdata = _T("");
	sRefTempGdata = _T("");
	sRefTempBdata = _T("");
	sGrabTempRdata = _T("");
	sGrabTempGdata = _T("");
	sGrabTempBdata = _T("");
	
	for(i=0; i<MAX_NO_COLOR_BITS-2; i++)
	{
		// Reference Image
		sRefTempRdata.Format("%d,",pCurStep->m_aRefNoLowBitPair[R_DATA][i]);
		sRefRdata += sRefTempRdata;
	
		sRefTempGdata.Format("%d,",pCurStep->m_aRefNoLowBitPair[G_DATA][i]);
		sRefGdata += sRefTempRdata;

		sRefTempBdata.Format("%d,",pCurStep->m_aRefNoLowBitPair[B_DATA][i]);
		sRefBdata += sRefTempRdata;

		// Grab Image
		sGrabTempRdata.Format("%d,",pCurStep->m_aGrabNoLowBitPair[R_DATA][i]);
		sGrabRdata += sGrabTempRdata;

		sGrabTempGdata.Format("%d,",pCurStep->m_aGrabNoLowBitPair[G_DATA][i]);
		sGrabGdata += sGrabTempRdata;

		sGrabTempBdata.Format("%d,",pCurStep->m_aGrabNoLowBitPair[B_DATA][i]);
		sGrabBdata += sGrabTempRdata;
	}

	// Reference Image
	sRefTempRdata.Format("%d",pCurStep->m_aRefNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2]);
	sRefRdata += sRefTempRdata;

	sRefTempGdata.Format("%d",pCurStep->m_aRefNoLowBitPair[G_DATA][MAX_NO_COLOR_BITS-2]);
	sRefGdata += sRefTempGdata;
	
	sRefTempBdata.Format("%d",pCurStep->m_aRefNoLowBitPair[B_DATA][MAX_NO_COLOR_BITS-2]);
	sRefBdata += sRefTempBdata;

	// Grab Image
	sGrabTempRdata.Format("%d",pCurStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2]);
	sGrabRdata += sGrabTempRdata;

	sGrabTempGdata.Format("%d",pCurStep->m_aGrabNoLowBitPair[G_DATA][MAX_NO_COLOR_BITS-2]);
	sGrabGdata += sGrabTempGdata;
	
	sGrabTempBdata.Format("%d",pCurStep->m_aGrabNoLowBitPair[B_DATA][MAX_NO_COLOR_BITS-2]);
	sGrabBdata += sGrabTempBdata;

	// Write
	pIni->SetProfileString(sSection, "REF-R-LowBitPair", sRefRdata);
	pIni->SetProfileString(sSection, "REF-G-LowBitPair", sRefGdata);
	pIni->SetProfileString(sSection, "REF-B-LowBitPair", sRefBdata);

	pIni->SetProfileString(sSection, "GRAB-R-LowBitPair", sGrabRdata);
	pIni->SetProfileString(sSection, "GRAB-G-LowBitPair", sGrabGdata);
	pIni->SetProfileString(sSection, "GRAB-B-LowBitPair", sGrabBdata);
	
	//----------------------//
	// No of 1-Pair Bit
	//----------------------//

	sRefRdata = _T("");
	sRefGdata = _T("");
	sRefBdata = _T("");
	sGrabRdata = _T("");
	sGrabGdata = _T("");
	sGrabBdata = _T("");

	sRefTempRdata = _T("");
	sRefTempGdata = _T("");
	sRefTempBdata = _T("");
	sGrabTempRdata = _T("");
	sGrabTempGdata = _T("");
	sGrabTempBdata = _T("");
	
	for(i=0; i<MAX_NO_COLOR_BITS-2; i++)
	{
		// Reference Image
		sRefTempRdata.Format("%d,",pCurStep->m_aRefNoHighBitPair[R_DATA][i]);
		sRefRdata += sRefTempRdata;
	
		sRefTempGdata.Format("%d,",pCurStep->m_aRefNoHighBitPair[G_DATA][i]);
		sRefGdata += sRefTempRdata;

		sRefTempBdata.Format("%d,",pCurStep->m_aRefNoHighBitPair[B_DATA][i]);
		sRefBdata += sRefTempRdata;

		// Grab Image
		sGrabTempRdata.Format("%d,",pCurStep->m_aGrabNoHighBitPair[R_DATA][i]);
		sGrabRdata += sGrabTempRdata;

		sGrabTempGdata.Format("%d,",pCurStep->m_aGrabNoHighBitPair[G_DATA][i]);
		sGrabGdata += sGrabTempRdata;

		sGrabTempBdata.Format("%d,",pCurStep->m_aGrabNoHighBitPair[B_DATA][i]);
		sGrabBdata += sGrabTempRdata;
	}

	// Reference Image
	sRefTempRdata.Format("%d",pCurStep->m_aRefNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2]);
	sRefRdata += sRefTempRdata;

	sRefTempGdata.Format("%d",pCurStep->m_aRefNoHighBitPair[G_DATA][MAX_NO_COLOR_BITS-2]);
	sRefGdata += sRefTempGdata;
	
	sRefTempBdata.Format("%d",pCurStep->m_aRefNoHighBitPair[B_DATA][MAX_NO_COLOR_BITS-2]);
	sRefBdata += sRefTempBdata;

	// Grab Image
	sGrabTempRdata.Format("%d",pCurStep->m_aGrabNoHighBit[R_DATA][MAX_NO_COLOR_BITS-2]);
	sGrabRdata += sGrabTempRdata;

	sGrabTempGdata.Format("%d",pCurStep->m_aGrabNoHighBit[G_DATA][MAX_NO_COLOR_BITS-2]);
	sGrabGdata += sGrabTempGdata;
	
	sGrabTempBdata.Format("%d",pCurStep->m_aGrabNoHighBit[B_DATA][MAX_NO_COLOR_BITS-2]);
	sGrabBdata += sGrabTempBdata;

	// Write
	pIni->SetProfileString(sSection, "REF-R-HighBitPair", sRefRdata);
	pIni->SetProfileString(sSection, "REF-G-HighBitPair", sRefGdata);
	pIni->SetProfileString(sSection, "REF-B-HighBitPair", sRefBdata);

	pIni->SetProfileString(sSection, "GRAB-R-HighBitPair", sGrabRdata);
	pIni->SetProfileString(sSection, "GRAB-G-HighBitPair", sGrabGdata);
	pIni->SetProfileString(sSection, "GRAB-B-HighBitPair", sGrabBdata);
}

void SaveTestResult(CString sResultPath)
{
	CStdioFile pFile;
	CFileException FileExcept;
	CString sTmp, sSection;

	CString sRdata,sGdata,sBdata;
	CString sTempRdata,sTempGdata,sTempBdata;

	//+change kwmoon 080910
//	if(!pFile.Open(sResultPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate , &FileExcept)) 
	if(!pFile.Open(sResultPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate| CFile::shareDenyNone , &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return;
	}
	pFile.SeekToEnd();
	pFile.Close();

	m_Ini.SetIniFile(sResultPath);

	sSection.Format("Step Count");
	m_Ini.SetProfileInt(sSection, "Total Step", pCurStep->GetStepNo());

	//Default Info
	sSection.Format("Step %d", pCurStep->GetStepNo());

	m_Ini.SetProfileString(sSection, "Step Name", pCurStep->m_sName);
	m_Ini.SetProfileInt(sSection, "Step Type", pCurStep->m_nStepType);
	m_Ini.SetProfileInt(sSection, "Test Type", pCurStep->m_nTestType);
	
	//+2007.10.17 Mod BY USY
	if(pCurStep->m_bTest == FALSE) 
	{
		sTmp = _T("SKIP");
		m_Ini.SetProfileString(sSection, "Result", sTmp);
		return;
	}
	//-

	if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		sTmp.Format(_T("%5.2f"), pCurStep->m_fElapsedTime);
		m_Ini.SetProfileString(sSection, "Time(s)", sTmp);
		return;
	}
	else
	{
		//Function Type
		m_Ini.SetProfileInt(sSection, "Function Type", pCurStep->m_nFuncType);

		if (pCurStep->m_bResult)
		{
			sTmp = _T("OK");
			m_Ini.SetProfileString(sSection, "Result", sTmp);
		}
		else
		{
			//090324
			sTmp = _T("NG");
			if(pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{
				m_Ini.SetProfileString(sSection, "AudioTest", "TRUE");

				if(pCurStep->m_bVideoTestResult == FALSE)
				{
					sTmp = _T("NG/V");
				}

				if(pCurStep->m_bAudioTestResult == FALSE)
				{
					sTmp = _T("NG/A");
				}

				if((pCurStep->m_bVideoTestResult == FALSE) && (pCurStep->m_bAudioTestResult == FALSE))
				{
					sTmp = _T("NG/AV");
				}
			}
			else if(!pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{
				m_Ini.SetProfileString(sSection, "AudioTest", "FALSE");
				if(pCurStep->m_bVideoTestResult == FALSE)
				{
					sTmp = _T("NG/V");
				}
			}
			else if(pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
			{
				m_Ini.SetProfileString(sSection, "AudioTest", "TRUE");

				if(pCurStep->m_bAudioTestResult == FALSE)
				{
					sTmp = _T("NG/A");
				}
			}
			else
			{
				m_Ini.SetProfileString(sSection, "AudioTest", "FALSE");
				sTmp = _T("NG");
			}
			
			m_Ini.SetProfileString(sSection, "Result", sTmp);
		}

		if(pCurStep->m_nFuncType == MEAS_BOOL)
		{	
			if(pCurStep->m_bMeasured == FALSE)
				sTmp.Format(_T("%s"), "FALSE");
			else
				sTmp.Format(_T("%s"), "TRUE");
		}
		else
			sTmp.Format(_T("%4.1f"), pCurStep->m_fMeasured);

		m_Ini.SetProfileString(sSection, "Measure Value", sTmp);

		// TARGET VALUE
		if(pCurStep->m_nFuncType == MEAS_BOOL)
		{	
			if(pCurStep->m_bNominal == FALSE)
				sTmp.Format(_T("%s"), "FALSE");
			else
				sTmp.Format(_T("%s"), "TRUE");
		}
		else
		{
			sTmp.Format(_T("%4.1f"), pCurStep->m_fNominal);
		}

		m_Ini.SetProfileString(sSection, "Target Value", sTmp);
			
		if(pCurStep->m_nFuncType != MEAS_BOOL)
		{
//			double fGap =  pCurStep->m_fMeasured - pCurStep->m_fNominal;
//			sTmp.Format(_T("%4.1f"), fGap);
//			m_Ini.SetProfileString(sSection, "Gap", sTmp);
			sTmp.Format(_T("%4.1f"), pCurStep->m_fLowLimit);
			m_Ini.SetProfileString(sSection, "Low Limit", sTmp);
			sTmp.Format(_T("%4.1f"), pCurStep->m_fHighLimit);
			m_Ini.SetProfileString(sSection, "High Limit", sTmp);
			sTmp = pCurStep->m_sUnit;
			m_Ini.SetProfileString(sSection, "Unit", sTmp);
			
		}

		sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);
		m_Ini.SetProfileString(sSection, "Time(s)", sTmp);

		m_Ini.SetProfileString(sSection, "Grab Image", pCurStep->m_strGrabPath);
		m_Ini.SetProfileString(sSection, "Ref Image", pCurStep->m_strRefPath);
		m_Ini.SetProfileString(sSection, "Proc Image", pCurStep->m_strProcPath);
		m_Ini.SetProfileString(sSection, "Mask Image", pCurStep->m_strMaskPath);

		//+ 2007.7.23 Add BY USY
		m_Ini.SetProfileString(sSection, "Message", pCurStep->m_strMsg);
		//-

		//+ 2007.10.15 Move BY USY
		if(pCurStep->m_bRunAudioTest == TRUE)
		{
			//Audio Target, Measure, Level
			m_Ini.SetProfileInt(sSection, "Audio Left Freq Target" ,pCurStep->m_nLeftFreq[0]);
			m_Ini.SetProfileInt(sSection, "Audio Right Freq Target" ,pCurStep->m_nRightFreq[0]);
			m_Ini.SetProfileInt(sSection, "Audio Left Freq Measure" ,pCurStep->m_nLeftFreq[1]);
			m_Ini.SetProfileInt(sSection, "Audio Right Freq Measure" ,pCurStep->m_nRightFreq[1]);

			m_Ini.SetProfileInt(sSection, "Audio Left Level Target" ,pCurStep->m_nLeftLevel[0]);
			m_Ini.SetProfileInt(sSection, "Audio Right Level Target" ,pCurStep->m_nRightLevel[0]);
			m_Ini.SetProfileInt(sSection, "Audio Left Level Measure" ,pCurStep->m_nLeftLevel[1]);
			m_Ini.SetProfileInt(sSection, "Audio Right Level Measure" ,pCurStep->m_nRightLevel[1]);

			m_Ini.SetProfileInt(sSection, "Audio Margin", pCurStep->m_nAudioMargin);
		}
		//-

		sRdata = _T("");
		sGdata = _T("");
		sBdata = _T("");

		sTempRdata = _T("");
		sTempGdata = _T("");
		sTempBdata = _T("");

		//+add kwmoon 070807
		for(int i=0; i<NO_DIFFERENT_POINT_BLOCK-1;i++)
		{
			sTempRdata.Format("%d,",pCurStep->m_aNoDifferentPoint[R_DATA][i]);
			sRdata += sTempRdata;

			sTempGdata.Format("%d,",pCurStep->m_aNoDifferentPoint[G_DATA][i]);
			sGdata += sTempGdata;
			
			sTempBdata.Format("%d,",pCurStep->m_aNoDifferentPoint[B_DATA][i]);
			sBdata += sTempBdata;
		}
		sTempRdata.Format("%d",pCurStep->m_aNoDifferentPoint[R_DATA][NO_DIFFERENT_POINT_BLOCK-1]);
		sRdata += sTempRdata;

		sTempGdata.Format("%d",pCurStep->m_aNoDifferentPoint[G_DATA][NO_DIFFERENT_POINT_BLOCK-1]);
		sGdata += sTempGdata;
		
		sTempBdata.Format("%d",pCurStep->m_aNoDifferentPoint[B_DATA][NO_DIFFERENT_POINT_BLOCK-1]);
		sBdata += sTempBdata;

		m_Ini.SetProfileString(sSection, "R-Diff", sRdata);
		m_Ini.SetProfileString(sSection, "G-Diff", sGdata);
		m_Ini.SetProfileString(sSection, "B-Diff", sBdata);

		//-------------------------------------------------//
		
		SaveLineTestData(&m_Ini,sSection);
/*
		CString sRefRdata = _T("");
		CString sRefGdata = _T("");
		CString sRefBdata = _T("");
		CString sGrabRdata = _T("");
		CString sGrabGdata = _T("");
		CString sGrabBdata = _T("");

		CString sRefTempRdata = _T("");
		CString sRefTempGdata = _T("");
		CString sRefTempBdata = _T("");
		CString sGrabTempRdata = _T("");
		CString sGrabTempGdata = _T("");
		CString sGrabTempBdata = _T("");
		
		//----------------------//
		// No of High Bit
		//----------------------//
		for(int i=0; i<MAX_NO_COLOR_BITS-1; i++)
		{
			// Reference Image
			sRefTempRdata.Format("%d,",pCurStep->m_aRefNoHighBit[R_DATA][i]);
			sRefRdata += sRefTempRdata;
		
			sRefTempGdata.Format("%d,",pCurStep->m_aRefNoHighBit[G_DATA][i]);
			sRefGdata += sRefTempRdata;

			sRefTempBdata.Format("%d,",pCurStep->m_aRefNoHighBit[B_DATA][i]);
			sRefBdata += sRefTempRdata;

			// Grab Image
			sGrabTempRdata.Format("%d,",pCurStep->m_aGrabNoHighBit[R_DATA][i]);
			sGrabRdata += sGrabTempRdata;

			sGrabTempGdata.Format("%d,",pCurStep->m_aGrabNoHighBit[G_DATA][i]);
			sGrabGdata += sGrabTempRdata;

			sGrabTempBdata.Format("%d,",pCurStep->m_aGrabNoHighBit[B_DATA][i]);
			sGrabBdata += sGrabTempRdata;
		}

		// Reference Image
		sRefTempRdata.Format("%d",pCurStep->m_aRefNoHighBit[R_DATA][MAX_NO_COLOR_BITS-1]);
		sRefRdata += sRefTempRdata;

		sRefTempGdata.Format("%d",pCurStep->m_aRefNoHighBit[G_DATA][MAX_NO_COLOR_BITS-1]);
		sRefGdata += sRefTempGdata;
		
		sRefTempBdata.Format("%d",pCurStep->m_aRefNoHighBit[B_DATA][MAX_NO_COLOR_BITS-1]);
		sRefBdata += sRefTempBdata;

		// Grab Image
		sGrabTempRdata.Format("%d",pCurStep->m_aGrabNoHighBit[R_DATA][MAX_NO_COLOR_BITS-1]);
		sGrabRdata += sGrabTempRdata;

		sGrabTempGdata.Format("%d",pCurStep->m_aGrabNoHighBit[G_DATA][MAX_NO_COLOR_BITS-1]);
		sGrabGdata += sGrabTempGdata;
		
		sGrabTempBdata.Format("%d",pCurStep->m_aGrabNoHighBit[B_DATA][MAX_NO_COLOR_BITS-1]);
		sGrabBdata += sGrabTempBdata;

		// Write
		m_Ini.SetProfileString(sSection, "REF-R-HighBit", sRefRdata);
		m_Ini.SetProfileString(sSection, "REF-G-HighBit", sRefGdata);
		m_Ini.SetProfileString(sSection, "REF-B-HighBit", sRefBdata);

		m_Ini.SetProfileString(sSection, "GRAB-R-HighBit", sGrabRdata);
		m_Ini.SetProfileString(sSection, "GRAB-G-HighBit", sGrabGdata);
		m_Ini.SetProfileString(sSection, "GRAB-B-HighBit", sGrabBdata);

		//----------------------//
		// No of 0-Pair Bit
		//----------------------//

		sRefRdata = _T("");
		sRefGdata = _T("");
		sRefBdata = _T("");
		sGrabRdata = _T("");
		sGrabGdata = _T("");
		sGrabBdata = _T("");

		sRefTempRdata = _T("");
		sRefTempGdata = _T("");
		sRefTempBdata = _T("");
		sGrabTempRdata = _T("");
		sGrabTempGdata = _T("");
		sGrabTempBdata = _T("");
		
		for(i=0; i<MAX_NO_COLOR_BITS-2; i++)
		{
			// Reference Image
			sRefTempRdata.Format("%d,",pCurStep->m_aRefNoLowBitPair[R_DATA][i]);
			sRefRdata += sRefTempRdata;
		
			sRefTempGdata.Format("%d,",pCurStep->m_aRefNoLowBitPair[G_DATA][i]);
			sRefGdata += sRefTempRdata;

			sRefTempBdata.Format("%d,",pCurStep->m_aRefNoLowBitPair[B_DATA][i]);
			sRefBdata += sRefTempRdata;

			// Grab Image
			sGrabTempRdata.Format("%d,",pCurStep->m_aGrabNoLowBitPair[R_DATA][i]);
			sGrabRdata += sGrabTempRdata;

			sGrabTempGdata.Format("%d,",pCurStep->m_aGrabNoLowBitPair[G_DATA][i]);
			sGrabGdata += sGrabTempRdata;

			sGrabTempBdata.Format("%d,",pCurStep->m_aGrabNoLowBitPair[B_DATA][i]);
			sGrabBdata += sGrabTempRdata;
		}

		// Reference Image
		sRefTempRdata.Format("%d",pCurStep->m_aRefNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2]);
		sRefRdata += sRefTempRdata;

		sRefTempGdata.Format("%d",pCurStep->m_aRefNoLowBitPair[G_DATA][MAX_NO_COLOR_BITS-2]);
		sRefGdata += sRefTempGdata;
		
		sRefTempBdata.Format("%d",pCurStep->m_aRefNoLowBitPair[B_DATA][MAX_NO_COLOR_BITS-2]);
		sRefBdata += sRefTempBdata;

		// Grab Image
		sGrabTempRdata.Format("%d",pCurStep->m_aGrabNoLowBitPair[R_DATA][MAX_NO_COLOR_BITS-2]);
		sGrabRdata += sGrabTempRdata;

		sGrabTempGdata.Format("%d",pCurStep->m_aGrabNoLowBitPair[G_DATA][MAX_NO_COLOR_BITS-2]);
		sGrabGdata += sGrabTempGdata;
		
		sGrabTempBdata.Format("%d",pCurStep->m_aGrabNoLowBitPair[B_DATA][MAX_NO_COLOR_BITS-2]);
		sGrabBdata += sGrabTempBdata;

		// Write
		m_Ini.SetProfileString(sSection, "REF-R-LowBitPair", sRefRdata);
		m_Ini.SetProfileString(sSection, "REF-G-LowBitPair", sRefGdata);
		m_Ini.SetProfileString(sSection, "REF-B-LowBitPair", sRefBdata);

		m_Ini.SetProfileString(sSection, "GRAB-R-LowBitPair", sGrabRdata);
		m_Ini.SetProfileString(sSection, "GRAB-G-LowBitPair", sGrabGdata);
		m_Ini.SetProfileString(sSection, "GRAB-B-LowBitPair", sGrabBdata);
		
		//----------------------//
		// No of 1-Pair Bit
		//----------------------//

		sRefRdata = _T("");
		sRefGdata = _T("");
		sRefBdata = _T("");
		sGrabRdata = _T("");
		sGrabGdata = _T("");
		sGrabBdata = _T("");

		sRefTempRdata = _T("");
		sRefTempGdata = _T("");
		sRefTempBdata = _T("");
		sGrabTempRdata = _T("");
		sGrabTempGdata = _T("");
		sGrabTempBdata = _T("");
		
		for(i=0; i<MAX_NO_COLOR_BITS-2; i++)
		{
			// Reference Image
			sRefTempRdata.Format("%d,",pCurStep->m_aRefNoHighBitPair[R_DATA][i]);
			sRefRdata += sRefTempRdata;
		
			sRefTempGdata.Format("%d,",pCurStep->m_aRefNoHighBitPair[G_DATA][i]);
			sRefGdata += sRefTempRdata;

			sRefTempBdata.Format("%d,",pCurStep->m_aRefNoHighBitPair[B_DATA][i]);
			sRefBdata += sRefTempRdata;

			// Grab Image
			sGrabTempRdata.Format("%d,",pCurStep->m_aGrabNoHighBitPair[R_DATA][i]);
			sGrabRdata += sGrabTempRdata;

			sGrabTempGdata.Format("%d,",pCurStep->m_aGrabNoHighBitPair[G_DATA][i]);
			sGrabGdata += sGrabTempRdata;

			sGrabTempBdata.Format("%d,",pCurStep->m_aGrabNoHighBitPair[B_DATA][i]);
			sGrabBdata += sGrabTempRdata;
		}

		// Reference Image
		sRefTempRdata.Format("%d",pCurStep->m_aRefNoHighBitPair[R_DATA][MAX_NO_COLOR_BITS-2]);
		sRefRdata += sRefTempRdata;

		sRefTempGdata.Format("%d",pCurStep->m_aRefNoHighBitPair[G_DATA][MAX_NO_COLOR_BITS-2]);
		sRefGdata += sRefTempGdata;
		
		sRefTempBdata.Format("%d",pCurStep->m_aRefNoHighBitPair[B_DATA][MAX_NO_COLOR_BITS-2]);
		sRefBdata += sRefTempBdata;

		// Grab Image
		sGrabTempRdata.Format("%d",pCurStep->m_aGrabNoHighBit[R_DATA][MAX_NO_COLOR_BITS-2]);
		sGrabRdata += sGrabTempRdata;

		sGrabTempGdata.Format("%d",pCurStep->m_aGrabNoHighBit[G_DATA][MAX_NO_COLOR_BITS-2]);
		sGrabGdata += sGrabTempGdata;
		
		sGrabTempBdata.Format("%d",pCurStep->m_aGrabNoHighBit[B_DATA][MAX_NO_COLOR_BITS-2]);
		sGrabBdata += sGrabTempBdata;

		// Write
		m_Ini.SetProfileString(sSection, "REF-R-HighBitPair", sRefRdata);
		m_Ini.SetProfileString(sSection, "REF-G-HighBitPair", sRefGdata);
		m_Ini.SetProfileString(sSection, "REF-B-HighBitPair", sRefBdata);

		m_Ini.SetProfileString(sSection, "GRAB-R-HighBitPair", sGrabRdata);
		m_Ini.SetProfileString(sSection, "GRAB-G-HighBitPair", sGrabGdata);
		m_Ini.SetProfileString(sSection, "GRAB-B-HighBitPair", sGrabBdata);
*/
	}
//	//+ 2007.7.23 Add BY USY
//	m_Ini.SetProfileString(sSection, "Message", pCurStep->m_strMsg);
	//-
//	if(pCurStep->m_aTestResult.GetSize() > 0 )
//	{
//		CString sTmp2;
//		for(int nTmp = 0; nTmp < pCurStep->m_aTestResult.GetSize(); nTmp++)
//		{
//			sTmp.Format("Work Result %d", nTmp+1);
//			sTmp2.Format("%s",pCurStep->m_aTestResult.GetAt(nTmp));
//			sTmp2.TrimLeft();
//			m_Ini.SetProfileString(sSection, sTmp,sTmp2  );
//		}
//	}

	return;
}
/*
void SaveResult2File(CString sRstPath, BOOL bSave2Txt, BOOL bSave2Html, BOOL bSave2Csv)
{
	CString sTxtPath, sHtmlPath, sCsvPath;
	sTxtPath = sRstPath;
	sTxtPath.Replace(".dat" , ".txt");
	sHtmlPath = sRstPath;
	sHtmlPath.Replace(".dat" , ".html");
	sCsvPath = sRstPath;
	sCsvPath.Replace(".dat" , ".csv");

	if(bSave2Txt == TRUE)
	{
		g_pView->m_ctrlSummaryGrid.SaveGrid(sTxtPath, flexFileTabText, COleVariant(long(TRUE)));
	}

	if(bSave2Html == TRUE)
	{
		SaveGridToHTML(&g_pView->m_ctrlSummaryGrid,(char*)(LPCTSTR)sHtmlPath);
	}
	if(bSave2Csv == TRUE)
	{
		g_pView->m_ctrlSummaryGrid.SaveGrid(sCsvPath, flexFileCommaText, COleVariant(long(TRUE)));
	}
}
*/
//+add kwmoon 080123
/*
void SaveRemoteLog2File(CString sRstPath, BOOL bSave2Txt, BOOL bSave2Html, BOOL bSave2Csv)
{
	CString sNewRstPath;
	sNewRstPath = sRstPath.Left(sRstPath.ReverseFind('\\'));

	CString sTxtPath, sHtmlPath, sCsvPath;

	sTxtPath  = sNewRstPath + "\\Time.txt";
	sHtmlPath = sNewRstPath + "\\Time.html";
	sCsvPath  = sNewRstPath + "\\Time.csv";

	if(bSave2Txt == TRUE)
	{
		ctrlRemoteComLogGrid.SaveGrid(sTxtPath, flexFileTabText, COleVariant(long(TRUE)));
	}

	if(bSave2Html == TRUE)
	{
		SaveGridToHTML(&ctrlRemoteComLogGrid,(char*)(LPCTSTR)sHtmlPath);
	}
	if(bSave2Csv == TRUE)
	{
		ctrlRemoteComLogGrid.SaveGrid(sCsvPath, flexFileCommaText, COleVariant(long(TRUE)));
	}
}
*/
//
//void SaveRemoteLog2File(CString sLog)
//{
//	CString sNewRstPath;
//	sNewRstPath = sRstPath.Left(sRstPath.ReverseFind('\\'));
//
//	CString sTxtPath, sHtmlPath, sCsvPath;
//
//	sTxtPath = sNewRstPath + "\\Time.txt";
//	sHtmlPath = sNewRstPath + "\\Time.html";
//	sCsvPath = sNewRstPath + "\\Time.csv";
//
//	if (bSave2Txt == TRUE)
//	{
//		ctrlRemoteComLogGrid.SaveGrid(sTxtPath, flexFileTabText, COleVariant(long(TRUE)));
//	}
//
//	if (bSave2Html == TRUE)
//	{
//		SaveGridToHTML(&ctrlRemoteComLogGrid, (char*)(LPCTSTR)sHtmlPath);
//	}
//	if (bSave2Csv == TRUE)
//	{
//		ctrlRemoteComLogGrid.SaveGrid(sCsvPath, flexFileCommaText, COleVariant(long(TRUE)));
//	}
//}

void DeleteVFTestList()
{
	POSITION Pos = CurrentSet->VF_TestInfo.GetHeadPosition();
	while(Pos != NULL)
	{
		delete CurrentSet->VF_TestInfo.GetNext(Pos);
	}
	CurrentSet->VF_TestInfo.RemoveAll();
}

BOOL OpenVFTestFile(CString sVFTestPath)
{
	POSITION PosStep = NULL;
	int nTCount = 0;
	CString sSection, sReadString, sTmp;
	
	sReadString = _T("");
	sTmp = _T("");

	DeleteVFTestList();

	CStdioFile pFile;
	CFileException FileExcept;

	if(!pFile.Open(sVFTestPath, CFile::modeRead | CFile::modeNoTruncate |CFile::shareDenyNone   , &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}

	pFile.SeekToBegin();
	
	
	PosStep = CurrentSet->VF_TestInfo.GetHeadPosition();
	while(1)
	{
		pFile.ReadString(sReadString);
		if(sReadString.IsEmpty()) break;

		CVFTestInfo* pVFTest = new CVFTestInfo ;
		
		//Use
		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->bRun = (BOOL)((atoi)(sTmp));

		//ChNo
		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->nChNo = atoi(sTmp);

		//Name
		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->strTestName = sTmp;

		//CheckType
		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->nCheckType = atoi(sTmp);
	
		//Target
/*		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->dTarget = atof(sTmp);
*/
		//Target
		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->dLowerLimit = atof(sTmp);

		//Target2
		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->dUpperLimit = atof(sTmp);

	//	sTmp = sReadString.Left(sReadString.GetLength());
	//	sTmp.TrimRight(); sTmp.TrimLeft();
	//	pVFTest->dUpperLimit = atof(sTmp);


		//Unit
/*		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->strUnit = sTmp;
*/
		//Time
		sTmp = sReadString.Left(sReadString.Find(":", 0));
		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		//sTmp = sReadString.Left(sReadString.GetLength());
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->nTime = atoi(sTmp);

//		sTmp = sReadString.Left(sReadString.Find(":", 0));
//		sReadString.Delete(0, sReadString.Find(":", 0) + 1);
//		sTmp.TrimRight(); sTmp.TrimLeft();
//		pVFTest->nTime = atoi(sTmp);

		//sMargin
		sTmp = sReadString.Left(sReadString.GetLength());
		//sReadString.Delete(0, sReadString.Find(":", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pVFTest->nMarginType = atoi(sTmp);

		CurrentSet->VF_TestInfo.AddTail(pVFTest);
	}
	pFile.Close();
	
	return TRUE;
}

BOOL OpenRemoteFile(CString sRemotePath)
{
	POSITION PosStep = NULL;
	int nTCount = 0;
	CString sSection, sReadString, sTmp;
	
	sReadString = _T("");
	sTmp = _T("");

	DeleteRemoteList();

	CStdioFile pFile;
	CFileException FileExcept;

	//+change kwmoon 080910
//	if(!pFile.Open(sRemotePath, CFile::modeRead | CFile::modeNoTruncate |CFile::shareExclusive  , &FileExcept)) 
	if(!pFile.Open(sRemotePath, CFile::modeRead | CFile::modeNoTruncate |CFile::shareDenyNone   , &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}

	pFile.SeekToBegin();
	pFile.ReadString(sTmp);
	
	
	PosStep = CurrentSet->RemoteList.GetHeadPosition();
	while(1)
	{
		pFile.ReadString(sReadString);
		if(sReadString.IsEmpty()) break;

		CRemoteCode* pRemote = new CRemoteCode ;
		
		//Code
		sTmp = sReadString.Left(sReadString.Find(",", 0));
		sReadString.Delete(0, sReadString.Find(",", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pRemote->sCode = sTmp;

		//Use
		sTmp = sReadString.Left(sReadString.Find(",", 0));
		sReadString.Delete(0, sReadString.Find(",", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pRemote->bUse = (BOOL)((atoi)(sTmp));

		//Name
		sTmp = sReadString.Left(sReadString.Find(",", 0));
		sReadString.Delete(0, sReadString.Find(",", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pRemote->sName = sTmp;

		//Description
		sTmp = sReadString.Left(sReadString.GetLength());
		sTmp.TrimRight(); sTmp.TrimLeft();
		pRemote->sDesc = sTmp;

		CurrentSet->RemoteList.AddTail(pRemote);
	}
	//090919
	pFile.Close();
	
	return TRUE;
}

//+add kwmoon 071224
BOOL OpenPatternFile(CString sPatternPath)
{
	POSITION PosStep = NULL;

	int nTCount = 0;

	//+add kwmoon 080110
	CString szHDMIRoiShape = _T("");
	
	CString sSection, sTmp;
	DeletePatternList();

	//+add kwmoon 071106
	if(!FileExists(sPatternPath))
	{
		sTmp.Format("%s\n File doesn't Exist!", sPatternPath);
		AfxMessageBox(sTmp); return FALSE;
	}

	m_Ini.SetIniFile(sPatternPath);
	sSection.Format("Pattern No Count");
	nTCount = m_Ini.GetProfileInt(sSection,"Total Count");

	//+add kwmoon 080110
	sSection.Format("HDMI Multimedia Player");
	CurrentSet->sHDMIRoiShape = m_Ini.GetProfileString(sSection, "RoiShape");
	//-

	//Position Initialize
	PosStep = CurrentSet->PatternList.GetHeadPosition();

	for(int nTmp = 0; nTmp < nTCount; nTmp++)
	{
		CPatternInfo* pPattern = new CPatternInfo ;

		sSection.Format("Pattern No %d", nTmp+1);
		
		pPattern->nPatternNo   = nTmp+1;
		pPattern->nFilterSize  = m_Ini.GetProfileInt(sSection, "FilterSize");
		pPattern->nNoFiltering = m_Ini.GetProfileInt(sSection, "NoFiltering");
		pPattern->bRepaint	   = (BOOL)m_Ini.GetProfileInt(sSection, "Repaint");
		pPattern->szRoiShape   = m_Ini.GetProfileString(sSection, "RoiShape");
		
		CurrentSet->PatternList.AddTail(pPattern);
	}

	return TRUE;
}

//+add kwmoon 071215
BOOL FindPatternInfo(int nPatternNo, int* pFilterSize, int* pNoFiltering, int* pRepaint, CString* szRoiShape)
{
	POSITION Pos = NULL;
	
	CPatternInfo* pPattern = NULL;

	CString sSection, sTmp;
	
	BOOL bFind = FALSE;

	Pos = CurrentSet->PatternList.GetHeadPosition();

	while(Pos)
	{
		pPattern = CurrentSet->PatternList.GetNext(Pos);

		if(pPattern->nPatternNo == nPatternNo)
		{
			*pFilterSize  = pPattern->nFilterSize;
			*pNoFiltering = pPattern->nNoFiltering;
			*pRepaint	  = (int)pPattern->bRepaint;
			*szRoiShape	  = pPattern->szRoiShape;

			bFind = TRUE; return TRUE;
		}
	}
	if(bFind == FALSE) 
	{
		return FALSE;
	}
	return TRUE;
}

//+del kwmoon 080411
/*
void SetDirPath(CString sModelFolder)
{
	if(!sModelFolder.IsEmpty())
	{
		CurrentSet->sModelFolder;					// Model Folder;
		CurrentSet->sGrabFolder = sModelFolder + "\\Grab\\";					// Grab Image Folder ( /ModelName / Grab / 2xxx_xx_xx_xx_xx_xx/
		CurrentSet->sRefFolder = sModelFolder + "\\Ref\\";						// Ref Image Folder( /Model Name / Ref)
		CurrentSet->sMaskFolder = sModelFolder + "\\Ref\\Mask\\";					// Mask Image Folder( /Model Name / Ref)
		CurrentSet->sResultFolder = sModelFolder + "\\Result";					// Test Result Folder
	}
}
*/
UINT hex2dec(const  char ccHex)
{
	if(ccHex >= '0' && ccHex <='9')	
	{
		return (ccHex - '0');
	} 
	else if(ccHex >= 'a' && ccHex <='f')	
	{
		return (ccHex - 'a' + 10);
	} 
	else if(ccHex >= 'A' && ccHex <='F')	
	{
		return (ccHex - 'A' + 10);
	} 
	else return 0;
}

// Convert hex CString to Decimal number
UINT hexCstr2decNum(const  CString csHex)
{
	return hexStr2DecNum((LPSTR)(LPCTSTR)csHex);
}

// Convert hex string to Decimal number
UINT hexStr2DecNum(char* cpcHex)
{
	UINT uiDecimal = 0, uiPos;
	char *cpTmp;
	
	cpTmp = strstr(cpcHex, "0x");
	if(cpTmp)	uiPos = cpTmp - cpcHex + 1;
	else		uiPos = 0;

	while( *(cpcHex+uiPos) )	
	{
		uiDecimal *= 0x10;
		uiDecimal += hex2dec(*(cpcHex+uiPos));
		uiPos++;
	}
	return uiDecimal;
}

unsigned long long hex64Cstr2decNum(const  CString csHex)
{
	return hex64Str2DecNum((LPSTR)(LPCTSTR)csHex);
}
unsigned long long hex64Str2DecNum(char* cpcHex)
{
	unsigned int uiPos;
	unsigned long long uiDecimal = 0;
	char *cpTmp;

	cpTmp = strstr(cpcHex, "0x");
	if (cpTmp)	uiPos = cpTmp - cpcHex + 1;
	else		uiPos = 0;

	while (*(cpcHex + uiPos))
	{
		uiDecimal *= 0x10;
		uiDecimal += hex2dec(*(cpcHex + uiPos));
		uiPos++;
	}
	return uiDecimal;
}

//+ 2007.10.19 Add BY USY
BOOL InitPatternGen(CString sComPort, DWORD wBaudRate)
{
	CString szCmdString;
	szCmdString.Format("%c",START_TERMINAL_MODE);
	
	if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(PatternGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			//+add kwmoon 071015 : [2in1] : _Wait(100)
			PatternGeneratorCtrl.PortClose(); _Wait(100);
			if(!PatternGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sPatternComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if(PatternGeneratorCtrl.m_bPortOpen == TRUE)
	{
		//+add kwmoon 071015 : [2in1] : _Wait(100)
		PatternGeneratorCtrl.PortClose(); _Wait(100);
		if(!PatternGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			//+add kwmoon 080313
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sPatternComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}

	// Send On-Line Mode On Cmd
	PatternGeneratorCtrl.SendCommString(szCmdString); 
	return TRUE;
}
BOOL InitHDMIGen(CString sComPort, DWORD wBaudRate)
{
	CString szCmdString;
	szCmdString.Format("%c", START_TERMINAL_MODE);

	if (HDMIGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if (HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			HDMIGeneratorCtrl.PortClose(); _Wait(100);
			//if (!HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)				
			if (HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)", CurrentSet->sHDMIComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if (HDMIGeneratorCtrl.m_bPortOpen == TRUE)
	{
		HDMIGeneratorCtrl.PortClose(); _Wait(100);
		//if (!HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)			
		if (HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)", CurrentSet->sHDMIComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}

	// Send On-Line Mode On Cmd
	HDMIGeneratorCtrl.SendCommString(szCmdString);
	HDMIGeneratorCtrl.m_nRemoteMode = ONLINE;

	if (CurrentSet->nHDMIGenType == 0) {
		Sleep(500);
		HDMIGeneratorCtrl.SetMHL_Control(FALSE);;
	}

	return TRUE;
}
BOOL InitDPGen(CString sComPort, DWORD wBaudRate)
{
	CString szCmdString;
	szCmdString.Format("%c", START_TERMINAL_MODE);

	if (DPGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if (DPGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			DPGeneratorCtrl.PortClose(); _Wait(100);
			//if (!HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)				
			if (DPGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)", CurrentSet->sDpgComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if (DPGeneratorCtrl.m_bPortOpen == TRUE)
	{
		DPGeneratorCtrl.PortClose(); _Wait(100);
		//if (!HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)			
		if (DPGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)", CurrentSet->sDpgComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}

	// Send On-Line Mode On Cmd
	DPGeneratorCtrl.SendCommString(szCmdString);
	DPGeneratorCtrl.m_nRemoteMode = ONLINE;

	//if (CurrentSet->nDP_TimeSel == 0) 
	{
		Sleep(500);
		DPGeneratorCtrl.SetTime_Control(CurrentSet->nDP_TimeSel);;
	}

	return TRUE;
}

BOOL InitAVC(CString sComPort, DWORD wBaudRate)
{
	
	if(gAvConvertorCtrl.m_bPortOpen == FALSE)
	{
		if(gAvConvertorCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			gAvConvertorCtrl.PortClose (); _Wait(100);
			if(!gAvConvertorCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvcComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if(gAvConvertorCtrl.m_bPortOpen== TRUE)
	{
		gAvConvertorCtrl.PortClose(); _Wait(100);
		if(!gAvConvertorCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvcComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}

	gAvConvertorCtrl.Init(); 

	return TRUE;
}

BOOL InitTVCommPort(CString sComPort, DWORD wBaudRate)
{
	if(TVCommCtrl.m_bPortOpen == FALSE)
	{
		if(TVCommCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			//+add kwmoon 071015 : [2in1] : _Wait(100)
			TVCommCtrl.PortClose(); _Wait(100);
			if(!TVCommCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if(TVCommCtrl.m_bPortOpen == TRUE)
	{
		//+add kwmoon 071015 : [2in1] : _Wait(100)
		TVCommCtrl.PortClose(); _Wait(100);
		if(!TVCommCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			//+add kwmoon 080313
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}
	return TRUE;
}

//+add kwmoon 080625
BOOL InitI2cAdcCommPort(CString sComPort, DWORD wBaudRate)
{
	if(I2cAdcCtrl.m_bPortOpen == FALSE)
	{
		if(I2cAdcCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			//+add kwmoon 071015 : [2in1] : _Wait(100)
			I2cAdcCtrl.PortClose(); _Wait(100);

			if(!I2cAdcCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sI2cAdcComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if(I2cAdcCtrl.m_bPortOpen == TRUE)
	{
		//+add kwmoon 071015 : [2in1] : _Wait(100)
		I2cAdcCtrl.PortClose(); _Wait(100);

		if(!I2cAdcCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			//+add kwmoon 080313
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sI2cAdcComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL InitAvSwitchBox(CString sComPort, DWORD wBaudRate)
{

	if(!CurrentSet->bUseAVSwitchBox) return TRUE;
	
	//=======================
	// AvSwitchBox Port Open
	//=======================
	if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
	{
		if(!AvSwitchBoxCtrl.Create(sComPort, wBaudRate)) 
		{
			AvSwitchBoxCtrl.PortClose(); _Wait(100);

			if(!AvSwitchBoxCtrl.Create(sComPort, wBaudRate))
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sAvSwitchBoxComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	
	//==================
	// AVSwitchBox Init
	//==================
	CString sCmd;

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sCmd.Format("Set Audio CH 1 %s",  AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	for(int nTmp = 1; nTmp <= MAX_DIO_PORT; nTmp++)
	{
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_DIGIT_OUT, MAX_AVSWITCH_WAIT_DELAY,  nTmp, 0))
		{
			sCmd.Format("Set DIO Port %d Off %s", nTmp, AvSwitchBoxCtrl.m_strErrMsg);
			AfxMessageBox(sCmd); return FALSE;
		}
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  1, DC_OUT_VOLTAGE_1_0))
	{
		sCmd.Format("Set DC OUT CH1 Volt to 1.0V %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  2, DC_OUT_VOLTAGE_1_0))
	{
		sCmd.Format("Set DC OUT CH2 Volt to 1.0V %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sCmd.Format("Set COMP CH 1 %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sCmd.Format("Set DVBS CH 1 %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sCmd.Format("Set VIDEO CH 1 %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sCmd.Format("Set Remote Type 1 %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_REMOTE_REPEAT_NO, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sCmd.Format("Set Remote Repeat No %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	return TRUE;
}

BOOL InitLNB_Detector(CString sComPort, DWORD wBaudRate)
{
	
	if(gLNBDetectorCtrl.m_bPortOpen == FALSE)
	{
		if(gLNBDetectorCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			gLNBDetectorCtrl.PortClose(); _Wait(100);
			if(!gLNBDetectorCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sLNBComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}

	gLNBDetectorCtrl.Init(); 

	return TRUE;
}
BOOL InitVfm_Detector(CString sComPort, DWORD wBaudRate)
{
	
	if(gVF1000Ctrl.m_bPortOpen == FALSE)
	{
		if(gVF1000Ctrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			gVF1000Ctrl.PortClose(); _Wait(100);
			if(!gVF1000Ctrl.Create(sComPort, wBaudRate) == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sVfmComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
		double dVol;
		gVF1000Ctrl.m_bSystemMixType = _IR_MIX_TYPE;//  _VF_ONLY_TYPE
#ifdef				DEBUG_IRLANTEST__CODE__
		CString strRead;
		if (gVF1000Ctrl.GetIRBlasterData(strRead, 1000))
#else
		if (gVF1000Ctrl.GetVoltageData(1, dVol))
#endif
		 {
			//sMsg += "M PASS";
		}
		else {

			gVF1000Ctrl.m_bSystemMixType = _VF_ONLY_TYPE;
			if (gVF1000Ctrl.GetVoltageData(1, dVol)) {
			//	sMsg += "V PASS";
			}
			else
			{
				CString szErrMsg;
				szErrMsg.Format("VF Responce Fail (%s)", CurrentSet->sVfmComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}

		}

	}

	gVF1000Ctrl.Init(); 

	return TRUE;
}


/*
//+add kwmoon 071120
void InitSysInfoGrid()
{
	ctrlSysInfoGrid.SetRedraw(FALSE);
	ctrlSysInfoGrid.SetAllowBigSelection(FALSE);
	ctrlSysInfoGrid.SetExtendLastCol(TRUE);

	ctrlSysInfoGrid.SetBackColorBkg(COLOR_WHITE);
	ctrlSysInfoGrid.SetForeColorFixed(RGB(51,102,153));
	ctrlSysInfoGrid.SetBackColor(RGB(247,247,231));
	ctrlSysInfoGrid.SetGridColor(RGB(210,216,176));

	//Fixed Rows & Cols
	ctrlSysInfoGrid.SetFixedRows(0);
	ctrlSysInfoGrid.SetFixedCols(0);
	
	//Grid Line Setting
	ctrlSysInfoGrid.SetGridLines(flexGridFlat);
	
	//Column Width
	ctrlSysInfoGrid.SetColWidth(0, 1400);

	//Alignment
	ctrlSysInfoGrid.SetColAlignment(0,flexAlignLeftCenter);

	//Rows & Cols Number
	ctrlSysInfoGrid.SetCols(2);
	ctrlSysInfoGrid.SetRows(5);

	ctrlSysInfoGrid.SetTextMatrix(0 , 0,"SYS NO");
	ctrlSysInfoGrid.SetTextMatrix(1 , 0,"RUN MODE");
	ctrlSysInfoGrid.SetTextMatrix(2 , 0,"TOTAL");
	ctrlSysInfoGrid.SetTextMatrix(3 , 0,"NG");
	ctrlSysInfoGrid.SetTextMatrix(4 , 0,"NG(%)");
	
	ctrlSysInfoGrid.SetRedraw(TRUE);
	ctrlSysInfoGrid.Refresh();
}
*/
//+add kwmoon 080710
BOOL CreateTimeDataFile(CString sDate, CString sTime)
{
	CloseTimeDataFile();// g_pView->m_bTimeDataFileOpen = FALSE;

	//+add kwmoon 081024
	int		nIndex  = 0;
	CString szTemp1 = _T("");
	CString szTemp2 = _T("");
	CString szTemp3 = _T("");

	CString sFolderPath = _T("");
	//+change kwmoon 081024
//	sFolderPath = CurrentSet->sModelFolder  + "\\Data\\" + sDate + "\\" + sTime + "_" + m_strWipId + "\\";

	//del PSH 081105
	//+change kwmoon 081024
/*	nIndex  = CurrentSet->sModelFolder.ReverseFind('\\');
	szTemp1 = CurrentSet->sModelFolder.Left(nIndex);
	
	nIndex  = szTemp1.ReverseFind('\\');
	szTemp2 = szTemp1.Left(nIndex);
	
	nIndex  = szTemp2.ReverseFind('\\');
	szTemp3 = CurrentSet->sModelFolder.Mid(nIndex+1,CurrentSet->sModelFolder.GetLength()-nIndex-1);

	szTemp1.Format("%s\\Data\\%s",g_pView->m_szExePath,szTemp3);
	
	sFolderPath  = szTemp1  + "\\" + sDate + "\\" + sTime + "_" + m_strWipId + "\\";  
	*/
	//-
	sFolderPath  = CurrentSet->sResultFolder;  
	

	if(!FileExists(sFolderPath)) CreateFullPath(sFolderPath);
	
	CString sFilePath = _T("");
	sFilePath = sFolderPath + sTime + ".tim";

	CFileException FileExcept;
	try {
		//+change kwmoon 080910
	//	if(!g_pView->m_pTimeDataFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite |CFile::shareExclusive , &FileExcept)) 
	//	if (!g_pView->m_pTimeDataFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &FileExcept))
		if (!g_pView->m_pTimeDataFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone))
		{
		/*	char szCause[255];
			FileExcept.GetErrorMessage(szCause, 255);
			AfxMessageBox(szCause);*/
			return FALSE;
		}
	}
	catch (CFileException *e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}

	g_pView->m_pTimeDataFile.SeekToBegin();

	g_pView->m_bTimeDataFileOpen = TRUE;
	g_pView->m_sTimFileName = sFilePath;

	return TRUE;
}

//+add kwmoon 080710
void CloseTimeDataFile()
{
	if(g_pView->m_bTimeDataFileOpen)
	{
		g_pView->m_pTimeDataFile.Close();

		//+add kwmoon 080716
		g_pView->m_bTimeDataFileOpen = FALSE;
	}
}

//+add kwmoon 080710
void AddTimeData(CString szTimeData)
{
	CString szData;
	szData = szTimeData + "\n";

	if(g_pView->m_bTimeDataFileOpen) g_pView->m_pTimeDataFile.WriteString(szData);
}

//+add kwmoon 080716
void SaveDetailTestResult()
{
	CFile pFile;
	CFileException FileExcept;

	CString sRdata			= _T("");
	CString sGdata			= _T("");
	CString sBdata			= _T("");
	CString sTempRdata		= _T("");
	CString sTempGdata		= _T("");
	CString sTempBdata		= _T("");
	CString sTemp			= _T("");

	CString szOutputString	= _T("");
	CString szResult		= _T("");
	CString szMeasure		= _T("");
	CString szLowLimit		= _T("");
	CString szHighLimit		= _T("");
	CString szUnit			= _T("");
	
	CString sResultPath = CurrentSet->sDetailResultDataPath;

	int nOutputStringLength = 0;

	//+change kwmoon 080910
//	if(!pFile.Open(sResultPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate , &FileExcept)) 
	if(!pFile.Open(sResultPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyNone , &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return;
	}
//	pFile.SeekToEnd();
//	pFile.Close();

	
	szOutputString.Format("StepNo,StepName,Result,Measure,LowerLimit,UpperLimit,Unit,RoiPixel,TargetPixel,GoodPixel,ErrorPixel1/AllowedPixel1,ErrorPixel2/AllowedPixel2,ErrorPixel3/AllowedPixel3,ErrorPixel4/AllowedPixel4,ErrorPixel5/AllowedPixel5,");

	for(int i=0; i<(NO_DIFFERENT_POINT_BLOCK-1);i++)
	{
		sTemp.Format("%d%%,",i+1);
		szOutputString += sTemp;	
	}
	szOutputString += "100%\r\n";	

	pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);

	PosStep = StepList.GetHeadPosition();       

	while(PosStep != NULL)
	{
		pCurStep = StepList.GetNext(PosStep);	
		
		// Skip or Process Step
		if((pCurStep->m_bTest == FALSE) || (pCurStep->m_nStepType == PROCESS_FUNCTION)) 
		{
			continue;
		}

		//=============//	
		// Test Result //
		//=============//
		if (pCurStep->m_bResult)
		{
			szResult = _T("OK");
		}
		else
		{
			szResult = _T("NG");

			if(pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{
				if(pCurStep->m_bVideoTestResult == FALSE)
				{
					szResult = _T("NG/V");
				}

				if(pCurStep->m_bAudioTestResult == FALSE)
				{
					szResult = _T("NG/A");
				}

				if((pCurStep->m_bVideoTestResult == FALSE) && (pCurStep->m_bAudioTestResult == FALSE))
				{
					szResult = _T("NG/AV");
				}
			}
			else if(!pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{
				if(pCurStep->m_bVideoTestResult == FALSE)
				{
					szResult = _T("NG/V");
				}
			}
			else if(pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
			{
				if(pCurStep->m_bAudioTestResult == FALSE)
				{
					szResult = _T("NG/A");
				}
			}
			else
			{
				szResult = _T("NG");
			}
		}

		//===============//
		// Measure Value //
		//===============//
		if(pCurStep->m_nFuncType == MEAS_BOOL)
		{	
			if(pCurStep->m_bMeasured == FALSE)
				szMeasure = "FALSE";
			else
				szMeasure = "TRUE";
		}
		else
		{
			szMeasure.Format("%4.1f", pCurStep->m_fMeasured);
		}

		//=========================//
		// Low & High Limit & Unit //
		//=========================//
		if(pCurStep->m_nFuncType != MEAS_BOOL)
		{
			szLowLimit.Format("%4.1f",  pCurStep->m_fLowLimit);
			szHighLimit.Format("%4.1f", pCurStep->m_fHighLimit);
			szUnit = pCurStep->m_sUnit;
		}

		//=====================//
		// RGB Difference Data //
		//=====================//


		//+change kwmoon 080716
	/*
		for(int i=0; i<NO_DIFFERENT_POINT_BLOCK-1;i++)
		{
			sTempRdata.Format("%d,",pCurStep->m_aNoDifferentPoint[R_DATA][i]);
			sRdata += sTempRdata;
		
			sTempGdata.Format("%d,",pCurStep->m_aNoDifferentPoint[G_DATA][i]);
			sGdata += sTempGdata;
			
			sTempBdata.Format("%d,",pCurStep->m_aNoDifferentPoint[B_DATA][i]);
			sBdata += sTempBdata;
		}

		sTempRdata.Format("%d",pCurStep->m_aNoDifferentPoint[R_DATA][NO_DIFFERENT_POINT_BLOCK-1]);
		sRdata += sTempRdata;

		sTempGdata.Format("%d",pCurStep->m_aNoDifferentPoint[G_DATA][NO_DIFFERENT_POINT_BLOCK-1]);
		sGdata += sTempGdata;
		
		sTempBdata.Format("%d",pCurStep->m_aNoDifferentPoint[B_DATA][NO_DIFFERENT_POINT_BLOCK-1]);
		sBdata += sTempBdata;
	*/
		char cData[3][10];
		char DifferentPointInfo[3][500];

		int i;
		for(i=0; i<3; i++)
		{
			memset(cData[i],NULL,10);
			memset(DifferentPointInfo[i],NULL,500);
		}

		for(i=0; i<NO_DIFFERENT_POINT_BLOCK;i++)
		{
			_itoa_s(pCurStep->m_aNoDifferentPoint[R_DATA][i],cData[R_DATA],10);
			_itoa_s(pCurStep->m_aNoDifferentPoint[G_DATA][i],cData[G_DATA],10);
			_itoa_s(pCurStep->m_aNoDifferentPoint[B_DATA][i],cData[B_DATA],10);

			strcat_s(DifferentPointInfo[R_DATA],cData[R_DATA]);
			strcat_s(DifferentPointInfo[G_DATA],cData[G_DATA]);
			strcat_s(DifferentPointInfo[B_DATA],cData[B_DATA]);
			
			if(i != (NO_DIFFERENT_POINT_BLOCK-1))
			{
				strcat_s(DifferentPointInfo[R_DATA],",");
				strcat_s(DifferentPointInfo[G_DATA],",");
				strcat_s(DifferentPointInfo[B_DATA],",");
			}
		}


		szOutputString.Format("%d,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d/%d,%d/%d,%d/%d,%d/%d,%d/%d,%s\r\n",
					   pCurStep->GetStepNo(),pCurStep->m_sName,szResult,szMeasure,szLowLimit,szHighLimit,szUnit,
					   pCurStep->m_nNoRoiPixel,
			   		   pCurStep->m_nNoTargetPixel,
					   pCurStep->m_nNoGoodPixel,
					   pCurStep->m_nNoBandErrorPixel[0],pCurStep->m_nNoAllowedBandErrorPixel[0],
					   pCurStep->m_nNoBandErrorPixel[1],pCurStep->m_nNoAllowedBandErrorPixel[1],
					   pCurStep->m_nNoBandErrorPixel[2],pCurStep->m_nNoAllowedBandErrorPixel[2],
					   pCurStep->m_nNoBandErrorPixel[3],pCurStep->m_nNoAllowedBandErrorPixel[3],
					   pCurStep->m_nNoBandErrorPixel[4],pCurStep->m_nNoAllowedBandErrorPixel[4],
					   DifferentPointInfo[R_DATA]);
	//				   sRdata);

		pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);

		szOutputString.Format(",,,,,,,,,,,,,,,%s\r\n",
					   DifferentPointInfo[G_DATA]);
	//				   sRdata);
		
		pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);
			
		szOutputString.Format(",,,,,,,,,,,,,,,%s\r\n",
					   DifferentPointInfo[B_DATA]);
	//				   sRdata);

		pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);
	}

	pFile.Close();
}

//add 090423
BOOL SaveAdcAdjResult()
{
	CStdioFile stdFile;
	CFileException fileExcept;
	CString szOutputString	= _T("");
	CString sTemp;
	CString sDay;
	CString sFileName;

	CTime curTime = CTime::GetCurrentTime();

	sDay = curTime.Format(_T("%Y%m%d")); 
	sTemp.Format(_T("%s\\TestResult\\ADC_Data"), g_pView->m_szExePath);
	

	if(!FileExists(sTemp))   CreateFullPath(sTemp);

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	sFileName.Format(_T("%s\\ADC_%s_%d.txt"),sTemp, sDay, CurrentSet->nSystemNo); 

	szOutputString.Format("ChassisName,ModelName,StepNo,StepName,Offset1(R,G,B),Offset2(R,G,B),Gain(R,G,B),Target(R,G,B),Margin");

	hFind = FindFirstFile(sFileName, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		if(!stdFile.Open( sFileName, CFile::modeCreate | CFile::modeWrite, &fileExcept))
		{
			return FALSE;
		}
		stdFile.WriteString(szOutputString);
		stdFile.WriteString(_T("\n"));
		stdFile.Close();
		return TRUE;
	}
	FindClose(hFind);

	if(!stdFile.Open(sFileName, CFile::modeWrite, &fileExcept))
	{
        return FALSE;
    }
	stdFile.SeekToEnd(); 


	PosStep = StepList.GetHeadPosition();       

	while(PosStep != NULL)
	{
		pCurStep = StepList.GetNext(PosStep);	
		
		// Skip or Process Step
		if((pCurStep->m_bTest == FALSE) || (pCurStep->m_nTestType != ADC_TEST)) 
		{
			continue;
		}

		if(pCurStep->m_bAdcValueCheckStep)
		{

			szOutputString.Format("%s,%s,%d,%s,%s\r\n",
						   CurrentSet->sChassisName, CurrentSet->sModelName,pCurStep->GetStepNo(),pCurStep->m_sName, pCurStep->m_szAdcValue);
			
			TRY 
			{
			stdFile.WriteString(szOutputString);
			}
			CATCH (CFileException, e) 
			{
				sTemp = _T("Error occurs while saving ADC Result");
				AfxMessageBox(sTemp);  
				e->Delete();
				stdFile.Close();
				
				return FALSE;
			}
			END_CATCH
		}
	}
	stdFile.Close();
	
	return TRUE;
}

//+change kwmoon 080904
BOOL CreateTestParamIniFile(CString sTestParamFolder)
{
	CurrentSet->sTestParamIni = sTestParamFolder + "\\" + "TestParam.Ini";
	CFile pFile;
	CFileException FileExcept;
	CString sTmp;

	//+change kwmoon 080910
//	if(!pFile.Open(CurrentSet->sTestParamIni, CFile::modeCreate | CFile::modeWrite, &FileExcept)) 
	if(!pFile.Open(CurrentSet->sTestParamIni, CFile::modeCreate | CFile::modeWrite| CFile::shareDenyNone, &FileExcept)) 
	{
		char szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	m_Ini.SetIniFile(CurrentSet->sTestParamIni);

	//===================================================
	// Set Allowed Error Pixel Percentage for Each Bands
	//===================================================
	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[0]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band1 Error Pixel",sTmp);

	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[1]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band2 Error Pixel",sTmp);

	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[2]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band3 Error Pixel",sTmp);

	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[3]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band4 Error Pixel",sTmp);
	
	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[4]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band5 Error Pixel",sTmp);

	//=======================================
	// Set Audio Margin (Used in CheckAudio)
	//=======================================
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Level Margin",CurrentSet->nLevelMargin);
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Freq Margin",CurrentSet->nFreqMargin);


	//=======================================
	// Set Audio Forced Check
	//=======================================

	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HP_L", CurrentSet->nAudioForce_HP_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HP_R", CurrentSet->nAudioForce_HP_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_ANALOG_L", CurrentSet->nAudioForce_Analog_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_ANALOG_R", CurrentSet->nAudioForce_Analog_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HDMI_L", CurrentSet->nAudioForce_HDMI_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HDMI_R", CurrentSet->nAudioForce_HDMI_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_RGB_L", CurrentSet->nAudioForce_RGB_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_RGB_R", CurrentSet->nAudioForce_RGB_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_SCART_L", CurrentSet->nAudioForce_SCART_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_SCART_R", CurrentSet->nAudioForce_SCART_R);

	/*int
int			nAudioForce_HP_R		;
int			nAudioForce_HP_L		;
int			nAudioForce_Analog_L	;
int			nAudioForce_Analog_R	;
int			nAudioForce_HDMI_L		;
int			nAudioForce_HDMI_R		;
int			nAudioForce_RGB_L		;
int			nAudioForce_RGB_R		;
int			nAudioForce_SCART_L		;
int			nAudioForce_SCART_R		;*/

	return TRUE;
}

BOOL SaveTestParamIniFile(CString sIniPath)
{
	CFile pFile;
	CFileException FileExcept;
	CString sTmp = _T("");

	//+change kwmoon 080910
	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::modeWrite| CFile::shareDenyNone, &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	m_Ini.SetIniFile(sIniPath);

	//===================================================
	// Set Allowed Error Pixel Percentage for Each Bands
	//===================================================
	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[0]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band1 Error Pixel",sTmp);
	
	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[1]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band2 Error Pixel",sTmp);
	
	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[2]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band3 Error Pixel",sTmp);
	
	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[3]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band4 Error Pixel",sTmp);

	sTmp.Format("%f",CurrentSet->fAllowedBandErrorPixelPercentage[4]);
	m_Ini.SetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band5 Error Pixel",sTmp);

	//=======================================
	// Set Audio Margin (Used in CheckAudio)
	//=======================================
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Level Margin",CurrentSet->nLevelMargin);
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Freq Margin",CurrentSet->nFreqMargin);



	//=======================================
	// Set Audio Forced Check
	//=======================================

	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HP_L", CurrentSet->nAudioForce_HP_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HP_R", CurrentSet->nAudioForce_HP_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_ANALOG_L", CurrentSet->nAudioForce_Analog_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_ANALOG_R", CurrentSet->nAudioForce_Analog_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HDMI_L", CurrentSet->nAudioForce_HDMI_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HDMI_R", CurrentSet->nAudioForce_HDMI_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_RGB_L", CurrentSet->nAudioForce_RGB_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_RGB_R", CurrentSet->nAudioForce_RGB_R);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_SCART_L", CurrentSet->nAudioForce_SCART_L);
	m_Ini.SetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_SCART_R", CurrentSet->nAudioForce_SCART_R);


	m_Ini.SetProfileInt(VIDEO_MARGIN_S, "MOVING_PIC_NO_OF_COLOR", CurrentSet->nNoUsedColors);

	/*int
int			nAudioForce_HP_R		;
int			nAudioForce_HP_L		;
int			nAudioForce_Analog_L	;
int			nAudioForce_Analog_R	;
int			nAudioForce_HDMI_L		;
int			nAudioForce_HDMI_R		;
int			nAudioForce_RGB_L		;
int			nAudioForce_RGB_R		;
int			nAudioForce_SCART_L		;
int			nAudioForce_SCART_R		;*/
	//int			nAudioForce_HP_R;
	//int			nAudioForce_HP_L;
	//int			nAudioForce_Analog_L;
	//int			nAudioForce_Analog_R;
	//int			nAudioForce_HDMI_L;
	//int			nAudioForce_HDMI_R;
	//int			nAudioForce_RGB_L;
	//int			nAudioForce_RGB_R;
	//int			nAudioForce_SCART_L;
	//int			nAudioForce_SCART_R;

	return TRUE;
}

BOOL OpenTestParamIniFile(CString sIniPath)
{
	CFile pFile;
	CFileException FileExcept;
	
	//+change kwmoon 080910
//	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::modeWrite, &FileExcept)) 
	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::shareDenyNone, &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	m_Ini.SetIniFile(sIniPath);

	//====================================================
	// Read Allowed Error Pixel Percentage for Each Bands
	//====================================================
	CurrentSet->fAllowedBandErrorPixelPercentage[0] = atof(m_Ini.GetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band1 Error Pixel"));
	CurrentSet->fAllowedBandErrorPixelPercentage[1] = atof(m_Ini.GetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band2 Error Pixel"));
	CurrentSet->fAllowedBandErrorPixelPercentage[2] = atof(m_Ini.GetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band3 Error Pixel"));
	CurrentSet->fAllowedBandErrorPixelPercentage[3] = atof(m_Ini.GetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band4 Error Pixel"));
	CurrentSet->fAllowedBandErrorPixelPercentage[4] = atof(m_Ini.GetProfileString(ALLOWED_ERROR_PIXEL_S, "Allowed Band5 Error Pixel"));

	int nStart = 0, nEnd = 0;
	
	CString sTmp = _T(""), sValue = _T("");
	
	//========================================
	// Read Audio Margin (Used in CheckAudio)
	//========================================
	CurrentSet->nLevelMargin = m_Ini.GetProfileInt(AUDIO_MARGIN_S, "Audio Level Margin");
	CurrentSet->nFreqMargin  = m_Ini.GetProfileInt(AUDIO_MARGIN_S, "Audio Freq Margin");

	if(CurrentSet->nLevelMargin == 0) CurrentSet->nLevelMargin = 45;
	if(CurrentSet->nFreqMargin == 0)  CurrentSet->nFreqMargin  = 15;


	//= ======================================
	// Get Audio Forced Check
	//=======================================
		
	CurrentSet->nAudioForce_HP_L		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HP_L");
	CurrentSet->nAudioForce_HP_R		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HP_R");
	CurrentSet->nAudioForce_Analog_L	= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_ANALOG_L");
	CurrentSet->nAudioForce_Analog_R	= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_ANALOG_R");
	CurrentSet->nAudioForce_HDMI_L		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HDMI_L");
	CurrentSet->nAudioForce_HDMI_R		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_HDMI_R");
	CurrentSet->nAudioForce_RGB_L		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_RGB_L");
	CurrentSet->nAudioForce_RGB_R		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_RGB_R");
	CurrentSet->nAudioForce_SCART_L		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_SCART_L");
	CurrentSet->nAudioForce_SCART_R		= m_Ini.GetProfileInt(FORCE_CHECK_S, "AUDIO_FORCE_SCART_R");

	if (CurrentSet->nAudioForce_HP_L == 0)		CurrentSet->nAudioForce_HP_L = 1000;
	if (CurrentSet->nAudioForce_HP_R == 0)		CurrentSet->nAudioForce_HP_R = 1000;
	if (CurrentSet->nAudioForce_Analog_L == 0)	CurrentSet->nAudioForce_Analog_L = 400;
	if (CurrentSet->nAudioForce_Analog_R == 0)	CurrentSet->nAudioForce_Analog_R = 1000;
	if (CurrentSet->nAudioForce_HDMI_L == 0)	CurrentSet->nAudioForce_HDMI_L = 400;
	if (CurrentSet->nAudioForce_HDMI_R == 0)	CurrentSet->nAudioForce_HDMI_R = 1000;
	if (CurrentSet->nAudioForce_RGB_L == 0)		CurrentSet->nAudioForce_RGB_L = 400;
	if (CurrentSet->nAudioForce_RGB_R == 0)		CurrentSet->nAudioForce_RGB_R = 1000;
	if (CurrentSet->nAudioForce_SCART_L == 0)	CurrentSet->nAudioForce_SCART_L = 400;
	if (CurrentSet->nAudioForce_SCART_R == 0)	CurrentSet->nAudioForce_SCART_R = 1000;
	//m_Ini.SetProfileInt(VIDEO_MARGIN_S, "MOVING_PIC_NO_OF_COLOR", CurrentSet->nNoUsedColors);
	CurrentSet->nNoUsedColors = m_Ini.GetProfileInt(VIDEO_MARGIN_S, "MOVING_PIC_NO_OF_COLOR");
	if (CurrentSet->nNoUsedColors == 0)	CurrentSet->nNoUsedColors = 5000;

	//int			nAudioForce_HP_R;
	//int			nAudioForce_HP_L;
	//int			nAudioForce_Analog_L;
	//int			nAudioForce_Analog_R;
	//int			nAudioForce_HDMI_L;
	//int			nAudioForce_HDMI_R;
	//int			nAudioForce_RGB_L;
	//int			nAudioForce_RGB_R; Cmd : 0x1012 (LDCMD_GET_BRIGHT_IMAGE)
	//int			nAudioForce_SCART_L;
	//int			nAudioForce_SCART_R;

	return TRUE;
}
BOOL OpenSystemInfoFile(CString sIniPath)
{
	CFile pFile;
	CFileException FileExcept;
	CString sTmp;
	
	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::shareDenyNone, &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}

	
	pFile.Close();



	FILE *l_fp;
	#if 1

	g_nGrabberType = -1;
	g_nUseNoScanType = -1;
	g_nSysRobotType = -1;
	g_nCommandOnlyType = FALSE;


	fopen_s(&l_fp, (LPCSTR)sIniPath, "r");
	char lbuf[1024];
	if (l_fp)
	{
		while (fgets(lbuf, 1024, l_fp) != NULL)
		{
			for (int i = 0; i < strlen(lbuf); i++)
			{
				if ((lbuf[i] == '\r') || (lbuf[i] == '\n'))
				{
					lbuf[i] = 0;
					break;
				}
			}
			sTmp = lbuf;

			sTmp.MakeUpper();
			if ((sTmp.Find("[") >= 0)||(sTmp.Find("//") >= 0))
			{
				continue;
			}
			else if (sTmp.Find("GRABBER") >= 0)
			{
				if (sTmp.Find("UHD") > 0)
				{
					g_nGrabberType = UHD_GRABBER;
				}
				else if (sTmp.Find("FHD") > 0)//(sTmp == "FHD")
				{
					g_nGrabberType = FHD_GRABBER;
				}
				else {
					AfxMessageBox("Grabber Type Error");
					return FALSE;
				}
			}
			else if (sTmp.Find("SYSTEM") >= 0)
			{


				if (sTmp.Find("AUTO") > 0) // (sTmp == "AUTO")
				{
					g_nSysType = AUTO_SYS;
					g_nSysRobotType = MANUAL_SYS;
				}
				else if (sTmp.Find("MANUAL") > 0) //(sTmp == "MANUAL")
				{
					g_nSysType = MANUAL_SYS;
					g_nSysRobotType = MANUAL_SYS;
				}
				else if (sTmp.Find("ROBOT") > 0) //(sTmp == "ROBOT")
				{
					g_nSysType = AUTO_SYS;
					g_nSysRobotType = AUTO_SYS;
				}			
				else {
					AfxMessageBox("System Type Error");
					return FALSE;
				}
			}
			else if (sTmp.Find("SCAN") >= 0)//SCANNER USE
			{
				if (g_nSysRobotType == AUTO_SYS)
				{
					g_nUseNoScanType = TRUE;
				}
				else
				{
					if (sTmp.Find("YES") >= 0)//(sTmp == "YES")
					{
						g_nUseNoScanType = FALSE;
					}
					else
					{
						g_nUseNoScanType = TRUE;//g_nSysType = AUTO_SYS
					}
				}
			}
			else if (sTmp.Find("COMMAND") >= 0)//COMMAND ONLY
			{
				
					if (sTmp.Find("YES") >= 0)
					{
						g_nCommandOnlyType = TRUE;
					}
					else
					{
						g_nCommandOnlyType = FALSE;//g_nSysType = AUTO_SYS
					}
				
			}
		}
		fclose(l_fp);
	}
	
	if((g_nGrabberType == -1)||
	(g_nUseNoScanType == -1)||
	(g_nSysRobotType == -1))
	{
		AfxMessageBox("DftSysInfo.ini File Error");
		return FALSE;
	}
#else

	m_Ini.SetIniFile(sIniPath);

	sTmp = m_Ini.GetProfileString(DFT_SYSTEM_INFO_S, "Grabber Type");
	sTmp.TrimLeft();
	sTmp.TrimRight();

	sTmp.MakeUpper();

	if(sTmp == "UHD"){ g_nGrabberType = UHD_GRABBER;}
	else if(sTmp == "FHD"){ g_nGrabberType = FHD_GRABBER;}
	else {
		AfxMessageBox("Grabber Type Error");
		return FALSE;
	}

	sTmp = m_Ini.GetProfileString(DFT_SYSTEM_INFO_S, "System Type");
	sTmp.TrimLeft();
	sTmp.TrimRight();

	sTmp.MakeUpper();

	if(sTmp == "AUTO")
	{
		g_nSysType = AUTO_SYS;
		g_nSysRobotType = MANUAL_SYS;
	}
	else if (sTmp == "MANUAL")
	{
		g_nSysType = MANUAL_SYS;
		g_nSysRobotType = MANUAL_SYS;
	}
	else if (sTmp == "ROBOT")
	{ 
		g_nSysType = AUTO_SYS; 
		g_nSysRobotType = AUTO_SYS;
	}
	else {
		AfxMessageBox("System Type Error");
		return FALSE;
	}


	if (g_nSysRobotType == AUTO_SYS)
	{
		g_nUseNoScanType = TRUE;
	}
	else
	{
		sTmp = m_Ini.GetProfileString(DFT_SYSTEM_INFO_S, "No Scan Type");
		sTmp.TrimLeft();
		sTmp.TrimRight();
		sTmp.MakeUpper();

		if (sTmp == "YES")
		{
			g_nUseNoScanType = TRUE;
		}
		else
		{
			g_nUseNoScanType = FALSE;
		}
	}
#endif
	//sTmp = m_Ini.GetProfileString(DFT_SYSTEM_INFO_S, "Robot Use Type");
	//sTmp.TrimLeft();
	//sTmp.TrimRight();
	//sTmp.MakeUpper();

	//if (g_nSysType == AUTO_SYS)
	//{
	//	if (sTmp == "YES") { g_nSysRobotType = AUTO_SYS; }//CurrentSet->bUsePLCRobot == TRUE
	//	//else if(sTmp == "MANUAL"){ g_nSysRobotType = MANUAL_SYS;}
	//	else if (sTmp == "AUTO") { g_nSysRobotType = AUTO_SYS; }//CurrentSet->bUsePLCRobot == TRUE
	//	//else if(sTmp == "MANUAL"){ g_nSysRobotType = MANUAL_SYS;}
	//	else {
	//		g_nSysRobotType = MANUAL_SYS;
	//	}
	//}
	//else
	//{
	//	g_nSysRobotType = MANUAL_SYS;

	//}
	return TRUE;
}

BOOL OpenEPI_ConfigFile(CString sIniPath)
{
	CFile pFile;
	CFileException FileExcept;
	CString sTmp;

	BOOL lNotFound = 1;

	if (!pFile.Open(sIniPath, CFile::modeRead | CFile::shareDenyNone, &FileExcept))
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AddStringToStatus(szCause);
		return FALSE;
	}


	pFile.Close();



	FILE *l_fp;


	int lStart = 0;
	int lEnd = 0;
	int lComPt = 0;

	fopen_s(&l_fp, (LPCSTR)sIniPath, "r");
	char lbuf[1024];
	if (l_fp)
	{
		while (fgets(lbuf, 1024, l_fp) != NULL)
		{

			lStart = 0;
			for (int i = 0; i < strlen(lbuf); i++)
			{
				//if ((lbuf[i] == '\t') || (lbuf[i] == ' '))
				//{
				//	lStart = i;
				//}
				
				if ((lbuf[i] == '\r') || (lbuf[i] == '\n'))
				{
					lbuf[i] = 0;
					break;
				}
			}
			sTmp = lbuf;

			lEnd = sTmp.Find("//");
			if (lEnd >= 0)
				sTmp = sTmp.Left(lEnd);
			
			lComPt = sTmp.Find(",");
			if (lComPt < 2)
				continue;
			CString lsOption;
			CString lsDipStatus;

			lsOption = sTmp.Left(lComPt);
			lsDipStatus = sTmp.Mid(lComPt+1);

	/*		for (int i = 0; i < strlen(lbuf); i++)
			{				
				if ((lbuf[i] == '\r') || (lbuf[i] == '\n'))
				{
					lbuf[i] = 0;
					break;
				}
			}*/
			lsOption.Trim();
			lsDipStatus.Trim();

			if (lsDipStatus.GetLength() < 4)
				continue;
			if (lsOption.Compare(CurrentSet->sToolOption1) == 0)
			{
				CurrentSet->sDipStatus = lsDipStatus;
				lNotFound = 0;
				break;
			}
			
			
		}
		fclose(l_fp);
	}
	
	if (lNotFound == 1)
	{
		AddStringToStatus("EPI_Config.ini File Error");
		return FALSE;
	}	
	return TRUE;
}
#if 1
void SaveResultSummary(CString strWipid, BOOL bResult, CString sTime)
{
	CFile pFile;
	CFileException FileExcept;

	CString sTemp = _T("");
	CString szOutputString = _T("");
	CString szResultTotal = _T("");
	CString sStepNo = _T("");
	CString sRun = _T("");
	CString sResult = _T("");
	CString sAudioTarget = _T("");
	CString sAudioMeasure = _T("");
	CString sMeasured = _T("");
	CString sNominal = _T("");
	CString sLowLimit = _T("");
	CString sHighLimit = _T("");
	CString sElapsedTime = _T("");
	CString sMsg = _T("");
	BOOL	bAlreadyExist = FALSE;

	CString sResultPath;
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	CString sKeyDL_Log = _T("");
	CString sSystemNo = _T("");
	CString sDayTime = _T("");
	CString sSPEC_SEQ = _T("1");

	int		nSeqNo = 1;
	CString sSeqNo = _T("");
	CString sGMES_PortNo = _T("");

	nSeqNo = CurrentSet->nDftSeq + 1;
	sSeqNo.Format("%d", nSeqNo);

	sGMES_PortNo.Format("%d", CurrentSet->nGmesPort);

	CTime curTime = CTime::GetCurrentTime();

	sTemp2 = curTime.Format(_T("[%H%M%S]"));
	sDayTime = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));
	sDay = curTime.Format(_T("%Y%m%d"));
	sTemp.Format(_T("%s\\TestResult\\Summary"), g_pView->m_szExePath);

	if (!FileExists(sTemp))   CreateFullPath(sTemp);

	sResultPath.Format(_T("%s\\ResultSummary_%s.CSV"), sTemp, sDay);

	int nOutputStringLength = 0;

	if (FileExists(sResultPath))
	{
		bAlreadyExist = TRUE;
	}

	if (!pFile.Open(sResultPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyNone, &FileExcept))
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return;
	}

	if (!bAlreadyExist)
	{
		//szOutputString.Format("WipID,Chassis,Model,Time,Result,TestTime,NO,NAME, StepResult,MEASURE,TARGET,L-LIMIT,U-LIMIT,A_TARGET,A_MEASURE,TIME,MESSAG,HDMIVer\r\n");
#if 1
		szOutputString.Format("GMES PORT,Read_ID,Scan ID,INSP_JUDGE_CODE,A_MEASURE_1,A_TARGET_1,CHASSIS,HDMI_GEN_VER,PCBA_SW_VER,\
KEYCHECK_LOG,LOWER_LIMIT_1,MACHINE_NO,MEASURE_1,MESSAGE_1,RUN_1,SEQ,STEP_NAME_1,\
STEP_NO_1,TARGET_1,TIME_1,TOTAL_TIME,UPPER_LIMIT_1,ACTDTTM,INSP_SEQ,PCBAID_Write");
		for (int i = 1; i <= 42; i++)
		{
			sTemp.Format(",V%d", i);
			szOutputString += sTemp;
		}	
		
		for (int i = 1; i <= 6; i++)
		{
			sTemp.Format(",F%d", i);
			szOutputString += sTemp;
		}
		szOutputString += "\r\n";

#else
		szOutputString.Format(	"GMES PORT,SET_ID,INSP_JUDGE_CODE,A_MEASURE_1,A_TARGET_1,CHASSIS,HDMI_GEN_VER,PCBA_SW_VER,\
KEYCHECK_LOG,LOWER_LIMIT_1,MACHINE_NO,MEASURE_1,MESSAGE_1,RUN_1,SEQ,STEP_NAME_1,\
STEP_NO_1,TARGET_1,TIME_1,TOTAL_TIME,UPPER_LIMIT_1,ACTDTTM,INSP_SEQ,PCBAID_Write\r\n");
#endif
		pFile.Write((LPSTR)(LPCTSTR)szOutputString, szOutputString.GetLength() + 1);
	}

	pFile.SeekToEnd();

	//=============	
	// Test Result 
	//=============
	if (CurrentSet->bDrmKey) {
		sKeyDL_Log.Format("KEY%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
	}
	else {
		sKeyDL_Log.Format("DFT%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
	}

	sSystemNo.Format("%d", CurrentSet->nSystemNo);

	CString sPCBID_Rewrite;
	if (CurrentSet->bPCBID_Rewrite == 1)
	{
		sPCBID_Rewrite = "1";
	}
	else
	{
		sPCBID_Rewrite = "0";
	}

	if (bResult)
	{
		szResultTotal = _T("OK");

		sAudioTarget = _T("-");
		sAudioMeasure = _T("-");


		sLowLimit = _T("-");

		sMeasured = _T("-");
		sNominal = _T("-");
		sHighLimit = _T("-");
		sElapsedTime = _T("-");
		sResult = _T("-");
		sSPEC_SEQ = "1";


//		szOutputString.Format("SET_ID,INSP_JUDGE_CODE,A_MEASURE_1,A_TARGET_1,CHASSIS,
//		HDMI_GEN_VER,\
//KEYCHECK_LOG,LOWER_LIMIT_1,MACHINE_NO,MEASURE_1,MESSAGE_1,RUN_1,SEQ,STEP_NAME_1,\
//STEP_NO_1,TARGET_1,TIME_1,TOTAL_TIME,UPPER_LIMIT_1,ACTDTTM,INSP_SEQ\r\n");

#if 1
		szOutputString.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,  ,%s,%s,%s,%s,%s,%s,%s,%s",
			sGMES_PortNo, CurrentSet->sPCBID_Read, CurrentSet->sPCBID_Scan, szResultTotal, sAudioMeasure, sAudioTarget,CurrentSet->sChassisName,
			HDMIGeneratorCtrl.m_FW_Ver, CurrentSet->sCPUVersionRead, sKeyDL_Log, sLowLimit, sSystemNo,sMeasured,
			sMsg, sRun, sSeqNo,/* pStep->m_sName,*/sStepNo, 
			sNominal, sElapsedTime, sTime, sHighLimit, sDayTime, sSPEC_SEQ, sPCBID_Rewrite
			);
#else
		szOutputString.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,  ,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
			sGMES_PortNo,strWipid,szResultTotal, sAudioMeasure, sAudioTarget,CurrentSet->sChassisName,
			HDMIGeneratorCtrl.m_FW_Ver, CurrentSet->sCPUVersionRead, sKeyDL_Log, sLowLimit, sSystemNo,sMeasured,
			sMsg, sRun, sSeqNo,/* pStep->m_sName,*/sStepNo, 
			sNominal, sElapsedTime, sTime, sHighLimit, sDayTime, sSPEC_SEQ, sPCBID_Rewrite
			);
#endif
		//pFile.Write((LPSTR)(LPCTSTR)szOutputString, szOutputString.GetLength() + 1);
	}
	else
	{
		szResultTotal = _T("NG");
		POSITION Position = StepList.GetHeadPosition();
		while (Position)
		{
			CStep* pStep = StepList.GetNext(Position);

			if (pStep->m_sName == _T(""))  pStep->m_sName = _T("-");
			if (pStep->m_strMsg == _T("")) pStep->m_strMsg = _T("-");
			sAudioTarget = _T("-");
			sAudioMeasure = _T("-");
			sMeasured = _T("-");
			sNominal = _T("-");
			sLowLimit = _T("-");
			sHighLimit = _T("-");
			sElapsedTime = _T("-");
			sResult = _T("-");
			sSPEC_SEQ = "1";

			if (pStep->m_bTest) {
				sRun = _T("1");
				if (pStep->m_bResult) { sResult = _T("OK"); }
				else {
					sResult = _T("NG");
					if (pStep->m_bRunAudioTest && pStep->m_bRunVideoTest) {
						if (pStep->m_bVideoTestResult == FALSE) { sResult = _T("NG_V"); }
						if (pStep->m_bAudioTestResult == FALSE) { sResult = _T("NG_A"); }

						if ((pStep->m_bVideoTestResult == FALSE) && (pStep->m_bAudioTestResult == FALSE)) {
							sResult = _T("NG_AV");
						}
					}
					else if (!pStep->m_bRunAudioTest && pStep->m_bRunVideoTest) {
						if (pStep->m_bVideoTestResult == FALSE) { sResult = _T("NG_V"); }
					}
					else if (pStep->m_bRunAudioTest && !pStep->m_bRunVideoTest) {
						if (pStep->m_bAudioTestResult == FALSE) { sResult = _T("NG_A"); }
					}
				}
				sElapsedTime.Format(_T("%.1f"), pStep->m_fElapsedTime);
			}
			else 
			{ 
				sResult = _T("SKIP"); 
				sRun = _T("0");
			}

			if (pStep->m_bResult || !pStep->m_bTest) continue;

			if (pStep->m_bRunAudioTest)
			{
				if (pStep->m_nMeasureAudioType == LEVEL_CHECK) {
					sAudioTarget.Format(_T("X_X_%d_%d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				else if (pStep->m_nMeasureAudioType == FREQUENCY_CHECK) {
					sAudioTarget.Format(_T("%d_%d_X_X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
				}
				else {
					sAudioTarget.Format(_T("%d_%d_%d_%d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				// Audio Measure
				sAudioMeasure.Format(_T("%d_%d_%d_%d"), pStep->m_nLeftFreq[1], pStep->m_nRightFreq[1], pStep->m_nLeftLevel[1], pStep->m_nRightLevel[1]);
			}

			if (pStep->m_bAdcValueCheckStep)
			{
				sAudioMeasure = pStep->m_szAdcValue;
			}
			if (pStep->m_nFuncType == MEAS_DOUBLE)
			{
				sMeasured.Format(_T("%.1f"), pStep->m_fMeasured);
				sNominal.Format(_T("%.1f"), pStep->m_fNominal);
				sLowLimit.Format(_T("%.1f"), pStep->m_fLowLimit);
				sHighLimit.Format(_T("%.1f"), pStep->m_fHighLimit);

			}
			sStepNo.Format("%d", pStep->m_nStep);
			sMsg.Format(_T("%s"), pStep->m_strMsg);

			//szOutputString.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s \r\n",
			//	strWipid,
			//	CurrentSet->sChassisName,
			//	CurrentSet->sModelName,
			//	sTemp2,
			//	szResultTotal,
			//	sTime,
			//	sStepNo, pStep->m_sName, sResult, sMeasured, sNominal, sLowLimit, sHighLimit, sAudioTarget,
			//	sAudioMeasure, sElapsedTime, sMsg, HDMIGeneratorCtrl.m_FW_Ver);
			szOutputString.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
				sGMES_PortNo, CurrentSet->sPCBID_Read, CurrentSet->sPCBID_Scan, szResultTotal, sAudioMeasure, sAudioTarget, CurrentSet->sChassisName,
			HDMIGeneratorCtrl.m_FW_Ver, CurrentSet->sCPUVersionRead, sKeyDL_Log, sLowLimit, sSystemNo, sMeasured,
			sMsg, sRun, sSeqNo, pStep->m_sName,sStepNo, 
			sNominal,sElapsedTime, sTime, sHighLimit, sDayTime, sSPEC_SEQ, sPCBID_Rewrite);

			//pFile.Write((LPSTR)(LPCTSTR)szOutputString, szOutputString.GetLength() + 1);
		}
	}
	POSITION	PosStep_EM;
	CVFTestInfo *pCurStep_EM;
	PosStep_EM = CurrentSet->VF_TestInfo.GetHeadPosition();
	float gVfList[50];
	int gRunEnable[50];
	memset(gVfList, 0, sizeof(gVfList));
	memset(gRunEnable, 0, sizeof(gRunEnable));
	while (PosStep_EM != NULL)
	{
		pCurStep_EM = CurrentSet->VF_TestInfo.GetNext(PosStep_EM);

		if (pCurStep_EM->bRun)
		{



			UINT nChNo = pCurStep_EM->nChNo;
			if (nChNo <= 42) {

				nChNo = nChNo - 1;
				gVfList[nChNo] = (pCurStep_EM->dMaesure_Min + pCurStep_EM->dMaesure_Max) / 2.0;
				gRunEnable[nChNo] = 1;
			}
			else if ((nChNo >= 101) && (nChNo <= 106))
			{
				nChNo = nChNo - 101 + 42;
				gVfList[nChNo - 101 + 42] = (pCurStep_EM->dMaesure_Min + pCurStep_EM->dMaesure_Max) / 2.0;
			}
			else
			{
				continue;
			}
		}
	}

		CString Stemp;
	for (int i = 0; i < 48; i++)
	{
		
		if (gRunEnable[i] == 1)
		{
			Stemp.Format(",%.1f", gVfList[i]);
		}
		else
		{
			Stemp = ",N";
		}
		szOutputString +=  Stemp;// 	
	}
	szOutputString += "\r\n";// 	
	pFile.Write((LPSTR)(LPCTSTR)szOutputString, szOutputString.GetLength() + 1);

	pFile.Close();
}
#else
void SaveResultSummary(CString strWipid, BOOL bResult, CString sTime)
{
	CFile pFile;
	CFileException FileExcept;

	CString sTemp			= _T("");
	CString szOutputString	= _T("");
	CString szResultTotal	= _T("");
	CString sStepNo			= _T("");
	CString sRun			= _T("");
	CString sResult			= _T("");
	CString sAudioTarget	= _T("");
	CString sAudioMeasure	= _T("");
	CString sMeasured		= _T("");
	CString sNominal		= _T("");
	CString sLowLimit		= _T("");
	CString sHighLimit		= _T("");
	CString sElapsedTime	= _T("");
	CString sMsg	= _T("");
	BOOL	bAlreadyExist		= FALSE;

	CString sResultPath;
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	

	CTime curTime = CTime::GetCurrentTime();
	
	sTemp2 = curTime.Format(_T("[%H%M%S]"));
	sDay = curTime.Format(_T("%Y%m%d")); 
	sTemp.Format(_T("%s\\TestResult\\Summary"), g_pView->m_szExePath);
	
	if(!FileExists(sTemp))   CreateFullPath(sTemp);
	
	sResultPath.Format(_T("%s\\ResultSummary_%s.CSV"),sTemp, sDay); 
	
	int nOutputStringLength = 0;

	if(FileExists(sResultPath))
	{
		bAlreadyExist = TRUE;
	}
	
	if(!pFile.Open(sResultPath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyNone , &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return;
	}
	
	if(!bAlreadyExist)
	{
		szOutputString.Format("WipID,Chassis,Model,Time,Result,TestTime,NO,NAME, StepResult,MEASURE,TARGET,L-LIMIT,U-LIMIT,A_TARGET,A_MEASURE,TIME,MESSAG,HDMIVer\r\n");
		pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);
	}

	pFile.SeekToEnd();

	//=============	
	// Test Result 
	//=============

	if (bResult)
	{
		szResultTotal = _T("OK");
		szOutputString.Format("%s,%s,%s,%s,%s,%s, , , , , , , , , , , ,%s\r\n",
			strWipid,
			CurrentSet->sChassisName, 
			CurrentSet->sModelName,
			sTemp2,
			szResultTotal,
			sTime, 
			HDMIGeneratorCtrl.m_FW_Ver);
		pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);
	}
	else
	{
		szResultTotal = _T("NG");
		POSITION Position = StepList.GetHeadPosition();
		while (Position) 
		{
			CStep* pStep = StepList.GetNext(Position);

			if(pStep->m_sName == _T(""))  pStep->m_sName  = _T("-");
			if(pStep->m_strMsg == _T("")) pStep->m_strMsg = _T("-");
			sAudioTarget	= _T("-");
			sAudioMeasure	= _T("-");
			sMeasured		= _T("-");
			sNominal		= _T("-");
			sLowLimit		= _T("-");
			sHighLimit		= _T("-");
			sElapsedTime	= _T("-");
			sResult			= _T("-");

			if(pStep->m_bTest){
				if (pStep->m_bResult){ sResult = _T("OK"); }
				else{ 
					sResult = _T("NG");
					if(pStep->m_bRunAudioTest && pStep->m_bRunVideoTest){
						if(pStep->m_bVideoTestResult == FALSE){ sResult = _T("NG_V");}
						if(pStep->m_bAudioTestResult == FALSE){	sResult = _T("NG_A"); }

						if((pStep->m_bVideoTestResult == FALSE) && (pStep->m_bAudioTestResult == FALSE)) {
							sResult = _T("NG_AV");
						}
					}
					else if(!pStep->m_bRunAudioTest && pStep->m_bRunVideoTest){
						if(pStep->m_bVideoTestResult == FALSE){	sResult = _T("NG_V"); }
					}
					else if(pStep->m_bRunAudioTest && !pStep->m_bRunVideoTest){
						if(pStep->m_bAudioTestResult == FALSE){	sResult = _T("NG_A"); }
					}
				}
				sElapsedTime.Format(_T("%.1f"), pStep->m_fElapsedTime);
			}
			else{ sResult = _T("SKIP");}

			if(pStep->m_bResult || !pStep->m_bTest) continue;

			if(pStep->m_bRunAudioTest)
			{
				if(pStep->m_nMeasureAudioType == LEVEL_CHECK){
					sAudioTarget.Format(_T("X_X_%d_%d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				else if(pStep->m_nMeasureAudioType == FREQUENCY_CHECK){
					sAudioTarget.Format(_T("%d_%d_X_X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
				}
				else{
					sAudioTarget.Format(_T("%d_%d_%d_%d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				// Audio Measure
				sAudioMeasure.Format(_T("%d_%d_%d_%d"), pStep->m_nLeftFreq[1], pStep->m_nRightFreq[1], pStep->m_nLeftLevel[1], pStep->m_nRightLevel[1]);
			}

			if(pStep->m_bAdcValueCheckStep)
			{
				sAudioMeasure = pStep->m_szAdcValue;
			}
			if(pStep->m_nFuncType == MEAS_DOUBLE)
			{
				sMeasured.Format(_T("%.1f"), pStep->m_fMeasured);
				sNominal.Format(_T("%.1f"), pStep->m_fNominal);
				sLowLimit.Format(_T("%.1f"), pStep->m_fLowLimit);
				sHighLimit.Format(_T("%.1f"), pStep->m_fHighLimit);

			}
			sStepNo.Format("%d", pStep->m_nStep);
			sMsg.Format(_T("%s"), pStep->m_strMsg);

			szOutputString.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s \r\n",
				strWipid,
				CurrentSet->sChassisName, 
				CurrentSet->sModelName,
				sTemp2,
				szResultTotal,
				sTime,
				sStepNo, pStep->m_sName, sResult, sMeasured, sNominal,sLowLimit,sHighLimit,sAudioTarget,
				sAudioMeasure,sElapsedTime,sMsg, HDMIGeneratorCtrl.m_FW_Ver);

			pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);

		}
	}

	pFile.Close();
}
#endif
BOOL OpenModelListFile(CString sModelListPath,int& nNoModel,CStringArray &Divisions)
{
	POSITION PosStep = NULL;
	CString sSection = _T("");
	CString sTmp	 = _T("");
	CStringArray Rows;

	DeleteModelList();

	if(!FileExists(CurrentSet->sModelListPath))
	{
		sTmp.Format("%s\n File doesn't Exist! Do you want to create file?", sModelListPath);
		
		if(AfxMessageBox(sTmp,MB_YESNO) == IDYES)
		{
			CreateModelListFile(CurrentSet->sModelListPath);
		}
		else
		{
			return FALSE;
		}
	}
	CSpreadSheet tempSS(sModelListPath, "ModelList");

	PosStep  = CurrentSet->ModelList.GetHeadPosition();
	nNoModel = tempSS.GetTotalRows();

	Divisions.RemoveAll();

	tempSS.ReadRow(Rows, 1);
	CString strTmp = Rows.GetAt(0);
	int lModelNewList = 0;
	if (strTmp.Find("Model.Suffix") >= 0)
		lModelNewList = 1;

	
	for (int i = 2; i <= nNoModel; i++)
	{
		CModelData* pModel = new CModelData;
		// Read row
		tempSS.ReadRow(Rows, i);

		
			if (Rows.GetSize() < 6) { continue; }

			
			pModel->m_szDivision = Rows.GetAt(0);
			pModel->m_szDivision = pModel->m_szDivision.Left(4);
			pModel->m_szChassisName = Rows.GetAt(1);
			pModel->m_szModelName = Rows.GetAt(2);
			pModel->m_szSeqFilePath = Rows.GetAt(3);
			pModel->m_szModelInfoFilePath = Rows.GetAt(4);
			pModel->m_szRefImgFolder = Rows.GetAt(5);
			if (Rows.GetSize() > 6) {
				pModel->m_szModelSuffix = Rows.GetAt(6);
			}
			//if (Rows.GetSize() > 7) {
			//	pModel->m_szSeqFilePath2 = Rows.GetAt(7);
			//}

		

		



		CurrentSet->ModelList.AddTail(pModel);

		int lFlag = 0;
		for (int id = 0; id < Divisions.GetCount(); id++)
		{
			if (Divisions.GetAt(id).Compare(pModel->m_szDivision) == 0)
			{
				lFlag = 1;
				break;
				
			}
			
		}

		
		if (lFlag == 0)
		{
			Divisions.Add(pModel->m_szDivision);
		}

	}

	return TRUE;
}



extern CTypedPtrList<CPtrList, CModelData*>	ModelList;
//BOOL CSelectGenerateModel_ModelListOpen(CString sModelListPath, int& nNoModel, CStringArray &Divisions)
//{
//
//#if 1
//
//
//
//	CStringArray Rows;
//	int i;
//	CString lFileName;
//	lFileName = sModelListPath;// +"ModelList.xls";
//	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
//	ModelList.RemoveAll();
//
//	for (i = 2; i <= SS.GetTotalRows(); i++)
//	{
//		// Read row
//		SS.ReadRow(Rows, i);
//
//		if (Rows.GetAt(0).GetLength() > 2)
//		{
//			CModelData* pModel = new CModelData;
//
//			pModel->m_szDivision = Rows.GetAt(0);
//			pModel->m_szDivision = pModel->m_szDivision.Left(4);
//			pModel->m_szChassisName = Rows.GetAt(1);
//			pModel->m_szModelName = Rows.GetAt(2);
//			pModel->m_szSeqFilePath = Rows.GetAt(3);
//			pModel->m_szModelInfoFilePath = Rows.GetAt(4);
//			pModel->m_szRefImgFolder = Rows.GetAt(5);
//
//			ModelList.AddTail(pModel);
//		}
//		else
//		{
//			continue;
//		}
//
//	}
//	
//#else
//
//
//	CStringArray Rows;
//	int i;
//	CString lFileName;
//	lFileName = sModelInfoFolder + "ModelList.xls";
//	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
//	ModelList.RemoveAll();
//
//	for (i = 2; i <= SS.GetTotalRows(); i++)
//	{
//		// Read row
//		SS.ReadRow(Rows, i);
//
//		if (Rows.GetAt(0).GetLength() > 2)
//		{
//			CModelData* pModel = new CModelData;
//
//			pModel->m_szDivision = Rows.GetAt(0);
//			pModel->m_szChassisName = Rows.GetAt(1);
//			pModel->m_szModelName = Rows.GetAt(2);
//			pModel->m_szSeqFilePath = Rows.GetAt(3);
//			pModel->m_szModelInfoFilePath = Rows.GetAt(4);
//			pModel->m_szRefImgFolder = Rows.GetAt(5);
//
//			ModelList.AddTail(pModel);
//		}
//		else
//		{
//			continue;
//		}
//
//	}
//#endif
//	return TRUE;
//}

//BOOL CSelectGenerateModel_SaveModelListFile(CString sModelListPath)
//{
//
//	CStringArray FieldNames, Rows;
//	POSITION	Pos = NULL;
//	CModelData* pModelData = NULL;
//	CString lFileName;
//	lFileName = sModelListPath;// +"ModelList.xls";
//	CSpreadSheet SS(lFileName, "ModelList");//sModelInfoFolder
//
//	SS.GetFieldNames(FieldNames);
//	SS.DeleteSheet();
//
//	SS.BeginTransaction();
//	SS.RollBack();
//
//	SS.AddHeaders(FieldNames);
//
//	Pos = ModelList.GetHeadPosition();
//	while (Pos)
//	{
//		Rows.RemoveAll();
//		pModelData = ModelList.GetNext(Pos);
//
//		Rows.Add(pModelData->m_szChassisName);
//		Rows.Add(pModelData->m_szModelName);
//		Rows.Add(pModelData->m_szSeqFilePath);
//		Rows.Add(pModelData->m_szModelInfoFilePath);
//		Rows.Add(pModelData->m_szRefImgFolder);
//
//		//Rows.Add(pModel->m_szDivision);
//		//Rows.Add(pModel->m_szChassisName);
//		//Rows.Add(pModel->m_szModelName);
//		//Rows.Add(pModel->m_szSeqFilePath);
//		//Rows.Add(pModel->m_szModelInfoFilePath);
//		//Rows.Add(pModel->m_szRefImgFolder);
//
//		SS.AddRow(Rows); // append test row to spreadsheet
//	}
//
//	SS.Commit();
//
//	return TRUE;
//}

//+add kwmoon 081024
BOOL SaveModelListFile()
{
	POSITION	PosStep			 = NULL;
	CString		sSection		 = _T("");
	CString		sTmp			 = _T("");
	int			nNoModelInFile   = 0;
	int			nNoModelInMemory = 0;
	CModelData* pModel			 = NULL;

	if(!FileExists(CurrentSet->sModelListPath))
	{
		//+change kwmoon 081028
	//	CreateFullPath(CurrentSet->sModelListPath);
		CreateModelListFile(CurrentSet->sModelListPath);
	}

	m_Ini.SetIniFile(CurrentSet->sModelListPath);

	sSection	   = "GENERAL_INFO";
	nNoModelInFile = m_Ini.GetProfileInt(sSection,"NO_Model");

	//----------------------
	// Delete Previous Data
	//----------------------
	int i;
	for(i=0; i<nNoModelInFile;i++)
	{
		sSection.Format("MODEL_%d", i+1);
		WritePrivateProfileString(sSection,NULL,NULL,CurrentSet->sModelListPath);
	}

	PosStep  = CurrentSet->ModelList.GetHeadPosition();

	while(PosStep)
	{
		pModel = CurrentSet->ModelList.GetNext(PosStep); nNoModelInMemory++;
	}

	sSection = "GENERAL_INFO";
	m_Ini.SetProfileInt(sSection,"No_Model",nNoModelInMemory);
	
	PosStep  = CurrentSet->ModelList.GetHeadPosition();

	//-------------------
	// Save Current Data
	//-------------------
	for(i=0; i<nNoModelInMemory; i++)
	{
		pModel = CurrentSet->ModelList.GetNext(PosStep);

		sSection.Format("MODEL_%d", i+1);
		
		m_Ini.SetProfileString(sSection, "Chassis_Name",pModel->m_szChassisName);
		m_Ini.SetProfileString(sSection, "Model_Name",pModel->m_szModelName);
		m_Ini.SetProfileString(sSection, "Sequence",pModel->m_szSeqFilePath);
		m_Ini.SetProfileString(sSection, "ModelInfo",pModel->m_szModelInfoFilePath);
		m_Ini.SetProfileString(sSection, "RefImgFolder",pModel->m_szRefImgFolder);
	}

	return TRUE;
}


//+add kwmoon 081028
BOOL CreateModelListFile(CString sModelListPath)
{
	CFile pFile;
	CFileException FileExcept;
	CString sTmp = _T("");

	//+change kwmoon 080910
	if(!pFile.Open(sModelListPath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &FileExcept)) 
	{
		char szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	POSITION	PosStep			 = NULL;
	CString		sSection		 = _T("");
	int			nNoModelInFile   = 0;
	int			nNoModelInMemory = 0;
	CModelData* pModel			 = NULL;

	m_Ini.SetIniFile(CurrentSet->sModelListPath);

	sSection	   = "GENERAL_INFO";
	nNoModelInFile = m_Ini.GetProfileInt(sSection,"NO_Model");

	//----------------------
	// Delete Previous Data
	//----------------------
	for(int i=0; i<nNoModelInFile;i++)
	{
		sSection.Format("MODEL_%d", i+1);
		WritePrivateProfileString(sSection,NULL,NULL,CurrentSet->sModelListPath);
	}

	PosStep  = CurrentSet->ModelList.GetHeadPosition();

	while(PosStep)
	{
		pModel = CurrentSet->ModelList.GetNext(PosStep); nNoModelInMemory++;
	}

	sSection = "GENERAL_INFO";
	m_Ini.SetProfileInt(sSection,"No_Model",nNoModelInMemory);
	
	PosStep  = CurrentSet->ModelList.GetHeadPosition();

	//-------------------
	// Save Current Data
	//-------------------
	for(int i=0; i<nNoModelInMemory; i++)
	{
		pModel = CurrentSet->ModelList.GetNext(PosStep);

		sSection.Format("MODEL_%d", i+1);
		
		m_Ini.SetProfileString(sSection, "Chassis_Name",pModel->m_szChassisName);
		m_Ini.SetProfileString(sSection, "Model_Name",pModel->m_szModelName);
		m_Ini.SetProfileString(sSection, "Sequence",pModel->m_szSeqFilePath);
		m_Ini.SetProfileString(sSection, "ModelInfo",pModel->m_szModelInfoFilePath);
		m_Ini.SetProfileString(sSection, "RefImgFolder",pModel->m_szRefImgFolder);
	}

	return TRUE;
}
BOOL StepInit_AvSwitchBox_UHD()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(!CurrentSet->bUseAVSwitchBox) return TRUE;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined_UHD[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined_UHD[i].pszName) == NULL)
		{
			nSourceIndex = i; break;
		}
	}

	g_GrabSource = LVDS_GRABBER; // LVDS_GRABBER : 0

	switch(nSourceIndex)
	{
		case 1:  //AV
		case 2:  //AV1
		case 35:  //SCART1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 3:  //AV2
		case 36:  //SCART2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;

	//	case 4:  //AV3
	//		bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
	//		break;

		case 7:  //COMP
		case 8:  //COMP1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 9:  //COMP2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;

	//	case 14:  //COMP3
	//		bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
	//		break;

		case 13:  //RGB
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_RGB_SOUND, MAX_AVSWITCH_WAIT_DELAY,  0, 0);
			break;

		case 11:  //TV-OUT
		case 12:  //AV-OUT
		case 23:  //VOUT
		case 24:  //VOUT1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			g_CurSetting.m_nAudioSource  = 2;

			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);

			g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1
			g_VideoSourceType = CurrentSet->nAnalogType;

			if(g_nGrabberType == FHD_GRABBER)
			{
				g_pView->SetGrabInfo(&g_GrabImage);
			}
			break;
		case 10:  //MNT-OUT
		case 25:  //VOUT2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
			g_CurSetting.m_nAudioSource  = 3;

			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);

			g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1

			g_VideoSourceType = CurrentSet->nAnalogType;

			if(g_nGrabberType == FHD_GRABBER)
			{
				g_pView->SetGrabInfo(&g_GrabImage);
			}
			break;

		case 20:  //VAR
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  4, 0);
			g_CurSetting.m_nAudioSource  = 4;
			break;

		case 21:  //E/P
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  5, 0);
			g_CurSetting.m_nAudioSource  = 5;
			break;

		case 22:  //OPT
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  9, 0);
			g_CurSetting.m_nAudioSource  = 9;
			break;

		case 33:  //SPDIF
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  8, 0);
			g_CurSetting.m_nAudioSource  = 8;
			break;
        // add PSH 100310
		case 38:  //COMP1-AV1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 39:  //COMP1-AV2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 40:  //COMP2-AV1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;

		case 41:  //COMP2-AV2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;

		case 43:  //AVC-RGB
			g_GrabSource = HDMI_GRABBER; 
			CurrentSet->nHdmiWidth = 1280; 
			CurrentSet->nHdmiHeight = 1024;
			CurrentSet->nHDMI_Grab_Resolution = 2;
		//	CurrentSet->nHDMI_Grab_BitShift = 0;
			bResult = gAvConvertorCtrl.SetInputSignal(0);
			break;

		case 44:  //AVC-DVI
			g_GrabSource = HDMI_GRABBER; 
			CurrentSet->nHdmiWidth = 1920; 
			CurrentSet->nHdmiHeight = 1080; 
			CurrentSet->nHDMI_Grab_Resolution = 3;
		///	CurrentSet->nHDMI_Grab_BitShift = 0;
			bResult = gAvConvertorCtrl.SetInputSignal(1);
			break;

		case 45:  //AVC-DP
			g_GrabSource = HDMI_GRABBER; 
			CurrentSet->nHdmiWidth = 1920; 
			CurrentSet->nHdmiHeight = 1080; 
			CurrentSet->nHDMI_Grab_Resolution = 3;
		//	CurrentSet->nHDMI_Grab_BitShift = 0;
			bResult = gAvConvertorCtrl.SetInputSignal(2);
			break;

		case 46:  //AVC-HDMI
			g_GrabSource = HDMI_GRABBER; 
			/*
			CurrentSet->nHdmiWidth = 1920; 
			CurrentSet->nHdmiHeight = 1080; 
			CurrentSet->nHDMI_Grab_Resolution = 3;
			*/
		//	CurrentSet->nHDMI_Grab_BitShift = 0;
			bResult = gAvConvertorCtrl.SetInputSignal(3);
			break;
			//-
		case 52:  //HDMI GRAB //190225 GODTECH
			g_GrabSource = HDMI_GRABBER;
			CurrentSet->nHdmiWidth = 1920;
			CurrentSet->nHdmiHeight = 1080;
			CurrentSet->nHDMI_Grab_Resolution = 3;
			//	CurrentSet->nHDMI_Grab_BitShift = 0;
			bResult = TRUE;//bResult = gAvConvertorCtrl.SetInputSignal(3);
			break;

		default:
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			g_CurSetting.m_nAudioSource  = 1;
			g_GrabSource = LVDS_GRABBER; // LVDS_GRABBER : 0
			break;
	}

//	g_pView->StopLVDSGrabThread();
//	Sleep(500);
//	g_pView->StartLVDSGrabThread();

	if(!bResult)
	{
		if((nSourceIndex >=43) && (nSourceIndex <=46)){
			szMsg1 = _T("StepInit_AvConvertor - FAIL");
		}
		else{
			szMsg1 = _T("StepInit_AvSwitchBox- FAIL");
		}
		AddStringToStatus(szMsg1);
		//091019
		if((pCurStep->m_nTestType == MODULE_TEST) || (pCurStep->m_nTestType == TV_COMM_TEST)) 
		{
			return TRUE;
		}

	//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		if(CurrentSet->bIsRunning)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
				pCurStep->m_strMsg = szMsg1;
			}		
		}
	}

	return bResult;
}
BOOL StepInit_AvSwitchBox_FHD()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(!CurrentSet->bUseAVSwitchBox) return TRUE;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined_FHD[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined_FHD[i].pszName) == NULL)
		{
			nSourceIndex = i; break;
		}
	}

	g_GrabSource = LVDS_GRABBER; // LVDS_GRABBER : 0

	switch(nSourceIndex)
	{
		case 2:  //AV1
		case 42:  //SCART1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 3:  //AV2
		case 43:  //SCART2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;

		case 4:  //AV3
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
			break;

		case 11:  //COMP
		case 12:  //COMP1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 13:  //COMP2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;
		// add psh 100310
		case 14:  //COMP3
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
			break;

		case 18:  //RGB
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_RGB_SOUND, MAX_AVSWITCH_WAIT_DELAY,  0, 0);
			break;

		case 16:  //TV-OUT
		case 17:  //AV-OUT
		case 28:  //VOUT
		case 29:  //VOUT1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			g_CurSetting.m_nAudioSource  = 2;

			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);

			g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1

			//+change 090420(Modification No1)
		/*	if((CurrentSet->nAnalogWidth == 640) && (CurrentSet->nAnalogHeight == 480)) 
			{
				g_VideoSourceType = V_SRC_NTSC;
			}
			else if((CurrentSet->nAnalogWidth == 768) && (CurrentSet->nAnalogHeight == 576))
			{
				g_VideoSourceType = V_SRC_PAL;
			}
		*/
			g_VideoSourceType = CurrentSet->nAnalogType;

			if(g_nGrabberType == FHD_GRABBER)
			{
				g_pView->SetGrabInfo(&g_GrabImage);
			}
			break;
		case 15:  //MNT-OUT
		case 30:  //VOUT2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
			g_CurSetting.m_nAudioSource  = 3;

			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);

			g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1

			//+change 090420(Modification No1)
		/*	if((CurrentSet->nAnalogWidth == 640) && (CurrentSet->nAnalogHeight == 480)) 
			{
				g_VideoSourceType = V_SRC_NTSC;
			}
			else if((CurrentSet->nAnalogWidth == 768) && (CurrentSet->nAnalogHeight == 576))
			{
				g_VideoSourceType = V_SRC_PAL;
			}
		*/
			g_VideoSourceType = CurrentSet->nAnalogType;

			
			if(g_nGrabberType == FHD_GRABBER)
			{
				g_pView->SetGrabInfo(&g_GrabImage);
			}
			break;

		case 25:  //VAR
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  4, 0);
			g_CurSetting.m_nAudioSource  = 4;
			break;

		case 26:  //E/P
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  5, 0);
			g_CurSetting.m_nAudioSource  = 5;
			break;

		case 27:  //OPT
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  9, 0);
			g_CurSetting.m_nAudioSource  = 9;
			break;

		case 40:  //SPDIF
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  8, 0);
			g_CurSetting.m_nAudioSource  = 8;
			break;
        // add PSH 100310
		case 45:  //COMP1-AV1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 46:  //COMP1-AV2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			break;

		case 47:  //COMP2-AV1
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;

		case 48:  //COMP2-AV2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);
			break;
        //-
		default:
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
			g_CurSetting.m_nAudioSource  = 1;
			g_GrabSource = LVDS_GRABBER; // LVDS_GRABBER : 0
			break;
	}

	if(!bResult)
	{
		szMsg1 = _T("StepInit_AvSwitchBox- FAIL");
		AddStringToStatus(szMsg1);
		//091019
		if((pCurStep->m_nTestType == MODULE_TEST) || (pCurStep->m_nTestType == TV_COMM_TEST)) 
		{
			return TRUE;
		}

	//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		if(CurrentSet->bIsRunning)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
				pCurStep->m_strMsg = szMsg1;
			}		
		}
	}

	return bResult;
}


BOOL StepInit_SourceAutoControl_UHD()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	BYTE nSourceCode = 0xff;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(!CurrentSet->bUseTVCommPort) return TRUE;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined_UHD[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined_UHD[i].pszName) == NULL)
		{
			nSourceIndex = i; break;
		}
	}
	switch(nSourceIndex)
	{
		case 1:  //AV
		case 2:  //AV1
		case 37:  //SCART_RGB
		case 38:  //COMP1-AV1
		case 40:  //COMP2-AV1
			nSourceCode = 0x20;
			break;

		case 3:  //AV2
		case 39:  //COMP1-AV2
		case 41:  //COMP2-AV2
			nSourceCode = 0x21;
			break;

	//	case 4:  //AV3
	//		nSourceCode = 0x22;
	//		break;

		case 4:  //YC
		case 5:  //YC1
		case 6:  //YC2
			nSourceCode = 0x30;
			break;

		case 7:  //COMP
		case 8:  //COMP1
			nSourceCode = 0x40;
			break;

		case 9:  //COMP2
			nSourceCode = 0x41;
			break;

		case 13:  //RGB
			nSourceCode = 0x60;
			break;

//		case 44:  //HDMI-DIMming
		case 14:  //HDMI
		case 15:  //HDMI1
			nSourceCode = 0x90;
			break;

		case 16:  //HDMI2
			nSourceCode = 0x91;
			break;

		case 17:  //HDMI3
			nSourceCode = 0x92;
			break;

		case 18:  //HDMI4
			nSourceCode = 0x93;
			break;

		case 26:  //DTV
			nSourceCode = 0x00;
			break;

		case 27:  //ATV
			nSourceCode = 0x10;
			break;

		case 28:  //RGB-DTV
			nSourceCode = 0x50;
			break;

		case 29:  //DVI-DTV
			nSourceCode = 0x70;
			break;

		case 30:  //DVI-PC
			nSourceCode = 0x80;
			break;

		case 31:  //HDMI-PC
			nSourceCode = 0xa0;
			break;

		case 32:  //DVD
			nSourceCode = 0xb0;
			break;

		case 50:  //DP
			if (CurrentSet->nTVControlType) // I2C
			{
				nSourceCode = 0xd0;
			}
			else
			{
				nSourceCode = 0xc0;
			}
			
			break;

		case 51:  //OPS
			nSourceCode = 0x98;
			break;

		default:
			break;
	}

	if(nSourceCode == 0xff) return TRUE;

	if(g_CurSetting.m_nInputSourceCode == nSourceCode)
	{
		if(pCurStep->m_bStepRetryFlag || pCurStep->m_bStepInternalRetryFlag)
		{
			if((pCurStep->m_bAudioTestResult == FALSE) && (pCurStep->m_bVideoTestResult == TRUE))
			{
				szMsg1.Format("%d.Set Input %s is skipped",pCurStep->m_nStep,szInputSource);
				AddStringToRemoteComLog(szMsg1);

				return TRUE;
			}
			else{
				if(CurrentSet->nTVControlType){
					bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
					//add 100101
					if(!bResult && (nSourceIndex == 14))
					{
						bResult = I2cAdcCtrl.SetIRcode(0xD9);
						//add 110114
						if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) _Wait(500);

					}
				}
				else{
					bResult = TVCommCtrl.InputSourceChange(nSourceCode); 
					//add 100101
					if(!bResult && (nSourceIndex == 14))
					{
						bResult = TVCommCtrl.SendCommString("mc 00 d9");
					}
				}
			}

		}
		else{
			// add 090909
			if(pCurStep->m_nTestType == ADC_TEST)
			{
				if(CurrentSet->nTVControlType){
					bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
					//add 110114
					if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) _Wait(500);
				}
				else{
					bResult = TVCommCtrl.InputSourceChange(nSourceCode); 
				}
			}
			return TRUE;
		}
	}
	else
	{
		if(CurrentSet->nTVControlType){
			bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
			if(!bResult)
			{
				I2cAdcCtrl.SetDDC2AB();
				I2cAdcCtrl.WakeUp();
				bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
				//add 100101
				if(!bResult && (nSourceIndex == 14))
				{
					bResult = I2cAdcCtrl.SetIRcode(0xD9);
				}
			}
			//add 110114
			if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) _Wait(500);
		}
		else{
			bResult = TVCommCtrl.InputSourceChange(nSourceCode); 
			//add 100101
			if(!bResult && (nSourceIndex == 14))
			{
				bResult = TVCommCtrl.SendCommString("mc 00 d9");
			}
		}
	}

	if(bResult)
	{
		g_CurSetting.m_nInputSourceCode = nSourceCode;
	}
	else
	{
		szMsg1 = _T("StepInit_SourceAutoControl- FAIL");
		AddStringToStatus(szMsg1);

		//091019
		if((pCurStep->m_nTestType == MODULE_TEST) || (pCurStep->m_nTestType == TV_COMM_TEST)) 
		{
			return TRUE;
		}
	//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		if(CurrentSet->bIsRunning)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
				pCurStep->m_strMsg = szMsg1;
			}		
		}
	}

	return bResult;
}

BOOL StepInit_SourceAutoControl_FHD()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	BYTE nSourceCode = 0xff;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(!CurrentSet->bUseTVCommPort) return TRUE;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined_FHD[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined_FHD[i].pszName) == NULL)
		{
			nSourceIndex = i; break;
		}
	}
	switch(nSourceIndex)
	{
		case 2:  //AV1
		case 44:  //SCART_RGB
			// add PSH 100316
		case 45:  //COMP1-AV1
		case 47:  //COMP2-AV1
			nSourceCode = 0x20;
			break;

		case 3:  //AV2
			// add PSH 100316
		case 46:  //COMP1-AV2
		case 48:  //COMP2-AV2
			nSourceCode = 0x21;
			break;

		case 4:  //AV3
			nSourceCode = 0x22;
			break;

		case 6:  //YC
		case 7:  //YC1
		case 8:  //YC2
		case 9:  //YC3
			nSourceCode = 0x30;
			break;

		case 11:  //COMP
		case 12:  //COMP1
			nSourceCode = 0x40;
			break;

		case 13:  //COMP2
			nSourceCode = 0x41;
			break;
// add 100101
		case 14:  //COMP3
			nSourceCode = 0x42;
			break;
//
		case 18:  //RGB
			nSourceCode = 0x60;
			break;

		case 19:  //HDMI
		case 20:  //HDMI1
			nSourceCode = 0x90;
			break;

		case 21:  //HDMI2
			nSourceCode = 0x91;
			break;

		case 22:  //HDMI3
			nSourceCode = 0x92;
			break;

		case 23:  //HDMI4
			nSourceCode = 0x93;
			break;

		case 33:  //DTV
			nSourceCode = 0x00;
			break;

		case 34:  //ATV
			nSourceCode = 0x10;
			break;

		case 35:  //RGB-DTV
			nSourceCode = 0x50;
			break;

		case 36:  //DVI-DTV
			nSourceCode = 0x70;
			break;

		case 37:  //DVI-PC
			nSourceCode = 0x80;
			break;

		case 38:  //HDMI-PC
			nSourceCode = 0xa0;
			break;

		case 39:  //DVD
			nSourceCode = 0xb0;
			break;

		default:
			break;
	}

	if(nSourceCode == 0xff) return TRUE;

	if(g_CurSetting.m_nInputSourceCode == nSourceCode)
	{
		if(pCurStep->m_bStepRetryFlag || pCurStep->m_bStepInternalRetryFlag)
		{
			if((pCurStep->m_bAudioTestResult == FALSE) && (pCurStep->m_bVideoTestResult == TRUE))
			{
				szMsg1.Format("%d.Set Input %s is skipped",pCurStep->m_nStep,szInputSource);
				AddStringToRemoteComLog(szMsg1);

				return TRUE;
			}
			else{
				if(CurrentSet->nTVControlType){
					bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
					//add 100101
					if(!bResult && (nSourceIndex == 14))
					{
						bResult = I2cAdcCtrl.SetIRcode(0xD9);
						//add 110114
						if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) _Wait(500);

					}
				}
				else{
					bResult = TVCommCtrl.InputSourceChange(nSourceCode); 
					//add 100101
					if(!bResult && (nSourceIndex == 14))
					{
						bResult = TVCommCtrl.SendCommString("mc 00 d9");
					}
				}
			}

		}
		else{
			// add 090909
			if(pCurStep->m_nTestType == ADC_TEST)
			{
				if(CurrentSet->nTVControlType){
					bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
					//add 110114
					if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) _Wait(500);
				}
				else{
					bResult = TVCommCtrl.InputSourceChange(nSourceCode); 
				}
			}
			return TRUE;
		}
	}
	else{
		if(CurrentSet->nTVControlType){
			bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
			if(!bResult)
			{
				I2cAdcCtrl.SetDDC2AB();
				I2cAdcCtrl.WakeUp();
				bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
				//add 100101
				if(!bResult && (nSourceIndex == 14))
				{
					bResult = I2cAdcCtrl.SetIRcode(0xD9);
				}
			}
			//add 110114
			if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) _Wait(500);
		}
		else{
			bResult = TVCommCtrl.InputSourceChange(nSourceCode); 
			//add 100101
			if(!bResult && (nSourceIndex == 14))
			{
				bResult = TVCommCtrl.SendCommString("mc 00 d9");
			}
		}
	}

	if(bResult){
		g_CurSetting.m_nInputSourceCode = nSourceCode;
	}
	else{
		szMsg1 = _T("StepInit_SourceAutoControl- FAIL");
		AddStringToStatus(szMsg1);

		//091019
		if((pCurStep->m_nTestType == MODULE_TEST) || (pCurStep->m_nTestType == TV_COMM_TEST)) 
		{
			return TRUE;
		}
	//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		if(CurrentSet->bIsRunning)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
				pCurStep->m_strMsg = szMsg1;
			}		
		}
	}

	return bResult;
}


//+add psh 100310
int GetHDMI_Model(int nModelOption)
{
	int nModel = 0;

	if(CurrentSet->nHDMIResolution == 0)  //1080p
	{
		switch(CurrentSet->nHDMIBitSpec)
		{
			case 0:  //8Bit
				if(CurrentSet->nHDMIGenType == 0){
					nModel = 408;
				}
				else{
					nModel = 43; 
				}
				break;

			case 1:  //10Bit
				if(CurrentSet->nHDMIGenType == 0){
					nModel = 407;
				}
				else{
					nModel = 42; 
				}
				break;

			case 2:  //12Bit
				if(CurrentSet->nHDMIGenType == 0){
					nModel = 406;
				}
				else{
					nModel = 41; 
				}
				break;

			default:
				if(CurrentSet->nHDMIGenType == 0){
					nModel = 408;
				}
				else{
					nModel = 43;  
				}
				break;
		}
	}
	else{

		switch(CurrentSet->nHDMIResolution)
		{
			case 1:  //1080i
				if(CurrentSet->nHDMIGenType == 0){
					nModel = 409;
				}
				else{
					nModel = 44;  
				}
				break;

			case 2:  //720p
				if(CurrentSet->nHDMIGenType == 0){
					nModel = 410;
				}
				else{
					nModel = 45;  
				}
				break;

			case 3:  //4K
				if(CurrentSet->nHDMIGenType == 0){
					if(nModelOption == 0){nModel = 475;}
					else if(nModelOption == 1){nModel = 463;}
					else if(nModelOption == 2){nModel = 492;}//499
					else {nModel = 452;} //459
				}
				else{
					nModel = 43; 
				}
				break;


			default:
				if(CurrentSet->nHDMIGenType == 0){
					nModel = 408;
				}
				else{
					nModel = 43;  
				}
				break;
		}
	}

	return nModel;
}

int GetCOMP_Model()
{
	int nModel = 209;

	switch(CurrentSet->nCompResolution)
	{
		case 0:  //1080P
			if(CurrentSet->nAnalogType == 0)
				nModel = 225;
			else
				nModel = 223;
			break;

		case 1:  //720p
			nModel = 217;
			break;

		case 2:  //480P
			nModel = 209;
			break;

		default:
			if(CurrentSet->nAnalogType == 0)
				nModel = 225;
			else
				nModel = 223;
			break;
	}

	return nModel;
}

//+add PSH 0903038
BOOL StepInit_PatternAutoControl_UHD()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	int nModel = 0;
	int nPattern = 0;
	int nAnalogFormat = -1;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

		
	if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) return TRUE;

	nAnalogFormat = CurrentSet->nAnalogType;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined_UHD[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined_UHD[i].pszName) == NULL)
		{
			nSourceIndex = i; break;
		}
	}
	//100101 Change
	switch(nSourceIndex)
	{
		case 1:  //AV
		case 2:  //AV1
			// add PSH 100316
		case 38:  //COMP1-AV1
		case 40:  //COMP2-AV1
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 33;
			break;

		case 3:  //AV2
			// add PSH 100316
		case 39:  //COMP1-AV2
		case 41:  //COMP2-AV2
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 65;
			break;

		//case 4:  //AV3
		//	if(nAnalogFormat == 0)
		//		nModel = 201;
		//	else
		//		nModel = 202;

		//	nPattern = 53;
		//	break;

		case 4:  //YC
		case 5:  //YC1
		case 6:  //YC2
	//	case 9:  //YC3
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 33;
			break;

		case 7:  //COMP
		case 8:  //COMP1
			nModel = GetCOMP_Model();
			nPattern = 65;
			break;

		case 9:  //COMP2
			nModel = GetCOMP_Model();
			nPattern = 53;
			break;

		//case 14:  //COMP3
		//	if(nAnalogFormat == 0)
		//		nModel = 225;
		//	else
		//		nModel = 223;

		//	nPattern = 67;
		//	break;
//-
		case 13:  //RGB
			nModel = 60;
			nPattern = 8;
			break;

		case 14:  //HDMI
		case 15:  //HDMI1
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI1_Port);
			}
			nModel = GetHDMI_Model(CurrentSet->nHDMI1_Model);
			if(CurrentSet->nHDMIGenType == 0){
				nPattern = 33;
			}
			else{
				nPattern = 16;
			}
			break;

		case 16:  //HDMI2
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI2_Port);
			}
			nModel = GetHDMI_Model(CurrentSet->nHDMI2_Model);
			if(CurrentSet->nHDMIGenType == 0){
				nPattern = 52;
			}
			else{
				nPattern = 20;
			}
			break;

		case 17:  //HDMI3
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI3_Port);
			}
			nModel = GetHDMI_Model(CurrentSet->nHDMI3_Model);
			if(CurrentSet->nHDMIGenType == 0){
				nPattern = 61;
			}
			else{
				nPattern = 23;
			}
			break;

		case 18:  //HDMI4
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI4_Port);
			}
			nModel = GetHDMI_Model(CurrentSet->nHDMI4_Model);
			if(CurrentSet->nHDMIGenType == 0){
				nPattern = 8;
			}
			else{
				nPattern = 6;
			}
			break;

		case 51:  //OPS(HDMI5)
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(5);
			}
			if(CurrentSet->nHDMIGenType == 0){
				nModel = 408;
				nPattern = 65;
			}
			else{
				nModel = 43; 
				nPattern = 24;
			}
			break;

		case 29:  //DVI-DTV
		case 30:  //DVI-PC
			nModel = 24;
			nPattern = 65;
			break;


		case 42:  //MHL
			if(CurrentSet->nHDMIGenType == 0){
				nModel = 395; //420;
				nPattern = 67;
			}
			else{
				nModel = 45;
				nPattern = 26;
			}
			break;

		case 44:  //HDMI-DIMming
			nModel = GetHDMI_Model(CurrentSet->nHDMI1_Model);
			if(CurrentSet->nHDMIGenType == 0){
				nPattern = 4;
			}
			else{
				nPattern = 2;
			}
			break;

		case 34:  //OSD_TEST
			nModel = HDMIGeneratorCtrl.m_nModelNo;
			if(CurrentSet->nHDMIGenType == 0){
				nPattern = 4;
			}
			else{
				nPattern = 2;
			}
			break;
		case 35:  //SCART1
			nModel = 202;
			nPattern = 33;
			break;

		case 36:  //SCART2
			nModel = 202;
			nPattern = 65;
			break;
		case 37:  //SCART_RGB
			nModel = 232;
			nPattern = 65;
			break;

		case 43:  //AVC-RGB
			nModel = 232;
			nPattern = 65;
			break;

		case 48:  //COMP-DIM
			nModel = GetCOMP_Model();
			nPattern = 4;
			break;

		case 49:  //AV-DIM
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 4;
			break;


		default:
			break;
	}

	if(pCurStep->m_nTestType == OSD_TEST) nPattern = 2;

	if(nModel == 0) return TRUE;

	if((nSourceIndex >= 14) && (nSourceIndex <= 18))  
	{
	//	if(HDMIGeneratorCtrl.m_bMhlMode){
	//		g_pView->Set_Mhl(1, 1);
	//	}
	//	Sleep(200);
		bResult = HDMIGeneratorCtrl.SetOut(nModel, nPattern);
		//add 090715
		if(!bResult){
			InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
			bResult = HDMIGeneratorCtrl.SetOut(nModel, nPattern); 
		}
	}
	else if((nSourceIndex == 42) || (nSourceIndex == 51))
	{
		bResult = HDMIGeneratorCtrl.SetOut(nModel, nPattern);
	//	Sleep(200);
	//	g_pView->Set_Mhl(1, 0);
	}
	else if((nSourceIndex == 44) || (nSourceIndex == 34)) 
	{
	//	if(HDMIGeneratorCtrl.m_bMhlMode){
	//		g_pView->Set_Mhl(1, 1);
	//	}
	//	Sleep(200);
		bResult = HDMIGeneratorCtrl.SetPattern(nPattern);
	}
	else{
		bResult = PatternGeneratorCtrl.SetOut(nModel, nPattern); 
	}

	if(bResult){
		g_CurSetting.m_nInputSource = nSourceIndex;
	}
	else{
		szMsg1 = _T("StepInit_PatternAutoControl- FAIL");
		AddStringToStatus(szMsg1);

		//091019
		if((pCurStep->m_nTestType == MODULE_TEST) || (pCurStep->m_nTestType == TV_COMM_TEST)) 
		{
			return TRUE;
		}

	//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		if(CurrentSet->bIsRunning)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
				pCurStep->m_strMsg = szMsg1;
			}		
		}
	}


	return bResult;
}

int GetHDMI_Model_FHD()
{
	int nModel = 0;

	if(CurrentSet->nHDMIResolution == 0)  //1080p
	{
		switch(CurrentSet->nHDMIBitSpec)
		{
			case 0:  //8Bit
				nModel = 43;
				break;

			case 1:  //10Bit
				nModel = 42;
				break;

			case 2:  //12Bit
				nModel = 41;
				break;

			default:
				nModel = 43;
				break;
		}
	}
	else{

		switch(CurrentSet->nHDMIResolution)
		{
			case 1:  //1080i
				nModel = 44;
				break;

			case 2:  //720p
				nModel = 45;
				break;

			case 3:  //480p
				nModel = 46;
				break;

			default:
				nModel = 43;
				break;
		}
	}

	return nModel;
}

//+add PSH 0903038
BOOL StepInit_PatternAutoControl_FHD()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	int nModel = 0;
	int nPattern = 0;
	int nAnalogFormat = -1;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

		
	if(pCurStep->m_nTestType == ADC_TEST || pCurStep->m_nTestType == TV_COMM_TEST) return TRUE;

	nAnalogFormat = CurrentSet->nAnalogType;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined_FHD[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined_FHD[i].pszName) == NULL)
		{
			nSourceIndex = i; break;
		}
	}
	//100101 Change
	switch(nSourceIndex)
	{
		case 1:  //AV
		case 2:  //AV1
			// add PSH 100316
		case 45:  //COMP1-AV1
		case 47:  //COMP2-AV1
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 33;
			break;

		case 3:  //AV2
			// add PSH 100316
		case 46:  //COMP1-AV2
		case 48:  //COMP2-AV2
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 65;
			break;

		case 4:  //AV3
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 53;
			break;

		case 6:  //YC
		case 7:  //YC1
		case 8:  //YC2
		case 9:  //YC3
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 33;
			break;

		case 11:  //COMP
		case 12:  //COMP1
			if(nAnalogFormat == 0)
				nModel = 225;
			else
				nModel = 223;

			nPattern = 65;
			break;

		case 13:  //COMP2
			if(nAnalogFormat == 0)
				nModel = 225;
			else
				nModel = 223;

			nPattern = 53;
			break;
// add 100101
		case 14:  //COMP3
			if(nAnalogFormat == 0)
				nModel = 225;
			else
				nModel = 223;

			nPattern = 67;
			break;
//-
		case 18:  //RGB
			nModel = 60;
			nPattern = 8;
			break;

		case 19:  //HDMI
		case 20:  //HDMI1
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI1_Port);
			}
			nModel = GetHDMI_Model_FHD();
			nPattern = 16;
			break;

		case 21:  //HDMI2
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI2_Port);
			}
			nModel = GetHDMI_Model_FHD();
			nPattern = 20;
			break;

		case 22:  //HDMI3
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI3_Port);
			}
			nModel = GetHDMI_Model_FHD();
			nPattern = 23;
			break;

		case 23:  //HDMI4
			if(CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(CurrentSet->nHDMI4_Port);
			}
			nModel = GetHDMI_Model_FHD();
			nPattern = 6;
			break;

		case 49:  //MHL
			nModel = 45;
			nPattern = 26;
			break;

		case 50:  //HDMI-DIMming
			nModel = GetHDMI_Model_FHD();
			nPattern = 2;
			break;

		case 41:  //OSD_TEST
			nModel = HDMIGeneratorCtrl.m_nModelNo;
			nPattern = 2;
			break;
		case 42:  //SCART1
			nModel = 202;
			nPattern = 33;
			break;

		case 43:  //SCART2
			nModel = 202;
			nPattern = 65;
			break;
		case 44:  //SCART_RGB
			nModel = 232;
			nPattern = 65;
			break;

		case 51:  //COMP-DIM
			if(nAnalogFormat == 0)
				nModel = 225;
			else
				nModel = 223;

			nPattern = 4;
			break;

		case 52:  //AV-DIM
			if(nAnalogFormat == 0)
				nModel = 201;
			else
				nModel = 202;

			nPattern = 4;
			break;


		default:
			break;
	}

	if(pCurStep->m_nTestType == OSD_TEST) nPattern = 2;

	if(nModel == 0) return TRUE;

	if(((nSourceIndex >= 19) && (nSourceIndex <= 23)) || (nSourceIndex == 49) || (nSourceIndex == 50)) 
	{
		bResult = HDMIGeneratorCtrl.SetOut(nModel, nPattern);
		//add 090715
		if(!bResult){
			InitHDMIGen(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate);
			bResult = HDMIGeneratorCtrl.SetOut(nModel, nPattern); 
		}
	}
	else{
		bResult = PatternGeneratorCtrl.SetOut(nModel, nPattern); 
	}

	if(bResult){
		g_CurSetting.m_nInputSource = nSourceIndex;
	}
	else{
		szMsg1 = _T("StepInit_PatternAutoControl- FAIL");
		AddStringToStatus(szMsg1);

		//091019
		if((pCurStep->m_nTestType == MODULE_TEST) || (pCurStep->m_nTestType == TV_COMM_TEST)) 
		{
			return TRUE;
		}

	//	if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		if(CurrentSet->bIsRunning)
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + "; " + szMsg1;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szMsg1);
				pCurStep->m_strMsg = szMsg1;
			}		
		}
	}


	return bResult;
}
BOOL StepInit_AvSwitchBox()
{
	BOOL bRev;

	if(g_nGrabberType == FHD_GRABBER){
		bRev = StepInit_AvSwitchBox_FHD();
	}
	else{
		bRev = StepInit_AvSwitchBox_UHD();
	}

	return bRev;
}
BOOL StepInit_SourceAutoControl()
{
	BOOL bRev;

	if(g_nGrabberType == FHD_GRABBER){
		bRev = StepInit_SourceAutoControl_FHD();
	}
	else{
		bRev = StepInit_SourceAutoControl_UHD();
	}

	return bRev;
}
BOOL StepInit_PatternAutoControl()
{
	BOOL bRev;

	if(g_nGrabberType == FHD_GRABBER){
		bRev = StepInit_PatternAutoControl_FHD();
	}
	else{
		bRev = StepInit_PatternAutoControl_UHD();
	}

	return bRev;
}


//===============================
// Load Reference Folder (Local)
//===============================

//*********************************************************************************
// Function name		- GetFolder
// Description			- Get a folder path
// 콧診 溢怏淸魏打?	- 25.09.2000
// 碣?溢怏淸打尊循壯	- S. Sokolenko
// In					-
//						  strSelectedFolder - reference to string for store folder path
// Out				-
//						  lpszTitle - title for caption
//						  hwndOwner - reference to parent window 
//						  strRootFolder - root folder 
//						  strStartFolder - current foldet
// Return				- TRUE if user select OK, else FALSE.
//*********************************************************************************
CString strTmpPath;

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szDir[MAX_PATH];
	switch(uMsg){
	case BFFM_INITIALIZED:
		if (lpData){
			strcpy_s(szDir, _countof(szDir), strTmpPath.GetBuffer(strTmpPath.GetLength()));
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);
		}
		break;
	case BFFM_SELCHANGED: {
	   if (SHGetPathFromIDList((LPITEMIDLIST) lParam ,szDir)){
		  SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
	   }
	   break;
	}
	default:
	   break;
	}
         
	return 0;
}

BOOL GetFolder(CString* strSelectedFolder,
				   const char* lpszTitle,
				   const HWND hwndOwner, 
				   const char* strRootFolder, 
				   const char* strStartFolder)
{
	char pszDisplayName[MAX_PATH];
	LPITEMIDLIST lpID;
	BROWSEINFOA bi;
	
	bi.hwndOwner = hwndOwner;
	if (strRootFolder == NULL){
		bi.pidlRoot = NULL;
	}else{
	   LPITEMIDLIST  pIdl = NULL;
	   IShellFolder* pDesktopFolder;
	   char          szPath[MAX_PATH];
	   OLECHAR       olePath[MAX_PATH];
	   ULONG         chEaten;
	   ULONG         dwAttributes;

	   strcpy_s(szPath, _countof(szPath), (LPCTSTR)strRootFolder);
	   if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
	   {
		   MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, olePath, MAX_PATH);
		   pDesktopFolder->ParseDisplayName(NULL, NULL, olePath, &chEaten, &pIdl, &dwAttributes);
		   pDesktopFolder->Release();
	   }
	   bi.pidlRoot = pIdl;
	}
	bi.pszDisplayName = pszDisplayName;
	bi.lpszTitle = lpszTitle;
//	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | 0x0040 | BIF_EDITBOX;
	bi.lpfn = BrowseCallbackProc;
	if (strStartFolder == NULL){
		bi.lParam = FALSE;
	}else{
		strTmpPath.Format("%s", strStartFolder);
		bi.lParam = TRUE;
	}
	bi.iImage = NULL;
	lpID = SHBrowseForFolderA(&bi);
	if (lpID != NULL){
		BOOL b = SHGetPathFromIDList(lpID, pszDisplayName);
		if (b == TRUE){
			strSelectedFolder->Format("%s",pszDisplayName);
			return TRUE;
		}
	}else{
		strSelectedFolder->Empty();
	}
	return FALSE;
}

BOOL IsDots(const TCHAR* str) {
    if(_tcscmp(str,".") && _tcscmp(str,"..")) return FALSE;
    return TRUE;
}

BOOL DeleteDirectory(const TCHAR* sPath) {
    HANDLE hFind;  // file handle
    WIN32_FIND_DATA FindFileData;

    TCHAR DirPath[MAX_PATH];
    TCHAR FileName[MAX_PATH];

    _tcscpy_s(DirPath,sPath);
    _tcscat_s(DirPath,"\\*");    // searching all files
    _tcscpy_s(FileName,sPath);
    _tcscat_s(FileName,"\\");

    hFind = FindFirstFile(DirPath,&FindFileData); // find the first file
    if(hFind == INVALID_HANDLE_VALUE) return FALSE;
    _tcscpy_s(DirPath,FileName);
        
    bool bSearch = true;
    while(bSearch) { // until we finds an entry
        if(FindNextFile(hFind,&FindFileData)) {
            if(IsDots(FindFileData.cFileName)) continue;
            _tcscat_s(FileName,FindFileData.cFileName);
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                // we have found a directory, recurse
                if(!DeleteDirectory(FileName)) { 
                    FindClose(hFind); 
                    return FALSE; // directory couldn't be deleted
                }
                RemoveDirectory(FileName); // remove the empty directory
                _tcscpy_s(FileName,DirPath);
            }
            else {
                if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                    _chmod(FileName, _S_IWRITE); // change read-only file mode
                if(!DeleteFile(FileName)) {  // delete the file
                    FindClose(hFind); 
                    return FALSE; 
                }                 
                _tcscpy_s(FileName,DirPath);
            }
        }
        else {
            if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
            bSearch = false;
            else {
                // some error occured, close the handle and return FALSE
                FindClose(hFind); 
                return FALSE;
            }

        }

    }
    FindClose(hFind);  // closing file handle
 
    return RemoveDirectory(sPath); // remove the empty directory

}

void RecursiveDelete(CString szPath)
{
	CFileFind ff;
	CString path = szPath;
	
	if(path.Right(1) != "\\")
		path += "\\";

	path += "*.*";

	BOOL res = ff.FindFile(path);

	while(res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDots() && !ff.IsDirectory())
			DeleteFile(ff.GetFilePath());
		else if (!ff.IsDots() && ff.IsDirectory())
		{
			path = ff.GetFilePath();
			RecursiveDelete(path);
			RemoveDirectory(path);
		}
	}
}


void ResultData_Delete(LONG nPeriodDay) 
{
	CTime curTime = CTime::GetCurrentTime();
	CTimeSpan ts( nPeriodDay, 0, 0, 0 ); 
	CTime delTime;
	CString szString;
	CString		 szYear			  = _T("");
	int nTemp;

	int nYear_temp;
	int nYear;
	int nMonth_temp;
	int nMonth;
	int nDay_temp;
	int nDay;

	delTime = curTime - ts;

	szYear = delTime.Format("[%y_%m_%d]");

	nYear = delTime.GetYear();
	nMonth = delTime.GetMonth();
	nDay = delTime.GetDay();


	CFileFind ff;
	CString sPath;
	CString sTemp;
	CString sTemp2;

	sPath.Format("%s\\DATA\\*.*", g_pView->m_szExePath);
	

	BOOL res = ff.FindFile(sPath);

	while(res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDots() && ff.IsDirectory())
		{
			sPath = ff.GetFilePath();
			if(sPath.Find('[') != -1)
			{
				sTemp = sPath.Right(sPath.GetLength()-sPath.ReverseFind('\\')-1);
     
#if 0

				sTemp.TrimLeft('[');
				sTemp.TrimRight(']');

				sTemp2 = sTemp.Left(2);
				nYear_temp = atoi(sTemp2);
				nYear_temp += 2000;

				sTemp2 = sTemp.Mid(3,2);
				nMonth_temp = atoi(sTemp2);

				sTemp2 = sTemp.Right(2);
				nDay_temp = atoi(sTemp2);

				if((nYear_temp <= nYear) && (nMonth_temp <= nMonth) && (nDay_temp <= nDay))				
				{
					RecursiveDelete(sPath);
					RemoveDirectory(sPath);
				}

#else
				nTemp = sTemp.Compare(szYear);
				if(nTemp == 0)
				{
					RecursiveDelete(sPath);
					RemoveDirectory(sPath);
				}

#endif
			}
		}
	}
}
void GmesLog_Delete(LONG nPeriodDay) 
{
	CTime curTime = CTime::GetCurrentTime();
	CTimeSpan ts( nPeriodDay, 0, 0, 0 ); 
	CTime delTime;
	CString szString;
	CString		 szYear			  = _T("");
	CString		 szYear2		  = _T("");
	int nTemp;

	delTime = curTime - ts;

	szYear = delTime.Format("GmesLog_%Y%m%d");
	szYear2 = delTime.Format("ToolOptionLog_%Y%m%d");
	
	CFileFind ff;
	CString sPath;
	CString sTemp;
	CString sTemp2;

	sPath.Format("%s\\TestResult\\GMES\\*.*", g_pView->m_szExePath);
	

	BOOL res = ff.FindFile(sPath);

	while(res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDots())
		{
			sPath = ff.GetFilePath();
			if(sPath.Find("GmesLog_") != -1)
			{
				sTemp2 = sPath.Right(sPath.GetLength()-sPath.ReverseFind('\\')-1);
				sTemp = sTemp2.Left(sTemp2.ReverseFind('_'));
        
			//	sTemp.TrimRight("_1.txt");
			//	sTemp.TrimRight("_2.txt");
			//	sTemp.TrimRight("_3.txt");

				nTemp = sTemp.Compare(szYear);
				if(nTemp == 0)
				{
					DeleteFile(sPath);
				}
			}
			else if(sPath.Find("ToolOptionLog_") != -1)
			{
				sTemp2 = sPath.Right(sPath.GetLength()-sPath.ReverseFind('\\')-1);
				sTemp = sTemp2.Left(sTemp2.ReverseFind('_'));

				nTemp = sTemp.Compare(szYear2);
				if(nTemp == 0)
				{
					DeleteFile(sPath);
				}
			}

		}
	}
}

void StringToken(CString Source, CString Deliminator, CStringArray& AddIt, BOOL bAddEmpty)
{
	// initialize the variables
	CString		 newCString = Source;
	CString		 tmpCString = _T("");
	CString		 AddCString = _T("");

	int pos1 = 0;
	int pos = 0;

	if(Deliminator.IsEmpty()) {
		// Add default [comma] if empty!
		// acknowledgement: Guru Prasad [gprasad@rti.ie]
		Deliminator = ","; 
	}

	// do this loop as long as you have a deliminator
	do {
		// set to zero
		pos1 = 0;
		// position of deliminator starting at pos1(0)
		pos = newCString.Find(Deliminator, pos1);
		// if the deliminator is found...
		if( pos != -1 ) {
			// load a new var with the info left
			// of the position
			CString AddCString = newCString.Left(pos);
			if(!AddCString.IsEmpty()) {
				// if there is a string to add, then
				// add it to the Array
				AddIt.Add(AddCString);
			}
			else if(bAddEmpty) {
				// if empty strings are ok, then add them
				AddIt.Add(AddCString);
			}

			// make a copy of the of this var. with the info
			// right of the deliminator
			tmpCString = newCString.Mid(pos + Deliminator.GetLength());
			
			// reset this var with new info
			newCString = tmpCString;
		}
	} while( pos != -1 );
	
	if(!newCString.IsEmpty()) {
		// as long as the variable is not emty, add it
		AddIt.Add(newCString);
	}

}


BOOL MesFailSave(CString sPCBID, CString sError)
{
    CStdioFile File;
    CFileException ex;
	CString sTemp;
    TCHAR tmpbuf[128];
    time_t ltime;
    struct tm today;
	CString filename;


    time( &ltime );
    localtime_s(&today, &ltime );

    _tcsftime(tmpbuf, 128, _T("%Y%m%d"), &today);

	sTemp = g_pView->m_szExePath + _T("\\TestResult\\MES");
	
	if(!FileExists(sTemp))   CreateFullPath(sTemp);
//	WIN32_FIND_DATA FindFileData;
//	if(INVALID_HANDLE_VALUE == FindFirstFile(sTemp, &FindFileData))
//		CreateFullPath(sTemp);

	filename.Format(_T("%s\\%s%d.txt"),sTemp, tmpbuf,g_nRunningProcessNo); 


    if (!File.Open(filename, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText, &ex)) {
  //      ex.ReportError();
        return FALSE;
    }
	File.SeekToEnd(); 

    TRY {
		_tcsftime(tmpbuf, 128, _T("%H%M%S"), &today);

		sTemp.Format(_T("# %s : %s\n"), tmpbuf, sPCBID);
        File.WriteString(sTemp);
        File.WriteString(sError);
		File.WriteString(_T("\n"));


        File.Close();
    }

    CATCH (CFileException, e) {
        AfxMessageBox(_T("Unable to save grid list"));
        e->Delete();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}

BOOL MesData_AutoUpLoad_TimeCheck()
{
	CTime curTime = CTime::GetCurrentTime();

	int nHour;

	nHour = curTime.GetHour();
	if(	nHour == 12){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

BOOL TestCountSave(CString sExePath)
{
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sBuf;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	int nPos;

	CTime curTime = CTime::GetCurrentTime();

	sDay = curTime.Format(_T("%Y%m%d")); 
	sMonth = curTime.Format(_T("%Y_%m")); 
	sTemp.Format(_T("%s\\TestResult"), sExePath);
	

	if(!FileExists(sTemp))   CreateFullPath(sTemp);

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	filename.Format(_T("%s\\Test_%s_%d.txt"),sTemp, sMonth, g_nRunningProcessNo); 

	sModel = CurrentSet->sChassisName + "_" + CurrentSet->sModelName;
	
	sTemp2.Format(_T("%s : %s"), sDay, sModel);

	//sTemp.Format("%s : %s, Tested = %d, Failed = %d, NormalFailed = %d, RetryStop = %d, UserStop = %d, UserRetry = %d);
	sTemp.LoadString(IDS_RESULT_SAVE);
	sBuf.Format(sTemp, sDay, sModel, g_nNoTested, g_nFailed, g_nNormalFailed, g_nRetryStop, g_nUserStop, g_nUserRetry);

	hFind = FindFirstFile(filename, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		if(!stdFile.Open( filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))
		{
//			fileExcept.ReportError();
			return FALSE;
		}
		stdFile.WriteString(sBuf);
		stdFile.WriteString(_T("\n"));
		stdFile.Close();
		return TRUE;
	}

	FindClose(hFind);

	CStdioFile stdFileSave;
	CStringArray saDif;

	if( !stdFile.Open(filename, CFile::modeRead, &fileExcept ) )
    {
//		fileExcept.ReportError();
		return FALSE;
	}
	while(stdFile.ReadString(sTemp)){
		if(sTemp != "")	saDif.Add(sTemp);
	}
	stdFile.Close();

	nPos = -1;
	

	for(int i = 0 ; i < saDif.GetSize() ; i++)
	{
		sTemp = saDif.GetAt(i);
		sTemp.TrimLeft();
		sTemp = sTemp.Left(sTemp2.GetLength());
		if(!sTemp2.CompareNoCase(sTemp)) nPos = i; 
	}


	if(nPos != -1)
	{
		saDif.SetAt(nPos, sBuf);
	}
	else
	{
		saDif.Add(sBuf);
	}

	if(!stdFileSave.Open(filename, CFile::modeWrite, &fileExcept))
	{
//		fileExcept.ReportError();
        return FALSE;
    }
	stdFileSave.SeekToBegin(); 

    TRY 
	{
		for(int j = 0 ; j < saDif.GetSize() ; j++)
		{
			stdFileSave.WriteString(saDif.GetAt(j));
			stdFileSave.WriteString(_T("\n"));
		}
		stdFileSave.Close();
    }
    CATCH (CFileException, e) 
	{
		sTemp = _T("Error occurs while saving test quantity information");
        AfxMessageBox(sTemp);  //_T("검사 수량 정보를 저장하는중 에러가 발생하였습니다."));
        e->Delete();
        return FALSE;
    }
    END_CATCH


    return TRUE;
	
}

BOOL GmesLog_Save(CString sExePath, CString sComData, BOOL bFlag)
{
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	//	int nPos;
	CString sMsg;

	CTime curTime = CTime::GetCurrentTime();

	EnterCriticalSection(&g_cs4);

	sTemp2 = curTime.Format(_T("[%H%M%S]"));
	sDay = curTime.Format(_T("%Y%m%d"));
	sTemp.Format(_T("%s\\TestResult\\GMES"), sExePath);

	if (bFlag) {
		sMsg.Format(_T("[RECV(GMES)] : %s\n"), sTemp2);
	}
	else {
		sMsg.Format(_T("[SEND(GMES)] : %s\n"), sTemp2);
	}


	if (!FileExists(sTemp))   CreateFullPath(sTemp);

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	filename.Format(_T("%s\\GmesLog_%s_%d.txt"), sTemp, sDay, g_nRunningProcessNo);

	CStdioFile stdFileSave;

	hFind = FindFirstFile(filename, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		if (!stdFile.Open(filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))
		{
			//			fileExcept.ReportError();
			LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFile.WriteString(sMsg);
		stdFile.WriteString(sComData);
		stdFile.WriteString(_T("\n"));
		stdFile.Close();
		LeaveCriticalSection(&g_cs4);
		return TRUE;
	}

	FindClose(hFind);


	TRY
	{
		if (!stdFileSave.Open(filename, CFile::modeWrite, &fileExcept))
		{
			LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFileSave.SeekToEnd();
		stdFileSave.WriteString(sMsg);
		stdFileSave.WriteString(sComData);
		stdFileSave.WriteString(_T("\n"));
		stdFileSave.Close();
	}
		CATCH(CFileException, e)
	{
		sTemp = _T("Error occurs while saving Gmes Log");
		AfxMessageBox(sTemp);  //_T("검사 수량 정보를 저장하는중 에러가 발생하였습니다."));
		e->Delete();
		LeaveCriticalSection(&g_cs4);
		return FALSE;
	}
	END_CATCH

		LeaveCriticalSection(&g_cs4);

	return TRUE;

}

BOOL ServerMonitorLog_Save(CString sExePath, CString sComData, BOOL bFlag)
{
	//SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	//	int nPos;
	CString sMsg;

	CTime curTime = CTime::GetCurrentTime();

	EnterCriticalSection(&g_cs4);

	sTemp2 = curTime.Format(_T("[%H%M%S]"));
	sDay = curTime.Format(_T("%Y%m%d"));
	sTemp.Format(_T("%s\\TestResult\\DFT_Server"), sExePath);

	if (bFlag) {
		sMsg.Format(_T("[RECV(SVR)] : %s\n"), sTemp2);
	}
	else {
		sMsg.Format(_T("[SEND(SVR)] : %s\n"), sTemp2);
	}

	sMsg += sComData;
	SystemMonitorLog_Save(sMsg);

	if (!FileExists(sTemp))   CreateFullPath(sTemp);

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	filename.Format(_T("%s\\DFT_ServerLog_%s_%d.txt"), sTemp, sDay, g_nRunningProcessNo);

	CStdioFile stdFileSave;

	hFind = FindFirstFile(filename, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		if (!stdFile.Open(filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))
		{
			//			fileExcept.ReportError();
			LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFile.WriteString(sMsg);
		stdFile.WriteString(sComData);
		stdFile.WriteString(_T("\n"));
		stdFile.Close();
		LeaveCriticalSection(&g_cs4);
		return TRUE;
	}

	FindClose(hFind);


	TRY
	{
		if (!stdFileSave.Open(filename, CFile::modeWrite, &fileExcept))
		{
			LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFileSave.SeekToEnd();
		stdFileSave.WriteString(sMsg);
		stdFileSave.WriteString(sComData);
		stdFileSave.WriteString(_T("\n"));
		stdFileSave.Close();
	}
		CATCH(CFileException, e)
	{
		sTemp = _T("Error occurs while saving DFT_Server Log");
		AfxMessageBox(sTemp);  //_T("검사 수량 정보를 저장하는중 에러가 발생하였습니다."));
		e->Delete();
		LeaveCriticalSection(&g_cs4);
		return FALSE;
	}
	END_CATCH

		LeaveCriticalSection(&g_cs4);

	return TRUE;

}


BOOL ToolOptionLog_Save(CString sExePath, CString sComData, BOOL bFlag)
{
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
//	int nPos;
	CString sMsg;

	CTime curTime = CTime::GetCurrentTime();

	EnterCriticalSection(&g_cs4);

	sTemp2 = curTime.Format(_T("[%H%M%S]"));
	sDay = curTime.Format(_T("%Y%m%d")); 
	sTemp.Format(_T("%s\\TestResult\\GMES"), sExePath);
	
	if(bFlag){
		sMsg.Format(_T("[Load] : %s\n"), sTemp2);
	}
	else{
		sMsg.Format(_T("[Save] : %s\n"), sTemp2);
	}


	if(!FileExists(sTemp))   CreateFullPath(sTemp);

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	filename.Format(_T("%s\\ToolOptionLog_%s_%d.txt"),sTemp, sDay, g_nRunningProcessNo); 

	CStdioFile stdFileSave;

	hFind = FindFirstFile(filename, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		if(!stdFile.Open( filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))
		{
//			fileExcept.ReportError();
			LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFile.WriteString(sMsg);
		stdFile.WriteString(sComData);
		stdFile.WriteString(_T("\n"));
		stdFile.Close();
		LeaveCriticalSection(&g_cs4);
		return TRUE;
	}

	FindClose(hFind);


	TRY 
	{
		if(!stdFileSave.Open(filename, CFile::modeWrite, &fileExcept))
		{
			LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFileSave.SeekToEnd(); 
		stdFileSave.WriteString(sMsg);
		stdFileSave.WriteString(sComData);
		stdFileSave.WriteString(_T("\n"));
		stdFileSave.Close();
	}
	CATCH (CFileException, e) 
	{
		sTemp = _T("Error occurs while saving Gmes Log");
		AfxMessageBox(sTemp);  //_T("검사 수량 정보를 저장하는중 에러가 발생하였습니다."));
		e->Delete();
		LeaveCriticalSection(&g_cs4);
		return FALSE;
	}
	END_CATCH

	LeaveCriticalSection(&g_cs4);

    return TRUE;
	
}


BOOL SystemMonitorLog_Save(CString sLogString)
{

	if (g_Using)
		return 0;
	g_Using = 1;


	//CStdioFile stdFile;
	//CFileException fileExcept;
	CString sTemp;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	CString sTemp3;
	//	int nPos;
	CString sMsg;

	CTime curTime = CTime::GetCurrentTime();



	sTemp2 = curTime.Format(_T("[%H:%M:%S]"));
	sDay = curTime.Format(_T("%Y%m%d"));
	
	sMsg = sTemp2;	
	sMsg += ":\n";
	sTemp = sMsg;
	sTemp += sLogString;
	sTemp += _T("\n");

	g_strLogArray.Add(sTemp);

	g_Using = 0;
	SystemMonitorLog_Write();
	//sTemp = g_pView->m_szExePath;
	//sTemp3.Format(_T("\\TestResult\\DebugLog\\System_%d"), g_nRunningProcessNo+1);
	//sTemp += sTemp3;// _T("\\TestResult\\SystemDebug");
	//sTemp += sDay;


	//if (!FileExists(sTemp))   CreateFullPath(sTemp);

	//WIN32_FIND_DATA FindFileData;
	////090909
	//HANDLE hFind;  // file handle


	//filename.Format(_T("%s\\SysLog_%s.txt"), sTemp, curTime.Format(_T("%Y%m%d_%Hh")));
	//

	//CStdioFile stdFileSave;

	//hFind = FindFirstFile(filename, &FindFileData);
	//if (INVALID_HANDLE_VALUE == hFind)
	//{
	//	if (!stdFile.Open(filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))//| CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText
	//	{
	//		//			fileExcept.ReportError();
	//		LeaveCriticalSection(&g_cs4);
	//		return FALSE;
	//	}
	//	stdFile.SeekToEnd();
	//	sMsg = sTemp2;
	//	sMsg += ":\n";
	//	stdFile.WriteString(sMsg);
	//	stdFile.WriteString(sLogString);
	//	stdFile.WriteString(_T("\n"));
	//	stdFile.Close();
	//	LeaveCriticalSection(&g_cs4);
	//	return TRUE;
	//}

	//FindClose(hFind);


	//TRY
	//{
	//	if (!stdFileSave.Open(filename, CFile::modeWrite, &fileExcept))
	//	{
	//		LeaveCriticalSection(&g_cs4);
	//		return FALSE;
	//	}
	//
	//	stdFileSave.SeekToEnd();
	//	sMsg = sTemp2;
	//	sMsg += ":\n";
	//	stdFileSave.WriteString(sMsg);
	//	stdFileSave.WriteString(sLogString);
	//	stdFileSave.WriteString(_T("\n"));
	//	stdFileSave.Close();
	//	
	//	
	//}
	//	CATCH(CFileException, e)
	//{
	//	sTemp = _T("Error occurs while saving Sys Log");
	//	AfxMessageBox(sTemp); 
	//	e->Delete();
	//	LeaveCriticalSection(&g_cs4);
	//	return FALSE;
	//}
	//END_CATCH

	//	LeaveCriticalSection(&g_cs4);

	return TRUE;

}


BOOL SystemMonitorLog_Save(CString sLogString, CString sFile, int Line)
{

	if (g_Using)
		return 0;
	g_Using = 1;

	//	g_strLogArray;


	//CStdioFile stdFile;
	//CFileException fileExcept;
	CString sTemp;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	//CString sTemp3;
	CString sTemp4;
	//	int nPos;
	CString sMsg;

	sFile = sFile.Right(sFile.GetLength());
	sTemp4.Format(_T("(%Ts:Line:%d)"), sFile,Line);

	CTime curTime = CTime::GetCurrentTime();

//	EnterCriticalSection(&g_cs4);

	sTemp2 = curTime.Format(_T("[%H%M%S]"));
	sDay = curTime.Format(_T("%Y%m%d"));

//	sTemp = g_pView->m_szExePath;
	//sTemp3.Format(_T("\\TestResult\\DebugLog\\System_%d"), g_nRunningProcessNo+1);
	//sTemp += sTemp3;// _T("\\TestResult\\SystemDebug");
	//sTemp += sDay;

	sMsg = sTemp2;
	sMsg += sTemp4;
	sMsg += ":\n";
	sTemp = sMsg;
	sTemp += sLogString;
	sTemp += _T("\n");

	g_strLogArray.Add(sTemp);

	g_Using = 0;

	//if (!FileExists(sTemp))   CreateFullPath(sTemp);

	//WIN32_FIND_DATA FindFileData;
	////090909
	//HANDLE hFind;  // file handle

	//filename.Format(_T("%s\\SysLog_%s.txt"), sTemp, curTime.Format(_T("%Y%m%d_%Hh"))); //filename.Format(_T("%s\\SysLog_%s_%H.txt"), sTemp);

	//CStdioFile stdFileSave;

	//hFind = FindFirstFile(filename, &FindFileData);
	//if (INVALID_HANDLE_VALUE == hFind)
	//{
	//	if (!stdFile.Open(filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))//| CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText
	//	{
	//		//			fileExcept.ReportError();
	//		LeaveCriticalSection(&g_cs4);
	//		return FALSE;
	//	}
	//	stdFile.SeekToEnd();
	/*	sMsg = sTemp2;
		sMsg += sTemp4;
		sMsg += ":\n";
		stdFile.WriteString(sMsg);
		stdFile.WriteString(sLogString);
		stdFile.WriteString(_T("\n"));*/
	//	stdFile.Close();
	/*	LeaveCriticalSection(&g_cs4);
		return TRUE;
	}

	FindClose(hFind);


	TRY
	{
		if (!stdFileSave.Open(filename, CFile::modeWrite, &fileExcept))
		{
			LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFileSave.SeekToEnd();
		sMsg = sTemp2;
		sMsg += sTemp4;
		sMsg += ":\n";
		stdFileSave.WriteString(sMsg);
		stdFileSave.WriteString(sLogString);
		stdFileSave.WriteString(_T("\n"));
		stdFileSave.Close();
	
		
	}
		CATCH(CFileException, e)
	{
		sTemp = _T("Error occurs while saving Sys Log");
		AfxMessageBox(sTemp); 
		e->Delete();
		LeaveCriticalSection(&g_cs4);
		return FALSE;
	}
	END_CATCH

		LeaveCriticalSection(&g_cs4);
*/
	return TRUE;

}

BOOL SystemMonitorLog_Write()
{

	if (g_Using)
		return 0;

	if (g_strLogArray.GetCount() > 0)
	{
		g_Using = 1;
	}
	else
	{
		return FALSE;
	}

	g_Using = 1;

//	g_strLogArray;
	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sDay;
	CString sMonth;
	CString filename;
	CString sModel;
	CString sTemp2;
	CString sTemp3;
	CString sTemp4;
	//	int nPos;
	CString sMsg;



	//sTemp4.Format(_T("(%Ts:Line:%d)"), sFile,Line);

	CTime curTime = CTime::GetCurrentTime();



//	sTemp2 = curTime.Format(_T("[%H%M%S]"));
	sDay = curTime.Format(_T("%Y%m%d"));

	sTemp = g_pView->m_szExePath;
	sTemp3.Format(_T("\\TestResult\\DebugLog\\System_%d_"), g_nRunningProcessNo);
	sTemp += sTemp3;// _T("\\TestResult\\SystemDebug");
	sTemp += sDay;


	if (!FileExists(sTemp))   CreateFullPath(sTemp);

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	filename.Format(_T("%s\\SysLog_%s.txt"), sTemp, curTime.Format(_T("%Y%m%d_%Hh"))); //filename.Format(_T("%s\\SysLog_%s_%H.txt"), sTemp);

	CStdioFile stdFileSave;

	hFind = FindFirstFile(filename, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		if (!stdFile.Open(filename, CFile::modeCreate | CFile::modeWrite, &fileExcept))//| CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText
		{
			//			fileExcept.ReportError();
			//LeaveCriticalSection(&g_cs4);
			return FALSE;
		}
		stdFile.SeekToEnd();
		for (int i = 0; i < g_strLogArray.GetCount(); i++)
		{
			sTemp2 = g_strLogArray.GetAt(i);
			stdFile.WriteString(sTemp2);//stdFile.WriteString(g_strLogArray.GetAt(i));
		}

		g_strLogArray.RemoveAll();
		stdFile.Close();
		g_Using = 0;
		return TRUE;
	}

	FindClose(hFind);


	TRY
	{
		if (!stdFileSave.Open(filename, CFile::modeWrite, &fileExcept))
		{
			//LeaveCriticalSection(&g_cs4);
			g_Using = 0;
			return FALSE;
		}
		stdFileSave.SeekToEnd();
		
		for (int i = 0; i < g_strLogArray.GetCount(); i++)
		{
			sTemp2 =  g_strLogArray.GetAt(i);
			stdFileSave.WriteString(sTemp2);
		}

		g_strLogArray.RemoveAll();

		stdFileSave.Close();		
	}
		CATCH(CFileException, e)
	{
		sTemp = _T("Error occurs while saving Sys Log");
		AfxMessageBox(sTemp); 
		e->Delete();
		g_Using = 0; ;// LeaveCriticalSection(&g_cs4);
		return FALSE;
	}
	END_CATCH

	g_Using = 0;

	return TRUE;

}

BOOL TestCountRead(CString sExePath)
{
 	CStdioFile stdFile;
	CFileException fileExcept;
	CString sTemp;
	CString sBuf;
	CString filename;
	CString sModel;
	int nPos;

	CString sDay;
	CString sMonth;
	CString sTemp2;
	CStringArray SplitIt;

	CTime curTime = CTime::GetCurrentTime();

	sDay = curTime.Format(_T("%Y%m%d")); 
	sMonth = curTime.Format(_T("%Y_%m")); 

	sTemp.Format(_T("%s\\TestResult"), sExePath);
	
	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	hFind = FindFirstFile(sTemp, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		g_nNoTested = 0;
		g_nFailed = 0;
		g_nNormalFailed = 0;
		g_nRetryStop = 0;
		g_nUserStop = 0;
		g_nUserRetry = 0;

		return FALSE;
	}

	FindClose(hFind);

	filename.Format(_T("%s\\Test_%s_%d.txt"),sTemp, sMonth, g_nRunningProcessNo); 

	sModel = CurrentSet->sChassisName + "_" + CurrentSet->sModelName;
	sTemp2.Format(_T("%s : %s"), sDay, sModel);

	hFind = FindFirstFile(filename, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		g_nNoTested = 0;
		g_nFailed = 0;
		g_nNormalFailed = 0;
		g_nRetryStop = 0;
		g_nUserStop = 0;
		g_nUserRetry = 0;

		return FALSE;
	}
	FindClose(hFind);

	CStringArray saDif;

	if( !stdFile.Open(filename, CFile::modeRead, &fileExcept ) )
    {
//		fileExcept.ReportError();
		g_nNoTested = 0;
		g_nFailed = 0;
		g_nNormalFailed = 0;
		g_nRetryStop = 0;
		g_nUserStop = 0;
		g_nUserRetry = 0;

		return FALSE;
	}
	while(stdFile.ReadString(sTemp)){
		saDif.Add(sTemp);
	}
	stdFile.Close();

	nPos = -1;

	for(int i = 0 ; i < saDif.GetSize() ; i++)
	{
		sTemp = saDif.GetAt(i);
		sTemp.TrimLeft();
		sBuf = sTemp.Left(sTemp2.GetLength());
		if(!sTemp2.CompareNoCase(sBuf)) nPos = i; 
	}

	if(nPos != -1)
	{
		sTemp = saDif.GetAt(nPos);
		StringToken(sTemp, ",", SplitIt, TRUE);
		
		sBuf = SplitIt.GetAt(1);
		sBuf.Delete(0, sBuf.Find('=') + 1);
		g_nNoTested = _ttoi((const TCHAR*)sBuf);

		sBuf = SplitIt.GetAt(2);
		sBuf.Delete(0, sBuf.Find('=') + 1);
		g_nFailed = _ttoi((const TCHAR*)sBuf);

		sBuf = SplitIt.GetAt(3);
		sBuf.Delete(0, sBuf.Find('=') + 1);
		g_nNormalFailed = _ttoi((const TCHAR*)sBuf);

		sBuf = SplitIt.GetAt(4);
		sBuf.Delete(0, sBuf.Find('=') + 1);
		g_nRetryStop = _ttoi((const TCHAR*)sBuf);

		sBuf = SplitIt.GetAt(5);
		sBuf.Delete(0, sBuf.Find('=') + 1);
		g_nUserStop = _ttoi((const TCHAR*)sBuf);

		sBuf = SplitIt.GetAt(6);
		sBuf.Delete(0, sBuf.Find('=') + 1);
		g_nUserRetry = _ttoi((const TCHAR*)sBuf);


		return TRUE;
	}
	else
	{
		g_nNoTested		= 0;
		g_nFailed		= 0;
		g_nNormalFailed = 0;
		g_nRetryStop	= 0;
		g_nUserStop		= 0;
		g_nUserRetry	= 0;

		return FALSE;
	}
}

BOOL MesData_AutoUpLoad()
{
	CString sFileName;
	CStdioFile stdFile;
	CFileException fileExcept;

	CFileFind ff;
	CString sPath;
	CString sTemp;
	BOOL bRev = TRUE;

	sFileName.Format(_T("%s\\TestResult\\GMES\\*_%d.xml"), g_pView->m_szExePath, g_nRunningProcessNo); 
	

	BOOL res = ff.FindFile(sFileName);

	while(res)
	{
		res = ff.FindNextFile();
		if (!ff.IsDots() && ff.IsDirectory())
		{
			bRev = FALSE;
			sPath = ff.GetFilePath();
			if( stdFile.Open(sPath, CFile::modeRead, &fileExcept ) )
			{
				if(stdFile.ReadString(sTemp))
				{
					bRev = gGmesCtrl.SendCommString(sTemp);
				}
				stdFile.Close();
			}
			if(bRev)
			{
				gGmesCtrl.DisplyComStatus("S6F1", FALSE);
				DeleteFile(sPath);
			}
		}
	}

	return TRUE;
}

int  GetUpLoadData_Count()
{
	CString sFileName;
	CStdioFile stdFile;
	CFileException fileExcept;
	CStringArray saMainData;
	CString sTemp;
	int nCount = 0;

	sFileName.Format(_T("%s\\TestResult\\MES\\MES_ResultData%d.txt"), g_pView->m_szExePath, g_nRunningProcessNo); 

	if(!FileExists(sFileName))
	{
		return 0;
	}

	if( !stdFile.Open(sFileName, CFile::modeRead, &fileExcept ) )
    {
	//	fileExcept.ReportError();
		return 0;
	}
	while(stdFile.ReadString(sTemp)){
		saMainData.Add(sTemp); 
	}
	stdFile.Close();

	nCount = saMainData.GetSize();

	return nCount;
}
BOOL Get_ToolOption(CString sOption)
{
	CStringArray SplitIt;
	CStringArray SplitIt_Option;
	CString sTemp;
	CString sBuf;
	CString sBuf2;
	int nSize;
	int nSize2;
	int nOptionNo;
	CString sOptionVal;
	int nCount;
	int nCount2;
	CString sString;
//	int nFind;
	CString sTemp_Option;

//	[NetCast4.0 L13] Tool Option(1:32852,2:2406,3:26525,4:46864,5:47108,6:97)/ Area Code:1793/ Country Group:JA/ 

	AddStringToStatus(sOption);
	sString = sOption;


	CurrentSet->sToolOption1 = "";
	CurrentSet->sToolOption2 = "";
	CurrentSet->sToolOption3 = "";
	CurrentSet->sToolOption4 = "";
	CurrentSet->sToolOption5 = "";
	CurrentSet->sToolOption6 = "";
	CurrentSet->sToolOption7 = "";
	CurrentSet->sToolOption8 = "";
	CurrentSet->sCommercialOption1 = "";
	CurrentSet->sBoardOption = "";
	CurrentSet->sCommercialBoardOption = "";
	CurrentSet->sToolCRC = "";
	CurrentSet->sAreaOption1 = "";
	CurrentSet->sCountryGroup = "";
	CurrentSet->sCountry = "";

	StringToken(sString, "/", SplitIt, TRUE);

/*	sString.Delete(0, nCount);
	nFind = sString.Find('/');
	sBuf = sString.Left(nFind);
	sTemp_Option = sBuf.Right(sBuf.GetLength() - (sBuf.Find("TOOL OPTION") + 11));
	sTemp_Option.TrimLeft("(");
	sTemp_Option.TrimRight(")");


	StringToken(sTemp_Option, ",", SplitIt, TRUE);
*/
	nSize = SplitIt.GetSize();
	if(nSize == 0) return FALSE;

	for(int j = 0; j< nSize; j++)
	{
		sTemp = SplitIt.GetAt(j);
		sTemp.TrimLeft();
		sTemp.TrimRight();

		nCount = sTemp.Find("TOOL OPTION");
		nCount2 = sTemp.Find("TOOLOPTION");
		if(nCount == -1){
			if(nCount2 != -1){
				nCount = nCount2;
			}
		}

		if(nCount != -1)
		{
			sTemp = sTemp.Right(sTemp.GetLength() - sTemp.Find("("));
			sTemp.TrimLeft("(");
			sTemp.TrimRight(")");

			StringToken(sTemp, ",", SplitIt_Option, TRUE);
			nSize2 = SplitIt_Option.GetSize();
			if(nSize2 == 0) return FALSE;

			for(int i = 0; i< nSize2; i++)
			{
				sTemp_Option = SplitIt_Option.GetAt(i);
				sBuf = sTemp_Option.Left(sTemp_Option.Find(':'));
				nOptionNo = _ttoi((const TCHAR*)sBuf);
				sOptionVal = sTemp_Option.Right(sTemp_Option.GetLength() - (sTemp_Option.Find(':') + 1));

				switch(nOptionNo)
				{
					case 1 :
						CurrentSet->sToolOption1 = sOptionVal;
						break;

					case 2 :
						CurrentSet->sToolOption2 = sOptionVal;
						break;

					case 3 :
						CurrentSet->sToolOption3 = sOptionVal;
						break;

					case 4 :
						CurrentSet->sToolOption4 = sOptionVal;
						break;

					case 5 :
						CurrentSet->sToolOption5 = sOptionVal;
						break;

					case 6 :
						CurrentSet->sToolOption6 = sOptionVal;
						break;

					case 7 :
						CurrentSet->sToolOption7 = sOptionVal;
						break;

					case 8 :
						CurrentSet->sCommercialOption1 = sOptionVal;
						break;

					case 9 :
						CurrentSet->sToolOption8 = sOptionVal;
						break;

				}
			}
		}
		else if((sTemp.Find("AREA CODE") != -1) || (sTemp.Find("AREACODE") != -1))
		{
		//	sTemp.TrimLeft("AREA CODE:");
			sTemp.Delete(0, sTemp.Find(':') +1);
			CurrentSet->sAreaOption1 = sTemp;
		}
		else if((sTemp.Find("COUNTRY GROUP") != -1) || (sTemp.Find("COUNTRYGROUP") != -1))
		{
			//sTemp.TrimLeft("COUNTRY GROUP:");
			sTemp.Delete(0, sTemp.Find(':') +1);
			//CurrentSet->sCountry = sTemp;
			CurrentSet->sCountryGroup = sTemp;
		}
		else if(sTemp.Find("COUNTRY") != -1)
		{
		//	sTemp.TrimLeft("COUNTRY:");
			sTemp.Delete(0, sTemp.Find(':') +1);
			CurrentSet->sCountry = sTemp;
		//	CurrentSet->sCountryGroup = sTemp;
		}
		else if(sTemp.Find("TOOLCRC") != -1)
		{
			sTemp.Delete(0, sTemp.Find(':') +1);
			CurrentSet->sToolCRC = sTemp;
		//	CurrentSet->sCountryGroup = sTemp;
		}
	}
//	AddStringToStatus(sOption);

	return TRUE;
}



int Get_CountryGroup_Code(CString sCountryGroup)
{
	int nCountryGroup_Code;

	if(sCountryGroup.Compare("KR") == 0)			nCountryGroup_Code = 0x01;   //(한국)
	else if(sCountryGroup.Compare("US") == 0)		nCountryGroup_Code = 0x02;   //(미국)
	else if(sCountryGroup.Compare("BR") == 0)		nCountryGroup_Code = 0x03;   //(브라질)
	else if(sCountryGroup.Compare("EU") == 0)		nCountryGroup_Code = 0x04;   //(유럽)
	else if(sCountryGroup.Compare("CN") == 0)		nCountryGroup_Code = 0x05;   //(중국)	
	else if(sCountryGroup.Compare("AU") == 0)		nCountryGroup_Code = 0x06;   //(Australia)
	else if(sCountryGroup.Compare("SG") == 0)		nCountryGroup_Code = 0x07;   //(Singapore) 
	else if(sCountryGroup.Compare("ZA") == 0)		nCountryGroup_Code = 0x08;   //(South Africa)	0x08 
	else if(sCountryGroup.Compare("VN") == 0)		nCountryGroup_Code = 0x09;   //(Vietnam)	0x09 
	else if(sCountryGroup.Compare("TW") == 0)		nCountryGroup_Code = 0x0a;   //(대만)		    
	else if(sCountryGroup.Compare("A-CSA") == 0)	nCountryGroup_Code = 0x0b;   //(Central and South America)	0x0b 
	else if(sCountryGroup.Compare("A-ASIA") == 0)	nCountryGroup_Code = 0x0c;   //(Middle East & Africa)	0x0c 
	else if(sCountryGroup.Compare("IL") == 0)		nCountryGroup_Code = 0x0d;   //(이스라엘)	    
	else if(sCountryGroup.Compare("ID") == 0)		nCountryGroup_Code = 0x0e;   //(Indonesia)	0x0e 
	else if(sCountryGroup.Compare("MY") == 0)		nCountryGroup_Code = 0x0f;   //(Malaysia)	0x0f 
	else if(sCountryGroup.Compare("IR") == 0)		nCountryGroup_Code = 0x10;   //(Iran)	0x10 
	else if(sCountryGroup.Compare("HK") == 0)		nCountryGroup_Code = 0x11;   //(홍콩)		    
	else if(sCountryGroup.Compare("JP") == 0)		nCountryGroup_Code = 0x12;   //(일본)		    
	else if(sCountryGroup.Compare("AJ") == 0)		nCountryGroup_Code = 0x13;   //(아시아)		
	else if(sCountryGroup.Compare("JA") == 0)		nCountryGroup_Code = 0x14;   //(중아)		    
	else if(sCountryGroup.Compare("CU") == 0)		nCountryGroup_Code = 0x15;   //(쿠바)		    
	else											nCountryGroup_Code = 0x00;   //(error)		    

	return nCountryGroup_Code;
}

int Get_Country_Code(CString sCountry)
{
	int nCountry_Code;

	if(sCountry.Compare("US") == 0)		 nCountry_Code =  0x555341;   //(미국)
	else if(sCountry.Compare("CA") == 0) nCountry_Code =  0x43414e;   //(캐나다)
	else if(sCountry.Compare("MX") == 0) nCountry_Code =  0x4d4558;   //(멕시코)
	else if(sCountry.Compare("PH") == 0) nCountry_Code =  0x50484c;   //(필리핀)	
	else if(sCountry.Compare("HN") == 0) nCountry_Code =  0x484e44;   //(온두라스스)	
	else if(sCountry.Compare("BR") == 0) nCountry_Code =  0x425241;   //(브라질)		    
	else if(sCountry.Compare("CL") == 0) nCountry_Code =  0x43484c;   //(칠레)		    
	else if(sCountry.Compare("PE") == 0) nCountry_Code =  0x504552;   //(페루)		    
	else if(sCountry.Compare("AR") == 0) nCountry_Code =  0x415247;   //(아르헨티나)	    
	else if(sCountry.Compare("EC") == 0) nCountry_Code =  0x454355;   //(에콰도르)		
	else if(sCountry.Compare("TW") == 0) nCountry_Code =  0x54574e;   //(대만)		    
	else if(sCountry.Compare("CO") == 0) nCountry_Code =  0x434f4c;   //(콜럼비아)		
	else if(sCountry.Compare("PA") == 0) nCountry_Code =  0x50414e;   //(파나마)		    
	else if(sCountry.Compare("CU") == 0) nCountry_Code =  0x435542;   //(쿠바)		    
	else								 nCountry_Code =  0x00;   //(error)		    

	return nCountry_Code;
}


void SetI2cClkDelay() 
{
	int nResult = TEST_PASS;
	CString szErrMsg = _T("");

	if(I2cAdcCtrl.m_bPortOpen == FALSE)
	{
		if(!I2cAdcCtrl.Create(CurrentSet->sI2cAdcComPort, CurrentSet->wI2cAdcBaudRate))
		{
			szErrMsg.LoadString(ERROR_I2C_COMPORT);
			AddStringToStatus(szErrMsg); return;
		}
	}

	if((nResult = I2cAdcCtrl.SetI2cClkSpeed(CurrentSet->nI2cClkSpeed)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AddStringToStatus(szErrMsg); return;
	}

	if((nResult = I2cAdcCtrl.SetI2cPacketDelay(CurrentSet->nI2cPacketDelay)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AddStringToStatus(szErrMsg); return;
	}

	if((nResult = I2cAdcCtrl.SetI2cReadByteDelay(CurrentSet->nI2cReadByteDelay)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AddStringToStatus(szErrMsg); return;
	}

	if((nResult = I2cAdcCtrl.SetI2cWritByteDelay(CurrentSet->nI2cWriteByteDelay)) != TEST_PASS)
	{
		szErrMsg.LoadString(nResult);
		AddStringToStatus(szErrMsg); return;
	}

	AddStringToStatus("I2C Clock and Delay are set successfully"); 
}
/*
int GetHdcpKeyData(CString sHdcpFilePath, BOOL bFlag)
{
	WIN32_FIND_DATA FindFileData;
	CFileException FileExcept;
    HANDLE hFind;  // file handle
	CString sFileName;
	CString sHdcpFileName;
	CString sHdcpFileTemp;
	CFile pFile;
	CFileStatus status;

	memset(I2cAdcCtrl.m_nHdcpKeyWriteData,400,0);

	sFileName = sHdcpFilePath + "\\*.bin";
	hFind = FindFirstFile(sFileName, &FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)
	{
		return 0;
	}

	sHdcpFileName.Format("%s\\%s", sHdcpFilePath, FindFileData.cFileName);	

	FindClose(hFind);

	sHdcpFileTemp = sHdcpFileName;
	sHdcpFileTemp.Replace(".bin", ".bi_");
	
	MoveFile((LPCTSTR)sHdcpFileName, (LPCTSTR)sHdcpFileTemp);
//	CurrentSet->sHdcpKeyFileName = sHdcpFileName;

	if(!pFile.Open(sHdcpFileTemp, CFile::modeRead | CFile::shareDenyNone, &FileExcept)) 
    {
		FileExcept.ReportError();
		return 0;
	}
	pFile.GetStatus(status);
	pFile.Read(I2cAdcCtrl.m_nHdcpKeyWriteData, status.m_size);
	pFile.Close();	

	if(bFlag){
		CurrentSet->sHdcpKeyFileName = sHdcpFileTemp;
	}
	else{
		MoveFile((LPCTSTR)sHdcpFileTemp, (LPCTSTR)sHdcpFileName);
		CurrentSet->sHdcpKeyFileName = sHdcpFileName;
	}
	return status.m_size;
}

int GetHdcpKeyNumber(CString sFilePath)
{
	CFileFind find; // file 검색을 위한 클래스
	CString path; 
	CString fname;
	CString pname;

	BOOL bRes = FALSE;
	int fileCnt = 0;
//	int fileSize;
  
// 	path = sHdcpFilePath + "\\*.bin";
	bRes = find.FindFile((LPCTSTR)sFilePath); 
  
	while(bRes)
	{
		bRes = find.FindNextFile();
		if (!find.IsDots() && !find.IsDirectory())
		{
			fileCnt++; // 이 부분에서 파일의 갯수를 세어줍니다.

		}
	}
	return fileCnt;
}
*/
void UpDateInfo_ToolOption()
{
	g_pView->UpdateToolOptionInfo(FALSE);

}
void DischargCheck(double dLimit)
{
	CString sVolt1 = _T("");
	CString sVolt2 = _T("");
	CString sVolt3 = _T("");
	TCHAR *stopstring;

	CStringArray SplitIt;

	int nCH_S;
	int nCH_E;
	double dVol = 0.0;
	double dVolt1 = 0.0;
	double dVolt2 = 0.0;
	double dVolt3 = 0.0;
	CString sVol = _T("");


	if(g_nRunningProcessNo == 1)
	{
		nCH_S = 1039;
		nCH_E = 1041;
	}
	else if(g_nRunningProcessNo == 2)
	{
		nCH_S = 2039;
		nCH_E = 2041;
	}
	else
	{
		nCH_S = 3039;
		nCH_E = 3041;
	}

	dVol = g_pView->GetDaqData(DAQ_FUNC_DC_M, nCH_S, nCH_E,sVol);

	StringToken(sVol, ",", SplitIt, TRUE);
	if(SplitIt.GetSize() == 3){ 
		sVolt1 = SplitIt.GetAt(0);
		sVolt2 = SplitIt.GetAt(1);
		sVolt3 = SplitIt.GetAt(2);
		dVolt1 = _tcstod((LPCTSTR)sVolt1, &stopstring);
		dVolt2 = _tcstod((LPCTSTR)sVolt2, &stopstring);
		dVolt3 = _tcstod((LPCTSTR)sVolt3, &stopstring);

	}
	else{
		return ;
	}

	if(dVolt1 < dVolt2){
		dVol = dVolt2;
	}
	else{
		dVol = dVolt1;
	}
	if(dVol < dVolt3){
		dVol = dVolt3;
	}
	if(dVol> dLimit)
	{
		CXDischargDlg *pDlg = new CXDischargDlg;

		pDlg->m_dLimit = dLimit;

		pDlg->DoModal();
		delete pDlg;
	}
}

BOOL Gmes_Connection()
{
	CString sTmp = _T("");
	CString sRtn;

	if(gGmesCtrl.m_bGmesCom){
		gGmesCtrl.SoketCom_Close();
	}
	Sleep(500);
	
	if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
		sTmp = gGmesCtrl.MakeElem_S1F1();
		if(gGmesCtrl.SendCommString(sTmp))
		{
			gGmesCtrl.DisplyComStatus("S1F1", FALSE);
		}
		CurrentSet->bGMES_Connection = TRUE;
		g_pView->UpdateInfo(LED_STATE,"");
		return TRUE;
	}
	else{
		sRtn.LoadString(IDS_MES_MSG2);
		AfxMessageBox(sRtn,MB_OK|MB_ICONINFORMATION);
		CurrentSet->bGMES_Connection = FALSE;
		g_pView->UpdateInfo(LED_STATE,"");
		return FALSE;
	}	
}

vector<int> makeBitArray(unsigned int val)
{
	vector<int> res(32,0);         //int값을 array로 변경하여 저장되는 장소
	for(int i=0;i<32;i++){          //해당 logic은 10진수를 2진수로 변경하는 logic
		res[i] = val%2;       
		val = val/2;
	}
	reverse(res.begin(),res.end());    //logic상 작은 숫자에서 큰숫자로 bit가 나오기 때문에 맨 마지막에 reverse를 해서 res[0]에 가장 큰 숫자가 오도록 배치
	return res;
}
//
//BOOL CheckToolCrc()
//{
//	vector<unsigned int> tool(13,0);
//	unsigned int ID = 0x8005;   //CRC Sum의 key값, 미리 define되는 부분이며 많이 사용되고 있는 0x8005로 사용
//
//	unsigned int CRC = 0;       //CRC Sum의 초기값은 0으로 둔다
//	unsigned int nCurCRC = 0;       
//	
//	if (CurrentSet->sBoardOption != "") {
//		tool[1] = hexStr2DecNum(CurrentSet->sBoardOption.GetBuffer());
//	}
//	else {
//		tool[1] = 0;
//	}
//	if (CurrentSet->sCommercialBoardOption != "") {
//		tool[2] = hexStr2DecNum(CurrentSet->sCommercialBoardOption.GetBuffer());
//	}
//	else {
//		tool[2] = 0;
//	}
//
//	if(CurrentSet->sToolOption1 != ""){
//		tool[3] = atoi(CurrentSet->sToolOption1);
//	}
//	else{
//		tool[3] = 0;
//	}
//	if(CurrentSet->sToolOption2 != ""){
//		tool[4] = atoi(CurrentSet->sToolOption2);
//	}
//	else{
//		tool[4] = 0;
//	}
//	if(CurrentSet->sToolOption3 != ""){
//		tool[5] = atoi(CurrentSet->sToolOption3);
//	}
//	else{
//		tool[5] = 0;
//	}
//	if(CurrentSet->sToolOption4 != ""){
//		tool[6] = atoi(CurrentSet->sToolOption4);
//	}
//	else{
//		tool[6] = 0;
//	}
//	if(CurrentSet->sToolOption5 != ""){
//		tool[7] = atoi(CurrentSet->sToolOption5);
//	}
//	else{
//		tool[7] = 0;
//	}
//	if(CurrentSet->sToolOption6 != ""){
//		tool[8] = atoi(CurrentSet->sToolOption6);
//	}
//	else{
//		tool[8] = 0;
//	}
//	if(CurrentSet->sToolOption7 != ""){
//		tool[9] = atoi(CurrentSet->sToolOption7);
//	}
//	else{
//		tool[9] = 0;
//	}
//	if(CurrentSet->sCommercialOption1 != ""){
//		tool[10] = atoi(CurrentSet->sCommercialOption1);
//	}
//	else{
//		tool[10] = 0;
//	}
//	if(CurrentSet->sToolOption8 != ""){
//		tool[11] = atoi(CurrentSet->sToolOption8);
//	}
//	else{
//		tool[11] = 0;
//	}
//
//	if(CurrentSet->sToolCRC != ""){
//		nCurCRC = atoi(CurrentSet->sToolCRC);
//	}
//	else{
//		nCurCRC = 0;
//	}
//
//
//	for(int t=1;t<=11;t++){      // ToolOption 1~9번까지 모든 data들을 확인한다.
//		unsigned int val = tool[t];          //해당 ToolOption값을 우선적으로 val값에 저장
//		vector<int> bitarray = makeBitArray(val);         //해당 val를 bitarray로 바꿔주는 부분 vector<int> 는 int형 vector (array로 생각해도 무방)
//		for(int i=0;i<32;i++){				 //32bit에 해당되는 모든 bit를 한 bit씩 순회 가장 큰 bit부터 확인
//			if((bitarray[i]) ^ (CRC>>15)){		 //현재 bit에 해당되는 부분과 CRC의 가장 큰 숫자 즉 (CRC>>15를 할시 2진법상 가장큰수)의 xor이 1일시 실행
//				CRC = ((CRC << 1) & 0xffff) ^ ID;	// (CRC<<1은 CRC*2와 동일) CRC 오른쪽으로 1bit shift값과 ID값으 xor값을 CRC에 저장 //((CRC << 1) & 0xffff)이유는 CRC가 unsigned int이기 때문에 16bit를 초과할 가능성이 있어 해당부분 반영
//			}	
//			else{
//				CRC = (CRC << 1) & 0xffff;  //xor값이 0일시 CRC만 오른쪽으로 1bit shift
//			}
//		}
//	}
//	
//	if(nCurCRC == CRC){
//		return TRUE;
//	}
//	else{
//		CString sTemp;
//		if(nCurCRC == 0){
//			sTemp.Format("TOOL-CRC NG : Calculation Value-%d, Set Value-%d\nKey-In CRC Value", nCurCRC, CRC);
//		}
//		else{
//			sTemp.Format("TOOL-CRC NG : Calculation Value-%d, Set Value-%d\nKey-In  right CRC Value", nCurCRC, CRC);
//		}
//
//		return FALSE;
//	}
//}
BOOL ChekDrmKey()
{
	BOOL bDrmKey = FALSE;

	if((CurrentSet->sCI == "YES") || (CurrentSet->sCNTV == "YES") || (CurrentSet->sDTCP_RX == "YES")||
		(CurrentSet->sESN == "YES") || (CurrentSet->sHDCP_RX == "YES") || (CurrentSet->sHDCP_RX_14 == "YES") ||
		(CurrentSet->sMAC_ADDRESS == "YES") || (CurrentSet->sASTC30_Key == "YES") || 
		(CurrentSet->sMfi_Key == "YES") ||(CurrentSet->sCI_CEP_Key == "YES") || (CurrentSet->sMARINKEY == "YES") || (CurrentSet->sWIDEVINE == "YES"))//sCI_CEP_Key
	{
		bDrmKey = TRUE;
	}

	return bDrmKey;
}

BOOL ChekDrmKeyScanRead()
{
	BOOL bDrmKey = FALSE;

	if((CurrentSet->sCI == "YES") 
		
		//|| (CurrentSet->sCNTV == "YES")
		|| (CurrentSet->sDVBS == "YES")
		|| (CurrentSet->sDVBS2 == "YES")
		|| (CurrentSet->sESN == "YES")
		|| (CurrentSet->sDTCP_RX == "YES")
		|| (CurrentSet->sHDCP_RX == "YES")
		|| (CurrentSet->sHDCP_RX_14 == "YES")
		|| (CurrentSet->sHDCP_TX == "YES")
		//|| (CurrentSet->sWASU == "YES")
		|| (CurrentSet->sADC == "YES")
		|| (CurrentSet->sWIDEVINE == "YES")
		|| (CurrentSet->sMARINKEY == "YES")
		|| (CurrentSet->sBUILT_WIFI == "YES")
		|| (CurrentSet->sJAPAN_LNB == "YES")
		|| (CurrentSet->sMAC_ADDRESS == "YES")
		|| (CurrentSet->sASTC30_Key == "YES")
		|| (CurrentSet->sMfi_Key == "YES")
		|| (CurrentSet->sCI_CEP_Key == "YES")
		|| (CurrentSet->sCA == "YES"))
	{
		bDrmKey = TRUE;
	}

	return bDrmKey;
}
int CheckKeyGmesVal()
{
	int nVal = 1;

	if(CurrentSet->sMAC_ADDRESS	== "YES"){
		nVal = nVal | 0x02;
	}
	if(CurrentSet->sESN	== "YES"){
		nVal = nVal | 0x04;
	}
	if(CurrentSet->sWIDEVINE	== "YES"){
		nVal = nVal | 0x08;
	}
	if(CurrentSet->sHDCP_RX_14	== "YES"){
		nVal = nVal | 0x10;
	}
	if(CurrentSet->sHDCP_RX	== "YES"){
		nVal = nVal | 0x20;
	}
	if(CurrentSet->sMARINKEY	== "YES"){
		nVal = nVal | 0x80;
	}
	if(CurrentSet->sCI	== "YES"){
		nVal = nVal | 0x100;
	}
	if(CurrentSet->sCNTV	== "YES"){
		nVal = nVal | 0x200;
	}
	if(CurrentSet->sDTCP_RX	== "YES"){
		nVal = nVal | 0x400;
	}
	if(CurrentSet->sASTC30_Key	== "YES"){
		nVal = nVal | 0x800;
	}
	if(CurrentSet->sMfi_Key	== "YES"){
		nVal = nVal | 0x1000;
	}	
		
	if(CurrentSet->sCI_CEP_Key == "YES"){
		nVal = nVal | 0x2000;
	}
		
		return nVal;
}
