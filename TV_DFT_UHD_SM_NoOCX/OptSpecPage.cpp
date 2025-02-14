// OptSpecPage.cpp : implementation file
//

#include "stdafx.h"
#include "datsys.h"
#include "OptSpecPage.h"
#include "Global.h"
#include "Step.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



extern CDATsysView*  g_pView;

/////////////////////////////////////////////////////////////////////////////
// COptSpecPage property page

IMPLEMENT_DYNCREATE(COptSpecPage, CPropertyPage)

COptSpecPage::COptSpecPage() : CPropertyPage(COptSpecPage::IDD)
, m_bToolOptionWriteFlag(FALSE)
, m_szBoardOption(_T(""))
, m_szCommercialBoardOption(_T(""))
{
	//{{AFX_DATA_INIT(COptSpecPage)
	m_szToolOption1 = _T("");
	m_szToolOption2 = _T("");
	m_szToolOption3 = _T("");
	m_szToolOption4 = _T("");
	m_szToolOption5 = _T("");
	m_szToolOption6 = _T("");
	m_szToolOption7 = _T("");
	m_szCommercialOption1 = _T("");
	m_szToolOption8 = _T("");
	m_szAreaOption1 = _T("");
	m_szCountryGroup = _T("");
	m_szCountry = _T("");
	m_szCpuVersion = _T("");
	m_szMicomVersion = _T("");
	m_szMntVersion = _T("");
	m_nToolOptionMethod = -1;
	m_szToolCRC = _T("");
	//}}AFX_DATA_INIT
}

COptSpecPage::~COptSpecPage()
{
}

void COptSpecPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptSpecPage)
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION1, m_szToolOption1);
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION2, m_szToolOption2);
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION3, m_szToolOption3);
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION4, m_szToolOption4);
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION5, m_szToolOption5);
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION6, m_szToolOption6);
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION7, m_szToolOption7);
	DDX_Text(pDX, IDC_EDIT_COMMERCIAL_OPTION, m_szCommercialOption1);
	DDX_Text(pDX, IDC_EDIT_TOOL_OPTION8, m_szToolOption8);
	DDX_Text(pDX, IDC_EDIT_AREA_OPTION1, m_szAreaOption1);
	DDX_Text(pDX, IDC_EDIT_COUNTRYGROUP, m_szCountryGroup);
	DDX_Text(pDX, IDC_EDIT_COUNTRY, m_szCountry);
	DDX_Text(pDX, IDC_EDIT_CPU_VERSION, m_szCpuVersion);
	DDX_Text(pDX, IDC_EDIT_MICOM_VERSION, m_szMicomVersion);
	DDX_Text(pDX, IDC_EDIT_MNT_VERSION, m_szMntVersion);
	DDX_Radio(pDX, IDC_RDO_TOOL_OPTION1, m_nToolOptionMethod);
	DDX_Control(pDX, IDC_CMB_TOOLCHEK_DELAY, m_ctrlOptionCheckDelay);
	DDX_Text(pDX, IDC_EDIT_TOOL_CRC, m_szToolCRC);
	//DDX_Control(pDX, IDC_SPEC_GRID, m_ctrlSpecGrid);
	//DDX_Control(pDX, IDC_SPEC_GRID2, m_ctrlSpecGrid2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_SPEC_INDEX_OPTION, m_CtrlListSpecIndexOption);
	DDX_Control(pDX, IDC_LIST_SPEC_GRID, m_CtrlListSpecGrid);
	DDX_Control(pDX, IDC_EDIT_OP_EPI_CFG_SW, m_CEdit_OP_EPI_CFG_SW);
	DDX_Check(pDX, IDC_CHECK_OPTION_WRITE, m_bToolOptionWriteFlag);
	DDX_Text(pDX, IDC_EDIT_BOARD_OPTION, m_szBoardOption);
	DDX_Text(pDX, IDC_EDIT_COMMERCIAL_BOARD_OPTION, m_szCommercialBoardOption);
}


BEGIN_MESSAGE_MAP(COptSpecPage, CPropertyPage)
	//{{AFX_MSG_MAP(COptSpecPage)
	ON_BN_CLICKED(IDC_BTN_OPT_APPLY, OnBtnOptApply)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_DRM_KEY, &COptSpecPage::OnEnChangeEditDrmKey)
//	ON_BN_CLICKED(IDC_BTN_CHECK_DRM, OnBnClickedBtnCheckDrm)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_DRM_KEY, &COptSpecPage::OnBnClickedButtonCheckDrmKey)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPEC_INDEX_OPTION, &COptSpecPage::OnNMClickListSpecIndexOption)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SPEC_INDEX_OPTION, &COptSpecPage::OnNMCustomdrawListSpecIndexOption)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPEC_GRID, &COptSpecPage::OnNMClickListSpecGrid)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SPEC_GRID, &COptSpecPage::OnNMCustomdrawListSpecGrid)
	ON_BN_CLICKED(IDC_BUTTON_OP_EPI_CFG_SW, &COptSpecPage::OnBnClickedButtonOpEpiCfgSw)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SPEC_GRID, &COptSpecPage::OnLvnItemchangedListSpecGrid)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptSpecPage message handlers


