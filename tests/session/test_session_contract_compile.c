/*
 * tests/session/test_session_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/session.h.
 * Pass condition: successful compilation.
 */

#include <kernul/session.h>
#include <kernul/compiler.h>

static struct session *(*const check_session_create)(struct process *) =
    session_create;
static struct process_group *(*const check_process_group_create)(
    struct session *, struct process *) = process_group_create;
static session_id_t (*const check_session_id)(const struct session *) =
    session_id;
static process_group_id_t (*const check_process_group_id)(
    const struct process_group *) = process_group_id;
static void (*const check_session_publish_ready_release)(
    struct session *) = session_publish_ready_release;
static bool (*const check_session_is_ready_acquire)(
    const struct session *) = session_is_ready_acquire;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct session s = {0};
    struct process_group pg = {0};

    (void)check_session_create;
    (void)check_process_group_create;
    (void)check_session_id;
    (void)check_process_group_id;
    (void)check_session_publish_ready_release;
    (void)check_session_is_ready_acquire;

    (void)s.id;
    (void)s.leader;
    (void)s.execution_handoff;
    (void)s.execution_handoff_live;
    (void)s.execution_transfer_operands_view;
    (void)s.execution_transfer_operands_view_live;
    (void)s.execution_outcome_record;
    (void)s.execution_outcome_record_live;
    (void)s.execution_failure_ack_view;
    (void)s.execution_failure_ack_view_live;
    (void)s.execution_completion_ack_view;
    (void)s.execution_completion_ack_view_live;
    (void)s.execution_initiation_permitted;
    (void)s.finalized_published;
    (void)s.ready_published;
    (void)s.ownership;

    (void)pg.id;
    (void)pg.session;
    (void)pg.leader;
}
