/***********************************************************

	FileName : comm_packet.h

	Provider : NaenTech, Inc.
	Version 1.0
	Date :
		2015.12.28

	Supported O.S. :
		- MS Windows : above Windows XP(SP3)
		- Linux : above Linux kernel 3.x

 ***********************************************************/

#if !defined(__COMM_PACKET_H)
#define  __COMM_PACKET_H

#pragma once

#include "lgdserver_def.h"

/////////////////////////////////////////////////
//	Define Data endian
//
#define	BYTE_ORDER_LITTLE_ENDIAN	0	//!< Define Little Endian Type
#define	BYTE_ORDER_BIG_ENDIAN		1	//!< Define Big Endian Type

#define	USE_ENDIAN_TYPE				BYTE_ORDER_LITTLE_ENDIAN	//!< Define current Endian Type

#define FPGA_REG_WINDOWTIME		0x20
#define FPGA_REG_LINETIME		0x21
#define FPGA_REG_GATEON		0x22
#define FPGA_REG_RESETTIME		0x24
#define FPGA_REG_CDS1		0x25
#define FPGA_REG_CDS2		0x26
#define FPGA_REG_FA1		0x27
#define FPGA_REG_FA2		0x28
#define FPGA_REG_ACLK3		0x29
#define FPGA_REG_GATEONDELAY		0x2A
#define FPGA_REG_ACLK0		0x2B
#define FPGA_REG_SENSITIVITY		0x02
#define FPGA_REG_IMAGEMODE		0x07
#define FPGA_REG_IMAGEBITMODE		0x08
#define FPGA_REG_LOWPASSFILTER		0x0D
#define FPGA_REG_REFADC		0x0C
#define FPGA_REG_ACLK7		0x2C
#define FPGA_REG_PRINTROIC		0x70
#define FPGA_REG_EXPOSURE		0x04
#define FPGA_REG_BACKBIAS		0x42
#define FPGA_REG_POWERMODE		0x0F
#define FPGA_REG_STANDBYMODE	0x40
#define FPGA_REG_INITDUMMY		0x30
#define FPGA_REG_CLEANDUMMY		0x33
#define FPGA_REG_DARKFLUSH		0x34
#define FPGA_REG_BRIGHTFLUSH	0x32

#define FPGA_MICROCHUP_UPDATE	0xF0
#define FPGA_TXPOWERSET			0xF1

// for test
#define FPGA_REG_POWER	0x100

#define FPGA_REG_ROIC0		0xE7
#define FPGA_REG_ROIC1		0xE6
#define FPGA_REG_ROIC2		0xE5
#define FPGA_REG_ROIC3		0xE4
#define FPGA_REG_ROIC4		0xE3
#define FPGA_REG_ROIC5		0xE2
#define FPGA_REG_ROIC6		0xE1
#define FPGA_REG_ROIC7		0xE0


