// SeqRoiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "SeqRoiDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeqRoiDlg dialog
CSeqRoiDlg::CSeqRoiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeqRoiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeqRoiDlg)
	m_nDisplayType = DISPLAY_TYPE_FIT;
	//}}AFX_DATA_INIT
	m_nPrevRoiX1 = 0;
	m_nPrevRoiX2 = 0;
	m_nPrevRoiY1 = 0;
	m_nPrevRoiY2 = 0;
}

void CSeqRoiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeqRoiDlg)
	DDX_Control(pDX, IDC_STATIC_Y_POS, m_ctrlYpos);
	DDX_Control(pDX, IDC_STATIC_X_POS, m_ctrlXpos);
	DDX_Control(pDX, IDC_STATIC_ROI_Y2, m_ctrlRoiY2);
	DDX_Control(pDX, IDC_STATIC_ROI_X2, m_ctrlRoiX2);
	DDX_Control(pDX, IDC_SCROLL_V, m_ctrlImgVScroll);
	DDX_Control(pDX, IDC_SCROLL_H, m_ctrlImgHScroll);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
	DDX_Control(pDX, IDC_STATIC_ROI_Y, m_ctrlRoiY1);
	DDX_Control(pDX, IDC_STATIC_ROI_X, m_ctrlRoiX1);
	DDX_Control(pDX, IDC_STATIC_ROI_WIDTH, m_ctrlRoiWidthStc);
	DDX_Control(pDX, IDC_STATIC_ROI_HEIGHT, m_ctrlRoiHeightStc);
	DDX_Control(pDX, IDC_LIST_REF_FILE, m_ctrlRefFileList);
	DDX_Radio(pDX, IDC_RADIO_DISPLAY_TYPE_FULL, m_nDisplayType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeqRoiDlg, CDialog)
	//{{AFX_MSG_MAP(CSeqRoiDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_LBN_SELCHANGE(IDC_LIST_REF_FILE, OnSelchangeListRefFile)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_TYPE_FULL, OnRadioDisplayTypeFull)
	ON_BN_CLICKED(IDC_RADIO_DISPLAY_TYPE_FIT, OnRadioDisplayTypeFit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeqRoiDlg message handlers
//////////////////////////////////////////////////////////////////////////

BOOL CSeqRoiDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Get Ref File List
	GetFileList(m_strModelFolder);

	//Roi Value Init
	m_ctrlRoiX1.SetWindowText(_T(""));
	m_ctrlRoiY1.SetWindowText(_T(""));
	m_ctrlRoiX2.SetWindowText(_T(""));
	m_ctrlRoiY2.SetWindowText(_T(""));
	m_ctrlRoiWidthStc.SetWindowText(_T(""));
	m_ctrlRoiHeightStc.SetWindowText(_T(""));

	//////////////////////////////////////////////////////////////////////////
	//Image View & Page Rect Calculate

	//+change kwmoon 080628
	m_pImgViewWnd = GetDlgItem(IDC_STATIC_IMAGE_VIEW);
//	m_pImgViewWnd = GetDlgItem(IDC_STATIC_VIEW);
	
	//ImageView Rect, Width, Height Calculate
	m_pImgViewWnd->GetWindowRect(&m_ImgViewRect);
//	m_pImgViewWnd->GetWindowRect(&m_ImgViewClientRect);

	//+change kwmoon 080628
	ScreenToClient(&m_ImgViewRect);
//	ScreenToClient(&m_ImgViewClientRect);

	
	//+del kwmoon 080628
//	m_ImgViewRect.top	 = m_ImgViewRect.top	+ 10;
//	m_ImgViewRect.bottom = m_ImgViewRect.bottom - 10;
//	m_ImgViewRect.left	 = m_ImgViewRect.left	+ 10;
//	m_ImgViewRect.right	 = m_ImgViewRect.right	- 10;

	m_nImgViewHeight = m_ImgViewRect.bottom - m_ImgViewRect.top;
	m_nImgViewWidth  = m_ImgViewRect.right  - m_ImgViewRect.left;

	//Scroll Init
	m_nScrHCur   = 0;
	m_nScrVCur   = 0;
	m_nScrHMax   = 0;
	m_nScrVMax   = 0;
	m_nStartX    = 0;
	m_nStartY    = 0;

	//Roi Value Init
	m_nRoiX1	 = 0;
	m_nRoiY1	 = 0;
	m_nRoiX2	 = 0;
	m_nRoiY2	 = 0;
	m_nRoiWidth  = 0;
	m_nRoiHeight = 0;

	//Flag Init
	m_bFlagMouse = FALSE;
	m_bFlagCrop  = FALSE;

	//+add kwmoon 080627
	m_nStepX	= 5;	
	m_nStepY	= 5;
	m_nPageX	= 10;	
	m_nPageY	= 10;
	
	m_fXratio	= 1.0;
	m_fYratio	= 1.0;
	
	//+add kwmoon 080628
	m_nImageWidth	= 0;
	m_nImageHeight	= 0;	

	m_nDisplayType = DISPLAY_TYPE_FIT;

	CString sTmp; 
	sTmp.Format("Image ROI Select - %s", m_strModelFolder);
	SetWindowText(sTmp);
	SetIcon(((CDATsysApp *)AfxGetApp())->LoadIcon(IDR_MAINFRAME), FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeqRoiDlg::GetFileList(CString sFolder)
{
	CFileFind finder;

	BOOL bWorking = FALSE;
	CString sTmp  = _T("");
	
	::SetCurrentDirectory(sFolder);

	m_aRefFile.RemoveAll();

	bWorking = finder.FindFile("*.bmp");
	
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		m_aRefFile.Add(finder.GetFileName());
	}

	m_ctrlRefFileList.ResetContent();

	if(!m_aRefFile.GetSize()) return;

	for(int nTmp = 0; nTmp < m_aRefFile.GetSize(); nTmp++)
	{
		m_ctrlRefFileList.AddString(m_aRefFile.GetAt(nTmp));
	}
}

void CSeqRoiDlg::OnSelchangeListRefFile() 
{
	int nCurSel = m_ctrlRefFileList.GetCurSel();
	if((nCurSel < 0)||(nCurSel > m_aRefFile.GetSize())) return;

	m_strRefPath = m_strModelFolder + m_aRefFile.GetAt(nCurSel);
	OpenImgFile(m_strRefPath);
}

void CSeqRoiDlg::DrawImage(int nDlgItemNo,CBaseImage& ImageData, int nStartXPos, int nStartYPos)
{
	CWnd *pWnd = (CWnd*)GetDlgItem(nDlgItemNo);

 //   CBrush brush(RGB(128, 128, 128));

	if(!pWnd->GetSafeHwnd())
	{
		AfxMessageBox("Failed to Show Image"); return;
	}

	CDC* pDC = pWnd->GetDC();

	CRect rect;
	pWnd->GetWindowRect(&rect);
	pWnd->ScreenToClient(&rect);

	rect.top	= rect.top + 1;
	rect.bottom = rect.bottom - 1;
	rect.left	= rect.left + 1;
	rect.right	= rect.right - 1;


	// Palette 설정
	HPALETTE hPalette	 = NULL;          
	HPALETTE hOldPalette = NULL;       

	int nX1 = 0;
	int nX2 = 0;
	int nY1 = 0;
	int nY2 = 0;

	if(ImageData.cPalette != NULL)
	{	
		hPalette	= (HPALETTE)ImageData.cPalette;
		hOldPalette = ::SelectPalette(pDC->GetSafeHdc(), hPalette, TRUE);
		RealizePalette(pDC->GetSafeHdc());
	}
	else
	{
		if(ImageData.SetSystemPalette(pDC->GetSafeHdc()))
		{
			RealizePalette(pDC->GetSafeHdc());
		}
	}

	::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);

	int nWidth, nHeight;

	if(ImageData.m_BmInfo->bmiHeader.biWidth < rect.Width())
	{
		nWidth = ImageData.m_BmInfo->bmiHeader.biWidth;
	}
	else nWidth = rect.Width();

	if(ImageData.m_BmInfo->bmiHeader.biHeight < rect.Height())
	{
		nHeight = ImageData.m_BmInfo->bmiHeader.biHeight;
	}
	else nHeight = rect.Height();

	CPen dottedYellowLine(PS_DOT, 1, DARK_YELLOW_COLOR);
	CPen solidRedLine(PS_SOLID, 3, LIGHT_RED_COLOR);
	CPen* pOldPen = pDC->SelectObject(&solidRedLine); 

//	FloodFill(pDC->GetSafeHdc(),nStartXPos,nStartYPos, RGB(128, 128, 128));
//	pDC->FillSolidRect(nStartXPos, nStartYPos, m_ImgViewRect.Width(), m_ImgViewRect.Height(), RGB(255, 255, 255));
	//+add kwmoon 080627
/*	if((nHeight < rect.Height()) && (nWidth  < rect.Width()))
	{
		SetDIBitsToDevice(pDC->GetSafeHdc()							//dc에 대한 핸들
						  ,0//1										//출력할 영상 영역의 왼쪽 위 X좌표
						  ,0//1										//출력할 영상 영역의 왼쪽 위 Y좌표
						  ,nWidth									//영상의 가로폭(픽셀단위)
						  ,nHeight									//영상의 세로폭(픽셀단위)
						  ,nStartXPos								//출력할 소스 영상의 왼쪽-아래 X 좌표
						  ,nStartYPos								//출력할 소스 영상의 왼쪽-아래 Y 좌표
						  ,0										//배열에서 출력할 첫 번째 스캔라인
						  ,ImageData.m_BmInfo->bmiHeader.biHeight	//출력할 스캔라인의 수
						  ,ImageData.m_pImageData					//영상데이터(장치 독립 비트맵의 비트배열
						  ,ImageData.m_BmInfo						//영상데이터의 비트맵 정보
						  ,DIB_RGB_COLORS);							//RGB 또는 팔레트 정보


		if((m_nPrevRoiX1 < (m_nImageWidth-1)) 
		&& (m_nPrevRoiX2 < (m_nImageWidth-1)) 
		&& (m_nPrevRoiX2 > m_nPrevRoiX1)
		&& (m_nPrevRoiY1 < (m_nImageHeight-1)) 
		&& (m_nPrevRoiY2 < (m_nImageHeight-1)) 
		&& (m_nPrevRoiY2 > m_nPrevRoiY1))
		{
			pDC->MoveTo(m_nPrevRoiX1,m_nPrevRoiY1);
			pDC->LineTo(m_nPrevRoiX2,m_nPrevRoiY1);
			pDC->LineTo(m_nPrevRoiX2,m_nPrevRoiY2);
			pDC->LineTo(m_nPrevRoiX1,m_nPrevRoiY2);
			pDC->LineTo(m_nPrevRoiX1,m_nPrevRoiY1);
		}
	}
	else
	{ */
		if(m_nDisplayType == DISPLAY_TYPE_FULL)
		{	
			SetDIBitsToDevice(pDC->GetSafeHdc()							//dc에 대한 핸들
							  ,0//1										//출력할 영상 영역의 왼쪽 위 X좌표
							  ,0//1										//출력할 영상 영역의 왼쪽 위 Y좌표
							  ,nWidth									//영상의 가로폭(픽셀단위)
							  ,nHeight									//영상의 세로폭(픽셀단위)
							  ,nStartXPos								//출력할 소스 영상의 왼쪽-아래 X 좌표
							  ,nStartYPos								//출력할 소스 영상의 왼쪽-아래 Y 좌표
							  ,0										//배열에서 출력할 첫 번째 스캔라인
							  ,ImageData.m_BmInfo->bmiHeader.biHeight	//출력할 스캔라인의 수
							  ,ImageData.m_pImageData					//영상데이터(장치 독립 비트맵의 비트배열
							  ,ImageData.m_BmInfo						//영상데이터의 비트맵 정보
							  ,DIB_RGB_COLORS);							//RGB 또는 팔레트 정보

			if((m_nPrevRoiX1 < (m_nImageWidth-1)) 
			&& (m_nPrevRoiX2 < (m_nImageWidth-1)) 
			&& (m_nPrevRoiX2 > m_nPrevRoiX1)
			&& (m_nPrevRoiY1 < (m_nImageHeight-1)) 
			&& (m_nPrevRoiY2 < (m_nImageHeight-1)) 
			&& (m_nPrevRoiY2 > m_nPrevRoiY1))
			{
				pDC->MoveTo(m_nPrevRoiX1,m_nPrevRoiY1);
				pDC->LineTo(m_nPrevRoiX2,m_nPrevRoiY1);
				pDC->LineTo(m_nPrevRoiX2,m_nPrevRoiY2);
				pDC->LineTo(m_nPrevRoiX1,m_nPrevRoiY2);
				pDC->LineTo(m_nPrevRoiX1,m_nPrevRoiY1);
			}

		}
		else
		{
			StretchDIBits(pDC->GetSafeHdc()
						  ,0//1
						  ,0//1
						  ,rect.Width()
						  ,rect.Height()
						  ,0
						  ,0
						  ,ImageData.m_BmInfo->bmiHeader.biWidth
						  ,ImageData.m_BmInfo->bmiHeader.biHeight
						  ,ImageData.m_pImageData
						  ,ImageData.m_BmInfo
						  ,DIB_RGB_COLORS
						  ,SRCCOPY);

			nX1 = (int)((float)(m_nPrevRoiX1 / m_fXratio)); 
			nX2 = (int)((float)(m_nPrevRoiX2 / m_fXratio)); 
			nY1 = (int)((float)(m_nPrevRoiY1 / m_fYratio)); 
			nY2 = (int)((float)(m_nPrevRoiY2 / m_fYratio)); 

//			if(nX1 > (nWidth-1))  nX1 = nWidth-1;
//			if(nX2 > (nWidth-1))  nX2 = nWidth-1;
//			if(nY1 > (nHeight-1)) nY1 = nHeight-1;
///			if(nY2 > (nHeight-1)) nY2 = nHeight-1;	

			if(nX1 > (m_ImgViewRect.Width()-1))  nX1 = m_ImgViewRect.Width()-1;
			if(nX2 > (m_ImgViewRect.Width()-1))  nX2 = m_ImgViewRect.Width()-1;
			if(nY1 > (m_ImgViewRect.Height()-1)) nY1 = m_ImgViewRect.Height()-1;
			if(nY2 > (m_ImgViewRect.Height()-1)) nY2 = m_ImgViewRect.Height()-1;	
			
			pDC->MoveTo(nX1,nY1);
			pDC->LineTo(nX2,nY1);
			pDC->LineTo(nX2,nY2);
			pDC->LineTo(nX1,nY2);
			pDC->LineTo(nX1,nY1);
		}
//	}

	// Palette 복원
	if(hOldPalette != NULL)
	{
		::SelectPalette(pDC->GetSafeHdc(), hOldPalette, TRUE);
	}

	pDC->SelectObject(&dottedYellowLine); 

	if(m_bFlagMouse == TRUE)
	{
		pDC->MoveTo(m_CropRect.left, m_CropRect.top);
		pDC->LineTo(m_CropRect.right,m_CropRect.top);
		pDC->LineTo(m_CropRect.right,m_CropRect.bottom);
		pDC->LineTo(m_CropRect.left, m_CropRect.bottom);
		pDC->LineTo(m_CropRect.left, m_CropRect.top);
	//	pDC->DrawEdge(&m_CropRect,EDGE_ETCHED,BF_RECT); 
	}
	else if(m_bFlagCrop == TRUE) 
	{
		pDC->MoveTo(m_CropRect.left, m_CropRect.top);
		pDC->LineTo(m_CropRect.right,m_CropRect.top);
		pDC->LineTo(m_CropRect.right,m_CropRect.bottom);
		pDC->LineTo(m_CropRect.left, m_CropRect.bottom);
		pDC->LineTo(m_CropRect.left, m_CropRect.top);
	//	pDC->DrawEdge(&m_CropRect,EDGE_ETCHED,BF_RECT); 
	}

	pDC->SelectObject(pOldPen);

	pWnd->ReleaseDC(pDC);
}

