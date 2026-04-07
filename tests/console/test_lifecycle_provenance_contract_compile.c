/*
 * tests/console/test_lifecycle_provenance_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_provenance.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_provenance.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle_provenance.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_provenance.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_provenance.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_provenance.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle_provenance.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle_provenance.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle_provenance.h must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle_provenance.h must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle_provenance.h must not include recovery authorization headers"
#endif

#ifdef KERNUL_LIFECYCLE_VERIFICATION_H
#error "lifecycle_provenance.h must not include verification headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle_provenance.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(LIFECYCLE_PROVENANCE_OK == 0,
                   "lifecycle provenance enum drift: OK");
KERN_STATIC_ASSERT(LIFECYCLE_PROVENANCE_NOT_READY == 1,
                   "lifecycle provenance enum drift: NOT_READY");
KERN_STATIC_ASSERT(LIFECYCLE_PROVENANCE_FAILED == 2,
                   "lifecycle provenance enum drift: FAILED");

static int
    (*const check_lifecycle_get_provenance)(
        const struct session *,
        lifecycle_provenance_t *) = lifecycle_get_provenance;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_provenance_t provenance = {0};

    (void)check_lifecycle_get_provenance;

    (void)provenance.finalization_timestamp;
    (void)provenance.readiness_timestamp;
    (void)provenance.ownership_timestamp;
    (void)provenance.reclamation_timestamp;
    (void)provenance.recovery_eligibility_timestamp;
    (void)provenance.recovery_authorization_timestamp;
    (void)provenance.recovery_execution_timestamp;
    (void)provenance.recovery_outcome_timestamp;
    (void)provenance.retry_policy_timestamp;
    (void)provenance.retry_authorization_timestamp;
    (void)provenance.retry_execution_timestamp;
    (void)provenance.retry_outcome_timestamp;

    (void)LIFECYCLE_PROVENANCE_OK;
    (void)LIFECYCLE_PROVENANCE_NOT_READY;
    (void)LIFECYCLE_PROVENANCE_FAILED;
}
