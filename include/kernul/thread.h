#ifndef KERNUL_THREAD_H
#define KERNUL_THREAD_H

/*
 * include/kernul/thread.h
 *
 * Thread structure and thread state model. Phase 4, Task 1.
 *
 * This header defines the data shape for a kernel thread. It is the
 * foundation for the run queue (Task 2) and the scheduler interface
 * (Task 3). No scheduling policy or behavior is defined here.
 *
 * Ownership:
 *   thread_t is allocated and owned by the scheduler subsystem.
 *   External subsystems hold thread_t* pointers and call scheduler
 *   functions to act on them. They must not read or write thread_t
 *   fields directly.
 *
 * Concurrency:
 *   All fields are protected by the run queue lock defined in
 *   include/kernul/runq.h (Task 2). No field may be read or written
 *   outside that lock except where explicitly noted.
 *
 * Lifetime:
 *   A thread_t is valid from creation until it is reaped. After reap,
 *   the pointer must not be accessed.
 */

#include <kernul/types.h>
#include <kernul/arch.h>
#include <kernul/list.h>

/*
 * thread_t embeds arch_cpu_state_t by value. C requires a complete type
 * for by-value embedding, so the complete definition of struct arch_cpu_state
 * must be visible here. cpu_state.h is supplied by the architecture-specific
 * include path (-Iarch/<target>) set in the build system.
 *
 * This binds the common thread contract to the selected target architecture
 * at build time. thread.h is bound common code, not architecture-opaque code.
 * Fields of arch_cpu_state_t must not be accessed here or in any common
 * subsystem; all operations on cpu_state go through arch_cpu_state_init()
 * and arch_cpu_state_switch() declared in cpu_state.h.
 */
#include <cpu_state.h>

/* -------------------------------------------------------------------------
 * Thread state model
 *
 * States are owned by the scheduler. Transitions happen only through
 * scheduler functions; nothing outside sched/ may write thread_t.state.
 *
 * Valid transitions:
 *
 *   THREAD_CREATED  -> THREAD_RUNNABLE   (after creation, when made ready)
 *   THREAD_RUNNABLE -> THREAD_RUNNING    (scheduler selects it)
 *   THREAD_RUNNING  -> THREAD_RUNNABLE   (preempted or yielded)
 *   THREAD_RUNNING  -> THREAD_SLEEPING   (blocks on an event)
 *   THREAD_SLEEPING -> THREAD_RUNNABLE   (event fires, thread woken)
 *   THREAD_RUNNING  -> THREAD_ZOMBIE     (thread calls thread_exit())
 *   THREAD_ZOMBIE   -> THREAD_DEAD       (reaped; pointer becomes invalid)
 * ---------------------------------------------------------------------- */

typedef enum {
    THREAD_CREATED  = 0,  /* allocated but not yet placed on a run queue  */
    THREAD_RUNNABLE = 1,  /* on a run queue, eligible to be scheduled     */
    THREAD_RUNNING  = 2,  /* currently executing on a CPU                 */
    THREAD_SLEEPING = 3,  /* blocked; waiting for an event                */
    THREAD_ZOMBIE   = 4,  /* exited; resources held until reaped          */
    THREAD_DEAD     = 5,  /* reaped; must not be accessed                 */
} thread_state_t;

/* -------------------------------------------------------------------------
 * Thread priority
 *
 * Lower numeric value means higher scheduling priority.
 * Values are used by the run queue to order threads within a queue.
 * Policy for assigning priorities is a scheduler concern, not defined here.
 * ---------------------------------------------------------------------- */

typedef u32 thread_prio_t;

#define THREAD_PRIO_IDLE    255   /* lowest; only runs when nothing else can */
#define THREAD_PRIO_LOW     192
#define THREAD_PRIO_NORMAL  128   /* default for new threads                 */
#define THREAD_PRIO_HIGH     64
#define THREAD_PRIO_KERNEL    0   /* reserved for kernel threads             */

/* -------------------------------------------------------------------------
 * Thread identifier
 *
 * Assigned at creation. Unique for the lifetime of the kernel.
 * Never reused.
 * ---------------------------------------------------------------------- */

typedef u32 thread_id_t;

#define THREAD_ID_INVALID  ((thread_id_t)0)

/* -------------------------------------------------------------------------
 * Thread name
 *
 * Human-readable label for diagnostics. Not required to be unique.
 * Not a security or identity mechanism.
 * ---------------------------------------------------------------------- */

#define THREAD_NAME_MAX  32

/* -------------------------------------------------------------------------
 * Thread structure
 *
 * Fields are grouped by access pattern to keep frequently-read state
 * near the top. See §8 of CODING_STANDARD.md.
 *
 * All fields are protected by the run queue lock unless noted otherwise.
 * ---------------------------------------------------------------------- */

typedef struct thread {

    /*
     * Scheduler state — read on every scheduling decision.
     * Protected by run queue lock.
     */
    thread_state_t   state;
    thread_prio_t    prio;

    /*
     * Run queue linkage.
     * Embedded node for intrusive list membership in a run queue.
     * Valid only while state == THREAD_RUNNABLE.
     * Protected by run queue lock.
     */
    list_node_t      runq_node;

    /*
     * Architecture execution state.
     * Embedded by value; requires the complete arch_cpu_state_t definition
     * from cpu_state.h (see include comment above). Fields must not be
     * accessed by common code. All operations go through the functions
     * declared in cpu_state.h: arch_cpu_state_init() and
     * arch_cpu_state_switch(). Not protected by the run queue lock; the
     * context switch path manages its own synchronization.
     */
    arch_cpu_state_t cpu_state;

    /*
     * Stack.
     * Base address and size of the kernel stack for this thread.
     * Set at creation; immutable thereafter.
     * Not protected by run queue lock (immutable after init).
     */
    void            *stack;
    u32              stack_size;

    /*
     * Identity.
     * Set at creation; immutable thereafter.
     * May be read without the run queue lock.
     */
    thread_id_t      id;
    char             name[THREAD_NAME_MAX];

} thread_t;

#endif /* KERNUL_THREAD_H */
