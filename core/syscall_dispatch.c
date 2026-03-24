/*
 * core/syscall_dispatch.c
 *
 * Common syscall dispatch stub. Phase 5.
 *
 * This is a deterministic placeholder implementation for the dispatch
 * surface defined in include/kernul/syscall.h.
 *
 * Scope at this phase:
 *   - no syscall table
 *   - no per-syscall handlers
 *   - no allocation, I/O, logging, or formatting
 *   - fixed return value for all inputs
 */

#include <kernul/syscall.h>
#include <kernul/errors.h>

long sys_dispatch(long nr,
                  long a0, long a1, long a2,
                  long a3, long a4, long a5)
{
    (void)nr;
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;

    return KERN_ENOSYS;
}
