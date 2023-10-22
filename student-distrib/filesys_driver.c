#include "filesys_driver.h"
#include "lib.h"

//global boot block to use for finding files
boot_block_t* bbl; //points to the boot block address

//dentry = directory entry

/*
 * filesys_init
 *   DESCRIPTION: initialize all the structs for the file system
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: boot block filled with inodes and dentries
 *   SIDE EFFECTS: initializes boot block variable
 */
void filesys_init(uint32_t memstart){
    //set the bbl address
    bbl = (boot_block_t*)memstart;
    //everything else done automatically from the memstart
}

/*
 * read_dentry_by_name
 *   DESCRIPTION: scan thru entries in boot block array and find file name
 *   INPUTS: file name and dentry
 *   OUTPUTS: 0 if success, -1 if fail
 *   RETURN VALUE: file data via dentry
 *   SIDE EFFECTS: dentry is filled in with the file data
 */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){
    uint8_t curname;
    int i;

    //check inputs are valid
    int flen = strlen((char*)fname);
    if(flen < 0 || flen > MAX_FILENAME_LEN || fname == NULL || dentry == NULL){
        return -1;
    }

    //go thru dir entries
    for(i = 0; i < bbl->dentry_count; i++){
        //get cur name
        curname = (bbl->entries[i]).filename;

        //if fname = curname
        if(strncmp((int8_t*)fname, (int8_t*)curname, MAX_FILENAME_LEN) == 0){
            //use the read by index function once found file
            read_dentry_by_index(i, dentry);
            return 0;
        }
    }

    //fail if file doesnt exist
    return -1;
}

/*
 * read_dentry_by_index
 *   DESCRIPTION: returns the dentry at the index in the blocks (entries[i])
 *   INPUTS: index in bbl->entries and the dentry pointer to put data into
 *   OUTPUTS: 0 if success, -1 if fail
 *   RETURN VALUE: file data via dentry
 *   SIDE EFFECTS: dentry is filled in with file data
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
    //check inputs valid
    if(index < 0 || index >= bbl->dentry_count || dentry == NULL){
        return -1;
    }
    
    //set input dentry
    *dentry = bbl->entries[index];
    return 0;
}

/*
 * read_data
 *   DESCRIPTION: gets the data at a certain location and puts it into buf
 *   INPUTS: inode to get data from, offset from inode, buf to put data into,
        length of data to read (from offset to offset+length)
 *   OUTPUTS: 0 if success, -1 if fail
 *   RETURN VALUE:
 *   SIDE EFFECTS: data is put into buf
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    //fail if inode number is invalid
    return -1;
}
