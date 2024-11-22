#pragma once

#include "ColorStaticST.h"	
// CUserMsgColorDlg 대화 상자

class CUserMsgColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserMsgColorDlg)

public:
	CUserMsgColorDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CUserMsgColorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USER_MSG_COLOR_DLG };
#endif
public:

	CColorStaticST		m_cMessageBox;
	CRect m_dlgRect;
	CString m_szMsg1;	
	CString m_szMsg2;
	CFont m_Step_New_Font;
	BOOL	m_bFontSetting;
	BOOL	m_bYes;
	BOOL	m_bAutoClose;
	BOOL		m_bOptionCheckEnable;
	BOOL		m_bActivate;
	void SetMessage(CString szMsg1, CString szMsg2);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnYes();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
