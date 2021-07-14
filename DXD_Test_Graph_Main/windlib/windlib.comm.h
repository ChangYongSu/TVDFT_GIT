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
 
#if !defined(__WINDLIB_COMM_H)
#define  __WINDLIB_COMM_H

#pragma once

#if !defined(__WINDLIB_H)
#error 'This header is subset of windlib.h.  use include windlib.h'
#endif

#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)

#	include <direct.h>

#	define	SleepMilliSec(mSec)		Sleep((mSec))

#else // not Windows

#	include <unistd.h> 
#	include <fcntl.h>
#	include <getopt.h>
#	include <errno.h>
#	include <pthread.h>
#	include <termios.h>
#	include <sys/types.h> 
#	include <sys/stat.h> 
#	include <sys/ioctl.h> 
#	include <sys/socket.h> 
#	include <netinet/in.h> 
#	include <netinet/ip.h>   /* iphdr */
#	include <netinet/tcp.h>  /* tcphdr */
#	include <net/if.h> 
#	include <arpa/inet.h> 
#	include <netdb.h>

#	define	SleepMilliSec(mSec)		usleep((mSec) * 1000)

typedef int		SOCKET;

#endif // not Windows


#ifndef  INVALID_SOCKET
#	define INVALID_SOCKET			(SOCKET)(~0)
#endif

#ifndef  SOCKET_ERROR
#	define SOCKET_ERROR		-1
#endif

//////////////////////////////////////////////////////////////////////
//	Class Hierarchy chart in this library
//
//
class WINDAPI CWSocket;
	class WINDAPI CWTCPClient;
	class WINDAPI CWTCPServer;
;

enum  __WCommError
{
#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
	WCOMM_TIMEDOUT		= WSAETIMEDOUT,
	WCOMM_WOULDBLOCK	= WSAEWOULDBLOCK,
	WCOMM_DISCONN		= WSAEDISCON,
	WCOMM_CANCELED		= WSAECANCELLED,
#else
	WCOMM_TIMEDOUT		= ETIMEDOUT,
	WCOMM_WOULDBLOCK	= EAGAIN,
	WCOMM_DISCONN		= ENOTCONN,
	WCOMM_CANCELED		= ECANCELED,
#endif

};

class CWSocket
{
public:
	enum BlockingMode
	{
		BLOCK_READ,
		BLOCK_WRITE,
		BLOCK_EXCEPT,
	};

	enum  __GetBuffSizeType
	{
		SEND_BUF,
		RECV_BUF,
	};

	// 프로그램 시작시와 종료시 한번만 호출한다.
	static int StartNetwork();
	static int EndNetwork();

	CWSocket();
	virtual ~CWSocket();

	bool IsValidSocket() const;

	bool Attach(SOCKET soc);
	SOCKET Detach();
		// Caution ::::::
		// if do not detach, the soc is automatically closed in Destructor.....

	int Shutdown();
	int CloseSocket();
	int CheckBlocking(__in BlockingMode nMode);
	int SetNonBlockingMode(__in bool bNonBlocking = true);
	void DisableNagel();
	void ReuseAddress(__in bool bReuse = true);
	int SetLingerOption(__in bool bOnOff = true, __in int nTimedOut = 0);

	int SetBufSize(__in int nType, __in int nBufSize);
	int GetBufSize(__in int nType, __out int * pnBufSize);

	/*
	Send, Recv, RecvExact
		- nTimedOut 이 지정되어 있고 TimedOut 이 발생하면 Bloking 상태일때 WCOMM_TIMEDOUT 이 반환된다.
		- nTimedOut 이 지정되어 있지 않으면( < 0) Bloking 상태일때 WCOMM_WOULDBLOCK 이 반환된다.
	*/

	/*Recv
		- return 0 : success
	*/
	int Send
		(
			__in const void * pData,
			__in int nLen,
			__in_opt int nTimedOut = -1,
			__in_opt CWEvent * pEventCancel = NULL
		);