void CSeqRoiDlg::OpenImgFile(CString sFilePath)
{
	CString sTmp;

	if(!FileExists(sFilePath))
	{
		sTmp.Format("%s\n File doesn't Exist!", sFilePath);
		AfxMessageBox(sTmp);
		return;
	}
	
	if(!m_Image.LoadImage(sFilePath, m_ctrlProgress)) return;

	//+add kwmoon 080628
	m_nImageWidth	= m_Image.m_BmInfo->bmiHeader.biWidth;
	m_nImageHeight	= m_Image.m_BmInfo->bmiHeader.biHeight;	

	InitScrollBar();

	//+add kwmoon 080627
	if(m_nDisplayType == DISPLAY_TYPE_FULL)
	{
		m_nPageX = (int)((float)m_nScrHMax * 0.1);
		m_nPageY = (int)((float)m_nScrVMax * 0.1);
		m_nStepX = (int)((float)m_nPageX * 0.5);
		m_nStepY = (int)((float)m_nPageY * 0.5);

		if(m_nStepX == 0) m_nStepX = MIN_LINE_SCROLL;
		if(m_nStepY == 0) m_nStepY = MIN_LINE_SCROLL;
	}
	else
	{
		m_fXratio = (float)m_nImageWidth/(float)m_ImgViewRect.Width();
		m_fYratio = (float)m_nImageHeight/(float)m_ImgViewRect.Height();
	}

	m_bFlagMouse = FALSE;
	m_bFlagCrop  = FALSE;

	InvalidateRect(m_ImgViewRect, TRUE);
	UpdateWindow();
}

