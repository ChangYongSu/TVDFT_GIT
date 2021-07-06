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
#ifndef _LANGIS_PITRE_MIXER_PEAKMETER_H_
#define _LANGIS_PITRE_MIXER_PEAKMETER_H_

#include "MixerBase.h"

//=============================================================================
//                             CMixerPeakMeter Class
//
// Since this class is derived from CStatic, it allows the user to associate
// a static resource with a mixer peak meter control. All messages are handled
// internally through message reflection.
//
// This class uses a timer to check for line activity.
//
// How to use:
//
// 1) create a static resource (specify a resource ID other than IDC_STATIC)
//    You can give it the Sunken style if you wish.
// 2) declare a variable of type CMixerPeakMeter in your CDialog-derived
//    class declaration
// 3) In your OnInitDialog, subclass the CMixerPeakMeter object to associate it
//    with the resource (or use DDE with a variable of type CMixerPeakMeter)
// 4) In your OnInitDialog, call the Init() member funcion.
//
// The peak meter works like the CProgressCtrl, except that it is vertical.
// It displays the audio level of the line it is associated with.
//
//=============================================================================
class CMixerPeakMeter : public CStatic, public CMixerBase
//===========================================================================
{
public:
	CMixerPeakMeter( COLORREF background = RGB( 192, 192, 192 ) );
	virtual ~CMixerPeakMeter();

	int  Init( DWORD DstType, DWORD SrcType );

	//{{AFX_VIRTUAL(CMixerPeakMeter)
	//}}AFX_VIRTUAL

protected:

    COLORREF m_Bkgnd;
	UINT     m_timer;
	MIXERCONTROLDETAILS_SIGNED *m_Value;

	virtual void DrawMeter( CDC *pDC = NULL );

	//{{AFX_MSG(CMixerPeakMeter)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//===========================================================================
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // _LANGIS_PITRE_MIXER_PEAKMETER_H_

//===========================================================================
