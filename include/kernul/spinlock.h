#ifndef KERNUL_SPINLOCK_H
#define KERNUL_SPINLOCK_H

/*
 * include/kernul/spinlock.h
 *
 * Minimal spinlock contract. Phase 3.
 *
 * Provides busy-waiting mutual exclusion for short, bounded critical
 * sections. The spinlock is the lowest-level synchronization primitive;
 * all higher-level primitives build on it.
 *
 * Ordering contract (MEMORY_ORDERING.md §8):
 *   spin_lock()   provides acquire semantics on entry.
 *   spin_unlock() provides release semantics on exit.
 *   Code inside the critical section is ordered with respect to any
 *   code that subsequently acquires the same lock on another CPU.
 *
 * Usage rules:
 *   - Must not be acquired from a context that may sleep.
 *   - Must not be held across any call that may block.
 *   - Must not be acquired recursively; a holder that re-acquires
 *     the same lock will deadlock.
 *   - Lock nesting order must be documented at every nesting site.
 *   - Hold durations must be short and bounded.
 *
 * IRQ-safe variants (spin_lock_irq / spin_unlock_irq) are not defined
 * here. No interrupt handler exists at Phase 3; those variants are
 * added when a requiring task arrives.
 */

#include <kernul/types.h>
#include <kernul/compiler.h>

/* -------------------------------------------------------------------------
 * Type
 * ---------------------------------------------------------------------- */

typedef struct {
    volatile u32 _val;   /* 0 = unlocked, 1 = locked; do not read directly */
} spinlock_t;

/* -------------------------------------------------------------------------
 * Static initializer
 *
 * Use SPINLOCK_INIT when declaring a spinlock at file scope or inside
 * a struct initializer. Use spin_init() for heap-allocated structures.
 * ---------------------------------------------------------------------- */
#define SPINLOCK_INIT  { ._val = 0 }

/* -------------------------------------------------------------------------
 * spin_init(lock)
 *
 * Initialize a spinlock at runtime. Must be called before first use on
 * any lock not initialized with SPINLOCK_INIT.
 *
 * CONTEXT: any, before first use
 * SLEEP:   no    ALLOC: no
 * ---------------------------------------------------------------------- */
void spin_init(spinlock_t *lock);

/* -------------------------------------------------------------------------
 * spin_lock(lock)
 *
 * Acquire the lock. Spins until the lock is available.
 * Provides acquire semantics on entry: all subsequent memory operations
 * in program order are visible after the lock is held.
 *
 * CONTEXT: any context that must not sleep
 * SLEEP:   no    ALLOC: no
 * FAILURE: none (spins until acquired)
 * ---------------------------------------------------------------------- */
void spin_lock(spinlock_t *lock);

/* -------------------------------------------------------------------------
 * spin_unlock(lock)
 *
 * Release the lock. Caller must be the current holder.
 * Provides release semantics on exit: all preceding memory operations
 * in program order are visible before the lock is released.
 *
 * CONTEXT: same context that called spin_lock()
 * SLEEP:   no    ALLOC: no
 * ---------------------------------------------------------------------- */
void spin_unlock(spinlock_t *lock);

#endif /* KERNUL_SPINLOCK_H */
