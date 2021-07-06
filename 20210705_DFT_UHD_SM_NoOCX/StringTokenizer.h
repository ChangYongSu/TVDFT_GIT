// StringTokenizer.h: interface for the CStringTokenizer class.
//
//////////////////////////////////////////////////////////////////////
// Please do not remove the folowing comment !!!
// This code is writen by Zoly Farkas 
// You may use this code free
// I used as model for this code the streamtokenizer class from Sun's java
// Please if any bug found send a mail to zolyfarkas@usa.net
// Thanks










#if !defined(AFX_STRINGTOKENIZER_H__5710B523_A492_11D2_8AB8_00002145DF63__INCLUDED_)
#define AFX_STRINGTOKENIZER_H__5710B523_A492_11D2_8AB8_00002145DF63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// String Tokenizer class

#define NCHAR 255
// defining of the Character types

#define CT_WHITESPACE	1
#define CT_DIGIT	2
#define CT_ALPHA	4
#define CT_QUOTE	8
#define CT_COMMENT	16
// now should be defined the token types
#define TT_EOF	-1
#define TT_EOL	'\n'
#define TT_INTEGER	-2
#define TT_REAL	-3
#define TT_WORD	-4
#define TT_STRING -5

#define BSIZE  20


class CStringTokenizer : public CObject  
{

public:
	CStringTokenizer(CString& string);	// Constructor
	virtual ~CStringTokenizer();		// Destructor

private:
	TCHAR GetChar()
	{ return m_sString[m_iChar++]; } // inline function

	CString m_sString;		// the tokenized string
	BOOL m_bSlSlComments;	// Slash slash comments enabled
	BOOL m_bSlStComments;	// Slash star comments enabled
	BOOL m_bPascalComments;
	BOOL m_bEolIsSignificant;
	BOOL m_bForceLower;
	BOOL m_bPushedBack;
	int m_peekc;	
	BYTE m_ctType[NCHAR+1];		// table vhere are represented the character types
	int m_iLineNo;
	int m_tType	;		// The last read token type
	int m_iChar;
	// the value of the token
protected:
	CString m_sVal;
	double m_dVal;
	// the index of the current character
public:
	double GetNumValue();
	void PascalComments(BOOL bFlag);
	virtual CString GetStrValue();
	void QuoteChar(int ch);
	int LineNo();
	virtual void PushBack();
	virtual int NextToken();
	void LowerCaseMode(BOOL bFlag);
	void SlSlComments(BOOL bFlag);
	void SlStComments(BOOL bFlag);
	void EolIsSignificant(BOOL bFlag);
	void ParseNumbers();
	
	// Public functions
	// Character Type Setting Functions
	void ResetSyntax();
	void WordChars(int cLow, int cHi);
	void WhiteSpaceChars(int cLow, int cHi);
	void OrdinaryChars(int cLow, int cHi);
	void OrdinaryChar(int ch);
	void CommentChar(int ch);

	//+add kwmoon 071004
	void WordDigit(int cLowData, int cHiData);
};

#endif // !defined(AFX_STRINGTOKENIZER_H__5710B523_A492_11D2_8AB8_00002145DF63__INCLUDED_)
