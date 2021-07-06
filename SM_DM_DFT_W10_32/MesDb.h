//#if !defined(AFX_MESDB_H__980B4155_3EA0_48B7_9D1C_867FEEE38B97__INCLUDED_)
//#define AFX_MESDB_H__980B4155_3EA0_48B7_9D1C_867FEEE38B97__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//// MesDb.h : header file
////
//
////////////////////////////////////////////////////////////////////////////
////+2006.04.04 Add BY USY
//#pragma warning(push)
//#pragma warning(disable:4146)
//#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
//    rename("EOF", "EndOfFile")
//#pragma warning(pop)
//
//#include <oledb.h>
//#include <stdio.h>
//#include <conio.h>
////-
////
//////+2008.01.08 Add BY USY
////#define _MDBCONNECT _T("Provider=MSDASQL.1;Persist Security Info=True;"\
////        "Extended Properties=\"DSN=MS Access Database;DBQ=%s;FIL=MS Access;"\
////        "MaxBufferSize=2048;\"")
//
//#include "Step.h"
//#include "Global.h"
//#include "Parser.h"
////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
//// CMesDb window
//class CMesDb : public CWnd
//{
//// Construction
//public:
//	CMesDb();
//
//// Attributes
//public:
//
//// Operations
//public:
//
//	//+2006.04.04 Add BY USY
//	bool		m_bCoInit;
//
//	//+add kwmoon 080501
//	bool		m_bDBconnection;
//
//	//+add kwmoon 080501
//	ADODB::_ConnectionPtr	m_pConn;
//
//	inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};
//
//	bool		Mes_Open_Test(CString strDsn, CString strUserID, 
//							  CString strPassWD, CString &strRtn);
//
//	bool		Mes_GetFileID_Test(CString strDsn, CString strUserID, 
//								   CString strPassWD, CString &strRtn);
//
//	bool		Mes_GetProcCode_Test(CString strDsn, CString strUserID, 
//									 CString strPassWD, CString strSn, 
//									 CString &strRtn1, CString &strRtn2);
//
//	bool		Mes_GetProcEndRst_Test(CString strDsn, CString strUserID, 
//									   CString strPassWD, CString strSn, 
//									   CString strStep, CString strLine, 
//									   CString strResult, CString &strRtn);
//	CString		Mes_ComErr_Make(_com_error &e);
//	//-
//
//	bool		Mdb_Open_Test(CString strMdbPath, CString &strRtn);
//	bool		Mdb_ExcuteSql_Test(CString strMdbPath, CString strSql, CString &strRtn);
//	
//	bool		Mes_ExcuteSql_Test(CString strDsn, CString strUserID, 
//								   CString strPassWD, CString strSql, CString &strRtn);
//	//-
//
//	//+change kwmoon 080522
////	CString		Make_Master_tb_Sql(int nType, CString strWipid, BOOL bResult, CString strUserid, float fTime);
//	CString		Make_Master_tb_Sql(int nType, int nSeqNo, CString strWipid, BOOL bResult, CString strUserid, float fTime);
//
//	//+change kwmoon 080522
////	CString		Make_Dft_tb_Sql(int nType, CString strWipid, CString strUserid, CStep* pStep);
//	CString		Make_Dft_tb_Sql(int nType, int nSeqNo, CString strWipid, CString strUserid, CStep* pStep);
//
//
//	//+add kwmoon 080522
////	bool		GetSeqNo_Master_tb_Sql(CString sCurDate, CString strWipid, CString strDsn, CString strUserID, CString strPassWD, int& nSeqNo, CString &strRtn);
////	bool		GetSeqNo_Master_tb_Sql(CString strWipid, CString strDsn, CString strUserID, CString strPassWD, int& nSeqNo, CString &strRtn);
//	// change 090603
//	bool		GetSeqNo_Master_tb_Sql(BOOL bUseLineNo, CString strWipid, CString strDsn, CString strUserID, CString strPassWD, int& nSeqNo, CString &strRtn);
//
//	//+add PSH 081203
//	CString	Make_Dft_tb_Sql_Local(CString strWipid, CStep* pStep);
//	CString	Make_Master_tb_Sql_Local(CString strWipid, BOOL bResult, float fTime);
//
//// Overrides
//	// ClassWizard generated virtual function overrides
//	//{{AFX_VIRTUAL(CMesDb)
//	//}}AFX_VIRTUAL
//
//// Implementation
//public:
//	virtual ~CMesDb();
//
//	// Generated message map functions
//protected:
//	//{{AFX_MSG(CMesDb)
//		// NOTE - the ClassWizard will add and remove member functions here.
//	//}}AFX_MSG
//	DECLARE_MESSAGE_MAP()
//};
//
///////////////////////////////////////////////////////////////////////////////
//
////{{AFX_INSERT_LOCATION}}
//// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//
//#endif // !defined(AFX_MESDB_H__980B4155_3EA0_48B7_9D1C_867FEEE38B97__INCLUDED_)
