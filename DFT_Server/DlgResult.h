#pragma once
#include "label_text.h"
#include "button_model_create.h"


// CDlgResult 대화 상자입니다.

class CDlgResult : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgResult)

public:
	CDlgResult(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgResult();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RESULT };
	CDialog* pDlg;

	int m_Result;
	int m_ReturnFlag;
	int m_HeartEnable;

	//CString m_BarcodeString;
	//CString m_Barcode;
	//CString m_DisplayString;
	//CString m_DetailMessage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


		virtual BOOL OnInitDialog();
public:

	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//CLabel_text m_cBtnResult;
	//CLabel_text m_cBtnResultOK;
	//CLabel_text m_cBtnResultNG;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//CButton_Ctrl m_cBtnResult;
	CLabel_text m_cBtnResult;
	//CLabel_text m_cLabelNGDisplay;
	//CString m_EditNgDisplay;
	//afx_msg void OnBnClickedButtonDetail();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};
