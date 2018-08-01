#include "data/dllist.h"

#define find_last 0

static inline void __list_add(
        doubly_linked_list_t *n,
        doubly_linked_list_t *prev,
        doubly_linked_list_t *next) {
    next->prev = n;
    n->next = next;
    n->prev = prev;
    prev->next = n;
}

// add before
doubly_linked_list_t *doubly_linked_list_add(
        doubly_linked_list_t *n,
        doubly_linked_list_t *head) {
    __list_add(n, head, head->next);
    return n;
}

// add after
doubly_linked_list_t *doubly_linked_list_add_tail(
        doubly_linked_list_t *n,
        doubly_linked_list_t *head) {
    __list_add(n, head->prev, head);
    return head;
}

doubly_linked_list_t *doubly_linked_list_del(
        doubly_linked_list_t *el) {
    doubly_linked_list_t *prev = el->prev;
    doubly_linked_list_t *next = el->next;
    prev->next = next;
    next->prev = prev;
    el->next = NULL;
    el->prev = NULL;
    return el;
}
