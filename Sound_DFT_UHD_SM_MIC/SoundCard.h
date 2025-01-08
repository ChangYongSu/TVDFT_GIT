// SoundCard.h: interface for the CSoundCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDCARD_H__E0E45662_80AB_11D3_9DE4_006097552948__INCLUDED_)
#define AFX_SOUNDCARD_H__E0E45662_80AB_11D3_9DE4_006097552948__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// OCX
//#include "mmoscopex.h"
//#include "mmspectrumx.h"
//#include "mmwaveinx.h"
//#include "mmledpanelx.h"
//#include "mmledlabelx.h"
//#include "mmpeakdetectx.h"
//#include "mmeapi.h"
#include "Windows.h"


#include "soundin.h"
#include "soundout.h"
#include "soundfile.h"
#include "ptrFIFO.h"


#define SAMPLE_DISPLAYCOUNT	1000
#define SAMPLE_BUFFER_MAX	2048
#define SAMPLE_BUFFER_CHECK_MAX	2038
#define SAMPLE_BYTE_MAX		4096
#define SAMPLE_RATE_PER_SECOND     44100

class CSoundCard : public CObject
{
public:
	BOOL Creat(CWnd* pWnd);

	CSoundCard();
	virtual ~CSoundCard();

	BOOL			wIn_Flag;

	//CMMWaveInX		ctrlWaveIn;

	//// Check Frequency
	//CMMSpectrumX	ctrlSpectrum_L;
	//CMMSpectrumX	ctrlSpectrum_R;

	//// Check Level
	//CMMPeakDetectX	ctrlPeakDetect;

	//// Show Values
	//CMMLEDPanelX	ctrlLED_inL;
	//CMMLEDPanelX	ctrlLED_inR;
	//CMMLEDPanelX	ctrlLED_inL_Level;
	//CMMLEDPanelX	ctrlLED_inR_Level;


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

	int		m_nCheckTimeLimit;

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
	CStringArray  strDeviceNameArry;
	//CStringArray  strHWDeviceIDArry;
	CStringArray  strHWDeviceLocationArry;
	CStringArray  strHWDeviceNameArry;
	CStringArray strSavedDeviceNameArry;
	int m_nDeviceCount;
	CString m_sCurrentDeviceName;
	int    m_nDeviceID;
	//int    m_nSoundDeviceSelectFlag;
	int    m_nSoundUSBDeviceFind;
	//int    m_nSoundUSBDeviceTest;
	int    m_nUSBDeviceID;
	CString m_sUSBDeviceName;
	int    m_nSoundStatus;
	int    m_nWinFlag;

	HWND m_hSoundMainWnd;

	//// Wave header structure for input
	//WAVEFORMATEX waveFormat;
	//WAVEHDR waveHeader;
	//HWAVEIN hWaveIn;
	//int waveCounter = 0;

public:
	CString CSoundCard::GetDeviceName(UINT lDeviceID);
	int CSoundCard::RefreshDeviceArray();
	void CSoundCard::CheckNamesAndLocations();
	int CSoundCard::SetDeviceID(int lDevice);
	//void CSoundCard::SetDeviceID(int lDevice);
	//void SaveShareSoundData();
	BOOL SoundInMainSet();
	int CSoundCard::Wave_Start();
	void WaveRead_Start();
	BOOL WaveRead_StartUSB(CString strSoundIn, int Fixname = 0);
	BOOL WaveCheck_Search(int Search = 0);
	void WaveRead_Stop();
	void WaveInOutClose();

	//int CSoundCard::IsMicSearchFree();
	//void CSoundCard::MicSearchBlock();
	//void CSoundCard::MicSearchRelease(int all_ch = 0);

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
	//void RunAudioMeasure();
	//static UINT StartAudioMeasureThread(LPVOID pParam);
	BOOL AudioMeasure();

public:

	void StartCapture(CWnd *parent, int nDeviceID);
	void StopCapture();

//	int SelectDevice(UINT nDevNum);

	//HWND m_hSoundMainWnd;

	void StopPlayingFromFile();
	void StartPlayingFromFile(CString lFileName);
	void StopRecordingToFile();
	void StartRecordingToFile(CString lFileName);
	void WriteSoundDataToFile(CBuffer* buffer);
	void ReadSoundDataFromFile(CBuffer* buffer);
	static void DataFromSoundIn(CBuffer* buffer, void* Owner);
	static void GetDataToSoundOut(CBuffer* buffer, void* Owner);

	void SetSamplesPerSecond(int sps);
	int GetSamplesPerSecond();
	void SetBufferSize(int NumberOfSamples);
	int GetBufferSize();


private:
	CPtrFIFO m_FIFOFull;	// FIFO holding pointers to buffers with sound
	CPtrFIFO m_FIFOEmpty;	// FIFO holding pointers to empty buffers that can be reused

	CSoundFile* m_pFile;
	CSoundOut	m_SoundOut;
	CSoundIn	m_SoundIn;

};

extern CSoundCard	g_SoundCard;

#endif // !defined(AFX_SOUNDCARD_H__E0E45662_80AB_11D3_9DE4_006097552948__INCLUDED_)
