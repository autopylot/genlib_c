#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "list.h"

/**
 * Function: TestDispose
 * ---------------------
 * Tests if ListDispose correctly frees memory of elements in List and resets size. ListDispose does not free the List only elements within.
 */
static void TestDispose(void **state)
{
    (void)state;
    list *test_lst;

    test_lst = malloc(sizeof(list));
    ListNew(test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(test_lst, &i);
    }
    ListDispose(test_lst);
    assert_non_null(test_lst);
    assert_null(ListBegin(test_lst));
    assert_null(ListEnd(test_lst));
    assert_int_equal(ListLength(test_lst), 0);
    free(test_lst);
}

/**
 * Function: TestPushFront
 * -----------------------
 * Push numbers to the front of the list.
 * Checks if the list is linked in order forwards and backs, incrementing size, and linked correctly with one item.
 */
static void TestPushFront(void **state)
{
    (void)state;
    list test_lst;
    int j = 0;

    ListNew(&test_lst, sizeof(int), free);
    ListPushFront(&test_lst, &j);
    assert_non_null(ListBegin(&test_lst));
    assert_non_null(ListEnd(&test_lst));
    assert_ptr_equal(ListBegin(&test_lst), ListEnd(&test_lst));

    for (int i = 1; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    assert_int_equal(ListLength(&test_lst), 10);
    node *curr = ListBegin(&test_lst);
    for (int i = 9; i >= 0; i--)
    {
        assert_int_equal(*((int *)NodeContent(curr)), i);
        curr = NodeNext(curr);
    }
    ListDispose(&test_lst);
}

/**
 * Function: TestPopFront
 * ----------------------
 * Tests if PopFront correctly moves the beginning of the list and completely unlinks the previous node.
 */
static void TestPopFront(void **state)
{
    (void)state;
    list test_lst;
    int list_size;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    for (int i = 9; i >= 0; i--)
    {
        assert_int_equal(*(int *)ListFront(&test_lst), i);
        ListPopFront(&test_lst);
        assert_int_equal(ListLength(&test_lst), i);
        assert_null(NodePrev(ListBegin(&test_lst)));
    }
    assert_null(ListBegin(&test_lst));
    assert_null(ListEnd(&test_lst));
}

/**
 * Function: TestPushBack
 * -----------------------
 * Tests if PushBack inserts correctly into the back of the list by traversing forwards and backwards through.
 */
static void TestPushBack(void **state)
{
    (void)state;
    node *curr;
    list test_lst;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushBack(&test_lst, &i);
        if (i == 0)
            assert_ptr_equal(ListBegin(&test_lst), ListEnd(&test_lst));
    }
    curr = ListBegin(&test_lst);
    for (int i = 0; i < 10; i++)
    {
        assert_int_equal(*(int *)NodeContent(curr), i);
        curr = NodeNext(curr);
    }
    curr = ListEnd(&test_lst);
    for (int i = 9; i >= 0; i--)
    {
        assert_int_equal(*(int *)NodeContent(curr), i);
        curr = NodePrev(curr);
    }
    assert_int_equal(ListLength(&test_lst), 26);
    ListDispose(&test_lst);
}

/**
 * Function: TestPopBack
 * ----------------------
 * Tests if PopBack correctly moves the end ptr and unlinks next ptr to removed node. Calling PopBack on the only node in list should leave the begin and end ptr NULL.
 */
static void TestPopBack(void **state)
{
    (void)state;
    list test_lst;
    int list_size;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    for (int i = 0; i < 10; i++)
    {
        assert_int_equal(*(int *)ListBack(&test_lst), i);
        assert_int_equal(ListLength(&test_lst), 10 - i);
        ListPopBack(&test_lst);
        assert_null(NodeNext(ListEnd(&test_lst)));
    }
    assert_null(ListBegin(&test_lst));
    assert_null(ListEnd(&test_lst));
}

/**
 * Function: TestInsert
 * ---------------------
 * Tests if Insert is correct by inserting values at the 0, 1, and end pos. Insert should place the value behind the given pos.
 */
static void TestInsert(void **state)
{
    (void)state;
    list test_lst;
    node *curr;

    int test_ints[4] = {-1, 0, 1, 2};
    ListNew(&test_lst, sizeof(int), free);
    ListInsert(&test_lst, NULL, &test_ints[2]);
    assert_non_null(ListBegin(&test_lst));
    assert_non_null(ListEnd(&test_lst));
    assert_ptr_equal(ListBegin(&test_lst), ListEnd(&test_lst));
    ListInsert(&test_lst, ListBegin(&test_lst), &test_ints[0]);
    ListInsert(&test_lst, ListEnd(&test_lst), &test_ints[1]);
    ListInsert(&test_lst, NULL, &test_ints[3]);
    curr = ListBegin(&test_lst);
    for (int i = -1; i <= 2; i++)
    {
        assert_int_equal(*(int *)NodeContent(curr), i);
        curr = NodeNext(curr);
    }
    curr = ListEnd(&test_lst);
    for (int i = 2; i >= -1; i--)
    {
        assert_int_equal(*(int *)NodeContent(curr), i);
        curr = NodePrev(curr);
    }
    ListDispose(&test_lst);
}

