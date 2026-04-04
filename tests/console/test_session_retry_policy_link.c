/*
 * tests/console/test_session_retry_policy_link.c
 *
 * Link-check for retry policy dependency boundary symbols.
 * Pass condition: successful link against retry policy implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_retry_policy.h>

int main(void)
{
    (void)&session_declare_retry_policy;
    (void)&session_get_retry_policy_acquire;
    return 0;
}
