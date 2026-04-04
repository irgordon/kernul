# KERNUL Roadmap

Status: Pre-Alpha  
Scope: Bootstrap through early compatibility milestones  
Purpose: Define implementation order and stop-safe priorities

---

# Phase 0 — Foundation

Status: Complete

Deliver:

- repository structure
- architecture guide
- coding standard
- commenting guide
- ABI strategy
- baseline build system
- baseline test harness
- boot contract (boot_info)
- panic path implementation
- kernel entry validation path (kernel_main)
- architecture doctrine updates
- memory ordering doctrine (MEMORY_ORDERING.md)

Phase completion rule:

The kernel must have a deterministic bootstrap entry path and documented architectural laws.  
No hardware-specific behavior is required.

---

# Phase 1 — Architecture boundary stabilization

Status: Complete

Purpose:

Freeze the smallest real hardware-dependent boundaries before expanding behavior.  
This phase prevents architectural drift and ensures portability across x86_64 and AArch64.

Deliver:

- architecture boundary header
- arch_halt() contract
- arch_early_init() contract
- arch_cpu_state placeholder type
- minimal x86_64 arch_halt() implementation
- minimal AArch64 arch_halt() implementation
- minimal arch_early_init() stubs
- panic path redirected to arch_halt()

Constraints:

- no interrupt handling yet
- no MMU enablement
- no scheduler assumptions
- no device initialization
- no platform discovery logic

Phase completion rule:

All terminal execution passes through a defined architecture boundary.

---

# Phase 2 — Memory and ordering foundations

Status: Complete

Purpose:

Establish deterministic memory and synchronization vocabulary before real runtime behavior expands.

Deliver:

- atomic.h contract surface
- barrier.h contract surface
- physical memory region structure
- memory map handoff contract
- PMM public interface
- PMM initialization stub
- deterministic memory structure tests

Constraints:

- no complex allocation yet
- no paging policy
- no SMP behavior
- no performance tuning

Phase completion rule:

Memory representation and ordering semantics are frozen before runtime complexity grows.

---

# Phase 3 — Core runtime primitives

Status: Complete

Purpose:

Create the smallest stable internal vocabulary required by all later subsystems.

Deliver:

- types.h
- errors.h
- assert.h
- assert_fail() implementation
- intrusive list structure
- minimal spinlock contract
- deterministic primitive tests

Constraints:

- no lockless algorithms
- no scheduler logic
- no advanced synchronization patterns

Phase completion rule:

Core kernel primitives exist and are mechanically reviewable.

---

# Phase 4 — Execution and scheduling foundations

Status: Complete

Purpose:

Define execution structure before introducing scheduling policy.

Completed surfaces:

- thread structure
- thread state model
- run queue structure
- scheduler public interface
- scheduler initialization stub
- enqueue and dequeue stubs
- context-switch contract surface
- idle thread contract

Constraints:

- no advanced scheduling policy
- no load balancing
- no performance tuning
- no preemption
- no SMP behavior

Scheduler bootstrap boundary:

The initial current thread is established by the context-switch bootstrap path defined in the context-switch contract surface.  
No scheduler behavior may assume a valid current thread before that boundary is executed.

Phase completion rule:

Thread lifecycle, scheduler entry points, and context-switch surfaces exist and are mechanically reviewable without policy complexity.

---

# Phase 5 — Userspace transition

Status: Complete

Purpose:

Create the first controlled boundary between kernel and userspace.

Completed surfaces:

- syscall entry contract
- syscall dispatch stub
- process structure
- address space ownership contract
- ELF loader contract
- init process launch path

Deterministic init transition boundary:

The userspace transition boundary is implemented by:

- include/kernul/init.h
- core/init.c

This boundary composes:

- process allocation
- address-space allocation
- structural ownership association
- ELF image allocation
- ELF image validation

and then stops deterministically.

Failure behavior:

- KERN_ENOMEM — allocation failure
- KERN_ENOSYS — deterministic phase boundary stop

Execution of userspace is intentionally not implemented at this phase.

Constraints:

- no compatibility claims
- no userspace execution
- no paging behavior
- no loader execution semantics
- no process policy
- no performance optimization

Phase completion rule:

A deterministic userspace entry boundary exists.

---

# Phase 6 — Unix base semantics

Status: Not Started

Purpose:

Establish the structural foundation required for Unix-style process and I/O behavior.

Deliver:

- file descriptor table
- VFS core contract
- pipe contract
- signal contract
- session and process group structures
- terminal and PTY contract surfaces

