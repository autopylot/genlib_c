#ifndef UNIT_TESTING

#include <malloc.h>

extern void *_test_malloc(const size_t size, const char *file, const int line);
extern void _test_free(void *const ptr, const char *file, const int line);

#define malloc(size) _test_malloc(size, __FILE__, __LINE__)
#define free(ptr) _test_free(ptr, __FILE__, __LINE__)

#endif