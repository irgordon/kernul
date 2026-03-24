#ifndef ARCH_STUB_BARRIER_IMPL_H
#define ARCH_STUB_BARRIER_IMPL_H

/*
 * arch/stub/barrier_impl.h
 *
 * Stub implementations of the barrier.h contract surface.
 *
 * STUB: all barriers reduce to a compiler barrier on the stub target.
 * This is sufficient for single-CPU hosted builds.
 * A real architecture provides the correct hardware fence instruction.
 */

static inline void barrier_acquire(void)
{
    __asm__ volatile("" ::: "memory");
}

static inline void barrier_release(void)
{
    __asm__ volatile("" ::: "memory");
}

static inline void barrier_full(void)
{
    __asm__ volatile("" ::: "memory");
}

#endif /* ARCH_STUB_BARRIER_IMPL_H */
