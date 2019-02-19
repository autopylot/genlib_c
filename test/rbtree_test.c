#include "rb_tree.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print2DUtil(rb_node *root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += 10; 
  
    // Process right child first 
    print2DUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (int i = 10; i < space; i++) 
        printf(" "); 
    printf("(%d, %c)\n", *(int*)root->content, root->color == RED ? 'r' : 'b'); 
  
    // Process left child 
    print2DUtil(root->left, space); 
} 

void print2D(rb_node *root) 
{ 
    if (root == NULL)
        printf("Empty\n");
   // Pass initial space count as 0 
   else
        print2DUtil(root, 0); 
} 

int cmpInt(void *elemAddr1, void *elemAddr2) {
    return (*(int*)elemAddr1 < *(int*)elemAddr2);
}

int isInt(void *elemAddr1, void *elemAddr2) {
    if (*(int*)elemAddr1 < *(int*)elemAddr2)
        return 1;
    else if (*(int*)elemAddr1 > *(int*)elemAddr2)
        return -1;
    else
        return 0;
}

int checkRedBlackTree(rb_node *node) {
    int left_bheight = 0;
    int right_bheight = 0;

    if (node == NULL)
        return 0;
    
    left_bheight = checkRedBlackTree(getLeft(node));
    right_bheight = checkRedBlackTree(getRight(node));

    if (left_bheight == -1 || right_bheight == -1)
        return -1;
    else if ((getColor(node) == RED) && (getColor(getParent(node)) == RED))
        return -1;
    else if (left_bheight != right_bheight)
        return -1;
    else if (getColor(node) == BLACK)
        return (left_bheight + 1);
    else
        return left_bheight;
}

int isRedBlackTree(rb_tree *rbt) {
    if (getColor(getRoot(rbt)) != BLACK)
        return 0;
    int left_subtree = checkRedBlackTree(getLeft(getRoot(rbt)));
    int right_subtree = checkRedBlackTree(getRight(getRoot(rbt)));
    if (left_subtree < 0 || right_subtree < 0)
        return 0;
    else if (left_subtree != right_subtree)
        return 0;
    else
        return 1;
}

static void TestRedUncleInsert(void **state)
{
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[4] = {5,4,6,3};
    for (int i = 0; i < 4; i++) {
        RbtInsert(test_rbt, &arr[i]);
    }
    print2D(test_rbt->root);
    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 5);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 4);

    assert_int_equal(getColor(getLeft(getLeft(getRoot(test_rbt)))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getLeft(getRoot(test_rbt)))), 3);

    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 6);
    RbtDispose(test_rbt);
}

static void TestLLInsert(void **state)
{
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[4] = {5,4,6,3};
    for(int i = 0; i < 4; i++)
        RbtInsert(test_rbt, &arr[i]);
    print2D(test_rbt->root);
    
    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 5);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 4);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 6);
    
    assert_int_equal(getColor(getLeft(getLeft(getRoot(test_rbt)))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getLeft(getRoot(test_rbt)))), 3);
    RbtDispose(test_rbt);
}

static void TestLRInsert(void **state)
{
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[3] = {20,10,15};
    for (int i = 0; i < 3; i++)
        RbtInsert(test_rbt, &arr[i]);
    print2D(test_rbt->root);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 15);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 10);

    assert_int_equal(getColor(getRight(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 20);
    RbtDispose(test_rbt);
}

static void TestRRInsert(void **state)
{
     (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[3] = {10,15,20};
    for(int i = 0; i < 3; i++)
        RbtInsert(test_rbt, &arr[i]);
    print2D(test_rbt->root);
    
    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 15);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 10);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 20);
    RbtDispose(test_rbt);
}

