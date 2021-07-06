//////////////////////////////////////////////////////////////////////////////////////////
//   
//   CMTest Program 
//                                                             WRITTEN BY NOH KYOUNG DUK
//                                                             LG PRC 검사기술그룹 
//
//	 DATE : 2003. 1. 17
//---------------------------------------------------------------------------------------
//   DATE             :  DESCRIPTION
//---------------------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------------------
//   BUG NOTE // WORK NOTE
//   DATE             :  DESCRIPTION
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
//   DESCRIPTION 
//---------------------------------------------------------------------------------------
//  
///////////////////////////////////////////////////////////////////////////////////////////
// IniAccess.h: interface for the CIniAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIACCESS_H__8984605C_F4C1_48BD_AABF_1BCBCCA3DCF9__INCLUDED_)
#define AFX_INIACCESS_H__8984605C_F4C1_48BD_AABF_1BCBCCA3DCF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniAccess  
{
public:
	VOID SetProfileString(CString szSection, CString szField, CString szData);
	VOID SetProfileInt(CString szSection, CString szField, int nData);

	CString GetProfileString(CString section, CString field);
	int GetProfileInt(CString section, CString field, INT nDefault=0);
	
	VOID SetIniFile(CString name);
	
	CIniAccess();
	virtual ~CIniAccess();

protected:
	CString m_szName;
};

#endif // !defined(AFX_INIACCESS_H__8984605C_F4C1_48BD_AABF_1BCBCCA3DCF9__INCLUDED_)
