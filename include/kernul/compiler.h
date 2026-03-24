#ifndef KERNUL_COMPILER_H
#define KERNUL_COMPILER_H

/*
 * include/kernul/compiler.h
 *
 * Compiler portability annotations.
 * Every macro here documents intent, not aesthetics.
 * Do not use decoratively.
 */

#define KERN_NORETURN       __attribute__((noreturn))
#define KERN_NOINLINE       __attribute__((noinline))
#define KERN_ALWAYS_INLINE  __attribute__((always_inline)) inline
#define KERN_PACKED         __attribute__((packed))
#define KERN_ALIGNED(n)     __attribute__((aligned(n)))
#define KERN_UNUSED         __attribute__((unused))
#define KERN_WEAK           __attribute__((weak))

/*
 * KERN_LIKELY / KERN_UNLIKELY
 *
 * Branch prediction hints. Use only where measurement confirms strong
 * directional bias. Incorrect hints degrade performance.
 */
#define KERN_LIKELY(x)      __builtin_expect(!!(x), 1)
#define KERN_UNLIKELY(x)    __builtin_expect(!!(x), 0)

/*
 * KERN_CACHELINE_SIZE
 *
 * Conservative cache-line size for alignment decisions.
 * Architecture code may define a tighter value.
 */
#define KERN_CACHELINE_SIZE    64
#define KERN_CACHELINE_ALIGNED KERN_ALIGNED(KERN_CACHELINE_SIZE)

/*
 * KERN_STATIC_ASSERT(cond, msg)
 *
 * Compile-time assertion. msg must be a string literal.
 */
#define KERN_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)

/*
 * KERN_ARRAY_SIZE(arr)
 *
 * Number of elements in a stack-allocated array.
 * Do not use on a pointer.
 */
#define KERN_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/*
 * KERN_CONTAINER_OF(ptr, type, member)
 *
 * Recover a pointer to the enclosing structure from a pointer to a member.
 * Used with intrusive list and queue designs.
 */
#define KERN_CONTAINER_OF(ptr, type, member) \
    ((type *)((uptr)(ptr) - __builtin_offsetof(type, member)))

#endif /* KERNUL_COMPILER_H */
