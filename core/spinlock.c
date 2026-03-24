/*
 * core/spinlock.c
 *
 * Spinlock stub implementation. Phase 3.
 * Contract: include/kernul/spinlock.h
 *
 * STUB: this implementation is correct for a single CPU in a hosted
 * build. It is not SMP-safe.
 *
 * What the real implementation requires (not in scope at Phase 3):
 *   - An atomic test-and-set or compare-and-swap instruction to make
 *     acquisition SMP-safe. See include/kernul/atomic.h.
 *   - hardware acquire barrier on spin_lock() exit.
 *   - hardware release barrier on spin_unlock() entry.
 *   - Preemption disable on entry and re-enable on exit.
 *
 * The compiler barrier in spin_lock() and spin_unlock() is correct
 * for the single-CPU hosted stub: it prevents the compiler from
 * hoisting or sinking memory accesses across the lock boundary.
 * It does not provide CPU-level ordering for SMP. Replace it with
 * barrier_acquire() and barrier_release() when real hardware support
 * is in place.
 */

#include <kernul/spinlock.h>
#include <kernul/barrier.h>

void spin_init(spinlock_t *lock)
{
    lock->_val = 0;
}

void spin_lock(spinlock_t *lock)
{
    /*
     * STUB: spin on the flag then claim it. Not SMP-safe.
     * Real: atomic test-and-set with acquire barrier.
     */
    while (lock->_val != 0)
        ;

    lock->_val = 1;

    /*
     * Compiler barrier: prevent the compiler from moving accesses
     * that belong inside the critical section to before this point.
     * Does not provide CPU ordering. Replace with barrier_acquire()
     * when SMP support is introduced.
     */
    barrier_compiler();
}

void spin_unlock(spinlock_t *lock)
{
    /*
     * Compiler barrier: prevent the compiler from moving accesses
     * that belong inside the critical section to after this point.
     * Does not provide CPU ordering. Replace with barrier_release()
     * when SMP support is introduced.
     */
    barrier_compiler();

    lock->_val = 0;
}
