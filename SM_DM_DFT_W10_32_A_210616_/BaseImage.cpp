// BaseImage.cpp: implementation of the CBaseImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseImage.h"
#include "Defines.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <Math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseImage::CBaseImage()
{
	m_pPalette			    = NULL;   // Bitmap Palette Info
	m_BmInfo			    = NULL;	
	m_pImageData		    = NULL;   // Bitmap Image Data
	m_pProcessedImageData   = NULL;
	lpLogPalette		    = NULL;
	cPalette			    = NULL;
	m_bImageLoaded		    = FALSE;
	m_bProcessedImageLoaded = FALSE;
	m_nModifiedWidth	    = 0;

	//+add kwmoon 070124
	m_bSaveProcessedImage   = FALSE;
	m_szFilePath			= _T("");

	//+add kwmoon 071204
	m_pLoadedFile			= NULL;
}

//+change kwmoon 071204
CBaseImage::~CBaseImage()
{

	if(m_BmInfo)		free(m_BmInfo);		m_BmInfo	 = NULL;
	if(m_pPalette)		delete m_pPalette;	m_pPalette	 = NULL;
	
	//+change 090226(Modification No1)
//	if(m_pImageData)	free(m_pImageData); m_pImageData = NULL;
	if(m_pImageData)	delete [] m_pImageData; m_pImageData = NULL;

	if(m_pProcessedImageData)	free(m_pProcessedImageData); m_pProcessedImageData = NULL;

	m_bImageLoaded = FALSE;
	m_bProcessedImageLoaded = FALSE;	

	// Close Image File
	if(m_pLoadedFile != NULL)
	{
		fclose(m_pLoadedFile);
	}
}

