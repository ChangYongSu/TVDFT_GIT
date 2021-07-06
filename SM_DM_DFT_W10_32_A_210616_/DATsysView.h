// DATsysView.h : interface of the CDATsysView class
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INCLUDES()
#include "vsflexgrid.h"
#include "mmhitimerx.h"
#include "mmconnectorx.h"
#include "mmwaveinx.h"
#include "mmspectrumx.h"
#include "mmpeakdetectx.h"
#include "mmledpanelx.h"
//}}AFX_INCLUDES
#include "ColorStaticST.h"	
#include "ImageProc.h"
#include "BaseImage.h"
#include "StopProcessingDlg.h"
#include "InitDlg.h"
//#include "DisplayImageDlg.h"
#include "ManualTestResultInputDlg.h"
#include "ResultDlg.h"
#include "StopStepDlg.h"
#include "UserMsgDlg.h"
#include "resource.h"

#include "ListCtrlEx.h"

//+add kwmoon 080826
//#include "I2cAdcManualCheckDlg.h"
//+add kwmoon 080904
#include "Rs232cAdcManualCheckDlg.h"

//+add kwmoon 071015 : [2in1]
#include "Defines.h"
#include "Aclapi.h"
//-
//#include "StaticFilespec.h"

//+add psh 090702
////#include "EditLog\EditLog.h"
//#include "EditLog\editlog_stream.h"
#include "MyEchoSocket.h"
#include "ButtonST\LedButton.h"

#include "OpenGLControl.h"

#include "SM_SoundControl.h" //#include "pipe.h"
#include "label_text.h"

#if !defined(AFX_DATSYSVIEW_H__A91C394C_305C_11D3_90A6_006008CED7F4__INCLUDED_)
#define AFX_DATSYSVIEW_H__A91C394C_305C_11D3_90A6_006008CED7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CBaseThread;
class CEnvironmentData;
class CBaseThread;

class CDATsysView : public CFormView
{
protected: // create from serialization only
	CDATsysView();
	DECLARE_DYNCREATE(CDATsysView)

private:
//    IApplicationPtr     m_ActiveMILApplication;
//    ISystemPtr          m_ActiveMILSystem;
//    IDigitizerPtr       m_ActiveMILDigitizer;
//    IImagePtr			m_ActiveMILImage;
public:
	COpenGLControl m_oglWindow;	
	COpenGLControl *m_poglWindow;
public:
	CSM_SoundControl m_sm_sound_ctrl;
	int m_InputEnableCount;
	int m_SignalEnable;

	int m_Sound_Display_Enable;
	PlotData_T	m_PlotDataL[2048];
	PlotData_T	m_PlotDataR[2048];

	int m_CurrentStep;
public:
	//{{AFX_DATA(CDATsysView)
	enum { IDD = IDD_DATSYS_FORM };
	CRichEditCtrl	m_ctrlEditComLog;
	CStringArray m_strLogArray;
	int GetNumVisibleLines(CRichEditCtrl* pCtrl);
	CTabCtrl	m_ctrlViewTab;
	BOOL	m_bSeq_Lock;
//	CVSFlexGrid	m_ctrlSummaryGrid;
//	CVSFlexGrid	m_ctrlVersionGrid;
	//}}AFX_DATA
//	CDisplay m_ActiveMILDisplay;

// Attributes
public:
	CDATsysDoc* GetDocument();

public:

	CBaseThread* m_pBaseThread;

	MyEchoSocket m_pSocketClient;

	// ANSI and UNICODE stream buffers for the EditLogger
	//std::editstreambuf	m_EditStrBuf;
	//std::weditstreambuf m_EditStrBufW;

	// Used to save the previos values for cout and wcout
	//std::basic_streambuf<char>*		m_pOldBuf;
	//std::basic_streambuf<wchar_t>*	m_pOldBufW;

	CColorStaticST m_stcModelFolder;
	CColorStaticST m_stcRefFolder;
	CColorStaticST m_stcChassisName;
	CColorStaticST m_stcSeqName;
	CColorStaticST m_stcMode;
	CColorStaticST m_stcStatus;
	CColorStaticST m_stcTime;
	CRect m_FormViewRect;
	CRect m_VideoViewClientRect;
	CRect m_StatusViewClientRect;
	CRect m_StatusViewScreenRect;
	CRect m_ElapsedTimeClientRect;
	CRect m_VideoViewScreenRect;
	CWnd* m_pVideoViewWnd;
	CWnd* m_pStatusViewWnd;
	CWnd* m_pElapsedTimeWnd;

	BOOL m_bGetRectSizeInfo;
	BOOL m_bTerminalModeOn;
	BOOL m_bThreadRunning;

	BOOL m_bGrabThreadRunning;
	BOOL m_bStopLVDSThread;
	
	BOOL m_bAdcRunning;

