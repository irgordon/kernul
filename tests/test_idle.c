/*
 * tests/test_idle.c
 *
 * Tests for include/kernul/idle.h and sched/idle.c. Phase 4, Task 6.
 *
 * Test boundary:
 *   - idle_init() preconditions, post-init invariants, and identity fields
 *   - idle_thread() returns the correct pointer after init
 *   - The idle thread's structural relationship to the run queue
 *
 * Not tested here:
 *   - Idle thread execution (idle_entry runs forever; not reachable)
 *   - Current thread bootstrap (depends on scheduler start path, later phase)
 *   - Context switching (Task 5 / real architecture port)
 */

#include <kernul/idle.h>
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
 * Tests
 * ---------------------------------------------------------------------- */

static void test_idle_init_succeeds(void)
{
    kern_err_t err = idle_init();
    CHECK("idle_init returns KERN_OK", err == KERN_OK);
}

static void test_idle_thread_pointer_valid(void)
{
    thread_t *t = idle_thread();
    CHECK("idle_thread() returns non-NULL after init", t != NULL);
}

static void test_idle_thread_identity(void)
{
    thread_t *t = idle_thread();

    CHECK("idle thread id is THREAD_ID_IDLE", t->id == THREAD_ID_IDLE);
    CHECK("idle thread priority is THREAD_PRIO_IDLE",
          t->prio == THREAD_PRIO_IDLE);
}

static void test_idle_thread_name(void)
{
    thread_t *t = idle_thread();

    CHECK("idle thread name starts with 'i'", t->name[0] == 'i');
    CHECK("idle thread name[1] is 'd'",       t->name[1] == 'd');
    CHECK("idle thread name[2] is 'l'",       t->name[2] == 'l');
    CHECK("idle thread name[3] is 'e'",       t->name[3] == 'e');
    CHECK("idle thread name is NUL-terminated", t->name[4] == '\0');
}

static void test_idle_thread_state_after_init(void)
{
    /*
     * idle_init() enqueues the idle thread via sched_enqueue(), which
     * requires THREAD_RUNNABLE on entry. The idle thread must therefore
     * be in THREAD_RUNNABLE state after idle_init() returns. It is not
     * yet THREAD_RUNNING; that transition occurs in the scheduler start
     * path, which is a later deliverable.
     */
    thread_t *t = idle_thread();
    CHECK("idle thread is THREAD_RUNNABLE after idle_init",
          t->state == THREAD_RUNNABLE);
}

static void test_idle_thread_on_run_queue(void)
{
    /*
     * Verify the idle thread is visible to the scheduler queue after
     * idle_init(). Dequeue it and confirm it is the idle thread.
     *
     * This proves that idle_init() successfully called sched_enqueue(),
     * and that the idle thread's runq_node embedding is correct.
     *
     * This does not prove that scheduler bootstrap is complete. The
     * current pointer is still NULL; the idle thread has not been
     * transitioned to THREAD_RUNNING. Bootstrap completion depends on
     * the scheduler start path, which is a later deliverable.
     */
    thread_t *got = sched_dequeue();
    CHECK("idle thread is visible to the scheduler queue after idle_init",
          got == idle_thread());
}

static void test_idle_id_not_invalid(void)
{
    CHECK("THREAD_ID_IDLE is not THREAD_ID_INVALID",
          THREAD_ID_IDLE != THREAD_ID_INVALID);
}

static void test_idle_priority_is_lowest(void)
{
    /*
     * THREAD_PRIO_IDLE must be the numerically highest priority value,
     * making it the lowest scheduling priority. Other threads with any
     * defined priority value will have a lower number and thus higher
     * priority.
     */
    CHECK("THREAD_PRIO_IDLE is higher number than THREAD_PRIO_LOW",
          THREAD_PRIO_IDLE > THREAD_PRIO_LOW);
    CHECK("THREAD_PRIO_IDLE is higher number than THREAD_PRIO_NORMAL",
          THREAD_PRIO_IDLE > THREAD_PRIO_NORMAL);
    CHECK("THREAD_PRIO_IDLE is higher number than THREAD_PRIO_HIGH",
          THREAD_PRIO_IDLE > THREAD_PRIO_HIGH);
    CHECK("THREAD_PRIO_IDLE is higher number than THREAD_PRIO_KERNEL",
          THREAD_PRIO_IDLE > THREAD_PRIO_KERNEL);
}

/* -------------------------------------------------------------------------
 * Test runner
 * ---------------------------------------------------------------------- */

int main(void)
{
    /*
     * sched_init() must precede idle_init(). Tests run in dependency order.
     */
    kern_err_t err = sched_init();
    if (err != KERN_OK)
        return 1;

    test_idle_id_not_invalid();
    test_idle_priority_is_lowest();
    test_idle_init_succeeds();
    test_idle_thread_pointer_valid();
    test_idle_thread_identity();
    test_idle_thread_name();
    test_idle_thread_state_after_init();
    test_idle_thread_on_run_queue();

    if (tests_failed == 0)
        return 0;
    else
        return 1;
}
