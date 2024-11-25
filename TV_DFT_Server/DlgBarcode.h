#pragma once
#include "label_text.h"


// CDlgBarcode 대화 상자입니다.

class CDlgBarcode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBarcode)

public:
	CDlgBarcode(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgBarcode();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BARCODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_Barcode ;
	CString m_BarcodeString ;
	int m_ReturnFlag;
public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CLabel_text m_clabelBarCode;
};
