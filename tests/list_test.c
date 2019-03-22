#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "list.h"

/**
 * Function: test_dispose
 * ---------------------
 * Tests if list_dispose correctly frees memory of elements in list and sets size to zero. list_dispose does not free the list only nodes within.
 */
static void test_dispose(void **state) {
    (void)state;
    List test_lst;
    int *content_ptrs[10];
    Node *curr = NULL;
    

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    curr = test_lst.root;
    for (int i = 0; i < 10; i++) {
        content_ptrs[i] = get_node_content(curr);
        curr = next_node(curr);
    }
    list_dispose(&test_lst);
    for (int i = 0; i < 10; i++) {
        assert_null(content_ptrs[i]);
    }
    assert_non_null(&test_lst);
    assert_null(list_begin(&test_lst));
    assert_null(list_end(&test_lst));
    assert_int_equal(list_len(&test_lst), 0);
}

/**
 * Function: test_push_front
 * -----------------------
 * Push numbers to the front of the list. Checks if the list is linked correctly forwards and backs, incrementing size, and linked correctly with one item.
 */
static void test_push_front(void **state) {
    (void)state;
    List test_lst;
    int j = 0;

    list_new(&test_lst, sizeof(int), free);
    list_push_front(&test_lst, &j);
    assert_non_null(list_begin(&test_lst));
    assert_non_null(list_end(&test_lst));
    assert_ptr_equal(list_begin(&test_lst), list_end(&test_lst));

    for (int i = 1; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    assert_int_equal(list_len(&test_lst), 10);
    Node *curr = list_begin(&test_lst);
    for (int i = 9; i >= 0; i--) {
        assert_int_equal(*((int *)get_node_content(curr)), i);
        curr = next_node(curr);
    }
    curr = list_end(&test_lst);
    for (int i = 0; i >= 9; i++) {
        assert_int_equal(*((int *)get_node_content(curr)), i);
        curr = prev_node(curr);
    }
    list_dispose(&test_lst);
}

/**
 * Function: test_pop_front
 * ----------------------
 * Tests if list_pop_front correctly moves the beginning of the list and completely unlinks the previous Node.
 */
static void test_pop_front(void **state) {
    (void)state;
    List test_lst;
    int list_size;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    for (int i = 9; i >= 0; i--) {
        assert_int_equal(*(int *)list_front(&test_lst), i);
        list_pop_front(&test_lst);
        assert_int_equal(list_len(&test_lst), i);
        assert_null(prev_node(list_begin(&test_lst)));
    }
    assert_null(list_begin(&test_lst));
    assert_null(list_end(&test_lst));
}

/**
 * Function: test_push_back
 * -----------------------
 * Tests if list_push_back inserts correctly into the back of the list by traversing forwards and backwards through.
 */
static void test_push_back(void **state) {
    (void)state;
    Node *curr;
    List test_lst;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_back(&test_lst, &i);
        if (i == 0)
            assert_ptr_equal(list_begin(&test_lst), list_end(&test_lst));
    }
    curr = list_begin(&test_lst);
    for (int i = 0; i < 10; i++) {
        assert_int_equal(*(int *)get_node_content(curr), i);
        curr = next_node(curr);
    }
    curr = list_end(&test_lst);
    for (int i = 9; i >= 0; i--) {
        assert_int_equal(*(int *)get_node_content(curr), i);
        curr = prev_node(curr);
    }
    assert_int_equal(list_len(&test_lst), 10);
    list_dispose(&test_lst);
}

/**
 * Function: test_pop_back
 * ----------------------
 * Tests if list_pop_back correctly moves the end ptr and unlinks next ptr to removed node. Calling list_pop_back on the only node in list should leave the begin and end ptr NULL.
 */
static void test_pop_back(void **state) {
    (void)state;
    List test_lst;
    int list_size = 0;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    for (int i = 0; i < 10; i++) {
        assert_int_equal(*(int *)list_back(&test_lst), i);
        assert_int_equal(list_len(&test_lst), 10 - i);
        list_pop_back(&test_lst);
        assert_null(next_node(list_end(&test_lst)));
    }
    assert_null(list_begin(&test_lst));
    assert_null(list_end(&test_lst));
}

/**
 * Function: test_insert
 * ---------------------
 * Tests if list_insert is correct by inserting values at the 0, 1, and end pos. Insert should place the value behind the given pos.
 */
