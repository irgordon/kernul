/*
 * init/main.c
 *
 * Bootstrap validation, memory selection, and the terminal phase boundary.
 * Architecture entry must establish the C execution environment and call
 * arch_early_init() before entering kernel_main(). No scheduler runs here.
 */

#include <kernul/arch.h>
#include <kernul/boot.h>
#include <kernul/errors.h>
#include <kernul/mm.h>
#include <kernul/types.h>

#define FIRST_REGION 0U
#define EMPTY_REGION_LENGTH ((phys_addr_t)0)

static void halt_on_boot_error(kern_err_t status);
static const phys_region_t *find_usable_range(const boot_info_t *info);
static void halt_without_boot_memory(const phys_region_t *region);
static void initialize_boot_memory(const boot_info_t *info, const phys_region_t *region);
static KERN_NORETURN void halt_after_bootstrap(void);
static const phys_region_t *matching_region(const phys_region_t *region, phys_region_type_t type);
static phys_addr_t region_length(const phys_region_t *region);

KERN_NORETURN void kernel_main(const boot_info_t *info)
{
    halt_on_boot_error(boot_info_validate(info));
    const phys_region_t *memory = find_usable_range(info);
    halt_without_boot_memory(memory);
    initialize_boot_memory(info, memory);
    halt_after_bootstrap();
}

static void halt_on_boot_error(kern_err_t status)
{
    if (status != KERN_OK)
        arch_halt();
}

/* The selected region is borrowed from the validated handoff. Ties keep the first. */
static const phys_region_t *find_usable_range(const boot_info_t *info)
{
    const phys_region_t *largest = NULL;
    u32 index;

    for (index = FIRST_REGION; index < info->num_regions; index++) {
        const phys_region_t *candidate = matching_region(&info->regions[index], PHYS_REGION_USABLE);

        if (region_length(candidate) > region_length(largest))
            largest = candidate;
    }
    return largest;
}

static void halt_without_boot_memory(const phys_region_t *region)
{
    if (region == NULL)
        arch_halt();
}

static void initialize_boot_memory(const boot_info_t *info, const phys_region_t *region)
{
    mm_phys_init(region->base, region->end,
                 info->kernel_phys_start, info->kernel_phys_end);
}

/* The current phase stops here; the architecture owns the terminal instruction. */
static KERN_NORETURN void halt_after_bootstrap(void)
{
    arch_halt();
}

static const phys_region_t *matching_region(const phys_region_t *region, phys_region_type_t type)
{
    if (region->type == type)
        return region;
    return NULL;
}

static phys_addr_t region_length(const phys_region_t *region)
{
    if (region == NULL)
        return EMPTY_REGION_LENGTH;
    return region->end - region->base;
}
