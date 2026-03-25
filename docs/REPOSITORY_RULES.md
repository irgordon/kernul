# KERNUL Repository Rules

Status: Binding  
Scope: Whole repository  
Purpose: Keep the tree coherent, reviewable, and maintainable

---

# Core Rules

1. `docs/` is authoritative for architecture and engineering rules.

2. No subsystem may bypass another subsystem's public surface without written justification.

3. Public API changes must update the relevant documentation.

4. New shared utilities in `lib/` require proof they are truly cross-subsystem.

5. Architecture-specific code must remain in `arch/`.

6. Userspace ABI-visible changes must update `docs/ABI_STRATEGY.md`
   or a more specific ABI document.

7. Non-trivial concurrency code requires comments covering:

   - locking requirements  
   - memory ordering  
   - interrupt discipline  
   - ownership transfer  

8. Performance-sensitive code requires measurable rationale.

9. Compatibility claims require test evidence.

10. Comments must follow `docs/COMMENTING_GUIDE.md`.

---

# Deterministic Phase Discipline

Development proceeds in explicit phases.

A phase is complete only when its completion rule is met.

No subsystem may introduce behavior that belongs to a future phase.

Examples:

- Scheduler policy must not appear before scheduler structure exists.
- Context switching must not appear before the context-switch contract exists.
- Idle-thread behavior must not imply power management or timer policy.
- Userspace behavior must not appear before the userspace transition boundary exists.
- File descriptor or filesystem behavior must not appear before Phase 6 begins.

Violations of phase discipline must be rejected.

---

# Single-Surface Rule

Each task must modify exactly one subsystem surface.

A surface is:

- a public header
- a contract definition
- a subsystem entry point
- a defined architectural boundary

A task may add supporting implementation code for that surface.

A task must not:

- modify unrelated subsystems
- introduce cross-cutting helpers
- expand subsystem responsibilities prematurely

This rule preserves reviewability and prevents architectural drift.

---

# Init Transition Boundary Rule

The init transition boundary defines the first structural userspace launch composition point.

This boundary is implemented by:

include/kernul/init.h  
core/init.c

This surface is the canonical init boundary.

It is not a helper.

It is not an early boot utility.

It is the single structural transition point between kernel initialization
and userspace execution readiness.

---

Required behavior of the init boundary:

The boundary must:

- allocate a process
- allocate an address space
- associate the address space with the process
- allocate an ELF image
- associate the ELF image with the process and address space
- perform structural validation of the image
- stop deterministically

The boundary must not:

- perform userspace execution
- perform context switching into userspace
- perform ELF mapping
- perform relocation
- perform paging or MMU behavior
- introduce scheduler policy
- introduce userspace compatibility claims

---

Deterministic stop condition:

After structural composition and validation:

init_launch() must return:

KERN_ENOSYS

This return value is a phase boundary marker.

It is not an error.

It is a correctness signal.

---

Architectural constraint:

Only one init transition boundary may exist.

Future init-related behavior must:

- layer around this boundary
- extend behavior after this boundary
- respect its deterministic stop condition

Future code must not:

- duplicate init logic
- bypass this boundary
- redefine process launch sequencing

---

# Scheduler Boundary Rule

The scheduler is initialized in stages.

The following boundaries are mandatory:

Idle initialization boundary:

- `idle_init()` creates and enqueues the idle thread
- the idle thread is not yet the current thread

Scheduler bootstrap boundary:

- the initial current thread is established only by a context switch
- no subsystem may assume a valid current thread before that switch occurs

These boundaries must remain explicit.

---

# Architecture Contract Rule

Architecture code defines hardware behavior.

Common code must never:

- assume register layout
- assume calling conventions
- assume interrupt behavior
- assume memory ordering guarantees
- invent architecture state structures

All CPU execution state must flow through:

arch_cpu_state  
arch_cpu_state_init()  
arch_cpu_state_switch()

These are the only valid context-switch surfaces.

---

# Idle Thread Invariant

The idle thread is the scheduler fallback execution thread.

The following properties are mandatory:

- permanent lifetime
- reserved thread identity
- scheduler fallback eligibility
- non-reapable
- non-terminating

State rules:

When not selected:

Idle thread must be:

THREAD_RUNNABLE

When selected:

Idle thread becomes:

THREAD_RUNNING

Invariant:

The scheduler must always have at least one runnable thread.

This invariant is structural.

It must not be implemented through scheduling policy.

---

# Run Queue Integrity Rule

The run queue is a structural data model.

It must not encode scheduling policy.

The run queue defines:

- thread containment
- queue membership
- structural ordering

The run queue must not define:

- priority selection
- fairness policy
- load balancing
- preemption logic

Policy belongs to scheduler behavior phases.

---

# Context-Switch Safety Rule

A context switch must obey strict preconditions.

At the current phase:

The caller must:

- hold the run queue lock

Future phases may additionally require:

- interrupts disabled
- CPU-local scheduling context

Lock ownership and interrupt discipline are not interchangeable.

Violations of these preconditions are correctness failures.

---

# Compile-Time Contract Verification Rule

All architectural contract surfaces must be mechanically verifiable.

Verification methods include:

- compile-time assertions
- type-check translation units
- object-only compilation tests

Runtime behavior is not required for contract validation.

---

# Bootstrap Visibility Rule

Initialization must establish clear visibility boundaries.

A structure becomes visible to other subsystems only after:

- initialization completes
- required invariants hold
- ownership is defined

Publishing partially initialized state is prohibited.

---

# Portability Drift Warning

Architectural drift begins when:

- page-table manipulation is unified too early
- barriers are hidden behind vague helpers
- interrupt paths are forced into identical shapes
- memory-ordering assumptions leak into common code
- generic helpers appear without a hardware reason
- architecture logic migrates into common subsystems
- correctness depends on undocumented hardware behavior

Reject these changes unless there is a concrete machine-level need.

---

# Reviewability Rule

Every change must leave the repository:

- buildable
- testable
- deterministic
- mechanically reviewable
- scope-bounded

If a reviewer cannot determine correctness by inspection and tests,
the change is incomplete.

---

# Stability Rule

Public contracts are stable once introduced.

Changes to existing contracts require:

- explicit documentation update
- test update
- compatibility justification

Silent contract drift is prohibited.