void CBaseImage::InitHeaders(int nColorType,int nWidth,int nHeight,int nImageSize)
{
	int nNoRgbQuad = 0;


	switch(nColorType)
	{
		case MONO_256COLOR :
		{
			m_nModifiedWidth = ((nWidth+3)/4)*4; // 4의 배수로 변경

			// Set BITMAP FILE HEADER
			m_FileHeader.bfType			 = 0x4D42; // 고정값
			m_FileHeader.bfSize			 = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) 
										 + sizeof(RGBQUAD) * 256 + m_nModifiedWidth * nHeight; 
			m_FileHeader.bfReserved1	 = 0;
			m_FileHeader.bfReserved2	 = 0;
			m_FileHeader.bfOffBits	 	 = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) 
										 + sizeof(RGBQUAD) * 256;

			// Set BITMAP INFO HEADER
			m_InfoHeader.biBitCount		 = 8;
			m_InfoHeader.biClrImportant	 = 256;
			m_InfoHeader.biClrUsed		 = 256;
			m_InfoHeader.biCompression	 = 0;
			m_InfoHeader.biHeight		 = nHeight;
			m_InfoHeader.biPlanes		 = 1;
			m_InfoHeader.biSize			 = 40;
			m_InfoHeader.biSizeImage	 = nImageSize;
			m_InfoHeader.biWidth		 = nWidth;
			m_InfoHeader.biXPelsPerMeter = 0;
			m_InfoHeader.biYPelsPerMeter = 0;

			// Set Palette Info
			if(m_pPalette != NULL){ delete m_pPalette; m_pPalette = NULL; }

			if(m_InfoHeader.biBitCount < 24) // 팔레트가 있는 경우 
			{
				nNoRgbQuad = (int)pow(2,m_InfoHeader.biBitCount);

				m_pPalette = new RGBQUAD [nNoRgbQuad]; // 팔레트의 크기만큼 메모리를 할당함 

				for(int i=0; i<nNoRgbQuad;i++)
				{
					m_pPalette[i].rgbGreen	   = i;
					m_pPalette[i].rgbRed	   = i;
					m_pPalette[i].rgbBlue	   = i;
					m_pPalette[i].rgbReserved  = 0;
				}
			}

			// Set BITMAPINFO
			if(m_BmInfo != NULL) free(m_BmInfo); m_BmInfo = NULL;

			m_BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));

			m_BmInfo->bmiHeader.biBitCount		= m_InfoHeader.biBitCount;
			m_BmInfo->bmiHeader.biClrImportant	= m_InfoHeader.biClrImportant;
			m_BmInfo->bmiHeader.biClrUsed		= m_InfoHeader.biClrUsed;
			m_BmInfo->bmiHeader.biCompression	= m_InfoHeader.biCompression;
			m_BmInfo->bmiHeader.biHeight		= m_InfoHeader.biHeight;
			m_BmInfo->bmiHeader.biPlanes		= m_InfoHeader.biPlanes;
			m_BmInfo->bmiHeader.biSize			= m_InfoHeader.biSize;
			m_BmInfo->bmiHeader.biSizeImage		= m_InfoHeader.biSizeImage;
			m_BmInfo->bmiHeader.biWidth			= m_InfoHeader.biWidth; 
			m_BmInfo->bmiHeader.biXPelsPerMeter	= m_InfoHeader.biXPelsPerMeter;
			m_BmInfo->bmiHeader.biYPelsPerMeter	= m_InfoHeader.biYPelsPerMeter;

			if(m_InfoHeader.biBitCount < 24) // 팔레트가 있는 경우 
			{
				nNoRgbQuad = (int)pow(2,m_InfoHeader.biBitCount);

				m_pPalette = new RGBQUAD [nNoRgbQuad]; // 팔레트의 크기만큼 메모리를 할당함 

				for(int i=0; i<nNoRgbQuad;i++)
				{
					m_pPalette[i].rgbGreen	   = i;
					m_pPalette[i].rgbRed	   = i;
					m_pPalette[i].rgbBlue	   = i;
					m_pPalette[i].rgbReserved  = 0;
				}
			}
		}
		break;
		
		case COLOR_24BIT :
		{
			m_nModifiedWidth = ((3*nWidth+3)/4)*4; // 4의 배수로 변경

			// Set BITMAP FILE HEADER
			m_FileHeader.bfType			 = 0x4D42; // 고정값
			
			m_FileHeader.bfSize			 = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) 
										   + m_nModifiedWidth * nHeight; 
			m_FileHeader.bfReserved1	 = 0;
			m_FileHeader.bfReserved2	 = 0;
			m_FileHeader.bfOffBits	 	 = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) ;

			// Set BITMAP INFO HEADER
			m_InfoHeader.biBitCount		 = 24;
			m_InfoHeader.biClrImportant	 = 0;
			m_InfoHeader.biClrUsed		 = 0;
			m_InfoHeader.biCompression	 = 0;
			m_InfoHeader.biHeight		 = nHeight;
			m_InfoHeader.biPlanes		 = 1;
			m_InfoHeader.biSize			 = 40;
			m_InfoHeader.biSizeImage	 = nImageSize;
			m_InfoHeader.biWidth		 = nWidth;
			m_InfoHeader.biXPelsPerMeter = 0;
			m_InfoHeader.biYPelsPerMeter = 0;
		
			// Set BITMAPINFO
			if(m_BmInfo != NULL) free(m_BmInfo); m_BmInfo = NULL;

			m_BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO));

			m_BmInfo->bmiHeader.biBitCount		= m_InfoHeader.biBitCount;
			m_BmInfo->bmiHeader.biClrImportant	= m_InfoHeader.biClrImportant;
			m_BmInfo->bmiHeader.biClrUsed		= m_InfoHeader.biClrUsed;
			m_BmInfo->bmiHeader.biCompression	= m_InfoHeader.biCompression;
			m_BmInfo->bmiHeader.biHeight		= m_InfoHeader.biHeight;
			m_BmInfo->bmiHeader.biPlanes		= m_InfoHeader.biPlanes;
			m_BmInfo->bmiHeader.biSize			= m_InfoHeader.biSize;
			m_BmInfo->bmiHeader.biSizeImage		= m_InfoHeader.biSizeImage;
			m_BmInfo->bmiHeader.biWidth			= m_InfoHeader.biWidth;
			m_BmInfo->bmiHeader.biXPelsPerMeter	= m_InfoHeader.biXPelsPerMeter;
			m_BmInfo->bmiHeader.biYPelsPerMeter	= m_InfoHeader.biYPelsPerMeter;
		}
		break;
	}

//	if(m_pImageData != NULL){ delete m_pImageData; m_pImageData = NULL; }

//	m_pImageData = new BYTE [m_InfoHeader.biSizeImage];

	m_bImageLoaded = FALSE;
}
//-

