
#include "stdafx.h"
#include "SoundBase.h"
#include "FFT_SM.h"
//




CFFT_SM::CFFT_SM()
{	
	for(int i = 0; i < 2000; i++)
	{		
		m_CheckError[i] = 0;
	}
	

}

CFFT_SM::~CFFT_SM()
{

}



int CFFT_SM::CheckHighFrequency(double *lSpectrum, double lCheckPoint, double lCheckLimit)
{
	double lsum = 0;
	double lsum1 = 0;
	double lsum2 = 0;
	double lsum3 = 0;
	double lAvg1 = 0;
	double lAvg2 = 0;
	double lAvg3 = 0;
	double lMaxVal = 0;
	int lmaxpt = 1500;
	int returnVal = 1;

	for(int i = 0; i < SAMPLE_BUFFER_MAX/2; i++)
	{
		if(lMaxVal < lSpectrum[i])
		{
			lMaxVal = lSpectrum[i];
			lmaxpt = i;
		}

		lsum += lSpectrum[i];
		//lMaxVal = max(lMaxVal, lSpectrum[i])
		m_CheckError[i] = 0;
	}

	if( CheckSideFrequencyNew(lSpectrum, 2, 10) == 0)//if( CheckSideFrequency(lSpectrum, 2, 10) == 0)
	{
		//CheckSideFrequencyNew(lSpectrum, 2, 10);
		returnVal =  11;
	}
	else
	{
		
		lAvg1 = lsum/(SAMPLE_BUFFER_MAX/2);
		if((lMaxVal - lAvg1 > 20)&&(lmaxpt > 5))
		{
			if((lmaxpt < 200)&&(lmaxpt > 0))
			{
				for(int i = 0; i < 100; i++)
				{
					lsum1 += lSpectrum[i+250];//250,-,350  300
					if(i < 100)
						lsum2 += lSpectrum[i+400];// 400,- 450 ,

					lsum3 += lSpectrum[i+550];//700,800 750
				}
				lAvg1 = lsum1/ 100;
				lAvg2 = lsum2/ 100;
				lAvg3 = lsum3/ 100;
				lAvg1 = (lAvg1 +lAvg3) /2;
				
				if((lAvg2 - lAvg1) >  0.0)
				{
			
					if((lAvg2 - lAvg1) >  lCheckLimit)
					{
						returnVal =  10;
						for(int i = 0; i < 100; i++)
						{
							m_CheckError[i+400] = 1;
						}
					}
				}
				//return 2;
			}

			//return 1;
		}
		else
		{
			returnVal =  3;
		}
	}
	return returnVal;
	
}

