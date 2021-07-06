#ifndef __STEP_H_
#define __STEP_H_

#include "Global.h"

typedef union argumentTag
{
	int		Integer;
	double	Double;
	char	String[256];		// Max 128 characters
	BOOL	Boolean;
} Argument;

class CFunction:public CObject
{
public :
	CFunction()
	{
	}
	~CFunction()
	{
		POSITION pos = m_ArgumentList.GetHeadPosition();
		while(pos != NULL)
		{
			delete m_ArgumentList.GetNext(pos);
		}
		m_ArgumentList.RemoveAll();
	}
// Attributes
public :
	BOOL (*m_pFunc)();
	CTypedPtrList<CPtrList, Argument*> m_ArgumentList;
	CString			m_strCommant;
};

typedef struct tag_InternalFunction
{
    char* pszFunc;                      // function name
    BOOL (*pFunc)();					// pointer to the function
    char achPara[31];                   // argument prototype list
	char* pszMean[20];
	//+ 2007.10.11 Add BY USY
	char argType[31];					// Argument Type 
	//-
} _InternalFunction;

typedef struct tag_MeasureFunction
{
    char*  pszFunc;                      // function name
	double (*pFunc)();					// pointer to the function
    int    FuncType;                   // Function Type
} _MeasureFunction;

typedef struct tag_NameTable
{
    char*  pszName;                      // function name
	int	   nValue;					// pointer to the function
    int    nType;                   // Function Type
} _NameTable;


//+ 2007.7.24 Add BY USY
typedef struct tag_InputSource
{
    char*  pszName;                 // Input Source Name
//	int	   nValue;					// pointer to the function
//  int    nType;                   // Function Type
} _InputSource;
//-

//+ 2007.7.27 Add BY USY
typedef struct tag_Category
{
	char* pszName;					// Category
} _Category;

typedef double (*FPN)();

class CStep:public CObject
{
public :
	CStep()
	{
		m_bResult = TRUE;		// Test Result
		m_nStep = 0;
		m_sName = _T("");
		m_sUnit = _T("");
		m_fElapsedTime = 0.0;
		m_fNominal = 0.0;
		m_bNominal = TRUE; 
		m_fHighLimit = 0.0;
		m_fLowLimit = 0.0;
		m_fMeasured = -1;
		m_bMeasured = -1;
		m_nFuncType = 0;
		m_nStepType = 0;
		m_nTestType			= 0;

		m_fMeasured_L = 0.0;
		m_fMeasured_R = 0.0;
	
		//+add kwmoon 080131
		m_nTestParam	= 0;

		m_strGrabPath	= _T("");
		m_strRefPath	= _T("");
		m_strProcPath	= _T("");
		m_strMaskPath	= _T("");
		
		m_strCurSrc = _T("");
		m_strNextSrc = _T("");

		m_strFuncName = _T("");
	
		m_bTest = TRUE;
		
		//090324
		m_bRunVideoTest  = FALSE;
		m_bRunAudioTest		= FALSE;
		m_bAudioTestResult	= FALSE;
		m_bVideoTestResult	= FALSE;
		m_bAdcTestResult	= FALSE;
		m_bRunAdcTest		= FALSE;
		
		m_nAudioSource = 0;
		
		for(int nTmp = 0 ; nTmp <2; nTmp++)
		{
			m_nLeftFreq[nTmp]	= 0;
			m_nRightFreq[nTmp]	= 0;
			m_nLeftLevel[nTmp]	= 0;
			m_nRightLevel[nTmp] = 0;
		}
		
		m_nAudioMargin = 0;

		
		m_nBitCount				= 0;
	
		m_nTotalNoErrorPixel	= 0;
		m_nTotalNoMaskedPixel	= 0;
		m_nTotalNoUnmaskedPixel	= 0;

		m_strMsg = _T("");
		
		for(int i=0; i<3; i++)
		{
			for(int j=0; j<NO_DIFFERENT_POINT_BLOCK; j++) m_aNoDifferentPoint[i][j] = -1;
		}

		m_nSecondDifferentPointFreePercent = -1;

		m_bStepRetryFlag = FALSE;

		//+add kwmoon 071224
		m_bStepInternalRetryFlag = FALSE;

		//+add kwmoon 071211
		m_bMakeRefDone = FALSE;

		//+add kwmoon 080710
		m_dwCheckAudioTickCount = 0;

		//+add kwmoon 080716
		for(int i=0; i<5; i++)
		{
			m_nNoBandErrorPixel[i]			= 0;
			m_nNoAllowedBandErrorPixel[i]	= 0;

			m_fBandErrorPixelPercentage[i]			= 0.0f;
			m_fAllowedBandErrorPixelPercentage[i]	= 0.0f;
		}

		//+add kwmoon 080715
		m_nNoRoiPixel	 = 0;
		m_nNoTargetPixel = 0;
		m_nNoGoodPixel	 = 0;
		m_fGoodPixelPercentage = 0.0f;

		//+add kwmoon 081013
		m_bAdcValueCheckStep = FALSE;
		m_szAdcValue		 = _T("");

		//+add psh 090306
		m_nMeasureAudioType = -1;
		m_bRemocon = FALSE;
	}

