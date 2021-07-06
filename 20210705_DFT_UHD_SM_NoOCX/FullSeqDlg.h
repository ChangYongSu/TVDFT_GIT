//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_FULLSEQDLG_H__F8BA26B5_0DEC_48A8_B621_0E02625462FC__INCLUDED_)
#define AFX_FULLSEQDLG_H__F8BA26B5_0DEC_48A8_B621_0E02625462FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FullSeqDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "Global.h"
#include "Step.h"
#include "Parser.h"

//+add kwmoon 080508
#include "SeqEditorDlg.h"
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CFullSeqDlg dialog

class CFullSeqDlg : public CDialog
{
// Construction
public:
	CFullSeqDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFullSeqDlg)
	enum { IDD = IDD_FULL_SEQ_DLG };
	CEdit	m_ctrlFullSeqPathEdit;
	CVSFlexGrid	m_ctrlFullSeqGrid;
	//}}AFX_DATA

public:
	//+add kwmoon 080508
	CSeqEditorDlg* m_pParentDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFullSeqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFullSeqDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickFullSeqGrid();
	afx_msg void OnDblClickFullSeqGrid();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnAdd();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	int m_nSelRow;
	//Function
	void InitFullSeqGrid();
	void UpdateFullSeqGrid();
	void ChangeRowColor(int nRow);

	//+add kwmoon 080508
	void SetParentDlg(CSeqEditorDlg* pParentDlg);

/////////////////////////////////////////////////////////////////////////////

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FULLSEQDLG_H__F8BA26B5_0DEC_48A8_B621_0E02625462FC__INCLUDED_)
