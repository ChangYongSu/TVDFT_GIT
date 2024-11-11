//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_GRABPAGE_H__69649592_2A63_46CD_9B76_72136AD11D13__INCLUDED_)
#define AFX_GRABPAGE_H__69649592_2A63_46CD_9B76_72136AD11D13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrabPage.h : header file
//////////////////////////////////////////////////////////////////////////
#include "Global.h"
#include "PatternGeneratorCtl.h"
#include "ProgressCtrlX.h"
#include "ColorStaticST.h"	
#include "AvSwitchBoxCtrl.h"

extern CPatternGenerator	PatternGeneratorCtrl;
//extern CAnalogGrabber		AnalogControl;
extern CAvSwitchBox			AvSwitchBoxCtrl;

class CImageTotalDlg;
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CGrabPage dialog

class CGrabPage : public CDialog
{
// Construction
public:
	CGrabPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGrabPage)
	enum { IDD = IDD_GRAB_PAGE };
	CComboBox	m_ctrlLvdsModeCmb;
	CComboBox	m_ctrlAncInputCmb;
	CEdit	m_ctrlAvcInputSelectCmb;
	CButton	m_ctrlHdmiGrabChk;
	CComboBox	m_ctrlHdmiResolution;
	CComboBox	m_ctrlHdmiBitShift;
	CComboBox	m_ctrlUhdType;
	CComboBox	m_ctrlGrabMode;
	CComboBox	m_ctrlBitShift;
	CButton	m_ctrlMhlDetChk;
	CButton	m_ctrlComp;
	CButton	m_ctrlEarphone;
	CComboBox	m_ctrlHdmiOutPortCmb2;
	CButton	m_ctrlArcChk;
	CComboBox	m_ctrlHdmiOutPortCmb;
	CButton	m_ctrlOddDE_Only;
	CButton	m_ctrlDDCLineChk;
	CButton	m_ctrlHdcpChk;
	CButton	m_ctrlEdidChk;
	CEdit	m_ctrlPatternEdit2;
	CButton	m_ctrlOnlineChk2;
	CEdit	m_ctrlModelEdit2;
	CEdit		m_ctrlGrabStatus;
	CTabCtrl	m_ctrlControlTab;
	CEdit		m_ctrlSendRemoteRtnEdit;
	CEdit		m_ctrlAvSwitchRtnEdit;
	CEdit		m_ctrlAvSwitchRstEdit;
	CEdit		m_ctrlNoGrabFrame;
	CStatic		m_ctrlAvSwitchVal2Name;
	CStatic		m_ctrlAvSwitchVal1Name;
	CComboBox	m_ctrlAvSwitchVal2Cmb;
	CComboBox	m_ctrlAvSwitchVal1Cmb;
	CButton		m_ctrlSaveAsBtn;
	CButton		m_ctrlVideoOutGrabChk;
	CButton		m_ctrlLvdsGrabChk;
	CButton		m_ctrlGrabBtn;
	CButton		m_ctrlOnLineChk;
	CEdit		m_ctrlModelEdit;
	CEdit		m_ctrlPatternEdit;
	CEdit		m_ctrlRemoteCodeEdit;
	CButton		m_ctrlManualGrabChk;
	CButton		m_ctrlAutoGrabChk;
	int			m_nAnalogSignalType;
	CString		m_szGrabStatus;
	int			m_nLvdsResolutionIndex;
	int			m_nUranousMode;
	int			m_nBitMode;
	BOOL	m_bOddDE_Only;
	BOOL	m_bImage_Rotation;
	int		m_n3DMode;
	CButton	m_ctrlRelay1Chk;
	BOOL	m_bRelay1;
	int		m_nMhlMode;
	BOOL	m_bHdmiGenArc;
	BOOL	m_bMhlDetect;
	BOOL	m_bEarphone;
	BOOL	m_bComp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrabPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGrabPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkAutoGrab();
	afx_msg void OnChkManualGrab();
	afx_msg void OnBtnManualGrab();
	afx_msg void OnBtnSendRemoteCode();
	afx_msg void OnBtnOpenRemoteDlg();
	afx_msg void OnChkOnline();
	afx_msg void OnBtnSendPattern();
	afx_msg void OnBtnSendModel();
	afx_msg void OnBtnSaveBmp();
	afx_msg void OnClose();
	afx_msg void OnChkLvdsGrab();
	afx_msg void OnChkHdmiGrab();
	afx_msg void OnChkVideoOutGrab();
	afx_msg void OnRadioNtsc();
	afx_msg void OnRadioPal();
	afx_msg void OnRdoAvswitchCmd1();
	afx_msg void OnRdoAvswitchCmd2();
	afx_msg void OnRdoAvswitchCmd3();
	afx_msg void OnRdoAvswitchCmd4();
	afx_msg void OnRdoAvswitchCmd5();
	afx_msg void OnRdoAvswitchCmd6();
	afx_msg void OnRdoAvswitchCmd7();
	afx_msg void OnRdoAvswitchCmd8();
	afx_msg void OnRdoAvswitchCmd9();
	afx_msg void OnRdoAvswitchCmd10();
	afx_msg void OnRdoAvswitchCmd11();
	afx_msg void OnBtnSendAvswitchCmd();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCmbAvswitchValue1();
	afx_msg void OnSelchangeCmbAvswitchValue2();
	afx_msg void OnRdoAvswitchCmd12();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRemoteButtonClick(UINT nButtonID);
	afx_msg void OnSelchangeControlTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnGrabOptionApply();
	//afx_msg void OnRdoLvdsResolution1(UINT nButtonID);
	afx_msg void OnRdoColorDepth1(UINT nButtonID);
	afx_msg void OnRdo3dMode1(UINT nButtonID);
	afx_msg void OnRdoAvswitchCmd13();
	afx_msg void OnChkEdid();
	afx_msg void OnChkHdcp();
	afx_msg void OnChkOnline2();
	afx_msg void OnBtnSendModel2();
	afx_msg void OnBtnSendPattern2();
	afx_msg void OnChkDdcline();
	afx_msg void OnRadioSecam();
	afx_msg void OnChkOddDeOnly();
	afx_msg void OnChkImageRotation();
	afx_msg void OnRdoAvswitchCmd14();
	afx_msg void OnSelchangeComboOutPort();
	afx_msg void OnCheckRelay1();
	afx_msg void OnRadioMhl(UINT nButtonID);
	afx_msg void OnCheckArc();
	afx_msg void OnCheckMhlLine();
	afx_msg void OnBtnAvcCom();
	afx_msg void OnSelchangeCmbAvcInput();
	afx_msg void OnBtnAvcIr();
	afx_msg void OnBtnAvcBuf();
	afx_msg void OnCheckEarphone();
	afx_msg void OnCheckComp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

		CBaseImage		m_Image1;

		int				m_nTabType;		
		int				m_nAvSwitchCmd;

		UINT			m_nPatternMode;	//0 : offLine, 1 : online
		UINT			m_nGrabMode;	//0 : Manual Grab, 1 : Auto Grab
		UINT			m_nGrabDevice;	//0 : LVDS Grab, 1 : VideoOut Grab
			
		CImageTotalDlg*		m_MWnd;
		CProgressCtrlX*		m_ctrlProgress;
		CStopProcessingDlg* m_pSystemMessageDlg;

		BOOL			m_bThreadRunning;
		BOOL			m_bStopGrabThread;
