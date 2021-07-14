#if !defined(__WINDLIB_TEMPLATE_H)
#define __WINDLIB_TEMPLATE_H

#pragma once

#if !defined(__WINDLIB_H)
#error 'This header is subset of windlib.h.  use include windlib.h'
#endif


#include <stdio.h>
#include <stdlib.h>

#if (CUR_OS_TYPE == OS_TYPE_WINDOWS) && (!defined(__WINDLIB_API_LOCAL))
#	define WTEMPLATE_FLAG	__declspec(dllexport)
#else
#	define WTEMPLATE_FLAG
#endif

//////////////////////////////////////////////////////////////////////
//	Class Hierarchy chart in this library
//
//
//	- 아래 객체들은 Object 자체를 다루는 버퍼들이다.
//	- 생성자 또는 소멸자의 작업이 필요한 객체에 대해서는 사용하지 않는다
//	- Class 객체에 대해서는 아래의 CWPtrList 를 사용한다.
//
//		CWArray<BYTE>		- O
//		CWArray<CWString>	- X
//
template<typename Type> class WTEMPLATE_FLAG CWBuffer;
	template<typename Type> class WTEMPLATE_FLAG CWArray;
template<typename Type> class CWCircularQueue;
;


//
//	CWBuffer
//		Alloc 할때의 크기는 버퍼크기가 아닌 배열 크기 이다.
//
template<typename Type>
class CWBuffer
{
public:
	//	nArraySize - Not Buffer Size
	CWBuffer(int nArraySize = 0);
	virtual ~CWBuffer();

	//	nArraySize - Not Buffer Size
	Type * Alloc(int nArraySize);
	
	//	nArraySize - Not Buffer Size
	//	if nArraySize is <= 0, all data are cleard
	Type * Resize(int nArraySize);

	void Free();

	//	Set all buffer to 0(Zero)
	bool Clear();

	Type * GetBuffer();
	const Type * GetBuffer() const;

	operator Type *();
	operator const Type *() const;

	int GetArraySize() const;
	int GetBufferSize() const;

	//	내부 버퍼들을 서로 바꾼다.
	void Exchange(CWBuffer<Type> & dest);

protected:
	int	m_nArraySize;
	int	m_nBufSize;
	Type*	m_pBuffer;
};


//
//	CWArray
//
//		GetHead, GetTail, GetAt, [] 함수를 사용할때는 반드시
//		Array 가 비어있는지, 지정한 Index 가 정상적인지  체크해야 하며,
//		잘못된 위치 지정이 넘어오면 Gabage 의 객체가 반환된다.
//
#define N_DEF_WARRAY_GROW_SIZE		256

template<typename Type>
class CWArray : protected CWBuffer<Type>
{
public:
	CWArray(int nSize = 0, int nGrow = N_DEF_WARRAY_GROW_SIZE);
	virtual ~CWArray();

	void Free();

	//  Operation
public:
	bool AddHead(Type item);
	bool AddTail(Type item);
	bool Add(Type item);	// same AddTail
	bool Append(const Type * pItem, int nItemCnt);

	//	nIndex < 0 - AddHead
	//	nIndex >= m_nDataCount - AddTail
	bool Insert(int nIndex, Type item);

	bool SetAt(int nIndex, Type item);

	bool RemoveHead();
	bool RemoveTail();
	bool RemoveAt(int nIndex, int nCount = 1);

	Type& GetHead();
	const Type& GetHead() const;

	Type& GetTail();
	const Type& GetTail() const;

	Type& GetAt(int nIndex);
	const Type& GetAt(int nIndex) const;

	Type& operator[](int nIndex);
	const Type& operator[](int nIndex) const;

	int FindItemIndex(Type item);

	void Clear();
	bool IsEmpty() const;
	bool IsFull() const;
	int GetCount() const;

	//	nArraySize <= 0, Resize to m_nDataCount * 2
	bool Resize(int nArraySize = -1);