void CSeqRoiDlg::SetVScrollBar()
{
	m_nScrVMax = m_nImageHeight - m_nImgViewHeight;

	if(m_nDisplayType == DISPLAY_TYPE_FULL)
	{
		if(m_nScrVMax > 0)
		{
			m_ctrlImgVScroll.SetScrollRange(0, m_nScrVMax);
			m_ctrlImgVScroll.ShowScrollBar(TRUE);
			m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgVScroll.SetScrollPos(m_nScrVCur, TRUE);

			m_nStartY = (m_nImageHeight - m_nScrVCur - 1) - m_ImgViewRect.Height()+ 1;
		}
		else
		{
			m_ctrlImgVScroll.SetScrollRange(0, 0);
			m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgVScroll.SetScrollPos(0, TRUE);
			m_ctrlImgVScroll.ShowScrollBar(FALSE);
		}
	}
	else
	{
		m_ctrlImgVScroll.SetScrollRange(0, 0);
		m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
		m_ctrlImgVScroll.SetScrollPos(0, TRUE);
		m_ctrlImgVScroll.ShowScrollBar(FALSE);

		m_nScrVCur = 0;
	}
}

void CSeqRoiDlg::SetHScrollBar()
{
	m_nScrHMax = m_nImageWidth - m_nImgViewWidth;

	if(m_nDisplayType == DISPLAY_TYPE_FULL)
	{
		if(m_nScrHMax > 0)
		{
			m_ctrlImgHScroll.SetScrollRange(0, m_nScrHMax);
			m_ctrlImgHScroll.ShowScrollBar(TRUE);
			m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgHScroll.SetScrollPos(m_nScrHCur, TRUE);

			m_nStartX = m_nScrHCur;
		}
		else
		{
			m_ctrlImgHScroll.SetScrollRange(0, 0);
			m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgHScroll.SetScrollPos(0, TRUE);
			m_ctrlImgHScroll.ShowScrollBar(FALSE);
		}
	}
	else
	{
		m_ctrlImgHScroll.SetScrollRange(0, 0);
		m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
		m_ctrlImgHScroll.SetScrollPos(0, TRUE);
		m_ctrlImgHScroll.ShowScrollBar(FALSE);

		m_nScrHCur = 0;
	}
}

