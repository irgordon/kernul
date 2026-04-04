#ifndef KERNUL_SESSION_RETRY_AUTHORIZATION_STORAGE_H
#define KERNUL_SESSION_RETRY_AUTHORIZATION_STORAGE_H

/*
 * core/session_retry_authorization_storage.h
 *
 * Mechanical-only storage helpers for retry authorization capability.
 *
 * Scope restrictions:
 *   These helpers provide load/store/CAS primitives only.
 *   They must not implement readiness checks, policy checks, semantic logic,
 *   retries, scheduling, coordination, or derived-state convenience APIs.
 */

#include <kernul/types.h>
#include <kernul/session_retry_authorization.h>

struct session;

u32
session_retry_authorization_state_load_acquire(const struct session *session);

void
session_retry_authorization_state_store_release(
    struct session *session,
    session_retry_authorization_state_t state);

bool
session_retry_authorization_try_grant_release(struct session *session);

bool
session_retry_authorization_try_consume_release(struct session *session);

#endif /* KERNUL_SESSION_RETRY_AUTHORIZATION_STORAGE_H */