	const Type * GetBuffer() const;
	operator const Type *() const;

	//	내부 버퍼들을 서로 바꾼다.
	void Exchange(CWArray<Type> & dest);

protected:
	int	m_nGrow;
	int	m_nDataCount;
	Type	m_gabage;
};

//
//	CWCircularQueue
//
template<typename Type>
class CWCircularQueue : protected CWBuffer<Type>
{
public:
	CWCircularQueue(int nSize = 10);

	void SetQueueSize(int nSize);

	void Clear();

	Type * GetBuffer();

	int GetQueueSize() const;

	int GetCount() const;
	bool IsEmpty() const;
	bool IsFull() const;

	bool Add(Type data);
	bool Add(Type * ptrData, int nDataCnt);

	bool Delete(Type * ptrData);	// 1 data
	bool Delete(Type * ptrData, int nDataCnt);

	void Undelete(int nCount);

public://protected:
	int	m_nHead;
	int	m_nTail;
	int	m_nDataCount;
};

//////////////////////////////////////////////////////////////////////////
//	Implement object
//

template<typename Type>
inline CWBuffer<Type>::CWBuffer(int nArraySize)
	{
		m_nArraySize = 0;
		m_nBufSize = 0;
		m_pBuffer = NULL;

		Alloc(nArraySize);
	}

template<typename Type>
inline CWBuffer<Type>::~CWBuffer()
	{	Free();	}

template<typename Type>
inline Type * CWBuffer<Type>::Alloc(int nArraySize)
	{
		Free();

		if (nArraySize <= 0)
			return NULL;

		m_nArraySize = nArraySize;
		m_nBufSize = nArraySize * sizeof(Type);
		m_pBuffer = (Type *) ::malloc(m_nBufSize);
		
		return m_pBuffer;
	}

template<typename Type>
inline Type * CWBuffer<Type>::Resize(int nArraySize)
	{
		if (nArraySize <= 0)
		{
			Free();
			return NULL;
		}

		//	New Alloc
		if (! m_pBuffer)
			return Alloc(nArraySize);

		m_nArraySize = nArraySize;
		m_nBufSize = nArraySize * sizeof(Type);
		m_pBuffer = (Type *) ::realloc(m_pBuffer, m_nBufSize);

		return m_pBuffer;
	}

template<typename Type>
inline void CWBuffer<Type>::Free()
	{
		if (m_pBuffer)
			::free(m_pBuffer);

		m_pBuffer = NULL;
		m_nArraySize = 0;
		m_nBufSize = 0;
	}

template<typename Type>
inline bool CWBuffer<Type>::Clear()
	{
		if (! m_pBuffer)
			return false;

		memset(m_pBuffer, 0, GetBufferSize());
		return true;
	}

template<typename Type>
inline Type * CWBuffer<Type>::GetBuffer()
	{	return m_pBuffer;	}
template<typename Type>
inline const Type * CWBuffer<Type>::GetBuffer() const
	{	return m_pBuffer;	}

template<typename Type>
inline CWBuffer<Type>::operator Type *()
	{	return m_pBuffer;	}
template<typename Type>
inline CWBuffer<Type>::operator const Type *() const
	{	return m_pBuffer;	}

template<typename Type>
int CWBuffer<Type>::GetArraySize() const
	{	return m_nArraySize;	}

template<typename Type>
int CWBuffer<Type>::GetBufferSize() const
	{	return m_nBufSize;	}

template<typename Type>
void CWBuffer<Type>::Exchange(CWBuffer<Type> & dest)
	{
		Type*	pBuffer = m_pBuffer;
		int	nArrSize = m_nArraySize;
		int	nBufSize = m_nBufSize;

		m_pBuffer = dest.m_pBuffer;
		m_nArraySize = dest.m_nArraySize;
		m_nBufSize = dest.m_nBufSize;

		dest.m_pBuffer = pBuffer;
		dest.m_nArraySize = nArrSize;
		dest.m_nBufSize = nBufSize;
	}

