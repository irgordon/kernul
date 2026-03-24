# KERNUL Commenting Guide

Status: Authoritative  
Scope: All source, headers, tests, and design-adjacent code comments  
Purpose: Maximize developer understanding, review quality, and long-term maintainability

## 1. Commenting philosophy

Comments are part of the engineering surface.

A good comment improves:

- understanding
- reviewability
- correctness
- maintenance
- onboarding

A bad comment adds noise, drifts out of date, or repeats the code.

KERNUL comments must explain what the code cannot safely communicate on its own.

## 2. What comments must explain

Comments should focus on:

- invariants
- ownership
- lifetime
- locking rules
- interrupt context rules
- memory ordering
- hardware constraints
- non-obvious control-flow decisions
- compatibility constraints
- why a tradeoff was chosen

## 3. What comments must not do

Do not write comments that only restate syntax.

Bad:

```c
/* Set the count to zero. */
count = 0;
````

Do not write decorative comments that add no information.

Bad:

```c
/* Helper function */
static int do_work(void)
```

Do not use comments to excuse unclear code that should be rewritten.

## 4. Preferred comment categories

### 4.1 File header comments

Every source file should begin with a short header that states:

* what the file owns
* what role it serves
* what it does not own if that matters

Example:

```c
/*
 * sched/run_queue.c
 *
 * Per-CPU run queue management.
 *
 * This file owns run queue insertion, removal, and local selection logic.
 * Global scheduling policy and cross-CPU balancing are defined elsewhere.
 */
```

### 4.2 Function contract comments

Public and non-obvious internal functions should document:

* purpose
* caller context
* locking expectations
* sleep behavior
* ownership behavior
* failure behavior

Example:

```c
/*
 * vfs_lookup_path() - Resolve a path relative to a mount root.
 * @mount: mount that anchors the lookup
 * @path: NUL-terminated path string
 * @flags: lookup modifiers
 * @out_node: receives referenced node on success
 *
 * Context: process context only.
 * Locks: no locks required by caller.
 * Sleep: yes.
 * Ownership: on success, transfers one referenced node to @out_node.
 * Returns: 0 on success, negative error code on failure.
 */
```

### 4.3 Structure comments

Document a struct when:

* field meaning is not obvious
* fields are governed by different locks
* lifetime rules matter
* publication rules matter
* fields are hot-path sensitive

Example:

```c
struct thread {
    /*
     * Immutable after initialization.
     * Safe to read without rq lock after thread publication.
     */
    thread_id_t tid;

    /*
     * Protected by rq->lock while runnable.
     * Protected by waitq lock while blocked.
     */
    unsigned int state;

    /*
     * Shared lifetime. Final release occurs after the last scheduler and
     * process references are dropped.
     */
    atomic_t refcount;
};
```

### 4.4 Block comments for invariants

Use block comments before tricky logic, not inside every line of it.

Example:

```c
/*
 * Publish order matters here.
 *
 * Readers may observe @node through the parent table without taking the
 * table lock. The node must therefore be fully initialized before the table
 * slot is made visible.
 */
```

### 4.5 Hardware comments

Hardware-facing code must document:

* required ordering
* architecture assumptions
* register semantics
* DMA/cache constraints
* interrupt acknowledge rules

Example:

```c
/*
 * The device latches the completion bit before DMA visibility is guaranteed.
 * Read completion status first, then issue the required barrier before
 * touching the descriptor ring.
 */
```

## 5. Tone and style

Comments must be:

* direct
* technical
* concise
* grammatically correct
* human-readable

Avoid:

* jokes
* conversational filler
* vague language
* speculative language unless uncertainty is real and stated

Bad:

```c
/* We kind of do this first because otherwise things get weird. */
```

Good:

```c
/* IRQs must be disabled before queue splice to prevent same-CPU reentry. */
```

## 6. Comment placement rules

* Put comments above the code they explain.
* Avoid end-of-line comments except for tiny clarifications.
* Avoid large comment walls inside short functions.
* Prefer one strong explanatory block over many weak line comments.

## 7. Header documentation rules

Public headers must explain interface contracts, not implementation details.

A header comment should answer:

* what this interface provides
* who should include it
* what the ownership/locking contract is

## 8. Comments and synchronization

Whenever synchronization is not obvious, the code must say:

* which lock protects which field
* whether readers may proceed locklessly
* what publication rules apply
* what barriers are required
* whether the function may run in interrupt context

This is mandatory for shared-state code.

## 9. Comments and ownership

Whenever a pointer or object lifetime is not obvious, the code must say:

* who owns it
* whether the reference is borrowed or owned
* when ownership transfers
* when final release is expected

## 10. Comments and compatibility

If a behavior exists to preserve userspace compatibility, say so.

Example:

```c
/*
 * Linux userspace expects EINTR here rather than automatic restart for this
 * path, so preserve that behavior unless SA_RESTART handling applies.
 */
```

## 11. Comments and tests

Tests should explain:

* the contract being validated
* the failure mode being guarded against
* why the test exists

Bad:

```c
/* Test stuff */
```

Good:

```c
/*
 * Verifies that stale file descriptor table entries are not reused without
 * generation change after close/open churn.
 */
```

## 12. When to rewrite code instead of commenting

Rewrite the code instead of adding comments when:

* names are poor
* control flow is too deep
* state transitions are implicit
* helper boundaries are confusing
* the logic only becomes understandable through a paragraph

Comments are not a substitute for clear code.

## 13. Examples

### Bad example

```c
/* lock */
spin_lock(&dev->lock);

/* check if active */
if (dev->active) {
    /* set stopped */
    dev->active = 0;
}

/* unlock */
spin_unlock(&dev->lock);
```

### Better example

```c
/*
 * Serialize state transition against IRQ-side completion handling.
 * Once @active is cleared, no new completions may be attributed to this
 * device instance.
 */
spin_lock(&dev->lock);
if (dev->active)
    dev->active = 0;
spin_unlock(&dev->lock);
```

## 14. Required comment checklist

Before committing non-trivial code, ask:

1. Does the code expose ownership clearly?
2. Does the code expose locking clearly?
3. Does the code explain non-obvious ordering?
4. Does the code explain hardware-specific constraints?
5. Does the code explain compatibility-driven behavior?
6. Are any comments merely repeating syntax?
7. Should unclear code be rewritten instead of further commented?
