//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_PARMADJPAGE_H__8A319693_09F0_4D72_B6FA_3EABD1ECC845__INCLUDED_)
#define AFX_PARMADJPAGE_H__8A319693_09F0_4D72_B6FA_3EABD1ECC845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParmAdjPage.h : header file
//

//////////////////////////////////////////////////////////////////////////
#include "BaseImage.h"
#include "ImageProc.h"
#include "ProgressCtrlX.h"

/////////////////////////////////////////////////////////////////////////////
// CParmAdjPage dialog

class CSignalTestResultDlg;
class CImageTotalDlg;

class CParmAdjPage : public CDialog
{
// Construction
public:
	CParmAdjPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParmAdjPage)
	enum { IDD = IDD_PARM_ADJ_PAGE };
	CEdit	m_ctrlNoSkippedSignalLineLowBits;
	CComboBox	m_ctrlMaskWidthCmb;
	CEdit	m_ctrlMaskImgPath;
	CButton	m_ctrlUseMaskImageChk;
	CEdit	m_ctrlRoiWidthEdit;
	CEdit	m_ctrlRoiHeightEdit;
	CEdit	m_ctrlRoiYEdit;
	CEdit	m_ctrlRoiXEdit;
	CListBox	m_ctrlTestResultList;
	CButton	m_ctrlColMagHighbitsChk;
	CButton	m_ctrlColMagLevelChk;
	CEdit	m_ctrlColMagHighbitsEdit;
	CEdit	m_ctrlColMagLevelEdit;
	CEdit	m_ctrlPosShiftXEdit;
	CEdit	m_ctrlPosShiftYEdit;
	CEdit	m_ctrlPosMagYEdit;
	CEdit	m_ctrlPosMagXEdit;
	CStatic	m_ctrlRefBValStatic;
	CStatic	m_ctrlRefGValStatic;
	CStatic	m_ctrlRefRValStatic;
	CStatic	m_ctrlGrabBValStatic;
	CStatic	m_ctrlGrabGValStatic;
	CStatic	m_ctrlGrabRValStatic;
	CEdit	m_ctrlYPosEdit;
	CEdit	m_ctrlXPosEdit;
	CEdit	m_ctrlThresholdEdit;
	CEdit	m_ctrlRefImgPath;
	CEdit	m_ctrlGrabImgEdit;
	CEdit	m_ctrlCropYEdit;
	CEdit	m_ctrlCropXEdit;
	CEdit	m_ctrlMaskYEdit;
	CEdit	m_ctrlMaskXEdit;
	int		m_nBinaryType;
	//CVSFlexGrid	m_ctrlResultGrid;
	int		m_nRegionType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParmAdjPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParmAdjPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnParmFilter();
	afx_msg void OnBtnParmBinary();
	afx_msg void OnBtnParmPatternMatch();
	afx_msg void OnBtnParmColorTest();
	afx_msg void OnBtnParmOpenGrabImg();
	afx_msg void OnBtnParmOpenRefImg();
	afx_msg void OnBtnParmReload();
	afx_msg void OnBtnParmGetRgbValue();
	afx_msg void OnChkParmColorMarginLevel();
	afx_msg void OnChkParmColorMarginHighBits();
	afx_msg void OnBtnParmMakeMaskImage();
	afx_msg void OnBtnParmSaveAs();
	afx_msg void OnBtnParmOpenMaskImg();
	afx_msg void OnChkParmUseMask();
	afx_msg void OnRadioNormal();
	afx_msg void OnRadioReverse();
	afx_msg void OnBtnParmSignalLineTest();
	afx_msg void OnBtnParmLabel();
	afx_msg void OnBtnParmRepaint();
	afx_msg void OnBtnParmZoomOut();
	afx_msg void OnBtnParmNoiseTest();
	afx_msg void OnRadioOsdRegion();
	afx_msg void OnRadioRoi();
	afx_msg void OnBtnParmMovingPicTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
public:
//Variable

//	CWnd*		m_MWnd;			//Image Total Dlg Wnd
	CImageTotalDlg* m_MWnd;
	
	int			m_nTabType;		//2: Review Page
	int*		m_nImgType;		//0: Grab Image, 1: Ref Image, 2: ProcImage

	//+ 2007.4.28 Mod BY USY
	BOOL		m_bFirstOpen[4];
	BOOL		m_bRefresh[4];
	BOOL		m_bZoomAllReset;
	BOOL		m_bUseMaskImage;

	//+ 2007.4.28 Mod BY USY
	CBaseImage	m_Image[4];		//Grab Image , Ref Image, Proc Image, Mask Image

	CImageProc	m_ImageProc;	

	CString		m_strGrabPath;
	CString		m_strRefPath;
	CString		m_strMaskPath;

	CString		m_strGrabFileName;
	CString		m_strRefFileName;
	CString		m_strMaskFileName;

	CString		m_strGrabFolder;
	CString		m_strRefFolder;
	CString		m_strMaskFolder;

	CProgressCtrlX* m_ctrlProgress;	//Image Total Dlg¿« Progress Ctrl
	CComboBox*	    m_ctrlZoomCmb;

	CSignalTestResultDlg* m_pSignalTestResultDlg;

//Function

	void	InitParmValue();
	void	InitResultGrid();
	void	AddStringToResultGrid(CString s);
	void	AddStringToResultGrid(int nRow, int nColumn,CString s);

	BOOL	GetRgbValue(int nImgType, CBaseImage& Image, int nXPos, int nYPos, int& nRvalue,int& nGvalue,int& nBvalue);
	BOOL	OpenImgFile(CString sImgPath, int nImgType);
	CListCtrl m_ctrlResultList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMADJPAGE_H__8A319693_09F0_4D72_B6FA_3EABD1ECC845__INCLUDED_)
