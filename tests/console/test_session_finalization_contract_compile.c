/*
 * tests/console/test_session_finalization_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_finalization.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_finalization.h>
#include <kernul/compiler.h>

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_ACK_GATE_H
#error "session_finalization.h must not include failure ack gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_ACK_GATE_H
#error "session_finalization.h must not include completion ack gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_RECORD_H
#error "session_finalization.h must not include outcome record headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_VIEW_H
#error "session_finalization.h must not include outcome view headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_OUTCOME_AGGREGATE_H
#error "session_finalization.h must not include outcome aggregate headers"
#endif

#ifdef KERNUL_SESSION_TERMINAL_CAUSE_H
#error "session_finalization.h must not include terminal cause headers"
#endif

#ifdef KERNUL_SCHED_H
#error "session_finalization.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_finalization.h must not include execution headers"
#endif

static void (*const check_session_publish_finalized_if_terminal)(
    struct session *) = session_publish_finalized_if_terminal;

static bool (*const check_session_is_finalized_acquire)(
    const struct session *) = session_is_finalized_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_publish_finalized_if_terminal;
    (void)check_session_is_finalized_acquire;
}
