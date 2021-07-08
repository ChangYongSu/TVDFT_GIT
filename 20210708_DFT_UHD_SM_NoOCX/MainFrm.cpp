// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
//Main Class
#include "DATsys.h"
#include "MainFrm.h"
#include "DATsysDoc.h"
#include "DATsysView.h"
//-

#include "step.h"
#include "parser.h"
#include "global.h"
#include "Splash.h"
//+add kwmoon 080514
#include "SoundCard.h"

#include "I2cAdcManualCheckDlg.h"
#include "ScannerCtrl.h"
#include "JigCtl.h""
//#include "thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//+add kwmoon 080515
extern CString g_WndClassName;

//+add kwmoon 080516
extern CSoundCard g_SoundCard;

extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_RUN_CONTINUOUS, OnRunContinuous)
	ON_COMMAND(ID_RUN_ERRORABORT, OnRunErrorabort)
	ON_UPDATE_COMMAND_UI(ID_RUN_ERRORABORT, OnUpdateRunErrorabort)
	ON_UPDATE_COMMAND_UI(ID_RUN_AUTO, OnUpdateRunAuto)
	ON_UPDATE_COMMAND_UI(ID_RUN_CONTINUOUS, OnUpdateRunContinuous)
	ON_COMMAND(ID_RUN_ERRORSTEP, OnRunErrorstep)
	ON_UPDATE_COMMAND_UI(ID_RUN_ERRORSTEP, OnUpdateRunErrorstep)
	ON_COMMAND(ID_RUN_STEP, OnRunStep)
	ON_UPDATE_COMMAND_UI(ID_RUN_STEP, OnUpdateRunStep)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
	ON_UPDATE_COMMAND_UI(ID_APP_EXIT, OnUpdateAppExit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COMPILE, OnUpdateEditCompile)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CURRENT, OnUpdateEditCurrent)
	ON_UPDATE_COMMAND_UI(ID_MODEL_OPEN, OnUpdateModelOpen)
	ON_UPDATE_COMMAND_UI(ID_RUN_RUN, OnUpdateRunRun)
	ON_UPDATE_COMMAND_UI(ID_RUN_NEXT, OnUpdateRunNext)
	ON_UPDATE_COMMAND_UI(ID_RUN_PREV, OnUpdateRunPrev)
	ON_UPDATE_COMMAND_UI(ID_RUN_JUMP, OnUpdateRunJump)
	ON_UPDATE_COMMAND_UI(ID_RUN_ABORT, OnUpdateRunAbort)
	ON_WM_CLOSE()
	ON_COMMAND(ID_SPEAKER_CONTROL, OnSpeakerControl)
	ON_UPDATE_COMMAND_UI(ID_AUDIO_MEAS_START, OnUpdateAudioMeasStart)
	ON_UPDATE_COMMAND_UI(ID_AUDIO_MEAS_STOP, OnUpdateAudioMeasStop)
	ON_COMMAND(ID_PRINT_ALLRESULT, OnPrintAllresult)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ALLRESULT, OnUpdatePrintAllresult)
	ON_COMMAND(ID_PRINT_ERROR, OnPrintError)
	ON_UPDATE_COMMAND_UI(ID_PRINT_ERROR, OnUpdatePrintError)
	ON_COMMAND(ID_PRINT_NONE, OnPrintNone)
	ON_UPDATE_COMMAND_UI(ID_PRINT_NONE, OnUpdatePrintNone)
	ON_UPDATE_COMMAND_UI(ID_RUN_MAKE_REF, OnUpdateRunMakeRef)
	ON_COMMAND(ID_I2C_MANUAL_CHECK_DIALOG, OnI2cManualCheckDialog)
	ON_COMMAND(ID_RS232C_MANUAL_CHECK_DIALOG, OnRs232cManualCheckDialog)
	ON_UPDATE_COMMAND_UI(ID_SEQ_EDIT, OnUpdateSeqEdit)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_DIALOG, OnUpdateImageDialog)
	ON_UPDATE_COMMAND_UI(ID_OPTION, OnUpdateOption)
	ON_UPDATE_COMMAND_UI(ID_GRAB_START, OnUpdateGrabStart)
	ON_UPDATE_COMMAND_UI(ID_GRAB_STOP, OnUpdateGrabStop)
	ON_UPDATE_COMMAND_UI(ID_ENTER, OnUpdateEnter)
	ON_UPDATE_COMMAND_UI(ID_EXIT, OnUpdateExit)
	ON_UPDATE_COMMAND_UI(ID_REF_IMAGE, OnUpdateRefImage)
	ON_UPDATE_COMMAND_UI(ID_SCANNER_ENABLE, OnUpdateScannerEnable)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_JIGDOWN_GOOD, &CMainFrame::OnUpdateJigdownGood)
	ON_UPDATE_COMMAND_UI(ID_JIGDOWN_ALL, &CMainFrame::OnUpdateJigdownAll)
	ON_UPDATE_COMMAND_UI(ID_JIGDOWN_NOT, &CMainFrame::OnUpdateJigdownNot)
	ON_UPDATE_COMMAND_UI(ID_ACPOWER_FAIL, &CMainFrame::OnUpdateAcpowerFail)
	ON_UPDATE_COMMAND_UI(ID_ACPOWER_GOOD, &CMainFrame::OnUpdateAcpowerGood)
	ON_UPDATE_COMMAND_UI(ID_ACPOWER_ALL, &CMainFrame::OnUpdateAcpowerAll)
	ON_UPDATE_COMMAND_UI(ID_ACPOWER_NOT, &CMainFrame::OnUpdateAcpowerNot)
	ON_UPDATE_COMMAND_UI(ID_ANALYSE_VF, &CMainFrame::OnUpdateAnalyseVf)
	ON_COMMAND(ID_START_AUTO, &CMainFrame::OnStartAuto)
	ON_UPDATE_COMMAND_UI(ID_START_AUTO, &CMainFrame::OnUpdateStartAuto)
	ON_COMMAND(ID_START_MANUAL, &CMainFrame::OnStartManual)
	ON_UPDATE_COMMAND_UI(ID_START_MANUAL, &CMainFrame::OnUpdateStartManual)
	ON_UPDATE_COMMAND_UI(ID_VF_CONTROL, &CMainFrame::OnUpdateVfControl)
	ON_UPDATE_COMMAND_UI(ID_OPTION_VFOPTION, &CMainFrame::OnUpdateOptionVfoption)
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_MYWORD,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
};



