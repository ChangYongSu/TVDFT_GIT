#include "stdafx.h"
#include "CExcel.h"
#include <io.h>

//access 함수에 사용
#define EXIST 0x00
#define W_OK  0x02
#define R_OK  0x04

CExcel::CExcel()
{
	bInit = false;

	covTrue = (short) TRUE;
	covFalse = (short) FALSE;
	covOptional = (long) DISP_E_PARAMNOTFOUND;
	covOptional.vt = VT_ERROR;
}

BOOL CExcel::InitExcel(LPCTSTR filename , LPCTSTR saveName)
{
	bInit = false;

	// Start Excel and get Application object.
	if(!app.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("Cannot start Excel and get Application object."));
		bInit = false;
	}

	books = app.get_Workbooks();

	//파일 존재 유무 확인
	if ( _access(filename,EXIST) == 0 )
	{
		//파일 존재함

		if( _access(filename,W_OK | R_OK) == 0 )
		{
			m_FileName = saveName;

			//읽기 쓰기 가능
			
			//파일 오픈
			LPDISPATCH lpdisp =  books.Open( filename,
							covOptional, covOptional, covOptional, covOptional,
							covOptional, covOptional, covOptional, covOptional,
							covOptional, covOptional, covOptional, covOptional,
							covOptional, covOptional);

			//오픈한 파일 연결
			book.AttachDispatch( lpdisp ); 

			//첫번째 시트 선택
			sheets =book.get_Sheets();
			
			//시트 선택하기
			sheet = sheets.get_Item(COleVariant((short)1));

			bInit = true;
		}
		else
		{
			//읽기 쓰기 오류 있음

			bInit = false;
		}
	}
	else
	{
		//파일 없음

		bInit = false;
	}
	


	return bInit;
}

BOOL CExcel::CloseExcel(BOOL bVisible, BOOL bSave)
{
	
	//첫번째 시트 활성화
	sheet = sheets.get_Item(COleVariant(short(1)));
	sheet.Activate();

	app.put_Visible(bVisible);
    app.put_UserControl(bVisible);

	TRY{
		
		book.put_Saved(bSave);
		book.SaveAs(COleVariant(m_FileName), 
			covOptional, covOptional, covOptional,covOptional, covOptional, 
      0, covOptional, covOptional, covOptional, covOptional, covOptional);
		//app.SaveWorkspace(COleVariant(m_FileName));
		//app.Save(COleVariant(m_FileName));
	}
	CATCH_ALL(e)
	{

	}
	END_CATCH_ALL

	cols.ReleaseDispatch();
    range.ReleaseDispatch();
    sheet.ReleaseDispatch();
    sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.ReleaseDispatch();

	return TRUE;
}

//워크시트 추가하기
BOOL CExcel::AddSheet(LPCTSTR name)
{
	bool rslt = false;

	//시트 선택하기
	//에러창 대신 리턴으로 에러 구분하기위해서 try~catch에 집어넣음
	TRY{
		//제일 마지막 시트를 가져온다
		short nsheet = sheets.get_Count();
		sheet = sheets.get_Item(COleVariant(nsheet));
		_variant_t p1(sheet.m_lpDispatch);

		//시트를 추가한다.
		sheet = sheets.Add(covOptional,p1,covOptional,covOptional);
		sheet.put_Name(name);

		rslt = true;
	}
	CATCH_ALL(e){
		rslt = false;
	}
	END_CATCH_ALL

	return rslt;
}

//워크시트 선택하기
BOOL CExcel::SelectSheet(short nSheet)
{
	bool rslt = false;

	//시트 선택하기
	//에러창 대신 리턴으로 에러 구분하기위해서 try~catch에 집어넣음
	TRY{
		sheet = sheets.get_Item(COleVariant(nSheet));
		rslt = true;
	}
	CATCH_ALL(e){
		rslt = false;
	}
	END_CATCH_ALL

	return rslt;
}

//한 셀에 입력
BOOL CExcel::SetCellValue( COleVariant cell, COleVariant value )
{
	bool rslt = false;

	TRY{

		range = sheet.get_Range(cell, cell);
		range.put_Value2(value);
		

		rslt = true;
	}
	CATCH_ALL(e){
		rslt = false;
	}
	END_CATCH_ALL

	return rslt;
}

BOOL CExcel::SetCellArray( char* cell_1, char* cell_2, COleSafeArray arr )
{
	range = sheet.get_Range( COleVariant(cell_1), COleVariant(cell_2) );
	range.put_Value2(COleVariant(arr));

	return TRUE;
}

BOOL CExcel::SetCellArray( char* cell_1, char* cell_2, COleVariant value )
{
	BOOL rslt = FALSE;

	



	return rslt;


	////현재 작업 중단
	//return FALSE;
	//
	//bool rslt = false;
	//char temp[10];
	//
	//
	//int len1 = strlen(cell_1);
	//int yCnt = 0;
	//
	//if ( len1 > 10 )
	//	return rslt;
	//
	//for ( int i = 0 ; i < len1 ; i++ )
	//{
	//	if ( cell_1[i] >= 0x30 && cell_1[i] <= 0x39 )
	//	{
	//		memcpy(temp,&cell_1[i],len1-i);
	//		break;
	//	}
	//}
	//
	//int len2 = strlen(cell_2);
	//int xPoint = 0;
	//
	//int x = (cell_2[0] - cell_1[0]);
	//int y ;
	//
	//
    //range = sheet.get_Range( COleVariant(cell_1), COleVariant(cell_2) );
	//
    //COleSafeArray saRet;
    //DWORD numElements[]={5,2};   //5x2 element array
    //saRet.Create(VT_BSTR, 2, numElements);
	//
	//
	//return rslt;
}


void CExcel::FillSafeArray(OLECHAR FAR* sz, int iRow, int iCol,
                         COleSafeArray* sa)
{
    VARIANT v;
    long index[2];

    index[0] = iRow;
    index[1] = iCol;

    VariantInit(&v);
    v.vt = VT_BSTR;
    v.bstrVal = SysAllocString(sz);
    sa->PutElement(index, v.bstrVal);
    SysFreeString(v.bstrVal);
    VariantClear(&v);
}


BOOL CExcel::InsertImage( LPCTSTR FileName , COleVariant cell_1,  COleVariant cell_2 )
{
	CRange oRange = sheet.get_Range(cell_1, cell_2);

	COleVariant VLeft, VTop, VWidth, VHeight;
	VLeft = oRange.get_Left();
	VTop = oRange.get_Top();
	VWidth = oRange.get_Width();
	VHeight = oRange.get_Height();

	CShapes oShapes = sheet.get_Shapes();
	CShape oShape;

	TRY{
		oShape = oShapes.AddPicture(

		FileName, 
		FALSE, //LinkTofile
		TRUE, //SaveWithDocument
		(float) VLeft.dblVal, //Left
		(float) VTop.dblVal, //Top
		(float) VWidth.dblVal, //Width
		(float) VHeight.dblVal //Height
		);
	}
	CATCH_ALL(e)
	{
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}