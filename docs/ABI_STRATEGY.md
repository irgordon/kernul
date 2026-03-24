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
