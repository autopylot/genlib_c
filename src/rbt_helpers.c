#include <stdlib.h>
#include "rbt_helpers.h"

int isRed(rb_node *node)
{
    if (node == NULL)
        return 0;
    return node->color == RED;
}

int isBlack(rb_node *node)
{
    return (node == NULL || node->color == BLACK);
}

int isLeft(rb_node *node)
{
    if (node == NULL)
        return 0;
    return (node == getLeft(getParent(node)));
}

int isRight(rb_node *node)
{
    if (node == NULL)
        return 0;
    return (node == getRight(getParent(node)));
}

rb_node *getUncle(rb_node *node)
{
    if (node == NULL)
        return NULL;
    return getSibling(getParent(node));
}

rb_node *getSibling(rb_node *node)
{
    if (node->parent) {
        if (isLeft(node))
            return getRight(getParent(node));
        else
            return getLeft(getParent(node));
    }
    return NULL;
}

rb_node * getLeft(rb_node *node) {
    if (node == NULL || node->left == NULL)
        return NULL;
    return node->left;
}

rb_node *getRight(rb_node *node)
{
    if (node == NULL || node->right == NULL)
        return NULL;
    return node->right;
}

rb_node *getParent(rb_node *node) {
    return (node == NULL || node->parent == NULL) ? NULL : node->parent;
}

rb_node *getGrandParent(rb_node *node)
{
    return getParent(getParent(node));
}

enum COLOR getColor(rb_node *node) {
    return (node ? node->color : BLACK);
}

void setColor(rb_node *node,enum COLOR color) {
    node->color = color;
}

rb_node *getRoot(rb_tree *rbt) {
    return rbt->root;
}

void *getContent(rb_node *node) {
    return node->content;
}