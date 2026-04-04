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
#include <kernul/session_recovery_execution.h>

static struct session session_slot;
static struct process_group process_group_slot;
static u32 session_slot_live;
static u32 process_group_slot_live;
static session_id_t next_session_id = 1U;
static process_group_id_t next_process_group_id = 1U;

void
session_publish_ready_release(struct session *session)
{
    if (session == NULL)
        return;

    __atomic_store_n(&session->ready_published, 1U, __ATOMIC_RELEASE);
}

bool
session_is_ready_acquire(const struct session *session)
{
    if (session == NULL)
        return false;

    return __atomic_load_n(&session->ready_published, __ATOMIC_ACQUIRE) == 1U;
}

struct session *session_create(struct process *leader)
{
    u32 expected = 0U;
    u32 ownership_entry_index;
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
    __atomic_store_n(&session_slot.execution_initiation_permitted,
                     1U,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.terminal_state_published,
                     0U,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.finalized_published, 0U, __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.recovery_eligibility,
                     0U,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.recovery_authorization_state,
                     0U,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.recovery_execution_completed,
                     0U,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.recovery_execution_result,
                     (u32)SESSION_RECOVERY_EXEC_FAILED,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.recovery_outcome_state,
                     (u32)SESSION_RECOVERY_NOT_ATTEMPTED,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.ready_published, 0U, __ATOMIC_RELAXED);
    __atomic_store_n(&session_slot.terminal_cause,
                     SESSION_TERMINAL_CAUSE_UNSPECIFIED,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session_slot.ownership.lock, 0U, __ATOMIC_RELAXED);
    for (ownership_entry_index = 0U;
         ownership_entry_index < SESSION_OWNERSHIP_FIXED_CAPACITY;
         ++ownership_entry_index) {
        __atomic_store_n(&session_slot.ownership.entries[ownership_entry_index],
                         0U,
                         __ATOMIC_RELAXED);
    }
    __atomic_store_n(&session_slot.ownership.count, 0U, __ATOMIC_RELEASE);
    session_publish_ready_release(&session_slot);

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
    if (!session_is_ready_acquire(s))
        return 0U;

    return s->id;
}

process_group_id_t process_group_id(const struct process_group *pg)
{
    if (pg == NULL)
        return 0U;

    return pg->id;
}
