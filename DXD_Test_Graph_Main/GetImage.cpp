// DlgImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AcqVoltageSamples_IntClk.h"
#include "GetImage.h"

#include "ping.h"

// CGetImage 대화 상자입니다.


CGetImage::CGetImage()	
{

}

CGetImage::~CGetImage()
{
}

void CGetImage::DoDataExchange(CDataExchange* pDX)
{
	
	DDX_Control(pDX, IDC_STATIC_PIC, m_pic);
	DDX_Control(pDX, IDC_STATIC_PIC2, m_pic2);

	DDX_Control(pDX, IDC_SLIDER1, m_CtrlSlider1);
	DDX_Control(pDX, IDC_SLIDER2, m_CtrlSlider2);
	DDX_Control(pDX, IDC_SLIDER3, m_CtrlSliderErode);
	DDX_Control(pDX, IDC_SLIDER4, m_CtrlSliderDlate);
	DDX_Control(pDX, IDC_SLIDER_TRESHOLD, m_CtrlSlider_Treshold);
	DDX_Control(pDX, IDC_SLIDER_TRESMAX, m_CtrlSliderTresmax);
	DDX_Control(pDX, IDC_SLIDER_X, m_CtrlSlider_X);
	DDX_Control(pDX, IDC_SLIDER_Y, m_CtrlSlider_Y);
	DDX_Control(pDX, IDC_SLIDER_XW, m_CtrlSlider_XW);
	DDX_Control(pDX, IDC_SLIDER_YH, m_CtrlSlider_YH);
	DDX_Control(pDX, IDC_SLIDER_R, m_CtrlSlider_R);
	DDX_Control(pDX, IDC_SLIDER_G, m_CtrlSlider_G);
	DDX_Control(pDX, IDC_SLIDER_B, m_CtrlSlider_B);
	DDX_Control(pDX, IDC_CHECK_SELECT, m_CheckSelect);
}




BOOL CGetImage::OnInitDialog()
{
    CDialog::OnInitDialog();
  /*
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
  */
	
	m_CtrlSlider1.SetRange(0,2000);
	m_CtrlSlider2.SetRange(0,2000);
	m_CtrlSliderErode.SetRange(0,10);
	m_CtrlSliderDlate.SetRange(0,10);
	m_CtrlSlider_Treshold.SetRange(0,255);
	m_CtrlSliderTresmax.SetRange(0,20000);
	m_CtrlSlider_X.SetRange(0,1920);
	m_CtrlSlider_Y.SetRange(0,1920);
	m_CtrlSlider_XW.SetRange(0,1920);
	m_CtrlSlider_YH.SetRange(0,1920);	
	m_CtrlSlider_R.SetRange(0,400);
	m_CtrlSlider_G.SetRange(0,255);
	m_CtrlSlider_B.SetRange(0,255);

	m_CtrlSlider1.SetTicFreq(1);
	m_CtrlSlider2.SetTicFreq(1);
	m_CtrlSlider_Treshold.SetTicFreq(1);
	m_CtrlSliderTresmax.SetTicFreq(1);
	m_CtrlSlider_X.SetTicFreq(1);
	m_CtrlSlider_Y.SetTicFreq(1);
	m_CtrlSlider_XW.SetTicFreq(1);
	m_CtrlSlider_YH.SetTicFreq(1);


	CString temp;

	
	if(ReadImageRef())
	{
		mX	= 404;
		mY	= 490;
		mXW = 205;
		mYH = 47;
	
		m_ocrX	= 404;
		m_ocrY	= 490;	
		m_ocrXW= 205;
		m_ocrYH = 47;	
	
		nErode	= 0;
		nDilate	= 0;	
		mTres	= 82;	
		mTresMax	= 1000;	
	}

	//	
	//temp.Format("%d", nErode);
	//GetDlgItem(IDC_STATIC_SLIDER_ERODE)->SetWindowTextA(temp);
	//	
	//temp.Format("%d",nDilate);
	//GetDlgItem(IDC_STATIC_SLIDER_DILATE)->SetWindowTextA(temp);



	//temp.Format("%d",mTres);
	//GetDlgItem(IDC_STATIC_SLIDER_TRESHOLD)->SetWindowTextA(temp);
	//
	//
	//temp.Format("%d",mTresMax);
	//GetDlgItem(IDC_STATIC_SLIDER_TRESMAX)->SetWindowTextA(temp);


	
	m_SelImage = 0;
	tres1 = 90;
	tres2 = 102;

	mR = 0;
	mG = 61;
	mB = 64;

	m_CtrlSlider1		.SetPos(tres1);	
	m_CtrlSlider2		.SetPos(tres2);	
	m_CtrlSliderErode		.SetPos(nErode);		
	m_CtrlSliderDlate		.SetPos(nDilate);	
	m_CtrlSlider_Treshold	.SetPos(mTres);		
	m_CtrlSliderTresmax	.SetPos(mTresMax);	
	m_CtrlSlider_X		.SetPos(mX	);
	m_CtrlSlider_Y		.SetPos(mY	);
	m_CtrlSlider_XW		.SetPos(mXW );
	m_CtrlSlider_YH		.SetPos(mYH );
	m_CtrlSlider_R		.SetPos(mR );
	m_CtrlSlider_G		.SetPos(mG );
	m_CtrlSlider_B		.SetPos(mB );


	
	bProcess= false;
	binv = false;
	
	//if (api.Init("C:\\Program Files\\Tesseract-OCR\\tessdata", "fm+nousb+nodisk+cdread+portable+btreadyT+_auxT+portable2")) 
	//if (api.Init("C:\\Program Files\\Tesseract-OCR\\tessdata", "smfont")) 
	if (api.Init("D:\\SM Digital\\AudioTester_1100AP\\tessdata", "smfont")) 
	//if (api.Init("C:\\Program Files\\Tesseract-OCR\\tessdata", "sm")) 
	{
		AfxMessageBox("tessract Init 실패");
        exit(1);
    }


	bShow= false;
	bShutdown = false;;
	bCapture = false;;
	pThreadCap = AfxBeginThread(threadCapture, this);

	SetTimer(1,100,0);


	
	/*
	m_pDC = m_pic.GetDC();
	m_pic.GetClientRect(&rect);
	
	memDC.CreateCompatibleDC(m_pDC);
	Bitmap.CreateCompatibleBitmap(m_pDC,rect.Width(),rect.Height());
	
	pOldBitmap = memDC.SelectObject(&Bitmap);

	
	m_pDC2 = m_pic2.GetDC();
	m_pic2.GetClientRect(&rect2);
	
	memDC2.CreateCompatibleDC(m_pDC2);
	Bitmap2.CreateCompatibleBitmap(m_pDC2,rect2.Width(),rect2.Height());
	
	pOldBitmap2 = memDC2.SelectObject(&Bitmap2);*/

	return TRUE;
}
// CGetImage 메시지 처리기입니다.


void CGetImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(pScrollBar)
	{
		// 어떤 슬라이더인지 검사
		if(pScrollBar == (CScrollBar*)&m_CtrlSlider1)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider1.GetPos();
			
			tres1 =nPos;

			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_TRES1)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider2)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider2.GetPos();
			
			tres2 =nPos;

			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_TRES2)->SetWindowTextA(temp);
		}
		else if ( pScrollBar == (CScrollBar*)&m_CtrlSliderErode)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSliderErode.GetPos();
			
			nErode =nPos;

			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_ERODE)->SetWindowTextA(temp);
		}
		else if ( pScrollBar == (CScrollBar*)&m_CtrlSliderDlate)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSliderDlate.GetPos();
			
			nDilate =nPos;

			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_DILATE)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_X)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_X.GetPos();
			
			mX =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_X)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_Y)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_Y.GetPos();
			
			mY =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_Y)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_XW)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_XW.GetPos();
			
			mXW =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_XW)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_YH)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_YH.GetPos();
			
			mYH =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_YH)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_Treshold)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_Treshold.GetPos();
			
			mTres =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_TRESHOLD)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSliderTresmax)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSliderTresmax.GetPos();
			
			mTresMax =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_TRESMAX)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_R)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_R.GetPos();
			
			mR =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_R)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_G)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_G.GetPos();
			
			mG =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_G)->SetWindowTextA(temp);
		}
		else if (pScrollBar == (CScrollBar*)&m_CtrlSlider_B)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos = m_CtrlSlider_B.GetPos();
			
			mB =nPos;
			
			CString temp;
			temp.Format("%d",nPos);
			GetDlgItem(IDC_STATIC_SLIDER_B)->SetWindowTextA(temp);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CGetImage::ProcessTemplate()
{
	double min, max;
	double smin = 100, smax = 0;
	cv::Point left_top;
	
	cv::Size Size;
	Size.width = src_Mat.cols;
	Size.height = src_Mat.rows;
	
	Mat showMat;

	namedWindow("image");

	int cnt = 20;

	int i = 0;
	//for (  i = 0 ; i < cnt ; i++ )
	{
		Mat image(src_Mat);
		//IplImage* image = cvCreateImage ( Size, src->depth, src->nChannels );
		Mat imageGray(Size.height,Size.width,1);
		//IplImage* imageGray = cvCreateImage ( Size, 8,1 );

		cv::resize(src_Mat,image,Size);
		//cvResize(src,image);

		cvtColor(image,imageGray,CV_BGR2GRAY);
		//cvCvtColor(image,imageGray,CV_BGR2GRAY);
		CString str = PATH_BASE_DIR;
		str += _T("\\Image\\");
		CString lRefFile = str;
		lRefFile +=  "SearchImage.jpg";


		Mat B = imread(lRefFile.GetBuffer(), -1); 
	//	Mat B = imread("template1.jpg", -1); 
		//IplImage *B = cvLoadImage("template\\template1.jpg", -1); 

		Mat BGray(B.rows,B.cols,1);
		//IplImage *BGray = cvCreateImage(cvGetSize(B), 8, 1);

		cv::cvtColor(B,BGray,CV_BGR2GRAY);
		//cvCvtColor(B,BGray,CV_BGR2GRAY);

		Mat C(cv::Size( image.cols - B.cols+1, image.rows - B.rows+1 ) ,CV_32FC1 , 1);
		//IplImage *C = cvCreateImage( cvSize( image->width - B->width+1, image->height - B->height+1 ), IPL_DEPTH_32F, 1 ); // 상관계수를 구할 이미지(C)

		cv::matchTemplate(imageGray, BGray, C, CV_TM_CCOEFF_NORMED); 
		//cvMatchTemplate(imageGray, BGray, C, CV_TM_CCOEFF_NORMED); // 상관계수를 구하여 C 에 그린다.
		
		cv::minMaxLoc(C, &min, &max, NULL, &left_top);
		//cvMinMaxLoc(C, &min, &max, NULL, &left_top); // 상관계수가 최대값을 값는 위치 찾기 

		showMat = image;

		if ( max > 0.6 )
		{
			//매칭결과 이미지 잘라서 다른 이미지로 옮기기
			Template_Rslt = showMat(cv::Rect(left_top.x, left_top.y, B.cols, B.rows));

			i = cnt;

			imshow("Template Result",Template_Rslt);

			ProcessImage();
		}
		
		cv::rectangle(showMat,cv::Rect(left_top.x, left_top.y, B.cols, B.rows), CV_RGB(255,255,255),2);
		
		if(min < smin)
			smin = min;
		if(max > smax)
			smax = max;

		CString csMin,csMax;
		csMin.Format("Min : %f",smin);
		csMax.Format("Max : %f",smax);
		string strMin = csMin;
		string strMax = csMax;
	
		cv::putText(image,strMin,cv::Point(0,50),FONT_HERSHEY_SIMPLEX,1.0,CV_RGB(255,255,255),2);
		cv::putText(image,strMax,cv::Point(0,80),FONT_HERSHEY_SIMPLEX,1.0,CV_RGB(255,255,255),2);
		imshow("image",showMat);
		cv::waitKey(1);

		//템플릿 매칭값이 충분하지 못함
		if ( i < cnt/2 )
		{
			//사이즈값 축소
			Size.width*=0.99;
			Size.height*=0.99;
		}
		else if ( i == cnt/2 )
		{
			Size.width = src_Mat.cols;
			Size.height = src_Mat.rows;
		}
		else
		{
			Size.width*=1.01;
			Size.height*=1.01;
		}
		
	}
	
	
}

