#if !defined(__WINDLIB_PTRLIST_H)
#define  __WINDLIB_PTRLIST_H

#pragma once

#if !defined(__WINDLIB_H)
#error 'This header is subset of windlib.h.  use include windlib.h'
#endif


#include "osconfig.h"

#if (CUR_OS_TYPE == OS_TYPE_WINDOWS) && (!defined(__WINDLIB_API_LOCAL))
#	define WTEMPLATE_FLAG	__declspec(dllexport)
#else
#	define WTEMPLATE_FLAG
#endif

//
//	Double-Linked List
//

//////////////////////////////////////////////////////////////////////
//	Class Hierarchy chart in this library
//
template<class PtrType> struct WTEMPLATE_FLAG CWPtrListNode;
template<class Type> class WTEMPLATE_FLAG CWPtrList;
	template<class Type> class WTEMPLATE_FLAG CWSortedPtrList;

typedef	void *		LISTPOS;

template<class PtrType>
struct CWPtrListNode
{
	CWPtrListNode<PtrType>*	pPrev;
	CWPtrListNode<PtrType>*	pNext;
	PtrType	pData;
};


//
//	PtrType = Object *
//		ex) CWPtrList<CMyObj *>	aaa;
//		aa.Add(new CMyObj)

template<class PtrType>
class CWPtrList
{
	//  nIndex : 0 Base Index

public:
	CWPtrList();
	virtual ~CWPtrList();

	bool Lock();
	void Unlock();

	int  GetCount() const;
	bool IsEmpty() const;

	PtrType GetAt(LISTPOS position) const;
	PtrType GetHead() const;
	PtrType GetTail() const;
	PtrType GetIndex(int nIndex) const;

	//
	//	Add Function
	//	- 나머지 Addxxx 함수는 모두 Fail 임,
	//
	LISTPOS Add      (PtrType pData);

	LISTPOS AddHead  (PtrType pData);
	LISTPOS AddTail  (PtrType pData);
	LISTPOS AddIndex (int nIndex, PtrType pData);
		// nIndex < 0 -> AddHead
		// nIndex >= DataCount -> AddTail
	LISTPOS AddAfter (LISTPOS pos, PtrType pData);
	LISTPOS AddBefore(LISTPOS pos, PtrType pData);

	// 아래 3개의 함수는 추가하고 나면 pList 는 모두 삭제된다.
	void AddHead(CWPtrList<PtrType> * pList);
	void AddTail(CWPtrList<PtrType> * pList);

	//
	//	Delete Function
	//
	bool Clear();			// Data Buffer 만 Clear
	bool RemoveAll();		// Same to Clear

	bool ClearFreeNode();	// Free Buffer 가 무한정 증가하는 것을 방지

	// 아래 Delete 함수는 User 가 Buffer 를 해제한다.
	PtrType DeleteHead();
	PtrType DeleteTail();
	PtrType DeleteAt(LISTPOS position);
	PtrType DeleteIndex(int nIndex);
		// nIndex 번째에 Data 를 삭제한다.

	//
	//	Find Function
	//
	LISTPOS FindHead() const;
	LISTPOS FindTail() const;
	LISTPOS FindNext(LISTPOS pos) const;
	LISTPOS FindPrev(LISTPOS pos) const;
	LISTPOS FindIndex(int nIndex) const;
		// nIndex 번째에 Data 를 찾는다.
	
	//
	// Iteration
	//
	void Rewind(bool bReverse = false);
	PtrType operator ()();
	LISTPOS GetCurPos() const;

	PtrType GetNext(LISTPOS & rPosition) const; // return *Position++
	PtrType GetPrev(LISTPOS & rPosition) const; // return *Position--

protected:
	int		m_nDataCount;
	bool	m_bReverseRewind;

	CWPtrListNode<PtrType>*	m_pHead;
	CWPtrListNode<PtrType>*	m_pTail;
	CWPtrListNode<PtrType>*	m_pFree;
	LISTPOS	m_posCurrent;	// for Operator ()

