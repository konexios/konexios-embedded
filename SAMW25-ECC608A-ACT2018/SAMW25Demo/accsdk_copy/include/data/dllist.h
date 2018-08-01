#if !defined(ACN_SDK_C_DOUBLY_LINKED_LIST_H_)
#define ACN_SDK_C_DOUBLY_LINKED_LIST_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <data/linkedlist.h>

typedef struct _doubly_linked_list_ {
  struct _doubly_linked_list_ *next;
  struct _doubly_linked_list_ *prev;
} doubly_linked_list_t;

#define dl_for_each_node(p, head, type) \
    doubly_linked_list_t *UN(base_p) = NULL; \
    type *UN(first) = (head)?container_of((head)->node.next, type, node):NULL; \
    for ( (p) = UN(first) ; \
          (p) && \
          UN(base_p) != (head)->node.next ; \
          UN(base_p) = UN(base_p) ? UN(base_p)->next : UN(first)->node.next, \
          (p) = container_of(UN(base_p), type, node) )
      
#define dl_for_each_node_safe(p, head, type) \
  doubly_linked_list_t *UN(base_p) = NULL; \
  doubly_linked_list_t UN(base_p_obj) = { NULL, NULL }; \
  type *UN(first) = (head)?container_of((head)->node.next, type, node):NULL; \
  for ( (p) = UN(first), \
        UN(base_p_obj) = UN(base_p)?*UN(base_p):UN(base_p_obj) ; \
        (p) && \
        UN(base_p) != (head)->node.next ; \
        UN(base_p) = UN(base_p) ? UN(base_p_obj).next : UN(first)->node.next, \
        (p) = container_of(UN(base_p), type, node), \
        UN(base_p_obj) = (UN(base_p)?(*UN(base_p)):UN(base_p_obj)) )

#define doubly_linked_list_find_node(p, root, type, func, data) { \
      type *UN(tmp) = NULL; \
      (p) = NULL; \
      dl_for_each_node(UN(tmp), root, type) { \
          (p) = UN(tmp); \
          if ( (func)((UN(tmp)), (data)) == 0 ) break; \
      } \
}

doubly_linked_list_t *doubly_linked_list_add(
        doubly_linked_list_t *n,
        doubly_linked_list_t *head);
doubly_linked_list_t *doubly_linked_list_add_tail(
        doubly_linked_list_t *n,
        doubly_linked_list_t *head);
doubly_linked_list_t *doubly_linked_list_del(
        doubly_linked_list_t *el);

#define doubly_linked_list_head_node doubly_linked_list_t node

#define doubly_linked_list_add_node(head, type, el) { \
  if ( !(head) ) { \
    (head) = (el); \
    (head)->node.prev = &(el)->node; \
    (head)->node.next = &(el)->node; \
  } else { \
    doubly_linked_list_t *base_p = doubly_linked_list_add(&(el)->node, &(head)->node); \
    (head) = container_of(base_p, type, node); \
  } \
}

#define doubly_linked_list_add_node_tail(head, type, el) { \
  if ( !(head) ) { \
    (head) = (el); \
    (head)->node.prev = &(el)->node; \
    (head)->node.next = &(el)->node; \
  } else { \
    doubly_linked_list_t *base_p = doubly_linked_list_add_tail(&(el)->node, &(head)->node); \
    (head) = container_of(base_p, type, node); \
  } \
}

#define doubly_linked_list_del_node(head, type, el) { \
  if ( el ) { \
    if ( (el) == (head) ) (head) = container_of((head)->node.prev, type, node); \
    if ( !doubly_linked_list_del(&(el)->node) ) el = NULL; \
  } \
}

#define doubly_linked_list_del_node_head(head, type) { \
  if ( head ) { \
    type *head_prev = container_of((head)->node.prev, type, node); \
    doubly_linked_list_del(&(head)->node); \
    (head) = head_prev; \
  } \
}

#if defined(__cplusplus)
}
#endif

#endif  // ACN_SDK_C_DOUBLY_LINKED_LIST_H_

