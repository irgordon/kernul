/*
 * tests/console/test_lifecycle_freeze_enforcement_contract_compile.c
 *
 * Compile-check translation unit for lifecycle freeze enforcement headers.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_freeze_enforcement.h>
#include <kernul/lifecycle_freeze_baseline.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle freeze enforcement headers must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle freeze enforcement headers must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle freeze enforcement headers must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle freeze enforcement headers must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle freeze enforcement headers must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle freeze enforcement headers must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle freeze enforcement headers must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle freeze enforcement headers must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle freeze enforcement headers must not include recovery authorization headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle freeze enforcement headers must not include signaling headers"
#endif

#ifdef KERNUL_LIFECYCLE_VERIFICATION_H
#error "lifecycle freeze enforcement headers must not include verification headers"
#endif

#ifdef KERNUL_LIFECYCLE_ASSERTION_H
#error "lifecycle freeze enforcement headers must not include assertion headers"
#endif

LIFECYCLE_FREEZE_ASSERT(LIFECYCLE_FREEZE_VERSION == 1U,
                        "lifecycle freeze version drift");
LIFECYCLE_FREEZE_ASSERT(LIFECYCLE_STATE_SIZE == 48U,
                        "lifecycle state size baseline drift");
LIFECYCLE_FREEZE_ASSERT(LIFECYCLE_PROVENANCE_SIZE == 96U,
                        "lifecycle provenance size baseline drift");

static lifecycle_introspect_sig_t const check_lifecycle_introspect = lifecycle_introspect;
static lifecycle_get_provenance_sig_t const check_lifecycle_get_provenance =
    lifecycle_get_provenance;

static int lifecycle_frozen_annotation_probe LIFECYCLE_FROZEN KERN_UNUSED;
static void
lifecycle_frozen_fn_annotation_probe(void) LIFECYCLE_FROZEN_FN KERN_UNUSED;
static void
lifecycle_frozen_fn_annotation_probe(void)
{
}

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_snapshot_t snapshot = {0};
    lifecycle_provenance_t provenance = {0};

    (void)check_lifecycle_introspect;
    (void)check_lifecycle_get_provenance;
    (void)snapshot.finalized;
    (void)provenance.recovery_outcome_timestamp;
    (void)lifecycle_frozen_annotation_probe;
    (void)&lifecycle_frozen_fn_annotation_probe;
}
