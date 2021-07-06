// ImageProc.cpp: implementation of the CImageProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageProc.h"
#include "Global.h"

#include <Math.h>
#include <malloc.h>

#define MAX_MASK_SIZE	9
#define DEFAULT_POSITION_MARGIN		10
#define MAX_NO_IMAGE_TO_MAKE_AVERAGE_IMAGE	10

//+add 090219(Modification No2)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

//=============
// Filter Mask
//=============
int Mask3x3_a[3*3] = { 
	114,114,114,
	114,112,114,
	114,114,114 };
	
int Mask5x5_a[5*5] = { 
	41,41,41,41,41,
	41,41,41,41,41,
	41,41,40,41,41,
	41,41,41,41,41,
	41,41,41,41,41 }; 

int Mask5x5_b[5*5] = { 
	  0,-41, -41,-41,  0,
	-41, 41,  41, 41,-41,
	-41, 41, 164, 41,-41,
	-41, 41,  41, 41,-41,
	  0,-41, -41,-41,  0
};

int Mask7x7_a[7*7] = {
	0,-1534,-1535,-1535,-1535,-1534,0,
	-1534,-205,103,205,103,-205,-1534,
	-1535,103,2867,3268,2867,103,-1535,
	-1535,205,3268,5328,3268,205,-1535,
	-1535,103,2867,3268,2867,103,-1535,
	-1534,-205,103,205,103,-205,-1534,
	0,-1534,-1535,-1535,-1535,-1534,0
};

extern CDATsysView*  g_pView;
extern BOOL RefImage_DirectLoad(CString szRefFileName,int nUseMaskImage);


//========================
// Temporary Image Buffer
//========================
#define FULL_HD_IMAGE_SIZE	6220800	// 1920*1080*3 = 6220800

BYTE g_ImageBuffer1[6220800];
BYTE g_ImageBuffer2[6220800];
BYTE g_ImageBuffer3[6220800];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageProc::CImageProc()
{
	m_nColorMarginLevel		= 0;
	m_nBrightMarginLevel	= 0;

	m_nNoUsedColorHighBits	= 0;
	m_nNoUsedBrightHighBits	= 0;

	m_fTargetMatchRate		= 99.5f;

	m_ptPositionShift.x		= 0;
	m_ptPositionShift.y		= 0;

	m_ptRoiPosition.x		= 0;
	m_ptRoiPosition.y		= 0;

	m_nRoiWidth				= 0;
	m_nRoiHeight			= 0;
	
	m_nAvgMaskWidth			= 0;
	m_nAvgMaskHeight		= 0;
	m_nNoRepeat				= 0;

	m_fLowerLimit			= 0.0;
	m_fUpperLimit			= 0.0;

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++)
		{
			m_aNoDifferentPoint[i][j] = 0;
		}
	}
	m_nSecondDifferentPointFreePercent = -1;

	//+del kwmoon 080715
//	m_fAllowedErrorPixelPercentage = 0;
	m_nCaptionWidth  = 0;
	m_nCaptionHeight = 0;
	
	//+del kwmoon 080715
//	m_fAllowedErrorPixelPercentageForOsdTest	= 0;

	//+add kwmoon 071224
	m_nCaptionKeyDelay		= 0;
	m_nCaptionEnterKeyDelay	= 0;
	m_nCaptionCheckTime		= 0;

	//+change kwmoon 080124
	//+add kwmoon 080110
//	m_nNoKeyPress			= 0;
	m_nNoOnKeyPress			= 0;
	m_nNoOffKeyPress		= 0;

	//+add kwmoon 071127
	m_nDigitalEyeDelay = 0;

	//+add kwmoon 080121
	m_ptOsdPosition.x		= 0;
	m_ptOsdPosition.y		= 0;
	
	//+add kwmoon 080121
	m_nOsdWidth				= 0;
	m_nOsdHeight			= 0;

	//+add kwmoon 080828
	m_ptInvalidPosition.x	= 0;
	m_ptInvalidPosition.y	= 0;
	
	m_nInvalidWidth			= 0;
	m_nInvalidHeight		= 0;

	//+add kwmoon 080228
	for(int i=0; i<3; i++)
	{
		m_nNoPixelInMiddleRange[i]	= 0;
		m_nNoNonBlackPixel[i]		= 0;
	}

	//+add kwmoon 080715
	for(int i=0; i<5;i++)
	{
		m_nNoBandErrorPixel[i]			= 0;
		m_nNoAllowedBandErrorPixel[i]	= 0;

		m_fBandErrorPixelPercentage[i]			= 0.0f;
		m_fAllowedBandErrorPixelPercentage[i]	= 0.0f;
	}

	//+add kwmoon 080715
	m_nNoRoiPixel	 = 0;
	m_nNoTargetPixel = 0;
    m_nNoGoodPixel	 = 0;
	m_fGoodPixelPercentage = 0.0f;

	//+add kwmoon 080828
	m_bInvalidateInvalidRegion = FALSE;

	m_nDiff_LowerLimit = -20;
	m_nDiff_UpperLimit = 20;
	m_fMovingPicTest_Limit = 0.2;

}

CImageProc::~CImageProc()
{

}

// Set Position Margin
void CImageProc::SetMatchRate(float fMatchRate)
{
	m_fTargetMatchRate	= fMatchRate;
}

// Set Color Margin
void CImageProc::SetColorMargin(int nColorMargin)
{
	m_nColorMarginLevel		 = nColorMargin;
}

// Set Color Limit
void CImageProc::SetColorLimit(float fLowerLimit, float fUpperLimit)
{
	m_fLowerLimit = fLowerLimit;
	m_fUpperLimit = fUpperLimit;
}

// Set Bright Margin
void CImageProc::SetBrightMargin(int nBrightMargin)
{
	m_nBrightMarginLevel	  = nBrightMargin;
}

void CImageProc::SetUsedNoColorHighBits(int nNoUsedColorHighBits)
{
	m_nNoUsedColorHighBits   = nNoUsedColorHighBits;
}

void CImageProc::SetUsedNoBrightHighBits(int nNoUsedBrightHighBits)
{
	m_nNoUsedBrightHighBits   = nNoUsedBrightHighBits;
}

void CImageProc::SetCaptionSize(int nWidth, int nHeight)
{
	m_nCaptionWidth   = nWidth;
	m_nCaptionHeight  = nHeight;
}

void CImageProc::SetCaptionKey(int nCaptionKeyDelay, int nCaptionEnterKeyDelay, int nNoOnKeyPress, int nNoOffKeyPress)
{
	m_nCaptionKeyDelay		 = nCaptionKeyDelay;
	m_nCaptionEnterKeyDelay  = nCaptionEnterKeyDelay;

	//+change kwmoon 080124
	m_nNoOnKeyPress			 = nNoOnKeyPress;
	m_nNoOffKeyPress		 = nNoOffKeyPress;
}

//+add kwmoon 071224
void CImageProc::SetCaptionCheckTime(int nCaptionCheckTime)
{
	m_nCaptionCheckTime	= nCaptionCheckTime;
}


//+add kwmoon 071112
void CImageProc::SetDigitalEyeDelay(int nDelay)
{
	m_nDigitalEyeDelay  = nDelay;
}

// Set Region of Interest (ROI) 
void CImageProc::SetRoi(POINT ptRoi1Position,POINT ptRoi2Position)
{
	m_ptRoiPosition.x = ptRoi1Position.x;
	m_ptRoiPosition.y = ptRoi1Position.y;
	m_nRoiWidth		  = ptRoi2Position.x - ptRoi1Position.x;
	m_nRoiHeight	  = ptRoi2Position.y - ptRoi1Position.y;
}

// Set Position Shift
void CImageProc::SetPositionShift(CPoint ptPositionShift)
{
	m_ptPositionShift.x = ptPositionShift.x;
	m_ptPositionShift.y = ptPositionShift.y;
}

void CImageProc::CalcTestRange(CBaseImage& SnapImage,
							  int& nStartXposition, int& nEndXposition, int& nStartYposition, int& nEndYposition)
{
	CString szString = _T("");

	int nSnapWidth  = SnapImage.m_InfoHeader.biWidth;
	int nSnapHeight = SnapImage.m_InfoHeader.biHeight;
	
	if((m_nRoiHeight > 0) && (m_nRoiWidth > 0)) // In case of applying ROI
	{
		// Set start position & end position
		if(m_ptPositionShift.x >= 0) 
		{ 
			nStartXposition = m_ptRoiPosition.x;
			nEndXposition   = nStartXposition + m_nRoiWidth;

			if((nEndXposition + m_ptPositionShift.x) > nSnapWidth) 
			{
				nEndXposition   = nEndXposition - m_ptPositionShift.x;
			}
		}
		else
		{ 
			nStartXposition = m_ptRoiPosition.x;
			nEndXposition   = nStartXposition + m_nRoiWidth;

			if((nStartXposition - abs(m_ptPositionShift.x)) < 0) 
			{
				nStartXposition = nStartXposition + abs(m_ptPositionShift.x);
				//+change kwmoon 080819
			//	nEndXposition   = nStartXposition - abs(m_ptPositionShift.x);
				nEndXposition   = nEndXposition - abs(m_ptPositionShift.x);
			}

		}

		if(m_ptPositionShift.y >= 0) 
		{	
			nStartYposition = GetOppositeStartYposition(m_ptRoiPosition.y,m_nRoiHeight,nSnapHeight);
			nEndYposition   = nStartYposition + m_nRoiHeight;
			
			if((nStartYposition - abs(m_ptPositionShift.y)) < 0) 
			{
				nStartYposition = nStartYposition + abs(m_ptPositionShift.y);
				nEndYposition   = nEndYposition - abs(m_ptPositionShift.y);
			}
		}
		else
		{ 
			nStartYposition = GetOppositeStartYposition(m_ptRoiPosition.y,m_nRoiHeight,nSnapHeight);
			nEndYposition   = nStartYposition + m_nRoiHeight;

			if((nEndYposition + abs(m_ptPositionShift.y)) > nSnapHeight) 
			{
				nEndYposition   = nEndYposition - abs(m_ptPositionShift.y);
			}
		}
	}
	else // In case of full size test
	{
		// Set start position & end position
		if(m_ptPositionShift.x >= 0) 
		{ 
			//+change kwmoon 080618
		//	nStartXposition = 0;
			nStartXposition = m_ptPositionShift.x;

			//+change kwmoon 080618
		//	nEndXposition   = (RefImage.m_InfoHeader.biWidth-1)-m_ptPositionShift.x;
			nEndXposition   = (nSnapWidth-1)-m_ptPositionShift.x;
		}
		else
		{ 
			nStartXposition = abs(m_ptPositionShift.x); 
			//+change kwmoon 080618
		//	nEndXposition   = (RefImage.m_InfoHeader.biWidth-1)-abs(m_ptPositionShift.x);
			nEndXposition   = (nSnapWidth-1)-abs(m_ptPositionShift.x);
		}

		if(m_ptPositionShift.y >= 0) 
		{ 
			//+change kwmoon 080618
		//	nStartYposition = 0;
			nStartYposition = m_ptPositionShift.y;
			//+change kwmoon 080618
		//	nEndXposition   = (RefImage.m_InfoHeader.biWidth-1)-abs(m_ptPositionShift.x);
			nEndYposition   = (nSnapHeight-1)-m_ptPositionShift.y;
		}
		else
		{ 
			nStartYposition = 0; 
			//+change kwmoon 080618
		//	nEndXposition   = (RefImage.m_InfoHeader.biWidth-1)-abs(m_ptPositionShift.x);
			nEndYposition   = (nSnapHeight-1)-abs(m_ptPositionShift.y);
		}
	}
}

/*-----------------------------------------------------------------------------------------
 함수명 : RGBTest

 기  능 : 
     - Grab한 이미지와 Reference 이미지를 비교하여 Spec In/Out을 판정함
     - Mask 이미지를 참조하여 Black 영역만을 검사함
	 - 각 Pixel을 1:1로 비교하는 방식과 칼라값의 상위 몇 Bit를 비교하는 2가지 방식이 있음
     - 각 Pixel을 1:1로 비교하는 방식의 경우, 가장 큰 차이를 갖는 칼라값 정보를 제공함
 
  입력파라미터 :
     -  int nType : OSD_IMAGE_TEST    - OSD 검사, Grab이미지와 Ref이미지의 ROI영역에 대해 Avg-Filter 적용함
				    NORMAL_IMAGE_TEST - COLOR 검사, Ref이미지는 이미 Avg-Filter(3x3) 적용해 두었으므로, 
					                    Grab에만 Avg-Filter(3x3) 적용함 
	 - CBaseImage& SnapImage	: Grab한 이미지가 저장되어 있는 Object	                 
	 - CBaseImage& RefImage		: Reference 이미지가 저장되어 있는 Object	                 
	 - CBaseImage& MaskImage	: Mask 이미지가 저장되어 있는 Object	                 
 	 - POINT& ptResultPosition	: 가장 큰 칼라값 차이가 발견된 좌표
	 - float& fMaxDifference	: 가장 큰 칼라값 차이
	 - int& nTotalNoErrorPixel	: SpecOut인 Pixel의 총 개수
	 - BOOL bUseMaskImage		: 칼라값 비교시, Mask 이미지를 참조할지의 여부

  Return 값 :
     - TEST_PASS, TEST_FAIL 또는 TEST_ABORT 

-----------------------------------------------------------------------------------------*/
int CImageProc::RGBTest(int nType, CBaseImage& SnapImage, CBaseImage& RefImage, CBaseImage& MaskImage,
						POINT& ptResultPosition,float& fMaxDifference,int& nTotalNoErrorPixel,
						BOOL bUseMaskImage,CProgressCtrl& Progress)
{
	int nProgress	= 0;
	int nTestResult = TEST_PASS;

	//==================================
	// Dimension of the Reference Image
	//==================================
	int nRefWidth   = RefImage.m_InfoHeader.biWidth;	 
	int nRefHeight  = RefImage.m_InfoHeader.biHeight;
	int nMaskWidth  = 0;	 
	int nMaskHeight = 0;

	if(MaskImage.m_bImageLoaded)
	{
		nMaskWidth  = MaskImage.m_InfoHeader.biWidth;	 
		nMaskHeight = MaskImage.m_InfoHeader.biHeight;
	}

	//=============================
	// Dimension of the Snap Image
	//=============================
	int nSnapWidth    = SnapImage.m_InfoHeader.biWidth;
	int nSnapHeight   = SnapImage.m_InfoHeader.biHeight;
	int nSnapBitCount = SnapImage.m_InfoHeader.biBitCount;

	//=================
	// Local Variables
	//=================
	int nDifference	  = 0;

	int nGrabRcolor   = 0,nGrabGcolor = 0,nGrabBcolor = 0;
	int nRefRcolor	  = 0,nRefGcolor  = 0,nRefBcolor  = 0;

	int nShiftedDiff  = 0;
	int nColorMargin  = 0;
	int nLowerLimit	  = 0,nUpperLimit   = 0;
	int nRefRGBvaule  = 0,nSnapRGBvalue = 0;

	int nNoShift = 0;

	int nStartXposition = 0,nStartYposition	= 0;
	int nEndXposition   = 0,nEndYposition   = 0;

	int nColorDepth	 = RefImage.m_InfoHeader.biBitCount;
	int nBytesInRow  = WIDTHBYTES(nColorDepth*RefImage.m_InfoHeader.biWidth); 

	int nNoLevel	 = (int)pow(2,(nColorDepth/3))-1;

	BYTE* SnapImageBackup = NULL;
	BYTE* RefImageBackup  = NULL;

	POINT ptPassPlusResultPosition	= {0,0};
	POINT ptPassMinusResultPosition	= {0,0};

	POINT ptNgPlusResultPosition	= {0,0};
	POINT ptNgMinusResultPosition	= {0,0};

	int	  nPassPlusMaxDifference	= 0;
	int	  nNgPlusMaxDifference		= 0;
	int   nPassMinusMaxDifference	= 0;
	int   nNgMinusMaxDifference		= 0;

	int	  nImageBufferSize			= 0;
	int	  nImageSize				= 0;
	int	  nTestRetryFlag			= 0;

	int	  nPlusMaxDifference		= 0;
	int	  nMinusMaxDifference		= 0;
	POINT ptPlusResultPosition;
	POINT ptMinusResultPosition;

	int	  nAvgMaskWidth				= 3;
	int	  nAvgMaskHeight			= 3;

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");


	CString szErrMsg = _T("");

START_TEST :

	//=======================
	// Check Image Attribute
	//=======================
	if(CheckImageAttribute(SnapImage,RefImage,MaskImage) == TEST_ABORT)
	{
		return TEST_ABORT;
	}

	nTotalNoErrorPixel		= 0;
	m_nNoErrorInfo			= 0;
    m_nNoGoodPixel			= 0;
	m_fGoodPixelPercentage	= 0.0f;

	//============
	// Init array
	//============
	if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData || g_pView->m_bAdjSpecMode)
	{
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++) m_aNoDifferentPoint[i][j] = 0;
		}
		m_nSecondDifferentPointFreePercent = -1;
	}

	for(int i=0; i<5;i++)
	{
		m_nNoBandErrorPixel[i]			= 0;
		m_nNoAllowedBandErrorPixel[i]	= 0;
		m_fBandErrorPixelPercentage[i]	= 0.0f;
	}

	Progress.SetPos(10);

	//=========================================
	// Allocate memory for the processed image
	//=========================================
	nImageSize = SnapImage.m_InfoHeader.biSizeImage;

	if(SnapImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(SnapImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(SnapImage.m_pProcessedImageData); SnapImage.m_pProcessedImageData = NULL;
			SnapImage.m_pProcessedImageData = (BYTE *)malloc(SnapImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		SnapImage.m_pProcessedImageData = (BYTE *)malloc(SnapImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(SnapImage.m_pProcessedImageData,0,SnapImage.m_BmInfo->bmiHeader.biSizeImage);

	//============================================
	// Allocate memory for temporary image buffer
	//============================================
	//+change kwmoon 080820
//	BYTE* pProcessedImageData = (BYTE *)malloc(SnapImage.m_InfoHeader.biSizeImage); 
//	memset(pProcessedImageData,0,SnapImage.m_InfoHeader.biSizeImage);
	BYTE* pProcessedImageData = g_ImageBuffer1;
	memset(g_ImageBuffer1,0,FULL_HD_IMAGE_SIZE);

	Progress.SetPos(20);

	nLowerLimit = Percentage2Level(m_fLowerLimit,nNoLevel);
	nUpperLimit = Percentage2Level(m_fUpperLimit,nNoLevel);

	if(m_nNoUsedColorHighBits > 0)
	{
		nNoShift = SnapImage.m_InfoHeader.biBitCount/3 - m_nNoUsedColorHighBits;
	}

	//=======================
	// Calculate Coordinates
	//=======================
	CalcTestRange(SnapImage,nStartXposition,nEndXposition,nStartYposition,nEndYposition);

	//======================
	// Apply Average Filter
	//======================
	if(nType == OSD_IMAGE_TEST)
	{
		if(Average(nAvgMaskWidth,nAvgMaskHeight,nStartXposition,nEndXposition,
			       nStartYposition,nEndYposition,SnapImage,FALSE) == TEST_FAIL)
		{
			AfxMessageBox("Failed to run average-filter");
			return TEST_ABORT;	
		}

		if(Average(nAvgMaskWidth,nAvgMaskHeight,nStartXposition,nEndXposition,
			       nStartYposition,nEndYposition,RefImage,FALSE) == TEST_FAIL)	
		{
			AfxMessageBox("Failed to run average-filter"); return TEST_ABORT;	
		}
	}
/*	else 
	{
		if(Average(nAvgMaskWidth,nAvgMaskHeight,nStartXposition,nEndXposition,
			       nStartYposition,nEndYposition,SnapImage,FALSE) == TEST_FAIL)	
		{
			AfxMessageBox("Failed to run average-filter"); return TEST_ABORT;	
		}
		if(RefImage.m_pProcessedImageData == NULL)
		{
			RefImage.m_pProcessedImageData = (BYTE *)malloc(RefImage.m_BmInfo->bmiHeader.biSizeImage);
		}
		memcpy((BYTE*)RefImage.m_pProcessedImageData,(BYTE*)RefImage.m_pImageData,(int)RefImage.m_InfoHeader.biSizeImage);

	}
*/
	SnapImage.m_bProcessedImageLoaded = FALSE;

	int nX = 0,nY = 0;
	int nColorChannel = 0;
	int nLastErrorXpos  = -1,nLastErrorYpos = -1;
	int nSnapImageIndex = 0, nRefImageIndex = 0;
	int	nBlockIndex		= 0, nMaskValue		= 0;
	int nDifferenceR    = 0, nDifferenceG   = 0, nDifferenceB = 0;
	int nMaxDifference  = 0, nMinDifference = 0;

	BOOL bInvalidMask	= TRUE;

	m_nNoTargetPixel = 0;

	//20101207 Change :m_pImageData -> m_pProcessedImageData
	//=====================
	// Compare each pixels
	//=====================
	for(int nRow = nStartYposition; nRow <= nEndYposition; nRow++)
	{
		for(int nColumn = nStartXposition; nColumn <= nEndXposition; nColumn++) 
		{ 
			nRefImageIndex  = (nRow)*nBytesInRow+3*(nColumn); 
			nSnapImageIndex = (nRow-m_ptPositionShift.y)*nBytesInRow+3*(nColumn+m_ptPositionShift.x);

			if(MaskImage.m_bImageLoaded && bUseMaskImage)
			{
				nMaskValue =  MaskImage.m_pImageData[nRefImageIndex] 
							| MaskImage.m_pImageData[nRefImageIndex+1] 
							| MaskImage.m_pImageData[nRefImageIndex+2]; 
			}

			if(nMaskValue == 0) // Inspection Region
			{
				m_nNoTargetPixel++;

				bInvalidMask = FALSE;

				if(nType == OSD_IMAGE_TEST)
				{
					nRefBcolor  = RefImage.m_pProcessedImageData[nRefImageIndex];
					nGrabBcolor = SnapImage.m_pProcessedImageData[nSnapImageIndex];

					nRefGcolor  = RefImage.m_pProcessedImageData[nRefImageIndex+1];
					nGrabGcolor = SnapImage.m_pProcessedImageData[nSnapImageIndex+1];
					
					nRefRcolor  = RefImage.m_pProcessedImageData[nRefImageIndex+2];
					nGrabRcolor = SnapImage.m_pProcessedImageData[nSnapImageIndex+2];
				}
				else{
					nRefBcolor  = RefImage.m_pImageData[nRefImageIndex];
					nGrabBcolor = SnapImage.m_pImageData[nSnapImageIndex];

					nRefGcolor  = RefImage.m_pImageData[nRefImageIndex+1];
					nGrabGcolor = SnapImage.m_pImageData[nSnapImageIndex+1];
					
					nRefRcolor  = RefImage.m_pImageData[nRefImageIndex+2];
					nGrabRcolor = SnapImage.m_pImageData[nSnapImageIndex+2];
				}
				//===================
				// Color Difference
				//===================
				if(m_nNoUsedColorHighBits > 0)
				{
					nDifferenceB = ((nGrabBcolor>>nNoShift) - (nRefBcolor>>nNoShift)); 
					nDifferenceG = ((nGrabGcolor>>nNoShift) - (nRefGcolor>>nNoShift)); 
					nDifferenceR = ((nGrabRcolor>>nNoShift) - (nRefRcolor>>nNoShift)); 
				}
				else
				{
					nDifferenceB = (nGrabBcolor - nRefBcolor); 
					nDifferenceG = (nGrabGcolor - nRefGcolor); 
					nDifferenceR = (nGrabRcolor - nRefRcolor); 
				}

				//===================
				// Difference Image
				//===================
				//+change kwmoon 080820
			//	pProcessedImageData[nRefImageIndex]	  = abs(nDifferenceB);
			//	pProcessedImageData[nRefImageIndex+1] = abs(nDifferenceG);
			//	pProcessedImageData[nRefImageIndex+2] = abs(nDifferenceR);

				if((abs(nDifferenceG) <= nUpperLimit) && (abs(nDifferenceG) >= nLowerLimit)  // R
				&& (abs(nDifferenceG) <= nUpperLimit) && (abs(nDifferenceG) >= nLowerLimit)  // G
				&& (abs(nDifferenceB) <= nUpperLimit) && (abs(nDifferenceB) >= nLowerLimit)) // B
				{
					// Spec In
					pProcessedImageData[nRefImageIndex+2] = 0;
					pProcessedImageData[nRefImageIndex+1] = 0;
					pProcessedImageData[nRefImageIndex  ] = 0;
				}
				else
				{
					// Spec Out
					pProcessedImageData[nRefImageIndex+2] = 255;
					pProcessedImageData[nRefImageIndex+1] = 255;
					pProcessedImageData[nRefImageIndex  ] = 255;
				}
				//-

				nMaxDifference = GetMaxValue(nDifferenceB,nDifferenceG,nDifferenceR); 
				nMinDifference = GetMinValue(nDifferenceB,nDifferenceG,nDifferenceR); 
				nDifference	   = abs(nMaxDifference) >= abs(nMinDifference) ? nMaxDifference : nMinDifference; 
				
				//============
				// Error Pixel
				//============
				if(((m_nNoUsedColorHighBits  > 0) && (abs(nDifference) > 0))
				|| ((m_nNoUsedColorHighBits == 0) && ((nMaxDifference > nUpperLimit) || (nMinDifference < nLowerLimit))))
				{
					if(m_nNoUsedColorHighBits > 0)
					{
						nDifference = nDifference<<nNoShift;
					}

					if((nColumn != nLastErrorXpos) || (nRow != nLastErrorYpos))
					{
						nTotalNoErrorPixel++;

						float fDifferenePercentage = 0;

						if(nDifference >= 0) // nDifference > nUpperLimit
						{ 
							fDifferenePercentage = Level2Percentage(abs(nDifference-nUpperLimit),nNoLevel);

							if(fDifferenePercentage  <  2) m_nNoBandErrorPixel[0]++;
							if((fDifferenePercentage >= 2) && (fDifferenePercentage < 4)) m_nNoBandErrorPixel[1]++;
							if((fDifferenePercentage >= 4) && (fDifferenePercentage < 6)) m_nNoBandErrorPixel[2]++;
							if((fDifferenePercentage >= 6) && (fDifferenePercentage < 8)) m_nNoBandErrorPixel[3]++;
							if(fDifferenePercentage  >= 8) m_nNoBandErrorPixel[4]++;
						}
						else // nDifference < nLowerLimit
						{
							fDifferenePercentage = Level2Percentage(abs(nDifference-nLowerLimit),nNoLevel);

							if(fDifferenePercentage  <  2) m_nNoBandErrorPixel[0]++;
							if((fDifferenePercentage >= 2) && (fDifferenePercentage < 4)) m_nNoBandErrorPixel[1]++;
							if((fDifferenePercentage >= 4) && (fDifferenePercentage < 6)) m_nNoBandErrorPixel[2]++;
							if((fDifferenePercentage >= 6) && (fDifferenePercentage < 8)) m_nNoBandErrorPixel[3]++;
							if(fDifferenePercentage  >= 8) m_nNoBandErrorPixel[4]++;
						}
						nLastErrorXpos = nColumn;
						nLastErrorYpos = nRow;
					}

					if((nDifference <0) && (nDifference < nNgMinusMaxDifference))
					{
						nNgMinusMaxDifference     = nDifference;
						ptNgMinusResultPosition.x = nColumn;
						ptNgMinusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
					}

					if((nDifference >=0) && (nDifference >= nNgPlusMaxDifference))
					{
						nNgPlusMaxDifference     = nDifference;
						ptNgPlusResultPosition.x = nColumn;
						ptNgPlusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
					}
				}
				//============
				// Good Pixel
				//============
				else
				{
					m_nNoGoodPixel++;

					if((nDifference <0) && (nDifference < nPassMinusMaxDifference))
					{
						nPassMinusMaxDifference     = nDifference;
						ptPassMinusResultPosition.x = nColumn;
						ptPassMinusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
					}

					if((nDifference >=0) && (nDifference >= nPassPlusMaxDifference))
					{
						nPassPlusMaxDifference     = nDifference;
						ptPassPlusResultPosition.x = nColumn;
						ptPassPlusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
					}
				}

				if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData || g_pView->m_bAdjSpecMode)
				{
					nBlockIndex = (int)Level2Percentage(abs(nDifferenceB),nNoLevel);
					m_aNoDifferentPoint[0][nBlockIndex]++;

					nBlockIndex = (int)Level2Percentage(abs(nDifferenceG),nNoLevel);
					m_aNoDifferentPoint[1][nBlockIndex]++;

					nBlockIndex = (int)Level2Percentage(abs(nDifferenceR),nNoLevel);
					m_aNoDifferentPoint[2][nBlockIndex]++;
				}
			}
		}
	}

	//============================
	// Store data for spec-adjust
	//============================
	if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData || g_pView->m_bAdjSpecMode)
	{
		int nDifferentPointFreePercentNo = 0;
	
		for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++)
		{
			if((m_aNoDifferentPoint[0][j] == 0) && (m_aNoDifferentPoint[1][j] == 0) && (m_aNoDifferentPoint[2][j] == 0))
			{
				nDifferentPointFreePercentNo++;

				if(nDifferentPointFreePercentNo == 2)
				{
					m_nSecondDifferentPointFreePercent = j; break;
				}
			}
		}
	}

	//==============
	// Invalid Mask
	//==============
	if(bInvalidMask)
	{
		CString szMsg	   = _T("");
		CString szFileName = _T("");

		if(nType == GRAB_CHECK_TEST)
		{
			szMsg.Format("[Grab_Image_Check] No test was performed because of a mask image values. check mask image! (%d,%d)",m_nRoiWidth,m_nRoiHeight);
		}
		else 
		{
			szMsg.Format("[Color Test] No test was performed because of a mask image values. check mask image!");
		}

		if(nTestRetryFlag == 0)
		{
			if(RefImage.m_szFilePath != _T(""))
			{
				szFileName = RefImage.m_szFilePath.Right(RefImage.m_szFilePath.GetLength()-RefImage.m_szFilePath.ReverseFind('\\')-1);

				if(!RefImage_DirectLoad(szFileName,1))
				{
					AfxMessageBox(szMsg); return TEST_ABORT;
				}
				else
				{
					nTestRetryFlag = 1;	goto START_TEST;
				}
			}
		}
		AfxMessageBox(szMsg); return TEST_ABORT;
	}

	SnapImage.m_bProcessedImageLoaded = TRUE;

	Progress.SetPos(100);

	//==========================================================
	// Copy temoporary image to SnapImage.m_pProcessedImageData
	//==========================================================
	memcpy((BYTE*)SnapImage.m_pProcessedImageData,(BYTE*)pProcessedImageData,(int)SnapImage.m_InfoHeader.biSizeImage);

	//=======================================
	// Deallocate memory for temporary image
	//=======================================

	//+change kwmoon 080820
/*
	if(pProcessedImageData != NULL)
	{
		free(pProcessedImageData); pProcessedImageData = NULL;
	}
*/
	pProcessedImageData = NULL;

	//=======================
	// Make Difference Image
	//=======================
	//+del kwmoon 080820
//	BinaryForDiffImage(NORMAL_BINARY,SnapImage,nLowerLimit,nUpperLimit);

	//=============================
	// Store data for error pixels
	//=============================
	for(int i=0; i<5; i++)
	{
		if(m_nNoBandErrorPixel[i] == 0)
		{
			m_fBandErrorPixelPercentage[i] = 0;
		}
		else
		{
			m_fBandErrorPixelPercentage[i] = (float)m_nNoBandErrorPixel[i] * 100 / (float)m_nNoTargetPixel;
		}
		m_nNoAllowedBandErrorPixel[i]  = (int)((float)m_fAllowedBandErrorPixelPercentage[i] * 0.01 * (float)m_nNoTargetPixel);
	}
	m_fGoodPixelPercentage =(float)m_nNoGoodPixel * 100 / (float)m_nNoTargetPixel;

	//===========================
	// Find Max Difference Value
	//===========================
	if(nTotalNoErrorPixel == 0)
	{
		nTestResult				= TEST_PASS;
		nPlusMaxDifference		= nPassPlusMaxDifference;
		nMinusMaxDifference		= nPassMinusMaxDifference;
		ptPlusResultPosition	= ptPassPlusResultPosition;
		ptMinusResultPosition	= ptPassMinusResultPosition;
	}
	else
	{
		if((m_fBandErrorPixelPercentage[0] < CurrentSet->fAllowedBandErrorPixelPercentage[0])
		&& (m_fBandErrorPixelPercentage[1] < CurrentSet->fAllowedBandErrorPixelPercentage[1])
		&& (m_fBandErrorPixelPercentage[2] < CurrentSet->fAllowedBandErrorPixelPercentage[2])
		&& (m_fBandErrorPixelPercentage[3] < CurrentSet->fAllowedBandErrorPixelPercentage[3])
		&& (m_fBandErrorPixelPercentage[4] < CurrentSet->fAllowedBandErrorPixelPercentage[4]))
		{
			nTestResult				= TEST_PASS;
			nPlusMaxDifference		= nPassPlusMaxDifference;
			nMinusMaxDifference		= nPassMinusMaxDifference;
			ptPlusResultPosition	= ptPassPlusResultPosition;
			ptMinusResultPosition	= ptPassMinusResultPosition;
		}	
		else
		{
			nTestResult				= TEST_FAIL;
			nPlusMaxDifference		= nNgPlusMaxDifference;
			nMinusMaxDifference		= nNgMinusMaxDifference;
			ptPlusResultPosition	= ptNgPlusResultPosition;
			ptMinusResultPosition	= ptNgMinusResultPosition;
		}
	}

	if(nPlusMaxDifference >= abs(nMinusMaxDifference))
	{
		nMaxDifference		= nPlusMaxDifference;
		ptResultPosition.x  = ptPlusResultPosition.x;
		ptResultPosition.y  = ptPlusResultPosition.y;
	}
	else
	{
		nMaxDifference		= nMinusMaxDifference;
		ptResultPosition.x  = ptMinusResultPosition.x;
		ptResultPosition.y  = ptMinusResultPosition.y;
	}
	fMaxDifference = Level2Percentage(nMaxDifference,nNoLevel);

	return nTestResult;
}


