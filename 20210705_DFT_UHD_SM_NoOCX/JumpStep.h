// 명    칭 : AIAS(Automatic Inline Alignment System)
// 창작일자 : 1997. 2. 21
// 소    속 : LG전자(LG생산기술원 MI Gr. 오구환)
// File  명 : JumpStep.h

// JumpStep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJumpStep dialog

class CJumpStep : public CDialog
{
// Construction
public:
	int m_nChoice;
	CJumpStep(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJumpStep)
	enum { IDD = IDD_JUMPSTEP };
	CListBox	m_stepList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJumpStep)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJumpStep)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkSteplist();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
