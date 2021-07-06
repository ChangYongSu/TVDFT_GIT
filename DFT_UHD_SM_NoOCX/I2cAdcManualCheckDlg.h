//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_I2CADCMANUALCHECKDLG_H__FF48D34B_8B24_4093_9E6C_28E7B0DAC369__INCLUDED_)
#define AFX_I2CADCMANUALCHECKDLG_H__FF48D34B_8B24_4093_9E6C_28E7B0DAC369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// I2cAdcManualCheckDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CI2cAdcManualCheckDlg dialog

class CI2cAdcManualCheckDlg : public CDialog
{
// Construction
public:
	CI2cAdcManualCheckDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CI2cAdcManualCheckDlg)
	enum { IDD = IDD_I2C_ADC_MANUAL_CHECK_DLG };
	CEdit	m_ctrlModelEdit;
	CEdit	m_ctrlPatternEdit;
	CComboBox	m_ctrlI2cAreaOptionNo;
	CComboBox	m_ctrlI2cToolOptionNo;
	CEdit	m_ctrl2bWriteDataList;
	CProgressCtrl	m_ctrlReadDataProgress;
	CComboBox	m_ctrl2abReadByte;
	CComboBox	m_ctrl2bReadByte;
	CComboBox	m_ctrlI2cChannelNo;
	CComboBox	m_ctrlI2cClkSpeed;
	CVSFlexGrid	m_ctrlReadDataGrid;
	CVSFlexGrid	m_ctrlI2cAdcComLogGrid;
	CString	m_sz2abData;
	CString	m_sz2abCmd;
	CString	m_sz2abReadAdh;
	CString	m_sz2abReadAdl;
	UINT	m_nI2cPacketDelay;
	UINT	m_nI2cReadByteDelay;
	UINT	m_nI2cWriteByteDelay;
	CString	m_sz2bWriteAdr;
	CString	m_sz2bReadAdr;
	CString	m_sz2abCurrentAreaOption;
	CString	m_sz2abCurrentToolOption;
	CString	m_sz2abReadAreaOption;
	CString	m_sz2abReadToolOption;
	CString	m_sz2abWriteAreaOption;
	CString	m_sz2abWriteToolOption;
	CString	m_sz2abReadVersion;
	CString	m_szEdidDownloadResult;
	CString	m_sz2abReadVersion2;
	//}}AFX_DATA


	BOOL	m_bActive;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CI2cAdcManualCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CI2cAdcManualCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnSetI2cClkDelay();
	afx_msg void OnBtnSetI2cChannelNo();
	afx_msg void OnBtnSend2abCmd();
	afx_msg void OnBtn2abWakeup();
	afx_msg void OnBtnRead2abDataByte();
	afx_msg void OnBtnRead2abDataPage();
	afx_msg void OnBtnRead2bDataByte();
	afx_msg void OnBtnRead2bDataPage();
	afx_msg void OnBtnWrite2bData();
	afx_msg void OnBtn2abSetType();
	afx_msg void OnClose();
	afx_msg void OnBtnClearComLogGrid();
	afx_msg void OnBtnClearReadData();
	afx_msg void OnBtn2bWakeup();
	afx_msg void OnBtnClose();
	afx_msg void OnBtn2abToolOptionRead();
	afx_msg void OnBtn2abReadVersion();
	afx_msg void OnBtn2abAreaOptionRead();
	afx_msg void OnSelchangeCmbToolOptionNo();
	afx_msg void OnSelchangeCmbAreaOptionNo();
	afx_msg void OnBtn2abToolOptionWrite();
	afx_msg void OnBtn2abAreaOptionWrite();
	afx_msg void OnBtnSendModel();
	afx_msg void OnBtnSendPattern();
	afx_msg void OnBtnRemoteDlg29();
	afx_msg void OnBtnRemoteDlg31();
	afx_msg void OnBtnRemoteDlg30();
	afx_msg void OnBtnRemoteDlg33();
	afx_msg void OnBtnRemoteDlg81();
	afx_msg void OnBtnRemoteDlg71();
	afx_msg void OnBtnRemoteDlg32();
	afx_msg void OnBtnRemoteDlg14();
	afx_msg void OnBtnRemoteDlg34();
	afx_msg void OnBtnRemoteDlg73();
	afx_msg void OnBtn2abEdidDownload();
	afx_msg void OnBtn2abReadVersion2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	void InitReadDataGrid();
	void InitComLogGrid();
	void WriteLog(CString szString);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_I2CADCMANUALCHECKDLG_H__FF48D34B_8B24_4093_9E6C_28E7B0DAC369__INCLUDED_)
