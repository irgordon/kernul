# KERNUL Roadmap

Status: Pre-Alpha  
Scope: Bootstrap, session lifecycle, and lifecycle governance
Purpose: Define implementation order and stop-safe priorities

Current implementation checkpoint: Phase 46, governance consistency verification.
Phase 45 remains the latest kernel contract surface.
The current changelog milestone is `v0.0.26`; the earlier milestone heading is
corrected to `v0.0.25`. These milestones do not establish runtime or compatibility
guarantees. Phase 46 below records the completed verification boundary.

The bootstrap descriptions below retain their original completion rules.
The execution and lifecycle ledgers record the boundaries actually implemented;
a contract or stub does not establish a working shell or userspace execution.

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

Status: Complete (structural contracts)

Evidence: `fd_table.h`, `vfs.h`, `pipe.h`, `signal.h`, `session.h`,
`terminal.h`, and `controlling_terminal.h`, with compile and link tests under
`tests/`. Runtime Unix I/O behavior remains outside these stub contracts.

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

Status: In Progress (structural boundaries implemented)

Interactive console, activation, readiness, admission, enqueue, dispatch,
execution-target, and switch-operand contracts exist in `include/kernul/` and
`core/`. A functional interactive shell, real context transfer, terminal job
control, and process-signal integration are still required by the completion
rule below.

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

# Compatibility expansion — deferred forecast

Status: Not Started

The original Phase 8 compatibility forecast was superseded by the execution
transfer boundary recorded in the implemented ledger below. Compatibility
expansion remains a future milestone and has no assigned phase number.

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

# Implemented execution boundary ledger (Phases 8–14)

These are implemented contract and stub boundaries, not claims of working
userspace execution or shell compatibility.

| Phase | Implemented boundary | Primary surface |
| :--- | :--- | :--- |
| 8 | Execution transfer and explicit selection | `interactive_execution.h`, `interactive_selection.h` |
| 9 | Scheduler state, runnable container, and container selection | `interactive_scheduler_state.h`, `interactive_scheduler_container.h` |
| 10 | Selection handoff, transfer operands, and initiation gate | `interactive_execution_handoff.h`, `interactive_execution_transfer_initiation_gate.h` |
| 11 | Execution outcome publication, view, and aggregation | `interactive_execution_outcome_record.h`, `interactive_execution_outcome_view.h`, `interactive_execution_outcome_aggregate.h` |
| 12 | Failure and completion acknowledgment gates | `interactive_execution_failure_ack_gate.h`, `interactive_execution_completion_ack_gate.h` |
| 13 | Acknowledgment-driven initiation gating | `interactive_execution_failure_initiation_gate.h`, `interactive_execution_completion_initiation_gate.h` |
| 14 | Terminal session state and cause publication | `session_terminal_state.h`, `session_terminal_cause.h` |

---

# Phase 46 — Governance consistency verification

Status: Complete

Purpose:

Verify that the existing immutable governance tables agree across their public
surfaces before adding another lifecycle or execution boundary.

Surface:

- `tests/governance/test_governance_consistency.c`
- `make test-governance`, also required by `make test`

Acceptance criteria:

- The index, provenance, and compatibility tables agree on ordered phase
  identities and surface names for Phases 31–40.
- Every compatibility entry references `LIFECYCLE_FREEZE_VERSION`.
- Structured schema entries resolve to the same names in the index or manifest.
  Phases 32 and 35 have no structured payload and are intentionally absent from
  the schema; its canonical structured-surface count remains eleven.
- The manifest binds Phases 41–44 to their respective index, provenance,
  compatibility, and schema roles.
- A mismatched identity, freeze version, or role fails verification.
- Runtime test failures propagate as a nonzero `make test` result.
- The strict freestanding build and hosted test suite pass with the local
  compiler. Metadata used only by compile-time checks remains warning-clean.

Constraints:

- Hosted verification only; no new kernel API or lifecycle storage.
- No session or scheduler linkage, authority, scheduling, retry automation, or
  userspace execution.
- Existing frozen contracts and table contents retain their semantics.

Phase completion rule:

The cross-surface checks pass, deliberate metadata drift is rejected, and a
runtime test failure is demonstrated to fail the test command.

Verification evidence (2026-10-21):

- `make -B all` passes with strict freestanding warnings enabled.
- `make -B test` passes 100 compile checks and 78 runtime test invocations.
- Seven isolated mutations of phase identities, names, freeze association, and
  role binding are rejected by the integration test.
- Injected command failures return nonzero status in all 77 pre-existing
  runtime test recipes.

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

# Canonical implemented phase ledger (lifecycle and governance boundaries)

Implemented canonical sequence:

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
11. Phase 26 — retry execution (single-attempt)
12. Phase 27 — retry execution result publication (final)
13. Phase 28 — retry outcome recording
14. Phase 29 — retry outcome observation (read-only)
15. Phase 30 — retry lifecycle freeze and architectural consolidation
16. Phase 31 — read-only lifecycle introspection
17. Phase 32 — passive lifecycle invariant verification
18. Phase 33 — read-only lifecycle provenance
19. Phase 34 — diagnostic lifecycle boundary assertion
20. Phase 35 — compile-time lifecycle freeze enforcement
21. Phase 36 — deterministic lifecycle documentation synthesis
22. Phase 37 — snapshot-scoped lifecycle audit
23. Phase 38 — deterministic lifecycle export serialization
24. Phase 39 — inert lifecycle extension declaration
25. Phase 40 — inert lifecycle governance consolidation
26. Phase 41 — governance index for Phases 31–40
27. Phase 42 — governance provenance for Phases 31–40
28. Phase 43 — governance compatibility for Phases 31–40
29. Phase 44 — governance schema for structured surfaces in Phases 31–43
30. Phase 45 — governance manifest for metadata surfaces in Phases 41–44

Phase 25 is the historical freeze through retry authorization; Phases 26–30
explicitly introduced and then froze the remaining retry boundaries. The
canonical lifecycle numbering above takes precedence over earlier forecasts.
Earlier phase maps may use separate historical numbering schemes; new work
uses the canonical implementation phases recorded here.

Lifecycle stopping points (frozen):

- Recovery lifecycle is complete through outcome observation.
- Retry lifecycle is complete through outcome observation.
- No additional lifecycle behavior may be introduced without a new phase.
- Existing semantics must not be reinterpreted or extended.

Semantic freeze invariants:

Recovery:
- Recovery execution is single-attempt.
- Recovery outcome is immutable once published.
- Outcome observation is read-only and readiness-gated.

Retry:
- Retry lifecycle consists of exactly:
  1) retry execution;
  2) retry execution result publication;
  3) retry outcome recording;
  4) retry outcome observation.
- Observation is passive and read-only.
- Retry policy remains declarative meaning only.
- Retry authorization remains single-use authority only.
- No retry scheduling exists.
- No retry coordination exists.
- No retry escalation exists.
- No retry loops exist.
- No retry policy automation exists.
- No retry outcome reaction exists.
- Observation does not imply handling.
- Outcome existence does not imply action.

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
- No documentation may describe retry scheduling, coordination, escalation,
  loops, policy automation, or outcome reaction as existing.

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