/*!
	\brief The kind of Command.
*/
//enum PacketCommand
//{
//	LDCMD_UNKNOWN					= 0x0000,	//!< 
//
//	LDCMD_GET_PARAMETER				= 0x1001,	//!< Get Detector Parameter. See \ref DETECTOR_PARAM
//	LDCMD_SET_PARAMETER				= 0x1002,	//!< Set Detector Parameter See \ref DETECTOR_PARAM
//	LDCMD_GET_FACTORY_PARAMETER		= 0x1003,	//!< Get Detector Parameter of factory. See \ref DETECTOR_PARAM
//	LDCMD_SET_FACTORY_PARAMETER		= 0x1004,	//!< Set Detector Parameter of factory. See \ref DETECTOR_PARAM
//
//	LDCMD_GET_DETECTOR_STATUS		= 0x1005,	//!< Get Detector Status. See \ref ResponseCode
//	LDCMD_GET_MONITORING_DATA		= 0x1006,	//!< Get Monitoring Data. See \ref MONITORING_DATA
//
//	LDCMD_GET_PARAMETER_RANGE = 0x1007,	//!< Get the range of Detector Parameter Value.
//	LDCMD_SET_PARAMETER_RANGE = 0x1008,	//!< Set the range of Detector Parameter Value.
//
//	LDCMD_GET_DARK_IMAGE			= 0x1011,	//!< Get Dark Image
//	LDCMD_GET_BRIGHT_IMAGE			= 0x1012,	//!< Get Bright Image
//	LDCMD_GET_LATEST_IMAGE			= 0x1013,	//!< Get Latest Image
//
//	LDCMD_CANCEL_JOB				= 0x1014,	//!< Cancel GetBright Job
//
//	LDCMD_UPLOAD_SOC				= 0x1021,	//!< Upload SOC. \arg Firmware, \arg DServer, \arg MiCom, \arg FPGA
//	LDCMD_LOAD_CALDATA				= 0x1022,	//!< Load Calibration Data
//	LDCMD_SAVE_CALDATA				= 0x1023,	//!< Save Calibration Data
//
//	LDCMD_GET_TIME					= 0x1024,	//!< Get system time. See \ref SET_TIME_DATA
//	LDCMD_SET_TIME					= 0x1025,	//!< Set sytem time. See \ref SET_TIME_DATA
//
//	LDCMD_GET_NETWORK_INFO			= 0x1026,	//!< Get Network Info. See \ref NETWORK_INFO_DATA
//	LDCMD_SET_NETWORK_INFO			= 0x1027,	//!< Set Network Info. See \ref NETWORK_INFO_DATA
//
//	LDCMD_GET_SYSTEM_INFO			= 0x1028,	//!< Get Network Info. See \ref SYSTEM_INFO_DATA
//	LDCMD_SET_SYSTEM_INFO			= 0x1029,	//!< Set Network Info. See \ref SYSTEM_INFO_DATA
//
//	LDCMD_GET_HISTORY_DATA			= 0x1030,	//!< 
//	LDCMD_RESET_SYSTEM				= 0x1031,	//!< 
//
//	LDCMD_SET_FPGAREGISTER			= 0x1041,	//!<
//
//	LDCMD_GET_PARAMETER2			= 0x1051,	//!< Get Detector Parameter. See \ref DETECTOR_PARAM
//
//	LDCMD_GET_ROIC_DATA				= 0x1061,	//!< Get ROIC data
//	LDCMD_SET_ROIC_DATA				= 0x1062,	//!< Set ROIC data
//	LDCMD_SET_MICROCHIP_COMMAND		= 0x1063,	//!< Set Microchip command
//	LDCMD_SET_AVR_COMMAND			= 0x1064,	//!< Set AVR command
//
//	LDCMD_GET_WIFI_MODULE			= 0x1071,	//!< Get WiFi module is cinnected ot not
//};
enum PacketCommand
{
	LDCMD_UNKNOWN = 0x0000,	//!< 

	LDCMD_GET_PARAMETER = 0x1001,	//!< Get Detector Parameter. See \ref DETECTOR_PARAM
	LDCMD_SET_PARAMETER = 0x1002,	//!< Set Detector Parameter See \ref DETECTOR_PARAM
	LDCMD_GET_FACTORY_PARAMETER = 0x1003,	//!< Get Detector Parameter of factory. See \ref DETECTOR_PARAM
	LDCMD_SET_FACTORY_PARAMETER = 0x1004,	//!< Set Detector Parameter of factory. See \ref DETECTOR_PARAM

	LDCMD_GET_DETECTOR_STATUS = 0x1005,	//!< Get Detector Status. See \ref ResponseCode
	LDCMD_GET_MONITORING_DATA = 0x1006,	//!< Get Monitoring Data. See \ref MONITORING_DATA

	LDCMD_GET_PARAMETER_RANGE = 0x1007,	//!< Get the range of Detector Parameter Value.
	LDCMD_SET_PARAMETER_RANGE = 0x1008,	//!< Set the range of Detector Parameter Value.

	LDCMD_GET_DARK_IMAGE = 0x1011,	//!< Get Dark Image
	LDCMD_GET_BRIGHT_IMAGE = 0x1012,	//!< Get Bright Image
	LDCMD_GET_BRIGHT_CALMAP_IMAGE = 0x1015,	//!< Get Bright Image
	LDCMD_GET_LATEST_IMAGE = 0x1013,	//!< Get Latest Image