BOOL COptSpecPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_szToolOption1 = CurrentSet->sToolOption1;
	m_szToolOption2 = CurrentSet->sToolOption2;
	m_szToolOption3 = CurrentSet->sToolOption3;
	m_szToolOption4 = CurrentSet->sToolOption4;
	m_szToolOption5 = CurrentSet->sToolOption5;
	m_szToolOption6 = CurrentSet->sToolOption6;
	m_szToolOption7 = CurrentSet->sToolOption7;
	m_szCommercialOption1 = CurrentSet->sCommercialOption1;
	m_szToolOption8 = CurrentSet->sToolOption8;
	m_szBoardOption = CurrentSet->sBoardOption ;
	m_szCommercialBoardOption = CurrentSet->sCommercialBoardOption ;
	m_szToolCRC = CurrentSet->sToolCRC;
	m_bToolOptionWriteFlag = CurrentSet->bToolOptionWriteFlag;



	m_szAreaOption1  = CurrentSet->sAreaOption1;
	m_szCountryGroup = CurrentSet->sCountryGroup;
	m_szCountry      = CurrentSet->sCountry;
	m_szCpuVersion   = CurrentSet->sCPUVersion;
	m_szMicomVersion = CurrentSet->sMicomVersion;
	m_szMntVersion   = CurrentSet->sMNTVersion;

	m_nToolOptionMethod = CurrentSet->nToolOptionSetMethod;
	m_ctrlOptionCheckDelay.SetCurSel(CurrentSet->nToolOptionCheck_Delay);
	InitSpecGrid();
	InitSpecGrid2();
	UpdateData(FALSE);
	OnBnClickedButtonOpEpiCfgSw();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(COptSpecPage, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(COptSpecPage)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(COptSpecPage, IDC_SPEC_GRID, 28, COptSpecPage::ChangeEditSpecGrid, VTS_NONE)
END_EVENTSINK_MAP()



void COptSpecPage::OnBtnOptApply() 
{
	UpdateData(TRUE);

	CurrentSet->sToolOption1 = m_szToolOption1;
	CurrentSet->sToolOption2 = m_szToolOption2;
	CurrentSet->sToolOption3 = m_szToolOption3;
	CurrentSet->sToolOption4 = m_szToolOption4;
	CurrentSet->sToolOption5 = m_szToolOption5;
	CurrentSet->sToolOption6 = m_szToolOption6;
	CurrentSet->sToolOption7 = m_szToolOption7;
	CurrentSet->sCommercialOption1 = m_szCommercialOption1;
	CurrentSet->sToolOption8 = m_szToolOption8;
	CurrentSet->sBoardOption = m_szBoardOption;
	CurrentSet->sCommercialBoardOption=  m_szCommercialBoardOption;

	CurrentSet->sToolCRC = m_szToolCRC;
	CurrentSet->bToolOptionWriteFlag = m_bToolOptionWriteFlag;

	CurrentSet->sAreaOption1  = m_szAreaOption1;
	CurrentSet->sCountryGroup = m_szCountryGroup;
	CurrentSet->sCountry	  = m_szCountry;
	CurrentSet->sCPUVersion	  = m_szCpuVersion;
	CurrentSet->sMicomVersion = m_szMicomVersion;
	CurrentSet->sMNTVersion	  = m_szMntVersion;

#if 1
	CurrentSet->sEDID_HDMI1		= m_CtrlListSpecGrid.GetItemText(0, 1);
	CurrentSet->sEDID_HDMI2		= m_CtrlListSpecGrid.GetItemText(1, 1);
	CurrentSet->sEDID_HDMI3		= m_CtrlListSpecGrid.GetItemText(2, 1);
	CurrentSet->sEDID_HDMI4		= m_CtrlListSpecGrid.GetItemText(3, 1);
	CurrentSet->sEDID_HDMI5		= m_CtrlListSpecGrid.GetItemText(4, 1);
	CurrentSet->sEDID_DP		= m_CtrlListSpecGrid.GetItemText(5, 1);
	CurrentSet->sEDID_DP2		= m_CtrlListSpecGrid.GetItemText(6, 1);
	CurrentSet->sEDID_DVI		= m_CtrlListSpecGrid.GetItemText(7, 1);
	CurrentSet->sEDID_PC		= m_CtrlListSpecGrid.GetItemText(8, 1);
	CurrentSet->sEDID_RGB		= m_CtrlListSpecGrid.GetItemText(9, 1);
	CurrentSet->sCI				= m_CtrlListSpecGrid.GetItemText(10, 1); //0x100		//CurrentSet->sCNTV			= m_ctrlSpecGrid.GetTextMatrix(11, 1); //0x200		
	CurrentSet->sDVBS			= m_CtrlListSpecGrid.GetItemText(11, 1);
	CurrentSet->sDVBS2			= m_CtrlListSpecGrid.GetItemText(12, 1);
	CurrentSet->sESN			= m_CtrlListSpecGrid.GetItemText(13, 1); 	//0x04	
	CurrentSet->sDTCP_RX		= m_CtrlListSpecGrid.GetItemText(14, 1); 	//0x40       
	CurrentSet->sHDCP_RX		= m_CtrlListSpecGrid.GetItemText(15, 1); 	//0x20       
	CurrentSet->sHDCP_RX_14		= m_CtrlListSpecGrid.GetItemText(16, 1); 	//0x10
	CurrentSet->sHDCP_TX		= m_CtrlListSpecGrid.GetItemText(17, 1);	//CurrentSet->sWASU			= m_ctrlSpecGrid.GetTextMatrix(19, 1);
	CurrentSet->sADC			= m_CtrlListSpecGrid.GetItemText(18, 1);
	CurrentSet->sWIDEVINE		= m_CtrlListSpecGrid.GetItemText(19, 1); //0x08	        
	CurrentSet->sMARINKEY		= m_CtrlListSpecGrid.GetItemText(20, 1); //0x80	
	CurrentSet->sBUILT_WIFI		= m_CtrlListSpecGrid.GetItemText(21, 1);
	CurrentSet->sJAPAN_LNB		= m_CtrlListSpecGrid.GetItemText(22, 1);
	CurrentSet->sMAC_ADDRESS	= m_CtrlListSpecGrid.GetItemText(23, 1); 	//0x02
	CurrentSet->sASTC30_Key		= m_CtrlListSpecGrid.GetItemText(24, 1);//0x800
	CurrentSet->sMfi_Key		= m_CtrlListSpecGrid.GetItemText(25, 1);//0x1000
	CurrentSet->sCI_CEP_Key		= m_CtrlListSpecGrid.GetItemText(26, 1);//0x2000
	CurrentSet->sCA				= m_CtrlListSpecGrid.GetItemText(27, 1);
	CurrentSet->sMODEL_NAME		= m_CtrlListSpecGrid.GetItemText(28, 1);
	CurrentSet->sSERIAL_NO		= m_CtrlListSpecGrid.GetItemText(29, 1);
																  
#else
	CurrentSet->sEDID_HDMI1		= m_ctrlSpecGrid.GetTextMatrix(0, 1);  	
	CurrentSet->sEDID_HDMI2		= m_ctrlSpecGrid.GetTextMatrix(1, 1);  	
	CurrentSet->sEDID_HDMI3		= m_ctrlSpecGrid.GetTextMatrix(2, 1);  	
	CurrentSet->sEDID_HDMI4		= m_ctrlSpecGrid.GetTextMatrix(3, 1);  	
	CurrentSet->sEDID_HDMI5		= m_ctrlSpecGrid.GetTextMatrix(4, 1);  	
	CurrentSet->sEDID_DP		= m_ctrlSpecGrid.GetTextMatrix(5, 1);  	        
	CurrentSet->sEDID_DP2		= m_ctrlSpecGrid.GetTextMatrix(6, 1);  	        
	CurrentSet->sEDID_DVI		= m_ctrlSpecGrid.GetTextMatrix(7, 1);  	        
	CurrentSet->sEDID_PC		= m_ctrlSpecGrid.GetTextMatrix(8, 1);  	        
	CurrentSet->sEDID_RGB		= m_ctrlSpecGrid.GetTextMatrix(9, 1);  	        
	
	CurrentSet->sCI				= m_ctrlSpecGrid.GetTextMatrix(10, 1); //0x100	
	CurrentSet->sCNTV			= m_ctrlSpecGrid.GetTextMatrix(11, 1); //0x200		
	CurrentSet->sDVBS			= m_ctrlSpecGrid.GetTextMatrix(12, 1); 		
	CurrentSet->sDVBS2			= m_ctrlSpecGrid.GetTextMatrix(13, 1); 		
	CurrentSet->sESN			= m_ctrlSpecGrid.GetTextMatrix(14, 1); 	//0x04	
	CurrentSet->sDTCP_RX		= m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x40       
	CurrentSet->sHDCP_RX		= m_ctrlSpecGrid.GetTextMatrix(16, 1); 	//0x20       
	CurrentSet->sHDCP_RX_14		= m_ctrlSpecGrid.GetTextMatrix(17, 1); 	//0x10
	CurrentSet->sHDCP_TX		= m_ctrlSpecGrid.GetTextMatrix(18, 1); 	        
	CurrentSet->sWASU			= m_ctrlSpecGrid.GetTextMatrix(19, 1); 		
	CurrentSet->sADC			= m_ctrlSpecGrid.GetTextMatrix(20, 1); 		
	CurrentSet->sWIDEVINE		= m_ctrlSpecGrid.GetTextMatrix(21, 1); //0x08	        
	CurrentSet->sMARINKEY		= m_ctrlSpecGrid.GetTextMatrix(22, 1); //0x80	        
	CurrentSet->sBUILT_WIFI		= m_ctrlSpecGrid.GetTextMatrix(23, 1); 	
	CurrentSet->sJAPAN_LNB		= m_ctrlSpecGrid.GetTextMatrix(24, 1); 	
	CurrentSet->sMAC_ADDRESS	= m_ctrlSpecGrid.GetTextMatrix(25, 1); 	//0x02
	CurrentSet->sASTC30_Key		= m_ctrlSpecGrid.GetTextMatrix(26, 1);//0x800
	CurrentSet->sMfi_Key		= m_ctrlSpecGrid.GetTextMatrix(27, 1);//0x1000
	CurrentSet->sCI_CEP_Key		= m_ctrlSpecGrid.GetTextMatrix(28, 1);//0x2000
	CurrentSet->sCA				= m_ctrlSpecGrid.GetTextMatrix(29, 1); 		
	CurrentSet->sMODEL_NAME		= m_ctrlSpecGrid.GetTextMatrix(30, 1); 	
	CurrentSet->sSERIAL_NO		= m_ctrlSpecGrid.GetTextMatrix(31, 1);

#endif
	//0x01 = 1
	//0x40, = 0

	CurrentSet->sJACK_AV1		= m_CtrlListSpecIndexOption.GetItemText(0, 1);  	
	CurrentSet->sJACK_AV2		= m_CtrlListSpecIndexOption.GetItemText(1, 1);  	
	CurrentSet->sJACK_SCART		= m_CtrlListSpecIndexOption.GetItemText(2, 1);  	
	CurrentSet->sJACK_COMP1		= m_CtrlListSpecIndexOption.GetItemText(3, 1);  	
	CurrentSet->sJACK_COMP2		= m_CtrlListSpecIndexOption.GetItemText(4, 1);  	
	CurrentSet->sJACK_USB1		= m_CtrlListSpecIndexOption.GetItemText(5, 1);  	        
	CurrentSet->sJACK_USB2		= m_CtrlListSpecIndexOption.GetItemText(6, 1);  	        
	CurrentSet->sJACK_USB3		= m_CtrlListSpecIndexOption.GetItemText(7, 1);  	        
	CurrentSet->sJACK_USB4		= m_CtrlListSpecIndexOption.GetItemText(8, 1);  	        
	CurrentSet->sJACK_HEAD_PHONE= m_CtrlListSpecIndexOption.GetItemText(9, 1);  	        
	CurrentSet->sJACK_HDMI1		= m_CtrlListSpecIndexOption.GetItemText(10, 1); 		
	CurrentSet->sJACK_HDMI2		= m_CtrlListSpecIndexOption.GetItemText(11, 1); 		
	CurrentSet->sJACK_HDMI3		= m_CtrlListSpecIndexOption.GetItemText(12, 1); 		
	CurrentSet->sJACK_HDMI4		= m_CtrlListSpecIndexOption.GetItemText(13, 1); 		
	CurrentSet->sJACK_RGB		= m_CtrlListSpecIndexOption.GetItemText(14, 1); 		
	CurrentSet->sJACK_DVI		= m_CtrlListSpecIndexOption.GetItemText(15, 1); 	        
	CurrentSet->sJACK_DP		= m_CtrlListSpecIndexOption.GetItemText(16, 1); 	        
	CurrentSet->sJACK_SMHL		= m_CtrlListSpecIndexOption.GetItemText(17, 1); 	
#if 0
	CurrentSet->sJACK_AV1		= m_ctrlSpecGrid2.GetTextMatrix(0, 1);  	
	CurrentSet->sJACK_AV2		= m_ctrlSpecGrid2.GetTextMatrix(1, 1);  	
	CurrentSet->sJACK_SCART		= m_ctrlSpecGrid2.GetTextMatrix(2, 1);  	
	CurrentSet->sJACK_COMP1		= m_ctrlSpecGrid2.GetTextMatrix(3, 1);  	
	CurrentSet->sJACK_COMP2		= m_ctrlSpecGrid2.GetTextMatrix(4, 1);  	
	CurrentSet->sJACK_USB1		= m_ctrlSpecGrid2.GetTextMatrix(5, 1);  	        
	CurrentSet->sJACK_USB2		= m_ctrlSpecGrid2.GetTextMatrix(6, 1);  	        
	CurrentSet->sJACK_USB3		= m_ctrlSpecGrid2.GetTextMatrix(7, 1);  	        
	CurrentSet->sJACK_USB4		= m_ctrlSpecGrid2.GetTextMatrix(8, 1);  	        
	CurrentSet->sJACK_HEAD_PHONE= m_ctrlSpecGrid2.GetTextMatrix(9, 1);  	        
	CurrentSet->sJACK_HDMI1		= m_ctrlSpecGrid2.GetTextMatrix(10, 1); 		
	CurrentSet->sJACK_HDMI2		= m_ctrlSpecGrid2.GetTextMatrix(11, 1); 		
	CurrentSet->sJACK_HDMI3		= m_ctrlSpecGrid2.GetTextMatrix(12, 1); 		
	CurrentSet->sJACK_HDMI4		= m_ctrlSpecGrid2.GetTextMatrix(13, 1); 		
	CurrentSet->sJACK_RGB		= m_ctrlSpecGrid2.GetTextMatrix(14, 1); 		
	CurrentSet->sJACK_DVI		= m_ctrlSpecGrid2.GetTextMatrix(15, 1); 	        
	CurrentSet->sJACK_DP		= m_ctrlSpecGrid2.GetTextMatrix(16, 1); 	        
	CurrentSet->sJACK_SMHL		= m_ctrlSpecGrid2.GetTextMatrix(17, 1); 	
#endif
	g_pView->UpdateToolOptionInfo(FALSE);

	CurrentSet->nToolOptionCheck_Delay = m_ctrlOptionCheckDelay.GetCurSel();
	CurrentSet->bDrmKey = ChekDrmKey();

	CurrentSet->nToolOptionSetMethod = m_nToolOptionMethod;
}

void COptSpecPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!CheckToolCrc()){return;}

	OnBtnOptApply();
	
	CPropertyPage::OnOK();
}

