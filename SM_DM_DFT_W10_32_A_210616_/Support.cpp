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
#include "IrCheckerCtrl.h"
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
#include "AnalogGrabber.h"
#include "CurrentSetting.h"
#include "SoundCard.h"
#include "CSpreadSheet.h"
#include "HdmiGenerator.h"
#include "thermometer.h"

#include "Gmes.h"
#include "XLEzAutomation.h"// #include "CExcel.h"

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
extern _InputSource			InputDefined[];
extern CPatternGenerator	PatternGeneratorCtrl;
extern CTVCommCtrl			TVCommCtrl;
extern CAvSwitchBox			AvSwitchBoxCtrl;
extern CScannerCtrl			ScannerCtrl;
extern CString				m_strWipId;	//Wip ID
//extern UINT					g_GrabSource;
extern UINT					g_VideoSourceType;
extern CCurrentSetting		g_CurSetting;
extern CSoundCard			g_SoundCard;
//+add 090210(Modification No1)
extern CBaseImage			g_GrabImage;

//+change 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

extern CRITICAL_SECTION g_cs4;  
extern int g_nRunningProcessNo;

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

		if(pCurStep->GetNominal_Bool() != pCurStep->m_bMeasured)
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
		if(pCurStep->m_nTestType == MULTICHECL_TEST)
		{
		//	sMsg.Format(_T("%.02f, %.02f"), pCurStep->m_fMeasured_L,pCurStep->m_fMeasured_R);
		//	g_pView->MeasurDisplay(sMsg);
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
	// change 090903
//	g_ctrlComLog.AddText( "\r\n" );
//	g_ctrlComLog.AddText( szString );
	g_pView->ComLog_AddString( "\r\n" );
	g_pView->ComLog_AddString( szString );
}

