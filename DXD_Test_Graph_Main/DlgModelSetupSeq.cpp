// DlgModelSetupSeq.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgModelSetupSeq.h"
#include "afxdialogex.h"
#include "FileControl.h"
#include "DlgNewModelName.h"
#include "DlgPassword.h"
#include "DlgConfig.h"

//#include "DlgImage.h"
extern CAcqVoltageSamples_IntClkDlg *m_P_Main;

// DlgModelSetupSeq 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgModelSetupSeq, CDialogEx)

DlgModelSetupSeq::DlgModelSetupSeq(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgModelSetupSeq::IDD, pParent)
	//, m_IPaddressGmes(0)
	//, m_ServerPortNumber(_T(""))
	//, m_EditCheckSum(_T(""))
	//, m_EditFreq[0](_T(""))
	, m_EditFPGAVer(_T(""))
	, m_EditDXD_ServerVer(_T(""))
	, m_EditMicomVer(_T(""))
	, m_EditAVR_Ver(_T(""))
	, m_Radio_HK_HQ(0)
{

}

DlgModelSetupSeq::~DlgModelSetupSeq()
{
}

void DlgModelSetupSeq::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEST_PROCESS, m_CtrlListTestProcess);
	//	DDX_Control(pDX, IDC_COMBO_MODEL, m_cSeqProcessCombo);
	//DDX_Control(pDX, IDC_COMBO_RECOMON, m_cRemoconCombo);
	DDX_Control(pDX, IDC_COMBO_RECOMON2, m_cModelComboSeq);
	DDX_Control(pDX, IDC_LABEL_MODEL_NAME, m_TextModelName);


	//	DDX_Check(pDX, IDC_CHECK_MES_ENABLE, m_CheckMesEnable);	
	//DDX_Control(pDX, IDC_LABEL_MES_TNS, m_TextMesTns);
	//DDX_Control(pDX, IDC_LABEL_MES_AVN, m_TextMesAvn);
	//DDX_Control(pDX, IDC_LABEL_MES_USERID, m_TextMesUserID);
	//DDX_Control(pDX, IDC_LABEL_MES_PASSWORD, m_TextMesPassword);
	//DDX_Control(pDX, IDC_LABEL_MES_LINE, m_TextMesLine);
	//DDX_Control(pDX, IDC_LABEL_MES_INSP_STEP, m_TextMesInspStep);

	//DDX_Control(pDX, IDC_COMBO_RMT_SEQ_TYPE, m_cComboRmtType);
	//DDX_IPAddress(pDX, IDC_IPADDRESS_GMES, m_IPaddressGmes);
	//	DDX_Text(pDX, IDC_EDIT_SERVER_PORT1, m_ServerPortNumber);
	DDX_Text(pDX, IDC_EDIT_FPGA_VER_MD, m_EditFPGAVer);
	DDX_Text(pDX, IDC_EDIT_DXD_SERVER_VER_MD, m_EditDXD_ServerVer);
	DDX_Text(pDX, IDC_EDIT_MICOM_VER_MD, m_EditMicomVer);
	DDX_Text(pDX, IDC_EDIT_AVR_VER_MD, m_EditAVR_Ver);
	//	DDX_Radio(pDX, IDC_RADIO_HK, m_Radio_HK_HQ);
	//DDX_Radio(pDX, IDC_RADIO_HK, m_Radio_HK_HQ);
	DDX_Control(pDX, IDC_COMBO_MICOM_HKHQ_TYPE_MD, m_cComboMicomHKHQ);

	DDX_Text(pDX, IDC_EDIT_ROIC_MIN1, m_EditRoicMinValue[0]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN2, m_EditRoicMinValue[1]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN3, m_EditRoicMinValue[2]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN4, m_EditRoicMinValue[3]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN5, m_EditRoicMinValue[4]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN6, m_EditRoicMinValue[5]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN7, m_EditRoicMinValue[6]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN8, m_EditRoicMinValue[7]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN9, m_EditRoicMinValue[8]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN10, m_EditRoicMinValue[9]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN11, m_EditRoicMinValue[10]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN12, m_EditRoicMinValue[11]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN13, m_EditRoicMinValue[12]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN14, m_EditRoicMinValue[13]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN15, m_EditRoicMinValue[14]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MIN16, m_EditRoicMinValue[15]);


	DDX_Text(pDX, IDC_EDIT_ROIC_MAX1, m_EditRoicMaxValue[0]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX2, m_EditRoicMaxValue[1]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX3, m_EditRoicMaxValue[2]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX4, m_EditRoicMaxValue[3]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX5, m_EditRoicMaxValue[4]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX6, m_EditRoicMaxValue[5]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX7, m_EditRoicMaxValue[6]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX8, m_EditRoicMaxValue[7]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX9, m_EditRoicMaxValue[8]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX10, m_EditRoicMaxValue[9]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX11, m_EditRoicMaxValue[10]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX12, m_EditRoicMaxValue[11]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX13, m_EditRoicMaxValue[12]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX14, m_EditRoicMaxValue[13]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX15, m_EditRoicMaxValue[14]);
	DDX_Text(pDX, IDC_EDIT_ROIC_MAX16, m_EditRoicMaxValue[15]);
	DDX_Control(pDX, IDC_COMBO_MODEL_NAME_MD, m_cComboSetModelName);
}


BEGIN_MESSAGE_MAP(DlgModelSetupSeq, CDialogEx)
	//ON_EN_CHANGE(IDC_EDIT_CAMERA_IPADDRESS, &DlgModelSetupSeq::OnEnChangeEditCameraIpaddress)
	ON_CBN_SELCHANGE(IDC_COMBO_RECOMON2, &DlgModelSetupSeq::OnCbnSelchangeComboRecomon2)
	ON_BN_CLICKED(IDOK, &DlgModelSetupSeq::OnBnClickedOk)

//	ON_MESSAGE(WM_COMM_RXCHAR_PCB,OnReceiveDataPCB)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &DlgModelSetupSeq::OnBnClickedCancel)
//	ON_CBN_SELCHANGE(IDC_COMBO_MES_PORT_TYPE, &DlgModelSetupSeq::OnCbnSelchangeComboMesPortType)
//	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &DlgModelSetupSeq::OnBnClickedButtonSaveas)
ON_BN_CLICKED(IDC_BUTTON_GESWVER_MD, &DlgModelSetupSeq::OnBnClickedButtonGeswverMd)
ON_CBN_SELCHANGE(IDC_COMBO_MICOM_HKHQ_TYPE_MD, &DlgModelSetupSeq::OnCbnSelchangeComboMicomHkhqTypeMd)
ON_BN_CLICKED(IDC_BUTTON_GET_MODEL_NAME, &DlgModelSetupSeq::OnBnClickedButtonGetModelName)
END_MESSAGE_MAP()


// DlgModelSetupSeq 메시지 처리기입니다.
BOOL DlgModelSetupSeq::OnInitDialog()
{
    CDialog::OnInitDialog();
  
	//::m_bWindowOnly = TRUE;
	int lXpos = 0;
	int lYpos = 0;
//


	GetModelList();
	GetSeqList();
//	GetRemoconList();


	m_TextModelName.put_Caption(gTestSeqData.strModelName);
	SetListControl();

	LoadTestData();

	DisplayTestData();

	SetTimer(1, 30, NULL);
	m_PCB_COM_PROCESS =  COM_MODE_IDLE;

	m_TestOrgSeqData = gTestSeqData;

    return TRUE;
}

void DlgModelSetupSeq::GetModelList()
{
	BOOL rtnValue = 0;
	
	CString pathName;
	CString fileName;

//	int lFileRename = 0;
	//CString PathNameList[200];
	CString fileNameList[2000];
	
	//CString titleNameList[200];
	//CString  accpathName;
	//CString  ukpathName;
	//CString  chkpathName;
	char lBuf[2048];
	char ltmpBuf[2048];


	int nFileCnt = 0;
	CString str;

	CFileFind finder;
	// build a string with wildcards   
	CString strWildcard;
		

	//int nIndexPos = 0;
	BOOL bFileFound = 0;
	//BOOL rtnValue = 0;
	CFileException ex;

   
	strWildcard = PATH_BASE_DIR;
	strWildcard += _T("\\Model\\*.mdl");//"C:\\Program Files\\SM Digital\\nagra\\HDCPKey"
  
	BOOL bWorking = finder.FindFile(strWildcard);
	
	nFileCnt = 0;
	bFileFound = 0;
	while (bWorking)
	{   
		bWorking = finder.FindNextFile();
		
		// skip . and .. files; otherwise, we'd
		// recur infinitely!!
		fileName = finder.GetFileName();
		//str = (LPCTSTR)finder.GetRoot();
		//str = finder.GetFileTitle();
		pathName = finder.GetFilePath();
		//str = finder.GetFileURL();
		//str = finder.GetFileName();

	//	PathNameList[nFileCnt] = pathName;
		fileNameList[nFileCnt] = fileName;
	//	titleNameList[nFileCnt] = finder.GetFileTitle();
			
		nFileCnt++;
		if(nFileCnt >= 1000)
		{
				
			break;
		}
		bFileFound = 1;
				
	}

	finder.Close();


	if(bFileFound)
	{
		int l_nModelSelNum = 0;
	/*	int litemcount = m_cModelComboSeq.GetCount();
		for(int i = 0; i < litemcount; i++)
		{
			m_cModelComboSeq.ResetContent();
		}*/
		
		m_cModelComboSeq.ResetContent();
		
		for(int i = 0 ; i < nFileCnt; i++)
		{
			m_strModelNameList[i] = fileNameList[i].Left(fileNameList[i].GetLength() - 4);

			m_cModelComboSeq.AddString(m_strModelNameList[i]);
		
			if(m_strModelNameList[i].Compare(gTestSeqData.strModelName) == 0)
			{
				l_nModelSelNum = i;
				//break;
			}
		}

		m_cModelComboSeq.SetCurSel(l_nModelSelNum);

	}
	else
	{
		MessageBox("Model Test File(*.mdl) Not Found!!");
	}
}

