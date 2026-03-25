/*
 * tests/vfs/test_vfs_link.c
 *
 * Link-check for VFS core contract symbols.
 * Pass condition: successful link.
 */

#include <kernul/vfs.h>

int main(void)
{
    (void)&vfs_file_alloc;
    (void)&vfs_file_get;
    (void)&vfs_file_put;
    (void)&vfs_init;
    (void)&vfs_open;
    (void)&vfs_close;
    (void)&vfs_read;
    (void)&vfs_write;
    return 0;
}
