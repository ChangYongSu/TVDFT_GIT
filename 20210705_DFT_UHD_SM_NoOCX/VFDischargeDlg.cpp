// VFDischargeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DATsys.h"
#include "VFDischargeDlg.h"
#include "afxdialogex.h"
#include "Global.h"
#include "VF1000.h"


extern CDATsysView*  g_pView;
UINT VolMeasThread(LPVOID lParam);

// CVFDischargeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVFDischargeDlg, CDialog)

CVFDischargeDlg::CVFDischargeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_VF_DISCHARGE_CHECK, pParent)
{
	m_bVfCheckResult = FALSE;
	m_bTestClose = TRUE;
	m_bTestRunning = FALSE;
	m_bThredStop = FALSE;
	m_nNoFailed = -1;
	m_pMeasureThread = NULL;

}

CVFDischargeDlg::~CVFDischargeDlg()
{
}

void CVFDischargeDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_INPUTCHECK_GRID, m_ctrlVfMeasureCheckGrid);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVFDischargeDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ENTER, &CVFDischargeDlg::OnClickedBtnEnter)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVFDischargeDlg 메시지 처리기입니다.

void CVFDischargeDlg::InitVfMeasureCheckGrid()
{
	CString sTmp;
	//	int nKeyItem;
	int nRow;

	char *sHeader1[] = { "CH","JIG POINT", "MEASURE","RESULT" };

	//int nRows = CurrentSet->VF_TestInfo.GetCount();
	int nRows = gVF1000Ctrl.GetVF_Count();

	m_ctrlVfMeasureCheckGrid.SetRedraw(FALSE);
	m_ctrlVfMeasureCheckGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlVfMeasureCheckGrid.SetBackColorFixed(RGB(210, 216, 176));
	m_ctrlVfMeasureCheckGrid.SetForeColorFixed(RGB(51, 102, 153));
	m_ctrlVfMeasureCheckGrid.SetBackColor(RGB(247, 247, 231));
	m_ctrlVfMeasureCheckGrid.SetGridColor(RGB(200, 200, 176));

	m_ctrlVfMeasureCheckGrid.SetFixedRows(1);
	m_ctrlVfMeasureCheckGrid.SetFixedCols(0);

	m_ctrlVfMeasureCheckGrid.SetFillStyle(flexFillRepeat);
	m_ctrlVfMeasureCheckGrid.SetFontName("Arial");
	m_ctrlVfMeasureCheckGrid.SetFontSize(8);

	m_ctrlVfMeasureCheckGrid.SetCols(4);
	m_ctrlVfMeasureCheckGrid.SetRows(nRows + 1);
	m_ctrlVfMeasureCheckGrid.SetExtendLastCol(TRUE);

	for (int nCol = 0; nCol < 4; nCol++)
	{
		m_ctrlVfMeasureCheckGrid.SetRow(0);
		m_ctrlVfMeasureCheckGrid.SetCol(nCol);
		m_ctrlVfMeasureCheckGrid.SetText(sHeader1[nCol]);
		m_ctrlVfMeasureCheckGrid.SetFixedAlignment(nCol, flexAlignCenterCenter);

		switch (nCol)
		{
		case 0: // No
			m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol, flexAlignCenterCenter);
			m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 600); //500
			break;

		case 1: // Test Name
			m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol, flexAlignLeftCenter);
			m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 1900);//3100
			break;

		case 2: // Measure
			m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol, flexAlignRightCenter);
			m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 1000);
			break;

		case 3: // Result
			m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol, flexAlignCenterCenter);
			m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 700);
			break;
		}
	}

	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();

}