//+add kwmoon 080904
void COptSpecPage::OnBtnSave() 
{
	CString sMsg = _T("");

	if(!CheckToolCrc()){return;}

	if((CurrentSet->nAuthority != AUTHORITY_DEV) && (CurrentSet->nAuthority != AUTHORITY_ADMIN))
	{
		CString sMsg = _T("");
		sMsg.LoadString(ERROR_ADMIN_LOGIN);
		AfxMessageBox(sMsg); return;
	}

	OnBtnOptApply();
	if (OpenEPI_ConfigFile(CurrentSet->sEpiCfgIni))
	{
		//CurrentSet->sDipStatus
	}
	else
	{
		CurrentSet->sDipStatus = "";
	}
	m_CEdit_OP_EPI_CFG_SW.SetWindowText(CurrentSet->sDipStatus);

	if(FileExists(CurrentSet->sModelIni))
	{
		SaveModelIniFile(CurrentSet->sModelIni);
	}
	else
	{
		CreateModelIniFile(CurrentSet->sModelIni);
		sMsg.Format("File is created.\n[%s]",CurrentSet->sModelIni);
		AfxMessageBox(sMsg);
	}
}

BOOL COptSpecPage::CheckToolCrc()
{
	vector<unsigned int> tool(15,0);
	const unsigned int ID_32 = 0x8005;   //CRC Sum의 key값, 미리 define되는 부분이며 많이 사용되고 있는 0x8005로 사용
	const unsigned short ID_16 = 0x8005;   //CRC Sum의 key값, 미리 define되는 부분이며 많이 사용되고 있는 0x8005로 사용

	unsigned int CRC_32 = 0;       //CRC Sum의 초기값은 0으로 둔다
	unsigned int nCurCRC_32 = 0;       
	unsigned short CRC_16 = 0;       //CRC Sum의 초기값은 0으로 둔다
	unsigned short nCurCRC_16 = 0;       
	BOOL bCrcCheck32 = FALSE;
	BOOL bCrcCheck16 = FALSE;
	unsigned int val32;
	unsigned short val16;
	CString sTemp;
	CString sBuf;
	//unsigned long long ltemp = 0x0000123412341234;
	//sTemp.Format("%016x", ltemp);
	//sTemp.Insert(4, ' ');
	//sTemp.Insert(9, ' ');
	//sTemp.Insert(14, ' ');



	UpdateData(TRUE);
#if 1


	if (m_szToolOption1 != "") {
		tool[1] = atoi(m_szToolOption1);
	}
	else {
		tool[1] = 0;
	}

	if (m_szToolOption2 != "") {
		tool[2] = atoi(m_szToolOption2);
	}
	else {
		tool[2] = 0;
	}

	if (m_szToolOption3 != "") {
		tool[3] = atoi(m_szToolOption3);
	}
	else {
		tool[3] = 0;
	}

	if (m_szToolOption4 != "") {
		tool[4] = atoi(m_szToolOption4);
	}
	else {
		tool[4] = 0;
	}

	if (m_szToolOption5 != "") {
		tool[5] = atoi(m_szToolOption5);
	}
	else {
		tool[5] = 0;
	}


	if (m_szToolOption6 != "") {
		tool[6] = atoi(m_szToolOption6);
	}
	else {
		tool[6] = 0;
	}

	if (m_szToolOption7 != "") {
		tool[7] = atoi(m_szToolOption7);
	}
	else {
		tool[7] = 0;
	}

	if (m_szCommercialOption1 != "") {
		tool[8] = atoi(m_szCommercialOption1);
	}
	else {
		tool[8] = 0;
	}

	if (m_szToolOption8 != "") {
		tool[9] = atoi(m_szToolOption8);
	}
	else {
		tool[9] = 0;
	}

	//m_szBoardOption = CurrentSet->sBoardOption;
	////m_szCommercialBoardOption = CurrentSet->sCommercialBoardOption;

	unsigned long long Hex64bit;
	if (m_szBoardOption != "") {
		Hex64bit = hex64Str2DecNum(m_szBoardOption.GetBuffer());// atoi(m_szBoardOption);
		tool[10] = (Hex64bit& 0xFFFFFFFF00000000) >> 32 ;
		tool[11] = Hex64bit & 0xFFFFFFFF;
	}
	else {
		tool[10] = 0;
		tool[11] = 0;
	}


	if (m_szCommercialBoardOption != "") {
		Hex64bit = hex64Str2DecNum(m_szCommercialBoardOption.GetBuffer());
		tool[12] = (Hex64bit >> 32) & 0xFFFFFFFF;
		tool[13] = Hex64bit & 0xFFFFFFFF;

	}
	else {
		tool[12] = 0;
		tool[13] = 0;
	}
	int CRC_Count = 0;
	if (m_szBoardOption == "")
	{
		CRC_Count = 9;
	}
	else if (m_szCommercialBoardOption == "")
	{
		CRC_Count = 11;
	}
	else
	{
		CRC_Count = 13;
	}
		
	for (int i = 1; i <= CRC_Count; i++)
	{
		if (tool[i] > 0xFFFF) { bCrcCheck32 = TRUE; }
	}
	if (!bCrcCheck32) {
		if (m_szToolCRC != "") {
			bCrcCheck16 = TRUE;
		}
		else {
			return TRUE;
		}
	}

	if (m_szToolCRC != "") {
		if (bCrcCheck32) {
			nCurCRC_32 = atoi(m_szToolCRC);
		}
		else {
			nCurCRC_16 = atoi(m_szToolCRC);
		}
	}


	if (bCrcCheck32) {
		for (int t = 1; t <= CRC_Count; t++) {      // ToolOption 1~9번까지 모든 data들을 확인한다.
			val32 = tool[t];          //해당 ToolOption값을 우선적으로 val값에 저장
			vector<int> bitarray = makeBitArray(val32);         //해당 val를 bitarray로 바꿔주는 부분 vector<int> 는 int형 vector (array로 생각해도 무방)
			for (int i = 0; i < 32; i++) {				 //32bit에 해당되는 모든 bit를 한 bit씩 순회 가장 큰 bit부터 확인
				if ((bitarray[i]) ^ (CRC_32 >> 15)) {		 //현재 bit에 해당되는 부분과 CRC의 가장 큰 숫자 즉 (CRC>>15를 할시 2진법상 가장큰수)의 xor이 1일시 실행
					CRC_32 = ((CRC_32 << 1) & 0xffff) ^ ID_32;	// (CRC<<1은 CRC*2와 동일) CRC 오른쪽으로 1bit shift값과 ID값으 xor값을 CRC에 저장 //((CRC << 1) & 0xffff)이유는 CRC가 unsigned int이기 때문에 16bit를 초과할 가능성이 있어 해당부분 반영
				}
				else {
					CRC_32 = (CRC_32 << 1) & 0xffff;  //xor값이 0일시 CRC만 오른쪽으로 1bit shift
				}
			}
		}

		if (nCurCRC_32 == CRC_32) {
			return TRUE;
		}
		else {

			sBuf.Format("%03d", CRC_32);

			sBuf.SetAt(0, '*');
			sBuf.SetAt(1, '*');

			if (nCurCRC_32 == 0) {
				sTemp.Format("TOOL-CRC32 NG\nCalculation Value: %s, Setting Value: %d\nKey-In CRC Value", sBuf, nCurCRC_32);
			}
			else {
				sTemp.Format("TOOL-CRC32 NG\n Calculation Value: %s, Setting Value: %d\nKey-In  right CRC Value", sBuf, nCurCRC_32);
			}
			AfxMessageBox(sTemp);
			return FALSE;
		}

	}
	


#else
	if (m_szBoardOption != "") {
		tool[1] = hexStr2DecNum(m_szBoardOption.GetBuffer());// atoi(m_szBoardOption);
	}
	else {
		tool[1] = 0;
	}

	
	if(m_szToolOption1 != ""){
		tool[2] = atoi(m_szToolOption1);
	}
	else{
		tool[2] = 0;
	}
	if(m_szToolOption2 != ""){
		tool[3] = atoi(m_szToolOption2);
	}
	else{
		tool[3] = 0;
	}
	if(m_szToolOption3 != ""){
		tool[4] = atoi(m_szToolOption3);
	}
	else{
		tool[4] = 0;
	}
	if(m_szToolOption4 != ""){
		tool[5] = atoi(m_szToolOption4);
	}
	else{
		tool[5] = 0;
	}
	if(m_szToolOption5 != ""){
		tool[6] = atoi(m_szToolOption5);
	}
	else{
		tool[6] = 0;
	}
	if(m_szToolOption6 != ""){
		tool[7] = atoi(m_szToolOption6);
	}
	else{
		tool[7] = 0;
	}
	if(m_szToolOption7 != ""){
		tool[8] = atoi(m_szToolOption7);
	}
	else{
		tool[8] = 0;
	}
	if(m_szCommercialOption1 != ""){
		tool[9] = atoi(m_szCommercialOption1);
	}
	else{
		tool[9] = 0;
	}
	if(m_szToolOption8 != ""){
		tool[10] = atoi(m_szToolOption8);
	}
	else{
		tool[10] = 0;
	}
	if (m_szCommercialBoardOption != "") {
		tool[11] = hexStr2DecNum(m_szCommercialBoardOption.GetBuffer());
	}
	else {
		tool[11] = 0;
	}

	//m_szBoardOption = CurrentSet->sBoardOption;
	//m_szCommercialBoardOption = CurrentSet->sCommercialBoardOption;

	//if(m_szCommercialBoardOption != ""){
	//	tool[11] = atoi(m_szCommercialBoardOption);
	//}
	//else{
	//	tool[11] = 0;
	//}

	for(int i = 1; i < 12; i++)
	{
		if(tool[i] > 0xFFFF){bCrcCheck32 = TRUE;}
	}
	if(!bCrcCheck32) {
		if(m_szToolCRC != ""){
			bCrcCheck16 = TRUE;			
		}
		else{
			return TRUE;
		}
	}

	if(m_szToolCRC != ""){
		if(bCrcCheck32){
			nCurCRC_32 = atoi(m_szToolCRC);
		}
		else{
			nCurCRC_16 = atoi(m_szToolCRC);
		}		
	}


	if(bCrcCheck32){
		for(int t=1;t<=10;t++){      // ToolOption 1~9번까지 모든 data들을 확인한다.
			val32 = tool[t];          //해당 ToolOption값을 우선적으로 val값에 저장
			vector<int> bitarray = makeBitArray(val32);         //해당 val를 bitarray로 바꿔주는 부분 vector<int> 는 int형 vector (array로 생각해도 무방)
			for(int i=0;i<32;i++){				 //32bit에 해당되는 모든 bit를 한 bit씩 순회 가장 큰 bit부터 확인
				if((bitarray[i]) ^ (CRC_32>>15)){		 //현재 bit에 해당되는 부분과 CRC의 가장 큰 숫자 즉 (CRC>>15를 할시 2진법상 가장큰수)의 xor이 1일시 실행
					CRC_32 = ((CRC_32 << 1) & 0xffff) ^ ID_32;	// (CRC<<1은 CRC*2와 동일) CRC 오른쪽으로 1bit shift값과 ID값으 xor값을 CRC에 저장 //((CRC << 1) & 0xffff)이유는 CRC가 unsigned int이기 때문에 16bit를 초과할 가능성이 있어 해당부분 반영
				}	
				else{
					CRC_32 = (CRC_32 << 1) & 0xffff;  //xor값이 0일시 CRC만 오른쪽으로 1bit shift
				}
			}
		}
		
		if(nCurCRC_32 == CRC_32){
			return TRUE;
		}
		else{
			
			sBuf.Format("%03d", CRC_32);
			
			sBuf.SetAt(0, '*');
			sBuf.SetAt(1, '*');
			
			if(nCurCRC_32 == 0){
				sTemp.Format("TOOL-CRC32 NG\nCalculation Value: %s, Setting Value: %d\nKey-In CRC Value", sBuf, nCurCRC_32);
			}
			else{
				sTemp.Format("TOOL-CRC32 NG\n Calculation Value: %s, Setting Value: %d\nKey-In  right CRC Value", sBuf, nCurCRC_32);
			}
			AfxMessageBox(sTemp);
			return FALSE;
		}

	}
#endif	
	if(bCrcCheck16){
		for(int t=1;t<=11;t++){      // ToolOption 1~9번까지 모든 data들을 확인한다.
			val16 = tool[t];          //해당 ToolOption값을 우선적으로 val값에 저장
			if(val16 == 0){ continue;}
			vector<int> bitarray = makeBitArray(val16);         //해당 val를 bitarray로 바꿔주는 부분 vector<int> 는 int형 vector (array로 생각해도 무방)
			for(int i=0;i<16;i++){				 //32bit에 해당되는 모든 bit를 한 bit씩 순회 가장 큰 bit부터 확인
				if((val16 >> 15) ^ (CRC_16>>15)){		 //현재 bit에 해당되는 부분과 CRC의 가장 큰 숫자 즉 (CRC>>15를 할시 2진법상 가장큰수)의 xor이 1일시 실행
					CRC_16 = ((CRC_16 << 1) & 0xffff) ^ ID_16;	// (CRC<<1은 CRC*2와 동일) CRC 오른쪽으로 1bit shift값과 ID값으 xor값을 CRC에 저장 //((CRC << 1) & 0xffff)이유는 CRC가 unsigned int이기 때문에 16bit를 초과할 가능성이 있어 해당부분 반영
				}	
				else{
					CRC_16 = (CRC_16 << 1) & 0xffff;  //xor값이 0일시 CRC만 오른쪽으로 1bit shift
				}
				val16 <<= 1; 
			//	if((bitarray[i]) ^ (CRC>>15)){		 //현재 bit에 해당되는 부분과 CRC의 가장 큰 숫자 즉 (CRC>>15를 할시 2진법상 가장큰수)의 xor이 1일시 실행
			//		CRC = ((CRC << 1) & 0xffff) ^ ID;	// (CRC<<1은 CRC*2와 동일) CRC 오른쪽으로 1bit shift값과 ID값으 xor값을 CRC에 저장 //((CRC << 1) & 0xffff)이유는 CRC가 unsigned int이기 때문에 16bit를 초과할 가능성이 있어 해당부분 반영
			//	}	
			//	else{
			//		CRC = (CRC << 1) & 0xffff;  //xor값이 0일시 CRC만 오른쪽으로 1bit shift
			//	}
			}
		}


		if(nCurCRC_16 == CRC_16){
			return TRUE;
		}
		else{
			sBuf.Format("%03d", CRC_16);

			sBuf.SetAt(0, '*');
			sBuf.SetAt(1, '*');

			sTemp.Format("TOOL-CRC16 NG\n Calculation Value: %s, Setting Value: %d\nKey-In  right CRC Value", sBuf, nCurCRC_16);
			AfxMessageBox(sTemp);
			return FALSE;
		}
	}
	return TRUE;
}

