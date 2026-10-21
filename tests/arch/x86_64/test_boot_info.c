/*
 * Focused adapter checks: reservation/lifetime preservation and rejection of
 * map shapes that could wrap an address, overlap ownership, or overrun storage.
 * Firmware execution is verified separately in the real q35 guest.
 */

#include "boot_info.h"
#include <kernul/errors.h>

#include <assert.h>
#include <stdlib.h>

#define IMAGE_VIRTUAL_START UINT64_C(0xffffffff80000000)
#define IMAGE_VIRTUAL_END   UINT64_C(0xffffffff80003000)
#define IMAGE_PHYSICAL_START UINT64_C(0x200000)
#define IMAGE_PHYSICAL_END   UINT64_C(0x203000)
#define USABLE_END UINT64_C(0x800000)
#define RESERVED_PAGE_START UINT64_C(0x900000)
#define RESERVED_PAGE_END UINT64_C(0x902000)
#define TEST_MAX_ENTRIES 7U
#define EMPTY_LENGTH 0U
#define UNALIGNED_OFFSET 1U
#define OVERFLOW_BASE (UINT64_MAX - UINT64_C(0xfff))
#define OVERFLOW_LENGTH UINT64_C(0x2000)
#define EXCESS_REGION_COUNT (BOOT_MAX_PHYS_REGIONS + 1U)

enum {
    IMAGE_REGION,
    USABLE_REGION,
    LOADER_REGION,
    ACPI_REGION,
    RESERVED_REGION,
    BAD_REGION,
    UNKNOWN_REGION,
};

static const struct limine_executable_address_response image = {
    .physical_base = IMAGE_PHYSICAL_START,
    .virtual_base = IMAGE_VIRTUAL_START,
};

/* Each case owns its stack fixture and rebinds its response before conversion. */
struct map_fixture {
    struct limine_memmap_entry regions[TEST_MAX_ENTRIES];
    struct limine_memmap_entry *pointers[TEST_MAX_ENTRIES];
    struct limine_memmap_response map;
};

static const struct map_fixture map_template = {
    .regions = {
        { IMAGE_PHYSICAL_START, 0x3000, LIMINE_MEMMAP_EXECUTABLE_AND_MODULES },
        { IMAGE_PHYSICAL_END, USABLE_END - IMAGE_PHYSICAL_END, LIMINE_MEMMAP_USABLE },
        { 0x800000, 0x10000, LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE },
        { 0x810000, 0x10000, LIMINE_MEMMAP_ACPI_RECLAIMABLE },
        { 0x900123, 0x1000, LIMINE_MEMMAP_RESERVED },
        { 0xa00000, 0x1000, LIMINE_MEMMAP_BAD_MEMORY },
        { 0xb00000, 0x1000, UINT64_MAX },
    },
    .map = { .entry_count = TEST_MAX_ENTRIES },
};

static void verify_reservations(void);
static void verify_invalid_ranges(void);
static void verify_map_capacity(void);
static void verify_required_responses(void);
static struct map_fixture valid_fixture(void);
static kern_err_t convert(struct map_fixture *fixture, boot_info_t *info);
static void require(bool condition);

int main(void)
{
    verify_reservations();
    verify_invalid_ranges();
    verify_map_capacity();
    verify_required_responses();
    return EXIT_SUCCESS;
}

static void verify_reservations(void)
{
    struct map_fixture fixture = valid_fixture();
    boot_info_t info;

    require(convert(&fixture, &info) == KERN_OK);
    require(info.num_regions == TEST_MAX_ENTRIES);
    require(info.kernel_phys_start == IMAGE_PHYSICAL_START);
    require(info.kernel_phys_end == IMAGE_PHYSICAL_END);
    require(info.regions[IMAGE_REGION].type == PHYS_REGION_KERNEL);
    require(info.regions[USABLE_REGION].type == PHYS_REGION_USABLE);
    require(info.regions[LOADER_REGION].type == PHYS_REGION_RESERVED);
    require(info.regions[ACPI_REGION].type == PHYS_REGION_RESERVED);
    require(info.regions[RESERVED_REGION].base == RESERVED_PAGE_START);
    require(info.regions[RESERVED_REGION].end == RESERVED_PAGE_END);
    require(info.regions[BAD_REGION].type == PHYS_REGION_UNUSABLE);
    require(info.regions[UNKNOWN_REGION].type == PHYS_REGION_RESERVED);
    fixture.regions[USABLE_REGION].length = EMPTY_LENGTH;
    require(info.regions[USABLE_REGION].end == USABLE_END);
}

static void verify_invalid_ranges(void)
{
    struct map_fixture fixture = valid_fixture();
    boot_info_t info;

    fixture.regions[USABLE_REGION].base = IMAGE_PHYSICAL_START;
    require(convert(&fixture, &info) == KERN_EINVAL);
    fixture = valid_fixture();
    fixture.regions[USABLE_REGION].base += UNALIGNED_OFFSET;
    require(convert(&fixture, &info) == KERN_EINVAL);
    fixture = valid_fixture();
    fixture.regions[UNKNOWN_REGION].base = OVERFLOW_BASE;
    fixture.regions[UNKNOWN_REGION].length = OVERFLOW_LENGTH;
    require(convert(&fixture, &info) == KERN_EINVAL);
    fixture = valid_fixture();
    fixture.regions[IMAGE_REGION].type = LIMINE_MEMMAP_RESERVED;
    require(convert(&fixture, &info) == KERN_EINVAL);
}

static void verify_map_capacity(void)
{
    struct map_fixture fixture = valid_fixture();
    boot_info_t info;

    fixture.map.entry_count = EXCESS_REGION_COUNT;
    require(convert(&fixture, &info) == KERN_EINVAL);
}

static void verify_required_responses(void)
{
    struct map_fixture fixture = valid_fixture();
    boot_info_t info;

    require(arch_boot_info_build(NULL, &image, IMAGE_VIRTUAL_START,
                                 IMAGE_VIRTUAL_END, &info) == KERN_EINVAL);
    require(arch_boot_info_build(&fixture.map, NULL, IMAGE_VIRTUAL_START,
                                 IMAGE_VIRTUAL_END, &info) == KERN_EINVAL);
}

static struct map_fixture valid_fixture(void)
{
    return map_template;
}

static kern_err_t convert(struct map_fixture *fixture, boot_info_t *info)
{
    for (u32 index = 0; index < TEST_MAX_ENTRIES; index++)
        fixture->pointers[index] = &fixture->regions[index];
    fixture->map.entries = fixture->pointers;
    return arch_boot_info_build(&fixture->map, &image, IMAGE_VIRTUAL_START,
                                IMAGE_VIRTUAL_END, info);
}

static void require(bool condition)
{
    assert(condition);
}