void CGetImage::ProcessImage()
{
	Mat image(Template_Rslt);

	Mat imageGray(image.rows, image.cols, 1);
	
	Mat imageThres;
	Mat imageThres2;
	Mat imageThres3;

	cvtColor(image,imageGray,CV_BGR2GRAY);
	cv::GaussianBlur(imageGray, imageGray, cv::Size(0,0), 0.5 );	

#ifdef __IMAGE_DEBUG__
	imshow("blur",imageGray);
#endif

	//이미지 선명하게
	Mat sharpen_image;
	
	Mat sharpen_kernel = (Mat_<char>(3,3) << -1,-1, -1,
											-1, 9, -1,
											-1, -1, -1);

	
	Mat sharpen_kernel2 = (Mat_<char>(3,3) << -1,-1, -1,
											-1, 9, -1,
											-1, -1, -1);
	
	filter2D(imageGray, sharpen_image, imageGray.depth(), sharpen_kernel2);

	sharpen_image.convertTo(gray_Mat,CV_8UC1);

	Canny(gray_Mat,edge_Mat,tres1,tres2);
	
	gray_Mat.copyTo(edgeGray_Mat);
	EdgeGray(&gray_Mat, &edge_Mat, &edgeGray_Mat);	
	
	threshold(edgeGray_Mat,imageThres, mTres, mTresMax, CV_THRESH_BINARY);

	threshold(sharpen_image,imageThres2, mTres, mTresMax, CV_THRESH_BINARY);

	adaptiveThreshold(sharpen_image,imageThres3,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,5,tres2);
	

#ifdef __IMAGE_DEBUG__	//#endif

	imshow("sharpen_image",sharpen_image);
	imshow("thres",imageThres);
	imshow("thres2",imageThres2);
	imshow("thres3",imageThres3);
#endif

	
	for ( int i = 0 ; i < nErode; i++ )
	{
		erode(imageThres2,imageThres2,Mat());
		//cvErode( edgeGray_Mat, edgeGray_Mat,0,1);
	}

	for ( int i = 0 ; i < nDilate ; i ++)
	{
		dilate(imageThres2,imageThres2,Mat());
		//cvDilate (edgeGray, edgeGray);
	}

	
	//mX,mY;	
	//int mXW,mYH
	//	int m_ocrX,m_ocrY;	
	//int m_ocrXW,m_ocrYH;
	int lx = m_ocrX - mX ;
	int ly = m_ocrY - mY ;

	if(lx < 0)
		lx = 0;

	if(ly < 0)
		ly = 0;

//	Mat crop = imageThres2(Rect(0,imageThres.size().height/6*2,imageThres.size().width,imageThres.size().height/6*3));
	Mat crop = imageThres2(Rect(lx,ly, m_ocrXW, m_ocrYH));
	//Mat crop = sharpen_image(Rect(lx,ly, m_ocrXW, m_ocrYH));
#ifdef __IMAGE_DEBUG__	//#endif		
	imshow("crop",crop);
#endif

	DisplayImage2(&IplImage(crop));//	DisplayImage(&IplImage(org_img));

	char *outText;
	api.SetImage((uchar*)crop.data,crop.size().width,crop.size().height,crop.channels(),crop.step1());
	
	outText = api.GetUTF8Text();
	GetDlgItem(IDC_EDIT_RSLT)->SetWindowTextA(outText);		
}


