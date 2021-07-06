#pragma once

#define CHASSIS_GRID	0
#define MODEL_GRID		1
#include "ListCtrlEx.h"

//#include "XLEzAutomation.h"

//#include "CExcel.h"

extern int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
// CSelectGenerateModel 대화 상자

class CSelectGenerateModel : public CDialog
{
	DECLARE_DYNAMIC(CSelectGenerateModel)

public:
	CSelectGenerateModel(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSelectGenerateModel();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_GENERATE_MODEL_DLG };
#endif

public:
	void InitGrid();
	void UpdateGrid();
	BOOL IsSameChassis(CString szChassisName);
	void CSelectGenerateModel::OnClickChassisGrid();
	void CSelectGenerateModel::Write_DFTRegistry();
	//BOOL CSelectGenerateModel::GetFolder(CString* strSelectedFolder,
	//	const char* lpszTitle,
	//	const HWND hwndOwner,
	//	const char* strRootFolder,
	//	const char* strStartFolder);



	void CSelectGenerateModel::OnClickModelGrid();
	void CSelectGenerateModel::ChangeRowColor(int nRow, int nGridType);

	BOOL CSelectGenerateModel::ModelListOpen();
	BOOL CSelectGenerateModel::CheckModel(CString sChassis, CString sModel);


	void CSelectGenerateModel::OnBtnAdd();

	void CSelectGenerateModel::OnBtnEdit();

	void CSelectGenerateModel::OnBtnCopy();
	BOOL CSelectGenerateModel::SaveModelListFile();
	//CXLEzAutomation XL; // 
	void CSelectGenerateModel::OnBtnDel();


	void CSelectGenerateModel::OnImportText();

	void CSelectGenerateModel::OnImportExcel();

	//CTypedPtrList<CPtrList, CModelData*>	ModelList;
	//CTypedPtrList<CPtrList, CModelData*>	ModelList_temp;
	int		m_nChaassis_SelectRow;
	int		m_nModel_SelectRow;

	CString m_sSelChassis;
	CString m_sSelModel;

	CListCtrlEx		m_cListChassisEx;
	CListCtrlEx		m_cListModelEx;

	void CSelectGenerateModel::UpdateGridLast();
	CString m_ModelListPathOld;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnCopy();
	afx_msg void OnBnClickedBtnDel();

	CString m_szServerPath;
	CString m_szRefImagePath;
//	CString m_szModelInfoPath;
	CString  m_szModelInfoFilePath;
	CString   m_szRefImgFolder;
	CString   m_szSeqFilePath;
	CString   m_szDivision;

	afx_msg void OnBnClickedOk();
	CString m_szModelListPath;
	CListCtrl m_cListChassis;
	CListCtrl m_cListModel;
	afx_msg void OnBnClickedBtnChangeModelPath();
	afx_msg void OnNMClickListChassis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListChassis(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListModel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
};
