// DATsys.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DATsys.h"
#include "MainFrm.h"
#include "DATsysDoc.h"
#include "DATsysView.h"
#include "global.h"
#include "Splash.h"

//======================
// MMTools2 header file
//======================
#include "Register.h"

#include "Defines.h"
#include "Aclapi.h"
#include "Global.h"
#include "SoundCard.h"

//==================
// Get Process Info
//==================
#include "tlhelp32.h"
//#include "PsApi\\psapi.h" 
 
//{{AFX_INCLUDES()
#include "labelcontrol.h"
//}}AFX_INCLUDES
//-

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===================
// External Variable
//===================
static BOOL bClassRegistered = FALSE;

//extern CSoundCard	g_SoundCard;
extern CString		g_WndClassName;
extern int			g_nRunningProcessNo;
extern CDATsysView* g_pView;

//+add 090213(Modification No1)
//CStdioFile			g_LogFile;

/////////////////////////////////////////////////////////////////////////////
// CDATsysApp

BEGIN_MESSAGE_MAP(CDATsysApp, CWinApp)
	//{{AFX_MSG_MAP(CDATsysApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDATsysApp construction

CDATsysApp::CDATsysApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDATsysApp object

CDATsysApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDATsysApp initialization

BOOL CDATsysApp::InitInstance()
{
// TODO: richedit2 라이브러리를 초기화하려면 AfxInitRichEdit2()을(를) 호출합니다.\n"	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
 //   

// Determine whether to use DAO 3.6, 3.5, or 3.0
// Use DAO 3.0 if DLL build and not built with MFC 4.21 or later
// Use DAO 3.6 if MFC 6.01 or later
// otherwise, DAO 3.5

//#undef _MFC_VER
//#define _MFC_VER 0x0601
	if (!AfxSocketInit())
    {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE ;
    }

	AfxInitRichEdit2();

#ifdef _DFT_SYSTEM1

	g_nRunningProcessNo = 1;
	g_WndClassName		= _T("DFT1Class");

	if (!Check_Checksum())
	{
		return FALSE;
	}

#endif

#ifdef _DFT_SYSTEM2

	g_nRunningProcessNo = 2;
	g_WndClassName		= _T("DFT2Class");

#endif

    // Register our unique class name that we wish to use
    WNDCLASS wndcls;

    memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

    wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wndcls.lpfnWndProc = ::DefWindowProc;
    wndcls.hInstance = AfxGetInstanceHandle();
    wndcls.hIcon = LoadIcon(IDR_MAINFRAME); // or load a different icon
    wndcls.hCursor = LoadCursor(IDC_ARROW);
    wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndcls.lpszMenuName = NULL;

    // Specify our own class name for using FindWindow later
	wndcls.lpszClassName = g_WndClassName;

	if(!FirstInstance())
	{
		AfxMessageBox("Program is already running!"); return FALSE;
	}

	//del 20100831
/*	if(!CheckHwIfRunnning())
	{
		PROCESS_INFORMATION pi;
		STARTUPINFO si = {0};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_SHOW;

		TCHAR szModulePath[_MAX_PATH] = _T("");
		GetModuleFileName(NULL,szModulePath,_MAX_PATH);

		CString szTemp = szModulePath;
		CString szModuleName1 = _T("");
		CString szModuleName2 = _T("");

		szModuleName1 = szTemp.Left(szTemp.ReverseFind('\\'));
		szModuleName2 = szModuleName1 + "\\HwIf.exe";

		BOOL bSuccess = CreateProcess( NULL, (LPSTR)(LPCTSTR)szModuleName2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
	
		if(!bSuccess)
		{
			AfxMessageBox("Failed to load HwIf.exe"); return FALSE;
		}
	}
*/
    // Register new class and exit if it fails
    if(!AfxRegisterClass(&wndcls))
    {
      TRACE(_T("Class Registration Failed\n"));
      return FALSE;
    }
	else{
		bClassRegistered = TRUE;
	}

	AfxEnableControlContainer();

	CheckNoRunningProcess();

	RegisterPackages();


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	CString szRegisterKey;
	szRegisterKey.Format("DM DFT Program%d",g_nRunningProcessNo);

	SetRegistryKey(szRegisterKey);

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDATsysDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDATsysView));
	AddDocTemplate(pDocTemplate);

