/*
 * core/session.c
 *
 * Session and process-group identity container stubs.
 *
 * This file provides deterministic bounded creation and identity accessors.
 * No signal behavior, no terminal behavior, no job-control behavior, and no
 * scheduler behavior are implemented here.
 */

#include <kernul/session.h>

static struct session session_slot;
static struct process_group process_group_slot;
static u32 session_slot_live;
static u32 process_group_slot_live;
static session_id_t next_session_id = 1U;
static process_group_id_t next_process_group_id = 1U;

struct session *session_create(struct process *leader)
{
    u32 expected = 0U;
    session_id_t id;

    if (leader == NULL)
        return NULL;

    if (!__atomic_compare_exchange_n(&session_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    id = __atomic_fetch_add(&next_session_id, 1U, __ATOMIC_RELAXED);
    if (id == 0U)
        id = __atomic_fetch_add(&next_session_id, 1U, __ATOMIC_RELAXED);

    session_slot.id = id;
    session_slot.leader = leader;
    session_slot.scheduler_state.session = NULL;
    session_slot.scheduler_state.runnable = NULL;
    session_slot.scheduler_state.state = 0U;
    __atomic_store_n(&session_slot.scheduler_state_live, 0U, __ATOMIC_RELEASE);
    session_slot.scheduler_container.session = NULL;
    session_slot.scheduler_container.scheduler_state = NULL;
    session_slot.scheduler_container.runnable = NULL;
    __atomic_store_n(&session_slot.scheduler_container_live, 0U,
                     __ATOMIC_RELEASE);
    session_slot.execution_handoff.session = NULL;
    session_slot.execution_handoff.selection = NULL;
    session_slot.execution_handoff.operand_identity = NULL;
    __atomic_store_n(&session_slot.execution_handoff_live, 0U,
                     __ATOMIC_RELEASE);
    session_slot.execution_transfer_operands_view.session = NULL;
    session_slot.execution_transfer_operands_view.handoff = NULL;
    session_slot.execution_transfer_operands_view.operand_identity = NULL;
    __atomic_store_n(&session_slot.execution_transfer_operands_view_live,
                     0U,
                     __ATOMIC_RELEASE);
    session_slot.execution_outcome_record.kind =
        INTERACTIVE_EXECUTION_OUTCOME_FAILED;
    __atomic_store_n(&session_slot.execution_outcome_record_live,
                     0U,
                     __ATOMIC_RELEASE);
    session_slot.execution_failure_ack_view.published = 0U;
    __atomic_store_n(&session_slot.execution_failure_ack_view_live,
                     0U,
                     __ATOMIC_RELEASE);
    session_slot.execution_completion_ack_view.published = 0U;
    __atomic_store_n(&session_slot.execution_completion_ack_view_live,
                     0U,
                     __ATOMIC_RELEASE);

    return &session_slot;
}

struct process_group *process_group_create(struct session *session,
                                           struct process *leader)
{
    u32 expected = 0U;
    process_group_id_t id;

    if (session == NULL || leader == NULL)
        return NULL;

    if (!__atomic_compare_exchange_n(&process_group_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    id = __atomic_fetch_add(&next_process_group_id, 1U, __ATOMIC_RELAXED);
    if (id == 0U)
        id = __atomic_fetch_add(&next_process_group_id, 1U, __ATOMIC_RELAXED);

    process_group_slot.id = id;
    process_group_slot.session = session;
    process_group_slot.leader = leader;

    return &process_group_slot;
}

session_id_t session_id(const struct session *s)
{
    if (s == NULL)
        return 0U;

    return s->id;
}

process_group_id_t process_group_id(const struct process_group *pg)
{
    if (pg == NULL)
        return 0U;

    return pg->id;
}
