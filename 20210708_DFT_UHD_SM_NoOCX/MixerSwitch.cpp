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
//                             CMixerSwitch
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
//=============================================================================
//=============================================================================
//=============================================================================

#include "stdafx.h"
#include "MixerSwitch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMixerSwitch::CMixerSwitch() : CMixerBase()
{
	m_ItemIndex = 0;
	m_Value = NULL;
}

CMixerSwitch::~CMixerSwitch()
{
	CloseMixer();
}


BEGIN_MESSAGE_MAP(CMixerSwitch, CButton)
	//{{AFX_MSG_MAP(CMixerSwitch)
	ON_MESSAGE( MM_MIXM_CONTROL_CHANGE, OnMixerControlChanged )
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
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
// Arg    : DWORD ControlType : type of desired switch or selector.
//                              These are valid arguments:
//
//  Selectors   MIXERCONTROL_CONTROLTYPE_SINGLESELECT 
//              MIXERCONTROL_CONTROLTYPE_MUX
//              MIXERCONTROL_CONTROLTYPE_MULTIPLESELECT
//              MIXERCONTROL_CONTROLTYPE_MIXER
//
//
//  Switches    MIXERCONTROL_CONTROLTYPE_BOOLEAN 
//              MIXERCONTROL_CONTROLTYPE_ONOFF
//              MIXERCONTROL_CONTROLTYPE_MUTE
//              MIXERCONTROL_CONTROLTYPE_MONO
//              MIXERCONTROL_CONTROLTYPE_LOUDNESS
//              MIXERCONTROL_CONTROLTYPE_STEREOENH
//
// Arg    : int index : if the mixer control is a Selector (List), specifies the
//                      0-based index of the selector's item to which this object
//                      (and the windows control) will be associated.
//
//-----------------------------------------------------------------------------
int CMixerSwitch::Init( DWORD DstType, DWORD SrcType, DWORD srcIndex, DWORD ControlType, int index )
{
	if ( m_hWnd == NULL )
	{
		PrintMessage( "CMixerFader::Init", "No Window attached. Did you create or subclass one?" );
		return 0;
	}
	EnableWindow( FALSE );

	// check the type requested

	if( (ControlType & MIXERCONTROL_CT_CLASS_MASK) != MIXERCONTROL_CT_CLASS_SWITCH )
	{
		if( (ControlType & MIXERCONTROL_CT_CLASS_MASK) != MIXERCONTROL_CT_CLASS_LIST )
		{
			PrintMessage( "CMixerSwitch::Init", 
		                  "Wrong control type: %s", 
					      GetControlTypeString( ControlType ) );
			return 0;
		}
		else
			m_ItemIndex = index;
	}
	
	if ( CMixerBase::Init( DstType, SrcType, srcIndex, ControlType, GetSafeHwnd() ) == 0 )
		return 0;

	// check if the 'index' parameter is within the number of items in a multiple
	// items mixer control

	int nItems = 1;
	if( m_Control.fdwControl & MIXERCONTROL_CONTROLF_MULTIPLE )
	{
		nItems = m_Control.cMultipleItems;
		if( index < 0 || index >= nItems )
		{
			PrintMessage( "CMixerNumber::Init",
			              "Invalid control index passed: %d", index );
			CloseMixer();
			return 0;
		}
	}

	m_Value = (MIXERCONTROLDETAILS_BOOLEAN*)GetValuePtr();

	if( GetMixerControlValue() )
	{
		EnableWindow( TRUE );
		SetCheck( m_Value[m_ItemIndex].fValue );
	}
	
	return 1;
}


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnClicked
//          
// Descr. : button callback reflected message. Switches the mixer control off/on
//          
// Return : void
//-----------------------------------------------------------------------------
void CMixerSwitch::OnClicked() 
{
	if( m_HMixer )
	{
		m_Value[m_ItemIndex].fValue = !m_Value[m_ItemIndex].fValue;
		SetCheck( m_Value[m_ItemIndex].fValue );

		SetMixerControlValue();
	}
	else
	{
		PrintMessage( "CMixerSwitch::OnClicked", "Error: Did you call Init() first?" );
	}
}

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnMixerControlChanged
//          
// Descr. : Message handler called whenever the MCI device sends the MM_MIXM_CONTROL_CHANGE
//          message. The MCI mixer device will send this message for any changing
//          setting, so we need to check that the MCI control that sent the message
//          corresponds to the current selector control.
//          
// Return : LONG
// Arg    : UINT         : 
// Arg    : LONG message : 
//-----------------------------------------------------------------------------
LONG CMixerSwitch::OnMixerControlChanged( UINT, LONG message )
{
	UINT id = ( UINT )message;

	if( id == m_Control.dwControlID ) // query the new value
	{
		if( GetMixerControlValue() )
		{
			SetCheck( m_Value[m_ItemIndex].fValue );
		}
		else
		{
			TRACE( "CMixerSwitch::OnMixerControlChanged mixerGetControlDetails failed.\n" );
		}
	}

	return 0L;
}

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnDestroy
//          
// Descr. : 
//          
// Return : void
//-----------------------------------------------------------------------------
void CMixerSwitch::OnDestroy() 
{
	CloseMixer();
	CButton ::OnDestroy();
}
