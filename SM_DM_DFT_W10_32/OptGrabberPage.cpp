// OptGrabberPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "Defines.h"
#include "OptGrabberPage.h"
#include "Global.h"
#include "SetDateDlg.h"
#include "UserMsgDlg.h"

#include "AnalogGrabber.h"
#include "AvSwitchBoxCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*  g_pView;
extern CBaseImage	g_GrabImage;
extern CAnalogGrabber		AnalogControl;


/////////////////////////////////////////////////////////////////////////////
// COptGrabberPage property page

IMPLEMENT_DYNCREATE(COptGrabberPage, CPropertyPage)

COptGrabberPage::COptGrabberPage() : CPropertyPage(COptGrabberPage::IDD)
{
	//{{AFX_DATA_INIT(COptGrabberPage)
	m_nAnalogFormat = -1;
	m_nAudioMeasureMethod = 1;
	m_bAutoDataDelete = FALSE;
	m_nHDMI_Resolution = -1;
	m_nRemoconCustomCode = -1;
	m_nRemoteType = -1;
	m_nAvSwitchBoxBuzzerMute = -1;
	m_nGrabCheckArea = -1;
	m_nAvSwitchBoxVideoMode = -1;
	//}}AFX_DATA_INIT
}

COptGrabberPage::~COptGrabberPage()
{
}

void COptGrabberPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptGrabberPage)
	DDX_Control(pDX, IDC_HDMI_BIT_SPEC, m_ctrlHDMI_BitSpec);
	DDX_Control(pDX, IDC_COMBO_PERIOD, m_ctrlPeriod);
	DDX_Radio(pDX, IDC_RDO_ANALOG_FORMAT1, m_nAnalogFormat);
	DDX_Radio(pDX, IDC_RDO_AVSWITCHBOX, m_nAudioMeasureMethod);
	DDX_Check(pDX, IDC_CHK_AUTO_DATA_DELETE, m_bAutoDataDelete);
	DDX_Radio(pDX, IDC_HDMI_RESOLUTION1, m_nHDMI_Resolution);
	DDX_Radio(pDX, IDC_RDO_REMOCON_CUSTOM1, m_nRemoconCustomCode);
	DDX_Radio(pDX, IDC_RDO_REMOCON_TYPE1, m_nRemoteType);
	DDX_Radio(pDX, IDC_RDO_BUZZER_MUTE1, m_nAvSwitchBoxBuzzerMute);
	DDX_Radio(pDX, IDC_GRAB_CHECK_AREA1, m_nGrabCheckArea);
	DDX_Radio(pDX, IDC_RDO_VIDEO_MODE1, m_nAvSwitchBoxVideoMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptGrabberPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptGrabberPage)
	ON_BN_CLICKED(IDC_BTN_GRABBER_OPT_APPLY, OnBtnGrabberOptApply)
	ON_BN_CLICKED(IDC_BTN_GRABBER_OPT_DELETE, OnBtnGrabberOptDelete)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_CBN_SELCHANGE(IDC_COMBO_PERIOD, OnSelchangeComboPeriod)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION1, OnHdmiResolution1)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION2, OnHdmiResolution2)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION3, OnHdmiResolution3)
	ON_BN_CLICKED(IDC_HDMI_RESOLUTION4, OnHdmiResolution4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptGrabberPage message handlers


void COptGrabberPage::OnBtnGrabberOptApply() 
{
	UpdateData(TRUE);
	
	CString sMsg = _T("");
//	UINT nVal1; 
	BOOL bGrabFlag;
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

	//+del0831
//	CurrentSet->nFullHd120Hz = 0;
	// add 091214
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
			// change psh 090630
			CurrentSet->bOddDE_Only = m_bOddDE_Only;
//			if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
//			{
//				nTmp = nTmp | 0x10;
//			}
		}
		break;
		
		case W_1920_H_1080_120: 
		{
			CurrentSet->nLvdsWidth = 1920; CurrentSet->nLvdsHeight = 1080;			
//			CurrentSet->nFullHd120Hz = 1;
			// change psh 090630
			CurrentSet->bOddDE_Only = m_bOddDE_Only;
//			if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 1) && m_bOddDE_Only)
//			{
//				nTmp = nTmp | 0x10;
//			}
		}
		break;
		
		//+add kwmoon 080922
		case W_1364_H_768: 
		{	
			CurrentSet->nLvdsWidth = 1364; CurrentSet->nLvdsHeight = 768;	
			// change psh 090630
//			if((g_pView->m_pImageInfo != NULL) && (g_pView->m_pImageInfo->nUseExtAnalogGrabber == 0))
//			{
//				nTmp = nTmp | 0x10;
//			}
		}
		break;
		
		default : CurrentSet->nLvdsWidth = 1366; CurrentSet->nLvdsHeight = 768;		break;
	}
