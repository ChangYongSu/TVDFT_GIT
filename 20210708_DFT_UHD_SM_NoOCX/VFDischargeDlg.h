#pragma once

//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES

// CVFDischargeDlg 대화 상자입니다.

class CVFDischargeDlg : public CDialog
{
	DECLARE_DYNAMIC(CVFDischargeDlg)

public:
	CVFDischargeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVFDischargeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VF_DISCHARGE_CHECK };
#endif

	CVSFlexGrid	m_ctrlVfMeasureCheckGrid;

	BOOL		m_bVfCheckResult;
	BOOL		m_bTestClose;
	BOOL		m_bTestRunning;
	BOOL		m_bThredStop;
	int			m_nNoFailed;
	CWinThread*		m_pMeasureThread;


	void InitVfMeasureCheckGrid();
	void ConfigDisplay();
	void UpdateResult(int nIndex, CString sMeasure, BOOL bResult);
	BOOL VfMeasureCheck();



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedBtnEnter();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
