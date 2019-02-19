#include <stdlib.h>
#include <string.h>
#include "rb_tree.h"

void RbTreeNew(rb_tree *rbt, int elemSize, RbtCompareFn cmpFn, RbtFreeFn freeFn)
{
    rbt->root = NULL;
    rbt->elemSize = elemSize;
    rbt->cmpFn = cmpFn;
    rbt->freeFn = freeFn;
}

static void RbtFreeNode(rb_node *node, RbtFreeFn freeFn) {
    if (node == NULL)
        return;
    RbtFreeNode(node->left, freeFn);
    RbtFreeNode(node->right, freeFn);
    freeFn(node->content);
    free(node);
}

void RbtDispose(rb_tree *rbt) {
    RbtFreeNode(rbt->root, rbt->freeFn);
    free(rbt);
}

rb_node *RbNodeNew(const void *elemAddr, rb_node *parent, int elemSize)
{
    rb_node *newNode = NULL;

    newNode = (rb_node*)malloc(sizeof(struct rb_node));
    newNode->content = (void*)malloc(sizeof(elemSize));
    memcpy(newNode->content, elemAddr, elemSize);
    newNode->parent = parent;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->color = RED;
    return newNode;
}

static void RbtRecolor(rb_node *node, int ncase)
{
    if (ncase == 1)
    {
        getParent(node)->color = BLACK;
        getSibling(node)->color = RED;
    }
    else if (ncase == 2)
    {
        getParent(node)->color = BLACK;
        (getUncle(node))->color = BLACK;
        getParent(getParent(node))->color = RED;
    }
}

static void RbtLeftRotate(rb_tree *rbt, rb_node *node)
{
    rb_node *subtree;
  
    subtree = getRight(node);
    node->right = getLeft(subtree);
    if (getLeft(subtree)) {
        getLeft(subtree)->parent = node;
    }
    if (getParent(subtree)) {
        subtree->parent = getParent(node);
    }
    if (getParent(node) == NULL) {
        rbt->root = subtree;
    }
    else if (isLeft(node)) {
        node->parent->left = subtree;
    }
    else {
        node->parent->right = subtree;
    }
    if (subtree) {
        subtree->left = node;
    }
    node->parent = subtree;
}

static void RbtRightRotate(rb_tree *rbt, rb_node *node)
{
    rb_node *subtree;

    subtree = node->left;
    node->left = subtree->right;
    if (subtree->right != NULL) {
        subtree->right->parent = node;
    }
    subtree->parent = node->parent;
    if (node->parent == NULL) {
        rbt->root = subtree;
    }
    else {
        if (node == (node->parent)->right)
            node->parent->right = subtree;
        else
            node->parent->left = subtree;
    }
    subtree->right = node;
    node->parent = subtree;
}

static rb_node *TreeInsert(rb_tree *rbt, void *elemAddr)
{
    rb_node *current = rbt->root;
    rb_node **insert = NULL;
    rb_node *parent = NULL;

    if (rbt->root == NULL) {
        rbt->root = RbNodeNew(elemAddr, NULL, rbt->elemSize);
        return rbt->root;
    }
    while (current) {
        parent = current;
        if (rbt->cmpFn(current->content, elemAddr)) {
            insert = &(current->right);
            current = current->right;
        }
        else {
            insert = &(current->left);
            current = current->left;
        }
    }
    *insert = RbNodeNew(elemAddr, parent, rbt->elemSize);
    rbt->logical += 1;
    return *insert;
}

void RbtInsert(rb_tree *rbt, void *elemAddr)
{
    rb_node *current= NULL;
    rb_node *parent = NULL;
    rb_node *gparent = NULL;
    rb_node *uncle = NULL;
  
    current = TreeInsert(rbt, elemAddr);
    while (rbt->root != current && isRed(getParent(current))) {
        parent = getParent(current);
        gparent = getGrandParent(current);
        uncle = getUncle(current);
        if (isRed(uncle)) {
            setColor(parent, BLACK);
            setColor(uncle, BLACK);
            setColor(gparent, RED);
            current = gparent;
        }
        else {
            if (isLeft(current) && isRight(parent)) {
                RbtRightRotate(rbt, parent);
                current = parent;
            }
            else if (isRight(current) && isLeft(parent)) {
                RbtLeftRotate(rbt, parent);
                current = parent;
            }
            else if (isLeft(current) && isLeft(parent)) {
                RbtRightRotate(rbt, gparent);
                enum COLOR clr = getColor(parent);
                setColor(parent, getColor(getSibling(current)));
                setColor(getSibling(current), clr);
            }
            else {
                RbtLeftRotate(rbt, gparent);
                enum COLOR clr = getColor(parent);
                setColor(parent, getColor(getSibling(current)));
                setColor(getSibling(current), clr);
            }
        }   
    }
    setColor(getRoot(rbt), BLACK);
}

