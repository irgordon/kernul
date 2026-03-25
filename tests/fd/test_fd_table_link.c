/*
 * tests/fd/test_fd_table_link.c
 *
 * Link-check for fd-table lifecycle helper symbols.
 * Pass condition: successful link.
 */

#include <kernul/fd_table.h>

int main(void)
{
    (void)&fd_table_alloc;
    (void)&fd_table_get;
    (void)&fd_table_put;
    return 0;
}
