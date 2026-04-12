/*
 * tests/console/test_lifecycle_audit_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/lifecycle_audit.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_audit.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle_audit.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_audit.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_audit.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_audit.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle_audit.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle_audit.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle_audit.h must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_H
#error "lifecycle_audit.h must not include retry outcome headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_OBSERVATION_H
#error "lifecycle_audit.h must not include retry observation headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle_audit.h must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle_audit.h must not include recovery authorization headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_H
#error "lifecycle_audit.h must not include recovery outcome headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_OBSERVE_H
#error "lifecycle_audit.h must not include recovery observation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle_audit.h must not include signaling headers"
#endif

#ifdef KERNUL_LIFECYCLE_DOCUMENTATION_H
#error "lifecycle_audit.h must not include documentation synthesis headers"
#endif

#ifdef KERNUL_LIFECYCLE_FREEZE_ENFORCEMENT_H
#error "lifecycle_audit.h must not include freeze enforcement headers"
#endif

KERN_STATIC_ASSERT(LIFECYCLE_AUDIT_OK == 0, "lifecycle audit enum drift: OK");
KERN_STATIC_ASSERT(LIFECYCLE_AUDIT_FAILED == 1, "lifecycle audit enum drift: FAILED");

static lifecycle_audit_result_t
    (*const check_lifecycle_generate_audit_report)(
        const lifecycle_snapshot_t *,
        lifecycle_verify_result_t,
        const lifecycle_provenance_t *,
        lifecycle_assert_result_t,
        lifecycle_audit_report_t *) = lifecycle_generate_audit_report;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_audit_report_t report = {0};

    (void)check_lifecycle_generate_audit_report;
    (void)report.snapshot.ready;
    (void)report.verify_result;
    (void)report.provenance.readiness_timestamp;
    (void)report.assertion;
    (void)report.freeze_version;

    (void)LIFECYCLE_AUDIT_OK;
    (void)LIFECYCLE_AUDIT_FAILED;
}
