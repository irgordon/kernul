/*
 * tests/console/test_lifecycle_documentation_link.c
 *
 * Link-check for lifecycle documentation dependency boundary symbols.
 * Pass condition: successful link against documentation implementation.
 */

#include <kernul/lifecycle_documentation.h>

int main(void)
{
    char output[4096];
    lifecycle_documentation_t doc = {
        .buffer = output,
        .capacity = sizeof(output),
        .length = 0U,
    };

    if (lifecycle_generate_documentation(&doc) != LIFECYCLE_DOC_OK)
        return 1;
    if (doc.length == 0U)
        return 1;

    return 0;
}
