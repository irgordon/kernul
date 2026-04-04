#ifndef KERNUL_SESSION_RECOVERY_ELIGIBILITY_STORAGE_H
#define KERNUL_SESSION_RECOVERY_ELIGIBILITY_STORAGE_H

#include <kernul/types.h>
#include <kernul/session_recovery_eligibility.h>

struct session;

u32
session_recovery_eligibility_state_acquire(const struct session *session);

bool
session_recovery_eligibility_try_publish_release(
    struct session *session,
    session_recovery_eligibility_t eligibility);

#endif /* KERNUL_SESSION_RECOVERY_ELIGIBILITY_STORAGE_H */
