// MesDb.cpp : implementation file
//

#include "stdafx.h"
#include "DATsys.h"
#include "MesDb.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMesDb

CMesDb::CMesDb()
{
	if(FAILED(::CoInitialize(NULL))) 
	{
		m_bCoInit = false;
	}
	else
	{
		m_bCoInit = true;
	}

	//+add kwmoon 080501
	m_bDBconnection = false;

	//+add kwmoon 080501
	m_pConn = NULL;
}

CMesDb::~CMesDb()
{
	//+add kwmoon 080501
	if(m_pConn != NULL)
	{
		TESTHR(m_pConn->Close());
		m_bDBconnection = false;

		m_pConn = NULL;
	}
	
	if(m_bCoInit == true)
	{
		::CoUninitialize();
	}
	
}


BEGIN_MESSAGE_MAP(CMesDb, CWnd)
	//{{AFX_MSG_MAP(CMesDb)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMesDb message handlers

//////////////////////////////////////////////////////////////////////////

/*
===============================================================================
 함 수 명 : Mes_Open_Test()
 설    명 : 가장 기본적인 MES 접속 및 종료 테스트 
 입 력 값 : strDsn		=> MES DB DSN		default : DAVMDEV
			strUserID	=> MES DB UserID	테스트용서버ID : oraspc
			strPassWD	=> MES DB PassWD	테스트용서버Pass : spcora00
			strRtn		=> 결과값이 false일 경우 에러 메세지를 저장함
 결 과 값 : bool type
 작 성 일 : 2006.04.04
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
bool CMesDb::Mes_Open_Test(CString strDsn, CString strUserID, CString strPassWD, CString &strRtn)
{
	CString strConn, strTmp;

//	ADODB::_ConnectionPtr m_pConn = NULL;
	HRESULT	hRst = S_OK;

	//+2006.08.28 Add BY USY
	CString sString;
	//-
	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
			//strRtn.Format("DB Initialize Fail!");
			//+2006.08.28 Add BY USY
			sString.LoadString(IDS_MES_MSG);
			strRtn.Format(sString);
			//strRtn.Format("DB 접속 초기화에 실패하였습니다!");
			//-
			return false;
		}
		else{
			m_bCoInit = true;
		}
	}
	
	//DB Open
	try
	{
		TESTHR(hRst = m_pConn.CreateInstance(__uuidof(ADODB::Connection)));

		//Oracle DB 연결 문자열
		strConn = "";
		strConn = strConn + "Provider=MSDAORA;";
		strConn = strConn + "Data Source=" + strDsn + ";";
		
		TESTHR(m_pConn->Open((_bstr_t)strConn,(_bstr_t)strUserID,(_bstr_t)strPassWD,NULL));
		//연결이 제대로 되었으면 닫아준다.
//		TESTHR(m_pConn->Close());
		//연결이 성공했다는 메세지를 strRtn에 넣어주고 true 리턴
		//+2006.08.28 Add BY USY
		sString.LoadString(IDS_MES_MSG1);
		strRtn.Format(sString);
		//strRtn.Format("MES DB 연결에 성공하였습니다!\n");
		//-
		//+add kwmoon 080527
		m_bDBconnection = TRUE;
		//-
		return true;
	}
	//에러처리
	catch (_com_error &e) 
	{
		//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
		sString.LoadString(IDS_MES_MSG2);
		strRtn.Format(sString);
		//strRtn.Format("MES DB 연결에 실패하였습니다!\n");
		//-
		//+del kwmoon 080523
	//	strTmp.Format("[%s]", Mes_ComErr_Make(e));
	//	strRtn += strTmp;
		return false;
	}
	
}

/*
===============================================================================
 함 수 명 : Mes_GetFileID_Test()
 설    명 : MES에서 저장할 파일명을 가져옴
 입 력 값 : strDsn		=> MES DB DSN		default : DAVMDEV
			strUserID	=> MES DB UserID	테스트용서버ID : oraspc
			strPassWD	=> MES DB PassWD	테스트용서버Pass : spcora00
			strRtn		=> 결과값이 true일 경우 파일명을 저장함
						   결과값이 false일 경우 에러 메세지를 저장함
 결 과 값 : bool type
 작 성 일 : 2006.04.04
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
bool CMesDb::Mes_GetFileID_Test(CString strDsn, CString strUserID, CString strPassWD, CString &strRtn)
{
	CString			strConn, strTmp;
	ADODB::_ConnectionPtr	pConn = NULL;
	ADODB::_CommandPtr		pCmd  = NULL;
	ADODB::_ParameterPtr	pPara = NULL;
	ADODB::_RecordPtr		pRec  = NULL;
	HRESULT			hRst = S_OK;

	//+2006.08.28 Add BY USY
	CString sString;
	//-

	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
			//+2006.08.28 Add BY USY
			sString.LoadString(IDS_MES_MSG);
			strRtn.Format(sString);
			//strRtn.Format("DB 접속 초기화에 실패하였습니다!");
			//-
			return false;
		}
		else{
			m_bCoInit = true;
		}
	}

	
	//DB 연결
	try
	{			
		TESTHR(hRst = pConn.CreateInstance(__uuidof(ADODB::Connection)));
		
		//Oracle DB 연결 문자열
		strConn = "";
		strConn = strConn + "Provider=MSDAORA;";
		strConn = strConn + "Data Source=" + strDsn + ";";
		
		TESTHR(pConn->Open((_bstr_t)strConn,(_bstr_t)strUserID,(_bstr_t)strPassWD,NULL));
	}

	//에러처리
	catch (_com_error &e) 
	{
		//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
		sString.LoadString(IDS_MES_MSG2);
		strRtn.Format(sString);
		//strRtn.Format("MES DB 연결에 실패하였습니다!\n");
		//-
		//+del kwmoon 080523
	//strTmp.Format("[%s]", Mes_ComErr_Make(e));
	//	strRtn += strTmp;
		return false;
	}
	//-
	
	//Get File ID
	try
	{
		TESTHR(hRst = pCmd.CreateInstance(__uuidof(ADODB::Command)));

		//Oracle DB 연결 문자열
		pCmd->ActiveConnection = pConn;
		//+2006.10.17 Mod BY USY
		pCmd->CommandText = "ORAKSAVMES.MES_SPC_PKG.GET_FILE_ID";
		//pCmd->CommandText = "ORADAVMES.MES_SPC_PKG.GET_FILE_ID";
		//-
		pCmd->CommandType = ADODB::adCmdStoredProc;		

		//파라미터 Output
		//V_RETVALUE => File ID
		pPara = pCmd->CreateParameter("V_RETVALUE", ADODB::adVarChar, ADODB::adParamOutput, 15);
		pCmd->Parameters->Append(pPara);

		//V_ERR_MSG => TRUE : File ID 가져오기 성공
		//			   FALSE : File ID 가져오기 실패
		pPara = pCmd->CreateParameter("V_ERRMSG", ADODB::adVarChar, ADODB::adParamOutput, 512);
		pCmd->Parameters->Append(pPara);

		//MES DB로부터 V_ERR_MSG값  가져오기
		pRec = pCmd->Execute(NULL, NULL, ADODB::adCmdStoredProc);
		pPara = pCmd->Parameters->Item["V_ERRMSG"];
		strRtn.Format("%s",(LPCSTR)(_bstr_t)(pPara->Value));

		//V_ERR_MSG가 TRUE가 아닐경우 File ID 가져오기 실패
		if( strRtn.Compare("TRUE") != 0)
		{
			//+2006.08.28 Add BY USY
			sString.LoadString(IDS_MES_MSG3);
			strTmp.Format(sString, (const char*)strRtn);
			//strTmp.Format("MES DB에서 파일이름을 가져오는데 실패하였습니다!\n[%s]", strRtn);
			//-
			strRtn.Format("%s", (const char*)strTmp);
			return false;
		}
		//File ID 가져오기 성공
		else 
		{	
			//V_RETVALUE에 저장된 File ID를 strRtn에 저장하고 true 리턴
			pPara = pCmd->Parameters->Item["V_RETVALUE"];
			strRtn.Format("%s",(LPCSTR)(_bstr_t)(pPara->Value));
			return true;
		}
	}

	//에러처리
	catch (_com_error &e) 
	{	
		//파일 이름을 가져오기  실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
		sString.LoadString(IDS_MES_MSG7);
		strRtn.Format(sString);
		//strRtn.Format("MES DB에서 파일이름을 가져오는데 실패하였습니다!\n");
		//-
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	//-
}

/*
===============================================================================
 함 수 명 : Mes_GetProcCode_Test()
 설    명 : 공정 코드 가져오기
 입 력 값 : strDsn		=> MES DB DSN		default : DAVMDEV
			strUserID	=> MES DB UserID	테스트용서버ID : oraspc
			strPassWD	=> MES DB PassWD	테스트용서버Pass : spcora00
			strSn		=> 기판 S/N(25자리)

			strRtn1		=> 결과값이 true일 경우 공정코드를 저장함		ex)GMPBST
						   결과값이 false일 경우 에러 메세지를 저장함	

			strRtn2		=> 결과값이 true일 경우 공정코드에 대한 설명 메세지를 저장함 
						   ex)	"다음 공정은 BST 검사 입니다."
			
 결 과 값 : bool type
 작 성 일 : 2006.04.04
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
bool CMesDb::Mes_GetProcCode_Test(CString strDsn, CString strUserID, CString strPassWD, CString strSn, 
								  CString &strRtn1, CString &strRtn2)
{
	CString			strConn, strTmp;
	ADODB::_ConnectionPtr	pConn = NULL;
	ADODB::_CommandPtr		pCmd  = NULL;
	ADODB::_ParameterPtr	pPara = NULL;
	ADODB::_RecordPtr		pRec  = NULL;
	HRESULT			hRst = S_OK;

	//+2006.08.28 Add BY USY
	CString sString;
	//-

	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
			//strRtn1.Format("DB Initialize Fail!");
			//+2006.08.28 Add BY USY
			sString.LoadString(IDS_MES_MSG);
			strRtn1.Format(sString);
			//strRtn1.Format("DB 접속 초기화에 실패하였습니다!");
			//-
			return false;
		}
		else{
			m_bCoInit = true;
		}
	}

	
	//DB Open
	try
	{			
		TESTHR(hRst = pConn.CreateInstance(__uuidof(ADODB::Connection)));

		//Oracle DB 연결 문자열
		strConn = "";
		strConn = strConn + "Provider=MSDAORA;";
		strConn = strConn + "Data Source=" + strDsn + ";";
		
		TESTHR(pConn->Open((_bstr_t)strConn,(_bstr_t)strUserID,(_bstr_t)strPassWD,NULL));
	}
	//에러처리
	catch (_com_error &e) 
	{
		//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
		sString.LoadString(IDS_MES_MSG2);
		strRtn1.Format(sString);
		//strRtn1.Format("MES DB 연결에 실패하였습니다!\n");
		//-
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn1 += strTmp;
		return false;
	}
	//-
	//Get Proc Code
	try
	{
		TESTHR(hRst = pCmd.CreateInstance(__uuidof(ADODB::Command)));

		//Oracle DB 연결 문자열
		pCmd->ActiveConnection = pConn;
		//+2006.10.17 Mod BY USY
		pCmd->CommandText = "ORAKSAVMES.MES_SPC_PKG.STEP_CHECK";
		//pCmd->CommandText = "ORADAVMES.MES_SPC_PKG.STEP_CHECK";
		//-
		pCmd->CommandType = ADODB::adCmdStoredProc;

		//파라미터 Input
		//V_SET_SN => 기판 S/N(25자리)
		pPara = pCmd->CreateParameter("V_SET_SN", ADODB::adVarChar, ADODB::adParamInput, 40);
		pCmd->Parameters->Append(pPara);
		pPara->Value = (_bstr_t)strSn;

		//파라미터 Output
		//V_INSP_STEP => 공정코드
		pPara = pCmd->CreateParameter("V_INSP_STEP", ADODB::adVarChar, ADODB::adParamOutput, 30);
		pCmd->Parameters->Append(pPara);

		//V_INSP_STEP_DESC => 공정코드에 대한 설명메세지
		pPara = pCmd->CreateParameter("V_INSP_STEP_DESC", ADODB::adVarChar, ADODB::adParamOutput, 128);
		pCmd->Parameters->Append(pPara);

		//V_ERR_MSG => TRUE : 공정코드 가져오기 성공
		//			   FALSE : 공정코드 가져오기 실패
		pPara = pCmd->CreateParameter("V_ERRMSG", ADODB::adVarChar, ADODB::adParamOutput, 512);
		pCmd->Parameters->Append(pPara);
		
		//공정코드 가져오기 실행
		pRec = pCmd->Execute(NULL, NULL, ADODB::adCmdStoredProc);
		pPara = pCmd->Parameters->Item["V_ERRMSG"];
		strRtn1.Format("%s",(LPCSTR)(_bstr_t)(pPara->Value));

		//V_ERR_MSG가 TRUE가 아닐경우 공정코드 가져오기 실패
		if(strRtn1.Compare("TRUE") != 0)
		{
			//공정코드 가져오기 실패에 대한 에러메시지를 strRtn1에 저장하고 false 리턴
			strTmp.Format("[%s]", (const char*)strRtn1);
			strRtn1.Format("%s", (const char*)strTmp);
			return false;
		}
		//공정코드 가져오기 성공
		else 
		{
			//V_INSP_STEP에 저장된 공정코드를 strRtn1에 저장
			//V_INSP_STEP_DESC에 저장된 공정코드설명을 strRtn2에 저장하고 true 리턴
			pPara = pCmd->Parameters->Item["V_INSP_STEP"];
			strRtn1.Format("%s",(LPCSTR)(_bstr_t)(pPara->Value));

			pPara = pCmd->Parameters->Item["V_INSP_STEP_DESC"];
			strRtn2.Format("%s",(LPCSTR)(_bstr_t)(pPara->Value));
			return true;
		}
	}
	//에러처리
	catch (_com_error &e) 
	{
		//공정코드 가져오기  실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn1에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
		sString.Format(IDS_MES_MSG4);
		strRtn1.Format(sString);
		//strRtn1.Format("MES DB로부터 현재 공정코드를 가져오는데 실패하였습니다!\n");
		//-
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn1 += strTmp;
		return false;
	}


}

/*
===============================================================================
 함 수 명 : Mes_GetProcEndRst_Test()
 설    명 : 공정 완료 처리
 입 력 값 : strDsn		=> MES DB DSN		default : DAVMDEV
			strUserID	=> MES DB UserID	테스트용서버ID : oraspc
			strPassWD	=> MES DB PassWD	테스트용서버Pass : spcora00
			strSn		=> 기판 S/N(25자리)
			strStep		=> 검사공정코드		default : GMPBST
			strLine		=> 생산라인			default : PV03(기판라인)
			strResult	=> 테스트 결과		OK / NG
			strRtn		=> 결과값이 false일 경우 에러 메세지를 저장함

 결 과 값 : bool type
 작 성 일 : 2006.04.04
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
bool CMesDb::Mes_GetProcEndRst_Test(CString strDsn, CString strUserID, CString strPassWD, CString strSn, 
									CString strStep, CString strLine, CString strResult, CString &strRtn)
{
	CString			strConn, strTmp;
	ADODB::_ConnectionPtr	pConn = NULL;
	ADODB::_CommandPtr		pCmd  = NULL;
	ADODB::_ParameterPtr	pPara = NULL;
	ADODB::_RecordPtr		pRec  = NULL;
	HRESULT			hRst = S_OK;

	//+2006.08.28 Add BY USY
	CString sString;
	//-

	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
			//strRtn.Format("DB Initialize Fail!");
			//+2006.08.28 Add BY USY
			sString.LoadString(IDS_MES_MSG);
			strRtn.Format(sString);
			//strRtn.Format("DB 접속 초기화에 실패하였습니다!");
			//-
			return false;
		}
		else{
			m_bCoInit = true;
		}
	}

	
	//DB Open
	try
	{			
		TESTHR(hRst = pConn.CreateInstance(__uuidof(ADODB::Connection)));

		//Oracle DB 연결 문자열
		strConn = "";
		strConn = strConn + "Provider=MSDAORA;";
		strConn = strConn + "Data Source=" + strDsn + ";";
		
		TESTHR(pConn->Open((_bstr_t)strConn,(_bstr_t)strUserID,(_bstr_t)strPassWD,NULL));
	}
	//에러처리
	catch (_com_error &e) 
	{
		//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
		sString.LoadString(IDS_MES_MSG2);
		strRtn.Format(sString);
		//strRtn.Format("MES DB 연결에 실패하였습니다!\n");
		//-
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	//-
	//+Get Proc End Result
	pConn->BeginTrans();
	try
	{
		TESTHR(hRst = pCmd.CreateInstance(__uuidof(ADODB::Command)));

		//Oracle DB 연결 문자열
		pCmd->ActiveConnection = pConn;
		//+2006.10.17 Mod BY USY
		pCmd->CommandText = "ORAKSAVMES.MES_SPC_PKG.STEP_COMPLETE";
		//pCmd->CommandText = "ORADAVMES.MES_SPC_PKG.STEP_COMPLETE";
		//-
		pCmd->CommandType = ADODB::adCmdStoredProc;

		//파라미터 Input
		//V_SET_SN => 기판 S/N(25자리)
		pPara = pCmd->CreateParameter("V_SET_SN", ADODB::adVarChar, ADODB::adParamInput, 40);
		pCmd->Parameters->Append(pPara);
		pPara->Value = (_bstr_t)strSn;

		//V_INSP_STEP => 공정코드
		pPara = pCmd->CreateParameter("V_INSP_STEP", ADODB::adVarChar, ADODB::adParamInput, 30);
		pCmd->Parameters->Append(pPara);
		pPara->Value = (_bstr_t)strStep;

		//V_LINE => 생산라인
		pPara = pCmd->CreateParameter("V_LINE", ADODB::adVarChar, ADODB::adParamInput, 30);
		pCmd->Parameters->Append(pPara);
		pPara->Value = (_bstr_t)strLine;

		//V_INSP_RESULT => 테스트 결과
		pPara = pCmd->CreateParameter("V_INSP_RESULT", ADODB::adVarChar, ADODB::adParamInput, 10);
		pCmd->Parameters->Append(pPara);
		pPara->Value = (_bstr_t)strResult;

		//파라미터 Output
		//V_ERR_MSG => TRUE : 공정코드 가져오기 성공
		//			   FALSE : 공정코드 가져오기 실패
		pPara = pCmd->CreateParameter("V_ERRMSG", ADODB::adVarChar, ADODB::adParamOutput, 512);
		pCmd->Parameters->Append(pPara);
		
		//공정완료 처리 실행
		pRec = pCmd->Execute(NULL, NULL, ADODB::adCmdStoredProc);
		pPara = pCmd->Parameters->Item["V_ERRMSG"];
		strRtn.Format("%s",(LPCSTR)(_bstr_t)(pPara->Value));

		//V_ERR_MSG가 TRUE가 아닐경우 공정완료 처리 실패
		if(strRtn.Compare("TRUE") != 0)
		{
			//공정코드 가져오기 실패에 대한 에러메시지를 strRtn에 저장하고 false 리턴
			//+2006.08.28 Add BY USY
			sString.LoadString(IDS_MES_MSG5);
			strTmp.Format(sString, (const char*)strRtn);
			//strTmp.Format("공정완료 처리에 실패하였습니다!\n[%s]", strRtn);
			//-
			strRtn.Format("%s", (const char*)strTmp);
			pConn->RollbackTrans();
			return false;
		}
		
		//공정완료 처리 성공
		else 
		{	//strRtn에 메세지를 저장하지 않고 true만 리턴
			pConn->CommitTrans();
			return true;
		}
	}
	//에러처리
	catch (_com_error &e) 
	{
		//공정완료 처리 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
		sString.LoadString(IDS_MES_MSG6);
		strRtn.Format(sString);
		//strRtn.Format("공정완료 처리에 실패하였습니다!\n");
		//-
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	//-
}

/*
===============================================================================
 함 수 명 : Mes_ComErr_Make()
 설    명 : MES DB 관련 작업 진행중 에러가 발생하면 그와 관련된 에러메세지를 만들어서 넘겨줌.
 입 력 값 : e => 에러 값
			
 결 과 값 : CString type (에러메세지를 리턴해줌)
 작 성 일 : 2006.04.04
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
CString	CMesDb::Mes_ComErr_Make(_com_error &e)
{
	CString sMsg, errMsg;
	
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

	errMsg.Format("%08lx : %s\n", e.Error(), e.ErrorMessage());
	sMsg += errMsg;
	errMsg.Format("Source = %s\n", (LPCSTR) bstrSource);
	sMsg += errMsg;
	errMsg.Format("Description =%s", (LPCSTR) bstrDescription);
	sMsg += errMsg;

	return sMsg;
}
//-
//////////////////////////////////////////////////////////////////////////
/*
===============================================================================
 함 수 명 : Mdb_Open_Test()
 설    명 : MDB 기본적인 접속 테스트 
 입 력 값 : strMdbPath	=> MDB File Local Path
			strRtn		=> 결과값이 false일 경우 에러 메세지를 저장함

 결 과 값 : bool type
 작 성 일 : 2008.01.08
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
bool CMesDb::Mdb_Open_Test(CString strMdbPath, CString &strRtn)
{
	CString strConn, strTmp;
	ADODB::_ConnectionPtr pConn = NULL;
	HRESULT	hRst = S_OK;

	CString sString;

	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
			//strRtn.Format("DB Initialize Fail!");
//			sString.LoadString(IDS_MES_MSG);
//			strRtn.Format(sString);
			strRtn.Format("MDB 접속 초기화에 실패하였습니다!");
			//-
			return false;
		}
		else{
			m_bCoInit = true;
		}
	}
	
	//DB Open
	try
	{
		TESTHR(hRst = pConn.CreateInstance(__uuidof(ADODB::Connection)));

		//MDB 연결 문자열
		strConn = "";
		strConn.Format(_MDBCONNECT, (const char*)strMdbPath);

		TESTHR(pConn->Open((_bstr_t)strConn,L"", L"", -1));
		
		//연결이 제대로 되었으면 닫아준다.
		TESTHR(pConn->Close());
		//연결이 성공했다는 메세지를 strRtn에 넣어주고 true 리턴
//		sString.LoadString(IDS_MES_MSG1);
//		strRtn.Format(sString);
		strRtn.Format("MDB 연결에 성공하였습니다!\n");

		return true;
	}
	//에러처리
	catch (_com_error &e) 
	{
		//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
//		sString.LoadString(IDS_MES_MSG2);
//		strRtn.Format(sString);
		strRtn.Format("MDB 연결에 실패하였습니다!\n");

		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	return true;
}

/*
===============================================================================
 함 수 명 : Mdb_ExcuteSql_Test()
 설    명 : Sql문을 실행함
 입 력 값 : strMdbPath	=> MDB File Local Path
			strSql		=> Sql Query 		
			strRtn		=> 결과값이 false일 경우 에러 메세지를 저장함
 결 과 값 : bool type
 작 성 일 : 2008.01.08
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
bool CMesDb::Mdb_ExcuteSql_Test(CString strMdbPath, CString strSql, CString &strRtn)
{
	CString			strConn, strTmp;
	ADODB::_ConnectionPtr	pConn = NULL;
	ADODB::_CommandPtr		pCmd  = NULL;
	ADODB::_ParameterPtr	pPara = NULL;
	ADODB::_RecordPtr		pRec  = NULL;
	HRESULT			hRst = S_OK;

	CString sString;

	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
//			sString.LoadString(IDS_MES_MSG);
//			strRtn.Format(sString);
			strRtn.Format("DB 접속 초기화에 실패하였습니다!");

			return false;
		}
		else{
			m_bCoInit = true;
		}
	}

	
	//DB 연결
	try
	{			
		TESTHR(hRst = pConn.CreateInstance(__uuidof(ADODB::Connection)));
		//MDB 연결 문자열
		strConn = "";
		strConn.Format(_MDBCONNECT, (const char*)strMdbPath);

		TESTHR(pConn->Open((_bstr_t)strConn,L"", L"", -1));
	}

	//에러처리
	catch (_com_error &e) 
	{
		//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
//		sString.LoadString(IDS_MES_MSG2);
//		strRtn.Format(sString);
		strRtn.Format("MES DB 연결에 실패하였습니다!\n");

		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	//-
	
	
	try
	{
		TESTHR(hRst = pCmd.CreateInstance(__uuidof(ADODB::Command)));

		//MDB 연결 문자열
		pCmd->ActiveConnection = pConn;

		pCmd->CommandText = _bstr_t(strSql);

		pCmd->CommandType = ADODB::adCmdText; // adCmdText		adCmdUnspecified

		pRec = pCmd->Execute(NULL, NULL, ADODB::adCmdText); //adCmdText   adCmdUnspecified


		
		return true;
	}

	//에러처리 
	catch (_com_error &e) 
	{	
		//Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
//		sString.LoadString(IDS_MES_MSG7);
//		strRtn.Format(sString);
		strRtn.Format("MDB에서 Sql문을 실행하는데 실패하였습니다!\n");
		//-
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	//-
}

/*
===============================================================================
 함 수 명 : Mes_ExcuteSql_Test()
 설    명 : Sql문을 실행함
 입 력 값 : strDsn		=> MES DB DSN		default : MESDEV
			strUserID	=> MES DB UserID	테스트용서버ID : dft
			strPassWD	=> MES DB PassWD	테스트용서버Pass : dft
			strSql		=> Sql Query 		
			strRtn		=> 결과값이 false일 경우 에러 메세지를 저장함
 결 과 값 : bool type
 작 성 일 : 2008.01.08
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
bool CMesDb::Mes_ExcuteSql_Test(CString strDsn, CString strUserID, CString strPassWD, CString strSql, CString &strRtn)
{
	CString			strConn, strTmp;
	//+del kwmoon 080501
//	ADODB::_ConnectionPtr	m_pConn = NULL;
	ADODB::_CommandPtr		pCmd  = NULL;
	ADODB::_ParameterPtr	pPara = NULL;
	ADODB::_RecordPtr		pRec  = NULL;
	HRESULT			hRst = S_OK;

	CString sString;


	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
//			sString.LoadString(IDS_MES_MSG);
//			strRtn.Format(sString);
			strRtn.Format("MES Connection failed(1)!");

			return false;
		}
		else{
			m_bCoInit = true;
		}
	}

	
	//DB 연결
	
	//+add kwmoon 080501
	if(!m_bDBconnection || m_pConn == NULL)
	{
		try
		{	
			//+change kwmoon 080501		
			TESTHR(hRst = m_pConn.CreateInstance(__uuidof(ADODB::Connection)));

			//Oracle DB 연결 문자열
			strConn = "";
			strConn = strConn + "Provider=MSDAORA;";
			strConn = strConn + "Data Source=" + strDsn + ";";
			
			//+change kwmoon 080501		
			TESTHR(m_pConn->Open((_bstr_t)strConn,(_bstr_t)strUserID,(_bstr_t)strPassWD,NULL));
		}

		//에러처리
		catch (_com_error &e) 
		{
			//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
			//strRtn에 저장한 후 false 리턴
			strRtn.Format("MES Connection failed(2)!\n");
			strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
			strRtn += strTmp;

			return false;
		}
		//+add kwmoon 080501
		m_bDBconnection = TRUE;
		//-
	}	
	
	try
	{
		TESTHR(hRst = pCmd.CreateInstance(__uuidof(ADODB::Command)));

		//MDB 연결 문자열
		pCmd->ActiveConnection = m_pConn;

		pCmd->CommandText = _bstr_t(strSql);

		pCmd->CommandType = ADODB::adCmdText;		

		pRec = pCmd->Execute(NULL, NULL, ADODB::adCmdText);

		return true;
	}

	//에러처리
	catch (_com_error &e) 
	{	
		strRtn.Format("MES Excute-Sql failed!\n");
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	//-
}

//+add kwmoon 080522
//bool CMesDb::GetSeqNo_Master_tb_Sql(CString sCurDate, CString strWipid, CString strDsn, CString strUserID, CString strPassWD, int& nSeqNo, CString &strRtn)
//bool CMesDb::GetSeqNo_Master_tb_Sql(CString strWipid, CString strDsn, CString strUserID, CString strPassWD, int& nSeqNo, CString &strRtn)
// change 090603
bool CMesDb::GetSeqNo_Master_tb_Sql(BOOL bUseLineNo, CString strWipid, CString strDsn, CString strUserID, CString strPassWD, int& nSeqNo, CString &strRtn)
{
	CString			strConn, strTmp;
	ADODB::_CommandPtr		pCmd  = NULL;
	ADODB::_ParameterPtr	pPara = NULL;

	//+add kwmoon 080522
	ADODB::_RecordsetPtr	pRec  = NULL;

	HRESULT			hRst = S_OK;

	CString strSql;
	CString sString;
	_variant_t vData;

	int nCount;

	//Coinitialize가 안되어 있다면 다시 한다.
	if(m_bCoInit == false)
	{
		if(FAILED(::CoInitialize(NULL))) 
		{
			m_bCoInit = false;
//			sString.LoadString(IDS_MES_MSG);
//			strRtn.Format(sString);
			strRtn.Format("MES Connection failed(1)!");

			return false;
		}
		else{
			m_bCoInit = true;
		}
	}

	
	//DB 연결

	//+add kwmoon 080501
	if(!m_bDBconnection || m_pConn == NULL)
	{
		try
		{	
			TESTHR(hRst = m_pConn.CreateInstance(__uuidof(ADODB::Connection)));

			//Oracle DB 연결 문자열
			strConn = "";
			strConn = strConn + "Provider=MSDAORA;";
			strConn = strConn + "Data Source=" + strDsn + ";";
			
			TESTHR(m_pConn->Open((_bstr_t)strConn,(_bstr_t)strUserID,(_bstr_t)strPassWD,NULL));
		}

		//에러처리
		catch (_com_error &e) 
		{
			//연결이 실패했다는 메세지와 Mes_ComErr_Make를 통해 오류메세지를 
			//strRtn에 저장한 후 false 리턴
			strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
			strRtn += strTmp;
			strRtn.Format("MES Connection failed(2)!\n");

			return false;
		}
		//+add kwmoon 080501
		m_bDBconnection = TRUE;
		//-
	}	
	
	try
	{
		TESTHR(hRst = pCmd.CreateInstance(__uuidof(ADODB::Command)));



		if(bUseLineNo)
		{
			strSql.Format("Select count(lotid) from %s where ((lotid='%s') and (line_id=%d))"
						, (const char*)MES_DFT_MASTER_TABLE, (const char*)strWipid, CurrentSet->nLineNo);
		}
		else{
			strSql.Format("Select count(lotid) from %s where lotid='%s'"
						, (const char*)MES_DFT_MASTER_TABLE, (const char*)strWipid);
		}

//		strSql.Format("Select count(lotid) from %s where ((lotid='%s') and (line_id=%d) and (create_date >= '%s'))"
//					, (const char*)MES_DFT_MASTER_TABLE, (const char*)strWipid, CurrentSet->nLineNo, sCurDate);


		//MDB 연결 문자열
		pCmd->ActiveConnection = m_pConn;

		pCmd->CommandText = _bstr_t(strSql);

		pCmd->CommandType = ADODB::adCmdText;		

		pRec = pCmd->Execute(NULL, NULL, ADODB::adCmdText);


		nSeqNo = 1;

		vData= pRec->Fields->GetItem("count(lotid)")->Value; 
		nCount =	vData.intVal;	
		nSeqNo = nCount + 1;
/*
		while(!pRec->GetEndOfFile())
		{
			nSeqNo++;
			
			pRec->MoveNext();
		}
*/		
		return true;
	}

	//에러처리
	catch (_com_error &e) 
	{	
		//Mes_ComErr_Make를 통해 오류메세지를 
		//strRtn에 저장한 후 false 리턴
		//+2006.08.28 Add BY USY
//		sString.LoadString(IDS_MES_MSG7);
//		strRtn.Format(sString);
		strRtn.Format("MES Query-Send failed!!\n");
		//-
		//+del kwmoon 080523
		strTmp.Format("[%s]", (const char*)Mes_ComErr_Make(e));
		strRtn += strTmp;
		return false;
	}
	//-
}

