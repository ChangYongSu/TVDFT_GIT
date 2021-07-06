#pragma once


//#include "CommThread.h"



// CVF1000 명령 대상입니다.

class CListCtrlColor : public CObject
{
public:

	//Variable
	


	UINT		m_ItemCount;	
	unsigned long		m_RGB_BackColor[200][20];
	unsigned long		m_RGB_ForeColor[200][20];

	CListCtrlColor();
	virtual ~CListCtrlColor();

	void CreateColor(CListCtrl m_ctrlVoltSpecList);

	BOOL SetCellBackColor(int row, int col, unsigned long lColor);
	//BOOL SetCellBackColor(int row, int col, COLORREF lColorRef);
	unsigned long GetCellBackColor(int row, int col);
	BOOL SetCellForeColor(int row, int col, unsigned long lColor);
//	BOOL SetCellForeColor(int row, int col, COLORREF lColorRef);
	unsigned long GetCellForeColor(int row, int col);

	
private:
	//void Delay();

};


