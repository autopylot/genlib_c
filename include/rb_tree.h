#ifndef _rb_tree_
#define _rb_tree_

typedef int (*RbtCompareFn)(void *elemAddr1, void *elemAddr2);
typedef void (*RbtFreeFn)(void *elemAddr);
typedef struct rb_node rb_node;

enum COLOR
{
    RED,
    BLACK
};

typedef struct rb_node
{
    void        *content;
    rb_node     *parent;
    rb_node     *left;
    rb_node     *right;
    enum COLOR  color;
} rb_node;

typedef struct rb_tree
{
    rb_node         *root;
    int             elemSize;
    int             logical;
    RbtCompareFn    cmpFn;
    RbtFreeFn       freeFn;
} rb_tree;

void RbTreeNew(rb_tree *rbt, int elemSize, RbtCompareFn cmpFn, RbtFreeFn freeFn);
rb_node *RbNodeNew(const void *elemAddr, rb_node *parent, int elemSize);
void RbtInsert(rb_tree *rbt, void *elemAddr);
void RbtDelete(rb_tree *rbt, rb_node *target);
rb_node *RbtSearch(rb_node *root, RbtCompareFn cmpFn, void *key);
void RbtDispose(rb_tree *rbt);

int isRed(rb_node *node);
int isBlack(rb_node *node);
int isLeft(rb_node *node);
int isRight(rb_node *node);
void setColor(rb_node *node,enum COLOR);
enum COLOR getColor(rb_node *node);
rb_node *getRoot(rb_tree *rbt);
rb_node *getUncle(rb_node *node);
rb_node *getSibling(rb_node *node);
rb_node *getLeft(rb_node *node);
rb_node *getRight(rb_node *node);
rb_node *getParent(rb_node *node);
rb_node *getGrandParent(rb_node *node);
void *getContent(rb_node *node);
#endif //_rb_tree_