void CBaseImage::MakeBlankImage()
{
	// Read Bmp File Data
	if(m_pImageData != NULL)
	{ 
		memset(m_pImageData,127,m_InfoHeader.biSizeImage);
	}
}
BOOL CBaseImage::LoadImage(CString szFileName,CProgressCtrl& Progress,BOOL bShowErrorMsg)
{
	int nNoRgbQuad = 0;
	int nIndex, nRcolor, nGcolor, nBcolor;
	
	FILE* pFile = NULL;

	m_bImageLoaded = FALSE;
	
	//+add kwmoon 071204
	m_szFilePath = szFileName;

	// Open Image File
	if(!(pFile = fopen((LPCTSTR)szFileName,"rb")))
	{
		if(bShowErrorMsg) AfxMessageBox("Failed to load Image File : "+szFileName); 
		return FALSE;
	}
	//+add kwmoon 071204
	else
	{
		m_pLoadedFile = pFile;
	}
	Progress.SetPos(10);


	// Read Bitmap File Header
	fread(&m_FileHeader,sizeof(BITMAPFILEHEADER),1,pFile);
	
	if(m_FileHeader.bfType != 0x4D42)
	{
		if(bShowErrorMsg) AfxMessageBox("Unsupported Image File"); 
		return  FALSE;
	}

	Progress.SetPos(20);

	// Read Bitmap Info Header
	fread(&m_InfoHeader,sizeof(BITMAPINFOHEADER),1,pFile);

	if(m_InfoHeader.biBitCount < 8 || m_InfoHeader.biBitCount > 24 )
	{
		if(bShowErrorMsg) AfxMessageBox("Unsupported Image File"); 
		return FALSE;
	}

	Progress.SetPos(30);
	
	if(m_InfoHeader.biBitCount<8 ) 
	{ 
		if(bShowErrorMsg) AfxMessageBox("Unsupported Format(Color Depth < 8)!"); 
		return FALSE;
	}

	// Set Palette Info
	if(m_pPalette != NULL){ delete m_pPalette; m_pPalette = NULL; }

	nNoRgbQuad = (int)pow(2,m_InfoHeader.biBitCount);

	if(m_InfoHeader.biBitCount < 24) 
	{
		m_pPalette= new RGBQUAD [nNoRgbQuad];
		fread(m_pPalette,sizeof(RGBQUAD),nNoRgbQuad,pFile); 
	}

	// Read Bmp File Data
	if(m_pImageData != NULL){ delete m_pImageData; m_pImageData = NULL; }

	m_pImageData = new BYTE [m_InfoHeader.biSizeImage];
	
	MakeBlankImage();

	Progress.SetPos(40);

	// 영상데이타를 저장할 메모리 할당 
	BYTE *lpImg = new BYTE [m_InfoHeader.biSizeImage];
	fread(lpImg,sizeof(char),m_InfoHeader.biSizeImage,pFile); 

	int rwsize = WIDTHBYTES(m_InfoHeader.biBitCount*m_InfoHeader.biWidth);
	int rwsize2= WIDTHBYTES(24*m_InfoHeader.biWidth); 

	if(m_InfoHeader.biBitCount==24)   
	{
		for(int i=0; i<m_InfoHeader.biHeight; i++)
		{
			for(int j=0; j<m_InfoHeader.biWidth; j++) 
			{  
				m_pImageData[i*rwsize2+3*j+2] = lpImg[i*rwsize+3*j+2];
				m_pImageData[i*rwsize2+3*j+1] = lpImg[i*rwsize+3*j+1];
				m_pImageData[i*rwsize2+3*j  ] = lpImg[i*rwsize+3*j  ];
			}
		}
	}
	else 
	{
		for(int i=0; i<m_InfoHeader.biHeight; i++)
		{
			for(int j=0; j<m_InfoHeader.biWidth; j++) 
			{
				nIndex  = lpImg[i*rwsize+j];		     
				nRcolor = m_pPalette[nIndex].rgbRed;    
				nGcolor = m_pPalette[nIndex].rgbGreen;  
				nBcolor = m_pPalette[nIndex].rgbBlue;  

				m_pImageData[i*rwsize2+3*j+2] = (BYTE)nRcolor; 
				m_pImageData[i*rwsize2+3*j+1] = (BYTE)nGcolor;
				m_pImageData[i*rwsize2+3*j  ] = (BYTE)nBcolor;
			}
		}
	}

	if(m_InfoHeader.biBitCount == 8)
	{
		InitHeaders(MONO_256COLOR,m_InfoHeader.biWidth, m_InfoHeader.biHeight,m_InfoHeader.biSizeImage);
	}
	else
	{
		InitHeaders(COLOR_24BIT,m_InfoHeader.biWidth, m_InfoHeader.biHeight,m_InfoHeader.biSizeImage);
	}

	m_bImageLoaded = TRUE;

	fclose(pFile);

	Progress.SetPos(100);

	if(lpImg != NULL) delete []lpImg;

	return TRUE;
}