void COptSpecPage::InitSpecGrid()
{
	CString sTmp;

	int nRow;

	char *sHeader1[] = {"EDID_HDMI1", "EDID_HDMI2", "EDID_HDMI3", "EDID_HDMI4", "EDID_HDMI5", "EDID_DP", "EDID_DP2", "EDID_DVI", "EDID_PC", "EDID_RGB",
						"CI", /*"CNTV", */"DVBS", "DVBS2", "ESN", "DTCP_RX", "HDCP_RX", "HDCP_RX_14", "HDCP_TX", /*"WASU",*/ "ADC", "WIDEVINE", "MARINKEY",
						"BUILT_WIFI", "JAPAN_LNB", "MAC_ADDRESS" , "ATSC3_0_KEY" , "MFI", "CI+ECP","CA", "MODEL_NAME", "SERIAL_NO" };


#if 1
	m_CtrlListSpecGrid.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListSpecGrid.InsertColumn(0, _T("INDEX"), LVCFMT_CENTER, 100);
	m_CtrlListSpecGrid.InsertColumn(1, _T("OPTION"), LVCFMT_CENTER, 95);// 리스트 행 추가
	for (int i = 0; i < 30; i++)
	{
		sTmp = sHeader1[i];
		m_CtrlListSpecGrid.InsertItem(i, sTmp);
	}
	//SetItemText

	m_CtrlListSpecGrid.SetItemText(0, 1, CurrentSet->sEDID_HDMI1);
	m_CtrlListSpecGrid.SetItemText(1, 1, CurrentSet->sEDID_HDMI2);
	m_CtrlListSpecGrid.SetItemText(2, 1, CurrentSet->sEDID_HDMI3);
	m_CtrlListSpecGrid.SetItemText(3, 1, CurrentSet->sEDID_HDMI4);
	m_CtrlListSpecGrid.SetItemText(4, 1, CurrentSet->sEDID_HDMI5);
	m_CtrlListSpecGrid.SetItemText(5, 1, CurrentSet->sEDID_DP);
	m_CtrlListSpecGrid.SetItemText(6, 1, CurrentSet->sEDID_DP2);
	m_CtrlListSpecGrid.SetItemText(7, 1, CurrentSet->sEDID_DVI);
	m_CtrlListSpecGrid.SetItemText(8, 1, CurrentSet->sEDID_PC);
	m_CtrlListSpecGrid.SetItemText(9, 1, CurrentSet->sEDID_RGB);

	m_CtrlListSpecGrid.SetItemText(10, 1, CurrentSet->sCI);//	m_CtrlListSpecGrid.SetItemText(11, 1, CurrentSet->sCNTV);
	m_CtrlListSpecGrid.SetItemText(11, 1, CurrentSet->sDVBS);
	m_CtrlListSpecGrid.SetItemText(12, 1, CurrentSet->sDVBS2);
	m_CtrlListSpecGrid.SetItemText(13, 1, CurrentSet->sESN);
	m_CtrlListSpecGrid.SetItemText(14, 1, CurrentSet->sDTCP_RX);
	m_CtrlListSpecGrid.SetItemText(15, 1, CurrentSet->sHDCP_RX);
	m_CtrlListSpecGrid.SetItemText(16, 1, CurrentSet->sHDCP_RX_14);
	m_CtrlListSpecGrid.SetItemText(17, 1, CurrentSet->sHDCP_TX);//	m_CtrlListSpecGrid.SetItemText(19, 1, CurrentSet->sWASU);
	m_CtrlListSpecGrid.SetItemText(18, 1, CurrentSet->sADC);
	m_CtrlListSpecGrid.SetItemText(19, 1, CurrentSet->sWIDEVINE);
	m_CtrlListSpecGrid.SetItemText(20, 1, CurrentSet->sMARINKEY);
	m_CtrlListSpecGrid.SetItemText(21, 1, CurrentSet->sBUILT_WIFI);
	m_CtrlListSpecGrid.SetItemText(22, 1, CurrentSet->sJAPAN_LNB);
	m_CtrlListSpecGrid.SetItemText(23, 1, CurrentSet->sMAC_ADDRESS);
	m_CtrlListSpecGrid.SetItemText(24, 1, CurrentSet->sASTC30_Key);
	m_CtrlListSpecGrid.SetItemText(25, 1, CurrentSet->sMfi_Key);
	m_CtrlListSpecGrid.SetItemText(26, 1, CurrentSet->sCI_CEP_Key);
	m_CtrlListSpecGrid.SetItemText(27, 1, CurrentSet->sCA);
	m_CtrlListSpecGrid.SetItemText(28, 1, CurrentSet->sMODEL_NAME);
	m_CtrlListSpecGrid.SetItemText(29, 1, CurrentSet->sSERIAL_NO);


#else
	
	m_ctrlSpecGrid.SetRedraw(FALSE);
	m_ctrlSpecGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlSpecGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlSpecGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlSpecGrid.SetBackColor(RGB(247,247,231));
	m_ctrlSpecGrid.SetGridColor(RGB(200,200,176));

	m_ctrlSpecGrid.SetFixedRows(0);
	m_ctrlSpecGrid.SetFixedCols(1);

	m_ctrlSpecGrid.SetFillStyle(flexFillRepeat);
	m_ctrlSpecGrid.SetFontName("Arial");
	m_ctrlSpecGrid.SetFontSize(8);

	m_ctrlSpecGrid.SetCols(2);
	m_ctrlSpecGrid.SetRows(32);
	m_ctrlSpecGrid.SetExtendLastCol(TRUE);

	m_ctrlSpecGrid.SetColWidth(0, 1300); //500
	m_ctrlSpecGrid.SetColWidth(1, 1000); //500
	m_ctrlSpecGrid.SetColAlignment(1,flexAlignCenterCenter);		
	m_ctrlSpecGrid.SetFixedAlignment(0,flexAlignRightCenter);

	for(nRow = 0; nRow < 32 ; nRow++)
	{
		m_ctrlSpecGrid.SetRow(nRow);
		m_ctrlSpecGrid.SetCol(0);
	
	//	m_ctrlSpecGrid.SetRowHeight(nRow, 200);
		m_ctrlSpecGrid.SetText(sHeader1[nRow]);

		m_ctrlSpecGrid.SetCol(1);
		m_ctrlSpecGrid.SetEditable(flexEDKbdMouse);
		if(nRow >= 10){
			m_ctrlSpecGrid.SetComboList("|NO|YES");
		}

	}

	m_ctrlSpecGrid.SetTextMatrix(0, 1, CurrentSet->sEDID_HDMI1);
	m_ctrlSpecGrid.SetTextMatrix(1, 1, CurrentSet->sEDID_HDMI2);
	m_ctrlSpecGrid.SetTextMatrix(2, 1, CurrentSet->sEDID_HDMI3);
	m_ctrlSpecGrid.SetTextMatrix(3, 1, CurrentSet->sEDID_HDMI4);
	m_ctrlSpecGrid.SetTextMatrix(4, 1, CurrentSet->sEDID_HDMI5);
	m_ctrlSpecGrid.SetTextMatrix(5, 1, CurrentSet->sEDID_DP);
	m_ctrlSpecGrid.SetTextMatrix(6, 1, CurrentSet->sEDID_DP2);
	m_ctrlSpecGrid.SetTextMatrix(7, 1, CurrentSet->sEDID_DVI);
	m_ctrlSpecGrid.SetTextMatrix(8, 1, CurrentSet->sEDID_PC);
	m_ctrlSpecGrid.SetTextMatrix(9, 1, CurrentSet->sEDID_RGB);
	
	m_ctrlSpecGrid.SetTextMatrix(10, 1, CurrentSet->sCI);
	m_ctrlSpecGrid.SetTextMatrix(11, 1, CurrentSet->sCNTV);
	m_ctrlSpecGrid.SetTextMatrix(12, 1, CurrentSet->sDVBS);
	m_ctrlSpecGrid.SetTextMatrix(13, 1, CurrentSet->sDVBS2);
	m_ctrlSpecGrid.SetTextMatrix(14, 1, CurrentSet->sESN);
	m_ctrlSpecGrid.SetTextMatrix(15, 1, CurrentSet->sDTCP_RX);
	m_ctrlSpecGrid.SetTextMatrix(16, 1, CurrentSet->sHDCP_RX);
	m_ctrlSpecGrid.SetTextMatrix(17, 1, CurrentSet->sHDCP_RX_14);
	m_ctrlSpecGrid.SetTextMatrix(18, 1, CurrentSet->sHDCP_TX);
	m_ctrlSpecGrid.SetTextMatrix(19, 1, CurrentSet->sWASU);
	m_ctrlSpecGrid.SetTextMatrix(20, 1, CurrentSet->sADC);
	m_ctrlSpecGrid.SetTextMatrix(21, 1, CurrentSet->sWIDEVINE);
	m_ctrlSpecGrid.SetTextMatrix(22, 1, CurrentSet->sMARINKEY);
	m_ctrlSpecGrid.SetTextMatrix(23, 1, CurrentSet->sBUILT_WIFI);
	m_ctrlSpecGrid.SetTextMatrix(24, 1, CurrentSet->sJAPAN_LNB);
	m_ctrlSpecGrid.SetTextMatrix(25, 1, CurrentSet->sMAC_ADDRESS);
	m_ctrlSpecGrid.SetTextMatrix(26, 1, CurrentSet->sASTC30_Key);
	m_ctrlSpecGrid.SetTextMatrix(27, 1, CurrentSet->sMfi_Key);
	m_ctrlSpecGrid.SetTextMatrix(28, 1, CurrentSet->sCI_CEP_Key);
	m_ctrlSpecGrid.SetTextMatrix(29, 1, CurrentSet->sCA);
	m_ctrlSpecGrid.SetTextMatrix(30, 1, CurrentSet->sMODEL_NAME);
	m_ctrlSpecGrid.SetTextMatrix(31, 1, CurrentSet->sSERIAL_NO);

	m_ctrlSpecGrid.SetRedraw(TRUE);
	m_ctrlSpecGrid.Refresh();

#endif

}
void COptSpecPage::InitSpecGrid2()
{
	CString sTmp;
//	int nCol;
	int nRow;

	char *sHeader1[] = {"AV1", "AV2", "SCART", "COMP1", "COMP2", "USB1", "USB2", "USB3", "USB4", "HEAD_PHONE", "HDMI1", "HDMI2", "HDMI3", "HDMI4", "RGB", "DVI", "DP", "SMHL"};
//	CString sTmp;

	m_CtrlListSpecIndexOption.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);// 리스트 컨트롤 초기화: 열 추가
	m_CtrlListSpecIndexOption.InsertColumn(0, _T("INDEX"), LVCFMT_CENTER, 100);
	m_CtrlListSpecIndexOption.InsertColumn(1, _T("OPTION"), LVCFMT_CENTER, 95);// 리스트 행 추가
	for (int i = 0; i < 18; i++)
	{
		sTmp = sHeader1[i];
		m_CtrlListSpecIndexOption.InsertItem(i, sTmp);
	}

	//m_CtrlListIndexOption.SetItemText(0,1,)
	//m_ctrlStepMainList.SetItem(nTmp, 1, LVIF_TEXT, pStep->GetCurrentSource(), NULL, NULL, NULL, NULL);

	m_CtrlListSpecIndexOption.SetItemText(0, 1, CurrentSet->sJACK_AV1);
	m_CtrlListSpecIndexOption.SetItemText(1, 1, CurrentSet->sJACK_AV2);
	m_CtrlListSpecIndexOption.SetItemText(2, 1, CurrentSet->sJACK_SCART);
	m_CtrlListSpecIndexOption.SetItemText(3, 1, CurrentSet->sJACK_COMP1);
	m_CtrlListSpecIndexOption.SetItemText(4, 1, CurrentSet->sJACK_COMP2);
	m_CtrlListSpecIndexOption.SetItemText(5, 1, CurrentSet->sJACK_USB1);
	m_CtrlListSpecIndexOption.SetItemText(6, 1, CurrentSet->sJACK_USB2);
	m_CtrlListSpecIndexOption.SetItemText(7, 1, CurrentSet->sJACK_USB3);
	m_CtrlListSpecIndexOption.SetItemText(8, 1, CurrentSet->sJACK_USB4);
	m_CtrlListSpecIndexOption.SetItemText(9, 1, CurrentSet->sJACK_HEAD_PHONE);
	m_CtrlListSpecIndexOption.SetItemText(10, 1, CurrentSet->sJACK_HDMI1);;
	m_CtrlListSpecIndexOption.SetItemText(11, 1, CurrentSet->sJACK_HDMI2);
	m_CtrlListSpecIndexOption.SetItemText(12, 1, CurrentSet->sJACK_HDMI3);
	m_CtrlListSpecIndexOption.SetItemText(13, 1, CurrentSet->sJACK_HDMI4);
	m_CtrlListSpecIndexOption.SetItemText(14, 1, CurrentSet->sJACK_RGB);
	m_CtrlListSpecIndexOption.SetItemText(15, 1, CurrentSet->sJACK_DVI);
	m_CtrlListSpecIndexOption.SetItemText(16, 1, CurrentSet->sJACK_DP);
	m_CtrlListSpecIndexOption.SetItemText(17, 1, CurrentSet->sJACK_SMHL);
