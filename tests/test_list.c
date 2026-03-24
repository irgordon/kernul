/*
 * tests/test_list.c
 *
 * Deterministic tests for include/kernul/list.h. Phase 3.
 *
 * Tests run in a hosted environment. No hardware required.
 */

#include <kernul/list.h>
#include <kernul/types.h>

/* -------------------------------------------------------------------------
 * Minimal test harness (duplicated from test_boot.c; no shared header yet)
 * ---------------------------------------------------------------------- */

static int tests_run    = 0;
static int tests_failed = 0;

#define CHECK(label, expr) \
    do { \
        tests_run++; \
        if (!(expr)) { \
            tests_failed++; \
        } \
    } while (0)

/* -------------------------------------------------------------------------
 * Test fixture
 *
 * A minimal struct with an embedded list_node_t.
 * ---------------------------------------------------------------------- */

struct item {
    int         value;
    list_node_t node;
};

/* -------------------------------------------------------------------------
 * Tests
 * ---------------------------------------------------------------------- */

static void test_init_is_empty(void)
{
    list_t head;
    list_init(&head);
    CHECK("freshly initialized list is empty", list_empty(&head));
}

static void test_static_init_is_empty(void)
{
    list_t head = LIST_INIT(head);
    CHECK("statically initialized list is empty", list_empty(&head));
}

static void test_add_tail_not_empty(void)
{
    list_t      head;
    struct item a = { .value = 1 };

    list_init(&head);
    list_add_tail(&head, &a.node);

    CHECK("list is not empty after add_tail", !list_empty(&head));
}

static void test_add_head_not_empty(void)
{
    list_t      head;
    struct item a = { .value = 1 };

    list_init(&head);
    list_add_head(&head, &a.node);

    CHECK("list is not empty after add_head", !list_empty(&head));
}

static void test_remove_single_leaves_empty(void)
{
    list_t      head;
    struct item a = { .value = 1 };

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_remove(&a.node);

    CHECK("list is empty after removing only node", list_empty(&head));
}

static void test_add_tail_fifo_order(void)
{
    /*
     * Three items added to the tail. Iteration must visit them in
     * insertion order: a, b, c.
     */
    list_t      head;
    struct item a = { .value = 1 };
    struct item b = { .value = 2 };
    struct item c = { .value = 3 };
    list_node_t *cursor;
    int          seen[3];
    int          i = 0;

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);

    list_for_each(&head, cursor) {
        struct item *it = list_entry(cursor, struct item, node);
        if (i < 3)
            seen[i++] = it->value;
    }

    CHECK("add_tail: first visited value is 1",  i == 3 && seen[0] == 1);
    CHECK("add_tail: second visited value is 2", i == 3 && seen[1] == 2);
    CHECK("add_tail: third visited value is 3",  i == 3 && seen[2] == 3);
}

static void test_add_head_lifo_order(void)
{
    /*
     * Three items added to the head. Iteration must visit them in
     * reverse insertion order: c, b, a.
     */
    list_t      head;
    struct item a = { .value = 1 };
    struct item b = { .value = 2 };
    struct item c = { .value = 3 };
    list_node_t *cursor;
    int          seen[3];
    int          i = 0;

    list_init(&head);
    list_add_head(&head, &a.node);
    list_add_head(&head, &b.node);
    list_add_head(&head, &c.node);

    list_for_each(&head, cursor) {
        struct item *it = list_entry(cursor, struct item, node);
        if (i < 3)
            seen[i++] = it->value;
    }

    CHECK("add_head: first visited value is 3",  i == 3 && seen[0] == 3);
    CHECK("add_head: second visited value is 2", i == 3 && seen[1] == 2);
    CHECK("add_head: third visited value is 1",  i == 3 && seen[2] == 1);
}

static void test_remove_middle(void)
{
    /*
     * Remove the middle node. Iteration must visit only a and c.
     */
    list_t      head;
    struct item a = { .value = 1 };
    struct item b = { .value = 2 };
    struct item c = { .value = 3 };
    list_node_t *cursor;
    int          seen[2];
    int          i = 0;

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);

    list_remove(&b.node);

    list_for_each(&head, cursor) {
        struct item *it = list_entry(cursor, struct item, node);
        if (i < 2)
            seen[i++] = it->value;
    }

    CHECK("after middle remove, two nodes remain", i == 2);
    CHECK("after middle remove, first node is a",  i == 2 && seen[0] == 1);
    CHECK("after middle remove, second node is c", i == 2 && seen[1] == 3);
}

static void test_for_each_safe_remove_during_iteration(void)
{
    /*
     * Remove every node during list_for_each_safe.
     * List must be empty afterward.
     */
    list_t      head;
    struct item a = { .value = 1 };
    struct item b = { .value = 2 };
    struct item c = { .value = 3 };
    list_node_t *cursor;
    list_node_t *tmp;

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_add_tail(&head, &b.node);
    list_add_tail(&head, &c.node);

    list_for_each_safe(&head, cursor, tmp)
        list_remove(cursor);

    CHECK("list empty after removing all nodes during safe iteration",
          list_empty(&head));
}

static void test_removed_node_self_links(void)
{
    /*
     * After list_remove, the node's next and prev must point to itself.
     * This ensures a removed node is safe to remove again (no dangling
     * pointer into a list it no longer belongs to).
     */
    list_t      head;
    struct item a = { .value = 1 };

    list_init(&head);
    list_add_tail(&head, &a.node);
    list_remove(&a.node);

    CHECK("removed node next points to itself", a.node.next == &a.node);
    CHECK("removed node prev points to itself", a.node.prev == &a.node);
}

/* -------------------------------------------------------------------------
 * Test runner
 * ---------------------------------------------------------------------- */

int main(void)
{
    test_init_is_empty();
    test_static_init_is_empty();
    test_add_tail_not_empty();
    test_add_head_not_empty();
    test_remove_single_leaves_empty();
    test_add_tail_fifo_order();
    test_add_head_lifo_order();
    test_remove_middle();
    test_for_each_safe_remove_during_iteration();
    test_removed_node_self_links();

    if (tests_failed == 0)
        return 0;
    else
        return 1;
}
