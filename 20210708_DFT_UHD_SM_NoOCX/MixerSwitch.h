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
#ifndef _LANGIS_PITRE_MIXER_SWITCH_H_
#define _LANGIS_PITRE_MIXER_SWITCH_H_

#include "MixerBase.h"

//=============================================================================
//                             CMixerSwitch Class
//
// Since this class is derived from CButton, it allows the user to associate
// a check box or radio button with a mixer switch or an item in a mixer selector
// control. All messages are handled internally through message reflection.
//
// Mixer selectors are mostly used to select one or more input audio line
// associated with a given output line. For example, my SoundBlaster enables
// the selection of the CD audio, microphone, and line-in input lines associated
// with the speakers' output line.
// Mixer selectors are multiple mixer controls. Each item of the control is a
// boolean value. For each item of the mixerselector, you shoud have a
// CMixerSwitch object in your program. You pass to Init() the index of the
// mixer selector you want to implement with your CMixerSwitch object.
// If you just want to implement a mixer switch, the index is ignored.
//
// How to use:
//
// 1) create a checkbox or radio button resource.
// 2) declare a variable of type CMixerSwitch in your CDialog-derived class
//    declaration
// 3) In your OnInitDialog, subclass the CMixerSwitch object to associate it
//    with the resource (or use DDE with a variable of type CMixerSwitch)
// 4) In your OnInitDialog, call the Init() member funcion.
//
// These controls work on both channels simutaneously.
//
//===========================================================================

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
class CMixerSwitch : public CButton, public CMixerBase
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
{
// Construction
public:
	CMixerSwitch();
	virtual ~CMixerSwitch();

	int Init( DWORD DstType, DWORD SrcType, DWORD srcIndex, DWORD ControlType, int index );

	//{{AFX_VIRTUAL(CMixerSwitch)
	//}}AFX_VIRTUAL

protected:

	int m_ItemIndex;

	MIXERCONTROLDETAILS_BOOLEAN  *m_Value;

	//{{AFX_MSG(CMixerSwitch)
	afx_msg LONG OnMixerControlChanged( UINT, LONG );
	afx_msg void OnDestroy();
	afx_msg void OnClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//===========================================================================
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // _LANGIS_PITRE_MIXER_SWITCH_H_

//===========================================================================
