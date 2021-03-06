#pragma once


// CEditDlg 대화 상자

class CEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditDlg)

public:
	CEditDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEditDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDIT };
#endif

	int m_nTargetCount;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditTotalCount;
	afx_msg void OnBnClickedOk();
	CString m_EditTotal;
};