int CFFT_SM::CheckSideFrequency(double *lSpectrum, int lSideCount, double lCheckLimit )
{
	int lReturn = 1;
	double lsum = 0;
	double lsum1 = 0;
	//double lsum2 = 0;
	//double lsum3 = 0;
	double lAvg = 0;
	double lAvg1 = 0;
	//double lAvg2 = 0;
	//double lAvg3 = 0;

	double ldtemp[2000];


	double lMaxVal = 0;
	double lMinVal = 100;

	double lCheckVal = 100;

	int lmaxpt = 0;
	int lminpt = 0;
	int lMaxCount = 0;
	int lSlopUpCheck = 0;
	int lSlopDownCheck = 0;

	int lDownpt = 0;

	int lstartpt = 0;
	//int lstartpt = 0;

	
	//lSlopDownCheck = 1;
	int lmaxpt1 = 0;
	int lmaxpt2 = 0;
	int lmaxptTmp = 0;


	int lErrorCount = 0;


#if 1	

	for(int i = 0; i < 200; i++)
	{
		ldtemp[i] = lSpectrum[i];
	}
	
	for(int i = 0; i < 1000; i++)
	{
		if(lMaxVal < lSpectrum[i])
		{
			lMaxVal = lSpectrum[i];
			lmaxpt = i;
		}
	}

	if(lmaxpt < 80)
	{
		lSlopDownCheck = 0;

		for(int i = lmaxpt; i < 150; i++)
		{
			if(lMaxVal < lSpectrum[i])
			{
				lMaxVal = lSpectrum[i];	
				lmaxptTmp = i;
			}

			if(lMinVal > lSpectrum[i])
			{
				lMinVal = lSpectrum[i];
				lDownpt = i;
			}

			if(lSlopDownCheck == 0)
			{
				if(lSpectrum[i] - lMinVal > 3)
				{
					lSlopDownCheck = 1;
					lMaxVal = lSpectrum[i];
					lminpt = lDownpt;
				}
			}

			if(lSlopDownCheck == 1)
			{
				if((lMaxVal - lSpectrum[i] > 5)&&(lMaxVal  > 41))
				{
					lErrorCount++;
					lMinVal = lSpectrum[i];
					lSlopDownCheck = 0;
					for(int j = lminpt; j < i; j++)
					{
						m_CheckError[j] = 1;
					}
				}
			}

		}

	

	
		if(lErrorCount >= 1)
		{
		
			lReturn = 0;
		}

	}		

	

	
		
	return lReturn;

#else
	for(int i = 0; i < SAMPLE_BUFFER_MAX/2; i++)
	{
		if(lMaxVal < lSpectrum[i])
		{
			lMaxVal = lSpectrum[i];
			lmaxpt = i;
		}

		lsum += lSpectrum[i];

		if(i == 199)
			lsum1 += lsum;

		//lMaxVal = max(lMaxVal, lSpectrum[i])
	}
	lAvg = lsum/(SAMPLE_BUFFER_MAX/2);
	if((lMaxVal - lAvg > 10)&&(lmaxpt > 2))
	{
		if((lmaxpt < 100)&&(lmaxpt > 0))
		{
			
			lAvg1 = lsum1/ 200;
			for(int i = 0; i < 100; i++)
			{
				if((lSpectrum[i] < lSpectrum[i+1]) 
					&& ( lSpectrum[i+1] < lSpectrum[i+2]) 
					&& ( lSpectrum[i+2] < lSpectrum[i+3]) 
					&& ( lSpectrum[i+3] > lSpectrum[i+4]) 
					&& ( lSpectrum[i+4] > lSpectrum[i+5])
					&& ( lSpectrum[i+5] > lSpectrum[i+6])
					&& (lAvg1 < lSpectrum[i+3]))
				{
					lMaxCount++;
					if(lMaxCount > 1)
					{
						m_CheckError[i] = 1;
						m_CheckError[i+1] = 1;
						m_CheckError[i+2] = 1;
						m_CheckError[i+3] = 1;
						m_CheckError[i+4] = 1;
						m_CheckError[i+5] = 1;
					}
				}
		
			}		
		
				
		
			return 2;
		}
		return 1;
	}
	else
	{
		return 3;
	}
	if((lMaxCount) >  2)
	{			
				
		return 0;
				
	}

	return 1;
#endif
}


