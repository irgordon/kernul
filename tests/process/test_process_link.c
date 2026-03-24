/*
 * tests/process/test_process_link.c
 *
 * Link-check for process lifecycle helper symbols.
 * Pass condition: successful link.
 */

#include <kernul/process.h>

int main(void)
{
    (void)&process_alloc;
    (void)&process_get;
    (void)&process_put;
    return 0;
}
