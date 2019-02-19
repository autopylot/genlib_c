#ifndef _rbt_helpers_
#define _rbt_helpers_

#include "rb_tree.h"

int isRed(rb_node *node);
int isBlack(rb_node *node);
int isLeft(rb_node *node);
int isRight(rb_node *node);
void setColor(rb_node *node,enum COLOR);
rb_node *getRoot(rb_tree *rbt);
rb_node *getUncle(rb_node *node);
rb_node *getSibling(rb_node *node);
rb_node *getLeft(rb_node *node);
rb_node *getRight(rb_node *node);
rb_node *getParent(rb_node *node);
rb_node *getGrandParent(rb_node *node);

#endif //_rbt_helpers_