	~CStep()
	{
		POSITION pos = m_InstructionList.GetHeadPosition();
		while(pos != NULL)
		{
			delete m_InstructionList.GetNext(pos);
		}
		m_InstructionList.RemoveAll();
	}
// Attributes
public :
	CTypedPtrList<CPtrList, CFunction*> m_InstructionList;
	BOOL			m_bResult;
	int 			m_nStep;			// Step Number
	CString			m_sName;			// Step NAME
	CString			m_sUnit;
	double			m_fElapsedTime;
	double			m_fNominal;			// Target Value : 
	BOOL			m_bNominal;			// Target Value : FALSE / TRUE
	double			m_fHighLimit;
	double			m_fLowLimit;
	double			m_fMeasured;
	BOOL			m_bMeasured;		
	int				m_nFuncType;		// MEAS_BOOL or MEAS_DOUBLE
	double			(*m_pFunc)();
	int				m_nStepType;		// CHECK_FUNCTION or PROCESS_FUNCTION
	int				m_nTestType;		// POSITION_TEST / SIZE_TEST / RGB_TEST / BRIGHT_TEST / OSD_TEST / NOISE_TEST / SIGNAL_LINE_TEST

	double			m_fMeasured_L;
	double			m_fMeasured_R;

	//+add kwmoon 080131
	int				m_nTestParam;
	//-

	CString			m_strFuncName;		// MAIN STEP Function NAME
	CString			m_strGrabPath;
	CString			m_strRefPath;
	CString			m_strProcPath;
	CString			m_strMaskPath;
	CString			m_strMsg;

	CString			m_strCurSrc;
	CString			m_strNextSrc;
	BOOL			m_bTest;		//False  : Skip Step, True : Test Step
	BOOL			m_bRunAudioTest;
	//090324
	BOOL			m_bRunVideoTest;
	BOOL			m_bAudioTestResult;
	BOOL			m_bVideoTestResult;
	//+add psh 090306
	BOOL			m_nMeasureAudioType;

	//+ 2007.12.5 Add BY USY
	BOOL			m_bRunAdcTest;
	BOOL			m_bAdcTestResult;
	//-
	BOOL			m_bRemocon;

	////Audio Value
	//+ 2007.10.8 Add BY USY
	int				m_nAudioSource;			// Audio Source 
	//-
	int				m_nLeftFreq[2];			// 0 : Left Freq Target, 1 : Left Freq Meas
	int				m_nRightFreq[2];		// 0 : Right Freq Target, 1 : Right Freq Measure

	int				m_nLeftLevel[2];		// 0 : Left Level Target, 1 : Left Level Measure
	int				m_nRightLevel[2];		// 0 : Right Level Target, 1 : Right Level Measure
	int				m_nAudioMargin;
	//-

	int m_aNoDifferentPoint[3][NO_DIFFERENT_POINT_BLOCK];
	int m_nSecondDifferentPointFreePercent;

	int m_nBitCount;
	int	m_nTotalNoErrorPixel;
	int m_nTotalNoMaskedPixel;
	int m_nTotalNoUnmaskedPixel;

	int m_aRefNoHighBit[3][MAX_NO_COLOR_BITS];
	int m_aGrabNoHighBit[3][MAX_NO_COLOR_BITS];
	
	int m_aRefNoHighBitPair[3][MAX_NO_COLOR_BITS-1];
	int m_aGrabNoHighBitPair[3][MAX_NO_COLOR_BITS-1];
	
	int m_aRefNoLowBitPair[3][MAX_NO_COLOR_BITS-1];
	int m_aGrabNoLowBitPair[3][MAX_NO_COLOR_BITS-1];

	//+add kwmoon 070929
	BOOL m_bStepRetryFlag;
	
	//+add kwmoon 071224
	BOOL m_bStepInternalRetryFlag;

	//+add kwmoon 071211
	BOOL m_bMakeRefDone;


	//+add kwmoon 080710
	DWORD m_dwCheckAudioTickCount;


	//+add kwmoon 080716
	int	  m_nNoRoiPixel;
	int	  m_nNoTargetPixel;
	int   m_nNoGoodPixel;
	int   m_nNoBandErrorPixel[5];
	int   m_nNoAllowedBandErrorPixel[5];
	float m_fBandErrorPixelPercentage[5];
	float m_fAllowedBandErrorPixelPercentage[5];
	float m_fGoodPixelPercentage;

	//+add kwmoon 081013
	BOOL	m_bAdcValueCheckStep;
	CString	m_szAdcValue;

