// AnalogGrabber.cpp : implementation file
//

#include "stdafx.h"
#include "AnalogGrabber.h"
#include "Defines.h"
#include <stdio.h>
#include <wtypes.h>

//+del kwmoon 080318
//#include "board.h"
//#include "pcibrd.h"
#include "Global.h"

#include <mil.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Define
#define NO_MODE_SET_RETRY	2

CAnalogGrabber AnalogControl;

extern BOOL  RefImage_DirectLoad(CString szRefFileName,int nUseMaskImage);

//+add kwmoon 071031
extern int g_nRunningProcessNo;

//+add 090131(수정항목6)
extern CDATsysView*  g_pView;

MIL_INT MFTYPE DisplayErrorExt(MIL_INT HookType, MIL_ID EventId, void MPTYPE *UserDataPtr);
/////////////////////////////////////////////////////////////////////////////
// CAnalogGrabber 

// Matrox Board Control Using MIL Library //

CAnalogGrabber::CAnalogGrabber()
{
	m_nWidth		 = 0;
	m_nHeight		 = 0;
	m_nImageSize	 = 0;
	m_nErrorCode	 = 0;
	m_nVideoSourceType = 0;

	m_bGrabberInit = FALSE;

	m_MilApplication	= M_NULL;    /* Application identifier.  */
	m_MilSystem			= M_NULL;    /* System identifier.       */
	m_MilDigitizer		= M_NULL;    /* Digitizer identifier.    */ 
	m_MilImage			= M_NULL;    /* Image buffer identifier. */
	m_MilDisplay		= M_NULL;    /* Display identifier.      */

	m_pImageData = NULL;
	m_isOverlayEnabled = FALSE;
	m_isCurrentlyHookedOnErrors = false;
}

CAnalogGrabber::~CAnalogGrabber()
{
	if(m_MilDisplay	  != M_NULL)    MappFree(m_MilDisplay);
	if(m_MilImage	  != M_NULL)	MbufFree(m_MilImage);
	if(m_MilDigitizer != M_NULL)	MdigFree(m_MilDigitizer);
	if(m_MilSystem	  != M_NULL)	MsysFree(m_MilSystem);
//	if(m_MilApplication != M_NULL)	MappFree(m_MilApplication);

	if(m_MilApplication)  
	{
		// Enable the typical MIL error message display[CALL TO MIL]
		MappControl(M_ERROR,M_PRINT_ENABLE);
		
		// Unhook MIL error on function DisplayError() [CALL TO MIL]
		if(m_isCurrentlyHookedOnErrors)
		{
			MappHookFunction(M_ERROR_CURRENT+M_UNHOOK,DisplayErrorExt,this);
			m_isCurrentlyHookedOnErrors = false;
		}
		
		// Free the application [CALL TO MIL]
		MappFree(m_MilApplication);
      }
}


BEGIN_MESSAGE_MAP(CAnalogGrabber, CWnd)
	//{{AFX_MSG_MAP(CAnalogGrabber)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnalogGrabber message handlers

