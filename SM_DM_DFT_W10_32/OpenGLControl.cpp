#include "stdafx.h"
#include "OpenGLControl.h"
//#include ".\openglcontrol.h"
#include "math.h"

#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286f

COpenGLControl::COpenGLControl(void)
{
	m_fPosX = 0.0f;		// X position of model in camera view
	m_fPosY = 0.0f;		// Y position of model in camera view
	m_fZoom = 10.0f;	// Zoom on model in camera view
	m_fRotX = 0.0f;		// Rotation on model in camera view
	m_fRotY	= 0.0f;		// Rotation on model in camera view
	m_fRotZ = 0.0f;	
	m_bIsMaximized = false;
	m_bDisplayVoltEnable = 0;
	m_DisplayPage = __DISPLAY_PAGE_MAIN;
}

COpenGLControl::~COpenGLControl(void)
{
	destroy();
}

BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

void COpenGLControl::destroy(void)
{
	if (hrc != NULL)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
	}
}




void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

	// Map the OpenGL coordinates.
	glViewport(0, 0, cx, cy);

	// Projection view
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glOrtho(-1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f);
	//glOrtho(-5.0f,5.0f,-5.2f,5.0f,100.0f,-100.0f);
	// Set our current view perspective
	//gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f);

	// Model view
	glMatrixMode(GL_MODELVIEW);

	switch (nType)
	{
		// If window resize token is "maximize"
		case SIZE_MAXIMIZED:
		{
			// Get the current window rect
			GetWindowRect(m_rect);

			// Move the window accordingly
			MoveWindow(6, 6, cx - 14, cy - 14);

			// Get the new window rect
			GetWindowRect(m_rect);

			// Store our old window as the new rect
			m_oldWindow = m_rect;

			break;
		}

		// If window resize token is "restore"
		case SIZE_RESTORED:
		{
			// If the window is currently maximized
			if (m_bIsMaximized)
			{
				// Get the current window rect
				GetWindowRect(m_rect);

				// Move the window accordingly (to our stored old window)
				MoveWindow(m_oldWindow.left, m_oldWindow.top - 18, m_originalRect.Width() - 4, m_originalRect.Height() - 4);

				// Get the new window rect
				GetWindowRect(m_rect);

				// Store our old window as the new rect
				m_oldWindow = m_rect;
			}
		
			break;
		}
	}
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	oglInitialize();
	for(int i = 0; i < 1000; i++)
	{
		m_PlotData[i].Data = -0.7;
		m_PlotData[i].bNG = 0;
	}

	return 0;
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	// If the current view is perspective...
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -m_fZoom);
//	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
	//glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
}

void COpenGLControl::Rotate()
{
	// If the current view is perspective...
	//glLoadIdentity();
	
	glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
	
}

void COpenGLControl::OnTimer(UINT_PTR  nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			
			// Clear color and depth buffer bits
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Draw OpenGL scene
			//oglDrawScene();
			DrawGLScene();

			// Swap buffers
			//SwapBuffers(hdc);

			break;
		}

		default:
			break;
	}

	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX  = (float)point.x;
	m_fLastY  = (float)point.y;

	// Left mouse button
	if (nFlags & MK_LBUTTON)
	{
		m_fRotX += (float)0.5f * diffY;

		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
		{
			m_fRotX = 0.0f;
		}

		m_fRotY += (float)0.5f * diffX;

		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
		{
			m_fRotY = 0.0f;
		}
	}

	// Right mouse button
	else if (nFlags & MK_RBUTTON)
	{
		m_fZoom -= (float)0.1f * diffY;
		m_fRotZ += (float)0.5f * diffX;
	}

	// Middle mouse button
	else if (nFlags & MK_MBUTTON)
	{
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;
	}

	OnDraw(NULL);

	//// Clear color and depth buffer bits
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// Draw OpenGL scene
	//oglDrawScene();

	//// Swap buffers
	//SwapBuffers(hdc);
	*/

	CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::oglCreate(CRect rect, CWnd *parent, LPCTSTR lWindName)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, (HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, lWindName, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);
//	CreateEx(0, className, "OpenGL", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);
	//"Afx:001A0000:23:00000000:00900011:00000000"
	// Set initial variables' values
	m_oldWindow	   = rect;
	m_originalRect = rect;

	hWnd = parent;
}

void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32, // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16, // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};

	// Get device context only once.
	hdc = GetDC()->m_hDC;
	
	// Pixel format.
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);

	// Create the OpenGL Rendering Context.
	hrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hrc);

	// Basic Setup:
	//
	// Set color to use when clearing the background.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	// Turn on backface culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	
	// Turn on depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	glEnable (GL_POINT_SMOOTH) ;
	glEnable (GL_LINE_SMOOTH) ;
	glEnable(GL_POLYGON_SMOOTH) ;
	/*glEnable (GL_POINT_SMOOTH_HINT) ;
	glEnable (GL_LINE_SMOOTH_HINT) ;(GL_BLEND)
	glEnable (GL_POLYGON_SMOOTH_HINT) ;*/
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable (GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//loadImage();
	/*  texture[0] = SOIL_load_OGL_texture
            (
                "dcenter.bmp",
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS
                );*/

   // glEnable(GL_TEXTURE_2D);

	//glBitmap(16,16,0,10,20,0,bitmap);




	// Send draw request
	OnDraw(NULL);
}
//
//GLvoid COpenGLControl::BuildFont(GLvoid)  
//{  
//    HFONT font; // <1>  
//    base = glGenLists(96); // <2>  
//    font = CreateFont(-24, // <3-1>  
//                0,  
//                0,  
//                0,  
//                FW_BOLD, // <3-2>  
//                FALSE, // <3-3>  
//                FALSE, // <3-4>  
//                FALSE, // <3-5>  
//                ANSI_CHARSET, // <3-6>  
//                OUT_TT_PRECIS,  
//                CLIP_DEFAULT_PRECIS,  
//                ANTIALIASED_QUALITY,  
//                FF_DONTCARE|DEFAULT_PITCH,  
//                _T("Courier New")); // <3-6>  
//     
//    SelectObject(hdc, font); // <4>  
//    wglUseFontBitmaps(hdc, 32, 96, base); // <5>  
//}  
//
//GLvoid COpenGLControl::KillFont(GLvoid)  
//{  
//    glDeleteLists(base, 96);  
//}  
//
//GLvoid COpenGLControl::glPrint(const char *text)  
//{  
//    glPushAttrib(GL_LIST_BIT); //<1>  
//    glListBase(base - 32); //<2>  
//    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); //<3>  
//    glPopAttrib(); //<4>  
//}  
//

//
//void COpenGLControl::oglDrawScene(void)
//{
//	// Wireframe Mode
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//	//glBegin(GL_QUADS);
//	//		// Front Side
//	//		glVertex3f( 1.0f,  1.0f, 1.0f);
//	//		glVertex3f(-1.0f,  1.0f, 1.0f);
//	//		glVertex3f(-1.0f, -1.0f, 1.0f);
//	//		glVertex3f( 1.0f, -1.0f, 1.0f);
//
//	//		// Back Side
//	//		glVertex3f(-1.0f, -1.0f, -1.0f);
//	//		glVertex3f(-1.0f,  1.0f, -1.0f);
//	//		glVertex3f( 1.0f,  1.0f, -1.0f);
//	//		glVertex3f( 1.0f, -1.0f, -1.0f);
//
//	//		// Top Side
//	//		glVertex3f( 1.0f, 1.0f,  1.0f);
//	//		glVertex3f( 1.0f, 1.0f, -1.0f);
//	//		glVertex3f(-1.0f, 1.0f, -1.0f);
//	//		glVertex3f(-1.0f, 1.0f,  1.0f);
//
//	//		// Bottom Side
//	//		glVertex3f(-1.0f, -1.0f, -1.0f);
//	//		glVertex3f( 1.0f, -1.0f, -1.0f);
//	//		glVertex3f( 1.0f, -1.0f,  1.0f);
//	//		glVertex3f(-1.0f, -1.0f,  1.0f);
//
//	//		// Right Side
//	//		glVertex3f( 1.0f,  1.0f,  1.0f);
//	//		glVertex3f( 1.0f, -1.0f,  1.0f);
//	//		glVertex3f( 1.0f, -1.0f, -1.0f);
//	//		glVertex3f( 1.0f,  1.0f, -1.0f);
//
//	//		// Left Side
//	//		glVertex3f(-1.0f, -1.0f, -1.0f);
//	//		glVertex3f(-1.0f, -1.0f,  1.0f);
//	//		glVertex3f(-1.0f,  1.0f,  1.0f);
//	//		glVertex3f(-1.0f,  1.0f, -1.0f);
//	//glEnd();
//
//	glPushMatrix();
//	glTranslatef(m_fPosX, m_fPosY, 0.0f);
//
////	glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//		glBegin(GL_QUADS);
//			// Front Side
//			glVertex3f( 1.0f,  1.0f, 1.0f);
//			glVertex3f(-1.0f,  1.0f, 1.0f);
//			glVertex3f(-1.0f, -1.0f, 1.0f);
//			glVertex3f( 1.0f, -1.0f, 1.0f);
//		glEnd();
//	glPopMatrix();
//
//	//glColor3f(1.0f, 1.0f, 0.0f);   
//	//glRasterPos2f(1.0f, 0.0f);
//	//glPrint("0123456789");  
//
//}
//


