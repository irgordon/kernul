/*
 * tests/console/test_session_finalization_link.c
 *
 * Link-check for terminal session finalization publication symbols.
 * Pass condition: successful link.
 */

#include <kernul/session_finalization.h>
#include <kernul/session_terminal_state.h>

int main(void)
{
    (void)&session_publish_finalized_if_terminal;
    (void)&session_is_finalized_acquire;
    (void)&session_is_terminal_acquire;
    return 0;
}
