#pragma once


// CDlgPassword 대화 상자입니다.

class CDlgPassword : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPassword)

public:
	CDlgPassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPassword();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PASSWORD };



	CString m_sPassword;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

	CString m_EditPassword;
	CString m_NewPassword;
	CString m_EditConfirmPassword;
	afx_msg void OnEnChangeEditEnterPassword();
	afx_msg void OnBnClickedButtonPasswordChange();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditConfirmPassword();
};