void CGetImage::OnBnClickedButtonTemplate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ProcessTemplate();
}


void CGetImage::EdgeGray(cv::Mat *gray, cv::Mat *edge, cv::Mat *outImage)
{
	for(int y = 0; y < gray->rows; ++y){
		uchar *gpt = (uchar*)(gray->data + gray->step1() * y);
		uchar *ept = (uchar*)(edge->data + edge->step1() * y);
		uchar *opt = (uchar*)(outImage->data + outImage->step1() * y);

		for(int x = 0; x < gray->cols; ++x){
			if(*ept == 255)
				*opt = 0;
			else
				*opt = *gpt;
			++gpt;
			++ept;
			++opt;
		}
	}
}

UINT CGetImage::threadCapture(LPVOID pParam)
{
	CGetImage* dlg = (CGetImage*) pParam;

	while(!dlg->bShutdown)
	{
		vcap >> dlg->image;

		//cvShowImage("Video",dlg->image);
		if ( dlg->bCapture )
		{
			//dlg->src = &IplImage(dlg->image);
			dlg->src_Mat = dlg->image;

			dlg->bCapture = false;
		}

		Sleep(10);
	}


	return 0;
}


void CGetImage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ( nIDEvent == 1 )
	{
		bCapture = true;
		while(bCapture)
			Sleep(10);

		//imshow("src",src_Mat);
		//ProcessImage2();
		SelectImage();
		//DisplayImage();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CGetImage::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	bShutdown = true;
	Sleep(100);

//	if( vcap.isOpened() )
	{

//		vcap.release();
		
		memDC.SelectObject(pOldBitmap);
		memDC2.SelectObject(pOldBitmap2);
	}

	KillTimer(1);

	api.End();
}

