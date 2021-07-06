// Gmes.h: interface for the CGmes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMES_H__3E36F47B_03C0_418A_AEAF_6B8AEA78F618__INCLUDED_)
#define AFX_GMES_H__3E36F47B_03C0_418A_AEAF_6B8AEA78F618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGmes  
{
public:
	CGmes();
	virtual ~CGmes();

	CSocket m_pClient;
	BOOL	m_bGmesCom;
	CString	m_sReceive;


	CString	m_sSetID;
	CString	m_sTransactionID;
	CString	m_sWorkOrderID;
	CString	m_sOrganizationID;
	CString	m_sLineID;
	CString	m_sEquipmentID;
	CString	m_sOperationID;
	CString	m_sModelID;
	CString	m_sModelName;
	CString	m_sModelSuffixName;
	int		m_nOrganizationID;
	int		m_nDftSeq;
	CString	m_sErrorMsg;

	BOOL    SoketCom_Creat(CString sIpAdd, int nPort);
	void	SoketCom_Close();
	BOOL	SendCommString(CString str);
	BOOL	ReceiveCommString(int nWaitLimit, CString& sReadString);

	void	DisplyComStatus(CString sItem, BOOL bFlag);
	CString MakeElem_S1F1();
	CString MakeElem_S2F32(CString sEquipmentID, int nFlag);
	CString MakeElem_S2F4(CString sEquipmentID);
	CString MakeElem_S6F11(CString sEquipmentID, CString sPcbID);
	CString MakeElem_S6F11_2(CString sEquipmentID, CString sPcbID, CString sMac_Add);
	BOOL	Paser_ReceiveData(CString sReceiveData);
	BOOL	Paser_S2F3(CString sReceiveData);
	BOOL	Paser_S1F2(CString sReceiveData);
	BOOL	Paser_S2F31(CString sReceiveData);
	BOOL	Paser_S6F12(CString sReceiveData);
	BOOL	Paser_S6F5(CString sReceiveData, CString& sSetId);
	CString MakeElem_S6F6(CString sEquipmentID, CString sPcbID, int nFlag);
	CString MakeElem_S6F1(CString sEquipmentID, CString sPcbID, BOOL bResult, CString sTestTime, BOOL bFailOnly);
	BOOL	Paser_S6F2(CString sReceiveData);
	
	BOOL	ChangeSystemTime(CString sTimeInfo);
};

extern CGmes gGmesCtrl;

#endif // !defined(AFX_GMES_H__3E36F47B_03C0_418A_AEAF_6B8AEA78F618__INCLUDED_)
