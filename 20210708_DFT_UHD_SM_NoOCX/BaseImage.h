 // BaseImage.h: interface for the CBaseImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEIMAGE_H__7E1E6994_7C58_42E8_BC31_8622566DCC35__INCLUDED_)
#define AFX_BASEIMAGE_H__7E1E6994_7C58_42E8_BC31_8622566DCC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRGBPixel												// RGB CE¨ù¢¯ A©ø¢¬¢ç A¡þ¡¤¢®¨ö¨¬
{
public:
	int R, G, B;

	CRGBPixel()
		{ R = 0; G = 0; B = 0; }
	CRGBPixel(RGBQUAD rgb)
		{ R = rgb.rgbRed; G = rgb.rgbGreen; B = rgb.rgbBlue; }
	CRGBPixel(COLORREF rgb)
		{ R = GetRValue(rgb); G = GetGValue(rgb); B = GetBValue(rgb); }
	virtual ~CRGBPixel() { }

	// Cu¨¬?E? ¢¯¡þ¡íeAU ¢¯A©öo¡¤I¥ìu
	inline operator RGBQUAD() const;
	inline BOOL operator==(CRGBPixel a)						{ return (R == a.R && G == a.G && B == a.B); }
	
	inline CRGBPixel operator=(RGBQUAD rgb)					{ R = rgb.rgbRed; G = rgb.rgbGreen; B = rgb.rgbBlue; return *this; }
	inline CRGBPixel operator=(CRGBPixel a)					{ R = a.R; G = a.G; B = a.B; return *this; }
	inline CRGBPixel operator*(CRGBPixel a)					{ CRGBPixel r; r.R = R * a.R; r.G = G * a.G; r.B = B * a.B; return r; }
	inline CRGBPixel operator/(CRGBPixel a)					{ CRGBPixel r; r.R = (int)((double)R / a.R + 0.5); r.G = (int)((double)G / a.G + 0.5); r.B = (int)((double)B / a.B + 0.5); return r; }
	inline CRGBPixel operator+(CRGBPixel a) 				{ CRGBPixel r; r.R = R + a.R; r.G = G + a.G; r.B = B + a.B; return r; }
	inline CRGBPixel operator-(CRGBPixel a)					{ CRGBPixel r; r.R = R - a.R; r.G = G - a.G; r.B = B - a.B; return r; }
	inline CRGBPixel operator*=(CRGBPixel a)				{ R = R * a.R; G = G * a.G; B = B * a.B; return *this; }
	inline CRGBPixel operator/=(CRGBPixel a)				{ R = (int)((double)R / a.R + 0.5); G = (int)((double)G / a.G + 0.5); B = (int)((double)B / a.B + 0.5); return *this; }
	inline CRGBPixel operator+=(CRGBPixel a)				{ R = R + a.R; G = G + a.G; B = B + a.B; return *this; }
	inline CRGBPixel operator-=(CRGBPixel a)				{ R = R - a.R; G = G - a.G; B = B - a.B; return *this; }
	inline CRGBPixel operator++()							{ R = R + 1; G = G + 1; B = B + 1; return *this; }
	inline CRGBPixel operator--()							{ R = R - 1; G = G - 1; B = B - 1; return *this; }

	// A¢´¨ùoCu¡Æ¨£¡ÆuAC ¢¯¡þ¡ícAU ¢¯A©öo¡¤I¥ìu
	inline CRGBPixel operator=(int a)						{ R = a; G = a; B = a; return *this; }
	inline CRGBPixel operator*(int a)						{ R = R * a; G = G * a; B = B * a; return *this; }
	inline CRGBPixel operator/(int a)						{ R = (int)((double)R / a + 0.5); G = (int)((double)G / a + 0.5); B = (int)((double)B / a + 0.5); return *this; }
	inline CRGBPixel operator+(int a)						{ CRGBPixel r; r.R = R + a; r.G = G + a; r.B = B + a; return r; }
	inline CRGBPixel operator-(int a)						{ CRGBPixel r; r.R = R - a; r.G = G - a; r.B = B - a; return r; }
	inline CRGBPixel operator*=(int a)						{ CRGBPixel r; r.R = R * a; r.G = G * a; r.B = B * a; return r; }
	inline CRGBPixel operator/=(int a)						{ CRGBPixel r; r.R = (int)((double)R / a + 0.5); r.G = (int)((double)G / a + 0.5); r.B = (int)((double)B / a + 0.5); return r; }
	inline CRGBPixel operator+=(int a)						{ R = R + a; G = G + a; B = B + a; return *this; }
	inline CRGBPixel operator-=(int a)						{ R = R - a; G = G - a; B = B - a; return *this; }

	// Binary ¢¯¡þ¡íeAU ¢¯A©öo¡¤I¥ìu (CA¡¤¡í¥ìaCO¨ùo)
	friend inline CRGBPixel operator*(int a, CRGBPixel b)	{ CRGBPixel r; r.R = a * b.R; r.G = a * b.G; r.B = a * b.B; return r; }
	friend inline CRGBPixel operator/(int a, CRGBPixel b)	{ CRGBPixel r; r.R = (int)((double)a / b.R + 0.5); r.G = (int)((double)a / b.G + 0.5); r.B = (int)((double)a / b.B + 0.5); return r; }
	friend inline CRGBPixel operator+(int a, CRGBPixel b)	{ CRGBPixel r; r.R = a + b.R; r.G = a + b.G; r.B = a + b.B; return r; }
	friend inline CRGBPixel operator-(int a, CRGBPixel b)	{ CRGBPixel r; r.R = a - b.R; r.G = a - b.G; r.B = a - b.B; return r; }
};

