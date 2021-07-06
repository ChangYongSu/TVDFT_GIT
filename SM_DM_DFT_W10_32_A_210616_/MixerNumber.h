//=============================================================================
// Copyright Langis Pitre 1998
// You may do whatever you want with this code, as long as you include this
// copyright notice in your implementation files.
//
// If you wish to add new classes to this collection, feel free to do so.
// But please send me your code so that I can update the collection.
//
// Comments and bug reports: lpitre@sympatico.ca
//
//=============================================================================
//=============================================================================
//                             CMixerNumber
//
// Since this class is derived from CSpinButton, it allows the user to associate
// a spin button resource with a mixer number control (a gain control for example). 
// All messages are handled internally through message reflection.
// 
// How to use:
//
// 1) create a spin button resource (to display the value, you can attach it to
//    a CStatic resource that precedes it in the Tab Order).
// 2) declare a variable of type CMixerNumber in your CDialog-derived class
//    declaration
// 3) In your OnInitDialog, subclass the CMixerNumber object to associate it
//    with the resource (or use DDE with a variable of type CMixerNumber)
// 4) In your OnInitDialog, call the Init() member funcion.
//
//=============================================================================
//=============================================================================
//=============================================================================
#ifndef _LANGIS_PITRE_MIXER_NUMBER_H_
#define _LANGIS_PITRE_MIXER_NUMBER_H_

#include "MixerBase.h"

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
class CMixerNumber : public CSpinButtonCtrl, public CMixerBase
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
{
public:
	CMixerNumber();
	virtual ~CMixerNumber();
	
	int Init( DWORD DstType, DWORD SrcType, DWORD srcIndex, DWORD ControlType );

	//{{AFX_VIRTUAL(CMixerNumber)
	//}}AFX_VIRTUAL

protected:
    
	MIXERCONTROLDETAILS_UNSIGNED *m_Value;

	//{{AFX_MSG(CMixerNumber)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LONG OnMixerControlChanged( UINT, LONG );

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(_LANGIS_PITRE_MIXER_NUMBER_H_)