void CVFDischargeDlg::ConfigDisplay()
{

	POSITION Pos = NULL;
	CVFTestInfo* pVFTestInfo = NULL;
	BOOL		bRun;
	UINT		nChNo;
	CString		strTestName;
	UINT		nCheckType;
	//	double		dTarget;	
	double		dLowerLimit;
	double		dUpperLimit;
	UINT		nTime = 0;
	CString sTmp;
	int nTestIndex = 0;

	if (CurrentSet->VF_TestInfo.GetCount() == 0) { return; }

	Pos = CurrentSet->VF_TestInfo.GetHeadPosition();

	while (Pos)
	{
		pVFTestInfo = CurrentSet->VF_TestInfo.GetNext(Pos);

		bRun = pVFTestInfo->bRun;
		nChNo = pVFTestInfo->nChNo;
		strTestName = pVFTestInfo->strTestName;
		nCheckType = pVFTestInfo->nCheckType;
		dLowerLimit = pVFTestInfo->dLowerLimit;
		dUpperLimit = pVFTestInfo->dUpperLimit;
		nTime = pVFTestInfo->nTime;
		if ((nCheckType == 1) && (dLowerLimit >= 0) && (dLowerLimit <= 0.5))
		{
			continue;
		}
		nTestIndex++;

		m_ctrlVfMeasureCheckGrid.SetRow(nTestIndex);

		if (nChNo <= 42) {
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 0, _T("V"));
			sTmp.Format(_T("[%d] "), nChNo);
			sTmp += strTestName;
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 1, sTmp);
		}
		else {
			continue;
		}
#ifdef SM_MODIFY_CODE__
		sTmp.Format(_T("%.1f"), dLowerLimit);
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 2, sTmp);

		//sTmp.Format(_T("%.1f"), dLowerLimit);
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 3, _T("CHECK"));


#else
	//	sTmp.Format(_T("%.1f"), dLowerLimit);
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 2, strTestName);

		sTmp.Format(_T("%.1f"), dLowerLimit);
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 3, sTmp);

		if (nCheckType == 2) {
			sTmp.Format(_T("%.1f"), dUpperLimit);
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 4, sTmp);
		}

#endif
	}
	m_ctrlVfMeasureCheckGrid.SetRow(-1);
	m_ctrlVfMeasureCheckGrid.SetCol(-1);
	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();
}

void CVFDischargeDlg::UpdateResult(int nIndex, CString sMeasure, BOOL bResult)
{
	CString sTmp;
#ifdef SM_MODIFY_CODE__

	m_ctrlVfMeasureCheckGrid.SetRow(nIndex);
	m_ctrlVfMeasureCheckGrid.SetCol(3);
	//m_ctrlVfMeasureCheckGrid.SetColDataType(6, flexDTString);
	m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 2, sMeasure);
	if (bResult) {
		sTmp = _T("OK");
		m_ctrlVfMeasureCheckGrid.SetCellBackColor(RGB(0, 128, 64));
		m_ctrlVfMeasureCheckGrid.SetCellForeColor(RGB(255, 255, 255));
		
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 3, sTmp);
	}
	else {
		sTmp = _T("NG");
		m_ctrlVfMeasureCheckGrid.SetCellBackColor(RGB(255, 0, 0));
		m_ctrlVfMeasureCheckGrid.SetCellForeColor(RGB(255, 255, 255));
		//m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 3, sMeasure);
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 3, sTmp);
	}

	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();
#else
	m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 5, sMeasure);

	m_ctrlVfMeasureCheckGrid.SetRow(nIndex);
	m_ctrlVfMeasureCheckGrid.SetCol(6);
	m_ctrlVfMeasureCheckGrid.SetColDataType(6, flexDTString);

	if (bResult) {
		sTmp = _T("OK");
		m_ctrlVfMeasureCheckGrid.SetCellBackColor(RGB(0, 128, 64));
		m_ctrlVfMeasureCheckGrid.SetCellForeColor(RGB(255, 255, 255));
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 6, sTmp);
	}
	else {
		sTmp = _T("NG");
		m_ctrlVfMeasureCheckGrid.SetCellBackColor(RGB(255, 0, 0));
		m_ctrlVfMeasureCheckGrid.SetCellForeColor(RGB(255, 255, 255));
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 6, sTmp);
	}

	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();
#endif
}

