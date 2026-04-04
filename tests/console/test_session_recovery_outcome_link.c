/*
 * tests/console/test_session_recovery_outcome_link.c
 *
 * Link-check for recovery outcome dependency boundary symbols.
 * Pass condition: successful link against outcome implementation with
 * explicit local stubs for allowed dependency surfaces only.
 */

#include <kernul/session_recovery_execution.h>
#include <kernul/session_recovery_outcome.h>

int main(void)
{
    (void)&session_record_recovery_outcome;
    (void)&session_get_recovery_outcome_acquire;
    return 0;
}
