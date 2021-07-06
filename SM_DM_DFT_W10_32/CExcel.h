#pragma once
#include <afxtempl.h>
#include "CApplication.h"
//#include "CPicture.h"
//#include "CPictures.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CShape.h"
#include "CShapes.h"

class CExcel
{

public :
	CExcel();

	
	CApplication app;
	CWorkbooks books;
    CWorkbook book;

    CWorksheets sheets;
    CWorksheet sheet;
    CRange range;
    CRange cols;


	COleVariant covTrue;
	COleVariant covFalse;
	COleVariant covOptional;

protected:
	//엑셀 init완료됬는지 체크 하는 변수
	bool bInit;

public:
	LPCTSTR m_FileName;

	//엑셀 시작
	BOOL InitExcel( LPCTSTR defname ,LPCTSTR saveName);

	//엑셀 종료
	BOOL CloseExcel(BOOL bVisible, BOOL bSave);

	//시트 추가
	BOOL AddSheet(LPCTSTR name);

	//시트 선택
	BOOL SelectSheet( short nSheet );

	//셀에 값 입력
	//셀 하나에 입력
	BOOL SetCellValue( COleVariant cell, COleVariant value );

	//배열 입력
	BOOL SetCellArray( char* cell_1, char* cell_2, COleVariant value );
	BOOL SetCellArray( char* cell_1, char* cell_2, COleSafeArray arr );

	//셀에 범위 입력시 COleSafeArray에 값 입력 하는 용도
	void FillSafeArray(OLECHAR FAR* sz, int iRow, int iCol, COleSafeArray* sa);


	//이미지 삽입
	BOOL InsertImage( LPCTSTR FileName , COleVariant cell_1,  COleVariant cell_2 );

};
