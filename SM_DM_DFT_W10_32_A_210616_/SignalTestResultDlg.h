//{{AFX_INCLUDES()
#include "vsflexgrid.h"
#include "labelcontrol.h"
//}}AFX_INCLUDES
#if !defined(AFX_SIGNALTESTRESULTDLG_H__2DDEF5CC_CF70_4391_871E_019CDB0686BB__INCLUDED_)
#define AFX_SIGNALTESTRESULTDLG_H__2DDEF5CC_CF70_4391_871E_019CDB0686BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SignalTestResultDlg.h : header file
//

#include "ReviewPage.h"

/////////////////////////////////////////////////////////////////////////////
// CSignalTestResultDlg dialog

class CSignalTestResultDlg : public CDialog
{
// Construction
public:
	CSignalTestResultDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSignalTestResultDlg)
	enum { IDD = IDD_SIGNAL_LINE_TEST_RESULT_DLG };
	CEdit	m_ctrlNoMaskedPixel;
	CEdit	m_ctrlNoPixel;
	CVSFlexGrid	m_ctrlNoHighBitGrid;
	CLabelControl	m_ctrlTabTitle1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSignalTestResultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

public:
	void SetParentWnd(CReviewPage* pWnd);
	void InitGrid();
	void UpdateGrid();

	int  m_nBitCount;
	int  m_nTotalNoMaskedPixel;
	int  m_nTotalNoUnmaskedPixel;

	int* m_pRefNoHighBit;
	int* m_pGrabNoHighBit;

	
	CReviewPage* m_pParentWnd;
	
	CRect m_dlgRect;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSignalTestResultDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALTESTRESULTDLG_H__2DDEF5CC_CF70_4391_871E_019CDB0686BB__INCLUDED_)