void DlgModelSetupSeq::GetSeqList()
{
	BOOL rtnValue = 0;
	
	CString pathName;
	CString fileName;

//	int lFileRename = 0;
	//CString PathNameList[200];
	CString fileNameList[1000];
	
	//CString titleNameList[200];
	//CString  accpathName;
	//CString  ukpathName;
	//CString  chkpathName;
	char lBuf[2048];
	char ltmpBuf[2048];


	int nFileCnt = 0;
	CString str;

	CFileFind finder;
	// build a string with wildcards   
	CString strWildcard;
		

	//int nIndexPos = 0;
	BOOL bFileFound = 0;
	//BOOL rtnValue = 0;
	CFileException ex;

   
	strWildcard = PATH_BASE_DIR;
	strWildcard += _T("\\Model\\*.seq");//"C:\\Program Files\\SM Digital\\nagra\\HDCPKey"
  
	BOOL bWorking = finder.FindFile(strWildcard);
	
	nFileCnt = 0;
	bFileFound = 0;
	while (bWorking)
	{   
		bWorking = finder.FindNextFile();
		
		// skip . and .. files; otherwise, we'd
		// recur infinitely!!
		fileName = finder.GetFileName();
		//str = (LPCTSTR)finder.GetRoot();
		//str = finder.GetFileTitle();
		pathName = finder.GetFilePath();
		//str = finder.GetFileURL();
		//str = finder.GetFileName();

	//	PathNameList[nFileCnt] = pathName;
		fileNameList[nFileCnt] = fileName;
	//	titleNameList[nFileCnt] = finder.GetFileTitle();
			
		nFileCnt++;
		if(nFileCnt >= 1000)
		{
				
			break;
		}
		bFileFound = 1;
				
	}

	finder.Close();


	if(bFileFound)
	{
		int l_nModelSelNum = 0;
		
		for(int i = 0 ; i < nFileCnt; i++)
		{
//			m_strModelSeqList[i] = fileNameList[i].Left(fileNameList[i].GetLength() - 4);
			m_strModelSeqList[i] = fileNameList[i];

		/*	m_cSeqProcessCombo.AddString(m_strModelSeqList[i]);
		
			if(m_strModelSeqList[i].Compare(gTestSeqData.strSeqFileName) == 0)
			{
				l_nModelSelNum = i;
			}*/
		}

	//	m_cSeqProcessCombo.SetCurSel(l_nModelSelNum);

	}
	else
	{
		MessageBox("Model Test File(*.seq) Not Found!!");
	}
}


//void DlgModelSetupSeq::GetRemoconList()
//{
//
//	BOOL rtnValue = 0;
//	
//	CString pathName;
//	CString fileName;
//
////	int lFileRename = 0;
//	//CString PathNameList[200];
//	CString fileNameList[1000];
//	
//
////	CString m_strRemoconList[100];
////	int m_nRecomonCount = 0;
//	//CString titleNameList[200];
//	//CString  accpathName;
//	//CString  ukpathName;
//	//CString  chkpathName;
//	char lBuf[2048];
//	char ltmpBuf[2048];
//
//
//	int nFileCnt = 0;
//	CString str;
//
//	CFileFind finder;
//	// build a string with wildcards   
//	CString strWildcard;
//		
//
//		//int nIndexPos = 0;
//		BOOL bFileFound = 0;
//		//BOOL rtnValue = 0;
//		CFileException ex;
//
//   
//		strWildcard = PATH_BASE_DIR;
//		strWildcard += _T("\\Remote\\*.rmt");//"C:\\Program Files\\SM Digital\\nagra\\HDCPKey"
//  
//		BOOL bWorking = finder.FindFile(strWildcard);
//	
//		nFileCnt = 0;
//		bFileFound = 0;
//		while (bWorking)
//		{   
//			bWorking = finder.FindNextFile();
//		
//			// skip . and .. files; otherwise, we'd
//			// recur infinitely!!
//			fileName = finder.GetFileName();
//			//str = (LPCTSTR)finder.GetRoot();
//			//str = finder.GetFileTitle();
//			pathName = finder.GetFilePath();
//			//str = finder.GetFileURL();
//			//str = finder.GetFileName();
//
//		//	PathNameList[nFileCnt] = pathName;
//			fileNameList[nFileCnt] = fileName;
//		//	titleNameList[nFileCnt] = finder.GetFileTitle();
//			
//			nFileCnt++;
//			if(nFileCnt >= 100)
//			{
//				
//				break;
//			}
//			bFileFound = 1;
//				
//		}
//
//		finder.Close();
//
//
//		if(bFileFound)
//		{
//			/*
//			CString m_strRemoconList[100];
//			int m_nRecomonCount = 0;
//			*/
//			g_nRemoconSelNum = 0;//g_strRemoconFileName
//			m_nRecomonCount = nFileCnt;
//			for(int i = 0 ; i < nFileCnt; i++)
//			{
//				m_strRemoconList[i] = fileNameList[i];
//				m_cRemoconCombo.AddString(m_strRemoconList[i]);
//				
//				if(m_strRemoconList[i].Compare(gTestSeqData.strRemoconFileName) == 0)
//				{
//					g_nRemoconSelNum = i;
//				}
//			}
//
//			m_cRemoconCombo.SetCurSel(g_nRemoconSelNum);
//		}
//		else
//		{
//			MessageBox("Remote Control File(*.rmt) Not Found!!");
//		}
//}


