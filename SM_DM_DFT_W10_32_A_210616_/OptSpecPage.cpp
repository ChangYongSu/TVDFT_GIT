// OptSpecPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptSpecPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "Global.h"
#include "Step.h"
#include <TVCommCtrl.h>

extern _InputSource	InputDefined[];

extern  CTVCommCtrl	TVCommCtrl;

/////////////////////////////////////////////////////////////////////////////
// COptSpecPage property page

IMPLEMENT_DYNCREATE(COptSpecPage, CPropertyPage)

COptSpecPage::COptSpecPage() : CPropertyPage(COptSpecPage::IDD)
{
	//{{AFX_DATA_INIT(COptSpecPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COptSpecPage::~COptSpecPage()
{
}

void COptSpecPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptSpecPage)
	//	DDX_Control(pDX, IDC_AUDIO_SPEC_GRID, m_ctrlVerSpecGrid);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_AUDIO_SPEC_LIST, m_ctrlAudioVerSpecList);
	DDX_Control(pDX, IDC_EDIT_OPT_VERSION, m_cEditOptVersion);
}


BEGIN_MESSAGE_MAP(COptSpecPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptSpecPage)
	ON_BN_CLICKED(IDC_BTN_OPT_APPLY, OnBtnOptApply)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_AUDIO_SPEC_LIST, &COptSpecPage::OnNMDblclkListAudioSpecList)
	ON_NOTIFY(NM_CLICK, IDC_LIST_AUDIO_SPEC_LIST, &COptSpecPage::OnNMClickListAudioSpecList)
	ON_EN_CHANGE(IDC_EDIT_OPT_VERSION, &COptSpecPage::OnEnChangeEditOptVersion)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_VERSION, &COptSpecPage::OnBnClickedButtonUpdateVersion)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptSpecPage message handlers

void COptSpecPage::InitVerSpecGrid()
{
	CString sTmp;
	char *sHeader[] = {"BE Ver.", "FE Ver.", "Micom Ver.", "DSP Ver.", "Sub DSP Ver.", "EQ Ver.", "MEQ Ver.", "Touch Ver.", "Demo Ver.", "DJ Sound Ver.", "WirelessTx Ver.", "WirelessRx Ver.","BT Version", "HDMI Version","Checksum"};
	m_ctrlAudioVerSpecList.DeleteAllItems();	
//	m_ctrlAudioVerSpecList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_ctrlAudioVerSpecList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);// 리스트 컨트롤 초기화: 열 추가
	m_ctrlAudioVerSpecList.InsertColumn(0, "ITEM", LVCFMT_CENTER, 110);
	m_ctrlAudioVerSpecList.InsertColumn(1, "VERSION", LVCFMT_CENTER, 260);

	for(int nRow = 0; nRow < 15 ; nRow++)
	{
		m_ctrlAudioVerSpecList.InsertItem(nRow, sHeader[nRow]);
	}
	
	m_ctrlAudioVerSpecList.Invalidate();
}


