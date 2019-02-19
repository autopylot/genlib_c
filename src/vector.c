#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    assert(initialAllocation >= 0 && elemSize > 0);
    if (initialAllocation == 0) {
        v->elems = malloc(4 * elemSize);
        v->allocated = 4;
    }
    else {
        v->elems = malloc(initialAllocation * elemSize);
        v->allocated = initialAllocation;
    }
    assert(v->elems != NULL);
    v->elemSize = elemSize;
    v->logical = 0;
    v->freeFn = freeFn;
}

void VectorDispose(vector *v)
{
    if (v->freeFn && v->logical > 0) {
        for (int i = 0; i < v->logical; i++) {
            v->freeFn((char*)v->elems + i * v->elemSize);
        }
    }
}

// element access

void *VectorNth(const vector *v, int position)
{
    assert(position >= 0 && position < v->logical && v != NULL);
    return (char*)v->elems + position * v->elemSize;
}

// capacity

int VectorLength(const vector *v)
{
    assert(v != NULL);
    return v->logical;
}


// modifiers

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    assert(position >= 0 && position < v->logical && v != NULL);
    void *replAddr = (char*)v->elems + position * v->elemSize;
    if (v->freeFn)
        v->freeFn(replAddr);
    memcpy(replAddr, elemAddr, v->elemSize);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
    void *tmp;
    assert(position >= 0 && !(position > v->logical) && v != NULL);
    
    if (v->logical == v->allocated) {
        if (v->allocated == 0)
            v->allocated = 4;
        else
            v->allocated *= 2;
        tmp = realloc(v->elems, v->allocated * v->elemSize);
        assert(tmp != NULL);
        v->elems = tmp;
    }

    char *insertAddr = (char *)v->elems + position * v->elemSize;

    if (position != v->logical) {
        char *dst = insertAddr + v->elemSize;
        char *end = (char*)v->elems + v->logical * v->elemSize;
        memmove(dst, insertAddr, (end - insertAddr));
    }
    memcpy(insertAddr, elemAddr, v->elemSize);
    v->logical++;
}

void VectorAppend(vector *v, const void *elemAddr)
{
    assert(v != NULL);
    VectorInsert(v, elemAddr, v->logical);
}

void VectorDelete(vector *v, int position)
{
    assert (position >= 0 && position < v->logical && v != NULL);
    void *elemAddr = (char*)v->elems + position * v->elemSize;  
    if (v->freeFn != NULL)
        v->freeFn(elemAddr);
    char *dst = (char*)elemAddr + v->elemSize;
    memmove(elemAddr, dst, (v->logical - 1 - position) * v->elemSize);
    v->logical--;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    assert(compare != NULL && v != NULL);
    qsort(v->elems, v->logical, v->elemSize, compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    assert(mapFn != NULL);
    for (int i = 0; i < v->logical; i++) {
        mapFn((char*)v->elems + i * v->elemSize, auxData);
    }
}

static const int kNotFound = -1;
int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ 
    assert(key != NULL && searchFn != NULL && startIndex >= 0 && startIndex <= v->logical);
    if (isSorted) {
        char *elemAddr = (char*)bsearch(key, VectorNth(v,startIndex), v->logical, v->elemSize, searchFn);
        if (elemAddr)
            return ((elemAddr - (char*)v->elems) / v->elemSize);
        else
            return kNotFound;
    }
    else {
        for (int i = 0; i < v->logical; i++) {
            if (searchFn(key, (char*)v->elems + i * v->elemSize) == 0) {
                return i;
            }
        }
        return kNotFound;
    }    
} 
