/*
 * Decode Limine address/range shapes into the existing boot-info contract.
 * This adapter neither selects allocation ranges nor reclaims loader memory.
 * Output storage belongs to the single boot caller and is unpublished until
 * conversion succeeds; no response pointers survive conversion.
 */

#include "boot_info.h"
#include <kernul/errors.h>
#include <kernul/mm.h>

#define FIRST_REGION 0U
#define EMPTY_LENGTH 0U
#define PAGE_OFFSET_MASK (MM_PAGE_SIZE - 1UL)
#define MAX_PAGE_BOUNDARY (UINTPTR_MAX - PAGE_OFFSET_MASK)

static kern_err_t decode_responses(const struct limine_memmap_response *map,
                                   const struct limine_executable_address_response *image,
                                   virt_addr_t start, virt_addr_t end, boot_info_t *info);
static kern_err_t decode_memory_map(const struct limine_memmap_response *map, boot_info_t *info);
static kern_err_t decode_memory_entries(const struct limine_memmap_response *map, boot_info_t *info);
static kern_err_t complete_decoded_map(kern_err_t status, u32 count, boot_info_t *info);
static bool image_bounds_are_valid(const struct limine_executable_address_response *image,
                                   virt_addr_t start, virt_addr_t end);
static u64 image_span(const struct limine_executable_address_response *image,
                      virt_addr_t start, virt_addr_t end);
static u64 ordered_image_span(phys_addr_t physical, virt_addr_t start, virt_addr_t end);
static u64 aligned_image_span(phys_addr_t physical, virt_addr_t start, virt_addr_t end);
static bool map_response_is_valid(const struct limine_memmap_response *map);
static bool map_storage_is_valid(const struct limine_memmap_response *map);
static bool map_count_is_valid(u64 count);
static kern_err_t decode_ordered_region(const struct limine_memmap_entry *entry,
                                        boot_info_t *info, u32 index);
static kern_err_t decode_region(const struct limine_memmap_entry *entry, phys_region_t *region);
static kern_err_t decode_region_shape(const struct limine_memmap_entry *entry, phys_region_t *region);
static bool region_shape_is_valid(const struct limine_memmap_entry *entry);
static bool range_is_representable(u64 base, u64 length);
static bool length_fits(u64 length, u64 available);
static bool page_bounds_are_valid(const struct limine_memmap_entry *entry);
static kern_err_t validate_order(const boot_info_t *info, u32 index);
static bool range_is_ordered(const boot_info_t *info, u32 index);
static void decode_region_bounds(const struct limine_memmap_entry *entry, phys_region_t *region);
static phys_region_type_t decode_region_type(u64 type);
static phys_region_type_t decode_reserved_type(u64 type);
static void mark_image_region(const struct limine_memmap_entry *entry,
                              boot_info_t *info, u32 index);
static bool entry_covers_image(const struct limine_memmap_entry *entry,
                               const boot_info_t *info, u32 index);
static bool region_contains_image(const phys_region_t *region, const boot_info_t *info);
static bool should_decode_next(kern_err_t status, u32 index, u64 count);

kern_err_t arch_boot_info_build(const struct limine_memmap_response *map,
                               const struct limine_executable_address_response *image,
                               virt_addr_t virtual_start, virt_addr_t virtual_end,
                               boot_info_t *info)
{
    if (info == NULL)
        return KERN_EINVAL;
    return decode_responses(map, image, virtual_start, virtual_end, info);
}

static kern_err_t decode_responses(const struct limine_memmap_response *map,
                                   const struct limine_executable_address_response *image,
                                   virt_addr_t start, virt_addr_t end, boot_info_t *info)
{
    info->num_regions = FIRST_REGION;
    if (!image_bounds_are_valid(image, start, end))
        return KERN_EINVAL;
    info->kernel_phys_start = image->physical_base;
    info->kernel_phys_end = image->physical_base + (end - start);
    return decode_memory_map(map, info);
}

static kern_err_t decode_memory_map(const struct limine_memmap_response *map, boot_info_t *info)
{
    if (!map_response_is_valid(map))
        return KERN_EINVAL;
    return decode_memory_entries(map, info);
}

/* A validated response has at least one entry; stop at the first decode error. */
static kern_err_t decode_memory_entries(const struct limine_memmap_response *map, boot_info_t *info)
{
    u32 index = FIRST_REGION;
    kern_err_t status;

    do {
        status = decode_ordered_region(map->entries[index], info, index);
        index++;
    } while (should_decode_next(status, index, map->entry_count));
    return complete_decoded_map(status, index, info);
}

static kern_err_t complete_decoded_map(kern_err_t status, u32 count, boot_info_t *info)
{
    if (status != KERN_OK)
        return status;
    info->num_regions = count;
    return boot_info_validate(info);
}

