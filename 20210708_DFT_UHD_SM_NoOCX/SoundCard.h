// SoundCard.h: interface for the CSoundCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDCARD_H__E0E45662_80AB_11D3_9DE4_006097552948__INCLUDED_)
#define AFX_SOUNDCARD_H__E0E45662_80AB_11D3_9DE4_006097552948__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// OCX
#include "mmoscopex.h"
#include "mmspectrumx.h"
#include "mmwaveinx.h"
#include "mmledpanelx.h"
#include "mmledlabelx.h"
#include "mmpeakdetectx.h"



class CSoundCard : public CObject  
{
public:
	BOOL Creat(CWnd* pWnd);
	
	CSoundCard();
	virtual ~CSoundCard();

	BOOL			wIn_Flag;

	CMMWaveInX		ctrlWaveIn;
	
	// Check Frequency
	CMMSpectrumX	ctrlSpectrum_L;
	CMMSpectrumX	ctrlSpectrum_R;

	// Check Level
	CMMPeakDetectX	ctrlPeakDetect;

	// Show Values
	CMMLEDPanelX	ctrlLED_inL;
	CMMLEDPanelX	ctrlLED_inR;
	CMMLEDPanelX	ctrlLED_inL_Level;
	CMMLEDPanelX	ctrlLED_inR_Level;


	float		m_fTargetLeftFreq;
	float		m_fTargetRightFreq;

	float		m_fTargetLeftLevel;
	float		m_fTargetRightLevel;

	float		m_fMeasuredLeftFreq;
	float		m_fMeasuredRightFreq;

	float		m_fMeasuredLeftLevel;
	float		m_fMeasuredRightLevel;

	float		m_fLeftFreqLowerLimit;
	float		m_fLeftFreqUpperLimit;
	float		m_fRightFreqLowerLimit;
	float		m_fRightFreqUpperLimit;

	float		m_fLeftLevelLowerLimit;
	float		m_fLeftLevelUpperLimit;
	float		m_fRightLevelLowerLimit;
	float		m_fRightLevelUpperLimit;

	//+change kwmoon 080724
//	float		m_fPercentMargin;
	float		m_fFreqMargin;

	//+add kwmoon 080724
	float		m_fLevelMargin;

	BOOL	m_bThreadRunnuing;
	BOOL	m_bResult;
	//int		m_nSoundOK_Count;


	BOOL	m_bExitThread;

	HANDLE	m_hAudioMeasureThreadKillEvent;

	//+change 090213(Modification No1)
//	HANDLE  m_hThread;
	CWinThread* m_pAudioMeasureThread;

	//+add PSH 080816
//	BOOL    m_bMeasureAudio;
	//+add PSH 090305
	int    m_bMeasureAudioType;

public:

	void WaveRead_Start();
	void WaveRead_Stop();
	void WaveInOutClose();

	// Get Frequency
	void GetWavePeak_L(double *freq, double *ampl, double *dbampl);
	void GetWavePeak_R(double *freq, double *ampl, double *dbampl);

	// Get Level
	void GetPeackDetectorPeak_L(double *pLevel);
	void GetPeackDetectorPeak_R(double *pLevel);

	void SetLedValue_inL(long value);
	void SetLedValue_inR(long value);
	void SetLedValue_inL_Level(long value);
	void SetLedValue_inR_Level(long value);
	
	// Thread
	void RunAudioMeasure();
	static UINT StartAudioMeasureThread(LPVOID pParam); 
	BOOL AudioMeasure();

	BOOL AudioTestMeasure();

};

extern CSoundCard	g_SoundCard;

#endif // !defined(AFX_SOUNDCARD_H__E0E45662_80AB_11D3_9DE4_006097552948__INCLUDED_)
