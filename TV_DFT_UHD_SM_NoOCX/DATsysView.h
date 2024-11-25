// DATsysView.h : interface of the CDATsysView class
//
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INCLUDES()
#include "vsflexgrid.h"
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
#include "CUserMsgColorDlg.h"
#include "resource.h"


#include "ListCtrlEx.h"

//+add kwmoon 080826
#include "I2cAdcManualCheckDlg.h"
//+add kwmoon 080904
#include "Rs232cAdcManualCheckDlg.h"
#include "GetToolOptionDlg.h"
#include "SelectChassisModelDlg.h"

//+add kwmoon 071015 : [2in1]
#include "Defines.h"
#include "Aclapi.h"
//-
#include "StaticFilespec.h"
#include "KeyDownloadCheckDlg.h"
#include "InputDetectCheckDlg.h"
#include "VfMeasureCheckDlg.h"

//+add psh 090702
//#include "EditLog\EditLog.h"
//#include "EditLog\editlog_stream.h"
#include "MyEchoSocket.h"
#include "ButtonST\LedButton.h"
#include "PciExpWrapper.h"

#if !defined(AFX_DATSYSVIEW_H__A91C394C_305C_11D3_90A6_006008CED7F4__INCLUDED_)
#define AFX_DATSYSVIEW_H__A91C394C_305C_11D3_90A6_006008CED7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef struct tagBITMAPINFO8 {
	BITMAPINFOHEADER    bmiHeader;
	RGBQUAD             bmiColors[256];
} BITMAPINFO8;

//class CBaseThread;
class CEnvironmentData;
class CVFTestInfo;

class CDATsysView : public CFormView
{
protected: // create from serialization only
	CDATsysView();
	DECLARE_DYNCREATE(CDATsysView)

public:
	//{{AFX_DATA(CDATsysView)
#ifdef _DFT_3IN1
	enum { IDD = IDD_DATSYS_FORM_3IN1 };
#endif

#ifdef _DFT_2IN1
	enum { IDD = IDD_DATSYS_FORM_2IN1 };
#endif

//	CEdit	m_ctrlEditComLog;
	CTabCtrl	m_ctrlViewTab;
	BOOL	m_bInput_Lock;
	BOOL	m_bSeq_Lock;
	//CVSFlexGrid	m_ctrlSummaryGrid;
	//CString	m_szGrabberVersion;
	int		m_nToolOptionMethod;
	//CVSFlexGrid	m_ctrlOptionGrid;
	//CVSFlexGrid	m_ctrlCountryGrid;
	//CVSFlexGrid	m_ctrlVersionGrid;
	//}}AFX_DATA

// Attributes
public:
	CDATsysDoc* GetDocument();

public:
	int m_InitDialogEnd;

	MyEchoSocket m_pSocketClient;
	MyEchoSocket m_pPLCSocketClient;
	MyEchoSocket m_pMonitorServerSocketClient;
	CPciExpWrapper		m_clsPCI			;

	// ANSI and UNICODE stream buffers for the EditLogger
//	std::editstreambuf	m_EditStrBuf;
//	std::weditstreambuf m_EditStrBufW;

	// Used to save the previos values for cout and wcout
//	std::basic_streambuf<char>*		m_pOldBuf;
//	std::basic_streambuf<wchar_t>*	m_pOldBufW;

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

//	CRect m_RemoteComLogClientRect;

	CWnd* m_pVideoViewWnd;
	CWnd* m_pStatusViewWnd;
	CWnd* m_pElapsedTimeWnd;
//	CWnd* m_pRemoteComLogWnd;


	BOOL m_bGetRectSizeInfo;
	BOOL m_bTerminalModeOn;
	BOOL m_bThreadRunning;

	BOOL m_bGrabThreadRunning;
	BOOL m_bGrabConnection;
	BOOL m_bStopLVDSThread;
	
	BOOL m_bAdcRunning;

	BOOL m_bVFTestThreadRunning;
	BOOL m_bStopVFTestThread;

	CMainFrame* m_pFrame;

	//+change kwmoon 080724
//	CString m_szMessage;
	CString m_szCurrentStatus;

	CFont	m_CustomFont;
	CFont	m_StatusWindowFont;
	CFont	m_ProgressFont;

	CFont	m_PLC_Font;
	CFont	m_Step_New_Font;

	CFont	m_OptionList_Font;