/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_bAutoMenuEnable = FALSE;	

	m_newToolBar.SetBorders(1, 1, 1, 1);

   DWORD dwToolbarStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;

#ifdef _DFT_3IN1
	if (!m_newToolBar.Create(this, dwToolbarStyle) ||
		!m_newToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_newToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_newToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH_3IN1,
									   IDB_TOOLBAR_DRAW,
		                               IDB_TOOLBAR_DRAW,
									   IDB_TOOLBAR_DRAW_DISABLED);

#endif
#ifdef _DFT_2IN1
	if (!m_newToolBar.Create(this, dwToolbarStyle) ||
		!m_newToolBar.LoadToolBar(IDR_MAINFRAME_2IN1))
	{
		TRACE0("Failed to create toolbar\n");
		AfxMessageBox("Failed to create toolbar bar");
		return -1;      // fail to create
	}
	m_newToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_newToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH_2IN1,
									   IDB_TOOLBAR_DRAW_2IN1,
		                               IDB_TOOLBAR_DRAW_2IN1,
									   IDB_TOOLBAR_DRAW_DISABLED_2IN1);
	m_newToolBar.SetButtonText(0,"MODEL");
	m_newToolBar.SetButtonText(1,"SYNTAX");
	m_newToolBar.SetButtonText(2,"RUN");
	m_newToolBar.SetButtonText(3,"STOP");
	m_newToolBar.SetButtonText(5,"SCAN");
