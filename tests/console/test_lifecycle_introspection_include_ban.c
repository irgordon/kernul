/*
 * tests/console/test_lifecycle_introspection_include_ban.c
 *
 * Compile-time include ban verification for core/lifecycle_introspection.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/lifecycle_introspection.c"

#ifdef KERNUL_SCHED_H
#error "lifecycle introspection include path must not expose scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle introspection include path must not expose scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle introspection include path must not expose scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle introspection include path must not expose retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle introspection include path must not expose retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle introspection include path must not expose retry policy headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle introspection include path must not expose recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle introspection include path must not expose recovery authorization headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle introspection include path must not expose signaling headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)&lifecycle_introspect;
}