	CWMutex	m_mutex;

protected:
	CWPtrListNode<PtrType>* NewNode(PtrType pData = NULL);
	void     FreeNode(CWPtrListNode<PtrType>* pNode);

	LISTPOS	 HeadNode() const;
	LISTPOS  TailNode() const;

	void AddToList(CWPtrListNode<PtrType> * pNode, CWPtrListNode<PtrType> * pPrev, CWPtrListNode<PtrType> * pNext);
	PtrType DeleteFromList(CWPtrListNode<PtrType> * pNode);
};

//
//	CWSortedPtrList
//
template<typename PtrType>
class CWSortedPtrList : public CWPtrList<PtrType>
{
public:
	LISTPOS AddSort(PtrType pData, bool bAscending = true)
	{
		if (! pData)
			return NULL;

		CWPtrListNode<PtrType>*	pNew = CWPtrList<PtrType>::NewNode(pData);
		if (! pNew)
			return NULL;

		int		nComp;
		CWPtrListNode<PtrType>*	pPrev;
		CWPtrListNode<PtrType>*	pNext;

		pNext = CWPtrList<PtrType>::m_pHead->pNext;
		while (pNext != CWPtrList<PtrType>::m_pTail)
		{
			nComp = pNext->pData->Compare(pData);
			//	자료가 같을때는 맨 뒤로 가도록 하기 위해서
			//	비교할때 >= (or <=) 을 사용하지 않는다.
			if ((bAscending && nComp > 0) || (! bAscending && nComp < 0))
				break;
			pNext = pNext->pNext;
		}
		pPrev = pNext->pPrev;

		CWPtrList<PtrType>::AddToList(pNew, pPrev, pNext);

		return (LISTPOS) pNew;
	}
};

//
//	CWPtrList
//
template<class PtrType>
inline CWPtrList<PtrType>::CWPtrList()
	{
		m_nDataCount = 0;
		m_bReverseRewind = false;

		m_pFree = NULL;	// NewNode 하기 전에 설정

		m_pHead = NewNode();
		m_pTail = NewNode();
		m_pHead->pPrev = m_pHead;
		m_pHead->pNext = m_pTail;
		m_pTail->pPrev = m_pHead;
		m_pTail->pNext = m_pTail;

		m_posCurrent = NULL;

		m_mutex.Create();
	}

template<class PtrType>
inline CWPtrList<PtrType>::~CWPtrList()
	{
		Clear();

		FreeNode(m_pHead);
		FreeNode(m_pTail);
		m_pHead = NULL;
		m_pTail = NULL;

		ClearFreeNode();	// FreeNode 한후에 호출

		m_mutex.Destroy();
	}

template<class PtrType>
inline bool  CWPtrList<PtrType>::Lock()
	{
		return m_mutex.Lock();
	}

template<class PtrType>
inline void CWPtrList<PtrType>::Unlock()
	{
		m_mutex.Unlock();
	}

template<class PtrType>
inline int CWPtrList<PtrType>::GetCount() const
	{	return m_nDataCount;	}

