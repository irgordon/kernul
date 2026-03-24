#ifndef KERNUL_ASSERT_H
#define KERNUL_ASSERT_H

/*
 * include/kernul/assert.h
 *
 * Kernel assertion surface. Phase 3.
 *
 * Provides:
 *   assert_fail(expr, file, line)  — terminal failure point
 *   ASSERT(cond)                   — runtime assertion macro
 *
 * assert_fail() is the single implementation point for all assertion
 * failures. All failures route through it so the site is unambiguous.
 *
 * At Phase 3, assert_fail() calls arch_halt() directly. Console output
 * is added when a console subsystem exists. The contract does not change;
 * only the implementation depth grows.
 *
 * ASSERT(cond) evaluates cond exactly once.
 * Use only for invariants that must always hold.
 * Do not use on hot paths unless the check cost is negligible.
 */

#include <kernul/compiler.h>

/* -------------------------------------------------------------------------
 * assert_fail
 *
 * Called when an assertion fails. Never returns.
 *
 * expr:  the stringified condition that was false
 * file:  source file path (__FILE__)
 * line:  source line number (__LINE__)
 *
 * CONTEXT: any
 * SLEEP:   no    ALLOC: no
 * ---------------------------------------------------------------------- */
KERN_NORETURN void assert_fail(const char *expr,
                                const char *file,
                                unsigned int line);

/* -------------------------------------------------------------------------
 * ASSERT(cond)
 *
 * Evaluates cond. If false, calls assert_fail() with source location.
 * KERN_UNLIKELY reflects that failures must be vanishingly rare in
 * correct code; the hint keeps the passing path free of branch overhead.
 * ---------------------------------------------------------------------- */
#define ASSERT(cond) \
    do { \
        if (KERN_UNLIKELY(!(cond))) { \
            assert_fail(#cond, __FILE__, __LINE__); \
        } \
    } while (0)

#endif /* KERNUL_ASSERT_H */
