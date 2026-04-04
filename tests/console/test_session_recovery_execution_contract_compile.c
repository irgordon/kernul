/*
 * tests/console/test_session_recovery_execution_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_recovery_execution.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_recovery_execution.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_recovery_execution.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_recovery_execution.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_recovery_execution.h must not include scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_recovery_execution.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_recovery_execution.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_recovery_execution.h must not include execution failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_recovery_execution.h must not include execution completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_recovery_execution.h must not include reclamation headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "session_recovery_execution.h must not include signaling headers"
#endif

KERN_STATIC_ASSERT(SESSION_RECOVERY_EXEC_OK == 0,
                   "execution result enum value drift: OK");
KERN_STATIC_ASSERT(SESSION_RECOVERY_EXEC_NOT_FINALIZED == 1,
                   "execution result enum value drift: NOT_FINALIZED");
KERN_STATIC_ASSERT(SESSION_RECOVERY_EXEC_NOT_AUTHORIZED == 2,
                   "execution result enum value drift: NOT_AUTHORIZED");
KERN_STATIC_ASSERT(SESSION_RECOVERY_EXEC_FAILED == 3,
                   "execution result enum value drift: FAILED");

static session_recovery_execution_result_t
    (*const check_session_execute_recovery)(
        struct session *) = session_execute_recovery;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_execute_recovery;
    (void)SESSION_RECOVERY_EXEC_OK;
    (void)SESSION_RECOVERY_EXEC_NOT_FINALIZED;
    (void)SESSION_RECOVERY_EXEC_NOT_AUTHORIZED;
    (void)SESSION_RECOVERY_EXEC_FAILED;
}
