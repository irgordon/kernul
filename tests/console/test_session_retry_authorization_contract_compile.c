/*
 * tests/console/test_session_retry_authorization_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_retry_authorization.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_retry_authorization.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_retry_authorization.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_retry_authorization.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_retry_authorization.h must not include scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_retry_authorization.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_retry_authorization.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_retry_authorization.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_retry_authorization.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_retry_authorization.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_retry_authorization.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_NONE == 0,
                   "retry authorization enum value drift: NONE");
KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_GRANTED == 1,
                   "retry authorization enum value drift: GRANTED");
KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_CONSUMED == 2,
                   "retry authorization enum value drift: CONSUMED");

KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_OK == 0,
                   "retry authorization result enum value drift: OK");
KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_NOT_READY == 1,
                   "retry authorization result enum value drift: NOT_READY");
KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_POLICY_FORBIDS == 2,
                   "retry authorization result enum value drift: POLICY_FORBIDS");
KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_ALREADY_GRANTED == 3,
                   "retry authorization result enum value drift: ALREADY_GRANTED");
KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_NOT_GRANTED == 4,
                   "retry authorization result enum value drift: NOT_GRANTED");
KERN_STATIC_ASSERT(SESSION_RETRY_AUTH_FAILED == 5,
                   "retry authorization result enum value drift: FAILED");

static session_retry_authorization_result_t
    (*const check_session_grant_retry_authorization)(
        struct session *) = session_grant_retry_authorization;

static session_retry_authorization_result_t
    (*const check_session_consume_retry_authorization)(
        struct session *) = session_consume_retry_authorization;

static session_retry_authorization_state_t
    (*const check_session_get_retry_authorization_acquire)(
        const struct session *) = session_get_retry_authorization_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_grant_retry_authorization;
    (void)check_session_consume_retry_authorization;
    (void)check_session_get_retry_authorization_acquire;
    (void)SESSION_RETRY_AUTH_NONE;
    (void)SESSION_RETRY_AUTH_GRANTED;
    (void)SESSION_RETRY_AUTH_CONSUMED;
    (void)SESSION_RETRY_AUTH_OK;
    (void)SESSION_RETRY_AUTH_NOT_READY;
    (void)SESSION_RETRY_AUTH_POLICY_FORBIDS;
    (void)SESSION_RETRY_AUTH_ALREADY_GRANTED;
    (void)SESSION_RETRY_AUTH_NOT_GRANTED;
    (void)SESSION_RETRY_AUTH_FAILED;
}
