/*
 * tests/elf/test_elf_loader_link.c
 *
 * Link-check for ELF image helper symbols.
 * Pass condition: successful link.
 */

#include <kernul/elf_loader.h>

int main(void)
{
    (void)&elf_image_alloc;
    (void)&elf_image_get;
    (void)&elf_image_put;
    (void)&elf_image_validate;
    return 0;
}
