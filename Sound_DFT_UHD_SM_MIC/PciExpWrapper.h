// PciExpWrapper.h: interface for the CPciExpWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCIEXPWRAPPER_H__114BF46B_9176_44E0_983E_BDC6F7BCD4C6__INCLUDED_)
#define AFX_PCIEXPWRAPPER_H__114BF46B_9176_44E0_983E_BDC6F7BCD4C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _DLL_COMPILE
#define DLL_API	__declspec(dllexport)
#endif

#include "stdio.h"

#define		FPGA_FW_FILE_SIZE		6692572

typedef struct _ISD_DEV_INFO
{
	UINT		nDevDrvVer;
	UINT		nTotalPCICardNumber;
	UCHAR		cBusNumber;
	UCHAR		cFuncNumber;
	UCHAR		cDevNumber;
} ISD_DEV_INFO;

//typedef struct _I2C_CMD_LIST
//{
//	unsigned char		cSlaveID;
//	unsigned short int	snAddr;
//	unsigned char		cWriteVal;
//} I2C_CMD_LIST;
// 20140204 16M 수정
typedef struct _I2C_CMD_LIST
{
	unsigned char		cSlaveID ;
	unsigned short int	snAddr ;
	unsigned int		cWriteVal ;
	unsigned int		nI2cMode ;	
} I2C_CMD_LIST;


typedef struct
{
	int			wr_date_year ;
	int			wr_date_Day ;
	int			wr_code ;
	int			reser1 ;
	float		cur_offset_ch0 ;
	float		cur_offset_ch1 ;
	float		cur_offset_ch2 ;
	float		cur_offset_ch3 ;
	char		phase_tap ;
	char		reser2[7] ;
} tagGRABBER_PARAM ;

typedef struct
{
	int		firstTry[256] ;
	int		secondTry[256] ;
	int		thirdTry[256] ;
} tagI2C_RESULT ;

typedef enum
{
	FW_VESION5 = 0,
	FW_VESION6,
	FW_VESION7,
	FW_VESION8
} tagFW_VESION ;

//#define			PACK_TYPE_UHDFULL		0
//#define			PACK_TYPE_UHDNORMAL		1
//#define			PACK_TYPE_1_60HZ		2
//#define			PACK_TYPE_1_120HZ		3
//#define			PACK_TYPE_10			4
//#define			PACK_TYPE_8				5
//#define			PACK_TYPE_5				6
//#define			PACK_TYPE_UHDEPI		7
//#define			PACK_TYPE_UHDEPI60		8
//#define			PACK_TYPE_CMVIEW		9

#define			PACK_TYPE_UHDFULL		0
#define			PACK_TYPE_UHDNORMAL		1
#define			PACK_TYPE_1_60HZ		2
#define			PACK_TYPE_1_120HZ		3
#define			PACK_TYPE_10			4
#define			PACK_TYPE_8				5
#define			PACK_TYPE_5				6
#define			PACK_TYPE_UHDEPI		7
#define			PACK_TYPE_UHDEPI60		8
#define			PACK_TYPE_CMVIEW		9
#define			PACK_TYPE_5_HD_Rev		10
#define			PACK_TYPE_5_Rev_2019	11
#define			PACK_TYPE_5_Rev_2020	12
#define			PACK_TYPE_UHDFULL_LOWSPEED		13
#define			PACK_TYPE_QHD_FULL_SPEED		14
#define			PACK_TYPE_120HZ_LOW_SPEED		15


/*
0. UHD Full Speed;
1. UHD Normal Speed;
2. Pack1 (51P Only);
3. Pack1 (51P+41P);
4. Pack10;
5. Pack8;
6. Pack#5;
*/


