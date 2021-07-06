// HdmiGenerator.h: interface for the CHdmiGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDMIGENERATOR_H__F4CD6D37_294E_4492_B1DB_11578366774B__INCLUDED_)
#define AFX_HDMIGENERATOR_H__F4CD6D37_294E_4492_B1DB_11578366774B__INCLUDED_

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

class CHdmiGenerator : public CObject 
{
public:
	CHdmiGenerator();
	virtual ~CHdmiGenerator();

	//Variable
	BOOL			m_bPortOpen;
	CCommThread		m_ctrlPatternGenerator;
	BYTE			m_strReceive[225];
	CString			m_strErrMsg;
	int				m_nPortID;

	UINT			m_nRemoteMode;	//0 : offLine, 1 : online
	BOOL			m_bHDCPMode;	//0 : off,     1 : one
	BOOL			m_bEDIDMode;	//0 : Pass,    1 : Check
	BOOL			m_bDDCLineMode;	//0 : Open,    1 : Close
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
};

extern CHdmiGenerator HDMIGeneratorCtrl;

#endif // !defined(AFX_HDMIGENERATOR_H__F4CD6D37_294E_4492_B1DB_11578366774B__INCLUDED_)
