#ifndef KERNUL_TYPES_H
#define KERNUL_TYPES_H

/*
 * include/kernul/types.h
 *
 * Kernel primitive types. All subsystems use these names.
 * No subsystem defines its own integer types.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

typedef uintptr_t uptr;
typedef ptrdiff_t sptr;

/* ssize_t: signed size, used for read/write return values */
typedef s64       ssize_t;

/*
 * Address types.
 * phys_addr_t and virt_addr_t are not interchangeable.
 * Implicit conversion between them is a defect.
 */
typedef uptr phys_addr_t;
typedef uptr virt_addr_t;

/*
 * Error type.
 *
 * Zero is success. Negative values are errors. Positive values are
 * subsystem-defined results. Every return value must be checked.
 * Interfaces that cannot fail must document why.
 *
 * Error codes are defined in include/kernul/errors.h.
 */
typedef s32 kern_err_t;

#endif /* KERNUL_TYPES_H */
