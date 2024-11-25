#pragma once

//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES


// CVfMeasureCheckDlg ��ȭ �����Դϴ�.

class CVfMeasureCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CVfMeasureCheckDlg)

public:
	CVfMeasureCheckDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CVfMeasureCheckDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VF_MEASURE_CHECK };
	CVSFlexGrid	m_ctrlVfMeasureCheckGrid;

	BOOL		m_bActivate;
	BOOL		m_bVfCheckResult;
	BOOL		m_bVfCheckNG_Wait;
	BOOL		m_bTestClose;
	BOOL		m_bTestRunning;
	int			m_nVfCheckCount;
	BOOL		m_bVfCheckResultList[200];
	CString		m_sVfCheckResultMesure[200];


	void InitVfMeasureCheckGrid();
	void ConfigDisplay();
	void UpdateResult(int nIndex, CString sMeasure, BOOL bResult);
	BOOL VfMeasureCheck();
	void SaveResult2File();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnEnter();
	afx_msg void OnBnClickedBtnRecheck();
	afx_msg long OnProcessMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnDestroy();
};
