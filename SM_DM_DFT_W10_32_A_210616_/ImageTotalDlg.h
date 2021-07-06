//{{AFX_INCLUDES()
#include "mmhitimerx.h"
#include "mmconnectorx.h"
#include "mmwaveinx.h"
#include "mmspectrumx.h"
#include "mmpeakdetectx.h"
#include "mmledpanelx.h"
//}}AFX_INCLUDES
#if !defined(AFX_IMAGETOTALDLG_H__677304C9_4BF7_4902_887C_E473D30666BF__INCLUDED_)
#define AFX_IMAGETOTALDLG_H__677304C9_4BF7_4902_887C_E473D30666BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageTotalDlg.h : header file
//
//#define AUDIO_DEBUG__
//////////////////////////////////////////////////////////////////////////
#include "ImageProc.h"
#include "BaseImage.h"

#include "GrabPage.h"
#include "ReviewPage.h"
#include "ParmAdjPage.h"
#include "Defines.h"

//+add kwmoon 080724
#include "SoundCard.h"
//#include "Display.h"
//#include "Digitizer.h"
//#include "Image.h"
//#include "ImageProcessing.h"

#include "OpenGLControlSub.h"

//////////////////////////////////////////////////////////////////////////

#define AUDIO_MEASURE_START	0
#define AUDIO_MEASURE_STOP	1


/////////////////////////////////////////////////////////////////////////////
// CImageTotalDlg dialog

class CImageTotalDlg : public CDialog
//-
{
// Construction
public:
	CImageTotalDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageTotalDlg)
	enum { IDD = IDD_IMAGE_TOTAL_DLG };
	CComboBox	m_ctrlSelImageCmb;
	CButton	m_ctrlMaskImgChk;
	CButton	m_ctrlAveragePageChk;
	CStatic	m_ctrlYPosStatic;
	CStatic	m_ctrlXPosStatic;
	CStatic	m_ctrlBValStatic;
	CStatic	m_ctrlGValStatic;
	CStatic	m_ctrlRValStatic;
	CProgressCtrlX	m_ctrlProgress;
	CComboBox	m_ctrlZoomCmb;
	CButton	m_ctrlProcImgChk;
	CButton	m_ctrlRefImgChk;
	CButton	m_ctrlGrabImgChk;
	CScrollBar	m_ctrlImgVScroll;
	CScrollBar	m_ctrlImgHScroll;
	CButton	m_ctrlParmPageChk;
	CButton	m_ctrlReviewPageChk;
	CButton	m_ctrlCropPageChk;
	CButton	m_ctrlGrabPageChk;
	int		m_nRadioAudioMeasureStart;
	int		m_nRadioAudioOutL;
	int		m_nRadioAudioOutR;
	//}}AFX_DATA
 //   CDisplay        m_ActiveMILDisplay;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageTotalDlg)
	public:
//	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
#ifndef	AUDIO_DEBUG__
	COpenGLControlSub *mp_oglWindowLeft;
#endif
//	COpenGLControlSub m_oglWindowRight;

	CFont	m_AudioFont;
	CColorStaticST  m_cLabelSoundLeftFREQ;//IDC_TEXT_FREQ_LEFT // ctrlPLC_COM_Status.SubclassDlgItem(IDC_PLC_COM_STATUS,this);
	CColorStaticST  m_cLabelSoundLeftVolt;//IDC_TEXT_VOLT_LEFT
	CColorStaticST  m_cLabelSoundRightFREQ;//IDC_TEXT_FREQ_RIGHT
	CColorStaticST  m_cLabelSoundRightVolt;//IDC_TEXT_VOLT_RIGHT

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImageTotalDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnChkGrabPage();
	afx_msg void OnChkCropPage();
	afx_msg void OnChkReviewPage();
	afx_msg void OnChkParmPage();
	afx_msg void OnChkGrabImg();
	afx_msg void OnChkRefImg();
	afx_msg void OnChkProcImg();
	afx_msg void OnSelchangeCmbZoom();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnChkMaskImg();
	afx_msg void OnChkAveragePage();
	afx_msg void OnSelchangeCmbSelImage();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRadioMeasure();
	afx_msg void OnRadioWaveoutL();
	afx_msg void OnRadioWaveoutR();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

