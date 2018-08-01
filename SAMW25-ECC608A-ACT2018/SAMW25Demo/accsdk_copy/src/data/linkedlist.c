#include "data/linkedlist.h"

arrow_linked_list_t *arrow_linked_list_add(arrow_linked_list_t *root, arrow_linked_list_t *el) {
    arrow_linked_list_t *last = root;
    el->next = 0;
    while ( last && last->next ) {
        last = last->next;
    }
    if ( last ) last->next = el;
    else root = el;
    return root;
}

arrow_linked_list_t *arrow_linked_list_add_first(arrow_linked_list_t *root, arrow_linked_list_t *el) {
    el->next = root;
    return el;
}

arrow_linked_list_t *arrow_linked_list_del(arrow_linked_list_t *root, arrow_linked_list_t *el) {
    arrow_linked_list_t *last = root;
    arrow_linked_list_t *prev = 0;
    while ( last ) {
        if ( last == el) {
            if ( prev ) {
                prev->next = last->next;
                last = 0;
                return root;
            } else {
                last = root->next;
                root->next = 0;
                return last;
            }
        }
        prev = last;
        last = last->next;
    }
    return 0;
}

arrow_linked_list_t *arrow_linked_list_del_last(arrow_linked_list_t *root) {
    arrow_linked_list_t *last = root;
    arrow_linked_list_t *prev = 0;
    while ( last && last->next ) {
        prev = last;
        last = last->next;
    }
    if ( prev ) {
        prev->next = last->next;
        last->next = 0;
        return root;
    } 
    return 0;
}