CRGBPixel::operator RGBQUAD() const
{
	RGBQUAD rgb; rgb.rgbRed = R; rgb.rgbGreen = G; rgb.rgbBlue = B; return rgb;
}



class CBaseImage  
{
public:
	CBaseImage();
	virtual ~CBaseImage();

public:

	void InitHeaders(int nColorType,int nWidth, int nHeight,int nImageSize, BOOL bJpg = FALSE);

	BOOL LoadImage(CString szFileName,CProgressCtrl& Progress,BOOL bShowErrorMsg = TRUE);
	BOOL LoadJpgImage(CString szFileName,CProgressCtrl& Progress,BOOL bShowErrorMsg = TRUE);
	BOOL SaveImage(CString szFileName,int nImageType,CProgressCtrl& Progress);
	BOOL SaveJpgImage(CString szFileName,int nImageType,CProgressCtrl& Progress);

	//+add kwmoon 071204
	BOOL UnloadImage(BOOL bShowErrorMsg = TRUE);

	BOOL SetSystemPalette(HDC hDC);
	void MakeBlankImage();
	CRGBPixel GetRGBPixelAt(int, int);						// ¨¬n¨¡¢ç¢¬E CE¨ù¢¯¡Æ¨£ ¡Æ¢®A¢ç¢¯A¡¾a (¥ìUAy¨úiA©ª ¨ù©ª¨ù¡©¢¥e¡¤I : ¢¥¡ì¢¯¢® ¨¬¢¬AI¢¥A¡ÆI¡Æu Flip¥ìE A¢ÒC¡Í¡Æ¨£)
	void SetRGBPixelAt(int, int, CRGBPixel);				// ¨¬n¨¡¢ç¢¬E CE¨ù¢¯¡Æ¨£ ¨ù©øA¢´CI¡¾a (¥ìUAy¨úiA©ª ¨ù©ª¨ù¡©¢¥e¡¤I : ¢¥¡ì¢¯¢® ¨¬¢¬AI¢¥A¡ÆI¡Æu Flip¥ìE A¢ÒC¡Í¡Æ¨£)

public:

	int m_nModifiedWidth;
	
	// Data from loading bmp file
	BITMAPFILEHEADER m_FileHeader;   // Bitmap File Header
	BITMAPINFOHEADER m_InfoHeader;   // Bitmap Info Header
	RGBQUAD*		 m_pPalette;     // Bitmap Palette Info

	BITMAPINFO*		 m_BmInfo;
	
	BYTE*			 m_pImageData;			  // Bitmap Image Data
	BYTE*			 m_pProcessedImageData;    // Modified Bitmap Image Data

	BOOL			 m_bImageLoaded;		  // TRUE : Image is loaded
	BOOL			 m_bProcessedImageLoaded;  // TRUE : Image is loaded

	LPLOGPALETTE	lpLogPalette;
	CPalette*		cPalette;

	BOOL m_bSaveProcessedImage;
	CString			m_szFilePath;

	//+add kwmoon 071204
	FILE*			m_pLoadedFile;
	
	UINT m_width, m_height, m_rowbytes;
	DWORD m_dwDibSize;

};

#endif // !defined(AFX_BASEIMAGE_H__7E1E6994_7C58_42E8_BC31_8622566DCC35__INCLUDED_)