static bool image_bounds_are_valid(const struct limine_executable_address_response *image,
                                   virt_addr_t start, virt_addr_t end)
{
    if (image == NULL)
        return false;
    return range_is_representable(image->physical_base, image_span(image, start, end));
}

static u64 image_span(const struct limine_executable_address_response *image,
                      virt_addr_t start, virt_addr_t end)
{
    if (start != image->virtual_base)
        return EMPTY_LENGTH;
    return ordered_image_span(image->physical_base, start, end);
}

static u64 ordered_image_span(phys_addr_t physical, virt_addr_t start, virt_addr_t end)
{
    if (end <= start)
        return EMPTY_LENGTH;
    return aligned_image_span(physical, start, end);
}

static u64 aligned_image_span(phys_addr_t physical, virt_addr_t start, virt_addr_t end)
{
    /* OR combines the page-offset bits of all three addresses. */
    if (!mm_is_page_aligned(physical | start | end))
        return EMPTY_LENGTH;
    return end - start;
}

static bool map_response_is_valid(const struct limine_memmap_response *map)
{
    if (map == NULL)
        return false;
    return map_storage_is_valid(map);
}

static bool map_storage_is_valid(const struct limine_memmap_response *map)
{
    if (map->entries == NULL)
        return false;
    return map_count_is_valid(map->entry_count);
}

static bool map_count_is_valid(u64 count)
{
    return count != EMPTY_LENGTH && count <= BOOT_MAX_PHYS_REGIONS;
}

static kern_err_t decode_ordered_region(const struct limine_memmap_entry *entry,
                                        boot_info_t *info, u32 index)
{
    kern_err_t status = decode_region(entry, &info->regions[index]);

    if (status != KERN_OK)
        return status;
    mark_image_region(entry, info, index);
    return validate_order(info, index);
}

static kern_err_t decode_region(const struct limine_memmap_entry *entry, phys_region_t *region)
{
    if (entry == NULL)
        return KERN_EINVAL;
    return decode_region_shape(entry, region);
}

static kern_err_t decode_region_shape(const struct limine_memmap_entry *entry, phys_region_t *region)
{
    if (!region_shape_is_valid(entry))
        return KERN_EINVAL;
    decode_region_bounds(entry, region);
    region->type = decode_region_type(entry->type);
    return KERN_OK;
}

static bool region_shape_is_valid(const struct limine_memmap_entry *entry)
{
    return range_is_representable(entry->base, entry->length) && page_bounds_are_valid(entry);
}

/* Also leave room for rounding reserved ends upward without wrapping. */
static bool range_is_representable(u64 base, u64 length)
{
    if (base >= MAX_PAGE_BOUNDARY)
        return false;
    return length_fits(length, MAX_PAGE_BOUNDARY - base);
}

static bool length_fits(u64 length, u64 available)
{
    return length != EMPTY_LENGTH && length <= available;
}

static bool page_bounds_are_valid(const struct limine_memmap_entry *entry)
{
    if (entry->type != LIMINE_MEMMAP_USABLE)
        return true;
    return mm_is_page_aligned(entry->base | entry->length);
}

static kern_err_t validate_order(const boot_info_t *info, u32 index)
{
    if (!range_is_ordered(info, index))
        return KERN_EINVAL;
    return KERN_OK;
}

static bool range_is_ordered(const boot_info_t *info, u32 index)
{
    if (index == FIRST_REGION)
        return true;
    return info->regions[index].base >= info->regions[index - 1U].end;
}

static void decode_region_bounds(const struct limine_memmap_entry *entry, phys_region_t *region)
{
    region->base = mm_page_align_down(entry->base);
    region->end = mm_page_align_up(entry->base + entry->length);
}

static phys_region_type_t decode_region_type(u64 type)
{
    if (type == LIMINE_MEMMAP_USABLE)
        return PHYS_REGION_USABLE;
    return decode_reserved_type(type);
}

static phys_region_type_t decode_reserved_type(u64 type)
{
    if (type == LIMINE_MEMMAP_BAD_MEMORY)
        return PHYS_REGION_UNUSABLE;
    return PHYS_REGION_RESERVED;
}

static void mark_image_region(const struct limine_memmap_entry *entry,
                              boot_info_t *info, u32 index)
{
    if (entry_covers_image(entry, info, index))
        info->regions[index].type = PHYS_REGION_KERNEL;
}

static bool entry_covers_image(const struct limine_memmap_entry *entry,
                               const boot_info_t *info, u32 index)
{
    return entry->type == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES &&
           region_contains_image(&info->regions[index], info);
}

static bool region_contains_image(const phys_region_t *region, const boot_info_t *info)
{
    return region->base <= info->kernel_phys_start && region->end >= info->kernel_phys_end;
}

static bool should_decode_next(kern_err_t status, u32 index, u64 count)
{
    return status == KERN_OK && index < count;
}