void CGetImage::DisplayImage(IplImage* img)
{
	IplImage* img_resize=NULL;
//	IplImage* img_orgsize=NULL;

	CRect prevRect = rect;


	ResizeImage(img,&img_resize,&rect);
//	ResizeImage(img,&img_resize,&prevRect);

	//img_resize = img;CV_IMWRITE_JPEG_QUALITY

//	DisplayBitmap(&memDC,rect,img_resize);
	DisplayBitmap(&memDC,rect,img_resize);
	
	//나머지 공간 흰색으로 채우기
	//m_pDC->PatBlt(0,0,prevRect.Width(), prevRect.Height(), WHITENESS);
	m_pDC->BitBlt(0,0,img_resize->width,img_resize->height,&memDC,0,0,SRCCOPY);
	
	if(img_resize)
	{
		cvReleaseImage(&img_resize);
	}
}


void CGetImage::DisplayImage2(IplImage* img)
{

	IplImage* img_resize=NULL;

	CRect prevRect = rect2;
	ResizeImage(img,&img_resize,&rect2);

	DisplayBitmap(&memDC2,rect2,img_resize);
	
	//나머지 공간 흰색으로 채우기
	m_pDC2->PatBlt(0,0,prevRect.Width(), prevRect.Height(), WHITENESS);
	m_pDC2->BitBlt(0,0,img_resize->width,img_resize->height,&memDC2,0,0,SRCCOPY);
	
	if(img_resize)
	{
		cvReleaseImage(&img_resize);
	}
}



void CGetImage::DisplayBitmap(CDC* pDC, CRect rect, IplImage* img)
{
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes=1;
	bitmapInfo.bmiHeader.biCompression=BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter=100;
	bitmapInfo.bmiHeader.biYPelsPerMeter=100;
	bitmapInfo.bmiHeader.biClrUsed=0;
	bitmapInfo.bmiHeader.biClrImportant=0;
	bitmapInfo.bmiHeader.biSizeImage=0;
	bitmapInfo.bmiHeader.biWidth=img->width;
	bitmapInfo.bmiHeader.biHeight=-img->height;

	if (img->nChannels == 3)
	{
		bitmapInfo.bmiHeader.biBitCount=img->depth * img->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom,
		0, 0, img->width, img->height, img->imageData, &bitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);
	}
	else
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
		IplImage * tmp = cvCreateImage(cvGetSize(img),img->depth,3);
		cvCvtColor(img,tmp,CV_GRAY2BGR);

		bitmapInfo.bmiHeader.biBitCount=tmp->depth * tmp->nChannels;
		
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom,
		0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);
		cvReleaseImage(&tmp);
		delete tmp;
	}
}


void CGetImage::ResizeImage(IplImage* src_img, IplImage** dst_img, CRect* rect)
{
	int ndst_width=0,ndst_height=0;
	//먼저 영상의 가로와 세로중 장축을 구한다.

	if(src_img->width > src_img->height)
	{
		ndst_width  = rect->Width();
		if ( src_img->width > 0 )
			ndst_height  = (src_img->height * rect->Width()) / src_img->width; 
	}
	else
	{
		if ( src_img->height > 0 )
			ndst_width  = (src_img->width * rect->Height()) / src_img->height;
		ndst_height  = rect->Height(); 
	}

	(*dst_img) = cvCreateImage(cvSize(ndst_width,ndst_height),IPL_DEPTH_8U,src_img->nChannels);
	
	cvResize(src_img,(*dst_img));
	rect->right   = rect->left + ndst_width;
	rect->bottom  = rect->top + ndst_height;
}


