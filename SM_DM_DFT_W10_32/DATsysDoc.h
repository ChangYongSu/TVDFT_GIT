// DATsysDoc.h : interface of the CDATsysDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATSYSDOC_H__A91C394A_305C_11D3_90A6_006008CED7F4__INCLUDED_)
#define AFX_DATSYSDOC_H__A91C394A_305C_11D3_90A6_006008CED7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDATsysDoc : public CDocument
{
protected: // create from serialization only
	CDATsysDoc();
	DECLARE_DYNCREATE(CDATsysDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDATsysDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

public:
	CString		 m_szTargetIpAddress;	
	UINT		 m_nSendPortNo;
	UINT		 m_nReceivePortNo;

// Implementation
public:
	virtual ~CDATsysDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDATsysDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATSYSDOC_H__A91C394A_305C_11D3_90A6_006008CED7F4__INCLUDED_)
