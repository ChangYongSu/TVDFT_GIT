// DlgSimpleInform.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgSimpleInform.h"
#include "afxdialogex.h"


// CDlgSimpleInform 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSimpleInform, CDialogEx)

CDlgSimpleInform::CDlgSimpleInform(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSimpleInform::IDD, pParent)
{

}

CDlgSimpleInform::~CDlgSimpleInform()
{
}

void CDlgSimpleInform::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_DISPLAY, m_cLabelInformDisplay);
}


BEGIN_MESSAGE_MAP(CDlgSimpleInform, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgSimpleInform 메시지 처리기입니다.


BOOL CDlgSimpleInform::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	  m_Count = 0;
//	m_EditLEDPortString.Format(_T("%d"),m_nPortLED );

	SetTimer(1, 100, NULL);
	




	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSimpleInform::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//static int m_Count = 0;
	m_Count -= 100;
	if(m_Count < 0)
	{
		ShowWindow(SW_HIDE);
		KillTimer(1);

	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgSimpleInform::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(bShow)
		SetTimer(1, 100, NULL);
	m_cLabelInformDisplay.put_Caption(m_DisplayString);
	 //m_Count = 0;

	if(m_Count > 10000)
	{
		m_Count = 10000;
	}

}
