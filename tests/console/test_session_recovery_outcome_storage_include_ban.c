/*
 * tests/console/test_session_recovery_outcome_storage_include_ban.c
 *
 * Compile-time include ban verification for
 * core/session_recovery_outcome_storage.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/session_recovery_outcome_storage.c"

#ifdef KERNUL_SESSION_FINALIZATION_H
#error "outcome storage must not include finalization headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "outcome storage must not include reclamation headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_ELIGIBILITY_H
#error "outcome storage must not include recovery eligibility headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "outcome storage must not include recovery authorization headers"
#endif

#ifdef KERNUL_SCHED_H
#error "outcome storage must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "outcome storage must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "outcome storage must not include scheduler container headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)&session_recovery_outcome_state_acquire;
    (void)&session_recovery_outcome_try_publish_release;
}
