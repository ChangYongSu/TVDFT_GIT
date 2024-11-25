// CableChangeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "CableChangeDlg.h"
#include "afxdialogex.h"
#include "FileControl.h"

// CCableChangeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCableChangeDlg, CDialogEx)

CCableChangeDlg::CCableChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCableChangeDlg::IDD, pParent)
	, m_EditUsbCableMax(0)
	, m_EditUsbCableCount(0)
	, m_EditPortCableMax(0)
	, m_EditPortCableCount(0)
	, m_EditCheckerMax(0)
	, m_EditCheckerCount(0)
	, m_EditMicCableMax(0)
	, m_EditMicCableCount(0)
	, m_EditCheckerCountCM5(0)
	, m_EditCheckerMaxCM5(0)
{

}

CCableChangeDlg::~CCableChangeDlg()
{
}

void CCableChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USB_CABLE_MAX, m_EditUsbCableMax);
	DDX_Text(pDX, IDC_EDIT_USB_CABLE_USED, m_EditUsbCableCount);
	DDX_Text(pDX, IDC_EDIT_PORT_CABLE_MAX, m_EditPortCableMax);
	DDX_Text(pDX, IDC_EDIT_PORT_CABLE_USED, m_EditPortCableCount);
	DDX_Text(pDX, IDC_EDIT_CHECKER_MAX, m_EditCheckerMax);
	DDX_Text(pDX, IDC_EDIT_CHECKER_USED, m_EditCheckerCount);
	DDX_Text(pDX, IDC_EDIT_MIC_CABLE_MAX, m_EditMicCableMax);
	DDX_Text(pDX, IDC_EDIT_MIC_CABLE_USED, m_EditMicCableCount);
	DDX_Text(pDX, IDC_EDIT_CHECKER_USED2, m_EditCheckerCountCM5);
	DDX_Text(pDX, IDC_EDIT_CHECKER_MAX2, m_EditCheckerMaxCM5);
	DDX_Control(pDX, IDC_LABEL_DISP_USB_CABLE, m_cLabelUsbCable);
	DDX_Control(pDX, IDC_LABEL_PORT_CABLE, m_cLabelPortCable);
	DDX_Control(pDX, IDC_LABEL_DISP_CM4_PIN, m_cLabelCM4Pin);
	DDX_Control(pDX, IDC_LABEL_DISP_CM5_PIN, m_cLabelCM5Pin);
	DDX_Control(pDX, IDC_LABEL_DISP_MIC_CABLE, m_cLabelMICCable);
}


BEGIN_MESSAGE_MAP(CCableChangeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_USB_COUNT_RESET, &CCableChangeDlg::OnBnClickedButtonUsbCountReset)
	ON_BN_CLICKED(IDC_BUTTON_PORT_COUNT_RESET, &CCableChangeDlg::OnBnClickedButtonPortCountReset)
	ON_BN_CLICKED(IDC_BUTTON_CHECKER_PIN_COUNT_RESET, &CCableChangeDlg::OnBnClickedButtonCheckerPinCountReset)
	ON_BN_CLICKED(IDC_BUTTON_MIC_COUNT_RESET, &CCableChangeDlg::OnBnClickedButtonMicCountReset)
	ON_BN_CLICKED(IDOK, &CCableChangeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CHECKER_PIN_COUNT_RESET2, &CCableChangeDlg::OnBnClickedButtonCheckerPinCountReset2)
END_MESSAGE_MAP()


