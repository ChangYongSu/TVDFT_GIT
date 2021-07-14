#pragma once

// 컴퓨터에서 Microsoft Visual C++를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// 참고: 이 파일의 내용을 수정하지 마십시오. Microsoft Visual C++에서
//  이 클래스를 다시 생성할 때 수정한 내용을 덮어씁니다.

/////////////////////////////////////////////////////////////////////////////
// CCwbool_SWLED 래퍼 클래스입니다.

class CCwbool_SWLED : public CWnd
{
protected:
	DECLARE_DYNCREATE(CCwbool_SWLED)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x1C9C118A, 0x4EEB, 0x4D3C, { 0xBF, 0x78, 0x91, 0x85, 0x1A, 0x4, 0xC0, 0xA7 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 특성입니다.
public:
enum
{
    cwLineNone = 0,
    cwLineSolid = 1,
    cwLineStepXY = 2,
    cwLineStepYX = 3,
    cwLineDash = 4,
    cwLineDot = 5,
    cwLineDashDot = 6,
    cwLineDashDotDot = 7,
    cwLineStepCentered = 8
}CWLineStyles;
enum
{
    cwPointNone = 0,
    cwPointEmptySquare = 1,
    cwPointSolidSquare = 2,
    cwPointAsterisk = 3,
    cwPointDottedEmptySquare = 4,
    cwPointDottedSolidSquare = 5,
    cwPointSolidDiamond = 6,
    cwPointEmptySquareWithX = 7,
    cwPointEmptySquareWithCross = 8,
    cwPointEmptyCircle = 9,
    cwPointSolidCircle = 10,
    cwPointDottedEmptyCircle = 11,
    cwPointDottedSolidCircle = 12,
    cwPointX = 13,
    cwPointBoldX = 14,
    cwPointSmallX = 15,
    cwPointCross = 16,
    cwPointBoldCross = 17,
    cwPointSmallCross = 18,
    cwPointSmallEmptySquare = 19,
    cwPointSmallSolidSquare = 20,
    cwPointSimpleDot = 21,
    cwPointEmptyDiamond = 22
}CWPointStyles;
enum
{
    cwVPLabelNone = 0,
    cwVPLabelName = 1,
    cwVPLabelValue = 2
}CWValuePairLabels;
enum
{
    cwVPLocationValue = 0,
    cwVPLocationIndex = 1
}CWValuePairLocations;
enum
{
    cwFillNone = 0,
    cwFillToMin = 1,
    cwFillToMax = 2,
    cwFillToGreater = 3,
    cwFillToLess = 4
}CWPointerFillStyles;
enum
{
    cwPointerNone = 0,
    cwPointerNormal = 1,
    cwPointerLeftBottomArrow = 2,
    cwPointerRightTopArrow = 3,
    cwPointerCustom = 4,
    cwPointer3D = 5
}CWPointerStyles;
enum
{
    cwPointerModeControl = 0,
    cwPointerModeIndicator = 1,
    cwPointerModeMinimum = 2,
    cwPointerModeMaximum = 3,
    cwPointerModeMean = 4,
    cwPointerModeControlSmooth = 5,
    cwPointerModeControlExact = 6
}CWPointerModes;
enum
{
    cwCSnapFloating = 0,
    cwCSnapNearestPoint = 1,
    cwCSnapPointsOnPlot = 2,
    cwCSnapFixed = 3,
    cwCSnapNearestYForFixedX = 4,
    cwCSnapAnchoredToPoint = 5
}CWCursorSnapModes;
enum
{
    cwCrosshairNone = 0,
    cwCrosshairMinorX = 1,
    cwCrosshairMajorX = 2,
    cwCrosshairMinorY = 3,
    cwCrosshairMinorXMinorY = 4,
    cwCrosshairMajorXMinorY = 5,
    cwCrosshairMajorY = 6,
    cwCrosshairMinorXMajorY = 7,
    cwCrosshairMajorXMajorY = 8
}CWCrosshairStyles;
enum
{
    cwSpeedOff = 0,
    cwSpeedFastest = 150,
    cwSpeedVeryFast = 300,
    cwSpeedFast = 600,
    cwSpeedMedium = 900,
    cwSpeedSlow = 1200,
    cwSpeedVerySlow = 1500,
    cwSpeedSlowest = 1800
}CWSpeeds;
enum
{
    cwdsRead = 2,
    cwdsReadAutoUpdate = 3,
    cwdsWrite = 4,
    cwdsWriteAutoUpdate = 5
}CWDSAccessModes;
enum
{
    cwdsUnconnected = 1,
    cwdsConnectionActive = 2,
    cwdsConnectionIdle = 3,
    cwdsConnectionError = 4,
    cwdsConnecting = 5
}CWDSStatus;
enum
{
    cwdsOPCGood = 192,
    cwdsOPCBad = 0,
    cwdsOPCUncertain = 64,
    cwdsOPCConfigError = 4,
    cwdsOPCNotConnected = 8,
    cwdsOPCDeviceFailure = 12,
    cwdsOPCSensorFailure = 16,
    cwdsOPCLastKnown = 20,
    cwdsOPCCommFailure = 24,
    cwdsOPCOutOfService = 28,
    cwdsOPCLastUsable = 68,
    cwdsOPCSensorCal = 80,
    cwdsOPCEguExceeded = 84,
    cwdsOPCSubNormal = 88,
    cwdsOPCLocalOverride = 216
}CWDSOPCQuality;
enum
{
    cwAxesCoordinates = 0,
    cwScreenCoordinates = 1,
    cwPlotAreaCoordinates = 2
}CWCoordinateTypes;
enum
{
    cwuiLeftJustify = 0,
    cwuiRightJustify = 1,
    cwuiCenter = 2,
    cwuiTopLeft = 3,
    cwuiTopRight = 4,
    cwuiTopCenter = 5,
    cwuiBottomLeft = 6,
    cwuiBottomRight = 7,
    cwuiBottomCenter = 8
}CWAlignments;
enum
{
    cwShapeNone = 0,
    cwShapePoint = 1,
    cwShapeLine = 2,
    cwShapeRectangle = 3,
    cwShapePolygon = 4,
    cwShapeOval = 5,
    cwShapeArc = 6,
    cwShapeImage = 7,
    cwShapeRegion = 8,
    cwShapeMinMaxRegion = 9
}CWShapeTypes;
enum
{
    cwRegionNormal = 0,
    cwRegionInverted = 1
}CWRegionAreas;
enum
{
    cwArrowHeadNone = 0,
    cwArrowHeadSolid = 1,
    cwArrowHeadLine = 2,
    cwArrowHeadStealth = 3,
    cwArrowHeadDiamond = 4,
    cwArrowHeadRound = 5
}CWArrowHeadStyles;
enum
{
    cwAnnotationStyleText = 0,
    cwAnnotationStyleArrow = 1,
    cwAnnotationStyleTextAndArrow = 2,
    cwAnnotationStylePicture = 3,
    cwAnnotationStyleLine = 4,
    cwAnnotationStyleRectangle = 5,
    cwAnnotationStyleHalfSpace = 6,
    cwAnnotationStyleRange = 7
}CWAnnotationStyles;
enum
{
    cwKeyboardNone = 0,
    cwKeyboardHandled = 1
}CWKeyboardModes;
enum
{
    cwShowFocusNone = 0,
    cwShowFocusControl = 1
}CWShowFocusModes;
enum
{
    cwSlideStyleV = 0,
    cwSlideStyleH = 1,
    cwSlideStyleVPointer = 2,
    cwSlideStyleHPointer = 3,
    cwSlideStyleV2 = 4,
    cwSlideStyleH2 = 5,
    cwSlideStyleVBar = 6,
    cwSlideStyleHBar = 7,
    cwSlideStyleTank = 8,
    cwSlideStyleThermometer = 9,
    cwSlideStyle3DTank = 10,
    cwSlideStyle3DThermometer = 11,
    cwSlideStyle3DVFillSlide = 12,
    cwSlideStyle3DHFillSlide = 13,
    cwSlideStyle3DVPointerSlide = 14,
    cwSlideStyle3DHPointerSlide = 15,
    cwSlideStyle3DVProgressBar = 16,
    cwSlideStyle3DHProgressBar = 17,
    cwSlideStyle3DVGraduatedBar = 18,
    cwSlideStyle3DHGraduatedBar = 19
}CWSlideStyles;
enum
{
    cwKnobStyleKnob = 0,
    cwKnobStyleDial = 1,
    cwKnobStyleHTopGauge = 2,
    cwKnobStyleVRightGauge = 3,
    cwKnobStyleHBottomGauge = 4,
    cwKnobStyleVLeftGauge = 5,
    cwKnobStyle3DKnob = 6,
    cwKnobStyle3DDial = 7,
    cwKnobStyle3DTopMeter = 8,
    cwKnobStyle3DRightMeter = 9,
    cwKnobStyle3DBottomMeter = 10,
    cwKnobStyle3DLeftMeter = 11
}CWKnobStyles;
enum
{
    cwButtonStyleVToggle = 0,
    cwButtonStyleHToggle = 1,
    cwButtonStyleSquareLED = 2,
    cwButtonStyleSquareStarLED = 3,
    cwButtonStyleRoundLED = 4,
    cwButtonStyleRoundStarLED = 5,
    cwButtonStyleRoundPushButton = 6,
    cwButtonStyleSquarePushButton = 7,
    cwButtonStyleV3dSlide = 8,
    cwButtonStyleH3dSlide = 9,
    cwButtonStyleVSlide = 10,
    cwButtonStyleHSlide = 11,
    cwButtonStyleToggleOnOff = 12,
    cwButtonStyleCommandOk = 13,
    cwButtonStyleBitmap = 14,
    cwButtonStyle3DVToggle = 15,
    cwButtonStyle3DHToggle = 16,
    cwButtonStyle3DSquareLED = 17,
    cwButtonStyle3DRoundLED = 18,
    cwButtonStyle3DPushButton = 19,
    cwButtonStyle3DVRocker = 20,
    cwButtonStyle3DHRocker = 21,
    cwButtonStyle3DVSlideSwitch = 22,
    cwButtonStyle3DHSlideSwitch = 23,
    cwButtonStyle3DToggleOnOff = 24,
    cwButtonStyle3DCommandOk = 25,
    cwButtonStyle3DBitmap = 26
}CWButtonStyles;
enum
{
    cwModeSwitchWhenPressed = 1,
    cwModeSwitchUntilReleased = 2,
    cwModeIndicator = 3
}CWButtonModes;
enum
{
    cwEdModeControl = 0,
    cwEdModeIndicator = 1
}CWNumEditModes;
enum
{
    cwChartStrip = 0,
    cwChartScope = 1
}CWChartStyles;
enum
{
    cwGTrackPlotAreaEvents = 0,
    cwGTrackAllEvents = 1,
    cwGTrackDragCursor = 2,
    cwGTrackPanPlotAreaXY = 3,
    cwGTrackPanPlotAreaX = 4,
    cwGTrackPanPlotAreaY = 5,
    cwGTrackZoomRectXY = 6,
    cwGTrackZoomRectX = 7,
    cwGTrackZoomRectY = 8,
    cwGTrackDragAnnotation = 9,
    cwGTrackZoomRectXYWithUndo = 10,
    cwGTrackZoomRectXWithUndo = 11,
    cwGTrackZoomRectYWithUndo = 12,
    cwGTrackZoomRectProp = 13,
    cwGTrackZoomRectPropWithUndo = 14
}CWGraphTrackModes;
enum
{
    cwGraphFrameClassic = 0,
    cwGraphFrame3D = 1
}CWGraphFrameStyles;
enum
{
    cwCursorCrosshair = 0,
    cwCursorXLine = 1,
    cwCursorYLine = 2
}CWCursorParts;
enum
{
    cwuiBorderStyleNone = 0,
    cwuiBorderStyleFixedSingle = 1,
    cwuiBorderStyle3D = 2
}CWBorderStyles;
enum
{
    cwuiNumEditButtonStyleClassic = 0,
    cwuiNumEditButtonStyle3D = 1
}CWNumEditButtonStyles;
enum
{
    cwuiAppearanceFlat = 0,
    cwuiAppearanceThreeD = 1
}CWAppearances;
enum
{
    cwuiLeft = 0,
    cwuiRight = 1,
    cwuiTop = 2,
    cwuiBottom = 3
}CWPositions;
enum
{
    cwAnnotationText = 0,
    cwAnnotationShape = 1,
    cwAnnotationShapePoint = 2,
    cwAnnotationAll = 3
}CWAnnotationParts;


// 작업입니다.
public:

// _DCWButton

// Functions
//

	void SetBuiltinStyle(long Style)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Style);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ImportStyle(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x202, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void ExportStyle(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x201, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	LPDISPATCH OnImages(VARIANT Item)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Item);
		return result;
	}
	LPDISPATCH OffImages(VARIANT Item)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Item);
		return result;
	}
	LPDISPATCH ControlImage()
	{
		LPDISPATCH result;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void Refresh()
	{
		InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	LPDISPATCH get_CWBindings()
	{
		LPDISPATCH result;
		InvokeHelper(0x115, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH ControlImageEx(long dpiX, long dpiY)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, dpiX, dpiY);
		return result;
	}

// Properties
//

BOOL GetEnabled()
{
	BOOL result;
	GetProperty(DISPID_ENABLED, VT_BOOL, (void*)&result);
	return result;
}
void SetEnabled(BOOL propVal)
{
	SetProperty(DISPID_ENABLED, VT_BOOL, propVal);
}
IFontDisp * GetFont()
{
	IFontDisp * result;
	GetProperty(DISPID_FONT, VT_DISPATCH, (void*)&result);
	return result;
}
void SetFont(IFontDisp * propVal)
{
	SetProperty(DISPID_FONT, VT_DISPATCH, propVal);
}
unsigned long GetBackColor()
{
	unsigned long result;
	GetProperty(DISPID_BACKCOLOR, VT_UI4, (void*)&result);
	return result;
}
void SetBackColor(unsigned long propVal)
{
	SetProperty(DISPID_BACKCOLOR, VT_UI4, propVal);
}
long GetMode()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}
void SetMode(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}
CString GetCaption()
{
	CString result;
	GetProperty(DISPID_CAPTION, VT_BSTR, (void*)&result);
	return result;
}
void SetCaption(CString propVal)
{
	SetProperty(DISPID_CAPTION, VT_BSTR, propVal);
}
BOOL GetValue()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}
void SetValue(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}
BOOL Get_Value()
{
	BOOL result;
	GetProperty(0x0, VT_BOOL, (void*)&result);
	return result;
}
void Set_Value(BOOL propVal)
{
	SetProperty(0x0, VT_BOOL, propVal);
}
CString GetOnText()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}
void SetOnText(CString propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}
CString GetOffText()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}
void SetOffText(CString propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}
unsigned long GetOnColor()
{
	unsigned long result;
	GetProperty(0x5, VT_UI4, (void*)&result);
	return result;
}
void SetOnColor(unsigned long propVal)
{
	SetProperty(0x5, VT_UI4, propVal);
}
unsigned long GetOffColor()
{
	unsigned long result;
	GetProperty(0x6, VT_UI4, (void*)&result);
	return result;
}
void SetOffColor(unsigned long propVal)
{
	SetProperty(0x6, VT_UI4, propVal);
}
unsigned long GetOnTextColor()
{
	unsigned long result;
	GetProperty(0x7, VT_UI4, (void*)&result);
	return result;
}
void SetOnTextColor(unsigned long propVal)
{
	SetProperty(0x7, VT_UI4, propVal);
}
unsigned long GetOffTextColor()
{
	unsigned long result;
	GetProperty(0x8, VT_UI4, (void*)&result);
	return result;
}
void SetOffTextColor(unsigned long propVal)
{
	SetProperty(0x8, VT_UI4, propVal);
}
unsigned long GetCaptionColor()
{
	unsigned long result;
	GetProperty(0x9, VT_UI4, (void*)&result);
	return result;
}
void SetCaptionColor(unsigned long propVal)
{
	SetProperty(0x9, VT_UI4, propVal);
}
CWPictureDisp * GetOnImage()
{
	CWPictureDisp * result;
	GetProperty(0xa, VT_DISPATCH, (void*)&result);
	return result;
}
void SetOnImage(CWPictureDisp * propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}
CWPictureDisp * GetOffImage()
{
	CWPictureDisp * result;
	GetProperty(0xb, VT_DISPATCH, (void*)&result);
	return result;
}
void SetOffImage(CWPictureDisp * propVal)
{
	SetProperty(0xb, VT_DISPATCH, propVal);
}
BOOL GetImmediateUpdates()
{
	BOOL result;
	GetProperty(0xc, VT_BOOL, (void*)&result);
	return result;
}
void SetImmediateUpdates(BOOL propVal)
{
	SetProperty(0xc, VT_BOOL, propVal);
}
long GetShowFocusMode()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}
void SetShowFocusMode(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}
CWPictureDisp * GetBackgroundImage()
{
	CWPictureDisp * result;
	GetProperty(0xe, VT_DISPATCH, (void*)&result);
	return result;
}
void SetBackgroundImage(CWPictureDisp * propVal)
{
	SetProperty(0xe, VT_DISPATCH, propVal);
}
BOOL GetWindowless()
{
	BOOL result;
	GetProperty(0x10, VT_BOOL, (void*)&result);
	return result;
}
void SetWindowless(BOOL propVal)
{
	SetProperty(0x10, VT_BOOL, propVal);
}
long GetReadyState()
{
	long result;
	GetProperty(DISPID_READYSTATE, VT_I4, (void*)&result);
	return result;
}
void SetReadyState(long propVal)
{
	SetProperty(DISPID_READYSTATE, VT_I4, propVal);
}
long GetKeyboardMode()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}
void SetKeyboardMode(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}


};