//	m_newToolBar.SetButtonText(6,"SCAN(X)");
	m_newToolBar.SetButtonText(6,"GRAB");
	m_newToolBar.SetButtonText(8,"IMAGE");
	m_newToolBar.SetButtonText(9,"EDIT");
	m_newToolBar.SetButtonText(10,"OPTION");
	m_newToolBar.SetButtonText(11,"ROT-IMG");

	m_newToolBar.SetButtonText(13,"POWER");
	m_newToolBar.SetButtonText(14,"ESC");
	m_newToolBar.SetButtonText(15,"ENTER");
	//+add kwmoon 080819
	m_newToolBar.SetButtonText(17, "REF-IMG");
	m_newToolBar.SetButtonText(18, "REF-VF");
	m_newToolBar.SetButtonText(19,"EXIT");


	m_newToolBar.SetSizes(CSize(64,54), CSize(32,32));
#endif
//	if (!m_wndStatusBar.Create(this) ||
//		!m_wndStatusBar.SetIndicators(indicators, 3 /*sizeof(indicators)/sizeof(UINT)*/))

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		AfxMessageBox("Failed to create status bar");
		return -1;      // fail to create
	}
//	if (!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
//	{
//		TRACE0("Failed to create status bar\n");
//		AfxMessageBox("Failed to SetIndicators status bar");
//		return -1;      // fail to create
//	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 200);
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_MYWORD, SBPS_NORMAL, 400);

	m_newToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_newToolBar);
//		AfxMessageBox("CMainFrame::OnCreate OK");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//+change kwmoon 080515
//	cs.lpszClass = _T("DFTsysClass");
	cs.lpszClass = g_WndClassName;

//	cs.style = WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	cs.style &= ~FWS_ADDTOTITLE;
//	cs.style &= WS_THICKFRAME;

 // Create a window without min/max buttons or sizable border 
    //cs.style = WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_MAXIMIZE;

	//+add 20110531 : [3in1]
	if(g_nRunningProcessNo == 1)
	{
		cs.x = 0;
		cs.y = 0; 
	}
	else if(g_nRunningProcessNo == 2)
	{
#ifdef _DFT_3IN1
		cs.x = 640;
		cs.y = 0; 
#endif

#ifdef _DFT_2IN1
		cs.x = 960;
		cs.y = 0; 
#endif

	}
	else
	{
		cs.x = 1280;
		cs.y = 0; 
	}

#ifdef _DFT_3IN1
	cs.cx = 640; 
#endif

#ifdef _DFT_2IN1
	cs.cx = 960; 
#endif

	cs.cy = 1080; 
/*
	if(g_nRunningProcessNo == 1)
	{
		cs.x = 0;
		cs.y = 0; 
	}
	else
	{
		cs.x = 960;
		cs.y = 0; 
	}

	cs.cx = 960; 
	cs.cy = 1080; 
*/

    //cs.y = ((cs.cy * 3) - cs.cy) / 2; 
    //cs.x = ((cs.cx * 3) - cs.cx) / 2;

    // Call the base-class version
    return CFrameWnd::PreCreateWindow(cs); 

//eturn TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateAppExit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateEditCompile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateEditCurrent(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateModelOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateRunAuto(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nRunType == AUTO);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnRunContinuous() 
{
	CurrentSet->nRunType = CONT;
	sMode = _T("AUTO");
	CDATsysView* pView = (CDATsysView*)GetActiveView();
	pView->UpdateInfo(MODE, sMode);	
}

void CMainFrame::OnUpdateRunContinuous(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nRunType == CONT);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnRunErrorabort() 
{
	CurrentSet->nRunType = STOP;
	sMode = _T("STOP(NG)");
	CDATsysView* pView = (CDATsysView*)GetActiveView();
	pView->UpdateInfo(MODE, sMode);
}

void CMainFrame::OnUpdateRunErrorabort(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nRunType == STOP);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}


void CMainFrame::OnRunErrorstep() 
{
	CurrentSet->nRunType = ERRORSTEP;
	sMode = _T("STEP(NG)");
	CDATsysView* pView = (CDATsysView*)GetActiveView();
	pView->UpdateInfo(MODE, sMode);
}

