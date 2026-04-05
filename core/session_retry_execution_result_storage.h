#ifndef KERNUL_SESSION_RETRY_EXECUTION_RESULT_STORAGE_H
#define KERNUL_SESSION_RETRY_EXECUTION_RESULT_STORAGE_H

/*
 * core/session_retry_execution_result_storage.h
 *
 * Mechanical-only storage helpers for retry execution result publication.
 *
 * Scope restrictions:
 *   These helpers provide load/store/CAS ordering primitives only.
 *   They must not implement readiness checks, authorization checks, policy
 *   logic, retries, scheduling, coordination, or semantic convenience APIs.
 */

#include <kernul/types.h>
#include <kernul/session_retry_execution_result.h>

struct session;

void
session_retry_execution_result_storage_init_release(struct session *session);

u32
session_retry_execution_result_state_load_acquire(const struct session *session);

bool
session_retry_execution_result_try_publish_release(
    struct session *session,
    session_retry_execution_result_t exec_result);

#endif /* KERNUL_SESSION_RETRY_EXECUTION_RESULT_STORAGE_H */
