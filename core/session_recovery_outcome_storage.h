#ifndef KERNUL_SESSION_RECOVERY_OUTCOME_STORAGE_H
#define KERNUL_SESSION_RECOVERY_OUTCOME_STORAGE_H

/*
 * core/session_recovery_outcome_storage.h
 *
 * Mechanical-only storage helpers for immutable recovery outcome publication.
 *
 * Scope restrictions:
 *   These helpers provide storage ordering and CAS publication only.
 *   They must not implement lifecycle policy, execution authority checks,
 *   retries, scheduling, coordination, or semantic convenience APIs.
 */

#include <kernul/types.h>
#include <kernul/session_recovery_outcome.h>

struct session;

u32
session_recovery_outcome_state_acquire(const struct session *session);

bool
session_recovery_outcome_try_publish_release(
    struct session *session,
    session_recovery_outcome_t outcome);

u64
session_recovery_outcome_timestamp_acquire(const struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_OUTCOME_STORAGE_H */
