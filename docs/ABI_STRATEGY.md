# KERNUL ABI Strategy

Status: Authoritative  
Scope: Userspace ABI, syscall contract, compatibility direction  
Purpose: Define realistic compatibility goals and validation strategy

## 1. Position

KERNUL targets broad practical Linux userspace compatibility and POSIX-oriented behavior.

KERNUL does not claim universal compatibility at bootstrap.

Compatibility is delivered incrementally through:

- ABI definition
- syscall implementation
- userspace execution testing
- shell validation
- conformance work
- regression tracking

## 2. Compatibility layers

### 2.1 POSIX-oriented base

Initial focus:

- process model
- file descriptors
- pipes
- permissions
- signals
- sessions and process groups
- terminals and PTYs
- time and timers
- directory and pathname behavior

### 2.2 Linux userspace essentials

Target as needed:

- ELF loading behavior
- Linux-like syscall numbering/ABI strategy
- procfs/devfs surfaces where required
- futex-compatible primitives or equivalent compatibility strategy
- poll/select/epoll behavior
- signal delivery semantics relevant to real userspace

### 2.3 Shell viability

Z shell support requires correct behavior for:

- controlling terminals
- job control
- PTYs
- process groups
- signal propagation
- environment/argv/exec
- file descriptor conventions

## 3. Validation rules

No compatibility claim is accepted without one or more of:

- automated test evidence
- trace comparison
- userspace runtime verification
- documented conformance results

## 4. Non-goals at bootstrap

- universal binary compatibility claim
- complete ioctl compatibility
- total kernel-internal behavior mimicry
- unrestricted third-party userspace support promise

## 5. Production compatibility boundary

The active delivery sequence and release gates are in [ROADMAP.md](ROADMAP.md).
The `v1.0.0` goal is a working kernel and command-line system on documented
x86_64 and AArch64 targets, with desktop foundations available to userspace.
The full desktop UI follows the initial production release.

Implement the syscall and process semantics required by the selected real
userland build. Record its libc/shell configuration and supported operations;
the Z shell goal remains part of the compatibility workload. Missing behavior
must be established by an actual workload dependency rather than a speculative
list of Linux interfaces. Architecture-specific syscall numbering, entry/return
conventions, structure layout, pointer validation, and error semantics must be
documented before relying on the associated binaries.

Freeze only the documented, working ABI subset at the release-candidate gate.
Kernel-internal lifecycle snapshot/layout freezes are distinct from that public
userspace ABI. Neither hosted contract tests nor metadata consistency implies
Linux binary compatibility.

## 6. Desktop foundations

The pre-`v1.0.0` desktop foundation gate requires real userspace use of:

- protected mappings and shared-memory objects with explicit ownership;
- descriptor-based local IPC and blocking readiness notification;
- monotonic clocks and scheduler behavior suitable for a blocking event loop;
- authorized display-buffer mappings with validated geometry, format, size,
  permissions, and teardown;
- keyboard/pointer events with explicit lifetime and overflow behavior.

Reuse the process, memory, descriptor, permission, and event models already
needed by the command-line system. Introduce concrete interfaces only when the
display/input proof consumes them; reserve no speculative syscall numbers or
public structure fields for a hypothetical toolkit.

Keep rendering, compositor/client protocols, window policy, focus routing,
fonts, and application UI in userspace. They may evolve through versioned
userspace protocols above the stable kernel subset. GPU acceleration and broad
display/input driver coverage follow a demonstrated desktop workload.

Validation must prove that a userspace client draws to an authorized buffer,
shares a buffer with another process, waits for real input without busy polling,
and cleans up after exit or failure. A kernel-drawn boot logo does not satisfy
this ABI or establish desktop readiness.