//	m_nCmdShow = SW_SHOWMAXIMIZED;

	//// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	////// Dispatch commands specified on the command line
 	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	//OnFileNew();

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CLabelControl	m_ctrlLastUpdateLbl;
	CLabelControl	m_ctrlFileVerLbl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_LBL_LAST_UPDATE, m_ctrlLastUpdateLbl);
	DDX_Control(pDX, IDC_LBL_FILE_VER, m_ctrlFileVerLbl);
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);

//	COLORREF crBackColor = RGB(210, 210, 198);//::GetSysColor(COLOR_BTNFACE) + RGB(30, 30, 30);
//	COLORREF crPushColor = RGB(238, 239, 231);//RGB(210, 210, 198);
//	m_ctrlBtnOK.SetBitmaps(IDB_OK_2, RGB(192, 192, 192), IDB_OK_2, RGB(192, 192, 192));
//	m_ctrlBtnOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, crPushColor);
//	m_ctrlBtnOK.SetColor(CButtonST::BTNST_COLOR_BK_OUT, crBackColor);
//	m_ctrlBtnOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, crBackColor);
//	m_ctrlBtnOK.SetFlat(FALSE);
	
	//=================
	// Get DFT Version
	//=================
	CString sSwVer, sHwVer ;

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
	sSwVer = "Product Version ";
	sSwVer += pcVer;
	m_ctrlFileVerLbl.SetCaption(sSwVer);
	delete [] pcVer;

	VerQueryValue(lpVersionInfo, TEXT("\\StringFileInfo\\040904b0\\SpecialBuild"), &lpBuffer, &uiLen);

	pcVer = new char [uiLen+1];
	strncpy(pcVer, (char *)lpBuffer, uiLen);	// Special Build
	sSwVer = "Last Update     ";
	sSwVer += pcVer;
	m_ctrlLastUpdateLbl.SetCaption(sSwVer);
	delete [] pcVer;
	delete [] lpVersionInfo;

	return TRUE;  
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CDATsysApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDATsysApp message handlers

BOOL CDATsysApp::FirstInstance()
{
  CWnd *pWndPrev, *pWndChild;
  
  //+change kwmoon 080515
  // Determine if another window with our class name exists...
//if (pWndPrev = CWnd::FindWindow(_T("DFTsysClass"),NULL))
  if (pWndPrev = CWnd::FindWindow(g_WndClassName,NULL))
  {
    // if so, does it have any popups?
    pWndChild = pWndPrev->GetLastActivePopup(); 

    // If iconic, restore the main window
    if (pWndPrev->IsIconic()) pWndPrev->ShowWindow(SW_RESTORE);

    // Bring the main window or its popup to the foreground
    pWndChild->SetForegroundWindow();         

    // and we are done activating the previous one.
    return FALSE;                             
  }
  // First instance. Proceed as normal.
  else return TRUE;                              
}


int CDATsysApp::ExitInstance() 
{

	CoFreeUnusedLibraries();

	if(bClassRegistered)
		::UnregisterClass(g_WndClassName, AfxGetInstanceHandle());
	
	return CWinApp::ExitInstance();
}

BOOL CDATsysApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::PreTranslateAppMessage(pMsg);

	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CDATsysApp::CheckNoRunningProcess()
{
	TCHAR szModulePath[_MAX_PATH] = _T("");

	GetModuleFileName(NULL,szModulePath,_MAX_PATH);

	CString szTemp = szModulePath;

	CString szModuleName1 = _T("");
	CString szModuleName2 = _T("");
	int nNoRunningProcess = 0;
	DWORD dwExitCode = 0;

	szModuleName1 = szTemp.Right(szTemp.GetLength()-szTemp.ReverseFind('\\')-1);

	HANDLE hndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	DWORD dwsma = GetLastError();


	PROCESSENTRY32  procEntry={0};
	PROCESSENTRY32  procEntry_T={0};
	procEntry.dwSize = sizeof( PROCESSENTRY32 );
	Process32First(hndl,&procEntry);
	do
	{
		szModuleName2 = procEntry.szExeFile; 
		if(szModuleName1.Compare(szModuleName2) == NULL)
		{
			nNoRunningProcess++;
			
			if(nNoRunningProcess == 1) procEntry_T = procEntry;

			if(nNoRunningProcess > 1)
			{
				break;
			}
		}
	}while(Process32Next(hndl,&procEntry));
	
	if(nNoRunningProcess > 1)
	{
		HANDLE hHandle;

		hHandle = ::OpenProcess(PROCESS_ALL_ACCESS,0,procEntry_T.th32ProcessID);
	
		::GetExitCodeProcess(hHandle,&dwExitCode);
		::TerminateProcess(hHandle,dwExitCode);

		CloseHandle(hHandle);
	}

	CloseHandle(hndl);
	
	return TRUE;
}


	//del 20100831
