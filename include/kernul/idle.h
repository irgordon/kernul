#ifndef KERNUL_IDLE_H
#define KERNUL_IDLE_H

/*
 * include/kernul/idle.h
 *
 * Idle thread contract. Phase 4, Task 6.
 *
 * Defines the structural role, identity, lifetime, and initialization
 * contract of the idle thread.
 *
 * -------------------------------------------------------------------------
 * Identity
 *
 * The idle thread is a single, statically allocated thread_t with the
 * following fixed properties:
 *
 *   state:    THREAD_RUNNABLE at initialization; THREAD_RUNNING when
 *             selected by the scheduler
 *   priority: THREAD_PRIO_IDLE (255 — the lowest defined priority)
 *   name:     "idle"
 *   id:       THREAD_ID_IDLE (reserved; see below)
 *   stack:    statically allocated; not drawn from the PMM heap
 *   entry:    idle_entry() — a non-returning loop; see below
 *
 * The idle thread is not dynamically created. It is not heap-allocated.
 * Its storage is owned by the scheduler subsystem for the kernel lifetime.
 *
 * -------------------------------------------------------------------------
 * Lifetime
 *
 * The idle thread is initialized exactly once, by idle_init(), during
 * the scheduler start sequence. It is never reaped. It is never destroyed.
 * It exists for the lifetime of the kernel.
 *
 * The idle thread must never transition to THREAD_ZOMBIE or THREAD_DEAD.
 *
 * Its state at any point is governed by the thread state model from Task 1:
 *   - When not selected as current: THREAD_RUNNABLE (on the run queue).
 *     The idle thread must remain on the run queue whenever it is not
 *     executing, so the scheduler always has a fallback thread to select.
 *   - When selected as current: THREAD_RUNNING, via the scheduler and
 *     context-switch path, exactly as for any other thread.
 *
 * The structural invariant this provides: the scheduler always has at
 * least one THREAD_RUNNABLE thread available. That invariant is upheld by
 * the idle thread's permanent eligibility as the fallback, not by a
 * claim that it is "always runnable" regardless of its current state.
 *
 * -------------------------------------------------------------------------
 * Relationship to the scheduler start sequence
 *
 * idle_init() must be called after sched_init() and before the scheduler
 * begins selecting threads. The required call order is:
 *
 *   1. sched_init()   — run queue initialized
 *   2. idle_init()    — idle thread initialized and enqueued
 *   3. [scheduler start path — not yet defined; later phase]
 *
 * After idle_init() returns, the idle thread is enqueued as THREAD_RUNNABLE.
 * It is not yet the current thread. The step that dequeues the idle thread,
 * transitions it to THREAD_RUNNING, and establishes it as the initial value
 * of current is the scheduler start path. That path is a later deliverable
 * and is not part of this task.
 *
 * No scheduler behavior may assume a valid current thread until the
 * scheduler start path has executed. (See ROADMAP.md: Scheduler bootstrap
 * boundary.)
 *
 * -------------------------------------------------------------------------
 * Idle execution is not scheduling policy
 *
 * The idle thread's purpose is structural: it ensures the scheduler always
 * has a thread to select when no other runnable thread exists. This is a
 * mechanical invariant, not a scheduling decision.
 *
 * The idle thread does not make scheduling decisions. It does not influence
 * which thread runs next. It does not implement power management. It does
 * not implement CPU sleep or low-power states. It does not interact with
 * timers. It does not perform load balancing. It simply loops until the
 * scheduler selects another thread.
 *
 * Any behavior added to the idle thread that causes it to select, defer,
 * or influence the scheduling of other threads is a policy decision and
 * does not belong here.
 *
 * -------------------------------------------------------------------------
 * Excluded from this contract
 *
 *   - power management behavior
 *   - CPU sleep or low-power state entry (no WFI, HLT, or equivalent)
 *   - load balancing
 *   - timer-driven scheduling
 *   - any interaction with hardware beyond spinning
 */

#include <kernul/thread.h>
#include <kernul/errors.h>

/*
 * THREAD_ID_IDLE
 *
 * Reserved thread identifier for the idle thread. The idle thread is the
 * only thread whose ID is assigned by convention rather than by an
 * allocator. All other thread IDs begin above this value.
 *
 * THREAD_ID_INVALID is 0. THREAD_ID_IDLE is 1. General-purpose thread ID
 * allocation begins at 2.
 */
#define THREAD_ID_IDLE  ((thread_id_t)1)

/*
 * idle_init()
 *
 * Initialize the idle thread and place it on the run queue.
 *
 * Actions performed:
 *   1. Zero-initialize the idle thread_t storage.
 *   2. Set identity fields: id, name, priority, state.
 *   3. Call arch_cpu_state_init() to set up initial CPU execution state
 *      so the idle thread can be switched into correctly.
 *   4. Call sched_enqueue() to make the idle thread runnable.
 *
 * After this function returns:
 *   - The idle thread is in THREAD_RUNNABLE state on the run queue.
 *   - The idle thread is not yet current. That transition occurs in the
 *     scheduler start path, which is a later deliverable.
 *
 * Preconditions:
 *   - sched_init() must have been called.
 *   - idle_init() must not be called more than once. Repeated calls are
 *     a contract violation and trigger ASSERT.
 *
 * CONTEXT: early boot, single CPU, after sched_init()
 * SLEEP:   no
 * ALLOC:   no  (idle thread storage is statically allocated)
 * LOCKS:   acquires run queue lock internally via sched_enqueue()
 * FAILURE: none returned. Precondition violations trigger ASSERT.
 */
kern_err_t idle_init(void);

/*
 * idle_thread()
 *
 * Return a pointer to the idle thread_t.
 *
 * Valid after idle_init() returns. Invalid before.
 * The returned pointer is borrowed; do not free.
 *
 * CONTEXT: any (after idle_init())
 * SLEEP:   no    ALLOC: no    LOCKS: none
 * OWNERSHIP: borrowed reference
 */
thread_t *idle_thread(void);

#endif /* KERNUL_IDLE_H */
