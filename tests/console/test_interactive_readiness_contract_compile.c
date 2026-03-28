/*
 * tests/console/test_interactive_readiness_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/interactive_readiness.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_readiness.h>
#include <kernul/compiler.h>

static struct interactive_readiness *(*const check_interactive_readiness_mark_ready)(
    struct interactive_activation *) = interactive_readiness_mark_ready;
static u32 (*const check_interactive_readiness_state)(
    const struct interactive_readiness *) = interactive_readiness_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_readiness ir = {0};

    (void)check_interactive_readiness_mark_ready;
    (void)check_interactive_readiness_state;

    (void)INTERACTIVE_READINESS_STATE_INACTIVE;
    (void)INTERACTIVE_READINESS_STATE_READY;
    (void)ir.activation;
    (void)ir.consumer_group;
    (void)ir.state;
}
