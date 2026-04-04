# Changelog

All notable changes to KERNUL are documented in this file.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

KERNUL does not yet follow Semantic Versioning. Version numbers reflect
phase milestones. v0.0.0 marks the completion of Phase 0 through Phase 4.

---

## [v0.0.2] — 2026-04-02

### Added — Phase 15: Terminal lifecycle action boundary

#### Unreleased — Phase 16, Task 1 — Session resource ownership declaration

- Introduced a session resource ownership registry via
  `include/kernul/session_ownership.h` and `core/session_ownership.c`.
- Ownership is declarative, idempotent, and monotonic:
  repeated registration of the same `resource_id_t` returns
  `SESSION_OWNERSHIP_REGISTER_ALREADY_PRESENT` with no duplicate publication.
- Ownership storage is explicitly capacity-bounded with fixed per-session
  capacity (`SESSION_OWNERSHIP_FIXED_CAPACITY`) and deterministic overflow
  (`SESSION_OWNERSHIP_REGISTER_CAPACITY_EXCEEDED`).
- Ownership declaration is informational only and does not imply allocation
  authority, reclamation intent, cleanup, retry, scheduling, or lifecycle
  behavior.

#### Unreleased — Phase 17, Task 1 — Session resource reclamation (explicit, local)

- Introduced explicit session resource reclamation via
  `include/kernul/session_reclamation.h` and `core/session_reclamation.c`.
- Reclamation is finalized-only and ownership-bounded:
  `session_reclaim_resource_if_finalized(struct session *, resource_id_t)`
  returns deterministic guard outcomes for NULL, not-finalized, and not-owned
  paths.
- Reclaim primitive contract is constrained to deterministic, synchronous,
  idempotent-per-identity behavior with external effect visibility on return;
  no deferred teardown or hidden completion path is introduced.
- Reclaim primitive behavior is identity-local and must not depend on session
  lifecycle state beyond the provided resource identity.
- Reclamation attempts do not mutate the ownership registry; ownership remains
  historical and observable after both success and failure.
- Reclamation failure does not reopen lifecycle transitions and does not alter
  finalization state.
- No retries, scheduling, coordination, or new synchronization channels are
  introduced.

#### Unreleased — Phase 18, Task 1 — Session recovery eligibility declaration

- Introduced declarative recovery eligibility via
  `include/kernul/session_recovery_eligibility.h` and
  `core/session_recovery_eligibility.c`.
- Recovery eligibility is finalized-only, single-assignment, and tri-state:
  `SESSION_RECOVERY_ELIGIBILITY_UNSET`,
  `SESSION_RECOVERY_ELIGIBLE`, and
  `SESSION_RECOVERY_INELIGIBLE`.
- Declaration is informational only and does not initiate recovery or alter
  lifecycle state.
- No retries, scheduling, coordination, or lifecycle mutation behavior was
  introduced by this boundary.

#### Unreleased — Phase 19, Task 1 — Explicit recovery attempt authorization

- Introduced explicit recovery attempt authorization via
  `include/kernul/session_recovery_authorization.h` and
  `core/session_recovery_authorization.c`.
- Authorization is finalized-only, eligibility-gated, single-use, and
  irreversible.
- `AUTHORIZED` is capability-only: permission to attempt recovery, not a
  trigger or scheduling signal.
- Consumption is explicit, safe, idempotent, and monotonic through terminal
  `CONSUMED` state.
- No retries, coordination, scheduling, recovery execution logic, or lifecycle
  mutation behavior was introduced by this boundary.

#### Unreleased — Phase 20, Task 1 — Explicit recovery execution (single attempt)

- Introduced explicit single-attempt recovery execution via
  `include/kernul/session_recovery_execution.h` and
  `core/session_recovery_execution.c`.
- Execution consumes authorization before recovery attempt and is irreversible.
- Execution effects are synchronous and visible before
  `session_execute_recovery(struct session *)` returns.
- Execution outcome is informational only and does not mutate eligibility or
  authorization state beyond the authorization consumption transition.
- Recovery execution primitive is constrained from invoking authorization or
  eligibility APIs.
- No retries, scheduling, coordination, or policy evaluation behavior was
  introduced by this boundary.

#### Unreleased — Phase 21, Task 1 — Recovery outcome recording

- Introduced immutable recovery outcome recording via
  `include/kernul/session_recovery_outcome.h` and
  `core/session_recovery_outcome.c`.
- Recovery outcome storage is initialized to
  `SESSION_RECOVERY_NOT_ATTEMPTED` before session visibility.
- Outcome recording is single-assignment, terminal, and published only after
  recovery execution completion is observed with acquire semantics.
- Outcome publication and observation use explicit release/acquire visibility.
- Outcome is declarative lifecycle meaning only and introduces no authority,
  retry, scheduling, or coordination behavior.

#### Unreleased — Phase 21, Task 1.5 — Hardening: readiness boundary, authority purity, visibility contracts

- Hardened session publication boundaries by making slot reservation and
  observer-visible readiness explicit and distinct via
  `session_publish_ready_release(struct session *)` and
  `session_is_ready_acquire(const struct session *)`.
- Enforced readiness-gated observation: lifecycle observers and identity
  getters treat not-ready the same as NULL-safe defaults and do not derive
  authoritative lifecycle facts from those values.
- Hardened ownership visibility contract as normative: entry writes publish
  through release `ownership.count`, and readers acquire `ownership.count`
  before scanning only `[0, count-1]`.
- Hardened recovery execution authority purity: authorization consumption is
  the sole authority gate for execution attempt.
- Hardened completion/result contract: completion is release-published,
  completion observation is acquire, and post-completion result is stable and
  visible to acquire observers.
- Hardened recovery outcome recording path: execution result is observed once
  per call and single-assignment authority is centralized in CAS publish helper.
- Added storage-helper hardening constraints and include-ban tests to keep
  execution/outcome storage helpers mechanical-only and isolated from semantic
  lifecycle/retry/scheduler/coordination surfaces.
- Canonical implemented phase sequence is:
  Phase 15 finalization publication; Phase 16 ownership declaration;
  Phase 17 reclamation; Phase 18 recovery eligibility;
  Phase 19 recovery authorization; Phase 20 recovery execution;
  Phase 21 recovery outcome recording.
- Forecast deviation note: Phase 19 introduced recovery authorization semantics
  in place of the originally forecast retry action, and Phase 20 introduced
  bounded recovery execution in place of the originally forecast scheduler
  observation surface; the scheduler observation surface remains unimplemented
  and unassigned to a phase.

#### Unreleased — Phase 22, Task 1 — Recovery outcome observation surface

- Introduced read-only recovery outcome observation snapshot via
  `include/kernul/session_recovery_outcome_observe.h` and
  `core/session_recovery_outcome_observe.c`.
- Observation is readiness-gated and acquire-ordered for readiness, completion,
  execution result observation, outcome state, and timestamp reads.
- Snapshot reports immutable recovery outcome, outcome publication timestamp,
  and execution attempt count.
- Attempt count is derived from completion observation only and remains bounded
  to `0` or `1`.
- Outcome timestamp is immutable metadata published with outcome and never
  mutated afterward.
- Observation is passive and introduces no authority, retries, scheduling, or
  coordination behavior.

#### Unreleased — Phase 23, Task 1 — Retry policy declaration (meaning only)

- Introduced declarative retry policy marker via
  `include/kernul/session_retry_policy.h` and
  `core/session_retry_policy.c`.
- Retry policy storage is initialized to `SESSION_RETRY_UNSET` before session
  readiness publication.
- Retry policy declaration is readiness-gated, NULL-safe, atomic, and
  single-assignment with monotonic transitions:
  `SESSION_RETRY_UNSET` -> `SESSION_RETRY_ALLOWED` or
  `SESSION_RETRY_UNSET` -> `SESSION_RETRY_FORBIDDEN`.
- Retry policy observation is readiness-gated via centralized session
  observation policy and is side-effect free.
- No authority, retry execution, scheduling, coordination, or recovery outcome
  mutation behavior was introduced by this boundary.

#### Phase 15, Task 1 — Terminal session finalization publication

- Introduced terminal session finalization marker publication through
  `include/kernul/session_finalization.h` and
  `core/session_finalization.c`.
- Finalization publication is explicit-invocation only and terminal-guarded:
  `session_publish_finalized_if_terminal(struct session *)` is a no-op unless
  `session_is_terminal_acquire(const struct session *)` is true.
- Finalized marker publication is single-assignment, monotonic, and
  irreversible for session lifetime; terminal does not imply finalized.
- Finalized marker storage is session-owned and atomically ordered with
  explicit release publication and acquire observation semantics.
- No cleanup, retry/restart, scheduling, or additional lifecycle actions are
  introduced by this boundary.

---

## [v0.0.1] — 2026-04-01

### Added — Phase 14: Policy composition

#### Phase 14, Task 1 — Terminal session state publication

- Introduced terminal session state publication through
  `include/kernul/session_terminal_state.h` and
  `core/session_terminal_state.c`.
- Terminal state is derived solely from initiation-forbidden control through
  `interactive_execution_initiation_is_permitted_acquire(const struct session *)`.
- Terminal state is declarative lifecycle signaling only:
  no retries, no scheduling changes, and no lifecycle action behavior.
- Terminal state is monotonic and irreversible for session lifetime with no
  reset path.
- Terminal publication and observation use explicit release/acquire visibility.

#### Phase 14, Task 2 — Terminal cause classification (declarative)

- Introduced terminal cause classification through
  `include/kernul/session_terminal_cause.h` and
  `core/session_terminal_cause.c`.
