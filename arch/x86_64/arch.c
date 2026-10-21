/*
 * x86_64 terminal and early exception boundaries.
 * Task 47.2 installs image-owned interrupt gates before the protocol is used.
 * The table is initialized once with interrupts disabled and remains owned
 * by this translation unit. No IRQ servicing, scheduling, or paging policy.
 */

#include <kernul/arch.h>
#include <kernul/compiler.h>
#include <kernul/types.h>

#define EARLY_VECTOR_COUNT 256U
#define INTERRUPT_GATE_BYTES 16U
#define DESCRIPTOR_POINTER_BYTES 10U
#define KERNEL_INTERRUPT_GATE 0x8eU
#define OFFSET_MIDDLE_SHIFT 16U
#define OFFSET_HIGH_SHIFT 32U

struct interrupt_gate {
    u16 offset_low;
    u16 selector;
    u8 ist;
    u8 attributes;
    u16 offset_middle;
    u32 offset_high;
    u32 reserved;
} KERN_PACKED;

struct descriptor_pointer {
    u16 limit;
    uptr base;
} KERN_PACKED;

/* Static image storage is never reclaimed while this IDTR is active. */
static struct interrupt_gate early_idt[EARLY_VECTOR_COUNT] KERN_ALIGNED(16);

extern KERN_NORETURN void arch_exception_stop(void);
static void initialize_exception_table(void);
static void load_exception_table(void);
static u16 current_code_selector(void);
static void encode_interrupt_gate(struct interrupt_gate *gate, u16 selector);

KERN_NORETURN void arch_halt(void)
{
    __asm__ volatile("cli" ::: "memory");
    for (;;) {
        __asm__ volatile("hlt");
    }
}

void arch_early_init(void)
{
    initialize_exception_table();
    load_exception_table();
}

static void initialize_exception_table(void)
{
    u16 selector = current_code_selector();

    for (u32 vector = 0U; vector < EARLY_VECTOR_COUNT; vector++)
        encode_interrupt_gate(&early_idt[vector], selector);
}

static void load_exception_table(void)
{
    const struct descriptor_pointer pointer = {
        .limit = sizeof(early_idt) - 1U,
        .base = (uptr)early_idt,
    };

    __asm__ volatile("lidt %0" : : "m"(pointer) : "memory");
}

static u16 current_code_selector(void)
{
    u16 selector;

    __asm__ volatile("movw %%cs, %0" : "=r"(selector));
    return selector;
}

static void encode_interrupt_gate(struct interrupt_gate *gate, u16 selector)
{
    uptr offset = (uptr)arch_exception_stop;

    gate->offset_low = (u16)offset;
    gate->selector = selector;
    gate->ist = 0U;
    gate->attributes = KERNEL_INTERRUPT_GATE;
    gate->offset_middle = (u16)(offset >> OFFSET_MIDDLE_SHIFT);
    gate->offset_high = (u32)(offset >> OFFSET_HIGH_SHIFT);
    gate->reserved = 0U;
}

KERN_STATIC_ASSERT(sizeof(struct interrupt_gate) == INTERRUPT_GATE_BYTES,
                   "x86_64 interrupt gates must occupy 16 bytes");
KERN_STATIC_ASSERT(sizeof(struct descriptor_pointer) == DESCRIPTOR_POINTER_BYTES,
                   "x86_64 descriptor pointers must occupy 10 bytes");
