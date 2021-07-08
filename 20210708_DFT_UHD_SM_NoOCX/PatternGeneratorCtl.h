#ifndef	__PATTERN_GENERATOR_CTL_H_
#define __PATTERN_GENERATOR_CTL_H_

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



/*
#define	C_RD	0x20
#define	C_WR	0x30
#define C_RUN	0x40
*/
/*
typedef struct tag_PatternGeneratorData
{
    char* pszFunc;              // function name
	BYTE  SL_ADD;
	BYTE  PC_ADD;	
    BYTE  Freq;					// PatternGenerator Frequency
    BYTE  Duty;                 // PatternGenerator Signal Duty
	BYTE  Reapeat;
	BYTE  HF;
	BYTE  eData[4];	
	BYTE  Data[220];
} _PatternGeneratorData;
*/

class CPatternGenerator : public CObject
{
public : 

	//Variable
	BOOL			m_bPortOpen;
	CCommThread		m_ctrlPatternGenerator;
	BYTE			m_strReceive[225];
	CString			m_strErrMsg;
	int				m_nPortID;
	int				m_nModelNo;
	int				m_nPatternNo;
	
	//Function
	CPatternGenerator();
	~CPatternGenerator();
	BOOL Create(CString sComPort, DWORD nBaudRate);
	void PortClose();
	BOOL Init();
	BOOL SendByte(BYTE Data);
	BOOL SendCommString(CString str);
	BOOL SendRun(BYTE nCmd, int nData);
	BOOL SetModel(int nData);
	BOOL SetPattern(int nData);
	BOOL SetModel_Pattern(int nMode, int nPattern);
	BOOL SetOut(int nMode, int nPattern);
	BOOL ReceiveCommString(int nWaitLimit);
	BOOL SetOnOffLine(BOOL bFlag);
private:
	void Delay();
};


extern CPatternGenerator PatternGeneratorCtrl;
//extern _PatternGeneratorData PgRdData;

#endif	// __PATTERN_GENERATOR_CTL_H__