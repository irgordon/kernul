/*
 * Limine x86_64 machine boundary: long mode is supplied by the bootloader.
 * Our image owns the stack and GDT before the first C call. The GDT retains
 * Limine's 0x28/0x30 selectors, so the active segment caches remain valid.
 */

.set BOOT_STACK_BYTES, 16384
.set STACK_ALIGNMENT, 16

.section .text.entry, "ax", @progbits
.global _start
.type _start, @function
_start:
    cli
    cld
    leaq arch_boot_stack_top(%rip), %rsp
    xorl %ebp, %ebp
    lgdt arch_boot_gdtr(%rip)
    call arch_boot_entry
    /* Transfer the borrowed handoff using the x86_64 System V C ABI. */
    movq %rax, %rdi
    call kernel_main
    jmp arch_halt
.size _start, . - _start

/* All early vectors are terminal. No interrupted context is ever resumed. */
.global arch_exception_stop
.type arch_exception_stop, @function
arch_exception_stop:
    cli
    cld
    andq $-STACK_ALIGNMENT, %rsp
    call arch_boot_exception_stop
    jmp arch_halt
.size arch_exception_stop, . - arch_exception_stop

.section .rodata.boot_gdt, "a", @progbits
.balign 8
arch_boot_gdt:
    .quad 0                         /* null selector */
    .quad 0, 0, 0, 0               /* unused selectors 0x08 through 0x20 */
    /* Accessed bits are preset because this GDT is mapped read-only. */
    .quad 0x00af9b000000ffff        /* 0x28: ring-0 64-bit readable code */
    .quad 0x00cf93000000ffff        /* 0x30: ring-0 writable data */
arch_boot_gdt_end:
arch_boot_gdtr:
    .word arch_boot_gdt_end - arch_boot_gdt - 1
    .quad arch_boot_gdt

.section .bss.boot_stack, "aw", @nobits
.balign STACK_ALIGNMENT
arch_boot_stack:
    .skip BOOT_STACK_BYTES
arch_boot_stack_top:

.section .note.GNU-stack, "", @progbits
