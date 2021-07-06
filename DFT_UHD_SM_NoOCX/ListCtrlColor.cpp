// VF1000.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DATsys.h"
#include "ListCtrlColor.h"
#include "global.h"

extern CDATsysView*  g_pView;
// CVF1000



CListCtrlColor::CListCtrlColor()
{
	m_ItemCount = 0;
	
	for (int i = 0; i < m_ItemCount; i++)
	{
		for (int j = 0; i < 20; i++)
		{
			m_RGB_BackColor[i][j] = 0xFFFFFF;
			m_RGB_ForeColor[i][j] = 0xFFFFFF;
		}
	}
}

CListCtrlColor::~CListCtrlColor()
{

}


void CListCtrlColor::CreateColor(CListCtrl m_ctrlVoltSpecList)
{
	m_ItemCount = m_ctrlVoltSpecList.GetItemCount();



	for (int i = 0; i < m_ItemCount; i++)
	{
		for (int j = 0; i < 20; i++)
		{
			m_RGB_BackColor[i][j] = 0xFFFFFF;
			m_RGB_ForeColor[i][j] = 0xFFFFFF;
		}
	}


}


//
BOOL CListCtrlColor::SetCellBackColor(int row, int col, unsigned long lColor)
{
	if ((row < 200) && (col < 20))
	{
		m_RGB_BackColor[row][col] = lColor;
	}
	return TRUE;
}

//BOOL CListCtrlColor::SetCellBackColor(int row, int col, COLORREF lColorRef)
//{
//	if ((row < 200) && (col < 20))
//	{
//		m_RGB_BackColor[row][col] = unsigned long(lColorRef);
//	}
//	return TRUE;
//}
unsigned long CListCtrlColor::GetCellBackColor(int row, int col)
{
	if ((row < 200) && (col < 20))
	{
		return m_RGB_BackColor[row][col] ;
	}
	else
	{
		return 0xFFFFFF;
	}
}

BOOL CListCtrlColor::SetCellForeColor(int row, int col, unsigned long lColor)
{
	if ((row < 200) && (col < 20))
	{
		m_RGB_ForeColor[row][col] = lColor;
	}
	return TRUE;
}
//
//BOOL CListCtrlColor::SetCellForeColor(int row, int col, COLORREF lColorRef)
//{
//	if ((row < 200) && (col < 20))
//	{
//		m_RGB_ForeColor[row][col] = unsigned long(lColorRef);
//	}
//	return TRUE;
//}
unsigned long CListCtrlColor::GetCellForeColor(int row, int col)
{
	if ((row < 200) && (col < 20))
	{
		return m_RGB_ForeColor[row][col];
	}
	else
	{
		return 0xFFFFFF;
	}
}

//int CVF1000::GetVF_Count()
//{
//
//	POSITION Pos = NULL;
//	CVFTestInfo* pVFTestInfo = NULL;
//	BOOL		bRun;
//	UINT		nChNo;
//	CString		strTestName;
//	UINT		nCheckType;
//	//	double		dTarget;	
//	double		dLowerLimit;
//	double		dUpperLimit;
//	UINT		nTime = 0;
//	CString sTmp;
//	int nTestIndex = 0;
//	if (CurrentSet->VF_TestInfo.GetCount() == 0) { return 0; }
//
//	Pos = CurrentSet->VF_TestInfo.GetHeadPosition();
//
//	while (Pos)
//	{
//		pVFTestInfo = CurrentSet->VF_TestInfo.GetNext(Pos);
//		bRun = pVFTestInfo->bRun;
//		nChNo = pVFTestInfo->nChNo;
//		strTestName = pVFTestInfo->strTestName;
//		nCheckType = pVFTestInfo->nCheckType;
//		dLowerLimit = pVFTestInfo->dLowerLimit;
//		dUpperLimit = pVFTestInfo->dUpperLimit;
//		nTime = pVFTestInfo->nTime;
//		if (bRun)
//		{
//			if ((nCheckType == 1) && (dLowerLimit >= 0) && (dLowerLimit <= 0.5))
//			{
//				continue;
//			}
//
//			nTestIndex++;
//		}
//	}
//	return nTestIndex;
//}
//
