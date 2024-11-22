#pragma once
#include "afxwin.h"

#include <gl/gl.h>
#include <gl/glu.h>
//#include <SOIL.h>

#define __MODE_IDLE		0
#define __MODE_RUN		1

#define __DISPLAY_PAGE_MAIN			0
#define __DISPLAY_PAGE_TOTAL_LEFT	1
#define __DISPLAY_PAGE_TOTAL_RIGHT	2

//typedef struct
//{
//	double Data;
//	int bNG;
//}PlotData_T;

class COpenGLControlSub : public CWnd
{
	public:
		/******************/
		/* Public Members */
		/******************/
		UINT_PTR m_unpTimer;
		// View information variables
		float	 m_fLastX;
		float	 m_fLastY;
		float	 m_fPosX;
		float	 m_fPosY;
		float	 m_fZoom;
		float	 m_fRotX;
		float	 m_fRotY;
		float	 m_fRotZ;
		float	 m_fDeviationX;
		float	 m_fDeviationY;
		float	 m_bDisplayOK;
		bool	 m_bIsMaximized;

		float	 m_fDeviationVoltX;
		float	 m_fDeviationVoltY;
		float	 m_bDisplayVoltOK;
		float	 m_fDirVoltZ;

		bool	m_bDisplayVoltEnable;

		void	Rotate();


		GLuint base; 
		GLvoid BuildFont(GLvoid); 
		GLvoid KillFont(GLvoid);
		GLvoid glPrint(const char *text) ;

	
		PlotData_T	m_PlotData[2048];
		PlotData_T	m_PlotDataL[2048];
		PlotData_T	m_PlotDataR[2048];

		int m_CheckMode;// = __MODE_IDLE;
		int m_DisplayPage;// = __DISPLAY_PAGE_MAIN;

		void	DrawGLScene();
		void	DrawISFD() ;
		void	SMglNumber(int lnumber,float lPosX , float lPosY , float lFontSize, int lwidth);
		void	SMglChar(char lAlpha,float lPosX , float lPosY , float lFontSize, int lwidth);
		void	SMglArc(float lCentX, float lCenty, float lRadius, float lStart, float lAngleSize,  int lStep );
		void	SMglCircle(float lCentX, float lCenty, float lRadius, float lStart, float lAngleSize,  int lStep );

		void  COpenGLControlSub::DrawBaseGround();
		void  COpenGLControlSub::DrawTotalBaseGround();
		void	DrawRollTick();
		void	DrawPlot();
		void  COpenGLControlSub::DrawTotalPlot(int DispalyID);
		//void	DrawSkyGround();
		//void	DrawHeading();
		//void	DrawOutLine();
		//void	DrawWingShip();
		//void	DrawPitchLadder();

	private:
		/*******************/
		/* Private Members */
		/*******************/
		// Window information
		CWnd  *hWnd;
		HDC   hdc;			
		HGLRC hrc;			
		int   m_nPixelFormat;
		CRect m_rect;
		CRect m_oldWindow;
		CRect m_originalRect;

	public:
		COpenGLControlSub(void);
		virtual ~COpenGLControlSub(void);

		void oglCreate(CRect rect, CWnd *parent, LPCTSTR lWindName);
		void oglInitialize(void);
		void oglDrawScene(void);
		void destroy(void);

		// Added message classes:
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg	void OnDraw(CDC *pDC);
		afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);

		DECLARE_MESSAGE_MAP()
};