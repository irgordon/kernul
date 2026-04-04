#ifndef KERNUL_SESSION_RECOVERY_EXECUTION_STORAGE_H
#define KERNUL_SESSION_RECOVERY_EXECUTION_STORAGE_H

/*
 * core/session_recovery_execution_storage.h
 *
 * Mechanical-only storage helpers for recovery execution publication.
 *
 * Completion/result ordering contract (normative):
 *   - session_publish_recovery_execution_completion_release() stores result,
 *     then release-publishes completion.
 *   - session_recovery_execution_completed_acquire() acquire-observes
 *     completion.
 *   - After acquire observation of completion, the execution result is stable
 *     (immutable) and fully visible via
 *     session_recovery_execution_result_acquire().
 *
 * Scope restrictions:
 *   These helpers provide load/store/CAS-style ordering primitives only.
 *   They must not implement lifecycle policy, authorization/finalization
 *   checks, retries, scheduling, coordination, or semantic convenience APIs.
 */

#include <kernul/types.h>
#include <kernul/session_recovery_execution.h>

struct session;

void
session_publish_recovery_execution_completion_release(
    struct session *session,
    session_recovery_execution_result_t result);

bool
session_recovery_execution_completed_acquire(const struct session *session);

session_recovery_execution_result_t
session_recovery_execution_result_acquire(const struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_EXECUTION_STORAGE_H */
