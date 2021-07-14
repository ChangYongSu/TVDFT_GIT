
#pragma once


#include <opencv2\opencv.hpp>

#include "afxwin.h"

#include <allheaders.h>
#include <baseapi.h>
#include "afxcmn.h"
//#include "ping.h"


#pragma comment(lib, "liblept168")
#pragma comment(lib, "libtesseract302")

using namespace std;
using namespace cv;
using namespace tesseract;

#define __IMAGE_DEBUG__


// CGetImage 대화 상자입니다.

class CGetImage //: public CDialogEx
{
	//DECLARE_DYNAMIC(CGetImage)

public:
	CGetImage();   // 표준 생성자입니다.
	virtual ~CGetImage();

// 대화 상자 데이터입니다.

	double tres1;
	double tres2;
	
	int nErode;
	int nDilate;
	int mTres,mTresMax;//	
	int mX,mY;	
	int mXW,mYH;	
	int mR,mG,mB;

	int m_ocrX,m_ocrY;	
	int m_ocrXW,m_ocrYH;	
	int m_SelImage;

	////////////////////////////

	bool bProcess;

	
	TessBaseAPI api;

	Mat org_img;
	Mat gray_img;
	Mat dst_img;
	Mat bin_img;
	CStatic m_pic;
	
	CRect rect;
	CDC memDC;
	CBitmap Bitmap,*pOldBitmap;
	CDC *m_pDC;

	
	CStatic m_pic2;
	CRect rect2;
	CDC memDC2;
	CBitmap Bitmap2,*pOldBitmap2;
	CDC *m_pDC2;




	Mat src_Mat;
	Mat org_Mat;
	Mat gray_Mat;
	Mat edge_Mat;
	Mat edgeGray_Mat;
	Mat show_Mat;

	
	bool binv;
	
	// Filter type
	#define NONE 0  // no filter
	#define HARD 1  // hard shrinkage
	#define SOFT 2  // soft shrinkage
	#define GARROT 3  // garrot filter
	float sgn(float x);
	float soft_shrink(float d,float T);
	float hard_shrink(float d,float T);
	float Garrot_shrink(float d,float T);
	void cvHaarWavelet(Mat &src,Mat &dst,int NIter);
	void cvInvHaarWavelet(Mat &src,Mat &dst,int NIter, int SHRINKAGE_TYPE=0, float SHRINKAGE_T=50);
	
#define LOWER_PERCENT 0.05
#define UPPER_PERCENT 0.95
	void ContrastStretching(Mat &src, Mat &dst);

	int getim(Mat src, Mat *dst);




	vector<Mat> MatVector;
	Mat avgMat;
	//템플릿 매칭
	IplImage *TemplateImg;	
	Mat Template_Rslt;
	Mat Template_Arr[10];

	IplImage *TempCharImg;	

	void ProcessImage();
	void ProcessTemplate();
	
	void DisplayImage(IplImage* img);
	void DisplayImage2(IplImage* img);
	void DisplayImage(Mat img);
	void DisplayBitmap(CDC* pDC, CRect rect, IplImage* img);
	void ResizeImage(IplImage* src_img, IplImage** dst_img, CRect* rect);
	
	void EdgeGray(cv::Mat *gray, cv::Mat *edge, cv::Mat *outImage);

	//영상 출력

	void CGetImage::SelectImage();

	int CGetImage::ReadImageRef();
	
	//cv::VideoCapture vcap; 
    cv::Mat image;

	//영상받는 스레드
	CWinThread *pThreadCap;
	static UINT threadCapture(LPVOID pParam);
	bool bShutdown;
	bool bCapture;
	bool bShow;

	CvPoint m_point1,m_point2;

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_CtrlSlider1;
	CSliderCtrl m_CtrlSlider2;
	CSliderCtrl m_CtrlSliderErode;
	CSliderCtrl m_CtrlSliderDlate;
	CSliderCtrl m_CtrlSlider_Treshold;
	CSliderCtrl m_CtrlSliderTresmax;
	CSliderCtrl m_CtrlSlider_X;
	CSliderCtrl m_CtrlSlider_Y;
	CSliderCtrl m_CtrlSlider_XW;
	CSliderCtrl m_CtrlSlider_YH;
	CSliderCtrl m_CtrlSlider_R;
	CSliderCtrl m_CtrlSlider_G;
	CSliderCtrl m_CtrlSlider_B;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonTemplate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	CButton m_CheckSelect;
	afx_msg void OnBnClickedCheckSelect();
	afx_msg void OnBnClickedButtonSaveRef();
};
