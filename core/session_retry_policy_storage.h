#ifndef KERNUL_SESSION_RETRY_POLICY_STORAGE_H
#define KERNUL_SESSION_RETRY_POLICY_STORAGE_H

/*
 * core/session_retry_policy_storage.h
 *
 * Mechanical-only storage helpers for immutable retry policy declaration.
 *
 * Scope restrictions:
 *   These helpers provide load and CAS publication primitives only.
 *   They must not implement derived-state logic, lifecycle policy, authority
 *   checks, retries, scheduling, coordination, or semantic convenience APIs.
 */

#include <kernul/types.h>
#include <kernul/session_retry_policy.h>

struct session;

u32
session_retry_policy_state_acquire(const struct session *session);

bool
session_retry_policy_try_publish_release(
    struct session *session,
    session_retry_policy_t policy);

#endif /* KERNUL_SESSION_RETRY_POLICY_STORAGE_H */
