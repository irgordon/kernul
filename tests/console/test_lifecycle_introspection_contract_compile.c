/*
 * tests/console/test_lifecycle_introspection_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_introspection.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_introspection.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "lifecycle_introspection.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_introspection.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_introspection.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle_introspection.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle_introspection.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle_introspection.h must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle_introspection.h must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle_introspection.h must not include recovery authorization headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle_introspection.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(LIFECYCLE_INTROSPECT_OK == 0,
                   "lifecycle introspection enum drift: OK");
KERN_STATIC_ASSERT(LIFECYCLE_INTROSPECT_NOT_READY == 1,
                   "lifecycle introspection enum drift: NOT_READY");
KERN_STATIC_ASSERT(LIFECYCLE_INTROSPECT_FAILED == 2,
                   "lifecycle introspection enum drift: FAILED");

static lifecycle_introspect_result_t
    (*const check_lifecycle_introspect)(
        const struct session *,
        lifecycle_snapshot_t *) = lifecycle_introspect;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_snapshot_t snapshot = {0};

    (void)check_lifecycle_introspect;

    (void)snapshot.finalized;
    (void)snapshot.ready;
    (void)snapshot.owner_id;
    (void)snapshot.reclaimed;
    (void)snapshot.recovery_eligible;
    (void)snapshot.recovery_authorized;
    (void)snapshot.recovery_executed;
    (void)snapshot.recovery_execution_result;
    (void)snapshot.recovery_outcome;
    (void)snapshot.retry_policy;
    (void)snapshot.retry_authorized;
    (void)snapshot.retry_executed;
    (void)snapshot.retry_execution_result;
    (void)snapshot.retry_outcome;

    (void)LIFECYCLE_INTROSPECT_OK;
    (void)LIFECYCLE_INTROSPECT_NOT_READY;
    (void)LIFECYCLE_INTROSPECT_FAILED;
}