	// Modeless Dialog
	CStopProcessingDlg*	m_pStopProcessingDlg;
	CInitDlg*			m_pInitDlg;
	CKeyDownloadCheckDlg* m_pKeyCheckDlg;
	BOOL		m_bKeyDLCheckResultList[200];
	CInputDetectCheckDlg m_pInputCheckDlg;
	CVfMeasureCheckDlg m_pVfMeasureCheckDlg;
	// Modal Dialog
	CManualTestResultInputDlg	m_ManualTestDlg;
	CStopStepDlg				m_StopStepDlg;
	CResultDlg*					m_ResultDlg;
	//+add 090902
	CUserMsgDlg					m_UserMsgDlg;
	CUserMsgColorDlg			m_UserColorMsgDlg;

	//+add kwmoon 080826
	CI2cAdcManualCheckDlg		m_I2cAdcManualCheckDlg;

	//+add kwmoon 080904
	CRs232cAdcManualCheckDlg	m_Rs232cAdcManualCheckDlg;
	
	GetToolOptionDlg			m_GetToolOptiondlg;
	CSelectChassisModelDlg		m_SelectChassisModelDlg;

	CColorStaticST	m_ctrLEDTested;
	CColorStaticST	m_ctrLEDFailed;
	CColorStaticST	m_ctrLEDRate;


	CBaseImage		m_Image[5];
//	CBaseImage		m_GrabImage;

	CString			m_szExePath;
	CString			m_szNoImageBmpPath;

	CString			m_szMakeRefPath;

	int				m_bMakeReferenceMode;

	CStringArray	m_aRefIntFileArray;

	// FileMapping Handle
	HANDLE  m_hImageDataFileMapping;
	HANDLE  m_hImageInfoFileMapping;

	// FileMapping Object Creation Flag
	BOOL	m_IsFileImageDataMappingCreated;
	BOOL	m_IsFileImageInfoMappingCreated;
	
	// FileMapping GrabberOption
	ImageInfoStruct*	m_pImageInfo;


	UsbDioStruct* m_pUsbDio_Control;
	HANDLE  m_hUsbDioCtrlFileMapping;
	BOOL	m_IsFileUsbDioCtrlMappingCreated;
	long	m_BufferUsbDioCtrl_Size;

	MhlControlStruct*	m_pMhlCtrl;
	HANDLE  m_hMhlCtrlFileMapping;
	BOOL	m_IsFileMhlCtrlMappingCreated;
	long	m_BufferMhlCtrl_Size;

	HANDLE  m_hBarcodeDataFileMapping;
	BOOL	m_IsBarcodeDataMappingCreated;
	BarcodeStruct* m_pBarcodeData;

	//HANDLE  m_hUSB_MICFileMapping;
	//BOOL	m_IsUSB_MICMappingCreated;
	//USB_MIC_Struct* m_pUSB_MIC_Struct;
	//int nMicLive;
//	HANDLE  m_hDaqDataFileMapping;
//	BOOL	m_IsDaqDataMappingCreated;
	//DaqStruct* m_pDaqData;


	// CBaseThread* m_pBaseThread;
//	CBaseThread* m_pJoyStickThread;

	BOOL	m_Grab1RunningFlag;
	BOOL	m_Grab2RunningFlag;
	BOOL	m_Grab3RunningFlag;
//add 090526
//	BOOL	m_Dimming1RunningFlag;
//	BOOL	m_Dimming2RunningFlag;

	BOOL	m_bResultDlgActivate;
	BOOL	m_bInputCheckDlgActivate;
	BOOL	m_bUseDefaultREFfolder;

	HANDLE	m_hGrabThreadKillEvent;

	CWinThread* m_pGrabImageThread;

	//+add kwmoon 080618
	int		m_nGrabType;
	int		m_nGrabResult;
	BOOL	m_bStopMeasureAudio;

	//+add kwmoon 080710
	CStdioFile	m_pTimeDataFile;
	BOOL	m_bTimeDataFileOpen;
	DWORD	m_dwStepElapsedTime;
	CString m_sTimFileName;


	//+add kwmoon 080724
	CString	m_szVersion;
	CStopProcessingDlg* m_pSystemMessageDlg;

	//CStatic			m_cStepResult;
	CColorStaticST		m_cStepResultNew;
	//CBitmap			m_bmpNormal;
	//CBitmap			m_bmpGood;
	//CBitmap			m_bmpFail;
	//CBitmap			m_bmpRun;

	CString			m_sDatabaseName;

	CString			m_strWipId_Old;
	int				m_nOldFailedStep;
	//+change 090213(Modification No1)
	CWinThread*		m_pTestThread;
	CWinThread*		m_pVFTestThread;

