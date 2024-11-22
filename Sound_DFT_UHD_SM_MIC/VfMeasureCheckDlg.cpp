// VfMeasureCheckDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DATsys.h"
#include "VfMeasureCheckDlg.h"
#include "afxdialogex.h"
#include "Global.h"
#include "VF1000.h"

extern CDATsysView*  g_pView;

// CVfMeasureCheckDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVfMeasureCheckDlg, CDialog)

CVfMeasureCheckDlg::CVfMeasureCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVfMeasureCheckDlg::IDD, pParent)
{
	m_bVfCheckResult = FALSE;
	m_bActivate = FALSE;
	m_bTestClose = TRUE;
	m_bTestRunning = FALSE;
}

CVfMeasureCheckDlg::~CVfMeasureCheckDlg()
{
}

void CVfMeasureCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_INPUTCHECK_GRID, m_ctrlVfMeasureCheckGrid);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVfMeasureCheckDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_ENTER, &CVfMeasureCheckDlg::OnBnClickedBtnEnter)
	ON_BN_CLICKED(IDC_BTN_RECHECK, &CVfMeasureCheckDlg::OnBnClickedBtnRecheck)
	ON_MESSAGE (WM_JOYSTICK_MSG, OnProcessMessage)
	ON_BN_CLICKED(IDC_BTN_STOP, &CVfMeasureCheckDlg::OnBnClickedBtnStop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CVfMeasureCheckDlg 메시지 처리기입니다.
long CVfMeasureCheckDlg::OnProcessMessage(WPARAM wParam, LPARAM lParam)
{
	if(g_nRunningProcessNo == 1)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
				break;
			case MSG_STOP_BTN1:
				PostMessage(WM_COMMAND,IDC_BTN_STOP);
				break;
				
			case MSG_RETRY_BTN1:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDC_BTN_RECHECK);
				}
				break;

			case MSG_OK_BTN1:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;

			case MSG_NG_BTN1:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
				
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_OK_BTN2:
			case MSG_NG_BTN2:
				break;
		}
	}
	else if(g_nRunningProcessNo == 2)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
			case MSG_STOP_BTN1: 
			case MSG_RETRY_BTN1:
			case MSG_OK_BTN1:
			case MSG_NG_BTN1:
			case MSG_START_BTN2:
				break;
			case MSG_STOP_BTN2:
				PostMessage(WM_COMMAND,IDC_BTN_STOP);
				break;

			case MSG_RETRY_BTN2:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDC_BTN_RECHECK);
				}
				break;
			case MSG_OK_BTN2:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
			case MSG_NG_BTN2:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
		}
	}
	else if(g_nRunningProcessNo == 3)
	{
		switch(lParam) 
		{
			case MSG_START_BTN1:
			case MSG_STOP_BTN1: 
			case MSG_RETRY_BTN1:
			case MSG_OK_BTN1:
			case MSG_NG_BTN1:
			case MSG_START_BTN2:
			case MSG_STOP_BTN2:
			case MSG_RETRY_BTN2:
			case MSG_OK_BTN2:
			case MSG_NG_BTN2:
			case MSG_START_BTN3:
				break;
			case MSG_STOP_BTN3:
				PostMessage(WM_COMMAND,IDC_BTN_STOP);
				break;

			case MSG_RETRY_BTN3:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDC_BTN_RECHECK);
				}
				break;
			case MSG_OK_BTN3:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
			case MSG_NG_BTN3:
				if(!m_bTestRunning){
					PostMessage(WM_COMMAND, IDOK);
				}
				break;
		}
	}	return 1;
}


