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

#if !defined(__WINDLIB_H)
#define __WINDLIB_H

#pragma once

#include "osconfig.h"


////////////////////////////////////////////////////////
//
//아래 둘중 하나만 사용하거나 둘다 사용하지 않는다.
//	둘다 사용하지 않으면 IMPORTS Type 이다.
//

//#define	__WINDLIB_API_EXPORTS
#define	__WINDLIB_API_LOCAL		//	Use WLib for only local

//
//
////////////////////////////////////////////////////////


#undef WINDAPI
#undef WINDOBJ

#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)

#	include <Winsock2.h>
#	pragma comment(lib, "Ws2_32.lib")

#	include <windows.h>

#	if defined(__WINDLIB_API_LOCAL)
#		define	WINDAPI
#		define	WINDOBJ
#	elif defined(__WINDLIB_API_EXPORTS)
#		define	WINDAPI		__declspec(dllexport)
#		define	WINDOBJ		__declspec(dllexport)
#	else
#		define	WINDAPI		__declspec(dllimport)
#		define	WINDOBJ		__declspec(dllimport)
#	endif

#	define	HANDLE_THREAD	HANDLE
#	define	THREADPROC_CONVENTION WINAPI

#	define	EXIT_THREAD_SELF(retVal)	ExitThread((DWORD) retVal)

typedef DWORD	THREADPROC_RESULT;

#	pragma warning(disable:4996)	// disable Secure-Warning

#else

#	define	CALLBACK

#	define WINDAPI
#	define WINDOBJ

#	define	HANDLE_THREAD	pthread_t
#	define	THREADPROC_CONVENTION
#	define	EXIT_THREAD_SELF(retVal)	pthread_exit((LPVOID) retVal)

typedef void * THREADPROC_RESULT;

#	define	_MAX_PATH		PATH_MAX
#	define	MAX_PATH		PATH_MAX

#endif	//(CUR_OS_TYPE == OS_TYPE_WINDOWS)


typedef THREADPROC_RESULT (THREADPROC_CONVENTION * WLIB_THREAD_STARTPROC)(void *);

#ifndef  INVALID_THREAD_HANDLE
#	define	INVALID_THREAD_HANDLE	((HANDLE_THREAD) -1)
#endif


#define	CH_DIR_SEP			'/'
#define	CH_DIR_SEP2			'\\'
#define	SZ_DIR_SEP			"/"
#define	SZ_DIR_SEP2			"\\"

#define	SZ_EMPTY_STR		""

///////////////////////////////////////////////////
//	Type Check and Convert
//
#define	WToUpper(ch)		(WIsLower((ch)) ? (ch)-'a'+'A' : (ch))
#define	WToLower(ch)		(WIsUpper((ch)) ? (ch)-'A'+'a' : (ch))

#define	WIsUpper(ch)		(((ch)>='A' && (ch)<='Z') ? 1 : 0)
#define	WIsLower(ch)		(((ch)>='a' && (ch)<='z') ? 1 : 0)
#define	WIsAlpha(ch)		(WIsUpper((ch)) || WIsLower((ch)))
#define	WIsAlNum(ch)		(WIsAlpha((ch)) || WIsDigit((ch)))
#define	WIsSpace(ch)		(((ch)==' ' || (ch)=='\n' || (ch)=='\r' || (ch)=='\t') ? 1 : 0)
#define	WIsDigit(ch)		(((ch)>='0' && (ch)<='9') ? 1 : 0)
#define	WIsHexDigit(ch) \
			(WIsDigit(ch) ? 1 : (WToUpper(ch)>='A' && WToUpper(ch) <= 'F') ? 1 : 0)

inline const char * SafeString(const char * p)
	{  return (p) ? p : "";	}

#define MAKE_INT64(dwl, dwh)	((((INT64) (dwh)) << 32) + ((INT64) (dwl)))

///////////////////////////////////////////////////
//	Type Check and Convert
template <typename Type>
Type WABS(Type data)
	{	return (data >= 0) ? data : -data;	}

template <typename Type>
Type WMax(Type a, Type b)
	{	return (a >= b) ? a : b;	}

template <typename Type>
Type WMin(Type a, Type b)
	{	return (a <= b) ? a : b;	}

template <typename Type>
Type WPow2(Type data)
	{	return data * data;	}

template <typename Type>
Type WClipValue(Type data, Type minVal, Type maxVal)
	{
		if (data < minVal)
			return minVal;
		else if (data > maxVal)
			return maxVal;
		else
			return data;
	}