rb_node *RbtSearch(rb_node *root, RbtCompareFn cmpFn, void *key) {
    if (root == NULL) {
        return root;
    }
    if (cmpFn(root->content, key) < 0) {
        return RbtSearch(root->left, cmpFn, key);
    }
    else if (cmpFn(root->content, key) > 0) {
        return RbtSearch(root->right, cmpFn, key);
    }
    else {
        return root;
    }
}

rb_node *minValueNode(rb_node *node) { 
    rb_node *curr = node; 
  
    while (curr->left) 
      curr = curr->left;
    return curr; 
} 
  
rb_node *findSuccessor(rb_node *x) { 
    if (x->left != NULL && x->right != NULL) 
      return minValueNode(x->right); 
    if (x->left == NULL && x->right == NULL) 
      return NULL; 
    if (x->left != NULL) 
      return x->left; 
    else
      return x->right; 
} 

void fixDoubleBlack(rb_tree *rbt, rb_node *target) {
    while (target != rbt->root && isBlack(target)) {
        rb_node *sibling = getSibling(target);
        rb_node *r_child = getRight(sibling);
        rb_node *l_child = getLeft(sibling);

        if (isBlack(sibling) && (isRed(l_child) || isRed(r_child))) { 
            if (isLeft(sibling) && isRed(l_child)) {
                RbtRightRotate(rbt, getParent(target));
                setColor(l_child, BLACK);
            }
            else if (isLeft(sibling) && isRed(r_child)) {
                RbtLeftRotate(rbt, sibling);
                RbtRightRotate(rbt, getParent(target));
                setColor(r_child, BLACK);
            }
            else if (isRight(sibling) && isRed(r_child)) {
                RbtLeftRotate(rbt, getParent(target));
                setColor(r_child, BLACK);
                setColor(getGrandParent(target), RED);
                setColor(getParent(target), BLACK);
            }
            else {
                RbtRightRotate(rbt, sibling);
                RbtLeftRotate(rbt, getParent(target));
                setColor(l_child, BLACK);
                setColor(getGrandParent(target), RED);
                setColor(getParent(target), BLACK);
            }
            break;
        }
        else if (isBlack(sibling) && isBlack(r_child) && isBlack(l_child)) {
            setColor(sibling, RED);
            if (isBlack(getParent(target)))
                target = getParent(target);
            else
                setColor(getParent(target), BLACK);
            break;
        }
        else if (isRed(sibling)) {
            if (isRight(sibling))
                RbtLeftRotate(rbt, getParent(target));
            else
                RbtRightRotate(rbt, getParent(target));
            setColor(getGrandParent(target), BLACK);
            setColor(getParent(target), RED);
        }
    }
    setColor(getRoot(rbt), BLACK);
}

void RbtDelete(rb_tree *rbt, rb_node *target) {
    if (target == NULL)
        return ;
    rb_node *successor = findSuccessor(target); 
    rbt->logical -= 1;
    int doubleBlack = ((successor == NULL || successor->color == BLACK) && (target->color == BLACK)); 
    rb_node *parent = getParent(target); 
    if (successor == NULL) { 
      if (target == rbt->root) 
        rbt->root = NULL;
      else { 
        if (doubleBlack)
          fixDoubleBlack(rbt, target);
        else {
            if (getSibling(target)) 
                setColor(getSibling(target), RED); 
        }
        if (isLeft(target))
          parent->left = NULL; 
        else
          parent->right = NULL;
      }
    //   rbt->freeFn(target->content);
      free(target);
      return; 
    }

    if (target->left == NULL || target->right == NULL) { 
        if (target == rbt->root) {
            // rbt->freeFn(target->content);
            target->content = successor->content; 
            target->left = target->right = NULL; 
            free(successor);
        } 
        else { 
            if (isLeft(target)) { 
                parent->left = successor; 
            }
            else { 
                parent->right = successor; 
            } 
            // rbt->freeFn(target->content);
            free(target); 
            successor->parent = parent; 
            if (doubleBlack) { 
                fixDoubleBlack(rbt, successor);   
            }
            else
                successor->color = BLACK;
        }
        return ;
    }
    void *swap1 = successor->content;
    successor->content = target->content;
    target->content = swap1;
    RbtDelete(rbt, successor); 
}
