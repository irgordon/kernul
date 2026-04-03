/*
 * tests/console/test_session_ownership_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/session_ownership.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_ownership.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SESSION_TERMINAL_STATE_H
#error "session_ownership.h must not include terminal state headers"
#endif

#ifdef KERNUL_SESSION_FINALIZATION_H
#error "session_ownership.h must not include finalization headers"
#endif

#ifdef KERNUL_SCHED_H
#error "session_ownership.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_ownership.h must not include execution headers"
#endif

KERN_STATIC_ASSERT(SESSION_OWNERSHIP_REGISTER_OK == 0,
                   "ownership result enum drift: OK");
KERN_STATIC_ASSERT(SESSION_OWNERSHIP_REGISTER_ALREADY_PRESENT == 1,
                   "ownership result enum drift: ALREADY_PRESENT");
KERN_STATIC_ASSERT(SESSION_OWNERSHIP_REGISTER_CAPACITY_EXCEEDED == 2,
                   "ownership result enum drift: CAPACITY_EXCEEDED");
KERN_STATIC_ASSERT(SESSION_OWNERSHIP_FIXED_CAPACITY > 0U,
                   "ownership capacity must be positive");

static session_ownership_register_result_t
    (*const check_session_register_resource)(
        struct session *,
        resource_id_t) = session_register_resource;

static bool (*const check_session_owns_resource_acquire)(
    const struct session *,
    resource_id_t) = session_owns_resource_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_register_resource;
    (void)check_session_owns_resource_acquire;
    (void)SESSION_OWNERSHIP_FIXED_CAPACITY;
}
