// DlgBarcode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgBarcode.h"
#include "afxdialogex.h"


// CDlgBarcode 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgBarcode, CDialogEx)

CDlgBarcode::CDlgBarcode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBarcode::IDD, pParent)
{

}

CDlgBarcode::~CDlgBarcode()
{
}

void CDlgBarcode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL__BARCODE_1, m_clabelBarCode);
}


BEGIN_MESSAGE_MAP(CDlgBarcode, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgBarcode::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgBarcode::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CDlgBarcode::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
		else if(pMsg->wParam == VK_RETURN)
		{
			//ClickCbtnStart();
			if(m_BarcodeString.GetLength() > 6)
			{
				m_Barcode = m_BarcodeString;
				OnBnClickedOk();
			}
			m_BarcodeString = _T("");
			return TRUE;
		}
	}
		
	else if(pMsg->message == WM_CHAR)
	{
		m_BarcodeString = m_BarcodeString  + (char)(pMsg->wParam);
		m_BarcodeString.MakeUpper();
		m_clabelBarCode.put_Caption(m_BarcodeString);
		return TRUE;		
	}	
	
	
	return CDialog::PreTranslateMessage(pMsg);
}

// CDlgBarcode 메시지 처리기입니다.


void CDlgBarcode::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ReturnFlag = 1;
	m_clabelBarCode.put_Caption("");
	CDialogEx::OnOK();
}


void CDlgBarcode::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_ReturnFlag = 2;
	CDialogEx::OnCancel();
	m_clabelBarCode.put_Caption("");
}