*/
	CurrentSet->nAnalogType = m_nAnalogFormat;

	AvSwitchBoxCtrl.SetVideoOutType(CurrentSet->nAnalogType);
	
	bGrabFlag = g_pView->m_bGrabThreadRunning;
	g_pView->StopGrabThread();
	Sleep(500);
	AnalogControl.SetVideoSourceType(CurrentSet->nAnalogType);
	Sleep(500);
	if(bGrabFlag) g_pView->StartGrabThread();

	switch(m_nAnalogFormat) 
	{
		//+change 090420(Modification No1)
		case 0:  CurrentSet->nAnalogWidth = 640; CurrentSet->nAnalogHeight = 480; CurrentSet->nAnalogType = V_SRC_NTSC;	 break;
		case 1:  CurrentSet->nAnalogWidth = 768; CurrentSet->nAnalogHeight = 576; CurrentSet->nAnalogType = V_SRC_PAL;	 break;
		case 2:  CurrentSet->nAnalogWidth = 768; CurrentSet->nAnalogHeight = 576; CurrentSet->nAnalogType = V_SRC_SECAM; break;
		default: CurrentSet->nAnalogWidth = 640;   CurrentSet->nAnalogHeight = 480;	  CurrentSet->nAnalogType = V_SRC_NTSC;	 break;
	}

/*	switch(m_nBitMode) 
	{
		case 0:   CurrentSet->nUranousMode = nTmp;		   CurrentSet->nBitCount = 6*3;	 break;
		case 1:	  CurrentSet->nUranousMode = (nTmp | 0x1); CurrentSet->nBitCount = 8*3;  break;
		case 2:	  CurrentSet->nUranousMode = (nTmp | 0x2); CurrentSet->nBitCount = 10*3; break;
		case 3:	  CurrentSet->nUranousMode = (nTmp | 0x3); CurrentSet->nBitCount = 12*3; break;
		default : CurrentSet->nUranousMode = (nTmp | 0x3); CurrentSet->nBitCount = 8*3;	 break;
	}
*/
	CurrentSet->nGrabCheckArea = m_nGrabCheckArea;

	if(m_nAvSwitchBoxBuzzerMute ==0){
		CurrentSet->bAvSwitchBoxBuzzerMute = TRUE;
	}
	else{
		CurrentSet->bAvSwitchBoxBuzzerMute = FALSE;
	}
	AvSwitchBoxCtrl.SetBuzzerMute(CurrentSet->bAvSwitchBoxBuzzerMute);

	//add psh 110226
	if(CurrentSet->nRemoteCustomCode != m_nRemoconCustomCode)
	{
		CurrentSet->nRemoteCustomCode = m_nRemoconCustomCode;
		switch(CurrentSet->nRemoteCustomCode)
		{
			case REMOCON_HT:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
				break;

			case REMOCON_MINI:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_MINI_REMOTE.rmt";
				break;

			case REMOCON_BD:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_BD_REMOTE.rmt";
				break;

			case REMOCON_PN:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_PN_REMOTE.rmt";
				break;

			case REMOCON_LO:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LO_REMOTE.rmt";
				break;

			case REMOCON_NA:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_NA_REMOTE.rmt";
				break;

			case REMOCON_SO:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_SO_REMOTE.rmt";
				break;

			case REMOCON_CM:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSC_CM_REMOTE.rmt";
				break;

			case REMOCON_GS:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSC_GS_REMOTE.rmt";
				break;

			case REMOCON_TB:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSC_TB_REMOTE.rmt";
				break;

			case REMOCON_SH:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSH_REMOTE.rmt";
				break;

			case REMOCON_SI:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_LSI_REMOTE.rmt";
				break;

			case REMOCON_JV:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_JVC_REMOTE.rmt";
				break;

			default:
				CurrentSet->sRemoconTitle = g_pView->m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
				break;

		}
			//+add kwmoon 080821 : Add FileExists Check
		if(FileExists(CurrentSet->sRemoconTitle))
		{
			OpenRemoteFile(CurrentSet->sRemoconTitle);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sRemoconTitle);
			AfxMessageBox(sMsg);
		}

	}
	CurrentSet->nRemoteType = m_nRemoteType;

	//add psh 091214
