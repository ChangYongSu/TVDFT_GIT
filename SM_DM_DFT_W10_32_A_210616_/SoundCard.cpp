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
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//+add kwmoon 080515
extern int g_nRunningProcessNo;

//+add kwmoon 080516
extern CDATsysView*  g_pView;
extern CColorStaticST		ctrlStepMeasur_A;
//+add PSH 081202
//extern CCriticalSection g_CriticalSection;
extern CRITICAL_SECTION g_cs;  //psh추가

//+add 090213(Modification No1)
//extern CStdioFile g_LogFile;
//extern BOOL	g_LogFileOpen;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//extern CMMLEDLABELX		ctrlLEDLabel_AudioIn;

CSoundCard::CSoundCard()
{
	m_fTargetLeftFreq		= 0;
	m_fTargetRightFreq		= 0;
	
	m_fTargetLeftLevel		= 0;
	m_fTargetRightLevel		= 0;

	m_fFreqMargin			= 0;
	m_fLevelMargin			= 0;

	m_fMeasuredLeftFreq		= 0;
	m_fMeasuredRightFreq	= 0;

	m_fMeasuredLeftLevel	= 0;
	m_fMeasuredRightLevel	= 0;

	m_fLeftFreqLowerLimit	= 0;
	m_fLeftFreqUpperLimit	= 0;
	m_fRightFreqLowerLimit	= 0;
	m_fRightFreqUpperLimit	= 0;

	m_fLeftLevelLowerLimit	= 0;
	m_fLeftLevelUpperLimit	= 0;
	m_fRightLevelLowerLimit	= 0;
	m_fRightLevelUpperLimit	= 0;

	wIn_Flag				= FALSE;

	wOutL_Flag				= FALSE;
	wOutR_Flag				= FALSE;

	m_bResult				= FALSE;
	
	m_bThreadRunnuing		= FALSE;
	m_bExitThread			= FALSE;

	//+change 090213(Modification No1)
//	m_hThread			= NULL;
	m_pAudioMeasureThread = NULL;

	m_hAudioMeasureThreadKillEvent = NULL;
	
	//+add PSH 080816
//	m_bMeasureAudio = FALSE;
	//+add PSH 090305
	m_bMeasureAudioType = 0;
	m_nMeasureWait = 0;
}

CSoundCard::~CSoundCard()
{
	if(m_hAudioMeasureThreadKillEvent != NULL)
	{
		CloseHandle(m_hAudioMeasureThreadKillEvent); m_hAudioMeasureThreadKillEvent = NULL;
	}
	WaveOutL_Stop();
	WaveOutR_Stop();
}

void CSoundCard::WaveRead_Start()
{
//	long aStatus;

	if (!ctrlWaveIn.GetNumdevs()) 
	{
		AfxMessageBox("Sorry, No WaveIn device installed!", MB_ICONERROR + MB_OK,0);
		wIn_Flag = FALSE;
	} 
	else
	{
		ctrlWaveIn.Start();
/*		aStatus = ctrlWaveIn.GetState();
		switch(aStatus)
		{
			case 1 :
				ctrlWaveIn.Open();
				ctrlWaveIn.Start();
				break;
			case 2 :
			case 4 :
				ctrlWaveIn.Start();
				break;
			default:
				break;
		}  */ 
		wIn_Flag = TRUE;
	}
}

void CSoundCard::WaveRead_Stop()
{
	if(!wIn_Flag) return;

	//+add 090131(수정항목1)
	DWORD dwExitCode = 0;
	if(m_pAudioMeasureThread)
	{
		TerminateThread(m_pAudioMeasureThread->m_hThread,dwExitCode);

		//+add 090224(Modification No2)
		m_pAudioMeasureThread = NULL; 
	}

//	ctrlWaveIn.Stop();
	ctrlWaveIn.Close(); 
	ctrlLED_inL.SetValue(0);
	ctrlLED_inR.SetValue(0);

	ctrlLED_inL_Level.SetValue(0);
	ctrlLED_inR_Level.SetValue(0);

	//090615
	ctrlPeakDetect.ResetData();
	ctrlSpectrum_L.ResetData();
	ctrlSpectrum_R.ResetData();  

	wIn_Flag = FALSE;

} 