void DlgModelSetupSeq::SetListControl()
{
	
//	m_CtrlListTestProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListTestProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListTestProcess.InsertColumn(0, _T("No."), LVCFMT_CENTER, 40); 
	m_CtrlListTestProcess.InsertColumn(1, _T("TEST"), LVCFMT_CENTER, 150);// 리스트 행 추가
	m_CtrlListTestProcess.InsertColumn(2, _T("Discription"), LVCFMT_LEFT, 350);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(3, _T("Wait"), LVCFMT_LEFT, 50);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(4, _T("Time"), LVCFMT_CENTER, 70);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(5, _T("Ref1"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(6, _T("Ref2"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(7, _T("Value"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(8, _T("OK/NG"), LVCFMT_CENTER, 60);// 리스트 행 추가
	//m_CtrlListTestProcess.InsertColumn(9, _T("T/T"), LVCFMT_CENTER, 50);// 리스트 행 추가

	m_CtrlListTestProcess.InsertItem(0, _T("1"));   // 첫째행(0), 첫째열에 삽입
	m_CtrlListTestProcess.SetItem(0, 1, LVIF_TEXT, _T("FM TEST"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	m_CtrlListTestProcess.SetItem(0, 2, LVIF_TEXT, _T("97.8Hz"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 3, LVIF_TEXT, _T("2.5"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 4, LVIF_TEXT, _T("0.0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 5, LVIF_TEXT, _T("0"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 6, LVIF_TEXT, _T("80%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 7, LVIF_TEXT, _T("90%"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 
	//m_CtrlListTestProcess.SetItem(0, 8, LVIF_TEXT, _T("OK"), NULL, NULL, NULL, NULL);   // 첫째행(0), 둘째열(1)에 삽입 

	
	
	font = new CFont;
		//font->CreateFontA( 16,                          // nHeight
		//			   0,                          // nWidth
		//			   0,                          // nEscapement
		//			   0,                          // nOrientation
		//			   400,                           // nWeight
		//			   0,                          // bItalic
		//			   0,                          // bUnderline 
		//			   0,                          // cStrikeOut 
		//			   0,                          // nCharSet
		//			   0,           // nOutPrecision 
		//			   0,                          // nClipPrecision 
		//			 PROOF_QUALITY,              // nQuality
		//			 0,  // nPitchAndFamily 
		//			 _T("Arial"));                     // lpszFacename
	font->CreateFontA( 20,                          // nHeight
					   0,                          // nWidth
					   0,                          // nEscapement
					   0,                          // nOrientation
					   400,                           // nWeight
					   0,                          // bItalic
					   0,                          // bUnderline 
					   0,                          // cStrikeOut 
					   0,                          // nCharSet
					   0,           // nOutPrecision 
					   0,                          // nClipPrecision 
					 PROOF_QUALITY,              // nQuality
					 0,  // nPitchAndFamily 
					 _T("Arial"));                     // lpszFacename
	//font->CreatePointFont(20,_T("Arial"));   
	m_CtrlListTestProcess.SetFont(font);

	delete font;
	
}


void DlgModelSetupSeq::SetSubListControl()//void DlgModelSetupSeq::SetListControl()
{
	
	CSize lSize;
	CString strTemp;

	m_CtrlListTestProcess.DeleteAllItems();
	
	for(int  i = 0; i < gTestSeqData.TestCnt; i++)
	{
		strTemp.Format(_T("%d"), i+1);
		m_CtrlListTestProcess.InsertItem(i, strTemp);   


		strTemp = gTestSeqData.TestStepList[i].strStepName;
		m_CtrlListTestProcess.SetItem(i, 1, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);
		strTemp = gTestSeqData.TestStepList[i].strStepWork;
		m_CtrlListTestProcess.SetItem(i, 2, LVIF_TEXT, strTemp, NULL, NULL, NULL, NULL);

		gTestSeqData.TestStepList[i].bEnable = 1;
		if(gTestSeqData.TestStepList[i].bEnable == 1)
		{
			m_CtrlListTestProcess.SetCheck(i,1);
		}
		else
		{
			m_CtrlListTestProcess.SetCheck(i,0);
		}
			
	}

	
	//gTestSeqData
	//CString lStr;
	//TestListTable_TYPE lTempList;

	//m_CtrlListTestProcess.DeleteAllItems();
	//
	//for(int  i = 0; i < gTestModel.m_ProcessCnt; i++)
	//{
	//	lTempList = gTestModel.m_TestList[i];
	//	lStr.Format(_T("%d"), i+1);
	//	m_CtrlListTestProcess.InsertItem(i, lStr);   // 첫째행(0), 첫째열에 삽입

	//	lTempList.m_Test_Enable = 1;
	//	if(lTempList.m_Test_Enable == 1)
	//	{
	//		m_CtrlListTestProcess.SetCheck(i,1);
	//	}
	//	else
	//	{
	//		m_CtrlListTestProcess.SetCheck(i,0);
	//	}
	//		

	//	m_CtrlListTestProcess.SetItem(i, 1, LVIF_TEXT, lTempList.s_TestName, NULL, NULL, NULL, NULL);  
	//	if(1)//lTempList.m_IPType == IP_TYPE_LAN)		
	//	{
	//		m_CtrlListTestProcess.SetItem(i, 2, LVIF_TEXT, _T("LAN"), NULL, NULL, NULL, NULL);  
	//	}
	//	else
	//	{
	//		m_CtrlListTestProcess.SetItem(i, 2, LVIF_TEXT, _T("Wifi"), NULL, NULL, NULL, NULL);  
	//	}
	//	lStr.Format(_T("%.1f"), lTempList.m_WaitTime/1000.0);
	//	m_CtrlListTestProcess.SetItem(i, 3, LVIF_TEXT, lStr, NULL, NULL, NULL, NULL);   // 

	//	lStr.Format(_T("%.1f"), lTempList.m_TestTime/1000.0);
	//	m_CtrlListTestProcess.SetItem(i, 4, LVIF_TEXT, lStr, NULL, NULL, NULL, NULL);   // 
	//	
	//	lStr.Format(_T("%.1f"), lTempList.m_RefVal_Min);
	//	m_CtrlListTestProcess.SetItem(i, 5, LVIF_TEXT, lStr, NULL, NULL, NULL, NULL);   // 

	//	lStr.Format(_T("%.1f"), lTempList.m_RefVal_Max);		
	//	m_CtrlListTestProcess.SetItem(i, 6, LVIF_TEXT, lStr, NULL, NULL, NULL, NULL); 


	//	//if((lTempList.m_RefVal_Min == 0) && (lTempList.m_RefVal_Min == 0))
	//	//{
	//	//	m_CtrlListTestProcess.SetItem(i, 6, LVIF_TEXT, _T("CHECK"), NULL, NULL, NULL, NULL);   
	//	//}
	//	//else
	//	//{
	//	//	if(0)//lTempList.m_ValueType == REF_TYPE_PERCENT)
	//	//	{
	//	//		lStr.Format(_T("%.1f%%"), lTempList.m_RefVal_Max);		
	//	//		m_CtrlListTestProcess.SetItem(i, 6, LVIF_TEXT, lStr, NULL, NULL, NULL, NULL); 
	//	//	}
	//	//	else
	//	//	{
	//	//		lStr.Format(_T("%.1f"), lTempList.m_RefVal_Max);		
	//	//		m_CtrlListTestProcess.SetItem(i, 6, LVIF_TEXT, lStr, NULL, NULL, NULL, NULL); 
	//	//	}
	//	//}
	//}

	//mCheckAll = 1;
	m_CtrlListTestProcess.Invalidate();
}




BEGIN_EVENTSINK_MAP(DlgModelSetupSeq, CDialogEx)
	ON_EVENT(DlgModelSetupSeq, IDC_BUTTON_SEQ_LOAD, DISPID_CLICK, DlgModelSetupSeq::ClickButtonSeqLoad, VTS_NONE)
	ON_EVENT(DlgModelSetupSeq, IDC_BUTTON_SEQ_EDIT, DISPID_CLICK, DlgModelSetupSeq::ClickButtonSeqEdit, VTS_NONE)
	ON_EVENT(DlgModelSetupSeq, IDC_BUTTON_MODEL_CREATE, DISPID_CLICK, DlgModelSetupSeq::ClickButtonModelCreate, VTS_NONE)
	ON_EVENT(DlgModelSetupSeq, IDC_BUTTON_MODEL_SAVE, DISPID_CLICK, DlgModelSetupSeq::ClickButtonModelSave, VTS_NONE)
	ON_EVENT(DlgModelSetupSeq, IDC_BUTTON_MODEL_REMOVE, DISPID_CLICK, DlgModelSetupSeq::ClickButtonModelRemove, VTS_NONE)
	ON_EVENT(DlgModelSetupSeq, IDC_BUTTON_MODEL_CONFIG, DISPID_CLICK, DlgModelSetupSeq::ClickButtonModelConfig, VTS_NONE)
	ON_EVENT(DlgModelSetupSeq, IDC_BUTTON_IMAGE_SET, DISPID_CLICK, DlgModelSetupSeq::ClickButtonImageSet, VTS_NONE)
//	ON_EVENT(DlgModelSetupSeq, IDC_LABEL_CHECKSUM, DISPID_DBLCLICK, DlgModelSetupSeq::DblClickLabelChecksum, VTS_DISPATCH)
END_EVENTSINK_MAP()


void DlgModelSetupSeq::ClickButtonSeqLoad()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//CString lstrModelname  = _T("TEST");
	//CFileControl lFile;
	//lFile.OpenModelSeq(lstrModelname);

	LoadTestData();
	DisplayTestData();
}


void DlgModelSetupSeq::ClickButtonSeqEdit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	char buf[1024];
	CString lstrModelname  = _T("TEST");
//	m_cSeqProcessCombo.GetLBText(m_cSeqProcessCombo.GetCurSel(), lstrModelname) ;
	
	int lpos = gTestSeqData.strSeqFileName.Find(".seq");
	if(gTestSeqData.strModelName.Compare(gTestSeqData.strSeqFileName.Left(lpos)) != 0)
	{
		if(lpos > 1)
		{
			CString Filename = PATH_BASE_DIR;
			Filename += _T("\\Model\\");
			Filename += gTestSeqData.strSeqFileName;

			CString FilenameNew = PATH_BASE_DIR;
			FilenameNew += _T("\\Model\\");
			FilenameNew += gTestSeqData.strModelName;
			FilenameNew += _T(".seq");

			if(!CopyFile(Filename, FilenameNew, 0))
			{
				//MessageBox("Copy File Fail!!");
			}
		}
		else
		{	
		
		}
		//CopyFile(
		//CFile LCtrfile(
		//CFile::Rename(pOldName, pNewName);
		//	CFile::Duplicate();	
		lstrModelname = gTestSeqData.strModelName;
		lstrModelname += ".seq";
		gTestSeqData.strSeqFileName  = lstrModelname;
		CFileControl lFile;
		if(lFile.SaveModel(gTestSeqData.strModelName))
		{
			MessageBox("File Save Error !!");
		}
	}




	lstrModelname = gTestSeqData.strSeqFileName;


	CString Filename = PATH_BASE_DIR;
	Filename += _T("\\Model\\");
	Filename += lstrModelname;
//	Filename +=  _T(".seq");

	sprintf(buf, "C:\\WINDOWS\\system32\\notepad.exe %s" ,Filename.GetBuffer());
//	sprintf(buf, "%ws" ,m_openFile.GetBuffer());
	WinExec(buf,SW_SHOW);
//	ShellExecute( "open", "http://www.tipssoft.com", "", "", SW_SHOW);

}

void DlgModelSetupSeq::LoadTestData()
{
	//m_cSeqProcessCombo.getm_cSeqProcessCombo.GetCurSel();
	//CString lstrModelname  = _T("TEST");


	CString lstrModelSeqname;
	//m_cModelCombo.GetWindowTextA(lstrModelname, 64);
	//m_cSeqProcessCombo.GetLBText(m_cSeqProcessCombo.GetCurSel(), lstrModelSeqname) ;
	lstrModelSeqname = gTestSeqData.strSeqFileName;


	int lpos = gTestSeqData.strSeqFileName.Find(".seq");
	if(gTestSeqData.strModelName.Compare(gTestSeqData.strSeqFileName.Left(lpos)) != 0)
	{
		if(lpos > 1)
		{
			CString Filename = PATH_BASE_DIR;
			Filename += _T("\\Model\\");
			Filename += gTestSeqData.strSeqFileName;

			CString FilenameNew = PATH_BASE_DIR;
			FilenameNew += _T("\\Model\\");
			FilenameNew += gTestSeqData.strModelName;
			FilenameNew += _T(".seq");

			if(!CopyFile(Filename, FilenameNew, 0))
			{
				//MessageBox("Copy File Fail!!");
			}
		//	CString Filename;
			Filename = gTestSeqData.strModelName;
			Filename += ".seq";
			gTestSeqData.strSeqFileName  = Filename; 
			lstrModelSeqname = gTestSeqData.strSeqFileName;
		}
	
	}

	CFileControl lFile;
	lFile.OpenModelSeq(lstrModelSeqname);

	
}


void DlgModelSetupSeq::DisplayTestData()
{
	SetSubListControl();
	
	m_EditFPGAVer.Format(_T("%d.%d.%d"),
		g_FPGA_Ver_event,
		g_FPGA_Ver_major,
		g_FPGA_Ver_minor);

	m_EditDXD_ServerVer.Format(_T("%d.%d%d.%02d"),
		g_DSERVER_Ver_event,
		g_DSERVER_Ver_major,
		g_DSERVER_Ver_minor,
		g_DSERVER_Ver_release);

	m_EditMicomVer.Format(_T("%d.%d.%d"),
		g_Microchip_Ver_event,
		g_Microchip_Ver_major,
		g_Microchip_Ver_minor);

	m_EditAVR_Ver.Format(_T("%d.%d.%d"),
		g_AVR_Ver_event,
		g_AVR_Ver_major,
		g_AVR_Ver_minor);

	m_cComboMicomHKHQ.SetCurSel(g_AVR_Type_HK_HQ);// m_Radio_HK_HQ = g_AVR_Type_HK_HQ;

	if (g_AVR_Type_HK_HQ == MICOM_TYPE_HQ)
	{
		SetDlgItemText(IDC_STATIC_AED_DROP, "Drop Micom ver.");//
		m_cComboSetModelName.ResetContent();
		//char *modelName[DXD_MODEL_COUNT] = { "10HQ701G", "14HQ701G", "14HQ901G", "17HQ701G", "17HQ901G", "14HQ721G", "17HQ901G_D" };
		int lSeL = 0;
		for (int i = 0; i < DXD_MODEL_COUNT; i++)
		{
			m_cComboSetModelName.AddString(gHQ_ModelNmae[i].GetString());
			if (gHQ_ModelNmae[i].Compare(g_SetModelName) == 0)
			{
				lSeL = i;
			}
			m_cComboSetModelName.SetCurSel(lSeL);
		}
		//m_cComboSetModelName.AddString("10HQ701G");
		//m_cComboSetModelName.AddString("14HQ701G");
		//m_cComboSetModelName.AddString("14HQ901G");
		//m_cComboSetModelName.AddString("17HQ701G");
		//m_cComboSetModelName.AddString("17HQ901G");
		//m_cComboSetModelName.AddString("14HQ721G");
		//m_cComboSetModelName.AddString("17HQ901G_D");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_AED_DROP, "AED Micom ver.");//
		
		m_cComboSetModelName.ResetContent();
		//char *gHK_ModelName[DXD_HK_MODEL_NUM] = { "17HK700G", "14HK701G", "17HK701G"};
		int lSeL = 0;
		for (int i = 0; i < DXD_HK_MODEL_NUM; i++)
		{
			m_cComboSetModelName.AddString(gHK_ModelName[i].GetString());
			if (gHK_ModelName[i].Compare(g_SetModelName) == 0)
			{
				lSeL = i;
			}
			m_cComboSetModelName.SetCurSel(lSeL);
		}
		//m_cComboSetModelName.AddString("17HK700G");
		//m_cComboSetModelName.AddString("14HK701G");
		//m_cComboSetModelName.AddString("17HK701G");		
	}

	for (int i = 0; i < 16; i++)
	{
		m_EditRoicMinValue[i].Format("%d", g_RefMin_buf[i]);
		m_EditRoicMaxValue[i].Format("%d", g_RefMax_buf[i]);
	}


	UpdateData(FALSE);
	//SetDlgItemText(IDC_EDIT_IPADDRESS_GMES1, g_nRemoteIPAddress);	
	//SetDlgItemText( IDC_EDIT_CAMERA_IPADDRESS, gTestSeqData.strCameraIPAddress);
	//SetDlgItemText( IDC_IPADDRESS_CAMERA, gTestSeqData.strCameraIPAddress);
}


//void CDlgModelSetup::ClickButtonModelSave()
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//
//	SaveModel(gTestSeqData.strModelName);//gTestSeqData
//}

bool DlgModelSetupSeq::NameListCheckAndADD(CString lCheckName)
{
	CString str;
	for(int i = 0; i <  m_cModelComboSeq.GetCount(); i++)
	{
		m_cModelComboSeq.GetLBText(i, str);
		if(str.Compare(lCheckName) == 0)
		{
			return 1;
		}
	}
	m_cModelComboSeq.AddString(lCheckName);
	
	return 0;
}


void DlgModelSetupSeq::ClickButtonModelCreate()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
		// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	

	OnBnClickedButtonSaveas();
}


void DlgModelSetupSeq::ClickButtonModelSave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveModel(gTestSeqData.strModelName);
}


void DlgModelSetupSeq::ClickButtonModelRemove()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
		// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int lOk = MessageBox(_T("Remove Current Model and Erase All of Current Model Data!! \r\n\r\nContinue Remove??"), _T("REMOVE MODEL"), MB_YESNO | MB_DEFBUTTON2);
	if(lOk == IDYES)
	{
		CString lstrModelName;
		CFileControl lFile;
		m_cModelComboSeq.GetLBText(m_cModelComboSeq.GetCurSel(),lstrModelName);
		lFile.RemoveModel(lstrModelName);
		m_cModelComboSeq.DeleteString(m_cModelComboSeq.GetCurSel());
		
	
		m_cModelComboSeq.SetCurSel(0);
		LoadTestData();
		DisplayTestData();
	}
	else
	{
		MessageBox(_T("Cancelled Remove Current Model !!"));
	}
}



void DlgModelSetupSeq::SaveModel(CString lFilename)//gTestSeqData
{
	
	int lError = UpdateTestData();
	if(lError)
		return;

	CFileControl lFile;

	if(lFile.SaveModel(lFilename))
	{
		MessageBox("File Save Error!!");
	}
//	CFileControl lFile;
	lFile.SaveINIModel();
	m_TestOrgSeqData = gTestSeqData;
}

int  DlgModelSetupSeq::UpdateTestData()//gTestSeqData
{
	CString strTemp;
	CString strTemp1;
	CString strTemp2;
	CString strTemp3;
	CString strTemp4;
	int lPos = 0;
	int lNumber = 0;
	UpdateData();

	CString lstrModelName;	
	m_cModelComboSeq.GetLBText(m_cModelComboSeq.GetCurSel(),lstrModelName);
	gTestSeqData.strModelName = lstrModelName;


	lPos = m_EditFPGAVer.Find(".");
	strTemp1 = m_EditFPGAVer.Left(lPos);
	strTemp = m_EditFPGAVer.Mid(lPos + 1);
	strTemp2 = strTemp.Left(lPos);
	lPos = strTemp.Find(".");
	strTemp = strTemp.Mid(lPos + 1);
	strTemp3 = strTemp;


	g_FPGA_Ver_event = _ttoi(strTemp1);
	g_FPGA_Ver_major = _ttoi(strTemp2);
	g_FPGA_Ver_minor = _ttoi(strTemp3);




	lPos = m_EditDXD_ServerVer.Find(".");
	strTemp = m_EditDXD_ServerVer.Mid(lPos + 1);
	strTemp1 = m_EditDXD_ServerVer.Left(lPos);

	lPos = strTemp.Find(".");
	strTemp2 = strTemp.Left(lPos);
	strTemp3 = strTemp2.Mid(1);
	strTemp2 = strTemp2.Left(1);

	strTemp = strTemp.Mid(lPos + 1);
	strTemp4 = strTemp;


	g_DSERVER_Ver_event = _ttoi(strTemp1);
	g_DSERVER_Ver_major = _ttoi(strTemp2);
	g_DSERVER_Ver_minor = _ttoi(strTemp3);
	g_DSERVER_Ver_release = _ttoi(strTemp4);


	lPos = m_EditMicomVer.Find(".");
	strTemp = m_EditMicomVer.Mid(lPos + 1);
	strTemp1 = m_EditMicomVer.Left(lPos);
	strTemp2 = strTemp.Left(lPos);
	lPos = strTemp.Find(".");
	strTemp = strTemp.Mid(lPos + 1);
	strTemp3 = strTemp;


	g_Microchip_Ver_event = _ttoi(strTemp1);
	g_Microchip_Ver_major = _ttoi(strTemp2);
	g_Microchip_Ver_minor = _ttoi(strTemp3);

	/*
	lPos = m_EditAVR_Ver.Find(".");
	strTemp = m_EditAVR_Ver.Mid(lPos + 1);
	strTemp1 = m_EditAVR_Ver.Left(lPos);
	strTemp = strTemp.Mid(lPos + 1);
	strTemp2 = strTemp.Left(lPos);
	strTemp3 = strTemp;*/

	lPos = m_EditAVR_Ver.Find(".");
	strTemp = m_EditAVR_Ver.Mid(lPos + 1);
	strTemp1 = m_EditAVR_Ver.Left(lPos);
	strTemp2 = strTemp.Left(lPos);
	lPos = strTemp.Find(".");
	strTemp = strTemp.Mid(lPos + 1);
	strTemp3 = strTemp;


	g_AVR_Type_HK_HQ = m_cComboMicomHKHQ.GetCurSel(); //g_AVR_Type_HK_HQ = m_Radio_HK_HQ;


	m_cComboSetModelName.GetLBText(m_cComboSetModelName.GetCurSel(), strTemp);
	g_SetModelName = strTemp;


	g_AVR_Ver_event = _ttoi(strTemp1);
	g_AVR_Ver_major = _ttoi(strTemp2);
	g_AVR_Ver_minor = _ttoi(strTemp3);


	for (int i = 0; i < 16; i++)
	{
		g_RefMin_buf[i] = _ttoi(m_EditRoicMinValue[i]);
		g_RefMax_buf[i] = _ttoi(m_EditRoicMaxValue[i]);

	}


	//m_cSeqProcessCombo.GetLBText(m_cSeqProcessCombo.GetCurSel(),lstrModelName);
	int lpos = gTestSeqData.strSeqFileName.Find(".seq");
	if(gTestSeqData.strModelName.Compare(gTestSeqData.strSeqFileName.Left(lpos)) != 0)
	{
		if(lpos > 1)
		{
			CString Filename = PATH_BASE_DIR;
			Filename += _T("\\Model\\");
			Filename += gTestSeqData.strSeqFileName;

			CString FilenameNew = PATH_BASE_DIR;
			FilenameNew += _T("\\Model\\");
			FilenameNew += gTestSeqData.strModelName;
			FilenameNew += _T(".seq");

			if(!CopyFile(Filename, FilenameNew, 0))
			{
			//	MessageBox("Copy File Fail!!");
			}
		}
		else
		{	
		
		}
		//CopyFile(
		//CFile LCtrfile(
		//CFile::Rename(pOldName, pNewName);
		lstrModelName = gTestSeqData.strModelName;
		lstrModelName += ".seq";
		gTestSeqData.strSeqFileName  = lstrModelName;		
	}

	CFileControl lFile;
	//if(lFile.SaveModel(gTestSeqData.strModelName))
	//{
	//	MessageBox("File Save Error !!");
	//}
	CString str1, str2;





	return 0;
}


void DlgModelSetupSeq::ClickButtonModelConfig()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	
	CFileControl lFile;

	CDlgPassword dlgpw;

	dlgpw.m_sPassword = g_strPassword;



	if(dlgpw.DoModal()==IDOK)
	{
		//m_sPassword.Compare(
		//if(strncmp(m_sPassword.GetBuffer(),dlgpw.m_EditCurrentPW.GetBuffer(), m_sPassword.GetLength()) == 0)
		if(g_strPassword == dlgpw.m_sPassword)
		{
			CDlgConfig dlg;
						
	

			if(dlg.DoModal()==IDOK)
			{

	

				lFile.SaveConfigSeqData();

		
			}
		}
		else
		{
			g_strPassword = dlgpw.m_sPassword;
			lFile.SaveConfigSeqData();
		}
	}
}



void DlgModelSetupSeq::ClickButtonImageSet()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//if(g_CameraFound == 1)
	//{
	//	CDlgImage ldlg;
	//	ldlg.DoModal();
	//}
	//else
	//{
	//	//MessageBox("Camrea Not Connected!");
	//}
}



void DlgModelSetupSeq::OnCbnSelchangeComboRecomon2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	int lid = m_cModelComboSeq.GetCurSel();

	
	CString lstrModelname;	
	m_cModelComboSeq.GetLBText(m_cModelComboSeq.GetCurSel(), lstrModelname) ;
	gTestSeqData.strModelName = lstrModelname;


	CFileControl lFile;
//	lFile.SaveINIModel();
	m_TextModelName.put_Caption(gTestSeqData.strModelName);

	lFile.OpenModel(lstrModelname);

//	gTestSeqData.bMesEnable = 0;


//	int l_nModelSelNum = 0;
//		
//	for(int i = 0 ; i < m_cSeqProcessCombo.GetCount(); i++)
//	{
////			m_strModelSeqList[i] = fileNameList[i].Left(fileNameList[i].GetLength() - 4);
////		m_strModelSeqList[i] = fileNameList[i];
//
////		m_cSeqProcessCombo.AddString(m_strModelSeqList[i]);
//		m_cSeqProcessCombo.GetLBText(i, lstrModelname) ;
//		if(lstrModelname.Compare(gTestSeqData.strSeqFileName) == 0)
//		{
//			l_nModelSelNum = i;
//		}
//	}
//	if(l_nModelSelNum < m_cSeqProcessCombo.GetCount())
//		m_cSeqProcessCombo.SetCurSel(l_nModelSelNum);

	//GetSeqList();
	//GetRemoconList();


//	m_TextModelName.put_Caption(gTestSeqData.strModelName);
// m_TextModelName.put_Caption(gTestSeqData.strModelName);

	//SetListControl();

//	LoadTestData();

//	DisplayTestData();
	LoadTestData();

	DisplayTestData();
}


/** \file FileOperations.h                   
   Project: FopDemo\n
   Project type: MFC App\n
   Author: Vinnichenko Alexey\n
   E-mail: subj@mail.ru\n
   Description: Declaration of CFileOperation class and CFileExeption class.
*/ 

//#include "resource.h"
//
//#define PATH_ERROR   -1
//#define PATH_NOT_FOUND  0
//#define PATH_IS_FILE  1
//#define PATH_IS_FOLDER  2
//
//
//class CFExeption
//{
//public:
// CFExeption(DWORD dwErrCode);
// CFExeption(CString sErrText);
// CString GetErrorText() {return m_sError;}
// DWORD GetErrorCode() {return m_dwError;}
//
//private:
// CString m_sError;
// DWORD m_dwError;
//};
//
//
////*****************************************************************************************************
//
//class CFileOperation
//{
//public:
// CFileOperation(); // constructor
// bool Delete(CString sPathName); // delete file or folder
// bool Copy(CString sSource, CString sDest); // copy file or folder
// bool Replace(CString sSource, CString sDest); // move file or folder
// bool Rename(CString sSource, CString sDest); // rename file or folder
// CString GetErrorString() {return m_sError;} // return error description
// DWORD GetErrorCode() {return m_dwError;} // return error code
// void ShowError() // show error message
//  {MessageBox(NULL, m_sError, _T("Error"), MB_OK | MB_ICONERROR);}
// void SetAskIfReadOnly(bool bAsk = true) // sets behavior with readonly files(folders)
//  {m_bAskIfReadOnly = bAsk;}
// bool IsAskIfReadOnly() // return current behavior with readonly files(folders)
//  {return m_bAskIfReadOnly;}
// bool CanDelete(CString sPathName); // check attributes
// void SetOverwriteMode(bool bOverwrite = false) // sets overwrite mode on/off
//  {m_bOverwriteMode = bOverwrite;}
// bool IsOverwriteMode() {return m_bOverwriteMode;} // return current overwrite mode
// int CheckPath(CString sPath);
// bool IsAborted() {return m_bAborted;}
//
//protected:
// void DoDelete(CString sPathName);
// void DoCopy(CString sSource, CString sDest, bool bDelteAfterCopy = false);
// void DoFileCopy(CString sSourceFile, CString sDestFile, bool bDelteAfterCopy = false);
// void DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDelteAfterCopy = false);
// void DoRename(CString sSource, CString sDest);
// bool IsFileExist(CString sPathName);
// void PreparePath(CString &sPath);
// void Initialize();
// void CheckSelfRecursion(CString sSource, CString sDest);
// bool CheckSelfCopy(CString sSource, CString sDest);
// CString ChangeFileName(CString sFileName);
// CString ParseFolderName(CString sPathName);
//
//private:
// CString m_sError;
// DWORD m_dwError;
// bool m_bAskIfReadOnly;
// bool m_bOverwriteMode;
// bool m_bAborted;
// int m_iRecursionLimit;
//};
//
//
////*****************************************************************************************************
//
// 
//
// 
//
// 
//
// 
//
// 
//
// 
//
// 
//
// 
//
// 
//
///** \file FileOperations.cpp
//   Project: FopDemo\n
//   Project type: MFC App\n
//   Author: Vinnichenko Alexey\n
//   E-mail: subj@mail.ru\n
//   Description: Implementation of CFileOperation class and CFileExeption class.
//*/ 
//
//#include "stdafx.h" 
//#include "resource.h" 
//#include "FileOperations.h" 
//#include "TypeCastFuncs.h"
//
////************************************************************************************************************
//CFExeption::CFExeption(DWORD dwErrCode)
//{
// LPVOID lpMsgBuf;
// FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//         NULL, dwErrCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
// m_sError = (LPTSTR)lpMsgBuf;
// LocalFree(lpMsgBuf);
// m_dwError = dwErrCode;
//}
//CFExeption::CFExeption(CString sErrText)
//{
// m_sError = sErrText;
// m_dwError = 0;
//}
//
//
////************************************************************************************************************
//
//CFileOperation::CFileOperation()
//{
// Initialize();
//}
//
//
//void CFileOperation::Initialize()
//{
// m_sError = _T("No error");
// m_dwError = 0;
// m_bAskIfReadOnly = true;
// m_bOverwriteMode = false;
// m_bAborted = false;
// m_iRecursionLimit = -1;
//}
//
//
//void CFileOperation::DoDelete(CString sPathName)
//{
// CFileFind ff;
// CString sPath = sPathName;
//
// if (CheckPath(sPath) == PATH_IS_FILE)
// {
//  if (!CanDelete(sPath)) 
//  {
//   m_bAborted = true;
//   return;
//  }
//  if (!DeleteFile(sPath)) throw new CFExeption(GetLastError());
//  return;
// }
//
// PreparePath(sPath);
// sPath += "*.*";
//
// BOOL bRes = ff.FindFile(sPath);
// while(bRes)
// {
//  bRes = ff.FindNextFile();
//  if (ff.IsDots()) continue;
//  if (ff.IsDirectory())
//  {
//   sPath = ff.GetFilePath();
//   DoDelete(sPath);
//  }
//  else DoDelete(ff.GetFilePath());
// }
// ff.Close();
// if (!RemoveDirectory(sPathName) && !m_bAborted) throw new CFExeption(GetLastError());
//}
//
//
//void CFileOperation::DoFolderCopy(CString sSourceFolder, CString sDestFolder, bool bDelteAfterCopy)
//{
// CFileFind ff;
// CString sPathSource = sSourceFolder;
// BOOL bRes = ff.FindFile(sPathSource);
// while (bRes)
// {
//  bRes = ff.FindNextFile();
//  if (ff.IsDots()) continue;
//  if (ff.IsDirectory()) // source is a folder
//  {
//   if (m_iRecursionLimit == 0) continue;
//   sPathSource = ff.GetFilePath() + CString("\\") + CString("*.*");
//   CString sPathDest = sDestFolder + ff.GetFileName() + CString("\\");
//   if (CheckPath(sPathDest) == PATH_NOT_FOUND) 
//   {
//    if (!CreateDirectory(sPathDest, NULL))
//    {
//     ff.Close();
//     throw new CFExeption(GetLastError());
//    }
//   }
//   if (m_iRecursionLimit > 0) m_iRecursionLimit --;
//   DoFolderCopy(sPathSource, sPathDest, bDelteAfterCopy);
//  }
//  else // source is a file
//  {
//   CString sNewFileName = sDestFolder + ff.GetFileName();
//   DoFileCopy(ff.GetFilePath(), sNewFileName, bDelteAfterCopy);
//  }
// }
// ff.Close();
//}
//
//
//bool CFileOperation::Delete(CString sPathName)
//{
// try
// {
//  DoDelete(sPathName);
// }
// catch(CFExeption* e)
// {
//  m_sError = e->GetErrorText();
//  m_dwError = e->GetErrorCode();
//  delete e;
//  if (m_dwError == 0) return true;
//  return false;
// }
// return true;
//}
//
//
//bool CFileOperation::Rename(CString sSource, CString sDest)
//{
// try
// {
//  DoRename(sSource, sDest);
// }
// catch(CFExeption* e)
// {
//  m_sError = e->GetErrorText();
//  m_dwError = e->GetErrorCode();
//  delete e;
//  return false;
// }
// return true;
//}
//
//
//void CFileOperation::DoRename(CString sSource, CString sDest)
//{
// if (!MoveFile(sSource, sDest)) throw new CFExeption(GetLastError());
//}
//
//
//void CFileOperation::DoCopy(CString sSource, CString sDest, bool bDelteAfterCopy)
//{
// CheckSelfRecursion(sSource, sDest);
// // source not found
// if (CheckPath(sSource) == PATH_NOT_FOUND)
// {
//  CString sError = sSource + CString(L" not found");
//  throw new CFExeption(sError);
// }
// // dest not found
// if (CheckPath(sDest) == PATH_NOT_FOUND)
// {
//  CString sError = sDest + CString(L" not found");
//  throw new CFExeption(sError);
// }
// // folder to file
// if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FILE) 
// {
//  throw new CFExeption(L"Wrong operation");
// }
// // folder to folder
// if (CheckPath(sSource) == PATH_IS_FOLDER && CheckPath(sDest) == PATH_IS_FOLDER) 
// {
//  CFileFind ff;
//  CString sError = sSource + CString(L" not found");
//  PreparePath(sSource);
//  PreparePath(sDest);
//  sSource += "*.*";
//  if (!ff.FindFile(sSource)) 
//  {
//   ff.Close();
//   throw new CFExeption(sError);
//  }
//  if (!ff.FindNextFile()) 
//  {
//   ff.Close();
//   throw new CFExeption(sError);
//  }
//  CString sFolderName = ParseFolderName(sSource);
//  if (!sFolderName.IsEmpty()) // the source is not drive
//  {
//   sDest += sFolderName;
//   PreparePath(sDest);
//   if (!CreateDirectory(sDest, NULL))
//   {
//    DWORD dwErr = GetLastError();
//    if (dwErr != 183)
//    {
//     ff.Close();
//     throw new CFExeption(dwErr);
//    }
//   }
//  }
//  ff.Close();
//  DoFolderCopy(sSource, sDest, bDelteAfterCopy);
// }
// // file to file
// if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FILE) 
// {
//  DoFileCopy(sSource, sDest);
// }
//
// CTypeCastUtil typecast;
// // file to folder
// if (CheckPath(sSource) == PATH_IS_FILE && CheckPath(sDest) == PATH_IS_FOLDER) 
// {
//  PreparePath(sDest);
//  char drive[MAX_PATH], dir[MAX_PATH], name[MAX_PATH], ext[MAX_PATH];
//  _splitpath_s(typecast.CStringToChar(sSource), drive, dir, name, ext);
//  sDest = sDest + CString(name) + CString(ext);
//  DoFileCopy(sSource, sDest);
// }
//}
//
//
//void CFileOperation::DoFileCopy(CString sSourceFile, CString sDestFile, bool bDelteAfterCopy)
//{
// BOOL bOvrwriteFails = FALSE;
// if (!m_bOverwriteMode)
// {
//  while (IsFileExist(sDestFile)) 
//  {
//   sDestFile = ChangeFileName(sDestFile);
//  }
//  bOvrwriteFails = TRUE;
// }
// if (!CopyFile(sSourceFile, sDestFile, bOvrwriteFails)) throw new CFExeption(GetLastError());
// if (bDelteAfterCopy)
// {
//  DoDelete(sSourceFile);
// }
//}
//
//
//bool CFileOperation::Copy(CString sSource, CString sDest)
//{
// if (CheckSelfCopy(sSource, sDest)) return true;
// bool bRes;
// try
// {
//  DoCopy(sSource, sDest);
//  bRes = true;
// }
// catch(CFExeption* e)
// {
//  m_sError = e->GetErrorText();
//  m_dwError = e->GetErrorCode();
//  delete e;
//  if (m_dwError == 0) bRes = true;
//  bRes = false;
// }
// m_iRecursionLimit = -1;
// return bRes;
//}
//
//
//bool CFileOperation::Replace(CString sSource, CString sDest)
//{
// if (CheckSelfCopy(sSource, sDest)) return true;
// bool bRes;
// try
// {
//  bool b = m_bAskIfReadOnly;
//  m_bAskIfReadOnly = false;
//  DoCopy(sSource, sDest, true);
//  DoDelete(sSource);
//  m_bAskIfReadOnly = b;
//  bRes = true;
// }
// catch(CFExeption* e)
// {
//  m_sError = e->GetErrorText();
//  m_dwError = e->GetErrorCode();
//  delete e;
//  if (m_dwError == 0) bRes = true;
//  bRes = false;
// }
// m_iRecursionLimit = -1;
// return bRes;
//}
//
//
//CString CFileOperation::ChangeFileName(CString sFileName)
//{
// CString sName, sNewName, sResult;
// char drive[MAX_PATH];
// char dir  [MAX_PATH];
// char name [MAX_PATH];
// char ext  [MAX_PATH];
// 
// CTypeCastUtil typecast;
//
// _splitpath_s(typecast.CStringToChar(sFileName), drive, dir, name, ext);
// sName = name;
//
// int pos = sName.Find(L"Copy ");
// if (pos == -1)
// {
//  sNewName = CString(L"Copy of ") + sName + CString(ext);
// }
// else
// {
//  int pos1 = sName.Find('(');
//  if (pos1 == -1)
//  {
//   sNewName = sName;
//   sNewName.Delete(0, 8);
//   sNewName = CString("Copy (1) of ") + sNewName + CString(ext);
//  }
//  else
//  {
//   CString sCount;
//   int pos2 = sName.Find(')');
//   if (pos2 == -1)
//   {
//    sNewName = CString("Copy of ") + sNewName + CString(ext);
//   }
//   else
//   {
//    sCount = sName.Mid(pos1 + 1, pos2 - pos1 - 1);
//    sName.Delete(0, pos2 + 5);
//    int iCount = atoi(typecast.CStringToChar(sCount));
//    iCount ++;
//    sNewName.Format(L"%s%d%s%s%s", "Copy (", iCount, ") of ", (LPCTSTR)sName, ext);
//   }
//  }
// }
//
// sResult = CString(drive) + CString(dir) + sNewName;
//
// return sResult;
//}
//
//
//bool CFileOperation::IsFileExist(CString sPathName)
//{
// HANDLE hFile;
// hFile = CreateFile(sPathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
// if (hFile == INVALID_HANDLE_VALUE) return false;
// CloseHandle(hFile);
// return true;
//}
//
//
//int CFileOperation::CheckPath(CString sPath)
//{
// DWORD dwAttr = GetFileAttributes(sPath);
// if (dwAttr == 0xffffffff) 
// {
//  if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND) 
//   return PATH_NOT_FOUND;
//  return PATH_ERROR;
// }
// if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) return PATH_IS_FOLDER;
// return PATH_IS_FILE;
//}
//
//
//void CFileOperation::PreparePath(CString &sPath)
//{
// if(sPath.Right(1) != "\\") sPath += "\\";
//}
//
//
//bool CFileOperation::CanDelete(CString sPathName)
//{
// DWORD dwAttr = GetFileAttributes(sPathName);
// if (dwAttr == -1) return false;
// if (dwAttr & FILE_ATTRIBUTE_READONLY)
// {
//  if (m_bAskIfReadOnly)
//  {
//   CString sTmp = sPathName;
//   int pos = sTmp.ReverseFind('\\');
//   if (pos != -1) sTmp.Delete(0, pos + 1);
//   CString sText = sTmp + CString(" is read olny. Do you want delete it?");
//   int iRes = MessageBox(NULL, sText, _T("Warning"), MB_YESNOCANCEL | MB_ICONQUESTION);
//   switch (iRes)
//   {
//    case IDYES:
//    {
//     if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
//     return true;
//    }
//    case IDNO:
//    {
//     return false;
//    }
//    case IDCANCEL:
//    {
//     m_bAborted = true;
//     throw new CFExeption(0);
//     return false;
//    }
//   }
//  }
//  else
//  {
//   if (!SetFileAttributes(sPathName, FILE_ATTRIBUTE_NORMAL)) return false;
//   return true;
//  }
// }
// return true;
//}
//
//
//CString CFileOperation::ParseFolderName(CString sPathName)
//{
// CString sFolderName = sPathName;
// int pos = sFolderName.ReverseFind('\\');
// if (pos != -1) sFolderName.Delete(pos, sFolderName.GetLength() - pos);
// pos = sFolderName.ReverseFind('\\');
// if (pos != -1) sFolderName = sFolderName.Right(sFolderName.GetLength() - pos - 1);
// else sFolderName.Empty();
// return sFolderName;
//}
//
//
//void CFileOperation::CheckSelfRecursion(CString sSource, CString sDest)
//{
// if (sDest.Find(sSource) != -1)
// {
//  int i = 0, count1 = 0, count2 = 0;
//  for(i = 0; i < sSource.GetLength(); i ++) if (sSource[i] == '\\') count1 ++;
//  for(i = 0; i < sDest.GetLength(); i ++) if (sDest[i] == '\\') count2 ++;
//  if (count2 >= count1) m_iRecursionLimit = count2 - count1;
// }
//}
//
//
//bool CFileOperation::CheckSelfCopy(CString sSource, CString sDest)
//{
// bool bRes = false;
// if (CheckPath(sSource) == PATH_IS_FOLDER)
// {
//  CString sTmp = sSource;
//  int pos = sTmp.ReverseFind('\\');
//  if (pos != -1)
//  {
//   sTmp.Delete(pos, sTmp.GetLength() - pos);
//   if (sTmp.CompareNoCase(sDest) == 0) bRes = true;
//  }
// }
// return bRes;
//}



void DlgModelSetupSeq::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	SaveModel(gTestSeqData.strModelName);
	CDialogEx::OnOK();
}

//
//long DlgModelSetupSeq::OnReceiveDataPCB(WPARAM wParam, LPARAM lParam )
//{
//
//	BYTE lbuf[4096];
//	int lcnt;
//	lcnt = (int)wParam;
//
//	if((m_RecvCntPCB < 0)||(m_RecvCntPCB > 4095))
//		m_RecvCntPCB = 0;
//
//	memcpy(lbuf,(void *)lParam,lcnt);
//
//	for(int i = 0; i < lcnt; i++)
//	{
//		m_RecvBufPCB[m_RecvCntPCB++] = lbuf[i];
//		if(m_RecvCntPCB > 4095)
//			m_RecvCntPCB = 0;
//	}
//		
//	
//
//	if(m_RecvCntPCB > 2)
//	{
//		if(m_RecvBufPCB[0] == 0xBB)
//		{
//			if(m_RecvCntPCB >= m_RecvBufPCB[1]+2)
//			{
//			//	memset(lbuf,0,sizeof(lbuf));
//				unsigned char lCheckSum = 0;
//				for(int i = 0; i < m_RecvBufPCB[1]+1; i++)
//				{
//					lCheckSum ^= m_RecvBufPCB[i];
//				}
//
//				if(m_RecvBufPCB[m_RecvBufPCB[1]+1] == lCheckSum)
//				{
//					m_PCBComAck = COM_ACK_OK;
//					/*for(int i = 0; i < m_UartOkAckDataCnt; i++)
//					{
//						if(m_RecvBufPCB[4+i] == m_UartOkAckData[i])
//						{
//							m_PCB_CheckOK = 1;
//						}
//						else
//						{
//							m_PCB_CheckOK = 0;
//							break;
//						}
//					}*/
//
//					//m_PCBComAck = 1;
//				}
//				else
//				{
//					m_PCBComAck = COM_ACK_BUSY;
//				}
//
//
//				CString strtmp;
//				CString strReceive ;
//
//			/*	for(int i = 0; i < m_RecvCntPCB; i++)
//				{
//					strtmp.Format(_T("0x%02x "), m_RecvBufPCB[4]);
//					strReceive += strtmp;
//				}	*/
//				strtmp.Format(_T("0x%02X 0x%02X"), m_RecvBufPCB[4],m_RecvBufPCB[5]);
//				strReceive += strtmp;
//
//				m_EditCheckSum = strReceive;
//				UpdateData(FALSE);
////#ifdef __MESSAGE_DEBUG_MODE__
////				MessageDisplay(2, strReceive);//SetDlgItemText(IDC_EDIT_READ_DATA,strReceive.GetBuffer());
////#endif
//			//	m_PCBComAck = COM_ACK_OK;
//				m_RecvCntPCB = 0;
//							
//			}
//		}
//		else
//		{
//			m_RecvCntPCB = 0;
//		}
//	}
//
//
//
//	return 0;
//}
//
//void DlgModelSetupSeq::OpenComportPCB()//long CAcqVoltageSamples_IntClkDlg::OnReceiveData(WPARAM wParam, LPARAM lParam )
//{
//
//	CString str;
//	char lParityChar[3] = {'N', 'E', 'O'};
//
//	if(g_nParity_Pcb > 2)
//	{
//		g_nParity_Pcb = 0;
//	}
//
//	if(m_ComPortPCB.mPort_Opened)
//	{
//		m_ComPortPCB.ClosePort();
//	}
//	if(m_ComPortPCB.InitPort(this, g_nPort_Pcb, g_nBaud_Pcb, lParityChar[g_nParity_Pcb], 8, 1, EV_RXCHAR, 4096 ))
//	{		
//		m_ComPortPCB.StartMonitoring();
//	}
//	else
//	{
//		str.Format(_T("Audio COM Port %d Open FAIL !!"),  g_nPort_Pcb, g_nBaud_Pcb);
//		MessageBox(str);
//		//MessageDisplay(1, str);
//		/*StatusDisplayMsg(str, 1 );*/
//		//MessageBox(str);
//	}
//
//	m_ComPortPCB.m_RxMessageID = WM_COMM_RXCHAR_PCB;
//}

//
//void DlgModelSetupSeq::DblClickLabelChecksum(LPDISPATCH Cancel)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	//OpenComportPCB();
//
//	BYTE lbuf[128]; 
//	int lcnt;
//
////	int lType = m_ComboPcbType.GetCurSel();
//	CString Str;
//	GetDlgItemText(IDC_EDIT_PCB_COMMAND,Str);
//
//	char lchar = 0;
//	Str.MakeUpper();
//
//	lbuf[0] = 0xAA;
//	lbuf[1] = 0x03;
//	lbuf[2] = 0x01;//lType + 1;
//
//
//	//lchar = Str.GetAt(0);
//	//if(lchar > 0x39)
//	//{
//	//	lchar = lchar - 'A' + 0x0A;
//	//}
//	//else
//	//{
//	//	lchar = lchar - 0x30;
//	//}
//	lbuf[3] = 0x2D;//lchar * 0x10;
//
//	//lchar = Str.GetAt(1);
//	//if(lchar > 0x39)
//	//{
//	//	lchar = lchar - 'A' + 0x0A;
//	//}
//	//else
//	//{
//	//	lchar = lchar - 0x30;
//	//}
//	//lbuf[3] = lbuf[3] + lchar ;
//
//
//
//	unsigned char lCheckSum = 0;
//	for(int i = 0; i < 4; i++)
//	{
//		lCheckSum ^= lbuf[i];
//	}
//	lbuf[4] = lCheckSum;
//
//	memcpy(m_SendBufPCB, lbuf, 5); 	
//	m_SendCntPCB = 5;
//
//	//ComPCBSend();
//
//	m_PCB_COM_PROCESS = COM_MODE_WAIT_ACK;
//	m_PCBComAck =COM_ACK_WAIT;
//	m_PCBWaitCnt = 0;
//	m_PCBResendCnt = 0;
//
//	//if(m_SendBufPCB[3] == 0xc1)
//	//	m_WaitPCB_Ack_Limit = 500;
//	//else
//	//	m_WaitPCB_Ack_Limit = 100;
//
//
//	//MessageDisplay(2, "Command Send!");
//
//	if(m_ComPortPCB.mPort_Opened)
//	{
//		
//		m_ComPortPCB.WriteToPort(m_SendBufPCB, m_SendCntPCB);
//	}
//	else
//	{
//		OpenComportPCB();
//		//MessageBox(_T("Com Port Not Opened !!"));
//		if(m_ComPortPCB.mPort_Opened)
//		{
//			
//			m_ComPortPCB.WriteToPort(m_SendBufPCB, m_SendCntPCB);
//		}
//	}
//
//}
//

void DlgModelSetupSeq::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}


void DlgModelSetupSeq::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CDialogEx::OnClose();
}


void DlgModelSetupSeq::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(1);
	//switch(m_PCB_COM_PROCESS)
	//{
	//case COM_MODE_IDLE:
	//	break;
	//case COM_MODE_WAIT_ACK:

	//	if(m_PCBComAck == COM_ACK_OK)
	//	{
	//		m_PCBComAck = COM_ACK_WAIT;
	//		m_PCB_COM_PROCESS = COM_MODE_IDLE;
	//		MessageBox("COM OK !! ");
	//		//MessageDisplay(0, "COM OK !! ");
	//	}
	//	else if(m_PCBComAck == COM_ACK_BUSY)
	//	{
	//		MessageBox("COM Fail !! ");
	//		//MessageDisplay(1, "COM FAIL !! ");
	//		m_PCB_COM_PROCESS = COM_MODE_IDLE;
	//	}
	//	else
	//	{
	//		/*
	//		int m_WaitCnt;
	//		int m_ResendCnt;
	//		*/
	//		m_PCBWaitCnt++;

	//		if(m_PCBWaitCnt > 150)
	//		{
	//		
	//			m_PCBResendCnt++;
	//			if(m_PCBResendCnt < 3)
	//			{
	//				ComPCBSend();
	//			}
	//			else
	//			{
	//				MessageBox("COM Time Out !! ");
	//				//MessageDisplay(1, "COM Time Out !!");
	//				m_PCB_COM_PROCESS = COM_MODE_IDLE;
	//			}
	//		}
	//	}
	//	break;
	//default:
	//	break;

	//}
	SetTimer(1, 30, NULL);
	CDialogEx::OnTimer(nIDEvent);
}


void DlgModelSetupSeq::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();

	gTestSeqData = m_TestOrgSeqData;
}


