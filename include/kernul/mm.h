#ifndef KERNUL_MM_H
#define KERNUL_MM_H

/*
 * include/kernul/mm.h
 *
 * Physical memory manager public interface. Phase 2.
 *
 * Phase 2 scope: PMM only.
 *   - Physical page allocation and release.
 *   - Page size constants.
 *
 * Out of scope until later phases:
 *   - Heap / object allocation (no complex allocation — Phase 2 constraint)
 *   - Virtual mapping and paging policy (no paging policy — Phase 2 constraint)
 *   - DMA mapping
 *
 * Concurrency model:
 *   mm_phys_init() is called once from a single CPU before any other
 *   mm_phys_* call. mm_phys_alloc() and mm_phys_free() acquire an
 *   internal lock and are safe from any context once init completes.
 *
 * Lifetime model:
 *   mm_phys_alloc() transfers ownership of one page to the caller.
 *   The caller owns the page until mm_phys_free() is called.
 *   Double-free is undefined behavior.
 */

#include <kernul/types.h>

/* -------------------------------------------------------------------------
 * Page constants
 * ---------------------------------------------------------------------- */

#define MM_PAGE_SHIFT       12
#define MM_PAGE_SIZE        (1UL << MM_PAGE_SHIFT)
#define MM_PAGE_MASK        (~(MM_PAGE_SIZE - 1UL))

#define mm_page_align_down(a)  ((a) & MM_PAGE_MASK)
#define mm_page_align_up(a)    (((a) + MM_PAGE_SIZE - 1UL) & MM_PAGE_MASK)
#define mm_is_page_aligned(a)  (((a) & (MM_PAGE_SIZE - 1UL)) == 0)

/* -------------------------------------------------------------------------
 * Physical memory manager
 * ---------------------------------------------------------------------- */

/*
 * mm_phys_init(mem_start, mem_end, reserved_start, reserved_end)
 *
 * Initialize the physical page allocator over the usable range
 * [mem_start, mem_end). The range [reserved_start, reserved_end)
 * is the loaded kernel image and must never be handed out.
 *
 * Called once during early boot from init/main.c after boot_info is
 * validated. Must not be called again.
 *
 * CONTEXT: early boot, single CPU, no interrupts assumed
 * SLEEP:   no    ALLOC: no
 * FAILURE: calls panic() on invalid or zero-sized ranges
 */
void mm_phys_init(phys_addr_t mem_start,     phys_addr_t mem_end,
                  phys_addr_t reserved_start, phys_addr_t reserved_end);

/*
 * mm_phys_alloc() -> physical address of one page, or 0 on exhaustion
 *
 * The returned page is not zeroed. Alignment is MM_PAGE_SIZE.
 *
 * CONTEXT: any
 * SLEEP:   no    ALLOC: yes (one physical page)
 * FAILURE: returns 0
 * OWNERSHIP: caller owns until mm_phys_free()
 */
phys_addr_t mm_phys_alloc(void);

/*
 * mm_phys_free(addr)
 *
 * Return a page to the allocator. addr must be page-aligned and must
 * have been returned by mm_phys_alloc(). Double-free is undefined behavior.
 *
 * CONTEXT: any
 * SLEEP:   no    ALLOC: no
 */
void mm_phys_free(phys_addr_t addr);

/*
 * mm_phys_free_count() -> snapshot of available page count
 *
 * Value may be stale immediately after return.
 * Use for diagnostics and tests only; not for allocation decisions.
 *
 * CONTEXT: any
 * SLEEP:   no    ALLOC: no
 */
u64 mm_phys_free_count(void);

#endif /* KERNUL_MM_H */
