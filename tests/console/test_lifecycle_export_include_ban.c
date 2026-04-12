/*
 * tests/console/test_lifecycle_export_include_ban.c
 *
 * Compile-time include ban verification for core/lifecycle_export.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/lifecycle_export.c"

#ifdef KERNUL_SESSION_H
#error "lifecycle export include path must not expose session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle export include path must not expose scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle export include path must not expose scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle export include path must not expose scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "lifecycle export include path must not expose execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TARGET_H
#error "lifecycle export include path must not expose execution target headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle export include path must not expose retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle export include path must not expose retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle export include path must not expose retry policy headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_H
#error "lifecycle export include path must not expose retry outcome headers"
#endif

#ifdef KERNUL_SESSION_RETRY_OUTCOME_OBSERVATION_H
#error "lifecycle export include path must not expose retry observation headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle export include path must not expose recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle export include path must not expose recovery authorization headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_H
#error "lifecycle export include path must not expose recovery outcome headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_OUTCOME_OBSERVE_H
#error "lifecycle export include path must not expose recovery observation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle export include path must not expose signaling headers"
#endif

#ifdef KERNUL_LIFECYCLE_DOCUMENTATION_H
#error "lifecycle export include path must not expose documentation synthesis headers"
#endif

#ifdef KERNUL_LIFECYCLE_FREEZE_ENFORCEMENT_H
#error "lifecycle export include path must not expose freeze enforcement headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)&lifecycle_serialize_audit_report;
}
