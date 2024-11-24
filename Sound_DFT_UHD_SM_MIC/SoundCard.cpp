// SoundCard.cpp: implementation of the CSoundCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DATsys.h"
#include "SoundCard.h"
#include "Global.h"

//+add kwmoon 080618
#include "RealMeas.h"
#include <afxmt.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern CDATsysView*  g_pView;
extern CColorStaticST		ctrlStepMeasur_A;
//+add PSH 081202
//extern CCriticalSection g_CriticalSection;
extern CRITICAL_SECTION g_cs;  //psh√ﬂ∞°

//+add 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//extern CMMLEDLABELX		ctrlLEDLabel_AudioIn;

const int BUFFER_SIZE = 8192;// 4096; // Size of the recording buffer

void CSoundCard::StartCapture(CWnd *parent, int nDeviceID)
{

	int lFound = 0;
	RefreshDeviceArray();
	if (strDeviceNameArry.GetSize() <= nDeviceID)
	{
		return;
	}
	if (m_nSoundStatus > 0)
		StopCapture();

	m_SoundIn.m_uDeviceID = nDeviceID;
	m_hSoundMainWnd = parent->m_hWnd;
	
	m_SoundIn.Start();
}
void CSoundCard::StopCapture()
{
	m_SoundIn.Stop();
	m_nSoundStatus = 0;

}

//int CSoundCard::SelectDevice(UINT nDevNum)
//{
//	int lError = 0;
//	//CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DEVICES);
//	UINT nDevices, nC1;
//	WAVEINCAPS stWIC = { 0 };
//	MMRESULT mRes;
//	CString Strtemp;
//
//	//pBox->ResetContent();
//	nDevices = waveInGetNumDevs();
//
//
//
//	m_aStrSoundDevice.RemoveAll();
//	for (nC1 = 0; nC1 < nDevices; ++nC1)
//	{
//		ZeroMemory(&stWIC, sizeof(WAVEINCAPS));
//		mRes = waveInGetDevCaps(nC1, &stWIC, sizeof(WAVEINCAPS));
//		if (mRes == 0)
//			m_aStrSoundDevice.Add(stWIC.szPname);// pBox->AddString(stWIC.szPname);
//		else
//		{
//			Strtemp.Format("File: %s ,Line Number:%d, mRes: %d", __FILE__, __LINE__, mRes);
//			TRACE(Strtemp);
//			//m_aStrSoundDevice.Add(stWIC.szPname);//// StoreError(mRes, TRUE, "File: %s ,Line Number:%d", __FILE__, __LINE__);
//		}
//	}
//
//
//	return lError;
//}


void CSoundCard::DataFromSoundIn(CBuffer* buffer, void* Owner)
{
	//((CSM_SoundControl*) Owner)->WriteSoundDataToFile(buffer);
	::SendMessage(((CSoundCard*)Owner)->m_hSoundMainWnd, WM_GRAPH_UPDATE, (WPARAM)buffer->ByteLen, (LPARAM)buffer->ptr.c);
}

//////////////////////////////////////////////////////////////////////

CSoundCard::CSoundCard()
{
	m_fTargetLeftFreq = 0;
	m_fTargetRightFreq = 0;

	m_fTargetLeftLevel = 0;
	m_fTargetRightLevel = 0;

	m_fFreqMargin = 0;
	m_fLevelMargin = 0;

	m_fMeasuredLeftFreq = 0;
	m_fMeasuredRightFreq = 0;

	m_fMeasuredLeftLevel = 0;
	m_fMeasuredRightLevel = 0;

	m_fLeftFreqLowerLimit = 0;
	m_fLeftFreqUpperLimit = 0;
	m_fRightFreqLowerLimit = 0;
	m_fRightFreqUpperLimit = 0;

	m_fLeftLevelLowerLimit = 0;
	m_fLeftLevelUpperLimit = 0;
	m_fRightLevelLowerLimit = 0;
	m_fRightLevelUpperLimit = 0;

	wIn_Flag = FALSE;
	m_bResult = FALSE;

	m_bThreadRunnuing = FALSE;
	m_bExitThread = FALSE;

	//+change 090213(Modification No1)
//	m_hThread			= NULL;
	m_pAudioMeasureThread = NULL;

	m_hAudioMeasureThreadKillEvent = NULL;

	//+add PSH 080816
//	m_bMeasureAudio = FALSE;
	//+add PSH 090305
	m_bMeasureAudioType = 0;

	m_nSoundStatus = 0;
	
	m_SoundIn.DataFromSoundIn = DataFromSoundIn;	// assign pointer to callback function
	m_SoundIn.m_pOwner = this;
	//m_SoundOut.GetDataToSoundOut = GetDataToSoundOut;  // assign pointer to callback function
	//m_SoundOut.m_pOwner = this;


}

CSoundCard::~CSoundCard()
{
	if (m_hAudioMeasureThreadKillEvent != NULL)
	{
		CloseHandle(m_hAudioMeasureThreadKillEvent); m_hAudioMeasureThreadKillEvent = NULL;
	}
	
}
CString CSoundCard::GetDeviceName(UINT lDeviceID)
{
	UINT lNumber = waveInGetNumDevs();
	CString StrDeviceName;
	
	if (lDeviceID  < lNumber) 
	{
		WAVEINCAPS wic;
		if (waveInGetDevCaps(lDeviceID, &wic, sizeof(WAVEINCAPS)) == MMSYSERR_NOERROR) {
			StrDeviceName= wic.szPname ;
		}
		else {
			AfxMessageBox( "Error retrieving device capabilities for device ");
			StrDeviceName = "";
		}
	}

	return StrDeviceName;
}

int CSoundCard::RefreshDeviceArray()
{
	int lNumber = waveInGetNumDevs();
	CString StrDeviceName;

	strDeviceNameArry.RemoveAll();
	for (int i = 0; i < lNumber; i++)
	{
		WAVEINCAPS wic;
		if (waveInGetDevCaps(i, &wic, sizeof(WAVEINCAPS)) == MMSYSERR_NOERROR)
		{
			StrDeviceName = wic.szPname;
			StrDeviceName.MakeUpper();
			strDeviceNameArry.Add(StrDeviceName);
		}
		else
		{
			CString sTemp;
			sTemp.Format("Error retrieving device capabilities for device :%d", i);
			AfxMessageBox(sTemp);
		}
	}
	

	return lNumber;
}

int CSoundCard::SetDeviceID(int lDevice)
{
	MMRESULT result;

	StartCapture(g_pView, lDevice);
	m_nDeviceID = lDevice;
	m_nSoundStatus = 1;
	return 0;
}