/*
BOOL CDATsysApp::CheckHwIfRunnning()
{
//	TCHAR szModulePath[_MAX_PATH] = _T("");

//	GetModuleFileName(NULL,szModulePath,_MAX_PATH);

	CString szTemp = _T("");
	
	CString szModuleName1 = "HWIF.EXE";
	CString szModuleName2 = _T("");

//	szModuleName1 = szTemp.Right(szTemp.GetLength()-szTemp.ReverseFind('\\')-1);

	int nNoRunningProcess = 0;

	int index = 0;

	PIS *p_data = NULL;
	
	HANDLE h_snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	
	PROCESSENTRY32 entry_data;
	
//	entry_data.dwSize = sizeof(entry_data);
	entry_data.dwSize = sizeof(PROCESSENTRY32);

	char continue_flag = Process32First(h_snap_shot, &entry_data);
	
	char temp[256] = {0, };
	
	while(continue_flag)
	{
		HANDLE hwnd = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,entry_data.th32ProcessID);
		
		if(hwnd != NULL)
		{
			if(GetModuleFileNameEx(hwnd, NULL, temp, 256))
			{
				szTemp = temp;
				szModuleName2 = szTemp.Right(szTemp.GetLength()-szTemp.ReverseFind('\\')-1);
				szModuleName2.MakeUpper();

				if(szModuleName1.Compare(szModuleName2) == NULL)
				{
					nNoRunningProcess++; 
					//090909
					CloseHandle(hwnd);
					break;

				//	if(nNoRunningProcess > 2)
				//	{
				//		CloseHandle(hwnd); CloseHandle(h_snap_shot); return FALSE;	
				//	}
				}
			}
			CloseHandle(hwnd);
		} 
		continue_flag = Process32Next(h_snap_shot, &entry_data);
	}
	CloseHandle(h_snap_shot);

//	g_nRunningProcessNo = nNoRunningProcess;

	if(nNoRunningProcess >0 )return TRUE;
	else return FALSE;
}
*/


std::string CDATsysApp::MD5(std::string input)
{
#ifdef	DEBUG_MD5_CODE__
	return " ";
#else
	HCRYPTPROV CryptProv;
	HCRYPTHASH CryptHash;
	BYTE BytesHash[33];
	DWORD dwHashLen = 16;
	CString log(_T(""));

	std::string final;	if (CryptAcquireContext(&CryptProv,
		NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
	{
		if (CryptCreateHash(CryptProv, CALG_MD5, 0, 0, &CryptHash))
		{
			if (CryptHashData(CryptHash, (BYTE*)input.c_str(), input.length(), 0))
			{
				if (CryptGetHashParam(CryptHash, HP_HASHVAL, BytesHash, &dwHashLen, 0))
				{
					final.clear();
					std::string hexcharset = "0123456789ABCDEF";
					for (int j = 0; j < 16; j++)
					{
						final += hexcharset.substr(((BytesHash[j] >> 4) & 0xF), 1);
						final += hexcharset.substr(((BytesHash[j]) & 0x0F), 1);
					}
				}
				else
				{
					log.Format(_T("Error Code %x"), GetLastError());
					AfxMessageBox(log);
				}
			}
		}
	}	CryptDestroyHash(CryptHash);
	CryptReleaseContext(CryptProv, 0);
	return final;
#endif
}

BOOL CDATsysApp::Check_Checksum()
{
#if 1
	return TRUE;
#else

	CString target_data("");
	CString date("");
	CString my_data("");
	CString sMsg;

	target_data.Format(_T("%s"), __argv[1]);
	target_data.MakeUpper();

	CTime now = CTime::GetCurrentTime();
	date = now.Format("%Y%m%d");
	my_data.Format(_T("-%s"), MD5(date.GetString()).c_str());
	g_sData_MD5 = my_data;

	if (my_data.Compare(target_data) == 0) {
		return TRUE;
	}
	else {


#ifdef	DEBUG_MD5_CODE__
		//	sMsg.Format("Pass MD5 code is invalid\n\rT:%s, D:%s", target_data, my_data);
		AfxMessageBox("SKIP MD5 CODE");
		return TRUE;
#else
		sMsg.Format("MD5 code is invalid\n\rT:%s, D:%s", target_data, my_data);
		AfxMessageBox(sMsg);
		return FALSE;

#endif
	}
#endif
}