//void DlgModelSetupSeq::OnCbnSelchangeComboMesPortType()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	int lsel = m_cComboPortType.GetCurSel( );
//	
//	if(lsel == LINE_CM89)
//	{
//		m_ServerPortNumber.Format(_T("%d"), g_nRemotePortNumberCM89);
//	}
//	else
//	{
//		m_ServerPortNumber.Format(_T("%d"), g_nRemotePortNumberCM45);
//	}
//	UpdateData(FALSE);
//}
//

void DlgModelSetupSeq::OnBnClickedButtonSaveas()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString lstrPath;
	lstrPath = PATH_BASE_DIR;
	lstrPath += _T("\\Model\\");

	CString InitNewName;
	InitNewName = gTestSeqData.strModelName;
	InitNewName += ".mdl";

	CFileDialog FileDlg(FALSE, _T("mdl"), InitNewName, OFN_OVERWRITEPROMPT, _T("mdl 파일(*.mdl)|*.mdl||")); 
	FileDlg.m_ofn.lpstrInitialDir = lstrPath;

	CString FilenameOrg = PATH_BASE_DIR;
	FilenameOrg += _T("\\Model\\");
	FilenameOrg += gTestSeqData.strModelName;

		
	CString FilenameOrgSeq;
	FilenameOrgSeq = FilenameOrg;
	FilenameOrg += _T(".mdl");
	FilenameOrgSeq += _T(".seq");
	
	if (FileDlg.DoModal() == IDOK) {   
		// 파일경로 가져오기    
		CString Filename = FileDlg.GetPathName();
		CString lModelName =  FileDlg.GetFileName();
		int lpos = lModelName.Find(".mdl");
		lModelName = lModelName.Left(lpos);

		if(FilenameOrg.Compare(Filename) == 0)
		{
		}
		else
		{
			//if(!CopyFile(FilenameOrg, Filename, 0))
			//{
			//	if(IDNO == MessageBox("File Exist Update Old File?","Warning", MB_YESNO) )
			//	{
			//		if(!CopyFile(FilenameOrg, Filename, 1))
			//		{
			//			MessageBox("File Copy FAIL!!","Warning") ;
			//			return;
			//		}
			//	}
			//}
			//	
			//if(!CopyFile(FilenameOrg, Filename, 0))
			//{
			//	if(IDNO == MessageBox("File Exist Update Old File?","Warning", MB_YESNO) )
			//	{
			//		if(!CopyFile(FilenameOrg, Filename, 1))
			//		{
			//			MessageBox("File Copy FAIL!!","Warning") ;
			//			return;
			//		}
			//	}
			//}
			//else
			//{

				if(lModelName.GetLength() < 3)
				{
					MessageBox("New Name Is Too Short !!");
					return;
				}

				for(int i = 0 ; i < lModelName.GetLength(); i++)
				{
					if((lModelName.GetAt(i) == ' ')||(lModelName.GetAt(i) == '\t'))
					{
						lModelName.SetAt(i, '_');
					}
				}

				if(NameListCheckAndADD(lModelName))
				{
					int lok = MessageBoxA(_T("New Name Is IN MODEL LIST !!") , _T("Used Found"), MB_YESNO |MB_DEFBUTTON2  );
					if(IDYES != lok)
					{
						return;
					}
				}

			//	m_cModelComboSeq.SetCurSel(m_cModelComboSeq.GetCount()-1);
			//}

			if(!CopyFile(FilenameOrg, Filename, 0))
			{
				MessageBox("File Copy FAIL!!","Warning") ;
				return;
			}
			Filename = Filename.Left(Filename.Find(".mdl"));
			Filename +=  _T(".seq");
			if(!CopyFile(FilenameOrgSeq, Filename, 0))
			{
				MessageBox("File Copy FAIL!!","Warning") ;
				return;
			}
			gTestSeqData.strModelName = lModelName;
		//	gTestSeqData.strSeqFileName = lModelName;
			GetModelList();
			SaveModel(lModelName);

			LoadTestData();

			DisplayTestData();
		}


		
		

	} 
}