//+add kwmoon 080124
int CImageProc::GetMaxValue(int nDifferenceB,int nDifferenceG,int nDifferenceR)
{
	int nDifference	   = 0;
	int nMaxDifference = 0;
	
	nDifference	   = nDifferenceB >= nDifferenceG ? nDifferenceB : nDifferenceG;
	nMaxDifference = nDifference  >= nDifferenceR ? nDifference  : nDifferenceR;

	return nMaxDifference;
}

//+add kwmoon 080124
int CImageProc::GetMinValue(int nDifferenceB,int nDifferenceG,int nDifferenceR)
{
	int nDifference	   = 0;
	int nMinDifference = 0;
	
	nDifference	   = nDifferenceB <= nDifferenceG ? nDifferenceB : nDifferenceG;
	nMinDifference = nDifference  <= nDifferenceR ? nDifference  : nDifferenceR;

	return nMinDifference;
}

int CImageProc::BrightnessTest(CBaseImage& SnapImage, CBaseImage& RefImage, CBaseImage& MaskImage,
						POINT& ptResultPosition,int& nMaxDifference,CProgressCtrl& Progress)
{
	// If it's not a true color image, return false
	if((SnapImage.m_InfoHeader.biBitCount < 24)
	|| (RefImage.m_InfoHeader.biBitCount  < 24))
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_ABORT;
	}
	
	int nProgress	= 0;
	int nTestResult = TEST_PASS;

	// Dimension of the Reference Image
	int nRefWidth  = RefImage.m_InfoHeader.biWidth;	 
	int nRefHeight = RefImage.m_InfoHeader.biHeight;

	// Dimension of the Snap Image
	int nSnapWidth  = SnapImage.m_InfoHeader.biWidth;
	int nSnapHeight = SnapImage.m_InfoHeader.biHeight;
	int nSnapBitCount = SnapImage.m_InfoHeader.biBitCount;

	int nDifference		= 0;
	int nMaxPassDifference	= 0;
	int nMaxFailDifference	= 0;

	int nGrabRcolor = 0;
	int nGrabGcolor = 0;
	int nGrabBcolor = 0;

	int nRefRcolor = 0;
	int nRefGcolor = 0;
	int nRefBcolor = 0;

	int nGrabYcolor	 = 0;
	int nGrabCbcolor = 0;
	int nGrabCrcolor = 0;

	int nRefYcolor	= 0;
	int nRefCbcolor = 0;
	int nRefCrcolor = 0;

	int nShiftedDiff = 0;
	int nColorMargin = 0;

	int nRefBrightValue  = 0;
	int nSnapBrightValue = 0;

	int nNoShift = 0;

	int nStartXposition = 0;
	int nStartYposition	= 0;
	int nEndXposition   = 0;
	int nEndYposition   = 0;

	int nColorDepth	 = RefImage.m_InfoHeader.biBitCount;
	int nBytesInRow  = WIDTHBYTES(nColorDepth*RefImage.m_InfoHeader.biWidth); 

	BYTE* SnapImageBackup = NULL;
	BYTE* RefImageBackup  = NULL;

	int nImageBufferSize = 0;
	int nImageSize = 0;

	CString szErrMsg = _T("");

	if(SnapImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Snap Image is not loaded!"); return TEST_ABORT;
	}

	if(RefImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Ref Image is not loaded!"); return TEST_ABORT;
	}
	
	if(MaskImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Mask Image is not loaded!"); return TEST_ABORT;
	}
	
	// Compare the size and color depth of images 
	if((nRefWidth != nSnapWidth) || (nRefHeight != nSnapHeight))
	{
		AfxMessageBox("The size of two images is different!"); return TEST_ABORT;
	}

	if(RefImage.m_InfoHeader.biBitCount != SnapImage.m_InfoHeader.biBitCount)
	{
		AfxMessageBox("The color depth of two images is different!"); return TEST_ABORT;
	}

	if((m_nRoiWidth >= nSnapWidth)	|| (m_nRoiHeight >= nSnapHeight))
	{
		szErrMsg.Format("[ROI Error] Roi with & height should be less than witdh & height of image! (W:%d_%d,H:%d_%d)"
					   ,m_nRoiWidth,nSnapWidth,m_nRoiHeight,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if((m_ptRoiPosition.x >= (nSnapWidth-1)) || (m_ptRoiPosition.y >= (nSnapHeight-1))) 
	{
		szErrMsg.Format("[ROI Error] Roi (x,y) should be less than witdh & height of image! (X:%d,Y:%d,W:%d,H:%d)"
					   ,m_ptRoiPosition.x,m_ptRoiPosition.y,nSnapWidth,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}
	
	m_nNoErrorInfo = 0;

	Progress.SetPos(10);

	// Allocate memory (store the processed image into a snapImage object)
	nImageSize = SnapImage.m_InfoHeader.biSizeImage;

	if(SnapImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(SnapImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(SnapImage.m_pProcessedImageData); SnapImage.m_pProcessedImageData = NULL;
			SnapImage.m_pProcessedImageData = (BYTE *)malloc(SnapImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		SnapImage.m_pProcessedImageData = (BYTE *)malloc(SnapImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(SnapImage.m_pProcessedImageData,0,SnapImage.m_BmInfo->bmiHeader.biSizeImage);

	Progress.SetPos(20);

	nColorMargin = m_nBrightMarginLevel;

	if(m_nNoUsedBrightHighBits > 0)
	{
		nNoShift = SnapImage.m_InfoHeader.biBitCount/3 - m_nNoUsedBrightHighBits;
	}

	//+change kwmoon 080618
	CalcTestRange(SnapImage,nStartXposition,nEndXposition,nStartYposition,nEndYposition);

	SnapImage.m_bProcessedImageLoaded = FALSE;

	int nX = 0;
	int nY = 0;
	int nColorChannel = 0;

	int nLastErrorXpos = -1;
	int nLastErrorYpos = -1;
	int nSnapImageIndex = 0;
	int nRefImageIndex  = 0;

	int nMaskValue		= 0;

	if(RefImage.m_InfoHeader.biBitCount==24) // If the color depth of image is 24 bits  
	{
		for(int nRow=nStartYposition; nRow<=nEndYposition; nRow++)
		{
			for(int nColumn=nStartXposition; nColumn<=nEndXposition; nColumn++) 
			{ 
				nRefImageIndex  = (nRow)*nBytesInRow+3*(nColumn); 
				nSnapImageIndex = (nRow-m_ptPositionShift.y)*nBytesInRow+3*(nColumn+m_ptPositionShift.x);
				
				nMaskValue =  MaskImage.m_pImageData[nRefImageIndex] 
							| MaskImage.m_pImageData[nRefImageIndex+1] 
							| MaskImage.m_pImageData[nRefImageIndex+2]; 

				if(nMaskValue == 0)
				{
					nGrabRcolor = SnapImage.m_pImageData[nSnapImageIndex+2];
					nGrabGcolor = SnapImage.m_pImageData[nSnapImageIndex+1];
					nGrabBcolor = SnapImage.m_pImageData[nSnapImageIndex];	

					nRefRcolor = RefImage.m_pImageData[nRefImageIndex+2];
					nRefGcolor = RefImage.m_pImageData[nRefImageIndex+1];
					nRefBcolor = RefImage.m_pImageData[nRefImageIndex];

					nGrabYcolor  = (int)(0.29900*nGrabRcolor + 0.58700*nGrabGcolor + 0.11400*nGrabBcolor);
					nGrabCbcolor = (int)(-0.16874*nGrabRcolor - 0.33126*nGrabGcolor + 0.50000*nGrabBcolor);
					nGrabCrcolor = (int)(0.50000*nGrabRcolor - 0.41869*nGrabGcolor - 0.08131*nGrabBcolor);

					nRefYcolor  = (int)(0.29900*nRefRcolor + 0.58700*nRefGcolor + 0.11400*nRefBcolor);
					nRefCbcolor = (int)(-0.16874*nRefRcolor - 0.33126*nRefGcolor + 0.50000*nRefBcolor);
					nRefCrcolor = (int)(0.50000*nRefRcolor - 0.41869*nRefGcolor - 0.08131*nRefBcolor);
				
					nRefBrightValue  = nRefYcolor;
					nSnapBrightValue = nGrabYcolor;

					if(m_nNoUsedColorHighBits > 0)
					{
						nDifference = (abs)((nRefBrightValue>>nNoShift) - (nSnapBrightValue>>nNoShift)); 
					}
					else
					{
						nDifference = (abs)(nRefBrightValue - nSnapBrightValue); 
					}

					SnapImage.m_pProcessedImageData[nRefImageIndex+2] = nDifference;
					SnapImage.m_pProcessedImageData[nRefImageIndex+1] = nDifference;
					SnapImage.m_pProcessedImageData[nRefImageIndex]   = nDifference;

					if(((m_nNoUsedColorHighBits > 0) && (nDifference > 0))
					||((m_nNoUsedColorHighBits == 0) && (nDifference > nColorMargin)))
					{
						if(m_nNoUsedColorHighBits > 0)
						{
							nDifference = nDifference<<nNoShift;
						}

						nTestResult = TEST_FAIL;

						if((nColumn != nLastErrorXpos) || (nRow != nLastErrorYpos))
						{
							nLastErrorXpos = nColumn;
							nLastErrorYpos = nRow;
						}
						
						if(nDifference > nMaxFailDifference)
						{
							nMaxFailDifference = nDifference;

							// 최대 차이값이 찾아진 좌표를 저장함
							ptResultPosition.x = nColumn;
							ptResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
						}
						
					}
					// 표준 이미지와 색상값의 차이가 없는 경우, 최대 차이값을 찾아 표시함
					else
					{
						if(nDifference > nMaxPassDifference)
						{
							nMaxPassDifference = nDifference;

							// 최대 차이값이 찾아진 좌표를 저장함
							ptResultPosition.x = nColumn;
							ptResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
						}
					}
				}
			}
		}
	}
	else
	{
		return TEST_ABORT;
	}

	SnapImage.m_bProcessedImageLoaded = TRUE;

	Progress.SetPos(100);

	BinaryForMonoImage(NORMAL_BINARY,SnapImage.m_pProcessedImageData,nSnapHeight,nSnapWidth,nSnapBitCount,nColorMargin);

	if(nTestResult == TEST_PASS)
	{
		// Pass인 경우, 정상범위내에서의 최대차이값을 리턴해줌
		nMaxDifference = nMaxPassDifference;
	}
	else
	{
		// Fail인 경우, Fail시의 최대차이값을 리턴해줌 
		nMaxDifference = nMaxFailDifference;
	}

	return nTestResult;
}

int CImageProc::SignalLineTest(int nType,BOOL bUseMaskImage, CProgressCtrl& Progress)
{
	if((m_LineBitTestData.m_pGrabImage->m_InfoHeader.biBitCount < 24)
	|| (m_LineBitTestData.m_pRefImage->m_InfoHeader.biBitCount  < 24))
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_ABORT;
	}
	
	// Dimension of the Reference Image
	int nRefWidth		= m_LineBitTestData.m_pRefImage->m_InfoHeader.biWidth;	 
	int nRefHeight		= m_LineBitTestData.m_pRefImage->m_InfoHeader.biHeight;
	int nRefBitCount	= m_LineBitTestData.m_pRefImage->m_InfoHeader.biBitCount;

	// Dimension of the Grab Image
	int nSnapWidth		= m_LineBitTestData.m_pGrabImage->m_InfoHeader.biWidth;
	int nSnapHeight		= m_LineBitTestData.m_pGrabImage->m_InfoHeader.biHeight;
	int nGrabBitCount	= m_LineBitTestData.m_pGrabImage->m_InfoHeader.biBitCount;

	// Dimension of the Mask Image
	int nMaskWidth		= m_LineBitTestData.m_pMaskImage->m_InfoHeader.biWidth;	 
	int nMaskHeight		= m_LineBitTestData.m_pMaskImage->m_InfoHeader.biHeight;

	int nProgress		= 0;
	int nTestResult		= TEST_PASS;

	int nDifference		= 0;

	int nLowBitPairDifference	= 0;
	int nHighBitPairDifference	= 0;

	int nRefRGBvaule  = 0;
	int nGrabRGBvalue = 0;

	int nNoShift = 0;

	int nStartXposition = 0;
	int nStartYposition	= 0;
	int nEndXposition   = 0;
	int nEndYposition   = 0;

	int nBytesInRow  = WIDTHBYTES(nRefBitCount*nRefWidth); 

	BYTE* GrabImageBackup = NULL;
	BYTE* RefImageBackup  = NULL;

	POINT ptPassPlusResultPosition	= {0,0};
	POINT ptPassMinusResultPosition	= {0,0};

	POINT ptNgPlusResultPosition	= {0,0};
	POINT ptNgMinusResultPosition	= {0,0};

	int	  nPassPlusMaxDifference	= 0;
	int	  nNgPlusMaxDifference		= 0;
	int   nPassMinusMaxDifference	= 0;
	int   nNgMinusMaxDifference		= 0;
	
	int nImageBufferSize = 0;
	int nImageSize = 0;

	CString szErrMsg = _T("");

	if(m_LineBitTestData.m_pGrabImage->m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Grab Image is not loaded!"); return TEST_ABORT;
	}

	if(m_LineBitTestData.m_pRefImage->m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Ref Image is not loaded!"); return TEST_ABORT;
	}
	
	if((nRefWidth != nSnapWidth) || (nRefHeight != nSnapHeight))
	{
		AfxMessageBox("The size of two images is different!"); return TEST_ABORT;
	}

	if(nRefBitCount != nGrabBitCount)
	{
		AfxMessageBox("The color depth of two images is different!"); return TEST_ABORT;
	}
	
	if((m_nRoiWidth >= nSnapWidth)	|| (m_nRoiHeight >= nSnapHeight))
	{
		szErrMsg.Format("[ROI Error] Roi with & height should be less than witdh & height of image! (W:%d_%d,H:%d_%d)"
					   ,m_nRoiWidth,nSnapWidth,m_nRoiHeight,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if((m_ptRoiPosition.x >= (nSnapWidth-1)) || (m_ptRoiPosition.y >= (nSnapHeight-1))) 
	{
		szErrMsg.Format("[ROI Error] Roi (x,y) should be less than witdh & height of image! (X:%d,Y:%d,W:%d,H:%d)"
					   ,m_ptRoiPosition.x,m_ptRoiPosition.y,nSnapWidth,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}
	
	if(m_LineBitTestData.m_pMaskImage->m_bImageLoaded)
	{
		if((nRefWidth != nMaskWidth) || (nRefHeight != nMaskHeight))
		{
			AfxMessageBox("The size of a reference image is different from that of a mask image!"); return TEST_ABORT;
		}
	}

	Progress.SetPos(10);

	nImageSize = m_LineBitTestData.m_pGrabImage->m_InfoHeader.biSizeImage;

	if(m_LineBitTestData.m_pGrabImage->m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(m_LineBitTestData.m_pGrabImage->m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(m_LineBitTestData.m_pGrabImage->m_pProcessedImageData); m_LineBitTestData.m_pGrabImage->m_pProcessedImageData = NULL;
			m_LineBitTestData.m_pGrabImage->m_pProcessedImageData = (BYTE *)malloc(m_LineBitTestData.m_pGrabImage->m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		m_LineBitTestData.m_pGrabImage->m_pProcessedImageData = (BYTE *)malloc(m_LineBitTestData.m_pGrabImage->m_InfoHeader.biSizeImage);
	}
	memset(m_LineBitTestData.m_pGrabImage->m_pProcessedImageData,0,m_LineBitTestData.m_pGrabImage->m_InfoHeader.biSizeImage);

	Progress.SetPos(20);

	//+change kwmoon 080618
	CalcTestRange(*m_LineBitTestData.m_pGrabImage,nStartXposition,nEndXposition,nStartYposition,nEndYposition);

	m_LineBitTestData.m_pGrabImage->m_bProcessedImageLoaded = FALSE;

	int nX = 0;
	int nY = 0;
	int nColorChannel = 0;

	int nLastErrorXpos = -1;
	int nLastErrorYpos = -1;
	int nGrabImageIndex = 0;
	int nRefImageIndex  = 0;

	int nMaskValue		= 0;
	
	BOOL bInvalidMask	= TRUE;
	
	int nRefBitValue	= 0;
	int nGrabBitValue	= 0;
	float fDifferencePercentage = 0;

	int nNoTestdPixels	= 0;

	if(m_LineBitTestData.m_pRefImage->m_InfoHeader.biBitCount==24) 
	{
		for(int nRow=nStartYposition; nRow<=nEndYposition; nRow++)
		{
			for(int nColumn=nStartXposition; nColumn<=nEndXposition; nColumn++) 
			{ 
				nRefImageIndex  = (nRow)*nBytesInRow+3*(nColumn); 
				nGrabImageIndex = (nRow-m_ptPositionShift.y)*nBytesInRow+3*(nColumn+m_ptPositionShift.x);

				if(m_LineBitTestData.m_pMaskImage->m_bImageLoaded && bUseMaskImage)
				{
					nMaskValue =  m_LineBitTestData.m_pMaskImage->m_pImageData[nRefImageIndex] 
								| m_LineBitTestData.m_pMaskImage->m_pImageData[nRefImageIndex+1] 
								| m_LineBitTestData.m_pMaskImage->m_pImageData[nRefImageIndex+2]; 
				}
				
				m_LineBitTestData.m_nTotalNoUnmaskedPixel++;

				if(nMaskValue == 0)
				{
					bInvalidMask = FALSE;

					nNoTestdPixels++;

					m_LineBitTestData.m_nTotalNoMaskedPixel++;

					for(int j=0; j<3; j++) // R,G,B
					{
						nRefRGBvaule  = m_LineBitTestData.m_pRefImage->m_pImageData[nRefImageIndex+j];
						nGrabRGBvalue = m_LineBitTestData.m_pGrabImage->m_pImageData[nGrabImageIndex+j];

						//+add kwmoon 070817
						nRefBitValue  = 0;
						nGrabBitValue = 0;

						for(int k=m_LineBitTestData.m_nNoSkippedSignalLineLowBits; k<MAX_NO_COLOR_BITS; k++)
						{
							nRefBitValue = ((((nRefRGBvaule >> k) & 0x01) == 1) ? 1 : 0);
							if(nRefBitValue) m_LineBitTestData.m_aRefNoHighBit[j][k]++;
			
							nGrabBitValue = ((((nGrabRGBvalue >> k) & 0x01) == 1) ? 1 : 0);
							if(nGrabBitValue) m_LineBitTestData.m_aGrabNoHighBit[j][k]++;

						}
					}
				}
			}
		}
	}
	else
	{
		return TEST_ABORT;
	}

	if(bInvalidMask)
	{
		CString szMsg;

		if(nType == GRAB_CHECK_TEST)
		{
			szMsg.Format("[Grab_Image_Check] No test was performed because of a mask image values. check mask image!");
		}
		else 
		{
			szMsg.Format("[Color Test] No test was performed because of a mask image values. check mask image!");
		}

		AfxMessageBox(szMsg); return TEST_ABORT;
	}

	Progress.SetPos(100);

	m_LineBitTestData.m_nBitCount = nRefBitCount;

	nTestResult = TEST_PASS; 

	for(int j=0; j<3; j++) // R,G,B
	{
		for(int k=m_LineBitTestData.m_nNoSkippedSignalLineLowBits; k<(nRefBitCount/3); k++)
		{
			if((m_LineBitTestData.m_aGrabNoHighBit[j][k] == 0)
			|| (m_LineBitTestData.m_aGrabNoHighBit[j][k] == nNoTestdPixels))
			{
				m_LineBitTestData.m_nErrorColor = j;
				m_LineBitTestData.m_nErrorBitPosition = k;

				nTestResult = TEST_FAIL; break;
			}
		}
		if(nTestResult == TEST_FAIL) break;
	}
	return nTestResult;
}

#define ZOOM_OUT_50		0.5f

int CImageProc::NoiseTest(int nType, CBaseImage& SnapImage, CBaseImage& RefImage, CBaseImage& MaskImage,
						POINT& ptPlusResultPosition,POINT& ptMinusResultPosition, 
						int& nPlusMaxDifference,int& nMinusMaxDifference,int& nTotalNoErrorPixel,BOOL bUseMaskImage,CProgressCtrl& Progress)
{
	// If it's not a true color image, return false
	if((SnapImage.m_InfoHeader.biBitCount < 24)
	|| (RefImage.m_InfoHeader.biBitCount  < 24))
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_ABORT;
	}
	
	int nProgress	= 0;
	int nTestResult = TEST_PASS;

	// Dimension of the Reference Image
	int nRefWidth  = RefImage.m_InfoHeader.biWidth;	 
	int nRefHeight = RefImage.m_InfoHeader.biHeight;

	int nMaskWidth  = 0;	 
	int nMaskHeight = 0;

	if(MaskImage.m_bImageLoaded)
	{
		nMaskWidth  = MaskImage.m_InfoHeader.biWidth;	 
		nMaskHeight = MaskImage.m_InfoHeader.biHeight;
	}

	// Dimension of the Snap Image
	int nSnapWidth  = SnapImage.m_InfoHeader.biWidth;
	int nSnapHeight = SnapImage.m_InfoHeader.biHeight;
	int nSnapBitCount = SnapImage.m_InfoHeader.biBitCount;

	int nDifference	= 0;

	int nGrabRcolor = 0;
	int nGrabGcolor = 0;
	int nGrabBcolor = 0;

	int nRefRcolor	= 0;
	int nRefGcolor	= 0;
	int nRefBcolor	= 0;

	int nShiftedDiff = 0;
	int nColorMargin = 0;

	int nLowerLimit	= 0;
	int nUpperLimit = 0;

	int nRefRGBvaule  = 0;
	int nSnapRGBvalue = 0;

	int nNoShift = 0;

	int nStartXposition = 0;
	int nStartYposition	= 0;
	int nEndXposition   = 0;
	int nEndYposition   = 0;

	int nColorDepth	 = RefImage.m_InfoHeader.biBitCount;
	int nBytesInRow  = WIDTHBYTES(nColorDepth*RefImage.m_InfoHeader.biWidth); 

	//+change kwmoon 080722
//	int nNoLevel	 = (int)pow(2,(nColorDepth/3));
	int nNoLevel	 = (int)pow(2,(nColorDepth/3))-1;

	BYTE* SnapImageBackup = NULL;
	BYTE* RefImageBackup  = NULL;

	POINT ptPassPlusResultPosition	= {0,0};
	POINT ptPassMinusResultPosition	= {0,0};

	POINT ptNgPlusResultPosition	= {0,0};
	POINT ptNgMinusResultPosition	= {0,0};

	int	  nPassPlusMaxDifference	= 0;
	int	  nNgPlusMaxDifference		= 0;
	int   nPassMinusMaxDifference	= 0;
	int   nNgMinusMaxDifference		= 0;

	int nImageBufferSize = 0;
	int nImageSize = 0;

	if((m_nRoiHeight > 0) && (m_nRoiWidth > 0))
	{
		m_nNoRoiPixel = m_nRoiWidth * m_nRoiHeight;
	}
	else
	{
		m_nNoRoiPixel = nRefWidth * nRefHeight;
	}
	//-

	DWORD dwFuncTickCount     = 0;
	DWORD dwFuncElapsedTime   = 0;
	CString szFuncElapsedTime = _T("");

	
	CString szErrMsg = _T("");

	if(SnapImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Snap Image is not loaded!"); return TEST_ABORT;
	}

	if(RefImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Ref Image is not loaded!"); return TEST_ABORT;
	}
	
	if((nRefWidth != nSnapWidth) || (nRefHeight != nSnapHeight))
	{
		szErrMsg.Format("[Image Size Error] Grab(W:%d,H%d), Ref(W:%d,H%d)",
						nSnapWidth,nSnapHeight,nRefWidth,nRefHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}
	
	if(RefImage.m_InfoHeader.biBitCount != SnapImage.m_InfoHeader.biBitCount)
	{
		AfxMessageBox("The color depth of two images is different!"); return TEST_ABORT;
	}
	
	if((m_nRoiWidth >= nSnapWidth)	|| (m_nRoiHeight >= nSnapHeight))
	{
		szErrMsg.Format("[ROI Error] Roi with & height should be less than witdh & height of image! (W:%d_%d,H:%d_%d)"
					   ,m_nRoiWidth,nSnapWidth,m_nRoiHeight,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if((m_ptRoiPosition.x >= (nSnapWidth-1)) || (m_ptRoiPosition.y >= (nSnapHeight-1))) 
	{
		szErrMsg.Format("[ROI Error] Roi (x,y) should be less than witdh & height of image! (X:%d,Y:%d,W:%d,H:%d)"
					   ,m_ptRoiPosition.x,m_ptRoiPosition.y,nSnapWidth,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if(MaskImage.m_bImageLoaded)
	{
		if((nRefWidth != nMaskWidth) || (nRefHeight != nMaskHeight))
		{
			szErrMsg.Format("[Image Size Error] Ref(W:%d,H%d), Mask(W:%d,H%d)",
							nRefWidth,nRefHeight,nMaskWidth,nMaskHeight);

			AfxMessageBox(szErrMsg); return TEST_ABORT;
		}
	}

	// Averaging Process
	int nAvgMaskWidth  = 3;
	int nAvgMaskHeight = 3;

	//+change 080129
	if(nType == OSD_IMAGE_TEST)
	{
		if(Average(nAvgMaskWidth,nAvgMaskHeight,nStartXposition,nEndXposition,
			       nStartYposition,nEndYposition,SnapImage,TRUE) == TEST_FAIL)	
		{
			AfxMessageBox("Failed to run average-filter");
			return TEST_ABORT;	
		}

		if(Average(nAvgMaskWidth,nAvgMaskHeight,nStartXposition,nEndXposition,
			       nStartYposition,nEndYposition,RefImage,TRUE) == TEST_FAIL)	
		{
			AfxMessageBox("Failed to run average-filter");
			return TEST_ABORT;	
		}
	}
	else
	{
		if(Average(nAvgMaskWidth,nAvgMaskHeight,nStartXposition,nEndXposition,
			       nStartYposition,nEndYposition,SnapImage,TRUE) == TEST_FAIL)
		{
			AfxMessageBox("Failed to run average-filter");
			return TEST_ABORT;	
		}
	}

	m_nNoErrorInfo		= 0;
	nTotalNoErrorPixel	= 0;

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++) m_aNoDifferentPoint[i][j] = 0;
	}
	m_nSecondDifferentPointFreePercent = -1;

	//+add kwmoon 080715
	for(int i=0; i<5;i++)
	{
		m_nNoBandErrorPixel[i]			= 0;
		m_nNoAllowedBandErrorPixel[i]	= 0;

		m_fBandErrorPixelPercentage[i]	= 0.0f;
	}

	Progress.SetPos(10);

	SnapImage.m_bImageLoaded = FALSE;
	RefImage.m_bImageLoaded  = FALSE;


	//+change kwmoon 080130 : 0.4f -> 0.5f
	ZoomOut(SnapImage,ZOOM_OUT_50);


	ZoomOut(RefImage, ZOOM_OUT_50);
	
	if(MaskImage.m_bImageLoaded)
	{
		ZoomOut(MaskImage,ZOOM_OUT_50);
	}

	//+add kwmoon 080130
	m_ptRoiPosition.x = (int)(m_ptRoiPosition.x * ZOOM_OUT_50);
	m_ptRoiPosition.y = (int)(m_ptRoiPosition.y * ZOOM_OUT_50);
	m_nRoiWidth		  = (int)(m_nRoiWidth  * ZOOM_OUT_50);
	m_nRoiHeight	  = (int)(m_nRoiHeight * ZOOM_OUT_50);
	//-
	
	// Dimension of the Reference Image
	nRefWidth  = RefImage.m_InfoHeader.biWidth;	 
	nRefHeight = RefImage.m_InfoHeader.biHeight;

	if(MaskImage.m_bImageLoaded)
	{
		nMaskWidth  = MaskImage.m_InfoHeader.biWidth;	 
		nMaskHeight = MaskImage.m_InfoHeader.biHeight;
	}

	// Dimension of the Snap Image
	nSnapWidth  = SnapImage.m_InfoHeader.biWidth;
	nSnapHeight = SnapImage.m_InfoHeader.biHeight;
	nSnapBitCount = SnapImage.m_InfoHeader.biBitCount;

	nColorDepth	 = RefImage.m_InfoHeader.biBitCount;
	nBytesInRow  = WIDTHBYTES(nColorDepth*RefImage.m_InfoHeader.biWidth); 

	nImageSize = SnapImage.m_InfoHeader.biSizeImage;

	if(SnapImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(SnapImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(SnapImage.m_pProcessedImageData); SnapImage.m_pProcessedImageData = NULL;
			SnapImage.m_pProcessedImageData = (BYTE *)malloc(SnapImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		SnapImage.m_pProcessedImageData = (BYTE *)malloc(SnapImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(SnapImage.m_pProcessedImageData,0,SnapImage.m_BmInfo->bmiHeader.biSizeImage);

	// Allocate memory (store the processed image)
	BYTE* pProcessedImageData = (BYTE *)malloc(SnapImage.m_InfoHeader.biSizeImage); 
	memset(pProcessedImageData,0,SnapImage.m_InfoHeader.biSizeImage);

	Progress.SetPos(20);

	nLowerLimit = Percentage2Level(m_fLowerLimit,nNoLevel);
	nUpperLimit = Percentage2Level(m_fUpperLimit,nNoLevel);

	if(m_nNoUsedColorHighBits > 0)
	{
		nNoShift = SnapImage.m_InfoHeader.biBitCount/3 - m_nNoUsedColorHighBits;
	}

	//+change kwmoon 080618
	CalcTestRange(SnapImage,nStartXposition,nEndXposition,nStartYposition,nEndYposition);

	SnapImage.m_bProcessedImageLoaded = FALSE;

	int nX = 0;
	int nY = 0;
	int nColorChannel = 0;

	int nLastErrorXpos = -1;
	int nLastErrorYpos = -1;
	int nSnapImageIndex = 0;
	int nRefImageIndex  = 0;

	int	nBlockIndex		= 0;
	int nMaskValue		= 0;

	BOOL bInvalidMask	= TRUE;

	//+add kwmoon 080124
	int nDifferenceR	= 0;
	int nDifferenceG	= 0;
	int nDifferenceB	= 0;
	int nMaxDifference	= 0;
	int nMinDifference	= 0;

	//+add kwmoon 080716
	m_nNoTargetPixel = 0;

	if(RefImage.m_InfoHeader.biBitCount==24) // If the color depth of image is 24 bits  
	{
		for(int nRow=nStartYposition; nRow<=nEndYposition; nRow++)
		{
			for(int nColumn=nStartXposition; nColumn<=nEndXposition; nColumn++) 
			{ 
				nRefImageIndex  = (nRow)*nBytesInRow+3*(nColumn); 
				nSnapImageIndex = (nRow-m_ptPositionShift.y)*nBytesInRow+3*(nColumn+m_ptPositionShift.x);

				if(MaskImage.m_bImageLoaded && bUseMaskImage)
				{
					nMaskValue =  MaskImage.m_pImageData[nRefImageIndex] 
								| MaskImage.m_pImageData[nRefImageIndex+1] 
								| MaskImage.m_pImageData[nRefImageIndex+2]; 
				}

				if(nMaskValue == 0)
				{
					//+add kwmoon 080716
					m_nNoTargetPixel++;

					bInvalidMask = FALSE;

				//	for(int j=0; j<3; j++) // R,G,B
					{
					//	nRefRGBvaule  = RefImage.m_pImageData[nRefImageIndex+j];
					//	nSnapRGBvalue = SnapImage.m_pImageData[nSnapImageIndex+j];

						// B
						nRefBcolor  = RefImage.m_pImageData[nRefImageIndex];
						nGrabBcolor = SnapImage.m_pImageData[nSnapImageIndex];

						// G
						nRefGcolor  = RefImage.m_pImageData[nRefImageIndex+1];
						nGrabGcolor = SnapImage.m_pImageData[nSnapImageIndex+1];
						
						// R
						nRefRcolor  = RefImage.m_pImageData[nRefImageIndex+2];
						nGrabRcolor = SnapImage.m_pImageData[nSnapImageIndex+2];

						//	if(m_nNoUsedColorHighBits > 0)
					//	{
					//		nDifference = ((nSnapRGBvalue>>nNoShift) - (nRefRGBvaule>>nNoShift)); 
					//	}
					//	else
					//	{
					//		nDifference = (nSnapRGBvalue - nRefRGBvaule); 
					//	}

						if(m_nNoUsedColorHighBits > 0)
						{
							nDifferenceB = ((nGrabBcolor>>nNoShift) - (nRefBcolor>>nNoShift)); 
							nDifferenceG = ((nGrabGcolor>>nNoShift) - (nRefGcolor>>nNoShift)); 
							nDifferenceR = ((nGrabRcolor>>nNoShift) - (nRefRcolor>>nNoShift)); 
						}
						else
						{
							nDifferenceB = (nGrabBcolor - nRefBcolor); 
							nDifferenceG = (nGrabGcolor - nRefGcolor); 
							nDifferenceR = (nGrabRcolor - nRefRcolor); 
						}

					//	pProcessedImageData[nRefImageIndex+j] = abs(nDifference);
						pProcessedImageData[nRefImageIndex]	  = abs(nDifferenceB);
						pProcessedImageData[nRefImageIndex+1] = abs(nDifferenceG);
						pProcessedImageData[nRefImageIndex+2] = abs(nDifferenceR);

						nMaxDifference = GetMaxValue(nDifferenceB,nDifferenceG,nDifferenceR); 
						nMinDifference = GetMinValue(nDifferenceB,nDifferenceG,nDifferenceR); 

						nDifference	   = abs(nMaxDifference) >= abs(nMinDifference) ? nMaxDifference : nMinDifference; 
						
						// Error Pixel
						if(((m_nNoUsedColorHighBits > 0) && (abs(nDifference) > 0))
					//	|| ((m_nNoUsedColorHighBits == 0) && (nDifference > nUpperLimit))
					//	|| ((m_nNoUsedColorHighBits == 0) && (nDifference < nLowerLimit)))
						|| ((m_nNoUsedColorHighBits == 0) && (nMaxDifference > nUpperLimit))
						|| ((m_nNoUsedColorHighBits == 0) && (nMinDifference < nLowerLimit)))
						{
							// Spec-Out : White
						//	pProcessedImageData[nRefImageIndex]	  = 255;
						//	pProcessedImageData[nRefImageIndex+1] = 255;
						//	pProcessedImageData[nRefImageIndex+2] = 255;
							
							if(m_nNoUsedColorHighBits > 0)
							{
								nDifference = nDifference<<nNoShift;
							}

							if((nColumn != nLastErrorXpos) || (nRow != nLastErrorYpos))
							{
								nTotalNoErrorPixel++;

								float fDifferenePercentage = 0;

								if(nDifference >= 0) // nDifference > nUpperLimit
								{ 
									fDifferenePercentage = Level2Percentage(abs(nDifference-nUpperLimit),nNoLevel);

									if(fDifferenePercentage  <= 2) m_nNoBandErrorPixel[0]++;
									if((fDifferenePercentage >  2) && (fDifferenePercentage <= 4)) m_nNoBandErrorPixel[1]++;
									if((fDifferenePercentage >  4) && (fDifferenePercentage <= 6)) m_nNoBandErrorPixel[2]++;
									if((fDifferenePercentage >  6) && (fDifferenePercentage <= 8)) m_nNoBandErrorPixel[3]++;
									if(fDifferenePercentage  >  8) m_nNoBandErrorPixel[4]++;
								}
								else // nDifference < nLowerLimit
								{
									fDifferenePercentage = Level2Percentage(abs(nDifference-nLowerLimit),nNoLevel);

									if(fDifferenePercentage  <= 2) m_nNoBandErrorPixel[0]++;
									if((fDifferenePercentage >  2) && (fDifferenePercentage <= 4)) m_nNoBandErrorPixel[1]++;
									if((fDifferenePercentage >  4) && (fDifferenePercentage <= 6)) m_nNoBandErrorPixel[2]++;
									if((fDifferenePercentage >  6) && (fDifferenePercentage <= 8)) m_nNoBandErrorPixel[3]++;
									if(fDifferenePercentage  >  8) m_nNoBandErrorPixel[4]++;
								}

								nLastErrorXpos = nColumn;
								nLastErrorYpos = nRow;
							}


							if((nDifference <0) && (nDifference < nNgMinusMaxDifference))
							{
								nNgMinusMaxDifference = nDifference;

								ptNgMinusResultPosition.x = nColumn;
								ptNgMinusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);

								if((ptNgMinusResultPosition.x == 340) && (ptNgMinusResultPosition.y == 168))
								{
									int nDebug = 1;
								}
							}

							if((nDifference >=0) && (nDifference >= nNgPlusMaxDifference))
							{
								nNgPlusMaxDifference = nDifference;
				
								ptNgPlusResultPosition.x = nColumn;
								ptNgPlusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
							}
						}
						// Good Pixel
						else
						{
							//+add kwmoon 080124
							// Spec-In : Black
						//	pProcessedImageData[nRefImageIndex]	  = 0;
						//	pProcessedImageData[nRefImageIndex+1] = 0;
						//	pProcessedImageData[nRefImageIndex+2] = 0;


							//+add kwmoon 080716
							m_nNoGoodPixel++;

							if((nDifference <0) && (nDifference < nPassMinusMaxDifference))
							{
								nPassMinusMaxDifference = nDifference;

								ptPassMinusResultPosition.x = nColumn;
								ptPassMinusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
							}

							if((nDifference >=0) && (nDifference >= nPassPlusMaxDifference))
							{
								nPassPlusMaxDifference = nDifference;

								ptPassPlusResultPosition.x = nColumn;
								ptPassPlusResultPosition.y = (RefImage.m_InfoHeader.biHeight-nRow-1);
							}
						}

						//+change kwmoon 080124
					//	nBlockIndex = (int)floor((float)abs(nDifference)*100/(float)nNoLevel);
					//	m_aNoDifferentPoint[j][nBlockIndex]++;

						//+change kwmoon 080716
					//	if(CurrentSet->bSaveDistributionTable || g_pView->m_bAdjSpecMode)
						if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData || g_pView->m_bAdjSpecMode)
						{
							nBlockIndex = (int)((float)abs(nDifferenceB)*100/(float)nNoLevel);
							m_aNoDifferentPoint[0][nBlockIndex]++;

							nBlockIndex = (int)((float)abs(nDifferenceG)*100/(float)nNoLevel);
							m_aNoDifferentPoint[1][nBlockIndex]++;

							nBlockIndex = (int)((float)abs(nDifferenceR)*100/(float)nNoLevel);
							m_aNoDifferentPoint[2][nBlockIndex]++;
						}
					}
				}
			}
		}
	}
	else
	{
		//+add kwmoon 080201
		CString szMsg;
		szMsg.Format("Reference image data may be corrupted. Color depth is not 24bits.(%d)bits",RefImage.m_InfoHeader.biBitCount);
		AfxMessageBox(szMsg);
		//-
		return TEST_ABORT;
	}

	int nDifferentPointFreePercentNo = 0;

	//+change kwmoon 080716
//	if(CurrentSet->bSaveDistributionTable || g_pView->m_bAdjSpecMode)
	if(CurrentSet->bSaveReviewData || CurrentSet->bSaveDetailResultData  || g_pView->m_bAdjSpecMode)
	{
		for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++)
		{
			if((m_aNoDifferentPoint[0][j] == 0)
			&& (m_aNoDifferentPoint[1][j] == 0)
			&& (m_aNoDifferentPoint[2][j] == 0))
			{
				nDifferentPointFreePercentNo++;

				if(nDifferentPointFreePercentNo == 2)
				{
					m_nSecondDifferentPointFreePercent = j; break;
				}
			}
		}
	}

	if(bInvalidMask)
	{
		CString szMsg;

		if(nType == GRAB_CHECK_TEST)
		{
			szMsg.Format("[Grab_Image_Check] No test was performed because of a mask image values. check mask image!");
		}
		else 
		{
			szMsg.Format("[Color Test] No test was performed because of a mask image values. check mask image!");
		}

		AfxMessageBox(szMsg); return TEST_ABORT;
	}

	SnapImage.m_bProcessedImageLoaded = TRUE;

	Progress.SetPos(100);

	//+change kwmoon 080124
//	for(j=0; j<(int)SnapImage.m_InfoHeader.biSizeImage; j++)
//	{
//		SnapImage.m_pProcessedImageData[j] = pProcessedImageData[j];
//	}
	memcpy((BYTE*)SnapImage.m_pProcessedImageData,(BYTE*)pProcessedImageData,(int)SnapImage.m_InfoHeader.biSizeImage);

	if(pProcessedImageData != NULL)
	{
		free(pProcessedImageData); pProcessedImageData = NULL;
	}

	//+change kwmoon 071112
//	BinaryForDiffImage(NORMAL_BINARY,SnapImage,RefImage,nLowerLimit,nUpperLimit);
	BinaryForDiffImage(NORMAL_BINARY,SnapImage,nLowerLimit,nUpperLimit);

	//+add kwmoon 080716
	for(int i=0; i<10; i++)
	{
		if(m_nNoBandErrorPixel[i] == 0)
		{
			m_fBandErrorPixelPercentage[i] = 0;
		}
		else
		{
			m_fBandErrorPixelPercentage[i] = (float)m_nNoBandErrorPixel[i] * 100 / (float)m_nNoTargetPixel;
		}
		//+change kwmoon 080804
	//	m_nNoAllowedBandErrorPixel[i]  = (int)((float)m_fAllowedBandErrorPixelPercentage[i] * (float)m_nNoTargetPixel);
		m_nNoAllowedBandErrorPixel[i]  = (int)((float)m_fAllowedBandErrorPixelPercentage[i] * 0.01 * (float)m_nNoTargetPixel);
	}
	m_fGoodPixelPercentage =(float)m_nNoGoodPixel * 100 / (float)m_nNoTargetPixel;

	if(nTotalNoErrorPixel == 0)
	{
		nTestResult = TEST_PASS;
		nPlusMaxDifference	= nPassPlusMaxDifference;
		nMinusMaxDifference = nPassMinusMaxDifference;
		
		ptPlusResultPosition	= ptPassPlusResultPosition;
		ptMinusResultPosition	= ptPassMinusResultPosition;
	}
	else
	{
		//+del kwmoon 080716
	/*
		if(nType == OSD_IMAGE_TEST)
		{
			if(nTotalNoErrorPixel > m_fAllowedErrorPixelPercentageForOsdTest)
			{
				nTestResult = TEST_FAIL;
				nPlusMaxDifference	= nNgPlusMaxDifference;
				nMinusMaxDifference = nNgMinusMaxDifference;

				ptPlusResultPosition	= ptNgPlusResultPosition;
				ptMinusResultPosition	= ptNgMinusResultPosition;
			}
			else
			{
				nTestResult = TEST_PASS;
				nPlusMaxDifference	= nPassPlusMaxDifference;
				nMinusMaxDifference = nPassMinusMaxDifference;

				ptPlusResultPosition	= ptPassPlusResultPosition;
				ptMinusResultPosition	= ptPassMinusResultPosition;
			}
		}
	*/
	//	else
		{	

			//+change kwmoon 080716
			if((m_fBandErrorPixelPercentage[0] < CurrentSet->fAllowedBandErrorPixelPercentage[0])
			&& (m_fBandErrorPixelPercentage[1] < CurrentSet->fAllowedBandErrorPixelPercentage[1])
			&& (m_fBandErrorPixelPercentage[2] < CurrentSet->fAllowedBandErrorPixelPercentage[2])
			&& (m_fBandErrorPixelPercentage[3] < CurrentSet->fAllowedBandErrorPixelPercentage[3])
			&& (m_fBandErrorPixelPercentage[4] < CurrentSet->fAllowedBandErrorPixelPercentage[4]))
			{
				nTestResult = TEST_PASS;
				nPlusMaxDifference	= nPassPlusMaxDifference;
				nMinusMaxDifference = nPassMinusMaxDifference;

				ptPlusResultPosition	= ptPassPlusResultPosition;
				ptMinusResultPosition	= ptPassMinusResultPosition;
			}
			else
			{
				nTestResult = TEST_FAIL;
				nPlusMaxDifference	= nNgPlusMaxDifference;
				nMinusMaxDifference = nNgMinusMaxDifference;

				ptPlusResultPosition	= ptNgPlusResultPosition;
				ptMinusResultPosition	= ptNgMinusResultPosition;
			}
			//+del kwmoon 080716
		/*	if(nTotalNoErrorPixel > m_fAllowedErrorPixelPercentage)
			{
				nTestResult = TEST_FAIL;
				nPlusMaxDifference	= nNgPlusMaxDifference;
				nMinusMaxDifference = nNgMinusMaxDifference;

				ptPlusResultPosition	= ptNgPlusResultPosition;
				ptMinusResultPosition	= ptNgMinusResultPosition;
			}
			else
			{
				nTestResult = TEST_PASS;
				nPlusMaxDifference	= nPassPlusMaxDifference;
				nMinusMaxDifference = nPassMinusMaxDifference;

				ptPlusResultPosition	= ptPassPlusResultPosition;
				ptMinusResultPosition	= ptPassMinusResultPosition;
			}
		*/
		}
	}

	SnapImage.m_bImageLoaded = TRUE;
	RefImage.m_bImageLoaded  = TRUE;

	return nTestResult;
}

void CImageProc::SetAvgMaskSize(int nMaskWidth, int nMaskHeight,int nNoRepeat)
{
	m_nAvgMaskWidth  = nMaskWidth;
	m_nAvgMaskHeight = nMaskHeight;
	m_nNoRepeat	  = nNoRepeat;
}

//+change kwmoon 080820
//int CImageProc::Average(int nMaskWidth, int nMaskHeight, CBaseImage& DstImage,BOOL bOverwrite)
int CImageProc::Average(int nMaskWidth, int nMaskHeight, CBaseImage& DstImage)
{
	if(nMaskWidth > MAX_MASK_SIZE) 
	{
		AfxMessageBox("Max width of average-mask is 9!"); return TEST_FAIL;
	}

	if(nMaskHeight > MAX_MASK_SIZE) 
	{
		AfxMessageBox("Max height of average-mask is 9!"); return TEST_FAIL;
	}

	if(((nMaskWidth % 2) != 1) || ((nMaskHeight % 2) != 1)) // Mask size should be an odd number!
	{
		AfxMessageBox("Mask size should be an odd number!"); return TEST_FAIL;
	}

	int nNewRValue = 0;
	int nNewGValue = 0;
	int nNewBValue = 0;

	int nXmargin = nMaskWidth/2;
	int nYmargin = nMaskHeight/2;

	int nMaskBox[MAX_MASK_SIZE][MAX_MASK_SIZE];

	int nBytesInRow= WIDTHBYTES(DstImage.m_InfoHeader.biBitCount*DstImage.m_InfoHeader.biWidth); 

	int nImageBufferSize = 0;
	int nImageSize = 0;

	for(int nX=0; nX<MAX_MASK_SIZE; nX++)
	{
		for(int nY=0; nY<MAX_MASK_SIZE; nY++)
		{
			nMaskBox[nX][nY] = 1;			
		}
	}

	nImageSize = DstImage.m_InfoHeader.biSizeImage;

	if(DstImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pProcessedImageData); DstImage.m_pProcessedImageData = NULL;
			DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(DstImage.m_pProcessedImageData,0,DstImage.m_BmInfo->bmiHeader.biSizeImage);

	for(int j=0; j<(int)DstImage.m_InfoHeader.biSizeImage; j++)
	{
		DstImage.m_pProcessedImageData[j] = DstImage.m_pImageData[j];
	}
	
	DstImage.m_bProcessedImageLoaded = FALSE;

	if(DstImage.m_InfoHeader.biBitCount==24) // If the color depth of image is 24 bits  
	{
		for(int nRow=nYmargin; nRow<DstImage.m_InfoHeader.biHeight-nYmargin; nRow++)
		{
			for(int nColumn=nXmargin; nColumn<DstImage.m_InfoHeader.biWidth-nXmargin; nColumn++) 
			{ 
				nNewRValue = 0;
				nNewGValue = 0;
				nNewBValue = 0;
				
				for(int nX=0; nX<nMaskWidth; nX++)
				{
					for(int nY=0; nY<nMaskHeight; nY++)
					{
						nNewBValue += nMaskBox[nX][nY] * DstImage.m_pImageData[(nRow+nY-nYmargin)*nBytesInRow+3*(nColumn+nX-nXmargin)];
						nNewGValue += nMaskBox[nX][nY] * DstImage.m_pImageData[(nRow+nY-nYmargin)*nBytesInRow+3*(nColumn+nX-nXmargin)+1];
						nNewRValue += nMaskBox[nX][nY] * DstImage.m_pImageData[(nRow+nY-nYmargin)*nBytesInRow+3*(nColumn+nX-nXmargin)+2];
					}
				}
			
				nNewRValue /= nMaskWidth*nMaskHeight;
				nNewGValue /= nMaskWidth*nMaskHeight;
				nNewBValue /= nMaskWidth*nMaskHeight;
				
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn]   = (BYTE)nNewBValue;//BYTE값으로 변환 
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+1] = (BYTE)nNewGValue;//BYTE값으로 변환 
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2] = (BYTE)nNewRValue;//BYTE값으로 변환
			}
		}
	}

	DstImage.m_bProcessedImageLoaded = TRUE;

	//+del kwmoon 080820
//	if(bOverwrite)
	{
		for(int j=0; j<(int)DstImage.m_InfoHeader.biSizeImage; j++)
		{
			DstImage.m_pImageData[j] = DstImage.m_pProcessedImageData[j];
		}
	}

	return TEST_PASS;
}

/*-----------------------------------------------------------------------------------------
 함수명 : Average

 기  능 : 
     - 파라미터로 넘어온 이미지에 대하여 주어진 Mask 사이즈에 따라 Average Filter를 적용함
 
  입력파라미터 :
     - int nMaskWidth		: Average Filter의 Mask의 Width
	 - int nMaskHeight		: Average Filter의 Mask의 Height
	 - int nStartXposition	: Average Filter를 적용할 영역의 X1 좌표
	 - int nEndXposition	: Average Filter를 적용할 영역의 Y1 좌표
	 - int nStartYposition	: Average Filter를 적용할 영역의 X2 좌표
	 - int nEndYposition	: Average Filter를 적용할 영역의 Y2 좌표
	 - CBaseImage& DstImage : Average Filter를 적용한 이미지가 저장되어 있는 Object
     - BOOL bOverwrite      : Average Filter를 적용한 결과를 입력 이미지 Object에 OverWrite할가의 여부
	 
  Return 값 :
     - TEST_PASS 또는 TEST_FAIL 

-----------------------------------------------------------------------------------------*/
int CImageProc::Average(int nMaskWidth, int nMaskHeight, int nStartXposition, int nEndXposition,
						int nStartYposition, int nEndYposition, CBaseImage& DstImage,BOOL bOverwrite)
{
	if(nMaskWidth > MAX_MASK_SIZE) 
	{
		AfxMessageBox("Max width of average-mask is 9!"); return TEST_FAIL;
	}

	if(nMaskHeight > MAX_MASK_SIZE) 
	{
		AfxMessageBox("Max height of average-mask is 9!"); return TEST_FAIL;
	}

	if(((nMaskWidth % 2) != 1) || ((nMaskHeight % 2) != 1)) 
	{
		AfxMessageBox("Mask size should be an odd number!"); return TEST_FAIL;
	}

	int nNewRValue = 0;
	int nNewGValue = 0;
	int nNewBValue = 0;

	int nXmargin = nMaskWidth/2;
	int nYmargin = nMaskHeight/2;

	int nMaskBox[MAX_MASK_SIZE][MAX_MASK_SIZE];

	int nBytesInRow= WIDTHBYTES(DstImage.m_InfoHeader.biBitCount*DstImage.m_InfoHeader.biWidth); 

	int nImageBufferSize = 0;
	int nImageSize = 0;

	for(int nX=0; nX<MAX_MASK_SIZE; nX++)
	{
		for(int nY=0; nY<MAX_MASK_SIZE; nY++)
		{
			nMaskBox[nX][nY] = 1;			
		}
	}

	nImageSize = DstImage.m_InfoHeader.biSizeImage;

	if(DstImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pProcessedImageData); DstImage.m_pProcessedImageData = NULL;
			DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(DstImage.m_pProcessedImageData,0,DstImage.m_BmInfo->bmiHeader.biSizeImage);

	for(int j=0; j<(int)DstImage.m_InfoHeader.biSizeImage; j++)
	{
		DstImage.m_pProcessedImageData[j] = DstImage.m_pImageData[j];
	}
	
	DstImage.m_bProcessedImageLoaded = FALSE;

	//+add kwmoon 080820
	if(nStartXposition > 6)
	{
		nStartXposition -= 5;
	}

	if(nEndXposition < (DstImage.m_InfoHeader.biWidth-6))
	{
		nEndXposition += 5;
	}

	if(nStartYposition > 6)
	{
		nStartYposition -=  5;
	}

	if(nEndYposition < (DstImage.m_InfoHeader.biHeight-6))
	{
		nEndYposition += 5;
	}
	//-

	if(nStartXposition - nXmargin < 0)
	{
		nStartXposition =  nXmargin;
	}

	if(nEndXposition + nXmargin >= DstImage.m_InfoHeader.biWidth)
	{
		nEndXposition =  nEndXposition - nXmargin;
	}

	if(nStartYposition - nYmargin < 0)
	{
		nStartYposition =  nYmargin;
	}

	if(nEndYposition + nYmargin >= DstImage.m_InfoHeader.biHeight)
	{
		nEndYposition =  nEndYposition - nYmargin;
	}

	//+add 090130
	unsigned char *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *pD;

	if(DstImage.m_InfoHeader.biBitCount==24)
	{
		for(int nRow=nStartYposition; nRow<=nEndYposition; nRow++)
		{
			//+add 090130
			p1 = DstImage.m_pImageData + (nRow-nYmargin)  *nBytesInRow+3*(nStartXposition-nXmargin);
			p2 = DstImage.m_pImageData + (nRow-nYmargin)  *nBytesInRow+3*(nStartXposition-nXmargin+1);
			p3 = DstImage.m_pImageData + (nRow-nYmargin)  *nBytesInRow+3*(nStartXposition-nXmargin+2);
			p4 = DstImage.m_pImageData + (nRow-nYmargin+1)*nBytesInRow+3*(nStartXposition-nXmargin);
			p5 = DstImage.m_pImageData + (nRow-nYmargin+1)*nBytesInRow+3*(nStartXposition-nXmargin+1);
			p6 = DstImage.m_pImageData + (nRow-nYmargin+1)*nBytesInRow+3*(nStartXposition-nXmargin+2);
			p7 = DstImage.m_pImageData + (nRow-nYmargin+2)*nBytesInRow+3*(nStartXposition-nXmargin);
			p8 = DstImage.m_pImageData + (nRow-nYmargin+2)*nBytesInRow+3*(nStartXposition-nXmargin+1);
			p9 = DstImage.m_pImageData + (nRow-nYmargin+2)*nBytesInRow+3*(nStartXposition-nXmargin+2);

			pD = DstImage.m_pProcessedImageData + nRow*nBytesInRow + 3*nStartXposition;
			//-

			for(int nColumn=nStartXposition; nColumn<=nEndXposition; nColumn++) 
			{ 
				//+add 090130
				*pD++ = (*p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++ + *p9++)/9;
				*pD++ = (*p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++ + *p9++)/9;
				*pD++ = (*p1++ + *p2++ + *p3++ + *p4++ + *p5++ + *p6++ + *p7++ + *p8++ + *p9++)/9;

				//+del 090130
			/*
				nNewRValue = 0;
				nNewGValue = 0;
				nNewBValue = 0;
				
				for(int nX=0; nX<nMaskWidth; nX++)
				{
					for(int nY=0; nY<nMaskHeight; nY++)
					{
						nNewBValue += nMaskBox[nX][nY] * DstImage.m_pImageData[(nRow+nY-nYmargin)*nBytesInRow+3*(nColumn+nX-nXmargin)];
						nNewGValue += nMaskBox[nX][nY] * DstImage.m_pImageData[(nRow+nY-nYmargin)*nBytesInRow+3*(nColumn+nX-nXmargin)+1];
						nNewRValue += nMaskBox[nX][nY] * DstImage.m_pImageData[(nRow+nY-nYmargin)*nBytesInRow+3*(nColumn+nX-nXmargin)+2];
					}
				}
			
				nNewRValue /= nMaskWidth*nMaskHeight;
				nNewGValue /= nMaskWidth*nMaskHeight;
				nNewBValue /= nMaskWidth*nMaskHeight;

				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn]   = (BYTE)nNewBValue;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+1] = (BYTE)nNewGValue;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2] = (BYTE)nNewRValue;
			*/
			}
		}
	}

	DstImage.m_bProcessedImageLoaded = TRUE;

	if(bOverwrite)
	{
		for(int j=0; j<(int)DstImage.m_InfoHeader.biSizeImage; j++)
		{
			DstImage.m_pImageData[j] = DstImage.m_pProcessedImageData[j];
		}
	}

	return TEST_PASS;
}

BOOL CImageProc::BinaryForMaskImage(int nType,CBaseImage& DstImage,int nThreshold,BOOL bOverwrite)
{
	if(DstImage.m_InfoHeader.biBitCount < 24)
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_FAIL;
	}
	
	int nProgress	= 0;
	int nColorDepth = DstImage.m_InfoHeader.biBitCount;
	int nBytesInRow = WIDTHBYTES(nColorDepth*DstImage.m_InfoHeader.biWidth); 
	
	int nBinaryValue1  = 0;
	int nBinaryValue2  = 0;

	int nImageBufferSize = 0;
	int nImageSize = 0;
	
	if(nType == NORMAL_BINARY)
	{
		nBinaryValue1  = 255;
		nBinaryValue2  = 0;
	}
	else
	{
		nBinaryValue1  = 0;
		nBinaryValue2  = 255;
	}

	nImageSize = DstImage.m_InfoHeader.biSizeImage;

	if(DstImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pProcessedImageData); DstImage.m_pProcessedImageData = NULL;
			DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(DstImage.m_pProcessedImageData,0,DstImage.m_BmInfo->bmiHeader.biSizeImage);

	DstImage.m_bProcessedImageLoaded = FALSE;

	for(int nRow=0; nRow<DstImage.m_InfoHeader.biHeight; nRow++)
	{
		for(int nColumn=0; nColumn<DstImage.m_InfoHeader.biWidth; nColumn++) 
		{  
			if((DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2] > nThreshold)	// R
			|| (DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1] > nThreshold)	// G
			|| (DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn]	> nThreshold))	// B
			{
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue1;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue1;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue1;
			}
			else
			{
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue2;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue2;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue2;
			}
		}
	}

	DstImage.m_bProcessedImageLoaded = TRUE;

	if(bOverwrite)
	{
		for(int j=0; j<(int)DstImage.m_InfoHeader.biSizeImage; j++)
		{
			DstImage.m_pImageData[j] = DstImage.m_pProcessedImageData[j];
		}
	}

	return TEST_PASS;
}

// Under Threshold : Black, Over Threashold : White
int CImageProc::BinaryForMonoImage(int nType, BYTE* pImageData,int nHeight,int nWidth,int nBitCount,int nThreshold)
{
	if(pImageData == NULL)
	{
		AfxMessageBox("Input data is null!"); return TEST_FAIL;
	}

	int nProgress	= 0;
	int nColorDepth = nBitCount;
	int nBytesInRow = WIDTHBYTES(nColorDepth*nWidth); 

	int nBinaryValue1  = 0;
	int nBinaryValue2  = 0;

	if(nType == NORMAL_BINARY)
	{
		nBinaryValue1  = 255;
		nBinaryValue2  = 0;
	}
	else
	{
		nBinaryValue1  = 0;
		nBinaryValue2  = 255;
	}

	for(int nRow=0; nRow<nHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nWidth; nColumn++) 
		{ 
			if((pImageData[nRow*nBytesInRow+3*nColumn+2] > nThreshold))		// R
			{
				pImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue1;
				pImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue1;
				pImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue1;
			}
			else
			{
				pImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue2;
				pImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue2;
				pImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue2;
			}
		}
	}
	return TEST_PASS;
}

int CImageProc::BinaryForDiffImage(int nType, CBaseImage& DstImage, CBaseImage& RefImage, int nLowerLimit, int nUpperLimit)
{
	int nImageBufferSize = 0;
	int nImageSize = DstImage.m_InfoHeader.biSizeImage;

	if(DstImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pProcessedImageData); DstImage.m_pProcessedImageData = NULL;
			DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(DstImage.m_pProcessedImageData,0,DstImage.m_BmInfo->bmiHeader.biSizeImage);

	BYTE* pImageData = DstImage.m_pProcessedImageData;

	int nProgress	= 0;
	int nColorDepth = RefImage.m_InfoHeader.biBitCount;
	int nBytesInRow = WIDTHBYTES(nColorDepth*RefImage.m_InfoHeader.biWidth); 

	int nBinaryValue1  = 0;
	int nBinaryValue2  = 0;

	int nDifferenceR = 0;
	int nDifferenceG = 0;
	int nDifferenceB = 0;

	int nHeight = RefImage.m_InfoHeader.biHeight;
	int nWidth  = RefImage.m_InfoHeader.biWidth;

	if(nType == NORMAL_BINARY)
	{
		nBinaryValue1  = 255;
		nBinaryValue2  = 0;
	}
	else
	{
		nBinaryValue1  = 0;
		nBinaryValue2  = 255;
	}

	for(int nRow=0; nRow<nHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nWidth; nColumn++) 
		{ 
			nDifferenceR = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2] - RefImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2];
			nDifferenceG = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1] - RefImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1];
			nDifferenceB = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn  ] - RefImage.m_pImageData[nRow*nBytesInRow+3*nColumn  ];
			
			if((nDifferenceR <= nUpperLimit) && (nDifferenceR >= nLowerLimit)  // R
			&& (nDifferenceG <= nUpperLimit) && (nDifferenceG >= nLowerLimit)  // G
			&& (nDifferenceB <= nUpperLimit) && (nDifferenceB >= nLowerLimit)) // B
			{
				// Spec In
				pImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue2;
				pImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue2;
				pImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue2;
			}
			else
			{
				// Spec Out
				pImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue1;
				pImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue1;
				pImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue1;
			}
		}
	}

	DstImage.m_bProcessedImageLoaded = TRUE;

	return TEST_PASS;
}

