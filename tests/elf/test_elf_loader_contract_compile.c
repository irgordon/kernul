/*
 * tests/elf/test_elf_loader_contract_compile.c
 *
 * Compile-check translation unit for include/kernul/elf_loader.h.
 * Pass condition: successful compilation.
 */

#include <kernul/elf_loader.h>
#include <kernul/compiler.h>

static struct elf_image *(*const check_alloc)(void) = elf_image_alloc;
static struct elf_image *(*const check_get)(struct elf_image *) = elf_image_get;
static void (*const check_put)(struct elf_image *) = elf_image_put;
static void (*const check_validate)(struct elf_image *) = elf_image_validate;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    struct elf_image img = {0};

    (void)check_alloc;
    (void)check_get;
    (void)check_put;
    (void)check_validate;

    (void)img.generation;
    (void)img.state;
    (void)img.refcount;
    (void)img.owner;
    (void)img.address_space;
    (void)img.arch_handle;

    (void)ELF_IMAGE_STATE_UNVALIDATED;
    (void)ELF_IMAGE_STATE_UNSUPPORTED;
    (void)ELF_IMAGE_STATE_INVALID;
}
