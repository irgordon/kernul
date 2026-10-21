/*
 * Architecture-private Limine memory handoff adapter.
 * No allocator, firmware calls, or bootloader pointers cross this boundary.
 */
#ifndef X86_64_BOOT_INFO_H
#define X86_64_BOOT_INFO_H

#include <kernul/boot.h>
#include <limine.h>

/*
 * Convert borrowed, readable Limine responses into caller-owned storage.
 * virtual_start/end are the page-aligned linker bounds of the loaded image.
 * Context: single-CPU early boot, interrupts disabled. No locks, sleep, or
 * allocation. The caller owns all output fields; they are valid only on
 * KERN_OK and contain no borrowed pointers. Input/output must not alias.
 *
 * Only USABLE becomes free RAM. Other types remain reserved or unusable;
 * the executable-containing range becomes KERNEL. Reserved bounds round
 * outward to pages. Unaligned usable RAM, overflow, unordered/overlapping
 * converted ranges, missing image coverage, or more than 32 entries return
 * KERN_EINVAL. Reclaiming bootloader memory is a later ownership task.
 */
kern_err_t arch_boot_info_build(const struct limine_memmap_response *map,
                               const struct limine_executable_address_response *image,
                               virt_addr_t virtual_start, virt_addr_t virtual_end,
                               boot_info_t *info);

#endif /* X86_64_BOOT_INFO_H */