void COpenGLControl::DrawGLScene()
{
	

	glClear(GL_COLOR_BUFFER_BIT
		 |  GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();

	if (m_DisplayPage == __DISPLAY_PAGE_MAIN)
	{
		DrawBaseGround();
	}
	else
	{
		DrawTotalBaseGround();
	}
	

	
	if (m_DisplayPage == __DISPLAY_PAGE_MAIN)
	{
		DrawPlot();
	}
	else //if (m_DisplayPage == __DISPLAY_PAGE_TOTAL_LEFT)
	{
		DrawTotalPlot(m_DisplayPage);
	}
	//else if (m_DisplayPage == __DISPLAY_PAGE_TOTAL_RIGHT)
	//{
	//	DrawTotalPlot(__DISPLAY_PAGE_TOTAL_RIGHT);
	//}
	
	
	glColor3f(1.0f, 1.0f, 0.0f);   
	glRasterPos2f(-1.0f, 0.0f);     



	SwapBuffers(hdc);
}



void  COpenGLControl::DrawISFD() 
{

	glBegin(GL_TRIANGLES);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,-1.0f,0.0f);
		glColor3f(0.0f,0.1f,0.0f);
		glVertex3f(-1.0f,-1.0f,0.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,1.0f,0.0f);
	glEnd();

	glBegin(GL_QUADS);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-1.0f,-1.0,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,0.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(1.0f,1.0f,0.0f);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex3f(-1.0f,0.7f,0.0f);
	glEnd();

	/*
		m_Simulator_Dp.SetRoll(Roll_Value);
		m_Simulator_Dp.SetPitch(Pitch_Value);
		m_Simulator_Dp.SetAirspeed(Airspeed_Value);
		m_Simulator_Dp.SetAltitude(Altitude_Value);
	*/
}

void  COpenGLControl::SMglNumber(int lnumber,float lPosX , float lPosY , float lFontSize, int lwidth)
{
	
	float lCenterX = lPosX ;
	float lCenterY = lPosY ;
	float lSize = lFontSize;

	float lfontwidth = lSize/10.0f;

	glLineWidth(lwidth);


	switch(lnumber)
	{
	case 1:
	
		glBegin(GL_LINE_STRIP);
			//glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(lCenterX, lCenterY - 0.5f*lSize, 0.0f);		
			glVertex3f(lCenterX, lCenterY + 0.5f*lSize,0.0f);	

			glVertex3f(lCenterX - 0.2 * lSize, lCenterY + 0.3f * lSize,0.0f);
		
		glEnd();

		break;

	case 2:		
	
		glBegin(GL_LINE_STRIP);
			
		
		glEnd();

		SMglArc(lCenterX, lCenterY + 0.2f * lSize, 0.3f * lSize, 270, 200,  50 );
		glBegin(GL_LINE_STRIP);
			//glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.15f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.1f * lSize, 0.0f);	

			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.4f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY - 0.5f * lSize, 0.0f);			
		
		glEnd();
		break;
			
	case 3:	
	

		SMglArc(lCenterX, lCenterY + 0.25f * lSize, 0.25f * lSize, 270, 270,  50 );

		SMglArc(lCenterX, lCenterY - 0.25f * lSize, 0.25f * lSize, 0, 270,  50 );
	
		break;
	case 4:	
		glBegin(GL_LINE_STRIP);

			glVertex3f(lCenterX + 0.2f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.2f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY - 0.2f * lSize, 0.0f);	
				
		
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex3f(lCenterX + 0.2f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
		glVertex3f(lCenterX + 0.2f * lSize, lCenterY - 0.5f * lSize, 0.0f);	
			
		glEnd();


		break;

		
	case 5:		
	
		glBegin(GL_LINE_STRIP);			

			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.15f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.28f * lSize, lCenterY - 0.1f * lSize, 0.0f);					
		
		glEnd();
	
		SMglArc(lCenterX, lCenterY - 0.2f * lSize, 0.3f * lSize, 300, 310,  50 );	
		break;

			
	case 6:	
	
		glBegin(GL_LINE_STRIP);			
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY + 0.2f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.2f * lSize, 0.0f);				
		
		glEnd();

		SMglArc(lCenterX, lCenterY + 0.2f * lSize, 0.3f * lSize, 270, 160,  50 );

		SMglArc(lCenterX, lCenterY - 0.2f * lSize, 0.3f * lSize, 0, 360,  50 );	
		break;
		
	case 7:

		glBegin(GL_LINE_STRIP);
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.5f * lSize, 0.0f);				
		
		glEnd();	
		break;
			
	case 8:

		SMglArc(lCenterX, lCenterY + 0.25f * lSize, 0.25f * lSize, 275, 170,  50 );
		SMglArc(lCenterX, lCenterY + 0.3f * lSize, 0.25f * lSize, 95, 170,  50 );

		SMglArc(lCenterX, lCenterY - 0.25f * lSize, 0.3f * lSize, 275, 170,  50 );
		SMglArc(lCenterX, lCenterY - 0.2f * lSize, 0.3f * lSize, 95, 170,  50 );
	
		break;
			
	case 9:
			
		glBegin(GL_LINE_STRIP);
			
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.2f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY - 0.2f * lSize, 0.0f);	
						
		glEnd();

		SMglArc(lCenterX, lCenterY + 0.2f * lSize, 0.3f * lSize, 0, 360,  50 );
		SMglArc(lCenterX, lCenterY - 0.2f * lSize, 0.3f * lSize, 90, 160,  50 );	
		break;


	case 0:

		glBegin(GL_LINE_STRIP);			
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.2f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY - 0.2f * lSize, 0.0f);			
		glEnd();	

		glBegin(GL_LINE_STRIP);
			//glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY + 0.2f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.2f * lSize, 0.0f);	
		glEnd();
		
		SMglArc(lCenterX, lCenterY + 0.2f * lSize, 0.3f * lSize, 270, 180,  50 );
		SMglArc(lCenterX, lCenterY - 0.2f * lSize, 0.3f * lSize, 90, 180,  50 );
	
		break;
	default:
		break;
	}

	
	
}

void  COpenGLControl::SMglChar(char lAlpha,float lPosX , float lPosY , float lFontSize, int lwidth)
{
	
	float lCenterX = lPosX ;
	float lCenterY = lPosY ;
	float lSize = lFontSize;

	float lfontwidth = lSize/10.0f;

	glLineWidth(lwidth);


	switch(lAlpha)
	{
	case 'N':
	
		glBegin(GL_LINE_STRIP);
			//glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.5f*lSize, 0.0f);		
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY + 0.5f*lSize,0.0f);
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY - 0.5f*lSize,0.0f);
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.5f*lSize,0.0f);				
		
		glEnd();

		break;

	case 'E':		
	
		
		glBegin(GL_LINE_STRIP);
			//glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY - 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY - 0.5f * lSize, 0.0f);	
		
		glEnd();
		glBegin(GL_LINES);
			//glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(lCenterX + 0.3f * lSize, lCenterY , 0.0f);	
			glVertex3f(lCenterX - 0.3f * lSize, lCenterY , 0.0f);	

		
		glEnd();
					
		break;
			
	case 'S':	
	

		SMglArc(lCenterX, lCenterY + 0.25f * lSize, 0.25f * lSize, 180, 280,  50 );

		SMglArc(lCenterX, lCenterY - 0.25f * lSize, 0.25f * lSize, 0, 280,  50 );

		//glBegin(GL_LINES);
		//	//glColor3f(1.0f,0.0f,0.0f);
		//	glVertex3f(lCenterX + 0.15f * lSize, lCenterY - 0.125 * lSize , 0.0f);	
		//	glVertex3f(lCenterX - 0.15f * lSize, lCenterY + 0.125 * lSize, 0.0f);	

		//
		//glEnd();
	
		break;
	case 'W':	
		glBegin(GL_LINE_STRIP);

			glVertex3f(lCenterX - 0.4f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX - 0.2f * lSize, lCenterY - 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.0f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.2f * lSize, lCenterY - 0.5f * lSize, 0.0f);	
			glVertex3f(lCenterX + 0.4f * lSize, lCenterY + 0.5f * lSize, 0.0f);	
				
		
		glEnd();		


		break;

		

	default:
		break;
	}

	
	
}
void  COpenGLControl::SMglArc(float lCentX, float lCenty, float lRadius, float lStart, float lAngleSize,  int lStep )
{

	lStart = lStart/180*PI; 
	float lEnd = lStart + lAngleSize/180*PI; 
	float fstep = (lEnd - lStart)/lStep;
	//float fstep = 
	glBegin(GL_LINE_STRIP);
		
			
		for(int i = 0; i <  lStep; i++)
		{
			glVertex3f(lCentX + lRadius * sin((lStart + fstep*i)), lCenty + lRadius * cos((lStart + fstep*i)), 0.0f);	
		}
		

		glVertex3f(lCentX + lRadius * sin((lEnd)), lCenty + lRadius *cos(lEnd), 0.0f);		
		
	glEnd();
}

