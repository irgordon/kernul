/*
 * tests/console/test_lifecycle_assertion_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_assertion.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_assertion.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle_assertion.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_assertion.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_assertion.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_assertion.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle_assertion.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle_assertion.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle_assertion.h must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_H
#error "lifecycle_assertion.h must not include retry outcome headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_OBSERVATION_H
#error "lifecycle_assertion.h must not include retry observation headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle_assertion.h must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle_assertion.h must not include recovery authorization headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_H
#error "lifecycle_assertion.h must not include recovery outcome headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_OBSERVE_H
#error "lifecycle_assertion.h must not include recovery observation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle_assertion.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(LIFECYCLE_ASSERT_SATISFIED == 0,
                   "lifecycle assert enum drift: SATISFIED");
KERN_STATIC_ASSERT(LIFECYCLE_ASSERT_VIOLATED == 1,
                   "lifecycle assert enum drift: VIOLATED");
KERN_STATIC_ASSERT(LIFECYCLE_ASSERT_UNKNOWN == 2,
                   "lifecycle assert enum drift: UNKNOWN");

static lifecycle_assert_result_t
    (*const check_lifecycle_assert)(
        const lifecycle_snapshot_t *,
        lifecycle_verify_result_t,
        const lifecycle_provenance_t *,
        const lifecycle_boundary_t *) = lifecycle_assert;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_snapshot_t snapshot = {0};
    lifecycle_provenance_t provenance = {0};
    lifecycle_boundary_t boundary = {0};

    (void)check_lifecycle_assert;
    (void)snapshot.finalized;
    (void)provenance.recovery_outcome_timestamp;
    (void)boundary.require_retry_complete;

    (void)LIFECYCLE_ASSERT_SATISFIED;
    (void)LIFECYCLE_ASSERT_VIOLATED;
    (void)LIFECYCLE_ASSERT_UNKNOWN;
}
