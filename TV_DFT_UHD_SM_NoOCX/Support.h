// 명    칭 : AIAS(Automatic Inline Alignment System)
// 창작일자 : 1997. 2. 21
// 소    속 : LG전자(LG생산기술원 MI Gr. 오구환)
// File  명 : Support.h

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

void	RegisterStep(int nStep, void(*pFunc)());
int 	GetStepNo();
double	GetNominal();
double	GetHighLimit();
double	GetLowLimit();
BOOL	CheckValue(double value);
void	SetTimeLimit(int nTime);
BOOL	IsTimeOut();
__inline int	GetInteger();
__inline double	GetDouble();
__inline char*	GetString();
__inline BOOL	GetBoolean();

#endif	//__SUPPORT_H__

