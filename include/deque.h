#ifndef _deque_
#include "list.h"
#include "vector.h"

typedef void (*DequeFreeFunction)(void *elemAddr);

typedef struct deque
{
    vector elemIndex;
    list elems;
} deque;

void DequeNew(deque *dq, int elemSize, DequeFreeFunction freeFn);
void DequeDispose(deque *dq);
node *DequeBegin(deque *dq);
node *DequeEnd(deque *dq);
int DequeSize(deque *dq);
int DequeEmpty(deque *dq);
void *DequeAt(deque *dq, int idx);
void *DequeFront(deque *dq);
void *DequeBack(deque *dq);
void DequePushBack(deque *dq, void *elemAddr);
void DequePushFront(deque *dq, void *elemAddr);
void DequePopBack(deque *dq);
void DequeInsert(deque *dq, int position, void *elemAddr);
node *DequeErase(deque *dq, int position);

#endif //_deque_
