/*
 * tests/test_runq.c
 *
 * Deterministic tests for include/kernul/runq.h. Phase 4, Task 2.
 *
 * These tests verify the shape and initialization contract of runq_t.
 * No scheduler behavior is tested here. No enqueue or dequeue operations
 * exist yet; those belong to Task 4.
 */

#include <kernul/runq.h>
#include <kernul/thread.h>
#include <kernul/types.h>

/* -------------------------------------------------------------------------
 * Minimal test harness
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
 * Tests
 * ---------------------------------------------------------------------- */

static void test_init_count_is_zero(void)
{
    /* count is set to zero by runq_init(). This tests initialization only.
     * Enqueue and dequeue semantics, which increment and decrement count,
     * are Task 4. */
    runq_t rq;
    runq_init(&rq);
    CHECK("count is zero after runq_init", rq.count == 0);
}

static void test_init_list_is_empty(void)
{
    runq_t rq;
    runq_init(&rq);
    CHECK("thread list is empty after runq_init", list_empty(&rq.threads));
}

static void test_init_list_head_self_links(void)
{
    /*
     * The circular sentinel design requires that after init, the list
     * head's next and prev both point to the head itself. This is the
     * invariant the list operations depend on.
     */
    runq_t rq;
    runq_init(&rq);
    CHECK("threads.next points to itself after init",
          rq.threads.next == &rq.threads);
    CHECK("threads.prev points to itself after init",
          rq.threads.prev == &rq.threads);
}

static void test_multiple_queues_are_independent(void)
{
    /*
     * Two run queues initialized independently must not share list
     * state. Each must be an isolated empty list.
     */
    runq_t a;
    runq_t b;
    runq_init(&a);
    runq_init(&b);

    CHECK("queue a is empty", list_empty(&a.threads));
    CHECK("queue b is empty", list_empty(&b.threads));
    CHECK("queue a head does not point into queue b",
          a.threads.next != &b.threads);
    CHECK("queue b head does not point into queue a",
          b.threads.next != &a.threads);
}

static void test_runq_node_field_offset_roundtrip(void)
{
    /*
     * Given the current embedding of list_node_t as runq_node inside
     * thread_t, and the list_entry macro contract, a live node inserted
     * into a run queue's thread list can be converted back to its
     * containing thread_t correctly.
     *
     * This does not prove general field-offset correctness in the abstract.
     * It proves that, for this struct layout, the round-trip holds.
     * It is a structural test, not a scheduler behavior test. No scheduler
     * interface is used; list primitives are used directly.
     */
    runq_t   rq;
    thread_t t = {0};

    runq_init(&rq);

    /* Directly insert the node — no scheduler involvement. */
    list_add_tail(&rq.threads, &t.runq_node);

    /* Recover the thread pointer from the list node. */
    list_node_t *node      = rq.threads.next;
    thread_t    *recovered = list_entry(node, thread_t, runq_node);

    CHECK("list_entry recovers thread_t from runq_node in queue",
          recovered == &t);

    /* Remove to leave the list clean. */
    list_remove(&t.runq_node);
    CHECK("list is empty after removing the thread node",
          list_empty(&rq.threads));
}

/* -------------------------------------------------------------------------
 * Test runner
 * ---------------------------------------------------------------------- */

int main(void)
{
    test_init_count_is_zero();
    test_init_list_is_empty();
    test_init_list_head_self_links();
    test_multiple_queues_are_independent();
    test_runq_node_field_offset_roundtrip();

    if (tests_failed == 0)
        return 0;
    else
        return 1;
}