BOOL CAnalogGrabber::OnPciCardOpen() 
{
	MIL_ID MReV;
	
	/* Allocate defaults. */
//	MappAllocDefault(M_SETUP, &m_MilApplication, &m_MilSystem,
//	                           M_NULL, &m_MilDigitizer, &m_MilImage);

	MIL_INT32  SizeX = 0, SizeY = 0;

	/* Allocate a MIL application */
	MappAlloc(M_DEFAULT, &m_MilApplication);
		
	// Hook MIL error on function DisplayError() [CALL TO MIL]
	MappHookFunction(M_ERROR_CURRENT,DisplayErrorExt,this);
	
	m_isCurrentlyHookedOnErrors = true;
	
	// Disable the typical MIL error message display [CALL TO MIL]
	MappControl(M_ERROR,M_PRINT_DISABLE);
	

	if(g_nRunningProcessNo == 1)
	{
		/* Allocate a hardware system */
		MReV = MsysAlloc(M_SYSTEM_CRONOSPLUS, M_DEV0, M_DEFAULT, &m_MilSystem);
	}	
	else{
		// Allocate a hardware system 
		MReV = MsysAlloc(M_SYSTEM_CRONOSPLUS, M_DEV1, M_DEFAULT, &m_MilSystem);
	}
	if(MReV == M_NULL){
        MappControl(M_ERROR_HOOKS, M_ENABLE);
		if(m_MilSystem		 != M_NULL){ MsysFree(m_MilSystem);			m_MilSystem		  = M_NULL;}
		if(m_MilApplication  != M_NULL){ MappFree(m_MilApplication);	m_MilApplication  = M_NULL;}

		m_bGrabberInit = FALSE;
		return FALSE;
	}

 	if(CurrentSet->nAnalogType == V_SRC_NTSC)
	{
		MdigAlloc(m_MilSystem, M_DEFAULT, "M_NTSC", M_DEFAULT, &m_MilDigitizer);
	}
	else{
		MdigAlloc(m_MilSystem, M_DEFAULT, "M_PAL", M_DEFAULT, &m_MilDigitizer);
	}
	
	/* Get the width, height of the image */
	MdigInquire(m_MilDigitizer, M_SOURCE_SIZE_X, &SizeX);
	MdigInquire(m_MilDigitizer, M_SOURCE_SIZE_Y, &SizeY);
	
	/* Allocate a 2D data buffer */
	MbufAllocColor(m_MilSystem, 3, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB, &m_MilImage);

	MbufClear(m_MilImage, 0);

	/* Allocate a display */
	MdispAlloc(m_MilSystem, M_DEFAULT, "M_DEFAULT", M_DEFAULT, &m_MilDisplay);
	
	/*Filled with the selected image buffer using an automatically calculated zoom factor. */
	MdispControl(m_MilDisplay, M_FILL_DISPLAY , M_ENABLE);

	// Disable Show Error Message Dialog
//	MappControl(M_ERROR, M_PRINT_DISABLE);

	m_nVideoSourceType = CurrentSet->nAnalogType; 
//	SetVideoSourceType(V_SRC_NTSC);
	SetResolution(SizeX, SizeY, 24);	
	m_bGrabberInit = TRUE;
    MappControl(M_ERROR_HOOKS, M_ENABLE);

	return TRUE;
}

void CAnalogGrabber::OnPciCardClose() 
{
	if(m_MilDigitizer){ MdigFree(m_MilDigitizer);	m_MilDigitizer	  = M_NULL;}
	if(m_MilDisplay){ MdispFree(m_MilDisplay);		m_MilDisplay	  = M_NULL;}
	if(m_MilImage){ MbufFree(m_MilImage);			m_MilImage		  = M_NULL;}
	if(m_MilSystem){ MsysFree(m_MilSystem);			m_MilSystem		  = M_NULL;}
	if(m_MilApplication){ MappFree(m_MilApplication);	m_MilApplication  = M_NULL;}

	m_bGrabberInit = FALSE;

	if(m_pImageData != NULL){ delete m_pImageData; m_pImageData = NULL; }
}

void CAnalogGrabber::SetResolution(int nWidth,int nHeight, int nBitCount)
{
	m_nWidth	 = nWidth;
	m_nHeight	 = nHeight;
	m_nBitCount	 = nBitCount;

	int nRowWidth = WIDTHBYTES(m_nBitCount*m_nWidth);

	m_nImageSize = nRowWidth * m_nHeight;
}