/*
MakeNormalPathName
	- Path 내에 '/' 또는 '\\' 로 되있는 경로를 주어진 문자로 바꾼다.
	- cDirSep 는 '/' 또는 '\\' 만 올수 있다.
	- 그외 문자는 모두 '/' 로 대체함,
*/
WINDAPI
void MakeNormalPathName(__inout char * pszPath, __in_opt int cDirSep = CH_DIR_SEP);


/*
LoadConfigFile
	- Entry = data.... 구조로 되어있는것.  '#' 으로 시작하면 Comment
	- 대소문자 구분 안함.
*/
WINDAPI
int LoadConfigFile(const char * pszConfigFile, const char * pszEntry, char * pszBuf, int nBufLen);



//	Standard String
WINDAPI
char * StrTrimAll(char *  pszStr);

WINDAPI
char * StrTrimLeft(char *  pszStr);

WINDAPI
char * StrTrimRight(char *  pszStr);

WINDAPI
int SafeStrLen(const char * pszStr);

WINDAPI
char * StrToUpper(char * pszStr);

WINDAPI
char * StrToLower(char * pszStr);

//
// String 함수에서 bCaseSinsitibe 가 false이면 대소문자를 구분하지 않는다.
//
WINDAPI
int SafeStrCmp(const char * pszSour, const char * pszDest, bool bCaseSensitive);

WINDAPI
int SafeStrNCmp(const char * pszSour, const char * pszDest, int nLen, bool bCaseSensitive);

WINDAPI
char * SafeStrCpy(char * pszDest, const char * pszSour);

//	cbMaxLen < 0 이면 전체 복사
WINDAPI
char * SafeStrNCpy(char * pszDest, const char * pszSour, int cbMaxLen);

WINDAPI
char * SafeStrStr(const char * pszSour, const char * pszSub, bool bCaseSensitive);

// 주어진 문장에서 nLen 만큼 복사하고 pszDest 의 주소 반환
// 중간에 NULL 이 있으면 거기까지만 복사하다.
// 마지막에 NULL 문자 추가됨.
WINDAPI
char * TruncStrCpy(char * pszDest, const char * pszSour, int nLen);

// 주어진 문장에서 nLen 크기만큼 복사하고 pDest 의 주소 반환
// 중간에 NULL 이 있으면 거기까지만 복사하고 나머지 부분은 cFillChar 로 채움.
// 마지막에 NULL 문자 추가되지 않음.
WINDAPI
char * PackingCopy(char * pszDest, const char * pszSour, int nLen, int cFillChar);

// pszSour 에서 nCH 가 나오거나 NULL 이 나올때까지 pDest 로 복사한다.
// pszSour 에서 nCH 가 있으면 그 다음 부분의 주소를
// NULL 로 끊나는 문장이면 NULL 있는 부분의 주소를 반환한다.
// pszDest 의 마지막에 NULL 문자 추가됨.
WINDAPI
const char * FindDelimeter(const char * pszSour, char * pszDest, int nCH);

// pszSour 에서 pszFindStr 가 나오거나 NULL 이 나올때까지 pDest 로 복사한다.
// pszSour 에서 pszFindStr 가 있으면 그 다음 부분의 주소를
// NULL 로 끊나는 문장이면 NULL 있는 부분의 주소를 반환한다.
// pszDest 의 마지막에 NULL 문자 추가됨.
WINDAPI
const char * FindStrDelimeter(const char * pszSour, char * pszDest, const char * pszFindStr);

//	Binary Data 중에서 동일 데이타를 검색한다.
//	데이타 없으면 -1 반환.
WINDAPI
int FindBinaryData(const BYTE * pData, int cbData, const BYTE * pFindData, int nFindDataLen);

// 버퍼에서 nLen 크기만큼을 이용하여 숫자로 변환
// 문자열 중간에 숫자가 아닌 문자 또는 NULL 문자가 있으면 거기까지만 변환한다.
// nRadix : 진법을 써준다. 2, 4, 8, 10, 16 만 적용
WINDAPI
int NStrToInt(const char * pszStr, int nLen, int nRadix);

//	xxx,yyy 형태로 저장된 문자열 값을 변환한다.
WINDAPI
bool ParsingPoint(const char * pszStr, int * pnX, int * pnY);

//	Fills in the first 6 bytes of the `address' array with the
//	MAC address of our LAN card or with host-id/name dependent information.
WINDAPI
void GetSysAddress(unsigned char * pszAddress);


//////////////////////////////////////////////////////////////////////
//	Class Hierarchy chart in this library
//
class WINDOBJ CWObject;
	class WINDOBJ CWString;
	class WINDOBJ CWError;
	class WINDOBJ CWFile;

