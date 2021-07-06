#ifndef	__AV_SWITCH_BOX_CTRL_H_
#define __AV_SWITCH_BOX_CTRL_H_

#include "CommThread.h"


#define DC_OUT_VOLTAGE_0_0			0x00	// 0.0V
#define DC_OUT_VOLTAGE_1_0			0x01	// 1.0V
#define DC_OUT_VOLTAGE_2_0			0x02	// 2.0V
#define DC_OUT_VOLTAGE_2_5			0x03	// 2.5V
#define DC_OUT_VOLTAGE_3_3			0x04	// 3.3V

class CAvSwitchBox : public CObject
{
public : 
	
	//Variable
	BOOL			m_bPortOpen;

	CCommThread		m_ctrlAvSwitchBox;
	BYTE			m_strReceive[225];

	int				m_nPortID;
	int				m_nAvSwitchCmd;

	float			m_fTargetLeftFreq;
	float			m_fTargetRightFreq;

	float			m_fTargetLeftLevel;
	float			m_fTargetRightLevel;

	float			m_fMeasuredLeftFreq;
	float			m_fMeasuredRightFreq;

	float			m_fMeasuredLeftLevel;
	float			m_fMeasuredRightLevel;

	float			m_fLeftFreqLowerLimit;
	float			m_fLeftFreqUpperLimit;
	float			m_fRightFreqLowerLimit;
	float			m_fRightFreqUpperLimit;

	float			m_fLeftLevelLowerLimit;
	float			m_fLeftLevelUpperLimit;
	float			m_fRightLevelLowerLimit;
	float			m_fRightLevelUpperLimit;

	//+change kwmoon 080724
	float			m_fFreqMargin;

	//+add kwmoon 080724
	float			m_fLevelMargin;
	
	BOOL			m_bThreadRunning;
	BOOL			m_bResult;
	BOOL			m_bExitThread;

	HANDLE			m_hAudioMeasureThreadKillEvent;
	//+change 090213(Modification No1)
//	HANDLE			m_hThread;
	CWinThread*		m_pAudioMeasureThread;
	
	CString			m_strErrMsg;
	//+add PSH 090305
	int    m_bMeasureAudioType;
	//+add PSH 110303
	CString m_sRemoteName;
	CString m_sRemoteCustom;
	CString m_sRemoteCode;
	int		m_nRemoteByte;
	int		m_nRemoteDelay;
	int		m_nVideoSourceType;

	//+add PSH 110511
	int		m_nNoRemoteRetry;

	int		m_nAudioCh;
	int		m_nVideoCh;

	//Function
	CAvSwitchBox();
	~CAvSwitchBox();

	BOOL	Create(CString sComPort, DWORD nBaudRate);
	void	PortClose();
	BOOL	Init();

	BOOL	SendCommString(CString str);

	void	RunAudioMeasure();
	static	UINT StartAudioMeasureThread(LPVOID pParam); 

	UINT	CheckReceiveMsg(int nCmd, int nWaitLimit, CString& sValue);
	CString MakeCmdString(int nCmd,CString& szSwitchBoxMsg, int nValue1 = 0, int nValue2 = 0);
	BOOL	ReceiveCommString(int nRev, int nWaitLimit);
	BOOL	SendCmd(int nCmd, int nWaitLimit, CString& sValue, int nValue1 = 0, int nValue2 = 0);
	
	BOOL	SetAvSwitch(int nCmd, int nWaitLimit, int nValue1 = 0, int nValue2 = 0);
	BOOL	SendRemoteCmd(int nWaitLimit, int nRemoteType, CString sCustom, CString sCmd, int nByte);
	BOOL	CheckAudio(int nCmd, int nWaitLimit, CString& sValue, int nValue1 = 0, int nValue2 = 0);

	BOOL	InitAvSwitch();
	BOOL    AudioMeasure();
	//add 20110328
	BOOL	SetVideoOutType(int nVideoSourceType);

	BOOL	SetBuzzerMute(BOOL bFlag); 

private:
	void Delay();
};


extern CAvSwitchBox AvSwitchBoxCtrl;

#endif	// __AV_SWITCH_BOX_CTRL_H__