void  COpenGLControl::SMglCircle(float lCentX, float lCenty, float lRadius, float lStart, float lAngleSize,  int lStep )
{

	lStart = lStart/180*PI; 
	float lEnd = lStart + lAngleSize/180*PI; 
	float fstep = (lEnd - lStart)/lStep;
	//float fstep = 
	glBegin(GL_POLYGON);
		
			
		for(int i = 0; i <  lStep; i++)
		{
			glVertex3f(lCentX + lRadius * sin((lStart + fstep*i)), lCenty + lRadius * cos((lStart + fstep*i)), 0.0f);	
		}
		

		glVertex3f(lCentX + lRadius * sin((lEnd)), lCenty + lRadius *cos(lEnd), 0.0f);		
		
	glEnd();
}

void  COpenGLControl::DrawRollTick()
{
	int iNum = 0;
	float fFont;
	int iWidth;

	//ROLL 
	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(4);
	//SMglArc(0.0f, -0.07f, 0.8, 300, 120,  50 );
	SMglArc(0.0f, 0.00f, 0.4, 300, 360,  100 );

	SMglArc(0.0f, 0.00f, 0.8, 300, 360,  150 );

#if 1
	glPushMatrix();
	for(int i = 0 ; i < 12; i++)
	{
//	glRotatef(0,0.0f,0.0f,1.0f);
//	SMglNumber(0, 0, 0.9, 0.06, 3 =
		iNum = i * 3;
		if(i%3 == 0)
		{
			glColor3f(0.8f,0.8f,0.8f);
			if(iNum >= 10)
			{
				SMglNumber(iNum/10, -0.06, 0.9, 0.06, 3);
				SMglNumber(iNum%10, 0.00, 0.9, 0.06, 3);
				SMglNumber(0, 0.06, 0.9, 0.06, 3);
			}
			else if(i == 0)
			{				
				SMglNumber(0, 0.0, 0.9, 0.06, 3);
			}	
			else 
			{
				SMglNumber(iNum, -0.03, 0.9, 0.06, 3);
				SMglNumber(0, 0.03, 0.9, 0.06, 3);
			}
			
		}
		else
		{
			glColor3f(0.7f,0.7f,0.7f);
			
			if(iNum >= 10)
			{
				SMglNumber(iNum/10, -0.05, 0.89, 0.04, 2);
				SMglNumber(iNum%10, 0.00, 0.89, 0.04, 2);
				SMglNumber(0, 0.06, 0.89, 0.04, 2);
			}
			else 
			{
				SMglNumber(iNum, -0.025, 0.89, 0.04, 2);
				SMglNumber(0, 0.025, 0.89, 0.04, 2);
			}
		}
		glRotatef(30.0f,0.0f,0.0f,1.0f);
//	glRotatef(3,0.0f,0.0f,1.0f);
	}

	glPopMatrix();
#endif
	

	//VOLT INPUT
	///////////////////////////////////////////////////////////
	if(m_bDisplayVoltEnable == 1)
	{
		glPushMatrix();

		//	float	 m_fDeviationVoltX;
			//float	 m_fDeviationVoltY;
			//float	 m_bDisplayVoltOK;
			//float	 m_fDirVoltZ;
		glLoadIdentity();
		//glEnable(GL_POINT_SMOOTH) ;
		//glEnable(GL_LINE_SMOOTH) ;
		//glEnable(GL_POLYGON_SMOOTH) ;
	
		if(m_bDisplayVoltOK == 1)
			glColor3f(0.0f,1.0f,0.0f);
		else
			glColor3f(0.0f,0.0f,0.0f);
		glLineWidth(22);

		glTranslatef(0, 0, -0.001f);
		SMglArc(m_fDeviationVoltX, m_fDeviationVoltY, 0.55, 300, 360,  150 );
		//SMglCircle(m_fDeviationVoltX, m_fDeviationVoltY, 0.08, 0, 360,  50 );

		if(m_bDisplayVoltOK == 1)
		{
		}
		else
		{
			//ROLL TriAngle Rolling
			glLineWidth(1);

			glRotatef(m_fDirVoltZ,0.0f,0.0f,1.0f);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f,0.8f,0.0f);		
				glVertex3f(0.07f,0.7f,0.0f);		
				glVertex3f(-0.07f,0.7f,0.0f);
			glEnd();

			glLineWidth(1);

			glBegin(GL_LINES);
		
				glVertex3f(0.0f,0.8f,0.0f);		
				glVertex3f(0.07f,0.7f,0.0f);	

				glVertex3f(0.07f,0.7f,0.0f);		
				glVertex3f(-0.07f,0.7f,0.0f);

				glVertex3f(-0.07f,0.7f,0.0f);
				glVertex3f(0.0f,0.8f,0.0f);		
			glEnd();
		}

		glPopMatrix();

	}
	else
	{

		glPushMatrix();

		//	float	 m_fDeviationVoltX;
			//float	 m_fDeviationVoltY;
			//float	 m_bDisplayVoltOK;
			//float	 m_fDirVoltZ;
		glLoadIdentity();
		//glEnable(GL_POINT_SMOOTH) ;
		//glEnable(GL_LINE_SMOOTH) ;
		//glEnable(GL_POLYGON_SMOOTH) ;
	
		
		
		glColor3f(0.0f,0.0f,0.0f);
		glLineWidth(22);

		glTranslatef(0, 0, -0.001f);
		SMglArc(0, 0, 0.55, 300, 360,  150 );

		glPopMatrix();
	}

	//////////////////////////////////
	glPushMatrix();


	glLoadIdentity();
	//glEnable(GL_POINT_SMOOTH) ;
	//glEnable(GL_LINE_SMOOTH) ;
	//glEnable(GL_POLYGON_SMOOTH) ;
	
	if(m_bDisplayOK)
		glColor3f(0.0f,1.0f,0.0f);
	else
		glColor3f(1.0f,0.0f,0.0f);
	glLineWidth(6);

	glTranslatef(0, 0, -0.001f);
	SMglArc(m_fDeviationX, m_fDeviationY, 0.38, 300, 360,  150 );
	//SMglCircle(m_fDeviationX, m_fDeviationY, 0.08, 0, 360,  50 );

	if(m_bDisplayOK)
	{
	}
	else
	{
		//ROLL TriAngle Rolling
		glLineWidth(1);

		glRotatef(m_fRotZ,0.0f,0.0f,1.0f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f,0.8f,0.0f);		
			glVertex3f(0.07f,0.7f,0.0f);		
			glVertex3f(-0.07f,0.7f,0.0f);
		glEnd();

		glLineWidth(1);

		glBegin(GL_LINES);
		
			glVertex3f(0.0f,0.8f,0.0f);		
			glVertex3f(0.07f,0.7f,0.0f);	

			glVertex3f(0.07f,0.7f,0.0f);		
			glVertex3f(-0.07f,0.7f,0.0f);

			glVertex3f(-0.07f,0.7f,0.0f);
			glVertex3f(0.0f,0.8f,0.0f);		
		glEnd();
	}

	glPopMatrix();



	//ROLL TriAngle fix
	
	//glBegin(GL_TRIANGLES);
	//	glColor3f(1.0f,1.0f,1.0f);
	//	glVertex3f(0.0f,0.8f,0.0f);		
	//	glVertex3f(0.07f,0.9f,0.0f);		
	//	glVertex3f(-0.07f,0.9f,0.0f);
	//glEnd();
	//glBegin(GL_LINES);
	//	
	//	glVertex3f(0.0f,0.8f,0.0f);		
	//	glVertex3f(0.07f,0.9f,0.0f);	

	//	glVertex3f(0.07f,0.9f,0.0f);		
	//	glVertex3f(-0.07f,0.9f,0.0f);

	//	glVertex3f(-0.07f,0.9f,0.0f);
	//	glVertex3f(0.0f,0.8f,0.0f);	
	//glEnd();
	
	//ROLL Tick Minor
	//glBegin(GL_QUADS);
	//	glColor3f(1.0f,1.0f,1.0f);

	//	glVertex3f(0.0f + 0.8*sin(-10.0f/180 * PI), -0.07+0.8f*cos(-10.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(-10.0f/180 * PI), -0.07+0.85f*cos(-10.0f/180 * PI),0.0f);	
	//	glVertex3f(0.0f + 0.85*sin(-10.0f/180 * PI) + 0.02*cos(-10.0f/180 * PI), -0.07+0.85f*cos(-10.0f/180 * PI)- 0.02*sin(-10.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.8*sin(-10.0f/180 * PI) + 0.02*cos(-10.0f/180 * PI), -0.07+0.8f*cos(-10.0f/180 * PI)- 0.02*sin(-10.0f/180 * PI),0.0f);		

	///*	glVertex3f(0.0f + 0.8*sin(-20.0f/180 * PI), -0.07+0.8f*cos(-20.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(-20.0f/180 * PI), -0.07+0.85f*cos(-20.0f/180 * PI),0.0f);	

	//	glVertex3f(0.0f + 0.8*sin(-45.0f/180 * PI), -0.07+0.8f*cos(-45.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(-45.0f/180 * PI), -0.07+0.85f*cos(-45.0f/180 * PI),0.0f);	


	//	glVertex3f(0.0f + 0.8*sin(10.0f/180 * PI), -0.07+0.8f*cos(10.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(10.0f/180 * PI), -0.07+0.85f*cos(10.0f/180 * PI),0.0f);	

	//	glVertex3f(0.0f + 0.8*sin(20.0f/180 * PI), -0.07+0.8f*cos(20.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(20.0f/180 * PI), -0.07+0.85f*cos(20.0f/180 * PI),0.0f);	

	//	glVertex3f(0.0f + 0.8*sin(45.0f/180 * PI), -0.07+0.8f*cos(45.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(45.0f/180 * PI), -0.07+0.85f*cos(45.0f/180 * PI),0.0f);	*/
	//	
	//glEnd();
	glLineWidth(1);
	glBegin(GL_LINES);
		glColor3f(1.0f,1.0f,1.0f);

		for(int i = 0; i < 36; i++)
		{
			glVertex3f(0.0f + 0.8*sin(i*10.0f/180 * PI), 0.8f*cos(i*10.0f/180 * PI),0.0f);		
			glVertex3f(0.0f + 0.83*sin(i*10.0f/180 * PI), 0.83f*cos(i*10.0f/180 * PI),0.0f);	
		}
		//glVertex3f(0.0f + 0.8*sin(-10.0f/180 * PI), 0.8f*cos(-10.0f/180 * PI),0.0f);		
		//glVertex3f(0.0f + 0.85*sin(-10.0f/180 * PI), 0.85f*cos(-10.0f/180 * PI),0.0f);	

		//glVertex3f(0.0f + 0.8*sin(-20.0f/180 * PI), 0.8f*cos(-20.0f/180 * PI),0.0f);		
		//glVertex3f(0.0f + 0.85*sin(-20.0f/180 * PI), 0.85f*cos(-20.0f/180 * PI),0.0f);	

		//glVertex3f(0.0f + 0.8*sin(-45.0f/180 * PI), 0.8f*cos(-45.0f/180 * PI),0.0f);		
		//glVertex3f(0.0f + 0.85*sin(-45.0f/180 * PI), 0.85f*cos(-45.0f/180 * PI),0.0f);	


		//glVertex3f(0.0f + 0.8*sin(10.0f/180 * PI), 0.8f*cos(10.0f/180 * PI),0.0f);		
		//glVertex3f(0.0f + 0.85*sin(10.0f/180 * PI), 0.85f*cos(10.0f/180 * PI),0.0f);	

		//glVertex3f(0.0f + 0.8*sin(20.0f/180 * PI), 0.8f*cos(20.0f/180 * PI),0.0f);		
		//glVertex3f(0.0f + 0.85*sin(20.0f/180 * PI), 0.85f*cos(20.0f/180 * PI),0.0f);	

		//glVertex3f(0.0f + 0.8*sin(45.0f/180 * PI), 0.8f*cos(45.0f/180 * PI),0.0f);		
		//glVertex3f(0.0f + 0.85*sin(45.0f/180 * PI), 0.85f*cos(45.0f/180 * PI),0.0f);	
		//
	glEnd();

	glLineWidth(2);
	//ROLL Tick Major
	glBegin(GL_LINES);
		glColor3f(1.0f,1.0f,1.0f);
		for(int i = 0; i < 12; i++)
		{
			glVertex3f(0.0f + 0.8*sin(i*30.0f/180 * PI), 0.8f*cos(i*30.0f/180 * PI),0.0f);		
			glVertex3f(0.0f + 0.86*sin(i*30.0f/180 * PI), 0.86f*cos(i*30.0f/180 * PI),0.0f);	
		}
				
