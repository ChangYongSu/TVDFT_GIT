/***********************************************************

	FileName : windlib.h

	Provider : NaenTech. Inc.
	Version 1.0
	Date : 05-01-2015
	Supported O.S. :
		- MS Windows : above Windows XP(SP3)
		- Linux : above Linux kernel 2.2.
		- MAC   : -

 ***********************************************************/
 
#if !defined(__WINDLIB_CTRL_H)
#define  __WINDLIB_CTRL_H

#pragma once

#if !defined(__WINDLIB_H)
#error 'This header is subset of windlib.h.  use include windlib.h'
#endif

#include "osconfig.h"

class WINDAPI CWApplication;


//
//	CWApplication
//
class CWApplication
{
public:
	/*!	WAppMain

		@breif  call CWApplication::WAppMain in your main()
	*/
	static int WAppMain(CWApplication * pApp, int argc, char ** argv)
		{
			if (! pApp)
				return -1;

			pApp->__argc = argc;
			pApp->__argv = argv;

			int		nRet;
			if (! pApp->InitInstance())
				nRet = -1;
			else
				nRet = pApp->Run();

			pApp->ExitInstance();

			return nRet;
		}

public:
	CWApplication();
	virtual ~CWApplication();

	virtual bool InitInstance();
	virtual int  ExitInstance();
	virtual int  Run();
		
public:
	char	m_szAppPath[_MAX_PATH];
	char	m_szAppName[_MAX_PATH];

public:
	int		__argc;
	char	** __argv;
};


#endif  // __WINDLIB_CTRL_H
