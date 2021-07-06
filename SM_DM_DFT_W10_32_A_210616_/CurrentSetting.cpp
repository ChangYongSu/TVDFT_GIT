
#include "StdAfx.h"

#include "CurrentSetting.h"

//+add kwmoon 070720

CCurrentSetting::CCurrentSetting()
{
	InitVariables();
};

CCurrentSetting::~CCurrentSetting()
{

}


void CCurrentSetting::InitVariables()
{
	m_szGrabRefImageName	= _T("");
	
	m_nUseMaskImage		= -1;
	
	m_nPatternGenNo		= -1;
	m_szInputSource		= _T("");
	m_nChannel			= -1;
	
	m_nVideoSource		= -1;
	m_nVideoRfType		= -1;
	m_nVideoAvChannel	= -1;

	m_nResolutionX		= -1;
	m_nResolutionY		= -1;
	
	//+del kwmoon 071004
///	m_nPositionMarginX	= -1;
//	m_nPositionMarginY	= -1;
	//+change kwmoon 071201 : 100.0f -> 99.5f
	//+add kwmoon 071004
	m_fTargetMatchRate	= 99.5f;

	m_nRoiX1			= -1;
	m_nRoiY1			= -1;
	m_nRoiX2			= -1;
	m_nRoiY2			= -1;

	
	m_nAudioSource		= -1;

	//+change kwmoon 071201 : chagne defalut value : -1 -> 0
//	m_nNoUsedColorHighBits	= -1;
//	m_nNoUsedBrightHighBits	= -1;
	m_nNoUsedColorHighBits	= 0;
	m_nNoUsedBrightHighBits	= 0;

	m_nSwitchButtonNo	  = -1;
	m_nSwitchButtonValue1 = -1;
	m_nSwitchButtonValue2 = -1;
	//+2007.10.02 Add BY USY
	m_nGrabWidth = -1;
	m_nGrabHeight = -1;
	//-

	//+add kwmoon 071206
	m_nNotSkipSameSourceChannel	= 0;	// 1 : Skip Same Source or Channel

	m_nInputSourceCode = 0xff;
	m_nInputSource = -1;
}


BOOL CCurrentSetting::IsSamePatternGenNo(int nPatternGenNo)
{
	if(nPatternGenNo != m_nPatternGenNo) 
	{
		m_nPatternGenNo = nPatternGenNo;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CCurrentSetting::IsSameInputSource(CString szInputSource)
{
	if(m_szInputSource.Compare(szInputSource)) 
	{
		m_szInputSource = szInputSource;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CCurrentSetting::IsSameChannel(int nChannel)
{
	if(m_nChannel != nChannel) 
	{
		m_nChannel = nChannel;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CCurrentSetting::IsSameVideoSource(int nVideoSource,int nVideoRfType,int nVideoAvChannel)
{
	if((m_nVideoSource	  != nVideoSource)
	|| (m_nVideoRfType	  != nVideoRfType)
	|| (m_nVideoAvChannel != nVideoAvChannel))
	{
		m_nVideoSource	  = nVideoSource;
		m_nVideoRfType	  = nVideoRfType;
		m_nVideoAvChannel = nVideoAvChannel;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


BOOL CCurrentSetting::IsSameResolution(int nResolutionX,int nResolutionY)
{
	if((m_nResolutionX	  != nResolutionX)
	|| (m_nResolutionY	  != nResolutionY))
	{
		m_nResolutionX	  = nResolutionX;
		m_nResolutionY	  = nResolutionY;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//+del kwmoon 071004
/*
BOOL CCurrentSetting::IsSamePositionMargin(int nPositionMarginX,int nPositionMarginY)
{
	if((m_nPositionMarginX	  != nPositionMarginX)
	|| (m_nPositionMarginY	  != nPositionMarginY))
	{
		m_nPositionMarginX	  = nPositionMarginX;
		m_nPositionMarginY	  = nPositionMarginY;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
*/

BOOL CCurrentSetting::IsSameMatchRate(float fMatchRate)
{
	if(m_fTargetMatchRate  != fMatchRate)
	{
		m_fTargetMatchRate = fMatchRate;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


BOOL CCurrentSetting::IsSameRoi(int nRoiX1,int nRoiY1,int nRoiX2,int nRoiY2)
{
	if((m_nRoiX1	  != nRoiX1)
	|| (m_nRoiY1	  != nRoiY1)
	|| (m_nRoiX2	  != nRoiX2)
	|| (m_nRoiY2	  != nRoiY2))
	{
		m_nRoiX1	  = nRoiX1;
		m_nRoiY1	  = nRoiY1;
		m_nRoiX2	  = nRoiX2;
		m_nRoiY2	  = nRoiY2;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CCurrentSetting::IsSameAudioSource(int nAudioSource)
{
	if(m_nAudioSource != nAudioSource) 
	{
		m_nAudioSource = nAudioSource;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//+add kwmoon 070122
BOOL CCurrentSetting::IsSameGrabRefImage(CString szGrabRefImageName)
{
	if(m_szGrabRefImageName.Compare(szGrabRefImageName) != NULL) 
	{
		m_szGrabRefImageName = szGrabRefImageName;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CCurrentSetting::IsSameUseColorBits(int nNoUsedColorHighBits)
{
	if(m_nNoUsedColorHighBits != nNoUsedColorHighBits) 
	{
		m_nNoUsedColorHighBits = nNoUsedColorHighBits;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}


BOOL CCurrentSetting::IsSameUseBrightBits(int nNoUsedBrightHighBits)
{
	if(m_nNoUsedBrightHighBits != nNoUsedBrightHighBits) 
	{
		m_nNoUsedBrightHighBits = nNoUsedBrightHighBits;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CCurrentSetting::IsSameAvSwitchBoxSignal(int nSwitchButtonNo,int nSwitchButtonValue1,int nSwitchButtonValue2)
{
	if((m_nSwitchButtonNo		!= nSwitchButtonNo)
	|| (m_nSwitchButtonValue1	!= nSwitchButtonValue1)
	|| (m_nSwitchButtonValue2	!= nSwitchButtonValue2))
	{
		m_nSwitchButtonNo		= nSwitchButtonNo;
		m_nSwitchButtonValue1	= nSwitchButtonValue1;
		m_nSwitchButtonValue2	= nSwitchButtonValue2;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
BOOL CCurrentSetting::IsSameLvdsResolution(int nGrabWidth, int nGrabHeight)
{
	if((m_nGrabWidth	  != nGrabWidth)
	|| (m_nGrabHeight	  != nGrabHeight))
	{
		m_nGrabWidth	  = nGrabWidth;
		m_nGrabHeight	  = nGrabHeight;
		
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}
