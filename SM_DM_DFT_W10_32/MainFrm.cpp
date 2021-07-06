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

//#include "I2cAdcManualCheckDlg.h"
#include "ScannerCtrl.h"
//#include "thread.h"
#include "PciDio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//+add kwmoon 071015 : [2in1]
extern int g_nRunningProcessNo;

//+add kwmoon 080515
extern CString g_WndClassName;

//+add kwmoon 080516
//extern CSoundCard g_SoundCard;

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
	ON_UPDATE_COMMAND_UI(ID_RUN_ADJ_SPEC, OnUpdateRunAdjSpec)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_DIALOG, OnUpdateImageDialog)
	ON_UPDATE_COMMAND_UI(ID_OPTION, OnUpdateOption)
	ON_UPDATE_COMMAND_UI(ID_GRAB_START, OnUpdateGrabStart)
	ON_UPDATE_COMMAND_UI(ID_GRAB_STOP, OnUpdateGrabStop)
	ON_UPDATE_COMMAND_UI(ID_ENTER, OnUpdateEnter)
	ON_UPDATE_COMMAND_UI(ID_EXIT, OnUpdateExit)
	ON_UPDATE_COMMAND_UI(ID_REF_IMAGE, OnUpdateRefImage)
	ON_UPDATE_COMMAND_UI(ID_REF_AUDIO, OnUpdateRefAudio)
	ON_UPDATE_COMMAND_UI(ID_SCANNER_DISABLE, OnUpdateScannerDisable)
	ON_UPDATE_COMMAND_UI(ID_SCANNER_ENABLE, OnUpdateScannerEnable)
	ON_WM_ACTIVATE()
	ON_UPDATE_COMMAND_UI(ID_JIG_UP, OnUpdateJigUp)
	ON_UPDATE_COMMAND_UI(ID_JIGUP_ALL, OnUpdateJigupAll)
	ON_UPDATE_COMMAND_UI(ID_JIGUP_GOOD, OnUpdateJigupGood)
	ON_UPDATE_COMMAND_UI(ID_JIGUP_NOT, OnUpdateJigupNot)
	ON_UPDATE_COMMAND_UI(ID_START_AUTO, OnUpdateStartAuto)
	ON_UPDATE_COMMAND_UI(ID_START_MANUAL, OnUpdateStartManual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_MYWORD,
	ID_INDICATOR_CAPS
//	ID_INDICATOR_NUM,
//	ID_INDICATOR_SCRL,
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

/*	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
*/
	m_newToolBar.SetBorders(1, 1, 1, 1);
   DWORD dwToolbarStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP | CCS_ADJUSTABLE | 
						   CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
//	if (!m_newToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP
//		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||

	if (!m_newToolBar.Create(this, dwToolbarStyle) ||
		!m_newToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_newToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_newToolBar.LoadTrueColorToolBar(TOOLBAR_DRAW_BUTTON_WIDTH,
									   IDB_TOOLBAR_DRAW,
									IDB_TOOLBAR_DRAW, //IDB_TOOLBAR_DRAW_PRESS,
									   IDB_TOOLBAR_DRAW_DISABLED);
	m_newToolBar.SetButtonText(0,"MODEL");
	m_newToolBar.SetButtonText(1,"SYNTAX");
	m_newToolBar.SetButtonText(2,"RUN");
	m_newToolBar.SetButtonText(3,"STOP");

	m_newToolBar.SetButtonText(5,"SCAN");
	m_newToolBar.SetButtonText(6,"GRAB");

	m_newToolBar.SetButtonText(8,"IMAGE");
	m_newToolBar.SetButtonText(9,"EDIT");
	m_newToolBar.SetButtonText(10,"OPTION");
	
//	m_newToolBar.SetButtonText(11,"ROT-IMG");

	m_newToolBar.SetButtonText(12,"POWER");
	m_newToolBar.SetButtonText(13,"INPUT");
	m_newToolBar.SetButtonText(14,"HOME");
	//+add kwmoon 080819
	m_newToolBar.SetButtonText(16,"REF-IMG");
	m_newToolBar.SetButtonText(18,"EXIT");


	m_newToolBar.SetSizes(CSize(64,54), CSize(32,32));


	//090324
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NOBORDERS, 200);
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_MYWORD, SBPS_NORMAL, 400);