BOOL CSoundCard::SoundInMainSet()
{
	//	long aStatus;
	int lFindFlag = 0;
	int lAudioDeviceID[3] = { -1,-1,-1 };
	int lAudioCount = 0;

	CString StrMainMicName3System[3];
	CString StrUSBMicName3System[3];
	CString Stemp;

	int lNumber = 0;
	//CString strSoundIn = "DFT MIC";
	CString StrDeviceName;

	StrMainMicName3System[0] = g_pView->m_pUSB_MIC_Struct->sSystem1_MIC;
	StrMainMicName3System[1] = g_pView->m_pUSB_MIC_Struct->sSystem2_MIC;
	StrMainMicName3System[2] = g_pView->m_pUSB_MIC_Struct->sSystem3_MIC;

	StrUSBMicName3System[0] = g_pView->m_pUSB_MIC_Struct->sSystem1_USBMIC;
	StrUSBMicName3System[1] = g_pView->m_pUSB_MIC_Struct->sSystem1_USBMIC;
	StrUSBMicName3System[2] = g_pView->m_pUSB_MIC_Struct->sSystem1_USBMIC;
	
	lNumber = RefreshDeviceArray();	

	if (CurrentSet->sMainMIC_Name.GetLength() > 5)
	{
		for (int i = 0; i < lNumber; i++)
		{
			StrDeviceName = strDeviceNameArry.GetAt(i);
			StrDeviceName.MakeUpper();

			if ((StrDeviceName.Find(CurrentSet->sMainMIC_Name) >= 0) && (CurrentSet->sMainMIC_Name.GetLength() > 10))
			{
				m_nDeviceID = i;
			
				lFindFlag = 1;
				break;
			}
		}
	}
	else
	{
		lFindFlag = 0;
	}


	if (lFindFlag == 1)
	{
		SetDeviceID(m_nDeviceID);
	}
	else//"NA"		
	{
		if (g_nRunningProcessNo == 1)
		{

			if (lNumber >= 1)   //					if (ctrlWaveIn.GetNumdevs() >= 1) {
			{
				SetDeviceID(0);
				m_nDeviceID = 0;
				CurrentSet->sMainMIC_Name = strDeviceNameArry.GetAt(0);

			}
			else {
				//SetDeviceID(-1);
				m_nDeviceID = -1;
				CurrentSet->sMainMIC_Name = "N/A";
			}

		}
		else if (g_nRunningProcessNo == 2) {
			//nDev = ctrlWaveIn.GetNumdevs();
			if (lNumber >= 2) {
				SetDeviceID(1);
				m_nDeviceID = 1;
				CurrentSet->sMainMIC_Name = strDeviceNameArry.GetAt(0);
			}
			else {
				//SetDeviceID(-1);
				m_nDeviceID = -1;
				CurrentSet->sMainMIC_Name = "N/A";
			}		
		}
		else {
			if (lNumber >= 3) {
				//SetDeviceID(2);
				m_nDeviceID = 2;
				CurrentSet->sMainMIC_Name = strDeviceNameArry.GetAt(0);
			}
			else {
				//SetDeviceID(-1);
				m_nDeviceID = -1;

				CurrentSet->sMainMIC_Name = "N/A";
			}			
		}
	}

	SaveShareSoundData();

	for (int i = 0; i < 3; i++)
	{
		if (((CurrentSet->sMainMIC_Name.Find(StrMainMicName3System[i]) >= 0) && (StrMainMicName3System[i].GetLength() > 5)
			&& (g_nRunningProcessNo) != i + 1))
		{
			Stemp.Format("Same Sound Device Selected DFT%d!! \r\n[", i + 1);
			Stemp += CurrentSet->sMainMIC_Name;
			Stemp += "]";
			AddStringToStatus(Stemp);
			AfxMessageBox(Stemp);
		}
	}
	if ((m_nDeviceID == -1)||(m_nSoundStatus == 0))
	{
		return  0;
	}
	else
	{
		Stemp.Format("Sound Set :[%d]", m_nDeviceID);
		Stemp += CurrentSet->sMainMIC_Name;
		//Stemp += "\";
		AddStringToStatus(Stemp);
		return 1;
	}
}

/* // Start recording
    if (waveInStart(hWaveIn) != MMSYSERR_NOERROR) {
        std::cerr << "Failed to start recording." << std::endl;
        waveInClose(hWaveIn);
        delete[] waveHeader.lpData;
        return 1;
    }
*/
int CSoundCard::Wave_Start()
{	
	g_pView->m_Sound_Display_Enable = 1;
	m_nSoundStatus = 2;
	return 1;
}


void CSoundCard::WaveRead_Start()
{
	//	long aStatus;
	int lFindFlag = 0;
	int lAudioDeviceID[3] = { -1,-1,-1 };
	int lAudioCount = 0;

	if (m_nSoundUSBDeviceFind == 1)
	{
		WaveRead_StartUSB("USB");
		return;
	}
	g_SoundCard.MicSearchRelease(); //g_pView->m_pUSB_MIC_Struct->nNewMicRequest = 0;
	lFindFlag = SoundInMainSet();
	wIn_Flag = FALSE;
	if (lFindFlag == 1)
	{
		if (Wave_Start())
		{
			
			wIn_Flag = TRUE;
		}		
	}

	if(wIn_Flag == FALSE)
	{
		if (g_nRunningProcessNo == 1) {
			AddStringToStatus("Sorry, No 1 WaveIn device installed!");
			wIn_Flag = FALSE;
			return;
		}
		else if (g_nRunningProcessNo == 2) {

			AddStringToStatus("Sorry, No 2 WaveIn device installed!");
			wIn_Flag = FALSE;
			return;
		}
		else {
			AddStringToStatus("Sorry, No 3 WaveIn device installed!");
			wIn_Flag = FALSE;
			return;
		}
	}
}