#ifdef _DLL_COMPILE
class DLL_API CPciExpWrapper  
#else
class CPciExpWrapper  
#endif
{
private :	
//	bool			m_bRevInfo ;
//	tagIMG_INFO		m_tImgInfo ;
//	int				m_iframe_cnt ;
//	int				m_iline_cnt ;
//	int				m_iWidth ;
//	int				m_iRaw10Width ;
// 	int				m_iHeight ;
// 	__int64			m_iGrabLength ;
// 	int				m_iOutFormat ;
// 	tagGRABBER_PARAM	m_tGrabberParam ;
// 	tagFW_VESION	m_tFwVersion ;
//	bool			m_bIFCardEndian ;

public:
	int					m_nPara;
	bool				m_bErrorLog ;
	CRITICAL_SECTION	m_csI2C ;
	// PCIe
	int GetDeviceInfo				(int nMaxDevNum, ISD_DEV_INFO	*devInfo);
	int DeviceOpen					(int nDeviceNum);	// to open device
	int DeviceOpenWithDevInfo		(int nReqBusNum, int nReqFuncNum, int nReqDevNum);
	int GetDrvInfo					(int *npPciNum, int *npDrvVer, int *npBusNo, int *npFuncNo, int *npDevNo);
	int DeviceClose					();
	int DeviceReset					();	// device reset & I/F Card reset	

	int	SRIO_CfgRd					(int nInDexNum, int nDstID, int nHop, int nOff, int *nRdData, int nLocal);
	int	SRIO_CfgWr					(int nInDexNum, int nDstID, int nHop, int nOff, int *nRdData, int nLocal);

	int CM_RegisterWrite			(int nOffset, __int64 n64Value) ;
	int CM_RegisterRead				(int nOffset, __int64 *n64Value) ;
	int	CM_CheckReadDone			(int k, int nThreadID, int *nReadValue) ;

	int CM_ChkGrabDone				(int *nCurrentIndex);
	int	CM_ImageCopySafe			(int nIndexNum, int nLength, int **nBuffer);

 	int CM_MemInit					(__int64 nGrabLength, int nTimeOutMs);
 	int CM_ImageCopy				(int nIndexNum, int *nBuffer, int nLength);	
 	int CM_GrabEnable				(bool bOn) ;

	void Aurora_ReInit				();

//	int	DFT3_UHDGrabStart			(int nModel, int nShiftVal, int nWidth, int nHeight, int nOffset, int nDelay);	// 150422 cks수정
	int	DFT3_UHDGrabStart(int nModel, int nShiftVal, int nWidth, int nHeight, int nOffset, int nDelay, int nLvdsType);
	int DFT3_AnalogGrabStart		(int nModel, int nWidth, int nHeight);
	int DFT3_HDMIGrabStart			(int nModel, int nWidth, int nHeight, int nShift);
	int	DFT3_UHDGrabStop			();

	int	DFT3_EPIPuzzle				(unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
	int	DFT3_CMVPuzzle				(unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
	int	DFT3_UHDPuzzle				(int nMode, unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
	int	DFT3_HDMIPuzzle				(unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int nRotate);
	int DFT3_AnalogPuzzle			(int nMode, unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight, int *nNewWidth, int *nNewHeight, int nRotate);
	int DFT3_WhiteMerge				(int nMode, unsigned char *bufIn, unsigned char *bufOut, int nWidth, int nHeight) ;


	BOOL FLASH_Sector_Erase			(int nFlashNum, int nAddress ); 
	BOOL FLASH_SubSector_Erase		(int nFlashNum, int nAddress );
	BOOL FLASH_8x32_Write			(int nFlashNum, int nAddress, unsigned char *data, unsigned char byteLength);
	BOOL FLASH_Mod_Reset			(int nFlashNum);
	BOOL FLASH_Verification			(int nFlashNum, wchar_t* str);
	void FLASH_ResFlush				(int nFlashNum);
	bool FlashGetResFifoDcnt		(int nFlashNum, int *pDataCnt);
	bool FlashGetResFifoData		(int nFlashNum, __int64 *pData)	;
	BOOL FLASH_SubSector_Read		(int nFlashNum, int nAddress, unsigned char* ReadPtr)			;
	BOOL FLASH_SubSector_Write		(int nFlashNum, int nAddress, unsigned char* SubSectorInitPtr)	;

	int			m_nAllocPCIXpCore	;
	BOOL		m_bIFCardEndian		;
	CPciExpWrapper();
	virtual ~CPciExpWrapper();
};

#endif // !defined(AFX_PCIEXPWRAPPER_H__114BF46B_9176_44E0_983E_BDC6F7BCD4C6__INCLUDED_)

