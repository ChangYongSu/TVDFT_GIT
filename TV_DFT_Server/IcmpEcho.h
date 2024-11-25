#if !defined(AFX_ICMPECHO_H__050EBBEF_5A35_11D3_B954_204C4F4F5020__INCLUDED_)
#define AFX_ICMPECHO_H__050EBBEF_5A35_11D3_B954_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IcmpEcho.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// IcmpEcho command target

#define PING_TIMEOUT 100

class IcmpEcho : public CSocket
{
// Attributes
public:

// Operations
public:
	IcmpEcho();
	virtual ~IcmpEcho();

	unsigned long ResolveIP( CString strIP );
	unsigned long ResolveName( CString strHostName );
	DWORD PingHost( unsigned long ulIP, int iPingTimeout );
	DWORD PingHostOK( unsigned long ulIP, int iPingTimeout );//PingHostOK( unsigned long ulIP, int iPingTimeout, CString RtnStr )
	CString GetIP( unsigned long ulIP );

// Overrides
public:
	CString RtnStr;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IcmpEcho)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(IcmpEcho)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICMPECHO_H__050EBBEF_5A35_11D3_B954_204C4F4F5020__INCLUDED_)
