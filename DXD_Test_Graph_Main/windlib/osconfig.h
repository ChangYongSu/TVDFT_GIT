/***********************************************************

	FileName : osconfig.h

	LG Detector SDK.
	Provider : NaenTech. Inc.
	Version 1.0
	Date : 05-01-2015
	Supported O.S. :
		- MS Windows : above Windows XP(SP3)
		- Linux : above Linux kernel 2.2.
		- MAC   : -

 ***********************************************************/

#if !defined(__OS_CONFIG_H)
#define __OS_CONFIG_H

#pragma once


/////////////////////////////////////////////////
//	Define O.S.
//
#define	OS_TYPE_WINDOWS		0
#define	OS_TYPE_LINUX		1
#define	OS_TYPE_APPLE		2

#if defined(WIN32) || defined(WIN64) || defined(_WIN64)
#	define	CUR_OS_TYPE			OS_TYPE_WINDOWS
#else
#	define	CUR_OS_TYPE			OS_TYPE_LINUX
#endif


#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)

#	include <Winsock2.h>
#	pragma comment(lib, "Ws2_32.lib")

#	include <windows.h>

#	define WIN32_LEAN_AND_MEAN

#	ifdef WINVER
#		if WINVER < 0x0502
#			error	"WLib is support above Windows XP(SP3)"
#		endif
#	else
#		define WINVER			0x0502		// Windows XP(SP3) 이상
#	endif

#	ifdef _WIN32_WINNT
#		if _WIN32_WINNT < 0x0502
#			error	"WLib is support above Windows XP(SP3)"
#		endif
#	else
#		define _WIN32_WINNT			0x0502	// Windows XP(SP3) 이상
#	endif

#	ifdef _WIN32_IE
#		if _WIN32_IE < 0x0601
#			error	"WLib is support above Windows IE 6.0(SP1)"
#		endif
#	else
#		define _WIN32_IE			0x0601	// Windows IE 6.01(SP1) 이상
#	endif

#else	//if (CUR_OS_TYPE != OS_TYPE_WINDOWS)

#	include <iostream>
#	include <assert.h>
#	include <dirent.h>
#	include <sys/time.h>

#	define	__in
#	define	__out
#	define	__inout
#	define	__in_opt
#	define	__out_opt
#	define	__ecount(size)		// element count
#	define	__in_ecount(size)
#	define	__out_ecount(size)
#	define	__bcount(size)		// binary count
#	define	__in_bcount(size)
#	define	__out_bcount(size)

//
//	New Type
//
typedef char			CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef int				INT;
typedef int				BOOL;
typedef unsigned int	UINT;
typedef long			LONG;
typedef	unsigned long	ULONG;
typedef unsigned long	DWORD;
typedef	float			FLOAT;
typedef double			DOUBLE;
typedef	void			VOID;
typedef long long		INT64;

typedef struct __tagPOINT
{
	LONG	x;
	LONG	y;
} POINT;

typedef struct __tagSIZE
{
	LONG	cx;
	LONG	cy;
} SIZE;

typedef struct __tagRECT
{
	LONG	left;
	LONG	top;
	LONG	right;
	LONG	bottom;
} RECT;

typedef char			* LPSTR;
typedef unsigned char	* LPBYTE;
typedef const char		* LPCSTR;
typedef unsigned short	* LPWORD;
typedef	int				* LPINT;
typedef unsigned int	* LPUINT;
typedef int				* LPbool;
typedef	long            * LPLONG;
typedef unsigned long   * LPULONG;
typedef unsigned long	* LPDWORD;
typedef	float			* LPFLOAT;
typedef double			* LPDOUBLE;
typedef void			* LPVOID;
typedef	const void		* LPCVOID;
typedef void			* HANDLE;

typedef POINT			* LPPOINT;
typedef SIZE			* LPSIZE;
typedef RECT			* LPRECT;
typedef const RECT		* LPCRECT;


#ifdef  NULL
#	undef  NULL
#	ifdef __cplusplus
#		define	NULL	0
#	else
#		define	NULL	((void *)0)
#	endif
#endif

#define	TRUE			1
#define	FALSE			0

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))

#endif	//(CUR_OS_TYPE == OS_TYPE_WINDOWS)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#endif  // __OS_CONFIG_H
