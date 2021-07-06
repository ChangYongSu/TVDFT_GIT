#include "stdafx.h"
#include "Pris.h"
#include "Metrox.h"
#include "direct.h"
#include "ImageUtil.h"
#include "Definition.h"
///////////////////////////////////////////////////////////////////////
CMetrox::CMetrox()
{
	m_milImage = M_NULL;		
	m_milDigitizer = M_NULL;
	m_milSystem = M_NULL;
	m_milApplication = M_NULL;
	m_milDisplay = M_NULL;
	m_bGrabIsStarted = FALSE;
}

CMetrox::~CMetrox()
{
	
	if(m_milImage != M_NULL)		MbufFree(m_milImage);
	if(m_milDigitizer != M_NULL)	MdigFree(m_milDigitizer);
	if(m_milSystem != M_NULL)		MsysFree(m_milSystem);
	if(m_milApplication != M_NULL)	MappFree(m_milApplication);
	if(m_milDisplay != M_NULL)      MappFree(m_milDisplay);
    
}



long CMetrox::fnAllocApp()
{
	MappAlloc(M_SETUP, &m_milApplication);
	MappGetError(M_CURRENT, &m_lLastError);

	return	m_lLastError;
}

long CMetrox::fnControlApp()
{
	MappControl(M_ERROR, M_PRINT_DISABLE);
	MappGetError(M_CURRENT, &m_lLastError);

	return	m_lLastError;
}

long CMetrox::fnAllocSys()
{
	MsysAlloc(M_SYSTEM_SETUP,M_DEV0,M_COMPLETE,&m_milSystem);
	MappGetError(M_CURRENT, &m_lLastError);

	return	m_lLastError;
}

long CMetrox::fnAllocColorBuf()
{
long lImgSizeX, lImgSizeY, lBand;
	
	
    MdigInquire(m_milDigitizer,M_SIZE_X,&lImgSizeX);
    MdigInquire(m_milDigitizer,M_SIZE_Y,&lImgSizeY);  	
    MdigInquire(m_milDigitizer,M_SIZE_BAND,&lBand);
	//MbufAllocColor(m_milSystem, lBand, lImgSizeX, lImgSizeY, M_DEF_IMAGE_TYPE, M_IMAGE+M_DISP+M_GRAB+M_PROC, &m_milImage);
	MbufAllocColor(m_milSystem, lBand, lImgSizeX, lImgSizeY, M_DEF_IMAGE_TYPE, M_DEF_IMAGE_ATTRIBUTE_MIN + M_GRAB, &m_milImage);

	MappGetError(M_CURRENT, &m_lLastError);

	return	m_lLastError;
}

long CMetrox::fnAllocDig()
{
	MdigAlloc(m_milSystem, M_DEFAULT, M_CAMERA_SETUP, M_DEFAULT, &m_milDigitizer);
	return	m_lLastError;
}

long CMetrox::fnGrabImage()
{
	MdigGrab(m_milDigitizer, m_milImage);
	MappGetError(m_milDigitizer, &m_lLastError);

	return	m_lLastError;
}

long CMetrox::fnSaveImage(char *pchFileName)
{
	MbufExport(pchFileName, M_BMP, m_milImage);
	MappGetError(M_CURRENT, &m_lLastError);

	return	m_lLastError;
}

long CMetrox::fnAllocDisplay()
{
    MdispAlloc(m_milSystem,	M_DEFAULT, M_DISPLAY_SETUP, M_WINDOWED, &m_milDisplay);

      /* Do not allow example to run in dual screen mode */             
     MdispInquire(m_milDisplay, M_DISPLAY_MODE, M_NULL); 
	 
	 //확인 할것....
	 
	 MdispZoom   (m_milDisplay, -2,-2);
                                              
     MdispControl(m_milDisplay,M_WINDOW_SCROLLBAR,M_DISABLE);
	 

   	   
   	 // Select the buffer from it's display object and given window
   	 MdispSelectWindow(m_milDisplay,m_milImage,m_dispWnd->m_hWnd);
   	 return m_lLastError;
}

long CMetrox::fnGrabStart()
{
 m_bGrabIsStarted = TRUE;
	MdigGrabContinuous(m_milDigitizer,m_milImage);
   MappGetError(m_milDigitizer, &m_lLastError);
   return	m_lLastError;
}

