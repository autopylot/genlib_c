#ifndef _list_
#define _list_

typedef void (*ListFreeFunction)(void *elemAddr);
typedef int (*ListCmpFunction)(void *elemAddr1, void *elemAddr2);

typedef struct node node;

typedef struct node
{
    void *content;
    node *prev;
    node *next;
} node;

typedef struct
{
    node *root;
    node *end;
    int elemSize;
    int logical;
    ListFreeFunction freeFn;
} list;

void *NodeNew(int elemSize);
node *NodeNext(node *n);
node *NodePrev(node *n);
void *NodeContent(node *n);
void ListNew(list *l, int elemSize, ListFreeFunction freeFn);
void ListDispose(list *l);
int ListLength(const list *l);
int ListEmpty(list *l);
node *ListBegin(list *l);
node *ListEnd(list *l);
void *ListFront(list *l);
void *ListBack(list *l);
void ListPushFront(list *l, const void *elemAddr);
void ListPopFront(list *l);
void ListPushBack(list *l, const void *elemAddr);
void ListPopBack(list *l);
void *ListInsert(list *l, node *position, void *elemAddr);
void *ListErase(list *l, node *position);
void ListRemoveIf(list *l, ListCmpFunction CmpFn, void *auxData);
void ListMap(list *l, void (*mapFn)(void *elemAddr, void *), void *auxData);

#endif