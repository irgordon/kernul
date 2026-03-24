/*
 * mm/mm.c
 *
 * Physical memory manager stub. Phase 2.
 * Public interface: include/kernul/mm.h
 *
 * Phase 2 scope: physical page allocation only.
 * Heap, virtual mapping, and DMA are out of scope.
 *
 * Implementation: bump allocator.
 * STUB: no free-list reclaim. mm_phys_free() is a no-op.
 * Replace with a real free-list before reclaim is required.
 */

#include <kernul/mm.h>
#include <kernul/boot.h>

/* Alignment constant for the bump pointer. Named to avoid magic numbers. */
#define PMM_ALIGN_MASK  (MM_PAGE_SIZE - 1UL)

static phys_addr_t phys_bump;   /* next page to hand out          */
static phys_addr_t phys_end;    /* one past the last usable byte  */

void mm_phys_init(phys_addr_t mem_start,     phys_addr_t mem_end,
                  phys_addr_t reserved_start, phys_addr_t reserved_end)
{
    /*
     * STUB: start the bump pointer after the reserved kernel region.
     * Usable memory below reserved_start is lost in this model.
     *
     * A real PMM would build a free-list from both:
     *   [mem_start, reserved_start) and [reserved_end, mem_end)
     * using the phys_region_t entries from boot_info.
     */
    (void)mem_start;
    (void)reserved_start;

    phys_bump = mm_page_align_up(reserved_end);
    phys_end  = mm_page_align_down(mem_end);
}

phys_addr_t mm_phys_alloc(void)
{
    /* STUB: bump. Not SMP-safe. No reclaim. */
    if (phys_bump + MM_PAGE_SIZE > phys_end)
        return 0;

    phys_addr_t page  = phys_bump;
    phys_bump        += MM_PAGE_SIZE;
    return page;
}

void mm_phys_free(phys_addr_t addr)
{
    /*
     * STUB: bump allocator cannot reclaim. No-op.
     * Replace with free-list return before reclaim is needed.
     */
    (void)addr;
}

u64 mm_phys_free_count(void)
{
    if (phys_bump >= phys_end)
        return 0;
    return (phys_end - phys_bump) / MM_PAGE_SIZE;
}