void CSoundCard::SaveShareSoundData()
{
	if (g_nRunningProcessNo == 1)
	{
		strncpy(g_pView->m_pUSB_MIC_Struct->sSystem1_USBMIC, m_sUSBDeviceName.GetBuffer(),
			m_sUSBDeviceName.GetLength() < 20
			? m_sUSBDeviceName.GetLength()
			: 20);
		strncpy(g_pView->m_pUSB_MIC_Struct->sSystem1_MIC, CurrentSet->sMainMIC_Name.GetBuffer(),
			CurrentSet->sMainMIC_Name.GetLength() < 20
			? CurrentSet->sMainMIC_Name.GetLength()
			: 20);
	}
	else if (g_nRunningProcessNo == 2)
	{
		strncpy(g_pView->m_pUSB_MIC_Struct->sSystem2_USBMIC, m_sUSBDeviceName.GetBuffer(),
			m_sUSBDeviceName.GetLength() < 20
			? m_sUSBDeviceName.GetLength()
			: 20);
		strncpy(g_pView->m_pUSB_MIC_Struct->sSystem2_MIC, CurrentSet->sMainMIC_Name.GetBuffer(),
			CurrentSet->sMainMIC_Name.GetLength() < 20
			? CurrentSet->sMainMIC_Name.GetLength()
			: 20);
	}
	else
	{
		strncpy(g_pView->m_pUSB_MIC_Struct->sSystem3_USBMIC, m_sUSBDeviceName.GetBuffer(),
			m_sUSBDeviceName.GetLength() < 20
			? m_sUSBDeviceName.GetLength()
			: 20);
		strncpy(g_pView->m_pUSB_MIC_Struct->sSystem3_MIC, CurrentSet->sMainMIC_Name.GetBuffer(),
			CurrentSet->sMainMIC_Name.GetLength() < 20
			? CurrentSet->sMainMIC_Name.GetLength()
			: 20);
	}

}

BOOL CSoundCard::WaveRead_StartUSB(CString strSoundIn, int Fixname)
{
	//	long aStatus;
	int lFlagCheck = 0;
	int lNumber = 0;
	CString StrDeviceName;
	CString StrNewSelectName;
	int lFindFlag = 0;


	
	if (Fixname == 1)
	{
		StrNewSelectName = strSoundIn;
		m_nSoundUSBDeviceFind = 0;

	}
	else
	{
		if (!WaveCheck_Search(1))
		{
			m_nSoundUSBDeviceFind = 0;
			m_nUSBDeviceID = -1;
			m_sUSBDeviceName = "N/A";
			return 0;
		}
		m_nSoundUSBDeviceFind = 1;

		MicSearchRelease();// g_pView->m_pUSB_MIC_Struct->nNewMicRequest = 0;
		StrNewSelectName = m_sUSBDeviceName;
		SaveShareSoundData();
	}

	lNumber = RefreshDeviceArray();
	if (StrNewSelectName.GetLength() > 5)
	{
		for (int i = 0; i < lNumber; i++)
		{
			StrDeviceName = strDeviceNameArry.GetAt(i);
			StrDeviceName.MakeUpper();
			StrNewSelectName.MakeUpper();

			if ((StrDeviceName.Find(StrNewSelectName) >= 0) && (StrNewSelectName.GetLength() > 10))
			{
				m_nDeviceID = i;				
				lFindFlag = 1;
				break;
			}
		}
	}
	else
	{
		lFindFlag = 0;
	}


	if (lFindFlag == 1)
	{
		SetDeviceID(m_nDeviceID);
	}

	if ((m_nDeviceID >= 0) && (m_nDeviceID < lNumber))// && (m_nSoundUSBDeviceFind == 1))
	{
		CString Stemp;
		Stemp.Format("MIC Sound Set :[%d]", m_nDeviceID);
		Stemp += m_sUSBDeviceName;
		AddStringToStatus(Stemp);

		Wave_Start();
		wIn_Flag = TRUE;
	}
	else
	{
		AfxMessageBox("Start Error, No USB WaveIn device installed!", MB_ICONERROR + MB_OK, 0);
		wIn_Flag = FALSE;

	}

	return wIn_Flag;
}

BOOL CSoundCard::WaveCheck_Search(int Search)
{
	//	long aStatus;
		//int lFlagCheck = 0;
	int lNumber = 0;
	CString StrDeviceName;
	CString StrOldDeviceName;
	CString StrMainMicName3System[3];
	CString StrUSBMicName3System[3];

	
	lNumber = waveInGetNumDevs(); //

	if (Search == 0)
	{
		m_nSoundUSBDeviceFind = 0;
		m_sUSBDeviceName = "N/A";
		m_nUSBDeviceID = -1;
		RefreshDeviceArray();		
		strSavedDeviceNameArry.RemoveAll();
		for (int i = 0; i < lNumber; i++)
		{
			StrDeviceName = strDeviceNameArry.GetAt(i);
			//strDeviceNameArry.Add(StrDeviceName);
			strSavedDeviceNameArry.Add(StrDeviceName);
		}
		
		m_nDeviceCount = lNumber;

		CString Stemp;
		Stemp.Format("Ready to Check New Sound Input!!");
		AddStringToStatus(Stemp);
	}
	else
	{
		StrMainMicName3System[0] = g_pView->m_pUSB_MIC_Struct->sSystem1_MIC;
		StrMainMicName3System[1] = g_pView->m_pUSB_MIC_Struct->sSystem2_MIC;
		StrMainMicName3System[2] = g_pView->m_pUSB_MIC_Struct->sSystem3_MIC;

		StrUSBMicName3System[0] = g_pView->m_pUSB_MIC_Struct->sSystem1_USBMIC;
		StrUSBMicName3System[1] = g_pView->m_pUSB_MIC_Struct->sSystem1_USBMIC;
		StrUSBMicName3System[2] = g_pView->m_pUSB_MIC_Struct->sSystem1_USBMIC;

		int lCheck = 0;
		for (int i = 0; i < lNumber; i++)
		{

			lCheck = 0;
			StrDeviceName = GetDeviceName(i);			
			for (int j = 0; j < strSavedDeviceNameArry.GetSize(); j++)
			{
				StrOldDeviceName = strSavedDeviceNameArry.GetAt(j);
				if (StrDeviceName.Find(StrOldDeviceName) >= 0)
				{
					lCheck = 1;
					break;
				}
			}
			if (lCheck == 1)
				continue;

			for (int j = 0; j < 3; j++)
			{
				if (g_nRunningProcessNo == j + 1)
				{
					continue;
				}

				if (((StrDeviceName.Find(StrMainMicName3System[j]) >= 0) && (StrMainMicName3System[j].GetLength() >= 5))
					|| ((StrDeviceName.Find(StrUSBMicName3System[j]) >= 0)) && (StrUSBMicName3System[j].GetLength() >= 5))
				{
					lCheck = 1;
					break;
				}
			}
			if (lCheck == 1)
				continue;

			m_nSoundUSBDeviceFind = 1;
			m_sUSBDeviceName = StrDeviceName;
			m_nUSBDeviceID = i;
			break;
		}
	}

	return  m_nSoundUSBDeviceFind;
}

