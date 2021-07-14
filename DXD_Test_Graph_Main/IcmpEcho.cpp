// IcmpEcho.cpp : implementation file
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "IcmpEcho.h"

extern "C" {
#include "ipexport.h"
#include "icmpapi.h"
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IcmpEcho

IcmpEcho::IcmpEcho()
{
}

IcmpEcho::~IcmpEcho()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(IcmpEcho, CSocket)
	//{{AFX_MSG_MAP(IcmpEcho)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// IcmpEcho member functions

unsigned long IcmpEcho::ResolveIP( CString strIP )
{
	unsigned long ulIP;

	ulIP = (IPAddr)inet_addr( strIP );
	return ulIP;
}

unsigned long IcmpEcho::ResolveName( CString strHostName )
{
	unsigned long ulIP;
	hostent *phostent;

	phostent = gethostbyname( strHostName );

	if ( phostent == NULL ) return 0;
	ulIP = *(DWORD*)(*phostent->h_addr_list);

	return ulIP;
}

CString IcmpEcho::GetIP( unsigned long ulIP )
{
	LPSTR szAddr;

	struct in_addr inetAddr;
	inetAddr.s_addr = (IPAddr)ulIP;
	szAddr = inet_ntoa( inetAddr );
	CString csIP = szAddr;

	return csIP;
}

DWORD IcmpEcho::PingHost( unsigned long ulIP, int iPingTimeout )
{
	unsigned long ip = ulIP;
	HANDLE icmphandle = IcmpCreateFile();

	char reply[ sizeof( icmp_echo_reply ) + 8 ];

	icmp_echo_reply *iep = (icmp_echo_reply *)&reply;
	iep->RoundTripTime = 0xffffffff;

	IcmpSendEcho( icmphandle, ip, 0, 0, NULL, reply, sizeof( icmp_echo_reply ) + 8, iPingTimeout );
	IcmpCloseHandle( icmphandle );

	return iep->RoundTripTime;
}


DWORD IcmpEcho::PingHostOK( unsigned long ulIP, int iPingTimeout )
{
	unsigned long ip = ulIP;
	HANDLE icmphandle = IcmpCreateFile();

	char reply[ sizeof( icmp_echo_reply ) + 8 ];

	icmp_echo_reply *iep = (icmp_echo_reply *)&reply;
	iep->RoundTripTime = 0xffffffff;
	DWORD dwRetVal = 0;
	dwRetVal =  IcmpSendEcho( icmphandle, ip, 0, 0, NULL, reply, sizeof( icmp_echo_reply ) + 8, iPingTimeout );
	IcmpCloseHandle( icmphandle );

	//ICMP_ECHO_REPLY * p = (ICMP_ECHO_REPLY *)reply;
	unsigned long returnTime = 10000; 
	CString str ;
	CString str1 ;
	/*str.Format(_T("st Address %d\n"),p->Address);
	str1 += str;
	str.Format(_T("st Status %d\n"),p->Status);
	str1 += str;
	str.Format(_T("st RoundTripTime %d\n"),p->RoundTripTime);
	str1 += str;
	str.Format(_T("st DataSize %d\n"),p->DataSize);
	str1 += str;
	str.Format(_T("st Reserved %d\n"),p->Reserved);
	str1 += str;
	str.Format(_T("st Data %s\n"),p->Data);*/
	switch(iep->Status)
	{
	case IP_SUCCESS:
		str1 = _T("The status was success.");
		returnTime = iep->RoundTripTime;
		break;
 
	case IP_BUF_TOO_SMALL://11001  
		str1 = _T("The reply buffer was too small.");
		//return 10000;
		break;
 
	case IP_DEST_NET_UNREACHABLE://11002 
		str1 = _T("The destination network was unreachable. ");
		//return 10000;
		break;
 
	case IP_DEST_HOST_UNREACHABLE://11003 
		str1 = _T("The destination host was unreachable. ");
		//return 10000;
		break;
 
	case IP_DEST_PROT_UNREACHABLE://11004 
		str1 = _T("The destination protocol was unreachable. ");
		//return 10000;
		break;
 
	case IP_DEST_PORT_UNREACHABLE://11005 
		str1 = _T("The destination port was unreachable. ");
		//return 10000;
		break;
 
	case IP_NO_RESOURCES://11006 
		str1 = _T("Insufficient IP resources were available.");
		//return 10000;
		break;
 
	case IP_BAD_OPTION ://11007 
		str1 = _T("A bad IP option was specified.");
		//return 10000;
		break;
 
	case IP_HW_ERROR://11008 
		str1 = _T("A hardware error occurred.");
		//return 10000;
		break;
 
	case IP_PACKET_TOO_BIG://11009 
		str1 = _T("The packet was too big.");
		//return 10000;
		break;
 
	case IP_REQ_TIMED_OUT://11010 
		str1 = _T("The request timed out.");
		//return 10000;
		break;
 
	case IP_BAD_REQ://11011 
		str1 = _T("A bad request.");
		return 10000;
		break;
 
	case IP_BAD_ROUTE://11012 
		str1 = _T("A bad route.");
		//return 10000;
		break;
 
	case IP_TTL_EXPIRED_TRANSIT://11013 
		str1 = _T("The time to live (TTL) expired in transit. ");
		//return 10000;
		break;
 
	case  IP_TTL_EXPIRED_REASSEM://11014 
		str1 = _T("The time to live expired during fragment reassembly.");
		//return 10000;
		break;
 
	case IP_PARAM_PROBLEM://11015 
		str1 = _T("A parameter problem. ");
		//return 10000;
		break;
 
	case IP_SOURCE_QUENCH://11016 
		str1 = _T("Datagrams are arriving too fast to be processed and datagrams may have been discarded.");
		//return 10000;
		break;
 
	case IP_OPTION_TOO_BIG://11017 
		str1 = _T("An IP option was too big. ");
		//return 10000;
		break;
 
	case  IP_BAD_DESTINATION://11018 
		str1 = _T("A bad destination. ");
		//return 10000;
		break;
 
	case IP_GENERAL_FAILURE://11050 
		str1 = _T("A general failure. This error can be returned for some malformed ICMP packets");
		//return 10000;
		break;
	default:
		return 10000;
		break;
	}

//	if(returnTime >= 100)
		RtnStr = str1;
		

	return returnTime;

}