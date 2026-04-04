/*
 * tests/console/test_session_recovery_eligibility_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_recovery_eligibility.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_recovery_eligibility.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_recovery_eligibility.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_recovery_eligibility.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_recovery_eligibility.h must not include scheduler container headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_recovery_eligibility.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_TRANSFER_INITIATION_GATE_H
#error "session_recovery_eligibility.h must not include execution transfer gate headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_recovery_eligibility.h must not include failure initiation headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_COMPLETION_INITIATION_GATE_H
#error "session_recovery_eligibility.h must not include completion initiation headers"
#endif

#ifdef KERNUL_SESSION_RECLAMATION_H
#error "session_recovery_eligibility.h must not include reclamation headers"
#endif

static session_recovery_eligibility_result_t (*const
    check_session_declare_recovery_eligibility)(
    struct session *,
    session_recovery_eligibility_t) =
    session_declare_recovery_eligibility;

static session_recovery_eligibility_t (*const
    check_session_get_recovery_eligibility_acquire)(
    const struct session *) =
    session_get_recovery_eligibility_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_declare_recovery_eligibility;
    (void)check_session_get_recovery_eligibility_acquire;
}
