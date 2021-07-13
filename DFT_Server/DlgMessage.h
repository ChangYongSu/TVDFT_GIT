#pragma once


// CDlgMessage 대화 상자입니다.

class CDlgMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessage)

public:
	CDlgMessage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMessage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_EditMessage;
};