void CSoundCard::WaveRead_Stop()
{

	if (!wIn_Flag) return;

	StopCapture();

	wIn_Flag = FALSE;
	g_pView->m_Sound_Display_Enable = 0;

}

int  CSoundCard::IsMicSearchFree()
{
	if (g_nRunningProcessNo == 1)
	{
		if (g_pView->m_pUSB_MIC_Struct->nNewMicRequest2 || g_pView->m_pUSB_MIC_Struct->nNewMicRequest3)
		{
			return 0;
		}
	}
	else if (g_nRunningProcessNo == 2)
	{
		if (g_pView->m_pUSB_MIC_Struct->nNewMicRequest1 || g_pView->m_pUSB_MIC_Struct->nNewMicRequest3)
		{
			return 0;
		}
	}
	else if (g_nRunningProcessNo == 3)
	{
		if (g_pView->m_pUSB_MIC_Struct->nNewMicRequest1 || g_pView->m_pUSB_MIC_Struct->nNewMicRequest2)
		{
			return 0;
		}
	}

	return 1;
}

void CSoundCard::MicSearchBlock()
{
	if (g_nRunningProcessNo == 1)
		g_pView->m_pUSB_MIC_Struct->nNewMicRequest1 = 1;
	else if (g_nRunningProcessNo == 2)
		g_pView->m_pUSB_MIC_Struct->nNewMicRequest2 = 1;
	else if (g_nRunningProcessNo == 3)
		g_pView->m_pUSB_MIC_Struct->nNewMicRequest3 = 1;
}

void CSoundCard::MicSearchRelease(int all_ch)
{
	if (all_ch)
	{
		g_pView->m_pUSB_MIC_Struct->nNewMicRequest1 = 0;
		g_pView->m_pUSB_MIC_Struct->nNewMicRequest2 = 0;
		g_pView->m_pUSB_MIC_Struct->nNewMicRequest3 = 0;
	}
	else
	{
		if (g_nRunningProcessNo == 1)
			g_pView->m_pUSB_MIC_Struct->nNewMicRequest1 = 0;
		else if (g_nRunningProcessNo == 2)
			g_pView->m_pUSB_MIC_Struct->nNewMicRequest2 = 0;
		else if (g_nRunningProcessNo == 3)
			g_pView->m_pUSB_MIC_Struct->nNewMicRequest3 = 0;
	}

}
BOOL CSoundCard::Creat(CWnd* pWnd)
{
	long nDev;

	//ctrlWaveIn.SubclassDlgItem(IDC_MMWAVEINX1, pWnd);

	//ctrlSpectrum_L.SubclassDlgItem(IDC_MMSPECTRUMX1, pWnd);
	//ctrlSpectrum_R.SubclassDlgItem(IDC_MMSPECTRUMX2, pWnd);

	//ctrlLED_inL.SubclassDlgItem(IDC_MMLEDPANELX_INL, pWnd);
	//ctrlLED_inR.SubclassDlgItem(IDC_MMLEDPANELX_INR, pWnd);

	//ctrlLED_inL_Level.SubclassDlgItem(IDC_MMLEDPANELX_INL_LEVEL, pWnd);
	//ctrlLED_inR_Level.SubclassDlgItem(IDC_MMLEDPANELX_INR_LEVEL, pWnd);

	//ctrlPeakDetect.SubclassDlgItem(IDC_MMPEAKDETECTX1, pWnd);
	
	m_hSoundMainWnd = pWnd->m_hWnd;

	//if (g_pView->nMicLive == 3)
	//{
	//	SoundInMainSet();

	//}
	//else
	//{
	//	if (g_nRunningProcessNo == 1) {
	//		//+add PSH 080527
	//		nDev = waveInGetNumDevs();
	//		if (nDev >= 1) {
	//			SetDeviceID(0);
	//			m_nDeviceID = 0;

	//		}
	//		else {
	//			SetDeviceID(-1);
	//		}
	//	}
	//	else if (g_nRunningProcessNo == 2) {
	//		nDev = waveInGetNumDevs();
	//		if (nDev >= 2) {
	//			SetDeviceID(1);
	//			m_nDeviceID = 1;
	//		}
	//		else {
	//			SetDeviceID(-1);
	//		}
	//	}
	//	else {
	//		nDev = waveInGetNumDevs();
	//		if (nDev >= 3) {
	//			SetDeviceID(2);
	//			m_nDeviceID = 2;
	//		}
	//		else {
	//			SetDeviceID(-1);
	//		}
	//	}
	//}
	m_hAudioMeasureThreadKillEvent = ::CreateEvent(NULL, false, false, NULL);

	return TRUE;
}

void CSoundCard::GetWavePeak_L(double *freq, double *ampl, double *dbampl)
{
	if (!wIn_Flag) return;
	//ctrlSpectrum_L.GetPeak(freq, ampl, dbampl);
}

void CSoundCard::GetWavePeak_R(double *freq, double *ampl, double *dbampl)
{
	if (!wIn_Flag) return;
	//ctrlSpectrum_R.GetPeak(freq, ampl, dbampl);
}

void CSoundCard::SetLedValue_inL(long value)
{
	if (!wIn_Flag) return;
	//ctrlLED_inL.SetValue(value);
}

void CSoundCard::SetLedValue_inR(long value)
{
	if (!wIn_Flag) return;
	//ctrlLED_inR.SetValue(value);
}

void CSoundCard::SetLedValue_inL_Level(long value)
{
	if (!wIn_Flag) return;
	//ctrlLED_inL_Level.SetValue(value);
}

void CSoundCard::SetLedValue_inR_Level(long value)
{
	if (!wIn_Flag) return;
	//ctrlLED_inR_Level.SetValue(value);
}

void CSoundCard::GetPeackDetectorPeak_R(double *pLevel)
{
	if (!wIn_Flag) return;
	//*pLevel = ctrlPeakDetect.GetPeakRight();
}

void CSoundCard::GetPeackDetectorPeak_L(double *pLevel)
{
	if (!wIn_Flag) return;
	//*pLevel = ctrlPeakDetect.GetPeakLeft();
}

