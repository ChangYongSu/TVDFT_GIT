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
#include "stdafx.h"
#include "MixerNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : CMixerNumber
//          
// Descr. : 
//          
//-----------------------------------------------------------------------------
CMixerNumber::CMixerNumber() : CMixerBase()
{
	m_Value = NULL;
}


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : ~CMixerNumber
//          
// Descr. : 
//          
//-----------------------------------------------------------------------------
CMixerNumber::~CMixerNumber()
{
}


BEGIN_MESSAGE_MAP(CMixerNumber, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CMixerNumber)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE( MM_MIXM_CONTROL_CHANGE, OnMixerControlChanged )
END_MESSAGE_MAP()



//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : Init
//          
// Descr. : Initializes the mixer control. Most of the work is done in the
//          base class' implementation of Init.
//          Init queries the mixer device for the specified mixer control type.
//          If such a control is found, its current value is queried, and used
//          to initialize the windows control.
//        - If the call to CMixerBase::Init() is succesfull, m_Value is set to
//          the address of the memory allocated by CMixerBase.
//          
// Return : int 0 if failed, 1 otherwise
//
// Arg    : DWORD DstType    : see definition of CMixerBase::Init() for a description
// Arg    : DWORD SrcType    : see definition of CMixerBase::Init() for a description
// Arg    : DWORD ControlType: must be MIXERCONTROL_CONTROLTYPE_UNSIGNED
//-----------------------------------------------------------------------------
int CMixerNumber::Init( DWORD DstType, DWORD SrcType, DWORD srcIndex, DWORD ControlType )
{	
	if ( m_hWnd == NULL )
	{
		PrintMessage( "CMixerFader::Init", "No Window attached. Did you create or subclass one?" );
		return 0;
	}
	EnableWindow( FALSE );

	//if( (ControlType & MIXERCONTROL_CT_CLASS_MASK) != MIXERCONTROL_CT_CLASS_NUMBER )
	if( ControlType != MIXERCONTROL_CONTROLTYPE_UNSIGNED )
	{
		PrintMessage( "CMixerNumber::Init", 
		              "Wrong control type: %s", 
					  GetControlTypeString( ControlType ) );
		return 0;
	}

	if( CMixerBase::Init( DstType, SrcType, srcIndex, ControlType, m_hWnd ) == 0 )
		return 0;
	
	m_Value = (MIXERCONTROLDETAILS_UNSIGNED *)GetValuePtr();

	if( GetMixerControlValue() )
	{
		SetRange( (int)m_Control.Bounds.dwMinimum, (int)m_Control.Bounds.dwMaximum );
		SetPos( (int)m_Value[0].dwValue );

		EnableWindow( TRUE );
	}

	return 1;
}

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnMixerControlChanged
//          
// Descr. : Message handler called whenever the MCI device sends the
//          MM_MIXM_CONTROL_CHANGE message.
//          
// Return : LONG
// Arg    : UINT         : 
// Arg    : LONG message : 
//-----------------------------------------------------------------------------
LONG CMixerNumber::OnMixerControlChanged( UINT hMix, LONG message )
{
	UINT id = ( UINT )message;

	if( id == m_Control.dwControlID && m_HMixer == (HMIXER)hMix )
	{
		if( GetMixerControlValue() )
		{
			SetPos( (int)m_Value[0].dwValue );
		}
		else
		{
			TRACE( "CMixerPeakMeter::OnMixerControlChanged mixerGetControlDetails error\n" );
		}
	}

	return 0L;
}

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnDeltapos
//          
// Descr. : Called when the spin button value has changed. Checks if the new
//          position is within the range of the mixer control, then sets the new
//          value.
//          
// Return : void
// Arg    : NMHDR* pNMHDR    : 
// Arg    : LRESULT* pResult : 
//-----------------------------------------------------------------------------
void CMixerNumber::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	DWORD newVal = pNMUpDown->iPos + pNMUpDown->iDelta;

	if( newVal > m_Control.Bounds.dwMaximum || newVal < m_Control.Bounds.dwMinimum )
		return;
	
	if( m_Value == NULL )
	{
		PrintMessage( "CMixerNumber::OnDeltapos", "Error: Did you call Init() first?"  );
		return;
	}
	
	if( newVal == m_Value[0].dwValue )
		return;
	
	m_Value[0].dwValue = newVal;
	if( m_nChannels > 1 )
	{
		m_Value[1].dwValue = newVal;
	}

	SetMixerControlValue();

	*pResult = 0;
}


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnDestroy
//          
// Descr. : 
//          
// Return : void
//-----------------------------------------------------------------------------
void CMixerNumber::OnDestroy() 
{
	CloseMixer();
	CSpinButtonCtrl::OnDestroy();
}
