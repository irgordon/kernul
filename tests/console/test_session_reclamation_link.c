/*
 * tests/console/test_session_reclamation_link.c
 *
 * Link-check for session reclamation dependency boundary symbols.
 * Pass condition: successful link against reclamation implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_reclamation.h>
#include <kernul/session_finalization.h>
#include <kernul/session_ownership.h>

bool
session_is_finalized_acquire(const struct session *session)
{
    (void)session;
    return true;
}

bool
session_owns_resource_acquire(const struct session *session,
                              resource_id_t resource)
{
    (void)session;
    (void)resource;
    return true;
}

int main(void)
{
    (void)&session_reclaim_resource_if_finalized;
    (void)&session_is_finalized_acquire;
    (void)&session_owns_resource_acquire;
    return 0;
}
