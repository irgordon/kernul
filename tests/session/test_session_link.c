/*
 * tests/session/test_session_link.c
 *
 * Link-check for session and process-group contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/session.h>

int main(void)
{
    (void)&session_create;
    (void)&process_group_create;
    (void)&session_publish_ready_release;
    (void)&session_is_ready_acquire;
    (void)&session_id;
    (void)&process_group_id;
    return 0;
}
