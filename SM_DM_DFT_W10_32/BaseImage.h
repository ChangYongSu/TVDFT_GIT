 // BaseImage.h: interface for the CBaseImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEIMAGE_H__7E1E6994_7C58_42E8_BC31_8622566DCC35__INCLUDED_)
#define AFX_BASEIMAGE_H__7E1E6994_7C58_42E8_BC31_8622566DCC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBaseImage  
{
public:
	CBaseImage();
	virtual ~CBaseImage();

public:

	void InitHeaders(int nColorType,int nWidth, int nHeight,int nImageSize);

	BOOL LoadImage(CString szFileName,CProgressCtrl& Progress,BOOL bShowErrorMsg = TRUE);
	BOOL SaveImage(CString szFileName,int nImageType,CProgressCtrl& Progress);

	//+add kwmoon 071204
	BOOL UnloadImage(BOOL bShowErrorMsg = TRUE);

	BOOL SetSystemPalette(HDC hDC);
	void MakeBlankImage();

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
};

#endif // !defined(AFX_BASEIMAGE_H__7E1E6994_7C58_42E8_BC31_8622566DCC35__INCLUDED_)
