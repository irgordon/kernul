/*
 * tests/console/test_interactive_runnable_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/interactive_runnable.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_runnable.h>
#include <kernul/compiler.h>

static struct interactive_runnable *(*const check_interactive_runnable_enqueue)(
    struct interactive_admission *) = interactive_runnable_enqueue;
static u32 (*const check_interactive_runnable_state)(
    const struct interactive_runnable *) = interactive_runnable_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_runnable ir = {0};

    (void)check_interactive_runnable_enqueue;
    (void)check_interactive_runnable_state;

    (void)INTERACTIVE_RUNNABLE_STATE_NONE;
    (void)INTERACTIVE_RUNNABLE_STATE_ENQUEUED;
    (void)ir.admission;
    (void)ir.consumer_group;
    (void)ir.state;
}
