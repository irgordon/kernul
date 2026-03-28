/*
 * tests/console/test_interactive_execution_target_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_target.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_target.h>
#include <kernul/compiler.h>

static struct interactive_execution_target *(*const
    check_interactive_execution_prepare)(struct interactive_dispatch *) =
    interactive_execution_prepare;
static u32 (*const check_interactive_execution_target_state)(
    const struct interactive_execution_target *) =
    interactive_execution_target_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_target iet = {0};

    (void)check_interactive_execution_prepare;
    (void)check_interactive_execution_target_state;

    (void)INTERACTIVE_EXECUTION_TARGET_STATE_NONE;
    (void)INTERACTIVE_EXECUTION_TARGET_STATE_PREPARED;
    (void)iet.dispatch;
    (void)iet.consumer_group;
    (void)iet.state;
}