//	CurrentSet->nImageRotation = m_bImage_Rotation;

	CString szErrorMsg   = _T("");
/*	CString szOptionData = _T("");

	CurrentSet->sToolOption1 = m_szToolOption1;
	CurrentSet->sToolOption2 = m_szToolOption2;
	CurrentSet->sToolOption3 = m_szToolOption3;
	CurrentSet->sToolOption4 = m_szToolOption4;
	CurrentSet->sToolOption5 = m_szToolOption5;
	CurrentSet->sAreaOption1 = m_szAreaOption1;

	g_pView->UpdateInfo(TOOL_OPTION1,CurrentSet->sToolOption1);
	g_pView->UpdateInfo(TOOL_OPTION2,CurrentSet->sToolOption2);
	g_pView->UpdateInfo(TOOL_OPTION3,CurrentSet->sToolOption3);
	g_pView->UpdateInfo(TOOL_OPTION4,CurrentSet->sToolOption4);
	g_pView->UpdateInfo(TOOL_OPTION5,CurrentSet->sToolOption5);
	g_pView->UpdateInfo(AREA_OPTION1,CurrentSet->sAreaOption1);
//	g_pView->UpdateInfo(AREA_OPTION2,CurrentSet->sAreaOption2);
*/
	m_pProgressMsgDlg	= NULL;

	if(m_nAudioMeasureMethod == METHOD_AVSWITCHBOX)
	{
		CurrentSet->nAudioCheckMethod = METHOD_AVSWITCHBOX;
	}
	else
	{
		CurrentSet->nAudioCheckMethod = METHOD_SOUNDCARD;
	}
	//+add psh 090401
	CurrentSet->nDataSavePeriod = m_nPeriod;	
	CurrentSet->bAutoDataDelete = m_bAutoDataDelete;

	//+add 090131(수정항목2)
	g_pView->SetGrabInfo(&g_GrabImage);

	//+add psh 100310
	CurrentSet->nHDMIResolution = m_nHDMI_Resolution;
	CurrentSet->nHDMIBitSpec = m_ctrlHDMI_BitSpec.GetCurSel();

	g_pView->SaveRegistrySetting();

	//add psh 110226
