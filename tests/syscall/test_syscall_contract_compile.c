/*
 * tests/syscall/test_syscall_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/syscall.h.
 * Pass condition: successful compilation.
 */

#include <kernul/syscall.h>

static long (*const check_dispatch)(long,
                                    long, long, long,
                                    long, long, long) = sys_dispatch;

static void (*const check_enter)(struct arch_cpu_state *) = arch_syscall_enter;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_dispatch;
    (void)check_enter;
}