/*
	//+del kwmoon 070109 : TrueColor ToolBar
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	FrameEnableDocking(this,CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
*/
	
	m_newToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_newToolBar);
	//-

	// CG: The following line was added by the Splash Screen component.
//	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//+change kwmoon 080515
//	cs.lpszClass = _T("DFTsysClass");
	cs.lpszClass = g_WndClassName;

	cs.style &= ~FWS_ADDTOTITLE;
//	cs.style &= ~WS_THICKFRAME;

    // Create a window without min/max buttons or sizable border 
    //cs.style = WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_MAXIMIZE;

	//+add kwmoon 071015 : [2in1]
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

	CString text;

	//+change PSH 080904 
//	pCmdUI->Enable(!CurrentSet->sSeqPath.IsEmpty() && !CurrentSet->bIsRunning && !CurrentSet->bUseScanner);
	pCmdUI->Enable(!CurrentSet->sSeqPath.IsEmpty() && !CurrentSet->bIsRunning);
	
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

/*	// Close Sound Driver
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
	if (pWndPrev = FindWindow(NULL, _T("º¼·ý Á¶Àý")))
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
	strcpy(achCommand, _T("C:\\WINDOWS\\SNDVOL32.EXE"));

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
	//+change psh 080904
	//pCmdUI->Enable(!CurrentSet->bIsRunning && (g_pView->wIn_Flag  == FALSE));
	//pCmdUI->Enable(!CurrentSet->bIsRunning && (gP_View->m_SignalEnable  == FALSE));
}

void CMainFrame::OnUpdateAudioMeasStop(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(!CurrentSet->bIsRunning && (g_pView->wIn_Flag   == TRUE));
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
	sTmp.LoadString(IDR_MAINFRAME);

	if(g_nRunningProcessNo == 1)
	{
		sTmp = sTmp + "1-" + sModelFolder;
	}
	else // (g_nRunningProcessNo == 2)
	{
		sTmp = sTmp + "2-" + sModelFolder;
	}
	SetWindowText(sTmp);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
/*
	DWORD dwStyle = 1342210626;
	DWORD dwOffsetExt = 0;
	DWORD dwOffsetInt = 0;

	if(!m_TabViewContainer.Create(this,CRect(0,0,0,0),dwStyle)) return FALSE;

	m_TabViewContainer.SetOffsetExternal(dwOffsetExt);
	m_TabViewContainer.SetOffsetInternal(dwOffsetInt);

	if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CEditView),
		pContext,_T("Grab"),0))
		return FALSE;
	if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CEditView),
		pContext,_T("Reference"),1))
		return FALSE;

	if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CEditView),
		pContext,_T("Processed"),2))
		return FALSE;

	m_TabViewContainer.SetActivePageIndex(0);

	//+add kwmoon 070829
	m_TabViewContainer.SetWindowPos(&wndTop, 50,50  ,  800, 500,SWP_SHOWWINDOW);
	m_TabViewContainer.ShowWindow(SW_SHOW);
	m_TabViewContainer.UpdateWindow();
*/	
	return CFrameWnd::OnCreateClient(lpcs, pContext);

