#ifndef _list_
#define _list_

typedef void (*ListFreeFunction)(void *elemAddr);
typedef int (*ListCmpFunction)(void *elemAddr1, void *elemAddr2);

typedef struct Node Node;

typedef struct Node {
    void *content;
    Node *prev;
    Node *next;
} Node;

typedef struct {
    int elem_size;
    int logical;
    Node *root;
    Node *end;
    ListFreeFunction freeFn;
} List;

void *create_node(int elemSize);
Node *next_node(const Node *n);
Node *prev_node(const Node *n);
void *get_node_content(const Node *n);

/**list functions**/
void list_new(List *l, int elemSize, ListFreeFunction freeFn);
void list_dispose(List *l);
int list_len(const List *l);
int list_empty(const List *l);
Node *list_begin(const List *l);
Node *list_end(List *l);
void *list_front(const List *l);
void *list_back(const List *l);
void list_push_front(List *l, const void *elemAddr);
void list_pop_front(List *l);
void list_push_back(List *l, const void *elemAddr);
void list_pop_back(List *l);
void *list_insert(List *l, Node *position, const void *elemAddr);
void *list_erase(List *l, Node *position);
void list_remove_if(List *l, ListCmpFunction CmpFn, void *aux_data);
void list_map(const List *l, void (*mapFn)(void *, void *), void *aux_data);

#endif