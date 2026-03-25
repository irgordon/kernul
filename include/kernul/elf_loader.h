#ifndef KERNUL_ELF_LOADER_H
#define KERNUL_ELF_LOADER_H

/*
 * include/kernul/elf_loader.h
 *
 * Kernel-internal ELF loader contract surface. Phase 5 boundary definition.
 *
 * This header defines ELF image shape, validation state vocabulary, and
 * ownership/association boundaries only.
 *
 * This is not a userspace ABI surface.
 * No userspace compatibility guarantees are implied by this contract.
 *
 * This header does not define:
 *   - ELF parsing behavior
 *   - mapping or paging behavior
 *   - relocation behavior
 *   - executable start behavior
 *
 * Ownership and lifetime:
 *   struct elf_image owns its own refcounted lifetime.
 *   Holders call elf_image_get() to acquire a reference and elf_image_put()
 *   to release it.
 *
 * Relationship model:
 *   @owner and @address_space are borrowed pointers only.
 *   No cross-object reference acquisition is required by this contract.
 *   No retention or reclamation policy beyond refcounting is introduced.
 */

#include <kernul/types.h>

struct process;
struct address_space;
struct arch_elf_image;

typedef u64 elf_image_generation_t;

typedef enum {
    ELF_IMAGE_STATE_UNVALIDATED = 0,
    ELF_IMAGE_STATE_UNSUPPORTED = 1,
    ELF_IMAGE_STATE_INVALID     = 2,
} elf_image_state_t;

struct elf_image {
    elf_image_generation_t   generation;
    elf_image_state_t        state;
    u32                      refcount;
    struct process          *owner;
    struct address_space    *address_space;
    struct arch_elf_image   *arch_handle;
};

/*
 * elf_image_alloc() - Allocate an ELF image object with one owned reference.
 *
 * CONTEXT: process context only.
 * SLEEP: no.
 * ALLOC: implementation-defined stub behavior.
 * LOCKS: no lock required by caller.
 * OWNERSHIP: on success returns one owned reference; on failure returns NULL.
 * FAILURE: returns NULL when stub slot is unavailable.
 */
struct elf_image *elf_image_alloc(void);

/*
 * elf_image_get() - Acquire one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount increment; caller still owns publication
 *        and object-graph locking.
 * OWNERSHIP: returns the same pointer with one additional reference.
 * FAILURE: NULL input returns NULL.
 */
struct elf_image *elf_image_get(struct elf_image *img);

/*
 * elf_image_put() - Release one shared reference.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * LOCKS: none required for refcount decrement; caller still owns publication
 *        and object-graph locking.
 * OWNERSHIP: drops one reference; final-release reclamation remains stubbed.
 * FAILURE: NULL input is ignored.
 */
void elf_image_put(struct elf_image *img);

/*
 * elf_image_validate() - Apply deterministic validation-state transition.
 *
 * CONTEXT: any.
 * SLEEP: no.
 * ALLOC: no.
 * I/O: no.
 * LOCKS: no lock required by caller in this phase; caller owns publication.
 * SIDE EFFECTS: state transition only.
 * TRANSITIONS: may transition only to ELF_IMAGE_STATE_UNSUPPORTED or
 *              ELF_IMAGE_STATE_INVALID; no state implies loadability.
 */
void elf_image_validate(struct elf_image *img);

#endif /* KERNUL_ELF_LOADER_H */
