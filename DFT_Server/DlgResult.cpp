// DlgResult.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgResult.h"
#include "afxdialogex.h"

#include "AcqVoltageSamples_IntClkDlg.h"

// CDlgResult 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgResult, CDialogEx)

CDlgResult::CDlgResult(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgResult::IDD, pParent)
//	, m_EditNgDisplay(_T(""))
//	, m_DetailMessage(_T(""))
{

}

CDlgResult::~CDlgResult()
{
}

void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LABEL_RESULT_OK, m_cBtnResultOK);
	//DDX_Control(pDX, IDC_LABEL_RESULT_NG, m_cBtnResultNG);
	//DDX_Control(pDX, IDC_LABEL_RESULT, m_cBtnResult);
	//DDX_Control(pDX, IDC_BUTTON_MODEL_CONFIG, m_cBtnResult);
	DDX_Control(pDX, IDC_LABEL__OKNG, m_cBtnResult);
	//DDX_Control(pDX, IDC_LABEL_NG_DISPLAY, m_cLabelNGDisplay);
	//DDX_Text(pDX, IDC_EDIT_NG_DISPLAY, m_EditNgDisplay);
}


BEGIN_MESSAGE_MAP(CDlgResult, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CDlgResult::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgResult::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BUTTON_DETAIL, &CDlgResult::OnBnClickedButtonDetail)
	ON_WM_TIMER()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


// CDlgResult 메시지 처리기입니다.
BOOL CDlgResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	/*if(m_Result == 0)
	{
		m_cBtnResult.put_BackColor(0x000000FF);
		m_cBtnResult.put_Caption("N G");
	}
	else
	{
		m_cBtnResult.put_BackColor(0x0000FF00);
		m_cBtnResult.put_Caption("O K");
	}*/
//	SetTimer(1, 1500, NULL);
	time_t ti;
	time(&ti);
	struct tm *t = localtime(&ti);	
	//if(((t->tm_wday == 1)||(t->tm_wday == 5))&&((t->tm_mon >= 6)||(t->tm_year > 116)))
	if(0)//(t->tm_wday == 5)&&((t->tm_mon >= 8)||(t->tm_year > 116)))
	{
		m_HeartEnable = 1;
	}
	else
	{
		m_HeartEnable = 0;
	}


	return TRUE;
}

BOOL CDlgResult::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			//ClickCbtnStart();
			//if(m_BarcodeString.GetLength() > 6)
			//{
			//	m_Barcode = m_BarcodeString;
			//	
			//}
			//m_BarcodeString = _T("");
			return TRUE;
		}
	}
		
	else if(pMsg->message == WM_CHAR)
	{
		//m_BarcodeString = m_BarcodeString  + (char)(pMsg->wParam);
		//m_BarcodeString.MakeUpper();
		//((CAcqVoltageSamples_IntClkDlg *)pDlg)->m_Barcode.put_Caption(m_BarcodeString);
		////m_BarCode.put_Caption(m_BarcodeString);
		return TRUE;		
	}	
	
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgResult::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//m_HeartEnable = 0;
	if(bShow == 1)
	{
		m_ReturnFlag = 0;
		if(m_Result == 0)
		{
		//	m_cBtnResultNG.ShowWindow(1);
		//	m_cBtnResultOK.ShowWindow(0);
			m_cBtnResult.put_BackColor(0x000000FF);
			m_cBtnResult.put_Caption("N G");
			GetDlgItem(IDOK)->ShowWindow(TRUE);
//			m_cLabelNGDisplay.put_Caption(m_DisplayString);
		//	m_EditNgDisplay = m_DisplayString;
		}
		else
		{
		//	m_cBtnResultNG.ShowWindow(0);
		//	m_cBtnResultOK.ShowWindow(1);
			m_cBtnResult.put_BackColor(0x0000FF00);
			m_cBtnResult.put_Caption("O K");
			GetDlgItem(IDOK)->ShowWindow(FALSE);
		//	m_cLabelNGDisplay.put_Caption("CHECK PASS");
		//	m_EditNgDisplay = "";
			
		}
		SetTimer(1, 50, NULL);
	}
	else
	{
		KillTimer(1);
	}

	UpdateData(FALSE);
}


void CDlgResult::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ReturnFlag = 1;
	CDialogEx::OnOK();
}


void CDlgResult::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ReturnFlag = 2;

	CDialogEx::OnCancel();
}

//
//void CDlgResult::OnBnClickedButtonDetail()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDlgMessage dlg;
//	dlg.m_EditMessage =  m_DisplayString;
//	dlg.m_EditMessage +=  "\r\n";
//	dlg.m_EditMessage += m_DetailMessage;
//	int rtn  = dlg.DoModal();
//}
//

void CDlgResult::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	static int scnt = 0;

	//if((m_Result != 1)||(g_nRemotePortNumberCM45 != 40134))
	//{
	//	KillTimer(1);
	//}
	scnt++;
	if(scnt >= 50)
	{
	//	if(m_Result == 1)
	//	{
	//		m_cBtnResult.put_BackColor(0x0000FF00);
	//		m_cBtnResult.put_Caption("O K");
	//		GetDlgItem(IDOK)->ShowWindow(FALSE);
	////		m_cLabelNGDisplay.put_Caption("CHECK PASS");
	//	}		
		OnBnClickedOk();
		scnt = 0;
	}
//	else if(scnt == 30)
//	{
//		if(m_Result == 1)
//		{
//			if(m_HeartEnable == 1)
//			{
//				m_cBtnResult.put_BackColor(0x0000FF00);
//				m_cBtnResult.put_Caption("O K");
//				GetDlgItem(IDOK)->ShowWindow(FALSE);
////				m_cLabelNGDisplay.put_Caption("CHECK PASS\r\n♡♡♡♡♡♡♡♡");
//			}
//		}	
//	}

	//SetTimer(1, 50, NULL);

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgResult::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_HeartEnable)
	{
		m_HeartEnable = 0;
	}
	else
	{
		m_HeartEnable = 1;
	}
	CDialogEx::OnRButtonDblClk(nFlags, point);
}
