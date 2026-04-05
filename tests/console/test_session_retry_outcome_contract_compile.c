/*
 * tests/console/test_session_retry_outcome_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_retry_outcome.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_retry_outcome.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_retry_outcome.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_retry_outcome.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_retry_outcome.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "session_retry_outcome.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "session_retry_outcome.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "session_retry_outcome.h must not include retry policy headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_retry_outcome.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_retry_outcome.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_retry_outcome.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_retry_outcome.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_retry_outcome.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_retry_outcome.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_UNSET == 0,
                   "retry outcome enum value drift: UNSET");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_SUCCESS == 1,
                   "retry outcome enum value drift: SUCCESS");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_FAILURE == 2,
                   "retry outcome enum value drift: FAILURE");

KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_RECORD_OK == 0,
                   "retry outcome record result enum drift: OK");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_RECORD_NOT_READY == 1,
                   "retry outcome record result enum drift: NOT_READY");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_RECORD_ALREADY_SET == 2,
                   "retry outcome record result enum drift: ALREADY_SET");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_RECORD_NO_RESULT == 3,
                   "retry outcome record result enum drift: NO_RESULT");
KERN_STATIC_ASSERT(SESSION_RETRY_OUTCOME_RECORD_FAILED == 4,
                   "retry outcome record result enum drift: FAILED");

static session_retry_outcome_record_result_t
    (*const check_session_record_retry_outcome)(
        struct session *) = session_record_retry_outcome;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_record_retry_outcome;
    (void)SESSION_RETRY_OUTCOME_UNSET;
    (void)SESSION_RETRY_OUTCOME_SUCCESS;
    (void)SESSION_RETRY_OUTCOME_FAILURE;
    (void)SESSION_RETRY_OUTCOME_RECORD_OK;
    (void)SESSION_RETRY_OUTCOME_RECORD_NOT_READY;
    (void)SESSION_RETRY_OUTCOME_RECORD_ALREADY_SET;
    (void)SESSION_RETRY_OUTCOME_RECORD_NO_RESULT;
    (void)SESSION_RETRY_OUTCOME_RECORD_FAILED;
}