#if 0
	m_ctrlSpecGrid2.SetRedraw(FALSE);
	m_ctrlSpecGrid2.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlSpecGrid2.SetBackColorFixed(RGB(210,216,176));
	m_ctrlSpecGrid2.SetForeColorFixed(RGB(51,102,153));
	m_ctrlSpecGrid2.SetBackColor(RGB(247,247,231));
	m_ctrlSpecGrid2.SetGridColor(RGB(200,200,176));

	m_ctrlSpecGrid2.SetFixedRows(0);
	m_ctrlSpecGrid2.SetFixedCols(1);

	m_ctrlSpecGrid2.SetFillStyle(flexFillRepeat);
	m_ctrlSpecGrid2.SetFontName("Arial");
	m_ctrlSpecGrid2.SetFontSize(8);

	m_ctrlSpecGrid2.SetCols(2);
	m_ctrlSpecGrid2.SetRows(18);
	m_ctrlSpecGrid2.SetExtendLastCol(TRUE);

	m_ctrlSpecGrid2.SetColWidth(0, 1300); //500
	m_ctrlSpecGrid2.SetColWidth(1, 1000); //500
	m_ctrlSpecGrid2.SetColAlignment(1,flexAlignCenterCenter);		
	m_ctrlSpecGrid2.SetFixedAlignment(0,flexAlignRightCenter);

	for(nRow = 0; nRow < 18 ; nRow++)
	{
		m_ctrlSpecGrid2.SetRow(nRow);
		m_ctrlSpecGrid2.SetCol(0);
	
	//	m_ctrlSpecGrid2.SetRowHeight(nRow, 200);
		m_ctrlSpecGrid2.SetText(sHeader1[nRow]);

		m_ctrlSpecGrid2.SetCol(1);
		m_ctrlSpecGrid2.SetEditable(flexEDKbdMouse);
		if(nRow >= 10){
			m_ctrlSpecGrid2.SetComboList("|NO|YES");
		}

	}
	m_ctrlSpecGrid2.SetTextMatrix(0, 1, CurrentSet->sJACK_AV1);       
	m_ctrlSpecGrid2.SetTextMatrix(1, 1, CurrentSet->sJACK_AV2);       
	m_ctrlSpecGrid2.SetTextMatrix(2, 1, CurrentSet->sJACK_SCART);     
	m_ctrlSpecGrid2.SetTextMatrix(3, 1, CurrentSet->sJACK_COMP1);     
	m_ctrlSpecGrid2.SetTextMatrix(4, 1, CurrentSet->sJACK_COMP2);     
	m_ctrlSpecGrid2.SetTextMatrix(5, 1, CurrentSet->sJACK_USB1);      
	m_ctrlSpecGrid2.SetTextMatrix(6, 1, CurrentSet->sJACK_USB2);      
	m_ctrlSpecGrid2.SetTextMatrix(7, 1, CurrentSet->sJACK_USB3);      
	m_ctrlSpecGrid2.SetTextMatrix(8, 1, CurrentSet->sJACK_USB4);      
	m_ctrlSpecGrid2.SetTextMatrix(9, 1, CurrentSet->sJACK_HEAD_PHONE);
	m_ctrlSpecGrid2.SetTextMatrix(10, 1, CurrentSet->sJACK_HDMI1);;    
	m_ctrlSpecGrid2.SetTextMatrix(11, 1, CurrentSet->sJACK_HDMI2);     
	m_ctrlSpecGrid2.SetTextMatrix(12, 1, CurrentSet->sJACK_HDMI3);     
	m_ctrlSpecGrid2.SetTextMatrix(13, 1, CurrentSet->sJACK_HDMI4);     
	m_ctrlSpecGrid2.SetTextMatrix(14, 1, CurrentSet->sJACK_RGB);       
	m_ctrlSpecGrid2.SetTextMatrix(15, 1, CurrentSet->sJACK_DVI);       
	m_ctrlSpecGrid2.SetTextMatrix(16, 1, CurrentSet->sJACK_DP);        
	m_ctrlSpecGrid2.SetTextMatrix(17, 1, CurrentSet->sJACK_SMHL);      

	m_ctrlSpecGrid2.SetRedraw(TRUE);
	m_ctrlSpecGrid2.Refresh();
