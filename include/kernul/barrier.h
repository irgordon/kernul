#ifndef KERNUL_BARRIER_H
#define KERNUL_BARRIER_H

/*
 * include/kernul/barrier.h
 *
 * Memory barrier contract surface. §2.11, Phase 2
 *
 * Defines the four barrier operations available to all kernel subsystems.
 * Does not define hardware instructions. Architecture code in
 * arch/<target>/barrier.h provides the implementations via static inline
 * functions included through <barrier_impl.h>.
 *
 * Memory ordering vocabulary: docs/MEMORY_ORDERING.md
 *
 * Rules:
 *   - Common code must use these functions, never architecture mnemonics.
 *   - Use the weakest barrier that is correct for the access pattern.
 *   - Barrier pairs must be documented: which producer barrier matches
 *     which consumer barrier.
 *   - barrier_compiler() does not protect against CPU reordering.
 *     Do not use it where CPU ordering is required.
 */

#include <kernul/types.h>

/* -------------------------------------------------------------------------
 * barrier_compiler()
 *
 * Compiler barrier. Prevents the compiler from reordering accesses
 * across this point. Does not emit a hardware instruction.
 *
 * Use for:
 *   - Preventing compiler reordering in regions where hardware ordering
 *     is already sufficient (e.g., within a spin_lock on TSO architectures).
 *   - Marking the boundary of a hand-inspected ordering region.
 *
 * Does NOT protect against CPU reordering.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no    COST: zero (compile-time only)
 * ---------------------------------------------------------------------- */
static inline void barrier_compiler(void)
{
    __asm__ volatile("" ::: "memory");
}

/* -------------------------------------------------------------------------
 * barrier_acquire()
 *
 * Acquire barrier (load fence). All loads and stores that follow in
 * program order are guaranteed to occur after this point.
 *
 * Pair with: barrier_release() on the producer side.
 *
 * Use for:
 *   - Reading a pointer or flag that was published with barrier_release().
 *   - The consumer side of any lock-free publication protocol.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 * COST: free on x86-64 (TSO); one instruction on AArch64/RISC-V
 * ---------------------------------------------------------------------- */
void barrier_acquire(void);

/* -------------------------------------------------------------------------
 * barrier_release()
 *
 * Release barrier (store fence). All loads and stores that precede in
 * program order are guaranteed to be visible before the stores that follow.
 *
 * Pair with: barrier_acquire() on the consumer side.
 *
 * Use for:
 *   - Publishing a data structure to another CPU.
 *   - The producer side of any lock-free publication protocol.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 * COST: free on x86-64 (TSO); one instruction on AArch64/RISC-V
 * ---------------------------------------------------------------------- */
void barrier_release(void);

/* -------------------------------------------------------------------------
 * barrier_full()
 *
 * Full system memory barrier. All memory accesses before this point are
 * complete before any access after it, from the perspective of all CPUs.
 *
 * This is the strongest and most expensive barrier. Use only when
 * acquire/release pairing is insufficient to express the required ordering.
 *
 * Common uses:
 *   - Synchronization points without a lock (e.g., device MMIO flush).
 *   - Ensuring prior stores are visible before an interrupt enable.
 *
 * CONTEXT: any    SLEEP: no    ALLOC: no
 * COST: one mfence (x86-64) or dmb ish (AArch64) — measurably expensive
 *       on out-of-order CPUs. Do not use on hot paths without measurement.
 * ---------------------------------------------------------------------- */
void barrier_full(void);

#endif /* KERNUL_BARRIER_H */
