#if !defined(AFX_ANALOGGRABBER_H__A03BAAB3_0D70_42E2_86AD_8B9CB0973583__INCLUDED_)
#define AFX_ANALOGGRABBER_H__A03BAAB3_0D70_42E2_86AD_8B9CB0973583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PeriGrabber.h : header file
//

#include <mil.h> 

//+del 090420(Modification No1)
// Video Source
/*
#define V_SRC_NTSC	0
#define V_SRC_PAL	1
#define V_SRC_SECAM 2
*/

/////////////////////////////////////////////////////////////////////////////
// CAnalogGrabber window


class CAnalogGrabber : public CWnd
{
// Construction
public:
	CAnalogGrabber();

// Attributes
public:

	int m_nWidth;
	int m_nHeight;
	int m_nImageSize;
	int m_nBitCount;
	int	m_nErrorCode;
	int m_nVideoSourceType;

	BOOL m_bGrabberInit;

	// MIL Grabber
	MIL_ID m_MilApplication;  /* Application identifier.  */
	MIL_ID m_MilSystem;       /* System identifier.       */
	MIL_ID m_MilDigitizer;    /* Digitizer identifier.    */ 

	MIL_ID m_MilImage;        /* Image buffer identifier. */
	MIL_ID m_MilDisplay;      /* Display identifier.      */

//	MIL_ID m_MilImage2;        /* Image buffer identifier. */
//	MIL_ID m_MilDisplay2;      /* Display identifier.      */


	BYTE* m_pImageData;
  
	BOOL   m_isOverlayEnabled;              //Overlay show state

// Operations
public:

//	BOOL	OnPciCardOpen(CProgressCtrl* Progress,int StartPos, HWND hwnd); 
	BOOL	OnPciCardOpen(); 
	void	OnPciCardClose(); 
	void	SetResolution(int nWidth,int nHeight, int nBitCount);
	void	SetVideoSourceType(int nVideoSourceType);
	int		OnGrabStart(int nType,CString szRefFileName = _T(""),int nUseMaskImage = 0, BOOL bMultiGrab = TRUE);
	void	OverlayDraw(long StartX, long StartY, long RoiWidth, long RoiHeight);
	void	OverlayClear();
	void    Grab_Average();

	long MFTYPE DisplayError(MIL_INT HookType, MIL_ID EventId, void MPTYPE *UserDataPtr);
	void HookedOnErrors(bool IsHooked){m_isCurrentlyHookedOnErrors = IsHooked;}
private:
	bool m_isCurrentlyHookedOnErrors;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnalogGrabber)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAnalogGrabber();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAnalogGrabber)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//+add kwmoon 070213[#1]
extern CAnalogGrabber		AnalogControl;
//-

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANALOGGRABBER_H__A03BAAB3_0D70_42E2_86AD_8B9CB0973583__INCLUDED_)
