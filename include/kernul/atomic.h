#ifndef KERNUL_ATOMIC_H
#define KERNUL_ATOMIC_H

/*
 * include/kernul/atomic.h
 *
 * Atomic operation contract surface. §2.11, Phase 2
 *
 * This header defines the names, semantics, and ordering contracts for
 * all atomic operations used in KERNUL. It does not define hardware
 * instructions. Architecture code in arch/<target>/atomic.h provides
 * the implementations.
 *
 * Memory ordering vocabulary is defined in docs/MEMORY_ORDERING.md.
 * The ordering suffixes used here correspond directly to that vocabulary:
 *
 *   _relaxed     — no ordering; atomicity only
 *   _acquire     — acquire barrier on load
 *   _release     — release barrier on store
 *   _acqrel      — acquire-release on read-modify-write
 *   (no suffix)  — full barrier; use only when acquire/release is insufficient
 *
 * Common code must use these names exclusively.
 * Common code must never call architecture assembly directly for ordering.
 *
 * Types:
 *   atomic32_t   — 32-bit atomic integer
 *   atomic64_t   — 64-bit atomic integer
 *   atomicptr_t  — pointer-width atomic (uptr)
 */

#include <kernul/types.h>
#include <kernul/compiler.h>

/* -------------------------------------------------------------------------
 * Atomic types
 * ---------------------------------------------------------------------- */

typedef struct { volatile u32 _val; } atomic32_t;
typedef struct { volatile u64 _val; } atomic64_t;
typedef struct { volatile uptr _val; } atomicptr_t;

#define ATOMIC32_INIT(v)   { ._val = (v) }
#define ATOMIC64_INIT(v)   { ._val = (v) }
#define ATOMICPTR_INIT(v)  { ._val = (uptr)(v) }

/* -------------------------------------------------------------------------
 * 32-bit atomic loads
 *
 * atomic32_load_relaxed(a)   — load with no ordering
 * atomic32_load_acquire(a)   — load with acquire semantics
 * ---------------------------------------------------------------------- */
u32 atomic32_load_relaxed(const atomic32_t *a);
u32 atomic32_load_acquire(const atomic32_t *a);

/* -------------------------------------------------------------------------
 * 32-bit atomic stores
 *
 * atomic32_store_relaxed(a, val)  — store with no ordering
 * atomic32_store_release(a, val)  — store with release semantics
 * ---------------------------------------------------------------------- */
void atomic32_store_relaxed(atomic32_t *a, u32 val);
void atomic32_store_release(atomic32_t *a, u32 val);

/* -------------------------------------------------------------------------
 * 32-bit atomic read-modify-write
 *
 * atomic32_add(a, delta)   — add delta; return previous value; acqrel
 * atomic32_sub(a, delta)   — subtract delta; return previous value; acqrel
 * atomic32_inc(a)          — increment; no ordering guarantee needed for
 *                            counters; use _relaxed variant if available
 * atomic32_dec(a)          — decrement; returns new value (for refcounts)
 * atomic32_or(a, bits)     — bitwise OR; acqrel
 * atomic32_and(a, bits)    — bitwise AND; acqrel
 * atomic32_xchg(a, val)    — exchange; return previous value; acqrel
 *
 * atomic32_cmpxchg(a, expected, desired)
 *   If *a == expected, store desired and return true.
 *   If *a != expected, return false (and update *expected to observed value
 *   on some architectures — do not rely on *expected after a failed cmpxchg
 *   without re-reading it explicitly).
 *   Ordering: acqrel on success, acquire on failure.
 * ---------------------------------------------------------------------- */
u32  atomic32_add(atomic32_t *a, u32 delta);
u32  atomic32_sub(atomic32_t *a, u32 delta);
void atomic32_inc(atomic32_t *a);
u32  atomic32_dec(atomic32_t *a);            /* returns new value */
void atomic32_or(atomic32_t *a, u32 bits);
void atomic32_and(atomic32_t *a, u32 bits);
u32  atomic32_xchg(atomic32_t *a, u32 val);
bool atomic32_cmpxchg(atomic32_t *a, u32 *expected, u32 desired);

/* -------------------------------------------------------------------------
 * 64-bit atomic operations
 *
 * Same semantics as the 32-bit variants above.
 * 64-bit atomics may not be available on all architectures. Architecture
 * code must define ARCH_HAS_ATOMIC64 if it provides these.
 * Common code that uses 64-bit atomics must guard with #ifdef ARCH_HAS_ATOMIC64.
 * ---------------------------------------------------------------------- */
u64  atomic64_load_relaxed(const atomic64_t *a);
u64  atomic64_load_acquire(const atomic64_t *a);
void atomic64_store_relaxed(atomic64_t *a, u64 val);
void atomic64_store_release(atomic64_t *a, u64 val);
u64  atomic64_add(atomic64_t *a, u64 delta);
u64  atomic64_dec(atomic64_t *a);
bool atomic64_cmpxchg(atomic64_t *a, u64 *expected, u64 desired);

/* -------------------------------------------------------------------------
 * Pointer-width atomic operations
 *
 * Used for publishing and consuming pointers between CPUs.
 * Ordering follows the same rules as the typed variants.
 * ---------------------------------------------------------------------- */
void *atomicptr_load_acquire(const atomicptr_t *a);
void  atomicptr_store_release(atomicptr_t *a, void *ptr);
bool  atomicptr_cmpxchg(atomicptr_t *a, void **expected, void *desired);

/* -------------------------------------------------------------------------
 * Architecture implementation
 *
 * arch/<target>/atomic.h provides static inline implementations of all
 * operations declared above. The build system sets -Iarch/<target> so
 * that the correct implementation is found by:
 *
 *   #include <atomic_impl.h>
 *
 * That include is in arch/<target>/atomic.h, not here.
 * Common code includes only this header.
 * ---------------------------------------------------------------------- */

#endif /* KERNUL_ATOMIC_H */