//+add kwmoon 071112
// Spec-Out : White, Spec-In : Black
int CImageProc::BinaryForDiffImage(int nType, CBaseImage& SnapImage, int nLowerLimit, int nUpperLimit)
{
	int nProgress	= 0;
	int nColorDepth = SnapImage.m_InfoHeader.biBitCount;
	int nBytesInRow = WIDTHBYTES(nColorDepth*SnapImage.m_InfoHeader.biWidth); 

	int nBinaryValue1  = 0;
	int nBinaryValue2  = 0;

	int nDifferenceR = 0;
	int nDifferenceG = 0;
	int nDifferenceB = 0;

	int nHeight = SnapImage.m_InfoHeader.biHeight;
	int nWidth  = SnapImage.m_InfoHeader.biWidth;

	int nIndex	= 0;

	if(nType == NORMAL_BINARY)
	{
		nBinaryValue1  = 255;
		nBinaryValue2  = 0;
	}
	else
	{
		nBinaryValue1  = 0;
		nBinaryValue2  = 255;
	}

	for(int nRow=0; nRow<nHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nWidth; nColumn++) 
		{ 
			nIndex = nRow*nBytesInRow+3*nColumn;

			if((nColumn == 334) && (nRow == 384))
			{
				int nDebug = 1;
			}

			nDifferenceR = SnapImage.m_pProcessedImageData[nIndex+2];
			nDifferenceG = SnapImage.m_pProcessedImageData[nIndex+1];
			nDifferenceB = SnapImage.m_pProcessedImageData[nIndex  ];
			
			if((nDifferenceR <= nUpperLimit) && (nDifferenceR >= nLowerLimit)  // R
			&& (nDifferenceG <= nUpperLimit) && (nDifferenceG >= nLowerLimit)  // G
			&& (nDifferenceB <= nUpperLimit) && (nDifferenceB >= nLowerLimit)) // B
			{
				// Spec In
				SnapImage.m_pProcessedImageData[nIndex+2] = nBinaryValue2;
				SnapImage.m_pProcessedImageData[nIndex+1] = nBinaryValue2;
				SnapImage.m_pProcessedImageData[nIndex  ] = nBinaryValue2;
			}
			else
			{
				// Spec Out
				SnapImage.m_pProcessedImageData[nIndex+2] = nBinaryValue1;
				SnapImage.m_pProcessedImageData[nIndex+1] = nBinaryValue1;
				SnapImage.m_pProcessedImageData[nIndex  ] = nBinaryValue1;
			}
		}
	}

	SnapImage.m_bProcessedImageLoaded = TRUE;

	return TEST_PASS;
}

