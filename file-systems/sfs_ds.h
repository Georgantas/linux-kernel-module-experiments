
#ifndef SFS_DS_H
#define SFS_DS_H

// This magic number is used to verify that we are dealing with the right file system, when operating on it.
#define SIMULA_FS_TYPE 0x13090D15 /* Magic Number for our file system */
// note the hard coded assumptions made about the block size of 512 bytes
#define SIMULA_FS_BLOCK_SIZE 512 /* in bytes */
#define SIMULA_FS_ENTRY_SIZE 64 /* in bytes */
#define SIMULA_FS_DATA_BLOCK_CNT ((SIMULA_FS_ENTRY_SIZE - (16 + 3 * 4)) / 4)

// a regular file is used to simulate a partition named ".sfsf"
#define SIMULA_DEFAULT_FILE ".sfsf"

typedef unsigned int uint4_t;

// The design mainly contains two structures:
// the super block containing the info about the file system,
// and the file entry structure containing the info about each
// file in the file system. 
typedef struct sfs_super_block
{
    uint4_t type; /* Magic number to identify the file system */
    uint4_t block_size; /* Unit of allocation */
    uint4_t partition_size; /* in blocks */
    uint4_t entry_size; /* in bytes */ 
    uint4_t entry_table_size; /* in blocks */
    uint4_t entry_table_block_start; /* in blocks */
    uint4_t entry_count; /* Total entries in the file system */
    uint4_t data_block_start; /* in blocks */
    uint4_t reserved[SIMULA_FS_BLOCK_SIZE / 4 - 8];
} sfs_super_block_t; /* Making it of SIMULA_FS_BLOCK_SIZE */

// every file contains these fields
typedef struct sfs_file_entry
{
    // name of the file
    char name[16];
    // size of the file
    uint4_t size; /* in bytes */
    // a timestamp that could represent creation time or last modification time
    uint4_t timestamp; /* Seconds since Epoch */
    uint4_t perms; /* Permissions for user */
    // added so the struct can be of SIMULA_FS_ENTRY_SIZE (64)
    uint4_t blocks[SIMULA_FS_DATA_BLOCK_CNT];
} sfs_file_entry_t;

#endif

