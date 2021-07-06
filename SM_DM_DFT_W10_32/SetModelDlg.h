#if !defined(AFX_SETMODELDLG_H__FC0F032E_470E_462B_95D9_0A58DFEE0342__INCLUDED_)
#define AFX_SETMODELDLG_H__FC0F032E_470E_462B_95D9_0A58DFEE0342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetModelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetModelDlg dialog

class CSetModelDlg : public CDialog
{
// Construction
public:
	CSetModelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetModelDlg)
	enum { IDD = IDD_SET_MODEL_DLG };
	CEdit	m_ctrlRefImageFolderEdit;
	CEdit	m_ctrlTestParamFileEdit;
	CEdit	m_ctrlRemoteFileEdit;
	CEdit	m_ctrlPatternFileEdit;
	CEdit	m_ctrlFullSeqFileEdit;
	CEdit	m_ctrlSeqFileEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetModelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetModelDlg)
	afx_msg void OnBtnSelSeqFile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnSelFullSeqFile();
	afx_msg void OnBtnSelPatternFile();
	afx_msg void OnBtnSelRemoteFile();
	afx_msg void OnBtnSelTestParamFile();
	afx_msg void OnBtnSelRefImageFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
public:

	//+add kwmoon 080904
	CString m_strServerFolder;
	CString m_strRefImageFolder;

	CString m_strModelFolder;
	CString m_strSeqFilePath;
	CString m_strFullSeqFilePath;
	CString m_strRemoteFilePath;
	CString m_strPatternFilePath;
	CString m_strTestParamFilePath;

/////////////////////////////////////////////////////////////////////////////
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETMODELDLG_H__FC0F032E_470E_462B_95D9_0A58DFEE0342__INCLUDED_)
