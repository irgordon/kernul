/*
 * core/assert.c
 *
 * assert_fail() implementation. Phase 3.
 * Contract: include/kernul/assert.h
 *
 * At Phase 3, the only available terminal is arch_halt(). The expr,
 * file, and line arguments are accepted but unused until a console
 * subsystem exists to print them. They are kept in the signature
 * because the contract must not change when output is added later.
 */

#include <kernul/assert.h>
#include <kernul/arch.h>
#include <kernul/compiler.h>

KERN_NORETURN void assert_fail(const char *expr,
                                const char *file,
                                unsigned int line)
{
    /*
     * expr, file, and line are unused until a console subsystem is
     * available. Suppress warnings explicitly; do not remove the
     * parameters — the signature is the stable contract.
     */
    (void)expr;
    (void)file;
    (void)line;

    arch_halt();
}
