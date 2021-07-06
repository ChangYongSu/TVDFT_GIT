//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_RS232CADCMANUALCHECKDLG_H__0A2BB11E_0D75_4AB4_8DF2_E64B5FB3E4D0__INCLUDED_)
#define AFX_RS232CADCMANUALCHECKDLG_H__0A2BB11E_0D75_4AB4_8DF2_E64B5FB3E4D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Rs232cAdcManualCheckDlg.h : header file
//
//////#include "EditLog\EditLog.h"
////#include "EditLog\editlog_stream.h"

/////////////////////////////////////////////////////////////////////////////
// CRs232cAdcManualCheckDlg dialog

class CRs232cAdcManualCheckDlg : public CDialog
{
// Construction
public:
	CRs232cAdcManualCheckDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRs232cAdcManualCheckDlg)
	enum { IDD = IDD_RS232C_ADC_MANUAL_CHECK_DLG };

	CStringArray m_strLogArray;
	CRichEditCtrl	m_ctrlEdit;
	CEdit	m_ctrlPatternEdit;
	CEdit	m_ctrlModelEdit;
	CComboBox	m_ctrlBaudRate;
	CComboBox	m_ctrlComPort;
	CString	m_szCmd1;
	int		m_nEchoOn;
	int		m_nMaxWait;
	CString	m_szStrCmd;
	//}}AFX_DATA

	BOOL	m_bActive;
	CString GetComPortVal(int nComportCmbID);
	DWORD	GetBaudRateVal(int nBaudRateCmbID);
	void	InitComPort(CString sComPort, DWORD wBaudRate, int nComportCmbID, int nBaudRateCmbID);
	void	WriteLog(CString szString);
	int CRs232cAdcManualCheckDlg::AppendToLogAndScroll(CString str,  COLORREF color = RGB(0, 0, 0));
	void	InitComLogGrid();

	// The logger object
	//CEditLog			m_ctrlRs232cAdcComLog;
	
	// ANSI and UNICODE stream buffers for the EditLogger
	//std::editstreambuf	m_EditStrBuf;
	//std::weditstreambuf m_EditStrBufW;

	// Used to save the previos values for cout and wcout
	//std::basic_streambuf<char>*		m_pOldBuf;
	//std::basic_streambuf<wchar_t>*	m_pOldBufW;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRs232cAdcManualCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRs232cAdcManualCheckDlg)
	afx_msg void OnBtnSetComPort();
	afx_msg void OnBtnSetEchoMode();
	afx_msg void OnBtnSendCommand();
	afx_msg void OnBtnClearRs232cComLogGrid();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboRs232cComPort();
	afx_msg void OnBtnClose();
	afx_msg void OnBtnSendModel();
	afx_msg void OnBtnSendPattern();
	afx_msg void OnBtnSendCommand2();
	afx_msg void OnBtnRemoteRgbR();
	afx_msg void OnBtnRemoteComp1R();
	afx_msg void OnBtnRemoteTvR();
	afx_msg void OnBtnRemoteAv1R();
	afx_msg void OnBtnRemoteInstartR();
	afx_msg void OnBtnRemoteAdjR();
	afx_msg void OnBtnRemoteUpR();
	afx_msg void OnBtnRemoteRightR();
	afx_msg void OnBtnRemoteDownR();
	afx_msg void OnBtnRemoteExitR();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RS232CADCMANUALCHECKDLG_H__0A2BB11E_0D75_4AB4_8DF2_E64B5FB3E4D0__INCLUDED_)
