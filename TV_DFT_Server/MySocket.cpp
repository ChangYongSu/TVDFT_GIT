// MySocket.cpp : implementation file
//

#include "stdafx.h"
//#include "Sock.h"
#include "AcqVoltageSamples_IntClk.h"
#include "MySocket.h"
//#include "SockDlg.h"

#include "AcqVoltageSamples_IntClkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySocket

CMySocket::CMySocket()
{

	m_SocketOpen = 0;
	m_SocketID = 100;
}

CMySocket::~CMySocket()
{
	Close();

}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMySocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMySocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMySocket member functions

void CMySocket::SetParent(CDialog *pWnd)
{
	// Set the member pointer
	m_pWnd = pWnd;
}

void CMySocket::OnAccept(int nErrorCode)
{
	// Were there any errors?
	if(nErrorCode == 0)
		// no, call the dialog's OnAceept fuction
		( (CAcqVoltageSamples_IntClkDlg*) m_pWnd)->OnShockAccept(m_SocketID);

}

void CMySocket::OnConnect(int nErrorCode)
{
	if(nErrorCode == 0)
		// no, call the dialog's OnConnect fuction
		( (CAcqVoltageSamples_IntClkDlg*) m_pWnd)->OnShockConnect(m_SocketID);


}

void CMySocket::OnOutOfBandData(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class

	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CMySocket::OnClose(int nErrorCode)
{
//	if(nErrorCode == 0)

		// no, call the dialog's OnClose fuction
		( (CAcqVoltageSamples_IntClkDlg*) m_pWnd)->OnShockClose(m_SocketID);

	m_SocketOpen = 0;

}

void CMySocket::OnReceive(int nErrorCode)
{
	if(nErrorCode == 0)
		// no, call the dialog's OnReceivve fuction
		( (CAcqVoltageSamples_IntClkDlg*) m_pWnd)->OnShockReceive(m_SocketID);


}

void CMySocket::OnSend(int nErrorCode)
{
	if(nErrorCode == 0)
		// no, call the dialog's OnSend fuction
		( (CAcqVoltageSamples_IntClkDlg*) m_pWnd)->OnShockSend(m_SocketID);


}
