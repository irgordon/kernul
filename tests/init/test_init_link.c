/*
 * tests/init/test_init_link.c
 *
 * Link-check for init_launch symbol.
 * Pass condition: successful link.
 */

#include <kernul/init.h>

int main(void)
{
    (void)&init_launch;
    return 0;
}
