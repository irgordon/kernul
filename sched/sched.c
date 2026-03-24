/*
 * sched/sched.c
 *
 * Scheduler stub implementation. Phase 4, Task 4.
 * Public interface: include/kernul/sched.h
 *
 * Provides a minimal implementation for every declared scheduler entry
 * point. sched_yield() remains phase-limited: no bootstrap path exists
 * to establish the initial current thread before Task 5, so its full
 * execution path is not reachable in this phase.
 *
 * What this implementation does:
 *   sched_init    — initializes the run queue; returns KERN_EINVAL on
 *                   repeated calls, matching the header contract exactly
 *   sched_enqueue — appends to run queue tail; asserts THREAD_RUNNABLE
 *   sched_dequeue — removes from run queue head; returns NULL if empty
 *   sched_yield   — state bookkeeping only; no context switch (Task 5)
 *   sched_current — returns the global current pointer
 *
 * What this implementation deliberately does not do:
 *   - No context switching. sched_yield() updates state only.
 *     arch_cpu_state_switch() is wired in Task 5.
 *   - No priority ordering. Threads are served in insertion order.
 *   - No idle thread. That contract is Task 6.
 *   - No preemption, no load balancing, no per-CPU state.
 *
 * Every STUB: comment identifies a site that Task 5 or later must change.
 */

#include <kernul/sched.h>
#include <kernul/runq.h>
#include <kernul/assert.h>
#include <kernul/errors.h>

/* -------------------------------------------------------------------------
 * Scheduler state
 *
 * Both are file-scope globals. They are not exposed outside this file.
 * External code accesses scheduler state only through sched_* functions.
 * ---------------------------------------------------------------------- */

/*
 * The single run queue for this phase. Not per-CPU. Not a priority queue.
 * Protected by rq.lock for all accesses after sched_init() returns.
 */
static runq_t rq;

/*
 * The currently executing thread. Written only by sched_yield().
 * Zero-initialized by C static storage rules (NULL on program start).
 * sched_init() does not write it; the initial current is established
 * by the context-switch bootstrap path in Task 5.
 *
 * At this phase there is no per-CPU current pointer.
 * STUB: per-CPU current pointers are introduced when SMP support arrives.
 */
static thread_t *current;

/*
 * Initialization guard. Prevents double-init.
 * Written once by sched_init(); read thereafter.
 */
static bool initialized;

/* -------------------------------------------------------------------------
 * sched_init
 * ---------------------------------------------------------------------- */

kern_err_t sched_init(void)
{
    if (initialized)
        return KERN_EINVAL;

    runq_init(&rq);
    initialized = true;

    return KERN_OK;
}

/* -------------------------------------------------------------------------
 * sched_enqueue
 * ---------------------------------------------------------------------- */

void sched_enqueue(thread_t *thread)
{
    /*
     * Precondition: thread must be in THREAD_RUNNABLE state.
     * The caller is responsible for setting state before enqueue.
     * An invalid state is a contract violation; assert, do not return error.
     */
    ASSERT(thread != NULL);
    ASSERT(thread->state == THREAD_RUNNABLE);

    spin_lock(&rq.lock);

    list_add_tail(&rq.threads, &thread->runq_node);
    rq.count++;

    spin_unlock(&rq.lock);
}

/* -------------------------------------------------------------------------
 * sched_dequeue
 * ---------------------------------------------------------------------- */

thread_t *sched_dequeue(void)
{
    thread_t *thread = NULL;

    spin_lock(&rq.lock);

    if (!list_empty(&rq.threads)) {
        list_node_t *node = rq.threads.next;
        list_remove(node);
        rq.count--;
        thread = list_entry(node, thread_t, runq_node);
    }

    spin_unlock(&rq.lock);

    return thread;
}

/* -------------------------------------------------------------------------
 * sched_yield
 * ---------------------------------------------------------------------- */

void sched_yield(void)
{
    thread_t *prev;
    thread_t *next;

    ASSERT(current != NULL);
    ASSERT(current->state == THREAD_RUNNING);

    prev = current;

    /*
     * Transition the calling thread back to THREAD_RUNNABLE and
     * re-enqueue it. sched_enqueue() asserts THREAD_RUNNABLE on entry,
     * so the state write must precede the enqueue call.
     */
    prev->state = THREAD_RUNNABLE;
    sched_enqueue(prev);

    /*
     * Select the next thread from the head of the queue.
     *
     * Single-thread edge case: if prev was the only runnable thread,
     * it was just re-enqueued above and is now at the tail. Dequeue
     * removes from the head. If it was the only entry, it is both head
     * and tail, so dequeue returns prev again. This is correct and
     * deliberate stub behavior: when no other thread is available, the
     * current thread continues. This is not idle-thread behavior; the
     * idle thread contract belongs to Task 6.
     */
    next = sched_dequeue();

    /*
     * sched_dequeue returns NULL only on an empty queue. We just
     * enqueued prev, so the queue is guaranteed non-empty.
     */
    ASSERT(next != NULL);

    next->state = THREAD_RUNNING;
    current     = next;

    /*
     * STUB: no context switch here. Task 5 wires arch_cpu_state_switch()
     * at this point. Until then, sched_yield() updates bookkeeping only.
     * The CPU continues executing the calling thread despite current
     * having been updated. This is correct for a single-threaded stub
     * where no real concurrency exists.
     */
}

/* -------------------------------------------------------------------------
 * sched_current
 * ---------------------------------------------------------------------- */

thread_t *sched_current(void)
{
    /*
     * No lock required. current is written only by sched_yield(), which
     * is called from task context. Reading it here in task context on
     * the same single CPU is safe at this phase.
     *
     * STUB: this requires re-examination when per-CPU state or preemption
     * is introduced.
     */
    return current;
}
