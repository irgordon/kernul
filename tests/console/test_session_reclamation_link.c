/*
 * tests/console/test_session_reclamation_link.c
 *
 * Link-check for session reclamation dependency boundary symbols.
 * Pass condition: successful link against reclamation, finalization,
 * and ownership surfaces only.
 */

#include <kernul/session_reclamation.h>
#include <kernul/session_finalization.h>
#include <kernul/session_ownership.h>

int main(void)
{
    (void)&session_reclaim_resource_if_finalized;
    (void)&session_is_finalized_acquire;
    (void)&session_owns_resource_acquire;
    return 0;
}
