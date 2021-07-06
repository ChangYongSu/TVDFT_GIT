// UsbdioTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "UsbdioTestDlg.h"
#include "UsbDio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUsbdioTestDlg dialog


CUsbdioTestDlg::CUsbdioTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsbdioTestDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CUsbdioTestDlg)
	m_bRelay1 = FALSE;
	m_bRelay2 = FALSE;
	m_bRelay3 = FALSE;
	m_bRelay4 = FALSE;
	//}}AFX_DATA_INIT
}


void CUsbdioTestDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CUsbdioTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsbdioTestDlg)
	DDX_Control(pDX, IDC_CHECK_SELECT_ALL, m_ctrlSelectAll);
	DDX_Check(pDX, IDC_CHECK_RELAY1, m_bRelay1);
	DDX_Check(pDX, IDC_CHECK_RELAY2, m_bRelay2);
	DDX_Check(pDX, IDC_CHECK_RELAY3, m_bRelay3);
	DDX_Check(pDX, IDC_CHECK_RELAY4, m_bRelay4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsbdioTestDlg, CDialog)
	//{{AFX_MSG_MAP(CUsbdioTestDlg)
	ON_BN_CLICKED(IDC_CHECK_SELECT_ALL, OnCheckSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CUsbdioTestDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CUsbdioTestDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IUsbdioTestDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {62958E78-B26C-4639-8879-4FA7DC66155F}
static const IID IID_IUsbdioTestDlg =
{ 0x62958e78, 0xb26c, 0x4639, { 0x88, 0x79, 0x4f, 0xa7, 0xdc, 0x66, 0x15, 0x5f } };

BEGIN_INTERFACE_MAP(CUsbdioTestDlg, CDialog)
	INTERFACE_PART(CUsbdioTestDlg, IID_IUsbdioTestDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsbdioTestDlg message handlers

void CUsbdioTestDlg::OnCheckSelectAll() 
{

	if(m_ctrlSelectAll.GetCheck())
	{
		m_bRelay1 = TRUE;
		m_bRelay2 = TRUE;
		m_bRelay3 = TRUE;
		m_bRelay4 = TRUE;
	}
	else{
		m_bRelay1 = FALSE;
		m_bRelay2 = FALSE;
		m_bRelay3 = FALSE;
		m_bRelay4 = FALSE;
	}

	UpdateData(FALSE);

}

BOOL CUsbdioTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BYTE nBuf;

	//nBuf =  gUsbDioCtrl.nDIO_Out & 0x01;
	//if(nBuf == 0x01){
	//	m_bRelay1 = TRUE;
	//}

	//nBuf =  gUsbDioCtrl.nDIO_Out & 0x02;
	//if(nBuf == 0x02){
	//	m_bRelay2 = TRUE;
	//}

	//nBuf =  gUsbDioCtrl.nDIO_Out & 0x04;
	//if(nBuf == 0x04){
	//	m_bRelay3 = TRUE;
	//}

	//nBuf =  gUsbDioCtrl.nDIO_Out & 0x08;
	//if(nBuf == 0x08){
	//	m_bRelay1 = TRUE;
	//}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUsbdioTestDlg::OnButtonSet() 
{
	BYTE nBuf = 0;

	UpdateData();
	
	if(m_bRelay1) nBuf = nBuf + 0x01;
	if(m_bRelay2) nBuf = nBuf + 0x02;
	if(m_bRelay3) nBuf = nBuf + 0x04;
	if(m_bRelay4) nBuf = nBuf + 0x08;

	gUsbDioCtrl.WriteByte(nBuf);
}