#endif
}


void COptSpecPage::OnEnChangeEditDrmKey()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CPropertyPage::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//
//void COptSpecPage::OnBnClickedBtnCheckDrm()
//{
//	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//CurrentSet->sCI = m_ctrlSpecGrid.GetTextMatrix(10, 1); //0x100	
//	//CurrentSet->sCNTV = m_ctrlSpecGrid.GetTextMatrix(11, 1); //0x200		
//	//CurrentSet->sDVBS = m_ctrlSpecGrid.GetTextMatrix(12, 1);
//	//CurrentSet->sDVBS2 = m_ctrlSpecGrid.GetTextMatrix(13, 1);
//	//CurrentSet->sESN = m_ctrlSpecGrid.GetTextMatrix(14, 1); 	//0x04	
//	//CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400       
//	//CurrentSet->sHDCP_RX = m_ctrlSpecGrid.GetTextMatrix(16, 1); 	//0x20       
//	//CurrentSet->sHDCP_RX_14 = m_ctrlSpecGrid.GetTextMatrix(17, 1); 	//0x10
//	//CurrentSet->sHDCP_TX = m_ctrlSpecGrid.GetTextMatrix(18, 1);
//	//CurrentSet->sWASU = m_ctrlSpecGrid.GetTextMatrix(19, 1);
//	//CurrentSet->sADC = m_ctrlSpecGrid.GetTextMatrix(20, 1);
//	//CurrentSet->sWIDEVINE = m_ctrlSpecGrid.GetTextMatrix(21, 1); //0x08	        
//	//CurrentSet->sMARINKEY = m_ctrlSpecGrid.GetTextMatrix(22, 1); //0x80	        
//	//CurrentSet->sBUILT_WIFI = m_ctrlSpecGrid.GetTextMatrix(23, 1);
//	//CurrentSet->sJAPAN_LNB = m_ctrlSpecGrid.GetTextMatrix(24, 1);
//	//CurrentSet->sMAC_ADDRESS = m_ctrlSpecGrid.GetTextMatrix(25, 1); 	//0x02
//	//CurrentSet->sCA = m_ctrlSpecGrid.GetTextMatrix(26, 1);
//	//CurrentSet->sMODEL_NAME = m_ctrlSpecGrid.GetTextMatrix(27, 1);
//	//CurrentSet->sSERIAL_NO = m_ctrlSpecGrid.GetTextMatrix(28, 1);
//	////0x01 = 1
//	////0x40, = 0
//
//	//LONG lDrmKey = 0x01;
//	//if (CurrentSet->sMAC_ADDRESS == "YES")		 lDrmKey |= 0x02;
//	//if (CurrentSet->sESN == "YES")		 lDrmKey |= 0x04;//CurrentSet->sESN = m_ctrlSpecGrid.GetTextMatrix(14, 1); 	//0x04	
//	//if (CurrentSet->sWIDEVINE == "YES")		 lDrmKey |= 0x08;//CurrentSet->sWIDEVINE = m_ctrlSpecGrid.GetTextMatrix(21, 1); //0x08	 
//	//if (CurrentSet->sHDCP_RX_14 == "YES")		 lDrmKey |= 0x10;//CurrentSet->sHDCP_RX_14 = m_ctrlSpecGrid.GetTextMatrix(17, 1); 	//0x10
//	//if (CurrentSet->sHDCP_RX == "YES")		 lDrmKey |= 0x20;//CurrentSet->sHDCP_RX = m_ctrlSpecGrid.GetTextMatrix(16, 1); 	//0x20   
//	////0x40, = 0
//	//if (CurrentSet->sMARINKEY == "YES")		 lDrmKey |= 0x80;// CurrentSet->sMARINKEY = m_ctrlSpecGrid.GetTextMatrix(22, 1); //0x80	
//	//if (CurrentSet->sCI == "YES")		 lDrmKey |= 0x100;// CurrentSet->sCI = m_ctrlSpecGrid.GetTextMatrix(10, 1); //0x100	
//	//if (CurrentSet->sCNTV == "YES")		 lDrmKey |= 0x200;// CurrentSet->sCNTV = m_ctrlSpecGrid.GetTextMatrix(11, 1); //0x200
//	//if (CurrentSet->sDTCP_RX == "YES")		 lDrmKey |= 0x400;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 
//
//	//CString strtmp;
//	//strtmp.Format(_T("0x%04X"), lDrmKey); 
//	//SetDlgItemText(IDC_EDIT_DRM_KEY, strtmp);
//}

