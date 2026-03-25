/*
 * tests/pipe/test_pipe_link.c
 *
 * Link-check for pipe contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/pipe.h>

int main(void)
{
    (void)&pipe_alloc;
    (void)&pipe_get;
    (void)&pipe_put;
    (void)&pipe_open_pair;
    return 0;
}
