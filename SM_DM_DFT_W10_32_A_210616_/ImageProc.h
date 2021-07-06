// ImageProc.h: interface for the CImageProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROC_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_)
#define AFX_IMAGEPROC_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"
#include "BaseImage.h"
#include "Defines.h"
#include <afxtempl.h>

#define MAX_NO_COLOR_BITS	12

class CLineBitTestData
{

public:
	int	m_nNoSkippedSignalLineLowBits;
	
	int m_nBitCount;
	int m_nTotalNoMaskedPixel;
	int m_nTotalNoUnmaskedPixel;

	int m_nErrorColor;
	int m_nErrorBitPosition;

	CBaseImage*	m_pGrabImage;
	CBaseImage*	m_pRefImage;
	CBaseImage*	m_pMaskImage;

	int m_aRefNoHighBit[3][MAX_NO_COLOR_BITS];
	int m_aGrabNoHighBit[3][MAX_NO_COLOR_BITS];
	int m_aRefNoHighBitPair[3][MAX_NO_COLOR_BITS-1];
	int m_aGrabNoHighBitPair[3][MAX_NO_COLOR_BITS-1];
	int m_aRefNoLowBitPair[3][MAX_NO_COLOR_BITS-1];
	int m_aGrabNoLowBitPair[3][MAX_NO_COLOR_BITS-1];

	// High Bit 
	int nPlusColorType;
	int nPlusBitPosition;
	int nPlusMaxDifference;
	float fPlusDifferencePercentage;

	int nMinusColorType;
	int nMinusBitPosition;
	int nMinusMaxDifference;
	float fMinusDifferencePercentage;

	int nTotalNoErrorBitLine;
/*	
	// Low Bit Pair
	int nPlusLowBitPairColorType;
	int nPlusLowBitPairBitPosition;
	int nPlusMaxNoLowBitPair;
	float fPlusLowBitPairDifferencePercentage;
	
	int nMinusLowBitPairColorType;
	int nMinusLowBitPairBitPosition;
	int nMinusMaxNoLowBitPair;
	float fMinusLowBitPairDifferencePercentage;
	
	int nTotalNoErrorLowBitLine;
	
	// High Bit Pair
	int nPlusHighBitPairColorType;
	int nPlusHighBitPairBitPosition;
	int nMaxNoPlusHighBitPair;
	float fPlusHighBitPairDifferencePercentage;

	int nMinusHighBitPairColorType;
	int nMinusHighBitPairBitPosition;
	int nMaxNoMinusHighBitPair;
	float fMinusHighBitPairDifferencePercentage;
	
	int nTotalNoErrorHighBitLine;
*/
public:

	CLineBitTestData()
	{
		InitBitTestData();
	};

	~CLineBitTestData(){};

	void InitBitTestData()
	{
		m_nBitCount = 0;
		m_nNoSkippedSignalLineLowBits = 0;
		m_nTotalNoMaskedPixel	= 0;
		m_nTotalNoUnmaskedPixel	= 0;

		m_nErrorColor			= 0;
		m_nErrorBitPosition		= 0;

		m_pGrabImage	= NULL;
		m_pRefImage		= NULL;
		m_pMaskImage	= NULL;

		// High Bit 
		nPlusColorType				 = 0;
		nPlusBitPosition			 = 0;
		nPlusMaxDifference			 = 0;
		fPlusDifferencePercentage	 = 0;
		
		nMinusColorType				 = 0;
		nMinusBitPosition			 = 0;
		nMinusMaxDifference			 = 0;
		fMinusDifferencePercentage	 = 0;
	
		nTotalNoErrorBitLine		 = 0;
/*		
		// Low Bit Pair
		nPlusLowBitPairColorType	 = 0;
		nPlusLowBitPairBitPosition	 = 0;
		nPlusMaxNoLowBitPair		 = 0;
		fPlusLowBitPairDifferencePercentage		= 0;
		
		nMinusLowBitPairColorType	 = 0;
		nMinusLowBitPairBitPosition	 = 0;
		nMinusMaxNoLowBitPair		 = 0;
		fMinusLowBitPairDifferencePercentage	= 0;
		
		nTotalNoErrorLowBitLine		 = 0;
	
		// High Bit Pair
		nPlusHighBitPairColorType	 = 0;
		nPlusHighBitPairBitPosition	 = 0;
		nMaxNoPlusHighBitPair		 = 0;
		fPlusHighBitPairDifferencePercentage	= 0;

		nMinusHighBitPairColorType	 = 0;
		nMinusHighBitPairBitPosition = 0;
		nMaxNoMinusHighBitPair		 = 0;
		fMinusHighBitPairDifferencePercentage	= 0;
		
		nTotalNoErrorHighBitLine	 = 0;
*/
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<MAX_NO_COLOR_BITS; j++)
			{
				m_aRefNoHighBit[i][j]	= 0;
				m_aGrabNoHighBit[i][j]	= 0;

				if(j < MAX_NO_COLOR_BITS-1)
				{
					m_aRefNoHighBitPair[i][j]	= 0;
					m_aGrabNoHighBitPair[i][j]	= 0;
					m_aRefNoLowBitPair[i][j]	= 0;
					m_aGrabNoLowBitPair[i][j]	= 0;
				}
			}
		}
	}

	void SetNoSkippedSignalLineLowBits(int nNoLowBit)
	{
		m_nNoSkippedSignalLineLowBits = nNoLowBit;
	}
};