/**
 * Function: TestErase
 * -------------------
 * Test if Erase is correct by deleting nodes and iterating through.
 * Erasing the last node in a list should set both front and end to NULL.
 */
static void TestErase(void **state)
{
    (void)state;
    node *curr;
    list test_lst;
    int test_num[7] = {1,2,3,4,5,6,8};

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushBack(&test_lst, &i);
    }
    ListErase(&test_lst, NodePrev(NodePrev(ListEnd(&test_lst)))); 
    ListErase(&test_lst, ListBegin(&test_lst));
    ListErase(&test_lst, ListEnd(&test_lst));
    curr = ListBegin(&test_lst);
    for (int i = 0; i < 7; i++) {
        assert_int_equal(*(int *)NodeContent(curr), test_num[i]);
        curr = NodeNext(curr);
    }
    curr = ListEnd(&test_lst);
    for (int i = 0; i < 7; i++)
    {
        assert_int_equal(*(int *)NodeContent(curr), test_num[6 - i]);
        curr = NodePrev(curr);
    }
    for (int i = 0; i < 3; i++) {
        ListErase(&test_lst, ListBegin(&test_lst));
        ListErase(&test_lst, ListEnd(&test_lst));
    }
    assert_int_equal(*(int *)ListFront(&test_lst), 4);
    ListErase(&test_lst, ListBegin(&test_lst));
    assert_null(ListBegin(&test_lst));
    assert_null(ListEnd(&test_lst));
}

static int isEven(void *elem, void *elem2) {
    return (((*(int*)elem) % 2) == 0);
}

/**
 * Function: TestRemoveIf
 * ----------------------
 * Test if RemoveIf by removing all even integers and iterating through to check.
 */
static void TestRemoveIf(void **state)
{
    (void)state;
    node *curr;
    list test_lst;
    int test_num[5] = {1,3,5,7,9};

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushBack(&test_lst, &i);
    }
    ListRemoveIf(&test_lst, isEven, 0);
    curr = ListBegin(&test_lst);
    for (int i = 0; i < 5; i++) {
        assert_int_equal(*(int *)NodeContent(curr), test_num[i]);
        curr = NodeNext(curr);
    }
    ListDispose(&test_lst);
}

static void add(void *elem, void *aux) {
    *(int*)elem = (*(int*)elem) + (*(int*)aux);
}

/**
 * Function: TestMap
 * -----------------
 * Maps an add function over the list adding x to each value.
 */
static void TestMap(void **state)
{
    (void)state;
    node *curr;
    list test_lst;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushBack(&test_lst, &i);
    }
    int i = 1;
    ListMap(&test_lst, add, &i);
    curr = ListBegin(&test_lst);
    for (int i = 0; i < 10; i++) {
        assert_int_equal(*(int*)NodeContent(curr), i + 1);
        curr = NodeNext(curr);
    }
    ListDispose(&test_lst);
}

static void TestEmpty(void **state)
{
    (void)state;
    list test_lst;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    assert_int_equal(ListEmpty(&test_lst), 0);
    ListDispose(&test_lst);
    assert_int_equal(ListEmpty(&test_lst), 1);
}

static void TestBegin(void **state)
{
    (void)state;
    list test_lst;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    assert_ptr_equal(ListBegin(&test_lst), test_lst.root);
    ListDispose(&test_lst);
}

static void TestEnd(void **state)
{
    (void)state;
    list test_lst;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    assert_ptr_equal(ListEnd(&test_lst), test_lst.end);
    ListDispose(&test_lst);
}

static void TestFront(void **state)
{
    (void)state;
    list test_lst;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    assert_int_equal((*(int *)ListFront(&test_lst)), 9);
}

static void TestBack(void **state)
{
    (void)state;
    list test_lst;

    ListNew(&test_lst, sizeof(int), free);
    for (int i = 0; i < 10; i++)
    {
        ListPushFront(&test_lst, &i);
    }
    assert_int_equal((*(int *)ListFront(&test_lst)), 9);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(TestPushFront),
        cmocka_unit_test(TestPopFront),
        cmocka_unit_test(TestPopBack),
        cmocka_unit_test(TestDispose),
        cmocka_unit_test(TestEmpty),
        cmocka_unit_test(TestBegin),
        cmocka_unit_test(TestInsert),
        cmocka_unit_test(TestErase),
        cmocka_unit_test(TestRemoveIf),
        cmocka_unit_test(TestMap),

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
