/*
 * tests/console/test_session_recovery_outcome_observe_link.c
 *
 * Link-check for recovery outcome observation dependency boundary symbols.
 * Pass condition: successful link against observation implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_recovery_execution.h>
#include <kernul/session_recovery_outcome_observe.h>

int main(void)
{
    (void)&session_get_recovery_outcome_info_acquire;
    return 0;
}