class CImageProc  
{
public:

	int m_nNoErrorInfo;
	int m_nColorMarginLevel;		
	int m_nBrightMarginLevel;
	int m_nNoUsedColorHighBits;		//0: Use All Bits, 1~ : No High Bits
	int m_nNoUsedBrightHighBits;	//0: Use All Bits, 1~ : No High Bits

	float m_fTargetMatchRate;

	POINT m_ptPositionShift;
	POINT m_ptRoiPosition;
	
	int m_nRoiWidth;
	int m_nRoiHeight;

	int m_nAvgMaskWidth;
	int m_nAvgMaskHeight;
	int m_nNoRepeat;

	float m_fLowerLimit;
	float m_fUpperLimit;

	int m_aNoDifferentPoint[3][100];
	int m_nSecondDifferentPointFreePercent;

	CLineBitTestData m_LineBitTestData;

	//+del kwmoon 080715
//	float m_fAllowedErrorPixelPercentage;
	int m_nCaptionWidth;
	int m_nCaptionHeight;

	//+del kwmoon 080715
//	float m_fAllowedErrorPixelPercentageForOsdTest;

	//+add kwmoon 071224
	int m_nCaptionKeyDelay;
	int	m_nCaptionEnterKeyDelay;
	int m_nCaptionCheckTime;

	//+change kwmoon 080124
	//+add kwmoon 080110
//	int m_nNoKeyPress;
	int m_nNoOnKeyPress;
	int m_nNoOffKeyPress;

	//+add kwmoon 071127
	int m_nDigitalEyeDelay;

	//+add kwmoon 080121
	POINT m_ptOsdPosition;
	
	//+add kwmoon 080121
	int m_nOsdWidth;
	int m_nOsdHeight;

	//+add kwmoon 080228
	int m_nNoPixelInMiddleRange[3];
	int m_nNoNonBlackPixel[3];
	
	//+add kwmoon 080715
	int	 m_nNoBandErrorPixel[5];
	int	 m_nNoAllowedBandErrorPixel[5];

	float m_fBandErrorPixelPercentage[5];
	float m_fAllowedBandErrorPixelPercentage[5];
	float m_fGoodPixelPercentage;

	int	  m_nNoRoiPixel;
	int   m_nNoTargetPixel;
    int	  m_nNoGoodPixel;

	//+add kwmoon 080828
	POINT m_ptInvalidPosition;
	int	  m_nInvalidWidth;
	int	  m_nInvalidHeight;
	BOOL  m_bInvalidateInvalidRegion;

	//+add 20110802
	int    m_nDiff_LowerLimit;
	int    m_nDiff_UpperLimit;
	double m_fMovingPicTest_Limit;


public:

	void SetMovingPicTestMargin(int nDiff_LoLimit, int nDiff_HiLimit, int nTest_Limit);
	void InitMovingPicTestMargin();