	//add 090903
//	CEditLog			m_ctrlComLog;
	//+add psh 101101
	int m_nGrabber_FWVer;
	//+add psh 101220
	HACCEL			m_hAccel;

	BOOL			m_bAcPowerOnOff;
	BOOL			m_bMhlMode;

	HANDLE			m_hReadEvent_S6F5;
	HANDLE			m_hReadEvent_S6F12;
	HANDLE			m_hReadEvent_S6F2;
	BOOL			m_bResult_S6F5;
	BOOL			m_bResult_S6F12;
	BOOL			m_bResult_S6F2;
	CString			m_sGrabberFW_Version;

	HANDLE	m_hVfThreadKillEvent;

	CString	m_sTempModelSuffix;


	CString m_A_CAS_ID_Check;

public :

	int	m_nNoRepeatExecution;
	int m_nNoCurrentRepeatExecution;
	int m_nNoPassInRepeatExecution;
	int m_nNoFailInRepeatExecution;
	int m_nCurrentViewImageType;

	int m_nCurrentResetExecution;

	BOOL m_bContinuousLVDSGrabRunning;

	CString m_szResultMsg1;
	CString m_szResultMsg2;

//	static void	FillBmInfo			(BITMAPINFO8* bmi, int width, int height, int bpp, int origin = 0);

	void GetRectSizeInfo();

	static UINT StartTestThread(LPVOID pParam); 
	//static UINT TestThread_EM(LPVOID pParam); 

    void UpdateInfo(int nIndex, CString s);
	void DrawImage(int nDlgItemNo,CBaseImage& ImageData);
	BOOL InitAvSwitchController();
	void SetFont();
	BOOL OpenImgFile(CString sImgPath, int nImgType);
	void ContinuousLVDSGrab(); 

	//+add kwmoon 071015 : [2in1]
	void ContinuousLVDSGrab_FHD(); 
	BOOL Grab_UHD();
	int Grab_FHD(int nType,int nGrabMode = EXT_LVDS_GRABBER);
	//-

	//+add kwmoon 080411
	void PlayWavSound(UINT nIDS); 

	//+add kwmoon 071015 : [2in1]
	void SetGrabInfo(CBaseImage* pBaseImage);
	void InitEventAndFileMappingObject();
	void InitEventAndFileMappingObject_FHD();
//	static UINT ThreadProc(LPVOID param);
	BOOL CopyDataToFileMappingObject();
	BOOL CreateFileMappingObject();
	BOOL CreateFileMappingObject_FHD();
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
	LRESULT  UpdateDisplay(WPARAM wParam, LPARAM lParam);
	LRESULT  CloseStopProcessingDlg(WPARAM wParam, LPARAM lParam);
	LRESULT  ShowResultDisplay(WPARAM wParam, LPARAM lParam);	
	LRESULT  RunTest(WPARAM wParam, LPARAM lParam);
	BOOL Read_PCBAID_READ();
	//BOOL Read_No_GMES_PCBAID_READ();
	LRESULT  InitDeviceDialog(WPARAM wParam, LPARAM lParam);
	LRESULT  RunContinuousLVDSGrab_FHD(WPARAM wParam, LPARAM lParam);
	LRESULT  OnUploadMesData(WPARAM wParam, LPARAM lParam);
	LRESULT  CloseResultDlg(WPARAM wParam, LPARAM lParam);

	void InitSubClassItem();
	

	BOOL InitScanner(CString sComPort, DWORD wBaudRate);

	void DrawRoiBox(CDC* pDC, CRect& rect,CBaseImage& ImageData);

	static UINT GrabImageThread(LPVOID pParam); 
	static UINT GrabImageThread_FHD(LPVOID pParam); 

	void SetStopStepDlgMsg(int nStepResult); 

	BOOL RunInternalRetry();

	BOOL RunUserRetry(int nStepResult); 

	void SaveAdjSpecData();
	void SaveMakeRefData();

	void GetApplicationVersion(); 

	void InitStepVariable(); 

	int DFT3_UHDGrabStartLocal(int nModel, int nShiftVal, int nWidth, int nHeight, int nOffset, int nDelay, int nLvdsType);
	int IF_Pack_Reset();
	int CDATsysView::CheckGrabberStatus();
	LRESULT  UpdateGrabImage(WPARAM wParam, LPARAM lParam);
	void StartLVDSGrabThread();
	void StopLVDSGrabThread();
	void StartLVDSGrabThread_FHD();
	void StopLVDSGrabThread_FHD();
	void ResetGrabStartThread();
	void GrabBaseResetStartThread();
	void GrabBaseReset();