/*
===============================================================================
 함 수 명 : Mes_Make_Master_tb_Sql()
 설    명 : MES 
 입 력 값 : strDsn		=> MES DB DSN		default : MESDEV
			strUserID	=> MES DB UserID	테스트용서버ID : dft
			strPassWD	=> MES DB PassWD	테스트용서버Pass : dft
			strSql		=> Sql Query 		
			strRtn		=> 결과값이 false일 경우 에러 메세지를 저장함
 결 과 값 : bool type
 작 성 일 : 2008.01.08
 작 성 자 : 엄성용(usy79@lge.com)
===============================================================================
*/
CString	CMesDb::Make_Master_tb_Sql(int nType, int nSeqNo, CString strWipid, BOOL bResult, CString strUserid, float fTime)
{
	int nLineNo	   = CurrentSet->nLineNo;
	int nMachineNo = CurrentSet->nSystemNo;

	CString sSqlString	= _T("");
	CString sResult		= _T("");
	CString sChassis	= _T("");	
	
	if(bResult == TRUE)
	{
		sResult = "OK";
	}
	else
	{
		sResult = "NG";
	}

	sChassis = CurrentSet->sChassisName; 

	sSqlString.Format("Insert into %s (seq, lotid, result, machine_no, line_id, time, chassis) \
				values(%d,'%s', '%s', %d, %d, %.1f,'%s')"
				, (const char*)MES_DFT_MASTER_TABLE, nSeqNo,(const char*)strWipid, (const char*)sResult, nMachineNo, nLineNo, fTime,(const char*)sChassis);

	return sSqlString;
}


