#pragma once
//#include "stdafx.h"

// CDlgModelSetup ��ȭ �����Դϴ�.

class CFileControl 
{
	

public:
	CFileControl();   // ǥ�� �������Դϴ�.
	virtual ~CFileControl();

// ��ȭ ���� �������Դϴ�.




public:	

	int  OpenModel(CString lFilename);
	
	int SaveModel(CString lFilename);
	int RemoveModel(CString lFilename);
	int SaveINIModel();
	int Read_IniModel();

	//int Read_ConfigData();
	//int SaveConfigData();

	int Read_ConfigSeqData();
	int SaveConfigSeqData();

	int LoadData();
	int SaveLog(SaveLogDataT lSaveData );// 
	int SaveLogScan(SaveLogDataT lSaveData );// 
	int SaveGmesLOG(CString LogStr, int lRev = 0);
	int SavePLCLOG(CString LogStr, int lRev = 0);
	
	
	int SaveCountData();
	int ReadCountData();

	int SaveCableCountData();
	int ReadCableCountData();

	int OpenModelSeq(CString lFilename);
	CString FindStepName(CString StringLine);
	CString FindStepWork(CString StringLine);

	unsigned char  ReadHex(CString lstrASCII);

	//int CFileControl::SavePLCAlarm(CString LogStr);
	//int m_AlarmCount;
	int  CFileControl::ReadAlarmCount(CString Filename);

public:
	
	
	
};