	/*Recv
		- pnLen 에 pData 의 버퍼 크기가 들어 있어야 함.
		- return 0 : success
	*/
	int Recv
		(
			__out void * pData,
			__inout int * pnLen,
			__in_opt int nTimedOut = -1,
			__in_opt CWEvent * pEventCancel = NULL
		);
	int RecvExact
		(
			__out void * pBuffer,
			__in int nLen,
			__in_opt int nTimedOut = -1,
			__in_opt CWEvent * pEventCancel = NULL
		);

	operator SOCKET();

	static u_long GetHostAddress(const char * pszHostName);
	static bool GetPeerName(SOCKET soc, char * pszBuf, int cbBuf, unsigned short * puPeerPort);

	static bool IsValidSocket(__in SOCKET soc);

	static int Shutdown(__in SOCKET soc);
	static int CloseSocket(__in SOCKET soc);
	static int CheckBlocking(__in SOCKET soc, __in BlockingMode nReadWriteMode);
	static int SetNonBlockingMode(__in SOCKET soc, __in bool bNonBlocking);

	static void DisableNagel(__in SOCKET soc);
	static void ReuseAddress(__in SOCKET soc, __in bool bReuse /*= true*/);
	static int SetLingerOption(__in SOCKET soc, __in bool bOnOff /*= true*/, __in int nTimedOut /*= 0*/);

	static int SetBufSize(__in SOCKET soc, __in int nType, __in int nBufSize);
	static int GetBufSize(__in SOCKET soc, __in int nType, __out int * pnBufSize);

	static int Send
		(
			__in SOCKET soc,
			__in const void * pData,
			__in int nLen,
			__in int nTimedOut /*= -1*/,
			__in CWEvent * pEventCancel /*= NULL*/
		);
	static int Recv
		(
			__in SOCKET soc,
			__out void * pData,
			__inout int * pnLen,
			__in int nTimedOut /*= -1*/,
			__in CWEvent * pEventCancel /*= NULL*/
		);
	static int RecvExact
		(
			__in SOCKET soc,
			__out void * pBuffer,
			__in int nLen,
			__in int nTimedOut /*= -1*/,
			__in CWEvent * pEventCancel /*= NULL*/
		);

	static int GetLastError();
	static bool CheckErrorWouldBlock(int nErr);
	static bool CheckErrorConnected(int nErr);
	static bool CheckErrorInvalidArgs(int nErr);

public:
	SOCKET	m_soc;
};

//
//	CWTCPClient
//
class CWTCPClient : public CWSocket
{
public:
	CWTCPClient();
	virtual ~CWTCPClient();

	/*
	Connect
		여기서 연결되는 Socket 은
			1. Non-Blocking Mode.
			2. Linger(on, time=0) 으로 설정된다.
		설정을 변경 하려면 Connect 성공 후 
			SetNonBlockingMode(...) 
			SetLingerOption(...) 을 사용한다.
	*/
	int Connect(u_long nIpAddr, int nPortNum, int nTimedOut);
	int Connect(const char * pszHostName, int nPortNum, int nTimedOut);
};


//
//	CWTCPServer
//
class CWTCPServer : public CWSocket
{
public:
	CWTCPServer();
	virtual ~CWTCPServer();
	
	int Start(int nPortNum, int nBackLog = 5);

	/*
	WaitClient
		여기서 연결되는 Client 의 Socket 은
			1. Non-Blocking Mode.
			2. ReuseAddress Mode.
		설정을 변경 하려면 Connect 성공 후 
			CWSocket::SetNonBlockingMode(...) 
			CWSocket::ReuseAddress(...)
	*/
	int WaitClient(SOCKET * pSocClient, char * pszCliName = NULL);
};


//////////////////////////////////////////////////////////////////////
//	inline Function
//


