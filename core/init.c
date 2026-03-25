/*
 * core/init.c
 *
 * Deterministic init launch-path composition stub for the Phase 5 boundary.
 *
 * This file composes process/address-space/ELF contract surfaces structurally.
 * No userspace execution, no VM behavior, and no loader execution is
 * implemented here.
 */

#include <kernul/init.h>
#include <kernul/process.h>
#include <kernul/address_space.h>
#include <kernul/elf_loader.h>
#include <kernul/errors.h>

int init_launch(void)
{
    struct process *proc;
    struct address_space *as;
    struct elf_image *img;

    proc = process_alloc();
    if (proc == NULL)
        return KERN_ENOMEM;

    as = address_space_alloc();
    if (as == NULL)
        return KERN_ENOMEM;

    proc->address_space = as;
    as->owner = proc;

    img = elf_image_alloc();
    if (img == NULL)
        return KERN_ENOMEM;

    img->owner = proc;
    img->address_space = as;

    elf_image_validate(img);

    return KERN_ENOSYS;
}
