/*
 * tests/console/test_lifecycle_governance_schema_contract_compile.c
 *
 * Compile-check translation unit for
 * include/kernul/lifecycle_governance_schema.h.
 * Pass condition: successful compilation.
 */

#include <kernul/lifecycle_governance_schema.h>
#include <kernul/compiler.h>

#ifdef KERNUL_LIFECYCLE_FREEZE_BASELINE_H
#error "lifecycle_governance_schema.h must not include freeze baseline headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_INDEX_H
#error "lifecycle_governance_schema.h must not include governance index headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_PROVENANCE_H
#error "lifecycle_governance_schema.h must not include governance provenance headers"
#endif

#ifdef KERNUL_LIFECYCLE_GOVERNANCE_COMPATIBILITY_H
#error "lifecycle_governance_schema.h must not include governance compatibility headers"
#endif

#ifdef KERNUL_SESSION_H
#error "lifecycle_governance_schema.h must not include session internals"
#endif

#ifdef KERNUL_SCHED_H
#error "lifecycle_governance_schema.h must not include scheduler headers"
#endif

KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_schema_entry_t *)0)->phase_number) == sizeof(uint32_t),
                   "lifecycle governance schema phase_number type drift");
KERN_STATIC_ASSERT(sizeof(((lifecycle_governance_schema_entry_t *)0)->field_count) == sizeof(size_t),
                   "lifecycle governance schema field_count type drift");

static const lifecycle_governance_schema_entry_t *const check_lifecycle_governance_schema =
    lifecycle_governance_schema;
static const size_t *const check_lifecycle_governance_schema_count =
    &lifecycle_governance_schema_count;

static void use_checks(void) KERN_UNUSED;
static void use_checks(void)
{
    lifecycle_governance_schema_field_t field = {0};
    lifecycle_governance_schema_entry_t entry = {0};

    (void)check_lifecycle_governance_schema;
    (void)check_lifecycle_governance_schema_count;

    (void)field.field_name;
    (void)field.field_type;
    (void)field.field_role;

    (void)entry.phase_number;
    (void)entry.surface_name;
    (void)entry.fields;
    (void)entry.field_count;
}
