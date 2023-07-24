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
	double	m_fAllowedErrorPixelPercentage;
	UINT	m_nNoFrameForRefImageCreation;
	UINT	m_nNoRetry;
	UINT	m_nFreqMargin;
	UINT	m_nDelayPercentage;
	BOOL	m_bSaveDistributionTable;
	BOOL	m_bSaveIntermediateImage;
	BOOL	m_bSaveCaptionTestImg;
	BOOL	m_bSaveMovingPicTestImg;
	double	m_fAllowedErrorPixelPercentageForOsdTest;//m_nNoAllowedErrorPixelForOsdTest;
	UINT	m_nNonBlackPixel;
	UINT	m_nPixelInMiddleRange;
	int		m_bSaveGrabImg;
	BOOL	m_bSaveProcessingTimeData;
	double	m_fAllowedBand1ErrorPixelPercentage;
	double	m_fAllowedBand2ErrorPixelPercentage;
	double	m_fAllowedBand3ErrorPixelPercentage;
	double	m_fAllowedBand4ErrorPixelPercentage;
	double	m_fAllowedBand5ErrorPixelPercentage;
	BOOL	m_bSaveReviewData;
	BOOL	m_bSaveDetailResultData;
	int		m_nLevelMargin;
	int		m_nNoUserStop;
	int		m_nNoTestedBoard;
	int		m_nNoFailedBoard;
	UINT	m_nNoUsedColors;
	BOOL	m_bNgCountCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptEtcPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptEtcPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnOpenViewer();
	afx_msg void OnBtnEtcOptApply();
	afx_msg void OnBtnEtcOptClearTotalCount();
	afx_msg void OnBtnSave();
	afx_msg void OnChkSaveCaptionTestImage();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
public:
/////////////////////////////////////////////////////////////////////////////

	//CString m_nAudioForce_HP_L;
	int m_nAudioForce_HP_R;
	int m_nAudioForce_HP_L;
	int m_nAudioForce_Analog_L;
	int m_nAudioForce_Analog_R;
	int m_nAudioForce_HDMI_L;
	int m_nAudioForce_HDMI_R;
	int m_nAudioForce_RGB_L;	
	int m_nAudioForce_RGB_R;
	int m_nAudioForce_SCART_L;
	int m_nAudioForce_SCART_R;
	CComboBox m_nARC_Force_Port_Number;
	CComboBox m_nARC_Force_OnOffChannel;
	BOOL m_bCheckGrabACPower;
	BOOL m_bCheckIFPackReset;
	BOOL m_bCheckGenderReset;
	BOOL m_bRobotSkipFFC;
	BOOL m_bCheckUsePcbSensor;
	BOOL m_bCheckRotateReset;
	BOOL m_bCheckImageFullReset;
	BOOL m_bCheckOledReset;
	afx_msg void OnBnClickedCheckSkipFfc();
	BOOL m_bCheckBatteryCheck;
	afx_msg void OnBnClickedChkEpiPackReset2();
	BOOL m_bEpiPAckReset;
	afx_msg void OnBnClickedChkGrabBaseReset2();
	BOOL m_bGrabBaseReset;
	BOOL m_bCheckGrabberOnOffCheck;
	BOOL m_bFHDUHD_OLED_Pack_Reset;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTETCPAGE_H__D475C9EB_927D_4B9E_9906_D01ACFBED76E__INCLUDED_)