long CMetrox::fnGrabStop()
{
    m_bGrabIsStarted = FALSE;
	MdigHalt(m_milDigitizer);
    MappGetError(m_milDigitizer,&m_lLastError);
    return	m_lLastError;
}

void CMetrox::fnInitMetrox()
{
    /////
	/*
	fnRemoveDisplay();
    fnAllFree();
	fnReadyMatrox();
    /////
	*/
	m_lLastError = 0;

	// initialize process
	int		nSeq = 0;
	BOOL	bIsSeqEnd = FALSE;
	CString	stPath;


	while(!m_lLastError && !bIsSeqEnd)
	{
		switch(nSeq)
		{
		case 0:
			{
				// get full path name
				//stPath.LoadString(IDS_METROX_DCF_FOLDER);
				// time out setting for c60 initialize
				//_chdir(stPath.operator LPCTSTR());
				nSeq = 5;
				break;
			}
		case 5:
			{
				// application allocation
				fnAllocApp();
				nSeq = 10;
				break;
			}
		case 10:
			{
				// disable message display
				fnControlApp();
				nSeq = 20;
				break;
			}		
		case 20:
			{
				// system allocation
				fnAllocSys();
				nSeq = 30;
				break;
			}
		case 30:
			{
				// digitizer allocation
				fnAllocDig();
				nSeq = 40;
				break;
			}
		case 40:
			{
				// allocation image buffer
				fnAllocColorBuf();
				nSeq = 45;
				break;
			}
		case 45:
			{
				fnAllocDisplay();
				nSeq =50;
				break;
			}
		case 50:
			{
				bIsSeqEnd = TRUE;
				break;
			}
		default:	break;
		}
	}
}

int CMetrox::fnFindPat()
{
    //int px,py;
	CPDPApp *pApp = (CPDPApp *)AfxGetApp();
	
	imageSizeX=640;
	imageSizeY=480;
	//src Image 공간을 할당한다...
	
	m_Image = new BYTE[imageSizeX*imageSizeY];
    
	//pattern을 찾기위해 Grap...
	MdigGrab(m_milDigitizer, m_milImage);
	//grap한 이미지를 새로운 공간에 할당..
    MbufGet(m_milImage,m_Image);
	
	//Image 영역에서 dia의 중심 좌표를 얻는다..
	//FidService(640,480,m_Image,90,210,160,0,&px,&py);
	FidService(640,480,m_Image,pApp->m_nMinDia,pApp->m_nMaxDia,pApp->m_nThr,0,&px,&py);
	//ViewCenter(px,py,BLUECOLOR,20,TRUE);
	MbufPut(m_milImage,m_Image);

	if((px<0 || px>640) || (py<0 || py>480))
	{
		px = 320;
		py = 240;
	}

    delete [] m_Image;

	return px, py;
}





void CMetrox::ViewCenter(int cx, int cy, int color, int dL, int invalid)
{
	//CPDPApp*	pApp = (CPDPApp *) AfxGetApp();
	//CMainFrame*	pFrame = (CMainFrame *)pApp->m_pMainWnd;
	//CPDPView*	pView = (CPDPView *)pFrame->GetActiveView();
    
	//CPDPApp*	pApp = (CPDPApp *) AfxGetApp();
	//CMainFrame*	pFrame = (CMainFrame *)pApp->m_pMainWnd;
	//CPDPView*	pView = (CPDPView *)pFrame->GetActiveView();
    
	CClientDC dc(m_dispWnd);
    CRect pRect;
	//pView->m_stcPanel.GetClientRect(&pRect);
  	m_dispWnd->GetClientRect(pRect);
	CPen   pNewPen,*pOldPen;
	CBrush pNewBrush,*pOldBrush;
	//CDC    *theDC = GetDC();
	
	pNewPen.CreatePen(PS_SOLID,1,color);  
	pNewBrush.CreateSolidBrush(color);			
	pOldPen   = dc.SelectObject(&pNewPen); 
	pOldBrush = dc.SelectObject(&pNewBrush); 
	pRect.left= cx-dL/2, pRect.right = cx+dL/2, pRect.top = cy-dL/2, pRect.bottom= cy+dL/2;
	dc.MoveTo(pRect.left,pRect.top),    dc.LineTo(pRect.right,pRect.bottom); //Image로 전환할때는  +m_*offset - 10을 더해줘야 한다.
	dc.MoveTo(pRect.left,pRect.bottom), dc.LineTo(pRect.right,pRect.top);
	//잠시 LimDC->Rectangle(&pRect);	 
	dc.SelectObject(pOldBrush),			dc.SelectObject(pOldPen);
	ReleaseDC(m_dispWnd->m_hWnd,dc);

	
}

