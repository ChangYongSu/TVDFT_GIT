// ModelInfoCreate_GraberOption.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "ModelInfoCreate_GraberOption.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate_GraberOption dialog


CModelInfoCreate_GraberOption::CModelInfoCreate_GraberOption(CWnd* pParent /*=NULL*/)
	: CDialog(CModelInfoCreate_GraberOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModelInfoCreate_GraberOption)
	m_nAnalogFormat = -1;
	m_ctlTVControlMode = 0;
	m_bAutoSourceControl = FALSE;
	m_bAutoPatternControl = FALSE;
	m_nRemoconCustomCode = -1;
	m_nRemoteType = -1;
	//}}AFX_DATA_INIT
}


void CModelInfoCreate_GraberOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModelInfoCreate_GraberOption)
	DDX_Control(pDX, IDC_CMB_BAUDRATE_TV2, m_ctrlTVBaudCmb);
	DDX_Radio(pDX, IDC_RDO_ANALOG_FORMAT1, m_nAnalogFormat);
	DDX_Radio(pDX, IDC_RADIO_TV_CONTROL_RS232C, m_ctlTVControlMode);
	DDX_Check(pDX, IDC_CHK_AUTO_SOURCE_CONTROL, m_bAutoSourceControl);
	DDX_Check(pDX, IDC_CHK_AUTO_PATTERN_CONTROL, m_bAutoPatternControl);
	DDX_Radio(pDX, IDC_RDO_REMOCON_CUSTOM1, m_nRemoconCustomCode);
	DDX_Radio(pDX, IDC_RDO_REMOCON_TYPE1, m_nRemoteType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModelInfoCreate_GraberOption, CDialog)
	//{{AFX_MSG_MAP(CModelInfoCreate_GraberOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate_GraberOption message handlers

