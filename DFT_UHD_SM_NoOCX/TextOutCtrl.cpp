// OScopeCtrl.cpp : implementation file//

#include "stdafx.h"
#include "math.h"

#include "TextOutCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__ ;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextOutCtrl
CTextOutCtrl::CTextOutCtrl()
{
  m_crBackColor  = RGB(255, 255, 255) ;  // see also SetBackgroundColor
  m_crTextColor  = RGB(0, 0, 255) ;  // see also SetGridColor

//  m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor) ;
  m_brushBack.CreateSolidBrush(m_crBackColor) ;

  // public member variables, can be set directly 
  m_strText.Format("Samples") ;  // can also be set with SetXUnits
  fontSize = 18;
  textAlign =TA_LEFT|TA_TOP;
	
}  // CTextOutCtrl

/////////////////////////////////////////////////////////////////////////////
CTextOutCtrl::~CTextOutCtrl()
{
  // just to be picky restore the bitmaps for the two memory dc's
  // (these dc's are being destroyed so there shouldn't be any leaks)
  if (m_pbitmapOldText != NULL)
    m_dcText.SelectObject(m_pbitmapOldText) ;  
} // ~CTextOutCtrl


BEGIN_MESSAGE_MAP(CTextOutCtrl, CWnd)
  //{{AFX_MSG_MAP(CTextOutCtrl)
  ON_WM_PAINT()
  ON_WM_SIZE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTextOutCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
BOOL CTextOutCtrl::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, UINT nID) 
{
  BOOL result ;
  static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;

  result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
                          className, NULL, dwStyle, 
                          rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
                          pParentWnd->GetSafeHwnd(), (HMENU)nID) ;
  

//  if (result != 0)
//    InvalidateCtrl() ;

  return result ;

} // Create

/////////////////////////////////////////////////////////////////////////////
void CTextOutCtrl::SetText(CString string)
{
  m_strText = string ;

  // clear out the existing garbage, re-start with a clean plot
//  InvalidateCtrl() ;

}  

/////////////////////////////////////////////////////////////////////////////
void CTextOutCtrl::SetTextColor(COLORREF color)
{
  m_crTextColor = color ;

  // clear out the existing garbage, re-start with a clean plot
//  InvalidateCtrl() ;

}  // SetGridColor


/////////////////////////////////////////////////////////////////////////////
void CTextOutCtrl::SetBackgroundColor(COLORREF color)
{
  m_crBackColor = color ;

  m_brushBack.DeleteObject() ;
  m_brushBack.CreateSolidBrush(m_crBackColor) ;

  // clear out the existing garbage, re-start with a clean plot
//  InvalidateCtrl() ;

}  // SetBackgroundColor

/////////////////////////////////////////////////////////////////////////////
void CTextOutCtrl::TextOuts(CString s1, CString s2, CString s3)
{
  CPen solidPen(PS_SOLID, 0, m_crTextColor) ;
  CFont TextFont, *oldFont ;
  CString strTemp ;

  // in case we haven't established the memory dc's
  CClientDC dc(this) ;  

  // if we don't have one yet, set up a memory dc for the grid
  if (m_dcText.GetSafeHdc() == NULL)
  {
    m_dcText.CreateCompatibleDC(&dc) ;
    m_bitmapText.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
    m_pbitmapOldText = m_dcText.SelectObject(&m_bitmapText) ;
  }
  
  m_dcText.SetBkColor (m_crBackColor) ;

  // fill the grid background
  m_dcText.FillRect(m_rectClient, &m_brushBack) ;


  // draw the plot rectangle
//  oldPen = m_dcText.SelectObject (&solidPen) ; 
//  m_dcText.MoveTo (m_rectPlot.left, m_rectPlot.top) ;
//  m_dcText.LineTo (m_rectPlot.right+1, m_rectPlot.top) ;
//  m_dcText.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1) ;
//  m_dcText.LineTo (m_rectPlot.left, m_rectPlot.bottom+1) ;
//  m_dcText.LineTo (m_rectPlot.left, m_rectPlot.top) ;
//  m_dcText.SelectObject (oldPen) ; 

  TextFont.CreateFont (fontSize, 0, 0, 0, FW_BOLD,
                       FALSE, FALSE, 0, ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS, 
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY, 
                       DEFAULT_PITCH|FF_SWISS, "Arial") ;
  
  // grab the horizontal font
  oldFont = m_dcText.SelectObject(&TextFont) ;
  
  m_dcText.SetTextColor (m_crTextColor) ;
  m_dcText.SetTextAlign (textAlign) ;
  if(textAlign == 0)
  {
	m_dcText.TextOut (m_rectPlot.left, m_rectPlot.top, s1) ;
	m_dcText.TextOut (m_rectPlot.left, m_rectPlot.top+fontSize+5, s2) ;
	m_dcText.TextOut (m_rectPlot.left, m_rectPlot.top+(fontSize+5)*2, s3) ;  
  } 
  else
  {	
	m_dcText.TextOut ((m_rectPlot.right-m_rectPlot.left)/2 , m_rectPlot.top, s1) ;
	m_dcText.TextOut ((m_rectPlot.right-m_rectPlot.left)/2 , m_rectPlot.top+fontSize+5, s2) ;
	m_dcText.TextOut ((m_rectPlot.right-m_rectPlot.left)/2 , m_rectPlot.top+(fontSize+5)*2, s3) ;
  }	  
  // restore the font
  m_dcText.SelectObject(oldFont) ;

  InvalidateRect(m_rectClient) ;

} // InvalidateCtrl

////////////////////////////////////////////////////////////////////////////
void CTextOutCtrl::OnPaint() 
{
  CPaintDC dc(this) ;  // device context for painting
  CDC memDC ;
  CBitmap memBitmap ;
  CBitmap* oldBitmap ; // bitmap originally found in CMemDC

  // no real plotting work is performed here, 
  // just putting the existing bitmaps on the client

  // to avoid flicker, establish a memory dc, draw to it 
  // and then BitBlt it to the client
  memDC.CreateCompatibleDC(&dc) ;
  memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight) ;
  oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap) ;

  if (memDC.GetSafeHdc() != NULL)
  {
    // first drop the grid on the memory dc
    memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
                 &m_dcText, 0, 0, SRCCOPY) ;
    // finally send the result to the display
    dc.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
              &memDC, 0, 0, SRCCOPY) ;
  }

  memDC.SelectObject(oldBitmap) ;

} // OnPaint
void CTextOutCtrl::OnSize(UINT nType, int cx, int cy) 
{
  CWnd::OnSize(nType, cx, cy) ;

  // NOTE: OnSize automatically gets called during the setup of the control
  
  GetClientRect(m_rectClient) ;

  // set some member variables to avoid multiple function calls
  m_nClientHeight = m_rectClient.Height() ;
  m_nClientWidth  = m_rectClient.Width() ;

  // the "left" coordinate and "width" will be modified in 
  // InvalidateCtrl to be based on the width of the y axis scaling
  m_rectPlot.left   = 7 ;  
  m_rectPlot.top    = 5 ;
  m_rectPlot.right  = m_rectClient.right-5 ;
  m_rectPlot.bottom = m_rectClient.bottom-5 ;

  // set some member variables to avoid multiple function calls
  m_nPlotHeight = m_rectPlot.Height() ;
  m_nPlotWidth  = m_rectPlot.Width() ;


} // OnSize

/////////////////////////////////////////////////////////////////////////////
void CTextOutCtrl::Reset()
{
  // to clear the existing data (in the form of a bitmap)
  // simply invalidate the entire control
//  InvalidateCtrl() ;
}