BOOL CMetrox::fnAlignValid()
{
    
    if (px == 0 || py == 0)
	{
	    AfxMessageBox("Align이 제대로 되지 않았습니다.align을 다시 확인해 주십시요.");
		return FALSE;

	}
	   else
	{
	   return TRUE;
	}

}

void CMetrox::DrawCross(int sx, int sy, int len, COLORREF color)
{
    len/=2;
	CClientDC		dc(m_dispWnd);
	CRect pRect;
    m_dispWnd->GetClientRect(pRect);
	CPen* pPen = 	new CPen;
	CPen* pOldPen; 
	pPen->CreatePen(PS_SOLID, 1, color);
	dc.SelectStockObject(NULL_BRUSH);
	pOldPen = dc.SelectObject(pPen); 

	dc.MoveTo(sx-len, sy);
	dc.LineTo(sx+len, sy);
	dc.MoveTo(sx, sy-len);
	dc.LineTo(sx, sy+len);

	dc.SelectObject(pOldPen);
	delete pPen; 
}

void CMetrox::fnRemoveDisplay()
{
    // Halt grab if in process in THIS view
	
	if ((m_dispWnd)&& (m_bGrabIsStarted))
	{
	   	MdigHalt(m_milDigitizer);
    	m_bGrabIsStarted = FALSE;	
	}
   	
   	
   	// Deselect the buffer from it's display object and given window 
  	if (m_milImage && m_milDisplay) MdispDeselect(m_milDisplay, m_milImage);
	
	// Free the display
	if (m_milDisplay!= M_NULL)      MdispFree(m_milDisplay);
	//if(m_milDisplay != M_NULL)      MappFree(m_milDisplay);
	m_milDisplay = M_NULL;
}

void CMetrox::DrawLine(int sx, int sy, int ex, int ey, COLORREF color)
{
  	CClientDC dc(m_dispWnd);
    CRect pRect;
    m_dispWnd->GetClientRect(pRect);
	
	CPen* pPen = 	new CPen;
 	CPen* pOldPen; 
 	pPen->CreatePen(PS_SOLID, 1, color);
 	dc.SelectStockObject(NULL_BRUSH);
 	pOldPen = dc.SelectObject(pPen); 
 
 	dc.MoveTo(sx, sy);
 	dc.LineTo(ex, ey);
 
 	dc.SelectObject(pOldPen);
 	delete pPen; 
}

