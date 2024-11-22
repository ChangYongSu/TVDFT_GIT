//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_OPTHDCPPAGE_H__C492F6FC_23CA_407A_B099_DCB7FD85C35F__INCLUDED_)
#define AFX_OPTHDCPPAGE_H__C492F6FC_23CA_407A_B099_DCB7FD85C35F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptHdcpPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptHdcpPage dialog

class COptHdcpPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptHdcpPage)
// Construction
public:
	COptHdcpPage();
	~COptHdcpPage();

// Dialog Data
	//{{AFX_DATA(COptHdcpPage)
	enum { IDD = IDD_OPT_HDCP_DLG };
	CComboBox	m_ctrlHdcpLAddCmb;
	CComboBox	m_ctrlHdcpHAddCmb;
	CComboBox	m_ctrlI2cClkSpeed;
	CComboBox	m_ctrlKeyClassCmb;
	CVSFlexGrid	m_ctrlSpecHdcpGrid;
	UINT	m_nI2cReadByteDelay;
	UINT	m_nI2cWriteByteDelay;
	UINT	m_nHdcpDataLength;
	UINT	m_nI2cPacketDelay;
	UINT	m_nHdcpKeyQuantity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptHdcpPage)
	protected:
	virtual void OnOK();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptHdcpPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeComboKeyClass();
	afx_msg void OnSelchangeComboHadd();
	afx_msg void OnSelchangeComboLadd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	int		m_nHAdd;
	int		m_nLAdd;
//Function
	void InitHdcpSpecGrid();
	void UpdateHdcpSpecGrid(int nHadd, int nLadd, int nDataSize);
	void HdcpOptApply();
/////////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTHDCPPAGE_H__C492F6FC_23CA_407A_B099_DCB7FD85C35F__INCLUDED_)
