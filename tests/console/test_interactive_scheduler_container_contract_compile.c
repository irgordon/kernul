/*
 * tests/console/test_interactive_scheduler_container_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_scheduler_container.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_scheduler_container.h>
#include <kernul/compiler.h>
#include <kernul/interactive_scheduler_state.h>
#include <kernul/session.h>

static struct interactive_scheduler_container *(*const check_interactive_scheduler_container_materialize)(
    struct interactive_scheduler_state *) = interactive_scheduler_container_materialize;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_scheduler_container container = {0};
    struct session session = {0};

    (void)check_interactive_scheduler_container_materialize;
    (void)container.session;
    (void)container.scheduler_state;
    (void)container.runnable;
    (void)session.scheduler_container;
    (void)session.scheduler_container_live;
}
