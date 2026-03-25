/*
 * tests/signal/test_signal_link.c
 *
 * Link-check for signal contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/signal.h>

int main(void)
{
    (void)&signal_state_alloc;
    (void)&signal_state_free;
    (void)&signal_send;
    (void)&signal_block;
    (void)&signal_unblock;
    return 0;
}
