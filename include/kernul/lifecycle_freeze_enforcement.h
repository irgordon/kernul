#ifndef KERNUL_LIFECYCLE_FREEZE_ENFORCEMENT_H
#define KERNUL_LIFECYCLE_FREEZE_ENFORCEMENT_H

/*
 * include/kernul/lifecycle_freeze_enforcement.h
 *
 * Kernel-internal compile-time lifecycle freeze enforcement surface.
 * Phase 35, Task 1 boundary.
 */

/* Advisory marker macros. */
#define LIFECYCLE_FROZEN __attribute__((annotate("kernul_lifecycle_frozen")))
#define LIFECYCLE_FROZEN_FN __attribute__((annotate("kernul_lifecycle_frozen_fn")))

/* Static assertion helper. */
#define LIFECYCLE_FREEZE_ASSERT(cond, msg) _Static_assert((cond), msg)

extern const unsigned int lifecycle_freeze_enforcement_link_anchor;

#endif /* KERNUL_LIFECYCLE_FREEZE_ENFORCEMENT_H */