- Classification is declarative and informational only; no retries, cleanup,
  scheduling, or lifecycle actions are driven by terminal cause.
- Terminal cause publication is terminal-state-guarded and is a no-op when
  `session_is_terminal_acquire(const struct session *)` is false.
- Terminal cause is monotonic and irreversible for session lifetime:
  initialized to `SESSION_TERMINAL_CAUSE_UNSPECIFIED`, then published once with
  no reset or override path.
- Terminal cause publication and observation use explicit release/acquire
  visibility semantics.

This Phase 14 boundary step is kernel-internal only and is not a userspace ABI.

---

## [v0.0.0] — 2026-03-24

### Summary

Pre-alpha milestone. Covers Phase 0 (Foundation) through Phase 4
(Execution and scheduling foundations). No userspace ABI is exposed.
No real hardware switching occurs. The kernel is not bootable on
physical hardware at this version.

---

### Added — Phase 0: Foundation

- `docs/ROADMAP.md` — phase-structured implementation plan with global
  engineering rules, stub definition, and phase completion rules
- `docs/CODING_STANDARD.md` — authoritative C coding standard covering
  naming, prefixing discipline, formatting, error handling, and review
  questions
- `docs/COMMENTING_GUIDE.md` — rules for what comments must explain,
  file header format, public function comment contract, and STUB marker
  convention
- `docs/ABI_STRATEGY.md` — compatibility target, ABI boundary definition,
  syscall stability rules, and phased ABI expansion plan
- `docs/MEMORY_ORDERING.md` — authoritative memory ordering vocabulary
  (relaxed, acquire, release, acquire-release, full barrier, compiler
  barrier), publication rule, visibility rule, wakeup and handoff rule,
  MMIO caution, and minimal doctrine
- `include/kernul/types.h` — kernel primitive types (`u8`–`u64`,
  `s8`–`s64`, `uptr`, `sptr`, `ssize_t`, `phys_addr_t`, `virt_addr_t`,
  `kern_err_t`)
- `include/kernul/compiler.h` — compiler annotations (`KERN_NORETURN`,
  `KERN_NOINLINE`, `KERN_ALWAYS_INLINE`, `KERN_PACKED`, `KERN_ALIGNED`,
  `KERN_UNUSED`, `KERN_WEAK`, `KERN_LIKELY`, `KERN_UNLIKELY`,
  `KERN_STATIC_ASSERT`, `KERN_ARRAY_SIZE`, `KERN_CONTAINER_OF`)
- `include/kernul/boot.h` — `boot_info_t` memory map handoff contract,
  `phys_region_t`, `BOOT_MAX_PHYS_REGIONS`, `boot_info_validate()`
- `init/boot.c` — `boot_info_validate()` implementation
- `init/main.c` — `kernel_main()` entry path; validates boot contract,
  initializes PMM, halts deterministically
- `Makefile` — freestanding kernel build and hosted test build targets
- `tests/test_boot.c` — 12 assertions covering boot contract validation
  and PMM structure tests

---

### Added — Phase 1: Architecture boundary stabilization

- `include/kernul/arch.h` — three mandatory architecture boundary surface
  declarations: `arch_halt()` (§2.9.1), `arch_early_init()` (§2.9.2),
  `arch_cpu_state_t` forward declaration (§2.9.3)
- `arch/stub/arch.c` — stub `arch_halt()` (infinite loop) and
  `arch_early_init()` (no-op) for hosted build verification
- `arch/x86_64/arch.c` — `arch_halt()` via `cli` + `hlt` loop;
  `arch_early_init()` no-op for standard bootloader environments
- `arch/aarch64/arch.c` — `arch_halt()` via `msr daifset` + `wfi` loop;
  `arch_early_init()` sets `VBAR_EL1` to early halt vectors
- `arch/stub/cpu_state.h` — `struct arch_cpu_state` placeholder (Phase 1
  scope: non-zero size, no register fields)
- `arch/x86_64/cpu_state.h` — placeholder (Phase 1 scope)
- `arch/aarch64/cpu_state.h` — placeholder (Phase 1 scope); documents
  that Phase 4 switch implementation must include `DSB ISH + ISB`

---

### Added — Phase 2: Memory and ordering foundations

- `include/kernul/atomic.h` — atomic contract surface: `atomic32_t`,
  `atomic64_t`, `atomicptr_t`; operations with ordering suffixes
  (`_relaxed`, `_acquire`, `_release`); `cmpxchg`, `xchg`, `add`, `dec`,
  `or`, `and`
- `include/kernul/barrier.h` — barrier contract surface:
  `barrier_compiler()`, `barrier_acquire()`, `barrier_release()`,
  `barrier_full()` with cost and pairing documentation
- `arch/stub/atomic_impl.h` — single-CPU hosted stub implementations of
  all atomic operations via compiler barrier
- `arch/stub/barrier_impl.h` — stub barrier implementations reducing to
  compiler barrier
- `include/kernul/mm.h` — PMM public interface: `mm_phys_init()`,
  `mm_phys_alloc()`, `mm_phys_free()`, `mm_phys_free_count()`, page
  size constants and alignment macros
- `mm/mm.c` — PMM stub: bump allocator over usable memory range;
  `mm_phys_free()` is a documented no-op pending free-list implementation

---

### Added — Phase 3: Core runtime primitives

- `include/kernul/errors.h` — 13 named error codes (`KERN_OK`,
  `KERN_ENOMEM`, `KERN_EINVAL`, `KERN_ENOENT`, `KERN_EBUSY`,
  `KERN_ENOSYS`, `KERN_EFAULT`, `KERN_EPERM`, `KERN_EIO`, `KERN_EAGAIN`,
  `KERN_EDEADLK`, `KERN_EEXIST`, `KERN_ERANGE`, `KERN_ETIMEDOUT`) with
  usage rules; separated from `types.h`
- `include/kernul/assert.h` — `assert_fail()` declaration, `ASSERT(cond)`
  macro, no side effects on true condition
- `core/assert.c` — `assert_fail()` calls `arch_halt()`; accepts
  `expr`/`file`/`line` parameters with `(void)` suppression pending
  console subsystem
- `include/kernul/list.h` — intrusive doubly-linked circular list:
  `list_node_t`, `list_t`, `LIST_INIT`, `list_init()`, `list_empty()`,
  `list_add_head()`, `list_add_tail()`, `list_remove()`,
  `list_for_each()`, `list_for_each_safe()`, `list_entry()`
- `include/kernul/spinlock.h` — minimal spinlock contract: `spinlock_t`,
  `SPINLOCK_INIT`, `spin_init()`, `spin_lock()`, `spin_unlock()` with
  acquire/release ordering contract per `MEMORY_ORDERING.md §8`
- `core/spinlock.c` — single-CPU stub using volatile flag and
  `barrier_compiler()`; documents SMP replacement requirement
- `tests/test_list.c` — 14 assertions: init, FIFO/LIFO insertion order,
  removal, safe iteration, post-removal self-linkage
- `tests/test_spinlock.c` — 5 assertions: static init, runtime init,
  re-acquisition after unlock, write visibility, independence of two locks

---

### Added — Phase 4: Execution and scheduling foundations

#### Task 1 — Thread data shape

- `include/kernul/thread.h` — `thread_state_t` (6 states with documented
  valid transitions), `thread_prio_t` (5 named constants), `thread_id_t`,
  `THREAD_ID_INVALID`, `THREAD_NAME_MAX`, `thread_t` struct; documents
  that `thread.h` is bound common code via `-Iarch/<target>`
- `tests/test_thread.c` — 7 assertions: state value distinctness, priority
  ordering, zero-init behaviour, `list_entry` round-trip, name buffer
  capacity

#### Task 2 — Run queue data shape

- `include/kernul/runq.h` — `runq_t` (embedded `list_t`, embedded
  `spinlock_t`, `u32 count`), `runq_init()`; initialization visibility
  rule and post-init invariants documented; `RUNQ_INIT` macro deliberately
  omitted (LIST_INIT field-embedding limitation)
- `tests/test_runq.c` — 8 assertions: count zero, empty list, sentinel
  self-link, queue independence, `list_entry` field-offset round-trip

#### Task 3 — Scheduler API contract

- `include/kernul/sched.h` — five declarations with full §8 annotation
  (CONTEXT, SLEEP, ALLOC, LOCKS, OWNERSHIP, FAILURE, state transitions):
  `sched_init()`, `sched_enqueue()`, `sched_dequeue()`, `sched_yield()`,
  `sched_current()`
- `tests/test_sched_api.c` — compile-check translation unit; function
  pointer assignments verify signature types at compile time

#### Task 4 — Scheduler stub implementation

- `sched/sched.c` — implements all five declared entry points; single
  global `runq_t` and `current` pointer; `sched_enqueue` asserts
  `THREAD_RUNNABLE`; `sched_dequeue` removes from head; `sched_yield`
  performs state bookkeeping only (no context switch; STUB comment marks
  `arch_cpu_state_switch()` site); `sched_current` returns global pointer
- `tests/test_sched.c` — 8 assertions: init, double-init rejection, empty
  dequeue, single enqueue/dequeue, FIFO order across three threads,
  current NULL before any thread runs, single-thread yield round-trip;
  documents bootstrap gap (yield not reachable before Task 5)

#### Task 5 — Context-switch contract surface

- `arch/stub/cpu_state.h` — expanded from Phase 1 placeholder to full
  context-switch contract surface; `struct arch_cpu_state` with
  `KERN_STATIC_ASSERT` (baseline guard, not structural guarantee);
  `arch_cpu_state_init()` with preconditions, calling context, and no-op
  stub; `arch_cpu_state_switch()` with unambiguous interrupt/lock
  precondition (run queue lock required; interrupt disable deferred to
  preemptible ports), precise return semantics (resumes in execution
  context of `*prev`, not lexical call site), and no-op stub