	LDCMD_CANCEL_JOB = 0x1014,	//!< Cancel GetBright Job

	LDCMD_UPLOAD_SOC = 0x1021,	//!< Upload SOC. \arg Firmware, \arg DServer, \arg MiCom, \arg FPGA
	LDCMD_LOAD_CALDATA = 0x1022,	//!< Load Calibration Data
	LDCMD_SAVE_CALDATA = 0x1023,	//!< Save Calibration Data

	LDCMD_GET_TIME = 0x1024,	//!< Get system time. See \ref SET_TIME_DATA
	LDCMD_SET_TIME = 0x1025,	//!< Set sytem time. See \ref SET_TIME_DATA

	LDCMD_GET_NETWORK_INFO = 0x1026,	//!< Get Network Info. See \ref NETWORK_INFO_DATA
	LDCMD_SET_NETWORK_INFO = 0x1027,	//!< Set Network Info. See \ref NETWORK_INFO_DATA

	LDCMD_GET_SYSTEM_INFO = 0x1028,	//!< Get Network Info. See \ref SYSTEM_INFO_DATA
	LDCMD_SET_SYSTEM_INFO = 0x1029,	//!< Set Network Info. See \ref SYSTEM_INFO_DATA

	LDCMD_GET_HISTORY_DATA = 0x1030,	//!< 
	LDCMD_RESET_SYSTEM = 0x1031,	//!< 
	LDCMD_FACTORY_RESET = 0x1032,	//!< 

	LDCMD_SET_FPGAREGISTER = 0x1041,	//!<
	LDCMD_GET_FPGAREGISTER = 0x1042,	//!<
	LDCMD_SET_FPGAREGISTER2 = 0x1043,	//!<
	LDCMD_GET_FPGAREGISTER2 = 0x1044,	//!< get line buffer data

	LDCMD_GET_PARAMETER2 = 0x1051,	//!< Get Detector Parameter. See \ref DETECTOR_PARAM

	LDCMD_GET_ROIC_DATA = 0x1061,	//!< Get ROIC data
	LDCMD_SET_ROIC_DATA = 0x1062,	//!< Set ROIC data
	LDCMD_SET_MICROCHIP_COMMAND = 0x1063,	//!< Set Microchip command
	LDCMD_SET_AVR_COMMAND = 0x1064,	//!< Set AVR command
	LDCMD_SET_AUTOEXPOSURE = 0x1065,	//!<

	LDCMD_SET_AGINGMODE = 0x1066,	//!<

	LDCMD_GET_WIFI_MODULE = 0x1071,	//!< Get WiFi module is cinnected ot not
	LDCMD_SET_ANTENNA_POWER = 0x1072,	//!< Set Antenna transmitter power
	LDCMD_SET_WIFI_TEST = 0x1073,	//!< Set wifi test mode

	LDCMD_GET_SHOCK_DATA = 0x1080,	//!< Get Shock data
	LDCMD_SET_INSTALL_DATE = 0x1090,	//!< Set DXD Install date

	LDCMD_GET_MAC_ADDRESS = 0x2003,	//!<
	LDCMD_SET_MAC_ADDRESS = 0x2004,	//!<
	LDCMD_UPLOAD_BOOTLOADER = 0x2005,	//!<
};


/*!
	\brief The kind of Response Code.
*/
enum ResponseCode
{
	LDRET_SUCCESS					= 0x0000,	//!< Success
	LDRET_FAIL						= 0x0001,	//!< 
	LDRET_CANCELED					= 0x0002,	//!< Job is canceled.
	LDRET_INVALID_PACKET			= 0x0003,	//!< Invalid Packet Data
	LDRET_NET_ERROR					= 0x0004,	//!< Network error.

	LDRET_MEM_ALLOC_FAIL			= 0x0011,	//!< Memory allocation fail
	LDRET_INVALID_FILE_SIZE			= 0x0012,	//!< Invalid file size
	LDRET_INVALID_FILE_DATA			= 0x0013,	//!< Invalid file Data
	LDRET_OPEN_FILE_FAIL			= 0x0014,	//!< Open file is fail
	LDRET_WRITE_FILE_FAIL			= 0x0015,	//!< Write to file is fail
	LDRET_SAVE_ETHERNET_INFO_FAIL	= 0x0016,	//!< Save ethernet information is fail.
	LDRET_SAVE_WIFI_INFO_FAIL		= 0x0017,	//!< Save wireless information is fail.

