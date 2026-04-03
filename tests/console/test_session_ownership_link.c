/*
 * tests/console/test_session_ownership_link.c
 *
 * Link-check for session ownership registry symbols.
 * Pass condition: successful link.
 */

#include <kernul/session_ownership.h>

int main(void)
{
    (void)&session_register_resource;
    (void)&session_owns_resource_acquire;
    return 0;
}