//
//	CWArray
//
template<typename Type>
inline CWArray<Type>::CWArray(int nArraySize, int nGrow)
	: CWBuffer<Type>(nArraySize)
	{
		m_nGrow = nGrow;
		if (m_nGrow < N_DEF_WARRAY_GROW_SIZE)
			m_nGrow = N_DEF_WARRAY_GROW_SIZE;

		m_nDataCount = 0;
	}

template<typename Type>
inline CWArray<Type>::~CWArray()
	{  Free();	}

template<typename Type>
inline bool CWArray<Type>::AddHead(Type item)
	{  return Insert(0, item);	}

template<typename Type>
inline bool CWArray<Type>::AddTail(Type item)
	{  return Insert(m_nDataCount, item);	}

template<typename Type>
inline bool CWArray<Type>::Add(Type item)	// same AddTail
	{  return AddTail(item);	}

template<typename Type>
inline bool CWArray<Type>::Append(const Type * pItem, int nItemCnt)
	{
		if ((m_nDataCount + nItemCnt) > CWBuffer<Type>::GetArraySize())
		{
			if (! Resize(m_nDataCount + nItemCnt))
				return false;
		}

		memcpy(CWBuffer<Type>::m_pBuffer + m_nDataCount, pItem, nItemCnt * sizeof(Type));
		m_nDataCount += nItemCnt;

		return true;
	}

template<typename Type>
inline bool CWArray<Type>::Insert(int nIndex, Type item)
	{
		if (nIndex < 0)
			nIndex = 0;
		if (nIndex > m_nDataCount)
			nIndex = m_nDataCount;

		if (IsFull())
		{
			if (! Resize())
				return false;
		}

		memmove(CWBuffer<Type>::m_pBuffer + nIndex + 1, CWBuffer<Type>::m_pBuffer + nIndex, (m_nDataCount - nIndex) * sizeof(Type));
		CWBuffer<Type>::m_pBuffer[nIndex] = item;
		m_nDataCount ++;

		return true;
	}

template<typename Type>
inline bool CWArray<Type>::SetAt(int nIndex, Type item)
	{
		if (nIndex < 0 || nIndex >= m_nDataCount)
			return false;
		CWBuffer<Type>::m_pBuffer[nIndex] = item;
		return true;
	}

template<typename Type>
inline bool CWArray<Type>::RemoveHead()
	{  return RemoveAt(0);	}

template<typename Type>
inline bool CWArray<Type>::RemoveTail()
	{  return RemoveAt(m_nDataCount - 1);	}

template<typename Type>
inline bool CWArray<Type>::RemoveAt(int nIndex, int nCount)
	{
		if ((nIndex < 0) || (nCount <= 0))
			return false;

		int	nEnd = nIndex + nCount;	// 끝위치 다음
		if (nEnd > m_nDataCount)
			return false;

		memcpy(CWBuffer<Type>::m_pBuffer + nIndex, CWBuffer<Type>::m_pBuffer + nEnd, (m_nDataCount - nEnd) * sizeof(Type));
		m_nDataCount -= nCount;

		return true;
	}

template<typename Type>
inline Type& CWArray<Type>::GetHead()
	{  return GetAt(0);	}
template<typename Type>
inline const Type& CWArray<Type>::GetHead() const
	{  return GetAt(0);	}

template<typename Type>
inline Type& CWArray<Type>::GetTail()
	{  return GetAt(m_nDataCount - 1);	}
template<typename Type>
inline const Type& CWArray<Type>::GetTail() const
	{  return GetAt(m_nDataCount - 1);	}

template<typename Type>
inline Type& CWArray<Type>::GetAt(int nIndex)
	{
		if (nIndex < 0 || nIndex >= m_nDataCount)
			return m_gabage;
		else
			return CWBuffer<Type>::m_pBuffer[nIndex];
	}

