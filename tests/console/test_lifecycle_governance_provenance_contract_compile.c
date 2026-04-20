/*
 * tests/console/test_lifecycle_governance_provenance_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_governance_provenance.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_governance_provenance.h>
#include <kernul/compiler.h>

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle_governance_provenance.h must not include freeze baseline headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#error "lifecycle_governance_provenance.h must not include governance index headers"
#endif

#ifdef KERNUL_SESSION_H
#error "lifecycle_governance_provenance.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_governance_provenance.h must not include scheduler headers"
#endif

KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_provenance_entry_t *)0)->phase_number) == sizeof(uint32_t),
                   "lifecycle governance provenance phase_number type drift");

static const lifecycle_governance_provenance_entry_t *const check_lifecycle_governance_provenance =
    lifecycle_governance_provenance;
static const size_t *const check_lifecycle_governance_provenance_count =
    &lifecycle_governance_provenance_count;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_governance_provenance_entry_t entry = {0};

    (void)check_lifecycle_governance_provenance;
    (void)check_lifecycle_governance_provenance_count;

    (void)entry.phase_number;
    (void)entry.surface_name;
    (void)entry.purpose;
    (void)entry.origin;
}
