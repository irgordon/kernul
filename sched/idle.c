/*
 * sched/idle.c
 *
 * Idle thread stub implementation. Phase 4, Task 6.
 * Contract: include/kernul/idle.h
 *
 * Provides the idle thread's static storage, entry function, and
 * initialization path. No policy decisions. No hardware interaction.
 */

#include <kernul/idle.h>
#include <kernul/sched.h>
#include <kernul/assert.h>
#include <cpu_state.h>

/* -------------------------------------------------------------------------
 * Idle thread storage
 *
 * Statically allocated. Not drawn from the PMM heap. Owned by this
 * translation unit for the kernel lifetime.
 * ---------------------------------------------------------------------- */

/*
 * Idle thread stack.
 *
 * One page (4096 bytes) is sufficient for an idle thread that only loops.
 * A real architecture may impose alignment requirements on the stack.
 * The KERN_ALIGNED(16) annotation satisfies the most common requirement
 * (16-byte alignment for AArch64 and x86-64 ABIs).
 *
 * STUB: the stub does not perform real context switching, so the stack
 * is never actually used. It is allocated here so that arch_cpu_state_init()
 * receives a valid stack_top pointer and the contract is fully satisfied.
 */
#define IDLE_STACK_SIZE  4096

static unsigned char idle_stack[IDLE_STACK_SIZE]
    __attribute__((aligned(16)));

/* The idle thread_t itself. */
static thread_t idle;

/* Initialization guard. idle_init() must be called exactly once. */
static bool idle_initialized;

/* -------------------------------------------------------------------------
 * idle_entry
 *
 * The idle thread's execution body. Runs when no other thread is runnable.
 *
 * Contract:
 *   - Does not return.
 *   - Does not make scheduling decisions.
 *   - Does not interact with hardware beyond spinning.
 *   - Does not implement power management or CPU sleep.
 *
 * STUB: spins unconditionally. A real architecture port may replace this
 * with a pause or yield instruction to reduce power consumption, provided
 * no scheduling policy is embedded in doing so. Any such change requires
 * explicit documentation and is not part of this task.
 * ---------------------------------------------------------------------- */
static KERN_NORETURN void idle_entry(void *arg)
{
    (void)arg;
    for (;;) {
        /* intentional spin; no policy, no hardware interaction */
    }
}

/* -------------------------------------------------------------------------
 * idle_init
 * ---------------------------------------------------------------------- */

kern_err_t idle_init(void)
{
    ASSERT(!idle_initialized);

    /*
     * Zero the thread_t. C zero-initializes static storage, but the
     * explicit zero here documents that we are starting from a known
     * clean state, not relying on C storage rules.
     */
    unsigned char *p = (unsigned char *)&idle;
    for (int i = 0; i < (int)sizeof(thread_t); i++)
        p[i] = 0;

    /*
     * Set identity fields.
     * These are immutable after init; no lock required here.
     */
    idle.id    = THREAD_ID_IDLE;
    idle.prio  = THREAD_PRIO_IDLE;
    idle.state = THREAD_RUNNABLE;

    idle.name[0] = 'i';
    idle.name[1] = 'd';
    idle.name[2] = 'l';
    idle.name[3] = 'e';
    idle.name[4] = '\0';

    idle.stack      = idle_stack;
    idle.stack_size = IDLE_STACK_SIZE;

    /*
     * Initialize CPU execution state.
     * stack_top is the highest address of the stack region.
     * arch_cpu_state_init() sets up the initial frame so that the first
     * switch into this thread begins executing idle_entry(NULL).
     */
    arch_cpu_state_init(&idle.cpu_state,
                         idle_entry,
                         NULL,
                         idle_stack + IDLE_STACK_SIZE);

    /*
     * Enqueue the idle thread. After this point it is THREAD_RUNNABLE
     * on the run queue. It is not yet current; the scheduler start path
     * (a later deliverable) performs the initial dequeue and establishes
     * the idle thread as the first value of current.
     */
    sched_enqueue(&idle);

    idle_initialized = true;
    return KERN_OK;
}

/* -------------------------------------------------------------------------
 * idle_thread
 * ---------------------------------------------------------------------- */

thread_t *idle_thread(void)
{
    ASSERT(idle_initialized);
    return &idle;
}