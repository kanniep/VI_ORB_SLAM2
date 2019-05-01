#ifndef LIST_H
#define LIST_H

typedef struct node{
    void *val;
    struct node *next;
    struct node *prev;
} node;

typedef struct dlist{
    int size;
    node *front;
    node *back;
} dlist;

#ifdef __cplusplus
extern "C" {
#endif
dlist *make_list();
int list_find(dlist *l, void *val);

void list_insert(dlist *, void *);

void **list_to_array(dlist *l);

void free_list(dlist *l);
void free_list_contents(dlist *l);
void free_list_contents_kvp(dlist *l);

#ifdef __cplusplus
}
#endif
#endif
