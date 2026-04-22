/*
 * core/lifecycle_governance_schema.c
 *
 * Descriptive, inert governance schema summary for governance surfaces.
 */

#include <kernul/lifecycle_governance_schema.h>

#define LIFECYCLE_GOVERNANCE_SCHEMA_FIRST_PHASE 31U
#define LIFECYCLE_GOVERNANCE_SCHEMA_LAST_PHASE 43U
#define LIFECYCLE_GOVERNANCE_SCHEMA_STRUCTURED_SURFACE_COUNT 11U

static const char FIELD_ROLE_STATE_FLAG[] = "state_flag";
static const char FIELD_ROLE_STATE_VALUE[] = "state_value";
static const char FIELD_ROLE_STATUS_CODE[] = "status_code";
static const char FIELD_ROLE_TIMESTAMP[] = "timestamp";
static const char FIELD_ROLE_SURFACE_REFERENCE[] = "surface_reference";
static const char FIELD_ROLE_VERSION_REFERENCE[] = "version_reference";
static const char FIELD_ROLE_BUFFER_POINTER[] = "buffer_pointer";
static const char FIELD_ROLE_BUFFER_CAPACITY[] = "buffer_capacity";
static const char FIELD_ROLE_BUFFER_LENGTH[] = "buffer_length";
static const char FIELD_ROLE_PHASE_REFERENCE[] = "phase_reference";
static const char FIELD_ROLE_DESCRIPTOR_TEXT[] = "descriptor_text";

static const char *const lifecycle_governance_schema_field_role_vocabulary[] = {
    FIELD_ROLE_STATE_FLAG,
    FIELD_ROLE_STATE_VALUE,
    FIELD_ROLE_STATUS_CODE,
    FIELD_ROLE_TIMESTAMP,
    FIELD_ROLE_SURFACE_REFERENCE,
    FIELD_ROLE_VERSION_REFERENCE,
    FIELD_ROLE_BUFFER_POINTER,
    FIELD_ROLE_BUFFER_CAPACITY,
    FIELD_ROLE_BUFFER_LENGTH,
    FIELD_ROLE_PHASE_REFERENCE,
    FIELD_ROLE_DESCRIPTOR_TEXT,
};

static const lifecycle_governance_schema_field_t lifecycle_introspection_fields[] = {
    { .field_name = "finalized", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "ready", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "owner_id", .field_type = "uint64_t", .field_role = FIELD_ROLE_STATE_VALUE },
    { .field_name = "reclaimed", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "recovery_eligible", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "recovery_authorized", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "recovery_executed", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "recovery_execution_result", .field_type = "int", .field_role = FIELD_ROLE_STATUS_CODE },
    { .field_name = "recovery_outcome", .field_type = "int", .field_role = FIELD_ROLE_STATE_VALUE },
    { .field_name = "retry_policy", .field_type = "int", .field_role = FIELD_ROLE_STATE_VALUE },
    { .field_name = "retry_authorized", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "retry_executed", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "retry_execution_result", .field_type = "int", .field_role = FIELD_ROLE_STATUS_CODE },
    { .field_name = "retry_outcome", .field_type = "int", .field_role = FIELD_ROLE_STATE_VALUE },
};

