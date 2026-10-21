/*
 * Limine C entry after the assembly boundary has installed our stack and GDT.
 * Request storage is image-owned; the bootloader publishes compatibility
 * before entry. It is only observed afterward, with interrupts disabled.
 */

#include <kernul/arch.h>
#include <kernul/compiler.h>
#include <kernul/types.h>
#include <limine.h>

#define BOOT_PROTOCOL_REVISION 6U

__attribute__((used, section(".limine_requests_start"), aligned(8)))
static volatile u64 limine_requests_start[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests"), aligned(8)))
static volatile u64 limine_base_revision[] = LIMINE_BASE_REVISION(BOOT_PROTOCOL_REVISION);

__attribute__((used, section(".limine_requests_end"), aligned(8)))
static volatile u64 limine_requests_end[] = LIMINE_REQUESTS_END_MARKER;

static void initialize_boot_architecture(void);
static void halt_on_unsupported_protocol(void);
static KERN_NORETURN void halt_before_memory_handoff(void);
static bool boot_protocol_is_supported(void);

/* Called only by _start. No boot_info or allocator is manufactured here. */
KERN_NORETURN void arch_boot_entry(void)
{
    initialize_boot_architecture();
    halt_on_unsupported_protocol();
    halt_before_memory_handoff();
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

/* Task 47.3 replaces this stop with the real memory-map handoff. */
static KERN_NORETURN void halt_before_memory_handoff(void)
{
    arch_halt();
}

static bool boot_protocol_is_supported(void)
{
    return LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision);
}