void CVfMeasureCheckDlg::InitVfMeasureCheckGrid()
{
	CString sTmp;
//	int nKeyItem;
	int nRow;

	char *sHeader1[] = {" ", "CH","JIG POINT", "TARGET1", "TARGET2", "MEASURE","RESULT"};
	char *sHeader3[] = {"F", "NO(CH)","JIG POINT(Freq.)", "TARGET1", "TARGET2", "MEASURE","RESULT"};

	//int nRows = CurrentSet->VF_TestInfo.GetCount();
	
	int nRows = gVF1000Ctrl.GetVF_Count();

	m_ctrlVfMeasureCheckGrid.SetRedraw(FALSE);
	m_ctrlVfMeasureCheckGrid.Clear(COleVariant(long(flexClearEverywhere)), COleVariant(long(flexClearEverything))); //flex , flexClearEverything

	m_ctrlVfMeasureCheckGrid.SetBackColorFixed(RGB(210,216,176));
	m_ctrlVfMeasureCheckGrid.SetForeColorFixed(RGB(51,102,153));
	m_ctrlVfMeasureCheckGrid.SetBackColor(RGB(247,247,231));
	m_ctrlVfMeasureCheckGrid.SetGridColor(RGB(200,200,176));

	m_ctrlVfMeasureCheckGrid.SetFixedRows(1);
	m_ctrlVfMeasureCheckGrid.SetFixedCols(1);

	m_ctrlVfMeasureCheckGrid.SetFillStyle(flexFillRepeat);
	m_ctrlVfMeasureCheckGrid.SetFontName("Arial");
	m_ctrlVfMeasureCheckGrid.SetFontSize(8);

	m_ctrlVfMeasureCheckGrid.SetCols(7);
	m_ctrlVfMeasureCheckGrid.SetRows(nRows + 1);
	m_ctrlVfMeasureCheckGrid.SetExtendLastCol(TRUE);

	for(int nCol = 0; nCol < 7 ; nCol++)
	{
		m_ctrlVfMeasureCheckGrid.SetRow(0);
		m_ctrlVfMeasureCheckGrid.SetCol(nCol);
		m_ctrlVfMeasureCheckGrid.SetText(sHeader1[nCol]);
		m_ctrlVfMeasureCheckGrid.SetFixedAlignment(nCol,flexAlignCenterCenter);

		switch(nCol)
		{
			case 0 : // Check
						m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 300); //300
						break;

			case 1 : // No
						m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol,flexAlignCenterCenter);		
	 					m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 600); //500
						break;

			case 2 : // Test Name
						m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol,flexAlignLeftCenter);
	 					m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 1900);//3100
				  		break;

			case 3 : // Target1
						m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 800);
						break;

			case 4 : // Target2
						m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 800);
				  		break;

			case 5 : // Measure
						m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol,flexAlignRightCenter);	
	 					m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 1100);
				  		break;

			case 6 : // Result
						m_ctrlVfMeasureCheckGrid.SetColAlignment(nCol,flexAlignCenterCenter);	
	 					m_ctrlVfMeasureCheckGrid.SetColWidth(nCol, 700);
				  		break;
		}
	}

	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();

}

void CVfMeasureCheckDlg::ConfigDisplay()
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

	if(CurrentSet->VF_TestInfo.GetCount() == 0){return ;}

	Pos = CurrentSet->VF_TestInfo.GetHeadPosition();

	while(Pos)
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

		if( nChNo <= 42){
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex,0, _T("V"));
			sTmp.Format(_T("%d"), nChNo);
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex,1, sTmp);
		}
		else{
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex,0, _T("F"));
			sTmp.Format(_T("%d"), nChNo - 100);
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex,1, sTmp);
		}

		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 2, strTestName);

		sTmp.Format(_T("%.1f"), dLowerLimit);
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 3, sTmp);

		if(nCheckType == 2){
			sTmp.Format(_T("%.1f"), dUpperLimit);
			m_ctrlVfMeasureCheckGrid.SetTextMatrix(nTestIndex, 4, sTmp);
		}
	}
	m_ctrlVfMeasureCheckGrid.SetRow(-1);
	m_ctrlVfMeasureCheckGrid.SetCol(-1);
	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();
}

