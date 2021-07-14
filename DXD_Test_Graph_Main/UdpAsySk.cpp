// UdpAsySk.cpp : implementation file
//

#include "stdafx.h"
#include "UDpAsySk.h"
#include "AcqVoltageSamples_IntClk.h"
#include "AcqVoltageSamples_IntClkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUdpAsySk

CUdpAsySk::CUdpAsySk(CDialog* pDlg)
{
	m_pDlg = pDlg;
	m_sendBuffer = "";   //for async send
	m_nBytesSent = 0;
	m_nBytesBufferSize = 0;
	m_Connected = 0;
}

CUdpAsySk::~CUdpAsySk()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUdpAsySk, CAsyncSocket)
	//{{AFX_MSG_MAP(CUdpAsySk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUdpAsySk member functions

void CUdpAsySk::OnAccept(int nErrorCode) 
{
	OutputDebugString("CUdpAsySk::OnAccept\n");
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CUdpAsySk::OnClose(int nErrorCode) 
{


	OutputDebugString("CUdpAsySk::OnClose\n");
	((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_pUDPSocket = NULL;
	
	CAsyncSocket::OnClose(nErrorCode);

//	((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_GMES_Alive_WaitCnt = 3000;
	delete this;
}

void CUdpAsySk::OnConnect(int nErrorCode) 
{
	OutputDebugString("CUdpAsySk::OnConnect\n");
	if(nErrorCode == 0)
	{
		m_Connected = 1;
	}
	else 
	{
		m_Connected = 0;
		switch(nErrorCode)
		{
			case WSAEPROTOTYPE:
				AfxMessageBox("A protocol was specified in the socket function call that does not support the semantics of the socket type requested.");
				break;
			case WSAENOPROTOOPT:
				AfxMessageBox("An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call.");
				break;
			case WSAEPROTONOSUPPORT:
				AfxMessageBox("The requested protocol has not been configured into the system, or no implementation for it exists.");
				break;
			case WSAESOCKTNOSUPPORT :
				AfxMessageBox("The support for the specified socket type does not exist in this address family.");
				break;
			case  WSAEOPNOTSUPP :
				AfxMessageBox("The attempted operation is not supported for the type of object referenced.");
				break;
			case WSAEPFNOSUPPORT :
				AfxMessageBox("The protocol family has not been configured into the system or no implementation for it exists.");
				break;
			case WSAEAFNOSUPPORT :
				AfxMessageBox("An address incompatible with the requested protocol was used.");
				break;
			case  WSAEADDRINUSE  :
				//AfxMessageBox(" Only one usage of each socket address (protocol/network address/port) is normally permitted.");
				
				g_nLocalPortNumber++;
				if(g_nLocalPortNumber > 7500)
				{
					g_nLocalPortNumber = 7000;
				}
				g_nPortReopen = 1;//((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_EthernetMode =  ETHERNET_MODE_CREATE_SOCKET;
				break;
			case  WSAEADDRNOTAVAIL :
				AfxMessageBox("The requested address is not valid in its context.");
				break;
			case  WSAENETDOWN :
				AfxMessageBox(" A socket operation encountered a dead network.");
				break;
			case  WSAENETUNREACH   :
				AfxMessageBox(" A socket operation was attempted to an unreachable network.");
				break;
			case  WSAENETRESET  :
				AfxMessageBox("The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.");
				break;
			case WSAECONNABORTED  :
				AfxMessageBox(" An established connection was aborted by the software in your host machine.");
				break;
			case  WSAECONNRESET   :
				AfxMessageBox(" An existing connection was forcibly closed by the remote host.");
				break;
			case  WSAENOBUFS    :
				AfxMessageBox(" An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.");
				break;
			case WSAEISCONN   :
				AfxMessageBox("A connect request was made on an already connected socket.");
				break;
			case  WSAENOTCONN    :
				AfxMessageBox(" A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied.");
				break;
			case  WSAESHUTDOWN   :
				AfxMessageBox("A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call.");
				break;
			case  WSAETOOMANYREFS  :
				AfxMessageBox("Too many references to some kernel object.");
				break;
			case  WSAETIMEDOUT     :
				AfxMessageBox(" A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond.");
				break;
			case  WSAECONNREFUSED      :
				AfxMessageBox("  No connection could be made because the target machine actively refused it.");
				break;
			default:
				break;
		}
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CUdpAsySk::OnOutOfBandData(int nErrorCode) 
{
	OutputDebugString("CUdpAsySk::OnOutOfBandData\n");
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CUdpAsySk::OnReceive(int nErrorCode) 
{
	OutputDebugString("CUdpAsySk::OnReceive\n");
	TCHAR buff[70000];
	int nRead;

	//((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->UpdateData(TRUE);
	CString  lstrIPaddress;
	UINT lPort = 0;
	nRead = ReceiveFrom(buff, 69999, lstrIPaddress, lPort); //make it smaller so we can experiment mulitple notifications
	switch (nRead)
	{
	case 0:
		Close();
		break;

	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK) 
		{
			if (GetLastError() != WSAEMSGSIZE)
			{
				TCHAR szError[256];
				wsprintf(szError, "OnReceive error: %d", GetLastError());
				AfxMessageBox (szError);
			}
			else
			{
				AfxMessageBox ("The datagram was too large and was truncated");
				CString szTemp(buff);
				((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_strEthernetReceive += szTemp;
				((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->OnParseDataEthernet();
				//((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_strReceive += szTemp;
				//((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->UpdateData(FALSE);
			}
		}
		break;

	default:
		if (nRead != SOCKET_ERROR && nRead != 0 )
		{
			TCHAR szError[256];
			wsprintf(szError, "OnReceive bytes: %d", nRead);
			//AfxMessageBox (szError);
			
			buff[nRead] = 0; //terminate the string
			CString szTemp(buff);
			((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_strEthernetReceive += szTemp;			
			((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->OnParseDataEthernet();

			//((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_strReceive += szTemp;
			//((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->UpdateData(FALSE);
		}	
	}
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CUdpAsySk::OnSend(int nErrorCode) 
{
	OutputDebugString("CUdpAsySk::OnSend\n");
	DoAsyncSendBuff();	
	CAsyncSocket::OnSend(nErrorCode);
}

int CUdpAsySk::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	OutputDebugString("CUdpAsySk::Receive\n");
	
	return CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
}

int CUdpAsySk::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	OutputDebugString("CUdpAsySk::Send\n");
	
	return CAsyncSocket::Send(lpBuf, nBufLen, nFlags);
}

int CUdpAsySk::DoAsyncSendBuff()
{
	int lResult = 0;
	 
	while (m_nBytesSent < m_nBytesBufferSize)  //m_nBytesBufferSize should be m_sendBuffer.GetLength() +1
	{
		int dwBytes;

		//if (((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_chkBroadcast)
		//{
		//	SOCKADDR_IN remoteAddr;
		//	remoteAddr.sin_family = AF_INET;
		//	remoteAddr.sin_port = htons(((CMsocudpDlg*)m_pDlg)->m_remoteport);
		//	remoteAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		//	dwBytes = SendTo((LPCTSTR)m_sendBuffer + m_nBytesSent, m_nBytesBufferSize - m_nBytesSent, (const SOCKADDR* )&remoteAddr, sizeof(remoteAddr));
		//}
		//else 
		{
//			dwBytes = SendTo((LPCTSTR)m_sendBuffer + m_nBytesSent, m_nBytesBufferSize - m_nBytesSent, 
//				((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->g_nRemotePortNumber, (LPCTSTR)((CAcqVoltageSamples_IntClkDlg*)m_pDlg)->m_strRemote);
			dwBytes = SendTo((LPCTSTR)m_sendBuffer + m_nBytesSent, m_nBytesBufferSize - m_nBytesSent, 
				g_nRemotePortNumber, (LPCTSTR)g_nRemoteIPAddress);
		}
		if (dwBytes == SOCKET_ERROR)
		{
			if (GetLastError() == WSAEWOULDBLOCK)
			{
				lResult = 1;
					break;
			}
			else
			{
				TCHAR szError[256];
				wsprintf(szError, "Server Socket failed to send: %d", GetLastError());
				Close();
				AfxMessageBox (szError);
				lResult = 1;
				break;
			}

		}
		else
		{
			m_nBytesSent += dwBytes;
		}
	}

	if (m_nBytesSent == m_nBytesBufferSize)
	{
		m_nBytesSent = m_nBytesBufferSize = 0;
		m_sendBuffer = "";
	}

	return lResult;
}

