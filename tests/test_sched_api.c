/*
 * tests/test_sched_api.c
 *
 * Compile-check translation unit for include/kernul/sched.h.
 * Phase 4, Task 3.
 *
 * This is not a runtime test. It is not linked into an executable for
 * Task 3. Its purpose is to verify at compile time that:
 *   - sched.h is internally consistent and includes its own dependencies
 *   - the five declared functions have the expected return types and
 *     parameter types, expressed as function-pointer assignments that
 *     the compiler type-checks
 *
 * Successful compilation of this translation unit is the pass condition.
 * Runtime behavioral tests are deferred to Task 4, when implementations
 * exist and the binary can be fully linked.
 */

#include <kernul/sched.h>
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
 * Type-consistency checks
 *
 * Assign each declared function to a typed function pointer. If any
 * declared signature does not match, this produces a compile error.
 * These are compile-time checks expressed as runtime-reachable code
 * so they participate in the test binary rather than being invisible
 * to the build system.
 * ---------------------------------------------------------------------- */

static void test_sched_init_signature(void)
{
    kern_err_t (*fn)(void) = sched_init;
    CHECK("sched_init has expected signature", fn == sched_init);
}

static void test_sched_enqueue_signature(void)
{
    void (*fn)(thread_t *) = sched_enqueue;
    CHECK("sched_enqueue has expected signature", fn == sched_enqueue);
}

static void test_sched_dequeue_signature(void)
{
    thread_t *(*fn)(void) = sched_dequeue;
    CHECK("sched_dequeue has expected signature", fn == sched_dequeue);
}

static void test_sched_yield_signature(void)
{
    void (*fn)(void) = sched_yield;
    CHECK("sched_yield has expected signature", fn == sched_yield);
}

static void test_sched_current_signature(void)
{
    thread_t *(*fn)(void) = sched_current;
    CHECK("sched_current has expected signature", fn == sched_current);
}

/* -------------------------------------------------------------------------
 * Test runner
 * ---------------------------------------------------------------------- */

int main(void)
{
    test_sched_init_signature();
    test_sched_enqueue_signature();
    test_sched_dequeue_signature();
    test_sched_yield_signature();
    test_sched_current_signature();

    if (tests_failed == 0)
        return 0;
    else
        return 1;
}
