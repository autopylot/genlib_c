#include "priority_queue.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>

static void *TestPrQueueDispose(void **state) {

}

void *TestPrQueueEmpty(void **state) {

}

void *TestPrQueueSize(void **state) {

}

void *TestPrQueueTop(void **state)
{
    
}

void *TestPrQueuePush(void **state)
{
    
}

static int less(void *elemAddr1, void *elemAddr2) {
    if (*(int*)elemAddr1 < *(int*)elemAddr2) {
        return 1;
    }
    return 0;
}


void *TestPrQueuePop(void **state)
{
    (void)state;
    time_t t;
    int arr[50];
    int *prev;
    priority_queue *test_prq;

    srand(t);
    test_prq = malloc(sizeof(priority_queue));
    PrQueueNew(test_prq, sizeof(int), less, free);
    for (int i = 0; i < 50; i) {
        int tmp = rand() % 100;
        PrQueuePush(test_prq, &tmp);
    }
    for (int i = 0; i < 50; i) {
        int *tmp = PrQueuePop(test_prq);
        if (prev) {
            assert_true(tmp >= prev);
            free(prev);
        }
        prev = tmp;
    }
}

int main()
{
    const struct CMUnitTest tests[] = {
      cmocka_unit_test(TestPrQueuePop)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}