/*
 * tests/console/test_session_terminal_state_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_terminal_state.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_terminal_state.h>
#include <kernul/compiler.h>
#include <kernul/session.h>

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_ACK_GATE_H
#error "session_terminal_state.h must not include failure ack gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_ACK_GATE_H
#error "session_terminal_state.h must not include completion ack gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_RECORD_H
#error "session_terminal_state.h must not include outcome record headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_VIEW_H
#error "session_terminal_state.h must not include outcome view headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_AGGREGATE_H
#error "session_terminal_state.h must not include outcome aggregate headers"
#endif

static void (*const
    check_session_publish_terminal_state_if_initiation_forbidden)(
    struct session *) =
    session_publish_terminal_state_if_initiation_forbidden;

static bool (*const check_session_is_terminal_acquire)(
    const struct session *) =
    session_is_terminal_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct session session = {0};

    (void)check_session_publish_terminal_state_if_initiation_forbidden;
    (void)check_session_is_terminal_acquire;
    (void)session.terminal_state_published;
}