	//Detector's Status
	LDRET_STATUS_READY				= 0x1010,	//!< The Detector status is Ready.
	LDRET_STATUS_BUSY				= 0x1011,	//!< The Detector status is Busy.
	LDRET_STATUS_PREPARE_SIGNAL		= 0x1012,	//!< Prepare signal is Notified.
	LDRET_STATUS_EXPOSURE_SIGNAL	= 0x1013,	//!< Exposure signal is Notified.
	LDRET_STATUS_WINDOW_OPEN		= 0x1014,	//!< Detector's Window is opened.
	LDRET_STATUS_WINDOW_CLOSED		= 0x1015,	//!< Detector's Window is closed.
	LDRET_STATUS_IMAGE_READY		= 0x1016,	//!< Image data is ready for sending.
	LDRET_STATUS_CLEARING			= 0x1017,	//!< The Detector status is clearing.
	LDRET_STATUS_POWER_SAVING		= 0x1018,	//!< The Detector status is power saving.

	LDRET_STATUS_FPGA_ERROR = 0x1F00,	//!< 
	LDRET_STATUS_ERROR = 0x1FFF,	//!< 
};


#define	LGD_COMM_ACK				0xAAAA	//!< Define the ACK code for tcp comm.

#define	SZ_PACKET_MAGIC_STR			"LGED"	//!< Define of Magic String for struct \ref COMM_PACKET_HEADER
#define	SIZE_PACKET_MAGIC_STR		4		//!< Define of length of Magic String for struct \ref COMM_PACKET_HEADER

/*!
	\brief	Definition of Comm Packet Header.

	 After this Header, [dwDataSize] bytes data is sent to dest.
*/
typedef struct __tagCommPacketHeader__
{
	char	szMagicStr[SIZE_PACKET_MAGIC_STR];	//!< Fixed data. "LGED"
	DWORD	dwCmd;			//!< Packet Command.  See \ref PacketCommand
	DWORD	dwResponseCode;	//!< Response Code
	DWORD	dwDataSize;		//!< Data size.
} COMM_PACKET_HEADER;

#define	SIZE_COMM_PACKET_HEADER		((int)sizeof(COMM_PACKET_HEADER))

#if defined(__cplusplus)

//	CCommPacket
/*!
	\brief	Command Packet Manager between dserver And application.
*/
class CCommPacket : public COMM_PACKET_HEADER
{
public:
	//optional
	BYTE*	m_pDataBuf;

public:
	
	//GetCommandName
	/*!
		\brief	Convert given command code to string.

		\param	[in]  nCmd : Command Code. \ref PacketCommand.
		\param	[out] pszBuffer : The Data Buffer to be stored.
		\param	[in]  nBufSize : sizeof (pszBuffer)

		\return	The pointer of buffer. It same to pszBuffer
	*/
	static const char * GetCommandName
		(
			__in  PacketCommand nCmd,
			__out_ecount(nBufSize) char * pszBuffer,
			__in  int nBufSize
		);

	//GetResponseCodeName
	/*!
		\brief	Convert given Response code to string.

		\param	[in]  nResponseCode : Response Code. \ref ResponseCode.
		\param	[out] pszBuffer : The Data Buffer to be stored.
		\param	[in]  nBufSize : sizeof (pszBuffer)

		\return	The pointer of buffer. It same to pszBuffer
	*/
	static const char * GetResponseCodeName
		(
			__in  ResponseCode nResponseCode,
			__out_ecount(nBufSize) char * pszBuffer,
			__in  int nBufSize
		);

	//SetCommError
	/*!
		\brief	Set Comm Error Code and message to CWError Object.

		\param	[out] wErr : CWError object to be sotred.
		\param	[in] nErrCode : ErrCode Code. \ref ResponseCode.

		\return	The reference of wErr. It same to wErr
	*/
	static CWError & SetCommError
		(
			__out CWError & wErr,
			__in  ResponseCode nErrCode
		);

public:
	CCommPacket();			//!< Constructure
	virtual ~CCommPacket();	//!< Destructure

