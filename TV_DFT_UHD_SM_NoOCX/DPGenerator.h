// HdmiGenerator.h: interface for the CHdmiGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DPGENERATOR_H__F4CD6D37_294E_4492_B1DB_11578366774B__INCLUDED_)
#define AFX_DPGENERATOR_H__F4CD6D37_294E_4492_B1DB_11578366774B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommThread.h"

#define	STX		0x02
#define	ETX		0x03

#define END_TERMINAL_MODE		0x04
#define START_TERMINAL_MODE		0x05
#define CALL_MODEL			0x07
#define CALL_PATTERN		0x08
#define CALL_MODEL_PATTERN	0x09
#define VIDEO_COLOR_CHANGE	0x0a
#define HSYNC_ONOFF			0x0b
#define VSYNC_ONOFF			0x0c
#define SET_OUT_PORT		0x1c
#define CHECK_FW_VER		0x1A//#define CHECK_FW_VER		0x1A


class CDPGenerator : public CObject 
{
public:
	CDPGenerator();
	virtual ~CDPGenerator();

	//Variable
	BOOL			m_bPortOpen;
	
	CCommThread		m_ctrlDPGenerator;
	BYTE			m_strReceive[225];
	CString			m_strErrMsg;
	int				m_nPortID;
	BOOL			m_bResetPatternModel;

	UINT			m_nRemoteMode;	//0 : offLine, 1 : online

	int				m_nModelNo;
	int				m_nPatternNo;
	
	BOOL Create(CString sComPort, DWORD nBaudRate);
	void PortClose();
	BOOL Init();
	BOOL SendByte(BYTE Data);
	BOOL SendCommString(CString str);
	BOOL ReceiveCommString(int nWaitLimit);
	BOOL SetModel(int nData);
	BOOL SetPattern(int nData);
	BOOL SetModel_Pattern(int nMode, int nPattern);
	BOOL SetEDID_PassCheck(BOOL bFlag);
	BOOL SetHDCP_OnOff(BOOL bFlag);
	BOOL SetDDCLine_OpenClose(BOOL bFlag);
	BOOL SetOnOffLine(BOOL bFlag);
	BOOL SetOut(int nMode, int nPattern);
	BOOL SetOut(int nModel, int nPattern, int Reset);
	BOOL SetOutPort(int nPort);
	BOOL SetCEC_OnOff(BOOL bFlag);
	BOOL SetARC_OnOff(int nPort, BOOL bFlag);
	BOOL SetARC_Control(BOOL bFlag);
	BOOL SetMHL_OnOff(int nPort, BOOL bFlag);
	BOOL SetMHL_Control(BOOL bFlag);

	BOOL SetDFT_Mode(int nData);

	BOOL SetTime_Control(int nOutPutTime);

	CString			m_FW_Ver;
	int			m_nReceiveCount;
	BOOL CheckVer();
};

extern CDPGenerator DPGeneratorCtrl;

#endif // !defined(AFX_HDMIGENERATOR_H__F4CD6D37_294E_4492_B1DB_11578366774B__INCLUDED_)