#if (CUR_OS_TYPE == OS_TYPE_WINDOWS)
	inline int CWSocket::GetLastError()
		{	return (int) WSAGetLastError();	}
	inline bool CWSocket::CheckErrorConnected(int nErr)
		{	return (nErr == WSAEISCONN);	}
	inline bool CWSocket::CheckErrorWouldBlock(int nErr)
		{	return ((nErr == WSAEWOULDBLOCK) || (nErr == WSAEALREADY) ||
				(nErr == WSAEINPROGRESS)); }
	inline bool CWSocket::CheckErrorInvalidArgs(int nErr)
		{	return (nErr == WSAEINVAL);	}
#else
	inline int CWSocket::GetLastError()
		{	return errno;	}
	inline bool CWSocket::CheckErrorConnected(int nErr)
		{	return (nErr == EISCONN); }
	inline bool CWSocket::CheckErrorWouldBlock(int nErr)
		{	return ((nErr == EWOULDBLOCK) || (nErr == EAGAIN) || (nErr == EALREADY) ||
				(nErr == EINPROGRESS));	}
	inline bool CWSocket::CheckErrorInvalidArgs(int nErr)
		{	return (nErr == EINVAL); }
#endif


//	CWSocket
inline bool CWSocket::IsValidSocket() const
	{	return (m_soc != INVALID_SOCKET);	}
inline bool CWSocket::IsValidSocket(SOCKET soc)
	{	return (soc != INVALID_SOCKET);	}

inline CWSocket::operator SOCKET()
	{  return m_soc;	}

inline int CWSocket::Shutdown()
	{	return CWSocket::Shutdown(m_soc);	}
inline int CWSocket::CheckBlocking(__in BlockingMode nReadWriteMode)
	{	return CWSocket::CheckBlocking(m_soc, nReadWriteMode);	}
inline int CWSocket::SetNonBlockingMode(__in bool bNonBlocking)
	{  return CWSocket::SetNonBlockingMode(m_soc, bNonBlocking);	}
inline void CWSocket::ReuseAddress(bool bReuse)
	{	CWSocket::ReuseAddress(m_soc, bReuse);	}
inline int CWSocket::SetLingerOption(bool bOnOff, int nTimedOut)
	{	return CWSocket::SetLingerOption(m_soc, bOnOff, nTimedOut);	}
inline void CWSocket::DisableNagel()
	{	CWSocket::DisableNagel(m_soc);	}

inline int CWSocket::SetBufSize(int nType, int nBufSize)
	{  return CWSocket::SetBufSize(m_soc, nType, nBufSize);	}
inline int CWSocket::GetBufSize(int nType, int * pnBufSize)
	{  return CWSocket::GetBufSize(m_soc, nType, pnBufSize);	}
inline int CWSocket::Send(const void * pData, int nLen, int nTimedOut, CWEvent * pEventCancel)
	{  return CWSocket::Send(m_soc, pData, nLen, nTimedOut, pEventCancel);	}
inline int CWSocket::Recv(void * pData, int * pnLen, int nTimedOut, CWEvent * pEventCancel)
	{  return CWSocket::Recv(m_soc, pData, pnLen, nTimedOut, pEventCancel);	}
inline int CWSocket::RecvExact(void * pData, int nLen, int nTimedOut, CWEvent * pEventCancel)
	{  return CWSocket::RecvExact(m_soc, pData, nLen, nTimedOut, pEventCancel);	}

//	CWTCPClient
inline CWTCPClient::CWTCPClient()
	{}
inline CWTCPClient::~CWTCPClient()
	{}
inline int CWTCPClient::Connect(const char * pszHostName, int nPortNum, int nTimedOut)
	{	return CWTCPClient::Connect(CWSocket::GetHostAddress(pszHostName), nPortNum, nTimedOut);	}

//	CWTCPServer
inline CWTCPServer::CWTCPServer()
	{}
inline CWTCPServer::~CWTCPServer()
	{}

#endif	//  __WINDLIB_COMM_H
