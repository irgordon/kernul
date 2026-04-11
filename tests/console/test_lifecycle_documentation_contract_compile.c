/*
 * tests/console/test_lifecycle_documentation_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_documentation.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_documentation.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_H
#error "lifecycle_documentation.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_documentation.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_STATE_H
#error "lifecycle_documentation.h must not include scheduler state headers"
#endif

#ifdef KERNUL_INTERACTIVE_SCHEDULER_CONTAINER_H
#error "lifecycle_documentation.h must not include scheduler container headers"
#endif

#ifdef KERNUL_SESSION_RETRY_EXECUTION_H
#error "lifecycle_documentation.h must not include retry execution headers"
#endif

#ifdef KERNUL_SESSION_RETRY_AUTHORIZATION_H
#error "lifecycle_documentation.h must not include retry authorization headers"
#endif

#ifdef KERNUL_SESSION_RETRY_POLICY_H
#error "lifecycle_documentation.h must not include retry policy headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_EXECUTION_H
#error "lifecycle_documentation.h must not include recovery execution headers"
#endif

#ifdef KERNUL_SESSION_RECOVERY_AUTHORIZATION_H
#error "lifecycle_documentation.h must not include recovery authorization headers"
#endif

#ifdef KERNUL_SIGNAL_H
#error "lifecycle_documentation.h must not include signaling headers"
#endif

#ifdef KERNUL_LIFECYCLE_FREEZE_ENFORCEMENT_H
#error "lifecycle_documentation.h must not include freeze enforcement headers"
#endif

KERN_STATIC_ASSERT(LIFECYCLE_DOC_OK == 0, "lifecycle doc enum drift: OK");
KERN_STATIC_ASSERT(LIFECYCLE_DOC_FAILED == 1, "lifecycle doc enum drift: FAILED");

static lifecycle_doc_result_t
    (*const check_lifecycle_generate_documentation)(
        lifecycle_documentation_t *) = lifecycle_generate_documentation;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_documentation_t doc = {0};

    (void)check_lifecycle_generate_documentation;
    (void)doc.buffer;
    (void)doc.capacity;
    (void)doc.length;

    (void)LIFECYCLE_DOC_OK;
    (void)LIFECYCLE_DOC_FAILED;
}
