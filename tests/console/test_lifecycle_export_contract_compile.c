/*
 * tests/console/test_lifecycle_export_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/lifecycle_export.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_export.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle_export.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_export.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_export.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_export.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle_export.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle_export.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle_export.h must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_H
#error "lifecycle_export.h must not include retry outcome headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_OBSERVATION_H
#error "lifecycle_export.h must not include retry observation headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle_export.h must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle_export.h must not include recovery authorization headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_H
#error "lifecycle_export.h must not include recovery outcome headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_OBSERVE_H
#error "lifecycle_export.h must not include recovery observation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle_export.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(LIFECYCLE_EXPORT_OK == 0, "lifecycle export enum drift: OK");
KERN_STATIC_ASSERT(LIFECYCLE_EXPORT_FAILED == 1, "lifecycle export enum drift: FAILED");

static lifecycle_export_result_t
    (*const check_lifecycle_serialize_audit_report)(
        const lifecycle_audit_report_t *,
        lifecycle_export_buffer_t *) = lifecycle_serialize_audit_report;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_export_buffer_t out_export = {0};

    (void)check_lifecycle_serialize_audit_report;
    (void)out_export.buffer;
    (void)out_export.capacity;
    (void)out_export.length;
    (void)LIFECYCLE_EXPORT_OK;
    (void)LIFECYCLE_EXPORT_FAILED;
}