/*		glVertex3f(0.0f + 0.8*sin(-30.0f/180 * PI), 0.8f*cos(-30.0f/180 * PI),0.0f);		
		glVertex3f(0.0f + 0.9*sin(-30.0f/180 * PI), 0.9f*cos(-30.0f/180 * PI),0.0f);	

		glVertex3f(0.0f + 0.8*sin(-60.0f/180 * PI), 0.8f*cos(-60.0f/180 * PI),0.0f);		
		glVertex3f(0.0f + 0.9*sin(-60.0f/180 * PI), 0.9f*cos(-60.0f/180 * PI),0.0f);	

		glVertex3f(0.0f + 0.8*sin(30.0f/180 * PI), 0.8f*cos(30.0f/180 * PI),0.0f);		
		glVertex3f(0.0f + 0.9*sin(30.0f/180 * PI), 0.9f*cos(30.0f/180 * PI),0.0f);	

		glVertex3f(0.0f + 0.8*sin(60.0f/180 * PI), 0.8f*cos(60.0f/180 * PI),0.0f);		
		glVertex3f(0.0f + 0.9*sin(60.0f/180 * PI), 0.9f*cos(60.0f/180 * PI),0.0f);*/	
	glEnd();

}


void  COpenGLControl::DrawPlot()
{
	int iNum = 0;
	float fFont;
	int iWidth;
	int ibNg = 0;

	
	glColor3f(0.0f,1.0f,0.0f);
	glLineWidth(1);
	//SMglArc(0.0f, -0.07f, 0.8, 300, 120,  50 );
	//SMglArc(0.0f, 0.00f, 0.4, 300, 360,  100 );

	//SMglArc(0.0f, 0.00f, 0.8, 300, 360,  150 );

#if 1
	glPushMatrix();
	glLineWidth(1);
	//if(m_CheckMode != __MODE_RUN)
	//{
	//	glColor3f(1.0f,1.0f,0.0f);
	//}
	glBegin(GL_LINES);
	for(int i = 0; i < 990; i++)
	{	
		//if(m_CheckMode == __MODE_RUN)
		//{
		//	if(m_PlotData[i].bNG != ibNg)
		//	{
		//		ibNg = m_PlotData[i].bNG;
		//	
		//		if(m_PlotData[i].bNG)
		//		{
		//			glColor3f(1.0f,0.0f,0.0f);
		//		}
		//		else
		//		{
		//			glColor3f(0.0f,1.0f,0.0f);
		//		}
		//	}		
		//}
		
		
		glVertex3f(i*0.001 - 1.0 , m_PlotDataL[i].Data, 0.0f);		
		glVertex3f((i+1)*0.001 - 1.0,m_PlotDataL[i+1].Data, 0.0f);	

		glVertex3f(i*0.001 + 0.01, m_PlotDataR[i].Data, 0.0f);
		glVertex3f((i + 1)*0.001 + 0.01, m_PlotDataR[i + 1].Data, 0.0f);


	}
	glEnd();
	glPopMatrix();

	//glLineWidth(1);

#endif
	


	
}

