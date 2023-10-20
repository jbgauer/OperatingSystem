// //definitions for filesys_driver.c

// //file supports
// //~~ BOOT BLOCK is its own special struct
// typedef struct {
//     int32_t dentry_count;   //num of directory entries
//     int32_t inode_count;    //num of inodes
//     int32_t block_count;    //num of data blocks
//     //int8_t reserved[52];    //reserved space, can't use?
//     dentry_t entries[63];   //array of all file entries

// } boot_block_t;

// //~~ INODE
// // for actual directories/folders, it is treated as a file
// // except the data blocks will point to inodes of the inside files
// typedef struct {
//     int32_t length; //file length in Bytes
//     int32_t data_block_num[1023];
//     // ^ data block number for each block of the file
// } inode_t;

// //~~ DENTRY (directory entry)
// // basically the file type
// typedef struct {
//     int8_t filename[100];  //file name of this entry
//     int32_t filetype;   //file type of this entry
//     int32_t inode_num;  //inode number for this entry
//     //int8_t reserved[24]; //idk

// } dentry_t;

// //^ initialize all this stuff for the file system driver
// void filesys_init();

// //utility fuctions for kernel
// int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);

// int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

// int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

// //fuctions for user?
