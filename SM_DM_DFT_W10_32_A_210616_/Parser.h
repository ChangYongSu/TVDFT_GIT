#ifndef __PARSER_H__
#define __PARSER_H__

class CStep;

enum TokenType
{
    TNULL = 0, LP, RP, BRACE, IDENTIFIER, COMMA, INTEGER, REAL
	, STRING, FINISHED, KEYWORD, COMMENT, DELIMITER, UNIT, SIGN
	, INTARRAY, END_OF_FILE
};

//+ 2007.10.11 Add BY USY
enum SeqDataType
{  NULL_TYPE = 0 , NO_EDIT_TYPE , NORMAL_EDIT_TYPE , FILE_SEL_TYPE , REMOTE_SEL_TYPE	,
   ROI_SEL_TYPE , AUDIO_SRC_SEL_TYPE ,  GRABBER_SEL_TYPE , RF_SEL_TYPE , VIDEO_SRC_SEL_TYPE ,
   USE_SEL_TYPE , PATTERN_CMD_SEL_TYPE, AVSWITCH_CMD_SEL_TYPE, NUM_SEL_TYPE, ECHO_SEL_TYPE
};
//-

enum ErrorType
{
    MISSING_PAREN, MISSING_BRACE, UNDEFINED_SYMBOL, NOT_NUMBER, 
    MISSING_QUOTE, TYPE_MISMATCH, EXPECTED_COMMA, STEP_REDEFINITION,
    MACRO_REDEFINITION
};

typedef enum 
{
    FUNCTION_TYPEERROR, PROCESS_FUNCTION, CHECK_FUNCTION, ADJUST_FUNCTION, MANUAL_ADJUST_FUNCTION
}FuncionType;


typedef enum
{
	HEADERERROR, STEPHEADER,ADC_TYPE
} HEADERTYPE;

typedef enum 
{
    MEAS_TYPEERROR, MEAS_BOOL, MEAS_INTEGER, MEAS_DOUBLE, MEAS_STRING
}MeasFuncionType;

typedef struct tag_Token
{
    int type;       
    union 
    {
        char c;
		BOOL b;
        int i;
        float f;
        char s[128];
    } value;
} Token;

#define MAXLINE 2000

BOOL		IsDelimiter(char c);
BOOL		IsWhite(char c);
BOOL		GetToken();
BOOL		GetFloat();
int			LoadFile(CString sPath);
BOOL		ParseStep(int nStepCnt);
void		SyntexError(int line);
BOOL		ParseBlock(CStep* pStep);
BOOL		SyntexChecking(char* achFunc);
HEADERTYPE	HeaderChecking(char* pszString);
FuncionType FunctionTypeChecking(char* pszString);
BOOL		Prescan(CString sPath);
//+ 2007.10.2 Add BY USY
BOOL		OpenFullSeq(CString sPath);
int			LoadFullSeq(CString sPath);
void		DeleteFullSeq();
void		FreeUserFile(int nTotalLine);
CStep*		FindFullSeqStep(int nStepNo);
BOOL		ParseFullSeqStep(int nStepCnt, int nTotalLine);
BOOL		GetTokenFullSeq(int nTotalLine);
BOOL		ParseFullSeqBlock(CStep* pStep, int nTotalLine);
//-
//+ 2007.10.4 Add BY USY
void		GetAudioValue(CStep* pStep);
BOOL		CheckSource(CString sSource);
//-

CStep*		FindStep(int nStepNo);
void		DeleteStepList();

void		FreeUserFile();
BOOL		AnalyzeIntArray();
long		GetModifyTime(CString sPath);
void		MakeFunctionList(CStep* pStep, char* fun);
void		DeleteRemoteList();

//+add kwmoon 071214
void		DeletePatternList();

BOOL		SaveSeqFile(CString sSeqPath);
CString		MakeStepHeader(CStep* pStep);
BOOL		MakeSubStep(CStep* pStep, CStringArray& aArray);
CString		MakeStepTail(CStep* pStep);
void		SortSubStep(CStep* pStep);

//+add PSH 080410
BOOL GetADC_AdjType();

//+add kwmoon 081024
void		DeleteModelList();

#endif	// __PARSER_H__