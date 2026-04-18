/*
 * tests/console/test_lifecycle_governance_index_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_governance_index.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_governance_index.h>
#include <kernul/compiler.h>

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle_governance_index.h must not include freeze baseline headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_H
#error "lifecycle_governance_index.h must not include governance summary headers"
#endif

#ifdef KERNUL_SESSION_H
#error "lifecycle_governance_index.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_governance_index.h must not include scheduler headers"
#endif

KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_index_entry_t *)0)->phase_number) == sizeof(uint32_t),
                   "lifecycle governance index phase_number type drift");

static const lifecycle_governance_index_entry_t *const check_lifecycle_governance_index =
    lifecycle_governance_index;
static const size_t *const check_lifecycle_governance_index_count =
    &lifecycle_governance_index_count;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_governance_index_entry_t entry = {0};

    (void)check_lifecycle_governance_index;
    (void)check_lifecycle_governance_index_count;

    (void)entry.surface_name;
    (void)entry.phase_number;
}
