
// AcqVoltageSamples_IntClk.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "AcqVoltageSamples_IntClkDlg.h"
#include "TlHelp32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CAcqVoltageSamples_IntClkDlg *m_P_Main;

#define NO_PAD_CTRL						0x80000000	//(1 << 17)
#define NO_PAD_I						0
#define NO_MUX_I						0

#define	PAD_CTL_HYS                     (1 << 16)
#define	PAD_CTL_PUS_100K_DOWN           (0 << 14)
#define	PAD_CTL_PUS_47K_UP              (1 << 14)
#define	PAD_CTL_PUS_100K_UP             (2 << 14)
#define	PAD_CTL_PUS_22K_UP              (3 << 14)
#define	PAD_CTL_PUE                     (1 << 13)
#define	PAD_CTL_PKE                     (1 << 12)
#define	PAD_CTL_ODE                     (1 << 11)
#define	PAD_CTL_SPEED_LOW               (1 << 6)
#define	PAD_CTL_SPEED_MED               (2 << 6)
#define	PAD_CTL_SPEED_HIGH              (3 << 6)
#define	PAD_CTL_DSE_DISABLE             (0 << 3)
#define	PAD_CTL_DSE_240ohm              (1 << 3)
#define	PAD_CTL_DSE_120ohm              (2 << 3)
#define	PAD_CTL_DSE_80ohm               (3 << 3)
#define	PAD_CTL_DSE_60ohm               (4 << 3)
#define	PAD_CTL_DSE_48ohm               (5 << 3)
#define	PAD_CTL_DSE_40ohm               (6 << 3)
#define	PAD_CTL_DSE_34ohm               (7 << 3)
#define	PAD_CTL_SRE_FAST                (1 << 0)
#define	PAD_CTL_SRE_SLOW                (0 << 0)

#define MX6DL_WEIM_NOR_PAD_CTRL (PAD_CTL_SRE_FAST | PAD_CTL_DSE_40ohm |	\
		PAD_CTL_SPEED_MED | PAD_CTL_PKE	| PAD_CTL_PUE | PAD_CTL_PUS_100K_UP)


#define AIPS1_ARB_BASE_ADDR		0x02000000
#define AIPS1_ARB_END_ADDR		0x020FFFFF
#define AIPS2_ARB_BASE_ADDR		0x02100000
#define AIPS2_ARB_END_ADDR		0x021FFFFF

/* Defines for Blocks connected via AIPS (SkyBlue) */
#define ATZ1_BASE_ADDR			AIPS1_ARB_BASE_ADDR
#define ATZ2_BASE_ADDR			AIPS2_ARB_BASE_ADDR

/* ATZ#1- Off Platform */
#define AIPS1_OFF_BASE_ADDR		(ATZ1_BASE_ADDR + 0x80000)
/* ATZ#2- Off Platform */
#define AIPS2_OFF_BASE_ADDR		(ATZ2_BASE_ADDR + 0x80000)

