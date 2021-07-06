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

#include <sdkddkver.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxtempl.h>		// MFC templete class
//#include <afxdao.h>         // MFC DAO database
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Include core STL header
#include <string>
#include <streambuf>
#include <iostream>

#include <afxsock.h>



#include <stdio.h>

#include <wtypes.h>


#ifdef _DEBUG

#define DEBUG_MD5_CODE__

#endif

#define  MAX_COLUMN_NO	14



typedef struct
{
	double Data;
	int bNG;
}PlotData_T;



//extern "C" int _imp___vsnprintf(char *buffer, size_t count, const char *format, va_list argptr) {
//
//	return vsnprintf(buffer, count, format, argptr);

//}
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A91C3946_305C_11D3_90A6_006008CED7F4__INCLUDED_)
