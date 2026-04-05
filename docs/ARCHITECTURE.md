# KERNUL Architecture Guide

Status: Authoritative  
Scope: Whole kernel  
Purpose: Define architectural structure, subsystem model, and engineering laws

---

# 1. Architectural identity

KERNUL is a monolithic, Linux-inspired operating system kernel.

The kernel is:

- one kernel address space
- one kernel trust domain
- subsystem-structured
- performance-first
- hardware-conscious
- procedurally implemented

KERNUL is not capability-rooted.  
KERNUL is not microkernel-based.  
KERNUL is not built around a universal object abstraction.

The system evolves through deterministic phase boundaries.  
Each phase introduces structure before behavior and behavior before policy.

---

# 2. Architectural laws

## 2.1 Performance first

Every kernel subsystem must be designed with awareness of:

- cache locality
- branch predictability
- allocation behavior
- lock contention
- interrupt latency
- I/O path cost

Hot paths must remain short, explicit, and unsurprising.

---

## 2.2 Hardware reality first

Hardware support is a design requirement.

The architecture must tolerate:

- diverse CPU capabilities
- varying interrupt models
- DMA requirements
- MMIO access patterns
- memory ordering constraints
- IOMMU and platform differences

No abstraction may erase necessary hardware truth.

---

## 2.3 Subsystems are the primary unit of design

The kernel is organized by machine responsibility, not by pattern theory.

Primary subsystems include:

- architecture
- core kernel
- memory management
- scheduling
- VFS
- filesystem implementations
- block I/O
- networking
- IPC
- drivers
- userspace ABI

---

## 2.4 Mechanism over ideology

Mechanisms must be selected pragmatically.

Allowed patterns include:

- direct calls
- function tables for true pluggable families
- intrusive data structures
- per-CPU state
- simple locks
- lockless paths where proven necessary

No subsystem is required to conform to a universal abstraction model.

---

## 2.5 Visible cost

Public interfaces must not hide:

- allocation
- blocking
- lock acquisition
- interrupt-state changes
- I/O
- ownership transfer

If the cost is not visible to the caller, the interface is deficient.

---

## 2.6 Hardware boundary rule

Architectural boundaries must exist only when the hardware is genuinely different.

A valid boundary exists when:

- the instruction sequence differs
- the privilege transition differs
- the boot protocol differs
- the memory ordering model differs
- the MMU or page-table format differs
- the interrupt controller or timer model differs
- the cache, TLB, or DMA rules differ
- the platform initialization sequence differs

Readable, explicit, hardware-true code is preferred over artificial uniformity.

---

## 2.7 Portability discipline

KERNUL must run correctly across multiple processor architectures without hiding machine-level behavior.

Portability is achieved through:

- explicit contracts
- architecture-local implementation
- shared policy only where behavior is genuinely common

Common code must be correct on the weakest supported architecture.

---

## 2.8 Portability drift warning

Architectural drift begins when:

- barriers are hidden behind vague helpers
- interrupt paths are forced into identical shapes
- memory-ordering assumptions leak into common code
- architecture code migrates into common subsystems
- correctness depends on undocumented hardware behavior

These changes must be rejected unless a concrete machine-level requirement exists.

---

# 2.9 Architecture boundary surfaces

The following boundary surfaces are defined early and remain stable.

These are real hardware boundaries.  
They must remain minimal.  
They must not grow into abstraction frameworks.

---

## 2.9.1 arch_halt()

Purpose:

Provide a single architecture-owned terminal execution endpoint.

Contract:

- does not return
- stops forward execution
- does not allocate memory
- does not depend on scheduler behavior
- may disable interrupts
- may enter a processor-defined halt state

All terminal execution must pass through this surface.

---

## 2.9.2 arch_early_init()

Purpose:

Provide a minimal architecture bring-up boundary.

Contract:

- callable once during early bootstrap
- may be a no-op
- must not depend on scheduler
- must not require dynamic allocation
- must not assume interrupts are enabled

---

## 2.9.3 arch_cpu_state

Purpose:

Provide the architecture-defined execution context structure used for thread switching.

Current implementation state:

- declared as a placeholder structure
- non-zero size enforced by compile-time assertion
- alignment and layout defined by architecture ports
- used by scheduler and idle thread initialization
- preserved across context switches

Required preserved state categories:

- callee-saved registers
- stack pointer
- program counter
- thread-local pointer registers

This contract defines behavior.  
It does not define register layout.

---

## 2.9.4 arch_cpu_state_init()

Purpose:

Initialize execution state for a new thread.

Responsibilities:

- establish initial stack pointer
- establish entry instruction pointer
- prepare argument passing state
- produce a valid execution context

Constraints:

- does not perform scheduling
- does not allocate memory
- does not assume interrupts enabled

---

## 2.9.5 arch_cpu_state_switch()

Purpose:

Transfer execution from one thread context to another.

Contract:

- caller must hold the run queue lock
- interrupts are controlled by the architecture port
- saves current execution state into prev
- restores execution state from next
- execution resumes using the restored context

Execution resumes in the machine state stored in prev,  
not in the lexical context of the switching thread.

This distinction is mandatory.

---

# 2.10 Memory ordering discipline

Memory ordering correctness must be established before SMP or concurrency features are implemented.

Required vocabulary:

- relaxed
- acquire
- release
- acquire-release
- full barrier
- compiler barrier

Defined in:

