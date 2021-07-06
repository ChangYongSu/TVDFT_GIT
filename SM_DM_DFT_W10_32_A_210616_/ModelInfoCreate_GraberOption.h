#if !defined(AFX_MODELINFOCREATE_GRABEROPTION_H__9CDD4521_1064_4D75_9F21_5764B38E31FF__INCLUDED_)
#define AFX_MODELINFOCREATE_GRABEROPTION_H__9CDD4521_1064_4D75_9F21_5764B38E31FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelInfoCreate_GraberOption.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelInfoCreate_GraberOption dialog

class CModelInfoCreate_GraberOption : public CDialog
{
// Construction
public:
	CModelInfoCreate_GraberOption(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CModelInfoCreate_GraberOption)
	enum { IDD = IDD_MODELINFO_OPTION_DLG };
	CComboBox	m_ctrlTVBaudCmb;
	int		m_nAnalogFormat;
	int		m_ctlTVControlMode;
	BOOL	m_bAutoSourceControl;
	BOOL	m_bAutoPatternControl;
	int		m_nRemoconCustomCode;
	int		m_nRemoteType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelInfoCreate_GraberOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModelInfoCreate_GraberOption)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELINFOCREATE_GRABEROPTION_H__9CDD4521_1064_4D75_9F21_5764B38E31FF__INCLUDED_)
