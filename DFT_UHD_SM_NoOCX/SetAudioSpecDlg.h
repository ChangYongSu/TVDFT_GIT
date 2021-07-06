#if !defined(AFX_SETAUDIOSPECDLG_H__9675AA3C_6289_4DC6_9DD9_2614F9563874__INCLUDED_)
#define AFX_SETAUDIOSPECDLG_H__9675AA3C_6289_4DC6_9DD9_2614F9563874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetAudioSpecDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetAudioSpecDlg dialog

class CSetAudioSpecDlg : public CDialog
{
// Construction
public:
	CSetAudioSpecDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetAudioSpecDlg)
	enum { IDD = IDD_SET_AUDIO_SPEC_DLG };
	CEdit	m_ctrlRightLevelEdit;
	CEdit	m_ctrlRightFreqEdit;
	CEdit	m_ctrlLeftLevelEdit;
	CEdit	m_ctrlLeftFreqEdit;
	CEdit	m_ctrlAudioMarginEdit;
	CComboBox	m_ctrlAudioSrcCmb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetAudioSpecDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetAudioSpecDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
public:

	//Variable
	int		m_nAudioSource;

	int		m_nLeftFreq;
	int		m_nRightFreq;
	int		m_nLeftLevel;
	int		m_nRightLevel;
	
	int		m_nAudioMargin;
	//Function
	

/////////////////////////////////////////////////////////////////////////////

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETAUDIOSPECDLG_H__9675AA3C_6289_4DC6_9DD9_2614F9563874__INCLUDED_)