void DlgModelSetupSeq::OnBnClickedButtonGeswverMd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int lMicomTypeOld =  m_cComboMicomHKHQ.GetCurSel();// g_AVR_Type_HK_HQ); m_Radio_HK_HQ;
	//g_AVR_Type_HK_HQ = m_cComboMicomHKHQ.GetCurSel();// g_AVR_Type_HK_HQ); m_Radio_HK_HQ;

	if (m_P_Main->GetDetParam())
	{

		m_EditFPGAVer.Format(_T("%d.%d.%d"),
			m_P_Main->m_FPGA_Ver_event,
			m_P_Main->m_FPGA_Ver_major,
			m_P_Main->m_FPGA_Ver_minor);

		m_EditDXD_ServerVer.Format(_T("%d.%d%d.%02d"),
			m_P_Main->m_DSERVER_Ver_event,
			m_P_Main->m_DSERVER_Ver_major,
			m_P_Main->m_DSERVER_Ver_minor,
			m_P_Main->m_DSERVER_Ver_release);

		m_EditMicomVer.Format(_T("%d.%d.%d"),
			m_P_Main->m_Microchip_Ver_event,
			m_P_Main->m_Microchip_Ver_major,
			m_P_Main->m_Microchip_Ver_minor);

		m_EditAVR_Ver.Format(_T("%d.%d.%d"),
			m_P_Main->m_AVR_Ver_event,
			m_P_Main->m_AVR_Ver_major,
			m_P_Main->m_AVR_Ver_minor);
		if (lMicomTypeOld != g_AVR_Type_HK_HQ)
		{
			m_cComboMicomHKHQ.SetCurSel(g_AVR_Type_HK_HQ);
			OnCbnSelchangeComboMicomHkhqTypeMd();
		}
		UpdateData(FALSE);
	}
}


