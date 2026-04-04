/*
 * tests/console/test_session_retry_execution_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_retry_execution.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_retry_execution.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_retry_execution.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_retry_execution.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_retry_execution.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "session_retry_execution.h must not include retry policy headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_retry_execution.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_retry_execution.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_retry_execution.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_retry_execution.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_retry_execution.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_retry_execution.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(SESSION_RETRY_EXEC_OK == 0,
                   "retry execution enum value drift: OK");
KERN_STATIC_ASSERT(SESSION_RETRY_EXEC_NOT_READY == 1,
                   "retry execution enum value drift: NOT_READY");
KERN_STATIC_ASSERT(SESSION_RETRY_EXEC_NOT_AUTHORIZED == 2,
                   "retry execution enum value drift: NOT_AUTHORIZED");
KERN_STATIC_ASSERT(SESSION_RETRY_EXEC_FAILED == 3,
                   "retry execution enum value drift: FAILED");

static session_retry_execution_result_t
    (*const check_session_execute_retry)(
        struct session *) = session_execute_retry;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_execute_retry;
    (void)SESSION_RETRY_EXEC_OK;
    (void)SESSION_RETRY_EXEC_NOT_READY;
    (void)SESSION_RETRY_EXEC_NOT_AUTHORIZED;
    (void)SESSION_RETRY_EXEC_FAILED;
}
