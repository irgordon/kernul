/*
 * tests/syscall/test_sys_dispatch_link.c
 *
 * Link-check for sys_dispatch symbol.
 * Pass condition: successful link.
 */

#include <kernul/syscall.h>

int main(void)
{
    (void)&sys_dispatch;
    return 0;
}
