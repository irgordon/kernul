#ifndef ARCH_STUB_ATOMIC_IMPL_H
#define ARCH_STUB_ATOMIC_IMPL_H

/*
 * arch/stub/atomic_impl.h
 *
 * Stub implementations of the atomic.h contract surface.
 * Included by arch/<target>/atomic.h via <atomic_impl.h>.
 *
 * STUB: these are not SMP-safe. They satisfy the interface contract for
 * single-CPU hosted build verification only.
 * A real architecture provides lock-prefixed instructions (x86-64),
 * ldxr/stxr loops (AArch64), or lr/sc loops (RISC-V).
 */

#include <kernul/types.h>
#include <kernul/atomic.h>

static inline u32 atomic32_load_relaxed(const atomic32_t *a)
    { return a->_val; }

static inline u32 atomic32_load_acquire(const atomic32_t *a)
    { u32 v = a->_val; __asm__ volatile("" ::: "memory"); return v; }

static inline void atomic32_store_relaxed(atomic32_t *a, u32 val)
    { a->_val = val; }

static inline void atomic32_store_release(atomic32_t *a, u32 val)
    { __asm__ volatile("" ::: "memory"); a->_val = val; }

static inline u32 atomic32_add(atomic32_t *a, u32 delta)
    { u32 old = a->_val; a->_val += delta; return old; }

static inline u32 atomic32_sub(atomic32_t *a, u32 delta)
    { u32 old = a->_val; a->_val -= delta; return old; }

static inline void atomic32_inc(atomic32_t *a)
    { a->_val++; }

static inline u32 atomic32_dec(atomic32_t *a)
    { return --(a->_val); }

static inline void atomic32_or(atomic32_t *a, u32 bits)
    { a->_val |= bits; }

static inline void atomic32_and(atomic32_t *a, u32 bits)
    { a->_val &= bits; }

static inline u32 atomic32_xchg(atomic32_t *a, u32 val)
    { u32 old = a->_val; a->_val = val; return old; }

static inline bool atomic32_cmpxchg(atomic32_t *a, u32 *expected, u32 desired)
{
    if (a->_val == *expected) { a->_val = desired; return true; }
    *expected = a->_val;
    return false;
}

static inline u64 atomic64_load_relaxed(const atomic64_t *a)
    { return a->_val; }

static inline u64 atomic64_load_acquire(const atomic64_t *a)
    { u64 v = a->_val; __asm__ volatile("" ::: "memory"); return v; }

static inline void atomic64_store_relaxed(atomic64_t *a, u64 val)
    { a->_val = val; }

static inline void atomic64_store_release(atomic64_t *a, u64 val)
    { __asm__ volatile("" ::: "memory"); a->_val = val; }

static inline u64 atomic64_add(atomic64_t *a, u64 delta)
    { u64 old = a->_val; a->_val += delta; return old; }

static inline u64 atomic64_dec(atomic64_t *a)
    { return --(a->_val); }

static inline bool atomic64_cmpxchg(atomic64_t *a, u64 *expected, u64 desired)
{
    if (a->_val == *expected) { a->_val = desired; return true; }
    *expected = a->_val;
    return false;
}

static inline void *atomicptr_load_acquire(const atomicptr_t *a)
    { void *v = (void *)a->_val; __asm__ volatile("" ::: "memory"); return v; }

static inline void atomicptr_store_release(atomicptr_t *a, void *ptr)
    { __asm__ volatile("" ::: "memory"); a->_val = (uptr)ptr; }

static inline bool atomicptr_cmpxchg(atomicptr_t *a, void **expected,
                                       void *desired)
{
    if (a->_val == (uptr)*expected) {
        a->_val = (uptr)desired;
        return true;
    }
    *expected = (void *)a->_val;
    return false;
}

#endif /* ARCH_STUB_ATOMIC_IMPL_H */
