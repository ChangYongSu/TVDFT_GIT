// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__A91C3948_305C_11D3_90A6_006008CED7F4__INCLUDED_)
#define AFX_MAINFRM_H__A91C3948_305C_11D3_90A6_006008CED7F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TOOLBAR_DRAW_BUTTON_WIDTH 32
#include "TrueColorToolBarCtrl_src\\TrueColorToolBar.h"
//-
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	//+2007.02.02 Add BY USY
	void SetTitle(CString sModelFolder);
	void SetStatusBarText(CString sText);
	//-

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	//+del kwmoon 070109 : TrueColor ToolBar
	CToolBar    m_wndToolBar;

	//+add kwmoon 070109 : TrueColor ToolBar
	CTrueColorToolBar    m_newToolBar;
	//-

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRunContinuous();
	afx_msg void OnRunErrorabort();
	afx_msg void OnUpdateRunErrorabort(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunAuto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunContinuous(CCmdUI* pCmdUI);
	afx_msg void OnRunErrorstep();
	afx_msg void OnUpdateRunErrorstep(CCmdUI* pCmdUI);
	afx_msg void OnRunStep();
	afx_msg void OnUpdateRunStep(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAppExit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCompile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCurrent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModelOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunNext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunPrev(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunJump(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunAbort(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnSpeakerControl();
	afx_msg void OnUpdateAudioMeasStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAudioMeasStop(CCmdUI* pCmdUI);
	afx_msg void OnPrintAllresult();
	afx_msg void OnUpdatePrintAllresult(CCmdUI* pCmdUI);
	afx_msg void OnPrintError();
	afx_msg void OnUpdatePrintError(CCmdUI* pCmdUI);
	afx_msg void OnPrintNone();
	afx_msg void OnUpdatePrintNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunMakeRef(CCmdUI* pCmdUI);
	afx_msg void OnI2cManualCheckDialog();
	afx_msg void OnRs232cManualCheckDialog();
	afx_msg void OnUpdateSeqEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunAdjSpec(CCmdUI* pCmdUI);
	afx_msg void OnUpdateImageDialog(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOption(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGrabStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGrabStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEnter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRefImage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRefAudio(CCmdUI* pCmdUI);
	afx_msg void OnUpdateScannerDisable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateScannerEnable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateJigUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateJigupAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateJigupGood(CCmdUI* pCmdUI);
	afx_msg void OnUpdateJigupNot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartAuto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartManual(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__A91C3948_305C_11D3_90A6_006008CED7F4__INCLUDED_)
