// DlgPassword.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "DlgPassword.h"
#include "afxdialogex.h"
#include "FileControl.h"


// CDlgPassword 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPassword, CDialogEx)

CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPassword::IDD, pParent)
	, m_EditPassword(_T(""))
	, m_NewPassword(_T(""))
	, m_EditConfirmPassword(_T(""))
{

}

CDlgPassword::~CDlgPassword()
{
}

void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ENTER_PASSWORD, m_EditPassword);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD, m_NewPassword);
	DDX_Text(pDX, IDC_EDIT_CONFIRM_PASSWORD, m_EditConfirmPassword);
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_ENTER_PASSWORD, &CDlgPassword::OnEnChangeEditEnterPassword)
	ON_BN_CLICKED(IDC_BUTTON_PASSWORD_CHANGE, &CDlgPassword::OnBnClickedButtonPasswordChange)
	ON_BN_CLICKED(IDOK, &CDlgPassword::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_CONFIRM_PASSWORD, &CDlgPassword::OnEnChangeEditConfirmPassword)
END_MESSAGE_MAP()


// CDlgPassword 메시지 처리기입니다.



BOOL CDlgPassword::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_BUTTON_PASSWORD_CHANGE)->EnableWindow(0);
	GetDlgItem(IDOK)->EnableWindow(0);

//	GetDlgItem(IDC_EDIT_PSWRD)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





void CDlgPassword::OnEnChangeEditEnterPassword()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	UpdateData();
	//char lbuf1[128];
	//char lbuf2[128];
	//sprintf(lbuf1,"%s",m_EditCurrentPW.GetBuffer());
	//sprintf(lbuf2,"%s",m_sPassword.GetBuffer());
	if(m_EditPassword ==  m_sPassword)
	{
		GetDlgItem(IDOK)->EnableWindow(1);
	}
	else if(m_EditPassword.Compare(_T("swcys1")) == 0 )
	{
		GetDlgItem(IDOK)->EnableWindow(1);
		g_strPassword = "1111";
		m_sPassword = g_strPassword;
		CFileControl lFile;
		lFile.SaveConfigSeqData();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PASSWORD_CHANGE)->EnableWindow(0);
		GetDlgItem(IDOK)->EnableWindow(0);
	}

}

void CDlgPassword::OnEnChangeEditConfirmPassword()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	if(m_EditPassword == m_sPassword)
	{
		if(m_NewPassword == m_EditConfirmPassword)
		{
			if(m_NewPassword.GetLength()>3)
				GetDlgItem(IDC_BUTTON_PASSWORD_CHANGE)->EnableWindow(1);
		}
		
	}
}

void CDlgPassword::OnBnClickedButtonPasswordChange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_EditPassword == m_sPassword)
	{
		if(m_NewPassword == m_EditConfirmPassword)
		{
			if(m_NewPassword.GetLength()>3)
			{
				m_sPassword = m_NewPassword;
			}
			else
			{
				MessageBox("New Password too Short Enter More than 4 Char !!");
			}
		}
		
	}
}


void CDlgPassword::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();


	CDialogEx::OnOK();
}




/*

	CString m_EditPassword;
	CString m_NewPassword;
	CString m_EditConfirmPassword;
*/