class WINDOBJ CWTimer;
class WINDOBJ CWMutex;
class WINDOBJ CWEvent;
class WINDOBJ CWThread;
class WINDOBJ CWLog;
class WINDOBJ CWLogManager;
class WINDOBJ CLogWriter;

;


/*!	CWObject

*/
#define	DEFAULT_WOBJECT_MASK		(* ((const DWORD *) "WLIB"))

class CWObject
{
protected:
	DWORD	m_dwCheckValid;

public:
	CWObject()	{	m_dwCheckValid = DEFAULT_WOBJECT_MASK;	}
	virtual ~CWObject()	{}

	bool IsValidObject() const
		{	return ((this != NULL) && (m_dwCheckValid == DEFAULT_WOBJECT_MASK));	}
};


/*!	CWString

*/
class CWString : public CWObject
{
public:
	CWString();
	CWString(__in const CWString & strSrc);
	CWString(__in char ch);
	CWString(__in const char * psz);
	CWString(__in const char * psz, __in int nLen);
	CWString(__in const unsigned char * psz);

	virtual ~CWString();

	int Compare(__in const CWString & dst) const;
	int Compare(__in const CWString * pDest) const;
	int Compare(__in const char* psz, __in bool bCaseSensitive = true) const;
	int CompareNoCase(__in const char* psz) const;

	int GetLength() const;
	bool IsEmpty() const;
	void Empty();			// 버퍼는 유지되고, 내용만 지운다.

	char* GetBuffer(__in int nMinLength = -1);
	void ReleaseBuffer();

	char GetAt(__in int nIndex) const;
	char operator[](__in int nIndex) const;
	void SetAt(__in int nIndex,__in  char ch);

	operator const char*() const;

	const CWString & operator=(__in const CWString & strSrc);
	const CWString & operator=(__in char ch);
	const CWString & operator=(__in const char * psz);
	const CWString & operator=(__in const unsigned char * psz);
	const CWString & operator+=(__in const CWString & str);
	const CWString & operator+=(__in char ch);

	friend bool operator==(__in const CWString & str1, __in const CWString & str2);
	friend bool operator==(__in const CWString & str, __in const char* psz);
	friend bool operator==(__in const char* psz, __in const CWString & str);
	friend bool operator!=(__in const CWString & str1, __in const CWString & str2);
	friend bool operator!=(__in const CWString & str, __in const char* psz);
	friend bool operator!=(__in const char* psz, __in const CWString & str);
	friend CWString operator+(__in const CWString & str1, __in const CWString & str2);
	friend CWString operator+(__in const CWString & str, __in char ch);
	friend CWString operator+(__in char ch, __in const CWString & str);
	friend CWString operator+(__in const CWString & str, __in const char* psz);
	friend CWString operator+(__in const char* psz, __in const CWString & str);

	CWString Left(__in int nCount) const;
	CWString Right(__in int nCount) const;
	CWString Mid(__in int nFirst) const;
	CWString Mid(__in int nFirst, __in int nCount) const;

	int Insert(int nIndex, char ch);
	int Remove(char chRemove);
	int Replace(char chOld, char chNew, bool bReplaceOnce = false);
	int Replace(const char* pszOld, const char* pszNew, bool bReplaceOnce = false);
	int NoCaseReplace(const char* pszOld, const char* pszNew, bool bReplaceOnce = false);

	void MakeUpper();
	void MakeLower();

	CWString& Trim();
	CWString& TrimRight();
	CWString& TrimLeft();

	// -1 if not found
	int Find(__in char ch, __in int nStart = 0) const;
	int Find(__in const char* pszSub, __in int nStart) const;
	int ReverseFind(__in char ch) const;
	int NoCaseFind(__in const char* pszSub, __in int nStart = 0) const;

	// 아래 함수는 Max 4KB String
	int Format(__in const char* pszFmt, ...);
	int AppendFormat(__in const char* pszFmt, ...);
	int FormatV(__in const char* pszFmt, __in va_list argList);

protected:
	void Init();
	void DeleteMem();
	void AllocCopy(CWString & dest, int nCopyLen, int nCopyIndex) const;
	int Replace(const char* pszOld, const char* pszNew, bool bCaseSensitive, bool bReplaceOnce);

	bool IsValidIndex(int nIndex) const;

