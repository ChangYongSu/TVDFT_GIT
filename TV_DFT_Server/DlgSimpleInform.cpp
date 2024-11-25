// DlgSimpleInform.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgSimpleInform.h"
#include "afxdialogex.h"


// CDlgSimpleInform ��ȭ �����Դϴ�.

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


// CDlgSimpleInform �޽��� ó�����Դϴ�.


BOOL CDlgSimpleInform::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	  m_Count = 0;
//	m_EditLEDPortString.Format(_T("%d"),m_nPortLED );

	SetTimer(1, 100, NULL);
	




	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlgSimpleInform::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if(bShow)
		SetTimer(1, 100, NULL);
	m_cLabelInformDisplay.put_Caption(m_DisplayString);
	 //m_Count = 0;

	if(m_Count > 10000)
	{
		m_Count = 10000;
	}

}