// Under Threshold : Black, Over Threashold : White
int CImageProc::BinaryForColorImage(int nType,CBaseImage& DstImage,int nThreshold,BOOL bOverwrite)
{
	int nProgress	= 0;
	int nColorDepth = DstImage.m_InfoHeader.biBitCount;
	int nBytesInRow = WIDTHBYTES(nColorDepth*DstImage.m_InfoHeader.biWidth); 

	int nBinaryValue1  = 0;
	int nBinaryValue2  = 0;
	
	int nImageBufferSize = 0;
	int nImageSize = 0;

	if(nType == NORMAL_BINARY)
	{
		nBinaryValue1  = 255;
		nBinaryValue2  = 0;
	}
	else
	{
		nBinaryValue1  = 0;
		nBinaryValue2  = 255;
	}

	nImageSize = DstImage.m_InfoHeader.biSizeImage;

	if(DstImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pProcessedImageData); DstImage.m_pProcessedImageData = NULL;
			DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(DstImage.m_pProcessedImageData,0,DstImage.m_BmInfo->bmiHeader.biSizeImage);

	if(RGB2Gray(NORMAL_BINARY,DstImage,TRUE) == TEST_FAIL) return TEST_ABORT;	

	for(int nRow=0; nRow<DstImage.m_InfoHeader.biHeight; nRow++)
	{
		for(int nColumn=0; nColumn<DstImage.m_InfoHeader.biWidth; nColumn++) 
		{  
			if((DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2] > nThreshold))	// R
			{
				// Spec In
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue1;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue1;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue1;
			}
			else
			{
				// Spec Out
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2] = nBinaryValue2;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+1] = nBinaryValue2;
				DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn]   = nBinaryValue2;
			}
		}
	}

	DstImage.m_bProcessedImageLoaded = TRUE;

	if(bOverwrite)
	{
		for(int j=0; j<(int)DstImage.m_InfoHeader.biSizeImage; j++)
		{
			DstImage.m_pImageData[j] = DstImage.m_pProcessedImageData[j];
		}
	}

	return TEST_PASS;
}

