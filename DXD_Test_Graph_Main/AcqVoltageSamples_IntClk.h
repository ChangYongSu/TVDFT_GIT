
// AcqVoltageSamples_IntClk.h : main header file for the AcqVoltageSamples_IntClk application
//

#pragma once

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "./windlib/windlib.h"
#include "./main/comm_packet.h"


#include "resource.h"   // main symbols


// CAcqVoltageSamples_IntClkApp
// See AcqVoltageSamples_IntClk.cpp for the implementation of this class
//



class CAcqVoltageSamples_IntClkApp : public CWinApp
{
public:
     CAcqVoltageSamples_IntClkApp();


	 CString	m_strAppPath;
	 CString	m_strSavePath;
// Overrides
public:
    virtual BOOL InitInstance();
	int GetProcessList(CString lcurname, int MaxNum, int lExitCode);
// Implementation

    DECLARE_MESSAGE_MAP()
};

extern CAcqVoltageSamples_IntClkApp g_theApp;

