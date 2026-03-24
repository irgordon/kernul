#ifndef KERNUL_BOOT_H
#define KERNUL_BOOT_H

/*
 * include/kernul/boot.h
 *
 * Boot contract: memory map handoff from the bootloader to the kernel.
 * Phase 0 deliverable.
 *
 * The architecture boot code populates a boot_info_t and passes a pointer
 * to kernel_main(). This is the only mechanism by which the early memory
 * map is communicated to the kernel.
 *
 * Scope discipline:
 *   Fields exist only when a concrete requiring task exists.
 *   Platform discovery fields (ACPI, initrd, cmdline) are not included
 *   because no phase 0-2 task requires them. They are added when the
 *   requiring phase arrives.
 *
 * Ownership:
 *   boot_info_t is provided by architecture boot code and resides in
 *   memory not yet managed by the PMM. The PMM consumes it during
 *   mm_phys_init() and does not retain a pointer afterward.
 *
 * Stability:
 *   Kernel-internal. Not visible to userspace. Fields may be appended
 *   when a requiring task demands them; existing offsets must not change
 *   without updating all architecture boot code.
 */

#include <kernul/types.h>

/* -------------------------------------------------------------------------
 * Physical memory region type
 * ---------------------------------------------------------------------- */

typedef enum {
    PHYS_REGION_USABLE   = 0,  /* free RAM, available to the PMM          */
    PHYS_REGION_RESERVED = 1,  /* firmware or hardware-reserved           */
    PHYS_REGION_KERNEL   = 2,  /* loaded kernel image; must not be freed  */
    PHYS_REGION_UNUSABLE = 3,  /* defective or inaccessible RAM           */
} phys_region_type_t;

/*
 * phys_region_t
 *
 * Describes one contiguous range of physical address space.
 * base and end must both be page-aligned.
 * base < end must hold for every valid entry.
 */
typedef struct {
    phys_addr_t        base;
    phys_addr_t        end;   /* one past the last byte of the region */
    phys_region_type_t type;
} phys_region_t;

/*
 * BOOT_MAX_PHYS_REGIONS
 *
 * Maximum number of regions the boot contract carries. Sized to cover
 * realistic firmware-provided memory maps without dynamic allocation
 * during early boot.
 */
#define BOOT_MAX_PHYS_REGIONS 32

/* -------------------------------------------------------------------------
 * Boot information block
 * ---------------------------------------------------------------------- */

typedef struct {
    /*
     * Physical memory map from the bootloader.
     *
     * regions[0..num_regions) describe all characterized physical ranges.
     * No memory may be treated as usable unless it appears here as
     * PHYS_REGION_USABLE.
     *
     * Regions may be unordered. Regions must not overlap.
     */
    phys_region_t regions[BOOT_MAX_PHYS_REGIONS];
    u32           num_regions;

    /*
     * Kernel image physical extent.
     *
     * Must correspond to a PHYS_REGION_KERNEL entry in regions[].
     * The PMM uses these to exclude the image from the free pool.
     */
    phys_addr_t kernel_phys_start;
    phys_addr_t kernel_phys_end;

} boot_info_t;

/* -------------------------------------------------------------------------
 * Boot info validation
 * ---------------------------------------------------------------------- */

/*
 * boot_info_validate(info) -> KERN_OK or KERN_EINVAL
 *
 * Called by kernel_main() before passing boot_info to any subsystem.
 * Checks that num_regions is in range, all regions are page-aligned,
 * no regions overlap, and the kernel extent matches a KERNEL region.
 *
 * CONTEXT: early boot, single CPU, no interrupts
 * SLEEP:   no    ALLOC: no
 * FAILURE: returns KERN_EINVAL; caller must panic()
 */
kern_err_t boot_info_validate(const boot_info_t *info);

#endif /* KERNUL_BOOT_H */