void AddStringToRemoteComLog(CString szString)
{
	// change 090903
//	g_ctrlComLog.AddText( "\r\n" );
//	g_ctrlComLog.AddText( szString );
	g_pView->ComLog_AddString( "\r\n" );
	g_pView->ComLog_AddString( szString );
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

	if(!pFile.Open(sModelInfoFilePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &FileExcept)) 
	{
		char szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	m_Ini.SetIniFile(sModelInfoFilePath);

/*	//=========================
	// Grabber Resolution Info
	//=========================
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Height",CurrentSet->nLvdsHeight);
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Width" ,CurrentSet->nLvdsWidth);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Height",CurrentSet->nAnalogHeight);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Width",CurrentSet->nAnalogWidth);
	m_Ini.SetProfileInt(GRAB_S, "Uranous Grabber Mode",CurrentSet->nUranousMode);
	m_Ini.SetProfileInt(GRAB_S, "Color Bit Count",CurrentSet->nBitCount);
	m_Ini.SetProfileInt(GRAB_S, "ImageRotation",CurrentSet->nImageRotation);
*/	
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Type",CurrentSet->nAnalogType);

	//==================
	// Tool/Area Option
	//==================
/*	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option1",CurrentSet->sToolOption1);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option2",CurrentSet->sToolOption2);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option3",CurrentSet->sToolOption3);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option4",CurrentSet->sToolOption4);
	//091016
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option5",CurrentSet->sToolOption5);
	m_Ini.SetProfileString(TOOL_AREA_S, "Area Option1",CurrentSet->sAreaOption1);

	m_Ini.SetProfileString(VERSION_CHECK_S, "CPU Version",CurrentSet->sCPUVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Micom Version",CurrentSet->sMicomVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "USB Version",CurrentSet->sUSBVersion);
*/

	//CurrentSet->wTVBaudRate = wBaudRate;
	//CurrentSet->wTVBaudRate1 = wBaudRate;
	//CurrentSet->wTVBaudRate2 = wBaudRate;

	//CurrentSet->nChipType = CHIP_MTK;
	//CurrentSet->nChipType1 = CHIP_MTK;
	//CurrentSet->nChipType2 = CHIP_MTK;

	//CurrentSet->nTVParity = EVENPARITY;
	//CurrentSet->nTVParity1 = EVENPARITY;
	//CurrentSet->nTVParity2 = EVENPARITY;
	//+add psh 080701  pCurrentSet->bUseTVCommPort			= pApp->GetProfileInt(_T("Config"), _T("Use TVComm Port"), 1);
	//if (g_nRunningProcessNo == 1)
	//{
	//	m_Ini.SetProfileInt(GENERAL_S, "Use TVComm Port1", CurrentSet->bUseTVCommPort);
	//}
	//else
	//{
	//	m_Ini.SetProfileInt(GENERAL_S, "Use TVComm Port2", CurrentSet->bUseTVCommPort);
	//}
	CurrentSet->bUseTVCommPort = 1;


//	m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate", CurrentSet->wTVBaudRate);
//	m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Parity", CurrentSet->nTVParity);
	//m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number", CurrentSet->sTVComPort);
	if (g_nRunningProcessNo == 1)
	{
		m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number1", CurrentSet->sTVComPort);
		//m_Ini.SetProfileInt(TV_CONTROL_S, "Micom Chip Type", CurrentSet->nChipType);
		m_Ini.SetProfileInt(TV_CONTROL_S, "Micom Chip Type1", CurrentSet->nChipType);
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Parity1", CurrentSet->nTVParity);
		
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate1", CurrentSet->wTVBaudRate);
		
	}
	else
	{
		m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number2", CurrentSet->sTVComPort);
		m_Ini.SetProfileInt(TV_CONTROL_S, "Micom Chip Type2", CurrentSet->nChipType);
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Parity2", CurrentSet->nTVParity);
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate2", CurrentSet->wTVBaudRate);
	}
	

	//pCurrentSet->sTVComPort				= pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "COM6");
//if(pCurrentSet->sTVComPort			== _T("")) pCurrentSet->sTVComPort = "COM6";


/*
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
*/
	m_Ini.SetProfileString(VERSION_CHECK_S, "BE Version",			CurrentSet->sBE_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "FE Version",			CurrentSet->sFE_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Micom Version",		CurrentSet->sMicom_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "DSP Version",			CurrentSet->sDSP_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Sub DSP Version",		CurrentSet->sSubDSP_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "EQ Version",			CurrentSet->sEQ_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "MEQ Version",			CurrentSet->sMEQ_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Touch Version",		CurrentSet->sTouch_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Demo Version",			CurrentSet->sDemo_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "DJ Sound Version",		CurrentSet->sDJSound_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Wireless Tx Version",	CurrentSet->sWirelessTx_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Wireless Rx Version",	CurrentSet->sWirelessRx_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "BT Version",			CurrentSet->sBT_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "HDMI Version",			CurrentSet->sHDMI_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Checksum", CurrentSet->sChecksum);
	CString sTemp;
	sTemp = "";
	for (int i = 0; i < 15; i++)
	{
		if (CurrentSet->bVerChecked[i] == 1)
		{
			sTemp += "1";
		}
		else
		{
			sTemp += "0";
		}
	}
	m_Ini.SetProfileString(VERSION_CHECK_S, "Checked Display", sTemp);
	//VerChecked	CString		sBT_Version;
	//CString		sHDMI_Version;

	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Source Control",CurrentSet->bAutoSourceControl);
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Pattern Control",CurrentSet->bAutoPatternControl);

	//+add psh 100310
	//============
	// HDMI Gen. Control
	//============
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Resolution",CurrentSet->nHDMIResolution);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Color Bit Count",CurrentSet->nHDMIBitSpec);

	//+add psh 110416
	//============
	// Remocon Custom Code
	//============
	m_Ini.SetProfileInt(REMOCON_CUSTOM_S, "Remocon Custom Code",CurrentSet->nRemoteCustomCode);
	m_Ini.SetProfileInt(REMOCON_CUSTOM_S, "Remocon Type",CurrentSet->nRemoteType);

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

	if(!pFile.Open(sIniPath, CFile::modeRead | CFile::modeWrite | CFile::shareDenyNone, &FileExcept)) 
	{
		char   szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}
	pFile.Close();

	m_Ini.SetIniFile(sIniPath);

/*	//=========================
	// Grabber Resolution Info
	//=========================
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Height",CurrentSet->nLvdsHeight);
	m_Ini.SetProfileInt(GRAB_S, "LVDS Grabber Width" ,CurrentSet->nLvdsWidth);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Height",CurrentSet->nAnalogHeight);
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Width",CurrentSet->nAnalogWidth);
	m_Ini.SetProfileInt(GRAB_S, "Uranous Grabber Mode",CurrentSet->nUranousMode);
	//+add psh 090630
	m_Ini.SetProfileInt(GRAB_S, "LVDS OddDE Only",CurrentSet->bOddDE_Only);

	m_Ini.SetProfileInt(GRAB_S, "Color Bit Count",CurrentSet->nBitCount);
	m_Ini.SetProfileInt(GRAB_S, "Uranous Grabber Mode",CurrentSet->nUranousMode);
*/
	m_Ini.SetProfileInt(GRAB_S, "Analog Grabber Type",CurrentSet->nAnalogType);

	m_Ini.SetProfileInt(GRAB_S, "Minimum no of used colors",CurrentSet->nNoUsedColors);

//	m_Ini.SetProfileInt(GRAB_S, "ImageRotation",CurrentSet->nImageRotation);
/*
	//==================
	// Tool/Area Option
	//==================
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option1",CurrentSet->sToolOption1);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option2",CurrentSet->sToolOption2);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option3",CurrentSet->sToolOption3);
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option4",CurrentSet->sToolOption4);
	//091016
	m_Ini.SetProfileString(TOOL_AREA_S, "Tool Option5",CurrentSet->sToolOption5);
	m_Ini.SetProfileString(TOOL_AREA_S, "Area Option1",CurrentSet->sAreaOption1);
//	m_Ini.SetProfileString(TOOL_AREA_S, "Area Option2",CurrentSet->sAreaOption2);

	m_Ini.SetProfileString(VERSION_CHECK_S, "CPU Version",CurrentSet->sCPUVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Micom Version",CurrentSet->sMicomVersion);
	m_Ini.SetProfileString(VERSION_CHECK_S, "USB Version",CurrentSet->sUSBVersion);

*/
	//+add psh 080701
	//if (g_nRunningProcessNo == 1)
	//{
	//	m_Ini.SetProfileInt(GENERAL_S, "Use TVComm Port1", CurrentSet->bUseTVCommPort);
	//}
	//else
	//{
	//	m_Ini.SetProfileInt(GENERAL_S, "Use TVComm Port2", CurrentSet->bUseTVCommPort);
	//}
	//m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate", CurrentSet->wTVBaudRate);
	//m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Parity", CurrentSet->nTVParity);
	//	m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate", CurrentSet->wTVBaudRate);
	//	m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Parity", CurrentSet->nTVParity);
	//	m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number", CurrentSet->sTVComPort);
	if (g_nRunningProcessNo == 1)
	{
		m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number1", CurrentSet->sTVComPort);
		m_Ini.SetProfileInt(TV_CONTROL_S, "Micom Chip Type1", CurrentSet->nChipType);
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Parity1", CurrentSet->nTVParity);
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate1", CurrentSet->wTVBaudRate);
	}
	else
	{
		m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number2", CurrentSet->sTVComPort);
		m_Ini.SetProfileInt(TV_CONTROL_S, "Micom Chip Type2", CurrentSet->nChipType);
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate2", CurrentSet->wTVBaudRate);
		m_Ini.SetProfileInt(GENERAL_S, "TV Link ComPort Parity2", CurrentSet->nTVParity);
	}




/*
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
*/
	m_Ini.SetProfileString(VERSION_CHECK_S, "BE Version",			CurrentSet->sBE_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "FE Version",			CurrentSet->sFE_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Micom Version",		CurrentSet->sMicom_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "DSP Version",			CurrentSet->sDSP_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Sub DSP Version",		CurrentSet->sSubDSP_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "EQ Version",			CurrentSet->sEQ_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "MEQ Version",			CurrentSet->sMEQ_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Touch Version",		CurrentSet->sTouch_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Demo Version",			CurrentSet->sDemo_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "DJ Sound Version",		CurrentSet->sDJSound_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Wireless Tx Version",	CurrentSet->sWirelessTx_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Wireless Rx Version",	CurrentSet->sWirelessRx_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "BT Version", CurrentSet->sBT_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "HDMI Version", CurrentSet->sHDMI_Version);
	m_Ini.SetProfileString(VERSION_CHECK_S, "Checksum",				CurrentSet->sChecksum);
	CString sTemp;
	sTemp = "";
	for (int i = 0; i < 15; i++)
	{
		if (CurrentSet->bVerChecked[i] == 1)
		{
			sTemp += "1";
		}
		else
		{
			sTemp += "0";
		}
	}
	m_Ini.SetProfileString(VERSION_CHECK_S, "Checked Display", sTemp);

	m_Ini.SetProfileInt(TV_CONTROL_S, "Micom Chip Type",CurrentSet->nChipType);
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Source Control",CurrentSet->bAutoSourceControl);
	m_Ini.SetProfileInt(TV_CONTROL_S, "Auto Pattern Control",CurrentSet->bAutoPatternControl);

	//+add psh 100310
	//============
	// HDMI Gen. Control
	//============
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Resolution",CurrentSet->nHDMIResolution);
	m_Ini.SetProfileInt(HDMI_FORMAT_S, "Color Bit Count",CurrentSet->nHDMIBitSpec);


	//+add psh 110226
	//============
	// Remocon Custom Code
	//============
	m_Ini.SetProfileInt(REMOCON_CUSTOM_S, "Remocon Custom Code",CurrentSet->nRemoteCustomCode);
	m_Ini.SetProfileInt(REMOCON_CUSTOM_S, "Remocon Type",CurrentSet->nRemoteType);
	
	return TRUE;
}

BOOL OpenModelIniFile(CString sIniPath)
{
	CFile pFile;
	CFileException FileExcept;
	
	int nNoColors;
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

/*	//=========================
	// Grabber Resolution Info
	//=========================
	CurrentSet->nLvdsHeight   = m_Ini.GetProfileInt(GRAB_S, "LVDS Grabber Height");
	CurrentSet->nLvdsWidth    = m_Ini.GetProfileInt(GRAB_S, "LVDS Grabber Width");
	CurrentSet->nAnalogHeight = m_Ini.GetProfileInt(GRAB_S, "Analog Grabber Height");
	CurrentSet->nAnalogWidth  = m_Ini.GetProfileInt(GRAB_S, "Analog Grabber Width");
	CurrentSet->nUranousMode  = m_Ini.GetProfileInt(GRAB_S, "Uranous Grabber Mode");
	//+add psh 090630
	CurrentSet->bOddDE_Only	  = m_Ini.GetProfileInt(GRAB_S, "LVDS OddDE Only");

//	CurrentSet->nFullHd120Hz  = m_Ini.GetProfileInt(GRAB_S, "FullHD120Hz");
	CurrentSet->nBitCount	  = m_Ini.GetProfileInt(GRAB_S, "Color Bit Count");
*/	
	//+add 090420(Modification No1)
	CurrentSet->nAnalogType  = m_Ini.GetProfileInt(GRAB_S, "Analog Grabber Type");

	nNoColors = m_Ini.GetProfileInt(GRAB_S, "Minimum no of used colors");
	if(nNoColors > 0) CurrentSet->nNoUsedColors = nNoColors;


//	if(CurrentSet->nBitCount == 0) CurrentSet->nBitCount = 24;

	//+add 091214(Modification No1)
//	CurrentSet->nImageRotation = m_Ini.GetProfileInt(GRAB_S, "ImageRotation");
/*	
	//==================
	// Tool/Area Option
	//==================
	CurrentSet->sToolOption1	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option1");
	CurrentSet->sToolOption2	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option2");
	CurrentSet->sToolOption3	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option3");
	CurrentSet->sToolOption4	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option4");
	//091016
	CurrentSet->sToolOption5	= m_Ini.GetProfileString(TOOL_AREA_S, "Tool Option5");

	CurrentSet->sAreaOption1	= m_Ini.GetProfileString(TOOL_AREA_S, "Area Option1");
//	CurrentSet->sAreaOption2	= m_Ini.GetProfileString(TOOL_AREA_S, "Area Option2");
	CurrentSet->sCPUVersion	  = m_Ini.GetProfileString(VERSION_CHECK_S, "CPU Version");
	CurrentSet->sMicomVersion = m_Ini.GetProfileString(VERSION_CHECK_S, "Micom Version");
	CurrentSet->sUSBVersion	  = m_Ini.GetProfileString(VERSION_CHECK_S, "USB Version");

	if(CurrentSet->sToolOption1 == "NULL") CurrentSet->sToolOption1 = "";
	if(CurrentSet->sToolOption2 == "NULL") CurrentSet->sToolOption2 = "";
	if(CurrentSet->sToolOption3 == "NULL") CurrentSet->sToolOption3 = "";
	if(CurrentSet->sToolOption4 == "NULL") CurrentSet->sToolOption4 = "";
	//091016
	if(CurrentSet->sToolOption5 == "NULL") CurrentSet->sToolOption5 = "";

	if(CurrentSet->sAreaOption1 == "NULL") CurrentSet->sAreaOption1 = "";
	if(CurrentSet->sCPUVersion == "NULL") CurrentSet->sCPUVersion = "";
	if(CurrentSet->sMicomVersion == "NULL") CurrentSet->sMicomVersion = "";
	if(CurrentSet->sUSBVersion == "NULL") CurrentSet->sUSBVersion = "";
*/
	//if (g_nRunningProcessNo == 1)
	//{
	//	CurrentSet->bUseTVCommPort = m_Ini.GetProfileInt(GENERAL_S, "Use TVComm Port1");
	//}
	//else
	//{
	//	CurrentSet->bUseTVCommPort = m_Ini.GetProfileInt(GENERAL_S, "Use TVComm Port2");
	//}
	
	



	if (g_nRunningProcessNo == 1)
	{
		//m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number1", CurrentSet->sTVComPort1);//m_Ini.GetProfileString(VERSION_CHECK_S, "USB Version");
		CurrentSet->sTVComPort = m_Ini.GetProfileString(GENERAL_S, "TV Link ComPort Number1");
		if (CurrentSet->sTVComPort.Find("COM") < 0) CurrentSet->sTVComPort = "COM5";
		//CurrentSet->nChipType				= m_Ini.GetProfileInt(TV_CONTROL_S, "Micom Chip Type");
		//if(CurrentSet->nChipType == 0){CurrentSet->nChipType = 0x01;}
		CurrentSet->nChipType = m_Ini.GetProfileInt(TV_CONTROL_S, "Micom Chip Type1");
		if (CurrentSet->nChipType == 0) { CurrentSet->nChipType = 0x01; }



		//CurrentSet->wTVBaudRate = m_Ini.GetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate");
		//if(CurrentSet->wTVBaudRate == 0) CurrentSet->wTVBaudRate = 57600;
		//CurrentSet->nTVParity = m_Ini.GetProfileInt(GENERAL_S, "TV Link ComPort Parity",EVENPARITY);
		CurrentSet->wTVBaudRate = m_Ini.GetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate1");
		if (CurrentSet->wTVBaudRate == 0) CurrentSet->wTVBaudRate = 57600;
		CurrentSet->nTVParity = m_Ini.GetProfileInt(GENERAL_S, "TV Link ComPort Parity1", EVENPARITY);

	}
	else
	{
		//m_Ini.SetProfileString(GENERAL_S, "TV Link ComPort Number2", CurrentSet->sTVComPort2);//m_Ini.GetProfileString(VERSION_CHECK_S, "USB Version");
		CurrentSet->sTVComPort = m_Ini.GetProfileString(GENERAL_S, "TV Link ComPort Number2");
		if (CurrentSet->sTVComPort.Find("COM") < 0) CurrentSet->sTVComPort = "COM6";
		CurrentSet->nChipType = m_Ini.GetProfileInt(TV_CONTROL_S, "Micom Chip Type2");
		if (CurrentSet->nChipType == 0) { CurrentSet->nChipType = 0x01; }
		CurrentSet->wTVBaudRate = m_Ini.GetProfileInt(GENERAL_S, "TV Link ComPort Baud Rate2");
		if (CurrentSet->wTVBaudRate == 0) CurrentSet->wTVBaudRate = 57600;
		CurrentSet->nTVParity = m_Ini.GetProfileInt(GENERAL_S, "TV Link ComPort Parity2", EVENPARITY);
	}
	//if (g_nRunningProcessNo == 1)
	//{
	//	//CurrentSet->bUseTVCommPort = CurrentSet->bUseTVCommPort1;

	//	CurrentSet->sTVComPort = CurrentSet->sTVComPort1;
	//	CurrentSet->nChipType = CurrentSet->nChipType1;		
	//	CurrentSet->wTVBaudRate =  CurrentSet->wTVBaudRate1;
	//	CurrentSet->nTVParity = CurrentSet->nTVParity1;



	//}
	//else
	//{
	//	//CurrentSet->bUseTVCommPort = CurrentSet->bUseTVCommPort2;
	//	CurrentSet->sTVComPort = CurrentSet->sTVComPort2;
	//	CurrentSet->sTVComPort = CurrentSet->sTVComPort2;
	//	CurrentSet->nChipType = CurrentSet->nChipType2;
	//	CurrentSet->wTVBaudRate = CurrentSet->wTVBaudRate2;
	//	CurrentSet->nTVParity = CurrentSet->nTVParity2;

	//}
	/*
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
*/
	CurrentSet->sBE_Version			= m_Ini.GetProfileString(VERSION_CHECK_S, "BE Version");
	CurrentSet->sFE_Version			= m_Ini.GetProfileString(VERSION_CHECK_S, "FE Version");
	CurrentSet->sMicom_Version		= m_Ini.GetProfileString(VERSION_CHECK_S, "Micom Version");
	CurrentSet->sDSP_Version		= m_Ini.GetProfileString(VERSION_CHECK_S, "DSP Version");
	CurrentSet->sSubDSP_Version		= m_Ini.GetProfileString(VERSION_CHECK_S, "Sub DSP Version");
	CurrentSet->sEQ_Version			= m_Ini.GetProfileString(VERSION_CHECK_S, "EQ Version");
	CurrentSet->sMEQ_Version		= m_Ini.GetProfileString(VERSION_CHECK_S, "MEQ Version");
	CurrentSet->sTouch_Version		= m_Ini.GetProfileString(VERSION_CHECK_S, "Touch Version");
	CurrentSet->sDemo_Version		= m_Ini.GetProfileString(VERSION_CHECK_S, "Demo Version");
	CurrentSet->sDJSound_Version	= m_Ini.GetProfileString(VERSION_CHECK_S, "DJ Sound Version");
	CurrentSet->sWirelessTx_Version	= m_Ini.GetProfileString(VERSION_CHECK_S, "Wireless Tx Version");
	CurrentSet->sWirelessRx_Version = m_Ini.GetProfileString(VERSION_CHECK_S, "Wireless Rx Version");
	CurrentSet->sBT_Version = m_Ini.GetProfileString(VERSION_CHECK_S, "BT Version");
	CurrentSet->sHDMI_Version = m_Ini.GetProfileString(VERSION_CHECK_S, "HDMI Version");
	CurrentSet->sChecksum			= m_Ini.GetProfileString(VERSION_CHECK_S, "Checksum");

	//CString sTemp;
	//sTemp = m_Ini.GetProfileString(VERSION_CHECK_S, "Checked Display");	
	//for (int i = 0; i < 15; i++)
	//{
	//	if (sTemp.GetLength() <= i)
	//	{
	//		CurrentSet->bVerChecked[i] = 0;
	//	}
	//	else if (sTemp.GetAt(i) == '1')
	//	{
	//		CurrentSet->bVerChecked[i] = 1;		
	//	}
	//	else
	//	{
	//		CurrentSet->bVerChecked[i] = 0;
	//	}
	//}


	if(CurrentSet->sBE_Version		   == "NULL") CurrentSet->sBE_Version		  = "";
	if(CurrentSet->sFE_Version		   == "NULL") CurrentSet->sFE_Version		  = "";
	if(CurrentSet->sMicom_Version	   == "NULL") CurrentSet->sMicom_Version	  = "";
	if(CurrentSet->sDSP_Version		   == "NULL") CurrentSet->sDSP_Version		  = "";
	if(CurrentSet->sSubDSP_Version	   == "NULL") CurrentSet->sSubDSP_Version	  = "";
	if(CurrentSet->sEQ_Version		   == "NULL") CurrentSet->sEQ_Version	      = "";
	if(CurrentSet->sMEQ_Version		   == "NULL") CurrentSet->sMEQ_Version	      = "";
	if(CurrentSet->sTouch_Version	   == "NULL") CurrentSet->sTouch_Version      = "";
	if(CurrentSet->sDemo_Version	   == "NULL") CurrentSet->sDemo_Version	      = "";
	if(CurrentSet->sDJSound_Version	   == "NULL") CurrentSet->sDJSound_Version    = "";
	if(CurrentSet->sWirelessTx_Version == "NULL") CurrentSet->sWirelessTx_Version = "";
	if (CurrentSet->sWirelessRx_Version == "NULL") CurrentSet->sWirelessRx_Version = "";
	if (CurrentSet->sBT_Version == "NULL") CurrentSet->sBT_Version = "";
	if (CurrentSet->sHDMI_Version == "NULL") CurrentSet->sHDMI_Version = "";
	if(CurrentSet->sChecksum		   == "NULL") CurrentSet->sChecksum	          = "";

	CString sTemp;
	sTemp = m_Ini.GetProfileString(VERSION_CHECK_S, "Checked Display");
	if (sTemp == "NULL")
	{
		
		for (int i = 0; i < 15; i++)
		{
			CurrentSet->bVerChecked[i] = 1;
		}
			
		if (CurrentSet->sBE_Version == "")			CurrentSet->bVerChecked[0] = 0;
		if (CurrentSet->sFE_Version == "")			CurrentSet->bVerChecked[1] = 0;
		if (CurrentSet->sMicom_Version == "")		CurrentSet->bVerChecked[2] = 0;
		if (CurrentSet->sDSP_Version == "")			CurrentSet->bVerChecked[3] = 0;
		if (CurrentSet->sSubDSP_Version == "")		CurrentSet->bVerChecked[4] = 0;
		if (CurrentSet->sEQ_Version == "")			CurrentSet->bVerChecked[5] = 0;
		if (CurrentSet->sMEQ_Version == "")			CurrentSet->bVerChecked[6] = 0;
		if (CurrentSet->sTouch_Version == "")		CurrentSet->bVerChecked[7] = 0;
		if (CurrentSet->sDemo_Version == "")		CurrentSet->bVerChecked[8] = 0;
		if (CurrentSet->sDJSound_Version == "")		CurrentSet->bVerChecked[9] = 0;
		if (CurrentSet->sWirelessTx_Version == "")	CurrentSet->bVerChecked[10] = 0;
		if (CurrentSet->sWirelessRx_Version == "")	CurrentSet->bVerChecked[11] = 0;
		if (CurrentSet->sBT_Version == "")			CurrentSet->bVerChecked[12] = 0;
		if (CurrentSet->sHDMI_Version == "")		CurrentSet->bVerChecked[13] = 0;
		if (CurrentSet->sChecksum == "")			CurrentSet->bVerChecked[14] = 0;

		
	}
	else
	{
		for (int i = 0; i < 15; i++)
		{
			if (sTemp.GetLength() <= i)
			{
				CurrentSet->bVerChecked[i] = 0;
			}
			else if (sTemp.GetAt(i) == '1')
			{
				CurrentSet->bVerChecked[i] = 1;
			}
			else
			{
				CurrentSet->bVerChecked[i] = 0;
			}
		}
	}


	CurrentSet->bAutoSourceControl		= m_Ini.GetProfileInt(TV_CONTROL_S, "Auto Source Control");
	CurrentSet->bAutoPatternControl		= m_Ini.GetProfileInt(TV_CONTROL_S, "Auto Pattern Control");
	
	//+add psh 100310
	//============
	// HDMI Gen. Control
	//============
	CurrentSet->nHDMIResolution 		= m_Ini.GetProfileInt(HDMI_FORMAT_S, "Resolution");
	CurrentSet->nHDMIBitSpec			= m_Ini.GetProfileInt(HDMI_FORMAT_S, "Color Bit Count");

	//+add psh 110226
	//============
	// Remocon Custom Code
	//============
	CurrentSet->nRemoteCustomCode		= m_Ini.GetProfileInt(REMOCON_CUSTOM_S, "Remocon Custom Code");
	CurrentSet->nRemoteType				= m_Ini.GetProfileInt(REMOCON_CUSTOM_S, "Remocon Type");
	if((CurrentSet->nRemoteType < 0) || (CurrentSet->nRemoteType >1))
	{
		CurrentSet->nRemoteType = 0;
	}

	return TRUE;
}

BOOL GetBmpSize(CString sBmpPath, CPoint &pSize)
{
	BITMAPFILEHEADER FileHeader;   // Bitmap File Header
	BITMAPINFOHEADER InfoHeader;   // Bitmap Info Header
	FILE*	 pFile = NULL;
	
	// Open Image File
	if(!(pFile = fopen((LPCTSTR)sBmpPath,"rb")))
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

/*
//+ 2007.5.29 Add BY USY
void InitMainGrid(int nGridType)
{
	CString sTmp;

	char *sHeader1[] = {"", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT"};
	char *sHeader2[] = {"", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE"};

	g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	g_pView->m_ctrlSummaryGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	g_pView->m_ctrlSummaryGrid.SetBackColorFixed(RGB(210,216,176));
	g_pView->m_ctrlSummaryGrid.SetForeColorFixed(RGB(51,102,153));
	g_pView->m_ctrlSummaryGrid.SetBackColor(RGB(247,247,231));
//	g_pView->m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
	g_pView->m_ctrlSummaryGrid.SetGridColor(RGB(200,200,176));

	g_pView->m_ctrlSummaryGrid.SetFixedRows(0);
	g_pView->m_ctrlSummaryGrid.SetFixedCols(0);
	g_pView->m_ctrlSummaryGrid.SetFrozenRows(1);

	g_pView->m_ctrlSummaryGrid.SetFillStyle(flexFillRepeat);
	g_pView->m_ctrlSummaryGrid.SetFontName("Arial");
//	g_pView->m_ctrlSummaryGrid.SetFontSize(10);
	g_pView->m_ctrlSummaryGrid.SetFontSize(9);

	if(nGridType == DETAILEDGRID) 
	{
		g_pView->m_ctrlSummaryGrid.SetCols(13);
		g_pView->m_ctrlSummaryGrid.SetRows(1);
		g_pView->m_ctrlSummaryGrid.SetExtendLastCol(TRUE);
//		g_pView->m_ctrlSummaryGrid.SetFrozenCols(3);	
		g_pView->m_ctrlSummaryGrid.SetFrozenCols(2);	

		// Set Column Alignment & Column Title
		for(int nCol = 0; nCol < 13 ; nCol++)
		{
			g_pView->m_ctrlSummaryGrid.SetRow(0);
			g_pView->m_ctrlSummaryGrid.SetCol(nCol);
		
			g_pView->m_ctrlSummaryGrid.SetText(sHeader2[nCol]);
		
			switch(nCol)
			{
				case 0 : // Check
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 300); //300
						 g_pView->m_ctrlSummaryGrid.SetColDataType(nCol, flexDTBoolean);
						 g_pView->m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
				  		 break;

				case 1 : // No
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 400); //500
				  		 break;

				case 2 : // Step
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignLeftCenter);
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 2400);//3100
				  		 break;

				case 3 : // Result
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 900);
				  		 break;

				case 4 : // Measure
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 700);
				  		 break;

				case 5 : // Target
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 900);
				  		 break;

				case 6 : // L-Limit
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 750);
				  		 break;

				case 7 : // U-Limit
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 750);
				  		 break;

				case 8 : // Unit
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 650);
				  		 break;
				
				case 9 : // Audio Target
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 2000);
				  		 break;
				case 10 : // Audio Measure
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 2000);
				  		 break;
				case 11 : // Elapsed Time
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 760);
				  		 break;

				case 12 : // Message
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nCol, 5000);
				  		 break;
			}
			g_pView->m_ctrlSummaryGrid.SetFixedAlignment(nCol,flexAlignCenterCenter);
		}

	}


	g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
	g_pView->m_ctrlSummaryGrid.Refresh();
}

void InsertStepData2Grid(int nGridType)
{
	g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	g_pView->m_ctrlSummaryGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	//+ 2007.10.10 Add BY USY
	char *sHeader1[] = {"", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT"};
	char *sHeader2[] = {"", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE"};
	//-

	//Row 
	if(CurrentSet->bCompiled)
	{
		if( nGridType == DETAILEDGRID)
		{
			if(StepList.GetCount() + 1 < 12) g_pView->m_ctrlSummaryGrid.SetRows(12);
			else g_pView->m_ctrlSummaryGrid.SetRows(StepList.GetCount() + 2);
		}
	}
	else g_pView->m_ctrlSummaryGrid.SetRows(12);

	//+Insert Step Default Data
	POSITION Position;
	CStep* pStep;
	CString sTmp;

	Position = StepList.GetHeadPosition();

	if(nGridType == DETAILEDGRID)
	{
		g_pView->m_ctrlSummaryGrid.SetRow(0);
//		g_pView->m_ctrlSummaryGrid.SetFrozenCols(3);
		g_pView->m_ctrlSummaryGrid.SetFrozenCols(2);

		for(int nTmp = 0; nTmp < 13 ; nTmp++)
		{
			g_pView->m_ctrlSummaryGrid.SetCol(nTmp);
			g_pView->m_ctrlSummaryGrid.SetText(sHeader2[nTmp]);
			g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(210,216,176));
			g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(51,102,153));
		
			switch(nTmp)
			{
				case 0 : // Check
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);		
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 300); //300
						 g_pView->m_ctrlSummaryGrid.SetColDataType(nTmp, flexDTBoolean);
						 g_pView->m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
				  		 break;

				case 1 : // No
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);		
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 400);  //500
				  		 break;

				case 2 : // Step
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignLeftCenter);
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 2400); //3100
				  		 break;

				case 3 : // Result
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
				  		 break;

				case 4 : // Measure
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 700);
				  		 break;

				case 5 : // Target
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
				  		 break;

				case 6 : // L-Limit
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
				  		 break;

				case 7 : // U-Limit
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
				  		 break;

				case 8 : // Unit
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 650);
				  		 break;
				
				case 9 : // Audio Target
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
				  		 break;
				case 10 : // Audio Measure
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
				  		 break;
				case 11 : // Elapsed Time
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 760);
				  		 break;

				case 12 : // Message
						 g_pView->m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 g_pView->m_ctrlSummaryGrid.SetColWidth(nTmp, 5000);
				  		 break;
			}
		}

		if(StepList.GetCount() > 0)
		{
			nTmp = 1;

			while (Position) 
			{
				pStep = StepList.GetNext(Position);

				g_pView->m_ctrlSummaryGrid.SetRow(nTmp);

				//(Column 0) Step Check 
				g_pView->m_ctrlSummaryGrid.SetCol(0);
				g_pView->m_ctrlSummaryGrid.SetColDataType(0, flexDTBoolean);

				if(pStep->GetTest() == TRUE) g_pView->m_ctrlSummaryGrid.SetCellChecked(flexChecked);
				else g_pView->m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);

				//(Column 1) Step No
				sTmp.Format("%d", pStep->GetStepNo());
				g_pView->m_ctrlSummaryGrid.SetCol(1);
				g_pView->m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
				g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 1, sTmp);
			
				//(Column 2) Step Name
				g_pView->m_ctrlSummaryGrid.SetCol(2);
				g_pView->m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
				if(pStep->m_nTestType == ADC_TEST || pStep->m_nTestType == TV_COMM_TEST)
				{
					g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(189,231,97));
					//g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(	255,	 255,   255 ));
				}
				g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 2, pStep->GetStepName());
				

				//(Column 4) Measure
				g_pView->m_ctrlSummaryGrid.SetCol(4);
				g_pView->m_ctrlSummaryGrid.SetColDataType(4, flexDTString);

				if(pStep->m_nStepType == CHECK_FUNCTION)
				{ 
					if(pStep->GetFuncType() == MEAS_BOOL)
					{						
						if(pStep->GetNominal_Bool() == FALSE) sTmp.Format(_T("%s"), "FALSE");
						else sTmp.Format(_T("%s"), "TRUE");
					}
					else sTmp.Format("%4.1f", pStep->GetNominal_float());

					// (Column 5) Target
					g_pView->m_ctrlSummaryGrid.SetCol(5);
					g_pView->m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
					g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 5, sTmp);

					if(pStep->GetFuncType() != MEAS_BOOL)
					{
						// (Column 6) Low Limit
						sTmp.Format("%4.1f", pStep->GetLowLimit());
						g_pView->m_ctrlSummaryGrid.SetCol(6);
						g_pView->m_ctrlSummaryGrid.SetColDataType(6, flexDTString);
						g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 6, sTmp);

						// (Column 7) High Limit
						sTmp.Format("%4.1f", pStep->GetHighLimit());
						g_pView->m_ctrlSummaryGrid.SetCol(7);
						g_pView->m_ctrlSummaryGrid.SetColDataType(7, flexDTString);
						g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 7, sTmp);

						// (Column 8) Unit
						sTmp.Format(_T("%s"), pStep->GetUnit());
						g_pView->m_ctrlSummaryGrid.SetCol(8);
						g_pView->m_ctrlSummaryGrid.SetColDataType(8, flexDTString);
						g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 8, sTmp);
						

					}
					//+2007.10.15 Add BY USY
					if(pStep->m_bRunAudioTest == TRUE)
					{
						// (Column 9) Audio Target
						// change 090323
						if(pStep->m_nMeasureAudioType == LEVEL_CHECK){
							sTmp.Format(_T("X, X, %d, %d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
						}
						else if(pStep->m_nMeasureAudioType == FREQUENCY_CHECK){
							sTmp.Format(_T("%d, %d, X, X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
						}
						else {
							sTmp.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
						}

						g_pView->m_ctrlSummaryGrid.SetCol(9);
						g_pView->m_ctrlSummaryGrid.SetColDataType(9, flexDTString);
						g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 9, sTmp);
					}
					//-
				}
				nTmp++;
			}
			//Result Row
;
			//(Column 1) Step No
			//sTmp.Format("%d", StepList.GetCount() + 1);
			g_pView->m_ctrlSummaryGrid.SetCol(1);
			g_pView->m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 1, _T(""));
		
			//(Column 2) Step Name
			g_pView->m_ctrlSummaryGrid.SetCol(2);
			g_pView->m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nTmp , 2, "Total");

			g_pView->m_ctrlSummaryGrid.SetRow(0);
			g_pView->m_ctrlSummaryGrid.SetCol(0);
			g_pView->m_ctrlSummaryGrid.SetCellChecked(flexChecked); 
			//+ 2007.10.10 Add BY USY
			g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(247,247,231));
//			g_pView->m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
			g_pView->m_ctrlSummaryGrid.SetGridColor(RGB(200,200,176));
			//-
		}
	}
	g_pView->m_ctrlSummaryGrid.SetCol(-1);
	g_pView->m_ctrlSummaryGrid.SetRow(-1);
	g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
	g_pView->m_ctrlSummaryGrid.Refresh();
	return;
}

void InsertResultData2Grid(int nGridType, int nStepNo)
{
	if(nGridType == DETAILEDGRID) InsertResult2DetailedGrid(nStepNo);
} 

void InsertResult2NormalGrid(int nStepNo)
{
	if((nStepNo > (g_pView->m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

	CString sTmp;

	int nRow, nCol; // nCol = 3 or 7 or 11
	nRow = ((nStepNo - 1) / 3) + 1;

	g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	g_pView->m_ctrlSummaryGrid.SetRow(nRow);
	if(nRow % 20 == 0) g_pView->m_ctrlSummaryGrid.SetTopRow(nRow);

	if(nStepNo % 3 == 1) nCol = 3;
	else if(nStepNo % 3 == 2) nCol = 7;
	else if(nStepNo % 3 == 0) nCol = 11;

	g_pView->m_ctrlSummaryGrid.SetRow(nRow);
	g_pView->m_ctrlSummaryGrid.SetCol(nCol);
	g_pView->m_ctrlSummaryGrid.SetColDataType(nCol, flexDTString);
	
	if(pCurStep->m_bTest == TRUE)
	{
		if (pCurStep->m_bResult)
		{
			sTmp = _T("OK");
			g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(0,0,255));
			g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
			
		}
		else // Video Test NG
		{
			//090324
			sTmp = _T("NG");
			if(pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{

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
				if(pCurStep->m_bVideoTestResult == FALSE)
				{
					sTmp = _T("NG/V");
				}
			}
			else if(pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
			{
				if(pCurStep->m_bAudioTestResult == FALSE)
				{
					sTmp = _T("NG/A");
				}
			}
			else
			{
				sTmp = _T("NG");
			}
				
			g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
			g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
		}
	}
	else if(pCurStep->m_bTest == FALSE)
	{
		sTmp = _T("SKIP");
		g_pView->m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
	}
	
	//+2007.10.15 Add BY USY
	g_pView->m_ctrlSummaryGrid.SetRow(-1);
	g_pView->m_ctrlSummaryGrid.SetCol(-1);
	//-
	g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
	g_pView->m_ctrlSummaryGrid.Refresh();
	return ;
}
*/
/*
void InsertResult2DetailedGrid(int nStepNo)
{
	if((nStepNo > (g_pView->m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

    CString sTmp;
	g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	g_pView->m_ctrlSummaryGrid.SetRow(nStepNo);

	if(nStepNo % 7 == 0) g_pView->m_ctrlSummaryGrid.SetTopRow(nStepNo);
		
	// (Column 11) Elapsed Time
    if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);

		g_pView->m_ctrlSummaryGrid.SetCol(11);
		g_pView->m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
		g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);
		
		g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
		g_pView->m_ctrlSummaryGrid.Refresh();

		return;
	}

	//===============
	// Test Function
	//===============
	if(pCurStep->m_bTest) // == TRUE)
	{
		//===================
		// (Column 3) Result
		//===================
	    if (pCurStep->m_bResult) // PASS
		{
			sTmp = _T("OK");
			g_pView->m_ctrlSummaryGrid.SetCol(3);
			g_pView->m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
			g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(0,0,255));
			g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);
		}
		// NG
		else
		{
			//090324
			sTmp = _T("NG");
			if(pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{

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
				if(pCurStep->m_bVideoTestResult == FALSE)
				{
					sTmp = _T("NG/V");
				}
			}
			else if(pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
			{
				if(pCurStep->m_bAudioTestResult == FALSE)
				{
					sTmp = _T("NG/A");
				}
			}
			else
			{
				sTmp = _T("NG");
			}

			g_pView->m_ctrlSummaryGrid.SetCol(3);
			g_pView->m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
			g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
			g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);
		}
		if(pCurStep->m_bRunAudioTest)
		{
			sTmp.Format(_T("%d, %d, %d, %d"), pCurStep->m_nLeftFreq[1], pCurStep->m_nRightFreq[1], pCurStep->m_nLeftLevel[1], pCurStep->m_nRightLevel[1]);

			g_pView->m_ctrlSummaryGrid.SetCol(10);
			g_pView->m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
		}
		
		//+add kwmoon 081013
		if(pCurStep->m_bAdcValueCheckStep)
		{
			//========================================================
			// (Column 10) Display ADC value instead of Audio Measure
			//========================================================
			sTmp = pCurStep->m_szAdcValue;
			g_pView->m_ctrlSummaryGrid.SetCol(10);
			g_pView->m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
		}
	}
	//======
	// SKIP
	//======
	else // if(pCurStep->m_bTest == FALSE)
	{
		sTmp = _T("SKIP");
		g_pView->m_ctrlSummaryGrid.SetCol(3);
		g_pView->m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
		g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);
		g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
		g_pView->m_ctrlSummaryGrid.Refresh();
		return;
	}

	// (Column 4) Measure
	if(pCurStep->m_nFuncType == MEAS_BOOL)
	{	
		if(pCurStep->m_bMeasured == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
    }
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fMeasured);

	g_pView->m_ctrlSummaryGrid.SetCol(4);
	g_pView->m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, sTmp);

	// (Column 5) Target
	if(pCurStep->m_nFuncType == MEAS_BOOL)
	{	
		if(pCurStep->m_bNominal == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
    }
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fNominal);

	g_pView->m_ctrlSummaryGrid.SetCol(5);
	g_pView->m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);

							

	// (Column 11) Elapsed Time
	sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);
	g_pView->m_ctrlSummaryGrid.SetCol(11);
	g_pView->m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);

	g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
	g_pView->m_ctrlSummaryGrid.Refresh();
	return ;
}

void InsertMsg2DetailedGrid(int nStepNo, CString sMsg)
{
	CString szDetailMessage = sMsg;

	if((nStepNo > (g_pView->m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

	g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	g_pView->m_ctrlSummaryGrid.SetRow(nStepNo);
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 12, szDetailMessage);
	
	g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
	g_pView->m_ctrlSummaryGrid.Refresh();
}

void InsertTotalData2Grid(int nGridType, BOOL bResult, CString sElapsedTime)
{
	if(nGridType == DETAILEDGRID)
	{
		if(StepList.GetCount() > 0)
		{
			CString sTmp;
			g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
			g_pView->m_ctrlSummaryGrid.SetCol(3);
			g_pView->m_ctrlSummaryGrid.SetRow(StepList.GetCount() + 1);
			if(bResult == TRUE)
			{
				g_pView->m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(0,0,255));
				g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
				g_pView->m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "OK");
			}
			else if(bResult == FALSE)
			{
				
				g_pView->m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
				g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
				g_pView->m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "NG");
			}


			g_pView->m_ctrlSummaryGrid.SetCol(11);
			g_pView->m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
			g_pView->m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 11, sElapsedTime);
			
			g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
			g_pView->m_ctrlSummaryGrid.Refresh();
		}
	}
}

//-

void DeleteResultDataFromGrid(int nGridType, int nStepNo)
{
	if(nGridType == DETAILEDGRID) DeleteResultDataFromDetailedGrid(nStepNo);
}
void DeleteResultDataFromNormalGrid(int nStepNo)
{
	if((nStepNo > (g_pView->m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

	CString sTmp;

	int nRow, nCol; // nCol = 3 or 7 or 11
	nRow = ((nStepNo - 1) / 3) + 1;

	g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	g_pView->m_ctrlSummaryGrid.SetRow(nRow);
	if(nRow % 20 == 0) g_pView->m_ctrlSummaryGrid.SetTopRow(nRow);

	if(nStepNo % 3 == 1) nCol = 3;
	else if(nStepNo % 3 == 2) nCol = 7;
	else if(nStepNo % 3 == 0) nCol = 11;

	g_pView->m_ctrlSummaryGrid.SetRow(nRow);
	g_pView->m_ctrlSummaryGrid.SetCol(nCol);
	g_pView->m_ctrlSummaryGrid.SetColDataType(nCol, flexDTString);
	
	g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(255,255,255));
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, _T(""));

	g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
	g_pView->m_ctrlSummaryGrid.Refresh();
	return ;
}
void DeleteResultDataFromDetailedGrid(int nStepNo)
{
		if((nStepNo > (g_pView->m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

    CString sTmp;
	g_pView->m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	g_pView->m_ctrlSummaryGrid.SetRow(nStepNo);

	if(nStepNo % 7 == 0) g_pView->m_ctrlSummaryGrid.SetTopRow(nStepNo);
		
	// (Column 11) Elapsed Time
    if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		g_pView->m_ctrlSummaryGrid.SetCol(11);
		g_pView->m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
		g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));
		
		g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
		g_pView->m_ctrlSummaryGrid.Refresh();

		return;
	}

	// (Column 3) Result
	g_pView->m_ctrlSummaryGrid.SetCol(3);
	g_pView->m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
	g_pView->m_ctrlSummaryGrid.SetCellBackColor(RGB(255,255,255));
	g_pView->m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, _T(""));
	   
	if(pCurStep->m_bRunAudioTest)
	{
		// (Column 10) Audio Measure
		g_pView->m_ctrlSummaryGrid.SetCol(10);
		g_pView->m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
		g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, _T(""));

	}

	// (Column 4) Measure
	g_pView->m_ctrlSummaryGrid.SetCol(4);
	g_pView->m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, _T(""));

	g_pView->m_ctrlSummaryGrid.SetCol(5);
	g_pView->m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);


	// (Column 11) Elapsed Time
	g_pView->m_ctrlSummaryGrid.SetCol(11);
	g_pView->m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	g_pView->m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));

	g_pView->m_ctrlSummaryGrid.SetRow(0);
	g_pView->m_ctrlSummaryGrid.SetCol(0);
	g_pView->m_ctrlSummaryGrid.SetRedraw(-1);
	g_pView->m_ctrlSummaryGrid.Refresh();
	return ;
}

BOOL GetResultFromDetailedGrid(int nStepNo,CString& sMsg)
{
	if((nStepNo > (g_pView->m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return FALSE;
	
	sMsg = g_pView->m_ctrlSummaryGrid.GetTextMatrix(nStepNo, 12);

	return TRUE;
}
*/
/*
void GetCheckStep(int nGridType)
{
	POSITION Position;
	CStep* pStep;
	int nRow, nCol, nTmp;

	Position = StepList.GetHeadPosition();
	if(StepList.GetCount() > 0)
	{
		nTmp = 1;
		nRow = 1;
		while (Position) 
		{
			pStep = StepList.GetNext(Position);
			
			if(nGridType == DETAILEDGRID)
			{
				nCol = 0;
			}

			g_pView->m_ctrlSummaryGrid.SetRow(nRow);
			g_pView->m_ctrlSummaryGrid.SetCol(nCol);
			if(g_pView->m_ctrlSummaryGrid.GetCellChecked() == flexChecked)
			{
				pStep->SetTest(TRUE);
			}
			else pStep->SetTest(FALSE);

			nRow++;
			nTmp++;
		
		}
	}
}

//+add kwmoon 080229
int GetNoCheckedStep(int nGridType)
{
	POSITION Position;
	CStep* pStep;
	
	int nRow	= 0;
	int nCol	= 0;
	int nNoCheckedStep = 0;

	Position = StepList.GetHeadPosition();

	if(StepList.GetCount() > 0)
	{
		nRow = 1;

		while (Position) 
		{
			pStep = StepList.GetNext(Position);
			
			if(nGridType == DETAILEDGRID)
			{
				nCol = 0;
			}

			g_pView->m_ctrlSummaryGrid.SetRow(nRow);
			g_pView->m_ctrlSummaryGrid.SetCol(nCol);

			if(g_pView->m_ctrlSummaryGrid.GetCellChecked() == flexChecked)
			{
				nNoCheckedStep++;
			}
			nRow++;
		}
	}
	return nNoCheckedStep;
}
*/
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
	
	for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
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
	
	for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
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
		
		for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
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
		
		for(int i=0; i<MAX_NO_COLOR_BITS-2; i++)
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
//	if(!pFile.Open(sRemotePath, CFile::modeRead | CFile::modeNoTruncate |CFile::shareExclusive   , &FileExcept)) 
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
		
		//Custom  - add 20110322
		sTmp = sReadString.Left(sReadString.Find(",", 0));
		sReadString.Delete(0, sReadString.Find(",", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		sTmp.MakeUpper();
		pRemote->sCustomCode = sTmp;

		//Code
		sTmp = sReadString.Left(sReadString.Find(",", 0));
		sReadString.Delete(0, sReadString.Find(",", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		sTmp.MakeUpper();
		pRemote->sCode = sTmp;

		//Use
		sTmp = sReadString.Left(sReadString.Find(",", 0));
		sReadString.Delete(0, sReadString.Find(",", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		pRemote->nByte = ((atoi)(sTmp));

		//Name
		sTmp = sReadString.Left(sReadString.Find(",", 0));
		sReadString.Delete(0, sReadString.Find(",", 0) + 1);
		sTmp.TrimRight(); sTmp.TrimLeft();
		sTmp.MakeUpper();
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
UINT hex2dec(const char ccHex)
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
UINT hexCstr2decNum(const CString csHex)
{
	return hexStr2DecNum((LPSTR)(LPCTSTR)csHex);
}

// Convert hex string to Decimal number
UINT hexStr2DecNum( char* cpcHex)
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
	szCmdString.Format("%c",START_TERMINAL_MODE);
	
	if(HDMIGeneratorCtrl.m_bPortOpen == FALSE)
	{
		if(HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			HDMIGeneratorCtrl.PortClose(); _Wait(100);
			if(!HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sHDMIComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if(HDMIGeneratorCtrl.m_bPortOpen == TRUE)
	{
		HDMIGeneratorCtrl.PortClose(); _Wait(100);
		if(!HDMIGeneratorCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sHDMIComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}

	// Send On-Line Mode On Cmd
	HDMIGeneratorCtrl.SendCommString(szCmdString); 
	HDMIGeneratorCtrl.m_nRemoteMode = ONLINE;

	return TRUE;
}

//add 20100622
BOOL InitThermometer(CString sComPort, DWORD wBaudRate)
{
	 if(gThermometerCtrl.m_bPortOpen)
	{
		gThermometerCtrl.CloseComm(); _Wait(100);
	 }
	
	if(gThermometerCtrl.CreateComm(sComPort, wBaudRate) == FALSE) 
	{
		gThermometerCtrl.CloseComm (); _Wait(100);
		if(!gThermometerCtrl.CreateComm(sComPort, wBaudRate) == FALSE)
		{
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sThComPort);
			AfxMessageBox(szErrMsg);
			return FALSE;
		}
	}

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
BOOL InitIrChkCommPort(CString sComPort, DWORD wBaudRate)
{
	if(gIrCheckerCtrl.m_bPortOpen == FALSE)
	{
		if(gIrCheckerCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			//+add kwmoon 071015 : [2in1] : _Wait(100)
			gIrCheckerCtrl.PortClose(); _Wait(100);

			if(!gIrCheckerCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sIrChkComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if(gIrCheckerCtrl.m_bPortOpen == TRUE)
	{
		//+add kwmoon 071015 : [2in1] : _Wait(100)
		gIrCheckerCtrl.PortClose(); _Wait(100);

		if(!gIrCheckerCtrl.Create(sComPort, wBaudRate) == FALSE)
		{
			//+add kwmoon 080313
			CString szErrMsg;
			szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sIrChkComPort);
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

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sCmd.Format("Set Audio CH 1 %s",  AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

/*	for(int nTmp = 1; nTmp <= 8; nTmp++)
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
*/
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nAvSwitchBoxInitVideoCh, 0))
	{
		sCmd.Format("Set VIDEO CH 1 %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sCmd.Format("Set Remote Type 1 %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}

/*	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_REMOTE_REPEAT_NO, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sCmd.Format("Set Remote Repeat No %s!", AvSwitchBoxCtrl.m_strErrMsg);
		AfxMessageBox(sCmd); return FALSE;
	}
*/
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
	g_pView->m_bTimeDataFileOpen = FALSE;

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

	//+change kwmoon 080910
//	if(!g_pView->m_pTimeDataFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite |CFile::shareExclusive , &FileExcept)) 
	if(!g_pView->m_pTimeDataFile.Open(sFilePath, CFile::modeCreate | CFile::modeWrite |CFile::shareDenyNone , &FileExcept)) 
	{
		char szCause[255];
		FileExcept.GetErrorMessage(szCause, 255);
		AfxMessageBox(szCause);
		return FALSE;
	}

	g_pView->m_pTimeDataFile.SeekToBegin();

	g_pView->m_bTimeDataFileOpen = TRUE;

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

		for(int i=0; i<3; i++)
		{
			memset(cData[i],NULL,10);
			memset(DifferentPointInfo[i],NULL,500);
		}

		for(int i=0; i<NO_DIFFERENT_POINT_BLOCK;i++)
		{
			itoa(pCurStep->m_aNoDifferentPoint[R_DATA][i],cData[R_DATA],10);
			itoa(pCurStep->m_aNoDifferentPoint[G_DATA][i],cData[G_DATA],10);
			itoa(pCurStep->m_aNoDifferentPoint[B_DATA][i],cData[B_DATA],10);

			strcat(DifferentPointInfo[R_DATA],cData[R_DATA]);
			strcat(DifferentPointInfo[G_DATA],cData[G_DATA]);
			strcat(DifferentPointInfo[B_DATA],cData[B_DATA]);
			
			if(i != (NO_DIFFERENT_POINT_BLOCK-1))
			{
				strcat(DifferentPointInfo[R_DATA],",");
				strcat(DifferentPointInfo[G_DATA],",");
				strcat(DifferentPointInfo[B_DATA],",");
			}
		}


		szOutputString.Format("%d,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d_%d,%d_%d,%d_%d,%d_%d,%d_%d,%s\r\n",
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

	//=============================================================
	// Set Audio Spec for Each Input Source (Used in MeasureAudio)
	//=============================================================
	for(int i = 0;InputDefined[i].pszName != NULL; i++)
	{
		sTmp.Format("%d,%d,%d,%d,%d",
					CurrentSet->nAudioSpec[i][0],CurrentSet->nAudioSpec[i][1],
					CurrentSet->nAudioSpec[i][2],CurrentSet->nAudioSpec[i][3],
					CurrentSet->nAudioSpec[i][4]);
	
		m_Ini.SetProfileString(AUDIO_SPEC_S, InputDefined[i].pszName, sTmp);
	}

	//+add kwmoon 080904
	//=======================================
	// Set Audio Margin (Used in CheckAudio)
	//=======================================
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Level Margin",CurrentSet->nLevelMargin);
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Freq Margin",CurrentSet->nFreqMargin);

	//+move kwmoon 080904
	//=======================================
	// Set Image Margin (Used in SPEC-ADJ)
	//=======================================
	//del psh 090519
	/*
	for(i = 0;InputDefined[i].pszName != NULL; i++)
	{
		m_Ini.SetProfileInt(SRCMARGIN_S, InputDefined[i].pszName, CurrentSet->nSrcMargin[i]);
	}
	*/
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

	//=============================================================
	// Set Audio Spec for Each Input Source (Used in MeasureAudio)
	//=============================================================
	for(int i = 0;InputDefined[i].pszName != NULL; i++)
	{
		sTmp.Format("%d,%d,%d,%d,%d",
					CurrentSet->nAudioSpec[i][0],CurrentSet->nAudioSpec[i][1],
					CurrentSet->nAudioSpec[i][2],CurrentSet->nAudioSpec[i][3],
					CurrentSet->nAudioSpec[i][4]);
	
		m_Ini.SetProfileString(AUDIO_SPEC_S, InputDefined[i].pszName, sTmp);
	}

	//+add kwmoon 080904
	//=======================================
	// Set Audio Margin (Used in CheckAudio)
	//=======================================
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Level Margin",CurrentSet->nLevelMargin);
	m_Ini.SetProfileInt(AUDIO_MARGIN_S, "Audio Freq Margin",CurrentSet->nFreqMargin);

	//+move kwmoon 080904
	//=======================================
	// Set Image Margin (Used in SPEC-ADJ)
	//=======================================
	//del psh 090519
	/*
	for(i = 0;InputDefined[i].pszName != NULL; i++)
	{
		m_Ini.SetProfileInt(SRCMARGIN_S, InputDefined[i].pszName, CurrentSet->nSrcMargin[i]);
	}
	*/
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
	
	//==============================================================
	// Read Audio Spec for Each Input Source (Used in MeasureAudio)
	//==============================================================
	for(int i = 0;InputDefined[i].pszName != NULL; i++)
	{
		sTmp = m_Ini.GetProfileString(AUDIO_SPEC_S, InputDefined[i].pszName);

		nStart = 0; nEnd = 0;

		if(sTmp.Compare("NULL") != 0)
		{
			for(int j=0; j<4; j++)
			{
				nEnd   = sTmp.Find(",",nStart);
				sValue = sTmp.Mid(nStart,nEnd-nStart);

				CurrentSet->nAudioSpec[i][j] = atoi(sValue);

				nStart = nEnd+1;
			}

			sValue = sTmp.Right(sTmp.GetLength()-nStart);
			CurrentSet->nAudioSpec[i][4] = atoi(sValue);
		}
	}

	//+add kwmoon 080904
	//========================================
	// Read Audio Margin (Used in CheckAudio)
	//========================================
	CurrentSet->nLevelMargin = m_Ini.GetProfileInt(AUDIO_MARGIN_S, "Audio Level Margin");
	CurrentSet->nFreqMargin  = m_Ini.GetProfileInt(AUDIO_MARGIN_S, "Audio Freq Margin");

	if(CurrentSet->nLevelMargin == 0) CurrentSet->nLevelMargin = 45;
	if(CurrentSet->nFreqMargin == 0)  CurrentSet->nFreqMargin  = 15;

	//+move kwmoon 080904
	//=======================================
	// Read Image Margin (Used in SPEC-ADJ)
	//=======================================
	//del psh 090519
	/*
	for(i = 0;InputDefined[i].pszName != NULL; i++)
	{
		CurrentSet->nSrcMargin[i]	= m_Ini.GetProfileInt(SRCMARGIN_S, InputDefined[i].pszName);
	}
	*/
	return TRUE;
}

//+change kwmoon 081004
void SaveResultSummary()
{
	CFile pFile;
	CFileException FileExcept;

	CString sTemp			= _T("");
	CString szOutputString	= _T("");
	CString szResult		= _T("");
	CString szMeasure		= _T("");
	CString szLowLimit		= _T("");
	CString szHighLimit		= _T("");
	CString sAudioTarget	= _T("");
	CString sAudioMeasure	= _T("");

	BOOL	bAlreadyExist		= FALSE;

	CString sResultPath = CurrentSet->sResultSummaryPath;

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
	
	//=============
	// Print Title
	//=============
	szOutputString.Format("[ No of Tested B/d : %d, No of Failed B/d = %d, No of User Stop = %d ]\r\n\r\n",
						   g_nNoTested, g_nFailed, g_nUserStop);
	pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);

	if(!bAlreadyExist)
	{
		szOutputString.Format("WipID,Result,LastStep,Measure,LowerLimit,UpperLimit,AudioMeasure,AudioTarget\r\n");
		pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);
	}

	pFile.SeekToEnd();

	//=============	
	// Test Result 
	//=============
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

	//===============
	// Measure Value 
	//===============
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

	//=========================
	// Low & High Limit & Unit 
	//=========================
	if(pCurStep->m_nFuncType != MEAS_BOOL)
	{
		szLowLimit.Format("%4.1f",  pCurStep->m_fLowLimit);
		szHighLimit.Format("%4.1f", pCurStep->m_fHighLimit);
	}

	//========================
	// Audio Target & Measure
	//========================
	if(pCurStep->m_bRunAudioTest)
	{
		if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){
			sAudioTarget.Format(_T("X, X, %d, %d"), pCurStep->m_nLeftLevel[0], pCurStep->m_nRightLevel[0]);
		}
		else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){
			sAudioTarget.Format(_T("%d, %d, X, X"), pCurStep->m_nLeftFreq[0], pCurStep->m_nRightFreq[0]);
		}
		else{
			sAudioTarget.Format(_T("%d, %d, %d, %d"), pCurStep->m_nLeftFreq[0], pCurStep->m_nRightFreq[0], pCurStep->m_nLeftLevel[0], pCurStep->m_nRightLevel[0]);
		}
		sAudioMeasure.Format(_T("%d, %d, %d, %d"), pCurStep->m_nLeftFreq[1], pCurStep->m_nRightFreq[1], pCurStep->m_nLeftLevel[1], pCurStep->m_nRightLevel[1]);
	}

	if(pCurStep->m_bRunAudioTest)
	{
		szOutputString.Format("%s,%s,%d,%s,%s,%s,%s\r\n",
					   m_strWipId,
					   szResult,
					   pCurStep->GetStepNo(),
					   pCurStep->m_sName,
					   szMeasure,szLowLimit,szHighLimit,
					   sAudioMeasure,
					   sAudioTarget);
	}
	else
	{
		szOutputString.Format("%s,%s,%d,%s,%s,%s,%s\r\n",
					   m_strWipId,
					   szResult,
					   pCurStep->GetStepNo(),
					   pCurStep->m_sName,
					   szMeasure,szLowLimit,szHighLimit);
	}

	pFile.Write((LPSTR)(LPCTSTR)szOutputString,szOutputString.GetLength()+1);
	pFile.Close();
}
//CString GetExcelDriver()
//{
//	char szBuf[2001];
//	WORD cbBufMax = 2000;
//	WORD cbBufOut;
//	char *pszBuf = szBuf;
//	CString sExcelDriver = "";
//
//	// Get the names of the installed drivers ("odbcinst.h" has to be included )
//	if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
//	{
//		sExcelDriver = "";
//	}
//	
//	// Search for the driver...
//	do
//	{
//		if( strstr( pszBuf, "Excel" ) != 0 )
//		{
//			// Found !
//			sExcelDriver = CString( pszBuf );
//			break;
//		}
//		pszBuf = strchr( pszBuf, '\0' ) + 1;
//	}
//	while( pszBuf[1] != '\0' );
//
//	return sExcelDriver;
//}

BOOL OpenModelListFile(CString sModelListPath,int& nNoModel,CStringArray &Divisions)
{
#if 0
	POSITION PosStep = NULL;
	CString sSection = _T("");
	CString sTmp = _T("");
	//CStringArray Rows;

	DeleteModelList();

	if (!FileExists(CurrentSet->sModelListPath))
	{
		sTmp.Format("%s\n File doesn't Exist! Do you want to create file?", sModelListPath);

		if (AfxMessageBox(sTmp, MB_YESNO) == IDYES)
		{
			CreateModelListFile(CurrentSet->sModelListPath);
		}
		else
		{
			return FALSE;
		}
	}


	//CSpreadSheet tempSS(sModelListPath, "ModelList");

	//PosStep = CurrentSet->ModelList.GetHeadPosition();
	//nNoModel = tempSS.GetTotalRows();
	CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

	XL.OpenExcelFile(sModelListPath);
	
	nNoModel = 0;
	for (int i = 1; i < 10000; i++)
	{
		sTmp = XL.GetCellValue(1, i);
		if (sTmp.GetLength() > 0)
			nNoModel++;
		else
			i = 10000;
	}


// +add 090219(Modification No2)
//	CString szLog = "B:OpenModelListFile\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	for (int i = 2; i <= nNoModel; i++)
	{
		CModelData* pModel = new CModelData;

		// Read row
//		tempSS.ReadRow(Rows, i);
		//strData += "\n(1,1) = ";
		//strData += XL.GetCellValue(1, 1);
		//strData += "\n(1,2) = ";
		//strData += XL.GetCellValue(1, 2);
		//strData += "\n(1,3) = ";
		//strData += XL.GetCellValue(1, 3);
		//strData += "\n(1,4) = ";
		//strData += XL.GetCellValue(1, 4);

		//strData += "\n(4,4) = ";
		//strData += XL.GetCellValue(4, 4);

		//strData += "\n(7,4) = ";
		//strData += XL.GetCellValue(7, 4);

		pModel->m_szDivision = XL.GetCellValue( 1, i);
		pModel->m_szChassisName = XL.GetCellValue( 2, i);
		pModel->m_szModelName = XL.GetCellValue(3, i);
		pModel->m_szSeqFilePath = XL.GetCellValue( 4, i);
		pModel->m_szModelInfoFilePath = XL.GetCellValue( 5, i);
		pModel->m_szRefImgFolder = XL.GetCellValue( 6, i);

		//pModel->m_szDivision = Rows.GetAt(0);
		//pModel->m_szChassisName = Rows.GetAt(1);
		//pModel->m_szModelName = Rows.GetAt(2);
		//pModel->m_szSeqFilePath = Rows.GetAt(3);
		//pModel->m_szModelInfoFilePath = Rows.GetAt(4);
		//pModel->m_szRefImgFolder = Rows.GetAt(5);

		CurrentSet->ModelList.AddTail(pModel);

		int lFound = 0;
		for (int j = 0; j < Divisions.GetSize(); j++)
		{
			if (Divisions.GetAt(j).Compare(pModel->m_szDivision) == 0)
			{
				lFound = 1;
			}
		}

		if (lFound == 0)
		{
			Divisions.Add(pModel->m_szDivision);
			//Divisions.Serialize
		}
		//Divisions
		//m_aDivision.Add(m_stempString);
	}
	XL.ReleaseExcel();  // tempSS.GetDivisions(Divisions);

//+add 090219(Modification No2)
//	szLog = "E:OpenModelListFile\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;

#else
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



	//+add 090219(Modification No2)
//	CString szLog = "B:OpenModelListFile\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	for (int i = 2; i <= nNoModel; i++)
	{
		CModelData* pModel = new CModelData;

		// Read row
		tempSS.ReadRow(Rows, i);

		pModel->m_szDivision          = Rows.GetAt(0);
		pModel->m_szChassisName		  = Rows.GetAt(1);
		pModel->m_szModelName		  = Rows.GetAt(2);
		pModel->m_szSeqFilePath		  = Rows.GetAt(3);
		pModel->m_szModelInfoFilePath = Rows.GetAt(4);
		pModel->m_szRefImgFolder	  = Rows.GetAt(5);
		
		CurrentSet->ModelList.AddTail(pModel);
		

		//int lFlag = 0;
		//for (int id = 0; id < Divisions.GetCount(); id++)
		//{
		//	if (Divisions.GetAt(id).Compare(pModel->m_szDivision) == 0)
		//	{
		//		lFlag = 1;
		//		break;

		//	}

		//}


		//if (lFlag == 0)
		//{
		//	Divisions.Add(pModel->m_szDivision);
		//}

	}
	tempSS.GetDivisions(Divisions);

	//+add 090219(Modification No2)
//	szLog = "E:OpenModelListFile\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return TRUE;
#endif
}

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
BOOL StepInit_AvSwitchBox()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(!CurrentSet->bUseAVSwitchBox) return TRUE;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined[i].pszName) == NULL)
		{
			nSourceIndex = i; break;
		}
	}

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

//			g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1

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

			//+add 090210(Modification No1)
			g_pView->SetGrabInfo(&g_GrabImage);

			break;
		case 15:  //MNT-OUT
		case 30:  //VOUT2
			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
			g_CurSetting.m_nAudioSource  = 3;

			bResult = AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  2, 0);

//			g_GrabSource = ANALOG_GRABBER; // ANALOG_GRABBER : 1

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

			
			//+add 090210(Modification No1)
			g_pView->SetGrabInfo(&g_GrabImage);

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
//			g_GrabSource = LVDS_GRABBER; // LVDS_GRABBER : 0
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

		if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1;
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

BOOL StepInit_SourceAutoControl()
{
	CString szInputSource;
	int nSourceIndex = 0;
	BOOL bResult;
	BYTE nSourceCode = 0xff;
	CString szMsg1 = _T("");
	CString szPrevMsg = _T("");

	if(!CurrentSet->bUseTVCommPort) return TRUE;

	szInputSource = pCurStep->m_strCurSrc;

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined[i].pszName) == NULL)
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
				//	bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
				//	if(!bResult && (nSourceIndex == 14))
				//	{
				//		bResult = I2cAdcCtrl.SetIRcode(0xD9);
				//	}
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
				//	bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
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
		//	bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
		//	if(!bResult)
		//	{
		//		I2cAdcCtrl.SetDDC2AB();
		//		I2cAdcCtrl.WakeUp();
		//		bResult = I2cAdcCtrl.InputSourceChange(nSourceCode);
				//add 100101
		//		if(!bResult && (nSourceIndex == 14))
		//		{
		//			bResult = I2cAdcCtrl.SetIRcode(0xD9);
		//		}
		//	}
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
		if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1;
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
int GetHDMI_Model()
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
BOOL StepInit_PatternAutoControl()
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

	for(int i = 0; InputDefined[i].pszName != NULL; i++)
	{
		if(szInputSource.Compare(InputDefined[i].pszName) == NULL)
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
			nModel = GetHDMI_Model();
			nPattern = 16;
			break;

		case 21:  //HDMI2
			nModel = GetHDMI_Model();
			nPattern = 20;
			break;

		case 22:  //HDMI3
			nModel = GetHDMI_Model();
			nPattern = 23;
			break;

		case 23:  //HDMI4
			nModel = GetHDMI_Model();
			nPattern = 6;
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

		default:
			break;
	}

	if(pCurStep->m_nTestType == OSD_TEST) nPattern = 2;

	if(nModel == 0) return TRUE;

	if((nSourceIndex >= 19) && (nSourceIndex <= 23))
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

		if((CurrentSet->bIsRunning) && (CurrentSet->nDisplayType == DETAILEDGRID))
		{
			g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
			
			if(szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + szMsg1);
				pCurStep->m_strMsg = szPrevMsg + " , " + szMsg1;
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
			strcpy(szDir, strTmpPath.GetBuffer(strTmpPath.GetLength()));
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

	   strcpy(szPath, (LPCTSTR)strRootFolder);
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

    _tcscpy(DirPath,sPath);
    _tcscat(DirPath,"\\*");    // searching all files
    _tcscpy(FileName,sPath);
    _tcscat(FileName,"\\");

    hFind = FindFirstFile(DirPath,&FindFileData); // find the first file
    if(hFind == INVALID_HANDLE_VALUE) return FALSE;
    _tcscpy(DirPath,FileName);
        
    bool bSearch = true;
    while(bSearch) { // until we finds an entry
        if(FindNextFile(hFind,&FindFileData)) {
            if(IsDots(FindFileData.cFileName)) continue;
            _tcscat(FileName,FindFileData.cFileName);
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                // we have found a directory, recurse
                if(!DeleteDirectory(FileName)) { 
                    FindClose(hFind); 
                    return FALSE; // directory couldn't be deleted
                }
                RemoveDirectory(FileName); // remove the empty directory
                _tcscpy(FileName,DirPath);
            }
            else {
                if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                    _chmod(FileName, _S_IWRITE); // change read-only file mode
                if(!DeleteFile(FileName)) {  // delete the file
                    FindClose(hFind); 
                    return FALSE; 
                }                 
                _tcscpy(FileName,DirPath);
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

	delTime = curTime - ts;

	szYear = delTime.Format("[%y_%m_%d]");
	
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
        
/*
				sTemp.TrimLeft('[');
				sTemp.TrimRight(']');

				sTemp2 = sTemp.Left(2);
				nYear_temp = atoi(sTemp2);

				sTemp2 = sTemp.Mid(3,2);
				nMonth_temp = atoi(sTemp2);

				sTemp2 = sTemp.Right(2);
				nDay_temp = atoi(sTemp2);

				if((nYear_temp <= nYear) && (nMonth_temp <= nMonth) && (nDay_temp <= nDay))
*/
				nTemp = sTemp.Compare(szYear);
				if(nTemp <= 0)
				{
					RecursiveDelete(sPath);
					RemoveDirectory(sPath);
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
    struct tm *today;
	CString filename;


    time( &ltime );
    today = localtime( &ltime );

    _tcsftime(tmpbuf, 128, _T("%Y%m%d"), today);

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
		_tcsftime(tmpbuf, 128, _T("%H%M%S"), today);

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
	
	//+add 090219(Modification No2)
	CString szLog = "B:TestCountSave\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

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

	//+add 090219(Modification No2)
	szLog = "E:TestCountSave\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

    return TRUE;
	
}

BOOL GmesLog_Save(CString sExePath, CString sComData, BOOL bFlag)
{
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

	sTemp2 = curTime.Format(_T("[%D%H%M%S]"));
	sDay = curTime.Format(_T("%Y%m%d")); 
	sTemp.Format(_T("%s\\TestResult"), sExePath);
	
	if(bFlag){
		sMsg.Format(_T("[RECV(GMES)] : %s\n"), sTemp2);
	}
	else{
		sMsg.Format(_T("[SEND(GMES)] : %s\n"), sTemp2);
	}


	if(!FileExists(sTemp))   CreateFullPath(sTemp);

	WIN32_FIND_DATA FindFileData;
	//090909
	HANDLE hFind;  // file handle

	filename.Format(_T("%s\\GmesLog_%s_%d.txt"),sTemp, sDay, g_nRunningProcessNo); 

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

	//+add 090219(Modification No2)
	CString szLog = "B:TestCountRead\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

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


		//+add 090219(Modification No2)
		szLog = "E:TestCountRead\n";
//		if(g_LogFileOpen) g_LogFile.WriteString(szLog);

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

		//+add 090219(Modification No2)
		szLog = "E:TestCountRead\n";
//		if(g_LogFileOpen) g_LogFile.WriteString(szLog);

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

BOOL FindRemoconCode(CString strName, CString& strCustom, CString& strCode, int& nByte)
{
	POSITION Pos = NULL;
	CRemoteCode* pRemote = NULL;
	CString sName, sCode;
	BOOL bFind = FALSE;

	Pos = CurrentSet->RemoteList.GetHeadPosition();

	while(Pos)
	{
		pRemote = CurrentSet->RemoteList.GetNext(Pos);
		sName = pRemote->sName;
		sName.MakeUpper();
		strName.MakeUpper();

		if(sName == strName)
		{
			strCustom = pRemote->sCustomCode;
			strCode = pRemote->sCode;
			nByte = pRemote->nByte;
			bFind = TRUE;
			return TRUE;
		}
	}

	if(bFind == FALSE) 
	{
		return FALSE;
	}
	return TRUE;
}

BOOL Get_ToolOption(CString sOption)
{
	CStringArray SplitIt;
	CString sTemp;
	CString sBuf;
	int nSize;
	int nOptionNo;
	CString sOptionVal;
	int nCount;
	CString sString;
	int nFind;
	CString sTemp_Option;

//	[NetCast4.0 L13] Tool Option(1:32852,2:2406,3:26525,4:46864,5:47108,6:97)/ Area Code:1793/ Country Group:JA/ 
//	CurrentSet->sToolOption1 = "";
//	CurrentSet->sToolOption2 = "";
//	CurrentSet->sToolOption3 = "";
//	CurrentSet->sToolOption4 = "";
//	CurrentSet->sToolOption5 = "";
//	CurrentSet->sToolOption6 = "";
//	CurrentSet->sToolOption7 = "";
//	CurrentSet->sAreaOption1 = "";

	sString = sOption;
	nCount = sString.Find("TOOL OPTION");
	if(nCount == -1){
		return FALSE;
	}
	AddStringToStatus(sOption);
	sString.Delete(0, nCount);
	nFind = sString.Find('/');
	sBuf = sString.Left(nFind);
	sTemp_Option = sBuf.Right(sBuf.GetLength() - (sBuf.Find("TOOL OPTION") + 11));
	sTemp_Option.TrimLeft("(");
	sTemp_Option.TrimRight(")");


	StringToken(sTemp_Option, ",", SplitIt, TRUE);
	nSize = SplitIt.GetSize();
	if(nSize == 0) return FALSE;

	for(int i = 0; i< nSize; i++)
	{
		sTemp = SplitIt.GetAt(i);
		sBuf = sTemp.Left(sTemp.Find(':'));
		nOptionNo = _ttoi((const TCHAR*)sBuf);
		sOptionVal = sTemp.Right(sTemp.GetLength() - (sTemp.Find(':') + 1));

	/*	switch(nOptionNo)
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
		} */
	}
	nCount = sString.Find("AREA CODE");
	if(nCount != -1){
		sString.Delete(0, nCount);
		nFind = sString.Find('/');
		sBuf = sString.Left(nFind);
		sBuf.TrimLeft("AREA CODE:");

	//	CurrentSet->sAreaOption1 = sBuf;
	}
//	AddStringToStatus(sOption);

	return TRUE;
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

BOOL Send_ResultMpKey( BOOL bResult) 
{
	CString sCmd = _T("");
	CString sName = _T("");
	CString sCustom = _T("");
	int nByte = 1;
	CString sMsg;
	BOOL bRev = FALSE;

	if(CurrentSet->nProcessType > 1){return TRUE;}

	if(CurrentSet->nProcessType == 0)
	{
		if(bResult){
			sName = "DFT1_OK";
		}
		else{
			sName = "DFT1_NG";
		}
	}
	else{
		if(bResult){
			sName = "DFT2_OK";
		}
		else{
			sName = "DFT2_NG";
		}
	}

	if(!FindRemoconCode(sName, sCustom, sCmd, nByte))
	{
		sMsg.Format("Remocon Code Not Found: %s", sName);
		AddStringToRemoteComLog(sMsg);
		return FALSE;
	}

	bRev = AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte);	

	return bRev;
}