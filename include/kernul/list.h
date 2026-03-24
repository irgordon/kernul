#ifndef KERNUL_LIST_H
#define KERNUL_LIST_H

/*
 * include/kernul/list.h
 *
 * Intrusive doubly-linked circular list. Phase 3.
 *
 * Design:
 *   list_node_t is embedded directly in the owning structure.
 *   The caller recovers the enclosing structure with list_entry().
 *   No allocation occurs per list operation.
 *
 * Circular sentinel:
 *   An empty list has head.next == &head and head.prev == &head.
 *   This eliminates NULL checks from insert and remove operations.
 *
 * Ownership:
 *   The list does not own nodes. The caller is responsible for the
 *   lifetime of every object whose node appears in a list.
 *
 * Concurrency:
 *   No locking is provided here. The caller must hold the appropriate
 *   lock before modifying any list shared between contexts.
 */

#include <kernul/types.h>
#include <kernul/compiler.h>

/* -------------------------------------------------------------------------
 * Types
 *
 * list_t and list_node_t are the same struct. list_t names the sentinel
 * head; list_node_t names an embedded member. Keeping them as one type
 * avoids casts while the typedef communicates intent at the declaration.
 * ---------------------------------------------------------------------- */

typedef struct list_node {
    struct list_node *next;
    struct list_node *prev;
} list_node_t;

typedef list_node_t list_t;

/* -------------------------------------------------------------------------
 * list_entry(node, type, member)
 *
 * Recover a pointer to the enclosing structure from a list_node_t*.
 *   node:   pointer to the embedded list_node_t
 *   type:   type of the enclosing struct
 *   member: name of the list_node_t field within that struct
 * ---------------------------------------------------------------------- */
#define list_entry(node, type, member) \
    KERN_CONTAINER_OF(node, type, member)

/* -------------------------------------------------------------------------
 * Initialization
 * ---------------------------------------------------------------------- */

/*
 * list_init(head)
 *
 * Initialize a list head. Must be called before any other operation.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 */
static inline void list_init(list_t *head)
{
    head->next = head;
    head->prev = head;
}

/*
 * LIST_INIT(name)
 *
 * Static initializer for a list_t at file scope or inside a struct.
 */
#define LIST_INIT(name)  { .next = &(name), .prev = &(name) }

/* -------------------------------------------------------------------------
 * Queries
 * ---------------------------------------------------------------------- */

/*
 * list_empty(head)
 *
 * Returns true if the list contains no nodes.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 */
static inline bool list_empty(const list_t *head)
{
    return head->next == head;
}

/* -------------------------------------------------------------------------
 * Insertion
 * ---------------------------------------------------------------------- */

/*
 * list_add_head(head, node)
 *
 * Insert node immediately after head (front of list).
 * Suitable for LIFO access patterns.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 */
static inline void list_add_head(list_t *head, list_node_t *node)
{
    node->next       = head->next;
    node->prev       = head;
    head->next->prev = node;
    head->next       = node;
}

/*
 * list_add_tail(head, node)
 *
 * Insert node immediately before head (back of list).
 * Suitable for FIFO access patterns.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 */
static inline void list_add_tail(list_t *head, list_node_t *node)
{
    node->next       = head;
    node->prev       = head->prev;
    head->prev->next = node;
    head->prev       = node;
}

/* -------------------------------------------------------------------------
 * Removal
 * ---------------------------------------------------------------------- */

/*
 * list_remove(node)
 *
 * Remove node from whichever list it belongs to.
 * After removal, node->next and node->prev point to node itself.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 */
static inline void list_remove(list_node_t *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next       = node;
    node->prev       = node;
}

/* -------------------------------------------------------------------------
 * Iteration
 * ---------------------------------------------------------------------- */

/*
 * list_for_each(head, cursor)
 *
 * Iterate over all nodes. cursor is a list_node_t* visiting each node.
 * Do not remove cursor during iteration; use list_for_each_safe for that.
 */
#define list_for_each(head, cursor) \
    for ((cursor) = (head)->next; \
         (cursor) != (head); \
         (cursor) = (cursor)->next)

/*
 * list_for_each_safe(head, cursor, tmp)
 *
 * Iteration safe for removing cursor from the list inside the loop.
 * tmp is a list_node_t* used to save the next pointer before removal.
 */
#define list_for_each_safe(head, cursor, tmp) \
    for ((cursor) = (head)->next, (tmp) = (cursor)->next; \
         (cursor) != (head); \
         (cursor) = (tmp), (tmp) = (cursor)->next)

#endif /* KERNUL_LIST_H */
