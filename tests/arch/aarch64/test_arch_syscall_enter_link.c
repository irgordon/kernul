/*
 * tests/arch/aarch64/test_arch_syscall_enter_link.c
 *
 * Link-check for AArch64 arch_syscall_enter symbol.
 * Pass condition: successful link.
 */

#include <kernul/syscall.h>

int main(void)
{
    (void)&arch_syscall_enter;
    return 0;
}