void CMetrox::fnOverlay()
{
   	/////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'add overlay' is selected
   /////////////////////////////////////////////////////////////////////////

   // Initialize overlay if not already done
   if ( (!fOverlayIsInitialized) && (m_milDisplay))
      {
      if ((MdispInquire(m_milDisplay, M_DISPLAY_MODE, M_NULL) == M_WINDOWED ) &&
          (m_milImage) && (m_milDisplay) )
         {
         // Prepare overlay buffer //
         ////////////////////////////
         
         // Disable overlay display while preparing the data
 	      MdispControl(m_milDisplay, M_WINDOW_OVR_SHOW, M_DISABLE);
 	           
         // Enable writting Overlay graphics on top of display buffer.
 	      MdispControl(m_milDisplay, M_WINDOW_OVR_WRITE, M_ENABLE);
 	           
         // Inquire the Overlay buffer associated with the displayed buffer.
 	      MdispInquire(m_milDisplay, M_WINDOW_OVR_BUF_ID, &m_milOverlayImage);
         
         // Inquire the current keying color.
         MdispInquire(m_milDisplay, M_KEY_COLOR, &TransparentColor);    
                                                                      
         // Clear the overlay buffer with the keying color.           
         MbufClear(m_milOverlayImage, TransparentColor); 

         // Draw MIL monochrome overlay annotation *
         //*****************************************

         // Inquire MilOverlayImage size x and y. 
         ImageWidth  = MbufInquire(m_milOverlayImage,M_SIZE_X,M_NULL);
         ImageHeight = MbufInquire(m_milOverlayImage,M_SIZE_Y,M_NULL);
            
         // Set graphic text to transparent background.
         MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

         // Set drawing color to white.
         MgraColor(M_DEFAULT, 255);


		  // Draw GDI color overlay annotation *
         //************************************

         // Create a device context to draw in the overlay buffer with GDI. 
         MbufControl(m_milOverlayImage, M_WINDOW_DC_ALLOC, M_DEFAULT);

         // Attach the device context.
         CDC NewDC;
         NewDC.Attach((HDC)MbufInquire(m_milOverlayImage, M_WINDOW_DC, M_NULL));

		 CPen* pPen = 	new CPen;
     	 CPen* pOldPen; 
     	 pPen->CreatePen(PS_SOLID, 1, RGB(255,0,0));
 	     NewDC.SelectStockObject(NULL_BRUSH);
     	 pOldPen = NewDC.SelectObject(pPen); 
 
 	     NewDC.MoveTo(0,ImageHeight/2);
         NewDC.LineTo(ImageWidth,ImageHeight/2);
         NewDC.MoveTo(ImageWidth/2,0);
         NewDC.LineTo(ImageWidth/2,ImageHeight);
 
         NewDC.SelectObject(pOldPen);
 	     delete pPen; 
         
         // Detach the device context.
         NewDC.Detach();
         
         // Delete created device context. 
         MbufControl(m_milOverlayImage, M_WINDOW_DC_FREE, M_DEFAULT);

         // Overlay is now initialized
         fOverlayIsInitialized = TRUE;
         }
      }   
   
   // Make sure that overlay is initialized
   if (fOverlayIsInitialized)
      {
      // Enable overlay
      if (OverlayIsEnable == FALSE)
         {
         // Enable the overlay display. 
         MdispControl(m_milDisplay, M_WINDOW_OVR_SHOW, M_ENABLE);

         OverlayIsEnable = TRUE;
         }
      
      // Disable overlay
      else
         {
         // Disable the overlay display. 
         MdispControl(m_milDisplay, M_WINDOW_OVR_SHOW, M_DISABLE);

         OverlayIsEnable = FALSE;
         }
      }
   
   
     // The following lines show how to deinitialize the overlay if necessary:
//   if (fOverlayIsInitialized)
//      {
//      // Disable the overlay display. 
//      MdispControl(MilDisplay, M_WINDOW_OVR_SHOW, M_DISABLE;
//         
//      // If the overlay buffer is monochrome, reomove the pseudo-color 
//      // LUT from the overlay display.        
//      if (MbufInquire(MilOverlayImage, M_SIZE_BAND, M_NULL) == 1)
//         {
//         // Associate a default LUT to the overlay display. 
//         MdispControl(MilDisplay, M_WINDOW_OVR_LUT, M_DEFAULT);
//         }
//
//      // Disable writing Overlay graphics on top of display buffer.
//      MdispControl(MilDisplay, M_WINDOW_OVR_WRITE, M_DISABLE);
//      }
   
   
   /////////////////////////////////////////////////////////////////////////
   // MIL: Write code that will be executed when 'add overlay' is selected
   /////////////////////////////////////////////////////////////////////////
 
}

void CMetrox::fnAllFree()
{
    if(m_milImage != M_NULL)		MbufFree(m_milImage);
	if(m_milDigitizer != M_NULL)	MdigFree(m_milDigitizer);
	if(m_milSystem != M_NULL)		MsysFree(m_milSystem);
	if(m_milApplication != M_NULL)	MappFree(m_milApplication);
	if(m_milDisplay != M_NULL)      MappFree(m_milDisplay);
	if(m_milOverlayImage !=M_NULL)  MappFree(m_milOverlayImage);
//	if(m_milDisplay != M_NULL)      MdispFree(m_milDisplay);
	//if(m_milOverlayImage !=M_NULL)  MdispFree(m_milOverlayImage);
}

void CMetrox::fnReadyMatrox()
{

	m_milImage = M_NULL;		
	m_milDigitizer = M_NULL;
	m_milSystem = M_NULL;
	m_milApplication = M_NULL;
	m_milDisplay = M_NULL;
	m_milOverlayImage =M_NULL;
	OverlayIsEnable       = FALSE;    // Overlay state
    fOverlayIsInitialized = FALSE;
	m_bGrabIsStarted = FALSE;
	//m_bMatroxInit = FALSE;

}
