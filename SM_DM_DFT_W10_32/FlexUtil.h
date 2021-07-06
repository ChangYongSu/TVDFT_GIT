#if !defined(AFX_FLEXUTIL_H__1A4914CA_D735_4719_A6DC_F8242225F74E__INCLUDED_)
#define AFX_FLEXUTIL_H__1A4914CA_D735_4719_A6DC_F8242225F74E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlexUtil.h : VSFlexGrid Pro 7.0 utilities
//

#include "vsflexgrid.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unit conversion utilities
//
long XTwipsToPixels(long twips);
long YTwipsToPixels(long twips);
long XPixelsToTwips(long pixels);
long YPixelsToTwips(long pixels);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HTML generation
//
BOOL SaveGridToHTML(CListCtrl* ctl, LPSTR fname);
BOOL SaveGridToText(CListCtrl* ctl, LPSTR fname);
BOOL SaveGridToCSV(CListCtrl* ctl, LPSTR fname);
//BOOL SaveGridToHTML(CVSFlexGrid* ctl, LPSTR fname);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Copy control to a bitmap (so it can be copied to the clipboard)
//
HBITMAP ControlToBMP(HWND hWnd);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// CVSFlexGridDataObject wrapper class
//
// this is useful for handling the data object passed as a parameter in
// OLE drag drop events.
//
// for example:
//
// void CBigDemoDlg::OnOLEDragDrop(LPDISPATCH FAR* Data, ...)
// {
//		// wrap data object's IDispatch
//		CVSFlexGridDataObject ddData(*Data);
//
//		// make sure the object has files
//		BOOL bHasFiles = (ddData.GetFileCount() > 0);
//	...
// }
//

#ifndef __CVSFlexGridDATAOBJECT__
#define __CVSFlexGridDATAOBJECT__

class CVSFlexGridDataObject : public COleDispatchDriver
{
public:

	CVSFlexGridDataObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch, FALSE) {}

	void Clear()
	{
		InvokeHelper(1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	short GetFileCount()
	{
		short result;
		InvokeHelper(2, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
		return result;
	}
	VARIANT GetData(short Format)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I2;
		InvokeHelper(3, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, parms, Format);
		return result;
	}
	VARIANT_BOOL GetFormat(short Format)
	{
		BOOL result;
		static BYTE parms[] = VTS_I2;
		InvokeHelper(4, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Format);
		return result;
	}
	CString GetFiles(short Index)
	{
		CString result;
		static BYTE parms[] = VTS_I2;
		InvokeHelper(5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	void SetData(const VARIANT& Data, short Format = 0)
	{
		static BYTE parms[] = VTS_VARIANT VTS_I2;
		InvokeHelper(6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, &Data, Format);
	}
};
#endif // __CVSFlexGridDATAOBJECT__


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Enumeration constants for VSFlexGrid Pro 7.0
//
typedef enum EditableSettings {
    flexEDNone,
    flexEDKbd,
    flexEDKbdMouse,
} EditableSettings;

typedef enum RedrawSettings {
    flexRDNone,
    flexRDDirect,
    flexRDBuffered,
} RedrawSettings;

typedef enum AppearanceSettings {
    flexFlat,
    flex3D,
    flex3DLight,
} AppearanceSettings;

typedef enum BorderStyleSettings {
    flexBorderNone,
    flexBorderFlat,
} BorderStyleSettings;

typedef enum MousePointerSettings {
    flexDefault,
    flexArrow,
    flexCross,
    flexIBeam,
    flexIcon,
    flexSize,
    flexSizeNESW,
    flexSizeNS,
    flexSizeNWSE,
    flexSizeEW,
    flexUpArrow,
    flexHourglass,
    flexNoDrop,
    flexArrowHourGlass,
    flexArrowQuestion,
    flexSizeAll,
    flexPointerCopy = 50,
    flexPointerMove = 51,
    flexSizeHorz    = 52,
    flexSizeVert    = 53,
    flexHand        = 54,
    flexCustom      = 99,
} MousePointerSettings;

typedef enum RowStatusSettings {
	flexrsNew,
	flexrsUpdated,
	flexrsModified,
	flexrsDeleted,
} RowStatusSettings;

typedef enum CellPropertySettings {
	flexcpText,
	flexcpTextStyle,
	flexcpAlignment,
	flexcpPicture,
	flexcpPictureAlignment,
	flexcpChecked,
	flexcpBackColor,
	flexcpForeColor,
	flexcpFloodPercent,
	flexcpFloodColor,
	flexcpFont,
	flexcpFontName,
	flexcpFontSize,
	flexcpFontBold,
	flexcpFontItalic,
	flexcpFontUnderline,
	flexcpFontStrikethru,
	flexcpFontWidth,
	flexcpValue,
	flexcpTextDisplay,
	flexcpData,
	flexcpCustomFormat,
	flexcpLeft,
	flexcpTop,
	flexcpWidth,
	flexcpHeight,
	flexcpVariantValue,
	flexcpRefresh,
	flexcpSort,
} CellPropertySettings;

typedef enum ExplorerBarSettings {
	flexExNone            = 0,
	flexExSort            = 1,
	flexExMove            = 2,
	flexExSortAndMove     = 3,
	flexExSortShow        = 5,
	flexExSortShowAndMove = 7,
	flexExMoveRows        = 8,
} ExplorerBarSettings;

typedef enum AutoSearchSettings {
	flexSearchNone,
	flexSearchFromTop,
	flexSearchFromCursor,
} AutoSearchSettings;

typedef enum ComboSearchSettings {
	flexCmbSearchNone,
	flexCmbSearchLists,
	flexCmbSearchCombos,
	flexCmbSearchAll,
} ComboSearchSettings;

typedef enum MergeCompareSettings {
	flexMCExact,
	flexMCNoCase,
	flexMCTrimNoCase,
} MergeCompareSettings;

typedef enum EllipsisSettings {
	flexNoEllipsis,
	flexEllipsisEnd,
	flexEllipsisPath,
} EllipsisSettings;

typedef enum CellCheckedSettings {
	flexNoCheckbox,
	flexChecked,
	flexUnchecked,
	flexTSChecked,
	flexTSUnchecked,
	flexTSGrayed,
} CellCheckedSettings;

typedef enum TabBehaviorSettings {
    flexTabControls,
    flexTabCells,
} TabBehaviorSettings;

typedef enum SaveLoadSettings {
    flexFileAll,
    flexFileData,
    flexFileFormat,
    flexFileCommaText,
    flexFileTabText,
    flexFileCustomText,
} SaveLoadSettings;

typedef enum ArchiveSettings {
	arcAdd,
	arcStore,
	arcExtract,
	arcDelete,
} ArchiveSettings;

typedef enum ArchiveInfoSettings {
	arcFileCount,
	arcFileName,
	arcFileSize,
	arcFileCompressedSize,
	arcFileDate
} ArchiveInfoSettings;

typedef enum OLEDragModeSettings {
    flexOLEDragManual,
    flexOLEDragAutomatic,
} OLEDragModeSettings;

typedef enum OLEDropModeSettings {
    flexOLEDropNone,
    flexOLEDropManual,
    flexOLEDropAutomatic,
} OLEDropModeSettings;

typedef enum DataModeSettings {
    flexDMFree,
    flexDMBound,
    flexDMBoundBatch,
    flexDMBoundImmediate,
} DataModeSettings;

typedef enum OwnerDrawSettings {
    flexODNone,
    flexODOver,
    flexODContent,
    flexODComplete,
    flexODOverFixed,
    flexODContentFixed,
    flexODCompleteFixed,
} OwnerDrawSettings;

typedef enum DataTypeSettings {
	flexDTEmpty    = 0,
	flexDTNull     = 1,
	flexDTShort    = 2,
	flexDTLong     = 3,
	flexDTSingle   = 4,
	flexDTDouble   = 5,
	flexDTCurrency = 6,
	flexDTDate     = 7,
	flexDTString   = 8,
	flexDTDispatch = 9,
	flexDTError    = 10,
	flexDTBoolean  = 11,
	flexDTVariant  = 12,
	flexDTUnknown  = 13,
	flexDTDecimal  = 14,
	flexDTLong8    = 20,
	flexDTStringC  = 30,
	flexDTStringW  = 31,
} DataTypeSettings;

typedef enum AutoSizeSettings {
    flexAutoSizeColWidth,
    flexAutoSizeRowHeight,
} AutoSizeSettings;

typedef enum CollapsedSettings {
    flexOutlineExpanded,
    flexOutlineSubtotals,
    flexOutlineCollapsed,
} CollapsedSettings;

typedef enum OutlineBarSettings {
    flexOutlineBarNone,
    flexOutlineBarComplete,
    flexOutlineBarSimple,
    flexOutlineBarSymbols,
    flexOutlineBarCompleteLeaf,
    flexOutlineBarSimpleLeaf,
    flexOutlineBarSymbolsLeaf,
} OutlineBarSettings;

typedef enum SubtotalSettings {
    flexSTNone,
    flexSTClear,
    flexSTSum,
    flexSTPercent,
    flexSTCount,
    flexSTAverage,
    flexSTMax,
    flexSTMin,
    flexSTStd,
    flexSTVar,
} SubtotalSettings;

typedef enum SortSettings {
    flexSortNone,
    flexSortGenericAscending,
    flexSortGenericDescending,
    flexSortNumericAscending,
    flexSortNumericDescending,
    flexSortStringNoCaseAscending,
    flexSortStringNoCaseDescending,
    flexSortStringAscending,
    flexSortStringDescending,
    flexSortCustom,
    flexSortUseColSort,
} SortSettings;

typedef enum FocusRectSettings {
    flexFocusNone,
    flexFocusLight,
    flexFocusHeavy,
    flexFocusSolid,
    flexFocusRaised,
    flexFocusInset,
} FocusRectSettings;

typedef enum ShowSelSettings {
    flexHighlightNever,
    flexHighlightAlways,
    flexHighlightWithFocus,
} ShowSelSettings;

typedef enum ScrollBarsSettings {
    flexScrollBarNone,
    flexScrollBarHorizontal,
    flexScrollBarVertical,
    flexScrollBarBoth,
} ScrollBarsSettings;

typedef enum TextStyleSettings { 
    flexTextFlat,
    flexTextRaised,
    flexTextInset,
    flexTextRaisedLight,
    flexTextInsetLight,
} TextStyleSettings;

typedef enum FillStyleSettings {
    flexFillSingle,
    flexFillRepeat,
} FillStyleSettings;

typedef enum GridStyleSettings {
    flexGridNone,
    flexGridFlat,
    flexGridInset,
    flexGridRaised,
    flexGridFlatHorz,
    flexGridInsetHorz,
    flexGridRaisedHorz,
    flexGridSkipHorz,
    flexGridFlatVert,
    flexGridInsetVert,
    flexGridRaisedVert,
    flexGridSkipVert,
    flexGridExplorer,
    flexGridExcel,
} GridStyleSettings;

typedef enum SelModeSettings {
    flexSelectionFree,
    flexSelectionByRow,
    flexSelectionByColumn,
    flexSelectionListBox,
} SelModeSettings;

typedef enum MergeSettings {
    flexMergeNever,
    flexMergeFree,
    flexMergeRestrictRows,
    flexMergeRestrictColumns,
    flexMergeRestrictAll,
    flexMergeFixedOnly,
    flexMergeSpill,
    flexMergeOutline,
} MergeSettings;

typedef enum PictureTypeSettings {
    flexPictureColor,
    flexPictureMonochrome,
} PictureTypeSettings;

typedef enum AllowUserResizeSettings {
    flexResizeNone,
    flexResizeColumns,
    flexResizeRows,
    flexResizeBoth,
    flexResizeBothUniform,
} AllowUserResizeSettings;

typedef enum AllowUserFreezeSettings {
    flexFreezeNone,
    flexFreezeColumns,
    flexFreezeRows,
    flexFreezeBoth,
} AllowUserFreezeSettings;

typedef enum SubtotalPositionSettings {
    flexSTBelow,
    flexSTAbove,
} SubtotalPositionSettings;

typedef enum AlignmentSettings {
    flexAlignLeftTop,
    flexAlignLeftCenter,
    flexAlignLeftBottom,
    flexAlignCenterTop,
    flexAlignCenterCenter,
    flexAlignCenterBottom,
    flexAlignRightTop,
    flexAlignRightCenter,
    flexAlignRightBottom,
    flexAlignGeneral,
} AlignmentSettings;

typedef enum PictureAlignmentSettings {
    flexPicAlignLeftTop,
    flexPicAlignLeftCenter,
    flexPicAlignLeftBottom,
    flexPicAlignCenterTop,
    flexPicAlignCenterCenter,
    flexPicAlignCenterBottom,
    flexPicAlignRightTop,
    flexPicAlignRightCenter,
    flexPicAlignRightBottom,
    flexPicAlignStretch,
    flexPicAlignTile,
} PictureAlignmentSettings;

typedef enum NodeMoveSettings {
    flexNMIn,
    flexNMOut,
    flexNMUp,
    flexNMDown,
    flexNMFirst,
    flexNMLast,
    flexNMChildOf,
} NodeMoveSettings;

typedef enum NodeTypeSettings {
	flexNTRoot,
	flexNTParent,
	flexNTFirstChild,
	flexNTLastChild,
	flexNTFirstSibling,
	flexNTLastSibling,
	flexNTNextSibling,
	flexNTPreviousSibling,
} NodeTypeSettings;

//+ 2007.5.29 Add BY USY
typedef enum ClearMethodWhereSetting {
	flexClearEverywhere,		//Clear everywhere(this is the default setting
	flexClearScrollable,		//Clear scrollable region(excludes fixed rows and columns)
	flexClearSelection,			//Clear current selection
} ClearMethodWhereSetting;

typedef enum ClearMethodWhartSetting {
	flexClearEverything,		//Clear cell text, formatting and custom data(this is the default setting)
	flexClearText,				//Clear text only
	flexClearFormatting,		//clear custom formatting only(including pictures and cell data)
	flexClearData,				//Clears all custom data(row, column , and cell data)
}ClearMethodWhartSetting;
//-


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLEXUTIL_H__1A4914CA_D735_4719_A6DC_F8242225F74E__INCLUDED_)
