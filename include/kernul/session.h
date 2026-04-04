#ifndef KERNUL_SESSION_H
#define KERNUL_SESSION_H

/*
 * include/kernul/session.h
 *
 * Kernel-internal session and process-group contract. Phase 6 boundary.
 *
 * This header defines identity, ownership, and linkage vocabulary only.
 *
 * This is a kernel-internal contract.
 * This is not a userspace ABI surface.
 * No compatibility guarantees are implied by this contract.
 *
 * Identity rules:
 *   session_id_t and process_group_id_t are small integer identifiers only.
 *   No POSIX numbering guarantees are provided.
 *   No implicit relationship to process IDs is defined.
 *   No ID reuse policy is defined in this phase.
 *
 * Ownership and lifetime:
 *   struct session and struct process_group are identity containers only.
 *   They are not refcounted kernel objects.
 *   Lifetime is externally managed, typically by process lifecycle.
 *   All pointers in these structures are borrowed.
 *   No automatic cleanup or cascading behavior is defined.
 *
 * Scope boundary:
 *   No process-group membership tracking is required in this phase.
 *   No signal delivery or routing behavior is defined.
 *   No terminal association is defined.
 *   No foreground/background or job-control semantics are defined.
 *   No scheduler behavior is defined.
 */

#include <kernul/types.h>
#include <kernul/interactive_scheduler_container.h>
#include <kernul/interactive_selection_result.h>
#include <kernul/interactive_execution_handoff.h>
#include <kernul/interactive_execution_transfer_operands_view.h>
#include <kernul/interactive_execution_outcome_record.h>
#include <kernul/interactive_execution_failure_ack_gate.h>
#include <kernul/interactive_execution_completion_ack_gate.h>
#include <kernul/session_terminal_cause.h>
#include <kernul/session_ownership.h>

struct process;
struct interactive_runnable;
struct interactive_scheduler_state {
    struct session *session;
    struct interactive_runnable *runnable;
    u32 state;
};

struct session_owned_resource_registry {
    resource_id_t entries[SESSION_OWNERSHIP_FIXED_CAPACITY];
    u32 count;
    u32 lock;
};

typedef u32 session_id_t;
typedef u32 process_group_id_t;

struct session {
    session_id_t     id;
    struct process  *leader;
    struct interactive_scheduler_state scheduler_state;
    u32 scheduler_state_live;
    struct interactive_scheduler_container scheduler_container;
    u32 scheduler_container_live;
    struct interactive_execution_handoff execution_handoff;
    u32 execution_handoff_live;
    struct interactive_execution_transfer_operands_view
        execution_transfer_operands_view;
    u32 execution_transfer_operands_view_live;
    struct interactive_execution_outcome_record execution_outcome_record;
    u32 execution_outcome_record_live;
    struct interactive_execution_failure_ack_view execution_failure_ack_view;
    u32 execution_failure_ack_view_live;
    struct interactive_execution_completion_ack_view
        execution_completion_ack_view;
    u32 execution_completion_ack_view_live;
    u32 execution_initiation_permitted;
    u32 terminal_state_published;
    u32 finalized_published;
    u32 recovery_eligibility;
    u32 recovery_authorization_state;
    enum session_terminal_cause terminal_cause;
    struct session_owned_resource_registry ownership;
};

struct process_group {
    process_group_id_t  id;
    struct session     *session;
    struct process     *leader;
};

struct session *session_create(struct process *leader);
struct process_group *process_group_create(struct session *session,
                                           struct process *leader);

session_id_t session_id(const struct session *s);
process_group_id_t process_group_id(const struct process_group *pg);

#endif /* KERNUL_SESSION_H */
