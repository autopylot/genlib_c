#ifndef _priority_queue_

#include "rb_tree.h"
typedef bool (*PriorityCompareFn)(void *elemAddr1, void *elemAddr2);
typedef void (*PriorityFreeFn)(void *elemAddr);

typedef struct priority_queue
{
    rb_tree container;
    rb_node *top;
    // int elemSize;
    // int logical;
    // PriorityCompareFn cmpFn;
    // PriorityFreeFn freeFn;
} priority_queue;

void PrQueueNew(priority_queue *prq, int elemSize, RbtCompareFn cmpFn, RbtFreeFn freeFn);
int PrQueueEmpty(priority_queue *prq);
int PrQueueSize(priority_queue *prq);
void *PrQueueTop(priority_queue *prq);
#endif //_priority_queue_
