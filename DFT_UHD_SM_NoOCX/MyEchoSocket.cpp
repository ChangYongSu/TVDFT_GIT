// MyEchoSocket.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "MyEchoSocket.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDATsysView*			g_pView;
/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket

MyEchoSocket::MyEchoSocket()
{
}

MyEchoSocket::~MyEchoSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(MyEchoSocket, CSocket)
	//{{AFX_MSG_MAP(MyEchoSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// MyEchoSocket member functions

void MyEchoSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(nErrorCode==0)
//	{
//		((CEchoServerDlg*)m_pDlg)->OnAccept();
//	}
	CAsyncSocket::OnAccept(nErrorCode);
}

void MyEchoSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(nErrorCode==0)
//	{
//		((CEchoServerDlg*)m_pDlg)->OnClose(); 
//	}
	m_SerConnected = 0;
	
	CAsyncSocket::OnClose(nErrorCode);
}

void MyEchoSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_SerConnected = 1;

	CAsyncSocket::OnConnect(nErrorCode);
}

void MyEchoSocket::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void MyEchoSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode==0)
	{
		if (m_SerVerDeviceID == _SERVER_PLC_ID)
		{
			g_pView->OnPLCSocketReceive();
		}
		else if (m_SerVerDeviceID == _SERVER_MONITOR_CONTROL_ID)
		{
			g_pView->OnMonitorSocketReceive();
		}
		else if(m_SerVerDeviceID == _SERVER_GMES_ID)//		0 
		{
			g_pView->OnSocketReceive();
		}
			
		//#define 		1 
		
	}
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void MyEchoSocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnSend(nErrorCode);
}

//void MyEchoSocket::SetParentDlg(CDialog *pDlg)
//{
//m_pDlg=pDlg;
//}
