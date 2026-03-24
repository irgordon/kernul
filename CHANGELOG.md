# Changelog

All notable changes to KERNUL are documented in this file.

The format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

KERNUL does not yet follow Semantic Versioning. Version numbers reflect
phase milestones. v0.0.0 marks the completion of Phase 0 through Phase 4.

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