void COptSpecPage::OnBnClickedButtonCheckDrmKey()
{
#if 1
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CurrentSet->sCI				= m_CtrlListSpecGrid.GetItemText(10, 1); //0x100	//	CurrentSet->sCNTV	= m_CtrlListSpecGrid.GetItemText(11, 1); //0x200		
	CurrentSet->sDVBS			= m_CtrlListSpecGrid.GetItemText(11, 1);
	CurrentSet->sDVBS2			= m_CtrlListSpecGrid.GetItemText(12, 1);
	CurrentSet->sESN			= m_CtrlListSpecGrid.GetItemText(13, 1); 	//0x04	
	CurrentSet->sDTCP_RX		= m_CtrlListSpecGrid.GetItemText(14, 1); 	//0x400       
	CurrentSet->sHDCP_RX		= m_CtrlListSpecGrid.GetItemText(15, 1); 	//0x20       
	CurrentSet->sHDCP_RX_14		= m_CtrlListSpecGrid.GetItemText(16, 1); 	//0x10
	CurrentSet->sHDCP_TX		= m_CtrlListSpecGrid.GetItemText(17, 1); //	CurrentSet->sWASU			= m_CtrlListSpecGrid.GetItemText(19, 1);
	CurrentSet->sADC			= m_CtrlListSpecGrid.GetItemText(18, 1);
	CurrentSet->sWIDEVINE		= m_CtrlListSpecGrid.GetItemText(19, 1); //0x08	        
	CurrentSet->sMARINKEY		= m_CtrlListSpecGrid.GetItemText(20, 1); //0x80	        
	CurrentSet->sBUILT_WIFI		= m_CtrlListSpecGrid.GetItemText(21, 1);
	CurrentSet->sJAPAN_LNB		= m_CtrlListSpecGrid.GetItemText(22, 1);
	CurrentSet->sMAC_ADDRESS	= m_CtrlListSpecGrid.GetItemText(23, 1); 	//0x02
	CurrentSet->sASTC30_Key		= m_CtrlListSpecGrid.GetItemText(24, 1);
	CurrentSet->sMfi_Key		= m_CtrlListSpecGrid.GetItemText(25, 1);
	CurrentSet->sCI_CEP_Key		= m_CtrlListSpecGrid.GetItemText(26, 1);
	CurrentSet->sCA				= m_CtrlListSpecGrid.GetItemText(27, 1);
	CurrentSet->sMODEL_NAME		= m_CtrlListSpecGrid.GetItemText(28, 1);
	CurrentSet->sSERIAL_NO		= m_CtrlListSpecGrid.GetItemText(29, 1);
	//0x01 = 1
	//0x40, = 0

	LONG lDrmKey = 0x01;
	if (CurrentSet->sMAC_ADDRESS == "YES")		 lDrmKey |= 0x02;
	if (CurrentSet->sESN == "YES")		 lDrmKey |= 0x04;//CurrentSet->sESN = m_ctrlSpecGrid.GetTextMatrix(14, 1); 	//0x04	
	if (CurrentSet->sWIDEVINE == "YES")		 lDrmKey |= 0x08;//CurrentSet->sWIDEVINE = m_ctrlSpecGrid.GetTextMatrix(21, 1); //0x08	 
	if (CurrentSet->sHDCP_RX_14 == "YES")		 lDrmKey |= 0x10;//CurrentSet->sHDCP_RX_14 = m_ctrlSpecGrid.GetTextMatrix(17, 1); 	//0x10
	if (CurrentSet->sHDCP_RX == "YES")		 lDrmKey |= 0x20;//CurrentSet->sHDCP_RX = m_ctrlSpecGrid.GetTextMatrix(16, 1); 	//0x20   
	//0x40, = 0
	if (CurrentSet->sMARINKEY == "YES")		 lDrmKey |= 0x80;// CurrentSet->sMARINKEY = m_ctrlSpecGrid.GetTextMatrix(22, 1); //0x80	
	if (CurrentSet->sCI == "YES")		 lDrmKey |= 0x100;// CurrentSet->sCI = m_ctrlSpecGrid.GetTextMatrix(10, 1); //0x100	
	if (CurrentSet->sCNTV == "YES")		 lDrmKey |= 0x200;// CurrentSet->sCNTV = m_ctrlSpecGrid.GetTextMatrix(11, 1); //0x200
	if (CurrentSet->sDTCP_RX == "YES")		 lDrmKey |= 0x400;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 
	if (CurrentSet->sASTC30_Key == "YES")		 lDrmKey |= 0x800;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 
	if (CurrentSet->sMfi_Key == "YES")		 lDrmKey |= 0x1000;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 
	if (CurrentSet->sCI_CEP_Key == "YES")		 lDrmKey |= 0x2000;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 

	CString strtmp;
	strtmp.Format(_T("0x%04X"), lDrmKey);
	SetDlgItemText(IDC_EDIT_DRM_KEY, strtmp);
#else
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CurrentSet->sCI				= m_ctrlSpecGrid.GetTextMatrix(10, 1); //0x100	
	CurrentSet->sCNTV			= m_ctrlSpecGrid.GetTextMatrix(11, 1); //0x200		
	CurrentSet->sDVBS			= m_ctrlSpecGrid.GetTextMatrix(12, 1);
	CurrentSet->sDVBS2			= m_ctrlSpecGrid.GetTextMatrix(13, 1);
	CurrentSet->sESN			= m_ctrlSpecGrid.GetTextMatrix(14, 1); 	//0x04	
	CurrentSet->sDTCP_RX		= m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400       
	CurrentSet->sHDCP_RX		= m_ctrlSpecGrid.GetTextMatrix(16, 1); 	//0x20       
	CurrentSet->sHDCP_RX_14		= m_ctrlSpecGrid.GetTextMatrix(17, 1); 	//0x10
	CurrentSet->sHDCP_TX		= m_ctrlSpecGrid.GetTextMatrix(18, 1);
	CurrentSet->sWASU			= m_ctrlSpecGrid.GetTextMatrix(19, 1);
	CurrentSet->sADC			= m_ctrlSpecGrid.GetTextMatrix(20, 1);
	CurrentSet->sWIDEVINE		= m_ctrlSpecGrid.GetTextMatrix(21, 1); //0x08	        
	CurrentSet->sMARINKEY		= m_ctrlSpecGrid.GetTextMatrix(22, 1); //0x80	        
	CurrentSet->sBUILT_WIFI		= m_ctrlSpecGrid.GetTextMatrix(23, 1);
	CurrentSet->sJAPAN_LNB		= m_ctrlSpecGrid.GetTextMatrix(24, 1);
	CurrentSet->sMAC_ADDRESS	= m_ctrlSpecGrid.GetTextMatrix(25, 1); 	//0x02
	CurrentSet->sASTC30_Key		= m_ctrlSpecGrid.GetTextMatrix(26, 1);
	CurrentSet->sMfi_Key		= m_ctrlSpecGrid.GetTextMatrix(27, 1);
	CurrentSet->sCI_CEP_Key		= m_ctrlSpecGrid.GetTextMatrix(28, 1);
	CurrentSet->sCA				= m_ctrlSpecGrid.GetTextMatrix(29, 1);
	CurrentSet->sMODEL_NAME		= m_ctrlSpecGrid.GetTextMatrix(30, 1);
	CurrentSet->sSERIAL_NO		= m_ctrlSpecGrid.GetTextMatrix(31, 1);
	//0x01 = 1
	//0x40, = 0

	LONG lDrmKey = 0x01;
	if (CurrentSet->sMAC_ADDRESS == "YES")		 lDrmKey |= 0x02;
	if (CurrentSet->sESN == "YES")		 lDrmKey |= 0x04;//CurrentSet->sESN = m_ctrlSpecGrid.GetTextMatrix(14, 1); 	//0x04	
	if (CurrentSet->sWIDEVINE == "YES")		 lDrmKey |= 0x08;//CurrentSet->sWIDEVINE = m_ctrlSpecGrid.GetTextMatrix(21, 1); //0x08	 
	if (CurrentSet->sHDCP_RX_14 == "YES")		 lDrmKey |= 0x10;//CurrentSet->sHDCP_RX_14 = m_ctrlSpecGrid.GetTextMatrix(17, 1); 	//0x10
	if (CurrentSet->sHDCP_RX == "YES")		 lDrmKey |= 0x20;//CurrentSet->sHDCP_RX = m_ctrlSpecGrid.GetTextMatrix(16, 1); 	//0x20   
	//0x40, = 0
	if (CurrentSet->sMARINKEY == "YES")		 lDrmKey |= 0x80;// CurrentSet->sMARINKEY = m_ctrlSpecGrid.GetTextMatrix(22, 1); //0x80	
	if (CurrentSet->sCI == "YES")		 lDrmKey |= 0x100;// CurrentSet->sCI = m_ctrlSpecGrid.GetTextMatrix(10, 1); //0x100	
	if (CurrentSet->sCNTV == "YES")		 lDrmKey |= 0x200;// CurrentSet->sCNTV = m_ctrlSpecGrid.GetTextMatrix(11, 1); //0x200
	if (CurrentSet->sDTCP_RX == "YES")		 lDrmKey |= 0x400;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 
	if (CurrentSet->sASTC30_Key == "YES")		 lDrmKey |= 0x800;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 
	if (CurrentSet->sMfi_Key == "YES")		 lDrmKey |= 0x1000;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 
	if (CurrentSet->sCI_CEP_Key == "YES")		 lDrmKey |= 0x2000;// CurrentSet->sDTCP_RX = m_ctrlSpecGrid.GetTextMatrix(15, 1); 	//0x400 

	CString strtmp;
	strtmp.Format(_T("0x%04X"), lDrmKey);
	SetDlgItemText(IDC_EDIT_DRM_KEY, strtmp);

#endif
}