void  COpenGLControl::DrawTotalPlot(int DispalyID)
{
	int iNum = 0;
	float fFont;
	int iWidth;
	int ibNg = 0;

	
	glColor3f(0.0f,1.0f,0.0f);
	glLineWidth(1);
	//SMglArc(0.0f, -0.07f, 0.8, 300, 120,  50 );
	//SMglArc(0.0f, 0.00f, 0.4, 300, 360,  100 );

	//SMglArc(0.0f, 0.00f, 0.8, 300, 360,  150 );

#if 1
	glPushMatrix();
	glLineWidth(1);
	//if(m_CheckMode != __MODE_RUN)
	//{
	//	glColor3f(1.0f,1.0f,0.0f);
	//}
	glBegin(GL_LINES);
	for(int i = 0; i < 500; i++)
	{	
		glVertex3f(i*0.004 - 1.0 , m_PlotDataL[i].Data/2.5+0.4, 0.0f);		
		glVertex3f((i+1)*0.004 - 1.0,m_PlotDataL[i+1].Data / 2.5 + 0.4, 0.0f);

		glVertex3f(i*0.004 - 1.0, m_PlotDataR[i].Data / 2.5 - 0.6, 0.0f);
		glVertex3f((i + 1)*0.004 - 1.0, m_PlotDataR[i + 1].Data / 2.5 - 0.6, 0.0f);

	}
	glEnd();
	glPopMatrix();

	glLineWidth(1);
//	glBegin(GL_LINES);
//		glColor3f(1.0f,1.0f,1.0f);
//
//		for(int i = 0; i < 36; i++)
//		{
//			glVertex3f(0.0f + 0.8*sin(i*10.0f/180 * PI), 0.8f*cos(i*10.0f/180 * PI),0.0f);		
//			glVertex3f(0.0f + 0.83*sin(i*10.0f/180 * PI), 0.83f*cos(i*10.0f/180 * PI),0.0f);	
//		}
//		//glVertex3f(0.0f + 0.8*sin(-10.0f/180 * PI), 0.8f*cos(-10.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(-10.0f/180 * PI), 0.85f*cos(-10.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(-20.0f/180 * PI), 0.8f*cos(-20.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(-20.0f/180 * PI), 0.85f*cos(-20.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(-45.0f/180 * PI), 0.8f*cos(-45.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(-45.0f/180 * PI), 0.85f*cos(-45.0f/180 * PI),0.0f);	
//
//
//		//glVertex3f(0.0f + 0.8*sin(10.0f/180 * PI), 0.8f*cos(10.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(10.0f/180 * PI), 0.85f*cos(10.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(20.0f/180 * PI), 0.8f*cos(20.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(20.0f/180 * PI), 0.85f*cos(20.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(45.0f/180 * PI), 0.8f*cos(45.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(45.0f/180 * PI), 0.85f*cos(45.0f/180 * PI),0.0f);	
//		//
//	glEnd();
#endif
	

	////VOLT INPUT
	/////////////////////////////////////////////////////////////
	//if(m_bDisplayVoltEnable == 1)
	//{
	//	glPushMatrix();

	//	//	float	 m_fDeviationVoltX;
	//		//float	 m_fDeviationVoltY;
	//		//float	 m_bDisplayVoltOK;
	//		//float	 m_fDirVoltZ;
	//	glLoadIdentity();
	//	//glEnable(GL_POINT_SMOOTH) ;
	//	//glEnable(GL_LINE_SMOOTH) ;
	//	//glEnable(GL_POLYGON_SMOOTH) ;
	//
	//	if(m_bDisplayVoltOK == 1)
	//		glColor3f(0.0f,1.0f,0.0f);
	//	else
	//		glColor3f(0.0f,0.0f,0.0f);
	//	glLineWidth(22);

	//	glTranslatef(0, 0, -0.001f);
	//	SMglArc(m_fDeviationVoltX, m_fDeviationVoltY, 0.55, 300, 360,  150 );
	//	//SMglCircle(m_fDeviationVoltX, m_fDeviationVoltY, 0.08, 0, 360,  50 );

	//	if(m_bDisplayVoltOK == 1)
	//	{
	//	}
	//	else
	//	{
	//		//ROLL TriAngle Rolling
	//		glLineWidth(1);

	//		glRotatef(m_fDirVoltZ,0.0f,0.0f,1.0f);
	//		glBegin(GL_TRIANGLES);
	//			glVertex3f(0.0f,0.8f,0.0f);		
	//			glVertex3f(0.07f,0.7f,0.0f);		
	//			glVertex3f(-0.07f,0.7f,0.0f);
	//		glEnd();

	//		glLineWidth(1);

	//		glBegin(GL_LINES);
	//	
	//			glVertex3f(0.0f,0.8f,0.0f);		
	//			glVertex3f(0.07f,0.7f,0.0f);	

	//			glVertex3f(0.07f,0.7f,0.0f);		
	//			glVertex3f(-0.07f,0.7f,0.0f);

	//			glVertex3f(-0.07f,0.7f,0.0f);
	//			glVertex3f(0.0f,0.8f,0.0f);		
	//		glEnd();
	//	}

	//	glPopMatrix();

	//}
	//else
	//{

	//	glPushMatrix();

	//	//	float	 m_fDeviationVoltX;
	//		//float	 m_fDeviationVoltY;
	//		//float	 m_bDisplayVoltOK;
	//		//float	 m_fDirVoltZ;
	//	glLoadIdentity();
	//	//glEnable(GL_POINT_SMOOTH) ;
	//	//glEnable(GL_LINE_SMOOTH) ;
	//	//glEnable(GL_POLYGON_SMOOTH) ;
	//
	//	
	//	
	//	glColor3f(0.0f,0.0f,0.0f);
	//	glLineWidth(22);

	//	glTranslatef(0, 0, -0.001f);
	//	SMglArc(0, 0, 0.55, 300, 360,  150 );

	//	glPopMatrix();
	//}

	////////////////////////////////////
	//glPushMatrix();


	//glLoadIdentity();
	////glEnable(GL_POINT_SMOOTH) ;
	////glEnable(GL_LINE_SMOOTH) ;
	////glEnable(GL_POLYGON_SMOOTH) ;
	//
	//if(m_bDisplayOK)
	//	glColor3f(0.0f,1.0f,0.0f);
	//else
	//	glColor3f(1.0f,0.0f,0.0f);
	//glLineWidth(6);

	//glTranslatef(0, 0, -0.001f);
	//SMglArc(m_fDeviationX, m_fDeviationY, 0.38, 300, 360,  150 );
	////SMglCircle(m_fDeviationX, m_fDeviationY, 0.08, 0, 360,  50 );

	//if(m_bDisplayOK)
	//{
	//}
	//else
	//{
	//	//ROLL TriAngle Rolling
	//	glLineWidth(1);

	//	glRotatef(m_fRotZ,0.0f,0.0f,1.0f);
	//	glBegin(GL_TRIANGLES);
	//		glVertex3f(0.0f,0.8f,0.0f);		
	//		glVertex3f(0.07f,0.7f,0.0f);		
	//		glVertex3f(-0.07f,0.7f,0.0f);
	//	glEnd();

	//	glLineWidth(1);

	//	glBegin(GL_LINES);
	//	
	//		glVertex3f(0.0f,0.8f,0.0f);		
	//		glVertex3f(0.07f,0.7f,0.0f);	

	//		glVertex3f(0.07f,0.7f,0.0f);		
	//		glVertex3f(-0.07f,0.7f,0.0f);

	//		glVertex3f(-0.07f,0.7f,0.0f);
	//		glVertex3f(0.0f,0.8f,0.0f);		
	//	glEnd();
	//}

	//glPopMatrix();



	//ROLL TriAngle fix
	
	//glBegin(GL_TRIANGLES);
	//	glColor3f(1.0f,1.0f,1.0f);
	//	glVertex3f(0.0f,0.8f,0.0f);		
	//	glVertex3f(0.07f,0.9f,0.0f);		
	//	glVertex3f(-0.07f,0.9f,0.0f);
	//glEnd();
	//glBegin(GL_LINES);
	//	
	//	glVertex3f(0.0f,0.8f,0.0f);		
	//	glVertex3f(0.07f,0.9f,0.0f);	

	//	glVertex3f(0.07f,0.9f,0.0f);		
	//	glVertex3f(-0.07f,0.9f,0.0f);

	//	glVertex3f(-0.07f,0.9f,0.0f);
	//	glVertex3f(0.0f,0.8f,0.0f);	
	//glEnd();
	
	//ROLL Tick Minor
	//glBegin(GL_QUADS);
	//	glColor3f(1.0f,1.0f,1.0f);

	//	glVertex3f(0.0f + 0.8*sin(-10.0f/180 * PI), -0.07+0.8f*cos(-10.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(-10.0f/180 * PI), -0.07+0.85f*cos(-10.0f/180 * PI),0.0f);	
	//	glVertex3f(0.0f + 0.85*sin(-10.0f/180 * PI) + 0.02*cos(-10.0f/180 * PI), -0.07+0.85f*cos(-10.0f/180 * PI)- 0.02*sin(-10.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.8*sin(-10.0f/180 * PI) + 0.02*cos(-10.0f/180 * PI), -0.07+0.8f*cos(-10.0f/180 * PI)- 0.02*sin(-10.0f/180 * PI),0.0f);		

	///*	glVertex3f(0.0f + 0.8*sin(-20.0f/180 * PI), -0.07+0.8f*cos(-20.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(-20.0f/180 * PI), -0.07+0.85f*cos(-20.0f/180 * PI),0.0f);	

	//	glVertex3f(0.0f + 0.8*sin(-45.0f/180 * PI), -0.07+0.8f*cos(-45.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(-45.0f/180 * PI), -0.07+0.85f*cos(-45.0f/180 * PI),0.0f);	


	//	glVertex3f(0.0f + 0.8*sin(10.0f/180 * PI), -0.07+0.8f*cos(10.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(10.0f/180 * PI), -0.07+0.85f*cos(10.0f/180 * PI),0.0f);	

	//	glVertex3f(0.0f + 0.8*sin(20.0f/180 * PI), -0.07+0.8f*cos(20.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(20.0f/180 * PI), -0.07+0.85f*cos(20.0f/180 * PI),0.0f);	

	//	glVertex3f(0.0f + 0.8*sin(45.0f/180 * PI), -0.07+0.8f*cos(45.0f/180 * PI),0.0f);		
	//	glVertex3f(0.0f + 0.85*sin(45.0f/180 * PI), -0.07+0.85f*cos(45.0f/180 * PI),0.0f);	*/
	//	
	//glEnd();
//	glLineWidth(1);
//	glBegin(GL_LINES);
//		glColor3f(1.0f,1.0f,1.0f);
//
//		for(int i = 0; i < 36; i++)
//		{
//			glVertex3f(0.0f + 0.8*sin(i*10.0f/180 * PI), 0.8f*cos(i*10.0f/180 * PI),0.0f);		
//			glVertex3f(0.0f + 0.83*sin(i*10.0f/180 * PI), 0.83f*cos(i*10.0f/180 * PI),0.0f);	
//		}
//		//glVertex3f(0.0f + 0.8*sin(-10.0f/180 * PI), 0.8f*cos(-10.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(-10.0f/180 * PI), 0.85f*cos(-10.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(-20.0f/180 * PI), 0.8f*cos(-20.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(-20.0f/180 * PI), 0.85f*cos(-20.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(-45.0f/180 * PI), 0.8f*cos(-45.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(-45.0f/180 * PI), 0.85f*cos(-45.0f/180 * PI),0.0f);	
//
//
//		//glVertex3f(0.0f + 0.8*sin(10.0f/180 * PI), 0.8f*cos(10.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(10.0f/180 * PI), 0.85f*cos(10.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(20.0f/180 * PI), 0.8f*cos(20.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(20.0f/180 * PI), 0.85f*cos(20.0f/180 * PI),0.0f);	
//
//		//glVertex3f(0.0f + 0.8*sin(45.0f/180 * PI), 0.8f*cos(45.0f/180 * PI),0.0f);		
//		//glVertex3f(0.0f + 0.85*sin(45.0f/180 * PI), 0.85f*cos(45.0f/180 * PI),0.0f);	
//		//
//	glEnd();
//
//	glLineWidth(2);
//	//ROLL Tick Major
//	glBegin(GL_LINES);
//		glColor3f(1.0f,1.0f,1.0f);
//		for(int i = 0; i < 12; i++)
//		{
//			glVertex3f(0.0f + 0.8*sin(i*30.0f/180 * PI), 0.8f*cos(i*30.0f/180 * PI),0.0f);		
//			glVertex3f(0.0f + 0.86*sin(i*30.0f/180 * PI), 0.86f*cos(i*30.0f/180 * PI),0.0f);	
//		}
//				
///*		glVertex3f(0.0f + 0.8*sin(-30.0f/180 * PI), 0.8f*cos(-30.0f/180 * PI),0.0f);		
//		glVertex3f(0.0f + 0.9*sin(-30.0f/180 * PI), 0.9f*cos(-30.0f/180 * PI),0.0f);	
//
//		glVertex3f(0.0f + 0.8*sin(-60.0f/180 * PI), 0.8f*cos(-60.0f/180 * PI),0.0f);		
//		glVertex3f(0.0f + 0.9*sin(-60.0f/180 * PI), 0.9f*cos(-60.0f/180 * PI),0.0f);	
//
//		glVertex3f(0.0f + 0.8*sin(30.0f/180 * PI), 0.8f*cos(30.0f/180 * PI),0.0f);		
//		glVertex3f(0.0f + 0.9*sin(30.0f/180 * PI), 0.9f*cos(30.0f/180 * PI),0.0f);	
//
//		glVertex3f(0.0f + 0.8*sin(60.0f/180 * PI), 0.8f*cos(60.0f/180 * PI),0.0f);		
//		glVertex3f(0.0f + 0.9*sin(60.0f/180 * PI), 0.9f*cos(60.0f/180 * PI),0.0f);*/	
//	glEnd();
//
}

