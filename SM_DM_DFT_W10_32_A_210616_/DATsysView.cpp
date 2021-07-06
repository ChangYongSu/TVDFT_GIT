// DATsysView.cpp : implementation of the CDATsysView class
//C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib;

#include <Afxmt.h>
#include <malloc.h>
#include <conio.h>
#include <direct.h>

#include "stdafx.h"
#include "DATsys.h"
#include "DATsysDoc.h"
#include "DATsysView.h"
#include "global.h"
#include "parser.h"
#include "step.h"
#include "jumpstep.h"
#include "ImageProc.h"
#include "BaseImage.h"
#include "PatternGeneratorCtl.h"
#include "AvSwitchBoxCtrl.h"
#include "ScannerCtrl.h"
#include "TvCommCtrl.h"
#include "IniAccess.h"
#include "ImageTotalDlg.h"
#include "SeqEditorDlg.h"
#include "ProgressCtrlX.h"
//#include "MirProgress.h"
#include "OptionDlg.h"
#include "StopProcessingDlg.h"
//#include "CheckDeviceDlg.h"
#include "SetModelDlg.h"
#include "CurrentSetting.h"
#include "NoDifferentPointDlg.h"
//#include "joystimm.h"
//#include "thread.h"

//#include "MesDb.h"
#include "Gmes.h"

#include <mmsystem.h>
#include "UserMsgDlg.h"
#include "SoundCard.h"
#include "RealMeas.h"
#include "IrCheckerCtrl.h"

//+add kwmoon 081024
#include "SelectChassisModelDlg.h"

//+add psh 090120
#include "ModelInfoCreate.h"
//+add psh 090910
#include "Keydll1.h"//Include this for functions in the DLL.
#include "Keydll2.h"//Include this for functions in the DLL.

#include "thread.h"

#include "UsbDio.h"
#include "UsbdioTestDlg.h"
#include "Markup.h"
#include "PciDio.h"
#include "thermometer.h"


#include "SM_SoundControl.h"
#include "FFT_SM.h"


#include "CSelectGenerateModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//+add psh 090702
using namespace std;

//////////////////////
// Global Variables //
//////////////////////

CColorStaticST	ctrlFixtureID;
CColorStaticST	ctrlTestStep;
CColorStaticST  ctrlTestSubStep;
CColorStaticST	ctrlStepTarget_V;
CColorStaticST  ctrlStepMeasur_V;
CColorStaticST	ctrlStepTarget_A;
CColorStaticST  ctrlStepMeasur_A;

CColorStaticST  ctrlWipIDLabel;

CProgressCtrlX	ctrlTestProgress;



int				g_nProgressPos;
int				g_nNoTested;
int				g_nFailed;

//+add kwmoon 081004
int				g_nUserStop;

//+add PSH 081218
int				g_nRetryStop;
int				g_nUserRetry;
int				g_nNormalFailed;

double			g_dRate;	
int				g_nNoFailedStep;
int				g_nStepSize;

CString			sEditRemocon;
CString			sModel;
CString			sSeq;
CString			sRemocon;
CString			sTested;
CString			sFailed;
//+add kwmoon 081004
CString			sUserStop;

CString			sRate;
CString			sStatus;
CString			sTime;
CString			sMode;
clock_t			Start;
clock_t			StepStart;

CImageProc		g_ImageProc;
CBaseImage		g_GrabImage;
CBaseImage		g_RefImage;
CBaseImage		g_MaskImage;

CDATsysView *	g_pView;

UINT			m_nSelectedImage;	//0 :  SNAP_IMAGE	1 : LOAD_IMAGE   2 : PROC_IMAGE, 3: INTERNAL_PROC_IMAGE

CProgressCtrlX	ctrlImgProg;

CIniAccess		m_Ini;				// IniAccess Class


UINT			nDefectCurSel;
CEdit			ctrlWipIdEdit;			
CEdit			ctrlSuffixEdit;			
CString			m_strWipId;			//Wip ID
CTime			m_tStart;			//Test Start Time

//+add kwmoon 080514
//CSoundCard		g_SoundCard;

//CEdit ctrlToolOption1;
//CEdit ctrlToolOption2;
//CEdit ctrlToolOption3;
//CEdit ctrlToolOption4;
//091016
//CEdit ctrlToolOption5;
//CEdit ctrlAreaOption1;

//CEdit ctrlCPUVersion;
//CEdit ctrlMicomVersion;
//CEdit ctrlUSBVersion;


//+add kwmoon 080502
//CEdit			ctrlToolOptionTitle;			
//CEdit			ctrlAreaOptionTitle;			
CEdit			ctrlAudioLTitle;			
CEdit			ctrlAudioRTitle;			
//CEdit			ctrlAudioLLevel;			
//CEdit			ctrlAudioLFreq;			
//CEdit			ctrlAudioRLevel;			
//CEdit			ctrlAudioRFreq;			

//CEdit			ctrlCPUVersionTitle;			
//CEdit			ctrlMicomVersionTitle;			
//CEdit			ctrlUSBVersionTitle;			

CCurrentSetting	g_CurSetting;

HWND			g_hCommWnd;
BYTE			GrabData[1920][1080][3];

int				g_nRunningProcessNo;

//HANDLE			hClientEvents[8]; 
//HANDLE			hServerEvents[6]; 

CString			g_WndClassName;
//+add 090213(Modification No1)
//extern CStdioFile		g_LogFile;
//BOOL	g_LogFileOpen;

//+add PSH 081202
//CCriticalSection g_CriticalSection;
CRITICAL_SECTION g_cs;  //psh추가
CRITICAL_SECTION g_cs2;  //add 090818
CRITICAL_SECTION g_cs4;  //add 130913


CStringArray g_Divisions;

CString			g_sData_MD5;



int		gJigStatus;		// Jig의 UP/Down 상태

////////////////////////
// External Variables //
////////////////////////

extern _InputSource InputDefined[];
//extern UINT	g_GrabSource;

//+add 090409(Modification No1)
extern int g_nCurSwitchBoxLogLine;
extern int g_nCurRemoteLogLine;

/////////////////////////////////////////////////////////////////////////////
// CDATsysView

IMPLEMENT_DYNCREATE(CDATsysView, CFormView)

BEGIN_MESSAGE_MAP(CDATsysView, CFormView)
	//{{AFX_MSG_MAP(CDATsysView)
	ON_COMMAND(ID_EDIT_COMPILE, OnEditCompile)
	ON_COMMAND(ID_EDIT_CURRENT, OnEditCurrent)
	ON_COMMAND(ID_MODEL_OPEN, OnModelOpen)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_RUN_ABORT, OnRunAbort)
	ON_COMMAND(ID_RUN_NEXT, OnRunNext)
	ON_COMMAND(ID_RUN_JUMP, OnRunJump)
	ON_COMMAND(ID_RUN_PREV, OnRunPrev)
	ON_COMMAND(ID_RUN_RUN, OnRunRun)
	ON_WM_TIMER()
	ON_COMMAND(ID_IMAGE_DIALOG, OnImageDialog)
	ON_COMMAND(ID_SEQ_EDIT, OnSeqEdit)
	ON_COMMAND(ID_OPTION, OnOption)
	ON_COMMAND(ID_RUN_MAKE_REF, OnRunMakeRef)
	ON_COMMAND(ID_RUN_ADJ_SPEC, OnRunAdjSpec)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_PWR_ON, OnPwrOn)
	ON_COMMAND(ID_PWR_OFF, OnPwrOff)
	ON_COMMAND(ID_REF_IMAGE, OnRefImage)
	ON_COMMAND(ID_REF_AUDIO, OnRefAudio)
	ON_COMMAND(ID_GRAB_START, OnGrabStart)
	ON_COMMAND(ID_GRAB_STOP, OnGrabStop)
	ON_COMMAND(ID_ENTER, OnEnter)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_COMMAND(ID_MES_DATA_UPLOAD, OnMesDataUpload)
	ON_COMMAND(ID_MODEL_REGISTRATION, OnModelRegistration)
	ON_COMMAND(ID_AUDIO_MEAS_START, OnAudioMeasStart)
	ON_COMMAND(ID_AUDIO_MEAS_STOP, OnAudioMeasStop)
	ON_COMMAND(ID_SCANNER_ENABLE, OnScannerEnable)
	ON_COMMAND(ID_SCANNER_DISABLE, OnScannerDisable)
	ON_NOTIFY(TCN_SELCHANGE, IDC_VIEW_TAB, OnSelchangeViewTab)
	ON_BN_CLICKED(IDC_SEQ_LOCK, OnSeqLock)
	ON_WM_CLOSE()
	ON_COMMAND(ID_INPUT, OnInput)
	ON_COMMAND(ID_AUDIO_OUTL_START, OnAudioOutlStart)
	ON_COMMAND(ID_AUDIO_OUTL_STOP, OnAudioOutlStop)
	ON_COMMAND(ID_AUDIO_OUTR_START, OnAudioOutrStart)
	ON_COMMAND(ID_AUDIO_OUTR_STOP, OnAudioOutrStop)
	ON_COMMAND(ID_JIG_UP, OnJigUp)
	ON_COMMAND(ID_JIGUP_ALL, OnJigupAll)
	ON_COMMAND(ID_JIGUP_GOOD, OnJigupGood)
	ON_COMMAND(ID_JIGUP_NOT, OnJigupNot)
	ON_COMMAND(ID_START_AUTO, OnStartAuto)
	ON_COMMAND(ID_START_MANUAL, OnStartManual)
	ON_MESSAGE(WM_KEYSTROKE, ProcessKeyPad)
	ON_BN_CLICKED(IDC_CHECK_MESON, OnCheckMeson)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_MESSAGE(UM_UPDATE_DISPLAY, UpdateDisplay)
	ON_MESSAGE(UM_CLOSE_STOP_PROCESSING_DLG, CloseStopProcessingDlg)
	ON_MESSAGE(UM_RESULT_DISPLAY, ShowResultDisplay)
	ON_MESSAGE(UM_RUN_TEST, RunTest)
	ON_MESSAGE(UM_INIT_DIALOG, InitDeviceDialog)
	ON_MESSAGE(UM_CONTINUOUS_LVDS_GRAB, RunContinuousLVDSGrab)
	ON_MESSAGE(WM_COMM_READ , OnCommunication)
	ON_MESSAGE(UM_UPLOAD_MES_DATA,OnUploadMesData)
	ON_MESSAGE(UM_CLOSE_RESULT_DLG, CloseResultDlg)
	ON_MESSAGE(UM_UPDATE_GRAB_STATUS, UpdateGrabImage)

	ON_MESSAGE(WM_CHANGE_MODEL , OnChangeModelOpen)
	ON_MESSAGE(WM_GRAPH_UPDATE, OnUpdateGraph)
	//+add 090220(Modification No3)
//	ON_MESSAGE(UM_START_LVDS_GRAB_THREAD, StartLVDSGrabThread)
	//ON_CBN_SELCHANGE(IDC_COMBO_SOUND_L, OnSelchangeCmbSoudDevice)
	
ON_STN_CLICKED(IDC_STEPRESULT, &CDATsysView::OnStnClickedStepresult)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MAIN_PROCESS, &CDATsysView::OnNMCustomdrawListMainProcess)
ON_NOTIFY(HDN_ITEMCLICK, 0, &CDATsysView::OnHdnItemclickListMainProcess)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_VERSION_PROCESS, &CDATsysView::OnNMCustomdrawListVersionProcess)
ON_NOTIFY(NM_CLICK, IDC_LIST_MAIN_PROCESS, &CDATsysView::OnNMClickListMainProcess)
ON_WM_LBUTTONDOWN()
ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAIN_PROCESS, &CDATsysView::OnNMDblclkListMainProcess)
//ON_CBN_EDITCHANGE(IDC_COMBO_SOUND_L, &CDATsysView::OnCbnEditchangeComboSoundL)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDATsysView construction/destruction

CDATsysView::CDATsysView()
	: CFormView(CDATsysView::IDD)//, m_EditStrBuf( m_ctrlComLog ), m_EditStrBufW( m_ctrlComLog )
{
	//{{AFX_DATA_INIT(CDATsysView)
	m_bSeq_Lock = FALSE;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	m_hGrabThreadKillEvent	= NULL;
	m_pSystemMessageDlg		= NULL;

	m_pFrame				= NULL;
	CurrentSet				= NULL;
	m_pVideoViewWnd			= NULL;

	m_bGetRectSizeInfo		= FALSE;
	m_bTerminalModeOn		= FALSE;
	m_bThreadRunning		= FALSE;

	m_bGrabThreadRunning	= FALSE;
	m_bStopLVDSThread		= FALSE;
	m_bAdcRunning			= FALSE;
	m_pStopProcessingDlg	= NULL;

	m_nNoRepeatExecution		= 1;
	m_nNoCurrentRepeatExecution = 0;
	m_nNoPassInRepeatExecution	= 0;
	m_nNoFailInRepeatExecution	= 0;

	m_szResultMsg1			= _T("");
	m_szResultMsg2			= _T("");

	m_nCurrentViewImageType	= 0;

	m_bMakeReferenceMode	= FALSE;
	m_bAdjSpecMode			= FALSE;
	m_szExePath				= _T("");
	m_szNoImageBmpPath		= _T("");
	m_szMakeRefPath			= _T("");

	m_bContinuousLVDSGrabRunning = FALSE;
	
	m_bUseDefaultREFfolder	= FALSE;

	//+add psh 080603
	m_bResultDlgActivate	= FALSE;

	m_nGrabType				= 0;
	m_nGrabResult			= 0;
	m_bStopMeasureAudio		= FALSE;
	m_bTimeDataFileOpen		= FALSE;
	m_dwStepElapsedTime		= 0;
	m_szVersion				= _T("");

	//+change 090213(Modification No1)
//	m_pGrabImageThread		= NULL;
	m_pTestThread			= NULL;


	InitializeCriticalSection(&g_cs);//psh추
	InitializeCriticalSection(&g_cs2);//add 090818
	InitializeCriticalSection(&g_cs4);//add 130913
	m_nOldFailedStep		= 0;
	m_bExitFlag				= FALSE;

	m_bResult_S6F5			= FALSE;
	m_bResult_S6F12			= FALSE;
	m_bResult_S6F2			= FALSE;

	m_strFixtureId			= _T("");

	m_pBaseThread = NULL;

}

CDATsysView::~CDATsysView()
{

	m_bmpNormal.DeleteObject();
	m_bmpGood.DeleteObject();
	m_bmpFail.DeleteObject();
	m_bmpRun.DeleteObject();

	DeleteCriticalSection(&g_cs);//psh추
	DeleteCriticalSection(&g_cs2);//add 090818
	DeleteCriticalSection(&g_cs4);
/*
//	CoFreeUnusedLibraries();
    // Free the digitizer control
    //
    if(m_ActiveMILDigitizer)
    {
        if(m_ActiveMILDigitizer->IsAllocated)
        {
            m_ActiveMILDigitizer->Free();
        }
        m_ActiveMILDigitizer = NULL;
    }

    // Free the system control
    //
    if(m_ActiveMILSystem)
    {
        if(m_ActiveMILSystem->IsAllocated)
        {
            m_ActiveMILSystem->Free();
        }
        m_ActiveMILSystem = NULL;
    }

    // Free the application
    //
    if(m_ActiveMILApplication)
    {
        m_ActiveMILApplication = NULL;
    }
*/
}

void CDATsysView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDATsysView)
	DDX_Control(pDX, IDC_COM_DEBUG_LOG, m_ctrlEditComLog);
	DDX_Control(pDX, IDC_VIEW_TAB, m_ctrlViewTab);
	DDX_Check(pDX, IDC_SEQ_LOCK, m_bSeq_Lock);
	//	DDX_Control(pDX, IDC_MAINVIEW_GRID, m_ctrlSummaryGrid);
	//	DDX_Control(pDX, IDC_VERSION_GRID, m_ctrlVersionGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_MESON, m_LedsMesOn);
	//	DDX_Control(pDX, IDC_MIL_DISPLAY1, m_ActiveMILDisplay);
	DDX_Control(pDX, IDC_LIST_MAIN_PROCESS, m_CtrlListMainProcess);
	DDX_Control(pDX, IDC_LIST_VERSION_PROCESS, m_CtrlListVersionProcess);
	///	DDX_Control(pDX, IDC_LABEL_SOUND_LEFT_INFO, m_cLabelSoundLeft);
	//	DDX_Control(pDX, IDC_LABEL_SOUND_RIGHT_INFO, m_cLabelSoundRight);
	//DDX_Control(pDX, IDC_COMBO_SOUND_L, m_cComboSoundLeft);
	//DDX_Control(pDX, IDC_COMBO_SOUND_R, m_cComboSoundRight);
	DDX_Control(pDX, IDC_REDIT_COM_DEBUG, m_ctrlEditComLog1);
}

BOOL CDATsysView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	return CFormView::PreCreateWindow(cs);
}

//+add kwmoon 080528
void CDATsysView::InitSubClassItem()
{
	COLORREF crBackColor = RGB(225, 225, 198);
	COLORREF crPushColor = RGB(238, 239, 231);

	m_stcModelFolder.SubclassDlgItem(IDC_MODEL_FOLDER_STATIC, this);
	m_stcModelFolder.SetBkColor(crBackColor); 
	m_stcModelFolder.SetTextColor(RGB(0,0,0));  
//	m_stcModelFolder.SetPath (TRUE);

	m_stcRefFolder.SubclassDlgItem(IDC_REF_FOLDER_STATIC, this);
	m_stcRefFolder.SetBkColor(crBackColor); 
	m_stcRefFolder.SetTextColor(RGB(0,0,0));  
//	m_stcRefFolder.SetPath (TRUE);

	m_stcChassisName.SubclassDlgItem(IDC_CHASSISNAME, this);
	m_stcChassisName.SetBkColor(crBackColor); 
	m_stcChassisName.SetTextColor(RGB(0,0,0));  

	m_stcSeqName.SubclassDlgItem(IDC_SEQFILENAME, this);
	m_stcSeqName.SetBkColor(crBackColor); 
	m_stcSeqName.SetTextColor(RGB(0,0,0));  

	m_stcMode.SubclassDlgItem(IDC_TESTMODE, this);
	m_stcMode.SetBkColor(RGB(255,255,255)); 

	m_stcStatus.SubclassDlgItem(IDC_CURRENTSTATUS, this);
	m_stcStatus.SetBkColor(RGB(0,0,255)); 
	m_stcStatus.SetTextColor(RGB(255,255,255)); 

	m_stcTime.SubclassDlgItem(IDC_TESTTIME, this);
	m_stcTime.SetBkColor(crBackColor); 
	m_stcTime.SetTextColor(RGB(0,0,0));  

	m_ctrLEDTested.SubclassDlgItem(IDC_TESTNUMBER, this);
	m_ctrLEDFailed.SubclassDlgItem(IDC_FAILNUMBER, this);
	m_ctrLEDRate.SubclassDlgItem(IDC_FAILRATE, this);

	//+add kwmoon 071015 : [2in1]
	m_ctrLEDTested.SetBkColor(RGB(189,231,97));
	m_ctrLEDTested.SetTextColor(RGB(0,0,0));  
	m_ctrLEDFailed.SetBkColor(RGB(189,231,97));
	m_ctrLEDFailed.SetTextColor(RGB(0,0,0));  
	m_ctrLEDRate.SetBkColor(RGB(189,231,97));
	m_ctrLEDRate.SetTextColor(RGB(0,0,0));  
	//-

//	ctrlSummaryGrid.SubclassDlgItem(IDC_MAINVIEW_GRID, this);

	ctrlTestProgress.SubclassDlgItem(IDC_TEST_PROGRESS, this);
	ctrlTestProgress.SetTextColor(RGB(255,128,64),RGB(255,255,255));

	ctrlTestProgress.SetGradientColors(RGB(0,0,255),RGB(0,0,255));
	ctrlTestProgress.SetPos(0);
	ctrlTestProgress.SetShowPercent();
	ctrlTestProgress.SetTiedText(TRUE);

	ctrlImgProg.SubclassDlgItem(IDC_IMG_PROC_PROG, this);
	ctrlImgProg.SetBkColor(DARK_BLACK_COLOR);
	ctrlImgProg.SetGradientColors(LIGHT_BLUE_COLOR,DARK_BLUE_COLOR);
	ctrlImgProg.SetPos(0);

	ctrlTestStep.SubclassDlgItem(IDC_TESTSTEP,this);
	ctrlTestSubStep.SubclassDlgItem(IDC_TEST_SUBSTEP, this);

	ctrlTestStep.SetBkColor(RGB(189,231,97)); 
	ctrlTestStep.SetTextColor(RGB(0,0,0));  

	ctrlTestSubStep.SetBkColor(RGB(189,231,97)); 
	ctrlTestSubStep.SetTextColor(RGB(0,0,0));  

	ctrlTestSubStep.SetWindowText(_T(""));

	ctrlFixtureID.SubclassDlgItem(IDC_FIXTURE_ID,this);
	ctrlFixtureID.SetBkColor(RGB(195,231,197)); 
	ctrlFixtureID.SetTextColor(RGB(0,0,0));  

	ctrlStepTarget_V.SubclassDlgItem(IDC_STEP_VIDEO_T,this);
	ctrlStepMeasur_V.SubclassDlgItem(IDC_STEP_VIDEO_M,this);
	ctrlStepTarget_A.SubclassDlgItem(IDC_STEP_AUDIO_T,this);
	ctrlStepMeasur_A.SubclassDlgItem(IDC_STEP_AUDIO_M,this);

	ctrlStepTarget_V.SetBkColor(RGB(189,231,97)); 
	ctrlStepTarget_V.SetTextColor(RGB(0,0,0));  
	ctrlStepMeasur_V.SetBkColor(RGB(189,231,97)); 
	ctrlStepMeasur_V.SetTextColor(RGB(0,0,0));  
	ctrlStepTarget_A.SetBkColor(RGB(189,231,97)); 
	ctrlStepTarget_A.SetTextColor(RGB(0,0,0));  
	ctrlStepMeasur_A.SetBkColor(RGB(189,231,97)); 
	ctrlStepMeasur_A.SetTextColor(RGB(0,0,0));  

	ctrlWipIDLabel.SubclassDlgItem(IDC_STATIC_WIP_ID_LABEL,this);
	ctrlWipIDLabel.SetBkColor(RGB(195,231,197));
	ctrlWipIDLabel.SetTextColor(RGB(0,0,0));

	ctrlStepTarget_V.SetWindowText(_T(""));
	ctrlStepMeasur_V.SetWindowText(_T(""));
	ctrlStepTarget_A.SetWindowText(_T(""));
	ctrlStepMeasur_A.SetWindowText(_T(""));

	ctrlWipIdEdit.SubclassDlgItem(IDC_EDIT_WIPID, this);
	ctrlWipIdEdit.SetWindowText(_T(""));
	m_strWipId = _T("");

	ctrlSuffixEdit.SubclassDlgItem(IDC_EDIT_SUFFIX, this);
	ctrlSuffixEdit.SetWindowText(_T(""));

	//+add kwmoon 080502
//	ctrlToolOptionTitle.SubclassDlgItem(IDC_EDIT_TOOL_OPTION_TITLE, this);
//	ctrlToolOptionTitle.SetWindowText(_T("TOOL OPTION"));
//	ctrlAreaOptionTitle.SubclassDlgItem(IDC_EDIT_AREA_OPTION_TITLE, this);
//	ctrlAreaOptionTitle.SetWindowText(_T("AREA OPTION"));
	//ctrlAudioLTitle.SubclassDlgItem(IDC_EDIT_AUDIO_L_TITLE, this);
	//ctrlAudioLTitle.SetWindowText(_T("AUDIO-L"));
	//ctrlAudioRTitle.SubclassDlgItem(IDC_EDIT_AUDIO_R_TITLE, this);
	//ctrlAudioRTitle.SetWindowText(_T("AUDIO-R"));

//	ctrlCPUVersionTitle.SubclassDlgItem(IDC_EDIT_CPU_VERSION_TITLE, this);
//	ctrlCPUVersionTitle.SetWindowText(_T("CPU Ver"));
//	ctrlMicomVersionTitle.SubclassDlgItem(IDC_EDIT_MICOM_VERSION_TITLE, this);
//	ctrlMicomVersionTitle.SetWindowText(_T("MICOM Ver"));
//	ctrlUSBVersionTitle.SubclassDlgItem(IDC_EDIT_USB_VERSION_TITLE, this);
//	ctrlUSBVersionTitle.SetWindowText(_T("USB Ver"));


	
//	ctrlToolOption1.SubclassDlgItem(IDC_TOOL_OPTION1, this);
//	ctrlToolOption2.SubclassDlgItem(IDC_TOOL_OPTION2, this);
//	ctrlToolOption3.SubclassDlgItem(IDC_TOOL_OPTION3, this);
//	ctrlToolOption4.SubclassDlgItem(IDC_TOOL_OPTION4, this);
	//091016
//	ctrlToolOption5.SubclassDlgItem(IDC_TOOL_OPTION5, this);

//	ctrlAreaOption1.SubclassDlgItem(IDC_AREA_OPTION1, this);

//	ctrlCPUVersion.SubclassDlgItem(IDC_CPU_VERSION, this);
//	ctrlMicomVersion.SubclassDlgItem(IDC_MICOM_VERSION, this);
//	ctrlUSBVersion.SubclassDlgItem(IDC_USB_VERSION, this);

	m_LedsMesOn.SetIcons(IDI_LEDOFF, IDI_LEDON);
	m_LedsMesOn.SetTextColors(0, COLOR_DARKBLUE, COLOR_LIGHTGRAY); //RGB(238, 239, 231));
	m_LedsMesOn.SetTextColors(1, COLOR_BLUE, COLOR_LIGHTGRAY); //RGB(238, 239, 231));


	m_cLabelSoundLeftFREQ.SubclassDlgItem(IDC_TEXT_FREQ_LEFT, this);//IDC_TEXT_FREQ_LEFT // ctrlPLC_COM_Status.SubclassDlgItem(IDC_PLC_COM_STATUS,this);
	m_cLabelSoundLeftVolt.SubclassDlgItem(IDC_TEXT_VOLT_LEFT, this);//IDC_TEXT_VOLT_LEFT
	m_cLabelSoundRightFREQ.SubclassDlgItem(IDC_TEXT_FREQ_RIGHT, this);//IDC_TEXT_FREQ_RIGHT
	m_cLabelSoundRightVolt.SubclassDlgItem(IDC_TEXT_VOLT_RIGHT, this);//IDC_TEXT_VOLT_RIGHT

	m_cLabelSoundLeftFREQ.SetWindowText(_T("0 Hz"));
	m_cLabelSoundLeftFREQ.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundLeftFREQ.SetTextColor(RGB(0, 255, 0));

	m_cLabelSoundLeftVolt.SetWindowText(_T("0 mV"));
	m_cLabelSoundLeftVolt.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundLeftVolt.SetTextColor(RGB(0, 255, 0));

	m_cLabelSoundRightFREQ.SetWindowText(_T("0 Hz"));
	m_cLabelSoundRightFREQ.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundRightFREQ.SetTextColor(RGB(0, 255, 0));

	m_cLabelSoundRightVolt.SetWindowText(_T("0 mV"));
	m_cLabelSoundRightVolt.SetBkColor(RGB(0, 0, 0));
	m_cLabelSoundRightVolt.SetTextColor(RGB(0, 255, 0));


}

void CDATsysView::OnInitialUpdate()
{
	CRect rect;
	// 090910
	CString sMsg		= _T("");
	CString sTemp;

	CFormView::OnInitialUpdate();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_bSeq_Lock = 1;
	UpdateData(FALSE);
	m_pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;

    m_pFrame->GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	if(g_nRunningProcessNo ==1){

		m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	}
	else{
		m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME1));
	}

	// 090909
	//====================
	// Open Sequence File 
	//====================
	if(CurrentSet->sSeqPath.IsEmpty())
	{
		CurrentSet->sSeqPath = "D:\\DM_DFT\\SEQ_18\\SN6x_Test_Install.seq";//'\SEQ_18\SN6x_Test_Install.seq
	}
	//else
	{
		//+add kwmoon 080821 : Add FileExists Check
		if(FileExists(CurrentSet->sSeqPath))
		{
			CurrentSet->bCompiled = Prescan(CurrentSet->sSeqPath);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sSeqPath);
			AfxMessageBox(sMsg);
		}
	}

	//Control Subclassing
	InitSubClassItem();

	TestCountRead(m_szExePath);
	g_dRate		= 0.0;

	sTested.Format("%d", g_nNoTested);
    sFailed.Format("%d", g_nFailed);
	//+add kwmoon 081004
	sUserStop.Format("%d", g_nUserStop);
    
	if(g_nNoTested > 0)
	{
		g_dRate =((double)g_nFailed /(double)g_nNoTested);
		g_dRate = g_dRate*100.0;
		sRate.Format("%4.1f", g_dRate);
	}
	else
	{
		sRate	= _T("0.0");
	}

	SetFont();
	
	// Create Sound Card Object
//	g_pView->Creat(this);

	InitMainGrid(CurrentSet->nDisplayType);
	InitVersionGrid();

	InsertStepData2Grid(CurrentSet->nDisplayType);

	g_pView = this;
	m_pVideoViewWnd	=(CWnd*)(CStatic*) GetDlgItem(IDC_VIDEOVIEW);


	GetRectSizeInfo();

	m_bGetRectSizeInfo = TRUE;

	CRect VideoViewRect;
	m_pVideoViewWnd->GetWindowRect(&VideoViewRect);

	// View Tab
	TC_ITEM item;

	item.mask = TCIF_TEXT;

	item.pszText = "Grab";
	m_ctrlViewTab.InsertItem(0,&item);
	
	item.pszText = "Reference";
	m_ctrlViewTab.InsertItem(1,&item);

	item.pszText = "Processed";
	m_ctrlViewTab.InsertItem(2,&item);

	m_szNoImageBmpPath.Format("%s\\SystemImage\\NoImage.bmp",m_szExePath);

	// Load default image
	if(!m_Image[I_GRAB].LoadImage(m_szNoImageBmpPath, ctrlTestProgress))
	{
		AfxMessageBox("Check <NoImage.bmp> in Work Foder\\SystemImage");
		PostMessage(WM_CLOSE); return;
	}

	if(!m_Image[I_REF].LoadImage(m_szNoImageBmpPath, ctrlTestProgress))
	{
		AfxMessageBox("Check <NoImage.bmp> in Work Foder\\SystemImage");
		PostMessage(WM_CLOSE); return;
	}

	if(!m_Image[I_MASK].LoadImage(m_szNoImageBmpPath, ctrlTestProgress))
	{
		AfxMessageBox("Check <NoImage.bmp> in Work Foder\\SystemImage");
		PostMessage(WM_CLOSE); return;
	}

	if(!m_Image[I_PROC].LoadImage(m_szNoImageBmpPath, ctrlTestProgress))
	{
		AfxMessageBox("Check <NoImage.bmp> in Work Foder\\SystemImage");
		PostMessage(WM_CLOSE); return;
	}


	//m_ctrlComLog.SetEditCtrl( m_ctrlEditComLog.m_hWnd );
	// Redirect cout to our Edit-Control
	//m_pOldBuf = cout.rdbuf( &m_EditStrBuf );
	// and also the unicode version
	//m_pOldBufW = wcout.rdbuf( &m_EditStrBufW );

	Invalidate();  

	SetTimer(TIMER_UPDATE_STATUS_INFO, 400, NULL);


	//+add 090409(Modification No1)
	g_nCurSwitchBoxLogLine = 0;
	g_nCurRemoteLogLine	   = 0;

	PostMessage(UM_INIT_DIALOG,0,0);

	ctrlSuffixEdit.SetReadOnly(TRUE);
	ctrlWipIdEdit.SetFocus();

	//+add kwmoon 080618
	m_hGrabThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);

//	SetGrabInfo(&g_GrabImage);

	GetApplicationVersion(); 

	m_cStepResult.SubclassDlgItem(IDC_STEPRESULT, this);

	m_bmpNormal.LoadBitmap(IDB_BITMAP_NOMAL);
	m_bmpGood.LoadBitmap(IDB_BITMAP_GOOD);
	m_bmpFail.LoadBitmap(IDB_BITMAP_FAIL);
	m_bmpRun.LoadBitmap(IDB_BITMAP_RUN);

	m_cStepResult.SetBitmap(m_bmpNormal);

/*	UpdateInfo(TOOL_OPTION1,CurrentSet->sToolOption1);
	UpdateInfo(TOOL_OPTION2,CurrentSet->sToolOption2);
	UpdateInfo(TOOL_OPTION3,CurrentSet->sToolOption3);
	UpdateInfo(TOOL_OPTION4,CurrentSet->sToolOption4);
	UpdateInfo(TOOL_OPTION5,CurrentSet->sToolOption5);
	UpdateInfo(AREA_OPTION1,CurrentSet->sAreaOption1);
	UpdateInfo(CPU_VERSION,CurrentSet->sCPUVersion);
	UpdateInfo(MICOM_VERSION,CurrentSet->sMicomVersion);
	UpdateInfo(USB_VERSION,CurrentSet->sUSBVersion);
*/
	ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);

	UpdateVersionInfo();

	CurrentSet->bRunAbort	 = FALSE;

	if(CurrentSet->nRunStartType == AUTO_START){
		if(g_nRunningProcessNo == 1)
		{
			sTemp = sTemp + "1(" + m_szVersion + ")-Auto Start";
		}
		else{
			sTemp = sTemp + "2(" + m_szVersion + ")-Auto Start";
		}
	}
	else{
		if(g_nRunningProcessNo == 1)
		{
			sTemp = sTemp + "1(" + m_szVersion + ")-Manual Start";
		}
		else{
			sTemp = sTemp + "2(" + m_szVersion + ")-Manual Start";
		}
	}
	AfxGetMainWnd()->SetWindowText(sTemp);

	UpdateData(FALSE);




	if((g_hCommWnd = GetSafeHwnd()) == NULL)
	{
		AfxMessageBox("View Handle Null!");
	}

	m_hReadEvent_S6F5 = ::CreateEvent(NULL,false,false,NULL);
	m_hReadEvent_S6F12 = ::CreateEvent(NULL,false,false,NULL);
	m_hReadEvent_S6F2 = ::CreateEvent(NULL,false,false,NULL);

//	AudioMeasureStart();

	SetTimer(TIMER_CONTINUOUS_LVDS_GRAB,1000,NULL);

	/***************/
	/* ADDED CODE: */
	/***************/
	//CRect rect;

	//GetDlgItem(IDC_STATIC_INST_DISPLAY)->SetWindowPos(NULL, 50,50, 1200, 400,  SWP_ASYNCWINDOWPOS );//.SetWindowPos(NULL,367, 107,228,28,SWP_ASYNCWINDOWPOS);

	GetDlgItem(IDC_STATIC_INST_DISPLAY)->GetWindowRect(rect);
	ScreenToClient(rect);

	////m_oglWindow = new COpenGLControl();
	//// Create OpenGL Control window
	//m_oglWindow.oglCreate(rect, this,"OpenGL");
	//// Setup the OpenGL Window's timer to render
	//m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 100, 0);
	m_poglWindow = new COpenGLControl();
	m_poglWindow->oglCreate(rect, this,"OpenGL");
	//// Setup the OpenGL Window's timer to render
	m_poglWindow->m_unpTimer = m_poglWindow->SetTimer(1, 100, 0);

	m_Sound_Display_Enable = 1;
	
	m_sm_sound_ctrl.SelectDevice( 0);
	if (m_sm_sound_ctrl.m_aStrSoundDevice.GetSize() > 0)
	{
		//for (int i = 0; i < m_sm_sound_ctrl.m_aStrSoundDevice.GetSize(); i++)
		//{
		//	m_cComboSoundLeft.AddString(m_sm_sound_ctrl.m_aStrSoundDevice.GetAt(i));
		//}
	}
	if ((m_sm_sound_ctrl.m_aStrSoundDevice.GetSize() > CurrentSet->nSoundInDeviceID)
		&&(0 <= CurrentSet->nSoundInDeviceID))
	{
//		m_cComboSoundLeft.SetCurSel(CurrentSet->nSoundInDeviceID);
		
		m_sm_sound_ctrl.StartCapture(this, CurrentSet->nSoundInDeviceID);
	}
	else
	{
		if (m_sm_sound_ctrl.m_aStrSoundDevice.GetSize() >= 2)
		{
			if (g_nRunningProcessNo == 1)
			{
				CurrentSet->nSoundInDeviceID = 1;
			}
			else
			{
				CurrentSet->nSoundInDeviceID = 0;
			}
	//		m_cComboSoundLeft.SetCurSel(CurrentSet->nSoundInDeviceID);

			m_sm_sound_ctrl.StartCapture(this, CurrentSet->nSoundInDeviceID);
		}
		else
		{	//		m_cComboSoundLeft.SetCurSel(-1);
			sTemp.Format(_T("Sound Card ID(%d) Select Error"), CurrentSet->nSoundInDeviceID);
			MessageBox("Sound Card Select Error");
		}
	}
	//CComboBox m_cComboSoundLeft;
	//CComboBox m_cComboSoundRight;


//	m_cComboCaptureSize.SetCurSel(g_Sample_Buffer_Max);
//	m_SampleRate.SetCurSel(g_Sample_Rate_Per_Second);
//	OnCbnSelchangeComboSamPerSecond();
//	OnCbnSelchangeComboSampleBuffer();

//	m_Sample_Buffer_Max = m_Sample_Buffer_Max;
//	m_Sample_Rate_Per_Second = m_Sample_Rate_Per_Second;




//	 m_cLabelSoundLeft.put_Caption("LEFT");
//	 m_cLabelSoundRight.put_Caption("RIGHT");
}

void CDATsysView::SetFont()
{
	// Set Font1
	LOGFONT logFont1;
	logFont1.lfHeight			= 18;
	logFont1.lfWidth			= 0;
	logFont1.lfEscapement		= 0;
	logFont1.lfOrientation		= 0;
	logFont1.lfWeight			= FW_BOLD;
	logFont1.lfItalic			= 0;
	logFont1.lfUnderline		= 0;
	logFont1.lfStrikeOut		= 0;
	logFont1.lfOutPrecision		= 1;
	logFont1.lfClipPrecision	= 2;
	logFont1.lfQuality			= 1;
	logFont1.lfPitchAndFamily	= 17;
	logFont1.lfCharSet			= 205;
	strcpy(logFont1.lfFaceName,(LPCTSTR)"Courier New");
//	strcpy(logFont1.lfFaceName,(LPCTSTR)"Trebuchet MS");
//	strcpy(logFont1.lfFaceName,(LPCTSTR)"Tahoma");

	m_CustomFont.CreateFontIndirect(&logFont1);

	// Set Font2
	LOGFONT logFont2;
	logFont2.lfHeight			= 32;
	logFont2.lfWidth			= 0;
	logFont2.lfEscapement		= 0;
	logFont2.lfOrientation		= 0;
	logFont2.lfWeight			= FW_BOLD;
	logFont2.lfItalic			= 0;
	logFont2.lfUnderline		= 0;
	logFont2.lfStrikeOut		= 0;
	logFont2.lfOutPrecision		= 1;
	logFont2.lfClipPrecision	= 2;
	logFont2.lfQuality			= 1;
	logFont2.lfPitchAndFamily	= 17;
	logFont2.lfCharSet			= 205;
	strcpy(logFont2.lfFaceName,(LPCTSTR)"Courier New");

	m_StatusWindowFont.CreateFontIndirect(&logFont2);

	// Set Font3
	LOGFONT logFont3;
	logFont3.lfHeight			= 18;
	logFont3.lfWidth			= 12;
	logFont3.lfEscapement		= 0;
	logFont3.lfOrientation		= 0;
	logFont3.lfWeight			= FW_BOLD;
	logFont3.lfItalic			= 0;
	logFont3.lfUnderline		= 0;
	logFont3.lfStrikeOut		= 0;
	logFont3.lfOutPrecision		= 1;
	logFont3.lfClipPrecision	= 2;
	logFont3.lfQuality			= 1;
	logFont3.lfPitchAndFamily	= 17;
	logFont3.lfCharSet			= ANSI_CHARSET;
	strcpy(logFont3.lfFaceName,(LPCTSTR)"System");

	m_ProgressFont.CreateFontIndirect(&logFont3);

	// Apply Font
//	ctrlStepTarget_V.SetFont(&m_CustomFont);
//	ctrlStepMeasur_V.SetFont(&m_CustomFont);
//	ctrlStepTarget_A.SetFont(&m_CustomFont);
//	ctrlStepMeasur_A.SetFont(&m_CustomFont);
	
//	ctrlTestStep.SetFont(&m_CustomFont);
//	m_stcModelFolder.SetFont(&m_CustomFont);
//	m_stcRefFolder.SetFont(&m_CustomFont);
//	m_stcSeqName.SetFont(&m_CustomFont);
//	m_stcMode.SetFont(&m_CustomFont);
//	m_stcTime.SetFont(&m_CustomFont);
//	ctrlTestSubStep.SetFont(&m_CustomFont);
	ctrlWipIdEdit.SetFont(&m_CustomFont);
	ctrlSuffixEdit.SetFont(&m_CustomFont);
//	m_stcStatus.SetFont(&m_StatusWindowFont);
//	m_stcStatus.SetFont(&m_CustomFont);

	//+add kwmoon 080502
//	ctrlToolOptionTitle.SetFont(&m_CustomFont);
//	ctrlAreaOptionTitle.SetFont(&m_CustomFont);
//	ctrlAudioLTitle.SetFont(&m_CustomFont);
//	ctrlAudioRTitle.SetFont(&m_CustomFont);

//	ctrlCPUVersionTitle.SetFont(&m_CustomFont);
//	ctrlMicomVersionTitle.SetFont(&m_CustomFont);
//	ctrlUSBVersionTitle.SetFont(&m_CustomFont);

	//+add kwmoon 071015 : [2in1]
//	m_stcProgramNo.SetFont(&m_StatusWindowFont);
	m_ctrLEDTested.SetFont(&m_CustomFont);
	m_ctrLEDFailed.SetFont(&m_CustomFont);
	m_ctrLEDRate.SetFont(&m_CustomFont);
//	m_stcLFreq.SetFont(&m_CustomFont);
//	m_stcLLevel.SetFont(&m_CustomFont);
//	m_stcRFreq.SetFont(&m_CustomFont);
//	m_stcRLevel.SetFont(&m_CustomFont);

//	ctrlToolOption1.SetFont(&m_CustomFont);
//	ctrlToolOption2.SetFont(&m_CustomFont);
//	ctrlToolOption3.SetFont(&m_CustomFont);
//	ctrlToolOption4.SetFont(&m_CustomFont);
	//091016
//	ctrlToolOption5.SetFont(&m_CustomFont);

//	ctrlAreaOption1.SetFont(&m_CustomFont);

//	ctrlCPUVersion.SetFont(&m_CustomFont);
//	ctrlMicomVersion.SetFont(&m_CustomFont);
//	ctrlUSBVersion.SetFont(&m_CustomFont);

	ctrlTestProgress.SetFont(&m_ProgressFont);

	LOGFONT logFont5;
	logFont5.lfHeight = 30;
	logFont5.lfWidth = 10;
	logFont5.lfEscapement = 0;
	logFont5.lfOrientation = 0;
	logFont5.lfWeight = FW_BOLD;
	logFont5.lfItalic = 0;
	logFont5.lfUnderline = 0;
	logFont5.lfStrikeOut = 0;
	logFont5.lfOutPrecision = 1;
	logFont5.lfClipPrecision = 2;
	logFont5.lfQuality = 1;
	logFont5.lfPitchAndFamily = 17;
	logFont5.lfCharSet = ANSI_CHARSET;
	strcpy_s(logFont5.lfFaceName, _countof(logFont5.lfFaceName), (LPCTSTR)"Arial");
	//strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Courier New");
	m_AudioFont.CreateFontIndirect(&logFont5);


	//ctrlPLC_COM_Status.SetFont(&m_AudioFont);
	m_cLabelSoundLeftFREQ.SetFont(&m_AudioFont);
	m_cLabelSoundLeftVolt.SetFont(&m_AudioFont);
	m_cLabelSoundRightFREQ.SetFont(&m_AudioFont);
	m_cLabelSoundRightVolt.SetFont(&m_AudioFont);
}

/////////////////////////////////////////////////////////////////////////////
// CDATsysView printing

BOOL CDATsysView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDATsysView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDATsysView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDATsysView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CDATsysView diagnostics

#ifdef _DEBUG
void CDATsysView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDATsysView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDATsysDoc* CDATsysView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDATsysDoc)));
	return(CDATsysDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDATsysView message handlers

void CDATsysView::OnDraw(CDC* pDC) 
{

//	m_ctrlSummaryGrid.Refresh();
//	ctrlSysInfoGrid.Refresh();
	
//	sModel = CurrentSet->sModelFolder;
	sModel = CurrentSet->sModelName;
	UpdateInfo(MODEL, sModel);

	UpdateInfo(CHASSIS, CurrentSet->sChassisName);

	UpdateInfo(REF, CurrentSet->sRefFolder);


	sSeq = CurrentSet->sSeqPath;
//	sSeq = CurrentSet->sModelFolder;
	UpdateInfo(SEQ, sSeq);

	CString szString;
	szString.Format("PRG%d",g_nRunningProcessNo);
	UpdateInfo(PROG_NO, szString);

	UpdateInfo(TESTED, sTested);


	sStatus = m_szCurrentStatus;

	UpdateInfo(STATUS, sStatus);
	UpdateInfo(FAILED, sFailed);
	UpdateInfo(TIME, sTime);
	UpdateInfo(RATE, sRate);
	
	switch(CurrentSet->nRunType)
	{
		case AUTO :
			sMode = _T("AUTO");
			break;
		case CONT :
			sMode = _T("AUTO");
			break;
		case STOP :
			sMode = _T("STOP(NG)");
			break;
		case ERRORSTEP :
			sMode = _T("STEP(NG)");
			break;
		case STEP :
			sMode = _T("STEP");
			break;
	}
	UpdateInfo(MODE, sMode);

	switch(m_nCurrentViewImageType)
	{
		case SNAP_IMAGE :
		{
			// change PSH 080603
			//if(m_bThreadRunning)
/*			if(CurrentSet->bIsRunning)
			{
				if(g_GrabImage.m_bImageLoaded)
				{
					DrawImage(IDC_VIDEOVIEW,g_GrabImage);
				}
			}
			else
			{
				if(m_bResultDlgActivate)
				{
					if(g_GrabImage.m_bImageLoaded)
					{
						DrawImage(IDC_VIDEOVIEW,g_GrabImage);
					}
				}
				else
				{
					if(m_bGrabThreadRunning)
					{
						if(g_GrabImage.m_bImageLoaded)
						{
							DrawImage(IDC_VIDEOVIEW,g_GrabImage);
						}
					}
					else if(m_Image[I_GRAB].m_bImageLoaded)
					{
						DrawImage(IDC_VIDEOVIEW,m_Image[I_GRAB]);
					}
				} 
			}*/
			if((!CurrentSet->bIsRunning) && (!m_bGrabThreadRunning))
			{
				if(m_Image[I_GRAB].m_bImageLoaded)
				{
					DrawImage(IDC_VIDEOVIEW,m_Image[I_GRAB]);
				}
			}
			 
		}
		break;

		case LOAD_IMAGE :
		{
			if(m_Image[I_REF].m_bImageLoaded)
			{
				DrawImage(IDC_VIDEOVIEW,m_Image[I_REF]);
			}
			else
			{
				AfxMessageBox("Reference Image is not loaded!");
			}
		}
		break;

		case PROC_IMAGE :
		{
			if(m_Image[I_PROC].m_bImageLoaded)
			{
				DrawImage(IDC_VIDEOVIEW,m_Image[I_PROC]);
			}
			else
			{
				AfxMessageBox("Prcoessed Image is not loaded!");
			}
		}
		break;
	}
}

void CDATsysView::UpdateInfo(int nIndex, CString s)
{
	CString sTmp,sModelName, sModelFolder;

	switch(nIndex)
	{
		case SEQ :
			m_stcModelFolder.SetWindowText(_T(s)); 
			break;
		case MODEL :
			m_stcSeqName.SetWindowText(_T(s));
			break;
		case CHASSIS :
			m_stcChassisName.SetWindowText(_T(s)); 
			break;
		case REF :
			m_stcRefFolder.SetWindowText(_T(s)); 
			break;
		case TESTED : 
			m_ctrLEDTested.SetWindowText(_T(s)); 
			break;
		case FAILED :
			m_ctrLEDFailed.SetWindowText(_T(s)); 
			break;
		case RATE:
			m_ctrLEDRate.SetWindowText(_T(s)); 
			break;
		case STATUS :
			m_stcStatus.SetWindowText(_T(s)); 
			break;
		case MODE:
			m_stcMode.SetWindowText(_T(s)); 
			break;
		case TIME :
			m_stcTime.SetWindowText(_T(s)); 
			break;
		case PROG_NO :
//			m_stcProgramNo.SetWindowText(_T(s)); 
			break;
		case AUDIO_L_FREQ :
//			m_stcLFreq.SetWindowText(_T(s));
			break;
		case AUDIO_R_FREQ :
//			m_stcRFreq.SetWindowText(_T(s));
			break;
		case AUDIO_L_LEVEL :
//			m_stcLLevel.SetWindowText(_T(s));
			break;
		case AUDIO_R_LEVEL :
//			m_stcRLevel.SetWindowText(_T(s));
			break;
	
		case LED_STATE:
			//m_LedsMesOn.EnableWindow(CurrentSet->bGMES_Connection);
			m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
		//	m_LedsMesOn.SetLedState(1);
			break;
	}

 }


void CDATsysView::OnEditCompile() 
{
	//+add PSH 090507
	BOOL bFlag;

	bFlag = m_SignalEnable;// g_pView->wIn_Flag;
	OnAudioMeasStop();
	//-
	Sleep(100);

	if(FileExists(CurrentSet->sSeqPath))
	{
		if(Prescan(CurrentSet->sSeqPath) == TRUE)
		{
			CurrentSet->bCompiled = TRUE;
//			CurrentSet->sSeqPath = dlg.m_strSeqFilePath;
			CurrentSet->lTime = GetModifyTime(CurrentSet->sSeqPath);
		}
		else 
		{
			DeleteStepList();
			CurrentSet->bCompiled = FALSE;
			// delete PSH 080603
			//CurrentSet->sSeqPath = _T("");
			CurrentSet->lTime = 0;
		}
	}
	else 
	{
		DeleteStepList();
		CurrentSet->bCompiled = FALSE;
		CurrentSet->sSeqPath = _T("");
		CurrentSet->lTime = 0;
	}
	
//	UpdateInfo(MODEL, CurrentSet->sModelFolder);
	UpdateInfo(MODEL, CurrentSet->sModelName);
	UpdateInfo(CHASSIS, CurrentSet->sChassisName);
	
	UpdateInfo(REF, CurrentSet->sRefFolder);
	UpdateInfo(SEQ, CurrentSet->sSeqPath);
	CWinApp* pApp = AfxGetApp();
	
	//+change kwmoon 070912 : Current TI -> Current Seq
	pApp->WriteProfileString(_T("File"), _T("Current Seq"), CurrentSet->sSeqPath);
	InsertStepData2Grid(CurrentSet->nDisplayType);

	//+add PSH 090507
//	if(bFlag) SetTimer(2, 1500, NULL);
}

void CDATsysView::OnEditCurrent() 
{//+ Check - Edit 프로그램 경로 설정해야함
	PROCESS_INFORMATION pInfo;
	STARTUPINFO         sInfo;
	char				achCommand[128];
	CWnd				*pWndPrev, *pWndChild;
	CString				s;
	if ((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	// Determine if another window with our class name exists...
	if(pWndPrev = FindWindow(_T("TIEditClass"),NULL))
	{
		pWndChild = pWndPrev->GetLastActivePopup(); 

		// If iconic, restore the main window
		if(pWndPrev->IsIconic()) 
		   pWndPrev->ShowWindow(SW_RESTORE);

		// Bring the main window or its popup to
		// the foreground
		pWndChild->SetForegroundWindow();         
		return;                             
	}

	sInfo.cb              = sizeof(STARTUPINFO);
	sInfo.lpReserved      = NULL;
	sInfo.lpReserved2     = NULL;
	sInfo.cbReserved2     = 0;
	sInfo.lpDesktop       = NULL;
	sInfo.lpTitle         = NULL;
	sInfo.dwFlags         = 0;
	sInfo.dwX             = 0;
	sInfo.dwY             = 0;
	sInfo.dwFillAttribute = 0;
	sInfo.wShowWindow     = SW_SHOW;

//	strcpy(achCommand,(LPCSTR)s);
	strcpy(achCommand, _T("C:\\Windows\\Notepad.exe "));
    if(!CurrentSet->sSeqPath.IsEmpty())
	{
		char temp[2];
		temp[0] = '"';
		temp[1] = '\0';
		strcat(achCommand, temp);
	    strcat(achCommand,(LPCSTR)CurrentSet->sSeqPath);
		strcat(achCommand, temp);
	}
	if(!CreateProcess(NULL			// address of module name
		, achCommand				// address of command line
		, NULL						// address of process security attributes
		, NULL						// address of thread security attributes
		, FALSE						// new process inherits handles
		, 0							// creation flags
		, NULL						// address of new environment block
		, NULL						// address of current directory name
		, &sInfo					// address of STARTUPINFO
		, &pInfo))					// address of PROCESS_INFORMATION
	{
	//	CString s;
		s.LoadString(IDS_ERROR_CHILDPROCESS);
		OkMessage(s);
	}
	
}


//+change kwmoon 081024
void CDATsysView::OnModelOpen() 
{
	CWinApp* pApp = AfxGetApp();

	int		nNoModel = 0;
	CString sTmp	 = _T("");
	CString sMsg	 = _T("");
	BOOL bFlag;
	BOOL bGrabFlag;
//	UINT nVal1;//

	if(OpenModelListFile(CurrentSet->sModelListPath,nNoModel,g_Divisions) == FALSE) return;	

	//+add PSH 090507
	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	//090615
	AudioMeasureStop();
	//-
	StopGrabThread();

	Sleep(100);

	CSelectChassisModelDlg dlg;
	dlg.m_nNoModel = nNoModel;

	if(dlg.DoModal() == IDOK)
	{
		//+move kwmoon 080818
		CurrentSet->sSeqPath		= CurrentSet->sServerFolder + dlg.m_szSeqFilePath;
		CurrentSet->sModelIni		= CurrentSet->sModelInfoFolder + dlg.m_szModelInfoFilePath;
		CurrentSet->sModelFolder	= CurrentSet->sServerFolder + dlg.m_szModelInfoFilePath.Left(dlg.m_szModelInfoFilePath.ReverseFind('\\'));
		CurrentSet->sRefFolder		= CurrentSet->sRefRootFolder + dlg.m_szRefImgFolder;
		CurrentSet->sMaskFolder		= CurrentSet->sRefFolder + "\\Mask";
		
		CurrentSet->sChassisName 	= dlg.m_sSelChassis;
		CurrentSet->sModelName 	= dlg.m_sSelModel;

		//add PSH 20091019
		if(!FileExists(CurrentSet->sRefFolder)){
			CreateFullPath(CurrentSet->sRefFolder);
			CreateFullPath(CurrentSet->sMaskFolder);
		}

		//=====================
		// Open Model.ini File 
		//=====================
		if(FileExists(CurrentSet->sModelIni))
		{
			OpenModelIniFile(CurrentSet->sModelIni);
			
			AvSwitchBoxCtrl.SetVideoOutType(CurrentSet->nAnalogType);
			
			bGrabFlag = g_pView->m_bGrabThreadRunning;
			StopGrabThread();
			Sleep(500);
			AnalogControl.SetVideoSourceType(CurrentSet->nAnalogType);
			Sleep(500);
			if(bGrabFlag) StartGrabThread();
			SetGrabInfo(&g_GrabImage);
		}
		else
		{
		//	sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sModelIni);
		//	AfxMessageBox(sMsg);
			CModelInfoCreate dlg;
				
			dlg.m_sModelInfo_FileName = CurrentSet->sModelIni;
			dlg.DoModal();
		}

		// position change PSH 080911
		g_pView->SaveRegistrySetting();

		//====================
		// Open Sequence File 
		//====================
		if(FileExists(CurrentSet->sSeqPath))
		{
			if(Prescan(CurrentSet->sSeqPath) == TRUE)
			{
				CurrentSet->bCompiled = TRUE;
				CurrentSet->lTime = GetModifyTime(CurrentSet->sSeqPath);
			}
			else 
			{
				DeleteStepList();
				CurrentSet->bCompiled = FALSE;
				
				//+del kwmoon 081024
				// change PSH 080603
			//	CurrentSet->sSeqPath = dlg.m_strSeqFilePath;
				CurrentSet->lTime = 0;
			}
		}
		else 
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sSeqPath);
			AfxMessageBox(sMsg);

			DeleteStepList();
			CurrentSet->bCompiled = FALSE;
			CurrentSet->sSeqPath = _T("");
			CurrentSet->lTime = 0;
		}
	

		//==========================
		// Open Test Parameter File 
		//==========================
		if(CurrentSet->sTestParamIni.IsEmpty())
		{
			CurrentSet->sTestParamIni = m_szExePath + "\\TestParam.Ini";
		}

		if(FileExists(CurrentSet->sTestParamIni))
		{
			OpenTestParamIniFile(CurrentSet->sTestParamIni);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sTestParamIni);
			AfxMessageBox(sMsg);
		}

		//===================
		// Open Pattern File 
		//===================
		if(CurrentSet->sPatternTitle.IsEmpty())
		{
			CurrentSet->sPatternTitle = m_szExePath + "\\Pattern.pat";
		}

		if(FileExists(CurrentSet->sPatternTitle))
		{
			OpenPatternFile(CurrentSet->sPatternTitle);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sPatternTitle);
			AfxMessageBox(sMsg);
		}
		
		//===================
		// Open Remocon File 
		//===================
/*		if(CurrentSet->sRemoconTitle.IsEmpty())
		{
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_REMOTE.rmt";
		}
*/
		switch(CurrentSet->nRemoteCustomCode)
		{
			case REMOCON_HT:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
			//	nVal1 = 0x2C;
				break;

			case REMOCON_MINI:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_MINI_REMOTE.rmt";
			//	nVal1 = 0x10;
				break;

			case REMOCON_BD:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_BD_REMOTE.rmt";
				break;

			case REMOCON_PN:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_PN_REMOTE.rmt";
				break;

			case REMOCON_LO:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LO_REMOTE.rmt";
				break;

			case REMOCON_NA:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_NA_REMOTE.rmt";
				break;

			case REMOCON_SO:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_SO_REMOTE.rmt";
				break;

			case REMOCON_CM:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_CM_REMOTE.rmt";
				break;

			case REMOCON_GS:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_GS_REMOTE.rmt";
				break;

			case REMOCON_TB:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_TB_REMOTE.rmt";
				break;

			case REMOCON_SH:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSH_REMOTE.rmt";
				break;

			case REMOCON_SI:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSI_REMOTE.rmt";
				break;

			case REMOCON_JV:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_JVC_REMOTE.rmt";
				break;

			default:
				CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
			//	nVal1 = 0x2C;
				break;
		}


		if(FileExists(CurrentSet->sRemoconTitle))
		{
			OpenRemoteFile(CurrentSet->sRemoconTitle);
		//	AvSwitchBoxCtrl.SetAvSwitch(SET_CUSTOM_CODE, MAX_AVSWITCH_WAIT_DELAY,  nVal1, 0);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sRemoconTitle);
			AfxMessageBox(sMsg);
		}

		//AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nRemoteType, 0);

		//====================
		// Open Template File 
		//====================
		if(!CurrentSet->sFullSeqPath.IsEmpty())
		{
			if(FileExists(CurrentSet->sFullSeqPath))
			{
				OpenFullSeq(CurrentSet->sFullSeqPath);
			}
		}

//		UpdateInfo(MODEL, CurrentSet->sModelFolder);
		UpdateInfo(MODEL, CurrentSet->sModelName);
		UpdateInfo(CHASSIS, CurrentSet->sChassisName);
		UpdateInfo(REF, CurrentSet->sRefFolder);
		UpdateInfo(SEQ, CurrentSet->sSeqPath);

		ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);

/*
		UpdateInfo(TOOL_OPTION1,CurrentSet->sToolOption1);
		UpdateInfo(TOOL_OPTION2,CurrentSet->sToolOption2);
		UpdateInfo(TOOL_OPTION3,CurrentSet->sToolOption3);
		UpdateInfo(TOOL_OPTION4,CurrentSet->sToolOption4);
		UpdateInfo(TOOL_OPTION5,CurrentSet->sToolOption5);
		UpdateInfo(AREA_OPTION1,CurrentSet->sAreaOption1);
		UpdateInfo(CPU_VERSION,CurrentSet->sCPUVersion);
		UpdateInfo(MICOM_VERSION,CurrentSet->sMicomVersion);
		UpdateInfo(USB_VERSION,CurrentSet->sUSBVersion);
*/
		
		UpdateVersionInfo();

		InsertStepData2Grid(CurrentSet->nDisplayType);
		
		//+add psh 080701
		if(TVCommCtrl.m_bPortOpen)
		{
			TVCommCtrl.PortClose();
		}

		if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
			if(TVCommCtrl.m_bPortOpen == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
			}
		}
		//-
	}

	//+add PSH 090615
	StartGrabThread();
//	if(bFlag) SetTimer(2, 2000, NULL);
	
}


LRESULT  CDATsysView::OnChangeModelOpen(WPARAM wParam, LPARAM lParam)
{
	CWinApp* pApp = AfxGetApp();

	int		nNoModel = 0;
	CString sTmp = _T("");
	CString sMsg = _T("");
	BOOL bFlag;
	BOOL bFindFlag;
	BOOL bGrabFlag;
	POSITION	Pos = NULL;
	CModelData* pModelData = NULL;
	//	UINT nVal1;//

	if (OpenModelListFile(CurrentSet->sModelListPath, nNoModel, g_Divisions) == FALSE) return 0;

	//+add PSH 090507
	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	//090615
	AudioMeasureStop();
	//-
	StopGrabThread();

	Sleep(100);

	CString str;
	CString lFindName;
	lFindName = CurrentSet->sModelSuffixName;
	//lFindName.Replace(".", "_");
	str = "Check S/N is";
	str += lFindName;
	str += " MODEL. \r\n \r\n";
	str += "  Do You Change MODEL ?";

//	if(IDOK == MessageWindow(str))
	if (IDYES == MessageBox(str, "New Model Detected", MB_YESNO))
	{

		//if (dlg.DoModal() == IDOK)
		Pos = CurrentSet->ModelList.GetHeadPosition();
		bFindFlag = 0;
		while (Pos)
		{
			pModelData = CurrentSet->ModelList.GetNext(Pos);

			if (pModelData->m_szChassisName != _T(""))
			{
				if (pModelData->m_szModelName.Find(lFindName) >= 0)
				{
					//lFindName = pModelData->m_szModelName;
					bFindFlag = 1;
				}
			}
		}
	}
	else
	{
		return 0;
	}
	
	//CurrentSet->sModelSuffixName;
	if(bFindFlag == 1)
	{
		//+move kwmoon 080818
		CurrentSet->sSeqPath = CurrentSet->sServerFolder + pModelData->m_szSeqFilePath;
		CurrentSet->sModelIni = CurrentSet->sModelInfoFolder + pModelData->m_szModelInfoFilePath;
		CurrentSet->sModelFolder
			= CurrentSet->sServerFolder 
			+ pModelData->m_szModelInfoFilePath.Left(pModelData->m_szModelInfoFilePath.ReverseFind('\\'));

	
		CurrentSet->sRefFolder = CurrentSet->sRefRootFolder + pModelData->m_szRefImgFolder;// .m_szRefImgFolder;
		CurrentSet->sMaskFolder = CurrentSet->sRefFolder + "\\Mask";

		CurrentSet->sChassisName = pModelData->m_szChassisName;// .m_sSelChassis;
		CurrentSet->sModelName = pModelData->m_szModelName;;

		//add PSH 20091019
		if (!FileExists(CurrentSet->sRefFolder)) {
			CreateFullPath(CurrentSet->sRefFolder);
			CreateFullPath(CurrentSet->sMaskFolder);
		}

		//=====================
		// Open Model.ini File 
		//=====================
		if (FileExists(CurrentSet->sModelIni))
		{
			OpenModelIniFile(CurrentSet->sModelIni);

			AvSwitchBoxCtrl.SetVideoOutType(CurrentSet->nAnalogType);

			bGrabFlag = g_pView->m_bGrabThreadRunning;
			StopGrabThread();
			Sleep(500);
			AnalogControl.SetVideoSourceType(CurrentSet->nAnalogType);
			Sleep(500);
			if (bGrabFlag) StartGrabThread();
			SetGrabInfo(&g_GrabImage);
		}
		else
		{
			//	sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sModelIni);
			//	AfxMessageBox(sMsg);
			CModelInfoCreate dlg;

			dlg.m_sModelInfo_FileName = CurrentSet->sModelIni;
			dlg.DoModal();
		}

		// position change PSH 080911
		g_pView->SaveRegistrySetting();

		//====================
		// Open Sequence File 
		//====================
		if (FileExists(CurrentSet->sSeqPath))
		{
			if (Prescan(CurrentSet->sSeqPath) == TRUE)
			{
				CurrentSet->bCompiled = TRUE;
				CurrentSet->lTime = GetModifyTime(CurrentSet->sSeqPath);
			}
			else
			{
				DeleteStepList();
				CurrentSet->bCompiled = FALSE;

				//+del kwmoon 081024
				// change PSH 080603
			//	CurrentSet->sSeqPath = dlg.m_strSeqFilePath;
				CurrentSet->lTime = 0;
			}
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sSeqPath);
			AfxMessageBox(sMsg);

			DeleteStepList();
			CurrentSet->bCompiled = FALSE;
			CurrentSet->sSeqPath = _T("");
			CurrentSet->lTime = 0;
		}


		//==========================
		// Open Test Parameter File 
		//==========================
		if (CurrentSet->sTestParamIni.IsEmpty())
		{
			CurrentSet->sTestParamIni = m_szExePath + "\\TestParam.Ini";
		}

		if (FileExists(CurrentSet->sTestParamIni))
		{
			OpenTestParamIniFile(CurrentSet->sTestParamIni);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sTestParamIni);
			AfxMessageBox(sMsg);
		}

		//===================
		// Open Pattern File 
		//===================
		if (CurrentSet->sPatternTitle.IsEmpty())
		{
			CurrentSet->sPatternTitle = m_szExePath + "\\Pattern.pat";
		}

		if (FileExists(CurrentSet->sPatternTitle))
		{
			OpenPatternFile(CurrentSet->sPatternTitle);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sPatternTitle);
			AfxMessageBox(sMsg);
		}

		//===================
		// Open Remocon File 
		//===================
/*		if(CurrentSet->sRemoconTitle.IsEmpty())
		{
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_REMOTE.rmt";
		}
*/
		switch (CurrentSet->nRemoteCustomCode)
		{
		case REMOCON_HT:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
			//	nVal1 = 0x2C;
			break;

		case REMOCON_MINI:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_MINI_REMOTE.rmt";
			//	nVal1 = 0x10;
			break;

		case REMOCON_BD:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_BD_REMOTE.rmt";
			break;

		case REMOCON_PN:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_PN_REMOTE.rmt";
			break;

		case REMOCON_LO:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LO_REMOTE.rmt";
			break;

		case REMOCON_NA:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_NA_REMOTE.rmt";
			break;

		case REMOCON_SO:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_SO_REMOTE.rmt";
			break;

		case REMOCON_CM:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_CM_REMOTE.rmt";
			break;

		case REMOCON_GS:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_GS_REMOTE.rmt";
			break;

		case REMOCON_TB:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_TB_REMOTE.rmt";
			break;

		case REMOCON_SH:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSH_REMOTE.rmt";
			break;

		case REMOCON_SI:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSI_REMOTE.rmt";
			break;

		case REMOCON_JV:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_JVC_REMOTE.rmt";
			break;

		default:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
			//	nVal1 = 0x2C;
			break;
		}


		if (FileExists(CurrentSet->sRemoconTitle))
		{
			OpenRemoteFile(CurrentSet->sRemoconTitle);
			//	AvSwitchBoxCtrl.SetAvSwitch(SET_CUSTOM_CODE, MAX_AVSWITCH_WAIT_DELAY,  nVal1, 0);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sRemoconTitle);
			AfxMessageBox(sMsg);
		}

		//AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nRemoteType, 0);

		//====================
		// Open Template File 
		//====================
		if (!CurrentSet->sFullSeqPath.IsEmpty())
		{
			if (FileExists(CurrentSet->sFullSeqPath))
			{
				OpenFullSeq(CurrentSet->sFullSeqPath);
			}
		}

		//		UpdateInfo(MODEL, CurrentSet->sModelFolder);
		UpdateInfo(MODEL, CurrentSet->sModelName);
		UpdateInfo(CHASSIS, CurrentSet->sChassisName);
		UpdateInfo(REF, CurrentSet->sRefFolder);
		UpdateInfo(SEQ, CurrentSet->sSeqPath);

		ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);

		/*
				UpdateInfo(TOOL_OPTION1,CurrentSet->sToolOption1);
				UpdateInfo(TOOL_OPTION2,CurrentSet->sToolOption2);
				UpdateInfo(TOOL_OPTION3,CurrentSet->sToolOption3);
				UpdateInfo(TOOL_OPTION4,CurrentSet->sToolOption4);
				UpdateInfo(TOOL_OPTION5,CurrentSet->sToolOption5);
				UpdateInfo(AREA_OPTION1,CurrentSet->sAreaOption1);
				UpdateInfo(CPU_VERSION,CurrentSet->sCPUVersion);
				UpdateInfo(MICOM_VERSION,CurrentSet->sMicomVersion);
				UpdateInfo(USB_VERSION,CurrentSet->sUSBVersion);
		*/

		UpdateVersionInfo();

		InsertStepData2Grid(CurrentSet->nDisplayType);

		//+add psh 080701
		if (TVCommCtrl.m_bPortOpen)
		{
			TVCommCtrl.PortClose();
		}

		if (TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
		{
			TVCommCtrl.PortClose();
			TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
			if (TVCommCtrl.m_bPortOpen == FALSE)
			{
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)", CurrentSet->sTVComPort);
				AfxMessageBox(szErrMsg);
			}
		}
		//-
	}

	//+add PSH 090615
	StartGrabThread();
	//	if(bFlag) SetTimer(2, 2000, NULL);
	return 0;

}
BOOL CDATsysView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CDATsysView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CFormView::OnCreate(lpCreateStruct) == -1) return -1;
	
	CWinApp* pApp		= AfxGetApp();
	CString sTmp		= _T("");
	CString sBaudRate	= _T("");
	CString sMsg		= _T("");

	CurrentSet = new CEnvironmentData;

	//+move kwmoon 080821
	//================
	// Get Exe Folder 
	//================
	TCHAR szExeName[_MAX_PATH] = _T("");

	GetModuleFileName(NULL,szExeName,_MAX_PATH);

	m_szExePath = szExeName;

	m_szExePath = m_szExePath.Left(m_szExePath.ReverseFind('\\'));

	
	CString szLogFile = _T("");

	sTmp.Format(_T("%s\\TestResult"), m_szExePath);
	

	if(!FileExists(sTmp))   CreateFullPath(sTmp);
/*	if(g_nRunningProcessNo == 1)
	{
		szLogFile = m_szExePath + "\\TestResult\\DFT1.log";
	}
	else
	{
		szLogFile = m_szExePath + "\\TestResult\\DFT2.log";
	}
	if(g_LogFile.Open(szLogFile,CFile::modeWrite|CFile::modeCreate,0)){
		 g_LogFileOpen = TRUE;
	}
	else{
		 g_LogFileOpen = FALSE;
	}
*/
#if _DEBUG
LoadRegistrySetting(CurrentSet); 
#else
	LoadRegistrySetting(CurrentSet); 
#endif

	//=====================
	// Open Model.ini File 
	//=====================
	if(CurrentSet->sModelIni.IsEmpty())
	{
		//+change kwmoon 080904
	//	CreateModelIniFile(m_szExePath);
		//CurrentSet->sModelIni = m_szExePath + "\\ModelInfo.Ini";
		CurrentSet->sModelIni = m_szExePath + "\\Ref_20\\BAR\\SN6xTEST\\SN6xTEST.ini";//'\Ref_20\BAR\SN6xTEST\SN6xTEST.ini
	}

	//+add kwmoon 080821 : Add FileExists Check
	if(FileExists(CurrentSet->sModelIni))
	{
		OpenModelIniFile(CurrentSet->sModelIni);
		//+add PSH 081016
//		SetGrabInfo(&g_GrabImage);
	}
	else
	{
		sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sModelIni);
		AfxMessageBox(sMsg);
	}
	
	//==========================
	// Open Test Parameter File 
	//==========================
	if(CurrentSet->sTestParamIni.IsEmpty())
	{
		CurrentSet->sTestParamIni = m_szExePath + "\\TestParam.Ini";
	}

	//+add kwmoon 080821 : Add FileExists Check
	if(FileExists(CurrentSet->sTestParamIni))
	{
		OpenTestParamIniFile(CurrentSet->sTestParamIni);
	}
	else
	{
		sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sTestParamIni);
		AfxMessageBox(sMsg);
	}

	//===================
	// Open Pattern File 
	//===================
	if(CurrentSet->sPatternTitle.IsEmpty())
	{
		CurrentSet->sPatternTitle = m_szExePath + "\\Pattern.pat";
	}

	//+add kwmoon 080821 : Add FileExists Check
	if(FileExists(CurrentSet->sPatternTitle))
	{
		OpenPatternFile(CurrentSet->sPatternTitle);
	}
	else
	{
		sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sPatternTitle);
		AfxMessageBox(sMsg);
	}
	
	//===================
	// Open Remocon File 
	//===================
/*	if(CurrentSet->sRemoconTitle.IsEmpty())
	{
		CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_REMOTE.rmt";
	}
*/
	switch(CurrentSet->nRemoteCustomCode)
	{
		case REMOCON_HT:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
			break;

		case REMOCON_MINI:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_MINI_REMOTE.rmt";
			break;

		case REMOCON_BD:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_BD_REMOTE.rmt";
			break;

		case REMOCON_PN:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_PN_REMOTE.rmt";
		//	nVal1 = 0x10;
			break;

		case REMOCON_LO:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LO_REMOTE.rmt";
		//	nVal1 = 0x2D;
			break;

		case REMOCON_NA:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_NA_REMOTE.rmt";
		//	nVal1 = 0x10;
			break;

		case REMOCON_SO:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_SO_REMOTE.rmt";
		//	nVal1 = 0x2D;
			break;

		case REMOCON_CM:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_CM_REMOTE.rmt";
			break;

		case REMOCON_GS:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_GS_REMOTE.rmt";
			break;

		case REMOCON_TB:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSC_TB_REMOTE.rmt";
			break;

		case REMOCON_SH:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSH_REMOTE.rmt";
			break;

		case REMOCON_SI:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_LSI_REMOTE.rmt";
			break;

		case REMOCON_JV:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_JVC_REMOTE.rmt";
			break;

		default:
			CurrentSet->sRemoconTitle = m_szExePath + "\\DM_DFT_HT_REMOTE.rmt";
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

	//====================
	// Open Template File 
	//====================
	if(!CurrentSet->sFullSeqPath.IsEmpty())
	{
		if(FileExists(CurrentSet->sFullSeqPath))
		{
			OpenFullSeq(CurrentSet->sFullSeqPath);
		}
	}
	
	if(CurrentSet->nRunType < CONT || CurrentSet->nRunType > AUTO)
	{
		CurrentSet->nRunType = CONT;
	}
	
	if(CurrentSet->nPrintType < ALL || CurrentSet->nPrintType >NONE)
	{
		CurrentSet->nPrintType = ERRORVIEW;
	}

	//+add kwmoon 081024
	//======================
	// Open Model List File 
	//======================
	if(CurrentSet->sModelListPath.IsEmpty())
	{
//		CurrentSet->sModelListPath = m_szExePath + "\\ModelList.ini";
		CurrentSet->sModelListPath = m_szExePath + "\\ModelList.xls";
	}
	else{
		sTmp = CurrentSet->sModelListPath.Right(3);
		sTmp.MakeLower();
		if(sTmp == "ini")
		{
			CurrentSet->sModelListPath = m_szExePath + "\\ModelList.xls";
		}
	}

	if(FileExists(CurrentSet->sModelListPath))
	{
		int nNoModel = 0;
		OpenModelListFile(CurrentSet->sModelListPath,nNoModel,g_Divisions);
	}
	else
	{
		sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sModelListPath);
		AfxMessageBox(sMsg);
	}


	return 0;
}

//+add kwmoon 071210
void CDATsysView::LoadRegistrySetting(CEnvironmentData* pCurrentSet) 
{
	CWinApp* pApp		= AfxGetApp();
	CString sTmp		= _T("");
	CString sBaudRate	= _T("");

	// Initialize variable
	pCurrentSet->bIsRunning				= FALSE;
	pCurrentSet->bIsRunMsg				= FALSE;	

	pCurrentSet->bMesUploadCheckTimer 	= FALSE;	

	pCurrentSet->sSeqPath				= pApp->GetProfileString(_T("File"), _T("Current Seq"), _T(""));
	pCurrentSet->sFullSeqPath			= pApp->GetProfileString(_T("File"), _T("Current Full Seq"), _T(""));
	pCurrentSet->sModelIni				= pApp->GetProfileString(_T("File"), _T("Current ModelIni"), _T(""));
	pCurrentSet->sRemoconTitle			= pApp->GetProfileString(_T("File"), _T("Current Remocon"), _T(""));
	pCurrentSet->sPatternTitle			= pApp->GetProfileString(_T("File"), _T("Current Pattern"), _T(""));
	pCurrentSet->sTestParamIni			= pApp->GetProfileString(_T("File"), _T("Current TestParam"), _T(""));
	//+add kwmoon 081024
	pCurrentSet->sModelListPath			= pApp->GetProfileString(_T("File"), _T("Current ModelList"), _T(""));

	pCurrentSet->sChassisName			= pApp->GetProfileString(_T("File"), _T("Current Chassis"), _T("BAR"));
	pCurrentSet->sModelName				= pApp->GetProfileString(_T("File"), _T("Current Model"), _T("SN6xInstall"));
	pCurrentSet->sModelSuffixName		= pApp->GetProfileString(_T("File"), _T("Current Model Suffix Name"), _T(""));

	CurrentSet->sWorkOrderID			= pApp->GetProfileString(_T("GMES"), _T("Current WorkOrder ID"), _T(""));
	CurrentSet->sOrganizationID			= pApp->GetProfileString(_T("GMES"), _T("Current Organization ID"), _T(""));
	CurrentSet->sLineID					= pApp->GetProfileString(_T("GMES"), _T("Current Line ID"), _T(""));
	CurrentSet->sEquipmentID			= pApp->GetProfileString(_T("GMES"), _T("Current Equipment ID"), _T(""));
	CurrentSet->sOperationID			= pApp->GetProfileString(_T("GMES"), _T("Current Operation ID"), _T(""));
	CurrentSet->sModelID				= pApp->GetProfileString(_T("GMES"), _T("Current Model ID"), _T(""));
	CurrentSet->sModelName_GMES			= pApp->GetProfileString(_T("GMES"), _T("Current Model Name"), _T(""));
	CurrentSet->sModelSuffix			= pApp->GetProfileString(_T("GMES"), _T("Current Model Suffix"), _T(""));

	pCurrentSet->nRunType				= pApp->GetProfileInt(_T("Run"), _T("Run Type"), STOP);
	pCurrentSet->nRunStartType			= pApp->GetProfileInt(_T("Run"), _T("Start Type"), MANUAL_START);

	pCurrentSet->nDisplayType			= pApp->GetProfileInt(_T("Display"), _T("Display Type"), DETAILEDGRID);
	pCurrentSet->nPrintType				= pApp->GetProfileInt(_T("Display"), _T("PrintOut Type"), ERRORVIEW);
	pCurrentSet->bCommDisplay			= pApp->GetProfileInt(_T("Display"), _T("Comm Display"), TRUE);
	pCurrentSet->sViewerPath			= pApp->GetProfileString(_T("File"), _T("Current Viewer"), "C:\\Windows\\Notepad.exe");
//	pCurrentSet->nLvdsWidth				= pApp->GetProfileInt(_T("Config"), _T("LVDS Grabber Width"), 1366);
//	pCurrentSet->nLvdsHeight			= pApp->GetProfileInt(_T("Config"), _T("LVDS Grabber Height"), 768);
	pCurrentSet->nAnalogWidth			= pApp->GetProfileInt(_T("Config"), _T("Analog Grabber Width"), 640);
	pCurrentSet->nAnalogHeight			= pApp->GetProfileInt(_T("Config"), _T("Analog Grabber Height"), 480);

	//+add 090420(Modification No1)
	pCurrentSet->nAnalogType			= pApp->GetProfileInt(_T("Config"), _T("Analog Grabber Type"), 0);

	CurrentSet->nUseUserRetry			= pApp->GetProfileInt(_T("Config"), _T("Use User Retry"), 0);

//	if(pCurrentSet->nLvdsWidth	 == 0)	pCurrentSet->nLvdsWidth		= 1366;
//	if(pCurrentSet->nLvdsHeight	 == 0)	pCurrentSet->nLvdsHeight	= 768;
	if(pCurrentSet->nAnalogWidth  == 0)	pCurrentSet->nAnalogWidth	= 640;
	if(pCurrentSet->nAnalogHeight == 0)	pCurrentSet->nAnalogHeight	= 480;

	pCurrentSet->sModelFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Folder"), "D:\\DM_DFT");
	if (pCurrentSet->sModelFolder.IsEmpty())
		pCurrentSet->sModelFolder = "D:\\DM_DFT";
	pCurrentSet->sGrabFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Grab Folder"), "D:\\DM_DFT\\data");
	if (pCurrentSet->sGrabFolder.IsEmpty())
		pCurrentSet->sGrabFolder = "D:\\DM_DFT\\data";
	pCurrentSet->sRefFolder				= pApp->GetProfileString(_T("Folder"), _T("Current Ref Folder"), "D:\\DM_DFT\\ref");
	if (pCurrentSet->sRefFolder.IsEmpty())
		pCurrentSet->sRefFolder = "D:\\DM_DFT\\ref";
	pCurrentSet->sMaskFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Mask Folder"), "D:\\DM_DFT\\ref\\Mask");
	if (pCurrentSet->sMaskFolder.IsEmpty())
		pCurrentSet->sMaskFolder = "D:\\DM_DFT\\ref\\Mask";

	//+add kwmoon 080904
	pCurrentSet->sServerFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Server Folder"), "D:\\DM_DFT");
	if (pCurrentSet->sServerFolder.IsEmpty())
		pCurrentSet->sServerFolder = "D:\\DM_DFT";
	//+add PSH 081029
	pCurrentSet->sRefRootFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Reference Image Root Folder"), "D:\\DM_DFT");
	if (pCurrentSet->sRefRootFolder.IsEmpty())
		pCurrentSet->sRefRootFolder = "D:\\DM_DFT";
	//+add PSH 081211
	pCurrentSet->sModelInfoFolder		= pApp->GetProfileString(_T("Folder"), _T("Current Model Info  Root Foler"), "D:\\DM_DFT");
	if (pCurrentSet->sModelInfoFolder.IsEmpty())
		pCurrentSet->sModelInfoFolder = "D:\\DM_DFT";

	pCurrentSet->sParmAdjGrabFolder		= pApp->GetProfileString(_T("Folder"), _T("ParmAdj Current Grab Folder"), "D:\\DM_DFT\\data");
	if (pCurrentSet->sParmAdjGrabFolder.IsEmpty())
		pCurrentSet->sParmAdjGrabFolder = "D:\\DM_DFT";
	pCurrentSet->sParmAdjRefFolder		= pApp->GetProfileString(_T("Folder"), _T("ParmAdj Current Ref Folder"), "D:\\DM_DFT\\ref");
	if (pCurrentSet->sParmAdjRefFolder.IsEmpty())
		pCurrentSet->sParmAdjRefFolder = "D:\\DM_DFT";
	pCurrentSet->sParmAdjMaskFolder		= pApp->GetProfileString(_T("Folder"), _T("ParmAdj Current Mask Folder"), "D:\\DM_DFT\\ref\\Mask");
	if (pCurrentSet->sParmAdjMaskFolder.IsEmpty())
		pCurrentSet->sParmAdjMaskFolder = "D:\\DM_DFT";

	if(g_nRunningProcessNo == 1)
	{
		pCurrentSet->sPatternComPort		= pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "COM1");
		if(pCurrentSet->sPatternComPort		== _T("")) pCurrentSet->sPatternComPort = "COM1";
								
		pCurrentSet->sAvSwitchBoxComPort	= pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "COM3");
		if(pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "COM3";

		//pCurrentSet->sTVComPort				= pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "COM5");
		//if(pCurrentSet->sTVComPort			== _T("")) pCurrentSet->sTVComPort = "COM5";

		pCurrentSet->sScannerComPort		= pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "COM7");
		if(pCurrentSet->sScannerComPort		== _T("")) pCurrentSet->sScannerComPort = "COM7";
	
		pCurrentSet->sIrChkComPort			= pApp->GetProfileString(_T("Config"), _T("IR Checker ComPort"), "COM9");
		if(pCurrentSet->sIrChkComPort		== _T("")) pCurrentSet->sIrChkComPort = "COM9";

		pCurrentSet->sHDMIComPort			= pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "\\\\.\\COM11");
		if(pCurrentSet->sHDMIComPort		== _T("")) pCurrentSet->sHDMIComPort = "\\\\.\\COM11";
		
		pCurrentSet->sThComPort				= pApp->GetProfileString(_T("Config"), _T("Thermometer ComPort"), "\\\\.\\COM13");
		if(pCurrentSet->sThComPort			== _T("")) pCurrentSet->sThComPort = "\\\\.\\COM13";
	}
	else
	{
		pCurrentSet->sPatternComPort		= pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "COM2");
		if(pCurrentSet->sPatternComPort		== _T("")) pCurrentSet->sPatternComPort = "COM2";
								
		pCurrentSet->sAvSwitchBoxComPort	= pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "COM4");
		if(pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "COM4";

		//pCurrentSet->sTVComPort				= pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "COM6");
		//if(pCurrentSet->sTVComPort			== _T("")) pCurrentSet->sTVComPort = "COM6";

		pCurrentSet->sScannerComPort		= pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "COM8");
		if(pCurrentSet->sScannerComPort		== _T("")) pCurrentSet->sScannerComPort = "COM8";

		pCurrentSet->sIrChkComPort			= pApp->GetProfileString(_T("Config"), _T("IR Checker ComPort"), "\\\\.\\COM10");
		if(pCurrentSet->sIrChkComPort		== _T("")) pCurrentSet->sIrChkComPort = "\\\\.\\COM10";

		pCurrentSet->sHDMIComPort			= pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "\\\\.\\COM12");
		if(pCurrentSet->sHDMIComPort		== _T("")) pCurrentSet->sHDMIComPort = "\\\\.\\COM12";
		
		pCurrentSet->sThComPort				= pApp->GetProfileString(_T("Config"), _T("Thermometer ComPort"), "\\\\.\\COM14");
		if(pCurrentSet->sThComPort			== _T("")) pCurrentSet->sThComPort = "\\\\.\\COM14";
	}

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("Pattern Generator Baud Rate"), "9600");
	pCurrentSet->wPatternBaudRate		= (DWORD)(atoi(sBaudRate));

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("AV SwitchBox Baud Rate"), "9600");
	pCurrentSet->wAvSwitchBoxBaudRate	= (DWORD)(atoi(sBaudRate));

	//+del psh 080701
//	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("TV Baud Rate"), "115200");
//	pCurrentSet->wTVBaudRate			= (DWORD)(atoi(sBaudRate));
//-
	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("Scanner Baud Rate"), "9600");
	pCurrentSet->wScannerBaudRate		= (DWORD)(atoi(sBaudRate));

	//+Add kwmoon 080625
	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("IR Checker Baud Rate"), "19200");
	pCurrentSet->wIrChkBaudRate		= (DWORD)(atoi(sBaudRate));

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("HDMI Generator Baud Rate"), "19200");
	pCurrentSet->wHDMIBaudRate			= (DWORD)(atoi(sBaudRate));

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("Thermometer Baud Rate"), "9600");
	pCurrentSet->wThBaudRate			= (DWORD)(atoi(sBaudRate));

	pCurrentSet->bUsePatternGen			= pApp->GetProfileInt(_T("Config"), _T("Use Pattern Generator"), 1);
//	pCurrentSet->bUseTVCommPort			= pApp->GetProfileInt(_T("Config"), _T("Use TVComm Port"), 1);
	pCurrentSet->bUseAVSwitchBox		= pApp->GetProfileInt(_T("Config"), _T("Use AVSwitchingBox"), 1);
	pCurrentSet->bUseScanner			= pApp->GetProfileInt(_T("Config"), _T("Use Scanner"), 1);
	pCurrentSet->bUseHDMIGen			= pApp->GetProfileInt(_T("Config"), _T("Use HDMI Generator"), 1);
	pCurrentSet->bUseIrChk				= pApp->GetProfileInt(_T("Config"), _T("Use IR Checker"), 1);
	pCurrentSet->bUseTh					= pApp->GetProfileInt(_T("Config"), _T("Use Thermometer"), 1);

	//+change kwmoon 080804
	pCurrentSet->nAdcType				= pApp->GetProfileInt(_T("Config"), _T("ADC Type"), 0); // 0 : RS232C, 1 : I2C

	pCurrentSet->bSaveGrabImg			= pApp->GetProfileInt(_T("Config"), _T("Auto Save Grab Image"), 0);// 0 : NG, 1 : ALL
	pCurrentSet->bSaveProcImg			= TRUE; // pApp->GetProfileInt(_T("Config"), _T("Auto Save Processed Image"), 1);
	pCurrentSet->bSaveCaptionTestImg	= pApp->GetProfileInt(_T("Config"), _T("Save Caption Test Image"), 0);
	pCurrentSet->bSaveMovingPicTestImg	= pApp->GetProfileInt(_T("Config"), _T("Save Moving Pic Test Image"), 0);
	pCurrentSet->bSaveIntermediateImage	= pApp->GetProfileInt(_T("Config"), _T("Save Intermediate Image"), 0);
	pCurrentSet->bSaveProcessingTimeData = pApp->GetProfileInt(_T("Config"), _T("Save Processing Time Data"), 0);

	//+add kwmoon 080716
	pCurrentSet->bSaveReviewData		= pApp->GetProfileInt(_T("Config"), _T("Save Review Data"), 0);
	pCurrentSet->bSaveDetailResultData	= pApp->GetProfileInt(_T("Config"), _T("Save Detail Result Data"), 0);

	pCurrentSet->nAudioSource			= pApp->GetProfileInt(_T("Config"), _T("Audio Input Source"), 0);

	//+change kwmoon 080715
	pCurrentSet->bSaveResult2Txt		=  pApp->GetProfileInt(_T("Config"), _T("Save Result to Text File"), 0);
	pCurrentSet->bSaveResult2Html		=  pApp->GetProfileInt(_T("Config"), _T("Save Result to Html File"), 1);
	pCurrentSet->bSaveResult2Csv		=  pApp->GetProfileInt(_T("Config"), _T("Save Result to Csv File"), 1);
	//-

	pCurrentSet->nNoRepeatExecution		= pApp->GetProfileInt(_T("Config"), _T("No of repeat execution"), 1);

	//+change kwmoon 080925
//	pCurrentSet->nNoFrameForRefImageCreation	= pApp->GetProfileInt(_T("Config"), _T("No of frames for refimage creation"), 2);
	pCurrentSet->nNoFrameForRefImageCreation	= pApp->GetProfileInt(_T("Config"), _T("No of frames for refimage creation"), 1);


	pCurrentSet->nPixelInMiddleRange	=  pApp->GetProfileInt(_T("Config"), _T("Percentage of pixels in middle range"), 85);
	pCurrentSet->nNonBlackPixel			=  pApp->GetProfileInt(_T("Config"), _T("Percentage of non-black pixels"), 30);

	//+add 090218(Modification No1)
	pCurrentSet->nNoUsedColors			=  pApp->GetProfileInt(_T("Config"), _T("No Used Colors"), 5000);

	//+del kwmoon 080715
//	pCurrentSet->nNoAllowedErrorPixelForOsdTest =  pApp->GetProfileInt(_T("Config"), _T("No of allowed error pixel for osd test"), 0);

	//+add kwmoon 080123
	pCurrentSet->nDelayPercentage		= pApp->GetProfileInt(_T("Config"), _T("Delay Percentage"), 100);

//	pCurrentSet->nUranousMode			= pApp->GetProfileInt(_T("Config"), _T("Uranous Grabber Mode"), NS_SINGLE8);
    
	//+add psh 090630
//	pCurrentSet->bOddDE_Only			= (BOOL)pApp->GetProfileInt(_T("Config"), _T("LVDS OddDE Only"), 0);

//	if(pCurrentSet->nUranousMode == 0)	pCurrentSet->nUranousMode = NS_SINGLE;

//	pCurrentSet->nBitCount				= pApp->GetProfileInt(_T("Config"), _T("Color Bit Count"), 24);
	//+del PSH 081218
//	pCurrentSet->nTotalNoTest			= pApp->GetProfileInt(_T("Config"), _T("Total No of test"), 0);
//	pCurrentSet->nTotalNoNg				= pApp->GetProfileInt(_T("Config"), _T("Total No of Ng"), 0);

	//+add kwmoon 081004
//	pCurrentSet->nTotalNoUserStop		= pApp->GetProfileInt(_T("Config"), _T("Total No of User Stop"), 0);
	
	pCurrentSet->nNoRetry				= pApp->GetProfileInt(_T("Config"), _T("No of Internal Retry"), 1);
	
	//+add psh 081030
	pCurrentSet->nGrabCheckArea			= pApp->GetProfileInt(_T("Config"), _T("Grab Image Check Area"), 7);

	//+del kwmoon 080125
	//+add kwmoon 071227
//	pCurrentSet->nSpecMargin			= pApp->GetProfileInt(_T("Config"), _T("Spec Margin"), 0);

	//+del kwmoon 080819
	//+add kwmoon 080123
//	pCurrentSet->nFreqMargin		= pApp->GetProfileInt(_T("Config"), _T("Audio Spec Margin"), 30);

	pCurrentSet->bTVCommPortEcho		= (BOOL)pApp->GetProfileInt(_T("Config"), _T("TVComm Port Echo"), 0);

	pCurrentSet->sHDMIRoiShape			= pApp->GetProfileString(_T("Config"), _T("HDMI Roi Shape"), "VERTICAL");
	pCurrentSet->nLocation				= pApp->GetProfileInt(_T("DataBase"), _T("Location"), 0);
	pCurrentSet->nLineNo				= pApp->GetProfileInt(_T("DataBase"), _T("Line Number"), 0);
	pCurrentSet->nSystemNo				= pApp->GetProfileInt(_T("DataBase"), _T("System Number"), 0);
	pCurrentSet->nAuthority				= pApp->GetProfileInt(_T("DataBase"), _T("Authority"), 0);
	pCurrentSet->strPassword			= pApp->GetProfileString(_T("DataBase"), _T("PassWord"), _T("DFT"));

//	pCurrentSet->strMesDsn				= pApp->GetProfileString(_T("DataBase"), _T("MES DSN"), _T("DISMES"));
//	pCurrentSet->strMesUserId			= pApp->GetProfileString(_T("DataBase"), _T("MES USERID"), _T("MESLOOK"));
//	pCurrentSet->strMesPassWord			= pApp->GetProfileString(_T("DataBase"), _T("MES PASSWORD"), _T("LOOKMES"));
	pCurrentSet->strGmesIP				= pApp->GetProfileString(_T("DataBase"), _T("GMES Host IP"), _T("156.147.82.152"));
	pCurrentSet->nGmesPort			= pApp->GetProfileInt(_T("DataBase"), _T("GMES Host Port"), 40201);

	pCurrentSet->bUploadMes				= (BOOL)pApp->GetProfileInt(_T("DataBase Upload"), _T("UPLOAD MES DATA"), 0); 
	pCurrentSet->bAutoUploadMes			= (BOOL)pApp->GetProfileInt(_T("DataBase"), _T("AUTO UPLOAD MES DATA"), 0); 
	pCurrentSet->bAutoGMES_ChangeModel = (BOOL)pApp->GetProfileInt(_T("AUTO CHANGE MODEL"), _T("AUTO CHANGE MODEL"), 0);

	pCurrentSet->bAutoDataDelete		= (BOOL)pApp->GetProfileInt(_T("DataBase"), _T("AUTO UPLOAD MES DATA"), 0); 

	//+change kwmoon 080925
//	pCurrentSet->nAudioCheckMethod		= pApp->GetProfileInt(_T("Config"), _T("AudioCheckMethod"), 0);
	pCurrentSet->nAudioCheckMethod		= pApp->GetProfileInt(_T("Config"), _T("AudioCheckMethod"), 1);

	//+add psh 090401
	pCurrentSet->nDataSavePeriod		= pApp->GetProfileInt(_T("Config"), _T("Data Save Period"), 60);
	
	if ((pCurrentSet->nDataSavePeriod < 0) || (pCurrentSet->nDataSavePeriod > 1000))
	{
		pCurrentSet->nDataSavePeriod = 60;
	}
	pCurrentSet->bAutoDataDelete			= pApp->GetProfileInt(_T("Config"), _T("Auto Data Delete"), 1);
	if ((pCurrentSet->bAutoDataDelete < 0) || (pCurrentSet->bAutoDataDelete > 1))
	{
		pCurrentSet->bAutoDataDelete = 1;
	}

//	CurrentSet->sCPUVersion   = pApp->GetProfileString(_T("Config"), _T("CPU Version"), _T(""));
//	CurrentSet->sMicomVersion = pApp->GetProfileString(_T("Config"), _T("Micom Version"), _T(""));
//	CurrentSet->sUSBVersion   = pApp->GetProfileString(_T("Config"), _T("USB Version"), _T(""));

	// 090120 External DFT
//	pCurrentSet->nLvdsGrabberType		= pApp->GetProfileInt(_T("Config"), _T("Lvds Grabber Type"), INTERNAL_GRABBER);

	//+add psh 110225
	pCurrentSet->nRemoteCustomCode		= pApp->GetProfileInt(_T("Config"), _T("Remocon Custom Code"), 0);
	if((pCurrentSet->nRemoteCustomCode < 0) || (pCurrentSet->nRemoteCustomCode >12))
	{
		pCurrentSet->nRemoteCustomCode = 0;
	}
	pCurrentSet->nRemoteType		= pApp->GetProfileInt(_T("Config"), _T("Remocon Type"), 0);
	if((pCurrentSet->nRemoteType < 0) || (pCurrentSet->nRemoteType >1))
	{
		pCurrentSet->nRemoteType = 0;
	}

	pCurrentSet->sAvSwitchBoxComPort	= pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "COM4");
	pCurrentSet->bAvSwitchBoxBuzzerMute	= pApp->GetProfileInt(_T("Config"), _T("AV SwitchBox Buzzet Mute"), 1);

	pCurrentSet->nAvSwitchBoxInitVideoCh= pApp->GetProfileInt(_T("Config"), _T("AV SwitchBox Initialize Video Mode"), 1);

	pCurrentSet->bVersionCheckOnly		= pApp->GetProfileInt(_T("Config"), _T("Version Check Only"), 0);

	pCurrentSet->nProcessType			= pApp->GetProfileInt(_T("Config"), _T("Inspection Process Type"), 0);

	pCurrentSet->nModelInfo_Check		= pApp->GetProfileInt(_T("Config"), _T("Model Info. Check Type"), 0);

	pCurrentSet->bFixtureIdCheck		= pApp->GetProfileInt(_T("Config"), _T("Fixture ID Check"), 1);

	pCurrentSet->nJigUpType				= pApp->GetProfileInt(_T("Config"), _T("JigUpType"), 0); 

	pCurrentSet->nSoundInDeviceID = pApp->GetProfileInt(_T("Config"), _T("Sound Device ID"), 0);

	

}

void CDATsysView::OnDestroy() 
{
//	CFormView::OnDestroy();

	DWORD	dwStatus;
	CString sTmp				 = _T("");
	CString sMsg				 = _T("");
	CString sBaudRate		 	 = _T("");
	int nCount = 0;
//	DWORD	dwStatus;
	DWORD dwExitCode = 0;
	DWORD dwThreadResult = 0;

	m_bExitFlag = TRUE;
//	UpdateData();

	//add 090703
//	cout.rdbuf( m_pOldBuf );
	//wcout.rdbuf( m_pOldBufW );

	m_sm_sound_ctrl.StopCapture();


	if(gGmesCtrl.m_bGmesCom){
		gGmesCtrl.SoketCom_Close();
	}

	
	// add 090902
	if(g_nRunningProcessNo == 1){
		removehook1();
	}
	else{
		removehook2();
	}

	if(m_hReadEvent_S6F5 != NULL)
	{
		CloseHandle(m_hReadEvent_S6F5); m_hReadEvent_S6F5 = NULL;
	}
	if(m_hReadEvent_S6F12 != NULL)
	{
		CloseHandle(m_hReadEvent_S6F12); m_hReadEvent_S6F12 = NULL;
	}
	if(m_hReadEvent_S6F2 != NULL)
	{
		CloseHandle(m_hReadEvent_S6F2); m_hReadEvent_S6F2 = NULL;
	}
	KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);

//	UserControlInit(FALSE);

	if(m_bResultDlgActivate)
	{
		delete m_ResultDlg;
	}

	//+add 090131(수정항목1)
	if(CurrentSet->bUseScanner)
	{
		ScannerCtrl.PortClose(); Sleep(500);
	}

	//====================
	// Close Sound Driver
	//====================
	//090615
//	AudioMeasureStop();


	if(m_bGrabThreadRunning)
	{
		OnGrabStop();
		Sleep(500);
	}
	if(AnalogControl.m_bGrabberInit) AnalogControl.OnPciCardClose();

	if(m_pTestThread)	   TerminateThread(m_pTestThread->m_hThread,dwExitCode);


	if(m_hGrabThreadKillEvent != NULL)
	{
		CloseHandle(m_hGrabThreadKillEvent); m_hGrabThreadKillEvent = NULL;
	}

	AudioMeasureStop();

	SaveRegistrySetting(); 

	// Delete Font Object
	m_CustomFont.DeleteObject();
	m_StatusWindowFont.DeleteObject();
	m_ProgressFont.DeleteObject();

	DeleteFullSeq();
	DeleteStepList();
	DeleteRemoteList();
	DeletePatternList();
	DeleteModelList();

	// Close Pattern Generator Com Port
	if(PatternGeneratorCtrl.m_bPortOpen == TRUE)
	{
		if(m_bTerminalModeOn)
		{
			CString szCmdString;
			szCmdString.Format("%c",END_TERMINAL_MODE);
			PatternGeneratorCtrl.SendCommString(szCmdString);  
			m_bTerminalModeOn = FALSE;
		}
		PatternGeneratorCtrl.PortClose();
	}

	// Close Pattern Generator Com Port
	if(HDMIGeneratorCtrl.m_bPortOpen == TRUE)
	{
		if(HDMIGeneratorCtrl.m_nRemoteMode == 1)
		{
			CString szCmdString;
			szCmdString.Format("%c",END_TERMINAL_MODE);
			PatternGeneratorCtrl.SendCommString(szCmdString);  
			m_bTerminalModeOn = FALSE;
		}
		PatternGeneratorCtrl.PortClose();
	}

	// Close Thermometer Com Port
	if(gThermometerCtrl.m_bPortOpen)
	{
		gThermometerCtrl.CloseComm();
	}
	
	// Close Remote Com Port
	if(TVCommCtrl.m_bPortOpen == TRUE)
	{
		TVCommCtrl.PortClose();
	}

	// Close AvSwitchBox Com Port
	if(AvSwitchBoxCtrl.m_bPortOpen == TRUE)
	{
		CString sTmp = _T("");
		CString sMsg = _T("");
		//090409
		/*
		for(int nTmp = 1; nTmp <= 8; nTmp++)
		{
			if(!AvSwitchBoxCtrl.SetAvSwitch(SET_DIGIT_OUT, MAX_AVSWITCH_WAIT_DELAY, nTmp, 0))
			{
				sTmp.Format("Set DIO Port %d Off %s\n", nTmp, AvSwitchBoxCtrl.m_strErrMsg);
				sMsg += sTmp;
			}
		}
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY, 1, DC_OUT_VOLTAGE_1_0))
		{
			sTmp.Format("Set DC OUT CH1 Volt to 1.0V %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
			sMsg += sTmp;
		}
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY, 2, DC_OUT_VOLTAGE_1_0))
		{
			sTmp.Format("Set DC OUT CH2 Volt to 1.0V %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
			sMsg += sTmp;
		}
		*/
		AvSwitchBoxCtrl.PortClose();
	}

	
	// Close I2C Com Port
	if(gIrCheckerCtrl.m_bPortOpen == TRUE)
	{
		gIrCheckerCtrl.PortClose();
	}

	if(gPciDioCtrl.m_bPCIDIO){
		gPciDioCtrl.Close_DioCard();
	}

	if(gUsbDioCtrl.m_bUsbDio){
		gUsbDioCtrl.Close();
	}


//	UserControlInit(FALSE);

	// Delete CurrentSet
	if(CurrentSet != NULL) delete CurrentSet;

	if(g_GrabImage.m_bImageLoaded)
	{
		g_GrabImage.m_bImageLoaded = FALSE; Sleep(100);
	}
	
	if(g_RefImage.m_bImageLoaded)
	{
		g_RefImage.m_bImageLoaded = FALSE; Sleep(100);
		g_RefImage.UnloadImage();
	}

	if(g_MaskImage.m_bImageLoaded)
	{
		g_MaskImage.m_bImageLoaded = FALSE; Sleep(100);
		g_MaskImage.UnloadImage();
	}

	for(int i=0; i<5; i++)
	{
		if(m_Image[i].m_bImageLoaded)
		{
			m_Image[i].m_bImageLoaded = FALSE; Sleep(100);
			m_Image[i].UnloadImage();
		}
	}

	if (m_pBaseThread != NULL)
	{
		//	VERIFY(::GetExitCodeThread(m_pBaseThread->m_hThread, &dwStatus));
		GetExitCodeThread(m_pBaseThread->m_hThread, &dwStatus);
		if (dwStatus == STILL_ACTIVE)
		{
			nCount++;
			m_pBaseThread->m_bDone = TRUE;
		}
		else
		{
			delete m_pBaseThread;
			m_pBaseThread = NULL;
		}
	}
	if (nCount == 0) CFormView::OnDestroy();
	else PostMessage(WM_DESTROY, 0, 0);	
//	CFormView::OnDestroy();
}

//+add kwmoon 071210
void CDATsysView::SaveRegistrySetting() 
{
	CWinApp* pApp = AfxGetApp();

	CString sTmp					= _T("");
	CString sBaudRate				= _T("");
//	CurrentSet->nTotalNoTest		= g_nNoTested;
//	CurrentSet->nTotalNoNg			= g_nFailed;
	//+add kwmoon 081004
//	CurrentSet->nTotalNoUserStop	= g_nUserStop;

	pApp->WriteProfileString(_T("File"), _T("Current Seq"),					CurrentSet->sSeqPath);
	pApp->WriteProfileString(_T("File"), _T("Current Full Seq"),			CurrentSet->sFullSeqPath);
	pApp->WriteProfileString(_T("File"), _T("Current ModelIni"),			CurrentSet->sModelIni);
	pApp->WriteProfileString(_T("File"), _T("Current Remocon"),				CurrentSet->sRemoconTitle);
	pApp->WriteProfileString(_T("File"), _T("Current Pattern"),				CurrentSet->sPatternTitle);

	pApp->WriteProfileString(_T("File"), _T("Current Chassis"),				CurrentSet->sChassisName);
	pApp->WriteProfileString(_T("File"), _T("Current Model"),				CurrentSet->sModelName);
	pApp->WriteProfileString(_T("File"), _T("Current Model Suffix Name"),	CurrentSet->sModelSuffixName);

	pApp->WriteProfileString(_T("GMES"), _T("Current WorkOrder ID"),		CurrentSet->sWorkOrderID);
	pApp->WriteProfileString(_T("GMES"), _T("Current Line ID"),				CurrentSet->sLineID);
	pApp->WriteProfileString(_T("GMES"), _T("Current Equipment ID"),		CurrentSet->sEquipmentID);
	pApp->WriteProfileString(_T("GMES"), _T("Current Operation ID"),		CurrentSet->sOperationID);
	pApp->WriteProfileString(_T("GMES"), _T("Current Model ID"),			CurrentSet->sModelID);
	pApp->WriteProfileString(_T("GMES"), _T("Current Model Name"),			CurrentSet->sModelName_GMES);
	pApp->WriteProfileString(_T("GMES"), _T("Current Model Suffix"),		CurrentSet->sModelSuffix);


	//+add kwmoon 080724
	pApp->WriteProfileString(_T("File"), _T("Current TestParam"),			CurrentSet->sTestParamIni);

	//+add kwmoon 081024
	pApp->WriteProfileString(_T("File"), _T("Current ModelList"),			CurrentSet->sModelListPath);

	pApp->WriteProfileInt(_T("Run"), _T("Run Type"),						CurrentSet->nRunType);
	pApp->WriteProfileInt(_T("Run"), _T("Start Type"),						CurrentSet->nRunStartType);
	pApp->WriteProfileInt(_T("Display"), _T("Display Type"),				CurrentSet->nDisplayType);
	pApp->WriteProfileInt(_T("Display"), _T("PrintOut Type"),				CurrentSet->nPrintType);
	pApp->WriteProfileInt(_T("Display"), _T("Comm Display"),				CurrentSet->bCommDisplay);
	pApp->WriteProfileString(_T("File"), _T("Current Viewer"),				CurrentSet->sViewerPath);

	pApp->WriteProfileString(_T("Folder"), _T("Current Folder"),			CurrentSet->sModelFolder);
	pApp->WriteProfileString(_T("Folder"), _T("Current Grab Folder"),		CurrentSet->sGrabFolder);
	pApp->WriteProfileString(_T("Folder"), _T("Current Ref Folder"),		CurrentSet->sRefFolder);
	pApp->WriteProfileString(_T("Folder"), _T("Current Mask Folder"),		CurrentSet->sMaskFolder);

	//+add kwmoon 080904
	pApp->WriteProfileString(_T("Folder"), _T("Current Server Folder"),		CurrentSet->sServerFolder);
	//+add PSH 081029
	pApp->WriteProfileString(_T("Folder"), _T("Current Reference Image Root Folder"),		CurrentSet->sRefRootFolder);
	//+add PSH 081211
	pApp->WriteProfileString(_T("Folder"), _T("Current Model Info  Root Foler"),		CurrentSet->sModelInfoFolder);

	pApp->WriteProfileString(_T("Folder"), _T("ParmAdj Current Grab Folder"), CurrentSet->sParmAdjGrabFolder);
	pApp->WriteProfileString(_T("Folder"), _T("ParmAdj Current Ref Folder"),  CurrentSet->sParmAdjRefFolder);
	pApp->WriteProfileString(_T("Folder"), _T("ParmAdj Current Mask Folder"), CurrentSet->sParmAdjMaskFolder);

//	pApp->WriteProfileInt(_T("Config"), _T("LVDS Grabber Width"),			CurrentSet->nLvdsWidth);
//	pApp->WriteProfileInt(_T("Config"), _T("LVDS Grabber Height"),			CurrentSet->nLvdsHeight);
	pApp->WriteProfileInt(_T("Config"), _T("Analog Grabber Width"),			CurrentSet->nAnalogWidth);
	pApp->WriteProfileInt(_T("Config"), _T("Analog Grabber Height"),		CurrentSet->nAnalogHeight);

	pApp->WriteProfileInt(_T("Config"), _T("Use User Retry"),		CurrentSet->nUseUserRetry);

	//+add 090420(Modification No1)
	pApp->WriteProfileInt(_T("Config"), _T("Analog Grabber Type"),			CurrentSet->nAnalogType);

	pApp->WriteProfileString(_T("Config"), _T("Pattern Generator ComPort"), CurrentSet->sPatternComPort);
	sBaudRate.Format("%d", CurrentSet->wPatternBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("Pattern Generator Baud Rate"), sBaudRate);

	pApp->WriteProfileString(_T("Config"), _T("HDMI Generator ComPort"), CurrentSet->sHDMIComPort);
	sBaudRate.Format("%d", CurrentSet->wHDMIBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("HDMI Generator Baud Rate"), sBaudRate);

	pApp->WriteProfileString(_T("Config"), _T("Thermometer ComPort"), CurrentSet->sThComPort);
	sBaudRate.Format("%d", CurrentSet->wThBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("Thermometer Baud Rate"), sBaudRate);
	pApp->WriteProfileInt(_T("Config"), _T("Use Thermometer"),			CurrentSet->bUseTh);
	
	pApp->WriteProfileString(_T("Config"), _T("TV ComPort"),				CurrentSet->sTVComPort);
	//+del psh 080701
//	sBaudRate.Format("%d", CurrentSet->wTVBaudRate);
//	pApp->WriteProfileString(_T("Config"), _T("TV Baud Rate"), sBaudRate);
//-

	pApp->WriteProfileString(_T("Config"), _T("AV SwitchBox ComPort"),		CurrentSet->sAvSwitchBoxComPort);
	sBaudRate.Format("%d", CurrentSet->wAvSwitchBoxBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("AV SwitchBox Baud Rate"), sBaudRate);

	pApp->WriteProfileString(_T("Config"), _T("Scanner ComPort"),			CurrentSet->sScannerComPort);
	sBaudRate.Format("%d", CurrentSet->wScannerBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("Scanner Baud Rate"), sBaudRate);

	//+add kwmoon 080625
	pApp->WriteProfileString(_T("Config"), _T("IR Checker ComPort"),			CurrentSet->sIrChkComPort);
	sBaudRate.Format("%d", CurrentSet->wIrChkBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("IR Checker Baud Rate"), sBaudRate);
	
	pApp->WriteProfileInt(_T("Config"), _T("Use Pattern Generator"),		CurrentSet->bUsePatternGen);
	//pApp->WriteProfileInt(_T("Config"), _T("Use TVComm Port"),				CurrentSet->bUseTVCommPort);
	pApp->WriteProfileInt(_T("Config"), _T("Use AVSwitchingBox"),			CurrentSet->bUseAVSwitchBox);
	pApp->WriteProfileInt(_T("Config"), _T("Use Scanner"),					CurrentSet->bUseScanner);
	pApp->WriteProfileInt(_T("Config"), _T("Use HDMI Generator"),			CurrentSet->bUseHDMIGen);
	pApp->WriteProfileInt(_T("Config"), _T("Use IR Checker"),				CurrentSet->bUseIrChk);

	pApp->WriteProfileInt(_T("Config"), _T("ADC Type"),						(BOOL)(CurrentSet->nAdcType));
	pApp->WriteProfileInt(_T("Config"), _T("Auto Save Grab Image"),			(BOOL)(CurrentSet->bSaveGrabImg));
	pApp->WriteProfileInt(_T("Config"), _T("Auto Save Processed Image"),	(BOOL)(CurrentSet->bSaveProcImg));
	pApp->WriteProfileInt(_T("Config"), _T("Save Caption Test Image"),		(BOOL)(CurrentSet->bSaveCaptionTestImg));
	pApp->WriteProfileInt(_T("Config"), _T("Save Moving Pic Test Image"),	(BOOL)(CurrentSet->bSaveMovingPicTestImg));
	pApp->WriteProfileInt(_T("Config"), _T("Save Intermediate Image"),		(BOOL)(CurrentSet->bSaveIntermediateImage));

	//+del kwmoon 080716
//	pApp->WriteProfileInt(_T("Config"), _T("Save Distribution Table"),		(BOOL)(CurrentSet->bSaveDistributionTable));

	//+add kwmoon 080710
	pApp->WriteProfileInt(_T("Config"), _T("Save Processing Time Data"),	(BOOL)(CurrentSet->bSaveProcessingTimeData));
	
	//+add kwmoon 080716
	pApp->WriteProfileInt(_T("Config"), _T("Save Review Data"),	(BOOL)(CurrentSet->bSaveReviewData));
	pApp->WriteProfileInt(_T("Config"), _T("Save Detail Result Data"),	(BOOL)(CurrentSet->bSaveDetailResultData));


	pApp->WriteProfileInt(_T("Config"), _T("Audio Input Source"),			CurrentSet->nAudioSource);

	pApp->WriteProfileInt(_T("Config"), _T("Save Result to Text File"),		(BOOL)(CurrentSet->bSaveResult2Txt));
	pApp->WriteProfileInt(_T("Config"), _T("Save Result to Html File"),		(BOOL)(CurrentSet->bSaveResult2Html));
	pApp->WriteProfileInt(_T("Config"), _T("Save Result to Csv File"),		(BOOL)(CurrentSet->bSaveResult2Csv));
	
	pApp->WriteProfileInt(_T("Config"), _T("No of repeat execution"),		(UINT)(CurrentSet->nNoRepeatExecution));
	//+del kwmoon 080715
//	pApp->WriteProfileInt(_T("Config"), _T("No of allowed error pixel"),	(UINT)(CurrentSet->nNoAllowedErrorPixel));
	pApp->WriteProfileInt(_T("Config"), _T("No of frames for refimage creation"),		(UINT)(CurrentSet->nNoFrameForRefImageCreation));
	pApp->WriteProfileInt(_T("Config"), _T("Percentage of pixels in middle range"),		(UINT)(CurrentSet->nPixelInMiddleRange));
	pApp->WriteProfileInt(_T("Config"), _T("Percentage of non-black pixels"),			(UINT)(CurrentSet->nNonBlackPixel));

	//+add 090218(Modification No1)
	pApp->WriteProfileInt(_T("Config"), _T("No Used Colors"),			(UINT)(CurrentSet->nNoUsedColors));
	
	//+del kwmoon 080715
//	pApp->WriteProfileInt(_T("Config"), _T("No of allowed error pixel for osd test"),	(UINT)(CurrentSet->nNoAllowedErrorPixelForOsdTest));

	pApp->WriteProfileInt(_T("Config"), _T("Delay Percentage"),				(UINT)(CurrentSet->nDelayPercentage));
//	pApp->WriteProfileInt(_T("Config"), _T("Uranous Grabber Mode"),			CurrentSet->nUranousMode);
	//+add psh 090630
//	pApp->WriteProfileInt(_T("Config"), _T("LVDS OddDE Only"),			(UINT)CurrentSet->bOddDE_Only);

//	pApp->WriteProfileInt(_T("Config"), _T("Color Bit Count"),				CurrentSet->nBitCount);
//	pApp->WriteProfileInt(_T("Config"), _T("Total No of test"),				(UINT)(CurrentSet->nTotalNoTest));
//	pApp->WriteProfileInt(_T("Config"), _T("Total No of Ng"),				(UINT)(CurrentSet->nTotalNoNg));
	
	//+add kwmoon 081004
//	pApp->WriteProfileInt(_T("Config"), _T("Total No of User Stop"),			(UINT)(CurrentSet->nTotalNoUserStop));

	pApp->WriteProfileInt(_T("Config"), _T("No of Internal Retry"),			CurrentSet->nNoRetry);

	//+add PSH 081030
	pApp->WriteProfileInt(_T("Config"), _T("Grab Image Check Area"),			CurrentSet->nGrabCheckArea);

	pApp->WriteProfileInt(_T("Config"), _T("Inspection Process Type"),			CurrentSet->nProcessType);
	
	pApp->WriteProfileInt(_T("Config"), _T("TVComm Port Echo"),				(UINT)CurrentSet->bTVCommPortEcho);
	pApp->WriteProfileString(_T("Config"), _T("HDMI Roi Shape"),			CurrentSet->sHDMIRoiShape);
	pApp->WriteProfileInt(_T("DataBase"), _T("Location"),					(UINT)CurrentSet->nLocation);
	pApp->WriteProfileInt(_T("DataBase"), _T("Line Number"),				(UINT)CurrentSet->nLineNo);
	pApp->WriteProfileInt(_T("DataBase"), _T("System Number"),				(UINT)CurrentSet->nSystemNo);
	pApp->WriteProfileInt(_T("DataBase"), _T("Authority"),					(UINT)CurrentSet->nAuthority);
	pApp->WriteProfileString(_T("DataBase"), _T("PassWord"),				CurrentSet->strPassword);

	pApp->WriteProfileString(_T("DataBase"), _T("GMES Host IP"),			CurrentSet->strGmesIP);
	pApp->WriteProfileInt(_T("DataBase"), _T("GMES Host Port"),				CurrentSet->nGmesPort);
//	pApp->WriteProfileString(_T("DataBase"), _T("MES DSN"),					CurrentSet->strMesDsn);
//	pApp->WriteProfileString(_T("DataBase"), _T("MES USERID"),				CurrentSet->strMesUserId);
//	pApp->WriteProfileString(_T("DataBase"), _T("MES PASSWORD"),			CurrentSet->strMesPassWord);

	pApp->WriteProfileInt(_T("DataBase"), _T("AUTO UPLOAD MES DATA"),		(UINT)CurrentSet->bAutoUploadMes);

	//+add kwmoon 080502
	pApp->WriteProfileInt(_T("DataBase Upload"), _T("UPLOAD MES DATA"),		(UINT)CurrentSet->bUploadMes);
	pApp->WriteProfileInt(_T("AUTO CHANGE MODEL"), _T("AUTO CHANGE MODEL"),		(UINT)CurrentSet->bAutoGMES_ChangeModel);

	//+del kwmoon 080724
//	for(int nTmp = 0;InputDefined[nTmp].pszName != NULL; nTmp++)
//	{
//		pApp->WriteProfileInt(SRCMARGIN_S, InputDefined[nTmp].pszName, CurrentSet->nSrcMargin[nTmp]);
//	}

	//+add kwmoon 080523
	pApp->WriteProfileInt(_T("Config"), _T("AudioCheckMethod"),		(UINT)CurrentSet->nAudioCheckMethod);

	//+add psh 090401
	pApp->WriteProfileInt(_T("Config"), _T("Data Save Period"),		(UINT)CurrentSet->nDataSavePeriod);
	pApp->WriteProfileInt(_T("Config"), _T("Auto Data Delete"),		(UINT)CurrentSet->bAutoDataDelete);

	//+add PSH 0811118
/*	pApp->WriteProfileString(_T("Config"), _T("Tool Option1"),			CurrentSet->sToolOption1);
	pApp->WriteProfileString(_T("Config"), _T("Tool Option2"),			CurrentSet->sToolOption2);
	pApp->WriteProfileString(_T("Config"), _T("Tool Option3"),			CurrentSet->sToolOption3);
	pApp->WriteProfileString(_T("Config"), _T("Tool Option4"),			CurrentSet->sToolOption4);
	pApp->WriteProfileString(_T("Config"), _T("Area Option1"),			CurrentSet->sAreaOption1);
*/
//	pApp->WriteProfileString(_T("Config"), _T("CPU Version"),			CurrentSet->sCPUVersion);
//	pApp->WriteProfileString(_T("Config"), _T("Micom Version"),			CurrentSet->sMicomVersion);
//	pApp->WriteProfileString(_T("Config"), _T("USB Version"),			CurrentSet->sUSBVersion);

	// 090120 External DFT
//	pApp->WriteProfileInt(_T("Config"), _T("Lvds Grabber Type"),		(UINT)CurrentSet->nLvdsGrabberType);

	//+add psh 110226
	pApp->WriteProfileInt(_T("Config"), _T("Remocon Custom Code"),		(UINT)CurrentSet->nRemoteCustomCode);
	pApp->WriteProfileInt(_T("Config"), _T("Remocon Type"),		(UINT)CurrentSet->nRemoteType);

	pApp->WriteProfileInt(_T("Config"), _T("AV SwitchBox Buzzet Mute"),		(UINT)CurrentSet->bAvSwitchBoxBuzzerMute);

	pApp->WriteProfileInt(_T("Config"), _T("AV SwitchBox Initialize Video Mode"),		(UINT)CurrentSet->nAvSwitchBoxInitVideoCh);

	pApp->WriteProfileInt(_T("Config"), _T("Version Check Only"), CurrentSet->bVersionCheckOnly);

	pApp->WriteProfileInt(_T("Config"), _T("Model Info. Check Type"), CurrentSet->nModelInfo_Check);
	pApp->WriteProfileInt(_T("Config"), _T("Fixture ID Check"), CurrentSet->bFixtureIdCheck);
	pApp->WriteProfileInt(_T("Config"), _T("JigUpType"), CurrentSet->nJigUpType); 

	pApp->WriteProfileInt(_T("Config"), _T("Sound Device ID"), CurrentSet->nSoundInDeviceID);

}

void CDATsysView::OnRunAbort() 
{
// change PSH 080603
//	if(m_bThreadRunning)
	if(CurrentSet->bIsRunning)
	{
		int nLeft   = 0;
		int nTop    = 0;
		int nWidth  = 0;
		int nHeight = 0;
//		int nChoice = AfxMessageBox("Do you want to stop the process?", MB_YESNO);

//		if(nChoice == IDYES)
//		{
			CurrentSet->bRunAbort = TRUE;
			
			//+del kwmoon 071215
		//	g_nNoFailedStep++;

			// Modeless Dialog
			m_pStopProcessingDlg = new CStopProcessingDlg(this);
			m_pStopProcessingDlg->Create(IDD_STOP_PROCESSING_DLG);
			m_pStopProcessingDlg->SetMessage(STOP_TEST_PROCESSING_MSG);
	
			nLeft   = m_FormViewRect.left + (m_FormViewRect.Width()/2)  - (m_pStopProcessingDlg->m_dlgRect.Width()/2);
			nTop    = (m_FormViewRect.Height()/2) - (m_pStopProcessingDlg->m_dlgRect.Height()/2);
			nWidth  = m_pStopProcessingDlg->m_dlgRect.Width();
			nHeight = m_pStopProcessingDlg->m_dlgRect.Height();
			
			m_pStopProcessingDlg->ShowWindow(SW_SHOW);
			m_pStopProcessingDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
			m_pStopProcessingDlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
			m_pStopProcessingDlg->UpdateWindow();
			m_pStopProcessingDlg->SetShowDlg(TRUE);
//		}
	}
	else
	{
		AfxMessageBox("Test is not running!");
	}
}

void CDATsysView::OnRunNext() 
{

	CurrentSet->nStepKey = NEXT;
	
}

void CDATsysView::OnRunJump() 
{

	CurrentSet->nStepKey = JUMP;
	
}

void CDATsysView::OnRunPrev() 
{

	CurrentSet->nStepKey = PREV;
}

void CDATsysView::OnRunMakeRef() 
{
	CString sTmp;
	CString sBuf;
	BOOL bUniqueFolderName = FALSE;
	if (m_bResultDlgActivate)
	{
		SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
		//	m_bResultDlgActivate = FALSE;
	}
// change PSH 080603
//	if(!m_bThreadRunning)
	if(!CurrentSet->bIsRunning)
	{
		StartGrabThread();

		m_bMakeReferenceMode = TRUE;
		m_bAdjSpecMode		 = FALSE;

		m_aRefIntFileArray.RemoveAll();

		//+Change PSH 081029
		sBuf = CurrentSet->sRefFolder;
		sBuf.MakeLower(); 
		if(sBuf.Find("\\ref") != -1){
			sTmp = CurrentSet->sRefFolder.Left(CurrentSet->sRefFolder.ReverseFind('\\'));
		//	sBuf.TrimRight("\\ref"); 
		//	sTmp = CurrentSet->sRefFolder.Left(sBuf.GetLength());
		}
		else{
			sTmp = CurrentSet->sRefFolder;
		}
		//+add kwmoon 071214
		m_szMakeRefPath.Format("%s\\Ref",sTmp); 

		if(!FileExists(m_szMakeRefPath))
		{
			CreateFullPath(m_szMakeRefPath);
		}
		//+add kwmoon 080508 : add else statement
		else
		{
			//+add kwmoon 080229
			int nNoCheckedStep = GetNoCheckedStep(CurrentSet->nDisplayType);

			if(nNoCheckedStep == StepList.GetCount())
			{
				CString szString;
				szString.LoadString(IDS_OVERWRITE_REF_IMAGE);

				//090325
				if(AfxMessageBox(szString,MB_YESNO) == IDYES)
				{
					m_bUseDefaultREFfolder = TRUE;
				}
				else
				{
					m_bUseDefaultREFfolder = FALSE;

					for(int i=0; i<500; i++)
					{
						m_szMakeRefPath.Format("%s\\Ref%d",sTmp,i); 

						if(!FileExists(m_szMakeRefPath))
						{
							CreateFullPath(m_szMakeRefPath);
							bUniqueFolderName = TRUE; break;
						}
					}

					if(!bUniqueFolderName)
					{
						AfxMessageBox("Failed to create new REF folder!"); return;
					}
				}
			}
			else
			{
				m_bUseDefaultREFfolder = TRUE;

			}
		}

	//	AfxMessageBox("First, You have to close Explorer & Sequence file!");

		RunTest(1,0);
	}
//	else
//	{
//		AfxMessageBox("Program is alreay runnung!");
//	}
}

//+change 070120 : Change a current function to a Thread-based function 
void CDATsysView::OnRunRun() 
{
	CString sTmp;

	if(CurrentSet->bIsRunMsg) return;
	
	CurrentSet->bIsRunMsg = TRUE;

	if(!CurrentSet->bIsRunning)
	{
		if(gPciDioCtrl.m_bPCIDIO){
			if(gPciDioCtrl.Jig_Ready_Check() != IN_JIGDN)
			{
				AfxMessageBox(IDS_JIG_DOWN);
				CurrentSet->bIsRunMsg = FALSE;	
				return;
			}
		}
		StartGrabThread();

		if(CurrentSet->nNoRepeatExecution != 0)
		{
			m_nNoRepeatExecution = CurrentSet->nNoRepeatExecution;
			m_nNoCurrentRepeatExecution = m_nNoRepeatExecution;
			m_nNoPassInRepeatExecution = 0;
			m_nNoFailInRepeatExecution = 0;
			m_szResultMsg1 = _T("");
			m_szResultMsg2 = _T("");
	
		}

		m_bMakeReferenceMode = FALSE;
		m_bAdjSpecMode		 = FALSE;

		if(m_bResultDlgActivate)
		{
			SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
		//	m_bResultDlgActivate = FALSE;
		}


		RunTest(0,0);
	}
	//return 0;
}

LRESULT CDATsysView::RunTest(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;

	CString sTmp;
	int nVal = 0;

	ComLogClear();
	CurrentSet->sMacAdd = _T("");
	CurrentSet->bMacAddCheck = FALSE;
	if(!CurrentSet->bIsRunning)
	{
		if(CurrentSet->sSeqPath.IsEmpty())
		{
			AfxMessageBox(IDS_ERR_INI); 
			CurrentSet->bIsRunMsg = FALSE; return 0;
		}
		
		if((nType == 0) || (nType == 2)) // 0:Run Test, 1:Make Ref 2:Adjust Spec
		{
			ctrlWipIdEdit.GetWindowText(sTmp);
			
			//+add 090203(Modification No2)
			sTmp.TrimRight(); sTmp.TrimLeft();

			if(sTmp.IsEmpty())
			{
				m_strWipId = _T("");
				gGmesCtrl.m_sSetID = _T("");
				AfxMessageBox("Please Insert Wip ID!");
				CurrentSet->bIsRunMsg = FALSE; return 0;
			}
			else
			{
				sTmp.TrimRight(0x0a); 
				sTmp.TrimRight(0x0d);
				m_strWipId = sTmp; 
				gGmesCtrl.m_sSetID = sTmp;
			//	m_strWipId = "AAX74167611404HI140826";
			//	m_strWipId = "TS372100XL";
				
			}

			if(CurrentSet->bUseScanner)
			{
				if(CurrentSet->bFixtureIdCheck){
					ctrlFixtureID.GetWindowText(sTmp);
					
					sTmp.TrimRight(); sTmp.TrimLeft();
					if(sTmp.IsEmpty())
					{
						m_strWipId = _T("");
						gGmesCtrl.m_sSetID = _T("");
						CurrentSet->sFixtureId = _T("");
						AfxMessageBox("Please Insert Fixture ID!");
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}
					else
					{
						CurrentSet->sFixtureId = sTmp; 			
					}
				}
				if(CurrentSet->bUploadMes && (CurrentSet->nModelInfo_Check == 0)){
					if(!GetToolOption()){
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}
				}
			}
		}

		//090615
		AudioMeasureStop();

		UpdateData(TRUE);
		//if (CurrentSet->bUseScanner && CurrentSet->bUploadMes)
			
		if ( CurrentSet->bUploadMes)
		{
			//m_ctrlSummaryGrid.SetCol(0);
			for(int nTmp = 0; nTmp <= StepList.GetCount(); nTmp++)
			{
				//m_ctrlSummaryGrid.SetRow(nTmp);
				//m_ctrlSummaryGrid.SetCellChecked(flexChecked);
				m_CtrlListMainProcess.SetCheck(nTmp, 1);
				m_LockStatus[nTmp] = 1;
			}
		}

		m_bSeq_Lock = TRUE;

		//m_ctrlSummaryGrid.EnableWindow(FALSE);
		m_pTestThread = AfxBeginThread(StartTestThread,this);

		//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
		CString szString;
		szString.Format("StartTestThread %x\n",m_pTestThread->m_nThreadID);
//		if(g_LogFileOpen) g_LogFile.WriteString(szString);
#endif	
		
		m_bThreadRunning = TRUE;
	}
	return 0;
}


LRESULT CDATsysView::InitDeviceDialog(WPARAM wParam, LPARAM lParam)
{
	int nLeft=0, nTop=0, nWidth=0, nHeight=0;
	int nUsbdio;
	CString sTmp = _T("");
	CString sMsg = _T("");

	BOOL bReturn = TRUE;

	//=================
	// Modeless Dialog
	//=================
	m_pInitDlg = new CInitDlg(this);
	m_pInitDlg->Create(IDD_INIT_DIALOG);
	m_pInitDlg->m_MWnd = this;


	//+add 090220(Modification No4)
	if(m_pInitDlg->m_bInitComplete == FALSE)
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE); return 0;
	}

	nLeft   = m_FormViewRect.left + (m_FormViewRect.Width()/2)  - (m_pInitDlg->m_dlgRect.Width()/2);
	nTop    = (m_FormViewRect.Height()/2) - (m_pInitDlg->m_dlgRect.Height()/2);
	nWidth  = m_pInitDlg->m_dlgRect.Width();
	nHeight = m_pInitDlg->m_dlgRect.Height();
	
	m_pInitDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
	m_pInitDlg->ShowWindow(SW_SHOW);
	m_pInitDlg->UpdateWindow();



	//========================
	// Init Pattern Generator 
	//========================
	CString szCmdString;

	if(CurrentSet->bUsePatternGen)
	{
		sMsg = _T("[P/G] Port Open Check = ");

		if(PatternGeneratorCtrl.m_bPortOpen == FALSE)
		{
			if(PatternGeneratorCtrl.Create(CurrentSet->sPatternComPort, CurrentSet->wPatternBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);

		//+add 090922(Modification No1)
		if(PatternGeneratorCtrl.m_bPortOpen == TRUE)
		{			
			PatternGeneratorCtrl.SetOnOffLine(TRUE);
		
			if(PatternGeneratorCtrl.SetPattern(8))
			{
				m_bTerminalModeOn = TRUE;

				sMsg = "[P/G] On Line Mode On";
			}
			else{
				m_bTerminalModeOn = FALSE;
				sMsg = "[P/G] On Line Mode = Off(Com. Error)- FAIL";
			}
			m_pInitDlg->AddString2List(sMsg);
		}
	}
	//+add 090203(Modification No5)
	m_pInitDlg->m_ctrlProgress.SetPos(10);
	
	//=================
	// Init TVCommPort 
	//=================
	if(CurrentSet->bUseTVCommPort)
	{
		sMsg = _T("[TVCommPort] Port Open Check = ");
		
		if(TVCommCtrl.m_bPortOpen == FALSE)
		{
			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
	}
	//+add 090203(Modification No5)
	m_pInitDlg->m_ctrlProgress.SetPos(20);
	
	//==============
	// Init Scanner
	//==============
	if(CurrentSet->bUseScanner)
	{
		if((g_hCommWnd = GetSafeHwnd()) == NULL)
		{
			AfxMessageBox("View Handle Null!");
		}
		sMsg = _T("[Scanner] Port Open Check = ");

		if(ScannerCtrl.m_bPortOpen == FALSE)
		{
			if(ScannerCtrl.Create(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
	}
	m_pInitDlg->m_ctrlProgress.SetPos(30);

	//========================
	// Init HDMI Generator 
	//========================

	//CurrentSet->bUseHDMIGen = 0;
	if(CurrentSet->bUseHDMIGen)
	{
		sMsg = _T("[HDMI Gen.] Port Open Check = ");

		// Pattern Generator Ctrl Port Open
		if(HDMIGeneratorCtrl.m_bPortOpen == FALSE)
		{
			if(HDMIGeneratorCtrl.Create(CurrentSet->sHDMIComPort, CurrentSet->wHDMIBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
		//+add 090922(Modification No1)
		if(HDMIGeneratorCtrl.m_bPortOpen == TRUE)
		{			
			HDMIGeneratorCtrl.SetOnOffLine(TRUE);
		
			if(HDMIGeneratorCtrl.SetPattern(16))
			{
				HDMIGeneratorCtrl.m_nRemoteMode = ONLINE;

				sMsg = "[HDMI Gen.] On Line Mode = On";
			}
			else{
				HDMIGeneratorCtrl.m_nRemoteMode = FALSE;
				sMsg = "[HDMI Gen.] On Line Mode = Off(Com. Error)- FAIL";
			}
			m_pInitDlg->AddString2List(sMsg);
		}

	}
	m_pInitDlg->m_ctrlProgress.SetPos(40);
	
	// Init DIO 
	//========================
	gJigStatus = INPUT_ERROR;
	sMsg = "DIO Card Open and initializing..."; 
	m_pInitDlg->AddString2List(sMsg);
	
	sMsg = _T("[DIO] Card Open Check = ");
	
	if(gPciDioCtrl.Open_DioCard()){
		gPciDioCtrl.DioOut_Reset();
		if(gPciDioCtrl.Jig_Ready_Check() == IN_JIGDN)
		{
			gJigStatus = IN_JIGDN;
		}
		else{
			gJigStatus = IN_JIGUP;
		}
		sMsg += "PASS";
	}
	else sMsg += "FAIL";
	
	CurrentSet->nJigStatus = gJigStatus;
	
	m_pInitDlg->AddString2List(sMsg);

	if(!gPciDioCtrl.m_bPCIDIO){
		if(gUsbDioCtrl.GetNumDevices(nUsbdio))
		{
			sMsg = _T("[USB DIO  Port Open Check = ");
			
			if(gUsbDioCtrl.m_bUsbDio == FALSE)
			{
				if(gUsbDioCtrl.Create()) sMsg += "PASS";
				else sMsg += "FAIL";
			}
			else sMsg += "PASS";
			
			if(gUsbDioCtrl.m_bUsbDio == TRUE)
			{			
				if(gUsbDioCtrl.WriteByte(0x00))
				{
					sMsg = "[USB DIO All Relay Off : OK]";
				}
				else{
					sMsg = "[USB DIO All Relay Off : Failed]";
				}
			}
			
		}
		m_pInitDlg->AddString2List(sMsg);
	}
	m_pInitDlg->m_ctrlProgress.SetPos(50);

	//==================
	// Init AvSwitchBox      
	//==================
	if(CurrentSet->bUseAVSwitchBox)
	{
		
		//=======================
		// AvSwitchBox Port Open
		//=======================
		if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
		{
			if(!AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate))
			{
				m_pInitDlg->AddString2List(AvSwitchBoxCtrl.m_strErrMsg); goto END_INIT;
			}
		}
		
		//==================
		// AVSwitchBox Init
		//==================
		if(CurrentSet->bAvSwitchBoxBuzzerMute){
			bReturn = AvSwitchBoxCtrl.SetBuzzerMute(TRUE);
			sMsg.Format("[AVSwitch] Buzzer Mute ON : %s",  AvSwitchBoxCtrl.m_strErrMsg);
		}
		else{
			bReturn = AvSwitchBoxCtrl.SetBuzzerMute(FALSE);
			sMsg.Format("[AVSwitch] Buzzer Mute OFF : %s",  AvSwitchBoxCtrl.m_strErrMsg);
		}
		m_pInitDlg->AddString2List(sMsg);

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
		sMsg.Format("[AVSwitch] Set Audio CH 1 %s",  AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);

		if(!bReturn) goto END_INIT;

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nAvSwitchBoxInitVideoCh, 0);
		sMsg.Format("[AVSwitch] Set VIDEO CH 1 %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;

		Sleep(1000);
		bReturn = AvSwitchBoxCtrl.SetVideoOutType(CurrentSet->nAnalogType);
		sMsg.Format("[AVSwitch] Set Video Out Type %s",  AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;
		Sleep(1000);

	}


END_INIT :

	m_pInitDlg->m_ctrlProgress.SetPos(60);

	//sMsg = "[Analog Grabber] Check = ";

	// Close AnalogGrabber
	if(AnalogControl.m_bGrabberInit == TRUE) 
	{
		AnalogControl.OnPciCardClose();
	}

	//=================
	// Init IR Checker 
	//=================
	if(CurrentSet->bUseIrChk)
	{
		sMsg = _T("[IR Checker] Port Open Check = ");
		
		if(gIrCheckerCtrl.m_bPortOpen == FALSE)
		{
			if(gIrCheckerCtrl.Create(CurrentSet->sIrChkComPort, CurrentSet->wIrChkBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
	}
	m_pInitDlg->m_ctrlProgress.SetPos(70);

	//=================
	// Init Thermometer 
	//=================
	if(CurrentSet->bUseTh)
	{
		sMsg = _T("[Thermometer Checker] Port Open Check = ");
		
		if(gThermometerCtrl.m_bPortOpen == FALSE)
		{
			if(gThermometerCtrl.CreateComm(CurrentSet->sThComPort, CurrentSet->wThBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
	}
	m_pInitDlg->m_ctrlProgress.SetPos(80);

	// Open AnalogGrabber
//		if(AnalogControl.OnPciCardOpen(&m_pInitDlg->m_ctrlProgress,70, m_pVideoViewWnd->GetSafeHwnd()) == FALSE) sMsg += "FAIL";
	sMsg = "[Analog Grabber] Check = ";
	if(AnalogControl.OnPciCardOpen() == FALSE) sMsg += "FAIL";
	else sMsg += "PASS";

	m_pInitDlg->AddString2List(sMsg);
	m_pInitDlg->m_ctrlProgress.SetPos(90);

	if(CurrentSet->bUploadMes){
		sMsg = "[GMES Connection] Check = ";
		if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
			sMsg += "PASS";
			sTmp = gGmesCtrl.MakeElem_S1F1();
			CurrentSet->bGMES_Connection = TRUE;
			m_LedsMesOn.EnableWindow(CurrentSet->bGMES_Connection);
			m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
			if(gGmesCtrl.SendCommString(sTmp))
			{
				gGmesCtrl.DisplyComStatus("S1F1", FALSE);

			//	gGmesCtrl.ReceiveCommString(1000,sTmp);
			//	gGmesCtrl.Paser_S1F2_S2F31(sTmp);
			}
		}
		else sMsg += "FAIL";
		m_pInitDlg->AddString2List(sMsg);
	}

	m_pInitDlg->m_ctrlProgress.SetPos(100);
	sMsg = _T(".....");
	m_pInitDlg->AddString2List(sMsg);
	sMsg = _T("Device Check Complete!!");
	m_pInitDlg->AddString2List(sMsg);

	SetGrabInfo(&g_GrabImage);
	return 0;
}

BOOL CDATsysView::InitAvSwitchController() 
{
	UINT nVal1;
	CString sCustom;
	CString sTmp = _T("");
	CString sMsg = _T("");


	if(!CurrentSet->bUseAVSwitchBox) return TRUE;

	//==================
	// Init AvSwitchBox       
	//==================
	if(AvSwitchBoxCtrl.m_bPortOpen == FALSE)
	{
		AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate);
	}

	// Send <Video/Audio-On> Cmd
	if(AvSwitchBoxCtrl.m_bPortOpen != TRUE)
	{
		if(!AvSwitchBoxCtrl.Create(CurrentSet->sAvSwitchBoxComPort, CurrentSet->wAvSwitchBoxBaudRate))
		{
			return FALSE;
		}
	}
	
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sTmp.Format("Set Audio CH 1 %s!\n",  AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

/*	for(int nTmp = 1; nTmp <= 8; nTmp++)
	{
		if(!AvSwitchBoxCtrl.SetAvSwitch(SET_DIGIT_OUT, MAX_AVSWITCH_WAIT_DELAY,  nTmp, 0))
		{
			sTmp.Format("Set DIO Port %d Off %s!\n", nTmp, AvSwitchBoxCtrl.m_strErrMsg);
			sMsg += sTmp;
		}
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  1, DC_OUT_VOLTAGE_1_0))
	{
		sTmp.Format("Set DC OUT CH1 Volt to 1.0V %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  2, DC_OUT_VOLTAGE_1_0))
	{
		sTmp.Format("Set DC OUT CH2 Volt to 1.0V %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set COMP CH 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set DVBS CH 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}
*/
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nAvSwitchBoxInitVideoCh, 0))
	{
		sTmp.Format("Set VIDEO CH 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}
/*
	if(!AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  CurrentSet->nRemoteType, 0))
	{
		sTmp.Format("Set Remote Type %d %s!\n", CurrentSet->nRemoteType, AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}
*/
/*	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_REMOTE_REPEAT_NO, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sTmp.Format("Set Remote Repeat No %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}
*/
	AvSwitchBoxCtrl.m_nAudioCh = 0;
	AvSwitchBoxCtrl.m_nVideoCh = 0;
	switch(CurrentSet->nRemoteCustomCode)
	{
		case 0:  nVal1 = 0x2C; sCustom = _T("Home Theater");	break;
		case 1:	 nVal1 = 0x10; sCustom = _T("MINI/MICRO");		break;
		case 2:	 nVal1 = 0x2D; sCustom = _T("BD Player");	break;
		default : nVal1 = 0x2C;	sCustom = _T("Home Theater");	break;
	}
	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_CUSTOM_CODE, MAX_AVSWITCH_WAIT_DELAY,  nVal1, 0))
	{
		sTmp.Format("Set Remocon Custom Code %s- %s!\n", sCustom, AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}
	Sleep(500);
	if(!AvSwitchBoxCtrl.SetVideoOutType(CurrentSet->nAnalogType))
	{
		sTmp.Format("Set Video Out Type!\n",  AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	return TRUE;
}

UINT CDATsysView::StartTestThread(LPVOID pParam) 
{
	CDATsysView* pView =(CDATsysView* )pParam;
	pView->m_CurrentStep = 0;
	// RunMode : CONT = 0, ERRORSTEP = 1, STOP = 2, STEP = 3, AUTO = 4
	int nRunMode = CONT;

	BOOL TestEnd = FALSE;

	g_nNoFailedStep  = 0;
	g_nProgressPos	 = 0;
	
	CString szMsg		 = _T("");
	CString szCmdString  = _T("");
	CString szRefSeqFile = _T("");
	CString szTemp1		 = _T("");
	CString szTemp2		 = _T("");
	CString szTemp3		 = _T("");
	CString szSeqPath	 = _T("");
	CString szSeqFile	 = _T("");
	CString sDate		 = _T("");
	CString sTime		 = _T("");
	CString szMsg1		 = _T("");
	CString szMsg2		 = _T("");
	CString szMsg3		 = _T("");
	CString szLog		 = _T("");
	CString szLogFile	 = _T("");

	CTime Time;
	Time  = CTime::GetCurrentTime();
	sDate = Time.Format("[%y_%m_%d]");
	sTime = Time.Format("%HH_%MM_%SS");
	
//	BOOL bRunAdcTest	= FALSE;

	pView->m_bThreadRunning = TRUE;
	
	//+add 090409(Modification No1)
	g_nCurSwitchBoxLogLine = 0;
	g_nCurRemoteLogLine	   = 0;

	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

	CPoint ptPositionShift;
	ptPositionShift.x = 0;
	ptPositionShift.y = 0;

	int		nStepResult = TEST_FAIL;
	int		nIndex		= 0;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	pView->m_Image[I_GRAB].LoadImage(pView->m_szNoImageBmpPath, ctrlTestProgress);
	pView->m_Image[I_REF].LoadImage(pView->m_szNoImageBmpPath, ctrlTestProgress);
	pView->m_Image[I_MASK].LoadImage(pView->m_szNoImageBmpPath, ctrlTestProgress);
	pView->m_Image[I_PROC].LoadImage(pView->m_szNoImageBmpPath, ctrlTestProgress);

//	pView->ComLogClear();

	//pView->m_ctrlSummaryGrid.SetLeftCol(1);
	//pView->m_ctrlSummaryGrid.SetTopRow(1);
	//pView->m_CtrlListMainProcess.setsel

	pView->m_cStepResult.SetBitmap(pView->m_bmpNormal);
	pView->m_szCurrentStatus = pView->m_szVersion;
	pView->m_nCurrentViewImageType = SNAP_IMAGE;


	// if the present sequence file is out of date, compile it again
    if((CurrentSet->bCompiled != TRUE) 
	|| (GetModifyTime(CurrentSet->sSeqPath) != CurrentSet->lTime))
    {
        int nChoice = AfxMessageBox(IDS_QUERY_COMPILE, MB_YESNO);
        
		switch(nChoice)
        {
            case IDYES :	
			{
				CurrentSet->bCompiled = Prescan(CurrentSet->sSeqPath);
               
				if(CurrentSet->bCompiled == FALSE)
				{
					pView->m_nNoCurrentRepeatExecution = 1; goto END_EXIT;
				}
       		}
			break;
		}
    }

    if((CurrentSet->nTotalStep = StepList.GetCount()) == 0 )
    {
		szMsg.LoadString(IDS_EMPTY_STEP);
		AfxMessageBox(szMsg);
		pView->m_nNoCurrentRepeatExecution = 1; goto END_EXIT;
	}

	pView->GetCheckStep(CurrentSet->nDisplayType);
	pView->InsertStepData2Grid(CurrentSet->nDisplayType);

	// Initailize variables
	CurrentSet->bIsRunning   = TRUE;              
	CurrentSet->bRunAbort	 = FALSE;
	CurrentSet->bSystemBreak = FALSE;
	CurrentSet->nStepKey	 = EMPTY;			

//+ add PSH 080520
	pView->Invalidate();
	pView->PostMessage(UM_UPDATE_DISPLAY,ALL_VIEW, 0);
	_Wait(200);

	g_ImageProc.SetPositionShift(ptPositionShift);
	g_ImageProc.SetAllowedErrorPixel(CurrentSet->fAllowedBandErrorPixelPercentage);
	
	// Get the position of first step-item.
    PosStep = StepList.GetHeadPosition();       
	
	// add 090821
//	pView->m_bSeq_Lock = TRUE;
//	m_ctrlSummaryGrid.EnableWindow(FALSE);
	//-

	// Get a run mode
	nRunMode = CurrentSet->nRunType;            
	
	// Init g_CurSetting Object
	g_CurSetting.InitVariables();
 
    // Disable program closing menu item
//    pView->m_pFrame->GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

 //   pView->m_pFrame->GetMenu()->EnableMenuItem(ID_APP_EXIT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	//090615
	//if(!g_pView->wIn_Flag) g_pView->WaveRead_Start();
//	g_pView->AudioMeasureStart();

    Start = clock();

//	szTemp1.Format(_T("%s\\Data\\%s\\%s\\%s"),pView->m_szExePath, sDate, CurrentSet->sChassisName, CurrentSet->sModelName);
	if(CurrentSet->sModelSuffixName.IsEmpty()){
		szTemp1.Format(_T("%s\\Data\\%s\\%s\\%s"),pView->m_szExePath, sDate, CurrentSet->sChassisName, CurrentSet->sModelName);
	}
	else{
		szTemp1.Format(_T("%s\\Data\\%s\\%s\\%s"),pView->m_szExePath, sDate, CurrentSet->sChassisName, CurrentSet->sModelSuffixName);
	}
	
/*	if(g_nRunningProcessNo == 1)
	{
		szTemp1 = szTemp1 + _T("\\Jig1");
	}
	else{
		szTemp1 = szTemp1 + _T("\\Jig2");
	}
*/
	szTemp2.Format(_T("%s\\%s_%s_%d"), szTemp1, m_strWipId, sTime, g_nRunningProcessNo);

//	CurrentSet->sGrabFolder			  = szTemp1 + "\\" + sTime + "_" + m_strWipId + "\\";  
//	CurrentSet->sResultFolder		  = CurrentSet->sGrabFolder;
//	CurrentSet->sResultPath			  = CurrentSet->sResultFolder + sTime + ".dat";
//	CurrentSet->sDetailResultDataPath = CurrentSet->sResultFolder + sTime + ".inf";

	CurrentSet->sGrabFolder			  = szTemp2;  
	CurrentSet->sResultFolder		  = szTemp2;
	CurrentSet->sResultPath			  = szTemp2 + ".dat";
	CurrentSet->sDetailResultDataPath = szTemp2 + ".inf";

	CurrentSet->sResultSummaryPath    = szTemp1 + "\\" + "TestSummary.dat";

//	if(!FileExists(CurrentSet->sGrabFolder))   CreateFullPath(CurrentSet->sGrabFolder);
//	if(!FileExists(CurrentSet->sResultFolder)) CreateFullPath(CurrentSet->sResultFolder);
	if(!FileExists(szTemp1)) CreateFullPath(szTemp1);


	// Update a current status window
	if(g_pView->m_bMakeReferenceMode) 
	{
		pView->m_szCurrentStatus = _T("REF-IMG"); 
	}
	else if(g_pView->m_bAdjSpecMode)
	{
		pView->m_szCurrentStatus = _T("REF-AUD"); 
	}
	else
	{
		pView->m_szCurrentStatus = _T("TESTING"); 
	}
	pView->SendMessage(UM_UPDATE_DISPLAY,STATUS, 0);

	// Initalize a test result for all steps
	// Set m_bResult TRUE, Set m_fMeasured 0.0
    ResetStep();


	// Initialize variables
	ctrlTestProgress.SetRange(0,100);
	ctrlTestProgress.SetPos(g_nProgressPos);

	if(CurrentSet->nRunType == STEP)
	{
		CurrentSet->nStepKey = NEXT;
	}

	if(gPciDioCtrl.m_bPCIDIO){
		gPciDioCtrl.DioOut_Reset();
	}
	else{
		gUsbDioCtrl.WriteByte(0x00);
	}
	g_pView->AudioMeasureStart();

	while(PosStep != NULL && CurrentSet->bIsRunning && !TestEnd)
	{

		pView->m_bResultDlgActivate = FALSE;


		g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition);
		g_ImageProc.m_bInvalidateInvalidRegion = FALSE;
		//add 20101206
		AnalogControl.OverlayClear();
	
		if(CurrentSet->bRunAbort)
		{
			pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
		}
		
		StepStart = clock();

		switch(CurrentSet->nRunType)
		{
			case CONT : case AUTO : 
			{
				if(CurrentSet->nStepKey == PREV)
				{
					if(PosStep != StepList.GetHeadPosition())
					{
						pCurStep = StepList.GetPrev(PosStep);
					}
                    else 
					{
						pCurStep = StepList.GetHead();
					}
					CurrentSet->nStepKey = EMPTY;
					pView->m_Display_StepTime = 1;
					if((nStepResult = pView->StepRun()) == TEST_ABORT)
					{
						pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
					}
				}
				else // Normal Process
				{
					pCurStep = StepList.GetNext(PosStep);
					pView->m_Display_StepTime = 1;
					//==================
					// Run Current Step 
					//==================
					if((nStepResult = pView->StepRun()) == TEST_ABORT) // TEST_ABORT
					{
						pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
					}
					else // TEST_PASS or TEST_FAIL
					{
						// TEST_FAIL
						//change 090305
					//	if(pCurStep->GetResult() == FALSE)
						if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST)
						{
							// Internal Retry
							if(CurrentSet->nNoRetry > 0)
							{
								if(!pView->RunInternalRetry()) goto END_WHILE_LOOP;
							}

							// User Retry
							if((pCurStep->GetResult() == FALSE) && (!CurrentSet->nUseUserRetry))
							{
								if(!pView->RunUserRetry(nStepResult)) goto END_WHILE_LOOP;
							}
						}
					}
				}
			}
			break;
			
			case ERRORSTEP: 
			{
                pCurStep = StepList.GetNext(PosStep);
				pView->m_Display_StepTime = 1;
				if((nStepResult = pView->StepRun()) == TEST_ABORT)				
				{
					pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
				}
				if(pCurStep->m_bResult != TRUE) CurrentSet->nRunType = STEP;
			}
			break;

			case STEP : 
			{
				if(CurrentSet->nStepKey == NEXT)
				{
					if(PosStep !=  StepList.GetTailPosition())
					{
                        pCurStep = StepList.GetNext(PosStep);
					}
                    else 
					{
						TestEnd = TRUE; 
						pCurStep = StepList.GetTail();
					}
				}
				else if(CurrentSet->nStepKey == PREV)
				{
					if(PosStep != StepList.GetHeadPosition())
					{
						pCurStep = StepList.GetPrev(PosStep);
					}
                    else pCurStep = StepList.GetHead();
				}
				else if(CurrentSet->nStepKey == JUMP)
				{
					CJumpStep dlg;

					if(dlg.DoModal() == IDOK)
					{
                        PosStep = StepList.FindIndex(dlg.m_nChoice);
                        pCurStep = StepList.GetAt(PosStep);
					}
					else
					{
						CurrentSet->nStepKey = EMPTY;
						continue;
					}
				}
				else break;	

				CurrentSet->nStepKey = EMPTY;
			
			}
			break;

			case STOP: 
			{
				pCurStep = StepList.GetNext(PosStep);
				pView->m_Display_StepTime = 1;

				//==================
				// Run Current Step 
				//==================
				if((nStepResult = pView->StepRun()) == TEST_ABORT) // TEST_ABORT
				{
					pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
				}
				else // TEST_PASS or TEST_FAIL
				{
					// TEST_FAIL
					//change 090305
				//	if(pCurStep->GetResult() == FALSE)
					if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST)
					{
						// Internal Retry
						if(CurrentSet->nNoRetry > 0)
						{
							if(!pView->RunInternalRetry()) goto END_WHILE_LOOP;
						}

						// User Retry
						if((pCurStep->GetResult() == FALSE) && (!CurrentSet->nUseUserRetry))
						{
							if(!pView->RunUserRetry(nStepResult)) goto END_WHILE_LOOP;
						}
					}
				}
			}
			break;
		}

		if((CurrentSet->nRunType == STOP) && (pCurStep->GetResult() == FALSE))
		{
			pView->m_szCurrentStatus = _T("STOPPED"); 
			pView->SendMessage(UM_UPDATE_DISPLAY,STATUS, 0); 
			
			TestEnd = TRUE;
		}
		static clock_t lsclock = clock();
		pCurStep->SetElapsedTime((double)(clock() - StepStart) / CLOCKS_PER_SEC);
		pView->m_Display_StepTime = 0;
		CString lsStepTime;
		lsStepTime.Format("%2.1f", pCurStep->m_fElapsedTime);
		pView->UpdateGridStepTime(lsStepTime, pCurStep->m_nStep);
		//m_CtrlListMainProcess.SetItem(StepList.GetCount(), 3, LVIF_TEXT, sElapsedTime, NULL, NULL, NULL, NULL);
		if(CurrentSet->bSaveReviewData) SaveTestResult(CurrentSet->sResultPath);

		pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);


	}

END_WHILE_LOOP :

	//add 20110121
	AnalogControl.OverlayClear();
#if 0
	if(gPciDioCtrl.m_bPCIDIO){
		gPciDioCtrl.DioOut_Reset();
	}
	else{
		gUsbDioCtrl.WriteByte(0x00);
	}
#endif
	if(FindResult())
		g_nNoFailedStep = 0;
	else
		g_nNoFailedStep = 1;
	

	if(CurrentSet->bUseScanner)
	{
		if(m_strWipId == pView->m_strWipId_Old && pView->m_nOldFailedStep > 0  && g_nNoFailedStep == 0)
		{
			g_nFailed--;
		}
		if(m_strWipId != pView->m_strWipId_Old)
		{
			g_nNoTested++;
			pView->m_strWipId_Old = m_strWipId;

			if(!CurrentSet->bRunAbort)
			{
				if(g_nNoFailedStep != 0) g_nNormalFailed++;  //g_nFailed++;

			}
			else
			{
				g_nUserStop++;
			}
			if(g_nNoFailedStep != 0) g_nFailed++;
		}
		pView->m_nOldFailedStep = g_nNoFailedStep;
	}
	else
	{
		g_nNoTested++;
	
		if(!CurrentSet->bRunAbort)
		{
			if(g_nNoFailedStep != 0) g_nNormalFailed++;  //g_nFailed++;
		}
		else
		{
			g_nUserStop++;
		}
		if(g_nNoFailedStep != 0) g_nFailed++;
	}
	sTime.Format("%5.1f",(double)(clock() - Start)/(CLOCKS_PER_SEC));
	sTested.Format("%d", g_nNoTested);
    sFailed.Format("%d", g_nFailed);
    sUserStop.Format("%d", g_nUserStop);

	pView->UpdateInfo(TIME, sTime);

	if(g_nNoTested > 0)
	{
		g_dRate =((double)g_nFailed /(double)g_nNoTested);
		g_dRate = g_dRate*100.0;
		sRate.Format("%4.1f", g_dRate);
	}

	if(g_nNoFailedStep == 0)
	{
		pView->m_nNoPassInRepeatExecution++;
	}
	else
	{
		pView->m_nNoFailInRepeatExecution++;
	}

	//090615
	pView->AudioMeasureStop();
	//if(g_pView->wIn_Flag) g_pView->WaveRead_Stop();

	CurrentSet->bCommDisplay = FALSE;
	pView->InitAvSwitchController();
	CurrentSet->bCommDisplay = TRUE;
	
	if(CurrentSet->bRunAbort)
	{
		pView->m_nNoCurrentRepeatExecution = 1;
	}
	
	pView->m_szResultMsg1 = _T("");
	pView->m_szResultMsg2 = _T("");

	if(g_pView->m_bMakeReferenceMode) g_pView->SaveMakeRefData();
	if(g_pView->m_bAdjSpecMode) g_pView->SaveAdjSpecData();
	
//#ifdef _TIME_ANALYSIS
//	SaveRemoteLog2File(CurrentSet->sResultPath, CurrentSet->bSaveResult2Txt, CurrentSet->bSaveResult2Html, CurrentSet->bSaveResult2Csv);
//#endif

	ctrlTestProgress.SetPos(100);


END_EXIT:

	StepInforInit();

	pView->m_szCurrentStatus	= pView->m_szVersion;
	
	pView->PostMessage(UM_UPDATE_DISPLAY,STATUS, 0);


	pView->Invalidate();

	if(pView->m_pStopProcessingDlg != NULL)
	{
		pView->SendMessage(UM_CLOSE_STOP_PROCESSING_DLG,0,0); _Wait(100);
	}

	if((!g_pView->m_bMakeReferenceMode) && (!g_pView->m_bAdjSpecMode))
	{
		if(g_nNoFailedStep == 0)
		{
			// Set Result OK
			if(CurrentSet->nChipType == CHIP_BCM){
				TVCommCtrl.Send_ResultKey(TRUE);
			}
			else{
				Send_ResultMpKey(TRUE);
			}
			pView->InsertTotalData2Grid(DETAILEDGRID, TRUE, sTime);
		}
		else
		{
			// Set Result NG
			if(CurrentSet->nChipType == CHIP_BCM){
				TVCommCtrl.Send_ResultKey(FALSE);
			}
			else{
				Send_ResultMpKey(FALSE);
			}
			pView->InsertTotalData2Grid(DETAILEDGRID, FALSE, sTime);
		}
	}
	if(CurrentSet->bRunAbort)
	{
		szMsg.Format(_T("%s : User Stop"), m_strWipId);
		pView->InsertPcbid2Grid(m_strWipId);
	}
	else{
		pView->InsertPcbid2Grid(m_strWipId);
	}

	TestCountSave(g_pView->m_szExePath);

	//===========================
	// Create Detail Result File 
	//===========================
	if(CurrentSet->bSaveDetailResultData)
	{
		SaveDetailTestResult();
		SaveAdcAdjResult();
	}

	//=========================
	// Create Result HTML File 
	//=========================
	pView->SaveResult2File(CurrentSet->sResultPath, CurrentSet->bSaveResult2Txt, CurrentSet->bSaveResult2Html, CurrentSet->bSaveResult2Csv);

	pView->LogDataSave(CurrentSet->sResultPath);

	if(CurrentSet->nChipType == CHIP_BCM){
		Sleep(2000);
	}

	//===================
	// Show Result Diaog
	//===================
	if(pView->m_nNoRepeatExecution == 1)
	{
		if(!CurrentSet->bRunAbort && !CurrentSet->bSystemBreak){
			if(!pView->m_bMakeReferenceMode && !pView->m_bAdjSpecMode)
			{
				pView->m_szResultMsg1.Format("PCBA ID : %s",m_strWipId);
				pView->m_szResultMsg2.Format("Elased Time : %s(s) : %d",sTime, g_nNoFailedStep);
			}
			if(g_nNoFailedStep == 0)
			{
				if(!pView->m_bMakeReferenceMode && !pView->m_bAdjSpecMode){

					if(CurrentSet->bVersionCheckOnly)
					{
						if(gPciDioCtrl.m_bPCIDIO){
							gPciDioCtrl.PLC_ResultSignalOut(TRUE);
						}
						else{
							gUsbDioCtrl.Set_Relay(1, TRUE);
							gUsbDioCtrl.Set_Relay(2, TRUE);
							Sleep(400);
							gUsbDioCtrl.Set_Relay(1, FALSE);
							gUsbDioCtrl.Set_Relay(2, FALSE);
						}
					}
					else{
						if(CurrentSet->nJigUpType != 2){
							if(gPciDioCtrl.m_bPCIDIO){
								gPciDioCtrl.Jig_Up();
							}
						}

					}
				}
				pView->PostMessage(UM_RESULT_DISPLAY,1,0);
			}
			else
			{
				if(!pView->m_bMakeReferenceMode && !pView->m_bAdjSpecMode){ 
			
					if(CurrentSet->bVersionCheckOnly)
					{
						if(gPciDioCtrl.m_bPCIDIO){
							gPciDioCtrl.PLC_ResultSignalOut(TRUE);
						}
						else{
							gUsbDioCtrl.Set_Relay(3, TRUE);
							gUsbDioCtrl.Set_Relay(4, TRUE);
							Sleep(400);
							gUsbDioCtrl.Set_Relay(3, FALSE);
							gUsbDioCtrl.Set_Relay(4, FALSE);
						}
					}
					else{
						if(CurrentSet->nJigUpType == 1){
							if(gPciDioCtrl.m_bPCIDIO){
								gPciDioCtrl.Jig_Up();
							}
						}
					}
				}
				pView->PostMessage(UM_RESULT_DISPLAY,0,0);
			}
		}
		else{
			pView->m_szResultMsg1.Format("PCBA ID : %s",m_strWipId);
			pView->PostMessage(UM_RESULT_DISPLAY,2,0);
		}
	}
	//======================
	// Repetative Operation
	//======================
	else
	{
		if(pView->m_nNoCurrentRepeatExecution == 1)
		{
			if(!pView->m_bMakeReferenceMode && !pView->m_bAdjSpecMode)
			{
				pView->m_szResultMsg1.Format("No Repeat : %d ( Pass:%d,Fail:%d )",
				pView->m_nNoRepeatExecution, pView->m_nNoPassInRepeatExecution, pView->m_nNoFailInRepeatExecution); 
				pView->m_szResultMsg2.Format("Elased Time : %s(s)",sTime);
			}
			
			if(pView->m_nNoFailInRepeatExecution == 0)
			{
				pView->PostMessage(UM_RESULT_DISPLAY,1,0);
			}
			else
			{
				pView->PostMessage(UM_RESULT_DISPLAY,0,0);
			}
		}
	}	

	if(g_nNoFailedStep == 0)
	{
		pView->PlayWavSound(IDR_WAVE_FINISH);
	}
	else{
		pView->PlayWavSound(IDR_WAVE_NG);
	}

	// Repetative Operation
	if(pView->m_nNoRepeatExecution != 1)
	{
		pView->m_nNoCurrentRepeatExecution--;

		if(pView->m_nNoCurrentRepeatExecution >= 1)
		{
			pView->PostMessage(UM_RUN_TEST,0,0);		
		}
	}
//	pView->SetTimer(2, 1000, NULL);

	//====================
	// Upload Data to MES
	//====================
	if(!CurrentSet->bRunAbort && !CurrentSet->bSystemBreak){
		if((!g_pView->m_bMakeReferenceMode) && (!g_pView->m_bAdjSpecMode))
		{

			if(g_nNoFailedStep == 0)
			{
	//			if(CurrentSet->bUploadMes)
				if(CurrentSet->bUploadMes && CurrentSet->bUseScanner)
				{
	//				pView->PostMessage(UM_UPLOAD_MES_DATA,1,0);
					pView->SendMessage(UM_UPLOAD_MES_DATA,1,0);
				}
			}
			else
			{

	//			if(CurrentSet->bUploadMes)
				if(CurrentSet->bUploadMes && CurrentSet->bUseScanner)
				{
	//				pView->PostMessage(UM_UPLOAD_MES_DATA,0,0);
					pView->SendMessage(UM_UPLOAD_MES_DATA,0,0);
				}
			}
		}
	}
//	pView->AudioMeasureStart();

/*	if(CurrentSet->bUseScanner){
		m_strWipId = _T("");
		ctrlWipIdEdit.SetWindowText(_T(""));

		ctrlWipIdEdit.SetFocus();
	}
*/
	CurrentSet->nRunType   = nRunMode;
	CurrentSet->bIsRunning = FALSE;
	CurrentSet->bRunAbort  = FALSE;
	
	CurrentSet->bIsRunMsg	= FALSE;	
	pView->m_bThreadRunning = FALSE;

	pView->m_CurrentStep = 0;

	return 0;
}

void StepInfoDisplay()
{
	CString s;

	ctrlStepTarget_V.SetWindowText(_T(""));
	ctrlStepMeasur_V.SetWindowText(_T(""));
	ctrlStepTarget_A.SetWindowText(_T(""));
	ctrlStepMeasur_A.SetWindowText(_T(""));
	
	s.Format(" (%d)", pCurStep->GetStepNo());
	s = s + pCurStep->GetStepName();

	ctrlTestStep.SetWindowText(s); 

	g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpRun);

	//+add kwmoon 080419
	ctrlTestStep.Invalidate();
	ctrlTestStep.UpdateWindow();
	
	if(pCurStep->GetFuncType() == MEAS_BOOL)
	{	
		if(pCurStep->GetNominal_Bool() == FALSE)
			s.Format("%s", "FALSE");
		else
			s.Format("%s", "TRUE");
    }
	else
	{
		s.Format("%5.1f∼%5.1f", pCurStep->GetLowLimit(), pCurStep->GetHighLimit());
		s = s + pCurStep->GetUnit(); 
		ctrlStepTarget_V.SetWindowText(s);
		ctrlStepTarget_V.Invalidate();
		ctrlStepTarget_V.UpdateWindow();
	}

	if(pCurStep->m_bRunAudioTest == TRUE)
	{
		if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){ 
			s.Format(_T("L:%d, %d"), pCurStep->m_nLeftLevel[0], pCurStep->m_nRightLevel[0]);
		}
		else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
			s.Format(_T("F:%d,%d"), pCurStep->m_nLeftFreq[0], pCurStep->m_nRightFreq[0]);
		}
		else{ 
			s.Format(_T("F:%d,%d/ L:%d, %d"), pCurStep->m_nLeftFreq[0], pCurStep->m_nRightFreq[0], pCurStep->m_nLeftLevel[0], pCurStep->m_nRightLevel[0]);
		}
		ctrlStepTarget_A.SetWindowText(s);
		ctrlStepTarget_A.Invalidate();
		ctrlStepTarget_A.UpdateWindow();
	}

}

int CDATsysView::StepRun()
{
	int nPosition = 0;
	BOOL nResult  = FALSE;
		
	if(pCurStep->m_bTest == FALSE) 
	{
		g_pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

		if(CurrentSet->bSaveReviewData) SaveTestResult(CurrentSet->sResultPath);

		return TEST_PASS;
	}

	m_CurrentStep = pCurStep->m_nStep;
	m_CtrlListMainProcess.Invalidate();
	m_CtrlListMainProcess.SetItem(m_CurrentStep - 1, 2, LVIF_TEXT, _T("RUN"), NULL, NULL, NULL, NULL);
	m_CtrlListMainProcessEx.SetCellBackColor(m_CurrentStep - 1, RGB(255,255,0));


	if(pCurStep->m_bRunAudioTest){
		g_pView->AudioMeasureStop();
	//	_Wait(250);
		g_pView->AudioMeasureStart();
	}	
	//g_pView->m_ctrlSummaryGrid.SetRow(pCurStep->m_nStep);



	PosFunc = pCurStep->m_InstructionList.GetHeadPosition();
	
	AvSwitchBoxCtrl.m_nNoRemoteRetry = 1;
	if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		pCurStep->SetResult(TRUE); 
	}
	
	while(PosFunc != NULL)
	{
		pCurFunc = pCurStep->m_InstructionList.GetNext(PosFunc);
		
		nResult =(*pCurFunc->m_pFunc)();

		if(CurrentSet->bRunAbort) return  TEST_ABORT;
		if(CurrentSet->bSystemBreak) return  TEST_ABORT;
		// If the result is TEST_NG or TEST_ABORT
		if(nResult != TEST_PASS)
		{
			pCurStep->SetResult(FALSE); 
			if(CurrentSet->bRunAbort)nResult = TEST_ABORT;
			g_nNoFailedStep++; 
			pCurStep->SetElapsedTime((double)(clock() - StepStart) / CLOCKS_PER_SEC);
			g_pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

			return nResult;
		}

	}
	nPosition =(int)((double)((++g_nProgressPos)*100)/(double)g_nStepSize);
	ctrlTestProgress.SetPos(nPosition);

	return nResult;
}

void ResetStep()
{
    CStep *pStep;
	POSITION pos;

	//+add kwmoon 071206
	g_nStepSize = 0;

	pos = StepList.GetHeadPosition();
	while(pos != NULL)
	{
		pStep = StepList.GetNext(pos);
        pStep->SetResult(TRUE);
		pStep->m_fMeasured = 0.0;

		//+add kwmoon 071206
		if(pStep->m_bTest)
		{
			g_nStepSize++;
		}

		//+add kwmoon 070807
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++) pStep->m_aNoDifferentPoint[i][j] = 0;
		}
		pStep->m_nSecondDifferentPointFreePercent = -1;

		for(int i=0; i<3; i++)
		{
			for(int j=0; j<MAX_NO_COLOR_BITS; j++)
			{
				pStep->m_aRefNoHighBit[i][j]	= 0;
				pStep->m_aGrabNoHighBit[i][j] = 0;

				if(j < MAX_NO_COLOR_BITS-1)
				{
					pStep->m_aRefNoHighBitPair[i][j]  = 0;
					pStep->m_aGrabNoHighBitPair[i][j] = 0;
					pStep->m_aRefNoLowBitPair[i][j]	  = 0;
					pStep->m_aGrabNoLowBitPair[i][j]  = 0;
				}
			}
		}

		//+add kwmoon 080715
		for(int i=0; i<5;i++)
		{
			pStep->m_nNoBandErrorPixel[i]			= 0;
			pStep->m_nNoAllowedBandErrorPixel[i]	= 0;

			pStep->m_fBandErrorPixelPercentage[i]			= 0.0f;
			pStep->m_fAllowedBandErrorPixelPercentage[i]	= 0.0f;
		}

		//+add kwmoon 080715
		pStep->m_nNoRoiPixel = 0;
		pStep->m_nNoTargetPixel = 0;
		pStep->m_nNoGoodPixel = 0;
		pStep->m_fGoodPixelPercentage = 0.0f;

		//+add kwmoon 081013
		pStep->m_bAdcValueCheckStep = FALSE;
		pStep->m_szAdcValue			= _T("");
	}       
}

BOOL FindResult()
{
	POSITION pos = StepList.GetHeadPosition();
    while(pos != NULL)
	{
		CStep* temp = StepList.GetNext(pos);
        if(!temp->GetResult()) return FALSE;
    }
    return TRUE;        
}

int FindResult2()
{
	int nCount = 0;
	POSITION pos = StepList.GetHeadPosition();
    while(pos != NULL)
	{
		CStep* temp = StepList.GetNext(pos);

		if(temp->m_bTest){
			if(!temp->GetResult()) nCount ++;
		}
    }
    return nCount;        
}

void StepInforInit()
{

	ctrlTestStep.SetWindowText("");
	ctrlStepTarget_V.SetWindowText(_T(""));
	ctrlStepMeasur_V.SetWindowText(_T(""));
	ctrlStepTarget_A.SetWindowText(_T(""));
	ctrlStepMeasur_A.SetWindowText(_T(""));
	g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpNormal);
}

void ResultPrint()
{
//Check  - Print 쪽을 어떻게 할지 결정해야함
//	int lstNo;
//	CString s;
//	CString tps;
//	CString buf;
//	int prtPos;
//
//	lstNo = ctrlResult.GetItemCount();
//	CPrintDialog *pDlg = new CPrintDialog(FALSE);
//	//프린터로 출력시에 출력하는 이름 설정
//	DOCINFO di={ sizeof(DOCINFO),"DATsys",NULL};
//
//	//대화사상자 실행
////	if(pDlg->DoModal()==IDCANCEL)
////	{
////		delete pDlg;
////		return;
////	}
////	pDlg->
//	pDlg->GetDefaults();
//	//프린터 DC를 받는다.
//	HDC hdc= pDlg->GetPrinterDC();
//	if(hdc == NULL)
//	{
//		OkMessage("프린터가 설치되어 있지 않거나, 프린터 정보를 찾을 수 없습니다.");
//		return;
//	}
//	//프린터 DC에 정보를 출력 시작
//	::StartDoc(hdc,&di);
//	//페이지 시작을 알린다.
//	::StartPage(hdc);
//	//데이터 출력
//	tps = _T("                              ");
//	s = _T("Step                   | Nominal| Measure");
//	::TextOut(hdc,0,0,s,s.GetLength());
//
//	s = _T("-----------------------------------------");
//	::TextOut(hdc,0,10,s,s.GetLength());
//	
//	prtPos = 20;
//
//	if(CurrentSet->nPrintType == ERRORVIEW)
//	{
//		for(int i =0; i<lstNo; i++)
//		{
//			buf = ctrlResult.GetItemText(i, 2);
//			if( buf == _T("NG"))
//			{
//				s = ctrlResult.GetItemText(i,0) + _T(" ") + ctrlResult.GetItemText(i,1);
//				if(s.GetLength() > 23)
//					s = s.Left(23);
//				else if(s.GetLength() < 23)
//					s += tps.Left(23 - s.GetLength()); 
//			
//				s +=  _T("| ") + ctrlResult.GetItemText(i,4);
//
//				if(s.GetLength() < 32)
//					s += tps.Left(32 - s.GetLength()); 
//
//				s +=  _T("| ") + ctrlResult.GetItemText(i,3);  
//				::TextOut(hdc,0,prtPos,s,s.GetLength());
//
//				prtPos += 10;
//			}
//		}
//	}
//	else
//	{
//		for(int i =0; i<lstNo; i++)
//		{
//			s = ctrlResult.GetItemText(i,0) + _T(" ") + ctrlResult.GetItemText(i,1);
//			if(s.GetLength() > 23)
//				s = s.Left(23);
//			else if(s.GetLength() < 23)
//				s += tps.Left(23 - s.GetLength()); 
//			
//			s +=  _T("| ") + ctrlResult.GetItemText(i,4);
//
//			if(s.GetLength() < 32)
//				s += tps.Left(32 - s.GetLength()); 
//
//			s +=  _T("| ") + ctrlResult.GetItemText(i,3);  
//			::TextOut(hdc,0,prtPos,s,s.GetLength());
//
//			prtPos += 10;
//		}
//	}
//	s = _T("-----------------------------------------");
//	::TextOut(hdc,0,prtPos,s,s.GetLength());
//	
//	s.Format("%d Step  Failed", FailedStep); 
//	::TextOut(hdc,15,prtPos+10,s,s.GetLength());
//
//	::RestoreDC(hdc,-1);
//	//페이지 종료를 알린다.
//	::EndPage(hdc);
//	//정보출력을 종료 한다.
//	::EndDoc(hdc);
//	
//	delete pDlg;
}

void CDATsysView::OnTimer(UINT_PTR nIDEvent) 
{
	double nLevel_L = 0;
	double nLevel_R = 0;
	double freq, amp, dbamp;
	double nCorrectionValue = 0.97;
	double dElapsedTime = 0;

	static BOOL	bToggleTextColorState;

	//+add 090213(Modification No1)
	CString szString;

	switch(nIDEvent)
	{
		case 1 :
			KillTimer(nIDEvent);

			delete m_pInitDlg;
			//+add psh 090909
			if((g_nRunningProcessNo == 1) && (CurrentSet->bAutoDataDelete))
			{
				ShowSystemMsg(DATA_DELETE_PROCESSING_MSG);
				ResultData_Delete((LONG)CurrentSet->nDataSavePeriod);
				CloseSystemMsg();
			}

			if(g_nRunningProcessNo == 1){
				installhook1(this->GetSafeHwnd());
			}
			else{
				installhook2(this->GetSafeHwnd());
			}
			if(gPciDioCtrl.m_bPCIDIO){
				m_pBaseThread = (CBaseThread*)AfxBeginThread(RUNTIME_CLASS(CBaseThread)
					, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
				m_pBaseThread->SetOwner(this);
				m_pBaseThread->ResumeThread();
			}

			break;

		case 2 :
			KillTimer(nIDEvent);

	//		AudioMeasureStart();
			if(CurrentSet->bUseScanner){
				m_strWipId = _T("");
				ctrlWipIdEdit.SetWindowText(_T(""));
				
				UpdateData(TRUE);

				ctrlWipIdEdit.SetFocus();
			}

			break;

		case TIMER_UPDATE_STATUS_INFO :
			if(CurrentSet->bIsRunning)
			{
				if(bToggleTextColorState)
				{
					m_stcStatus.SetBkColor(RGB(189,231,97));
					m_stcStatus.SetTextColor(RGB(0,0,255)); 
					bToggleTextColorState = FALSE;
				}
				else
				{
					m_stcStatus.SetBkColor(RGB(0,0,255));  
					m_stcStatus.SetTextColor(RGB(255,255,255));
					bToggleTextColorState = TRUE;
				}
				
				dElapsedTime = (double)(clock() - Start)/(CLOCKS_PER_SEC);
				sTime.Format("%5.1f",dElapsedTime);
				if (m_Display_StepTime == 1)
				{
					double ldStepTime = (double)(clock() - StepStart) / CLOCKS_PER_SEC;
					CString lsStepTime;
					lsStepTime.Format("%2.1f", ldStepTime);
					UpdateGridStepTime(lsStepTime, pCurStep->m_nStep);
				}
		//		InvalidateRect(&m_StatusViewClientRect);
				InvalidateRect(&m_ElapsedTimeClientRect);
				
			}
			break;

		case TIMER_MEASURE_AUDIO_OUTPUT :

			//if(g_pView->wIn_Flag)
			//{

			//	EnterCriticalSection(&g_cs);
			//	//g_CriticalSection.Lock();
			//	// LEFT
			//	g_pView->GetWavePeak_L(&freq, &amp, &dbamp);
			//	g_pView->GetPeackDetectorPeak_L(&nLevel_L);

			//	g_pView->SetLedValue_inL((long)freq);
			//	g_pView->SetLedValue_inL_Level((long)(nLevel_L/(2*1.4142)*0.1*nCorrectionValue));

			//	// RIGHT
			//	g_pView->GetWavePeak_R(&freq, &amp, &dbamp);
			//	g_pView->GetPeackDetectorPeak_R(&nLevel_R);
			//	
			//	g_pView->SetLedValue_inR((long)freq);
			//	g_pView->SetLedValue_inR_Level((long)(nLevel_R/(2*1.4142)*0.1*nCorrectionValue));

			//	LeaveCriticalSection(&g_cs);
			//	//g_CriticalSection.Unlock();
			//}
			break;
		
		case TIMER_CONTINUOUS_LVDS_GRAB :
		{
//			ContinuousLVDSGrab_2in1();
			KillTimer(nIDEvent);
			OnGrabStart();			
			_Wait(100);
		}
		break;

		case TIMER_CHECK_LIST_UPDATE:
		{
//			ContinuousLVDSGrab_2in1();
			KillTimer(nIDEvent);
			UpdateSeqCheck();
		
		}
		break;

		//+add kwmoon 071227
		//delete PSH 080603
	/*	case TIMER_UPDATE_ALL_SCREEN :
		{
			if(!m_bThreadRunning || !CurrentSet->bIsRunning || m_ResultDlg.m_bActivate)
			{
				//+change kwmoon 080508
				Invalidate(FALSE); UpdateWindow();
			}
		}
		break;
	*/
		default:
			break;
	}
}

BEGIN_EVENTSINK_MAP(CDATsysView, CFormView)
    //{{AFX_EVENTSINK_MAP(CDATsysView)
	ON_EVENT(CDATsysView, IDC_MAINVIEW_GRID, -600 /* Click */, OnClickMainViewGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

//+add 090213(Modification No1)
#include <tlhelp32.h>
//  Forward declarations:
BOOL ListProcessThreads( DWORD dwOwnerPID );
void printError( TCHAR* msg );


//+add 090213(Modification No1)
void printError( TCHAR* msg )
{
  DWORD eNum;
  TCHAR sysMsg[256];
  TCHAR* p;

  eNum = GetLastError( );
  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         NULL, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         sysMsg, 256, NULL );

  // Trim the end of the line and terminate it with a null
  p = sysMsg;
  while( ( *p > 31 ) || ( *p == 9 ) )
    ++p;
  do { *p-- = 0; } while( ( p >= sysMsg ) &&
                          ( ( *p == '.' ) || ( *p < 33 ) ) );

  // Display the message
  printf( "\n  WARNING: %s failed with error %d (%s)", msg, eNum, sysMsg );
}

//+add 090213(Modification No1)
BOOL ListProcessThreads( DWORD dwOwnerPID ) 
{ 
  CString szMsg = _T("");

  HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
  THREADENTRY32 te32; 
 
  // Take a snapshot of all running threads  
  hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
  if( hThreadSnap == INVALID_HANDLE_VALUE ) 
    return( FALSE ); 
 
  // Fill in the size of the structure before using it. 
  te32.dwSize = sizeof(THREADENTRY32 ); 
 
  // Retrieve information about the first thread,
  // and exit if unsuccessful
  if( !Thread32First( hThreadSnap, &te32 ) ) 
  {
    printError( "Thread32First" );  // Show cause of failure
    CloseHandle( hThreadSnap );     // Must clean up the snapshot object!
    return( FALSE );
  }

  // Now walk the thread list of the system,
  // and display information about each thread
  // associated with the specified process
  do 
  { 
    if( te32.th32OwnerProcessID == dwOwnerPID )
    {
#ifdef _THREAD_DEBUG
      szMsg.Format( "\n\n     THREAD ID      = 0x%08X", te32.th32ThreadID ); 
//	  if(g_LogFileOpen) g_LogFile.WriteString(szMsg);	
      
	  szMsg.Format( "\n     base priority  = %d", te32.tpBasePri ); 
//	  if(g_LogFileOpen) g_LogFile.WriteString(szMsg);	
      
	  szMsg.Format( "\n     delta priority = %d", te32.tpDeltaPri ); 
//	  if(g_LogFileOpen) g_LogFile.WriteString(szMsg);	
#endif
    }
  } while( Thread32Next(hThreadSnap, &te32 ) ); 

//  Don't forget to clean up the snapshot object.
  CloseHandle( hThreadSnap );
  return( TRUE );
}
/*
void CDATsysView::OnAppExit() 
{

	KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);

//	UserControlInit(FALSE);

	if(m_bResultDlgActivate)
	{
		delete m_ResultDlg;
	}

	//+add 090131(수정항목1)
	if(CurrentSet->bUseScanner)
	{
		ScannerCtrl.PortClose(); Sleep(500);
	}
	
	//=======================
	// Stop Auto-Grab Thread
	//=======================
	StopLVDSGrabThread();

	//090615
	AudioMeasureStop();

	//+add 090213(Modification No1)
	DWORD dwExitCode = 0;
	if(m_pGrabImageThread) TerminateThread(m_pGrabImageThread->m_hThread,dwExitCode);
	if(m_pTestThread)	   TerminateThread(m_pTestThread->m_hThread,dwExitCode);

	//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
	ListProcessThreads(GetCurrentProcessId());
#endif
//	if(g_LogFileOpen){ g_LogFile.Close(); g_LogFileOpen = FALSE;}

	_Wait(500);

	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
*/
void CDATsysView::OnClose() 
{
/*	KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);


	if(m_bResultDlgActivate)
	{
		delete m_ResultDlg;
	}

	//+add 090131(수정항목1)
	if(CurrentSet->bUseScanner)
	{
		ScannerCtrl.PortClose(); Sleep(500);
	}
	
	//=======================
	// Stop Auto-Grab Thread
	//=======================
	StopLVDSGrabThread();

	//090615
	AudioMeasureStop();

	//+add 090213(Modification No1)
	DWORD dwExitCode = 0;
	if(m_pGrabImageThread) TerminateThread(m_pGrabImageThread->m_hThread,dwExitCode);
	if(m_pTestThread)	   TerminateThread(m_pTestThread->m_hThread,dwExitCode);

	//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
	ListProcessThreads(GetCurrentProcessId());
#endif
//	if(g_LogFileOpen){ g_LogFile.Close(); g_LogFileOpen = FALSE;}

	_Wait(500);
*/
	delete m_pInitDlg;

	AfxGetMainWnd()->PostMessage(WM_CLOSE);

	CFormView::OnClose();
}


void OkMessage(CString MsgString)
{
	AfxMessageBox(MsgString);
}


void CDATsysView::DrawImage(int nDlgItemNo,CBaseImage& ImageData)
{
	CWnd *pWnd =(CWnd*)GetDlgItem(nDlgItemNo);

	if(!pWnd->GetSafeHwnd())
	{
		OkMessage("Failed to Show Image"); return;
	}
	//add 090818
	EnterCriticalSection(&g_cs2);

	CDC* pDC = pWnd->GetDC();

	CRect rect;
	pWnd->GetWindowRect(&rect);
	pWnd->ScreenToClient(&rect);

	HPALETTE hPalette	 = NULL;          
	HPALETTE hOldPalette = NULL;       

	if(ImageData.cPalette != NULL)
	{	
		hPalette	=(HPALETTE)ImageData.cPalette;
		hOldPalette = ::SelectPalette(pDC->GetSafeHdc(), hPalette, TRUE);
		RealizePalette(pDC->GetSafeHdc());
	}
	else
	{
		if(ImageData.SetSystemPalette(pDC->GetSafeHdc()))
		{
			RealizePalette(pDC->GetSafeHdc());
		}
	}

	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);

	StretchDIBits(pDC->GetSafeHdc(),1,1,rect.Width()-2,rect.Height()-2
				  ,0,0,ImageData.m_BmInfo->bmiHeader.biWidth,ImageData.m_BmInfo->bmiHeader.biHeight
				  ,ImageData.m_pImageData
				  ,ImageData.m_BmInfo,DIB_RGB_COLORS,SRCCOPY);

	
	//+add kwmoon 080617
	if(m_bThreadRunning) DrawRoiBox(pDC,rect,ImageData);

	if(hOldPalette != NULL)
	{
		::SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
	}
	pWnd->UpdateWindow();

	pWnd->ReleaseDC(pDC);

	//add 090818
	LeaveCriticalSection(&g_cs2);
}

//+add kwmoon 080617
void CDATsysView::DrawRoiBox(CDC* pDC, CRect& rect,CBaseImage& ImageData)
{
	if((g_ImageProc.m_nRoiWidth != 0) && (g_ImageProc.m_nRoiHeight != 0))
	{
		CPen whitePen(PS_SOLID, 1, DARK_YELLOW_COLOR);
		CPen blackPen(PS_SOLID, 1, DARK_BLUE_COLOR);
		CPen* pOldPen = NULL;

		float fXratio = (float)rect.Width()/(float)ImageData.m_BmInfo->bmiHeader.biWidth;
		float fYratio = (float)rect.Height()/(float)ImageData.m_BmInfo->bmiHeader.biHeight;

		CRect WhiteRoiRect,BlackRoiRect;

		WhiteRoiRect.left   = (long)((float)(g_ImageProc.m_ptRoiPosition.x) * fXratio - 1);
		WhiteRoiRect.top    = (long)((float)(g_ImageProc.m_ptRoiPosition.y) * fYratio - 1);
		WhiteRoiRect.right  = (long)(WhiteRoiRect.left + 2 + (float)(g_ImageProc.m_nRoiWidth) * fXratio);
		WhiteRoiRect.bottom = (long)(WhiteRoiRect.top  + 2 + (float)(g_ImageProc.m_nRoiHeight) * fYratio);

		if(WhiteRoiRect.left   <= 0) WhiteRoiRect.left = 1;
		if(WhiteRoiRect.right  >= (WhiteRoiRect.left + rect.Width()-1)) WhiteRoiRect.right = (WhiteRoiRect.left + rect.Width()-1)-1;
		if(WhiteRoiRect.top    <= 0) WhiteRoiRect.top  = 1;
		if(WhiteRoiRect.bottom >= (WhiteRoiRect.top + rect.Height()-1)) WhiteRoiRect.bottom = (WhiteRoiRect.top + rect.Height()-1)-1;

		pOldPen = pDC->SelectObject(&whitePen); 
		pDC->MoveTo(WhiteRoiRect.left,WhiteRoiRect.top);
		pDC->LineTo(WhiteRoiRect.right,WhiteRoiRect.top);
		pDC->LineTo(WhiteRoiRect.right,WhiteRoiRect.bottom);
		pDC->LineTo(WhiteRoiRect.left,WhiteRoiRect.bottom);
		pDC->LineTo(WhiteRoiRect.left,WhiteRoiRect.top);

		if((WhiteRoiRect.left-2)   <= 0) BlackRoiRect.left = 2;
		else BlackRoiRect.left = WhiteRoiRect.left-2;

		if((WhiteRoiRect.right+2)  >= (WhiteRoiRect.left + rect.Width()-1)) BlackRoiRect.right = (WhiteRoiRect.left + rect.Width()-1)-1;
		else BlackRoiRect.right = WhiteRoiRect.right+2;

		if((WhiteRoiRect.top-2)    <= 0) BlackRoiRect.top  = 2;
		else BlackRoiRect.top = WhiteRoiRect.top-2; 

		if((WhiteRoiRect.bottom+2) >= (WhiteRoiRect.top + rect.Height()-1)) BlackRoiRect.bottom = (WhiteRoiRect.top + rect.Height()-1)-1;
		else BlackRoiRect.bottom = WhiteRoiRect.bottom+2;

		pOldPen = pDC->SelectObject(&blackPen); 
		pDC->MoveTo(BlackRoiRect.left,BlackRoiRect.top);
		pDC->LineTo(BlackRoiRect.right,BlackRoiRect.top);
		pDC->LineTo(BlackRoiRect.right,BlackRoiRect.bottom);
		pDC->LineTo(BlackRoiRect.left,BlackRoiRect.bottom);
		pDC->LineTo(BlackRoiRect.left,BlackRoiRect.top);

		pDC->SelectObject(pOldPen) ;	
	}
}


//+add kwmoon 070116
void CDATsysView::GetRectSizeInfo()
{
	GetWindowRect(&m_FormViewRect);
	
	m_pVideoViewWnd		= (CWnd*) GetDlgItem(IDC_VIDEOVIEW);
	m_pStatusViewWnd	= (CWnd*) GetDlgItem(IDC_CURRENTSTATUS);
	m_pElapsedTimeWnd	= (CWnd*) GetDlgItem(IDC_TESTTIME);
//	m_pRemoteComLogWnd	= (CWnd*) GetDlgItem(IDC_REMOTE_COM_LOG_GRID);

	m_pVideoViewWnd->GetWindowRect(&m_VideoViewClientRect);
	m_pVideoViewWnd->GetWindowRect(&m_VideoViewScreenRect);

	m_pStatusViewWnd->GetWindowRect(&m_StatusViewClientRect);
	m_pStatusViewWnd->GetWindowRect(&m_StatusViewScreenRect);
	
	m_pElapsedTimeWnd->GetWindowRect(&m_ElapsedTimeClientRect);

//	m_pRemoteComLogWnd->GetWindowRect(&m_RemoteComLogClientRect);

	int nHeight = 0;
	int nWidth  = 0;

	// m_ResultWndRect
/*	nHeight = m_ResultWndClientRect.Height();
	nWidth  = m_ResultWndClientRect.Width();

	m_ResultWndClientRect.top	 -= m_FormViewRect.top;
	m_ResultWndClientRect.left   -= m_FormViewRect.left;
	m_ResultWndClientRect.bottom  = m_ResultWndClientRect.top  + nHeight;
	m_ResultWndClientRect.right   = m_ResultWndClientRect.left + nWidth;
*/
	// m_TestStatusRect
/*	nHeight = m_TestStatusClientRect.Height();
	nWidth  = m_TestStatusClientRect.Width();

	m_TestStatusClientRect.top    -= m_FormViewRect.top;
	m_TestStatusClientRect.left   -= m_FormViewRect.left;
	m_TestStatusClientRect.bottom  = m_TestStatusClientRect.top  + nHeight;
	m_TestStatusClientRect.right   = m_TestStatusClientRect.left + nWidth;
*/
	// m_TestStatusRect
/*	nHeight = m_StatusResultClientRect.Height();
	nWidth  = m_StatusResultClientRect.Width();

	m_StatusResultClientRect.top	-= m_FormViewRect.top;
	m_StatusResultClientRect.left   -= m_FormViewRect.left;
	m_StatusResultClientRect.bottom  = m_StatusResultClientRect.top  + nHeight;
	m_StatusResultClientRect.right   = m_StatusResultClientRect.left + nWidth;
*/
	// m_VideoViewRect
	nHeight = m_VideoViewClientRect.Height();
	nWidth  = m_VideoViewClientRect.Width();

	m_VideoViewClientRect.top	 -= m_FormViewRect.top;
	m_VideoViewClientRect.left   -= m_FormViewRect.left;
	m_VideoViewClientRect.bottom  = m_VideoViewClientRect.top  + nHeight;
	m_VideoViewClientRect.right   = m_VideoViewClientRect.left + nWidth;

	// m_pStatusViewWnd
	nHeight = m_StatusViewClientRect.Height();
	nWidth  = m_StatusViewClientRect.Width();

	m_StatusViewClientRect.top	  -= m_FormViewRect.top;
	m_StatusViewClientRect.left   -= m_FormViewRect.left;
	m_StatusViewClientRect.bottom  = m_StatusViewClientRect.top  + nHeight;
	m_StatusViewClientRect.right   = m_StatusViewClientRect.left + nWidth;

	// m_pElapsedTimeWnd
	nHeight = m_ElapsedTimeClientRect.Height();
	nWidth  = m_ElapsedTimeClientRect.Width();

	m_ElapsedTimeClientRect.top	   -= m_FormViewRect.top;
	m_ElapsedTimeClientRect.left   -= m_FormViewRect.left;
	m_ElapsedTimeClientRect.bottom  = m_ElapsedTimeClientRect.top  + nHeight;
	m_ElapsedTimeClientRect.right   = m_ElapsedTimeClientRect.left + nWidth;

	//+add kwmoon 071215

	// m_pRemoteComLogWnd
//	nHeight = m_RemoteComLogClientRect.Height();
//	nWidth  = m_RemoteComLogClientRect.Width();

//	m_RemoteComLogClientRect.top	-= m_FormViewRect.top;
//	m_RemoteComLogClientRect.left	-= m_FormViewRect.left;
//	m_RemoteComLogClientRect.bottom	 = m_RemoteComLogClientRect.top  + nHeight;
//	m_RemoteComLogClientRect.right	 = m_RemoteComLogClientRect.left + nWidth;
}
//-



//+change 070120 : Change a current function to a Thread-based function 

LRESULT CDATsysView::UpdateDisplay(WPARAM wParam, LPARAM lParam)
{
	int nIndex =(int)wParam;

	UpdateData(FALSE);
	//+change kwmoon 080618
	switch(nIndex)
	{
		case STATUS:
		{
			UpdateInfo(nIndex,m_szCurrentStatus);
		}
		break;

		case ALL_VIEW:
		{
			UpdateWindow();
		}
		break;
	}
	return 0;
}

LRESULT CDATsysView::CloseStopProcessingDlg(WPARAM wParam, LPARAM lParam)
{
	m_pStopProcessingDlg->m_bShowDlg = FALSE;
	m_pStopProcessingDlg->DestroyWindow();
	delete m_pStopProcessingDlg;
	m_pStopProcessingDlg = NULL;
	return 0;
}

//+add kwmoon 080618
LRESULT CDATsysView::CloseResultDlg(WPARAM wParam, LPARAM lParam)
{
	m_ResultDlg->DestroyWindow();
	m_bResultDlgActivate = FALSE;
	m_ResultDlg = NULL;
	return 0;
}

LRESULT CDATsysView::ShowResultDisplay(WPARAM wParam, LPARAM lParam)
{
	int nResult = (int)wParam;
	int nLeft=0, nTop=0, nWidth=0, nHeight=0;
	CRect rect;

	m_ResultDlg = new CResultDlg;

	if(nResult == 0)
	{
		m_ResultDlg->SetImage(IDB_FAIL2_BMP);
		m_ResultDlg->SetTestResultInfo(m_szResultMsg1,m_szResultMsg2);
	} 
	else if(nResult == 1)
	{
		m_ResultDlg->SetImage(IDB_PASS2_BMP);
		m_ResultDlg->SetTestResultInfo(m_szResultMsg1,m_szResultMsg2);
	}
	else 
	{
		m_ResultDlg->SetImage(IDB_CHECK_BMP);
		m_ResultDlg->SetTestResultInfo(m_szResultMsg1,m_szResultMsg2);
	}

	m_ResultDlg->Create(IDD_RESULT_DLG, this);

	m_ResultDlg->GetWindowRect(&rect);

	nLeft   = m_FormViewRect.left + (m_FormViewRect.Width()/2)  - (rect.Width()/2) - 30;
	nTop    = (m_FormViewRect.Height()/2) + 210 ; // - (rect.Height()/2);
	nWidth  = rect.Width();
	nHeight = rect.Height();
	
	m_ResultDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
	m_ResultDlg->ShowWindow(SW_SHOW);
	m_ResultDlg->UpdateWindow();

	m_bResultDlgActivate = TRUE;
	return 0;
}

void CDATsysView::OnOption() 
{
	BOOL bFlag;

	if ((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	COptionDlg dlg("Option");

	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	OnAudioMeasStop();

	Sleep(100);

	if(m_bGrabThreadRunning)
	{
		OnGrabStop();
		Sleep(500);
		if(AnalogControl.m_bGrabberInit) AnalogControl.OnPciCardClose();
	}
	int lDeviceIDOld = CurrentSet->nSoundInDeviceID;

	dlg.DoModal();

	//+add PSH 080527
	if(CurrentSet->bUseScanner)
	{
		ctrlWipIdEdit.SetReadOnly(TRUE);
	}
	else
	{
		ctrlWipIdEdit.SetReadOnly(FALSE);
	}
	UpdateVersionInfo();

	if(AnalogControl.OnPciCardOpen()){
		OnGrabStart();
	}

	if (CurrentSet->nSoundInDeviceID != lDeviceIDOld)
	{
		//CurrentSet->nSoundInDeviceID = lDeviceID;
		m_sm_sound_ctrl.StopCapture();
		m_sm_sound_ctrl.StartCapture(this, CurrentSet->nSoundInDeviceID);

	//	pApp->WriteProfileInt(_T("Config"), _T("Sound Device ID"), CurrentSet->nSoundInDeviceID);
	}

//	if(bFlag) SetTimer(2, 1000, NULL);

}

void CDATsysView::OnImageDialog() 
{
	BOOL bFlag;

	Sleep(200);

	if(CurrentSet->bIsRunning)
	{
		AfxMessageBox("First, Stop running Program"); return;
	}
	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	OnAudioMeasStop();
	
	m_Sound_Display_Enable = 0;
	
	////m_oglWindow->KillTimer(1);
	////m_oglWindow->destroy();
	////m_oglWindow->DestroyWindow();
	////delete m_oglWindow;
	//m_oglWindow.destroy();
	//m_poglWindow = new COpenGLControl();
	//m_poglWindow->oglCreate(rect, this, "OpenGL");
	////// Setup the OpenGL Window's timer to render
	//m_poglWindow->m_unpTimer = m_poglWindow->SetTimer(1, 100, 0);
	m_poglWindow->destroy();
	//m_poglWindow->~COpenGLControl();
	delete m_poglWindow;
	
	Sleep(100);
#if 1
	OnGrabStop();


	CImageTotalDlg dlg;
	dlg.m_nTabType = 0;	// 0 : P_GRAB

	dlg.DoModal();


	//Invalidate(TRUE);
	OnGrabStart();
#endif

	CRect rect;
	//GetDlgItem(IDC_STATIC_INST_DISPLAY)->GetWindowRect(rect);
	//ScreenToClient(rect);

	//// Create OpenGL Control window
	//m_oglWindow = new COpenGLControl();
	//m_oglWindow->oglCreate(rect, this,"OpenGL");
	//m_oglWindow->m_unpTimer = m_oglWindow->SetTimer(1, 100, 0);
	//m_oglWindow->m_DisplayPage = __DISPLAY_PAGE_MAIN;

	GetDlgItem(IDC_STATIC_INST_DISPLAY)->GetWindowRect(rect);
	ScreenToClient(rect);

	m_poglWindow = new COpenGLControl();
	m_poglWindow->oglCreate(rect, this, "OpenGL");
	//// Setup the OpenGL Window's timer to render
	m_poglWindow->m_unpTimer = m_poglWindow->SetTimer(1, 100, 0);
	////m_oglWindow = new COpenGLControl();
	//// Create OpenGL Control window
	//m_oglWindow.oglCreate(rect, this, "OpenGL");
	//// Setup the OpenGL Window's timer to render
	//m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 100, 0);
	m_poglWindow->m_DisplayPage = __DISPLAY_PAGE_MAIN;
	m_Sound_Display_Enable = 1;
	//m_Sound_Display_Enable = 1;

//	if(bFlag) SetTimer(2, 1000, NULL);
}

void CDATsysView::OnSeqEdit() 
{

	BOOL bFlag;

	if(CurrentSet->bIsRunning)
	{
		AfxMessageBox("First, Stop running Program"); return;
	}
	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	OnAudioMeasStop();
	Sleep(100);

	CSeqEditorDlg dlg;
	if(dlg.DoModal())
	{
		if(!CurrentSet->sSeqPath.IsEmpty())
		{
			Prescan(CurrentSet->sSeqPath);
			InsertStepData2Grid(CurrentSet->nDisplayType);
		}		
	}

//	if(bFlag) SetTimer(2, 1500, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CEnvironmentData

CEnvironmentData::CEnvironmentData()
{
    nTotalLine		= 0; 				// TOTAL LINES & TOTAL STEPS
	nTotalStep		= 0;
    nTotalProduct	= 0;		        // Total Products & OK Product
    nOKProduct		= 0;
	nRunType		= 0;              	// Current Run Mode 
	nDisplayType	= 0;				// Current Display type	0 : Main Grid -> Normal Grid, 1 : Main Grid -> Detailed Grid
	nPrintType		= 0;				// Current Print Out type
	nStepKey		= 0;				// StepKey
    lTime			= 0;       			// Last Compile Time

	bIsRunning		= FALSE;			// Running state
	bRunAbort		= FALSE;			// User abort key
    bSystemBreak	= FALSE;            // System break
	bCompiled		= FALSE;			// Parsing result
	bCommDisplay	= FALSE;			// rs232 통신내용 Display
	bIsRunMsg		= FALSE;			// thread 에서 실행 메세지를 보냈는지 여부
	bSaveGrabImg	= FALSE;			// TRUE : Auto Save Grab Image
	bSaveProcImg	= FALSE;			// TRUE : Auto Save Processed Image

	bSaveCaptionTestImg    = FALSE;
	bSaveMovingPicTestImg  = FALSE;
	bSaveIntermediateImage = FALSE;

	//+del kwmoon 080716
//	bSaveDistributionTable = FALSE;

	//+add kwmoon 080710
	bSaveProcessingTimeData= FALSE;

	//+add kwmoon 080716
	bSaveReviewData			= FALSE;
	bSaveDetailResultData	= FALSE;
	
	bSaveResult2Txt = FALSE;			// Save Result to Text File
	bSaveResult2Html= TRUE;// FALSE;	// Save Result to Html File
	bSaveResult2Csv = FALSE;			// Save Result to Csv File
	//-
	nNoRepeatExecution	 = 1;				// Save Number of repeat execution

	//+del kwmoon 080715
//	nNoAllowedErrorPixel = 0;

	//+change kwmoon 080925
//	nNoFrameForRefImageCreation = 2;
	nNoFrameForRefImageCreation = 1;

	nPixelInMiddleRange = 0;
	nNonBlackPixel		= 0;
	
	//+add 090218(Modification No1)
	nNoUsedColors		= 0;

	//+del kwmoon 080715
//	nNoAllowedErrorPixelForOsdTest = 0;

	nDelayPercentage	 =	100;

	//------------------
	// File/Folder Path
	//------------------
	sSeqPath		= _T("");	
	sFullSeqPath	= _T("");	
    sRemoconTitle	= _T("");   
	sPatternTitle	= _T("");   
	sViewerPath		= _T("");	
	sTestParamIni	= _T("");   
	sModelIni		= _T("");			
	sModelFolder	= _T("");	
	sGrabFolder		= _T("");	
	sRefFolder		= _T("");	
	sMaskFolder		= _T("");	
	sResultFolder	= _T("");	
	sResultPath		= _T("");	

	//+add kwmoon 080716
	sDetailResultDataPath = _T("");

	//+add kwmoon 081024
	sModelListPath		  = _T("");	

//	nLvdsWidth		= 0;				// Uranous Grabber Width
//	nLvdsHeight		= 0;				// Uranous Grabber Height
	nAnalogWidth	= 0;				// Matrox Analog Grabber(Cronos+) Width
	nAnalogHeight	= 0;				// Matrox Analog Grabber(Cronos+) Height

	//------------------
	// COM Port
	//------------------
	sPatternComPort		= _T("");
	sTVComPort			= _T("");
	sAvSwitchBoxComPort	= _T("");
	sScannerComPort		= _T("");
	sIrChkComPort		= _T("");
	sHDMIRoiShape		= _T("");

	wPatternBaudRate		= 0;
	//+change kwmoon 080804
//	wTVBaudRate				= 0;
	wTVBaudRate				= 57600;
	nTVParity				= EVENPARITY;
	wAvSwitchBoxBaudRate	= 0;
	wScannerBaudRate		= 0;
	wIrChkBaudRate			= 0;

	bUsePatternGen			= FALSE;
	bUseTVCommPort			= TRUE;
	bUseAVSwitchBox			= FALSE;
	bUseScanner				= FALSE;

	nAdcType				= ADC_TYPE_RS232C; 
	nAudioSource			= 0;					// Audio Input Source 0: LineIn, 1: DigitalIn
//	nUranousMode			= 0;
//	nBitCount				= 0;
//	nTotalNoTest			= 0;
//	nTotalNoNg				= 0;

	//+ change kwmoon 080724
	for(int i = 0; i < NO_SOURCE_TYPE; i++)
	{
		nSrcMargin[i] = 0;
	}

	nNoRetry = 0;
	bTVCommPortEcho			= FALSE;
	
	//------------------
	// MES Data
	//------------------
	nLocation			= 0;		//Location 0 : LG-PRI, 1 : LG-DD, 2 : JINSUNG, 3 : SG TECH
	nLineNo				= 1;		//Line Number 1 ~ 10
	nSystemNo			= 1;		//System Number 1 ~ 10
	nAuthority			= 0;		//Authority 0 : Developer, 1 : Administrator, 2 : Operator
	strPassword			= _T("");	// Developer : Password "5555" , Administrator : Password "DFT"
									// Operator : No Password
	strUserId			= _T("");
//	strMesDsn			= _T("");
//	strMesUserId		= _T("");
//	strMesPassWord		= _T("");
	bAutoUploadMes		= FALSE;
	bUploadMes			= FALSE;
	bGMES_Connection	= FALSE;
	bAutoGMES_ChangeModel = FALSE;

//	sToolOption1		= _T("");
//	sToolOption2		= _T("");
//	sToolOption3		= _T("");
//	sToolOption4		= _T("");
//	sToolOption5		= _T("");
//	sAreaOption1		= _T("");

	sBE_Version			= _T("");
	sFE_Version			= _T("");
	sMicom_Version		= _T("");
	sDSP_Version		= _T("");
	sSubDSP_Version		= _T("");
	sEQ_Version			= _T("");
	sMEQ_Version		= _T("");
	sTouch_Version		= _T("");
	sDemo_Version		= _T("");
	sDJSound_Version	= _T("");
	sWirelessTx_Version	= _T("");
	sWirelessRx_Version	= _T("");
	sChecksum			= _T("");
	

	nAudioCheckMethod	= 1;
//	nFullHd120Hz		= 0; // 0 :
// add 091214
//	nImageRotation		= 0;

	//+add kwmoon 080715
	fAllowedBandErrorPixelPercentage[0] = 0.0f;
	fAllowedBandErrorPixelPercentage[1] = 0.0f;
	fAllowedBandErrorPixelPercentage[2] = 0.0f;
	fAllowedBandErrorPixelPercentage[3] = 0.0f;
	fAllowedBandErrorPixelPercentage[4] = 0.0f;

	//+add kwmoon 080716
	nLevelMargin = 45;

	//+add kwmoon 080819
	nFreqMargin = 15;
	
	//+add kwmoon 080724
	for(int i=0;i<NO_SOURCE_TYPE; i++)
	{
		for(int j=0; j<5; j++)
		{
			nAudioSpec[i][j] = 0;
		}
	}
	
	bUseIrChk			= 0;

	//+add PSH 100310
	nHDMIResolution = 0;
	nHDMIBitSpec = 0;

	bAvSwitchBoxBuzzerMute = 1;
	nAvSwitchBoxInitVideoCh = 0;
	nUseUserRetry = 0;
	bVersionCheckOnly = FALSE;
	sOrganizationID	= _T("");
	sLineID			= _T("");
	sEquipmentID	= _T("");
	sOperationID	= _T("");
	sModelID		= _T("");
	sModelName_GMES	= _T("");
	sModelSuffix	= _T("");
	nDftSeq			= 0;

	nChipType = 0x01;
	nProcessType = 0;
	sFixtureId	= _T("");
	sMacAdd	= _T("");
	nModelInfo_Check = 0;
	bFixtureIdCheck = TRUE;
	bMacAddCheck = FALSE;
}

void CDATsysView::OnClickMainViewGrid() 
{
#if 0
	//+add 090219(Modification No2)
	CString szLog = "B:OnClickMainViewGrid\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	if(CurrentSet->bIsRunning) return;

	int nCol = 0;// m_ctrlSummaryGrid.GetCol();
	int nRow = 0;// m_ctrlSummaryGrid.GetRow();
//	if(nCol > m_ctrlSummaryGrid.GetCols()) return;
//	if(nRow > m_ctrlSummaryGrid.GetRows()) return;

	//+ 2007.9.17 Add BY USY
	BOOL bCellCheck = FALSE;
	//-
	if(CurrentSet->nDisplayType == DETAILEDGRID)
	{
		if((nRow > 0) && (nCol == 0))
		{
			if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
			{
				CString sTmp = _T("");
				sTmp.LoadString(ERROR_ADMIN_LOGIN);
				AfxMessageBox(sTmp); return;
			}
			if(CurrentSet->bUseScanner && CurrentSet->bUploadMes){
				return;
			}
			
			m_ctrlSummaryGrid.SetCol(nCol);
			m_ctrlSummaryGrid.SetRow(nRow);
			if(m_ctrlSummaryGrid.GetCellChecked() == flexChecked) m_ctrlSummaryGrid.SetCellChecked(flexUnchecked	);
			else m_ctrlSummaryGrid.SetCellChecked(flexChecked);
		}
		else if((nRow == 0) && (nCol == 0))
		{
			if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
			{
				CString sTmp = _T("");
				sTmp.LoadString(ERROR_ADMIN_LOGIN);
				AfxMessageBox(sTmp); return;
			}
			if(CurrentSet->bUseScanner && CurrentSet->bUploadMes){
				return;
			}
			m_ctrlSummaryGrid.SetRow(nRow);
			m_ctrlSummaryGrid.SetCol(nCol);
			if(m_ctrlSummaryGrid.GetCellChecked() == flexChecked	) bCellCheck = TRUE;
			else  bCellCheck = FALSE;
			for(int nTmp = 0; nTmp <= StepList.GetCount(); nTmp++)
			{
				m_ctrlSummaryGrid.SetRow(nTmp);
				if(bCellCheck == TRUE) m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
				else m_ctrlSummaryGrid.SetCellChecked(flexChecked);
			}
		}
		else if((nRow > 0) && (nCol > 0))
		{
			m_ctrlSummaryGrid.SetCol(nCol);
			m_ctrlSummaryGrid.SetRow(nRow);

			if(nRow <= StepList.GetCount())
			{
				if(m_bGrabThreadRunning)
				{
					AfxMessageBox("First, Stop Image Grabbing"); return;
				}

				POSITION PosStep = StepList.FindIndex(nRow-1);

				pCurStep = StepList.GetAt(PosStep);

				if(pCurStep->m_strGrabPath != _T(""))
				{
					if(m_Image[I_GRAB].m_bImageLoaded)
					{
						m_Image[I_GRAB].MakeBlankImage();
					}

					if(!OpenImgFile(pCurStep->m_strGrabPath,I_GRAB))
					{
						m_Image[I_GRAB].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);
					}
				}

				if(pCurStep->m_strRefPath != _T(""))
				{
					if(m_Image[I_REF].m_bImageLoaded)
					{
						m_Image[I_REF].MakeBlankImage();
					}

					if(!OpenImgFile(pCurStep->m_strRefPath,I_REF))
					{
						m_Image[I_REF].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);
					}
				}

				if(pCurStep->m_strProcPath != _T(""))
				{
					if(m_Image[I_PROC].m_bImageLoaded)
					{
						m_Image[I_PROC].MakeBlankImage();
					}

					if(!OpenImgFile(pCurStep->m_strProcPath,I_PROC))
					{
						m_Image[I_PROC].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);
					}
				}
				InvalidateRect(&m_VideoViewClientRect);
			}

		}
	}

	//+add 090219(Modification No2)
	szLog = "E:OnClickMainViewGrid\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);
#endif
}

BOOL CDATsysView::OpenImgFile(CString sImgPath, int nImgType)
{
	CString sTmp;

	if(!FileExists(sImgPath))
	{
		//+add 080617 : add if statement
		if(nImgType != I_PROC)
		{
			sTmp.Format("Failed to find %s!", sImgPath);

			AfxMessageBox(sTmp); 
		}
		return FALSE;
	}

	if(!m_Image[nImgType].LoadImage(sImgPath, ctrlTestProgress))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDATsysView::PreTranslateMessage(MSG* pMsg) 
{
	//if (pMsg->message == NM_CLICK)
	//{
	//	return TRUE;
	//}

	if (m_hAccel != NULL)
    {
		if(TranslateAccelerator(this->m_hWnd, m_hAccel, pMsg))
			return TRUE;
    }


	return CFormView::PreTranslateMessage(pMsg);
}
void CDATsysView::OnSelchangeViewTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nViewNo = m_ctrlViewTab.GetCurSel();
	
	switch(nViewNo)
	{
		
		case SNAP_IMAGE :	// Grab
		{
			m_nCurrentViewImageType = SNAP_IMAGE;
		}
		break;

		case LOAD_IMAGE :	// Reference
		{
			m_nCurrentViewImageType = LOAD_IMAGE;
		}
		break;

		case PROC_IMAGE :	// Processed
		{
			m_nCurrentViewImageType = PROC_IMAGE;
		}
		break;
	}

	InvalidateRect(&m_VideoViewClientRect);

	*pResult = 0;
}

void CDATsysView::OnRunAdjSpec() 
{
	CString sTmp = _T("");
	BOOL bUniqueFolderName = FALSE;

	//+add kwmoon 080910
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}

// change PSH 080603
//  if(!m_bThreadRunning)
	if(!CurrentSet->bIsRunning)
	{
		StartGrabThread();

		m_bMakeReferenceMode = FALSE;
		m_bAdjSpecMode		 = TRUE;

		RunTest(2,0);
	}
}

//+add kwmoon 080527
LRESULT CDATsysView::OnUploadMesData(WPARAM wParam, LPARAM lParam)
{
	BOOL bPass = (BOOL) wParam;
	int nResult;
	CString sCurentTime;

	// change PSH 090507

	if(m_strWipId == _T("")){
		m_strWipId = gGmesCtrl.m_sSetID;
	}

	if(bPass)
	{
		nResult = UploadMasterTable(m_strWipId, TRUE, sTime);
		if(nResult == 2){
			nResult = UploadMasterTable(m_strWipId, TRUE, sTime);
		}
	}
	else
	{
		nResult = UploadMasterTable(m_strWipId, FALSE, sTime);
		if(nResult == 2){
			nResult = UploadMasterTable(m_strWipId, FALSE, sTime);
		}
	}


	if(nResult != 0)
	{
		COleDateTime  CurTime;
		CurTime	= COleDateTime ::GetCurrentTime();
		
		sCurentTime = CurTime.Format("%Y-%m-%d %H:%M:%S"); 

		AfxMessageBox("GMES ERROR : Test Result Upload");
		MesData_LocalSaveText(sCurentTime, m_strWipId, bPass, sTime);
	}

	if(MesData_AutoUpLoad_DataCheck())
	{
		ShowSystemMsg(MES_DATA_AUTOUPLOAD_MSG);
		MesData_AutoUpLoad();
		CloseSystemMsg();
	}

	SetTimer(2, 1000, NULL);
	return 0;

}

LRESULT CDATsysView::RunContinuousLVDSGrab(WPARAM wParam, LPARAM lParam)
{
	BOOL bStart = (BOOL) wParam;

	if(bStart)
	{
		if(!m_bContinuousLVDSGrabRunning)
		{
			m_bContinuousLVDSGrabRunning = TRUE;
			SetTimer(TIMER_CONTINUOUS_LVDS_GRAB,1000,NULL);
		}
	}
	else
	{
		if(m_bContinuousLVDSGrabRunning)
		{
			KillTimer(TIMER_CONTINUOUS_LVDS_GRAB); 
			m_bContinuousLVDSGrabRunning = FALSE;
		}
	}
	return 0;
}

LRESULT CDATsysView::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	static CString	buf1 = _T(""); //, reply1 = _T("");
	static CString	buf2 = _T(""); //, reply2 = _T("");
	static CString	buf3 = _T(""), reply3 = _T("");
//	int idx1;
	CString strReadData;
	CString strReadData2;
	CString strWId;
	CString sTmp;
	int		nSize = 0;
	BYTE	aByte; 
	int		i;
	int		nTemp;
	BYTE	nRevSize = 0;
	BYTE	strReceive[2048];


	if((int)wParam == ScannerCtrl.m_nPortID)
	{
		nSize=  (ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead).GetSize(); 

		memset(strReceive, 0, 2048);

		for(int i = 0 ; i < nSize; i++)
		{
	  		(ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴
			strReceive[i] = aByte;
		}	
		buf2.Format("%s",strReceive);

		buf1 += buf2;
	//	buf1 += (LPCTSTR)ScannerCtrl.m_ctrlScannerCtrl.abIn;
	//	buf1.Format("%s",ScannerCtrl.m_ctrlScannerCtrl.abIn);
		if(CurrentSet->bIsRunning)
		{
			buf1.Empty();
			ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
			return 1;
		}

		nSize = buf1.GetLength(); 
		
		strWId = _T("");
		
		if((nSize  >= 10) && (buf1.Find(0x0d) != -1)) 
//		if(nSize  >= 10) 
		{
			strWId += buf1;
			buf1.Empty();
		
			strWId.TrimLeft(0x20);
			strWId.TrimRight(0x20);
			strWId.TrimRight(0x0a);
			strWId.TrimRight(0x0d);
//			if((strWId.GetLength() == 18) && (strWId.Find("SYS") != -1))
			if((strWId.GetLength() == 18) && (strWId.Find("EAX") != -1))
			{
				ctrlFixtureID.SetWindowText(strWId);
				ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
			}
			else{
				if(strWId.GetLength() > 22)
				{
					if(strWId.Find(0x0d) != -1){
						sTmp = strWId.Left(strWId.Find(0x0d));
					}
					else if(strWId.Find("EBT") != -1){
						sTmp = strWId.Mid(strWId.Find("EBT"));
					}
					else if(strWId.Find("EBR") != -1){
						sTmp = strWId.Mid(strWId.Find("EBR"));
					}
					else if(strWId.Find("AAX") != -1){
						sTmp = strWId.Mid(strWId.Find("AAX"));
					}
					else{
						sTmp = strWId;
					}
					ctrlWipIdEdit.SetWindowText(sTmp);
				}
				else{
					ctrlWipIdEdit.SetWindowText(strWId);
				}
				ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
			}
			UpdateData(TRUE);
		} 
		
		//+add kwmoon 080522
/*		if(nSize  >= 10) 
		{
			if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
			{
				//+add psh 080603
			//	if(m_bResultDlgActivate)
			//	{
			//		delete m_ResultDlg;
			//	}
				PostMessage(WM_COMMAND, ID_RUN_RUN);
				CurrentSet->bIsRunMsg = TRUE;
			}
		} */ 
	}
	else if((int)wParam == TVCommCtrl.m_nPortID)
	{
		strReadData = _T("");

		nSize =  (TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetSize(); 
		if(nSize > 0)
		{
			if((g_pView->m_Rs232cAdcManualCheckDlg.GetSafeHwnd() != NULL)
			&& (g_pView->m_Rs232cAdcManualCheckDlg.m_bActive == TRUE))
			{
				(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);
				
				if (aByte == 0xbb) 
				{
					memset(TVCommCtrl.m_nReceiveData, 0, 225);
					TVCommCtrl.m_nReceiveLength = 1;
					TVCommCtrl.m_nReceiveData[0] = aByte;
					if (nSize >= 2) {
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&nRevSize);  //글자 하나씩 읽어옴
						TVCommCtrl.m_nRevSize = nRevSize;
						TVCommCtrl.m_nReceiveData[1] = nRevSize;
						TVCommCtrl.m_nReceiveLength = nSize;
					}
					for (i = 2; i < nSize; i++)
					{
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

						TVCommCtrl.m_nReceiveData[i] = aByte;
						//sTmp.Format(" %02X", aByte);
						//strReadData = strReadData + sTmp;
					}
					if ((nRevSize + 2) >= nSize) 
					{						
						strReadData = "DFT<:";
						//sTmp += strReadData;
						for (i = 0; i < nSize; i++)
						{
							//(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴
							sTmp.Format(" %02X", TVCommCtrl.m_nReceiveData[i]);
							strReadData = strReadData + sTmp;
						}

						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
						g_pView->m_Rs232cAdcManualCheckDlg.WriteLog(strReadData);
					}
				}
				else
				{
					strReadData = "DFT<:";
					sTmp.Format(" %02X", aByte);
					strReadData = strReadData + sTmp;

					for (i = 1; i < nSize; i++)
					{
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

						sTmp.Format(" %02X", aByte);
						strReadData = strReadData + sTmp;
					}
					//TVCommCtrl.m_sReceive.Format("%s", strReadData);
					//::SetEvent(TVCommCtrl.m_hReadEvent);
					(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
					g_pView->m_Rs232cAdcManualCheckDlg.WriteLog(strReadData);
				}
			}
			else if((!CurrentSet->bIsRunning) && CurrentSet->bCommDisplay)
			{
				strReadData = "DFT<:";
				for(i  = 0 ; i < nSize; i++)
				{
				   (TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

					sTmp.Format(" %02X", aByte);
					strReadData = strReadData + sTmp;
				}	
				AddStringToStatus(strReadData);
			}

			if(CurrentSet->bIsRunning)
			{
				(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴
				if(aByte == 0xbb){
					memset(TVCommCtrl.m_nReceiveData, 0, 225);
					TVCommCtrl.m_nReceiveLength = 1;
					TVCommCtrl.m_nReceiveData[0] = aByte;
					if(nSize >= 2){
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&nRevSize);  //글자 하나씩 읽어옴
						TVCommCtrl.m_nRevSize = nRevSize;
						TVCommCtrl.m_nReceiveData[1] = nRevSize;
						TVCommCtrl.m_nReceiveLength = nSize;
					}
					for(i  = 2 ; i < nSize; i++)
					{
					   (TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

						TVCommCtrl.m_nReceiveData[i] = aByte;
						sTmp.Format(" %02X", aByte);
						strReadData = strReadData + sTmp;
					}	
					if((nRevSize + 2) == nSize){
						strReadData = _T("");
						for(i  = 0 ; i < nSize; i++)
						{
							sTmp.Format(" %02X", TVCommCtrl.m_nReceiveData[i]);
							strReadData = strReadData + sTmp;
						}	

						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
						TVCommCtrl.m_sReceive.Format("%s",strReadData);
						::SetEvent(TVCommCtrl.m_hReadEvent);
						if(CurrentSet->bCommDisplay){
							AddStringToStatus("DFT<:" + strReadData);
						}
					}
				}
				else{
					if(TVCommCtrl.m_nReceiveLength == 1)
					{
						TVCommCtrl.m_nRevSize = aByte;
					}
					if((TVCommCtrl.m_nRevSize + 2) == TVCommCtrl.m_nReceiveLength){
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
					}
					else if((TVCommCtrl.m_nRevSize + 2) == (TVCommCtrl.m_nReceiveLength  + nSize))
					{
						TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						for(i  = 1 ; i < nSize; i++)
						{
						   (TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

							TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						}
						strReadData = _T("");
						for(i  = 0 ; i < TVCommCtrl.m_nRevSize + 2; i++)
						{
							sTmp.Format(" %02X", TVCommCtrl.m_nReceiveData[i]);
							strReadData = strReadData + sTmp;
						}	

						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
						TVCommCtrl.m_sReceive.Format("%s",strReadData);
						::SetEvent(TVCommCtrl.m_hReadEvent);
						if(CurrentSet->bCommDisplay){
							AddStringToStatus("DFT<:" + strReadData);
						}
					}
					else if((TVCommCtrl.m_nRevSize + 2) > (TVCommCtrl.m_nReceiveLength  + nSize))
					{	
						TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						for(i  = 1 ; i < nSize; i++)
						{
						   (TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

							TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						}
					}
					else if((TVCommCtrl.m_nRevSize + 2) <= (TVCommCtrl.m_nReceiveLength  + nSize))
					{	
						TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						nTemp = TVCommCtrl.m_nRevSize + 2 - TVCommCtrl.m_nReceiveLength;
						for(i  = 1 ; i < nTemp; i++)
						{
						   (TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

							TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						}

						strReadData = _T("");
						for(i  = 0 ; i < TVCommCtrl.m_nRevSize + 2; i++)
						{
							sTmp.Format(" %02X", TVCommCtrl.m_nReceiveData[i]);
							strReadData = strReadData + sTmp;
						}	

						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
						TVCommCtrl.m_sReceive.Format("%s",strReadData);
						::SetEvent(TVCommCtrl.m_hReadEvent);
						if(CurrentSet->bCommDisplay){
							AddStringToStatus("DFT<:" + strReadData);
						}
					}
				}
			}
			else if (TVCommCtrl.m_bCheckReadVersion == 1)
			{
				(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴
				if (aByte == 0xbb) {
					memset(TVCommCtrl.m_nReceiveData, 0, 225);
					TVCommCtrl.m_nReceiveLength = 1;
					TVCommCtrl.m_nReceiveData[0] = aByte;
					if (nSize >= 2) {
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&nRevSize);  //글자 하나씩 읽어옴
						TVCommCtrl.m_nRevSize = nRevSize;
						TVCommCtrl.m_nReceiveData[1] = nRevSize;
						TVCommCtrl.m_nReceiveLength = nSize;
					}
					for (i = 2; i < nSize; i++)
					{
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

						TVCommCtrl.m_nReceiveData[i] = aByte;
						sTmp.Format(" %02X", aByte);
						strReadData = strReadData + sTmp;
					}
					if ((nRevSize + 2) == nSize) {
						strReadData = _T("");
						for (i = 0; i < nSize; i++)
						{
							sTmp.Format(" %02X", TVCommCtrl.m_nReceiveData[i]);
							strReadData = strReadData + sTmp;
						}

						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
						TVCommCtrl.m_sReceive.Format("%s", strReadData);
						::SetEvent(TVCommCtrl.m_hReadEvent);
						if (CurrentSet->bCommDisplay) {
							AddStringToStatus("DFT<:" + strReadData);
						}
					}
				}
				else {
					if (TVCommCtrl.m_nReceiveLength == 1)
					{
						TVCommCtrl.m_nRevSize = aByte;
					}
					if ((TVCommCtrl.m_nRevSize + 2) == TVCommCtrl.m_nReceiveLength) {
						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
					}
					else if ((TVCommCtrl.m_nRevSize + 2) == (TVCommCtrl.m_nReceiveLength + nSize))
					{
						TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						for (i = 1; i < nSize; i++)
						{
							(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

							TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						}
						strReadData = _T("");
						for (i = 0; i < TVCommCtrl.m_nRevSize + 2; i++)
						{
							sTmp.Format(" %02X", TVCommCtrl.m_nReceiveData[i]);
							strReadData = strReadData + sTmp;
						}

						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
						TVCommCtrl.m_sReceive.Format("%s", strReadData);
						::SetEvent(TVCommCtrl.m_hReadEvent);
						if (CurrentSet->bCommDisplay) {
							AddStringToStatus("DFT<:" + strReadData);
						}
					}
					else if ((TVCommCtrl.m_nRevSize + 2) > (TVCommCtrl.m_nReceiveLength + nSize))
					{
						TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						for (i = 1; i < nSize; i++)
						{
							(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

							TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						}
					}
					else if ((TVCommCtrl.m_nRevSize + 2) <= (TVCommCtrl.m_nReceiveLength + nSize))
					{
						TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						nTemp = TVCommCtrl.m_nRevSize + 2 - TVCommCtrl.m_nReceiveLength;
						for (i = 1; i < nTemp; i++)
						{
							(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //글자 하나씩 읽어옴

							TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
						}

						strReadData = _T("");
						for (i = 0; i < TVCommCtrl.m_nRevSize + 2; i++)
						{
							sTmp.Format(" %02X", TVCommCtrl.m_nReceiveData[i]);
							strReadData = strReadData + sTmp;
						}

						(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).Clear();
						TVCommCtrl.m_sReceive.Format("%s", strReadData);
						::SetEvent(TVCommCtrl.m_hReadEvent);
						if (CurrentSet->bCommDisplay) {
							AddStringToStatus("DFT<:" + strReadData);
						}
					}
				}
			}

		}

	}
/*	else if((int)wParam == DMM.m_nPortID)
	{
		strReadData2 = _T("");
	//	buf3 += (LPCTSTR)DMM.m_CommCtrl.abIn;
		strReadData2.Format("%s",DMM.m_CommCtrl.abIn);

		buf3 += strReadData2;
	
		idx1 = buf3.Find(_T("\r\n"));
		if(idx1 >= 0)
		{
			DMM.m_strReceive = buf3.Left(idx1);
			reply3 = buf3.Left(idx1);
			buf3.Empty();
		}
	} */
	return 1;
}

void CDATsysView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//+ 2007.10.8 Add BY USY
	GetRectSizeInfo();
	//-
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDATsysView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//+ 2007.10.8 Add BY USY
	GetRectSizeInfo();
	//-
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDATsysView::SetGrabInfo(CBaseImage* pBaseImage)
{
	BOOL bChanged = FALSE;

	int nRowWidth  = 0;
	int nImageSize = 0;


	//+add kwmoon 080402
	CString szErrMsg = _T("");

		
	pBaseImage->InitHeaders(COLOR_24BIT, AnalogControl.m_nWidth, AnalogControl.m_nHeight,AnalogControl.m_nImageSize);

	if(pBaseImage->m_pImageData != NULL){ delete pBaseImage->m_pImageData; pBaseImage->m_pImageData = NULL; }

	pBaseImage->m_pImageData = new BYTE [pBaseImage->m_InfoHeader.biSizeImage];

}

//+ 2007.11.5 Add BY USY
LRESULT CDATsysView::OnProcessMessage(WPARAM wParam, LPARAM lParam)
{
/*	if(m_ResultDlg.m_bActivate == TRUE)
	{
		::SendMessage(m_ResultDlg, WM_JOYSTICK_MSG, 0, lParam);
	}
	else */ if(m_StopStepDlg.m_bActivate == TRUE)
	{
		::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, lParam);
	}
	else if(m_ManualTestDlg.m_bActivate == TRUE)
	{
		::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, lParam);
	}
	else
	{
		switch(lParam) 
		{
			case START_BTN1:
				{
					if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
					{
						PostMessage(WM_COMMAND, ID_RUN_RUN);
					//	CurrentSet->bIsRunMsg = TRUE;
							//     m_pOwner->SendMessage(WM_COMMAND, ID_RUN_RUN);
					}
				}
				break;
			case STOP_BTN1: 
				{
					if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
					{
						CurrentSet->bRunAbort = TRUE;
					}
				}
				break;
			case RETRY_BTN1:
			case OK_BTN1:
			case NG_BTN1:
				break;

				
			case START_BTN2:
				{
					if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
					{
						PostMessage(WM_COMMAND, ID_RUN_RUN);
					//	CurrentSet->bIsRunMsg = TRUE;
							//     m_pOwner->SendMessage(WM_COMMAND, ID_RUN_RUN);
					}
				}
				break;
			case STOP_BTN2: 
				{
					if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
					{
						CurrentSet->bRunAbort = TRUE;
						
					}
				}
				break;
			case RETRY_BTN2:
			case OK_BTN2:
			case NG_BTN2:
				break;

		}
	}
	return 1;
}

//+add kwmoon 071204
BOOL CDATsysView::RenameMakeRefFolderName() 
{
	CTime Time = CTime::GetCurrentTime();

	CString szTemp = CurrentSet->sSeqPath.Left(CurrentSet->sSeqPath.ReverseFind('\\'));

	CString szRefFolderPath;
	szRefFolderPath.Format("%s\\Ref",szTemp);

	CString	szTime;
	szTime = Time.Format("%m%d_%H%M%S");

	CString szTempRefFolderPath;
	szTempRefFolderPath.Format("%s\\Ref_%s",szTemp,szTime);

	// Free image memories
	if(g_GrabImage.m_bImageLoaded)
	{
		g_GrabImage.m_bImageLoaded = FALSE; _Wait(100);
	}
	
	if(g_RefImage.m_bImageLoaded)
	{
		g_RefImage.m_bImageLoaded = FALSE; _Wait(100);
		g_RefImage.UnloadImage();
	}

	if(g_MaskImage.m_bImageLoaded)
	{
		g_MaskImage.m_bImageLoaded = FALSE; _Wait(100);
		g_MaskImage.UnloadImage();
	}

	for(int i=0; i<5; i++)
	{
		if(m_Image[i].m_bImageLoaded)
		{
			m_Image[i].m_bImageLoaded = FALSE; _Wait(100);
			m_Image[i].UnloadImage();
		}
	}

	_Wait(100);

	int result = 0;

	result = rename((char*)(LPCTSTR)szRefFolderPath,(char*)(LPCTSTR)szTempRefFolderPath);

	if(result != 0) return FALSE;

	result = rename((char*)(LPCTSTR)m_szMakeRefPath,(char*)(LPCTSTR)szRefFolderPath);
		
	if(result != 0) return FALSE;
	result = rename((char*)(LPCTSTR)szTempRefFolderPath,(char*)(LPCTSTR)m_szMakeRefPath);

	if(result != 0) return FALSE;

	return TRUE;
}

//+add kwmoon 071204
BOOL CDATsysView::CopyReferenceImage() 
{
	CString szTemp = CurrentSet->sSeqPath.Left(CurrentSet->sSeqPath.ReverseFind('\\'));

	CString szRefFolderPath;
	szRefFolderPath.Format("%s\\Ref",szTemp);

	CString szReferenceFilePath;
	szReferenceFilePath.Format("%s\\*.*",m_szMakeRefPath);

	CString szMaskFilePath;
	szMaskFilePath.Format("%s\\Mask\\*.*",m_szMakeRefPath);

	// Free image memories
	if(g_GrabImage.m_bImageLoaded)
	{
		g_GrabImage.m_bImageLoaded = FALSE; _Wait(100);
	}
	
	if(g_RefImage.m_bImageLoaded)
	{
		g_RefImage.m_bImageLoaded = FALSE; _Wait(100);
		g_RefImage.UnloadImage();
	}

	if(g_MaskImage.m_bImageLoaded)
	{
		g_MaskImage.m_bImageLoaded = FALSE; _Wait(100);
		g_MaskImage.UnloadImage();
	}

	for(int i=0; i<5; i++)
	{
		if(m_Image[i].m_bImageLoaded)
		{
			m_Image[i].m_bImageLoaded = FALSE; _Wait(100);
			m_Image[i].UnloadImage();
		}
	}

	_Wait(100);

    CFileFind find;
    BOOL bFind = FALSE;
	
	CString szFileName = _T("");
	CString szFilePath = _T("");

	if(bFind = find.FindFile(szReferenceFilePath))
    {       
        while(bFind)
        {
            bFind = find.FindNextFile();

            if(find.IsDots()) continue;

            if(!find.IsDirectory())
			{
				szFileName = find.GetFileName();

				CopyFile(m_szMakeRefPath+"\\"+szFileName, szRefFolderPath+"\\"+szFileName, FALSE);
			}
       }
    }

	if(bFind = find.FindFile(szMaskFilePath))
    {       
        while(bFind)
        {
            bFind = find.FindNextFile();

            if(find.IsDots()) continue;

            if(!find.IsDirectory())
			{
				szFileName = find.GetFileName();

				CopyFile(m_szMakeRefPath+"\\Mask\\"+szFileName, szRefFolderPath+"\\Mask\\"+szFileName, FALSE);
			}
       }
    }

    find.Close();

	return TRUE;
}

/*
BOOL CDATsysView::UserControlInit(BOOL bInit)
{
	if(bInit == TRUE)
	{
		g_hInst = AfxGetInstanceHandle();
		CWnd* pHwnd  = GetForegroundWindow();

		if(	InitDirectInput_Joystic(this->m_hWnd) == TRUE)
		{	
			bJoysticInit = TRUE;
			SetAcquire_Joystic();

		}
		else
		{
//			AfxMessageBox("JoyStick Init Failed!");
			bJoysticInit = FALSE;
//			return FALSE;
		}
//		SetAcquire_Joystic();
		SetTimer(1, 1000, NULL);
	}
	else if(bInit == FALSE)
	{
		if(bJoysticInit)
		{
			DWORD dwJoyStickThreadResult = 0;
			DWORD dwExitCode = 0;

			//+change 090213(Modification No1)
		//	m_pBaseThread->m_bDone = TRUE;
		//	dwJoyStickThreadResult = WaitForSingleObject(m_pBaseThread->m_hJoyStickThreadKillEvent, 3000);
			m_pJoyStickThread->m_bDone = TRUE;
			dwJoyStickThreadResult = WaitForSingleObject(m_pJoyStickThread->m_hJoyStickThreadKillEvent, 3000);
			
			if(dwJoyStickThreadResult == WAIT_OBJECT_0 )
			{
				//+change 090213(Modification No1)
			//	m_pBaseThread = NULL;
				m_pJoyStickThread = NULL;
			}
			else 
			{
				//+change 090213(Modification No1)
			//	dwJoyStickThreadResult = WaitForSingleObject(m_pBaseThread->m_hJoyStickThreadKillEvent,1000);
				dwJoyStickThreadResult = WaitForSingleObject(m_pJoyStickThread->m_hJoyStickThreadKillEvent,1000);
						
				if(dwJoyStickThreadResult == WAIT_OBJECT_0) 
				{
					//+change 090213(Modification No1)
				//	m_pBaseThread = NULL;
					m_pJoyStickThread = NULL;
				}
				else if(dwJoyStickThreadResult != WAIT_OBJECT_0)
				{
					//+change 090213(Modification No1)
			//		TerminateThread(m_pBaseThread,dwExitCode);
					if(m_pJoyStickThread) TerminateThread(m_pJoyStickThread->m_hThread,dwExitCode);
				}
			}
			FreeDirectInput_Joystic();
		}
	}
	return TRUE;
}
*/

int CDATsysView::UploadMasterTable(CString strWipid, BOOL bResult, CString sTime)
{
	DWORD dwEventResult = 0;
    clock_t		start;
	MSG msg;
	CString sMsg;
	CString sElemString;

	//+change PSH 090507
	if(!CurrentSet->bUploadMes) return 0;

	::ResetEvent(m_hReadEvent_S6F2);
	sElemString = gGmesCtrl.MakeElem_S6F1(CurrentSet->sEquipmentID, m_strWipId, bResult, sTime, TRUE);
	if(!gGmesCtrl.SendCommString(sElemString))
	{
		return 2;
	}

	gGmesCtrl.DisplyComStatus("S6F1", FALSE);

	start = clock();
	while (TRUE)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   
		dwEventResult = WaitForSingleObject(m_hReadEvent_S6F2, 500);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(m_hReadEvent_S6F2);
			if(!m_bResult_S6F2){
				sMsg.Format(_T("GMES Result Upload Error\nError Code : %s"), gGmesCtrl.m_sErrorMsg);
				AfxMessageBox(sMsg);
				return 1;
			}
			break;
		}
		if ((clock() - start)*1000/CLOCKS_PER_SEC > 5000){ 
			sMsg.Format(_T("GMES S6F2(Result Upload) Time Out."));
			AfxMessageBox(sMsg);
			Gmes_Connection();
			return 2;
		}

	}

	return 0;

}


void CDATsysView::OnPwrOn() 
{
	CString sCmd = _T("");
	CString sName = _T("");
	CString sCustom = _T("");
	int nByte = 1;
	CString sMsg;

	sName = "POWER";

	if(!FindRemoconCode(sName, sCustom, sCmd, nByte))
	{
		sMsg.Format("Remocon Code Not Found: %s", sName);
		AddStringToRemoteComLog(sMsg);
	}

	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte);	
}

void CDATsysView::OnPwrOff() 
{
	// TODO: Add your command handler code here
//	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "C5");	
}
//+del kwmoon 080819
/*
void CDATsysView::OnInstop() 
{
	// TODO: Add your command handler code here
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "FA");	
}
*/
void CDATsysView::OnExit() 
{
	CString sCmd = _T("");
	CString sName = _T("");
	CString sCustom = _T("");
	int nByte = 1;
	CString sMsg;

	sName = "INPUT";
//	sName = "RETURN";

	if(!FindRemoconCode(sName, sCustom, sCmd, nByte))
	{
		sMsg.Format("Remocon Code Not Found: %s", sName);
		AddStringToRemoteComLog(sMsg);
	}

	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte);	
}
/*
void CDATsysView::OnTv() 
{
	// TODO: Add your command handler code here
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "D6");	
}
*/
//+add kwmoon 080417
void CDATsysView::OnEnter() 
{
	CString sCmd = _T("");
	CString sName = _T("");
	CString sCustom = _T("");
	int nByte = 1;
	CString sMsg;

	sName = "HOME";

	if(!FindRemoconCode(sName, sCustom, sCmd, nByte))
	{
		sMsg.Format("Remocon Code Not Found: %s", sName);
		AddStringToRemoteComLog(sMsg);
	}

	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte);	
}


//+add kwmoon 080819
void CDATsysView::OnRefImage() 
{
	// TODO: Add your command handler code here
	if ((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	OnRunMakeRef();
}

//+add kwmoon 080819
void CDATsysView::OnRefAudio() 
{
	// TODO: Add your command handler code here
	OnRunAdjSpec();
}

//+add kwmoon 080411
void CDATsysView::PlayWavSound(UINT nIDS) 
{
	HRSRC hRes;
	HGLOBAL hData;
	BOOL bOk = FALSE;

	LPCTSTR lpszSound = MAKEINTRESOURCE(nIDS);

	if((hRes  = ::FindResource(AfxGetResourceHandle(),lpszSound,_T("WAVE"))) != NULL 
		&& (hData = ::LoadResource(AfxGetResourceHandle(),hRes)) != NULL) 
	{
		bOk = sndPlaySound((LPCTSTR)::LockResource(hData),SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
		FreeResource(hData);
	}
}


//+ 2008.06.06 Add BY PSH
BOOL CDATsysView::InitScanner(CString sComPort, DWORD wBaudRate)
{
	if(ScannerCtrl.m_bPortOpen == FALSE)
	{
	//	if((g_hCommWnd = GetSafeHwnd()) == NULL)
	//	{
	//		AfxMessageBox("View Handle Null!");
	//	}

		if(ScannerCtrl.Create(sComPort, wBaudRate) == FALSE) 
		{
			//+add kwmoon 071015 : [2in1] : _Wait(100)
			ScannerCtrl.PortClose();
			if((g_hCommWnd = GetSafeHwnd()) == NULL)
			{
				AfxMessageBox("View Handle Null!");
			}

			if(!ScannerCtrl.Create(sComPort, wBaudRate) == FALSE)
			{
				//+add kwmoon 080313
				CString szErrMsg;
				szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sScannerComPort);
				AfxMessageBox(szErrMsg);
				return FALSE;
			}
		}
	}
	else if(ScannerCtrl.m_bPortOpen == TRUE)
	{
		//+add kwmoon 071015 : [2in1] : _Wait(100)
		ScannerCtrl.PortClose();
	//	if((g_hCommWnd = GetSafeHwnd()) == NULL)
	//	{
	//		AfxMessageBox("View Handle Null!");
	//	}
		if(!ScannerCtrl.Create(sComPort, wBaudRate) == FALSE)
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


//+change kwmoon 080804
void CDATsysView::SetStopStepDlgMsg(int nStepResult) 
{
	CString szMsg1 = _T("");
	CString szMsg2 = _T("");
	CString szMsg3 = _T("");

	szMsg1.Format("Step(%d) %s",pCurStep->m_nStep,pCurStep->m_sName);

	if(nStepResult == TEST_FAIL)
	{
		//090325
		if(pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
		{	
			szMsg2 = _T("NG");

			if((pCurStep->m_bAudioTestResult == TRUE) && (pCurStep->m_bVideoTestResult == FALSE))
			{
				szMsg2 = _T("Video NG");

				//+change kwmoon 090326
			//	szMsg3.Format("Good:%.1f | Error:%.2f/%.2f/%.3f/%.3f/%.3f",
			//	pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
			//	pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
			//	pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);
				for(int i=0; i<5;i++)
				{
					if(pCurStep->m_fBandErrorPixelPercentage[i] > CurrentSet->fAllowedBandErrorPixelPercentage[i])
					{
						szMsg3.Format("Error:%.1f (Band%d-Limit : %.1f)",pCurStep->m_fBandErrorPixelPercentage[i],i,CurrentSet->fAllowedBandErrorPixelPercentage[i]); break;
					}
				}
			}

			if((pCurStep->m_bVideoTestResult == TRUE) && (pCurStep->m_bAudioTestResult == FALSE))
			{
				szMsg2 = _T("Audio NG");

				//+add kwmoon 080716
				if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
				{
					if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftLevel,(int)AvSwitchBoxCtrl.m_fTargetRightLevel,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftLevel,(int)AvSwitchBoxCtrl.m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftFreq,(int)AvSwitchBoxCtrl.m_fTargetRightFreq,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftFreq,(int)AvSwitchBoxCtrl.m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftFreq,(int)AvSwitchBoxCtrl.m_fTargetRightFreq,
						(int)AvSwitchBoxCtrl.m_fTargetLeftLevel,(int)AvSwitchBoxCtrl.m_fTargetRightLevel,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftFreq,(int)AvSwitchBoxCtrl.m_fMeasuredRightFreq,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftLevel,(int)AvSwitchBoxCtrl.m_fMeasuredRightLevel);
					}
				}
				else
				{
					if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)m_fTargetLeftLevel,(int)m_fTargetRightLevel,
						(int)m_fMeasuredLeftLevel,(int)m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)m_fTargetLeftFreq,(int)m_fTargetRightFreq,
						(int)m_fMeasuredLeftFreq,(int)m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)m_fTargetLeftFreq,(int)m_fTargetRightFreq,
						(int)m_fTargetLeftLevel,(int)m_fTargetRightLevel,
						(int)m_fMeasuredLeftFreq,(int)m_fMeasuredRightFreq,
						(int)m_fMeasuredLeftLevel,(int)m_fMeasuredRightLevel);
					}
				}
			}

			if((pCurStep->m_bVideoTestResult == FALSE) && (pCurStep->m_bAudioTestResult == FALSE))
			{
			//	szMsg2 = _T("Audio/Video NG");

				//+change kwmoon 090326
			//	szMsg3.Format("Good:%.1f | Error:%.2f/%.2f/%.3f/%.3f/%.3f",
			//	pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
			//	pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
			//	pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);

				for(int i=0; i<5;i++)
				{
					if(pCurStep->m_fBandErrorPixelPercentage[i] > CurrentSet->fAllowedBandErrorPixelPercentage[i])
					{
						szMsg2.Format("[Video NG] Error:%.1f (Band%d-Limit : %.1f)",pCurStep->m_fBandErrorPixelPercentage[i],i ,CurrentSet->fAllowedBandErrorPixelPercentage[i]); break;
					}
				}

				if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
				{
					if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){ 
						szMsg3.Format("[Audio NG] T(%d,%d) M(%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftLevel,(int)AvSwitchBoxCtrl.m_fTargetRightLevel,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftLevel,(int)AvSwitchBoxCtrl.m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("[Audio NG] T(%d,%d) M(%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftFreq,(int)AvSwitchBoxCtrl.m_fTargetRightFreq,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftFreq,(int)AvSwitchBoxCtrl.m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("[Audio NG] T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftFreq,(int)AvSwitchBoxCtrl.m_fTargetRightFreq,
						(int)AvSwitchBoxCtrl.m_fTargetLeftLevel,(int)AvSwitchBoxCtrl.m_fTargetRightLevel,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftFreq,(int)AvSwitchBoxCtrl.m_fMeasuredRightFreq,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftLevel,(int)AvSwitchBoxCtrl.m_fMeasuredRightLevel);
					}
				}
				else
				{
					if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){ 
						szMsg3.Format("[Audio NG] T(%d,%d) M(%d,%d)",
						(int)m_fTargetLeftLevel,(int)m_fTargetRightLevel,
						(int)m_fMeasuredLeftLevel,(int)m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("[Audio NG] T(%d,%d) M(%d,%d)",
						(int)m_fTargetLeftFreq,(int)m_fTargetRightFreq,
						(int)m_fMeasuredLeftFreq,(int)m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("[Audio NG] T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)m_fTargetLeftFreq,(int)m_fTargetRightFreq,
						(int)m_fTargetLeftLevel,(int)m_fTargetRightLevel,
						(int)m_fMeasuredLeftFreq,(int)m_fMeasuredRightFreq,
						(int)m_fMeasuredLeftLevel,(int)m_fMeasuredRightLevel);
					}
				}
			}
		}
		else if(!pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
		{
			if(pCurStep->m_bVideoTestResult == FALSE)
			{
				szMsg2 = _T("Video NG");

				//+change kwmoon 090326
			//	szMsg3.Format("Good:%.1f | Error:%.2f/%.2f/%.3f/%.3f/%.3f",
			//	pCurStep->m_fGoodPixelPercentage,pCurStep->m_fBandErrorPixelPercentage[0],
			//	pCurStep->m_fBandErrorPixelPercentage[1],pCurStep->m_fBandErrorPixelPercentage[2],
			//	pCurStep->m_fBandErrorPixelPercentage[3],pCurStep->m_fBandErrorPixelPercentage[4]);

				for(int i=0; i<5;i++)
				{
					if(pCurStep->m_fBandErrorPixelPercentage[i] > CurrentSet->fAllowedBandErrorPixelPercentage[i])
					{
						szMsg3.Format("Error:%.1f (Band%d-Limit : %.1f)",pCurStep->m_fBandErrorPixelPercentage[i],i ,CurrentSet->fAllowedBandErrorPixelPercentage[i]); break;
					}
				}
			}
		}
		else if(pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
		{
			if(pCurStep->m_bAudioTestResult == FALSE)
			{
				szMsg2 = _T("Audio NG");

				//+add kwmoon 080716
				if(CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
				{
					if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftLevel,(int)AvSwitchBoxCtrl.m_fTargetRightLevel,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftLevel,(int)AvSwitchBoxCtrl.m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftFreq,(int)AvSwitchBoxCtrl.m_fTargetRightFreq,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftFreq,(int)AvSwitchBoxCtrl.m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)AvSwitchBoxCtrl.m_fTargetLeftFreq,(int)AvSwitchBoxCtrl.m_fTargetRightFreq,
						(int)AvSwitchBoxCtrl.m_fTargetLeftLevel,(int)AvSwitchBoxCtrl.m_fTargetRightLevel,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftFreq,(int)AvSwitchBoxCtrl.m_fMeasuredRightFreq,
						(int)AvSwitchBoxCtrl.m_fMeasuredLeftLevel,(int)AvSwitchBoxCtrl.m_fMeasuredRightLevel);
					}
				}
				else
				{
					if(pCurStep->m_nMeasureAudioType == LEVEL_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)m_fTargetLeftLevel,(int)m_fTargetRightLevel,
						(int)m_fMeasuredLeftLevel,(int)m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)m_fTargetLeftFreq,(int)m_fTargetRightFreq,
						(int)m_fMeasuredLeftFreq,(int)m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)m_fTargetLeftFreq,(int)m_fTargetRightFreq,
						(int)m_fTargetLeftLevel,(int)m_fTargetRightLevel,
						(int)m_fMeasuredLeftFreq,(int)m_fMeasuredRightFreq,
						(int)m_fMeasuredLeftLevel,(int)m_fMeasuredRightLevel);
					}
				}
			}
		}
		else
		{
			if(pCurStep->m_nTestType == ADC_TEST)
			{
				szMsg2 = _T("ADC NG");
			}
			else
			{
				szMsg2 = _T("NG");
			}

			szMsg3.Format(_T("%s"), pCurStep->m_strMsg);
		}
	}
	else
	{
		szMsg2 = _T("NG");
		szMsg3.Format(_T("%s"), pCurStep->m_strMsg);
	}

	m_StopStepDlg.SetTestResultInfo(szMsg1,szMsg2,szMsg3);
}

//+add kwmoon 080624
BOOL CDATsysView::RunInternalRetry() 
{
	int nStepResult = TEST_FAIL;

	CString szMsg = _T("");

	pCurStep->m_bStepInternalRetryFlag = TRUE;

	//+add kwmoon 080724
	InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

	for(UINT nTmp = 0; nTmp < CurrentSet->nNoRetry; nTmp++)
	{
		//+add kwmoon 080711
		m_bResultDlgActivate = FALSE;

//		AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5B");	_Wait(500);

		g_nNoFailedStep -= 1;
		DeleteResultDataFromGrid(CurrentSet->nDisplayType, pCurStep->GetStepNo());
		InsertMsg2DetailedGrid(pCurStep->m_nStep,_T(""));
		pCurStep->m_strMsg = _T("");

		// Run Current Step Again
		szMsg.Format("Retry#%d Current Step",nTmp+1);
		ShowSubStepMessage(_T(""),szMsg);

		if((nStepResult = StepRun()) == TEST_ABORT)
		{
			m_nNoCurrentRepeatExecution = 1; 
			pCurStep->m_bStepInternalRetryFlag = FALSE;
			
			// goto END_WHILE_LOOP;
			return FALSE;
		}
		else // if TEST_PASS or TEST_FAIL
		{
			if(pCurStep->GetResult() == TRUE) break;
		}
	}
	pCurStep->m_bStepInternalRetryFlag = FALSE;

	return TRUE;
}

//+change kwmoon 080804
BOOL CDATsysView::RunUserRetry(int nStepResult)
{
	PlayWavSound(IDR_WAVE_RETRY);

	ctrlWipIdEdit.SetFocus();

	InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

	m_bResultDlgActivate = FALSE;


	// Display User Dialog
	SetStopStepDlgMsg(nStepResult);
	m_StopStepDlg.DoModal();

	switch(m_StopStepDlg.m_nRunType)
	{
		case STEP_STOP:
		{
			CurrentSet->bRunAbort = TRUE;
			
			g_nRetryStop++;
		}
		break;

		case STEP_RETRY:
		{
			g_nUserRetry++;

			g_nNoFailedStep -= 1;

			DeleteResultDataFromGrid(CurrentSet->nDisplayType, pCurStep->GetStepNo());

			InsertMsg2DetailedGrid(pCurStep->m_nStep, _T(""));
			pCurStep->m_strMsg = _T("");
			pCurStep->m_bStepRetryFlag = TRUE;
			
			ShowSubStepMessage(_T(""),"Retry Current Step");
			
			if((nStepResult = StepRun()) == TEST_ABORT)
			{
				m_nNoCurrentRepeatExecution = 1; 
				pCurStep->m_bStepRetryFlag = FALSE;
				
				return FALSE;
			}
			pCurStep->m_bStepRetryFlag = FALSE;
		}
		break;

		case STEP_OK:
		{
			g_nNoFailedStep -= 1;
		
			if(pCurStep->m_bRunAudioTest)
			{
				pCurStep->m_bAudioTestResult = TRUE;
			}
			if(pCurStep->m_bRunVideoTest)
			{
				pCurStep->m_bVideoTestResult = TRUE;
			}
			pCurStep->m_bResult = TRUE;
		}
		break;

		case STEP_NG: 
		{
			break;
		}
	}
	return TRUE;
}

//+add kwmoon 080624
void CDATsysView::SaveAdjSpecData() 
{
	CString szSeqPath	 = _T("");
	CString szSeqFile	 = _T("");
	CString szRefSeqFile = _T("");
	CString szTemp1		 = _T("");
	CString szTemp2		 = _T("");

	//Change 090304
/*
	szSeqPath = CurrentSet->sSeqPath.Left(CurrentSet->sSeqPath.ReverseFind('\\'));
	szSeqFile = CurrentSet->sSeqPath.Right(CurrentSet->sSeqPath.GetLength()-CurrentSet->sSeqPath.ReverseFind('\\')-1);
	szSeqFile = szSeqFile.Left(szSeqFile.ReverseFind('.'));

	szTemp1 = szSeqFile.Right(6);
	szTemp1.MakeUpper();

	if(szTemp1.Compare("_AUDIO") == NULL)
	{
		szRefSeqFile.Format("%s\\%s.seq",szSeqPath,szSeqFile);
	}
	else
	{
		szRefSeqFile.Format("%s\\%s_Audio.seq",szSeqPath,szSeqFile);
	}
	if(SaveSeqFile(szRefSeqFile))
*/
	if(SaveSeqFile(CurrentSet->sSeqPath))
	{
		m_szResultMsg1.Format("Color test spec is adjusted!");
		m_szResultMsg2.Format("(%s)",szRefSeqFile);
	}
	else
	{
		m_szResultMsg1.Format("Failed to create new seq file");
	}
}

//+add kwmoon 080624
void CDATsysView::SaveMakeRefData() 
{
	CString szSeqPath	 = _T("");
	CString szSeqFile	 = _T("");
	CString szRefSeqFile = _T("");

	if(g_nNoFailedStep == 0)
	{
		szSeqPath = CurrentSet->sSeqPath.Left(CurrentSet->sSeqPath.ReverseFind('\\'));
		szSeqFile = CurrentSet->sSeqPath.Right(CurrentSet->sSeqPath.GetLength()-CurrentSet->sSeqPath.ReverseFind('\\')-1);
		szSeqFile = szSeqFile.Left(szSeqFile.ReverseFind('.'));

		//+add kwmoon 080229
		int nNoCheckedStep = GetNoCheckedStep(CurrentSet->nDisplayType);

		if(nNoCheckedStep == StepList.GetCount())
		{
			//+add kwmoon 071203
			if(!m_bUseDefaultREFfolder)
			{
				m_szResultMsg1.Format("Folder is created");
				m_szResultMsg2.Format("[%s]",m_szMakeRefPath);
			}
			else
			{
				m_szResultMsg1.Format("New images are saved in REF folder");
			}
		}
		_Wait(100);
	}
	else
	{
		m_szResultMsg1.Format("Failed to create new REF images");
	}
}


//+add kwmoon 080724
void CDATsysView::GetApplicationVersion() 
{
	CString sSwVer, sHwVer;

	LPVOID lpBuffer;
	UINT uiLen;          
	char caAppName[20];
	strcpy(caAppName, AfxGetApp()->m_pszExeName);
	strcat(caAppName, ".exe");

	DWORD dwInfoSize = GetFileVersionInfoSize(caAppName, NULL);
	LPVOID lpVersionInfo = new BYTE[dwInfoSize];

	GetFileVersionInfo(caAppName, 0, dwInfoSize, lpVersionInfo);
	VerQueryValue(lpVersionInfo, TEXT("\\StringFileInfo\\040904b0\\ProductVersion"), &lpBuffer, &uiLen);

	char *pcVer = new char [uiLen+1];
	strncpy(pcVer, (char *)lpBuffer, uiLen);	// Product Version
	m_szVersion.Format("%s",pcVer);

	m_szCurrentStatus	= m_szVersion;

	CMainFrame* pMainFrame =(CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetTitle(m_szCurrentStatus);

	delete [] pcVer;

	//+add 090226(Modification No2)
	delete [] lpVersionInfo;
}

void CDATsysView::Display_GMESInfo(BOOL bFlag) 
{
	CString sStatusMsg;

	if(bFlag){
		sStatusMsg.Format("EQPID:%s, PROCID:%s, LINEID:%s", CurrentSet->sEquipmentID, CurrentSet->sOperationID, CurrentSet->sLineID); 
	}
	else{
		sStatusMsg.Format("Tool Option D/L Fail"); 
	}
	CMainFrame* pMainFrame =(CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetStatusBarText(sStatusMsg);
}

void CDATsysView::InitStepVariable() 
{
	POINT ptZeroPosition;
	ptZeroPosition.x = 0;
	ptZeroPosition.y = 0;

	//+add kwmoon 080711
	m_bResultDlgActivate = FALSE;

//	g_GrabSource = LVDS_GRABBER;
	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	//+add kwmoon 080828
	g_ImageProc.m_bInvalidateInvalidRegion = FALSE;
}

/*
UINT CDATsysView::GrabImageThread(LPVOID pParam)
{
	BOOL bGrabbing = FALSE;

	CDATsysView* pView = (CDATsysView*)pParam;

	int nWidth		= 0;
	int nHeight		= 0;
	int nImageSize	= 0;
	int nErrorCode	= 0;
	int nBitCount	= 0;
	int nRowWidth   = 0;
	int nImageBufferSize = 0;
	int nGrabResult = GRAB_FAIL;

	BOOL bResult	= FALSE;

	CString szFileName = _T("");
	CString szErrorMsg = _T("");

	BYTE* pImageBuffer = NULL;

	::ResetEvent(pView->m_hGrabThreadKillEvent);

//	pView->m_bGrabThreadRunning = TRUE;

	nWidth		= CurrentSet->nLvdsWidth;
	nHeight		= CurrentSet->nLvdsHeight;
	nBitCount	= CurrentSet->nBitCount;
	nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
	nImageSize  = nRowWidth * nHeight;


	while(!pView->m_bStopLVDSThread)
	{
		g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);
		
		if((nGrabResult = pView->Grab_2in1(GRAB_WITH_NO_RETRY)) != GRAB_PASS)
		{
			g_GrabImage.m_bImageLoaded = TRUE;
		
			pView->PostMessage(UM_UPDATE_GRAB_STATUS,0,0); 			

		}
		else
		{
 			g_GrabImage.m_bImageLoaded = TRUE;

			pView->PostMessage(UM_UPDATE_GRAB_STATUS,0,0); 			

		}  
		Sleep(10);
	}

	pView->m_bStopLVDSThread = FALSE;

	pView->m_bGrabThreadRunning = FALSE;

	::SetEvent(pView->m_hGrabThreadKillEvent);
	
	return TRUE;
}
*/
LRESULT CDATsysView::UpdateGrabImage(WPARAM wParam, LPARAM lParam)
{
	CRect nImageRect;
	CWnd* pImageWnd;

	UpdateData(FALSE);

	
	pImageWnd = GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	InvalidateRect(nImageRect, FALSE);
	//+add kwmoon 080618
	DrawImage(IDC_VIDEOVIEW,g_GrabImage);

	pImageWnd->UpdateWindow();
//	UpdateWindow();
	return 0;
}

void CDATsysView::StartGrabThread()
{
	//=========================
	// Start Auto-Grab Thread
	//=========================
	if(!m_bGrabThreadRunning)
	{ 
		OnGrabStart();
	//	m_pGrabImageThread = AfxBeginThread(GrabImageThread,this);
	}


}

void CDATsysView::StopGrabThread()
{
	//=======================
	// Stop Auto-Grab Thread
	//=======================
	if(m_bGrabThreadRunning)
	{
		OnGrabStop();
	}
}

void CDATsysView::OnGrabStart() 
{
	BOOL bFlag;

	if(AnalogControl.m_bGrabberInit == FALSE) return;

	bFlag = m_SignalEnable;//g_pView->wIn_Flag;

	OnAudioMeasStop();
	Sleep(100);

	if(!m_bGrabThreadRunning){
		m_pVideoViewWnd		= (CWnd*) GetDlgItem(IDC_VIDEOVIEW);
	   // Start a continuous grab in this view
		MdispSelectWindow(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage, m_pVideoViewWnd->GetSafeHwnd());
	//	MdigControl(AnalogControl.m_MilDigitizer, M_GRAB_SCALE, 1.0);
		MdigGrabContinuous(AnalogControl.m_MilDigitizer, AnalogControl.m_MilImage);
//		MappGetError(AnalogControl.m_MilDigitizer,&AnalogControl.m_nErrorCode);

		m_bGrabThreadRunning = TRUE;
	}
	else{
		MdigHalt(AnalogControl.m_MilDigitizer);	
//		MappGetError(AnalogControl.m_MilDigitizer,&AnalogControl.m_nErrorCode);
		MbufLoad((LPSTR)(LPCTSTR)m_szNoImageBmpPath,AnalogControl.m_MilImage);
 		MdispDeselect(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage);

		m_Image[I_GRAB].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);

		m_bGrabThreadRunning = FALSE;
		m_pVideoViewWnd = NULL;
	}
//	if(bFlag) SetTimer(2, 1000, NULL);

#if 0
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed on a grab start
   /////////////////////////////////////////////////////////////////////////
   // If there is a grab in a view, halt the grab before starting a new one
	if (((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted)
		((CMdispMFCApp*)AfxGetApp())->m_pGrabView->SendMessage(WM_COMMAND, ID_GRAB_STOP, 0);

	// Start a continuous grab in this view
	MdigGrabContinuous(((CMdispMFCApp*)AfxGetApp())->m_MilDigitizer,
		((CMdispMFCDoc*)GetDocument())->m_MilImage);

	// Update the variable GrabIsStarted
	((CMdispMFCApp*)AfxGetApp())->m_isGrabStarted = TRUE;

	// GrabInViewPtr is now a pointer to m_pGrabView view
	((CMdispMFCApp*)AfxGetApp())->m_pGrabView = this;

	// Document has been modified
	((CMdispMFCDoc*)GetDocument())->SetModifiedFlag(TRUE);

	/////////////////////////////////////////////////////////////////////////   
	// MIL: Write code that will be executed on a grab start
	/////////////////////////////////////////////////////////////////////////
#endif
}

void CDATsysView::OnGrabStop() 
{
	BOOL bFlag;

	if(!m_bGrabThreadRunning)return;

	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	OnAudioMeasStop();
	Sleep(100);


	MdigHalt(AnalogControl.m_MilDigitizer);	
    MbufLoad((LPSTR)(LPCTSTR)m_szNoImageBmpPath,AnalogControl.m_MilImage);
  //  MappGetError(AnalogControl.m_MilDigitizer,&AnalogControl.m_nErrorCode);

	MdispDeselect(AnalogControl.m_MilDisplay, AnalogControl.m_MilImage);

	m_Image[I_GRAB].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);

	m_bGrabThreadRunning = FALSE;
	m_pVideoViewWnd = NULL;
//	if(bFlag) SetTimer(2, 1000, NULL);
}

void CDATsysView::ShowSystemMsg(int nMgaNo) 
{
	// Modeless Dialog
	if(m_pSystemMessageDlg == NULL)
	{
		m_pSystemMessageDlg = new CStopProcessingDlg; //(this);
		m_pSystemMessageDlg->Create(IDD_STOP_PROCESSING_DLG);
//		m_pSystemMessageDlg->Create(this);
		m_pSystemMessageDlg->SetMessage(nMgaNo);
		m_pSystemMessageDlg->ShowWindow(SW_SHOW);
		m_pSystemMessageDlg->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
		m_pSystemMessageDlg->UpdateWindow();
		m_pSystemMessageDlg->SetShowDlg(TRUE);
	}
}

void CDATsysView::CloseSystemMsg() 
{
	if((m_pSystemMessageDlg != NULL)
	&& (m_pSystemMessageDlg->m_bShowDlg))
	{
		m_pSystemMessageDlg->m_bShowDlg = FALSE;
		m_pSystemMessageDlg->DestroyWindow();
		delete m_pSystemMessageDlg;
		m_pSystemMessageDlg = NULL;
	}
}

/*
void CDATsysView::OnChangeCpuVersion() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	
	CurrentSet->sCPUVersion = m_szCPUVersion;	
}

void CDATsysView::OnChangeMicomVersion() 
{
	UpdateData(TRUE);

	CurrentSet->sMicomVersion = m_szMicomVersion;	
}

void CDATsysView::OnChangeUsbVersion() 
{
	UpdateData(TRUE);
	CurrentSet->sUSBVersion = m_szUSBVersion;	
}

void CDATsysView::OnChangeToolOption1() 
{
	UpdateData(TRUE);
	CurrentSet->sToolOption1 = m_szToolOption1;	
}

void CDATsysView::OnChangeToolOption2() 
{
	UpdateData(TRUE);
	CurrentSet->sToolOption2 = m_szToolOption2;	
}

void CDATsysView::OnChangeToolOption3() 
{
	UpdateData(TRUE);
	CurrentSet->sToolOption3 = m_szToolOption3;	
}

void CDATsysView::OnChangeToolOption4() 
{
	UpdateData(TRUE);
	CurrentSet->sToolOption4 = m_szToolOption4;	
}
void CDATsysView::OnChangeToolOption5() 
{
	UpdateData(TRUE);
	CurrentSet->sToolOption5 = m_szToolOption5;	
}

void CDATsysView::OnChangeAreaOption1() 
{
	UpdateData(TRUE);
	CurrentSet->sAreaOption1 = m_szAreaOption1;	
}

void CDATsysView::OnInputLock() 
{
	CString sMsg				 = _T("");

	UpdateData(TRUE);

	if(m_bInput_Lock)
	{
		ctrlToolOption1.SetReadOnly(TRUE);
		ctrlToolOption2.SetReadOnly(TRUE);
		ctrlToolOption3.SetReadOnly(TRUE);
		ctrlToolOption4.SetReadOnly(TRUE);
		//091016
		ctrlToolOption5.SetReadOnly(TRUE);

		ctrlAreaOption1.SetReadOnly(TRUE);

		ctrlCPUVersion.SetReadOnly(TRUE);
		ctrlMicomVersion.SetReadOnly(TRUE);
		ctrlUSBVersion.SetReadOnly(TRUE);
   
		if((CurrentSet->nAuthority == AUTHORITY_DEV) || (CurrentSet->nAuthority == AUTHORITY_ADMIN))
		{
			if(FileExists(CurrentSet->sModelIni))
			{
				SaveModelIniFile(CurrentSet->sModelIni);
			}
			else
			{
				sMsg.Format("Failed to save file.\n[%s]",CurrentSet->sModelIni);
				AfxMessageBox(sMsg);
			}
		}
	}
	else{
		ctrlToolOption1.SetReadOnly(FALSE);
		ctrlToolOption2.SetReadOnly(FALSE);
		ctrlToolOption3.SetReadOnly(FALSE);
		ctrlToolOption4.SetReadOnly(FALSE);
		//091016
		ctrlToolOption5.SetReadOnly(FALSE);
		
		ctrlAreaOption1.SetReadOnly(FALSE);

		ctrlCPUVersion.SetReadOnly(FALSE);
		ctrlMicomVersion.SetReadOnly(FALSE);
		ctrlUSBVersion.SetReadOnly(FALSE); 
	}
}
*/

BOOL CDATsysView::MesData_LocalSaveText(CString sCurentTime, CString strWipid, BOOL bResult, CString sTime)
{
 	CStdioFile stdFile;
	CFileException fileExcept;
	CString sFileName;
	CString sFilePath;
	CString sData;
	CString sTemp;
//	CString	sSeparator = ",;.?";


	sData = gGmesCtrl.MakeElem_S6F1(CurrentSet->sEquipmentID, m_strWipId, bResult, sTime, TRUE);

	if(bResult)
		sTemp.Format(_T("%s_%s_OK"), m_strWipId, sCurentTime);
	else
		sTemp.Format(_T("%s_%s_NG"), m_strWipId, sCurentTime);

	sFilePath = m_szExePath + _T("\\TestResult\\GMES");
	
	if(!FileExists(sFilePath))   CreateFullPath(sFilePath);

	sFileName.Format(_T("%s\\%s_%d.xml"), sFilePath, sTemp, g_nRunningProcessNo); 


    if (!stdFile.Open(sFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText, &fileExcept)) 
	{
//        fileExcept.ReportError();
        return FALSE;
    }
	stdFile.WriteString(sData);
	stdFile.Close();



	return TRUE;
}


BOOL CDATsysView::MesData_AutoUpLoad_DataCheck()
{
	CString sFileName;
	CStdioFile stdFile;
	CFileException fileExcept;
	CStringArray saMainData;
	CString sTemp;

	sFileName.Format(_T("%s\\TestResult\\GMES\\*_%d.xml"), m_szExePath, g_nRunningProcessNo); 

	if(FileExists(sFileName))
	{
		return TRUE;
	}
	else{
		return FALSE;
	}
}


void CDATsysView::OnMesDataUpload() 
{
	CString sTemp;

	if(MesData_AutoUpLoad_DataCheck())
	{
		ShowSystemMsg(MES_DATA_AUTOUPLOAD_MSG);
		MesData_AutoUpLoad();
		CloseSystemMsg();
	}
	else{
		sTemp = _T("Nothing of Data"); 
		AfxMessageBox(sTemp);
	}

}

void CDATsysView::OnModelRegistration() 
{

	CSelectGenerateModel lDlg;

	if (IDOK == lDlg.DoModal())
	{

	}
	//PROCESS_INFORMATION pInfo;
	//STARTUPINFO         sInfo;
	//char				achCommand[128];
	//CWnd				*pWndPrev, *pWndChild;
	//CString				s;

	//
	//if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	//{
	//	CString sTmp = _T("");
	//	sTmp.LoadString(ERROR_ADMIN_LOGIN);
	//	AfxMessageBox(sTmp); return;
	//}
	//
 // 
	//// Determine if another window with our class name exists...
	//if(pWndPrev = FindWindow(_T("DFT Chassis/Model Registration"),NULL))
	//{
	//	pWndChild = pWndPrev->GetLastActivePopup(); 

	//	// If iconic, restore the main window
	//	if(pWndPrev->IsIconic()) 
	//	   pWndPrev->ShowWindow(SW_RESTORE);

	//	// Bring the main window or its popup to
	//	// the foreground
	//	pWndChild->SetForegroundWindow();         
	//	return;                             
	//}

	//sInfo.cb              = sizeof(STARTUPINFO);
	//sInfo.lpReserved      = NULL;
	//sInfo.lpReserved2     = NULL;
	//sInfo.cbReserved2     = 0;
	//sInfo.lpDesktop       = NULL;
	//sInfo.lpTitle         = NULL;
	//sInfo.dwFlags         = 0;
	//sInfo.dwX             = 0;
	//sInfo.dwY             = 0;
	//sInfo.dwFillAttribute = 0;
	//sInfo.wShowWindow     = SW_SHOW;

	//// add 090902
	//s = m_szExePath + _T("\\DM_DFT_ModelReg.exe ");
	//strcpy(achCommand,(LPCSTR)s);
 //   if(!CurrentSet->sSeqPath.IsEmpty())
	//{
	//	char temp[2];
	//	temp[0] = '"';
	//	temp[1] = '\0';
	//	strcat(achCommand, temp);
	//    strcat(achCommand,(LPCSTR)CurrentSet->sSeqPath);
	//	strcat(achCommand, temp);
	//}
	//if(!CreateProcess(NULL			// address of module name
	//	, achCommand				// address of command line
	//	, NULL						// address of process security attributes
	//	, NULL						// address of thread security attributes
	//	, FALSE						// new process inherits handles
	//	, 0							// creation flags
	//	, NULL						// address of new environment block
	//	, NULL						// address of current directory name
	//	, &sInfo					// address of STARTUPINFO
	//	, &pInfo))					// address of PROCESS_INFORMATION
	//{
	////	CString s;
	//	s.LoadString(IDS_ERROR_CHILDPROCESS);
	//	OkMessage(s);
	//}
}

void CDATsysView::OnAudioMeasStart() 
{
	AudioMeasureStart();
}

void CDATsysView::OnAudioMeasStop() 
{

	AudioMeasureStop();
}	

void CDATsysView::SummaryGridDisabled(BOOL bEnable)
{
	GetDlgItem(IDC_MAINVIEW_GRID)->EnableWindow(bEnable);
}

void CDATsysView::OnScannerEnable() 
{
	BOOL bFlag;

	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	OnAudioMeasStop();
	Sleep(100);

	if(!CurrentSet->bUseScanner){
		ctrlWipIdEdit.SetReadOnly(TRUE);
		
		InitScanner(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate);
		CurrentSet->bUseScanner	= TRUE;
		ctrlWipIDLabel.SetBkColor(0x00FF00);

	}
	else{
		ctrlWipIdEdit.SetReadOnly(FALSE);

		if(ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();
		CurrentSet->bUseScanner	= FALSE;
		ctrlWipIDLabel.SetBkColor(0x808080);
	}
//	if(bFlag) SetTimer(2, 1000, NULL);
}

void CDATsysView::OnScannerDisable() 
{
	BOOL bFlag;
	ctrlWipIDLabel.SetBkColor(0x808080);

	if(!CurrentSet->bUseScanner)return;

	bFlag = m_SignalEnable;//g_pView->wIn_Flag;
	OnAudioMeasStop();
	Sleep(100);

	CurrentSet->bUseScanner	= FALSE;
	ctrlWipIdEdit.SetReadOnly(FALSE);

	if(ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();

//	if(bFlag) SetTimer(2, 1000, NULL);
}



void CDATsysView::AudioMeasureStart()
{
	//if(!g_pView->wIn_Flag)
	//{
	//	g_pView->WaveRead_Start();
	//	SetTimer(TIMER_MEASURE_AUDIO_OUTPUT, 200, NULL);

	//}
}
void CDATsysView::AudioMeasureStop()
{
	//if(g_pView->wIn_Flag)
	//{
	//	KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);
	//	g_pView->WaveRead_Stop();
	////	Sleep(500);
	//}
}

void CDATsysView::ComLogClear() 
{
	m_strLogArray.RemoveAll();
	HWND hEdit = m_ctrlEditComLog.m_hWnd;

	BOOL bReadOnly = ::GetWindowLong( hEdit, GWL_STYLE ) & ES_READONLY;
	if( bReadOnly )
		::SendMessage( hEdit, EM_SETREADONLY, FALSE, 0 );
	::SendMessage( hEdit, EM_SETSEL, 0, -1 );
	::SendMessage( hEdit, WM_CLEAR, 0, 0 );
	if( bReadOnly )
		::SendMessage( hEdit, EM_SETREADONLY, TRUE, 0 );

}



void CDATsysView::OnSeqLock() 
{
	UpdateData(TRUE);

	if(m_bSeq_Lock)
	{
		//m_ctrlSummaryGrid.SetLeftCol(1);
		//m_ctrlSummaryGrid.EnableWindow(FALSE);
	}
	else{
		//m_ctrlSummaryGrid.EnableWindow(TRUE);
	}
	
}

//This block processes the keystroke info.
LRESULT CDATsysView::ProcessKeyPad(WPARAM wParam, LPARAM lParam)
{
	LONG nStatus = 0;
//	MSG msg;
	static int count = 0;


	switch(lParam)
	{
		case 0x470001 : //7(Home)
			if((wParam == 0x24) || (wParam == 0x67)){
				nStatus = NG_BTN1;
			}
#ifdef _DEBUG
			else if (wParam == 0x01)
			{
				nStatus = 0;
				count++;
				CString strTmp;
				strTmp.Format("KeyPad:%d:", count);

				m_stcTime.SetWindowText(strTmp);
			}
#endif

			break;

		case 0x4B0001 : //4(<-)
			if((wParam == 0x25) || (wParam == 0x64)){
				nStatus = OK_BTN1;
			}
			break;

		case 0x4F0001 : //1(End)
			if((wParam == 0x23) || (wParam == 0x61)) nStatus = RETRY_BTN1; 
			break;

		case 0x520001 :  //0(Ins)
			if((wParam == 0x2D) || (wParam == 0x60)){
				if (CurrentSet->bIsRunning)
				{
					nStatus = STOP_BTN1;
				}
				else{
					nStatus = START_BTN1;
				}
			}
			break;
	
		case 0x490001 : //9(PgUp)
			if((wParam == 0x21) || (wParam == 0x69)){
				nStatus = NG_BTN2;
			}
			break;

		case 0x4D0001 : //6(->)
			if((wParam == 0x27) || (wParam == 0x66)){
				nStatus = OK_BTN2;
			}
			break;

		case 0x510001 : //3(PgDn)
			if((wParam == 0x22) || (wParam == 0x63)){
				nStatus = RETRY_BTN2;
			}
			break;

		case 0x530001 : //.(Del)
			if((wParam == 0x2E) || (wParam == 0x6E)){
				if (CurrentSet->bIsRunning)
				{
					nStatus = STOP_BTN2;
				}
				else{
					nStatus = START_BTN2;
				}
			}
			break;

		default :
			nStatus =0;
			
			if(g_nRunningProcessNo == 1)
			{
				if(wParam == VK_F5){
					nStatus = START_BTN1;
					//PostMessage(WM_COMMAND, ID_RUN_RUN);
				}
				else if(wParam == VK_ESCAPE){
					nStatus = STOP_BTN1;
					//PostMessage(WM_COMMAND, ID_RUN_ABORT);
				}  
				else if(wParam == VK_F2){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
					{
						PostMessage(WM_COMMAND, ID_EDIT_CURRENT);
					}
				}
				else if(wParam == VK_F3){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
					{
						PostMessage(WM_COMMAND, ID_MODEL_OPEN);
					}
				}
				else if(wParam == VK_F4){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
					{
						PostMessage(WM_COMMAND, ID_EDIT_COMPILE);
					}
				}
				else if(wParam == VK_F6){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
					{
						if(m_bResultDlgActivate)
						{
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
						//	m_bResultDlgActivate = FALSE;
						}
						PostMessage(WM_COMMAND, ID_RUN_MAKE_REF);
					}
				}
				else if(wParam == VK_F7){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
					{
						if(m_bResultDlgActivate)
						{
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
						//	m_bResultDlgActivate = FALSE;
						}
						PostMessage(WM_COMMAND, ID_RUN_ADJ_SPEC);
					}
				}
			}
			else if(g_nRunningProcessNo == 2)
			{
				if ((GetKeyState(VK_SHIFT) & 0x8000))
				{
					if(wParam == VK_F5){
						nStatus = START_BTN2;
						//PostMessage(WM_COMMAND, ID_RUN_RUN);
					}
					else if(wParam == VK_ESCAPE){
						nStatus = STOP_BTN2;
						//PostMessage(WM_COMMAND, ID_RUN_ABORT);
					}  
					else if(wParam == VK_F2){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							PostMessage(WM_COMMAND, ID_EDIT_CURRENT);
						}
					}
					else if(wParam == VK_F3){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							PostMessage(WM_COMMAND, ID_MODEL_OPEN);
						}
					}
					else if(wParam == VK_F4){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							PostMessage(WM_COMMAND, ID_EDIT_COMPILE);
						}
					}
					else if(wParam == VK_F6){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							//	m_bResultDlgActivate = FALSE;
							}
							PostMessage(WM_COMMAND, ID_RUN_MAKE_REF);
						}
					}
					else if(wParam == VK_F7){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							//	m_bResultDlgActivate = FALSE;
							}
							PostMessage(WM_COMMAND, ID_RUN_ADJ_SPEC);
						}
					}
				}
			}
			break;
	}

	if(g_nRunningProcessNo == 1)
	{
		switch(nStatus) 
		{
			case START_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else
					{
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							//	m_bResultDlgActivate = FALSE;
							}

						//	if(!CurrentSet->bUseScanner)
						//	{
								PostMessage(WM_COMMAND, ID_RUN_RUN);
							//	CurrentSet->bIsRunMsg = TRUE;
						//	}
						}	
					}
				}
				break;

			case STOP_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
					}
					else
					{
						if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
						{
							CurrentSet->bRunAbort = TRUE;
							g_nUserStop++;
						}

						if(m_bResultDlgActivate)
						{
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
						//	m_bResultDlgActivate = FALSE;
						}
					}
				}
				break;

			case RETRY_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
					}
				}
				break;

			case OK_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
					else if(m_UserMsgDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
				}
				break;

			case NG_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
					}
				}
				break;

			case START_BTN2: 
			case STOP_BTN2: 
			case RETRY_BTN2: 
			case OK_BTN2: 
			case NG_BTN2: 
			default :
			break;
		}
	}
	else if(g_nRunningProcessNo == 2)
	{
		switch(nStatus) 
		{
			case START_BTN1: 
			case STOP_BTN1: 
			case RETRY_BTN1: 
			case OK_BTN1: 
			case NG_BTN1: 
				break;

			case START_BTN2: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else
					{
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							//	m_bResultDlgActivate = FALSE;
							}

						//	if(!CurrentSet->bUseScanner)
						//	{
								PostMessage(WM_COMMAND, ID_RUN_RUN);
							//	CurrentSet->bIsRunMsg = TRUE;
						//	}
						}	
					}
				}
				break;

			case STOP_BTN2: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
					}
					else
					{
						if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
						{
							CurrentSet->bRunAbort = TRUE;
						}

						if(m_bResultDlgActivate)
						{
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
						//	m_bResultDlgActivate = FALSE;
						}
						
					}
				}
				break;

			case RETRY_BTN2: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
					}
				}
				break;

			case OK_BTN2: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
					else if(m_UserMsgDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
				}
				break;

			case NG_BTN2: 
				{
      				if(m_StopStepDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE)
					{
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
				}
				break;
			default :
			break;
		}
	}

	return 0L;
}

void CDATsysView::ComLog_AddString(CString szString)
{
//	if(m_bExitFlag) return;
	//CString sTime = _T("");
	//SYSTEMTIME cur_time;
	//GetLocalTime(&cur_time);

	//gPLC_Ctrl.m_sRunProcessMsgCommand = "ADD";
	//gPLC_Ctrl.m_sRunProcessMsg = szString;
	//gServer_Ctrl.SendMsgCtrl(_SERVER_COM_RUNMSG_ID); //_SERVER_COM_RUNMSG_ID

//	sTime.Format("[%02d:%02d:%02d.%03d]%s\r\n", cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds, szString);
//	sTime.Format("%s\r\n", szString);
	AppendToLogAndScroll(szString);


}


int CDATsysView::GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}


int CDATsysView::AppendToLogAndScroll(CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = m_ctrlEditComLog.GetWindowTextLength();
	m_ctrlEditComLog.SetSel(nInsertionPoint, -1);

	// Set the character format
	m_ctrlEditComLog.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing 
	// selected, this will simply insert
	// the string at the current caret position.
	m_strLogArray.Add(str); 
	m_ctrlEditComLog.ReplaceSel(str);


	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible = GetNumVisibleLines(&m_ctrlEditComLog);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (&m_ctrlEditComLog != m_ctrlEditComLog.GetFocus())
	{
		m_ctrlEditComLog.LineScroll(10);
		//m_ctrlEditComLog.LineScroll(1 - nVisible);
	}

	return 0;
}



#if 1

//#define  MAX_COLUMN_NO	13
void CDATsysView::InitMainGrid(int nGridType)
{
	CString sTmp;

	//char *sHeader1[] = { "", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT" };
	char *sHeader2[14] = {  "NO","STEP", "OK/NG", "T/T", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE","" };
	int  nWidth[14] = { 40, 117, 55,45, 70, 90, 75, 75, 65, 200, 200, 76, 500,10 }; //int  nWidth[13] = { 300,  400, 2400, 900, 700, 900, 750, 750, 650, 2000, 2000, 760, 5000};

	 
	//m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	
	////	m_CtrlListTestProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
//	m_CtrlListMainProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	//m_CtrlListMainProcess.InsertColumn(0, _T("No."), LVCFMT_CENTER, 35);
	//m_CtrlListMainProcess.InsertColumn(1, _T("STEP"), LVCFMT_CENTER, 142);// 리스트 행 추가
	//m_CtrlListMainProcess.InsertColumn(2, _T("RESULT"), LVCFMT_CENTER, 50);// 리스트 행 추가
	//m_CtrlListMainProcess.InsertColumn(3, _T("T/T"), LVCFMT_CENTER, 45);// 리스트 행 추가
	

	//m_CtrlListMainProcess.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
	//m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T("FM TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	
	



	CFont *font = new CFont;
	font->CreateFontA(16,                          // nHeight
		0,                          // nWidth
		0,                          // nEscapement
		0,                          // nOrientation
		400,                           // nWeight
		0,                          // bItalic
		0,                          // bUnderline 
		0,                          // cStrikeOut 
		0,                          // nCharSet
		0,           // nOutPrecision 
		0,                          // nClipPrecision 
		PROOF_QUALITY,              // nQuality
		0,  // nPitchAndFamily 
		_T("Arial"));                     // lpszFacename

	delete font;



	//CSize lSize;
	//CString strTemp;

	//m_CtrlListMainProcess.DeleteAllItems();

	//mCheckAll = 1;

	//for (int i = 0; i < gTestSeqData.TestCnt; i++)
	//{
	//	strTemp.Format(_T("%d"), i + 1);
	//	m_CtrlListMainProcess.InsertItem(i, strTemp);


	//	strTemp = gTestSeqData.TestStepList[i].strStepName;
	//	m_CtrlListMainProcess.SetItem(i, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
	//	m_CtrlListMainProcess.SetItem(i, 2, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
	//	m_CtrlListMainProcess.SetItem(i, 3, LVIF_TEXT, "0", NULL, NULL, NULL, NULL);
	//	strTemp = gTestSeqData.TestStepList[i].strStepWork;
	//	//		m_CtrlListMainProcess.SetItem(i, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

	//	gTestSeqData.TestStepList[i].bEnable = 1;
	//	if (gTestSeqData.TestStepList[i].bEnable == 1)
	//	{
	//		m_CtrlListMainProcess.SetCheck(i, 1);
	//	}
	//	else
	//	{
	//		m_CtrlListMainProcess.SetCheck(i, 0);
	//	}
	//}
	//m_CtrlListMainProcess.Invalidate();

	///////////////////////////////////////////////////////////
	
	m_CtrlListMainProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가


	if (nGridType == DETAILEDGRID)
	{
		//m_ctrlSummaryGrid.SetCols(13);
		//m_ctrlSummaryGrid.SetRows(1);
		//m_ctrlSummaryGrid.SetExtendLastCol(TRUE);
		////		m_ctrlSummaryGrid.SetFrozenCols(3);	
		//m_ctrlSummaryGrid.SetFrozenCols(2);

		// Set Column Alignment & Column Title
		for (int nCol = 0; nCol < MAX_COLUMN_NO; nCol++)
		{
			m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, nWidth[nCol]);
			
					
		}

		m_CtrlListMainProcess.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
		m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T("FM TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 


	}

	m_CtrlListMainProcess.Invalidate();
}

#else
void CDATsysView::InitMainGrid(int nGridType)
{
	CString sTmp;

	char *sHeader1[] = {"", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT"};
	char *sHeader2[] = {"", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE"};

	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	m_ctrlSummaryGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlSummaryGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlSummaryGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlSummaryGrid.SetBackColor(RGB(247,247,231));
//	m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
	m_ctrlSummaryGrid.SetGridColor(RGB(200,200,176));

	m_ctrlSummaryGrid.SetFixedRows(0);
	m_ctrlSummaryGrid.SetFixedCols(0);
	m_ctrlSummaryGrid.SetFrozenRows(1);

	m_ctrlSummaryGrid.SetFillStyle(flexFillRepeat);
	m_ctrlSummaryGrid.SetFontName("Arial");
//	m_ctrlSummaryGrid.SetFontSize(10);
	m_ctrlSummaryGrid.SetFontSize(9);

	if(nGridType == DETAILEDGRID) 
	{
		m_ctrlSummaryGrid.SetCols(13);
		m_ctrlSummaryGrid.SetRows(1);
		m_ctrlSummaryGrid.SetExtendLastCol(TRUE);
//		m_ctrlSummaryGrid.SetFrozenCols(3);	
		m_ctrlSummaryGrid.SetFrozenCols(2);	

		// Set Column Alignment & Column Title
		for(int nCol = 0; nCol < 13 ; nCol++)
		{
			m_ctrlSummaryGrid.SetRow(0);
			m_ctrlSummaryGrid.SetCol(nCol);
		
			m_ctrlSummaryGrid.SetText(sHeader2[nCol]);
		
			switch(nCol)
			{
				case 0 : // Check
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 300); //300
						 m_ctrlSummaryGrid.SetColDataType(nCol, flexDTBoolean);
						 m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
				  		 break;

				case 1 : // No
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 400); //500
				  		 break;

				case 2 : // Step
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignLeftCenter);
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 2400);//3100
				  		 break;

				case 3 : // Result
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 900);
				  		 break;

				case 4 : // Measure
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 700);
				  		 break;

				case 5 : // Target
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 900);
				  		 break;

				case 6 : // L-Limit
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 750);
				  		 break;

				case 7 : // U-Limit
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 750);
				  		 break;

				case 8 : // Unit
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 650);
				  		 break;
				
				case 9 : // Audio Target
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 2000);
				  		 break;
				case 10 : // Audio Measure
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 2000);
				  		 break;
				case 11 : // Elapsed Time
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 760);
				  		 break;

				case 12 : // Message
						 m_ctrlSummaryGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nCol, 5000);
				  		 break;
			}
			m_ctrlSummaryGrid.SetFixedAlignment(nCol,flexAlignCenterCenter);
		}

	}


	m_ctrlSummaryGrid.SetRedraw(-1);
	m_ctrlSummaryGrid.Refresh();
}
#endif
void CDATsysView::InitVersionGrid()
{
	CString sTmp;

	m_CtrlListVersionProcess.DeleteAllItems();

	char *sHeader[] = {"BE Ver.", "FE Ver.", "Micom Ver.", "DSP Ver.", "Sub DSP Ver.", "EQ Ver.", "MEQ Ver.", "Touch Ver.", "Demo Ver.", "DJ Sound Ver.", "WirelessTx Ver.", "WirelessRx Ver.","BT Version", "HDMI Version", "Checksum"};
	

	// Set Column Alignment & Column Title
	m_CtrlListVersionProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListVersionProcess.InsertColumn(0, _T("No"), LVCFMT_CENTER, 30);
	m_CtrlListVersionProcess.InsertColumn(1, _T("ITEM"), LVCFMT_CENTER, 117);
	m_CtrlListVersionProcess.InsertColumn(2, _T("VERSION"), LVCFMT_CENTER, 110);
	
	int lCount = 0;
	for(int nRow = 0; nRow < 15 ; nRow++)
	{
		if (CurrentSet->bVerChecked[nRow] == 1)
		{

			sTmp.Format(_T("%2d"), nRow + 1);
			m_CtrlListVersionProcess.InsertItem(lCount, sTmp);   // 첫째행(0), 첫째열에 삽입	
			m_CtrlListVersionProcess.SetItem(lCount, 1, LVIF_TEXT, sHeader[nRow], NULL, NULL, NULL, NULL);
			lCount++;
		}
		
	}
////	m_ctrlVersionGrid.SetFixedAlignment(0,flexAlignRightCenter);
//	m_ctrlVersionGrid.SetColAlignment(0,flexAlignRightCenter);
//	m_ctrlVersionGrid.SetColAlignment(1,flexAlignCenterCenter);
//
//	m_ctrlVersionGrid.SetCol(-1);
//	m_ctrlVersionGrid.SetRow(-1);

//	m_ctrlVersionGrid.SetRedraw(-1);
//	m_ctrlVersionGrid.Refresh();
}

#if 1
void CDATsysView::InsertStepData2Grid(int nGridType)
{
	char *sHeader2[13] = { "", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE" };
	int  nWidth[13] = { 30,  140, 90, 70, 90, 75, 75, 65, 200, 200, 76, 500, };

	//return;
	
	//+Insert Step Default Data
	POSITION Position;
	CStep* pStep;
	CString sTmp;
	int nTmp = 0;
	
	
	m_CtrlListMainProcess.DeleteAllItems();

	Position = StepList.GetHeadPosition();

	if (nGridType == DETAILEDGRID)
	{
		//m_ctrlSummaryGrid.SetRow(0);
		//		m_ctrlSummaryGrid.SetFrozenCols(3);
		//m_ctrlSummaryGrid.SetFrozenCols(2);
		//for (nTmp = 0; nTmp < MAX_COLUMN_NO; nTmp++)
		//{
		//	m_CtrlListMainProcess.InsertColumn(nTmp, _T(sHeader2[nTmp]), LVCFMT_CENTER, nWidth[nTmp]);
		//}
	
		if (StepList.GetCount() > 0)
		{
			nTmp = 0;

			while (Position)
			{
				pStep = StepList.GetNext(Position);

				//(Column 1) Step No
				sTmp.Format("%d", pStep->GetStepNo());
				m_CtrlListMainProcess.InsertItem(nTmp, sTmp);   // 첫째행(0), 첫째열에 삽입
				//(Column 0) Step Check 

				if (pStep->GetTest() == TRUE)
				{
					m_CtrlListMainProcess.SetCheck(nTmp, 1); //  m_ctrlSummaryGrid.SetCellChecked(flexChecked);
					m_LockStatus[nTmp] = 1;
				}
				else
				{
					m_CtrlListMainProcess.SetCheck(nTmp, 0); // m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
					m_LockStatus[nTmp] = 0;
				}


			//(Column 2) Step Name
			
				if (pStep->m_nTestType == ADC_TEST || pStep->m_nTestType == TV_COMM_TEST)
				{
					m_CtrlListMainProcessEx.SetCellBackColor(nTmp, RGB(189, 231, 97)); //  m_ctrlSummaryGrid.SetCellBackColor(RGB(189, 231, 97));
					m_CtrlListMainProcessEx.SetCellForeColor(nTmp, RGB(255, 255, 255));//m_ctrlSummaryGrid.SetCellForeColor(RGB(	255,	 255,   255 ));
				}
				else
				{
					m_CtrlListMainProcessEx.SetCellBackColor(nTmp, RGB(255, 255, 255));
					m_CtrlListMainProcessEx.SetCellForeColor(nTmp, RGB(0, 0, 0));
				}
				m_CtrlListMainProcess.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetStepName(), NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nTmp, 2, pStep->GetStepName());


				//(Column 4) Measure
		/*		m_ctrlSummaryGrid.SetCol(4);
				m_ctrlSummaryGrid.SetColDataType(4, flexDTString);*/

				if (pStep->m_nStepType == CHECK_FUNCTION)
				{
					if (pStep->GetFuncType() == MEAS_BOOL)
					{
						if (pStep->GetNominal_Bool() == FALSE) sTmp.Format(_T("%s"), "FALSE");
						else sTmp.Format(_T("%s"), "TRUE");
					}
					else sTmp.Format("%4.1f", pStep->GetNominal_float());

					// (Column 5) Target
					//m_ctrlSummaryGrid.SetCol(5);
					//m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
					m_CtrlListMainProcess.SetItem(nTmp, 5, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nTmp, 5, sTmp);



					if (pStep->GetFuncType() != MEAS_BOOL)
					{
						// (Column 6) Low Limit
						sTmp.Format("%4.1f", pStep->GetLowLimit());
						//m_ctrlSummaryGrid.SetCol(6);
						//m_ctrlSummaryGrid.SetColDataType(6, flexDTString);
						m_CtrlListMainProcess.SetItem(nTmp, 6, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 6, sTmp);

						// (Column 7) High Limit
						sTmp.Format("%4.1f", pStep->GetHighLimit());
						//m_ctrlSummaryGrid.SetCol(7);
						//m_ctrlSummaryGrid.SetColDataType(7, flexDTString);
						m_CtrlListMainProcess.SetItem(nTmp, 7, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 7, sTmp);

						// (Column 8) Unit
						sTmp.Format(_T("%s"), pStep->GetUnit());
					//	m_ctrlSummaryGrid.SetCol(8);
					//	m_ctrlSummaryGrid.SetColDataType(8, flexDTString);
						m_CtrlListMainProcess.SetItem(nTmp, 8, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 8, sTmp);


					}
					//+2007.10.15 Add BY USY
					if (pStep->m_bRunAudioTest == TRUE)
					{
						// (Column 9) Audio Target
						// change 090323
						if (pStep->m_nMeasureAudioType == LEVEL_CHECK) {
							sTmp.Format(_T("X, X, %d, %d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
						}
						else if (pStep->m_nMeasureAudioType == FREQUENCY_CHECK) {
							sTmp.Format(_T("%d, %d, X, X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
						}
						else {
							sTmp.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
						}

						//m_ctrlSummaryGrid.SetCol(9);
						//m_ctrlSummaryGrid.SetColDataType(9, flexDTString);
						m_CtrlListMainProcess.SetItem(nTmp, 9, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 9, sTmp);
					}
					//-
				}
				nTmp++;
			}
			//Result Row
			;
			//(Column 1) Step No
			//sTmp.Format("%d", StepList.GetCount() + 1);
			///m_ctrlSummaryGrid.SetCol(1);
			//m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
			m_CtrlListMainProcess.InsertItem(nTmp, _T("")); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 1, _T(""));		
			m_CtrlListMainProcess.SetItem(nTmp, 1, LVIF_TEXT, "Total", NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 2, "Total");

			//m_ctrlSummaryGrid.SetRow(0);
			//m_ctrlSummaryGrid.SetCol(0);
			//m_ctrlSummaryGrid.SetCellChecked(flexChecked);
			//+ 2007.10.10 Add BY USY
			m_CtrlListMainProcessEx.SetCellBackColor(nTmp, RGB(247, 247, 231));
			//			m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
			//m_ctrlSummaryGrid.SetGridColor(RGB(200, 200, 176));
			//-
		}
	}
	m_CtrlListMainProcess.Invalidate();
	return;
}


#else
void CDATsysView::InsertStepData2Grid(int nGridType)
{
	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	m_ctrlSummaryGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything))); 

	//+ 2007.10.10 Add BY USY
	char *sHeader1[] = {"", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT"};
	char *sHeader2[] = {"", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE"};
	//-

	//Row 
	if(CurrentSet->bCompiled)
	{
		if( nGridType == DETAILEDGRID)
		{
			if(StepList.GetCount() + 1 < 12) m_ctrlSummaryGrid.SetRows(12);
			else m_ctrlSummaryGrid.SetRows(StepList.GetCount() + 2);
		}
	}
	else m_ctrlSummaryGrid.SetRows(12);

	//+Insert Step Default Data
	POSITION Position;
	CStep* pStep;
	CString sTmp;
	int nTmp = 0;
	Position = StepList.GetHeadPosition();

	if(nGridType == DETAILEDGRID)
	{
		m_ctrlSummaryGrid.SetRow(0);
//		m_ctrlSummaryGrid.SetFrozenCols(3);
		m_ctrlSummaryGrid.SetFrozenCols(2);

		for( nTmp = 0; nTmp < 13 ; nTmp++)
		{
			m_ctrlSummaryGrid.SetCol(nTmp);
			m_ctrlSummaryGrid.SetText(sHeader2[nTmp]);
			m_ctrlSummaryGrid.SetCellBackColor(RGB(210,216,176));
			m_ctrlSummaryGrid.SetCellForeColor(RGB(51,102,153));
		
			switch(nTmp)
			{
				case 0 : // Check
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);		
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 300); //300
						 m_ctrlSummaryGrid.SetColDataType(nTmp, flexDTBoolean);
						 m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
				  		 break;

				case 1 : // No
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);		
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 400);  //500
				  		 break;

				case 2 : // Step
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignLeftCenter);
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 2400); //3100
				  		 break;

				case 3 : // Result
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
				  		 break;

				case 4 : // Measure
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 700);
				  		 break;

				case 5 : // Target
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
				  		 break;

				case 6 : // L-Limit
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
				  		 break;

				case 7 : // U-Limit
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignRightCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
				  		 break;

				case 8 : // Unit
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 650);
				  		 break;
				
				case 9 : // Audio Target
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
				  		 break;
				case 10 : // Audio Measure
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
				  		 break;
				case 11 : // Elapsed Time
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 760);
				  		 break;

				case 12 : // Message
						 m_ctrlSummaryGrid.SetColAlignment(nTmp,flexAlignCenterCenter);	
	 					 m_ctrlSummaryGrid.SetColWidth(nTmp, 5000);
				  		 break;
			}
		}

		if(StepList.GetCount() > 0)
		{
			nTmp = 1;

			while (Position) 
			{
				pStep = StepList.GetNext(Position);

				m_ctrlSummaryGrid.SetRow(nTmp);

				//(Column 0) Step Check 
				m_ctrlSummaryGrid.SetCol(0);
				m_ctrlSummaryGrid.SetColDataType(0, flexDTBoolean);

				if(pStep->GetTest() == TRUE) m_ctrlSummaryGrid.SetCellChecked(flexChecked);
				else m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);

				//(Column 1) Step No
				sTmp.Format("%d", pStep->GetStepNo());
				m_ctrlSummaryGrid.SetCol(1);
				m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
				m_ctrlSummaryGrid.SetTextMatrix(nTmp , 1, sTmp);
			
				//(Column 2) Step Name
				m_ctrlSummaryGrid.SetCol(2);
				m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
				if(pStep->m_nTestType == ADC_TEST || pStep->m_nTestType == TV_COMM_TEST)
				{
					m_ctrlSummaryGrid.SetCellBackColor(RGB(189,231,97));
					//m_ctrlSummaryGrid.SetCellForeColor(RGB(	255,	 255,   255 ));
				}
				m_ctrlSummaryGrid.SetTextMatrix(nTmp , 2, pStep->GetStepName());
				

				//(Column 4) Measure
				m_ctrlSummaryGrid.SetCol(4);
				m_ctrlSummaryGrid.SetColDataType(4, flexDTString);

				if(pStep->m_nStepType == CHECK_FUNCTION)
				{ 
					if(pStep->GetFuncType() == MEAS_BOOL)
					{						
						if(pStep->GetNominal_Bool() == FALSE) sTmp.Format(_T("%s"), "FALSE");
						else sTmp.Format(_T("%s"), "TRUE");
					}
					else sTmp.Format("%4.1f", pStep->GetNominal_float());

					// (Column 5) Target
					m_ctrlSummaryGrid.SetCol(5);
					m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
					m_ctrlSummaryGrid.SetTextMatrix(nTmp , 5, sTmp);

					if(pStep->GetFuncType() != MEAS_BOOL)
					{
						// (Column 6) Low Limit
						sTmp.Format("%4.1f", pStep->GetLowLimit());
						m_ctrlSummaryGrid.SetCol(6);
						m_ctrlSummaryGrid.SetColDataType(6, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp , 6, sTmp);

						// (Column 7) High Limit
						sTmp.Format("%4.1f", pStep->GetHighLimit());
						m_ctrlSummaryGrid.SetCol(7);
						m_ctrlSummaryGrid.SetColDataType(7, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp , 7, sTmp);

						// (Column 8) Unit
						sTmp.Format(_T("%s"), pStep->GetUnit());
						m_ctrlSummaryGrid.SetCol(8);
						m_ctrlSummaryGrid.SetColDataType(8, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp , 8, sTmp);
						

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

						m_ctrlSummaryGrid.SetCol(9);
						m_ctrlSummaryGrid.SetColDataType(9, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp , 9, sTmp);
					}
					//-
				}
				nTmp++;
			}
			//Result Row
;
			//(Column 1) Step No
			//sTmp.Format("%d", StepList.GetCount() + 1);
			m_ctrlSummaryGrid.SetCol(1);
			m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
			m_ctrlSummaryGrid.SetTextMatrix(nTmp , 1, _T(""));
		
			//(Column 2) Step Name
			m_ctrlSummaryGrid.SetCol(2);
			m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
			m_ctrlSummaryGrid.SetTextMatrix(nTmp , 2, "Total");

			m_ctrlSummaryGrid.SetRow(0);
			m_ctrlSummaryGrid.SetCol(0);
			m_ctrlSummaryGrid.SetCellChecked(flexChecked); 
			//+ 2007.10.10 Add BY USY
			m_ctrlSummaryGrid.SetCellBackColor(RGB(247,247,231));
//			m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
			m_ctrlSummaryGrid.SetGridColor(RGB(200,200,176));
			//-
		}
	}
	m_ctrlSummaryGrid.SetCol(-1);
	m_ctrlSummaryGrid.SetRow(-1);
	m_ctrlSummaryGrid.SetRedraw(-1);
	m_ctrlSummaryGrid.Refresh();
	return;
}
#endif
void CDATsysView::InsertResultData2Grid(int nGridType, int nStepNo)
{
	if(nGridType == DETAILEDGRID) InsertResult2DetailedGrid(nStepNo);
} 
void CDATsysView::UpdateGridStepTime(CString strStepTime,   int nStepNo)
{
	nStepNo -= 1;
	m_CtrlListMainProcess.SetItem(nStepNo, 3, LVIF_TEXT, strStepTime, NULL, NULL, NULL, NULL);
} 
//void CDATsysView::InsertResult2NormalGrid(int nStepNo)
//{
//	if ((nStepNo >= (m_CtrlListMainProcess.GetItemCount())) || (nStepNo <= 0)) return;
//
//	CString sTmp;
//
//	int nRow, nCol; // nCol = 3 or 7 or 11
//	nRow = ((nStepNo - 1) / 3) + 1;
//
//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
//	m_ctrlSummaryGrid.SetRow(nRow);
//	if (nRow % 20 == 0) m_ctrlSummaryGrid.SetTopRow(nRow);
//
//	if (nStepNo % 3 == 1) nCol = 3;
//	else if (nStepNo % 3 == 2) nCol = 7;
//	else if (nStepNo % 3 == 0) nCol = 11;
//
//	m_ctrlSummaryGrid.SetRow(nRow);
//	m_ctrlSummaryGrid.SetCol(nCol);
//	m_ctrlSummaryGrid.SetColDataType(nCol, flexDTString);
//
//	if (pCurStep->m_bTest == TRUE)
//	{
//		if (pCurStep->m_bResult)
//		{
//			sTmp = _T("OK");
//			m_ctrlSummaryGrid.SetCellBackColor(RGB(0, 0, 255));
//			m_ctrlSummaryGrid.SetCellForeColor(RGB(255, 255, 255));
//			m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//
//		}
//		else // Video Test NG
//		{
//			//090324
//			sTmp = _T("NG");
//			if (pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
//			{
//
//				if (pCurStep->m_bVideoTestResult == FALSE)
//				{
//					sTmp = _T("NG/V");
//				}
//
//				if (pCurStep->m_bAudioTestResult == FALSE)
//				{
//					sTmp = _T("NG/A");
//				}
//
//				if ((pCurStep->m_bVideoTestResult == FALSE) && (pCurStep->m_bAudioTestResult == FALSE))
//				{
//					sTmp = _T("NG/AV");
//				}
//			}
//			else if (!pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
//			{
//				if (pCurStep->m_bVideoTestResult == FALSE)
//				{
//					sTmp = _T("NG/V");
//				}
//			}
//			else if (pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
//			{
//				if (pCurStep->m_bAudioTestResult == FALSE)
//				{
//					sTmp = _T("NG/A");
//				}
//			}
//			else
//			{
//				sTmp = _T("NG");
//			}
//
//			m_ctrlSummaryGrid.SetCellBackColor(RGB(255, 0, 0));
//			m_ctrlSummaryGrid.SetCellForeColor(RGB(255, 255, 255));
//			m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//		}
//	}
//	else if (pCurStep->m_bTest == FALSE)
//	{
//		sTmp = _T("SKIP");
//		m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//	}
//
//	//+2007.10.15 Add BY USY
//	m_ctrlSummaryGrid.SetRow(-1);
//	m_ctrlSummaryGrid.SetCol(-1);
//	//-
//	m_ctrlSummaryGrid.SetRedraw(-1);
//	m_ctrlSummaryGrid.Refresh();
//	return;
//	//if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;
//
//	//CString sTmp;
//
//	//int nRow, nCol; // nCol = 3 or 7 or 11
//	//nRow = ((nStepNo - 1) / 3) + 1;
//
//	//m_ctrlSummaryGrid.SetRedraw(flexRDNone);
//	//m_ctrlSummaryGrid.SetRow(nRow);
//	//if(nRow % 20 == 0) m_ctrlSummaryGrid.SetTopRow(nRow);
//
//	//if(nStepNo % 3 == 1) nCol = 3;
//	//else if(nStepNo % 3 == 2) nCol = 7;
//	//else if(nStepNo % 3 == 0) nCol = 11;
//
//	//m_ctrlSummaryGrid.SetRow(nRow);
//	//m_ctrlSummaryGrid.SetCol(nCol);
//	//m_ctrlSummaryGrid.SetColDataType(nCol, flexDTString);
//	//
//	//if(pCurStep->m_bTest == TRUE)
//	//{
//	//	if (pCurStep->m_bResult)
//	//	{
//	//		sTmp = _T("OK");
//	//		m_ctrlSummaryGrid.SetCellBackColor(RGB(0,0,255));
//	//		m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
//	//		m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//	//		
//	//	}
//	//	else // Video Test NG
//	//	{
//	//		//090324
//	//		sTmp = _T("NG");
//	//		if(pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
//	//		{
//
//	//			if(pCurStep->m_bVideoTestResult == FALSE)
//	//			{
//	//				sTmp = _T("NG/V");
//	//			}
//
//	//			if(pCurStep->m_bAudioTestResult == FALSE)
//	//			{
//	//				sTmp = _T("NG/A");
//	//			}
//
//	//			if((pCurStep->m_bVideoTestResult == FALSE) && (pCurStep->m_bAudioTestResult == FALSE))
//	//			{
//	//				sTmp = _T("NG/AV");
//	//			}
//	//		}
//	//		else if(!pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
//	//		{
//	//			if(pCurStep->m_bVideoTestResult == FALSE)
//	//			{
//	//				sTmp = _T("NG/V");
//	//			}
//	//		}
//	//		else if(pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
//	//		{
//	//			if(pCurStep->m_bAudioTestResult == FALSE)
//	//			{
//	//				sTmp = _T("NG/A");
//	//			}
//	//		}
//	//		else
//	//		{
//	//			sTmp = _T("NG");
//	//		}
//	//			
//	//		m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
//	//		m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
//	//		m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//	//	}
//	//}
//	//else if(pCurStep->m_bTest == FALSE)
//	//{
//	//	sTmp = _T("SKIP");
//	//	m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//	//}
//	//
//	////+2007.10.15 Add BY USY
//	//m_ctrlSummaryGrid.SetRow(-1);
//	//m_ctrlSummaryGrid.SetCol(-1);
//	////-
//	//m_ctrlSummaryGrid.SetRedraw(-1);
//	//m_ctrlSummaryGrid.Refresh();
//	//return ;
//}

void CDATsysView::InsertResult2DetailedGrid(int nStepNo)
{
	if((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return;
	nStepNo -= 1;
    CString sTmp;
//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
//	m_ctrlSummaryGrid.SetRow(nStepNo);

	//if (nStepNo % 7 == 0) m_CtrlListMainProcess.Scroll(nStepNo*20);//.settop.SetTopRow(nStepNo);
		
	// (Column 11) Elapsed Time
    if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);

	//	m_ctrlSummaryGrid.SetCol(11);
	//	m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	//	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);
		
	//	m_ctrlSummaryGrid.SetRedraw(-1);
	//	m_ctrlSummaryGrid.Refresh();
		m_CtrlListMainProcessEx.SetCellBackColor(nStepNo, 2, RGB(255, 255, 128));
		m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, "PASS", NULL, NULL, NULL, NULL);
		m_CtrlListMainProcess.SetItem(nStepNo, 11, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
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
			//m_ctrlSummaryGrid.SetCol(3);
			//m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
			m_CtrlListMainProcessEx.SetCellBackColor(nStepNo, 2, RGB(0,0,255));
			m_CtrlListMainProcessEx.SetCellForeColor(nStepNo, 2, RGB(255,255,255));
			//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);

			m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
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

			//m_ctrlSummaryGrid.SetCol(3);
			//m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
			//m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
			//m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
			//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);

			m_CtrlListMainProcessEx.SetCellBackColor(nStepNo, 2, RGB(255, 0, 0));
			m_CtrlListMainProcessEx.SetCellForeColor(nStepNo, 2, RGB(255, 255, 255));
			m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
		}
		if(pCurStep->m_bRunAudioTest)
		{
			sTmp.Format(_T("%d, %d, %d, %d"), pCurStep->m_nLeftFreq[1], pCurStep->m_nRightFreq[1], pCurStep->m_nLeftLevel[1], pCurStep->m_nRightLevel[1]);

			//m_ctrlSummaryGrid.SetCol(10);
			//m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			m_CtrlListMainProcess.SetItem(nStepNo, 10, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
		}
		
		//+add kwmoon 081013
		if(pCurStep->m_bAdcValueCheckStep)
		{
			//========================================================
			// (Column 10) Display ADC value instead of Audio Measure
			//========================================================
			sTmp = pCurStep->m_szAdcValue;
			//m_ctrlSummaryGrid.SetCol(10);
			//m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			m_CtrlListMainProcess.SetItem(nStepNo, 10, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
		}
	}
	//======
	// SKIP
	//======
	else // if(pCurStep->m_bTest == FALSE)
	{
		sTmp = _T("SKIP");
		//m_ctrlSummaryGrid.SetCol(3);
		//m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
		//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);
		//m_ctrlSummaryGrid.SetRedraw(-1);
		//m_ctrlSummaryGrid.Refresh();

		m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // 
		return;
	}

	// (Column 4) Measure
	if(pCurStep->m_nFuncType == MEAS_BOOL)
	{	
		if(pCurStep->m_bMeasured == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
    }
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fMeasured);

	if(pCurStep->m_nTestType == MULTICHECL_TEST){
		sTmp.Format(_T("%.02f, %.02f"), pCurStep->m_fMeasured_L,pCurStep->m_fMeasured_R);
	}

	//m_ctrlSummaryGrid.SetCol(4);
	//m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, sTmp);

	// (Column 5) Target
	if(pCurStep->m_nFuncType == MEAS_BOOL)
	{	
		if(pCurStep->m_bNominal == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
    }
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fNominal);

	//m_ctrlSummaryGrid.SetCol(5);
	//m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 5, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);

							

	// (Column 11) Elapsed Time
	sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);
	//m_ctrlSummaryGrid.SetCol(11);
	//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 11, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);

	//m_ctrlSummaryGrid.SetRedraw(-1);
	//m_ctrlSummaryGrid.Refresh();
	m_CtrlListMainProcess.Invalidate();

	return ;
}

void CDATsysView::InsertMsg2DetailedGrid(int nStepNo, CString sMsg)
{
	CString szDetailMessage = sMsg;

	if((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return;

//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
//	m_ctrlSummaryGrid.SetRow(nStepNo);
	m_CtrlListMainProcess.SetItem(nStepNo-1, 12, LVIF_TEXT, szDetailMessage, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 12, szDetailMessage);
	
//	m_ctrlSummaryGrid.SetRedraw(-1);
//	m_ctrlSummaryGrid.Refresh();
	m_CtrlListMainProcess.Invalidate();
}

void CDATsysView::InsertTotalData2Grid(int nGridType, BOOL bResult, CString sElapsedTime)
{
	if(nGridType == DETAILEDGRID)
	{
		if(StepList.GetCount() > 0)
		{
			CString sTmp;
			//m_ctrlSummaryGrid.SetRedraw(flexRDNone);
			//m_ctrlSummaryGrid.SetCol(3);
			//m_ctrlSummaryGrid.SetRow(StepList.GetCount() + 1);
			if(bResult == TRUE)
			{
			//	m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				m_CtrlListMainProcessEx.SetCellBackColor(StepList.GetCount(), 2, RGB(0,0,255));
				m_CtrlListMainProcessEx.SetCellForeColor(StepList.GetCount(), 2, RGB(255,255,255));
				m_CtrlListMainProcess.SetItem(StepList.GetCount(), 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "OK");
			}
			else if(bResult == FALSE)
			{
				
			//	m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				m_CtrlListMainProcessEx.SetCellBackColor(StepList.GetCount(), 2, RGB(255,0,0));
				m_CtrlListMainProcessEx.SetCellForeColor(StepList.GetCount(), 2, RGB(255,255,255));
				m_CtrlListMainProcess.SetItem(StepList.GetCount(), 2, LVIF_TEXT, "NG", NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "NG");
			}


		//	m_ctrlSummaryGrid.SetCol(11);
			//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
			m_CtrlListMainProcess.SetItem(StepList.GetCount(), 11, LVIF_TEXT, sElapsedTime, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 11, sElapsedTime);

			m_CtrlListMainProcess.SetItem(StepList.GetCount(), 3, LVIF_TEXT, sElapsedTime, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 11, sElapsedTime);

		//	m_ctrlSummaryGrid.SetRedraw(-1);
		//	m_ctrlSummaryGrid.Refresh();
			m_CtrlListMainProcess.Invalidate();
		}
	}
}

void CDATsysView::InsertPcbid2Grid(CString sWipID)
{
	if(StepList.GetCount() > 0)
	{
		//m_ctrlSummaryGrid.SetCol(12);
		//m_ctrlSummaryGrid.SetColDataType(12, flexDTString);
		m_CtrlListMainProcess.SetItem(StepList.GetCount(), 12, LVIF_TEXT, sWipID, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 12, sWipID);
		
	//	m_ctrlSummaryGrid.SetRedraw(-1);
	//	m_ctrlSummaryGrid.Refresh();
		m_CtrlListMainProcess.Invalidate();
	}
}


//-

void CDATsysView::DeleteResultDataFromGrid(int nGridType, int nStepNo)
{
	if(nGridType == DETAILEDGRID) DeleteResultDataFromDetailedGrid(nStepNo);
}
//void CDATsysView::DeleteResultDataFromNormalGrid(int nStepNo)
//{
//	if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;
//
//	CString sTmp;
//
//	int nRow, nCol; // nCol = 3 or 7 or 11
//	nRow = ((nStepNo - 1) / 3) + 1;
//
//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
//	m_ctrlSummaryGrid.SetRow(nRow);
//	if(nRow % 20 == 0) m_ctrlSummaryGrid.SetTopRow(nRow);
//
//	if(nStepNo % 3 == 1) nCol = 3;
//	else if(nStepNo % 3 == 2) nCol = 7;
//	else if(nStepNo % 3 == 0) nCol = 11;
//
//	m_ctrlSummaryGrid.SetRow(nRow);
//	m_ctrlSummaryGrid.SetCol(nCol);
//	m_ctrlSummaryGrid.SetColDataType(nCol, flexDTString);
//	
//	m_ctrlSummaryGrid.SetCellBackColor(RGB(255,255,255));
//	m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, _T(""));
//
//	m_ctrlSummaryGrid.SetRedraw(-1);
//	m_ctrlSummaryGrid.Refresh();
//	return ;
//}
void CDATsysView::DeleteResultDataFromDetailedGrid(int nStepNo)
{
		if((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return;

    CString sTmp;
//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
//	m_ctrlSummaryGrid.SetRow(nStepNo);

	//if(nStepNo % 7 == 0) m_CtrlListMainProcess.Scroll(nStepNo*20);
		
	// (Column 11) Elapsed Time
    if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		//m_ctrlSummaryGrid.SetCol(11);
		//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
		m_CtrlListMainProcess.SetItem(nStepNo, 11, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));
		
	//	m_ctrlSummaryGrid.SetRedraw(-1);
	//	m_ctrlSummaryGrid.Refresh();

		return;
	}

	// (Column 3) Result
	//m_ctrlSummaryGrid.SetCol(3);
	//m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
	m_CtrlListMainProcessEx.SetCellBackColor(StepList.GetCount(), 2, RGB(255,255,255));
	m_CtrlListMainProcessEx.SetCellForeColor(StepList.GetCount(), 2, RGB(255,255,255));
	m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, _T(""));
	   
	if(pCurStep->m_bRunAudioTest)
	{
		// (Column 10) Audio Measure
		//m_ctrlSummaryGrid.SetCol(10);
		//m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
		m_CtrlListMainProcess.SetItem(nStepNo, 10, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, _T(""));

	}

	// (Column 4) Measure
//	m_ctrlSummaryGrid.SetCol(4);
//	m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 4, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, _T(""));

	//m_ctrlSummaryGrid.SetCol(5);
	//m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 5, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);


	// (Column 11) Elapsed Time
	//m_ctrlSummaryGrid.SetCol(11);
	//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 11, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));

	//m_ctrlSummaryGrid.SetRow(0);
	//m_ctrlSummaryGrid.SetCol(0);
	//m_ctrlSummaryGrid.SetRedraw(-1);
	//m_ctrlSummaryGrid.Refresh();
	m_CtrlListMainProcess.Invalidate();
	return ;
}

BOOL CDATsysView::GetResultFromDetailedGrid(int nStepNo,CString& sMsg)
{
	if((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return FALSE;
	
	sMsg = m_CtrlListMainProcess.GetItemText(nStepNo, 12);

	return TRUE;
}

void CDATsysView::GetCheckStep(int nGridType)
{
	POSITION Position;
	CStep* pStep;
	int nRow, nCol, nTmp;

	Position = StepList.GetHeadPosition();
	if(StepList.GetCount() > 0)
	{
		nTmp = 0;
		nRow = 0;
		while (Position) 
		{
			pStep = StepList.GetNext(Position);
			
			if(nGridType == DETAILEDGRID)
			{
				nCol = 0;
			}

			//g_pView->m_ctrlSummaryGrid.SetRow(nRow);
			//g_pView->m_ctrlSummaryGrid.SetCol(nCol);
			if(g_pView->m_CtrlListMainProcess.GetCheck(nRow) == 1)
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
int CDATsysView::GetNoCheckedStep(int nGridType)
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

			//g_pView->m_ctrlSummaryGrid.SetRow(nRow);
		//	g_pView->m_ctrlSummaryGrid.SetCol(nCol);

			if(m_CtrlListMainProcess.GetCheck(nRow) )
			{
				nNoCheckedStep++;
			}
			nRow++;
		}
	}
	return nNoCheckedStep;
}
void CDATsysView::SaveResult2File(CString sRstPath, BOOL bSave2Txt, BOOL bSave2Html, BOOL bSave2Csv)
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
		SaveGridToText(&m_CtrlListMainProcess, (char*)(LPCTSTR)sTxtPath); //g_pView->m_ctrlSummaryGrid.SaveGrid(sTxtPath, flexFileTabText, COleVariant(long(TRUE)));
	}

	if(bSave2Html == TRUE)
	{
		SaveGridToHTML(&m_CtrlListMainProcess,(char*)(LPCTSTR)sHtmlPath);
	}
	if(bSave2Csv == TRUE)
	{
		SaveGridToCSV(&m_CtrlListMainProcess, (char*)(LPCTSTR)sCsvPath);// g_pView->m_ctrlSummaryGrid.SaveGrid(sCsvPath, flexFileCommaText, COleVariant(long(TRUE)));
	}
}

/*
void CDATsysView::OnDmmcotrol() 
{
	CDMMDlg dlg;
	dlg.DoModal();	
	
}
*/



void CDATsysView::OnInput() 
{
	CString sCmd = _T("");
	CString sName = _T("");
	CString sCustom = _T("");
	int nByte = 1;
	CString sMsg;

	sName = "INPUT";

	if(!FindRemoconCode(sName, sCustom, sCmd, nByte))
	{
		sMsg.Format("Remocon Code Not Found: %s", sName);
		AddStringToRemoteComLog(sMsg);
	}

	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, CurrentSet->nRemoteType, sCustom, sCmd, nByte);	
}

void CDATsysView::OnAudioOutlStart() 
{
	//if(!g_pView->wOutL_Flag) g_pView->WaveOutL_Start();		
}

void CDATsysView::OnAudioOutlStop() 
{
	//if(g_pView->wOutL_Flag) g_pView->WaveOutL_Stop();
}

void CDATsysView::OnAudioOutrStart() 
{
	//if(!g_pView->wOutR_Flag) g_pView->WaveOutR_Start();	
}

void CDATsysView::OnAudioOutrStop() 
{
	//if(g_pView->wOutR_Flag) g_pView->WaveOutR_Stop();
}

/*
void CDATsysView::OnUsbdioCheckDialog() 
{
	CUsbdioTestDlg dlg;

	dlg.DoModal();
	
}
*/

void CDATsysView:: LogDataSave(CString sLogFilePath)
{
	CString szLogFile = _T("");

	szLogFile = sLogFilePath;
	szLogFile.Replace(".dat" , ".log");

	HANDLE hText = NULL;
	HANDLE hClipData;                            /* handles to clip data  */
	LPSTR  lpClipData;                           /* pointers to clip data */
	HWND hEdit = m_ctrlEditComLog.m_hWnd;

    CStdioFile File;
    CFileException ex;
	
	CString str;
	if (!File.Open(szLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText, &ex)) {
			EmptyClipboard();
			CloseClipboard();
			AddStringToStatus(_T("Unable to save Log list : File Open Error"));
			return;
		}
	
	TRY{
		for (int i = 0; i < m_strLogArray.GetCount(); i++)
		{
			str = m_strLogArray.GetAt(i);
					File.WriteString(str);



		}
		File.Close();
	}
	CATCH(CFileException, e) {
			AddStringToStatus(_T("Unable to save Log list : File Write Error"));
			e->Delete();
			
	}
	END_CATCH
	

	
	//m_ctrlEditComLog.
	//m_ctrlEditComLog.SetSel(0,-1);// SetSel(0, -1, TRUE);
	//m_ctrlEditComLog.Copy();
#if 0
	// Copy all of the text to the clipboard.
	m_ctrlEditComLog.SetSel(0, -1);
	m_ctrlEditComLog.Copy();


	if (OpenClipboard()) {
		if (!(hClipData = GetClipboardData(CF_TEXT))) {
			CloseClipboard();
			return;
		}

		lpClipData =(char*) GlobalLock(hClipData);

		if (!File.Open(szLogFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText, &ex)) {
			EmptyClipboard();
			CloseClipboard();
			AddStringToStatus(_T("Unable to save Log list : File Open Error"));
			return;
		}

		TRY {
			File.WriteString(lpClipData);


			File.Close();
		}

		CATCH (CFileException, e) {
			AddStringToStatus(_T("Unable to save Log list : File Write Error"));
			e->Delete();
		}
		END_CATCH

		EmptyClipboard();
		CloseClipboard();
	}	
#endif
}


BOOL CDATsysView::OnSocketReceive()
{
	CString sReceiveData;
	CString sXmlData;
	CMarkup xmlTemp;
	BOOL bRev = TRUE;
	CString tempId;
	CString sFindElem;
	CString sSetID;

	sReceiveData = _T("");
	if(gGmesCtrl.ReceiveCommString(500, sReceiveData))
	{

		GmesLog_Save(g_pView->m_szExePath, sReceiveData, TRUE);

	//	gGmesCtrl.Paser_ReceiveData(sRevData);
		xmlTemp.SetDoc(sReceiveData);
		xmlTemp.ResetPos();

		while (xmlTemp.FindElem("EIF")) {
			tempId = xmlTemp.GetAttrib("ID");
			sFindElem = xmlTemp.GetSubDoc();
			if (tempId == "S2F3") {
				if(gGmesCtrl.Paser_S2F3(sFindElem))
				{
					sXmlData = gGmesCtrl.MakeElem_S2F4(CurrentSet->sEquipmentID);
					if(gGmesCtrl.SendCommString(sXmlData))
					{
						gGmesCtrl.DisplyComStatus("S2F4", FALSE);
					}
				}
			}
			else if(tempId == "S1F2") {
				gGmesCtrl.Paser_S1F2(sFindElem);
			}
			else if(tempId == "S2F31") {
				if(gGmesCtrl.Paser_S2F31(sFindElem)){
					sXmlData = gGmesCtrl.MakeElem_S2F32(CurrentSet->sEquipmentID, 0);
				}
				else{
					sXmlData = gGmesCtrl.MakeElem_S2F32(CurrentSet->sEquipmentID, 1);
				}

				if(gGmesCtrl.SendCommString(sXmlData))
				{
					gGmesCtrl.DisplyComStatus("S2F31", FALSE);
				}
			}
			else if(tempId == "S6F2") {
				bRev = gGmesCtrl.Paser_S6F2(sFindElem);

				m_bResult_S6F2 = bRev;
				UpdateData(TRUE);
				::SetEvent(m_hReadEvent_S6F2);
			}
			else if(tempId == "S6F12") {
				bRev = gGmesCtrl.Paser_S6F12(sFindElem);

				m_bResult_S6F12 = bRev;
				UpdateData(TRUE);
				::SetEvent(m_hReadEvent_S6F12);
			}
			else if(tempId == "S6F5") {
				if(gGmesCtrl.Paser_S6F5(sFindElem, sSetID)){
					bRev = TRUE;
					sXmlData = gGmesCtrl.MakeElem_S6F6(CurrentSet->sEquipmentID, sSetID, 0);
					
					Display_GMESInfo(TRUE);
				}
				else{
					bRev = FALSE;
					sXmlData = gGmesCtrl.MakeElem_S6F6(CurrentSet->sEquipmentID, sSetID, 1);
					Display_GMESInfo(FALSE);
				}

				m_bResult_S6F5 = bRev;
				UpdateData(TRUE);
				::SetEvent(m_hReadEvent_S6F5);

				if(gGmesCtrl.SendCommString(sXmlData))
				{
					gGmesCtrl.DisplyComStatus("S6F5", FALSE);
				}
			}
			else{
				bRev = FALSE;
			}

		}
	}
	return bRev;

}

BOOL CDATsysView::Check_MacAddress(CString sMac_Add)
{ 
	CString sTmp;
	CString sMsg;
	DWORD dwEventResult = 0;
    clock_t		start;
	MSG msg;

	sTmp = gGmesCtrl.MakeElem_S6F11_2(CurrentSet->sEquipmentID, m_strWipId, sMac_Add);
	::ResetEvent(m_hReadEvent_S6F12);

	if(gGmesCtrl.SendCommString(sTmp))
	{
		gGmesCtrl.DisplyComStatus("S6F11", FALSE);
	}

	start = clock();
	while (TRUE)
	{
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				CurrentSet->bIsRunning = FALSE;
				::PostQuitMessage(0);
				break;
			}
			if (!AfxGetApp()->PreTranslateMessage(&msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}   
		dwEventResult = WaitForSingleObject(m_hReadEvent_S6F12, 500);
			
		if(dwEventResult == WAIT_OBJECT_0 )
		{
			::ResetEvent(m_hReadEvent_S6F12);
			if(!m_bResult_S6F12){
			//	sMsg.Format(_T("GMES Barcode Report Acknowledge Error\nError Code : %s"), gGmesCtrl.m_sErrorMsg);
			//	AfxMessageBox(sMsg);
			//	CurrentSet->bIsRunMsg = FALSE; 
				return FALSE;
			}
			break;
		}
		if ((clock() - start)*1000/CLOCKS_PER_SEC > 10000){ 
			sMsg.Format(_T("GMES S6F12(MAC Address Check) Fail\nTime Out..."));
			AfxMessageBox(sMsg);
		//	Gmes_Connection();
		//	CurrentSet->bIsRunMsg = FALSE; 
			return FALSE;
		}
	}
		
	return TRUE;
}

BOOL CDATsysView::GetToolOption()
{ 
	CString sTmp;
	CString sMsg;
	DWORD dwEventResult = 0;
    clock_t		start;
	MSG msg;
//	int nVal = 0;

//	if(CurrentSet->nToolOptionSetMethod == 0)
//	{
		sTmp = gGmesCtrl.MakeElem_S6F11(CurrentSet->sEquipmentID, m_strWipId);
		::ResetEvent(m_hReadEvent_S6F12);
		::ResetEvent(m_hReadEvent_S6F5);

		if(gGmesCtrl.SendCommString(sTmp))
		{
			gGmesCtrl.DisplyComStatus("S6F11", FALSE);
		}

		start = clock();
		while (TRUE)
		{
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					CurrentSet->bIsRunning = FALSE;
					::PostQuitMessage(0);
					break;
				}
				if (!AfxGetApp()->PreTranslateMessage(&msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}   
			dwEventResult = WaitForSingleObject(m_hReadEvent_S6F12, 500);
				
			if(dwEventResult == WAIT_OBJECT_0 )
			{
				::ResetEvent(m_hReadEvent_S6F12);
				if(!m_bResult_S6F12){
					sMsg.Format(_T("GMES Barcode Report Acknowledge Error\nError Code : %s"), gGmesCtrl.m_sErrorMsg);
					AfxMessageBox(sMsg);
					CurrentSet->bIsRunMsg = FALSE; return FALSE;
				}
				break;
			}
			if ((clock() - start)*1000/CLOCKS_PER_SEC > 10000){ 
				sMsg.Format(_T("GMES S6F12(Barcode Report Acknowledg) Fail\nTime Out..."));
				AfxMessageBox(sMsg);
				Gmes_Connection();
				CurrentSet->bIsRunMsg = FALSE; return FALSE;
			}
		}
		
		start = clock();
		while (TRUE)
		{
			while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					CurrentSet->bIsRunning = FALSE;
					::PostQuitMessage(0);
					break;
				}
				if (!AfxGetApp()->PreTranslateMessage(&msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}   

			dwEventResult = WaitForSingleObject(m_hReadEvent_S6F5, 500);
				
			if(dwEventResult == WAIT_OBJECT_0 )
			{
				::ResetEvent(m_hReadEvent_S6F5);
				if(!m_bResult_S6F5){
					sMsg.Format(_T("GMES Product Data D/L Error\nParsing Error.."));
					AfxMessageBox(sMsg);
					CurrentSet->bIsRunMsg = FALSE; return FALSE;
				}
				break;
			}
			if ((clock() - start)*1000/CLOCKS_PER_SEC > 5000){ 
				sMsg.Format(_T("GMES S6F5(Product Data) D/L Fail\nTime Out..."));
				AfxMessageBox(sMsg);
				Gmes_Connection();
				CurrentSet->bIsRunMsg = FALSE; return FALSE;
			}
		}
		
//	}
	return TRUE;
}

void CDATsysView::UpdateToolOptionInfo()
{
	ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);

	SaveModelIniFile(CurrentSet->sModelIni);
}


void CDATsysView::UpdateVersionInfo() 
{
	UpdateData(TRUE);
	InitVersionGrid();
#if 1
	CString saVersion[15];
	saVersion[0] = CurrentSet->sBE_Version;
	saVersion[1] = CurrentSet->sFE_Version;
	saVersion[2] = CurrentSet->sMicom_Version;
	saVersion[3] = CurrentSet->sDSP_Version;
	saVersion[4] = CurrentSet->sSubDSP_Version;
	saVersion[5] = CurrentSet->sEQ_Version;
	saVersion[6] = CurrentSet->sMEQ_Version;
	saVersion[7] = CurrentSet->sTouch_Version;
	saVersion[8] = CurrentSet->sDemo_Version;
	saVersion[9] = CurrentSet->sDJSound_Version;
	saVersion[10] = CurrentSet->sWirelessTx_Version;
	saVersion[11] = CurrentSet->sWirelessRx_Version;
	saVersion[12] = CurrentSet->sBT_Version;
	saVersion[13] = CurrentSet->sHDMI_Version;
	saVersion[14] = CurrentSet->sChecksum;
	int lCount = 0;
	for (int nRow = 0; nRow < 15; nRow++)
	{
		if (CurrentSet->bVerChecked[nRow] == 1)
		{
			m_CtrlListVersionProcess.SetItem(lCount, 2, LVIF_TEXT, saVersion[nRow], NULL, NULL, NULL, NULL);
			lCount++;
		}
	}
#else
	//m_CtrlListMainProcess.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetStepName(), NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(0, 2, LVIF_TEXT, CurrentSet->sBE_Version, NULL, NULL, NULL, NULL);  //SetTextMatrix(1, 1, CurrentSet->sBE_Version);
//	m_CtrlListVersionProcess.SetItem(0, 2, LVIF_TEXT, "sBE_Version", NULL, NULL, NULL, NULL);  //SetTextMatrix(1, 1, CurrentSet->sBE_Version);
	m_CtrlListVersionProcess.SetItem(1, 2, LVIF_TEXT, CurrentSet->sFE_Version, NULL, NULL, NULL, NULL);
//	m_CtrlListVersionProcess.SetItem(1, 2, LVIF_TEXT, "sFE_Version", NULL, NULL, NULL, NULL);

	
	m_CtrlListVersionProcess.SetItem(2, 2, LVIF_TEXT, CurrentSet->sMicom_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(3, 2, LVIF_TEXT, CurrentSet->sDSP_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(4, 2, LVIF_TEXT, CurrentSet->sSubDSP_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(5, 2, LVIF_TEXT, CurrentSet->sEQ_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(6, 2, LVIF_TEXT, CurrentSet->sMEQ_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(7, 2, LVIF_TEXT, CurrentSet->sTouch_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(8, 2, LVIF_TEXT, CurrentSet->sDemo_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(9, 2, LVIF_TEXT, CurrentSet->sDJSound_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(10, 2, LVIF_TEXT, CurrentSet->sWirelessTx_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(11, 2, LVIF_TEXT, CurrentSet->sWirelessRx_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(12, 2, LVIF_TEXT, CurrentSet->sBT_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(13, 2, LVIF_TEXT, CurrentSet->sHDMI_Version, NULL, NULL, NULL, NULL);
	m_CtrlListVersionProcess.SetItem(14, 2, LVIF_TEXT, CurrentSet->sChecksum, NULL, NULL, NULL, NULL);
#endif
	
}

void CDATsysView::OnJigUp() 
{
	gPciDioCtrl.Jig_Up();
}

void CDATsysView::OnJigupAll() 
{
	CurrentSet->nJigUpType = 1;	
}

void CDATsysView::OnJigupGood() 
{
	CurrentSet->nJigUpType = 0;	
}

void CDATsysView::OnJigupNot() 
{
	CurrentSet->nJigUpType = 2;	
}

void CDATsysView::OnStartAuto() 
{
	CString sTemp;
	
	CurrentSet->nRunStartType = AUTO_START;

	sTemp.LoadString(IDR_MAINFRAME);
	
	if(g_nRunningProcessNo == 1)
	{
		sTemp = sTemp + "1(" + g_pView->m_szVersion + ")-Auto Start";
	}
	else{
		sTemp = sTemp + "2(" + g_pView->m_szVersion + ")-Auto Start";
	}
	AfxGetMainWnd()->SetWindowText(sTemp);
}

void CDATsysView::OnStartManual() 
{
	CString sTemp;
	
	CurrentSet->nRunStartType = MANUAL_START;
	sTemp.LoadString(IDR_MAINFRAME);
	if(g_nRunningProcessNo == 1)
	{
		sTemp = sTemp + "1(" + g_pView->m_szVersion + ")-Manual Start";
	}
	else{
		sTemp = sTemp + "2(" + g_pView->m_szVersion + ")-Manual Start";
	}
	AfxGetMainWnd()->SetWindowText(sTemp);
}

void CDATsysView::OnCheckMeson() 
{
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	if(CurrentSet->bUploadMes){
		CurrentSet->bUploadMes = FALSE;
	}
	else{
		CurrentSet->bUploadMes = TRUE;
	}
	
	m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
}


void CDATsysView::OnStnClickedStepresult()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


LRESULT CDATsysView::OnUpdateGraph(WPARAM wParam, LPARAM lParam)
{
	static int sWaitCount = 0;
	//static int s_InputEnableCount = 0;
	CFFT_SM lfft;
	//	COMPLEX XData[m_Sample_Buffer_Max];
	COMPLEX XData[10000];

	//CNiReal64Matrix Check_data;
	//CNiReal64Matrix Check_data_1;

	int len;
	char lbBuffer[100000];
	short lSBuffer[100000];
	///	double lDbuffer[m_Sample_Buffer_Max];
	double lDbuffer[10000];
	double lDbufferR[10000];
	double lDbufferL[10000];

	double lDbufferDisplayR[2000];
	double lDbufferDisplayL[2000];

	//double lmax = 0, lmin = 1000;
	CString szString;
	CString sTemp;
	len = (int)wParam;


	if (sWaitCount > 0)
		sWaitCount--;

	if (m_InputEnableCount > 10)
	{
		if (m_SignalEnable == 0)
		{
			m_SignalEnable = 1;
		}
	}
	else
	{
		m_InputEnableCount++;
	}

	//	int m_Sample_Byte_Max;
	//int m_Sample_Buffer_Max;
	//int m_Sample_Rate_Per_Second;
	if (len != SAMPLE_BUFFER_MAX * 4)
	{
		len = SAMPLE_BUFFER_MAX * 4;
		return 1;
	}
	memcpy(lbBuffer, (char *)lParam, len);
	lbBuffer[len] = 0;
	memcpy(lSBuffer, lbBuffer, len);
	//5/65535
	//Check_data.SetSize(1, len / 2);
	for (int i = 0; i < SAMPLE_BUFFER_MAX; i++)
	{
		lDbuffer[i] = (double)lSBuffer[i*2]/ 3277.;
		//lDbuffer[i] = lDbuffer[i] / 3277.;//65536;
		lDbufferL[i] = (double)lSBuffer[i * 2] / 3277.;
		lDbufferR[i] = (double)lSBuffer[i * 2+1] / 3277.;
		
	}

	double lFreq_Left = 0;
	double lVolume_Left = 0;
	double lFreq_Right = 0;
	double lVolume_Right = 0;

	static double sFreq_Left = 0;
	static double sVolume_Left = 0;
	static double sFreq_Right = 0;
	static double sVolume_Right = 0;
	static int sDisplayCount = 0;

	double lFreq_Left_Sum = 0;
	double lFreq_Right_Sum = 0;
	double lFreq_Left_Avg = 0;
	double lFreq_Right_Avg = 0;


	static double sFreq_Left_Array[30];
	static double sFreq_Right_Array[30];

	double lAvgDbufferR[10000];
	double lAvgDbufferL[10000];
	double lSumDbufferR = 0;
	double lSumDbufferL = 0;

#if 0

	for (int i = 0; i < 2048; i++)
	{
		
		lAvgDbufferR[i] = lDbufferR[i];
		lAvgDbufferL[i] = lDbufferL[i];

	}
#else

	for (int i = 0; i < 9; i++)
	{
		lSumDbufferR += lDbufferR[i];
		lSumDbufferL += lDbufferL[i];
	}

	for (int i = 0; i < 2038; i++)
	{
		lSumDbufferR += lDbufferR[i+10];
		lSumDbufferL += lDbufferL[i+10];
		lAvgDbufferR[i] = lSumDbufferR / 10.0;
		lAvgDbufferL[i] = lSumDbufferL / 10.0;

		lSumDbufferR -= lDbufferR[i];
		lSumDbufferL -= lDbufferL[i];
	}
	
#endif
#if 1

	lfft.CheckData(lAvgDbufferR, lDbufferDisplayR, &lFreq_Right, &lVolume_Right);
	lfft.CheckData(lAvgDbufferL, lDbufferDisplayL, &lFreq_Left, &lVolume_Left);


	

#else
	//lfft.CheckData(lDbufferR, lDbufferDisplayR, &lFreq_Right, &lVolume_Right);
	//lfft.CheckData(lDbufferL, lDbufferDisplayL, &lFreq_Left, &lVolume_Left);
	//
	//
	//for (int i = 0; i < 1000; i++)
	//{
	//	m_PlotDataR[i].Data = lDbufferDisplayR[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//	m_PlotDataL[i].Data = lDbufferDisplayL[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//}
#endif
	
	



	for (int i = 0; i < 9; i++)
	{
		sFreq_Left_Array[i] = sFreq_Left_Array[i + 1];
		lFreq_Left_Sum += sFreq_Left_Array[i];

	}
	sFreq_Left_Array[9] = lFreq_Left;
	lFreq_Left_Sum += sFreq_Left_Array[9];
	lFreq_Left_Avg = lFreq_Left_Sum / 10.0;


	for (int i = 0; i < 9; i++)
	{
		sFreq_Right_Array[i] = sFreq_Right_Array[i + 1];
		lFreq_Right_Sum += sFreq_Right_Array[i];

	}
	sFreq_Right_Array[9] = lFreq_Right;
	lFreq_Right_Sum += sFreq_Right_Array[9];
	lFreq_Right_Avg = lFreq_Right_Sum / 10.0;


	if(lVolume_Left < 20)
		lFreq_Left = 0;

	if(lVolume_Right < 20)
		lFreq_Right = 0;

	m_Frequency_Right = lFreq_Right;
	m_Frequency_Left = lFreq_Left;
#ifdef __NEW_WAVE_DISPLAY	
	m_Voltage_Left = lVolume_Left  *2.5;
	m_Voltage_Right = lVolume_Right  *2.5;
#else
	m_Voltage_Left = lVolume_Left;
	m_Voltage_Right = lVolume_Right;
#endif
	 


	 double lDisplayAdjustDbufferR[10000];
	 double lDisplayAdjustDbufferL[10000];

	 memcpy(lDisplayAdjustDbufferR, lDbufferDisplayR, sizeof(PlotData_T) * 1000);
	 memcpy(lDisplayAdjustDbufferL, lDbufferDisplayL, sizeof(PlotData_T) * 1000);
#if __NEW_WAVE_DISPLAY
	 for (int i = 0; i < 1000; i++)
	 {

		 if (abs(m_Voltage_Left) >= 100)
		 {			
			 m_PlotDataL[i].Data = (0.9 + 100 / abs(m_Voltage_Left) * 9) * lDisplayAdjustDbufferL[i] / (5);
		 }
		 else if (abs(m_Voltage_Left) >= 30)
		 {			
			 m_PlotDataL[i].Data = ( abs(m_Voltage_Left) / 100 * 10) * lDisplayAdjustDbufferL[i] / (5);
		 }
		 else
		 {
			 m_PlotDataL[i].Data =  lDisplayAdjustDbufferL[i] / (5);
		 }

		 if (abs(m_Voltage_Right) >= 100)
		 {
			 m_PlotDataR[i].Data = (0.9 + 100 / abs(m_Voltage_Right) * 9) * lDisplayAdjustDbufferR[i] / (5);
			
		 }
		 else if (abs(m_Voltage_Right) >= 30)
		 {
			 m_PlotDataR[i].Data = ( abs(m_Voltage_Right) / 100 * 9) * lDisplayAdjustDbufferR[i] / (5);
		 }
		 else
		 {
			 m_PlotDataR[i].Data =  lDisplayAdjustDbufferR[i] / (5);
		 }
		 //else
		 //{
		 //	lDisplayAdjustDbufferL[i] = lDisplayAdjustDbufferL[i];
		 //	
		 //}

		 ///if(lDbufferDisplayR[i] == 0)
	 //	m_PlotDataR[i].Data = lDbufferDisplayR[i] / (10); //(0.9 + 0.5 / lDbufferDisplayR[i] * 9) * lDbufferDisplayR[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	 //	m_PlotDataL[i].Data = lDbufferDisplayL[i] / (10); //(0.9 + 0.5 / lDbufferDisplayL[i] * 9) * lDbufferDisplayL[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	 }
	 //memcpy(lDisplayAdjustDbufferR, lDbufferDisplayR, sizeof(PlotData_T) * 1000);
	 //memcpy(lDisplayAdjustDbufferL, lDbufferDisplayL, sizeof(PlotData_T) * 1000);
	 ////=(0.9 + 50/E11*9)*E11  =(0.92 + 100/E7*4)*E7 50 => 5000, 
#else
	 for (int i = 0; i < 1000; i++)
	 {
		 m_PlotDataR[i].Data = lDisplayAdjustDbufferR[i] / 10; //(0.9 + 0.5 / lDbufferDisplayR[i] * 9) * lDbufferDisplayR[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
		 m_PlotDataL[i].Data = lDisplayAdjustDbufferL[i] / 10; //(0.9 + 0.5 / lDbufferDisplayL[i] * 9) * lDbufferDisplayL[i]/ (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	 }
#endif
	 if (m_Sound_Display_Enable == 1)
	 {
		 memcpy(m_poglWindow->m_PlotDataL, m_PlotDataL, sizeof(PlotData_T) * 1000);
		 memcpy(m_poglWindow->m_PlotDataR, m_PlotDataR, sizeof(PlotData_T) * 1000);
		 memcpy(m_oglWindow.m_PlotDataL, m_PlotDataL, sizeof(PlotData_T) * 1000);
		 memcpy(m_oglWindow.m_PlotDataR, m_PlotDataR, sizeof(PlotData_T) * 1000);

	 }





	 static int sAudioMeasureCount = 0;
	 if (m_RunAudioMeasure == 1)
	 {
		 sAudioMeasureCount++;
		 if (sAudioMeasureCount < 3)
		 {
			 if(AudioMeasure() == 1)
				 m_RunAudioMeasure = 0;
		 }
		 else
		 {

			 m_RunAudioMeasure = 0;
		 }
	 }
	 else
	 {
		 sAudioMeasureCount = 0;
	 }

	sDisplayCount++;
	if (sDisplayCount > 10)
	{
		sDisplayCount = 0;
		if (sFreq_Left != m_Frequency_Left)
		{
			sFreq_Left = m_Frequency_Left;
			sTemp.Format(_T(" %4.0f Hz "), m_Frequency_Left);
			m_cLabelSoundLeftFREQ.SetWindowText(sTemp);
		}

		if (sVolume_Left != m_Voltage_Left)
		{
			sVolume_Left = m_Voltage_Left;
			sTemp.Format(_T(" %4.0f mV "), m_Voltage_Left);
			m_cLabelSoundLeftVolt.SetWindowText(sTemp);

		}

		if (sFreq_Right != m_Frequency_Right)
		{
			sFreq_Right = m_Frequency_Right;

			sTemp.Format(_T(" %4.0f Hz "), m_Frequency_Right);
			m_cLabelSoundRightFREQ.SetWindowText(sTemp);
		}

		if (sVolume_Right != m_Voltage_Right)
		{
			sVolume_Right = m_Voltage_Right;

			sTemp.Format(_T(" %4.0f mV "), m_Voltage_Right);
			m_cLabelSoundRightVolt.SetWindowText(sTemp);

		}
	}



//	lmax = ((int)(lmax * 100)) / 100.0 + 0.1;
	//m_GraphMainWave.Axes.Item("YAxis-1").Maximum = lmax;
	//m_GraphMainWave.Axes.Item("YAxis-1").Minimum = -lmax;

	//m_GraphMainWave.Plots.RemoveAll();


	//for (unsigned int i = 0; i < 8; i++)
	//{
	//	m_GraphMainWave.Plots.Add();
	//	m_GraphMainWave.Plots.Item(i + 1).LineColor = m_colors[0];
	//}

	//m_GraphMainWave.PlotY(Check_data);



	double lFreq = 0;
	double lVolume = 0;
	int lReturn = lfft.CheckFreqVol(lDbuffer, &lFreq, &lVolume);

	CString sTmp;
	sTmp.Format(_T("Freq: %4.2f, Vol: %4.2f  "), lFreq, lVolume);
//	SetDlgItemText(IDC_STATIC_FREQ_VOL, sTmp);


////////////////////////////////////
////////FFT

	for (int i = 0; i < SAMPLE_BUFFER_MAX; i++)
	{
		XData[i].real = lDbuffer[i];
		//Check_data(0,i) = lDbuffer[i];
	}
	int m = lfft.log2(SAMPLE_BUFFER_MAX);
	int fft_length;
	fft_length = 1 << m;   //bit shift
	lfft.fft(XData, m);
//////////////////////////////////////////////////
	double SpectrumMag[10000];;
	double tempflt;
	double tempMax = 0;
	int MaxPt  = 0;

	double m_Lodata[10000];

	//double m_Lodata[SAMPLE_BUFFER_MAX];
	//SpectrumMag = (double *)calloc(fft_length, sizeof(double));
//	Check_data.SetSize(1, fft_length / 2 - 1);
	//double lDbuffer[10000];
	for (int i = 1; i < fft_length / 2; i++)
	{
		tempflt = XData[i].real * XData[i].real;
		tempflt += XData[i].imag * XData[i].imag;
		tempflt = sqrt(tempflt);
		tempflt = MAX(tempflt, 1.e-14);
		if (tempMax < tempflt)
		{
			MaxPt = i;
		}
		tempMax = MAX(tempflt, tempMax);
		m_Lodata[i] = tempflt;
		SpectrumMag[i] = 10 * log10(tempflt);
		
	}

	


	const int cAvgCount = 20;
	double lSum = 0;
	double lAvgDbuffer[4096];
	for (int i = 1; i < cAvgCount; i++)
	{
		lSum += SpectrumMag[i];
	}

	for (int i = 1; i < fft_length / 2; i++)
	{
		if (i < fft_length / 2 - cAvgCount)
		{
			lSum += SpectrumMag[i + cAvgCount];
			lAvgDbuffer[i - 1] = lSum / cAvgCount;
		}
		else
		{
			lSum += SpectrumMag[fft_length / 2 - 1];
			lAvgDbuffer[i - 1] = lSum / cAvgCount;
		}
		lSum -= SpectrumMag[i];
	}



	///////////////////////////////////////////////////////
	//
	//lAvgDbuffer[0] = Check_data(0, 0);
	//for (int i = 1; i < fft_length / 2; i++)
	//{
	//	lAvgDbuffer[i] = Check_data(0, i - 1);
	//}

	CString strTmp;
	int lRtn;
#if 1
	//for (int i = 0; i < 500; i++)
	//{
	//	//	m_oglWindow.m_PlotData[i].Data = m_Lodata[i] / (1.e+6 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//	m_oglWindow.m_PlotData[i].Data = lDbuffer[i] / (10); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
	//	m_oglWindow.m_PlotData[i].bNG = lfft.m_CheckError[i];
	//	//m_dataBuffer[i] = Check_data(0, i);
	//}
#else
//	if (m_LogDisplayOn != 1)
	{

		for (int i = 0; i < fft_length / 2 - 1; i++)
		{
		//	m_oglWindow.m_PlotData[i].Data = m_Lodata[i] / (1.e+6 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
			m_oglWindow.m_PlotData[i].Data = lDbuffer[i] / (10 ); //m_Lodata[i] / (10000 / 2.0) - 1.0;// (Check_data(0,i) - 50.0) / 50.0;
			m_oglWindow.m_PlotData[i].bNG = lfft.m_CheckError[i];
			//m_dataBuffer[i] = Check_data(0, i);
		}
	}
	//else
	//{
	//	for (int i = 0; i < fft_length / 2 - 1; i++)
	//	{
	//		m_oglWindow.m_PlotData[i].Data = (Check_data(0, i) - 50.0) / 50.0;
	//		m_oglWindow.m_PlotData[i].bNG = lfft.m_CheckError[i];
	//		//m_dataBuffer[i] = Check_data(0, i);
	//	}
	//}

#endif

	return 1;
}


BOOL CDATsysView::AudioMeasure()
{
	CString sTemp;
	double freq, amp, dbamp;
	double nCorrectionValue = 0.97;
	double dLevel_L = 0;
	double dLevel_R = 0;

	int aResult[4] = { 0,0,0,0 };

	
	

	m_fMeasuredLeftFreq = m_Frequency_Left;;
	m_fMeasuredRightFreq = m_Frequency_Right;;

	m_fMeasuredLeftLevel = m_Voltage_Left;
	m_fMeasuredRightLevel = m_Voltage_Right;


	float fLowerMargin = (float)(1 - (m_fFreqMargin*0.01));
	float fUpperMargin = (float)(1 + (m_fFreqMargin*0.01));

	float fLevelLowerMargin = (float)(1 - (m_fLevelMargin * 0.01));
	float fLevelUpperMargin = (float)(1 + (m_fLevelMargin * 0.01));

	//m_fMeasuredLeftFreq = 0;
	//m_fMeasuredRightFreq = 0;
	//m_fMeasuredLeftLevel = 0;
	//m_fMeasuredRightLevel = 0;

	//	m_fLeftFreqLowerLimit	= m_fTargetLeftFreq * fLowerMargin;
	//	m_fLeftFreqUpperLimit	= m_fTargetLeftFreq * fUpperMargin;

	//	m_fRightFreqLowerLimit	= m_fTargetRightFreq * fLowerMargin;
	//	m_fRightFreqUpperLimit	= m_fTargetRightFreq * fUpperMargin;

	m_fLeftFreqLowerLimit = m_fTargetLeftFreq - m_fFreqMargin;
	m_fLeftFreqUpperLimit = m_fTargetLeftFreq + m_fFreqMargin;

	m_fRightFreqLowerLimit = m_fTargetRightFreq - m_fFreqMargin;
	m_fRightFreqUpperLimit = m_fTargetRightFreq + m_fFreqMargin;


	if (m_bMeasureAudioType == LEVEL_CHECK)
	{
		m_fLeftLevelLowerLimit = m_fTargetLeftLevel;

		m_fRightLevelLowerLimit = m_fTargetRightLevel;

	}
	else {
		//	m_fLeftLevelLowerLimit	= m_fTargetLeftLevel * fLevelLowerMargin;
		//	m_fRightLevelLowerLimit	= m_fTargetRightLevel * fLevelLowerMargin;
		m_fLeftLevelLowerLimit = m_fTargetLeftLevel - m_fLevelMargin;
		m_fRightLevelLowerLimit = m_fTargetRightLevel - m_fLevelMargin;
	}
	//	m_fLeftLevelUpperLimit  = m_fTargetLeftLevel * fLevelUpperMargin;
	//	m_fRightLevelUpperLimit	= m_fTargetRightLevel * fLevelUpperMargin;
	m_fLeftLevelUpperLimit = m_fTargetLeftLevel + m_fLevelMargin;
	m_fRightLevelUpperLimit = m_fTargetRightLevel + m_fLevelMargin;

	//if (!wIn_Flag) { WaveRead_Start(); _Wait(100); }

	//EnterCriticalSection(&g_cs);
	////g_CriticalSection.Lock();

	//// LEFT
	//ctrlPeakDetect.Invalidate();
	//ctrlSpectrum_L.Invalidate();
	//GetWavePeak_L(&freq, &amp, &dbamp);
	//GetPeackDetectorPeak_L(&dLevel_L);

	//m_fMeasuredLeftFreq = (float)freq;
	//m_fMeasuredLeftLevel = (float)(dLevel_L / (2 * 1.4142)*0.1*nCorrectionValue);
	//// RIGHT
	////ctrlSpectrum_R.Invalidate();
	////GetWavePeak_R(&freq, &amp, &dbamp);
	////GetPeackDetectorPeak_R(&dLevel_R);

	//m_fMeasuredRightFreq = (float)freq;
	//m_fMeasuredRightLevel = (float)(dLevel_R / (2 * 1.4142)*0.1*nCorrectionValue);

	LeaveCriticalSection(&g_cs);
	//g_CriticalSection.Unlock(); 

	switch (m_bMeasureAudioType)
	{
	case FREQ_LEVEL_CHECK:
		sTemp.Format(_T("F:%d,%d/ L:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq, (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
		if ((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)) {
			aResult[0] = TEST_PASS;
		}
		else {
			aResult[0] = TEST_FAIL;
		}

		if ((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit) && (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
			aResult[1] = TEST_PASS;
		}
		else {
			aResult[1] = TEST_FAIL;
		}

		//			if(m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
		if ((m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) && (m_fMeasuredLeftLevel <= m_fLeftLevelUpperLimit)) {
			aResult[2] = TEST_PASS;
		}
		else {
			aResult[2] = TEST_FAIL;
		}

		//			if(m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
		if ((m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) && (m_fMeasuredRightLevel <= m_fRightLevelUpperLimit)) {
			aResult[3] = TEST_PASS;
		}
		else {
			aResult[3] = TEST_FAIL;
		}

		if ((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS) && (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
		{
			m_bAudio_Result = TRUE;
		}
		else {
			m_bAudio_Result = FALSE;
		}
		break;

	case FREQUENCY_CHECK:
		sTemp.Format(_T("F:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq);
		if ((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)) {
			aResult[0] = TEST_PASS;
		}
		else {
			aResult[0] = TEST_FAIL;
		}

		if ((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit) && (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
			aResult[1] = TEST_PASS;
		}
		else {
			aResult[1] = TEST_FAIL;
		}

		if ((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS))
		{
			m_bAudio_Result = TRUE;
		}
		else {
			m_bAudio_Result = FALSE;
		}
		break;

	case LEVEL_CHECK:
		sTemp.Format(_T("L:%d,%d"), (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
		if (m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
			//			if((m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit)	&& (m_fMeasuredLeftLevel <= m_fLeftLevelUpperLimit)) {
			aResult[2] = TEST_PASS;
		}
		else {
			aResult[2] = TEST_FAIL;
		}

		if (m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
			//			if((m_fMeasuredRightLevel >= m_fRightLevelLowerLimit)	&& (m_fMeasuredRightLevel <= m_fRightLevelUpperLimit)) {
			aResult[3] = TEST_PASS;
		}
		else {
			aResult[3] = TEST_FAIL;
		}

		if ((aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
		{
			m_bAudio_Result = TRUE;
		}
		else {
			m_bAudio_Result = FALSE;
		}
		break;
	}
	ctrlStepMeasur_A.SetWindowText(sTemp);
	ctrlStepMeasur_A.Invalidate();
	ctrlStepMeasur_A.UpdateWindow();

	return m_bAudio_Result;
}
//
//void CDATsysView::OnSelchangeCmbSoudDevice()
//{
//	CWinApp* pApp = AfxGetApp();
//	int lDeviceID = m_cComboSoundLeft.GetCurSel();
//	if (CurrentSet->nSoundInDeviceID != lDeviceID)
//	{
//		CurrentSet->nSoundInDeviceID = lDeviceID;
//		m_sm_sound_ctrl.StopCapture();
//		m_sm_sound_ctrl.StartCapture(this, CurrentSet->nSoundInDeviceID);
//
//		pApp->WriteProfileInt(_T("Config"), _T("Sound Device ID"), CurrentSet->nSoundInDeviceID);
//	}
//
//}
void CDATsysView::SelchangeSoudDevice(int lDeviceID)
{
	CWinApp* pApp = AfxGetApp();
	//int lDeviceID = m_cComboSoundLeft.GetCurSel();
	if (CurrentSet->nSoundInDeviceID != lDeviceID)
	{
		CurrentSet->nSoundInDeviceID = lDeviceID;
		m_sm_sound_ctrl.StopCapture();
		m_sm_sound_ctrl.StartCapture(this, CurrentSet->nSoundInDeviceID);

		pApp->WriteProfileInt(_T("Config"), _T("Sound Device ID"), CurrentSet->nSoundInDeviceID);
	}

}
void CDATsysView::OnNMCustomdrawListMainProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;


	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	CString strCol = _T("");



	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}
	case CDDS_ITEMPREPAINT:
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		return;
	}
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
	{
		COLORREF crText, crBkgnd;

		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
		//	nRow= m_CtrlListMainProcess.GetItemCount();

		int l_nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
		int l_nSubItem = (int)(pLVCD->iSubItem);

		//if ((m_CurrentStep > 0) && (m_CurrentStep < 200)&&(l_nItem == m_CurrentStep-1)&&(l_nSubItem < 2))
		//{
		//	crText = RGB(0, 0, 0);		//글자색
		//	crBkgnd = RGB(128, 128, 255);	//배경색으로 한다   
		//}
		//else if ((l_nItem >= 0) && (l_nItem < 200) && (l_nSubItem >= 0) && (l_nSubItem < 20))
		//{
		//	crText = m_CtrlListMainProcessEx.nSelForeColor[l_nItem][l_nSubItem];// RGB(0, 0, 0); //글자색
		//	crBkgnd = m_CtrlListMainProcessEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
		//}
		
		if(l_nSubItem == 2)
		{
			strCol = m_CtrlListMainProcess.GetItemText(l_nItem, l_nSubItem);
			if (strCol.Find("OK") >= 0)
			{
				crText = RGB(255, 255, 255); //글자색
				crBkgnd = RGB(0, 0, 255); //배경색으로 한다   
			}
			else if (strCol.Find("NG") >= 0)
			{
				crText = RGB(255, 255, 255); //글자색
				crBkgnd = RGB(255, 0, 0); //배경색으로 한다 
			}
			else if(l_nItem == m_CurrentStep - 1)
			{
				crText = RGB(0, 0, 0); //글자색
				crBkgnd = RGB(255, 255, 0); //배경색으로 한다 


			}
			else
			{
				crText = RGB(0, 0, 0); //글자색
				crBkgnd = RGB(230, 200, 255);//배경색으로 한다   
			}
		}
		else
		{
			if (l_nItem == m_CurrentStep - 1)
			{
				crText = RGB(0, 0, 0); //글자색
				crBkgnd = RGB(255, 255, 0); //배경색으로 한다 


			}
			else
			{
				crText = RGB(0, 0, 0); //글자색
				crBkgnd =  RGB(230, 200, 255);//배경색으로 한다   
			}
		}


		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;


		*pResult = CDRF_DODEFAULT;
		return;
	}
	}

	*pResult = 0;
}


void CDATsysView::OnHdnItemclickListMainProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_bSeq_Lock == 0)
	{
		int lint = phdr->iItem;
		int i, lcnt;
		if (lint == 0)
		{
			if (mCheckAll)
			{
				mCheckAll = 0;
				lcnt = m_CtrlListMainProcess.GetItemCount();
				for (i = 0; i < lcnt; i++)
				{
					m_CtrlListMainProcess.SetCheck(i, 0);
					m_LockStatus[i] = 0;
				}
			}
			else
			{
				mCheckAll = 1;
				lcnt = m_CtrlListMainProcess.GetItemCount();
				for (i = 0; i < lcnt; i++)
				{
					m_CtrlListMainProcess.SetCheck(i, 1);
					m_LockStatus[i] = 1;
				}
			}
		}
	}
	SetTimer(TIMER_CHECK_LIST_UPDATE, 100, NULL);
	*pResult = 0;
}


void CDATsysView::OnNMCustomdrawListVersionProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;


	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	CString strCol = _T("");



	switch (pLVCD->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			return;
		}
		case CDDS_ITEMPREPAINT:
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		{
			COLORREF crText, crBkgnd;

			//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
			//	nRow= m_CtrlListMainProcess.GetItemCount();

			int l_nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
			int l_nSubItem = (int)(pLVCD->iSubItem);

			//if ((m_CurrentStep > 0) && (m_CurrentStep < 200)&&(l_nItem == m_CurrentStep-1)&&(l_nSubItem < 2))
			//{
			//	crText = RGB(0, 0, 0);		//글자색
			//	crBkgnd = RGB(128, 128, 255);	//배경색으로 한다   
			//}
			//else if ((l_nItem >= 0) && (l_nItem < 200) && (l_nSubItem >= 0) && (l_nSubItem < 20))
			//{
			//	crText = m_CtrlListMainProcessEx.nSelForeColor[l_nItem][l_nSubItem];// RGB(0, 0, 0); //글자색
			//	crBkgnd = m_CtrlListMainProcessEx.nSelBackColor[l_nItem][l_nSubItem];// = RGB(255, 0, 0); //배경색으로 한다   
			//}

			//if (l_nSubItem == 2)
			//{
			//	strCol = m_CtrlListMainProcess.GetItemText(l_nItem, l_nSubItem);
			//	if (strCol.Find("OK") >= 0)
			//	{
			//		crText = RGB(255, 255, 255); //글자색
			//		crBkgnd = RGB(0, 0, 255); //배경색으로 한다   
			//	}
			//	else if (strCol.Find("NG") >= 0)
			//	{
			//		crText = RGB(255, 255, 255); //글자색
			//		crBkgnd = RGB(255, 0, 0); //배경색으로 한다 
			//	}
			//	else if (l_nItem == m_CurrentStep - 1)
			//	{
			//		crText = RGB(0, 0, 0); //글자색
			//		crBkgnd = RGB(255, 255, 0); //배경색으로 한다 


			//	}
			//	else
			//	{
			//		crText = RGB(0, 0, 0); //글자색
			//		crBkgnd = RGB(255, 200, 200);//배경색으로 한다   
			//	}
			//}
			//else
			//{
			//	if (l_nItem == m_CurrentStep - 1)
			//	{
			//		crText = RGB(0, 0, 0); //글자색
			//		crBkgnd = RGB(255, 255, 0); //배경색으로 한다 


			//	}
			//	else
			//	{
			//		crText = RGB(0, 0, 0); //글자색
			//		crBkgnd = RGB(255, 200, 200);//배경색으로 한다   
			//	}
			//}
						
			crText = RGB(0, 0, 0); //글자색			
			crBkgnd = RGB(230, 200, 255);//배경색으로 한다   

			pLVCD->clrText = crText;
			pLVCD->clrTextBk = crBkgnd;


			*pResult = CDRF_DODEFAULT;
			return;
		}
	}

	*pResult = 0;

}


void CDATsysView::OnNMClickListMainProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	int l_nItem;
	//int l_nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
	//int l_nSubItem = (int)(pLVCD->iSubItem);
	l_nItem = phdr->iItem;
	int l_check;
	l_check = m_CtrlListMainProcess.GetCheck(l_nItem);
	
	if (m_bSeq_Lock == 1)
	{
		if(l_check)
			m_CtrlListMainProcess.SetCheck(l_nItem,0);
		else
			m_CtrlListMainProcess.SetCheck(l_nItem, 1);

	}
	else
	{
		if (m_LockStatus[l_nItem] == 1)
		{
			m_LockStatus[l_nItem] = 0;
		}
		else
		{
			m_LockStatus[l_nItem] = 1;
		}

	}
	SetTimer(TIMER_CHECK_LIST_UPDATE, 100, NULL);
	
	*pResult = 0;
}

void CDATsysView::UpdateSeqCheck()
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int l_ListCount = StepList.GetCount();
	for (int i = 0; i < m_CtrlListMainProcess.GetItemCount(); i++)
	{
		int l_check;
		l_check = m_CtrlListMainProcess.GetCheck(i);
		if (m_LockStatus[i] == 1)
		{
			if (l_check == 0)
			{
				m_CtrlListMainProcess.SetCheck(i, 1);
			}
		}
		else
		{
			if (l_check == 1)
			{
				m_CtrlListMainProcess.SetCheck(i, 0);
			}
		}
	}
	//pCurStep->m_bTest
	//for (int nTmp = 0; nTmp <= StepList.GetCount(); nTmp++)
	//{
	//		if(pCurStep->m_bTest)
	//			m_CtrlListMainProcess.SetCheck(l_nItem,0);
	//		else
	//			m_CtrlListMainProcess.SetCheck(l_nItem, 1);

	//	}
	//	
	//}
	//*pResult = 0;
}


void CDATsysView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnLButtonDown(nFlags, point);
}


void CDATsysView::OnNMDblclkListMainProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	SetTimer(TIMER_CHECK_LIST_UPDATE, 100, NULL);
	*pResult = 0;


}

//
//void CDATsysView::OnCbnEditchangeComboSoundL()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	int Sel = m_cComboSoundLeft.GetCurSel();
//	//m_cComboSoundLeft.SetText(m_sm_sound_ctrl.m_aStrSoundDevice.GetAt(Sel));
//	SetDlgItemText(IDC_COMBO_SOUND_L, m_sm_sound_ctrl.m_aStrSoundDevice.GetAt(Sel));
//}
