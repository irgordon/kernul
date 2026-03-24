/*
 * tests/arch/x86_64/test_arch_syscall_enter_link.c
 *
 * Link-check for x86_64 arch_syscall_enter symbol.
 * Pass condition: successful link.
 */

#include <kernul/syscall.h>

int main(void)
{
    (void)&arch_syscall_enter;
    return 0;
}