// In case of using ROI, position margin can be applied.
int CImageProc::PatternMatching(int nTestType, CBaseImage &SnapImage, CBaseImage &RefImage, 
								POINT& ptResultPosition,float& fMatchRate, CProgressCtrl &Progress)
{
	if((SnapImage.m_InfoHeader.biBitCount < 24)
	|| (RefImage.m_InfoHeader.biBitCount  < 24))
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_ABORT;
	}
	
	int nProgress	= 0;
	int nTestResult = TEST_PASS;
	int nThreshold  = 127;

	int nColorDepth = RefImage.m_InfoHeader.biBitCount;

	int nMatchXpos = 0; // X position of matching image
	int nMatchYpos = 0; // Y position of matching image

	m_nNoErrorInfo = 0;

	// Image Width in Byte
	int nSnapBytesInRow = WIDTHBYTES(nColorDepth*SnapImage.m_InfoHeader.biWidth); 
	int nRefBytesInRow  = WIDTHBYTES(nColorDepth*RefImage.m_InfoHeader.biWidth); 

	// Dimension of Ref Image
	int nRefWidth  = RefImage.m_InfoHeader.biWidth;
	int nRefHeight = RefImage.m_InfoHeader.biHeight;
	
	// Dimension of Snap Image
	int nSnapWidth  = SnapImage.m_InfoHeader.biWidth;
	int nSnapHeight = SnapImage.m_InfoHeader.biHeight;
	
	CString szErrMsg = _T("");

	if((nRefWidth != nSnapWidth) || (nRefHeight != nSnapHeight))
	{
		AfxMessageBox("The size of reference image should be the same!"); 
		return TEST_ABORT;
	}

	if(RefImage.m_InfoHeader.biBitCount != SnapImage.m_InfoHeader.biBitCount)
	{
		AfxMessageBox("The color depth of two images is different!"); return TEST_ABORT;
	}

	if((m_nRoiWidth >= nSnapWidth)	|| (m_nRoiHeight >= nSnapHeight))
	{
		szErrMsg.Format("[ROI Error] Roi with & height should be less than witdh & height of image! (W:%d_%d,H:%d_%d)"
					   ,m_nRoiWidth,nSnapWidth,m_nRoiHeight,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if((m_ptRoiPosition.x >= (nSnapWidth-1)) || (m_ptRoiPosition.y >= (nSnapHeight-1))) 
	{
		szErrMsg.Format("[ROI Error] Roi (x,y) should be less than witdh & height of image! (X:%d,Y:%d,W:%d,H:%d)"
					   ,m_ptRoiPosition.x,m_ptRoiPosition.y,nSnapWidth,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if((m_nRoiWidth < 0) || (m_nRoiHeight < 0)) 
	{
		szErrMsg.Format("[ROI Error] Roi width & height should be more than 0! (Width:%d,Height:%d)"
					   ,m_nRoiWidth,m_nRoiHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	// Set Margin in pixel
	POINT nPositionMargin;
	
	nPositionMargin.x = DEFAULT_POSITION_MARGIN;
	nPositionMargin.y = DEFAULT_POSITION_MARGIN;

	int nSnapStartXposition  = 0;
	int nSnapStartYposition  = 0;
	int nSnapEndXposition	 = 0;
	int nSnapEndYposition    = 0;
	int nStartXposition = 0;
	int nStartYposition = 0;
	int nEndXposition	= 0;
	int nEndYposition	= 0;
	
	//+change kwmoon 080618
	CalcTestRange(SnapImage,nStartXposition,nEndXposition,nStartYposition,nEndYposition);

	// Binary Process
	if(RGB2Gray(NORMAL_BINARY,SnapImage,FALSE) == TEST_FAIL) return TEST_ABORT;	
	if(RGB2Gray(NORMAL_BINARY,RefImage,FALSE)  == TEST_FAIL) return TEST_ABORT;
	

	// Variables for image processing
	double dwMatchRate = 0.0f;

	int NoT;
	float ST,temp, SumT;
	float MaxCorr, CurCorr, a1, a2, DeNomi, Nomi;
	float SumI, SI, ScT, tpi;

	POINT nOutputPosition;

	ST		= 0.0f;
	temp	= 0.0f;
	SumT	= 0.0f;
	NoT		= 0;

	Progress.SetPos(10);

	if((m_nRoiHeight > 0) && (m_nRoiWidth > 0)) // In case of applying ROI
	{
		for(int nRow=nStartYposition;nRow<=(nStartYposition+m_nRoiHeight);nRow++)
		{
			for(int nColumn=nStartXposition;nColumn<=(nStartXposition+m_nRoiWidth) ;nColumn++) 
			{
				temp =(float)RefImage.m_pProcessedImageData[nRow*nRefBytesInRow+3*nColumn];
				SumT  += temp;
				ST += temp * temp;
				NoT++;
			}
		}
	}
	else 
	{
		for(int nRow=0;nRow<nRefHeight;nRow++)
		{
			for(int nColumn=0;nColumn<nRefWidth ;nColumn++) 
			{
				temp =(float)RefImage.m_pProcessedImageData[nRow*nRefBytesInRow+3*nColumn];
				SumT  += temp;
				ST += temp * temp;
				NoT++;
			}
		}
	}
	Progress.SetPos(20);

	MaxCorr = 0.0f;
	CurCorr = 0.0f;
	a1		= 0.0f;
	a2		= 0.0f;
	DeNomi  = 0.0f;
	Nomi	= 0.0f;
	SumI	= 0.0f;
	SI		= 0.0f;
	ScT		= 0.0f;
	tpi		= 0.0f;
	nOutputPosition.x = 0;
	nOutputPosition.y = 0;

	if((m_nRoiHeight > 0) && (m_nRoiWidth > 0)) // In case of applying ROI
	{
		// Set nSnapStartXposition
		if((nStartXposition - nPositionMargin.x) < 0)
		{
			nSnapStartXposition = 0;
		}
		else
		{
			nSnapStartXposition = - nPositionMargin.x;
		}

		// Set nSnapStartYposition
		if((nStartYposition  - nPositionMargin.y) < 0)
		{
			nSnapStartYposition = 0;
		}
		else
		{
			nSnapStartYposition = - nPositionMargin.y;
		}

		// Set nSnapEndXposition
		if((nEndXposition + nPositionMargin.x) > nSnapWidth)
		{
			nSnapEndXposition = (nSnapWidth-1) - nEndXposition;
		}
		else
		{
			nSnapEndXposition = nPositionMargin.x;
		}

		if((nEndYposition + nPositionMargin.y) > nSnapHeight)
		{
			nSnapEndYposition = (nSnapHeight-1) - nEndYposition;
		}
		else
		{
			nSnapEndYposition = nPositionMargin.y;
		}
	}
	else // In case of full size test
	{
		// Snap Image Position
		nSnapStartXposition = 0;
		nSnapStartYposition = 0;

		nSnapEndXposition =  0;	
		nSnapEndYposition = 0;	
	}

	for(int m=nSnapStartYposition; m<=nSnapEndYposition; m++) 
	{
		for(int n=nSnapStartXposition; n<=nSnapEndXposition; n++) 
		{
			SumI = SI = ScT  = 0.0f;

			for(int nRow=nStartYposition;nRow<=nEndYposition;nRow++)
			{
				for(int nColumn=nStartXposition;nColumn<=nEndXposition ;nColumn++) 
				{
					temp  = (float)SnapImage.m_pProcessedImageData[(m+(nRow-m_ptPositionShift.y))*nSnapBytesInRow+3*(n+(nColumn+m_ptPositionShift.x))];
					tpi   = (float)RefImage.m_pProcessedImageData[(nRow)*nRefBytesInRow+3*(nColumn)];
				
					SumI += temp;
					SI   += temp * temp ;  
					ScT  += tpi*temp;
				}
			}

			a1 = NoT*SI-SumI*SumI;
			a2 = NoT*ST-SumT*SumT;

			DeNomi = (float)( a1*a2 );
			Nomi   = ( NoT*ScT - SumI*SumT);
			
			if (DeNomi < 0.0001 ) CurCorr = 0;
			else CurCorr = Nomi*Nomi/DeNomi;

			if (CurCorr>MaxCorr) 
			{
				MaxCorr			  = CurCorr;
				nOutputPosition.y = m;
				nOutputPosition.x = n;
			}
		}

		if(nSnapEndYposition > 0)
		{
			nProgress = 20 + (int)((float)m*80/(float)(nSnapEndYposition));
			Progress.SetPos(nProgress);
		}
	}

	nMatchXpos = nStartXposition + nOutputPosition.x;

	if((m_nRoiHeight > 0) && (m_nRoiWidth > 0)) // In case of applying ROI
	{
		nMatchYpos = GetOppositeStartYposition(nStartYposition + nOutputPosition.y,m_nRoiHeight,nSnapHeight);
	}
	else
	{
		nMatchYpos = nOutputPosition.y;
	}
	
	dwMatchRate = sqrt(MaxCorr);

	dwMatchRate = (float)dwMatchRate * 100;

	if(dwMatchRate < m_fTargetMatchRate)
	{
		nTestResult = TEST_FAIL;
	}
	else
	{
		ptResultPosition.x = nMatchXpos;
		ptResultPosition.y = nMatchYpos;
	}

	Progress.SetPos(100);

	fMatchRate = (float)dwMatchRate;

	//+change kwmoon 071105
	if(fMatchRate > 100.0)
	{
		fMatchRate = 100.0;
	}

	return nTestResult;
}

int CImageProc::GetOppositeStartYposition(int nPosition, int nRoiHeight, int nImageHeight)
{
	int nResult = 0;
	int nMidPosition = nImageHeight/2;

	if(nImageHeight % 2 == 0) // Even Number
	{
		if(nPosition <= nMidPosition)
		{
			nResult = (nMidPosition - nPosition) + nMidPosition - 1 - nRoiHeight;
		}
		else
		{
			nResult = nMidPosition - (nPosition - nMidPosition) - 1 - nRoiHeight;
		}
	}
	else // Odd Number
	{
		if(nPosition <= nMidPosition)
		{
			nResult = (nMidPosition - nPosition) + nMidPosition - nRoiHeight;
		}
		else
		{
			nResult = nMidPosition - (nPosition - nMidPosition) - nRoiHeight;
		}
	}
	return nResult;
}

int	CImageProc::MakeAvgImg(CStringArray& aFilePathArray,CBaseImage& DstImage,CProgressCtrl& ItemProgress,CProgressCtrl& TotalProgress)
{
	//+add 090219(Modification No2)
	CString szLog = "B:MakeAvgImg\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	CString szErrMsg	= _T("");
	CString szFilePath	= _T("");

	int nTestResult = TEST_PASS;
	
	int nImageColorDepth		= 0;
	int nFirstImageColorDepth	= 0;

	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nFirstImageHeight	= 0;
	int nFirstImageWidth	= 0;

	int nNoImage	= aFilePathArray.GetSize();
	int nBytesInRow	= 0; 

	int nAvgRvalue	= 0;
	int nAvgGvalue	= 0;
	int nAvgBvalue	= 0;
	int nImageSize	= 0;

	int nImageBufferSize = 0;

	CBaseImage	m_aImageList[MAX_NO_IMAGE_TO_MAKE_AVERAGE_IMAGE];

	TotalProgress.SetPos(0);

	if(nNoImage < 2)
	{
		AfxMessageBox("To make an average image, at least 2 images are required!"); return TEST_ABORT;
	}

	if(nNoImage > MAX_NO_IMAGE_TO_MAKE_AVERAGE_IMAGE)
	{
		AfxMessageBox("The maximum number of input images for making an average image is 10 images!"); return TEST_ABORT;
	}

	// Load Images
	for(int i=0; i<nNoImage; i++)
	{
		szFilePath = aFilePathArray.GetAt(i);

		if(!m_aImageList[i].LoadImage(szFilePath,ItemProgress))
		{
			for(int j=0; j<i; j++)
			{
				m_aImageList[i].~CBaseImage();	
			}
			return TEST_ABORT;
		}
	}

	// Check image size & Color depth
	nFirstImageWidth		= m_aImageList[0].m_InfoHeader.biWidth;
	nFirstImageHeight		= m_aImageList[0].m_InfoHeader.biHeight;
	nFirstImageColorDepth	= m_aImageList[0].m_InfoHeader.biBitCount;

	for(int j=1; j<nNoImage; j++)
	{
		nImageWidth			= m_aImageList[j].m_InfoHeader.biWidth;
		nImageHeight		= m_aImageList[j].m_InfoHeader.biHeight;
		nImageColorDepth	= m_aImageList[j].m_InfoHeader.biBitCount;

		if((nFirstImageWidth != nImageWidth) || (nFirstImageHeight != nImageHeight))
		{
			szErrMsg.Format("The size of two images is different! Img1:(%d,%d) -> Img%d:(%d,%d)",
							nFirstImageWidth,nFirstImageHeight,j,nImageWidth,nImageHeight);

			for(int k=0; k<nNoImage; k++) m_aImageList[k].~CBaseImage();	
			
			AfxMessageBox(szErrMsg); return TEST_ABORT;
		}

		if(nFirstImageColorDepth != nImageColorDepth)
		{
			szErrMsg.Format("The color depth of two images is different! Img1:(%dbits) -> Img%d:(%dbits)",
							nFirstImageColorDepth,j,nImageColorDepth);

			for(int k=0; k<nNoImage; k++) m_aImageList[k].~CBaseImage();	

			AfxMessageBox(szErrMsg); return TEST_ABORT;
		}
	}

	nBytesInRow= WIDTHBYTES(nFirstImageColorDepth*nFirstImageWidth); 

	TotalProgress.SetPos(10);

	nImageSize = nFirstImageHeight * nBytesInRow;

	if(DstImage.m_pImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pImageData); DstImage.m_pImageData = NULL;
			DstImage.m_pImageData = (BYTE *)malloc(nImageSize);
		}
	}
	else
	{
		DstImage.m_pImageData = (BYTE *)malloc(nImageSize);
	}
	memset(DstImage.m_pImageData,0,nImageSize);

	DstImage.InitHeaders(COLOR_24BIT,nFirstImageWidth,nFirstImageHeight,nImageSize);

	if(nFirstImageColorDepth == 24) // If the color depth of image is 24 bits  
	{
		for(int nRow=0; nRow<nFirstImageHeight; nRow++)
		{
			for(int nColumn=0; nColumn<nFirstImageWidth; nColumn++) 
			{  
				nAvgRvalue = 0;
				nAvgGvalue = 0;
				nAvgBvalue = 0;

				for(int j=0; j<nNoImage; j++)
				{
					nAvgRvalue += m_aImageList[j].m_pImageData[nRow*nBytesInRow+3*nColumn+2];
					nAvgGvalue += m_aImageList[j].m_pImageData[nRow*nBytesInRow+3*nColumn+1];
					nAvgBvalue += m_aImageList[j].m_pImageData[nRow*nBytesInRow+3*nColumn];
				}
				DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2]	= (nAvgRvalue/nNoImage);
				DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1] = (nAvgGvalue/nNoImage);
				DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn]	= (nAvgBvalue/nNoImage);
			}
		}
	}

	DstImage.m_bImageLoaded = TRUE;

	TotalProgress.SetPos(100);

	//+add 090219(Modification No2)
	szLog = "E:MakeAvgImg\n";
//	if(g_LogFileOpen) g_LogFile.WriteString(szLog);

	return nTestResult;			
}