	const CWString & CopyFrom(const char * psz, int nLen = -1);
	const CWString & ConcatFrom(const char* src, int nLen = -1);

private:
	static char * CopyOrConvert(const char * src, int cchCount);
	static char * Alloc(int cbCount);
	static char * ReAlloc(char * pBuffer, int cbCount);
	static void Release(void * pBuffer);
	static void * GetNilString();
	static void Free(char * ptr);
	static char * GetNillString();

protected:
	char*	m_pszData;
};


/*!	CWError

*/
class CWError : public CWObject
{
public:
	enum
	{
		WERR_SUCCESS		= 0,
	};
public:
	CWError()
		{	SetSuccess();	}
	CWError(int nErrCode, const char * pszErrMesg)
		{	SetError(nErrCode, pszErrMesg);	}

	CWError & operator=(const CWError & src)
		{
			m_nErrCode = src.m_nErrCode;
			m_strErrMesg = src.m_strErrMesg;
			return * this;
		}

	CWError & SetSuccess()
		{	return SetError(WERR_SUCCESS);	}
	CWError & SetError(int nErrCode, const char * pszErrMesg = NULL)
		{
			m_nErrCode = nErrCode;
			if (m_nErrCode == WERR_SUCCESS)
				m_strErrMesg.Format("[ErrCode=%d] : [Success]", m_nErrCode);
			else
				m_strErrMesg.Format("[ErrCode=%d] : %s", m_nErrCode, (pszErrMesg) ? pszErrMesg : "[Unknown]");
			return * this;
		}

	bool IsSuccess() const
		{	return (m_nErrCode == WERR_SUCCESS);	}
	int GetErrCode()
		{	return m_nErrCode;	}
	const char * GetErrMesg() const
		{	return m_strErrMesg;	}

protected:
	int		m_nErrCode;
	CWString	m_strErrMesg;
};


/*!	CWFile
	File Manager
*/
class CWFile : public CWObject
{
public:
	enum __AccessFlags__
	{
		accessExist = 0x00,
		accessWrite = 0x02,
		accessRead = 0x04,
		accessReadWrite = 0x06,
	};

	enum __FileSeekPosition__
	{
		seekBegin = SEEK_SET,
		seekCurrent = SEEK_CUR,
		seekEnd = SEEK_END
	};
	
	enum  __Constant__
	{
		F_EOF = -1,
	};

	CWFile();
	CWFile(const char * pszFileName, const char * pszFlags);
	virtual ~CWFile();

	virtual bool Open(const char * pszFileName, const char * pszFlags);
	virtual void Close();

	virtual bool EndOfFile();

	virtual bool Flush();
	virtual int Read (void * lpBuf, int nCount);
	virtual int Write(const void * lpBuf, int nCount);
	virtual int WriteS(const char * pszStr);

	virtual long GetFileSize();

	virtual long Seek(long lOff, int nFrom);
	virtual long SeekToEnd();
	virtual long Tell();

	// Treat 1 Byte char, not Wide Char, if Error, return -1
	virtual int GetChar();
	virtual int PutChar(int chVal);

	int GetLine(char * pBuffer, int nMaxLen);

	virtual bool IsOpened() const;

	const char * GetFileName() const;

	//
	//	Static Function
	//
	/*
		pCreateTime, pModifiedTime, pAccessTime - NULL 이 아닌곳만 값이 저장된다.
	*/
	static bool GetFileTime(const char * pszPath, INT64 * pCreateTime, INT64 * pAccessTime, INT64 * pModifiedTime);

	static bool IsAccess(const char * pszFile, int nAccessMode = CWFile::accessExist);

	static bool GetFullPathName(const char * pszPath, char * pszFullPath);
	static bool GetFullPathName(const char * pszPath, CWString & strFullPath);

	static bool CopyFile  (const char * pszSour, const char * pszDest);
	static bool RemoveFile(const char * pszPath);

	static bool CreateDir(const char * pszPath);

protected:
	FILE*	m_fp;
	CWString	m_strFileName;
};


/*	CWTimer
*/
class CWTimer : public CWObject
{
public:
	CWTimer(bool bUseMicroSecond = false)
		{
			m_bUseMicroSecond = bUseMicroSecond;
			m_nTimedOut = 0;
		#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
			m_dwStart = 0;
		#else
			m_timeStart.tv_sec = 0;
			m_timeStart.tv_usec = 0;
		#endif
		}

	void SetTimedOut(int nTimedOut)
		{	m_nTimedOut =	nTimedOut;	}

	bool IsTimedOut() const
		{	return (GetEllapsedTime() > m_nTimedOut);	}

