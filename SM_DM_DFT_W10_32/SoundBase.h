// SoundBase.h: interface for the CSoundBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDBASE_H__041D24CC_3A2F_4E33_AAB9_6C98297DAC59__INCLUDED_)
#define AFX_SOUNDBASE_H__041D24CC_3A2F_4E33_AAB9_6C98297DAC59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include "buffer.h"
#define SAMPLE_DISPLAYCOUNT	1000
#define SAMPLE_BUFFER_MAX	2048
#define SAMPLE_BUFFER_CHECK_MAX	2038
#define SAMPLE_BYTE_MAX		4096
#define SAMPLE_RATE_PER_SECOND     44100

typedef struct {
	double freq, vol;
} _AudSignal_T;

class CSoundBase  
{
public:
	WAVEFORMATEX* GetFormat();
	int GetBufferSize();
	void SetBufferSize(int NumberOfSamples);
	int GetNumberOfChannels();
	void SetNumberOfChannels(int nchan);
	int GetSamplesPerSecond();
	void SetSamplesPerSecond(int sps);
	int GetBitsPerSample();
	void SetBitsPerSample(int bps);
	CSoundBase();
	virtual ~CSoundBase();

protected:
	WAVEFORMATEX m_Format;
	int			 m_BufferSize;	// number of samples

private:
	void Update();
};

#endif // !defined(AFX_SOUNDBASE_H__041D24CC_3A2F_4E33_AAB9_6C98297DAC59__INCLUDED_)
