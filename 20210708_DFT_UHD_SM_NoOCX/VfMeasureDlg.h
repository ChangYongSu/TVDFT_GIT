/// File  Έν : VfMeasureDlg.h

// VfMeasureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDMMDlg dialog

class CVfMeasureDlg : public CDialog
{
// Construction
public:
	BOOL m_bPause;
	CVfMeasureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDMMDlg)
	enum { IDD = IDD_VF_MEASURE };
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
	//{{AFX_VIRTUAL(CVfMeasureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVfMeasureDlg)
	afx_msg void OnSelchangeFunction();
	afx_msg void OnMeasure();
	afx_msg void OnPause();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeMeasureChannel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ctrlCh;
};