int	CImageProc::MakeAvgImg(CStringArray& aFilePathArray,CBaseImage& DstImage,CProgressCtrl& ItemProgress)
{
	CString szErrMsg	= _T("");
	CString szFilePath	= _T("");

	int nTestResult = TEST_PASS;
	
	int nImageColorDepth		= 0;
	int nFirstImageColorDepth	= 0;

	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nFirstImageHeight	= 0;
	int nFirstImageWidth	= 0;

	int nNoImage	= aFilePathArray.GetSize();
	int nBytesInRow	= 0; 

	int nAvgRvalue	= 0;
	int nAvgGvalue	= 0;
	int nAvgBvalue	= 0;
	int nImageSize	= 0;

	int nImageBufferSize = 0;

	CBaseImage	m_aImageList[MAX_NO_IMAGE_TO_MAKE_AVERAGE_IMAGE];

	if(nNoImage < 2)
	{
		AfxMessageBox("To make an average image, at least 2 images are required!"); return TEST_ABORT;
	}

	if(nNoImage > MAX_NO_IMAGE_TO_MAKE_AVERAGE_IMAGE)
	{
		AfxMessageBox("The maximum number of input images for making an average image is 10 images!"); return TEST_ABORT;
	}

	// Load Images
	for(int i=0; i<nNoImage; i++)
	{
		szFilePath = aFilePathArray.GetAt(i);

		if(!m_aImageList[i].LoadImage(szFilePath,ItemProgress))
		{
			for(int j=0; j<i; j++)
			{
				m_aImageList[i].~CBaseImage();	
			}
			return TEST_ABORT;
		}
	}

	// Check image size & Color depth
	nFirstImageWidth		= m_aImageList[0].m_InfoHeader.biWidth;
	nFirstImageHeight		= m_aImageList[0].m_InfoHeader.biHeight;
	nFirstImageColorDepth	= m_aImageList[0].m_InfoHeader.biBitCount;

	for(int j=1; j<nNoImage; j++)
	{
		nImageWidth			= m_aImageList[j].m_InfoHeader.biWidth;
		nImageHeight		= m_aImageList[j].m_InfoHeader.biHeight;
		nImageColorDepth	= m_aImageList[j].m_InfoHeader.biBitCount;

		if((nFirstImageWidth != nImageWidth) || (nFirstImageHeight != nImageHeight))
		{
			szErrMsg.Format("The size of two images is different! Img1:(%d,%d) -> Img%d:(%d,%d)",
							nFirstImageWidth,nFirstImageHeight,j,nImageWidth,nImageHeight);

			for(int k=0; k<nNoImage; k++) m_aImageList[k].~CBaseImage();	
			
			AfxMessageBox(szErrMsg); return TEST_ABORT;
		}

		if(nFirstImageColorDepth != nImageColorDepth)
		{
			szErrMsg.Format("The color depth of two images is different! Img1:(%dbits) -> Img%d:(%dbits)",
							nFirstImageColorDepth,j,nImageColorDepth);

			for(int k=0; k<nNoImage; k++) m_aImageList[k].~CBaseImage();	

			AfxMessageBox(szErrMsg); return TEST_ABORT;
		}
	}

	nBytesInRow= WIDTHBYTES(nFirstImageColorDepth*nFirstImageWidth); 

	nImageSize = nFirstImageHeight * nBytesInRow;

	if(DstImage.m_pImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pImageData); DstImage.m_pImageData = NULL;
			DstImage.m_pImageData = (BYTE *)malloc(nImageSize);
		}
	}
	else
	{
		DstImage.m_pImageData = (BYTE *)malloc(nImageSize);
	}
	memset(DstImage.m_pImageData,0,nImageSize);

	DstImage.InitHeaders(COLOR_24BIT,nFirstImageWidth,nFirstImageHeight,nImageSize);

	if(nFirstImageColorDepth == 24) // If the color depth of image is 24 bits  
	{
		for(int nRow=0; nRow<nFirstImageHeight; nRow++)
		{
			for(int nColumn=0; nColumn<nFirstImageWidth; nColumn++) 
			{  
				nAvgRvalue = 0;
				nAvgGvalue = 0;
				nAvgBvalue = 0;

				for(int j=0; j<nNoImage; j++)
				{
					nAvgRvalue += m_aImageList[j].m_pImageData[nRow*nBytesInRow+3*nColumn+2];
					nAvgGvalue += m_aImageList[j].m_pImageData[nRow*nBytesInRow+3*nColumn+1];
					nAvgBvalue += m_aImageList[j].m_pImageData[nRow*nBytesInRow+3*nColumn];
				}
				DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2]	= (nAvgRvalue/nNoImage);
				DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1] = (nAvgGvalue/nNoImage);
				DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn]	= (nAvgBvalue/nNoImage);
			}
		}
	}

	DstImage.m_bImageLoaded = TRUE;

	return nTestResult;			
}

//+change kwmoon 080125
//BOOL CImageProc::MakeMaskImg(int nMaskWidth,CBaseImage& SrcImage,CBaseImage& DstImage,CProgressCtrl& Progress)
BOOL CImageProc::MakeMaskImg(int nMaskWidth,CBaseImage& SrcImage,CBaseImage& DstImage,CProgressCtrl& Progress,BOOL bInvalidateOsdRegion)
{
	BYTE   *lBmBuf;

	int	   *lImg[2][3];	// lImg[0] is for the source image and llmg[1] is for the processed image.
	int	   **lImgPtr,**lImgPtr0,**lImgPtr1;
	int    l_i,l_j;
	int	   lCnt;

	// Dimension of the Snap Image
	int nSrcWidth    = SrcImage.m_InfoHeader.biWidth;
	int nSrcHeight   = SrcImage.m_InfoHeader.biHeight;
	int nSrcBitCount = SrcImage.m_InfoHeader.biBitCount;

	int nBytesInRow= WIDTHBYTES(SrcImage.m_InfoHeader.biBitCount*SrcImage.m_InfoHeader.biWidth); 

	int nImageSize = SrcImage.m_InfoHeader.biSizeImage;

	int nImageBufferSize = 0;

	if(&SrcImage != &DstImage)
	{

		if(DstImage.m_pImageData != NULL)
		{
			nImageBufferSize = _msize(DstImage.m_pImageData);

			if(nImageBufferSize != nImageSize)
			{
				free(DstImage.m_pImageData); DstImage.m_pImageData = NULL;
				DstImage.m_pImageData = (BYTE *)malloc(nImageSize);
			}
		}
		else
		{
			DstImage.m_pImageData = (BYTE *)malloc(nImageSize);
		}
		memset(DstImage.m_pImageData,0,nImageSize);
	
		DstImage.InitHeaders(COLOR_24BIT,nSrcWidth,nSrcHeight,nImageSize);

		for(int i=0; i<nImageSize; i++)
		{ 
			DstImage.m_pImageData[i] = SrcImage.m_pImageData[i];
		}
	}

	int nDstWidth    = DstImage.m_InfoHeader.biWidth;
	int nDstHeight   = DstImage.m_InfoHeader.biHeight;
	int nDstBitCount = DstImage.m_InfoHeader.biBitCount;

	Progress.SetPos(10);

	lBmBuf = DstImage.m_pImageData;

	CString szMessage;

	for (l_j = 0; l_j < 2; l_j++)
	{
		for (l_i = 0; l_i < 3; l_i++)
		{
			if ((lImg[l_j][l_i] = (int*)malloc(nDstWidth*nDstHeight*4)) == NULL)	// *4 -> int	
			{
				szMessage.Format("Allocte Error : size = %d",nDstWidth*nDstHeight*4);
				AfxMessageBox(szMessage);
				return FALSE;
			}
		}
	}

	ExtPlane(lBmBuf,lImg[0],nDstWidth,nDstHeight,nDstBitCount); // Extract each R/G/B Planes from lBmBuf and then save it to llmg[j]

	Progress.SetPos(20);

	// lImg[0] is for the source image and llmg[1] is for the processed image.	
	lImgPtr  = lImg[0];	
	lImgPtr0 = lImg[1]; 

	// Process Image 
	for (l_i = 0; l_i < 3; l_i++)
	{
		Filter(*(lImgPtr+l_i),*(lImgPtr0+l_i),nDstWidth,nDstHeight,3,3,Mask3x3_a);	// Apply Mask3x3_a to all of three R/G/B planes
	}
	lImgPtr1 = lImgPtr; lImgPtr = lImgPtr0; lImgPtr0 = lImgPtr1;

	Progress.SetPos(30);

	for (l_i = 0; l_i < 3; l_i++)
	{
		Filter(*(lImgPtr+l_i),*(lImgPtr0+l_i),nDstWidth,nDstHeight,7,7,Mask7x7_a);	// Apply Mask7x7_a to all of three R/G/B planes
	}
	lImgPtr1 = lImgPtr; lImgPtr = lImgPtr0; lImgPtr0 = lImgPtr1;

	Progress.SetPos(40);

	for (l_i = 0; l_i < 3; l_i++)
	{
		OuterFill(*(lImgPtr+l_i),nDstWidth,nDstHeight,4,4,255);	// Paint outlines with wihte color
	}

	Progress.SetPos(50);

	for (l_i = 0; l_i < 3; l_i++)
	{
		ConstThr(*(lImgPtr+l_i),*(lImgPtr0+l_i),nDstWidth,nDstHeight,-80,80,0,255);	// Binary-processing
	}

	Progress.SetPos(60);

	lImgPtr1 = lImgPtr; lImgPtr = lImgPtr0; lImgPtr0 = lImgPtr1;


	for (lCnt = 0; lCnt < nMaskWidth; lCnt++)
	{
		for (l_i = 0; l_i < 3; l_i++)
			Dilation(*(lImgPtr+l_i),*(lImgPtr0+l_i),nDstWidth,nDstHeight);
		lImgPtr1 = lImgPtr; lImgPtr = lImgPtr0; lImgPtr0 = lImgPtr1;
	}

	Progress.SetPos(70);

	// Desposit Image
	DepPlane(lImgPtr,lBmBuf,nDstWidth,nDstHeight,nDstBitCount);	// Copy the processed image stored in llmgPtr to lBmBuf

	for (l_j = 0; l_j < 2; l_j++)
	{
		for (l_i = 0; l_i < 3; l_i++) 
		{
			free(lImg[l_j][l_i]);
		}
	}

	DstImage.m_bImageLoaded = TRUE;

	// Make Black&White Mask Image
	BinaryForMaskImage(NORMAL_BINARY,DstImage,0,TRUE);

	// Make an Osd Region white
	if(bInvalidateOsdRegion)
	{
		InvalidateOsdRegion(DstImage);
	}

	//+add kwmoon 080828
	if(m_bInvalidateInvalidRegion)
	{
		InvalidateInvalidRegion(DstImage);
	}
	
	Progress.SetPos(100);

	return TRUE;
}

int	CImageProc::ExtPlane(BYTE* aBm,int** aImg,int aBmWid,int aBmHgt,int aPxlWid)
{

	int  l_i,l_j,l_k;
	int *lDstPtr;
	BYTE *lSrcPtr,*lSrcPtr0;
	int  lOff,lLineOff;

	lOff = 3;
	lLineOff = (aBmWid*3 + 3) & ~3;	
	
	for (l_i = 0; l_i < 3; l_i++)
	{
		lSrcPtr0 = aBm; 
		lDstPtr = *aImg;

		for (l_j = 0; l_j < aBmHgt; l_j++)
		{
			lSrcPtr = lSrcPtr0;
			for (l_k = 0; l_k < aBmWid; l_k++)
			{	
				*lDstPtr++ = (int)*lSrcPtr;
				lSrcPtr += lOff;
			}
			lSrcPtr0 += lLineOff;
		}

		aBm++;
		aImg++;
	}

	return 0;
}

int	CImageProc::DepPlane(int** aImg,BYTE* aBm,int aBmWid,int aBmHgt,int aPxlWid)
{

	int  l_i,l_j,l_k;
	int *lSrcPtr;
	BYTE *lDstPtr,*lDstPtr0;
	int  lOff,lLineOff;

	lOff = 3;
	lLineOff = (aBmWid*3 + 3) & ~3;
	
	for (l_i = 0; l_i < 3; l_i++)
	{
		lDstPtr0 = aBm; 
		lSrcPtr = *aImg;

		for (l_j = 0; l_j < aBmHgt; l_j++)
		{
			lDstPtr = lDstPtr0;
			for (l_k = 0; l_k < aBmWid; l_k++)
			{	
				*lDstPtr = (BYTE)*lSrcPtr++;
				lDstPtr += lOff;
			}
			lDstPtr0 += lLineOff;
		}

		aBm++;
		aImg++;
	}

	return 0;

}

int	CImageProc::Filter(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt,int aMaskWid,int aMaskHgt,int* aMask)
{

	int l_i,l_j,l_m,l_n;
	int* lSrc,*lSrc0,*lSrc1,*lSrc2;
	int* lDst,*lDst0,lVal;
	int* lMask;

	if (aMaskWid < 1 || aMaskHgt < 1) return -1;
	if (aBmWid < aMaskWid || aBmHgt < aMaskHgt) return -1;

	lSrc0 = aSrcImg + (aMaskHgt/2)*aBmWid + aMaskWid/2;
	lDst0 = aDstImg + (aMaskHgt/2)*aBmWid + aMaskWid/2;

	for (l_i = 0; l_i < aBmHgt-(aMaskHgt-1); l_i++)
	{
		lSrc = lSrc0;
		lDst = lDst0;

		for (l_j = 0; l_j < aBmWid-(aMaskWid-1); l_j++)
		{
			lMask = aMask;
			lSrc1 = (lSrc - (aMaskHgt/2)*aBmWid - aMaskWid/2);
			lVal = 0;
			for (l_m = 0; l_m < aMaskHgt; l_m++)
			{
				lSrc2 = lSrc1;
				for (l_n = 0; l_n < aMaskWid; l_n++)
					lVal += *lSrc2++ * *lMask++;
				lSrc1 += aBmWid;
			}
			*lDst++ = lVal >> 10;
			lSrc++;
		}
		lSrc0 += aBmWid;
		lDst0 += aBmWid;
	}

	return 0;

}

int	CImageProc::OuterFill(int* aSrcImg,int aBmWid,int aBmHgt,int aHor,int aVer,int aVal)
{

	int l_i,l_j;
	int* lSrc,*lSrc0;

	if (aBmWid < aHor*2 || aBmHgt < aVer*2) return 0;

	lSrc0 = aSrcImg;
	for (l_i = 0; l_i < aBmHgt; l_i++)
	{
		lSrc = lSrc0;
		for (l_j = 0; l_j < aHor; l_j++) *lSrc++ = aVal;
		lSrc0 += aBmWid;
	}

	lSrc0 = aSrcImg + aBmWid - aHor;
	for (l_i = 0; l_i < aBmHgt; l_i++)
	{
		lSrc = lSrc0;
		for (l_j = 0; l_j < aHor; l_j++) *lSrc++ = aVal;
		lSrc0 += aBmWid;
	}

	lSrc0 = aSrcImg;
	for (l_i = 0; l_i < aVer; l_i++)
	{
		lSrc = lSrc0;
		for (l_j = 0; l_j < aBmWid; l_j++) *lSrc++ = aVal;
		lSrc0 += aBmWid;
	}

	lSrc0 = aSrcImg + (aBmHgt-aVer)*aBmWid;
	for (l_i = 0; l_i < aVer; l_i++)
	{
		lSrc = lSrc0;
		for (l_j = 0; l_j < aBmWid; l_j++) *lSrc++ = aVal;
		lSrc0 += aBmWid;
	}

	return 0;
}


int	CImageProc::ConstThr(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt,int aThrl,int aThrh,int aInval,int aOutVal)
{
	int l_i,l_j;
	int* lSrc,*lSrc0,*lDst,*lDst0,lVal;

	if (aBmWid < 1 || aBmHgt < 1) return 0;

	lSrc0 = aSrcImg;
	lDst0 = aDstImg;

	for (l_i = 0; l_i < aBmHgt; l_i++)
	{
		lSrc = lSrc0;
		lDst = lDst0;

		for (l_j = 0; l_j < aBmWid; l_j++)
		{
			lVal = *lSrc++;
			if (lVal >= aThrl && lVal <= aThrh) 
				lVal = aInval;
			else
				lVal = aOutVal;	
			*lDst++ = lVal;
		}

		lSrc0 += aBmWid;
		lDst0 += aBmWid;
	}

	return 0;
}

int	CImageProc::Dilation(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt)
{

	int l_i,l_j;
	int* lSrc,*lSrc0;
	int* lDst,*lDst0,lVal;

	if (aBmWid < 3 || aBmHgt < 3) return -1;

	lSrc0 = aSrcImg;
	lDst0 = aDstImg;

	lSrc = lSrc0;
	lDst = lDst0;
	lVal  = *lSrc | *(lSrc+1);
	lVal |= *(lSrc+aBmWid) | *(lSrc+aBmWid+1);
	*lDst++ = lVal;
	lSrc++;
	for (l_j = 0; l_j < aBmWid-2; l_j++)
	{
		lVal  = *(lSrc-1) | *lSrc | *(lSrc+1);
		lVal |= *(lSrc+aBmWid-1) | *(lSrc+aBmWid) | *(lSrc+aBmWid+1);

		*lDst++ = lVal;
		lSrc++;
	}
	lVal  = *(lSrc-1) | *lSrc;
	lVal |= *(lSrc+aBmWid-1) | *(lSrc+aBmWid);
	*lDst = lVal;
	lSrc0 += aBmWid;
	lDst0 += aBmWid;

	for (l_i = 0; l_i < aBmHgt-2; l_i++)
	{
		lSrc = lSrc0;
		lDst = lDst0;
		lVal =  *(lSrc-aBmWid) | *(lSrc-aBmWid+1);
		lVal |= *lSrc | *(lSrc+1);
		lVal |= *(lSrc+aBmWid) | *(lSrc+aBmWid+1);
		*lDst++ = lVal;
		lSrc++;
		for (l_j = 0; l_j < aBmWid-2; l_j++)
		{
			lVal = *(lSrc-aBmWid-1) | *(lSrc-aBmWid) | *(lSrc-aBmWid+1);
			lVal |= *(lSrc-1) | *lSrc | *(lSrc+1);
			lVal |= *(lSrc+aBmWid-1) | *(lSrc+aBmWid) | *(lSrc+aBmWid+1);

			*lDst++ = lVal;
			lSrc++;
		}
		lVal = *(lSrc-aBmWid-1) | *(lSrc-aBmWid);
		lVal |= *(lSrc-1) | *lSrc | *(lSrc+1);
		lVal |= *(lSrc+aBmWid-1) | *(lSrc+aBmWid);

		*lDst++ = lVal;
		lSrc0 += aBmWid;
		lDst0 += aBmWid;
	}

	lSrc = lSrc0;
	lDst = lDst0;
	lVal = *(lSrc-aBmWid) | *(lSrc-aBmWid+1);
	lVal |= *lSrc | *(lSrc+1);
	*lDst++ = lVal;
	lSrc++;
	for (l_j = 0; l_j < aBmWid-2; l_j++)
	{
		lVal = *(lSrc-aBmWid-1) | *(lSrc-aBmWid) | *(lSrc-aBmWid+1);
		lVal |= *(lSrc-1) | *lSrc | *(lSrc+1);
		*lDst++ = lVal;
		lSrc++;
	}
	lVal = *(lSrc-aBmWid-1) | *(lSrc-aBmWid);
	lVal |= *(lSrc-1) | *lSrc;
	*lDst = lVal;

	return 0;
}

int	CImageProc::Xor(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt,int aVal)
{

	int l_i,l_j;
	int* lSrc,*lSrc0,*lDst,*lDst0,lVal;

	if (aBmWid < 1 || aBmHgt < 1) return 0;

	lSrc0 = aSrcImg;
	lDst0 = aDstImg;

	for (l_i = 0; l_i < aBmHgt; l_i++)
	{
		lSrc = lSrc0;
		lDst = lDst0;

		for (l_j = 0; l_j < aBmWid; l_j++)
		{
			lVal = *lSrc++;
			*lDst++ = lVal ^ aVal;
		}

		lSrc0 += aBmWid;
		lDst0 += aBmWid;
	}

	return 0;
}

int CImageProc::StackPush(int* stackX, int* stackY, int arr_size, int vx, int vy, int *top)
{
	// forbid overflow
	if(*top >= arr_size) return -1;

	(*top)++;

	stackX[*top] = vx;
	stackY[*top] = vy;

	return 1;
}

int CImageProc::StackPop(int* stackX, int* stackY, int* vx, int* vy, int *top)
{
	// if no data is left in the stack, return -1;
	if(*top == 0) return -1;

	*vx = stackX[*top];
	*vy = stackY[*top];

	(*top)--;

	return 1;
}