template<typename Type>
inline const Type& CWArray<Type>::GetAt(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= m_nDataCount)
			return m_gabage;
		else
			return CWBuffer<Type>::m_pBuffer[nIndex];
	}

template<typename Type>
inline int CWArray<Type>::FindItemIndex(Type item)
	{
		for(int i = 0; i < m_nDataCount; i ++)
			if (CWBuffer<Type>::m_pBuffer[i] == item)
				return i;

		return -1;
	}
	
template<typename Type>
inline bool CWArray<Type>::IsEmpty() const
	{  return (m_nDataCount <= 0) ? true : false;	}

template<typename Type>
inline bool CWArray<Type>::IsFull() const
	{  return (m_nDataCount >= CWBuffer<Type>::GetArraySize() - 1) ? true : false;	}

template<typename Type>
inline int CWArray<Type>::GetCount() const
	{  return m_nDataCount;	}

template<typename Type>
inline bool CWArray<Type>::Resize(int nArraySize)
	{
		if (nArraySize <= 0)
			nArraySize = m_nDataCount + m_nGrow;
		else if (nArraySize < m_nDataCount)
			nArraySize = m_nDataCount;

		nArraySize = (nArraySize + m_nGrow - 1) / m_nGrow * m_nGrow;
		if (CWBuffer<Type>::Resize(nArraySize) == NULL)
			return false;

		m_nDataCount = (nArraySize < m_nDataCount) ? nArraySize : m_nDataCount;

		return true;
	}

template<typename Type>
inline Type & CWArray<Type>::operator[](int nIndex)
	{
		if ((nIndex >= 0) && (nIndex < m_nDataCount))
			return CWBuffer<Type>::m_pBuffer[nIndex];
		else
			return m_gabage;
	}

template<typename Type>
inline const Type & CWArray<Type>::operator[](int nIndex) const
	{
		if ((nIndex >= 0) && (nIndex < m_nDataCount))
			return CWBuffer<Type>::m_pBuffer[nIndex];
		else
			return m_gabage;
	}

template<typename Type>
inline void CWArray<Type>::Clear()
	{  m_nDataCount = 0;	}

template<typename Type>
inline void CWArray<Type>::Free()
	{
		CWBuffer<Type>::Free();
		m_nDataCount = 0;
	}
template<typename Type>
inline const Type * CWArray<Type>::GetBuffer() const
	{  return CWBuffer<Type>::m_pBuffer;	}

template<typename Type>
inline CWArray<Type>::operator const Type *() const
	{  return CWBuffer<Type>::m_pBuffer;	}

//	내부 버퍼들을 서로 바꾼다.
template<typename Type>
inline void CWArray<Type>::Exchange(CWArray<Type> & dest)
	{
		CWBuffer<Type>::Exchange(dest);

		int	nGrow = m_nGrow;
		int	nDataCount = m_nDataCount;
		
		m_nGrow = dest.m_nGrow;
		m_nDataCount = dest.m_nDataCount;

		dest.m_nGrow = nGrow;
		dest.m_nDataCount = nDataCount;
	}


//
//	CWCircularQueue
//
template<typename Type>
inline CWCircularQueue<Type>::CWCircularQueue(int nArraySize)
	: CWBuffer<Type>(nArraySize)
	{  Clear();	}

template<typename Type>
inline void CWCircularQueue<Type>::SetQueueSize(int nArraySize)
	{	CWBuffer<Type>::Resize(nArraySize);	Clear();	}

template<typename Type>
inline void CWCircularQueue<Type>::Clear()
	{  m_nHead = m_nTail = m_nDataCount = 0;	}

template<typename Type>
inline Type * CWCircularQueue<Type>::GetBuffer()
	{	return CWBuffer<Type>::m_pBuffer;	}

template<typename Type>
inline int CWCircularQueue<Type>::GetQueueSize() const
	{  return CWBuffer<Type>::GetArraySize();	}

template<typename Type>
inline int CWCircularQueue<Type>::GetCount() const
	{  return m_nDataCount;	}

