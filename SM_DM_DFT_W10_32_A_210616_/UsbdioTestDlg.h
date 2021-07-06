#if !defined(AFX_USBDIOTESTDLG_H__AFF652C9_909C_4981_9EE8_CCC1F8EF39B3__INCLUDED_)
#define AFX_USBDIOTESTDLG_H__AFF652C9_909C_4981_9EE8_CCC1F8EF39B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UsbdioTestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUsbdioTestDlg dialog

class CUsbdioTestDlg : public CDialog
{
// Construction
public:
	CUsbdioTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUsbdioTestDlg)
	enum { IDD = IDD_USBDIO_DLG };
	CButton	m_ctrlSelectAll;
	BOOL	m_bRelay1;
	BOOL	m_bRelay2;
	BOOL	m_bRelay3;
	BOOL	m_bRelay4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsbdioTestDlg)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsbdioTestDlg)
	afx_msg void OnCheckSelectAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CUsbdioTestDlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USBDIOTESTDLG_H__AFF652C9_909C_4981_9EE8_CCC1F8EF39B3__INCLUDED_)
