/*
 * init/boot.c
 *
 * boot_info_validate() implementation. Phase 0.
 * Contract: include/kernul/boot.h
 */

#include <kernul/boot.h>
#include <kernul/errors.h>
#include <kernul/mm.h>

kern_err_t boot_info_validate(const boot_info_t *info)
{
    bool kernel_region_found = false;

    if (info == (void *)0)
        return KERN_EINVAL;

    if (info->num_regions == 0 || info->num_regions > BOOT_MAX_PHYS_REGIONS)
        return KERN_EINVAL;

    for (u32 i = 0; i < info->num_regions; i++) {
        const phys_region_t *r = &info->regions[i];

        /* Both ends must be page-aligned. */
        if (!mm_is_page_aligned(r->base) || !mm_is_page_aligned(r->end))
            return KERN_EINVAL;

        /* Range must be non-empty and correctly ordered. */
        if (r->end <= r->base)
            return KERN_EINVAL;

        /*
         * If the kernel extent is set, verify a matching KERNEL region
         * exists. We accept a match when the region fully contains the
         * kernel extent.
         */
        if (r->type == PHYS_REGION_KERNEL &&
            r->base <= info->kernel_phys_start &&
            r->end  >= info->kernel_phys_end)
            kernel_region_found = true;
    }

    /*
     * If a kernel extent was specified, a matching KERNEL region must
     * exist. An extent of (0, 0) is permitted for minimal test builds.
     */
    if (info->kernel_phys_start != 0 || info->kernel_phys_end != 0) {
        if (!kernel_region_found)
            return KERN_EINVAL;
    }

    return KERN_OK;
}
