
#if !defined(AFX_FFT_SM_H__191002__INCLUDED_)
#define AFX_FFT_SM_H__191002__INCLUDED_


#include <math.h>

#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define PI  3.141592653589793238462643383279502884197169399375105820974944

typedef struct {
	double real, imag;
} COMPLEX;



/* function prototypes for dft and inverse dft functions */

class CFFT_SM 
{
public:

	CFFT_SM();
	virtual ~CFFT_SM();

	void fft(COMPLEX *, int);
	int log2(unsigned int);

	int m_CheckError[2000];
	int m_CheckMode;

	int CFFT_SM::CheckHighFrequency(double *lSpectrum, double lCheckPoint, double lCheckLimit);
	int CFFT_SM::CheckSideFrequency(double *lSpectrum, int lSideCount, double lCheckLimit );
	int CFFT_SM::CheckSideFrequencyNew(double *lSpectrum, int lSideCount, double lCheckLimit );
	int CFFT_SM::CheckStartSweep(double *lSpectrum, double lFreq, double lVolume );
	int CFFT_SM::CheckEndSweep(double lFreq, double lVolume  );
	int CFFT_SM::CheckFreqVol(double *lWave,  double *lFreq, double *lvol );
	int CFFT_SM::CheckChart(_AudSignal_T *lp_WaveSignal, _AudSignal_T *lp_RefWaveSignal) ;
	void CFFT_SM::CheckData(double *lWave, double *DisplaylWave, double *lFreq, double *lvol);
};

#endif


