// Pipe.cpp: implementation of the CPipe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "fister.h"
#include "Pipe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPipe::CPipe()
{
	m_SoundIn.DataFromSoundIn = DataFromSoundIn;	// assign pointer to callback function
	m_SoundIn.m_pOwner = this;
	m_SoundOut.GetDataToSoundOut = GetDataToSoundOut;  // assign pointer to callback function
	m_SoundOut.m_pOwner = this;
}

CPipe::~CPipe()
{

}

void CPipe::DataFromSoundIn(CBuffer* buffer, void* Owner)
{
	((CPipe*) Owner)->WriteSoundDataToFile(buffer);
	::SendMessage(((CPipe*) Owner)->m_hPipeMainWnd, WM_GRAPH_UPDATE, (WPARAM)buffer->ByteLen, (LPARAM)buffer->ptr.c);
}

void CPipe::WriteSoundDataToFile(CBuffer* buffer)
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

void CPipe::GetDataToSoundOut(CBuffer* buffer, void* Owner)
{
	((CPipe*) Owner)->ReadSoundDataFromFile(buffer);

	::SendMessage(((CPipe*) Owner)->m_hPipeMainWnd, WM_GRAPH_UPDATE, (WPARAM)buffer->ByteLen, (LPARAM)buffer->ptr.c);
}

void CPipe::ReadSoundDataFromFile(CBuffer* buffer)
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

void CPipe::StartRecordingToFile(CString lFileName)
{
//	m_pFile = new CSoundFile("sound1.wav", m_SoundIn.GetFormat());
	m_pFile = new CSoundFile(lFileName, m_SoundIn.GetFormat());
	if(m_pFile && m_pFile->IsOK())
		m_SoundIn.Start();
}

void CPipe::StopRecordingToFile()
{
	m_SoundIn.Stop();
	// close output file
	if(m_pFile)
		delete m_pFile;
}

void CPipe::StartPlayingFromFile(CString lFileName)
{
	
//	m_pFile = new CSoundFile(lFileName.GetBuffer(),m_SoundIn.GetFormat());//"sound1.wav"
	m_pFile = new CSoundFile(lFileName.GetBuffer());//"sound1.wav"
	

	if(m_pFile && m_pFile->IsOK())
		m_SoundOut.Start(m_pFile->GetFormat());
	else
		ErrorMsg("Unable to open file");

	//m_ReadStep = 3;
}

void CPipe::StopPlayingFromFile()
{
	m_SoundOut.Stop();
	// close output file
	if(m_pFile)
		delete m_pFile;
}

void CPipe::OnEndOfPlayingFile()
{
	// implement this function in the GUI to change things when EOF is reached
}

void CPipe::SetSamplesPerSecond(int sps)
{

		//CSoundOut	m_SoundOut;
	
	m_SoundIn.SetSamplesPerSecond(sps);
	m_SoundOut.SetSamplesPerSecond(sps);

}

int CPipe::GetSamplesPerSecond()
{
	return m_SoundIn.GetSamplesPerSecond();
}

void CPipe::SetBufferSize(int NumberOfSamples)
{
	m_SoundIn.SetBufferSize(NumberOfSamples);	
	m_SoundOut.SetBufferSize(NumberOfSamples);
}

int CPipe::GetBufferSize()	
{
	return m_SoundIn.GetBufferSize();
}
