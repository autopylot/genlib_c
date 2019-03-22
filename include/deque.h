#ifndef _deque_
#include "list.h"
#include "vector.h"

typedef void (*DequeFreeFunction)(void *elemAddr);

typedef struct deque
{
    vector elemIndex;
    List elems;
} Deque;

void deque_new(Deque *dq, int elemSize, DequeFreeFunction freeFn);
void deque_dispose(Deque *dq);
void *deque_at(Deque *dq, int idx);
void *deque_front(Deque *dq);
void *deque_back(Deque *dq);
Node *deque_begin(Deque *dq);
Node *deque_end(Deque *dq);
int deque_size(Deque *dq);
int deque_empty(Deque *dq);
void deque_push_back(Deque *dq, void *elemAddr);
void deque_push_front(Deque *dq, void *elemAddr);
void deque_pop_back(Deque *dq);
void deque_pop_front(Deque *dq);
void deque_insert(Deque *dq, int position, void *elemAddr);
Node *deque_erase(Deque *dq, int position);

#endif
