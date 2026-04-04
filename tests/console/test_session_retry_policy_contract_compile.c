/*
 * tests/console/test_session_retry_policy_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_retry_policy.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_retry_policy.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_retry_policy.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_retry_policy.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_retry_policy.h must not include scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_retry_policy.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_retry_policy.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_retry_policy.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_retry_policy.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_retry_policy.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_retry_policy.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(SESSION_RETRY_UNSET == 0,
                   "retry policy enum value drift: UNSET");
KERN_STATIC_ASSERT(SESSION_RETRY_ALLOWED == 1,
                   "retry policy enum value drift: ALLOWED");
KERN_STATIC_ASSERT(SESSION_RETRY_FORBIDDEN == 2,
                   "retry policy enum value drift: FORBIDDEN");

KERN_STATIC_ASSERT(SESSION_RETRY_POLICY_OK == 0,
                   "retry policy result enum value drift: OK");
KERN_STATIC_ASSERT(SESSION_RETRY_POLICY_NOT_READY == 1,
                   "retry policy result enum value drift: NOT_READY");
KERN_STATIC_ASSERT(SESSION_RETRY_POLICY_ALREADY_DECLARED == 2,
                   "retry policy result enum value drift: ALREADY_DECLARED");
KERN_STATIC_ASSERT(SESSION_RETRY_POLICY_FAILED == 3,
                   "retry policy result enum value drift: FAILED");

static session_retry_policy_result_t
    (*const check_session_declare_retry_policy)(
        struct session *,
        session_retry_policy_t) = session_declare_retry_policy;

static session_retry_policy_t
    (*const check_session_get_retry_policy_acquire)(
        const struct session *) = session_get_retry_policy_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_declare_retry_policy;
    (void)check_session_get_retry_policy_acquire;
    (void)SESSION_RETRY_UNSET;
    (void)SESSION_RETRY_ALLOWED;
    (void)SESSION_RETRY_FORBIDDEN;
    (void)SESSION_RETRY_POLICY_OK;
    (void)SESSION_RETRY_POLICY_NOT_READY;
    (void)SESSION_RETRY_POLICY_ALREADY_DECLARED;
    (void)SESSION_RETRY_POLICY_FAILED;
}