	void Start()
		{
		#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
			m_dwStart = GetTickCount();
		#else
			if (gettimeofday(& m_timeStart, NULL) != 0)
			{
				m_timeStart.tv_sec = 0;
				m_timeStart.tv_usec = 0;
			}
		#endif
		}

	INT64 GetEllapsedTime() const
		{
			INT64	nRet;

		#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
			nRet = ((INT64) GetTickCount() - m_dwStart) / CLOCKS_PER_SEC;
			nRet = nRet * 1000;
			if (m_bUseMicroSecond)
				nRet *= 1000;
			return nRet;
		#else
			struct timeval timeCur;
			if (gettimeofday(& timeCur, NULL) != 0)
			{
				timeCur.tv_sec = 0x7FFF;
				timeCur.tv_usec = 0;
			}

			nRet = (INT64) (timeCur.tv_sec - m_timeStart.tv_sec) * 1000 * 1000;
			nRet = nRet + (timeCur.tv_usec - m_timeStart.tv_usec);

			if (! m_bUseMicroSecond)
				nRet /= 1000;
			return (INT64)(nRet);
		#endif
		}

private:
	bool	m_bUseMicroSecond;
	int		m_nTimedOut;

#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
	DWORD	m_dwStart;
#else 
	struct timeval m_timeStart;
#endif
};


/*!
 *****************************************************************************
	CWMutex
 *****************************************************************************
 */
class CWMutex
{
public:
    CWMutex();
	virtual ~CWMutex();

	bool IsCreated();

    bool Create();
    bool Destroy();

	bool Lock();
    bool Unlock();
	bool IsSignaled();

protected:
#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
	HANDLE	m_hMutex;
#else
	friend class CWEvent;
	pthread_mutex_t m_hMutex;
	bool m_bValid;
#endif
};


/*!
 *****************************************************************************
	CWEvent
 *****************************************************************************
 */
class CWEvent
{
public:
    CWEvent();
	virtual ~CWEvent();

	bool IsCreated();

    bool Create(bool bManuslReset);
    bool Destroy();

	bool Set();
    bool Reset();

	bool IsSignaled();

#if (CUR_OS_TYPE != OS_TYPE_WINDOWS)
	bool IsSignaled(CWMutex * pMutex);
#endif

protected:
#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
	HANDLE	m_hEvent;
#else
	pthread_cond_t	m_cond;
	bool	m_bIsSet; // The state of the event variable
	bool	m_bManualReset;
	bool	m_bIsCreated; // False if `cond' is meaningless
#endif
};

//
//	CWThread - Class Object 가 제거되도 Thread 는 종료하지 않는다.
//	프로그램에서 직접 Thread 를 닫고, Handle 을 닫아줘야 한다.
//	WaitUntilKilled 나 Terminate 를 호출하면 자동으로 Handle 을 닫는다.
//
class CWThread
{
public:
	CWThread();
	virtual ~CWThread();

	bool Create(WLIB_THREAD_STARTPROC start_proc, void *pParam);

	//
	//	Handle 을 닫는다고 해서 Thread 가 종료되는것은 아니다.
	//
	void CloseHandle();

	bool SetCPUAffinity(LONG nAffinityMask);

	bool  Terminate(DWORD dwExitCode = 0);

	int   GetPriority(int & min_priority, int & max_priority);
	bool  SetPriority(int nPriority);

	DWORD WaitUntilKilled(int nMaxWaitTimeMilliSec = -1);
	
	bool  IsAlive();

	static bool YieldThread();

public:
	HANDLE_THREAD	m_hThread;
#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
	DWORD	m_dwThreadID;
#else
	bool	m_bValidThread; // False if empty
#	if (CUR_OS_TYPE == OS_TYPE_APPLE)
		thread_act_t mach_thread_id;
#	endif // OS_TYPE_APPLE
#endif
};


//
//	CWLog
//
enum WLogLevel
{
	//
	//	SetLimitLogLevel 및 WriteLogXXXXX 에 사용함.
	WLOGLEVEL_NONE,		//  로그 기록 안함.(시작과 종료는 기록)
	WLOGLEVEL_CRITICAL,
	WLOGLEVEL_ERROR,
	WLOGLEVEL_WARNING,
	WLOGLEVEL_INFO,
	WLOGLEVEL_DEBUG,

	WLOGLEVEL_ALL = 10,

	//	WLOGLEVEL_FORCE 은 SetLimitLogLevel 에서는 무시함.
	//	WriteLogXXXXX 에 사용함.
	WLOGLEVEL_FORCE		= -1,	// 무조건 출력
};


