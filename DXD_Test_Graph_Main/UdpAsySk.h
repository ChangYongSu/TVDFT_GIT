#if !defined(AFX_UDPASYSK_H__20F99C82_8C83_11D1_A121_0000F875C45F__INCLUDED_)
#define AFX_UDPASYSK_H__20F99C82_8C83_11D1_A121_0000F875C45F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UdpAsySk.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CUdpAsySk command target

class CUdpAsySk : public CAsyncSocket
{
// Attributes
public:
	CDialog* m_pDlg; //back pointers to the main dialog

	CString		m_sendBuffer;   //for async send
	int			m_nBytesSent;
	int			m_nBytesBufferSize;

	int		DoAsyncSendBuff();
	int		m_Connected;

// Operations
public:
	CUdpAsySk(CDialog* pDlg);
	virtual ~CUdpAsySk();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUdpAsySk)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUdpAsySk)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPASYSK_H__20F99C82_8C83_11D1_A121_0000F875C45F__INCLUDED_)