void CVfMeasureCheckDlg::UpdateResult(int nIndex, CString sMeasure, BOOL bResult)
{
	CString sTmp;

	m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 5, sMeasure);

	m_ctrlVfMeasureCheckGrid.SetRow(nIndex);
	m_ctrlVfMeasureCheckGrid.SetCol(6);
	m_ctrlVfMeasureCheckGrid.SetColDataType(6, flexDTString);

	if(bResult){
		sTmp = _T("OK");
		m_ctrlVfMeasureCheckGrid.SetCellBackColor(RGB(0,128,64));
		m_ctrlVfMeasureCheckGrid.SetCellForeColor(RGB(255,255,255));
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 6, sTmp);
	}
	else{
		sTmp = _T("NG");
		m_ctrlVfMeasureCheckGrid.SetCellBackColor(RGB(255,0,0));
		m_ctrlVfMeasureCheckGrid.SetCellForeColor(RGB(255,255,255));
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(nIndex, 6, sTmp);
	}

	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();

}

BOOL  CVfMeasureCheckDlg::VfMeasureCheck()
{
	POSITION	PosStep_EM;

	BOOL		bRev = FALSE;
	BOOL		bRun;		
	UINT		nChNo;		
	UINT		nCheckType;	
	UINT		nMargin;
	double		dSpec1;
	double		dSpec2;
	double		dLowerLimit1;
	double		dUpperLimit1;
	double		dLowerLimit2;
	double		dUpperLimit2;
	double		dMargin = 0.2;

	double dVol = 0.0;
	double dVol2 = 0.0;
	UINT nTmp;
	int nTime = 0;
	int nWait = 0;
	int nTestIndex = 0;
	int nNoFailed = 0;
	CString sMeasur;
	CString sMessageStr;

	if(CurrentSet->VF_TestInfo.GetCount() == 0){
		AfxMessageBox(_T("Failed to load VF_TestInfo."));
		
		return FALSE;
	}

	m_bTestRunning = TRUE;
	GetDlgItem(IDC_BTN_RECHECK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ENTER)->EnableWindow(FALSE);

	PosStep_EM = CurrentSet->VF_TestInfo.GetHeadPosition();       

	while(PosStep_EM != NULL)
	{
	
		

		if(CurrentSet->bRunAbort)
		{
			m_bTestRunning = FALSE;
			GetDlgItem(IDC_BTN_RECHECK)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_ENTER)->EnableWindow(TRUE);
			return FALSE;
		}
		
		pCurStep_EM = CurrentSet->VF_TestInfo.GetNext(PosStep_EM);
		bRun = pCurStep_EM->bRun;
		


		if(bRun){
			nChNo = pCurStep_EM->nChNo;
			nCheckType = pCurStep_EM->nCheckType;
			dSpec1 = pCurStep_EM->dLowerLimit;
			dSpec2 = pCurStep_EM->dUpperLimit;
			nTime = pCurStep_EM->nTime;
			nMargin = pCurStep_EM->nMarginType;
			if ((nCheckType == 1) && (dSpec1 >= 0) && (dSpec1 <= 0.5))
			{
				continue;
			}

			if (nCheckType == 1)
			{
				if ((dSpec1 >= 0) && (dSpec1 <= 0.5))
				{
					continue;
				}
				if (nMargin == 1)
					dMargin = 0.1;
				else if (nMargin == 2)
					dMargin = 0.2;
				else if (nMargin == 3)
					dMargin = 0.3;
				else if (nMargin == 4)
					dMargin = 0.4;
				else
					dMargin = 0.2;


			}
			else
			{
				if(nChNo == 34 || nChNo ==35 || nChNo ==36 || nChNo ==104 || nChNo ==105 || nChNo ==106){
					dMargin = 0.1;
				}
				else if(nChNo ==101 || nChNo ==102 || nChNo ==103){
					dMargin = 0.3;
				}
				else{
					dMargin = 0.2;
				}
			}

			nTestIndex++;

			if (m_bVfCheckResultList[nTestIndex] == TRUE)
			{
				UpdateResult(nTestIndex, m_sVfCheckResultMesure[nTestIndex], pCurStep_EM->bResult);
				continue;
			}

			dLowerLimit1 = dSpec1 - (abs(dSpec1) * dMargin);
			dUpperLimit1 = dSpec1 + (abs(dSpec1) * dMargin);
			dLowerLimit2 = dSpec2 - (abs(dSpec2) * dMargin);
			dUpperLimit2 = dSpec2 + (abs(dSpec2) * dMargin);

#ifdef SM_MODIFY_CODE__
			for (nTmp = 0; nTmp <= CurrentSet->nNoRetry; nTmp++)
#else
			for(nTmp = 0; nTmp < CurrentSet->nNoRetry; nTmp++)
#endif
			{

				if(CurrentSet->bRunAbort)
				{
					m_bTestRunning = FALSE;
					GetDlgItem(IDC_BTN_RECHECK)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_ENTER)->EnableWindow(TRUE);
					return FALSE;
				}
				if(nCheckType == 1){
					if(nChNo < 43){
						bRev = gVF1000Ctrl.GetVoltageData(nChNo,dVol);
					}
					else{
						bRev = gVF1000Ctrl.GetFrequencyData(nChNo - 100,dVol);
					}
					if(bRev){
						pCurStep_EM->dMaesure_Min = dVol;
						pCurStep_EM->dMaesure_Max = dVol;
						if(dVol >= dLowerLimit1 && dVol <= dUpperLimit1){
							pCurStep_EM->bResult = TRUE;
						}
						else{
							pCurStep_EM->bResult = FALSE;
							
						}
					}
					else{
						pCurStep_EM->dMaesure_Min = 0.0;
						pCurStep_EM->dMaesure_Max = 0.0;
						pCurStep_EM->bResult = FALSE;
					}
					sMeasur.Format(_T("%.1f"), pCurStep_EM->dMaesure_Min);
					sMessageStr.Format(_T("Ch No: %d, Ch Name: %s, T:%.1f~%.1f, M:%.1f"), 
						nTestIndex, 
						pCurStep_EM->strTestName,
						dLowerLimit1 , dUpperLimit1,
						pCurStep_EM->dMaesure_Min);//Ch No; xxx Ch Name; xxx T;xx M;xx 
				}
				else if(nCheckType == 2){
					nWait = (int)((nTime + 1) - (nTime * 0.5)) * 1000;
					if(nChNo < 43){
						bRev = gVF1000Ctrl.GetVoltageData2(nChNo, nWait, dVol, dVol2);
					}
					else{
						bRev = gVF1000Ctrl.GetFrequencyData2(nChNo - 100, nWait, dVol, dVol2);
					}
					if(bRev){
						pCurStep_EM->dMaesure_Min = dVol;
						pCurStep_EM->dMaesure_Max = dVol2;
//						if((dVol >= dLowerLimit1 && dVol <= dUpperLimit1) && (dVol2 >= dLowerLimit2 && dVol2 <= dUpperLimit2)){
						if((dVol <= dSpec1) && (dVol2 >= dSpec2)){
							pCurStep_EM->bResult = TRUE;
						}
						else{
							pCurStep_EM->bResult = FALSE;
						}
					}
					else{
						pCurStep_EM->dMaesure_Min = 0.0;
						pCurStep_EM->dMaesure_Max = 0.0;
						pCurStep_EM->bResult = FALSE;
					}
					sMeasur.Format(_T("%.1f,%.1f"), pCurStep_EM->dMaesure_Min, pCurStep_EM->dMaesure_Max);
					sMessageStr.Format(_T("Ch No: %d, Ch Name: %s, T:%.1f~%.1f, M:%.1f~%.1f"),
						nTestIndex, 
						pCurStep_EM->strTestName,
						dSpec2, dSpec1, 
						pCurStep_EM->dMaesure_Min, pCurStep_EM->dMaesure_Max);//Ch No; xxx Ch Name; xxx T;xx M;xx 

				}
				if(pCurStep_EM->bResult){break;} 
			}
			if(!pCurStep_EM->bResult)
			{				
				g_pView->SaveMessageList(sMessageStr);
				nNoFailed++;
			} 

			m_bVfCheckResultList[nTestIndex ] = pCurStep_EM->bResult;
			m_sVfCheckResultMesure[nTestIndex] = sMeasur;
						
			UpdateResult(nTestIndex, sMeasur, pCurStep_EM->bResult);			
		}
	}
	m_ctrlVfMeasureCheckGrid.SetRow(-1);
	m_ctrlVfMeasureCheckGrid.SetCol(-1);
	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();

	m_bTestRunning = FALSE;
	GetDlgItem(IDC_BTN_RECHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ENTER)->EnableWindow(TRUE);
	SaveResult2File();

	if(nNoFailed > 0){ 
		m_bVfCheckNG_Wait = 1;
		return FALSE;
	}
	else{
		m_bVfCheckNG_Wait = 0;
		return TRUE;
	}
}