	void Reset();

	static int GetPacketHeaderSize();

	//GetPacketHeader
	/*!
		\brief	Get packed header data from this object.

		\param	[out] pBuf : The Data Buffer to be stored.
		\param	[in]  nBufSize : sizeof (pBuf)

		\return	The size of consumed data.
	*/
	int GetPacketHeader
		(
			__out_ecount(nBufSize) LPVOID pBuf,
			__in int nBufSize
		) const;

	//GetPacketHeader
	/*!
		\brief	Set packed header data to this object.

		\param	[in]  pBuf : The Data Buffer to be stored.
		\param	[in]  nBufSize : sizeof (pBuf)

		\return	The size of consumed data.
	*/
	int SetPacketHeader
		(
			__in_ecount(nBufSize) LPVOID pBuf,
			__in int  nBufSize
		);

	//SetHeaderData
	/*!
		\brief	Set header data only to this object.

		\param	[in]  dwCmd : Packet Command. See \ref PacketCommand.
		\param	[in]  dwResponseCode : Response Code. See \ref ResponseCode.
		\param	[in]  dwDataSize : Size of data.

		\return	The size of saved data. It should be same to \ref SIZE_COMM_PACKET_HEADER
	*/
	int SetHeaderData
		(
			__in  DWORD dwCmd,
			__in  DWORD dwResponseCode,
			__in  DWORD dwDataSize
		);

	//IsValidCommand
	/*!
		\brief	Check the command is valid using \ref dwCmd field of this object.

		\return	TRUE if success, else FALSE
	*/
	BOOL IsValidCommand() const;


	//AllocDataBuffer
	/*!
		\brief	Alloc data buffer using \ref dwDataSize field of this object.

		\return	TRUE if success, else FALSE
	*/
	BOOL AllocDataBuffer();


	//SendPacket
	/*!
		\brief	Send data to destination using this object.

		\param	[in]  tcpSoc : Socket Obejct.
		\param	[in]  bSendHeaderOnly : If true, send header only, else send header And Data.
		\param	[in]  pEventCancel : The Event Object. Using this object, The caller can cancel send operation anytime. It can be NULL

		\return	The CWError object.
	*/
	CWError SendPacket
		(
			__in  CWSocket & tcpSoc,
			__in  bool bSendHeaderOnly,
			__in_opt CWEvent * pEventCancel = NULL
		);


	//RecvPacket
	/*!
		\brief	Receive data from client and save to this object.

			The Header And Data is received all.

		\param	[in]  tcpSoc : Socket Obejct.
		\param	[in]  pEventCancel : The Event Object. Using this object, The caller can cancel receive operation anytime. It can be NULL

		\return	The CWError object.
	*/
	CWError RecvPacket
		(
			__in  CWSocket & tcpSoc,
			__in_opt CWEvent * pEventCancel = NULL
		);

	//SendHandShakeCode
	/*!
		\brief	Send Hand Shake Code when GetBright Command.

		\param	[in]  tcpSoc : Socket Obejct.
		\param	[in]  dwCode : The Code to sent.  See \ref ResponseCode.
		\param	[in]  pEventCancel : The Event Object. Using this object, The caller can cancel receive operation anytime. It can be NULL

		\return	true if success, else false.
	*/
	static bool SendHandShakeCode
		(
			__in  CWSocket & tcpSoc,
			__in  DWORD dwCode,
			__in  CWEvent * pEventCancel = NULL
		);


	//CheckACK
	/*!
		\brief	Recv ACK Code and check valid.

		See \ref LGD_COMM_ACK.

		\param	[in]  tcpSoc : Socket Obejct.
		\param	[in]  pEventCancel : The Event Object. Using this object, The caller can cancel receive operation anytime. It can be NULL

		\return	true if success, else false.
	*/
	static bool CheckACK(CWSocket & tcpSoc, CWEvent * pEventCancel);

public:
	static WORD  PacketDataToHostData(WORD wData);
	static DWORD PacketDataToHostData(DWORD dwData);
	static float PacketDataToHostData(float fData);