void CSeqRoiDlg::InitScrollBar()
{
	m_nStartX  = m_nScrHCur = 0;
	m_nStartY  = m_nScrVCur = 0;
	m_nScrHMax = m_nImageWidth - m_nImgViewWidth;
	m_nScrVMax = m_nImageHeight - m_nImgViewHeight;
	
	if(m_nDisplayType == DISPLAY_TYPE_FULL)
	{
		if(m_nScrHMax > 0)
		{
			m_ctrlImgHScroll.SetScrollRange(0, m_nScrHMax);
			m_ctrlImgHScroll.ShowScrollBar(TRUE);
			m_ctrlImgHScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgHScroll.SetScrollPos(m_nScrHCur, TRUE);
		}
		else
		{
			m_ctrlImgHScroll.ShowScrollBar(FALSE);
		}

		if(m_nScrVMax > 0)
		{
			m_ctrlImgVScroll.SetScrollRange(0, m_nScrVMax);
			m_ctrlImgVScroll.ShowScrollBar(TRUE);
			m_ctrlImgVScroll.EnableScrollBar(ESB_ENABLE_BOTH);
			m_ctrlImgVScroll.SetScrollPos(m_nScrVCur, TRUE);
		
			m_nStartY = (m_nImageHeight - m_nScrVCur - 1) - m_ImgViewRect.Height() + 1;
		}
		else
		{
			m_ctrlImgVScroll.ShowScrollBar(FALSE);
		}
	}
	else
	{
		m_ctrlImgHScroll.ShowScrollBar(FALSE);
		m_ctrlImgVScroll.ShowScrollBar(FALSE);
	}
}

void CSeqRoiDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if((point.y < m_ImgViewRect.top)||(point.y > m_ImgViewRect.bottom)) return;
	if((point.x < m_ImgViewRect.left)||(point.x > m_ImgViewRect.right)) return;
	if(m_Image.m_bImageLoaded == FALSE) return;

	int nPointX, nPointY;
	nPointX = point.x - m_ImgViewRect.left  + m_nScrHCur;
	nPointY = point.y  - m_ImgViewRect.top  + m_nScrVCur;

//	if(m_nDisplayType == DISPLAY_TYPE_FULL){
//		if((nPointX >= m_nImageWidth)
//		|| (nPointY >= m_nImageHeight)) return;
//	}
	m_CropRect.left = m_CropRect.right  = point.x - m_ImgViewRect.left;
	m_CropRect.top  = m_CropRect.bottom = point.y - m_ImgViewRect.top;

	if(m_nDisplayType == DISPLAY_TYPE_FULL)
	{
		if((m_nImageWidth < m_ImgViewRect.Width()) && (m_nImageHeight < m_ImgViewRect.Height()))
		{
			m_nRoiX1 = nPointX;
			m_nRoiY1 = nPointY;
		}
		else{
			m_nRoiX1 = nPointX;
			m_nRoiY1 = nPointY;
			if(nPointX > (m_nImageWidth-1))  m_nRoiX1 = (m_nImageWidth-1);
			if(nPointY > (m_nImageHeight-1)) m_nRoiY1 = (m_nImageHeight-1);
		}
		if(m_CropRect.right > (m_nImageWidth-1))  m_CropRect.left = m_CropRect.right = (m_nImageWidth-1);
		if(m_CropRect.bottom > (m_nImageHeight-1)) m_CropRect.top  = m_CropRect.bottom = (m_nImageHeight-1);
	}
	else
	{
		m_nRoiX1 = (int)((float)nPointX * m_fXratio);
		m_nRoiY1 = (int)((float)nPointY * m_fYratio);

		if(m_nRoiX1 > (m_nImageWidth-1))  m_nRoiX1 = (m_nImageWidth-1);
		if(m_nRoiY1 > (m_nImageHeight-1)) m_nRoiY1 = (m_nImageHeight-1);
	}

	m_bFlagMouse    = TRUE;

	CString sTmp;
	sTmp.Format("%d", m_nRoiX1);
	m_ctrlRoiX1.SetWindowText(sTmp);
	sTmp.Format("%d", m_nRoiY1);
	m_ctrlRoiY1.SetWindowText(sTmp);

	InvalidateRect(&m_ImgViewRect,FALSE);	

	CDialog::OnLButtonDown(nFlags, point);
}

void CSeqRoiDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	UINT nTemp;
	CString sTmp;

	if(m_bFlagMouse == TRUE)
	{
		if((point.x < m_ImgViewRect.left)||(point.x > m_ImgViewRect.right))
		{
			m_bFlagMouse = m_bFlagCrop = FALSE; return;
		}

		if((point.y < m_ImgViewRect.top) || (point.y > m_ImgViewRect.bottom))
		{
			m_bFlagMouse = m_bFlagCrop = FALSE;	return;
		}

//		if((m_CropRect.top >= m_CropRect.bottom) || (m_CropRect.left >= m_CropRect.right))
//		{
//			m_bFlagMouse = m_bFlagCrop = FALSE; return;
//		}

		if((abs(m_CropRect.Width()) <= MIN_LINE_SCROLL)||(abs(m_CropRect.Height()) <= MIN_LINE_SCROLL))
		{
			m_bFlagMouse = m_bFlagCrop = FALSE; return;
		}

		int nPointX, nPointY;
		nPointX = point.x - m_ImgViewRect.left + m_nScrHCur;
		nPointY = point.y  - m_ImgViewRect.top + m_nScrVCur;

/*			if((nPointX >= m_nImageWidth)
			|| (nPointY >= m_nImageHeight)) 
			{
				m_bFlagMouse = m_bFlagCrop = FALSE; return;
			}
*/
		m_CropRect.right	= point.x - m_ImgViewRect.left;
		m_CropRect.bottom	= point.y - m_ImgViewRect.top;


		
		if(m_nDisplayType == DISPLAY_TYPE_FULL)
		{
			if((nPointX < m_nImageWidth) && (nPointY < m_nImageHeight))
			{
				m_nRoiX2			= nPointX;
				m_nRoiY2			= nPointY;
			}
			else{
				m_nRoiX2			= nPointX;
				m_nRoiY2			= nPointY;
				if(nPointX > (m_nImageWidth-1))  m_nRoiX2 = (m_nImageWidth-1);
				if(nPointY > (m_nImageHeight-1)) m_nRoiY2 = (m_nImageHeight-1);
			}
			if(m_CropRect.right > (m_nImageWidth-1))  m_CropRect.right = (m_nImageWidth-1);
			if(m_CropRect.bottom > (m_nImageHeight-1)) m_CropRect.bottom = (m_nImageHeight-1);
		}
		else 
		{
			m_nRoiX2			= (int)((float)(point.x - m_ImgViewRect.left ) * m_fXratio);
			m_nRoiY2			= (int)((float)(point.y  - m_ImgViewRect.top ) * m_fYratio);

			if(m_nRoiX2 > (m_nImageWidth-1))  m_nRoiX2 = (m_nImageWidth-1);
			if(m_nRoiY2 > (m_nImageHeight-1)) m_nRoiY2 = (m_nImageHeight-1);
		}
		if(m_nRoiY1 > m_nRoiY2)
		{
			nTemp = m_nRoiY1;
			m_nRoiY1 = m_nRoiY2;
			m_nRoiY2 = nTemp;
			sTmp.Format("%d", m_nRoiY1);
			m_ctrlRoiY1.SetWindowText(sTmp);

		}

		if(m_nRoiX1 > m_nRoiX2)
		{
			nTemp = m_nRoiX1;
			m_nRoiX1 = m_nRoiX2;
			m_nRoiX2 = nTemp;
			sTmp.Format("%d", m_nRoiX1);
			m_ctrlRoiX1.SetWindowText(sTmp);
		}


		m_nRoiWidth			= m_nRoiX2 - m_nRoiX1;
		m_nRoiHeight		= m_nRoiY2 - m_nRoiY1;

		m_bFlagMouse		= FALSE;
		m_bFlagCrop			= TRUE;


		sTmp.Format("%d", m_nRoiX2);
		m_ctrlRoiX2.SetWindowText(sTmp);
		sTmp.Format("%d", m_nRoiY2);
		m_ctrlRoiY2.SetWindowText(sTmp);
	
		sTmp.Format("%d", m_nRoiHeight);
		m_ctrlRoiHeightStc.SetWindowText(sTmp);
		sTmp.Format("%d", m_nRoiWidth);
		m_ctrlRoiWidthStc.SetWindowText(sTmp);

		InvalidateRect(&m_ImgViewRect,FALSE);	
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CSeqRoiDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if((point.x < m_ImgViewRect.left) || (point.x > m_ImgViewRect.right)) 
	{
		return;
	}
	if((point.y < m_ImgViewRect.top) || (point.y > m_ImgViewRect.bottom)) 
	{
		return;
	}

	int nPointX = point.x - m_ImgViewRect.left + m_nScrHCur;
	int nPointY = point.y - m_ImgViewRect.top  + m_nScrVCur;

	if(m_nDisplayType == DISPLAY_TYPE_FULL)
	{
		if((nPointX >= m_nImageWidth)
		|| (nPointY >= m_nImageHeight)) 
		{
			return;
		}
	}

	if((m_nImageHeight >= m_ImgViewRect.Height()) 
	|| (m_nImageWidth  >= m_ImgViewRect.Width()))
	{
		if(m_nDisplayType == DISPLAY_TYPE_FIT)
		{
			nPointX = (int)((float)(point.x - m_ImgViewRect.left) * m_fXratio);
			nPointY = (int)((float)(point.y  - m_ImgViewRect.top) * m_fYratio);

			if(nPointX > (m_nImageWidth-1))  nPointX = (m_nImageWidth-1);
			if(nPointY > (m_nImageHeight-1)) nPointY = (m_nImageHeight-1);
		}
	}

	if(m_bFlagMouse == TRUE)
	{
		m_CropRect.right  = point.x - m_ImgViewRect.left;
		m_CropRect.bottom = point.y - m_ImgViewRect.top;

		CString sTmp;
		sTmp.Format("%d", nPointX);
		m_ctrlXpos.SetWindowText(sTmp);
		sTmp.Format("%d", nPointY);
		m_ctrlYpos.SetWindowText(sTmp);
	}
	else
	{
		CString sTmp;
		sTmp.Format("%d", nPointX);
		m_ctrlXpos.SetWindowText(sTmp);
		sTmp.Format("%d", nPointY);
		m_ctrlYpos.SetWindowText(sTmp);
	}

	InvalidateRect(&m_ImgViewRect,FALSE);

	CDialog::OnMouseMove(nFlags, point);
}

void CSeqRoiDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_Image.m_bImageLoaded == TRUE)
	{
		//+change kwmoon 080628
		DrawImage(IDC_STATIC_IMAGE_VIEW, m_Image, m_nStartX, m_nStartY);
	//	DrawImage(IDC_STATIC_VIEW, m_Image, m_nStartX, m_nStartY);
/*
		CPen dottedYellowLine(PS_DOT, 1, DARK_YELLOW_COLOR);
		CPen solidRedLine(PS_SOLID, 3, LIGHT_RED_COLOR);
		CPen* pOldPen = dc.SelectObject(&dottedYellowLine); 

		if(m_bFlagMouse == TRUE)
		{
			dc.MoveTo(m_CropRect.left, m_CropRect.top);
			dc.LineTo(m_CropRect.right,m_CropRect.top);
			dc.LineTo(m_CropRect.right,m_CropRect.bottom);
			dc.LineTo(m_CropRect.left, m_CropRect.bottom);
			dc.LineTo(m_CropRect.left, m_CropRect.top);
		//	dc.DrawEdge(&m_CropRect,EDGE_ETCHED,BF_RECT); 
		}
		else if(m_bFlagCrop == TRUE) 
		{
			dc.MoveTo(m_CropRect.left, m_CropRect.top);
			dc.LineTo(m_CropRect.right,m_CropRect.top);
			dc.LineTo(m_CropRect.right,m_CropRect.bottom);
			dc.LineTo(m_CropRect.left, m_CropRect.bottom);
			dc.LineTo(m_CropRect.left, m_CropRect.top);
		//	dc.DrawEdge(&m_CropRect,EDGE_ETCHED,BF_RECT); 
		}

		dc.SelectObject(&solidRedLine); 

		//+add kwmoon 080627
		if((nHeight < rect.Height()) && (nWidth  < rect.Width()))
		{
			if(m_nDisplayType == DISPLAY_TYPE_FIT)
			{
				int nX1 = m_ImgViewRect.left + (int)((float)(m_nPrevRoiX1 / m_fXratio)); 
				int nX2 = m_ImgViewRect.left + (int)((float)(m_nPrevRoiX2 / m_fXratio)); 
				int nY1 = m_ImgViewRect.top  + (int)((float)(m_nPrevRoiY1 / m_fYratio)); 
				int nY2 = m_ImgViewRect.top  + (int)((float)(m_nPrevRoiY2 / m_fYratio)); 

				if((nX2 < m_nImageWidth) && (nY2 < m_nImageHeight))
				{
					dc.MoveTo(nX1,nY1);
					dc.LineTo(nX2,nY1);
					dc.LineTo(nX2,nY2);
					dc.LineTo(nX1,nY2);
					dc.LineTo(nX1,nY1);
				}
			}
		}
		dc.SelectObject(pOldPen);
		*/
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CSeqRoiDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(pScrollBar == &m_ctrlImgHScroll)
	{
		int nScrollPos = pScrollBar->GetScrollPos();
		
		switch(nSBCode) 
		{
			case SB_LINELEFT:
				if(nScrollPos > 0) nScrollPos -= m_nStepX;
				else nScrollPos = 0;
				break;
			case SB_PAGELEFT:
				if(nScrollPos > m_nPageX) nScrollPos -= m_nPageX;
				else nScrollPos = 0;
				break;
			case SB_LINERIGHT:
				if(nScrollPos < m_nScrHMax) nScrollPos+= m_nStepX;
				else nScrollPos = m_nScrHMax;
				break;
			case SB_PAGERIGHT:
				if(nScrollPos < m_nScrHMax - m_nPageX) nScrollPos+= m_nPageX;
				else nScrollPos = m_nScrHMax;
				break;
			case SB_THUMBTRACK:
				nScrollPos = nPos;
				break;
		}

		if(nScrollPos < 0) nScrollPos = 0;
		if(nScrollPos > m_nScrHMax) nScrollPos = m_nScrHMax;

		m_nScrHCur = nScrollPos;
		pScrollBar->SetScrollPos(m_nScrHCur);
		
		if(m_nScrHMax > 0)
		{
			m_nStartX = m_nScrHCur;
		}

		m_bFlagCrop  = FALSE;

		InvalidateRect(&m_ImgViewRect,FALSE);
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSeqRoiDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(pScrollBar == &m_ctrlImgVScroll)
	{
		int nScrollPos;
		nScrollPos = pScrollBar->GetScrollPos();
		switch(nSBCode) 
		{
			case SB_LINELEFT:
				if(nScrollPos > 0) nScrollPos -= m_nStepY;
				else nScrollPos = 0;
				break;
			case SB_PAGELEFT:
				if(nScrollPos > m_nPageY) nScrollPos -= m_nPageY;
				else nScrollPos = 0;
				break;
			case SB_LINERIGHT:
				if(nScrollPos < m_nScrVMax) nScrollPos+= m_nStepY;
				else nScrollPos = m_nScrVMax;
				break;
			case SB_PAGERIGHT:
				if(nScrollPos < m_nScrVMax - m_nPageY) nScrollPos+= m_nPageY;
				else nScrollPos = m_nScrVMax;
				break;
			case SB_THUMBTRACK:
				nScrollPos = nPos;
				break;
		}
		
		if(nScrollPos < 0) nScrollPos = 0;
		if(nScrollPos > m_nScrVMax) nScrollPos = m_nScrVMax;

		m_nScrVCur = nScrollPos;
		pScrollBar->SetScrollPos(nScrollPos);
		
		if(m_nScrVMax > 0)
		{
			m_nStartY = (m_nImageHeight - m_nScrVCur - 1) - m_ImgViewRect.Height()+ 1;
		}

		m_bFlagCrop  = FALSE;

		InvalidateRect(&m_ImgViewRect,FALSE);
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSeqRoiDlg::OnRadioDisplayTypeFull() 
{
	CWnd *pWnd = (CWnd*)GetDlgItem(IDC_STATIC_IMAGE_VIEW);
	if(!pWnd->GetSafeHwnd())
	{
		AfxMessageBox("Failed to Show Image"); return;
	}

	CDC* pDC = pWnd->GetDC();
	pDC->FillSolidRect(0, 0, m_ImgViewRect.Width(), m_ImgViewRect.Height(), RGB(255, 255, 255));

	m_nDisplayType = DISPLAY_TYPE_FULL;	

	m_nPageX = (int)((float)m_nScrHMax * (float)0.1);
	m_nPageY = (int)((float)m_nScrVMax * (float)0.1);
	m_nStepX = (int)((float)m_nPageX * 0.5);
	m_nStepY = (int)((float)m_nPageY * 0.5);

	if(m_nStepX == 0) m_nStepX = MIN_LINE_SCROLL;
	if(m_nStepY == 0) m_nStepY = MIN_LINE_SCROLL;

	SetHScrollBar();
	SetVScrollBar();

	m_bFlagCrop  = FALSE;

	InvalidateRect(&m_ImgViewRect,FALSE);

	pWnd->ReleaseDC(pDC);
	
	UpdateWindow();
}

void CSeqRoiDlg::OnRadioDisplayTypeFit() 
{
	CWnd *pWnd = (CWnd*)GetDlgItem(IDC_STATIC_IMAGE_VIEW);
	if(!pWnd->GetSafeHwnd())
	{
		AfxMessageBox("Failed to Show Image"); return;
	}

	CDC* pDC = pWnd->GetDC();
	pDC->FillSolidRect(0, 0, m_ImgViewRect.Width(), m_ImgViewRect.Height(), RGB(255, 255, 255));

	m_nDisplayType = DISPLAY_TYPE_FIT;	

	SetHScrollBar();
	SetVScrollBar();

	m_bFlagCrop  = FALSE;

	InvalidateRect(&m_ImgViewRect,FALSE);

	pWnd->ReleaseDC(pDC);

	UpdateWindow();
}