// CDlgResult 메시지 처리기입니다.
BOOL CCableChangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_EditUsbCableMax = gCableManager.nUSB_CableCountMax;;
	m_EditUsbCableCount = gCableManager.nUSB_CableCount;
	m_EditPortCableMax = gCableManager.nPort_CableCountMax;
	m_EditPortCableCount = gCableManager.nPort_CableCount;
	m_EditCheckerMax = gCableManager.nCM4CheckerCountMax;
	m_EditCheckerCount = gCableManager.nCM4CheckerCount;
	m_EditCheckerMaxCM5 = gCableManager.nCM5CheckerCountMax;
	m_EditCheckerCountCM5 = gCableManager.nCM5CheckerCount;
	m_EditMicCableMax = gCableManager.nMIC_CableCountMax;
	m_EditMicCableCount = gCableManager.nMIC_CableCount;
	UpdateData(FALSE);

	if(m_EditUsbCableMax <=	m_EditUsbCableCount)
	{
		m_cLabelUsbCable.put_BackColor(0x000000FF);
	}
	else
	{
		m_cLabelUsbCable.put_BackColor(0x0000FF00);
	}

	if(m_EditPortCableMax <=	m_EditPortCableCount)
	{
		m_cLabelPortCable.put_BackColor(0x000000FF);
	}
	else
	{
		m_cLabelPortCable.put_BackColor(0x0000FF00);
	}

	if(m_EditCheckerMax <=	m_EditCheckerCount)
	{
		m_cLabelCM4Pin.put_BackColor(0x000000FF);
	}
	else
	{
		m_cLabelCM4Pin.put_BackColor(0x0000FF00);
	}

	if(m_EditCheckerMaxCM5 <=	m_EditCheckerCountCM5)
	{
		m_cLabelCM5Pin.put_BackColor(0x000000FF);
	}
	else
	{
		m_cLabelCM5Pin.put_BackColor(0x0000FF00);
	}

	if(m_EditMicCableMax <=	m_EditMicCableCount)
	{
		m_cLabelMICCable.put_BackColor(0x000000FF);
	}
	else
	{
		m_cLabelMICCable.put_BackColor(0x0000FF00);
	}


	return TRUE;
}


// CCableChangeDlg 메시지 처리기입니다.



void CCableChangeDlg::OnBnClickedButtonUsbCountReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	m_EditUsbCableCount = 0;//m_EditUsbCableMax;
	UpdateData(FALSE);
	m_cLabelUsbCable.put_BackColor(0x0000FF00);
}


void CCableChangeDlg::OnBnClickedButtonPortCountReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	m_EditPortCableCount = 0;//m_EditPortCableMax;
	UpdateData(FALSE);
	m_cLabelPortCable.put_BackColor(0x0000FF00);

}


void CCableChangeDlg::OnBnClickedButtonCheckerPinCountReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	m_EditCheckerCount = 0;//m_EditCheckerMax;
	UpdateData(FALSE);
	m_cLabelCM4Pin.put_BackColor(0x0000FF00);


}
void CCableChangeDlg::OnBnClickedButtonCheckerPinCountReset2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	m_EditCheckerCount = 0;
	UpdateData(FALSE);
	m_cLabelCM5Pin.put_BackColor(0x0000FF00);
}


void CCableChangeDlg::OnBnClickedButtonMicCountReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	m_EditMicCableCount = 0;//m_EditMicCableMax;
	UpdateData(FALSE);
	m_cLabelMICCable.put_BackColor(0x0000FF00);
}


void CCableChangeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();

	CFileControl lFile;


	gCableManager.nUSB_CableCountMax = m_EditUsbCableMax ;
	gCableManager.nUSB_CableCount = m_EditUsbCableCount ;
	gCableManager.nPort_CableCountMax = m_EditPortCableMax ;
	gCableManager.nPort_CableCount = m_EditPortCableCount;
	gCableManager.nCM4CheckerCountMax = m_EditCheckerMax ;
	gCableManager.nCM4CheckerCount = m_EditCheckerCount;
	gCableManager.nCM5CheckerCountMax = m_EditCheckerMaxCM5 ;
	gCableManager.nCM5CheckerCount = m_EditCheckerCountCM5;
	gCableManager.nMIC_CableCountMax = m_EditMicCableMax ;
	gCableManager.nMIC_CableCount = m_EditMicCableCount;
	lFile.SaveCableCountData();


	CDialogEx::OnOK();
}