static const lifecycle_governance_schema_field_t lifecycle_provenance_fields[] = {
    { .field_name = "finalization_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "readiness_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "ownership_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "reclamation_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "recovery_eligibility_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "recovery_authorization_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "recovery_execution_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "recovery_outcome_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "retry_policy_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "retry_authorization_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "retry_execution_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
    { .field_name = "retry_outcome_timestamp", .field_type = "uint64_t", .field_role = FIELD_ROLE_TIMESTAMP },
};

static const lifecycle_governance_schema_field_t lifecycle_assertion_fields[] = {
    { .field_name = "require_finalized", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "require_ready", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "require_verified", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "require_recovery_complete", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
    { .field_name = "require_retry_complete", .field_type = "bool", .field_role = FIELD_ROLE_STATE_FLAG },
};

static const lifecycle_governance_schema_field_t lifecycle_documentation_fields[] = {
    { .field_name = "buffer", .field_type = "char *", .field_role = FIELD_ROLE_BUFFER_POINTER },
    { .field_name = "capacity", .field_type = "size_t", .field_role = FIELD_ROLE_BUFFER_CAPACITY },
    { .field_name = "length", .field_type = "size_t", .field_role = FIELD_ROLE_BUFFER_LENGTH },
};

static const lifecycle_governance_schema_field_t lifecycle_audit_fields[] = {
    { .field_name = "snapshot", .field_type = "lifecycle_snapshot_t", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "verify_result", .field_type = "lifecycle_verify_result_t", .field_role = FIELD_ROLE_STATUS_CODE },
    { .field_name = "provenance", .field_type = "lifecycle_provenance_t", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "assertion", .field_type = "lifecycle_assert_result_t", .field_role = FIELD_ROLE_STATUS_CODE },
    { .field_name = "freeze_version", .field_type = "uint32_t", .field_role = FIELD_ROLE_VERSION_REFERENCE },
};

static const lifecycle_governance_schema_field_t lifecycle_export_fields[] = {
    { .field_name = "buffer", .field_type = "char *", .field_role = FIELD_ROLE_BUFFER_POINTER },
    { .field_name = "capacity", .field_type = "size_t", .field_role = FIELD_ROLE_BUFFER_CAPACITY },
    { .field_name = "length", .field_type = "size_t", .field_role = FIELD_ROLE_BUFFER_LENGTH },
};

static const lifecycle_governance_schema_field_t lifecycle_extension_fields[] = {
    { .field_name = "id", .field_type = "const char *", .field_role = FIELD_ROLE_DESCRIPTOR_TEXT },
    { .field_name = "target_freeze_version", .field_type = "uint32_t", .field_role = FIELD_ROLE_VERSION_REFERENCE },
    { .field_name = "description", .field_type = "const char *", .field_role = FIELD_ROLE_DESCRIPTOR_TEXT },
};

static const lifecycle_governance_schema_field_t lifecycle_governance_fields[] = {
    { .field_name = "freeze_version", .field_type = "uint32_t", .field_role = FIELD_ROLE_VERSION_REFERENCE },
    { .field_name = "introspection_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "verification_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "provenance_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "assertion_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "documentation_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "audit_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "export_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "extension_surface", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
};

static const lifecycle_governance_schema_field_t lifecycle_governance_index_fields[] = {
    { .field_name = "surface_name", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "phase_number", .field_type = "uint32_t", .field_role = FIELD_ROLE_PHASE_REFERENCE },
};

static const lifecycle_governance_schema_field_t lifecycle_governance_provenance_fields[] = {
    { .field_name = "phase_number", .field_type = "uint32_t", .field_role = FIELD_ROLE_PHASE_REFERENCE },
    { .field_name = "surface_name", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "purpose", .field_type = "const char *", .field_role = FIELD_ROLE_DESCRIPTOR_TEXT },
    { .field_name = "origin", .field_type = "const char *", .field_role = FIELD_ROLE_DESCRIPTOR_TEXT },
};

static const lifecycle_governance_schema_field_t lifecycle_governance_compatibility_fields[] = {
    { .field_name = "phase_number", .field_type = "uint32_t", .field_role = FIELD_ROLE_PHASE_REFERENCE },
    { .field_name = "surface_name", .field_type = "const char *", .field_role = FIELD_ROLE_SURFACE_REFERENCE },
    { .field_name = "compatible_freeze_version", .field_type = "uint32_t", .field_role = FIELD_ROLE_VERSION_REFERENCE },
    { .field_name = "compatibility_notes", .field_type = "const char *", .field_role = FIELD_ROLE_DESCRIPTOR_TEXT },
};

const lifecycle_governance_schema_entry_t lifecycle_governance_schema[] = {
    {
        .phase_number = 31U,
        .surface_name = "Lifecycle introspection surface",
        .fields = lifecycle_introspection_fields,
        .field_count = sizeof(lifecycle_introspection_fields) / sizeof(lifecycle_introspection_fields[0]),
    },
    {
        .phase_number = 33U,
        .surface_name = "Lifecycle provenance surface",
        .fields = lifecycle_provenance_fields,
        .field_count = sizeof(lifecycle_provenance_fields) / sizeof(lifecycle_provenance_fields[0]),
    },
    {
        .phase_number = 34U,
        .surface_name = "Lifecycle boundary assertion surface",
        .fields = lifecycle_assertion_fields,
        .field_count = sizeof(lifecycle_assertion_fields) / sizeof(lifecycle_assertion_fields[0]),
    },
    {
        .phase_number = 36U,
        .surface_name = "Lifecycle documentation synthesis surface",
        .fields = lifecycle_documentation_fields,
        .field_count = sizeof(lifecycle_documentation_fields) / sizeof(lifecycle_documentation_fields[0]),
    },
    {
        .phase_number = 37U,
        .surface_name = "Lifecycle audit surface",
        .fields = lifecycle_audit_fields,
        .field_count = sizeof(lifecycle_audit_fields) / sizeof(lifecycle_audit_fields[0]),
    },
    {
        .phase_number = 38U,
        .surface_name = "Lifecycle export serialization surface",
        .fields = lifecycle_export_fields,
        .field_count = sizeof(lifecycle_export_fields) / sizeof(lifecycle_export_fields[0]),
    },
    {
        .phase_number = 39U,
        .surface_name = "Lifecycle extension declaration surface",
        .fields = lifecycle_extension_fields,
        .field_count = sizeof(lifecycle_extension_fields) / sizeof(lifecycle_extension_fields[0]),
    },
    {
        .phase_number = 40U,
        .surface_name = "Lifecycle governance consolidation surface",
        .fields = lifecycle_governance_fields,
        .field_count = sizeof(lifecycle_governance_fields) / sizeof(lifecycle_governance_fields[0]),
    },
    {
        .phase_number = 41U,
        .surface_name = "Governance index surface",
        .fields = lifecycle_governance_index_fields,
        .field_count = sizeof(lifecycle_governance_index_fields) / sizeof(lifecycle_governance_index_fields[0]),
    },
    {
        .phase_number = 42U,
        .surface_name = "Governance provenance surface",
        .fields = lifecycle_governance_provenance_fields,
        .field_count = sizeof(lifecycle_governance_provenance_fields) / sizeof(lifecycle_governance_provenance_fields[0]),
    },
    {
        .phase_number = 43U,
        .surface_name = "Governance compatibility surface",
        .fields = lifecycle_governance_compatibility_fields,
        .field_count = sizeof(lifecycle_governance_compatibility_fields) / sizeof(lifecycle_governance_compatibility_fields[0]),
    },
};

const size_t lifecycle_governance_schema_count =
    sizeof(lifecycle_governance_schema) / sizeof(lifecycle_governance_schema[0]);

_Static_assert((sizeof(lifecycle_governance_schema) / sizeof(lifecycle_governance_schema[0]))
                   == LIFECYCLE_GOVERNANCE_SCHEMA_STRUCTURED_SURFACE_COUNT,
               "governance schema count must match structured governance surfaces in phase range 31-43");

_Static_assert((sizeof(lifecycle_governance_schema_field_role_vocabulary)
                / sizeof(lifecycle_governance_schema_field_role_vocabulary[0]))
                   > 0U,
               "governance schema field role vocabulary must be non-empty");

_Static_assert(LIFECYCLE_GOVERNANCE_SCHEMA_FIRST_PHASE == 31U,
               "governance schema first phase must remain canonical");
_Static_assert(LIFECYCLE_GOVERNANCE_SCHEMA_LAST_PHASE == 43U,
               "governance schema last phase must remain canonical");
