//#if !defined(CURRENT_SETTING_H__A91C3948_305C_11D3_90A6_006008CED7F4__INCLUDED_)
//#define CURRENT_SETTING_H__A91C3948_305C_11D3_90A6_006008CED7F4__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#include "stdafx.h"

//+add kwmoon 070720
class CCurrentSetting 
{
public:

	CCurrentSetting();
	~CCurrentSetting();

public:

	CString m_szGrabRefImageName;
	
	int	m_nUseMaskImage;
	
	int	m_nPatternGenNo;

	CString m_szInputSource;

	BYTE m_nInputSourceCode;
	BYTE m_nInputSource;

	int m_nChannel;
	
	int m_nVideoSource;
	int m_nVideoRfType;
	int m_nVideoAvChannel;

	int m_nResolutionX;
	int m_nResolutionY;

	//+del kwmoon 071004
//	int m_nPositionMarginX;
//	int m_nPositionMarginY;
	//+add kwmoon 071004
	float m_fTargetMatchRate;

	int m_nRoiX1;
	int m_nRoiY1;
	int m_nRoiX2;
	int m_nRoiY2;
	
	int m_nAudioSource;
	int m_nNoUsedColorHighBits;
	int m_nNoUsedBrightHighBits;

	int m_nSwitchButtonNo;
	int m_nSwitchButtonValue1;
	int m_nSwitchButtonValue2;
	//+2007.10.02 Add BY USY
	int m_nGrabWidth;
	int m_nGrabHeight;
//	int m_nGrabType;	//0 : LVDS_GRABBER, 1 : ANALOG_GRABBER
	//-
	
	//+add kwmoon 071206
	int	m_nNotSkipSameSourceChannel;	// 1 : Skip Same Source or Channel

public:

	void InitVariables();

	//+del kwmoon 071004
//	BOOL IsSamePositionMargin(int nPositionMarginX,int nPositionMarginY);
	//+add kwmoon 071004
	BOOL IsSameMatchRate(float fMatchRate);

	BOOL IsSameInputSource(CString szInputSource);
	BOOL IsSamePatternGenNo(int nPatternGenNo);
	BOOL IsSameChannel(int nChannel);
	BOOL IsSameVideoSource(int nVideoSource,int nVideoRfType,int nVideoAvChannel);
	BOOL IsSameResolution(int nResolutionX,int nResolutionY);
	BOOL IsSameRoi(int nRoiX1,int nRoiY1,int nRoiX2,int nRoiY2);
	BOOL IsSameAudioSource(int nAudioSource);
	BOOL IsSameUseColorBits(int nNoUsedColorHighBits);
	BOOL IsSameUseBrightBits(int nNoUsedBrightHighBits);
	BOOL IsSameAvSwitchBoxSignal(int nSwitchButtonNo,int nSwitchButtonValue1,int nSwitchButtonValue2);
	//+2007.10.02 Add BY USY
	BOOL IsSameLvdsResolution(int nGrabWidth, int nGrabHeight);
	//-

	//+add kwmoon 080122
	BOOL IsSameGrabRefImage(CString szGrabRefImageName);
	//-
};


//#endif // !defined(CURRENT_SETTING_H__A91C3948_305C_11D3_90A6_006008CED7F4__INCLUDED_)