- `tests/test_cpu_state_api.c` — compile-check translation unit;
  function pointer assignments verify both signatures

#### Task 6 — Idle thread contract

- `include/kernul/idle.h` — idle thread identity (fixed `thread_t`
  properties), lifetime rule (permanent; THREAD_RUNNABLE when not current;
  THREAD_RUNNING when selected; never ZOMBIE or DEAD), initialization
  contract, scheduler start sequence relationship (enqueued by
  `idle_init()`; bootstrap to current deferred), explicit statement that
  idle execution is not scheduling policy; `THREAD_ID_IDLE = 1`;
  `idle_init()` and `idle_thread()` declarations
- `sched/idle.c` — static `thread_t idle`, static 4096-byte 16-byte-aligned
  stack; `idle_entry()` non-returning spin loop with explicit no-policy
  comment; `idle_init()` zero-inits, sets identity fields, calls
  `arch_cpu_state_init()`, calls `sched_enqueue()`; `idle_thread()` with
  init guard assertion
- `tests/test_idle.c` — 12 assertions: init succeeds, pointer valid,
  identity fields (`id`, `prio`), name characters, THREAD_RUNNABLE state
  after init, queue visibility (proves enqueue succeeded; does not claim
  bootstrap complete), `THREAD_ID_IDLE ≠ THREAD_ID_INVALID`, priority
  ordering against all other defined levels

#### Phase 5 boundary step — Syscall entry contract surface

- `include/kernul/syscall.h` — first controlled userspace-to-kernel boundary
  contract surface; declares architecture-owned `arch_syscall_enter()` and
  common-owned `sys_dispatch()` with explicit calling context, ownership, and
  kernel-internal ABI-boundary statement
- `core/syscall_dispatch.c` — deterministic `sys_dispatch()` stub that returns
  fixed `KERN_ENOSYS` for all inputs; no syscall table, no registration
  mechanism, no per-syscall handlers
- `arch/x86_64/arch_syscall_enter.c` — x86_64 architecture-owned
  `arch_syscall_enter()` stub that unconditionally terminates via `arch_halt()`
- `arch/aarch64/arch_syscall_enter.c` — AArch64 architecture-owned
  `arch_syscall_enter()` stub that unconditionally terminates via `arch_halt()`
- `tests/syscall/test_syscall_contract_compile.c` — compile-check translation
  unit validating syscall contract declaration presence and signatures
- `tests/syscall/test_sys_dispatch_link.c` — link-check for `sys_dispatch`
  symbol presence
- `tests/arch/x86_64/test_arch_syscall_enter_link.c` — link-check for x86_64
  `arch_syscall_enter` symbol presence
- `tests/arch/aarch64/test_arch_syscall_enter_link.c` — link-check for AArch64
  `arch_syscall_enter` symbol presence

This step defines boundary shape only. It does not implement userspace
execution, ABI compatibility claims, syscall dispatch policy, real trap
handling, or return-to-userspace behavior.

#### Phase 5 boundary step — Kernel process structure contract

- `include/kernul/process.h` — kernel-internal process structure contract
  defining structural process identity (`process_id_t`, `PROCESS_ID_INVALID`),
  structural lifecycle states (`PROCESS_STATE_CREATED`, `PROCESS_STATE_ALIVE`,
  `PROCESS_STATE_ZOMBIE`, `PROCESS_STATE_DEAD`), shared lifetime model
  (`refcount`), and structural relationships (`parent`, `threads`,
  `address_space`, `children`)
- `core/process.c` — deterministic lifecycle helper stubs:
  `process_alloc()` (fixed NULL failure stub), `process_get()` (reference
  acquisition), `process_put()` (reference release)
- `tests/process/test_process_contract_compile.c` — compile-check translation
  unit validating declaration visibility, helper signatures, and visible
  process struct field layout in kernel code
- `tests/process/test_process_link.c` — link-check for process lifecycle helper
  symbol presence

This step defines structural process identity only as a Phase 5 boundary.
It does not implement address-space management, scheduling behavior, userspace
execution semantics, process creation policy, IPC, signals, or file descriptor
semantics.

#### Phase 5 boundary step — Address-space ownership contract

- `include/kernul/address_space.h` — kernel-internal address-space contract
  defining ownership and association shape: structural identity
  (`address_space_id_t`), structural initialization state
  (`ADDRESS_SPACE_STATE_UNINITIALIZED`, `ADDRESS_SPACE_STATE_INITIALIZED`),
  shared lifetime model (`refcount`), optional owning process association
  (`owner`, nullable during early lifecycle), and architecture-specific opaque
  handle reference (`arch_handle`)
- `core/address_space.c` — deterministic lifecycle helper stubs:
  `address_space_alloc()` (fixed NULL failure stub), `address_space_get()`
  (reference acquisition), `address_space_put()` (reference release)
- `tests/address_space/test_address_space_contract_compile.c` — compile-check
  translation unit validating contract declaration visibility, helper
  signatures, and visible struct field layout in kernel code
- `tests/address_space/test_address_space_link.c` — link-check for
  address-space lifecycle helper symbol presence

This step defines ownership and process-association boundaries only as a
Phase 5 contract surface. It does not implement VM policy, mapping behavior,
paging behavior, MMU enablement, loader logic, or userspace memory semantics.

#### Phase 5 boundary step — Kernel-internal ELF loader contract

- `include/kernul/elf_loader.h` — kernel-internal ELF loader contract
  defining ELF image shape and ownership boundaries: structural image
  generation (`elf_image_generation_t`), deterministic non-success validation
  state model (`ELF_IMAGE_STATE_UNVALIDATED`, `ELF_IMAGE_STATE_UNSUPPORTED`,
  `ELF_IMAGE_STATE_INVALID`), shared lifetime model (`refcount`), borrowed
  process association (`owner`), borrowed address-space association
  (`address_space`), and architecture-specific opaque handle reference
  (`arch_handle`)
- `core/elf_loader.c` — deterministic lifecycle/helper stubs: `elf_image_alloc()`
  (single-slot allocation stub with `ELF_IMAGE_STATE_UNVALIDATED` initialization),
  `elf_image_get()` (reference acquisition), `elf_image_put()` (reference
  release), and `elf_image_validate()` (state-only transition from
  `ELF_IMAGE_STATE_UNVALIDATED` to `ELF_IMAGE_STATE_UNSUPPORTED`)
- `tests/elf/test_elf_loader_contract_compile.c` — compile-check translation
  unit validating contract declaration visibility, helper signatures, and
  visible struct field layout in kernel code
- `tests/elf/test_elf_loader_link.c` — link-check for ELF loader helper symbol
  presence

This Phase 5 step defines loader shape, ownership, and validation-state
boundaries only. It does not implement ELF parsing, mapping behavior, paging
behavior, relocation handling, or executable start/userspace execution.

#### Phase 5 completion boundary step — Init process launch path contract

- `include/kernul/init.h` — kernel-internal `init_launch()` contract defining
  deterministic init launch attempt sequencing, explicit calling context
  boundaries, ownership/borrowed-pointer rules, and explicit non-goals
- `core/init.c` — deterministic composition stub that performs fixed structural
  sequence only: process object attempt, address-space association, ELF image
  association, and deterministic validation call
- `tests/init/test_init_contract_compile.c` — compile-check translation unit
  validating init launch contract declaration visibility and signature
- `tests/init/test_init_link.c` — link-check for `init_launch` symbol presence

This Phase 5 completion boundary step is deterministic composition only.
It does not implement userspace execution/resume behavior, VM behavior
(mapping/paging/MMU), or loader execution.
It does not claim init-process execution or userspace compatibility.

#### Phase 6 boundary step — File descriptor table contract

- `include/kernul/fd_table.h` — kernel-internal file descriptor table
  contract defining descriptor identity and ownership shape only: `fd_t`
  as integer descriptor identity, shared lifetime model (`refcount`),
  borrowed owning-process association (`owner`), and bounded descriptor slot
  state container (`slots`) with occupied/free representation
- `core/fd_table.c` — deterministic lifecycle helper stubs:
  `fd_table_alloc()` (single bounded internal slot allocation stub with
  all slots initialized `FD_SLOT_FREE`), `fd_table_get()` (reference
  acquisition), and `fd_table_put()` (reference release)
- `tests/fd/test_fd_table_contract_compile.c` — compile-check translation
  unit validating declaration visibility, helper signatures, and visible
  fd-table struct field layout in kernel code
- `tests/fd/test_fd_table_link.c` — link-check for fd-table lifecycle helper
  symbol presence

This Phase 6 boundary step defines descriptor identity, ownership, and
process association shape only.
It does not implement Unix I/O behavior, filesystem behavior, device behavior,
pipes, terminals, or read/write semantics.

#### Phase 6 boundary step — VFS core contract surface

- `include/kernul/vfs.h` — kernel-internal VFS core contract defining minimal
  refcounted open-file object shape (`struct vfs_file`) and operation attachment
  vocabulary (`struct vfs_ops`) with structural identity (`vfs_file_id_t`),
  ownership/lifetime rules, and explicit non-ABI scope boundary
- `core/vfs.c` — deterministic VFS core stubs: bounded single-slot
  `vfs_file_alloc()`/`vfs_file_get()`/`vfs_file_put()` lifecycle helpers and
  fixed `KERN_ENOSYS` return behavior for `vfs_init()`, `vfs_open()`,
  `vfs_close()`, `vfs_read()`, and `vfs_write()`
