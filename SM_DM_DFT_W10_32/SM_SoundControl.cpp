// Pipe.cpp: implementation of the CSM_SoundControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Defines.h"
#include "DATsys.h"
#include "global.h"
#include "SM_SoundControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSM_SoundControl::CSM_SoundControl()
{
	m_SoundIn.DataFromSoundIn = DataFromSoundIn;	// assign pointer to callback function
	m_SoundIn.m_pOwner = this;
	m_SoundOut.GetDataToSoundOut = GetDataToSoundOut;  // assign pointer to callback function
	m_SoundOut.m_pOwner = this;
}

CSM_SoundControl::~CSM_SoundControl()
{

}
void CSM_SoundControl::StartCapture(CWnd *parent, int nDeviceID)
{
	
	int lFound = 0;
	int lError = SelectDevice(0);
	if (m_aStrSoundDevice.GetSize() <= nDeviceID)
	{
		
		return;
	}

	//if (g_nRunningProcessNo == 1)
	//{
	//}
	//else
	//{
	//	m_SoundIn.m_uDeviceID = 2;
	//}
	//	
	m_SoundIn.m_uDeviceID = nDeviceID;

	//if(m_aStrSoundDevice)

	//for (int i = 0 ; i < m_aStrSoundDevice.GetSize(); i++)
	//{
	//	if (CurrentSet->sAudioDevice.CompareNoCase(m_aStrSoundDevice.GetAt(i)) == 0)
	//	{
	//		m_SoundIn.m_uDeviceID = i;
	//		lFound = 1;
	//		break;
	//	}
	//}
	//if (lFound == 0)
	//	retrun ;

	m_hSoundMainWnd = parent->m_hWnd;
	//m_pFile = new CSoundFile(lFileName, m_SoundIn.GetFormat());
	//if(m_pFile && m_pFile->IsOK())
	m_SoundIn.Start();
}
void CSM_SoundControl::StopCapture()
{
	m_SoundIn.Stop();

}

int CSM_SoundControl::SelectDevice(UINT nDevNum)
{
	int lError = 0;
	//CComboBox *pBox = (CComboBox*)GetDlgItem(IDC_DEVICES);
	UINT nDevices, nC1;
	WAVEINCAPS stWIC = { 0 };
	MMRESULT mRes;
	CString Strtemp;

	//pBox->ResetContent();
	nDevices = waveInGetNumDevs();
	
	

	m_aStrSoundDevice.RemoveAll();
	for (nC1 = 0; nC1 < nDevices; ++nC1)
	{
		ZeroMemory(&stWIC, sizeof(WAVEINCAPS));
		mRes = waveInGetDevCaps(nC1, &stWIC, sizeof(WAVEINCAPS));
		if (mRes == 0)
			m_aStrSoundDevice.Add(stWIC.szPname);// pBox->AddString(stWIC.szPname);
		else
		{
			Strtemp.Format("File: %s ,Line Number:%d, mRes: %d", __FILE__, __LINE__, mRes);
			TRACE(Strtemp);
				//m_aStrSoundDevice.Add(stWIC.szPname);//// StoreError(mRes, TRUE, "File: %s ,Line Number:%d", __FILE__, __LINE__);
		}
	}

	
	return lError;
}


void CSM_SoundControl::DataFromSoundIn(CBuffer* buffer, void* Owner)
{
	//((CSM_SoundControl*) Owner)->WriteSoundDataToFile(buffer);
	::SendMessage(((CSM_SoundControl*) Owner)->m_hSoundMainWnd, WM_GRAPH_UPDATE, (WPARAM)buffer->ByteLen, (LPARAM)buffer->ptr.c);
}

void CSM_SoundControl::WriteSoundDataToFile(CBuffer* buffer)
{
	if(m_pFile)
	{
		if(!m_pFile->Write(buffer))
		{
			m_SoundIn.Stop();
			AfxMessageBox("Unable to write to file");
		}
	}

	/*
	TRACE("InTop: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());

	// add to fifo buffer
	m_FifoFull.Add(buffer);
	buffer = (CBuffer*) m_FifoEmpty.Consume();

	TRACE("InEnd: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());
	*/
}

void CSM_SoundControl::GetDataToSoundOut(CBuffer* buffer, void* Owner)
{
	((CSM_SoundControl*) Owner)->ReadSoundDataFromFile(buffer);

	::SendMessage(((CSM_SoundControl*) Owner)->m_hSoundMainWnd, WM_GRAPH_UPDATE, (WPARAM)buffer->ByteLen, (LPARAM)buffer->ptr.c);
}

void CSM_SoundControl::ReadSoundDataFromFile(CBuffer* buffer)
{
	//if(m_ReadStep == 1)
	//{
	//	m_ReadStep = 2;
	//}
	//else if(m_ReadStep == 2)
	//{
	//	return;
	//}

	if(m_pFile)
	{
		if(!m_pFile->Read(buffer))
		{
			// enf of file -> tell the GUI
			OnEndOfPlayingFile();
			delete m_pFile;
		}
	}

	/*
	TRACE("OutTop: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());

	// consume from fifo buffer
	m_FifoEmpty.Add(buffer);
	buffer = (CBuffer*) m_FifoFull.Consume();

	TRACE("OutEnd: Full=%2d, Empty=%2d\n", m_FifoFull.GetCount(), m_FifoEmpty.GetCount());
	*/
}

void CSM_SoundControl::StartRecordingToFile(CString lFileName)
{
//	m_pFile = new CSoundFile("sound1.wav", m_SoundIn.GetFormat());
	m_pFile = new CSoundFile(lFileName, m_SoundIn.GetFormat());
	if(m_pFile && m_pFile->IsOK())
		m_SoundIn.Start();
}

void CSM_SoundControl::StopRecordingToFile()
{
	m_SoundIn.Stop();
	// close output file
	if(m_pFile)
		delete m_pFile;
}

void CSM_SoundControl::StartPlayingFromFile(CString lFileName)
{
	
//	m_pFile = new CSoundFile(lFileName.GetBuffer(),m_SoundIn.GetFormat());//"sound1.wav"
	m_pFile = new CSoundFile(lFileName.GetBuffer());//"sound1.wav"
	

	if(m_pFile && m_pFile->IsOK())
		m_SoundOut.Start(m_pFile->GetFormat());
	else
		ErrorMsg("Unable to open file");

	//m_ReadStep = 3;
}

void CSM_SoundControl::StopPlayingFromFile()
{
	m_SoundOut.Stop();
	// close output file
	if(m_pFile)
		delete m_pFile;
}

void CSM_SoundControl::OnEndOfPlayingFile()
{
	// implement this function in the GUI to change things when EOF is reached
}

void CSM_SoundControl::SetSamplesPerSecond(int sps)
{

		//CSoundOut	m_SoundOut;
	
	m_SoundIn.SetSamplesPerSecond(sps);
	m_SoundOut.SetSamplesPerSecond(sps);

}

int CSM_SoundControl::GetSamplesPerSecond()
{
	return m_SoundIn.GetSamplesPerSecond();
}

void CSM_SoundControl::SetBufferSize(int NumberOfSamples)
{
	m_SoundIn.SetBufferSize(NumberOfSamples);	
	m_SoundOut.SetBufferSize(NumberOfSamples);
}

int CSM_SoundControl::GetBufferSize()	
{
	return m_SoundIn.GetBufferSize();
}
