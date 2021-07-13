// DlgReadyCodeSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgReadyCodeSetup.h"
#include "afxdialogex.h"


// CDlgReadyCodeSetup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgReadyCodeSetup, CDialogEx)

CDlgReadyCodeSetup::CDlgReadyCodeSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgReadyCodeSetup::IDD, pParent)
	//, m_EditRdSq[0](_T(""))
{

}

CDlgReadyCodeSetup::~CDlgReadyCodeSetup()
{
}

void CDlgReadyCodeSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_RD_RMT_CODE, m_cComboReRmtCode);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_1, m_cComboRdSeq[0]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_2, m_cComboRdSeq[1]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_3, m_cComboRdSeq[2]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_4, m_cComboRdSeq[3]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_5, m_cComboRdSeq[4]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_6, m_cComboRdSeq[5]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_7, m_cComboRdSeq[6]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_8, m_cComboRdSeq[7]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_9, m_cComboRdSeq[8]);
	DDX_Control(pDX, IDC_COMBO_RDSEQ_10, m_cComboRdSeq[9]);

	DDX_Text(pDX, IDC_EDIT_RDSQ1, m_EditRdSq[0]);
	DDX_Text(pDX, IDC_EDIT_RDSQ2, m_EditRdSq[1]);
	DDX_Text(pDX, IDC_EDIT_RDSQ3, m_EditRdSq[2]);
	DDX_Text(pDX, IDC_EDIT_RDSQ4, m_EditRdSq[3]);
	DDX_Text(pDX, IDC_EDIT_RDSQ5, m_EditRdSq[4]);
	DDX_Text(pDX, IDC_EDIT_RDSQ6, m_EditRdSq[5]);
	DDX_Text(pDX, IDC_EDIT_RDSQ7, m_EditRdSq[6]);
	DDX_Text(pDX, IDC_EDIT_RDSQ8, m_EditRdSq[7]);
	DDX_Text(pDX, IDC_EDIT_RDSQ9, m_EditRdSq[8]);
	DDX_Text(pDX, IDC_EDIT_RDSQ10, m_EditRdSq[9]);
//IDC_COMBO_RDSEQ_1 IDC_EDIT_RDSQ1

}


BEGIN_MESSAGE_MAP(CDlgReadyCodeSetup, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_1, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq1)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_2, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq2)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_3, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq3)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_4, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq4)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_5, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq5)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_6, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq6)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_7, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq7)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_8, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq8)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_9, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq9)
	ON_CBN_SELCHANGE(IDC_COMBO_RDSEQ_10, &CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq10)
	ON_BN_CLICKED(IDOK, &CDlgReadyCodeSetup::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgReadyCodeSetup 메시지 처리기입니다.


BOOL CDlgReadyCodeSetup::OnInitDialog()
{
    CDialog::OnInitDialog();
  
	

	for(int i = 0; i < 10; i++)
	{
		m_cComboRdSeq[i].AddString(" END "); 
		m_cComboRdSeq[i].AddString("DELAY"); 
		m_cComboRdSeq[i].AddString("PRE_KEY"); 		
		m_cComboRdSeq[i].AddString("REPEAT_KEY"); 
		m_cComboRdSeq[i].SetCurSel(0);
	}

	GetRdRmtCodeList(gTestSeqData.strRemoconFileName);


	DisplaySeqData();
	
	
    return TRUE;
}



void CDlgReadyCodeSetup::GetRdRmtCodeList(CString lRmtFile)//gTestSeqData.strRemoconFileName;
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	FILE *m_fp;
	CString str;
	CString Filename;
	char lbuf[1024];
	char lTemp_Company[64];
	char lTemp_Code[64];
	char lTemp_Repeat[64];
	char lTemp_Name[64];
	char lTemp_Discription[129];

	

	str = PATH_BASE_DIR;
	str += _T("\\Remote\\");
	str += lRmtFile;//gTestSeqData.strRemoconFileName;
	
	// 파일경로 가져오기    
	 
	Filename = str;
		
	fopen_s(&m_fp, (LPCSTR)Filename, "r");
	if(m_fp)
	{
		m_RemoconModel.m_Cnt = 0;
		//memset(&m_RemoconModel, 0, sizeof(Recomon_ModelT));
		while(fgets(lbuf, 1024, m_fp) != NULL) 
		{
			if(lbuf[0] == '[')
			{
				continue;
			}
			if(strlen(lbuf) < 16)
			{
				break;
			}
			sscanf_s(lbuf, "%s %s %s %s", lTemp_Company,32, lTemp_Code,32,lTemp_Repeat,32, lTemp_Name,32);
			if(lTemp_Company[strlen(lTemp_Company)-1] == ',')
			{
				lTemp_Company[strlen(lTemp_Company)-1] = 0 ;
			}
				
			if(lTemp_Code[strlen(lTemp_Code)-1] == ',')
			{
				lTemp_Code[strlen(lTemp_Code)-1] = 0 ;
			}
				
			if(lTemp_Repeat[strlen(lTemp_Repeat)-1] == ',')
			{
				lTemp_Repeat[strlen(lTemp_Repeat)-1] = 0; 
			}

			if(lTemp_Name[strlen(lTemp_Name)-1] == ',')
			{
				lTemp_Name[strlen(lTemp_Name)-1] = 0;
			}

			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Company.Format(_T("%s"),lTemp_Company);
			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Code.Format(_T("%s"),lTemp_Code); 
			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Repeat = atoi(lTemp_Repeat);
			m_RemoconModel.Data[m_RemoconModel.m_Cnt].m_Name.Format(_T("%s"),lTemp_Name); 
			m_RemoconModel.m_Cnt++;	
			if(m_RemoconModel.m_Cnt >= 200)
			{
				break;
			}
		}
					

		fclose(m_fp);	
		//SetDlgItemText(IDC_STATIC_RMT_FILE, Filename);
		
		m_cComboReRmtCode.ResetContent();
		
		for(int i = 0; i < m_RemoconModel.m_Cnt; i++)
		{
			/*str = m_RemoconModel.Data[i].m_Company;
			str += _T(" : "); */
			str = m_RemoconModel.Data[i].m_Name;
			m_cComboReRmtCode.AddString(str);
		}
		m_cComboReRmtCode.SetCurSel(0);	
		
	} 
}