int CFFT_SM::CheckSideFrequencyNew(double *lSpectrum, int lSideCount, double lCheckLimit )
{
	int lReturn = 1;
	double lsum = 0;
	double lsum1 = 0;
	//double lsum2 = 0;
	//double lsum3 = 0;
	double lAvg = 0;
	double lAvg1 = 0;
	//double lAvg2 = 0;
	//double lAvg3 = 0;

	double ldtemp[2000];


	double lMaxVal = 0;
	double lMinVal = 100;

	double lCheckVal = 100;

	int lmaxpt = 0;
	int lminpt = 0;
	int lMaxCount = 0;
	int lSlopUpCheck = 0;
	int lSlopDownCheck = 0;

	int lDownpt = 0;

	int lstartpt = 0;
	//int lstartpt = 0;
		
	//lSlopDownCheck = 1;
	int lmaxptSide[15] = {0,0,0,0,0};
	int lmaxpt2 = 0;
	int lmaxptTmp = 0;
	
	int lErrorCount = 0;
	
#if 1	

	for(int i = 0; i < 200; i++)
	{
		ldtemp[i] = lSpectrum[i];
	}
	
	for(int i = 0; i < 1000; i++)
	{
		if(lMaxVal < lSpectrum[i])
		{
			lMaxVal = lSpectrum[i];
			lmaxpt = i;
		}
	}
	lmaxptSide[0] = lmaxpt;
	if(lmaxpt < 80)
	{
		lSlopDownCheck = 0;

		for(int i = lmaxpt; i < 180; i++)
		{
			if(lMaxVal < lSpectrum[i])
			{
				lMaxVal = lSpectrum[i];	
				lmaxptTmp = i;
			}

			if(lMinVal > lSpectrum[i])
			{
				lMinVal = lSpectrum[i];
				lDownpt = i;
			}

			if(lSlopDownCheck == 0)
			{
				if(lSpectrum[i] - lMinVal > 3)
				{
					lSlopDownCheck = 1;
					lMaxVal = lSpectrum[i];
					lminpt = lDownpt;
				}
			}

			if(lSlopDownCheck == 1)
			{
				if(lMaxVal - lSpectrum[i] > 5)
				{
					lErrorCount++;
					lmaxptSide[lErrorCount] = lmaxptTmp;
					if(lErrorCount >= 4)
					{
						break;
					}
					lMinVal = lSpectrum[i];
					lSlopDownCheck = 0;
					for(int j = lminpt; j < i; j++)
					{
						m_CheckError[j] = 1;
					}
				}
			}
		}	
			
		if((lErrorCount >= 2)&&(lmaxpt < 60)&&(lmaxpt > 10))
		{				
			if((abs(lmaxpt*2 - lmaxptSide[1]) < 10)
				&&(abs(lmaxpt*3 - lmaxptSide[2]) < 10))			
			{
			
				lReturn = 0;
			}
			else if(lErrorCount >= 2)
			{
				lReturn = 0;
			}
		}
	}		

	return lReturn;


#else
	for(int i = 0; i < SAMPLE_BUFFER_MAX/2; i++)
	{
		if(lMaxVal < lSpectrum[i])
		{
			lMaxVal = lSpectrum[i];
			lmaxpt = i;
		}

		lsum += lSpectrum[i];

		if(i == 199)
			lsum1 += lsum;

		//lMaxVal = max(lMaxVal, lSpectrum[i])
	}
	lAvg = lsum/(SAMPLE_BUFFER_MAX/2);
	if((lMaxVal - lAvg > 10)&&(lmaxpt > 2))
	{
		if((lmaxpt < 100)&&(lmaxpt > 0))
		{
			
			lAvg1 = lsum1/ 200;
			for(int i = 0; i < 100; i++)
			{
				if((lSpectrum[i] < lSpectrum[i+1]) 
					&& ( lSpectrum[i+1] < lSpectrum[i+2]) 
					&& ( lSpectrum[i+2] < lSpectrum[i+3]) 
					&& ( lSpectrum[i+3] > lSpectrum[i+4]) 
					&& ( lSpectrum[i+4] > lSpectrum[i+5])
					&& ( lSpectrum[i+5] > lSpectrum[i+6])
					&& (lAvg1 < lSpectrum[i+3]))
				{
					lMaxCount++;
					if(lMaxCount > 1)
					{
						m_CheckError[i] = 1;
						m_CheckError[i+1] = 1;
						m_CheckError[i+2] = 1;
						m_CheckError[i+3] = 1;
						m_CheckError[i+4] = 1;
						m_CheckError[i+5] = 1;
					}
				}
		
			}		
		
				
		
			return 2;
		}
		return 1;
	}
	else
	{
		return 3;
	}
	if((lMaxCount) >  2)
	{			
				
		return 0;
				
	}

	return 1;
#endif
}

