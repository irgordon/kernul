/*
 * core/session_ownership.c
 *
 * Declarative session resource ownership registry.
 *
 * This file owns session-scoped registration and observation of stable
 * resource identities. The registry is additive, idempotent, and capacity
 * bounded. No allocation, reclamation, cleanup, scheduling, or lifecycle
 * behavior is implemented here.
 */

#include <kernul/session_ownership.h>
#include <kernul/session.h>

static void
session_ownership_lock_acquire(struct session_owned_resource_registry *registry)
{
    while (__atomic_exchange_n(&registry->lock, 1U, __ATOMIC_ACQUIRE) != 0U)
        ;
}

static void
session_ownership_lock_release(struct session_owned_resource_registry *registry)
{
    __atomic_store_n(&registry->lock, 0U, __ATOMIC_RELEASE);
}

session_ownership_register_result_t
session_register_resource(struct session *session, resource_id_t resource)
{
    u32 index;
    u32 count;

    if (session == NULL)
        return SESSION_OWNERSHIP_REGISTER_CAPACITY_EXCEEDED;
    if (!session_is_ready_acquire(session))
        return SESSION_OWNERSHIP_REGISTER_CAPACITY_EXCEEDED;

    session_ownership_lock_acquire(&session->ownership);

    count = __atomic_load_n(&session->ownership.count, __ATOMIC_ACQUIRE);
    for (index = 0U; index < count; ++index) {
        if (__atomic_load_n(&session->ownership.entries[index],
                            __ATOMIC_ACQUIRE) == resource) {
            session_ownership_lock_release(&session->ownership);
            return SESSION_OWNERSHIP_REGISTER_ALREADY_PRESENT;
        }
    }

    if (count >= SESSION_OWNERSHIP_FIXED_CAPACITY) {
        session_ownership_lock_release(&session->ownership);
        return SESSION_OWNERSHIP_REGISTER_CAPACITY_EXCEEDED;
    }

    __atomic_store_n(&session->ownership.entries[count],
                     resource,
                     __ATOMIC_RELEASE);
    __atomic_store_n(&session->ownership.count, count + 1U, __ATOMIC_RELEASE);
    session_ownership_lock_release(&session->ownership);
    return SESSION_OWNERSHIP_REGISTER_OK;
}

bool
session_owns_resource_acquire(const struct session *session,
                              resource_id_t resource)
{
    u32 index;
    u32 count;

    if (session == NULL)
        return false;
    if (!session_is_ready_acquire(session))
        return false;

    count = __atomic_load_n(&session->ownership.count, __ATOMIC_ACQUIRE);
    for (index = 0U; index < count; ++index) {
        if (__atomic_load_n(&session->ownership.entries[index],
                            __ATOMIC_ACQUIRE) == resource) {
            return true;
        }
    }

    return false;
}
