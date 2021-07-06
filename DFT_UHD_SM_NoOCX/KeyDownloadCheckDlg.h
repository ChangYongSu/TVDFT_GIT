//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_KEYDOWNLOADCHECKDLG_H__03A0B146_C2D8_42C2_8B70_B997BC1AAFA4__INCLUDED_)
#define AFX_KEYDOWNLOADCHECKDLG_H__03A0B146_C2D8_42C2_8B70_B997BC1AAFA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyDownloadCheckDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyDownloadCheckDlg dialog

class CKeyDownloadCheckDlg : public CDialog
{
// Construction
public:
	CKeyDownloadCheckDlg(CWnd* pParent = NULL);   // standard constructor

	CRect			m_dlgRect;



	void InitKeyCheckGrid();
	void UpdateResult(int nIndex, int nResult);
	void SaveResult2File();

// Dialog Data
	//{{AFX_DATA(CKeyDownloadCheckDlg)
	enum { IDD = IDD_KEY_DOWNLOAD_CHECK_DLG };
	CVSFlexGrid	m_ctrlKeyCheckGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyDownloadCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyDownloadCheckDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYDOWNLOADCHECKDLG_H__03A0B146_C2D8_42C2_8B70_B997BC1AAFA4__INCLUDED_)