#define WEIM_BASE_ADDR			(AIPS2_OFF_BASE_ADDR + 0x38000)
#define CCM_BASE_ADDR			(AIPS1_OFF_BASE_ADDR + 0x44000)


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
	int		a = PAD_CTL_SRE_FAST | PAD_CTL_DSE_40ohm | PAD_CTL_SPEED_MED | PAD_CTL_HYS;//	0x0001 00b1 = 
	int		b = PAD_CTL_SRE_SLOW | PAD_CTL_DSE_40ohm | PAD_CTL_SPEED_MED | PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP | PAD_CTL_HYS;//	0x0001 b0b0 = 
	int		c = PAD_CTL_SRE_FAST | PAD_CTL_DSE_40ohm | PAD_CTL_SPEED_MED | PAD_CTL_PKE | PAD_CTL_PUE | PAD_CTL_PUS_100K_UP;//	0x0000 b0b1 = 

	int		aa = WEIM_BASE_ADDR;
	int		bb = CCM_BASE_ADDR;


		unsigned int reg;
	struct clk *clk;
	int ret;
	union {
		int value;
		struct {
			int csen:1;
			int swr:1;
			int srd:1;
			int mum:1;
			int wfl:1;
			int rfl:1;
			int cre:1;
			int crep:1;
			int bl:3;
			int wc:1;
			int bcd:2;
			int bcs:2;
			int dsz:3;
			int sp:1;
			int csrec:3;
			int aus:1;
			int gbc:3;
			int wp:1;
			int psz:4;
		} reg;
	}gcr1;

	union {
		int value;
		struct {
			int adh:2;
			int reserve_1:2;
			int daps:4;
			int dae:1;
			int dap:1;
			int reserve_2:2;
			int mux16_byp_grant:1;
			int reserve_3:19;
		} reg;
	}gcr2;

	union {
		int value;
		struct {
			int rcsn:3;
			int reserve_1:1;
			int rcsa:3;
			int reserve_2:1;
			int oen:3;
			int reserve_3:1;
			int oea:3;
			int reserve_4:1;
			int radvn:3;
			int ral:1;
			int radva:3;
			int reserve_5:1;
			int rwsc:6;
			int reserve_6:2;
		} reg;
	}rcr1;

	union {
		int value;
		struct {
			int rben:3;
			int rbe:1;
			int rbea:3;
			int reserve_1:1;
			int rl:2;
			int reserve_2:2;
			int pat:3;
			int apr:1;
			int reserve_4:16;
		} reg;
	}rcr2;

	union {
		int value;
		struct {
			int wcsn:3;
			int wcsa:3;
			int wen:3;
			int wea:3;
			int wben:3;
			int wbea:3;
			int wadvn:3;
			int wadva:3;
			int wwsc:6;
			int wbed:1;
			int wal:1;
		} reg;
	}wcr1;

	union {
		int value;
		struct {
			int wbcdd:1;
			int reserved:31;
		} reg;
	}wcr2;

	/* For EIM General Configuration registers. */
	gcr1.value = 0;
	gcr1.reg.csen = 1;
	gcr1.reg.mum  = 1;
	gcr1.reg.dsz  = 3;	//32bit
	gcr1.reg.bcd  = 1;
	gcr1.reg.srd  = 1;
	gcr1.reg.swr  = 1;
	gcr1.reg.aus  = 0;		//address unshift	32bit aus = 1

	gcr1.reg.rfl  = 1;
	gcr1.reg.wfl  = 1;
	////gcr1.value = 0x00620081

	gcr2.value = 0;
	gcr2.reg.adh = 1;
	////gcr2.value = 0x00000001;

	/* For EIM Read Configuration registers.*/
	rcr1.value = 0;
	rcr1.reg.rwsc = 6;
	rcr1.reg.radvn = 1;
	////rcr1.value = 0x06018000;

	rcr2.value = 0;
	////rcr2.value = 0x0000C000;

	/* For EIM Write Configuration registers. */
	wcr1.value = 0;
	wcr1.reg.wwsc = 6;
	wcr1.reg.wadvn = 1;
	////wcr1.value = 0x1404a38e;

	wcr2.value = 0;
	////wcr2.value = 0;
}

CAcqVoltageSamples_IntClkApp g_theApp;


// The one and only CAcqVoltageSamples_IntClkApp object
//CAcqVoltageSamples_IntClkApp theApp;

// CAcqVoltageSamples_IntClkApp initialization
BOOL CAcqVoltageSamples_IntClkApp::InitInstance()
{
    CWinApp::InitInstance();
    
	
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
	
	CString lfname = _T("DXD_AutoTester.exe");//HomeViewAutoTester.exe
	if( GetProcessList(lfname, 1024, 0) > 1)
	{
		if(IDYES ==AfxMessageBox(" DXD_AutoTester.exe Program Running Detected!!\r\n\r\nTerminate Old Program??", MB_YESNO))
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
		m_P_Main = &dlg;
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