- `tests/vfs/test_vfs_contract_compile.c` — compile-check translation unit
  validating VFS declaration visibility, signatures, and visible `struct vfs_file`
  field layout
- `tests/vfs/test_vfs_link.c` — link-check for VFS core lifecycle and VFS entry
  point symbol presence

This Phase 6 boundary step is kernel-internal only and is not a userspace ABI.
It does not implement path lookup/resolution, filesystem behavior, device
behavior, pipe or terminal integration, or read/write semantics beyond
deterministic `KERN_ENOSYS` stubs.


#### Phase 6 boundary step — Pipe contract surface

- `include/kernul/pipe.h` — kernel-internal pipe contract defining minimal
  refcounted shared pipe object shape (`struct pipe_object`) and directional
  endpoint vocabulary (`struct pipe_end`) with structural identity (`pipe_id_t`),
  endpoint role identity (`pipe_end_kind_t`), ownership/lifetime rules, and
  explicit non-ABI scope boundary
- `core/pipe.c` — deterministic pipe contract stubs: bounded single-slot
  `pipe_alloc()`/`pipe_get()`/`pipe_put()` lifecycle helpers and structural
  `pipe_open_pair()` endpoint initialization for one read endpoint and one write
  endpoint only
- `tests/pipe/test_pipe_contract_compile.c` — compile-check translation unit
  validating pipe declaration visibility, signatures, and visible struct field
  layout
- `tests/pipe/test_pipe_link.c` — link-check for pipe lifecycle and open-pair
  symbol presence

This Phase 6 boundary step is kernel-internal only and is not a userspace ABI.
It does not implement buffering or read/write transfer semantics.
It does not implement scheduler interaction or wakeup behavior.
It does not implement signal or terminal integration.
It does not implement VFS attachment or fd-table wiring.

#### Phase 6 boundary step — Signal contract surface

- `include/kernul/signal.h` — kernel-internal signal contract defining minimal
  signal identity (`signal_id_t`), bounded signal-set membership representation
  (`struct signal_set`), and per-process signal-state container shape
  (`struct signal_state`) with explicit ownership/lifetime scope
- `core/signal.c` — deterministic signal state stubs:
  `signal_state_alloc()`/`signal_state_free()` lifecycle helpers and structural
  `signal_send()`/`signal_block()`/`signal_unblock()` state updates only
- `tests/signal/test_signal_contract_compile.c` — compile-check translation
  unit validating signal declaration visibility, signatures, and visible signal
  struct field layout
- `tests/signal/test_signal_link.c` — link-check for signal contract symbol
  presence

This Phase 6 boundary step is kernel-internal only and is not a userspace ABI.
It does not implement signal delivery or execution interruption semantics.
It does not implement scheduler, terminal, or job-control integration.
It does not imply POSIX numbering or compatibility guarantees.

#### Phase 6 boundary step — Session and process group contract surface

- `include/kernul/session.h` — kernel-internal session/process-group contract
  defining structural identity (`session_id_t`, `process_group_id_t`),
  identity containers (`struct session`, `struct process_group`), borrowed
  leader/session associations, and explicit ownership/lifetime boundaries
- `core/session.c` — deterministic bounded creation stubs:
  `session_create()`/`process_group_create()` for identity containers only and
  structural ID accessors `session_id()`/`process_group_id()`
- `tests/session/test_session_contract_compile.c` — compile-check translation
  unit validating declaration visibility, signatures, and visible session and
  process-group struct field layout
- `tests/session/test_session_link.c` — link-check for session/process-group
  contract symbol presence

This Phase 6 boundary step is kernel-internal only and is not a userspace ABI.
It defines identity and grouping structure only.
It does not implement signal routing, terminal behavior, or job-control behavior.
It does not imply POSIX numbering or compatibility guarantees.

#### Phase 6 boundary step — Terminal and PTY contract surfaces

- `include/kernul/terminal.h` — kernel-internal terminal/PTY contract defining
  structural terminal identity (`terminal_id_t`), structural PTY identity
  (`pty_id_t`), minimal identity containers (`struct terminal`,
  `struct pty_master`, `struct pty_pair`), and structural session/process-group
  attachment points only
- `core/terminal.c` — deterministic bounded single-slot creation stubs:
  `terminal_create()`/`pty_master_create()`/`pty_pair_create()` and structural
  ID accessors `terminal_id()`/`pty_master_id()`/`pty_pair_id()`
- `tests/terminal/test_terminal_contract_compile.c` — compile-check translation
  unit validating declaration visibility, signatures, required ID constants, and
  visible terminal/PTY struct field layout
- `tests/terminal/test_terminal_link.c` — link-check for terminal/PTY contract
  symbol presence

This Phase 6 boundary step is kernel-internal only and is not a userspace ABI.
It defines identity and attachment only.
It does not implement terminal I/O semantics.
It does not implement line discipline.
It does not implement signal or job-control integration.
It does not implement VFS or fd-table wiring.

#### Phase 6 boundary step — Controlling terminal attachment surface

- `include/kernul/controlling_terminal.h` — kernel-internal controlling
  terminal attachment contract defining `struct controlling_terminal` with
  borrowed `session` and `terminal` associations only, explicit ownership and
  lifetime boundaries, and one-to-one attachment invariant documentation
- `core/controlling_terminal.c` — deterministic bounded single-slot attachment
  stubs for `controlling_terminal_attach()` and `controlling_terminal_get()`
- `tests/terminal/test_controlling_terminal_contract_compile.c` — compile-check
  translation unit validating declaration visibility, signatures, and visible
  controlling terminal struct field layout
- `tests/terminal/test_controlling_terminal_link.c` — link-check for
  controlling terminal attachment contract symbol presence

This Phase 6 boundary step is kernel-internal only and is not a userspace ABI.
It defines structural session-to-terminal attachment only.
It defines a one-to-one attachment invariant.
It does not implement job-control or signal semantics.
It does not implement terminal I/O behavior.
It does not implement VFS or fd-table wiring.

#### Phase 7 boundary step — Interactive console path contract

- `include/kernul/interactive_console.h` — kernel-internal interactive console
  path contract defining `struct interactive_console` with borrowed
  controlling-terminal/session/process-group associations only, explicit
  ownership and lifetime boundaries, and one-to-one routing invariant
  documentation
- `core/interactive_console.c` — deterministic bounded single-slot routing
  designation stubs for `interactive_console_attach()` and
  `interactive_console_consumer()`
- `tests/console/test_interactive_console_contract_compile.c` — compile-check
  translation unit validating declaration visibility, signatures, and visible
  interactive console struct field layout
- `tests/console/test_interactive_console_link.c` — link-check for interactive
  console contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines routing and designation behavior only.
It defines terminal-to-consumer routing direction only.
It enforces controlling-terminal attachment dependency.
It does not implement terminal I/O semantics.
It does not implement job-control or signal behavior.
It does not implement scheduler or VFS interaction.

#### Phase 7 boundary step — Interactive process activation path

- `include/kernul/interactive_activation.h` — kernel-internal interactive
  activation contract defining `struct interactive_activation` with borrowed
  interactive-console/process-group associations only, explicit ownership and
  lifetime boundaries, explicit activation state constants
  (`INTERACTIVE_ACTIVATION_STATE_PENDING`,
  `INTERACTIVE_ACTIVATION_STATE_RECORDED`), routing dependency chain
  documentation (`session -> controlling_terminal -> interactive_console ->
  interactive_activation`), and single activation-intent/idempotence invariant
  documentation
- `core/interactive_activation.c` — deterministic bounded single-slot
  activation-intent stubs for `interactive_activation_create()` and
  `interactive_activation_state()` with identical-input idempotence
- `tests/console/test_interactive_activation_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  activation state constants, signatures, and visible interactive activation
  struct field layout
- `tests/console/test_interactive_activation_link.c` — link-check for
  interactive activation contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines activation intent only.
It enforces the routing dependency chain through interactive console
associations.
It defines idempotent activation behavior for identical inputs.
It does not invoke the scheduler or process wakeups.
It does not implement signal or job-control behavior.
It does not implement terminal I/O behavior.

#### Phase 7 boundary step — Interactive process readiness transition

- `include/kernul/interactive_readiness.h` — kernel-internal interactive
  readiness contract defining `struct interactive_readiness` with borrowed
  activation/process-group associations only, explicit ownership and lifetime
  boundaries, explicit readiness state constants
  (`INTERACTIVE_READINESS_STATE_INACTIVE`,
  `INTERACTIVE_READINESS_STATE_READY`), scheduler-visible eligibility meaning,
  activation dependency chain documentation
  (`session -> controlling_terminal -> interactive_console ->
  interactive_activation -> interactive_readiness`), and single-readiness
  idempotence invariant documentation
- `core/interactive_readiness.c` — deterministic bounded single-slot
  readiness-eligibility stubs for `interactive_readiness_mark_ready()` and
  `interactive_readiness_state()` with identical-input idempotence
- `tests/console/test_interactive_readiness_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  readiness state constants, signatures, and visible interactive readiness
  struct field layout
- `tests/console/test_interactive_readiness_link.c` — link-check for
  interactive readiness contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines readiness eligibility only.
It freezes scheduler-visible meaning as discoverable eligibility indication only.
It enforces the activation dependency chain through interactive activation
associations.
It defines idempotent readiness behavior for identical inputs.
It does not invoke the scheduler or process wakeups.
It does not implement signal or job-control behavior.
It does not implement terminal I/O behavior.

#### Phase 7 boundary step — Scheduler admission boundary