int CImageProc::Labeling(CBaseImage& SnapImage, CBaseImage& MaskImage, CBaseImage& ProcImage,CProgressCtrl& Progress)
{
	if((SnapImage.m_InfoHeader.biBitCount < 24)
	|| (MaskImage.m_InfoHeader.biBitCount < 24))
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_ABORT;
	}

	// Dimension of the Grab Image
	int nGrabWidth		= SnapImage.m_InfoHeader.biWidth;
	int nGrabHeight		= SnapImage.m_InfoHeader.biHeight;
	int nGrabBitCount	= SnapImage.m_InfoHeader.biBitCount;

	// Dimension of the Mask Image
	int nMaskWidth		= MaskImage.m_InfoHeader.biWidth;	 
	int nMaskHeight		= MaskImage.m_InfoHeader.biHeight;

	int nBytesInRow		= WIDTHBYTES(nGrabBitCount*nGrabWidth); 
	
	if(SnapImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Grab Image is not loaded!"); return TEST_ABORT;
	}

	if(MaskImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Mask Image is not loaded!"); return TEST_ABORT;
	}
	
	if((nGrabWidth != nMaskWidth) || (nGrabHeight != nMaskHeight))
	{
		AfxMessageBox("The size of two images is different!"); return TEST_ABORT;
	}
	
	if(&SnapImage == &MaskImage)
	{
		AfxMessageBox("Mask Image can not be labeled!"); return TEST_ABORT;
	}

	if(&SnapImage == &ProcImage)
	{
		AfxMessageBox("Proc Image can not be labeled!"); return TEST_ABORT;
	}
	int nGroupNo	= 0;
	int nPixelIndex	= 0;
	int nArraySize	= nMaskWidth * nMaskHeight;

	int r=0,c=0,top=0;
	
	int* StackX = (int *)malloc(nMaskWidth * nMaskHeight * 4); 
	memset(StackX,0,SnapImage.m_InfoHeader.biSizeImage);

	int* StackY = (int *)malloc(nMaskWidth * nMaskHeight * 4); 
	memset(StackY,0,SnapImage.m_InfoHeader.biSizeImage);

	BYTE* LabelImage = (BYTE *)malloc(SnapImage.m_InfoHeader.biSizeImage); 
	memset(LabelImage,0,SnapImage.m_InfoHeader.biSizeImage);

	if(SnapImage.m_InfoHeader.biBitCount==24) // If the color depth of image is 24 bits  
	{
		for(int nRow=0; nRow<nMaskHeight; nRow++)
		{
			for(int nColumn=0; nColumn<nMaskWidth; nColumn++) 
			{ 
				nPixelIndex  = (nRow)*nBytesInRow+3*(nColumn); 
				
				// 이미 방문한 픽셀인 경우, Skip함
				if((LabelImage[nPixelIndex] != 0) || (MaskImage.m_pImageData[nPixelIndex] != 0)) continue;

				r	= nRow;
				c	= nColumn;
				top = 0;
		
				nGroupNo++; // Valid Group No : 1 ~

				while(1)
				{
GRASSFIRE:
					for(int m=r-1; m<=r+1; m++)
					{
						for(int n=c-1; n<=c+1; n++)
						{
							// 대상영역을 벗어나면, Skip함
							if(m<0 || m>=nMaskHeight || n<0 || n>= nMaskWidth) continue;
		
							nPixelIndex = (m)*nBytesInRow+3*(n); 

							// 처음 방문한 픽셀인 경우
							if((LabelImage[nPixelIndex] == 0) && (MaskImage.m_pImageData[nPixelIndex] == 0))
							{
								LabelImage[nPixelIndex] = nGroupNo;

								if(StackPush(StackX,StackY,nArraySize,m,n,&top) == -1) continue;

								r	= m;
								c	= n;
					
								goto GRASSFIRE;
							}
						}
					}
					if(StackPop(StackX,StackY,&r,&c,&top) == -1)
					{
						break;
					}
				}
			}
		}
	}

	float fGrayGap = 250.0f/(float)nGroupNo;

	for(int nRow=0; nRow<nMaskHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nMaskWidth; nColumn++) 
		{ 
			nPixelIndex  = (nRow)*nBytesInRow+3*(nColumn); 

			if(MaskImage.m_pImageData[nPixelIndex] == 255)
			{
				ProcImage.m_pImageData[nPixelIndex]		= 255;
				ProcImage.m_pImageData[nPixelIndex+1]	= 255;
				ProcImage.m_pImageData[nPixelIndex+2]	= 255;
			}
			else
			{
				ProcImage.m_pImageData[nPixelIndex]		= (BYTE)(LabelImage[nPixelIndex]*fGrayGap);
				ProcImage.m_pImageData[nPixelIndex+1]	= (BYTE)(LabelImage[nPixelIndex]*fGrayGap);
				ProcImage.m_pImageData[nPixelIndex+2]	= (BYTE)(LabelImage[nPixelIndex]*fGrayGap);
			}
		}
	}

	if(StackX != NULL) delete StackX; StackX = NULL;
	if(StackY != NULL) delete StackY; StackY = NULL;
	if(LabelImage != NULL) delete LabelImage; LabelImage = NULL;

	return TEST_PASS;
}

int CImageProc::Repaint(CBaseImage& SnapImage, CBaseImage& MaskImage, CBaseImage& ProcImage,CProgressCtrl& Progress)
{
	if((SnapImage.m_InfoHeader.biBitCount < 24)
	|| (MaskImage.m_InfoHeader.biBitCount < 24))
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_ABORT;
	}

	// Dimension of the Grab Image
	int nGrabWidth		= SnapImage.m_InfoHeader.biWidth;
	int nGrabHeight		= SnapImage.m_InfoHeader.biHeight;
	int nGrabBitCount	= SnapImage.m_InfoHeader.biBitCount;
	int nImageSize		= SnapImage.m_InfoHeader.biSizeImage;

	// Dimension of the Mask Image
	int nMaskWidth		= MaskImage.m_InfoHeader.biWidth;	 
	int nMaskHeight		= MaskImage.m_InfoHeader.biHeight;

	// Dimension of the Mask Image
	int nProcWidth		= ProcImage.m_InfoHeader.biWidth;	 
	int nProcHeight		= ProcImage.m_InfoHeader.biHeight;

	int nBytesInRow		= WIDTHBYTES(nGrabBitCount*nGrabWidth); 
	
	int nImageBufferSize = 0;

	if(SnapImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Grab Image is not loaded!"); return TEST_ABORT;
	}

	if(MaskImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Mask Image is not loaded!"); return TEST_ABORT;
	}
	
	if((nGrabWidth != nMaskWidth) || (nGrabHeight != nMaskHeight))
	{
		AfxMessageBox("The size of two images is different!"); return TEST_ABORT;
	}
	
	if(&SnapImage == &MaskImage)
	{
		AfxMessageBox("Mask Image can not be repainted!"); return TEST_ABORT;
	}
	
	if(&SnapImage == &ProcImage)
	{
		AfxMessageBox("Proc Image can not be labeled!"); return TEST_ABORT;
	}

	if((nGrabWidth != nProcWidth) || (nGrabHeight != nProcHeight))
	{
		if(ProcImage.m_pImageData != NULL)
		{
			nImageBufferSize = _msize(ProcImage.m_pImageData);

			if(nImageBufferSize != nImageSize)
			{
				free(ProcImage.m_pImageData); ProcImage.m_pImageData = NULL;
				ProcImage.m_pImageData = (BYTE *)malloc(nImageSize);
			}
		}
		else
		{
			ProcImage.m_pImageData = (BYTE *)malloc(nImageSize);
		}
		memset(ProcImage.m_pImageData,0,nImageSize);

		ProcImage.InitHeaders(COLOR_24BIT,nGrabWidth,nGrabHeight,nImageSize);
	}

	for(int i=0; i<nImageSize; i++)
	{ 
		ProcImage.m_pImageData[i] = SnapImage.m_pImageData[i];
	}

	int nGroupNo	= 0;
	int nPixelIndex	= 0;
	int nArraySize	= nMaskWidth * nMaskHeight;

	int r=0,c=0,top=0;
	
	int* StackX = (int *)malloc(nMaskWidth * nMaskHeight * 4); 
	memset(StackX,0,SnapImage.m_InfoHeader.biSizeImage);

	int* StackY = (int *)malloc(nMaskWidth * nMaskHeight * 4); 
	memset(StackY,0,SnapImage.m_InfoHeader.biSizeImage);

	BYTE* LabelImage = (BYTE *)malloc(SnapImage.m_InfoHeader.biSizeImage); 
	memset(LabelImage,0,SnapImage.m_InfoHeader.biSizeImage);

	if(SnapImage.m_InfoHeader.biBitCount==24) // If the color depth of image is 24 bits  
	{
		for(int nRow=0; nRow<nMaskHeight; nRow++)
		{
			for(int nColumn=0; nColumn<nMaskWidth; nColumn++) 
			{ 
				nPixelIndex  = (nRow)*nBytesInRow+3*(nColumn); 

				// 이미 방문한 픽셀인 경우, Skip함
				if((LabelImage[nPixelIndex] != 0) || (MaskImage.m_pImageData[nPixelIndex] != 0)) continue;

				r	= nRow;
				c	= nColumn;
				top = 0;
		
				nGroupNo++; // Valid Group No : 1 ~

				while(1)
				{
GRASSFIRE:
					for(int m=r-1; m<=r+1; m++)
					{
						for(int n=c-1; n<=c+1; n++)
						{
							// 대상영역을 벗어나면, Skip함
							if(m<0 || m>=nMaskHeight || n<0 || n>= nMaskWidth) continue;
		
							nPixelIndex = (m)*nBytesInRow+3*(n); 

							// 처음 방문한 픽셀인 경우
							if((LabelImage[nPixelIndex] == 0) && (MaskImage.m_pImageData[nPixelIndex] == 0))
							{
								LabelImage[nPixelIndex] = nGroupNo;

								if(StackPush(StackX,StackY,nArraySize,m,n,&top) == -1) continue;

								r	= m;
								c	= n;
					
								goto GRASSFIRE;
							}
						}
					}

					//+add kwmoon 071210
					if(CurrentSet->bRunAbort) 
					{
						 return TEST_ABORT;
					}

					if(StackPop(StackX,StackY,&r,&c,&top) == -1) break;
				}
			}
		}
	}

	if(StackX != NULL) delete StackX; StackX = NULL;
	if(StackY != NULL) delete StackY; StackY = NULL;

	int nAvgRColor	= 0;
	int nAvgGColor	= 0;
	int nAvgBColor	= 0;
	int nNoGroupItem = 0;

	for(int i=1; i<=nGroupNo;i++)
	{
		nAvgRColor	 = 0;
		nAvgGColor	 = 0;
		nAvgBColor 	 = 0;
		nNoGroupItem = 0;

		for(int nRow=0; nRow<nMaskHeight; nRow++)
		{
			for(int nColumn=0; nColumn<nMaskWidth; nColumn++) 
			{ 
				nPixelIndex  = (nRow)*nBytesInRow+3*(nColumn); 

				if(LabelImage[nPixelIndex] == i)
				{
					nNoGroupItem++;

					nAvgRColor += SnapImage.m_pImageData[nPixelIndex+2];
					nAvgGColor += SnapImage.m_pImageData[nPixelIndex+1];
					nAvgBColor += SnapImage.m_pImageData[nPixelIndex];
				}
			}
		}

		nAvgRColor = (int)((float)nAvgRColor/(float)nNoGroupItem);
		nAvgGColor = (int)((float)nAvgGColor/(float)nNoGroupItem);
		nAvgBColor = (int)((float)nAvgBColor/(float)nNoGroupItem);

		for(int nRow=0; nRow<nMaskHeight; nRow++)
		{
			for(int nColumn=0; nColumn<nMaskWidth; nColumn++) 
			{ 
				nPixelIndex  = (nRow)*nBytesInRow+3*(nColumn); 

				if(LabelImage[nPixelIndex] == i)
				{
					ProcImage.m_pImageData[nPixelIndex+2]	= nAvgRColor;
					ProcImage.m_pImageData[nPixelIndex+1]	= nAvgGColor;
					ProcImage.m_pImageData[nPixelIndex]		= nAvgBColor;
				}
			}
		}
	}		
	
	if(LabelImage != NULL) delete LabelImage; LabelImage = NULL;

	return TEST_PASS;
}

BOOL CImageProc::RGB2Gray(int nType,CBaseImage& DstImage,BOOL bOverwrite)
{
	if(DstImage.m_InfoHeader.biBitCount < 24)
	{
		AfxMessageBox("Only true color image is supported!"); return TEST_FAIL;
	}
	
	int nProgress	= 0;
	int nColorDepth = DstImage.m_InfoHeader.biBitCount;
	int nBytesInRow = WIDTHBYTES(nColorDepth*DstImage.m_InfoHeader.biWidth); 
	
	float fRvalue = 0;
	float fGvalue = 0;
	float fBvalue = 0;

	int nGrayValue  = 0;

	int nImageBufferSize = 0;
	int nImageSize = 0;

	nImageSize = DstImage.m_InfoHeader.biSizeImage;

	if(DstImage.m_pProcessedImageData != NULL)
	{
		nImageBufferSize = _msize(DstImage.m_pProcessedImageData);

		if(nImageBufferSize != nImageSize)
		{
			free(DstImage.m_pProcessedImageData); DstImage.m_pProcessedImageData = NULL;
			DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
		}
	}
	else
	{
		DstImage.m_pProcessedImageData = (BYTE *)malloc(DstImage.m_BmInfo->bmiHeader.biSizeImage);
	}
	memset(DstImage.m_pProcessedImageData,0,DstImage.m_BmInfo->bmiHeader.biSizeImage);

	DstImage.m_bProcessedImageLoaded = FALSE;

	for(int nRow=0; nRow<DstImage.m_InfoHeader.biHeight; nRow++)
	{
		for(int nColumn=0; nColumn<DstImage.m_InfoHeader.biWidth; nColumn++) 
		{  
			fBvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn];  
			fGvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1]; 
			fRvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2]; 

			nGrayValue = (int)(fRvalue * 0.3 + fGvalue * 0.59 + fBvalue * 0.11);		

			DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn]	 = nGrayValue;  
			DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+1] = nGrayValue;	 
			DstImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2] = nGrayValue; 
		}
	}

	DstImage.m_bProcessedImageLoaded = TRUE;

	if(bOverwrite)
	{
		for(int j=0; j<(int)DstImage.m_InfoHeader.biSizeImage; j++)
		{
			DstImage.m_pImageData[j] = DstImage.m_pProcessedImageData[j];
		}
	}

	return TEST_PASS;
}

void CImageProc::ZoomOut(CBaseImage& SrcImage,float fZoomFactor)
{
	int nProgress = 0;

	int nHeight = SrcImage.m_InfoHeader.biHeight;
	int nWidth  = SrcImage.m_InfoHeader.biWidth;

	int nNewHeight = (int)(nHeight * fZoomFactor);
	int nNewWidth  = (int)(nWidth  * fZoomFactor);

	int nRowWidth	 = WIDTHBYTES(SrcImage.m_BmInfo->bmiHeader.biBitCount*nWidth);
	int nNewRowWidth = WIDTHBYTES(SrcImage.m_BmInfo->bmiHeader.biBitCount*nNewWidth);

	int nNewImageSize = nNewRowWidth * nNewHeight;

	int nRow	= 0;
	int nCol	= 0;

	float fRow	= 0;
	float fCol	= 0;

	BYTE nNewValue = 0;

	float fSr = 0.0f;
	float fSc = 0.0f;

	float fI1,fI2,fI3,fI4;

	BYTE* pImageData = (BYTE *)malloc(nNewImageSize); 
	memset(pImageData,0,nNewImageSize);

	for(int r=0;r<nNewHeight;r++)
	{
		for(int c=0;c<nNewWidth;c++)
		{
			for(int i=0; i<3; i++)
			{
				fRow = r/fZoomFactor;
				fCol = c/fZoomFactor;

				nRow = (int)floor(fRow);
				nCol = (int)floor(fCol);

				fSr = fRow - nRow;
				fSc = fCol - nCol;

				if((nRow<0) || (nRow>(nHeight-1)) 
				|| (nCol<0) || (nCol>(nWidth-1)))
				{
					SrcImage.m_pProcessedImageData[r*nNewWidth+c] = 0;
				}
				else
				{
					fI1 = (float)SrcImage.m_pImageData[nRow*nRowWidth+(3*nCol+i)];
					fI2 = (float)SrcImage.m_pImageData[nRow*nRowWidth+(3*(nCol+1)+i)];
					fI3 = (float)SrcImage.m_pImageData[(nRow+1)*nRowWidth+(3*(nCol+1)+i)];
					fI4 = (float)SrcImage.m_pImageData[(nRow+1)*nRowWidth+(3*nCol+i)];

					nNewValue = (BYTE)( fI1*(1-fSc)*(1-fSr) 
									  + fI2*fSc*(1-fSr)
									  + fI3*fSc*fSr 
									  + fI4*(1-fSc)*fSr);
					
					pImageData[r*nNewRowWidth+3*c+i] = nNewValue;
				}
			}
		}
	}

	memcpy(SrcImage.m_pImageData,pImageData,nNewImageSize);

	if(pImageData != NULL) delete pImageData; pImageData = NULL;

	SrcImage.InitHeaders(COLOR_24BIT,nNewWidth,nNewHeight,nNewImageSize);

	SrcImage.m_bImageLoaded = TRUE;
}

float CImageProc::Level2Percentage(int nLevel, int nNoLevel)
{
	//+change kwmoon 080716
//	return (float)floor((float)nLevel*100/(float)nNoLevel);
	return (float)((float)nLevel*100/(float)nNoLevel);
}

int CImageProc::Percentage2Level(float fPercent, int nNoLevel)
{
	//+change kwmoon 080716
//	return (int)floor(fPercent*0.01*(float)nNoLevel);
	return (int)(fPercent*0.01*(float)nNoLevel);
}

#define START_COLOR_LEVEL	25
#define END_COLOR_LEVEL		230

int	CImageProc::NoVideoSignalTest(CBaseImage& DstImage,CProgressCtrl& ItemProgress)
{
	if(DstImage.m_pImageData == NULL)
	{
		AfxMessageBox("Invalid Image!"); return TEST_ABORT;
	}

	CString szErrMsg	= _T("");
	CString szFilePath	= _T("");

	int nTestResult = TEST_PASS;
	
	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nImageColorDepth	= 0;
	int nBytesInRow			= 0; 
	int nRvalue				= 0;
	int nGvalue				= 0;
	int nBvalue				= 0;
	int nImageDataSize		= 0;
	int nImageBufferSize	= 0;

	// Check image size & Color depth
	nImageWidth			= DstImage.m_InfoHeader.biWidth;
	nImageHeight		= DstImage.m_InfoHeader.biHeight;
	nImageColorDepth	= DstImage.m_InfoHeader.biBitCount;

	nBytesInRow	= WIDTHBYTES(nImageColorDepth*nImageWidth); 

	nImageDataSize = nImageHeight * nBytesInRow;

	int nHistogram[3][256];
	int nImageSize = 0;

	for(int i=0; i<3; i++)
	{
		m_nNoPixelInMiddleRange[i]	= 0;
		m_nNoNonBlackPixel[i]		= 0;

		for(int j=0; j<256; j++)
		{
			nHistogram[i][j] = 0;
		}
	}

	if((m_nRoiWidth == 0) && (m_nRoiHeight == 0))
	{
		nImageSize = nImageHeight * nImageWidth - (m_nOsdWidth * m_nOsdHeight);
	}
	else
	{
		nImageSize = m_nRoiHeight * m_nRoiWidth;
	}

	int nStartXposition=0, nEndXposition=0, nStartYposition=0, nEndYposition=0;
	
	CalcTestRange(DstImage,nStartXposition, nEndXposition, nStartYposition, nEndYposition);

	for(int nRow=nStartYposition; nRow<=nEndYposition; nRow++)
	{
		for(int nColumn=nStartXposition; nColumn<=nEndXposition; nColumn++) 
		{

			nRvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2];
			nGvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1];
			nBvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn];
		
			if((m_nRoiWidth == 0) && (m_nRoiHeight == 0))
			{
				if((m_nOsdWidth  != 0) && (nColumn >= m_ptOsdPosition.x) && (nColumn <= (m_ptOsdPosition.x + m_nOsdWidth)) 
				&& (m_nOsdHeight != 0) && (nRow	  >= m_ptOsdPosition.y) && (nRow	<= (m_ptOsdPosition.y + m_nOsdHeight)))
				{
					continue;
				}
			}

			nHistogram[R_DATA][nRvalue]++;
			nHistogram[G_DATA][nGvalue]++;
			nHistogram[B_DATA][nBvalue]++;

			if(nRvalue > 10) m_nNoNonBlackPixel[R_DATA]++;
			if(nGvalue > 10) m_nNoNonBlackPixel[G_DATA]++;
			if(nBvalue > 10) m_nNoNonBlackPixel[B_DATA]++;
		}
	}

	int nPercentageNonBlackPixel	  = CurrentSet->nNonBlackPixel;
	int nPercentagePixelInMiddleRange = CurrentSet->nPixelInMiddleRange;

	for(int i=START_COLOR_LEVEL; i<END_COLOR_LEVEL; i++)
	{
		m_nNoPixelInMiddleRange[R_DATA] += nHistogram[R_DATA][i];
		m_nNoPixelInMiddleRange[G_DATA] += nHistogram[G_DATA][i];
		m_nNoPixelInMiddleRange[B_DATA] += nHistogram[B_DATA][i];
	}

	if((m_nNoNonBlackPixel[R_DATA] > (int)((float)nImageSize * nPercentageNonBlackPixel * 0.01))  
	|| (m_nNoNonBlackPixel[G_DATA] > (int)((float)nImageSize * nPercentageNonBlackPixel * 0.01))  
	|| (m_nNoNonBlackPixel[B_DATA] > (int)((float)nImageSize * nPercentageNonBlackPixel * 0.01))) 
	{
		if((m_nNoPixelInMiddleRange[R_DATA] > (int)((float)nImageSize * nPercentagePixelInMiddleRange * 0.01)) 
		|| (m_nNoPixelInMiddleRange[G_DATA] > (int)((float)nImageSize * nPercentagePixelInMiddleRange * 0.01)) 
		|| (m_nNoPixelInMiddleRange[B_DATA] > (int)((float)nImageSize * nPercentagePixelInMiddleRange * 0.01)))
		{
			return TEST_PASS;
		}
		else
		{
			return TEST_FAIL;
		}
	}
	else
	{
		return TEST_FAIL;
	}
}

void CImageProc::SetAllowedErrorPixel(float fAllowedErrorPixelPercentage[])
{
	for(int i=0; i<5; i++)
	{
		m_fAllowedBandErrorPixelPercentage[i] = fAllowedErrorPixelPercentage[i];
	}
}

int	CImageProc::CaptionTest(CBaseImage& SnapImage, CBaseImage& RefImage, CBaseImage& MaskImage, int nLowerLimit, int nUpperLimit, CProgressCtrl& ItemProgress)
{
	if((SnapImage.m_pImageData == NULL) || (RefImage.m_pImageData == NULL))
	{
		AfxMessageBox("Invalid Image!"); return TEST_ABORT;
	}

	CString szErrMsg	= _T("");
	CString szFilePath	= _T("");

	int nTestResult = TEST_PASS;
	
	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nImageColorDepth	= 0;

	int nBytesInRow	= 0; 

//	int nRvalue	= 0;
//	int nGvalue	= 0;
//	int nBvalue	= 0;
	int nValue	= 0;

//	int nNoNonBlackRvalue	= 0;
//	int nNoNonBlackGvalue	= 0;
//	int nNoNonBlackBvalue	= 0;
	int nNoNonBlackValue	= 0;

	int nImageSize	= 0;
	int nImageBufferSize = 0;
	int nMaskValue = 0;

	// Check image size & Color depth
	nImageWidth			= SnapImage.m_InfoHeader.biWidth;
	nImageHeight		= SnapImage.m_InfoHeader.biHeight;
	nImageColorDepth	= SnapImage.m_InfoHeader.biBitCount;

	nBytesInRow	= WIDTHBYTES(nImageColorDepth*nImageWidth); 

	nImageSize = nImageHeight * nBytesInRow;

	//+change kwmoon 071112
	BinaryForDiffImage(NORMAL_BINARY,SnapImage,RefImage,nLowerLimit,nUpperLimit);

	for(int nRow=0; nRow<nImageHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nImageWidth; nColumn++) 
		{  
			if(MaskImage.m_bImageLoaded)
			{
				nMaskValue =  MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn] 
							| MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1] 
							| MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2]; 
			}

			//+add kwmoon 080213
			if((m_nOsdWidth  != 0) && (nColumn >= m_ptOsdPosition.x) && (nColumn <= (m_ptOsdPosition.x + m_nOsdWidth)) 
			&& (m_nOsdHeight != 0) && (nRow	  >= m_ptOsdPosition.y) && (nRow	<= (m_ptOsdPosition.y + m_nOsdHeight)))
			{
				continue;
			}
			//-
			
			if(nMaskValue == 0)
			{
			//	nRvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2];
			//	nGvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1];
			//	nBvalue = DstImage.m_pImageData[nRow*nBytesInRow+3*nColumn];
				nValue = SnapImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2];
			
			//	if(nRvalue != 0) nNoNonBlackRvalue++;
			//	if(nGvalue != 0) nNoNonBlackGvalue++;
			//	if(nBvalue != 0) nNoNonBlackBvalue++;
				if(nValue != 0) nNoNonBlackValue++;
			}
		}
	}

