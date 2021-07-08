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
#include <psapi.h> 

#include <wincrypt.h>
#include <string>
using namespace std;

//{{AFX_INCLUDES()
//}}AFX_INCLUDES
//-

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "TlHelp32.h"






//===================
// External Variable
//===================
static BOOL bClassRegistered = FALSE;

extern CSoundCard	g_SoundCard;
extern CString		g_WndClassName;
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






int CDATsysApp::GetProcessList(CString lcurname, int MaxNum, int lExitCode)
{
	CString lfname;
	int Index = 0;
	HANDLE         hProcessSnap = NULL;
	//    DWORD          Return       = FALSE;
	PROCESSENTRY32 pe32 = { 0 };

	//프로세스의 스냅샷을 생성한다.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return 0;
	DWORD MyID, CheckId;

	MyID = GetCurrentProcessId();
	// HANDLE hMyProcess = GetCurrentProcess();
	pe32.dwSize = sizeof(PROCESSENTRY32);

	//시스템의 모든 프로세스를 가져와 processName와 이름이 같은 것의 카운트를 리턴.
	if (Process32First(hProcessSnap, &pe32))
	{
		//        DWORD Code = 0;
		DWORD         dwPriorityClass;

		do
		{
			HANDLE hProcess;

			// Get the actual priority class.
			hProcess = OpenProcess(PROCESS_ALL_ACCESS,
				FALSE, pe32.th32ProcessID);
			dwPriorityClass = GetPriorityClass(hProcess);

			if (Index < MaxNum)
			{
				//pProcessList[Index].ProcessName = pe32.szExeFile;
				lfname = pe32.szExeFile;
				lfname.MakeUpper();
				if (lfname == lcurname)
				{
					Index++;
					if (lExitCode)
					{
						//CheckId = GetProcessId(hProcess);
						//if (MyID != CheckId)
						//{
						//	if (TerminateProcess(hProcess, 0))
						//	{
						//	}
						//}
						break;
					}
				}
			}

			CloseHandle(hProcess);

		} while (Process32Next(hProcessSnap, &pe32));
	}

	else
		return 0;

// could not walk the list of processes 
// Do not forget to clean up the snapshot object.
	CloseHandle(hProcessSnap);
	return Index;
}


/////////////////////////////////////////////////////////////////////////////
// CDATsysApp initialization

