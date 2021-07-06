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
#include "AnalogGrabber.h"
#include "AvSwitchBoxCtrl.h"

//////////////////////////////////////////////////////////////////////////
#define MANUAL_GRAB		0
#define AUTO_GRAB		1

#define GRAB_STOP		0
#define GRAB_START		1

#define NO_AVSWITCHBOX_TAB_ITEM	27 //24 //20  //19

//+change kwmoon 080923
// #define NO_GRAB_OPTION_TAB_ITEM 21
//+change psh 091214
//#define NO_GRAB_OPTION_TAB_ITEM 23
#define NO_USB_DIO_TAB_ITEM 7

/////////////////////////////////////////////////////////////////////////////


extern CPatternGenerator	PatternGeneratorCtrl;
extern CAnalogGrabber		AnalogControl;
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
	CEdit	m_ThDataOut;
	CButton	m_ctrlSelectAll;
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
	CStatic		m_ctrlAvSwitchVal2Name;
	CStatic		m_ctrlAvSwitchVal1Name;
	CComboBox	m_ctrlAvSwitchVal2Cmb;
	CComboBox	m_ctrlAvSwitchVal1Cmb;
	CButton		m_ctrlSaveAsBtn;
	CButton		m_ctrlVideoOutGrabChk;
	CButton		m_ctrlGrabBtn;
	CButton		m_ctrlOnLineChk;
	CEdit		m_ctrlModelEdit;
	CEdit		m_ctrlPatternEdit;
	CEdit		m_ctrlRemoteCodeEdit;
	CButton		m_ctrlManualGrabChk;
	CButton		m_ctrlAutoGrabChk;
	int			m_nAnalogSignalType;
	CString		m_szGrabStatus;
	BOOL	m_bRelay1;
	BOOL	m_bRelay2;
	BOOL	m_bRelay3;
	BOOL	m_bRelay4;
	CEdit	m_ctrlIrDataOut;
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRemoteButtonClick(UINT nButtonID);
	afx_msg void OnSelchangeControlTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChkEdid();
	afx_msg void OnChkHdcp();
	afx_msg void OnChkOnline2();
	afx_msg void OnBtnSendModel2();
	afx_msg void OnBtnSendPattern2();
	afx_msg void OnChkDdcline();
	afx_msg void OnRadioSecam();
	afx_msg void OnChkOddDeOnly();
	afx_msg void OnChkImageRotation();
	afx_msg void OnCheckSelectAll();
	afx_msg void OnButtonSet2();
	afx_msg void OnBtnAvcBuf();
	afx_msg void OnBtnAvcIr();
	afx_msg void OnBtnTh();
	afx_msg void OnBtnAvcIr2();
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
		BOOL			m_bStopLVDSThread;
		BOOL			m_bStopAnalogThread;
//		BOOL			m_bGrabSingleFrame;
		HANDLE			m_hPageGrabThreadKillEvent;
	
//		CWinThread*		m_pGrabImageThread_2in1;

		int				m_nNewBitCount;
		int				m_nNewLvdsWidth;
		int				m_nNewLvdsHeight;
		int				m_nNewLVDSMode;
		int				m_nCustomCode;
		int				m_nRmoteType;

		BOOL GrabImage_2in1();

		BOOL InitGrabber(int nGrabberType);
		BOOL InitPatternGenerator();
		BOOL InitAvSwitchController();
		BOOL FindSourceKey(int nSource, CString& strName, CString& strCode, CString& strDesc, int& nByte);
		BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCustomCode, CString& strCode, CString& strDesc, int& nByte);
//		BOOL FindButtonKey(CString sBtnName, CString& strName, CString& strCode, CString& strDesc, int& nByte);
		BOOL SendButtonKey(CString strName);

		void GrabStart();
		void GrabStop();
		void SetAvSwitchValueCmb(int nCmdType);
		void ShowSystemMsg(); 
		void CloseSystemMsg(); 
		afx_msg LRESULT UpdateGrabStatus(WPARAM wParam, LPARAM lParam);
//		void ChkLVDSAutoGrab(); 
		void ShowAvSwitchBoxCtrlGroup(BOOL bShow);
		void ShowUsbDioGroup(BOOL bShow);
		void SetHDMIGenGroup(BOOL bEnable);
		void ChkVideoAutoGrab(); 
		void SetPatternGenGroup(BOOL bEnable);
		void InitRemoteButtonGroup();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRABPAGE_H__69649592_2A63_46CD_9B76_72136AD11D13__INCLUDED_)