//	if((nNoNonBlackRvalue > (int)((float)(nImageHeight * nImageWidth) * 0.1))
//	|| (nNoNonBlackGvalue > (int)((float)(nImageHeight * nImageWidth) * 0.1))
//	|| (nNoNonBlackBvalue > (int)((float)(nImageHeight * nImageWidth) * 0.1)))
	if(nNoNonBlackValue > (int)((float)(m_nCaptionWidth * m_nCaptionHeight) * 0.4))
	{
		return TEST_PASS;
	}
	else
	{
		return TEST_FAIL;
	}
}
//+add kwmoon 080121
// Set Osd Region 
void CImageProc::SetOsdRegion(POINT ptOsd1Position,POINT ptOsd2Position)
{
	m_ptOsdPosition.x = ptOsd1Position.x;
	m_ptOsdPosition.y = ptOsd1Position.y;
	m_nOsdWidth		  = ptOsd2Position.x - ptOsd1Position.x;
	m_nOsdHeight	  = ptOsd2Position.y - ptOsd1Position.y;
}

//+add kwmoon 080828
void CImageProc::SetInvalidRegion(POINT pt1Position,POINT pt2Position)
{
	m_ptInvalidPosition.x = pt1Position.x;
	m_ptInvalidPosition.y = pt1Position.y;
	m_nInvalidWidth		  = pt2Position.x - pt1Position.x;
	m_nInvalidHeight	  = pt2Position.y - pt1Position.y;
}

//+add kwmoon 080121
int	CImageProc::InvalidateOsdRegion(CBaseImage& MaskImage)
{
	if((MaskImage.m_pImageData == NULL))
	{
		AfxMessageBox("Invalid Image!"); return TEST_ABORT;
	}

	int nTestResult			= TEST_PASS;
	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nImageColorDepth	= 0;
	int nBytesInRow			= 0; 
	int	nImageSize			= 0;

	CString szErrorMsg		= _T("");

	// Check image size & Color depth
	nImageWidth			= MaskImage.m_InfoHeader.biWidth;
	nImageHeight		= MaskImage.m_InfoHeader.biHeight;
	nImageColorDepth	= MaskImage.m_InfoHeader.biBitCount;

	nBytesInRow	= WIDTHBYTES(nImageColorDepth*nImageWidth); 
	
	nImageSize	= nImageHeight * nBytesInRow;

	if((m_nOsdWidth == 0) || (m_nOsdHeight == 0))
	{
		return TEST_PASS;

		//+add kwmoon 080409
		//SetDefaultOsdRegion(nImageWidth,nImageHeight);
	}

	if((m_ptOsdPosition.x < 0) || ((m_ptOsdPosition.x + m_nOsdWidth ) >= nImageWidth ) 
	|| (m_ptOsdPosition.y < 0) || ((m_ptOsdPosition.y + m_nOsdHeight) >= nImageHeight))
	{
		szErrorMsg.Format("Wrong OSD coordinates (%d,%d,%d,%d)",
						   m_ptOsdPosition.x,m_ptOsdPosition.y,m_ptOsdPosition.x+m_nOsdWidth,m_ptOsdPosition.y+m_nOsdHeight);
		
		AfxMessageBox(szErrorMsg); return TEST_ABORT;
	}

	int	nStartYposition = GetOppositeStartYposition(m_ptOsdPosition.y,m_nOsdHeight,nImageHeight);
	int	nEndYposition   = nStartYposition + m_nOsdHeight;

	for(int nRow=0; nRow<nImageHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nImageWidth; nColumn++) 
		{  
			if((nColumn >= m_ptOsdPosition.x) && (nColumn <= m_ptOsdPosition.x + m_nOsdWidth)
			&& (nRow	>= nStartYposition)   && (nRow	  <= nEndYposition))
			{
				MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn]		= 255; 
				MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1]	= 255; 
				MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2]	= 255; 
			}
		}
	}

	return TEST_PASS;
}

//+add kwmoon 080828
int	CImageProc::InvalidateInvalidRegion(CBaseImage& MaskImage)
{
	if((MaskImage.m_pImageData == NULL))
	{
		AfxMessageBox("Invalid Image!"); return TEST_ABORT;
	}

	int nTestResult			= TEST_PASS;
	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nImageColorDepth	= 0;
	int nBytesInRow			= 0; 
	int	nImageSize			= 0;

	CString szErrorMsg		= _T("");

	// Check image size & Color depth
	nImageWidth			= MaskImage.m_InfoHeader.biWidth;
	nImageHeight		= MaskImage.m_InfoHeader.biHeight;
	nImageColorDepth	= MaskImage.m_InfoHeader.biBitCount;

	nBytesInRow	= WIDTHBYTES(nImageColorDepth*nImageWidth); 
	
	nImageSize	= nImageHeight * nBytesInRow;

	if((m_nInvalidWidth == 0) || (m_nInvalidHeight == 0))
	{
		return TEST_PASS;

		//+add kwmoon 080409
	//	SetDefaultOsdRegion(nImageWidth,nImageHeight);
	}

	if((m_ptInvalidPosition.x < 0) || ((m_ptInvalidPosition.x + m_nInvalidWidth ) >= nImageWidth ) 
	|| (m_ptInvalidPosition.y < 0) || ((m_ptInvalidPosition.y + m_nInvalidHeight) >= nImageHeight))
	{
		szErrorMsg.Format("Wrong Invalid coordinates (%d,%d,%d,%d)",
						   m_ptInvalidPosition.x,
						   m_ptInvalidPosition.y,
						   m_ptInvalidPosition.x+m_nInvalidWidth,
						   m_ptInvalidPosition.y+m_nInvalidHeight);
		
		AfxMessageBox(szErrorMsg); return TEST_ABORT;
	}

	int	nStartYposition = GetOppositeStartYposition(m_ptInvalidPosition.y,m_nInvalidHeight,nImageHeight);
	int	nEndYposition   = nStartYposition + m_nInvalidHeight;

	for(int nRow=0; nRow<nImageHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nImageWidth; nColumn++) 
		{  
			if((nColumn >= m_ptInvalidPosition.x) && (nColumn <= m_ptInvalidPosition.x + m_nInvalidWidth)
			&& (nRow	>= nStartYposition)   && (nRow	  <= nEndYposition))
			{
				MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn]		= 255; 
				MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1]	= 255; 
				MaskImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2]	= 255; 
			}
		}
	}

	return TEST_PASS;
}

//+add kwmoon 080121
int	CImageProc::SetDefaultOsdRegion(int nImageWidth,int nImageHeight)
{
	if((nImageWidth == 852)	&& (nImageHeight == 480))
	{
		m_ptOsdPosition.x	=	0;
		m_ptOsdPosition.y	=	0;
		m_nOsdWidth			= 	0;
		m_nOsdHeight		= 	0;
	}
	else if((nImageWidth == 1024) && (nImageHeight == 768))
	{
		m_ptOsdPosition.x	=	0;
		m_ptOsdPosition.y	=	0;
		m_nOsdWidth			= 	0;
		m_nOsdHeight		= 	0;
	}
	else if((nImageWidth == 1280) && (nImageHeight == 768))
	{
		m_ptOsdPosition.x	=	0;
		m_ptOsdPosition.y	=	0;
		m_nOsdWidth			= 	0;
		m_nOsdHeight		= 	0;
	}
	else if((nImageWidth == 1280) && (nImageHeight == 1024))
	{
		m_ptOsdPosition.x	=	0;
		m_ptOsdPosition.y	=	0;
		m_nOsdWidth			= 	0;
		m_nOsdHeight		= 	0;
	}
	else if((nImageWidth == 1366) && (nImageHeight == 768))
	{
		m_ptOsdPosition.x	= 900;
		m_ptOsdPosition.y	= 20;
		m_nOsdWidth			= 450;
		m_nOsdHeight		= 300;
	}
	else if((nImageWidth == 1440) && (nImageHeight == 900))
	{
		m_ptOsdPosition.x	=	0;
		m_ptOsdPosition.y	=	0;
		m_nOsdWidth			= 	0;
		m_nOsdHeight		= 	0;
	}
	else if((nImageWidth == 1680) && (nImageHeight == 1050))
	{
		m_ptOsdPosition.x	= 1300;
		m_ptOsdPosition.y	= 20;
		m_nOsdWidth			= 370;
		m_nOsdHeight		= 250;
	}
	else if((nImageWidth == 1920) && (nImageHeight == 1080))
	{
		m_ptOsdPosition.x	= 1200;
		m_ptOsdPosition.y	= 20;
		m_nOsdWidth			= 700;
		m_nOsdHeight		= 450;
	}
	else
	{
		m_ptOsdPosition.x	=	0;
		m_ptOsdPosition.y	=	0;
		m_nOsdWidth			= 	0;
		m_nOsdHeight		= 	0;
	}

	return TRUE;
}

//+add kwmoon 080716
int	CImageProc::CheckImageAttribute(CBaseImage& SnapImage, CBaseImage& RefImage,CBaseImage& MaskImage)
{
	int nRefWidth   = RefImage.m_InfoHeader.biWidth;	 
	int nRefHeight  = RefImage.m_InfoHeader.biHeight;
	int nSnapWidth  = SnapImage.m_InfoHeader.biWidth;	 
	int nSnapHeight = SnapImage.m_InfoHeader.biHeight;
	int nMaskWidth  = 0;
	int nMaskHeight = 0;

	CString szErrMsg	 = _T("");
	CString sRefFileName = _T("");
	CString	szFileName	 = _T("");

	if((m_nRoiHeight > 0) && (m_nRoiWidth > 0))
	{
		m_nNoRoiPixel = m_nRoiWidth * m_nRoiHeight;
	}
	else
	{
		m_nNoRoiPixel = nRefWidth * nRefHeight;
	}

	if((SnapImage.m_InfoHeader.biBitCount != 24) || (RefImage.m_InfoHeader.biBitCount  != 24))
	{
		AfxMessageBox("Only 24Bit color image is supported!"); return TEST_ABORT;
	}
	
	if(SnapImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Snap Image is not loaded!"); return TEST_ABORT;
	}

	if(RefImage.m_bImageLoaded == FALSE)
	{
		AfxMessageBox("Ref Image is not loaded!"); return TEST_ABORT;
	}
	
	if((nRefWidth != nSnapWidth) || (nRefHeight != nSnapHeight))
	{
		szErrMsg.Format("[Image Size Error] Grab(W:%d,H%d), Ref(W:%d,H%d)",
						nSnapWidth,nSnapHeight,nRefWidth,nRefHeight);

		// Load a Reference Image
		if(RefImage.m_szFilePath != _T(""))
		{
			szFileName = RefImage.m_szFilePath.Right(RefImage.m_szFilePath.GetLength()-RefImage.m_szFilePath.ReverseFind('\\')-1);

			//+change kwmoon 080807
		//	if(!RefImage_DirectLoad(RefImage.m_szFilePath,0))
			if(!RefImage_DirectLoad(szFileName,0))
			{
				AfxMessageBox(szErrMsg); return TEST_ABORT;
			}
		}
	}

	if(MaskImage.m_bImageLoaded)
	{
		nMaskWidth  = MaskImage.m_InfoHeader.biWidth;	 
		nMaskHeight = MaskImage.m_InfoHeader.biHeight;
	}

	if((nRefWidth != nSnapWidth) || (nRefHeight != nSnapHeight))
	{
		szErrMsg.Format("[Image Size Error] Grab(W:%d,H%d), Ref(W:%d,H%d)",
						nSnapWidth,nSnapHeight,nRefWidth,nRefHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}
	
	if(RefImage.m_InfoHeader.biBitCount != SnapImage.m_InfoHeader.biBitCount)
	{
		AfxMessageBox("The color depth of two images is different!"); return TEST_ABORT;
	}
	
	if((m_nRoiWidth >= nSnapWidth)	|| (m_nRoiHeight >= nSnapHeight))
	{
		szErrMsg.Format("[ROI Error] Roi with & height should be less than witdh & height of image! (W:%d_%d,H:%d_%d)"
					   ,m_nRoiWidth,nSnapWidth,m_nRoiHeight,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if((m_ptRoiPosition.x >= (nSnapWidth-1)) || (m_ptRoiPosition.y >= (nSnapHeight-1))) 
	{
		szErrMsg.Format("[ROI Error] Roi (x,y) should be less than witdh & height of image! (X:%d,Y:%d,W:%d,H:%d)"
					   ,m_ptRoiPosition.x,m_ptRoiPosition.y,nSnapWidth,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if((m_ptRoiPosition.x >= (nSnapWidth-1)) || (m_ptRoiPosition.y >= (nSnapHeight-1))) 
	{
		szErrMsg.Format("[ROI Error] Roi (x,y) should be less than witdh & height of image! (X:%d,Y:%d,W:%d,H:%d)"
					   ,m_ptRoiPosition.x,m_ptRoiPosition.y,nSnapWidth,nSnapHeight);

		AfxMessageBox(szErrMsg); return TEST_ABORT;
	}

	if(MaskImage.m_bImageLoaded)
	{
		if((nRefWidth != nMaskWidth) || (nRefHeight != nMaskHeight))
		{
			szErrMsg.Format("[Image Size Error] Ref(W:%d,H%d), Mask(W:%d,H%d)",
							nRefWidth,nRefHeight,nMaskWidth,nMaskHeight);

			AfxMessageBox(szErrMsg); return TEST_ABORT;
		}
	}
	return TEST_PASS;
}

//+add kwmoon 080722
int	CImageProc::ColorBiasTest(CBaseImage& SnapImage, int &nDifference,CString& szResultInfo, CProgressCtrl& ItemProgress)
{

	if((SnapImage.m_pImageData == NULL))
	{
		AfxMessageBox("Invalid Image!"); return TEST_ABORT;
	}

	
	CString szErrMsg		= _T("");
	CString szFilePath		= _T("");

	int nTestResult			= TEST_PASS;
	
	int nSnapWidth			= SnapImage.m_InfoHeader.biWidth;
	int nSnapHeight			= SnapImage.m_InfoHeader.biHeight;
	int nColorDepth			= SnapImage.m_InfoHeader.biBitCount;
	int nBytesInRow			= WIDTHBYTES(nColorDepth*nSnapWidth); 

	int nMinColorValue		= 0;
	int nMaxColorValue		= 0;

	int nLowerLimit			= 0;
	int nUpperLimit			= 0;

	//+change kwmoon 080722
//	int nNoLevel			= (int)pow(2,(nColorDepth/3));
//	int nNoLevel			= (int)pow(2,(nColorDepth/3))-1;
	int nNoPixel			= 0;

	int nRvalue=0, nGvalue=0, nBvalue=0;
	int nAvgRvalue=0, nAvgGvalue=0, nAvgBvalue=0;

	int nYposition			= 0;

	//+del 
	nLowerLimit = (int)m_fLowerLimit;//Percentage2Level(m_fLowerLimit,nNoLevel);
	nUpperLimit = (int)m_fUpperLimit;//Percentage2Level(m_fUpperLimit,nNoLevel);


	//+add kwmoon 080618
	int nStartXposition=0, nEndXposition=0, nStartYposition=0, nEndYposition=0;
	
	CalcTestRange(SnapImage,nStartXposition, nEndXposition, nStartYposition, nEndYposition);

	
	for(int nRow=nStartYposition; nRow<=nEndYposition; nRow++)
	{
		for(int nColumn=nStartXposition; nColumn<=nEndXposition; nColumn++) 
		{

			nRvalue = SnapImage.m_pImageData[nRow*nBytesInRow+3*nColumn+2];
			nGvalue = SnapImage.m_pImageData[nRow*nBytesInRow+3*nColumn+1];
			nBvalue = SnapImage.m_pImageData[nRow*nBytesInRow+3*nColumn];
		
			//+add kwmoon 080618
			if((m_nRoiWidth == 0) && (m_nRoiHeight == 0))
			{
				//+add kwmoon 080129
				if((m_nOsdWidth  != 0) && (nColumn >= m_ptOsdPosition.x) && (nColumn <= (m_ptOsdPosition.x + m_nOsdWidth)) 
				&& (m_nOsdHeight != 0) && (nRow	  >= m_ptOsdPosition.y) && (nRow	<= (m_ptOsdPosition.y + m_nOsdHeight)))
				{
					continue;
				}
				//-
			}
			nNoPixel++;

			nAvgRvalue += nRvalue;
			nAvgGvalue += nGvalue;
			nAvgBvalue += nBvalue;

		//	if(nRvalue || nGvalue || nBvalue)
		//	{
		//		nYposition = GetOppositeStartYposition(nRow,0,nSnapHeight);
		//	}
		}
	}

	nAvgRvalue = nAvgRvalue/nNoPixel;
	nAvgGvalue = nAvgGvalue/nNoPixel;
	nAvgBvalue = nAvgBvalue/nNoPixel;


	nMaxColorValue = GetMaxValue(nAvgRvalue,nAvgGvalue,nAvgBvalue); 
	nMinColorValue = GetMinValue(nAvgRvalue,nAvgGvalue,nAvgBvalue); 

	nDifference = abs(nMaxColorValue - nMinColorValue);

	szResultInfo.Format("R:%d,G:%d,B:%d",nAvgRvalue,nAvgGvalue,nAvgBvalue);

	if(nDifference > nUpperLimit)
	{
		return TEST_FAIL;
	}

	return TEST_PASS;
}

//+change 090218(Modification No1)
BYTE g_byHisto[256*256*256];

//+add 090130
int	CImageProc::NoVideoSignalTest2(CBaseImage& DstImage,int ColNumThre, int* pColNum, int nStep)
{
	if(DstImage.m_pImageData == NULL)
	{
		AfxMessageBox("Invalid Image!"); return TEST_ABORT;
	}

	CString szErrMsg	= _T("");
	CString szFilePath	= _T("");

	int nTestResult = TEST_PASS;
	
	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nImageColorDepth	= 0;
	int nBytesInRow			= 0; 
	int nRvalue				= 0;
	int nGvalue				= 0;
	int nBvalue				= 0;
	int nImageDataSize			= 0;
	int nImageBufferSize	= 0;

	// Check image size & Color depth
	nImageWidth			= DstImage.m_InfoHeader.biWidth;
	nImageHeight		= DstImage.m_InfoHeader.biHeight;
	nImageColorDepth	= DstImage.m_InfoHeader.biBitCount;

	nBytesInRow	= WIDTHBYTES(nImageColorDepth*nImageWidth); 

	nImageDataSize = nImageHeight * nBytesInRow;

	int nImageSize = 0;
	int i;
	int maxcolor = 256*256*256;
	int ridx, cidx, RGBVal;
	int nCount = 0;


	memset(g_byHisto,0,256*256*256);
		
	for(int nRow=0; nRow<nImageHeight; nRow+=nStep)
	{
		ridx = nRow*nBytesInRow;

		for(int nColumn=0; nColumn<nImageWidth; nColumn+=nStep) 
		{
			cidx = ridx + 3*nColumn;

			nRvalue = DstImage.m_pImageData[cidx+2];
			nGvalue = DstImage.m_pImageData[cidx+1];
			nBvalue = DstImage.m_pImageData[cidx];

			RGBVal = (nRvalue<<16)|(nGvalue<<8)|nBvalue;

			g_byHisto[RGBVal] = 1;
/*
			if(Histo[RGBVal])
			{
				nCount -= Histo[RGBVal]-1;
			}
			else 
			{
				Histo[RGBVal]++; 
				nCount++;
			}
*/
		}
	}

	for(int i=0; i<(256*256*256); i++)
	{
		if(g_byHisto[i] == 1) nCount++;
	}

	*pColNum = nCount;

	if(nCount > ColNumThre)
	{
		return TEST_PASS;
	}
	else
	{
		return TEST_FAIL;
	}
}
	//+add 101123
int CImageProc::CopyImage(CBaseImage& SrcImage, CBaseImage& DestImage)
{
	// Dimension of the Grab Image
	int nGrabWidth		= SrcImage.m_InfoHeader.biWidth;
	int nGrabHeight		= SrcImage.m_InfoHeader.biHeight;
	int nGrabBitCount	= SrcImage.m_InfoHeader.biBitCount;
	int nImageSize		= SrcImage.m_InfoHeader.biSizeImage;


/*	if(DestImage.m_pImageData != NULL)
	{
		free(DestImage.m_pImageData); DestImage.m_pImageData = NULL;
	}

	DestImage.m_pImageData = (BYTE *)malloc(nImageSize);
*/
	memset(DestImage.m_pImageData,0,nImageSize);

	DestImage.InitHeaders(COLOR_24BIT,nGrabWidth,nGrabHeight,nImageSize);

	for(int i=0; i<nImageSize; i++)
	{ 
		DestImage.m_pImageData[i] = SrcImage.m_pImageData[i];
	}


	return TEST_PASS;
}

int	CImageProc::CheckImageDiff(CBaseImage& SnapImage, CBaseImage& RefImage, int nLowerLimit, int nUpperLimit)
{
	if((SnapImage.m_pImageData == NULL) || (RefImage.m_pImageData == NULL))
	{
		AfxMessageBox("Invalid Image!"); return TEST_ABORT;
	}

	CString szErrMsg	= _T("");
	CString szFilePath	= _T("");

	int nTestResult = TEST_PASS;
	
	int nImageHeight		= 0;
	int nImageWidth			= 0;
	int nImageColorDepth	= 0;

	int nBytesInRow	= 0; 

	int nValue	= 0;

	int nNoNonBlackValue	= 0;

	int nImageSize	= 0;
	int nImageBufferSize = 0;
	int nMaskValue = 0;

	// Check image size & Color depth
	nImageWidth			= SnapImage.m_InfoHeader.biWidth;
	nImageHeight		= SnapImage.m_InfoHeader.biHeight;
	nImageColorDepth	= SnapImage.m_InfoHeader.biBitCount;

	nBytesInRow	= WIDTHBYTES(nImageColorDepth*nImageWidth); 

	nImageSize = nImageHeight * nBytesInRow;

	//+change kwmoon 071112
	BinaryForDiffImage(NORMAL_BINARY,SnapImage,RefImage,nLowerLimit,nUpperLimit);

	for(int nRow=0; nRow<nImageHeight; nRow++)
	{
		for(int nColumn=0; nColumn<nImageWidth; nColumn++) 
		{  
			if((m_nOsdWidth  != 0) && (nColumn >= m_ptOsdPosition.x) && (nColumn <= (m_ptOsdPosition.x + m_nOsdWidth)) 
			&& (m_nOsdHeight != 0) && (nRow	  >= m_ptOsdPosition.y) && (nRow	<= (m_ptOsdPosition.y + m_nOsdHeight)))
			{
				continue;
			}
			
			nValue = SnapImage.m_pProcessedImageData[nRow*nBytesInRow+3*nColumn+2];

			if(nValue != 0) nNoNonBlackValue++;
		}
	}

	if(nNoNonBlackValue > (int)((float)(nImageWidth * nImageHeight) * m_fMovingPicTest_Limit))
	{
		return TEST_PASS;
	}
	else
	{
		return TEST_FAIL;
	}
}

void CImageProc::SetMovingPicTestMargin(int nDiff_LoLimit, int nDiff_HiLimit, int nTest_Limit)
{
	m_nDiff_LowerLimit = nDiff_LoLimit;
	m_nDiff_UpperLimit = nDiff_HiLimit;
	m_fMovingPicTest_Limit = (double)(nTest_Limit*0.01);
}
void CImageProc::InitMovingPicTestMargin()
{
	m_nDiff_LowerLimit = -20;
	m_nDiff_UpperLimit = 20;
	m_fMovingPicTest_Limit = 0.2;
}
