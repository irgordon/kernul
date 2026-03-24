/*
 * init/main.c
 *
 * Kernel entry validation path. Phase 0.
 *
 * kernel_main() is the first C function called by architecture boot code.
 * Its responsibilities at Phase 0-2 are strictly bounded:
 *
 *   1. Validate boot_info.
 *   2. Initialize the PMM.
 *   3. Halt deterministically.
 *
 * No subsystem beyond the PMM is initialized here yet.
 * Scheduler, drivers, VFS, and userspace are all later phases.
 *
 * Governance:
 *   This file must not grow beyond the current phase's requirements.
 *   Each new init step is added only when its phase arrives.
 *   See docs/ROADMAP.md and Global Rule 6 (no speculative expansion).
 */

#include <kernul/arch.h>
#include <kernul/boot.h>
#include <kernul/errors.h>
#include <kernul/mm.h>
#include <kernul/types.h>

/*
 * find_usable_range(info, out_start, out_end)
 *
 * Walk the boot memory map and return the extents of the largest
 * contiguous PHYS_REGION_USABLE range. Used to seed the PMM.
 *
 * Returns KERN_OK if a usable range was found, KERN_ENOENT if none exists.
 *
 * CONTEXT: early boot    SLEEP: no    ALLOC: no
 */
static kern_err_t find_usable_range(const boot_info_t *info,
                                     phys_addr_t *out_start,
                                     phys_addr_t *out_end)
{
    phys_addr_t best_start = 0;
    phys_addr_t best_end   = 0;

    for (u32 i = 0; i < info->num_regions; i++) {
        const phys_region_t *r = &info->regions[i];

        if (r->type != PHYS_REGION_USABLE)
            continue;

        if ((r->end - r->base) > (best_end - best_start)) {
            best_start = r->base;
            best_end   = r->end;
        }
    }

    if (best_end == 0)
        return KERN_ENOENT;

    *out_start = best_start;
    *out_end   = best_end;
    return KERN_OK;
}

/*
 * kernel_main(info)
 *
 * Primary kernel entry point. Called by arch boot code with a populated
 * boot_info_t after the minimal hardware environment is established.
 *
 * Does not return. Halts via arch_halt() at the end of Phase 0-2 work.
 *
 * CONTEXT: early boot, single CPU, interrupts in arch-defined state
 * SLEEP:   no    ALLOC: no (before PMM init)
 */
KERN_NORETURN void kernel_main(const boot_info_t *info)
{
    kern_err_t    err;
    phys_addr_t   mem_start;
    phys_addr_t   mem_end;

    /* --- Phase 1: architecture boundary is already established.       --- */
    /* arch_early_init() was called by the architecture boot code before
     * jumping here. kernel_main() does not call it again.               */

    /* --- Phase 0: validate boot contract.                             --- */
    err = boot_info_validate(info);
    if (err != KERN_OK)
        arch_halt();   /* panic() is a Phase 3 symbol; halt directly here */

    /* --- Phase 2: initialize physical memory manager.                 --- */
    err = find_usable_range(info, &mem_start, &mem_end);
    if (err != KERN_OK)
        arch_halt();   /* no usable memory — unrecoverable              */

    mm_phys_init(mem_start,               mem_end,
                 info->kernel_phys_start,  info->kernel_phys_end);

    /*
     * Phase 3 and beyond are not initialized here yet.
     * Each phase adds its init call when the phase arrives.
     * Do not add calls speculatively. See docs/ROADMAP.md Rule 6.
     */

    arch_halt();
}
