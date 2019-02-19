#include "deque.h"

void DequeNew(deque *dq, int elemSize, DequeFreeFunction freeFn)
{
    VectorNew(&(dq->elemIndex), sizeof(void *), NULL, 2);
    ListNew(&(dq->elems), elemSize, freeFn);
}

void DequeDispose(deque *dq)
{
    VectorDispose(&(dq->elemIndex));
    ListDispose(&(dq->elems));
}

// element access

void *DequeAt(deque *dq, int idx)
{
    return ((node *)VectorNth(&(dq->elemIndex), idx))->content;
}

void *DequeFront(deque *dq)
{
    return ListFront(&(dq->elems));
}

void *DequeBack(deque *dq)
{
    return ListBack(&(dq->elems));
}

// iterators

node *DequeBegin(deque *dq)
{
    return ListBegin(&(dq->elems));
}

node *DequeEnd(deque *dq)
{
    return ListEnd(&(dq->elems));
}

// capacity

int DequeSize(deque *dq)
{
    return VectorLength(&(dq->elemIndex));
}

int DequeEmpty(deque *dq)
{
    return ListEmpty(&(dq->elems));
}

// modifiers

void DequePushBack(deque *dq, void *elemAddr)
{
    ListPushBack(&(dq->elems), elemAddr);
    VectorAppend(&(dq->elemIndex), ListEnd(&(dq->elems)));
}

void DequePushFront(deque *dq, void *elemAddr)
{
    ListPushFront(&(dq->elems), elemAddr);
    VectorInsert(&(dq->elemIndex), ListFront(&(dq->elems)), 0);
}

void DequePopBack(deque *dq)
{
    ListPopBack(&(dq->elems));
    VectorDelete(&(dq->elemIndex), (VectorLength(&(dq->elemIndex)) - 1));
}

void DequePopFront(deque *dq)
{
    ListPopFront(&(dq->elems));
    VectorDelete(&(dq->elemIndex), 0);
}

void DequeInsert(deque *dq, int position, void *elemAddr)
{
    node *newStart = ListInsert(&(dq->elems), VectorNth(&(dq->elemIndex), position), elemAddr);
    VectorInsert(&(dq->elemIndex), newStart, position);
}

node *DequeErase(deque *dq, int position)
{
    assert(dq != NULL);
    assert(position >= 0 && !(position > VectorLength(&dq->elemIndex)));
    node *nextElem = ListErase(&(dq->elems), ((node *)VectorNth(&(dq->elemIndex), position)));
    VectorDelete(&(dq->elemIndex), position);
    return nextElem;
}