void CMainFrame::OnUpdateRunErrorstep(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nRunType == ERRORSTEP);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnRunStep() 
{
	CurrentSet->nRunType = STEP;
	sMode = _T("STEP");
	CDATsysView* pView = (CDATsysView*)GetActiveView();
	pView->UpdateInfo(MODE, sMode);
}

void CMainFrame::OnUpdateRunStep(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nRunType == STEP);
	pCmdUI->Enable(!CurrentSet->bIsRunning);	
}

void CMainFrame::OnUpdateRunRun(CCmdUI* pCmdUI) 
{
//	int index;
//	CString text;
	
	//+change PSH 080904 
	if((!CurrentSet->sSeqPath.IsEmpty() && !CurrentSet->bIsRunning && !CurrentSet->bUseScanner) 
		&& (!CurrentSet->nRunStartType || (g_nSysType != AUTO_SYS)))
	{
		pCmdUI->Enable(1);
	}
	else
	{
		pCmdUI->Enable(0);
	}
	
/*	index=m_wndStatusBar.CommandToIndex(ID_INDICATOR_MYWORD);

	if(CurrentSet->bIsRunning)
	{
		if(g_pView->m_bMakeReferenceMode) 
		{
			text=_T("   Running : REF-IMG"); 
		}
		else
		{
			text=_T("   Running : TESTING"); 
		}
	}
	else
	{
		if(g_nRunningProcessNo ==1){
			text.LoadString(ID_INDICATOR_MYWORD);
		}
		else if(g_nRunningProcessNo ==2){
			text.LoadString(ID_INDICATOR_MYWORD2);
		}
		else {
			text.LoadString(ID_INDICATOR_MYWORD3);
		}
	}

	m_wndStatusBar.SetPaneText(index,text);
*/
}

void CMainFrame::SetStatusBarText(CString sText)
{
	int index;

	index=m_wndStatusBar.CommandToIndex(ID_INDICATOR_MYWORD);

	m_wndStatusBar.SetPaneText(index,sText);
}

void CMainFrame::OnUpdateRunNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CurrentSet->nRunType == STEP && CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateRunPrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CurrentSet->nRunType == STEP && CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateRunJump(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CurrentSet->nRunType == STEP && CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateRunAbort(CCmdUI* pCmdUI) 
{
	//+del kwmoon 080527
	pCmdUI->Enable(CurrentSet->bIsRunning);	
}

void CMainFrame::OnClose() 
{

/*	g_pView->KillTimer(TIMER_MEASURE_AUDIO_OUTPUT);

//	UserControlInit(FALSE);

	if(g_pView->m_bResultDlgActivate)
	{
		delete g_pView->m_ResultDlg;
	}

	//+add 090131(수정항목1)
	if(CurrentSet->bUseScanner)
	{
		ScannerCtrl.PortClose(); Sleep(500);
	}
	

	// Close Sound Driver
	//090615
	g_pView->AudioMeasureStop();
	Sleep(500);

	g_pView->StopLVDSGrabThread();
	Sleep(2500);

	//+add 090213(Modification No1)
	DWORD dwExitCode = 0;
	if(g_pView->m_pGrabImageThread) TerminateThread(g_pView->m_pGrabImageThread->m_hThread,dwExitCode);
	if(g_pView->m_pTestThread)	   TerminateThread(g_pView->m_pTestThread->m_hThread,dwExitCode);
	
*/
//	g_pView->AudioMeasureStop();

//	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
//	AfxGetMainWnd()->PostMessage(WM_QUIT, 0, 0);
	CoFreeUnusedLibraries();

	CFrameWnd::OnClose();
}

void CMainFrame::OnSpeakerControl() 
{
	
	PROCESS_INFORMATION pInfo;
	STARTUPINFO         sInfo;
	char				achCommand[128];
	CWnd				*pWndPrev, *pWndChild;
  
	// Determine if another window with our class name exists...
	if (pWndPrev = FindWindow(NULL, _T("볼륨 조절")))
	{
		pWndChild = pWndPrev->GetLastActivePopup(); 

		// If iconic, restore the main window
		if (pWndPrev->IsIconic()) 
		   pWndPrev->ShowWindow(SW_RESTORE);

		// Bring the main window or its popup to
		// the foreground
	//	pWndChild->SetForegroundWindow();         
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
	strcpy_s(achCommand, _countof(achCommand),  _T("C:\\WINDOWS\\SNDVOL32.EXE"));

	if (!CreateProcess(NULL			// address of module name
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
		CString s;
		s.LoadString(IDS_ERROR_CHILDPROCESS);
		OkMessage(s);
	}

}

void CMainFrame::OnUpdateAudioMeasStart(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(!CurrentSet->bIsRunning && (g_SoundCard.wIn_Flag  == FALSE));
}

void CMainFrame::OnUpdateAudioMeasStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning && (g_SoundCard.wIn_Flag  == TRUE));
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case WM_MOVE:
		break;
	case WM_SIZE:
		break;
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}


void CMainFrame::OnPrintAllresult() 
{
	CurrentSet->nPrintType = ALL;
}

void CMainFrame::OnUpdatePrintAllresult(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nPrintType == ALL);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnPrintError() 
{
	CurrentSet->nPrintType = ERRORVIEW;
}

void CMainFrame::OnUpdatePrintError(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nPrintType == ERRORVIEW);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}

