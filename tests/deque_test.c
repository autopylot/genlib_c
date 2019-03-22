#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <time.h>
#include "deque.h"

static void test_deque_at(void **state) {
    (void)state;
    Deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    deque_new(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++) {
        int tmp = rand() % 100;
        arr[i] = tmp;
        deque_push_back(&test_dq, &tmp);
    }
    for (int i = 0; i < 20; i++) {
        void *ret_val = deque_at(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
    }
    deque_dispose(&test_dq);
}

static void test_deque_push_back(void ** state) {
    (void)state;
    Deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    deque_new(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++) {
        int tmp = rand() % 100;
        arr[i] = tmp;
        deque_push_back(&test_dq, &tmp);
    }
    Node *start= deque_begin(&test_dq);
    for (int i = 0; i < 20; i++) {
        void * ret_val = deque_at(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], *(int*)get_node_content(start));
        start = next_node(start);
    }
    assert_int_equal(20, deque_size(&test_dq));
    deque_dispose(&test_dq);
}

static void test_deque_push_front(void ** state) {
    (void)state;
    Deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    deque_new(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++) {
        int tmp = rand() % 100;
        arr[20 - (i + 1)] = tmp;
        deque_push_front(&test_dq, &tmp);
    }
    Node *start = deque_begin(&test_dq);
    for (int i = 0; i < 20; i++) {
        void * ret_val = deque_at(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], get_node_content(start));
        start = next_node(start);
    }
    assert_int_equal(20, deque_size(&test_dq));
    deque_dispose(&test_dq);
}

static void test_deque_pop_back(void ** state) {
    (void)state;
    Deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    deque_new(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++) {
        int tmp = rand() % 100;
        arr[i] = tmp;
        deque_push_back(&test_dq, &tmp);
    }
    for (int i = 0; i < 5; i++)  {
        deque_pop_back(&test_dq);
    }
    Node *start = deque_begin(&test_dq);
    for (int i = 0; i < 15; i++) {
        void *ret_val = deque_at(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[1], *(int*)get_node_content(start));
        start = next_node(start);
    }
    assert_int_equal(15, deque_size(&test_dq));
    deque_dispose(&test_dq);
}

static void test_deque_pop_front(void ** state) {
    (void)state;
    Deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    deque_new(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++) {
        int tmp = rand() % 100;
        arr[i] = tmp;
        deque_push_back(&test_dq, &tmp);
    }
    for (int i = 0; i < 5; i++)  {
        deque_pop_front(&test_dq);
    }
    Node *start = deque_begin(&test_dq);
    for (int i = 5; i < 20; i++) {
        void *ret_val = deque_at(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], *(int*)get_node_content(start));
        start = next_node(start);
    }
    assert_int_equal(15, deque_size(&test_dq));
    deque_dispose(&test_dq);
}

static void test_deque_insert(void ** state) {
    (void)state;
    Deque test_dq;
    time_t t;
    int arr[5] = {0,1,2,3,4};
    
    srand(t);
    deque_new(&test_dq, sizeof(int), free);
    deque_push_back(&test_dq, &arr[1]);
    deque_push_back(&test_dq, &arr[3]);
    deque_insert(&test_dq, 0, &arr[0]);
    deque_insert(&test_dq, 2, &arr[2]);
    deque_insert(&test_dq, 4, &arr[4]);
    Node *start = deque_begin(&test_dq);
    for (int i = 0; i < 5; i++) {
        void *ret_val = deque_at(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], *(int*)get_node_content(start));
        start = next_node(start);
    }
    assert_int_equal(5, deque_size(&test_dq));
    deque_dispose(&test_dq);
}

static void test_deque_erase(void ** state) {
    (void)state;
    Deque test_dq;
    time_t t;
    int arr[20];

    srand(t);
    deque_new(&test_dq, sizeof(int), free);
    for (int i = 0; i < 20; i++) {
        int tmp = rand() % 100;
        arr[i] = tmp;
        deque_push_back(&test_dq, &tmp);
    }
    for (int i = 0; i < 5; i++)  {
        deque_pop_front(&test_dq);
    }
    Node *start = deque_begin(&test_dq);
    for (int i = 5; i < 20; i++) {
        void *ret_val = deque_at(&test_dq, i);
        assert_int_equal(arr[i], *(int*)ret_val);
        assert_int_equal(arr[i], *(int*)get_node_content(start));
        start = next_node(start);
    }
    assert_int_equal(15, deque_size(&test_dq));
    deque_dispose(&test_dq);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_deque_at),
        cmocka_unit_test(test_deque_push_back),
        cmocka_unit_test(test_deque_push_front),
        cmocka_unit_test(test_deque_pop_back),
        cmocka_unit_test(test_deque_pop_front),
        cmocka_unit_test(test_deque_insert),
        cmocka_unit_test(test_deque_erase)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}