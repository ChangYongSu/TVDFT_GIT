
#ifndef	__COMMTHREAD_H_
#define __COMMTHREAD_H_


//---------------------------- 상수 정의 --------------------------//
#define	BUFF_SIZE		8192
///#define	WM_COMM_READ	(WM_USER+2)		

//#define	ASCII_LF		0x0a
//#define	ASCII_CR		0x0d
#define	ASCII_XON		0x11
#define	ASCII_XOFF		0x13

// Queue 클래스 정의 //
class	CQueue
{
public:
	BYTE	buff[BUFF_SIZE];
	int		m_iHead, m_iTail;
	CQueue();
	void	Clear();
	int		GetSize();
	BOOL	PutByte(BYTE b);		// 1 byte 넣기
	BOOL	GetByte(BYTE *pb);		// 1 byte 꺼내기
};


//	통신 클래스	CCommThread 

// 포트 열기 : OpenPort("COM1", CBR_9600);
// 포트에서 읽기 :
//   포트를 연 후에 포트에 자료가 도착하면 WM_COMM_READ 메시지가 메인 
//   윈도우에 전달된다. ON_MESSAGE 매크로를 이용, 함수를 연결하고
//   m_ReadData String에 저장된 데이터를 이용 읽기
// 포트에 쓰기 : WriteComm(buff, 30)과 같이 버퍼와 그 크기를 건네면 된다.

class	CCommThread
{
public:
	CCommThread( );
	//--------- 환경 변수 -----------------------------------------//
	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	BOOL		m_bConnected;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD		m_wPortID;				// WM_COMM_READ와 함께 보내는 인수.
	BYTE		abIn[2049] ;//컴포트에서 들어오는 데이타

	//--------- 통신 버퍼 -----------------------------------------//
	CQueue	m_QueueRead;

	//--------- 외부 사용 함수 ------------------------------------//
	//+ 2007.9.28 Mod BY USY
	BOOL	OpenPort(CString sPortName, DWORD dwBaud, WORD wParam, BYTE bByteSize =8, BYTE bParity=0, BYTE bStopBits=0, BOOL bWatch = FALSE);

//	BOOL	OpenPort(CString sPortName, DWORD dwBaud, WORD wParam);
	//-
	void	ClosePort();
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);

	//--------- 내부 사용 함수 ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);

	//+ 2007.11.22 Add BY USY
	int		ClearPort();
	//-
	//+ 2008.05.30 Add BY PSH
//	HANDLE	   hReadEnd;

	void SetComPort(int port,DWORD rate,BYTE bytesize,BYTE stop,BYTE parity);
	
	void SetReadData(LPTSTR data);

// Attributes
public:
   BYTE        bPort;
   BOOL         fXonXoff;
   BYTE        bByteSize, bFlowCtrl, bParity, bStopBits ;
   DWORD       dwBaudRate ;
   HANDLE hWatchThread;
//   HWND        hTermWnd ;
   DWORD       dwThreadID ;
   OVERLAPPED  osWrite, osRead ;

public:
	virtual ~CCommThread();

};

// Thread로 사용할 함수 
DWORD	ThreadWatchComm(CCommThread* pComm);


#endif	// __COMMTHREAD_H__