/*
 * tests/console/test_session_retry_policy_storage_include_ban.c
 *
 * Compile-time include ban verification for
 * core/session_retry_policy_storage.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/session_retry_policy_storage.c"

#ifdef KERNUL_SESSION_FINALIZATION_H
#error "retry policy storage must not include finalization headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "retry policy storage must not include reclamation headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_ELIGIBILITY_H
#error "retry policy storage must not include recovery eligibility headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "retry policy storage must not include recovery authorization headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "retry policy storage must not include recovery execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "retry policy storage must not include interactive execution headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)&session_retry_policy_state_acquire;
    (void)&session_retry_policy_try_publish_release;
}