- `include/kernul/interactive_admission.h` — kernel-internal interactive
  scheduler admission contract defining `struct interactive_admission` with
  borrowed readiness/process-group associations only, explicit ownership and
  lifetime boundaries, explicit admission state constants
  (`INTERACTIVE_ADMISSION_STATE_NONE`, `INTERACTIVE_ADMISSION_STATE_ADMITTED`),
  authorization-only scheduler meaning, monotonic single-transition rule,
  readiness dependency chain documentation
  (`session -> controlling_terminal -> interactive_console ->
  interactive_activation -> interactive_readiness ->
  interactive_admission`), and single-admission idempotence invariant
  documentation
- `core/interactive_admission.c` — deterministic bounded single-slot
  admission-authorization stubs for `interactive_admission_admit()` and
  `interactive_admission_state()` with identical-input idempotence
- `tests/console/test_interactive_admission_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  admission state constants, signatures, and visible interactive admission
  struct field layout
- `tests/console/test_interactive_admission_link.c` — link-check for
  interactive admission contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines scheduler admission authorization only.
It defines admission as a monotonic transition in this contract.
It enforces the readiness dependency chain through interactive readiness
associations.
It defines idempotent admission behavior for identical inputs.
It does not invoke scheduler policy or manipulate run queues.
It does not implement signal or job-control behavior.
It does not implement terminal I/O behavior.

#### Phase 7 boundary step — Interactive runnable enqueue boundary

- `include/kernul/interactive_runnable.h` — kernel-internal interactive
  runnable enqueue contract defining `struct interactive_runnable` with borrowed
  admission/process-group associations only, explicit ownership and lifetime
  boundaries, explicit runnable state constants
  (`INTERACTIVE_RUNNABLE_STATE_NONE`,
  `INTERACTIVE_RUNNABLE_STATE_ENQUEUED`), frozen membership-registration
  meaning (marker creation/recording only; no scheduling queue insertion; no
  execution state mutation), admission dependency chain documentation
  (`session -> controlling_terminal -> interactive_console ->
  interactive_activation -> interactive_readiness -> interactive_admission ->
  interactive_runnable`), monotonic single-transition rule, and single-runnable
  idempotence invariant documentation
- `core/interactive_runnable.c` — deterministic bounded single-slot runnable
  membership stubs for `interactive_runnable_enqueue()` and
  `interactive_runnable_state()` with identical-input idempotence and
  conflicting-input rejection
- `tests/console/test_interactive_runnable_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  runnable state constants, signatures, and visible interactive runnable struct
  field layout
- `tests/console/test_interactive_runnable_link.c` — link-check for
  interactive runnable enqueue contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines runnable membership registration only.
It freezes membership-registration meaning as marker recording only, with no
queue insertion semantics and no execution state mutation.
It defines runnable membership as a monotonic transition in this contract.
It enforces the admission dependency chain through interactive admission
associations.
It defines idempotent runnable enqueue behavior for identical inputs.
It does not implement scheduling policy, fairness, time slicing, or preemption.
It does not implement signal or job-control behavior.
It does not implement terminal I/O behavior.

#### Phase 7 boundary step — Interactive scheduler dispatch boundary

- `include/kernul/interactive_dispatch.h` — kernel-internal interactive
  scheduler dispatch contract defining `struct interactive_dispatch` with
  borrowed interactive-runnable/process-group associations only, explicit
  ownership and lifetime boundaries, explicit dispatch state constants
  (`INTERACTIVE_DISPATCH_STATE_NONE`,
  `INTERACTIVE_DISPATCH_STATE_DISPATCHED`), selection-acknowledgment-only
  dispatch meaning (no CPU binding, no resource reservation, no execution-state
  changes), explicit exclusion of CPU/thread/resource association, runnable
  dependency chain documentation
  (`session -> controlling_terminal -> interactive_console ->
  interactive_activation -> interactive_readiness -> interactive_admission ->
  interactive_runnable -> interactive_dispatch`), terminal monotonic-transition
  rule for this Phase 7 sequence, and single-dispatch idempotence invariant
  documentation
- `core/interactive_dispatch.c` — deterministic bounded single-slot dispatch
  visibility stubs for `interactive_dispatch_select()` and
  `interactive_dispatch_state()` with identical-input idempotence and
  conflicting-input rejection
- `tests/console/test_interactive_dispatch_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  dispatch state constants, signatures, and visible interactive dispatch struct
  field layout
- `tests/console/test_interactive_dispatch_link.c` — link-check for interactive
  dispatch contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines dispatch as selection acknowledgment visibility only.
It excludes CPU/thread/resource association at this contract boundary.
It enforces the runnable dependency chain through interactive runnable
associations.
It defines dispatch as the terminal monotonic transition in this Phase 7
sequence.
It defines idempotent dispatch behavior for identical inputs.
It does not execute threads or perform context switching.
It does not bind to CPUs or reserve execution resources.
It does not implement scheduling policy, fairness, time slicing, or preemption.
It does not implement signal or job-control behavior.
It does not implement terminal I/O behavior.

#### Phase 7 boundary step — Interactive execution target preparation boundary

- `include/kernul/interactive_execution_target.h` — kernel-internal
  interactive execution-target preparation contract defining
  `struct interactive_execution_target` with borrowed
  interactive-dispatch/process-group associations only, explicit ownership and
  lifetime boundaries, explicit preparation state constants
  (`INTERACTIVE_EXECUTION_TARGET_STATE_NONE`,
  `INTERACTIVE_EXECUTION_TARGET_STATE_PREPARED`), preparation-only meaning
  (dispatch resolved into execution-target preparation record only), explicit
  exclusion of thread binding, CPU binding, run-queue/resource/token
  association, complete dependency chain documentation
  (`session -> controlling_terminal -> interactive_console ->
  interactive_activation -> interactive_readiness -> interactive_admission ->
  interactive_runnable -> interactive_dispatch ->
  interactive_execution_target`), monotonic single-transition rule, and
  idempotent identical-input behavior definition (same dispatch-associated
  session and consumer group in this phase)
- `core/interactive_execution_target.c` — deterministic bounded single-slot
  preparation stubs for `interactive_execution_prepare()` and
  `interactive_execution_target_state()` with identical-input idempotence and
  conflicting-input rejection
- `tests/console/test_interactive_execution_target_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  preparation state constants, signatures, and visible interactive execution
  target struct field layout
- `tests/console/test_interactive_execution_target_link.c` — link-check for
  interactive execution-target preparation contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines preparation only: dispatch is resolved into an execution-target
preparation record suitable for later architecture-specific context switching.
It defines preparation as a monotonic transition in this contract.
It defines idempotent preparation behavior for identical inputs, where identical
means the same dispatch-associated session and consumer group in this phase.
It does not identify or bind a specific thread.
It does not bind to a CPU or manage run queues/resources/tokens.
It does not perform context switching and does not call
`arch_cpu_state_switch()`.
It does not implement scheduling policy, fairness, time slicing, or preemption.
It does not implement signal or job-control behavior.
It does not implement terminal I/O behavior.

#### Phase 7, Task 7 — Interactive switch operand preparation boundary

- `include/kernul/interactive_switch_operands.h` — kernel-internal
  interactive switch-operand preparation contract defining
  `struct interactive_switch_operands` with borrowed
  interactive-execution-target/process-group/`arch_cpu_state_t` operand
  associations only, explicit ownership and lifetime boundaries, explicit
  preparation state constants (`INTERACTIVE_SWITCH_OPERANDS_STATE_NONE`,
  `INTERACTIVE_SWITCH_OPERANDS_STATE_PREPARED`), preparation-only meaning
  (concrete switch operands prepared; no execution transfer), explicit
  prohibition on `arch_cpu_state_switch()` invocation at this boundary,
  complete dependency chain documentation
  (`session -> controlling_terminal -> interactive_console ->
  interactive_activation -> interactive_readiness -> interactive_admission ->
  interactive_runnable -> interactive_dispatch -> interactive_execution_target ->
  interactive_switch_operands`), deterministic/bounded operand-source
  constraints using static non-owning architecture-state placeholders in this
  phase, monotonic single-transition rule, and idempotent identical-input
  behavior definition (same execution-target-associated session and consumer
  group in this phase)
- `core/interactive_switch_operands.c` — deterministic bounded single-slot
  switch-operand preparation stubs for `interactive_switch_operands_prepare()`
  and `interactive_switch_operands_state()` with identical-input idempotence,
  conflicting-input rejection, and deterministic static stub `from`/`to`
  architecture operand placeholders
