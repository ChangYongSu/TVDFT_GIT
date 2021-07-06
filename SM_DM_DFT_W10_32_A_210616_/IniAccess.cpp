// IniAccess.cpp: implementation of the CIniAccess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniAccess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniAccess::CIniAccess()
{

}

CIniAccess::~CIniAccess()
{

}

VOID CIniAccess::SetIniFile(CString szName)
{
	m_szName = szName;
}

int CIniAccess::GetProfileInt(CString szSection, CString szField, INT nDefault)
{
	int nValue;

	//+change kwmoon 071015
	nValue = GetPrivateProfileInt(szSection.GetBuffer(szSection.GetLength()),
								  szField.GetBuffer(szField.GetLength()),
	//							  5, m_szName.GetBuffer(m_szName.GetLength()));
								  nDefault, m_szName.GetBuffer(m_szName.GetLength()));
	return nValue;
}

CString CIniAccess::GetProfileString(CString szSection, CString szField)
{
	CString szValue;
	DWORD	dwSize = 512; //+change 100 => 512 : 060102	
	char strval[512];

	dwSize = GetPrivateProfileString(szSection.GetBuffer(szSection.GetLength()),
								     szField.GetBuffer(szField.GetLength()),
								     "NULL", strval, dwSize, 
									 m_szName.GetBuffer(m_szName.GetLength()));


	strval[dwSize+1] = '\0';

	szValue.Format("%s",strval);
	return szValue;
}

VOID CIniAccess::SetProfileInt(CString szSection, CString szField, int nData)
{
	CString val;
	val.Format("%d",nData);
	int status = WritePrivateProfileString( szSection.GetBuffer(szSection.GetLength()),
										   szField.GetBuffer(szField.GetLength()),
										   val.GetBuffer(val.GetLength()),
										   m_szName.GetBuffer(m_szName.GetLength()) );

}

VOID CIniAccess::SetProfileString(CString szSection, CString szField, CString szData)
{
	WritePrivateProfileString( szSection.GetBuffer(szSection.GetLength()),
							   szField.GetBuffer(szField.GetLength()),
						       szData.GetBuffer(szData.GetLength()),
						       m_szName.GetBuffer(m_szName.GetLength()) );
}