int CFFT_SM::CheckFreqVol(double *lWave,  double *lFreq, double *lvol )
{
	//SAMPLE_BUFFER_MAX
	double labsSum = 0;
	double lstepPrev = 0;
	double lFreqCheck = 0;
	double lVoltCheck = 0;
	int lCrossCount = 0;

	for(int  i = 0; i < SAMPLE_BUFFER_MAX; i++)
	{
		labsSum += abs(lWave[i]);
		if(lWave[i] < -1)
		{
			lstepPrev = 1;
		}
		else if(lWave[i] > 1)
		{
			if(lstepPrev == 1)
			{
				lstepPrev = 0;
				lCrossCount++;
			}
		}

	}
	lFreqCheck = (double)lCrossCount * SAMPLE_RATE_PER_SECOND / SAMPLE_BUFFER_MAX;
	lVoltCheck = labsSum / SAMPLE_BUFFER_MAX / 3277.5;

	*lFreq =lFreqCheck;
	*lvol = lVoltCheck;

	return 1;
}
void CFFT_SM::CheckData(double *lWave, double *DisplaylWave, double *lFreq, double *lvol)
{

	double     lbuf[4000];
	double lMinCmp = 0, lmaxCmp = 0;

	double lMinSum = 0, lmaxSum = 0, lAverage = 0;
	double lMin[300], lmax[300];
	int lMinCnt = 0, lmaxCnt = 0;
	double PPVoltage = 0;
	double Frequency = 0;

	int lStartNum = 0, lEndNum = 0, lCheckCrossCnt = 0;
	int lSignalError = 0;
	int i;
	int lmaxEnable = 0;
	int lminEnable = 0;



	double CmpCoefFwd = 0, CmpCoefBak = 0;
	//////////////////////////////////////////////////
		//
#ifdef TESTMODE_LEVEL_CHANGE	
	m_data[CheckID].SetSize(1, 6000);
	for (i = 0; i < 6000; i++)
	{
		m_data[CheckID](0, i) = 1.0*sin(3.141592 / 500 * i)*(CheckID / 12.0 + 1);
	}
#endif
	//////////////////////////////////////
	lmaxSum = 0;
	for (i = 0; i < SAMPLE_BUFFER_CHECK_MAX; i++)
	{
		lmaxSum += abs(lWave[i]);
	}
	lAverage = lmaxSum / (SAMPLE_BUFFER_CHECK_MAX);

	//CmpCoef1 = data[CheckID][0];
	memset(lMin, 0, sizeof(lMin));
	memset(lmax, 0, sizeof(lmax));
	//lMinCmp = -0.2;
	//lmaxCmp = 0.2;
	//m_data
	lMinSum = 0;
	lmaxSum = 0;
	lminEnable = 1;
	lmaxEnable = 1;

	double lchecklimit = lAverage;//*0.5;
	if (lchecklimit < 0.01)
		lchecklimit = 0.011;

	//lchecklimit = lchecklimit * 2;

	for (i = 0; i < SAMPLE_BUFFER_CHECK_MAX - 3; i++)
	{
		CmpCoefBak = CmpCoefFwd;
		CmpCoefFwd = lWave[i] + lWave[i + 1] + lWave[ i + 2] + lWave[ i + 3];
		//	if((-0.00 > CmpCoefFwd)&&(lmaxCmp >= lAverage*0.1)&&(lminEnable && lmaxEnable))
		//	if((-0.00 > CmpCoefFwd)&&(lmaxCmp >= lAverage*0.5)&&(lminEnable && lmaxEnable))
		if ((-0.00 > CmpCoefFwd) && (lmaxCmp >= lchecklimit) && (lminEnable && lmaxEnable))
		{
			if (lStartNum == 0)
			{
				lMinCnt = 0;
				lmaxCnt = 0;
				lStartNum = i;
			}
			else //if(lminEnable && lmaxEnable)
			{
				lMin[lMinCnt] = lMinCmp;
				lmax[lmaxCnt] = lmaxCmp;
				lMinCnt++;
				lmaxCnt++;
			}

			//if(lminEnable && lmaxEnable)
			{
				lCheckCrossCnt++;
				if (lCheckCrossCnt >= 200)
				{
					lSignalError = 1;
					break;
				}
				lEndNum = i;
			}
			lMinCmp = 0;
			lmaxCmp = 0;

			lmaxEnable = 0;
			lminEnable = 0;
		}
		else
		{
			//	if((lMinCmp > CmpCoefFwd)&&(CmpCoefFwd <= -lAverage*2))
			if ((lMinCmp > CmpCoefFwd) && (CmpCoefFwd < 0))
			{
				lMinCmp = CmpCoefFwd;
				lminEnable = 1;
			}
			//		if((lmaxCmp < CmpCoefFwd)&&(CmpCoefFwd >= lAverage*2))
			if ((lmaxCmp < CmpCoefFwd) && (CmpCoefFwd > 0))
			{
				lmaxCmp = CmpCoefFwd;
				lmaxEnable = 1;
			}

		}
	}
	if (lCheckCrossCnt < 2)
	{
		lSignalError = 1;
		PPVoltage = lmaxCmp - lMinCmp;
	}
	else
	{
		for (i = 0; i < lMinCnt; i++)
		{
			//lmaxSum += lmax[i];
			lMinSum += lMin[i];
		}

		for (i = 0; i < lmaxCnt; i++)
		{
			lmaxSum += lmax[i];
			//lMinSum += lMin[i];
		}

		PPVoltage = (lmaxSum / lmaxCnt - lMinSum / lMinCnt) * 100 / 1.798;
	}

	if (lSignalError)
	{
		Frequency = 0;
	}
	else
	{
		//	Frequency = 100000.0 / (lEndNum - lStartNum) * (lCheckCrossCnt-1);
		Frequency = SAMPLE_RATE_PER_SECOND / (lEndNum - lStartNum + 0.01) * (lCheckCrossCnt -1);

	}
	/*	NI_SAMPLE_RATE
	 NI_SAMPLE_COUNT	*/

	
	*lFreq   = Frequency*1.0;	
	*lvol= PPVoltage;
	//m_PPVoltage[CheckID] = PPVoltage;
	//m_Frequency[CheckID] = Frequency;

	//	M_Vol[CheckID].Format(_T("P%02d : %4.0fmV"), CheckID, PPVoltage);
	//	M_Frq[CheckID].Format(_T("F%02d : %4.0fHz"), CheckID, Frequency);
	//UpdateData(FALSE);

	if ((lStartNum < 0) || (lStartNum >= SAMPLE_BUFFER_CHECK_MAX - SAMPLE_DISPLAYCOUNT - 50))
	{
		lStartNum = 0;
	}

	int j;

	j = 0;
	lmaxSum = 0;


	//Check_data[CheckID].SetSize(1, NI_CHECK_COUNT);
	

	for (i = lStartNum; i < lStartNum + SAMPLE_DISPLAYCOUNT; i++)
	{
		lmaxSum = 0;
		//for (int k = 0; k < 20; k++)
		//{
		//	lmaxSum += lWave[i + k];
		//}
		//		
		//DisplaylWave[j] = lmaxSum / 20;	
		DisplaylWave[j] = lWave[i];
		j++;
	}

	lmaxSum = 0;
	for (i = 0; i < SAMPLE_DISPLAYCOUNT; i++)
	{
		lmaxSum += DisplaylWave[i];
	}
	lAverage = lmaxSum / SAMPLE_DISPLAYCOUNT;

	for (i = 0; i < SAMPLE_DISPLAYCOUNT; i++)
	{
		DisplaylWave[i] -= lAverage;
	}
	//lmaxSum = 0;
	//for (i = 0; i < NI_CHECK_COUNT; i++)
	//{
	//	if (lmaxSum < Check_data[CheckID](0, i))//m_Frequency[CheckID] > 25000)
	//	{
	//		lmaxSum = Check_data[CheckID](0, i);
	//	}

	//	if (lMinSum > Check_data[CheckID](0, i))//m_Frequency[CheckID] > 25000)
	//	{
	//		lMinSum = Check_data[CheckID](0, i);
	//	}
	//}
	//if (((lmaxSum - m_PPVoltage[CheckID] / 2) > m_PPVoltage[CheckID] / 2 / 5) && ((lMinSum - m_PPVoltage[CheckID] / 2) > m_PPVoltage[CheckID] / 2 / 5))
	//{
	//	m_NoiseFlag[CheckID] = 1;
	//}
	//else
	//{
	//	m_NoiseFlag[CheckID] = 0;
	//}

	//if ((lmaxSum < m_PPVoltage[CheckID] / 2000) && (m_PPVoltage[CheckID] > 20) && (m_Frequency[CheckID] > 1500))
	//{
	//	double lScaleCoef = 0;
	//	lScaleCoef = m_PPVoltage[CheckID] / lmaxSum / 2000;

	//	for (i = 0; i < NI_CHECK_COUNT; i++)
	//	{
	//		Check_data[CheckID](0, i) = Check_data[CheckID](0, i) * lScaleCoef;
	//	}
	//}

}

