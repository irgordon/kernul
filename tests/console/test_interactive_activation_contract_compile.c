/*
 * tests/console/test_interactive_activation_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/interactive_activation.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_activation.h>
#include <kernul/compiler.h>

static struct interactive_activation *(*const check_interactive_activation_create)(
    struct interactive_console *) = interactive_activation_create;
static u32 (*const check_interactive_activation_state)(
    const struct interactive_activation *) = interactive_activation_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_activation ia = {0};

    (void)check_interactive_activation_create;
    (void)check_interactive_activation_state;

    (void)INTERACTIVE_ACTIVATION_STATE_PENDING;
    (void)INTERACTIVE_ACTIVATION_STATE_RECORDED;
    (void)ia.console;
    (void)ia.consumer_group;
    (void)ia.state;
}