	CMainFrame* m_pFrame;

	CString m_szCurrentStatus;
	int m_Display_StepTime;

	CFont	m_CustomFont;
	CFont	m_StatusWindowFont;
	CFont	m_ProgressFont;
	CFont	m_AudioFont;

	// Modeless Dialog
	CStopProcessingDlg*	m_pStopProcessingDlg;
	CInitDlg*			m_pInitDlg;
		
	// Modal Dialog
	CManualTestResultInputDlg	m_ManualTestDlg;
	CStopStepDlg				m_StopStepDlg;
	CResultDlg*					m_ResultDlg;
	//+add 090902
	CUserMsgDlg					m_UserMsgDlg;

	//+add kwmoon 080826
//	CI2cAdcManualCheckDlg		m_I2cAdcManualCheckDlg;

	//+add kwmoon 080904
	CRs232cAdcManualCheckDlg	m_Rs232cAdcManualCheckDlg;

	CColorStaticST	m_ctrLEDTested;
	CColorStaticST	m_ctrLEDFailed;
	CColorStaticST	m_ctrLEDRate;


	CBaseImage		m_Image[5];
//	CBaseImage		m_GrabImage;

	CString			m_szExePath;
	CString			m_szNoImageBmpPath;

	CString			m_szMakeRefPath;

	int				m_bMakeReferenceMode;
	int				m_bAdjSpecMode;

	CStringArray	m_aRefIntFileArray;

	// FileMapping GrabberOption
//	ImageInfoStruct*	m_pImageInfo;

	//+add psh 080603
	BOOL	m_bResultDlgActivate;
	BOOL	m_bUseDefaultREFfolder;
	HANDLE	m_hGrabThreadKillEvent;

//	CWinThread* m_pGrabImageThread;

	int		m_nGrabType;
	int		m_nGrabResult;
	BOOL	m_bStopMeasureAudio;

	//+add kwmoon 080710
	CStdioFile	m_pTimeDataFile;
	BOOL	m_bTimeDataFileOpen;
	DWORD	m_dwStepElapsedTime;

	//+add kwmoon 080724
	CString	m_szVersion;
	CStopProcessingDlg* m_pSystemMessageDlg;

	CStatic			m_cStepResult;

	CBitmap			m_bmpNormal;
	CBitmap			m_bmpGood;
	CBitmap			m_bmpFail;
	CBitmap			m_bmpRun;

	CString			m_sDatabaseName;

	CString			m_strWipId_Old;
	int				m_nOldFailedStep;
	//+change 090213(Modification No1)
	CWinThread*		m_pTestThread;

	//add 090903
//	CEditLog			m_ctrlComLog;
	HANDLE			m_hReadEvent_S6F5;
	HANDLE			m_hReadEvent_S6F12;
	HANDLE			m_hReadEvent_S6F2;
	BOOL			m_bResult_S6F5;
	BOOL			m_bResult_S6F12;
	BOOL			m_bResult_S6F2;

	int	m_nNoRepeatExecution;
	int m_nNoCurrentRepeatExecution;
	int m_nNoPassInRepeatExecution;
	int m_nNoFailInRepeatExecution;
	int m_nCurrentViewImageType;

	BOOL m_bContinuousLVDSGrabRunning;

	CString m_szResultMsg1;
	CString m_szResultMsg2;

	BOOL m_bExitFlag;
	CString		m_strFixtureId;			//Wip ID
	HACCEL			m_hAccel;

public :
//    IApplicationPtr     &GetActiveMILApplication();
//    ISystemPtr          &GetActiveMILSystem();
//    IDigitizerPtr       &GetActiveMILDigitizer();
//    IImagePtr			&GetActiveMILImage();
    BOOL IsDigitizerGrabbing();
    void HaltDigitizer();

	void GetRectSizeInfo();

	static UINT StartTestThread(LPVOID pParam); 

    void UpdateInfo(int nIndex, CString s);
	void DrawImage(int nDlgItemNo,CBaseImage& ImageData);
	BOOL InitAvSwitchController();
	void SetFont();
	BOOL OpenImgFile(CString sImgPath, int nImgType);
	void ContinuousLVDSGrab(); 

	//+add kwmoon 071015 : [2in1]
//	void ContinuousLVDSGrab_2in1(); 
	//-

	//+add kwmoon 080411
	void PlayWavSound(UINT nIDS); 

	//+add kwmoon 071015 : [2in1]
	void SetGrabInfo(CBaseImage* pBaseImage);
//	BOOL CopyDataToFileMappingObject();
//	BOOL CreateFileMappingObject();
	BOOL TransferData();
	//-

	//+add kwmoon 071204
	BOOL RenameMakeRefFolderName(); 

	//+add kwmoon 080229
	BOOL CopyReferenceImage();

