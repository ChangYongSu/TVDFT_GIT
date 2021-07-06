
/////////////////////////////////////////////////////////////////////////////
// CBaseThread thread

class CBaseThread : public CWinThread
{
	DECLARE_DYNCREATE(CBaseThread)
protected:
	CBaseThread();           // protected constructor used by dynamic creation

// Attributes
public:
	void SetOwner(CWnd* pOwner);
	CWnd* m_pOwner;
	BOOL m_bDone;

// Operations
public:
	virtual ~CBaseThread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBaseThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
