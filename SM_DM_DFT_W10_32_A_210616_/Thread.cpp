// 명    칭 : AIAS(Automatic Inline Alignment System)
// 창작일자 : 1999. 10. 
// 소    속 : LG전자(LG생산기술원 MIT 박승화)
// File  명 : Thread.cpp

// Thread.cpp : implementation file
//

#include "stdafx.h"
#include <afxmt.h>
#include "datsys.h"
#include "Thread.h"
#include "global.h"
#include "support.h"		
#include <conio.h>
#include "PciDio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseThread
//CCriticalSection critic;

IMPLEMENT_DYNCREATE(CBaseThread, CWinThread)

CBaseThread::CBaseThread()
{
	m_bDone = FALSE;
}

CBaseThread::~CBaseThread()
{
}

BOOL CBaseThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CBaseThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CBaseThread, CWinThread)
	//{{AFX_MSG_MAP(CBaseThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseThread message handlers

int CBaseThread::Run() 
{
	int nJigStatus;
//	INPUT_Type nStatus;
	CString sMsg;

	while (!m_bDone)
	{ 
		nJigStatus = gPciDioCtrl.Jig_Ready_Check();

		if(CurrentSet->nRunStartType == AUTO_START)
		{	
			if(nJigStatus == IN_JIGUP)
			{
				if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
				{
						CurrentSet->bSystemBreak = TRUE;
						sMsg.LoadString(IDS_CYLINDER_UP);
						AddStringToStatus(sMsg);
						AfxMessageBox(sMsg);
				}
				CurrentSet->nJigStatus = IN_JIGUP;
				gJigStatus = IN_JIGUP; 
			//	CurrentSet->nOldFailedStep = 0;
			}
			else if(nJigStatus == IN_JIGDN)
			{
				if(CurrentSet->nJigStatus == IN_JIGUP)
				{
					if (CurrentSet->nRunStartType == AUTO_START && CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
					{
						m_pOwner->PostMessage(WM_COMMAND, ID_RUN_RUN);
					//	CurrentSet->bIsRunMsg = TRUE;
					}
				}
				CurrentSet->nJigStatus = IN_JIGDN;
			}
		}
		else{
			if(nJigStatus == IN_JIGUP)
			{
				if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
				{
						CurrentSet->bSystemBreak = TRUE;
						sMsg.LoadString(IDS_CYLINDER_UP);
						AddStringToStatus(sMsg);
						AfxMessageBox(sMsg);
				}
				CurrentSet->nJigStatus = IN_JIGUP;
				gJigStatus = IN_JIGUP; 
			//	CurrentSet->nOldFailedStep = 0;
			}
			/*
			else if(nJigStatus == IN_JIGDN)
			{
				nStatus = Check_StartStop_Switch();
				if (nStatus == IN_TEST_START)
				{
					if (CurrentSet->bIsRunning == FALSE && CurrentSet->bIsRunMsg == FALSE)
					{
						m_pOwner->PostMessage(WM_COMMAND, ID_RUN_RUN);
					}
				}
				else if (nStatus == IN_TEST_STOP)
				{
					if (CurrentSet->bIsRunning == TRUE && CurrentSet->bRunAbort == FALSE)
					{
						CurrentSet->bRunAbort = TRUE;
					}
				}
			}*/
		}
		Sleep(0);
	}
	return 0;
}

void CBaseThread::SetOwner(CWnd* pOwner)
{
	m_pOwner = pOwner;
}
