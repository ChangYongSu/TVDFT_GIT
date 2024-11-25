// DlgNewModelName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgNewModelName.h"
#include "afxdialogex.h"


// CDlgNewModelName 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgNewModelName, CDialogEx)

CDlgNewModelName::CDlgNewModelName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgNewModelName::IDD, pParent)
	, m_NewName(_T(""))
{

}

CDlgNewModelName::~CDlgNewModelName()
{
}

void CDlgNewModelName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEW_NAME, m_NewName);
}


BEGIN_MESSAGE_MAP(CDlgNewModelName, CDialogEx)
END_MESSAGE_MAP()


// CDlgNewModelName 메시지 처리기입니다.
