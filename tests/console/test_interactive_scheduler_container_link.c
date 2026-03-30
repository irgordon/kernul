/*
 * tests/console/test_interactive_scheduler_container_link.c
 *
 * Link-check for interactive scheduler container contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/interactive_scheduler_container.h>

int main(void)
{
    (void)&interactive_scheduler_container_materialize;
    return 0;
}