int CFFT_SM::CheckStartSweep(double *lSpectrum, double lFreq, double lVolume )
{//3277
	double lsum = 0;
	double lAvg = 0;
	//for(int i = 0; i <50; i++)
	//{		

	//	lsum += lSpectrum[i];

	//	
	//}
	//lAvg = lsum/50.0;
	//if(lAvg > 35)
	//{
	//	return 1;
	//}


	if(lVolume > 0.4)
	{
		if((lFreq < 500)&& (lVolume > 0.4))
		{
			return 1;
		}
	}

	return 0;
}
int CFFT_SM::CheckEndSweep(double lFreq, double lVolume )
{
	if (lVolume < 0.1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

int CFFT_SM::CheckChart(_AudSignal_T *lp_WaveSignal, _AudSignal_T *lp_RefWaveSignal) 
{
	return 1;
}
 /**********************************************************************
  fft - In-place radix 2 decimation in frequency FFT
  Requires pointer to complex array, x and power of 2 size of FFT, m
  (size of FFT = 2^m).  Places FFT output on top of input COMPLEX array.
  void fft(COMPLEX *x, int m)
  ***********************************************************************/

void CFFT_SM::fft(COMPLEX *x, int m)
{
	static COMPLEX *w;           // used to store the w complex array
	static int mstore = 0;       // stores m for future reference
	static int n = 1;            // length of fft stored for future

	COMPLEX u, temp, tm;
	COMPLEX *xi, *xip, *xj, *wptr;

	int i, j, k, l, le, windex;

	double arg, w_real, w_imag, wrecur_real, wrecur_imag, wtemp_real;

	if (m != mstore)
	{
		// free previously allocated storage and set new m
		if (mstore != 0) free(w);
		mstore = m;
		if (m == 0) return;       // if m=0 then done

 // n = 2^m = fft length
		n = 1 << m;
		le = n / 2;  //difference between the upper and lower leg indices

 // allocate the storage for w
		w = (COMPLEX *)calloc(le - 1, sizeof(COMPLEX));

		// calculate the w values recursively 계산
		arg = PI / le;         //  PI/le calculation
		wrecur_real = w_real = cos(arg);
		wrecur_imag = w_imag = -sin(arg);
		xj = w;
		for (j = 1; j < le; j++)
		{
			xj->real = (double)wrecur_real;
			xj->imag = (double)wrecur_imag;
			xj++;
			wtemp_real = wrecur_real * w_real - wrecur_imag * w_imag;
			wrecur_imag = wrecur_real * w_imag + wrecur_imag * w_real;
			wrecur_real = wtemp_real;
		}
	}
	// start fft 푸리에 변환 시작
	le = n;
	windex = 1;
	for (l = 0; l < m; l++)
	{
		le = le / 2;
		// first iteration with no multiplies
		for (i = 0; i < n; i = i + 2 * le)
		{
			xi = x + i;
			xip = xi + le;
			temp.real = xi->real + xip->real;
			temp.imag = xi->imag + xip->imag;
			xip->real = xi->real - xip->real;
			xip->imag = xi->imag - xip->imag;
			*xi = temp;
		}
		// remaining iterations use stored w
		wptr = w + windex - 1;
		for (j = 1; j < le; j++)
		{
			u = *wptr;
			for (i = j; i < n; i = i + 2 * le)
			{
				xi = x + i;
				xip = xi + le;
				temp.real = xi->real + xip->real;
				temp.imag = xi->imag + xip->imag;
				tm.real = xi->real - xip->real;
				tm.imag = xi->imag - xip->imag;
				xip->real = tm.real*u.real - tm.imag*u.imag;
				xip->imag = tm.real*u.imag + tm.imag*u.real;
				*xi = temp;
			}
			wptr = wptr + windex;
		}
		windex = 2 * windex;
	}
	// rearrange data by bit reversing
	j = 0;
	for (i = 1; i < (n - 1); i++)
	{
		k = n / 2;
		while (k <= j)
		{
			j = j - k;
			k = k / 2;
		}
		j = j + k;
		if (i < j)
		{
			xi = x + i;
			xj = x + j;
			temp = *xj;
			*xj = *xi;
			*xi = temp;
		}
	}
}

/**************************************************************************
 log2 - base 2 logarithm
 Returns base 2 log such that i = 2^ans where ans = log2(i).
 if log2(i) is between two values, the larger is returned.
 int log2(unsigned int x)
 *************************************************************************/
int CFFT_SM::log2(unsigned int x)
{
	unsigned int mask, i;

	if (x == 0) return(-1);      // zero is an error, return -1
	x--;                        // get the max index, x-1
	for (mask = 1, i = 0; ; mask *= 2, i++)
	{
		if (x == 0) return(i);   // return log2 if all zero
		x = x & (~mask);        // AND off a bit
	}

}