static void TestMediumInsert(void **state)
{
     (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[9] = {8,5,15,12,19,9,13,23,10};
    for (int i = 0; i < 9; i++)
        RbtInsert(test_rbt, &arr[i]);
    print2D(test_rbt->root);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 12);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 8);

    assert_int_equal(getColor(getRight(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 15);

    assert_int_equal(getColor(getLeft(getLeft(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getLeft(getRoot(test_rbt)))), 5);

    assert_int_equal(getColor(getRight(getLeft(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getLeft(getRoot(test_rbt)))), 9);
    
    assert_int_equal(getColor(getRight(getRight(getLeft(getRoot(test_rbt))))), RED);
    assert_int_equal(*(int*)getContent(getRight(getRight(getLeft(getRoot(test_rbt))))), 10);

    assert_int_equal(getColor(getLeft(getRight(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRight(getRoot(test_rbt)))), 13);

    assert_int_equal(getColor(getRight(getRight(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRight(getRoot(test_rbt)))), 19);

    assert_int_equal(getColor(getRight(getRight(getRight(getRoot(test_rbt))))), RED);
    assert_int_equal(*(int*)getContent(getRight(getRight(getRight(getRoot(test_rbt))))), 23);
    RbtDispose(test_rbt);
}

static void TestRLInsert(void **state)
{
     (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[3] = {15,25,20};
    for (int i = 0; i < 3; i++)
        RbtInsert(test_rbt, &arr[i]);
    print2D(test_rbt->root);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 20);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 15);

    assert_int_equal(getColor(getRight(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 25);
    RbtDispose(test_rbt);
}

static void TestDeleteSingleRed(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[4] = {30,20,40,10};
    for (int i = 0; i < 4; i++)
        RbtInsert(test_rbt, &arr[i]);
    int key = 20;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 30);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 10);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 40);
    RbtDispose(test_rbt);
}

static void TestDeleteRoot(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[4] = {30,20,40,10};
    for (int i = 0; i < 4; i++)
        RbtInsert(test_rbt, &arr[i]);
    int key = 30;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 20);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 10);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 40);
    RbtDispose(test_rbt);
}


static void SetAllBlack(rb_node *node) {
    if (node == NULL)
        return;
    node->color = BLACK;
    SetAllBlack(node->right);
    SetAllBlack(node->left);
}

static void TestDeleteAllBlack(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[7] = {50,25,75,20,80,30,70};
    for (int i=0; i < 7; i++)
        RbtInsert(test_rbt, &arr[i]);
    SetAllBlack(test_rbt->root);
    print2D(test_rbt->root);
    int key = 20;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);
    print2D(test_rbt->root);
    RbtDispose(test_rbt);
}

static void TestDeleteRR(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[5] = {30,20,40,35,50};
    for (int i = 0; i < 5; i++)
       RbtInsert(test_rbt, &arr[i]);
    int key = 20;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);
    print2D(test_rbt->root);
    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 40);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 30);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 50);

    assert_int_equal(getColor(getRight(getLeft(getRoot(test_rbt)))), RED);
    assert_int_equal(*(int*)getContent(getRight(getLeft(getRoot(test_rbt)))), 35);
    RbtDispose(test_rbt);
}

static void TestDeleteRL(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[4] = {30,20,40,35};
    for (int i = 0; i < 4; i++)
        RbtInsert(test_rbt, &arr[i]);
    int key = 20;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);
    
    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 35);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 30);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 40);

    RbtDispose(test_rbt);
}

static void TestDeleteLL(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[5] = {30,20,40,15,25};
    for (int i = 0; i < 5; i++) {
        RbtInsert(test_rbt, &arr[i]);
    }
    int key = 40;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);
    // test_rbt->freeFn(getContent(del_node));
    // free(del_node);
    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 20);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 15);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 30);

    assert_int_equal(getColor(getLeft(getRight(getRoot(test_rbt)))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRight(getRoot(test_rbt)))), 25);
    RbtDispose(test_rbt);
}

static void TestDeleteLR(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[4] = {30,20,40,25};
    for (int i = 0; i < 4; i++) {
        RbtInsert(test_rbt, &arr[i]);
    }
    int key = 40;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 25);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 20);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 30);
    RbtDispose(test_rbt);
}

static void TestDeleteRRedSib(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[6] = {20,10,30,25,35,26};
    for (int i=0; i < 6; i++) {
        RbtInsert(test_rbt, &arr[i]);
    }
    int key = 10;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 30);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 25);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 35);

    assert_int_equal(getColor(getRight(getLeft(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getLeft(getRoot(test_rbt)))), 26);

    assert_int_equal(getColor(getLeft(getLeft(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getLeft(getRoot(test_rbt)))), 20);
    RbtDispose(test_rbt);
}

static void TestDeleteLRedSib(void **state) {
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[7] = {25,20,30,35,15,18,16};
    for (int i = 0; i < 7; i++) {
        RbtInsert(test_rbt, &arr[i]);
    }
    int key = 35;
    rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);
    key = 30;
    del_node = RbtSearch(test_rbt->root, isInt, &key);
    RbtDelete(test_rbt, del_node);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 18);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 15);
    
    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 25);

    assert_int_equal(getColor(getRight(getLeft(getRoot(test_rbt)))), RED);
    assert_int_equal(*(int*)getContent(getRight(getLeft(getRoot(test_rbt)))), 16);

    assert_int_equal(getColor(getLeft(getRight(getRoot(test_rbt)))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRight(getRoot(test_rbt)))), 20);
    RbtDispose(test_rbt);
}

