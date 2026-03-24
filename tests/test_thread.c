/*
 * tests/test_thread.c
 *
 * Deterministic tests for include/kernul/thread.h. Phase 4, Task 1.
 *
 * These tests verify the shape and static properties of the thread
 * structure and state model. No scheduler behavior is tested here.
 */

#include <kernul/thread.h>
#include <kernul/list.h>
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
 * State model tests
 * ---------------------------------------------------------------------- */

static void test_state_values_are_distinct(void)
{
    /*
     * Each thread_state_t value must be distinct so that a switch
     * statement handling every case does not silently collapse two states.
     * This verifies enumeration shape only. Transition validity is not
     * asserted here; that is a scheduler behavior concern, not a data
     * shape concern.
     */
    CHECK("THREAD_CREATED  is distinct from THREAD_RUNNABLE",
          THREAD_CREATED  != THREAD_RUNNABLE);
    CHECK("THREAD_RUNNABLE is distinct from THREAD_RUNNING",
          THREAD_RUNNABLE != THREAD_RUNNING);
    CHECK("THREAD_RUNNING  is distinct from THREAD_SLEEPING",
          THREAD_RUNNING  != THREAD_SLEEPING);
    CHECK("THREAD_SLEEPING is distinct from THREAD_ZOMBIE",
          THREAD_SLEEPING != THREAD_ZOMBIE);
    CHECK("THREAD_ZOMBIE   is distinct from THREAD_DEAD",
          THREAD_ZOMBIE   != THREAD_DEAD);
}

static void test_priority_ordering(void)
{
    /*
     * Lower numeric value must mean higher priority.
     * The run queue (Task 2) depends on this ordering.
     */
    CHECK("THREAD_PRIO_KERNEL < THREAD_PRIO_HIGH",
          THREAD_PRIO_KERNEL < THREAD_PRIO_HIGH);
    CHECK("THREAD_PRIO_HIGH   < THREAD_PRIO_NORMAL",
          THREAD_PRIO_HIGH   < THREAD_PRIO_NORMAL);
    CHECK("THREAD_PRIO_NORMAL < THREAD_PRIO_LOW",
          THREAD_PRIO_NORMAL < THREAD_PRIO_LOW);
    CHECK("THREAD_PRIO_LOW    < THREAD_PRIO_IDLE",
          THREAD_PRIO_LOW    < THREAD_PRIO_IDLE);
}

/* -------------------------------------------------------------------------
 * Structure layout tests
 * ---------------------------------------------------------------------- */

static void test_thread_id_invalid_is_zero(void)
{
    /*
     * THREAD_ID_INVALID must be zero so that a zero-initialized
     * thread_t has an invalid ID by default.
     */
    CHECK("THREAD_ID_INVALID is zero", THREAD_ID_INVALID == 0);
}

static void test_zero_initialized_thread_is_created_state(void)
{
    /*
     * The header contracts THREAD_CREATED == 0. This test encodes that
     * contract: a zero-initialized thread_t must have state THREAD_CREATED.
     * The assertion is valid only because the header explicitly assigns
     * THREAD_CREATED the value 0; it does not assert anything about
     * transition validity or scheduler behavior.
     */
    thread_t t = {0};
    CHECK("zero-initialized thread has THREAD_CREATED state",
          t.state == THREAD_CREATED);
}

static void test_zero_initialized_thread_has_invalid_id(void)
{
    thread_t t = {0};
    CHECK("zero-initialized thread has THREAD_ID_INVALID id",
          t.id == THREAD_ID_INVALID);
}

static void test_runq_node_is_embedded(void)
{
    /*
     * The run queue node must be recoverable from a thread pointer via
     * list_entry(). This tests that the KERN_CONTAINER_OF arithmetic
     * round-trips correctly for this struct layout.
     */
    thread_t    t = {0};
    list_node_t *node = &t.runq_node;
    thread_t    *recovered = list_entry(node, thread_t, runq_node);

    CHECK("list_entry recovers thread pointer from embedded runq_node",
          recovered == &t);
}

static void test_name_buffer_capacity(void)
{
    /*
     * The name buffer must be at least THREAD_NAME_MAX bytes.
     * A name of exactly THREAD_NAME_MAX - 1 chars plus NUL must fit.
     */
    CHECK("name field is THREAD_NAME_MAX bytes",
          sizeof(((thread_t *)0)->name) == THREAD_NAME_MAX);
}

/* -------------------------------------------------------------------------
 * Test runner
 * ---------------------------------------------------------------------- */

int main(void)
{
    test_state_values_are_distinct();
    test_priority_ordering();
    test_thread_id_invalid_is_zero();
    test_zero_initialized_thread_is_created_state();
    test_zero_initialized_thread_has_invalid_id();
    test_runq_node_is_embedded();
    test_name_buffer_capacity();

    if (tests_failed == 0)
        return 0;
    else
        return 1;
}