//+changed kwmoon 080514
//UINT CSoundCard::StartAudioMeasureThread(LPVOID pParam)
//{
//	CSoundCard* pSoundCard = (CSoundCard*)pParam;
//
//	CString sTemp;
//
//	int aResult[4] = { 0,0,0,0 };
//	int nNoMeasureRepeat = 0;
//
//	float fLowerMargin = (float)(1 - (pSoundCard->m_fFreqMargin*0.01));
//	float fUpperMargin = (float)(1 + (pSoundCard->m_fFreqMargin*0.01));
//
//	//+add kwmoon 080716
//	float fLevelLowerMargin = (float)(1 - (pSoundCard->m_fLevelMargin * 0.01));
//	//+add kwmoon 080819
//	float fLevelUpperMargin = (float)(1 + (pSoundCard->m_fLevelMargin * 0.01));
//
//	pSoundCard->m_fMeasuredLeftFreq = 0;
//	pSoundCard->m_fMeasuredRightFreq = 0;
//
//	pSoundCard->m_fMeasuredLeftLevel = 0;
//	pSoundCard->m_fMeasuredRightLevel = 0;
//
//	pSoundCard->m_bResult = FALSE;
//	pSoundCard->m_bExitThread = FALSE;
//
//	pSoundCard->m_fLeftFreqLowerLimit = pSoundCard->m_fTargetLeftFreq * fLowerMargin;
//	pSoundCard->m_fLeftFreqUpperLimit = pSoundCard->m_fTargetLeftFreq * fUpperMargin;
//
//	pSoundCard->m_fRightFreqLowerLimit = pSoundCard->m_fTargetRightFreq * fLowerMargin;
//	pSoundCard->m_fRightFreqUpperLimit = pSoundCard->m_fTargetRightFreq * fUpperMargin;
//
//	/*	//+change kwmoon 080716
//	//	pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLowerMargin;
//		pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLevelLowerMargin;
//
//		//+del kwmoon 080716
//	//	pSoundCard->m_fLeftLevelUpperLimit	= pSoundCard->m_fTargetLeftLevel * fUpperMargin;
//
//		//+change kwmoon 080716
//	//	pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLowerMargin;
//		pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLevelLowerMargin;
//
//		//+del kwmoon 080716
//	//	pSoundCard->m_fRightLevelUpperLimit	= pSoundCard->m_fTargetRightLevel * fUpperMargin;
//	*/
//	//+del kwmoon 080828
//	//+change PSH 080816
////	if(pSoundCard->m_bMeasureAudio)
//	{
//		pSoundCard->m_fLeftLevelLowerLimit = pSoundCard->m_fTargetLeftLevel * fLevelLowerMargin;
//		pSoundCard->m_fRightLevelLowerLimit = pSoundCard->m_fTargetRightLevel * fLevelLowerMargin;
//
//	}
//	//+del kwmoon 080828
///*	else
//	{
//		//+change kwmoon 080819
//	//	pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLowerMargin;
//	//	pSoundCard->m_fLeftLevelUpperLimit	= pSoundCard->m_fTargetLeftLevel * fUpperMargin;
//
//	//	pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLowerMargin;
//	//	pSoundCard->m_fRightLevelUpperLimit	= pSoundCard->m_fTargetRightLevel * fUpperMargin;
//
//		pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLevelLowerMargin;
//		pSoundCard->m_fLeftLevelUpperLimit	= pSoundCard->m_fTargetLeftLevel * fLevelUpperMargin;
//
//		pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLevelLowerMargin;
//		pSoundCard->m_fRightLevelUpperLimit	= pSoundCard->m_fTargetRightLevel * fLevelUpperMargin;
//
//	}
//*/
//	::ResetEvent(pSoundCard->m_hAudioMeasureThreadKillEvent);
//
//	//090615
//	//if(!pSoundCard->wIn_Flag){ pSoundCard->WaveRead_Start(); _Wait(100); }
//	g_pView->AudioMeasureStart();
//
//	double freq_L, freq_R;
//	double ampl_L, ampl_R;
//	double dbampl_L, dbampl_R;
//
//	double Avgfreq_L, Avgfreq_R;
//	double Avgampl_L, Avgampl_R;
//	double Avgdbampl_L, Avgdbampl_R;
//
//	int nNoMeasureAudioFreqRetry = 0;//MAX_NO_MEASURE_AUDIO_RETRY;
//	int nNoMeasureAudioLevelRetry = 0;//MAX_NO_MEASURE_AUDIO_RETRY;
//	int nNoMeasureAudioResetRetry = 0;//MAX_NO_MEASURE_AUDIO_RETRY;
//
//	double nLevel_L, nLevel_R;
//	double nAvgLevel_L, nAvgLevel_R;
//	double nAmplitude_L, nAmplitude_R;
//	double nCorrectionValue = 0.97;
//
//	CString szMsg;
//
//	int nNoRetry = 0;
//
//MEASURE_LEVEL_START:
//
//	nLevel_L = 0;
//	nLevel_R = 0;
//	nAvgLevel_L = 0;
//	nAvgLevel_R = 0;
//	nAmplitude_L = 0;
//	nAmplitude_R = 0;
//	nCorrectionValue = 0.97;
//
//	//	if(nNoMeasureAudioLevelRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	//	{
//	//		szMsg.Format("Retry Check Audio Level (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioLevelRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//	//		ShowSubStepMessage(szMsg, "Retry Audio Level Check");
//	//	}
//
//	//	if(nNoMeasureAudioResetRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	//	{
//	//		szMsg.Format("Reset Audio Device (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioResetRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//	//		ShowSubStepMessage(szMsg, "Retry Audio Device Reset");
//	//	}
//
//	for (nNoMeasureRepeat = 0; nNoMeasureRepeat < 3; nNoMeasureRepeat++)
//	{
//		if (pSoundCard->m_bExitThread)
//		{
//			goto END_THREAD;
//		}
//
//		_Wait(100);
//		pSoundCard->GetPeackDetectorPeak_L(&nLevel_L);
//		pSoundCard->GetPeackDetectorPeak_R(&nLevel_R);
//		_Wait(50);
//
//		nAvgLevel_L += nLevel_L;
//		nAvgLevel_R += nLevel_R;
//	}
//
//	nAvgLevel_L = (double)(nAvgLevel_L / nNoMeasureRepeat);
//	nAvgLevel_R = (double)(nAvgLevel_R / nNoMeasureRepeat);
//
//	if ((nAvgLevel_L < (double)MIN_AUDIO_LEVEL)
//		|| (nAvgLevel_R < (double)MIN_AUDIO_LEVEL))
//	{
//		if (nNoMeasureAudioResetRetry > 0)
//		{
//			nNoMeasureAudioResetRetry--;
//			pSoundCard->WaveRead_Stop(); // _Wait(1000); 
//			pSoundCard->WaveRead_Start(); //_Wait(1000);
//			goto MEASURE_LEVEL_START;
//		}
//	}
//
//	nAmplitude_L = nAvgLevel_L / (2 * 1.4142)*0.1*nCorrectionValue;
//	nAmplitude_R = nAvgLevel_R / (2 * 1.4142)*0.1*nCorrectionValue;
//
//	pSoundCard->SetLedValue_inL_Level((long)nAmplitude_L);
//	pSoundCard->SetLedValue_inR_Level((long)nAmplitude_R);
//
//	pSoundCard->m_fMeasuredLeftLevel = (float)nAmplitude_L;
//	pSoundCard->m_fMeasuredRightLevel = (float)nAmplitude_R;
//
//	sTemp.Format(_T("Level:%d, %d"), (int)nAmplitude_L, (int)nAmplitude_R);
//
//	ctrlStepMeasur_A.SetWindowText(sTemp);
//	ctrlStepMeasur_A.Invalidate();
//	ctrlStepMeasur_A.UpdateWindow();
//
//	/*
//	//+change kwmoon 080716
////	if((pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
////	&& (pSoundCard->m_fMeasuredLeftLevel <= (pSoundCard->m_fLeftLevelUpperLimit)))
//	if(pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
//	{
//		aResult[2] = TEST_PASS;
//	}
//	else
//	{
//		aResult[2] = TEST_FAIL;
////		nNoRetry = nNoMeasureAudioLevelRetry--;
//
////		if(nNoRetry > 0){ _Wait(500); goto MEASURE_LEVEL_START; }
//	}
//
//	//+change kwmoon 080716
////	if((pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
////	&& (pSoundCard->m_fMeasuredRightLevel <= (pSoundCard->m_fRightLevelUpperLimit)))
//	if(pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
//	{
//		aResult[3] = TEST_PASS;
//	}
//	else
//	{
//		aResult[3] = TEST_FAIL;
////		nNoRetry = nNoMeasureAudioLevelRetry--;
//
////		if(nNoRetry > 0){ _Wait(500); goto MEASURE_LEVEL_START; }
//	}
//*/
////+del kwmoon 080828
////+change PSH 080816
////	if(pSoundCard->m_bMeasureAudio)
//	{
//		if (pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
//		{
//			aResult[2] = TEST_PASS;
//		}
//		else
//		{
//			aResult[2] = TEST_FAIL;
//		}
//		if (pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
//		{
//			aResult[3] = TEST_PASS;
//		}
//		else
//		{
//			aResult[3] = TEST_FAIL;
//		}
//	}
//	//+del kwmoon 080828
///*	else
//	{
//		if((pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
//		&& (pSoundCard->m_fMeasuredLeftLevel <= (pSoundCard->m_fLeftLevelUpperLimit)))
//		{
//			aResult[2] = TEST_PASS;
//		}
//		else
//		{
//			aResult[2] = TEST_FAIL;
//		}
//
//		if((pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
//		&& (pSoundCard->m_fMeasuredRightLevel <= (pSoundCard->m_fRightLevelUpperLimit)))
//		{
//			aResult[3] = TEST_PASS;
//		}
//		else
//		{
//			aResult[3] = TEST_FAIL;
//		}
//
//	}
//*/
//	if ((aResult[2] == TEST_FAIL) || (aResult[3] == TEST_FAIL))
//	{
//		if (nNoMeasureAudioLevelRetry++ < MAX_NO_MEASURE_AUDIO_RETRY)
//		{
//			szMsg.Format("Retry Check Level (%d/%d)", nNoMeasureAudioLevelRetry, MAX_NO_MEASURE_AUDIO_RETRY);
//			ShowSubStepMessage(szMsg, "Retry Audio Level Check");
//
//			//+change kwmoon 080618
//		//	_Wait(500); 
//			_Wait(200);
//
//			goto MEASURE_LEVEL_START;
//		}
//	}
//
//MEASURE_FREQ_START:
//
//	freq_L = 0.0;
//	freq_R = 0.0;
//	ampl_L = 0.0;
//	ampl_R = 0.0;
//	dbampl_L = 0.0;
//	dbampl_R = 0.0;
//
//	Avgfreq_L = 0.0;
//	Avgfreq_R = 0.0;
//	Avgampl_L = 0.0;
//	Avgampl_R = 0.0;
//	Avgdbampl_L = 0.0;
//	Avgdbampl_R = 0.0;
//
//	//	if(nNoMeasureAudioFreqRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	//	{
//	//		szMsg.Format("Retry Check Audio Freq (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioFreqRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//	//		ShowSubStepMessage(szMsg, "Retry Audio Freq Check");
//	//	}
//
//	//	if(nNoMeasureAudioResetRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	//	{
//	//		szMsg.Format("Reset Audio Device (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioResetRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//	//		ShowSubStepMessage(szMsg, "Retry Audio Device Reset");
//	//	}
//
//	for (nNoMeasureRepeat = 0; nNoMeasureRepeat < 3; nNoMeasureRepeat++)
//	{
//		if (pSoundCard->m_bExitThread)
//		{
//			goto END_THREAD;
//		}
//
//		_Wait(50);
//
////		pSoundCard->ctrlSpectrum_L.Invalidate();
//		pSoundCard->GetWavePeak_L(&freq_L, &ampl_L, &dbampl_L);
//
//	//	pSoundCard->ctrlSpectrum_R.Invalidate();
//		pSoundCard->GetWavePeak_R(&freq_R, &ampl_R, &dbampl_R);
//
//		_Wait(50);
//
//		Avgfreq_L += freq_L;
//		Avgampl_L += ampl_L;
//		Avgdbampl_L += dbampl_L;
//
//		Avgfreq_R += freq_R;
//		Avgampl_R += ampl_R;
//		Avgdbampl_R += dbampl_R;
//	}
//
//	Avgfreq_L = (double)(Avgfreq_L / nNoMeasureRepeat);
//	Avgampl_L = (double)(Avgampl_L / nNoMeasureRepeat);
//	Avgdbampl_L = (double)(Avgdbampl_L / nNoMeasureRepeat);
//
//	Avgfreq_R = (double)(Avgfreq_R / nNoMeasureRepeat);
//	Avgampl_R = (double)(Avgampl_R / nNoMeasureRepeat);
//	Avgdbampl_R = (double)(Avgdbampl_R / nNoMeasureRepeat);
//
//	if ((Avgfreq_L < (double)MIN_AUDIO_FREQ)
//		|| (Avgfreq_R < (double)MIN_AUDIO_FREQ))
//	{
//		if (nNoMeasureAudioResetRetry > 0)
//		{
//			nNoMeasureAudioResetRetry--;
//			pSoundCard->WaveRead_Stop();  //_Wait(1000); 
//			pSoundCard->WaveRead_Start(); //_Wait(1000); 
//			goto MEASURE_FREQ_START;
//		}
//	}
//
//	//pSoundCard->SetLedValue_inL((long)Avgfreq_L);
//	//pSoundCard->ctrlLED_inL.Invalidate();
//
//	//pSoundCard->SetLedValue_inR((long)Avgfreq_R);
//	//pSoundCard->ctrlLED_inR.Invalidate();
//
//	pSoundCard->m_fMeasuredLeftFreq = (float)freq_L;
//	pSoundCard->m_fMeasuredRightFreq = (float)freq_R;
//
//	sTemp.Format(_T("F:%d, %d/L:%d, %d"), (int)freq_L, (int)freq_R, (int)nAmplitude_L, (int)nAmplitude_R);
//
//	ctrlStepMeasur_A.SetWindowText(sTemp);
//	ctrlStepMeasur_A.Invalidate();
//	ctrlStepMeasur_A.UpdateWindow();
//
//	if ((pSoundCard->m_fMeasuredLeftFreq >= (pSoundCard->m_fLeftFreqLowerLimit))
//		&& (pSoundCard->m_fMeasuredLeftFreq <= (pSoundCard->m_fLeftFreqUpperLimit)))
//	{
//		aResult[0] = TEST_PASS;
//	}
//	else
//	{
//		aResult[0] = TEST_FAIL;
//	}
//
//	if ((pSoundCard->m_fMeasuredRightFreq >= (pSoundCard->m_fRightFreqLowerLimit))
//		&& (pSoundCard->m_fMeasuredRightFreq <= (pSoundCard->m_fRightFreqUpperLimit)))
//	{
//		aResult[1] = TEST_PASS;
//	}
//	else
//	{
//		aResult[1] = TEST_FAIL;
//	}
//
//	if ((aResult[0] == TEST_FAIL) || (aResult[1] == TEST_FAIL))
//	{
//		if (nNoMeasureAudioFreqRetry++ < MAX_NO_MEASURE_AUDIO_RETRY)
//		{
//			szMsg.Format("Retry Check Freq (%d/%d)", nNoMeasureAudioFreqRetry, MAX_NO_MEASURE_AUDIO_RETRY);
//			ShowSubStepMessage(szMsg, "Retry Audio Freq Check");
//			//+change kwmoon 080618
//		//	_Wait(500); 
//			_Wait(200);
//			goto MEASURE_FREQ_START;
//		}
//	}
//
//END_THREAD:
//
//	if (CurrentSet->nRunType == STEP)
//	{
//		if (pSoundCard->wIn_Flag) { pSoundCard->WaveRead_Stop(); _Wait(100); }
//	}
//
//	// If all of the 4 audio test are passed, set the result TRUE;
//	if ((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS)
//		&& (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
//
//	{
//		pSoundCard->m_bResult = TRUE;
//	}
//
//	pSoundCard->m_bThreadRunnuing = FALSE;
//
//	::SetEvent(pSoundCard->m_hAudioMeasureThreadKillEvent);
//
//	return 0;
//}

