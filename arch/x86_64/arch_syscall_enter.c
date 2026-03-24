/*
 * arch/x86_64/arch_syscall_enter.c
 *
 * x86-64 syscall entry stub. Phase 5.
 *
 * Declares the architecture-owned syscall trap entry boundary without
 * implementing real trap handling or userspace return.
 */

#include <kernul/syscall.h>
#include <kernul/arch.h>

KERN_NORETURN void arch_syscall_enter(struct arch_cpu_state *state)
{
    (void)state;
    arch_halt();
}
