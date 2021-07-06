//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_SEQEDITORDLG_H__E18C9DCA_9AC2_46FD_8F06_D8F4598E44BE__INCLUDED_)
#define AFX_SEQEDITORDLG_H__E18C9DCA_9AC2_46FD_8F06_D8F4598E44BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeqEditorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
//+ 2007.8.28 Add BY USY
#include "Global.h"
#include "Step.h"
#include "Parser.h"
//-
/////////////////////////////////////////////////////////////////////////////

#define MAIN_GRID		0
#define SUB_GRID		1
#define TEMPLATE_GRID	2

#define MAIN_NO_COL				0
#define MAIN_SOURCE_COL			1
#define MAIN_STEP_NAME_COL		2
#define MAIN_AUDIO_COL			3
#define MAIN_STEP_TYPE_COL		4
#define MAIN_TEST_COL			5
#define MAIN_TARGET_COL			6
#define MAIN_L_LIMIT_COL		7
#define MAIN_U_LIMIT_COL		8
#define MAIN_UNIT_COL			9
#define MAIN_AUD_SOURCE_COL		10
#define MAIN_AUD_SPEC_COL		11
#define MAIN_AUD_MARGIN_COL		12

/////////////////////////////////////////////////////////////////////////////
// CSeqEditorDlg dialog

class CSeqEditorDlg : public CDialog
{
// Construction
public:
	CSeqEditorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSeqEditorDlg)
	enum { IDD = IDD_SEQ_EDITOR_DLG };
	CEdit	m_ctrlTemplatePathEdit;
	CEdit	m_ctrlSeqPathEdit;
	//CVSFlexGrid	m_ctrlMainGrid;
	//CVSFlexGrid	m_ctrlSubGrid;
	//CVSFlexGrid	m_ctrlTemplateGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeqEditorDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeqEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnNewSeq();
	afx_msg void OnBtnOpenSeq();
	afx_msg void OnBtnSaveSeq();
	afx_msg void OnBtnSaveAsSeq();
	afx_msg void OnBtnInsertMainStep();
	afx_msg void OnBtnUpMainStep();
	afx_msg void OnBtnDownMainStep();
	afx_msg void OnBtnDeleteMainStep();
	afx_msg void OnBtnClearMainStep();
	virtual void OnOK();
	afx_msg void OnDblClickSubStepGrid();
	afx_msg void OnBtnInsertSubStep();
	afx_msg void OnBtnUpSubStep();
	afx_msg void OnBtnDownSubStep();
	afx_msg void OnBtnDeleteSubStep();
	afx_msg void OnBtnClearSubStep();
	afx_msg void OnCellChangedSubStepGrid(long Row, long Col);
	afx_msg void OnBtnUploadSubStep();
	afx_msg void OnValidateEditMainStepGrid(long Row, long Col, BOOL FAR* Cancel);
	afx_msg void OnAfterEditMainStepGrid(long Row, long Col);
	afx_msg void OnAfterEditSubStepGrid(long Row, long Col);
	afx_msg void OnClickSubStepGrid();
	afx_msg void OnBeforeEditSubStepGrid(long Row, long Col, BOOL FAR* Cancel);
	afx_msg void OnBtnCopyMainStep();
	afx_msg void OnBtnCopySubStep();
	afx_msg void OnBtnInsertFromFullseq();
	afx_msg void OnClickMainStepGrid();
	afx_msg void OnClickTemplateSeqGrid();
	afx_msg void OnDblClickTemplateSeqGrid();
	afx_msg void OnBtnAddAll();
	afx_msg void OnBtnBrowseSeqFile();
	afx_msg void OnBtnBrowseTemplateFile();
	afx_msg void OnBtnSelectAll();
	afx_msg void OnBtnDeselectAll();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	CString m_strSeqPath;		//Sequence File Full Path

	CTypedPtrList<CPtrList, CFunction*> m_CurFunc;	//Current Sub Step List

	int				m_nMCurRow;			//Main Current Step Row ( Default -1)
//	int				m_nSCurStep;
	int m_Main_Click_nItem;//  = pNMItemActivate->iItem;
	int m_Main_Click_nShell;// = pNMItemActivate->iSubItem;
	int m_Sub_Click_nItem;//  = pNMItemActivate->iItem;
	int m_Sub_Click_nShell;// = pNMItemActivate->iSubItem;
	int m_Temp_Click_nItem;//  = pNMItemActivate->iItem;
	int m_Temp_Click_nShell;// = pNMItemActivate->iSubItem;
	int m_Main_Edit_Enable;
	int m_Main_Combo_Enable;
	int m_Sub_Combo_Enable;
	//Function

	//Grid Function
	void			InitMainGrid();
	void			UpdateMainGrid();
	void			SetMainGrid4Edit(int nRow, int nCol);
	void			InitSubGrid();
	void			UpdateSubGrid();
	void			SetSubGridHeader(int nRow);
	void			SetSubGrid4Edit(int nFuncNo, int nRow, int nCol);
	void			SetDefaultData2SubStepGrid(int nRow, int nCol);

	void			SetSubDlg4SubGrid(int nType, int nRow, int nCol);

	CString			MakeMainStepComboList(int nType);
	CString			MakeSubStepFuncList();
	UINT			CheckMainStepType(int nRow);
	UINT			CheckMainTestType(int nRow);
	BOOL			UpdateMainStep(int nStepNo);

	void SetEditPos(int nRow, int nCol, int Width, int Height);
	void SetComboPos(int nRow, int nCol, int Width, int Height, CString Data);

	//+2007.10.12 Mod BY USY
	int				CheckSubStepType(BOOL (*m_pFunc)());
//	UINT			CheckSubStepType(BOOL (*m_pFunc)());
	//-
	CString			CheckSubStepName(BOOL (*m_pFunc)());
	BOOL			UpdateSubStep();
	void			DeleteFunctionList();

	void			SortSubStep(CStep* pStep);
	void			CopyMainStep(CStep* pOrgStep, CStep* pCopyStep, int nTargetStepNo);
	void			CopySubStep(CFunction* pOrgFunc, CFunction* pCopyFunc);
	
	
	BOOL			SaveSeqFile(CString sSeqPath);
	CString			MakeStepHeader(CStep* pStep);
	BOOL			MakeSubStep(CStep* pStep, CStringArray& aArray);
	CString			MakeStepTail(CStep* pStep);

	void			ChangeRowColor(int nRow, int nGridType);

	void			SetAudioSpec(int nRow, int nCol);

	//+add kwmoon 080508
	void			InsertFromFullseq(int nSelRow); 

	//+add kwmoon 080805
	int				m_nTemplateSelectedRow;
	CString			m_strTemplatePath;

	//Function
	void			InitTemplateGrid();
	void			UpdateTemplateGrid();
	void			SelectRow(int nRow,int nGridType);
	void			ChangeColumnColor(int nGridType, CString szSource); 

/////////////////////////////////////////////////////////////////////////////

	CListCtrl m_ctrlStepMainList;
	CListCtrl m_ctrlSubList;
	CListCtrl m_ctrlTemplateList;
	afx_msg void OnNMClickListMainStepList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickSubStepList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkSubStepList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickTemplateSeqList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkTemplateSeqList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditSeqEditor();
	CComboBox m_cComboSeqEditor;
	CStringArray m_ComboDataList;
	CStringArray m_ComboSubDataList;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboSeqEditor();
	CComboBox m_cComboSeqSubEditor;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEQEDITORDLG_H__E18C9DCA_9AC2_46FD_8F06_D8F4598E44BE__INCLUDED_)
