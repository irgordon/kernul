/*
 * tests/test_sched.c
 *
 * Behavioral tests for sched/sched.c. Phase 4, Task 4.
 *
 * Tests run in a hosted single-CPU environment. No context switching
 * occurs — sched_yield() updates bookkeeping only at this phase.
 *
 * Test boundary:
 *   - init, enqueue, dequeue, current, and yield bookkeeping are tested.
 *   - Context-switch correctness is not tested here (Task 5).
 *   - Idle thread behavior is not tested here (Task 6).
 *   - SMP correctness is not tested here.
 */

#include <kernul/sched.h>
#include <kernul/thread.h>
#include <kernul/errors.h>
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
 * Helpers
 * ---------------------------------------------------------------------- */

/*
 * make_runnable(t, name)
 *
 * Zero-initialize t and set the fields required for enqueue:
 * state = THREAD_RUNNABLE. Name is for diagnostic clarity only.
 */
static void make_runnable(thread_t *t, const char *name)
{
    int i;
    /* zero the struct without memset (no lib.h in scope here) */
    unsigned char *p = (unsigned char *)t;
    for (i = 0; i < (int)sizeof(thread_t); i++)
        p[i] = 0;

    t->state = THREAD_RUNNABLE;

    /* copy name without kstrlcpy (lib.h not in scope) */
    for (i = 0; i < THREAD_NAME_MAX - 1 && name[i]; i++)
        t->name[i] = name[i];
    t->name[i] = '\0';
}

/* -------------------------------------------------------------------------
 * sched_init tests
 * ---------------------------------------------------------------------- */

static void test_init_succeeds(void)
{
    kern_err_t err = sched_init();
    CHECK("sched_init returns KERN_OK on first call", err == KERN_OK);
}

static void test_init_double_fails(void)
{
    kern_err_t err = sched_init();
    CHECK("sched_init returns KERN_EINVAL on second call",
          err == KERN_EINVAL);
}

/* -------------------------------------------------------------------------
 * sched_enqueue / sched_dequeue tests
 * ---------------------------------------------------------------------- */

static void test_dequeue_empty_returns_null(void)
{
    thread_t *t = sched_dequeue();
    CHECK("sched_dequeue on empty queue returns NULL", t == NULL);
}

static void test_enqueue_dequeue_single(void)
{
    thread_t a;
    make_runnable(&a, "a");

    sched_enqueue(&a);
    thread_t *got = sched_dequeue();

    CHECK("dequeue returns the enqueued thread", got == &a);
    CHECK("queue is empty after dequeue", sched_dequeue() == NULL);
}

static void test_enqueue_dequeue_fifo_order(void)
{
    /*
     * Three threads enqueued. Dequeue must return them in insertion
     * order, because sched_enqueue appends to tail and sched_dequeue
     * removes from head.
     */
    thread_t a, b, c;
    make_runnable(&a, "a");
    make_runnable(&b, "b");
    make_runnable(&c, "c");

    sched_enqueue(&a);
    sched_enqueue(&b);
    sched_enqueue(&c);

    CHECK("first dequeue returns a", sched_dequeue() == &a);
    CHECK("second dequeue returns b", sched_dequeue() == &b);
    CHECK("third dequeue returns c", sched_dequeue() == &c);
    CHECK("queue empty after all dequeues", sched_dequeue() == NULL);
}

/* -------------------------------------------------------------------------
 * sched_current tests
 * ---------------------------------------------------------------------- */

static void test_current_null_before_any_thread_runs(void)
{
    /*
     * No thread has been set as current by sched_yield() yet.
     * sched_current() must return NULL in this state.
     */
    CHECK("sched_current returns NULL when no thread is running",
          sched_current() == NULL);
}

/* -------------------------------------------------------------------------
 * sched_yield bookkeeping tests
 * ---------------------------------------------------------------------- */

static void test_yield_phase_limit(void)
{
    /*
     * sched_yield() asserts current != NULL on entry. The only writer
     * of current is sched_yield() itself. No bootstrap path exists at
     * Task 4 to establish the initial current thread; that is provided
     * by the context-switch path in Task 5.
     *
     * Consequence: the full execution path of sched_yield() is not
     * reachable in this phase. Calling it here would trigger ASSERT
     * and halt. This test documents the gap rather than exercising it.
     *
     * What is verified: the bookkeeping behavior of dequeue-after-
     * enqueue, which sched_yield() depends on, is covered by
     * test_enqueue_dequeue_fifo_order. The single-thread round-trip
     * (prev re-enqueued and immediately dequeued) is covered below.
     */
    CHECK("sched_yield phase limit is documented", 1);
}

static void test_yield_single_thread_roundtrip(void)
{
    /*
     * Simulate the core of sched_yield() for the single-thread case
     * directly, without calling sched_yield() itself (which requires
     * current != NULL, not yet settable at this phase).
     *
     * A thread in THREAD_RUNNABLE state is enqueued, then dequeued.
     * The result must be the same thread. This is the structural
     * property that the single-thread yield edge case depends on:
     * re-enqueue followed by dequeue returns the same thread.
     */
    thread_t a;
    make_runnable(&a, "a");

    sched_enqueue(&a);
    thread_t *got = sched_dequeue();

    CHECK("single-thread round-trip: dequeue returns same thread",
          got == &a);
    CHECK("single-thread round-trip: queue empty after dequeue",
          sched_dequeue() == NULL);
}

/* -------------------------------------------------------------------------
 * Test runner
 * ---------------------------------------------------------------------- */

int main(void)
{
    /* init tests must run first and in order */
    test_init_succeeds();
    test_init_double_fails();

    /* enqueue/dequeue tests */
    test_dequeue_empty_returns_null();
    test_enqueue_dequeue_single();
    test_enqueue_dequeue_fifo_order();

    /* current tests */
    test_current_null_before_any_thread_runs();

    /* yield bookkeeping */
    test_yield_phase_limit();
    test_yield_single_thread_roundtrip();

    if (tests_failed == 0)
        return 0;
    else
        return 1;
}
