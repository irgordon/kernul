/*
 * tests/console/test_session_retry_outcome_observation_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_retry_outcome_observation.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_retry_outcome_observation.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_retry_outcome_observation.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_retry_outcome_observation.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_retry_outcome_observation.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "session_retry_outcome_observation.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_RESULT_H
#error "session_retry_outcome_observation.h must not include retry execution result headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "session_retry_outcome_observation.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "session_retry_outcome_observation.h must not include retry policy headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_retry_outcome_observation.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_retry_outcome_observation.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_retry_outcome_observation.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_retry_outcome_observation.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_retry_outcome_observation.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_retry_outcome_observation.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_OBS_OK == 0,
                   "retry outcome observation result enum drift: OK");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_OBS_NOT_READY == 1,
                   "retry outcome observation result enum drift: NOT_READY");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_OBS_FAILED == 2,
                   "retry outcome observation result enum drift: FAILED");

static session_retry_outcome_observation_result_t
    (*const check_session_observe_retry_outcome)(
        const struct session *,
        session_retry_outcome_state_t *) = session_observe_retry_outcome;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    session_retry_outcome_state_t outcome = SESSION_RETRY_OUTCOME_UNSET;

    (void)check_session_observe_retry_outcome;
    (void)outcome;
    (void)SESSION_RETRY_OUTCOME_OBS_OK;
    (void)SESSION_RETRY_OUTCOME_OBS_NOT_READY;
    (void)SESSION_RETRY_OUTCOME_OBS_FAILED;
}