Constraints:

- no filesystem feature breadth
- no networking behavior expansion
- no compatibility claims

Phase completion rule:

Core Unix semantics exist structurally and are testable.

---

# Phase 7 — Shell viability

Status: Not Started

Purpose:

Enable interactive shell behavior through minimal, deterministic kernel support.

Deliver:

- interactive console path
- terminal control behavior
- job control support
- process signal integration
- regression tests for shell-critical behavior

Constraints:

- no advanced userland tooling
- no feature expansion beyond shell requirements

Phase completion rule:

The kernel supports a functional interactive shell path.

---

# Phase 8 — Compatibility expansion

Status: Not Started

Purpose:

Gradually increase userspace compatibility based on verified behavior.

Deliver:

- additional syscall coverage
- filesystem compatibility improvements
- device and driver expansion
- compatibility regression testing
- performance tuning based on measurement

Constraints:

- no compatibility claims without test evidence
- no premature optimization

Phase completion rule:

Compatibility is demonstrated through measurable behavior, not assumption.

---

# Global engineering rules

Rule 1 — One task, one surface  
Each task must modify only one subsystem surface.

Rule 2 — Shape before behavior  
Define structures before implementing policy.

Rule 3 — Behavior before optimization  
Optimize only after correctness is established.

Rule 4 — Real hardware boundaries only  
Keep a boundary only when the hardware is genuinely different.  
If the boundary exists only to make code look cleaner, reject it.

Rule 5 — Common code may not assume architectural behavior not explicitly stated in a contract.

Rule 6 — No speculative expansion  
Do not add fields, helpers, or abstractions until the next task requires them.

Rule 7 — Deterministic stop condition  
At the end of each task, the repository must remain:

- buildable
- reviewable
- deterministic
- scope-bounded

---

# Canonical implemented phase ledger (session lifecycle boundaries)

Implemented canonical sequence (frozen for this run):

1. Phase 15 — session finalization publication
2. Phase 16 — ownership declaration
3. Phase 17 — reclamation
4. Phase 18 — recovery eligibility
5. Phase 19 — recovery authorization (single-use)
6. Phase 20 — recovery execution (single-attempt)
7. Phase 21 — recovery outcome recording (immutable)
8. Phase 22 — recovery outcome observation (read-only)
9. Phase 23 — retry policy declaration (meaning only)
10. Phase 24 — retry authorization (single-use capability)

Lifecycle stopping points (frozen):

- Recovery lifecycle is complete through outcome observation.
- Retry lifecycle is complete through authorization.
- No additional lifecycle behavior may be introduced without a new phase.
- Existing semantics must not be reinterpreted or extended.

Semantic freeze invariants:

Recovery:
- Recovery execution is single-attempt.
- Recovery outcome is immutable once published.
- Outcome observation is read-only and readiness-gated.

Retry:
- Retry policy is declarative meaning only.
- Retry authorization is single-use authority only.
- No retry execution exists.
- No retry scheduling exists.
- No retry coordination exists.

Authority freeze invariants:

- Authorization surfaces grant capability only.
- Authorization surfaces do not execute behavior.
- Authorization surfaces do not schedule work.
- Authorization surfaces do not coordinate subsystems.

Dependency freeze invariants:

No recovery or retry component may newly depend on:
- scheduler subsystems
- retry execution logic
- coordination or signaling systems
- time-based backoff or timers

Any such dependency requires a new phase.

Storage helper freeze invariants:

All storage helpers in the recovery/retry subsystem must remain:
- mechanical only
- load/store/CAS with explicit ordering
- non-semantic
- non-coordinating

Helpers must not grow:
- policy inference
- readiness checks
- derived-state logic
- convenience APIs

Observation freeze invariants:

All observation APIs must remain:
- readiness-gated
- acquire-ordered
- side-effect free
- non-allocating
- non-mutating

Observation must never:
- trigger behavior
- grant authority
- schedule work

Known forecast supersessions:

- Forecast Phase 19 retry action was superseded by retry authorization.
- Forecast Phase 20 scheduler observation was superseded by recovery execution.
- No documentation may describe retry execution or retry scheduling as existing.

---

## Stub definition

A stub provides the minimal behavior required to satisfy a declared interface and maintain repository buildability.

A stub may:

- return fixed values
- perform structural state transitions
- enforce contract assertions

A stub must not:

- introduce policy decisions
- introduce timing behavior
- introduce performance assumptions
- depend on concurrency not yet defined
