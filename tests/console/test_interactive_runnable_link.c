/*
 * tests/console/test_interactive_runnable_link.c
 *
 * Link-check for interactive runnable enqueue contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_runnable.h>

int main(void)
{
    (void)&interactive_runnable_enqueue;
    (void)&interactive_runnable_state;
    return 0;
}
