#pragma once


// ModelSelectDlg 대화 상자

class ModelSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ModelSelectDlg)

public:
	ModelSelectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ModelSelectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODEL_SELECT };
#endif

public:
	CFont* font;
	void SetListControl();
	int m_nDivision_Sel;
	int m_nChaassis_SelectRow;
	int m_nModel_SelectRow;
	CString m_sSelChassis;
	CString m_sSelModel;
	void UpdateGrid();
	BOOL IsSameChassis(CString szChassisName);
	void OnClickChassisGrid(int			nChassisRow);
	void OnClickModelGrid(int			nModelRow);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cListModelListDisplay;

	virtual BOOL OnInitDialog();
	CListCtrl m_cListCtrlChassisDisplay;
	CComboBox m_cComboYearDivision;
	afx_msg void OnNMClickListChassisListDisplay(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListModelListDisplay(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_szSeqFilePath;
	CString m_szModelInfoFilePath;
	CString m_szRefImgFolder;
	afx_msg void OnCbnSelchangeComboYearDevision();
};
