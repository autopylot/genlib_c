#include <stdlib.h>
#include <assert.h>
#include "deque.h"

void deque_new(Deque *dq, int elemSize, DequeFreeFunction freeFn) {
    VectorNew(&(dq->elemIndex), sizeof(void *), NULL, 2);
    list_new(&(dq->elems), elemSize, freeFn);
}

void deque_dispose(Deque *dq) {
    VectorDispose(&(dq->elemIndex));
    list_dispose(&(dq->elems));
}

// element access

void *deque_at(Deque *dq, int idx) {
    return ((Node *)VectorNth(&(dq->elemIndex), idx))->content;
}

void *deque_front(Deque *dq) {
    return list_front(&(dq->elems));
}

void *deque_back(Deque *dq) {
    return list_back(&(dq->elems));
}

// iterators

Node *deque_begin(Deque *dq) {
    return list_begin(&(dq->elems));
}

Node *deque_end(Deque *dq) {
    return list_end(&(dq->elems));
}

// capacity

int deque_size(Deque *dq) {
    return VectorLength(&(dq->elemIndex));
}

int deque_empty(Deque *dq) {
    return list_empty(&(dq->elems));
}

// modifiers

void deque_push_back(Deque *dq, void *elemAddr) {
    list_push_back(&(dq->elems), elemAddr);
    VectorAppend(&(dq->elemIndex), list_end(&(dq->elems)));
}

void deque_push_front(Deque *dq, void *elemAddr) {
    list_push_front(&(dq->elems), elemAddr);
    VectorInsert(&(dq->elemIndex), list_front(&(dq->elems)), 0);
}

void deque_pop_back(Deque *dq) {
    list_pop_back(&(dq->elems));
    VectorDelete(&(dq->elemIndex), (VectorLength(&(dq->elemIndex)) - 1));
}

void deque_pop_front(Deque *dq) {
    list_pop_front(&(dq->elems));
    VectorDelete(&(dq->elemIndex), 0);
}

void deque_insert(Deque *dq, int position, void *elemAddr) {
    Node *new_start = list_insert(&(dq->elems), VectorNth(&(dq->elemIndex), position), elemAddr);
    VectorInsert(&(dq->elemIndex), new_start, position);
}

Node *deque_erase(Deque *dq, int position) {
    assert(position >= 0 && !(position > VectorLength(&dq->elemIndex)));
    Node *nextElem = list_erase(&(dq->elems), ((Node *)VectorNth(&(dq->elemIndex), position)));
    VectorDelete(&(dq->elemIndex), position);
    return nextElem;
}
