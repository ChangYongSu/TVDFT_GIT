// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A91C3946_305C_11D3_90A6_006008CED7F4__INCLUDED_)
#define AFX_STDAFX_H__A91C3946_305C_11D3_90A6_006008CED7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <sdkddkver.h>         // MFC core and standard components

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>		// MFC templete class
//#include <afxdao.h>         // MFC DAO database
//#include <afxdb.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Include core STL header
#include <string>
#include <streambuf>
#include <iostream>

#include <afxsock.h>
#include <afxcontrolbars.h>
#include <afx.h>
#include <afxdlgs.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#define SM_MODIFY_CODE__
#define _EXIT_DEBUG
#ifdef _DEBUG

	#define DEBUG_MD5_CODE__
	//#define TEXT_MSG_EXE_DEBUG_CODE__
	#define _PLC_COM_SIM_DEBUG__MODE
#define _NO_CONNECT_DEBUG__MODE
//#define _ERROR_CHILD_CONNECT_DEBUG__MODE
//#define _ABORT_DEBUG__MODE
#define _LISTSCROLL_DEBUG__MODE
#define _TVCOM_PASS_DEBUG__MODE
#endif

#define  MAX_COLUMN_NO	13

//#define SM_CYS_TEMP_TEST_CODE__  //SM_MODIFY_CODE__


#endif // !defined(AFX_STDAFX_H__A91C3946_305C_11D3_90A6_006008CED7F4__INCLUDED_)
