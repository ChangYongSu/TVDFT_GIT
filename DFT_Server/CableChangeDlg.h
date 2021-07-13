#pragma once
#include "label_text.h"


// CCableChangeDlg 대화 상자입니다.

class CCableChangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCableChangeDlg)

public:
	CCableChangeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCableChangeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CABLE_CHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonUsbCountReset();
	afx_msg void OnBnClickedButtonPortCountReset();
	afx_msg void OnBnClickedButtonCheckerPinCountReset();
	afx_msg void OnBnClickedButtonMicCountReset();
	UINT m_EditUsbCableMax;
	UINT m_EditUsbCableCount;
	UINT m_EditPortCableMax;
	UINT m_EditPortCableCount;
	UINT m_EditCheckerMax;
	UINT m_EditCheckerCount;
	UINT m_EditMicCableMax;
	UINT m_EditMicCableCount;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonCheckerPinCountReset2();
	UINT m_EditCheckerCountCM5;
	UINT m_EditCheckerMaxCM5;
	CLabel_text m_cLabelUsbCable;
	CLabel_text m_cLabelPortCable;
	CLabel_text m_cLabelCM4Pin;
	CLabel_text m_cLabelCM5Pin;
	CLabel_text m_cLabelMICCable;
};
