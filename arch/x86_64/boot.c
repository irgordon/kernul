/*
 * Limine C entry after the assembly boundary has installed our stack and GDT.
 * Requests and the converted handoff are image-owned. The boot CPU alone
 * writes the handoff with interrupts disabled; it is immutable after a
 * successful conversion and lives for the kernel's lifetime. No locks or
 * allocation are needed, and no bootloader pointers are retained in it.
 */

#include "boot_info.h"
#include <kernul/arch.h>
#include <kernul/compiler.h>
#include <kernul/errors.h>
#include <kernul/types.h>
#include <limine.h>

#define BOOT_PROTOCOL_REVISION 6U

__attribute__((used, section(".limine_requests_start"), aligned(8)))
static volatile u64 limine_requests_start[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests"), aligned(8)))
static volatile u64 limine_base_revision[] = LIMINE_BASE_REVISION(BOOT_PROTOCOL_REVISION);

__attribute__((used, section(".limine_requests"), aligned(8)))
static volatile struct limine_memmap_request memory_map_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
};

__attribute__((used, section(".limine_requests"), aligned(8)))
static volatile struct limine_executable_address_request executable_address_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
};

__attribute__((used, section(".limine_requests_end"), aligned(8)))
static volatile u64 limine_requests_end[] = LIMINE_REQUESTS_END_MARKER;

extern const u8 __kernel_start[];
extern const u8 __kernel_end[];

static boot_info_t boot_info;

static void initialize_boot_architecture(void);
static void halt_on_unsupported_protocol(void);
static void prepare_boot_handoff(void);
static KERN_NORETURN void halt_after_boot_handoff(void);
static bool boot_protocol_is_supported(void);
static kern_err_t copy_boot_protocol_handoff(void);

/* Called only by _start; common bootstrap composition remains Task 47.5. */
KERN_NORETURN void arch_boot_entry(void)
{
    initialize_boot_architecture();
    halt_on_unsupported_protocol();
    prepare_boot_handoff();
    halt_after_boot_handoff();
}

static void initialize_boot_architecture(void)
{
    arch_early_init();
}

static void halt_on_unsupported_protocol(void)
{
    if (!boot_protocol_is_supported())
        arch_halt();
}

static void prepare_boot_handoff(void)
{
    if (copy_boot_protocol_handoff() != KERN_OK)
        arch_halt();
}

/* Diagnostics and kernel_main composition belong to Tasks 47.4 and 47.5. */
static KERN_NORETURN void halt_after_boot_handoff(void)
{
    arch_halt();
}

static bool boot_protocol_is_supported(void)
{
    return LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision);
}

static kern_err_t copy_boot_protocol_handoff(void)
{
    return arch_boot_info_build(memory_map_request.response,
                                executable_address_request.response,
                                (virt_addr_t)__kernel_start,
                                (virt_addr_t)__kernel_end, &boot_info);
}