- `tests/console/test_interactive_switch_operands_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  preparation state constants, signatures, visible interactive switch-operands
  struct field layout, and visible `arch_cpu_state_t *` operand field types
- `tests/console/test_interactive_switch_operands_link.c` — link-check for
  interactive switch-operand preparation contract symbol presence

This Phase 7 boundary step is kernel-internal only and is not a userspace ABI.
It defines operand preparation only: execution target is resolved into concrete
architecture switch operands for a later switch surface.
It defines preparation as a monotonic transition in this contract.
It defines idempotent preparation behavior for identical inputs, where identical
means the same execution-target-associated session and consumer group in this
phase.
It does not call `arch_cpu_state_switch()` and does not perform context
switching.
It does not execute threads, bind to CPUs, or reserve execution resources.
It does not implement scheduling policy, fairness, time slicing, or preemption.
It does not implement signal or job-control behavior.
It does not implement terminal I/O behavior.


#### Phase 8, Task 1 — Interactive execution transfer boundary

- `include/kernul/interactive_execution.h` — kernel-internal interactive
  execution-transfer contract defining `struct interactive_execution` with
  borrowed interactive-switch-operands/process-group associations only, explicit
  ownership and lifetime boundaries, execution state constants
  (`INTERACTIVE_EXECUTION_STATE_NONE`,
  `INTERACTIVE_EXECUTION_STATE_EXECUTED`), executed-state meaning as explicit
  `arch_cpu_state_switch()` invocation (or explicitly gated simulation in
  verification/test builds), complete dependency chain documentation
  (`session -> controlling_terminal -> interactive_console ->
  interactive_activation -> interactive_readiness -> interactive_admission ->
  interactive_runnable -> interactive_dispatch -> interactive_execution_target ->
  interactive_switch_operands -> interactive_execution ->
  arch_cpu_state_switch()`), explicit real-build control-flow boundary semantics,
  explicit verification gating requirement, monotonic single-transfer invariant,
  and idempotent identical-input behavior definition (same
  switch-operands-associated session and consumer group in this phase)
- `core/interactive_execution.c` — deterministic bounded single-slot
  execution-transfer stubs for `interactive_execution_transfer()` with full chain
  validation, identical-input idempotence, conflicting-input rejection, monotonic
  executed state transition, and guarded architecture switch invocation
- `tests/console/test_interactive_execution_contract_compile.c` — compile-check
  translation unit validating declaration visibility, required execution state
  constants, signature, and visible interactive execution struct field layout
- `tests/console/test_interactive_execution_link.c` — link-check for interactive
  execution transfer contract symbol presence

This Phase 8 boundary step is kernel-internal only and is not a userspace ABI.
It defines execution transfer as explicit invocation of `arch_cpu_state_switch()`.
It enforces switch-operands dependency at the execution boundary.
It gates architecture switching for verification/test safety.
It defines execution as a monotonic transition in this contract.
It defines idempotent behavior for identical inputs.
It does not implement scheduling policy, fairness, time slicing, or preemption.
It does not implement run-queue management, signal behavior, or job-control
behavior.
It does not implement terminal I/O behavior.


#### Phase 8, Task 2 — Scheduler policy introduction boundary

- `include/kernul/interactive_selection.h` — kernel-internal interactive
  scheduler selection-policy contract defining `struct interactive_selection`
  with borrowed interactive-runnable/interactive-dispatch/process-group
  associations only, explicit ownership and lifetime boundaries, selection
  state constants (`INTERACTIVE_SELECTION_STATE_NONE`,
  `INTERACTIVE_SELECTION_STATE_SELECTED`), explicit deterministic selection
  ordering authority sourced from interactive runnable membership ordering,
  explicit dependency boundary on interactive runnable/dispatch/
  execution target contracts, and explicit prohibition against execution
  transfer and architecture switching dependencies
- `core/interactive_selection.c` — deterministic bounded single-slot
  per-session stand-in selection-policy stubs for
  `interactive_selection_select()` and `interactive_selection_state()` with
  full upstream chain validation, identical-input idempotence by runnable and
  session/consumer-group pointer identity, conflicting-input rejection, and
  monotonic selected-state visibility
- `tests/console/test_interactive_selection_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  selection state constants, signatures, and visible interactive selection
  struct field layout
- `tests/console/test_interactive_selection_link.c` — link-check for
  interactive selection contract symbol presence

This Phase 8 boundary step is kernel-internal only and is not a userspace ABI.
It introduces a deterministic phase-local selection policy boundary:
interactive runnable membership ordering is authoritative for selection order.
This phase records selection with a deterministic per-session single-slot
stand-in and does not introduce broader scheduler commitments.
It defines a per-session single-slot selection invariant with idempotent
identical-input behavior and monotonic selected-state visibility guarantees.
It does not implement fairness, time slicing, preemption, or run-queue
management.
It does not implement job-control, signal, terminal, or execution-transfer
behavior.

#### Phase 9, Task 1 — Scheduler state materialization boundary

- `include/kernul/interactive_scheduler_state.h` — kernel-internal interactive
  scheduler-state materialization contract defining
  `struct interactive_scheduler_state` with borrowed
  session/interactive-runnable associations only, explicit ownership and
  lifetime boundaries, scheduler-state constants
  (`INTERACTIVE_SCHEDULER_STATE_NONE`,
  `INTERACTIVE_SCHEDULER_STATE_READY`), explicit runnable-membership ordering
  authority, explicit determinism/idempotence/conflict invariants, and explicit
  prohibitions against selection/execution/architecture-switch/timekeeping
  dependencies
- `core/interactive_scheduler_state.c` — deterministic bounded single-slot
  per-session stand-in scheduler-state materialization stubs for
  `interactive_scheduler_state_create()` and
  `interactive_scheduler_state_state()` with invalid-input rejection,
  identical-input idempotence by session/runnable pointer identity,
  conflicting-input rejection, and monotonic ready-state visibility
- `tests/console/test_interactive_scheduler_state_contract_compile.c` —
  compile-check translation unit validating declaration visibility, required
  scheduler-state constants, signatures, and visible
  interactive-scheduler-state struct field layout
- `tests/console/test_interactive_scheduler_state_link.c` — link-check for
  interactive scheduler-state contract symbol presence

This Phase 9 boundary step is kernel-internal only and is not a userspace ABI.
It introduces scheduler-state materialization as a per-session persistent
structure representing runnable membership deterministically.
It is phase-local substrate only and does not introduce scheduler behavior.
It does not implement timekeeping, fairness, time slicing, preemption, or
execution behavior.

#### Phase 9, Task 1.6 — Scheduler state ownership remediation

- `include/kernul/session.h` — extended `struct session` with explicitly
  owned scheduler-state storage and explicit scheduler-state liveness marker,
  preserving existing scheduler-state contract shape and semantics
- `include/kernul/interactive_scheduler_state.h` — scheduler-state record
  materialization now sourced from the session contract surface, preserving
  public scheduler-state API
- `core/interactive_scheduler_state.c` — removed subsystem-wide scheduler-state
  slot and subsystem-wide ownership flag; scheduler-state materialization now
  uses per-session owned storage with existing deterministic idempotence,
  conflict behavior, and acquire/release publication semantics preserved
- `core/session.c` — session creation initializes per-session scheduler-state
  storage to a deterministic unmaterialized baseline
- `tests/console/test_interactive_scheduler_state_contract_compile.c` —
  compile-check coverage extended to verify session-visible scheduler-state
  ownership fields

Scheduler state ownership is remediated to align implementation with the
existing per-session contract. No scheduler behavior or policy changes are
introduced.

#### Phase 9, Task 2 — Scheduler runnable container materialization

- `include/kernul/interactive_scheduler_container.h` — kernel-internal
  interactive scheduler runnable-container materialization contract defining
  `struct interactive_scheduler_container`, explicit session ownership and
  session-bounded lifetime, explicit interactive runnable membership ordering
  authority, deterministic/idempotent/conflict semantics, and explicit
  prohibition of scheduling policy or execution behavior
- `core/interactive_scheduler_container.c` — deterministic per-session
  container materialization stub for
  `interactive_scheduler_container_materialize()` with invalid-input rejection,
  identical-input idempotence by session/runnable pointer identity, conflict
  rejection for different runnable pointer identity, and persistent
  session-owned container publication
- `include/kernul/session.h` — extended `struct session` with explicit
  session-owned scheduler-container storage and explicit scheduler-container
  liveness marker
- `core/session.c` — session creation initializes scheduler-container storage
  and liveness marker to deterministic unmaterialized baseline
- `tests/console/test_interactive_scheduler_container_contract_compile.c` —
  compile-check translation unit validating container type/API visibility and
  session ownership association fields
- `tests/console/test_interactive_scheduler_container_link.c` — link-check for
  interactive scheduler container materialization symbol presence

This Phase 9 boundary step is kernel-internal only and is not a userspace ABI.
It materializes scheduler runnable container structure as persistent mechanical
representation only.
No scheduling policy or execution behavior is introduced.

#### Phase 9, Task 3 — Selection over scheduler container

- `core/interactive_selection.c` — selection now binds candidate enumeration to
  a per-session scheduler container membership view by materializing
  `interactive_scheduler_state` and `interactive_scheduler_container` before
  recording selection, while preserving existing deterministic selection logic,
  idempotence, and conflict behavior
- `include/kernul/interactive_selection.h` — selection contract documentation
  updated to state container-backed membership-view sourcing and explicit
  external ordering authority (`interactive runnable membership ordering`)
- `tests/console/test_interactive_selection_contract_compile.c` — compile-time
  dependency boundary checks extended to include scheduler-state/container
  surface visibility through selection integration context
- `tests/console/test_interactive_selection_link.c` / `Makefile` — link-check
  integration updated to ensure selection/container/state symbols resolve
  together in the bound integration path

This Phase 9 boundary step is kernel-internal only and is not a userspace ABI.
Selection now reads from per-session scheduler container substrate while
preserving the deterministic selection boundary with no policy change.
No time, fairness, priority, preemption, or execution behavior is introduced.

#### Phase 10, Task 1 — Execution handoff substrate materialization

- `include/kernul/interactive_selection_result.h` — kernel-internal
  `struct interactive_selection_result` identity record introduced as a
  structural selection-output dependency surface for handoff materialization
- `include/kernul/interactive_execution_handoff.h` — kernel-internal
  execution handoff substrate contract defining
  `struct interactive_execution_handoff` and
  `interactive_execution_handoff_materialize()` for per-session structural
  transfer-input record materialization only
- `include/kernul/session.h` — extended `struct session` with explicitly
  owned execution-handoff storage and explicit execution-handoff publication
  marker
