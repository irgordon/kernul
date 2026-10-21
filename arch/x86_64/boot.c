/*
 * Limine C entry after the assembly boundary has installed our stack and GDT.
 * Requests and the converted handoff are image-owned. The boot CPU alone
 * writes the handoff with interrupts disabled; it is immutable after a
 * successful conversion and lives for the kernel's lifetime. No locks or
 * allocation are needed, and no bootloader pointers are retained in it.
 * Diagnostics are best effort: a failed UART write must not prevent the
 * terminal halt. Return codes are deliberately discarded at these stops.
 */

#include "boot_console.h"
#include "boot_info.h"
#include <kernul/arch.h>
#include <kernul/boot.h>
#include <kernul/compiler.h>
#include <kernul/errors.h>
#include <kernul/types.h>
#include <limine.h>

#define BOOT_PROTOCOL_REVISION 6U

static const char BOOT_ENTRY_MESSAGE[] = "kernul: boot entry (x86_64)\r\n";
static const char UNSUPPORTED_PROTOCOL_MESSAGE[] =
    "kernul: fatal: unsupported Limine base revision\r\n";
static const char INVALID_HANDOFF_MESSAGE[] =
    "kernul: fatal: invalid Limine memory handoff\r\n";
static const char HANDOFF_READY_MESSAGE[] =
    "kernul: handoff valid; entering kernel_main\r\n";
static const char EARLY_EXCEPTION_MESSAGE[] = "kernul: fatal: early CPU exception\r\n";

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
static void initialize_boot_diagnostics(void);
static void halt_on_unsupported_protocol(void);
static void prepare_boot_handoff(void);
static const boot_info_t *prepare_common_bootstrap(void);
static void initialize_exception_diagnostics(void);
static KERN_NORETURN void halt_after_early_exception(void);
static bool boot_protocol_is_supported(void);
static kern_err_t copy_boot_protocol_handoff(void);

/*
 * Called only by _start on the image-owned stack, with interrupts disabled.
 * Returns a borrowed, immutable image-owned handoff for kernel_main(); storage
 * stays reserved for the kernel lifetime. Failure halts without returning.
 * No allocation, locks, or sleep. _start owns the calling-convention transfer.
 */
const boot_info_t *arch_boot_entry(void)
{
    initialize_boot_architecture();
    initialize_boot_diagnostics();
    halt_on_unsupported_protocol();
    prepare_boot_handoff();
    return prepare_common_bootstrap();
}

/* The assembly exception entry supplies an aligned stack and never resumes. */
KERN_NORETURN void arch_boot_exception_stop(void)
{
    initialize_exception_diagnostics();
    halt_after_early_exception();
}

static void initialize_boot_architecture(void)
{
    arch_early_init();
}

static void initialize_boot_diagnostics(void)
{
    arch_boot_console_init();
    (void)arch_boot_console_write(BOOT_ENTRY_MESSAGE);
}

static void halt_on_unsupported_protocol(void)
{
    if (boot_protocol_is_supported())
        return;
    (void)arch_boot_console_write(UNSUPPORTED_PROTOCOL_MESSAGE);
    arch_halt();
}

static void prepare_boot_handoff(void)
{
    if (copy_boot_protocol_handoff() == KERN_OK)
        return;
    (void)arch_boot_console_write(INVALID_HANDOFF_MESSAGE);
    arch_halt();
}

static const boot_info_t *prepare_common_bootstrap(void)
{
    (void)arch_boot_console_write(HANDOFF_READY_MESSAGE);
    return &boot_info;
}

static void initialize_exception_diagnostics(void)
{
    /* A terminal exception can precede normal console initialization. */
    arch_boot_console_init();
}

static KERN_NORETURN void halt_after_early_exception(void)
{
    (void)arch_boot_console_write(EARLY_EXCEPTION_MESSAGE);
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