typedef void (CALLBACK * WLOG_WRITE_CALLBACK)
	(
		__in  const char * pszMesg,
		__in  LPVOID pUserData
	);

class CWLog
{
public:
	CWLog(LPCSTR pszLogFile = NULL);
	virtual ~CWLog();

	void SetLogFileName(LPCSTR pszLogFile);

	void SetWriteLogCallback
		(
			__in  WLOG_WRITE_CALLBACK pWriteLogCallback,
			__in  LPVOID pUserData
		);

	void StartLog();
	void ExitLog();

	/*
	GetLimitLogLevel, SetLimitLogLevel
		- 여기에 지정한 Level 보다 낮은 Level 의 Log 만 Write 한다.
		- 숫자는 임의로 정해도 되고, 위에 정의된 enum __WriteLogLevel__ 을 사용해도 된다.

		see WLogLevel
	*/
	int GetLimitLogLevel() const;
	void SetLimitLogLevel(int nLogLevle);

	void SetTimeOption(bool bDate, bool bTime);

	/*
	WriteHead
		인쇄 형태
			###################################################
			WLib Logfile(pszAppName pszVersion)
			Debug Log Name : LogFileName
	*/
	void WriteHead(LPCSTR pszAppName, LPCSTR pszVersion);

	//
	//  Write Log 함수들
	//		bWriteDateTime 이 false 이면 시간과 Title 은 출력되지 않는다.
	//

	/*
	Write
		- 인쇄 형태
			[MM/DD/YYYY hh:mm:ss] pszMesg
	*/
	void Write(int nLogLevel, LPCSTR pszMesg, bool bWriteDateTime = true);

	/*
	WriteF
		- 인쇄 형태
			[MM/DD/YYYY hh:mm:ss] pszFormat, ...
		- Max 4096 Byte
	*/
	void WriteF(int nLogLevel, bool bWriteDateTime, LPCSTR pszFormat, ...);

	/*
	WriteData
		- 인쇄 형태
			[MM/DD/YYYY hh:mm:ss] [ErrCode=code] pszMesg
			bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb   aaaaaaaa
					.............
			bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb   aaaaaaaa
	*/
	void WriteData(int nLogLevel, LPCSTR pszMesg, LPVOID pBuf, int nLen, bool bWriteDateTime = true);

	/*
	WriteMesgOnly
		- 인쇄 형태
			pszMesg
	*/
	void WriteMesgOnly(int nLogLevel, LPCSTR pszMesg);


protected:
	void LOG_TimeAndMesg(LPCSTR pszStr, bool bWriteDateTime = true);
	void LOG_Data(LPVOID pBuf, int nLen);

protected:
	CWFile	m_file;
	CWString	m_strLogFile;
	int		m_nLimitLogLevel;

	bool	m_bStart;

	bool	m_bWriteDate;
	bool	m_bWriteTime;
	WLOG_WRITE_CALLBACK	m_funWriteLogCallback;
	LPVOID	m_pCallbackUserData;
};

//
//	CWLogManager
//
class CWLogManager
{
public:
	CWLogManager();
	virtual ~CWLogManager();

	void StartLog();
	void ExitLog();

	void SetLogFileName(const char * pszLogFileName);

	void SetTimeOption(bool bDate, bool bTime);

	void SetWriteLogCallback
		(
			__in  WLOG_WRITE_CALLBACK pWriteLogCallback,
			__in  LPVOID pUserData
		);

	int GetLimitLogLevel() const;
	void SetLimitLogLevel(int nLevel);

public:
	CWLog	m_wLog;
	int		m_nLogIndent;
};


//
//	CLogWriter
//
class CLogWriter
{
public:
	CLogWriter(CWLogManager * pLogManager, const char * pszAPIName);
	virtual ~CLogWriter();

	virtual void Log(int nLogLevel, const char * pszMesg, bool bWriteDateTime = true);

	void Debug(const char * pszMesg);
	void DebugF(const char * pszFormat, ...);
	void Error(const char * pszMesg);
	void ErrorF(const char * pszFormat, ...);
	void Info(const char * pszMesg);
	void InfoF(const char * pszFormat, ...);

public:
	CWLogManager*	m_pLogManager;
	char	m_szAPIName[256];
};

//	wind library
#include "windlib.template.h"
#include "windlib.comm.h"
#include "windlib.ctrl.h"

//////////////////////////////////////////////////////////////////////
//	inline Function
//

//	CWString
inline CWString::CWString()
	{  Init();	}
inline CWString::CWString(__in const CWString & strSrc)
	{  Init();	CopyFrom((const char *) strSrc);	}
