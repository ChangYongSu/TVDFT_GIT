//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_REVIEWPAGE_H__05847186_003A_4205_98BC_5DBE6F4846E7__INCLUDED_)
#define AFX_REVIEWPAGE_H__05847186_003A_4205_98BC_5DBE6F4846E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReviewPage.h : header file
//

//////////////////////////////////////////////////////////////////////////
//+2007.3.16 Add BY USY
#include "BaseImage.h"
#include "ImageProc.h"
#include "parser.h"
#include "step.h"
#include "IniAccess.h"
#include "ProgressCtrlX.h"
//#include "NoDifferentPointDlg.h"
//-
//////////////////////////////////////////////////////////////////////////

class CNoDifferentPointDlg;
class CSignalTestResultDlg;
class CImageTotalDlg;

/////////////////////////////////////////////////////////////////////////////
// CReviewPage dialog

class CReviewPage : public CDialog
{
// Construction
public:
	CRect m_dlgRect;
	CReviewPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReviewPage)
	enum { IDD = IDD_REVIEW_PAGE };
	CStatic	m_ctrlRefBValStatic;
	CStatic	m_ctrlRefRValStatic;
	CStatic	m_ctrlRefGValStatic;
	CStatic	m_ctrlGrabBValStatic;
	CStatic	m_ctrlGrabGValStatic;
	CStatic	m_ctrlGrabRValStatic;
	CEdit	m_ctrlYPosEdit;
	CEdit	m_ctrlXPosEdit;
	CEdit	m_ctrlRstPathEdit;
	CEdit	m_ctrlRefImgEdit;
	CEdit	m_ctrlProcImgEdit;
	CEdit	m_ctrlGrabImgEdit;
	//CVSFlexGrid	m_ctrlReviewGrid;
	//}}AFX_DATA

public:
	//+add kwmoon 070807
	CNoDifferentPointDlg* m_pNoDifferentPointDlg;
	CSignalTestResultDlg* m_pSignalTestResultDlg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReviewPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReviewPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnReviewRstFileOpen();
	afx_msg void OnBtnReviewGetRgbVal();
	afx_msg void OnDestroy();
//	afx_msg void OnDblClickReviewGrid();
	afx_msg void OnClickReviewGrid();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
public:
//Variable
//+2007.3.16 Add BY USY

//	CWnd*		m_MWnd;			//Image Total Dlg Wnd
	CImageTotalDlg* m_MWnd;
	
	int			m_nTabType;		//2: Review Page
	int*		m_nImgType;		//0: Grab Image, 1: Ref Image, 2: Proc Image, 3: Mask Image

	//+ 2007.4.28 Mod BY USY
	BOOL		m_bFirstOpen[4];
	BOOL		m_bRefresh[4];
//	BOOL		m_bFirstOpen[3];
//	BOOL		m_bRefresh[3];
	//-
	BOOL		m_bZoomAllReset;

	//+ 2007.4.28 Mod BY USY
	CBaseImage  m_Image[4];		//Grab Image
//	CBaseImage  m_Image[3];		//Grab Image
	//-

	CProgressCtrlX* m_ctrlProgress;	//Image Total Dlg¿« Progress Ctrl
//	CProgressCtrl* m_ctrlProgress;	//Image Total Dlg¿« Progress Ctrl
	
//+2007.3.20 Add BY USY
	CString		m_strRstPath;
	CString		m_strGrabPath;
	CString		m_strRefPath;
	CString		m_strProcPath;
	//+ 2007.4.28 Add BY USY
	CString		m_strMaskPath;
	//-

	CTypedPtrList<CPtrList, CStep*>	m_StepList;
	CIniAccess	m_Ini;
//	CStep*							pStep;
		
//Function
//////////////////////////////////////////////////////////////////////////
//+2007.3.20 Add BY USY
	BOOL	GetRgbValue(int nImgType, CBaseImage& Image, int nXPos, int nYPos, int& nRvalue,int& nGvalue,int& nBvalue);
	void	OpenRstFile(CString sRstPath);
	void	OpenImgFile(CString sImgPath, int nImgType);
	//+ 2007.7.11 Add BY USY
	void	InitGrid();
	void	UpdateGrid();
//////////////////////////////////////////////////////////////////////////

	
	CListCtrl m_ctrlReviewList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REVIEWPAGE_H__05847186_003A_4205_98BC_5DBE6F4846E7__INCLUDED_)
