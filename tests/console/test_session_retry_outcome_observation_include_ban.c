/*
 * tests/console/test_session_retry_outcome_observation_include_ban.c
 *
 * Compile-time include ban verification for
 * core/session_retry_outcome_observation.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/session_retry_outcome_observation.c"

#ifdef KERNUL_SCHED_H
#error "session_retry_outcome_observation include path must not expose scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_retry_outcome_observation include path must not expose scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_retry_outcome_observation include path must not expose scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "session_retry_outcome_observation include path must not expose retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_RESULT_H
#error "session_retry_outcome_observation include path must not expose retry execution result headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "session_retry_outcome_observation include path must not expose retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "session_retry_outcome_observation include path must not expose retry policy headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_retry_outcome_observation include path must not expose signaling headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)&session_observe_retry_outcome;
}