void CModelInfoCreate_GraberOption::OnOK() 
{
	UpdateData(TRUE);
	
	CString sMsg = _T("");

/*	int nTmp = 0;
	
	switch(m_nUranousMode) 
	{
		//0 : JEIDA_DUAL(0x81), 1 : JEIDA_SINGLE(0x01), 2 : NS_DUAL(0x80), 3 : NS_SINGLE(0x00)
		case 0 :  nTmp = 0x84; break; //JEIDA_DUAL
		case 1 :  nTmp = 0x04; break; //JEIDA_SINGLE
		case 2 :  nTmp = 0x80; break; //NS_DUAL
		case 3 :  nTmp = 0x00; break; //NS_SINGLE
		default : nTmp = 0x00; break; //NS_SINGLE
	}

	CurrentSet->nImageRotation = 0;

	switch(m_nLvdsFormat) 
	{
		case W_852_H_480: 
			CurrentSet->nLvdsWidth = 852; CurrentSet->nLvdsHeight = 480;			break;
		case W_1024_H_768: 
			CurrentSet->nLvdsWidth = 1024; CurrentSet->nLvdsHeight = 768;			break;
		case W_1280_H_768: 
			CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 768;			break;
		case W_1280_H_1024: 
			CurrentSet->nLvdsWidth = 1280; CurrentSet->nLvdsHeight = 1024;			break;
		case W_1366_H_768: 
			CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;			break;
		case W_1440_H_900: 
			CurrentSet->nLvdsWidth = 1440; CurrentSet->nLvdsHeight = 900;			break;
		case W_1680_H_1050: 
			CurrentSet->nLvdsWidth = 1680; CurrentSet->nLvdsHeight = 1050;			break;
		
		case W_1920_H_1080_60: 
		{
			CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;			
//			CurrentSet->nFullHd120Hz = 0;
		}
		break;
		
		case W_1920_H_1080_120: 
		{
			CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;			
//			CurrentSet->nFullHd120Hz = 1;
		}
		break;
		
		//+add kwmoon 080922
		case W_1364_H_768: 
		{	
			CurrentSet->nLvdsWidth = 1364; CurrentSet->nLvdsHeight = 768;	
			nTmp = nTmp | 0x10;
		}
		break;
		
		default : CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
	}
*/
	CurrentSet->nAnalogType = m_nAnalogFormat;

	switch(m_nAnalogFormat) 
	{
		//+change 090420(Modification No1)
		case 0: CurrentSet->nAnalogWidth = 640; CurrentSet->nAnalogHeight = 480; CurrentSet->nAnalogType = V_SRC_NTSC;	break;
		case 1: CurrentSet->nAnalogWidth = 768; CurrentSet->nAnalogHeight = 576; CurrentSet->nAnalogType = V_SRC_PAL;	break;
		case 2: CurrentSet->nAnalogWidth = 768; CurrentSet->nAnalogHeight = 576; CurrentSet->nAnalogType = V_SRC_SECAM;	break;
		default: CurrentSet->nAnalogWidth = 640; CurrentSet->nAnalogHeight = 480;	 CurrentSet->nAnalogType = V_SRC_NTSC;	break;
	}

/*
	switch(m_nBitMode) 
	{
		case 0:   CurrentSet->nUranousMode = nTmp;		   CurrentSet->nBitCount = 6*3;	 break;
		case 1:	  CurrentSet->nUranousMode = (nTmp | 0x1); CurrentSet->nBitCount = 8*3;  break;
		case 2:	  CurrentSet->nUranousMode = (nTmp | 0x2); CurrentSet->nBitCount = 10*3; break;
		case 3:	  CurrentSet->nUranousMode = (nTmp | 0x3); CurrentSet->nBitCount = 12*3; break;
		default : CurrentSet->nUranousMode = (nTmp | 0x3); CurrentSet->nBitCount = 8*3;	 break;
	}
	//add psh 091214
	CurrentSet->bOddDE_Only = m_bOddDE_Only;
	CurrentSet->nImageRotation = m_bImage_Rotation;
*/
	CurrentSet->nGrabCheckArea = 7;
	CurrentSet->nAudioCheckMethod = METHOD_SOUNDCARD;



	//==================
	// Tool/Area Option
	//==================
//	CurrentSet->sToolOption1 = "";
//	CurrentSet->sToolOption2 = "";
//	CurrentSet->sToolOption3 = "";
//	CurrentSet->sToolOption4 = "";
//	CurrentSet->sToolOption5 = "";
//	CurrentSet->sAreaOption1 = "";

//	CurrentSet->sCPUVersion = "";
//	CurrentSet->sMicomVersion = "";
//	CurrentSet->sUSBVersion = "";

	
	CurrentSet->sTVComPort = "COM4";
	//CurrentSet->sTVComPort1 = "COM4";
	//CurrentSet->sTVComPort2 = "COM6";

	DWORD wBaudRate;
	int nCurSel = m_ctrlTVBaudCmb.GetCurSel();
	switch(nCurSel) 
	{
		case 0: wBaudRate = CBR_9600;	break;
		case 1: wBaudRate = CBR_19200;	break;
		case 2: wBaudRate = CBR_38400;	break;
		case 3: wBaudRate = CBR_57600;	break;
		case 4: wBaudRate = CBR_115200;	break;
		default: wBaudRate = CBR_9600; break;
	}
	CurrentSet->wTVBaudRate = wBaudRate;
//	CurrentSet->wTVBaudRate1 = wBaudRate;
	//CurrentSet->wTVBaudRate2 = wBaudRate;

	CurrentSet->nChipType = CHIP_MTK;
	//CurrentSet->nChipType1 = CHIP_MTK;
	//CurrentSet->nChipType2 = CHIP_MTK;

	CurrentSet->nTVParity = EVENPARITY;
	//CurrentSet->nTVParity1 = EVENPARITY;
	//CurrentSet->nTVParity2 = EVENPARITY;
	CurrentSet->bUseTVCommPort = 1;


	//============
	// TV Control
	//============
	CurrentSet->nTVControlType = m_ctlTVControlMode;
	CurrentSet->bAutoSourceControl = m_bAutoSourceControl;
	CurrentSet->bAutoPatternControl = m_bAutoPatternControl;

	//add psh 110226
	CurrentSet->nRemoteCustomCode = m_nRemoconCustomCode;
	CurrentSet->nRemoteType = m_nRemoteType;
	
	CDialog::OnOK();
}