inline CWString::CWString(__in char ch)
	{  Init();	CopyFrom(& ch, sizeof(char));	}
inline CWString::CWString(__in const char * psz)
	{  Init();	CopyFrom(psz);	}
inline CWString::CWString(__in const char * psz, __in int nLen)
	{  Init();	CopyFrom(psz, nLen);	}
inline CWString::CWString(__in const unsigned char * psz)
	{  Init();	CopyFrom((const char *) psz); }
inline CWString::~CWString()
	{  DeleteMem();	}
inline void CWString::Init()
	{  m_pszData = CWString::GetNillString();	}

inline bool CWString::IsValidIndex(int nIndex) const
	{  return (nIndex >= 0 && nIndex < GetLength()) ? true : false;	}

inline int CWString::Compare(__in const CWString & dst) const
	{  return SafeStrCmp(m_pszData, dst.m_pszData, true);	}
inline int CWString::Compare(__in const CWString * pDest) const
	{  return SafeStrCmp(m_pszData, pDest->m_pszData, true);	}
inline int CWString::Compare(__in const char* psz, __in bool bCaseSensitive) const
	{  return SafeStrCmp(m_pszData, psz, bCaseSensitive);	}
inline int CWString::CompareNoCase(__in const char* psz) const
	{  return SafeStrCmp(m_pszData, psz, false);	}

inline int CWString::GetLength() const
	{  return SafeStrLen(m_pszData);	}
inline bool CWString::IsEmpty() const
	{  return (m_pszData && m_pszData[0]) ? false : true;	}
inline void CWString::Empty()
	{	DeleteMem();	m_pszData = CWString::GetNillString();	}

inline char CWString::GetAt(__in int nIndex) const
	{  return IsValidIndex(nIndex) ? m_pszData[nIndex] : '\0';	}
inline char CWString::operator[](__in int nIndex) const
	{  return GetAt(nIndex);	}
inline void CWString::SetAt(__in int nIndex,__in  char ch)
	{  if (IsValidIndex(nIndex))   m_pszData[nIndex] = ch;	}
inline CWString::operator const char*() const
	{  return m_pszData; }

inline const CWString & CWString::operator=(__in const CWString & strSrc)
	{ return CopyFrom((const char *) strSrc);	}
inline const CWString & CWString::operator=(__in char ch)
	{ return CopyFrom(& ch, sizeof(char));	}
inline const CWString & CWString::operator=(__in const char * psz)
	{  return CopyFrom(psz);	}
inline const CWString & CWString::operator=(__in const unsigned char * psz)
	{ return CopyFrom((const char *) psz);	}

inline const CWString & CWString::operator+=(__in const CWString & str)
	{  return ConcatFrom((const char*) str);	}
inline const CWString & CWString::operator+=(__in char ch)
	{  return ConcatFrom(& ch, 1);	}

inline int CWString::Replace(const char* pszOld, const char* pszNew, bool bReplaceOnce )
	{  return Replace(pszOld, pszNew, true, bReplaceOnce);	}
inline int CWString::NoCaseReplace(const char* pszOld, const char* pszNew, bool bReplaceOnce)
	{  return Replace(pszOld, pszNew, false, bReplaceOnce);	}

inline void CWString::MakeUpper()
	{  if (m_pszData)  StrToUpper(m_pszData);	}
inline void CWString::MakeLower()
	{  if (m_pszData)  StrToLower(m_pszData);	}

inline CWString& CWString::Trim()
	{   return TrimRight().TrimLeft();	}
inline CWString& CWString::TrimRight()
	{  if (m_pszData)  StrTrimRight(m_pszData);	return * this;	}
inline CWString& CWString::TrimLeft()
	{  if (m_pszData)  StrTrimLeft(m_pszData);	return * this;	}

inline bool operator==(__in const CWString & str1, __in const CWString & str2)
	{  return (SafeStrCmp(str1.m_pszData, str2.m_pszData, true) == 0);	}
inline bool operator==(__in const CWString & str, __in const char* psz)
	{  return (SafeStrCmp(str.m_pszData, psz, true) == 0);	}
inline bool operator==(__in const char* psz, __in const CWString & str)
	{  return (SafeStrCmp(psz, str.m_pszData, true) == 0);	}
inline bool operator!=(__in const CWString & str1, __in const CWString & str2)
	{  return (SafeStrCmp(str1.m_pszData, str2.m_pszData, true) != 0);	}
inline bool operator!=(__in const CWString & str, __in const char* psz)
	{  return (SafeStrCmp(str.m_pszData, psz, true) != 0);	}
