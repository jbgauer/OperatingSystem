//definitions for filesys_driver.c
#include "types.h"

//prevent magic nums
#define MAX_ENTRIES         63      //1 entry is the boot block, so 62 files max
#define MAX_BLOCKS          1023
#define MAX_FILENAME_LEN    32      //32 + 3 (3 is end character)

#define MAX_FILE_SIZE   4190208 //4 MB
#define BLOCK_SIZE  4096

//file supports
//~~ INODE
// for actual directories/folders, it is treated as a file
// except the data blocks will point to inodes of the inside files
typedef struct {
    uint32_t length; //file length in Bytes
    uint32_t data_block_num[MAX_BLOCKS];
    // ^ data block number for each block of the file
} inode_t;

//~~ DENTRY (directory entry)
// basically the file type
typedef struct {
    uint8_t filename[MAX_FILENAME_LEN];  //file name of this entry
    uint32_t filetype;   //file type of this entry
    uint32_t inode_num;  //inode number for this entry
    int8_t reserved[24]; //idk
} dentry_t;

//~~ BOOT BLOCK is its own special struct
typedef struct {
    uint32_t dentry_count;   //num of directory entries
    uint32_t inode_count;    //num of inodes
    uint32_t block_count;    //num of data blocks
    uint8_t reserved[52];    //reserved space, can't use?
    dentry_t entries[MAX_ENTRIES];   //array of all file entries
} boot_block_t;

//~~ DATA BLOCK because the slides said so (use for pointer)
// data blocks hold 4096 chars
typedef struct {
    uint8_t actualdata[BLOCK_SIZE];
} data_block_t;

//^ initialize all this stuff for the file system driver
void filesys_init(uint32_t memstart);

//utility fuctions for file reading
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

//fuctions for system calls (comes from appendix A)
int32_t open_file(const uint8_t* filename, int fd);
int32_t close_file(int32_t fd);
int32_t read_file(int32_t fd, void* buf, int32_t nbytes);
int32_t write_file(int32_t fd, const void* buf, int32_t nbytes);

int32_t open_dir(const uint8_t* filename);
int32_t close_dir(int32_t fd);
int32_t read_dir(int32_t fd, void* buf, int32_t nbytes);
int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes);