void  COpenGLControl::DrawBaseGround()
{
	glPushMatrix();

	//	glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
		if(m_fPosY > 0.9)
		{
			glTranslatef(0, 0.9, 0.0f);
			
		}
		else if(m_fPosY < -0.9)
		{
			
			glTranslatef(0, -0.9, 0.0f);
			
		}
		else
		{
			
			glTranslatef(0, m_fPosY, 0.0f);

		}
	

		//SKY
		glBegin(GL_POLYGON);
			glColor3f(0.0f,0.0f,0.0f);
			//glColor3f(0.0f,0.3f,0.7f);
			glVertex3f(-2.0f,10.0f,0.0f);		
			glVertex3f(1.1f,10.0f,0.0f);
		
			glVertex3f(1.1f,-10.0f,0.0f);
			glVertex3f(-2.0f,-10.0f,0.0f);		
		glEnd();

		glBegin(GL_POLYGON);
		glColor3f(1.0f, 1.0f, 1.0f);
		//glColor3f(0.0f,0.3f,0.7f);
		glVertex3f(-0.01f, 10.0f, 0.0f);
		glVertex3f(0.01f, 10.0f, 0.0f);

		glVertex3f(0.01f, -10.0f, 0.0f);
		glVertex3f(-0.01f, -10.0f, 0.0f);
		glEnd();

		////GROUND
		//glBegin(GL_POLYGON);
		//	glColor3f(0.60f,0.2f,0.0f);
		//	glVertex3f(-2.0f,0.0f,0.0f);		
		//	glVertex3f(2.0f,0.0f,0.0f);
		//
		//	glVertex3f(2.0f,-10.0f,0.0f);
		//	glVertex3f(-2.0f,-10.0f,0.0f);	
		//glEnd();
		glPopMatrix();


		glPushMatrix();

		
		glBegin(GL_LINES);

			glColor3f(0.5f, 0.5f, 0.5f);
		//	glVertex3f(-1.0f,-1.0f,0.0f);	
		//	glVertex3f(1.0f,-1.0f,0.0f);
			for(int i = 0 ; i < 4; i++)
			{
				glVertex3f(-1.0f,-1.0f+(0.5)*i,0.0f);		
				glVertex3f(-0.01f,-1.0f+0.5*i,0.0f);

				glVertex3f(0.01f,-1.0f+(0.5)*i,0.0f);
				glVertex3f(1.01f,-1.0f+0.5*i,0.0f);
			}								
		
			for(int i = 0 ; i < 10; i++)
			{
				glVertex3f(-1.0f+0.099*i,-1.0f,0.0f);		
				glVertex3f(-1.0f+0.099*i,1.0f,0.0f);

				glVertex3f(0.01f +0.099*i,-1.0f,0.0f);
				glVertex3f(0.01f +0.099*i,1.0f,0.0f);
			}			
			

		glEnd();	

	glPopMatrix();
}

void  COpenGLControl::DrawTotalBaseGround()
{
	glPushMatrix();

	//	glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
		if(m_fPosY > 0.9)
		{
			glTranslatef(0, 0.9, 0.0f);
			
		}
		else if(m_fPosY < -0.9)
		{
			
			glTranslatef(0, -0.9, 0.0f);
			
		}
		else
		{
			
			glTranslatef(0, m_fPosY, 0.0f);

		}
	

		//SKY
		glBegin(GL_POLYGON);
			glColor3f(0.0f,0.0f,0.0f);
			//glColor3f(0.0f,0.3f,0.7f);
			glVertex3f(-2.0f,10.0f,0.0f);		
			glVertex3f(1.1f,10.0f,0.0f);
		
			glVertex3f(1.1f,-10.0f,0.0f);
			glVertex3f(-2.0f,-10.0f,0.0f);		
		glEnd();

		//glBegin(GL_POLYGON);
		//glColor3f(1.0f, 1.0f, 1.0f);
		////glColor3f(0.0f,0.3f,0.7f);
		//glVertex3f(-0.01f, 10.0f, 0.0f);
		//glVertex3f(0.01f, 10.0f, 0.0f);

		//glVertex3f(0.01f, -10.0f, 0.0f);
		//glVertex3f(-0.01f, -10.0f, 0.0f);
		//glEnd();

		glPopMatrix();


		glPushMatrix();

		
		glBegin(GL_LINES);

			glColor3f(0.5f, 0.5f, 0.5f);
		
			for(int i = 0 ; i < 4; i++)
			{
				glVertex3f(-1.0f,-1.0f+(0.2)*i,0.0f);		
				glVertex3f(1.01f,-1.0f+0.2*i,0.0f);

				glVertex3f(-1.0f,0.0f+(0.2)*i,0.0f);
				glVertex3f(1.01f,0.0f+0.2*i,0.0f);
			}								
		
			for(int i = 0 ; i < 10; i++)
			{
				glVertex3f(-1.0f+0.2*i,-1.0f,0.0f);		
				glVertex3f(-1.0f+0.2*i,-0.2f,0.0f);

				glVertex3f(-1.0f + 0.2*i,0.0f,0.0f);
				glVertex3f(-1.0f + 0.2*i,0.8f,0.0f);
			}			
			

		glEnd();	

	glPopMatrix();
}


//void  COpenGLControl::DrawTotalBaseGround()
//{
//	glPushMatrix();
//
//	//	glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//		if(m_fPosY > 0.9)
//		{
//			glTranslatef(0, 0.9, 0.0f);
//			
//		}
//		else if(m_fPosY < -0.9)
//		{
//			
//			glTranslatef(0, -0.9, 0.0f);
//			
//		}
//		else
//		{
//			
//			glTranslatef(0, m_fPosY, 0.0f);
//
//		}
//	
//
//		//SKY
//		glBegin(GL_POLYGON);
//			glColor3f(0.0f,0.0f,0.0f);
//			//glColor3f(0.0f,0.3f,0.7f);
//			glVertex3f(-2.0f,10.0f,0.0f);		
//			glVertex3f(1.1f,10.0f,0.0f);
//		
//			glVertex3f(1.1f,-10.0f,0.0f);
//			glVertex3f(-2.0f,-10.0f,0.0f);		
//		glEnd();
//
//		glBegin(GL_POLYGON);
//		glColor3f(1.0f, 1.0f, 1.0f);
//		//glColor3f(0.0f,0.3f,0.7f);
//		glVertex3f(-0.01f, 10.0f, 0.0f);
//		glVertex3f(0.01f, 10.0f, 0.0f);
//
//		glVertex3f(0.01f, -10.0f, 0.0f);
//		glVertex3f(-0.01f, -10.0f, 0.0f);
//		glEnd();
//
//		////GROUND
//		//glBegin(GL_POLYGON);
//		//	glColor3f(0.60f,0.2f,0.0f);
//		//	glVertex3f(-2.0f,0.0f,0.0f);		
//		//	glVertex3f(2.0f,0.0f,0.0f);
//		//
//		//	glVertex3f(2.0f,-10.0f,0.0f);
//		//	glVertex3f(-2.0f,-10.0f,0.0f);	
//		//glEnd();
//		glPopMatrix();
//
//
//		glPushMatrix();
//
//		
//		glBegin(GL_LINES);
//
//			glColor3f(0.5f, 0.5f, 0.5f);
//		//	glVertex3f(-1.0f,-1.0f,0.0f);	
//		//	glVertex3f(1.0f,-1.0f,0.0f);
//			for(int i = 0 ; i < 4; i++)
//			{
//				glVertex3f(-1.0f,-1.0f+(0.5)*i,0.0f);		
//				glVertex3f(-0.01f,-1.0f+0.5*i,0.0f);
//
//				glVertex3f(0.01f,-1.0f+(0.5)*i,0.0f);
//				glVertex3f(1.01f,-1.0f+0.5*i,0.0f);
//			}								
//		
//			for(int i = 0 ; i < 10; i++)
//			{
//				glVertex3f(-1.0f+0.099*i,-1.0f,0.0f);		
//				glVertex3f(-1.0f+0.099*i,1.0f,0.0f);
//
//				glVertex3f(0.01f +0.099*i,-1.0f,0.0f);
//				glVertex3f(0.01f +0.099*i,1.0f,0.0f);
//			}			
//			
//
//		glEnd();	
//
//	glPopMatrix();
//}