static void test_insert(void **state) {
    (void)state;
    List test_lst;
    Node *curr;

    int test_ints[4] = {-1, 0, 1, 2};
    list_new(&test_lst, sizeof(int), free);
    list_insert(&test_lst, NULL, &test_ints[2]);
    assert_non_null(list_begin(&test_lst));
    assert_non_null(list_end(&test_lst));
    assert_ptr_equal(list_begin(&test_lst), list_end(&test_lst));
    list_insert(&test_lst, list_begin(&test_lst), &test_ints[0]);
    list_insert(&test_lst, list_end(&test_lst), &test_ints[1]);
    list_insert(&test_lst, NULL, &test_ints[3]);
    curr = list_begin(&test_lst);
    for (int i = -1; i <= 2; i++) {
        assert_int_equal(*(int *)get_node_content(curr), i);
        curr = next_node(curr);
    }
    curr = list_end(&test_lst);
    for (int i = 2; i >= -1; i--) {
        assert_int_equal(*(int *)get_node_content(curr), i);
        curr = prev_node(curr);
    }
    list_dispose(&test_lst);
}

/**
 * Function: test_erase
 * -------------------
 * Test if list_erase is correct by deleting nodes and iterating through.
 * Erasing the last node in a list should set both front and end to NULL.
 */
static void test_erase(void **state) {
    (void)state;
    Node *curr;
    List test_lst;
    int test_num[7] = {1,2,3,4,5,6,8};

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_back(&test_lst, &i);
    }
    list_erase(&test_lst, prev_node(prev_node(list_end(&test_lst)))); 
    list_erase(&test_lst, list_begin(&test_lst));
    list_erase(&test_lst, list_end(&test_lst));
    curr = list_begin(&test_lst);
    for (int i = 0; i < 7; i++) {
        assert_int_equal(*(int *)get_node_content(curr), test_num[i]);
        curr = next_node(curr);
    }
    curr = list_end(&test_lst);
    for (int i = 0; i < 7; i++) {
        assert_int_equal(*(int *)get_node_content(curr), test_num[6 - i]);
        curr = prev_node(curr);
    }
    for (int i = 0; i < 3; i++) {
        list_erase(&test_lst, list_begin(&test_lst));
        list_erase(&test_lst, list_end(&test_lst));
    }
    assert_int_equal(*(int *)list_front(&test_lst), 4);
    list_erase(&test_lst, list_begin(&test_lst));
    assert_null(list_begin(&test_lst));
    assert_null(list_end(&test_lst));
}

static int isEven(void *elem, void *elem2) {
    return (((*(int*)elem) % 2) == 0);
}

/**
 * Function: test_remove_if
 * ----------------------
 * Test if list_remove_if by removing all even integers and iterating through to check.
 */
static void test_remove_if(void **state) {
    (void)state;
    Node *curr;
    List test_lst;
    int test_num[5] = {1,3,5,7,9};

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_back(&test_lst, &i);
    }
    list_remove_if(&test_lst, isEven, 0);
    curr = list_begin(&test_lst);
    for (int i = 0; i < 5; i++) {
        assert_int_equal(*(int *)get_node_content(curr), test_num[i]);
        curr = next_node(curr);
    }
    list_dispose(&test_lst);
}

static void add(void *elem, void *aux) {
    *(int*)elem = (*(int*)elem) + (*(int*)aux);
}

/**
 * Function: test_map
 * -----------------
 * Maps an add function over the list adding x to each value.
 */
static void test_map(void **state) {
    (void)state;
    Node *curr;
    List test_lst;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_back(&test_lst, &i);
    }
    int i = 1;
    list_map(&test_lst, add, &i);
    curr = list_begin(&test_lst);
    for (int i = 0; i < 10; i++) {
        assert_int_equal(*(int*)get_node_content(curr), i + 1);
        curr = next_node(curr);
    }
    list_dispose(&test_lst);
}

/**
 * Function: test_empty
 * ---------------------
 * Checks if list is empty.
 */
static void test_empty(void **state) {
    (void)state;
    List test_lst;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    assert_int_equal(list_empty(&test_lst), 0);
    list_dispose(&test_lst);
    assert_int_equal(list_empty(&test_lst), 1);
}

static void test_begin(void **state) {
    (void)state;
    List test_lst;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    assert_ptr_equal(list_begin(&test_lst), test_lst.root);
    list_dispose(&test_lst);
}

static void test_end(void **state) {
    (void)state;
    List test_lst;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    assert_ptr_equal(list_end(&test_lst), test_lst.end);
    list_dispose(&test_lst);
}

static void test_front(void **state) {
    (void)state;
    List test_lst;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    assert_int_equal((*(int *)list_front(&test_lst)), 9);
}

static void TestBack(void **state) {
    (void)state;
    List test_lst;

    list_new(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++) {
        list_push_front(&test_lst, &i);
    }
    assert_int_equal((*(int *)list_front(&test_lst)), 9);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_push_front),
        cmocka_unit_test(test_pop_front),
        cmocka_unit_test(test_pop_back),
        cmocka_unit_test(test_dispose),
        cmocka_unit_test(test_empty),
        cmocka_unit_test(test_begin),
        cmocka_unit_test(test_insert),
        cmocka_unit_test(test_erase),
        cmocka_unit_test(test_remove_if),
        cmocka_unit_test(test_map),

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
