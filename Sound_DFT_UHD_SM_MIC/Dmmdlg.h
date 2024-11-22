// File  Έν : DmmDlg.h

// DMMDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDMMDlg dialog

class CDMMDlg : public CDialog
{
// Construction
public:
	BOOL m_bPause;
	CDMMDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDMMDlg)
	enum { IDD = IDD_DMM_CONTROL_DLG };
	CButton	m_ctrlOK;
	CButton	m_ctrlPause;
	CButton	m_ctrlMeasure;
	int		m_nFunction;
	double	m_fMeasured;
	CString	m_sUnit;
	int		m_nChannel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDMMDlg)
	afx_msg void OnSelchangeFunction();
//	afx_msg void OnMeasure();
	afx_msg void OnPause();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeMeasureChannel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