//
///************************************************************************
// IFFTTEST.C - Demonstrate and test FFT and Inverse FFT functions
// Requires time domain data file in DSP_FILE format.
// Generates DSP_FILE format files for spectral magnitude
// and reconstructed time domain data.
// ************************************************************************/
//
//void main(void)
//{
//	int          i, length, fft_length, m;
//	double       tempflt;
//	double       signal[4096], log_mag[4096];
//	COMPLEX      samp[4096];
//	double		lSigArray[4096];
//
//	// Read the input data file from the dsp format.  데이타파일 읽는 명령
//	fp = open_read((char *)("test.txt"));
//	length = 128;
//	//signal = read_double_record(fp);
//
//	for (i = 0; i < length; i++)
//	{
//		lSigArray[i] = sin(i/128.*PI*64.0);
//		signal[i] = lSigArray[i];
//	}
//		
//	// determine fft size and allocate the complex array - fft size와 배열
//	m = log2(length);
//	fft_length = 1 << m;   //bit shift
//	//samp = (COMPLEX *)calloc(fft_length, sizeof(COMPLEX));
//
//	// copy input signal to complex array and do the fft
//	for (i = 0; i < length; i++)
//		samp[i].real = signal[i];
//	fft(samp, m);
//
//	/* find log magnitude and store for output */
//	//log_mag = (double *)calloc(fft_length, sizeof(double));
//	for (i = 0; i < fft_length; i++)
//	{
//		tempflt = samp[i].real * samp[i].real;
//		tempflt += samp[i].imag * samp[i].imag;
//		tempflt = sqrt(tempflt);
//		log_mag[i] = 10 * log10(MAX(tempflt, 1.e-14));
//
//		printf("%d: %.5f\n", i, log_mag[i]);
//	}
//	fp = open_write((char *)("test_out.txt"));  //결과 저장 텍스트 //
//	write_record(log_mag, length);
//	//getchar();
//}
//
///**************************************************************************
// DISKIO.C - Source code for DSP data format read and write functions
//	 open_read          open DSP data file to be read
//	 open_write         create header and open DSP data file for write
//	 read_record        read one record
//	 write_record       write one record
//	 read_float_record  read one record and convert to float array
// *************************************************************************/
//
// /***************************************************************************************
//  open_read - open a DSP data file for read
//  Returns a pointer to a DSP_FILE structure allocated by the function and opens file_name.
//  Allocation errors or improper type causes a call to exit(1).
//  A bad file_name returns a NULL pointer.
//  DSP_FILE *open_read(char *file_name)
//  ***************************************************************************************/
//
//FILE *open_read(char *file_name) /* file name string */
//{
//	/* allocate the DSP data file structure */
//
//	fp = (FILE *)malloc(sizeof(FILE));
//
//	/* open file for text read and update */
//	errno_t lError = fopen_s(&fp , file_name, "r");
//	return(fp);
//}
//
///**************************************************************************
// open_write - open a DSP data file for write
// Returns a pointer to a DSP_FILE structure allocated by the function.
// Allocation errors or improper type causes a call to exit(1).
// A bad file name returns a NULL pointer.
// DSP_FILE *open_write(char *file_name, int records,int rec_len)
//	 file_name       pointer to file name string
//	 records         number of records of data to be written
//	 rec_len         number of elements in each record
// *************************************************************************/
//
//FILE *open_write(char * file_name)
//{
//	/* allocate the DSP data file structure */
//	fp = (FILE *)malloc(sizeof(FILE));
//
//	/* open file for text write and update*/
//	 fopen_s(&fp,file_name, "wt");
//	return(fp);
//}
//
///********************************************************************
// read_record - read one record of DSP data file
// Exits if a read error occurs or if the DSP_FILE structure is invalid.
// Void read_record(FILE *fp , double *input , int length)
// ********************************************************************/
//
//void read_record(FILE *fp, double * input, int length)
//{
//	int status, i;
//	for (i = 0; i < length; i++)
//	{
//		status = fscanf_s(fp, "%lf\n", &input[length + i]);
//	}
//}
//
/////************************************************************************************************************************
//// read_double_record - read one record of DSP data file and convert to float array of values.
//// Returns a pointer to the beginning of the allocated float array of values representing the record read from the DSP_FILE.
//// Exits if a read or allocation error occurs.
//// float *read_double_record(DSP_FILE *dsp_info)
//// ************************************************************************************************************************/
//
//double *read_double_record(FILE * fp)
//{
//	static double *buf;            /* input buffer to read data in */
//	double *out;                    /* return output pointer */
//	double *out_ptr;
//	double *d_ptr;
//	int i, length, length_in;
//
//	length = 128;
//	length_in = 256;
//
//	buf = (double *)calloc(length_in, sizeof(double));
//	out = (double *)calloc(length, sizeof(double));
//
//	/* read the record into buf */
//	read_record(fp, buf, length);
//
//	/* perform conversion to floating point */
//	out_ptr = out;
//	d_ptr = buf + 128;
//	for (i = 0; i < length; i++)
//		*out_ptr++ = (double)(*d_ptr++);
//
//	return(out);                 /* return converted pointer */
//}
//
///**************************************************************************
// write_record - write one record of DSP_FILE data
// Exits if write error occurs or if the DSP_FILE structure is invalid.
// void write_record(char *ptr,DSP_FILE *dsp_info, int length)
//	 ptr        pointer to data to write to disk (type in dsp_info)
//	 dsp_info   pointer to DSP data file structure
// *************************************************************************/
//
//void write_record(double *ptr, int length)
//{
//	int status, i;
//	for (i = 0; i < length; i++)
//	{
//		status = fprintf(fp, "%d: %lf\n", i , *ptr++);
//	}
//}
//
///**********************************************************************
// DFT.C - SOURCE CODE FOR DISCRETE FOURIER TRANSFORM FUNCTIONS
// fft     In-place radix 2 decimation in time FFT
// log2    Base 2 logarithm
// ***********************************************************************/
//
// /**********************************************************************
//  fft - In-place radix 2 decimation in frequency FFT
//  Requires pointer to complex array, x and power of 2 size of FFT, m
//  (size of FFT = 2^m).  Places FFT output on top of input COMPLEX array.
//  void fft(COMPLEX *x, int m)
//  ***********************************************************************/
//
//void fft(COMPLEX *x, int m)
//{
//	static COMPLEX *w;           // used to store the w complex array
//	static int mstore = 0;       // stores m for future reference
//	static int n = 1;            // length of fft stored for future
//
//	COMPLEX u, temp, tm;
//	COMPLEX *xi, *xip, *xj, *wptr;
//
//	int i, j, k, l, le, windex;
//
//	double arg, w_real, w_imag, wrecur_real, wrecur_imag, wtemp_real;
//
//	if (m != mstore)
//	{
//		// free previously allocated storage and set new m
//		if (mstore != 0) free(w);
//		mstore = m;
//		if (m == 0) return;       // if m=0 then done
//
// // n = 2^m = fft length
//		n = 1 << m;
//		le = n / 2;  //difference between the upper and lower leg indices
//
// // allocate the storage for w
//		w = (COMPLEX *)calloc(le - 1, sizeof(COMPLEX));
//
//		// calculate the w values recursively 계산
//		arg = PI / le;         //  PI/le calculation
//		wrecur_real = w_real = cos(arg);
//		wrecur_imag = w_imag = -sin(arg);
//		xj = w;
//		for (j = 1; j < le; j++)
//		{
//			xj->real = (double)wrecur_real;
//			xj->imag = (double)wrecur_imag;
//			xj++;
//			wtemp_real = wrecur_real * w_real - wrecur_imag * w_imag;
//			wrecur_imag = wrecur_real * w_imag + wrecur_imag * w_real;
//			wrecur_real = wtemp_real;
//		}
//	}
//	// start fft 푸리에 변환 시작
//	le = n;
//	windex = 1;
//	for (l = 0; l < m; l++)
//	{
//		le = le / 2;
//		// first iteration with no multiplies
//		for (i = 0; i < n; i = i + 2 * le)
//		{
//			xi = x + i;
//			xip = xi + le;
//			temp.real = xi->real + xip->real;
//			temp.imag = xi->imag + xip->imag;
//			xip->real = xi->real - xip->real;
//			xip->imag = xi->imag - xip->imag;
//			*xi = temp;
//		}
//		// remaining iterations use stored w
//		wptr = w + windex - 1;
//		for (j = 1; j < le; j++)
//		{
//			u = *wptr;
//			for (i = j; i < n; i = i + 2 * le)
//			{
//				xi = x + i;
//				xip = xi + le;
//				temp.real = xi->real + xip->real;
//				temp.imag = xi->imag + xip->imag;
//				tm.real = xi->real - xip->real;
//				tm.imag = xi->imag - xip->imag;
//				xip->real = tm.real*u.real - tm.imag*u.imag;
//				xip->imag = tm.real*u.imag + tm.imag*u.real;
//				*xi = temp;
//			}
//			wptr = wptr + windex;
//		}
//		windex = 2 * windex;
//	}
//	// rearrange data by bit reversing
//	j = 0;
//	for (i = 1; i < (n - 1); i++)
//	{
//		k = n / 2;
//		while (k <= j)
//		{
//			j = j - k;
//			k = k / 2;
//		}
//		j = j + k;
//		if (i < j)
//		{
//			xi = x + i;
//			xj = x + j;
//			temp = *xj;
//			*xj = *xi;
//			*xi = temp;
//		}
//	}
//}
//
///**************************************************************************
// log2 - base 2 logarithm
// Returns base 2 log such that i = 2^ans where ans = log2(i).
// if log2(i) is between two values, the larger is returned.
// int log2(unsigned int x)
// *************************************************************************/
//int log2(unsigned int x)
//{
//	unsigned int mask, i;
//
//	if (x == 0) return(-1);      // zero is an error, return -1
//	x--;                        // get the max index, x-1
//	for (mask = 1, i = 0; ; mask *= 2, i++)
//	{
//		if (x == 0) return(i);   // return log2 if all zero
//		x = x & (~mask);        // AND off a bit
//	}
//
//}
//
//