BOOL CVfMeasureCheckDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetWindowRect(&rect);

	int nLeft   = g_pView->m_FormViewRect.left + (g_pView->m_FormViewRect.Width()/2)  - (rect.Width()/2);
	int nTop    = (g_pView->m_FormViewRect.Height()/2) - (rect.Height()/2);
	int nWidth  = rect.Width();
	int nHeight = rect.Height();

	MoveWindow(nLeft,nTop,nWidth,nHeight);

	m_bVfCheckResult = FALSE;
	m_bVfCheckNG_Wait = 0;
	m_nVfCheckCount = 0;
	
	InitVfMeasureCheckGrid();
	ConfigDisplay();

	m_bActivate = TRUE;
	memset(m_bVfCheckResultList, 0, sizeof(m_bVfCheckResultList));

	SetTimer(1, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CVfMeasureCheckDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
		case 1:
		{
			KillTimer(nIDEvent);

			m_bVfCheckResult = VfMeasureCheck();
			if((m_bVfCheckResult && m_bTestClose) || (CurrentSet->bRunAbort)) {
				OnOK();	
			}

			break;
		}
	
	}
	CDialog::OnTimer(nIDEvent);
}


void CVfMeasureCheckDlg::OnBnClickedBtnEnter()
{
	//OnOK();	
}


