#ifndef	__AV_CONVERTOR_CTRL_H_
#define __AV_CONVERTOR_CTRL_H_

#include "CommThread.h"


class CIrChecker : public CObject
{
public : 
	
	//Variable
	BOOL			m_bPortOpen;

	CCommThread		m_ctrlIrChecker;
	BYTE			m_strReceive[225];

	int				m_nPortID;
	int				m_nAvSwitchCmd;
	
	CString			m_strErrMsg;
	//Function
	CIrChecker();
	~CIrChecker();

	BOOL	Create(CString sComPort, DWORD nBaudRate);
	void	PortClose();
	BOOL	Init();

	BOOL	SendCommString(CString str);

	UINT	CheckReceiveMsg(int nCmd, int nWaitLimit, CString& sValue);
	CString MakeCmdString(int nCmd,CString& szSwitchBoxMsg);
	BOOL	ReceiveCommString(int nRev, int nWaitLimit);
	BOOL	CheckRemoteCmd(CString& sValue, int nMode = 1);
	BOOL	CheckBufferClear();

	BOOL	IRDataCheck();
	
private:
	void Delay();
};


extern CIrChecker gIrCheckerCtrl;

#endif	// __AV_CONVERTOR_CTRL_H__