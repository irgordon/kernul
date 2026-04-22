/*
 * tests/console/test_lifecycle_governance_compatibility_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_governance_compatibility.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_governance_compatibility.h>
#include <kernul/compiler.h>

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle_governance_compatibility.h must not include freeze baseline headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#error "lifecycle_governance_compatibility.h must not include governance index headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H
#error "lifecycle_governance_compatibility.h must not include governance provenance headers"
#endif

#ifdef KERNUL_SESSION_H
#error "lifecycle_governance_compatibility.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_governance_compatibility.h must not include scheduler headers"
#endif

KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_compatibility_entry_t *)0)->phase_number)
                       == sizeof(uint32_t),
                   "lifecycle governance compatibility phase_number type drift");
KERN_STATIC_ASSERT(
    sizeof(((lifecycle_governance_compatibility_entry_t *)0)->compatible_freeze_version)
        == sizeof(uint32_t),
    "lifecycle governance compatibility freeze version type drift");

static const lifecycle_governance_compatibility_entry_t *const
    check_lifecycle_governance_compatibility = lifecycle_governance_compatibility;
static const size_t *const check_lifecycle_governance_compatibility_count =
    &lifecycle_governance_compatibility_count;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_governance_compatibility_entry_t entry = {0};

    (void)check_lifecycle_governance_compatibility;
    (void)check_lifecycle_governance_compatibility_count;

    (void)entry.phase_number;
    (void)entry.surface_name;
    (void)entry.compatible_freeze_version;
    (void)entry.compatibility_notes;
}