template<typename Type>
inline bool CWCircularQueue<Type>::IsFull() const
	{  return !! ((CWBuffer<Type>::GetArraySize() <= 0) || (GetCount() >= CWBuffer<Type>::GetArraySize()));	}

template<typename Type>
inline bool CWCircularQueue<Type>::IsEmpty() const
	{  return !! (GetCount() <= 0);	}

template<typename Type>
inline bool CWCircularQueue<Type>::Add(Type data)
	{
		if (IsFull())
			return false;

		CWBuffer<Type>::m_pBuffer[m_nHead ++] = data;
		m_nHead %= GetQueueSize();
		m_nDataCount ++;

		return true;
	}

template<typename Type>
inline bool CWCircularQueue<Type>::Add(Type * ptrData, int nDataCnt)
	{
		if (CWBuffer<Type>::GetArraySize() <= 0)
			return false;

		if ((GetCount() + nDataCnt) > GetQueueSize())
			return false;

		int		nCopy;
		int		nSize = GetQueueSize() - m_nHead;

		if (nSize >= nDataCnt)
			nCopy = nDataCnt;
		else
			nCopy = nSize;

		if (ptrData)
			memcpy(CWBuffer<Type>::m_pBuffer + m_nHead, ptrData, nCopy * sizeof(Type));
		else
			memset(CWBuffer<Type>::m_pBuffer + m_nHead, 0, nCopy * sizeof(Type));

		nDataCnt -= nCopy;
		ptrData += nCopy;
		m_nHead += nCopy;
		m_nHead %= GetQueueSize();
		m_nDataCount += nCopy;

		if (nDataCnt > 0)
		{
			if (ptrData)
				memcpy(CWBuffer<Type>::m_pBuffer + m_nHead, ptrData, nDataCnt * sizeof(Type));
			else
				memset(CWBuffer<Type>::m_pBuffer + m_nHead, 0, nDataCnt * sizeof(Type));

			m_nHead += nDataCnt;
			m_nHead %= GetQueueSize();
			m_nDataCount += nDataCnt;
		}

		return true;
	}

template<typename Type>
inline bool CWCircularQueue<Type>::Delete(Type * ptrData)
	{
		if (IsEmpty())
			return false;

		* ptrData = CWBuffer<Type>::m_pBuffer[m_nTail ++];
		m_nTail %= GetQueueSize();
		m_nDataCount --;

		return true;
	}

template<typename Type>
inline bool CWCircularQueue<Type>::Delete(Type * ptrData, int nDataCnt)
	{
		if (GetCount() < nDataCnt)
			return false;

		if (! ptrData)
		{
			m_nTail += nDataCnt;
			m_nTail %= GetQueueSize();
			m_nDataCount -= nDataCnt;
			return true;
		}

		int		nCopy;
		int		nSize = GetQueueSize() - m_nTail;

		if (nSize >= nDataCnt)
			nCopy = nDataCnt;
		else
			nCopy = nSize;

		memcpy(ptrData, CWBuffer<Type>::m_pBuffer + m_nTail, nCopy * sizeof(Type));
		nDataCnt -= nCopy;
		ptrData += nCopy;
		m_nTail += nCopy;
		m_nTail %= GetQueueSize();
		m_nDataCount -= nCopy;

		if (nDataCnt > 0)
		{
			memcpy(ptrData,CWBuffer<Type>:: m_pBuffer + m_nTail, nDataCnt * sizeof(Type));
			m_nTail += nDataCnt;
			m_nTail %= GetQueueSize();
			m_nDataCount -= nDataCnt;
		}

		return true;
	}

template<typename Type>
inline void CWCircularQueue<Type>::Undelete(int nUndelCnt)
{
	m_nTail -= nUndelCnt;
	if (m_nTail < 0)
		m_nTail += GetQueueSize();
	m_nDataCount += nUndelCnt;
}

#undef	WTEMPLATE_FLAG

#endif  // __WINDLIB_TEMPLATE_H
