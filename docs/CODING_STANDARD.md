# KERNUL C Coding Standard

Status: Authoritative  
Scope: All C and header code  
Purpose: Maximize readability, consistency, reviewability, and runtime clarity

## 1. General principles

KERNUL code must be:

- readable top-down
- explicit
- mechanically understandable
- cost-aware
- human-reviewable

Code must favor:

- simple control flow
- explicit state
- explicit ownership
- explicit locking
- explicit failure handling

Code must avoid:

- hidden work
- decorative abstraction
- deep nesting
- unclear naming
- surprising side effects

## 2. Language scope

Primary implementation language: C

Assembly is permitted only for:

- entry code
- low-level context switching
- trap/interrupt transitions
- architecture-required primitives

Do not introduce C++-style patterns into C.

## 3. File structure

Source files should generally appear in this order:

1. file header comment
2. includes
3. local macros/constants
4. forward declarations
5. internal types
6. internal helpers
7. public/internal entry points
8. private cleanup or support routines

A reader should understand the file from top to bottom.

## 4. Naming

### 4.1 General rules

Names must be descriptive and consistent.

Use:

- `snake_case` for functions and variables
- `UPPER_CASE` for macros and constants
- subsystem prefixes where needed for clarity
- nouns for objects
- verbs for actions

### 4.2 Examples

Good:

- `sched_pick_next_thread`
- `vfs_lookup_path`
- `mm_map_region`
- `irq_disable_local`

Bad:

- `doit`
- `handle`
- `mgr`
- `stuff`
- `tmp2`

Add the following section **directly under section `4. Naming`** in `docs/CODING_STANDARD.md`.
This update formalizes the new guideline: **no mandatory project prefix**, prefix only when it serves a technical purpose.
The language is written to be enforceable during review and consistent with kernel conventions.

## 4.3 Prefixing and Namespace Discipline

Project-wide prefixes are not required for internal kernel symbols.

KERNUL relies on:

* directory structure
* subsystem boundaries
* header scoping
* linker namespace control

to manage symbol organization.

Over-prefixing reduces readability and increases cognitive load without improving correctness.

Names must be short, descriptive, and context-aware.

### 4.3.1 Default rule

Do **not** prepend the project name to every function or type.

Prefer:

```c
kernel_main()
panic()
assert_fail()
boot_validate()
init_subsystems()
sched_enqueue()
mm_map_region()
vfs_lookup()
```

Avoid:

```c
kernul_kernel_main()
kernul_panic()
kernul_assert_fail()
kernul_boot_validate()
kernul_memory_manager_initialize()
```

The repository already defines the namespace.

Redundant prefixes are noise.

### 4.3.2 When prefixes are required

Use a prefix only when it prevents ambiguity or reflects subsystem ownership.

Prefixes are required when:

* the symbol is exported outside its subsystem
* the symbol is part of the userspace ABI
* the symbol may collide with another subsystem
* the symbol represents a hardware or architecture boundary
* the symbol is globally visible across the kernel

### 4.3.3 Standard subsystem prefixes

Use subsystem prefixes consistently.

Examples:

```c
sched_pick_next_thread()
sched_enqueue_thread()

mm_alloc_pages()
mm_free_pages()

vfs_lookup_path()
vfs_mount()

net_send_packet()
net_receive_packet()

ipc_send_signal()
ipc_wait_event()

arch_switch_context()
arch_disable_interrupts()

sys_read()
sys_write()
sys_futex()
```

These prefixes communicate ownership, not branding.

### 4.3.4 Userspace ABI naming

Userspace-visible interfaces must use stable, conventional prefixes.

Examples:

```c
sys_open()
sys_close()
sys_execve()
sys_futex()
```

These names form part of the ABI surface.

ABI names must remain stable once released.

### 4.3.5 Local helper naming

Internal helpers may omit prefixes when context is obvious.

Example:

```c
static int validate_header(...)
static void release_buffer(...)
static bool node_is_empty(...)
```

Do not add prefixes to functions that are:

* file-local
* private to a subsystem
* not exported
* not part of an interface contract

### 4.3.6 Type naming

Structure and type names follow the same discipline.

Prefer:

```c
struct thread
struct process
struct run_queue
struct page
```

Use prefixes only when necessary:

```c
struct sched_class
struct vfs_node
struct net_device
struct arch_cpu_state
```

Avoid:

```c
struct kernul_thread
struct kernul_process
```

### 4.3.7 Review rule

During code review, reject naming that:

* redundantly prefixes symbols with the project name
* obscures meaning
* introduces inconsistent subsystem naming
* creates unnecessary verbosity
* conflicts with existing naming patterns

Accept naming that:

* clearly communicates responsibility
* matches subsystem conventions
* improves readability
* reduces ambiguity

### 4.3.8 Guiding principle

Use prefixes to clarify ownership.

Do not use prefixes to advertise the project.

Readable code is more important than uniform branding.


## 5. Functions

### 5.1 Function size

Functions should be small enough to reason about in one pass.

Guidelines:

- hot-path functions should be especially short
- split only when the split improves clarity
- do not create wrappers that hide cost or meaning

### 5.2 Function structure

A function should make these things obvious:

- inputs
- state assumptions
- locking assumptions
- ownership effects
- failure conditions
- output/result

### 5.3 Parameter order

Prefer stable parameter ordering:

1. primary object/context
2. secondary object/context
3. operation parameters
4. output pointers
5. flags