void COptSpecPage::UpdateVerSpecGrid()
{	
	m_ctrlAudioVerSpecList.SetItem(0, 1, LVIF_TEXT,CurrentSet->sBE_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(1, 1, LVIF_TEXT,CurrentSet->sFE_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(2, 1, LVIF_TEXT,CurrentSet->sMicom_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(3, 1, LVIF_TEXT,CurrentSet->sDSP_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(4, 1, LVIF_TEXT,CurrentSet->sSubDSP_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(5, 1, LVIF_TEXT,CurrentSet->sEQ_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(6, 1, LVIF_TEXT,CurrentSet->sMEQ_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(7, 1, LVIF_TEXT,CurrentSet->sTouch_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(8, 1, LVIF_TEXT,CurrentSet->sDemo_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(9, 1, LVIF_TEXT,CurrentSet->sDJSound_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(10, 1,LVIF_TEXT, CurrentSet->sWirelessTx_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(11, 1,LVIF_TEXT, CurrentSet->sWirelessRx_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(12, 1, LVIF_TEXT, CurrentSet->sBT_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(13, 1, LVIF_TEXT, CurrentSet->sHDMI_Version, NULL, NULL, NULL, NULL);
	m_ctrlAudioVerSpecList.SetItem(14, 1, LVIF_TEXT, CurrentSet->sChecksum, NULL, NULL, NULL, NULL);
	for (int i = 0; i < 15; i++)
	{
		if (CurrentSet->bVerChecked[i] == 1)
		{		
			m_ctrlAudioVerSpecList.SetCheck(i, 1);
		}
		else
		{
			m_ctrlAudioVerSpecList.SetCheck(i, 0);
		}
	}
	m_ctrlAudioVerSpecList.Invalidate();
}


BOOL COptSpecPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	GetDlgItem(IDC_EDIT_OPT_VERSION)->SetWindowPos(NULL, 2500, 20, 150, 23, SWP_ASYNCWINDOWPOS);
	
	// TODO: Add extra initialization here
	InitVerSpecGrid();
	UpdateVerSpecGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(COptSpecPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(COptSpecPage)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()



void COptSpecPage::OnBtnOptApply() 
{
	// TODO: Add your control notification handler code here

	CurrentSet->sBE_Version			= m_ctrlAudioVerSpecList.GetItemText(0, 1);  /// m_ctrlVerSpecGrid.GetTextMatrix(1, 1);
	CurrentSet->sFE_Version			= m_ctrlAudioVerSpecList.GetItemText(1, 1);
	CurrentSet->sMicom_Version		= m_ctrlAudioVerSpecList.GetItemText(2, 1);
	CurrentSet->sDSP_Version		= m_ctrlAudioVerSpecList.GetItemText(3, 1);
	CurrentSet->sSubDSP_Version		= m_ctrlAudioVerSpecList.GetItemText(4, 1);
	CurrentSet->sEQ_Version			= m_ctrlAudioVerSpecList.GetItemText(5, 1);
	CurrentSet->sMEQ_Version		= m_ctrlAudioVerSpecList.GetItemText(6, 1);
	CurrentSet->sTouch_Version		= m_ctrlAudioVerSpecList.GetItemText(7, 1);
	CurrentSet->sDemo_Version		= m_ctrlAudioVerSpecList.GetItemText(8, 1);
	CurrentSet->sDJSound_Version	= m_ctrlAudioVerSpecList.GetItemText(9,  1);
	CurrentSet->sWirelessTx_Version	= m_ctrlAudioVerSpecList.GetItemText(10, 1);
	CurrentSet->sWirelessRx_Version	= m_ctrlAudioVerSpecList.GetItemText(11, 1);
	CurrentSet->sBT_Version = m_ctrlAudioVerSpecList.GetItemText(12, 1);
	CurrentSet->sHDMI_Version = m_ctrlAudioVerSpecList.GetItemText(13, 1);
	CurrentSet->sChecksum = m_ctrlAudioVerSpecList.GetItemText(14, 1);
	
	for (int i = 0; i < 15; i++)
	{
		if (m_ctrlAudioVerSpecList.GetCheck(i) == 1)
		{
			CurrentSet->bVerChecked[i] = 1;
		}
		else
		{
			CurrentSet->bVerChecked[i] = 0;
		}
	}
	
}

void COptSpecPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	OnBtnOptApply();
	
	CPropertyPage::OnOK();
}

//+add kwmoon 080904
void COptSpecPage::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	//+add kwmoon 080910
	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sTmp = _T("");
		sTmp.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sTmp); return;
	}

	OnBtnOptApply();

	SaveModelIniFile(CurrentSet->sModelIni);
}

BOOL COptSpecPage::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			if (m_EditMode == 1)
			{
				if (pMsg->wParam != VK_SPACE)
				{
					m_EditMode = 0;
					//m_SelectItem = nItem;
					//m_SelectShell = nShell;
					CString strtemp;
					GetDlgItemText(IDC_EDIT_OPT_VERSION, strtemp);
					m_ctrlAudioVerSpecList.SetItemText(m_SelectItem, m_SelectShell, strtemp);
					GetDlgItem(IDC_EDIT_OPT_VERSION)->SetWindowPos(NULL, 2500, 20, 150, 23, SWP_ASYNCWINDOWPOS);
					return TRUE;
				}

			}
			return TRUE;
		}
           
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}


void COptSpecPage::OnNMDblclkListAudioSpecList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;


	m_EditMode = 0;
	if (nShell == 1)//m_cEditPinName
	{

		if (nItem < m_ctrlAudioVerSpecList.GetItemCount())
		{

			RECT lMainRect, lRect;
			this->GetWindowRect(&lMainRect);

			m_ctrlAudioVerSpecList.GetWindowRect(&lRect);

			int x = lRect.left - lMainRect.left;
			int y = lRect.top - lMainRect.top;

			//m_cEditPinName.SetWindowPos(this, lRect.left+70, lRect.top+nItem*30, 70, 30, SWP_ASYNCWINDOWPOS);
			GetDlgItem(IDC_EDIT_OPT_VERSION)->SetWindowPos(NULL, x + 120, y + nItem * 17 + 20, 250, 20, SWP_ASYNCWINDOWPOS);
			//	m_cEditPinName.SetRect(&lRect);
			SetDlgItemText(IDC_EDIT_OPT_VERSION, m_ctrlAudioVerSpecList.GetItemText(nItem, 1));//CurrentSet->sBE_Version			= m_ctrlAudioVerSpecList.GetItemText(0, 1);  

			m_cEditOptVersion.SetFocus();
			//m_cEditOptVersion.SetSel(0);
			m_SelectItem = nItem;
			m_SelectShell = nShell;

			m_EditMode = 1;
		}

	}
	
	*pResult = 0;
}


void COptSpecPage::OnNMClickListAudioSpecList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	if (m_EditMode == 1)
	{
		if ((m_SelectItem != nItem) || (m_SelectShell != nShell))
		{
			CString strtemp;
			GetDlgItemText(IDC_EDIT_OPT_VERSION, strtemp);
			if (strtemp.GetLength() > 0)
			{
				m_ctrlAudioVerSpecList.SetItemText(m_SelectItem, m_SelectShell, strtemp);
			}
			GetDlgItem(IDC_EDIT_OPT_VERSION)->SetWindowPos(NULL, 2500, 20, 250, 20, SWP_ASYNCWINDOWPOS);
			m_EditMode = 0;
		}
	}
}


void COptSpecPage::OnEnChangeEditOptVersion()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_EditMode == 1)
	{
		CString strtemp;
		GetDlgItemText(IDC_EDIT_OPT_VERSION, strtemp);

		m_ctrlAudioVerSpecList.SetItemText(m_SelectItem, m_SelectShell, strtemp);

	
	}
}


void COptSpecPage::OnBnClickedButtonUpdateVersion()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int lCheckFlag = 0;
	CString sTempVersion;

	TVCommCtrl.m_bCheckReadVersion = 1;
	for (int i = 0; i < 15; i++)
	{
		lCheckFlag = m_ctrlAudioVerSpecList.GetCheck(i);
		if (lCheckFlag == 1)
		{
			TVCommCtrl.Firmware_VersionRead(i+1, sTempVersion);
			m_ctrlAudioVerSpecList.SetItemText(i, 1, sTempVersion);
		}
		else
		{
			m_ctrlAudioVerSpecList.SetItemText(i, 1, "");
		}
	}
	
	TVCommCtrl.m_bCheckReadVersion = 0;
}