	//+add kwmoon 071210
	void LoadRegistrySetting(CEnvironmentData* pCurrentSet); 
	void SaveRegistrySetting(); 

	// Message Handler
	afx_msg LRESULT  UpdateDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  CloseStopProcessingDlg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  ShowResultDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  RunTest(WPARAM wParam, LPARAM lParam);
	//+del kwmoon 080724
//	void InitPeripheralDevice(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  InitDeviceDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  RunContinuousLVDSGrab(WPARAM wParam, LPARAM lParam);
	//+add kwmoon 080527
	afx_msg LRESULT  OnUploadMesData(WPARAM wParam, LPARAM lParam);

	//+add kwmoon 080618
	afx_msg LRESULT  CloseResultDlg(WPARAM wParam, LPARAM lParam);

//	BOOL UserControlInit(BOOL bInit);

	//+add kwmoon 080529
	void InitSubClassItem();
	
	//+del kwmoon 080716
	//+add psh 080603
//	void StartTestRun(); 

	BOOL InitScanner(CString sComPort, DWORD wBaudRate);

	//+add kwmoon 080617
	void DrawRoiBox(CDC* pDC, CRect& rect,CBaseImage& ImageData);

//	static UINT GrabImageThread(LPVOID pParam); 

	//+change kwmoon 080804
	void SetStopStepDlgMsg(int nStepResult); 

	//+add kwmoon 080624
	BOOL RunInternalRetry();

	//+change kwmoon 080804
	BOOL RunUserRetry(int nStepResult); 

	//+add kwmoon 080716
	void SaveAdjSpecData();
	void SaveMakeRefData();

	//+add kwmoon 080724
	void GetApplicationVersion(); 

	//+add kwmoon 080828
	void InitStepVariable(); 

	afx_msg LRESULT  UpdateGrabImage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  OnUpdateGraph(WPARAM wParam, LPARAM lParam);
	void StartGrabThread();
	void StopGrabThread();
	void ShowSystemMsg(int nMgaNo); 
	void CloseSystemMsg(); 
	BOOL MesData_LocalSaveText(CString sCurentTime, CString strWipid, BOOL bResult, CString sTime);
	BOOL MesData_AutoUpLoad_DataCheck();
	//090402
	void SummaryGridDisabled(BOOL bFlag); 

	//+add 090526
//	int LocalDimmingTest();
	//+add 090615
	void AudioMeasureStart();
	void AudioMeasureStop();

	//add 090702
	void ComLogClear();
	void ComLog_AddString(CString szString);
	int AppendToLogAndScroll(CString str, COLORREF color = RGB(0, 0, 0));

	//add 090909
	void InitMainGrid(int nGridType);
	void InsertStepData2Grid(int nGridType);
	void InsertResultData2Grid(int nGridType, int nStepNo);
	void InsertResult2NormalGrid(int nStepNo);
	void InsertResult2DetailedGrid(int nStepNo);
	void InsertMsg2DetailedGrid(int nStepNo, CString sMsg);
	void UpdateGridStepTime(CString strStepTime, int nStepNo);

	void InsertTotalData2Grid(int nGridType, BOOL bResult, CString sElapsedTime);
	void InsertPcbid2Grid(CString sWipID);

	void DeleteResultDataFromGrid(int nGridType, int nStepNo);
	void DeleteResultDataFromNormalGrid(int nStepNo);
	void DeleteResultDataFromDetailedGrid(int nStepNo);
	void GetCheckStep(int nGridType);
	int GetNoCheckedStep(int nDisplayType);
	void SaveResult2File(CString sRstPath, BOOL bSave2Txt, BOOL bSave2Html, BOOL bSave2Csv);
	
//-

	BOOL GetResultFromDetailedGrid(int nStepNo,CString &sMsg);

//	void WaveRead_Start();
	int StepRun();
//	void ResetStep();

