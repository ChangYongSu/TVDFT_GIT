#if !defined(AFX_SEQROIDLG_H__FB3C2AFF_783A_44FD_82A0_E75BB4EA6574__INCLUDED_)
#define AFX_SEQROIDLG_H__FB3C2AFF_783A_44FD_82A0_E75BB4EA6574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeqRoiDlg.h : header file
//

//////////////////////////////////////////////////////////////////////////
//+ 2007.5.8 Add BY USY
#include "ImageProc.h"
#include "BaseImage.h"
//////////////////////////////////////////////////////////////////////////

#define DISPLAY_TYPE_FULL	0
#define DISPLAY_TYPE_FIT	1
#define MIN_LINE_SCROLL		5	// 5 Pixels

/////////////////////////////////////////////////////////////////////////////
// CSeqRoiDlg dialog

class CSeqRoiDlg : public CDialog
{
// Construction
public:
	CSeqRoiDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSeqRoiDlg)
	enum { IDD = IDD_SEQ_ROI_DLG };
	CStatic	m_ctrlYpos;
	CStatic	m_ctrlXpos;
	CStatic	m_ctrlRoiY2;
	CStatic	m_ctrlRoiX2;
	CScrollBar		m_ctrlImgVScroll;
	CScrollBar		m_ctrlImgHScroll;
	CProgressCtrl	m_ctrlProgress;
	CStatic			m_ctrlRoiY1;
	CStatic			m_ctrlRoiX1;
	CStatic			m_ctrlRoiWidthStc;
	CStatic			m_ctrlRoiHeightStc;
	CListBox		m_ctrlRefFileList;
	int				m_nDisplayType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeqRoiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeqRoiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnPaint();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	afx_msg void OnSelchangeListRefFile();

	afx_msg void OnRadioDisplayTypeFull();
	afx_msg void OnRadioDisplayTypeFit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
//+ 2007.5.8 Add BY USY
public:
	//Variable
	CStringArray	m_aRefFile;
	CString			m_strModelFolder;
	CString			m_strRefPath;
	CString			m_strRefName;
	
	UINT			m_nRoiX1;
	UINT			m_nRoiY1;
	UINT			m_nRoiX2;
	UINT			m_nRoiY2;

	UINT			m_nPrevRoiX1;
	UINT			m_nPrevRoiY1;
	UINT			m_nPrevRoiX2;
	UINT			m_nPrevRoiY2;
	
	UINT			m_nRoiWidth;
	UINT			m_nRoiHeight;
	
	CBaseImage		m_Image;
	
	CWnd*			m_pImgViewWnd;	//Image View Wnd
	CRect			m_ImgViewRect;	//Image View Rect
	//+add kwmoon 080628
//	CRect			m_ImgViewClientRect;

	CRect			m_CropRect;
	BOOL			m_bFlagMouse;
	BOOL			m_bFlagCrop;

	
	
	int				m_nImgViewHeight;
	int				m_nImgViewWidth;

	//+add kwmoon 080628
	int				m_nImageWidth;
	int				m_nImageHeight;

	int				m_nScrHMax;	//H Scroll Max Pos
	int				m_nScrVMax;	//V Scroll Max Pos
	int				m_nScrHCur;	//H Scroll Current Pos
	int				m_nScrVCur;	//V Scroll Current Pos

	int				m_nStartX;	//Image Start X Pos
	int				m_nStartY;	//Image Start Y Pos

	//+add kwmoon 080627
	int				m_nStepX;	
	int				m_nStepY;
	int				m_nPageX;	
	int				m_nPageY;

	float			m_fXratio;
	float			m_fYratio; 

	//Function
	void GetFileList(CString sFolder);
	void DrawImage(int nDlgItemNo,CBaseImage& ImageData, int nStartXPos, int nStartYPos);
	void OpenImgFile(CString sFilePath);

	void SetVScrollBar();
	void SetHScrollBar();
	void InitScrollBar();
//////////////////////////////////////////////////////////////////////////

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEQROIDLG_H__FB3C2AFF_783A_44FD_82A0_E75BB4EA6574__INCLUDED_)
