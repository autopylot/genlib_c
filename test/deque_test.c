#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <time.h>
#include "deque.h"

static void TestDequeAt(void **state)
{
    (void)state;
    deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    DequeNew(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++)
    {
        int tmp = rand() % 100;
        arr[i] = tmp;
        DequePushBack(&test_dq, &tmp);
    }
    for (int i = 0; i < 20; i++)
    {
        void *ret_val = DequeAt(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
    }
    DequeDispose(&test_dq);
}

// modifiers

static void TestDequePushBack(void ** state)
{
    (void)state;
    deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    DequeNew(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++)
    {
        int tmp = rand() % 100;
        arr[i] = tmp;
        DequePushBack(&test_dq, &tmp);
    }
    node *start= DequeBegin(&test_dq);
    for (int i = 0; i < 20; i++) {
        void * ret_val = DequeAt(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], NodeContent(start));
        start = NodeNext(start);
    }
    assert_int_equal(20, DequeSize(&test_dq));
    DequeDispose(&test_dq);
}

static void TestDequePushFront(void ** state)
{
    (void)state;
    deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    DequeNew(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++)
    {
        int tmp = rand() % 100;
        arr[20 - (i + 1)] = tmp;
        DequePushFront(&test_dq, &tmp);
    }
    node *start = DequeBegin(&test_dq);
    for (int i = 0; i < 20; i++) {
        void * ret_val = DequeAt(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], NodeContent(start));
        start = NodeNext(start);
    }
    assert_int_equal(20, DequeSize(&test_dq));
    DequeDispose(&test_dq);
}

static void TestDequePopBack(void ** state)
{
    (void)state;
    deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    DequeNew(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++)
    {
        int tmp = rand() % 100;
        arr[i] = tmp;
        DequePushBack(&test_dq, &tmp);
    }
    for (int i = 0; i < 5; i++) 
    {
        DequePopBack(&test_dq);
    }
    node *start = DequeBegin(&test_dq);
    for (int i = 0; i < 15; i++)
    {
        void *ret_val = DequeAt(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[1], *(int*)NodeContent(start));
        start = NodeNext(start);
    }
    assert_int_equal(15, DequeSize(&test_dq));
    DequeDispose(&test_dq);
}

static void TestDequePopFront(void ** state)
{
    (void)state;
    deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    DequeNew(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++)
    {
        int tmp = rand() % 100;
        arr[i] = tmp;
        DequePushBack(&test_dq, &tmp);
    }
    for (int i = 0; i < 5; i++) 
    {
        DequePopFront(&test_dq);
    }
    node *start = DequeBegin(&test_dq);
    for (int i = 5; i < 20; i++)
    {
        void *ret_val = DequeAt(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], *(int*)NodeContent(start));
        start = NodeNext(start);
    }
    assert_int_equal(15, DequeSize(&test_dq));
    DequeDispose(&test_dq);
}

static void TestDequeInsert(void ** state)
{
    (void)state;
    deque test_dq;
    time_t t;
    int arr[5] = [0,1,2,3,4];
    
    srand(t);
    DequeNew(&test_dq, sizeof(int), free);
    DequePushBack(&test_dq, &arr[1]);
    DequePushBack(&test_dq, &arr[3]);
    DequeInsert(&test_dq, &arr[0], 0);
    DequeInsert(&test_dq, &arr[2], 2);
    DequeInsert(&test_dq, &arr[4], 4);
    node *start = DequeBegin(&test_dq);
    for (int i = 0; i < 5; i++)
    {
        void *ret_val = DequeAt(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], *(int*)NodeContent(start));
        start = NodeNext(start);
    }
    assert_int_equal(5, DequeSize(&test_dq));
    DequeDispose(&test_dq);
}

static void TestDequeErase(void ** state)
{
    (void)state;
    deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    DequeNew(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++)
    {
        int tmp = rand() % 100;
        arr[i] = tmp;
        DequePushBack(&test_dq, &tmp);
    }
    for (int i = 0; i < 5; i++) 
    {
        DequePopFront(&test_dq);
    }
    node *start = DequeBegin(&test_dq);
    for (int i = 5; i < 20; i++)
    {
        void *ret_val = DequeAt(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], *(int*)NodeContent(start));
        start = NodeNext(start);
    }
    assert_int_equal(15, DequeSize(&test_dq));
    DequeDispose(&test_dq);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(TestDequeAt),
        cmocka_unit_test(TestDequePushBack),
        cmocka_unit_test(TestDequePushFront),
        cmocka_unit_test(TestDequePopBack),
        cmocka_unit_test(TestDequePopFront),
        cmocka_unit_test(TestDequeInsert),
        cmocka_unit_test(TestDequeErase)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}