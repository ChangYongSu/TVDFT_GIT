#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "label_text.h"

#include "AcqVoltageSamples_IntClkDlg.h"
// DlgModelSetupSeq 대화 상자입니다.

class DlgModelSetupSeq : public CDialogEx
{
	DECLARE_DYNAMIC(DlgModelSetupSeq)

public:
	DlgModelSetupSeq(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~DlgModelSetupSeq();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL_SETUP_SEQ };

	
	CFont* font;
	HFONT			m_hFont;//m_hFont;


public:
	void DlgModelSetupSeq::GetModelList();
	void DlgModelSetupSeq::GetSeqList();
	void DlgModelSetupSeq::SetSubListControl();
	void DlgModelSetupSeq::SetListControl();

	void DlgModelSetupSeq::LoadTestData();
	void DlgModelSetupSeq::DisplayTestData();

//	void DlgModelSetupSeq::GetRemoconList();
	void DlgModelSetupSeq::SaveModel(CString lFilename);
	bool DlgModelSetupSeq::NameListCheckAndADD(CString lCheckName);
	int  DlgModelSetupSeq::UpdateTestData();
//	void DlgModelSetupSeq::GetRmtCodeList(CString lRmtFile);


//	CString m_strRemoconList[1000];
//	int m_nRecomonCount;
	CString m_strModelSeqList[2000];
	CString m_strModelNameList[2000];
	int m_nModelCount;
//	Recomon_ModelT m_RemoconModel;

	void OpenComportPCB();
	void ComPCBSend();
	BYTE m_SendBufPCB[4096];
	int  m_SendCntPCB;
	
	BYTE m_RecvBufPCB[4096];
	int  m_RecvCntPCB;

	int m_PCB_COM_PROCESS ;
	int m_PCBComAck ;
	int m_PCBWaitCnt ;
	int m_PCBResendCnt;

	TestSeqDataT	m_TestOrgSeqData;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	
	//afx_msg long OnReceiveDataPCB(WPARAM wParam, LPARAM lParam);



	CListCtrl m_CtrlListTestProcess;
	DECLARE_EVENTSINK_MAP()
	void ClickButtonSeqLoad();
	void ClickButtonSeqEdit();
	CComboBox m_cModelComboSeq;
	//CComboBox m_cRemoconCombo;
	//CComboBox m_cSeqProcessCombo;
	void ClickButtonModelCreate();
	void ClickButtonModelSave();
	void ClickButtonModelRemove();
	CLabel_text m_TextModelName;
	void ClickButtonModelConfig();

		
	BOOL m_CheckMesEnable;
	//CLabel_text m_TextMesTns;
	//CLabel_text m_TextMesAvn;
	//CLabel_text m_TextMesUserID;
	//CLabel_text m_TextMesPassword;
	//CLabel_text m_TextMesLine;
	//CLabel_text m_TextMesInspStep;
	//CComboBox m_cComboRmtType;
	void ClickButtonImageSet();
	//afx_msg void OnEnChangeEditCameraIpaddress();
	afx_msg void OnCbnSelchangeComboRecomon2();
	afx_msg void OnBnClickedOk();
	//DWORD m_IPaddressGmes;
	//CString m_ServerPortNumber;
	//void DblClickLabelChecksum(LPDISPATCH Cancel);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//CString m_EditCheckSum;
	//CString m_EditFreq[10];
	afx_msg void OnBnClickedCancel();
	//CComboBox m_cComboPortType;
	//afx_msg void OnCbnSelchangeComboMesPortType();
	afx_msg void OnBnClickedButtonSaveas();
	afx_msg void OnBnClickedButtonGeswverMd();


	//////////////////////////////////////////////////
	CString m_EditFPGAVer;
	CString m_EditDXD_ServerVer;
	CString m_EditMicomVer;
	CString m_EditAVR_Ver;
	//afx_msg void OnBnClickedButtonSave();

	//afx_msg void OnBnClickedButtonScannerPortCheck();
	//afx_msg void OnCbnSelchangeComboComportScan();
	//	BOOL m_Radio_HK_HQ;
	int m_Radio_HK_HQ;
	CComboBox m_cComboMicomHKHQ;
	//afx_msg void OnCbnSelchangeComboMicomHkhqType();
	//afx_msg void OnBnClickedCancel();

	CString m_EditRoicMinValue[16];
	CString m_EditRoicMaxValue[16];
	CComboBox m_cComboSetModelName;
	afx_msg void OnCbnSelchangeComboMicomHkhqTypeMd();
	afx_msg void OnBnClickedButtonGetModelName();
};