	void ShowSystemMsg(int nMgaNo); 
	void CloseSystemMsg(); 
	BOOL MesData_LocalSaveText(CString sCurentTime, CString strWipid, BOOL bResult, CString sTime);
	BOOL MesData_AutoUpLoad_DataCheck();
	//090402
	void SummaryGridDisabled(BOOL bFlag); 

	//+add 090526
//	int LocalDimmingTest(int nCh, int nAdd, int nCmd);
	int LocalDimmingTest();
	//+add 090615
	void AudioMeasureStart();
	///BOOL AudioMeasureStartUSB(CString strSoundInDevice);
	//BOOL AudioMeasureSearch();
	void AudioMeasureStop();

	//add 090702
	void ComLogClear();
	void ComLog_AddString(CString szString);
	void SaveMessageList(CString szString);
	//add 090909
	void InitMainGrid(int nGridType);
	void InsertStepData2Grid(int nGridType);
	void CDATsysView::InsertStepData2GridUpdate(int nGridType, int lStartStepNo);
	void InsertResultData2Grid(int nGridType, int nStepNo);
	//void InsertResultData2Grid_EM(int nFailNo);
	//void InsertResult2NormalGrid(int nStepNo);
	void InsertResult2DetailedGrid(int nStepNo);
	void InsertMsg2DetailedGrid(int nStepNo, CString sMsg);

	void InsertTotalData2Grid(int nGridType, BOOL bResult, CString sElapsedTime);
	void InsertPcbid2Grid(CString sWipID);

	void DeleteResultDataFromGrid(int nGridType, int nStepNo);
	void DeleteResultDataFromNormalGrid(int nStepNo);
	void DeleteResultDataFromDetailedGrid(int nStepNo);
	void GetCheckStep(int nGridType);
	int GetNoCheckedStep(int nDisplayType);
	void SaveResult2File(CString sRstPath, BOOL bSave2Txt, BOOL bSave2Html, BOOL bSave2Csv);

	void SetSubListMainControl(int nGridType);
	void SetListMainControl(int nGridType);



//-

	BOOL GetResultFromDetailedGrid(int nStepNo,CString &sMsg);

	int GetGrabberVersion_FHD();
	int Set_Relay(int nCmd);
	int Set_Mhl(int nFunc, int nCmd);

	void ProcessJigButton(char nData);
	int  GetBarcodeData(CString& sData);
	void JigStatus_Process(int nJigStatus); 
	double  GetDaqData(int nFunc, int nCh_S, int nCh_E, CString& sData);


	void ProcessStartBox(LONG lBTN);

	int StepRun();
	void ResetStep();
	void ResetStep_EM();

	BOOL OnSocketReceive();
	BOOL OnPLCSocketReceive();
	BOOL OnMonitorSocketReceive();
	void Display_GMESInfo(BOOL bFlag);
	int UploadMasterTable(CString strWipid, BOOL bResult, CString sTime);
	void UpdateToolOptionInfo(BOOL bSaveFlag);
	void LogDataSave(CString sLogFilePath);
	BOOL StepCheck();
	BOOL GetToolOption();
	BOOL CheckNgCount();
	void InitOptionGrid();
	void InitCountryGrid();
	void InitVersionGrid();
	BOOL GrabberOpen();
	BOOL GrabberClose();
	BOOL GrabberReset();
	BOOL CheckToolCrc();
	BOOL Check_KeyDownload();
	BOOL Check_InputDetect();
#ifdef SM_MODIFY_CODE__
	BOOL Check_HP_Disable();
	BOOL Loop_Auto_Check();
#endif
	BOOL Check_VfMeasure();
	BOOL Check_IR_Blaster();
	BOOL Read_A_CAS_ID();
	BOOL ReportSend_NgCount();
	int AppendToLogAndScroll(CString str, COLORREF color =RGB(0, 0, 0));
	int GetNumVisibleLines(CRichEditCtrl* pCtrl);
	void InitGrabImageEvent();

	void StartVFTestThread();
	BOOL ReadRegVal_Agent(char *val);

	BOOL AutoControlProcess();
	BOOL ServerControlReport();

	void ServerSendModelList();
	BOOL ServerChangeModel();
	BOOL ServerChangeModelAck(int lOK);

	int ProcessAutokey(LONG nStatus);

	int m_RestButtonStatus;

	void AddStringToStatusLocal(CString szString);
	CString m_LogString;
	CString m_LogBackupString;
	int m_bWriteMassageEnable;
	int m_bWriteMassageFlag;


