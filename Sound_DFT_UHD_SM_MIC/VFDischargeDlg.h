#pragma once

//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES

// CVFDischargeDlg ��ȭ �����Դϴ�.

class CVFDischargeDlg : public CDialog
{
	DECLARE_DYNAMIC(CVFDischargeDlg)

public:
	CVFDischargeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CVFDischargeDlg();

// ��ȭ ���� �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedBtnEnter();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