//void  COpenGLControl::DrawSkyGround()
//{
//	glPushMatrix();
//
//		glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//		if(m_fPosY > 0.9)
//		{
//			glTranslatef(0, 0.9, 0.0f);
//			
//		}
//		else if(m_fPosY < -0.9)
//		{
//			
//			glTranslatef(0, -0.9, 0.0f);
//			
//		}
//		else
//		{
//			
//			glTranslatef(0, m_fPosY, 0.0f);
//
//		}
//	
//
//		//SKY
//		glBegin(GL_POLYGON);
//			glColor3f(0.0f,0.3f,0.7f);
//			glVertex3f(-2.0f,10.0f,0.0f);		
//			glVertex3f(2.0f,10.0f,0.0f);
//		
//			glVertex3f(2.0f,0.0f,0.0f);
//			glVertex3f(-2.0f,0.0f,0.0f);		
//		glEnd();
//
//		//GROUND
//		glBegin(GL_POLYGON);
//			glColor3f(0.60f,0.2f,0.0f);
//			glVertex3f(-2.0f,0.0f,0.0f);		
//			glVertex3f(2.0f,0.0f,0.0f);
//		
//			glVertex3f(2.0f,-10.0f,0.0f);
//			glVertex3f(-2.0f,-10.0f,0.0f);	
//		glEnd();
//		glPopMatrix();
//
//
//		glPushMatrix();
//
//		glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//		glTranslatef(0, m_fPosY, 0.0f);
//
//		//Horizental
//		glBegin(GL_POLYGON);
//			glColor3f(1.0f,1.0f,1.0f);
//			glVertex3f(-2.0f,0.01f,0.0f);		
//			glVertex3f(2.0f,0.01f,0.0f);
//		
//			glVertex3f(2.0f,-0.01f,0.0f);
//			glVertex3f(-2.0f,-0.01f,0.0f);	
//		glEnd();	
//		
//		glBegin(GL_LINES);
//			glColor3f(1.0f,1.0f,1.0f);
//			glVertex3f(-2.0f,0.01f,0.0f);		
//			glVertex3f(2.0f,0.01f,0.0f);
//		
//			glVertex3f(2.0f,-0.01f,0.0f);
//			glVertex3f(-2.0f,-0.01f,0.0f);	
//		glEnd();	
//
//	glPopMatrix();
//}

//void  COpenGLControl::DrawHeading()
//{
//
//
//	
//	
//	glBegin(GL_POLYGON);
//		glColor3f(0.5f,0.4f,0.2f);
//		glVertex3f(-1.0f,-1.2f,-0.0f);		
//		glVertex3f(1.0f,-1.2f,-0.0f);
//		
//		glVertex3f(1.0f,-1.0f,-0.0f);
//		glVertex3f(-1.0f,-1.0f,-0.0f);		
//	glEnd();
//
//	
//	SMglArc(0.0f, -1.2f, 0.4, 0, 360,  100 );
//	/*glBegin(GL_POLYGON);
//		glColor3f(1.0f,1.0f,1.0f);
//		glVertex3f(-1.0f,-1.2f,-0.1f);		
//		glVertex3f(1.0f,-1.2f,-0.1f);
//		glVertex3f(1.0f,-1.19f,-0.1f);
//		glVertex3f(-1.0f,-1.19f,-0.1f);
//	glEnd();*/
//
//	glBegin(GL_POLYGON);
//		glColor3f(1.0f,1.0f,1.0f);
//		glVertex3f(-1.0f,-1.0f,-0.0f);		
//		glVertex3f(1.0f,-1.0f,-0.0f);
//		glVertex3f(1.0f,-1.01f,-0.0f);
//		glVertex3f(-1.0f,-1.01f,-0.0f);
//
//	glEnd();
//
//	glPushMatrix();
//	glColor3f(1.0f,1.0f,0.0f);
//		glBegin(GL_LINES);
//			glVertex3f(0.04f, -0.95f, 0.0f);		
//			glVertex3f(0.00f, -1.0f, 0.0f);
//
//			glVertex3f(0.00f, -1.0f, 0.0f);		
//			glVertex3f(-0.04f, -0.95f, 0.0f);
//
//			glVertex3f(0.04f, -0.95f, 0.0f);		
//			glVertex3f(-0.04f, -0.95f, 0.0f);
//		glEnd();
//		glBegin(GL_TRIANGLES);
//		
//			glVertex3f(0.0f, -1.0f, 0.0f);		
//			glVertex3f(0.04f, -0.95f, 0.0f);		
//			glVertex3f(-0.04f, -0.95f, 0.0f);
//		glEnd();
//	glPopMatrix();
//
//	int i;
//	int Heading = m_fPosX * 100;
//
//	Heading = Heading / 5 * 5; 
//
//	
//	//Pitch Ladder
//	glPushMatrix();
//		
//
//		glColor3f(1.0f,1.0f,1.0f);
//		//glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//		glTranslatef(-m_fPosX, 0.0f, 0.0f);
//		
//		for(i = Heading-120; i < Heading+120; i += 5)
//		{
//			int lhead = i;
//			if(lhead < 0)
//			{
//				lhead += 360;
//			}
//			if(lhead >= 360)
//			{
//				lhead -= 360;
//			}
//
//			if((i%30) == 0)
//			{
//				glBegin(GL_LINES);
//					glVertex3f(i/100.0f, -1.0f, 0.0f);		
//					glVertex3f(i/100.0f, -1.08f, 0.0f);
//				glEnd();
//				
//				if(lhead == 0)
//				{
//					SMglChar('N', i/100.0f, -1.15, 0.08, 1);
//				}
//				else if(lhead == 90)
//				{
//					SMglChar('E', i/100.0f, -1.15, 0.08, 1);
//				}
//				else if(lhead == 180)
//				{
//					SMglChar('S', i/100.0f, -1.15, 0.08, 1);
//				}
//				else if(lhead == 270)
//				{
//					SMglChar('W', i/100.0f, -1.15, 0.08, 1);
//				}
//				else
//				{
//					if(lhead > 100)
//					{
//						SMglNumber(int(abs(lhead/100)), i/100.0f-0.03f, -1.15, 0.06, 1);
//						SMglNumber(int(abs(lhead%100)/10), i/100.0f+0.03f, -1.15, 0.06, 1);
//					}
//					else
//					{
//						SMglNumber(int(abs(lhead/10)), i/100.0f, -1.15, 0.06, 1);
//					}
//				}
//
//			}
//			else if((i%10) == 0)
//			{
//				glPushMatrix();
//					glLineWidth(2);
//					glBegin(GL_LINES);
//						glVertex3f(i/100.0f, -1.0f, 0.0f);		
//						glVertex3f(i/100.0f, -1.08f, 0.0f);
//					glEnd();
//				glPopMatrix();
//			}
//			else 
//			{
//				glBegin(GL_LINES);
//					glVertex3f(i/100.0f, -1.0f, 0.0f);		
//					glVertex3f(i/100.0f, -1.05f, 0.0f);
//				glEnd();
//			}
//
//
//		}
//
//
//		
//			
//	glPopMatrix();
//
////
////glBegin(GL_POLYGON);
////		glVertex3f(-1.0f,-1.2f,-0.1f);
////		glVertex3f(-1.0f,-1.0f,-0.1f);
////		glVertex3f(-0.99f,-1.0f,-0.1f);
////		glVertex3f(-0.99f,-1.2f,-0.1f);
////glEnd();	
////
////glBegin(GL_POLYGON);
////		glVertex3f(1.0f,-1.2f,-0.1f);
////		glVertex3f(1.0f,-1.0f,-0.1f);
////		glVertex3f(0.99f,-1.0f,-0.1f);
////		glVertex3f(0.99f,-1.2f,-0.1f);
////glEnd();	
////
////	
////	glEnd();
//}

