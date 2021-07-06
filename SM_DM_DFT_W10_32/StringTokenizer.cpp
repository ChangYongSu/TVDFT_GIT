// StringTokenizer.cpp: implementation of the CStringTokenizer class.
//
//////////////////////////////////////////////////////////////////////
// Please do not remove the folowing comment !!!
// This code is writen by Zoly Farkas 
// You may use this code free
// I used as model for this code the streamtokenizer class from Sun's java
// Please if any bug found send a mail to zolyfarkas@usa.net
// Thanks


#include "stdafx.h"
#include "StringTokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringTokenizer::CStringTokenizer(CString& string)
{
	int nTemp;

	m_bForceLower = FALSE;
	m_bPushedBack = FALSE;
	m_bEolIsSignificant = FALSE;
	m_bSlSlComments = TRUE;
	m_bSlStComments = TRUE;
	m_bPascalComments = TRUE;
	m_iLineNo = 1;	// the first line
	m_iChar = 0;
	m_peekc = ' ';
	m_sString = string;
//	m_sString += -1;

	//+add 080402 psh
	nTemp = m_sString.GetLength();
	m_sString.GetBufferSetLength(nTemp+1);
	m_sString.SetAt(nTemp, -1);
	//-

	// Reset syntax
	ResetSyntax();
	// Set the word Chars
	WordChars('a','z');
	WordChars('A','Z');

	//+delete 080402 psh
//	WordChars('0','9');

	//+delete 080402 psh
	//+add kwmoon 071004
//	WordDigit('0','9');

	WordChars(128+32,255);
	QuoteChar('"');
	QuoteChar('\'');
	WhiteSpaceChars(0,' ');
	ParseNumbers();
}

CStringTokenizer::~CStringTokenizer()
{

}

// Function that resets the syntax table

void CStringTokenizer::ResetSyntax()
{
	int i;
	for (i = 0 ; i<=NCHAR; i++)
			m_ctType[i] = 0;
}

// Specifies the range of the word characters

//+change kwmoon 071004 
// cLow -> cData1 (varible name 'cLow' makes awful error. that is, it adds 1 to cLow value automatically)
void  CStringTokenizer::WordChars(int cLowData, int cHiData)
{
	if (cLowData<0)
		cLowData = 0;
	if (cHiData > NCHAR)
		cHiData = NCHAR;
	while (cLowData <= cHiData)
			m_ctType[cLowData++] |= CT_ALPHA;
}

//+add kwmoon 071004 
void  CStringTokenizer::WordDigit(int cLowData, int cHiData)
{
	if (cLowData<0)
		cLowData = 0;
	if (cHiData > NCHAR)
		cHiData = NCHAR;
	while (cLowData <= cHiData)
			m_ctType[cLowData++] |= CT_DIGIT;
}

// Specifies the range of the space characters

void CStringTokenizer::WhiteSpaceChars(int cLow, int cHi)
{
	if (cLow<0)
		cLow = 0;
	if (cHi > NCHAR)
		cHi = NCHAR;
	while (cLow <= cHi)
			m_ctType[cLow++] |= CT_WHITESPACE;
}

// Specifies the range of the ordinary characters

void CStringTokenizer::OrdinaryChars(int cLow, int cHi)
{
	if (cLow<0)
		cLow = 0;
	if (cHi > NCHAR)
		cHi = NCHAR;
	while (cLow <= cHi)
			m_ctType[cLow++] |= 0;
}

// Specifies a ordinary character

void CStringTokenizer::OrdinaryChar(int ch)
{
	m_ctType[ch]=0;
}


// Specifies that this char starts a comment 

void CStringTokenizer::CommentChar(int ch)
{
	m_ctType[ch]=CT_COMMENT;
}


// Specifies that Numbers should be parsed


void CStringTokenizer::ParseNumbers()
{
	int i;
	for (i = '0'; i<= '9'; i++)
		m_ctType[i] |= CT_ALPHA;
	m_ctType['_'] |= CT_ALPHA;
	m_ctType['-'] |= CT_ALPHA;
	//+add 080402 psh
	m_ctType['.'] |= CT_ALPHA;
}

// Specifies that EOL is significant or not

void CStringTokenizer::EolIsSignificant(BOOL bFlag)
{
	m_bEolIsSignificant = bFlag;
}


// Enable C Style comments

void CStringTokenizer::SlStComments(BOOL bFlag)
{
	m_bSlStComments = bFlag;
}

// Enable C++ Style comments

void CStringTokenizer::SlSlComments(BOOL bFlag)
{
	m_bSlSlComments = bFlag;
}

// Enable / disable case sensitivity

void CStringTokenizer::LowerCaseMode(BOOL bFlag)
{
	m_bForceLower = bFlag;
}

// gives the next Token, returns the token type