int CAnalogGrabber::OnGrabStart(int nType,CString szRefFileName,int nUseMaskImage, BOOL bMultiGrab) 
{
	int nNoGrabRetry = MAX_NO_GRAB_RETRY;
	MIL_INT lLastError = M_NULL_ERROR; //long lLastError = M_NULL_ERROR;
	

	if(m_bGrabberInit == FALSE) return GRAB_ABORT;
	//+moved kwmoon 080319
	if(szRefFileName != _T(""))
	{
		if(!RefImage_DirectLoad(szRefFileName,nUseMaskImage)) return GRAB_ABORT;	
	}


	lLastError = M_NULL_ERROR;

	if(m_pImageData != NULL){ delete m_pImageData; m_pImageData = NULL; }

	m_pImageData = new BYTE[m_nImageSize];
	memset(m_pImageData,NULL,m_nImageSize);

	if(g_pView->m_bGrabThreadRunning){

		MappGetError(M_CURRENT, &lLastError);

		if(lLastError)
		{
			if(nType == GRAB_WITH_NO_RETRY)
			{
				//+add kwmoon 080319
				memset(m_pImageData,0xff,m_nImageSize);
				return GRAB_FAIL;
			}
			while(TRUE)
			{
				MappGetError(M_CURRENT, &lLastError);
				if(lLastError)
				{
					if(nNoGrabRetry-- > 0)
					{ 
						Sleep(10); 
						continue;
					}
					else
					{
						memset(m_pImageData,0xff,m_nImageSize);
						return GRAB_FAIL;
					}
				}
				else{
					break;
				}
			}
		}
		MdigHalt(m_MilDigitizer);
	}
/*	else
	{
		MdigGrab(m_MilDigitizer, m_MilImage);
		MappGetError(M_CURRENT, &lLastError);
		if(lLastError)
		{
			if(nType == GRAB_WITH_NO_RETRY)
			{
				//+add kwmoon 080319
				memset(m_pImageData,0xff,m_nImageSize);
				return GRAB_FAIL;
			}
			while(TRUE)
			{
				MdigGrab(m_MilDigitizer, m_MilImage);
				MappGetError(M_CURRENT, &lLastError);
				if(lLastError)
				{
					if(nNoGrabRetry-- > 0)
					{ 
						Sleep(10); 
						continue;
					}
					else
					{
						memset(m_pImageData,0xff,m_nImageSize);
						return GRAB_FAIL;
					}
				}
				else{
					break;
				}
			}
		}
	}
*/	
	if(bMultiGrab){
		Grab_Average();
	}
	else{
		MdigGrab(m_MilDigitizer, m_MilImage);
		// Copy Grabbed Color Data
		MbufGetColor(m_MilImage,M_PACKED | M_BGR24,M_ALL_BANDS, m_pImageData);
	}

	if(g_pView->m_bGrabThreadRunning){
		MdigGrabContinuous(m_MilDigitizer, m_MilImage);
	}

   return GRAB_PASS;
}


void CAnalogGrabber::SetVideoSourceType(int nVideoSourceType)
{
	long SizeX = 0, 
		 SizeY = 0;

	if(m_nVideoSourceType == nVideoSourceType) return;
	if(m_bGrabberInit == FALSE) return;

	if(m_MilDisplay		!= M_NULL){ 
		MdispFree(m_MilDisplay);		
		Sleep(2000);
		m_MilDisplay	 = M_NULL;
	}
	if(m_MilImage		!= M_NULL){ 
		MbufFree(m_MilImage);		
		Sleep(1000);
		m_MilImage		 = M_NULL;
	}
	
	if(m_MilDigitizer	!= M_NULL){ 
		MdigFree(m_MilDigitizer);	
		Sleep(1000);
		m_MilDigitizer	 = M_NULL;
	}

	if(m_pImageData		!= NULL){ delete m_pImageData; m_pImageData = NULL; }

	OnPciCardClose();
	Sleep(1500);
	OnPciCardOpen();

	//
	//if(nVideoSourceType == V_SRC_NTSC){
	//	MdigAlloc(m_MilSystem, M_DEFAULT, "M_NTSC", M_DEFAULT, &m_MilDigitizer);
	//}
	//else{
	//	MdigAlloc(m_MilSystem, M_DEFAULT, "M_PAL", M_DEFAULT, &m_MilDigitizer);
	//}

	//MdigInquire(m_MilDigitizer, M_SOURCE_SIZE_X, &SizeX);
	//MdigInquire(m_MilDigitizer, M_SOURCE_SIZE_Y, &SizeY);
	//
	//MbufAllocColor(m_MilSystem, 3, SizeX, SizeY, 8L+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB, &m_MilImage);
	//MbufClear(m_MilImage, 0);

	//MdispAlloc(m_MilSystem, M_DEFAULT, "M_DEFAULT", M_DEFAULT, &m_MilDisplay);
	//
	//MdispControl(m_MilDisplay, M_FILL_DISPLAY , M_ENABLE);

	//SetResolution(SizeX, SizeY, 24);

	m_nVideoSourceType = nVideoSourceType;

}

