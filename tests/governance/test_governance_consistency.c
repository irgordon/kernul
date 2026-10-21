/*
 * tests/governance/test_governance_consistency.c
 *
 * Phase 46 integration verification of immutable governance metadata.
 * Each table must describe the same phase identities and freeze baseline.
 * All references are borrowed for the test process lifetime; no kernel
 * state, publication markers, or execution paths are changed.
 */

#include <kernul/lifecycle_freeze_baseline.h>
#include <kernul/lifecycle_governance_index.h>
#include <kernul/lifecycle_governance_provenance.h>
#include <kernul/lifecycle_governance_compatibility.h>
#include <kernul/lifecycle_governance_schema.h>
#include <kernul/lifecycle_governance_manifest.h>

#include <assert.h>
#include <stdlib.h>

enum {
    FIRST_ENTRY = 0,
    FIRST_GOVERNANCE_PHASE = 31,
    GOVERNANCE_SURFACE_COUNT = 10,
    FIRST_METADATA_PHASE = 41,
    LAST_SCHEMA_PHASE = 43,
    STRUCTURED_SURFACE_COUNT = 11,
    METADATA_SURFACE_COUNT = 4,
    TEXT_EQUAL = 0,
};

struct metadata_identity {
    uint32_t phase_number;
    const char *role;
};

static const struct metadata_identity metadata_identities[] = {
    { 41U, "index" },
    { 42U, "provenance" },
    { 43U, "compatibility" },
    { 44U, "schema" },
};

static void verify_phase_coverage(void);
static void verify_surface_identity(void);
static void verify_freeze_association(void);
static void verify_schema_identity(void);
static void verify_manifest_identity(void);
static void require_count(size_t actual, size_t expected);
static void require_number(uint32_t actual, uint32_t expected);
static void require_range(uint32_t actual, uint32_t first, uint32_t last);
static void require_text(const char *actual, const char *expected);
static bool text_matches(const char *left, const char *right);

int main(void)
{
    verify_phase_coverage();
    verify_surface_identity();
    verify_freeze_association();
    verify_schema_identity();
    verify_manifest_identity();
    return EXIT_SUCCESS;
}

static void verify_phase_coverage(void)
{
    require_count(lifecycle_governance_index_count, GOVERNANCE_SURFACE_COUNT);
    require_count(lifecycle_governance_provenance_count, lifecycle_governance_index_count);
    require_count(lifecycle_governance_compatibility_count, lifecycle_governance_index_count);
    require_count(lifecycle_governance_schema_count, STRUCTURED_SURFACE_COUNT);
    require_count(lifecycle_governance_manifest_count, METADATA_SURFACE_COUNT);
}

static void verify_surface_identity(void)
{
    size_t position;

    for (position = FIRST_ENTRY; position < lifecycle_governance_index_count; position++) {
        const lifecycle_governance_index_entry_t *identity = &lifecycle_governance_index[position];
        const lifecycle_governance_provenance_entry_t *provenance = &lifecycle_governance_provenance[position];
        const lifecycle_governance_compatibility_entry_t *compatibility = &lifecycle_governance_compatibility[position];

        require_number(identity->phase_number, FIRST_GOVERNANCE_PHASE + (uint32_t)position);
        require_number(provenance->phase_number, identity->phase_number);
        require_text(provenance->surface_name, identity->surface_name);
        require_number(compatibility->phase_number, identity->phase_number);
        require_text(compatibility->surface_name, identity->surface_name);
    }
}

static void verify_freeze_association(void)
{
    size_t position;

    for (position = FIRST_ENTRY; position < lifecycle_governance_compatibility_count; position++) {
        const lifecycle_governance_compatibility_entry_t *entry = &lifecycle_governance_compatibility[position];

        require_number(entry->compatible_freeze_version, LIFECYCLE_FREEZE_VERSION);
    }
}

static void verify_schema_identity(void)
{
    size_t position;

    for (position = FIRST_ENTRY; position < lifecycle_governance_schema_count; position++) {
        const lifecycle_governance_schema_entry_t *entry = &lifecycle_governance_schema[position];
        const char *canonical_name;

        require_range(entry->phase_number, FIRST_GOVERNANCE_PHASE, LAST_SCHEMA_PHASE);
        if (entry->phase_number < FIRST_METADATA_PHASE)
            canonical_name = lifecycle_governance_index[entry->phase_number - FIRST_GOVERNANCE_PHASE].surface_name;
        else
            canonical_name = lifecycle_governance_manifest[entry->phase_number - FIRST_METADATA_PHASE].surface_name;
        require_text(entry->surface_name, canonical_name);
    }
}

static void verify_manifest_identity(void)
{
    size_t position;

    for (position = FIRST_ENTRY; position < lifecycle_governance_manifest_count; position++) {
        const lifecycle_governance_manifest_entry_t *entry = &lifecycle_governance_manifest[position];
        const struct metadata_identity *identity = &metadata_identities[position];

        require_number(entry->phase_number, identity->phase_number);
        require_text(entry->surface_role, identity->role);
    }
}

/* Hosted assertion and text operations remain below domain verification. */
static void require_count(size_t actual, size_t expected)
{
    assert(actual == expected);
}

static void require_number(uint32_t actual, uint32_t expected)
{
    assert(actual == expected);
}

static void require_range(uint32_t actual, uint32_t first, uint32_t last)
{
    assert(actual >= first);
    assert(actual <= last);
}

static void require_text(const char *actual, const char *expected)
{
    assert(actual != NULL);
    assert(expected != NULL);
    assert(text_matches(actual, expected));
}

static bool text_matches(const char *left, const char *right)
{
    return __builtin_strcmp(left, right) == TEXT_EQUAL;
}
