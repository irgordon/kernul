/*
 * core/elf_loader.c
 *
 * ELF loader lifecycle stubs for the kernel-internal contract surface.
 *
 * This file provides deterministic object/reference helpers and a
 * deterministic validation-state transition only.
 * No ELF parsing, no mapping, no paging, no relocation, and no execution
 * behavior are implemented here.
 */

#include <kernul/elf_loader.h>
#include <kernul/assert.h>

static struct elf_image elf_slot;
static u32 elf_slot_live;
static elf_image_generation_t elf_generation_counter;

struct elf_image *elf_image_alloc(void)
{
    u32 expected = 0U;

    if (!__atomic_compare_exchange_n(&elf_slot_live,
                                     &expected,
                                     1U,
                                     false,
                                     __ATOMIC_ACQ_REL,
                                     __ATOMIC_ACQUIRE)) {
        return NULL;
    }

    elf_slot.generation = __atomic_add_fetch(&elf_generation_counter,
                                             1U,
                                             __ATOMIC_ACQ_REL);
    elf_slot.state = ELF_IMAGE_STATE_UNVALIDATED;
    elf_slot.refcount = 1U;
    elf_slot.owner = NULL;
    elf_slot.address_space = NULL;
    elf_slot.arch_handle = NULL;

    return &elf_slot;
}

struct elf_image *elf_image_get(struct elf_image *img)
{
    if (img == NULL)
        return NULL;

    (void)__atomic_add_fetch(&img->refcount, 1U, __ATOMIC_ACQ_REL);
    return img;
}

void elf_image_put(struct elf_image *img)
{
    u32 prev;

    if (img == NULL)
        return;

    prev = __atomic_load_n(&img->refcount, __ATOMIC_ACQUIRE);
    ASSERT(prev != 0U);

    while (prev != 0U) {
        if (__atomic_compare_exchange_n(&img->refcount,
                                        &prev,
                                        prev - 1U,
                                        false,
                                        __ATOMIC_ACQ_REL,
                                        __ATOMIC_ACQUIRE)) {
            if ((prev - 1U) == 0U)
                (void)__atomic_exchange_n(&elf_slot_live, 0U, __ATOMIC_RELEASE);
            break;
        }
    }
}

void elf_image_validate(struct elf_image *img)
{
    if (img == NULL)
        return;

    if (img->state == ELF_IMAGE_STATE_UNVALIDATED)
        img->state = ELF_IMAGE_STATE_UNSUPPORTED;
}
