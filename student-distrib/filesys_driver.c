#include "filesys_driver.h"
#include "lib.h"
#include "syscall.h"
#include "pcb.h"

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
    int i, curlen;

    //check inputs are valid
    int flen = strlen((char*)fname);
    //when at 32, it thinks length is actually 35
    //so use 35 instead of MAX_FILENAME_LEN
    if(flen < 0 || flen > 35 || fname == NULL || dentry == NULL){
        printf("read dentry by name fail, invalid inputs");
        return -1;
    }

    //go thru dir entries
    for(i = 0; i < bbl->dentry_count; i++){
        //get cur name
        curname = (bbl->entries[i]).filename;
        //get cur length, files might have parts match
        // (frame == frame0.txt)
        curlen = strlen((char*)curname);
        if(curlen > MAX_FILENAME_LEN){
            curlen = MAX_FILENAME_LEN;
        }

        //if fname = curname
        if(strncmp((int8_t*)fname, (int8_t*)curname, MAX_FILENAME_LEN) == 0){
            if(flen == curlen){
                //use the read by index function once found file
                read_dentry_by_index(i, dentry);
                return 0;
            }
        }
    }

    //fail if file doesnt exist
    printf("read dentry by name fail, file not found");
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
    int i, j;           //i is index for blocks, j is index for chars of block
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

    //printf("input len: %d ", length);
    //printf("file len: %d\n", itnode->length);

    //get each block to read
    // length is in CHARS
    // each index in data is 1 (ONE) CHAR
    for(i=0; i < totalblocks; i++){
        //get the data block
        curblock = (data_block_t*)(startblock + (itnode->data_block_num[i+bloff]));
        //printf("got block %d\n", i);
        //put each char of curblock into the buf
        for(j=0; j < BLOCK_SIZE; j++){
            //get page fault because file may be smaller than 1 block
            if(j+dataoff >= length){
                return bytesread;
            }
            //stop reading when reached length (may be shorter than a block)
            if(bytesread >= length){
                return bytesread;
            }
            //in case if length > file size, exit when file done
            if(bytesread >= itnode->length){
                return bytesread;
            }
            if((i == 0) && ((j+dataoff) >= BLOCK_SIZE)){
                return bytesread;
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
/*
 * open_file
 *   DESCRIPTION: set up file to be read
 *   INPUTS: filename, fd (index in file array)
 *   OUTPUTS: int to tell success or fail
 *   RETURN VALUE: always 0
 *   SIDE EFFECTS: puts file into file array
 */
//open always successful
//open just sets up the file
int32_t open_file(const uint8_t* filename){
    // int j;
    // //get file dentry
    // dentry_t* fentry = &(bbl->entries[0]);
    // read_dentry_by_name(filename, fentry);

    // //first need to get pcb (process that called me)
    // pcb_t* curpcb = pcb_array[curpid]; //get curpid from syscall file

    // //get the first available index
    // for(j=2; j < 8; j++){
    //     if(curpcb->fda[j].flags == 0){
    //         break;
    //     }
    // }
    // //put the file into the file array at this index
    // curpcb->fda[j].inode = fentry->inode_num;
    // curpcb->fda[j].file_position = 0;
    // curpcb->fda[j].flags = 1;   //flags = 0 when open, flags = 1 when file here
    // curpcb->fda[j].filename = fentry->filename;
    // curpcb->fda[j].file_type = fentry->file_type;

    return 0;
}

/*
 * close_file
 *   DESCRIPTION: undo what open file did
 *   INPUTS: fd
 *   OUTPUTS: int
 *   RETURN VALUE: always 0
 *   SIDE EFFECTS: get rid of file in file array
 */
//close always successful
int32_t close_file(int32_t fd){
    // //undo what open did
    // //get current pcb
    // pcb_t* curpcb = pcb_array[curpid];

    // //remove the file from curpcb's array
    // curpcb->fda[fd].inode = 0;
    // curpcb->fda[fd].file_position = 0;
    // curpcb->fda[fd].flags = 0;
    // curpcb->fda[fd].filename = "";
    // curpcb->fda[fd].file_type = 0;

    return 0;
}

/*
 * read_file
 *   DESCRIPTION: read data in a given file
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS: int
 *   RETURN VALUE: num bytes read, -1 if fail
 *   SIDE EFFECTS: puts data into buf
 */
int32_t read_file(int32_t fd, void* buf, int32_t nbytes){
    // //keep track of bytes read
    // int32_t actualbytes;

    // //get the current pcb
    // pcb_t* curpcb = pcb_array[curpid];

    // //get the dentry of the pcb's file
    // dentry_t* fentry = &(bbl->entries[0]); //initialize fentry to bbl, cause otherwise page fault
    // read_dentry_by_name(curpcb->fda[fd].inode, fentry);

    // //check invalid
    // if(fentry == NULL || buf == NULL){
    //     printf("read file fail");
    //     return -1;
    // }

    // //read the data
    // //account for offset (file_position)
    // actualbytes = read_data(fentry->inode_num, curpcb->fda[fd].file_position, buf, nbytes);

    // //add the bytes read to the file position
    // curpcb->fda[fd].file_position += actualbytes;

    // return actualbytes;
}

/*
 * write_file
 *   DESCRIPTION: does nothing
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS: always fail
 *   RETURN VALUE: -1
 *   SIDE EFFECTS: none
 */
int32_t write_file(int32_t fd, const void* buf, int32_t nbytes){
    //writes will always fail because read-only system
    return -1;
}


// ~~~~~ DIRECTORY FUNCTIONS ~~~~~
/*
 * open_dir
 *   DESCRIPTION: does nothing
 *   INPUTS: filename
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
//there is only 1 directory so don't really care about open and close
//open always successful
int32_t open_dir(const uint8_t* filename){
    return 0;
}

/*
 * close_dir
 *   DESCRIPTION: does nothing
 *   INPUTS: fd
 *   OUTPUTS: none
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
//close always successful
int32_t close_dir(int32_t fd){
    return 0;
}

/*
 * read_dir
 *   DESCRIPTION: reads all file names in dir
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS: 0 or -1
 *   RETURN VALUE: int of success or fail
 *   SIDE EFFECTS: puts file names into buffer
 */
//put all the names of files inside the directory into the buffer
int32_t read_dir(int32_t fd, uint8_t* buf, int32_t nbytes){
    int i, j;
    //dentry_t curentry;
    //inode_t* curnode;
    uint32_t lencounter = 0; //keep track of length of all the file names
    //will use this to put file names into the buffer at offsets
    // (so that file names won't overwrite each other)
    uint8_t* curname;
    //int8_t* filename = "file_name:"; //10 long

    for(i=0; i < bbl->dentry_count; i++){
        //get the name of current dentry
        curname = bbl->entries[i].filename;

        //put file_name:
        //strcpy(buf[lencounter], filename);
        //lencounter += 10;

        //33-strlen
        for(j=0; j < strlen((char*)curname); j++){
            //dont go over 32 char limit
            if(j >= 32){
                break;
            }
            buf[lencounter] = curname[j];
            lencounter++;
        }

        // buf[lencounter] = ',';
        // lencounter++;
        // buf[lencounter] = ' ';
        // lencounter++;
        // buf[lencounter] = (char*)(bbl->entries[i].filetype);
        // lencounter++;
        buf[lencounter] = '\n';
        lencounter++;
    }

    return lencounter;
}

/*
 * write_dir
 *   DESCRIPTION: does nothing
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS: none
 *   RETURN VALUE: always -1
 *   SIDE EFFECTS: none
 */
int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes){
    //writes will always fail because read-only system
    return -1;
}
