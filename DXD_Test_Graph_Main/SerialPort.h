/*
**	FILENAME			CSerialPort.h
**
**	PURPOSE				This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	12-11-1997
**
**	AUTHOR				Remon Spekreijse
**
**
*/
// A break was detected on input.입력시에 브레이크가 감지될때
// The CTS signal changed state.CTS 라인의 상태가 변경되었을때 (CTS:clear to send)
// The DSR signal changed state.DSR 라인의 상태가 변경되었을때 (DSR:data set ready)
// A line-status error occurred.Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY.라인 상태가 에러 일때 
// A ring indicator was detected.상대방에 호출할때...RING이라고 하죠
// The RLSD (receive-line-signal-detect) signal changed state.RLSD라인의 상태가 변경되었을때 (Receive-line-signal-detect)
// A character was received and placed in the input buffer.문자가 수신되어 입력버퍼에 들어갈때
// The event character was received and placed in the input buffer.이벤트 문자가 수신되고 입력버퍼에 들어갈때 
// The last character in the output buffer was sent. 출력버퍼의 마지막 문자를 송신할때...

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__





class CSerialPort
{														 
public:
	void ClosePort();
	// contruction and destruction
	CSerialPort();
	virtual		~CSerialPort();

	// port initialisation RunThread										
	BOOL	InitPort(CWnd* pPortOwner, 
					 UINT  portnr, 
					 UINT  baud,
					 char  parity, 
					 UINT  databits, 
					 UINT  stopsbits,
					 DWORD dwCommEvents, 
					 UINT  nBufferSize);

	// start/stop comm watching
	BOOL	StartMonitoring();  //thread발생
	BOOL	RestartMonitoring();
	BOOL	StopMonitoring();

	DWORD	GetWriteBufferSize();
	DWORD	GetCommEvents();
	DCB		GetDCB();
	void	WriteToPort(unsigned char* string, unsigned long len);
	unsigned long       total ;

	BOOL mPort_Opened;
	int  m_RxMessageID;
	

protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);
	static UINT	CommThread(LPVOID pParam);
//	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);
	static void	ReceiveChar(CSerialPort* port);
	static void	WriteChar(CSerialPort* port);

	// thread
	CWinThread*	m_Thread;

	// synchronisation objects
	CRITICAL_SECTION	m_csCommunicationSync;
	BOOL				m_bThreadAlive;

	// handles
	HANDLE		m_hShutdownEvent;
	HANDLE		m_hComm;
	HANDLE		m_hWriteEvent;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	HANDLE		m_hEventArray[3];

	// structures
	OVERLAPPED		m_ov;
	COMMTIMEOUTS	m_CommTimeouts;
	DCB				m_dcb;

	// owner window
	CWnd*		m_pOwner;

	// misc
	UINT		m_nPortNr;
	char		m_szWriteBuffer[4096];
	DWORD		m_dwCommEvents;
	DWORD		m_nWriteBufferSize;

};

#endif __SERIALPORT_H__


