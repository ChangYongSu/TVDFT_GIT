// DATsys.h : main header file for the DATSYS application
//

#if !defined(AFX_DATSYS_H__A91C3944_305C_11D3_90A6_006008CED7F4__INCLUDED_)
#define AFX_DATSYS_H__A91C3944_305C_11D3_90A6_006008CED7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDATsysApp:
// See DATsys.cpp for the implementation of this class
//

class CDATsysApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL FirstInstance();
	CDATsysApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDATsysApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	int GetProcessList(CString lcurname, int MaxNum, int lExitCode);

public: 

	//+add kwmoon 071015 : [2in1]
	BOOL CheckNoRunningProcess();
	BOOL CheckHwIfRunnning();
	BOOL CheckBarcodeIfRunnning();
	std::string MD5(std::string input);
	BOOL Check_Checksum();
	BOOL WriteRegVal_Agent(char *val);

// Implementation
	//{{AFX_MSG(CDATsysApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATSYS_H__A91C3944_305C_11D3_90A6_006008CED7F4__INCLUDED_)
