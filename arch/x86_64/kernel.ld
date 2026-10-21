/*
 * Flat x86_64 ELF layout for the bootstrap executable.
 * The 1 MiB load/link base is local to this layout. Architecture entry and
 * bootloader handoff are added separately; kernel_main requires a C stack
 * and a boot_info pointer and is not a firmware entry point.
 */

OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(kernel_main)

KERNEL_LOAD_BASE = 0x100000;
KERNEL_PAGE_SIZE = 0x1000;
PHDRS
{
    /* ELF permission bits: read = 4, write = 2, execute = 1. */
    text PT_LOAD FLAGS(5);          /* read, execute */
    rodata PT_LOAD FLAGS(4);        /* read */
    data PT_LOAD FLAGS(6);          /* read, write */
    stack PT_GNU_STACK FLAGS(6);    /* non-executable stack */
}

SECTIONS
{
    . = KERNEL_LOAD_BASE;

    .text ALIGN(KERNEL_PAGE_SIZE) :
    {
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

    /DISCARD/ :
    {
        *(.comment)
        *(.note .note.*)
        *(.eh_frame .eh_frame.*)
    }
}