void CGetImage::SelectImage()
{
	//이미지 처리
	
	//org_img = cvarrToMat(src);
	org_img = src_Mat;//Mat showMat;

	int x,y,h,s,v;
	

		CvPoint point1,point2;
		CvScalar scal= {255,255,255};
	if(m_CheckSelect.GetCheck())
	{
	
		point1.x = mX;
		point1.y = mY;
		
		point2.x = mX + mXW;
		point2.y = mY + mYH;
		
		
		scal.val[0]= 255;
		scal.val[1]= 255;
		scal.val[2]= 255;
		cvRectangle(&IplImage(org_img),point1,point2,scal,2);
	}
	else
	{

		//int m_ocrX,m_ocrY;	
		//int m_ocrXW,m_ocrYH;	

		point1.x = m_ocrX;
		point1.y = m_ocrY;
		
		point2.x = m_ocrX + m_ocrXW;
		point2.y = m_ocrY + m_ocrYH;

		scal.val[0]= 0;
		scal.val[1]= 255;
		scal.val[2]= 255;
	//	scal.val[3]= 0,255,255};
		cvRectangle(&IplImage(org_img),point1,point2,scal,2);
	}

		
	int lx,ly,lw,lh;
	lx = 500;//org_img.cols*0.8;
	ly = 500;//org_img.rows*0.8;
	lw = 500;//org_img.cols*0.8;
	lh = 500;//org_img.rows*0.8;
	//m_point1,m_point2
	//Template_Rslt = org_img(cv::Rect(org_img.cols*0.8, org_img.rows*0.8, org_img.cols*0.8, org_img.rows*0.8));
	//Template_Rslt = org_img(cv::Rect(lx,ly,lw,lh));

	DisplayImage(&IplImage(org_img));

	//IplImage* img_resize=NULL;
	//IplImage* img_orgsize=NULL;

	//CRect prevRect = rect;
	/*
	prevRect.bottom = rect.bottom;
	if(src_img->width > src_img->height)
	{
		ndst_width  = rect->Width();
		if ( src_img->width > 0 )
			ndst_height  = (src_img->height * rect->Width()) / src_img->width; 
	}
	else
	{
		if ( src_img->height > 0 )
			ndst_width  = (src_img->width * rect->Height()) / src_img->height;
		ndst_height  = rect->Height(); 
	}

	(*dst_img) = cvCreateImage(cvSize(ndst_width,ndst_height),IPL_DEPTH_8U,src_img->nChannels);
	
	cvResize(src_img,(*dst_img));
	rect->right   = rect->left + ndst_width;
	rect->bottom  = rect->top + ndst_height;
	*/
	//prevRect.top = 200;
	//prevRect.left = 300;

	//prevRect.bottom = prevRect.top + img->height;
	//prevRect.right = prevRect.left + img->width;

//	ResizeImage(img,&img_resize,&rect);
//	ResizeImage(img,&img_resize,&prevRect);

	//img_resize = img;CV_IMWRITE_JPEG_QUALITY

//	DisplayBitmap(&memDC,rect,img_resize);
	//DisplayBitmap(&memDC,prevRect,Template_Rslt);
	
	//나머지 공간 흰색으로 채우기
	//m_pDC->PatBlt(0,0,prevRect.Width(), prevRect.Height(), WHITENESS);
	//m_pDC->BitBlt(0,0,img_resize->width,img_resize->height,&memDC,0,0,SRCCOPY);
	
	//if(img_resize)
	//{
	//	cvReleaseImage(&img_resize);
	//}

}
//
//#define MOUSE_SEL_IDLE			0
//#define MOUSE_SEL_IMAGE_XY		1
//#define MOUSE_SEL_IMAGE_MOVE	2
//#define MOUSE_SEL_OCR_XY		3
//#define MOUSE_SEL_OCR_MOVE		4
//
//void CGetImage::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	int lx = (point.x-10)/0.8;
//	int ly = (point.y-10)/0.8;
//	if((0 < lx)&&(0 < ly)&&(1280 > lx)&&(720 > ly))
//	{
//
//		if(m_CheckSelect.GetCheck())
//		{
//			mX = lx;
//			mY = ly;
//			m_SelImage = MOUSE_SEL_IMAGE_XY;
//		}
//		else
//		{
//	
//			m_ocrX = lx;
//			m_ocrY = ly;
//
//			m_SelImage = MOUSE_SEL_OCR_XY;
//		}
//
//	}
//		
//	//point2.x = m_ocrX + m_ocrXW;
//	//point2.y = m_ocrY + m_ocrYH;
//
//
//	CDialogEx::OnLButtonDown(nFlags, point);
//}
//
//
//void CGetImage::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	m_SelImage = MOUSE_SEL_IDLE;
//	CDialogEx::OnLButtonUp(nFlags, point);
//}