docs/MEMORY_ORDERING.md

---

# 3. Kernel structure

```text
arch/
core/
mm/
sched/
vfs/
fs/
block/
net/
ipc/
drivers/
init/
lib/
include/
userabi/
tests/
docs/
````

---

# 4. Subsystem roles

## 4.1 arch/

Responsibilities:

* boot entry
* trap and exception handling
* interrupt entry
* context switching
* CPU feature detection
* barrier and atomic implementation

Rules:

* no scheduling policy
* no memory policy
* no subsystem coordination

---

## 4.2 core/

Responsibilities:

* synchronization primitives
* panic and assert
* kernel infrastructure
* shared utilities

---

## 4.3 mm/

Responsibilities:

* physical memory management
* virtual memory management
* page allocation
* mapping primitives

---

## 4.4 sched/

Responsibilities:

* thread lifecycle management
* run queue management
* scheduler entry points
* current thread tracking
* idle thread management

Current implemented surfaces:

* thread structure
* thread state model
* run queue structure
* scheduler public interface
* scheduler stub implementation
* context-switch contract surface
* idle thread contract

Not yet implemented:

* preemption
* SMP scheduling
* load balancing
* scheduling policy

---

## 4.5 Idle thread role

The idle thread is the scheduler fallback execution thread.

Identity:

* fixed thread identifier
* permanent lifetime
* reserved priority

Lifetime rules:

* never destroyed
* never reaped
* always eligible for execution

State model rules:

When not selected:

Idle thread remains:

THREAD_RUNNABLE

When selected:

Idle thread becomes:

THREAD_RUNNING

Structural invariant:

The scheduler must always have at least one runnable thread.

This invariant is upheld by the idle thread.

Idle execution is not scheduling policy.

---

# 5. Thread lifecycle model

Thread states:

* THREAD_CREATED
* THREAD_RUNNABLE
* THREAD_RUNNING
* THREAD_BLOCKED
* THREAD_TERMINATED

Transitions are controlled by scheduler behavior.

The idle thread follows the same state model.

No special state exists for idle.

---

# 6. Scheduler bootstrap boundary

The scheduler begins operation only after the initial execution context is established.

Sequence:

1. idle thread initialized
2. idle thread enqueued
3. scheduler start path invoked
4. context switch establishes initial current thread

The initial current thread is not defined during idle initialization.

Bootstrap completion occurs only after the first context switch.

This boundary is explicit and must not be bypassed.

---
# 6.1 Init transition boundary

Purpose:

Define the single deterministic kernel boundary that composes the first process,
address space, and executable image before userspace execution.

This boundary is implemented by:

core/init.c  
include/kernul/init.h

This surface is the canonical init transition boundary.

It is not a helper.

It is not a boot convenience.

It is the first structural userspace launch composition point.

---

Sequence performed by the init boundary:

1. process allocated
2. address space allocated
3. structural ownership associations established
4. ELF image allocated
5. ELF image structurally validated
6. deterministic stop condition reached

The boundary intentionally stops before:

- userspace execution
- context switch into userspace
- ELF mapping
- relocation
- paging or MMU behavior
- scheduling policy
- ABI compatibility guarantees

This boundary defines composition only.

It does not define execution.

---

Failure behavior:

The init boundary must return deterministic status codes:

KERN_ENOMEM

If any allocation fails.

KERN_ENOSYS

After successful structural composition and validation.

This return value explicitly marks the phase boundary.

It is not an error.

It is a stop condition.

---

Architectural rule:

Only one init transition boundary may exist.

Future init-related work must layer around this surface.

It must not bypass or replace it.

---

# 7. Concurrency model

Approved primitives:

* spinlocks
* atomic operations
* interrupt disabling where required

Rules:

* simple locking preferred over clever locking
* lock ownership must be documented
* memory ordering must be explicit
* concurrency assumptions must be verifiable

---

# 8. API design rules

Every public function must specify:

* caller context
* sleep behavior
* allocation behavior
* lock requirements
* ownership transfer
* failure conditions

No undocumented side effects are permitted.

---

# 9. Error handling

Use explicit status returns.

Reserve panic for:

* invariant violation
* unrecoverable corruption

Error handling must be:

* local
* predictable
* mechanically reviewable

---

# 10. Initialization order

Subsystem initialization must be dependency-driven.

Current deterministic order:

1. architecture bootstrap
2. early memory setup
3. core primitives
4. scheduler initialization
5. idle thread initialization
6. scheduler start boundary
7. userspace transition

---

# 11. Compatibility statement

KERNUL targets practical Linux userspace compatibility through incremental ABI alignment and testing.

Compatibility is demonstrated through:

* executable behavior
* reproducible tests
* verified interfaces

Compatibility is never assumed.

---

# 12. Session retry lifecycle freeze boundary

The retry lifecycle is complete and frozen through outcome observation.

The canonical retry lifecycle consists of exactly:

1. Retry execution
2. Retry execution result publication
3. Retry outcome recording
4. Retry outcome observation

Observation is passive and read-only.

No retry behavior beyond observation exists in this run.

Explicit non-features:

- retry scheduling
- retry coordination
- retry escalation
- retry loops
- retry policy automation
- retry outcome reaction

Observation does not imply handling.

Outcome existence does not imply action.

Any future introduction of behavior beyond observation requires:

- a new, explicitly named phase
- a new architectural surface
- explicit acceptance criteria
