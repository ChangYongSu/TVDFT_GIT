// CUserMsgColorDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "DATsys.h"
#include "CUserMsgColorDlg.h"
#include "afxdialogex.h"
#include "global.h"


extern CDATsysView*  g_pView;


// CUserMsgColorDlg 대화 상자

IMPLEMENT_DYNAMIC(CUserMsgColorDlg, CDialogEx)

CUserMsgColorDlg::CUserMsgColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USER_MSG_COLOR_DLG, pParent)
{
	m_bOptionCheckEnable = TRUE;
}

CUserMsgColorDlg::~CUserMsgColorDlg()
{
}

void CUserMsgColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserMsgColorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_YES, &CUserMsgColorDlg::OnBnClickedBtnYes)
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CUserMsgColorDlg 메시지 처리기


BOOL CUserMsgColorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	GetWindowRect(&m_dlgRect);

	int nLeft = g_pView->m_FormViewRect.left + (g_pView->m_FormViewRect.Width() / 2) - (m_dlgRect.Width() / 2)-100;
	int nTop = (g_pView->m_FormViewRect.Height() / 2) - 100; // - (m_dlgRect.Height()/2);
	int nWidth = m_dlgRect.Width();
	int nHeight = m_dlgRect.Height();

	MoveWindow(nLeft, nTop, nWidth, nHeight);


	//ShowMessage1(m_szMsg1);
	//ShowMessage2(m_szMsg2);

	//UpdateData(FALSE);
	////+add kwmoon 080508
	//m_bYes = TRUE;

	g_pView->PlayWavSound(IDR_WAVE_RETRY);
	CString Msg;
	Msg = m_szMsg1;
	Msg += "\r\n";
	Msg += m_szMsg2;
	m_cMessageBox.SubclassDlgItem(IDC_MESSAGE_NEW, this);
	m_cMessageBox.SetWindowText(Msg);//IDC_ROBOT_STATUS
	m_cMessageBox.SetBkColor(RGB(250, 250, 64));
	m_cMessageBox.SetTextColor(RGB(255, 0, 0));

	if (m_bFontSetting != 1)
	{
	
		LOGFONT logFont4;
		logFont4.lfHeight = 42;
		logFont4.lfWidth = 14;
		logFont4.lfEscapement = 0;
		logFont4.lfOrientation = 0;
		logFont4.lfWeight = FW_BOLD;
		logFont4.lfItalic = 0;
		logFont4.lfUnderline = 0;
		logFont4.lfStrikeOut = 0;
		logFont4.lfOutPrecision = 1;
		logFont4.lfClipPrecision = 2;
		logFont4.lfQuality = 1;
		logFont4.lfPitchAndFamily = 17;
		logFont4.lfCharSet = ANSI_CHARSET;
		strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Arial");
		//strcpy_s(logFont4.lfFaceName, _countof(logFont4.lfFaceName), (LPCTSTR)"Courier New");

		//CFont l_Step_New_Font;
		m_Step_New_Font.CreateFontIndirect(&logFont4);
		m_bFontSetting = 1;
	}	
	m_cMessageBox.SetFont(&m_Step_New_Font);
		
	
	m_cMessageBox.SetWindowText(Msg);
	//+ add 090902
	m_bActivate = TRUE;
	//-
	if (m_bAutoClose == 1)
	{
		SetTimer(1, 3000, NULL);
	}
	//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
	CString szString;
	szString.Format("UserKeyCheck %x\n", m_pUserKeyCheck->m_nThreadID);
	//	if(g_LogFileOpen) g_LogFile.WriteString(szString);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserMsgColorDlg::OnBnClickedBtnYes()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CUserMsgColorDlg::OnOK()
{
	m_bActivate = FALSE;

	m_bAutoClose = 0;

	CDialog::OnOK();
}

void CUserMsgColorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(nIDEvent);
	OnBnClickedBtnYes();
	CDialogEx::OnTimer(nIDEvent);
}


void CUserMsgColorDlg::SetMessage(CString szMessage1, CString szMessage2)
{
	m_szMsg1 = szMessage1;
	m_szMsg2 = szMessage2;
}


int CUserMsgColorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}