void CDlgReadyCodeSetup::DisplaySeqData()
{

	//IDC_COMBO_RDSEQ_1 IDC_EDIT_RDSQ1
	int lSeqComboID[10] = {
		IDC_COMBO_RDSEQ_1,
		IDC_COMBO_RDSEQ_2,
		IDC_COMBO_RDSEQ_3,
		IDC_COMBO_RDSEQ_4,
		IDC_COMBO_RDSEQ_5,
		IDC_COMBO_RDSEQ_6,
		IDC_COMBO_RDSEQ_7,
		IDC_COMBO_RDSEQ_8,
		IDC_COMBO_RDSEQ_9,
		IDC_COMBO_RDSEQ_10
	};
	

	int lSeqEditID[10] = {
		IDC_EDIT_RDSQ1, 
		IDC_EDIT_RDSQ2, 
		IDC_EDIT_RDSQ3, 
		IDC_EDIT_RDSQ4, 
		IDC_EDIT_RDSQ5, 
		IDC_EDIT_RDSQ6, 
		IDC_EDIT_RDSQ7, 
		IDC_EDIT_RDSQ8, 
		IDC_EDIT_RDSQ9, 
		IDC_EDIT_RDSQ10
	};

	for( int i = 0; i < 10; i++)
	{		
		if(m_RdTestProcess.aSeqData[i].nSeqType == TEST_SEQ_TYPE_DELAY)
		{
			m_cComboRdSeq[i].SetCurSel(1);
			m_EditRdSq[i].Format(_T("%d"), m_RdTestProcess.aSeqData[i].nDelay);
			GetDlgItem(lSeqEditID[i])->EnableWindow(1);
			GetDlgItem(lSeqComboID[i])->EnableWindow(1);
		}
		else if(m_RdTestProcess.aSeqData[i].nSeqType == TEST_SEQ_TYPE_MP_KEY)
		{
			m_cComboRdSeq[i].SetCurSel(2);
			m_EditRdSq[i] = m_RdTestProcess.aSeqData[i].strMP_Key;
			GetDlgItem(lSeqEditID[i])->EnableWindow(1);
			GetDlgItem(lSeqComboID[i])->EnableWindow(1);
		}
		else if(m_RdTestProcess.aSeqData[i].nSeqType == TEST_SEQ_TYPE_MP_REPEAT_KEY)
		{
			m_cComboRdSeq[i].SetCurSel(3);
			m_EditRdSq[i] = m_RdTestProcess.aSeqData[i].strMP_Key;
			GetDlgItem(lSeqEditID[i])->EnableWindow(1);
			GetDlgItem(lSeqComboID[i])->EnableWindow(1);
		}
		else
		{
			m_RdTestProcess.aSeqData[i].nSeqType = TEST_SEQ_TYPE_END;
			m_cComboRdSeq[i].SetCurSel(0);
			m_EditRdSq[i] = _T("");
			GetDlgItem(lSeqEditID[i])->EnableWindow(1);
			GetDlgItem(lSeqComboID[i])->EnableWindow(1);
			i++;
			for(;i < 10; i++)
			{
				GetDlgItem(lSeqEditID[i])->EnableWindow(0);
				GetDlgItem(lSeqComboID[i])->EnableWindow(0);
			}
		}
	}
	UpdateData(FALSE);
}

