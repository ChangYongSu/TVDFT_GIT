#pragma once

#include "vsflexgrid.h"
#include "ListCtrlColor.h"

// COptVoltMeasureDlg 대화 상자입니다.

class COptVoltMeasureDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(COptVoltMeasureDlg)

public:
	COptVoltMeasureDlg();
	virtual ~COptVoltMeasureDlg();

	CString m_strConfigFile;
	BOOL bTestAll;

	void	InitVoltSpecGrid();
	void	InitVoltSpecList();
	BOOL WriteConfig( CString strConfigName);
	void ConfigDisplay();
	void ConfigDisplayList();

	void OnBnClickedButtonTestallList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OPT_VOLTMEASURE_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CVSFlexGrid m_ctrlVoltSpecGrid;
	CVSFlexGrid m_ctrlVoltSpecGrid2;
	CVSFlexGrid m_ctrlVoltSpecGrid3;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSave();
	DECLARE_EVENTSINK_MAP()
	void ClickVoltSpecGrid();
	void ClickVoltSpecGrid2();
	void ClickVoltSpecGrid3();
	CComboBox m_ctrlComboTime;
	afx_msg void OnBnClickedButtonTestall();
	void DblClickVoltSpecGrid();
	void DblClickVoltSpecGrid2();
	void DblClickVoltSpecGrid3();
	afx_msg void OnBnClickedButtonTestcls();
	afx_msg void OnBnClickedBtnResultcopy();
	afx_msg void OnBnClickedButtonSelectall();
	afx_msg void OnBnClickedButtonUnselectall();
	afx_msg void OnBnClickedBtbGrabstop();
	afx_msg void OnBnClickedBtbGrabstart();
	CString m_szStrCmd;
	afx_msg void OnBnClickedBtnSendCommand2();
	afx_msg void OnBnClickedBtnSendReadyCommand();

	CListCtrl m_ctrlVoltSpecList;
	CListCtrlColor m_ctrlVoltSpecListColor;

	CListCtrl m_ctrlVoltSpecList2;
	CListCtrlColor m_ctrlVoltSpecListColor2;

	CListCtrl m_ctrlVoltSpecList3;
	CListCtrlColor m_ctrlVoltSpecListColor3;
};
