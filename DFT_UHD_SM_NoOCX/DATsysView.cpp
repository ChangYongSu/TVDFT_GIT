// DATsysView.cpp : implementation of the CDATsysView class
//

#include <Afxmt.h>
#include <malloc.h>
#include <conio.h>
#include <direct.h>
#include <assert.h>

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
#include "CurrentSetting.h"
#include "NoDifferentPointDlg.h"
//#include "joystimm.h"
//#include "thread.h"

//#include "MesDb.h"
#include "Gmes.h"
#include "PLC_Ctrl.h"
#include "Server_Ctrl.h"

#include <mmsystem.h>
#include "UserMsgDlg.h"
#include "SoundCard.h"
#include "RealMeas.h"
#include "I2cAdcCtrl.h"
#include "AvConvertorCtrl.h"

//+add kwmoon 081024
#include "SelectChassisModelDlg.h"

//+add psh 090120
#include "ModelInfoCreate.h"
//+add psh 090910
#include "Keydll1.h"//Include this for functions in the DLL.
#include "Keydll2.h"//Include this for functions in the DLL.
#include "Keydll3.h"//Include this for functions in the DLL.

//#include "Dmm.h"
#include "DmmDlg.h"
#include "VfMeasureDlg.h"
#include "LnbDetectorCtl.h"
#include "Markup.h"
#include "JigCtl.h"
#include "StepMeasFunc.h"
#include "VF1000.h"

#include "IcmpEcho.h"

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

//CVSFlexGrid		ctrlSwitchBoxComLogGrid;
//CVSFlexGrid		ctrlRemoteComLogGrid;
//CVSFlexGrid		ctrlSysInfoGrid;
//CVSFlexGrid		ctrlSummaryGrid;
CColorStaticST	ctrlTestStep;
CColorStaticST  ctrlTestSubStep;
CColorStaticST	ctrlStepTarget_V;
CColorStaticST  ctrlStepMeasur_V;
CColorStaticST	ctrlStepTarget_A;
CColorStaticST  ctrlStepMeasur_A;
CColorStaticST  ctrlPLC_COM_Status;//IDC_PLC_COM_STATUS
CColorStaticST  ctrlROBOT_Status;//IDC_ROBOT_STATUS

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
int				g_nNoFailedStep_EM;
int				g_nStepSize;
int				g_nStepSize_EM;
int				g_nStepPlusCount;


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
clock_t			StepStart_EM;

CImageProc		g_ImageProc;
CBaseImage		g_GrabDisplayImage;
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
CString			m_strWipIdSave;			//Wip ID
CTime			m_tStart;			//Test Start Time

CStringArray				g_strLogArray;
int g_Using = 0;

//+add kwmoon 080514
CSoundCard		g_SoundCard;


//CEdit			ctrlAudioLTitle;			
//CEdit			ctrlAudioRTitle;			
//CEdit			ctrlAudioLLevel;			
//CEdit			ctrlAudioLFreq;			
//CEdit			ctrlAudioRLevel;			
//CEdit			ctrlAudioRFreq;			


CCurrentSetting	g_CurSetting;

HWND			g_hCommWnd;
//BYTE			GrabData[2560][1080][3];

UINT			g_nRunningProcessNo;
UINT			g_nGrabberType;
UINT			g_nSysType;
UINT			g_nSysRobotType;
UINT			g_nUseNoScanType;
//FHD
HANDLE			hClientEvents[12]; 
HANDLE			hServerEvents[9]; 
//
//AUTO
HANDLE			hBarcodeClientEvents[6]; 
HANDLE			hBarcodeServerEvents[6]; 
//
HANDLE			hUsbCtrl; 
HANDLE			hMhlCtrl; 
HANDLE			hUsbCtrl_Complete; 
HANDLE			hMhlCtrl_Complete; 

//UHD
HANDLE			hGrabEvent; 
HANDLE			hGrabEvent_Complete; 
//


CString			g_WndClassName;
//CCriticalSection g_CriticalSection;
CRITICAL_SECTION g_cs;  //pshÃß°¡
CRITICAL_SECTION g_cs2;  //add 090818
CRITICAL_SECTION g_cs3;  
CRITICAL_SECTION g_cs4;  


CStringArray g_Divisions;

CString			g_sData_MD5;

///////////////////////////////////
//FORCED INPUT CHECK
int g_ForcedInputCheck = 0;
int g_ID_Check = 0;

///////////////////////////


////////////////////////
// External Variables //
////////////////////////

extern _InputSource InputDefined_FHD[];
extern _InputSource InputDefined_UHD[];
extern UINT	g_GrabSource;
extern int	g_AutoVideoRetry;

//+add 090409(Modification No1)
extern int g_nCurSwitchBoxLogLine;
extern int g_nCurRemoteLogLine;

extern CTVCommCtrl TVCommCtrl;

CString gStrTmpLog;

 int g_nResetGrabMode = 0;

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
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_PWR_ON, OnPwrOn)
	ON_COMMAND(ID_REF_IMAGE, OnRefImage)
	ON_COMMAND(ID_GRAB_START, OnGrabStart)
	ON_COMMAND(ID_GRAB_STOP, OnGrabStop)
	ON_COMMAND(ID_ENTER, OnEnter)
	ON_COMMAND(ID_EXIT, OnExit)
	ON_BN_CLICKED(IDC_INPUT_LOCK, OnInputLock)
	ON_COMMAND(ID_MES_DATA_UPLOAD, OnMesDataUpload)
	ON_COMMAND(ID_MODEL_REGISTRATION, OnModelRegistration)
	ON_COMMAND(ID_AUDIO_MEAS_START, OnAudioMeasStart)
	ON_COMMAND(ID_AUDIO_MEAS_STOP, OnAudioMeasStop)
	ON_COMMAND(ID_SCANNER_ENABLE, OnScannerEnable)
	ON_NOTIFY(TCN_SELCHANGE, IDC_VIEW_TAB, OnSelchangeViewTab)
	ON_BN_CLICKED(IDC_SEQ_LOCK, OnSeqLock)
	ON_WM_CLOSE()
	ON_COMMAND(ID_DMMCOTROL, OnDmmcotrol)
	ON_COMMAND(ID_IMG_ROTATE, OnImgRotate)
	ON_BN_CLICKED(IDC_BTN_GET_OPTION, OnBtnGetOption)
	ON_COMMAND_RANGE(IDC_RDO_TOOL_OPTION1,IDC_RDO_TOOL_OPTION3, OnRdoToolOption1)
	ON_BN_CLICKED(IDC_CHECK_MESON, OnCheckMeson)
	ON_MESSAGE(WM_KEYSTROKE, ProcessKeyPad)
	ON_COMMAND(ID_JIGDOWN_ALL, OnJigdownAll)
	ON_COMMAND(ID_JIGDOWN_GOOD, OnJigdownGood)
	ON_COMMAND(ID_JIGDOWN_NOT, OnJigdownNot)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
//	ON_BN_CLICKED(IDC_BTN_CHECK_CRC, OnBtnCheckCrc)
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
	ON_MESSAGE(UM_CONTINUOUS_LVDS_GRAB, RunContinuousLVDSGrab_FHD)
	ON_MESSAGE(WM_COMM_READ , OnCommunication)
	ON_MESSAGE(UM_UPLOAD_MES_DATA,OnUploadMesData)
	ON_MESSAGE(UM_CLOSE_RESULT_DLG, CloseResultDlg)
	ON_MESSAGE(UM_UPDATE_GRAB_STATUS, UpdateGrabImage)
	ON_MESSAGE(WM_MASSAGE_LOG_DISPLAY, OnUpdateLog)
	
	ON_COMMAND(ID_ACPOWER_FAIL, &CDATsysView::OnAcpowerFail)
	ON_COMMAND(ID_ACPOWER_GOOD, &CDATsysView::OnAcpowerGood)
	ON_COMMAND(ID_ACPOWER_ALL, &CDATsysView::OnAcpowerAll)
	ON_COMMAND(ID_ACPOWER_NOT, &CDATsysView::OnAcpowerNot)
	ON_COMMAND(ID_ANALYSE_VF, &CDATsysView::OnAnalyseVf)
	ON_COMMAND(ID_VF_CONTROL, &CDATsysView::OnVfControl)
	ON_COMMAND(ID_GRABRESET_AUTO, &CDATsysView::OnGrabresetAuto)
	ON_UPDATE_COMMAND_UI(ID_GRABRESET_AUTO, &CDATsysView::OnUpdateGrabresetAuto)
	ON_COMMAND(ID_GRABRESET_MANUAL, &CDATsysView::OnGrabresetManual)
	ON_UPDATE_COMMAND_UI(ID_GRABRESET_MANUAL, &CDATsysView::OnUpdateGrabresetManual)
	ON_STN_CLICKED(IDC_ROBOT_STATUS, &CDATsysView::OnStnClickedRobotStatus)
	ON_WM_RBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON_AUTO_ROBOT_RESET, &CDATsysView::OnBnClickedButtonAutoRobotReset)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_ROBOT_STOP, &CDATsysView::OnBnClickedButtonAutoRobotStop)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_AUTO_ROBOT_STOP, &CDATsysView::OnBnDoubleclickedButtonAutoRobotStop)
	ON_STN_CLICKED(IDC_PLC_COM_STATUS, &CDATsysView::OnStnClickedPlcComStatus)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_CONNECT, &CDATsysView::OnBnClickedButtonServerConnect)
	ON_COMMAND(ID_OPTION_VFOPTION, &CDATsysView::OnOptionVfoption)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MAIN_PROCESS, &CDATsysView::OnNMCustomdrawListMainProcess)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MAIN_PROCESS, &CDATsysView::OnNMDblclkListMainProcess)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAIN_PROCESS, &CDATsysView::OnNMClickListMainProcess)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CDATsysView::OnHdnItemclickListMainProcess)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OPTIN_PROCESS, &CDATsysView::OnNMCustomdrawListOptinProcess)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_VERSION_PROCESS, &CDATsysView::OnNMCustomdrawListVersionProcess)
	ON_BN_CLICKED(IDC_BUTTON_BAT_VER, &CDATsysView::OnBnClickedButtonBatVer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDATsysView construction/destruction

CDATsysView::CDATsysView()
	: CFormView(CDATsysView::IDD)
{
	g_pView = this;
	//{{AFX_DATA_INIT(CDATsysView)
	m_bInput_Lock = TRUE;
	m_bSeq_Lock = FALSE;
	//m_szGrabberVersion = _T("");
	m_nToolOptionMethod = -1;
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

	m_bVFTestThreadRunning	= FALSE;
	m_bStopVFTestThread		=  FALSE;

	m_nNoRepeatExecution		= 1;
	m_nNoCurrentRepeatExecution = 0;
	m_nNoPassInRepeatExecution	= 0;
	m_nNoFailInRepeatExecution	= 0;

	m_szResultMsg1			= _T("");
	m_szResultMsg2			= _T("");

	m_nCurrentViewImageType	= 0;

	m_bMakeReferenceMode	= FALSE;
	m_szExePath				= _T("");
	m_szNoImageBmpPath		= _T("");
	m_szMakeRefPath			= _T("");

	m_bContinuousLVDSGrabRunning = FALSE;
	
	m_bUseDefaultREFfolder	= FALSE;

	//+add psh 080603
	m_bResultDlgActivate	= FALSE;
	m_bInputCheckDlgActivate = FALSE;

	m_nGrabType				= 0;
	m_nGrabResult			= 0;
	m_bStopMeasureAudio		= FALSE;
	m_bTimeDataFileOpen		= FALSE;
	m_dwStepElapsedTime		= 0;
	m_szVersion				= _T("");

	//+change 090213(Modification No1)
	m_pGrabImageThread		= NULL;
	m_pTestThread			= NULL;
	m_pVFTestThread			= NULL;

// FHD
	m_pImageInfo			= NULL;
	m_hImageDataFileMapping = NULL;
	m_hImageInfoFileMapping = NULL;
//	m_hDimmingTestFileMapping = NULL;
//
	InitializeCriticalSection(&g_cs);//pshÃß
	InitializeCriticalSection(&g_cs2);//add 090818
	InitializeCriticalSection(&g_cs3);
	InitializeCriticalSection(&g_cs4);

	m_nOldFailedStep		= 0;
	m_nGrabber_FWVer		= 0;
	m_pUsbDio_Control		= NULL;
	m_bAcPowerOnOff			= FALSE;
	m_bMhlMode				= FALSE;

	m_bResult_S6F5			= FALSE;
	m_bResult_S6F12			= FALSE;
	m_bResult_S6F2			= FALSE;

	m_GetToolOptiondlg.m_bActivate = FALSE;
//	m_ResultDlg.m_bActivate = FALSE;
	m_StopStepDlg.m_bActivate = FALSE;
	m_ManualTestDlg.m_bActivate = FALSE;
	//UHD
	m_bGrabConnection = FALSE;
	m_sGrabberFW_Version = _T("NOT");
	m_hVfThreadKillEvent	= NULL;
}

CDATsysView::~CDATsysView()
{

	//m_bmpNormal.DeleteObject();
	//m_bmpGood.DeleteObject();
	//m_bmpFail.DeleteObject();
	//m_bmpRun.DeleteObject();

	DeleteCriticalSection(&g_cs);//pshÃß
	DeleteCriticalSection(&g_cs2);//add 090818
	DeleteCriticalSection(&g_cs3);
	DeleteCriticalSection(&g_cs4);

//	CoFreeUnusedLibraries();
}

void CDATsysView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDATsysView)
	DDX_Control(pDX, IDC_COM_DEBUG_LOG, m_ctrlEditComLog);
	DDX_Control(pDX, IDC_VIEW_TAB, m_ctrlViewTab);
	DDX_Check(pDX, IDC_INPUT_LOCK, m_bInput_Lock);
	DDX_Check(pDX, IDC_SEQ_LOCK, m_bSeq_Lock);
	//DDX_Control(pDX, IDC_MAINVIEW_GRID, m_ctrlSummaryGrid);
	//DDX_Text(pDX, IDC_GRABBER_VER, m_szGrabberVersion);
	DDX_Radio(pDX, IDC_RDO_TOOL_OPTION1, m_nToolOptionMethod);
	//DDX_Control(pDX, IDC_OPTION_GRID, m_ctrlOptionGrid);
	//DDX_Control(pDX, IDC_COUNTRY_GRID, m_ctrlCountryGrid);
	//DDX_Control(pDX, IDC_VERSION_GRID, m_ctrlVersionGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_MESON, m_LedsMesOn);
	DDX_Control(pDX, IDC_BUTTON_AUTO_ROBOT_RESET, m_cBtnResetRobot);
	DDX_Control(pDX, IDC_BUTTON_AUTO_ROBOT_STOP, m_cBtnStopRobot);
	//	DDX_Control(pDX, IDC_LIST_MAIN_PROCESS, m_CtrlListMainProcess);
	DDX_Control(pDX, IDC_LIST_MAIN_PROCESS, m_CtrlListMainProcess);
	DDX_Control(pDX, IDC_LIST_OPTIN_PROCESS, m_ctrlListOption);
	//	DDX_Control(pDX, IDC_LIST_COUNTRY_PROCESS, m_ctrlListCountry);
	DDX_Control(pDX, IDC_LIST_VERSION_PROCESS, m_ctrlListVersion);
	DDX_Control(pDX, IDC_EDIT_BAT_VER, CtrlBatVerEdit);
	DDX_Control(pDX, IDC_EDIT_EPI_CFG_SW, m_CEdit_EPI_CFG_SW);
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

	ctrlStepTarget_V.SetWindowText(_T(""));
	ctrlStepMeasur_V.SetWindowText(_T(""));
	ctrlStepTarget_A.SetWindowText(_T(""));
	ctrlStepMeasur_A.SetWindowText(_T(""));

	//IDC_PLC_COM_STATUS
	ctrlPLC_COM_Status.SubclassDlgItem(IDC_PLC_COM_STATUS,this);
	ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
	ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
	ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));

	ctrlROBOT_Status.SubclassDlgItem(IDC_ROBOT_STATUS,this);
	ctrlROBOT_Status.SetWindowText(_T("- -"));//IDC_ROBOT_STATUS
	ctrlROBOT_Status.SetBkColor(RGB(128, 128, 128));
	ctrlROBOT_Status.SetTextColor(RGB(0, 0, 0));
	//ctrlROBOT_Status;//IDC_ROBOT_STATUS IDC_ROBOT_STATUS


	ctrlWipIdEdit.SubclassDlgItem(IDC_EDIT_WIPID, this);
	ctrlWipIdEdit.SetWindowText(_T(""));
	m_strWipId = _T("");
	ctrlSuffixEdit.SubclassDlgItem(IDC_EDIT_SUFFIX, this);
	ctrlSuffixEdit.SetWindowText(_T(""));


	m_LedsMesOn.SetIcons(IDI_LEDOFF, IDI_LEDON);
//	m_LedsMesOn.SetTextBackground(0, RGB(238, 239, 231));
//	m_LedsMesOn.SetTextBackground(1, RGB(238, 239, 231));
	m_LedsMesOn.SetTextColors(0, COLOR_DARKBLUE, COLOR_LIGHTGRAY); //RGB(238, 239, 231));
	m_LedsMesOn.SetTextColors(1, COLOR_BLUE, COLOR_LIGHTGRAY); //RGB(238, 239, 231));

	
	m_cStepResultNew.SubclassDlgItem(IDC_STEPRESULT_NEW, this);
	m_cStepResultNew.SetWindowText(_T("READY"));//IDC_ROBOT_STATUS
	m_cStepResultNew.SetBkColor(RGB(128, 200, 64));
	m_cStepResultNew.SetTextColor(RGB(0, 0, 0));

	m_BatVerReadOnly = 1;	
	CtrlBatVerEdit.SetReadOnly(1);
	CtrlBatVerEdit.SetWindowText(CurrentSet->sBatVersion);

}

void CDATsysView::OnInitialUpdate()
{
	CRect rect;
	// 090910
	CString sMsg		= _T("");
	CString sTmp		= _T("");

	CFormView::OnInitialUpdate();

//	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();

	m_pFrame =(CMainFrame*)AfxGetApp()->m_pMainWnd;

    m_pFrame->GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	if(g_nRunningProcessNo ==1){

		m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	}
	else if(g_nRunningProcessNo ==2){
		m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME1));
	}
	else{
		m_hAccel = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME2));
	}

	// 090909
	//====================
	// Open Sequence File 
	//====================
	if(CurrentSet->sSeqPath.IsEmpty())
	{
	
	}
	else
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

		sTmp.Format(_T("%s_VF.cfg"), CurrentSet->sModelIni.Left(CurrentSet->sModelIni.ReverseFind('.')));
		CurrentSet->sVFTestIni = sTmp;

		if(!CurrentSet->sVFTestIni.IsEmpty())
		{
			//sTmp.Format(_T("%s\\VF_TestConfig.ini"),CurrentSet->sModelIni.Left(CurrentSet->sModelIni.ReverseFind('\\')));
			

			if(FileExists(CurrentSet->sVFTestIni))
			{
				OpenVFTestFile(CurrentSet->sVFTestIni);
			}
			else
			{
				sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sVFTestIni);
				AfxMessageBox(sMsg);
				DeleteVFTestList();
			}
		}


	}


	//Control Subclassing
	InitSubClassItem();

	TestCountRead(m_szExePath);
//	g_nNoTested = CurrentSet->nTotalNoTest;
//	g_nFailed	= CurrentSet->nTotalNoNg;
	//+add kwmoon 081004
//	g_nUserStop	= CurrentSet->nTotalNoUserStop;
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
	g_SoundCard.Creat(this);

	InitMainGrid(CurrentSet->nDisplayType);
	InsertStepData2Grid(CurrentSet->nDisplayType);
	InitOptionGrid();
	InitCountryGrid();
	InitVersionGrid();

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
	m_ctrlViewTab.InsertItem(2, &item);
#ifdef SM_MODIFY_CODE__
	item.pszText = "HDMI Grab";
	m_ctrlViewTab.InsertItem(3, &item);
#endif
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


//	m_ctrlComLog.SetEditCtrl( m_ctrlEditComLog.m_hWnd );
	// Redirect cout to our Edit-Control
//	m_pOldBuf = cout.rdbuf( &m_EditStrBuf );
	// and also the unicode version
//	m_pOldBufW = wcout.rdbuf( &m_EditStrBufW );

	Invalidate();  

	//===========
	// Set Timer
	//===========
//	SetTimer(TIMER_MEASURE_AUDIO_OUTPUT, 200, NULL);
	SetTimer(TIMER_UPDATE_STATUS_INFO, 400, NULL);

	//+del 090220(Modification No2)
//	SetTimer(TIMER_UPDATE_ALL_SCREEN, 5000, NULL);

	//+add 090409(Modification No1)
	g_nCurSwitchBoxLogLine = 0;
	g_nCurRemoteLogLine	   = 0;

	ctrlSuffixEdit.SetReadOnly(TRUE);
	ctrlWipIdEdit.SetFocus();

	if(g_nGrabberType == UHD_GRABBER)
	{
		InitGrabImageEvent();
	}

	m_hGrabThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);


	InitEventAndFileMappingObject();

	if(g_nGrabberType == UHD_GRABBER){
		g_GrabImage.m_pImageData = new unsigned char	[10000*8000];
		g_GrabDisplayImage.m_pImageData = new unsigned char	[10000*8000];
	}
//	m_pImageInfo = (ImageInfoStruct*) malloc(sizeof(ImageInfoStruct));
//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		SetGrabInfo(&g_GrabImage);
	}
//	
//	SetGrabInfo(&g_GrabDisplayImage);
	g_GrabSource = LVDS_GRABBER;

	PostMessage(UM_INIT_DIALOG,0,0);

	//+add kwmoon 071106
	m_Grab1RunningFlag	= FALSE;
	m_Grab2RunningFlag	= FALSE;
	m_Grab3RunningFlag	= FALSE;
//FHD
//	m_Dimming1RunningFlag = FALSE;
//	m_Dimming2RunningFlag = FALSE;
//	m_Dimming3RunningFlag = FALSE;
//	
	m_szCurrentStatus = _T("READY"); 
	//m_szGrabberVersion = _T("NOT LOAD");

	UpdateData(FALSE);
	//+add kwmoon 080724
	GetApplicationVersion(); 

	//m_cStepResult.SubclassDlgItem(IDC_STEPRESULT, this);

	//m_bmpNormal.LoadBitmap(IDB_BITMAP_NOMAL);
	//m_bmpGood.LoadBitmap(IDB_BITMAP_GOOD);
	//m_bmpFail.LoadBitmap(IDB_BITMAP_FAIL);
	//m_bmpRun.LoadBitmap(IDB_BITMAP_RUN);

	//m_cStepResult.SetBitmap(m_bmpNormal);
	//m_cStepResultNew


	UpdateToolOptionInfo(FALSE);

	UpdateInfo(LED_STATE,"");

	ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);

	CurrentSet->bRunAbort	 = FALSE;

	//AUTO
	if(g_nSysType == AUTO_SYS){
		//gJigCtrl.Set_ACPower(0);
		gJigCtrl.Set_CylinderUP();
	//
	}
	else{
		Set_Relay(0);
	}
	if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == 1))
	{
		ctrlPLC_COM_Status.ShowWindow(1);
		ctrlROBOT_Status.ShowWindow(1);
		m_cBtnResetRobot.ShowWindow(1);
		m_cBtnStopRobot.ShowWindow(1);
	}
	else
	{
		ctrlPLC_COM_Status.ShowWindow(0);
		ctrlROBOT_Status.ShowWindow(0);
		m_cBtnResetRobot.ShowWindow(0);
		m_cBtnStopRobot.ShowWindow(0);
	}
//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		Set_Mhl(1, 1);
	}
//
	if (CurrentSet->nToolOptionSetMethod == 1)
	{
		//godtech 190602
		if (g_nSysRobotType == AUTO_SYS)
		{
			GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);

		}
		else
		{
			//if (CurrentSet->bUseScanner) {
			GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);
		/*	}
			else {
				GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
			}*/
		}

	}
	else{
		GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
	}
	m_nToolOptionMethod = CurrentSet->nToolOptionSetMethod;

	//Comp1//m_ctrlSummaryGrid.SetRow(-1);
	if((g_hCommWnd = GetSafeHwnd()) == NULL)
	{
		AfxMessageBox("View Handle Null!");
	}
	UpdateData(FALSE);

	OnInputLock();

	TVCommCtrl.Open_MacAdd_List();

//	installhook(this->GetSafeHwnd());
	m_hReadEvent_S6F5 = ::CreateEvent(NULL,false,false,NULL);
	m_hReadEvent_S6F12 = ::CreateEvent(NULL,false,false,NULL);
	m_hReadEvent_S6F2 = ::CreateEvent(NULL,false,false,NULL);

	m_hVfThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);

	if ((g_nRunningProcessNo == 1) && (CurrentSet->bAutoRun_DFT2 == TRUE)) {
		CString szModuleName1;
		CString szModuleName2;
		BOOL bSuccess;

		PROCESS_INFORMATION pi;
		STARTUPINFO si = { 0 };
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_SHOW;

#ifdef _DFT_3IN1

		szModuleName1.Format(_T("%s\\DFT2_3in1.exe %s"), m_szExePath, g_sData_MD5);
		szModuleName2.Format(_T("%s\\DFT3_3in1.exe %s"), m_szExePath, g_sData_MD5);

		bSuccess = CreateProcess(NULL, (LPSTR)(LPCTSTR)szModuleName1, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		if (!bSuccess)
		{
			AfxMessageBox("Failed to load DFT2_3in1.exe");
		}

		Sleep(500);
		bSuccess = CreateProcess(NULL, (LPSTR)(LPCTSTR)szModuleName2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		if (!bSuccess)
		{
			AfxMessageBox("Failed to load DFT3_3in1.exe");
		}

#endif

#ifdef _DFT_2IN1
		szModuleName1.Format(_T("%s\\DFT2.exe %s"), m_szExePath, g_sData_MD5);
		bSuccess = CreateProcess(NULL, (LPSTR)(LPCTSTR)szModuleName1, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		if (!bSuccess)
		{
			AfxMessageBox("Failed to load DFT2.exe");
		}
#endif

	}
	if ((g_nSysRobotType == AUTO_SYS) && (g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
	{
		//if ((CurrentSet->nLvdsWidth == 1366) && (CurrentSet->nLvdsHeight == 768))
		//{
		//	if (CurrentSet->bRobotSkipFFC == 0)
		//	{
		//		MessageBox("Check POGO PIN BLOCK");
		//	}
		//}
		//else
		//{
		//	//if (CurrentSet->bRobotSkipFFC == 1)
		//	//{
		//	//	MessageBox("Check Robot Skip FFC");
		//	//}
		//}
		gPLC_Ctrl.CommandSkipFFC();
		//if (gPLC_Ctrl.m_cCheckSkipFFC == 1)
		//{
		//	gPLC_Ctrl.CommandSkipFFC();			
		//}
		//else
		//{			
		//	gPLC_Ctrl.CommandConnectFFC();			
		//}

	//	SetTimer(TIMER_SERVER_CONTROL_ID, 100, NULL);
	}

	if (CurrentSet->bCheckBatteryCheck == 1)
	{
		CtrlBatVerEdit.SetWindowText(CurrentSet->sBatVersion);
		CtrlBatVerEdit.ShowWindow(1);
		GetDlgItem(IDC_BUTTON_BAT_VER)->ShowWindow(1);
	}
	else
	{
		CtrlBatVerEdit.ShowWindow(0);
		GetDlgItem(IDC_BUTTON_BAT_VER)->ShowWindow(0);
	}


	CurrentSet->sEpiCfgIni = m_szExePath + "\\EPI_Config.ini";
	if (OpenEPI_ConfigFile(CurrentSet->sEpiCfgIni))
	{
		//CurrentSet->sDipStatus
	}
	else
	{
		CurrentSet->sDipStatus = "";
	}
	m_CEdit_EPI_CFG_SW.SetWindowText(CurrentSet->sDipStatus);

	SystemMonitorLog_Save("[START]\nOnInitialUpdate()", _T(__FILE__), __LINE__);
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
	logFont1.lfCharSet			= ANSI_CHARSET;
//	strcpy_s(logFont1.lfFaceName, _countof(logFont1.lfFaceName),(LPCTSTR)"Courier New");
	strcpy_s(logFont1.lfFaceName, _countof(logFont1.lfFaceName),(LPCTSTR)"Trebuchet MS");
//	strcpy_s(logFont1.lfFaceName, _countof(logFont1.lfFaceName),(LPCTSTR)"Tahoma");

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
	strcpy_s(logFont2.lfFaceName, _countof(logFont2.lfFaceName),(LPCTSTR)"Courier New");

	m_StatusWindowFont.CreateFontIndirect(&logFont2);

	// Set Font3
	LOGFONT logFont3;
	logFont3.lfHeight			= 14;
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
	strcpy_s(logFont3.lfFaceName, _countof(logFont3.lfFaceName),(LPCTSTR)"System");

	m_ProgressFont.CreateFontIndirect(&logFont3);

	
#ifdef _DFT_3IN1
	LOGFONT logFont4;
	logFont4.lfHeight = 24;
	logFont4.lfWidth = 8;
	logFont4.lfEscapement = 0;
	logFont4.lfOrientation = 0;
	logFont4.lfWeight = FW_BOLD;
	logFont4.lfItalic = 0;
	logFont4.lfUnderline = 0;
	logFont4.lfStrikeOut = 0;
	logFont4.lfOutPrecision = 1;
	logFont4.lfClipPrecision = 2;
	logFont4.lfQuality = 1;
	logFont4.lfPitchAndFamily = 17;
	logFont4.lfCharSet = ANSI_CHARSET;
	strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Arial");
	//strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Courier New");
	m_Step_New_Font.CreateFontIndirect(&logFont4);

	m_cStepResultNew.SetFont(&m_Step_New_Font);
#else

	LOGFONT logFont4;
	logFont4.lfHeight = 42;
	logFont4.lfWidth = 14;
	logFont4.lfEscapement = 0;
	logFont4.lfOrientation = 0;
	logFont4.lfWeight = FW_BOLD;
	logFont4.lfItalic = 0;
	logFont4.lfUnderline = 0;
	logFont4.lfStrikeOut = 0;
	logFont4.lfOutPrecision = 1;
	logFont4.lfClipPrecision = 2;
	logFont4.lfQuality = 1;
	logFont4.lfPitchAndFamily = 17;
	logFont4.lfCharSet = ANSI_CHARSET;
	strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Arial");
	//strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Courier New");
	m_Step_New_Font.CreateFontIndirect(&logFont4);

	m_cStepResultNew.SetFont(&m_Step_New_Font);

#endif
	//ctrlPLC_COM_Status.SetFont(&m_Step_New_Font);
	//ctrlROBOT_Status.SetFont(&m_Step_New_Font);

	//m_cBtnResetRobot.SetFont(&m_Step_New_Font);
	//LOGFONT logFont4;
	//logFont4.lfHeight = 28;
	//logFont4.lfWidth = 22;
	//logFont4.lfEscapement = 0;
	//logFont4.lfOrientation = 0;
	//logFont4.lfWeight = FW_BOLD;
	//logFont4.lfItalic = 0;
	//logFont4.lfUnderline = 0;
	//logFont4.lfStrikeOut = 0;
	//logFont4.lfOutPrecision = 1;
	//logFont4.lfClipPrecision = 2;
	//logFont4.lfQuality = 1;
	//logFont4.lfPitchAndFamily = 17;
	//logFont4.lfCharSet = ANSI_CHARSET;
	//strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"System");
	////strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Courier New");
	//m_PLC_Font.CreateFontIndirect(&logFont4);
	

	//ctrlPLC_COM_Status.SetFont(&m_PLC_Font);
	//ctrlROBOT_Status.SetFont(&m_PLC_Font);
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
	m_PLC_Font.CreateFontIndirect(&logFont5);

	
	ctrlPLC_COM_Status.SetFont(&m_PLC_Font);
	ctrlROBOT_Status.SetFont(&m_PLC_Font);
	m_cBtnResetRobot.SetFont(&m_PLC_Font);
	m_cBtnStopRobot.SetFont(&m_PLC_Font);
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

	CtrlBatVerEdit.SetFont(&m_CustomFont);

	ctrlSuffixEdit.SetFont(&m_CustomFont);
//	m_stcStatus.SetFont(&m_StatusWindowFont);
//	m_stcStatus.SetFont(&m_CustomFont);

	//+add kwmoon 080502
//	ctrlToolOptionTitle.SetFont(&m_CustomFont);
//	ctrlAreaOptionTitle.SetFont(&m_CustomFont);
//	ctrlCommercialOptionTitle.SetFont(&m_CustomFont);

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

	ctrlTestProgress.SetFont(&m_ProgressFont);
#ifdef _DFT_3IN1
	LOGFONT logFont6;
	logFont6.lfHeight = 16;
	logFont6.lfWidth = 0;
	logFont6.lfEscapement = 0;
	logFont6.lfOrientation = 0;
	logFont6.lfWeight = FW_BOLD;
	logFont6.lfItalic = 0;
	logFont6.lfUnderline = 0;
	logFont6.lfStrikeOut = 0;
	logFont6.lfOutPrecision = 1;
	logFont6.lfClipPrecision = 2;
	logFont6.lfQuality = 1;
	logFont6.lfPitchAndFamily = 17;
	logFont6.lfCharSet = ANSI_CHARSET;
	//	strcpy_s(logFont1.lfFaceName, _countof(logFont1.lfFaceName),(LPCTSTR)"Courier New");
	strcpy_s(logFont6.lfFaceName, _countof(logFont1.lfFaceName), (LPCTSTR)"Trebuchet MS");
	//	strcpy_s(logFont1.lfFaceName, _countof(logFont1.lfFaceName),(LPCTSTR)"Tahoma");

	m_OptionList_Font.CreateFontIndirect(&logFont6);
#else
	LOGFONT logFont6;
	logFont6.lfHeight = 18;
	logFont6.lfWidth = 0;
	logFont6.lfEscapement = 0;
	logFont6.lfOrientation = 0;
	logFont6.lfWeight = FW_BOLD;
	logFont6.lfItalic = 0;
	logFont6.lfUnderline = 0;
	logFont6.lfStrikeOut = 0;
	logFont6.lfOutPrecision = 1;
	logFont6.lfClipPrecision = 2;
	logFont6.lfQuality = 1;
	logFont6.lfPitchAndFamily = 17;
	logFont6.lfCharSet = ANSI_CHARSET;
	//	strcpy_s(logFont1.lfFaceName, _countof(logFont1.lfFaceName),(LPCTSTR)"Courier New");
	strcpy_s(logFont6.lfFaceName, _countof(logFont1.lfFaceName), (LPCTSTR)"Trebuchet MS");
	//	strcpy_s(logFont1.lfFaceName, _countof(logFont1.lfFaceName),(LPCTSTR)"Tahoma");

	m_OptionList_Font.CreateFontIndirect(&logFont6);
	
#endif
	m_CtrlListMainProcess.SetFont(&m_CustomFont);
	m_ctrlListOption.SetFont(&m_OptionList_Font);
	m_ctrlListVersion.SetFont(&m_OptionList_Font);
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
	CString sTemp;

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
//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		m_nGrabber_FWVer = m_pImageInfo->nGrabber_FWVer;
		if(m_nGrabber_FWVer > 0)
		{
			sTemp.Format("%08X", m_nGrabber_FWVer);
			GetDlgItem(IDC_GRABBER_VER)->SetWindowText(sTemp); //m_szGrabberVersion = sTemp;
		}
		else
		{
			sTemp.Format("NOT");
			GetDlgItem(IDC_GRABBER_VER)->SetWindowText(sTemp); //m_szGrabberVersion = sTemp;
		}
	//	UpdateData(FALSE);
	}
//
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
			if(CurrentSet->bIsRunning)
			{ 	//UHD
				if(g_nGrabberType == UHD_GRABBER)
				{
					if(g_GrabDisplayImage.m_bImageLoaded)
					{
						DrawImage(IDC_VIDEOVIEW,g_GrabDisplayImage);
					}
				}
				else  //FHD
				{ 
					if(g_GrabImage.m_bImageLoaded)
					{
						DrawImage(IDC_VIDEOVIEW,g_GrabImage);
					}
				}
				//
			}
			else
			{
				if(m_bResultDlgActivate)
				{//UHD
					if(g_nGrabberType == UHD_GRABBER)
					{
						if(g_GrabDisplayImage.m_bImageLoaded)
						{
							DrawImage(IDC_VIDEOVIEW,g_GrabDisplayImage);
						}
					}
					else //FHD
					{
						if(g_GrabImage.m_bImageLoaded)
						{
							DrawImage(IDC_VIDEOVIEW,g_GrabImage);
						}
					}//					
				}
				else
				{
					if(m_bGrabThreadRunning)
					{//UHD
						if(g_nGrabberType == UHD_GRABBER)
						{
							if(g_GrabDisplayImage.m_bImageLoaded)
							{
								DrawImage(IDC_VIDEOVIEW,g_GrabDisplayImage);
							}
						}
						else //FHD
						{
							if(g_GrabImage.m_bImageLoaded)
							{
								DrawImage(IDC_VIDEOVIEW,g_GrabImage);
							}
						}//						
					}
					else if(m_Image[I_GRAB].m_bImageLoaded)
					{
						DrawImage(IDC_VIDEOVIEW,m_Image[I_GRAB]);
					}
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
#if 1


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
	
		case TOOL_OPTION1 :
			//m_ctrlOptionGrid.SetTextMatrix(1, 1, _T(s));
			m_ctrlListOption.SetItemText(0, 1, _T(s)); //m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); 
			//m_ctrlOptionGrid.SetTextMatrix(1, 1, _T(s));

			break;
		case TOOL_OPTION2 :
			//m_ctrlOptionGrid.SetTextMatrix(2, 1, _T(s));
			m_ctrlListOption.SetItemText(1, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(2, 1, _T(s));
			break;

		case TOOL_OPTION3 :
			//m_ctrlOptionGrid.SetTextMatrix(3, 1, _T(s));
			m_ctrlListOption.SetItemText(2, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(3, 1, _T(s));
			break;

		case TOOL_OPTION4 :
			//m_ctrlOptionGrid.SetTextMatrix(4, 1, _T(s));
			m_ctrlListOption.SetItemText(3, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(4, 1, _T(s));
			break;

		case TOOL_OPTION5 :
			m_ctrlListOption.SetItemText(4, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(5, 1, _T(s));
			break;

		case TOOL_OPTION6 :
			m_ctrlListOption.SetItemText(5, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(6, 1, _T(s));
			break;
			//+add 110928
		case TOOL_OPTION7 :
			m_ctrlListOption.SetItemText(6, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(7, 1, _T(s));
			break;

		case COMMERCIAL_OPTION1 :
			m_ctrlListOption.SetItemText(7, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(8, 1, _T(s));
			break;

		case TOOL_OPTION8 :
			m_ctrlListOption.SetItemText(8, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(9, 1, _T(s));
			break;

		case TOOL_CRC :
			m_ctrlListOption.SetItemText(9, 1, _T(s)); //m_ctrlOptionGrid.SetTextMatrix(10, 1, _T(s));
			break;



		case CPU_VERSION :
			m_ctrlListVersion.SetItemText(0, 1, _T(s)); //m_ctrlVersionGrid.SetTextMatrix(0, 1, _T(s));
			break;

		case MICOM_VERSION :
			m_ctrlListVersion.SetItemText(1, 1, _T(s)); //m_ctrlVersionGrid.SetTextMatrix(1, 1, _T(s));
			break;

		case USB_VERSION :
		/*	if(s.Compare("NULL") != NULL)
				ctrlUSBVersion.SetWindowText(_T(s));
			else
				ctrlUSBVersion.SetWindowText(_T("")); */
			break;
		case MNT_VERSION :
			m_ctrlListVersion.SetItemText(2, 1, _T(s)); //m_ctrlVersionGrid.SetTextMatrix(2, 1, _T(s));
			break;

		case AREA_OPTION1 :
			m_ctrlListVersion.SetItemText(3, 1, _T(s)); //m_ctrlCountryGrid.SetTextMatrix(0, 1, _T(s));
			break;

		case COUNTRY_GROUP :
			m_ctrlListVersion.SetItemText(4, 1, _T(s)); //m_ctrlCountryGrid.SetTextMatrix(1, 1, _T(s));
			break;

		case COUNTRY :
			m_ctrlListVersion.SetItemText(5, 1, _T(s)); //m_ctrlCountryGrid.SetTextMatrix(2, 1, _T(s));
			break;
		case LED_STATE:
			m_LedsMesOn.EnableWindow(CurrentSet->bGMES_Connection);
			m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
		//	m_LedsMesOn.SetLedState(1);
			break;
	}


 }
#else
void CDATsysView::UpdateInfo(int nIndex, CString s)
{
	CString sTmp, sModelName, sModelFolder;

	switch (nIndex)
	{
	case SEQ:
		m_stcModelFolder.SetWindowText(_T(s));
		break;
	case MODEL:
		m_stcSeqName.SetWindowText(_T(s));
		break;
	case CHASSIS:
		m_stcChassisName.SetWindowText(_T(s));
		break;
	case REF:
		m_stcRefFolder.SetWindowText(_T(s));
		break;
	case TESTED:
		m_ctrLEDTested.SetWindowText(_T(s));
		break;
	case FAILED:
		m_ctrLEDFailed.SetWindowText(_T(s));
		break;
	case RATE:
		m_ctrLEDRate.SetWindowText(_T(s));
		break;
	case STATUS:
		m_stcStatus.SetWindowText(_T(s));
		break;
	case MODE:
		m_stcMode.SetWindowText(_T(s));
		break;
	case TIME:
		m_stcTime.SetWindowText(_T(s));
		break;
	case PROG_NO:
		//			m_stcProgramNo.SetWindowText(_T(s)); 
		break;
	case AUDIO_L_FREQ:
		//			m_stcLFreq.SetWindowText(_T(s));
		break;
	case AUDIO_R_FREQ:
		//			m_stcRFreq.SetWindowText(_T(s));
		break;
	case AUDIO_L_LEVEL:
		//			m_stcLLevel.SetWindowText(_T(s));
		break;
	case AUDIO_R_LEVEL:
		//			m_stcRLevel.SetWindowText(_T(s));
		break;

	case TOOL_OPTION1:
		//m_ctrlOptionGrid.SetTextMatrix(1, 1, _T(s));
		m_ctrlOptionGrid.SetTextMatrix(1, 1, _T(s));

		break;
	case TOOL_OPTION2:
		//m_ctrlOptionGrid.SetTextMatrix(2, 1, _T(s));
		m_ctrlOptionGrid.SetTextMatrix(2, 1, _T(s));
		break;

	case TOOL_OPTION3:
		//m_ctrlOptionGrid.SetTextMatrix(3, 1, _T(s));
		m_ctrlOptionGrid.SetTextMatrix(3, 1, _T(s));
		break;

	case TOOL_OPTION4:
		//m_ctrlOptionGrid.SetTextMatrix(4, 1, _T(s));
		m_ctrlOptionGrid.SetTextMatrix(4, 1, _T(s));
		break;

	case TOOL_OPTION5:
		m_ctrlOptionGrid.SetTextMatrix(5, 1, _T(s));
		break;

	case TOOL_OPTION6:
		m_ctrlOptionGrid.SetTextMatrix(6, 1, _T(s));
		break;
		//+add 110928
	case TOOL_OPTION7:
		m_ctrlOptionGrid.SetTextMatrix(7, 1, _T(s));
		break;

	case COMMERCIAL_OPTION1:
		m_ctrlOptionGrid.SetTextMatrix(8, 1, _T(s));
		break;

	case TOOL_OPTION8:
		m_ctrlOptionGrid.SetTextMatrix(9, 1, _T(s));
		break;

	case TOOL_CRC:
		m_ctrlOptionGrid.SetTextMatrix(10, 1, _T(s));
		break;


	case AREA_OPTION1:
		m_ctrlCountryGrid.SetTextMatrix(0, 1, _T(s));
		break;

	case COUNTRY_GROUP:
		m_ctrlCountryGrid.SetTextMatrix(1, 1, _T(s));
		break;

	case COUNTRY:
		m_ctrlCountryGrid.SetTextMatrix(2, 1, _T(s));
		break;

	case CPU_VERSION:
		m_ctrlVersionGrid.SetTextMatrix(0, 1, _T(s));
		break;

	case MICOM_VERSION:
		m_ctrlVersionGrid.SetTextMatrix(1, 1, _T(s));
		break;

	case USB_VERSION:
		/*	if(s.Compare("NULL") != NULL)
				ctrlUSBVersion.SetWindowText(_T(s));
			else
				ctrlUSBVersion.SetWindowText(_T("")); */
		break;
	case MNT_VERSION:
		m_ctrlVersionGrid.SetTextMatrix(2, 1, _T(s));
		break;

	case LED_STATE:
		m_LedsMesOn.EnableWindow(CurrentSet->bGMES_Connection);
		m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
		//	m_LedsMesOn.SetLedState(1);
		break;
	}


}
#endif

void CDATsysView::OnEditCompile() 
{
	//+add PSH 090507
	BOOL bFlag;

	CString sMsg;

	bFlag = g_SoundCard.wIn_Flag;
	OnAudioMeasStop();
	//-
	if (!CurrentSet->sVFTestIni.IsEmpty())
	{
		//sTmp.Format(_T("%s\\VF_TestConfig.ini"),CurrentSet->sModelIni.Left(CurrentSet->sModelIni.ReverseFind('\\')));


		if (FileExists(CurrentSet->sVFTestIni))
		{
			OpenVFTestFile(CurrentSet->sVFTestIni);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sVFTestIni);
			AfxMessageBox(sMsg);
			DeleteVFTestList();
		}
	}
	else
	{
		sMsg.Format("Failed to load file. File Name Empty !\n");
		AfxMessageBox(sMsg);
		DeleteVFTestList();

	}

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
	if(bFlag) SetTimer(2, 1000, NULL);
}

void CDATsysView::OnEditCurrent() 
{//+ Check - Edit ÇÁ·Î±×·¥ °æ·Î ¼³Á¤ÇØ¾ßÇÔ
	PROCESS_INFORMATION pInfo;
	STARTUPINFO         sInfo;
	char				achCommand[128];
	CWnd				*pWndPrev, *pWndChild;
	CString				s;
  
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

//	strcpy_s(achCommand, _countof(achCommand),(LPCSTR)s);
	strcpy_s(achCommand, _countof(achCommand), _T("C:\\Windows\\Notepad.exe "));
    if(!CurrentSet->sSeqPath.IsEmpty())
	{

//#ifdef		_ERROR_CHILD_CONNECT_DEBUG__MODE
//		strcpy_s(achCommand, _countof(achCommand), _T("C:\\Windows\\Notepad_.exe "));
//		CurrentSet->sSeqPath += "_";
//#endif
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
	CString sKeyClass	= _T("");
	BOOL bFlag;

	if(OpenModelListFile(CurrentSet->sModelListPath,nNoModel,g_Divisions) == FALSE) return;	

	//+add PSH 090507
	bFlag = g_SoundCard.wIn_Flag;
	//090615
	AudioMeasureStop();
//	OnAudioMeasStop();
	//-
	if (m_bGrabThreadRunning)
	{
		//SendMessage(ID_GRAB_START, ID_RUN_RUN);//
		/////////////////////////////////////////////
		////UHD
		if (g_nGrabberType == UHD_GRABBER)
		{
			StopLVDSGrabThread();
		}//
		else//FHD
		{
			StopLVDSGrabThread_FHD();
		}
	}

	//m_pFrame->OnUpdateGrabStart();
	//CSelectChassisModelDlg dlg;
	m_SelectChassisModelDlg.m_nNoModel = nNoModel;

#ifdef			DEBUG_MD5_CODE__
//	SetTimer(TIMER_CHECK_DEBUG_TEST, 10000, NULL);
#endif

	if(m_SelectChassisModelDlg.DoModal() == IDOK)
	{
		//+move kwmoon 080818
		CurrentSet->sSeqPath		= CurrentSet->sServerFolder + m_SelectChassisModelDlg.m_szSeqFilePath;
		CurrentSet->sModelIni		= CurrentSet->sModelInfoFolder + m_SelectChassisModelDlg.m_szModelInfoFilePath;

#ifdef SM_MODIFY_CODE__

		sTmp.Format(_T("%s_VF.cfg"), m_SelectChassisModelDlg.m_szModelInfoFilePath.Left(m_SelectChassisModelDlg.m_szModelInfoFilePath.ReverseFind('.')));
		CurrentSet->sVFTestIni = CurrentSet->sModelVFConfigFolder + sTmp;

#else	
		
		//sModeVFConfigFolder
//		sTmp.Format(_T("%s\\VF_TestConfig.ini"),CurrentSet->sModelIni.Left(CurrentSet->sModelIni.ReverseFind('\\')));
		sTmp.Format(_T("%s_VF.cfg"),CurrentSet->sModelIni.Left(CurrentSet->sModelIni.ReverseFind('.')));
		CurrentSet->sVFTestIni		= sTmp;
#endif


		CurrentSet->sModelFolder	= CurrentSet->sServerFolder + m_SelectChassisModelDlg.m_szModelInfoFilePath.Left(m_SelectChassisModelDlg.m_szModelInfoFilePath.ReverseFind('\\'));
		CurrentSet->sRefFolder		= CurrentSet->sRefRootFolder + m_SelectChassisModelDlg.m_szRefImgFolder;
		CurrentSet->sMaskFolder		= CurrentSet->sRefFolder + "\\Mask";
//AUTO
/*		sTmp = dlg.m_szSeqFilePath2;
		if(!sTmp.IsEmpty()){
			CurrentSet->sSeqPath_EM		= CurrentSet->sServerFolder + sTmp;
		} */
//		
		CurrentSet->sChassisName 	= m_SelectChassisModelDlg.m_sSelChassis;
		CurrentSet->sModelName 	= m_SelectChassisModelDlg.m_sSelModel;

		CurrentSet->sModelSuffixName = m_SelectChassisModelDlg.m_szModelSuffix;

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
			OpenModelIniFile(CurrentSet->sModelIni, m_szExePath);
			//FHD
			if(g_nGrabberType == FHD_GRABBER)
			{
				SetGrabInfo(&g_GrabImage);
			}//
			HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);
			HDMIGeneratorCtrl.SetHDCP_OnOff(CurrentSet->bHdmiHdcpControl);
			HDMIGeneratorCtrl.SetEDID_PassCheck(CurrentSet->bHdmiEdidControl);



			if(!CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(0);
			}
			if (CurrentSet->bUseDpg)
			{
				DPGeneratorCtrl.SetTime_Control(CurrentSet->nDP_TimeSel);
			}

			//CurrentSet->sEpiCfgIni = m_szExePath + "\\EPI_Config.ini";
			if (OpenEPI_ConfigFile(CurrentSet->sEpiCfgIni))
			{
				//CurrentSet->sDipStatus
			}
			else
			{
				CurrentSet->sDipStatus = "";
			}
			m_CEdit_EPI_CFG_SW.SetWindowText(CurrentSet->sDipStatus);
		}
		else
		{
		//	sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sModelIni);
		//	AfxMessageBox(sMsg);
				CModelInfoCreate dlg;
				
				dlg.m_sModelInfo_FileName = CurrentSet->sModelIni;
				dlg.DoModal();
		}

		if(FileExists(CurrentSet->sVFTestIni))
		{
			int nNoModel = 0;
			OpenVFTestFile(CurrentSet->sVFTestIni);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sVFTestIni);
			AfxMessageBox(sMsg);
			DeleteVFTestList();
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
		if(CurrentSet->sRemoconTitle.IsEmpty())
		{
			CurrentSet->sRemoconTitle = m_szExePath + "\\DFT_REMOTE.rmt";
		}

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
		//======================
		// Set HDCP-Key List File 
		//======================
		if(CurrentSet->sHdcpKeyListPath.IsEmpty())
		{
			CurrentSet->sHdcpKeyListPath = m_szExePath + "\\HDCP\\" + CurrentSet->sHDCP_Key_Class;
		}
		else{
			sTmp = CurrentSet->sHdcpKeyListPath;
			sKeyClass = sTmp.Right(sTmp.GetLength()-sTmp.ReverseFind('\\')-1);
			if(CurrentSet->sHDCP_Key_Class != sKeyClass)
			{
				CurrentSet->sHdcpKeyListPath = m_szExePath + "\\HDCP\\" + CurrentSet->sHDCP_Key_Class;
			}
		}

		SetI2cClkDelay();

//		UpdateInfo(MODEL, CurrentSet->sModelFolder);
		UpdateInfo(MODEL, CurrentSet->sModelName);
		UpdateInfo(CHASSIS, CurrentSet->sChassisName);
		UpdateInfo(REF, CurrentSet->sRefFolder);
		UpdateInfo(SEQ, CurrentSet->sSeqPath);

		ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);
		CtrlBatVerEdit.SetWindowText(CurrentSet->sBatVersion);

		UpdateToolOptionInfo(FALSE);

		InsertStepData2Grid(CurrentSet->nDisplayType);
		
		if(CurrentSet->bUseTVCommPort)
		{
			if(TVCommCtrl.m_bPortOpen)
			{
				TVCommCtrl.PortClose();
				Sleep(1000);
			}

			if(TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				TVCommCtrl.PortClose();
				Sleep(1000);
				TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
				if(TVCommCtrl.m_bPortOpen == FALSE)
				{
					CString szErrMsg;
					szErrMsg.Format("Failed to open COM port (%s)",CurrentSet->sTVComPort);
					AfxMessageBox(szErrMsg);
				}
			}
		}

		if ((g_nSysType == MANUAL_SYS)&&(m_nToolOptionMethod == 1)&& CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)
		{
			OnGrabStop();
			//pCmdUI->SetCheck(g_pView->m_bGrabThreadRunning);
			CurrentSet->sModelSuffixName = "";
			if (m_GetToolOptiondlg.DoModal() == IDOK)
			{
				ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);
				UpdateToolOptionInfo(TRUE);
				
			}
		}	
	}

	m_SelectChassisModelDlg.m_bActivate = 0;
	
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		StartLVDSGrabThread();
	}//
	else
	{
		StartLVDSGrabThread_FHD();
	}
	//FHD
	//if(bFlag) SetTimer(2, 1000, NULL);
	//
}

BOOL CDATsysView::Create(LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect, 
	CWnd* pParentWnd, 
	UINT nID,
	CCreateContext* pContext) 
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
	CString sKeyClass	= _T("");

	CurrentSet = new CEnvironmentData;

	//+move kwmoon 080821
	//================
	// Get Exe Folder 
	//================
	TCHAR szExeName[_MAX_PATH] = _T("");

	GetModuleFileName(NULL,szExeName,_MAX_PATH);

	m_szExePath = szExeName;

	m_szExePath = m_szExePath.Left(m_szExePath.ReverseFind('\\'));

	SetCurrentDirectory(m_szExePath);
	
	CString szLogFile = _T("");

	sTmp.Format(_T("%s\\TestResult"), m_szExePath);
	

	if(!FileExists(sTmp))   CreateFullPath(sTmp);
	LoadRegistrySetting(CurrentSet); 

	//=====================
	// Open Model.ini File 
	//=====================
	if(CurrentSet->sModelIni.IsEmpty())
	{
		//+change kwmoon 080904
	//	CreateModelIniFile(m_szExePath);
		CurrentSet->sModelIni = m_szExePath + "\\ModelInfo.Ini";
	}

	//+add kwmoon 080821 : Add FileExists Check
	if(FileExists(CurrentSet->sModelIni))
	{
		OpenModelIniFile(CurrentSet->sModelIni, m_szExePath);
		//+add PSH 081016
//		SetGrabInfo(&g_GrabImage);
	}
	else
	{
		sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sModelIni);
		AfxMessageBox(sMsg);
	}
	


	//=====================
	// Open VFTest.ini File 
	//=====================
#ifdef SM_MODIFY_CODE__
	/*
	if (CurrentSet->sVFTestIni.IsEmpty())
	{
		//CurrentSet->sVFTestIni = m_szExePath + "\\VF_TestInfo.Ini";
		sTmp.Format(_T("%s_VF.cfg"), gServer_Ctrl.m_szModelInfoFilePath.Left(gServer_Ctrl.m_szModelInfoFilePath.ReverseFind('.')));
		CurrentSet->sVFTestIni = CurrentSet->sModelVFConfigFolder + sTmp;
	}

	if (!CurrentSet->sVFTestIni.IsEmpty())
	{
		if (FileExists(CurrentSet->sVFTestIni))
		{
			int nNoModel = 0;
			OpenVFTestFile(CurrentSet->sVFTestIni);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sVFTestIni);
			AfxMessageBox(sMsg);
			DeleteVFTestList();
		}
	}

	*/
#else
/*
	if(CurrentSet->sVFTestIni.IsEmpty())
	{
		CurrentSet->sVFTestIni = m_szExePath + "\\VF_TestInfo.Ini";
	}

	if(!CurrentSet->sVFTestIni.IsEmpty())
	{
		if(FileExists(CurrentSet->sVFTestIni))
		{
			int nNoModel = 0;
			OpenVFTestFile(CurrentSet->sVFTestIni);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sVFTestIni);
			AfxMessageBox(sMsg);
		}
	}
*/

#endif
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

/*	//====================
	// Open Sequence File 
	//====================
	if(CurrentSet->sSeqPath.IsEmpty())
	{
	
	}
	else
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
*/	
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
	if(CurrentSet->sRemoconTitle.IsEmpty())
	{
		CurrentSet->sRemoconTitle = m_szExePath + "\\DFT_REMOTE.rmt";
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


	//======================
	// Set HDCP-Key List File 
	//======================
	if(CurrentSet->sHdcpKeyListPath.IsEmpty())
	{
		CurrentSet->sHdcpKeyListPath = m_szExePath + "\\HDCP\\" + CurrentSet->sHDCP_Key_Class;
	}
	else{
		sTmp = CurrentSet->sHdcpKeyListPath;
		sKeyClass = sTmp.Right(sTmp.GetLength()-sTmp.ReverseFind('\\')-1);
		if(CurrentSet->sHDCP_Key_Class != sKeyClass)
		{
			CurrentSet->sHdcpKeyListPath = m_szExePath + "\\HDCP\\" + CurrentSet->sHDCP_Key_Class;
		}
	}

	return 0;
}

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

	pCurrentSet->sVFTestIni				= pApp->GetProfileString(_T("File"), _T("Current VF_Test Ini"), _T(""));

	pCurrentSet->sModelListPath			= pApp->GetProfileString(_T("File"), _T("Current ModelList"), _T(""));

	pCurrentSet->sChassisName			= pApp->GetProfileString(_T("File"), _T("Current Chassis"), _T(""));
	pCurrentSet->sModelName				= pApp->GetProfileString(_T("File"), _T("Current Model"), _T(""));
	pCurrentSet->sModelSuffixName		= pApp->GetProfileString(_T("File"), _T("Current Model Suffix Name"), _T(""));

	pCurrentSet->sWorkOrderID			= pApp->GetProfileString(_T("GMES"), _T("Current WorkOrder ID"), _T(""));
	pCurrentSet->sOrganizationID		= pApp->GetProfileString(_T("GMES"), _T("Current Organization ID"), _T(""));
	pCurrentSet->sLineID				= pApp->GetProfileString(_T("GMES"), _T("Current Line ID"), _T(""));
	pCurrentSet->sEquipmentID			= pApp->GetProfileString(_T("GMES"), _T("Current Equipment ID"), _T(""));
	pCurrentSet->sOperationID			= pApp->GetProfileString(_T("GMES"), _T("Current Operation ID"), _T(""));
	pCurrentSet->sModelID				= pApp->GetProfileString(_T("GMES"), _T("Current Model ID"), _T(""));
	pCurrentSet->sModelName_GMES		= pApp->GetProfileString(_T("GMES"), _T("Current Model Name"), _T(""));
	pCurrentSet->sModelSuffix			= pApp->GetProfileString(_T("GMES"), _T("Current Model Suffix"), _T(""));

	pCurrentSet->bNgCountCheck			= pApp->GetProfileInt(_T("GMES"), _T("NG Count Check"), FALSE);
	
	pCurrentSet->bCheckGrabACPower = pApp->GetProfileInt(_T("Config"), _T("Reset Grabber Power Option"), FALSE);
	pCurrentSet->bCheckIFPackReset = pApp->GetProfileInt(_T("Config"), _T("Reset IF Pack Reset Option"), FALSE);
	pCurrentSet->bCheckGenderReset = pApp->GetProfileInt(_T("Config"), _T("Reset Gender Reset Option"), FALSE);
	pCurrentSet->bCheckRotateReset = pApp->GetProfileInt(_T("Config"), _T("Reset Rotate Reset Option"), FALSE);
//	pCurrentSet->bCheckImageFullReset = pApp->GetProfileInt(_T("Config"), _T("Image Full Reset Option"), FALSE);
	pCurrentSet->bCheckBatteryCheck = pApp->GetProfileInt(_T("Config"), _T("Battery Check Option"), FALSE);
	pCurrentSet->bCheckGrabberOnOffCheck = pApp->GetProfileInt(_T("Config"), _T("Grabber ON OFF Option"), FALSE);
	//pApp->WriteProfileInt(_T("Config"), _T(" Battery Check Option"), CurrentSet->bCheckBatteryCheck);
	pCurrentSet->bCheckUsePcbSensor = pApp->GetProfileInt(_T("Config"), _T("Use PCB Load Sensor"), FALSE);
	pCurrentSet->bCheckUsePcbSensor = 1;

	pCurrentSet->bFHDUHD_OLED_Pack_Reset = pApp->GetProfileInt(_T("Config"), _T("FHDUHD_OLED_PACK_RESET"), FALSE);
	pCurrentSet->bEpiPAckReset = pApp->GetProfileInt(_T("Config"), _T("EPI_PACK_RESET"), FALSE);
	pCurrentSet->bGrabBaseReset = pApp->GetProfileInt(_T("Config"), _T("GRAB_BASE_RESET"), FALSE);
	//m_Ini.SetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET", CurrentSet->bEpiPAckReset);
	//m_Ini.SetProfileInt(SPEC_CHECK_S, "GRAB_BASE_RESET", CurrentSet->bGrabBaseReset);
	//CurrentSet->bEpiPAckReset = m_Ini.GetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET", 0);
	//CurrentSet->bGrabBaseReset = m_Ini.GetProfileInt(SPEC_CHECK_S, "GRAB_BASE_RESET", 0);


	pCurrentSet->nRunType				= pApp->GetProfileInt(_T("Run"), _T("Run Type"), STOP);
	//AUTO
	if(g_nSysType == AUTO_SYS){
		pCurrentSet->nRunStartType			= pApp->GetProfileInt(_T("Run"), _T("Start Type"), AUTO_START);
	}
	//
	pCurrentSet->nDisplayType			= pApp->GetProfileInt(_T("Display"), _T("Display Type"), DETAILEDGRID);
	pCurrentSet->nPrintType				= pApp->GetProfileInt(_T("Display"), _T("PrintOut Type"), ERRORVIEW);
	pCurrentSet->bCommDisplay			= pApp->GetProfileInt(_T("Display"), _T("Comm Display"), TRUE);
	pCurrentSet->sViewerPath			= pApp->GetProfileString(_T("File"), _T("Current Viewer"), "C:\\Windows\\Notepad.exe");
	pCurrentSet->nLvdsWidth				= pApp->GetProfileInt(_T("Config"), _T("LVDS Grabber Width"), 1366);
	pCurrentSet->nLvdsHeight			= pApp->GetProfileInt(_T("Config"), _T("LVDS Grabber Height"), 768);
	pCurrentSet->nAnalogWidth			= pApp->GetProfileInt(_T("Config"), _T("Analog Grabber Width"), 640);
	pCurrentSet->nAnalogHeight			= pApp->GetProfileInt(_T("Config"), _T("Analog Grabber Height"), 480);
	pCurrentSet->bRobotSkipFFC = pApp->GetProfileInt(_T("Config"), _T("LVDS 30P Robot Skip FFC"), 1);
	pCurrentSet->bRobotSkipFFC = 1; // cys 200824 

	//pApp->WriteProfileInt(_T("Config"), _T("LVDS 30P Robot Skip FFC"), CurrentSet->bRobotSkipFFC);

	pCurrentSet->nAnalogType			= pApp->GetProfileInt(_T("Config"), _T("Analog Grabber Type"), 0);

	if(pCurrentSet->nAnalogWidth == 768){ 
		pCurrentSet->nAnalogWidth = 704;
		pCurrentSet->nAnalogHeight = 572;
	}

	if((pCurrentSet->nAnalogType == 0) && (pCurrentSet->nAnalogWidth == 704))
	{
		pCurrentSet->nAnalogType = 1;
	}

	if(pCurrentSet->nLvdsWidth	 == 0)	pCurrentSet->nLvdsWidth		= 1366;
	if(pCurrentSet->nLvdsHeight	 == 0)	pCurrentSet->nLvdsHeight	= 768;
	if(pCurrentSet->nAnalogWidth  == 0)	pCurrentSet->nAnalogWidth	= 640;
	if(pCurrentSet->nAnalogHeight == 0)	pCurrentSet->nAnalogHeight	= 480;

	pCurrentSet->sModelFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Folder"), "D:\\DFT");
	pCurrentSet->sGrabFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Grab Folder"), "D:\\DFT\\data");
	pCurrentSet->sRefFolder				= pApp->GetProfileString(_T("Folder"), _T("Current Ref Folder"), "D:\\DFT\\ref");
	pCurrentSet->sMaskFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Mask Folder"), "D:\\DFT\\ref\\Mask");

	pCurrentSet->sServerFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Server Folder"), "D:\\DFT");
	pCurrentSet->sRefRootFolder			= pApp->GetProfileString(_T("Folder"), _T("Current Reference Image Root Folder"), "D:\\DFT");
	pCurrentSet->sModelInfoFolder		= pApp->GetProfileString(_T("Folder"), _T("Current Model Info  Root Foler"), "D:\\DFT");

#ifdef SM_MODIFY_CODE__
	pCurrentSet->sModelVFConfigFolder = pApp->GetProfileString(_T("Folder"), _T("Current Model VF Config Root Foler"), "D:\\DFT");
#endif
	pCurrentSet->sParmAdjGrabFolder		= pApp->GetProfileString(_T("Folder"), _T("ParmAdj Current Grab Folder"), "D:\\DFT\\data");
	pCurrentSet->sParmAdjRefFolder		= pApp->GetProfileString(_T("Folder"), _T("ParmAdj Current Ref Folder"), "D:\\DFT\\ref");
	pCurrentSet->sParmAdjMaskFolder		= pApp->GetProfileString(_T("Folder"), _T("ParmAdj Current Mask Folder"), "D:\\DFT\\ref\\Mask");
//AUTO
	if(g_nSysType == AUTO_SYS){
		pCurrentSet->nJigUpType				= pApp->GetProfileInt(_T("Config"), _T("JigUpType"), 0); 
	}
	//

	pCurrentSet->nAcPowerOffType = pApp->GetProfileInt(_T("Config"), _T("AC Power Off Type"), 0);
	pCurrentSet->nAuto_Grab_Reset = pApp->GetProfileInt(_T("Config"), _T("Grabber Reset Type"), 0);
	//	pApp->WriteProfileInt(_T("Config"), _T("Grabber Reset Type"), CurrentSet->nAuto_Grab_Reset);//
#ifdef SM_MODIFY_CODE__
	if (g_nRunningProcessNo == 1)
	{
		pCurrentSet->sPatternComPort = pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "COM1");
		if (pCurrentSet->sPatternComPort == _T("")) pCurrentSet->sPatternComPort = "COM1";

		pCurrentSet->sAvSwitchBoxComPort = pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "COM2");
		if (pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "COM2";

		pCurrentSet->sTVComPort = pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "COM3");
		if (pCurrentSet->sTVComPort == _T("")) pCurrentSet->sTVComPort = "COM3";

		pCurrentSet->sScannerComPort = pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "COM4");
		if (pCurrentSet->sScannerComPort == _T("")) pCurrentSet->sScannerComPort = "COM4";
		pCurrentSet->sStartBoxComPort = pApp->GetProfileString(_T("Config"), _T("Start Box ComPort"), "COM4");
		if (pCurrentSet->sStartBoxComPort == _T("")) pCurrentSet->sStartBoxComPort = "COM4";

		pCurrentSet->sI2cAdcComPort = pApp->GetProfileString(_T("Config"), _T("I2c ADC ComPort"), "COM5");
		if (pCurrentSet->sI2cAdcComPort == _T("")) pCurrentSet->sI2cAdcComPort = "COM5";

		pCurrentSet->sHDMIComPort = pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "COM6");
		if (pCurrentSet->sHDMIComPort == _T("")) pCurrentSet->sHDMIComPort = "COM6";
		pCurrentSet->sDpgComPort = pApp->GetProfileString(_T("Config"), _T("DP Generator ComPort"), "COM6");
		if (pCurrentSet->sDpgComPort == _T("")) pCurrentSet->sDpgComPort = "COM6";

		pCurrentSet->sLNBComPort = pApp->GetProfileString(_T("Config"), _T("LNB ComPort"), "COM7");
		if (pCurrentSet->sLNBComPort == _T("")) pCurrentSet->sLNBComPort = "COM7";

		pCurrentSet->sAvcComPort = pApp->GetProfileString(_T("Config"), _T("AVC ComPort"), "COM8");
		if (pCurrentSet->sAvcComPort == _T("")) pCurrentSet->sAvcComPort = "COM8";

		//AUTO
		if (1)//g_nSysType == AUTO_SYS)
		{
			pCurrentSet->sJigComPort = pApp->GetProfileString(_T("Config"), _T("Jig ComPort"), "COM9");
			if (pCurrentSet->sJigComPort == _T("")) pCurrentSet->sJigComPort = "COM9";

			pCurrentSet->sVfmComPort = pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM10");
			if (pCurrentSet->sVfmComPort == _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM10";
		}
		else
		{
			//
			pCurrentSet->sVfmComPort = pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "COM9");
			if (pCurrentSet->sVfmComPort == _T("")) pCurrentSet->sVfmComPort = "COM9";
		}

	}
	else if (g_nRunningProcessNo == 2)
	{
		pCurrentSet->sPatternComPort = pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "\\\\.\\COM11");
		if (pCurrentSet->sPatternComPort == _T("")) pCurrentSet->sPatternComPort = "\\\\.\\COM11";

		pCurrentSet->sAvSwitchBoxComPort = pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "\\\\.\\COM12");
		if (pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "\\\\.\\COM12";

		pCurrentSet->sTVComPort = pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "\\\\.\\COM13");
		if (pCurrentSet->sTVComPort == _T("")) pCurrentSet->sTVComPort = "\\\\.\\COM13";

		pCurrentSet->sScannerComPort = pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "\\\\.\\COM14");
		if (pCurrentSet->sScannerComPort == _T("")) pCurrentSet->sScannerComPort = "\\\\.\\COM14";
		pCurrentSet->sStartBoxComPort = pApp->GetProfileString(_T("Config"), _T("Start Box ComPort"), "\\\\.\\COM14");
		if (pCurrentSet->sStartBoxComPort == _T("")) pCurrentSet->sStartBoxComPort = "\\\\.\\COM14";

		pCurrentSet->sI2cAdcComPort = pApp->GetProfileString(_T("Config"), _T("I2c ADC ComPort"), "\\\\.\\COM15");
		if (pCurrentSet->sI2cAdcComPort == _T("")) pCurrentSet->sI2cAdcComPort = "\\\\.\\COM15";

		pCurrentSet->sHDMIComPort = pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "\\\\.\\COM16");
		if (pCurrentSet->sHDMIComPort == _T("")) pCurrentSet->sHDMIComPort = "\\\\.\\COM16";
		pCurrentSet->sDpgComPort = pApp->GetProfileString(_T("Config"), _T("DP Generator ComPort"), "\\\\.\\COM16");
		if (pCurrentSet->sDpgComPort == _T("")) pCurrentSet->sDpgComPort = "\\\\.\\COM16";

		pCurrentSet->sLNBComPort = pApp->GetProfileString(_T("Config"), _T("LNB ComPort"), "\\\\.\\COM17");
		if (pCurrentSet->sLNBComPort == _T("")) pCurrentSet->sLNBComPort = "\\\\.\\COM17";

		pCurrentSet->sAvcComPort = pApp->GetProfileString(_T("Config"), _T("AVC ComPort"), "\\\\.\\COM18");
		if (pCurrentSet->sAvcComPort == _T("")) pCurrentSet->sAvcComPort = "\\\\.\\COM18";

		//AUTO
		if (1)//g_nSysType == AUTO_SYS) 
		{
			pCurrentSet->sJigComPort = pApp->GetProfileString(_T("Config"), _T("Jig ComPort"), "\\\\.\\COM19");
			if (pCurrentSet->sJigComPort == _T("")) pCurrentSet->sJigComPort = "\\\\.\\COM19";
			pCurrentSet->sVfmComPort = pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM20");
			if (pCurrentSet->sVfmComPort == _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM20";
		}
		else
		{
			//	
			pCurrentSet->sVfmComPort = pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM19");
			if (pCurrentSet->sVfmComPort == _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM19";
		}

	}
	else
	{
		pCurrentSet->sPatternComPort = pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "\\\\.\\COM21");
		if (pCurrentSet->sPatternComPort == _T("")) pCurrentSet->sPatternComPort = "\\\\.\\COM21";

		pCurrentSet->sAvSwitchBoxComPort = pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "\\\\.\\COM22");
		if (pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "\\\\.\\COM22";

		pCurrentSet->sTVComPort = pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "\\\\.\\COM23");
		if (pCurrentSet->sTVComPort == _T("")) pCurrentSet->sTVComPort = "\\\\.\\COM23";

		pCurrentSet->sScannerComPort = pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "\\\\.\\COM24");
		if (pCurrentSet->sScannerComPort == _T("")) pCurrentSet->sScannerComPort = "\\\\.\\COM24";

		pCurrentSet->sStartBoxComPort = pApp->GetProfileString(_T("Config"), _T("Start Box ComPort"), "\\\\.\\COM24");
		if (pCurrentSet->sStartBoxComPort == _T("")) pCurrentSet->sStartBoxComPort = "\\\\.\\COM24";

		pCurrentSet->sI2cAdcComPort = pApp->GetProfileString(_T("Config"), _T("I2c ADC ComPort"), "\\\\.\\COM25");
		if (pCurrentSet->sI2cAdcComPort == _T("")) pCurrentSet->sI2cAdcComPort = "\\\\.\\COM25";

		pCurrentSet->sHDMIComPort = pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "\\\\.\\COM26");
		if (pCurrentSet->sHDMIComPort == _T("")) pCurrentSet->sHDMIComPort = "\\\\.\\COM26";
		pCurrentSet->sDpgComPort = pApp->GetProfileString(_T("Config"), _T("DP Generator ComPort"), "\\\\.\\COM26");
		if (pCurrentSet->sDpgComPort == _T("")) pCurrentSet->sDpgComPort = "\\\\.\\COM26";

		pCurrentSet->sAvcComPort = pApp->GetProfileString(_T("Config"), _T("AVC ComPort"), "\\\\.\\COM27");
		if (pCurrentSet->sAvcComPort == _T("")) pCurrentSet->sAvcComPort = "\\\\.\\COM27";

		pCurrentSet->sLNBComPort = pApp->GetProfileString(_T("Config"), _T("LNB ComPort"), "\\\\.\\COM28");
		if (pCurrentSet->sLNBComPort == _T("")) pCurrentSet->sLNBComPort = "\\\\.\\COM28";
		//AUTO		
		if (1)//g_nSysType == AUTO_SYS)
 {
			pCurrentSet->sJigComPort = pApp->GetProfileString(_T("Config"), _T("Jig ComPort"), "\\\\.\\COM29");
			if (pCurrentSet->sJigComPort == _T("")) pCurrentSet->sJigComPort = "\\\\.\\COM29";	
			
			pCurrentSet->sVfmComPort = pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM30");
			if (pCurrentSet->sVfmComPort == _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM30";

		}
		else
		{
			//	
			pCurrentSet->sVfmComPort = pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM29");
			if (pCurrentSet->sVfmComPort == _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM29";
		}
	}
#else

	if(g_nRunningProcessNo == 1)
	{
		pCurrentSet->sPatternComPort		= pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "COM1");
		if(pCurrentSet->sPatternComPort		== _T("")) pCurrentSet->sPatternComPort = "COM1";
								
		pCurrentSet->sAvSwitchBoxComPort	= pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "COM4");
		if(pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "COM4";

		pCurrentSet->sTVComPort				= pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "COM7");
		if(pCurrentSet->sTVComPort			== _T("")) pCurrentSet->sTVComPort = "COM7";

		pCurrentSet->sScannerComPort		= pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "\\\\.\\COM10");
		if(pCurrentSet->sScannerComPort		== _T("")) pCurrentSet->sScannerComPort = "\\\\.\\COM10";
	
		pCurrentSet->sI2cAdcComPort			= pApp->GetProfileString(_T("Config"), _T("I2c ADC ComPort"), "\\\\.\\COM13");
		if(pCurrentSet->sI2cAdcComPort		== _T("")) pCurrentSet->sI2cAdcComPort = "\\\\.\\COM13";

		pCurrentSet->sHDMIComPort		= pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "\\\\.\\COM16");
		if(pCurrentSet->sHDMIComPort		== _T("")) pCurrentSet->sHDMIComPort = "\\\\.\\COM16";

		pCurrentSet->sAvcComPort		= pApp->GetProfileString(_T("Config"), _T("AVC ComPort"), "\\\\.\\COM19");
		if(pCurrentSet->sAvcComPort		== _T("")) pCurrentSet->sAvcComPort = "\\\\.\\COM19";

		pCurrentSet->sLNBComPort		= pApp->GetProfileString(_T("Config"), _T("LNB ComPort"), "\\\\.\\COM19");
		if(pCurrentSet->sLNBComPort		== _T("")) pCurrentSet->sLNBComPort = "\\\\.\\COM19";
//AUTO
		if(g_nSysType == AUTO_SYS){
			pCurrentSet->sJigComPort		= pApp->GetProfileString(_T("Config"), _T("Jig ComPort"), "\\\\.\\COM15");
			if(pCurrentSet->sJigComPort		== _T("")) pCurrentSet->sJigComPort = "\\\\.\\COM15";
		}
//
		pCurrentSet->sVfmComPort		= pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM19");
		if(pCurrentSet->sVfmComPort		== _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM19";

	}
	else if(g_nRunningProcessNo == 2)
	{
		pCurrentSet->sPatternComPort		= pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "COM2");
		if(pCurrentSet->sPatternComPort		== _T("")) pCurrentSet->sPatternComPort = "COM2";
								
		pCurrentSet->sAvSwitchBoxComPort	= pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "COM5");
		if(pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "COM5";

		pCurrentSet->sTVComPort				= pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "COM8");
		if(pCurrentSet->sTVComPort			== _T("")) pCurrentSet->sTVComPort = "COM8";

		pCurrentSet->sScannerComPort		= pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "\\\\.\\COM11");
		if(pCurrentSet->sScannerComPort		== _T("")) pCurrentSet->sScannerComPort = "\\\\.\\COM11";

		pCurrentSet->sI2cAdcComPort			= pApp->GetProfileString(_T("Config"), _T("I2c ADC ComPort"), "\\\\.\\COM14");
		if(pCurrentSet->sI2cAdcComPort		== _T("")) pCurrentSet->sI2cAdcComPort = "\\\\.\\COM14";

		pCurrentSet->sHDMIComPort		= pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "\\\\.\\COM17");
		if(pCurrentSet->sHDMIComPort		== _T("")) pCurrentSet->sHDMIComPort = "\\\\.\\COM17";

		pCurrentSet->sAvcComPort		= pApp->GetProfileString(_T("Config"), _T("AVC ComPort"), "\\\\.\\COM20");
		if(pCurrentSet->sAvcComPort		== _T("")) pCurrentSet->sAvcComPort = "\\\\.\\COM20";
		
		pCurrentSet->sLNBComPort		= pApp->GetProfileString(_T("Config"), _T("LNB ComPort"), "\\\\.\\COM20");
		if(pCurrentSet->sLNBComPort		== _T("")) pCurrentSet->sLNBComPort = "\\\\.\\COM20";
//AUTO
		if(g_nSysType == AUTO_SYS){
			pCurrentSet->sJigComPort		= pApp->GetProfileString(_T("Config"), _T("Jig ComPort"), "\\\\.\\COM16");
			if(pCurrentSet->sJigComPort		== _T("")) pCurrentSet->sJigComPort = "\\\\.\\COM16";
		}
//	
		pCurrentSet->sVfmComPort		= pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM20");
		if(pCurrentSet->sVfmComPort		== _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM20";

	}
	else
	{
		pCurrentSet->sPatternComPort		= pApp->GetProfileString(_T("Config"), _T("Pattern Generator ComPort"), "COM3");
		if(pCurrentSet->sPatternComPort		== _T("")) pCurrentSet->sPatternComPort = "COM3";
								
		pCurrentSet->sAvSwitchBoxComPort	= pApp->GetProfileString(_T("Config"), _T("AV SwitchBox ComPort"), "COM6");
		if(pCurrentSet->sAvSwitchBoxComPort == _T("")) pCurrentSet->sAvSwitchBoxComPort = "COM6";

		pCurrentSet->sTVComPort				= pApp->GetProfileString(_T("Config"), _T("TV ComPort"), "COM9");
		if(pCurrentSet->sTVComPort			== _T("")) pCurrentSet->sTVComPort = "COM9";

		pCurrentSet->sScannerComPort		= pApp->GetProfileString(_T("Config"), _T("Scanner ComPort"), "\\\\.\\COM12");
		if(pCurrentSet->sScannerComPort		== _T("")) pCurrentSet->sScannerComPort = "\\\\.\\COM12";

		pCurrentSet->sI2cAdcComPort			= pApp->GetProfileString(_T("Config"), _T("I2c ADC ComPort"), "\\\\.\\COM15");
		if(pCurrentSet->sI2cAdcComPort		== _T("")) pCurrentSet->sI2cAdcComPort = "\\\\.\\COM15";

		pCurrentSet->sHDMIComPort		= pApp->GetProfileString(_T("Config"), _T("HDMI Generator ComPort"), "\\\\.\\COM18");
		if(pCurrentSet->sHDMIComPort		== _T("")) pCurrentSet->sHDMIComPort = "\\\\.\\COM18";
		
		pCurrentSet->sAvcComPort		= pApp->GetProfileString(_T("Config"), _T("AVC ComPort"), "\\\\.\\COM21");
		if(pCurrentSet->sAvcComPort		== _T("")) pCurrentSet->sAvcComPort = "\\\\.\\COM21";
		
		pCurrentSet->sLNBComPort		= pApp->GetProfileString(_T("Config"), _T("LNB ComPort"), "\\\\.\\COM21");
		if(pCurrentSet->sLNBComPort		== _T("")) pCurrentSet->sLNBComPort = "\\\\.\\COM21";
//AUTO		
		if(g_nSysType == AUTO_SYS){
			pCurrentSet->sJigComPort		= pApp->GetProfileString(_T("Config"), _T("Jig ComPort"), "\\\\.\\COM16");
			if(pCurrentSet->sJigComPort		== _T("")) pCurrentSet->sJigComPort = "\\\\.\\COM16";
		}
//	
		pCurrentSet->sVfmComPort		= pApp->GetProfileString(_T("Config"), _T("VFM ComPort"), "\\\\.\\COM21");
		if(pCurrentSet->sVfmComPort		== _T("")) pCurrentSet->sVfmComPort = "\\\\.\\COM21";

	}
#endif
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

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("StartBox Baud Rate"), "9600");
	pCurrentSet->wStartBoxBaudRate		= (DWORD)(atoi(sBaudRate));

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("I2c ADC Baud Rate"), "115200");
	pCurrentSet->wI2cAdcBaudRate		= (DWORD)(atoi(sBaudRate));

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("HDMI Generator Baud Rate"), "19200");
	pCurrentSet->wHDMIBaudRate			= (DWORD)(atoi(sBaudRate));
	//pCurrentSet->nHDMIGenType			= pApp->GetProfileInt(_T("Config"), _T("HDMI Generator Type"), 0);
	if (g_nGrabberType == FHD_GRABBER)
	{
		pCurrentSet->nHDMIGenType = 1;
	}
	else
	{
		pCurrentSet->nHDMIGenType = 0;
	}
	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("AVC Baud Rate"), "19200");
	pCurrentSet->wAvcBaudRate			= (DWORD)(atoi(sBaudRate));

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("LNB Baud Rate"), "9600");
	pCurrentSet->wLNBBaudRate			= (DWORD)(atoi(sBaudRate));
//AUTO
	if(1)//g_nSysType == AUTO_SYS)
	{
		sBaudRate							= pApp->GetProfileString(_T("Config"), _T("Jig Baud Rate"), "9600");
		pCurrentSet->wJigBaudRate			= (DWORD)(atoi(sBaudRate));
	}
//
	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("VFM Baud Rate"), "19200");
	pCurrentSet->wVfmBaudRate			= (DWORD)(atoi(sBaudRate));

	sBaudRate							= pApp->GetProfileString(_T("Config"), _T("DPG Baud Rate"), "19200");
	pCurrentSet->wDpgBaudRate = (DWORD)(atoi(sBaudRate));

	


	pCurrentSet->bUsePatternGen			= pApp->GetProfileInt(_T("Config"), _T("Use Pattern Generator"), 1);
	pCurrentSet->bUseTVCommPort			= pApp->GetProfileInt(_T("Config"), _T("Use TVComm Port"), 1);
	pCurrentSet->bUseAVSwitchBox		= pApp->GetProfileInt(_T("Config"), _T("Use AVSwitchingBox"), 1);
	pCurrentSet->bUseScanner			= pApp->GetProfileInt(_T("Config"), _T("Use Scanner"), 1);
	pCurrentSet->bUseStartBox			= pApp->GetProfileInt(_T("Config"), _T("Use Start Box"), 1);
	if (g_nSysRobotType == AUTO_SYS)
	{
		pCurrentSet->bUseScanner = FALSE;
		pCurrentSet->bUseStartBox = FALSE;
	}
	else if(g_nUseNoScanType == FALSE)
	{
		pCurrentSet->bUseScanner = TRUE;
		pCurrentSet->bUseStartBox = FALSE;
	}
	pCurrentSet->bUseHDMIGen			= pApp->GetProfileInt(_T("Config"), _T("Use HDMI Generator"), 1);
	pCurrentSet->bUseAVC				= pApp->GetProfileInt(_T("Config"), _T("Use AVC"), 0);
	pCurrentSet->bUseLNB				= pApp->GetProfileInt(_T("Config"), _T("Use LNB"), 0);
//AUTO	
	if(1)//g_nSysType == AUTO_SYS)
	{
		pCurrentSet->bUseJig				= pApp->GetProfileInt(_T("Config"), _T("Use Jig"), 0);
	}
//
	pCurrentSet->bUseVfm				= pApp->GetProfileInt(_T("Config"), _T("Use VFM"), 0);
	pCurrentSet->bUseDpg = pApp->GetProfileInt(_T("Config"), _T("Use DPG"), 0);



	pCurrentSet->nAdcType				= pApp->GetProfileInt(_T("Config"), _T("ADC Type"), 0); // 0 : RS232C, 1 : I2C

	pCurrentSet->bSaveGrabImg			= pApp->GetProfileInt(_T("Config"), _T("Auto Save Grab Image"), 0);// 0 : NG, 1 : ALL
	pCurrentSet->bSaveProcImg			= TRUE; // pApp->GetProfileInt(_T("Config"), _T("Auto Save Processed Image"), 1);
	pCurrentSet->bSaveCaptionTestImg	= pApp->GetProfileInt(_T("Config"), _T("Save Caption Test Image"), 0);
	pCurrentSet->bSaveMovingPicTestImg	= pApp->GetProfileInt(_T("Config"), _T("Save Moving Pic Test Image"), 0);
	pCurrentSet->bSaveIntermediateImage	= pApp->GetProfileInt(_T("Config"), _T("Save Intermediate Image"), 0);
	pCurrentSet->bSaveProcessingTimeData = pApp->GetProfileInt(_T("Config"), _T("Save Processing Time Data"), 0);

	pCurrentSet->bSaveReviewData		= pApp->GetProfileInt(_T("Config"), _T("Save Review Data"), 0);
	pCurrentSet->bSaveDetailResultData	= pApp->GetProfileInt(_T("Config"), _T("Save Detail Result Data"), 0);

	pCurrentSet->nAudioSource			= pApp->GetProfileInt(_T("Config"), _T("Audio Input Source"), 0);

	pCurrentSet->bSaveResult2Txt		= FALSE; // pApp->GetProfileInt(_T("Config"), _T("Save Result to Text File"), 1);
	pCurrentSet->bSaveResult2Html		= TRUE;  // pApp->GetProfileInt(_T("Config"), _T("Save Result to Html File"), 1);
	pCurrentSet->bSaveResult2Csv		= FALSE; // pApp->GetProfileInt(_T("Config"), _T("Save Result to Csv File"), 1);
	//-

	pCurrentSet->nNoRepeatExecution		= pApp->GetProfileInt(_T("Config"), _T("No of repeat execution"), 1);

	pCurrentSet->nNoFrameForRefImageCreation	= pApp->GetProfileInt(_T("Config"), _T("No of frames for refimage creation"), 1);

	pCurrentSet->nPixelInMiddleRange	=  pApp->GetProfileInt(_T("Config"), _T("Percentage of pixels in middle range"), 85);
	pCurrentSet->nNonBlackPixel			=  pApp->GetProfileInt(_T("Config"), _T("Percentage of non-black pixels"), 30);
	pCurrentSet->nNoUsedColors			=  pApp->GetProfileInt(_T("Config"), _T("No Used Colors"), 5000);
	pCurrentSet->nDelayPercentage		= pApp->GetProfileInt(_T("Config"), _T("Delay Percentage"), 100);

	pCurrentSet->nLVDS_Mode				= pApp->GetProfileInt(_T("Config"), _T("LVDS Mode"), 0);
    
	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		pCurrentSet->nUranousMode			= pApp->GetProfileInt(_T("Config"), _T("Uranous Grabber Mode"), NS_SINGLE8);
		pCurrentSet->bOddDE_Only			= (BOOL)pApp->GetProfileInt(_T("Config"), _T("LVDS OddDE Only"), 0);
		if(pCurrentSet->nUranousMode == 0)	pCurrentSet->nUranousMode = NS_SINGLE;
	}//
	pCurrentSet->nBitCount				= pApp->GetProfileInt(_T("Config"), _T("Color Bit Count"), 24);
	
	pCurrentSet->nNoRetry				= pApp->GetProfileInt(_T("Config"), _T("No of Internal Retry"), 1);
	
	pCurrentSet->nGrabCheckArea			= pApp->GetProfileInt(_T("Config"), _T("Grab Image Check Area"), 7);

	pCurrentSet->bTVCommPortEcho		= (BOOL)pApp->GetProfileInt(_T("Config"), _T("TVComm Port Echo"), 0);

	pCurrentSet->sHDMIRoiShape			= pApp->GetProfileString(_T("Config"), _T("HDMI Roi Shape"), "VERTICAL");
	pCurrentSet->sLineCode				= pApp->GetProfileString(_T("DataBase"), _T("Line_Code"), _T(""));
	pCurrentSet->nLineNo				= pApp->GetProfileInt(_T("DataBase"), _T("Line Number"), 0);
	pCurrentSet->nSystemNo				= pApp->GetProfileInt(_T("DataBase"), _T("System Number"), 0);
	pCurrentSet->nAuthority				= pApp->GetProfileInt(_T("DataBase"), _T("Authority"), 0);
	pCurrentSet->strPassword			= pApp->GetProfileString(_T("DataBase"), _T("PassWord"), _T("DFT"));

	pCurrentSet->bUseGmes				= pApp->GetProfileInt(_T("Config"), _T("Use GMES"), 1);
	pCurrentSet->bUsePLCRobot			= pApp->GetProfileInt(_T("Config"), _T("Use PLC"), 1);
	pCurrentSet->bUseServerCtrlConnect	= pApp->GetProfileInt(_T("Config"), _T("Use SERVER Control"), 0);
	if (g_nSysRobotType == AUTO_SYS)
	{
		pCurrentSet->bUsePLCRobot = TRUE;
//		pCurrentSet->bUseServerCtrlConnect  = TRUE;
	}
	else
	{
		pCurrentSet->bUsePLCRobot = FALSE; 
//		pCurrentSet->bUseServerCtrlConnect = FALSE;
	}

	pCurrentSet->strGmesIP				= pApp->GetProfileString(_T("DataBase"), _T("GMES Host IP"), _T("156.147.82.152"));
	pCurrentSet->nGmesPort			= pApp->GetProfileInt(_T("DataBase"), _T("GMES Host Port"), 40201);

	pCurrentSet->strPLC_IP				= pApp->GetProfileString(_T("DataBase"), _T("PLC Host IP"), _T("192.168.0.2"));
	pCurrentSet->nPLC_Port			= pApp->GetProfileInt(_T("DataBase"), _T("PLC Host Port"), 2000);
	pCurrentSet->nDFT_No			= pApp->GetProfileInt(_T("DataBase"), _T("PLC DFT Number"), 0);

	pCurrentSet->strMonitorServer_IP = pApp->GetProfileString(_T("DataBase"), _T("Monitor Server IP"), _T("192.168.0.3"));
	pCurrentSet->nMonitorServer_Port			= pApp->GetProfileInt(_T("DataBase"), _T("Monitor Server Port"), 8001);
	
	pCurrentSet->bUploadMes				= (BOOL)pApp->GetProfileInt(_T("DataBase Upload"), _T("UPLOAD MES DATA"), 0); 
	pCurrentSet->bAutoDataDelete		= (BOOL)pApp->GetProfileInt(_T("DataBase"), _T("AUTO UPLOAD MES DATA"), 0); 

	pCurrentSet->nAudioCheckMethod		= pApp->GetProfileInt(_T("Config"), _T("AudioCheckMethod"), 1);

	//godtech 221104
	pCurrentSet->nDataSavePeriod = 180;//  pApp->GetProfileInt(_T("Config"), _T("Data Save Period"), 60);
	pCurrentSet->bAutoDataDelete = 1;// pApp->GetProfileInt(_T("Config"), _T("Auto Data Delete"), 1);

	pCurrentSet->nToolOptionSetMethod	= pApp->GetProfileInt(_T("Config"), _T("Tool Option Set Method"), 0);


	if(pCurrentSet->nToolOptionSetMethod < 0 || pCurrentSet->nToolOptionSetMethod > 2){
		pCurrentSet->nToolOptionSetMethod = 0;
	}

	pCurrentSet->bHdmiOutControl = 0;// pApp->GetProfileInt(_T("Config"), _T("HDMI Out Port Control"), 0);
	pCurrentSet->nHDMI1_Port			= pApp->GetProfileInt(_T("Config"), _T("HDMI1 Port"), 1);
	pCurrentSet->nHDMI2_Port			= pApp->GetProfileInt(_T("Config"), _T("HDMI2 Port"), 2);
	pCurrentSet->nHDMI3_Port			= pApp->GetProfileInt(_T("Config"), _T("HDMI3 Port"), 3);
	pCurrentSet->nHDMI4_Port			= pApp->GetProfileInt(_T("Config"), _T("HDMI4 Port"), 4);

	pCurrentSet->bHdmiCecControl = pApp->GetProfileInt(_T("Config"), _T("HDMI CEC On/Off Control"), 0);
	pCurrentSet->bHdmiHdcpControl = pApp->GetProfileInt(_T("Config"), _T("HDMI HDCP On/Off Control"), 1);
	pCurrentSet->bHdmiEdidControl = pApp->GetProfileInt(_T("Config"), _T("HDMI EDID On/Off Control"), 1);
	pCurrentSet->bAutoRun_DFT2			= pApp->GetProfileInt(_T("Config"), _T("Auto Run DFT2(3)"), 1);

	pCurrentSet->bPJT_GrabDisable			= pApp->GetProfileInt(_T("Config"), _T("PJT Grab Disable"), 0);
	//pCurrentSet->bPJT_GrabDisable = 0;
}

void CDATsysView::OnDestroy() 
{
//	CFormView::OnDestroy();

	CString sTmp				 = _T("");
	CString sMsg				 = _T("");
	CString sBaudRate		 	 = _T("");
	int nCount = 0;
//	DWORD	dwStatus;
	DWORD dwExitCode = 0;
	DWORD dwThreadResult = 0;
	CloseTimeDataFile();
	if (gPLC_Ctrl.m_bPLCCom)
	{
		gPLC_Ctrl.CommandClear();
	}
//AUTO
	if(g_nSysType == AUTO_SYS){
		gJigCtrl.Set_ACPower(0);
		
		SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
	}
//
	else{
		Set_Relay(0);
	}

	if(gGmesCtrl.m_bGmesCom){
		gGmesCtrl.SoketCom_Close();
	}
	// add 090902
	if(g_nRunningProcessNo == 1){
		removehook1();
	}
	else if(g_nRunningProcessNo == 2){
		removehook2();
	}
	else{
		removehook3();
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


	KillTimer(TIMER_UPDATE_STATUS_INFO);
	KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);

	if(g_nGrabberType == FHD_GRABBER)
	{
		if(m_bContinuousLVDSGrabRunning)
		{
			KillTimer(TIMER_CONTINUOUS_LVDS_GRAB); 
		}
	}
//	UserControlInit(FALSE);

	if(m_bResultDlgActivate)
	{
		delete m_ResultDlg;
	}

	if(CurrentSet->bUseScanner)
	{
		ScannerCtrl.PortClose(); Sleep(500);
	}
	if(CurrentSet->bUseStartBox)
	{
		ScannerCtrl.PortClose(); Sleep(500);
	}
	//====================
	// Close Sound Driver
	//====================
	AudioMeasureStop();

	//=======================
	// Stop Auto-Grab Thread
	//=======================
//	StopLVDSGrabThread();

	if(m_bGrabThreadRunning)
	{
		m_bStopLVDSThread = TRUE; 

		dwThreadResult = WaitForSingleObject(m_hGrabThreadKillEvent,2000);

		if(dwThreadResult != WAIT_OBJECT_0)
		{
			if(m_pGrabImageThread) TerminateThread(m_pGrabImageThread->m_hThread,dwExitCode);
		}
		m_pGrabImageThread = NULL; 
		m_bGrabThreadRunning = FALSE;

	}

	if(m_pGrabImageThread) TerminateThread(m_pGrabImageThread->m_hThread,dwExitCode);
	if(m_pTestThread)	   TerminateThread(m_pTestThread->m_hThread,dwExitCode);
	if(m_pVFTestThread)	TerminateThread(m_pVFTestThread->m_hThread,dwExitCode);

//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(g_GrabDisplayImage.m_pImageData != NULL)
		{
				delete[] g_GrabDisplayImage.m_pImageData; 
				g_GrabDisplayImage.m_pImageData = NULL;
		}

		if(g_GrabImage.m_pImageData != NULL)
		{
				delete[] g_GrabDisplayImage.m_pImageData;
				g_GrabImage.m_pImageData = NULL;
		}
	}//
	
	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		if(m_hImageDataFileMapping != NULL)
		{
			if(g_GrabImage.m_pImageData != NULL)
			{
				UnmapViewOfFile(g_GrabImage.m_pImageData); g_GrabImage.m_pImageData = NULL;
			}
			CloseHandle(m_hImageDataFileMapping); m_hImageDataFileMapping = NULL;
		}
	
		if(m_hImageInfoFileMapping != NULL)
		{
			if(m_pImageInfo != NULL)
			{
				UnmapViewOfFile(m_pImageInfo); m_pImageInfo = NULL;
			}
			CloseHandle(m_hImageInfoFileMapping); m_hImageInfoFileMapping = NULL;
		}
	} //
/*
	if(m_hDimmingTestFileMapping != NULL)
	{
		if(m_pDimmingTest != NULL)
		{
			UnmapViewOfFile(m_pDimmingTest); m_pDimmingTest = NULL;
		}
		CloseHandle(m_hDimmingTestFileMapping); m_hDimmingTestFileMapping = NULL;
	}
	*/
//	
	if (g_nUseNoScanType == FALSE)// if (g_nSysRobotType != AUTO_SYS)//godtech 190531
	{
		if (m_hMhlCtrlFileMapping != NULL)
		{
			if (m_pMhlCtrl != NULL)
			{
				UnmapViewOfFile(m_pMhlCtrl); m_pMhlCtrl = NULL;
			}
			CloseHandle(m_hMhlCtrlFileMapping); m_hMhlCtrlFileMapping = NULL;
		}
		//AUTO
		if (g_nSysType == AUTO_SYS) {
			//if (g_nSysRobotType != AUTO_SYS)
			{
				if (m_hBarcodeDataFileMapping != NULL)
				{
					if (m_pBarcodeData != NULL)
					{
						UnmapViewOfFile(m_pBarcodeData); m_pBarcodeData = NULL;
					}
					CloseHandle(m_hBarcodeDataFileMapping); m_hBarcodeDataFileMapping = NULL;
				}
			}
			//if (m_hDaqDataFileMapping != NULL)
			//{
			//	if (m_pDaqData != NULL)
			//	{
			//		UnmapViewOfFile(m_pDaqData); m_pDaqData = NULL;
			//	}
			//	CloseHandle(m_hDaqDataFileMapping); m_hDaqDataFileMapping = NULL;
			//}

		}//
		else {
			if (m_hUsbDioCtrlFileMapping != NULL)
			{
				if (m_pUsbDio_Control != NULL)
				{
					UnmapViewOfFile(m_pUsbDio_Control); m_pUsbDio_Control = NULL;
				}
				CloseHandle(m_hUsbDioCtrlFileMapping); m_hUsbDioCtrlFileMapping = NULL;
			}
			CloseHandle(hUsbCtrl);
			hUsbCtrl = NULL;
			CloseHandle(hUsbCtrl_Complete);
			hUsbCtrl_Complete = NULL;
		}
		CloseHandle(hMhlCtrl);
		hMhlCtrl = NULL;
		CloseHandle(hMhlCtrl_Complete);
		hMhlCtrl_Complete = NULL;
	}

	if(g_nGrabberType == UHD_GRABBER)
	{
		CloseHandle(hGrabEvent);
		hGrabEvent = NULL;
		CloseHandle(hGrabEvent_Complete);
		hGrabEvent_Complete = NULL;
	}


	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		for(int x = 0; x < 12; x++)
		{
			CloseHandle(hClientEvents[x]);
			hClientEvents[x] = NULL;
		}
		for(int y = 0; y < 9; y++)
		{
			CloseHandle(hServerEvents[y]);
			hServerEvents[y] = NULL;
		}
	}

	
	if(m_hGrabThreadKillEvent != NULL)
	{
		CloseHandle(m_hGrabThreadKillEvent); m_hGrabThreadKillEvent = NULL;
	}

	if(m_hVfThreadKillEvent != NULL)
	{
		CloseHandle(m_hVfThreadKillEvent); m_hVfThreadKillEvent = NULL;
	}

//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(m_bGrabConnection){
			GrabberClose();
		}
	}//

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

	// Close DMM Com Port
//	if(DMM.m_bDMM == TRUE)
//	{
//		DMM.CloseComm();
//	}
	
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
		for(int nTmp = 1; nTmp <= MAX_DIO_PORT; nTmp++)
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
	if(I2cAdcCtrl.m_bPortOpen == TRUE)
	{
		I2cAdcCtrl.PortClose();
	}
	if(gLNBDetectorCtrl.m_bPortOpen)		gLNBDetectorCtrl.PortClose();



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

	CFormView::OnDestroy();
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

	pApp->WriteProfileString(_T("File"), _T("Current VF_Test Ini"),			CurrentSet->sVFTestIni);

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

	pApp->WriteProfileInt(_T("GMES"), _T("NG Count Check"),					CurrentSet->bNgCountCheck);

	pApp->WriteProfileInt(_T("Config"), _T("Reset Grabber Power Option"),	CurrentSet->bCheckGrabACPower);
	pApp->WriteProfileInt(_T("Config"), _T("Reset IF Pack Reset Option"), CurrentSet->bCheckIFPackReset);
	pApp->WriteProfileInt(_T("Config"), _T("Reset Gender Reset Option"), CurrentSet->bCheckGenderReset);
	pApp->WriteProfileInt(_T("Config"), _T("Reset Rotate Reset Option"), CurrentSet->bCheckRotateReset);
	//pApp->WriteProfileInt(_T("Config"), _T("Image Full Reset Option"), CurrentSet->bCheckImageFullReset);
	pApp->WriteProfileInt(_T("Config"), _T("Battery Check Option"), CurrentSet->bCheckBatteryCheck);
	pApp->WriteProfileInt(_T("Config"), _T("Grabber ON OFF Option"), CurrentSet->bCheckGrabberOnOffCheck);
	
	pApp->WriteProfileInt(_T("Config"), _T("Use PCB Load Sensor"), CurrentSet->bCheckUsePcbSensor);
	//pCurrentSet->bCheckRotateReset = pApp->GetProfileInt(_T("Config"), _T("Reset Rotate Reset Option"), FALSE);

	pApp->WriteProfileInt(_T("Config"), _T("FHDUHD_OLED_PACK_RESET"), CurrentSet->bFHDUHD_OLED_Pack_Reset);
	pApp->WriteProfileInt(_T("Config"), _T("EPI_PACK_RESET"), CurrentSet->bEpiPAckReset);
	pApp->WriteProfileInt(_T("Config"), _T("GRAB_BASE_RESET"), CurrentSet->bGrabBaseReset);
	//m_Ini.SetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET", CurrentSet->bEpiPAckReset);
	//m_Ini.SetProfileInt(SPEC_CHECK_S, "GRAB_BASE_RESET", CurrentSet->bGrabBaseReset);
	//CurrentSet->bEpiPAckReset = m_Ini.GetProfileInt(SPEC_CHECK_S, "EPI_PACK_RESET", 0);
	//CurrentSet->bGrabBaseReset = m_Ini.GetProfileInt(SPEC_CHECK_S, "GRAB_BASE_RESET", 0);

	//+add kwmoon 080724
	pApp->WriteProfileString(_T("File"), _T("Current TestParam"),			CurrentSet->sTestParamIni);

	//+add kwmoon 081024
	pApp->WriteProfileString(_T("File"), _T("Current ModelList"),			CurrentSet->sModelListPath);

	pApp->WriteProfileInt(_T("Run"), _T("Run Type"),						CurrentSet->nRunType);
//AUTO	
	if(g_nSysType == AUTO_SYS){
		pApp->WriteProfileInt(_T("Run"), _T("Start Type"),						CurrentSet->nRunStartType);
	}
//
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
#ifdef SM_MODIFY_CODE__
	pApp->WriteProfileString(_T("Folder"), _T("Current Model VF Config Root Foler"), CurrentSet->sModelVFConfigFolder); //pCurrentSet->sModelVFConfigFolder = pApp->GetProfileString(_T("Folder"), _T("Current Model VF Config Root Foler"), "D:\\DFT");
#endif
	pApp->WriteProfileString(_T("Folder"), _T("ParmAdj Current Grab Folder"), CurrentSet->sParmAdjGrabFolder);
	pApp->WriteProfileString(_T("Folder"), _T("ParmAdj Current Ref Folder"),  CurrentSet->sParmAdjRefFolder);
	pApp->WriteProfileString(_T("Folder"), _T("ParmAdj Current Mask Folder"), CurrentSet->sParmAdjMaskFolder);
//AUTO
	if(g_nSysType == AUTO_SYS){
		pApp->WriteProfileInt(_T("Config"), _T("JigUpType"), CurrentSet->nJigUpType); 
	}
//
	pApp->WriteProfileInt(_T("Config"), _T("AC Power Off Type"), CurrentSet->nAcPowerOffType);
	pApp->WriteProfileInt(_T("Config"), _T("Grabber Reset Type"), CurrentSet->nAuto_Grab_Reset);

	pApp->WriteProfileInt(_T("Config"), _T("LVDS Grabber Width"),			CurrentSet->nLvdsWidth);
	pApp->WriteProfileInt(_T("Config"), _T("LVDS Grabber Height"),			CurrentSet->nLvdsHeight);
	pApp->WriteProfileInt(_T("Config"), _T("Analog Grabber Width"),			CurrentSet->nAnalogWidth);
	pApp->WriteProfileInt(_T("Config"), _T("Analog Grabber Height"),		CurrentSet->nAnalogHeight);
	pApp->WriteProfileInt(_T("Config"), _T("LVDS 30P Robot Skip FFC"),		CurrentSet->bRobotSkipFFC);

	//+add 090420(Modification No1)
	pApp->WriteProfileInt(_T("Config"), _T("Analog Grabber Type"),			CurrentSet->nAnalogType);

	pApp->WriteProfileString(_T("Config"), _T("Pattern Generator ComPort"), CurrentSet->sPatternComPort);
	sBaudRate.Format("%d", CurrentSet->wPatternBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("Pattern Generator Baud Rate"), sBaudRate);

	pApp->WriteProfileString(_T("Config"), _T("HDMI Generator ComPort"), CurrentSet->sHDMIComPort);
	sBaudRate.Format("%d", CurrentSet->wHDMIBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("HDMI Generator Baud Rate"), sBaudRate);
	pApp->WriteProfileInt(_T("Config"), _T("HDMI Generator Type"),			CurrentSet->nHDMIGenType);

	// add 20100622
	pApp->WriteProfileString(_T("Config"), _T("AVC ComPort"), CurrentSet->sAvcComPort);
	sBaudRate.Format("%d", CurrentSet->wAvcBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("AVC Baud Rate"), sBaudRate);
	pApp->WriteProfileInt(_T("Config"), _T("Use AVC"),			CurrentSet->bUseAVC);
	//-
	pApp->WriteProfileString(_T("Config"), _T("LNB ComPort"), CurrentSet->sLNBComPort);
	sBaudRate.Format("%d", CurrentSet->wLNBBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("LNB Baud Rate"), sBaudRate);
	pApp->WriteProfileInt(_T("Config"), _T("Use LNB"),			CurrentSet->bUseLNB);
//AUTO
	if(1)//g_nSysType == AUTO_SYS)
	{
		pApp->WriteProfileString(_T("Config"), _T("Jig ComPort"), CurrentSet->sJigComPort);
		sBaudRate.Format("%d", CurrentSet->wJigBaudRate);
		pApp->WriteProfileString(_T("Config"), _T("Jig Baud Rate"), sBaudRate);
		pApp->WriteProfileInt(_T("Config"), _T("Use Jig"),			CurrentSet->bUseJig);
	}
//
	pApp->WriteProfileString(_T("Config"), _T("VFM ComPort"), CurrentSet->sVfmComPort);
	sBaudRate.Format("%d", CurrentSet->wVfmBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("VFM Baud Rate"), sBaudRate);
	pApp->WriteProfileInt(_T("Config"), _T("Use VFM"),			CurrentSet->bUseVfm);

	pApp->WriteProfileString(_T("Config"), _T("DP Generator ComPort"), CurrentSet->sDpgComPort);
	sBaudRate.Format("%d", CurrentSet->wDpgBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("DPG Baud Rate"), sBaudRate);
	pApp->WriteProfileInt(_T("Config"), _T("Use DPG"),			CurrentSet->bUseDpg);
	


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

	pApp->WriteProfileString(_T("Config"), _T("Start Box ComPort"),			CurrentSet->sStartBoxComPort);
	sBaudRate.Format("%d", CurrentSet->wStartBoxBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("Start Box Baud Rate"), sBaudRate);

	pApp->WriteProfileString(_T("Config"), _T("I2c Adc ComPort"),			CurrentSet->sI2cAdcComPort);
	sBaudRate.Format("%d", CurrentSet->wI2cAdcBaudRate);
	pApp->WriteProfileString(_T("Config"), _T("I2c ADC Baud Rate"), sBaudRate);
	
	pApp->WriteProfileInt(_T("Config"), _T("Use Pattern Generator"),		CurrentSet->bUsePatternGen);
	pApp->WriteProfileInt(_T("Config"), _T("Use TVComm Port"),				CurrentSet->bUseTVCommPort);
	pApp->WriteProfileInt(_T("Config"), _T("Use AVSwitchingBox"),			CurrentSet->bUseAVSwitchBox);
	pApp->WriteProfileInt(_T("Config"), _T("Use Scanner"),					CurrentSet->bUseScanner);
	pApp->WriteProfileInt(_T("Config"), _T("Use Start Box"),				CurrentSet->bUseStartBox);
	pApp->WriteProfileInt(_T("Config"), _T("Use HDMI Generator"),			CurrentSet->bUseHDMIGen);

	pApp->WriteProfileInt(_T("Config"), _T("ADC Type"),						(BOOL)(CurrentSet->nAdcType));
	pApp->WriteProfileInt(_T("Config"), _T("Auto Save Grab Image"),			(BOOL)(CurrentSet->bSaveGrabImg));
	pApp->WriteProfileInt(_T("Config"), _T("Auto Save Processed Image"),	(BOOL)(CurrentSet->bSaveProcImg));
	pApp->WriteProfileInt(_T("Config"), _T("Save Caption Test Image"),		(BOOL)(CurrentSet->bSaveCaptionTestImg));
	pApp->WriteProfileInt(_T("Config"), _T("Save Moving Pic Test Image"),	(BOOL)(CurrentSet->bSaveMovingPicTestImg));
	pApp->WriteProfileInt(_T("Config"), _T("Save Intermediate Image"),		(BOOL)(CurrentSet->bSaveIntermediateImage));

	pApp->WriteProfileInt(_T("Config"), _T("Save Processing Time Data"),	(BOOL)(CurrentSet->bSaveProcessingTimeData));
	
	pApp->WriteProfileInt(_T("Config"), _T("Save Review Data"),	(BOOL)(CurrentSet->bSaveReviewData));
	pApp->WriteProfileInt(_T("Config"), _T("Save Detail Result Data"),	(BOOL)(CurrentSet->bSaveDetailResultData));


	pApp->WriteProfileInt(_T("Config"), _T("Audio Input Source"),			CurrentSet->nAudioSource);

	pApp->WriteProfileInt(_T("Config"), _T("Save Result to Text File"),		(BOOL)(CurrentSet->bSaveResult2Txt));
	pApp->WriteProfileInt(_T("Config"), _T("Save Result to Html File"),		(BOOL)(CurrentSet->bSaveResult2Html));
	pApp->WriteProfileInt(_T("Config"), _T("Save Result to Csv File"),		(BOOL)(CurrentSet->bSaveResult2Csv));
	
	pApp->WriteProfileInt(_T("Config"), _T("No of repeat execution"),		(UINT)(CurrentSet->nNoRepeatExecution));
	pApp->WriteProfileInt(_T("Config"), _T("No of frames for refimage creation"),		(UINT)(CurrentSet->nNoFrameForRefImageCreation));
	pApp->WriteProfileInt(_T("Config"), _T("Percentage of pixels in middle range"),		(UINT)(CurrentSet->nPixelInMiddleRange));
	pApp->WriteProfileInt(_T("Config"), _T("Percentage of non-black pixels"),			(UINT)(CurrentSet->nNonBlackPixel));

	pApp->WriteProfileInt(_T("Config"), _T("No Used Colors"),			(UINT)(CurrentSet->nNoUsedColors));
	
	pApp->WriteProfileInt(_T("Config"), _T("Delay Percentage"),				(UINT)(CurrentSet->nDelayPercentage));
	pApp->WriteProfileInt(_T("Config"), _T("LVDS Mode"),					CurrentSet->nLVDS_Mode);

	//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		pApp->WriteProfileInt(_T("Config"), _T("Uranous Grabber Mode"),			CurrentSet->nUranousMode);
		pApp->WriteProfileInt(_T("Config"), _T("LVDS OddDE Only"),			(UINT)CurrentSet->bOddDE_Only);
	}
	pApp->WriteProfileInt(_T("Config"), _T("Color Bit Count"),				CurrentSet->nBitCount);

	pApp->WriteProfileInt(_T("Config"), _T("No of Internal Retry"),			CurrentSet->nNoRetry);

	pApp->WriteProfileInt(_T("Config"), _T("Grab Image Check Area"),			CurrentSet->nGrabCheckArea);

	pApp->WriteProfileInt(_T("Config"), _T("TVComm Port Echo"),				(UINT)CurrentSet->bTVCommPortEcho);
	pApp->WriteProfileString(_T("Config"), _T("HDMI Roi Shape"),			CurrentSet->sHDMIRoiShape);
	pApp->WriteProfileString(_T("DataBase"), _T("Line_Code"),				CurrentSet->sLineCode);
	pApp->WriteProfileInt(_T("DataBase"), _T("Line Number"),				(UINT)CurrentSet->nLineNo);
	pApp->WriteProfileInt(_T("DataBase"), _T("System Number"),				(UINT)CurrentSet->nSystemNo);
	pApp->WriteProfileInt(_T("DataBase"), _T("Authority"),					(UINT)CurrentSet->nAuthority);
	pApp->WriteProfileString(_T("DataBase"), _T("PassWord"),				CurrentSet->strPassword);

	pApp->WriteProfileInt(_T("Config"), _T("Use GMES"),						CurrentSet->bUseGmes);
	pApp->WriteProfileInt(_T("Config"), _T("Use PLC"),						CurrentSet->bUsePLCRobot);
	pApp->WriteProfileInt(_T("Config"), _T("Use SERVER Control"),						CurrentSet->bUseServerCtrlConnect);
	

	pApp->WriteProfileString(_T("DataBase"), _T("GMES Host IP"),			CurrentSet->strGmesIP);
	pApp->WriteProfileInt(_T("DataBase"), _T("GMES Host Port"),				CurrentSet->nGmesPort);

	pApp->WriteProfileString(_T("DataBase"), _T("PLC Host IP"),			CurrentSet->strPLC_IP);
	pApp->WriteProfileInt(_T("DataBase"), _T("PLC Host Port"),				CurrentSet->nPLC_Port);
	pApp->WriteProfileInt(_T("DataBase"), _T("PLC DFT Number"),				CurrentSet->nDFT_No);

	pApp->WriteProfileString(_T("DataBase"), _T("Monitor Server IP"),			CurrentSet->strMonitorServer_IP);
	pApp->WriteProfileInt(_T("DataBase"), _T("Monitor Server Port"),				CurrentSet->nMonitorServer_Port);

	//pCurrentSet->strMonitorServer_IP = pApp->GetProfileString(_T("DataBase"), _T("Monitor Server IP"), _T("192.168.0.3"));
	//pCurrentSet->nMonitorServer_Port = pApp->GetProfileInt(_T("DataBase"), _T("Monitor Server Port"), 3000);

	pApp->WriteProfileInt(_T("DataBase"), _T("AUTO UPLOAD MES DATA"),		(UINT)CurrentSet->bAutoUploadMes);

	pApp->WriteProfileInt(_T("DataBase Upload"), _T("UPLOAD MES DATA"),		(UINT)CurrentSet->bUploadMes);

	pApp->WriteProfileInt(_T("Config"), _T("AudioCheckMethod"),				(UINT)CurrentSet->nAudioCheckMethod);

	pApp->WriteProfileInt(_T("Config"), _T("Data Save Period"),				(UINT)CurrentSet->nDataSavePeriod);
	pApp->WriteProfileInt(_T("Config"), _T("Auto Data Delete"),				(UINT)CurrentSet->bAutoDataDelete);

	pApp->WriteProfileInt(_T("Config"), _T("Tool Option Set Method"),		(UINT)CurrentSet->nToolOptionSetMethod);

	pApp->WriteProfileInt(_T("Config"), _T("HDMI Out Port Control"),		(UINT)CurrentSet->bHdmiOutControl);
	pApp->WriteProfileInt(_T("Config"), _T("HDMI1 Port"),					(UINT)CurrentSet->nHDMI1_Port);
	pApp->WriteProfileInt(_T("Config"), _T("HDMI2 Port"),					(UINT)CurrentSet->nHDMI2_Port);
	pApp->WriteProfileInt(_T("Config"), _T("HDMI3 Port"),					(UINT)CurrentSet->nHDMI3_Port);
	pApp->WriteProfileInt(_T("Config"), _T("HDMI4 Port"),					(UINT)CurrentSet->nHDMI4_Port);

	pApp->WriteProfileInt(_T("Config"), _T("HDMI CEC On/Off Control"), (UINT)CurrentSet->bHdmiCecControl);
	pApp->WriteProfileInt(_T("Config"), _T("HDMI HDCP On/Off Control"), (UINT)CurrentSet->bHdmiHdcpControl);
	pApp->WriteProfileInt(_T("Config"), _T("HDMI EDID On/Off Control"), (UINT)CurrentSet->bHdmiEdidControl);

	pApp->WriteProfileInt(_T("Config"), _T("Auto Run DFT2(3)"),				(UINT)CurrentSet->bAutoRun_DFT2);
	pApp->WriteProfileInt(_T("Config"), _T("PJT Grab Disable"),				(UINT)CurrentSet->bPJT_GrabDisable);
	
}

void CDATsysView::OnRunAbort() 
{
	CString sMsg;
//	if(m_bThreadRunning)
	if(g_nSysRobotType == AUTO_SYS)
		OnBnClickedButtonAutoRobotStop();

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
			sMsg.LoadString(IDS_USER_BREAK);
			AddStringToStatus(sMsg);
			m_szResultMsg2 = sMsg;
			
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

// change PSH 080603
//	if(!m_bThreadRunning)
	if(!CurrentSet->bIsRunning)
	{
		//+del kwmoon 071212
	//	if(CurrentSet->sSeqPath.IsEmpty())
	//	{
	//		AfxMessageBox(IDS_ERR_INI); return;
	//	}

		m_bMakeReferenceMode = TRUE;

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

// change PSH 080603
//	if(!m_bThreadRunning)
//AUTO
	if(g_nSysType == AUTO_SYS){
		CurrentSet->bIsRunMsg = TRUE;
	}
//
	if(!CurrentSet->bIsRunning)
	{
//AUTO	
		if((g_nSysType == AUTO_SYS) && (CurrentSet->nRunStartType == MANUAL_START)){
			if(!gJigCtrl.Get_JigReady()){
				Sleep(500);
				if(!gJigCtrl.Get_JigReady()){
#if 1 //220428
					CString sTemp;
					sTemp = "Move down jig before starting test.";
					AddStringToStatus(sTemp);
#else
					AfxMessageBox(IDS_JIG_DOWN); 
					CurrentSet->bIsRunMsg = FALSE;	
					return;

#endif
				}
			}
		}
//
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

		//+add psh 090611
		if(m_bResultDlgActivate)
		{
			if (m_ResultDlg->m_bActivate == 1)
			{
				delete m_ResultDlg;
			}
		}
		sTime = "";
		UpdateInfo(TIME, "");

		RunTest(0,0);
	}

}

#if 1
BOOL CDATsysView::Read_PCBAID_READ()
{
	int nRev = 0;
	int nKeyItem = 0;
	int nKeySerialItem = 0;
	CString sTmp;
	int nKeyItem_GMES = 0;

	CString sSerialTmp;
	CString sModelnameTmp;


	//#ifdef DEBUG_MD5_CODE__
	//	if ((g_nGrabberType == UHD_GRABBER) && (CurrentSet->bUseScanner == FALSE))
	//#else
	////	if ((g_nGrabberType == UHD_GRABBER) && (g_nSysType == AUTO_SYS) && ((CurrentSet->bUseScanner == FALSE) || (CurrentSet->bUsePLCRobot == TRUE)))
	//	if ((g_nGrabberType == UHD_GRABBER) && ((CurrentSet->bUseScanner == FALSE) || (CurrentSet->bUsePLCRobot == TRUE)))
	//#endif	
	if (g_nUseNoScanType == TRUE)//if (m_strWipId.Find("NO_SCAN") >= 0)//if (g_nGrabberType == UHD_GRABBER)
	{
		//_Wait(500);

		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
		if ((nRev != 2) || (sSerialTmp.Find("x") >= 0))
		{
			nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
			if ((nRev != 2) || (sSerialTmp.Find("x") >= 0))
			{
				CurrentSet->bSystemBreak = TRUE;
				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				CurrentSet->sPCBID_Read = "NO_READ";
				return FALSE;
			}
		}
		if (nRev == 2)
		{
			sSerialTmp = sSerialTmp.Left(10);
			CurrentSet->sPCBID_Read = sSerialTmp;

			if (m_strWipId.Find("NO_SCAN") >= 0)
			{
				CString StrPathFileNameTemp;
				m_strWipId = sSerialTmp;
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sGrabFolder;
				CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sResultFolder;
				CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				rename(StrPathFileNameTemp, CurrentSet->sResultFolder);

				StrPathFileNameTemp = CurrentSet->sResultPath;
				CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sResultPath);
				}

				StrPathFileNameTemp = CurrentSet->sDetailResultDataPath;
				CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sDetailResultDataPath);
				}

				if (m_bTimeDataFileOpen == TRUE)
				{

					CloseTimeDataFile();//

					StrPathFileNameTemp = m_sTimFileName;

					m_sTimFileName.Replace("NO_SCAN", m_strWipId);

					m_pTimeDataFile.Rename(StrPathFileNameTemp, m_sTimFileName);
				}
				/*	CStdioFile	m_pTimeDataFile;
					BOOL	m_bTimeDataFileOpen;
					DWORD	m_dwStepElapsedTime;
					CString m_sTimFileName;*/



			}
			//else if(m_strWipId.Find(sSerialTmp) == -1)
			//{
			//	m_strWipId = sSerialTmp;
			//	ctrlWipIdEdit.SetWindowText(m_strWipId);
			//	//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
			//}
			sTmp.Format("PCBA ID Read : ");
			sTmp += sSerialTmp;
			AddStringToStatus(sTmp);
			if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)
			{
				//godtech 220324
//				if (CurrentSet->bUsePLCRobot == TRUE)
				{
					if (CurrentSet->nToolOptionSetMethod == 0) {
						Sleep(500);
						if (!GetToolOption()) {
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					//if(CurrentSet->bNgCountCheck){
					if (!CheckNgCount()) {
						AddStringToStatus("NG Count Check Retry");
						if (!CheckNgCount()) {
							AddStringToStatus("NG Count Check Retry");
							if (!CheckNgCount()) {
								sTmp.Format("NG Count Check Error....");
								AddStringToStatus(sTmp);
								CurrentSet->bIsRunMsg = FALSE; return 0;
							}
						}
					}
					if (CurrentSet->nDftNgCount >= 3) {
						ReportSend_NgCount();
						sTmp.Format("%s : NG history is more than 3 times.", m_strWipId);
						AddStringToStatus(sTmp);//AfxMessageBox(sTmp);
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}

				}
			}
			return TRUE;

		}
		else
		{
			CurrentSet->bSystemBreak = TRUE;
			sTmp.Format("PCBA ID Read :FAIL");
			AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
			CurrentSet->sPCBID_Read = "NO_READ";
			return FALSE;
		}
	}
	else
	{
		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
		if ((nRev != 2) || (sSerialTmp.Find("x") >= 0))
		{
			nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
			if ((nRev != 2) || (sSerialTmp.Find("x") >= 0))
			{
#if 0
				CurrentSet->bSystemBreak = TRUE;
				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				CurrentSet->sPCBID_Read = "NO_READ";
				return FALSE;
#else
				sTmp.Format("PCBA ID Read :FAIL Continue");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				CurrentSet->sPCBID_Read = "NO_READ";
				return TRUE;
#endif
			}
		}
		sSerialTmp = sSerialTmp.Left(10);
		CurrentSet->sPCBID_Read = sSerialTmp;
		sTmp.Format("PCBA ID Check Read : ");
		sTmp += sSerialTmp;
		AddStringToStatus(sTmp);

		return TRUE;
	}




}

#else
BOOL CDATsysView::Read_PCBAID_READ()
{
	int nRev = 0;
	int nKeyItem = 0;
	int nKeySerialItem = 0;
	CString sTmp;
	int nKeyItem_GMES = 0;

	CString sSerialTmp;
	CString sModelnameTmp;


	
	if (g_nUseNoScanType == TRUE)//if (m_strWipId.Find("NO_SCAN") >= 0)//if (g_nGrabberType == UHD_GRABBER)
	{
		//_Wait(500);TVCommCtrl.ModelSNo_Write

		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
		if ((nRev != 2)||(sSerialTmp.Find("x") >= 0))
		{
			nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
			if ((nRev != 2) || (sSerialTmp.Find("x") >= 0))
			{
				CurrentSet->bSystemBreak = TRUE;
				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				return FALSE;
			}
		}
		if (nRev == 2)
		{
			sSerialTmp = sSerialTmp.Left(10);
			if (m_strWipId.Find("NO_SCAN") >= 0)
			{
				CString StrPathFileNameTemp;
				m_strWipId = sSerialTmp;
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sGrabFolder;
				CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sResultFolder;
				CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				rename(StrPathFileNameTemp, CurrentSet->sResultFolder);

				StrPathFileNameTemp = CurrentSet->sResultPath;
				CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sResultPath);
				}

				StrPathFileNameTemp = CurrentSet->sDetailResultDataPath;
				CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sDetailResultDataPath);
				}

				if (m_bTimeDataFileOpen == TRUE)
				{

					CloseTimeDataFile();//

					StrPathFileNameTemp = m_sTimFileName;
					
					m_sTimFileName.Replace("NO_SCAN", m_strWipId);

					m_pTimeDataFile.Rename(StrPathFileNameTemp, m_sTimFileName);
				}
			/*	CStdioFile	m_pTimeDataFile;
				BOOL	m_bTimeDataFileOpen;
				DWORD	m_dwStepElapsedTime;
				CString m_sTimFileName;*/



			}
			//else if(m_strWipId.Find(sSerialTmp) == -1)
			//{
			//	m_strWipId = sSerialTmp;
			//	ctrlWipIdEdit.SetWindowText(m_strWipId);
			//	//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
			//}
			sTmp.Format("PCBA ID Read : ");
			sTmp += sSerialTmp;
			AddStringToStatus(sTmp);
			if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)
			{
				if (CurrentSet->bUsePLCRobot == TRUE)
				{
					if (CurrentSet->nToolOptionSetMethod == 0) {
						Sleep(500);
						if (!GetToolOption()) {
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					//if(CurrentSet->bNgCountCheck){
					if (!CheckNgCount()) {
						AddStringToStatus("NG Count Check Retry");
						if (!CheckNgCount()) {
							sTmp.Format("NG Count Check Error....");
							AfxMessageBox(sTmp);
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					if (CurrentSet->nDftNgCount >= 3) {
						ReportSend_NgCount();
						sTmp.Format("%s : NG history is more than 3 times.", m_strWipId);
						AfxMessageBox(sTmp);
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}

				}
			}
			return TRUE;

		}
		else
		{
			CurrentSet->bSystemBreak = TRUE;
			sTmp.Format("PCBA ID Read :FAIL");
			AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
			return FALSE;
		}
	}
	else
	{	

		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
#ifdef	DEBUG_MD5_CODE__
		sSerialTmp = "SKY1107";
#endif
		if ((nRev != 2) && (sSerialTmp.Find("SKY") != 0))
		{
			nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
			if ((nRev != 2) && (sSerialTmp.Find("SKY") != 0))
			{
				CurrentSet->bSystemBreak = TRUE;
				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				return FALSE;
			}
		}
		if (nRev == 2)
		{
			sSerialTmp = sSerialTmp.Left(10);
			if (m_strWipId.Find("NO_SCAN") >= 0)
			{
				CString StrPathFileNameTemp;
				m_strWipId = sSerialTmp;
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sGrabFolder;
				CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sResultFolder;
				CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				rename(StrPathFileNameTemp, CurrentSet->sResultFolder);

				StrPathFileNameTemp = CurrentSet->sResultPath;
				CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sResultPath);
				}

				StrPathFileNameTemp = CurrentSet->sDetailResultDataPath;
				CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sDetailResultDataPath);
				}

				if (m_bTimeDataFileOpen == TRUE)
				{

					CloseTimeDataFile();//

					StrPathFileNameTemp = m_sTimFileName;

					m_sTimFileName.Replace("NO_SCAN", m_strWipId);

					m_pTimeDataFile.Rename(StrPathFileNameTemp, m_sTimFileName);
				}			
			}
			//else if(m_strWipId.Find(sSerialTmp) == -1)
			//{
			//	m_strWipId = sSerialTmp;
			//	ctrlWipIdEdit.SetWindowText(m_strWipId);
			//	//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
			//}
			sTmp.Format("PCBA ID Read : ");
			sTmp += sSerialTmp;
			AddStringToStatus(sTmp);
			if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)
			{
				if (CurrentSet->bUsePLCRobot == TRUE)
				{
					if (CurrentSet->nToolOptionSetMethod == 0) {
						Sleep(500);
						if (!GetToolOption()) {
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					//if(CurrentSet->bNgCountCheck){
					if (!CheckNgCount()) {
						AddStringToStatus("NG Count Check Retry");
						if (!CheckNgCount()) {
							sTmp.Format("NG Count Check Error....");
							AfxMessageBox(sTmp);
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					if (CurrentSet->nDftNgCount >= 3) {
						ReportSend_NgCount();
						sTmp.Format("%s : NG history is more than 3 times.", m_strWipId);
						AfxMessageBox(sTmp);
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}

				}
			}
			return TRUE;

		}
		else
		{
			if (ChekDrmKeyScanRead() == FALSE)
			{
#ifdef	DEBUG_MD5_CODE__
				m_strWipIdSave = "TEST_SN_DEBUG";
#endif
				sSerialTmp = m_strWipIdSave;
				if (m_strWipId.Find("NO_SCAN") >= 0)
				{
					CString StrPathFileNameTemp;
					m_strWipId = sSerialTmp;
					ctrlWipIdEdit.SetWindowText(m_strWipId);
					//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

					nRev = TVCommCtrl.ModelSNo_Write(m_strWipId, TRUE);
					if (nRev == FALSE)
					{
						CurrentSet->bSystemBreak = TRUE;
						sTmp.Format("PCBA ID Write :FAIL");
						AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
						return FALSE;
					}
									   
					StrPathFileNameTemp = CurrentSet->sGrabFolder;
					CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

					StrPathFileNameTemp = CurrentSet->sResultFolder;
					CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
					rename(StrPathFileNameTemp, CurrentSet->sResultFolder);

					StrPathFileNameTemp = CurrentSet->sResultPath;
					CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
					if (FileExists(StrPathFileNameTemp))
					{
						rename(StrPathFileNameTemp, CurrentSet->sResultPath);
					}

					StrPathFileNameTemp = CurrentSet->sDetailResultDataPath;
					CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
					if (FileExists(StrPathFileNameTemp))
					{
						rename(StrPathFileNameTemp, CurrentSet->sDetailResultDataPath);
					}

					if (m_bTimeDataFileOpen == TRUE)
					{

						CloseTimeDataFile();//

						StrPathFileNameTemp = m_sTimFileName;

						m_sTimFileName.Replace("NO_SCAN", m_strWipId);

						m_pTimeDataFile.Rename(StrPathFileNameTemp, m_sTimFileName);
					}
				}
				//else if(m_strWipId.Find(sSerialTmp) == -1)
				//{
				//	m_strWipId = sSerialTmp;
				//	ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
				//	CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				//	CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				//	CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
				//}
			//	sTmp.Format("PCBA ID Read : ");
			//	sTmp += sSerialTmp;
			//	AddStringToStatus(sTmp);
				if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)
				{
					if (CurrentSet->bUsePLCRobot == TRUE)
					{
						if (CurrentSet->nToolOptionSetMethod == 0) {
							Sleep(500);
							if (!GetToolOption()) {
								CurrentSet->bIsRunMsg = FALSE; return 0;
							}
						}
						//if(CurrentSet->bNgCountCheck){
						if (!CheckNgCount()) {
							AddStringToStatus("NG Count Check Retry");
							if (!CheckNgCount()) {
								sTmp.Format("NG Count Check Error....");
								AfxMessageBox(sTmp);
								CurrentSet->bIsRunMsg = FALSE; return 0;
							}
						}
						if (CurrentSet->nDftNgCount >= 3) {
							ReportSend_NgCount();
							sTmp.Format("%s : NG history is more than 3 times.", m_strWipId);
							AfxMessageBox(sTmp);
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}

					}
				}
				return TRUE;
			}
			else
			{
				CurrentSet->bSystemBreak = TRUE;
				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				return FALSE;
			}
		}		
	}
}
#endif

#if 0
BOOL CDATsysView::Read_No_GMES_PCBAID_READ()
{
	int nRev = 0;
	int nKeyItem = 0;
	int nKeySerialItem = 0;
	CString sTmp;
	int nKeyItem_GMES = 0;

	CString sSerialTmp;
	CString sModelnameTmp;



	if (g_nUseNoScanType == TRUE)//if (m_strWipId.Find("NO_SCAN") >= 0)//if (g_nGrabberType == UHD_GRABBER)
	{
		//_Wait(500);TVCommCtrl.ModelSNo_Write

		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
		if ((nRev != 2) || (sSerialTmp.Find("x") >= 0))
		{
			nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
			if ((nRev != 2) || (sSerialTmp.Find("x") >= 0))
			{
				//CurrentSet->bSystemBreak = TRUE;
				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				return FALSE;
			}
		}
		if (nRev == 2)
		{
			sSerialTmp = sSerialTmp.Left(10);
			if (m_strWipId.Find("NO_SCAN") >= 0)
			{
				CString StrPathFileNameTemp;
				m_strWipId = sSerialTmp;
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sGrabFolder;
				CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sResultFolder;
				CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				rename(StrPathFileNameTemp, CurrentSet->sResultFolder);

				StrPathFileNameTemp = CurrentSet->sResultPath;
				CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sResultPath);
				}

				StrPathFileNameTemp = CurrentSet->sDetailResultDataPath;
				CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sDetailResultDataPath);
				}

				if (m_bTimeDataFileOpen == TRUE)
				{

					CloseTimeDataFile();//

					StrPathFileNameTemp = m_sTimFileName;

					m_sTimFileName.Replace("NO_SCAN", m_strWipId);

					m_pTimeDataFile.Rename(StrPathFileNameTemp, m_sTimFileName);
				}
				/*	CStdioFile	m_pTimeDataFile;
					BOOL	m_bTimeDataFileOpen;
					DWORD	m_dwStepElapsedTime;
					CString m_sTimFileName;*/



			}
			//else if(m_strWipId.Find(sSerialTmp) == -1)
			//{
			//	m_strWipId = sSerialTmp;
			//	ctrlWipIdEdit.SetWindowText(m_strWipId);
			//	//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
			//}
			sTmp.Format("PCBA ID Read : ");
			sTmp += sSerialTmp;
			AddStringToStatus(sTmp);
			if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)
			{
				if (CurrentSet->bUsePLCRobot == TRUE)
				{
					if (CurrentSet->nToolOptionSetMethod == 0) {
						Sleep(500);
						if (!GetToolOption()) {
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					//if(CurrentSet->bNgCountCheck){
					if (!CheckNgCount()) {
						AddStringToStatus("NG Count Check Retry");
						if (!CheckNgCount()) {
							sTmp.Format("NG Count Check Error....");
							AfxMessageBox(sTmp);
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					if (CurrentSet->nDftNgCount >= 3) {
						ReportSend_NgCount();
						sTmp.Format("%s : NG history is more than 3 times.", m_strWipId);
						AfxMessageBox(sTmp);
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}

				}
			}
			return TRUE;

		}
		else
		{
			//	CurrentSet->bSystemBreak = TRUE;
			sTmp.Format("PCBA ID Read :FAIL");
			AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
			return FALSE;
		}
	}
	else
	{

		nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
#ifdef	DEBUG_MD5_CODE__
		sSerialTmp = "SKY1107";
#endif
		if ((nRev != 2) && (sSerialTmp.Find("SKY") != 0))
		{
			nRev = TVCommCtrl.ModelSNo_Read(nKeySerialItem, sSerialTmp, TRUE);
			if ((nRev != 2) && (sSerialTmp.Find("SKY") != 0))
			{
				//	CurrentSet->bSystemBreak = TRUE;
				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
				return FALSE;
			}
		}
		if (nRev == 2)
		{
			sSerialTmp = sSerialTmp.Left(10);
			if (m_strWipId.Find("NO_SCAN") >= 0)
			{
				CString StrPathFileNameTemp;
				m_strWipId = sSerialTmp;
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sGrabFolder;
				CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

				StrPathFileNameTemp = CurrentSet->sResultFolder;
				CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
				rename(StrPathFileNameTemp, CurrentSet->sResultFolder);

				StrPathFileNameTemp = CurrentSet->sResultPath;
				CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sResultPath);
				}

				StrPathFileNameTemp = CurrentSet->sDetailResultDataPath;
				CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
				if (FileExists(StrPathFileNameTemp))
				{
					rename(StrPathFileNameTemp, CurrentSet->sDetailResultDataPath);
				}

				if (m_bTimeDataFileOpen == TRUE)
				{

					CloseTimeDataFile();//

					StrPathFileNameTemp = m_sTimFileName;

					m_sTimFileName.Replace("NO_SCAN", m_strWipId);

					m_pTimeDataFile.Rename(StrPathFileNameTemp, m_sTimFileName);
				}
			}
			//else if(m_strWipId.Find(sSerialTmp) == -1)
			//{
			//	m_strWipId = sSerialTmp;
			//	ctrlWipIdEdit.SetWindowText(m_strWipId);
			//	//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
			//	CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
			//}
			sTmp.Format("PCBA ID Read : ");
			sTmp += sSerialTmp;
			AddStringToStatus(sTmp);

			return TRUE;

		}
		else
		{
			if (ChekDrmKeyScanRead() == FALSE)
			{
#ifdef	DEBUG_MD5_CODE__
				m_strWipIdSave = "TEST_SN_DEBUG";
#endif
				sSerialTmp = m_strWipIdSave;
				if (m_strWipId.Find("NO_SCAN") >= 0)
				{
					CString StrPathFileNameTemp;
					m_strWipId = sSerialTmp;
					ctrlWipIdEdit.SetWindowText(m_strWipId);
					//	CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

					nRev = TVCommCtrl.ModelSNo_Write(m_strWipId, TRUE);
					if (nRev == FALSE)
					{
						sTmp.Format("PCBA ID Write :FAIL");
						AddStringToStatus(sTmp); //AfxMessageBox(sTmp);
						return FALSE;
					}

					StrPathFileNameTemp = CurrentSet->sGrabFolder;
					CurrentSet->sGrabFolder.Replace("NO_SCAN", m_strWipId);

					StrPathFileNameTemp = CurrentSet->sResultFolder;
					CurrentSet->sResultFolder.Replace("NO_SCAN", m_strWipId);
					rename(StrPathFileNameTemp, CurrentSet->sResultFolder);

					StrPathFileNameTemp = CurrentSet->sResultPath;
					CurrentSet->sResultPath.Replace("NO_SCAN", m_strWipId);
					if (FileExists(StrPathFileNameTemp))
					{
						rename(StrPathFileNameTemp, CurrentSet->sResultPath);
					}

					StrPathFileNameTemp = CurrentSet->sDetailResultDataPath;
					CurrentSet->sDetailResultDataPath.Replace("NO_SCAN", m_strWipId);
					if (FileExists(StrPathFileNameTemp))
					{
						rename(StrPathFileNameTemp, CurrentSet->sDetailResultDataPath);
					}

					if (m_bTimeDataFileOpen == TRUE)
					{

						CloseTimeDataFile();//

						StrPathFileNameTemp = m_sTimFileName;

						m_sTimFileName.Replace("NO_SCAN", m_strWipId);

						m_pTimeDataFile.Rename(StrPathFileNameTemp, m_sTimFileName);
					}
				}

				return TRUE;
			}
			else
			{

				sTmp.Format("PCBA ID Read :FAIL");
				AddStringToStatus(sTmp); 
				return FALSE;
			}
		}
	}
}
#endif

LRESULT CDATsysView::RunTest(WPARAM wParam, LPARAM lParam)
{
	int nType = (int)wParam;

	int nVal = 0;
	CString sTmp;
	CString sMsg;
//	DWORD dwEventResult = 0;
//    clock_t		start;
//	MSG msg;
	int nNoCheckedStep;


	if((CurrentSet->nNoRepeatExecution != 0) && (m_nNoRepeatExecution == m_nNoCurrentRepeatExecution))
	{
		ComLogClear();
	}

	if(!CurrentSet->bIsRunning)
	{
		if(CurrentSet->sSeqPath.IsEmpty())
		{
			AfxMessageBox(IDS_ERR_INI); 
			CurrentSet->bIsRunMsg = FALSE; return 0;
		}
		
		if ((nType == 0) || (nType == 2)) // 0:Run Test, 1:Make Ref 2:Adjust Spec
		{

			m_strWipIdSave = "";
			if (g_nUseNoScanType == TRUE)
			{
				m_strWipId = "NO_SCAN";
				ctrlWipIdEdit.SetWindowText(m_strWipId);
				CurrentSet->bUseScanner = 0;
			}
#if 1
			else if (g_nSysType == AUTO_SYS)//AUTO
			{
				//	CurrentSet->nDftSeq = 0;
			//	if (CurrentSet->bUseScanner)
				{
					if (g_nNoFailedStep == 0) {
						nVal = GetBarcodeData(sTmp);
						if (nVal == 0) {
							Sleep(500);
							nVal = GetBarcodeData(sTmp);
						}
						if (nVal == 0) {
							AfxMessageBox("Barcode Scanning Error!");
							AddStringToStatus("Barcode Scanning Error!");
							CurrentSet->bIsRunMsg = FALSE;

							return 0;
						}
						m_strWipId = sTmp;
						ctrlWipIdEdit.SetWindowText(m_strWipId);
					}
					else {
						nVal = GetBarcodeData(sTmp);
						if (nVal == 0) {
							ctrlWipIdEdit.GetWindowText(sTmp);

							//+add 090203(Modification No2)
							sTmp.TrimRight(); sTmp.TrimLeft();
						}
						else {
							m_strWipId = sTmp;
							ctrlWipIdEdit.SetWindowText(m_strWipId);
						}

						if (sTmp.IsEmpty())
						{
							m_strWipId = "NO_SCAN";
							ctrlWipIdEdit.SetWindowText(m_strWipId);
							//m_strWipId = _T("");
							//AfxMessageBox("Please Insert Wip ID!");
							//CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
				}
				/*else
				{
					ctrlWipIdEdit.GetWindowText(sTmp);
					sTmp.TrimRight(); sTmp.TrimLeft();

					if (sTmp.IsEmpty())
					{
						m_strWipId = "NO_SCAN";
						ctrlWipIdEdit.SetWindowText(m_strWipId);
					}
					else
					{
						sTmp.TrimRight(0x0a);
						sTmp.TrimRight(0x0d);
						m_strWipId = sTmp;
					}
				}*/
			}
			else
			{
				if (CurrentSet->bUseStartBox)
				{
					OnScannerDisable();
					m_strWipId = "NO_SCAN";
					ctrlWipIdEdit.SetWindowText(m_strWipId);
				}
				else
				{
					ctrlWipIdEdit.GetWindowText(sTmp);
					sTmp.TrimRight(); sTmp.TrimLeft();

					if (sTmp.IsEmpty())
					{
						m_strWipId = "NO_SCAN";
						ctrlWipIdEdit.SetWindowText(m_strWipId);
					}
					else
					{
						sTmp.TrimRight(0x0a);
						sTmp.TrimRight(0x0d);
						m_strWipId = sTmp;
						//	m_strWipId = "GO43Q1001E";
						//	m_strWipId = "TS372100XL";
					}
				}
			}

#else
			else if (g_nSysType == AUTO_SYS)//AUTO
			{
				//	CurrentSet->nDftSeq = 0;
			//	if (CurrentSet->bUseScanner)
				{
					if (g_nNoFailedStep == 0) {
						nVal = GetBarcodeData(sTmp);
						if (nVal == 0) {
							Sleep(500);
							nVal = GetBarcodeData(sTmp);
						}

						if (ChekDrmKeyScanRead()==0)
						{
							if (nVal == 0) {
								AfxMessageBox("Barcode Scanning Error!");
								AddStringToStatus("Barcode Scanning Error!");
								CurrentSet->bIsRunMsg = FALSE;

								return 0;
							}
						//	m_strWipIdSave = sTmp;
						//	ctrlWipIdEdit.SetWindowText(m_strWipId);
						}
						//else
						{
							m_strWipIdSave = sTmp;
							m_strWipId = "NO_SCAN"; //m_strWipId = sTmp;
							//	m_strWipIdSave = m_strWipId;						
							ctrlWipIdEdit.SetWindowText(m_strWipId);	
						}
					}
					else {
						nVal = GetBarcodeData(sTmp);
						if (nVal == 0) {
							ctrlWipIdEdit.GetWindowText(sTmp);

							//+add 090203(Modification No2)
							sTmp.TrimRight(); sTmp.TrimLeft();
						}
						else {
							m_strWipId = sTmp;
							ctrlWipIdEdit.SetWindowText(m_strWipId);
						}

						if (sTmp.IsEmpty())
						{
							if (ChekDrmKeyScanRead() == 0)
							{
								AfxMessageBox("Barcode Scanning Error!");
								AddStringToStatus("Barcode Scanning Error!");
								CurrentSet->bIsRunMsg = FALSE;

								return 0;
							}
							m_strWipId = "NO_SCAN";
							ctrlWipIdEdit.SetWindowText(m_strWipId);
							//m_strWipId = _T("");
							//AfxMessageBox("Please Insert Wip ID!");
							//CurrentSet->bIsRunMsg = FALSE; return 0;
						}
						else
						{	
							m_strWipIdSave = sTmp;
							m_strWipId = "NO_SCAN";
							ctrlWipIdEdit.SetWindowText(m_strWipId);
						}
					}				
				}
				/*else
				{
					ctrlWipIdEdit.GetWindowText(sTmp);
					sTmp.TrimRight(); sTmp.TrimLeft();

					if (sTmp.IsEmpty())
					{
						m_strWipId = "NO_SCAN";
						ctrlWipIdEdit.SetWindowText(m_strWipId);	
					}
					else
					{
						sTmp.TrimRight(0x0a);
						sTmp.TrimRight(0x0d);
						m_strWipId = sTmp;
					}
				}*/
			}
			else
			{
				if (CurrentSet->bUseStartBox)
				{
					OnScannerDisable();
					m_strWipId = "NO_SCAN";
					ctrlWipIdEdit.SetWindowText(m_strWipId);
				}
				else
				{
					ctrlWipIdEdit.GetWindowText(sTmp);
					sTmp.TrimRight(); sTmp.TrimLeft();
					
					if (sTmp.IsEmpty())
					{
						m_strWipIdSave = "";
						m_strWipId = "NO_SCAN";
						ctrlWipIdEdit.SetWindowText(m_strWipId);
					}
					else
					{
						sTmp.TrimRight(0x0a);
						sTmp.TrimRight(0x0d);
						//m_strWipId = sTmp;
						m_strWipIdSave = sTmp;
						m_strWipId = "NO_SCAN";
						ctrlWipIdEdit.SetWindowText(m_strWipId);
						//	m_strWipId = "GO43Q1001E";
						//	m_strWipId = "TS372100XL";
					}
				}				
			}
#endif
			
			CurrentSet->sPCBID_Scan = m_strWipId;
			CurrentSet->sPCBID_Read = "";
			CurrentSet->bPCBID_Rewrite = 0;


			CurrentSet->nDftSeq = 0;
			CurrentSet->nDftNgCount = 0;
			CurrentSet->nDftOkCount = 0;

			nNoCheckedStep = GetNoCheckedStep(CurrentSet->nDisplayType);
			//if (CurrentSet->bUploadMes) {

			//	if (nNoCheckedStep == 0) {
			//		sMsg.LoadString(IDS_EMPTY_STEP);
			//		AfxMessageBox(sMsg);
			//		CurrentSet->bIsRunMsg = FALSE; return 0;
			//	}
			//}
				
			if(CurrentSet->bUploadMes){
			
				nNoCheckedStep = GetNoCheckedStep(CurrentSet->nDisplayType);

				if(nNoCheckedStep == 0){
					sMsg.LoadString(IDS_EMPTY_STEP);
					AfxMessageBox(sMsg);
					CurrentSet->bIsRunMsg = FALSE; return 0;
				}

				if(CurrentSet->bUseScanner)
				{
					//if (!StepCheck()) {
					//	CurrentSet->bIsRunMsg = FALSE; return 0;
					//}
					//(CurrentSet->bUsePLCRobot == TRUE)
					if(CurrentSet->nToolOptionSetMethod == 0){
						Sleep(500);
						if(!GetToolOption()){
							CurrentSet->bIsRunMsg = FALSE; return 0;
						}
					}
					//if(CurrentSet->bNgCountCheck){
					if(!CheckNgCount()){
						sTmp.Format("NG Count Check Error....");
						AfxMessageBox(sTmp);
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}
					if(CurrentSet->nDftNgCount >= 3){
						ReportSend_NgCount();
						sTmp.Format("%s : NG history is more than 3 times.", m_strWipId);
						AfxMessageBox(sTmp);
						CurrentSet->bIsRunMsg = FALSE; return 0;
					}
					//}
				}
			}
		}


		AudioMeasureStop();
//		if(g_SoundCard.wIn_Flag) g_SoundCard.WaveRead_Stop();

		//=======================
		// Stop Auto-Grab Thread
		//=======================
//FHD
		if(g_nGrabberType == FHD_GRABBER)
		{
			StopLVDSGrabThread_FHD();
		}//
		//if (CurrentSet->bUseScanner && CurrentSet->bGMES_Connection && CurrentSet->bUploadMes) {
			
		if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes &&(nType == 0))
		{
			/*	m_ctrlSummaryGrid.SetCol(0);
			for(int nTmp = 0; nTmp <= StepList.GetCount(); nTmp++)
			{
				m_ctrlSummaryGrid.SetRow(nTmp);
				m_ctrlSummaryGrid.SetCellChecked(flexChecked);
			}*/

			for (int nTmp = 0; nTmp <= StepList.GetCount(); nTmp++)
			{				
				m_CtrlListMainProcess.SetCheck(nTmp, 1);
				m_LockStatus[nTmp] = 1;
			}
		}
		SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);

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
//#ifdef _DEBUG
//	return 0;
//#endif
	int nLeft=0, nTop=0, nWidth=0, nHeight=0;

	CString sTmp = _T("");
	CString sMsg = _T("");

	BOOL bReturn = TRUE;


	m_InitDialogEnd = 0;
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
//AUTO
//
	if(g_nSysType != AUTO_SYS){
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
		else if (CurrentSet->bUseStartBox)
		{
			if ((g_hCommWnd = GetSafeHwnd()) == NULL)
			{
				AfxMessageBox("View Handle Null!");
			}
			sMsg = _T("[Start Box] Port Open Check = ");

			if (ScannerCtrl.m_bPortOpen == FALSE)
			{
				if (ScannerCtrl.Create(CurrentSet->sStartBoxComPort, CurrentSet->wStartBoxBaudRate))

				{
					sMsg += "PASS";
					if (g_nUseNoScanType == TRUE)//ScannerCtrl
					{
						ScannerCtrl.SendOK();
					}
				}
				else
				{
					sMsg += "FAIL";
					if (g_nUseNoScanType == TRUE)//ScannerCtrl
					{
						ScannerCtrl.SendNG();
					}

				}
			}
			else
			{
				sMsg += "PASS";
				if (g_nUseNoScanType == TRUE)//ScannerCtrl
				{
					ScannerCtrl.SendOK();
				}
			}

			m_pInitDlg->AddString2List(sMsg);
		}
	}
	//+add 090203(Modification No5)
	m_pInitDlg->m_ctrlProgress.SetPos(30);

	//==========
	// Init I2C
	//==========
	if(CurrentSet->bUseI2cAdc)
	{
		sMsg = _T("[I2C-ADC] Port Open Check = ");
		
		if(I2cAdcCtrl.m_bPortOpen == FALSE)
		{
			if(!I2cAdcCtrl.Create(CurrentSet->sI2cAdcComPort, CurrentSet->wI2cAdcBaudRate)) sMsg += "FAIL";
		}

		if(I2cAdcCtrl.m_bPortOpen)
		{
			int nResult = TEST_PASS;

			if((nResult = I2cAdcCtrl.SetI2cClkSpeed(CurrentSet->nI2cClkSpeed)) != TEST_PASS)
			{
				goto END_INIT_I2C;
			}
			
			if((nResult = I2cAdcCtrl.SetI2cPacketDelay(CurrentSet->nI2cPacketDelay)) != TEST_PASS)
			{
				goto END_INIT_I2C;
			}

			if((nResult = I2cAdcCtrl.SetI2cReadByteDelay(CurrentSet->nI2cReadByteDelay)) != TEST_PASS)
			{
				goto END_INIT_I2C;
			}

			if((nResult = I2cAdcCtrl.SetI2cWritByteDelay(CurrentSet->nI2cWriteByteDelay)) != TEST_PASS)
			{
				goto END_INIT_I2C;
			}

			CurrentSet->nAvrFW_Ver = I2cAdcCtrl.GetFWVersion();
			I2cAdcCtrl.SetChannel("00");
			I2cAdcCtrl.SetDDC2AB();

END_INIT_I2C:

			if(nResult == TEST_PASS)
			{
				sTmp.Format("AVR F/W Ver : %d.0", CurrentSet->nAvrFW_Ver);
				m_pInitDlg->AddString2List(sTmp);
				sMsg += "PASS";
			}
			else
			{
				sMsg += "FAIL";
			}
		}
		m_pInitDlg->AddString2List(sMsg);
		sTmp.Format("AVR F/W Ver : %d", CurrentSet->nAvrFW_Ver);
	}
	//+add 090203(Modification No5)
	m_pInitDlg->m_ctrlProgress.SetPos(40);

	//+add 090203(Modification No5)
	m_pInitDlg->m_ctrlProgress.SetPos(50);

	
	//========================
	// Init HDMI Generator 
	//========================
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
				if (HDMIGeneratorCtrl.SetDFT_Mode(1))
				{
					sMsg += " (DFT Booting OK)";
				}
				else
				{
					sMsg += " (DFT Booting Fail)";
				}
			}
			else{
				HDMIGeneratorCtrl.m_nRemoteMode = FALSE;
				sMsg = "[HDMI Gen.] On Line Mode = Off(Com. Error)- FAIL";
			}
			HDMIGeneratorCtrl.SetEDID_PassCheck(CurrentSet->bHdmiEdidControl) ;
			Sleep(100);
			HDMIGeneratorCtrl.SetHDCP_OnOff(CurrentSet->bHdmiHdcpControl) ;
			Sleep(100);
			HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);



			Sleep(100);
			g_pView->Set_Mhl(1, 1);

			Sleep(100);
			if(!CurrentSet->bHdmiOutControl){
				HDMIGeneratorCtrl.SetOutPort(0);
			}

			m_pInitDlg->AddString2List(sMsg);
		}

	}

	//+add 090203(Modification No5)
	m_pInitDlg->m_ctrlProgress.SetPos(60);

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
		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
		sMsg.Format("[AVSwitch] Set Audio CH 1 %s",  AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);

		if(!bReturn) goto END_INIT;

		for(int nTmp = 1; nTmp <= MAX_DIO_PORT; nTmp++)
		{
			bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_DIGIT_OUT, MAX_AVSWITCH_WAIT_DELAY,  nTmp, 0);
			sMsg.Format("[AVSwitch] Set DIO Port %d Off %s", nTmp, AvSwitchBoxCtrl.m_strErrMsg);
			m_pInitDlg->AddString2List(sMsg);
			if(!bReturn) goto END_INIT;
		}

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  1, DC_OUT_VOLTAGE_1_0);
		sMsg.Format("[AVSwitch] Set DC OUT CH1 Volt to 1.0V %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_ANALOG_VOLT, MAX_AVSWITCH_WAIT_DELAY,  2, DC_OUT_VOLTAGE_1_0);
		sMsg.Format("[AVSwitch] Set DC OUT CH2 Volt to 1.0V %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_COMP_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
		sMsg.Format("[AVSwitch] Set COMP CH 1 %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_CVBS_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
		sMsg.Format("[AVSwitch] Set DVBS CH 1 %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;
		
		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
		sMsg.Format("[AVSwitch] Set VIDEO CH 1 %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  1, 0);
		sMsg.Format("[AVSwitch] Set Remote Type 1 %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg);
		if(!bReturn) goto END_INIT;

		bReturn = AvSwitchBoxCtrl.SetAvSwitch(SET_REMOTE_REPEAT_NO, MAX_AVSWITCH_WAIT_DELAY,  3, 0);
		sMsg.Format("[AVSwitch] Set Remote Repeat No %s", AvSwitchBoxCtrl.m_strErrMsg);
		m_pInitDlg->AddString2List(sMsg); 
		if(!bReturn) goto END_INIT;
	
		sMsg = _T("[AVSwitch] Port Open Check = PASS");
		m_pInitDlg->AddString2List(sMsg); 
	}


END_INIT :

	//====================
	// UHD Grabber Connection
	//====================
	//UHD

	if (CurrentSet->bPJT_GrabDisable == 0)
	{
		if (g_nGrabberType == UHD_GRABBER)
		{
			sMsg = _T("[UHD Grabber] Connection Check = ");

			if (GrabberOpen() == FALSE) {
				sMsg += "FAIL";
			}
			else {
				sMsg += "PASS";
			}
			m_pInitDlg->AddString2List(sMsg);
		}
	}

	m_pInitDlg->m_ctrlProgress.SetPos(70);

	//=================
	// Init LNB Detector 
	//=================
	if(CurrentSet->bUseLNB)
	{
		sMsg = _T("[LNB Detector] Port Open Check = ");
		
		if(gLNBDetectorCtrl.m_bPortOpen == FALSE)
		{
			if(gLNBDetectorCtrl.Create(CurrentSet->sLNBComPort, CurrentSet->wLNBBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
	}
	m_pInitDlg->m_ctrlProgress.SetPos(80);

	//=================
	// Init AV Convertor 
	//=================
	if(CurrentSet->bUseAVC)
	{
		sMsg = _T("[AV Convertor] Port Open Check = ");
		
		if(gAvConvertorCtrl.m_bPortOpen == FALSE)
		{
			if(gAvConvertorCtrl.Create(CurrentSet->sAvcComPort, CurrentSet->wAvcBaudRate)) sMsg += "PASS";
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
	}
	m_pInitDlg->m_ctrlProgress.SetPos(90);
//AUTO
	//========================
	// Init Jig 
	//========================
	if(1)//g_nSysType == AUTO_SYS)
	{
		if(CurrentSet->bUseJig)
		{
			sMsg = _T("[Jig] Port Open Check = ");

			if(gJigCtrl.m_bPortOpen == FALSE)
			{
				if(gJigCtrl.Create(CurrentSet->sJigComPort, CurrentSet->wJigBaudRate)){
					sMsg += "PASS";
					Sleep(1000);
					if (g_nSysType == AUTO_SYS)
					{
						if (gJigCtrl.Get_JigReady()) {
							CurrentSet->nJigStatus = IN_JIGDN;
						}
						else {
							CurrentSet->nJigStatus = IN_JIGUP;
						}

						gJigCtrl.Set_CylinderUP();
					}
					else
					{
						gJigCtrl.Set_Grabber_Power(1);
					}

				}
				else sMsg += "FAIL";
			}
			else sMsg += "PASS";

			m_pInitDlg->AddString2List(sMsg);
			//+add 090922(Modification No1)
			if(gJigCtrl.m_bPortOpen == TRUE)
			{			
				gJigCtrl.Init();
			}

		}
	}
    //-
//
	//=================
	// Init V-F Measurement 
	//=================
	if(CurrentSet->bUseVfm)
	{
		sMsg = _T("[V-F Measurement] Port COM Check = ");
		
		if(gVF1000Ctrl.m_bPortOpen == FALSE)
		{
			if (gVF1000Ctrl.Create(CurrentSet->sVfmComPort, CurrentSet->wVfmBaudRate))
			{

				double dVol;
				CString strRead;
				gVF1000Ctrl.m_bSystemMixType = _IR_MIX_TYPE;//  _VF_ONLY_TYPE
#ifdef				DEBUG_IRLANTEST__CODE__
				if (gVF1000Ctrl.GetIRBlasterData(strRead, 1000) )
#else
				if (gVF1000Ctrl.GetVoltageData(1, dVol))
#endif
				{// {
					sMsg += "M PASS";
				}
				else 
				{				
					gVF1000Ctrl.m_bSystemMixType = _VF_ONLY_TYPE;
					if (gVF1000Ctrl.GetVoltageData(1, dVol)) {
						sMsg += "V PASS";
					}
					else
					{
						sMsg += "FAIL";
					}					
				}

			//	sMsg += "PASS";

			}
			else sMsg += "FAIL";
		}
		else sMsg += "PASS";

		m_pInitDlg->AddString2List(sMsg);
	}

	if (CurrentSet->bUseDpg)
	{
		sMsg = _T("[Display Port] Port COM Check = ");

		if (DPGeneratorCtrl.m_bPortOpen == FALSE)
		{
			if (DPGeneratorCtrl.Create(CurrentSet->sDpgComPort, CurrentSet->wDpgBaudRate))
			{

				double dVol;
				if (DPGeneratorCtrl.SetTime_Control(CurrentSet->nDP_TimeSel))
				{
					sMsg += "PASS";
				}
				else 
				{					
					sMsg += "FAIL";			

				}				

			}
			else sMsg += "FAIL";
		}
		else if (DPGeneratorCtrl.SetTime_Control(CurrentSet->nDP_TimeSel))
		{
			sMsg += "PASS";
		}
		else
		{
			sMsg += "FAIL";

		}

		m_pInitDlg->AddString2List(sMsg);
	}

	m_pInitDlg->m_ctrlProgress.SetPos(80);


	//=================
	// GMES 
	//=================
	if(CurrentSet->bUseGmes){
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
#if 1

	if (g_nSysType == AUTO_SYS)
	{
		if (g_nSysRobotType == AUTO_SYS)
		{
			sMsg = "[PLC Connection] Check = ";
			if (gPLC_Ctrl.SoketCom_Creat(CurrentSet->strPLC_IP, CurrentSet->nPLC_Port)) {
				sMsg += "PASS";
				//	sTmp = gPLC_Ctrl.MakeElem_S1F1();
				CurrentSet->bPLC_Connection = TRUE;
				
				ctrlPLC_COM_Status.SetWindowText(_T("PLC ON"));
				ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
				ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));

				g_AutoControlProcess = _AUTO_ROBOT_CTRL_PLC_ON;
				g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;

				//SetTimer(TIMER_AUTO_CONTROL_ID, 500, NULL);
				OnBnClickedButtonAutoRobotStop();
				gPLC_Ctrl.CommandClear();
				
			}
			else
			{
				ctrlPLC_COM_Status.SetWindowText(_T("PLC OFF"));
				ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
				ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
				sMsg += "FAIL";
			}
			m_pInitDlg->AddString2List(sMsg);

			//////////////////////////////////////SERVER
			if (CurrentSet->bUseServerCtrlConnect == 1)
			{
				sMsg = "[Server Connection] Check = ";
				if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port)) {
					sMsg += "PASS";
					//	sTmp = gPLC_Ctrl.MakeElem_S1F1();
					CurrentSet->bMonitorServer_Connection = TRUE;
					//	m_LedsMesOn.EnableWindow(CurrentSet->bGMES_Connection);
					//	m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
						//if (gPLC_Ctrl.SendCommString(sTmp))
						//{
						//	gPLC_Ctrl.DisplyComStatus("S1F1", FALSE);
						//	//	gGmesCtrl.ReceiveCommString(1000,sTmp);
						//	//	gGmesCtrl.Paser_S1F2_S2F31(sTmp);
						//}
		/*			ctrlPLC_COM_Status.SetWindowText(_T("PLC ON"));
					ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
					ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));*/

					//g_AutoControlProcess = _AUTO_ROBOT_CTRL_PLC_ON;
					//g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;

					//SetTimer(TIMER_AUTO_CONTROL_ID, 500, NULL);

					//gServer_Ctrl.CommandClear();
				}
				else
				{
					CurrentSet->bMonitorServer_Connection = FALSE;
					//ctrlPLC_COM_Status.SetWindowText(_T("PLC OFF"));
					//ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
					//ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
					sMsg += "FAIL";
					gServer_Ctrl.m_bCom_RetryCount = 3;
				}
				m_pInitDlg->AddString2List(sMsg);
			}
		}

	}
	else
	{
		//ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
		//ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
		//ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
	}

#else
	if (g_nSysType == AUTO_SYS)
	{
		if (g_nSysRobotType == AUTO_SYS)
		{
			sMsg = "[PLC Connection] Check = ";
			if (gPLC_Ctrl.SoketCom_Creat(CurrentSet->strPLC_IP, CurrentSet->nPLC_Port)) {
				sMsg += "PASS";
				//	sTmp = gPLC_Ctrl.MakeElem_S1F1();
				CurrentSet->bPLC_Connection = TRUE;
				//	m_LedsMesOn.EnableWindow(CurrentSet->bGMES_Connection);
				//	m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
					//if (gPLC_Ctrl.SendCommString(sTmp))
					//{
					//	gPLC_Ctrl.DisplyComStatus("S1F1", FALSE);
					//	//	gGmesCtrl.ReceiveCommString(1000,sTmp);
					//	//	gGmesCtrl.Paser_S1F2_S2F31(sTmp);
					//}
				ctrlPLC_COM_Status.SetWindowText(_T("PLC ON"));
				ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
				ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));

				g_AutoControlProcess = _AUTO_ROBOT_CTRL_PLC_ON;
				g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;

				//SetTimer(TIMER_AUTO_CONTROL_ID, 500, NULL);
				OnBnClickedButtonAutoRobotStop();
				gPLC_Ctrl.CommandClear();
			}
			else
			{
				ctrlPLC_COM_Status.SetWindowText(_T("PLC OFF"));
				ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
				ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
				sMsg += "FAIL";
			}
			m_pInitDlg->AddString2List(sMsg);

			//////////////////////////////////////SERVER
			if (CurrentSet->bUseServerCtrlConnect == 1)
			{
				sMsg = "[Server Connection] Check = ";
				if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port)) {
					sMsg += "PASS";
					//	sTmp = gPLC_Ctrl.MakeElem_S1F1();
					CurrentSet->bMonitorServer_Connection = TRUE;
					//	m_LedsMesOn.EnableWindow(CurrentSet->bGMES_Connection);
					//	m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
						//if (gPLC_Ctrl.SendCommString(sTmp))
						//{
						//	gPLC_Ctrl.DisplyComStatus("S1F1", FALSE);
						//	//	gGmesCtrl.ReceiveCommString(1000,sTmp);
						//	//	gGmesCtrl.Paser_S1F2_S2F31(sTmp);
						//}
		/*			ctrlPLC_COM_Status.SetWindowText(_T("PLC ON"));
					ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
					ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));*/

					//g_AutoControlProcess = _AUTO_ROBOT_CTRL_PLC_ON;
					//g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;

					//SetTimer(TIMER_AUTO_CONTROL_ID, 500, NULL);

					//gServer_Ctrl.CommandClear();
				}
				else
				{
					CurrentSet->bMonitorServer_Connection = FALSE;
					//ctrlPLC_COM_Status.SetWindowText(_T("PLC OFF"));
					//ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
					//ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
					sMsg += "FAIL";
				}
				m_pInitDlg->AddString2List(sMsg);
			}
		}

	}
	else
	{
		ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
		ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
		ctrlPLC_COM_Status.SetTextColor(RGB(0, 0, 0));
	}

#endif

	m_pInitDlg->m_ctrlProgress.SetPos(100);
	sMsg = _T(".....");
	m_pInitDlg->AddString2List(sMsg);
	sMsg = _T("Device Check Complete!!");
	m_pInitDlg->AddString2List(sMsg);

	SetTimer(TIMER_MES_DATA_UPLOAD, 10000, NULL);
	SetTimer(TIMER_SERVER_CONTROL_ID, 10000, NULL);

	m_InitDialogEnd = 1;
	return 0;
}

BOOL CDATsysView::InitAvSwitchController() 
{

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
	
	CString sTmp = _T("");
	CString sMsg = _T("");

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_AUDIO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set Audio CH 1 %s!\n",  AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	for(int nTmp = 1; nTmp <= MAX_DIO_PORT; nTmp++)
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

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_VIDEO_CH, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set VIDEO CH 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(REMOTE_TYPE_SEL, MAX_AVSWITCH_WAIT_DELAY,  1, 0))
	{
		sTmp.Format("Set Remote Type 1 %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	if(!AvSwitchBoxCtrl.SetAvSwitch(SET_REMOTE_REPEAT_NO, MAX_AVSWITCH_WAIT_DELAY,  3, 0))
	{
		sTmp.Format("Set Remote Repeat No %s!\n", AvSwitchBoxCtrl.m_strErrMsg);
		sMsg += sTmp;
	}

	return TRUE;
}

UINT CDATsysView::StartTestThread(LPVOID pParam) 
{
	CDATsysView* pView =(CDATsysView* )pParam;
	pView->m_CurrentStep = 0;
	pView->CtrlBatVerEdit.SetReadOnly(1);
	pView->m_BatVerReadOnly = 1;

	HDMIGeneratorCtrl.m_bResetPatternModel = 1;
	HDMIGeneratorCtrl.m_bResetHDCP_EDID = 1;

	CurrentSet->sCPUVersionRead = "";

	SystemMonitorLog_Save("StartTestThread(LPVOID pParam) ");
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
	
#ifdef _ABORT_DEBUG__MODE
	while (1)
	{
		_Moving_Pic_Test();
	}
	return 0;
#endif

//	BOOL bRunAdcTest	= FALSE;
//#ifdef ERR_DEBUG	
//	g_pView->m_bWriteMassageFlag = 0;
//	while (1)
//	{
//		Sleep(20);
//		AddStringToStatus("TEST");
//	}
//#endif
	pView->m_bThreadRunning = TRUE;

	if (g_nUseNoScanType == TRUE)//ScannerCtrl
	{
		ScannerCtrl.SendClear();
	}
	
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
	



	//pView->m_cStepResult.SetBitmap(pView->m_bmpNormal);
	pView->m_cStepResultNew.SetWindowText(_T("READY"));
	pView->m_cStepResultNew.SetBkColor(RGB(128, 200, 64));

	pView->m_szCurrentStatus = pView->m_szVersion;
	pView->m_nCurrentViewImageType = SNAP_IMAGE;

	I2cAdcCtrl.m_nI2cAdcSeqNo = 0;

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

	CurrentSet->nDrmKeyResultVal = 0;
	CurrentSet->nInputCheckResultVal = 0;

	pView->m_A_CAS_ID_Check = "";

//	sTime = "";
//	pView->UpdateInfo(TIME, "");

//+ add PSH 080520
	//CString StrTmpLog;
	//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);

	pView->Invalidate();
	pView->PostMessage(UM_UPDATE_DISPLAY,ALL_VIEW, 0);
	_Wait(200);

	g_ImageProc.SetPositionShift(ptPositionShift);
	g_ImageProc.SetAllowedErrorPixel(CurrentSet->fAllowedBandErrorPixelPercentage);
	
	// Get the position of first step-item.
    PosStep = StepList.GetHeadPosition();       
	//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	// add 090821
//	pView->m_bSeq_Lock = TRUE;
//	m_ctrlSummaryGrid.EnableWindow(FALSE);
	//-

	// Get a run mode
	nRunMode = CurrentSet->nRunType;            
	
	// Init g_CurSetting Object
	g_CurSetting.InitVariables();
	//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
    // Disable program closing menu item
//    pView->m_pFrame->GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

 //   pView->m_pFrame->GetMenu()->EnableMenuItem(ID_APP_EXIT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	//090615
	//if(!g_SoundCard.wIn_Flag) g_SoundCard.WaveRead_Start();
//	g_pView->AudioMeasureStart();


	if(CurrentSet->nToolOptionSetMethod == 2){
		szTemp1.Format(_T("%s\\Data\\%s\\%s\\%s"),pView->m_szExePath, sDate, CurrentSet->sChassisName, CurrentSet->sModelName);
	}
	else{
		if(CurrentSet->sModelSuffixName.IsEmpty()){
			szTemp1.Format(_T("%s\\Data\\%s\\%s\\%s"),pView->m_szExePath, sDate, CurrentSet->sChassisName, CurrentSet->sModelName);
		}
		else{
			szTemp1.Format(_T("%s\\Data\\%s\\%s\\%s"),pView->m_szExePath, sDate, CurrentSet->sChassisName, CurrentSet->sModelSuffixName);
		}
	}

/*	if(g_nRunningProcessNo == 1)
	{
		szTemp1 = szTemp1 + _T("\\Jig1");
	}
	else{
		szTemp1 = szTemp1 + _T("\\Jig2");
	} */
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
	
	if (!FileExists(szTemp1)) CreateFullPath(szTemp1);

	// change 090410
	if (CurrentSet->bSaveProcessingTimeData)
	{
		if (!CreateTimeDataFile(sDate, sTime))
		{
			AfxMessageBox("Failed to creat time data file!");
		}
		dwFuncTickCount = GetTickCount();
	}
	//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	if (g_nGrabberType == UHD_GRABBER)
	{
		pView->CheckFWVer();
	}
#if 1
	if (CurrentSet->bHdmiEdidControl)
	{
		if (!HDMIGeneratorCtrl.m_bEDIDMode) {
			if (CurrentSet->nHDMIGenType == 0) {
				HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE);
			}
			else {
				HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE);
			}

			Sleep(100);
		}
	}
	else
	{
		if (HDMIGeneratorCtrl.m_bEDIDMode != CurrentSet->bHdmiEdidControl) {
			HDMIGeneratorCtrl.SetEDID_PassCheck(CurrentSet->bHdmiEdidControl);
			Sleep(100);
		}
	}
	if (CurrentSet->bHdmiHdcpControl)
	{
		if (!HDMIGeneratorCtrl.m_bHDCPMode) {
			if (CurrentSet->nHDMIGenType == 0) {
				HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE);
			}
			else {
				HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE);
			}
			Sleep(100);
		}
	}
	else
	{
		if (HDMIGeneratorCtrl.m_bHDCPMode != CurrentSet->bHdmiHdcpControl) {
			HDMIGeneratorCtrl.SetHDCP_OnOff(CurrentSet->bHdmiHdcpControl);
			Sleep(100);
		}
	}
#else
	if(!HDMIGeneratorCtrl.m_bEDIDMode){
		if(CurrentSet->nHDMIGenType == 0){
			HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;
		}
		else{
			HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;
		}
	
		Sleep(100);
	}
	AddStringToStatus("Debug Check EDID");
	if(!HDMIGeneratorCtrl.m_bHDCPMode){
		if(CurrentSet->nHDMIGenType == 0){
			HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE) ;
		}
		else{
			HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE) ;
		}
		Sleep(100);
	}
	AddStringToStatus("Debug Check HDCP");

#endif
	if(HDMIGeneratorCtrl.m_bCECMode != CurrentSet->bHdmiCecControl){
		HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);
		Sleep(100);
	}
	if(g_pView->m_bMhlMode){
		g_pView->Set_Mhl(1, TRUE);
	}

	if(g_pView->m_bMakeReferenceMode) 
	{
		pView->m_szCurrentStatus = _T("REF-IMG"); 
	}
	else
	{
		pView->m_szCurrentStatus = _T("TESTING"); 
	}
	pView->SendMessage(UM_UPDATE_DISPLAY,STATUS, 0);


	//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	// Initalize a test result for all steps
	// Set m_bResult TRUE, Set m_fMeasured 0.0
    pView->ResetStep();


	// Initialize variables
	ctrlTestProgress.SetRange(0,100);
	ctrlTestProgress.SetPos(g_nProgressPos);
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	if(CurrentSet->nRunType == STEP)
	{
		CurrentSet->nStepKey = NEXT;
	}

	if(CurrentSet->bSaveProcessingTimeData)
	{
		dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
		szFuncElapsedTime.Format("[%dms]",dwFuncElapsedTime);
		AddTimeData("Before Step Loop " + szFuncElapsedTime);
	}
//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_GrabSource = LVDS_GRABBER;
		if(pView->m_bGrabThreadRunning){
			pView->StopLVDSGrabThread();
			Sleep(100);
			pView->StartLVDSGrabThread();

		}
		else{
			pView->StartLVDSGrabThread();
		}
	}//
	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	g_pView->AudioMeasureStart();
//AUTO
	if(g_nSysType == AUTO_SYS){
		gJigCtrl.Set_TestStatus(0x01);
	}
//
	CurrentSet->bTestTimeCount = TRUE;
    Start = clock();

	SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);

	g_ForcedInputCheck = 0;
	g_ID_Check = 0;

	while(PosStep != NULL && CurrentSet->bIsRunning && !TestEnd)
	{
#if 1
		if (g_ID_Check == 2)
		{
			//if (g_pView->m_bMakeReferenceMode == FALSE)// && (CurrentSet->bUseGmes) && (CurrentSet->bGMES_Connection) && (CurrentSet->bUploadMes))
			if ((g_pView->m_bMakeReferenceMode == FALSE) && (CurrentSet->bUseGmes) && (CurrentSet->bGMES_Connection) && (CurrentSet->bUploadMes))
			{

				if ((g_nUseNoScanType == TRUE) && (m_strWipId.Find("NO_SCAN") >= 0))
				{
					if (g_pView->Read_PCBAID_READ())
					{
						g_ID_Check = 3;
					}
					else
					{
						if (g_pView->Read_PCBAID_READ())
						{
							g_ID_Check = 3;
						}
						else
						{

							AddStringToStatus("=>NG:PCBAID READ FAIL!!");
							CurrentSet->bRunAbort = 1;
							pCurStep->m_bResult = FALSE;

							g_ID_Check = 4;
							goto END_WHILE_LOOP;
						}
					}
				}
				else
				{
					if (g_pView->Read_PCBAID_READ())
					{
						g_ID_Check = 3;
					}
					else
					{
						AddStringToStatus("=>NG:PCBAID READ FAIL!!");
						CurrentSet->bRunAbort = 1;
						pCurStep->m_bResult = FALSE;

						g_ID_Check = 4;
						goto END_WHILE_LOOP;
						
					}
				}
			}
			else
			{
				CurrentSet->sPCBID_Read = "GMES OFF";
				g_ID_Check = 3;
			}

		}
#else
		if (g_ID_Check == 2)
		{
			if ((g_pView->m_bMakeReferenceMode == FALSE) && (CurrentSet->bUseGmes)&&(CurrentSet->bGMES_Connection) && (CurrentSet->bUploadMes))
			{

				if( (g_nUseNoScanType == TRUE) && (m_strWipId.Find("NO_SCAN") >= 0))				
				{
					if (g_pView->Read_PCBAID_READ())
					{
						g_ID_Check = 3;
					}
					else
					{
						//if (g_nSysType == AUTO_SYS) {
						//	if (!pView->RunUserRetry(nStepResult)) 
						//}

						if (g_pView->Read_PCBAID_READ())
						{
							g_ID_Check = 3;
						}
						else
						{

							AddStringToStatus("=>NG:PCBAID READ FAIL!!");
							CurrentSet->bRunAbort = 1;
							pCurStep->m_bResult = FALSE;

							g_ID_Check = 4;
							goto END_WHILE_LOOP;
						}
					}
				}
				else
				{
					g_ID_Check = 3;
					if (g_pView->Read_PCBAID_READ())
					{
						g_ID_Check = 3;
					}
					else
					{						
						if (g_pView->Read_PCBAID_READ())
						{
							g_ID_Check = 3;
						}
						else
						{
							AddStringToStatus("=>NG:PCBAID READ FAIL!!");
							CurrentSet->bRunAbort = 1;
							pCurStep->m_bResult = FALSE;

							g_ID_Check = 4;
							goto END_WHILE_LOOP;
						}
					}
				}
			}
			else
			{
				g_ID_Check = 3;
				if (g_pView->Read_No_GMES_PCBAID_READ())
				{

				}
			}
			
		}
#endif	
		
		StepStart = clock();

		if(CurrentSet->bSaveProcessingTimeData)
		{
			dwFuncTickCount = GetTickCount();
		}

		pView->m_bResultDlgActivate = FALSE;

		//+add kwmoon 071227 : For updating screen
	//	_Wait(20);
//FHD	
		if(g_nGrabberType == FHD_GRABBER)
		{
			if(pView->m_bContinuousLVDSGrabRunning)
			{
				pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
			}
		
			g_GrabSource = LVDS_GRABBER;
		}//

		g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 
		g_ImageProc.m_bInvalidateInvalidRegion = FALSE;
	
		if(CurrentSet->bRunAbort)
		{
			pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
		}
		
		//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
		SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
	/*	if(CurrentSet->bSaveProcessingTimeData)
		{
			dwFuncElapsedTime = GetTickCount()-dwFuncTickCount;
			szFuncElapsedTime.Format("[%dms]",dwFuncElapsedTime);
			AddTimeData("Before Step Run " + szFuncElapsedTime);
			pView->m_dwStepElapsedTime = GetTickCount();
		}*/

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

					if((nStepResult = pView->StepRun()) == TEST_ABORT)
					{
						pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
					}
					//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
					SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
				}
				else // Normal Process
				{
					pCurStep = StepList.GetNext(PosStep);

					if(CurrentSet->bSaveProcessingTimeData)
					{
						szMsg2.Format("%d.<%s>",pCurStep->m_nStep,pCurStep->m_sName);
						AddTimeData(szMsg2);
						dwFuncTickCount = GetTickCount();
					}
//FHD
					if(g_nGrabberType == FHD_GRABBER)
					{
						if((pCurStep->m_nStepType == PROCESS_FUNCTION) && (pCurStep->m_bTest))				
						{
							if(!pView->m_bContinuousLVDSGrabRunning)
							{
								pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
							}
						}
						if((pCurStep->m_nTestType >= LAN_TEST) && (pCurStep->m_bTest))
						{
							if(!pView->m_bContinuousLVDSGrabRunning)
							{
								pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
							}
						}
					}
//
					//gStrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
					SystemMonitorLog_Save("Position", _T(__FILE__), __LINE__);
					//==================
					// Run Current Step 
					//==================
					pView->m_nCurrentResetExecution = 1;
					if((nStepResult = pView->StepRun()) == TEST_ABORT) // TEST_ABORT
					{
						pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
					}
					else // TEST_PASS or TEST_FAIL
					{
						// TEST_FAIL
						//change 090305
					//	if(pCurStep->GetResult() == FALSE)

					//	if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST)
					//	if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST &&  pCurStep->m_nTestType != BCM_CHIP_TEST)
						if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST &&  pCurStep->m_nTestType < BCM_CHIP_TEST)
						{
							if (g_nUseNoScanType == TRUE)//ScannerCtrl
							{
								ScannerCtrl.SendNG();
							}
							// Internal Retry
							pView->m_nCurrentResetExecution = 0;
							if(CurrentSet->nNoRetry > 0)
							{
								if(!pView->RunInternalRetry()) goto END_WHILE_LOOP;
							}

							// User Retry
							if(pCurStep->GetResult() == FALSE)
							{
								if(g_nSysType == AUTO_SYS){
									if(!pView->RunUserRetry(nStepResult)) goto END_WHILE_LOOP;
								}
								else{
									for(int nLoop = 0; nLoop < 3; nLoop++)
									{
										if(!pView->RunUserRetry(nStepResult)) goto END_WHILE_LOOP;
										if(pCurStep->GetResult()) break;
									}
								}
							}
						}
					}
				}
			}
			break;
			
			case ERRORSTEP: 
			{
                pCurStep = StepList.GetNext(PosStep);

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
			//FHD
				if(g_nGrabberType == FHD_GRABBER)
				{
					if((pCurStep->m_nStepType == PROCESS_FUNCTION) && pCurStep->m_bTest				
					&& (pCurStep->m_nStep != CurrentSet->nTotalStep))
					{
						if(!pView->m_bContinuousLVDSGrabRunning)
						{
							pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
						}
					}
					if((pCurStep->m_nTestType >= LAN_TEST) && (pCurStep->m_bTest))
					{
				
						if(!pView->m_bContinuousLVDSGrabRunning)
						{
							pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
						}
					}
				}//
				if((nStepResult = pView->StepRun()) == TEST_ABORT)				
				{
					pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
				}
			}
			break;

			case STOP: 
			{
				pCurStep = StepList.GetNext(PosStep);


				if(CurrentSet->bSaveProcessingTimeData)
				{
					szMsg2.Format("%d.<%s>",pCurStep->m_nStep,pCurStep->m_sName);
					AddTimeData(szMsg2);
					dwFuncTickCount = GetTickCount();
				}
				//FHD
				if(g_nGrabberType == FHD_GRABBER)
				{
					if((pCurStep->m_nStepType == PROCESS_FUNCTION) && (pCurStep->m_bTest))				
					{
						if(!pView->m_bContinuousLVDSGrabRunning)
						{
							pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
						}
					}
				
					if((pCurStep->m_nTestType >= LAN_TEST) && (pCurStep->m_bTest))
					{
						if(!pView->m_bContinuousLVDSGrabRunning)
						{
							pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
						}
					}
				}//

				//==================
				// Run Current Step 
				//==================
				pView->m_nCurrentResetExecution = 1;
				pView->m_WhiteResetFlag = 0;
				if((nStepResult = pView->StepRun()) == TEST_ABORT) // TEST_ABORT
				{
					pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
				}
				else // TEST_PASS or TEST_FAIL
				{
					// TEST_FAIL
					//change 090305
				//	if(pCurStep->GetResult() == FALSE)
				//	if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST)
				//	if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST &&  pCurStep->m_nTestType != BCM_CHIP_TEST)
					if(pCurStep->GetResult() == FALSE && pCurStep->m_nTestType != MANUAL_TEST &&  pCurStep->m_nTestType < BCM_CHIP_TEST)
					{
						if (g_nUseNoScanType == TRUE)//ScannerCtrl
						{
							ScannerCtrl.SendNG();
						}

						if (pView->m_WhiteResetFlag == 1)
						{
							if (pCurStep->GetResult() == FALSE)
							{
								goto END_WHILE_LOOP;
							}
							else
							{
								g_pView->m_WhiteResetCount = 0;
							}
							//else if (g_pView->m_WhiteResetCount >= 3)
							//{
							//	gJigCtrl.Set_CylinderUP();
							//	AddStringToStatus("Jig Up!");
							//	Sleep(1000);
							//	for (int i = 0; i < 10; i++)
							//	{
							//		if (gJigCtrl.m_bJigUpStatus)
							//			break;
							//		Sleep(500);
							//	}
							//}							
							//pView->m_WhiteResetFlag = 0;
							
						}

						// Internal Retry
						pView->m_nCurrentResetExecution = 0;
						if(CurrentSet->nNoRetry > 0)
						{
							if(!pView->RunInternalRetry()) goto END_WHILE_LOOP;
						}

						// User Retry
						if(pCurStep->GetResult() == FALSE)
						{
							if(g_nSysType == AUTO_SYS){
								//pView->m_nCurrentResetExecution = 1;
								if(!pView->RunUserRetry(nStepResult)) goto END_WHILE_LOOP;
							}
							else{
								for(int nLoop = 0; nLoop < 3; nLoop++)
								{
									if(!pView->RunUserRetry(nStepResult)) goto END_WHILE_LOOP;
									if(pCurStep->GetResult()) break;
								}
							}
						}
					}
				}
			}
			break;
		}

		//////////////////////////////godtech
		if (HDMIGeneratorCtrl.m_bResetHDCP_EDID == 1)
		{
			HDMIGeneratorCtrl.m_bResetHDCP_EDID = 0;
			if (CurrentSet->bHdmiEdidControl)
			{
				if (CurrentSet->nHDMIGenType == 0) {
					HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE);
				}
				else {
					HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE);
				}

				Sleep(100);

			}
			else
			{
				if (HDMIGeneratorCtrl.m_bEDIDMode != CurrentSet->bHdmiEdidControl) {
					HDMIGeneratorCtrl.SetEDID_PassCheck(CurrentSet->bHdmiEdidControl);
					Sleep(100);
				}
			}
			if (CurrentSet->bHdmiHdcpControl)
			{
				if (CurrentSet->nHDMIGenType == 0) {
					HDMIGeneratorCtrl.SetHDCP_OnOff(TRUE);
				}
				else {
					HDMIGeneratorCtrl.SetHDCP_OnOff(FALSE);
				}
				Sleep(100);
			}
			else
			{
				if (HDMIGeneratorCtrl.m_bHDCPMode != CurrentSet->bHdmiHdcpControl) {
					HDMIGeneratorCtrl.SetHDCP_OnOff(CurrentSet->bHdmiHdcpControl);
					Sleep(100);
				}
			}
		
			
			
			
		}
//
//#define FORCED_CHECK_ID_HEADPHONE		0
//#define FORCED_CHECK_ID_GREENEYE		1
//#define FORCED_CHECK_ID_COMPONENT		2
//#define FORCED_CHECK_ID_AV				3
//#define FORCED_CHECK_ID_USB_1			4
//#define FORCED_CHECK_ID_USB_2			5
//#define FORCED_CHECK_ID_USB_3			6
//#define FORCED_CHECK_ID_HDMI_1			7
//#define FORCED_CHECK_ID_HDMI_2			8
//#define FORCED_CHECK_ID_HDMI_3			9
//#define FORCED_CHECK_ID_HDMI_4			10
//#define FORCED_CHECK_ID_RGB				11
//#define FORCED_CHECK_ID_SCART			12
//#define FORCED_CHECK_ID_MAX				13

		///////////////////

		if((CurrentSet->nRunType == STOP) && (pCurStep->GetResult() == FALSE))
		{
			if(pCurStep->m_nStepType == PROCESS_FUNCTION)
			{
				pView->m_szResultMsg2.Format("%s : Process NG", pCurStep->m_sName);
				CurrentSet->bSystemBreak = TRUE;
			}

			pView->m_szCurrentStatus = _T("STOPPED"); 
			pView->SendMessage(UM_UPDATE_DISPLAY,STATUS, 0); 

			TestEnd = TRUE;
		}

		pCurStep->SetElapsedTime((double)(clock() - StepStart) / CLOCKS_PER_SEC);
		
		if(CurrentSet->bSaveReviewData) SaveTestResult(CurrentSet->sResultPath);

		pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

	/*	if(CurrentSet->bSaveProcessingTimeData)
		{
			DWORD dwStepElapsedTime = GetTickCount()-pView->m_dwStepElapsedTime;
			szFuncElapsedTime.Format("  Step Total (%dms)",dwStepElapsedTime);
			AddTimeData(szFuncElapsedTime);
		}*/

		if (g_ForcedInputCheck == 1)
		{
			POSITION	PosForceStep;
			PosForceStep = ForcedAutoStepList.FindIndex(0);
			while(PosForceStep != NULL)
			{
				pCurStep = ForcedAutoStepList.GetAt(PosForceStep);

				if ((nStepResult = pView->StepRun()) == TEST_ABORT)
				{
					pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
				}
				if (pCurStep->m_bResult != TRUE) CurrentSet->nRunType = STEP;
			}
			g_ForcedInputCheck = 0;
		}


	}

END_WHILE_LOOP :

	sTime.Format("%5.2f",(double)(clock() - Start)/(CLOCKS_PER_SEC));

	CurrentSet->bTestTimeCount = FALSE;

	sTested.Format("%d", g_nNoTested);
    sFailed.Format("%d", g_nFailed);
    sUserStop.Format("%d", g_nUserStop);


//	g_pView->Set_Relay(0);
//AUTO
//	if(g_nSysType == AUTO_SYS){
//		gJigCtrl.Set_ACPower(0);
//	}
//
	if(FindResult()){
		g_nNoFailedStep = 0;
		if((CurrentSet->nAcPowerOffType == 0) || (CurrentSet->nAcPowerOffType == 2)){
			if(g_nSysType == AUTO_SYS){
				gJigCtrl.Set_ACPower(0);
				SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
			}
			else{
				g_pView->Set_Relay(0);
			}
		}
	}
	else{
		g_nNoFailedStep = 1;
		if((CurrentSet->nAcPowerOffType == 1) || (CurrentSet->nAcPowerOffType == 2)){
			if(g_nSysType == AUTO_SYS){
				gJigCtrl.Set_ACPower(0);
				SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
			}
			else{
				g_pView->Set_Relay(0);
			}
		}
	}
/*
//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_GrabSource = LVDS_GRABBER;
		if(pView->m_bGrabThreadRunning){
			pView->StopLVDSGrabThread();
			Sleep(100);
			pView->StartLVDSGrabThread();

		}
		else{
			pView->StartLVDSGrabThread();
		}
	}//
	else//FHD
	*/
	if(g_nGrabberType == FHD_GRABBER)
	{
		//----------------
		// Stop Auto Grab
		//----------------
		if(pView->m_bContinuousLVDSGrabRunning)
		{
			pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
		}
	}//

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
//	if(CurrentSet->bRunAbort)
//	{
//		g_nNoFailedStep++;
//	}

/*	sTime.Format("%5.2f",(double)(clock() - Start)/(CLOCKS_PER_SEC));
	sTested.Format("%d", g_nNoTested);
    sFailed.Format("%d", g_nFailed);
    sUserStop.Format("%d", g_nUserStop); */

	pView->UpdateInfo(TIME, sTime);

	AddStringToStatus("Test End....");

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
	//if(g_SoundCard.wIn_Flag) g_SoundCard.WaveRead_Stop();

/*	CurrentSet->bCommDisplay = FALSE;
	pView->InitAvSwitchController();
	CurrentSet->bCommDisplay = TRUE;
*/
	
	if(CurrentSet->bRunAbort)
	{
		pView->m_nNoCurrentRepeatExecution = 1;
	}
	
	pView->m_szResultMsg1 = _T("");
	pView->m_szResultMsg2 = _T("");

	if(g_pView->m_bMakeReferenceMode) g_pView->SaveMakeRefData();
	
//#ifdef _TIME_ANALYSIS
//	SaveRemoteLog2File(CurrentSet->sResultPath, CurrentSet->bSaveResult2Txt, CurrentSet->bSaveResult2Html, CurrentSet->bSaveResult2Csv);
//#endif

	ctrlTestProgress.SetPos(100);


END_EXIT:
	AddStringToStatus("End Exit....");

	StepInforInit();
#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	Sleep(1000);
	AddStringToStatus("smcheck1 ....");
#endif
//AUTO
	if(g_nSysType == AUTO_SYS){
#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	AddStringToStatus("smcheck2 ....");
#endif	
	DischargCheck(1.5);
	}
//
	Sleep(1000);
//	pView->m_szCurrentStatus	= pView->m_szVersion;
	

	if(pView->m_pStopProcessingDlg != NULL)
	{
		pView->SendMessage(UM_CLOSE_STOP_PROCESSING_DLG,0,0); _Wait(100);
	}
#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	AddStringToStatus("smcheck3 ....");
#endif
	//090508
	if(!g_pView->m_bMakeReferenceMode)
	{
		if(g_nNoFailedStep == 0)
		{
			// Set Result OK
			pView->InsertTotalData2Grid(DETAILEDGRID, TRUE, sTime);
		}
		else
		{
			// Set Result NG
			pView->InsertTotalData2Grid(DETAILEDGRID, FALSE, sTime);
		}
	}

#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	AddStringToStatus("smcheck4 ....");
#endif
/*
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
*/

	//===============================
	// Create Elpased Time Data File 
	//===============================
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
	//===================
	// Show Result Diaog
	//===================
	if(pView->m_nNoRepeatExecution == 1)
	{

		if(!CurrentSet->bRunAbort && !CurrentSet->bSystemBreak){
			if(!pView->m_bMakeReferenceMode)
			{
				pView->m_szResultMsg1.Format("PCBA ID : %s",m_strWipId);
				pView->m_szResultMsg2.Format("Elased Time : %s(s) : %d",sTime, g_nNoFailedStep);
			}
			if(g_nNoFailedStep == 0)
			{
				pView->SendMessage(UM_RESULT_DISPLAY,1,0);
			}
			else
			{
				pView->SendMessage(UM_RESULT_DISPLAY,0,0);
			}
		}
		else{
			pView->m_szResultMsg1.Format("PCBA ID : %s",m_strWipId);
			pView->SendMessage(UM_RESULT_DISPLAY,2,0);
//#ifdef	DEBUG_MD5_CODE__
//			Sleep(2000);
//			Sleep(1000);
//#endif
		}
	}
	//======================
	// Repetative Operation
	//======================
	else
	{
		if(pView->m_nNoCurrentRepeatExecution == 1)
		{
			if(!pView->m_bMakeReferenceMode)
			{
				pView->m_szResultMsg1.Format("No Repeat : %d ( Pass:%d,Fail:%d )",
				pView->m_nNoRepeatExecution, pView->m_nNoPassInRepeatExecution, pView->m_nNoFailInRepeatExecution); 
				pView->m_szResultMsg2.Format("Elased Time : %s(s)",sTime);
			}
			
			if(!CurrentSet->bRunAbort && !CurrentSet->bSystemBreak){
				if(pView->m_nNoFailInRepeatExecution == 0)
				{
					pView->PostMessage(UM_RESULT_DISPLAY,TRUE,0);
				}
				else
				{
					pView->PostMessage(UM_RESULT_DISPLAY,FALSE,0);
				}
			}
			else{
				pView->PostMessage(UM_RESULT_DISPLAY,2,0);
			}
		}
	}
#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	AddStringToStatus("smcheck5 ....");
#endif
	pView->PlayWavSound(IDR_WAVE_FINISH);

//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
	if(CurrentSet->bRunAbort)
	{
		szMsg.Format(_T("%s : User Stop"), m_strWipId);
		pView->InsertPcbid2Grid(m_strWipId);
	}
	else{
		pView->InsertPcbid2Grid(m_strWipId);
	}
#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	AddStringToStatus("smcheck6 ....");
#endif
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
	TestCountSave(g_pView->m_szExePath);
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	AddStringToStatus("smcheck7 ....");
#endif
	//===========================
	// Create Detail Result File 
	//===========================
	if(CurrentSet->bSaveDetailResultData)
	{
		SaveDetailTestResult();
		SaveAdcAdjResult();
	}
#ifdef 	TEXT_MSG_EXE_DEBUG_CODE__
	AddStringToStatus("smcheck8 pView->SaveResult2File(....");
#endif
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
	//=========================
	// Create Result HTML File 
	//=========================
	pView->SaveResult2File(CurrentSet->sResultPath, CurrentSet->bSaveResult2Txt, CurrentSet->bSaveResult2Html, CurrentSet->bSaveResult2Csv);
//	pView->SaveResult2File(CurrentSet->sResultPath, TRUE, FALSE, TRUE);

	AddStringToStatus("Result Save....");
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
	CurrentSet->nRunType   = nRunMode;
	CurrentSet->bIsRunning = FALSE;
//	CurrentSet->bRunAbort  = FALSE;
	CurrentSet->bIsRunMsg	= FALSE;	
	pView->m_bThreadRunning = FALSE;

	
	// Repetative Operation
	if(pView->m_nNoRepeatExecution != 1)
	{
		pView->m_nNoCurrentRepeatExecution--;

		if(pView->m_nNoCurrentRepeatExecution >= 1)
		{
			if (g_nSysType == AUTO_SYS)
			{
				int iTryNo = 0;
				for (iTryNo = 0; iTryNo < 3; iTryNo++){
					gJigCtrl.Set_CylinderUP();
					for (int i = 0; i < 30; i++)
					{
						Sleep(300);
						if ((gJigCtrl.m_bJigUpStatus == 1) && (gJigCtrl.m_bJigDownStatus == 0))
						{
							//gJigCtrl.m_bJigUpStatus = 0;
							gJigCtrl.Set_CylinderDOWN();
							Sleep(1000);
							if (gJigCtrl.Get_JigReady())
							{
								iTryNo = 4;
								break;
							}
							for (int j = 0; j < 5; j++)
							{
								Sleep(300);
								if ((gJigCtrl.m_bJigUpStatus == 0) && (gJigCtrl.m_bJigDownStatus == 1))
								{
									iTryNo = 4;
									break;
								
								}
								if (gJigCtrl.Get_JigReady())
								{
									iTryNo = 4;
									break;
								}
							}
						}
						if(iTryNo == 4)
							break;
					}
				}
				if (iTryNo == 3)
				{
					AddStringToStatus("JIG UP/DOWN Control ERROR....");
				}
			}

			pView->PostMessage(UM_RUN_TEST,0,0);
			
			return 0;

		}
	}
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
	//====================
	// Upload Data to MES
	//====================
	if(!g_pView->m_bMakeReferenceMode && !CurrentSet->bRunAbort && !CurrentSet->bSystemBreak)
	{

		if(g_nNoFailedStep == 0)
		{
		//AUTO
			if(g_nSysType == AUTO_SYS){
				if(CurrentSet->nJigUpType == 2){
					gJigCtrl.Set_TestResult(0x01);
				}
				else{
					gJigCtrl.Set_TestResult(0x03);
				}
			}
		//
		//godtech 190530	if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes && CurrentSet->bUseScanner)
			if (CurrentSet->bGMES_Connection && CurrentSet->bUploadMes)// && CurrentSet->bUseScanner)
			{
				pView->SendMessage(UM_UPLOAD_MES_DATA,1,0);
			}
			if (g_nUseNoScanType == TRUE)//ScannerCtrl
			{
				ScannerCtrl.SendOK();
			}
		}
		else
		{
		//AUTO
			if(g_nSysType == AUTO_SYS){
				if(CurrentSet->nJigUpType == 1){
					gJigCtrl.Set_TestResult(0x02);
				}
				else{ 
					gJigCtrl.Set_TestResult(0x00);
				}
			}
		////godtech 190530 if(CurrentSet->bGMES_Connection && CurrentSet->bUploadMes && CurrentSet->bUseScanner)
			if(CurrentSet->bGMES_Connection && CurrentSet->bUploadMes )
			{
				if (CurrentSet->nDftNgCount > 0)
				{
					AddStringToStatus("Not Upload Mes NG Count....");
				}
				else
				{
					pView->SendMessage(UM_UPLOAD_MES_DATA, 0, 0);

					
				}
				
			}

			if (g_nUseNoScanType == TRUE)//ScannerCtrl
			{
				ScannerCtrl.SendNG();
			}
		}
	}
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(3000);
//#endif
	if (m_strWipId.Find("NO_SCAN") >= 0)
	{
		AddStringToStatus("NO S/N Do Not Upload Mes....");		
	}
	else
	{
		AddStringToStatus("Upload Mes....");	
		
	}

	pView->LogDataSave(CurrentSet->sResultPath);


//AUTO
	if(g_nSysType == AUTO_SYS){
		gJigCtrl.Set_TestStatus(0x00);
		Sleep(500);
	}
//

	CurrentSet->bCommDisplay = FALSE;
	pView->InitAvSwitchController();
	CurrentSet->bCommDisplay = TRUE;

	if(g_nNoFailedStep == 0)
	{
		SaveResultSummary(m_strWipId, TRUE, sTime);
	}
	else{
		SaveResultSummary(m_strWipId, FALSE, sTime);
	}
//FHD
//	if(g_nGrabberType == FHD_GRABBER)
//	{
//		pView->StartLVDSGrabThread_FHD();
//	}//
	//if(g_nNoFailedStep == 0)//if(!m_bThreadRunning)
//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		g_GrabSource = LVDS_GRABBER;
		if(pView->m_bGrabThreadRunning){
			pView->StopLVDSGrabThread();
			Sleep(100);
			pView->StartLVDSGrabThread();

		}
		else{
			pView->StartLVDSGrabThread();
		}
	}//
	else//FHD
	{
		pView->StartLVDSGrabThread_FHD();
	}//


	pView->m_szCurrentStatus = _T("READY"); 
	pView->PostMessage(UM_UPDATE_DISPLAY,STATUS, 0);

	ctrlWipIdEdit.SetFocus();

	pView->Invalidate();

//	CurrentSet->nRunType   = nRunMode;
//	CurrentSet->bIsRunning = FALSE;
	CurrentSet->bRunAbort  = FALSE;
//	CurrentSet->bIsRunMsg	= FALSE;	
//	pView->m_bThreadRunning = FALSE;

	pView->m_CurrentStep = 0;

	return 0;
}

//UINT CDATsysView::TestThread_EM(LPVOID pParam) 
//{
//	CDATsysView* pView =(CDATsysView* )pParam;
//
//	POSITION	PosStep_EM;
//
//	BOOL		bRev = FALSE;
//	BOOL		bRun;		
//	UINT		nChNo;		
//	UINT		nCheckType;
//	UINT		nMargin;
//	double		dSpec1;
//	double		dSpec2;
//	double		dLowerLimit1;
//	double		dUpperLimit1;
//	double		dLowerLimit2;
//	double		dUpperLimit2;
//	double		dMargin = 0.2;
//
//	double dVol = 0.0;
//	double dVol2 = 0.0;
//	UINT nTmp;
//	int nTime = 0;
//	int nWait = 0;
//	int nTestIndex = 0;
//
//	g_nNoFailedStep_EM  = 0;
//	::ResetEvent(pView->m_hVfThreadKillEvent);
//	
//	if(CurrentSet->VF_TestInfo.GetCount() == 0){
//		AfxMessageBox(_T("Failed to load VF_TestInfo."));
//		
//		return 0;
//	}
//
//	PosStep_EM = CurrentSet->VF_TestInfo.GetHeadPosition();       
//
//	while(PosStep_EM != NULL && !pView->m_bStopVFTestThread)
//	{
//	
//		if(CurrentSet->bRunAbort)
//		{
//			return 0;
//		}
//		
//		pCurStep_EM = CurrentSet->VF_TestInfo.GetNext(PosStep_EM);
//		bRun = pCurStep_EM->bRun;
//		if(bRun){
//			nChNo = pCurStep_EM->nChNo;
//			nCheckType = pCurStep_EM->nCheckType;
//			dSpec1 = pCurStep_EM->dLowerLimit;
//			dSpec2 = pCurStep_EM->dUpperLimit;
//			nTime = pCurStep_EM->nTime;
//			nMargin = pCurStep_EM->nMarginType;
//			if ((nCheckType == 1)&&(dSpec1 >= 0) && (dSpec1 <= 0.5))
//			{
//				continue;
//			}
//
////			if(nChNo == 34 || nChNo ==35 || nChNo ==36 || nChNo ==101 || nChNo ==102 || nChNo ==103){
////				dMargin = 0.1;
////			}
////			else{
////				dMargin = 0.2;
////			}
//			if (nCheckType == 1)
//			{
//				if ((dSpec1 >= 0)&&(dSpec1 <= 0.5))
//				{
//					continue;
//				}
//				if (nMargin == 1)
//					dMargin = 0.1;
//				else if (nMargin == 2)
//					dMargin = 0.2;
//				else if (nMargin == 3)
//					dMargin = 0.3;
//				else if (nMargin == 4)
//					dMargin = 0.4;
//				else
//					dMargin = 0.2;
//
//
//			}
//			else
//			{
//
//				if (nChNo == 34 || nChNo == 35 || nChNo == 36 || nChNo == 104 || nChNo == 105 || nChNo == 106) {
//					dMargin = 0.1;
//				}
//				else if (nChNo == 101 || nChNo == 102 || nChNo == 103) {
//					dMargin = 0.3;
//				}
//				else {
//					dMargin = 0.2;
//				}
//			}
//
//			dLowerLimit1 = dSpec1 - (abs(dSpec1) * dMargin);
//			dUpperLimit1 = dSpec1 + (abs(dSpec1) * dMargin);
//			dLowerLimit2 = dSpec2 - (abs(dSpec2) * dMargin);
//			dUpperLimit2 = dSpec2 + (abs(dSpec2) * dMargin);
//
//
//			for(nTmp = 0; nTmp < CurrentSet->nNoRetry; nTmp++)
//			{
//
//				if(nCheckType == 1){
//					if(nChNo < 43){
//						bRev = gVF1000Ctrl.GetVoltageData(nChNo,dVol);
//					}
//					else{
//						bRev = gVF1000Ctrl.GetFrequencyData(nChNo - 100,dVol);
//					}
//					if(bRev){
//						pCurStep_EM->dMaesure_Min = dVol;
//						pCurStep_EM->dMaesure_Max = dVol;
//						if(dVol >= dLowerLimit1 && dVol <= dUpperLimit1){
//							pCurStep_EM->bResult = TRUE;
//						}
//						else{
//							pCurStep_EM->bResult = FALSE;
//						}
//					}
//					else{
//						pCurStep_EM->dMaesure_Min = 0.0;
//						pCurStep_EM->dMaesure_Max = 0.0;
//						pCurStep_EM->bResult = FALSE;
//					}
//				}
//				else if(nCheckType == 2){
//					nWait = (int)((nTime + 1) - (nTime * 0.5)) * 1000;
//					if(nChNo < 43){
//						bRev = gVF1000Ctrl.GetVoltageData2(nChNo, nWait, dVol, dVol2);
//					}
//					else{
//						bRev = gVF1000Ctrl.GetFrequencyData2(nChNo - 100, nWait, dVol, dVol2);
//					}
//					if(bRev){
//						pCurStep_EM->dMaesure_Min = dVol;
//						pCurStep_EM->dMaesure_Max = dVol2;
////						if((dVol >= dLowerLimit1 && dVol <= dUpperLimit1) && (dVol2 >= dLowerLimit2 && dVol2 <= dUpperLimit2)){
//						if((dVol <= dSpec1) && (dVol2 >= dSpec2)){
//							pCurStep_EM->bResult = TRUE;
//						}
//						else{
//							pCurStep_EM->bResult = FALSE;
//						}
//					}
//					else{
//						pCurStep_EM->dMaesure_Min = 0.0;
//						pCurStep_EM->dMaesure_Max = 0.0;
//						pCurStep_EM->bResult = FALSE;
//					}
//				}
//				if(pCurStep_EM->bResult){break;} 
//			}
//			if(!pCurStep_EM->bResult){
//				g_nNoFailedStep_EM++;
//			//	nTestIndex++;
//			//	pView->InsertResultData2Grid_EM(nTestIndex, pCurStep_EM);
//			} 
//
//		//	nTestIndex++;
//		//	pView->InsertResultData2Grid_EM(nTestIndex, pCurStep_EM);
//		}
//
//	}
//	if(g_nNoFailedStep_EM > 0){
//		pView->InsertResultData2Grid_EM(g_nNoFailedStep_EM);
//	}
//
//	pView->m_bStopVFTestThread = FALSE;
//
//	pView->m_bVFTestThreadRunning = FALSE;
//
//	::SetEvent(pView->m_hVfThreadKillEvent);
//
//	return 0;
//}

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

	//g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpRun);
	g_pView->m_cStepResultNew.SetWindowText(_T("RUN"));
	g_pView->m_cStepResultNew.SetBkColor(RGB(200, 200, 64));


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
		s.Format("%5.1f¡­%5.1f", pCurStep->GetLowLimit(), pCurStep->GetHighLimit());
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
	POSITION	PosFunc;
	int nPosition = 0;
	BOOL nResult  = FALSE;

	gStrTmpLog.Format("Step Name: %Ts ", pCurStep->GetStepName());
	SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

	m_CurrentStep = pCurStep->m_nStep;
	//m_CtrlListMainProcess.Invalidate();
	CRect lRect;
	CRect lRectList;

	/*	m_CtrlListMainProcess.GetSubItemRect(m_CurrentStep - 1, 0, 3, lRect);
		m_CtrlListMainProcess.GetWindowRect(&lRectList);
		lRect.top = lRectList.top + lRect.top - 20;
		lRect.bottom = lRectList.bottom + lRect.bottom + 20;
		lRect.left = lRectList.left + lRect.left;*/

#ifdef _DFT_3IN1
	//if (m_CurrentStep > 16)
	//{
	//	SetScroll(m_CurrentStep - 17);
	//	//	m_CtrlListMainProcess.Invalidate();
	//}
	SetScroll(m_CurrentStep);
#else
	//if (m_CurrentStep > 30)
	//{
	//	SetScroll(m_CurrentStep - 31);	
	//}
	//
	SetScroll(m_CurrentStep);	
	
#endif
	if(pCurStep->m_bTest == FALSE) 
	{
		g_pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

		if(CurrentSet->bSaveReviewData) SaveTestResult(CurrentSet->sResultPath);

//CRect lRect;
//CRect lRectList;
//		InvalidateRect(lRect, TRUE);
///////////////////////////////////
		g_pView->m_ListUpdateFlag = 1;
		UpdateMainList(m_CurrentStep - 1);
		return TEST_PASS;
	}

	//m_CurrentStep = pCurStep->m_nStep;
	//m_CtrlListMainProcess.Invalidate();
	m_CtrlListMainProcess.SetItem(m_CurrentStep - 1, 2, LVIF_TEXT, _T("RUN"), NULL, NULL, NULL, NULL);
	m_CtrlListMainProcessEx.SetCellBackColor(m_CurrentStep - 1, RGB(255, 255, 0));
	g_pView->m_ListUpdateFlag = 1;
	UpdateMainList(m_CurrentStep - 1);
	//m_CtrlListMainProcess.GetSubItemRect(m_CurrentStep - 1, 0, 3, lRect);
	//m_CtrlListMainProcess.GetWindowRect(&lRectList);
	//lRect.top = lRectList.top + lRect.top - 20;
	//lRect.bottom = lRectList.bottom + lRect.bottom + 20;
	//lRect.left = lRectList.left + lRect.left;
	//InvalidateRect(lRect, TRUE);
	   
	if(CurrentSet->nAutoAVSwitchBoxControl == 1)
	{ 
		nResult = StepInit_AvSwitchBox();
		if(!nResult){
			Sleep(500);
			if(!StepInit_AvSwitchBox())
			{
				pCurStep->SetElapsedTime((double)(clock() - StepStart) / CLOCKS_PER_SEC);
				g_pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);
				m_szResultMsg2.Format("Error -> StepInit_AvSwitchBox");
#ifndef _NO_CONNECT_DEBUG__MODE
				CurrentSet->bSystemBreak = TRUE;
				return TEST_ABORT;
#endif
			}
		}
	}

	if(CurrentSet->bAutoSourceControl)
	{ 
		nResult = StepInit_SourceAutoControl();
		if(!nResult){
			Sleep(500);
			nResult = StepInit_SourceAutoControl();
			if(!nResult)
			{
				pCurStep->SetElapsedTime((double)(clock() - StepStart) / CLOCKS_PER_SEC);
				g_pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);
				m_szResultMsg2.Format("Error -> StepInit_SourceAutoControl");
#ifndef _ABORT_DEBUG__MODE				
				CurrentSet->bSystemBreak = TRUE;
				return TEST_ABORT;
#endif
			}
		}
	}

	if(CurrentSet->bAutoPatternControl)
	{ 
		nResult = StepInit_PatternAutoControl();
		if(!nResult){
			Sleep(500);
			if(!StepInit_PatternAutoControl())
			{
				pCurStep->SetElapsedTime((double)(clock() - StepStart) / CLOCKS_PER_SEC);
				g_pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);
				m_szResultMsg2.Format("Error -> StepInit_PatternAutoControl");
#ifndef _ABORT_DEBUG__MODE					
				CurrentSet->bSystemBreak = TRUE;
				return TEST_ABORT;
#endif
			}
		}
	}

	if(pCurStep->m_bRunAudioTest){
		g_pView->AudioMeasureStop();
		//_Wait(150);
		g_pView->AudioMeasureStart();
	}
	//UHD
	if (g_nGrabberType == UHD_GRABBER)
	{
		//if (g_AutoVideoRetry != 1)
		{
			if (CurrentSet->nGrabSource != g_GrabSource) {
				g_pView->StopLVDSGrabThread();
				Sleep(200);
				g_pView->StartLVDSGrabThread();
			}
		}
		g_AutoVideoRetry = 0;
	}
	//
	//g_pView->m_ctrlSummaryGrid.SetRow(pCurStep->m_nStep);

	PosFunc = pCurStep->m_InstructionList.GetHeadPosition();
	
	StepInfoDisplay();

	if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		pCurStep->SetResult(TRUE); 
	}
	
	int localRetryResult = TEST_FAIL;
	
	while(PosFunc != NULL)
	{
		if (localRetryResult != TEST_PASS)
		{
			pCurFunc = pCurStep->m_InstructionList.GetNext(PosFunc);
			localRetryResult = TEST_FAIL;
		}
		nResult =(*pCurFunc->m_pFunc)();
		localRetryResult = TEST_FAIL;
		
		gStrTmpLog= "StepRun()";// gStrTmpLog.Format("Step Name: %Ts ", pCurFunc->m_pFunc);
		SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

		if(CurrentSet->bRunAbort) return  TEST_ABORT;
		if(CurrentSet->bSystemBreak) return  TEST_ABORT;
		// If the result is TEST_NG or TEST_ABORT
		if(nResult != TEST_PASS)
		{
			pCurStep->SetResult(FALSE); 
			g_nNoFailedStep++; 
			
			pCurStep->SetElapsedTime((double)(clock() - StepStart) / CLOCKS_PER_SEC);
			g_pView->InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);
			if(nResult == TEST_ABORT){
				pCurStep->SetResult(FALSE); 
				if(!CurrentSet->bRunAbort && !CurrentSet->bSystemBreak){
					CurrentSet->bSystemBreak = TRUE;
				}
			}
			else 
			{
				if (CurrentSet->bPJT_GrabDisable == 1)
				{
				}				
				else if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
				{					
#ifdef				_PLC_COM_SIM_DEBUG__MODE
				//	pCurStep->m_bRunVideoTest = 1;
					//m_nCurrentVideoNG = 1;
#endif
					if ((pCurStep->m_bVideoTestResult == FALSE) && pCurStep->m_bRunVideoTest)
					{		
						if (CurrentSet->bGrabBaseReset == 1)
						{
							//WhitenessTest()
							int lWhiteFlag = _White_Test();
							if (lWhiteFlag == 1)
							{
								g_pView->GrabBaseResetStartThread();
								if (Grab_Image_UHD("", 0))//g_pView->Grab_UHD())
								{
									localRetryResult = (*pCurFunc->m_pFunc)();
									if (localRetryResult == TEST_PASS)
									{
										continue;
									}
								}
							}
						}

						g_pView->m_BlackPictureFlag =  _Dark_Test();
						if (g_pView->m_BlackPictureFlag == 1)
						{
							AddStringToStatus("Black Screen Check!!");
						}
						else if (CurrentSet->bEpiPAckReset == 1)
						{
							for (int i = 0; i < 2; i++)
							{
								Sleep(100);
								g_pView->IF_Pack_Reset();
								Sleep(100);
								if (g_pView->Grab_UHD())
								{
									localRetryResult = (*pCurFunc->m_pFunc)();
									if (localRetryResult == TEST_PASS)
									{
										break;
									}
								}
							}
							if (localRetryResult == TEST_PASS)
							{
								continue;
							}
						}
						else if(g_pView->m_nCurrentResetExecution == 1)
						{
							if (CurrentSet->bCheckOledReset == 1)
							{
#if 0   //
								AddStringToStatus("Grabber Power OFF");
								gJigCtrl.Set_Grabber_Power(0);
								Sleep(1000);

								AddStringToStatus("Grabber Power ON");
								gJigCtrl.Set_Grabber_Power(1);
								Sleep(2000);

								//	ResetGrabStartThread();
								//	Sleep(1000);

								for (int i = 0; i < 3; i++)
								{
									if (!GrabberReset())
									{
										continue;
									}
									else
									{
										AddStringToStatus("Gender_Reset!!");
										Sleep(1000);
										localRetryResult = (*pCurFunc->m_pFunc)();
										if (localRetryResult == TEST_PASS)
										{
											break;
										}
									}
								}
								if (localRetryResult == TEST_PASS)
								{
									//gPLC_Ctrl.m_nUserRetry = 0;
									continue;
								}

								if (CurrentSet->bCheckRotateReset == 1)
								{
									Sleep(1000);
									OnImgRotate();

									Sleep(1000);
									OnImgRotate();
								}

								Sleep(1000);
								localRetryResult = (*pCurFunc->m_pFunc)();
								if (localRetryResult == TEST_PASS)
								{
									//	gPLC_Ctrl.m_nUserRetry = 0;
									continue;
								}
#else

								for (int i = 0; i < 2; i++)
								{
									Sleep(500);
									ResetGrabStartThread();
									Sleep(500);
									if (g_pView->Grab_UHD())
									{
										localRetryResult = (*pCurFunc->m_pFunc)();
										if (localRetryResult == TEST_PASS)
										{
											break;
										}
										else
										{
											if ((pCurStep->m_bVideoTestResult == FALSE) && pCurStep->m_bRunVideoTest)
											{
												AddStringToStatus("Video Test NG");
											}
											if ((pCurStep->m_bAudioTestResult == FALSE) && pCurStep->m_bRunAudioTest)
											{
												AddStringToStatus("Audio Test NG");
											}

										}
									}
								}
								if (localRetryResult == TEST_PASS)
								{								
									continue;
								}
#if 0
								AddStringToStatus("Grabber Power OFF");
								gJigCtrl.Set_Grabber_Power(0);
								Sleep(1000);

								AddStringToStatus("Grabber Power ON");
								gJigCtrl.Set_Grabber_Power(1);
								Sleep(2000);
								g_pView->ResetGrabStartThread();
#else
								for (int i = 0; i < 3; i++)
								{
									AddStringToStatus("Grabber Power OFF");
									gJigCtrl.Set_Grabber_Power(0);
									Sleep(500);

									AddStringToStatus("Grabber Power ON");
									gJigCtrl.Set_Grabber_Power(1);
									Sleep(500);
									g_pView->ResetGrabStartThread();
									Sleep(500);
									if (g_pView->Grab_UHD())
									{
										localRetryResult = (*pCurFunc->m_pFunc)();
										AddStringToStatus("Video Check !!");
										if (localRetryResult == TEST_PASS)
										{
											break;
										}
										else
										{
											if ((pCurStep->m_bVideoTestResult == FALSE) && pCurStep->m_bRunVideoTest)
											{
												AddStringToStatus("Video Test NG");
											}
											if ((pCurStep->m_bAudioTestResult == FALSE) && pCurStep->m_bRunAudioTest)
											{
												AddStringToStatus("Audio Test NG");
											}
										}
									}		
								}	
								if (localRetryResult == TEST_PASS)
								{
									continue;
								}
#endif
								
#if 0
								for (int i = 0; i < 3; i++)
								{
									Sleep(1000);
									if (g_pView->Grab_UHD())
									{
										localRetryResult = (*pCurFunc->m_pFunc)();
										AddStringToStatus("Video Check !!");
										if (localRetryResult == TEST_PASS)
										{
											break;
										}
										else
										{
											if ((pCurStep->m_bVideoTestResult == FALSE) && pCurStep->m_bRunVideoTest)
											{
												AddStringToStatus("Video Test NG");
											}
											if ((pCurStep->m_bAudioTestResult == FALSE) && pCurStep->m_bRunAudioTest)
											{
												AddStringToStatus("Audio Test NG");
											}
										}
									}
								}
								
								if (localRetryResult == TEST_PASS)
								{								
									continue;
								}
#endif
								if (CurrentSet->bCheckRotateReset == 1)
								{
									Sleep(1000);
									g_pView->OnImgRotate();

									Sleep(1000);
									g_pView->OnImgRotate();


									Sleep(1000);
									localRetryResult = (*pCurFunc->m_pFunc)();
									if (localRetryResult == TEST_PASS)
									{
										//	gPLC_Ctrl.m_nUserRetry = 0;
										continue;
									}
								}
#endif								
							}
							else
							{

								for (int i = 0; i < 5; i++)
								{
									gJigCtrl.Set_Gender_Reset(0);
									AddStringToStatus("Gender_Reset!!");
									Sleep(1000);
									localRetryResult = (*pCurFunc->m_pFunc)();
									if (localRetryResult == TEST_PASS)
									{
										//gPLC_Ctrl.m_nUserRetry = 0;
										break;
									}
								}
								if (localRetryResult == TEST_PASS)
								{
									//gPLC_Ctrl.m_nUserRetry = 0;
									continue;
								}
								else
								{
									if (CurrentSet->nAuto_Grab_Reset == 1)
									{
										/*				if (CurrentSet->bCheckGrabACPower == 1)
														{
															AddStringToStatus("Grabber Power OFF");
															gJigCtrl.Set_Grabber_Power(0);
															Sleep(1000);

															AddStringToStatus("Grabber Power ON");
															gJigCtrl.Set_Grabber_Power(1);
															Sleep(2000);
														}

														if (CurrentSet->bCheckIFPackReset == 1)
														{
															gJigCtrl.Set_IF_Pack_Reset(0);
															AddStringToStatus("IF Pack Reset");
															Sleep(1000);
														}
														if (CurrentSet->bCheckGenderReset == 1)
														{

															AddStringToStatus("Gender Reset");
															gJigCtrl.Set_Gender_Reset(0);
														}*/

										Sleep(1000);
										g_pView->ResetGrabStartThread();
										/*		if (!GrabberReset())
												{

													if (CurrentSet->bCheckGrabACPower == 1)
													{
														AddStringToStatus("Grabber Power OFF");
														gJigCtrl.Set_Grabber_Power(0);
														Sleep(1000);

														AddStringToStatus("Grabber Power ON");
														gJigCtrl.Set_Grabber_Power(1);
														Sleep(2000);
													}
													if (!GrabberReset())
													{
														AddStringToStatus("Grabber Not Connected");
													}
												}*/

										if (CurrentSet->bCheckRotateReset == 1)
										{
											Sleep(1000);
											g_pView->OnImgRotate();

											Sleep(1000);
											g_pView->OnImgRotate();
										}

										Sleep(1000);
										localRetryResult = (*pCurFunc->m_pFunc)();
										if (localRetryResult == TEST_PASS)
										{
											//	gPLC_Ctrl.m_nUserRetry = 0;
											continue;
										}
									}
								}
							}
						}


						if (localRetryResult == TEST_PASS)
						{
							//gPLC_Ctrl.m_nUserRetry = 0;
							continue;
						}	
						
					}
				}
				else
				{
					if ((pCurStep->m_bVideoTestResult == FALSE) &&( pCurStep->m_bRunVideoTest))// &&(CurrentSet->bEpiPAckReset == 1))
					{
						if (CurrentSet->bFHDUHD_OLED_Pack_Reset == 1)
						{
							gJigCtrl.Set_Grabber_Power(0);
							Sleep(500);
							gJigCtrl.Set_Grabber_Power(1);
							Sleep(500);
							g_pView->GrabBaseResetStartThread();
							localRetryResult = (*pCurFunc->m_pFunc)();
							if (localRetryResult == TEST_PASS)
							{
								continue;
							}
						}
						else if (CurrentSet->bEpiPAckReset == 1)
						{	
								
							//gJigCtrl.Set_Grabber_Power(0);
							//Sleep(500); 
							//gJigCtrl.Set_Grabber_Power(1);
							//Sleep(500);
							gJigCtrl.Set_IF_Pack_Reset(1);
							Sleep(500);
							gJigCtrl.Set_Gender_Reset(1);
							Sleep(500);
								
							g_pView->GrabBaseResetStartThread();
							localRetryResult = (*pCurFunc->m_pFunc)();
							if (localRetryResult == TEST_PASS)
							{
								continue;
							}
								
							
						}
						else if (CurrentSet->bGrabBaseReset == 1)
						{
							int lWhiteFlag = _White_Test();
							if (lWhiteFlag == 1)
							{
								g_pView->GrabBaseResetStartThread();
								localRetryResult = (*pCurFunc->m_pFunc)();
								if (localRetryResult == TEST_PASS)
								{
									continue;
								}
							}
						}

						
												
					}
				}
			}
			pCurStep->m_nStepNgCount++;
			if (pCurStep->m_nStepNgCount > 5)
			{
			}
			gPLC_Ctrl.m_NG_StepNo = pCurStep->GetStepNo();
			if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE) 
				&& (gPLC_Ctrl.m_nUserRetry == 1)
				&& (gPLC_Ctrl.m_nCheckAbortRetry == 0))
			{			
				CurrentSet->bRunAbort = 1;
				gPLC_Ctrl.m_nCheckAbortRetry = 1;
				return  TEST_ABORT;
			}
			else
			{
				return nResult;
			}
		} 
		else
		{
			int lStepNo = pCurStep->GetStepNo();
			if (gPLC_Ctrl.m_NG_StepNo3Times == lStepNo)
			{
				gPLC_Ctrl.m_NG_StepNo3Times = -1;
			}
		}
		
	}
	nPosition =(int)((double)((++g_nProgressPos)*100)/(double)g_nStepSize);
	ctrlTestProgress.SetPos(nPosition);

	gPLC_Ctrl.m_nUserRetry = 0;

//	if(pCurStep->m_nStepType != PROCESS_FUNCTION){
//		if(!pCurStep->GetResult()) {g_nNoFailedStep++;}
//	}
	pCurStep->m_nStepNgCount = 0;
	return nResult;
}

	/*
int CDATsysView::StepRun_EM()
{
	POSITION	PosFunc_EM;
	int nPosition = 0;
	BOOL nResult  = FALSE;
		
	if(pCurStep_EM->m_bTest == FALSE) 
	{
		g_pView->InsertResultData2Grid_EM(pCurStep_EM->m_nStep);

		return TEST_PASS;
	}

//	g_pView->m_ctrlSummaryGrid.SetRow(pCurStep_EM->m_nStep);

	PosStep_EM = pCurStep_EM->m_InstructionList.GetHeadPosition();
	
//	StepInfoDisplay();
	
	while(PosStep_EM != NULL)
	{
		pCurFunc_EM = pCurStep_EM->m_InstructionList.GetNext(PosFunc_EM);
		
		nResult =(*pCurFunc_EM->m_pFunc)();

		// If the result is TEST_NG or TEST_ABORT
		if(nResult != TEST_PASS)
		{
			pCurStep_EM->SetResult(FALSE); 
			g_nNoFailedStep_EM++; 
			pCurStep_EM->SetElapsedTime((double)(clock() - StepStart_EM) / CLOCKS_PER_SEC);
			g_pView->InsertResultData2Grid_EM(pCurStep_EM->m_nStep);
			if(CurrentSet->bRunAbort) nResult = TEST_ABORT;

			return nResult;
		}
	}

	if(pCurStep_EM->m_nStepType == PROCESS_FUNCTION)
	{
		pCurStep_EM->SetResult(TRUE); 
	}

	return nResult;
}
*/
void CDATsysView::ResetStep()
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

		int i;
		for(i=0; i<3; i++)
		{
			for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++) pStep->m_aNoDifferentPoint[i][j] = 0;
		}
		pStep->m_nSecondDifferentPointFreePercent = -1;

		for( i=0; i<3; i++)
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
		for(i=0; i<5;i++)
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
	//AUTO
	ResetStep_EM();
//
}

void CDATsysView::ResetStep_EM()
{
    CVFTestInfo *pStep;
	POSITION pos;

	//g_nStepSize = 0;
	if(CurrentSet->VF_TestInfo.GetCount() == 0){return;}

	pos = CurrentSet->VF_TestInfo.GetHeadPosition();
	while(pos != NULL)
	{
		pStep = CurrentSet->VF_TestInfo.GetNext(pos);
		pStep->bResult = FALSE;
		pStep->dMaesure_Min = 0.0;
		pStep->dMaesure_Max = 0.0;
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
//AUTO
    return TRUE;        
  //  return FindResult_EM();
//        
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

BOOL FindResult_EM()
{
	if(CurrentSet->VF_TestInfo.GetCount() == 0){return TRUE;}

	POSITION pos = CurrentSet->VF_TestInfo.GetHeadPosition();
    while(pos != NULL)
	{
		CVFTestInfo* temp = CurrentSet->VF_TestInfo.GetNext(pos);
		if(!temp->bResult) return FALSE;
    }
    return TRUE;        
}

void StepInforInit()
{

	ctrlTestStep.SetWindowText("");
	ctrlStepTarget_V.SetWindowText(_T(""));
	ctrlStepMeasur_V.SetWindowText(_T(""));
	ctrlStepTarget_A.SetWindowText(_T(""));
	ctrlStepMeasur_A.SetWindowText(_T(""));
	//g_pView->m_cStepResult.SetBitmap(g_pView->m_bmpNormal);
	g_pView->m_cStepResultNew.SetWindowText(_T("READY"));
	g_pView->m_cStepResultNew.SetBkColor(RGB(128, 200, 64));
}

void ResultPrint()
{
//Check  - Print ÂÊÀ» ¾î¶»°Ô ÇÒÁö °áÁ¤ÇØ¾ßÇÔ
//	int lstNo;
//	CString s;
//	CString tps;
//	CString buf;
//	int prtPos;
//
//	lstNo = ctrlResult.GetItemCount();
//	CPrintDialog *pDlg = new CPrintDialog(FALSE);
//	//ÇÁ¸°ÅÍ·Î Ãâ·Â½Ã¿¡ Ãâ·ÂÇÏ´Â ÀÌ¸§ ¼³Á¤
//	DOCINFO di={ sizeof(DOCINFO),"DATsys",NULL};
//
//	//´ëÈ­»ç»óÀÚ ½ÇÇà
////	if(pDlg->DoModal()==IDCANCEL)
////	{
////		delete pDlg;
////		return;
////	}
////	pDlg->
//	pDlg->GetDefaults();
//	//ÇÁ¸°ÅÍ DC¸¦ ¹Þ´Â´Ù.
//	HDC hdc= pDlg->GetPrinterDC();
//	if(hdc == NULL)
//	{
//		OkMessage("ÇÁ¸°ÅÍ°¡ ¼³Ä¡µÇ¾î ÀÖÁö ¾Ê°Å³ª, ÇÁ¸°ÅÍ Á¤º¸¸¦ Ã£À» ¼ö ¾ø½À´Ï´Ù.");
//		return;
//	}
//	//ÇÁ¸°ÅÍ DC¿¡ Á¤º¸¸¦ Ãâ·Â ½ÃÀÛ
//	::StartDoc(hdc,&di);
//	//ÆäÀÌÁö ½ÃÀÛÀ» ¾Ë¸°´Ù.
//	::StartPage(hdc);
//	//µ¥ÀÌÅÍ Ãâ·Â
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
//	//ÆäÀÌÁö Á¾·á¸¦ ¾Ë¸°´Ù.
//	::EndPage(hdc);
//	//Á¤º¸Ãâ·ÂÀ» Á¾·á ÇÑ´Ù.
//	::EndDoc(hdc);
//	
//	delete pDlg;
}

void CDATsysView::OnTimer(UINT nIDEvent) 
{
	double nLevel_L = 0;
	double nLevel_R = 0;
	double freq, amp, dbamp;
	double nCorrectionValue = 0.97;
	double dElapsedTime = 0;

	static BOOL	bToggleTextColorState;

	//+add 090213(Modification No1)
	CString szString;

	//AddStringToStatusLocal();
	//SystemMonitorLog_Write();

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
				GmesLog_Delete((LONG)CurrentSet->nDataSavePeriod);
				CloseSystemMsg();
			}
			if((g_nUseNoScanType == FALSE)&&(g_nSysType == MANUAL_SYS))
			{
				if (g_nRunningProcessNo == 1) {
					installhook1(this->GetSafeHwnd());
				}
				else if (g_nRunningProcessNo == 2) {
					installhook2(this->GetSafeHwnd());
				}
				else {
					installhook3(this->GetSafeHwnd());
				}
			}
			break;


		case 2 :
			KillTimer(nIDEvent);
		//	AudioMeasureStart();
			break;

		case 3 :
			KillTimer(nIDEvent);
			//FHD
			if(g_nGrabberType == FHD_GRABBER)
			{
				GetGrabberVersion_FHD();
			}//
			break;
		case TIMER_AUTO_CONTROL_ID:
			KillTimer(nIDEvent);

			if (g_nSysRobotType == AUTO_SYS)
			AutoControlProcess();

			SetTimer(TIMER_AUTO_CONTROL_ID, 50,NULL);

			break;

		case TIMER_SERVER_CONTROL_ID:
			KillTimer(nIDEvent);
			if (g_nSysRobotType == AUTO_SYS)
			{
				if (CurrentSet->bUseServerCtrlConnect == 1)
				{
					ServerControlReport();
				}
			}

			SetTimer(TIMER_SERVER_CONTROL_ID, 100, NULL);

			break;

		case TIMER_SERVER_TIMEOUT_ID:
			//KillTimer(nIDEvent);

			//if (g_pView->m_pMonitorServerSocketClient.m_SerConnected != 1)
			//{
			//	g_pView->m_pMonitorServerSocketClient.Close();
			//}
			//ServerControlReport();

			//SetTimer(TIMER_SERVER_CONTROL_ID, 100, NULL);
			//TIMER_SERVER_TIMEOUT_ID, 2000, NULL);//g_pView->m_pMonitorServerSocketClient.m_SerConnected = 1;
			break;

		case TIMER_UPDATE_STATUS_INFO :
			//if(CurrentSet->bIsRunning)
			if (CurrentSet->bTestTimeCount)
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

				InvalidateRect(&m_ElapsedTimeClientRect);
				
			}

			UpdateMainList(m_CurrentStep-1);

			break;

		case TIMER_MEASURE_AUDIO_OUTPUT :

			if(g_SoundCard.wIn_Flag)
			{

				EnterCriticalSection(&g_cs);
				//g_CriticalSection.Lock();
				// LEFT
				g_SoundCard.GetWavePeak_L(&freq, &amp, &dbamp);
				g_SoundCard.GetPeackDetectorPeak_L(&nLevel_L);

				g_SoundCard.SetLedValue_inL((long)freq);
				g_SoundCard.SetLedValue_inL_Level((long)(nLevel_L/(2*1.4142)*0.1*nCorrectionValue));

				// RIGHT
				g_SoundCard.GetWavePeak_R(&freq, &amp, &dbamp);
				g_SoundCard.GetPeackDetectorPeak_R(&nLevel_R);
				
				g_SoundCard.SetLedValue_inR((long)freq);
				g_SoundCard.SetLedValue_inR_Level((long)(nLevel_R/(2*1.4142)*0.1*nCorrectionValue));
				LeaveCriticalSection(&g_cs);
				//g_CriticalSection.Unlock();
			}
			break;
		
		case TIMER_CONTINUOUS_LVDS_GRAB :
		{
		//FHD
			if(g_nGrabberType == FHD_GRABBER)
			{
				ContinuousLVDSGrab_FHD();
				_Wait(100);
			}//
			break;
		}

		case TIMER_MES_DATA_UPLOAD:
		{
			KillTimer(TIMER_MES_DATA_UPLOAD);
			if (CurrentSet->bUploadMes)
			{
				if (g_pView->m_pSocketClient.m_SerConnected == 0)
				{
					if (gGmesCtrl.m_bGmesCom) {
						gGmesCtrl.SoketCom_Close();
						CurrentSet->bGMES_Connection = FALSE;
					}

					Sleep(500);
					if (gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)) {
						CString sTmp = gGmesCtrl.MakeElem_S1F1();
						CurrentSet->bGMES_Connection = TRUE;
						if (gGmesCtrl.SendCommString(sTmp))
						{
							gGmesCtrl.DisplyComStatus("S1F1", FALSE);
						}
						g_pView->UpdateInfo(LED_STATE, "");
					}
					else {

						AddStringToStatus("GMES Connection Fail!");
					}	//+add kwmoon 080522
				}
			}
			SetTimer(TIMER_MES_DATA_UPLOAD, 1000, NULL);
			break;
		}
		case TIMER_CHECK_LIST_UPDATE:
		{
			
			KillTimer(nIDEvent);
			UpdateSeqCheck();
			SetTimer(TIMER_CHECK_LIST_UPDATEDELAY, 300, NULL);

		}		
		case TIMER_CHECK_LIST_UPDATEDELAY:
		{

			KillTimer(nIDEvent);
			UpdateSeqCheck();

		}
		break;	
		case TIMER_CHECK_DEBUG_TEST:
		{

			KillTimer(nIDEvent);
#ifdef			DEBUG_MD5_CODE__
			if (m_SelectChassisModelDlg.m_bActivate == 1)
			{
				CurrentSet->sModelSuffixName = "A123.459";
				::SendMessage(m_SelectChassisModelDlg, UM_UPDATE_SUFFIX_SCAN, 0, 0);
			}
#endif

		}
		break;

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

void CDATsysView::UpdateMainList(int lnStep)
{
	CString sTmp;
	if (m_ListUpdateFlag == 1)
	{
		m_ListUpdateFlag = 0;
		if( (lnStep < 0)||(m_CtrlListMainProcess.GetItemCount() <= lnStep))
			return;

		for (int i = 0; i < 3; i++)
		{
			if (lnStep > 0)
			{
				sTmp = m_CtrlListMainProcess.GetItemText(lnStep - 1, i);
				m_CtrlListMainProcess.SetItemText(lnStep - 1, i, sTmp);
			}
			sTmp = m_CtrlListMainProcess.GetItemText(lnStep, i);
			m_CtrlListMainProcess.SetItemText(lnStep, i, sTmp);
			sTmp = m_CtrlListMainProcess.GetItemText(lnStep+1, i);
			m_CtrlListMainProcess.SetItemText(lnStep+1, i, sTmp);
		}


	}

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



void CDATsysView::ServerSendModelList()
{

		//CWinApp* pApp = AfxGetApp();

		int		nNoModel = 0;
		CString sTmp = _T("");
		CString sMsg = _T("");
		CString sKeyClass = _T("");
		BOOL bFlag;

		if (OpenModelListFile(CurrentSet->sModelListPath, nNoModel, g_Divisions) == FALSE) return;
/*		POSITION	Pos = NULL;
		CModelData* pModelData = NULL;
	*/	//szChassisName = m_gridChassis.GetTextMatrix(nChassisRow, 0);
		//szModelName = m_gridModel.GetTextMatrix(nModelRow, 0);
		//m_sSelModel = szModelName;

		//Pos = CurrentSet->ModelList.GetHeadPosition();

		//while (Pos)
		//{
		//	pModelData = CurrentSet->ModelList.GetNext(Pos);

		//	if (pModelData->m_szModelName != _T(""))
		//	{
		//		if ((pModelData->m_szChassisName.Compare(szChassisName) == NULL) &&
		//			(pModelData->m_szModelName.Compare(szModelName) == NULL))
		//		{
		//			m_szSeqFilePath = pModelData->m_szSeqFilePath;
		//			m_szModelInfoFilePath = pModelData->m_szModelInfoFilePath;
		//			m_szRefImgFolder = pModelData->m_szRefImgFolder;

		//			break;
		//		}
		//	}
		//}

	
		gServer_Ctrl.SendModelList();



	return ;
}


BOOL CDATsysView::ServerChangeModel()
{

	/*
	str = "#/m_szSeqFilePath//";
	str += lDlg.m_szSeqFilePath;
	str = "#/m_szModelInfoFilePath//";
	str += lDlg.m_szModelInfoFilePath;
	str = "#/m_szRefImgFolder//";
	str += lDlg.m_szRefImgFolder;

	str = "#/m_sSelChassis//";
	str += lDlg.m_sSelChassis;
	str = "#/m_sSelModel//";
	str += lDlg.m_sSelModel;
	str = "#/END";
	*/
	CString str1;
	CString str2;
	int lpos;

	str1 = gServer_Ctrl.m_ServerControlData;

	lpos = str1.Find("#/m_szSeqFilePath//");
	str2 = str1.Mid(lpos + strlen("#/m_szSeqFilePath//"));
	lpos = str2.Find("#/");
//	str1 = str2;
	str2 = str2.Left(lpos);
	gServer_Ctrl.m_szSeqFilePath = str2;

	lpos = str1.Find("#/m_szModelInfoFilePath//");
	str2 = str1.Mid(lpos + strlen("#/m_szModelInfoFilePath//"));
	lpos = str2.Find("#/");
	str2 = str2.Left(lpos);
	gServer_Ctrl.m_szModelInfoFilePath = str2;

	lpos = str1.Find("#/m_szRefImgFolder//");
	str2 = str1.Mid(lpos + strlen("#/m_szRefImgFolder//"));
	lpos = str2.Find("#/");
	str2 = str2.Left(lpos);
	gServer_Ctrl.m_szRefImgFolder = str2;


	lpos = str1.Find("#/m_sSelChassis//");
	str2 = str1.Mid(lpos + strlen("#/m_sSelChassis//"));
	lpos = str2.Find("#/");
	str2 = str2.Left(lpos);
	gServer_Ctrl.m_sSelChassis = str2;


	lpos = str1.Find("#/m_sSelModel//");
	str2 = str1.Mid(lpos + strlen("#/m_sSelModel//"));
	lpos = str2.Find("#/");
	str2 = str2.Left(lpos);
	gServer_Ctrl.m_sSelModel = str2;


	//Check Jig Up
	CWinApp* pApp = AfxGetApp();

	int		nNoModel = 0;
	CString sTmp = _T("");
	CString sMsg = _T("");
	CString sKeyClass = _T("");
	BOOL bFlag;

//	if (OpenModelListFile(CurrentSet->sModelListPath, nNoModel, g_Divisions) == FALSE) return;

	//+add PSH 090507
	bFlag = g_SoundCard.wIn_Flag;
	//090615
	AudioMeasureStop();
	//	OnAudioMeasStop();
		//-
		//UHD
	if (g_nGrabberType == UHD_GRABBER)
	{
		StopLVDSGrabThread();
	}//
	else//FHD
	{
		StopLVDSGrabThread_FHD();
	}

	//CSelectChassisModelDlg dlg;
	//dlg.m_nNoModel = nNoModel;

	//if (dlg.DoModal() == IDOK)
	{
		//+move kwmoon 080818
		CurrentSet->sSeqPath = CurrentSet->sServerFolder + gServer_Ctrl.m_szSeqFilePath;
		CurrentSet->sModelIni = CurrentSet->sModelInfoFolder + gServer_Ctrl.m_szModelInfoFilePath;

#ifdef SM_MODIFY_CODE__

		sTmp.Format(_T("%s_VF.cfg"), gServer_Ctrl.m_szModelInfoFilePath.Left(gServer_Ctrl.m_szModelInfoFilePath.ReverseFind('.')));
		CurrentSet->sVFTestIni = CurrentSet->sModelVFConfigFolder + sTmp;

#else	

		//sModeVFConfigFolder
//		sTmp.Format(_T("%s\\VF_TestConfig.ini"),CurrentSet->sModelIni.Left(CurrentSet->sModelIni.ReverseFind('\\')));
		sTmp.Format(_T("%s_VF.cfg"), CurrentSet->sModelIni.Left(CurrentSet->sModelIni.ReverseFind('.')));
		CurrentSet->sVFTestIni = sTmp;
#endif


		CurrentSet->sModelFolder = CurrentSet->sServerFolder + gServer_Ctrl.m_szModelInfoFilePath.Left(gServer_Ctrl.m_szModelInfoFilePath.ReverseFind('\\'));
		CurrentSet->sRefFolder = CurrentSet->sRefRootFolder + gServer_Ctrl.m_szRefImgFolder;
		CurrentSet->sMaskFolder = CurrentSet->sRefFolder + "\\Mask";
		//AUTO
		/*		sTmp = dlg.m_szSeqFilePath2;
				if(!sTmp.IsEmpty()){
					CurrentSet->sSeqPath_EM		= CurrentSet->sServerFolder + sTmp;
				} */
				//		
		CurrentSet->sChassisName = gServer_Ctrl.m_sSelChassis;
		CurrentSet->sModelName = gServer_Ctrl.m_sSelModel;

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
			OpenModelIniFile(CurrentSet->sModelIni, m_szExePath);
			//FHD
			if (g_nGrabberType == FHD_GRABBER)
			{
				SetGrabInfo(&g_GrabImage);
			}//
			HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);
			Sleep(100);
			HDMIGeneratorCtrl.SetEDID_PassCheck(CurrentSet->bHdmiEdidControl);
			Sleep(100);
			HDMIGeneratorCtrl.SetHDCP_OnOff(CurrentSet->bHdmiHdcpControl);
			Sleep(100);
			if (!CurrentSet->bHdmiOutControl) {
				HDMIGeneratorCtrl.SetOutPort(0);
			}

			if (CurrentSet->bUseDpg)
			{
				DPGeneratorCtrl.SetTime_Control(CurrentSet->nDP_TimeSel);
			}

			//CurrentSet->sEpiCfgIni = m_szExePath + "\\EPI_Config.ini";
			if (OpenEPI_ConfigFile(CurrentSet->sEpiCfgIni))
			{
				//CurrentSet->sDipStatus
			}
			else
			{
				CurrentSet->sDipStatus = "";
			}
			m_CEdit_EPI_CFG_SW.SetWindowText(CurrentSet->sDipStatus);
		}
		else
		{
			////	sMsg.Format("Failed to load file.\n[%s]",CurrentSet->sModelIni);
			////	AfxMessageBox(sMsg);
			//CModelInfoCreate dlg;

			//dlg.m_sModelInfo_FileName = CurrentSet->sModelIni;
			//dlg.DoModal();
		}

		if (FileExists(CurrentSet->sVFTestIni))
		{
			int nNoModel = 0;
			OpenVFTestFile(CurrentSet->sVFTestIni);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sVFTestIni);
			AfxMessageBox(sMsg);
			DeleteVFTestList();
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
		if (CurrentSet->sRemoconTitle.IsEmpty())
		{
			CurrentSet->sRemoconTitle = m_szExePath + "\\DFT_REMOTE.rmt";
		}

		if (FileExists(CurrentSet->sRemoconTitle))
		{
			OpenRemoteFile(CurrentSet->sRemoconTitle);
		}
		else
		{
			sMsg.Format("Failed to load file.\n[%s]", CurrentSet->sRemoconTitle);
			AfxMessageBox(sMsg);
		}

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
		//======================
		// Set HDCP-Key List File 
		//======================
		if (CurrentSet->sHdcpKeyListPath.IsEmpty())
		{
			CurrentSet->sHdcpKeyListPath = m_szExePath + "\\HDCP\\" + CurrentSet->sHDCP_Key_Class;
		}
		else {
			sTmp = CurrentSet->sHdcpKeyListPath;
			sKeyClass = sTmp.Right(sTmp.GetLength() - sTmp.ReverseFind('\\') - 1);
			if (CurrentSet->sHDCP_Key_Class != sKeyClass)
			{
				CurrentSet->sHdcpKeyListPath = m_szExePath + "\\HDCP\\" + CurrentSet->sHDCP_Key_Class;
			}
		}

		SetI2cClkDelay();

		//		UpdateInfo(MODEL, CurrentSet->sModelFolder);
		UpdateInfo(MODEL, CurrentSet->sModelName);
		UpdateInfo(CHASSIS, CurrentSet->sChassisName);
		UpdateInfo(REF, CurrentSet->sRefFolder);
		UpdateInfo(SEQ, CurrentSet->sSeqPath);

		ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);

		UpdateToolOptionInfo(FALSE);

		InsertStepData2Grid(CurrentSet->nDisplayType);

		if (CurrentSet->bUseTVCommPort)
		{
			if (TVCommCtrl.m_bPortOpen)
			{
				TVCommCtrl.PortClose();
				Sleep(1000);
			}

			if (TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate) == FALSE)
			{
				TVCommCtrl.PortClose();
				Sleep(1000);
				TVCommCtrl.Create(CurrentSet->sTVComPort, CurrentSet->wTVBaudRate);
				if (TVCommCtrl.m_bPortOpen == FALSE)
				{
					CString szErrMsg;
					szErrMsg.Format("Failed to open COM port (%s)", CurrentSet->sTVComPort);
					AfxMessageBox(szErrMsg);
				}
			}
		}
	}
	//UHD
	if (g_nGrabberType == UHD_GRABBER)
	{
		StartLVDSGrabThread();
	}//
	else
	{
		StartLVDSGrabThread_FHD();
	}
	//FHD
	//if(bFlag) SetTimer(2, 1000, NULL);
	//


	return 1 ;
}

BOOL CDATsysView::ServerChangeModelAck(int lOK)
{
	//Check Jig Up
	
	gServer_Ctrl.ServerChangeModelAck(lOK);
		
	
	return 0;
}

BOOL CDATsysView::ServerControlReport()
{
	//Check Jig Up
	static int sWaitCount = 0;
	static int sRetryCount = 0;

	static int sInterProcess = 0;
	//static int g_Pause_AutoControlProcess = 0;
	static int sAlarmToggle = 0;

	static UINT sWaitServerMonitorCount = 0;
	CString str1;


	sWaitServerMonitorCount++;//100ms
	///////////////////////
#ifdef _DEBUG
	//str1.Format(_T("%d:%d:%d"), sWaitServerMonitorCount, gServer_Ctrl.m_bCom_SndCount,gServer_Ctrl.m_bCom_RcvCount);
	//ctrlPLC_COM_Status.SetWindowText(str1);
#endif
	////////////////////////
	if (gServer_Ctrl.m_bCom_SndCount >= 500)
	{
		gServer_Ctrl.m_bCom_SndCount = 0;
	}

	if (gServer_Ctrl.m_bCom_RcvCount >= 500)
	{
		gServer_Ctrl.m_bCom_RcvCount = 0;
	}	
	
	if (sWaitServerMonitorCount >= 500)
	{
		sWaitServerMonitorCount = 0;
		//CurrentSet->bMonitorServer_Connection
		//if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port)) {

		//	CurrentSet->bMonitorServer_Connection = TRUE;
		//	gServer_Ctrl.m_bCom_RetryCount = 0;
		//}
		//else
		//{
		//	CurrentSet->bMonitorServer_Connection = FALSE;
		//	gServer_Ctrl.m_bCom_RetryCount++;
		//}
	}

	if ((sWaitServerMonitorCount % 20) == 0)
	{
		gServer_Ctrl.SendStatus();
		return 0;
	}
	else if ((sWaitServerMonitorCount % 101) == 0)
	{
		if (gServer_Ctrl.m_bCom_NGCount > 10)
		{
			if (gServer_Ctrl.m_bServer_CtrlCom == 1)
			{
				gServer_Ctrl.SoketCom_Close();
			}

			if (gServer_Ctrl.m_bCom_RetryCount < 3)
			{
				if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port)) {

					CurrentSet->bMonitorServer_Connection = TRUE;
					gServer_Ctrl.m_bCom_RetryCount = 0;
				}
				else
				{
					CurrentSet->bMonitorServer_Connection = FALSE;
					gServer_Ctrl.m_bCom_RetryCount++;

				}
			}
			gServer_Ctrl.m_bCom_NGCount = 0;

		}
		else
		{
			if ((CurrentSet->bUseServerCtrlConnect == 1) && (g_pView->m_pMonitorServerSocketClient.m_SerConnected == 0))
			{
				if (gServer_Ctrl.m_bServer_CtrlCom == 1)
				{
					gServer_Ctrl.SoketCom_Close();
				}
				if (gServer_Ctrl.m_bCom_RetryCount < 3)
				{
					if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port)) {

						CurrentSet->bMonitorServer_Connection = TRUE;
						gServer_Ctrl.m_bCom_RetryCount = 0;
					}
					else
					{
						CurrentSet->bMonitorServer_Connection = FALSE;
						gServer_Ctrl.m_bCom_RetryCount++;
					}
				}
			}
		}
	}
	else
	{
		return 0;
	}
	
	return 0;
}




BOOL CDATsysView::AutoControlProcess()
{
	//Check Jig Up
	static int sWaitCount = 0;
	static int sRetryCount = 0;
	
	static int sInterProcess = 0;
	//static int g_Pause_AutoControlProcess = 0;
	static int sAlarmToggle = 0;

	static UINT sWaitServerMonitorCount = 0;


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

//#define		_SERVER_CTRL_IDLE		0
//#define		_SERVER_CTRL_START		1
//#define		_SERVER_CTRL_PAUSE		2
//#define		_SERVER_CTRL_STOP		3
	switch (gServer_Ctrl.m_ServerControlCommand)
	{
	case _SERVER_CTRL_IDLE:
		break;
	case _SERVER_CTRL_START:
		OnBnClickedButtonAutoRobotReset();
		AddStringToStatus("_SERVER_CTRL_START !!");
		break;
	case _SERVER_CTRL_PAUSE:
		if (g_AutoControlProcess != _AUTO_SERVER_CTRL_PAUSE)//
		{
			if (((g_AutoControlProcess > 0) && (g_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
				|| (g_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY))
			{
				g_Pause_AutoControlProcess = g_AutoControlProcess;
			}
			g_AutoControlProcess = _AUTO_SERVER_CTRL_PAUSE;
			ctrlPLC_COM_Status.SetWindowText(_T("SVR PAUSE"));
			ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));

			AddStringToStatus("_SERVER_CTRL_PAUSE !!");
		}
		break;
	case _SERVER_CTRL_STOP:
		OnBnClickedButtonAutoRobotStop();
		AddStringToStatus("_SERVER_CTRL_STOP !!");
		break;
	case _SERVER_CTRL_GETTOOL:
		
		CurrentSet->sModelSuffixName = gServer_Ctrl.m_ServerControlData;

		if (m_GetToolOptiondlg.m_bActivate) 
		{
			
			::SendMessage(m_GetToolOptiondlg, UM_UPDATE_TOOL_OPTION, 0, 0);
		}
		else
		{
			if (m_GetToolOptiondlg.DoModal() == IDOK)
			{
				ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);
				UpdateToolOptionInfo(TRUE);

			}
			else {
				CurrentSet->sModelSuffixName = "";
				ctrlSuffixEdit.SetWindowText("");
			}
		}

		break;	
	
	case _SERVER_CTRL_GETTOOL_CLOSE:
		//if (m_GetToolOptiondlg.m_bActivate)
		//{
		//	::SendMessage(m_GetToolOptiondlg, UM_CLOSE_TOOL_OPTION, 0, 0);
		//}
		break;
	
	case _SERVER_CTRL_GET_MODEL_LIST:
		//Open Model List
		//Send Model
		ServerSendModelList();
		AddStringToStatus("_SERVER_CTRL_GET_MODEL_LIST !!");
		break;

	case _SERVER_CTRL_CHANGE_MODEL:
		AddStringToStatus("_SERVER_CTRL_CHANGE_MODEL !!");
		if (ServerChangeModel())
		{
			ServerChangeModelAck(1);
		}
		else
		{
			ServerChangeModelAck(0);
		}
		break;
	case  _SERVER_CTRL_JIG_UP:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE)||(g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
			AddStringToStatus("_SERVER_CTRL_JIG_UP !!");
			gJigCtrl.Set_CylinderUP();
		}
		break;
		
	case  _SERVER_CTRL_JIG_DOWN:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
	//		if (gPLC_Ctrl.m_cCheckJigDownStatus == 0)
			{
				AddStringToStatus("_SERVER_CTRL_JIG_DOWN !!");

				gJigCtrl.Set_CylinderDOWN();
			}
		}
			
		break;
			
	case  _SERVER_CTRL_LOAD:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
			AddStringToStatus("_SERVER_CTRL_LOAD !!");
			gPLC_Ctrl.CommandLoad();
		}
				
		break;
				
	case  _SERVER_CTRL_TEST_PASS:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
			AddStringToStatus("_SERVER_CTRL_TEST_PASS !!");
			gPLC_Ctrl.CommandPassUnload();
		}
					
		break;
					
	case  _SERVER_CTRL_TEST_NG:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
			AddStringToStatus("_SERVER_CTRL_TEST_NG !!");
			gPLC_Ctrl.CommandNgUnload();
		}
					
		break;
					
	case  _SERVER_CTRL_TEST_CLEAR:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
			AddStringToStatus("_SERVER_CTRL_TEST_CLEAR !!");
			gPLC_Ctrl.CommandClear();
		}
					
		break;
						
	case  _SERVER_CTRL_TEST_READ:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
			AddStringToStatus("_SERVER_CTRL_TEST_READ !!");
			gPLC_Ctrl.CommandRead();
		}
						
		break;
								
	case  _SERVER_CTRL_AC_ON:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
		//	if (gPLC_Ctrl.m_cCheckJigDownStatus == 1)
			{
				AddStringToStatus("_SERVER_CTRL_AC_ON !!");
				gJigCtrl.Set_ACPower(1);
			}
		}
						
		break;
	case  _SERVER_CTRL_AC_OFF:
		if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
		{
			AddStringToStatus("_SERVER_CTRL_AC_OFF !!");
			gJigCtrl.Set_ACPower(0);
			SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
		}
						
		break;
	//case  _SERVER_CTRL_PLC_DATA_READ:
	//	if ((g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_MANUAL))
	//	{
	//		gPLC_Ctrl.CommandRead();
	//	}
	//					
	//	break;	

	default:
		break;
	}
	//if(gServer_Ctrl.m_ServerControlCommand == _SERVER_CTRL_TEST_READ)
	//{

	//	_SERVER_CTRL_PLC_DATA_READ
	gServer_Ctrl.m_ServerControlCommand = _SERVER_CTRL_IDLE;


	if (g_AutoControlProcess == _AUTO_SERVER_CTRL_PAUSE)
	{
		if (gPLC_Ctrl.m_cResetControl == 1)
		{		
			if (((g_Pause_AutoControlProcess > 0) && (g_Pause_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
				|| (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY) || (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY))		
			{
				g_AutoControlProcess = g_Pause_AutoControlProcess;
				if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_JIGUP)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_LOAD)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("LOAD"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_JIG_DOWN)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("JIG DOWN"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_TEST_RUN)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_JIGFINISH_UP)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_UNLOAD)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("UNLOAD"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("FFC:RTY"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("UPDOWN:RTY"));
				}
				else
				{
					ctrlPLC_COM_Status.SetWindowText(_T("RESTART"));
				}

			}
			else
			{
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
				ctrlPLC_COM_Status.SetWindowText(_T("RESTART"));
			}			
		}
		return 0;
	}

////////////////////////////////////////////////
//////////////////////////////////////////////////
	
	
	
	if ((CurrentSet->nRunStartType == AUTO_START) && (g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
	{
		gPLC_Ctrl.m_bPLC_WaitCount++;
		if (gPLC_Ctrl.m_bPLC_WaitCount > 300)
		{
			gPLC_Ctrl.m_bPLC_Online = 0;
		}
		else if ((gPLC_Ctrl.m_bPLC_WaitCount % 10) == 0)
		{
			gPLC_Ctrl.CommandRead();
			if (gPLC_Ctrl.m_cCheckSkipFFC == 0)
			{
				gPLC_Ctrl.CommandSkipFFC();
			}
			//if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_ALARM)
			//{
			//	if (sAlarmToggle)
			//	{
			//		sAlarmToggle = 0;
			//		ctrlPLC_COM_Status.SetBkColor(RGB(255, 0, 0));
			//	}
			//	else
			//	{
			//		sAlarmToggle = 1;
			//		ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 255));
			//	}
			//}

			//if (CurrentSet->bRobotSkipFFC == 1)
			//{
			//	if (gPLC_Ctrl.m_cCheckSkipFFC == 0)
			//	{
			//		gPLC_Ctrl.CommandSkipFFC();
			//	}
			//}
			//else
			//{
			//	if (gPLC_Ctrl.m_cCheckSkipFFC == 1)
			//	{
			//		gPLC_Ctrl.CommandConnectFFC();
			//	}
			//}
			
		}
		
		

		gPLC_Ctrl.m_bPLC_RegWaitCount++;
		if (gPLC_Ctrl.m_bPLC_RegWaitCount > 300)
		{
			gPLC_Ctrl.m_bPLC_Online = 0;
		}
		else if ((gPLC_Ctrl.m_bPLC_RegWaitCount % 10) == 0)
		{
			gPLC_Ctrl.CommandRegRead();
		}
#ifdef _PLC_COM_SIM_DEBUG__MODE
		gPLC_Ctrl.m_bPLC_Online = 1;
		gPLC_Ctrl.m_bPLCCom = 1;
		gPLC_Ctrl.m_cCheck_PLC_Start_Run = 1;
		gPLC_Ctrl.m_cCheck_PLC_Auto_Mode = 1;
		gPLC_Ctrl.m_cCheck_PLC_Manual_Mode = 0;
		gPLC_Ctrl.m_cCheck_PLC_STOP = 0;
		gPLC_Ctrl.m_cCheck_PLC_Pause = 0;
		gPLC_Ctrl.m_cCheck_PLC_Alarm = 0;
#endif

		if (gPLC_Ctrl.m_bPLCCom == 0)
		{
			if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_OFFLINE)
			{
				//g_Pause_AutoControlProcess = g_AutoControlProcess;
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_OFFLINE;
					ctrlPLC_COM_Status.SetWindowText(_T("OFF LINE"));
					ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
			}
		} //_AUTO_ROBOT_CTRL_NOACK
		else if (gPLC_Ctrl.m_bPLC_Online == 0)//m_bPLC_WaitCount
		{
			if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_NOACK)
			{
				//g_Pause_AutoControlProcess = g_AutoControlProcess;
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_NOACK;
				ctrlPLC_COM_Status.SetWindowText(_T("TIME OUT"));
				ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));

				if (gPLC_Ctrl.m_bPLCCom == 1)
				{
					gPLC_Ctrl.SoketCom_Close();
				}
				else
				{
					if (gPLC_Ctrl.SoketCom_Creat(CurrentSet->strPLC_IP, CurrentSet->nPLC_Port))
					{
					}
					else
					{
						gPLC_Ctrl.m_bPLCCom = 0;
					}
				}				
			}
		}//m_bPLC_Online //_AUTO_ROBOT_CTRL_NOACK
		//else if ((gPLC_Ctrl.m_bPLCCom)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 1)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Auto_Mode == 1)
		//	&& (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 1))
		////	&& (gPLC_Ctrl.m_cCheck_PLC_STOP == 0)
		////	&& (gPLC_Ctrl.m_cCheck_PLC_Pause == 0)
		////	&& (gPLC_Ctrl.m_cCheck_PLC_Alarm == 0))
		//{	
		//	if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_PAUSE)
		//	{
		//		if (((g_Pause_AutoControlProcess > 0) && (g_Pause_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
		//			|| (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY) || (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY))
		//		{
		//			g_Pause_AutoControlProcess = g_AutoControlProcess;
		//		}
		//		g_AutoControlProcess = _AUTO_ROBOT_CTRL_PAUSE;
		//		ctrlPLC_COM_Status.SetWindowText(_T("MAN"));
		//		ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
		//	}
		//	
		//	//if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_IDLE)
		//	//{
		//	//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
		//	//		ctrlPLC_COM_Status.SetWindowText(_T("MAN"));
		//	//	ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
		//	//}
		//}
		//else if ((gPLC_Ctrl.m_bPLCCom)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 1)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Auto_Mode == 1)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 1)
		//	//	&& (gPLC_Ctrl.m_cCheck_PLC_STOP == 0)
		//	//	&& (gPLC_Ctrl.m_cCheck_PLC_Pause == 0)
		//	&& (gPLC_Ctrl.m_cCheck_PLC_Alarm == 1))
		//{
		//	if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_PAUSE)
		//	{
		//		if (((g_Pause_AutoControlProcess > 0) && (g_Pause_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
		//			|| (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY) || (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY))
		//		{
		//			g_Pause_AutoControlProcess = g_AutoControlProcess;
		//		}
		//		g_AutoControlProcess = _AUTO_ROBOT_CTRL_PAUSE;
		//		//ctrlPLC_COM_Status.SetWindowText(_T("PLC PAUSE"));
		//		//ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
		//		ctrlPLC_COM_Status.SetWindowText(_T("ALARM"));
		//		ctrlPLC_COM_Status.SetBkColor(RGB(255, 0, 0));
		//	}
		//	//if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_ALARM)//_AUTO_ROBOT_CTRL_MANUAL
		//	//{
		//	//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_ALARM;
		//	//	ctrlPLC_COM_Status.SetWindowText(_T("ALARM"));
		//	//	ctrlPLC_COM_Status.SetBkColor(RGB(255, 0, 0));
		//	//}
		//}
		//else if ((gPLC_Ctrl.m_bPLCCom)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 1)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Auto_Mode == 1)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 1)
		//		&& (gPLC_Ctrl.m_cCheck_PLC_STOP == 1))
		//	//	&& (gPLC_Ctrl.m_cCheck_PLC_Pause == 0)
		//	//&& (gPLC_Ctrl.m_cCheck_PLC_Alarm == 1))
		//{
		//	if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_PAUSE)
		//	{
		//		if (((g_Pause_AutoControlProcess > 0) && (g_Pause_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
		//			|| (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY) || (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY))
		//		{
		//			g_Pause_AutoControlProcess = g_AutoControlProcess;
		//		}
		//		
		//		g_AutoControlProcess = _AUTO_ROBOT_CTRL_PAUSE;
		//		ctrlPLC_COM_Status.SetWindowText(_T("PLC PAUSE"));
		//		ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
		//	}
		//	//if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_MANUAL)//_AUTO_ROBOT_CTRL_MANUAL
		//	//{
		//	//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_MANUAL;
		//	//	ctrlPLC_COM_Status.SetWindowText(_T("PLC STOP"));
		//	//	ctrlPLC_COM_Status.SetBkColor(RGB(255, 0, 0));
		//	//}
		//}
		else if ((gPLC_Ctrl.m_bPLCCom)
				&& (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 1)
				&& (gPLC_Ctrl.m_cCheck_PLC_Auto_Mode == 1)
				&& (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 0)
				&& (gPLC_Ctrl.m_cCheck_PLC_STOP == 0)
				&& (gPLC_Ctrl.m_cCheck_PLC_Pause == 0)
				&& (gPLC_Ctrl.m_cCheck_PLC_Alarm == 0))
		{
			if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE)
			{
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_INIT;
				g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
				sInterProcess = 0;
				ctrlPLC_COM_Status.SetWindowText(_T("PLC START"));
				ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
			}

		}
	/*	else  if ((gPLC_Ctrl.m_bPLCCom)
		&& (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 1)
		&& (gPLC_Ctrl.m_cCheck_PLC_Auto_Mode == 1)
		&& (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 0)
		&& (gPLC_Ctrl.m_cCheck_PLC_STOP == 0)
		&& (gPLC_Ctrl.m_cCheck_PLC_Pause == 1)
		&& (gPLC_Ctrl.m_cCheck_PLC_Alarm == 0))*/
		else  if( (gPLC_Ctrl.m_bPLCCom)	&&
		( (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 1)
		|| (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 0)
		|| (gPLC_Ctrl.m_cCheck_PLC_STOP == 1)
		|| (gPLC_Ctrl.m_cCheck_PLC_Pause == 1)
		|| (gPLC_Ctrl.m_cCheck_PLC_Alarm == 1)))
		{
			if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_PAUSE)//_AUTO_SERVER_CTRL_PAUSE
			{
				if (((g_AutoControlProcess > 0) && (g_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
					|| (g_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY) || (g_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY))
				{
					g_Pause_AutoControlProcess =  g_AutoControlProcess;
				}
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_PAUSE;
				ctrlPLC_COM_Status.SetWindowText(_T("PLC PAUSE"));
				ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
			}
		}
	}
	else if ((CurrentSet->nRunStartType == MANUAL_START) && (g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))// (gPLC_Ctrl.m_bPLCCom
	{
		if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_MANUAL)
		{
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_MANUAL;
			ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
			ctrlPLC_COM_Status.SetBkColor(RGB(200, 200, 0));
		}
	}
	else
	{
		if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_MANUAL)
		{
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_MANUAL;
			ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
			ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
		}
	}



	switch (g_AutoControlProcess)
	{
	case _AUTO_ROBOT_CTRL_IDLE:
		sInterProcess = 0;
		gPLC_Ctrl.m_cResetControl = 0;
		g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;

		break;

	case  _AUTO_ROBOT_CTRL_MANUAL:
		if (gPLC_Ctrl.m_cResetControl == 1)
		{
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
			g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
		}
		//if ((CurrentSet->nRunStartType == AUTO_START) 
		//	&& (g_nSysType == AUTO_SYS) 
		//	&& (CurrentSet->bUsePLCRobot == TRUE)
		//	&& (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 0))
		//{
		//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;//_AUTO_ROBOT_CTRL_PLC_ON
		//}

		break;

	case _AUTO_ROBOT_CTRL_PAUSE:

		if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_LOAD)
		{
			if (gPLC_Ctrl.m_cCheckLoadFinish == 1)
			{
				gPLC_Ctrl.CommandClear();
				g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
			}
		}
		else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UNLOAD)
		{
			if (gPLC_Ctrl.m_cCheckUnLoadFinish == 1)
			{
				gPLC_Ctrl.CommandClear();
				g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
			}

		}
		else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY)
		{
			if (gPLC_Ctrl.m_cCheckReconnectFinish == 1)
			{
				gPLC_Ctrl.CommandClear();
				g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
			}

		}


		if((gPLC_Ctrl.m_bPLCCom)
			&& (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 1)
			&& (gPLC_Ctrl.m_cCheck_PLC_Auto_Mode == 1)
			&& (gPLC_Ctrl.m_cCheck_PLC_Manual_Mode == 0)
			&& (gPLC_Ctrl.m_cCheck_PLC_STOP == 0)
			&& (gPLC_Ctrl.m_cCheck_PLC_Pause == 0)
			&& (gPLC_Ctrl.m_cCheck_PLC_Alarm == 0)) 
		{
			if (((g_Pause_AutoControlProcess > 0) && (g_Pause_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
				||(g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY ) || (g_Pause_AutoControlProcess ==_AUTO_ROBOT_CTRL_UPDOWN_RETRY))
			{
				g_AutoControlProcess = g_Pause_AutoControlProcess;
				if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_JIGUP)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
				}
				else if(g_AutoControlProcess == _AUTO_ROBOT_CTRL_LOAD)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("LOAD"));
				}
				else if(g_AutoControlProcess == _AUTO_ROBOT_CTRL_JIG_DOWN)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("JIG DOWN"));
				}
				else if(g_AutoControlProcess == _AUTO_ROBOT_CTRL_TEST_RUN)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_JIGFINISH_UP)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_UNLOAD)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("UNLOAD"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("FFC:RTY"));
				}
				else if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("UPDOWN:RTY"));
				}
				else
				{
					ctrlPLC_COM_Status.SetWindowText(_T("RESTART"));
				}

			}
			else
			{
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
				ctrlPLC_COM_Status.SetWindowText(_T("RESTART"));
			}
			
		}
		break;

	case  _AUTO_ROBOT_CTRL_OFFLINE:
	case  _AUTO_ROBOT_CTRL_NOACK:
	case   _AUTO_ROBOT_CTRL_ALARM:
	case  	_AUTO_ROBOT_CTRL_PLC_ON:
		if (gPLC_Ctrl.m_cResetControl == 1)
		{
			gPLC_Ctrl.m_cResetControl = 0;
			/*if ((g_Pause_AutoControlProcess > 0) && (g_Pause_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
			{
				g_AutoControlProcess = g_Pause_AutoControlProcess;
			}
			else
			{*/
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
			//}
		}
		break;


	case   _AUTO_ROBOT_CTRL_TIMEOUT:
		if (gPLC_Ctrl.m_cResetControl == 1)
		{
			gPLC_Ctrl.m_cResetControl = 0;
			if (((g_Pause_AutoControlProcess > 0) && (g_Pause_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))
				|| (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY) || (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY))
			{
				g_AutoControlProcess = g_Pause_AutoControlProcess;
			}
			else
			{
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
			}

			sWaitCount = 0;
			sInterProcess = 0;
			sRetryCount = 0;
		}
		else
		{
			sWaitCount++;
			if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UNLOAD)
			{
				if (gPLC_Ctrl.m_cCheckUnLoadFinish)
				{
					sWaitCount = 0;
					g_AutoControlProcess = g_Pause_AutoControlProcess;
					sRetryCount = 0;
				}
				else if(sWaitCount > 30)
				{
					sWaitCount = 0;
					if (gPLC_Ctrl.m_cCheckUnLoad == 0)
					{
						//gPLC_Ctrl.CommandNgUnload();
						//gPLC_Ctrl.CommandPassUnload();
						//gPLC_Ctrl.CommandRetryUnload();
						if (gPLC_Ctrl.m_nTestResult == PLC_OK_MODE)
						{
							gPLC_Ctrl.CommandPassUnload();
						}
						else if (gPLC_Ctrl.m_NG_ConfirmSet == 1)
						{
							gPLC_Ctrl.CommandNgUnload();//NG Final
						}
						else
						{
							gPLC_Ctrl.CommandRetryUnload();//NG FIRST
						}
					}
				}
			}
			else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_LOAD)
			{
				if (gPLC_Ctrl.m_cCheckLoadFinish)
				{
					sWaitCount = 0;
					g_AutoControlProcess = g_Pause_AutoControlProcess;
					sRetryCount = 0;
				}
				else if (sWaitCount > 30)
				{
					sWaitCount = 0;
					if (gPLC_Ctrl.m_cCheckLoad == 0)
					{
						gPLC_Ctrl.CommandLoad();
						
					}
				}
			}
			else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_JIGUP)
			{
				
				if (gJigCtrl.m_bJigUpStatus == 1)
				{
					sWaitCount = 0;
					//sInterProcess = 2;
					g_AutoControlProcess = g_Pause_AutoControlProcess;
				
				}
				else
				{
					sWaitCount++;
					if (sWaitCount > 30)
					{

						gJigCtrl.Set_CylinderUP();
						sWaitCount = 0;		

					}
				}

			}
			else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_JIG_DOWN)
			{
				if (gJigCtrl.m_bJigDownStatus)
				{
					sWaitCount = 0;
					g_AutoControlProcess = g_Pause_AutoControlProcess;
					sRetryCount = 0;
				}
				else if (sWaitCount > 30)
				{
					sWaitCount = 0;
					gJigCtrl.Set_CylinderDOWN();
				}
			}
			else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_UPDOWN_RETRY)
			{
				if (gJigCtrl.m_bJigUpStatus == 1)
				{
					sWaitCount = 0;
					//sInterProcess = 2;
					g_AutoControlProcess = g_Pause_AutoControlProcess;

				}
				else
				{
					sWaitCount++;
					if (sWaitCount > 30)
					{
						gJigCtrl.Set_CylinderUP();
						sWaitCount = 0;

					}
				}
			}
			else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_JIGFINISH_UP)
			{
			if (gJigCtrl.m_bJigUpStatus == 1)
			{
				sWaitCount = 0;
				//sInterProcess = 2;
				g_AutoControlProcess = g_Pause_AutoControlProcess;

			}
			else
			{
				sWaitCount++;
				if (sWaitCount > 30)
				{
					gJigCtrl.Set_CylinderUP();
					sWaitCount = 0;

				}
			}
			}
			else if (g_Pause_AutoControlProcess == _AUTO_ROBOT_CTRL_FFC_RETRY)
			{
				if (gJigCtrl.m_bJigUpStatus == 1)
				{
					/*	sWaitCount = 0;
						sInterProcess = 2;
						g_AutoControlProcess = g_Pause_AutoControlProcess;*/
					if (gPLC_Ctrl.m_cCheckReconnectFinish)
					{
						sWaitCount = 0;
						g_AutoControlProcess = g_Pause_AutoControlProcess;
						sRetryCount = 0;
					}
					else if (sWaitCount > 30)
					{
						sWaitCount = 0;
						if (gPLC_Ctrl.m_cCheckReconnect == 0)
						{
							gPLC_Ctrl.CommandFfcRetry();

						}
					}

				}
				else
				{
					sWaitCount++;
					if (sWaitCount > 30)
					{

						gJigCtrl.Set_CylinderUP();
						sWaitCount = 0;

					}
				}

			}

		}

		break;


	case   _AUTO_ROBOT_CTRL_PCB_IN_ERROR:

		if ((gPLC_Ctrl.m_bPCB_Onload_JIG == 1) || (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside == 1)
			|| ((CurrentSet->bCheckUsePcbSensor == 1) && (gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 1)))
		{
			
			ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
			gJigCtrl.Set_CylinderUP();
			sWaitCount = 0;
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIGUP;
			sInterProcess = 0;
		}

		break;

	case _AUTO_ROBOT_CTRL_INIT:
		gPLC_Ctrl.m_cResetControl = 0;
		sWaitCount++;
		if (sWaitCount > 50)
		{
			ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
			//gJigCtrl.Set_CylinderUP();
			sWaitCount = 0;
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIGUP;
			sInterProcess = 0;
			sRetryCount = 0;
		}

		break;
	case _AUTO_ROBOT_CTRL_JIGUP:
		if (sInterProcess == 0)
		{
			sWaitCount = 0;
			sInterProcess = 1;
			sRetryCount = 0;
			if (CurrentSet->bCheckGrabberOnOffCheck == 1)
			{
				gJigCtrl.Set_Grabber_Power(0);
				Sleep(500);
			}
			gJigCtrl.Get_JigReady();
			ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
		}
		else if (sInterProcess == 1)
		{
			if(gPLC_Ctrl.m_cCheckJigUpStatus == 1) //if (gJigCtrl.m_bJigUpStatus == 1)
			{
				sWaitCount = 0;
				sInterProcess = 2;
				sRetryCount = 0;
			}
			else
			{
				sWaitCount++;
				if (sWaitCount > 10)
				{

					gJigCtrl.Set_CylinderUP();
					sWaitCount = 0;
					sInterProcess = 2;

				}

			}

		}
		else if (sInterProcess == 2)
		{
#ifdef _PLC_COM_SIM_DEBUG__MODE	
			gPLC_Ctrl.m_cCheckJigUpStatus = 1;
#endif
			if (gPLC_Ctrl.m_cCheckJigUpStatus == 1)// if (gJigCtrl.m_bJigUpStatus == 1)
			{

				sWaitCount = 0;
				//g_AutoControlProcess = _AUTO_ROBOT_CTRL_LOAD;
				sInterProcess = 3;
				gPLC_Ctrl.CommandClear();
			}
			else
			{
				sWaitCount++;
				if (sWaitCount > 150)
				{
#ifdef _PLC_COM_SIM_DEBUG__MODE
					gJigCtrl.m_bJigUpStatus = 1;

#else
					sRetryCount++;
					if (sRetryCount > 3)
					{
						g_Pause_AutoControlProcess = g_AutoControlProcess;
						g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
						ctrlPLC_COM_Status.SetWindowText(_T("TMOUT:UP"));
					}
					else
					{
						//gJigCtrl.Set_CylinderUP();
						sInterProcess = 1;
						sWaitCount = 0;
					}

#endif;
				}
				else
				{
					if ((sWaitCount % 10) == 0)
					{
						gJigCtrl.Get_JigReady();
					}
				}
			}
		}
		else if (sInterProcess == 3)
		{
			sWaitCount++;
			if (sWaitCount > 1)
			{
				//if ((gPLC_Ctrl.m_bPCB_Onload_JIG == 1) || (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside == 1)
				//	||( (CurrentSet->bCheckUsePcbSensor == 1)&&(gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 1)))
				if (gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 1)
				{
					//PCB ReportPCB_Check
					gServer_Ctrl.ReportPCB_Check();
					
					//if (IDNO == MessageBox("PCB INSIDE CHECK!!\r\n PCB IN THE JIG??", "PCB IN CHECK", MB_YESNO))
					//{
					//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_PCB_IN_ERROR;
					//	gPLC_Ctrl.m_bPCB_Onload_JIG = 0;

					//	ctrlROBOT_Status.SetWindowText(_T("- - -"));
					//	ctrlROBOT_Status.SetBkColor(RGB(64, 200, 64));

					//	/*	ctrlPLC_COM_Status.SetWindowText(_T("LOADING"));
					//		g_AutoControlProcess = _AUTO_ROBOT_CTRL_LOAD;
					//		sInterProcess = 0;
					//		gPLC_Ctrl.CommandLoad();*/
					//}
					//else
					{
						if (gPLC_Ctrl.m_nTestResult == PLC_OK_MODE)
						{
							sWaitCount = 0;
							sInterProcess = 0;
							g_AutoControlProcess = _AUTO_ROBOT_CTRL_UNLOAD;
							ctrlPLC_COM_Status.SetWindowText(_T("UNLOAD"));
							ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
						}
						else
						{
							ctrlPLC_COM_Status.SetWindowText(_T("JIG DOWN"));
							ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
							gPLC_Ctrl.CommandClear();
							//gJigCtrl.Set_CylinderDOWN();
							sWaitCount = 0;
							g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIG_DOWN;
							sInterProcess = 0;
						}

					}

					//PCB ReportPCB_Clear
					gServer_Ctrl.ReportPCB_Clear();//gServer_Ctrl.ReportPCB_Check();
				}
				else
				{
					ctrlPLC_COM_Status.SetWindowText(_T("LOADING"));
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_LOAD;
					sInterProcess = 0;
					//gPLC_Ctrl.CommandLoad();
					gPLC_Ctrl.m_nTestResult = PLC_INIT_MODE;
				}
			}
		}
		break;

	case  _AUTO_ROBOT_CTRL_LOAD:
		if (sInterProcess == 0)
		{
			ctrlPLC_COM_Status.SetWindowText(_T("LOADING"));
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_LOAD;
			sInterProcess = 1;
			gPLC_Ctrl.CommandLoad();
			sWaitCount = 0;
		}
		else if (sInterProcess == 1)
		{

			if (gPLC_Ctrl.m_cCheckLoadFinish)
			{
				ctrlPLC_COM_Status.SetWindowText(_T("JIG DOWN"));
				ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
				gPLC_Ctrl.CommandClear();

				
				//gJigCtrl.Set_CylinderDOWN();
				sWaitCount = 0;
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIG_DOWN;
				sInterProcess = 0;
				sRetryCount = 0;
				if (gPLC_Ctrl.m_cCheck_LoadFinish_NG_ConfirmSet == 1)
				{
					gPLC_Ctrl.m_NG_ConfirmSet = 1;
				}
				else
				{
					gPLC_Ctrl.m_NG_ConfirmSet = 0;
				}
			}
			else
			{
#ifdef _PLC_COM_SIM_DEBUG__MODE
				sWaitCount = 2000;
#endif
				sWaitCount++;
				if (sWaitCount > 2000)
				{
#ifdef _PLC_COM_SIM_DEBUG__MODE
					gPLC_Ctrl.m_cCheckLoadFinish = 1;

#else
					g_Pause_AutoControlProcess = g_AutoControlProcess;
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
					ctrlPLC_COM_Status.SetWindowText(_T("TMOUT:LD"));
					//gPLC_Ctrl.CommandLoad();
#endif
				}
				else if (sWaitCount == 100)
				{
					if (gPLC_Ctrl.m_cCheckLoad == 0)
					{
						gPLC_Ctrl.CommandLoad();
					}
				}
			}
		}

		break;

	case _AUTO_ROBOT_CTRL_JIG_DOWN:
		if (sInterProcess == 0)
		{
			if ((gPLC_Ctrl.m_cCheckRobotMoving == 0) && (gPLC_Ctrl.m_cCheck_PLC_Start_Run == 1))
			{
				sWaitCount = 0;
				sInterProcess = 1;
				sRetryCount = 0;
				gJigCtrl.m_bJigDownStatus = 0;
				gJigCtrl.Get_JigReady();
				ctrlPLC_COM_Status.SetWindowText(_T("JIG DOWN"));
				ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
				gPLC_Ctrl.CommandModelName(CurrentSet->sChassisName); //gPLC_Ctrl.CommandClear();
			}
			else
			{
				sWaitCount++;
				if (sWaitCount > 30)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("WAIT DOWN"));
					ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));

					sWaitCount = 0;
				}
			}
		}
		else if (sInterProcess == 1)
		{
			if (gJigCtrl.m_bJigDownStatus == 1)
			{
				sWaitCount = 0;
				sInterProcess = 2;
				sRetryCount = 0;
			}
			else
			{
				sWaitCount++;
				if (sWaitCount > 20)
				{
					gJigCtrl.Set_CylinderDOWN();

					sWaitCount = 0;
					sInterProcess = 2;
					sRetryCount = 0;
				}
			}

		}
		else if (sInterProcess == 2)
		{
			if (gJigCtrl.m_bJigDownStatus == 1)
			{
				sWaitCount = 0;
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_RUN;
				if (g_AutoControlProcessNoTEST)
				{
					ctrlPLC_COM_Status.SetWindowText(_T("TEST DLY"));
				}
				else
				{
					ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));
				}
				ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
				//gPLC_Ctrl.CommandLoad();
				sInterProcess = 0;
			}
			else
			{
#ifdef _PLC_COM_SIM_DEBUG__MODE
				sWaitCount = 2000;
#endif
				sWaitCount++;
				if (sWaitCount > 150)//50*250
				{
#ifdef _PLC_COM_SIM_DEBUG__MODE
					gJigCtrl.m_bJigDownStatus = 1;

#else
					sRetryCount++;
					if (sRetryCount > 2)
					{
						g_Pause_AutoControlProcess = g_AutoControlProcess;
						g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
						ctrlPLC_COM_Status.SetWindowText(_T("TMOUT(DN)"));
					}
					else
					{
						gJigCtrl.Set_CylinderDOWN();
						sWaitCount = 0;
						sInterProcess = 1;
					}
#endif
				}
				else if ((sWaitCount % 10) == 0)
				{
					gJigCtrl.Get_JigReady();
				}

			}
		}
		break;

	case  _AUTO_ROBOT_CTRL_TEST_RUN:
		if (sInterProcess == 0)
		{
			if (CurrentSet->bCheckGrabberOnOffCheck == 1)
			{
				gJigCtrl.Set_Grabber_Power(1);
			}
			if (g_AutoControlProcessNoTEST)
			{
				sWaitCount++;
				if (sWaitCount > g_AutoControlProcessWaitTime * 16)//20*80
				{
					sInterProcess = 3;
					sWaitCount = 0;
				}
				else
				{
					if (sWaitCount % 16 == 15)
					{
						CString str;
						str.Format(_T("TEST %d"), sWaitCount / 16+1);

						ctrlPLC_COM_Status.SetWindowText(str);
					}
				}
			}
			else
			{
				gPLC_Ctrl.m_nUserRetry = 0;
				gPLC_Ctrl.m_nTestResult = PLC_INIT_MODE;
				gPLC_Ctrl.m_nFirstVideoNG = 0;
				gPLC_Ctrl.m_nTestVideoCount = 0;
				gPLC_Ctrl.m_nCurrentVideoNG = 0;
				gPLC_Ctrl.m_nCheckAbortRetry = 0;
				
				//PLC_OK_MODE
								//PLC_NG_MODE
								//PLC_CHECK_MODE

				OnRunRun();//RUN Test
				sInterProcess = 1;
				sWaitCount = 0;
			}

		}
		else if (sInterProcess == 1)
		{
			if ((m_bThreadRunning) && (CurrentSet->bIsRunning))
			{

				sInterProcess = 2;
				sWaitCount = 0;
			}

		}
		else if (sInterProcess == 2)
		{
			sWaitCount++;
			if (sWaitCount > 50)
			{
				if (m_bResultDlgActivate == 1)//m_ResultDlg->SetClose();
				{
					/*	if (!m_bThreadRunning)
						{
							sInterProcess = 3;
							sWaitCount = 0;
						}
					*/
					//PLC_OK_MODE
					//PLC_NG_MODE
					//PLC_CHECK_MODE
					gPLC_Ctrl.m_nUserRetry++;
					if (gPLC_Ctrl.m_nTestResult == PLC_OK_MODE)
					{
						sInterProcess = 3;
						sWaitCount = 0;
						//ProcessAutokey(STOP_BTN);
						gServer_Ctrl.ServerReportTestResult(PLC_OK_MODE);
					}
					else// if (gPLC_Ctrl.m_nTestResult == PLC_NG_MODE)
					{
						if (gPLC_Ctrl.m_nTestResult == PLC_NG_MODE)
						{
							gServer_Ctrl.ServerReportTestResult(PLC_NG_MODE);
						}					

						AddStringToStatus("Jig Up!");
						///////////////
						if (m_WhiteResetFlag)
						{
							if (g_pView->m_WhiteResetCount >= 3)
							{
								gJigCtrl.Set_CylinderUP();
								
								Sleep(1000);
								for (int i = 0; i < 10; i++)
								{
									if (gJigCtrl.m_bJigUpStatus)
										break;
									Sleep(500);
								}

								AddStringToStatus("Grab HW Test Fail 3 Times!!!");
							//	gJigCtrl.Set_CylinderUP();
								CurrentSet->nRunStartType = MANUAL_START;
								//	g_pView->KillTimer(TIMER_AUTO_CONTROL_ID);

								g_AutoControlProcess = _AUTO_ROBOT_CTRL_MANUAL;
								ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
								ctrlPLC_COM_Status.SetBkColor(RGB(200, 200, 0)); //ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
								gPLC_Ctrl.Command3TimesError();
								g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
								AfxMessageBox("Grab HW Test Fail !!");
								gPLC_Ctrl.Command3TimesErrorClear();
							}
							else
							{
								sInterProcess = 3;
								sWaitCount = 0;
								//	ProcessAutokey(STOP_BTN);
								gPLC_Ctrl.m_nTestResult = PLC_NG_MODE;
							}
						}
						else if (gPLC_Ctrl.m_nUserRetry >= 2)
						{
							if (gPLC_Ctrl.m_nCheckAbortRetry == 1)
							{
								gPLC_Ctrl.m_nUserRetry = 0;
								gPLC_Ctrl.m_nCheckAbortRetry = 2;

								g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
								ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
								ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
								sInterProcess = 0;
								sWaitCount = 0;
							}
							//gPLC_Ctrl
							//if ((gPLC_Ctrl.m_nTestResult == PLC_CHECK_MODE) && (g_ID_Check != 4))
							//{
							//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
							//	ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
							//	ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
							//	sInterProcess = 0;
							//	sWaitCount = 0;
							//}
							else
							{
								sInterProcess = 3;
								sWaitCount = 0;
								//	ProcessAutokey(STOP_BTN);
								gPLC_Ctrl.m_nTestResult = PLC_NG_MODE;
							}

							//#define _AUTO_ROBOT_CTRL_FFC_RETRY		17
							//#define _AUTO_ROBOT_CTRL_UPDOWN_RETRY	18
						}
						else
						{
							if ((gPLC_Ctrl.m_nTestResult == PLC_CHECK_MODE)&&(g_ID_Check != 4))
							{
								g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
								ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
								ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
								sInterProcess = 0;
								sWaitCount = 0;
							}
							else
							{

								sInterProcess = 3;
								sWaitCount = 0;
								//ProcessAutokey(STOP_BTN);
								gPLC_Ctrl.m_nTestResult = PLC_NG_MODE;
							/*	if (gPLC_Ctrl.m_nFirstVideoNG == 1)
								{
									g_AutoControlProcess = _AUTO_ROBOT_CTRL_FFC_RETRY;
									ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
									ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
								}
								else
								{
									g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
									ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
									ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
								}
*/
							}
							
							sRetryCount = 0;
						}

					}
					/*	else if(gPLC_Ctrl.m_nTestResult == PLC_CHECK_MODE)
						{

						}*/

				}
				else if (m_StopStepDlg.m_bActivate == TRUE)
				{
					
					//if (g_nNoFailedStep != 0)
					{
						gPLC_Ctrl.m_nUserRetry++;

						if (gPLC_Ctrl.m_nUserRetry >= 2)
						{

							ProcessAutokey(NG_BTN);
							gPLC_Ctrl.m_nTestResult = PLC_NG_MODE;
								sInterProcess = 3;
								sWaitCount = 0;
						}
						else
						{
							//if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
							//{
							//	gJigCtrl.Set_Gender_Reset(0);
							//	AddStringToStatus("Gender_Reset!!");
							//	_Wait(500);
							//	//gJigCtrl.Set_Gender_Reset(0);
							//	//AddStringToStatus("Gender_Reset!!");
							//	//_Wait(500);
							//	//gJigCtrl.Set_Gender_Reset(0);
							//	//AddStringToStatus("Gender_Reset!!");
							//	//_Wait(500);
							//}

							//RETRY Process
							//if (gPLC_Ctrl.m_nFirstVideoNG == 1)//m_nTestVideoCount
							//{
							//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_FFC_RETRY;
							//	ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
							//	ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
							//}
							//else
							{
								g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
								ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
								ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
							}
							sInterProcess = 0;
							sWaitCount = 0;
							sRetryCount = 0;
						}
					}
				}
				else if ((m_pVfMeasureCheckDlg.m_bActivate == TRUE)&&(m_pVfMeasureCheckDlg.m_bVfCheckNG_Wait == 1))
				{
					gPLC_Ctrl.m_nUserRetry++;
					if (m_pVfMeasureCheckDlg.m_bVfCheckResult == 0)
					{

						if (gPLC_Ctrl.m_nUserRetry >= 2)
						{

							ProcessAutokey(NG_BTN);
							gPLC_Ctrl.m_nTestResult = PLC_NG_MODE;

							sInterProcess = 3;
							sWaitCount = 0;
						}
						else
						{
							
							for (int i = 0; i < 3; i++)
							{
								int lWaitCnt = 0;
								TVCommCtrl.Send_TVControl_Command("aa 00 00", 700);
								TVCommCtrl.Send_TVControl_Command("ai 00 30", 700);
								TVCommCtrl.Send_TVControl_Command("aa 00 20", 700);
								TVCommCtrl.Send_TVControl_Command("ar 00 30", 700);

								while (m_pVfMeasureCheckDlg.m_bTestRunning == 1)
								{
									lWaitCnt++;
									_Wait(300);
									if (lWaitCnt > 30)
									{
										break;
									}
								}

								if (m_pVfMeasureCheckDlg.m_bVfCheckNG_Wait == 1)
								{
									m_pVfMeasureCheckDlg.m_bVfCheckNG_Wait = 0;
									ProcessAutokey(RETRY_BTN);
									m_pVfMeasureCheckDlg.m_nVfCheckCount++;
								}
								else
								{
									break;
								}								

							}
							if (m_pVfMeasureCheckDlg.m_bVfCheckResult == 0)
							{
							//	ProcessAutokey(NG_BTN);
							//	gPLC_Ctrl.m_nTestResult = PLC_NG_MODE;

							//	sInterProcess = 3;
								sInterProcess = 0;
								sWaitCount = 0;
								g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
								ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
								ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
							}
						}
					}
				}
				else if ((m_pInputCheckDlg.m_bActivate) && (m_pInputCheckDlg.m_bInputCheckWait == 1))
				{
					m_pInputCheckDlg.m_nInputCheckCount++;
					m_pInputCheckDlg.m_bInputCheckWait = 0;
					if (m_pInputCheckDlg.m_nInputCheckCount > 10)//if (m_pInputCheckDlg.m_nInputCheckCount > 3)
					{
						m_pInputCheckDlg.m_nInputCheckCount = 0;
						gPLC_Ctrl.m_nUserRetry++;
						if (gPLC_Ctrl.m_nUserRetry >= 2)
						{
#if 1

							gJigCtrl.Set_CylinderUP();
							CurrentSet->nRunStartType = MANUAL_START;
							//	g_pView->KillTimer(TIMER_AUTO_CONTROL_ID);

							g_AutoControlProcess = _AUTO_ROBOT_CTRL_MANUAL;
							ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
							ctrlPLC_COM_Status.SetBkColor(RGB(200, 200, 0)); //ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
							gPLC_Ctrl.Command3TimesError();
							g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
#else
							ProcessAutokey(NG_BTN);
#endif
							//sInterProcess = 3;
							sWaitCount = 0;
						}
						else
						{
							g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
							ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
							ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
							sInterProcess = 0;
							sWaitCount = 0;
							sRetryCount = 0;
						}
					}
					else
					{
						ProcessAutokey(RETRY_BTN);
						//sWaitCount = 0;
					}			
					
				}
				else if (m_ManualTestDlg.m_bActivate == TRUE)											
					{
						gPLC_Ctrl.m_nUserRetry++;
					//	ProcessAutokey(OK_BTN);
					//	gPLC_Ctrl.m_nUserRetry = 0;
					if (gPLC_Ctrl.m_nUserRetry >= 2)
					{

						ProcessAutokey(NG_BTN);

						sInterProcess = 3;
						sWaitCount = 0;
					}
					else
					{
						//if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
						//{
						//	gJigCtrl.Set_Gender_Reset(0);
						//	AddStringToStatus("Gender_Reset!!");
						//	_Wait(500);
						//	//gJigCtrl.Set_Gender_Reset(0);
						//	//_Wait(500);
						//	//gJigCtrl.Set_Gender_Reset(0);
						//	//_Wait(500);
						//}

						//if (gPLC_Ctrl.m_nFirstVideoNG == 1)
						//{
						//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_FFC_RETRY;
						//	ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
						//	ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));

						//	sInterProcess = 0;
						//	sWaitCount = 0;
						//	sRetryCount = 0;
						//}
						//else
						{
						
							g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
							ctrlPLC_COM_Status.SetWindowText(_T("RTY UP"));
							ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));

							sInterProcess = 0;
							sWaitCount = 0;
							sRetryCount = 0;
						}
					}					
				}
			}
		}
		else if (sInterProcess == 3)
		{
			sWaitCount++;
			if (sWaitCount > 20)
			{
				//gJigCtrl.Set_CylinderUP();
				sWaitCount = 0;
				sInterProcess = 0;
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIGFINISH_UP;
				ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
				ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
			}
		}		

		break;

	case _AUTO_ROBOT_CTRL_JIGFINISH_UP:
		if (sInterProcess == 0)
		{
			sWaitCount = 0;
			gJigCtrl.m_bJigUpStatus = 0;
			gJigCtrl.Set_CylinderUP();
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIGFINISH_UP;
			ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
			ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
			sInterProcess = 1;
		}
		else if (sInterProcess == 1)
		{
			if ((gJigCtrl.m_bJigUpStatus == 1)|| (gPLC_Ctrl.m_cCheckJigUpStatus == 1))
			{
				sWaitCount = 0;
				sInterProcess = 0;
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_UNLOAD;
				ctrlPLC_COM_Status.SetWindowText(_T("UNLOAD"));
				ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
				//if(PASS)
				//{
				//if (g_nNoFailedStep == 0)
				//{
				//	gPLC_Ctrl.CommandPassUnload();
				//}
				//else
				//{
				//	gPLC_Ctrl.CommandNgUnload();
				//}
				if (g_AutoControlProcessNoTEST == 0)
				{
					SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
				}
			}
			else
			{
				sWaitCount++;
				if (sWaitCount > 150)
				{
#ifdef _PLC_COM_SIM_DEBUG__MODE
					gJigCtrl.m_bJigUpStatus = 1;

#else
					sRetryCount++;
					if (sRetryCount > 3)
					{
						g_Pause_AutoControlProcess = g_AutoControlProcess;
						g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
						ctrlPLC_COM_Status.SetWindowText(_T("TIMEOUT(UP)"));
					}
					else
					{
						gJigCtrl.Set_CylinderUP();
						sWaitCount = 0;
						ctrlPLC_COM_Status.SetWindowText(_T("UP re"));
					}
#endif
				}
			}
		}
		break;

	case  _AUTO_ROBOT_CTRL_UNLOAD:
		if (sInterProcess == 0)
		{
			sWaitCount = 0;

			g_AutoControlProcess = _AUTO_ROBOT_CTRL_UNLOAD;
			ctrlPLC_COM_Status.SetWindowText(_T("UNLOAD"));
			ctrlPLC_COM_Status.SetBkColor(RGB(0, 255, 0));
			//if(PASS)
			//{
		/*	if (g_nNoFailedStep == 0)
			{
				
			}
			else
			{
				gPLC_Ctrl.CommandNgUnload();
			}*/

			if (g_AutoControlProcessNoTEST == 1)
			{
				gPLC_Ctrl.CommandPassUnload();
			}
			else
			{

				if (gPLC_Ctrl.m_nTestResult == PLC_OK_MODE)
				{
					gPLC_Ctrl.CommandPassUnload();
				}
				else if (gPLC_Ctrl.m_NG_ConfirmSet == 1)
				{
					gPLC_Ctrl.CommandNgUnload();//NG Final
				}
				else if (g_ID_Check == 4)
				{
					gPLC_Ctrl.CommandNgUnload();//NG Final
				}		
				else
				{
					gPLC_Ctrl.CommandRetryUnload();//NG FIRST
				}
			}
			sInterProcess = 1;
		}
		else if (sInterProcess == 1)
		{

#ifdef _PLC_COM_SIM_DEBUG__MODE
			gPLC_Ctrl.m_cCheckUnLoadFinish = 1;

#endif
			if (gPLC_Ctrl.m_cCheckUnLoadFinish)
			{
				ctrlPLC_COM_Status.SetWindowText(_T("JIG UP"));
				ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
				gPLC_Ctrl.CommandClear();

				sWaitCount = 0;
				sInterProcess = 0;
				if (gPLC_Ctrl.m_nTestResult == PLC_OK_MODE)
				{
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_OK;
					gPLC_Ctrl.m_nTestStepNGCount = 0;
					gPLC_Ctrl.m_NG_StepNoOld = -1;
					gPLC_Ctrl.m_NG_StepNo = -1;
				}
				else
				{
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_NG;
					
					if ((gPLC_Ctrl.m_NG_StepNo > 0)&&(gPLC_Ctrl.m_NG_StepNo < 499))
					{
						int lError3TimeFlag = 0;
						if (gPLC_Ctrl.m_NG_StepNo3Times == gPLC_Ctrl.m_NG_StepNo)
						{
							int nTotalCnt = StepList.GetCount();
							if ((gPLC_Ctrl.m_NG_StepNo > 0) && (gPLC_Ctrl.m_NG_StepNo < nTotalCnt))
							{

								POSITION PosStep = StepList.FindIndex(gPLC_Ctrl.m_NG_StepNo - 1);

								CStep* pStep = StepList.GetAt(PosStep);
								CString str;
								str.Format("ERROR STEP[%d]: ", gPLC_Ctrl.m_NG_StepNo );
								str += pStep->m_sName;
								gPLC_Ctrl.Command3TimesError();
								MessageBox(str, "Continue Error Step!! \r\n Not Fixed!! Check Again!!");
								gPLC_Ctrl.Command3TimesErrorClear();
								lError3TimeFlag = 1;

							}
						}
						else
						{
							if (gPLC_Ctrl.m_NG_StepNoOld == gPLC_Ctrl.m_NG_StepNo)
							{
								gPLC_Ctrl.m_nTestStepNGCount++;
								if (gPLC_Ctrl.m_nTestStepNGCount >= 2)
								{
									gPLC_Ctrl.m_NG_StepNo3Times = gPLC_Ctrl.m_NG_StepNo;
									int nTotalCnt = StepList.GetCount();
									if ((gPLC_Ctrl.m_NG_StepNo > 0) && (gPLC_Ctrl.m_NG_StepNo < nTotalCnt))
									{
										POSITION PosStep = StepList.FindIndex(gPLC_Ctrl.m_NG_StepNo - 1);
										CStep* pStep = StepList.GetAt(PosStep);
										CString str;
										str.Format("ERROR STEP[%d]: ", gPLC_Ctrl.m_NG_StepNo );
										str += pStep->m_sName;
										gPLC_Ctrl.Command3TimesError();
										MessageBox(str, "Continue Error Step!!");
										gPLC_Ctrl.Command3TimesErrorClear();
									}
								}
							}
							else
							{

								gPLC_Ctrl.m_nTestStepNGCount = 0;
							}
						}
						gPLC_Ctrl.m_NG_StepNoOld = gPLC_Ctrl.m_NG_StepNo;


						if ((gPLC_Ctrl.m_NG_StepNo > 0) && (gPLC_Ctrl.m_NG_StepNo < 100))
						{
							gPLC_Ctrl.m_nTestStepNGCountList[gPLC_Ctrl.m_NG_StepNo]++;
						}

						if ((gPLC_Ctrl.m_nTestStepNGCountList[gPLC_Ctrl.m_NG_StepNo] % 5 == 0)
							&& (gPLC_Ctrl.m_nTestStepNGCountList[gPLC_Ctrl.m_NG_StepNo] > 0)
							&&(lError3TimeFlag == 0))
						{
							int nTotalCnt = StepList.GetCount();
							if ((gPLC_Ctrl.m_NG_StepNo > 0) && (gPLC_Ctrl.m_NG_StepNo < nTotalCnt))
							{
								POSITION PosStep = StepList.FindIndex(gPLC_Ctrl.m_NG_StepNo - 1);
								CStep* pStep = StepList.GetAt(PosStep);
								CString str;
								str.Format("ERROR STEP[%d]: %d times ", gPLC_Ctrl.m_NG_StepNo, gPLC_Ctrl.m_nTestStepNGCountList[gPLC_Ctrl.m_NG_StepNo]);
								str += pStep->m_sName;
								gPLC_Ctrl.Command3TimesError();
								MessageBox(str, " Count 5 Error Step!!");
								gPLC_Ctrl.Command3TimesErrorClear();
							}
						}
					}
				}
			}
			else
			{
				sWaitCount++;
				
#ifdef _PLC_COM_SIM_DEBUG__MODE
				if (sWaitCount > 5000)
				{
					gPLC_Ctrl.m_cCheckUnLoadFinish = 1;

#else
				if (sWaitCount > 1250)
				{
					g_Pause_AutoControlProcess = g_AutoControlProcess;
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
					ctrlPLC_COM_Status.SetWindowText(_T("TIMEOUT(UL)"));
					//gPLC_Ctrl.CommandNgUnload();
#endif
				}
				else if (sWaitCount % 50)// if (sWaitCount == 100)
				{
					if (gPLC_Ctrl.m_cCheckUnLoad == 0)
					{
						if (gPLC_Ctrl.m_nTestResult == PLC_OK_MODE)
						{
							gPLC_Ctrl.CommandPassUnload();
							//g_OK_Count++;
						}
						else if (gPLC_Ctrl.m_NG_ConfirmSet == 1)
						{
							gPLC_Ctrl.CommandNgUnload();//NG Final
							//g_NG_Count++;
						}
						else
						{
							gPLC_Ctrl.CommandRetryUnload();//NG FIRST
						}
					}
				}
			}
		}
		break;

	case  _AUTO_ROBOT_CTRL_TEST_NG:

		//if ((gPLC_Ctrl.m_NG_StepNo > 0) && (gPLC_Ctrl.m_NG_StepNo < 499))
		//{
		//	if (gPLC_Ctrl.m_NG_StepNoOld == gPLC_Ctrl.m_NG_StepNo)
		//	{
		//		gPLC_Ctrl.m_nTestStepNGCount++;
		//		if (gPLC_Ctrl.m_nTestStepNGCount > 2)
		//		{
		//			int nTotalCnt = StepList.GetCount();
		//			if ((gPLC_Ctrl.m_NG_StepNo > 0) && (gPLC_Ctrl.m_NG_StepNo < nTotalCnt))
		//			{

		//				POSITION PosStep = StepList.FindIndex(gPLC_Ctrl.m_NG_StepNo - 1);

		//				CStep* pStep = StepList.GetAt(PosStep);
		//				CString str;
		//				str.Format("ERROR STEP[%d]: ", gPLC_Ctrl.m_NG_StepNo + 1);
		//				str += pStep->m_sName;
		//				MessageBox(str, "Continue Error Step!!");
		//			}
		//		}
		//	}
		//	else
		//	{

		//		gPLC_Ctrl.m_nTestStepNGCount = 0;
		//	}
		//	gPLC_Ctrl.m_NG_StepNoOld = gPLC_Ctrl.m_NG_StepNo;
		//}

	case _AUTO_ROBOT_CTRL_TEST_OK:
		sWaitCount = 0;
		sInterProcess = 0;
		g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIGUP;
		gPLC_Ctrl.CommandRegRead();
		//	gJigCtrl.Set_CylinderUP();
		//	ctrlPLC_COM_Status.SetWindowText(_T("UNLOAD"));
			//if (gPLC_Ctrl.m_cCheckUnLoadFinish)
			//{
			//	gPLC_Ctrl.CommandClear();
			//	sWaitCount = 0;
			//	sInterProcess = 0;
			//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_JIGUP;
			//}
		break;
	case _AUTO_ROBOT_CTRL_UPDOWN_RETRY:

		if (sInterProcess == 0)
		{
			sRetryCount = 0;
			ctrlPLC_COM_Status.SetWindowText(_T("RETRY:UP"));
			gJigCtrl.Set_CylinderUP();
			sWaitCount = 0;
			sInterProcess = 1;

		//	gPLC_Ctrl.m_nFirstVideoNG = 0;
		//	gPLC_Ctrl.m_nTestVideoCount = 0;
			
		}
		else if (sInterProcess == 1)
		{	
			
#ifdef				_PLC_COM_SIM_DEBUG__MODE

			gPLC_Ctrl.m_nCurrentVideoNG = 1;
#endif

			sInterProcess = 2;

		}
		else if (sInterProcess == 2)
		{
#ifdef		_PLC_COM_SIM_DEBUG__MODE
			gJigCtrl.m_bJigUpStatus = 1;// sInterProcess = 6;
#endif
			sWaitCount++;
			if (gJigCtrl.m_bJigUpStatus)
			{
				sWaitCount = 0;
				sRetryCount = 0;
				sInterProcess = 3;
				gPLC_Ctrl.CommandClear();
			}
			else
			{
				if (sWaitCount > 150)
				{
					sRetryCount++;
					if (sRetryCount > 2)
					{
						gJigCtrl.Set_CylinderUP();
						g_Pause_AutoControlProcess = g_AutoControlProcess;
						g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
						ctrlPLC_COM_Status.SetWindowText(_T("TIMEOUT(UP)"));
					}
					else
					{
						gJigCtrl.Set_CylinderUP();
					}
				}
			}
		}
		else if (sInterProcess == 3)
		{
#ifdef				_PLC_COM_SIM_DEBUG__MODE
			
			gPLC_Ctrl.m_nCurrentVideoNG = 1;
#endif
			if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
			{
				if (CurrentSet->bCheckImageFullReset == 1)
				{
					//AddStringToStatus("Grabber Power OFF");
					//gJigCtrl.Set_Grabber_Power(0);
					//Sleep(1000);

					//AddStringToStatus("Grabber Power ON");
					//gJigCtrl.Set_Grabber_Power(1);
					//Sleep(2000);
					//gJigCtrl.Set_IF_Pack_Reset(0);
					//AddStringToStatus("IF Pack Reset");
					//Sleep(1000);

				}
				else if ((gPLC_Ctrl.m_NG_StepNoOld == gPLC_Ctrl.m_NG_StepNo) && (gPLC_Ctrl.m_nTestStepNGCount == 1))
				{
					//gJigCtrl.Set_IF_Pack_Reset(0);
					//AddStringToStatus("IF Pack Reset");
					//Sleep(1000);				
				}			
			}
						
			sWaitCount = 0;
			sInterProcess = 4;

#ifdef		_PLC_COM_SIM_DEBUG__MODE
			 sInterProcess = 5;
#endif

		}
		//else if (sInterProcess == 3)
		//{
		//	sWaitCount++;
		//	if (gPLC_Ctrl.m_cCheckReconnectFinish)
		//	{
		//		sWaitCount = 0;
		//		sInterProcess = 4;
		//		gPLC_Ctrl.CommandClear();
		//	}
		//	else if (sWaitCount > 1500)
		//	{
		//		//sRetryCount++;
		//		
		//		g_Pause_AutoControlProcess = g_AutoControlProcess;
		//		g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
		//		ctrlPLC_COM_Status.SetWindowText(_T("TMOUT:FFC\)"));
		//		
		//	}
		//}
		else if (sInterProcess == 4)
		{
			sRetryCount = 0;
			gJigCtrl.Set_CylinderDOWN();
			ctrlPLC_COM_Status.SetWindowText(_T("RETRY:DN"));
			sWaitCount = 0;
			sInterProcess = 5;
		}
		else if (sInterProcess == 5)
		{
			if (gJigCtrl.m_bJigDownStatus)
			{
				sWaitCount = 0;
				gPLC_Ctrl.CommandClear();
				if (gPLC_Ctrl.m_nTestResult == PLC_CHECK_MODE)
				{
					gPLC_Ctrl.m_nTestResult = PLC_INIT_MODE;
					gPLC_Ctrl.m_nCurrentVideoNG = 0;

					sRetryCount = 0;

					sWaitCount = 0;
					sInterProcess = 1;
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_RUN;
					ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));
					if (m_bResultDlgActivate == 1)
					{
						m_ResultDlg->SetContinue();
					}
					else
					{
						ProcessAutokey(RETRY_BTN);
					}
				}
				else
				{
					sInterProcess = 6;
					gJigCtrl.Set_ACPower(1);
					ctrlPLC_COM_Status.SetWindowText(_T("POWER ON"));
				}


			}
			else
			{
				sWaitCount++;
				if (sWaitCount > 150)
				{
					sRetryCount++;
					if (sRetryCount > 2)
					{
						
						g_Pause_AutoControlProcess = g_AutoControlProcess;
						g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
						ctrlPLC_COM_Status.SetWindowText(_T("TMOUT:DN"));
					}
					else
					{
						gJigCtrl.Set_CylinderDOWN();
						sWaitCount = 0;
					}
				}
			}
		}
		else if (sInterProcess == 6)
		{
#ifdef		_PLC_COM_SIM_DEBUG__MODE
		// sWaitCount = 600;
#endif
			sWaitCount++;

			if (m_BlackPictureFlag == 1)
			{
				if (sWaitCount > 280)
				{
					if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
					{
						Sleep(1000);
						if (CurrentSet->bCheckRotateReset == 1)
						{
							Sleep(2000);

							OnImgRotate();

							Sleep(1000);
							OnImgRotate();
						}
					}

					gPLC_Ctrl.m_nCurrentVideoNG = 0;

					sRetryCount = 0;


					if (m_pVfMeasureCheckDlg.m_bActivate == TRUE)
					{
						TVCommCtrl.Send_TVControl_Command("aa 00 00", 500);
						TVCommCtrl.Send_TVControl_Command("ai 00 30", 500);
						TVCommCtrl.Send_TVControl_Command("aa 00 20", 500);
						TVCommCtrl.Send_TVControl_Command("ar 00 30", 500);
					}

					sWaitCount = 0;
					sInterProcess = 1;
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_RUN;
					ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));
					if (m_bResultDlgActivate == 1)
					{
						m_ResultDlg->SetContinue();
					}
					else
					{
						ProcessAutokey(RETRY_BTN);
					}
				}
			}
			else if (CurrentSet->bCheckOledReset == 1)
			{
				if (sWaitCount > 280)
				{
					if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
					{

						AddStringToStatus("(sWaitCount > 280)");
						g_AutoVideoRetry = 1;
						Sleep(1000);
						ResetGrabStartThread();
						//if (g_AutoVideoRetry != 1)
						//{
						//	if (!GrabberReset())
						//	{
						//		Sleep(1000);
						//		if (!GrabberReset())
						//		{
						//			AddStringToStatus("Grabber Not Connected");
						//		}
						//	}
						//}
					//	AddStringToStatus("Grabber Reset");
						if (CurrentSet->bCheckRotateReset == 1)
						{
							Sleep(2000);

							OnImgRotate();

							Sleep(1000);
							OnImgRotate();
						}
					}
					else
					{
						Sleep(2000);
					}

					if (m_pVfMeasureCheckDlg.m_bActivate == TRUE)
					{
						TVCommCtrl.Send_TVControl_Command("aa 00 00", 500);
						TVCommCtrl.Send_TVControl_Command("ai 00 30", 500);
						TVCommCtrl.Send_TVControl_Command("aa 00 20", 500);
						TVCommCtrl.Send_TVControl_Command("ar 00 30", 500);
					}

					gPLC_Ctrl.m_nCurrentVideoNG = 0;

					sRetryCount = 0;

					sWaitCount = 0;
					sInterProcess = 1;
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_RUN;
					ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));
					if (m_bResultDlgActivate == 1)
					{
						m_ResultDlg->SetContinue();
					}
					else
					{
						ProcessAutokey(RETRY_BTN);
					}					
				}				
			}
			else
			{
				if (sWaitCount == 10)//|| (sWaitCount == 20))
				{
					if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
					{
						if (CurrentSet->bCheckImageFullReset == 1)
						{
							AddStringToStatus("Grabber Power OFF");
							if (gJigCtrl.Set_Grabber_Power(0) == 0)
							{
								gJigCtrl.Set_Grabber_Power(0);
							}

						}
						else if ((gPLC_Ctrl.m_NG_StepNoOld == gPLC_Ctrl.m_NG_StepNo) && (gPLC_Ctrl.m_nTestStepNGCount == 1))
						{
							AddStringToStatus("Grabber Power OFF");
							if (gJigCtrl.Set_Grabber_Power(0) == 0)
							{
								gJigCtrl.Set_Grabber_Power(0);
							}

						}

					}
				}
				else if (sWaitCount == 110)//||(sWaitCount == 110))
				{
					if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
					{
						if (CurrentSet->bCheckImageFullReset == 1)
						{


							AddStringToStatus("Grabber Power ON");
							if (gJigCtrl.Set_Grabber_Power(1) == 0)
							{
								gJigCtrl.Set_Grabber_Power(1);
							}

						}
						else if ((gPLC_Ctrl.m_NG_StepNoOld == gPLC_Ctrl.m_NG_StepNo) && (gPLC_Ctrl.m_nTestStepNGCount == 1))
						{


							AddStringToStatus("Grabber Power ON");
							if (gJigCtrl.Set_Grabber_Power(1) == 0)
							{
								gJigCtrl.Set_Grabber_Power(1);
							}
						}
					}
				}

				if (sWaitCount > 260)
				{
					if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
					{
						//	for (int i = 0; i < 3; i++)
						if (CurrentSet->bPJT_GrabDisable == 0)
						{
							if (CurrentSet->nAuto_Grab_Reset == 1)
							{
								/*
								if (CurrentSet->bCheckGrabACPower == 1)
								{
									AddStringToStatus("Grabber Power OFF");
									gJigCtrl.Set_Grabber_Power(0);
									Sleep(1000);

									AddStringToStatus("Grabber Power ON");
									gJigCtrl.Set_Grabber_Power(1);
									Sleep(2000);
								}

								if (CurrentSet->bCheckIFPackReset == 1)
								{
									gJigCtrl.Set_IF_Pack_Reset(0);
									AddStringToStatus("IF Pack Reset");
									Sleep(1000);
								}
								if (CurrentSet->bCheckGenderReset == 1)
								{

									AddStringToStatus("Gender Reset");
									gJigCtrl.Set_Gender_Reset(0);
								}
								*/
								//if (CurrentSet->bCheckImageFullReset == 1)
								//{
								//	AddStringToStatus("Grabber Power OFF");
								//	gJigCtrl.Set_Grabber_Power(0);
								//	Sleep(1000);

								//	AddStringToStatus("Grabber Power ON");
								//	gJigCtrl.Set_Grabber_Power(1);
								//	Sleep(2000);
								//	gJigCtrl.Set_IF_Pack_Reset(0);
								//	AddStringToStatus("IF Pack Reset");
								////	Sleep(1000);

								//}
								Sleep(2000);
								ResetGrabStartThread();
								//if (!GrabberReset())
								//{
								//	//	if (CurrentSet->bCheckGrabACPower == 1)
								//	{
								//		AddStringToStatus("Grabber Power OFF");
								//		gJigCtrl.Set_Grabber_Power(0);
								//		Sleep(1000);

								//		AddStringToStatus("Grabber Power ON");
								//		gJigCtrl.Set_Grabber_Power(1);
								//		Sleep(2000);
								//	}
								//	Sleep(1000);
								//	if (!GrabberReset())
								//	{
								//		AddStringToStatus("Grabber Not Connected");
								//	}
								//	
								//}

								if (CurrentSet->bCheckRotateReset == 1)
								{
									Sleep(2000);

									OnImgRotate();

									Sleep(1000);
									OnImgRotate();
								}
							}
						}
					}
					else
					{
						Sleep(2000);
					}

					if (m_pVfMeasureCheckDlg.m_bActivate == TRUE)
					{
						TVCommCtrl.Send_TVControl_Command("aa 00 00", 500);
						TVCommCtrl.Send_TVControl_Command("ai 00 30", 500);
						TVCommCtrl.Send_TVControl_Command("aa 00 20", 500);
						TVCommCtrl.Send_TVControl_Command("ar 00 30", 500);
					}
									   
					gPLC_Ctrl.m_nCurrentVideoNG = 0;
					sRetryCount = 0;
					sWaitCount = 0;
					sInterProcess = 1;
					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_RUN;
					ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));



					if (m_bResultDlgActivate == 1)
					{
						m_ResultDlg->SetContinue();
					}
					else
					{
						ProcessAutokey(RETRY_BTN);
					}
					/*
					if (gPLC_Ctrl.m_nTestResult == PLC_CHECK_MODE)
					{
						m_ResultDlg->SetContinue();
					}
					else
					{
						ProcessAutokey(RETRY_BTN);
					}*/
				}
			}
		}

		break;
		case _AUTO_ROBOT_CTRL_FFC_RETRY:

			g_AutoControlProcess = _AUTO_ROBOT_CTRL_UPDOWN_RETRY;
		//	if (sInterProcess == 0)
		//	{
		//		gJigCtrl.m_bJigUpStatus = 0;
		//		sRetryCount = 0;
		//		gJigCtrl.Set_CylinderUP();
		//		ctrlPLC_COM_Status.SetWindowText(_T("RETRY:UP"));
		//		sWaitCount = 0;
		//		sInterProcess = 1;

		//		gPLC_Ctrl.m_nFirstVideoNG = 0;
		//	//	gPLC_Ctrl.m_nTestVideoCount = 0;
		//		gPLC_Ctrl.m_nCurrentVideoNG = 0;
		//	}

		//	else if (sInterProcess == 1)
		//	{
		//		sWaitCount++;
		//		if (gJigCtrl.m_bJigUpStatus)
		//		{
		//			sWaitCount = 0;
		//			sRetryCount = 0;
		//			sInterProcess = 2;
		//			gPLC_Ctrl.CommandClear();
		//		}
		//		else
		//		{
		//			if (sWaitCount > 150)
		//			{
		//				sRetryCount++;
		//				if (sRetryCount > 3)
		//				{
		//					gJigCtrl.Set_CylinderUP();
		//					g_Pause_AutoControlProcess = g_AutoControlProcess;
		//					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
		//					ctrlPLC_COM_Status.SetWindowText(_T("TMOUT:UP"));
		//				}
		//				else
		//				{
		//					gJigCtrl.Set_CylinderUP();
		//					sWaitCount = 0;
		//				}
		//			}
		//		}
		//	}
		//	else if (sInterProcess == 2)
		//	{
		//		gPLC_Ctrl.CommandFfcRetry();
		//		ctrlPLC_COM_Status.SetWindowText(_T("RETRY:FFC"));
		//		
		///*		if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
		//		{
		//			if (CurrentSet->nAuto_Grab_Reset == 1)
		//			{
		//				if (CurrentSet->bCheckGrabACPower == 1)
		//				{
		//					AddStringToStatus("Grabber Power OFF");
		//					gJigCtrl.Set_Grabber_Power(0);
		//					Sleep(1000);

		//					AddStringToStatus("Grabber Power ON");
		//					gJigCtrl.Set_Grabber_Power(1);
		//					Sleep(2000);
		//				}

		//				if (CurrentSet->bCheckIFPackReset == 1)
		//				{
		//					gJigCtrl.Set_IF_Pack_Reset(0);
		//					AddStringToStatus("IF Pack Reset");
		//					Sleep(1000);
		//				}
		//				if (CurrentSet->bCheckGenderReset == 1)
		//				{

		//					AddStringToStatus("Gender Reset");
		//					gJigCtrl.Set_Gender_Reset(0);
		//				}

		//				Sleep(1000);
		//			}
		//		}*/

		//		sWaitCount = 0;
		//		sInterProcess = 3;
		//	
		//	}
		//	else if (sInterProcess == 3)
		//	{
		//		sWaitCount++;
		//		if (gPLC_Ctrl.m_cCheckReconnectFinish)
		//		{
		//			sWaitCount = 0;
		//			sInterProcess = 4;
		//			gPLC_Ctrl.CommandClear();
		//		}
		//		else if (sWaitCount > 1500)
		//		{
		//			//sRetryCount++;

		//			g_Pause_AutoControlProcess = g_AutoControlProcess;
		//			g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
		//			ctrlPLC_COM_Status.SetWindowText(_T("TMOUT:FFC"));
		//		}
		//		else if (sWaitCount == 100)
		//		{
		//			if (gPLC_Ctrl.m_cCheckReconnect != 1)
		//			{
		//				gPLC_Ctrl.CommandFfcRetry();
		//			}
		//		}
		//	}
		//	else if (sInterProcess == 4)
		//	{
		//		sRetryCount = 0;
		//		gJigCtrl.Set_CylinderDOWN();
		//		ctrlPLC_COM_Status.SetWindowText(_T("RETRY:DN"));
		//		sWaitCount = 0;
		//		sInterProcess = 5;
		//	}
		//	else if (sInterProcess == 5)
		//	{
		//		if (gJigCtrl.m_bJigDownStatus)
		//		{
		//			sWaitCount = 0;
		//			sInterProcess = 6;
		//			gPLC_Ctrl.CommandClear();
		//			gJigCtrl.Set_ACPower(1);
		//			ctrlPLC_COM_Status.SetWindowText(_T("POWER"));
		//		}
		//		else
		//		{
		//			sWaitCount++;
		//			if (sWaitCount > 150)
		//			{
		//				sRetryCount++;
		//				if (sRetryCount > 2)
		//				{
		//					gJigCtrl.Set_CylinderDOWN();
		//					g_Pause_AutoControlProcess = g_AutoControlProcess;
		//					g_AutoControlProcess = _AUTO_ROBOT_CTRL_TIMEOUT;
		//					ctrlPLC_COM_Status.SetWindowText(_T("TMOUT:DN"));
		//				}
		//			}
		//		}
		//	}
		//	else if (sInterProcess == 6)
		//	{
		//		sWaitCount++;
		//		if (sWaitCount > 220)
		//		{
		//			if (gPLC_Ctrl.m_nCurrentVideoNG == 1)
		//			{
		//				if (CurrentSet->nAuto_Grab_Reset == 1)
		//				{
		//					if (CurrentSet->bCheckGrabACPower == 1)
		//					{
		//						AddStringToStatus("Grabber Power OFF");
		//						gJigCtrl.Set_Grabber_Power(0);
		//						Sleep(1000);

		//						AddStringToStatus("Grabber Power ON");
		//						gJigCtrl.Set_Grabber_Power(1);
		//						Sleep(2000);
		//					}

		//					if (CurrentSet->bCheckIFPackReset == 1)
		//					{
		//						gJigCtrl.Set_IF_Pack_Reset(0);
		//						AddStringToStatus("IF Pack Reset");
		//						Sleep(1000);
		//					}
		//					if (CurrentSet->bCheckGenderReset == 1)
		//					{

		//						AddStringToStatus("Gender Reset");
		//						gJigCtrl.Set_Gender_Reset(0);
		//					}

		//					Sleep(1000);
		//					ResetGrabStartThread();
		//					//if (!GrabberReset())
		//					//{
		//					//	//	if (CurrentSet->bCheckGrabACPower == 1)
		//					//	{
		//					//		AddStringToStatus("Grabber Power OFF");
		//					//		gJigCtrl.Set_Grabber_Power(0);
		//					//		Sleep(1000);

		//					//		AddStringToStatus("Grabber Power ON");
		//					//		gJigCtrl.Set_Grabber_Power(1);
		//					//		Sleep(2000);
		//					//	}
		//					//	Sleep(1000);
		//					//	if (!GrabberReset())
		//					//	{
		//					//		AddStringToStatus("Grabber Not Connected");
		//					//	}
		//					//}

		//					if (CurrentSet->bCheckRotateReset == 1)
		//					{
		//						Sleep(2000);

		//						OnImgRotate();

		//						Sleep(1000);
		//						OnImgRotate();
		//					}
		//				}
		//			}

		//			gPLC_Ctrl.m_nCurrentVideoNG = 0;
		//			sRetryCount = 0;

		//			sWaitCount = 0;
		//			sInterProcess = 1;
		//			g_AutoControlProcess = _AUTO_ROBOT_CTRL_TEST_RUN;
		//			ctrlPLC_COM_Status.SetWindowText(_T("TEST RUN"));

		//			if (m_bResultDlgActivate == 1)
		//			{
		//				m_ResultDlg->SetContinue();
		//			}
		//			else
		//			{
		//				ProcessAutokey(RETRY_BTN);
		//			}
		//	/*		if (gPLC_Ctrl.m_nTestResult == PLC_CHECK_MODE)
		//			{
		//				m_ResultDlg->SetContinue();
		//			}
		//			else
		//			{
		//				ProcessAutokey(RETRY_BTN);
		//			}*/
		//		}
		//	}

			break;
	}

	return 0;
}

BEGIN_EVENTSINK_MAP(CDATsysView, CFormView)
    //{{AFX_EVENTSINK_MAP(CDATsysView)
	ON_EVENT(CDATsysView, IDC_MAINVIEW_GRID, -600 /* Click */, OnClickMainViewGrid, VTS_NONE)
	ON_EVENT(CDATsysView, IDC_OPTION_GRID, -600 /* Click */, OnClickOptionGrid, VTS_NONE)
	ON_EVENT(CDATsysView, IDC_COUNTRY_GRID, -600 /* Click */, OnClickCountryGrid, VTS_NONE)
	ON_EVENT(CDATsysView, IDC_VERSION_GRID, -600 /* Click */, OnClickVersionGrid, VTS_NONE)
	
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

	//+add 090131(¼öÁ¤Ç×¸ñ1)
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

	DWORD dwExitCode = 0;
	if(m_pGrabImageThread) TerminateThread(m_pGrabImageThread->m_hThread,dwExitCode);
	if(m_pTestThread)	   TerminateThread(m_pTestThread->m_hThread,dwExitCode);

#ifdef _THREAD_DEBUG
	ListProcessThreads(GetCurrentProcessId());
#endif

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

	//+add 090131(¼öÁ¤Ç×¸ñ1)
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
	if(g_nGrabberType == UHD_GRABBER)
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

		StretchDIBits(pDC->GetSafeHdc(),
				  1,1,rect.Width()-2,rect.Height()-2
				  ,0,0,ImageData.m_BmInfo->bmiHeader.biWidth, ImageData.m_BmInfo->bmiHeader.biHeight
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
	else{
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
	if (m_bResultDlgActivate == 1)
	{
		m_bResultDlgActivate = FALSE;
		m_ResultDlg->DestroyWindow();
		m_ResultDlg = NULL;
	}
	return 0;
}

LRESULT CDATsysView::ShowResultDisplay(WPARAM wParam, LPARAM lParam)
{
	int nResult = (int)wParam;
	int nLeft=0, nTop=0, nWidth=0, nHeight=0;
	CRect rect;

	m_ResultDlg = new CResultDlg;
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(2000);
//	Sleep(1000);
//#endif
	if(nResult == 0)
	{
		
		m_ResultDlg->SetImage(IDB_FAIL2_BMP);
		m_ResultDlg->SetTestResultInfo(m_szResultMsg1,m_szResultMsg2);

		gPLC_Ctrl.m_nTestResult = PLC_NG_MODE;
		//PLC_OK_MODE
						//PLC_NG_MODE
						//PLC_CHECK_MODE
	} 
	else if(nResult == 1)
	{
		m_ResultDlg->SetImage(IDB_PASS2_BMP);
		m_ResultDlg->SetTestResultInfo(m_szResultMsg1,m_szResultMsg2);
		gPLC_Ctrl.m_nTestResult = PLC_OK_MODE;
	}
	else 
	{
		m_ResultDlg->SetImage(IDB_CHECK_BMP);
		m_ResultDlg->SetTestResultInfo(m_szResultMsg1,m_szResultMsg2);
		gPLC_Ctrl.m_nTestResult = PLC_CHECK_MODE;
	}

//#ifdef	DEBUG_MD5_CODE__
//	Sleep(2000);
//	Sleep(1000);
//#endif

	m_ResultDlg->Create(IDD_RESULT_DLG, this);

	m_ResultDlg->GetWindowRect(&rect);

	//nLeft = m_FormViewRect.left + (m_FormViewRect.Width() / 2) - (rect.Width() / 2) - 30;
	//nTop = (m_FormViewRect.Height() / 2) + 210; // - (rect.Height()/2);
	nLeft = m_FormViewRect.left +  50;
	nTop = (m_FormViewRect.Height() / 2) + 200; // - (rect.Height()/2);
	nWidth  = rect.Width();
	nHeight = rect.Height();
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(2000);
//	Sleep(1000);
//#endif	
	m_ResultDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
	m_ResultDlg->ShowWindow(SW_SHOW);
	m_ResultDlg->UpdateWindow();
	m_bResultDlgActivate = TRUE;
//#ifdef	DEBUG_MD5_CODE__
//	Sleep(2000);
//	Sleep(1000);
//#endif
	return 0;
}

//+add kwmoon 070406
void CDATsysView::OnOption() 
{
	BOOL bFlag;

	bFlag = g_SoundCard.wIn_Flag;
	OnAudioMeasStop();

	int lOldArcNumber = CurrentSet->nARC_Force_Port_Number;

	COptionDlg dlg("Option", NULL, 0);

	dlg.DoModal();


//	if (lOldArcNumber != CurrentSet->nARC_Force_Port_Number)
	{
		PostMessage(WM_COMMAND, ID_EDIT_COMPILE);
	}


	//+add PSH 080527
	if(CurrentSet->bUseScanner)
	{
		ctrlWipIdEdit.SetReadOnly(TRUE);
	}
	else
	{
		ctrlWipIdEdit.SetReadOnly(FALSE);
	}

	if(CurrentSet->nToolOptionSetMethod == 1)
	{
//godtech 190602
		if (g_nSysRobotType == AUTO_SYS)
		{
			GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);

		}
		else
		{
		//	if (CurrentSet->bUseScanner) {
				GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);
			//}
			//else {
			//	GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
			//}
		}
	}
	else{
		GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
	}

	OnInputLock();

//	if(bFlag) SetTimer(2, 1000, NULL);

	if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == 1))
	{
		ctrlPLC_COM_Status.ShowWindow(1);
		ctrlROBOT_Status.ShowWindow(1);
		m_cBtnResetRobot.ShowWindow(1);
		m_cBtnStopRobot.ShowWindow(1);
	}
	else
	{
		ctrlPLC_COM_Status.ShowWindow(0);
		ctrlROBOT_Status.ShowWindow(0);
		m_cBtnResetRobot.ShowWindow(0);
		m_cBtnStopRobot.ShowWindow(0);
	}

	if (CurrentSet->bCheckBatteryCheck == 1)
	{
		CtrlBatVerEdit.SetWindowText(CurrentSet->sBatVersion);
		CtrlBatVerEdit.ShowWindow(1);
		GetDlgItem(IDC_BUTTON_BAT_VER)->ShowWindow(1);
	}
	else
	{
		CtrlBatVerEdit.ShowWindow(0);
		GetDlgItem(IDC_BUTTON_BAT_VER)->ShowWindow(0);
	}

	//CurrentSet->sEpiCfgIni = m_szExePath + "\\EPI_Config.ini";
	if (OpenEPI_ConfigFile(CurrentSet->sEpiCfgIni))
	{
		//CurrentSet->sDipStatus
	}
	else
	{
		CurrentSet->sDipStatus = "";
	}
	m_CEdit_EPI_CFG_SW.SetWindowText(CurrentSet->sDipStatus);
}

void CDATsysView::OnImageDialog() 
{
	BOOL bFlag;


	if(CurrentSet->bIsRunning)
	{
		AfxMessageBox("First, Stop running Program"); return;
	}
	bFlag = g_SoundCard.wIn_Flag;
	OnAudioMeasStop();

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		StopLVDSGrabThread();
	}//
	else//FHD
	{
		StopLVDSGrabThread_FHD();
	}

	CImageTotalDlg dlg;
	dlg.m_nTabType = 0;	// 0 : P_GRAB

//	if(dlg.DoModal() != IDOK) return;
	dlg.DoModal();

//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		StartLVDSGrabThread();
	}//
	else
	{
		StartLVDSGrabThread_FHD();
	}

//	if(bFlag) SetTimer(2, 1000, NULL);
}

void CDATsysView::OnSeqEdit() 
{


	CSeqEditorDlg dlg;
	if(dlg.DoModal())
	{
		if(!CurrentSet->sSeqPath.IsEmpty())
		{
			Prescan(CurrentSet->sSeqPath);
			InsertStepData2Grid(CurrentSet->nDisplayType);
		}		
	}
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
	nRunStartType   = AUTO_START;
	nDisplayType	= 0;				// Current Display type	0 : Main Grid -> Normal Grid, 1 : Main Grid -> Detailed Grid
	nPrintType		= 0;				// Current Print Out type
	nStepKey		= 0;				// StepKey
    lTime			= 0;       			// Last Compile Time

	bIsRunning		= FALSE;			// Running state
	bRunAbort		= FALSE;			// User abort key
    bSystemBreak	= FALSE;            // System break
	bCompiled		= FALSE;			// Parsing result
	bCommDisplay	= TRUE;			// rs232 Åë½Å³»¿ë Display
	bIsRunMsg		= FALSE;			// thread ¿¡¼­ ½ÇÇà ¸Þ¼¼Áö¸¦ º¸³Â´ÂÁö ¿©ºÎ
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
	sSeqPath_EM		= _T("");	
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

	sVFTestIni		= _T("");

	//+add kwmoon 080716
	sDetailResultDataPath = _T("");

	//+add kwmoon 081024
	sModelListPath		  = _T("");	

	nLvdsWidth		= 0;				// Uranous Grabber Width
	nLvdsHeight		= 0;				// Uranous Grabber Height
	nAnalogWidth	= 0;				// Matrox Analog Grabber(Cronos+) Width
	nAnalogHeight	= 0;				// Matrox Analog Grabber(Cronos+) Height

	sHDMIRoiShape		= _T("");
	//------------------
	// COM Port
	//------------------
	sPatternComPort		= _T("");
	sTVComPort			= _T("");
	sAvSwitchBoxComPort	= _T("");
	sScannerComPort		= _T("");
	sI2cAdcComPort		= _T("");
	sLNBComPort			= _T("");
	sJigComPort			= _T("");

	wPatternBaudRate		= 0;
	//+change kwmoon 080804
//	wTVBaudRate				= 0;
	wTVBaudRate				= 115200;
	wAvSwitchBoxBaudRate	= 0;
	wScannerBaudRate		= 0;
	wI2cAdcBaudRate			= 0;
	wLNBBaudRate			= 0;
	wJigBaudRate			= 0;

	bUsePatternGen			= FALSE;
	bUseTVCommPort			= FALSE;
	bUseAVSwitchBox			= FALSE;
	bUseScanner				= FALSE;
	bUseLNB					= FALSE;
	bUseJig					= FALSE;

	nAdcType				= ADC_TYPE_RS232C; 
	nAudioSource			= 0;					// Audio Input Source 0: LineIn, 1: DigitalIn
	nLVDS_Mode				= 0;
//FHD
	nUranousMode			= 0;
//	
	nBitCount				= 0;
//	nTotalNoTest			= 0;
//	nTotalNoNg				= 0;

	nNoRetry = 0;
	bTVCommPortEcho			= FALSE;
	
	//------------------
	// MES Data
	//------------------
	sLineCode			= _T("");		//Location 0 : LG-PRI, 1 : LG-DD, 2 : JINSUNG, 3 : SG TECH
	nLineNo				= 1;		//Line Number 1 ~ 10
	nLineID_GLZ			= 0;
	nLineID_GPZ			= 0;
	nLineID_GMZ			= 0;
	nSystemNo			= 1;		//System Number 1 ~ 10
	nAuthority			= 0;		//Authority 0 : Developer, 1 : Administrator, 2 : Operator
	strPassword			= _T("");	// Developer : Password "5555" , Administrator : Password "DFT"
									// Operator : No Password
	strUserId			= _T("");

	bUseGmes			= TRUE;
	bUsePLCRobot		= FALSE;
	bAutoUploadMes		= FALSE;
	bUploadMes			= FALSE;
	bGMES_Connection	= FALSE;
	bPLC_Connection	= FALSE;

	sToolOption1		= _T("");
	sToolOption2		= _T("");
	sToolOption3		= _T("");
	sToolOption4		= _T("");
	sToolOption5		= _T("");
	sToolOption6		= _T("");
	sToolOption7		= _T("");
	sToolOption8		= _T("");
	sToolCRC			= _T("");
	bToolOptionWriteFlag = FALSE;


	sAreaOption1		= _T("");

	sCountryGroup		= _T("");
	sCountry		= _T("");

	sCommercialOption1	= _T("");

	sCPUVersion		= _T("");
	sMicomVersion		= _T("");
	sUSBVersion		= _T("");
	sMNTVersion		= _T("");

	nToolOptionCheck_Delay = 0;

	sEDID_HDMI1		= _T("");
	sEDID_HDMI2		= _T("");
	sEDID_HDMI3		= _T("");
	sEDID_HDMI4		= _T("");
	sEDID_HDMI5		= _T("");
	sEDID_DP		= _T("");
	sEDID_DP2		= _T("");
	sEDID_DVI		= _T("");
	sEDID_PC		= _T("");
	sEDID_RGB		= _T("");

	sCI			= _T("");
	sCNTV			= _T("");
	sDTCP_RX		= _T("");
	sDVBS			= _T("");
	sDVBS2			= _T("");
	sESN			= _T("");
	sHDCP_RX		= _T("");
	sHDCP_RX_14		= _T("");
	sHDCP_TX		= _T("");
	sJAPAN_LNB		= _T("");
	sMAC_ADDRESS		= _T("");
	sMARINKEY		= _T("");
	sMODEL_NAME		= _T("");
	sSERIAL_NO		= _T("");
	sWASU			= _T("");
	sWIDEVINE		= _T("");
	sADC			= _T("");
	sBUILT_WIFI		= _T("");
	sCA				= _T("");

	sASTC30_Key = _T("");
	sMfi_Key	= _T("");
	sCI_CEP_Key = _T("");

	bDrmKey			= TRUE;
	nDrmKeyVal		= 0;
//+change kwmoon 080925
//	nAudioCheckMethod	= 0;
	nAudioCheckMethod	= 1;
	nFullHd120Hz		= 0; // 0 :
// add 091214
	nImageRotation		= 0;
	// 090120 External DFT
//	nLvdsGrabberType	= INTERNAL_GRABBER; 

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
	
	
	//------------------
	// I2C Setting 
	//------------------
	bUseI2cAdc				= 0;
	nI2cClkSpeed			= 1; // 42Khz
	nI2cPacketDelay			= 150;
	nI2cReadByteDelay		= 1;
	nI2cWriteByteDelay		= 1;

	//+add PSH 100310
	nHDMIGenType = 0;
	nHDMIResolution = 0;
	nHDMIBitSpec = 0;
	nCompResolution = 0;

	nHDMI1_Model = 0;
	nHDMI2_Model = 0;
	nHDMI3_Model = 2;
	nHDMI4_Model = 2;

	//add 101029
	n3DGrabbingMode = 0;

	nCountryGroup  = 0x00;

	bHdmiOutControl = 0;
	nHDMI1_Port = 1;
	nHDMI2_Port = 2;
	nHDMI3_Port = 3;
	nHDMI4_Port = 4;

	bHdmiCecControl = FALSE;
	bHdmiEdidControl = TRUE;
	bHdmiHdcpControl = TRUE;

	nHDCP_Start_ADH		= 0xAC;
	nHDCP_Start_ADL		= 0x80;
	nHDCP_Data_Size		= 320;
	nHDCP_Key_Class		= 0;
	sHDCP_Key_Class		= "MEDIATEK";
	sHdcpKeyFileName.Empty();

	sOrganizationID	= _T("");
	sLineID			= _T("");
	sEquipmentID	= _T("");
	sOperationID	= _T("");
	sModelID		= _T("");
	sModelName_GMES	= _T("");
	sModelSuffix	= _T("");
	nDftSeq			= 0;
	nDftNgCount		= 0;
	nDftOkCount		= 0;
	nToolOptionSetMethod = 0;
	//UHD
	nUHD_Type = 0;
	nUHD_Grab_BitShift = 0;
	nUHD_Grab_Mode = 0;
	nUHD_Grab_Delay = 1;	
	nUHD_Y20_SW_Mode = 1; 

	nGrabSource = LVDS_GRABBER;
	
	nHDMI_Grab_Resolution = 0;
	nHdmiWidth = 1024;					// Hdmi Grab Width
	nHdmiHeight = 768;				// Hdmi Grab Height
	nHDMI_Grab_BitShift = 0;
	nHDMI_Grab_Input = 0;
	bHdmiGrab_MaskOsd = FALSE;

	nDmmMeasureCh_S = 0;
	nDmmMeasureCh_E = 0;
	//
	bNgCountCheck = FALSE;
	bCheckGrabACPower = FALSE;
	bCheckIFPackReset = FALSE;
	bCheckGenderReset = FALSE;
	bCheckRotateReset = FALSE;
	bCheckBatteryCheck  = FALSE;
	sJACK_AV1	= _T("");
	sJACK_AV2	= _T("");
	sJACK_SCART	= _T("");
	sJACK_COMP1	= _T("");
	sJACK_COMP2	= _T("");
	sJACK_USB1	= _T("");
	sJACK_USB2	= _T("");
	sJACK_USB3	= _T("");
	sJACK_USB4	= _T("");
	sJACK_HEAD_PHONE= _T("");
	sJACK_HDMI1	= _T("");
	sJACK_HDMI2	= _T("");
	sJACK_HDMI3	= _T("");
	sJACK_HDMI4	= _T("");
	sJACK_RGB	= _T("");
	sJACK_DVI	= _T("");
	sJACK_DP	= _T("");
	sJACK_SMHL	= _T("");

	nDrmKeyResultVal = 0;
	nInputCheckResultVal = 0;
	nAcPowerOffType =0;
//	nSystemType = UHD_SYS;
	bTestTimeCount = FALSE;
	bAutoRun_DFT2 = FALSE;
	bPJT_GrabDisable = FALSE;
	sData_MD5 = _T("");

	sBatVersion = _T("31");
}

void CDATsysView::OnClickOptionGrid()
{
	//UpdateToolOptionInfo(0);
#ifdef DEBUG_MD5_CODE__

#endif
}
void CDATsysView::OnClickCountryGrid()
{
#ifdef DEBUG_MD5_CODE__

#endif
}

void CDATsysView::OnClickVersionGrid()
{
}

void CDATsysView::OnClickMainViewGrid() 
{
#if 0
	if(CurrentSet->bIsRunning) return;

 	int nCol = m_ctrlSummaryGrid.GetCol();
	int nRow = m_ctrlSummaryGrid.GetRow();
	if(nCol > m_ctrlSummaryGrid.GetCols()) return;
	if(nRow > m_ctrlSummaryGrid.GetRows()) return;

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
			//if (CurrentSet->bGMES_Connection && CurrentSet->bUseScanner && CurrentSet->bUploadMes) {
			
			if (CurrentSet->bGMES_Connection  && CurrentSet->bUploadMes) {
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
			if(CurrentSet->bGMES_Connection && CurrentSet->bUseScanner && CurrentSet->bUploadMes){
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
					AfxMessageBox("First, Stop LVDS Image Grabbing"); return;
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
	if (m_hAccel != NULL)
    {
		if(TranslateAccelerator(this->m_hWnd, m_hAccel, pMsg))
			return TRUE;
    }
//	if (pMsg->message == WM_KEYDOWN)//m_sModelSuffix
//	{
//	
//		if (pMsg->wParam == VK_RETURN)
//		{
//#ifdef			DEBUG_MD5_CODE__
//			if (m_SelectChassisModelDlg.m_bActivate == 1)
//			{
//				CurrentSet->sModelSuffixName = "A123.459";
//				SendMessage(UM_UPDATE_SUFFIX_SCAN, 0, 0);
//			}
//#endif
//			
//			return TRUE;
//		}
//	
//	}
//
	//if (pMsg->message == WM_KEYDOWN)//m_sModelSuffix
	//{
	//	ProcessKeyPad(pMsg->wParam, pMsg->lParam);
	//}
	//if (pMsg->message == WM_KEYDOWN)//m_sModelSuffix
	//{
	//	//if (pMsg->wParam == VK_ESCAPE)
	//	//	return TRUE;
	//	//else if (pMsg->wParam == VK_RETURN)
	//	//{
	//	//	//ClickCbtnStart();
	//	//	//if (m_sTempModelSuffix.Find(".") > 0)
	//	//	{
	//	//		if (CurrentSet->bUseScanner)
	//	//		{
	//	//			//CurrentSet->sModelSuffixName = m_sTempModelSuffix;
	//	//			//m_strWipId = m_sTempModelSuffix;
	//	//		//	ctrlWipIdEdit.SetWindowText(m_sTempModelSuffix);
	//	//		}
	//	//		//SendMessage(UM_UPDATE_TOOL_OPTION, 0, 0);
	//	//	}
	//	//	m_sTempModelSuffix = _T("");
	//	//	return TRUE;
	//	//}
	//	_Wait(100);
	//}
	//else if (pMsg->message == WM_CHAR)
	//{
	//	//m_sTempModelSuffix = m_sTempModelSuffix + (char)(pMsg->wParam);
	//	////ctrlWipIdEdit.SetWindowText(m_sTempModelSuffix);
	//	////m_sModelSuffix.MakeUpper();
	//	////m_BarCode.put_Caption(m_BarcodeString);
	//	//return TRUE;
	//	_Wait(100);
	//}
	return CFormView::PreTranslateMessage(pMsg);
}
void CDATsysView::OnSelchangeViewTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nViewNo = m_ctrlViewTab.GetCurSel();
	
	switch(nViewNo)
	{

		case _MAIN_VIEW_GRAB_IMAGE:	// Grab
		{
			m_nCurrentViewImageType = SNAP_IMAGE;
#ifdef SM_MODIFY_CODE__
			if (g_GrabSource == LVDS_GRABBER)
			{
				break;
			}
			else
			{
				g_GrabSource = LVDS_GRABBER;
				_Grabber_Reset();
			}

#endif
		}
		break;

		case _MAIN_VIEW_LOAD_IMAGE:	// Reference
		{
			m_nCurrentViewImageType = LOAD_IMAGE;
		}
		break;

		case _MAIN_VIEW_PROC_IMAGE:	// Processed
		{
			m_nCurrentViewImageType = PROC_IMAGE;
		}
		break;
#ifdef SM_MODIFY_CODE__
		case _MAIN_VIEW_HDMI_IMAGE:	// Processed
		{
			m_nCurrentViewImageType = SNAP_IMAGE;

			if (g_nGrabberType == FHD_GRABBER)
			{ 
				break;
			}
			
			

			if (gAvConvertorCtrl.m_bPortOpen == FALSE)
			{
				break;
			}


			g_GrabSource = HDMI_GRABBER;
			CurrentSet->nHDMI_Grab_Resolution = CurrentSet ->nHDMIResolution ;
		//	CurrentSet->nHDMI_Grab_BitShift = nBitShift;

			switch (CurrentSet->nHDMI_Grab_Resolution)
			{
			case 0:  CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
			case 1:  CurrentSet->nHdmiWidth = 1366; CurrentSet->nHdmiHeight = 768; break;
			case 2:  CurrentSet->nHdmiWidth = 1280; CurrentSet->nHdmiHeight = 1024; break;
			case 3:  CurrentSet->nHdmiWidth = 1920; CurrentSet->nHdmiHeight = 1080; break;
			case 4:  CurrentSet->nHdmiWidth = 1280; CurrentSet->nHdmiHeight = 540; break;
			default: CurrentSet->nHdmiWidth = 1024; CurrentSet->nHdmiHeight = 768; break;
			}
			_Grabber_Reset();

		}
		break;
#endif

		

		default:
			break;
	}

	InvalidateRect(&m_VideoViewClientRect);

	*pResult = 0;
}


//+add kwmoon 080527
LRESULT CDATsysView::OnUploadMesData(WPARAM wParam, LPARAM lParam)
{
	BOOL bPass = (BOOL) wParam;
	int nResult = TRUE;
	CString sCurentTime;

	// change PSH 090507
	if (m_strWipId.Find("NO_SCAN") >= 0)
	{
		return 0;
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
		AddStringToStatus("GMES ERROR : Test Result Upload");
	//	AfxMessageBox("GMES ERROR : Test Result Upload");
		MesData_LocalSaveText(sCurentTime, m_strWipId, bPass, sTime);
	}

	if(MesData_AutoUpLoad_DataCheck())
	{
		ShowSystemMsg(MES_DATA_AUTOUPLOAD_MSG);
		MesData_AutoUpLoad();
		CloseSystemMsg();
	}
	return 0;
}

LRESULT CDATsysView::RunContinuousLVDSGrab_FHD(WPARAM wParam, LPARAM lParam)
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

//FHD
void CDATsysView::ContinuousLVDSGrab_FHD()
{ 
	CString szErrorMsg = _T("");
	CWnd* pImageWnd    = g_pView->GetDlgItem(IDC_VIDEOVIEW);

	CRect nImageRect;
	pImageWnd->GetWindowRect(&nImageRect);

	int nGrabResult = GRAB_FAIL;
	int nWidth		= CurrentSet->nLvdsWidth;
	int nHeight		= CurrentSet->nLvdsHeight;
	int nBitCount	= CurrentSet->nBitCount;
	int nRowWidth	= WIDTHBYTES(nBitCount*nWidth);
	int nImageSize  = nRowWidth * nHeight;

	g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);

	if((nGrabResult = Grab_FHD(GRAB_WITH_RETRY)) != GRAB_PASS)
	{
		szErrorMsg = "No LVDS input image!"; 
	}

	g_GrabImage.m_bImageLoaded = TRUE;
	
	InvalidateRect(nImageRect, FALSE); UpdateWindow();

	ctrlImgProg.SetPos(100);
}

BOOL CDATsysView::Grab_UHD()
{


	BOOL bResult  = FALSE;
	DWORD dwEvent = 1000;

	bResult = ::SetEvent(hGrabEvent);

	do
	{
		dwEvent =  WaitForSingleObject(hGrabEvent_Complete,2000);			

		if(dwEvent == WAIT_OBJECT_0) 
		{
			bResult = TRUE; 
			dwEvent = -1; 

			::ResetEvent(hGrabEvent_Complete);
		}
		else
		{
			bResult = FALSE; 
			dwEvent = -1; 
			::ResetEvent(hGrabEvent_Complete);
		}
	}while(dwEvent != -1);

	return bResult;
}

int CDATsysView::Grab_FHD(int nType,int nGrabMode)
{
	int nNoGrabRetry = MAX_NO_GRAB_RETRY;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


GRAB_START :
	
	EnterCriticalSection(&g_cs3);

	BOOL bResult  = FALSE;
	DWORD dwEvent = 1000;

	if(g_nRunningProcessNo == 1)
	{
		// Ignores response for previous event
		if(nGrabMode == EXT_LVDS_GRABBER) {
			if(m_Grab1RunningFlag) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB1_COMPLETE],1000);			
			}

			//==================
			// Grab LVDS Image1
			//==================
			bResult = ::SetEvent(hClientEvents[EVENT_GRAB1_IMG]);
		//	if(bResult) {
		//		AfxMessageBox("EVENT_GRAB1_IMG:OK"); 
		//	}

		}
		else { // (nGrabMode == EXT_ANALOG_GRABBER)
			if(m_Grab1RunningFlag) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB4_COMPLETE],1000);			
			}
			//====================
			// Grab Analog Image1
			//====================
			::SetEvent(hClientEvents[EVENT_GRAB4_IMG]);
		}
		m_Grab1RunningFlag = TRUE;
	}
	else if(g_nRunningProcessNo == 2) {
		if(nGrabMode == EXT_LVDS_GRABBER) {
			if(m_Grab2RunningFlag){
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB2_COMPLETE],1000);			
			}
			//==================
			// Grab LVDS Image2
			//==================
			::SetEvent(hClientEvents[EVENT_GRAB2_IMG]);
		}
		else {
			if(m_Grab2RunningFlag) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB5_COMPLETE],1000);			
			}
			//====================
			// Grab Analog Image2
			//====================
			::SetEvent(hClientEvents[EVENT_GRAB5_IMG]);
		}
		m_Grab2RunningFlag = TRUE;
	}
	else {
		if(nGrabMode == EXT_LVDS_GRABBER) {
			if(m_Grab3RunningFlag) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB3_COMPLETE],1000);			
			}
			//==================
			// Grab LVDS Image2
			//==================
			::SetEvent(hClientEvents[EVENT_GRAB3_IMG]);
		}
		else {
			if(m_Grab3RunningFlag) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB6_COMPLETE],1000);			
			}
			//====================
			// Grab Analog Image2
			//====================
			::SetEvent(hClientEvents[EVENT_GRAB6_IMG]);
		}
		m_Grab3RunningFlag = TRUE;
	}

	do
	{
		//===========================
		// Wait LVDS Image1 Response
		//===========================
		if(g_nRunningProcessNo == 1) {
			if(nGrabMode == EXT_LVDS_GRABBER) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB1_COMPLETE],2000);
				
				if(dwEvent == WAIT_OBJECT_0) {
					if(m_pImageInfo->nGrabSucceed == 1) {
						bResult = TRUE; 
					}
					else {
						bResult = FALSE; 
						memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
					}
					dwEvent = -1; 

					::ResetEvent(hServerEvents[EVENT_GRAB1_COMPLETE]);

					m_Grab1RunningFlag = FALSE;
				}
				else {
					bResult = FALSE; dwEvent = -1; 
				}
			}
			else {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB4_COMPLETE],2000);			
	
				if(dwEvent == WAIT_OBJECT_0) {
					if(m_pImageInfo->nGrabSucceed == 1) {
						bResult = TRUE; 
					}
					else {
						bResult = FALSE; 
						memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
					}
					dwEvent = -1; 

					::ResetEvent(hServerEvents[EVENT_GRAB4_COMPLETE]);

					m_Grab1RunningFlag = FALSE;
				}
				else {
					bResult = FALSE; dwEvent = -1; 
				}
			}
		}
		//===========================
		// Wait LVDS Image2 Response
		//===========================
		else if(g_nRunningProcessNo == 2) {
			if(nGrabMode == EXT_LVDS_GRABBER) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB2_COMPLETE],2000);			
		
				if(dwEvent ==  WAIT_OBJECT_0) {
					if(m_pImageInfo->nGrabSucceed == 1) {
						bResult = TRUE; 
					}
					else {
						bResult = FALSE; 
						memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
					}
					dwEvent = -1; 

					::ResetEvent(hServerEvents[EVENT_GRAB2_COMPLETE]);
					
					m_Grab2RunningFlag = FALSE;
				}
				else {
					bResult = FALSE; dwEvent = -1; 
				}
			}
			else {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB5_COMPLETE],2000);			
		
				if(dwEvent ==  WAIT_OBJECT_0) {
					if(m_pImageInfo->nGrabSucceed == 1) {
						bResult = TRUE; 
					}
					else {
						bResult = FALSE; 
						memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
					}
					dwEvent = -1; 

					::ResetEvent(hServerEvents[EVENT_GRAB5_COMPLETE]);
					
					m_Grab2RunningFlag = FALSE;
				}
				else {
					bResult = FALSE; dwEvent = -1; 
				}
			}
		}
		//===========================
		// Wait LVDS Image3 Response
		//===========================
		else {
			if(nGrabMode == EXT_LVDS_GRABBER) {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB3_COMPLETE],2000);			
		
				if(dwEvent ==  WAIT_OBJECT_0) {
					if(m_pImageInfo->nGrabSucceed == 1) {
						bResult = TRUE; 
					}
					else {
						bResult = FALSE; 
						memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
					}
					dwEvent = -1; 

					::ResetEvent(hServerEvents[EVENT_GRAB3_COMPLETE]);
					
					m_Grab3RunningFlag = FALSE;
				}
				else {
					bResult = FALSE; dwEvent = -1; 
				}
			}
			else {
				dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRAB6_COMPLETE],2000);			
		
				if(dwEvent ==  WAIT_OBJECT_0) {
					if(m_pImageInfo->nGrabSucceed == 1) {
						bResult = TRUE; 
					}
					else {
						bResult = FALSE; 
						memset(g_GrabImage.m_pImageData,0xff,g_GrabImage.m_BmInfo->bmiHeader.biSizeImage);
					}
					dwEvent = -1; 

					::ResetEvent(hServerEvents[EVENT_GRAB6_COMPLETE]);
					
					m_Grab3RunningFlag = FALSE;
				}
				else {
					bResult = FALSE; dwEvent = -1; 
				}
			}
		}
	}while(dwEvent != -1);

	LeaveCriticalSection(&g_cs3);

	if(!bResult)
	{
		if(CurrentSet->bRunAbort){ return GRAB_ABORT; }

		if(nType == GRAB_WITH_NO_RETRY) return GRAB_FAIL;

		if(nNoGrabRetry-- > 0)
		{ 
			_Wait(10); goto GRAB_START;
		}
		else return GRAB_FAIL;
	}
	return GRAB_PASS;
}
//
long CDATsysView::OnCommunication(WPARAM wParam, LPARAM lParam)
{
	static CString	buf1 = _T(""); //, reply1 = _T("");
	static CString	buf2 = _T(""); //, reply2 = _T("");
	static CString	buf3 = _T(""), reply3 = _T("");
	int idx1;
	CString strReadData;
	CString strReadData2;
	CString strWId;
	int nSize;
	int nTemp;
	//AUTO
	int nSize2;
	int nJigStatus;
	//
	BYTE aByte; 
	CString sTmp;
	//CString strTmp;
	BYTE	strReceive[2048];
	static int sOldStart = 0;
	static int sOldStop = 0;
	static int sOldRetry = 0;
	static int sOldOK = 0;
	int lStartBTN = 0;
	 int lStopBTN = 0;
	 int lRetryBTN = 0;
	 int lOKBTN = 0;

	if((int)wParam == ScannerCtrl.m_nPortID)
	{
		//if((g_nSysType == AUTO_SYS)&& (g_nSysRobotType != AUTO_SYS)) {
		//	ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
		//	return 1;
		//}

		nSize=  (ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead).GetSize(); 

		memset(strReceive, 0, 2048);

		for(int i = 0 ; i < nSize; i++)
		{
	  		(ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead).GetByte(&aByte);  //
			strReceive[i] = aByte;
		}	
		buf2.Format("%s",strReceive);
		buf1 += buf2;
	
		if (g_nUseNoScanType == TRUE)//ScannerCtrl
		{

	

			nSize = buf1.GetLength();

			strWId = _T("");

			int lpos = buf1.Find("@");
			if (lpos >= 0)
			{
//				if (CurrentSet->bUseStartBox)
				{
					nSize = buf1.GetLength();

					if ((nSize >= 10) && (buf1.Find("@") != -1) && (buf1.Find("$") != -1))
					{
						//buf1[nSize] = 0;
						gStrTmpLog.Format("ScannerCtrl.m_nPortID: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
						SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

						int lpos = buf1.Find("@RI");
						if (lpos >= 0)
							sTmp = buf1.Mid(lpos);
						else
							sTmp = "";
						if (sTmp.GetLength() >= 10)
						{
							if (sTmp.GetAt(3) == '1')
							{
								if (sOldStart == 0)
									lStartBTN = 1;
								sOldStart = 1;
							}
							else
							{
								sOldStart = 0;
							}
							if (sTmp.GetAt(4) == '1')
							{
								if (sOldStop == 0)
									lStopBTN = 1;
								sOldStop = 1;
							}
							else
							{
								sOldStop = 0;
							}

							if (sTmp.GetAt(5) == '1')
							{
								if (sOldRetry == 0)
									lRetryBTN = 1;
								sOldRetry = 1;
							}
							else
							{
								sOldRetry = 0;
							}

							if (sTmp.GetAt(6) == 'O')
							{
							}
							else if (sTmp.GetAt(7) == 'O')
							{
								if (sTmp.GetAt(6) == '1')
								{
									if (sOldOK == 0)
										lOKBTN = 1;
									sOldOK = 1;
								}
								else
								{
									sOldOK = 0;
								}
							}


							if (lStartBTN == 1)
							{

								ProcessStartBox(START_BTN1);
							}
							else if (lStopBTN == 1)
							{
								ProcessStartBox(STOP_BTN1);
							}
							else if (lRetryBTN == 1)
							{
								ProcessStartBox(RETRY_BTN1);
							}
							else if (lOKBTN == 1)
							{
								ProcessStartBox(OK_BTN1);
							}

						}
						buf1.Empty();
						ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					}
					else
					{
						if ((nSize >= 1) && (buf1.Find("@") == -1))
						{
							buf1.Empty();
							ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
						}
					}
				}
			//	ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
				return 1;
			}
			else if (CurrentSet->bIsRunning)
			{
				buf1.Empty();
				ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
				return 1;
			}

			if ((nSize < 10) && ((buf1.Find(0x0d) != -1) || (buf1.Find(0x0d) != -1)))
			{
				ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
				buf1.Empty();
				return 1;
			}
			else if ((nSize >= 10) && (buf1.Find(0x0d) != -1))
			{
				gStrTmpLog.Format("Com: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
				SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

				strWId += buf1;
				buf1.Empty();

				/*strWId.TrimRight(0x0a);
				strWId.TrimRight(0x0d);*/
				int lpos = strWId.Find(0x0d);
				if (lpos >= 0)
				{
					strWId = strWId.Left(lpos);
				}

				lpos = strWId.Find(0x0a);
				if (lpos >= 0)
				{
					strWId = strWId.Left(lpos);
				}
				if ((strWId.Find(0x0d) != -1) || (strWId.Find(0x0a) != -1))
				{
					//	strWId.TrimLeft(0x0a);
					//	strWId.TrimLeft(0x0d);
					int lpos = strWId.Find(0x0d);
					if (lpos >= 0)
					{
						strWId = strWId.Mid(lpos + 1);
					}

					lpos = strWId.Find(0x0a);
					if (lpos >= 0)
					{
						if (strWId.Find('.') == -1)
						{
							strWId = strWId.Mid(lpos + 1);
							ctrlWipIdEdit.SetWindowText(strWId);
						}
					}

				}
				ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();

				if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
				{
					if (strWId.Find('.') >= 0)
					{
						if (m_GetToolOptiondlg.m_bActivate) {
							CurrentSet->sModelSuffixName = strWId;
							::SendMessage(m_GetToolOptiondlg, UM_UPDATE_TOOL_OPTION, 0, 0);
						}
						else if (m_SelectChassisModelDlg.m_bActivate == 1) {
							CurrentSet->sModelSuffixName = strWId;
							::SendMessage(m_SelectChassisModelDlg, UM_UPDATE_SUFFIX_SCAN, 0, 0);
						}
					}
				}

			}
		}
		else
		{
			if (g_nSysRobotType == AUTO_SYS)
			{
				if (CurrentSet->bIsRunning)
				{
					buf1.Empty();
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					return 1;
				}

				nSize = buf1.GetLength();

				strWId = _T("");

				int lpos = buf1.Find("@");
				if (lpos >= 0)
				{
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					return 1;
				}

				if ((nSize < 10) && ((buf1.Find(0x0d) != -1) || (buf1.Find(0x0d) != -1)))
				{
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					buf1.Empty();
					return 1;
				}
				else if ((nSize >= 10) && (buf1.Find(0x0d) != -1))
				{
					gStrTmpLog.Format("Com: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
					SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

					strWId += buf1;
					buf1.Empty();

					/*strWId.TrimRight(0x0a);
					strWId.TrimRight(0x0d);*/
					int lpos = strWId.Find(0x0d);
					if (lpos >= 0)
					{
						strWId = strWId.Left(lpos);
					}

					lpos = strWId.Find(0x0a);
					if (lpos >= 0)
					{
						strWId = strWId.Left(lpos);
					}
					if ((strWId.Find(0x0d) != -1) || (strWId.Find(0x0a) != -1))
					{
						//	strWId.TrimLeft(0x0a);
						//	strWId.TrimLeft(0x0d);
						int lpos = strWId.Find(0x0d);
						if (lpos >= 0)
						{
							strWId = strWId.Mid(lpos + 1);
						}

						lpos = strWId.Find(0x0a);
						if (lpos >= 0)
						{
							if (strWId.Find('.') == -1)
							{
								strWId = strWId.Mid(lpos + 1);
								ctrlWipIdEdit.SetWindowText(strWId);
							}
						}

					}
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();

					if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
					{
						if (strWId.Find('.') >= 0)
						{
							if (m_GetToolOptiondlg.m_bActivate) {
								CurrentSet->sModelSuffixName = strWId;
								::SendMessage(m_GetToolOptiondlg, UM_UPDATE_TOOL_OPTION, 0, 0);
							}
							else if (m_SelectChassisModelDlg.m_bActivate == 1) {
								CurrentSet->sModelSuffixName = strWId;
								::SendMessage(m_SelectChassisModelDlg, UM_UPDATE_SUFFIX_SCAN, 0, 0);
							}
						}
					}
				}
			}
			else if (CurrentSet->bUseScanner)
			{
				if (CurrentSet->bIsRunning)
				{
					buf1.Empty();
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					return 1;
				}

				nSize = buf1.GetLength();

				strWId = _T("");

				gStrTmpLog.Format("Com: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
				SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

				int lpos = buf1.Find("@");
				if (lpos >= 0)
				{
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					return 1;
				}
				if ((nSize >= 10) && (buf1.Find(0x0d) != -1))
				{
					strWId += buf1;
					buf1.Empty();

					strWId.TrimRight(0x0a);
					strWId.TrimRight(0x0d);
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();

					if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
					{
						if (strWId.Find('.') != -1)
						{
							if (m_GetToolOptiondlg.m_bActivate) {
								CurrentSet->sModelSuffixName = strWId;
								::SendMessage(m_GetToolOptiondlg, UM_UPDATE_TOOL_OPTION, 0, 0);
							}
							else if (m_SelectChassisModelDlg.m_bActivate == 1) {
								CurrentSet->sModelSuffixName = strWId;
								::SendMessage(m_SelectChassisModelDlg, UM_UPDATE_SUFFIX_SCAN, 0, 0);
							}
						}
						else {
							ctrlWipIdEdit.SetWindowText(strWId);
							PostMessage(WM_COMMAND, ID_RUN_RUN);
							CurrentSet->bIsRunMsg = TRUE;
						}
					}
				}
			}
			else if (CurrentSet->bUseStartBox)
			{
				nSize = buf1.GetLength();

				if ((nSize >= 10) && (buf1.Find("@") != -1) && (buf1.Find("$") != -1))
				{

					gStrTmpLog.Format("Com: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
					SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

					int lpos = buf1.Find("@RI");
					if (lpos >= 0)
						sTmp = buf1.Mid(lpos);
					else
						sTmp = "";
					if (sTmp.GetLength() >= 10)
					{
						if (sTmp.GetAt(3) == '1')
						{
							if (sOldStart == 0)
								lStartBTN = 1;
							sOldStart = 1;
						}
						else
						{
							sOldStart = 0;
						}

						if (sTmp.GetAt(4) == '1')
						{
							if (sOldStop == 0)
								lStopBTN = 1;
							sOldStop = 1;
						}
						else
						{
							sOldStop = 0;
						}

						if (sTmp.GetAt(5) == '1')
						{
							if (sOldRetry == 0)
								lRetryBTN = 1;
							sOldRetry = 1;
						}
						else
						{
							sOldRetry = 0;
						}

						if (sTmp.GetAt(6) == 'O')
						{
						}
						else if (sTmp.GetAt(7) == 'O')
						{
							if (sTmp.GetAt(6) == '1')
							{
								if (sOldOK == 0)
									lOKBTN = 1;
								sOldOK = 1;
							}
							else
							{
								sOldOK = 0;
							}
						}

						if (lStartBTN == 1)
						{

							ProcessStartBox(START_BTN1);
						}
						else if (lStopBTN == 1)
						{
							ProcessStartBox(STOP_BTN1);
						}
						else if (lRetryBTN == 1)
						{
							ProcessStartBox(RETRY_BTN1);
						}
						else if (lOKBTN == 1)
						{
							ProcessStartBox(OK_BTN1);
						}
					}
					buf1.Empty();
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
				}
				else
				{
					if (CurrentSet->bIsRunning)
					{
						buf1.Empty();
						ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
						return 1;
					}
					if ((m_GetToolOptiondlg.m_bActivate)&& (m_SelectChassisModelDlg.m_bActivate == 1))
					{
						//if (CurrentSet->bIsRunning)
						//{
						//	buf1.Empty();
						//	ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
						//	return 1;
						//}

						nSize = buf1.GetLength();

						strWId = _T("");

						gStrTmpLog.Format("Com: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
						SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

						int lpos = buf1.Find("@");
						if (lpos >= 0)
						{
							ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
							return 1;
						}
						if ((nSize >= 10) && (buf1.Find(0x0d) != -1))
						{
							strWId += buf1;
							buf1.Empty();

							strWId.TrimRight(0x0a);
							strWId.TrimRight(0x0d);
							ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();

							if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
							{
								if (strWId.Find('.') != -1)
								{
									if (m_GetToolOptiondlg.m_bActivate) {
										CurrentSet->sModelSuffixName = strWId;
										::SendMessage(m_GetToolOptiondlg, UM_UPDATE_TOOL_OPTION, 0, 0);
									}
									else if (m_SelectChassisModelDlg.m_bActivate == 1) {
										CurrentSet->sModelSuffixName = strWId;
										::SendMessage(m_SelectChassisModelDlg, UM_UPDATE_SUFFIX_SCAN, 0, 0);
									}
								}
								else {
									buf1.Empty();
									ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
									//ctrlWipIdEdit.SetWindowText(strWId);
									//PostMessage(WM_COMMAND, ID_RUN_RUN);
									//CurrentSet->bIsRunMsg = TRUE;
								}
							}
						}
					}
					else
					{
						if (buf1.Find(0x0d) != -1)
						{
							buf1.Empty();
							ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
						}
					}
					
				}
			}
			else 
			{
				if (CurrentSet->bIsRunning)
				{
					buf1.Empty();
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					return 1;
				}

				nSize = buf1.GetLength();

				strWId = _T("");

				int lpos = buf1.Find("@");
				if (lpos >= 0)
				{
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();
					return 1;
				}
				if ((nSize >= 10) && (buf1.Find(0x0d) != -1))
				{
					gStrTmpLog.Format("Com: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
					SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);

					strWId += buf1;
					buf1.Empty();

					strWId.TrimRight(0x0a);
					strWId.TrimRight(0x0d);
					ScannerCtrl.m_ctrlScannerCtrl.m_QueueRead.Clear();

					if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
					{
						if (strWId.Find('.') != -1)
						{
							if (m_GetToolOptiondlg.m_bActivate) {
								CurrentSet->sModelSuffixName = strWId;
								::SendMessage(m_GetToolOptiondlg, UM_UPDATE_TOOL_OPTION, 0, 0);
							}
							else if (m_SelectChassisModelDlg.m_bActivate == 1) {
								CurrentSet->sModelSuffixName = strWId;
								::SendMessage(m_SelectChassisModelDlg, UM_UPDATE_SUFFIX_SCAN, 0, 0);
							}
						}					
					}		
				}
			}
		}
	}
	else if((int)wParam == TVCommCtrl.m_nPortID)
	{
		nSize=  (TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetSize(); 
		memset(strReceive, 0, 2048);

		if(nSize >= 2048) {nSize = 2048;}

		if(nSize > 0)
		{
			CString strTmp;
			strTmp = "";
			for(int i = 0 ; i < nSize; i++)
			{
	  			(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //
				strReceive[i] = aByte;
				if (TVCommCtrl.m_nReceiveLength >= 2047) {
					memset(TVCommCtrl.m_nReceiveData, 0, 2048);
					TVCommCtrl.m_nReceiveLength = 0;
				}
				TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;

				sTmp.Format("%02X ", aByte);
				strTmp = strTmp + sTmp;
			}

			//AddStringToStatus(strTmp);
			//gStrTmpLog = "Com:";
			//gStrTmpLog += strTmp;
			//AddStringToStatus(strTmp);
			//gStrTmpLog.Format("Com: %Ts ", buf1);// gStrTmpLog = "CommandLoadFast()"; //
			//SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);



			buf2.Format("%s",strReceive);

			strReadData += buf2;
			buf2.Empty();

			if((m_Rs232cAdcManualCheckDlg.GetSafeHwnd() != NULL)
			&& (m_Rs232cAdcManualCheckDlg.m_bActive == TRUE))
			{
				m_Rs232cAdcManualCheckDlg.WriteLog(strReadData);
			}


			//for (idx1 = 0; idx1 < nSize; idx1++)
			//{			

			//	gJigCtrl.m_strReceive[idx1] = aByte;

			//	sTmp.Format("%02X", aByte);
			//	strReadData2 = strReadData2 + sTmp;

			//	sTmp.Format("%02X ", aByte);
			//	strTmp = strTmp + sTmp;
			//}

			//gStrTmpLog.Format("Com: %Ts ", strTmp);// gStrTmpLog = "CommandLoadFast()"; //
			//AddStringToStatus(strTmp);


			TVCommCtrl.m_sReceive += strReadData;
			UpdateData(TRUE);
			::SetEvent(TVCommCtrl.m_hReadEvent);

			/*
			if(CurrentSet->bIsRunning)
			{
				for(int i = 0 ; i < nSize; i++)
				{
	  				(TVCommCtrl.m_ctrlTVCommCtrl.m_QueueRead).GetByte(&aByte);  //±ÛÀÚ ÇÏ³ª¾¿ ÀÐ¾î¿È
					strReceive[i] = aByte;
					TVCommCtrl.m_nReceiveData[TVCommCtrl.m_nReceiveLength++] = aByte;
					nTemp = TVCommCtrl.m_nReceiveLength -1;
					if((aByte == 0x0d) ||  (aByte == 'x')) {
						strReadData.Format("%s",TVCommCtrl.m_nReceiveData);
						UpdateData(TRUE);
						AddStringToStatus(strReadData);
						TVCommCtrl.m_sReceive = strReadData;
						::SetEvent(TVCommCtrl.m_hReadEvent);

					}
					if(nTemp >= 5){ 
						if((TVCommCtrl.m_nReceiveData[nTemp-3] == 0x20) && ((TVCommCtrl.m_nReceiveData[nTemp-2] == 0x31) ||  (TVCommCtrl.m_nReceiveData[nTemp-2] == 0x32) || (TVCommCtrl.m_nReceiveData[nTemp-2] == 0x33)) && 
							(TVCommCtrl.m_nReceiveData[nTemp-1] == 0x20) &&(aByte == 0x30)) {
							strReadData.Format("%s",TVCommCtrl.m_nReceiveData);
							TVCommCtrl.m_sReceive = strReadData;
							UpdateData(TRUE);
							AddStringToStatus(strReadData);
							::SetEvent(TVCommCtrl.m_hReadEvent);

						}
					}
				}
				
			}*/



		}
	}
	else if((int)wParam == gJigCtrl.m_nPortID)
	{
		if(g_nSysType != AUTO_SYS){
			gJigCtrl.m_ctrlJigCom.m_QueueRead.Clear();
			return 1;
		}
		if (CurrentSet->bUsePLCRobot == TRUE)//(g_AutoControlProcess != _AUTO_ROBOT_CTRL_IDLE)
		{
			nSize2 = (gJigCtrl.m_ctrlJigCom.m_QueueRead).GetSize();

			if (nSize2 >= 30)
			{
				gJigCtrl.m_ctrlJigCom.m_QueueRead.Clear();
				nSize2 = 0;
			}
			if (nSize2 >= 4)
			{
				memset(gJigCtrl.m_strReceive, 0, 225);
				strReadData2 = _T("");
				for (idx1 = 0; idx1 < nSize2; idx1++)
				{
					(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&aByte);  //

					gJigCtrl.m_strReceive[idx1] = aByte;

					sTmp.Format("%02X", aByte);
					strReadData2 = strReadData2 + sTmp;
				}

				gStrTmpLog.Format("Com: %Ts ", strReadData2);// gStrTmpLog = "CommandLoadFast()"; //
				SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
			}

			
			while (nSize2 >= 4)
			{
				if ((nSize2 >= 4) && (gJigCtrl.m_strReceive[3] == 0x03))
				{
					if (gJigCtrl.m_strReceive[1] == 'S')
					{
						if ((gJigCtrl.m_strReceive[2] & 0x01) == 0x01)
						{
							gJigCtrl.m_bJigUpStatus = 0;
							gJigCtrl.m_bJigDownStatus = 1;
							gJigCtrl.m_nCylinder = 7;
						}
						else
						{
							gJigCtrl.m_bJigDownStatus = 0;
							gJigCtrl.m_bJigUpStatus = 1;
							gJigCtrl.m_nCylinder = 0;
						}

						if (gJigCtrl.m_bGet_JigReady) {
							gJigCtrl.m_sReceive = strReadData2;
							UpdateData(TRUE);
							::SetEvent(gJigCtrl.m_hReadEvent);
						}

					}
					else
					{
						gJigCtrl.m_sReceive = strReadData2;
						UpdateData(TRUE);
						::SetEvent(gJigCtrl.m_hReadEvent);
					}

					nSize2 -= 4;
					if (nSize2 >= 4)
					{
						for (int i = 0; i < nSize2; i++)
						{
							gJigCtrl.m_strReceive[i] = gJigCtrl.m_strReceive[i + 4];

						}
					}
				}
				else
				{
					gJigCtrl.m_ctrlJigCom.m_QueueRead.Clear();
					nSize2 = 0;

				}
				if (nSize2 > 0)
				{
					sTmp.Format("While : %02X %02X %02X %02X", gJigCtrl.m_strReceive[0], gJigCtrl.m_strReceive[1], gJigCtrl.m_strReceive[2], gJigCtrl.m_strReceive[3]);
					SystemMonitorLog_Save(sTmp);
					_Wait(30);
				}
			}
		}
		else
		{
			nSize2 = (gJigCtrl.m_ctrlJigCom.m_QueueRead).GetSize();

			if (nSize2 >= 4)
			{
				memset(gJigCtrl.m_strReceive, 0, 225);
				strReadData2 = _T("");
				
				CString strTmp = _T("Jig>>PC "); 
				for (idx1 = 0; idx1 < nSize2; idx1++)
				{
					(gJigCtrl.m_ctrlJigCom.m_QueueRead).GetByte(&aByte);  //±ÛÀÚ ÇÏ³ª¾¿ ÀÐ¾î¿È

					gJigCtrl.m_strReceive[idx1] = aByte;

					sTmp.Format("%02X", aByte);
					strReadData2 = strReadData2 + sTmp;

					sTmp.Format("%02X ", aByte);
					strTmp = strTmp + sTmp;
				}
				
				gStrTmpLog.Format("Com: %Ts ", strTmp);// gStrTmpLog = "CommandLoadFast()"; //
				SystemMonitorLog_Save(gStrTmpLog, _T(__FILE__), __LINE__);
				AddStringToStatus(strTmp);
			}
			while (nSize2 >= 4)
			{
				if ((nSize2 >= 4) && (gJigCtrl.m_strReceive[3] == 0x03))
				{
					if (gJigCtrl.m_strReceive[1] == 'K') {
						ProcessJigButton(gJigCtrl.m_strReceive[2]);
					}
					else if (gJigCtrl.m_strReceive[1] == 'S') {
						if ((gJigCtrl.m_strReceive[2] & 0x01) == 0x01) {
							nJigStatus = IN_JIGDN;

							//godtech 190724
							gJigCtrl.m_bJigUpStatus = 0;
							gJigCtrl.m_bJigDownStatus = 1;
							gJigCtrl.m_nCylinder = 7;
						}
						else {
							nJigStatus = IN_JIGUP;

							//godtech 190724
							gJigCtrl.m_bJigDownStatus = 0;
							gJigCtrl.m_bJigUpStatus = 1;
							gJigCtrl.m_nCylinder = 0;
						}
						if (gJigCtrl.m_bGet_JigReady) {
							gJigCtrl.m_sReceive = strReadData2;
							UpdateData(TRUE);
							::SetEvent(gJigCtrl.m_hReadEvent);
						}
						else {
							JigStatus_Process(nJigStatus);
						}
					}
					else {
						gJigCtrl.m_sReceive = strReadData2;
						UpdateData(TRUE);
						::SetEvent(gJigCtrl.m_hReadEvent);
					}
					nSize2 -= 4;
					if (nSize2 >= 4)
					{
						for (int i = 0; i < nSize2; i++)
						{
							gJigCtrl.m_strReceive[i] = gJigCtrl.m_strReceive[i + 4];

						}
					}
				}
				else
				{
					gJigCtrl.m_ctrlJigCom.m_QueueRead.Clear();
					nSize2 = 0;

				}
			}
			
		}
	} 
	return 1;
}

void CDATsysView::ProcessStartBox(LONG lBTN)//START_BTN1
{
	LONG nStatus = lBTN;
	LONG nSTART_BTN;
	LONG nSTOP_BTN;
	LONG nRETRY_BTN;
	LONG nOK_BTN;

	if (g_nRunningProcessNo == 1)
	{
		nSTART_BTN = MSG_START_BTN1;
		nSTOP_BTN = MSG_STOP_BTN1;
		nRETRY_BTN = MSG_RETRY_BTN1;
		nOK_BTN = MSG_OK_BTN1;
	}	
	else if (g_nRunningProcessNo == 2)
	{
		nSTART_BTN = MSG_START_BTN2;
		nSTOP_BTN = MSG_STOP_BTN2;
		nRETRY_BTN = MSG_RETRY_BTN2;
		nOK_BTN = MSG_OK_BTN2;
	}
	else if (g_nRunningProcessNo == 3)
	{
		nSTART_BTN = MSG_START_BTN3;
		nSTOP_BTN = MSG_STOP_BTN3;
		nRETRY_BTN = MSG_RETRY_BTN3;
		nOK_BTN = MSG_OK_BTN3;
	}
		switch (nStatus)
		{
		case START_BTN1:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, nSTART_BTN);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, nSTART_BTN);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, nSTART_BTN);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, nSTART_BTN);
			}
			else {
				if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
					if (m_bResultDlgActivate) {
						SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
						m_bResultDlgActivate = FALSE;
					}

					//AUTO
					if (g_nSysType == AUTO_SYS) {
						if (CurrentSet->nRunStartType == MANUAL_START)
						{
							//	CurrentSet->bIsRunMsg = TRUE;
							PostMessage(WM_COMMAND, ID_RUN_RUN);
						}
					}
					else {
						if (!CurrentSet->bUseScanner) {
							PostMessage(WM_COMMAND, ID_RUN_RUN);
							CurrentSet->bIsRunMsg = TRUE;
						}
					}
				}
			}
		}
		break;

		case STOP_BTN1:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, nSTOP_BTN);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, nSTOP_BTN);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, nSTOP_BTN);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, nSTOP_BTN);
			}
			else {
				if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE) {
					CurrentSet->bRunAbort = TRUE;
					m_szResultMsg2.LoadString(IDS_USER_BREAK);
					AddStringToStatus(m_szResultMsg2);
					g_nUserStop++;
				}

				if (m_bResultDlgActivate) {
					SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
					m_bResultDlgActivate = FALSE;
				}
			}
		}
		break;

		case RETRY_BTN1:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, nRETRY_BTN);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, nRETRY_BTN);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, nRETRY_BTN);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, nRETRY_BTN);
			}
		}
		break;

		
		case OK_BTN1:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, nOK_BTN);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, nOK_BTN);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, nOK_BTN);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, nOK_BTN);
			}
		}
		break;
	}
	

	//return 0L;
}


void CDATsysView::JigStatus_Process(int nJigStatus) 
{
	CString sMsg;

	if(CurrentSet->nRunStartType == AUTO_START)
	{	
		if(nJigStatus == IN_JIGUP)
		{
			if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
			{
					gJigCtrl.Set_ACPower(0);
					SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
					
					CurrentSet->bSystemBreak = TRUE;

					sMsg.LoadString(IDS_CYLINDER_UP);
					AfxMessageBox(sMsg);
			}
			CurrentSet->nJigStatus = IN_JIGUP;
			AddStringToStatus(_T("Jig Up"));
		}
		else if(nJigStatus == IN_JIGDN)
		{
			//if(CurrentSet->nJigStatus == IN_JIGUP)
			{
				if (CurrentSet->nRunStartType == AUTO_START && CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
				{
				
					PostMessage(WM_COMMAND, ID_RUN_RUN);
				}
			}
			CurrentSet->nJigStatus = IN_JIGDN;
			AddStringToStatus(_T("Jig Down"));
		}
	}
	else{
		if(nJigStatus == IN_JIGUP)
		{
			if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
			{
					gJigCtrl.Set_ACPower(0);
					SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
					//AddStringToStatus(_T("Jig Up"));
					CurrentSet->bSystemBreak = TRUE;
					sMsg.LoadString(IDS_CYLINDER_UP);
					AfxMessageBox(sMsg);
			}
			CurrentSet->nJigStatus = IN_JIGUP;
			AddStringToStatus(_T("Jig Up"));
		}
		else if(nJigStatus == IN_JIGDN)
		{
			CurrentSet->nJigStatus = IN_JIGDN;
			AddStringToStatus(_T("Jig Down"));
		}
	}
}

void CDATsysView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	GetRectSizeInfo();
	//-
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDATsysView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	GetRectSizeInfo();
	//-
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
}
void CDATsysView::InitGrabImageEvent()
{
	hGrabEvent = CreateEvent(NULL,false,false,NULL);

	if (hGrabEvent == NULL)
	{ 
		AfxMessageBox("Create UHD Grab Events error");

		ExitProcess(0); 
	} 

	hGrabEvent_Complete = CreateEvent(NULL,false,false,NULL);


	if (hGrabEvent_Complete == NULL)
	{ 
		AfxMessageBox("Create Grab Complete Events error");
		ExitProcess(0); 
	}
}
/*
void CDATsysView::InitEventAndFileMappingObject()
{
	if(g_nGrabberType == UHD_GRABBER)
	{
		InitEventAndFileMappingObject_UHD();
	}
	{
		InitEventAndFileMappingObject_FHD();
	}
}
*/
void CDATsysView::InitEventAndFileMappingObject()
{
	// Init File Mapping Handle
	m_hUsbDioCtrlFileMapping = NULL;
	m_hMhlCtrlFileMapping = NULL;

	m_IsFileMhlCtrlMappingCreated = FALSE;
	m_BufferMhlCtrl_Size = sizeof(MhlControlStruct);
//AUTO	
	if(g_nSysType == AUTO_SYS){
		m_hBarcodeDataFileMapping  = NULL;
		m_IsBarcodeDataMappingCreated = FALSE;

//		m_hDaqDataFileMapping  = NULL;
//		m_IsDaqDataMappingCreated = FALSE;

		hBarcodeClientEvents[0] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_BARCODE1");
		hBarcodeClientEvents[1] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_BARCODE2");
		hBarcodeClientEvents[2] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_BARCODE3");

		//hBarcodeClientEvents[3] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_DAQ1");
		//hBarcodeClientEvents[4] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_DAQ2");
		//hBarcodeClientEvents[5] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_DAQ3");

		hBarcodeServerEvents[0] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_BARCODE1_COMPLETE");
		hBarcodeServerEvents[1] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_BARCODE2_COMPLETE");
		hBarcodeServerEvents[2] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_BARCODE3_COMPLETE");

		//hBarcodeServerEvents[3] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_DAQ1_COMPLETE");
		//hBarcodeServerEvents[4] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_DAQ2_COMPLETE");
		//hBarcodeServerEvents[5] = CreateEvent(NULL,FALSE,FALSE,"EVENT_OBJ_DAQ3_COMPLETE");
		//if (hBarcodeClientEvents[0] == NULL || hBarcodeClientEvents[1] == NULL || hBarcodeClientEvents[2] == NULL
		//	|| hBarcodeClientEvents[3] == NULL || hBarcodeClientEvents[4] == NULL || hBarcodeClientEvents[5] == NULL
		//	|| hBarcodeServerEvents[0] == NULL || hBarcodeServerEvents[1] == NULL || hBarcodeServerEvents[2] == NULL
		//	|| hBarcodeServerEvents[3] == NULL || hBarcodeServerEvents[4] == NULL || hBarcodeServerEvents[5] == NULL)

		if (hBarcodeClientEvents[0] == NULL || hBarcodeClientEvents[1] == NULL || hBarcodeClientEvents[2] == NULL		 
		 || hBarcodeServerEvents[0] == NULL || hBarcodeServerEvents[1] == NULL || hBarcodeServerEvents[2] == NULL ) 
		{ 
			TRACE("Barcode CreateEvent error: %d\n", GetLastError() ); 
			ExitProcess(0); 
		}

	}
	else{
		m_IsFileUsbDioCtrlMappingCreated = FALSE;
		m_BufferUsbDioCtrl_Size = sizeof(UsbDioStruct);

		if(g_nRunningProcessNo == 1){
			hUsbCtrl = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_USBDIO_CTRL1");
		}
		else if(g_nRunningProcessNo == 2){
			hUsbCtrl = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_USBDIO_CTRL2");
		}
		else{
			hUsbCtrl = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_USBDIO_CTRL3");
		}
		if (hUsbCtrl == NULL)
		{ 
		//	TRACE("Create ClientEvents error: %d\n", GetLastError() ); 
			AfxMessageBox("Create ClientEvents error:USBDIO");

			ExitProcess(0); 
		}
		if(g_nRunningProcessNo == 1){
			hUsbCtrl_Complete = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_USBDIO_CTRL1_COMPLETE");
		}
		else if(g_nRunningProcessNo == 2){
			hUsbCtrl_Complete = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_USBDIO_CTRL2_COMPLETE");
		}
		else{
			hUsbCtrl_Complete = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_USBDIO_CTRL3_COMPLETE");
		}
		if (hUsbCtrl_Complete == NULL)
		{ 
		//	TRACE("Create ServerEvents error: %d\n", GetLastError() ); 
			AfxMessageBox("Create ServerEvents error:UsbCtrl_Complete");
			ExitProcess(0); 
		}
	}
//

	if(g_nRunningProcessNo == 1){
		hMhlCtrl = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_MHL_CTRL1");
	}
	else if(g_nRunningProcessNo == 2){
		hMhlCtrl = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_MHL_CTRL2");
	}
	else{
		hMhlCtrl = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_MHL_CTRL3");
	}


	if (hMhlCtrl == NULL)
	{ 
	//	TRACE("Create ClientEvents error: %d\n", GetLastError() ); 
		AfxMessageBox("Create ClientEvents error:MHL_CTRL");

		ExitProcess(0); 
	} 


	if(g_nRunningProcessNo == 1){
		hMhlCtrl_Complete = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_MHL_CTRL1_COMPLETE");
	}
	else if(g_nRunningProcessNo == 2){
		hMhlCtrl_Complete = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_MHL_CTRL2_COMPLETE");
	}
	else{
		hMhlCtrl_Complete = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_MHL_CTRL3_COMPLETE");
	}

	if (hMhlCtrl_Complete  == NULL)
	{ 
	//	TRACE("Create ServerEvents error: %d\n", GetLastError() ); 
		AfxMessageBox("Create ServerEvents error:MhlCtrl_Complete");
		ExitProcess(0); 
	}
	if(g_nGrabberType == FHD_GRABBER)
	{
		InitEventAndFileMappingObject_FHD();
	}

	// Create FileMapping Obejcts
	CreateFileMappingObject();
}

BOOL CDATsysView::CreateFileMappingObject()
{
	char MapFileName[100];
	//if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == 1))
	//{
	//	
	//}
//AUTO	
	if (g_nUseNoScanType == FALSE)//if (g_nSysRobotType != AUTO_SYS)//godtech 190531
	{
		if (g_nSysType == AUTO_SYS) {
			//if (g_nSysRobotType != AUTO_SYS)
			{
				if (!m_IsBarcodeDataMappingCreated)
				{
					memset(MapFileName, NULL, 100);

					if (g_nRunningProcessNo == 1)
					{
						strcpy_s(MapFileName, _countof(MapFileName), "BARCODE1_DATA"); //Some arbitrary name.  use this name to access the file from other processes
					}
					else if (g_nRunningProcessNo == 2)
					{
						strcpy_s(MapFileName, _countof(MapFileName), "BARCODE2_DATA"); //Some arbitrary name.  use this name to access the file from other processes
					}
					else
					{
						strcpy_s(MapFileName, _countof(MapFileName), "BARCODE3_DATA"); //Some arbitrary name.  use this name to access the file from other processes
					}

					m_hBarcodeDataFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapFileName);

					if (m_hBarcodeDataFileMapping != NULL)
					{

						SetNamedSecurityInfo(MapFileName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);

						m_pBarcodeData = (BarcodeStruct *)MapViewOfFile(m_hBarcodeDataFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

						if (m_pBarcodeData == NULL)
						{
							AfxMessageBox("Failed to create Barcode data mapping file!");
							CloseHandle(m_hBarcodeDataFileMapping);
							return FALSE;
						}
						else
						{
							m_IsBarcodeDataMappingCreated = TRUE;
						}
					}
					else
					{
						AfxMessageBox("Failed to create Barcode data mapping file!");
					}
				}
				m_pBarcodeData->nSize = 0;

			}
			//if (!m_IsDaqDataMappingCreated)
			//{
			//	memset(MapFileName, NULL, 100);

			//	if (g_nRunningProcessNo == 1)
			//	{
			//		strcpy_s(MapFileName, _countof(MapFileName), "DAQ1_DATA"); //Some arbitrary name.  use this name to access the file from other processes
			//	}
			//	else if (g_nRunningProcessNo == 2)
			//	{
			//		strcpy_s(MapFileName, _countof(MapFileName), "DAQ2_DATA"); //Some arbitrary name.  use this name to access the file from other processes
			//	}
			//	else
			//	{
			//		strcpy_s(MapFileName, _countof(MapFileName), "DAQ3_DATA"); //Some arbitrary name.  use this name to access the file from other processes
			//	}

			//	m_hDaqDataFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapFileName);

			//	if (m_hDaqDataFileMapping != NULL)
			//	{

			//		SetNamedSecurityInfo(MapFileName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);

			//		m_pDaqData = (DaqStruct *)MapViewOfFile(m_hDaqDataFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

			//		if (m_pDaqData == NULL)
			//		{
			//			AfxMessageBox("Failed to create Daq data mapping file!");
			//			CloseHandle(m_hDaqDataFileMapping);
			//			return FALSE;
			//		}
			//		else
			//		{
			//			m_IsDaqDataMappingCreated = TRUE;
			//		}
			//	}
			//	else
			//	{
			//		AfxMessageBox("Failed to create Daq data mapping file!");
			//	}
			//}
			//m_pDaqData->nFuncIndex = 0;
			//m_pDaqData->nStartChannel = 0;
			//m_pDaqData->nEndChannel = 0;
			//m_pDaqData->dRetValue = 0.0;
		}
		else {
			if (!m_IsFileUsbDioCtrlMappingCreated)//if (g_nSysType != AUTO_SYS) {
			{
				memset(MapFileName, NULL, 100);

				if (g_nRunningProcessNo == 1)
				{
					strcpy_s(MapFileName, _countof(MapFileName), "USB_DIO_CTRL1"); //Some arbitrary name.  use this name to access the file from other processes
				}
				else if (g_nRunningProcessNo == 2)
				{
					strcpy_s(MapFileName, _countof(MapFileName), "USB_DIO_CTRL2"); //Some arbitrary name.  use this name to access the file from other processes
				}
				else
				{
					strcpy_s(MapFileName, _countof(MapFileName), "USB_DIO_CTRL3"); //Some arbitrary name.  use this name to access the file from other processes
				}

				m_hUsbDioCtrlFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapFileName);

				if (m_hUsbDioCtrlFileMapping != NULL)
				{
					SetNamedSecurityInfo(MapFileName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);

					m_pUsbDio_Control = (UsbDioStruct *)MapViewOfFile(m_hUsbDioCtrlFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

					if (m_pUsbDio_Control == NULL)
					{
						AfxMessageBox("Failed to create USB DIO Control mapping file!");
						CloseHandle(m_hUsbDioCtrlFileMapping);
						return FALSE;
					}
					else
					{
						m_IsFileUsbDioCtrlMappingCreated = TRUE;
					}
				}
				else
				{
					AfxMessageBox("Failed to create USB DIO Control mapping file!");
				}
			}

			m_pUsbDio_Control->nChannel = 0;
			m_pUsbDio_Control->nCtrlData = 0;
			m_pUsbDio_Control->nResult = 0;
		}

		if (!m_IsFileMhlCtrlMappingCreated)
		{
			memset(MapFileName, NULL, 100);

			if (g_nRunningProcessNo == 1)
			{
				strcpy_s(MapFileName, _countof(MapFileName), "MHL_CTRL1"); //Some arbitrary name.  use this name to access the file from other processes
			}
			else if (g_nRunningProcessNo == 2)
			{
				strcpy_s(MapFileName, _countof(MapFileName), "MHL_CTRL2"); //Some arbitrary name.  use this name to access the file from other processes
			}
			else if (g_nRunningProcessNo == 3)
			{
				strcpy_s(MapFileName, _countof(MapFileName), "MHL_CTRL3"); //Some arbitrary name.  use this name to access the file from other processes
			}

			m_hMhlCtrlFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapFileName);

			if (m_hMhlCtrlFileMapping != NULL)
			{
				SetNamedSecurityInfo(MapFileName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);

				m_pMhlCtrl = (MhlControlStruct *)MapViewOfFile(m_hMhlCtrlFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

				if (m_pMhlCtrl == NULL)
				{
					AfxMessageBox("Failed to create MHL Control mapping file!");
					CloseHandle(m_hMhlCtrlFileMapping);
					return FALSE;
				}
				else
				{
					m_IsFileMhlCtrlMappingCreated = TRUE;
				}
			}
			else
			{
				AfxMessageBox("Failed to create MHL Control mapping file!");
			}
		}

		m_pMhlCtrl->nFunction = 0;
		m_pMhlCtrl->nChannel = 0;
		m_pMhlCtrl->nCtrlData = 0;
		m_pMhlCtrl->nResult = 0;
	}
	else
	{
		if (g_nSysType != AUTO_SYS)
		{
			if (!m_IsFileUsbDioCtrlMappingCreated)//
			{
				memset(MapFileName, NULL, 100);

				if (g_nRunningProcessNo == 1)
				{
					strcpy_s(MapFileName, _countof(MapFileName), "USB_DIO_CTRL1"); //Some arbitrary name.  use this name to access the file from other processes
				}
				else if (g_nRunningProcessNo == 2)
				{
					strcpy_s(MapFileName, _countof(MapFileName), "USB_DIO_CTRL2"); //Some arbitrary name.  use this name to access the file from other processes
				}
				else
				{
					strcpy_s(MapFileName, _countof(MapFileName), "USB_DIO_CTRL3"); //Some arbitrary name.  use this name to access the file from other processes
				}

				m_hUsbDioCtrlFileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapFileName);

				if (m_hUsbDioCtrlFileMapping != NULL)
				{
					SetNamedSecurityInfo(MapFileName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL)NULL, NULL);

					m_pUsbDio_Control = (UsbDioStruct *)MapViewOfFile(m_hUsbDioCtrlFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

					if (m_pUsbDio_Control == NULL)
					{
						AfxMessageBox("Failed to create USB DIO Control mapping file!");
						CloseHandle(m_hUsbDioCtrlFileMapping);
						return FALSE;
					}
					else
					{
						m_IsFileUsbDioCtrlMappingCreated = TRUE;
					}
				}
				else
				{
					AfxMessageBox("Failed to create USB DIO Control mapping file!");
				}
			}

			m_pUsbDio_Control->nChannel = 0;
			m_pUsbDio_Control->nCtrlData = 0;
			m_pUsbDio_Control->nResult = 0;
		}
	}


	if(g_nGrabberType == FHD_GRABBER)
	{
		  CreateFileMappingObject_FHD();
	}

	return TRUE;
}
//FHD
void CDATsysView::InitEventAndFileMappingObject_FHD()
{
	// Init File Mapping Handle
	m_hImageDataFileMapping  = NULL;
	m_hImageInfoFileMapping = NULL;

	// Init
	m_IsFileImageDataMappingCreated = FALSE;
	m_IsFileImageInfoMappingCreated = FALSE;

	hClientEvents[0] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB1_INFO");
	hClientEvents[1] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB2_INFO");
	hClientEvents[2] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB3_INFO");
	hClientEvents[3] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB1_IMAGE");
	hClientEvents[4] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB2_IMAGE");
	hClientEvents[5] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB3_IMAGE");
	hClientEvents[6] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB4_IMAGE");
	hClientEvents[7] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB5_IMAGE");
	hClientEvents[8] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB6_IMAGE");
	hClientEvents[9] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB1_GETFW");
	hClientEvents[10] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB2_GETFW");
	hClientEvents[11] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB3_GETFW");

	if (hClientEvents[0] == NULL  || hClientEvents[1] == NULL  || hClientEvents[2] == NULL  || hClientEvents[3] == NULL
	||	hClientEvents[4] == NULL  || hClientEvents[5] == NULL  || hClientEvents[6] == NULL  || hClientEvents[7] == NULL
	||	hClientEvents[8] == NULL  || hClientEvents[9] == NULL  || hClientEvents[10] == NULL || hClientEvents[11] == NULL) 
	{ 
		//TRACE("Create ClientEvents error: %d\n", GetLastError() ); 
		AfxMessageBox("Create ClientEvents error");
		ExitProcess(0); 
	} 

	hServerEvents[0] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB1_COMPLETE");
	hServerEvents[1] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB2_COMPLETE");
	hServerEvents[2] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB3_COMPLETE");
	hServerEvents[3] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB4_COMPLETE");
	hServerEvents[4] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB5_COMPLETE");
	hServerEvents[5] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRAB6_COMPLETE");
	hServerEvents[6] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRABBER1_FW_COMPLETE");
	hServerEvents[7] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRABBER2_FW_COMPLETE");
	hServerEvents[8] = OpenEvent(EVENT_ALL_ACCESS,FALSE,"EVENT_OBJ_GRABBER3_FW_COMPLETE");

	if (hServerEvents[0]  == NULL || hServerEvents[1]  == NULL || hServerEvents[2]  == NULL || hServerEvents[3]  == NULL
	||	hServerEvents[4]  == NULL || hServerEvents[5]  == NULL || hServerEvents[6]  == NULL || hServerEvents[7]  == NULL 
	||	hServerEvents[8]  == NULL)
	{ 
	//	TRACE("Create ServerEvents error: %d\n", GetLastError() ); 
		AfxMessageBox("Create ServerEvents error");
		ExitProcess(0); 
	}

}

BOOL CDATsysView::CreateFileMappingObject_FHD()
{
	char MapFileName[100];

	if(!m_IsFileImageDataMappingCreated)
	{
		memset(MapFileName,NULL,100);

		if(g_nRunningProcessNo == 1)
		{
			strcpy(MapFileName,"GRAB1_IMAGE"); //Some arbitrary name.  use this name to access the file from other processes
		//	strcpy_s(MapFileName, _countof(MapFileName),"GRAB1_IMAGE"); //Some arbitrary name.  use this name to access the file from other processes
		}
		else if(g_nRunningProcessNo == 2)
		{
			strcpy(MapFileName,"GRAB2_IMAGE"); //Some arbitrary name.  use this name to access the file from other processes
		//	strcpy_s(MapFileName, _countof(MapFileName),"GRAB2_IMAGE"); //Some arbitrary name.  use this name to access the file from other processes
		}
		else
		{
			strcpy(MapFileName,"GRAB3_IMAGE"); //Some arbitrary name.  use this name to access the file from other processes
			//strcpy_s(MapFileName, _countof(MapFileName),"GRAB3_IMAGE"); //Some arbitrary name.  use this name to access the file from other processes
		}

		m_hImageDataFileMapping = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, MapFileName);

		if(m_hImageDataFileMapping != NULL)
		{
			
			SetNamedSecurityInfo(MapFileName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL) NULL, NULL);

			g_GrabImage.m_pImageData = (BYTE *) MapViewOfFile( m_hImageDataFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

			if (g_GrabImage.m_pImageData == NULL)
			{
				AfxMessageBox("Failed to create image data mapping file!");
				CloseHandle(m_hImageDataFileMapping);
				return FALSE;
			}
			else
			{
				m_IsFileImageDataMappingCreated = TRUE;
			}
		}
		else
		{
			AfxMessageBox("Failed to create image data mapping file!");
		}
	}

	if(!m_IsFileImageInfoMappingCreated)
	{
		memset(MapFileName,NULL,100);

		if(g_nRunningProcessNo == 1)
		{
			strcpy_s(MapFileName, _countof(MapFileName),"GRAB1_INFO"); //Some arbitrary name.  use this name to access the file from other processes
		}
		else if(g_nRunningProcessNo == 2)
		{
			strcpy_s(MapFileName, _countof(MapFileName),"GRAB2_INFO"); //Some arbitrary name.  use this name to access the file from other processes
		}
		else
		{
			strcpy_s(MapFileName, _countof(MapFileName),"GRAB3_INFO"); //Some arbitrary name.  use this name to access the file from other processes
		}

		m_hImageInfoFileMapping = OpenFileMapping (FILE_MAP_ALL_ACCESS, FALSE, MapFileName);

		if(m_hImageInfoFileMapping != NULL)
		{
			SetNamedSecurityInfo(MapFileName, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, 0, 0, (PACL) NULL, NULL);

			m_pImageInfo = (ImageInfoStruct *) MapViewOfFile( m_hImageInfoFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

			if (m_pImageInfo == NULL)
			{
				AfxMessageBox("Failed to create image info1 mapping file!");
				CloseHandle(m_hImageInfoFileMapping);
				return FALSE;
			}
			else
			{
				m_IsFileImageInfoMappingCreated = TRUE;
			}
		}
		else
		{
			AfxMessageBox("Failed to create image data mapping file!");
		}
	}

	m_pImageInfo->nLVDSWidth		= 0;
	m_pImageInfo->nLVDSHeight		= 0;
	m_pImageInfo->nAnalogWidth		= 0;
	m_pImageInfo->nAnalogHeight		= 0;

	m_pImageInfo->nBitCount			= 0;
	m_pImageInfo->nGrabberMode		= 0;
	m_pImageInfo->nGrabImageLoaded	= 0;

	// 090120 External DFT
	m_pImageInfo->nHbackPorch		= 0;
	m_pImageInfo->nVbackPorch		= 0;

	//add psh 091214
	m_pImageInfo->nFullHd120Hz		= 0;
	m_pImageInfo->nImageRotation 	= 0;

	return TRUE;
}


void CDATsysView::SetGrabInfo(CBaseImage* pBaseImage)
{
	BOOL bChanged = FALSE;

	int nRowWidth  = 0;
	int nImageSize = 0;


	//+add kwmoon 080402
	CString szErrMsg = _T("");
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if((CurrentSet->nLvdsWidth < 0) || (CurrentSet->nLvdsWidth > 3840))
		{
			szErrMsg.Format("Current unsupported LVDS width is reset (%d->1366)",CurrentSet->nLvdsWidth);
			CurrentSet->nLvdsWidth = 1366;
		
			AfxMessageBox(szErrMsg);
		}

		if((CurrentSet->nLvdsHeight < 0) || (CurrentSet->nLvdsHeight > 2160))
		{
			szErrMsg.Format("Current unsupported LVDS height is reset (%d->768)",CurrentSet->nLvdsHeight);
			CurrentSet->nLvdsHeight = 768;

			AfxMessageBox(szErrMsg);
		}
	}
	else//FHD
	{
		if((CurrentSet->nLvdsWidth < 0) || (CurrentSet->nLvdsWidth > 2560))
		{
			szErrMsg.Format("Current unsupported LVDS width is reset (%d->1366)",CurrentSet->nLvdsWidth);
			CurrentSet->nLvdsWidth = 1366;
		
			AfxMessageBox(szErrMsg);
		}

		if((CurrentSet->nLvdsHeight < 0) || (CurrentSet->nLvdsHeight > 1440))
		{
			szErrMsg.Format("Current unsupported LVDS height is reset (%d->768)",CurrentSet->nLvdsHeight);
			CurrentSet->nLvdsHeight = 768;

			AfxMessageBox(szErrMsg);
		}

		if((unsigned)m_pImageInfo->nLVDSWidth != CurrentSet->nLvdsWidth)
		{
			m_pImageInfo->nLVDSWidth = CurrentSet->nLvdsWidth; 
			bChanged =TRUE;
		}

		if((unsigned)m_pImageInfo->nLVDSHeight != CurrentSet->nLvdsHeight)
		{
			m_pImageInfo->nLVDSHeight = CurrentSet->nLvdsHeight; 
			bChanged =TRUE;
		}

		CurrentSet->nBitCount = 24;
	
		if((unsigned)m_pImageInfo->nBitCount != CurrentSet->nBitCount)
		{
			m_pImageInfo->nBitCount = CurrentSet->nBitCount; bChanged =TRUE;
		}

		if((unsigned)m_pImageInfo->nGrabberMode != CurrentSet->nUranousMode)
		{
			m_pImageInfo->nGrabberMode = CurrentSet->nUranousMode; bChanged =TRUE;
		}

		if(m_pImageInfo->nFullHd120Hz != CurrentSet->nFullHd120Hz)
		{
			m_pImageInfo->nFullHd120Hz = CurrentSet->nFullHd120Hz; bChanged =TRUE;
		}

		if(m_pImageInfo->nImageRotation != CurrentSet->nImageRotation)
		{
			m_pImageInfo->nImageRotation = CurrentSet->nImageRotation; bChanged =TRUE;
		}

		if(m_pImageInfo->nGrabImageLoaded != 0)
		{
			m_pImageInfo->nGrabImageLoaded = 0; bChanged =TRUE;
		}

		nRowWidth  = WIDTHBYTES(CurrentSet->nBitCount*CurrentSet->nLvdsWidth);
		nImageSize = nRowWidth * CurrentSet->nLvdsHeight;
		
		pBaseImage->InitHeaders(COLOR_24BIT,CurrentSet->nLvdsWidth,CurrentSet->nLvdsHeight,nImageSize);

		if(CurrentSet->nAnalogType == V_SRC_NTSC)
		{
			m_pImageInfo->nAnalogWidth  = 640;
			m_pImageInfo->nAnalogHeight = 480;

			m_pImageInfo->nVbackPorch	= NTSC_DEFAULT_V_BPCH;
			m_pImageInfo->nHbackPorch	= NTSC_DEFAULT_H_BPCH;

			I2cAdcCtrl.SetExtAnalogGrabberMode(V_SRC_NTSC);
		}
		else if(CurrentSet->nAnalogType == V_SRC_PAL)
		{
			m_pImageInfo->nAnalogWidth  = 704;
			m_pImageInfo->nAnalogHeight = 572;

			m_pImageInfo->nVbackPorch	= PAL_DEFAULT_V_BPCH;
			m_pImageInfo->nHbackPorch	= PAL_DEFAULT_H_BPCH;

			I2cAdcCtrl.SetExtAnalogGrabberMode(V_SRC_PAL);
		}
		else // SECAM
		{
			m_pImageInfo->nAnalogWidth  = 704;
			m_pImageInfo->nAnalogHeight = 572;

			m_pImageInfo->nVbackPorch	= PAL_DEFAULT_V_BPCH;
			m_pImageInfo->nHbackPorch	= PAL_DEFAULT_H_BPCH;

			I2cAdcCtrl.SetExtAnalogGrabberMode(V_SRC_SECAM);
		}

		if(g_nRunningProcessNo == 1)
		{
			::SetEvent(hClientEvents[EVENT_GRAB1_INFO]);
		}
		else if(g_nRunningProcessNo == 2)
		{
			::SetEvent(hClientEvents[EVENT_GRAB2_INFO]);
		}
		else
		{
			::SetEvent(hClientEvents[EVENT_GRAB3_INFO]);
		}
	}
}

long CDATsysView::OnProcessMessage(WPARAM wParam, LPARAM lParam)
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
						CurrentSet->bIsRunMsg = TRUE;
							//     m_pOwner->SendMessage(WM_COMMAND, ID_RUN_RUN);
					}
				}
				break;
			case STOP_BTN1: 
				{
					if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
					{
						CurrentSet->bRunAbort = TRUE;
						m_szResultMsg2.LoadString(IDS_USER_BREAK);
						AddStringToStatus(m_szResultMsg2);
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
						CurrentSet->bIsRunMsg = TRUE;
							//     m_pOwner->SendMessage(WM_COMMAND, ID_RUN_RUN);
					}
				}
				break;
			case STOP_BTN2: 
				{
					if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
					{
						CurrentSet->bRunAbort = TRUE;
						m_szResultMsg2.LoadString(IDS_USER_BREAK);
						AddStringToStatus(m_szResultMsg2);						
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
int CDATsysView::UploadMasterTable(CString strWipid, BOOL bResult, CString sTime)
{
	DWORD dwEventResult = 0;
    clock_t		start;
	MSG msg;
	CString sMsg;
	CString sElemString;

	if(!CurrentSet->bUploadMes) return 0;

	::ResetEvent(m_hReadEvent_S6F2);
	sElemString = gGmesCtrl.MakeElem_S6F1(CurrentSet->sEquipmentID, m_strWipId, bResult, sTime, TRUE);
	if(!gGmesCtrl.SendCommString(sElemString))
	{
		AddStringToStatus("SEND GMES FAIL!!");
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
//	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "08");	
//AUTO
//#ifdef ERR_DEBUG
//	if (m_bWriteMassageFlag)
//	{
//		m_bWriteMassageFlag = 0;
//	}
//	else
//	{
//		m_bWriteMassageFlag = 1;
//	}
//#endif

	if(g_nSysType == AUTO_SYS){
		if(gJigCtrl.m_bAcPowerOnOff){
			gJigCtrl.Set_ACPower(0);
			SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
		}
		else{
			gJigCtrl.Set_ACPower(1);
		}
	}
//
	else{
		if(m_bAcPowerOnOff){
			Set_Relay(0);
		}
		else{
			Set_Relay(1);
		}
	}

}

//void CDATsysView::OnPwrOff() 
//{
	// TODO: Add your command handler code here
//	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "C5");	
//}
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
	// TODO: Add your command handler code here
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "5B");	
	SaveRegistrySetting();
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
	// TODO: Add your command handler code here
	AvSwitchBoxCtrl.SendRemoteCmd(MAX_REMOTE_WAIT_DELAY, "44");	
}


//+add kwmoon 080819
void CDATsysView::OnRefImage() 
{
	// TODO: Add your command handler code here
	OnRunMakeRef();
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

//+del psh 080701
//+add kwmoon 080523
/*void CDATsysView::OnAudioMeasStop() 
{
	// TODO: Add your command handler code here
	if(g_SoundCard.wIn_Flag)
	{
		g_SoundCard.WaveRead_Stop();
		g_SoundCard.~CSoundCard();
	}	
} */

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
				szErrMsg.Format("Failed to open COM port (%s)", sComPort);
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
			szErrMsg.Format("Failed to open COM port (%s)", sComPort);
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
						(int)g_SoundCard.m_fTargetLeftLevel,(int)g_SoundCard.m_fTargetRightLevel,
						(int)g_SoundCard.m_fMeasuredLeftLevel,(int)g_SoundCard.m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)g_SoundCard.m_fTargetLeftFreq,(int)g_SoundCard.m_fTargetRightFreq,
						(int)g_SoundCard.m_fMeasuredLeftFreq,(int)g_SoundCard.m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)g_SoundCard.m_fTargetLeftFreq,(int)g_SoundCard.m_fTargetRightFreq,
						(int)g_SoundCard.m_fTargetLeftLevel,(int)g_SoundCard.m_fTargetRightLevel,
						(int)g_SoundCard.m_fMeasuredLeftFreq,(int)g_SoundCard.m_fMeasuredRightFreq,
						(int)g_SoundCard.m_fMeasuredLeftLevel,(int)g_SoundCard.m_fMeasuredRightLevel);
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
						(int)g_SoundCard.m_fTargetLeftLevel,(int)g_SoundCard.m_fTargetRightLevel,
						(int)g_SoundCard.m_fMeasuredLeftLevel,(int)g_SoundCard.m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("[Audio NG] T(%d,%d) M(%d,%d)",
						(int)g_SoundCard.m_fTargetLeftFreq,(int)g_SoundCard.m_fTargetRightFreq,
						(int)g_SoundCard.m_fMeasuredLeftFreq,(int)g_SoundCard.m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("[Audio NG] T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)g_SoundCard.m_fTargetLeftFreq,(int)g_SoundCard.m_fTargetRightFreq,
						(int)g_SoundCard.m_fTargetLeftLevel,(int)g_SoundCard.m_fTargetRightLevel,
						(int)g_SoundCard.m_fMeasuredLeftFreq,(int)g_SoundCard.m_fMeasuredRightFreq,
						(int)g_SoundCard.m_fMeasuredLeftLevel,(int)g_SoundCard.m_fMeasuredRightLevel);
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
						(int)g_SoundCard.m_fTargetLeftLevel,(int)g_SoundCard.m_fTargetRightLevel,
						(int)g_SoundCard.m_fMeasuredLeftLevel,(int)g_SoundCard.m_fMeasuredRightLevel);
					}
					else if(pCurStep->m_nMeasureAudioType == FREQUENCY_CHECK){ 
						szMsg3.Format("T(%d,%d) M(%d,%d)",
						(int)g_SoundCard.m_fTargetLeftFreq,(int)g_SoundCard.m_fTargetRightFreq,
						(int)g_SoundCard.m_fMeasuredLeftFreq,(int)g_SoundCard.m_fMeasuredRightFreq);
					}
					else{
						szMsg3.Format("T(%d,%d,%d,%d) M(%d,%d,%d,%d)",
						(int)g_SoundCard.m_fTargetLeftFreq,(int)g_SoundCard.m_fTargetRightFreq,
						(int)g_SoundCard.m_fTargetLeftLevel,(int)g_SoundCard.m_fTargetRightLevel,
						(int)g_SoundCard.m_fMeasuredLeftFreq,(int)g_SoundCard.m_fMeasuredRightFreq,
						(int)g_SoundCard.m_fMeasuredLeftLevel,(int)g_SoundCard.m_fMeasuredRightLevel);
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

//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		StopLVDSGrabThread();
		Sleep(100);
		StartLVDSGrabThread();
	}//

	pCurStep->m_bStepInternalRetryFlag = TRUE;

	InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

	for(UINT nTmp = 0; nTmp < CurrentSet->nNoRetry; nTmp++)
	{
		m_bResultDlgActivate = FALSE;

		if(CurrentSet->bSaveProcessingTimeData)
		{
			szMsg.Format("   Internal Retry(%d)",nTmp+1);
			AddTimeData(szMsg);
		}

		g_nNoFailedStep -= 1;
		DeleteResultDataFromGrid(CurrentSet->nDisplayType, pCurStep->GetStepNo());
		InsertMsg2DetailedGrid(pCurStep->m_nStep,_T(""));
		pCurStep->m_strMsg = _T("");

		// Run Current Step Again
		szMsg.Format("Retry#%d Current Step",nTmp+1);
		ShowSubStepMessage(_T(""),szMsg);
		if (g_nUseNoScanType == TRUE)//ScannerCtrl
		{
			ScannerCtrl.SendClear();
		}
		if((nStepResult = StepRun()) == TEST_ABORT)
		{
			m_nNoCurrentRepeatExecution = 1; 
			pCurStep->m_bStepInternalRetryFlag = FALSE;
			
			// goto END_WHILE_LOOP;
			return FALSE;
		}
		else // if TEST_PASS or TEST_FAIL
		{
			if (pCurStep->GetResult() == TRUE)
			{
				break;
			}
			else
			{
				if (g_nUseNoScanType == TRUE)//ScannerCtrl
				{
					ScannerCtrl.SendNG();
				}
			}
		}
	}
	pCurStep->m_bStepInternalRetryFlag = FALSE;

	return TRUE;
}
/*
BOOL CDATsysView::RunInternalRetry_EM() 
{
	int nStepResult = TEST_FAIL;

	CString szMsg = _T("");

	pCurStep_EM->m_bStepInternalRetryFlag = TRUE;

	InsertResultData2Grid_EM(pCurStep_EM->m_nStep);

	for(UINT nTmp = 0; nTmp < CurrentSet->nNoRetry; nTmp++)
	{
		//m_bResultDlgActivate = FALSE;

		g_nNoFailedStep_EM -= 1;
		DeleteResultDataFromGrid_EM(pCurStep_EM->GetStepNo());

		if((nStepResult = StepRun_EM()) == TEST_ABORT)
		{
			pCurStep_EM->m_bStepInternalRetryFlag = FALSE;
			
			return FALSE;
		}
		else // if TEST_PASS or TEST_FAIL
		{
			if(pCurStep_EM->GetResult() == TRUE) break;
		}
	}
	pCurStep_EM->m_bStepInternalRetryFlag = FALSE;

	return TRUE;
}
*/


BOOL CDATsysView::RunUserRetry(int nStepResult)
{
//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		StopLVDSGrabThread();
		Sleep(200);
		StartLVDSGrabThread();
	}//
	else//FHD
	{
		if(!m_bContinuousLVDSGrabRunning)
		{
			SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
		}	
	}//

	PlayWavSound(IDR_WAVE_RETRY);

	ctrlWipIdEdit.SetFocus();
	InsertResultData2Grid(CurrentSet->nDisplayType,pCurStep->m_nStep);

	m_bResultDlgActivate = FALSE;

	// Display User Dialog
	SetStopStepDlgMsg(nStepResult);
	m_StopStepDlg.DoModal();
	HDMIGeneratorCtrl.m_bResetPatternModel = 1;
//FHD
	if(g_nGrabberType == FHD_GRABBER)
	{
		if(m_bContinuousLVDSGrabRunning)
		{
			SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
		}	
	}//
	switch(m_StopStepDlg.m_nRunType)
	{
		case STEP_STOP:
		{
			CurrentSet->bRunAbort = TRUE;
			m_szResultMsg2.LoadString(IDS_USER_BREAK);
			AddStringToStatus(m_szResultMsg2);
			
			g_nRetryStop++;
			return FALSE;
		}
		break;

		case STEP_RETRY:
		{
			g_nUserRetry++;
			if (g_nUseNoScanType == TRUE)//ScannerCtrl
			{
				ScannerCtrl.SendClear();				
			}

			if(pCurStep->m_nStepType != PROCESS_FUNCTION){
				if(!pCurStep->GetResult()) {g_nNoFailedStep -= 1;} 
			}

			DeleteResultDataFromGrid(CurrentSet->nDisplayType, pCurStep->GetStepNo());

			InsertMsg2DetailedGrid(pCurStep->m_nStep, _T(""));
			pCurStep->m_strMsg = _T("");
			pCurStep->m_bStepRetryFlag = TRUE;
			
			// Run Current Step Again
			ShowSubStepMessage(_T(""),"Retry Current Step");
			
			if((nStepResult = StepRun()) == TEST_ABORT)
			{
				m_nNoCurrentRepeatExecution = 1; 
				pCurStep->m_bStepRetryFlag = FALSE;
				
				// goto END_WHILE_LOOP;
				return FALSE;
			}
			if (g_nUseNoScanType == TRUE)//ScannerCtrl
			{
				if (nStepResult == TEST_FAIL)
				{
					ScannerCtrl.SendNG();
				}
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
			//090324
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
	strcpy_s(caAppName, _countof(caAppName), AfxGetApp()->m_pszExeName);
	strcat(caAppName, ".exe");

	DWORD dwInfoSize = GetFileVersionInfoSize(caAppName, NULL);
	LPVOID lpVersionInfo = new BYTE[dwInfoSize];

	GetFileVersionInfo(caAppName, 0, dwInfoSize, lpVersionInfo);
	VerQueryValue(lpVersionInfo, TEXT("\\StringFileInfo\\040904b0\\ProductVersion"), &lpBuffer, &uiLen);

	char *pcVer = new char [uiLen+1];
	strncpy(pcVer, (char *)lpBuffer, uiLen);	// Product Version

/*	m_szVersion.Format("%s",pcVer);
#ifdef _DFT_3IN1
	if(g_nGrabberType == UHD_GRABBER)
	{
		m_szVersion.Format("%s(3in1_UHD)",pcVer);
	}
	else//FHD
	{
		m_szVersion.Format("%s(3in1_FHD)",pcVer);
	}
#endif

#ifdef _DFT_2IN1
*/
#ifdef DEBUG_MD5_CODE__
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(g_nSysType == AUTO_SYS){
			m_szVersion.Format("SM DEBUG %s(UHD_AUTO)",pcVer);
		}
		else{
			m_szVersion.Format("SM DEBUG %s(UHD)",pcVer);
		}
	}
	else//FHD
	{
		if(g_nSysType == AUTO_SYS){
			m_szVersion.Format("SM DEBUG %s(FHD_AUTO)",pcVer);
		}
		else{
			m_szVersion.Format("SM DEBUG %s(FHD)",pcVer);
		}
	}

#else
	if (g_nGrabberType == UHD_GRABBER)
	{
		if (g_nSysType == AUTO_SYS) {
			m_szVersion.Format("%s(UHD_AUTO)", pcVer);
		}
		else {
			m_szVersion.Format("%s(UHD)", pcVer);
		}
}
	else//FHD
	{
		if (g_nSysType == AUTO_SYS) {
			m_szVersion.Format("%s(FHD_AUTO)", pcVer);
		}
		else {
			m_szVersion.Format("%s(FHD)", pcVer);
		}
	}

#endif

//	m_szCurrentStatus	= m_szVersion;
#ifdef __DEBUG_SCAN_SN_NO_WRITE
	m_szVersion += " ***** SCAN NO WRITE TEST VERSION ***** ";
#endif

	CMainFrame* pMainFrame =(CMainFrame*)AfxGetMainWnd();
	pMainFrame->SetTitle(m_szVersion);

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

	g_GrabSource = LVDS_GRABBER;
	g_ImageProc.SetRoi(ptZeroPosition,ptZeroPosition); 

	//+add kwmoon 080828
	g_ImageProc.m_bInvalidateInvalidRegion = FALSE;
}

UINT CDATsysView::GrabImageThread(LPVOID pParam)
{
//	BOOL bGrabbing = FALSE;

	CDATsysView* pView = (CDATsysView*)pParam;

	int nWidth		= 0;
	int nHeight		= 0;
	int nImageSize	= 0;
	int nErrorCode	= 0;
	int nBitCount	= 0;
//	int nRowWidth   = 0;
	int	nNewWidth, nNewHeight;

	BYTE* pImgBuf8 = NULL;
	unsigned char *bufTmp;
//	BYTE* pImgBuf8_2 = NULL;

	int	nCurIdx, nPrvIdx = 0;
	int	nFirst;
//	int	i; //, j, k;
	int nNoImage = 0;
//	MSG msg;
//	BYTE* pBits	= NULL;
//	int bpl		= ((nWidth*3+3)/4)*4;
//	UINT tSize  = bpl * nHeight;


	::ResetEvent(pView->m_hGrabThreadKillEvent);

	pView->m_bGrabThreadRunning = TRUE;


	nFirst		=	1;


//	pImgBuf8_2	=	new unsigned char [3840*2160*3];
	nBitCount	= 24;
	if(g_GrabSource == LVDS_GRABBER){
		if(CurrentSet->nUHD_Type == 8){
			nWidth		 = 3072*2; // 3840;
		}
		else{
			nWidth		 = CurrentSet->nLvdsWidth; // 3840;
		}
		nNewWidth		= CurrentSet->nLvdsWidth;
		nHeight		= CurrentSet->nLvdsHeight;
		nImageSize  = WIDTHBYTES(nBitCount*nNewWidth) * nHeight;
		g_GrabDisplayImage.InitHeaders(COLOR_24BIT,nNewWidth,nHeight,nImageSize);
		g_GrabImage.InitHeaders(COLOR_24BIT,nNewWidth,nHeight,nImageSize);
	}
	else if(g_GrabSource == HDMI_GRABBER){
		nWidth		= CurrentSet->nHdmiWidth;
		nHeight		= CurrentSet->nHdmiHeight;
		nImageSize  = WIDTHBYTES(nBitCount*nWidth) * nHeight;
		g_GrabDisplayImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);
		g_GrabImage.InitHeaders(COLOR_24BIT,nWidth,nHeight,nImageSize);
	}
	else{
		if(CurrentSet->nAnalogType == 0){nWidth = 768;	nHeight	= 624;	} //NTSC
		else{nWidth	= 768;	nHeight	= 630;}  //PAL, SECAM

		nNewWidth	= CurrentSet->nAnalogWidth;
		nNewHeight	= CurrentSet->nAnalogHeight;
		nImageSize  = WIDTHBYTES(nBitCount*nNewWidth) * nNewHeight;
		g_GrabDisplayImage.InitHeaders(COLOR_24BIT,nNewWidth,nNewHeight,nImageSize);
		g_GrabImage.InitHeaders(COLOR_24BIT,nNewWidth,nNewHeight,nImageSize);
	} 

	bufTmp = (unsigned char *) malloc (nWidth * nHeight * 3);
	memset(g_GrabDisplayImage.m_pImageData,0x00,nImageSize);
	SystemMonitorLog_Save("GrabImageThread", _T(__FILE__), __LINE__);
	int lRotateProcess = 0;
	while(!pView->m_bStopLVDSThread)
	{
		lRotateProcess = 0;
		pView->m_clsPCI.CM_ChkGrabDone(&nCurIdx);
		if (nFirst==1) {
			nCurIdx = 0;
			nFirst = 0;
		}

		g_GrabDisplayImage.m_bImageLoaded = FALSE;


	//	if (nCurIdx > nPrvIdx ) {
		if (nCurIdx > nPrvIdx ) {
			
			pView->m_clsPCI.CM_ImageCopySafe(nCurIdx, nWidth*3*nHeight, (int**) &pImgBuf8);
			//FILE *FP1;
			//FP1 = fopen("c:\\test.dat", "wb");
			//fwrite((void*) pImgBuf8, 1, nWidth*3*nHeight, FP1);
			//fclose (FP1);

			if(g_GrabSource == LVDS_GRABBER)
			{
				if (CurrentSet->nUHD_Type == PACK_TYPE_UHDEPI) {		// EPI Mode
	//frame->m_clsPCI.DFT3_EPIPuzzle(frame->m_nImgMode, imgBuf8, /*frame->m_pDoc->m_pImage*/frame->m_pEPIRGBImage, frame->m_pEPIWImage, nWidth, nHeight, frame->m_bRotate180);	// 150311 rotate추가
					g_pView->m_clsPCI.DFT3_EPIPuzzle(pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, 0);
					//memcpy(frame->m_pDoc->m_pImage, frame->m_pEPIRGBImage, nWidth*nHeight*3* sizeof(unsigned char)  );
				}
				else if (CurrentSet->nUHD_Type == PACK_TYPE_CMVIEW) {		// CM VIEW
					g_pView->m_clsPCI.DFT3_CMVPuzzle(pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation);
				}
				else {

					if (CurrentSet->nRedBlueSwap)
					{
						g_ImageProc.RedBlueSwap(pImgBuf8, nWidth, nHeight, CurrentSet->nRedBlueSwap);
					}


				//if (CurrentSet->nUHD_Type == 7) {		// EPI Mode
				//	pView->m_clsPCI.DFT3_EPIPuzzle( pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, 0);
				//}
				//else {
					
 					if (CurrentSet->nUHD_Grab_Mode == 9) {
						pView->m_clsPCI.DFT3_UHDPuzzle(CurrentSet->nUHD_Grab_Mode, pImgBuf8, bufTmp, nWidth, nHeight, CurrentSet->nImageRotation);	
 						pView->m_clsPCI.DFT3_WhiteMerge(CurrentSet->nUHD_Grab_Mode, bufTmp, g_GrabDisplayImage.m_pImageData, nWidth/2, nHeight);//
 					}
					else if (CurrentSet->nUHD_Grab_Mode == 11) {
						pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P60_CEDS20Y_Mapping(bufTmp, g_GrabDisplayImage.m_pImageData, nWidth, nHeight);

						g_ImageProc.Rotate(g_GrabDisplayImage, (float)CurrentSet->nImageRotation);
						lRotateProcess = 1;
					}
					else if ((CurrentSet->nUHD_Grab_Mode == 12) || (CurrentSet->nUHD_Grab_Mode == 14)) //|| ((CurrentSet->nUHD_Grab_Mode >= 14) && (CurrentSet->nUHD_Grab_Mode <= 17))) //201106 else if (CurrentSet->nUHD_Grab_Mode == 12)
					{	
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P68_50_UPQO_VA_CEDS_HKC(bufTmp, g_GrabDisplayImage.m_pImageData, nWidth, nHeight);

						g_ImageProc.Rotate(g_GrabDisplayImage, (float)CurrentSet->nImageRotation);

					}
					else if (CurrentSet->nUHD_Grab_Mode == 15) //( (CurrentSet->nUHD_Grab_Mode >= 14)&&(CurrentSet->nUHD_Grab_Mode <= 17))) //201106 else if (CurrentSet->nUHD_Grab_Mode == 12)
					{						
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P68_55UP77_CSOT(bufTmp, g_GrabDisplayImage.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(g_GrabDisplayImage, (float)CurrentSet->nImageRotation);

					}
					else if (CurrentSet->nUHD_Grab_Mode == 13) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P68_70_UPQO_Sharp_CEDS_DPT(bufTmp, g_GrabDisplayImage.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(g_GrabDisplayImage, (float)CurrentSet->nImageRotation);
						//	lRotateProcess = 1;
					}
					else if (CurrentSet->nUHD_Grab_Mode == 16) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.P22Y8K_TCONLESS(bufTmp, g_GrabDisplayImage.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(g_GrabDisplayImage, (float)CurrentSet->nImageRotation);
						//	lRotateProcess = 1;
					}
					else if (CurrentSet->nUHD_Grab_Mode == 17) {
						g_pView->m_clsPCI.DFT3_UHDPuzzle(2, pImgBuf8, bufTmp, nWidth, nHeight, 0);
						g_ImageProc.LM21A_HKC_220307(bufTmp, g_GrabDisplayImage.m_pImageData, nWidth, nHeight);
						g_ImageProc.Rotate(g_GrabDisplayImage, (float)CurrentSet->nImageRotation);
						//	lRotateProcess = 1;
					}
					else if (CurrentSet->nUHD_Grab_Mode == 18) {

						g_ImageProc.DFT3_UHDPuzzleLocal(CurrentSet->nUHD_Grab_Mode, pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation);

					}
					else {
#ifdef SM_MODIFY_CODE__	
						pView->m_clsPCI.DFT3_UHDPuzzle(CurrentSet->nUHD_Grab_Mode, pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, 0);
						g_ImageProc.Rotate(g_GrabDisplayImage,(float)CurrentSet->nImageRotation);
						lRotateProcess = 1;
#else
						pView->m_clsPCI.DFT3_UHDPuzzle(CurrentSet->nUHD_Grab_Mode, pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, CurrentSet->nImageRotation);
#endif
					}
					
					//if (CurrentSet->nRedBlueSwap)
					//{
					//	g_ImageProc.RedBlueSwap(g_GrabDisplayImage.m_pImageData, nWidth, nHeight, CurrentSet->nRedBlueSwap);
					//}

					if (CurrentSet->nUHD_Y20_SW_Mode == 3)
					{
						g_ImageProc.SW_MODE_Mapping(g_GrabDisplayImage.m_pImageData, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, CurrentSet->nUHD_Y20_SW_Mode);
					}
					
	/*		
					if (CurrentSet->nDeHsyncEnable || CurrentSet->nHyncEdgeEnable)
					{
						g_ImageProc.HsyncModeChange(CurrentSet->nDeHsyncEnable, CurrentSet->nHyncEdgeEnable);
					}
	*/
				}
			//	memcpy(g_GrabDisplayImage.m_pImageData, pImgBuf8_2, nWidth*3*nHeight);
			}
			else if(g_GrabSource == HDMI_GRABBER)
			{
				pView->m_clsPCI.DFT3_HDMIPuzzle(pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, 0);
			//	memcpy(g_GrabDisplayImage.m_pImageData, pImgBuf8_2, nWidth*3*nHeight);
			}
			else{
				pView->m_clsPCI.DFT3_AnalogPuzzle(CurrentSet->nAnalogType, pImgBuf8, g_GrabDisplayImage.m_pImageData, nWidth, nHeight, &nNewWidth, &nNewHeight, 0);
			//	memcpy(g_GrabDisplayImage.m_pImageData, pImgBuf8_2, nWidth*3*nHeight);
			}
			g_GrabDisplayImage.m_bImageLoaded = TRUE;
			pView->PostMessage(UM_UPDATE_GRAB_STATUS,0,0);

			if(WaitForSingleObject(hGrabEvent,100) == WAIT_OBJECT_0)
			{
				//g_GrabImage.m_pImageData = g_GrabDisplayImage.m_pImageData;
				memcpy(g_GrabImage.m_pImageData, g_GrabDisplayImage.m_pImageData, nWidth * 3 * nHeight);
				g_GrabImage.m_bImageLoaded = TRUE;
				::ResetEvent(hGrabEvent);
				::SetEvent(hGrabEvent_Complete);
				if (lRotateProcess != 1)
				{
					g_GrabImage.m_bImageLoaded = TRUE;
				}
			}
			Sleep(80);
			
			nPrvIdx = nCurIdx;
			nNoImage = 0;
		}
		else {
			if(nNoImage < 500){nNoImage++;}
			if(nNoImage > 14){
#if 1 //230712 Poland 
				memset(g_GrabDisplayImage.m_pImageData, 0xff, nImageSize * 3);
#else
				if (g_GrabSource == LVDS_GRABBER)
				{
					memset(g_GrabDisplayImage.m_pImageData, 0x00, nImageSize * 3);
				}
				else
				{
					memset(g_GrabDisplayImage.m_pImageData, 0xff, nImageSize * 3);
				}
#endif
				g_GrabDisplayImage.m_bImageLoaded = TRUE;
				pView->PostMessage(UM_UPDATE_GRAB_STATUS,0,0);

				if(WaitForSingleObject(hGrabEvent,100) == WAIT_OBJECT_0)
				{
					memcpy(g_GrabImage.m_pImageData, g_GrabDisplayImage.m_pImageData, nWidth * 3 * nHeight); //g_GrabImage.m_pImageData = g_GrabDisplayImage.m_pImageData;
					g_GrabImage.m_bImageLoaded = TRUE;
					::ResetEvent(hGrabEvent);
					::SetEvent(hGrabEvent_Complete);
				}
			}
			nPrvIdx = nCurIdx;
			Sleep(200);
		}
	}

	free(bufTmp);
	SystemMonitorLog_Save("GrabImageThread", _T(__FILE__), __LINE__);
	pView->m_bStopLVDSThread = FALSE;

	pView->m_bGrabThreadRunning = FALSE;

	::SetEvent(pView->m_hGrabThreadKillEvent);
	
	return TRUE;
}
UINT CDATsysView::GrabImageThread_FHD(LPVOID pParam)
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
		
		if((nGrabResult = pView->Grab_FHD(GRAB_WITH_NO_RETRY)) != GRAB_PASS)
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

LRESULT CDATsysView::UpdateGrabImage(WPARAM wParam, LPARAM lParam)
{
	CRect nImageRect;
	CWnd* pImageWnd;

	UpdateData(FALSE);
	
	pImageWnd = GetDlgItem(IDC_VIDEOVIEW);
	pImageWnd->GetWindowRect(&nImageRect);

	InvalidateRect(nImageRect, FALSE);
//	DrawImage(IDC_VIDEOVIEW,g_GrabDisplayImage);
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(g_GrabDisplayImage.m_bImageLoaded)
		{
			DrawImage(IDC_VIDEOVIEW,g_GrabDisplayImage);
		}
	}
	else //FHD
	{
		if(g_GrabImage.m_bImageLoaded)
		{
			DrawImage(IDC_VIDEOVIEW,g_GrabImage);
		}
	}//					

	pImageWnd->UpdateWindow();
//	UpdateWindow();
	return 0;
}

int CDATsysView::DFT3_UHDGrabStartLocal(int nModel, int nShiftVal, int nWidth, int nHeight, int nOffset, int nDelay, int nLvdsType)
{
	int lRet;

	int			addr[2], data[2];
	int			nGrabSize;
	int			nChInfo;
	__int64		n64PushOrder;
	__int64		n64BitSpeed;
	__int64		n64MemRdInfo;
	__int64		n64MemWrInfo;
	int			nChOffset;


	nGrabSize = ((nWidth*nHeight * 3) + 7) & 0xfffffff8;

	nChInfo = (nModel == PACK_TYPE_UHDFULL) ? 4 :
		(nModel == PACK_TYPE_UHDEPI) ? 4 :

		(nModel == PACK_TYPE_CMVIEW) ? 4 :	//	SJ 150821 ?

		(nModel == PACK_TYPE_UHDNORMAL) ? 2 :
		(nModel == PACK_TYPE_1_60HZ) ? 1 :
		(nModel == PACK_TYPE_1_120HZ) ? 2 :
		(nModel == PACK_TYPE_10) ? 2 :
		(nModel == PACK_TYPE_8) ? 1 :
		(nModel == PACK_TYPE_5_HD_Rev) ? 1 :
		(nModel == PACK_TYPE_5_Rev_2019) ? 1 :
		(nModel == PACK_TYPE_5_Rev_2020) ? 1 :
		(nModel == PACK_TYPE_5) ? 1 :
		(nModel == PACK_TYPE_UHDFULL_LOWSPEED) ? 4 :		//	230315 Low Speed Mode 추가
		(nModel == PACK_TYPE_QHD_FULL_SPEED) ? 4 :
		(nModel == PACK_TYPE_120HZ_LOW_SPEED) ? 2 :
		2;
		//(nModel == PACK_TYPE_5) ? 1 : 2;


	nChOffset = nWidth * 3 / nChInfo;

	n64MemWrInfo = ((((nChOffset))) | (((nChOffset * 8 / 512) - 1) << 16)) | ((__int64)nChInfo << 32);

	n64MemRdInfo = (__int64)nChOffset | ((__int64)nHeight << 32);
	n64PushOrder = ((nModel == PACK_TYPE_UHDFULL) || (nModel == PACK_TYPE_UHDNORMAL) 
		|| (nModel == PACK_TYPE_UHDEPI) 
		|| (nModel == PACK_TYPE_CMVIEW) 
		|| (nModel == PACK_TYPE_UHDFULL_LOWSPEED)
		|| (nModel == PACK_TYPE_QHD_FULL_SPEED)
		) ? 0x72 : 0x1b;	// lvds speed gbps
//	n64BitSpeed		=	((nModel == PACK_TYPE_UHDFULL) | (nModel == PACK_TYPE_UHDNORMAL)) ? 0x1120	:	0x450	;	// lvds speed gbps
	n64BitSpeed = ((nModel == PACK_TYPE_UHDFULL) || (nModel == PACK_TYPE_UHDNORMAL)) ? 0x1120 :
		((nModel == PACK_TYPE_UHDEPI) || (nModel == PACK_TYPE_UHDFULL_LOWSPEED) || (nModel == PACK_TYPE_QHD_FULL_SPEED)) ? 0x525 :
		(nModel == PACK_TYPE_120HZ_LOW_SPEED) ? 0x400 : 0x450;	// lvds speed gbps	//	230315 Low Speed Mode 추가) ? 0x525 : 0x36;	// lvds speed gbps

	if (nModel == PACK_TYPE_8) n64BitSpeed |= 0x10f0000;
	else if (nModel == PACK_TYPE_5) n64BitSpeed |= 0x110f0000;
	else if (nModel == PACK_TYPE_5_HD_Rev) n64BitSpeed |= 0x110f0000;
	else if (nModel == PACK_TYPE_5_Rev_2019) n64BitSpeed |= 0x113f0000;
//201125	
	else if (nModel == PACK_TYPE_5_Rev_2020)
	{
		__int64 lvds_de_to_hsync = 0x00400000;
		__int64 lvds_hsync_edge_en = 0x00800000;
		n64BitSpeed |= 0x113f0000 | lvds_de_to_hsync | lvds_hsync_edge_en;
	}
//END 201125

	lRet = m_clsPCI.CM_MemInit(nGrabSize, 5000);

	m_clsPCI.CM_RegisterWrite(0x0040, nDelay);
	m_clsPCI.CM_RegisterWrite(0x0130, n64MemWrInfo);		Sleep(1);
	m_clsPCI.CM_RegisterWrite(0x0140, n64MemRdInfo);		Sleep(1);
	m_clsPCI.CM_RegisterWrite(0x0150, n64PushOrder);		Sleep(1);

	// 	int nLVDSMapMode;
	// 
	// 	nLVDSMapMode = 0x2;
	//	CM_RegisterWrite(0x0160, n64BitSpeed | 0x100000);		Sleep(1);
	//	CM_RegisterWrite(0x0160, n64BitSpeed | 0x200000);		Sleep(1);
	m_clsPCI.CM_RegisterWrite(0x0160, n64BitSpeed | (nLvdsType << 20));		Sleep(1);//n64BitSpeed |= 0x113f0000;
///	CM_RegisterWrite(0x0160, n64BitSpeed);		Sleep(1);
	//if (CurrentSet->nUHD_Grab_Mode == 14) //201106 HKC_966_PIXEL
	//{		
	//	__int64 SW_Mode_Sel;
	//	SW_Mode_Sel = 0x00000001;
	//	m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
	//	Sleep(1); // SW_Mode_Sel
	//}
	////if (CurrentSet->nUHD_Grab_Mode == 14) //if ((CurrentSet->nUHD_Grab_Mode >= 14) && (CurrentSet->nUHD_Grab_Mode <= 17)) //201109 HKC_966_PIXEL
	//{
	//	__int64 SW_Mode_Sel;
	//	if ((CurrentSet->nUHD_Y20_SW_Mode >= 0) && (CurrentSet->nUHD_Y20_SW_Mode <= 7)) // Y20_SW_Mode_01
	//		SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode;
	//	else
	//		SW_Mode_Sel = 0x00000000;

	//	m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
	//	Sleep(1); // SW_Mode_Sel
	//}

	m_clsPCI.CM_RegisterWrite(0x0020, 0x1fffff);		Sleep(1);
	//	CM_RegisterWrite(0x0020, 0);			Sleep(1);		//	150226 삭제
	int	nChRstRelease = (nChInfo == 1) ? 0x0ffffc :
		(nChInfo == 2) ? 0x0ffff0 :
		(nChInfo == 3) ? 0x0fffc0 :
		(nChInfo == 4) ? 0x0fff00 :
		(nChInfo == 5) ? 0x0ffc00 : 0x0fffff;	// 150226 추가
	m_clsPCI.CM_RegisterWrite(0x0020, nChRstRelease);			Sleep(1);		// 150226 추가

	if ((nModel == PACK_TYPE_UHDFULL) || (nModel == PACK_TYPE_UHDNORMAL) || (nModel == PACK_TYPE_UHDEPI)
		|| (nModel == PACK_TYPE_CMVIEW)||(nModel == PACK_TYPE_UHDFULL_LOWSPEED) || (nModel == PACK_TYPE_QHD_FULL_SPEED))
	{
		m_clsPCI.CM_RegisterWrite(0x0050, 1);			Sleep(1); 
	}
	else 
	{
		m_clsPCI.CM_RegisterWrite(0x0050, 0);			Sleep(1); 
	}

	m_clsPCI.CM_RegisterWrite(0x0110, ((__int64)((__int64)nShiftVal << 48) | ((__int64)(nOffset) << 32) | (__int64)nGrabSize));		Sleep(1);

	m_clsPCI.CM_RegisterWrite(0x0010, (__int64)0x2);	Sleep(1);
	m_clsPCI.CM_RegisterWrite(0x0010, (__int64)0x0);	Sleep(1);
	m_clsPCI.CM_RegisterWrite(0x0010, (__int64)0x1);	Sleep(1);
	m_clsPCI.CM_RegisterWrite(0x0100, (__int64)0x0);	Sleep(1);
	m_clsPCI.CM_RegisterWrite(0x0100, (__int64)0x2);	Sleep(1);

	return lRet;
}
int CDATsysView::IF_Pack_Reset()
{
	//SW Reset (0x0170[16] active high)

	__int64		Rdata, Wdata;
	Rdata = 0; Wdata = 0;
	m_clsPCI.CM_RegisterRead(0x0170, &Rdata);		Sleep(1);	//SW Mode Read
	Wdata = Rdata | 0x10000;
	m_clsPCI.CM_RegisterWrite(0x0170, Wdata);		Sleep(1);	//Reset Active
	Wdata = Rdata & 0x0ffff;
	m_clsPCI.CM_RegisterWrite(0x0170, Wdata);		Sleep(1);	//Reset Release


	return 1;
}

int CDATsysView::CheckGrabberStatus()
{


	int lrtn, addr;
	__int64 wd;
	unsigned int img_cnt1, img_cnt2;

	CString str;

	addr = 0x060;	lrtn = m_clsPCI.CM_RegisterRead(addr, &wd);		//	Image Index 읽기
	img_cnt1 = 0xffffffff & (wd >> 0);

	str.Format(_T("PRI CHK - Image Index1 : %08x "), img_cnt1);		//OutputDebugString(str);	//	이 메시지를 로그로 남겨야 합니다.
	for (int i = 0; i < 10; i++)
	{
		Sleep(20);
		addr = 0x060;	lrtn = m_clsPCI.CM_RegisterRead(addr, &wd);		//	Image Index 읽기
		img_cnt2 = 0xffffffff & (wd >> 0);
		str.Format(_T("PRI CHK - Image Index2 : %08x "), img_cnt2);		//OutputDebugString(str);	//	이 메시지를 로그로 남겨야 합니다.
		if (img_cnt1 != img_cnt2)
		{
			break;
		}
	}
	// 여기에서 차이를 구해서 영상이 끊겼는지 확인가능
	if (img_cnt1 == img_cnt2)
	{
		str.Format(_T("PRI CHK -img_cnt1 == img_cnt2 : %08x "), img_cnt2);
		AddStringToStatus(str);
		//	End of 1. Image Index 읽기

		//	2. 그래버 버퍼 상태정보 읽어보기
	//	int lrtn, addr;
	//	__int64 wd;
		unsigned int buff_status;
		addr = 0x000;	lrtn = m_clsPCI.CM_RegisterRead(addr, &wd);		//	Image Index 읽기
		buff_status = 0xffffffff & (wd >> 32);

		str.Format(_T("PRI CHK - Buffer Status : %08x "), buff_status);		OutputDebugString(str);	//	이 메시지를 로그로 남겨야 합니다.
		AddStringToStatus(str);
		//	End of 2. 그래버 버퍼 상태정보 읽어보기
		return 0;
	}
	else
	{

		//	2. 그래버 버퍼 상태정보 읽어보기
	//	int lrtn, addr;
	//	__int64 wd;
		unsigned int buff_status;
		addr = 0x000;	lrtn = m_clsPCI.CM_RegisterRead(addr, &wd);		//	Image Index 읽기
		buff_status = 0xffffffff & (wd >> 32);

		str.Format(_T("PRI CHK - Buffer Status : %08x "), buff_status);	//	OutputDebugString(str);	//	이 메시지를 로그로 남겨야 합니다.
		AddStringToStatus(str);
		//	End of 2. 그래버 버퍼 상태정보 읽어보기

		return 1;
	}
	   	
}
void CDATsysView::StartLVDSGrabThread()
{
	BOOL bFlag = FALSE;
	int	lRet;
	int nModel;
	int nWidth;
	int nHeight;
	int nGrabSize;
	int nShiftVal;
	int nImageOffset;
	int nGrabDelay;
	int	nLvdsType = 0;

	if (CurrentSet->bPJT_GrabDisable == 1)
		return;

	if(!m_bGrabConnection)
	{
		if (g_nSysRobotType == AUTO_SYS)//AUTO_SYS
		{
			AddStringToStatus("Optic I/F  Not Connect...");
		}
		else
		{
			AfxMessageBox("Optic I/F  Not Connect...");
		}
		return;
	}

	//
	if (CurrentSet->bPJT_GrabDisable == 0)
	{
		if (g_nSysRobotType == AUTO_SYS)//AUTO_SYS
		{
			if (!GrabberReset())
			{
				if (g_nSysType == AUTO_SYS)
				{
					if (!CurrentSet->bIsRunning)
					{
					}
					else
					{
						if ((CurrentSet->bCheckGrabACPower == 1)&&(g_nResetGrabMode == 1))
						{
							AddStringToStatus("Grabber Power OFF");
							gJigCtrl.Set_Grabber_Power(0);
							Sleep(1000);

							AddStringToStatus("Grabber Power ON");
							gJigCtrl.Set_Grabber_Power(1);
							Sleep(2000);

						}
						else if (CurrentSet->nAuto_Grab_Reset == 1)
						{
							AddStringToStatus("Grabber Power OFF");
							gJigCtrl.Set_Grabber_Power(0);
							Sleep(1000);

							AddStringToStatus("Grabber Power ON");
							gJigCtrl.Set_Grabber_Power(1);
							Sleep(2000);

						}
					}
				}
				if (!GrabberReset())
				{
					if (g_nSysRobotType == AUTO_SYS)//AUTO_SYS
					{
						AddStringToStatus("Grabber  Not Connect...");
					}
					else
					{
						AfxMessageBox("Grabber  Not Connect...");
					}

				}
				else
				{
					AddStringToStatus("Grabber Recover OK");
				}
				//return;
			}
		}
		else
		{
			GrabberReset();
		}
	}

	nGrabDelay = CurrentSet->nUHD_Grab_Delay * 50;
	//=========================
	// Start Auto-Grab Thread
	//=========================
	if(!m_bGrabThreadRunning)
	{ 
		CurrentSet->nGrabSource = g_GrabSource;
		if(g_GrabSource == LVDS_GRABBER)
		{
			nModel		 = CurrentSet->nUHD_Type;
			if(nModel == 8){
				nWidth		 = 3072*2; // 3840;
			}
			else{
				nWidth		 = CurrentSet->nLvdsWidth; // 3840;
			}
			nHeight		 = CurrentSet->nLvdsHeight; // 2160;
			nGrabSize	 = ((nWidth * nHeight * 3) + 7) & 0xfffffff8;
			nShiftVal	 = CurrentSet->nUHD_Grab_BitShift;
			nLvdsType	 = CurrentSet->nLVDS_Mode;
			nImageOffset = nWidth * 3;
		}
		else if(g_GrabSource == HDMI_GRABBER)
		{
			nWidth		 = CurrentSet->nHdmiWidth; 
			nHeight		 = CurrentSet->nHdmiHeight; 
			nGrabSize	 = ((nWidth * nHeight * 3) + 7) & 0xfffffff8;
			nShiftVal	 = CurrentSet->nHDMI_Grab_BitShift;
			nModel		 = CurrentSet->nHDMI_Grab_Resolution;
			nImageOffset = nWidth * 3;
			lRet			=	g_pView->m_clsPCI.CM_RegisterWrite(0x00110, nGrabSize);
			lRet			=	g_pView->m_clsPCI.CM_MemInit(nGrabSize, 5000);
		}
		else{
			if(CurrentSet->nAnalogType == 0){nWidth = 768;	nHeight	= 624;	} //NTSC
			else{nWidth	= 768;	nHeight	= 630;}  //PAL, SECAM
			nGrabSize	 = ((nWidth * nHeight * 3) + 7) & 0xfffffff8;
			lRet			=	g_pView->m_clsPCI.CM_RegisterWrite(0x00110, nGrabSize);
			lRet			=	g_pView->m_clsPCI.CM_MemInit(nGrabSize, 5000);
		}

		
		m_bGrabThreadRunning = TRUE;
		m_pGrabImageThread = AfxBeginThread(GrabImageThread,this);

		if(g_GrabSource == LVDS_GRABBER)
		{
			if ((nModel == PACK_TYPE_5_Rev_2019) 
				|| (nModel == PACK_TYPE_5_Rev_2020)
				|| (nModel == PACK_TYPE_UHDFULL_LOWSPEED) 
				|| (nModel == PACK_TYPE_QHD_FULL_SPEED)
				|| (nModel == PACK_TYPE_120HZ_LOW_SPEED))
			{
				lRet = DFT3_UHDGrabStartLocal(nModel, nShiftVal, nWidth, nHeight, nImageOffset, nGrabDelay, nLvdsType);
			}
			else
			{
				lRet = m_clsPCI.DFT3_UHDGrabStart(nModel, nShiftVal, nWidth, nHeight, nImageOffset, nGrabDelay, nLvdsType);
			}
		
			//if (CurrentSet->nUHD_Grab_Mode == 14) //if ((CurrentSet->nUHD_Grab_Mode >= 14) && (CurrentSet->nUHD_Grab_Mode <= 17)) //201109 HKC_966_PIXEL
			{
				__int64 SW_Mode_Sel;
				if (CurrentSet->nUHD_Y20_SW_Mode >= 8)
				{
					SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode - 8;
				}
				else
				{
					SW_Mode_Sel = CurrentSet->nUHD_Y20_SW_Mode;
				}

				if ((SW_Mode_Sel >= 0) && (SW_Mode_Sel <= 7)) // Y20_SW_Mode_01
					SW_Mode_Sel = SW_Mode_Sel;
				else
					SW_Mode_Sel = 0x00000000;

				m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
				Sleep(1); 
			}
			//if ((CurrentSet->nUHD_Grab_Mode >= 14) && (CurrentSet->nUHD_Grab_Mode <= 17)) //201109 HKC_966_PIXEL
			//{
			//	__int64 SW_Mode_Sel;
			//	if (CurrentSet->nUHD_Grab_Mode == 14) // Y20_SW_Mode_01
			//		SW_Mode_Sel = 0x00000001;
			//	else if (CurrentSet->nUHD_Grab_Mode == 15) // Y20_SW_Mode_00
			//		SW_Mode_Sel = 0x00000000;
			//	else if (CurrentSet->nUHD_Grab_Mode == 16) // Y20_SW_Mode_02
			//		SW_Mode_Sel = 0x00000002;
			//	else if (CurrentSet->nUHD_Grab_Mode == 17) // Y20_SW_Mode_03
			//		SW_Mode_Sel = 0x00000003;
			//	else
			//		SW_Mode_Sel = 0x00000001;

			//	m_clsPCI.CM_RegisterWrite(0x0170, SW_Mode_Sel);
			//	Sleep(1); // SW_Mode_Sel
			//}
			//{
			//	Sleep(1);
			//	long long n64BitSpeed = ((nModel == PACK_TYPE_UHDFULL) | (nModel == PACK_TYPE_UHDNORMAL)) ? 0x1120 :
			//		((nModel == PACK_TYPE_UHDEPI)) ? 0x525 : 0x36;	// lvds speed gbps
			//	m_clsPCI.CM_RegisterWrite(0x0160, n64BitSpeed | (nLvdsType << 20));		Sleep(1);//n64BitSpeed |= 0x113f0000;
			//}
		}
		else if(g_GrabSource == HDMI_GRABBER)
		{
			lRet = m_clsPCI.DFT3_HDMIGrabStart(nModel, nWidth, nHeight, nShiftVal);
		}
		else{
			lRet = m_clsPCI.DFT3_AnalogGrabStart(0, nWidth, nHeight);
		}

		if (CurrentSet->nDeHsyncEnable || CurrentSet->nHyncEdgeEnable)
		{
			g_ImageProc.HsyncModeChange(CurrentSet->nDeHsyncEnable, CurrentSet->nHyncEdgeEnable);
		}

	}
	return;
}

void CDATsysView::StopLVDSGrabThread()
{
	DWORD dwThreadResult = 0;
	DWORD dwExitCode = 0;

	DWORD	dwCode=0;

	m_clsPCI.DFT3_UHDGrabStop();


	if(m_bGrabThreadRunning)
	{
		m_bStopLVDSThread = TRUE; 

	//	UpdateData(FALSE);
	//	ShowSystemMsg(STOP_GRAB_PROCESSING_MSG); 
		dwThreadResult = WaitForSingleObject(m_hGrabThreadKillEvent,2000);

		int nLastError = GetLastError();

		if(dwThreadResult != WAIT_OBJECT_0)
		{
			if(m_pGrabImageThread) TerminateThread(m_pGrabImageThread->m_hThread,dwExitCode);
		}

		m_pGrabImageThread = NULL; 

		CloseSystemMsg(); 
	
		m_bGrabThreadRunning = FALSE;
	//	UpdateData();

		::ResetEvent(m_hGrabThreadKillEvent);


	}
}
void CDATsysView::StartLVDSGrabThread_FHD()
{
	BOOL bFlag = FALSE;

	if (CurrentSet->bPJT_GrabDisable == 1)
		return;
	//=========================
	// Stop Auto-Grab by Timer
	//=========================
	if(m_bContinuousLVDSGrabRunning)
	{
		SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
	}

	//=========================
	// Start Auto-Grab Thread
	//=========================
	if(!m_bGrabThreadRunning)
	{ 
		m_bGrabThreadRunning = TRUE;
		m_pGrabImageThread = AfxBeginThread(GrabImageThread_FHD,this);

		//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
		CString szString;
		szString.Format("GrabImageThread %x\n",m_pGrabImageThread->m_nThreadID);
//		if(g_LogFileOpen) g_LogFile.WriteString(szString);
#endif
	}

	::ResetEvent(m_hGrabThreadKillEvent);

}

void CDATsysView::StopLVDSGrabThread_FHD()
{
	DWORD dwThreadResult = 0;
	DWORD dwExitCode = 0;

	//=======================
	// Stop Auto-Grab Thread
	//=======================
	if(m_bGrabThreadRunning)
	{
		m_bStopLVDSThread = TRUE; 

		UpdateData(FALSE);
		ShowSystemMsg(STOP_GRAB_PROCESSING_MSG); 
		dwThreadResult = WaitForSingleObject(m_hGrabThreadKillEvent,2000);

		int nLastError = GetLastError();

		if(dwThreadResult != WAIT_OBJECT_0)
		{
		//	AfxMessageBox("Failed to stop grab process!");
			
			//+change 090213(Modification No1)
		//	TerminateThread(m_hThread,dwExitCode);
			if(m_pGrabImageThread) TerminateThread(m_pGrabImageThread->m_hThread,dwExitCode);
		}

		//+add 090224(Modification No2)
		m_pGrabImageThread = NULL; 

		CloseSystemMsg(); 
	
		m_bGrabThreadRunning = FALSE;
		UpdateData();

	}
}

void CDATsysView::OnGrabStart() 
{
//	BOOL bFlag;
	if (CurrentSet->bPJT_GrabDisable == 1)
		return;
//	bFlag = g_SoundCard.wIn_Flag;
//	OnAudioMeasStop();
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		if(!m_bGrabThreadRunning){

			StartLVDSGrabThread();
		}
		else{
			StopLVDSGrabThread();
		}
	}
	else//FHD
	{
		if(!m_bGrabThreadRunning){
			SetGrabInfo(&g_GrabImage);

			StartLVDSGrabThread_FHD();
		}
		else{
			StopLVDSGrabThread_FHD();
		}
	}//
//	if(bFlag) SetTimer(2, 1000, NULL);

}

void CDATsysView::ResetGrabStartThread()
{
	//	BOOL bFlag;
	if (CurrentSet->bPJT_GrabDisable == 1)
		return;

	g_nResetGrabMode = 1;
		//UHD
	if (g_nGrabberType == UHD_GRABBER)
	{
		if (!m_bGrabThreadRunning) {

			//StopLVDSGrabThread();
			StartLVDSGrabThread();
		}
		else {
			StopLVDSGrabThread();
			Sleep(100);
			StartLVDSGrabThread();
		}
	}
	else//FHD
	{
		if (!m_bGrabThreadRunning) {
			SetGrabInfo(&g_GrabImage);

			StartLVDSGrabThread_FHD();
		}
		else {
			StopLVDSGrabThread_FHD();
			Sleep(100);
			SetGrabInfo(&g_GrabImage);

			StartLVDSGrabThread_FHD();
		}
	}//
//	if(bFlag) SetTimer(2, 1000, NULL);
	g_nResetGrabMode = 0;
}

void CDATsysView::GrabBaseResetStartThread()
{
	__int64		Rdata, Wdata;
	//	BOOL bFlag;
	if (CurrentSet->bPJT_GrabDisable == 1)
		return;

	g_nResetGrabMode = 1;
	//UHD
	if (g_nGrabberType == UHD_GRABBER)
	{
		if (!m_bGrabThreadRunning) {

			StartLVDSGrabThread();
		}
		else {
			StopLVDSGrabThread();
			Sleep(100);

			GrabBaseReset();

			StartLVDSGrabThread();
		}
	}
	else//FHD
	{
		if (!m_bGrabThreadRunning) {
			SetGrabInfo(&g_GrabImage);

			StartLVDSGrabThread_FHD();
		}
		else {
			StopLVDSGrabThread_FHD();
			Sleep(100);
			SetGrabInfo(&g_GrabImage);

			StartLVDSGrabThread_FHD();
		}
	}
//	if(bFlag) SetTimer(2, 1000, NULL);
	g_nResetGrabMode = 0;
}


void CDATsysView::GrabBaseReset()
{
	__int64		Rdata, Wdata;
	//	BOOL bFlag;

	m_clsPCI.CM_RegisterRead(0x0170, &Rdata);
	Sleep(1);
	Wdata = Rdata | 0x20000;
	m_clsPCI.CM_RegisterWrite(0x0170, Wdata);
	Sleep(1);
	Wdata = Rdata & 0x0FFFF;
	m_clsPCI.CM_RegisterWrite(0x0170, Wdata);
}

void CDATsysView::OnGrabStop() 
{
//	BOOL bFlag;

//	bFlag = g_SoundCard.wIn_Flag;
//	OnAudioMeasStop();

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		StopLVDSGrabThread();
	}
	else //FHD
	{
		StopLVDSGrabThread_FHD();

	}
//	if(bFlag) SetTimer(2, 1000, NULL);

}

void CDATsysView::ShowSystemMsg(int nMgaNo) 
{
	return;
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
	return;
	if((m_pSystemMessageDlg != NULL)
	&& (m_pSystemMessageDlg->m_bShowDlg))
	{
		m_pSystemMessageDlg->m_bShowDlg = FALSE;
		m_pSystemMessageDlg->DestroyWindow();
		delete m_pSystemMessageDlg;
		m_pSystemMessageDlg = NULL;
	}
}

void CDATsysView::OnInputLock() 
{
	CString sMsg				 = _T("");
//	int i;

	UpdateData(TRUE);

	if(m_bInput_Lock)
	{
		GetDlgItem(IDC_RDO_TOOL_OPTION1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RDO_TOOL_OPTION2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RDO_TOOL_OPTION3)->EnableWindow(FALSE);

		CurrentSet->nToolOptionSetMethod = m_nToolOptionMethod;
		if(CurrentSet->nToolOptionSetMethod == 1)
		{
			if (g_nSysRobotType == AUTO_SYS)
			{
				GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);
			}
			else
			{
//				if (CurrentSet->bUseScanner) {
					GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);
				//}
				//else {
				//	GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
				//}
			}
		}
		else{
			GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
		}
     /*
		for(i =1; i < 11; i++)
		{
			m_ctrlOptionGrid.SetCol(1);
			m_ctrlOptionGrid.SetRow(i);
			m_ctrlOptionGrid.SetEditable(flexEDNone);
		}
		for(i =0; i < 3; i++)
		{
			m_ctrlCountryGrid.SetCol(1);
			m_ctrlCountryGrid.SetRow(i);
			m_ctrlCountryGrid.SetEditable(flexEDNone);
		}
		for(i =0; i < 3; i++)
		{
			m_ctrlVersionGrid.SetCol(1);
			m_ctrlVersionGrid.SetRow(i);
			m_ctrlVersionGrid.SetEditable(flexEDNone);
		}
		if(CheckToolCrc()){
			CurrentSet->sToolOption1 = m_ctrlOptionGrid.GetTextMatrix(1, 1);
			CurrentSet->sToolOption2 = m_ctrlOptionGrid.GetTextMatrix(2, 1);
			CurrentSet->sToolOption3 = m_ctrlOptionGrid.GetTextMatrix(3, 1);
			CurrentSet->sToolOption4 = m_ctrlOptionGrid.GetTextMatrix(4, 1);
			CurrentSet->sToolOption5 = m_ctrlOptionGrid.GetTextMatrix(5, 1);
			CurrentSet->sToolOption6 = m_ctrlOptionGrid.GetTextMatrix(6, 1);
			CurrentSet->sToolOption7 = m_ctrlOptionGrid.GetTextMatrix(7, 1);
			CurrentSet->sCommercialOption1 = m_ctrlOptionGrid.GetTextMatrix(8, 1);
			CurrentSet->sToolOption8 = m_ctrlOptionGrid.GetTextMatrix(9, 1);
			CurrentSet->sToolCRC = m_ctrlOptionGrid.GetTextMatrix(10, 1);

			CurrentSet->sAreaOption1 = m_ctrlCountryGrid.GetTextMatrix(0, 1);

			CurrentSet->sCountryGroup = m_ctrlCountryGrid.GetTextMatrix(1, 1);
			CurrentSet->sCountry = m_ctrlCountryGrid.GetTextMatrix(2, 1);

			CurrentSet->sCPUVersion = m_ctrlVersionGrid.GetTextMatrix(0, 1);
			CurrentSet->sMicomVersion = m_ctrlVersionGrid.GetTextMatrix(1, 1);
			CurrentSet->sMNTVersion = m_ctrlVersionGrid.GetTextMatrix(2, 1);


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
		} */
	}
	else{
		GetDlgItem(IDC_RDO_TOOL_OPTION1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RDO_TOOL_OPTION2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RDO_TOOL_OPTION3)->EnableWindow(TRUE);
	//	GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
	/*	if(CurrentSet->nToolOptionSetMethod == 2)
		{
			for(i =1; i < 11; i++)
			{
				m_ctrlOptionGrid.SetCol(1);
				m_ctrlOptionGrid.SetRow(i);
				m_ctrlOptionGrid.SetEditable(flexEDKbdMouse);
			}
			for(i =0; i <3; i++)
			{
				m_ctrlCountryGrid.SetCol(1);
				m_ctrlCountryGrid.SetRow(i);
				m_ctrlCountryGrid.SetEditable(flexEDKbdMouse);
			}
			for(i =0; i < 3; i++)
			{
				m_ctrlVersionGrid.SetCol(1);
				m_ctrlVersionGrid.SetRow(i);
				m_ctrlVersionGrid.SetEditable(flexEDKbdMouse);
			}

		}
		else{
			for(i =1; i < 11; i++)
			{
				m_ctrlOptionGrid.SetCol(1);
				m_ctrlOptionGrid.SetRow(i);
				m_ctrlOptionGrid.SetEditable(flexEDNone);
			}
			for(i =0; i < 3; i++)
			{
				m_ctrlCountryGrid.SetCol(1);
				m_ctrlCountryGrid.SetRow(i);
				m_ctrlCountryGrid.SetEditable(flexEDNone);
			}
			for(i =0; i < 3; i++)
			{
				m_ctrlVersionGrid.SetCol(1);
				m_ctrlVersionGrid.SetRow(i);
				m_ctrlVersionGrid.SetEditable(flexEDNone);
			} 
		} */

	}
}




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


void CDATsysView::OnMesDataUpload() //OnUploadMesData(WPARAM wParam, LPARAM lParam
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
		if (g_nSysRobotType == AUTO_SYS)//AUTO_SYS
		{
			AddStringToStatus(sTemp);
		}
		else
		{
			AfxMessageBox(sTemp);
			
		}
		
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
	//s = m_szExePath + _T("\\DFT_ModelReg.exe ");
	//strcpy_s(achCommand, _countof(achCommand), (LPCSTR)s);
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
	//+add kwmoon 080516
/*	if(!g_SoundCard.wIn_Flag)
	{
		g_SoundCard.WaveRead_Start();

		SetTimer(TIMER_MEASURE_AUDIO_OUTPUT, 200, NULL);
	} */
	//090615
	AudioMeasureStart();
}

void CDATsysView::OnAudioMeasStop() 
{
	//+add kwmoon 080516
/*
	if(g_SoundCard.wIn_Flag)
	{
		KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);
		g_SoundCard.WaveRead_Stop();
	} */

	AudioMeasureStop();
}	

void CDATsysView::SummaryGridDisabled(BOOL bEnable)
{
	GetDlgItem(IDC_MAINVIEW_GRID)->EnableWindow(bEnable);
}

void CDATsysView::OnScannerEnable() 
{
	BOOL bFlag;
	if (g_nSysRobotType == AUTO_SYS)//godtech 190602
	{
		//if (CurrentSet->bUseScanner)
		{
			OnScannerDisable();
		}
		return;
	}
	bFlag = g_SoundCard.wIn_Flag;
	OnAudioMeasStop();

	if(!CurrentSet->bUseScanner)
	{
		ctrlWipIdEdit.SetReadOnly(TRUE);
//AUTO --> X		
		if(g_nSysType != AUTO_SYS){
			if (ScannerCtrl.m_bPortOpen)
			{
				ScannerCtrl.PortClose();
			}
				InitScanner(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate);
		}
//
		CurrentSet->bUseScanner	= TRUE;
		CurrentSet->bUseStartBox = FALSE;
	}
	else{
			ctrlWipIdEdit.SetReadOnly(FALSE);

			//AUTO --> X		
			if (g_nSysType != AUTO_SYS) {
				if (ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();
				InitScanner(CurrentSet->sStartBoxComPort, CurrentSet->wStartBoxBaudRate);
			}
			//
			CurrentSet->bUseScanner = FALSE;
			CurrentSet->bUseStartBox = TRUE;		
	}

	if(CurrentSet->nToolOptionSetMethod == 1)
	{
		
//			if (CurrentSet->bUseScanner) {
				GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);
			//}
			//else {
			//	GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
			//}
		
		//if(CurrentSet->bUseScanner){
		//	GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);
		//}
		//else{
		//	GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
		//}
	}
	else{
		GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
	}

	if(bFlag) SetTimer(2, 1000, NULL);
}

void CDATsysView::OnScannerDisable() 
{
	BOOL bFlag;

	if(!CurrentSet->bUseScanner)
		return;


	bFlag = g_SoundCard.wIn_Flag;
	OnAudioMeasStop();

	CurrentSet->bUseScanner	= FALSE;
	CurrentSet->bUseStartBox = TRUE;
	ctrlWipIdEdit.SetReadOnly(FALSE);

//AUTO --> X
	if(g_nSysType != AUTO_SYS){
		if(ScannerCtrl.m_bPortOpen) ScannerCtrl.PortClose();
		InitScanner(CurrentSet->sStartBoxComPort, CurrentSet->wStartBoxBaudRate);
	}
//
	if(bFlag) SetTimer(2, 1000, NULL);
}

int CDATsysView::LocalDimmingTest()
{
	int nResult  = -1;
/*	DWORD dwEvent = 1000;
	CString sMsg;
	CString szPrevMsg = _T("");

	if(g_pView->m_pImageInfo->nUseExtAnalogGrabber == 0)
		return -1;

//	m_pDimmingTest->nChannel   = nCh;
//	m_pDimmingTest->nW_Add	   = nAdd;
//	m_pDimmingTest->nW_Command = nCmd;

	if(g_nRunningProcessNo == 1)
	{
		if(m_Dimming1RunningFlag)
		{
			dwEvent =  WaitForSingleObject(hServerEvents[EVENT_DIMMING1_COMPLETE],1500);			
		}

		::SetEvent(hClientEvents[EVENT_DIMMING1]);
		m_Dimming1RunningFlag = TRUE;
	}
	else
	{
		if(m_Dimming2RunningFlag)
		{
			dwEvent =  WaitForSingleObject(hServerEvents[EVENT_DIMMING2_COMPLETE],1500);			
		}

		::SetEvent(hClientEvents[EVENT_DIMMING2]);
		m_Dimming2RunningFlag = TRUE;
	}

	do
	{
		if(g_nRunningProcessNo == 1)
		{
			dwEvent =  WaitForSingleObject(hServerEvents[EVENT_DIMMING1_COMPLETE],3500);			

			if(dwEvent == WAIT_OBJECT_0) 
			{
				nResult = m_pDimmingTest->nResult; 
				dwEvent = -1; 

				sMsg.Format("DimmingTest1:0x%06x", m_pDimmingTest->nResultValue);
				AddStringToStatus(sMsg);

				sMsg.Format("0x%06x", m_pDimmingTest->nResultValue);
				if(CurrentSet->nDisplayType == DETAILEDGRID)
				{
					GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
					
					if(szPrevMsg != _T(""))
					{
						InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + sMsg);
						pCurStep->m_strMsg = szPrevMsg + " , " + sMsg;
					}
					else
					{
						InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
						pCurStep->m_strMsg = sMsg;
					}		
				}

				::ResetEvent(hServerEvents[EVENT_DIMMING1_COMPLETE]);

				m_Dimming1RunningFlag = FALSE;
			}
			else {
				nResult = -1; 
				dwEvent = -1; 
			}
		}
		else if(g_nRunningProcessNo == 2) {
			dwEvent =  WaitForSingleObject(hServerEvents[EVENT_DIMMING2_COMPLETE],35000);			
	
			if(dwEvent ==  WAIT_OBJECT_0) {
				nResult = m_pDimmingTest->nResult; 
				dwEvent = -1; 

				sMsg.Format("DimmingTest2:0x%06x", m_pDimmingTest->nResultValue);
				AddStringToStatus(sMsg);

				sMsg.Format("0x%06x", m_pDimmingTest->nResultValue);
				if(CurrentSet->nDisplayType == DETAILEDGRID) {
					GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
					
					if(szPrevMsg != _T("")) {
						InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + sMsg);
						pCurStep->m_strMsg = szPrevMsg + " , " + sMsg;
					}
					else {
						InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
						pCurStep->m_strMsg = sMsg;
					}		
				}

				::ResetEvent(hServerEvents[EVENT_DIMMING2_COMPLETE]);
				
				m_Dimming2RunningFlag = FALSE;
			}
			else {
				nResult = -1; 
				dwEvent = -1; 
			}
		}
		else {
			dwEvent =  WaitForSingleObject(hServerEvents[EVENT_DIMMING3_COMPLETE],35000);			
	
			if(dwEvent ==  WAIT_OBJECT_0) {
				nResult = m_pDimmingTest->nResult; 
				dwEvent = -1; 

				sMsg.Format("DimmingTest3:0x%06x", m_pDimmingTest->nResultValue);
				AddStringToStatus(sMsg);

				sMsg.Format("0x%06x", m_pDimmingTest->nResultValue);
				if(CurrentSet->nDisplayType == DETAILEDGRID) {
					GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
					
					if(szPrevMsg != _T("")) {
						InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + " , " + sMsg);
						pCurStep->m_strMsg = szPrevMsg + " , " + sMsg;
					}
					else {
						InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
						pCurStep->m_strMsg = sMsg;
					}		
				}

				::ResetEvent(hServerEvents[EVENT_DIMMING3_COMPLETE]);
				
				m_Dimming3RunningFlag = FALSE;
			}
			else {
				nResult = -1; 
				dwEvent = -1; 
			}
		}

	}while(dwEvent != -1);
*/
	return nResult;
}

int CDATsysView::Set_Relay(int nCmd)
{
	int nResult  = -1;
	DWORD dwEvent = 1000;
	CString sMsg;
	CString szPrevMsg = _T("");

	if (g_nSysRobotType == AUTO_SYS)//godtech 190531
	{
		return 0;
	}
	

	//if (g_nUseNoScanType == 1)//godtech 190531
	//{
	//	return 0;
	//}

	m_pUsbDio_Control->nChannel	   = g_nRunningProcessNo;
	m_pUsbDio_Control->nCtrlData = nCmd;


	dwEvent =  WaitForSingleObject(hUsbCtrl_Complete,500);			
	::SetEvent(hUsbCtrl);

	do {
		dwEvent =  WaitForSingleObject(hUsbCtrl_Complete,1000);			

		if(dwEvent == WAIT_OBJECT_0) {
			nResult = m_pUsbDio_Control->nResult; 
			dwEvent = -1; 

			if(nResult){
				if(nCmd == 0){
					sMsg.Format("AC Power OFF: OK");
					m_bAcPowerOnOff = FALSE;
				}
				else{
					sMsg.Format("AC Power ON: OK");
					m_bAcPowerOnOff = TRUE;
				}
			}
			else{
				if(nCmd == 0){
					sMsg.Format("AC Power OFF: NG");
				}
				else{
					sMsg.Format("AC Power ON: NG");
				}
			}
			AddStringToStatus(sMsg);

			::ResetEvent(hUsbCtrl_Complete);
		}
		else {
			nResult = -1; 
			dwEvent = -1; 
		}
	}while(dwEvent != -1);

	return nResult;
}

int CDATsysView::Set_Mhl(int nFunc, int nCmd)
{
	int nResult  = -1;
	DWORD dwEvent = 1000;
	CString sMsg;
	CString szPrevMsg = _T("");

	if (g_nSysRobotType == AUTO_SYS)//godtech 190531
	{
		return 0;
	}
	if (g_nUseNoScanType == 1)//godtech 190531
	{
		return 0;
	}
	
	if(CurrentSet->nHDMIGenType == 0){
		if(nCmd == 0){
			nResult = HDMIGeneratorCtrl.SetMHL_Control(TRUE);
		}
		else{
			nResult = HDMIGeneratorCtrl.SetMHL_Control(FALSE);
		}
	}
	else{

		if(nCmd == 0){
			HDMIGeneratorCtrl.SetEDID_PassCheck(FALSE) ;
			Sleep(100);
			HDMIGeneratorCtrl.SetCEC_OnOff(FALSE);
		}
		else{
			HDMIGeneratorCtrl.SetEDID_PassCheck(TRUE) ;
			Sleep(100);
			HDMIGeneratorCtrl.SetCEC_OnOff(CurrentSet->bHdmiCecControl);
		}

		m_pMhlCtrl->nChannel	   = g_nRunningProcessNo;
		m_pMhlCtrl->nFunction = nFunc;
		m_pMhlCtrl->nCtrlData = nCmd;

		dwEvent =  WaitForSingleObject(hMhlCtrl_Complete,500);			
		::SetEvent(hMhlCtrl);

		do {
			dwEvent =  WaitForSingleObject(hMhlCtrl_Complete,1000);			

			if(dwEvent == WAIT_OBJECT_0) {
				nResult = m_pMhlCtrl->nResult; 
				dwEvent = -1; 

				if(nResult){
					if(nCmd == 0){
						sMsg.Format("MHL Buffer Mode:MHL OK");
					}
					else{
						sMsg.Format("MHL Buffer Mode:HDMI OK");
					}
				}
				else{
					if(nCmd == 0){
						sMsg.Format("MHL Buffer Mode:MHL NG");
					}
					else{
						sMsg.Format("MHL Buffer Mode:HDMI NG");
					}
				}
				AddStringToStatus(sMsg);

				::ResetEvent(hMhlCtrl_Complete);
			}
			else {
				nResult = -1; 
				dwEvent = -1; 
			}

		}while(dwEvent != -1);
	}
	if(nResult){
		if(nCmd == 0){
			m_bMhlMode = FALSE;
		}
		else{
			m_bMhlMode = TRUE;
		}
	}

	return nResult;
}

void CDATsysView::AudioMeasureStart()
{
	if(!g_SoundCard.wIn_Flag)
	{
		g_SoundCard.WaveRead_Start();
		SetTimer(TIMER_MEASURE_AUDIO_OUTPUT, 200, NULL);

	}
}
void CDATsysView::AudioMeasureStop()
{
	if(g_SoundCard.wIn_Flag)
	{
		KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);
		g_SoundCard.WaveRead_Stop();
	//	Sleep(500);
	}
}

void CDATsysView::ComLogClear() 
{
	//m_ctrlEditComLog.rest
	HWND hEdit = m_ctrlEditComLog.m_hWnd;

	BOOL bReadOnly = ::GetWindowLong( hEdit, GWL_STYLE ) & ES_READONLY;
	if( bReadOnly )
		::SendMessage( hEdit, EM_SETREADONLY, FALSE, 0 );
	::SendMessage( hEdit, EM_SETSEL, 0, -1 );
	::SendMessage( hEdit, WM_CLEAR, 0, 0 );
	if( bReadOnly )
		::SendMessage( hEdit, EM_SETREADONLY, TRUE, 0 );


	gPLC_Ctrl.m_sRunProcessMsgCommand = "CLEAR";	
	gPLC_Ctrl.m_sRunProcessMsg = "";
	gServer_Ctrl.SendMsgCtrl(_SERVER_COM_RUNMSG_ID); //_SERVER_COM_RUNMSG_ID
}



void CDATsysView::OnSeqLock() 
{
	UpdateData(TRUE);

	if(m_bSeq_Lock)
	{
	//	m_ctrlSummaryGrid.SetLeftCol(1);
	//	m_ctrlSummaryGrid.EnableWindow(FALSE);
	}
	else{
	//	m_ctrlSummaryGrid.EnableWindow(TRUE);
	}
	
}

//This block processes the keystroke info.
LRESULT CDATsysView::ProcessKeyPad(WPARAM wParam, LPARAM lParam)
{
	LONG nStatus = 0;
	LONG nIdle = 0;
	static long sCount = 0;
	static WPARAM swparam_old = 0;
	static LPARAM slparam_old = 0;

	
//	MSG msg;


#if 0
	//nIdle
	//sCount++;
	//if (sCount > 100000)
	//	sCount = 0;
	//return 1;//godtech debug 
	if ((swparam_old == wParam) && (slparam_old = lParam))
	{
		if (sCount < 300)
		{
			sCount++;
			if (sCount < 25)
			{
				//PostMessage(hwnd, WM_USER + 755, wparam, lparam);//Send info to app Window.

			}
			else //if (sCount == 220)
			{
				//char lBuf[128];
				//sprintf(lBuf, "wparam:%x, lparam:%x", wparam, lparam);
				//MessageBox(hwnd, lBuf, "OverFlow", 1);
				sCount = 301;
				//Sleep(100);
				//wparam = 0xC0;
				//lparam = 0x290001;
				//PostMessage(hwnd, WM_USER + 755, wparam, lparam);//
			}
		}

	}
	else
	{
		swparam_old = wParam;
		slparam_old = lParam;
		sCount = 0;
		
	}
#endif
	switch(lParam & 0xFFFFFF)
	{
//IF 2in1
#ifdef _DFT_2IN1
		case 0x0F0001 :
			if(wParam == 0x09) nStatus = NG_BTN1; 
			break;

		case 0x470001 : 
			if(wParam == 0x24) nStatus = OK_BTN1; 
			break;

		case 0x4B0001 : 
			if(wParam == 0x25) nStatus = RETRY_BTN1; 
			break;

		case 0x4F0001 : 
			if(wParam == 0x23) nStatus = STOP_BTN1; 
			break;

		case 0x520001 : 
			if(wParam == 0x2D) nStatus = START_BTN1; 
			break;
	
		case 0x370001 : 
			if(wParam == 0x6A) nStatus = NG_BTN2; 
			break;

		case 0x490001 : 
			if(wParam == 0x21) nStatus = OK_BTN2; 
			break;

		case 0x4D0001 : 
			if(wParam == 0x27) nStatus = RETRY_BTN2; 
			break;

		case 0x510001 : 
			if(wParam == 0x22) nStatus = STOP_BTN2; 
			break;

		case 0x530001:
			if (wParam == 0x2E) nStatus = START_BTN2;
			break;
	
			
			//
#endif
#ifdef _DFT_3IN1

		case 0x470001 : //7(Home)
			if(wParam == 0x67) nStatus = NG_BTN1; 
			break;

		case 0x4B0001 :  //4(<-)
			if(wParam == 0x64) nStatus = OK_BTN1; 
			break;

		case 0x4F0001 : //1(End)
			if(wParam == 0x61) nStatus = RETRY_BTN1; 
			break;

		case 0x520001 :  //0(Ins)
			if(wParam == 0x60){
				if (CurrentSet->bIsRunning)
				{
					nStatus = STOP_BTN1;
				}
				else{
					nStatus = START_BTN1;
				}
			}
			break;
	
		case 0x350001 : ///
			if(wParam == 0x6F) nStatus = NG_BTN2; 
			break;

		case 0x480001 : //8(Up)
			if(wParam == 0x68) nStatus = OK_BTN2; 
			break;

		case 0x4C0001 : //5
			if(wParam == 0x65) nStatus = RETRY_BTN2; 
			break;

		case 0x500001 : //2(Dn)
			if(wParam == 0x62){
				if (CurrentSet->bIsRunning)
				{
					nStatus = STOP_BTN2;
				}
				else{
					nStatus = START_BTN2;
				}
			}
			break;

		case 0x490001 : //9(PgUp)
			if(wParam == 0x69) nStatus = NG_BTN3; 
			break;

		case 0x4D0001 : //6(->)
			if(wParam == 0x66) nStatus = OK_BTN3; 
			break;

		case 0x510001 : //3(PgDn)
			if(wParam == 0x63) nStatus = RETRY_BTN3; 
			break;

		case 0x530001 : //.(Del)
			if(wParam == 0x6E){
				if (CurrentSet->bIsRunning)
				{
					nStatus = STOP_BTN3;
				}
				else{
					nStatus = START_BTN3;
				}
			}
			break;
#endif

		default :
				nStatus =0;
				if (wParam == 0xc0)
					nStatus = 0;
				

			
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
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
						PostMessage(WM_COMMAND, ID_EDIT_CURRENT);
					}
				}
				else if(wParam == VK_F3){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
						PostMessage(WM_COMMAND, ID_MODEL_OPEN);
					}
				}
				else if(wParam == VK_F4){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
						PostMessage(WM_COMMAND, ID_EDIT_COMPILE);
					}
				}
				else if(wParam == VK_F6){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
						if(m_bResultDlgActivate) {
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							m_bResultDlgActivate = FALSE;
						}
						PostMessage(WM_COMMAND, ID_RUN_MAKE_REF);
					}
				}
				else if(wParam == VK_F7){
					if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
						if(m_bResultDlgActivate) {
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							m_bResultDlgActivate = FALSE;
						}
						PostMessage(WM_COMMAND, ID_RUN_ADJ_SPEC);
					}
				}
				else
				{
					nIdle++;
#if 0
					
						sCount++;
					if (sCount > 100000)
						sCount = 0;
					return 1;//godtech debug 
#endif
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
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							PostMessage(WM_COMMAND, ID_EDIT_CURRENT);
						}
					}
					else if(wParam == VK_F3){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							PostMessage(WM_COMMAND, ID_MODEL_OPEN);
						}
					}
					else if(wParam == VK_F4){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							PostMessage(WM_COMMAND, ID_EDIT_COMPILE);
						}
					}
					else if(wParam == VK_F6){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}
							PostMessage(WM_COMMAND, ID_RUN_MAKE_REF);
						}
					}
					else if(wParam == VK_F7){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}
							PostMessage(WM_COMMAND, ID_RUN_ADJ_SPEC);
						}
					}
					else
					{
						nIdle++;
#if 0

					sCount++;
					if (sCount > 100000)
						sCount = 0;
					return 1;//godtech debug 
#endif 
					}
					
				}	
				else
					{
						nIdle++;
#if 0

					sCount++;
					if (sCount > 100000)
						sCount = 0;
					return 1;//godtech debug 
#endif 
					}
			}
			else if(g_nRunningProcessNo == 3)
			{
				if ((GetKeyState(VK_CONTROL) & 0x8000))
				{
					if(wParam == VK_F5){
						nStatus = START_BTN3;
						//PostMessage(WM_COMMAND, ID_RUN_RUN);
					}
					else if(wParam == VK_ESCAPE){
						nStatus = STOP_BTN3;
						//PostMessage(WM_COMMAND, ID_RUN_ABORT);
					}  
					else if(wParam == VK_F2){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							PostMessage(WM_COMMAND, ID_EDIT_CURRENT);
						}
					}
					else if(wParam == VK_F3){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							PostMessage(WM_COMMAND, ID_MODEL_OPEN);
						}
					}
					else if(wParam == VK_F4){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							PostMessage(WM_COMMAND, ID_EDIT_COMPILE);
						}
					}
					else if(wParam == VK_F6){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}
							PostMessage(WM_COMMAND, ID_RUN_MAKE_REF);
						}
					}
					else if(wParam == VK_F7){
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}
							PostMessage(WM_COMMAND, ID_RUN_ADJ_SPEC);
						}
					}
					else
					{
						nIdle++;
#if 0

						sCount++;
						if (sCount > 100000)
							sCount = 0;
						return 1;//godtech debug 
#endif 
					}
				}
				else
				{
					nIdle++;
#if 0

					sCount++;
					if (sCount > 100000)
						sCount = 0;
					return 1;//godtech debug 
#endif 
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
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else {
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							if(m_bResultDlgActivate) {
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}

//AUTO
							if(g_nSysType == AUTO_SYS){
								if(CurrentSet->nRunStartType == MANUAL_START)
								{
								//	CurrentSet->bIsRunMsg = TRUE;
									PostMessage(WM_COMMAND, ID_RUN_RUN);
								}
							}
							else{
								if(!CurrentSet->bUseScanner) {
									PostMessage(WM_COMMAND, ID_RUN_RUN);
									CurrentSet->bIsRunMsg = TRUE;
								}
							}
						}	
					}
				}
				break;

			case STOP_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
					}
					else {
						if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE) {
							CurrentSet->bRunAbort = TRUE;
							m_szResultMsg2.LoadString(IDS_USER_BREAK);
							AddStringToStatus(m_szResultMsg2);
							g_nUserStop++;
						}

						if(m_bResultDlgActivate) {
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							m_bResultDlgActivate = FALSE;
						}
					}
				}
				break;

			case RETRY_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
					}
				}
				break;

			case OK_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
					else if(m_UserMsgDlg.m_bActivate == TRUE) {
						::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
				}
				break;

			case NG_BTN1: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
					}
				}
				break;

			case START_BTN2: 
			case STOP_BTN2: 
			case RETRY_BTN2: 
			case OK_BTN2: 
			case NG_BTN2: 
			case START_BTN3: 
			case STOP_BTN3: 
			case RETRY_BTN3: 
			case OK_BTN3: 
			case NG_BTN3: 
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
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else {
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							if(m_bResultDlgActivate) {
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}
//AUTO
							if(g_nSysType != AUTO_SYS){
								if(CurrentSet->nRunStartType == MANUAL_START)
								{
								//	CurrentSet->bIsRunMsg = TRUE;
									PostMessage(WM_COMMAND, ID_RUN_RUN);
								}
							}
							else{
								if(!CurrentSet->bUseScanner) {
									PostMessage(WM_COMMAND, ID_RUN_RUN);
									CurrentSet->bIsRunMsg = TRUE;
								}
							}
						}	
					}
				}
				break;

			case STOP_BTN2: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
					}
					else {
						if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE) {
							CurrentSet->bRunAbort = TRUE;
							m_szResultMsg2.LoadString(IDS_USER_BREAK);
							AddStringToStatus(m_szResultMsg2);
						}

						if(m_bResultDlgActivate) {
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							m_bResultDlgActivate = FALSE;
						}
						
					}
				}
				break;

			case RETRY_BTN2: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
					}
				}
				break;

			case OK_BTN2: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
					else if(m_UserMsgDlg.m_bActivate == TRUE) {
						::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
				}
				break;

			case NG_BTN2: 
				{
      				if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
				}
				break;

			case START_BTN3: 
			case STOP_BTN3: 
			case RETRY_BTN3: 
			case OK_BTN3: 
			case NG_BTN3: 
			default :
			break;
		}
	}
	else if(g_nRunningProcessNo == 3)
	{
		switch(nStatus) 
		{
			case START_BTN1: 
			case STOP_BTN1: 
			case RETRY_BTN1: 
			case OK_BTN1: 
			case NG_BTN1: 
			case START_BTN2: 
			case STOP_BTN2: 
			case RETRY_BTN2: 
			case OK_BTN2: 
			case NG_BTN2: 
				break;

			case START_BTN3: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
					}
					else {
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
							if(m_bResultDlgActivate) {
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}

							if(!CurrentSet->bUseScanner) {
								PostMessage(WM_COMMAND, ID_RUN_RUN);
								CurrentSet->bIsRunMsg = TRUE;
							}
						}	
					}
				}
				break;

			case STOP_BTN3: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
					}
					else {
						if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE) {
							CurrentSet->bRunAbort = TRUE;
							m_szResultMsg2.LoadString(IDS_USER_BREAK);
							AddStringToStatus(m_szResultMsg2);
						}

						if(m_bResultDlgActivate) {
							SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
							m_bResultDlgActivate = FALSE;
						}
						
					}
				}
				break;

			case RETRY_BTN3: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
					}
				}
				break;

			case OK_BTN3: 
				{
					if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
					}
					else if(m_UserMsgDlg.m_bActivate == TRUE) {
						::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
					}
				}
				break;

			case NG_BTN3: 
				{
      				if(m_StopStepDlg.m_bActivate == TRUE) {
						::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
					}
					else if(m_ManualTestDlg.m_bActivate == TRUE) {
						::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
					}
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
					}
					else if(m_pVfMeasureCheckDlg.m_bActivate){
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
					}
				}
				break;

			default :
			break;
		}
	}

	
				
#if 0

	
	
	static WPARAM s_wParam = 0;
	static LPARAM s_lParam = 0;
	
	if((s_wParam == wParam)&&(s_lParam == lParam))
	{
		s_wParam = wParam;
		s_lParam = lParam;
		sCount++;
		if (sCount > 1000)
		{
			sCount = 0;
		//	removehook3();
			return 1L;
		}
	//	SendMessage( WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
		
	}
	else
	{
		s_wParam = wParam;
		s_lParam = lParam;
		sCount = 0;
	}
 

#endif 
					

	return 0L;
}



//This block processes the keystroke info.
int CDATsysView::ProcessAutokey(LONG nStatus)
{
	//LONG nStatus = 0;
	//	MSG msg;

	
	if (g_nRunningProcessNo == 1)
	{
		switch (nStatus)
		{
		case START_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
			}
			else {
				if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
					if (m_bResultDlgActivate) {
						SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
						m_bResultDlgActivate = FALSE;
					}

					//AUTO
					if (g_nSysType == AUTO_SYS) {
						if (CurrentSet->nRunStartType == MANUAL_START)
						{
							//	CurrentSet->bIsRunMsg = TRUE;
							PostMessage(WM_COMMAND, ID_RUN_RUN);
						}
					}
					else {
						if (!CurrentSet->bUseScanner) {
							PostMessage(WM_COMMAND, ID_RUN_RUN);
							CurrentSet->bIsRunMsg = TRUE;
						}
					}
				}
			}
		}
		break;

		case STOP_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
			}
			else {
				if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE) {
					CurrentSet->bRunAbort = TRUE;
					m_szResultMsg2.LoadString(IDS_USER_BREAK);
					AddStringToStatus(m_szResultMsg2);
					g_nUserStop++;
				}

				if (m_bResultDlgActivate) {
					SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
					m_bResultDlgActivate = FALSE;
				}
			}
		}
		break;

		case RETRY_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
			}
		}
		break;

		case OK_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
			}
			else if (m_UserMsgDlg.m_bActivate == TRUE) {
				::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
			}
		}
		break;

		case NG_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
			}
		}
		break;

		default:
			break;
		}
	}
	else if (g_nRunningProcessNo == 2)
	{
		switch (nStatus)
		{
		
		case START_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
			}
			else {
				if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
					if (m_bResultDlgActivate) {
						SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
						m_bResultDlgActivate = FALSE;
					}
					//AUTO
					if (g_nSysType != AUTO_SYS) {
						if (CurrentSet->nRunStartType == MANUAL_START)
						{
							//	CurrentSet->bIsRunMsg = TRUE;
							PostMessage(WM_COMMAND, ID_RUN_RUN);
						}
					}
					else {
						if (!CurrentSet->bUseScanner) {
							PostMessage(WM_COMMAND, ID_RUN_RUN);
							CurrentSet->bIsRunMsg = TRUE;
						}
					}
				}
			}
		}
		break;

		case STOP_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
			}
			else {
				if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE) {
					CurrentSet->bRunAbort = TRUE;
					m_szResultMsg2.LoadString(IDS_USER_BREAK);
					AddStringToStatus(m_szResultMsg2);
				}

				if (m_bResultDlgActivate) {
					SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
					m_bResultDlgActivate = FALSE;
				}

			}
		}
		break;

		case RETRY_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
			}
		}
		break;

		case OK_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
			}
			else if (m_UserMsgDlg.m_bActivate == TRUE) {
				::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
			}
		}
		break;

		case NG_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
			}
		}
		break;

		default:
			break;
		}
	}
	else if (g_nRunningProcessNo == 3)
	{
		switch (nStatus)
		{
		
		case START_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN3);
			}
			else {
				if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg) {
					if (m_bResultDlgActivate) {
						SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
						m_bResultDlgActivate = FALSE;
					}

					if (!CurrentSet->bUseScanner) {
						PostMessage(WM_COMMAND, ID_RUN_RUN);
						CurrentSet->bIsRunMsg = TRUE;
					}
				}
			}
		}
		break;

		case STOP_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN3);
			}
			else {
				if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE) {
					CurrentSet->bRunAbort = TRUE;
					m_szResultMsg2.LoadString(IDS_USER_BREAK);
					AddStringToStatus(m_szResultMsg2);
				}

				if (m_bResultDlgActivate) {
					SendMessage(UM_CLOSE_RESULT_DLG, 0, 0);
					m_bResultDlgActivate = FALSE;
				}

			}
		}
		break;

		case RETRY_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN3);
			}
		}
		break;

		case OK_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
			}
			else if (m_UserMsgDlg.m_bActivate == TRUE) {
				::SendMessage(m_UserMsgDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN3);
			}
		}
		break;

		case NG_BTN:
		{
			if (m_StopStepDlg.m_bActivate == TRUE) {
				::SendMessage(m_StopStepDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
			}
			else if (m_ManualTestDlg.m_bActivate == TRUE) {
				::SendMessage(m_ManualTestDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
			}
			else if (m_pInputCheckDlg.m_bActivate) {
				::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
			}
			else if (m_pVfMeasureCheckDlg.m_bActivate) {
				::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN3);
			}
		}
		break;

		default:
			break;
		}
	}

	return 0;
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
	m_ctrlEditComLog.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible   = GetNumVisibleLines(&m_ctrlEditComLog);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (&m_ctrlEditComLog != m_ctrlEditComLog.GetFocus())
	{
		m_ctrlEditComLog.LineScroll(INT_MAX);
		m_ctrlEditComLog.LineScroll(1 - nVisible);
	}

	return 0;
}
//-----------------------------------------------------------------------------
//  GetNumVisibleLines()
///
/// \brief	Returns the number of lines that are currently visible in the
///			client area of the given CRichEditCtrl.
///
/// 
///
/// \param [in]		pCtrl	Pointer to the CRichEditCtrl object to query.
///
/// \return					The number of currently visible lines.
///
/// \remark
/// The code is written for best comprehension / readability, not for code
/// or execution efficiency.
//-----------------------------------------------------------------------------
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
	nLastLine  = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}
void CDATsysView::ComLog_AddString(CString szString)
{

	CString sTime		 = _T("");
	SYSTEMTIME cur_time;
	GetLocalTime(&cur_time);

	gPLC_Ctrl.m_sRunProcessMsgCommand = "ADD";
	gPLC_Ctrl.m_sRunProcessMsg = szString;
	gServer_Ctrl.SendMsgCtrl(_SERVER_COM_RUNMSG_ID); //_SERVER_COM_RUNMSG_ID

//	sTime.Format("[%02d:%02d:%02d.%03d]%s\r\n", cur_time.wHour, cur_time.wMinute, cur_time.wSecond, cur_time.wMilliseconds, szString);
	sTime.Format("%s\r\n", szString);
	AppendToLogAndScroll(sTime);
}
void CDATsysView::SaveMessageList(CString szString)
{

	CString szPrevMsg;// 
	if (CurrentSet->bIsRunning)
	{
		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);
		if (szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + szString);
			pCurStep->m_strMsg = szPrevMsg + "; " + szString;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szString);
			pCurStep->m_strMsg = szString;
		}
	}
}


#if 1
	//char *sHeader1[] = { "", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT" };
	//char *sHeader2[14] = { "NO","STEP", "OK/NG", "T/T", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE","" };

//#define  MAX_COLUMN_NO	13
void CDATsysView::InitMainGrid(int nGridType)
{
	CString sTmp;

	char *sHeader2[14] = { "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE" ,""};
#ifdef _DFT_3IN1
	int  nWidth[14]    = {  40, 145,    57,      70     , 90       , 75       , 75    , 65            , 200            , 200     , 76, 500, 10, }; //int  nWidth[13] = { 300,  400, 2400, 900, 700, 900, 750, 750, 650, 2000, 2000, 760, 5000};
				// No 400// Step, 2400) // Result 900 // Measure 700 
				// Target 900 // L-Limit 750 // U-Limit 750 // Unit 650 
				// Audio Target 2000 // Audio Measure 2000 // Elapsed Time 760 
				// Message 5000);

#else
	int  nWidth[14] = { 40, 160,    65,      70     , 90       , 75       , 75    , 65            , 200            , 200     , 76, 500, 10, }; //int  nWidth[13] = { 300,  400, 2400, 900, 700, 900, 750, 750, 650, 2000, 2000, 760, 5000};

#endif

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
//+ 090909
void CDATsysView::InitMainGrid(int nGridType)
{



	//SetListMainControl(nGridType);

	CString sTmp;

	char *sHeader1[] = {"", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT"};
	char *sHeader2[] = {"", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE"};

	m_ctrlSummaryGrid.SetRedraw(FALSE);
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
//3in1
	m_ctrlSummaryGrid.SetFontSize(8);
//
//	m_ctrlSummaryGrid.SetFontSize(9);
//
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


	m_ctrlSummaryGrid.SetRedraw(TRUE);
	m_ctrlSummaryGrid.Refresh();
}
#endif
//void CDATsysView::SetListMainControl(int nGridType)
//{
//
////	//	m_CtrlListTestProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
////	m_CtrlListMainProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
////	m_CtrlListMainProcess.InsertColumn(0, _T("No."), LVCFMT_CENTER, 40);
////	m_CtrlListMainProcess.InsertColumn(1, _T("STEP"), LVCFMT_CENTER, 160);// 리스트 행 추가
////	m_CtrlListMainProcess.InsertColumn(2, _T("RESULT"), LVCFMT_CENTER, 60);// 리스트 행 추가
////	m_CtrlListMainProcess.InsertColumn(3, _T("T/T"), LVCFMT_CENTER, 50);// 리스트 행 추가
////	//m_CtrlListMainProcess.InsertColumn(4, _T("Discription"), LVCFMT_LEFT, 350);// 리스트 행 추가
////	//m_CtrlListTestProcess.InsertColumn(3, _T("Wait"), LVCFMT_LEFT, 50);// 리스트 행 추가
////	//m_CtrlListTestProcess.InsertColumn(4, _T("Time"), LVCFMT_CENTER, 70);// 리스트 행 추가
////	//m_CtrlListTestProcess.InsertColumn(5, _T("Ref1"), LVCFMT_CENTER, 60);// 리스트 행 추가
////	//m_CtrlListTestProcess.InsertColumn(6, _T("Ref2"), LVCFMT_CENTER, 60);// 리스트 행 추가
////	//m_CtrlListTestProcess.InsertColumn(7, _T("Value"), LVCFMT_CENTER, 60);// 리스트 행 추가
////	//m_CtrlListTestProcess.InsertColumn(8, _T("OK/NG"), LVCFMT_CENTER, 60);// 리스트 행 추가
////	//m_CtrlListTestProcess.InsertColumn(9, _T("T/T"), LVCFMT_CENTER, 50);// 리스트 행 추가
////
////	m_CtrlListMainProcess.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
////	m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T("FM TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////	m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////	//m_CtrlListTestProcess.SetItem(0, 3, LVIF_TEXT, _T("2.5"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////	//m_CtrlListTestProcess.SetItem(0, 4, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////	//m_CtrlListTestProcess.SetItem(0, 5, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////	//m_CtrlListTestProcess.SetItem(0, 6, LVIF_TEXT, _T("80%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////	//m_CtrlListTestProcess.SetItem(0, 7, LVIF_TEXT, _T("90%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////	//m_CtrlListTestProcess.SetItem(0, 8, LVIF_TEXT, _T("OK"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
////
////
////
//	CFont *font = new CFont;
//	//font->CreateFontA(15,                          // nHeight
//	//	0,                          // nWidth
//	//	0,                          // nEscapement
//	//	0,                          // nOrientation
//	//	100,                           // nWeight
//	//	0,                          // bItalic
//	//	0,                          // bUnderline 
//	//	0,                          // cStrikeOut 
//	//	0,                          // nCharSet
//	//	0,           // nOutPrecision 
//	//	0,                          // nClipPrecision 
//	//	PROOF_QUALITY,              // nQuality
//	//	0,  // nPitchAndFamily 
//	//	_T("Arial"));                     // lpszFacename
//
////
//////font->CreateFontA( 20,                          // nHeight
//////				   0,                          // nWidth
//////				   0,                          // nEscapement
//////				   0,                          // nOrientation
//////				   400,                           // nWeight
//////				   0,                          // bItalic
//////				   0,                          // bUnderline 
//////				   0,                          // cStrikeOut 
//////				   0,                          // nCharSet
//////				   0,           // nOutPrecision 
//////				   0,                          // nClipPrecision 
//////				 PROOF_QUALITY,              // nQuality
//////				 0,  // nPitchAndFamily 
//////				 _T("Arial"));                     // lpszFacename
//	font->CreatePointFont(10,_T("Arial"));   
//	m_CtrlListMainProcess.SetFont(font);
//	//m_CtrlListMainProcess.
////
//	delete font;
//
//	CString sTmp;
//
//	char *sHeader1[] = {  "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT" ,""};
//	char *sHeader2[] = {  "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE","" };
//
//	//m_ctrlSummaryGrid.SetRedraw(FALSE);
//	//m_ctrlSummaryGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything
//
//	//m_ctrlSummaryGrid.SetBackColorFixed(RGB(210, 216, 176));
//	//m_ctrlSummaryGrid.SetForeColorFixed(RGB(51, 102, 153));
//	//m_ctrlSummaryGrid.SetBackColor(RGB(247, 247, 231));
//	////	m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
//	//m_ctrlSummaryGrid.SetGridColor(RGB(200, 200, 176));
//
//	//m_ctrlSummaryGrid.SetFixedRows(0);
//	//m_ctrlSummaryGrid.SetFixedCols(0);
//	//m_ctrlSummaryGrid.SetFrozenRows(1);
//
//	//m_ctrlSummaryGrid.SetFillStyle(flexFillRepeat);
//	//m_ctrlSummaryGrid.SetFontName("Arial");
//	////	m_ctrlSummaryGrid.SetFontSize(10);
//	////3in1
//	//m_ctrlSummaryGrid.SetFontSize(8);
//	////
//	////	m_ctrlSummaryGrid.SetFontSize(9);
//	////
//	if (nGridType == DETAILEDGRID)
//	{
//		m_CtrlListMainProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
//		//m_ctrlSummaryGrid.SetCols(13);
//		//m_ctrlSummaryGrid.SetRows(1);
//		//m_ctrlSummaryGrid.SetExtendLastCol(TRUE);
//		////		m_ctrlSummaryGrid.SetFrozenCols(3);	
//		//m_ctrlSummaryGrid.SetFrozenCols(2);
//
//		// Set Column Alignment & Column Title
//		for (int nCol = 0; nCol < 13; nCol++)
//		{
//			//m_ctrlSummaryGrid.SetRow(0);
//			//m_ctrlSummaryGrid.SetCol(nCol);
//
//			//m_ctrlSummaryGrid.SetText(sHeader2[nCol]);
//			//m_CtrlListMainProcess.InsertColumn(0, _T(sHeader2[nCol]), LVCFMT_CENTER, 40);
//			//m_CtrlListMainProcess.InsertColumn(1, _T("STEP"), LVCFMT_CENTER, 160);// 리스트 행 추가
//
//
//			switch (nCol)
//			{
//			case 0: // Check// No
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignCenterCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 300); //300
//				//m_ctrlSummaryGrid.SetColDataType(nCol, flexDTBoolean);
//				//m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, 30);
//				break;
//
//
//			case 1: // Step
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignLeftCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 2400);//3100
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_LEFT, 200);
//				break;
//
//			case 2: // Result
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignCenterCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 900);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, 90);
//				break;
//
//			case 3: // Measure
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignRightCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 700);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_RIGHT, 70);
//				break;
//
//			case 4: // Target
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignRightCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 900);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_RIGHT, 90);
//				break;
//
//			case 5: // L-Limit
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignRightCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 750);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_RIGHT, 75);
//				break;
//
//			case 6: // U-Limit
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignRightCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 750);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_RIGHT, 750);
//				break;
//
//			case 7: // Unit
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignCenterCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 650);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, 65);
//				break;
//
//			case 8: // Audio Target
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignCenterCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 2000);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, 200);
//				break;
//			case 9: // Audio Measure
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignCenterCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 2000);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, 200);
//				break;
//			case 10: // Elapsed Time
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignCenterCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 760);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, 76);
//				break;
//
//			case 11: // Message
//				//m_ctrlSummaryGrid.SetColAlignment(nCol, flexAlignCenterCenter);
//				//m_ctrlSummaryGrid.SetColWidth(nCol, 5000);
//				m_CtrlListMainProcess.InsertColumn(nCol, _T(sHeader2[nCol]), LVCFMT_CENTER, 500);
//				break;
//			}
//			//m_ctrlSummaryGrid.SetFixedAlignment(nCol, flexAlignCenterCenter);
//		}
//
//	}
//
//	m_CtrlListMainProcess.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
//	m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T(" "), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//	m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T(" "), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
//
//
//	//m_ctrlSummaryGrid.SetRedraw(TRUE);
//	//m_ctrlSummaryGrid.Refresh();
//	SetSubListMainControl(nGridType);
//
//}
//
//
//void CDATsysView::SetSubListMainControl(int nGridType)//SetListMainControl()
//{
//
//	CSize lSize;
//	CString strTemp;
//
//	m_CtrlListMainProcess.DeleteAllItems();
//
//	//mCheckAll = 1;
//
//	//for (int i = 0; i < gTestSeqData.TestCnt; i++)
//	//{
//	//	strTemp.Format(_T("%d"), i + 1);
//	//	m_CtrlListMainProcess.InsertItem(i, strTemp);
//
//
//	//	strTemp = gTestSeqData.TestStepList[i].strStepName;
//	//	m_CtrlListMainProcess.SetItem(i, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
//	//	m_CtrlListMainProcess.SetItem(i, 2, LVIF_TEXT, "", NULL, NULL, NULL, NULL);
//	//	m_CtrlListMainProcess.SetItem(i, 3, LVIF_TEXT, "0", NULL, NULL, NULL, NULL);
//	//	strTemp = gTestSeqData.TestStepList[i].strStepWork;
//	//	//		m_CtrlListMainProcess.SetItem(i, 4, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
//
//	//	gTestSeqData.TestStepList[i].bEnable = 1;
//	//	if (gTestSeqData.TestStepList[i].bEnable == 1)
//	//	{
//	//		m_CtrlListMainProcess.SetCheck(i, 1);
//	//	}
//	//	else
//	//	{
//	//		m_CtrlListMainProcess.SetCheck(i, 0);
//	//	}
//	//}
//
//	//m_CtrlListMainProcess.Invalidate();
//
//	//m_ctrlSummaryGrid.SetRedraw(flexRDNone);
//	//m_ctrlSummaryGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything)));
//
//	//+ 2007.10.10 Add BY USY
//	char *sHeader1[] = { "", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT" };
//	char *sHeader2[] = { "", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE" };
//	//-
//
//	////Row 
//	//if (CurrentSet->bCompiled)
//	//{
//	//	if (nGridType == DETAILEDGRID)
//	//	{
//	//		//	if(StepList.GetCount() + CurrentSet->VF_TestInfo.GetCount() + 1 < 12) m_ctrlSummaryGrid.SetRows(12);
//	//		//	else m_ctrlSummaryGrid.SetRows(StepList.GetCount() + CurrentSet->VF_TestInfo.GetCount() + 2);
//	//		if (StepList.GetCount() + 1 < 12) m_ctrlSummaryGrid.SetRows(12);
//	//		else m_ctrlSummaryGrid.SetRows(StepList.GetCount() + 2);
//	//	}
//	//}
//	//else m_ctrlSummaryGrid.SetRows(12);
//
//	//+Insert Step Default Data
//	POSITION Position;
//	CStep* pStep;
//	CString sTmp;
//	int nTmp;
//	Position = StepList.GetHeadPosition();
//
//	if (nGridType == DETAILEDGRID)
//	{
//	//	m_ctrlSummaryGrid.SetRow(0);
//	//	//		m_ctrlSummaryGrid.SetFrozenCols(3);
//	//	m_ctrlSummaryGrid.SetFrozenCols(2);
//
//	//	for (nTmp = 0; nTmp < 13; nTmp++)
//	//	{
//	//		m_ctrlSummaryGrid.SetCol(nTmp);
//	//		m_ctrlSummaryGrid.SetText(sHeader2[nTmp]);
//	//		m_ctrlSummaryGrid.SetCellBackColor(RGB(210, 216, 176));
//	//		m_ctrlSummaryGrid.SetCellForeColor(RGB(51, 102, 153));
//
//	//		switch (nTmp)
//	//		{
//	//		case 0: // Check
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 300); //300
//	//			m_ctrlSummaryGrid.SetColDataType(nTmp, flexDTBoolean);
//	//			m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
//	//			break;
//
//	//		case 1: // No
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 400);  //500
//	//			break;
//
//	//		case 2: // Step
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignLeftCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 2400); //3100
//	//			break;
//
//	//		case 3: // Result
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
//	//			break;
//
//	//		case 4: // Measure
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 700);
//	//			break;
//
//	//		case 5: // Target
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
//	//			break;
//
//	//		case 6: // L-Limit
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
//	//			break;
//
//	//		case 7: // U-Limit
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
//	//			break;
//
//	//		case 8: // Unit
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 650);
//	//			break;
//
//	//		case 9: // Audio Target
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
//	//			break;
//	//		case 10: // Audio Measure
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
//	//			break;
//	//		case 11: // Elapsed Time
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 760);
//	//			break;
//
//	//		case 12: // Message
//	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
//	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 5000);
//	//			break;
//	//		}
//	//	}
//
//		if (StepList.GetCount() > 0)
//		{
//			nTmp = 0;
//
//			while (Position)
//			{
//				pStep = StepList.GetNext(Position);
//			
//				//m_ctrlSummaryGrid.SetRow(nTmp);
//
//				////(Column 0) Step Check 
//				//m_ctrlSummaryGrid.SetCol(0);
//				//m_ctrlSummaryGrid.SetColDataType(0, flexDTBoolean);
//
//				
//
//				//if (pStep->GetTest() == TRUE) m_ctrlSummaryGrid.SetCellChecked(flexChecked);
//				//else m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
//
//				////(Column 1) Step No
//				sTmp.Format("%d", pStep->GetStepNo());
//				//m_ctrlSummaryGrid.SetCol(1);
//				//m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
//				//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 1, sTmp);
//
//
//				//strTemp.Format(_T("%d"), i + 1);
//				m_CtrlListMainProcess.InsertItem(nTmp, sTmp);
//				if (pStep->GetTest() == TRUE) m_CtrlListMainProcess.SetCheck(nTmp, 1); //m_ctrlSummaryGrid.SetCellChecked(flexChecked);
//				else m_CtrlListMainProcess.SetCheck(nTmp, 0); //m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
//
//				//(Column 2) Step Name
//				//m_ctrlSummaryGrid.SetCol(2);
//				//m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
//				//if (pStep->m_nTestType == ADC_TEST || pStep->m_nTestType == TV_COMM_TEST)
//				//{
//				//	m_ctrlSummaryGrid.SetCellBackColor(RGB(189, 231, 97));
//				//	//m_ctrlSummaryGrid.SetCellForeColor(RGB(	255,	 255,   255 ));
//				//}
//				//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 2, pStep->GetStepName());
//				m_CtrlListMainProcess.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetStepName(), NULL, NULL, NULL, NULL);
//				if (pStep->m_nTestType == ADC_TEST || pStep->m_nTestType == TV_COMM_TEST)
//				{
//				//	m_CtrlListMainProcess.SetTextBkColor(RGB(189, 231, 97));//m_ctrlSummaryGrid.SetCellBackColor(RGB(189, 231, 97));
//					
//				}
//				
//
//				////(Column 4) Measure
//				//m_ctrlSummaryGrid.SetCol(4);
//				//m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
//
//				if (pStep->m_nStepType == CHECK_FUNCTION)
//				{
//					if (pStep->GetFuncType() == MEAS_BOOL)
//					{
//						if (pStep->GetNominal_Bool() == FALSE) sTmp.Format(_T("%s"), "FALSE");
//						else sTmp.Format(_T("%s"), "TRUE");
//					}
//					else sTmp.Format("%4.1f", pStep->GetNominal_float());
//
//					// (Column 5) Target
//					//m_ctrlSummaryGrid.SetCol(5);
//					//m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
//					//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 5, sTmp);
//					m_CtrlListMainProcess.SetItem(nTmp, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
//
//					if (pStep->GetFuncType() != MEAS_BOOL)
//					{
//						// (Column 6) Low Limit
//						sTmp.Format("%4.1f", pStep->GetLowLimit());
//						//m_ctrlSummaryGrid.SetCol(6);
//						//m_ctrlSummaryGrid.SetColDataType(6, flexDTString);
//						//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 6, sTmp);
//						m_CtrlListMainProcess.SetItem(nTmp, 5, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
//
//						// (Column 7) High Limit
//						sTmp.Format("%4.1f", pStep->GetHighLimit());
//						//m_ctrlSummaryGrid.SetCol(7);
//						//m_ctrlSummaryGrid.SetColDataType(7, flexDTString);
//						//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 7, sTmp);
//						m_CtrlListMainProcess.SetItem(nTmp, 6, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
//
//						// (Column 8) Unit
//						sTmp.Format(_T("%s"), pStep->GetUnit());
//						//m_ctrlSummaryGrid.SetCol(8);
//						//m_ctrlSummaryGrid.SetColDataType(8, flexDTString);
//						//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 8, sTmp);
//						m_CtrlListMainProcess.SetItem(nTmp, 7, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
//
//
//					}
//					//+2007.10.15 Add BY USY
//					if (pStep->m_bRunAudioTest == TRUE)
//					{
//						// (Column 9) Audio Target
//						// change 090323
//						if (pStep->m_nMeasureAudioType == LEVEL_CHECK) {
//							sTmp.Format(_T("X, X, %d, %d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
//						}
//						else if (pStep->m_nMeasureAudioType == FREQUENCY_CHECK) {
//							sTmp.Format(_T("%d, %d, X, X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
//						}
//						else {
//							sTmp.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
//						}
//
//						//m_ctrlSummaryGrid.SetCol(9);
//						//m_ctrlSummaryGrid.SetColDataType(9, flexDTString);
//						//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 9, sTmp);
//						m_CtrlListMainProcess.SetItem(nTmp, 8, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
//					}
//					//-
//				}
//				nTmp++;
//			}
//			////Result Row
//			//;
//			////(Column 1) Step No
//			////sTmp.Format("%d", StepList.GetCount() + 1);
//			//m_ctrlSummaryGrid.SetCol(1);
//			//m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
//			//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 1, _T(""));
//
//			////(Column 2) Step Name
//			//m_ctrlSummaryGrid.SetCol(2);
//			//m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
//			//m_ctrlSummaryGrid.SetTextMatrix(nTmp, 2, "Total");
//
//			//m_ctrlSummaryGrid.SetRow(0);
//			//m_ctrlSummaryGrid.SetCol(0);
//			//m_ctrlSummaryGrid.SetCellChecked(flexChecked);
//			////+ 2007.10.10 Add BY USY
//			//m_ctrlSummaryGrid.SetCellBackColor(RGB(247, 247, 231));
//			////			m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
//			//m_ctrlSummaryGrid.SetGridColor(RGB(200, 200, 176));
//			//-
//		//	sTmp.Format("%d", pStep->GetStepNo());
//	
//			m_CtrlListMainProcess.InsertItem(nTmp, " ");
//			m_CtrlListMainProcess.SetCheck(nTmp, 1); //m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
//			m_CtrlListMainProcess.InsertItem(nTmp, "Total");
//			//m_CtrlListMainProcess.SetCellBackColor(RGB(247, 247, 231));
//			//m_CtrlListMainProcess.SetGridColor(RGB(200, 200, 176));
//		}
//	}
//	//m_ctrlSummaryGrid.SetCol(-1);
//	//m_ctrlSummaryGrid.SetRow(-1);
//	//m_ctrlSummaryGrid.SetRedraw(TRUE);
//	//m_ctrlSummaryGrid.Refresh();
//}


#if 1
void CDATsysView::InsertStepData2Grid(int nGridType)
{
	//char *sHeader2[13] = { "", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE" };
	//int  nWidth[13] = { 30,  140, 90, 70, 90, 75, 75, 65, 200, 200, 76, 500, };

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
				m_CtrlListMainProcess.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetStepName(), NULL, NULL, NULL, NULL);
				// m_ctrlSummaryGrid.SetTextMatrix(nTmp, 2, pStep->GetStepName());


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
					m_CtrlListMainProcess.SetItem(nTmp, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nTmp, 5, sTmp);



					if (pStep->GetFuncType() != MEAS_BOOL)
					{
						// (Column 6) Low Limit
						sTmp.Format("%4.1f", pStep->GetLowLimit());
						//m_ctrlSummaryGrid.SetCol(6);
						//m_ctrlSummaryGrid.SetColDataType(6, flexDTString);
						m_CtrlListMainProcess.SetItem(nTmp, 5, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 6, sTmp);

						// (Column 7) High Limit
						sTmp.Format("%4.1f", pStep->GetHighLimit());
						//m_ctrlSummaryGrid.SetCol(7);
						//m_ctrlSummaryGrid.SetColDataType(7, flexDTString);
						m_CtrlListMainProcess.SetItem(nTmp, 6, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 7, sTmp);

						// (Column 8) Unit
						sTmp.Format(_T("%s"), pStep->GetUnit());
						//	m_ctrlSummaryGrid.SetCol(8);
						//	m_ctrlSummaryGrid.SetColDataType(8, flexDTString);
						m_CtrlListMainProcess.SetItem(nTmp, 7, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 8, sTmp);


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
						m_CtrlListMainProcess.SetItem(nTmp, 8, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nTmp, 9, sTmp);
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
	//m_CtrlListMainProcess.Invalidate();
	return;
}


#else
void CDATsysView::InsertStepData2Grid(int nGridType)
{
	CString StrTmpLog;
	StrTmpLog.Format("InsertStepData2Grid(int nGridType) (%Ts:%d)", _T(__FILE__), __LINE__);
	SystemMonitorLog_Save(StrTmpLog);

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
		//	if(StepList.GetCount() + CurrentSet->VF_TestInfo.GetCount() + 1 < 12) m_ctrlSummaryGrid.SetRows(12);
		//	else m_ctrlSummaryGrid.SetRows(StepList.GetCount() + CurrentSet->VF_TestInfo.GetCount() + 2);
			if(StepList.GetCount() + 1 < 12) m_ctrlSummaryGrid.SetRows(12);
			else m_ctrlSummaryGrid.SetRows(StepList.GetCount()  + 2);
		}
	}
	else m_ctrlSummaryGrid.SetRows(12);

	//+Insert Step Default Data
	POSITION Position;
	CStep* pStep;
	CString sTmp;
	int nTmp;
	Position = StepList.GetHeadPosition();

	if(nGridType == DETAILEDGRID)
	{
		m_ctrlSummaryGrid.SetRow(0);
//		m_ctrlSummaryGrid.SetFrozenCols(3);
		m_ctrlSummaryGrid.SetFrozenCols(2);

		for(nTmp = 0; nTmp < 13 ; nTmp++)
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
				/*if (nTmp < 9)
				{
					nTmp++;
					continue;
				}*/
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
	m_ctrlSummaryGrid.SetRedraw(TRUE);
	m_ctrlSummaryGrid.Refresh();
	return;
}
#endif


#if 1


void CDATsysView::InsertStepData2GridUpdate(int nGridType, int lStartStepNo)
{

	InsertStepData2Grid(nGridType);

	return;
}
#else

void CDATsysView::InsertStepData2GridUpdate(int nGridType, int lStartStepNo)
{


	//m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	//m_ctrlSummaryGrid.Clear(COleVariant(long(flexClearScrollable)), COleVariant(long(flexClearEverything)));

	//+ 2007.10.10 Add BY USY
	//char *sHeader1[] = { "", "NO","STEP", "RESULT", "", "NO", "STEP", "RESULT", "", "NO", "STEP", "RESULT" };
	//char *sHeader2[] = { "", "NO","STEP", "RESULT", "MEAS", "TARGET", "L-LIMIT", "U-LIMIT", "UNIT","A/T(FREQ,LVL)", "A/M(FREQ,LVL)", "TIME", "MESSAGE" };
	//-

	//Row 
	if (CurrentSet->bCompiled)
	{
		if (nGridType == DETAILEDGRID)
		{
			//	if(StepList.GetCount() + CurrentSet->VF_TestInfo.GetCount() + 1 < 12) m_ctrlSummaryGrid.SetRows(12);
			//	else m_ctrlSummaryGrid.SetRows(StepList.GetCount() + CurrentSet->VF_TestInfo.GetCount() + 2);
			if (StepList.GetCount() + 1 < 12) m_ctrlSummaryGrid.SetRows(12);
			else m_ctrlSummaryGrid.SetRows(StepList.GetCount() + 2);
		}
	}
	else m_ctrlSummaryGrid.SetRows(12);

	//+Insert Step Default Data
	POSITION Position;
	CStep* pStep;
	CString sTmp;
	int nTmp;
	Position = StepList.GetHeadPosition();

	if (nGridType == DETAILEDGRID)
	{
	//	m_ctrlSummaryGrid.SetRow(0);
	//	//		m_ctrlSummaryGrid.SetFrozenCols(3);
	//	m_ctrlSummaryGrid.SetFrozenCols(2);

	//	for (nTmp = 0; nTmp < 13; nTmp++)
	//	{
	//		m_ctrlSummaryGrid.SetCol(nTmp);
	//		m_ctrlSummaryGrid.SetText(sHeader2[nTmp]);
	//		m_ctrlSummaryGrid.SetCellBackColor(RGB(210, 216, 176));
	//		m_ctrlSummaryGrid.SetCellForeColor(RGB(51, 102, 153));

	//		switch (nTmp)
	//		{
	//		case 0: // Check
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 300); //300
	//			m_ctrlSummaryGrid.SetColDataType(nTmp, flexDTBoolean);
	//			m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
	//			break;

	//		case 1: // No
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 400);  //500
	//			break;

	//		case 2: // Step
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignLeftCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 2400); //3100
	//			break;

	//		case 3: // Result
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
	//			break;

	//		case 4: // Measure
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 700);
	//			break;

	//		case 5: // Target
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 900);
	//			break;

	//		case 6: // L-Limit
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
	//			break;

	//		case 7: // U-Limit
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignRightCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 750);
	//			break;

	//		case 8: // Unit
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 650);
	//			break;

	//		case 9: // Audio Target
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
	//			break;
	//		case 10: // Audio Measure
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 2000);
	//			break;
	//		case 11: // Elapsed Time
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 760);
	//			break;

	//		case 12: // Message
	//			m_ctrlSummaryGrid.SetColAlignment(nTmp, flexAlignCenterCenter);
	//			m_ctrlSummaryGrid.SetColWidth(nTmp, 5000);
	//			break;
	//		}
	//	}

		if (StepList.GetCount() > 0)
		{
			nTmp = 1;

			while (Position)
			{
				pStep = StepList.GetNext(Position);
				if (nTmp < lStartStepNo)
				{
					nTmp++;
					continue;
				}
				m_ctrlSummaryGrid.SetRow(nTmp);

				//(Column 0) Step Check 
				m_ctrlSummaryGrid.SetCol(0);
				m_ctrlSummaryGrid.SetColDataType(0, flexDTBoolean);

				if (pStep->GetTest() == TRUE) m_ctrlSummaryGrid.SetCellChecked(flexChecked);
				else m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);

				//(Column 1) Step No
				sTmp.Format("%d", pStep->GetStepNo());
				m_ctrlSummaryGrid.SetCol(1);
				m_ctrlSummaryGrid.SetColDataType(1, flexDTString);
				m_ctrlSummaryGrid.SetTextMatrix(nTmp, 1, sTmp);

				//(Column 2) Step Name
				m_ctrlSummaryGrid.SetCol(2);
				m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
				if (pStep->m_nTestType == ADC_TEST || pStep->m_nTestType == TV_COMM_TEST)
				{
					m_ctrlSummaryGrid.SetCellBackColor(RGB(189, 231, 97));
					//m_ctrlSummaryGrid.SetCellForeColor(RGB(	255,	 255,   255 ));
				}
				m_ctrlSummaryGrid.SetTextMatrix(nTmp, 2, pStep->GetStepName());


				//(Column 4) Measure
				m_ctrlSummaryGrid.SetCol(4);
				m_ctrlSummaryGrid.SetColDataType(4, flexDTString);

				if (pStep->m_nStepType == CHECK_FUNCTION)
				{
					if (pStep->GetFuncType() == MEAS_BOOL)
					{
						if (pStep->GetNominal_Bool() == FALSE) sTmp.Format(_T("%s"), "FALSE");
						else sTmp.Format(_T("%s"), "TRUE");
					}
					else sTmp.Format("%4.1f", pStep->GetNominal_float());

					// (Column 5) Target
					m_ctrlSummaryGrid.SetCol(5);
					m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
					m_ctrlSummaryGrid.SetTextMatrix(nTmp, 5, sTmp);

					if (pStep->GetFuncType() != MEAS_BOOL)
					{
						// (Column 6) Low Limit
						sTmp.Format("%4.1f", pStep->GetLowLimit());
						m_ctrlSummaryGrid.SetCol(6);
						m_ctrlSummaryGrid.SetColDataType(6, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp, 6, sTmp);

						// (Column 7) High Limit
						sTmp.Format("%4.1f", pStep->GetHighLimit());
						m_ctrlSummaryGrid.SetCol(7);
						m_ctrlSummaryGrid.SetColDataType(7, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp, 7, sTmp);

						// (Column 8) Unit
						sTmp.Format(_T("%s"), pStep->GetUnit());
						m_ctrlSummaryGrid.SetCol(8);
						m_ctrlSummaryGrid.SetColDataType(8, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp, 8, sTmp);


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

						m_ctrlSummaryGrid.SetCol(9);
						m_ctrlSummaryGrid.SetColDataType(9, flexDTString);
						m_ctrlSummaryGrid.SetTextMatrix(nTmp, 9, sTmp);
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
			m_ctrlSummaryGrid.SetTextMatrix(nTmp, 1, _T(""));

			//(Column 2) Step Name
			m_ctrlSummaryGrid.SetCol(2);
			m_ctrlSummaryGrid.SetColDataType(2, flexDTString);
			m_ctrlSummaryGrid.SetTextMatrix(nTmp, 2, "Total");

			m_ctrlSummaryGrid.SetRow(0);
			m_ctrlSummaryGrid.SetCol(0);
			m_ctrlSummaryGrid.SetCellChecked(flexChecked);
			//+ 2007.10.10 Add BY USY
			m_ctrlSummaryGrid.SetCellBackColor(RGB(247, 247, 231));
			//			m_ctrlSummaryGrid.SetGridColor(RGB(210,216,176));
			m_ctrlSummaryGrid.SetGridColor(RGB(200, 200, 176));
			//-
		}
	}
	m_ctrlSummaryGrid.SetCol(-1);
	m_ctrlSummaryGrid.SetRow(-1);
	m_ctrlSummaryGrid.SetRedraw(TRUE);
	m_ctrlSummaryGrid.Refresh();
	return;
}
#endif
void CDATsysView::InsertResultData2Grid(int nGridType, int nStepNo)
{

	if(nGridType == DETAILEDGRID) InsertResult2DetailedGrid(nStepNo);
}

//void CDATsysView::InsertResultData2Grid_EM(int nFailNo)
//{
//
// 	int nStepCount1 = StepList.GetCount();
//	CString sTmp;
//	int nCH;
//	int nIndex = 0;
//
//    CVFTestInfo *pStep;
//	POSITION pos;
//
//
//	if(StepList.GetCount() + nFailNo + 1 > 12){
//		m_ctrlSummaryGrid.SetRows(StepList.GetCount() + nFailNo + 2);
//	}
//
//	//g_nStepSize = 0;
//	if(CurrentSet->VF_TestInfo.GetCount() == 0){return;}
//
//	pos = CurrentSet->VF_TestInfo.GetHeadPosition();
//	while(pos != NULL)
//	{
//		pStep = CurrentSet->VF_TestInfo.GetNext(pos);
//		
//		if(!pStep->bResult){
//			nIndex ++;
//
//			nCH = pStep->nChNo;
//			if(nCH < 43){
//				sTmp.Format(_T("V_%d"), nCH);
//			}
//			else{
//				sTmp.Format(_T("F_%d"), nCH-100);
//			}
//			m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nIndex, 1, sTmp);
//			m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nIndex, 2, pStep->strTestName);
//		
//			sTmp = _T("NG");
//			m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nIndex, 3, sTmp);
//
//			if(pStep->nCheckType == 1){
//				sTmp.Format(_T("%4.1f"), pStep->dMaesure_Min);
//			}
//			else{
//				sTmp.Format(_T("%4.1f, %4.1f"), pStep->dMaesure_Min, pStep->dMaesure_Max);
//			}
//			m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nIndex, 4, sTmp);
//
//			sTmp.Format(_T("%4.1f"), pStep->dLowerLimit);
//			m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nIndex, 6, sTmp);
//					
//			sTmp.Format(_T("%4.1f"), pStep->dUpperLimit);
//			m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nIndex, 6, sTmp);
//		}
//	}
//	return ;
//}

//void CDATsysView::InsertResult2NormalGrid(int nStepNo)
//{
//
//	if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;
//
//	CString sTmp;
//
//	int nRow, nCol; // nCol = 3 or 7 or 11
//	nRow = ((nStepNo - 1) / 3) + 1;
//
//	m_ctrlSummaryGrid.SetRedraw(FALSE);
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
//	if(pCurStep->m_bTest == TRUE)
//	{
//		if (pCurStep->m_bResult)
//		{
//			sTmp = _T("OK");
//			m_ctrlSummaryGrid.SetCellBackColor(RGB(0,0,255));
//			m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
//			m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//			
//		}
//		else // Video Test NG
//		{
//			//090324
//			sTmp = _T("NG");
//			if(pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
//			{
//
//				if(pCurStep->m_bVideoTestResult == FALSE)
//				{
//					sTmp = _T("NG/V");
//				}
//
//				if(pCurStep->m_bAudioTestResult == FALSE)
//				{
//					sTmp = _T("NG/A");
//				}
//
//				if((pCurStep->m_bVideoTestResult == FALSE) && (pCurStep->m_bAudioTestResult == FALSE))
//				{
//					sTmp = _T("NG/AV");
//				}
//			}
//			else if(!pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
//			{
//				if(pCurStep->m_bVideoTestResult == FALSE)
//				{
//					sTmp = _T("NG/V");
//				}
//			}
//			else if(pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
//			{
//				if(pCurStep->m_bAudioTestResult == FALSE)
//				{
//					sTmp = _T("NG/A");
//				}
//			}
//			else
//			{
//				sTmp = _T("NG");
//			}
//				
//			m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
//			m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
//			m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//		}
//	}
//	else if(pCurStep->m_bTest == FALSE)
//	{
//		sTmp = _T("SKIP");
//		m_ctrlSummaryGrid.SetTextMatrix(nRow, nCol, sTmp);
//	}
//	
//	//+2007.10.15 Add BY USY
//	m_ctrlSummaryGrid.SetRow(-1);
//	m_ctrlSummaryGrid.SetCol(-1);
//	//-
//	m_ctrlSummaryGrid.SetRedraw(TRUE);
//	m_ctrlSummaryGrid.Refresh();
//	return ;
//}
#if 1
void CDATsysView::InsertResult2DetailedGrid(int nStepNo)
{
	if ((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return;
	nStepNo -= 1;
	CString sTmp;
	//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	//	m_ctrlSummaryGrid.SetRow(nStepNo);

		//if (nStepNo % 7 == 0) m_CtrlListMainProcess.Scroll(nStepNo*20);//.settop.SetTopRow(nStepNo);

	if (nStepNo == 0)
	{
		sTmp = "HDMI Gen Ver :";
		sTmp += HDMIGeneratorCtrl.m_FW_Ver;
		m_CtrlListMainProcess.SetItemText(nStepNo, 11, sTmp); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);
	}
	if (pCurStep->m_bTest) // == TRUE)
	{
		sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);

		m_CtrlListMainProcessEx.SetCellBackColor(nStepNo, 2, RGB(255, 255, 128));
		//m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, "PASS", NULL, NULL, NULL, NULL);
		m_CtrlListMainProcess.SetItem(nStepNo, 10, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
		if (pCurStep->m_nStepType == PROCESS_FUNCTION)
		{
//			m_CtrlListMainProcess.Invalidate();
			m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, "PASS", NULL, NULL, NULL, NULL);
			return;
		}
	}


	//===============
	// Test Function
	//===============
	if (pCurStep->m_bTest) // == TRUE)
	{
		//===================
		// (Column 3) Result
		//===================
		if (pCurStep->m_bResult) // PASS
		{
			sTmp = _T("OK");
			//m_ctrlSummaryGrid.SetCol(3);
			//m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
			m_CtrlListMainProcessEx.SetCellBackColor(nStepNo, 2, RGB(0, 0, 255));
			m_CtrlListMainProcessEx.SetCellForeColor(nStepNo, 2, RGB(255, 255, 255));
			//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);

			m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
		}
		// NG
		else
		{
			//090324
			sTmp = _T("NG");
			if (pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{

				if (pCurStep->m_bVideoTestResult == FALSE)
				{
					sTmp = _T("NG/V");
				}

				if (pCurStep->m_bAudioTestResult == FALSE)
				{
					sTmp = _T("NG/A");
				}

				if ((pCurStep->m_bVideoTestResult == FALSE) && (pCurStep->m_bAudioTestResult == FALSE))
				{
					sTmp = _T("NG/AV");
				}
			}
		else if (!pCurStep->m_bRunAudioTest && pCurStep->m_bRunVideoTest)
			{
				if (pCurStep->m_bVideoTestResult == FALSE)
				{
					sTmp = _T("NG/V");
				}
			}
			else if (pCurStep->m_bRunAudioTest && !pCurStep->m_bRunVideoTest)
			{
				if (pCurStep->m_bAudioTestResult == FALSE)
				{
					sTmp = _T("NG/A");
				}
			}
			else
			{
				sTmp = _T("NG");
			}
						
			m_CtrlListMainProcessEx.SetCellBackColor(nStepNo, 2, RGB(255, 0, 0));
			m_CtrlListMainProcessEx.SetCellForeColor(nStepNo, 2, RGB(255, 255, 255));
			m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL);
		}
		if (pCurStep->m_bRunAudioTest)
		{
			sTmp.Format(_T("%d, %d, %d, %d"), pCurStep->m_nLeftFreq[1], pCurStep->m_nRightFreq[1], pCurStep->m_nLeftLevel[1], pCurStep->m_nRightLevel[1]);

			//m_ctrlSummaryGrid.SetCol(10);
			//m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			m_CtrlListMainProcess.SetItem(nStepNo, 9, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); 
			// m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
		}

		//+add kwmoon 081013
		if (pCurStep->m_bAdcValueCheckStep)
		{
			//========================================================
			// (Column 10) Display ADC value instead of Audio Measure
			//========================================================
			sTmp = pCurStep->m_szAdcValue;
			//m_ctrlSummaryGrid.SetCol(10);
			//m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			m_CtrlListMainProcess.SetItem(nStepNo, 9, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); 
			// m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
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
		Sleep(50);

		return;
	}

	// (Column 4) Measure
	if (pCurStep->m_nFuncType == MEAS_BOOL)
	{
		if (pCurStep->m_bMeasured == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
	}
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fMeasured);

	//if (pCurStep->m_nTestType == MULTICHECL_TEST) {
	//	sTmp.Format(_T("%.02f, %.02f"), pCurStep->m_fMeasured_L, pCurStep->m_fMeasured_R);
	//}

	//m_ctrlSummaryGrid.SetCol(4);
	//m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 3, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, sTmp);

	// (Column 5) Target
	if (pCurStep->m_nFuncType == MEAS_BOOL)
	{
		if (pCurStep->m_bNominal == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
	}
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fNominal);

	//m_ctrlSummaryGrid.SetCol(5);
	//m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 4, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);



	// (Column 11) Elapsed Time
	sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);
	//m_ctrlSummaryGrid.SetCol(11);
	//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 10, LVIF_TEXT, sTmp, NULL, NULL, NULL, NULL); // m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);

	//m_ctrlSummaryGrid.SetRedraw(-1);
	//m_ctrlSummaryGrid.Refresh();
//	m_CtrlListMainProcess.Invalidate();

	return;
}
#else
void CDATsysView::InsertResult2DetailedGrid(int nStepNo)
{

	if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

    CString sTmp;
	m_ctrlSummaryGrid.SetRedraw(FALSE);
	m_ctrlSummaryGrid.SetRow(nStepNo);

	if(nStepNo % 7 == 0) m_ctrlSummaryGrid.SetTopRow(nStepNo);
		
	// (Column 11) Elapsed Time
 	sTmp.Format(_T("%5.2f"), pCurStep->m_fElapsedTime);

	m_ctrlSummaryGrid.SetCol(11);
	m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);
		

	//===============
	// Test Function
	//===============
	if(pCurStep->m_bTest) // == TRUE)
	{
	   if(pCurStep->m_nStepType == PROCESS_FUNCTION)
		{
			m_ctrlSummaryGrid.SetRedraw(TRUE);
			m_ctrlSummaryGrid.Refresh();

			return;
		}


		//===================
		// (Column 3) Result
		//===================
	    if (pCurStep->m_bResult) // PASS
		{
			sTmp = _T("OK");
			m_ctrlSummaryGrid.SetCol(3);
			m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
			m_ctrlSummaryGrid.SetCellBackColor(RGB(0,0,255));
			m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
			m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);
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

			m_ctrlSummaryGrid.SetCol(3);
			m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
			m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
			m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
			m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);
		}
		if(pCurStep->m_bRunAudioTest)
		{
			sTmp.Format(_T("%d, %d, %d, %d"), pCurStep->m_nLeftFreq[1], pCurStep->m_nRightFreq[1], pCurStep->m_nLeftLevel[1], pCurStep->m_nRightLevel[1]);

			m_ctrlSummaryGrid.SetCol(10);
			m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
		}
		
		//+add kwmoon 081013
		if(pCurStep->m_bAdcValueCheckStep)
		{
			//========================================================
			// (Column 10) Display ADC value instead of Audio Measure
			//========================================================
			sTmp = pCurStep->m_szAdcValue;
			m_ctrlSummaryGrid.SetCol(10);
			m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
			m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, sTmp);
		}
	}
	//======
	// SKIP
	//======
	else // if(pCurStep->m_bTest == FALSE)
	{
		sTmp = _T("SKIP");
		m_ctrlSummaryGrid.SetCol(3);
		m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
		m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, sTmp);
		m_ctrlSummaryGrid.SetRedraw(TRUE);
		m_ctrlSummaryGrid.Refresh();
		return;
	}

	// (Column 4) Measure
	if(pCurStep->m_nFuncType == MEAS_BOOL)
	{	
		if(pCurStep->m_bMeasured == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
    }
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fMeasured);

	m_ctrlSummaryGrid.SetCol(4);
	m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, sTmp);

	// (Column 5) Target
	if(pCurStep->m_nFuncType == MEAS_BOOL)
	{	
		if(pCurStep->m_bNominal == FALSE) sTmp.Format(_T("%s"), "FALSE");
		else sTmp.Format(_T("%s"), "TRUE");
    }
	else sTmp.Format(_T("%4.1f"), pCurStep->m_fNominal);

	m_ctrlSummaryGrid.SetCol(5);
	m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);

							

	// (Column 11) Elapsed Time
//	sTmp.Format(_T("%5.1f"), pCurStep->m_fElapsedTime);
//	m_ctrlSummaryGrid.SetCol(11);
//	m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
//	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, sTmp);

	m_ctrlSummaryGrid.SetRedraw(TRUE);
	m_ctrlSummaryGrid.Refresh();
	return ;
}
#endif

#if 1
void CDATsysView::InsertMsg2DetailedGrid(int nStepNo, CString sMsg)
{
	CString szDetailMessage = sMsg;

	if ((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return;

	//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	//	m_ctrlSummaryGrid.SetRow(nStepNo);
	m_CtrlListMainProcess.SetItem(nStepNo - 1, 11, LVIF_TEXT, szDetailMessage, NULL, NULL, NULL, NULL); 
	//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 12, szDetailMessage);

//	m_ctrlSummaryGrid.SetRedraw(-1);
//	m_ctrlSummaryGrid.Refresh();
	//m_CtrlListMainProcess.Invalidate();
}

#else
void CDATsysView::InsertMsg2DetailedGrid(int nStepNo, CString sMsg)
{

	CString szDetailMessage = sMsg;

	if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

	m_ctrlSummaryGrid.SetRedraw(FALSE);
	m_ctrlSummaryGrid.SetRow(nStepNo);
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 12, szDetailMessage);
	
	m_ctrlSummaryGrid.SetRedraw(TRUE);
	m_ctrlSummaryGrid.Refresh();
}
#endif

#if 1

void CDATsysView::InsertTotalData2Grid(int nGridType, BOOL bResult, CString sElapsedTime)
{
	if (nGridType == DETAILEDGRID)
	{
		if (StepList.GetCount() > 0)
		{
			CString sTmp;
			//m_ctrlSummaryGrid.SetRedraw(flexRDNone);
			//m_ctrlSummaryGrid.SetCol(3);
			//m_ctrlSummaryGrid.SetRow(StepList.GetCount() + 1);
			if (bResult == TRUE)
			{
				//	m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				m_CtrlListMainProcessEx.SetCellBackColor(StepList.GetCount(), 2, RGB(0, 0, 255));
				m_CtrlListMainProcessEx.SetCellForeColor(StepList.GetCount(), 2, RGB(255, 255, 255));
				m_CtrlListMainProcess.SetItem(StepList.GetCount(), 2, LVIF_TEXT, "OK", NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "OK");
			}
			else if (bResult == FALSE)
			{

				//	m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				m_CtrlListMainProcessEx.SetCellBackColor(StepList.GetCount(), 2, RGB(255, 0, 0));
				m_CtrlListMainProcessEx.SetCellForeColor(StepList.GetCount(), 2, RGB(255, 255, 255));
				m_CtrlListMainProcess.SetItem(StepList.GetCount(), 2, LVIF_TEXT, "NG", NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "NG");
			}


			//	m_ctrlSummaryGrid.SetCol(11);
				//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
			m_CtrlListMainProcess.SetItem(StepList.GetCount(), 10, LVIF_TEXT, sElapsedTime, NULL, NULL, NULL, NULL); 
			//m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 11, sElapsedTime);

			m_CtrlListMainProcess.SetItem(StepList.GetCount(), 3, LVIF_TEXT, sElapsedTime, NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 11, sElapsedTime);

		//	m_ctrlSummaryGrid.SetRedraw(-1);
		//	m_ctrlSummaryGrid.Refresh();
		//	m_CtrlListMainProcess.Invalidate();
		}
	}
}

#else
void CDATsysView::InsertTotalData2Grid(int nGridType, BOOL bResult, CString sElapsedTime)
{

	if(nGridType == DETAILEDGRID)
	{
		if(StepList.GetCount() > 0)
		{
			CString sTmp;
			m_ctrlSummaryGrid.SetRedraw(FALSE);
			m_ctrlSummaryGrid.SetCol(3);
			m_ctrlSummaryGrid.SetRow(StepList.GetCount() + 1);
			if(bResult == TRUE)
			{
				m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				m_ctrlSummaryGrid.SetCellBackColor(RGB(0,0,255));
				m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
				m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "OK");
			}
			else if(bResult == FALSE)
			{
				
				m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
				m_ctrlSummaryGrid.SetCellBackColor(RGB(255,0,0));
				m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
				m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 3, "NG");
			}


			m_ctrlSummaryGrid.SetCol(11);
			m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
			m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 11, sElapsedTime);
			
			m_ctrlSummaryGrid.SetRedraw(TRUE);
			m_ctrlSummaryGrid.Refresh();
		}
	}
}
#endif

#if 1
void CDATsysView::InsertPcbid2Grid(CString sWipID)
{
	if (StepList.GetCount() > 0)
	{
		//m_ctrlSummaryGrid.SetCol(12);
		//m_ctrlSummaryGrid.SetColDataType(12, flexDTString);
		m_CtrlListMainProcess.SetItem(StepList.GetCount(), 11, LVIF_TEXT, sWipID, NULL, NULL, NULL, NULL); 
		// m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 12, sWipID);

	//	m_ctrlSummaryGrid.SetRedraw(-1);
	//	m_ctrlSummaryGrid.Refresh();
	//	m_CtrlListMainProcess.Invalidate();
	}
}

#else
void CDATsysView::InsertPcbid2Grid(CString sWipID)
{

	if(StepList.GetCount() > 0)
	{
		m_ctrlSummaryGrid.SetRedraw(FALSE);

		m_ctrlSummaryGrid.SetCol(12);
		m_ctrlSummaryGrid.SetColDataType(12, flexDTString);
		m_ctrlSummaryGrid.SetTextMatrix(StepList.GetCount() + 1, 12, sWipID);
		
		m_ctrlSummaryGrid.SetRedraw(TRUE);
		m_ctrlSummaryGrid.Refresh();
	}
}
#endif

//-

void CDATsysView::DeleteResultDataFromGrid(int nGridType, int nStepNo)
{

	if(nGridType == DETAILEDGRID) DeleteResultDataFromDetailedGrid(nStepNo);
}
/*
void CDATsysView::DeleteResultDataFromGrid_EM(int nStepNo)
{
 	int nStepCount1 = StepList.GetCount();

	if(((nStepCount1 + 2 + nStepNo) > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

    CString sTmp;
	m_ctrlSummaryGrid.SetRedraw(FALSE);
	m_ctrlSummaryGrid.SetRow(nStepCount1 + 2 + nStepNo);

	// (Column 11) Elapsed Time
    if(pCurStep_EM->m_nStepType == PROCESS_FUNCTION)
	{
		m_ctrlSummaryGrid.SetCol(11);
		m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
		m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nStepNo, 11, _T(""));
		
		m_ctrlSummaryGrid.SetRedraw(TRUE);
		m_ctrlSummaryGrid.Refresh();

		return;
	}

	// (Column 3) Result
	m_ctrlSummaryGrid.SetCol(3);
	m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
	m_ctrlSummaryGrid.SetCellBackColor(RGB(255,255,255));
	m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
	m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nStepNo, 3, _T(""));
	   
	// (Column 4) Measure
	m_ctrlSummaryGrid.SetCol(4);
	m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nStepNo, 4, _T(""));

	m_ctrlSummaryGrid.SetCol(5);
	m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nStepNo, 5, _T(""));


	// (Column 11) Elapsed Time
	m_ctrlSummaryGrid.SetCol(11);
	m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepCount1 + 2 + nStepNo, 11, _T(""));

	m_ctrlSummaryGrid.SetRow(0);
	m_ctrlSummaryGrid.SetCol(0);
	m_ctrlSummaryGrid.SetRedraw(TRUE);
	m_ctrlSummaryGrid.Refresh();
	return ;
}
*/
//void CDATsysView::DeleteResultDataFromNormalGrid(int nStepNo)
//{
//
//	if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;
//
//	CString sTmp;
//
//	int nRow, nCol; // nCol = 3 or 7 or 11
//	nRow = ((nStepNo - 1) / 3) + 1;
//
//	m_ctrlSummaryGrid.SetRedraw(FALSE);
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
//	m_ctrlSummaryGrid.SetRedraw(TRUE);
//	m_ctrlSummaryGrid.Refresh();
//	return ;
//}

#if 1

void CDATsysView::DeleteResultDataFromDetailedGrid(int nStepNo)
{
	if ((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return;

	CString sTmp;
	//	m_ctrlSummaryGrid.SetRedraw(flexRDNone);
	//	m_ctrlSummaryGrid.SetRow(nStepNo);

		//if(nStepNo % 7 == 0) m_CtrlListMainProcess.Scroll(nStepNo*20);

		// (Column 11) Elapsed Time
	if (pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		//m_ctrlSummaryGrid.SetCol(11);
		//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
		m_CtrlListMainProcess.SetItem(nStepNo, 10, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); 
		// m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));

	//	m_ctrlSummaryGrid.SetRedraw(-1);
	//	m_ctrlSummaryGrid.Refresh();

		return;
	}

	// (Column 3) Result
	//m_ctrlSummaryGrid.SetCol(3);
	//m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
	m_CtrlListMainProcessEx.SetCellBackColor(StepList.GetCount(), 2, RGB(255, 255, 255));
	m_CtrlListMainProcessEx.SetCellForeColor(StepList.GetCount(), 2, RGB(255, 255, 255));
	m_CtrlListMainProcess.SetItem(nStepNo, 2, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); 
	//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, _T(""));

	if (pCurStep->m_bRunAudioTest)
	{
		// (Column 10) Audio Measure
		//m_ctrlSummaryGrid.SetCol(10);
		//m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
		m_CtrlListMainProcess.SetItem(nStepNo, 9, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); 
		//m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, _T(""));

	}

	// (Column 4) Measure
//	m_ctrlSummaryGrid.SetCol(4);
//	m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 3, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); 
	//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, _T(""));

	//m_ctrlSummaryGrid.SetCol(5);
	//m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 4, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); //m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);


	// (Column 11) Elapsed Time
	//m_ctrlSummaryGrid.SetCol(11);
	//m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	m_CtrlListMainProcess.SetItem(nStepNo, 10, LVIF_TEXT, _T(""), NULL, NULL, NULL, NULL); 
	//m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));

	//m_ctrlSummaryGrid.SetRow(0);
	//m_ctrlSummaryGrid.SetCol(0);
	//m_ctrlSummaryGrid.SetRedraw(-1);
	//m_ctrlSummaryGrid.Refresh();
	//m_CtrlListMainProcess.Invalidate();
	return;
}

BOOL CDATsysView::GetResultFromDetailedGrid(int nStepNo, CString& sMsg)
{
	if ((nStepNo > (m_CtrlListMainProcess.GetItemCount() - 1)) || (nStepNo <= 0)) return FALSE;
	nStepNo -= 1;
	sMsg = m_CtrlListMainProcess.GetItemText(nStepNo, 11);
	//sMsg = m_ctrlSummaryGrid.GetTextMatrix(nStepNo, 12);

	return TRUE;
}
void CDATsysView::GetCheckStep(int nGridType)
{
	POSITION Position;
	CStep* pStep;
	int nRow, nCol, nTmp;

	Position = StepList.GetHeadPosition();
	if (StepList.GetCount() > 0)
	{
		nTmp = 0;
		nRow = 0;
		while (Position)
		{
			pStep = StepList.GetNext(Position);

			if (nGridType == DETAILEDGRID)
			{
				nCol = 0;
			}

			//g_pView->m_ctrlSummaryGrid.SetRow(nRow);
			//g_pView->m_ctrlSummaryGrid.SetCol(nCol);
			if (g_pView->m_CtrlListMainProcess.GetCheck(nRow) == 1)
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

	int nRow = 0;
	int nCol = 0;
	int nNoCheckedStep = 0;

	Position = StepList.GetHeadPosition();

	if (StepList.GetCount() > 0)
	{
		nRow = 1;

		while (Position)
		{
			pStep = StepList.GetNext(Position);

			if (nGridType == DETAILEDGRID)
			{
				nCol = 0;
			}

			//g_pView->m_ctrlSummaryGrid.SetRow(nRow);
		//	g_pView->m_ctrlSummaryGrid.SetCol(nCol);

			if (m_CtrlListMainProcess.GetCheck(nRow))
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
	sTxtPath.Replace(".dat", ".txt");
	sHtmlPath = sRstPath;
	sHtmlPath.Replace(".dat", ".html");
	sCsvPath = sRstPath;
	sCsvPath.Replace(".dat", ".csv");

	if (bSave2Txt == TRUE)
	{
		SaveGridToText(&m_CtrlListMainProcess, (char*)(LPCTSTR)sTxtPath); //g_pView->m_ctrlSummaryGrid.SaveGrid(sTxtPath, flexFileTabText, COleVariant(long(TRUE)));
	}

	if (bSave2Html == TRUE)
	{
		SaveGridToHTML(&m_CtrlListMainProcess, (char*)(LPCTSTR)sHtmlPath);
	}
	if (bSave2Csv == TRUE)
	{
		SaveGridToCSV(&m_CtrlListMainProcess, (char*)(LPCTSTR)sCsvPath);// g_pView->m_ctrlSummaryGrid.SaveGrid(sCsvPath, flexFileCommaText, COleVariant(long(TRUE)));
	}
}


#else
void CDATsysView::DeleteResultDataFromDetailedGrid(int nStepNo)
{

		if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return;

    CString sTmp;
	m_ctrlSummaryGrid.SetRedraw(FALSE);
	m_ctrlSummaryGrid.SetRow(nStepNo);

	if(nStepNo % 7 == 0) m_ctrlSummaryGrid.SetTopRow(nStepNo);
		
	// (Column 11) Elapsed Time
    if(pCurStep->m_nStepType == PROCESS_FUNCTION)
	{
		m_ctrlSummaryGrid.SetCol(11);
		m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
		m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));
		
		m_ctrlSummaryGrid.SetRedraw(TRUE);
		m_ctrlSummaryGrid.Refresh();

		return;
	}

	// (Column 3) Result
	m_ctrlSummaryGrid.SetCol(3);
	m_ctrlSummaryGrid.SetColDataType(3, flexDTString);
	m_ctrlSummaryGrid.SetCellBackColor(RGB(255,255,255));
	m_ctrlSummaryGrid.SetCellForeColor(RGB(255,255,255));
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 3, _T(""));
	   
	if(pCurStep->m_bRunAudioTest)
	{
		// (Column 10) Audio Measure
		m_ctrlSummaryGrid.SetCol(10);
		m_ctrlSummaryGrid.SetColDataType(10, flexDTString);
		m_ctrlSummaryGrid.SetTextMatrix(nStepNo , 10, _T(""));

	}

	// (Column 4) Measure
	m_ctrlSummaryGrid.SetCol(4);
	m_ctrlSummaryGrid.SetColDataType(4, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 4, _T(""));

	m_ctrlSummaryGrid.SetCol(5);
	m_ctrlSummaryGrid.SetColDataType(5, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 5, sTmp);


	// (Column 11) Elapsed Time
	m_ctrlSummaryGrid.SetCol(11);
	m_ctrlSummaryGrid.SetColDataType(11, flexDTString);
	m_ctrlSummaryGrid.SetTextMatrix(nStepNo, 11, _T(""));

	m_ctrlSummaryGrid.SetRow(0);
	m_ctrlSummaryGrid.SetCol(0);
	m_ctrlSummaryGrid.SetRedraw(TRUE);
	m_ctrlSummaryGrid.Refresh();
	return ;
}

BOOL CDATsysView::GetResultFromDetailedGrid(int nStepNo,CString& sMsg)
{

	if((nStepNo > (m_ctrlSummaryGrid.GetRows() - 1)) || (nStepNo <= 0)) return FALSE;
	
	sMsg = m_ctrlSummaryGrid.GetTextMatrix(nStepNo, 12);

	return TRUE;
}

void CDATsysView::GetCheckStep(int nGridType)
{


	POSITION Position;
	CStep* pStep;
	int nRow, nCol, nTmp;

	SystemMonitorLog_Save("GetCheckStep(int nGridType)");

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

#endif

void CDATsysView::OnDmmcotrol() 
{
	//CDMMDlg dlg;
	//dlg.DoModal();	
	
}

void CDATsysView::OnImgRotate() 
{
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		StopLVDSGrabThread();
	}
	if(CurrentSet->nImageRotation){
		CurrentSet->nImageRotation = 0;
	}
	else{
		CurrentSet->nImageRotation = 1;
	}
	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		Sleep(200);
		StartLVDSGrabThread();
	}
	else //FHD
	{
		SetGrabInfo(&g_GrabImage);
	}
}


int CDATsysView::GetGrabberVersion_FHD()
{
	int nResult  = 0;
	DWORD dwEvent = 1000;
	CString sMsg;
	CString szPrevMsg = _T("");

	if(g_pView->m_pImageInfo->nUseExtAnalogGrabber == 0)
		return 0;

	if(g_nRunningProcessNo == 1)
	{
		::SetEvent(hClientEvents[EVENT_GRABBER1_FW]);
	}
	else
	{
		::SetEvent(hClientEvents[EVENT_GRABBER2_FW]);
	}

	do
	{
		if(g_nRunningProcessNo == 1)
		{
			dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRABBER1_FW_COMPLETE],1500);			

			if(dwEvent == WAIT_OBJECT_0) {
				nResult = m_pImageInfo->nGrabber_FWVer; 
				dwEvent = -1; 

				sMsg.Format("Grabber F/W: %08X", nResult);
				AddStringToStatus(sMsg);

				::ResetEvent(hServerEvents[EVENT_GRABBER1_FW_COMPLETE]);
			}
			else
			{
				nResult = 0; 
				dwEvent = -1; 
			}
		}
		else
		{
			dwEvent =  WaitForSingleObject(hServerEvents[EVENT_GRABBER2_FW_COMPLETE],1500);			

			if(dwEvent == WAIT_OBJECT_0) 
			{
				nResult = m_pImageInfo->nGrabber_FWVer; 
				dwEvent = -1; 

				sMsg.Format("Grabber F/W:%08X", nResult);
				AddStringToStatus(sMsg);

				::ResetEvent(hServerEvents[EVENT_GRABBER2_FW_COMPLETE]);
			}
			else
			{
				nResult = 0; 
				dwEvent = -1; 
			}
		}
	}while(dwEvent != -1);

	m_nGrabber_FWVer = nResult;
	if(m_nGrabber_FWVer > 0)
	{
		sMsg.Format("%08X", m_nGrabber_FWVer);
		GetDlgItem(IDC_GRABBER_VER)->SetWindowText(sMsg); //m_szGrabberVersion = sMsg;
		//GetDlgItem(IDC_GRABBER_VER)->SetWindowText(sMsg); //sTemp
	}
	else
	{
		sMsg.Format("NOT");
		GetDlgItem(IDC_GRABBER_VER)->SetWindowText(sMsg); //m_szGrabberVersion = sMsg;
	}
	//UpdateData(FALSE);
	return nResult;
}


void CDATsysView::OnBtnGetOption() 
{
//	long nOrgId;
	CString strRtn;
	int nVal;
	CString sTmp;

//	strRtn = gGmesCtrl.MakeElem_S1F1();
//	strRtn = gGmesCtrl.MakeElem_S2F4("GLZINS002");
//	strRtn = gGmesCtrl.MakeElem_S6F6("GLZINS002", 0);
//	strRtn = gGmesCtrl.MakeElem_S6F11_1(CurrentSet->sEquipmentID, "42LN5120-TC.BTMYLJT", TRUE);
//	strRtn = gGmesCtrl.MakeElem_S6F1("GLZINS002", "TS372101FL", TRUE, 72.5);
 //   AfxMessageBox(strRtn);
//	gGmesCtrl.SendCommString(strRtn);
//AUTO
	if(g_nSysType == AUTO_SYS){
//		if (g_nSysRobotType == AUTO_SYS) {
		if (g_nUseNoScanType == 1) {
			//ctrlWipIdEdit.GetWindowText(sTmp);
			//nVal = sTmp.GetLength();
			//if (sTmp.Find('.') == -1) {
			////	AfxMessageBox("Barcode Scanning Empty!");
			//	AddStringToStatus("Barcode Scanning Empty!");

			//	//return;
			//}
			//else
			//{
			//	CurrentSet->sModelSuffixName = sTmp;
			//}
			if (ScannerCtrl.m_bPortOpen)
			{
				ScannerCtrl.PortClose();
			}
			InitScanner(CurrentSet->sScannerComPort, CurrentSet->wScannerBaudRate);
		}
		else
		{
			nVal = GetBarcodeData(sTmp);
			if (nVal == 0) {
				Sleep(500);
				nVal = GetBarcodeData(sTmp);
			}
			if (nVal == 0) {
				AfxMessageBox("Barcode Scanning Error!");
				AddStringToStatus("Barcode Scanning Error!");

				//return;
			}
			else
			{
				CurrentSet->sModelSuffixName = sTmp;
			}
		}
	}
//
	if(m_GetToolOptiondlg.DoModal() == IDOK)
	{
		ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);

		UpdateToolOptionInfo(TRUE);
	//	SaveModelIniFile(CurrentSet->sModelIni);
		if (OpenEPI_ConfigFile(CurrentSet->sEpiCfgIni))
		{
			//CurrentSet->sDipStatus
		}
		else
		{
			CurrentSet->sDipStatus = "";
		}
		m_CEdit_EPI_CFG_SW.SetWindowText(CurrentSet->sDipStatus);

	}
	else{
		CurrentSet->sModelSuffixName = "";
		ctrlSuffixEdit.SetWindowText("");
	}

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
				Sleep(500);
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

BOOL CDATsysView::OnPLCSocketReceive()
{
	CString sReceiveData;
	CString sXmlData;
	CMarkup xmlTemp;
	BOOL bRev = TRUE;
	CString tempId;
	CString sFindElem;
	CString sSetID;

	sReceiveData = _T("");
	if(gPLC_Ctrl.ReceiveCommString(500, sReceiveData))
	{		
	//xmlTemp.SetDoc(sReceiveData);
	//void CSockDlg::OnReceive()
	//{
		byte pBuf[8192];
		int iBufSize = 4096;
		int iRcvd;
		CString strRecvd;
		byte RcvData[256];

		static int s_cCheckLoadFinish = 1;
		static int s_cCheckUnLoadFinish = 1;
		static int s_cCheckPCBOnLoadSensor = 1;
		static int s_cCheck_PLC_Data_PCB_Inside = 1;

		
		// Truncate the end of the message
			
		//strRecvd += pBuf;
		//////////////////////////////////////////////////////////////			

		iRcvd = gPLC_Ctrl.m_nReadSize;//  sReceiveData.GetLength();
		
		//sprintf(pBuf,"%s", sReceiveData.GetBuffer());	pBuf[iRcvd] = NULL;
		memcpy(pBuf, gPLC_Ctrl.m_sReadBuffer, iRcvd);
		if (iRcvd >= 11)
		{
			if ((pBuf[0] == 0xD0) && (pBuf[1] == 0x00))
			{
				//gPLC_Ctrl.m_bPLC_WaitCount = 0;
				gPLC_Ctrl.m_bPLC_Online = 1;
			}
			else
			{
				strRecvd += "PLC Head Error";
				//strRecvd += (char *)pBuf;
				AddStringToStatus(strRecvd); //m_ctlRecvd.AddString(strRecvd);
				return FALSE;
			}

			int ReceiveSize = pBuf[8];// + pBuf[7];
			ReceiveSize = ReceiveSize << 8;
			ReceiveSize += pBuf[7] + 9;
			if (iRcvd >= ReceiveSize)
			{

				int lDataLen;

				if ((pBuf[9] == 0x00) && (pBuf[10] == 0x00))//End Code
				{
						
				}
				else
				{
					strRecvd.Format(_T("Error Code: 0x%02X%02X"), pBuf[10], pBuf[9]);
					//	strRecvd += (char *)pBuf;
					AddStringToStatus(strRecvd); //m_ctlRecvd.AddString(strRecvd);
					return FALSE;
				}
				lDataLen = ReceiveSize - 11;
				lDataLen = lDataLen / 2;
				if (lDataLen >= 100)
				{
					gPLC_Ctrl.m_bPLC_WaitCount = 0;

					//IDC_STATIC_MODEL_NAME
					char lModelBuf[64];
					lModelBuf[40] = 0;
					for (int i = 0; i < 20; i++)
					{
						lModelBuf[i * 2] = pBuf[11 + (i * 2)];
						lModelBuf[i * 2 + 1] = pBuf[11 + (i * 2) + 1];
					}
					gPLC_Ctrl.m_sModelName = lModelBuf;
					//SetDlgItemText(IDC_STATIC_MODEL_NAME, lModelBuf);

					for (int i = 0; i < lDataLen; i++)
					{
						RcvData[i] = pBuf[11 + (i * 2)];
					}



					if (RcvData[_PLC_ADDRESS_LOAD_COMMAND])
					{
						gPLC_Ctrl.m_cCheckLoad = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckLoad = 0;
					}

					if (RcvData[_PLC_ADDRESS_UNLOAD_COMMAND])
					{
						gPLC_Ctrl.m_cCheckUnLoad = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckUnLoad = 0;
					}
					//int m_cCheckLoadFast;
					//int m_cCheckUnLoadFast;
					if (RcvData[_PLC_ADDRESS_FAST_COMMAND])
					{
						gPLC_Ctrl.m_cCheckSkipFFC = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckSkipFFC = 0;
					}

					if (RcvData[_PLC_ADDRESS_PCB_ONLOAD_SENSOR])
					{
						gPLC_Ctrl.m_cCheckPCBOnLoadSensor = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckPCBOnLoadSensor = 0;
					}
					if (RcvData[_PLC_ADDRESS_TEST_PASS])
					{
						gPLC_Ctrl.m_cCheckTestPass = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckTestPass = 0;
					}

					if (RcvData[_PLC_ADDRESS_TEST_NG])
					{
						gPLC_Ctrl.m_cCheckTestNg = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckTestNg = 0;
					}

					if (RcvData[_PLC_ADDRESS_FFC_RETRY_COMMAND])
					{
						gPLC_Ctrl.m_cCheckReconnect = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckReconnect = 0;
					}

					//if(RcvData[_PLC_ADDRESS_MODEL_NAME])
					//{
					//	m_cCheckLoad.SetCheck(1);
					//}
					//else
					//{
					//	m_cCheckLoad.SetCheck(0);
					//}

					if (RcvData[_PLC_ADDRESS_LOAD_FINISH])
					{

						gPLC_Ctrl.m_bPCB_Onload_JIG = 1;
						gPLC_Ctrl.m_cCheckLoadFinish = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckLoadFinish = 0;
					}

					if (RcvData[_PLC_ADDRESS_UNLOAD_FINISH])
					{

						gPLC_Ctrl.m_bPCB_Onload_JIG = 0;
						gPLC_Ctrl.m_cCheckUnLoadFinish = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckUnLoadFinish = 0;
					}

				



					if (RcvData[_PLC_ADDRESS_FFC_RETRY_COMMAND])
					{
						gPLC_Ctrl.m_cCheckReconnect = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckReconnect = 0;
					}

					if (RcvData[_PLC_ADDRESS_FFC_RETRY_FINISH])
					{
						gPLC_Ctrl.m_cCheckReconnectFinish = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckReconnectFinish = 0;
					}

			
					if (RcvData[_PLC_ADDRESS_NG_3_TIMES_ERROR])
					{
						gPLC_Ctrl.m_cCheckSetAlarm = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckSetAlarm = 0;
					}


					byte  lCtrlStatus = RcvData[_PLC_ADDRESS_PLC_STATUS];
					if (lCtrlStatus & 0x01)
					{
						gPLC_Ctrl.m_cCheck_PLC_Auto_Mode = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Auto_Mode = 0;
					}

					if (lCtrlStatus & 0x02)
					{
						gPLC_Ctrl.m_cCheck_PLC_Manual_Mode = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Manual_Mode = 0;
					}

					if (lCtrlStatus & 0x04)
					{
						gPLC_Ctrl.m_cCheck_PLC_Start_Run = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Start_Run = 0;
					}

					if (lCtrlStatus & 0x08)
					{
						gPLC_Ctrl.m_cCheck_PLC_Pause = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Pause = 0;
					}

					if (lCtrlStatus & 0x10)
					{
						gPLC_Ctrl.m_cCheck_PLC_STOP = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_STOP = 0;
					}

					if (lCtrlStatus & 0x20)
					{
						gPLC_Ctrl.m_cCheck_PLC_Warn = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Warn = 0;
					}


					if (lCtrlStatus & 0x20)
					{
						gPLC_Ctrl.m_cCheck_PLC_Alarm = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Alarm = 0;
					}

					if (RcvData[_PLC_ADDRESS_NG_SET_RETRY])
					{
						gPLC_Ctrl.m_cCheckUnLoadConfirmSet = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckUnLoadConfirmSet = 0;
					}
					
					
					if (RcvData[_PLC_ADDRESS_PLC_PCB_NG_RETRY_STATUS])
					{
						gPLC_Ctrl.m_cCheck_LoadFinish_NG_ConfirmSet = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_LoadFinish_NG_ConfirmSet = 0;
					}

					if (RcvData[_PLC_ADDRESS_ROBOT_MOVING_STATUS])//m_cCheckRobotMoving;//_PLC_ADDRESS_ROBOT_MOVING_STATUS
					{
						gPLC_Ctrl.m_cCheckRobotMoving = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckRobotMoving = 0;
					}

					if (RcvData[_PLC_ADDRESS_JIG_UP_STATUS])//m_cCheckRobotMoving;//_PLC_ADDRESS_ROBOT_MOVING_STATUS
					{
						gPLC_Ctrl.m_cCheckJigUpStatus = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckJigUpStatus = 0;
					}
						
					if (RcvData[_PLC_ADDRESS_JIG_DOWN_STATUS])//m_cCheckRobotMoving;//_PLC_ADDRESS_ROBOT_MOVING_STATUS
					{
						gPLC_Ctrl.m_cCheckJigDownStatus = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheckJigDownStatus = 0;
					}
						
					if (RcvData[_PLC_ADDRESS_NG_3_TIMES_ERROR])//m_cCheckRobotMoving;//_PLC_ADDRESS_ROBOT_MOVING_STATUS
					{
						gPLC_Ctrl.m_cCheck3TimesErrorStatus = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck3TimesErrorStatus = 0;
					}

				}
				else if (lDataLen == 20)
				{
					gPLC_Ctrl.m_bPLC_RegWaitCount = 0;
					gPLC_Ctrl.m_bPLC_Online = 1;
					//IDC_STATIC_MODEL_NAME
					char lModelBuf[64];
					lModelBuf[20] = 0;
					for (int i = 0; i < 10; i++)
					{
						lModelBuf[i * 2] = pBuf[11 + (i * 2)];
						lModelBuf[i * 2 + 1] = pBuf[11 + (i * 2) + 1];
					}
					//gPLC_Ctrl.m_sModelName = lModelBuf;
					//SetDlgItemText(IDC_STATIC_MODEL_NAME, lModelBuf);

					for (int i = 0; i < lDataLen; i++)
					{
						RcvData[i] = pBuf[11 + (i * 2)];
					}

//#define _PLC_ADDRESS_PLC_PCB_INSIDE_STATUS		10 
//#define _PLC_ADDRESS_PLC_PCB_DUMMY_STATUS		11 
//#define _PLC_ADDRESS_PLC_PCB_OK_STATUS			12
//#define _PLC_ADDRESS_PLC_PCB_NG_STATUS			13 

					if (RcvData[_PLC_ADDRESS_PLC_PCB_INSIDE_STATUS])
					{
						if (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside != 1)
						{
							gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside = 1;
							
							//if (gPLC_Ctrl.m_bPCB_Onload_JIG == 0)
							//{
							//	ctrlROBOT_Status.SetWindowText(_T("-R"));//IDC_ROBOT_STATUS
							//	ctrlROBOT_Status.SetBkColor(RGB(200, 64, 64));
							//}
							//else
							//{
							//	ctrlROBOT_Status.SetWindowText(_T("MR"));
							//	ctrlROBOT_Status.SetBkColor(RGB(64, 200, 64));
							//}
							//ctrlROBOT_Status.SetTextColor(RGB(0, 0, 0));
						}
					}
					else
					{
						//gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside = 0;
						if (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside != 0)
						{
							gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside = 0;
							
							//if (gPLC_Ctrl.m_bPCB_Onload_JIG == 0)
							//{
							//	ctrlROBOT_Status.SetWindowText(_T("--"));//IDC_ROBOT_STATUS
							//	ctrlROBOT_Status.SetBkColor(RGB(64, 200, 64));
							//}
							//else
							//{
							//	ctrlROBOT_Status.SetWindowText(_T("M-"));
							//	ctrlROBOT_Status.SetBkColor(RGB(200, 64, 64));
							//}
							//ctrlROBOT_Status.SetTextColor(RGB(0, 0, 0));
						}
					}


					//CColorStaticST  ctrlPLC_COM_Status;//IDC_PLC_COM_STATUS
							//ctrlROBOT_Status.SetWindowText(_T("LOAD"));//IDC_ROBOT_STATUS
							//ctrlROBOT_Status.SetBkColor(RGB(255, 255, 0));
							//ctrlROBOT_Status.SetTextColor(RGB(0, 0, 0));
					if (RcvData[_PLC_ADDRESS_PLC_PCB_DUMMY_STATUS])
					{
						gPLC_Ctrl.m_cCheck_PLC_Data_DummyCut = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Data_DummyCut = 0;
					}

					if (RcvData[_PLC_ADDRESS_PLC_PCB_OK_STATUS])
					{
						gPLC_Ctrl.m_cCheck_PLC_Data_OK = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Data_OK = 0;
					}

					if (RcvData[_PLC_ADDRESS_PLC_PCB_NG_STATUS])
					{
						gPLC_Ctrl.m_cCheck_PLC_Data_NG = 1;
					}
					else
					{
						gPLC_Ctrl.m_cCheck_PLC_Data_NG = 0;
					}
						
					
				}
				else
				{
				//	strRecvd += "PLC ACK";
					//	strRecvd += (char *)pBuf;
				//	AddStringToStatus(strRecvd); //m_ctlRecvd.AddString(strRecvd);
					return TRUE;
				}

				if ((s_cCheckLoadFinish != gPLC_Ctrl.m_cCheckLoadFinish)
					|| (s_cCheckUnLoadFinish != gPLC_Ctrl.m_cCheckUnLoadFinish)
					|| (s_cCheckPCBOnLoadSensor != gPLC_Ctrl.m_cCheckPCBOnLoadSensor)
					|| (s_cCheck_PLC_Data_PCB_Inside != gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside))
				{
					CString str;
					if (gPLC_Ctrl.m_bPCB_Onload_JIG == 1)//m_cCheckPCBOnLoadSensor //m_cCheck_PLC_Data_PCB_Inside
					{
						str = _T("M");
					}
					else
					{
						str = _T("-");
					}

					if (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside == 1)//m_cCheckPCBOnLoadSensor //m_cCheck_PLC_Data_PCB_Inside
					{
						str += _T("R");
					}
					else
					{
						str += _T("-");
					}
					if (CurrentSet->bCheckUsePcbSensor == 1)
					{
						if (gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 1)//m_cCheckPCBOnLoadSensor //m_cCheck_PLC_Data_PCB_Inside
						{
							str += _T("S");
						}
						else
						{
							str += _T("-");
						}
					}
					if ((gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 0) && (gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 0) && (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside == 0))
					{
						ctrlROBOT_Status.SetBkColor(RGB(64, 200, 64));
					}
					else if ((gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 1) && (gPLC_Ctrl.m_cCheckPCBOnLoadSensor == 1) && (gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside == 1))
					{
						ctrlROBOT_Status.SetBkColor(RGB(64, 200, 64));
					}
					else
					{
						ctrlROBOT_Status.SetBkColor(RGB(200, 64, 64));
					}
					ctrlROBOT_Status.SetWindowText(str);


					s_cCheckLoadFinish = gPLC_Ctrl.m_cCheckLoadFinish;
					s_cCheckUnLoadFinish = gPLC_Ctrl.m_cCheckUnLoadFinish;
					s_cCheckPCBOnLoadSensor = gPLC_Ctrl.m_cCheckPCBOnLoadSensor;
					s_cCheck_PLC_Data_PCB_Inside = gPLC_Ctrl.m_cCheck_PLC_Data_PCB_Inside;
				}

				
				//int m_cCheck_PLC_Auto_Mode;//:0
				//int m_cCheck_PLC_Manual_Mode;//:1
				//int m_cCheck_PLC_Start_Run;//:2
				//int m_cCheck_PLC_Pause;//:3
				//int m_cCheck_PLC_STOP;	//:4
				//int m_cCheck_PLC_Warn;	//:5
				//int m_cCheck_PLC_Alarm;//:6		   				 
			}
			else
			{
				strRecvd += "PLC Size Error";
				AddStringToStatus(strRecvd); //m_ctlRecvd.AddString(strRecvd);
				return FALSE;

			}

		}
		else
		{
			return FALSE;
		}
		
		
	}
	return TRUE;

}


BOOL CDATsysView::OnMonitorSocketReceive()
{
	CString sReceiveData;
	CString sXmlData;
	CMarkup xmlTemp;
	BOOL bRev = TRUE;
	CString tempId;
	CString sFindElem;
	CString sSetID;
	CString lStrTmp;
	int lpos;

	gServer_Ctrl.m_bCom_RcvCount++;
	sReceiveData = _T("");
	if(gServer_Ctrl.ReceiveCommString(500, sReceiveData))
	{		
	//xmlTemp.SetDoc(sReceiveData);
	//void CSockDlg::OnReceive()
	//{
		byte pBuf[8192];
		int iBufSize = 4096;
		int iRcvd;
		CString strRecvd;
		byte RcvData[256];

		
		// Truncate the end of the message
			
		//strRecvd += pBuf;
		//////////////////////////////////////////////////////////////			

		iRcvd = gServer_Ctrl.m_nReadSize;//  sReceiveData.GetLength();
		
		//sprintf(pBuf,"%s", sReceiveData.GetBuffer());	pBuf[iRcvd] = NULL;
		memcpy(pBuf, gServer_Ctrl.m_sReadBuffer, iRcvd);
	
		ServerMonitorLog_Save(g_pView->m_szExePath, sReceiveData, TRUE);
			
		if ((sReceiveData.Find("<EIF") >= 0) && (sReceiveData.Find("</EIF>") >= 0))
		{
			lpos = sReceiveData.Find("<EIF");
			if (lpos > 0)
			{
				sReceiveData = sReceiveData.Mid(lpos);
			}

			gServer_Ctrl.m_bServer_Ctrl_Online = 1;

			//gServer_Ctrl.AddGmesLog(m_strEthernetReceive[lEQP_Index], 1);
			//	m_GMES_NGCNT = 0;
			//	AddGmesLog(m_strEthernetReceive[lEQP_Index], 1);
			//	m_GMES_Alive_WaitCnt[lEQP_Index] = 0;
			//	//		m_cLb_ComMainGmes.put_BackColor(0x0000FF00);
			//			//l_Receivestr = m_strEthernetReceive;
			//	l_Receivestr = m_strEthernetReceive[lEQP_Index];
			//	m_strEthernetReceive[lEQP_Index] = "";
			//	SaveLogDataT lSaveReceivedData;


			if (sReceiveData.Find("\"S2F3\"") >= 0)
			{						
				//lpos = sReceiveData.Find("ID=\"");
				//631 713 040 7234
				//lStrTmp = "[RECV(SVR)] : S2F3";
					
				//AddStringToStatus(lStrTmp);
				gServer_Ctrl.LinkTestResponse();			
			}
			else if (sReceiveData.Find("\"S2F103\"") >= 0)
			{
				CString lstrEQPID;
				CString lstrCommand;
				CString lstrData;
				lpos = sReceiveData.Find("<EQPID>");	//</EQPID>	
				lStrTmp = sReceiveData.Mid(lpos + strlen("<EQPID>"));
				lpos = lStrTmp.Find("</EQPID>");	//</EQPID>	
				lStrTmp = lStrTmp.Left(lpos);
				lstrEQPID = lStrTmp;//g_strEQPID

				lpos = sReceiveData.Find("<COMMAND>");	//</EQPID>	
				lStrTmp = sReceiveData.Mid(lpos + strlen("<COMMAND>"));
				lpos = lStrTmp.Find("</COMMAND>");	//</EQPID>	
				lStrTmp = lStrTmp.Left(lpos);

				lstrCommand = lStrTmp;//g_strEQPID

				lpos = sReceiveData.Find("<COMMAND DATA>");	//</EQPID>	
				lStrTmp = sReceiveData.Mid(lpos + strlen("<COMMAND DATA>"));
				lpos = lStrTmp.Find("</COMMAND DATA>");	//</EQPID>	
				lStrTmp = lStrTmp.Left(lpos);
				lstrData = lStrTmp;

			//	gServer_Ctrl.ControlCommandReceive(CString lstrCommand, CString lstrData);
				gServer_Ctrl.ControlCommandReceive( lstrCommand, lstrData);

				

				gServer_Ctrl.ControlCommandAck();
			}


		}
				
#if 0
		//		lpos = l_Receivestr.Find("<EQPID>");	//</EQPID>	
		//		str1 = l_Receivestr.Mid(lpos + strlen("<EQPID>"));
		//		lpos = str1.Find("</EQPID>");	//</EQPID>	
		//		str1 = str1.Left(lpos);
		//		lSaveReceivedData.strEQPID = str1;//g_strEQPID

		//		lpos = l_Receivestr.Find("<WOID>");	//<WOID></WOID>
		//		str1 = l_Receivestr.Mid(lpos + strlen("<WOID>"));
		//		lpos = str1.Find("</WOID>");	//</EQPID>	
		//		str1 = str1.Left(lpos);
		//		lSaveReceivedData.strWOID = str1;

		//		str2.Format(_T("RACK %d_"), lEQP_Index + 1);
		//		str2 += str1;

		//		str1 = m_cLbEquip[lEQP_Index].get_Caption();
		//		if (str1.Compare(str2) != 0)
		//		{
		//			m_cLbEquip[lEQP_Index].put_Caption(str2);
		//		}


		//		lpos = l_Receivestr.Find("<EQPNAME>");	//</>	
		//		str1 = l_Receivestr.Mid(lpos + strlen("<EQPNAME>"));
		//		lpos = str1.Find("</EQPNAME>");	//</>	
		//		str1 = str1.Left(lpos);
		//		lSaveReceivedData.strModelName = str1;



		//		lpos = l_Receivestr.Find("<RUN_STATUS>");	//</>	
		//		str1 = l_Receivestr.Mid(lpos + strlen("<RUN_STATUS>"));
		//		lpos = str1.Find("</RUN_STATUS>");	//</>	
		//		str1 = str1.Left(lpos);
		//		lSaveReceivedData.strRunStatus = str1;


		//		lpos = l_Receivestr.Find("<COM_STATUS>");	//</>	
		//		str1 = l_Receivestr.Mid(lpos + strlen("<COM_STATUS>"));
		//		lpos = str1.Find("</COM_STATUS>");	//</>	
		//		str1 = str1.Left(lpos);
		//		lSaveReceivedData.strComStatus = str1;

		//		int	lDetectedNG = 0;

		//		for (int i = 0; i < 8; i++)
		//		{
		//			CString lstrHeadID;
		//			lstrHeadID.Format(_T("<SUB_DATA_%d>"), i + 1);
		//			lpos = l_Receivestr.Find(lstrHeadID);
		//			str1 = l_Receivestr.Mid(lpos + strlen(lstrHeadID));

		//			lstrHeadID.Format(_T("</SUB_DATA_%d>"), i + 1);
		//			lpos = str1.Find(lstrHeadID);
		//			str1 = str1.Left(lpos);
		//			//ParseSubDataSet(str1,lIndex);


		//		//	SaveLogDataT lSaveReceivedData;
		//			CString lstrData;
		//			CString str2;

		//			lstrData = str1;

		//			lpos = lstrData.Find("<SUB_NO>");
		//			str2 = lstrData.Mid(lpos + strlen("<SUB_NO>"));
		//			lpos = str2.Find("</SUB_NO>");
		//			str2 = str2.Left(lpos);

		//			int lSubNo = _ttoi(str2);
		//			if ((lSubNo >= 1) && (lSubNo <= 8))
		//			{
		//				lSubNo -= 1;
		//			}
		//			else
		//			{
		//				lSubNo = 0;
		//			}


		//			lpos = lstrData.Find("<SUB_MODELNAME>");
		//			str2 = lstrData.Mid(lpos + strlen("<SUB_MODELNAME>"));
		//			lpos = str2.Find("</SUB_MODELNAME>");
		//			str2 = str2.Left(lpos);
		//			lSaveReceivedData.TestSetData[lSubNo].strModelName = str2;


		//			lpos = lstrData.Find("<SUB_SN_ID>");
		//			str2 = lstrData.Mid(lpos + strlen("<SUB_SN_ID>"));
		//			lpos = str2.Find("</SUB_SN_ID>");
		//			str2 = str2.Left(lpos);
		//			lSaveReceivedData.TestSetData[lSubNo].strWipID_SetSN = str2;

		//			lpos = lstrData.Find("<SUB_PROCESS>");
		//			str2 = lstrData.Mid(lpos + strlen("<SUB_PROCESS>"));
		//			lpos = str2.Find("</SUB_PROCESS>");
		//			str2 = str2.Left(lpos);
		//			lSaveReceivedData.TestSetData[lSubNo].strProcess = str2;


		//			lpos = lstrData.Find("<SUB_CHECKTIME>");
		//			str2 = lstrData.Mid(lpos + strlen("<SUB_CHECKTIME>"));
		//			lpos = str2.Find("</SUB_CHECKTIME>");
		//			str2 = str2.Left(lpos);
		//			lSaveReceivedData.TestSetData[lSubNo].CheckTime = str2;


		//			lpos = lstrData.Find("<SUB_RESULT>");
		//			str2 = lstrData.Mid(lpos + strlen("<SUB_RESULT>"));
		//			lpos = str2.Find("</SUB_RESULT>");
		//			str2 = str2.Left(lpos);
		//			lSaveReceivedData.TestSetData[lSubNo].strTestResult = str2;
		//			if (lSaveReceivedData.TestSetData[lSubNo].strTestResult.Find("NG") >= 0)
		//			{

		//				lDetectedNG = 1;


		//				if (gSaveInfo[lEQP_Index].TestSetData[lSubNo].strTestResult.Find("NG") < 0)
		//				{

		//					m_AlarmNumberOn[lEQP_Index] = 1;
		//					CString m_NgWaveFile = _T(PATH_BASE_DIR);
		//					m_NgWaveFile += _T("\\Sound\\OK_CHECK.wav");//NG_CHECK.wav");//OK_CHECK.wav

		//					sndPlaySound(m_NgWaveFile, SND_ASYNC);
		//				}
		//			}


		//			lpos = lstrData.Find("<SUB_DISCRIPTION>");
		//			str2 = lstrData.Mid(lpos + strlen("<SUB_DISCRIPTION>"));
		//			lpos = str2.Find("</SUB_DISCRIPTION>");
		//			str2 = str2.Left(lpos);
		//			lSaveReceivedData.TestSetData[lSubNo].strTestDiscription = str2;

		//		}


		//		if (lDetectedNG == 1)
		//		{
		//			lSaveReceivedData.strSystemStatus = 0;
		//		}
		//		else
		//		{
		//			lSaveReceivedData.strSystemStatus = 1;
		//		}
		//		/*

		//	<SUB_DATA_1>");
		//	<SUB_NO>1</SUB_NO>
		//	<SUB_MODELNAME>strModelName</SUB_MODELNAME>"));
		//	<SUB_SN_ID>strWipID_SetSN</SUB_SN_ID>
		//	<SUB_PROCESS>strProcess</SUB_PROCESS>
		//	<SUB_CHECKTIME>CheckTime</SUB_CHECKTIME>
		//	<SUB_RESULT>strTestResult</SUB_RESULT>
		//	<SUB_DISCRIPTION></SUB_DISCRIPTION>
		//	</SUB_DATA_1>"));
		//	YB
		//	#Select//YB//SET1# 	2
		//	#BarCode//YB//TEXT#
		//	#Cnt_FM//YB//TEXT#
		//	#Cnt_Time//YB//TEXT#
		//	#SetStatus//YB//TEXT#
		//	#AC_Power//YB//TEXT#
		//	#Lan_Status//YB//TEXT#
		//	#USB1_Status//YB//TEXT#
		//	#USB2_Status//YB//TEXT#
		//	#PowerStatus//YB//TEXT#
		//	#OpenStatus//YB//TEXT#
		//	#AgingTimeLabel//YB//TEXT#



		//		CLabel_text m_cLB_Select[8];
		//		CLabel_text m_cLB_BarCode[8];
		//		CLabel_text m_cLB_Cnt_FM[8];
		//		CLabel_text m_cLB_Cnt_Time[8];
		//		CLabel_text m_cLB_SetStatus[8];

		//		CLabel_text m_cLB_AC_Power[8];
		//		CLabel_text m_cLB_Lan_Status[8];
		//		CLabel_text m_cLB_USB1_Status[8];
		//		CLabel_text m_cLB_USB2_Status[8];


		//		CLabel_text m_cLB_PowerStatus[8];
		//		CLabel_text m_cLB_OpenStatus[8];
		//		CLabel_text m_TotalAgingTimeLabel[8];



		//	*/


		//		gSaveInfo[lEQP_Index] = lSaveReceivedData;

		//		SetSubListControl(lEQP_Index, lSaveReceivedData);//SetOQAListControl();// SetSubListControl(lSaveReceivedData);

		//		CFileControl lFile;
		//		lFile.SaveLog(lSaveReceivedData);
		//		///////////////////////////////////////////		
		//		//str.Format(_T("%d"), sCount);

		//		//m_CtrlListMainProcess.InsertItem(0, str);   // 첫째행(0), 첫째열에 삽입
		//		//m_CtrlListMainProcess.SetItem(0, 1, LVIF_TEXT, _T("SECOND TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//		//m_CtrlListMainProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
		//		//if(m_CtrlListMainProcess.GetItemCount() > 20)
		//		//{
		//		//	m_CtrlListMainProcess.DeleteItem(20);
		//		//}	
		//		//

		/////////////////////////////////////////////////

		//		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S6F2\" NAME=\"Inspection(Processing) Data Ack\">\r\n");
		//		m_strMesSend += _T("    <EQPID>");
		//		m_strMesSend += lSaveReceivedData.strEQPID;
		//		m_strMesSend += _T("</EQPID>\r\n");

		//		m_strMesSend += _T("    <ACK>");
		//		m_strMesSend += _T("0");
		//		m_strMesSend += _T("</ACK>\r\n");
		//		m_strMesSend += _T("    <REASON>");
		//		m_strMesSend += _T("0");
		//		m_strMesSend += _T("</REASON>\r\n");
		//		m_strMesSend += _T("</EIF>");


		//		OnCmdGmesSend(lEQP_Index);

		//	}
		//	else if (l_Receivestr.Find("\"S1F1\"") >= 0)
		//	{

		//		lpos = l_Receivestr.Find("<EQPID>");	//</EQPID>	
		//		str1 = l_Receivestr.Mid(lpos + 7);
		//		lpos = str1.Find("</EQPID>");	//</EQPID>	
		//		str1 = str1.Left(lpos);
		//		lSaveReceivedData.strEQPID = str1;//g_strEQPID




		//		m_strMesSend = _T("<EIF VERSION=\"1.4\" ID=\"S1F2\" NAME=\"Are You There Data Send \">\r\n");
		//		m_strMesSend += _T("    <EQPID>");
		//		m_strMesSend += lSaveReceivedData.strEQPID;
		//		m_strMesSend += _T("</EQPID>\r\n");
		//		m_strMesSend += _T("</EIF>");

		//		OnCmdGmesSend(lEQP_Index);

		//	}

		//}
		////	gGmesCtrl.Paser_ReceiveData(sRevData);
		//xmlTemp.SetDoc(sReceiveData);
		//xmlTemp.ResetPos();

		//while (xmlTemp.FindElem("EIF")) {
		//	tempId = xmlTemp.GetAttrib("ID");
		//	sFindElem = xmlTemp.GetSubDoc();
		//	if (tempId == "S2F3") {
		//		if (gGmesCtrl.Paser_S2F3(sFindElem))
		//		{
		//			sXmlData = gGmesCtrl.MakeElem_S2F4(CurrentSet->sEquipmentID);
		//			if (gGmesCtrl.SendCommString(sXmlData))
		//			{
		//				gGmesCtrl.DisplyComStatus("S2F4", FALSE);
		//			}
		//		}
		//	}
		//	else if (tempId == "S1F2") {
		//		gGmesCtrl.Paser_S1F2(sFindElem);
		//	}
		//	else if (tempId == "S2F31") {
		//		if (gGmesCtrl.Paser_S2F31(sFindElem)) {
		//			sXmlData = gGmesCtrl.MakeElem_S2F32(CurrentSet->sEquipmentID, 0);
		//		}
		//		else {
		//			sXmlData = gGmesCtrl.MakeElem_S2F32(CurrentSet->sEquipmentID, 1);
		//		}

		//		if (gGmesCtrl.SendCommString(sXmlData))
		//		{
		//			gGmesCtrl.DisplyComStatus("S2F31", FALSE);
		//		}
		//	}
		//	else if (tempId == "S6F2") {
		//		bRev = gGmesCtrl.Paser_S6F2(sFindElem);

		//		m_bResult_S6F2 = bRev;
		//		UpdateData(TRUE);
		//		::SetEvent(m_hReadEvent_S6F2);
		//	}
		//	else if (tempId == "S6F12") {
		//		bRev = gGmesCtrl.Paser_S6F12(sFindElem);

		//		m_bResult_S6F12 = bRev;
		//		UpdateData(TRUE);
		//		::SetEvent(m_hReadEvent_S6F12);
		//		Sleep(500);
		//	}
		//	else if (tempId == "S6F5") {
		//		if (gGmesCtrl.Paser_S6F5(sFindElem, sSetID)) {
		//			bRev = TRUE;
		//			sXmlData = gGmesCtrl.MakeElem_S6F6(CurrentSet->sEquipmentID, sSetID, 0);

		//			Display_GMESInfo(TRUE);
		//		}
		//		else {
		//			bRev = FALSE;
		//			sXmlData = gGmesCtrl.MakeElem_S6F6(CurrentSet->sEquipmentID, sSetID, 1);
		//			Display_GMESInfo(FALSE);
		//		}

		//		m_bResult_S6F5 = bRev;
		//		UpdateData(TRUE);
		//		::SetEvent(m_hReadEvent_S6F5);

		//		if (gGmesCtrl.SendCommString(sXmlData))
		//		{
		//			gGmesCtrl.DisplyComStatus("S6F5", FALSE);
		//		}
		//	}
		//	else {
		//		bRev = FALSE;
		//	}

		//}

#endif
	}
	else 
	{
		bRev = FALSE;
	}
	
		
	
	return bRev;

}

void CDATsysView::UpdateToolOptionInfo(BOOL bSaveFlag)
{
	ctrlSuffixEdit.SetWindowText(CurrentSet->sModelSuffixName);
	UpdateInfo(TOOL_OPTION1,CurrentSet->sToolOption1);
	UpdateInfo(TOOL_OPTION2,CurrentSet->sToolOption2);
	UpdateInfo(TOOL_OPTION3,CurrentSet->sToolOption3);
	UpdateInfo(TOOL_OPTION4,CurrentSet->sToolOption4);
	UpdateInfo(TOOL_OPTION5,CurrentSet->sToolOption5);
	UpdateInfo(TOOL_OPTION6,CurrentSet->sToolOption6);
	UpdateInfo(TOOL_OPTION7,CurrentSet->sToolOption7);
	UpdateInfo(TOOL_OPTION8,CurrentSet->sToolOption8);
	UpdateInfo(COMMERCIAL_OPTION1,CurrentSet->sCommercialOption1);

	UpdateInfo(TOOL_CRC,CurrentSet->sToolCRC);

	UpdateInfo(AREA_OPTION1,CurrentSet->sAreaOption1);
	UpdateInfo(COUNTRY_GROUP,CurrentSet->sCountryGroup);
	UpdateInfo(COUNTRY,CurrentSet->sCountry);

	UpdateInfo(CPU_VERSION,CurrentSet->sCPUVersion);
	UpdateInfo(MICOM_VERSION,CurrentSet->sMicomVersion);
	UpdateInfo(USB_VERSION,CurrentSet->sUSBVersion);
	UpdateInfo(MNT_VERSION,CurrentSet->sMNTVersion);

	if(bSaveFlag){
		SaveModelIniFile(CurrentSet->sModelIni);
	}
}
void CDATsysView:: LogDataSave(CString sLogFilePath)
{
#ifdef _DEBUG
	return;
#endif
	CString szLogFile = _T("");

	szLogFile = sLogFilePath;
	szLogFile.Replace(".dat" , ".log");

	HANDLE hText = NULL;
	HANDLE hClipData;                            /* handles to clip data  */
	LPSTR  lpClipData;                           /* pointers to clip data */
	HWND hEdit = m_ctrlEditComLog.m_hWnd;

    CStdioFile File;
    CFileException ex;

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

}
BOOL CDATsysView::StepCheck()
{
	CString sTmp;
	CString sMsg;
	DWORD dwEventResult = 0;
	clock_t		start;
	MSG msg;
	//	int nVal = 0;

	sTmp = gGmesCtrl.MakeElem_S6F11(CurrentSet->sEquipmentID, m_strWipId);
	::ResetEvent(m_hReadEvent_S6F12);
	::ResetEvent(m_hReadEvent_S6F5);

	if (gGmesCtrl.SendCommString(sTmp))
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

		if (dwEventResult == WAIT_OBJECT_0)
		{
			::ResetEvent(m_hReadEvent_S6F12);
			if (!m_bResult_S6F12) {
				sMsg.Format(_T("GMES(S6F5) Event Report Send Error\nError Code : %s"), gGmesCtrl.m_sErrorMsg);
				AfxMessageBox(sMsg);
				CurrentSet->bIsRunMsg = FALSE; return FALSE;
			}
			break;
		}
		if ((clock() - start) * 1000 / CLOCKS_PER_SEC > 10000) {
			sMsg.Format(_T("GMES S6F12(Event Report Send) Time Out"));
			AfxMessageBox(sMsg);
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

		if (dwEventResult == WAIT_OBJECT_0)
		{
			::ResetEvent(m_hReadEvent_S6F5);
			if (!m_bResult_S6F5) {
				sMsg.Format(_T("GMES(S6F5)  Event Report Send Error\nParsing Error.."));
				AfxMessageBox(sMsg);
				CurrentSet->bIsRunMsg = FALSE; return FALSE;
			}
			break;
		}
		if ((clock() - start) * 1000 / CLOCKS_PER_SEC > 5000) {
			sMsg.Format(_T("GMES S6F5( Event Report Send) Time Out"));
			AfxMessageBox(sMsg);
			CurrentSet->bIsRunMsg = FALSE; return FALSE;
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

	if(CurrentSet->nToolOptionSetMethod == 0)
	{
		sTmp = gGmesCtrl.MakeElem_S6F11_1(CurrentSet->sEquipmentID, m_strWipId, FALSE);
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
					sMsg.Format(_T("GMES Tool Option D/L Error\nError Code : %s"), gGmesCtrl.m_sErrorMsg);
					AfxMessageBox(sMsg);
					CurrentSet->bIsRunMsg = FALSE; return FALSE;
				}
				break;
			}
			if ((clock() - start)*1000/CLOCKS_PER_SEC > 10000){ 
				sMsg.Format(_T("GMES S6F12(Tool Option) D/L Time Out"));
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
					sMsg.Format(_T("GMES Tool Option D/L Error\nParsing Error.."));
					AfxMessageBox(sMsg);
					CurrentSet->bIsRunMsg = FALSE; return FALSE;
				}
				break;
			}
			if ((clock() - start)*1000/CLOCKS_PER_SEC > 5000){ 
				sMsg.Format(_T("GMES S6F5(Tool Option) D/L Time Out"));
				AfxMessageBox(sMsg);
				Gmes_Connection();
				CurrentSet->bIsRunMsg = FALSE; return FALSE;
			}
		}
		
	}
	return TRUE;
}

BOOL CDATsysView::CheckNgCount()
{ 
	CString sTmp;
	CString sMsg;
	DWORD dwEventResult = 0;
    clock_t		start;
	MSG msg;

	int lFailFlag = 0;
	//	int nVal = 0;
	
	sTmp = gGmesCtrl.MakeElem_S6F11_2(CurrentSet->sEquipmentID, m_strWipId);
	::ResetEvent(m_hReadEvent_S6F12);
	::ResetEvent(m_hReadEvent_S6F5);
	
	if(gGmesCtrl.SendCommString(sTmp))
	{
		gGmesCtrl.DisplyComStatus("S6F11", FALSE);
	}
	else
	{
		gGmesCtrl.DisplyComStatus("S6F11:SEND FAIL", FALSE);
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
				sMsg.Format(_T("GMES DFT Sequence Check Error\nError Code : %s"), gGmesCtrl.m_sErrorMsg);
				//AfxMessageBox(sMsg);
				gGmesCtrl.DisplyComStatus(sMsg, TRUE);
				CurrentSet->bIsRunMsg = FALSE; return FALSE;
			}
			break;
		}
		if ((clock() - start)*1000/CLOCKS_PER_SEC > 5000){ 
			sMsg.Format(_T("GMES S6F12 DFT Sequence Check Time Out"));
			//AfxMessageBox(sMsg);
			gGmesCtrl.DisplyComStatus(sMsg, TRUE);
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
				sMsg.Format(_T("GMES DFT Sequence Check Error\nParsing Error.."));
				//AfxMessageBox(sMsg);
				gGmesCtrl.DisplyComStatus(sMsg, TRUE);
				CurrentSet->bIsRunMsg = FALSE; return FALSE;
			}
			break;
		}
		if ((clock() - start)*1000/CLOCKS_PER_SEC > 5000){ 
			sMsg.Format(_T("GMES S6F5(Sequence Check) Time Out"));
			//AfxMessageBox(sMsg);
			gGmesCtrl.DisplyComStatus(sMsg, TRUE);
			Gmes_Connection();
			CurrentSet->bIsRunMsg = FALSE; return FALSE;
		}
	}
		
	return TRUE;
}
/*
BOOL CDATsysView::CheckModelID()
{
	CString sModelName;
	CString sModelID;

	sModelName = CurrentSet->sModelName;
	sModelID = CurrentSet->sModelID;
	sModelName.MakeUpper();
	sModelID.MakeUpper();

	if(sModelID.Find(sModelName) != -1)
	{
		return TRUE;
	}else{
		if(sModelName.Find(sModelID) != -1)
		{

			return TRUE;
		}
	}
	return FALSE;
}
*/
void CDATsysView::OnButton2() 
{

#if 1
	CString sValue;

	//_Moving_Pic_Test();
	int nResult = TEST_FAIL;
	
	int nNoBlackScreen = 0;

	int nVideoMovingResult = TEST_FAIL;
	int nFreqMovingResult = TEST_FAIL;




	
	float fLeftFreqMax = 0;
	float fLeftFreqMin = 50000;
	float fRightFreqMax = 0;
	float fRightFreqMin = 50000;

	 g_pView->AudioMeasureStart();
	//_Wait(5000);

	for (int i = 0; i < 20; i++)
	{
		if (CurrentSet->nAudioCheckMethod == METHOD_AVSWITCHBOX)
		{
			if (!CurrentSet->bUseAVSwitchBox)
			{
				nFreqMovingResult = FALSE;
				break;
			}
			else
			{
				AvSwitchBoxCtrl.AudioMeasure();
				if (AvSwitchBoxCtrl.m_fMeasuredLeftFreq > 20)
				{
					fLeftFreqMax = max(fLeftFreqMax, AvSwitchBoxCtrl.m_fMeasuredLeftFreq);
					fLeftFreqMin = min(fLeftFreqMin, AvSwitchBoxCtrl.m_fMeasuredLeftFreq);
				}

				if (AvSwitchBoxCtrl.m_fMeasuredRightFreq > 20)
				{
					fRightFreqMax = max(fRightFreqMax, AvSwitchBoxCtrl.m_fMeasuredRightFreq);
					fRightFreqMin = min(fRightFreqMin, AvSwitchBoxCtrl.m_fMeasuredRightFreq);

				}

				if ((fLeftFreqMin < AvSwitchBoxCtrl.m_fMeasuredLeftFreq) && (AvSwitchBoxCtrl.m_fMeasuredLeftFreq < fLeftFreqMax)
					&& (fRightFreqMin < AvSwitchBoxCtrl.m_fMeasuredRightFreq) && (AvSwitchBoxCtrl.m_fMeasuredRightFreq < fRightFreqMax))
				{
					nFreqMovingResult = TRUE;
					break;
				}
			}
		}
		else
		{
			g_SoundCard.m_fMeasuredLeftFreq = 0;
			g_SoundCard.m_fMeasuredRightFreq = 0;
			g_SoundCard.AudioMeasure();
			
			if (g_SoundCard.m_fMeasuredLeftFreq > 20)
			{
				fLeftFreqMax = max(fLeftFreqMax, g_SoundCard.m_fMeasuredLeftFreq);
				fLeftFreqMin = min(fLeftFreqMin, g_SoundCard.m_fMeasuredLeftFreq);
			}

			if (g_SoundCard.m_fMeasuredRightFreq > 20)
			{
				fRightFreqMax = max(fRightFreqMax, g_SoundCard.m_fMeasuredRightFreq);
				fRightFreqMin = min(fRightFreqMin, g_SoundCard.m_fMeasuredRightFreq);
			}
			
		

			if ((fLeftFreqMin < g_SoundCard.m_fMeasuredLeftFreq) && (g_SoundCard.m_fMeasuredLeftFreq < fLeftFreqMax)
				&& (fRightFreqMin < g_SoundCard.m_fMeasuredRightFreq) && (g_SoundCard.m_fMeasuredRightFreq < fRightFreqMax))
			{
				nFreqMovingResult = TRUE;
				break;
			}
		}
		_Wait(1000);
	}




	return;
#endif

	//sValue = "Tool Option(1:11400,2:1286,3:49224,4:,5:,6:,7:,8:)/ Country Group:US/ Country:US/ Area Code:";
//	sValue = "Tool Option(1:8,2:2406,3:9989,4:13136,5:56322,6:272,7:,8:)/ Country Group:/ Country:JA/ Area Code:1793";

//	sValue.MakeUpper();
//	Get_ToolOption(sValue); 
//	UpdateToolOptionInfo();
//	m_ManualTestDlg.DoModal();

//	if(m_GetToolOptiondlg.m_bActivate){
//		CurrentSet->sModelSuffixName = "42LN5120-TC.BTMYLJT";
//		::SendMessage(m_GetToolOptiondlg, UM_UPDATE_TOOL_OPTION,0,0);
//	}
	
/*
	m_szResultMsg1.Format("PCBA ID : 42LN5120-TC");
	m_szResultMsg2.Format("Elased Time : 86(s) : 12");
	
	PostMessage(UM_RESULT_DISPLAY,TRUE,0);
//	PostMessage(UM_RESULT_DISPLAY,FALSE,0);
*/
/*	
	CString szMsg1 = "Step(10d) Comp1 Check";
	CString szMsg2 = _T("Audio NG");
	CString szMsg3 = "T(1000,400) M(400,400)";
	m_StopStepDlg.SetTestResultInfo(szMsg1,szMsg2,szMsg3);

	m_StopStepDlg.DoModal();
*/
/*	int nLeft   = 0;
	int nTop    = 0;
	int nWidth  = 0;
	int nHeight = 0;

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
*/
//	CString szMsg1 = "Step(10d) Comp1 Check";
//	CString szMsg2 = _T("Audio NG");
//	m_UserMsgDlg.SetMessage(szMsg1,szMsg2);
//	m_UserMsgDlg.DoModal();

//	Check_KeyDownload();
//	m_strWipId = "GN452108ZS";

//	CheckNgCount();
	
	//=================
	// Modeless Dialog
	//=================
	//BOOL bReturn;

	//m_pVfMeasureCheckDlg.m_bTestClose = FALSE;

	//m_pVfMeasureCheckDlg.DoModal();
	//bReturn = m_pVfMeasureCheckDlg.m_bVfCheckResult;
	//TVCommCtrl.Send_TVControl_Command("av 00 25", 1000);
	//if (Check_IR_Blaster())
	//{
	//	AddStringToStatus(_T("IR CHECK OK"));
	//}
	//else
	//{
	//	AddStringToStatus(_T("IR CHECK NG"));
	//}
	//TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);

}
#if 1

//CString sTmp;
//
//m_CtrlListVersionProcess.DeleteAllItems();
//
//char *sHeader[] = { "BE Ver.", "FE Ver.", "Micom Ver.", "DSP Ver.", "Sub DSP Ver.", "EQ Ver.", "MEQ Ver.", "Touch Ver.", "Demo Ver.", "DJ Sound Ver.", "WirelessTx Ver.", "WirelessRx Ver.","BT Version", "HDMI Version", "Checksum" };
//
//
//// Set Column Alignment & Column Title
//m_CtrlListVersionProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
//m_CtrlListVersionProcess.InsertColumn(0, _T("No"), LVCFMT_CENTER, 30);
//m_CtrlListVersionProcess.InsertColumn(1, _T("ITEM"), LVCFMT_CENTER, 117);
//m_CtrlListVersionProcess.InsertColumn(2, _T("VERSION"), LVCFMT_CENTER, 110);
//
//int lCount = 0;
//for (int nRow = 0; nRow < 15; nRow++)
//{
//	if (CurrentSet->bVerChecked[nRow] == 1)
//	{
//
//		sTmp.Format(_T("%2d"), nRow + 1);
//		m_CtrlListVersionProcess.InsertItem(lCount, sTmp);   // 첫째행(0), 첫째열에 삽입	
//		m_CtrlListVersionProcess.SetItem(lCount, 1, LVIF_TEXT, sHeader[nRow], NULL, NULL, NULL, NULL);
//		lCount++;
//	}
//
//}
void CDATsysView::InitOptionGrid()
{
	CString sTmp;

	char *sHeader[] = {"1", "2","3", "4", "5", "6", "7", "8(C)", "9", "CRC"};

	///m_ctrlOptionGrid.SetRedraw(FALSE);
	//m_ctrlOptionGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything
	m_ctrlListOption.DeleteAllItems();
	m_ctrlListOption.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가


//
//	m_ctrlOptionGrid.SetBackColorFixed(RGB(210,216,176));
//	m_ctrlOptionGrid.SetForeColorFixed(RGB(51,102,153));
//	m_ctrlOptionGrid.SetBackColor(RGB(247,247,231));
//	m_ctrlOptionGrid.SetGridColor(RGB(200,200,176));
//
//	m_ctrlOptionGrid.SetBackColorSel(RGB(247,247,231));
//
//	m_ctrlOptionGrid.SetFixedRows(1);
//	m_ctrlOptionGrid.SetFixedCols(1);
////	m_ctrlOptionGrid.SetFixedRows(2);
////	m_ctrlOptionGrid.SetFixedCols(0);
//
//    m_ctrlOptionGrid.SetMergeCells(5 /*flexMergeFixedOnly*/);
//	m_ctrlOptionGrid.SetMergeRow(0, true);
//
//	m_ctrlOptionGrid.SetFillStyle(flexFillRepeat);
//	m_ctrlOptionGrid.SetFontName("Arial");
//2IN1
#ifdef _DFT_2IN1
	//m_ctrlOptionGrid.SetFontSize(10);
//	
#endif
#ifdef _DFT_3IN1
	//m_ctrlOptionGrid.SetFontSize(8);
#endif

//	m_ctrlOptionGrid.SetFontBold(TRUE);
//
//	m_ctrlOptionGrid.SetCols(2);
//	m_ctrlOptionGrid.SetRows(11);
//	m_ctrlOptionGrid.SetExtendLastCol(TRUE);
//
//	m_ctrlOptionGrid.SetRow(0);
////	m_ctrlOptionGrid.SetCol(0);
////	m_ctrlOptionGrid.SetText("OPTION");
//	m_ctrlOptionGrid.SetCol(1);
//	m_ctrlOptionGrid.SetText("OPTION");
//2IN1
#ifdef _DFT_2IN1
	//m_ctrlOptionGrid.SetColWidth(0, 600); //300
	//m_ctrlOptionGrid.SetColWidth(1, 1300); //300
	m_ctrlListOption.InsertColumn(0, _T(" "), LVCFMT_CENTER, 30);
	m_ctrlListOption.InsertColumn(1, _T("OPTION"), LVCFMT_CENTER, 125);
//
#endif
#ifdef _DFT_3IN1
	//m_ctrlOptionGrid.SetColWidth(0, 500); //300
	//m_ctrlOptionGrid.SetColWidth(1, 1200); //300
	m_ctrlListOption.InsertColumn(0, _T(" "), LVCFMT_CENTER, 20);
	m_ctrlListOption.InsertColumn(1, _T("OPTION"), LVCFMT_CENTER, 88);
#endif

//	m_ctrlOptionGrid.SetRowHeight(2, 400);
	// Set Column Alignment & Column Title
//	for(int nCol = 0; nCol < 9 ; nCol++)
	for(int nRow = 0; nRow < 10 ; nRow++)
	{
		//m_ctrlOptionGrid.SetCol(0);

		//m_ctrlOptionGrid.SetRow(nRow + 1);
//2IN1		
#ifdef _DFT_2IN1
		//m_ctrlOptionGrid.SetRowHeight(nRow + 1, 305);
#endif
//
		//m_ctrlOptionGrid.SetText(sHeader[nRow]);
	
		//m_ctrlOptionGrid.SetColAlignment(0,flexAlignCenterCenter);

		//m_ctrlOptionGrid.SetFixedAlignment(0,flexAlignCenterCenter);
		
		
		
		m_ctrlListOption.InsertItem(nRow, sHeader[nRow]);   // 첫째행(0), 첫째열에 삽입	
		//m_CtrlListVersionProcess.SetItem(lCount, 1, LVIF_TEXT, sHeader[nRow], NULL, NULL, NULL, NULL);
	
			
	}

	//m_ctrlOptionGrid.SetFixedAlignment(0,flexAlignCenterCenter);
	//m_ctrlOptionGrid.SetFixedAlignment(1,flexAlignCenterCenter);

	//m_ctrlOptionGrid.SetCol(-1);
	//m_ctrlOptionGrid.SetRow(-1);

	//m_ctrlOptionGrid.SetRedraw(TRUE);
	//m_ctrlOptionGrid.Refresh();
}
void CDATsysView::InitCountryGrid()
{
//	CString sTmp;
//
//	char *sHeader[] = {"AREA CODE", "COUNTRY_GR","COUNTRY"};
//
//	m_ctrlListCountry.DeleteAllItems();
//	m_ctrlListCountry.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
//	
////2IN1
//#ifdef _DFT_2IN1
//	//m_ctrlOptionGrid.SetFontSize(10);
////	
//#endif
//#ifdef _DFT_3IN1
//	//m_ctrlOptionGrid.SetFontSize(8);
//#endif
//
////2IN1
//#ifdef _DFT_2IN1
//	//m_ctrlOptionGrid.SetColWidth(0, 600); //300
//	//m_ctrlOptionGrid.SetColWidth(1, 1300); //300
//	m_ctrlListCountry.InsertColumn(0, _T("NAME"), LVCFMT_CENTER, 30);
//	m_ctrlListCountry.InsertColumn(1, _T("DATA"), LVCFMT_CENTER, 117);
//	
//	//
//#endif
//#ifdef _DFT_3IN1
//	//m_ctrlOptionGrid.SetColWidth(0, 500); //300
//	//m_ctrlOptionGrid.SetColWidth(1, 1200); //300
//	m_ctrlListOption.InsertColumn(0, _T(" "), LVCFMT_CENTER, 30);
//	m_ctrlListOption.InsertColumn(1, _T("OPTION"), LVCFMT_CENTER, 117);
//#endif
//
//	//	m_ctrlOptionGrid.SetRowHeight(2, 400);
//		// Set Column Alignment & Column Title
//	//	for(int nCol = 0; nCol < 9 ; nCol++)
//	for (int nRow = 0; nRow < 6; nRow++)
//	{
//		
////2IN1		
//#ifdef _DFT_2IN1
//		//m_ctrlOptionGrid.SetRowHeight(nRow + 1, 305);
//#endif
//
//
//
//		m_ctrlListCountry.InsertItem(nRow, sHeader[nRow]);   // 첫째행(0), 첫째열에 삽입	
//		//m_CtrlListVersionProcess.SetItem(lCount, 1, LVIF_TEXT, sHeader[nRow], NULL, NULL, NULL, NULL);
//
//
//	}
}
void CDATsysView::InitVersionGrid()
{
	CString sTmp;

	char *sHeader[] = {"S/W Ver.", "MICOM Ver.","MNT Ver.","AREA CODE", "CNTRY_GR","CNTRY"};

	

	//m_ctrlVersionGrid.SetRedraw(FALSE);
	//m_ctrlVersionGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	/*m_ctrlVersionGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlVersionGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlVersionGrid.SetBackColor(RGB(247,247,231));
	m_ctrlVersionGrid.SetGridColor(RGB(200,200,176));

	m_ctrlVersionGrid.SetBackColorSel(RGB(247,247,231));

	m_ctrlVersionGrid.SetFixedRows(0);
	m_ctrlVersionGrid.SetFixedCols(1);

	m_ctrlVersionGrid.SetFillStyle(flexFillRepeat);
	m_ctrlVersionGrid.SetFontName("Arial");*/

	m_ctrlListVersion.DeleteAllItems();
	m_ctrlListVersion.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
//2In1
#ifdef _DFT_2IN1
	//m_ctrlListVersion.SetFont();//m_ctrlVersionGrid.SetFontSize(9);
//
#endif
#ifdef _DFT_3IN1
	//m_ctrlVersionGrid.SetFontSize(8);
#endif

	//m_ctrlVersionGrid.SetFontBold(TRUE);

	//m_ctrlVersionGrid.SetCols(2);
	//m_ctrlVersionGrid.SetRows(3);
	//m_ctrlVersionGrid.SetExtendLastCol(TRUE);

	//m_ctrlVersionGrid.SetColAlignment(0,flexAlignCenterCenter);


//2In1
#ifdef _DFT_2IN1
		m_ctrlListVersion.InsertColumn(0, _T("NAME"), LVCFMT_CENTER, 80);
	m_ctrlListVersion.InsertColumn(1, _T("DATA"), LVCFMT_CENTER, 97);
	
	//m_ctrlVersionGrid.SetColWidth(0, 1300);
//
#endif
#ifdef _DFT_3IN1
//	m_ctrlVersionGrid.SetColWidth(0, 1200);
	m_ctrlListVersion.InsertColumn(0, _T("NAME"), LVCFMT_CENTER, 55);
	m_ctrlListVersion.InsertColumn(1, _T("DATA"), LVCFMT_CENTER, 85);
#endif
	//m_ctrlVersionGrid.SetFixedAlignment(0,flexAlignCenterCenter);

//	m_ctrlVersionGrid.SetRowHeight(1, 400);
	// Set Column Alignment & Column Title
	for(int nRow = 0; nRow < 6 ; nRow++)
	{
//		m_ctrlVersionGrid.SetCol(0);
//		m_ctrlVersionGrid.SetRow(nRow);
//2In1
#ifdef _DFT_2IN1
		//m_ctrlVersionGrid.SetRowHeight(nRow, 290);
#endif
//		
		m_ctrlListVersion.InsertItem(nRow, sHeader[nRow]); //m_ctrlVersionGrid.SetText(sHeader[nRow]);
	
	}
	//m_ctrlVersionGrid.SetCol(-1);
	//m_ctrlVersionGrid.SetRow(-1);

	//m_ctrlVersionGrid.SetRedraw(TRUE);
	//m_ctrlVersionGrid.Refresh();
}
#else
void CDATsysView::InitOptionGrid()
{
	CString sTmp;

	char *sHeader[] = { "1", "2","3", "4", "5", "6", "7", "8(C)", "9", "CRC" };

	m_ctrlOptionGrid.SetRedraw(FALSE);
	m_ctrlOptionGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlOptionGrid.SetBackColorFixed(RGB(210, 216, 176));
	m_ctrlOptionGrid.SetForeColorFixed(RGB(51, 102, 153));
	m_ctrlOptionGrid.SetBackColor(RGB(247, 247, 231));
	m_ctrlOptionGrid.SetGridColor(RGB(200, 200, 176));

	m_ctrlOptionGrid.SetBackColorSel(RGB(247, 247, 231));

	m_ctrlOptionGrid.SetFixedRows(1);
	m_ctrlOptionGrid.SetFixedCols(1);
	//	m_ctrlOptionGrid.SetFixedRows(2);
	//	m_ctrlOptionGrid.SetFixedCols(0);

	m_ctrlOptionGrid.SetMergeCells(5 /*flexMergeFixedOnly*/);
	m_ctrlOptionGrid.SetMergeRow(0, true);

	m_ctrlOptionGrid.SetFillStyle(flexFillRepeat);
	m_ctrlOptionGrid.SetFontName("Arial");
	//2IN1
#ifdef _DFT_2IN1
	m_ctrlOptionGrid.SetFontSize(10);
	//	
#endif
#ifdef _DFT_3IN1
	m_ctrlOptionGrid.SetFontSize(8);
#endif

	m_ctrlOptionGrid.SetFontBold(TRUE);

	m_ctrlOptionGrid.SetCols(2);
	m_ctrlOptionGrid.SetRows(11);
	m_ctrlOptionGrid.SetExtendLastCol(TRUE);

	m_ctrlOptionGrid.SetRow(0);
	//	m_ctrlOptionGrid.SetCol(0);
	//	m_ctrlOptionGrid.SetText("OPTION");
	m_ctrlOptionGrid.SetCol(1);
	m_ctrlOptionGrid.SetText("OPTION");
	//2IN1
#ifdef _DFT_2IN1
	m_ctrlOptionGrid.SetColWidth(0, 600); //300
	m_ctrlOptionGrid.SetColWidth(1, 1300); //300
//
#endif
#ifdef _DFT_3IN1
	m_ctrlOptionGrid.SetColWidth(0, 500); //300
	m_ctrlOptionGrid.SetColWidth(1, 1200); //300
#endif

//	m_ctrlOptionGrid.SetRowHeight(2, 400);
	// Set Column Alignment & Column Title
//	for(int nCol = 0; nCol < 9 ; nCol++)
	for (int nRow = 0; nRow < 10; nRow++)
	{
		m_ctrlOptionGrid.SetCol(0);

		m_ctrlOptionGrid.SetRow(nRow + 1);
		//2IN1		
#ifdef _DFT_2IN1
		m_ctrlOptionGrid.SetRowHeight(nRow + 1, 305);
#endif
		//
		m_ctrlOptionGrid.SetText(sHeader[nRow]);

		m_ctrlOptionGrid.SetColAlignment(0, flexAlignCenterCenter);

		m_ctrlOptionGrid.SetFixedAlignment(0, flexAlignCenterCenter);
	}

	m_ctrlOptionGrid.SetFixedAlignment(0, flexAlignCenterCenter);
	m_ctrlOptionGrid.SetFixedAlignment(1, flexAlignCenterCenter);
	//	m_ctrlOptionGrid.SetColAlignment(0,flexAlignCenterCenter);	
	//	m_ctrlOptionGrid.SetColAlignment(1,flexAlignCenterCenter);	
	/*
		m_ctrlOptionGrid.SetColWidth(0, 300); //300
		m_ctrlOptionGrid.SetColWidth(1, 1000); //300

		m_ctrlOptionGrid.SetTextMatrix(0, 1, "OPTION");

		m_ctrlOptionGrid.SetTextMatrix(1, 0, "1");
		m_ctrlOptionGrid.SetTextMatrix(2, 0, "2");
		m_ctrlOptionGrid.SetTextMatrix(3, 0, "3");
		m_ctrlOptionGrid.SetTextMatrix(4, 0, "4");
		m_ctrlOptionGrid.SetTextMatrix(5, 0, "5");
		m_ctrlOptionGrid.SetTextMatrix(6, 0, "6");
		m_ctrlOptionGrid.SetTextMatrix(7, 0, "7");
		m_ctrlOptionGrid.SetTextMatrix(8, 0, "C");
	*/
	m_ctrlOptionGrid.SetCol(-1);
	m_ctrlOptionGrid.SetRow(-1);

	m_ctrlOptionGrid.SetRedraw(TRUE);
	m_ctrlOptionGrid.Refresh();
}
void CDATsysView::InitCountryGrid()
{
	CString sTmp;

	char *sHeader[] = { "AREA CODE", "COUNTRY_GR","COUNTRY" };

	m_ctrlCountryGrid.SetRedraw(FALSE);
	m_ctrlCountryGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlCountryGrid.SetBackColorFixed(RGB(210, 216, 176));
	m_ctrlCountryGrid.SetForeColorFixed(RGB(51, 102, 153));
	m_ctrlCountryGrid.SetBackColor(RGB(247, 247, 231));
	m_ctrlCountryGrid.SetGridColor(RGB(200, 200, 176));

	m_ctrlCountryGrid.SetBackColorSel(RGB(247, 247, 231));

	m_ctrlCountryGrid.SetFixedRows(0);
	m_ctrlCountryGrid.SetFixedCols(1);

	m_ctrlCountryGrid.SetFillStyle(flexFillRepeat);
	m_ctrlCountryGrid.SetFontName("Arial");
	//2IN1
#ifdef _DFT_2IN1
	m_ctrlCountryGrid.SetFontSize(9);
	//
#endif
#ifdef _DFT_3IN1
	m_ctrlCountryGrid.SetFontSize(8);
#endif

	m_ctrlCountryGrid.SetFontBold(TRUE);

	m_ctrlCountryGrid.SetCols(2);
	m_ctrlCountryGrid.SetRows(3);
	m_ctrlCountryGrid.SetExtendLastCol(TRUE);

	m_ctrlCountryGrid.SetColAlignment(0, flexAlignCenterCenter);
	//2IN1
#ifdef _DFT_2IN1
	m_ctrlCountryGrid.SetColWidth(0, 1300);
	//
#endif
#ifdef _DFT_3IN1
	m_ctrlCountryGrid.SetColWidth(0, 1200);
#endif
	m_ctrlCountryGrid.SetFixedAlignment(0, flexAlignCenterCenter);

	//	m_ctrlCountryGrid.SetRowHeight(1, 400);
		// Set Column Alignment & Column Title
	for (int nRow = 0; nRow < 3; nRow++)
	{
		m_ctrlCountryGrid.SetCol(0);
		m_ctrlCountryGrid.SetRow(nRow);
		//2in1	
#ifdef _DFT_2IN1
		m_ctrlCountryGrid.SetRowHeight(nRow, 290);
#endif
		//
		m_ctrlCountryGrid.SetText(sHeader[nRow]);
	}
	m_ctrlCountryGrid.SetCol(-1);
	m_ctrlCountryGrid.SetRow(-1);

	m_ctrlCountryGrid.SetRedraw(TRUE);
	m_ctrlCountryGrid.Refresh();
}
void CDATsysView::InitVersionGrid()
{
	CString sTmp;

	char *sHeader[] = { "S/W Ver.", "MICOM Ver.","MNT Ver." };


	m_ctrlVersionGrid.SetRedraw(FALSE);
	m_ctrlVersionGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlVersionGrid.SetBackColorFixed(RGB(210, 216, 176));
	m_ctrlVersionGrid.SetForeColorFixed(RGB(51, 102, 153));
	m_ctrlVersionGrid.SetBackColor(RGB(247, 247, 231));
	m_ctrlVersionGrid.SetGridColor(RGB(200, 200, 176));

	m_ctrlVersionGrid.SetBackColorSel(RGB(247, 247, 231));

	m_ctrlVersionGrid.SetFixedRows(0);
	m_ctrlVersionGrid.SetFixedCols(1);

	m_ctrlVersionGrid.SetFillStyle(flexFillRepeat);
	m_ctrlVersionGrid.SetFontName("Arial");
	//2In1
#ifdef _DFT_2IN1
	m_ctrlVersionGrid.SetFontSize(9);
	//
#endif
#ifdef _DFT_3IN1
	m_ctrlVersionGrid.SetFontSize(8);
#endif

	m_ctrlVersionGrid.SetFontBold(TRUE);

	m_ctrlVersionGrid.SetCols(2);
	m_ctrlVersionGrid.SetRows(3);
	m_ctrlVersionGrid.SetExtendLastCol(TRUE);

	m_ctrlVersionGrid.SetColAlignment(0, flexAlignCenterCenter);
	//2In1
#ifdef _DFT_2IN1
	m_ctrlVersionGrid.SetColWidth(0, 1300);
	//
#endif
#ifdef _DFT_3IN1
	m_ctrlVersionGrid.SetColWidth(0, 1200);
#endif
	m_ctrlVersionGrid.SetFixedAlignment(0, flexAlignCenterCenter);

	//	m_ctrlVersionGrid.SetRowHeight(1, 400);
		// Set Column Alignment & Column Title
	for (int nRow = 0; nRow < 3; nRow++)
	{
		m_ctrlVersionGrid.SetCol(0);
		m_ctrlVersionGrid.SetRow(nRow);
		//2In1
#ifdef _DFT_2IN1
		m_ctrlVersionGrid.SetRowHeight(nRow, 290);
#endif
		//		
		m_ctrlVersionGrid.SetText(sHeader[nRow]);

	}
	m_ctrlVersionGrid.SetCol(-1);
	m_ctrlVersionGrid.SetRow(-1);

	m_ctrlVersionGrid.SetRedraw(TRUE);
	m_ctrlVersionGrid.Refresh();
}
#endif

void CDATsysView::OnRdoToolOption1(UINT nButtonID) 
{
	UpdateData(TRUE);	

	if(m_bInput_Lock){
		m_nToolOptionMethod = CurrentSet->nToolOptionSetMethod;
		return;
	}

	switch(nButtonID) 
	{
		case IDC_RDO_TOOL_OPTION1: 
			m_nToolOptionMethod	= 0;		
		break;
		
		case IDC_RDO_TOOL_OPTION2: 
			m_nToolOptionMethod	= 1;	
		break;

		case IDC_RDO_TOOL_OPTION3: 
			m_nToolOptionMethod	= 2;	
		break;
	}
}
void CDATsysView::OnCheckMeson() 
{
	CString sTmp = _T("");

	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}
	if(CurrentSet->bUploadMes){
		CurrentSet->bUploadMes = FALSE;
	}
	else if(CurrentSet->bIsRunning == 0)
	{
		CurrentSet->bUploadMes = TRUE;
		if(CurrentSet->bUseGmes && (!CurrentSet->bGMES_Connection))
		{
			if(gGmesCtrl.SoketCom_Creat(CurrentSet->strGmesIP, CurrentSet->nGmesPort)){
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
			else{
			//	AfxMessageBox("[GMES Connection]  = FAIL");
				AddStringToStatus("[GMES Connection]  = FAIL");
			}
		}
	}
	
	m_LedsMesOn.SetLedState(CurrentSet->bUploadMes);
}

BOOL CDATsysView::GrabberOpen()
{
	int			lRet;
	
	lRet	=	m_clsPCI.DeviceOpen(g_nRunningProcessNo - 1);

	if (lRet) {
		AddStringToStatus("Grabber Connect Failed.");
		m_bGrabConnection = FALSE;
		return FALSE;
	}
	else{
		AddStringToStatus("Grabber Successfully Connected.");
		m_bGrabConnection = TRUE;
		return TRUE;
	}
}
BOOL CDATsysView::GrabberClose()
{
	int		lRet;
//	DWORD	dwCode;

	lRet = m_clsPCI.CM_RegisterWrite(0x010, 0);

	StopLVDSGrabThread();

	lRet = m_clsPCI.DeviceClose();

	if (lRet == 0) {
		AddStringToStatus("Grabber Close Successfully Done");
		return TRUE;
	}
	else{
		AddStringToStatus("Grabber Close Fail.");
		return FALSE;
	}
}
BOOL CDATsysView::GrabberReset()
{
	int		lRet=0;
	CString sTemp;
	CString		strRegRd1, strRegRd2; //, strDrvVer, strHWInfo, strPackID;
//	CString		strDrvVer, strHWInfo;
	__int64		nRegRd64;
	int			val1, val2, val3, val4, val5, val6, packid;
//	int			pciNo, DrvVer, BusNo, FuncNo, DevNo;

/*	lRet = m_clsPCI.DeviceReConfig();
	Sleep(200);
	
	if (lRet == 0) {
		AddStringToStatus("Grabber ReConfig Successfully Done");
		//	return TRUE;
	}
	else{
		sTemp.Format("Grabber Reset Fail. Error Code - %d", lRet); 
		AddStringToStatus(sTemp);
		return FALSE;
	}

	lRet = m_clsPCI.DeviceClose();
	Sleep(300);

	lRet	=	m_clsPCI.DeviceOpen(g_nRunningProcessNo - 1);
	Sleep(300);
*/
	m_clsPCI.Aurora_ReInit();

	lRet = m_clsPCI.DeviceReset();

	if (lRet == 0) {
		AddStringToStatus("Grabber Reset Successfully Done");
	//	return TRUE;
	}
	else{
	//	m_clsPCI.DeviceClose();
		sTemp.Format("Grabber Reset Fail. Error Code - %d", lRet); 
		AddStringToStatus(sTemp);
		//CurrentSet->bGMES_Connection
		//if (CurrentSet->nAuto_Grab_Reset == 1)
		//{

		//	gJigCtrl.Set_Grabber_Power(0);
		//	AddStringToStatus("Grabber Power OFF");
		//	Sleep(1000);
		//	gJigCtrl.Set_Grabber_Power(1);
		//	AddStringToStatus("Grabber Power ON");
		//	Sleep(2000);
		//	//gJigCtrl.Set_IF_Pack_Reset(0);
		//	//AddStringToStatus("IF Pack Reset");
		//	Sleep(1000);
		//	gJigCtrl.Set_Gender_Reset(0);
		//	AddStringToStatus("Gender Reset");
	//}


	////void CGrabPage::OnBnClickedBtnResetIfpack()
	////{
	////	// TODO: 
	////	gJigCtrl.Set_IF_Pack_Reset(0);
	////}


	////void CGrabPage::OnBnClickedBtnResetGender()
	////{
	////	// TODO: 
	////	gJigCtrl.Set_Gender_Reset(0);
	//	}
			
		return FALSE;
	}

	if (lRet == 0) {
		Sleep(1);
		lRet = m_clsPCI.CM_RegisterRead(0x0, &nRegRd64);
		if (lRet == 0) {
			val1	=	(nRegRd64&0xf8000000)>>27		;
			val2	=	(nRegRd64&0x07800000)>>23		;
			val3	=	(nRegRd64&0x007e0000)>>17		;
			val4	=	(nRegRd64&0x0001f000)>>12		;
			val5	=	(nRegRd64&0x00000fc0)>>6		;
			val6	=	(nRegRd64&0x0000003f)			;
			packid	=	(nRegRd64>>40)&0x3				;
		//	strPackID.Format(_T("%02x"), packid)		;

			strRegRd1.Format(_T("Main:%02d%02d%02d_%02d%02d%02d"), val3, val2, val1, val4, val5);
			lRet = m_clsPCI.CM_RegisterRead(0x200000, &nRegRd64);
			if (lRet == 0) {
				val1	=	(nRegRd64&0xf8000000)>>27		;
				val2	=	(nRegRd64&0x07800000)>>23		;
				val3	=	(nRegRd64&0x007e0000)>>17		;
				val4	=	(nRegRd64&0x0001f000)>>12		;
				val5	=	(nRegRd64&0x00000fc0)>>6		;
				val6	=	(nRegRd64&0x0000003f)			;
				strRegRd2.Format(_T("Slave:%02d%02d%02d_%02d%02d%02d"), val3, val2, val1, val4, val5);
			}
			else {
			//	m_clsPCI.DeviceClose()	;
				sTemp.Format("Read Slave Reg Fail. Error Code - %d", lRet); 
				AddStringToStatus(sTemp);
			//	return FALSE;
			}

		}
		else {
		//	m_clsPCI.DeviceClose()	;
			sTemp.Format("Read Master Reg Fail. Error Code - %d", lRet); 
			AddStringToStatus(sTemp);
		//	return FALSE;
		}
		sTemp.Format("%s,%s", strRegRd1,strRegRd2);
		m_sGrabberFW_Version = sTemp;
	//	m_szGrabberVersion = m_sGrabberFW_Version;
		GetDlgItem(IDC_GRABBER_VER)->SetWindowText(m_sGrabberFW_Version);
	//	UpdateData(FALSE);
	}
	return TRUE;

}
/*
void CDATsysView::FillBmInfo( BITMAPINFO8* bmi, int width, int height, int bpp, int origin )
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	memset(bmi, 0, sizeof(BITMAPINFO8));

	bmi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bmi->bmiHeader.biWidth			= width;
	bmi->bmiHeader.biHeight			= origin ? abs(height) : -abs(height);
	bmi->bmiHeader.biPlanes			= 1;
	bmi->bmiHeader.biBitCount		= (unsigned short)bpp;
	bmi->bmiHeader.biSizeImage		= WIDTHBYTES(width*bpp)*height;
	bmi->bmiHeader.biCompression	= BI_RGB;

	if (bpp == 8)
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			bmi->bmiColors[i].rgbBlue		= i;
			bmi->bmiColors[i].rgbGreen		= i;
			bmi->bmiColors[i].rgbRed		= i;
			bmi->bmiColors[i].rgbReserved	= 0;
		}
	}
}
*/

void CDATsysView::OnJigdownGood() 
{
	CurrentSet->nJigUpType = 0;	
}
void CDATsysView::OnJigdownAll() 
{
	CurrentSet->nJigUpType = 1;	
}
void CDATsysView::OnJigdownNot() 
{
	CurrentSet->nJigUpType = 2;		
}
void CDATsysView::ProcessJigButton(char nData)
{
	LONG nStatus = 0;

	if(g_nRunningProcessNo == 1){
		switch(nData)
		{
			case 0x01 :
				if (CurrentSet->bIsRunning){
					nStatus = STOP_BTN1; 
				}
				else{
					nStatus = START_BTN1;
				}
				break;

			case 0x02 : 
				nStatus = RETRY_BTN1; 
				break;

			case 0x04 : 
				nStatus = OK_BTN1; 
				break;

			case 0x08 : 
				nStatus = NG_BTN1; 
				break;

			default :
				nStatus =0;
		}			
	}
	else if(g_nRunningProcessNo == 2){
		switch(nData)
		{
			case 0x01 :
				if (CurrentSet->bIsRunning){
					nStatus = STOP_BTN2; 
				}
				else{
					nStatus = START_BTN2;
				}
				break;

			case 0x02 : 
				nStatus = RETRY_BTN2; 
				break;

			case 0x04 : 
				nStatus = OK_BTN2; 
				break;

			case 0x08 : 
				nStatus = NG_BTN2; 
				break;

			default :
				nStatus =0;
		}			
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN1);
					}
					else
					{
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}


							if(CurrentSet->nRunStartType == MANUAL_START)
							{
							//	CurrentSet->bIsRunMsg = TRUE;
								PostMessage(WM_COMMAND, ID_RUN_RUN);
							}
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN1);
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
							m_bResultDlgActivate = FALSE;
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN1);
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN1);
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN1);
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_START_BTN2);
					}
					else
					{
						if (!CurrentSet->bIsRunning && !CurrentSet->bIsRunMsg)
						{
							if(m_bResultDlgActivate)
							{
								SendMessage(UM_CLOSE_RESULT_DLG,0,0); 
								m_bResultDlgActivate = FALSE;
							}


							if(CurrentSet->nRunStartType == MANUAL_START)
							{
							//	CurrentSet->bIsRunMsg = TRUE;
								PostMessage(WM_COMMAND, ID_RUN_RUN);
							}
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_STOP_BTN2);
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
							m_bResultDlgActivate = FALSE;
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_RETRY_BTN2);
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_OK_BTN2);
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
					else if(m_pInputCheckDlg.m_bActivate){
						::SendMessage(m_pInputCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
					else if (m_pVfMeasureCheckDlg.m_bActivate) {
						::SendMessage(m_pVfMeasureCheckDlg, WM_JOYSTICK_MSG, 0, MSG_NG_BTN2);
					}
				}
				break;
			default :
			break;
		}
	}

}
int  CDATsysView::GetBarcodeData(CString& sData)
{
	if (g_nSysRobotType == AUTO_SYS)//godtech 190531
	{
		OnScannerDisable();
		return 0;
	}
	if (g_nUseNoScanType == 1)//godtech 190722
	{
		return 0;
	}
	BOOL bResult  = FALSE;
	DWORD dwEvent = 1000;
	int nSize = 0;
	char* sBarcode;

	if(g_nRunningProcessNo == 1)
	{
	//	dwEvent =  WaitForSingleObject(hBarcodeServerEvents[EVENT_BARCODE1_COMPLETE],500);			

		::SetEvent(hBarcodeClientEvents[EVENT_BARCODE1]);
	}
	else
	{
	//	dwEvent =  WaitForSingleObject(hBarcodeServerEvents[EVENT_BARCODE2_COMPLETE],500);			

		::SetEvent(hBarcodeClientEvents[EVENT_BARCODE2]);
	}

	do
	{
		//===========================
		// Wait Barcode Data1 Response
		//===========================
		if(g_nRunningProcessNo == 1)
		{
			dwEvent =  WaitForSingleObject(hBarcodeServerEvents[EVENT_BARCODE1_COMPLETE],5000);			
	
			if(dwEvent == WAIT_OBJECT_0) 
			{
				if(m_pBarcodeData->nSize >= 10)
				{
					nSize = m_pBarcodeData->nSize;
					sBarcode = m_pBarcodeData->sData;
					sData.Format("%s", sBarcode) ;
					bResult = TRUE; 
				}
				else
				{
					bResult = FALSE; 
					nSize = 0;
				}
				dwEvent = -1; 

				::ResetEvent(hBarcodeServerEvents[EVENT_BARCODE1_COMPLETE]);

			}
			else
			{
				bResult = FALSE; dwEvent = -1; 
			}
		}
		else
		{
			dwEvent =  WaitForSingleObject(hBarcodeServerEvents[EVENT_BARCODE2_COMPLETE],5000);			
	
			if(dwEvent == WAIT_OBJECT_0) 
			{
				if(m_pBarcodeData->nSize >= 10)
				{
					nSize = m_pBarcodeData->nSize;
					sData = m_pBarcodeData->sData;
					bResult = TRUE; 
				}
				else
				{
					bResult = FALSE; 
					nSize = 0;
				}
				dwEvent = -1; 

				::ResetEvent(hBarcodeServerEvents[EVENT_BARCODE2_COMPLETE]);

			}
			else
			{
				bResult = FALSE; dwEvent = -1; 
			}
		}
	}while(dwEvent != -1);

	if(!bResult)
	{
		return 0;
	}
	else{
		return nSize;
	}
}
double  CDATsysView::GetDaqData(int nFunc, int nCh_S, int nCh_E, CString& sData)
{

	return 0;
//	DWORD dwEvent = 1000;
//	double dDaqData;
//	char* sDaqData;
//	if (g_nSysRobotType == AUTO_SYS)//godtech 190531
//	{
//		return 0;
//	}
//	if (g_nUseNoScanType == 1)//godtech 190722
//	{
//		return 0;
//	}
//
//	m_pDaqData->nFuncIndex = nFunc;
//	m_pDaqData->nStartChannel = nCh_S;
//	m_pDaqData->nEndChannel = nCh_E;
//
//	if(g_nRunningProcessNo == 1)
//	{
//		::SetEvent(hBarcodeClientEvents[EVENT_DAQ1]);
//	}
//	else
//	{
//		::SetEvent(hBarcodeClientEvents[EVENT_DAQ2]);
//	}
//
//	do
//	{
//		//===========================
//		// Wait Daq Data1 Response
//		//===========================
//		if(g_nRunningProcessNo == 1)
//		{
//			dwEvent =  WaitForSingleObject(hBarcodeServerEvents[EVENT_DAQ1_COMPLETE],5000);			
//	
//			if(dwEvent == WAIT_OBJECT_0) 
//			{
//				dDaqData = m_pDaqData->dRetValue;
//				sDaqData = m_pDaqData->sRetValue;
//				sData.Format("%s", sDaqData) ;
//
//				::ResetEvent(hBarcodeServerEvents[EVENT_BARCODE1_COMPLETE]);
//				dwEvent = -1; 
//
//			}
//			else
//			{
//				dDaqData = 0.0;
//				sData = "0.0";
//				dwEvent = -1; 
//			}
//		}
//		else
//		{
//			dwEvent =  WaitForSingleObject(hBarcodeServerEvents[EVENT_DAQ2_COMPLETE],5000);			
//	
//			if(dwEvent == WAIT_OBJECT_0) 
//			{
//				dDaqData = m_pDaqData->dRetValue;
//				sDaqData = m_pDaqData->sRetValue;
//				sData.Format("%s", sDaqData) ;
//
//				::ResetEvent(hBarcodeServerEvents[EVENT_BARCODE1_COMPLETE]);
//				dwEvent = -1; 
//
//			}
//			else
//			{
//				dDaqData = 0.0;
//				sData = "0.0";
//				dwEvent = -1; 
//			}
//		}
//	}while(dwEvent != -1);
//
//	return dDaqData;
}
#if 1

BOOL CDATsysView::CheckToolCrc()
{
	vector<unsigned int> tool(11,0);
	unsigned int ID = 0x8005;   //CRC SumÀÇ key°

	unsigned int CRC = 0;       //CRC Sum
	unsigned int nCurCRC = 0;       
	BOOL bCrcCheck = FALSE;
	CString sVal;	

	for(int i = 1; i < 10; i++)
	{
		sVal = m_ctrlListOption.GetItemText(i,1);//sVal = m_ctrlListOption.GetTextMatrix(i, 1);
		if(sVal != ""){
			tool[i] = atoi(sVal);
			if(tool[i] > 0xFFFF){bCrcCheck = TRUE;}
		}
	}
	if(!bCrcCheck) { return TRUE;}

	sVal = m_ctrlListOption.GetItemText(10, 1); //sVal = m_ctrlOptionGrid.GetTextMatrix(10, 1);
	if(sVal != ""){
		nCurCRC = atoi(sVal);
	}



	for(int t=1;t<=9;t++){      // ToolOption 1~9¹ø±îÁö ¸ðµç dataµéÀ» È®ÀÎÇÑ´Ù.
		unsigned int val = tool[t];          //ÇØ´ç ToolOption°ªÀ» ¿ì¼±ÀûÀ¸·Î val°ª¿¡ ÀúÀå
		vector<int> bitarray = makeBitArray(val);         //ÇØ´ç val¸¦ bitarray·Î ¹Ù²ãÁÖ´Â ºÎºÐ vector<int> ´Â intÇü vector (array·Î »ý°¢ÇØµµ ¹«¹æ)
		for(int i=0;i<32;i++){				 //32bit¿¡ ÇØ´çµÇ´Â ¸ðµç bit¸¦ ÇÑ bit¾¿ ¼øÈ¸ °¡Àå Å« bitºÎÅÍ È®ÀÎ
			if((bitarray[i]) ^ (CRC>>15)){		 //ÇöÀç bit¿¡ ÇØ´çµÇ´Â ºÎºÐ°ú CRCÀÇ °¡Àå Å« ¼ýÀÚ Áï (CRC>>15¸¦ ÇÒ½Ã 2Áø¹ý»ó °¡ÀåÅ«¼ö)ÀÇ xorÀÌ 1ÀÏ½Ã ½ÇÇà
				CRC = ((CRC << 1) & 0xffff) ^ ID;	// (CRC<<1Àº CRC*2¿Í µ¿ÀÏ) CRC ¿À¸¥ÂÊÀ¸·Î 1bit shift°ª°ú ID°ªÀ¸ xor°ªÀ» CRC¿¡ ÀúÀå //((CRC << 1) & 0xffff)ÀÌÀ¯´Â CRC°¡ unsigned intÀÌ±â ¶§¹®¿¡ 16bit¸¦ ÃÊ°úÇÒ °¡´É¼ºÀÌ ÀÖ¾î ÇØ´çºÎºÐ ¹Ý¿µ
			}	
			else{
				CRC = (CRC << 1) & 0xffff;  //xor°ªÀÌ 0ÀÏ½Ã CRC¸¸ ¿À¸¥ÂÊÀ¸·Î 1bit shift
			}
		}
	}
	
	if(nCurCRC == CRC){
		return TRUE;
	}
	else{
		CString sTemp;
		if(nCurCRC == 0){
			sTemp.Format("TOOL-CRC NG : Calculation Value-%d, Set Value-%d\nKey-In CRC Value", nCurCRC, CRC);
		}
		else{
			sTemp.Format("TOOL-CRC NG : Calculation Value-%d, Set Value-%d\nKey-In  right CRC Value", nCurCRC, CRC);
		}

		return FALSE;
	}
}
#else
BOOL CDATsysView::CheckToolCrc()
{
	vector<unsigned int> tool(11, 0);
	unsigned int ID = 0x8005;   //CRC SumÀÇ key°ª, ¹Ì¸® defineµÇ´Â ºÎºÐÀÌ¸ç ¸¹ÀÌ »ç¿ëµÇ°í ÀÖ´Â 0x8005·Î »ç¿ë

	unsigned int CRC = 0;       //CRC SumÀÇ ÃÊ±â°ªÀº 0À¸·Î µÐ´Ù
	unsigned int nCurCRC = 0;
	BOOL bCrcCheck = FALSE;
	CString sVal;

	for (int i = 1; i < 10; i++)
	{
		sVal = m_ctrlOptionGrid.GetTextMatrix(i, 1);
		if (sVal != "") {
			tool[i] = atoi(sVal);
			if (tool[i] > 0xFFFF) { bCrcCheck = TRUE; }
		}
	}
	if (!bCrcCheck) { return TRUE; }

	sVal = m_ctrlOptionGrid.GetTextMatrix(10, 1);
	if (sVal != "") {
		nCurCRC = atoi(sVal);
	}



	for (int t = 1; t <= 9; t++) {      // ToolOption 1~9¹ø±îÁö ¸ðµç dataµéÀ» È®ÀÎÇÑ´Ù.
		unsigned int val = tool[t];          //ÇØ´ç ToolOption°ªÀ» ¿ì¼±ÀûÀ¸·Î val°ª¿¡ ÀúÀå
		vector<int> bitarray = makeBitArray(val);         //ÇØ´ç val¸¦ bitarray·Î ¹Ù²ãÁÖ´Â ºÎºÐ vector<int> ´Â intÇü vector (array·Î »ý°¢ÇØµµ ¹«¹æ)
		for (int i = 0; i < 32; i++) {				 //32bit¿¡ ÇØ´çµÇ´Â ¸ðµç bit¸¦ ÇÑ bit¾¿ ¼øÈ¸ °¡Àå Å« bitºÎÅÍ È®ÀÎ
			if ((bitarray[i]) ^ (CRC >> 15)) {		 //ÇöÀç bit¿¡ ÇØ´çµÇ´Â ºÎºÐ°ú CRCÀÇ °¡Àå Å« ¼ýÀÚ Áï (CRC>>15¸¦ ÇÒ½Ã 2Áø¹ý»ó °¡ÀåÅ«¼ö)ÀÇ xorÀÌ 1ÀÏ½Ã ½ÇÇà
				CRC = ((CRC << 1) & 0xffff) ^ ID;	// (CRC<<1Àº CRC*2¿Í µ¿ÀÏ) CRC ¿À¸¥ÂÊÀ¸·Î 1bit shift°ª°ú ID°ªÀ¸ xor°ªÀ» CRC¿¡ ÀúÀå //((CRC << 1) & 0xffff)ÀÌÀ¯´Â CRC°¡ unsigned intÀÌ±â ¶§¹®¿¡ 16bit¸¦ ÃÊ°úÇÒ °¡´É¼ºÀÌ ÀÖ¾î ÇØ´çºÎºÐ ¹Ý¿µ
			}
			else {
				CRC = (CRC << 1) & 0xffff;  //xor°ªÀÌ 0ÀÏ½Ã CRC¸¸ ¿À¸¥ÂÊÀ¸·Î 1bit shift
			}
		}
	}

	if (nCurCRC == CRC) {
		return TRUE;
	}
	else {
		CString sTemp;
		if (nCurCRC == 0) {
			sTemp.Format("TOOL-CRC NG : Calculation Value-%d, Set Value-%d\nKey-In CRC Value", nCurCRC, CRC);
		}
		else {
			sTemp.Format("TOOL-CRC NG : Calculation Value-%d, Set Value-%d\nKey-In  right CRC Value", nCurCRC, CRC);
		}

		return FALSE;
	}
}
#endif
//void CDATsysView::OnBtnCheckCrc() 
//{
//	CheckToolCrc();	
//}
BOOL CDATsysView::Check_KeyDownload()
{
	int nLeft=0, nTop=0, nWidth=0, nHeight=0;
	
	CString sWriteData = _T("");
	CString szPrevMsg = _T("");
	CString sMsg = _T("");
	BOOL bReturn = TRUE;
	int nKeyItem = 0;
	int nRow;
	int AcRetry = 0;
	//=================
	// Modeless Dialog
	//=================

	m_pKeyCheckDlg = new CKeyDownloadCheckDlg(this);
	m_pKeyCheckDlg->Create(IDD_KEY_DOWNLOAD_CHECK_DLG);

	nLeft   = m_FormViewRect.left + (m_FormViewRect.Width()/2)  - (m_pKeyCheckDlg->m_dlgRect.Width()/2);
	nTop    = (m_FormViewRect.Height()/2) - (m_pKeyCheckDlg->m_dlgRect.Height()/2);
	nWidth  = m_pKeyCheckDlg->m_dlgRect.Width();
	nHeight = m_pKeyCheckDlg->m_dlgRect.Height();
	
	m_pKeyCheckDlg->MoveWindow(nLeft,nTop,nWidth,nHeight);
	m_pKeyCheckDlg->ShowWindow(SW_SHOW);
	m_pKeyCheckDlg->UpdateWindow();
	///////////////////////////ToolOption Write

	if(!CurrentSet->bDrmKey){

		
		if(_Option_Check() == 0.0){
			m_pKeyCheckDlg->UpdateResult(1, 0);
			sMsg = sMsg + "Option(NG)";
			bReturn = FALSE;
		}
		else{
			CurrentSet->nDrmKeyResultVal = 1;
			m_pKeyCheckDlg->UpdateResult(1, 1);
			sWriteData.Format("DFT%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
			sMsg = sMsg + sWriteData;
			if(!TVCommCtrl.ModelSNo_Write(sWriteData, FALSE)){
				AddStringToStatus("Option(SN Write NG)");
				bReturn = FALSE;
			}
		}
		

		for(nRow = 2; nRow < 15 ; nRow++)
		{
			m_pKeyCheckDlg->UpdateResult(nRow, 2);
		}
	}
	else{
		//nKeyItem_GMES = CheckKeyGmesVal();

		nKeyItem = CheckKeyGmesVal();
		for(nRow = 1; nRow < 15 ; nRow++)
		{
			
			TVCommCtrl.m_RcvTimeOut = 0;
			if(nKeyItem & 0x01)
			{
				switch(nRow){
					case 1:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = 1;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(_Option_Check() == 0.0){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + "Option(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = 1;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 2:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x002;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.MAC_AddressCheck(3000)){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " MAC_ADDRESS(NG)";
								bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x002;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 3:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x004;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.ESN_Download_Check(3000)){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " ESN(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x004;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 4:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x008;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.Widevine_Download_Check(3000)){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " WIDEVINE(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x008;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 5:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x010;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.HDCP_Rx_14_DL_Check()){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " HDCP_RX_1.x(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x010;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 6:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x020;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.HDCP_Rx_20_DL_Check()){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " HDCP_RX_2.x(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x020;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 7:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x040;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.Google_Key_DL_Check()){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " GoogleTV(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x040;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;
						
					case 8:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x080;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.Marlin_Download_Check(3000)){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " MARINKEY(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x080;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;
						
					case 9:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x100;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.CI_Download_Check1(3000)){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " CI(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x100;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;
						
					case 10:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x200;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if(!TVCommCtrl.CNTV_Key_DL_Check()){
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " CNTV(NG)";
							bReturn = FALSE;
						}
						else{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x200;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;
						
					case 11:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x400;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if (!TVCommCtrl.DTCP_Rx_Key_DL_Check()) {
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " DTCP_RX(NG)";
							bReturn = FALSE;
						}
						else {
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x400;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 12:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x800;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if (!TVCommCtrl.ATSC_30_Key_DL_Check()) {
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " UHCP(NG)";
							bReturn = FALSE;
						}
						else {
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x800;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

					case 13:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x1000;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if (!TVCommCtrl.MFi_Key_DL_Check()) {
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " MFi(NG)";
							bReturn = FALSE;
						}
						else {
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x1000;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;
					case 14:
						if (m_bKeyDLCheckResultList[nRow] == 1)
						{
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x2000;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
							break;
						}
						if (!TVCommCtrl.CI_ECP_Download_Check(3000)) {
							m_pKeyCheckDlg->UpdateResult(nRow, 0);
							sMsg = sMsg + " CI+ECP(NG)";
							bReturn = FALSE;
						}
						else {
							CurrentSet->nDrmKeyResultVal = CurrentSet->nDrmKeyResultVal | 0x2000;
							m_pKeyCheckDlg->UpdateResult(nRow, 1);
						}
						break;

				}
			}
			else{
				m_pKeyCheckDlg->UpdateResult(nRow, 2);
			}
			
			nKeyItem = nKeyItem >> 1;
			
			if (TVCommCtrl.m_RcvTimeOut == 1)
			{
				AcRetry++;
				if (AcRetry >= 2)
				{
					bReturn = FALSE;
					break;
				}
				else
				{
					gJigCtrl.Set_ACPower(0);
					SystemMonitorLog_Save("Set_ACPower(0);", _T(__FILE__), __LINE__);
					Sleep(1000);
					gJigCtrl.Set_ACPower(1);
					Sleep(2000);
				}
			}
		}
		sWriteData.Format("KEY%04X%04X", CurrentSet->nInputCheckResultVal, CurrentSet->nDrmKeyResultVal);
		sMsg = sMsg + sWriteData;
		if (!TVCommCtrl.ModelSNo_Write(sWriteData, FALSE)) {
			Sleep(100);
			if (!TVCommCtrl.ModelSNo_Write(sWriteData, FALSE)) {
				AddStringToStatus("DrmKeyCheck(SN Write NG)");
				bReturn = FALSE;
			}
		}

	}

	if(CurrentSet->bUseScanner && bReturn){
		if (CurrentSet->sPCBID_Scan != CurrentSet->sPCBID_Read)
		{
#ifndef __DEBUG_SCAN_SN_NO_WRITE
			if (!TVCommCtrl.ModelSNo_Write(m_strWipId, TRUE)) {
				Sleep(100);

				if (!TVCommCtrl.ModelSNo_Write(m_strWipId, TRUE)) {
					sMsg =  "(PCBID Write NG)";
					bReturn = FALSE;
				}
			}
			CurrentSet->bPCBID_Rewrite = 1;
			sMsg= "SN Write: ScanID=>";
			sMsg += CurrentSet->sPCBID_Scan;
			sMsg += ",Read ID=>";
			sMsg += CurrentSet->sPCBID_Read;

			AddStringToStatus(sMsg);
#else
			
			sMsg = "(PCBID Different : Write SKIP)";
			
			CurrentSet->bPCBID_Rewrite = 0;
			sMsg = "SN Write: ScanID=>";
			sMsg += CurrentSet->sPCBID_Scan;
			sMsg += ",Read ID=>";
			sMsg += CurrentSet->sPCBID_Read;

			AddStringToStatus(sMsg);
#endif	
		}
		else
		{
			CurrentSet->bPCBID_Rewrite = 0;
			sMsg = "SN No Write: ScanID=>";
			sMsg += CurrentSet->sPCBID_Scan;
			sMsg += ",Read ID=>";
			sMsg += CurrentSet->sPCBID_Read;

			AddStringToStatus(sMsg);

		}
	}


	m_pKeyCheckDlg->SaveResult2File();

	m_pKeyCheckDlg->DestroyWindow();
	delete m_pKeyCheckDlg;
	m_pKeyCheckDlg = NULL;


	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg); 
	if(szPrevMsg != _T(""))
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + sMsg);
		pCurStep->m_strMsg = szPrevMsg + "; " + sMsg;
	}
	else
	{
		g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
		pCurStep->m_strMsg = sMsg;
	}

	

	return bReturn;
}
BOOL CDATsysView::Check_InputDetect()
{
	BOOL bReturn = TRUE;

	m_pInputCheckDlg.DoModal();

	bReturn = m_pInputCheckDlg.m_bInputCheckResult;

	return bReturn;
}

#ifdef SM_MODIFY_CODE__
BOOL CDATsysView::Check_HP_Disable()
{
	BOOL bReturn = TRUE;
	int lTestcount = 0;

	
	bReturn = TVCommCtrl.Input_Detect_Check(JACK_HEAD_PHONE, 0);

	while (!bReturn) 
	{
		CString szMsg;

		m_ManualTestDlg.SetInfoMessage("HP JACK DISABLE CHECK", "HP REMOVE JACK!!", " ");
		m_ManualTestDlg.DoModal();
		m_ManualTestDlg.ClearInfoMessage();


		if (m_ManualTestDlg.m_bTestResult)
		{
			bReturn = TVCommCtrl.Input_Detect_Check(JACK_HEAD_PHONE, 0);
			if (m_ManualTestDlg.m_intTestResult == 4)
			{
				CurrentSet->bRunAbort = TRUE;//m_nRunType = STEP_STOP;//
				break;
			}
			else
			{
				lTestcount++;
				if (lTestcount >= 3)
				{
					break;
				}
			}
		}
		else
		{
			bReturn = TVCommCtrl.Input_Detect_Check(JACK_HEAD_PHONE, 0);
			break;
		}
			
	}
	
	

	return bReturn;
}


BOOL CDATsysView::Loop_Auto_Check()
{
	BOOL bReturn = TRUE;
	//AUTO_COUNT_ID_HDMI

	int nStepno = 1;
	int lnSaveStepno = 1;

	//if (TVCommCtrl.Auto_Port_Detect(AUTO_COUNT_ID_HDMI))
	if (TVCommCtrl.Auto_Port_Detect(AUTO_COUNT_ID_MAX))
	{
		bReturn = TRUE;
	}
	else
	{
		return FALSE;
	}

	

	nStepno = pCurStep->GetStepNo();
	//POSITION lstepPos = StepList.get
	//	StepList.InsertAfter(PosStep, pCurStep);
	lnSaveStepno = nStepno;
	//POSITION	lp_CurPosStep = StepList.FindIndex(nStepno-1);
	//POSITION	lp_CurPosStep = StepList.Find("port")

	POSITION	PosForceStep;
	PosForceStep = PosStep;
	CStep *pForceStep = new CStep();
	g_nStepPlusCount = 0;
	
	for (int i = 0; i < AUTO_COUNT_ID_MAX; i++)
	{		
		switch (i)
		{
			case AUTO_COUNT_ID_HEADPHONE:
			{
				if (TVCommCtrl.m_AutoCountPort[AUTO_COUNT_ID_HEADPHONE] >= 1)
				{
					//PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HEADPHONE);
					//pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					//pForceStep->SetTest(TRUE);
					ParseForceInsertStep(AUTO_COUNT_ID_HEADPHONE, PosForceStep);
				}
	/*			else
				{
					PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HEADPHONE);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);
				}
*/
					
				break;
			}
			case AUTO_COUNT_ID_GREENEYE:
			{
				if (TVCommCtrl.m_AutoCountPort[AUTO_COUNT_ID_GREENEYE] >= 1)
				{
					ParseForceInsertStep(AUTO_COUNT_ID_GREENEYE, PosForceStep);
					/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_GREENEYE);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(TRUE);*/
				}
				else
				{
					/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_GREENEYE);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);*/
				}
				break;
			}					
			case AUTO_COUNT_ID_COMPONENT:
			{
				if (TVCommCtrl.m_AutoCountPort[i] >= 1)
				{
					ParseForceInsertStep(FORCED_CHECK_ID_COMPONENT, PosForceStep);
				/*	PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_COMPONENT);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(TRUE);*/
				}
				else
				{/*
					PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_COMPONENT);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);*/
				}
				break;
			}
			case AUTO_COUNT_ID_AV:
			{
				if (TVCommCtrl.m_AutoCountPort[i] >= 1)
				{
					ParseForceInsertStep(FORCED_CHECK_ID_AV, PosForceStep);
					/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_AV);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(TRUE);*/
				}
				else
				{
				/*	PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_AV);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);*/
				}
				break;
			}
			case AUTO_COUNT_ID_USB:
			{
				if (TVCommCtrl.m_AutoCountPort[i] >= 1)
				{
					ParseForceInsertStep(FORCED_CHECK_ID_USB_1, PosForceStep);
					/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_1);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(TRUE);*/
					if (TVCommCtrl.m_AutoCountPort[i] >= 2)
					{
						ParseForceInsertStep(FORCED_CHECK_ID_USB_2, PosForceStep);
						/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_2);
						pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
						pForceStep->SetTest(TRUE);*/
						if (TVCommCtrl.m_AutoCountPort[i] >= 3)
						{
							ParseForceInsertStep(FORCED_CHECK_ID_USB_3, PosForceStep);
							/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_3);
							pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
							pForceStep->SetTest(TRUE);*/
						}
						else
						{
							/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_3);
							pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
							pForceStep->SetTest(FALSE);*/
						}
					}
					else
					{
						/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_2);
						pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
						pForceStep->SetTest(FALSE);
						PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_3);
						pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
						pForceStep->SetTest(FALSE);*/
					}
				}
				else
				{
					/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_1);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);
					PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_2);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);
					PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_USB_3);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);*/
				}
				break;
			}			
			case AUTO_COUNT_ID_HDMI:
			{

				if (TVCommCtrl.m_AutoCountPort[i] >= 1)
				{
					ParseForceInsertStep(FORCED_CHECK_ID_HDMI_1, PosForceStep);
					if (CurrentSet->nARC_Force_Port_Number == FORCE_ARC_HDMI_PORT_1)
					{
						ParseForceInsertStep(FORCED_CHECK_ID_HDMI_1_ARC, PosForceStep);
					}
					/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_1);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(TRUE);*/
					if (TVCommCtrl.m_AutoCountPort[i] >= 2)
					{
						ParseForceInsertStep(FORCED_CHECK_ID_HDMI_2, PosForceStep);
						if (CurrentSet->nARC_Force_Port_Number == FORCE_ARC_HDMI_PORT_2)
						{
							ParseForceInsertStep(FORCED_CHECK_ID_HDMI_2_ARC, PosForceStep);
						}
						/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_2);
						pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
						pForceStep->SetTest(TRUE);*/
						if (TVCommCtrl.m_AutoCountPort[i] >= 3)
						{
							ParseForceInsertStep(FORCED_CHECK_ID_HDMI_3, PosForceStep);
							if (CurrentSet->nARC_Force_Port_Number == FORCE_ARC_HDMI_PORT_3)
							{
								ParseForceInsertStep(FORCED_CHECK_ID_HDMI_3_ARC, PosForceStep);
							}
							/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_3);
							pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
							pForceStep->SetTest(TRUE);*/
							if (TVCommCtrl.m_AutoCountPort[i] >= 4)
							{
								ParseForceInsertStep(FORCED_CHECK_ID_HDMI_4, PosForceStep);
								if (CurrentSet->nARC_Force_Port_Number == FORCE_ARC_HDMI_PORT_4)
								{
									ParseForceInsertStep(FORCED_CHECK_ID_HDMI_4_ARC, PosForceStep);
								}
								/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_4);
								pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
								pForceStep->SetTest(TRUE);*/
							}
							else
							{
								/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_4);
								pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
								pForceStep->SetTest(FALSE);*/
							}
						}
						else
						{
							/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_3);
							pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
							pForceStep->SetTest(FALSE);
							PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_4);
							pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
							pForceStep->SetTest(FALSE);*/
						}
					}
					else
					{/*
						PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_2);
						pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
						pForceStep->SetTest(FALSE);
						PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_3);
						pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
						pForceStep->SetTest(FALSE);
						PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_4);
						pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
						pForceStep->SetTest(FALSE);*/
					}
				}
				else
				{
					/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_1);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);
					PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_2);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);
					PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_3);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);
					PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_HDMI_4);
					pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
					pForceStep->SetTest(FALSE);*/

				}
				break;
					
			}				
			//case AUTO_COUNT_ID_RGB:
			//{
			//	if (TVCommCtrl.m_AutoCountPort[i] >= 1)
			//	{
			//		ParseForceInsertStep(FORCED_CHECK_ID_RGB, PosForceStep);
			//		/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_RGB);
			//		pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
			//		pForceStep->SetTest(TRUE);*/
			//	}
			//	else
			//	{
			//		/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_RGB);
			//		pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
			//		pForceStep->SetTest(FALSE);*/
			//	}
			//		
			//	break;
			//}			
			//case AUTO_COUNT_ID_SCART:
			//{
			//	if (TVCommCtrl.m_AutoCountPort[i] >= 1)
			//	{
			//		ParseForceInsertStep(FORCED_CHECK_ID_SCART, PosForceStep);
			//		/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_SCART);
			//		pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
			//		pForceStep->SetTest(TRUE);*/
			//	}
			//	else
			//	{
			//		/*PosForceStep = ForcedAutoStepList.FindIndex(FORCED_CHECK_ID_SCART);
			//		pForceStep = ForcedAutoStepList.GetAt(PosForceStep);
			//		pForceStep->SetTest(FALSE);*/
			//	}
			//	break;
			//}						
				
			default:
				break;

		}
		
	}


	POSITION lPosition = StepList.GetHeadPosition();

	//strTmpRef = strName[nCheckID];

	int temp_No = 1;
	while (lPosition != NULL)
	{
		CStep* pStep = StepList.GetNext(lPosition);
		pStep->SetStepNo(temp_No);
		
		CString strTmp = pStep->GetStepName();
		if (strTmp.Find("#") >= 0)
		{
			BOOL lCheck = pStep->GetTest();
			if (lCheck == FALSE)
			{
				pStep->SetTest(1);
				//m_ctrlSummaryGrid.SetRow(temp_No);
				//m_ctrlSummaryGrid.SetCol(0);
				//m_ctrlSummaryGrid.SetCellChecked(1);
				m_CtrlListMainProcess.SetCheck(temp_No-1, 1); //  m_ctrlSummaryGrid.SetCellChecked(flexChecked);
				m_LockStatus[temp_No-1] = 1;
			}
			
			//m_ctrlSummaryGrid.SetRow(nRow);
			//m_ctrlSummaryGrid.SetCol(nCol);
			//if (m_ctrlSummaryGrid.GetCellChecked() == flexChecked) bCellCheck = TRUE;
			//else  bCellCheck = FALSE;
			//for (int nTmp = 0; nTmp <= StepList.GetCount(); nTmp++)
			//{
			//	m_ctrlSummaryGrid.SetRow(nTmp);
			//	if (bCellCheck == TRUE) m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
			//	else m_ctrlSummaryGrid.SetCellChecked(flexChecked);
		}

		temp_No++;

	}
	if (g_nStepPlusCount > 0)
	{		//lnSaveStepno = nStepno;
		POSITION	lp_CurPosStep = StepList.FindIndex(lnSaveStepno);
		//POSITION	lp_CurPosStep = StepList.Find("port")
		PosStep = lp_CurPosStep;

		InsertStepData2GridUpdate(CurrentSet->nDisplayType, lnSaveStepno);
		//InsertStepData2Grid(CurrentSet->nDisplayType);
	}
	return bReturn;
}
//#define FORCED_CHECK_ID_HEADPHONE		0
//#define FORCED_CHECK_ID_GREENEYE		1
//#define FORCED_CHECK_ID_COMPONENT		2
//#define FORCED_CHECK_ID_AV				3
//#define FORCED_CHECK_ID_USB_1			4
//#define FORCED_CHECK_ID_USB_2			5
//#define FORCED_CHECK_ID_USB_3			6
//#define FORCED_CHECK_ID_HDMI_1			7
//#define FORCED_CHECK_ID_HDMI_1_ARC		8
//#define FORCED_CHECK_ID_HDMI_2			9
//#define FORCED_CHECK_ID_HDMI_2_ARC		10
//#define FORCED_CHECK_ID_HDMI_3			11
//#define FORCED_CHECK_ID_HDMI_3_ARC		12
//#define FORCED_CHECK_ID_HDMI_4			13
//#define FORCED_CHECK_ID_HDMI_4_ARC		14
//#define FORCED_CHECK_ID_RGB				15
//#define FORCED_CHECK_ID_SCART			16
//#define FORCED_CHECK_ID_MAX				17
	//if (g_ForcedInputCheck == 1)
	//{
	//	POSITION	PosForceStep;
	//	PosForceStep = ForcedAutoStepList.FindIndex(0);
	//	if (PosForceStep != NULL)
	//	{
	//		pCurStep = ForcedAutoStepList.GetAt(PosForceStep);

	//		if ((nStepResult = pView->StepRun()) == TEST_ABORT)
	//		{
	//			pView->m_nNoCurrentRepeatExecution = 1; goto END_WHILE_LOOP;
	//		}
	//		if (pCurStep->m_bResult != TRUE) CurrentSet->nRunType = STEP;
	//	}
	//	g_ForcedInputCheck = 0;
	//}

	//   
	//bReturn = TVCommCtrl.Input_Detect_Check(JACK_HEAD_PHONE, 0);

	//if (!bReturn) {
	//	//MESSAGE
	////	int nResult = TEST_FAIL;
	//	CString szMsg;

	//	m_ManualTestDlg.SetInfoMessage("HP JACK DISABLE CHECK","HP REMOVE JACK!!"," " );
	//	m_ManualTestDlg.DoModal();
	//	m_ManualTestDlg.ClearInfoMessage();
	//	

	//	if (m_ManualTestDlg.m_bTestResult)
	//	{
	//		bReturn = TVCommCtrl.Input_Detect_Check(JACK_HEAD_PHONE, 0);
	//	}
	//	else
	//	{
	//		bReturn = TVCommCtrl.Input_Detect_Check(JACK_HEAD_PHONE, 0);
	//	}


	//	//bReturn = FALSE;
	//}

	

#endif
BOOL CDATsysView::Check_VfMeasure()
{
	BOOL bReturn = TRUE;

	m_pVfMeasureCheckDlg.m_bTestClose = TRUE;
	m_pVfMeasureCheckDlg.DoModal();
	bReturn = m_pVfMeasureCheckDlg.m_bVfCheckResult;

	return bReturn;
}

BOOL CDATsysView::Check_IR_Blaster()
{
	int lReadCount = 0;
	BOOL bReturn = FALSE;
	CString strRead;
	CString strMsg;
	CString strIR_KeyData;
	CString str33Data;
	double Volt33Data;
	CString szPrevMsg;

	if (gVF1000Ctrl.GetIRBlasterData(strRead, 1000) == 0)
	{
		return FALSE;
	}
	if (TVCommCtrl.Send_TVControl_Command("av 00 25", 1000) == 0)
	{
		AddStringToStatus("Command Send Error: av 00 25");
		return FALSE;
	}

	for (int i = 0; i < 5; i++)
	{

		if (gVF1000Ctrl.GetIRBlasterData(strRead, 1000))
		{
			lReadCount++;
			strIR_KeyData = strRead.Left(8);
			str33Data = strRead.Mid(8);
			str33Data.Insert(1, '.');

			strMsg = "IR Data: ";
			strMsg += strIR_KeyData;
			strMsg += " DC: ";
			strMsg += str33Data;
			strMsg += "V";

			AddStringToStatus(strMsg);

			Volt33Data = _ttoi(strRead.Mid(8));
			Volt33Data = Volt33Data / 100.0;
			if (strRead.Find("DAC048B7") >= 0)
			{


				strMsg.Format("DBG: L %2.2f,H %2.2f,DC %2.2f,", pCurStep->m_fLowLimit, pCurStep->m_fHighLimit, Volt33Data);
				AddStringToStatus(strMsg);

				if ((pCurStep->m_fLowLimit <= Volt33Data) && (pCurStep->m_fHighLimit >= Volt33Data))//if ((2.8 <= Volt33Data) && (3.8 >= Volt33Data))
				{
					AddStringToStatus("IR OK DC OK");
					TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
					return Volt33Data;
				}
				else
				{
					AddStringToStatus("IR OK DC NG");
					TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
					//"V; xx, S; OK / NG"
					//CString szPrevMsg = "";
					strMsg.Format("V: %2.2fV,S: OK", Volt33Data);
					SaveMessageList(strMsg);
					/*g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);
					if (szPrevMsg != _T(""))
					{
						g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + strMsg);
						pCurStep->m_strMsg = szPrevMsg + "; " + strMsg;
					}
					else
					{
						g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, strMsg);
						pCurStep->m_strMsg = strMsg;
					}*/
					return FALSE;
				}
			}
			else
			{
				Sleep(150);
				continue;
			}

		}
		else
		{
			TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
			//CString szPrevMsg = "";
			strMsg = "Fail to Get Test Data From IR Blaster";
			SaveMessageList(strMsg);
		/*	g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);
			if (szPrevMsg != _T(""))
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + strMsg);
				pCurStep->m_strMsg = szPrevMsg + "; " + strMsg;
			}
			else
			{
				g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, strMsg);
				pCurStep->m_strMsg = strMsg;
			}*/
			return FALSE;
		}
	}
	//m_pVfMeasureCheckDlg.m_bTestClose = TRUE;
	//m_pVfMeasureCheckDlg.DoModal();
	//bReturn = m_pVfMeasureCheckDlg.m_bVfCheckResult;
	TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
	
	strMsg.Format("V: %2.2fV,S: NG", Volt33Data);
	SaveMessageList(strMsg);
	//g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);
	//if (szPrevMsg != _T(""))
	//{
	//	g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + strMsg);
	//	pCurStep->m_strMsg = szPrevMsg + "; " + strMsg;
	//}
	//else
	//{
	//	g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, strMsg);
	//	pCurStep->m_strMsg = strMsg;
	//}
	return bReturn;
}


BOOL CDATsysView::Read_A_CAS_ID()
{
	int lReadCount = 0;
	BOOL bReturn = FALSE;
	CString strRead;
	CString strMsg;
	CString strID_Data;
	CString szPrevMsg;
	CString sMsg;


	
	if (TVCommCtrl.Send_TVControl_ACAS_ID("av 00 28", strRead, 1000) == 0)
	{
		//AddStringToStatus("ACAS_ID Error: av 00 28");
		return FALSE;
	}
	int lstart = strRead.Find("OK") + 2;	
	strID_Data = strRead.Mid(lstart);

	int lEnd = strID_Data.Find("X");
	strID_Data = strID_Data.Left(lEnd);
	if (strID_Data.GetLength() == 24)
	{
		m_A_CAS_ID_Check = strID_Data;

		sMsg = "A_CAS_ID:";
		sMsg += strID_Data;

		g_pView->GetResultFromDetailedGrid(pCurStep->m_nStep, szPrevMsg);
		if (szPrevMsg != _T(""))
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, szPrevMsg + "; " + sMsg);
			pCurStep->m_strMsg = szPrevMsg + "; " + sMsg;
		}
		else
		{
			g_pView->InsertMsg2DetailedGrid(pCurStep->m_nStep, sMsg);
			pCurStep->m_strMsg = sMsg;
		}
		bReturn = TRUE;
	}
	else
	{
		AddStringToStatus("ACAS_ID Data Error: " + strRead);
		return FALSE;
	}
	//"OK0100 4000 1088 0872 4004X"
	//for (int i = 0; i < 5; i++)
	//{

	//	if (gVF1000Ctrl.GetIRBlasterData(strRead, 1000))
	//	{
	//		lReadCount++;
	//		strIR_KeyData = strRead.Left(8);
	//		str33Data = strRead.Mid(8);
	//		str33Data.Insert(1, '.');

	//		strMsg = "IR Data: ";
	//		strMsg += strIR_KeyData;
	//		strMsg += " DC: ";
	//		strMsg += str33Data;
	//		strMsg += "V";

	//		AddStringToStatus(strMsg);

	//		Volt33Data = _ttoi(strRead.Mid(8));
	//		Volt33Data = Volt33Data / 100.0;
	//		if (strRead.Find("DAC048B7") >= 0)
	//		{


	//			strMsg.Format("DBG: L %2.2f,H %2.2f,DC %2.2f,", pCurStep->m_fLowLimit, pCurStep->m_fHighLimit, Volt33Data);
	//			AddStringToStatus(strMsg);

	//			if ((pCurStep->m_fLowLimit <= Volt33Data) && (pCurStep->m_fHighLimit >= Volt33Data))//if ((2.8 <= Volt33Data) && (3.8 >= Volt33Data))
	//			{
	//				AddStringToStatus("IR OK DC OK");
	//				TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
	//				return Volt33Data;
	//			}
	//			else
	//			{
	//				AddStringToStatus("IR OK DC NG");
	//				TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
	//				return FALSE;
	//			}
	//		}
	//		else
	//		{
	//			Sleep(150);
	//			continue;
	//		}

	//	}
	//	else
	//	{
	//		TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
	//		return FALSE;
	//	}
	//}
	////m_pVfMeasureCheckDlg.m_bTestClose = TRUE;
	////m_pVfMeasureCheckDlg.DoModal();
	////bReturn = m_pVfMeasureCheckDlg.m_bVfCheckResult;
	//TVCommCtrl.Send_TVControl_Command("av 00 26", 1000);
	return bReturn;
}




BOOL CDATsysView::ReportSend_NgCount()
{ 
	CString sTmp;
	CString sMsg;
	DWORD dwEventResult = 0;
    clock_t		start;
	MSG msg;

	sTmp = gGmesCtrl.MakeElem_S6F11_3(CurrentSet->sEquipmentID, m_strWipId);
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
			sMsg.Format(_T("GMES S6F12(NG Count Report) Fail\nTime Out..."));
			AfxMessageBox(sMsg);
		//	Gmes_Connection();
		//	CurrentSet->bIsRunMsg = FALSE; 
			return FALSE;
		}
	}
		
	return TRUE;
}


void CDATsysView::OnAcpowerGood()
{
	CurrentSet->nAcPowerOffType = 0;
}


void CDATsysView::OnAcpowerFail()
{
	CurrentSet->nAcPowerOffType = 1;
}


void CDATsysView::OnAcpowerAll()
{
	CurrentSet->nAcPowerOffType = 2;
}


void CDATsysView::OnAcpowerNot()
{
	CurrentSet->nAcPowerOffType = 3;
}

void CDATsysView::StartVFTestThread()
{
	//BOOL bFlag = FALSE;

	//g_nNoFailedStep_EM = 0;
	////ResetStep_EM();

	//if(!m_bVFTestThreadRunning)
	//{ 
	//	m_bVFTestThreadRunning = TRUE;
	//	m_pVFTestThread = AfxBeginThread(TestThread_EM,this);

	//}
}



void CDATsysView::OnAnalyseVf()
{
	BOOL bReturn;

	m_pVfMeasureCheckDlg.m_bTestClose = FALSE;

	m_pVfMeasureCheckDlg.DoModal();
	bReturn = m_pVfMeasureCheckDlg.m_bVfCheckResult;
}


void CDATsysView::OnVfControl()
{
	CVfMeasureDlg dlg;
	dlg.DoModal();	
}

//·¹Áö½ºÆ®¸®°ª ÀÐ¾î¿À±â ÇÔ¼ö
BOOL CDATsysView::ReadRegVal_Agent(char *val)
{
	HKEY phk = 0;
	long nRet;

	DWORD nEC = REG_BINARY;                                         // ¹ÙÀÌ³Ê¸®¸ðµå·Î ÀÐ±â

	DWORD size = 1;                                            // ·¹Áö½ºÆ®¸®ÀÐ±â½Ã ¹öÆÛÅ©±â

	memset(val, 0, 1);                                              //mem clear

	RegOpenKey(HKEY_CURRENT_USER, _T("Software\\LGE\\SW.Agent"), &phk);

	nRet = RegQueryValueEx(phk, "TV_DFT", NULL, &nEC, (LPBYTE)val, &size);  //ÀÐ±â °ªÀº val¿¡ ÀúÀå

	RegCloseKey(phk);

	return TRUE;
}




void CDATsysView::OnGrabresetAuto()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CurrentSet->nAuto_Grab_Reset = 1;
}


void CDATsysView::OnUpdateGrabresetAuto(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nAuto_Grab_Reset == 1);
}


void CDATsysView::OnGrabresetManual()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CurrentSet->nAuto_Grab_Reset = 0;
}


void CDATsysView::OnUpdateGrabresetManual(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nAuto_Grab_Reset == 0);
}


void CDATsysView::OnStnClickedRobotStatus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef _PLC_COM_SIM_DEBUG__MODE
	SetTimer(TIMER_AUTO_CONTROL_ID, 50, NULL);
	g_AutoControlProcess = _AUTO_ROBOT_CTRL_INIT;
#else

	if ((CurrentSet->nRunStartType == AUTO_START) && (g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
	{
		if (gPLC_Ctrl.m_bPLCCom)
		{
			if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE)
			{
				g_AutoControlProcess = _AUTO_ROBOT_CTRL_INIT;
				
			}
		}
	}
	else
	{
		g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
		
	}

#endif
}


void CDATsysView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//#ifdef _PLC_COM_SIM_DEBUG__MODE
//	SetTimer(TIMER_AUTO_CONTROL_ID, 50, NULL);
//	g_AutoControlProcess = _AUTO_ROBOT_CTRL_INIT;
//#else
//
//	if ((CurrentSet->nRunStartType == AUTO_START) && (g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == TRUE))
//	{
//		if (gPLC_Ctrl.m_bPLCCom)
//		{
//			if (g_AutoControlProcess == _AUTO_ROBOT_CTRL_IDLE)
//			{
//				g_AutoControlProcess = _AUTO_ROBOT_CTRL_INIT;
//
//			}
//		}
//	}
//	else
//	{
//		g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
//
//	}
//
//#endif
	if (g_AutoControlProcessNoTEST == 1)
	{
		g_AutoControlProcessWaitTime += 10;
		if (g_AutoControlProcessWaitTime > 100)
		{
			g_AutoControlProcessWaitTime = 10;
			g_AutoControlProcessNoTEST = 0;
			//	g_AutoControlProcessNoTEST
			ctrlPLC_COM_Status.SetWindowText(_T("TEST ON"));
		}
		else
		{
			g_AutoControlProcessNoTEST = 1;
			CString str;
			str.Format(_T("TMDLY%d"), g_AutoControlProcessWaitTime);
			ctrlPLC_COM_Status.SetWindowText(str);
		}
		
	}
	else
	{
		g_AutoControlProcessWaitTime = 10;
		
			g_AutoControlProcessNoTEST = 1;
			CString str;
			str.Format(_T("TMDLY%d"), g_AutoControlProcessWaitTime);
			ctrlPLC_COM_Status.SetWindowText(str);
		
	}
	CFormView::OnRButtonDblClk(nFlags, point);
}


void CDATsysView::OnBnClickedButtonAutoRobotReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//static int sRestButtonStatus = 0;

	//gPLC_Ctrl.m_bPLC_WaitCount = 0;
	//gPLC_Ctrl.m_bPLC_Online = 1;



	if (gPLC_Ctrl.m_bPLCCom == 0)
	{
		if (gPLC_Ctrl.SoketCom_Creat(CurrentSet->strPLC_IP, CurrentSet->nPLC_Port))
		{
			ctrlPLC_COM_Status.SetWindowText(_T("RECONNECT"));
			ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
			gPLC_Ctrl.m_bPLC_WaitCount = 0;
			gPLC_Ctrl.m_bPLC_RegWaitCount = 0;
			gPLC_Ctrl.m_bPLC_Online = 1;
		}
		else
		{
			ctrlPLC_COM_Status.SetWindowText(_T("OFFLINE"));
			ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
		}


	}

	if (CurrentSet->bUseServerCtrlConnect == 1)
	{
		if (CurrentSet->bMonitorServer_Connection != TRUE)
		{

			if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port))
			{
				CurrentSet->bMonitorServer_Connection = TRUE;
			}
			else
			{

			}
		}

		gServer_Ctrl.m_bCom_RetryCount = 0;
	}

	if (CurrentSet->nRunStartType == AUTO_START)
	{
		if ((g_AutoControlProcess >= _AUTO_ROBOT_CTRL_JIGUP) && (g_AutoControlProcess <= _AUTO_ROBOT_CTRL_TEST_NG))//
		{
		
		}
		else
		{
			gPLC_Ctrl.m_cResetControl = 1;
			ctrlPLC_COM_Status.SetWindowText(_T("RESTART"));
			ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
		//	SetDlgItemText(IDC_BUTTON_AUTO_ROBOT_RESET, "MAN");
			gPLC_Ctrl.CommandClear();
		}
	}
	else
	{	

		//if ((CurrentSet->nRunStartType == MANUAL_START)
		//	&& (g_nSysType == AUTO_SYS)
		//	&& (gPLC_Ctrl.m_bPLCCom == 1)
		//	&& (CurrentSet->bUsePLCRobot == TRUE))// (gPLC_Ctrl.m_bPLCCom
		{
			CurrentSet->nRunStartType = AUTO_START;
			g_pView->SetTimer(TIMER_AUTO_CONTROL_ID, 50, NULL);
			g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
			ctrlPLC_COM_Status.SetWindowText(_T("AUTO"));
			ctrlPLC_COM_Status.SetBkColor(RGB(255, 255, 0));
		//	SetDlgItemText(IDC_BUTTON_AUTO_ROBOT_RESET, "MAN");
			gPLC_Ctrl.CommandClear();

		}
		
	}
#ifdef _PLC_COM_SIM_DEBUG__MODE
	gPLC_Ctrl.m_bPLC_WaitCount = 0;
	gPLC_Ctrl.m_bPLC_RegWaitCount = 0;
	gPLC_Ctrl.m_bPLC_Online = 1;
	CurrentSet->nRunStartType = AUTO_START;
	g_pView->SetTimer(TIMER_AUTO_CONTROL_ID, 50, NULL);
	g_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
	return;
#endif

}


void CDATsysView::OnBnClickedButtonAutoRobotStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CurrentSet->nRunStartType == MANUAL_START)
	{
		gJigCtrl.Set_CylinderUP();
	}
	else
	{
		CurrentSet->nRunStartType = MANUAL_START;
	//	g_pView->KillTimer(TIMER_AUTO_CONTROL_ID);

		g_AutoControlProcess = _AUTO_ROBOT_CTRL_MANUAL;
		ctrlPLC_COM_Status.SetWindowText(_T("MANUAL"));
		ctrlPLC_COM_Status.SetBkColor(RGB(200, 200, 0)); //ctrlPLC_COM_Status.SetBkColor(RGB(128, 128, 128));
		gPLC_Ctrl.CommandClear();
		g_Pause_AutoControlProcess = _AUTO_ROBOT_CTRL_IDLE;
	}

//	SetDlgItemText(IDC_BUTTON_AUTO_ROBOT_RESET, "AUTO");
}


void CDATsysView::OnBnDoubleclickedButtonAutoRobotStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CurrentSet->nRunStartType == MANUAL_START)
	{
		gJigCtrl.Set_CylinderUP();
	}
}


void CDATsysView::OnStnClickedPlcComStatus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CurrentSet->nRunStartType == MANUAL_START)
	{
		gJigCtrl.Set_CylinderUP();
	}
}

long CDATsysView::OnUpdateLog(WPARAM wParam, LPARAM lParam)
{
	//static int sInt = 0;
	int len;
	char lBuffer[1024];
	CString szString;
	len = (int)wParam;
	if (len > 1023)
	{
		len = 1023;
	}
	memcpy(lBuffer, (char *)lParam, len);
	lBuffer[len] = 0;
	szString = lBuffer;
	AddStringToStatusLocal(szString);
	return 1;
}

void CDATsysView::AddStringToStatusLocal(CString szString)
{
	//CString szString;
	int lpos;
	
	SystemMonitorLog_Save(szString);

//#ifdef ERR_DEBUG
//
//	//if (g_pView->m_bWriteMassageFlag == 1)
//	//{
//	//	g_pView->m_bWriteMassageEnable = 1;
//	//}
//#endif	
//		
//	if (g_pView->m_bWriteMassageEnable == 0)
//	{
//		if (m_LogString.GetLength() > 0)
//		{
//
//			lpos = m_LogString.Find("#/");
//
//			if (lpos > 0)
//			{
//				szString = m_LogString.Left(lpos);
//
//				m_LogString = m_LogString.Mid(lpos + 2);
//			}
//			else
//			{
//				szString = m_LogString;
//
//				m_LogString = "";
//			}
//			g_pView->m_bWriteMassageEnable = 1;
//		}
//		else
//		{
//			g_pView->m_bWriteMassageEnable = 1;
//			return;
//		}
//		
//	}
//	else
//	{	
//		
//	//	g_pView->m_bWriteMassageEnable = 1;
//
//		return;
//	}
//


	szString.TrimRight(0x0a);
	szString.TrimRight(0x0d);

	g_pView->ComLog_AddString(szString);


	if ((m_I2cAdcManualCheckDlg.GetSafeHwnd() != NULL)
		&& (m_I2cAdcManualCheckDlg.m_bActive == TRUE))
	{
		m_I2cAdcManualCheckDlg.WriteLog(I2cAdcCtrl.m_szCurrentStatusMsg);
	}

	if ((m_Rs232cAdcManualCheckDlg.GetSafeHwnd() != NULL)
		&& (m_Rs232cAdcManualCheckDlg.m_bActive == TRUE))
	{
		m_Rs232cAdcManualCheckDlg.WriteLog(szString);
	}

}

void CDATsysView::OnBnClickedButtonServerConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CurrentSet->bUseServerCtrlConnect == 1)
	{
	
		if (gServer_Ctrl.SoketCom_Creat(CurrentSet->strMonitorServer_IP, CurrentSet->nMonitorServer_Port)) 
		{
		
			CurrentSet->bMonitorServer_Connection = TRUE;
		}
		else
		{

		}
	}
}


void CDATsysView::OnOptionVfoption()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BOOL bFlag;

	bFlag = g_SoundCard.wIn_Flag;
	OnAudioMeasStop();

	int lOldArcNumber = CurrentSet->nARC_Force_Port_Number;

	COptionDlg dlg("Option", NULL, 4); //COptionDlg dlg("Option");
	dlg.m_VfPageOpen = 1;
	dlg.DoModal();


	//	if (lOldArcNumber != CurrentSet->nARC_Force_Port_Number)
	{
		PostMessage(WM_COMMAND, ID_EDIT_COMPILE);
	}


	//+add PSH 080527
	if (CurrentSet->bUseScanner)
	{
		ctrlWipIdEdit.SetReadOnly(TRUE);
	}
	else
	{
		ctrlWipIdEdit.SetReadOnly(FALSE);
	}

	if (CurrentSet->nToolOptionSetMethod == 1)
	{
		//godtech 190602
		if (g_nSysRobotType == AUTO_SYS)
		{
			GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);

		}
		else
		{
			//	if (CurrentSet->bUseScanner) {
			GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(TRUE);
			//}
			//else {
			//	GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
			//}
		}
	}
	else {
		GetDlgItem(IDC_BTN_GET_OPTION)->EnableWindow(FALSE);
	}

	OnInputLock();

	//	if(bFlag) SetTimer(2, 1000, NULL);

	if ((g_nSysType == AUTO_SYS) && (CurrentSet->bUsePLCRobot == 1))
	{
		ctrlPLC_COM_Status.ShowWindow(1);
		ctrlROBOT_Status.ShowWindow(1);
		m_cBtnResetRobot.ShowWindow(1);
		m_cBtnStopRobot.ShowWindow(1);
	}
	else
	{
		ctrlPLC_COM_Status.ShowWindow(0);
		ctrlROBOT_Status.ShowWindow(0);
		m_cBtnResetRobot.ShowWindow(0);
		m_cBtnStopRobot.ShowWindow(0);
	}
}


void CDATsysView::OnNMCustomdrawListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;
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

		if (l_nSubItem == 2)
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
			else if (strCol.Find("PASS") >= 0)
			{
				crText = RGB(255, 255, 255); //글자색
				crBkgnd = RGB(0, 0, 255); //배경색으로 한다   
			}
			else if (l_nItem == m_CurrentStep - 1)
			{
				crText = RGB(0, 0, 0); //글자색
				crBkgnd = RGB(100, 255, 100); //배경색으로 한다 


			}
			else
			{
				crText = RGB(0, 0, 0); //글자색
				//if (l_nItem % 2)
				//{
				//	crBkgnd = RGB(200, 240, 160);////배경색으로 한다   
				//}
				//else
				{
					crBkgnd = RGB(255, 255, 200);
				}
			}

			m_CtrlListMainProcessEx.nSelBackColor[l_nItem][2] = crBkgnd;
			m_CtrlListMainProcessEx.nSelForeColor[l_nItem][2] = crText;
		}
		else
		{
			if (l_nItem == m_CurrentStep - 1)
			{
				crText = RGB(0, 0, 0); //글자색
				crBkgnd = RGB(100, 255, 100); //배경색으로 한다 


			}
			else
			{
				crText = RGB(0, 0, 0); //글자색
				//if (l_nItem % 2)
				//{
				//	crBkgnd = RGB(200, 240, 160);////배경색으로 한다   
				//}
				//else
				{
					crBkgnd = RGB(255, 255, 200);
				}
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


void CDATsysView::OnNMDblclkListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pNMItemActivate->iItem;
	pNMItemActivate->iSubItem;
	if (pNMItemActivate->iSubItem == 0)
	{
		if (m_LockStatus[pNMItemActivate->iItem] == 1)
		{

			m_CtrlListMainProcess.SetCheck(pNMItemActivate->iItem, 1);

		}
		else
		{

			m_CtrlListMainProcess.SetCheck(pNMItemActivate->iItem, 0);

		}
	}
	*pResult = 0;
}


void CDATsysView::OnNMClickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int l_nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;

	//NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;
	//LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	//int l_nItem;
	//int l_nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
	//int l_nSubItem = (int)(pLVCD->iSubItem);
	//l_nItem = phdr->iItem;
	
	if (nShell == 0)
	{
		int l_check;
		l_check = m_CtrlListMainProcess.GetCheck(l_nItem);

		if (m_bSeq_Lock == 1)
		{
			if (l_check)
				m_CtrlListMainProcess.SetCheck(l_nItem, 0);
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
#ifdef _LISTSCROLL_DEBUG__MODE

		//	SetScroll(l_nItem);
#endif


		}
		SetTimer(TIMER_CHECK_LIST_UPDATE, 100, NULL);
	}
	*pResult = 0;
	//OnClickMainViewGrid();
#if 1
	if (CurrentSet->bIsRunning) return;

	int nCol = nShell + 1;// m_ctrlSummaryGrid.GetCol();
	int nRow = l_nItem + 1;// m_ctrlSummaryGrid.GetRow();
	//if (nCol > m_ctrlSummaryGrid.GetCols()) return;
	//if (nRow > m_ctrlSummaryGrid.GetRows()) return;

	BOOL bCellCheck = FALSE;
	//-
	if (CurrentSet->nDisplayType == DETAILEDGRID)
	{
		//if ((nRow > 0) && (nCol == 0))
		//{
		//	if ((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
		//	{
		//		CString sTmp = _T("");
		//		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		//		AfxMessageBox(sTmp); return;
		//	}
		//	//if (CurrentSet->bGMES_Connection && CurrentSet->bUseScanner && CurrentSet->bUploadMes) {

		//	if (CurrentSet->bGMES_Connection  && CurrentSet->bUploadMes) {
		//		return;
		//	}

		//	m_ctrlSummaryGrid.SetCol(nCol);
		//	m_ctrlSummaryGrid.SetRow(nRow);
		//	if (m_ctrlSummaryGrid.GetCellChecked() == flexChecked) m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
		//	else m_ctrlSummaryGrid.SetCellChecked(flexChecked);
		//}
		//else if ((nRow == 0) && (nCol == 0))
		//{
		//	if ((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
		//	{
		//		CString sTmp = _T("");
		//		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		//		AfxMessageBox(sTmp); return;
		//	}
		//	if (CurrentSet->bGMES_Connection && CurrentSet->bUseScanner && CurrentSet->bUploadMes) {
		//		return;
		//	}

		//	m_ctrlSummaryGrid.SetRow(nRow);
		//	m_ctrlSummaryGrid.SetCol(nCol);
		//	if (m_ctrlSummaryGrid.GetCellChecked() == flexChecked) bCellCheck = TRUE;
		//	else  bCellCheck = FALSE;
		//	for (int nTmp = 0; nTmp <= StepList.GetCount(); nTmp++)
		//	{
		//		m_ctrlSummaryGrid.SetRow(nTmp);
		//		if (bCellCheck == TRUE) m_ctrlSummaryGrid.SetCellChecked(flexUnchecked);
		//		else m_ctrlSummaryGrid.SetCellChecked(flexChecked);
		//	}
		//}
		//else
		if ((nRow > 0) && (nCol > 1))
		{
			//m_ctrlSummaryGrid.SetCol(nCol);
			//m_ctrlSummaryGrid.SetRow(nRow);

			if (nRow <= StepList.GetCount())
			{
				if (m_bGrabThreadRunning)
				{
					AfxMessageBox("First, Stop LVDS Image Grabbing"); return;
				}

				POSITION PosStep = StepList.FindIndex(nRow - 1);

				pCurStep = StepList.GetAt(PosStep);

				if (pCurStep->m_strGrabPath != _T(""))
				{
					if (m_Image[I_GRAB].m_bImageLoaded)
					{
						m_Image[I_GRAB].MakeBlankImage();
					}

					if (!OpenImgFile(pCurStep->m_strGrabPath, I_GRAB))
					{
						m_Image[I_GRAB].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);
					}
				}

				if (pCurStep->m_strRefPath != _T(""))
				{
					if (m_Image[I_REF].m_bImageLoaded)
					{
						m_Image[I_REF].MakeBlankImage();
					}

					if (!OpenImgFile(pCurStep->m_strRefPath, I_REF))
					{
						m_Image[I_REF].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);
					}
				}

				if (pCurStep->m_strProcPath != _T(""))
				{
					if (m_Image[I_PROC].m_bImageLoaded)
					{
						m_Image[I_PROC].MakeBlankImage();
					}

					if (!OpenImgFile(pCurStep->m_strProcPath, I_PROC))
					{
						m_Image[I_PROC].LoadImage(m_szNoImageBmpPath, ctrlTestProgress);
					}
				}
				InvalidateRect(&m_VideoViewClientRect);
			}

		}
	}
#endif
}


void CDATsysView::OnHdnItemclickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;
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


void CDATsysView::OnNMCustomdrawListOptinProcess(NMHDR *pNMHDR, LRESULT *pResult)
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

		
		crText = RGB(0, 0, 0); //글자색
		//if (l_nItem % 2)
		//{
		//	crBkgnd = RGB(200, 240, 160);////배경색으로 한다   
		//}
		//else
		{
			crBkgnd = RGB(255, 255, 200);
		}
			
	


		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;


		*pResult = CDRF_DODEFAULT;
		return;
	}
	}

	*pResult = 0;
}

void CDATsysView::SetScroll( int lScroll)
{
	int lyPos;
	int lDiffScroll;
	
	CSize lSize;

	//NG_Display(sId); //OK_Display(sId);
#ifdef _DFT_3IN1
	lyPos = m_CtrlListMainProcess.GetScrollPos(1);
	if ((lScroll - lyPos) > 17)
	{
		if (StepList.GetCount() < (lScroll + 17))
		{
			lDiffScroll = (StepList.GetCount() - 17 - lyPos) * 18;
		}
		else
		{
			lDiffScroll = (lScroll-lyPos) * 18;
		}

		lSize.SetSize(0, lDiffScroll);
		m_CtrlListMainProcess.Scroll(lSize);
	}
	else if (((lScroll - lyPos) < 0))
	{
		lDiffScroll = (lScroll - lyPos) * 18;
		lSize.SetSize(0, lDiffScroll);
		m_CtrlListMainProcess.Scroll(lSize);
	}
#else
	lyPos = m_CtrlListMainProcess.GetScrollPos(1);
	if ((lScroll - lyPos) > 31)
	{
		if (StepList.GetCount() < (lScroll + 31))
		{
			lDiffScroll = (StepList.GetCount() - 31) * 18;
		}
		else
		{
			lDiffScroll = (lScroll) * 18;
		}
			
		lSize.SetSize(0, lDiffScroll);
		m_CtrlListMainProcess.Scroll(lSize);
	}
	else if (((lScroll - lyPos) < 0))
	{
		lDiffScroll = (lScroll) * 18;
		lSize.SetSize(0, lDiffScroll);
		m_CtrlListMainProcess.Scroll(lSize);
	}

	

#endif

}

void CDATsysView::OnNMCustomdrawListVersionProcess(NMHDR *pNMHDR, LRESULT *pResult)
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
	
		crText = RGB(0, 0, 0); //글자색
		//if (l_nItem % 2)
		//{
		//	crBkgnd = RGB(200, 240, 160);////배경색으로 한다   
		//}
		//else
		{
			crBkgnd = RGB(255, 255, 200);
		}

		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;


		*pResult = CDRF_DODEFAULT;
		return;
	}
	}

	*pResult = 0;
}


void CDATsysView::OnBnClickedButtonBatVer()
{
//#ifdef _DE 
//	gVF1000Ctrl.CheckLanc(strRead, 1000)
//#endif
#ifdef	_PLC_COM_SIM_DEBUG__MODE
	CString strRead;
	gVF1000Ctrl.CheckLanClear(strRead, 1000);
	gServer_Ctrl.ServerReportTestResult(PLC_NG_MODE);
#endif
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//GetDlgItem(IDC_EDIT_BAT_VER)->set
	if (m_BatVerReadOnly == 0)
	{
		CtrlBatVerEdit.SetReadOnly(1);
		m_BatVerReadOnly = 1;
		CtrlBatVerEdit.GetWindowText(CurrentSet->sBatVersion);
		SaveModelIniFile(CurrentSet->sModelIni);
		SetDlgItemText(IDC_BUTTON_BAT_VER, "BAT VER");
	}
	else
	{
		CtrlBatVerEdit.SetWindowText(CurrentSet->sBatVersion);
		CtrlBatVerEdit.SetReadOnly(0);
		m_BatVerReadOnly = 0;

		SetDlgItemText(IDC_BUTTON_BAT_VER, "BAT VER SAVE");
	}
}


void CDATsysView::CheckFWVer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//925MSPG-925FU	LGFU-1F
	//MSSD - 47AV	100223
	//	MSHG - 800PLUS	1.0.30
	//	MSHG - 800	211101
//	AvSwitchBoxCtrl.CheckVer();
	
	HDMIGeneratorCtrl.CheckVer();
//	PatternGeneratorCtrl.CheckVer();

}