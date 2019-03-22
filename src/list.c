#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "list.h"

void *create_node(int elem_size) {
    Node *new_node = malloc(sizeof(Node));
    assert(new_node != NULL);
    new_node->content = malloc(elem_size);
    assert(new_node->content != NULL);
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

Node *next_node(const Node *n) {
    if (n) return (n->next);
    return NULL;
}

Node *prev_node(const Node *n) {
    if (n) return (n->prev);
    return NULL;
}

void *get_node_content(const Node *n) {
    if (n) return (n->content);
    return NULL;
}

void list_new(List *l, int elem_size, ListFreeFunction freeFn) {
    assert(elem_size > 0);
    assert(l != NULL);
    l->elem_size = elem_size;
    l->logical = 0;
    l->root = NULL;
    l->end = NULL;
    l->freeFn = freeFn;
}

void list_dispose(List *l) {
    assert(l != NULL);
    Node *curr = l->root;
    Node *next = NULL;
    while (curr) {
        l->freeFn(get_node_content(curr));
        next = next_node(curr);
        free(curr);
        curr = next;
    }
    l->root = l->end = NULL;
    l->logical = 0;
}

// element access

void *list_front(const List *l) {
    assert(l != NULL);
    return get_node_content(l->root);
}

void *list_back(const List *l) {
    assert(l != NULL);
    return get_node_content(l->end);
}

// iterators 

Node *list_begin(const List *l) {
    if (l) return l->root;
    return NULL;
}

Node *list_end(List *l) {
    if (l) return l->end;
    return NULL;    
}

// capacity

int list_len(const List *l) {
    assert(l != NULL);
    return l->logical;
}

int list_empty(const List *l) {
    assert(l != NULL);
    return (l->logical ? 0 : 1);
}

// modifiers

void list_push_front(List *l, const void *elemAddr) {
    assert(l != NULL && elemAddr != NULL);
    Node *new_node = create_node(l->elem_size);
    memcpy(get_node_content(new_node), elemAddr, l->elem_size);
    l->logical++;
    if (l->root == NULL && l->end == NULL) {
        l->root = l->end = new_node;
        return;
    }
    new_node->next = l->root;
    l->root->prev = new_node;
    l->root = new_node;
}

void list_pop_front(List *l) {
    assert(l != NULL);
    if (list_empty(l)) return;
    Node *tmp = l->root;
    if (next_node(l->root) == NULL) {
        l->root = l->end = NULL;
    } else {
        l->root = next_node(l->root);
        l->root->prev = NULL;
    }
    l->freeFn(get_node_content(tmp));
    free(tmp);
    l->logical--;
}

void list_push_back(List *l, const void *elemAddr) {
    assert(l != NULL && elemAddr != NULL);
    Node *new_node = create_node(l->elem_size);
    memcpy(new_node->content, elemAddr, l->elem_size);
    if (l->root == NULL && l->end == NULL) {
        l->root = l->end = new_node;
        return;
    }
    l->end->next = new_node;
    new_node->prev = l->end;
    l->end = new_node;
}

void list_pop_back(List *l) {
    assert(l != NULL);
    if (list_empty(l)) return;
    Node *tmp = l->end;
    if (prev_node(l->end) == NULL) {
        l->root = l->end = NULL;
    } else {
        l->end = prev_node(l->end);
        l->end->next = NULL;
    }
    l->freeFn(tmp->content);
    free(tmp);
    l->logical--;
}

void *list_insert(List *l, Node *position, const void *elemAddr) {
    Node *new_node = create_node(l->elem_size);
    memcpy(new_node->content, elemAddr, l->elem_size);
    if (l->root == NULL && l->end == NULL) {
        l->root = l->end = new_node;
    } else if (l->root && l->root == position) {
        l->root->prev = new_node;
        new_node->next = l->root;
        l->root = new_node;
    } else if (l->end && position == NULL) {
        l->end->next = new_node;
        new_node->prev = l->end;
        l->end = new_node;
    } else {
        new_node->prev = prev_node(position);
        position->prev->next = new_node;
        new_node->next = position;
        position->prev = new_node;
    }
    l->logical++;
    return new_node;
}

void *list_erase(List *l, Node *position) {
    assert(position != NULL);
    assert(l != NULL);
    Node *curr = NULL;
    if (l->root == l->end)
        l->root = l->end = NULL;
    else if (l->root == position) {
        l->root = next_node(position);
        l->root->prev = NULL;
        curr = l->root;
    } else if (position == l->end) {
        l->end = prev_node(position);
        l->end->next = NULL;
        curr = l->end;
    } else {
        position->prev->next = next_node(position);
        position->next->prev = prev_node(position);
        curr = next_node(position);
    }
    l->freeFn(get_node_content(position));
    free(position);
    l->logical--;
    return curr;
}

void list_remove_if(List *l, ListCmpFunction CmpFn, void *aux_data) {
    Node *curr = l->root;
    while (curr) {
        if (CmpFn(get_node_content(curr), aux_data)) {
            curr = list_erase(l, curr);
        } else {
            curr = next_node(curr);
        }
    }
}

void list_map(const List *l, void (*mapFn)(void *, void *),
            void *aux_data) {

    Node *curr = l->root;
    while (curr) {
        mapFn(get_node_content(curr), aux_data);
        curr = next_node(curr);
    }
}