	BOOL OnSocketReceive();
	void Display_GMESInfo(BOOL bFlag);
	int UploadMasterTable(CString strWipid, BOOL bResult, CString sTime);
	void LogDataSave(CString sLogFilePath);
	BOOL GetToolOption();
	BOOL Check_MacAddress(CString sMac_Add);
	void UpdateToolOptionInfo();
	void InitVersionGrid();
	void UpdateVersionInfo();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDATsysView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDATsysView();
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDATsysView)
	afx_msg void OnEditCompile();
	afx_msg void OnEditCurrent();
	afx_msg void OnModelOpen();
	afx_msg LRESULT  OnChangeModelOpen(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRunAbort();
	afx_msg void OnRunNext();
	afx_msg void OnRunJump();
	afx_msg void OnRunPrev();
	afx_msg void OnRunRun();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnImageDialog();
	afx_msg void OnSeqEdit();
	afx_msg void OnOption();
	afx_msg void OnClickMainViewGrid();
	afx_msg void OnRunMakeRef();
	afx_msg void OnRunAdjSpec();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPwrOn();
	afx_msg void OnPwrOff();
	afx_msg void OnRefImage();
	afx_msg void OnRefAudio();
	afx_msg void OnGrabStart();
	afx_msg void OnGrabStop();
	afx_msg void OnEnter();
	afx_msg void OnExit();
	afx_msg void OnMesDataUpload();
	afx_msg void OnModelRegistration();
	afx_msg void OnAudioMeasStart();
	afx_msg void OnAudioMeasStop();
	afx_msg void OnScannerEnable();
	afx_msg void OnScannerDisable();
	afx_msg void OnSelchangeViewTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSeqLock();
	afx_msg void OnClose();
	afx_msg void OnInput();
	afx_msg void OnAudioOutlStart();
	afx_msg void OnAudioOutlStop();
	afx_msg void OnAudioOutrStart();
	afx_msg void OnAudioOutrStop();
	afx_msg void OnJigUp();
	afx_msg void OnJigupAll();
	afx_msg void OnJigupGood();
	afx_msg void OnJigupNot();
	afx_msg void OnStartAuto();
	afx_msg void OnStartManual();
	afx_msg void OnCheckMeson();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg LRESULT OnCommunication(WPARAM wParam, LPARAM lParam);
	//+ 2007.11.5 Add BY USY
	afx_msg LRESULT OnProcessMessage(WPARAM wParam, LPARAM lParam);
	//-
	//+add 090902
	afx_msg LRESULT ProcessKeyPad(WPARAM wParam, LPARAM lParam);//Keystroke message handler
public:
	int m_SoundInDeviceID;
	int mCheckAll;
	void UpdateSeqCheck();
	int m_LockStatus[500];

	void SelchangeSoudDevice(int lDeviceID);
private:
    CLedButton			m_LedsMesOn;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStepresult();
	CListCtrl m_CtrlListMainProcess;
	CListCtrlEx  m_CtrlListMainProcessEx;
//	CListCtrl m_CtrlListVersionProcess;
	CListCtrlEx  m_CtrlListVersionProcessEx;
	CListCtrl m_CtrlListVersionProcess;
	//CLabel_text m_cLabelSoundLeft;
	//CLabel_text m_cLabelSoundRight;
	CColorStaticST  m_cLabelSoundLeftFREQ;//IDC_TEXT_FREQ_LEFT // ctrlPLC_COM_Status.SubclassDlgItem(IDC_PLC_COM_STATUS,this);
	CColorStaticST  m_cLabelSoundLeftVolt;//IDC_TEXT_VOLT_LEFT
	CColorStaticST  m_cLabelSoundRightFREQ;//IDC_TEXT_FREQ_RIGHT
	CColorStaticST  m_cLabelSoundRightVolt;//IDC_TEXT_VOLT_RIGHT

	double m_Frequency_Left;
	double m_Frequency_Right;
	double m_Voltage_Left;
	double m_Voltage_Right;

	int m_bMeasureAudioType;
	int m_bAudio_Result;
	int m_nMeasureWait;
	int m_RunAudioMeasure;
	BOOL CDATsysView::AudioMeasure();
	float		m_fTargetLeftFreq;
	float		m_fTargetRightFreq;

	float		m_fTargetLeftLevel;
	float		m_fTargetRightLevel;

	float		m_fMeasuredLeftFreq;
	float		m_fMeasuredRightFreq;

	float		m_fMeasuredLeftLevel;
	float		m_fMeasuredRightLevel;

	float		m_fLeftFreqLowerLimit;
	float		m_fLeftFreqUpperLimit;
	float		m_fRightFreqLowerLimit;
	float		m_fRightFreqUpperLimit;

	float		m_fLeftLevelLowerLimit;
	float		m_fLeftLevelUpperLimit;
	float		m_fRightLevelLowerLimit;
	float		m_fRightLevelUpperLimit;

	//+change kwmoon 080724
//	float		m_fPercentMargin;
	float		m_fFreqMargin;

	//+add kwmoon 080724
	float		m_fLevelMargin;

	//CComboBox m_cComboSoundLeft;
	CComboBox m_cComboSoundRight;
	//afx_msg void OnSelchangeCmbSoudDevice();
	afx_msg void OnNMCustomdrawListMainProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHdnItemclickListMainProcess(NMHDR* pNMHDR, LRESULT* pResult);
	CRichEditCtrl m_ctrlEditComLog1;
	afx_msg void OnNMCustomdrawListVersionProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListMainProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMDblclkListMainProcess(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnCbnEditchangeComboSoundL();
};

#ifndef _DEBUG  // debug version in DATsysView.cpp
inline CDATsysDoc* CDATsysView::GetDocument()
   { return (CDATsysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATSYSVIEW_H__A91C394C_305C_11D3_90A6_006008CED7F4__INCLUDED_)