BOOL CModelInfoCreate_GraberOption::OnInitDialog() 
{
	CDialog::OnInitDialog();
/*	
	if((CurrentSet->nLvdsWidth == 852)		 && (CurrentSet->nLvdsHeight == 480))  m_nLvdsFormat = W_852_H_480;
	else if((CurrentSet->nLvdsWidth == 1024) && (CurrentSet->nLvdsHeight == 768))  m_nLvdsFormat = W_1024_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 768))  m_nLvdsFormat = W_1280_H_768;
	else if((CurrentSet->nLvdsWidth == 1280) && (CurrentSet->nLvdsHeight == 1024)) m_nLvdsFormat = W_1280_H_1024;
	else if((CurrentSet->nLvdsWidth == 1366) && (CurrentSet->nLvdsHeight == 768))  m_nLvdsFormat = W_1366_H_768;
	else if((CurrentSet->nLvdsWidth == 1440) && (CurrentSet->nLvdsHeight == 900))  m_nLvdsFormat = W_1440_H_900;
	else if((CurrentSet->nLvdsWidth == 1680) && (CurrentSet->nLvdsHeight == 1050)) m_nLvdsFormat = W_1680_H_1050;

	//+add kwmoon 080922
	else if((CurrentSet->nLvdsWidth == 1364) && (CurrentSet->nLvdsHeight == 768)) m_nLvdsFormat = W_1364_H_768;
	else m_nLvdsFormat = -1;

	switch(CurrentSet->nUranousMode) 
	{
		case JEIDA_DUAL6	: case (JEIDA_DUAL6  | 0x10)	: m_nUranousMode = 0; m_nBitMode = 0;	break;
		case JEIDA_DUAL8	: case (JEIDA_DUAL8  | 0x10)	: m_nUranousMode = 0; m_nBitMode = 1;	break;
		case JEIDA_DUAL10	: case (JEIDA_DUAL10 | 0x10)	: m_nUranousMode = 0; m_nBitMode = 2;	break;
		case JEIDA_DUAL12	: case (JEIDA_DUAL12 | 0x10)	: m_nUranousMode = 0; m_nBitMode = 3;	break;

		case JEIDA_SINGLE6	: case (JEIDA_SINGLE6  | 0x10)	: m_nUranousMode = 1; m_nBitMode = 0;   break;
		case JEIDA_SINGLE8	: case (JEIDA_SINGLE8  | 0x10)	: m_nUranousMode = 1; m_nBitMode = 1;	break;
		case JEIDA_SINGLE10 : case (JEIDA_SINGLE10 | 0x10)	: m_nUranousMode = 1; m_nBitMode = 2;	break;
		case JEIDA_SINGLE12 : case (JEIDA_SINGLE12 | 0x10)	: m_nUranousMode = 1; m_nBitMode = 3;	break;
		
		case NS_DUAL6		: case (NS_DUAL6  | 0x10)	: m_nUranousMode = 2; m_nBitMode = 0;	break;
		case NS_DUAL8		: case (NS_DUAL8  | 0x10)	: m_nUranousMode = 2; m_nBitMode = 1;	break;
		case NS_DUAL10		: case (NS_DUAL10 | 0x10)	: m_nUranousMode = 2; m_nBitMode = 2;	break;
		case NS_DUAL12		: case (NS_DUAL12 | 0x10)	: m_nUranousMode = 2; m_nBitMode = 3;	break;

		case NS_SINGLE6		: case (NS_SINGLE6  | 0x10)	: m_nUranousMode = 3; m_nBitMode = 0;	break;
		case NS_SINGLE8		: case (NS_SINGLE8  | 0x10)	: m_nUranousMode = 3; m_nBitMode = 1;	break;
		case NS_SINGLE10	: case (NS_SINGLE10 | 0x10)	: m_nUranousMode = 3; m_nBitMode = 2;	break;
		case NS_SINGLE12	: case (NS_SINGLE12 | 0x10)	: m_nUranousMode = 3; m_nBitMode = 3;	break;
	}
*/
	// Analog Format
	m_nAnalogFormat = CurrentSet->nAnalogType;
	m_bAutoSourceControl = TRUE;
	m_bAutoPatternControl = TRUE;
	//+change 090331
	m_ctrlTVBaudCmb.SetCurSel(4);

	//add psh 110303
	m_nRemoconCustomCode = CurrentSet->nRemoteCustomCode;
	m_nRemoteType = CurrentSet->nRemoteType;
	
	UpdateData(FALSE);

	return TRUE;  
}