BOOL CDATsysApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
	{
	//	afxAmbientActCtx = FALSE;

		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
    

// Determine whether to use DAO 3.6, 3.5, or 3.0
// Use DAO 3.0 if DLL build and not built with MFC 4.21 or later
// Use DAO 3.6 if MFC 6.01 or later
// otherwise, DAO 3.5

//#undef _MFC_VER
//#define _MFC_VER 0x0601


//	Check_Checksum();
//	if (!Check_Checksum())
//	{
//		return FALSE;
//	}

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

#ifdef _DFT_SYSTEM3

	g_nRunningProcessNo = 3;
	g_WndClassName		= _T("DFT3Class");

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

	TCHAR szModulePath[_MAX_PATH] = _T("");
	GetModuleFileName(NULL,szModulePath,_MAX_PATH);

	CString szTemp = szModulePath;
	CString szModuleName1 = _T("");
	CString szModuleName2 = _T("");

	szModuleName1 = szTemp.Left(szTemp.ReverseFind('\\'));

	if(FileExists(szModuleName1 + "\\DftSysInfo.Ini"))
	{
		if(!OpenSystemInfoFile(szModuleName1 + "\\DftSysInfo.Ini")){
			AfxMessageBox("Failed to load file.\n[DftSysInfo.Ini]");
			ExitProcess(0); 
		}
	}
	else
	{
		AfxMessageBox("Failed to load file.\n[DftSysInfo.Ini]");
		ExitProcess(0); 
	}


	PROCESS_INFORMATION pi;
	STARTUPINFO si = {0};
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_SHOW;

//AUTO
	if (g_nSysType == AUTO_SYS)
	{
		//if(g_nSysRobotType == MANUAL_SYS)
		//{

		//	if (!CheckBarcodeIfRunnning())
		//	{
		//		szModuleName1 = szTemp.Left(szTemp.ReverseFind('\\'));
		//		szModuleName2 = szModuleName1 + "\\DFT_BarcodeRead.exe";

		//		BOOL bSuccess = CreateProcess(NULL, (LPSTR)(LPCTSTR)szModuleName2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		//		if (!bSuccess)
		//		{
		//			AfxMessageBox("Failed to load DFT_BarcodeRead.exe"); return FALSE;
		//		}
		//	}
		//}

		if (g_nUseNoScanType == FALSE)
		{

			if (!CheckBarcodeIfRunnning())
			{
				szModuleName1 = szTemp.Left(szTemp.ReverseFind('\\'));
				szModuleName2 = szModuleName1 + "\\DFT_BarcodeRead.exe";

				BOOL bSuccess = CreateProcess(NULL, (LPSTR)(LPCTSTR)szModuleName2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

				if (!bSuccess)
				{
					AfxMessageBox("Failed to load DFT_BarcodeRead.exe"); return FALSE;
				}
			}
		}
	}

	if(!CheckHwIfRunnning())
	{
		//UHD
		if(g_nGrabberType == UHD_GRABBER)
		{
			szModuleName2 = szModuleName1 + "\\HwIf_UHD.exe";
		}
		else//FHD
		{
			if(g_nSysType == AUTO_SYS){
				szModuleName2 = szModuleName1 + "\\HwIf_AUTO.exe";
			}
			else{
				szModuleName2 = szModuleName1 + "\\HwIf_FHD.exe";
			}
		}
		BOOL bSuccess = CreateProcess( NULL, (LPSTR)(LPCTSTR)szModuleName2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
	
		if(!bSuccess)
		{
			AfxMessageBox("Failed to load HwIf_XXX.exe"); return FALSE;
		}
	}

/*	if(!CheckBarcodeIfRunnning())
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
		szModuleName2 = szModuleName1 + "\\DFT_BarcodeRead.exe";

		BOOL bSuccess = CreateProcess( NULL, (LPSTR)(LPCTSTR)szModuleName2, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi );
	
		if(!bSuccess)
		{
			AfxMessageBox("Failed to load DFT_BarcodeRead.exe"); return FALSE;
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
/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	CString szRegisterKey;
	szRegisterKey.Format("DFT Program%d",g_nRunningProcessNo);

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

	if (!pDocTemplate){
		AfxMessageBox("CSingleDocTemplat Fail");
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);


//	m_nCmdShow = SW_SHOWMAXIMIZED;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line
 	if (!ProcessShellCommand(cmdInfo))
	{
		AfxMessageBox("ProcessShellCommand Fail");
		return FALSE;
	}

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
	CString	m_szInfomation;
	CString	m_szLastUpdate;
	CString	m_szFileVersion;
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
	m_szInfomation = _T("");
	m_szLastUpdate = _T("");
	m_szFileVersion = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_EDIT_INFO, m_szInfomation);
	DDX_Text(pDX, IDC_STATIC_LAST_UPDATE, m_szLastUpdate);
	DDX_Text(pDX, IDC_STATIC_FILE_VERSION, m_szFileVersion);
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

	m_szInfomation = _T("Warning : this computer program is protecting by copyright law and international treaties. Unauthorized reproduction or distribution of this program, or any portion of it , may result in severe civil and criminal penalties, and will be prosecuted to the maximum extent possible under the law.");
	strcpy_s(caAppName, _countof(caAppName), AfxGetApp()->m_pszExeName);
	strcat_s(caAppName, ".exe");

	DWORD dwInfoSize = GetFileVersionInfoSize(caAppName, NULL);
	LPVOID lpVersionInfo = new BYTE[dwInfoSize];

	GetFileVersionInfo(caAppName, 0, dwInfoSize, lpVersionInfo);
	VerQueryValue(lpVersionInfo, TEXT("\\StringFileInfo\\040904b0\\ProductVersion"), &lpBuffer, &uiLen);

	char *pcVer = new char [uiLen+1];
	strncpy_s(pcVer, uiLen+1, (char *)lpBuffer, uiLen);	// Product Version
	sSwVer = "Product Version : ";
	sSwVer += pcVer;
	m_szFileVersion = sSwVer;
//	m_ctrlFileVerLbl.SetCaption(sSwVer);
	delete [] pcVer;

	VerQueryValue(lpVersionInfo, TEXT("\\StringFileInfo\\040904b0\\SpecialBuild"), &lpBuffer, &uiLen);

	pcVer = new char [uiLen+1];
	strncpy_s(pcVer, uiLen+1, (char *)lpBuffer, uiLen);	// Special Build
	sSwVer = "Last Update    : ";
	sSwVer += pcVer;
	m_szLastUpdate = sSwVer;
//	m_ctrlLastUpdateLbl.SetCaption(sSwVer);
	delete [] pcVer;
	delete [] lpVersionInfo;
	UpdateData(FALSE);

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
	//+change kwmoon 080515
//	if(bClassRegistered)
//		::UnregisterClass(_T("DFTsysClass"), AfxGetInstanceHandle());
	//+add kwmoon 080514
	// Close Sound Driver
/*	if(g_SoundCard.wIn_Flag)
	{
		g_SoundCard.WaveRead_Stop(); 
	} */

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


//+add kwmoon 071015 : [2in1]
BOOL CDATsysApp::CheckHwIfRunnning()
{
//	TCHAR szModulePath[_MAX_PATH] = _T("");

//	GetModuleFileName(NULL,szModulePath,_MAX_PATH);

	CString szTemp = _T("");
	
	CString szModuleName1 = _T("");
	CString szModuleName2 = _T("");

	//UHD
	if(g_nGrabberType == UHD_GRABBER)
	{
		szModuleName1 = "HWIF_UHD.EXE";
	}
	else//FHD
	{
		if(g_nSysType == AUTO_SYS){
			szModuleName1 = "HWIF_AUTO.EXE";
		}
		else{
			szModuleName1 = "HWIF_FHD.EXE";
		}
	}
//	szModuleName1 = szTemp.Right(szTemp.GetLength()-szTemp.ReverseFind('\\')-1);


	////////////////////////////////////////////////////////////////////////////
	if (GetProcessList(szModuleName1, 1024, 1))
	{
		return TRUE;
	}
	else
	{
		return FALSE;

	}
	/////////////////////////////////////////////////////////////////////////////

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

BOOL CDATsysApp::CheckBarcodeIfRunnning()
{

	CString szTemp = _T("");
	
	CString szModuleName1 = "DFT_BARCODEREAD.EXE";
	CString szModuleName2 = _T("");

	int nNoRunningProcess = 0;

	int index = 0;

	PIS *p_data = NULL;
	
	HANDLE h_snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	
	PROCESSENTRY32 entry_data;
	
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
			//	trace(szModuleName2);
				if(szModuleName1.Compare(szModuleName2) == NULL)
				{
					nNoRunningProcess++; 
					//090909
					CloseHandle(hwnd);
					break;

				}
			}
			CloseHandle(hwnd);
		} 
		continue_flag = Process32Next(h_snap_shot, &entry_data);
	}
	CloseHandle(h_snap_shot);

	if(nNoRunningProcess >0 )return TRUE;
	else return FALSE;
}

std::string CDATsysApp::MD5(std::string input)
{


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

}

BOOL CDATsysApp::Check_Checksum()
{
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
}
