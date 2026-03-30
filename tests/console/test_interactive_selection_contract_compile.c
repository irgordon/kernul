/*
 * tests/console/test_interactive_selection_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_selection.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_selection.h>
#include <kernul/compiler.h>
#include <kernul/interactive_scheduler_container.h>
#include <kernul/interactive_scheduler_state.h>
#include <kernul/session.h>

static struct interactive_selection *(*const check_interactive_selection_select)(
    struct interactive_runnable *) = interactive_selection_select;
static u32 (*const check_interactive_selection_state)(
    const struct interactive_selection *) = interactive_selection_state;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_selection sel = {0};
    struct session session = {0};
    struct interactive_scheduler_state state = {0};
    struct interactive_scheduler_container container = {0};

    (void)check_interactive_selection_select;
    (void)check_interactive_selection_state;

    (void)INTERACTIVE_SELECTION_STATE_NONE;
    (void)INTERACTIVE_SELECTION_STATE_SELECTED;
    (void)sel.runnable;
    (void)sel.dispatch;
    (void)sel.consumer_group;
    (void)sel.state;
    (void)session.scheduler_state;
    (void)session.scheduler_container;
    (void)state.session;
    (void)state.runnable;
    (void)container.session;
    (void)container.scheduler_state;
    (void)container.runnable;
}