Example:

```c
int vfs_lookup_path(struct vfs_mount *mount,
                    const char *path,
                    unsigned int flags,
                    struct vfs_node **out_node);
````

## 6. Control flow

Prefer:

* early returns for invalid input
* simple if/else
* bounded loops
* cleanup labels when resource unwinding is needed

Avoid:

* deeply nested branches
* implicit fallthrough without comment
* hidden state transitions
* clever ternary expressions in complex code

## 7. Error handling

Use explicit status codes.

Validate inputs early.
Fail fast on invalid state.
Use structured cleanup when partial work has been done.

Example:

```c
int device_init(struct device *dev)
{
    int rc;

    if (dev == NULL)
        return -EINVAL;

    rc = device_map_resources(dev);
    if (rc != 0)
        return rc;

    rc = device_setup_irq(dev);
    if (rc != 0)
        goto err_unmap_resources;

    return 0;

err_unmap_resources:
    device_unmap_resources(dev);
    return rc;
}
```

## 8. Structs

Structure layout should reflect actual use.

Prefer:

* frequently-read fields near each other
* immutable-after-init fields separated from mutable fields when useful
* lock-protected fields grouped where practical
* comments on fields only when meaning is not obvious

Every important struct must have a clear ownership and lifetime model.

## 9. Pointers and ownership

Ownership rules must be explicit.

Use comments and API docs to distinguish:

* owned pointer
* borrowed pointer
* transferred ownership
* shared/refcounted pointer

Implicit ownership transfer is forbidden.

## 10. Concurrency and locking

Code that touches shared state must make synchronization visible.

Required clarity:

* what lock protects the state
* whether the function assumes the lock is held
* whether the function may sleep
* whether the function is IRQ-safe
* whether the function may run on multiple CPUs concurrently

Naming should help expose context where useful, for example:

* `_locked`
* `_irqsave`
* `_atomic`
* `_nocheck`

Do not overuse suffixes. Use them only when they prevent ambiguity.

## 11. Macros

Use macros sparingly.

Allowed uses:

* compile-time constants
* register/bit definitions
* tiny, obvious, side-effect-safe helpers where inline functions are unsuitable
* conditional build control

Prefer `static inline` functions over function-like macros.

Function-like macros that evaluate arguments multiple times are forbidden.

## 12. Inline functions

Use `static inline` for:

* tiny helpers
* type-safe wrappers
* architecture-sensitive operations requiring compile-time placement

Do not mark large functions inline.

## 13. Constants and flags

Use named constants.
Use bit flags explicitly.
Do not use unexplained numeric literals in logic.

Bad:

```c
if (state & 8)
```

Good:

```c
if (state & THREAD_FLAG_RUNNABLE)
```

## 14. Header rules

Headers define interfaces, not implementations.

Headers should contain only:

* declarations
* necessary type definitions
* constants/macros
* small inline functions when justified

Do not place unrelated private helpers in public headers.

## 15. Includes

Keep includes minimal and direct.

Rules:

* include what you use
* do not rely on indirect includes
* prefer local subsystem headers before broad umbrella headers where practical

## 16. Boolean and condition style

Conditions must be readable.

Prefer:

```c
if (node == NULL)
if (rc != 0)
if (!thread_is_runnable(thread))
```

Avoid compressed or ambiguous conditions.

## 17. Switch statements

Use `switch` for real state selection, not style preference.

Rules:

* each case should be short
* comment intentional fallthrough
* include `default` unless exhaustive handling is provably enforced elsewhere

## 18. Comments in code

Comments must explain:

* invariants
* ownership
* locking
* ordering
* hardware constraints
* why the code is shaped this way

Comments must not narrate obvious syntax.

Bad:

```c
/* Increment i */
i++;
```

Good:

```c
/*
 * Sequence must advance before publication so readers never observe
 * initialized data under an old generation value.
 */
obj->generation++;
```

## 19. Formatting baseline

Use a conventional, industry-standard C style:

* 4-space indentation
* no tabs in committed C source unless project tooling requires tabs
* opening brace on the next line for functions
* single-line conditionals may omit braces only for one simple statement
* braces required for multi-line branches
* one declaration per line when clarity matters
* avoid vertically dense code

Example:

```c
int sched_enqueue_thread(struct run_queue *rq, struct thread *thread)
{
    if (rq == NULL || thread == NULL)
        return -EINVAL;

    spin_lock(&rq->lock);
    list_add_tail(&thread->runq_node, &rq->threads);
    spin_unlock(&rq->lock);

    return 0;
}
```

## 20. Performance-sensitive code

Hot-path code must make cost obvious.

Reviewers should be able to see:

* allocations
* locking
* barriers
* atomics
* branch-heavy logic
* indirect calls

Hot-path cleverness without measurement is forbidden.

## 21. Prohibited patterns

* hidden allocation in helper APIs
* hidden locking in public APIs
* implicit ownership transfer
* multipurpose “manager” objects without defined responsibility
* meaningless wrapper layers
* deep callback forests in hot paths
* unscoped global mutable state
* unexplained magic numbers

## 22. Required review questions

Every non-trivial patch should be reviewable against:

1. What owns this state?
2. What synchronizes this state?
3. Can this sleep?
4. Can this allocate?
5. Is the caller cost obvious?
6. Is the failure mode explicit?
7. Is the code simpler than the alternative?
