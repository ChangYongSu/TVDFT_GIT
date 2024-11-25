
// ///////////////////////////////////////////////////////////////////////////
// Header Files
// ///////////////////////////////////////////////////////////////////////////
#ifndef __LISTCTRLEX_H_20200217__
#define __LISTCTRLEX_H_20200217__

#define N0_MAX_COLOR_HEIGHT	2000
#define N0_MAX_COLOR_WIDTH	20

class	CListCtrlEx
{
public:
	CListCtrlEx();

	COLORREF nSelForeColor[N0_MAX_COLOR_HEIGHT][N0_MAX_COLOR_WIDTH];
	COLORREF nSelBackColor[N0_MAX_COLOR_HEIGHT][N0_MAX_COLOR_WIDTH];

public:

	~CListCtrlEx();

	///////////////////////////////////////////////////////////////////////////
	// SetLedStatesNumber (Public)
	// ///////////////////////////////////////////////////////////////////////////
	void SetTextColor(COLORREF lColor);
	void SetCellBackColor(int lRow, COLORREF lColor);
	void SetCellBackColor(int lRow, int lCol, COLORREF lColor);
	void SetCellForeColor(int lRow, COLORREF lColor);
	void SetCellForeColor(int lRow, int lCol, COLORREF lColor);
};
#endif