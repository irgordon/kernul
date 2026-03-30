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

struct process;
struct interactive_runnable;

struct interactive_scheduler_state {
    struct session *session;
    struct interactive_runnable *runnable;
    u32 state;
};

typedef u32 session_id_t;
typedef u32 process_group_id_t;

struct session {
    session_id_t     id;
    struct process  *leader;
    struct interactive_scheduler_state scheduler_state;
    u32 scheduler_state_live;
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