- `core/interactive_execution_handoff.c` — deterministic per-session handoff
  materialization stub with pointer-identity idempotence and conflict semantics,
  plus explicit acquire/release publication boundaries
- `core/session.c` — session creation initializes execution-handoff storage
  and publication marker to deterministic unmaterialized baseline
- `tests/console/test_interactive_execution_handoff_contract_compile.c` —
  compile-check translation unit validating handoff type/API visibility and
  session ownership association
- `tests/console/test_interactive_execution_handoff_link.c` — link-check for
  interactive execution handoff materialization symbol presence
- `tests/session/test_session_contract_compile.c` — compile-check coverage
  extended for session-visible execution-handoff ownership fields
- `Makefile` — build/test integration for execution-handoff compile/link
  contract checks

This Phase 10 boundary step is kernel-internal only and is not a userspace ABI.
It materializes execution handoff substrate structure as structural packaging
only.
No execution, time, fairness, priority, or preemption behavior is introduced.

#### Phase 10, Task 2 — Selection-to-handoff binding

- `include/kernul/interactive_selection.h` — selection surface now exposes
  deterministic selection output through
  `interactive_selection_result_expose()` as
  `const struct interactive_selection_result *` identity
- `core/interactive_selection.c` — selection output identity record is
  materialized from the already-chosen operand/session association with
  selection semantics preserved
- `include/kernul/interactive_selection_handoff_binding.h` — kernel-internal
  binding contract added for selection-result to per-session handoff
  materialization
- `core/interactive_selection_handoff_binding.c` — binding entrypoint
  `interactive_selection_handoff_bind()` added; it obtains selection result,
  invokes `interactive_execution_handoff_materialize(session, selection)`, and
  propagates `NULL` unchanged
- `tests/console/test_interactive_selection_contract_compile.c` and
  `tests/console/test_interactive_selection_link.c` — compile/link coverage
  extended for selection-result exposure symbol visibility
- `tests/console/test_interactive_selection_handoff_binding_contract_compile.c`
  and `tests/console/test_interactive_selection_handoff_binding_link.c` —
  compile/link checks added for binding symbol visibility and
  selection/handoff linkage
- `Makefile` — build/test integration extended for the new binding
  compile/link contract checks

This Phase 10 boundary step is kernel-internal only and is not a userspace ABI.
Deterministic selection output is bound to per-session execution handoff
substrate with selection semantics preserved.
No execution initiation, time, fairness, priority, preemption, or CPU binding
behavior is introduced.

#### Phase 10, Task 3 — Handoff-to-transfer operand view binding

- `include/kernul/interactive_execution_transfer_operands_view.h` —
  kernel-internal transfer-input view contract added for
  `struct interactive_execution_transfer_operands_view` and
  `interactive_execution_transfer_operands_view_expose()`
- `include/kernul/session.h` — extended `struct session` with
  session-owned transfer-input view storage and explicit view publication
  marker
- `core/interactive_execution_transfer_operands_view.c` — transfer-input view
  exposure added as immutable structural packaging over the already-published
  per-session handoff record, with explicit acquire consumption of handoff
  publication and deterministic pointer-identity idempotence
- `core/session.c` — session creation initializes transfer-input view storage
  and publication marker to deterministic unexposed baseline
- `tests/console/test_interactive_execution_transfer_operands_view_contract_compile.c`
  — compile-check translation unit validating transfer-input view type/API
  visibility and session-owned storage visibility
- `tests/console/test_interactive_execution_transfer_operands_view_link.c` —
  link-check for transfer-input view exposure symbol presence
- `tests/session/test_session_contract_compile.c` — compile-check coverage
  extended for session-visible transfer-input view ownership fields
- `Makefile` — build/test integration extended for transfer-input view
  compile/link contract checks

This Phase 10 boundary step is kernel-internal only and is not a userspace ABI.
It binds execution handoff to transfer-input view exposure as immutable
structural packaging only, with no allocation.
No execution initiation, time, fairness, priority, preemption, or CPU binding
behavior is introduced.

#### Phase 10, Task 4 — Explicit transfer initiation gate

- `include/kernul/interactive_execution_transfer_initiation_gate.h` —
  kernel-internal explicit transfer initiation gate contract added for
  `interactive_execution_transfer_initiate(session, operands_view)`
- `core/interactive_execution_transfer_initiation_gate.c` — explicit initiation
  gate implementation added that consumes transfer operands view, performs a
  single boundary call, and returns boundary status unchanged
- `tests/console/test_interactive_execution_transfer_initiation_gate_contract_compile.c`
  — compile-check translation unit validating initiation gate symbol visibility
  and required signature
- `tests/console/test_interactive_execution_transfer_initiation_gate_link.c` —
  link-check for initiation gate symbol presence with operands-view and
  transfer-boundary symbols
- `Makefile` — build/test integration extended for initiation gate compile/link
  contract checks

This Phase 10 boundary step is kernel-internal only and is not a userspace ABI.
It introduces explicit execution transfer initiation that consumes transfer
operands view, with a single boundary call and transparent status passthrough.
No retries, no outcome observation, and no time/fairness/priority/preemption/CPU
binding behavior is introduced.

#### Phase 11, Task 1 — Execution outcome publication record

- `include/kernul/interactive_execution_outcome_record.h` —
  kernel-internal outcome publication contract added for
  `struct interactive_execution_outcome_record`,
  `interactive_execution_outcome_record_publish_from_transfer()`, and
  `interactive_execution_outcome_record_acquire()`
- `include/kernul/session.h` — extended `struct session` with session-owned
  execution outcome record storage and explicit publication marker
- `core/interactive_execution_outcome_record.c` — passive outcome publication
  implementation added with total deterministic transfer-result mapping to
  completed/failed and explicit release/acquire visibility boundaries
- `core/session.c` — session creation initializes execution outcome record
  storage and publication marker to deterministic unpublished baseline
- `tests/console/test_interactive_execution_outcome_record_contract_compile.c`
  — compile-check translation unit validating type visibility, symbol
  signatures, and required dependency context
- `tests/console/test_interactive_execution_outcome_record_link.c` —
  link-check for outcome publication/acquire symbols with transfer-boundary and
  session types
- `tests/session/test_session_contract_compile.c` — compile-check coverage
  extended for session-visible outcome record ownership fields
- `Makefile` — build/test integration extended for outcome record
  compile/link contract checks

This Phase 11 boundary step is kernel-internal only and is not a userspace ABI.
It introduces a session-owned execution outcome publication record with total
mechanical mapping to completed/failed and explicit publish/acquire visibility.
No retries/backoff, no time/fairness/priority/preemption/CPU binding behavior,
and no upstream consumption or reaction behavior is introduced.

#### Phase 11, Task 2 — Passive execution outcome view exposure

- `include/kernul/interactive_execution_outcome_view.h` — kernel-internal
  passive outcome view acquire contract added for
  `struct interactive_execution_outcome_view` and
  `interactive_execution_outcome_view_acquire()`
- `core/interactive_execution_outcome_view.c` — passive session-owned outcome
  view exposure added with explicit acquire load of the outcome publication
  marker and deterministic NULL return when unpublished
- `tests/console/test_interactive_execution_outcome_view_contract_compile.c` —
  compile-check translation unit validating outcome view type visibility,
  acquire symbol signature, and dependency-isolated include shape
- `tests/console/test_interactive_execution_outcome_view_link.c` — link-check
  for outcome view acquire symbol presence with session-owned storage context
- `Makefile` — build/test integration extended for outcome view compile/link
  contract checks

This Phase 11 boundary step is kernel-internal only and is not a userspace ABI.
It introduces passive execution outcome exposure with explicit acquire semantics
over the already-published session-owned outcome record. Unpublished outcome
returns NULL unchanged. No retries/backoff, no policy interpretation, no
time/fairness/priority/preemption/CPU binding behavior, and no consumers are
introduced in this task.

#### Phase 11, Task 3 — Passive per-session execution outcome aggregation

- `include/kernul/interactive_execution_outcome_aggregate.h` — kernel-internal
  per-session passive outcome aggregation contract added for
  `struct interactive_execution_outcome_aggregate` and
  `interactive_execution_outcome_aggregate_acquire()`
- `core/interactive_execution_outcome_aggregate.c` — passive per-session
  aggregate acquire implementation added; consumes outcome visibility only
  through `interactive_execution_outcome_view_acquire()` and returns aggregate
  by value
- `tests/console/test_interactive_execution_outcome_aggregate_contract_compile.c`
  — compile-check translation unit validating aggregate type visibility,
  acquire symbol signature, and dependency-isolated include shape
- `tests/console/test_interactive_execution_outcome_aggregate_link.c` —
  link-check for aggregate acquire symbol presence against session and outcome
  view objects
- `Makefile` — build/test integration extended for outcome aggregate
  compile/link contract checks

This Phase 11 boundary step is kernel-internal only and is not a userspace ABI.
It introduces passive, read-only, ephemeral per-session execution outcome
aggregation with explicit acquire semantics via the outcome view surface.
Counts are explicitly bounded by singular outcome record shape
(`completed_count` and `failed_count` each in `{0,1}`), with zeroed counts when
outcome is unpublished. No retries/backoff, no policy semantics, no reaction
behavior, and no time/fairness/priority/preemption/CPU binding behavior are
introduced in this task.

#### Phase 12, Task 1 — Failure acknowledgment gate update policy

- `include/kernul/interactive_execution_failure_ack_gate.h` —
  kernel-internal failure acknowledgment gate policy contract added for
  `struct interactive_execution_failure_ack_view`,
  `interactive_execution_failure_ack_publish_if_failed()`, and
  `interactive_execution_failure_ack_acquire()`