//void  COpenGLControl::DrawOutLine()
//{
//
//	//glBegin(GL_TRIANGLES);
//	//	glColor3f(0.5f,0.5f,0.5f);
//	//	glVertex3f(-1.0f,0.9f,-0.1f);		
//	//	glVertex3f(-1.0f,1.0f,-0.1f);
//	//	glVertex3f(-0.9f,1.0f,-0.1f);
//	//	
//	//	glVertex3f(1.0f,0.9f,-0.1f);		
//	//	glVertex3f(1.0f,1.0f,-0.1f);
//	//	glVertex3f(0.9f,1.0f,-0.1f);
//	//		
//	//	
//	//	//glColor3f(0.2f,0.2f,0.2f);
//	//	glVertex3f(-1.0f,-0.9f,-0.1f);		
//	//	glVertex3f(-1.0f,-1.0f,-0.1f);
//	//	glVertex3f(-0.9f,-1.0f,-0.1f);
//	//	
//	//	glVertex3f(1.0f,-0.9f,-0.1f);		
//	//	glVertex3f(1.0f,-1.0f,-0.1f);
//	//	glVertex3f(0.9f,-1.0f,-0.1f);
//
//	//glEnd();	
//	glBegin(GL_LINE_STRIP);
//		glColor3f(0.5f,0.5f,0.5f);
//		glVertex3f(-1.0f,-1.2f,-0.1f);		
//		glVertex3f(1.0f,-1.2f,-0.1f);
//		
//		glVertex3f(1.0f,1.0f,-0.1f);
//		glVertex3f(-1.0f,1.0f,-0.1f);		
//		glVertex3f(-1.0f,-1.2f,-0.1f);		
//	glEnd();
//
//
//
//
//}
//
//void COpenGLControl::DrawWingShip()
//{
//	//Wing Ship Center
//	glBegin(GL_QUADS);
//		glColor3f(0.0f,0.0f,0.0f);
//		glVertex3f(-0.025f,0.025f,-0.1f);		
//		glVertex3f(0.025f,0.025f,-0.1f);
//		
//		glVertex3f(0.025f,-0.025f,-0.1f);
//		glVertex3f(-0.025f,-0.025f,-0.1f);	
//	glEnd();	
//	
//	glBegin(GL_LINE_LOOP);
//		glColor3f(1.0f,1.0f,1.0f);
//		glVertex3f(-0.025f,0.025f,0.0f);		
//		glVertex3f(0.025f,0.025f,0.0f);
//		
//		glVertex3f(0.025f,-0.025f,0.0f);
//		glVertex3f(-0.025f,-0.025f,0.0f);	
//	glEnd();
//
//	////Wing Ship Left
//	glBegin(GL_POLYGON);
//		glColor3f(0.0f,0.0f,0.0f);
//		glVertex3f(-0.65f,0.025f,0.0f);		
//		glVertex3f(-0.65f,-0.025f,0.0f);
//		glVertex3f(-0.25f,-0.025f,0.0f);
//		glVertex3f(-0.25f,0.025f,0.0f);		
//	glEnd();	
//
//	glBegin(GL_POLYGON);
//
//		glVertex3f(-0.30f,0.025f,0.0f);		
//		glVertex3f(-0.30f,-0.10f,0.0f);	
//		glVertex3f(-0.25f,-0.10f,0.0f);	
//		glVertex3f(-0.25f,0.025f,0.0f);	
//	glEnd();
//
//	glBegin(GL_LINE_LOOP);
//		glColor3f(1.0f,1.0f,1.0f);
//		glVertex3f(-0.65f,0.025f,0.0f);		
//		glVertex3f(-0.65f,-0.025f,0.0f);
//		
//		glVertex3f(-0.30f,-0.025f,0.0f);
//		glVertex3f(-0.30f,-0.10f,0.0f);	
//		glVertex3f(-0.25f,-0.10f,0.0f);	
//		glVertex3f(-0.25f,0.025f,0.0f);	
//	glEnd();
//
//	////Wing Ship Right
//	glBegin(GL_POLYGON);
//		glColor3f(0.0f,0.0f,0.0f);
//		glVertex3f(0.65f,0.025f,0.0f);		
//		glVertex3f(0.65f,-0.025f,0.0f);
//		glVertex3f(0.25f,-0.025f,0.0f);
//		glVertex3f(0.25f,0.025f,0.0f);		
//	glEnd();	
//
//	glBegin(GL_POLYGON);
//
//		glVertex3f(0.30f,0.025f,0.0f);		
//		glVertex3f(0.30f,-0.10f,0.0f);	
//		glVertex3f(0.25f,-0.10f,0.0f);	
//		glVertex3f(0.25f,0.025f,0.0f);	
//	glEnd();
//
//	glBegin(GL_LINE_LOOP);
//		glColor3f(1.0f,1.0f,1.0f);
//		glVertex3f(0.65f,0.025f,0.0f);		
//		glVertex3f(0.65f,-0.025f,0.0f);
//		
//		glVertex3f(0.30f,-0.025f,0.0f);
//		glVertex3f(0.30f,-0.10f,0.0f);	
//		glVertex3f(0.25f,-0.10f,0.0f);	
//		glVertex3f(0.25f,0.025f,0.0f);	
//	glEnd();
//}
//
//void COpenGLControl::DrawPitchLadder()
//{
//	int i;
//	int Pitch = -m_fPosY * 25;
//
//	int lRoll = int(m_fRotZ) % 360; 
//
//	//Pitch Ladder
//	glPushMatrix();
//		
//
//		glColor3f(1.0f,1.0f,1.0f);
//		glRotatef(m_fRotZ, 0.0f, 0.0f, 1.0f);
//		glTranslatef(0, m_fPosY, 0.0f);
//		
//
//		if((abs(lRoll) < 30) || (abs(lRoll) > 330))
//		{
//			for(i = -90; i <= +90; i += 5)
//			{
//				if(i == 0)
//					continue;
//				if(abs(i - Pitch) > 30)
//				{
//					continue;
//				}
//
//				if(abs(i - Pitch) < 13)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//						
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//				else if(((i - Pitch) < 17) && ((i - Pitch) > 12))
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						/*SMglNumber(int(abs(i%10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i%10)), 0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.40, i/5*0.2, 0.08, 1);*/
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//
//				}
//				else if((i - Pitch) < 0)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//
//			}
//			
//
//		}	
//		else if((abs(lRoll) < 90) || (abs(lRoll) > 270))
//		{
//			for(i = -90; i <= +90; i += 5)
//			{
//				if(i == 0)
//					continue;
//				if(abs(i - Pitch) < 13)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//				else if(((i - Pitch) < 17) && ((i - Pitch) > 12))
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						if(((lRoll > 0)&&(lRoll < 90))||(lRoll < -270))
//						{
//							SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//							SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						}
//						else
//						{
//							SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//							SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//						}
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//
//				}
//				else if((i - Pitch) < 0)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//
//			}
//		}
//		else if((abs(lRoll) > 150) && (abs(lRoll)  < 210))
//		{
//			for(i = -90; i <= 90; i += 5)
//			{
//				if(i == 0)
//					continue;
//				if(abs(i - Pitch) < 13)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//				else if(((i - Pitch) > -17) && ((i - Pitch) < -12))
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						/*SMglNumber(int(abs(i%10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i%10)), 0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.40, i/5*0.2, 0.08, 1);*/
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//
//				}
//				else if((i - Pitch) > 0)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//
//			}
//			
//
//		}	
//		else if((abs(lRoll) > 90) && (abs(lRoll)  < 270))
//		{
//			for(i = -90; i <= 90; i += 5)
//			{
//				if(i == 0)
//					continue;
//				if(abs(i - Pitch) < 13)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//				else if(((i - Pitch) > -17) && ((i - Pitch) < -12))
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						if(((lRoll < -90)&&(lRoll > -180))||((lRoll > 180)&&(lRoll < 270)))
//						{
//							SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//							SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//						}
//						else
//						{
//							SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//							SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//							
//						}
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//
//				}
//				else if((i - Pitch) > 0)
//				{
//					if(i%10 == 0)
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.35f, i/5*0.2, 0.0f);
//						glEnd();
//						SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//						SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//					}
//					else
//					{
//						glBegin(GL_LINES);
//							glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//							glVertex3f(+0.25f, i/5*0.2, 0.0f);
//						glEnd();
//					}
//				}
//
//			}
//		}
//		else 
//		{
//			for(i = -90; i <= 90; i += 5)
//			{
//				if(i == 0)
//					continue;
//				
//				if(i%10 == 0)
//				{
//					glBegin(GL_LINES);
//						glVertex3f(-0.35f, i/5*0.2, 0.0f);		
//						glVertex3f(+0.35f, i/5*0.2, 0.0f);
//					glEnd();
//					SMglNumber(int(abs(i/10)), -0.48, i/5*0.2, 0.08, 1);
//					SMglNumber(0, -0.40, i/5*0.2, 0.08, 1);
//					SMglNumber(int(abs(i/10)), 0.40, i/5*0.2, 0.08, 1);
//						SMglNumber(0, 0.48, i/5*0.2, 0.08, 1);
//
//				}
//				else
//				{
//					glBegin(GL_LINES);
//						glVertex3f(-0.25f, i/5*0.2, 0.0f);		
//						glVertex3f(+0.25f, i/5*0.2, 0.0f);
//					glEnd();
//				}				
//
//			}
//		}
//			
//	
//			
//	glPopMatrix();
//}
//
