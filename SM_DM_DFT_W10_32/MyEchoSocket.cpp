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

	
	CAsyncSocket::OnClose(nErrorCode);
}

void MyEchoSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	
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
		g_pView->OnSocketReceive(); 
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