//void CSoundCard::RunAudioMeasure()
//{
//	if (!m_bThreadRunnuing)
//	{
//		//+change 090213(Modification No1)
//	//	m_hThread = AfxBeginThread(StartAudioMeasureThread,this);
//		m_pAudioMeasureThread = AfxBeginThread(StartAudioMeasureThread, this);
//
//		//+add 090213(Modification No1)
//#ifdef _THREAD_DEBUG
//		CString szString;
//		szString.Format("StartAudioMeasureThread %x\n", m_pAudioMeasureThread->m_nThreadID);
//		//		if(g_LogFileOpen) g_LogFile.WriteString(szString);
//#endif
//
//	//	StartAudioMeasureThread(this);
//		m_bThreadRunnuing = TRUE;
//	}
//}
//

void CSoundCard::WaveInOutClose()
{
	//	MSG msg;

	if (wIn_Flag)
	{
		//ctrlWaveIn.Close();
	
		m_nSoundStatus = 0;
		wIn_Flag = FALSE;

		_Wait(1000);
	}
	
}

BOOL CSoundCard::AudioMeasure()
{
	CString sTemp;
	double freq, amp, dbamp;
	double nCorrectionValue = 0.97;
	double dLevel_L = 0;
	double dLevel_R = 0;
	DWORD dwElapsedTime = 0;
	DWORD dwTickCount = GetTickCount();

	int aResult[4] = { 0,0,0,0 };

	float fLowerMargin = (float)(1 - (m_fFreqMargin*0.01));
	float fUpperMargin = (float)(1 + (m_fFreqMargin*0.01));

	float fLevelLowerMargin = (float)(1 - (m_fLevelMargin * 0.01));
	float fLevelUpperMargin = (float)(1 + (m_fLevelMargin * 0.01));

	m_fMeasuredLeftFreq = 0;
	m_fMeasuredRightFreq = 0;
	m_fMeasuredLeftLevel = 0;
	m_fMeasuredRightLevel = 0;

	if (m_fTargetLeftFreq < 100)
	{

		m_fLeftFreqLowerLimit = m_fTargetLeftFreq - 15;
		m_fLeftFreqUpperLimit = m_fTargetLeftFreq + 15;

	}
	else
	{

		m_fLeftFreqLowerLimit = m_fTargetLeftFreq * fLowerMargin;
		m_fLeftFreqUpperLimit = m_fTargetLeftFreq * fUpperMargin;

	}

	if (m_fTargetRightFreq < 100)
	{
		m_fRightFreqLowerLimit = m_fTargetRightFreq - 15;
		m_fRightFreqUpperLimit = m_fTargetRightFreq + 15;
	}
	else
	{
		m_fRightFreqLowerLimit = m_fTargetRightFreq * fLowerMargin;
		m_fRightFreqUpperLimit = m_fTargetRightFreq * fUpperMargin;
	}

#if 0
	if (m_fTargetLeftLevel < 100)
	{
		m_fLeftLevelLowerLimit = m_fTargetLeftLevel - 30;

	}
	else
	{
		m_fLeftLevelLowerLimit = m_fTargetLeftLevel * fLevelLowerMargin;
	}

	if (m_fTargetRightLevel < 100)
	{
		m_fRightLevelLowerLimit = m_fTargetRightLevel - 30;
	}
	else
	{
		m_fRightLevelLowerLimit = m_fTargetRightLevel * fLevelLowerMargin;
	}
#else

	m_fLeftLevelLowerLimit = m_fTargetLeftLevel * fLevelLowerMargin;
	m_fRightLevelLowerLimit = m_fTargetRightLevel * fLevelLowerMargin;
#endif
	int TestCount = 1;


	if ((m_fTargetLeftFreq < 15) && (m_fTargetRightFreq < 15))
	{
		if ((m_bMeasureAudioType == FREQ_LEVEL_CHECK) || (m_bMeasureAudioType == FREQUENCY_CHECK))
		{
			TestCount = 5;
		}
	}



	m_bResult = FALSE;
	while (m_bResult == FALSE)
	{
		for (int i = 0; i < TestCount; i++)
		{

			if (m_bResult == TRUE)
			{
				_Wait(100);
			}

			if (!wIn_Flag) { WaveRead_Start(); _Wait(100); }

			//EnterCriticalSection(&g_cs);

			////g_CriticalSection.Lock();
			//// LEFT
			/////ctrlPeakDetect.Invalidate();
			////ctrlSpectrum_L.Invalidate();
			//GetWavePeak_L(&freq, &amp, &dbamp);
			//GetPeackDetectorPeak_L(&dLevel_L);
			//m_fMeasuredLeftFreq = (float)freq;
			//m_fMeasuredLeftLevel = (float)(dLevel_L / (2 * 1.4142)*0.1*nCorrectionValue);
			//// RIGHT
			////ctrlSpectrum_R.Invalidate();
			//GetWavePeak_R(&freq, &amp, &dbamp);
			//GetPeackDetectorPeak_R(&dLevel_R);
			//m_fMeasuredRightFreq = (float)freq;
			//m_fMeasuredRightLevel = (float)(dLevel_R / (2 * 1.4142)*0.1*nCorrectionValue);
			//LeaveCriticalSection(&g_cs);
			////g_CriticalSection.Unlock(); 
			//EnterCriticalSection(&g_cs);

			//g_CriticalSection.Lock();
			

			// LEFT		
			m_fMeasuredLeftFreq =  g_pView->m_Frequency_Left;// (float)freq;
			m_fMeasuredLeftLevel = g_pView->m_Voltage_Left;//(float)(dLevel_L / (2 * 1.4142)*0.1*nCorrectionValue);
			
			// RIGHT			
			m_fMeasuredRightFreq = g_pView->m_Frequency_Right;//(float)freq;
			m_fMeasuredRightLevel = g_pView->m_Voltage_Right; //(float)(dLevel_R / (2 * 1.4142)*0.1*nCorrectionValue);



			switch (m_bMeasureAudioType)
			{
			case FREQ_LEVEL_CHECK://checkaudio_level_nofreq
				sTemp.Format(_T("F:%d,%d/ L:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq, (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
				if ((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)) {
					aResult[0] = TEST_PASS;
				}
				else {
					aResult[0] = TEST_FAIL;
				}

				if ((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit) && (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
					aResult[1] = TEST_PASS;
				}
				else {
					aResult[1] = TEST_FAIL;
				}

				if (m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
					aResult[2] = TEST_PASS;
				}
				else {
					aResult[2] = TEST_FAIL;
				}

				if (m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
					aResult[3] = TEST_PASS;
				}
				else {
					aResult[3] = TEST_FAIL;
				}

				if ((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS) && (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
				{
					m_bResult = TRUE;


				}
				else {
					m_bResult = FALSE;
				}
				break;

			case FREQUENCY_CHECK:
				sTemp.Format(_T("F:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq);
				if ((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)) {
					aResult[0] = TEST_PASS;
				}
				else {
					aResult[0] = TEST_FAIL;
				}

				if ((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit) && (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
					aResult[1] = TEST_PASS;
				}
				else {
					aResult[1] = TEST_FAIL;
				}

				// add 20101222
				if ((fabs(m_fMeasuredLeftLevel) < (m_fMeasuredRightLevel*0.1)) || (fabs(m_fMeasuredRightLevel) < fabs(m_fMeasuredLeftLevel*0.1))) {
					aResult[2] = TEST_FAIL;
				}
				else {
					aResult[2] = TEST_PASS;
				}
				//-

				if ((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS) && (aResult[2] == TEST_PASS))
				{
					m_bResult = TRUE;
				}
				else {
					m_bResult = FALSE;
				}
				break;

			case LEVEL_CHECK:
				sTemp.Format(_T("L:%d,%d"), (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
				if (m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
					aResult[2] = TEST_PASS;
				}
				else {
					aResult[2] = TEST_FAIL;
				}

				if (m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
					aResult[3] = TEST_PASS;
				}
				else {
					aResult[3] = TEST_FAIL;
				}

				if ((aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
				{
					m_bResult = TRUE;
				}
				else {
					m_bResult = FALSE;
				}
				break;

			case FREQ_LEVEL_CHECK_MOVING://checkaudio_level_nofreq
				sTemp.Format(_T("F:%d,%d/ L:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq, (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
				if ((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)) {
					aResult[0] = TEST_FAIL;
				}
				else {
					aResult[0] = TEST_PASS;
				}

				if ((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit) && (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
					aResult[1] = TEST_FAIL;
				}
				else {
					aResult[1] = TEST_PASS;
				}

				if (m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
					aResult[2] = TEST_PASS;
				}
				else {
					aResult[2] = TEST_FAIL;
				}

				if (m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
					aResult[3] = TEST_PASS;
				}
				else {
					aResult[3] = TEST_FAIL;
				}

				if ((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS) && (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
				{
					m_bResult = TRUE;


				}
				else {
					m_bResult = FALSE;
				}
				break;
			}
			ctrlStepMeasur_A.SetWindowText(sTemp);
			ctrlStepMeasur_A.Invalidate();
			ctrlStepMeasur_A.UpdateWindow();


			if (m_bResult == FALSE)
			{

				break;

			}
		}
		if (m_bResult == FALSE)
		{
			dwElapsedTime = GetTickCount() - dwTickCount;

			if ((dwElapsedTime > (DWORD)m_nCheckTimeLimit) || (dwElapsedTime > 30000))//if ((100 < (DWORD)m_nCheckTimeLimit) && (dwElapsedTime > (DWORD)m_nCheckTimeLimit))
			{
				break;
			}
			_Wait(100);
		}
		else
		{
			break;
		}
	}

	return m_bResult;
}

