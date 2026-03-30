/*
 * tests/console/test_interactive_scheduler_state_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_scheduler_state.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_scheduler_state.h>
#include <kernul/compiler.h>
#include <kernul/session.h>

static struct interactive_scheduler_state *(*const check_interactive_scheduler_state_create)(
    struct session *,
    struct interactive_runnable *) = interactive_scheduler_state_create;
static u32 (*const check_interactive_scheduler_state_state)(
    const struct interactive_scheduler_state *) = interactive_scheduler_state_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_scheduler_state state = {0};
    struct session session = {0};

    (void)check_interactive_scheduler_state_create;
    (void)check_interactive_scheduler_state_state;

    (void)INTERACTIVE_SCHEDULER_STATE_NONE;
    (void)INTERACTIVE_SCHEDULER_STATE_READY;
    (void)state.session;
    (void)state.runnable;
    (void)state.state;
    (void)session.scheduler_state;
    (void)session.scheduler_state_live;
}