static void TestMediumDelete(void **state){
    (void)state;
    rb_tree *test_rbt;

    test_rbt = malloc(sizeof(rb_tree));
    RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
    int arr[7] = {8,5,15,12,19,9,13};
    for (int i = 0; i < 7; i++)
        RbtInsert(test_rbt, &arr[i]);
    print2D(test_rbt->root);
    int key = 5;
    rb_node *del_node = RbtSearch(getRoot(test_rbt), isInt, &key);
    RbtDelete(test_rbt, del_node);
    print2D(test_rbt->root);

    assert_int_equal(getColor(getRoot(test_rbt)), BLACK);
    assert_int_equal(*(int*)getContent(getRoot(test_rbt)), 15);

    assert_int_equal(getColor(getLeft(getRoot(test_rbt))), RED);
    assert_int_equal(*(int*)getContent(getLeft(getRoot(test_rbt))), 12);

    assert_int_equal(getColor(getRight(getRoot(test_rbt))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getRoot(test_rbt))), 19);

    assert_int_equal(getColor(getLeft(getLeft(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getLeft(getLeft(getRoot(test_rbt)))), 8);

    assert_int_equal(getColor(getRight(getLeft(getRoot(test_rbt)))), BLACK);
    assert_int_equal(*(int*)getContent(getRight(getLeft(getRoot(test_rbt)))), 13);
    
    assert_int_equal(getColor(getRight(getLeft(getLeft(getRoot(test_rbt))))), RED);
    assert_int_equal(*(int*)getContent(getRight(getLeft(getLeft(getRoot(test_rbt))))), 9);
    RbtDispose(test_rbt);
}

static void TestDeleteRandom(void **state) {
    (void)state;
    int *arr;
    rb_tree *test_rbt;
    time_t t;
    int test_results[100];
    
    for (int i = 0; i < 100; i++) {
        srand(t);
        // srand(1851816912);
        // printf("%u", t);
        test_rbt = malloc(sizeof(rb_tree));
        RbTreeNew(test_rbt, sizeof(int), cmpInt, free);
        int rnum = rand() % 50;
        arr = malloc(sizeof(int) * rnum);
        for (int i = 0; i < rnum; i++) {
            int tmp = rand() % 200;
            arr[i] = tmp;
            // print_message("%d ", tmp);
            RbtInsert(test_rbt, &tmp);
            // print2D(test_rbt->root);
        }
        // print2D(test_rbt->root);
        // print_message("Test result: %d\n", isRedBlackTree(test_rbt));
        for (int i = 0; i < rand() & rnum; i++) {
            int key = arr[rand() % rnum];
            rb_node *del_node = RbtSearch(test_rbt->root, isInt, &key);
            RbtDelete(test_rbt, del_node);
            // print2D(test_rbt->root);
        }
        // print_message("Test result: %d\n", isRedBlackTree(test_rbt));
        // print2D(test_rbt->root);
        test_results[i] = isRedBlackTree(test_rbt);
        RbtDispose(test_rbt);
    }
    for (int i = 0; i < 100; i++) {
        print_message("%d ", test_results[i]);
    }
}




int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(TestRedUncleInsert),
        cmocka_unit_test(TestLLInsert),
        cmocka_unit_test(TestLRInsert),
        cmocka_unit_test(TestRRInsert),
        cmocka_unit_test(TestRLInsert),
        cmocka_unit_test(TestMediumInsert),
        cmocka_unit_test(TestDeleteSingleRed),
        cmocka_unit_test(TestDeleteRoot),
        cmocka_unit_test(TestDeleteAllBlack),
        cmocka_unit_test(TestDeleteLL),
        cmocka_unit_test(TestDeleteLR),
        cmocka_unit_test(TestDeleteRL),
        cmocka_unit_test(TestDeleteRR),
        cmocka_unit_test(TestDeleteLRedSib),
        cmocka_unit_test(TestDeleteRRedSib),
        cmocka_unit_test(TestMediumDelete),
        cmocka_unit_test(TestDeleteRandom)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}