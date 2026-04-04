/*
 * tests/console/test_session_recovery_authorization_include_ban.c
 *
 * Compile-time include ban verification for
 * core/session_recovery_authorization.c.
 * Pass condition: successful compilation with no forbidden header visibility.
 */

#include <kernul/compiler.h>
#include "../../core/session_recovery_authorization.c"

#ifdef KERNUL_SCHED_H
#error "session_recovery_authorization include path must not expose scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_recovery_authorization include path must not expose scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_recovery_authorization include path must not expose scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_recovery_authorization include path must not expose execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_recovery_authorization include path must not expose execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_recovery_authorization include path must not expose execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_recovery_authorization include path must not expose execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_recovery_authorization include path must not expose reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_recovery_authorization include path must not expose signaling headers"
#endif

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)&session_authorize_recovery_attempt;
    (void)&session_consume_recovery_authorization;
    (void)&session_is_recovery_authorized_acquire;
}