	CString			m_strCommant;
	CString			m_strInCommant;

protected :                      
	
// Operations
public :
	void SetStepNo(int number)
	{ 
		m_nStep = number; 
	}
	void SetElapsedTime(double ttime)
	{ 
		m_fElapsedTime = ttime; 
	}
	void SetResult(BOOL result)
	{ 
		m_bResult = result; 
	} 
	void SetStepName(CString name)
	{ 	
		m_sName = name; 
	}
	void SetNominal(double fNominal)
	{
		m_fNominal = fNominal;
	}
	void SetNominal(BOOL bNominal)
	{
		m_bNominal = bNominal;
	}
	void SetHighLimit(double fHigh)
	{
		m_fHighLimit = fHigh;
	}
	void SetLowLimit(double fLow)
	{
		m_fLowLimit = fLow;
	}
	void SetUnit(CString s)
	{
		m_sUnit = s;
	}
	void SetFuncType(int nFuncType)
	{
		m_nFuncType = nFuncType;
	}
	void SetFunction(double (*pFunc)())
	{
		m_pFunc = pFunc;
	}
	//+ 2007.7.24 Add BY USY
	void SetCurrentSource(CString s)
	{
		m_strCurSrc = s;
	}
	void SetNextSource(CString s)
	{
		m_strNextSrc = s;
	}
//	//+ 2007.5.9 Add BY USY
//	void SetInputSource(CString s)
//	{
//		m_strInputSource = s;
//	}
//	//-
	//-
	//+ 2007.5.16 Add BY USY
	void SetFuncName(CString s)
	{
		m_strFuncName = s;
	}
	//-

	//+ 2007.5.25 Add BY USY
	void SetTest(BOOL bTest)
	{
		m_bTest = bTest;
	}
	//-
	
	//+ 2007.7.23 Add BY USY
	void SetMsg(CString s)
	{
		m_strMsg = s;
	}
	//-
	
	//+ 2007.9.13 Add BY USY
	void SetStepType(int nStepType)
	{
		m_nStepType = nStepType;
	}
	void SetTestType(int nTestType)
	{
		m_nTestType = nTestType;
	}
	//-
	
	//+add kwmoon 080131
	void SetTestParm(int nTestParam)
	{
		m_nTestParam = nTestParam;
	}

	int GetTestParm()
	{
		return m_nTestParam;
	}
	//-

// Helper functions
public :                    
	int GetStepNo(void)
	{ 
		return m_nStep; 
	}
    double GetElapsedTime(void)
	{ 
		return m_fElapsedTime; 
	}
    BOOL GetResult(void)
	{ 
		return m_bResult; 
	} 
    CString GetStepName(void)
	{ 
		return m_sName; 
	}
	double GetNominal_float(void)
	{
		return m_fNominal;
	}
	BOOL GetNominal_Bool(void)
	{
		return m_bNominal;
	}
	double GetHighLimit(void)
	{
		return m_fHighLimit;
	}
	double GetLowLimit(void)
	{
		return m_fLowLimit;
	}
	double GetMeasured_float(void)
	{
		return m_fMeasured;
	}
	BOOL GetMeasured_Bool(void)
	{
		return m_bMeasured;
	}
	int GetFuncType(void)
	{
		return m_nFuncType;
	}
	FPN GetFunction()
	{
		return m_pFunc;
	}
	CString GetUnit(void)
	{
		return m_sUnit;
	}
    double GetGap(void)
    {
        return m_fNominal - m_fMeasured;
    }
	//+ 2007.7.24 Mod BY USY
	CString GetCurrentSource(void)
	{
		return m_strCurSrc;
	}
	CString GetNextSource(void)
	{
		return m_strNextSrc;
	}
	//+ 2007.5.9 Add BY USY
//	CString GetInputSource(void)
//	{
//		return m_strInputSource;
//	}
//	//-
	//-
	//+ 2007.5.16 Add BY USY
	CString GetFuncName(void)
	{
		return m_strFuncName;
	}

	//+ 2007.5.25 Add BY USY
	BOOL GetTest(void)
	{
		return m_bTest;
	}
	//-
	//+ 2007.7.23 Add BY USY
	CString GetMsg(void)
	{
		return m_strMsg;
	}
	//-

	//+ 2007.9.13 Add BY USY
	int GetStepType(void)
	{
		return m_nStepType;
	}

	int GetTestType(void)
	{
		return m_nTestType;
	}
	//-
	// Operator overloading
	CStep& operator=(CStep& s)
	{
		m_nStep = s.m_nStep;
		m_sName = s.m_sName;
		m_bResult = s.m_bResult;		
		m_sUnit = s.m_sUnit;
		m_fNominal = s.m_fNominal;
		m_fHighLimit = s.m_fHighLimit;
		m_fLowLimit = s.m_fLowLimit;

		return *this;
	}
};
	
#endif	// __STEP_H	
		