BOOL  CVFDischargeDlg::VfMeasureCheck()
{
	POSITION	PosStep_EM;

	BOOL		bRev = FALSE;
	BOOL		bRun;
	UINT		nChNo;

	double dVol = 0.0;
	int nTestIndex = 0;
	CString sMeasur;

	if (!gVF1000Ctrl.m_bPortOpen) {
		AfxMessageBox(_T("[VF1000] Failed to open comport!"));

		return FALSE;
	}

	if (CurrentSet->VF_TestInfo.GetCount() == 0) {
		AfxMessageBox(_T("Failed to load VF_TestInfo."));

		return FALSE;
	}

	m_bTestRunning = TRUE;

	PosStep_EM = CurrentSet->VF_TestInfo.GetHeadPosition();

	m_nNoFailed = 0;

	while (PosStep_EM != NULL)
	{

		if (CurrentSet->bRunAbort)
		{
			m_bTestRunning = FALSE;
			return FALSE;
		}

		pCurStep_EM = CurrentSet->VF_TestInfo.GetNext(PosStep_EM);
		bRun = pCurStep_EM->bRun;
		if (bRun) {
			nChNo = pCurStep_EM->nChNo;

			if (nChNo < 43) {
				bRev = gVF1000Ctrl.GetVoltageData(nChNo, dVol);
			}
			else {
				continue;
			}

			if ((pCurStep_EM->nCheckType == 1) && (pCurStep_EM->dLowerLimit >= 0) && (pCurStep_EM->dLowerLimit <= 0.5))
			{
				continue;
			}

			if (bRev) {
#ifdef SM_MODIFY_CODE__
				if (fabs(dVol) < 1.5) {
					pCurStep_EM->bResult = TRUE;
				}
				else {
					pCurStep_EM->bResult = FALSE;
				}

#else
				if (fabs(dVol) < 1.0) {
					pCurStep_EM->bResult = TRUE;
				}
				else {
					pCurStep_EM->bResult = FALSE;
				}
#endif
			}
			else {
				pCurStep_EM->bResult = FALSE;
			}
			sMeasur.Format(_T("%.1f"), dVol);
		}
		if (!pCurStep_EM->bResult) { m_nNoFailed++; }

		nTestIndex++;
		UpdateResult(nTestIndex, sMeasur, pCurStep_EM->bResult);
	}
	m_ctrlVfMeasureCheckGrid.SetRow(-1);
	m_ctrlVfMeasureCheckGrid.SetCol(-1);
	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();

	m_bTestRunning = FALSE;

	return TRUE;
}


void CVFDischargeDlg::OnClickedBtnEnter()
{
	CString sMsg;
	if (CurrentSet->bIsRunning)
	{
		CurrentSet->bRunAbort = TRUE;
		sMsg.LoadString(IDS_USER_BREAK);
		AddStringToStatus(sMsg);

		if (!m_bTestRunning) {
			OnOK();
		}
		else {
			Sleep(1500);
			OnOK();
		}
	}
}


BOOL CVFDischargeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);

	int nLeft = g_pView->m_FormViewRect.left + (g_pView->m_FormViewRect.Width() / 2) - (rect.Width() / 2);
	int nTop = (g_pView->m_FormViewRect.Height() / 2) - (rect.Height() / 2);
	int nWidth = rect.Width();
	int nHeight = rect.Height();

	MoveWindow(nLeft, nTop, nWidth, nHeight);

	m_bVfCheckResult = FALSE;

	InitVfMeasureCheckGrid();
	ConfigDisplay();


	m_bThredStop = FALSE;
	m_pMeasureThread = AfxBeginThread(VolMeasThread, this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


UINT VolMeasThread(LPVOID lParam)
{
	BOOL bRev = FALSE;
	CString sBuf;
	CVFDischargeDlg* pOwner = (CVFDischargeDlg*)lParam;

	if (pOwner == NULL)
	{
		//sTemp.LoadString(IDS_THREAD_ERROR);
		sBuf = "Failed to execute thread of DischargDlg";
		AfxMessageBox(sBuf);  //" XDischargDlg의 스레드를 실행 시키는데 실패하였습니다");
		return 0;
	}

	while (!pOwner->m_bThredStop)
	{
		bRev = pOwner->VfMeasureCheck();
		if(!bRev){
			pOwner->m_bThredStop = TRUE;
			pOwner->PostMessage(WM_COMMAND, IDOK);
			break;
		}

		if (pOwner->m_nNoFailed == 0) {
			pOwner->m_bThredStop = TRUE;
			pOwner->PostMessage(WM_COMMAND, IDOK);
			break;
		}
		Sleep(0);
	}
	return 1;
}


void CVFDischargeDlg::OnOK()
{
	DWORD dwExitCode = 0;

	m_bThredStop = TRUE;

	Sleep(500);

	if (m_pMeasureThread)
	{
		TerminateThread(m_pMeasureThread->m_hThread, dwExitCode);

		m_pMeasureThread = NULL;
	}

	CDialog::OnOK();
}
