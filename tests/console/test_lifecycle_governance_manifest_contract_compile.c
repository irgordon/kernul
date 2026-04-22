/*
 * tests/console/test_lifecycle_governance_manifest_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_governance_manifest.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_governance_manifest.h>
#include <kernul/compiler.h>

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle_governance_manifest.h must not include freeze baseline headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#error "lifecycle_governance_manifest.h must not include governance index headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H
#error "lifecycle_governance_manifest.h must not include governance provenance headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_COMPATIBILITY_H
#error "lifecycle_governance_manifest.h must not include governance compatibility headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_SCHEMA_H
#error "lifecycle_governance_manifest.h must not include governance schema headers"
#endif

#ifdef KERNUL_SESSION_H
#error "lifecycle_governance_manifest.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_governance_manifest.h must not include scheduler headers"
#endif

KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_manifest_entry_t *)0)->phase_number) == sizeof(uint32_t),
                   "lifecycle governance manifest phase_number type drift");
KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_manifest_entry_t *)0)->surface_name) == sizeof(const char *),
                   "lifecycle governance manifest surface_name type drift");
KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_manifest_entry_t *)0)->surface_role) == sizeof(const char *),
                   "lifecycle governance manifest surface_role type drift");

static const lifecycle_governance_manifest_entry_t *const check_lifecycle_governance_manifest =
    lifecycle_governance_manifest;
static const size_t *const check_lifecycle_governance_manifest_count =
    &lifecycle_governance_manifest_count;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_governance_manifest_entry_t entry = {0};

    (void)check_lifecycle_governance_manifest;
    (void)check_lifecycle_governance_manifest_count;

    (void)entry.phase_number;
    (void)entry.surface_name;
    (void)entry.surface_role;
}
