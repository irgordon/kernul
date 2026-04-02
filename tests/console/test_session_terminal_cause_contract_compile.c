/*
 * tests/console/test_session_terminal_cause_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/session_terminal_cause.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session_terminal_cause.h>
#include <kernul/compiler.h>

#ifdef KERNUL_SCHED_H
#error "session_terminal_cause.h must not include scheduler headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_H
#error "session_terminal_cause.h must not include execution headers"
#endif

#ifdef KERNUL_INTERACTIVE_EXECUTION_FAILURE_INITIATION_GATE_H
#error "session_terminal_cause.h must not include initiation gate headers"
#endif

KERN_STATIC_ASSERT(SESSION_TERMINAL_CAUSE_UNSPECIFIED == 0,
                   "terminal cause enum value drift: UNSPECIFIED");
KERN_STATIC_ASSERT(SESSION_TERMINAL_CAUSE_COMPLETED == 1,
                   "terminal cause enum value drift: COMPLETED");
KERN_STATIC_ASSERT(SESSION_TERMINAL_CAUSE_FAILED == 2,
                   "terminal cause enum value drift: FAILED");

static enum session_terminal_cause (*const check_session_get_terminal_cause_acquire)(
    const struct session *) = session_get_terminal_cause_acquire;

static void (*const check_session_publish_terminal_cause_if_terminal)(
    struct session *,
    enum session_terminal_cause) = session_publish_terminal_cause_if_terminal;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    (void)check_session_get_terminal_cause_acquire;
    (void)check_session_publish_terminal_cause_if_terminal;
    (void)SESSION_TERMINAL_CAUSE_UNSPECIFIED;
    (void)SESSION_TERMINAL_CAUSE_COMPLETED;
    (void)SESSION_TERMINAL_CAUSE_FAILED;
}
