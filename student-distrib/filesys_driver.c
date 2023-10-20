// #include "filesys_driver.h"

// #define MAX_FILE_COUNT  62
// #define MAX_FILE_SIZE   4190208 //4 MB

// //global boot block to use for finding files
// boot_block_t bbl;

// //dentry = directory entry

// /*
//  * filesys_init
//  *   DESCRIPTION: initialize all the structs for the file system
//  *   INPUTS: none
//  *   OUTPUTS: none
//  *   RETURN VALUE: boot block filled with inodes and dentries
//  *   SIDE EFFECTS: initializes boot block variable
//  */
// void filesys_init(){
//     //rn have 1 directory and 14 files (fsdir)
//     bbl.dentry_count = 15;   //num of directory entries
//     bbl.inode_count = 15;    //num of inodes
//     bbl.block_count = 0;    //num of data blocks
//     // ^ depends on num of files, have to get somehow

//     //boot block itself is in the entries array as "."
//     dentry_t bbentry = {".", 0, -1};
//     bbl.entries[0] = bbentry;

//     //fill in all the other entries?
// }

// /*
//  * read_dentry_by_name
//  *   DESCRIPTION: scan thru entries in boot block array and find file name
//  *   INPUTS: file name and dentry
//  *   OUTPUTS: 0 if success, -1 if fail
//  *   RETURN VALUE: file data via dentry
//  *   SIDE EFFECTS: dentry is filled in with the file data
//  */
// int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){
//     uint8_t curname;
//     //go thru dir entries
//     int i;
//     for(i = 0; i < bbl.dentry_count; i++){
//         //get cur name
//         curname = (bbl.entries[i]).filename;

//         if(fname = curname){
//             //use the read by index function once found file
//             read_dentry_by_index((bbl.entries[i]).inode_num, dentry);
//             return 0;
//         }
//     }

//     //fail if file doesnt exist
//     return -1;
// }

// /*
//  * read_dentry_by_index
//  *   DESCRIPTION: 
//  *   INPUTS:
//  *   OUTPUTS: 0 if success, -1 if fail
//  *   RETURN VALUE: file data via dentry
//  *   SIDE EFFECTS: dentry is filled in with file data
//  */
// int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
//     //have the inode so just go there in "the array"
//     //WHERE IS THE ARRAY??
    
//     //fail if index doesnt exist
//     return -1;
// }

// /*
//  * read_data
//  *   DESCRIPTION:
//  *   INPUTS:
//  *   OUTPUTS: 0 if success, -1 if fail
//  *   RETURN VALUE:
//  *   SIDE EFFECTS:
//  */
// int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
//     //fail if inode number is invalid
//     return -1;
// }
