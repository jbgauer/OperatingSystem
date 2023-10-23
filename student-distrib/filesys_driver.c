#include "filesys_driver.h"
#include "lib.h"

//global boot block to use for finding files
boot_block_t* bbl; //points to the boot block address
//global pointers to start of inode and data block sections
inode_t* startinode;
data_block_t* startblock;

//fd array just for now because PCB not set up yet
dentry_t* filearray[6];
int bytecount[6];

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

    //pointers to start of inode and data block sections
    // (just for making thing easier)
    startinode = (inode_t*)(bbl+1);
    startblock = (data_block_t*)(startinode + (bbl->inode_count));
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
    uint8_t* curname;
    int i;

    //check inputs are valid
    int flen = strlen((char*)fname);
    //when at 32, it thinks length is actually 35
    //so use 35 instead of MAX_FILENAME_LEN
    if(flen < 0 || flen > 35 || fname == NULL || dentry == NULL){
        printf("read dentry by name fail");
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
        printf("read dentry by index fail");
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
        length of data to read
        AND length of buf!
 *   OUTPUTS: 0 if success, -1 if fail
 *   RETURN VALUE: number of bytes read
 *   SIDE EFFECTS: data is put into buf
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    inode_t* itnode;    //actual inode of the file
    data_block_t* curblock;  //current data block being read
    int i, j, x;           //i is index for blocks, j is index for chars of block
    int bytesread = 0;
    int bloff;          //starting block index after offset
    int dataoff;
    int totalblocks = 1;
    int b = 0;

    //check inputs valid
    if(inode < 0 || inode >= bbl->inode_count || buf == NULL){
        printf("read data fail\n");
        return -1;
    }
  
    //get inode of the file
    itnode = (inode_t*)(startinode+inode);

    //put into buf from 0 to length
    // ^ this is CHAR length
    //num of blocks to read would be length/4096
    // ^ use this for getting each block from inode

    //offset is in bytes (int8), need to figure out how many blocks that is
    // ^ start at different block depending on offset
    bloff = (offset/BLOCK_SIZE);
    //   offset might not be perfect, may need to start middle of 1st block
    dataoff = (offset%BLOCK_SIZE);

    //get total blocks to read
    //if 0, then length less than 1 block, so only read 1 block
    totalblocks += length/BLOCK_SIZE;

    printf("input len: %d ", length);
    printf("file len: %d\n", itnode->length);

    //get each block to read
    // length is in CHARS
    // each index in data is 1 (ONE) CHAR
    for(i=0; i < totalblocks; i++){
        //get the data block
        curblock = (data_block_t*)(startblock + (itnode->data_block_num[i+bloff]));
        printf("got block\n");
        //put each char of curblock into the buf
        for(j=0; j < BLOCK_SIZE; j++){
            //get page fault because file may be smaller than 1 block
            if(j+dataoff >= length){
                break;
            }
            //in case if length > file size, exit when file done
            if(bytesread >= itnode->length){
                break;
            }
            if((i == 0) && ((j+dataoff) >= BLOCK_SIZE)){
                break;
            }
            if(bytesread >= 5000){
                x = 1;
            }
            //if at the starting block, need to account for offset
            if((i == 0) && ((j+dataoff) < BLOCK_SIZE)){
                buf[b] = curblock->actualdata[j+dataoff];
                //putc(buf[b]);
                //buf[j] = curblock->actualdata[j+dataoff];
            }
            //otherwise get data as normal
            else{
                buf[b] = curblock->actualdata[j];
                //buf[(i*BLOCK_SIZE) + j] = curblock->actualdata[j];
            }
            //keep track how many bytes read
            bytesread++;
            b++;
        }
    }

    return bytesread;
}

// ~~~~~ FILE FUNCTIONS ~~~~~
//open always successful
int32_t open_file(const uint8_t* filename, int fd){
    //must have a counter for each file
    //count how many bytes have been read
    //when read again, start where left off, not at beginning
    // counter is in fdarray, fd itself is the index

    //open just sets up the file
    
    //get file dentry and put into array
    dentry_t* fentry = &(bbl->entries[0]);
    read_dentry_by_name(filename, fentry);

    filearray[fd] = fentry;
    bytecount[fd] = 0;

    return 0;
}

//close always successful
int32_t close_file(int32_t fd){
    //undo what open did
    filearray[fd] = NULL;
    bytecount[fd] = -1;

    return 0;
}

int32_t read_file(int32_t fd, void* buf, int32_t nbytes){
    dentry_t* fentry = filearray[fd];
    int32_t actualbytes;

    //check invalid
    if(fentry == NULL || buf == NULL){
        printf("read file fail");
        return -1;
    }
//if statement triggered but still got to here?
    //get the inode (using fd)
    actualbytes = read_data(fentry->inode_num, bytecount[fd], buf, nbytes);

    bytecount[fd] += nbytes;

    return actualbytes;
}

int32_t write_file(int32_t fd, const void* buf, int32_t nbytes){
    //writes will always fail because read-only system
    return -1;
}


// ~~~~~ DIRECTORY FUNCTIONS ~~~~~
//there is only 1 directory so don't really care about open and close
//open always successful
int32_t open_dir(const uint8_t* filename){
    return 0;
}

//close always successful
int32_t close_dir(int32_t fd){
    return 0;
}

//put all the names of files inside the directory into the buffer
int32_t read_dir(int32_t fd, void* buf, int32_t nbytes){
    int i;
    dentry_t curentry;
    inode_t* curnode;
    uint32_t lencounter = 0; //keep track of length of all the files
    //will use this to put file names into the buffer at offsets
    // (so that file names won't overwrite each other)

    for(i=0; i < bbl->dentry_count; i++){
        //get the inode of cur entry
        curentry = bbl->entries[i];
        curnode = (inode_t*)(startinode + curentry.inode_num);
        //write to the buf
        read_data(curentry.inode_num, lencounter, buf, curnode->length);

        //get total length bytes used so far, so know what offset for buf
        lencounter += curnode->length;

        //add a newline after each file
        //buf[lencounter] = '\n';

        //lencounter += 1;
    }
    
    //remove extra new lines
    lencounter = lencounter - bbl->dentry_count;

    return lencounter;
}

int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes){
    //writes will always fail because read-only system
    return -1;
}
