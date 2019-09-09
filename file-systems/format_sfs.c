
// This program is used to build our file system (like mkfs.vfat)
// To format:
// $ gcc format_sfs.c -o format_sfs
// $ ./format_sfs 1024 # Partition size in blocks of 512-bytes
// $ ls -al # List the .sfsf created with a size of 512 KiBytes

// Since everything is a file in linux, the program can easily
// be modified to instead format a block device like the one
// developed in the "block-device" folder.
// See: https://sysplay.github.io/books/LinuxDrivers/book/Content/Part21.html

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "sfs_ds.h"

#define SFS_ENTRY_RATIO 0.10 /* 10% of all blocks */
#define SFS_ENTRY_TABLE_BLOCK_START 1

sfs_super_block_t sb =
{
    .type = SIMULA_FS_TYPE,
    .block_size = SIMULA_FS_BLOCK_SIZE,
    .entry_size = SIMULA_FS_ENTRY_SIZE,
    // the entry table starts in the block right after the super block
    .entry_table_block_start = SFS_ENTRY_TABLE_BLOCK_START
};
sfs_file_entry_t fe; /* All 0's */

void write_super_block(int sfs_handle, sfs_super_block_t *sb)
{
    write(sfs_handle, sb, sizeof(sfs_super_block_t));
}

// Right now its clearing the file entries one by one, i.e. writing 64-byte
// sized file entries one by one – that’s pretty non-optimal. A better
// approach would be to fill up a block with such entries, and then write
// these blocks one by one. In case of a 512-byte block (i.e.
// SIMULA_FS_BLOCK_SIZE defined as 512), that would mean 8 file entries
// in a 512-byte block and then writing these 512-byte blocks one by one.
// This is an advantage of using dd over cat when writing to a block device.
// However, this doesn't seem to matter with modern block drivers:
// https://unix.stackexchange.com/questions/9432/is-there-a-way-to-determine-the-optimal-value-for-the-bs-parameter-to-dd/9492#9492
void clear_file_entries(int sfs_handle, sfs_super_block_t *sb)
{
    int i;

    for (i = 0; i < sb->entry_count; i++)
    {
        write(sfs_handle, &fe, sizeof(fe));
    }
}
void mark_data_blocks(int sfs_handle, sfs_super_block_t *sb)
{
    char c = 0;

    lseek(sfs_handle, sb->partition_size * sb->block_size - 1, SEEK_SET);
    write(sfs_handle, &c, 1); /* To make the file size to partition size */
}

int main(int argc, char *argv[])
{
    int sfs_handle;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <partition size in 512-byte blocks>\n",
            argv[0]);
        return 1;
    }
    // "atoi" converts string to integer
    sb.partition_size = atoi(argv[1]);
    sb.entry_table_size = sb.partition_size * SFS_ENTRY_RATIO;
    sb.entry_count = sb.entry_table_size * sb.block_size / sb.entry_size;
    sb.data_block_start = SFS_ENTRY_TABLE_BLOCK_START + sb.entry_table_size;

    sfs_handle = creat(SIMULA_DEFAULT_FILE,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (sfs_handle == -1)
    {
        perror("No permissions to format");
        return 2;
    }

    // To see this pictorially, check pic_of_the_fs.png
    // Its first block as the super block, using write_super_block()
    write_super_block(sfs_handle, &sb);
    // The next 10% of total blocks as the file entry’s zeroed out table
    clear_file_entries(sfs_handle, &sb);
    // And the remaining as the data blocks, using mark_data_blocks().
    // This basically writes a zero at the end, to actually extend the
    // underlying file .sfsf to the partition size
    mark_data_blocks(sfs_handle, &sb);
    close(sfs_handle);
    return 0;
}

