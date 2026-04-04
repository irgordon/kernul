#ifndef KERNUL_SESSION_RECOVERY_OUTCOME_STORAGE_H
#define KERNUL_SESSION_RECOVERY_OUTCOME_STORAGE_H

#include <kernul/types.h>
#include <kernul/session_recovery_outcome.h>

struct session;

u32
session_recovery_outcome_state_acquire(const struct session *session);

bool
session_recovery_outcome_try_publish_release(
    struct session *session,
    session_recovery_outcome_t outcome);

#endif /* KERNUL_SESSION_RECOVERY_OUTCOME_STORAGE_H */
