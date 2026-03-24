# KERNUL Memory Ordering Guide

Status: Authoritative  
Scope: Whole kernel  
Purpose: Define the semantic memory-ordering model used by KERNUL before SMP, lock-free code, and architecture-specific atomic implementations are introduced

## 1. Purpose

This document defines memory-ordering semantics in KERNUL terms.

It exists to prevent common code from accidentally depending on architecture-specific behavior, especially stronger ordering behavior commonly observed on x86_64.

KERNUL must be correct on all supported architectures.

Common code must not assume:

- x86-style strong ordering
- implicit visibility after ordinary stores
- implicit ordering across CPUs
- implicit ordering between device state and normal memory
- implicit ordering from compiler behavior

Memory-ordering correctness must be explicit.

## 2. Design intent

KERNUL uses a semantic memory-ordering model.

The semantic model is shared.

The machine instructions are architecture-specific.

This means:

- common code uses ordering terms and contract surfaces
- architecture code implements the actual mechanics
- correctness is defined by observable kernel behavior, not by a particular CPU instruction sequence

This document defines what the terms mean.

It does not define how any architecture implements them.

## 3. Core rule

Shared code must be written as if it is running on the weakest supported ordering model.

If code is only correct because the hardware is unusually strong-ordering, the code is incorrect.

## 4. Ordering vocabulary

KERNUL uses the following mandatory ordering terms:

- relaxed
- acquire
- release
- acquire-release
- full barrier
- compiler barrier

These terms must be used consistently across:

- atomic interfaces
- barrier interfaces
- lock interfaces
- publication and visibility rules
- wakeup and handoff paths

## 5. Semantic definitions

### 5.1 Relaxed

A relaxed operation is atomic with respect to the target object, but does not create ordering guarantees for surrounding memory operations.

Use relaxed ordering only when:

- atomicity is required
- visibility timing of surrounding memory does not matter
- no publication or synchronization is being performed

Relaxed does not mean unordered in every hardware sense.

It means KERNUL code must not depend on any ordering beyond the atomic operation itself.

### 5.2 Acquire

An acquire operation prevents later memory operations from being observed before the acquire completes.

Use acquire ordering when code is consuming or observing state that was previously published by another execution context.

Typical use:

- reading a published pointer
- observing a ready flag before reading associated data
- lock acquisition semantics
- queue or state handoff from another CPU

Acquire does not order earlier local stores with respect to other CPUs.

Acquire is a consumer-side ordering rule.

### 5.3 Release

A release operation prevents earlier memory operations from being observed after the release completes.

Use release ordering when publishing state that another execution context will later consume.

Typical use:

- publishing a fully initialized object
- setting a ready flag after writing associated data
- lock release semantics
- making queue contents visible before updating queue state

Release is a producer-side ordering rule.

### 5.4 Acquire-release

Acquire-release combines both effects.

Use acquire-release ordering when a single operation both consumes prior state and publishes new state.

Typical use:

- compare-and-swap in synchronization paths
- transition operations that both observe and publish synchronization state

### 5.5 Full barrier

A full barrier prevents both loads and stores on either side of the barrier from being reordered across it, subject to the defined contract of the architecture implementation.

Use a full barrier only when weaker ordering is insufficient.

Typical use:

- global synchronization points
- lockless handoff paths with bidirectional visibility requirements
- rare low-level sequencing paths
- architecture or scheduler transitions that require a complete ordering boundary

Full barriers are expensive and must not be used casually.

### 5.6 Compiler barrier

A compiler barrier prevents the compiler from reordering memory accesses across the barrier.

A compiler barrier does not imply a CPU barrier.

Use a compiler barrier only when the machine-level ordering requirement is already satisfied or irrelevant, and the purpose is to preserve code generation order.

Compiler barriers must not be used as a substitute for real memory ordering.

## 6. Publication rule

Publication is the act of making an object or state visible to another execution context.

KERNUL requires this rule:

A published object must be fully initialized before publication becomes visible.

This typically means:

- producer writes object contents
- producer executes release operation or release-equivalent publication step
- consumer observes publication via acquire operation or acquire-equivalent observation step
- consumer then reads object contents

If the consumer can observe a pointer, flag, generation value, or queue state before the associated data is valid, the publication is incorrect.

## 7. Visibility rule

Visibility must be established explicitly.

Writing data does not by itself guarantee that another CPU, core, or execution context will observe it in the required order.

If one execution context depends on another execution context seeing memory in a particular sequence, the code must use explicit ordering semantics.

## 8. Locking rule

Locks are synchronization primitives, not just mutual exclusion tools.

When KERNUL locking primitives are fully implemented, they must provide ordering semantics consistent with their contract.

The intended rule is:

- lock acquire provides acquire semantics
- lock release provides release semantics

Common code may rely on this only when using the finalized locking interfaces.

Until then, no code may assume future lock behavior implicitly.

## 9. Wakeup and handoff rule

Whenever one execution context wakes, schedules, or hands work to another, ordering must be explicit.

The waiting or receiving context must not observe:

- stale task state
- partially initialized queue nodes
- visible flags with invisible payload data
- ready notifications without ready data

Wakeup correctness is a memory-ordering issue as much as a control-flow issue.

## 10. Device and MMIO caution

Normal memory ordering and device/MMIO ordering are not the same thing.

This document governs the semantic model for normal kernel memory visibility.

It does not fully define MMIO, device register, DMA, or cache-coherency ordering rules.

Those rules must be defined separately in architecture and driver contracts where needed.

No driver or subsystem may assume that normal atomic or barrier usage alone is sufficient for device correctness unless the relevant hardware contract explicitly says so.

## 11. What common code must not do

Common code must not:

- rely on x86_64 ordering behavior
- assume ordinary loads and stores synchronize CPUs
- assume pointer visibility implies object visibility
- assume queue visibility implies payload visibility
- use vague helpers instead of explicit ordering terms
- use compiler barriers when CPU ordering is required
- invent local ordering vocabulary outside the standard KERNUL terms

## 12. What architecture code must do

Architecture code must:

- implement the semantics defined in this document
- keep naming aligned with the common contract
- document any architecture-specific restrictions
- ensure atomic and barrier mechanics meet the required semantic meaning

Architecture code must not weaken the semantics defined here.

## 13. Intended header surfaces

The contract surfaces for this vocabulary are:

- `include/kernul/atomic.h`
- `include/kernul/barrier.h`

These headers define:

- operation names
- ordering semantics
- allowed usage assumptions
- interface contracts

They do not define high-level policy.

## 14. Review checklist

Any patch involving shared state, lockless access, wakeup paths, queues, or atomics must be reviewable against these questions:

1. What data is being published?
2. What event makes it visible?
3. What ordering guarantees the producer side?
4. What ordering guarantees the consumer side?
5. Is this relying on x86 behavior?
6. Would this still be correct on AArch64?
7. Is a weaker ordering sufficient?
8. Is a stronger ordering being used without justification?

## 15. Minimal doctrine

Use the weakest ordering that is correct.

Use stronger ordering only when required.

Never rely on accidental hardware behavior.

Never hide ordering behind vague names.

Correctness comes before cleverness.
