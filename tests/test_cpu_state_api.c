/*
 * tests/test_cpu_state_api.c
 *
 * Compile-check translation unit for the context-switch contract surface.
 * Phase 4, Task 5.
 *
 * This is not a runtime test. Successful compilation is the pass condition.
 *
 * Verified at compile time:
 *   - cpu_state.h is self-contained and includes its own dependencies
 *   - struct arch_cpu_state has non-zero size (KERN_STATIC_ASSERT)
 *   - arch_cpu_state_init() accepts the declared parameter types
 *   - arch_cpu_state_switch() accepts the declared parameter types
 *
 * Runtime behavioral verification — that a switch actually transfers
 * execution to a new thread — requires real register save/restore and
 * is deferred to the architecture port that provides assembly backing.
 */

#include <cpu_state.h>
#include <kernul/types.h>

/*
 * Type-consistency checks via function pointer assignment.
 * A signature mismatch produces a compile error, not a runtime failure.
 */

/* arch_cpu_state_init: void (state*, entry_fn, arg, stack_top) */
static void (*const check_init)(struct arch_cpu_state *,
                                 void (*)(void *),
                                 void *,
                                 void *) = arch_cpu_state_init;

/* arch_cpu_state_switch: void (prev*, next*) */
static void (*const check_switch)(struct arch_cpu_state *,
                                   struct arch_cpu_state *) = arch_cpu_state_switch;

/*
 * Suppress unused-variable warnings. The assignments above are the check;
 * the variables themselves are not used further.
 */
static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_init;
    (void)check_switch;
}
