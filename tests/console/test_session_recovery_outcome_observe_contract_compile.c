/*
 * tests/console/test_session_recovery_outcome_observe_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_recovery_outcome_observe.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_recovery_outcome_observe.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_recovery_outcome_observe.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_recovery_outcome_observe.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_recovery_outcome_observe.h must not include scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_recovery_outcome_observe.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_recovery_outcome_observe.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_recovery_outcome_observe.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_recovery_outcome_observe.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_recovery_outcome_observe.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_recovery_outcome_observe.h must not include signaling headers"
#endif

static bool
    (*const check_session_get_recovery_outcome_info_acquire)(
        const struct session *,
        session_recovery_outcome_info_t *) =
            session_get_recovery_outcome_info_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    session_recovery_outcome_info_t info = {0};

    (void)check_session_get_recovery_outcome_info_acquire;
    (void)info.outcome;
    (void)info.outcome_timestamp;
    (void)info.execution_attempt_count;
}