- `include/kernul/session.h` — extended `struct session` with session-owned
  failure acknowledgment gate view storage and explicit publication marker
- `core/interactive_execution_failure_ack_gate.c` — policy implementation
  added; consumes outcome only via
  `interactive_execution_outcome_view_acquire(session)`, publishes failure ack
  gate only on FAILED, and keeps COMPLETED inert by intentional asymmetry
- `core/session.c` — session creation initializes failure acknowledgment gate
  storage and publication marker to deterministic unpublished baseline
- `tests/console/test_interactive_execution_failure_ack_gate_contract_compile.c`
  — compile-check translation unit validating policy symbol visibility,
  signatures, and dependency-isolated include shape
- `tests/console/test_interactive_execution_failure_ack_gate_link.c` —
  link-check for failure acknowledgment policy symbols against session and
  outcome view objects
- `tests/session/test_session_contract_compile.c` — compile-check coverage
  extended for session-visible failure acknowledgment gate fields
- `Makefile` — build/test integration extended for failure acknowledgment gate
  compile/link contract checks

This Phase 12 boundary step is kernel-internal only and is not a userspace ABI.
It introduces the first explicit outcome consumer policy: externally triggered
failure acknowledgment gate publication on FAILED only, with intentional
COMPLETED inertness. Gate lifetime is session-bounded, publication is one-shot,
monotonic, and idempotent, and visibility is explicit release/acquire. No
retries/backoff, no time/fairness/priority/preemption/CPU binding behavior, and
no mutation of outcome record/view/aggregate surfaces are introduced.

#### Phase 12, Task 2 — Completion acknowledgment gate update policy

- `include/kernul/interactive_execution_completion_ack_gate.h` —
  kernel-internal completion acknowledgment gate policy contract added for
  `struct interactive_execution_completion_ack_view`,
  `interactive_execution_completion_ack_publish_if_completed()`, and
  `interactive_execution_completion_ack_acquire()`
- `include/kernul/session.h` — extended `struct session` with strictly additive
  session-owned completion acknowledgment gate view storage and explicit
  publication marker independent of failure acknowledgment storage
- `core/interactive_execution_completion_ack_gate.c` — policy implementation
  added; consumes outcome only via
  `interactive_execution_outcome_view_acquire(session)`, publishes completion
  ack gate only on COMPLETED, and keeps FAILED inert by intentional asymmetry
- `core/session.c` — session creation initializes completion acknowledgment gate
  storage and publication marker to deterministic unpublished baseline
- `tests/console/test_interactive_execution_completion_ack_gate_contract_compile.c`
  — compile-check translation unit validating policy symbol visibility,
  signatures, and dependency-isolated include shape
- `tests/console/test_interactive_execution_completion_ack_gate_link.c` —
  link-check for completion acknowledgment policy symbols against session and
  outcome view objects
- `tests/session/test_session_contract_compile.c` — compile-check coverage
  extended for session-visible completion acknowledgment gate fields
- `Makefile` — build/test integration extended for completion acknowledgment
  gate compile/link contract checks

This Phase 12 boundary step is kernel-internal only and is not a userspace ABI.
It introduces a second, independent externally triggered acknowledgment
reaction: completion acknowledgment gate publication on COMPLETED only, with
intentional FAILED inertness within this policy. The gate is session-lifetime
bounded, publication is one-shot, monotonic, and idempotent, and visibility is
explicit release/acquire with no background evaluation. No retries/backoff, no
time/fairness/priority/preemption/CPU binding behavior, no mutation of outcome
record/view/aggregate surfaces, and no mutation or observation of the failure
acknowledgment gate are introduced.

#### Phase 13, Task 1 — Failure acknowledgment–driven initiation gating

- `include/kernul/interactive_execution_failure_initiation_gate.h` —
  kernel-internal initiation-gating policy contract added for
  `interactive_execution_initiation_forbid_on_failure_ack()` and
  `interactive_execution_initiation_is_permitted_acquire()`
- `core/interactive_execution_failure_initiation_gate.c` — policy
  implementation added; consumes only
  `interactive_execution_failure_ack_acquire(session)` and performs one-way
  failure-ack-driven transition from initiation permitted to forbidden
- `include/kernul/session.h` — extended `struct session` with exactly one
  session-owned initiation-permitted marker
- `core/session.c` — session creation initializes initiation-permitted marker to
  deterministic permitted baseline
- `core/interactive_execution_transfer_initiation_gate.c` — existing execution
  initiation boundary now acquire-checks initiation-permitted state and refuses
  new initiation when forbidden
- `tests/console/test_interactive_execution_failure_initiation_gate_contract_compile.c`
  — compile-check translation unit validating policy symbol visibility,
  signatures, and dependency-isolated include shape
- `tests/console/test_interactive_execution_failure_initiation_gate_link.c` —
  link-check for failure initiation-gating symbols against session, failure-ack
  gate, and initiation-boundary objects
- `tests/session/test_session_contract_compile.c` — compile-check coverage
  extended for session-visible initiation-permitted marker
- `Makefile` — build/test integration extended for failure initiation-gating
  compile/link contract checks

This Phase 13 boundary step is kernel-internal only and is not a userspace ABI.
It introduces the first control-bearing policy: once the session failure
acknowledgment gate is published, future execution initiation for that session
is forbidden irreversibly for session lifetime. Enforcement occurs only at the
existing initiation boundary, and in-flight execution transfer/running execution
is never modified. Visibility is explicit: forbid publication is release and
initiation-boundary observation is acquire. No retries/fallback, no completion
ack reaction, and no scheduling/time/fairness/priority/preemption/CPU binding
behavior are introduced.

#### Phase 13, Task 2 — Completion acknowledgment–driven initiation gating

- `include/kernul/interactive_execution_completion_initiation_gate.h` —
  kernel-internal initiation-gating policy contract added for
  `interactive_execution_initiation_forbid_on_completion_ack()`
- `core/interactive_execution_completion_initiation_gate.c` — policy
  implementation added; consumes only
  `interactive_execution_completion_ack_acquire(session)` and performs one-way
  completion-ack-driven transition from initiation permitted to forbidden
- `tests/console/test_interactive_execution_completion_initiation_gate_contract_compile.c`
  — compile-check translation unit validating policy symbol visibility,
  signature, and dependency-isolated include shape
- `tests/console/test_interactive_execution_completion_initiation_gate_link.c`
  — link-check for completion initiation-gating symbol against session,
  completion-ack gate, and initiation-boundary objects
- `Makefile` — build/test integration extended for completion initiation-gating
  compile/link contract checks

This Phase 13 boundary step is kernel-internal only and is not a userspace ABI.
It introduces the second control-bearing policy: once the session completion
acknowledgment gate is published, future execution initiation for that session
is forbidden irreversibly for session lifetime. Both failure and completion
acknowledgment are independent triggers that share one irreversible
initiation-eligibility outcome through the existing
`execution_initiation_permitted` marker. Enforcement remains only at the
existing initiation boundary, and in-flight execution transfer/running execution
is never modified. Visibility is explicit: forbid publication is release and
initiation-boundary observation is acquire. No retries/fallback and no
scheduling/time/fairness/priority/preemption/CPU binding behavior are
introduced.

---

### Changed — Phase 3

- `include/kernul/types.h` — error codes (`KERN_OK`, `KERN_ENOMEM`, etc.)
  removed and moved to `include/kernul/errors.h`; `kern_err_t` typedef
  retained with updated comment referencing `errors.h`

### Changed — Phase 4 (corrections)

- `include/kernul/core.h` (removed from earlier generation) — `kern_panic`
  renamed to `panic`; `kern_mutex_t` → `mutex_t`; `kern_waitq_t` →
  `waitq_t`; `kern_timer_t` → `timer_t`; `kern_workq_t` → `workq_t`;
  `KERN_ASSERT` → `ASSERT`; `KERN_WORK_INIT` → `WORK_INIT` per coding
  standard §4.3 (no project-name prefix on core symbols)
- `arch/stub/cpu_state.h` — overstated architecture opacity claim corrected;
  "opaque to common code" replaced with precise statement that `thread.h`
  is bound common code
- `sched/sched.c` — redundant `current = NULL` write removed from
  `sched_init()`; C zero-initializes static storage; comment corrected

---

### Removed

- `mm/mm.c` — heap allocator (`mm_alloc`, `mm_zalloc`, `mm_free`), virtual
  mapping (`mm_map_phys`, `mm_unmap`), and DMA mapping (`mm_dma_alloc`,
  `mm_dma_free`) removed as out-of-scope for Phase 2; PMM only retained
- `include/kernul/boot.h` — `acpi_rsdp`, `cmdline`, `PHYS_REGION_INITRD`,
  `PHYS_REGION_ACPI` fields removed; no requiring task existed at Phase 0–2
- `arch/*/cpu_state.h` (all three) — full register layouts,
  `arch_cpu_state_init()`, `arch_cpu_state_switch()` removed at Phase 1
  audit; placeholder type only until Phase 4, Task 5

---

### Build

- Kernel objects build with `-std=c11 -Wall -Wextra -Wpedantic -Werror
  -ffreestanding -nostdlib`
- Test binaries build with `-std=c11 -Wall -Wextra -Werror` against hosted
  stdlib
- Architecture include path `-Iarch/stub` resolves `<cpu_state.h>` for
  stub target; replace with `-Iarch/<target>` for real hardware ports
- `make test` runs 7 linked test binaries (66 runtime assertions) and
  2 compile-check translation units

[v0.0.0]: https://github.com/irgordon/kernul/releases/tag/v0.0.0
