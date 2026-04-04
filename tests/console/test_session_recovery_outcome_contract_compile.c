/*
 * tests/console/test_session_recovery_outcome_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_recovery_outcome.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_recovery_outcome.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_recovery_outcome.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_recovery_outcome.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_recovery_outcome.h must not include scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_recovery_outcome.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_recovery_outcome.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_recovery_outcome.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_recovery_outcome.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_recovery_outcome.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_recovery_outcome.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(SESSION_RECOVERY_NOT_ATTEMPTED == 0,
                   "outcome enum value drift: NOT_ATTEMPTED");
KERN_STATIC_ASSERT(SESSION_RECOVERY_SUCCEEDED == 1,
                   "outcome enum value drift: SUCCEEDED");
KERN_STATIC_ASSERT(SESSION_RECOVERY_FAILED == 2,
                   "outcome enum value drift: FAILED");

KERN_STATIC_ASSERT(SESSION_RECOVERY_OUTCOME_OK == 0,
                   "outcome result enum value drift: OK");
KERN_STATIC_ASSERT(SESSION_RECOVERY_OUTCOME_NOT_READY == 1,
                   "outcome result enum value drift: NOT_READY");
KERN_STATIC_ASSERT(SESSION_RECOVERY_OUTCOME_ALREADY_RECORDED == 2,
                   "outcome result enum value drift: ALREADY_RECORDED");
KERN_STATIC_ASSERT(SESSION_RECOVERY_OUTCOME_FAILED == 3,
                   "outcome result enum value drift: FAILED");

static session_recovery_outcome_result_t
    (*const check_session_record_recovery_outcome)(
        struct session *,
        session_recovery_outcome_t) = session_record_recovery_outcome;

static session_recovery_outcome_t
    (*const check_session_get_recovery_outcome_acquire)(
        const struct session *) = session_get_recovery_outcome_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_record_recovery_outcome;
    (void)check_session_get_recovery_outcome_acquire;
    (void)SESSION_RECOVERY_NOT_ATTEMPTED;
    (void)SESSION_RECOVERY_SUCCEEDED;
    (void)SESSION_RECOVERY_FAILED;
    (void)SESSION_RECOVERY_OUTCOME_OK;
    (void)SESSION_RECOVERY_OUTCOME_NOT_READY;
    (void)SESSION_RECOVERY_OUTCOME_ALREADY_RECORDED;
    (void)SESSION_RECOVERY_OUTCOME_FAILED;
}