BOOL CBaseImage::SaveImage(CString szFileName,int nImageType,CProgressCtrl& Progress)
{
	FILE* pFile;
	CString szErrMsg = _T("");
	
	// Open Image File
	if(!(pFile = fopen((LPCTSTR)szFileName,"wb")))
	{
		//+change 090219(Modification No4)
	//	AfxMessageBox("Failed to save Image File"); return FALSE;
		szErrMsg.Format("Failed to save Image File(%s)",szFileName);
		AfxMessageBox(szErrMsg); return FALSE;
	}
	Progress.SetPos(10);

	// Write Bitmap File Header
	fwrite(&m_FileHeader,sizeof(BITMAPFILEHEADER),1,pFile);
	Progress.SetPos(30);
	
	// Write Bitmap Info Header
	fwrite(&m_InfoHeader,sizeof(BITMAPINFOHEADER),1,pFile);
	Progress.SetPos(50);

	// Write Palette Info
	if(m_InfoHeader.biBitCount < 24)
	{
		fwrite(m_pPalette,sizeof(RGBQUAD),m_InfoHeader.biClrUsed,pFile);
	}
	Progress.SetPos(70);

	// Write Image Data
	if((nImageType == SNAP_IMAGE) || (nImageType == LOAD_IMAGE) || (nImageType == PROC_IMAGE) || (nImageType == MASK_IMAGE))
	{
		fwrite(m_pImageData,sizeof(BYTE),m_InfoHeader.biSizeImage,pFile);
	}
	else // INTERNAL_PROC_IMAGE
	{
		fwrite(m_pProcessedImageData,sizeof(BYTE),m_InfoHeader.biSizeImage,pFile);
	}

	Progress.SetPos(90);

	// Close Image File
	fclose(pFile);

	Progress.SetPos(100);
	return TRUE;
}


BOOL CBaseImage::SetSystemPalette(HDC hDC)
{
	//현재 트루칼라모드이면 설정안함
	if(!(::GetDeviceCaps(hDC, RASTERCAPS) & RC_PALETTE)) return FALSE;

	//현재 모드가 몇칼라 모드이며 팔레트엔트리가 몇개인지 확인
	int nSysColors  = ::GetDeviceCaps(hDC, NUMCOLORS);
	int nPalEntries = ::GetDeviceCaps(hDC, SIZEPALETTE);

	if(nPalEntries ==0) return FALSE;
	
	int nEntries = (nPalEntries == 0) ? nSysColors : nPalEntries;

	LPLOGPALETTE pLogPal   = (LPLOGPALETTE) new char[2 * sizeof(WORD) + nEntries * sizeof(PALETTEENTRY)];
	pLogPal->palVersion    = 0x300;
	pLogPal->palNumEntries = nEntries;
	
	//시스템 팔레트를 얻고
	::GetSystemPaletteEntries(hDC, 0, nEntries, (LPPALETTEENTRY) ((LPBYTE) pLogPal + 2 * sizeof(WORD)));

	//시스템 팔레트를 설정한다.
	HPALETTE m_hPalette = ::CreatePalette(pLogPal);
	SelectPalette(hDC, m_hPalette, TRUE);

	delete pLogPal; return TRUE;
}

//+add kwmoon 071204
BOOL CBaseImage::UnloadImage(BOOL bShowErrorMsg)
{
	if(m_BmInfo)		free(m_BmInfo);		m_BmInfo	 = NULL;
	if(m_pPalette)		delete m_pPalette;	m_pPalette	 = NULL;
	
	//+change 090226(Modification No1)
//	if(m_pImageData)	free(m_pImageData); m_pImageData = NULL;
	if(m_pImageData)	delete [] m_pImageData; m_pImageData = NULL;

	if(m_pProcessedImageData)	free(m_pProcessedImageData); m_pProcessedImageData = NULL;

	m_bImageLoaded = FALSE;
	m_bProcessedImageLoaded = FALSE;

	// Open Image File
	if(m_pLoadedFile != NULL)
	{
		if(!fclose(m_pLoadedFile))
		{
			if(bShowErrorMsg) AfxMessageBox("Failed to close a Image File : "+m_szFilePath); 
			return FALSE;
		}
	}

	return TRUE;
}