inline bool operator!=(__in const char* psz, __in const CWString & str)
	{  return (SafeStrCmp(psz, str.m_pszData, true) != 0);	}
inline CWString operator+(__in const CWString & str1, __in const CWString & str2)
	{  CWString s = str1;	return s.ConcatFrom(str2);	}
inline CWString operator+(__in const CWString & str, __in char ch)
	{  CWString s = str;	return s.ConcatFrom(& ch, 1);	}
inline CWString operator+(__in char ch, __in const CWString & str)
	{  CWString s = ch;	return s.ConcatFrom(str);	}
inline CWString operator+(__in const CWString & str, __in const char* psz)
	{  CWString s = str;	return s.ConcatFrom(psz);	}
inline CWString operator+(__in const char* psz, __in const CWString & str)
	{  CWString s = psz;	return s.ConcatFrom(str);	}


//	CWFile
inline CWFile::CWFile()
	{	m_fp = NULL;	}
inline CWFile::CWFile(const char * pszFileName, const char * pszOpenFlags)
	{	m_fp = NULL;	Open(pszFileName, pszOpenFlags);	}
inline CWFile::~CWFile() 
	{	if (IsOpened())	Close();	}
inline bool CWFile::IsOpened() const
	{ return (m_fp != NULL);	}
inline const char * CWFile::GetFileName() const
	{	return m_strFileName;	}
inline long CWFile::Tell()
	{	return ftell(m_fp);	}
inline long CWFile::SeekToEnd()
	{	return Seek(0, CWFile::seekEnd);	}
inline bool CWFile::Flush()
	{	return (::fflush(m_fp) == 0);	}
inline bool CWFile::EndOfFile()
	{	return IsOpened() ? (feof(m_fp) != 0) : true;	}

//	CWFile static function
inline bool CWFile::IsAccess(const char * pszPath, int nAccessMode)
	{
	#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
		return ::GetFileAttributesA(pszPath) != (DWORD)0xFFFFFFFF ? true : false;
	#else
		return (access(pszPath, nAccessMode) == 0);
	#endif
	}
inline  bool CWFile::RemoveFile(const char * pszFile)
	{
	#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
		return (::DeleteFileA(pszFile) == TRUE);
	#else
		return (::unlink(pszFile) == 0);
	#endif
}

//	CWLog
inline int CWLog::GetLimitLogLevel() const
	{	return m_nLimitLogLevel;	}
inline void CWLog::SetLimitLogLevel(int nLevel)
	{	if (nLevel != WLOGLEVEL_FORCE) m_nLimitLogLevel = nLevel;	}
inline void CWLog::SetTimeOption(bool bDate, bool bTime)
	{	m_bWriteDate = bDate;	m_bWriteTime = bTime;	}

//CWLogManager
inline CWLogManager::CWLogManager()
	{
		m_wLog.SetLimitLogLevel(WLOGLEVEL_WARNING);
		m_nLogIndent = -1;
	}
inline CWLogManager::~CWLogManager()
	{	m_wLog.ExitLog();	}
inline void CWLogManager::StartLog()
	{	m_wLog.StartLog();	}
inline void CWLogManager::ExitLog()
	{	m_wLog.ExitLog();	}
inline void CWLogManager::SetWriteLogCallback(WLOG_WRITE_CALLBACK WLOG_WRITE_CALLBACK, LPVOID pUserData)
	{	m_wLog.SetWriteLogCallback(WLOG_WRITE_CALLBACK, pUserData);	}
inline void CWLogManager::SetLogFileName(const char * pszLogFileName)
	{	m_wLog.SetLogFileName(pszLogFileName);	}
inline int CWLogManager::GetLimitLogLevel() const
	{	return m_wLog.GetLimitLogLevel();	}
inline void CWLogManager::SetLimitLogLevel(int nLevel)
	{	m_wLog.SetLimitLogLevel(nLevel);	}
inline void CWLogManager::SetTimeOption(bool bDate, bool bTime)
	{	m_wLog.SetTimeOption(bDate, bTime);	}

//	CLogWriter
inline void CLogWriter::Debug(const char * pszMesg)
	{	Log(WLOGLEVEL_DEBUG, pszMesg);	}
inline void CLogWriter::Info(const char * pszMesg)
	{	Log(WLOGLEVEL_INFO, pszMesg);	}
inline void CLogWriter::Error(const char * pszMesg)
	{	Log(WLOGLEVEL_ERROR, pszMesg);	}

#endif  // __WINDLIB_H