template<class PtrType>
inline bool CWPtrList<PtrType>::IsEmpty() const
	{	return !! (m_nDataCount <= 0);	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::GetAt(LISTPOS pos) const
	{	return (pos) ? reinterpret_cast<CWPtrListNode<PtrType> *>(pos)->pData : NULL;	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::GetHead() const
	{	return GetAt(FindHead());	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::GetTail() const
	{	return GetAt(FindTail());	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::GetIndex(int nIndex) const
	{	return GetAt(FindIndex(nIndex));	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::Add(PtrType pData)
	{
		if (! pData)
			return NULL;
		return AddTail(pData);
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::AddHead(PtrType pData)
	{	return AddAfter(HeadNode(), pData);	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::AddTail(PtrType pData)
	{	return AddBefore(TailNode(), pData);	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::AddAfter (LISTPOS pos, PtrType pData)
	{
		if (! pos || ! pData)
			return NULL;

		CWPtrListNode<PtrType>*	pNew = NewNode(pData);

		if (pNew)
		{
			CWPtrListNode<PtrType>*	pPrev = reinterpret_cast<CWPtrListNode<PtrType> *>(pos);
			CWPtrListNode<PtrType>*	pNext = pPrev->pNext;
			AddToList(pNew, pPrev, pNext);
		}

		return (LISTPOS) pNew;
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::AddBefore(LISTPOS pos, PtrType pData)
	{
		if (! pos || ! pData)
			return NULL;

		CWPtrListNode<PtrType>*	pNew = NewNode(pData);

		if (pNew)
		{
			CWPtrListNode<PtrType>*	pNext = reinterpret_cast<CWPtrListNode<PtrType> *>(pos);
			CWPtrListNode<PtrType>*	pPrev = pNext->pPrev;
			AddToList(pNew, pPrev, pNext);
		}

		return (LISTPOS) pNew;
	}

template<class PtrType>
inline void CWPtrList<PtrType>::AddHead(CWPtrList<PtrType> * pList)
	{
		if (! pList)
			return;

		CWPtrListNode<PtrType>*	pTmp = m_pHead;
		m_pHead = pList->m_pHead;
		pList->m_pHead = pTmp;

		AddTail(pList);
	}

template<class PtrType>
inline void CWPtrList<PtrType>::AddTail(CWPtrList<PtrType> * pList)
	{
		if (! pList)
			return;

		CWPtrListNode<PtrType>*	pTmp = m_pTail;

		m_pTail->pPrev->pNext = pList->m_pHead->pNext;
		pList->m_pHead->pNext->pPrev = m_pTail->pPrev;
		m_pTail = pList->m_pTail;
		m_nDataCount += pList->m_nDataCount;

		pList->m_pTail = pTmp;
		pList->m_pHead->pNext = pTmp;
		pTmp->pPrev = pList->m_pHead;
		pList->m_nDataCount = 0;
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::AddIndex(int nIndex, PtrType pData)
	{
		if (nIndex < 0)
			return AddHead(pData);
		else if (nIndex >= GetCount())
			return AddTail(pData);
		else
			return AddBefore(FindIndex(nIndex), pData);
	}

template<class PtrType>
inline bool CWPtrList<PtrType>::Clear()
	{
		PtrType	pObj;

		while ((pObj = DeleteHead()))
			delete pObj;

		return true;
	}

template<class PtrType>
inline bool CWPtrList<PtrType>::RemoveAll()		// Same to Clear
	{  return Clear();	}

template<class PtrType>
inline bool CWPtrList<PtrType>::ClearFreeNode()
	{
		CWPtrListNode<PtrType>*	pNode;

		while ((pNode = m_pFree))
		{
			m_pFree = m_pFree->pNext;
			delete pNode;
		}

		return true;
	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::DeleteHead()
	{	return DeleteAt(FindHead());	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::DeleteTail()
	{	return DeleteAt(FindTail());	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::DeleteIndex(int nIndex)	//  nIndex : 0 Base Index
	{	return DeleteAt(FindIndex(nIndex));	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::DeleteAt(LISTPOS pos)
	{	return DeleteFromList((CWPtrListNode<PtrType>*) pos);	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::FindHead() const
	{
		if (IsEmpty())
			return NULL;
		else
			return (LISTPOS) m_pHead->pNext;
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::FindTail() const
	{
		if (IsEmpty())
			return NULL;
		else
			return (LISTPOS) m_pTail->pPrev;
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::FindNext(LISTPOS pos) const
	{
		CWPtrListNode<PtrType>*	pNode = (CWPtrListNode<PtrType>*) pos;

		if (! pNode || pNode->pNext == m_pTail)
			return NULL;
		else
			return (LISTPOS) pNode->pNext;
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::FindPrev(LISTPOS pos) const
	{
		CWPtrListNode<PtrType>*	pNode = (CWPtrListNode<PtrType>*) pos;

		if (! pNode || pNode->pPrev == m_pHead)
			return NULL;
		else
			return (LISTPOS) pNode->pPrev;
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::FindIndex(int nIndex) const
	{
		if (nIndex < 0 || nIndex >= m_nDataCount)
			return NULL;

		CWPtrListNode<PtrType>*	pTmp = NULL;

		if (nIndex <= (m_nDataCount / 2))
		{
			pTmp = m_pHead->pNext;
			for (int i = 0; i < nIndex; i ++)
				pTmp = pTmp->pNext;
		}
		else
		{
			nIndex = m_nDataCount - nIndex - 1;
			pTmp = m_pTail->pPrev;
			for (int i = 0; i < nIndex; i ++)
				pTmp = pTmp->pPrev;
		}

		return (LISTPOS) pTmp;
	}

template<class PtrType>
inline void CWPtrList<PtrType>::Rewind(bool bReverse)
	{
		if (! bReverse)
			m_posCurrent = FindHead();
		else
			m_posCurrent = FindTail();

		m_bReverseRewind = bReverse;
	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::operator()()
	{
		if (! m_bReverseRewind)
			return GetNext(m_posCurrent);
		else
			return GetPrev(m_posCurrent);
	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::GetCurPos() const
	{	return m_posCurrent;	}

template<class PtrType>
inline void CWPtrList<PtrType>::AddToList(CWPtrListNode<PtrType> * pNode, CWPtrListNode<PtrType> * pPrev, CWPtrListNode<PtrType> * pNext)
	{
		pPrev->pNext = pNode;
		pNext->pPrev = pNode;
		pNode->pPrev = pPrev;
		pNode->pNext = pNext;
		m_nDataCount ++;
	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::DeleteFromList(CWPtrListNode<PtrType> * pNode)
	{
		if (! pNode)
			return NULL;

		PtrType	pObject;

		pObject = pNode->pData;

		pNode->pPrev->pNext = pNode->pNext;
		pNode->pNext->pPrev = pNode->pPrev;

		FreeNode(pNode);
		m_nDataCount --;

		return pObject;
	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::GetNext(LISTPOS & pos) const
	{
		CWPtrListNode<PtrType>*	pNode = reinterpret_cast<CWPtrListNode<PtrType> *>(pos);
		PtrType	pObject;

		if (! pNode)
			return NULL;

		pObject = pNode->pData;
		pNode = pNode->pNext;
		if (pNode == m_pTail)
			pNode = NULL;

		pos = (LISTPOS) pNode;
		return pObject;
	}

template<class PtrType>
inline PtrType CWPtrList<PtrType>::GetPrev(LISTPOS & pos) const
	{
		CWPtrListNode<PtrType>*	pNode = reinterpret_cast<CWPtrListNode<PtrType> *>(pos);
		PtrType	pObject;

		if (! pNode)
			return NULL;

		pObject = pNode->pData;
		pNode = pNode->pPrev;
		if (pNode == m_pHead)
			pNode = NULL;

		pos = (LISTPOS) pNode;
		return pObject;
	}

template<class PtrType>
inline CWPtrListNode<PtrType>* CWPtrList<PtrType>::NewNode(PtrType pData)
	{
		if (! m_pFree)
		{
			m_pFree = new CWPtrListNode<PtrType>;
			m_pFree->pNext = NULL;
			m_pFree->pData = NULL;
		}

		CWPtrListNode<PtrType>* pNode = m_pFree;
		m_pFree = m_pFree->pNext;

		pNode->pPrev = NULL;
		pNode->pNext = NULL;
		pNode->pData = pData;

		return pNode;
	}

template<class PtrType>
inline void CWPtrList<PtrType>::FreeNode(CWPtrListNode<PtrType> * pNode)
	{
		pNode->pNext = m_pFree;
		m_pFree = pNode;
	}

template<class PtrType>
inline LISTPOS	CWPtrList<PtrType>::HeadNode() const
	{	return (LISTPOS) m_pHead;	}

template<class PtrType>
inline LISTPOS CWPtrList<PtrType>::TailNode() const
	{	return (LISTPOS) m_pTail;	}


#undef  WTEMPLATE_FLAG

#endif	//  __WINDLIB_PTRLIST_H