int CStringTokenizer::NextToken()
{
	if (m_bPushedBack)
	{
		m_bPushedBack = FALSE;
		return m_tType;
	}
	char c = m_peekc;
	m_sVal = _T("");
	
	if (c < 0)
		return m_tType = TT_EOF;
	
	int ctype = c<256 ? m_ctType[c] : CT_ALPHA;
	
	// is this whitespace 
	while ((ctype & CT_WHITESPACE) != 0)
	{
		if (c == '\r')
		{
			m_iLineNo++;
			c = GetChar();
			if (c == '\n')
				c= GetChar();
			if (m_bEolIsSignificant)
			{
				m_peekc = c;
				return m_tType = TT_EOL;
			}
		}
		else
		{
			if (c == '\n')
			{
				m_iLineNo++;
				if (m_bEolIsSignificant)
				{
					m_peekc = ' ';
					return m_tType = TT_EOL;
				}
			}
			c = GetChar();
		}
		if (c < 0)
			return m_tType = TT_EOF;
		ctype = c< 256 ? m_ctType[c] : CT_ALPHA;
	}
	
	// is this a number
	if ((ctype & CT_DIGIT) != 0)
	{
		BOOL neg = FALSE;		
		if (c == '-')
		{
			c = GetChar();
			if (c != '.' && (c < '0' || c > '9'))
			{
				m_peekc = c;
				return  m_tType = '-';
			}
			neg = TRUE;
		}
		double v = 0;
		int decexp = 0;
		int seendot = 0;
		while (true)
		{
			if (c == '.' && seendot == 0)
				seendot = 1;
			else if ('0' <= c && c <= '9')
			{
				v = v* 10 + (c - '0');
				decexp += seendot;
			}
			else break;
			c = GetChar();
		}
		m_peekc =c;
		if (decexp != 0)
		{
			double denom = 10;
			decexp--;
			while (decexp>0)
			{
				denom*=10;
				decexp--;
			}
			v = v/denom;
		}
		m_dVal = neg ? -v : v;
		if (seendot==0)
			return m_tType = TT_INTEGER;
		else
			return m_tType = TT_REAL;
	}
	// is this a word
	if ((ctype & CT_ALPHA) != 0)
	{
		int i = 0;
		int size = BSIZE;
		CString buf;
		do 
		{
			buf += (TCHAR) c;
			c = GetChar();
			ctype = c<0 ? CT_WHITESPACE : c<256 ? m_ctType[c] : CT_ALPHA;
		} while ((ctype & (CT_ALPHA | CT_DIGIT)) != 0);
		m_peekc = c;
		m_sVal = buf;
		if (m_bForceLower)
			m_sVal.MakeLower ();
		return m_tType = TT_WORD;
	}
	// is it a comment ?
	if ((ctype & CT_COMMENT) != 0)
	{
		while ((c = GetChar()) != '\n' && c != '\r' && c>= 0);
		m_peekc = c;
		return NextToken();
	}
	// fucking string shit la la la
	if ((ctype & CT_QUOTE) != 0)
	{
		int size = BSIZE;
		CString buf;
		m_tType = c;
		int i=0;
		while ((c = GetChar()) != m_tType && c!='\n' && c != '\r')
		{
			if (c == '\\')
				switch (c = GetChar())
				{
				case 'a': c = 0x7; break;
				case 'b': c = '\b'; break;
				case 'f': c = 0xC; break;
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				case 'v': c = 0xb; break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					c = c - '0';
					int c2 = GetChar();
					if ('0' <= c2 && c2 <= '7')
					{
						c = (c<<3) + (c2 - '0');
						c2 = GetChar();
						if ('0' <= c2 && c2 <= '7')
							c = (c<<3) + (c2 - '0');
						else m_peekc = c;
					}
					else
						m_peekc = c;
					break;
				}
			buf+= (TCHAR) c;
		}
		m_peekc = ' ';
		m_sVal=buf;
		return m_tType = TT_STRING;
	}
	// and now the // stuff
	if (c == '{' && m_bPascalComments)
	{
			while ((c = GetChar()) != '}')
			{
				if (c == '\n')
					m_iLineNo++;
				if (c<0)
					return m_tType = TT_EOF;
			}
			m_peekc = ' ';
			return NextToken();
	}
	
	if (c == '/' && (m_bSlSlComments || m_bSlStComments))
	{
		c = GetChar();
		if (c == '*' && m_bSlStComments)
		{
			int prevc = 0;
			while ((c = GetChar()) != '/' || prevc != '*')
			{
				if (c == '\n')
					m_iLineNo++;
				if (c<0)
					return m_tType = TT_EOF;
				prevc = c;
			}
			m_peekc = ' ';
			return NextToken();
		}
		else 
			if (c == '/' && m_bSlSlComments)
			{
				while ((c = GetChar()) != '\n' && c != '\r' && c >= 0);
				m_peekc = c;
				return NextToken();
			}
			else
			{
				m_peekc = ' ';
				return m_tType = '/';
			}
	}
	m_peekc = ' ';
	return m_tType = c;

}

void CStringTokenizer::PushBack()
{
	m_bPushedBack = TRUE;
}

int CStringTokenizer::LineNo()
{
	return m_iLineNo;
}

void CStringTokenizer::QuoteChar(int ch)
{
	m_ctType[ch] = CT_QUOTE;
}

CString CStringTokenizer::GetStrValue()
{
	CString ret;
	switch (m_tType)
	{
	case TT_EOF:
		ret = "EOF";
		break;
	case TT_EOL:
		ret = "EOL";
		break;
	case TT_WORD:
		ret = m_sVal;
		break;
	case TT_STRING:
		ret = m_sVal;
		break;
	case TT_INTEGER:
	case TT_REAL:
		ret.Format("%f",m_dVal);
		break;
	default:
		ret.Format ( "\'%c\'",(char) m_tType) ;
	}
	return ret;
}

void CStringTokenizer::PascalComments(BOOL bFlag)
{
	m_bPascalComments = bFlag;
}

double CStringTokenizer::GetNumValue()
{
	return m_dVal;
}
