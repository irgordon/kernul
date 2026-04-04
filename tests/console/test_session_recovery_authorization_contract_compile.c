/*
 * tests/console/test_session_recovery_authorization_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_recovery_authorization.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_recovery_authorization.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_recovery_authorization.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_recovery_authorization.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_recovery_authorization.h must not include scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_recovery_authorization.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_recovery_authorization.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_recovery_authorization.h must not include failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_recovery_authorization.h must not include completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_recovery_authorization.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_recovery_authorization.h must not include signal headers"
#endif

static session_recovery_authorization_result_t (*const
    check_session_authorize_recovery_attempt)(
    struct session *) = session_authorize_recovery_attempt;

static session_recovery_consume_result_t (*const
    check_session_consume_recovery_authorization)(
    struct session *) = session_consume_recovery_authorization;

static bool (*const check_session_is_recovery_authorized_acquire)(
    const struct session *) = session_is_recovery_authorized_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_authorize_recovery_attempt;
    (void)check_session_consume_recovery_authorization;
    (void)check_session_is_recovery_authorized_acquire;
}
