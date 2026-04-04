#ifndef KERNUL_SESSION_RECOVERY_AUTHORIZATION_STORAGE_H
#define KERNUL_SESSION_RECOVERY_AUTHORIZATION_STORAGE_H

#include <kernul/types.h>

struct session;

enum session_recovery_authorization_state {
    SESSION_RECOVERY_AUTHORIZATION_UNAUTHORIZED = 0,
    SESSION_RECOVERY_AUTHORIZATION_AUTHORIZED = 1,
    SESSION_RECOVERY_AUTHORIZATION_CONSUMED = 2,
};

u32
session_recovery_authorization_state_acquire(const struct session *session);

bool
session_recovery_authorization_try_authorize_release(struct session *session);

bool
session_recovery_authorization_try_consume_release(struct session *session);

#endif /* KERNUL_SESSION_RECOVERY_AUTHORIZATION_STORAGE_H */
