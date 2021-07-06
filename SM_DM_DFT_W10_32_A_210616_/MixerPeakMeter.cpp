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
//                             CMixerPeakMeter
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
//=============================================================================
//=============================================================================
#include "stdafx.h"

#include "MixerPeakMeter.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMixerPeakMeter


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : CMixerPeakMeter
//          
// Descr. : 
//          
// Arg    : COLORREF background : 
//-----------------------------------------------------------------------------
CMixerPeakMeter::CMixerPeakMeter(COLORREF background ) : CMixerBase()
{
	m_timer = 0;
    m_Bkgnd = background;
	m_Value = NULL;
}


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : ~CMixerPeakMeter
//          
// Descr. : 
//          
//-----------------------------------------------------------------------------
CMixerPeakMeter::~CMixerPeakMeter()
{
	if( m_timer )
		KillTimer( m_timer );
}


BEGIN_MESSAGE_MAP( CMixerPeakMeter, CStatic )
	//{{AFX_MSG_MAP(CMixerPeakMeter)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : Init
//          
// Descr. : Initializes the mixer control.
//          Init queries the mixer device for the specified mixer control type,
//          which is always MIXERCONTROL_CONTROLTYPE_PEAKMETER in this case.
//          If such a control is found, its current value is queried, and used
//          to initialize the windows control.
//        - If the call to CMixerBase::Init() is succesfull, m_Value is set to
//          the address of the memory allocated by CMixerBase.
//          
// Return : int 0 if failed, 1 otherwise
//
// Arg    : DWORD DstType    : see definition of CMixerBase::Init() for a description
// Arg    : DWORD SrcType    : see definition of CMixerBase::Init() for a description
//-----------------------------------------------------------------------------
int CMixerPeakMeter::Init( DWORD DstType, DWORD SrcType )
{		
	if ( m_hWnd == NULL )
	{
		PrintMessage( "CMixerFader::Init", "No Window attached. Did you create or subclass one?" );
		return 0;
	}
	EnableWindow( FALSE );

	if( CMixerBase::Init( DstType, SrcType, 0, MIXERCONTROL_CONTROLTYPE_PEAKMETER, GetSafeHwnd() ) == 0 )
		return 0;

	m_Value = (MIXERCONTROLDETAILS_SIGNED*)GetValuePtr();

	if( GetMixerControlValue() )
	{
		EnableWindow( TRUE );
		if( m_timer == 0 )
		{
			m_timer = SetTimer( 345, 100, NULL );
		}
	}
	
	return 1;
}


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnPaint
//          
// Descr. : 
//          
// Return : void
//-----------------------------------------------------------------------------
void CMixerPeakMeter::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawMeter( &dc );
}

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : DrawMeter
//          
// Descr. : Draws the windows control (static control). Draws a series of
//          rectangles, up to the current value. Since the value of the peak meter
//          mixer control ranges from -32,768 to 32,767, only the positive values
//          are used, and these values are mapped to fall between 0 and NSTEPS.
//          Thus, a maximum of NSTEPS-1 rectangles will be drawn.
//          
// Return : void
// Arg    : CDC *pDC : 
//-----------------------------------------------------------------------------
void CMixerPeakMeter::DrawMeter( CDC *pDC )
{
	const int NSTEPS = 10;

	if( m_Value == NULL || GetSafeHwnd() == NULL )
		return;

	CRect r;
	CDC *pdc;

	if( pDC != NULL ) pdc = pDC;
	else              pdc = new CClientDC(this);
	
	GetClientRect( &r );

	int rHeight = r.Height() / NSTEPS;
	int rWidth  = r.Width();
	
	// the peak meter range is -32,768 to 32,767
	if( m_Value[0].lValue >= 0 )
	{
		CMemDC mdc( pdc, r );
		
		mdc.FillSolidRect( r, m_Bkgnd );
		
		int value = int( m_Value[0].lValue * NSTEPS / 32768 );

		//TRACE("DrawMeter - value: %lu\n", value);

		// draw a series of rectangles from green to red
		for( int i = 0; i < value; i++ )
		{
			mdc.FillSolidRect( 0, (NSTEPS - i) * rHeight, r.Width(), rHeight-1,
							   RGB( i*255/NSTEPS, 255 - i*255/NSTEPS, 0 ) );
		}	
	}

	if( pDC == NULL )
		delete pdc;
}

//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnTimer
//          
// Descr. : queries the mixer device for the current control value
//          
// Return : void
// Arg    : UINT nIDEvent : 
//-----------------------------------------------------------------------------
void CMixerPeakMeter::OnTimer(UINT nIDEvent) 
{	
	if( m_Value == NULL )
		return;

	if( m_timer == nIDEvent)
	{
		if( GetMixerControlValue() )
			DrawMeter();
	}

	CStatic::OnTimer(nIDEvent);
}


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
// Name   : OnDestroy
//          
// Descr. : 
//          
// Return : void
//-----------------------------------------------------------------------------
void CMixerPeakMeter::OnDestroy() 
{
	CloseMixer();
	
	if( m_timer )
		KillTimer( m_timer );
	m_timer = 0;
		
	CStatic ::OnDestroy();	
}

