#pragma once


// CDlgNewModelName 대화 상자입니다.

class CDlgNewModelName : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNewModelName)

public:
	CDlgNewModelName(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgNewModelName();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_NewName;
};