//extern CAnalogGrabber AnalogControl;

/* --------------------------------------------------------------- */
/* Name:      OverlayDraw
 * Synopsis:  This function draws annotations in the display overlay.
 */
void CAnalogGrabber::OverlayDraw(long StartX, long StartY, long RoiWidth, long RoiHeight)
{
	MIL_ID   MilOverlayImage;

	/* Prepare overlay buffer. */
	/***************************/

	/* Enable display overlay annotations. */
	MdispControl(m_MilDisplay, M_OVERLAY, M_ENABLE);

	/* Inquire the overlay buffer associated with the display. */
	MdispInquire(m_MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

	/* Clear the overlay to transparent. */
	MdispControl(m_MilDisplay, M_OVERLAY_CLEAR, M_DEFAULT);

	/* Disable the overlay display updating to accelerate annotations. */
	MdispControl(m_MilDisplay, M_OVERLAY_SHOW, M_DISABLE);

	/* Draw MIL overlay annotations. */
	/*********************************/

	/* Set graphic text to transparent background. */
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

	/* Print a green string in the overlay image buffer. */
	MgraColor(M_DEFAULT, M_COLOR_MAGENTA); //M_COLOR_RED
	MgraRect(M_DEFAULT, MilOverlayImage, StartX, StartY, StartX+RoiWidth, StartY+RoiHeight);

	/* Re-enable the overlay display after all annotations are done. */
	MdispControl(m_MilDisplay, M_OVERLAY_SHOW, M_ENABLE);

	m_isOverlayEnabled = TRUE;
}
void CAnalogGrabber::OverlayClear()
{
	// Disable overlay
	if (m_isOverlayEnabled)
	{
		// Disable the overlay display. [CALL TO MIL] 
		MdispControl(m_MilDisplay, M_OVERLAY, M_DISABLE);
      
		m_isOverlayEnabled     = FALSE;
     }  
}

/* Maximum number of images for the multiple buffering grab. */
#define NB_GRAB_IMAGE_MAX  10

void CAnalogGrabber::Grab_Average()
{
	MIL_ID  MilGrabImages[NB_GRAB_IMAGE_MAX+1];
	long    NbFrames=0;
	long    n = 0;
	BYTE* pImageDataT;
	int i;

	pImageDataT = new BYTE[m_nImageSize];
	memset(pImageDataT,NULL,m_nImageSize);

	/* Allocate the grab buffers to hold the sequence buffering. */
	MappControl(M_ERROR, M_PRINT_DISABLE);
	for (NbFrames=0, n=0; n<NB_GRAB_IMAGE_MAX; n++)
    {
		MbufAllocColor(m_MilSystem,
                     (long)MdigInquire(m_MilDigitizer, M_SIZE_BAND, M_NULL),
                     (long)(MdigInquire(m_MilDigitizer, M_SIZE_X, M_NULL)),
                     (long)(MdigInquire(m_MilDigitizer, M_SIZE_Y, M_NULL)),
                     8L+M_UNSIGNED, M_IMAGE+M_GRAB, &MilGrabImages[n]);
		if (MilGrabImages[n])
        {
			NbFrames++;
			MbufClear(MilGrabImages[n], 0xFF);
        }
		else
			break;
	}
	MappControl(M_ERROR, M_PRINT_ENABLE);

	/* Free a buffer to leave some space for possible temporary buffer. */
//	NbFrames--;
//	MbufFree(MilGrabImages[NbFrames]);

	/* Put the digitizer in asynchronous mode to be able to process while grabbing. */
	MdigControl(m_MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);

	MdigGrab(m_MilDigitizer, m_MilImage);
//	MbufGetColor(m_MilImage,M_PACKED | M_BGR24,M_ALL_BANDS, m_pImageData);


	for (n=0; n<NbFrames; n++)
	{
		/* Grab one buffer at a time. */
		MdigGrab(m_MilDigitizer, MilGrabImages[n]);
		if(n > 0)
		{
			MbufCopy(MilGrabImages[n-1], m_MilImage);
			MbufGetColor(MilGrabImages[n-1],M_PACKED | M_BGR24,M_ALL_BANDS, pImageDataT);
			for(int i=0; i < m_nImageSize; i++)
			{
				m_pImageData[i] = (BYTE) ((m_pImageData[i] + pImageDataT[i])/2);
			}
		}
		else{
			MbufGetColor(m_MilImage,M_PACKED | M_BGR24,M_ALL_BANDS, m_pImageData);
		}
	}
	/* Wait until the end of the last grab and stop timer. */
	MdigGrabWait(m_MilDigitizer, M_GRAB_END);
	MdigControl(m_MilDigitizer, M_GRAB_MODE, M_SYNCHRONOUS);
	for (n=0; n<NbFrames; n++){
       MbufFree(MilGrabImages[n]);
	}
	delete pImageDataT;

}

MIL_INT MFTYPE DisplayErrorExt(MIL_INT HookType, MIL_ID EventId, void MPTYPE *UserDataPtr)
{
	//If user clicks NO on error message, unhook to errors.
	if(AnalogControl.DisplayError(HookType, EventId, (CWinApp*) UserDataPtr) == M_NO_ERROR)
	{
		MappHookFunction(M_ERROR_CURRENT+M_UNHOOK,DisplayErrorExt,UserDataPtr);
		AnalogControl.HookedOnErrors(false);
	}
	
	return M_NULL;
}

long MFTYPE CAnalogGrabber::DisplayError(MIL_INT HookType, 
									   MIL_ID EventId, void MPTYPE *UserDataPtr)
{
	MIL_TEXT_CHAR ErrorMessageFunction[M_ERROR_MESSAGE_SIZE] = MIL_TEXT("");
	MIL_TEXT_CHAR ErrorMessage[M_ERROR_MESSAGE_SIZE]         = MIL_TEXT("");
	MIL_TEXT_CHAR ErrorSubMessage1[M_ERROR_MESSAGE_SIZE]     = MIL_TEXT("");
	MIL_TEXT_CHAR ErrorSubMessage2[M_ERROR_MESSAGE_SIZE]     = MIL_TEXT("");
	MIL_TEXT_CHAR ErrorSubMessage3[M_ERROR_MESSAGE_SIZE]     = MIL_TEXT("");
	MIL_INT NbSubCode;
	CString   CErrorMessage;
	
	//Retrieve error message [CALL TO MIL]
	MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_FCT,ErrorMessageFunction);
	MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT,ErrorMessage);
	MappGetHookInfo(EventId,M_CURRENT_SUB_NB,&NbSubCode);   
	
	if (NbSubCode > 2)
		MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_SUB_3,ErrorSubMessage3);   
	if (NbSubCode > 1)   
		MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_SUB_2,ErrorSubMessage2);
	if (NbSubCode > 0)
		MappGetHookInfo(EventId,M_MESSAGE+M_CURRENT_SUB_1,ErrorSubMessage1);
	
	CErrorMessage = ErrorMessageFunction;
	CErrorMessage = CErrorMessage + "\n";
	CErrorMessage = CErrorMessage + ErrorMessage;
	
	if(NbSubCode > 0)
	{
		CErrorMessage = CErrorMessage + "\n";
		CErrorMessage = CErrorMessage + ErrorSubMessage1;
	}
	
	if(NbSubCode > 1)
	{
		CErrorMessage = CErrorMessage + "\n";
		CErrorMessage = CErrorMessage + ErrorSubMessage2;
	}
	
	if(NbSubCode > 2)
	{
		CErrorMessage = CErrorMessage + "\n";
		CErrorMessage = CErrorMessage + ErrorSubMessage3;
	}
	
	CErrorMessage = CErrorMessage + "\n\n";
	CErrorMessage = CErrorMessage + "Do you want to continue error print?";
	
	if(AfxMessageBox(CErrorMessage,MB_YESNO,0)==IDNO)
		return M_NO_ERROR;
	
	else
		return M_ERROR;
}