public:
// Variable

	CGrabPage		m_GrabPage;			// Grab Page
	CReviewPage		m_ReviewPage;		// Review Page
	CParmAdjPage	m_ParmPage;			// Parm Adj Page

	int				m_nTabType;			// 0 : Grab Page, 1 : Crop Page, 2 : Review Page, 3 : Parm Adj Page, 4 : Average Page 
	int				m_nImgType;			// 0 : Grab Image, 1 : Ref Image, 2 : Processed Image, 3: Mask Image
	int				m_nOldImgType[5];

	CBaseImage*		m_Image[4];			// Grab/Ref/Proc/Mask
	BITMAPINFO*		m_ZImgInfo;			// Zoom Image Info

	CWnd*			m_pImgViewWnd;		// Image View Wnd
	CWnd*			m_pPageWnd;			// Page Wnd
	CWnd*			m_pStaticWnd;		//Static_Group_1 Wnd
	CRect			m_ImgViewRect;		// Image View Rect
	CRect			m_PageRect;			// Page Rect;
	CRect			m_StaticRect;		//Static_Group_1 Rect
	
	CRect			m_PageScreenRect;	// Page Rect;

	int				m_nImgViewHeight;
	int				m_nImgViewWidth;
	int				m_nPageHeight;
	int				m_nPageWidth;
	int				m_nPage2Height;
	int				m_nPage2Width;
	int				m_nScrHMax[5][4];	// H Scroll Max Pos
	int				m_nScrVMax[5][4];	// V Scroll Max Pos
	int				m_nScrHCur[5][4];	// H Scroll Current Pos
	int				m_nScrVCur[5][4];	// V Scroll Current Pos

	int				m_nStartX[5][4];	// Image Start X Pos
	int				m_nStartY[5][4];	// Image Start Y Pos
	float			m_fZFactor[5][4];	// Zoom Factor 


	int				m_nXPos;
	int				m_nYPos;
	int				m_nRVal;
	int				m_nGVal;
	int				m_nBVal;

//	CSoundCard		m_ctrlSoundCard;


// Function

	void			SetChkBtn(int nTabType, int nImgType);
	void			SetImgChkBtn(int nTabType, int nImgType);
	void			SetPage(int nTabType, int nImgType);
	void			SetPageItem(int nTabType, int nImgType);
	void			SetImgPanel(BOOL bShow);
	void			InitImgPanel();
	void			SetZoomCmb(int nTabType, int nImgType);

	void			DrawImage(int nTabType, int nImageType,int nDlgItemNo,CBaseImage& ImageData, int nStartXPos, int nStartYPos, float fZoomFactor);
	void			ZoomIn(int nTabType, int nImageType,CBaseImage& ImageData);

	void			InitScrollBar(int nTabType, int nImgType);
	void			SetVScrollBar(int nTabType, int nImgType);
	void			SetHScrollBar(int nTabType, int nImgType);

	//+recover kwmoon 080724
	void			InitAudioMeasurement();

	// User Defined Message Handler
	afx_msg LRESULT UpdateImageViewDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT CalcAndMoveSubWindow(WPARAM wParam, LPARAM lParam);
	//void			CalcMainWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT 			ChangeActiveViewTab(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT 			UpdateAllViewTab(WPARAM wParam, LPARAM lParam);
	void			VideoDispSelectWindow();
	void			VideoDispDeselectWindow();

	//+add 20110328
	void AudioMeasureStart();
	void AudioMeasureStop();
//    IImagePtr		&GetActiveMILImage();

private:
//    IImagePtr   m_ActiveMILImage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGETOTALDLG_H__677304C9_4BF7_4902_887C_E473D30666BF__INCLUDED_)
