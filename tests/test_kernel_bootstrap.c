/*
 * Hosted verification of kernel_main's actual bootstrap control flow.
 * The real validator and entry orchestration run; only terminal hardware and
 * PMM initialization are observed. No firmware or context switch is simulated.
 */

#include <kernul/arch.h>
#include <kernul/boot.h>
#include <kernul/mm.h>
#include <kernul/types.h>

#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>

enum {
    INITIAL_CALL = 0,
    HALT_REACHED = 1,
    NO_INITIALIZATION = 0,
    ONE_INITIALIZATION = 1,
    KERNEL_START = 0x100000,
    KERNEL_END = 0x200000,
    SMALL_RANGE_END = 0x400000,
    LARGE_RANGE_START = 0x2000000,
    LARGE_RANGE_END = 0x3000000,
};

static const boot_info_t valid_handoff = {
    .regions = {
        { KERNEL_START, KERNEL_END, PHYS_REGION_KERNEL },
        { KERNEL_END, SMALL_RANGE_END, PHYS_REGION_USABLE },
        { SMALL_RANGE_END, LARGE_RANGE_START, PHYS_REGION_RESERVED },
        { LARGE_RANGE_START, LARGE_RANGE_END, PHYS_REGION_USABLE },
    },
    .num_regions = 4U,
    .kernel_phys_start = KERNEL_START,
    .kernel_phys_end = KERNEL_END,
};

static const boot_info_t invalid_handoff = { .num_regions = 0U };
static const boot_info_t reserved_handoff = {
    .regions = { { KERNEL_START, KERNEL_END, PHYS_REGION_KERNEL } },
    .num_regions = 1U,
    .kernel_phys_start = KERNEL_START,
    .kernel_phys_end = KERNEL_END,
};

struct memory_observation {
    u32 initializations;
    phys_addr_t start;
    phys_addr_t end;
    phys_addr_t reserved_start;
    phys_addr_t reserved_end;
};

/* Owned by this sequential test process; reset before each subject invocation. */
static struct memory_observation observation;
static jmp_buf halted;

static void verify_memory_selection(void);
static void verify_invalid_handoff(void);
static void verify_absent_memory(void);
static struct memory_observation observe_boot(const boot_info_t *info);
static void require_value(u64 actual, u64 expected);

int main(void)
{
    verify_memory_selection();
    verify_invalid_handoff();
    verify_absent_memory();
    return EXIT_SUCCESS;
}

static void verify_memory_selection(void)
{
    struct memory_observation result = observe_boot(&valid_handoff);

    require_value(result.initializations, ONE_INITIALIZATION);
    require_value(result.start, LARGE_RANGE_START);
    require_value(result.end, LARGE_RANGE_END);
    require_value(result.reserved_start, KERNEL_START);
    require_value(result.reserved_end, KERNEL_END);
}

static void verify_invalid_handoff(void)
{
    require_value(observe_boot(NULL).initializations, NO_INITIALIZATION);
    require_value(observe_boot(&invalid_handoff).initializations, NO_INITIALIZATION);
}

static void verify_absent_memory(void)
{
    require_value(observe_boot(&reserved_handoff).initializations, NO_INITIALIZATION);
}

/* setjmp must stay in the live invocation frame until the halt double jumps. */
static struct memory_observation observe_boot(const boot_info_t *info)
{
    observation = (struct memory_observation){0};
    if (setjmp(halted) == INITIAL_CALL)
        kernel_main(info);
    return observation;
}

static void require_value(u64 actual, u64 expected)
{
    assert(actual == expected);
}

KERN_NORETURN void arch_halt(void)
{
    longjmp(halted, HALT_REACHED);
}

void mm_phys_init(phys_addr_t start, phys_addr_t end,
                  phys_addr_t reserved_start, phys_addr_t reserved_end)
{
    observation.initializations++;
    observation.start = start;
    observation.end = end;
    observation.reserved_start = reserved_start;
    observation.reserved_end = reserved_end;
}
