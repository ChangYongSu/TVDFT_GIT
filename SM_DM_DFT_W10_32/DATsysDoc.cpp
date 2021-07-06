// DATsysDoc.cpp : implementation of the CDATsysDoc class
//

#include "stdafx.h"
#include "DATsys.h"

#include "DATsysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDATsysDoc

IMPLEMENT_DYNCREATE(CDATsysDoc, CDocument)

BEGIN_MESSAGE_MAP(CDATsysDoc, CDocument)
	//{{AFX_MSG_MAP(CDATsysDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDATsysDoc construction/destruction

CDATsysDoc::CDATsysDoc()
{
	// TODO: add one-time construction code here
	m_szTargetIpAddress	= _T("");	
	m_nSendPortNo		= 0;
	m_nReceivePortNo	= 0;
}

CDATsysDoc::~CDATsysDoc()
{

}

BOOL CDATsysDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDATsysDoc serialization

void CDATsysDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDATsysDoc diagnostics

#ifdef _DEBUG
void CDATsysDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDATsysDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDATsysDoc commands
