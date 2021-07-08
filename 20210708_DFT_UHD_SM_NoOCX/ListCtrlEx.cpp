

// ///////////////////////////////////////////////////////////////////////////
// Header Files
// ///////////////////////////////////////////////////////////////////////////
#include "StdAfx.h" // should be the 1st include ...
#include "ListCtrlEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// ///////////////////////////////////////////////////////////////////////////
// Default Constructor (Public)
// ///////////////////////////////////////////////////////////////////////////

CListCtrlEx::CListCtrlEx()
{

	for (int i = 0; i < N0_MAX_COLOR_HEIGHT; i++)
	{
		for (int j = 0; j < N0_MAX_COLOR_WIDTH; j++)
		{			
			nSelForeColor[i][j] = RGB(0,0,0);
			nSelBackColor[i][j] = RGB(255, 255, 255);
		}
	}

//	COLORREF nSelColor[N0_MAX_COLOR_HEIGHT][N0_MAX_COLOR_WIDTH];
//	COLORREF nSelBackColor[N0_MAX_COLOR_HEIGHT][N0_MAX_COLOR_WIDTH];

}


// ///////////////////////////////////////////////////////////////////////////
// Default Destructor (Public)
// ///////////////////////////////////////////////////////////////////////////
CListCtrlEx::~CListCtrlEx() // CLedButton::~CLedButton()
{

    //for(LedState ledState=0; ledState < m_listCount; ledState++)
    //{
    //    RemoveIcon(ledState);
    //}
    //
    //if (NULL != m_activityTimer)
    //{
    //    KillTimer(m_activityTimer);
    //    m_activityTimer = NULL;
    //}

    //m_ledDataList.RemoveAll();
    //m_listCount = 0;

}



// ///////////////////////////////////////////////////////////////////////////
// SetLedStatesNumber (Public)
// ///////////////////////////////////////////////////////////////////////////
void CListCtrlEx::SetTextColor(COLORREF lColor)
{  

}

void CListCtrlEx::SetCellBackColor(int lRow, COLORREF lColor)
{
	if (N0_MAX_COLOR_HEIGHT > lRow)
	{
		for (int j = 0; j < N0_MAX_COLOR_WIDTH; j++)
		{
			nSelBackColor[lRow][j] = lColor;
		}
	}
}

void CListCtrlEx::SetCellBackColor(int lRow, int lCol, COLORREF lColor)
{
	if ((N0_MAX_COLOR_HEIGHT > lRow)&&(N0_MAX_COLOR_WIDTH > lCol))
	{

		nSelBackColor[lRow][lCol] = lColor;
	}
}

void CListCtrlEx::SetCellForeColor(int lRow, COLORREF lColor)
{
	if (N0_MAX_COLOR_HEIGHT > lRow)
	{
		for (int j = 0; j < N0_MAX_COLOR_WIDTH; j++)
		{
			nSelForeColor[lRow][j] = lColor;
		}
	}
}

void CListCtrlEx::SetCellForeColor(int lRow, int lCol, COLORREF lColor)
{
	if ((N0_MAX_COLOR_HEIGHT > lRow) && (N0_MAX_COLOR_WIDTH > lCol))
	{
		nSelForeColor[lRow][lCol] = lColor;
	}
}