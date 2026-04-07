/*
 * tests/console/test_lifecycle_verification_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_verification.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_verification.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle_verification.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_verification.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_verification.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_verification.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle_verification.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle_verification.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle_verification.h must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle_verification.h must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle_verification.h must not include recovery authorization headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle_verification.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(LIFECYCLE_VERIFY_VERIFIED == 0,
                   "lifecycle verify enum drift: VERIFIED");
KERN_STATIC_ASSERT(LIFECYCLE_VERIFY_VIOLATION == 1,
                   "lifecycle verify enum drift: VIOLATION");
KERN_STATIC_ASSERT(LIFECYCLE_VERIFY_UNKNOWN == 2,
                   "lifecycle verify enum drift: UNKNOWN");

static lifecycle_verify_result_t
    (*const check_lifecycle_verify)(const lifecycle_snapshot_t *) =
        lifecycle_verify;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_snapshot_t snapshot = {0};

    (void)check_lifecycle_verify;
    (void)snapshot.finalized;

    (void)LIFECYCLE_VERIFY_VERIFIED;
    (void)LIFECYCLE_VERIFY_VIOLATION;
    (void)LIFECYCLE_VERIFY_UNKNOWN;
}
