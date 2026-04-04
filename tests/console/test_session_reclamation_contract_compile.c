/*
 * tests/console/test_session_reclamation_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_reclamation.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_reclamation.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_reclamation.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_reclamation.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "session_reclamation.h must not include scheduler-state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "session_reclamation.h must not include scheduler-container headers"
#endif

KERN_STATIC_ASSERT(SESSION_RECLAIM_OK == 0,
                   "reclaim result enum value drift: OK");
KERN_STATIC_ASSERT(SESSION_RECLAIM_NOT_FINALIZED == 1,
                   "reclaim result enum value drift: NOT_FINALIZED");
KERN_STATIC_ASSERT(SESSION_RECLAIM_NOT_OWNED == 2,
                   "reclaim result enum value drift: NOT_OWNED");
KERN_STATIC_ASSERT(SESSION_RECLAIM_FAILED == 3,
                   "reclaim result enum value drift: FAILED");

static session_reclaim_result_t
    (*const check_session_reclaim_resource_if_finalized)(
        struct session *,
        resource_id_t) = session_reclaim_resource_if_finalized;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_reclaim_resource_if_finalized;
    (void)SESSION_RECLAIM_OK;
    (void)SESSION_RECLAIM_NOT_FINALIZED;
    (void)SESSION_RECLAIM_NOT_OWNED;
    (void)SESSION_RECLAIM_FAILED;
}
