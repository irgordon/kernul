/*
 * tests/console/test_interactive_execution_failure_ack_gate_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/interactive_execution_failure_ack_gate.h.
 * Pass condition: successful compilation.
 */

#include <kernul/interactive_execution_failure_ack_gate.h>
#include <kernul/compiler.h>
#include <kernul/session.h>

static void (*const check_interactive_execution_failure_ack_publish_if_failed)(
    struct session *) = interactive_execution_failure_ack_publish_if_failed;

static const struct interactive_execution_failure_ack_view *(*const
    check_interactive_execution_failure_ack_acquire)(
    struct session *) = interactive_execution_failure_ack_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct interactive_execution_failure_ack_view view = {0};
    struct session session = {0};

    (void)check_interactive_execution_failure_ack_publish_if_failed;
    (void)check_interactive_execution_failure_ack_acquire;

    (void)view.published;
    (void)session.execution_failure_ack_view;
    (void)session.execution_failure_ack_view_live;
}