void CSoundCard::WaveOutL_Start()
{
	if (!ctrlWaveIn.GetNumdevs()) 
	{
		AfxMessageBox("Sorry, No WaveIn device installed!", MB_ICONERROR + MB_OK,0);
		wIn_Flag = FALSE;
	} 
	else
	{
		ctrlWaveOut_L.Open();
		ctrlWaveOut_L.Start();
		wOutL_Flag = TRUE;
	}
}

void CSoundCard::WaveOutL_Stop()
{
	if(!wOutL_Flag) return;

	ctrlWaveOut_L.Close(); 

	wOutL_Flag = FALSE;
} 

void CSoundCard::WaveOutR_Start()
{
	if (!ctrlWaveIn.GetNumdevs()) 
	{
		AfxMessageBox("Sorry, No WaveIn device installed!", MB_ICONERROR + MB_OK,0);
		wIn_Flag = FALSE;
	} 
	else
	{
		ctrlWaveOut_R.Open();
		ctrlWaveOut_R.Start();
		wOutR_Flag = TRUE;
	}
}

void CSoundCard::WaveOutR_Stop()
{
	if(!wOutR_Flag) return;

	ctrlWaveOut_R.Close(); 

	wOutR_Flag = FALSE;
} 



BOOL CSoundCard::Creat(CWnd* pWnd)
{
	ctrlWaveIn.SubclassDlgItem(IDC_MMWAVEINX1,pWnd);

	ctrlWaveOut_L.SubclassDlgItem(IDC_MMWAVEOUTX1,pWnd);
	ctrlWaveOut_R.SubclassDlgItem(IDC_MMWAVEOUTX2,pWnd);

	ctrlSpectrum_L.SubclassDlgItem(IDC_MMSPECTRUMX1,pWnd); 
	ctrlSpectrum_R.SubclassDlgItem(IDC_MMSPECTRUMX2,pWnd);
	
	ctrlLED_inL.SubclassDlgItem(IDC_MMLEDPANELX_INL,pWnd); 
	ctrlLED_inR.SubclassDlgItem(IDC_MMLEDPANELX_INR,pWnd); 

	ctrlLED_inL_Level.SubclassDlgItem(IDC_MMLEDPANELX_INL_LEVEL,pWnd); 
	ctrlLED_inR_Level.SubclassDlgItem(IDC_MMLEDPANELX_INR_LEVEL,pWnd);

	ctrlPeakDetect.SubclassDlgItem(IDC_MMPEAKDETECTX1,pWnd);

	//+add kwmoon 080515
	if(g_nRunningProcessNo == 1)
	{
		//+add PSH 080527
		if(ctrlWaveIn.GetNumdevs() >= 1)
		{
			ctrlWaveIn.SetDeviceID(0);
			ctrlWaveOut_L.SetDeviceID(0);
			ctrlWaveOut_R.SetDeviceID(0);
		}
		else{
			ctrlWaveIn.SetDeviceID(-1);
			ctrlWaveOut_L.SetDeviceID(-1);
			ctrlWaveOut_R.SetDeviceID(-1);
		}
	}
	else
	{
		if(ctrlWaveIn.GetNumdevs() >= 2)
		{
			ctrlWaveIn.SetDeviceID(1);
			ctrlWaveOut_L.SetDeviceID(1);
			ctrlWaveOut_R.SetDeviceID(1);
		}
		else{
			ctrlWaveIn.SetDeviceID(-1);
			ctrlWaveOut_L.SetDeviceID(-1);
			ctrlWaveOut_R.SetDeviceID(-1);
		}
	}

	m_hAudioMeasureThreadKillEvent = ::CreateEvent(NULL,false,false,NULL);

	return TRUE;
}

void CSoundCard::GetWavePeak_L(double *freq, double *ampl, double *dbampl)
{
	if(!wIn_Flag) return;
	ctrlSpectrum_L.GetPeak(freq, ampl, dbampl);
}

void CSoundCard::GetWavePeak_R(double *freq, double *ampl, double *dbampl)
{
	if(!wIn_Flag) return;
	ctrlSpectrum_R.GetPeak(freq, ampl, dbampl);
}

void CSoundCard::SetLedValue_inL(long value)
{
	if(!wIn_Flag) return;
	ctrlLED_inL.SetValue(value);	
}

