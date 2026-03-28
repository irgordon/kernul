/*
 * tests/console/test_interactive_dispatch_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/interactive_dispatch.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_dispatch.h>
#include <kernul/compiler.h>

static struct interactive_dispatch *(*const check_interactive_dispatch_select)(
    struct interactive_runnable *) = interactive_dispatch_select;
static u32 (*const check_interactive_dispatch_state)(
    const struct interactive_dispatch *) = interactive_dispatch_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_dispatch id = {0};

    (void)check_interactive_dispatch_select;
    (void)check_interactive_dispatch_state;

    (void)INTERACTIVE_DISPATCH_STATE_NONE;
    (void)INTERACTIVE_DISPATCH_STATE_DISPATCHED;
    (void)id.runnable;
    (void)id.consumer_group;
    (void)id.state;
}