	void CDATsysView::SetScroll( int lScroll);
	void CDATsysView::UpdateMainList(int lnStep);

	void CDATsysView::CheckFWVer();
	void CDATsysView::DisplayJigCount(int lJigID_UseCount);
public:
	int m_LockStatus[500];
	void UpdateSeqCheck();
	int m_CurrentStep;
	int mCheckAll;
	int m_BatVerReadOnly;

	int m_ListUpdateFlag;

	int m_BlackPictureFlag;
	int m_WhiteResetFlag;
	int m_WhiteResetCount;

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRunAbort();
	afx_msg void OnRunNext();
	afx_msg void OnRunJump();
	afx_msg void OnRunPrev();
	afx_msg void OnRunRun();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnImageDialog();
	afx_msg void OnSeqEdit();
	afx_msg void OnOption();
	afx_msg void OnClickMainViewGrid();
	afx_msg void OnClickOptionGrid();
	afx_msg void OnClickCountryGrid();
	afx_msg void OnClickVersionGrid();
	

	afx_msg void OnRunMakeRef();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPwrOn();
	afx_msg void OnRefImage();
	afx_msg void OnGrabStart();
	afx_msg void OnGrabStop();
	afx_msg void OnEnter();
	afx_msg void OnExit();
	afx_msg void OnInputLock();
	afx_msg void OnMesDataUpload();
	afx_msg void OnModelRegistration();
	afx_msg void OnAudioMeasStart();
	afx_msg void OnAudioMeasStop();
	afx_msg void OnScannerEnable();
	afx_msg void OnScannerDisable();
	afx_msg void OnSelchangeViewTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSeqLock();
	afx_msg void OnClose();
	afx_msg void OnDmmcotrol();
	afx_msg void OnImgRotate();
	afx_msg void OnBtnGetOption();
	afx_msg void OnRdoToolOption1(UINT nButtonID);
	afx_msg void OnCheckMeson();
	afx_msg void OnJigdownAll();
	afx_msg void OnJigdownGood();
	afx_msg void OnJigdownNot();
	afx_msg void OnButton2();
//	afx_msg void OnBtnCheckCrc();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	afx_msg long OnCommunication(WPARAM wParam, LPARAM lParam);
	afx_msg long OnUpdateLog(WPARAM wParam, LPARAM lParam);
	//+ 2007.11.5 Add BY USY
	afx_msg long OnProcessMessage(WPARAM wParam, LPARAM lParam);
	//-
	//+add 090902
	afx_msg LRESULT ProcessKeyPad(WPARAM wParam, LPARAM lParam);//Keystroke message handler

private:
    CLedButton			m_LedsMesOn;

	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl m_ctrlEditComLog;

	afx_msg void OnAcpowerFail();
	afx_msg void OnAcpowerGood();
	afx_msg void OnAcpowerAll();
	afx_msg void OnAcpowerNot();
	afx_msg void OnAnalyseVf();
	afx_msg void OnVfControl();
	afx_msg void OnGrabresetAuto();
	afx_msg void OnUpdateGrabresetAuto(CCmdUI *pCmdUI);
	afx_msg void OnGrabresetManual();
	afx_msg void OnUpdateGrabresetManual(CCmdUI *pCmdUI);
	afx_msg void OnStnClickedRobotStatus();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonAutoRobotReset();
	CButton m_cBtnResetRobot;

	CButton m_cBtnStopRobot;
	afx_msg void OnBnClickedButtonAutoRobotStop();
	afx_msg void OnBnDoubleclickedButtonAutoRobotStop();
	afx_msg void OnStnClickedPlcComStatus();
	//CListCtrl m_CtrlListMainProcess;
	afx_msg void OnBnClickedButtonServerConnect();
	afx_msg void OnOptionVfoption();
	CListCtrl m_CtrlListMainProcess;
	CListCtrlEx  m_CtrlListMainProcessEx;
	afx_msg void OnNMCustomdrawListMainProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListMainProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ctrlListOption;
	//CListCtrl m_ctrlListCountry;
	CListCtrl m_ctrlListVersion;
	afx_msg void OnHdnItemclickListMainProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListOptinProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawListVersionProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonBatVer();
	CEdit CtrlBatVerEdit;
	CEdit m_CEdit_EPI_CFG_SW;
};

#ifndef _DEBUG  // debug version in DATsysView.cpp
inline CDATsysDoc* CDATsysView::GetDocument()
   { return (CDATsysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATSYSVIEW_H__A91C394C_305C_11D3_90A6_006008CED7F4__INCLUDED_)
