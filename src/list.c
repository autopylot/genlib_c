#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "list.h"

void *NodeNew(int elemSize)
{
    node *newNode = malloc(sizeof(node));
    assert(newNode != NULL);
    newNode->content = malloc(elemSize);
    assert(newNode->content != NULL);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

node *NodeNext(node *n)
{
    if (n)
        return (n->next);
    return NULL;
}

node *NodePrev(node *n)
{
    if (n)
        return (n->prev);
    return NULL;
}

void *NodeContent(node *n)
{   
    if (n)
        return (n->content);
    return NULL;
}

void ListNew(list *l, int elemSize, ListFreeFunction freeFn)
{
    assert(elemSize > 0);
    assert(l != NULL);
    l->root = NULL;
    l->end = NULL;
    l->elemSize = elemSize;
    l->logical = 0;
    l->freeFn = freeFn;
}

void ListDispose(list *l)
{
    assert(l != NULL);
    node *curr = l->root;
    node *next = NULL;
    while (curr)
    {
        l->freeFn(curr->content);
        next = curr->next;
        free(curr);
        curr = next;
    }
    l->root = l->end = NULL;
    l->logical = 0;
}

int ListLength(const list *l)
{
    assert(l != NULL);
    return l->logical;
}

int ListEmpty(list *l)
{
    assert(l != NULL);
    return (l->logical ? 0 : 1);
}

node *ListBegin(list *l)
{
    if (l)
        return l->root;
    return NULL;
}

node *ListEnd(list *l)
{
    if (l)
        return l->end;
    return NULL;    
}

void *ListFront(list *l)
{
    assert(l != NULL);
    return l->root->content;
}

void *ListBack(list *l)
{
    assert(l != NULL);
    return l->end->content;
}

void ListPushFront(list *l, const void *elemAddr)
{
    assert(l != NULL && elemAddr != NULL);
    node *newNode = NodeNew(l->elemSize);
    memcpy(newNode->content, elemAddr, l->elemSize);
    l->logical++;
    if (l->root == NULL && l->end == NULL)
    {
        l->root = l->end = newNode;
        return;
    }
    newNode->next = l->root;
    l->root->prev = newNode;
    l->root = newNode;
}

void ListPopFront(list *l)
{
    assert(l != NULL);
    if (ListEmpty(l))
        return;
    node *tmp = l->root;
    if (l->root->next == NULL)
    {
        l->root = l->end = NULL;
    }
    else
    {
        l->root = l->root->next;
        l->root->prev = NULL;
    }
    l->logical--;
    void (*freeFn)(void*) = l->freeFn;
    l->freeFn(tmp->content);
    free(tmp);
}

void ListPushBack(list *l, const void *elemAddr)
{
    assert(l != NULL && elemAddr != NULL);
    node *newNode = NodeNew(l->elemSize);
    memcpy(newNode->content, elemAddr, l->elemSize);
    if (l->root == NULL && l->end == NULL)
    {
        l->root = l->end = newNode;
        return;
    }
    l->end->next = newNode;
    newNode->prev = l->end;
    l->end = newNode;
}

void ListPopBack(list *l)
{
    assert(l != NULL);
    if (ListEmpty(l))
        return;
    node *tmp = l->end;
    if (l->end->prev == NULL)
        l->root = l->end = NULL;
    else
    {
        l->end = l->end->prev;
        l->end->next = NULL;
    }
    l->logical--;
    l->freeFn(tmp->content);
    free(tmp);
}

void *ListInsert(list *l, node *position, void *elemAddr)
{
    node *newNode = NodeNew(l->elemSize);
    memcpy(newNode->content, elemAddr, l->elemSize);
    if (l->root == NULL && l->end == NULL)
    {
        l->root = l->end = newNode;
    }
    else if (l->root && l->root == position)
    {
        l->root->prev = newNode;
        newNode->next = l->root;
        l->root = newNode;
    }
    else if (l->end && position == NULL)
    {
        l->end->next = newNode;
        newNode->prev = l->end;
        l->end = newNode;
    }
    else
    {
        newNode->prev = position->prev;
        position->prev->next = newNode;
        newNode->next = position;
        position->prev = newNode;
    }
    return newNode;
}

void *ListErase(list *l, node *position)
{
    assert(position != NULL);
    assert(l != NULL);
    node *curr = NULL;
    if (l->root == l->end)
        l->root = l->end = NULL;
    else if (l->root == position) {
        l->root = position->next;
        l->root->prev = NULL;
        curr = l->root;
    }
    else if (position == l->end)
    {
        l->end = position->prev;
        l->end->next = NULL;
        curr = l->end;
    }
    else
    {
        position->prev->next = position->next;
        position->next->prev = position->prev;
        curr = position->next;
    }
    l->freeFn(position->content);
    free(position);
    return curr;
}

void ListRemoveIf(list *l, ListCmpFunction CmpFn, void *auxData)
{
    node *curr = l->root;
    while (curr)
    {
        if (CmpFn(curr->content, auxData))
        {
            curr = ListErase(l, curr);
        }
        else
            curr = curr->next;
    }
}

void ListMap(list *l, void (*mapFn)(void *elemAddr, void *), void *auxData)
{
    node *curr = l->root;
    while (curr)
    {
        mapFn(curr->content, auxData);
        curr = curr->next;
    }
}
