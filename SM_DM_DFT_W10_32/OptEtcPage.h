//{{AFX_INCLUDES()
#include "vsflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_OPTETCPAGE_H__D475C9EB_927D_4B9E_9906_D01ACFBED76E__INCLUDED_)
#define AFX_OPTETCPAGE_H__D475C9EB_927D_4B9E_9906_D01ACFBED76E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptEtcPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
#include "Global.h"
#include "Step.h"
#include "Parser.h"
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COptEtcPage dialog

class COptEtcPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COptEtcPage)

// Construction
public:
	COptEtcPage();
	~COptEtcPage();
// Dialog Data
	//{{AFX_DATA(COptEtcPage)
	enum { IDD = IDD_OPT_ETC_PAGE };
	CSpinButtonCtrl	m_ctrlAudioLevelSpecMargin;
	CSpinButtonCtrl	m_ctrlNonBlackPixel;
	CSpinButtonCtrl	m_ctrlPixelInMiddleRange;
	CSpinButtonCtrl	m_ctrlNoAllowedErrorPixelForOsdTest;
	CSpinButtonCtrl	m_ctrlDelayPercentage;
	CSpinButtonCtrl	m_ctrlAudioFreqSpecMargin;
	CSpinButtonCtrl	m_ctrlNoRetry;
	CSpinButtonCtrl	m_ctrlNoFrame;
	CSpinButtonCtrl	m_ctrlNoAllowedErrorPixel;
	CSpinButtonCtrl	m_ctrlNoRepeatExecution;
	BOOL	m_bSaveProcImg;
	CString	m_strViewerPath;
	BOOL	m_bResultSaveCsv;
	BOOL	m_bResultSaveHtml;
	BOOL	m_bResultSaveText;
	int		m_nDisplayType;
	UINT	m_nNoRepeatExecution;
	float	m_fAllowedErrorPixelPercentage;
	UINT	m_nNoFrameForRefImageCreation;
//	CVSFlexGrid	m_ctrlSpecAdjGrid;
	UINT	m_nNoRetry;
	UINT	m_nFreqMargin;
	UINT	m_nDelayPercentage;
	BOOL	m_bSaveDistributionTable;
	BOOL	m_bSaveIntermediateImage;
	BOOL	m_bSaveCaptionTestImg;
	BOOL	m_bSaveMovingPicTestImg;
	float	m_fAllowedErrorPixelPercentageForOsdTest;//m_nNoAllowedErrorPixelForOsdTest;
	UINT	m_nNonBlackPixel;
	UINT	m_nPixelInMiddleRange;
	int		m_bSaveGrabImg;
	BOOL	m_bSaveProcessingTimeData;
	float	m_fAllowedBand1ErrorPixelPercentage;
	float	m_fAllowedBand2ErrorPixelPercentage;
	float	m_fAllowedBand3ErrorPixelPercentage;
	float	m_fAllowedBand4ErrorPixelPercentage;
	float	m_fAllowedBand5ErrorPixelPercentage;
	BOOL	m_bSaveReviewData;
	BOOL	m_bSaveDetailResultData;
	int		m_nLevelMargin;
	int		m_nNoUserStop;
	int		m_nNoTestedBoard;
	int		m_nNoFailedBoard;
	UINT	m_nNoUsedColors;
	BOOL	m_bRelayOnOff;
	BOOL	m_bFixtureIdCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptEtcPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	int m_EditComboMode;
	int m_SelectItem ;
	int m_SelectShell;
	void SetComboBoxList(int nItem, int nShell);
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptEtcPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnOpenViewer();
	afx_msg void OnBtnEtcOptApply();
	afx_msg void OnBtnEtcOptClearTotalCount();
	afx_msg void OnDblClickSpecAdjGrid();
	afx_msg void OnBtnSave();
	afx_msg void OnChkSaveCaptionTestImage();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
public:
	//Variable
	//+change kwmoon 080724
	//change PSh 080605
//	int	m_nSrcMargin[22];
	int	m_nSrcMargin[NO_SOURCE_TYPE];

//Function
	//+add kwmoon 070930
	void InitSpecAdjGrid();
	void UpdateSpecAdjGrid();
/////////////////////////////////////////////////////////////////////////////

	CListCtrl m_ctrlSpecAdjList;
	CComboBox m_cComboMargin;
	afx_msg void OnNMDblclkListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnEndScrollListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMHoverListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginScrollListSpecAdjList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboMargin2();
	CComboBox m_cComboSoundLeft;
	afx_msg void OnCbnSelchangeComboSoundL();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTETCPAGE_H__D475C9EB_927D_4B9E_9906_D01ACFBED76E__INCLUDED_)