void CMainFrame::OnPrintNone() 
{
	CurrentSet->nPrintType = NONE;
}

void CMainFrame::OnUpdatePrintNone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(CurrentSet->nPrintType == NONE);
	pCmdUI->Enable(!CurrentSet->bIsRunning);	
}


void CMainFrame::SetTitle(CString sModelFolder)
{
	CDATsysView* pView = (CDATsysView*)GetActiveView();

	CString sTmp = _T("");
	CString sTitle = _T("");
	sTmp.LoadString(IDR_MAINFRAME);

	sTitle.Format("%s%d-%s", sTmp, g_nRunningProcessNo, sModelFolder);

	if(g_nSysType == AUTO_SYS){
		if(CurrentSet->nRunStartType == AUTO_START){
			sTitle = sTitle + ": Auto Start";
//			g_pView->SetTimer(TIMER_AUTO_CONTROL_ID, 50, NULL);
		}
		else{
			sTitle = sTitle + ": Manual Start";
//			g_pView->KillTimer(TIMER_AUTO_CONTROL_ID);
		}
	}
	SetWindowText(sTitle);

/*
	if(g_nRunningProcessNo == 1){
		sTmp = sTmp + "1-" + sModelFolder;
	}
	else if(g_nRunningProcessNo == 2) {
		sTmp = sTmp + "2-" + sModelFolder;
	}
	else { // (g_nRunningProcessNo == 3)
		sTmp = sTmp + "3-" + sModelFolder;
	}
	SetWindowText(sTmp);
*/
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	
	return CFrameWnd::OnCreateClient(lpcs, pContext);

}
void CMainFrame::OnUpdateRunMakeRef(CCmdUI* pCmdUI) 
{
	if (g_AutoControlProcess != _AUTO_ROBOT_CTRL_MANUAL)
	{
		pCmdUI->Enable(0);
	}
	else
	{
		pCmdUI->Enable(!CurrentSet->bIsRunning);
	}
}


void CMainFrame::OnI2cManualCheckDialog() 
{
	// TODO: Add your command handler code here
	CDATsysView* pView = (CDATsysView*)GetActiveView();

	if((CurrentSet->bIsRunning == TRUE) || (pView->m_bThreadRunning == TRUE))
	{
		AfxMessageBox("Test is running! First, Stop current test.");
	}
	else
	{
		
		pView->m_I2cAdcManualCheckDlg.DoModal();

	}
}

//+add kwmoon 080904
void CMainFrame::OnRs232cManualCheckDialog() 
{
	// TODO: Add your command handler code here
	CDATsysView* pView = (CDATsysView*)GetActiveView();

	if((CurrentSet->bIsRunning == TRUE) || (pView->m_bThreadRunning == TRUE))
	{
		AfxMessageBox("Test is running! First, Stop current test.");
	}
	else
	{

		pView->m_Rs232cAdcManualCheckDlg.DoModal();

	}
}
//+add PSH 080904