void CDlgReadyCodeSetup::EnableComboSeq()
{
	int i = 0;
	int lSeqComboID[10] = {
		IDC_COMBO_RDSEQ_1,
		IDC_COMBO_RDSEQ_2,
		IDC_COMBO_RDSEQ_3,
		IDC_COMBO_RDSEQ_4,
		IDC_COMBO_RDSEQ_5,
		IDC_COMBO_RDSEQ_6,
		IDC_COMBO_RDSEQ_7,
		IDC_COMBO_RDSEQ_8,
		IDC_COMBO_RDSEQ_9,
		IDC_COMBO_RDSEQ_10
	};
	

	int lSeqEditID[10] = {
		IDC_EDIT_RDSQ1, 
		IDC_EDIT_RDSQ2, 
		IDC_EDIT_RDSQ3, 
		IDC_EDIT_RDSQ4, 
		IDC_EDIT_RDSQ5, 
		IDC_EDIT_RDSQ6, 
		IDC_EDIT_RDSQ7, 
		IDC_EDIT_RDSQ8, 
		IDC_EDIT_RDSQ9, 
		IDC_EDIT_RDSQ10
	};

	int lIndex;
	for(i = 0; i < 9; i++)
	{
		lIndex = m_cComboRdSeq[i].GetCurSel();
		if(lIndex == 0)
		{
			GetDlgItem(lSeqComboID[i+1])->EnableWindow(0);
			GetDlgItem(lSeqEditID[i+1])->EnableWindow(0);
			m_cComboRdSeq[i+1].SetCurSel(0);
		}
		else
		{
			GetDlgItem(lSeqComboID[i+1])->EnableWindow(1);
			GetDlgItem(lSeqEditID[i+1])->EnableWindow(1);
		}
	}
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}

void CDlgReadyCodeSetup::OnCbnSelchangeComboRdseq10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EnableComboSeq();
}


void CDlgReadyCodeSetup::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateListData();
	CDialogEx::OnOK();
}


int  CDlgReadyCodeSetup::UpdateListData()
{
	int lError = 0;
	int lCurSel = 0;
	
	UpdateData();

		
	for(int i = 0; i < 10; i++)
	{		

		lCurSel = m_cComboRdSeq[i].GetCurSel();
		//m_RdTestProcess.aSeqData[i].nSeqType = m_cComboRdSeq[i].GetCurSel();
		if(lCurSel == 1)
		{
			m_RdTestProcess.aSeqData[i].nSeqType = TEST_SEQ_TYPE_DELAY;
			m_RdTestProcess.aSeqData[i].nDelay = atoi(m_EditRdSq[i].GetBuffer());
		}
		else if(lCurSel == 2)
		{
			m_RdTestProcess.aSeqData[i].nSeqType = TEST_SEQ_TYPE_MP_KEY;
			m_RdTestProcess.aSeqData[i].strMP_Key = m_EditRdSq[i] ;		
		}
		else if(lCurSel == 3)
		{
			m_RdTestProcess.aSeqData[i].nSeqType = TEST_SEQ_TYPE_MP_REPEAT_KEY;
			m_RdTestProcess.aSeqData[i].strMP_Key = m_EditRdSq[i] ;
		}
		else
		{
			m_RdTestProcess.aSeqData[i].nSeqType =  TEST_SEQ_TYPE_END;
			for(;i < 10; i++)
			{
				m_RdTestProcess.aSeqData[i].nSeqType = TEST_SEQ_TYPE_END;
			}
		}
	}

	return lError;
}
