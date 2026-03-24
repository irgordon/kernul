#ifndef KERNUL_SCHED_H
#define KERNUL_SCHED_H

/*
 * include/kernul/sched.h
 *
 * Scheduler public interface. Phase 4, Task 3.
 *
 * This header is declarations only. Implementations are Task 4.
 *
 * Scope at this phase:
 *   The scheduler manages a single run queue. No per-CPU queues.
 *   No priority ordering. No preemption. No load balancing.
 *   These are constraints of the current phase, not permanent limits.
 *
 * Run queue ownership:
 *   The run queue (runq_t) is internal to sched/. It is not part of
 *   this interface. Callers never hold or pass a runq_t.
 *
 * State transition ownership:
 *   Only scheduler functions may change thread_t.state. No caller may
 *   write thread_t.state directly. Every function below documents the
 *   state transition it performs, if any.
 *
 * Locking:
 *   Functions that acquire the run queue lock document it explicitly.
 *   No caller may hold the run queue lock on entry to any function here
 *   unless explicitly stated otherwise.
 */

#include <kernul/thread.h>
#include <kernul/errors.h>

/* -------------------------------------------------------------------------
 * sched_init()
 *
 * Initialize the scheduler subsystem. Initializes the internal run queue.
 * Must be called once during early boot before any other sched_* function.
 *
 * State transitions: none.
 *
 * CONTEXT: early boot, single CPU, no other sched_* calls in progress
 * SLEEP:   no
 * ALLOC:   no
 * LOCKS:   none held on entry; none acquired (run queue not yet shared)
 * FAILURE: KERN_EINVAL if called more than once
 * ---------------------------------------------------------------------- */
kern_err_t sched_init(void);

/* -------------------------------------------------------------------------
 * sched_enqueue(thread)
 *
 * Place thread on the run queue.
 *
 * Precondition (enforced by ASSERT, not return code):
 *   thread->state == THREAD_RUNNABLE
 *   thread must not already be on the run queue
 *
 * Appends thread to the tail of the run queue. No ordering by priority.
 * Increments the run queue count. Does not select or schedule.
 *
 * State transitions: none. Caller is responsible for setting
 *   thread->state = THREAD_RUNNABLE before calling.
 *
 * CONTEXT: any
 * SLEEP:   no
 * ALLOC:   no
 * LOCKS:   acquires and releases run queue lock internally
 * OWNERSHIP: scheduler borrows thread; caller retains ownership.
 *   thread_t remains valid until sched_dequeue returns it.
 * FAILURE: none returned. Invalid preconditions trigger ASSERT.
 * ---------------------------------------------------------------------- */
void sched_enqueue(thread_t *thread);

/* -------------------------------------------------------------------------
 * sched_dequeue()
 *
 * Return the next thread made available by the scheduler from its
 * internal queue state. Returns NULL if no thread is available.
 *
 * Preconditions and postconditions on thread state participation — which
 * states are valid for a returned thread, and what transitions the caller
 * must perform — are defined by the Task 4 implementation. They are not
 * frozen here.
 *
 * CONTEXT: scheduler internal
 * SLEEP:   no
 * ALLOC:   no
 * LOCKS:   acquires and releases run queue lock internally
 * OWNERSHIP: returns ownership of thread to caller.
 * FAILURE: returns NULL if no thread is available.
 * ---------------------------------------------------------------------- */
thread_t *sched_dequeue(void);

/* -------------------------------------------------------------------------
 * sched_yield()
 *
 * Called by the currently running thread to voluntarily relinquish
 * the CPU. The calling thread is re-enqueued as THREAD_RUNNABLE.
 *
 * If the run queue is empty after re-enqueue, the same thread will
 * be selected again immediately (no idle behavior at this phase).
 *
 * State transitions:
 *   THREAD_RUNNING -> THREAD_RUNNABLE (calling thread, on re-enqueue)
 *   THREAD_RUNNABLE -> THREAD_RUNNING (next thread selected from queue)
 *
 * CONTEXT: task (must be called from a running thread context)
 * SLEEP:   no (returns when this thread is next scheduled)
 * ALLOC:   no
 * LOCKS:   acquires and releases run queue lock internally
 * FAILURE: none.
 * ---------------------------------------------------------------------- */
void sched_yield(void);

/* -------------------------------------------------------------------------
 * sched_current()
 *
 * Return a pointer to the currently executing thread.
 *
 * At this phase, a single global current-thread pointer is used. Per-CPU
 * current pointers are a later phase.
 *
 * The returned pointer is valid for the duration of the call. It must not
 * be cached across a yield or any operation that may cause rescheduling.
 *
 * State transitions: none.
 *
 * CONTEXT: task (must be called from a running thread context)
 * SLEEP:   no
 * ALLOC:   no
 * LOCKS:   none
 * OWNERSHIP: borrowed reference; do not free.
 * FAILURE: none. Returns NULL only if called before any thread is running;
 *   that is a caller error.
 * ---------------------------------------------------------------------- */
thread_t *sched_current(void);

#endif /* KERNUL_SCHED_H */