void CMainFrame::OnUpdateSeqEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	
}

void CMainFrame::OnUpdateImageDialog(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	
}

void CMainFrame::OnUpdateOption(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	
}
//-

void CMainFrame::OnUpdateGrabStart(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(g_pView->m_bGrabThreadRunning);
	
}

void CMainFrame::OnUpdateGrabStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(!g_pView->m_bGrabThreadRunning);
	
}

void CMainFrame::OnUpdateEnter(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(!CurrentSet->bIsRunning);
	
}

void CMainFrame::OnUpdateExit(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(!CurrentSet->bIsRunning);
	
}

void CMainFrame::OnUpdateRefImage(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	
}

void CMainFrame::OnUpdateScannerEnable(CCmdUI* pCmdUI) 
{
	if (g_nSysType == AUTO_SYS)
	{
		if (g_nUseNoScanType == TRUE)
		{
			pCmdUI->Enable(0);
			pCmdUI->SetCheck(0);
		}
		else
		{
			pCmdUI->Enable(!CurrentSet->bIsRunning);
			pCmdUI->SetCheck(1);
		}

	}	
	else
	{
		if (g_nUseNoScanType == TRUE)
		{
			pCmdUI->Enable(0);
			pCmdUI->SetCheck(0);
		}
		else
		{
			pCmdUI->Enable(!CurrentSet->bIsRunning);
			pCmdUI->SetCheck(CurrentSet->bUseScanner);


		}
		//	pCmdUI->Enable(!CurrentSet->bIsRunning && !CurrentSet->bUseScanner);
	}
}

void CMainFrame::OnUpdateJigdownGood(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((g_nSysType == AUTO_SYS) && (!CurrentSet->bIsRunning));
	pCmdUI->SetCheck(CurrentSet->nJigUpType == 0);
}

void CMainFrame::OnUpdateJigdownAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((g_nSysType == AUTO_SYS) && (!CurrentSet->bIsRunning));
	pCmdUI->SetCheck(CurrentSet->nJigUpType == 1);
}


void CMainFrame::OnUpdateJigdownNot(CCmdUI *pCmdUI)
{
	pCmdUI->Enable((g_nSysType == AUTO_SYS) && (!CurrentSet->bIsRunning));
	pCmdUI->SetCheck(CurrentSet->nJigUpType == 2);
}

void CMainFrame::OnUpdateAcpowerGood(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nAcPowerOffType == 0);
}


void CMainFrame::OnUpdateAcpowerFail(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nAcPowerOffType == 1);
}

void CMainFrame::OnUpdateAcpowerAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nAcPowerOffType == 2);
}

void CMainFrame::OnUpdateAcpowerNot(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nAcPowerOffType == 3);
}


void CMainFrame::OnUpdateAnalyseVf(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}


void CMainFrame::OnStartAuto()
{
	CString sTemp;

	CurrentSet->nRunStartType = AUTO_START;
	SetTitle(g_pView->m_szVersion);
	g_pView->SetTimer(TIMER_AUTO_CONTROL_ID, 50, NULL);
}


void CMainFrame::OnUpdateStartAuto(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(CurrentSet->nRunStartType == AUTO_START);
	pCmdUI->Enable((g_nSysType == AUTO_SYS) && !CurrentSet->bIsRunning);
}


void CMainFrame::OnStartManual()
{
	CString sTemp;

	CurrentSet->nRunStartType = MANUAL_START;
	
	SetTitle(g_pView->m_szVersion);
	g_pView->KillTimer(TIMER_AUTO_CONTROL_ID);
}


void CMainFrame::OnUpdateStartManual(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(CurrentSet->nRunStartType == MANUAL_START);
	pCmdUI->Enable((g_nSysType == AUTO_SYS) && !CurrentSet->bIsRunning);
}


void CMainFrame::OnUpdateVfControl(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}


void CMainFrame::OnUpdateOptionVfoption(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}