void DlgModelSetupSeq::OnCbnSelchangeComboMicomHkhqTypeMd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static CString lModelNameHQ;
	static CString lModelNameHK;

	int lSel = m_cComboMicomHKHQ.GetCurSel();
	g_AVR_Type_HK_HQ = lSel;
	if (g_AVR_Type_HK_HQ == MICOM_TYPE_HQ)
	{
		SetDlgItemText(IDC_STATIC_AED_DROP, "Drop Micom ver.");//
		m_cComboSetModelName.ResetContent();
		//char *modelName[DXD_MODEL_COUNT] = { "10HQ701G", "14HQ701G", "14HQ901G", "17HQ701G", "17HQ901G", "14HQ721G", "17HQ901G_D" };
		int lSeL = 0;
		for (int i = 0; i < DXD_MODEL_COUNT; i++)
		{
			m_cComboSetModelName.AddString(gHQ_ModelNmae[i].GetString());
				if (gHQ_ModelNmae[i].Compare(g_SetModelName) == 0)
				{
					lModelNameHQ = g_SetModelName;
					lSeL = i;
				}
		}
			m_cComboSetModelName.SetCurSel(lSeL);
		//m_cComboSetModelName.AddString("10HQ701G");
		//m_cComboSetModelName.AddString("14HQ701G");
		//m_cComboSetModelName.AddString("14HQ901G");
		//m_cComboSetModelName.AddString("17HQ701G");
		//m_cComboSetModelName.AddString("17HQ901G");
		//m_cComboSetModelName.AddString("14HQ721G");
		//m_cComboSetModelName.AddString("17HQ901G_D");
	}
	else
	{
		SetDlgItemText(IDC_STATIC_AED_DROP, "AED Micom ver.");//		
		m_cComboSetModelName.ResetContent();
		//char *gHK_ModelName[DXD_HK_MODEL_NUM] = { "17HK700G", "14HK701G", "17HK701G"};
		int lSeL = 0;
		for (int i = 0; i < DXD_HK_MODEL_NUM; i++)
		{
			m_cComboSetModelName.AddString(gHK_ModelName[i].GetString());
			if (gHK_ModelName[i].Compare(g_SetModelName) == 0)
			{
				lModelNameHK = g_SetModelName;
				lSeL = i;
			}
		}
			m_cComboSetModelName.SetCurSel(lSeL);
		//m_cComboSetModelName.AddString("17HK700G");
		//m_cComboSetModelName.AddString("14HK701G");
		//m_cComboSetModelName.AddString("17HK701G");		
	}

}


void DlgModelSetupSeq::OnBnClickedButtonGetModelName()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strModelName;
	if (m_P_Main->GetModelname(strModelName))
	{
		SetDlgItemText(IDC_STATIC_READ_MODEL_NAME, strModelName);
	}
	else
	{
		MessageBox("Read Name Fail!!");
	}
}
