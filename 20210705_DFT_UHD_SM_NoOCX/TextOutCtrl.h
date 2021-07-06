// OScopeCtrl.h : header file
//

#ifndef __OScopeCtrl_H__
#define __OScopeCtrl_H__

/////////////////////////////////////////////////////////////////////////////
// CTextOutCtrl window

class CTextOutCtrl : public CWnd
{
// Construction
public:
  CTextOutCtrl();

// Attributes
public:
  void SetText(CString string);
  void SetTextColor(COLORREF color);
  void SetBackgroundColor(COLORREF color);
  void TextOuts(CString s1, CString s2, CString s3);
  void DrawPoint();
  void Reset();

  // Operations
public:

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CTextOutCtrl)
  public:
  virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);
  //}}AFX_VIRTUAL

// Implementation
public:

  CString m_strText;

  COLORREF m_crBackColor;        // background color
  COLORREF m_crTextColor;        // grid color
  int	   fontSize;
  UINT     textAlign;
  virtual ~CTextOutCtrl();

  // Generated message map functions
protected:
  //{{AFX_MSG(CTextOutCtrl)
  afx_msg void OnPaint();
  afx_msg void OnSize(UINT nType, int cx, int cy); 
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  int m_nHalfShiftPixels;
  int m_nPlotShiftPixels;
  int m_nClientHeight;
  int m_nClientWidth;
  int m_nPlotHeight;
  int m_nPlotWidth;

  double m_dLowerLimit;        // lower bounds
  double m_dUpperLimit;        // upper bounds
  double m_dRange;
  double m_dVerticalFactor;
  
  CRect  m_rectClient;
  CRect  m_rectPlot;
  CPen   m_penPlot;
  CBrush m_brushBack;

  CDC     m_dcText;
  CBitmap m_bitmapText;
  CBitmap *m_pbitmapOldText;
};

/////////////////////////////////////////////////////////////////////////////
#endif
