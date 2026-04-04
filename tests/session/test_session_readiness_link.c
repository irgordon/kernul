/*
 * tests/session/test_session_readiness_link.c
 *
 * Link-check for session readiness publication/observation symbols.
 * Pass condition: successful link.
 */

#include <kernul/session.h>

int main(void)
{
    (void)&session_publish_ready_release;
    (void)&session_is_ready_acquire;
    return 0;
}