	static WORD  HostDataToPacketData(WORD wData);
	static DWORD HostDataToPacketData(DWORD dwData);
	static float HostDataToPacketData(float fData);

	//	return length
	static int PackData_WORD   (LPVOID pBuf, WORD wData);
	static int PackData_DWORD  (LPVOID pBuf, DWORD dwData);
	static int PackData_FLOAT  (LPVOID pBuf, float fData);

	static int UnpackData_WORD (LPVOID pBuf, WORD & wData);
	static int UnpackData_DWORD(LPVOID pBuf, DWORD & dwData);
	static int UnpackData_FLOAT(LPVOID pBuf, float & fData);
};


///////////////////////////////////////////////////////////
//	inline function
//

inline int CCommPacket::GetPacketHeaderSize()
	{	return SIZE_COMM_PACKET_HEADER;	}	

inline WORD CCommPacket::HostDataToPacketData(WORD wData)
	{
	#if (USE_ENDIAN_TYPE == BYTE_ORDER_BIG_ENDIAN)
		return htons(dwData);
	#else
		return wData;
	#endif
	}
inline DWORD CCommPacket::HostDataToPacketData(DWORD dwData)
	{
	#if (USE_ENDIAN_TYPE == BYTE_ORDER_BIG_ENDIAN)
		return htonl(dwData);
	#else
		return dwData;
	#endif
	}
inline float CCommPacket::HostDataToPacketData(float fData)
	{
	#if (USE_ENDIAN_TYPE == BYTE_ORDER_BIG_ENDIAN)
		return (float) htonl((DWORD) fData);
	#else
		return fData;
	#endif
	}
inline WORD CCommPacket::PacketDataToHostData(WORD wData)
	{
	#if (USE_ENDIAN_TYPE == BYTE_ORDER_BIG_ENDIAN)
		return ntohs(wData);
	#else
		return wData;
	#endif
	}
inline DWORD CCommPacket::PacketDataToHostData(DWORD dwData)
	{
	#if (USE_ENDIAN_TYPE == BYTE_ORDER_BIG_ENDIAN)
		return ntohl(dwData);
	#else
		return dwData;
	#endif
	}
inline float CCommPacket::PacketDataToHostData(float fData)
	{
	#if (USE_ENDIAN_TYPE == BYTE_ORDER_BIG_ENDIAN)
		return (float) ntohl((DWORD) fData);
	#else
		return fData;
	#endif
	}

inline int CCommPacket::PackData_WORD(LPVOID pBuf, WORD wData)
	{
		wData = HostDataToPacketData(wData);
		memcpy(pBuf, & wData, 2);

		return 2;
	}
inline int CCommPacket::PackData_DWORD(LPVOID pBuf, DWORD dwData)
	{
		dwData = HostDataToPacketData(dwData);
		memcpy(pBuf, & dwData, 4);

		return 4;
	}
inline int CCommPacket::PackData_FLOAT(LPVOID pBuf, float fData)
	{
		fData = HostDataToPacketData(fData);
		memcpy(pBuf, & fData, 4);

		return 4;
	}

inline int CCommPacket::UnpackData_WORD(LPVOID pBuf, WORD & wData)
	{
		memcpy(& wData, pBuf, 2);
		wData = PacketDataToHostData(wData);

		return 2;
	}
inline int CCommPacket::UnpackData_DWORD(LPVOID pBuf, DWORD & dwData)
	{
		memcpy(& dwData, pBuf, 4);
		dwData = PacketDataToHostData(dwData);

		return 4;
	}
inline int CCommPacket::UnpackData_FLOAT(LPVOID pBuf, float & fData)
	{
		memcpy(& fData, pBuf, 4);
		fData = PacketDataToHostData(fData);

		return 4;
	}

inline CWError & CCommPacket::SetCommError(CWError & wErr, ResponseCode nErrCode)
	{
		char	szBuf[256];

		wErr.SetError(nErrCode, CCommPacket::GetResponseCodeName(nErrCode, szBuf, 256));
		return wErr;
	}

#endif	//  __cplusplus

#endif	//__COMM_PACKET_H