/*
	//+add kwmoon 070829	

	DWORD dwStyle = 1342210626;
	DWORD dwOffsetExt = 0;
	DWORD dwOffsetInt = 0;

	if(!m_TabViewContainer.Create(this,CRect(100,100,100,100),dwStyle)) return FALSE;

	m_TabViewContainer.SetOffsetExternal(dwOffsetExt);
	m_TabViewContainer.SetOffsetInternal(dwOffsetInt);

	//+add kwmoon 070829

	// edit control
	if(!m_GrabViewWnd.Create(WS_CHILD|WS_HSCROLL|WS_VSCROLL,
    CRect(0,0,0,0),&m_TabViewContainer,0))

	{
		return -1;
	}

	if(!m_RefViewWnd.Create(WS_CHILD|WS_HSCROLL|WS_VSCROLL,
    CRect(0,0,0,0),&m_TabViewContainer,1))

	{
		return -1;
	}

	if(!m_ProcessedViewWnd.Create(WS_CHILD|WS_HSCROLL|WS_VSCROLL,
    CRect(0,0,0,0),&m_TabViewContainer,2))

	{
		return -1;
	}

	if(!m_TabViewContainer.AddPage(&m_GrabViewWnd,_T("Grab"),0))
		return FALSE;
	if(!m_TabViewContainer.AddPage(&m_RefViewWnd,_T("Reference"),1))
		return FALSE;
	if(!m_TabViewContainer.AddPage(&m_ProcessedViewWnd,_T("Processed"),2))
		return FALSE;

	m_TabViewContainer.SetActivePageIndex(0);
	m_TabViewContainer.ShowWindow(SW_SHOW);
	/////
*/
}
void CMainFrame::OnUpdateRunMakeRef(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
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
		//+add kwmoon 080910
//		CurrentSet->bIsRunning = TRUE;
//		pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);
		
//		pView->m_I2cAdcManualCheckDlg.DoModal();

		//+add kwmoon 080910
//		pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
//		CurrentSet->bIsRunning = FALSE;
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
		//+add kwmoon 080910
//		CurrentSet->bIsRunning = TRUE;
//		pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,START_CONT_GRAB,0);

		pView->m_Rs232cAdcManualCheckDlg.DoModal();

		//+add kwmoon 080910
//		pView->SendMessage(UM_CONTINUOUS_LVDS_GRAB,STOP_CONT_GRAB,0);
//		CurrentSet->bIsRunning = FALSE;
	}
}
//+add PSH 080904

void CMainFrame::OnUpdateSeqEdit(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	
}

void CMainFrame::OnUpdateRunAdjSpec(CCmdUI* pCmdUI) 
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
//	pCmdUI->Enable(!CurrentSet->bIsRunning && !g_pView->m_bGrabThreadRunning);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(g_pView->m_bGrabThreadRunning);
	
}

void CMainFrame::OnUpdateGrabStop(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(!CurrentSet->bIsRunning && g_pView->m_bGrabThreadRunning);
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

void CMainFrame::OnUpdateRefAudio(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);

}

void CMainFrame::OnUpdateScannerDisable(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(!CurrentSet->bIsRunning && CurrentSet->bUseScanner);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(!CurrentSet->bUseScanner);
}

void CMainFrame::OnUpdateScannerEnable(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(!CurrentSet->bIsRunning && !CurrentSet->bUseScanner);
	pCmdUI->Enable(!CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->bUseScanner);
}

/*
void CMainFrame::OnUpdateUsbdioCheckDialog(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!CurrentSet->bIsRunning);
}
*/
void CMainFrame::OnUpdateJigUp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gPciDioCtrl.m_bPCIDIO && !CurrentSet->bIsRunning);
}

void CMainFrame::OnUpdateJigupAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gPciDioCtrl.m_bPCIDIO && !CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nJigUpType == 1);
}

void CMainFrame::OnUpdateJigupGood(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gPciDioCtrl.m_bPCIDIO && !CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nJigUpType == 0);	
}

void CMainFrame::OnUpdateJigupNot(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gPciDioCtrl.m_bPCIDIO && !CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nJigUpType == 2);
}

void CMainFrame::OnUpdateStartAuto(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gPciDioCtrl.m_bPCIDIO && !CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nRunStartType == AUTO_START);
}

void CMainFrame::OnUpdateStartManual(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(gPciDioCtrl.m_bPCIDIO && !CurrentSet->bIsRunning);
	pCmdUI->SetCheck(CurrentSet->nRunStartType == MANUAL_START);
}