void CSoundCard::SetLedValue_inR(long value)
{
	if(!wIn_Flag) return;
	ctrlLED_inR.SetValue(value);	
}

void CSoundCard::SetLedValue_inL_Level(long value)
{
	if(!wIn_Flag) return;
	ctrlLED_inL_Level.SetValue(value);	
}

void CSoundCard::SetLedValue_inR_Level(long value)
{
	if(!wIn_Flag) return;
	ctrlLED_inR_Level.SetValue(value);	
}

void CSoundCard::GetPeackDetectorPeak_R(double *pLevel)
{
	if(!wIn_Flag) return;
	*pLevel = ctrlPeakDetect.GetPeakRight();
}

void CSoundCard::GetPeackDetectorPeak_L(double *pLevel)
{
	if(!wIn_Flag) return;
	*pLevel = ctrlPeakDetect.GetPeakLeft();
}

//+changed kwmoon 080514
UINT CSoundCard::StartAudioMeasureThread(LPVOID pParam)
{
	CSoundCard* pSoundCard =(CSoundCard* )pParam;

	CString sTemp;

	int aResult[4] = {0,0,0,0};
	int nNoMeasureRepeat = 0;

	float fLowerMargin	 = (float)(1-(pSoundCard->m_fFreqMargin*0.01));
	float fUpperMargin	 = (float)(1+(pSoundCard->m_fFreqMargin*0.01));

	//+add kwmoon 080716
	float fLevelLowerMargin	 = (float)(1-(pSoundCard->m_fLevelMargin * 0.01));
	//+add kwmoon 080819
	float fLevelUpperMargin	 = (float)(1+(pSoundCard->m_fLevelMargin * 0.01));

	pSoundCard->m_fMeasuredLeftFreq		= 0;
	pSoundCard->m_fMeasuredRightFreq	= 0;
	
	pSoundCard->m_fMeasuredLeftLevel	= 0;
	pSoundCard->m_fMeasuredRightLevel	= 0;

	pSoundCard->m_bResult		= FALSE;
	pSoundCard->m_bExitThread	= FALSE;

	pSoundCard->m_fLeftFreqLowerLimit	= pSoundCard->m_fTargetLeftFreq * fLowerMargin;
	pSoundCard->m_fLeftFreqUpperLimit	= pSoundCard->m_fTargetLeftFreq * fUpperMargin;

	pSoundCard->m_fRightFreqLowerLimit	= pSoundCard->m_fTargetRightFreq * fLowerMargin;
	pSoundCard->m_fRightFreqUpperLimit	= pSoundCard->m_fTargetRightFreq * fUpperMargin;

/*	//+change kwmoon 080716
//	pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLowerMargin;
	pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLevelLowerMargin;
	
	//+del kwmoon 080716
//	pSoundCard->m_fLeftLevelUpperLimit	= pSoundCard->m_fTargetLeftLevel * fUpperMargin;
	
	//+change kwmoon 080716
//	pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLowerMargin;
	pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLevelLowerMargin;

	//+del kwmoon 080716
//	pSoundCard->m_fRightLevelUpperLimit	= pSoundCard->m_fTargetRightLevel * fUpperMargin;
*/
	//+del kwmoon 080828
	//+change PSH 080816
//	if(pSoundCard->m_bMeasureAudio)
	{
		pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLevelLowerMargin;
		pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLevelLowerMargin;

	}
	//+del kwmoon 080828
/*	else
	{
		//+change kwmoon 080819
	//	pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLowerMargin;
	//	pSoundCard->m_fLeftLevelUpperLimit	= pSoundCard->m_fTargetLeftLevel * fUpperMargin;
	
	//	pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLowerMargin;
	//	pSoundCard->m_fRightLevelUpperLimit	= pSoundCard->m_fTargetRightLevel * fUpperMargin;
	
		pSoundCard->m_fLeftLevelLowerLimit	= pSoundCard->m_fTargetLeftLevel * fLevelLowerMargin;
		pSoundCard->m_fLeftLevelUpperLimit	= pSoundCard->m_fTargetLeftLevel * fLevelUpperMargin;
	
		pSoundCard->m_fRightLevelLowerLimit	= pSoundCard->m_fTargetRightLevel * fLevelLowerMargin;
		pSoundCard->m_fRightLevelUpperLimit	= pSoundCard->m_fTargetRightLevel * fLevelUpperMargin;

	}
*/
	::ResetEvent(pSoundCard->m_hAudioMeasureThreadKillEvent);

	//090615
	//if(!pSoundCard->wIn_Flag){ pSoundCard->WaveRead_Start(); _Wait(100); }
	g_pView->AudioMeasureStart();
	
	double freq_L,freq_R;
	double ampl_L,ampl_R;
	double dbampl_L,dbampl_R;

	double Avgfreq_L,Avgfreq_R;
	double Avgampl_L,Avgampl_R;
	double Avgdbampl_L,Avgdbampl_R;

	int nNoMeasureAudioFreqRetry		= 0;//MAX_NO_MEASURE_AUDIO_RETRY;
	int nNoMeasureAudioLevelRetry		= 0;//MAX_NO_MEASURE_AUDIO_RETRY;
	int nNoMeasureAudioResetRetry		= 0;//MAX_NO_MEASURE_AUDIO_RETRY;

	double nLevel_L,nLevel_R;
	double nAvgLevel_L,nAvgLevel_R;
	double nAmplitude_L,nAmplitude_R;
	double nCorrectionValue = 0.97;
	
	CString szMsg;

	int nNoRetry = 0;

MEASURE_LEVEL_START : 

	nLevel_L			= 0;
	nLevel_R			= 0;
	nAvgLevel_L			= 0;
	nAvgLevel_R			= 0;
	nAmplitude_L		= 0;
	nAmplitude_R		= 0;
	nCorrectionValue	= 0.97;

//	if(nNoMeasureAudioLevelRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	{
//		szMsg.Format("Retry Check Audio Level (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioLevelRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//		ShowSubStepMessage(szMsg, "Retry Audio Level Check");
//	}
	
//	if(nNoMeasureAudioResetRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	{
//		szMsg.Format("Reset Audio Device (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioResetRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//		ShowSubStepMessage(szMsg, "Retry Audio Device Reset");
//	}
	
	for(nNoMeasureRepeat=0; nNoMeasureRepeat<3;nNoMeasureRepeat++)
	{
		if(pSoundCard->m_bExitThread)
		{
			goto END_THREAD;
		}
		
		_Wait(100);
		pSoundCard->GetPeackDetectorPeak_L(&nLevel_L);
		pSoundCard->GetPeackDetectorPeak_R(&nLevel_R);
		_Wait(50);

		nAvgLevel_L   += nLevel_L;
		nAvgLevel_R   += nLevel_R;
	}

	nAvgLevel_L   = (double)(nAvgLevel_L   / nNoMeasureRepeat);
	nAvgLevel_R   = (double)(nAvgLevel_R   / nNoMeasureRepeat);

	if((nAvgLevel_L < (double)MIN_AUDIO_LEVEL)
	|| (nAvgLevel_R < (double)MIN_AUDIO_LEVEL))
	{
		if(nNoMeasureAudioResetRetry > 0)
		{
			nNoMeasureAudioResetRetry--; 
			pSoundCard->WaveRead_Stop(); // _Wait(1000); 
			pSoundCard->WaveRead_Start(); //_Wait(1000);
			goto MEASURE_LEVEL_START;
		}
	}
	
	nAmplitude_L = nAvgLevel_L/(2*1.4142)*0.1*nCorrectionValue;
	nAmplitude_R = nAvgLevel_R/(2*1.4142)*0.1*nCorrectionValue;

	pSoundCard->SetLedValue_inL_Level((long)nAmplitude_L);
	pSoundCard->SetLedValue_inR_Level((long)nAmplitude_R);

	pSoundCard->m_fMeasuredLeftLevel  = (float)nAmplitude_L;
	pSoundCard->m_fMeasuredRightLevel = (float)nAmplitude_R;

	sTemp.Format(_T("Level:%d, %d"), (int)nAmplitude_L, (int)nAmplitude_R);

	ctrlStepMeasur_A.SetWindowText(sTemp);
	ctrlStepMeasur_A.Invalidate();
	ctrlStepMeasur_A.UpdateWindow();

	/*
	//+change kwmoon 080716
//	if((pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
//	&& (pSoundCard->m_fMeasuredLeftLevel <= (pSoundCard->m_fLeftLevelUpperLimit)))
	if(pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
	{
		aResult[2] = TEST_PASS;
	}
	else
	{
		aResult[2] = TEST_FAIL;
//		nNoRetry = nNoMeasureAudioLevelRetry--;
		
//		if(nNoRetry > 0){ _Wait(500); goto MEASURE_LEVEL_START; }
	}

	//+change kwmoon 080716
//	if((pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
//	&& (pSoundCard->m_fMeasuredRightLevel <= (pSoundCard->m_fRightLevelUpperLimit)))
	if(pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
	{
		aResult[3] = TEST_PASS;
	}
	else
	{
		aResult[3] = TEST_FAIL;
//		nNoRetry = nNoMeasureAudioLevelRetry--;

//		if(nNoRetry > 0){ _Wait(500); goto MEASURE_LEVEL_START; }
	}
*/
	//+del kwmoon 080828
	//+change PSH 080816
//	if(pSoundCard->m_bMeasureAudio)
	{
		if(pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
		{
			aResult[2] = TEST_PASS;
		}
		else
		{
			aResult[2] = TEST_FAIL;
		}
		if(pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
		{
			aResult[3] = TEST_PASS;
		}
		else
		{
			aResult[3] = TEST_FAIL;
		}
	}
	//+del kwmoon 080828
/*	else
	{
		if((pSoundCard->m_fMeasuredLeftLevel >= (pSoundCard->m_fLeftLevelLowerLimit))
		&& (pSoundCard->m_fMeasuredLeftLevel <= (pSoundCard->m_fLeftLevelUpperLimit)))
		{
			aResult[2] = TEST_PASS;
		}
		else
		{
			aResult[2] = TEST_FAIL;
		}

		if((pSoundCard->m_fMeasuredRightLevel >= (pSoundCard->m_fRightLevelLowerLimit))
		&& (pSoundCard->m_fMeasuredRightLevel <= (pSoundCard->m_fRightLevelUpperLimit)))
		{
			aResult[3] = TEST_PASS;
		}
		else
		{
			aResult[3] = TEST_FAIL;
		}

	}
*/
	if(!g_pView->m_bAdjSpecMode)
	{
		if((aResult[2] == TEST_FAIL) || (aResult[3] == TEST_FAIL))
		{
			if(nNoMeasureAudioLevelRetry++ < MAX_NO_MEASURE_AUDIO_RETRY)
			{
				szMsg.Format("Retry Check Level (%d_%d)",nNoMeasureAudioLevelRetry,MAX_NO_MEASURE_AUDIO_RETRY);
				ShowSubStepMessage(szMsg, "Retry Audio Level Check");	
				
				//+change kwmoon 080618
			//	_Wait(500); 
				_Wait(200);

				goto MEASURE_LEVEL_START;
			}
		}
	}

MEASURE_FREQ_START : 

	freq_L	= 0.0;
	freq_R	= 0.0;
	ampl_L	= 0.0;
	ampl_R	= 0.0;
	dbampl_L = 0.0;
	dbampl_R = 0.0;

	Avgfreq_L	= 0.0;
	Avgfreq_R	= 0.0;
	Avgampl_L	= 0.0;
	Avgampl_R	= 0.0;
	Avgdbampl_L	= 0.0;
	Avgdbampl_R	= 0.0;

//	if(nNoMeasureAudioFreqRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	{
//		szMsg.Format("Retry Check Audio Freq (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioFreqRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//		ShowSubStepMessage(szMsg, "Retry Audio Freq Check");
//	}

//	if(nNoMeasureAudioResetRetry != MAX_NO_MEASURE_AUDIO_RETRY)
//	{
//		szMsg.Format("Reset Audio Device (%d/%d)",MAX_NO_MEASURE_AUDIO_RETRY-nNoMeasureAudioResetRetry,MAX_NO_MEASURE_AUDIO_RETRY);
//		ShowSubStepMessage(szMsg, "Retry Audio Device Reset");
//	}

	for(nNoMeasureRepeat=0; nNoMeasureRepeat<3;nNoMeasureRepeat++)
	{
		if(pSoundCard->m_bExitThread)
		{
			goto END_THREAD;
		}

		_Wait(50);
		
		pSoundCard->ctrlSpectrum_L.Invalidate();
		pSoundCard->GetWavePeak_L(&freq_L, &ampl_L, &dbampl_L);

		pSoundCard->ctrlSpectrum_R.Invalidate();
		pSoundCard->GetWavePeak_R(&freq_R, &ampl_R, &dbampl_R);
		
		_Wait(50);

		Avgfreq_L   += freq_L;
		Avgampl_L   += ampl_L;
		Avgdbampl_L += dbampl_L;

		Avgfreq_R   += freq_R;
		Avgampl_R   += ampl_R;
		Avgdbampl_R += dbampl_R;
	}

	Avgfreq_L   = (double)(Avgfreq_L   / nNoMeasureRepeat);
	Avgampl_L   = (double)(Avgampl_L   / nNoMeasureRepeat);
	Avgdbampl_L = (double)(Avgdbampl_L / nNoMeasureRepeat);

	Avgfreq_R   = (double)(Avgfreq_R   / nNoMeasureRepeat);
	Avgampl_R   = (double)(Avgampl_R   / nNoMeasureRepeat);
	Avgdbampl_R = (double)(Avgdbampl_R / nNoMeasureRepeat);

	if((Avgfreq_L < (double)MIN_AUDIO_FREQ)
	|| (Avgfreq_R < (double)MIN_AUDIO_FREQ)) 
	{
		if(nNoMeasureAudioResetRetry > 0)
		{
			nNoMeasureAudioResetRetry--; 
			pSoundCard->WaveRead_Stop();  //_Wait(1000); 
			pSoundCard->WaveRead_Start(); //_Wait(1000); 
			goto MEASURE_FREQ_START;
		}
	}

	pSoundCard->SetLedValue_inL((long)Avgfreq_L);
	pSoundCard->ctrlLED_inL.Invalidate();

	pSoundCard->SetLedValue_inR((long)Avgfreq_R);
	pSoundCard->ctrlLED_inR.Invalidate();

	pSoundCard->m_fMeasuredLeftFreq  = (float)freq_L;
	pSoundCard->m_fMeasuredRightFreq = (float)freq_R;

	sTemp.Format(_T("F:%d, %d/L:%d, %d"), (int)freq_L, (int)freq_R, (int)nAmplitude_L, (int)nAmplitude_R);

	ctrlStepMeasur_A.SetWindowText(sTemp);
	ctrlStepMeasur_A.Invalidate();
	ctrlStepMeasur_A.UpdateWindow();
		
	if((pSoundCard->m_fMeasuredLeftFreq >= (pSoundCard->m_fLeftFreqLowerLimit))
	&& (pSoundCard->m_fMeasuredLeftFreq <= (pSoundCard->m_fLeftFreqUpperLimit)))
	{
		aResult[0] = TEST_PASS;
	}
	else
	{
		aResult[0] = TEST_FAIL;
	}

	if((pSoundCard->m_fMeasuredRightFreq >= (pSoundCard->m_fRightFreqLowerLimit))
	&& (pSoundCard->m_fMeasuredRightFreq <= (pSoundCard->m_fRightFreqUpperLimit)))
	{
		aResult[1] = TEST_PASS;
	}
	else
	{
		aResult[1] = TEST_FAIL;
	}

	if(!g_pView->m_bAdjSpecMode)
	{
		if((aResult[0] == TEST_FAIL) || (aResult[1] == TEST_FAIL))
		{
			if(nNoMeasureAudioFreqRetry++ < MAX_NO_MEASURE_AUDIO_RETRY)
			{
				szMsg.Format("Retry Check Freq (%d_%d)",nNoMeasureAudioFreqRetry,MAX_NO_MEASURE_AUDIO_RETRY);
				ShowSubStepMessage(szMsg, "Retry Audio Freq Check");	
				//+change kwmoon 080618
			//	_Wait(500); 
				_Wait(200);
				goto MEASURE_FREQ_START;
			}
		}
	}

END_THREAD :

	if(CurrentSet->nRunType == STEP)
	{
		if(pSoundCard->wIn_Flag){ pSoundCard->WaveRead_Stop(); _Wait(100); }
	}

	// If all of the 4 audio test are passed, set the result TRUE;
	if((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS)
	&& (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))

	{
		pSoundCard->m_bResult = TRUE;
	}
	
	pSoundCard->m_bThreadRunnuing = FALSE;

	::SetEvent(pSoundCard->m_hAudioMeasureThreadKillEvent);

	return 0;
}

void CSoundCard::RunAudioMeasure()
{
	if(!m_bThreadRunnuing)
	{
		//+change 090213(Modification No1)
	//	m_hThread = AfxBeginThread(StartAudioMeasureThread,this);
		m_pAudioMeasureThread = AfxBeginThread(StartAudioMeasureThread,this);

		//+add 090213(Modification No1)
#ifdef _THREAD_DEBUG
		CString szString;
		szString.Format("StartAudioMeasureThread %x\n",m_pAudioMeasureThread->m_nThreadID);
//		if(g_LogFileOpen) g_LogFile.WriteString(szString);
#endif

	//	StartAudioMeasureThread(this);
		m_bThreadRunnuing = TRUE;
	}
}


void CSoundCard::WaveInOutClose()
{
//	MSG msg;

	if(wIn_Flag)
	{
		ctrlWaveIn.Close();
		wIn_Flag =FALSE;

		_Wait(2000);
	}
/*
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == 45557 || msg.message == 45559)break;
		if (msg.message == WM_QUIT)
		{
			::PostQuitMessage(0);
			break;
		}
		if (!AfxGetApp()->PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	*/
}

BOOL CSoundCard::AudioMeasure()
{
	CString sTemp;
	double freq, amp, dbamp;
	double nCorrectionValue = 0.97;
	double dLevel_L = 0;
	double dLevel_R = 0;

	int aResult[4] = {0,0,0,0};

	float fLowerMargin	 = (float)(1-(m_fFreqMargin*0.01));
	float fUpperMargin	 = (float)(1+(m_fFreqMargin*0.01));

	float fLevelLowerMargin	 = (float)(1-(m_fLevelMargin * 0.01));
	float fLevelUpperMargin	 = (float)(1+(m_fLevelMargin * 0.01));

	m_fMeasuredLeftFreq		= 0;
	m_fMeasuredRightFreq	= 0;
	m_fMeasuredLeftLevel	= 0;
	m_fMeasuredRightLevel	= 0;

//	m_fLeftFreqLowerLimit	= m_fTargetLeftFreq * fLowerMargin;
//	m_fLeftFreqUpperLimit	= m_fTargetLeftFreq * fUpperMargin;

//	m_fRightFreqLowerLimit	= m_fTargetRightFreq * fLowerMargin;
//	m_fRightFreqUpperLimit	= m_fTargetRightFreq * fUpperMargin;

	m_fLeftFreqLowerLimit	= m_fTargetLeftFreq  - m_fFreqMargin;
	m_fLeftFreqUpperLimit	= m_fTargetLeftFreq  + m_fFreqMargin;

	m_fRightFreqLowerLimit	= m_fTargetRightFreq - m_fFreqMargin;
	m_fRightFreqUpperLimit	= m_fTargetRightFreq + m_fFreqMargin;


	if(m_bMeasureAudioType == LEVEL_CHECK)
	{
		m_fLeftLevelLowerLimit	= m_fTargetLeftLevel;

		m_fRightLevelLowerLimit	= m_fTargetRightLevel;

	}
	else{
	//	m_fLeftLevelLowerLimit	= m_fTargetLeftLevel * fLevelLowerMargin;
	//	m_fRightLevelLowerLimit	= m_fTargetRightLevel * fLevelLowerMargin;
		m_fLeftLevelLowerLimit	= m_fTargetLeftLevel - m_fLevelMargin;
		m_fRightLevelLowerLimit	= m_fTargetRightLevel - m_fLevelMargin;
	}
//	m_fLeftLevelUpperLimit  = m_fTargetLeftLevel * fLevelUpperMargin;
//	m_fRightLevelUpperLimit	= m_fTargetRightLevel * fLevelUpperMargin;
	m_fLeftLevelUpperLimit  = m_fTargetLeftLevel + m_fLevelMargin;
	m_fRightLevelUpperLimit	= m_fTargetRightLevel + m_fLevelMargin;

	if(!wIn_Flag){ WaveRead_Start(); _Wait(100); }
	
	EnterCriticalSection(&g_cs);
	//g_CriticalSection.Lock();

	// LEFT
	ctrlPeakDetect.Invalidate();
	ctrlSpectrum_L.Invalidate();
	GetWavePeak_L(&freq, &amp, &dbamp);
	GetPeackDetectorPeak_L(&dLevel_L);

	m_fMeasuredLeftFreq  = (float)freq;
	m_fMeasuredLeftLevel = (float)(dLevel_L/(2*1.4142)*0.1*nCorrectionValue);
	// RIGHT
	ctrlSpectrum_R.Invalidate();
	GetWavePeak_R(&freq, &amp, &dbamp);
	GetPeackDetectorPeak_R(&dLevel_R);

	m_fMeasuredRightFreq = (float)freq;
	m_fMeasuredRightLevel = (float)(dLevel_R/(2*1.4142)*0.1*nCorrectionValue);
	
	LeaveCriticalSection(&g_cs);
	//g_CriticalSection.Unlock(); 

	switch(m_bMeasureAudioType)
	{
		case FREQ_LEVEL_CHECK:  
			sTemp.Format(_T("F:%d,%d/ L:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq, (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
			if((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)){
				aResult[0] = TEST_PASS;
			}
			else {
				aResult[0] = TEST_FAIL;
			}

			if((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit)	&& (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
				aResult[1] = TEST_PASS;
			}
			else {
				aResult[1] = TEST_FAIL;
			}

//			if(m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
			if((m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit)	&& (m_fMeasuredLeftLevel <= m_fLeftLevelUpperLimit)) {
				aResult[2] = TEST_PASS;
			}
			else {
				aResult[2] = TEST_FAIL;
			}

//			if(m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
			if((m_fMeasuredRightLevel >= m_fRightLevelLowerLimit)	&& (m_fMeasuredRightLevel <= m_fRightLevelUpperLimit)) {
				aResult[3] = TEST_PASS;
			}
			else {
				aResult[3] = TEST_FAIL;
			}

			if((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS) && (aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
			{
				m_bResult = TRUE; 
			}
			else {
				m_bResult = FALSE;
			}
			break;

		case FREQUENCY_CHECK:  
			sTemp.Format(_T("F:%d,%d"), (int)m_fMeasuredLeftFreq, (int)m_fMeasuredRightFreq);
			if((m_fMeasuredLeftFreq >= m_fLeftFreqLowerLimit) && (m_fMeasuredLeftFreq <= m_fLeftFreqUpperLimit)){
				aResult[0] = TEST_PASS;
			}
			else {
				aResult[0] = TEST_FAIL;
			}

			if((m_fMeasuredRightFreq >= m_fRightFreqLowerLimit)	&& (m_fMeasuredRightFreq <= m_fRightFreqUpperLimit)) {
				aResult[1] = TEST_PASS;
			}
			else {
				aResult[1] = TEST_FAIL;
			}

			if((aResult[0] == TEST_PASS) && (aResult[1] == TEST_PASS))
			{
				m_bResult = TRUE; 
			}
			else {
				m_bResult = FALSE;
			}
			break;

		case LEVEL_CHECK:  
			sTemp.Format(_T("L:%d,%d"), (int)m_fMeasuredLeftLevel, (int)m_fMeasuredRightLevel);
			if(m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit) {
//			if((m_fMeasuredLeftLevel >= m_fLeftLevelLowerLimit)	&& (m_fMeasuredLeftLevel <= m_fLeftLevelUpperLimit)) {
				aResult[2] = TEST_PASS;
			}
			else {
				aResult[2] = TEST_FAIL;
			}

			if(m_fMeasuredRightLevel >= m_fRightLevelLowerLimit) {
//			if((m_fMeasuredRightLevel >= m_fRightLevelLowerLimit)	&& (m_fMeasuredRightLevel <= m_fRightLevelUpperLimit)) {
				aResult[3] = TEST_PASS;
			}
			else {
				aResult[3] = TEST_FAIL;
			}

			if((aResult[2] == TEST_PASS) && (aResult[3] == TEST_PASS))
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

	return m_bResult;
}