//
//void CGetImage::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//		
//	int lx = (point.x-10)/0.8;
//	int ly = (point.y-10)/0.8;
//
//	switch(m_SelImage)
//	{
//	case MOUSE_SEL_IMAGE_XY:
//		if((lx-mX) > 0)
//		{
//			mXW = lx - mX ;
//		}
//		else
//		{
//			mXW = 0;
//		}
//
//			
//		if((ly - mY)  > 0)
//		{
//			mYH = ly - mY ;
//		}
//		else
//		{
//			mYH = 0;
//		}
//		
//		break;
//
//	case MOUSE_SEL_IMAGE_MOVE:
//		
//
//		mX = lx;
//		mY = ly;
//		
//		break;
//		
//	case MOUSE_SEL_OCR_XY:
//		if((lx-m_ocrX) > 0)
//		{
//			m_ocrXW = lx - m_ocrX ;
//		}
//		else
//		{
//			m_ocrXW = 0;
//		}
//
//			
//		if((ly - m_ocrY)  > 0)
//		{
//			m_ocrYH = ly - m_ocrY ;
//		}
//		else
//		{
//			m_ocrYH = 0;
//		}
//		
//		break;
//
//	case MOUSE_SEL_OCR_MOVE:
//		
//
//		m_ocrX = lx;
//		m_ocrY = ly;
//		
//		break;
//
//	default:
//		break;
//	}
//	CDialogEx::OnMouseMove(nFlags, point);
//}

//
//BOOL CGetImage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	
//	//int lx = (point.x-340)/0.8;
//	//int ly = (point.y-10)/0.8;
//
//	//switch(m_SelImage)
//	//{
//	//case MOUSE_SEL_IMAGE_XY:
//	//	if((lx-m_ocrX) > 0)
//	//	{
//	//		m_ocrXW = lx - m_ocrX ;
//	//	}
//	//	else
//	//	{
//	//		m_ocrXW = 0;
//	//	}
//
//	//		
//	//	if((ly - m_ocrY)  > 0)
//	//	{
//	//		m_ocrYH = ly - m_ocrY ;
//	//	}
//	//	else
//	//	{
//	//		m_ocrYH = 0;
//	//	}
//	//	
//	//	break;
//
//	//default:
//	//	break;
//	//}
//	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
//}
//
//
//void CGetImage::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	
//	int lx = (point.x-10)/0.8;
//	int ly = (point.y-10)/0.8;
//
//	if((0 < lx)&&(0 < ly)&&(1280 > lx)&&(720 > ly))
//	{
//
//		if(m_CheckSelect.GetCheck())
//		{		
//		
//			if(((lx-mX) >= 0) && ((lx-(mX + mXW)) <= 0)&&((ly-mY) >= 0) && ((ly-(mY + mYH)) <= 0))
//			{
//				m_SelImage = MOUSE_SEL_IMAGE_MOVE;
//			}
//		}
//		else
//		{
//	
//			if(((lx-m_ocrX) >= 0) && ((lx-(m_ocrX + m_ocrXW)) <= 0)&&((ly-m_ocrY) >= 0) && ((ly-(m_ocrY + m_ocrYH)) <= 0))
//			{
//				m_SelImage = MOUSE_SEL_OCR_MOVE;
//			}
//		}
//
//	}
//	
//
//
//
//	CDialogEx::OnRButtonDown(nFlags, point);
//}
//
//
//void CGetImage::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	m_SelImage = MOUSE_SEL_IDLE;
//	CDialogEx::OnRButtonUp(nFlags, point);
//}
//

//void CGetImage::OnBnClickedCheckSelect()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//UpdateData();
//	//if(m_CheckSelect)
//}


void CGetImage::OnBnClickedButtonSaveRef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Mat showMat;
	Mat image(src_Mat);
		////IplImage* image = cvCreateImage ( Size, src->depth, src->nChannels );
		//Mat imageGray(Size.height,Size.width,1);
		////IplImage* imageGray = cvCreateImage ( Size, 8,1 );

		//cv::resize(src_Mat,image,Size);
		////cvResize(src,image);

		//cvtColor(image,imageGray,CV_BGR2GRAY);
		////cvCvtColor(image,imageGray,CV_BGR2GRAY);

		//Mat B = imread("template1.jpg", -1); 
		////IplImage *B = cvLoadImage("template\\template1.jpg", -1); 

		//Mat BGray(B.rows,B.cols,1);
		////IplImage *BGray = cvCreateImage(cvGetSize(B), 8, 1);

		//cv::cvtColor(B,BGray,CV_BGR2GRAY);
		////cvCvtColor(B,BGray,CV_BGR2GRAY);

		//Mat C(cv::Size( image.cols - B.cols+1, image.rows - B.rows+1 ) ,CV_32FC1 , 1);
		////IplImage *C = cvCreateImage( cvSize( image->width - B->width+1, image->height - B->height+1 ), IPL_DEPTH_32F, 1 ); // 상관계수를 구할 이미지(C)

		//cv::matchTemplate(imageGray, BGray, C, CV_TM_CCOEFF_NORMED); 
		////cvMatchTemplate(imageGray, BGray, C, CV_TM_CCOEFF_NORMED); // 상관계수를 구하여 C 에 그린다.
		//
		//cv::minMaxLoc(C, &min, &max, NULL, &left_top);
		////cvMinMaxLoc(C, &min, &max, NULL, &left_top); // 상관계수가 최대값을 값는 위치 찾기 

	showMat = image;

	
	//매칭결과 이미지 잘라서 다른 이미지로 옮기기
	//	int mX,mY;	
	//int mXW,mYH;
	Mat lSave_Rslt;
	lSave_Rslt = showMat(cv::Rect(mX, mY, mXW, mYH));

	vector<int> compression_params;
//compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(9);
//	imwrite("SearchImage.jpg",lSave_Rslt,compression_params);


	CString str;

	str = PATH_BASE_DIR;
	str += _T("\\Image\\");
	CString lRefFile = str;
	lRefFile +=  "SearchImage.jpg";//gTestSeqData.strRemoconFileName;
	
	imwrite(lRefFile.GetBuffer(),lSave_Rslt);
//	imwrite("SearchImage.jpg",lSave_Rslt);


	FILE *m_fp;	 

	lRefFile = str;
	lRefFile +=  "SearchImage.txt";//gTestSeqData.strRemoconFileName;
	fopen_s(&m_fp, (LPCSTR)lRefFile, "w+");

	if(m_fp)
	{
		fseek(m_fp, 0, 0);
	
		str.Format(_T("%d %d %d %d\n"), mX, mY, mXW, mYH);
		fputs(str.GetBuffer(),m_fp);

		str.Format(_T("%d %d %d %d\n"), m_ocrX, m_ocrY, m_ocrXW, m_ocrYH);
		fputs(str.GetBuffer(),m_fp);	

		str.Format(_T("%d %d %d %d\n"), 	mTres, mTresMax, nErode, nDilate );
		fputs(str.GetBuffer(),m_fp);		
			
		fclose(m_fp);	
	}
	else
	{
		//MessageBox("Save Image Data Fail!!");
	}
}


int CGetImage::ReadImageRef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int lError = 0;
	CString str;
	char lbuf[1024];

	str = PATH_BASE_DIR;
	str += _T("\\Image\\");
	CString lRefFile = str;
	

	FILE *m_fp;	 

	lRefFile = str;
	lRefFile +=  "SearchImage.txt";//gTestSeqData.strRemoconFileName;
	fopen_s(&m_fp, (LPCSTR)lRefFile, "r");

	if(m_fp)
	{
		int lTemp_X, lTemp_Y,lTemp_XW, lTemp_YH;
		if(fgets(lbuf, 1024, m_fp) != NULL) 
		{		
			//sscanf_s(lbuf, "%d %d %d %d ", &lTemp_X,32, &lTemp_Y,32,&lTemp_XW,32, &lTemp_YH,32);
			sscanf_s(lbuf, "%d %d %d %d ", &lTemp_X, &lTemp_Y, &lTemp_XW, &lTemp_YH);
			mX = lTemp_X;
			mY = lTemp_Y;
			mXW = lTemp_XW;
			mYH = lTemp_YH;
		}
		else
		{
			lError = 1;

		}


		if(fgets(lbuf, 1024, m_fp) != NULL) 
		{		
			sscanf_s(lbuf, "%d %d %d %d ", &lTemp_X, &lTemp_Y,&lTemp_XW, &lTemp_YH);
			m_ocrX = lTemp_X;
			m_ocrY = lTemp_Y;
			m_ocrXW = lTemp_XW;
			m_ocrYH = lTemp_YH;

		}
		else
		{
			lError = 2;
		}

			if(fgets(lbuf, 1024, m_fp) != NULL) 
		{		
			sscanf_s(lbuf, "%d %d %d %d ", &lTemp_X, &lTemp_Y,&lTemp_XW, &lTemp_YH);
			mTres = lTemp_X;
			mTresMax = lTemp_Y;
			nErode = lTemp_XW;
			nDilate = lTemp_YH;	
		

		}
		else
		{
			lError = 2;
		}		

		fclose(m_fp);	
	}
	else
	{
	//	MessageBox("Read Image Data Fail!!");
		lError = 3;
	}

	return lError ;
}

