

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "button_model_create.h"
#include "textbox_okng.h"
#include "label_text.h"


// CDlgModelSetup 대화 상자입니다.

class CDlgModelSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModelSetup)

public:
	CDlgModelSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgModelSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL_SETUP };
};
#if 0
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	Recomon_ModelT m_RemoconModel;

	//void LoadModel(CString lFilename);
	void SaveModel(CString lFilename);
	void SetTestEnable();
	//void SetUIData();
	void EnableComboSeq();

	void GetModelList();
	void GetRemoconList();
	void LoadTestData();
	void DisplayTestData();
	int UpdateTestData();
	bool NameListCheckAndADD(CString lCheckName);

	void GetRmtCodeList(CString lRmtFile);

	CString m_strRemoconList[1000];
	int m_nRecomonCount;
	CString m_strModelNameList[1000];
	int m_nModelCount;

public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonChngRmt();
	
	CListCtrl m_CtrlListSequence;


	CString m_EditReadyCode;

	CComboBox m_CtrlComboStart;
	CComboBox m_ComboSeq[20];
	CString m_EditSeqData[20];

	CComboBox m_ComboPort[24];
	BOOL m_CheckPortEn[24];
	afx_msg void OnBnClickedCheckEn11();
	afx_msg void OnBnClickedCheckEn12();
	afx_msg void OnBnClickedCheckEn13();
	afx_msg void OnBnClickedCheckEn14();
	afx_msg void OnBnClickedCheckEn15();
	afx_msg void OnBnClickedCheckEn16();
	afx_msg void OnBnClickedCheckEn21();
	afx_msg void OnBnClickedCheckEn22();
	afx_msg void OnBnClickedCheckEn23();
	afx_msg void OnBnClickedCheckEn24();
	afx_msg void OnBnClickedCheckEn25();
	afx_msg void OnBnClickedCheckEn26();
	afx_msg void OnBnClickedCheckEn31();
	afx_msg void OnBnClickedCheckEn32();
	afx_msg void OnBnClickedCheckEn33();
	afx_msg void OnBnClickedCheckEn34();
	afx_msg void OnBnClickedCheckEn35();
	afx_msg void OnBnClickedCheckEn36();
	afx_msg void OnBnClickedCheckEn41();
	afx_msg void OnBnClickedCheckEn42();
	afx_msg void OnBnClickedCheckEn43();
	afx_msg void OnBnClickedCheckEn44();
	afx_msg void OnBnClickedCheckEn45();
	afx_msg void OnBnClickedCheckEn46();

	CComboBox m_cModelCombo;
	CButton_Ctrl m_cBtnModelCreate;
	CButton_Ctrl m_cBtnModelSave;
	CButton_Ctrl m_cBtnModelRemove;
	int m_EditFreqMin[24];
	int m_EditFreqMax[24];
	int m_EditVoltMin[24];
	int m_EditVoltMax[24];
	afx_msg void OnCbnSelchangeComboModel();

	CLabel_text m_TextModelName;

	DECLARE_EVENTSINK_MAP()
	void ClickButtonModelSave();
	void ClickButtonModelCreate();
	afx_msg void OnCbnSelchangeComboSeq1 ();
	afx_msg void OnCbnSelchangeComboSeq2 ();
	afx_msg void OnCbnSelchangeComboSeq3 ();
	afx_msg void OnCbnSelchangeComboSeq4 ();
	afx_msg void OnCbnSelchangeComboSeq5 ();
	afx_msg void OnCbnSelchangeComboSeq6 ();
	afx_msg void OnCbnSelchangeComboSeq7 ();
	afx_msg void OnCbnSelchangeComboSeq8 ();
	afx_msg void OnCbnSelchangeComboSeq9 ();
	afx_msg void OnCbnSelchangeComboSeq10();
	afx_msg void OnCbnSelchangeComboSeq11();
	afx_msg void OnCbnSelchangeComboSeq12();
	afx_msg void OnCbnSelchangeComboSeq13 ();
	afx_msg void OnCbnSelchangeComboSeq14 ();
	afx_msg void OnCbnSelchangeComboSeq15 ();
	afx_msg void OnCbnSelchangeComboSeq16 ();
	afx_msg void OnCbnSelchangeComboSeq17 ();
	afx_msg void OnCbnSelchangeComboSeq18 ();
	afx_msg void OnCbnSelchangeComboSeq19 ();
	afx_msg void OnCbnSelchangeComboSeq20();
	
	CComboBox m_cRemoconCombo;
	CComboBox m_cComboRmtType;
	//void ClickButtonGetRmCode();
	afx_msg void OnCbnSelchangeComboRecomon();
	CComboBox m_cComboRmtCode;
	
	void ClickButtonModelConfig();
	void ClickButtonModelRemove();
	BOOL m_CheckMesEnable;
	CLabel_text m_TextMesTns;
	CLabel_text m_TextMesAvn;
	CLabel_text m_TextMesUserID;
	CLabel_text m_TextMesPassword;


	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	
	
	CLabel_text m_TextMesLine;
	CLabel_text m_TextMesInspStep;
	CLabel_text m_LabelDispMainMark;
	CLabel_text m_LabelDispAudioProcess;
	CLabel_text m_LabelDispReadyCode;
	CLabel_text m_LabelDispAudioTest1;
	CLabel_text m_LabelDispAudioTest2;
	CLabel_text m_LabelDispAudioTest3;
	CLabel_text m_LabelDispAudioTest4;
	CLabel_text m_LabelMark_RemoteSet;
	CLabel_text m_LabelMark_IR_Signal;
	CLabel_text m_LabelMark_IR_Code;
	CLabel_text m_LabelMark_MesInform;
	CLabel_text m_LabelMark_MesEnable;
	CLabel_text m_LabelMark_MesTns;
	CLabel_text m_LabelMark_MesAvn;
	CLabel_text m_LabelMark_MesUid;
	CLabel_text m_LabelMark_MesPwrd;
	CLabel_text m_LabelMark_MesLineCode;
	CLabel_text m_LabelMark_MesInStep;
	CButton_Ctrl m_cBtnModelConfig;
	afx_msg void OnBnClickedButtonReadycodeSet();

	BOOL m_LevelSkip1;
	BOOL m_LevelSkip2;
	BOOL m_LevelSkip3;
	BOOL m_LevelSkip4;
	afx_msg void OnBnClickedCheckLevelSkip1();
	afx_msg void OnBnClickedCheckLevelSkip2();
	afx_msg void OnBnClickedCheckLevelSkip3();
	afx_msg void OnBnClickedCheckLevelSkip4();
};

#endif