	int PatternMatching(int nTestType,CBaseImage &SnapImage,CBaseImage &RefImage, 
						POINT& ptResultPosition,float& fMatchRate,CProgressCtrl &Progress);

	//+change kwmoon 080716
//	int RGBTest(int nType,CBaseImage& SnapImage,CBaseImage& RefImage,CBaseImage& MaskImage,
//				POINT& ptPlusResultPosition,POINT& ptMinusResultPosition, 
//				int& nPlusMaxDifference,int& nMinusMaxDifference,int& nTotalNoErrorPixel,BOOL bUseMaskImage,CProgressCtrl& Progress);
	int RGBTest(int nType,CBaseImage& SnapImage,CBaseImage& RefImage,CBaseImage& MaskImage,
				POINT& ptResultPosition,float& nMaxDifference,int& nTotalNoErrorPixel,BOOL bUseMaskImage,CProgressCtrl& Progress);

	int NoiseTest(int nType,CBaseImage& SnapImage,CBaseImage& RefImage,CBaseImage& MaskImage,
				POINT& ptPlusResultPosition,POINT& ptMinusResultPosition, 
				int& nPlusMaxDifference,int& nMinusMaxDifference,int& nTotalNoErrorPixel,BOOL bUseMaskImage,CProgressCtrl& Progress);

	int SignalLineTest(int nType,BOOL bUseMaskImage,CProgressCtrl& Progress);

	int BrightnessTest(CBaseImage& SnapImage,CBaseImage& RefImage,CBaseImage& MaskImage,
				POINT& ptResultPosition,int& nMaxDifference, CProgressCtrl& Progress);

	int BinaryForMaskImage(int nType,CBaseImage& DstImage,int nThreshold,BOOL bOverwrite = FALSE);
	int BinaryForMonoImage(int nType,BYTE* pImageData,int nHeight,int nWidth,int nBitCount,int nThreshold);
	
	int BinaryForDiffImage(int nType,CBaseImage& SnapImage,int nLowerLimit, int nUpperLimit);
	//+add kwmoon 071112
	int BinaryForDiffImage(int nType,CBaseImage& DstImage, CBaseImage& RefImage,int nLowerLimit, int nUpperLimit);

	int BinaryForColorImage(int nType,CBaseImage& DstImage,int nThreshold,BOOL bOverwrite = FALSE);

	void SetResolution(int nXresolution, int nYresolution);
	void SetMatchRate(float fMatchRate);
	
	void SetColorMargin(int nColorMargin);
	void SetBrightMargin(int nBrightMargin);

	void SetUsedNoColorHighBits(int nNoUsedColorHighBits);
	void SetUsedNoBrightHighBits(int nNoUsedBrightHighBits);

	void SetRoi(POINT ptStartPosition,POINT ptEndPosition);
	void SetPositionShift(CPoint ptPositionShift);

	//+change kwmoon 080820
//	int Average(int nMaskWidth, int nMaskHeight, CBaseImage& DstImage,BOOL bOverwrite);
	int Average(int nMaskWidth, int nMaskHeight, CBaseImage& DstImage);
	void SetAvgMaskSize(int nMaskWidth, int nMaskHeight,int nNoRepeat);

	int Average(int nMaskWidth, int nMaskHeight, int nStartXposition, int nEndXposition,
				int nStartYposition, int nEndYposition, CBaseImage& DstImage,BOOL bOverwrite);

	BOOL MakeMaskImg(int nMaskWidth, CBaseImage& SrcImage, CBaseImage& DstImage,CProgressCtrl& Progress,BOOL bInvalidateOsdRegion = TRUE);

	int	ExtPlane(BYTE* aBm,int** aImg,int aBmWid,int aBmHgt,int aPxlWid);
	int	DepPlane(int** aImg,BYTE* aBm,int aBmWid,int aBmHgt,int aPxlWid);
	int	Filter(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt,int aMaskWid,int aMaskHgt,int* aMask);
	int	ConstThr(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt,int aThrl,int aThrh,int aInval,int aOutVal);
	int	Dilation(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt);
	int	OuterFill(int* aSrcImg,int aBmWid,int aBmHgt,int aHor,int aVer,int aVal);
	int	Xor(int* aSrcImg,int* aDstImg,int aBmWid,int aBmHgt,int aVal);

