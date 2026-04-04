/*
 * core/session_reclamation.c
 *
 * Finalization-gated local resource reclamation.
 *
 * This file provides one explicit synchronous action:
 * attempt reclamation for exactly one resource identity owned by one session.
 * No retries, no loops over multiple resources, no scheduling behavior, no
 * coordination, and no lifecycle mutation are implemented here.
 */

#include <kernul/session_reclamation.h>
#include <kernul/session_finalization.h>
#include <kernul/session_ownership.h>

/*
 * Reclaim primitive boundary for one resource identity.
 *
 * In this phase no resource-local reclaim primitive is available for
 * resource_id_t, so reclamation deterministically fails.
 */
static bool
resource_reclaim_identity(resource_id_t resource)
{
    (void)resource;
    return false;
}

session_reclaim_result_t
session_reclaim_resource_if_finalized(struct session *session,
                                      resource_id_t resource)
{
    if (session == NULL)
        return SESSION_RECLAIM_FAILED;

    if (!session_is_finalized_acquire(session))
        return SESSION_RECLAIM_NOT_FINALIZED;

    if (!session_owns_resource_acquire(session, resource))
        return SESSION_RECLAIM_NOT_OWNED;

    if (!resource_reclaim_identity(resource))
        return SESSION_RECLAIM_FAILED;

    return SESSION_RECLAIM_OK;
}
