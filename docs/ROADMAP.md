# KERNUL Roadmap

**Status:** Pre-Alpha  
**Scope:** Bootstrap through early compatibility milestones  
**Purpose:** Define implementation order and stop-safe priorities

---

# Phase 0 — Foundation

**Status:** Complete

**Deliver:**

- repository structure  
- architecture guide  
- coding standard  
- commenting guide  
- ABI strategy  
- baseline build system  
- baseline test harness  
- boot contract (`boot_info`)  
- panic path implementation  
- kernel entry validation path (`kernel_main`)  
- architecture doctrine updates  
- memory ordering doctrine (`MEMORY_ORDERING.md`)  

**Phase completion rule:**  
The kernel must have a deterministic bootstrap entry path and documented architectural laws.  
No hardware-specific behavior is required.

---

# Phase 1 — Architecture boundary stabilization

**Purpose:**  
Freeze the smallest real hardware-dependent boundaries before expanding behavior.  
This phase prevents architectural drift and ensures portability across `x86_64` and `AArch64`.

**Deliver:**

- architecture boundary header  
- `arch_halt()` contract  
- `arch_early_init()` contract  
- `arch_cpu_state` placeholder type  
- minimal `x86_64` `arch_halt()` implementation  
- minimal `AArch64` `arch_halt()` implementation  
- minimal `arch_early_init()` stubs  
- panic path redirected to `arch_halt()`  

**Constraints:**

- no interrupt handling yet  
- no MMU enablement  
- no scheduler assumptions  
- no device initialization  
- no platform discovery logic  

**Phase completion rule:**  
All terminal execution passes through a defined architecture boundary.

---

# Phase 2 — Memory and ordering foundations

**Purpose:**  
Establish deterministic memory and synchronization vocabulary before real runtime behavior expands.

**Deliver:**

- `atomic.h` contract surface  
- `barrier.h` contract surface  
- physical memory region structure  
- memory map handoff contract  
- PMM public interface  
- PMM initialization stub  
- deterministic memory structure tests  

**Constraints:**

- no complex allocation yet  
- no paging policy  
- no SMP behavior  
- no performance tuning  

**Phase completion rule:**  
Memory representation and ordering semantics are frozen before runtime complexity grows.

---

# Phase 3 — Core runtime primitives

**Purpose:**  
Create the smallest stable internal vocabulary required by all later subsystems.

**Deliver:**

- `types.h`  
- `errors.h`  
- `assert.h`  
- `assert_fail()` implementation  
- intrusive list structure  
- minimal spinlock contract  
- deterministic primitive tests  

**Constraints:**

- no lockless algorithms  
- no scheduler logic  
- no advanced synchronization patterns  

**Phase completion rule:**  
Core kernel primitives exist and are mechanically reviewable.

---

# Phase 4 — Execution and scheduling foundations

**Status:** In Progress

**Purpose:**  
Define execution structure before introducing scheduling policy.

**Completed surfaces:**

- thread structure  
- thread state model  
- run queue structure  
- scheduler public interface  
- scheduler initialization stub  
- enqueue and dequeue stubs  

**Remaining deliverables:**

- context-switch contract surface  
- idle thread contract  

---

## Context-switch contract surface

Defines the architecture-visible interface required to transfer execution state between threads.

This task declares the contract only.  
It does not implement hardware switching logic beyond minimal stubs.

**Required elements:**

- `arch_cpu_state_init()` declaration  
- `arch_cpu_state_switch()` declaration  
- documented calling context and ownership rules  
- explicit statement of what state is preserved across a switch  
- stub implementations for the active architecture target  
- compile-time verification of contract presence  

**Not included:**

- preemption  
- interrupt-driven scheduling  
- SMP coordination  
- performance tuning  

---

## Idle thread contract

Defines the structural role of the idle thread once scheduling and switching surfaces exist.

**Required elements:**

- idle thread identity and lifetime rules  
- initialization contract  
- relationship to scheduler start sequence  
- explicit statement that idle execution is not scheduling policy  

**Not included:**

- power management behavior  
- CPU sleep policy  
- load balancing  
- timer-driven scheduling  

---

**Constraints:**

- no advanced scheduling policy  
- no load balancing  
- no performance tuning  
- no preemption  
- no SMP behavior  

**Scheduler bootstrap boundary:**  
The initial current thread is established by the context-switch bootstrap path defined in the context-switch contract surface.  
No scheduler behavior may assume a valid current thread before that boundary is executed.

**Phase completion rule:**  
Thread lifecycle, scheduler entry points, and context-switch surfaces exist and are mechanically reviewable without policy complexity.

---

# Phase 5 — Userspace transition

**Purpose:**  
Create the first controlled boundary between kernel and userspace.

**Deliver:**

- syscall entry contract  
- process structure  
- address space ownership contract  
- syscall dispatch stub  
- ELF loader contract  
- init process launch path  

**Constraints:**

- no compatibility claims yet  
- no complex process management  
- no performance optimization  

**Phase completion rule:**  
A deterministic userspace entry boundary exists.

---

# Phase 6 — Unix base semantics

**Purpose:**  
Establish the structural foundation required for Unix-style process and I/O behavior.

**Deliver:**

- file descriptor table  
- VFS core contract  
- pipe contract  
- signal contract  
- session and process group structures  
- terminal and PTY contract surfaces  

**Constraints:**

- no filesystem feature breadth  
- no networking behavior expansion  
- no compatibility claims  

**Phase completion rule:**  
Core Unix semantics exist structurally and are testable.

---

# Phase 7 — Shell viability

**Purpose:**  
Enable interactive shell behavior through minimal, deterministic kernel support.

**Deliver:**

- interactive console path  
- terminal control behavior  
- job control support  
- process signal integration  
- regression tests for shell-critical behavior  

**Constraints:**

- no advanced userland tooling  
- no feature expansion beyond shell requirements  

**Phase completion rule:**  
The kernel supports a functional interactive shell path.

---

# Phase 8 — Compatibility expansion

**Purpose:**  
Gradually increase userspace compatibility based on verified behavior.

**Deliver:**

- additional syscall coverage  
- filesystem compatibility improvements  
- device and driver expansion  
- compatibility regression testing  
- performance tuning based on measurement  

**Constraints:**

- no compatibility claims without test evidence  
- no premature optimization  

**Phase completion rule:**  
Compatibility is demonstrated through measurable behavior, not assumption.

---

# Global engineering rules

**Rule 1 — One task, one surface**  
Each task must modify only one subsystem surface.

**Rule 2 — Shape before behavior**  
Define structures before implementing policy.

**Rule 3 — Behavior before optimization**  
Optimize only after correctness is established.

**Rule 4 — Real hardware boundaries only**  
Keep a boundary only when the hardware is genuinely different.  
If the boundary exists only to make code look cleaner, reject it.

**Rule 5 — Common code may not assume architectural behavior not explicitly stated in a contract.**

**Rule 6 — No speculative expansion**  
Do not add fields, helpers, or abstractions until the next task requires them.

**Rule 7 — Deterministic stop condition**  
At the end of each task, the repository must remain:

- buildable  
- reviewable  
- deterministic  
- scope-bounded  

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
