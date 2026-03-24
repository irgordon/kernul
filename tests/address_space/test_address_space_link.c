/*
 * tests/address_space/test_address_space_link.c
 *
 * Link-check for address-space lifecycle helper symbols.
 * Pass condition: successful link.
 */

#include <kernul/address_space.h>

int main(void)
{
    (void)&address_space_alloc;
    (void)&address_space_get;
    (void)&address_space_put;
    return 0;
}