//		BOOL			m_bStopHDMIThread;
//		BOOL			m_bStopAnalogThread;
		BOOL			m_bGrabSingleFrame;
		HANDLE			m_hPageGrabThreadKillEvent;
	
		//+change 090213(Modification No1)
		//	HANDLE			m_hThread;
		CWinThread*		m_pGrabImageThread_UHD;
		CWinThread*		m_pGrabImageThread_FHD;

		//+add kwmoon 080710
		int				m_nNewBitCount;
		int				m_nNewLvdsWidth;
		int				m_nNewLvdsHeight;
		int				m_nNewLVDSMode;

		static UINT	GrabImageThread_UHD(LPVOID pParam);
		static UINT GrabImageThread_FHD(LPVOID pParam);

		BOOL InitGrabber(int nGrabberType);
		BOOL InitPatternGenerator();
		BOOL InitAvSwitchController();
		BOOL FindSourceKey(int nSource, CString& strName, CString& strCode, CString& strDesc);
		BOOL SendInputKey(int nSource);
		BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCode, CString& strDesc);
		BOOL SendButtonKey(CString strName);

		void GrabStart(int nType);
		void GrabStop();
		void SetAvSwitchValueCmb(int nCmdType);
		void ShowSystemMsg(); 
		void CloseSystemMsg(); 
		LRESULT UpdateGrabStatus(WPARAM wParam, LPARAM lParam);
		void ChkLVDSAutoGrab(); 
		void ShowAvSwitchBoxCtrlGroup(BOOL bShow);
		void ShowGrabberOptionGroup(BOOL bShow);
		void SetHDMIGenGroup(BOOL bEnable);
		void ShowUsbDioGroup(BOOL bShow);

		CButton m_ctrlCECChk;
		afx_msg void OnBnClickedChkCec();
		CButton m_ctrlGrabPower;
		BOOL m_bGrabPower;
		afx_msg void OnBnClickedCheckGrabPower();
		afx_msg void OnBnClickedBtnResetIfpack();
		afx_msg void OnBnClickedBtnResetGender();
		afx_msg void OnBnClickedCheckHdmiUsbCyl();
		CButton m_cCheckHdmiUsbCyl;
		BOOL m_bCheckHdmiUsbCyl;
		afx_msg void OnBnClickedBtnMainJigUp();
		afx_msg void OnBnClickedBtnMainJigDown();



		afx_msg void OnBnClickedBtnCommandModelName();
		afx_msg void OnBnClickedBtnCommandLoad();
		afx_msg void OnBnClickedBtnCommandPassUnload();
		afx_msg void OnBnClickedBtnCommandNgUnload();
		//	afx_msg void OnBnClickedBtnCommandNgLoad2();
		afx_msg void OnBnClickedBtnCommandFfcRetry();
		afx_msg void OnBnClickedBtnCommandRead();
		CString m_strEditModelName;

		CButton m_cCheckLoad;
		CButton m_cCheckUnLoad;
		CButton m_cCheckTestPass;
		CButton m_cCheckTestNg;
		CButton m_cCheckReconnect;
		CButton m_cCheckLoadFinish;
		CButton m_cCheckUnLoadFinish;
		CButton m_cCheckReconnectFinish;	
		
		CButton m_cCheck_PLC_Auto_Mode;
		CButton m_cCheck_PLC_Manual_Mode;
		CButton m_cCheck_PLC_Start_Run;
		CButton m_cCheck_PLC_Pause;
		CButton m_cCheck_PLC_STOP;
		CButton m_cCheck_PLC_Warn;
		CButton m_cCheck_PLC_Alarm;
		CButton m_cCheck_PLC_Data_PCB_Inside;
		CButton m_cCheck_PLC_Data_DummyCut;
		CButton m_cCheck_PLC_Data_OK;
		CButton m_cCheck_PLC_Data_NG;
		BOOL m_bRedBlueSwap;
		afx_msg void OnBnClickedChkRbSwapGrab();
		BOOL m_bDeHsyncEnable;
		BOOL m_bHyncEdgeEnable;
		afx_msg void OnBnClickedChkDeHsyncGrab();
		afx_msg void OnBnClickedChkHsyncEdgeGrab();
		CComboBox m_cComboY20SwMode;
		afx_msg void OnBnClickedBtnResetGrab();
		afx_msg void OnCbnSelchangeComboGrabmode2();
		afx_msg void OnCbnSelchangeComboUhdtype2();
		afx_msg void OnBnClickedBtnLanTest();
		afx_msg void OnBnClickedBtnLanClear();
		afx_msg void OnBnClickedBtnDpmsCheck();
		CComboBox m_cComboLvdsFormat;
		afx_msg void OnCbnSelchangeComboLvdsResolution2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRABPAGE_H__69649592_2A63_46CD_9B76_72136AD11D13__INCLUDED_)