void CVfMeasureCheckDlg::OnBnClickedBtnRecheck()
{
	CString sTmp;

	int nRows = m_ctrlVfMeasureCheckGrid.GetRows();
	for(int i = 1; i < nRows; i++){
		m_ctrlVfMeasureCheckGrid.SetRow(i);
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(i, 5, _T(""));
		m_ctrlVfMeasureCheckGrid.SetCol(6);

		m_ctrlVfMeasureCheckGrid.SetBackColor(RGB(247,247,231));
	
		m_ctrlVfMeasureCheckGrid.SetCellBackColor(RGB(247,247,231));
		m_ctrlVfMeasureCheckGrid.SetCellForeColor(RGB(0,0,0));
		m_ctrlVfMeasureCheckGrid.SetTextMatrix(i, 6, _T(""));
	}
	m_ctrlVfMeasureCheckGrid.SetRow(-1);
	m_ctrlVfMeasureCheckGrid.SetCol(-1);
	m_ctrlVfMeasureCheckGrid.SetRedraw(TRUE);
	m_ctrlVfMeasureCheckGrid.Refresh();


	m_bVfCheckResult = VfMeasureCheck();
	if((m_bVfCheckResult && m_bTestClose) || (CurrentSet->bRunAbort)) {
		OnOK();	
	}
}


void CVfMeasureCheckDlg::OnBnClickedBtnStop()
{
	CString sMsg;
	if(CurrentSet->bIsRunning)
	{
		CurrentSet->bRunAbort = TRUE;
		sMsg.LoadString(IDS_USER_BREAK);
		AddStringToStatus(sMsg);

		if(!m_bTestRunning){
			OnOK();	
		}
		else{
			Sleep(1500);
			OnOK();
		}
	}
}
void CVfMeasureCheckDlg::SaveResult2File()
{
	CString sFolderPath = _T("");
	sFolderPath  = CurrentSet->sResultFolder+ "_VF.html";  

	SaveGridToHTML(&m_ctrlVfMeasureCheckGrid, (char*)(LPCTSTR)sFolderPath);
}

void CVfMeasureCheckDlg::OnDestroy()
{
	CDialog::OnDestroy();
	m_bActivate = FALSE;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