//+change kwmoon 080522
CString	CMesDb::Make_Dft_tb_Sql(int nType, int nSeqNo, CString strWipid, CString strUserid, CStep* pStep)

{
	CString sSqlString		= _T("");
	CString sTmp			= _T("");
	CString sResult			= _T("");
	CString sTable			= _T("");
	//+add kwmoon 080509
	CString sAudioTarget	= _T("");
	CString sAudioMeasure	= _T("");

	//+add kwmoon 080507
	int nLineNo	   = CurrentSet->nLineNo;
	int nMachineNo = CurrentSet->nSystemNo;

	if(pStep->m_bTest)
	{
		if (pStep->m_bResult)
		{
			sResult = _T("OK");
		}
		else // Video Test NG
		{
			//090324
			sResult = _T("NG");
			if(pStep->m_bRunAudioTest && pStep->m_bRunVideoTest)
			{

				if(pStep->m_bVideoTestResult == FALSE)
				{
					sResult = _T("NG/V");
				}

				if(pStep->m_bAudioTestResult == FALSE)
				{
					sResult = _T("NG/A");
				}

				if((pStep->m_bVideoTestResult == FALSE) && (pStep->m_bAudioTestResult == FALSE))
				{
					sResult = _T("NG/AV");
				}
			}
			else if(!pStep->m_bRunAudioTest && pStep->m_bRunVideoTest)
			{
				if(pStep->m_bVideoTestResult == FALSE)
				{
					sResult = _T("NG/V");
				}
			}
			else if(pStep->m_bRunAudioTest && !pStep->m_bRunVideoTest)
			{
				if(pStep->m_bAudioTestResult == FALSE)
				{
					sResult = _T("NG/A");
				}
			}
			else
			{
				sResult = _T("NG");
			}
		}
	}
	else sResult = _T("SKIP");

	if(nType == 0)
	{
		//+add kwmoon 080509
		if(pStep->m_sName == _T(""))  pStep->m_sName  = "-";
		if(pStep->m_strMsg == _T("")) pStep->m_strMsg = "-";

		// MES Table
		if(pStep->m_nStepType == PROCESS_FUNCTION)
		{
			sSqlString.Format("insert into %s (seq, lotid, run, step_no, step_name, time, message, machine_no, line_id)\
							   values (%d, '%s', %d, %d, '%s', %.1f, '%s', %d, %d) "
							   ,MES_DFT_TABLE, nSeqNo, strWipid, pStep->m_bTest, pStep->m_nStep
							   ,pStep->m_sName, pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);
							//   ,pStep->m_sName, pStep->m_fElapsedTime, "-", nMachineNo, nLineNo);

		}
		else if(pStep->m_nStepType == CHECK_FUNCTION)
		{
			//+add kwmoon 080509
			if(pStep->m_bRunAudioTest)
			{
				// Audio Target 090525
				//sAudioTarget.Format(_T("%d,%d,%d,%d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				if(pStep->m_nMeasureAudioType == LEVEL_CHECK){
					sAudioTarget.Format(_T("X, X, %d, %d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				else if(pStep->m_nMeasureAudioType == FREQUENCY_CHECK){
					sAudioTarget.Format(_T("%d, %d, X, X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
				}
				else{
					sAudioTarget.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				// Audio Measure
				sAudioMeasure.Format(_T("%d,%d,%d,%d"), pStep->m_nLeftFreq[1], pStep->m_nRightFreq[1], pStep->m_nLeftLevel[1], pStep->m_nRightLevel[1]);
			}

			if(pStep->m_nFuncType == MEAS_BOOL)
			{
				//+add kwmoon 080509
				if(pStep->m_bRunAudioTest)
				{
					sSqlString.Format("insert into %s (seq, lotid, run, step_no, step_name, result\
									   ,a_target, a_measure, time, message, machine_no, line_id) \
										values (%d, '%s', %d, %d, '%s', '%s','%s','%s',%.1f,'%s', %d, %d) "
									   ,MES_DFT_TABLE, nSeqNo, strWipid, pStep->m_bTest, pStep->m_nStep
									   ,pStep->m_sName, sResult,sAudioTarget, sAudioMeasure, pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);
									//   ,pStep->m_sName, sResult,sAudioTarget, sAudioMeasure, pStep->m_fElapsedTime, "-", nMachineNo, nLineNo);
				}
				else
				{
					if(pStep->m_bAdcValueCheckStep)
					{
						sSqlString.Format("insert into %s (seq, lotid, run, step_no, step_name, result\
										   , a_measure, time, message, machine_no, line_id) \
											values (%d, '%s', %d, %d, '%s', '%s','%s',%.1f,'%s', %d, %d) "
										   ,MES_DFT_TABLE, nSeqNo, strWipid, pStep->m_bTest, pStep->m_nStep
										   ,pStep->m_sName, sResult, pStep->m_szAdcValue, pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);
					}
					else
					{
						sSqlString.Format("insert into %s (seq, lotid, run, step_no, step_name, result\
										   ,time, message, machine_no, line_id) \
											values (%d, '%s', %d, %d, '%s', '%s',%.1f,'%s', %d, %d) "
										   ,MES_DFT_TABLE, nSeqNo, strWipid, pStep->m_bTest, pStep->m_nStep
										   ,pStep->m_sName, sResult,pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);
									//	   ,pStep->m_sName, sResult,pStep->m_fElapsedTime, "-", nMachineNo, nLineNo);
					}
				}
			}
			else if(pStep->m_nFuncType == MEAS_DOUBLE)
			{
				sSqlString.Format("insert into %s (seq, lotid, run, step_no, step_name, result, measure, target, lower_limit, upper_limit \
								   ,a_target, a_measure, time, message, machine_no, line_id) \
									values (%d, '%s', %d, %d, '%s', '%s', %.1f, %.1f, %.1f, %.1f, '%s','%s', %.1f, '%s', %d, %d) "
								   ,MES_DFT_TABLE, nSeqNo, strWipid, pStep->m_bTest, pStep->m_nStep
								   ,pStep->m_sName, sResult, pStep->m_fMeasured, pStep->m_fNominal, pStep->m_fLowLimit, pStep->m_fHighLimit
								   ,sAudioTarget, sAudioMeasure, pStep->m_fElapsedTime,pStep->m_strMsg, nMachineNo, nLineNo);
							//	   ,sAudioTarget, sAudioMeasure, pStep->m_fElapsedTime,"-", nMachineNo, nLineNo);
			}
		}
	}
	return sSqlString;
}

CString	CMesDb::Make_Dft_tb_Sql_Local(CString strWipid, CStep* pStep)
{
	CString sSqlString		= _T("");
	CString sTmp			= _T("");
	CString sResult			= _T("");
	CString sTable			= _T("");
	//+add kwmoon 080509
	CString sAudioTarget	= _T("");
	CString sAudioMeasure	= _T("");

	//+add kwmoon 080507
	int nLineNo	   = CurrentSet->nLineNo;
	int nMachineNo = CurrentSet->nSystemNo;

	if(pStep->m_bTest)
	{
		if (pStep->m_bResult)
		{
			sResult = _T("OK");
		}
		else // Video Test NG
		{
			//090324
			sResult = _T("NG");
			if(pStep->m_bRunAudioTest && pStep->m_bRunVideoTest)
			{

				if(pStep->m_bVideoTestResult == FALSE)
				{
					sResult = _T("NG/V");
				}

				if(pStep->m_bAudioTestResult == FALSE)
				{
					sResult = _T("NG/A");
				}

				if((pStep->m_bVideoTestResult == FALSE) && (pStep->m_bAudioTestResult == FALSE))
				{
					sResult = _T("NG/AV");
				}
			}
			else if(!pStep->m_bRunAudioTest && pStep->m_bRunVideoTest)
			{
				if(pStep->m_bVideoTestResult == FALSE)
				{
					sResult = _T("NG/V");
				}
			}
			else if(pStep->m_bRunAudioTest && !pStep->m_bRunVideoTest)
			{
				if(pStep->m_bAudioTestResult == FALSE)
				{
					sResult = _T("NG/A");
				}
			}
			else
			{
				sResult = _T("NG");
			}
		}
	}
	else sResult = _T("SKIP");

	if(pStep->m_sName == _T(""))  pStep->m_sName  = "-";
	if(pStep->m_strMsg == _T("")) pStep->m_strMsg = "-";

	// MES Table
	if(pStep->m_nStepType == PROCESS_FUNCTION)
	{
		sTmp.Format(" insert into %s (seq, lotid, run, step_no, step_name, time, message, machine_no, line_id) values (", MES_DFT_TABLE);
		sSqlString = sTmp + "%d";

		sTmp.Format(", '%s', %d, %d, '%s', %.1f, '%s', %d, %d) "
						   , strWipid, pStep->m_bTest, pStep->m_nStep
						   ,pStep->m_sName, pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);

		sSqlString = sSqlString + sTmp;
	}
	else if(pStep->m_nStepType == CHECK_FUNCTION)
	{
		//+add kwmoon 080509
		if(pStep->m_bRunAudioTest)
		{
				// Audio Target 090525
				//sAudioTarget.Format(_T("%d,%d,%d,%d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				if(pStep->m_nMeasureAudioType == LEVEL_CHECK){
					sAudioTarget.Format(_T("X, X, %d, %d"), pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
				else if(pStep->m_nMeasureAudioType == FREQUENCY_CHECK){
					sAudioTarget.Format(_T("%d, %d, X, X"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0]);
				}
				else{
					sAudioTarget.Format(_T("%d, %d, %d, %d"), pStep->m_nLeftFreq[0], pStep->m_nRightFreq[0], pStep->m_nLeftLevel[0], pStep->m_nRightLevel[0]);
				}
			// Audio Measure
			sAudioMeasure.Format(_T("%d,%d,%d,%d"), pStep->m_nLeftFreq[1], pStep->m_nRightFreq[1], pStep->m_nLeftLevel[1], pStep->m_nRightLevel[1]);
		}

		if(pStep->m_nFuncType == MEAS_BOOL)
		{
			//+add kwmoon 080509
			if(pStep->m_bRunAudioTest)
			{
				sTmp.Format("insert into %s (seq, lotid, run, step_no, step_name, result, a_target, a_measure, time, message, machine_no, line_id) values (", MES_DFT_TABLE);
				sSqlString = sTmp + "%d";
				sTmp.Format(", '%s', %d, %d, '%s', '%s','%s','%s',%.1f,'%s', %d, %d) "
								   , strWipid, pStep->m_bTest, pStep->m_nStep
								   ,pStep->m_sName, sResult,sAudioTarget, sAudioMeasure, pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);
				sSqlString = sSqlString + sTmp;
			}
			else
			{
				if(pStep->m_bAdcValueCheckStep)
				{
					sTmp.Format("insert into %s (seq, lotid, run, step_no, step_name, result, a_measure, time, message, machine_no, line_id) values (", MES_DFT_TABLE);
					sSqlString = sTmp + "%d";
					sTmp.Format(", '%s', %d, %d, '%s', '%s','%s',%.1f,'%s', %d, %d) "
									   , strWipid, pStep->m_bTest, pStep->m_nStep
									   ,pStep->m_sName, sResult,pStep->m_szAdcValue,pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);
					sSqlString = sSqlString + sTmp;
				}
				else
				{
					sTmp.Format("insert into %s (seq, lotid, run, step_no, step_name, result, time, message, machine_no, line_id) values (", MES_DFT_TABLE);
					sSqlString = sTmp + "%d";
					sTmp.Format(", '%s', %d, %d, '%s', '%s',%.1f,'%s', %d, %d) "
									   , strWipid, pStep->m_bTest, pStep->m_nStep
									   ,pStep->m_sName, sResult,pStep->m_fElapsedTime, pStep->m_strMsg, nMachineNo, nLineNo);
					sSqlString = sSqlString + sTmp;
				}
			}
		}
		else if(pStep->m_nFuncType == MEAS_DOUBLE)
		{
			sTmp.Format("insert into %s (seq, lotid, run, step_no, step_name, result, measure, target, lower_limit, upper_limit, a_target, a_measure, time, message, machine_no, line_id) values (", MES_DFT_TABLE);
			sSqlString = sTmp + "%d";
			sTmp.Format(", '%s', %d, %d, '%s', '%s', %.1f, %.1f, %.1f, %.1f, '%s','%s', %.1f, '%s', %d, %d) "
							   , strWipid, pStep->m_bTest, pStep->m_nStep
							   ,pStep->m_sName, sResult, pStep->m_fMeasured, pStep->m_fNominal, pStep->m_fLowLimit, pStep->m_fHighLimit
							   ,sAudioTarget, sAudioMeasure, pStep->m_fElapsedTime,pStep->m_strMsg, nMachineNo, nLineNo);
			sSqlString = sSqlString + sTmp;
		}
	}
	return sSqlString;
}
CString	CMesDb::Make_Master_tb_Sql_Local(CString strWipid, BOOL bResult, float fTime)
{
	int nLineNo	   = CurrentSet->nLineNo;
	int nMachineNo = CurrentSet->nSystemNo;

	CString sSqlString	= _T("");
	CString sResult		= _T("");
	CString sChassis	= _T("");	
	CString sTmp			= _T("");
	
	if(bResult == TRUE)
	{
		sResult = "OK";
	}
	else
	{
		sResult = "NG";
	}

	sChassis = CurrentSet->sChassisName; 

	sTmp.Format("Insert into %s (seq, lotid, result, machine_no, line_id, time, chassis) values(", MES_DFT_MASTER_TABLE);
	sSqlString = sTmp + "%d";
	
	sTmp.Format(",'%s', '%s', %d, %d, %.1f,'%s')", strWipid, sResult, nMachineNo, nLineNo, fTime, sChassis);
	sSqlString = sSqlString + sTmp;

	return sSqlString;
}