	int GetOppositeStartYposition(int nPosition, int nRoiHeight, int nImageHeight);

	//+change kwmoon 080618
//	void CalcTestRange(CBaseImage& SnapImage, CBaseImage& RefImage,
//					  int& nStartXposition, int& nEndXposition, int& nStartYposition, int& nEndYposition);
	void CalcTestRange(CBaseImage& SnapImage,
					  int& nStartXposition, int& nEndXposition, int& nStartYposition, int& nEndYposition);

	int	MakeAvgImg(CStringArray& aFilePathArray,CBaseImage& DstImage,CProgressCtrl& ItemProgress,CProgressCtrl& TotalProgress);
	int	MakeAvgImg(CStringArray& aFilePathArray,CBaseImage& DstImage,CProgressCtrl& ItemProgress);

	void SetColorLimit(float fLowerLimit, float fUpperLimit);

	int StackPush(int* stackX, int* stackY, int arr_size, int vx, int vy, int *top);
	int StackPop(int* stackX, int* stackY, int* vx, int* vy, int *top);
	int Labeling(CBaseImage& SnapImage, CBaseImage& MaskImage, CBaseImage& ProcImage,CProgressCtrl& Progress);
	int Repaint(CBaseImage& SnapImage, CBaseImage& MaskImage, CBaseImage& ProcImage,CProgressCtrl& Progress);

	BOOL RGB2Gray(int nType,CBaseImage& DstImage,BOOL bOverwrite);
	
	void ZoomOut(CBaseImage& SrcImage,float fZoomFactor);

	float Level2Percentage(int nLevel, int nNoLevel);
	int Percentage2Level(float fPercent, int nNoLevel);

	int	NoVideoSignalTest(CBaseImage& DstImage,CProgressCtrl& ItemProgress);
	int	CaptionTest(CBaseImage& SnapImage, CBaseImage& RefImage,CBaseImage& MaskImage,int nLowLimit, int nHighLimit, CProgressCtrl& ItemProgress);

	void SetCaptionSize(int nWidth, int nHeight);

	//+del kwmoon 080715
//	void SetNoAllowedErrorPixel(float fAllowedErrorPixel, float fAllowedErrorPixelForOsdTest);

	//+add kwmoon 080715
	void SetAllowedErrorPixel(float fAllowedBandErrorPixelPercentage[]);
	void SetCaptionKey(int nCaptionKeyDelay, int nCaptionEnterKeyDelay,int nNoOnKeyPress, int nNoOffKeyPress);
	void SetCaptionCheckTime(int nCaptionCheckTime);

	void SetDigitalEyeDelay(int nDelay);
	void SetOsdRegion(POINT ptStartPosition,POINT ptEndPosition);
	int InvalidateOsdRegion(CBaseImage& MaskImage);
	int GetMaxValue(int nDifferenceB,int nDifferenceG,int nDifferenceR);
	int GetMinValue(int nDifferenceB,int nDifferenceG,int nDifferenceR);
	int	SetDefaultOsdRegion(int nImageWidth,int nImageHeight);

	//+add kwmoon 080716
	int	CheckImageAttribute(CBaseImage& SnapImage, CBaseImage& RefImage,CBaseImage& MaskImage);

	//+add kwmoon 080722
	int	ColorBiasTest(CBaseImage& DstImage, int &nDifference, CString& szResultInfo,CProgressCtrl& ItemProgress);

	//+add kwmoon 080828
	void SetInvalidRegion(POINT ptStartPosition,POINT ptEndPosition);

	//+add kwmoon 080828
	int	InvalidateInvalidRegion(CBaseImage& MaskImage);

	int	NoVideoSignalTest2(CBaseImage& DstImage,int ColNumThre, int* pColNum, int nStep = 2);

	//+add 101123
	int CopyImage(CBaseImage& SrcImage, CBaseImage& DestImage);
	int	CheckImageDiff(CBaseImage& SnapImage, CBaseImage& RefImage, int nLowerLimit, int nUpperLimit);

	CImageProc();
	virtual ~CImageProc();
};

#endif // !defined(AFX_IMAGEPROC_H__31144BD6_3E8C_419F_8013_118E28B8369C__INCLUDED_)