//
//http://starpotato.springnote.com/pages/3236324 권영준님의 노트에서...
//
//Jan.06, 2009 : FFT의 개괄적인 이해는 2d fast fourier transform 을 참조하세요
//
//FFT 소스를 요청하는 분들이 많이 계셔서 소스코드를 공개합니다.이게 기본틀은 어디서 코드를 분석한 것이라서 거의 똑같다고 보시면 됩니다.간단한 주석을 달았구요.도움이 되시길 빕니다.
//
///* File            :       struct.h
// * Creator      :       BlackEngine
// * Date         :       2006/03/28
// * Version      :       0.0.1 ( created )
// * Descript     :       Definition of Complex Number , and etc
// */
//
//	typedef struct _COMPLEX {
//	double     real;
//	double     imag;
//} COMPLEX, *pCOMPLEX;
//
//
///* File            :       fft.h
// * Creator      :       BlackEngine
// * Date         :       2006/03/28
// * Version      :       0.0.1 ( created )
// * Descript     :       Perform the FFT Algorithm
// */
//#ifndef _BLACKENGINE_FFT_H_
//#define _BLACKENGINE_FFT_H_
//
//#include "struct.h"
// /* TODO : at 03/29 , decision the return values of FFT Function. */
//int fft_2d(COMPLEX **, int, int, int);
//int fft(int, int, double *, double *);
//#endif /* ifndef _BLACKENGINE_FFT_H_ */
//
//
///* File            :       fft.c
// * Creator      :       BlackEngine
// * Date         :       2006/03/28
// * Version      :       0.0.1 ( created )
// * Descript     :       Perform the FFT Algorithm ( 2-D Image FFT )
// */
//#include 
//#include 
//#include
//
//#include "fft.h"
//
//#define TRUE 1
//#define FALSE 0
// /*#define NULL 0*/
//
// /* Powerof2 function : compare the integer with 2^m
//  * and find maximum m , that is 2^m &lt;= nx
//  * twopm = 2^m
//  */
//int Powerof2(int nx, int* m, int* twopm) {
//	int pwr;
//	*m = 0;
//	for (pwr = 1; pwr &lt; nx; pwr = pwr * 2) {
//		*m = *m + 1;
//	}
//	*twopm = pwr;
//	return(TRUE);
//}
//
//int fft_2d(COMPLEX **c, int nx, int ny, int dir) {
//	int i, j;
//	int m, twopm;
//	double *real, *imag;
//
//	/* Transform the rows */
//	real = (double *)malloc(nx * sizeof(double));
//	imag = (double *)malloc(nx * sizeof(double));
//	if (real == NULL || imag == NULL)
//		return(FALSE);
//	if (!Powerof2(nx, &amp; m, &amp; twopm) || twopm != nx)
//		return(FALSE);
//	for (j = 0; j&lt; &lt; = m;
//
//		/* N의 절반값을 half에 저장한다. */
//		half = N & gt; &gt; 1;
//
//		/* Bit Reverse를 하는 과정 */
//		for (i = 0, j = 0; i &lt; N - 1; i++) {
//			if (i &lt; j)
//			{
//				swap_x = x[i];
//					swap_y = y[i];
//					x[i] = x[j];
//				y[i] = y[j];
//				x[j] = swap_x;
//				y[j] = swap_y;
//			}
//			step = half;
//
//			while (step &lt; = j)
//			{
//				j = j - k;
//				k &gt; &gt; = 1;
//			}
//			j = j + step;
//		}
//
//	/* 초기의 값을 입력하는 부분,
//	 * factor의 역할은 각 단계(Stage)에서 omega의 값을 변경하기 위한
//	 * 기본 값이다. 즉 omega N= 4일 경우에는 각도가 pi/2 만큼씩 변하므로
//	 * exp(-j pi/2) 의 값을 가지고 있는 것이 factor이다
//	 * factor는 각 stage가 올라갈 때 마다 변경되어야 한다.
//	 * 그 각도가 절반으로 줄어야 하므로
//	 * cos(t) + j sin(t) 가 cos(t/2) + jsin(t/2) 로 되어야 한다.
//	 * cos(t) = 2cos^2(t/2) -1 이므로 cos(t/2) = sqrt((1+cos(t))/2)이고
//	 * sin(t/2) = sqrt(1-cos^2(t/2))이므로 sqrt((1-cos(t))/2) 이다.
//	 */
//	factor_x = -1.0;
//	factor_y = 0;
//	butterfly = 1;
//
//	for (stage = 0; stage &lt; m; stage++)
//	{
//		/* step 은 butterfly가 두개의 샘플을 가지고 작업하므로
//		 * 현재 위치에서 몇번째 샘플을 가지고 해야 하는 가를
//		 * 가리키는 변수이다.
//		 * butterfly는 같은 omega 값에의해 반복되는 butterfly가
//		 * 존재하므로 그 위치를 가리키기 위한 값이다.
//		 */
//		step = butterfly;
//		butterfly &lt; &lt; = 1;
//		/* 각 단계에서 omega는 항상 1+j0으로 시작한다. */
//		omega_x = 1.0;
//		omega_y = 0;
//		for (numofbutter = 0; numofbutter &lt; step; numofbutter++)
//		{
//			for (i = numofbutter; i &lt; N; i = i + butterfly)
//			{
//				/* omega_N(k) 일때 omega_N(k+N/2) = -omega_N(k)
//				 * 의 성질을 이용한다. 그래서 두번째 샘플에 미리
//				 * omega_N(k)를 곱한 후 결과의 처음 샘플에는
//				 * 그냥 더하고 두번째 샘플에는 빼준다(-이므로)
//				 */
//				swap_x = x[i + step] * omega_x - y[i + step] * omega_y;
//				swap_y = y[i + step] * omega_x + x[i + step] * omega_y;
//
//				x[i + step] = x[i] - swap_x;
//				y[i + step] = y[i] - swap_y;
//
//				x[i] = x[i] + swap_x;
//				y[i] = y[i] + swap_y;
//
//			}
//			/* 공통된 omega를 사용하는 샘플을 다 곱한 후
//			 * omega를 factor의 각도만큼 증가시켜야 한다.
//			 * omega(k+1) 의 값은 omega와 factor의 곱을 통해
//			 * 구할 수 있다.
//			 */
//			swap_x = omega_x * factor_x - omega_y * factor_y;
//			swap_y = omega_y * factor_x + omega_x * factor_y;
//			omega_x = swap_x;
//			omega_y = swap_y;
//
//		}
//		/* 한 단계가 끝났으므로 factor를 기존 값에서 각도가
//		 * 절반인 값으로 구해야 한다.
//		 */
//		factor_y = sqrt((1.0 + factor_x) / 2.0);
//		if (dir == 1)
//			factor_y = -factor_y;
//		factor_x = sqrt((1.0 - factor_x) / 2.0);
//	}
//	/* Inverse FFT 의 경우에는 sampling을 해줘야 한다.
//	 * 다른 소스에서는 FFT의 값이 크게 나오므로 fft시에 sampling을
//	 * 하고 inverse에서 안하는 경우도 있다 ( 상관없음 )
//	 */
//	if (dir == -1)
//	{
//		for (i = 0; i &lt; N; i++)
//		{
//			x[i] /= N;
//			y[i] /= N;
//		}
//	}
//
//	return TRUE;
//}
//
//
//
//====== = dft.h============
//
//typedef struct {
//	double real, imag;
//} COMPLEX;
//#define PI 3.141592
//
//void fft(COMPLEX *, int);
//void ifft(COMPLEX *, int);
//void dft(COMPLEX *, COMPLEX *, int);
//void idft(COMPLEX *, COMPLEX *, int);
//void rfft(double *, COMPLEX *, int);
//void ham(COMPLEX *, int);
//void han(COMPLEX *, int);
//void triang(COMPLEX *, int);
//void black(COMPLEX *, int);
//void harris(COMPLEX *, int);
//int log2(unsigned int);
//
//
//======== = dft.c==========
//
//#include <math.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include "dft.h"
//
//void fft(COMPLEX *x, int m)
//{
//	static COMPLEX *w;
//	static int mstore = 0;
//	static int n = 1;
//	COMPLEX u, temp, tm;
//	COMPLEX *xi, *xip, *xj, *wptr;
//	int i, j, k, l, le, windex;
//	double arg, w_real, w_imag, wrecur_real, wrecur_imag, wtemp_real;
//	if (m != mstore) {
//
//		if (mstore != 0) free(w);
//		mstore = m;
//		if (m == 0) return;
//		n = 1 << m;
//		le = n / 2;
//
//		w = (COMPLEX *)calloc(le - 1, sizeof(COMPLEX));
//		if (!w) {
//			printf("\nUnable to allocate complex W array\n");
//			exit(1);
//		}
//
//		arg = PI / le;
//		wrecur_real = w_real = cos(arg);
//		wrecur_imag = w_imag = -sin(arg);
//		xj = w;
//		for (j = 1; j < le; j++) {
//			xj->real = (double)wrecur_real;
//			xj->imag = (double)wrecur_imag;
//			xj++;
//			wtemp_real = wrecur_real * w_real - wrecur_imag * w_imag;
//			wrecur_imag = wrecur_real * w_imag + wrecur_imag * w_real;
//			wrecur_real = wtemp_real;
//		}
//	}
//
//	le = n;
//	windex = 1;
//	for (l = 0; l < m; l++) {
//		le = le / 2;
//
//		for (i = 0; i < n; i = i + 2 * le) {
//			xi = x + i;
//			xip = xi + le;
//			temp.real = xi->real + xip->real;
//			temp.imag = xi->imag + xip->imag;
//			xip->real = xi->real - xip->real;
//			xip->imag = xi->imag - xip->imag;
//			*xi = temp;
//		}
//
//
//		wptr = w + windex - 1;
//		for (j = 1; j < le; j++) {
//			u = *wptr;
//			for (i = j; i < n; i = i + 2 * le) {
//				xi = x + i;
//				xip = xi + le;
//				temp.real = xi->real + xip->real;
//				temp.imag = xi->imag + xip->imag;
//				tm.real = xi->real - xip->real;
//				tm.imag = xi->imag - xip->imag;
//				xip->real = tm.real*u.real - tm.imag*u.imag;
//				xip->imag = tm.real*u.imag + tm.imag*u.real;
//				*xi = temp;
//			}
//			wptr = wptr + windex;
//		}
//		windex = 2 * windex;
//	}
//
//	j = 0;
//	for (i = 1; i < (n - 1); i++) {
//		k = n / 2;
//		while (k <= j) {
//			j = j - k;
//			k = k / 2;
//		}
//		j = j + k;
//		if (i < j) {
//			xi = x + i;
//			xj = x + j;
//			temp = *xj;
//			*xj = *xi;
//			*xi = temp;
//		}
//	}
//}
//
//main() {
//	int i;
//	COMPLEX com[1];
//	fft(com, 1);
//	for (i = 0; i < sizeof(com); i++)
//		printf("%f\n", com[i].real);
//}

