#if !defined(AFX_MESDB_H__980B4155_3EA0_48B7_9D1C_867FEEE38B97__INCLUDED_)
#define AFX_MESDB_H__980B4155_3EA0_48B7_9D1C_867FEEE38B97__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER > 1000
// MesDb.h : header file
//
#if 0
//////////////////////////////////////////////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4146)
//#import "C:\Program Files\Common Files\System\ADO\msado15.dll" \
//    rename("EOF", "EndOfFile")
 //   no_namespace rename("EOF", "EndOfFile")
//#import "C:\Program Files\Common Files\System\ADO\msado60_Backcompat.tlb" no_namespace rename("EOF", "EndOfFile")
//#import "C:\Program Files\Common Files\System\ADO\msado60_Backcompat.tlb" rename("EOF", "EndOfFile")
#pragma warning(pop)

#include <oledb.h>
#include <stdio.h>
#include <conio.h>
//-

//+2008.01.08 Add BY USY
//#define _MDBCONNECT _T("Provider=MSDASQL.1;Persist Security Info=True;"\
//        "Extended Properties=\"DSN=MS Access Database;DBQ=%s;FIL=MS Access;"\
//        "MaxBufferSize=2048;\"")
//
//using namespace ADODB;
//#include "Step.h"
//#include "Global.h"
#include "Parser.h"
//////////////////////////////////////////////////////////////////////////
//CYS TEST

#define MES_AVN_PT_TABLE		"ORADAVMES"
#define MES_AVN_KS_TABLE		"ORAKSAMES"

#define CMD_REN_MM2014	1
#define CMD_REN_ULC		2
#define CMD_HMC_HMC		3
#define CMD_GM_BYOM		4
#define CMD_GM_GEN10	5	
#define CMD_GM_TMU		6
#define CMD_VW_MIB		7

			
//CYS TEST
/////////////////////////////////////////////////////////////////////////////
// CMesDb window
class CMesDb : public CWnd
{
// Construction
public:
	CMesDb();

// Attributes
public:

// Operations
public:

	bool		m_bCoInit;
	bool		m_bDBconnection;
//	ADODB::_ConnectionPtr	m_pConn;

	CString		m_strWipId;			//Wip ID
	CString		m_strBarPrecode;
	CString		m_strBarPrePN;
	CString		m_strBarPn;
	CString		m_strBarSerial;

	CString		m_sPreCode;
	CString		m_sPrePN;
	CString		m_sMCode;
	CString		m_sSerialNumber;
	CString		m_sSku;
	CString		m_sPartNumber;

	CString		m_sOobFlag;
	CString		m_sOobFlag_Ref;
	BOOL		m_bStepCheck;

	CString	m_sStepIn;
	CString	m_sStepDesc;
	CString m_sTxNId;

	inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};

	BOOL	Mes_Open_Test(CString strDsn, CString strUserID, 
							  CString strPassWD, CString &strRtn);

	BOOL IsOpen();
	BOOL Close();
	void ParaInit();

	bool	Mes_GetFileID_Test(CString strDsn, CString strUserID, 
								   CString strPassWD, CString &strRtn);

	int		StepCheck();
	int		StepComplete(BOOL bResult);
	int		MasterInsert(BOOL bResult, int nTestItems);
	int		DetailInsert(CString sName, BOOL bResult, CString sValue);
	int		GetKeyInfoWip();
	int		GetKeyInfoSerial();
	int		GetKeyInfoWip2();
	int		GetKeyInfoSerial2();
	int		OobGetInfo();
	int		OobSetInfo();

	CString		Mes_ComErr_Make(_com_error &e);

	bool		Mes_ExcuteSql_Test(CString strDsn, CString strUserID, 
								   CString strPassWD, CString strSql, CString &strRtn);
	CString GetCmd_StepCheck();
	CString GetCmd_StepComplete();
	//CString GetCmd_MasterInsert();
	//CString GetCmd_DetailInsert();

	//CString GetCmd_KeyInfoWip();
	//CString GetCmd_KeyInfoSerial();
	//CString GetCmd_KeyInfoWip2();
	//CString GetCmd_KeyInfoSerial2();
	//CString GetCmd_OobGetInfo();
	//CString GetCmd_OobSetInfo();

	void AddStringToStatus(CString sMsg);
	CString m_strMessage;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMesDb)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMesDb();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMesDb)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
extern CMesDb		g_MesDb;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif
#endif // !defined(AFX_MESDB_H__980B4155_3EA0_48B7_9D1C_867FEEE38B97__INCLUDED_)


