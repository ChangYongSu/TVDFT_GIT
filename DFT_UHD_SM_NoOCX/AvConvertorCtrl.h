#ifndef	__AV_CONVERTOR_CTRL_H_
#define __AV_CONVERTOR_CTRL_H_

#include "CommThread.h"


class CAvConvertor : public CObject
{
public : 
	
	//Variable
	BOOL			m_bPortOpen;

	CCommThread		m_ctrlAvConvertor;
	BYTE			m_strReceive[225];

	int				m_nPortID;
	int				m_nAvSwitchCmd;
	
	CString			m_strErrMsg;
	//Function
	CAvConvertor();
	~CAvConvertor();

	BOOL	Create(CString sComPort, DWORD nBaudRate);
	void	PortClose();
	BOOL	Init();

	BOOL	SendCommString(CString str);

	UINT	CheckReceiveMsg(int nCmd, int nWaitLimit, int& nLength, CString& sValue);
	CString MakeCmdString(int nCmd,CString& szSwitchBoxMsg, int nValue1 = 0);
	BOOL	ReceiveCommString(int nRev, int nWaitLimit);
//	BOOL	SendCmd(int nCmd, int nWaitLimit, CString& sValue, int nValue1 = 0);
	
	BOOL	SetInputSignal(int nInput);
	BOOL	CheckRemoteCmd(int& nLength, CString& sValue);
	BOOL	CheckComData(int& nLength, CString& sValue);
	BOOL	CheckBufferClear();

	BOOL	InitAvSwitch();

	BOOL	IRDataCheck();
	BOOL	ComDataCheck();
	BOOL	IRBlasterDataCheck();
	
private:
	void Delay();
};


extern CAvConvertor gAvConvertorCtrl;

#endif	// __AV_CONVERTOR_CTRL_H__