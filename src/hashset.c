#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, 
		HashSetCompareFunction comparefn, 
		HashSetFreeFunction freefn)
{
	assert(elemSize > 0 && numBuckets > 0 && hashfn != NULL && comparefn != NULL);
	h->numBuckets = numBuckets;
	h->hashfn = hashfn;
	h->cmpfn = comparefn;
	h->freefn = freefn;
	h->size = 0;
	h->elems = malloc(numBuckets * sizeof(vector));
	assert(h->elems != NULL);
	for (int i = 0; i < numBuckets; i++) {
		VectorNew(&((vector*)h->elems)[i], elemSize, freefn, 1);
	}
}

void HashSetDispose(hashset *h)
{
	for (int i = 0; i < h->numBuckets; i++)
		VectorDispose(&((vector*)h->elems)[i]);
	free(h->elems);
}

int HashSetCount(const hashset *h)
{ 
	return h->size;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
	assert(mapfn != NULL);
	for (int i = 0; i < h->numBuckets; i++) {
		VectorMap(&((vector *)h->elems)[i], mapfn, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
	int foundIdx;

	assert(elemAddr != NULL);
	int hashKey = h->hashfn(elemAddr, h->numBuckets);
	assert(hashKey >= 0 && hashKey < h->numBuckets);
	foundIdx = VectorSearch(&((vector *)h->elems)[hashKey], elemAddr, h->cmpfn, 0, false);
	if (foundIdx != -1){
		VectorReplace(&((vector *)h->elems)[hashKey], elemAddr, foundIdx);
	}
	else
		VectorAppend(&((vector *)h->elems)[hashKey], elemAddr);
	h->size++;
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{
	assert(elemAddr != NULL);
	int hashKey = h->hashfn(elemAddr, h->numBuckets);
	assert(hashKey >= 0 && hashKey < h->numBuckets);

	int idx = VectorSearch(&((vector *)h->elems)[hashKey], elemAddr, h->cmpfn, 0, false);
	if (idx >= 0)
		return VectorNth(&((vector *)h->elems)[hashKey], idx);
	else
		return NULL;
}
