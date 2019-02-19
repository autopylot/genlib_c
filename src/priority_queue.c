#include "priority_queue.h"

void PrQueueNew(priority_queue *prq, int elemSize, RbtCompareFn cmpFn, RbtFreeFn freeFn)
{
    RbTreeNew(&(prq->container), elemSize, cmpFn, freeFn);
}

void PrQueueDispose(priority_queue *prq) {
    RbtDispose(&(prq->container));
}

int PrQueueEmpty(priority_queue *prq)
{
    return RbtEmpty(prq->container);
}

int PrQueueSize(priority_queue *prq)
{
    return (getSize(prq->container));
}

void *PrQueueTop(priority_queue *prq)
{
    return minValueNode(getRoot(&(prq->container)));
}

void PrQueuePush(priority_queue *prq, void *elemAddr)
{
    RbtInsert(&(prq->container), elemAddr);
}

void *PrQueuePop(priority_queue *prq) {
    rb_node *min_node;
    void *ret_content;

    min_node = minValueNode(getRoot(&(prq->container)));
    ret_content = getContent(min_node);
    RbtDelete(&(prq->container), min_node);
    return ret_content;
}