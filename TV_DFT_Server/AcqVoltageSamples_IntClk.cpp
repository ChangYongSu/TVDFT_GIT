
// AcqVoltageSamples_IntClk.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "AcqVoltageSamples_IntClkDlg.h"
#include "TlHelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



int CAcqVoltageSamples_IntClkApp::GetProcessList(CString lcurname, int MaxNum, int lExitCode)
{
	CString lfname;
    int Index = 0;
    HANDLE         hProcessSnap = NULL;
//    DWORD          Return       = FALSE;
    PROCESSENTRY32 pe32         = {0};

    //프로세스의 스냅샷을 생성한다.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return 0; 
	DWORD MyID ,CheckId;
		
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
            hProcess = OpenProcess (PROCESS_ALL_ACCESS,
                FALSE, pe32.th32ProcessID);
            dwPriorityClass = GetPriorityClass (hProcess);              

            if (Index < MaxNum)
            {
                //pProcessList[Index].ProcessName = pe32.szExeFile;
                lfname = pe32.szExeFile;
				if(lfname == lcurname)
				{
					Index++;
					if(lExitCode)
					{
						CheckId = GetProcessId(hProcess);
						if(MyID != CheckId)
						{
							if(TerminateProcess(hProcess,0))
							{
							}
						}
					}
				}
            }

            CloseHandle (hProcess);      

        }
        while (Process32Next(hProcessSnap, &pe32));
    } 

    else
        return 0;
           // could not walk the list of processes 

    // Do not forget to clean up the snapshot object.
    CloseHandle (hProcessSnap);

    return Index;

}


// CAcqVoltageSamples_IntClkApp

BEGIN_MESSAGE_MAP(CAcqVoltageSamples_IntClkApp, CWinApp)
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAcqVoltageSamples_IntClkApp construction
CAcqVoltageSamples_IntClkApp::CAcqVoltageSamples_IntClkApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CAcqVoltageSamples_IntClkApp object
CAcqVoltageSamples_IntClkApp theApp;

// CAcqVoltageSamples_IntClkApp initialization
BOOL CAcqVoltageSamples_IntClkApp::InitInstance()
{
// TODO: richedit2 라이브러리를 초기화하려면 AfxInitRichEdit2()을(를) 호출합니다.\n"    CWinApp::InitInstance();
    if (!AfxSocketInit())
	{
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

    AfxEnableControlContainer();

    //CAcqVoltageSamples_IntClkDlg dlg;
    //m_pMainWnd = &dlg;
    //INT_PTR nResponse = dlg.DoModal();
    //if (nResponse == IDOK)
    //{
    //    // TODO: Place code here to handle when the dialog is dismissed with OK
    //}
    //else if (nResponse == IDCANCEL)
    //{
    //    // TODO: Place code here to handle when the dialog is dismissed with Cancel
    //}
	
	int mExit = 0;
	
	CString lfname = _T("DFT_Server.exe");//HomeViewAutoTester.exe
	if( GetProcessList(lfname, 1024, 0) > 1)
	{
		if(IDYES ==AfxMessageBox(" DFT_Server.exe Program Running Detected!!\r\n\r\nTerminate Old Program??", MB_YESNO))
		{
			GetProcessList(lfname, 1024, 1);
		}
		else
		{
			mExit = 1;
		}
		
	}
	else
	{
	//	AfxMessageBox("Start HomeViewAutoTester.exe!!\r\n\r\nConnect Cable!!");
	}

//	CIntroDlg dlg;
//	dlg.DoModal();

	if(mExit == 0)
	{

		
		CAcqVoltageSamples_IntClkDlg dlg;
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is dismissed with Cancel
		}
	}
	
    // Since the dialog has been closed, return FALSE so that we exit the
    // application, rather than start the application's message pump.
    return FALSE;
}

