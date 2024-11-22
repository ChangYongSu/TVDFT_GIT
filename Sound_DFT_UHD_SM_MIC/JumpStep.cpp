// 명    칭 : AIAS(Automatic Inline Alignment System)
// 창작일자 : 1997. 2. 21
// 소    속 : LG전자(LG생산기술원 MI Gr. 오구환)
// File  명 : JumpStep.cpp

// JumpStep.cpp : implementation file
//
 

#include "stdafx.h"
#include "DATsys.h"
#include "step.h"
#include "JumpStep.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJumpStep dialog


CJumpStep::CJumpStep(CWnd* pParent /*=NULL*/)
	: CDialog(CJumpStep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJumpStep)
	//}}AFX_DATA_INIT
}


void CJumpStep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJumpStep)
	DDX_Control(pDX, IDC_STEPLIST, m_stepList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJumpStep, CDialog)
	//{{AFX_MSG_MAP(CJumpStep)
	ON_LBN_DBLCLK(IDC_STEPLIST, OnDblclkSteplist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJumpStep message handlers

BOOL CJumpStep::OnInitDialog() 
{
    CString s;

    CDialog::OnInitDialog();
    POSITION pos = StepList.GetHeadPosition();
	while(pos != NULL)
    {
		CStep* pStep = StepList.GetNext(pos);

        s.Format("%d\t%s"
            , pStep->GetStepNo(), pStep->GetStepName());
        m_stepList.AddString(s);
	}
    m_stepList.SetCurSel(0);

	CenterWindow();
	ShowWindow(SW_SHOWNORMAL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJumpStep::OnDblclkSteplist() 
{
    SendMessage(WM_COMMAND, IDOK);
}

void CJumpStep::OnOK() 
{
    m_nChoice = m_stepList.GetCurSel();	
	CDialog::OnOK();
}
