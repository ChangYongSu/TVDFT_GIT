// PciDio.cpp: implementation of the CPciDio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "datsys.h"
#include "PciDio.h"
#include "global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern int g_nRunningProcessNo;

CPciDio gPciDioCtrl;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPciDio::CPciDio()
{
	m_bPCIDIO = FALSE;
	m_nDioOutData = 0;
	m_nCardID = -1 ;

}

CPciDio::~CPciDio()
{

}

BOOL CPciDio::Open_DioCard()
{
	U16 nCardType;
	
	nCardType = PCI_7250;
	
	m_nCardID = Register_Card(nCardType, 0);
	
	if(m_nCardID < 0) 
	{
		m_bPCIDIO = FALSE;
		return FALSE;
    }
	else
	{
		m_bPCIDIO = TRUE;
		return TRUE;
	}
}
void CPciDio::Close_DioCard()
{
	if (m_nCardID >= 0){
		Release_Card(0);
	}
}
int CPciDio::Jig_Ready_Check()
{
	U32 nInTemp;
	U32 nDioInData;

	if(!m_bPCIDIO) return INPUT_ERROR;

	DI_ReadPort(m_nCardID, 0, &nDioInData);
	
	if(g_nRunningProcessNo == 1){
		nInTemp = nDioInData & TEST_START1;
	}
	else{
		nInTemp = nDioInData & TEST_START2;
	}

	if((nInTemp == JIG_DN1) || (nInTemp == JIG_DN2))
	{
		Sleep(100);
		
		DI_ReadPort(m_nCardID, 0, &nDioInData);
		
		if(g_nRunningProcessNo == 1){
			nInTemp = nDioInData & TEST_START1;
		}
		else{
			nInTemp = nDioInData & TEST_START2;
		}
		if((nInTemp == JIG_DN1) || (nInTemp == JIG_DN2))
		{
			return IN_JIGDN;
		}
		else
			return IN_JIGUP;
	}
	else
		return IN_JIGUP;
}
void CPciDio::Jig_Up()
{
	U32	nDioOutData = 0x00;

	if(!m_bPCIDIO) return;
	
	DO_ReadPort(m_nCardID, 0, &nDioOutData);

	if(g_nRunningProcessNo == 1){
		nDioOutData = nDioOutData | JIG_UP1;
		DO_WritePort(m_nCardID, 0, nDioOutData);
		Sleep(500);
		nDioOutData = nDioOutData & JIG_UP1_R;
		DO_WritePort(m_nCardID, 0, nDioOutData);
	}
	else{
		nDioOutData = nDioOutData | JIG_UP2;
		DO_WritePort(m_nCardID, 0, nDioOutData);
		Sleep(500);
		nDioOutData = nDioOutData & JIG_UP2_R;
		DO_WritePort(m_nCardID, 0, nDioOutData);
	}
}
void CPciDio::DioOut_Reset()
{
	U32	nDioOutData = 0x00;
	
	if(!m_bPCIDIO) return;

	DO_ReadPort(m_nCardID, 0, &nDioOutData);
	
	if(g_nRunningProcessNo == 1){
		nDioOutData = nDioOutData & 0xF8;
	}
	else{
		nDioOutData = nDioOutData & 0x8F;
	}
	
	DO_WritePort(m_nCardID, 0, nDioOutData);
}

void CPciDio::PLC_ResultSignalOut(BOOL nResult)
{
	U32	nDioOutData = 0x00;
	
	if(!m_bPCIDIO) return;

	DO_ReadPort(m_nCardID, 0, &nDioOutData);
	
	if(nResult){
		if(g_nRunningProcessNo == 1){
			nDioOutData = nDioOutData | 0x03;
			DO_WritePort(m_nCardID, 0, nDioOutData);
			Sleep(500);
			nDioOutData = nDioOutData & 0xFC;
			DO_WritePort(m_nCardID, 0, nDioOutData);
		}
		else{
			nDioOutData = nDioOutData | 0x30;
			DO_WritePort(m_nCardID, 0, nDioOutData);
			Sleep(500);
			nDioOutData = nDioOutData & 0xCF;
			DO_WritePort(m_nCardID, 0, nDioOutData);
		}
	}
	else{
		if(g_nRunningProcessNo == 1){
			nDioOutData = nDioOutData | 0x0C;
			DO_WritePort(m_nCardID, 0, nDioOutData);
			Sleep(500);
			nDioOutData = nDioOutData & 0xF3;
			DO_WritePort(m_nCardID, 0, nDioOutData);
		}
		else{
			nDioOutData = nDioOutData | 0xC0;
			DO_WritePort(m_nCardID, 0, nDioOutData);
			Sleep(500);
			nDioOutData = nDioOutData & 0x3F;
			DO_WritePort(m_nCardID, 0, nDioOutData);
		}
	}
}

BOOL CPciDio::Set_Relay(int nNo, BOOL bOnOff)
{
	BYTE nValue;
//	BOOL bRet;
	U32	nDIO_Out = 0x00;
	
	if(!m_bPCIDIO) return FALSE;

	DO_ReadPort(m_nCardID, 0, &nDIO_Out);
	
	if(g_nRunningProcessNo == 1){
		if(nNo == 1){
			if(bOnOff){
				nValue = nDIO_Out | 0x01;
			}
			else{
				nValue = nDIO_Out & 0xfe;
			}
		}
		else if(nNo == 2){
			if(bOnOff){
				nValue = nDIO_Out | 0x02;
			}
			else{
				nValue = nDIO_Out & 0xfd;
			}
		}
		else if(nNo == 3){
			if(bOnOff){
				nValue = nDIO_Out | 0x04;
			}
			else{
				nValue = nDIO_Out & 0xfb;
			}
		}
	//	else if(nNo == 4){
	//		if(bOnOff){
	//			nValue = nDIO_Out | 0x08;
	//		}
	//		else{
	//			nValue = nDIO_Out & 0xf7;
	//		}
	//	}
		else{
			return FALSE;
		}
	}
	else{
		if(nNo == 1){
			if(bOnOff){
				nValue = nDIO_Out | 0x10;
			}
			else{
				nValue = nDIO_Out & 0xef;
			}
		}
		else if(nNo == 2){
			if(bOnOff){
				nValue = nDIO_Out | 0x20;
			}
			else{
				nValue = nDIO_Out & 0xdf;
			}
		}
		else if(nNo == 3){
			if(bOnOff){
				nValue = nDIO_Out | 0x40;
			}
			else{
				nValue = nDIO_Out & 0xbf;
			}
		}
	//	else if(nNo == 4){
	//		if(bOnOff){
	//			nValue = nDIO_Out | 0x80;
	//		}
	//		else{
	//			nValue = nDIO_Out & 0x7f;
	//		}
	//	}
		else{
			return FALSE;
		}
	}
	DO_WritePort(m_nCardID, 0, nValue);
	
	return TRUE;
}

int CPciDio::DioOut_Read()
{
	U32	nDIO_Out = 0x00;
	int nRev = 0;
	
	if(!m_bPCIDIO) return 0;
	
	DO_ReadPort(m_nCardID, 0, &nDIO_Out);

	nRev = nDIO_Out & 0xFF;
  
	return nRev;
}
