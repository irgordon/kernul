/*
 * tests/test_spinlock.c
 *
 * Deterministic tests for include/kernul/spinlock.h. Phase 3.
 *
 * Tests run in a hosted single-CPU environment. No hardware required.
 *
 * SMP correctness (ordering, contention) cannot be tested here and is
 * deferred until a real SMP implementation exists. Those tests are
 * noted below as out of scope with the reason.
 */

#include <kernul/spinlock.h>
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

static void test_static_init_is_unlocked(void)
{
    /*
     * A spinlock initialized with SPINLOCK_INIT must be immediately
     * acquirable without spinning.
     */
    spinlock_t lock = SPINLOCK_INIT;

    spin_lock(&lock);
    CHECK("SPINLOCK_INIT produces an acquirable lock", 1);
    spin_unlock(&lock);
}

static void test_spin_init_is_unlocked(void)
{
    /*
     * A spinlock initialized with spin_init() must behave identically
     * to one initialized with SPINLOCK_INIT.
     */
    spinlock_t lock;

    spin_init(&lock);
    spin_lock(&lock);
    CHECK("spin_init() produces an acquirable lock", 1);
    spin_unlock(&lock);
}

static void test_lock_unlock_lock_again(void)
{
    /*
     * After a lock/unlock cycle, the lock must be acquirable again.
     * This verifies that spin_unlock() resets the lock to the unlocked
     * state rather than leaving it in a broken intermediate state.
     */
    spinlock_t lock = SPINLOCK_INIT;

    spin_lock(&lock);
    spin_unlock(&lock);
    spin_lock(&lock);
    CHECK("lock is acquirable after a full lock/unlock cycle", 1);
    spin_unlock(&lock);
}

static void test_critical_section_observes_writes(void)
{
    /*
     * Writes performed inside a critical section must be visible after
     * unlock, even in the single-CPU case. This catches a compiler
     * reordering failure where the write is sunk past spin_unlock().
     */
    spinlock_t lock = SPINLOCK_INIT;
    volatile int shared = 0;

    spin_lock(&lock);
    shared = 42;
    spin_unlock(&lock);

    CHECK("write inside critical section is visible after unlock",
          shared == 42);
}

static void test_multiple_locks_independent(void)
{
    /*
     * Two distinct spinlocks must not interfere with each other.
     * Acquiring one must not affect the acquirability of the other.
     */
    spinlock_t a = SPINLOCK_INIT;
    spinlock_t b = SPINLOCK_INIT;

    spin_lock(&a);
    spin_lock(&b);
    CHECK("two independent locks can be held simultaneously", 1);
    spin_unlock(&b);
    spin_unlock(&a);
}

/* -------------------------------------------------------------------------
 * Out-of-scope notes
 *
 * The following properties are not tested here because they require
 * either SMP hardware or a threading model not available at Phase 3:
 *
 *   - Contention: a second CPU spinning on a held lock eventually
 *     acquires it after unlock. Requires real SMP or thread simulation.
 *
 *   - Ordering: stores before unlock are visible to the next acquirer
 *     on a different CPU. Requires SMP with a weak memory model (e.g.
 *     AArch64) to be meaningful.
 *
 *   - Fairness: not guaranteed by the contract; not tested.
 * ---------------------------------------------------------------------- */

int main(void)
{
    test_static_init_is_unlocked();
    test_spin_init_is_unlocked();
    test_lock_unlock_lock_again();
    test_critical_section_observes_writes();
    test_multiple_locks_independent();

    if (tests_failed == 0)
        return 0;
    else
        return 1;
}