/*	switch(CurrentSet->nRemoteCustomCode) 
	{
		case 0:  nVal1 = 0x2C; 	break;
		case 1:	 nVal1 = 0x10; 	break;
		case 2:	 nVal1 = 0x2D; 	break;
		default : nVal1 = 0x2C;	break;
	}
	AvSwitchBoxCtrl.SetAvSwitch(SET_CUSTOM_CODE, MAX_AVSWITCH_WAIT_DELAY,  nVal1, 0);
*/
	AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nRemoteType, 0);

	switch(m_nAvSwitchBoxVideoMode) 
	{
		case 0	: CurrentSet->nAvSwitchBoxInitVideoCh = 1;	break;
		case 1	: CurrentSet->nAvSwitchBoxInitVideoCh = 3;	break;
		case 2	: CurrentSet->nAvSwitchBoxInitVideoCh = 8;	break;
		case 3	: CurrentSet->nAvSwitchBoxInitVideoCh = 9;	break;
		default : CurrentSet->nAvSwitchBoxInitVideoCh = 1;	break;

	}
	AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nAvSwitchBoxInitVideoCh, 0);
}

BOOL COptGrabberPage::MakeOptionString(CString& szOptionData,UINT nNoDigit)
{
	CString szZeroData = _T("");

	UINT nLength = szOptionData.GetLength();

	if(nLength <= nNoDigit)
	{
		for(int i=0; i<(int)(nNoDigit-nLength); i++)
		{
			szZeroData += '0';
		}
		szOptionData = szZeroData + szOptionData;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL COptGrabberPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
/*	
	// Lvds Format
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
*/
	m_nAnalogFormat = CurrentSet->nAnalogType;

/*	switch(CurrentSet->nUranousMode) 
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
	m_nGrabCheckArea = CurrentSet->nGrabCheckArea;
	//add psh 090630
//	m_bOddDE_Only = CurrentSet->bOddDE_Only;
	//add psh 091214
//	m_bImage_Rotation = CurrentSet->nImageRotation;

	//add psh 110226
	m_nRemoconCustomCode = CurrentSet->nRemoteCustomCode;
	m_nRemoteType = CurrentSet->nRemoteType;

	UpdateData(FALSE);

/*
	m_szToolOption1 = CurrentSet->sToolOption1;
	m_szToolOption2 = CurrentSet->sToolOption2;
	m_szToolOption3 = CurrentSet->sToolOption3;
	m_szToolOption4 = CurrentSet->sToolOption4;
	m_szToolOption5 = CurrentSet->sToolOption5;
	m_szAreaOption1 = CurrentSet->sAreaOption1;
//	m_szAreaOption2 = CurrentSet->sAreaOption2;
*/
	//090402
	m_bAutoDataDelete	 = CurrentSet->bAutoDataDelete;
	m_nPeriod  = CurrentSet->nDataSavePeriod;
	if(m_nPeriod <15){
		m_nPeriod = 15;
	}
	m_ctrlPeriod.SetCurSel(m_nPeriod/15 -1);
	//-

	//+add kwmoon 080523
	m_nAudioMeasureMethod = CurrentSet->nAudioCheckMethod;

	//+add psh 100310
	m_nHDMI_Resolution = CurrentSet->nHDMIResolution;
	m_ctrlHDMI_BitSpec.SetCurSel(CurrentSet->nHDMIBitSpec);
	if(CurrentSet->nHDMIResolution ==0)
	{
		GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(TRUE);
	}
	else{
		GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
	}
	if(CurrentSet->bAvSwitchBoxBuzzerMute){
		m_nAvSwitchBoxBuzzerMute = 0;
	}
	else{
		m_nAvSwitchBoxBuzzerMute = 1;
	}
	switch(CurrentSet->nAvSwitchBoxInitVideoCh) 
	{
		case 1	: m_nAvSwitchBoxVideoMode = 0;	break;
		case 3	: m_nAvSwitchBoxVideoMode = 1;	break;
		case 8	: m_nAvSwitchBoxVideoMode = 2;	break;
		case 9	: m_nAvSwitchBoxVideoMode = 3;	break;
		default : m_nAvSwitchBoxVideoMode = 0;	break;

	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptGrabberPage::OnBtnGrabberOptDelete() 
{
	UpdateData(TRUE);

	g_pView->ShowSystemMsg(DATA_DELETE_PROCESSING_MSG);

	ResultData_Delete((LONG)m_nPeriod);

	g_pView->CloseSystemMsg();
}

//+add kwmoon 080415
void COptGrabberPage::MakeModelFolderList(CString szModuleFolder,CStringArray& aModelFoler)
{
	CFileFind find;

	BOOL bFind		 = FALSE;
	CString szErrMsg = _T("");
	CString szModelFolderName = szModuleFolder + "\\*.*";

	if(!(bFind = find.FindFile(szModelFolderName)))
	{
		szErrMsg.Format("Failed to find <%d> folder!",szModuleFolder); 
		AfxMessageBox(szErrMsg); return;
	}  

	while(bFind)
	{
		bFind = find.FindNextFile();

		szModelFolderName = find.GetFileName();
		
		if (find.IsDirectory())
		{
			if (!find.IsDots())
			{
				szModelFolderName = find.GetFileName();

				aModelFoler.Add(szModelFolderName);
			}
		}
	}
	find.Close();
}

//+add kwmoon 080415
BOOL COptGrabberPage::DeleteFolder(CString szGrabFolderPath) 
{
	CFileFind find;

	BOOL bFind			 = FALSE;
	BOOL bSuccess		 = FALSE;
	
	CString szErrMsg	 = _T("");
	CString szFileName	 = _T("");
	CString szFilePath	 = _T("");

	CString szFolderName = _T("");
	CString szFolderPath = _T("");
	CString szGrabFolderFile = szGrabFolderPath + "\\*.*";

	DWORD dwErrCode		= 0;

//	szGrabFolderPath	+= "\\";

	if((bFind = find.FindFile(szGrabFolderFile)))
	{	
		while(bFind)
		{
			bFind = find.FindNextFile();
		
			szFileName = find.GetFileName();
			szFilePath = szGrabFolderPath + "\\" + szFileName;
	
			if(!find.IsDirectory())
			{
				m_pProgressMsgDlg->ShowMessage2(szFileName);

				bSuccess = ::DeleteFile((LPCTSTR)szFilePath);
			}
			else
			{
				if((szFileName != ".") && (szFileName != ".."))
				{
					DeleteFolder(szFilePath); 
				}
			}
		}	
		find.Close();

		if(!(bSuccess = ::RemoveDirectory((LPCTSTR)szGrabFolderPath)))
		{
			dwErrCode = GetLastError();
		}
		return TRUE;
	}  
	find.Close();
	return FALSE;
}

//+add kwmoon 080502
void COptGrabberPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnGrabberOptApply();

	CPropertyPage::OnOK();
}

//+add kwmoon 080904
void COptGrabberPage::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	//+add kwmoon 080910
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	
	CString sMsg = _T("");
	
	OnBtnGrabberOptApply();

	//+move kwmoon 080904
	if(FileExists(CurrentSet->sModelIni))
	{
		SaveModelIniFile(CurrentSet->sModelIni);
	}
	else
	{
		CreateModelIniFile(CurrentSet->sModelIni);
		sMsg.Format("File is created.\n[%s]",CurrentSet->sModelIni);
		AfxMessageBox(sMsg);
	}
}

void COptGrabberPage::OnSelchangeComboPeriod() 
{
	int nTemp;

	nTemp = m_ctrlPeriod.GetCurSel();

	m_nPeriod = (nTemp + 1) * 15;
}

	//+add psh 100310
void COptGrabberPage::OnHdmiResolution1() 
{
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(TRUE);
}

void COptGrabberPage::OnHdmiResolution2() 
{
	m_ctrlHDMI_BitSpec.SetCurSel(0);
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
	
}

void COptGrabberPage::OnHdmiResolution3() 
{
	m_ctrlHDMI_BitSpec.SetCurSel(0);
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
}

void COptGrabberPage::OnHdmiResolution4() 
{
	m_ctrlHDMI_BitSpec.SetCurSel(0);
	GetDlgItem(IDC_HDMI_BIT_SPEC)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_HDMI_SPEC)->EnableWindow(FALSE);
}
