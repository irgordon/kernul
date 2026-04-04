#ifndef KERNUL_SESSION_OWNERSHIP_H
#define KERNUL_SESSION_OWNERSHIP_H

/*
 * include/kernul/session_ownership.h
 *
 * Kernel-internal declarative session resource ownership policy.
 * Phase 16, Task 1 boundary.
 *
 * This header defines one informational ownership surface:
 * sessions may declare ownership of stable resource identities.
 *
 * Ownership semantics:
 *   - declarative only
 *   - additive and monotonic
 *   - idempotent duplicate registration
 *   - no removal path
 *
 * Dependency boundary:
 *   This surface depends only on session identity and session-owned storage.
 *   It must not depend on terminal/finalized state, scheduler policy,
 *   execution transfer behavior, or cleanup/reclamation behavior.
 *
 * Visibility contract (normative):
 *   Writers initialize ownership.entries[k] first and publish ownership.count
 *   with release semantics.
 *   Readers must acquire-load ownership.count before reading entries and must
 *   only read entries in [0, count-1].
 */

#include <kernul/types.h>

struct session;

typedef u64 resource_id_t;

enum session_ownership_register_result {
    SESSION_OWNERSHIP_REGISTER_OK = 0,
    SESSION_OWNERSHIP_REGISTER_ALREADY_PRESENT,
    SESSION_OWNERSHIP_REGISTER_CAPACITY_EXCEEDED,
};

typedef enum session_ownership_register_result
    session_ownership_register_result_t;

/*
 * Fixed-capacity ownership model:
 * each session owns a bounded registry with deterministic overflow result.
 */
#define SESSION_OWNERSHIP_FIXED_CAPACITY 64U

session_ownership_register_result_t
session_register_resource(struct session *session, resource_id_t resource);

bool
session_owns_resource_acquire(const struct session *session,
                              resource_id_t resource);

#endif /* KERNUL_SESSION_OWNERSHIP_H */
