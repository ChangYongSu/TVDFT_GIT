// 명    칭 : DAT(Digital Auto Test)
// 창작일자 : 2000. 4. 0
// 소    속 : LG전자(LG생산기술원 MIT 박승화)
// File  명 : Inst.h


#ifndef	__INSTRUMENTS_H_
#define __INSTRUMENTS_H_

#include <afxmt.h>

#include <conio.h>      // for ISA Bus, _inp, _oup
#include "Resource.h"
//#include "comm.h"
#include "CommThread.h"




class CThermometer : public CObject
{
public :
	CThermometer();
	virtual ~CThermometer();

//	CMSComm	m_CommCtrl;
	CCommThread	m_CommCtrl;
	BOOL	CreateComm(CString sComPort, DWORD nBaudRate);
	void	CloseComm();
	BOOL	SendString(CString str);
	BOOL	ReceiveString(CString &str, int nCount);

	double	MeasureTh();

	int		m_nPortID;
	BYTE			m_strReceive[225];
	BOOL	m_bCommError;
	CString	m_strErrMsg;		
	BOOL	m_bPortOpen;
private:

};



// Global Object
extern CThermometer gThermometerCtrl;

#endif	// __INSTRUMENTS_H__