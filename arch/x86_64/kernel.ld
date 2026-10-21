/*
 * Limine higher-half x86_64 ELF layout. The loader allocates physical backing
 * and maps these virtual segments; physical placement is not inferred here.
 */

OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

KERNEL_LOAD_BASE = 0xffffffff80000000;
KERNEL_PAGE_SIZE = 0x1000;
PHDRS
{
    /* ELF permission bits: read = 4, write = 2, execute = 1. */
    requests PT_LOAD FLAGS(6);      /* bootloader writes response fields */
    text PT_LOAD FLAGS(5);          /* read, execute */
    rodata PT_LOAD FLAGS(4);        /* read */
    data PT_LOAD FLAGS(6);          /* read, write */
    stack PT_GNU_STACK FLAGS(6);    /* non-executable stack */
}

SECTIONS
{
    . = KERNEL_LOAD_BASE;
    __kernel_start = .;

    .limine_requests ALIGN(8) :
    {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :requests

    .text ALIGN(KERNEL_PAGE_SIZE) :
    {
        KEEP(*(.text.entry))
        *(.text .text.*)
    } :text

    .rodata ALIGN(KERNEL_PAGE_SIZE) :
    {
        *(.rodata .rodata.*)
    } :rodata

    .data ALIGN(KERNEL_PAGE_SIZE) :
    {
        *(.data .data.*)
    } :data

    .bss ALIGN(KERNEL_PAGE_SIZE) (NOLOAD) :
    {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    __kernel_end = ALIGN(KERNEL_PAGE_SIZE);

    /DISCARD/ :
    {
        *(.comment)
        *(.note .note.*)
        *(.eh_frame .eh_frame.*)
    }
}