void COptSpecPage::ChangeEditSpecGrid()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//OnBnClickedButtonCheckDrmKey();
}


void COptSpecPage::OnNMClickListSpecIndexOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	CString strTmp;

	if (nShell == 1)
	{
		strTmp = m_CtrlListSpecIndexOption.GetItemText(nItem, nShell);
		if (strTmp.Find("YES") >= 0)
		{
			m_CtrlListSpecIndexOption.SetItemText(nItem, 1, "NO");
		}
		else
		{
			m_CtrlListSpecIndexOption.SetItemText(nItem, 1, "YES");
		}
	}


	*pResult = 0;
}


void COptSpecPage::OnNMCustomdrawListSpecIndexOption(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int nRow=0;
	//int nCol=0;
	CString strCol = _T("");

	switch (pLVCD->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			return;
		}
		case CDDS_ITEMPREPAINT:
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
		case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		{
			COLORREF crText, crBkgnd;

			//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
			//	nRow= m_CtrlListMainProcess.GetItemCount();

			int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row





			if (1 == (int)(pLVCD->iSubItem))
			{
				//m_ctrlSpecGrid1.SetBackColorFixed(RGB(210, 216, 176));
				//m_ctrlSpecGrid1.SetForeColorFixed(RGB(51, 102, 153));
				//m_ctrlSpecGrid1.SetBackColor(RGB(247, 247, 231));
				//m_ctrlSpecGrid1.SetGridColor(RGB(200, 200, 176));

				crText = RGB(0, 0, 0);
				crBkgnd = RGB(220, 220, 150);


			}
			else
			{
				//m_ctrlOptionGrid.SetBackColorFixed(RGB(210, 216, 176));
				//m_ctrlOptionGrid.SetForeColorFixed(RGB(51, 102, 153));
				//m_ctrlOptionGrid.SetBackColor(RGB(247, 247, 231));
				//m_ctrlOptionGrid.SetGridColor(RGB(200, 200, 176));

				crText = RGB(0, 0, 0);
				crBkgnd = RGB(210, 216, 176);

			}

			pLVCD->clrText = crText;
			pLVCD->clrTextBk = crBkgnd;


			*pResult = CDRF_DODEFAULT;
			return;
		}
	}
	*pResult = 0;
}


void COptSpecPage::OnNMClickListSpecGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	int nItem = pNMItemActivate->iItem;
	int nShell = pNMItemActivate->iSubItem;
	CString strTmp;

	if (nShell == 1)
	{
		strTmp = m_CtrlListSpecGrid.GetItemText(nItem, nShell);
		if (strTmp.Find("YES") >= 0)
		{
			m_CtrlListSpecGrid.SetItemText(nItem, 1, "NO");
		}
		else
		{
			m_CtrlListSpecGrid.SetItemText(nItem, 1, "YES");
		}
	}
	*pResult = 0;
}


void COptSpecPage::OnNMCustomdrawListSpecGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMHDR;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//int nRow=0;
	//int nCol=0;
	CString strCol = _T("");

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	}
	case CDDS_ITEMPREPAINT:
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		return;
	}
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
	{
		COLORREF crText, crBkgnd;

		//	nCol= m_CtrlListMainProcess.getse.GetSelectedColumn();
		//	nRow= m_CtrlListMainProcess.GetItemCount();

		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row





		if (1 == (int)(pLVCD->iSubItem))
		{
			//m_ctrlSpecGrid1.SetBackColorFixed(RGB(210, 216, 176));
			//m_ctrlSpecGrid1.SetForeColorFixed(RGB(51, 102, 153));
			//m_ctrlSpecGrid1.SetBackColor(RGB(247, 247, 231));
			//m_ctrlSpecGrid1.SetGridColor(RGB(200, 200, 176));

			crText = RGB(0, 0, 0);
			crBkgnd = RGB(220, 220, 150);


		}
		else
		{
			//m_ctrlOptionGrid.SetBackColorFixed(RGB(210, 216, 176));
			//m_ctrlOptionGrid.SetForeColorFixed(RGB(51, 102, 153));
			//m_ctrlOptionGrid.SetBackColor(RGB(247, 247, 231));
			//m_ctrlOptionGrid.SetGridColor(RGB(200, 200, 176));

			crText = RGB(0, 0, 0);
			crBkgnd = RGB(210, 216, 176);

		}

		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;


		*pResult = CDRF_DODEFAULT;
		return;
	}
	}
	*pResult = 0;
}


void COptSpecPage::OnBnClickedButtonOpEpiCfgSw()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	//OnBtnOptApply();
	UpdateData(TRUE);

	CurrentSet->sToolOption1 = m_szToolOption1;

	if (OpenEPI_ConfigFile(CurrentSet->sEpiCfgIni))
	{
		//CurrentSet->sDipStatus
	}
	else
	{
		CurrentSet->sDipStatus = "";
	}
	m_CEdit_OP_EPI_CFG_SW.SetWindowText(CurrentSet->sDipStatus);
}


void COptSpecPage::OnLvnItemchangedListSpecGrid(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

