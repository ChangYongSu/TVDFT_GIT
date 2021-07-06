// PciDio.h: interface for the CPciDio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCIDIO_H__A0BF2372_80C6_439D_B678_FC9A281DC888__INCLUDED_)
#define AFX_PCIDIO_H__A0BF2372_80C6_439D_B678_FC9A281DC888__INCLUDED_

#include "Dask64.h"


/////////////////////////////////////////////////////////////////////////////

#define TEST_START1	0x01
#define TEST_START2	0x10

#define JIG_DN1		0x01
#define JIG_DN2		0x10

#define JIG_UP1		0x08
#define JIG_UP2		0x80

#define JIG_UP1_R	0xF7
#define JIG_UP2_R	0x7F


typedef enum 
{
	INPUT_ERROR =0, IN_JIGUP, IN_JIGDN, IN_TEST_START, IN_TEST_STOP, IN_TEST_ENTER,
		IN_EMG
}INPUT_Type;


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPciDio  
{
public:
	CPciDio();
	virtual ~CPciDio();

	BOOL	m_bPCIDIO;
	U16 	m_nDioOutData;
	I16		m_nCardID ;

	BOOL Open_DioCard();
	int  Jig_Ready_Check();
	void DioOut_Reset();
	void Jig_Up();
	BOOL Set_Relay(int nNo, BOOL bOnOff);
	
	void PLC_ResultSignalOut(BOOL nResult);
	void Close_DioCard();

	int DioOut_Read();
};

extern CPciDio gPciDioCtrl;

#endif // !defined(AFX_PCIDIO_H__A0BF2372_80C6_439D_B678_FC9A281DC888__INCLUDED_)
