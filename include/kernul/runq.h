#ifndef KERNUL_RUNQ_H
#define KERNUL_RUNQ_H

/*
 * include/kernul/runq.h
 *
 * Run queue data shape. Phase 4, Task 2.
 *
 * runq_t is a structural contract with explicit runtime initialization
 * via runq_init(). No queue ordering, selection behavior, enqueue/dequeue
 * policy, or scheduler semantics are defined in this header.
 *
 * Shape:
 *   - list_t threads    — circular intrusive list of runnable thread_t nodes
 *   - spinlock_t lock   — protects all fields of this structure and the
 *                         thread_t.state and thread_t.runq_node fields of
 *                         every thread currently on this queue
 *   - u32 count         — number of threads currently on the queue
 *
 * Concurrency model:
 *   The run queue lock must be held for every read or write of any field
 *   in runq_t and for every state or linkage field in any thread_t on
 *   this queue. This is the single locking rule; there are no exceptions
 *   at this phase.
 *
 * Ownership:
 *   runq_t is statically allocated in sched/. It is not heap-allocated.
 *   It is not passed by pointer to external subsystems.
 *   The scheduler subsystem owns it exclusively.
 *
 * What this structure is not:
 *   - It is not a priority queue. Threads are stored in insertion order.
 *   - It is not a per-CPU structure. Per-CPU run queues are a later phase.
 *   - It does not select threads. Selection is a scheduler behavior concern.
 */

#include <kernul/list.h>
#include <kernul/spinlock.h>

/* -------------------------------------------------------------------------
 * Run queue structure
 * ---------------------------------------------------------------------- */

typedef struct {
    list_t     threads;   /* intrusive list of runnable thread_t nodes;    */
                          /* linked via thread_t.runq_node                 */
                          /* protected by lock                             */
    spinlock_t lock;      /* protects all fields of this struct and the    */
                          /* state and runq_node fields of member threads  */
    u32        count;     /* number of threads currently on this queue;    */
                          /* protected by lock                             */
} runq_t;

/*
 * runq_init(rq)
 *
 * Initialize a run queue at runtime. Must be called before any other
 * operation on rq.
 *
 * Initialization visibility rule:
 *   runq_init() writes to all three fields without holding any lock.
 *   The caller must ensure no other CPU or execution context can access
 *   rq until runq_init() returns. In practice this means runq_init()
 *   must be called during single-CPU early boot, before the scheduler
 *   is started and before any secondary CPU is brought up. The scheduler
 *   start provides the necessary visibility boundary after that point.
 *
 * Post-init invariants:
 *   After runq_init() returns the following invariants hold and must be
 *   preserved by all subsequent operations on rq:
 *     - rq->threads.next == &rq->threads  (sentinel self-linked, list empty)
 *     - rq->threads.prev == &rq->threads  (sentinel self-linked, list empty)
 *     - rq->lock is in the unlocked initial state
 *     - rq->count == 0
 *
 * A static initializer macro is not provided. LIST_INIT requires the
 * variable name as its argument and cannot be used for an embedded field
 * without relying on implementation-defined compound literal behavior.
 * runq_init() is the only correct initialization path.
 *
 * CONTEXT: early boot, single CPU, before rq is shared
 * SLEEP:   no    ALLOC: no
 */
static inline void runq_init(runq_t *rq)
{
    list_init(&rq->threads);
    spin_init(&rq->lock);
    rq->count = 0;
}

#endif /* KERNUL_RUNQ_H */
