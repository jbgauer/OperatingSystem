#include "syscall.h"

uint32_t programs_running = 0;

// file operations pointers

file_op_t fop_rtc = {.read = rtc_read, .write = rtc_write, .open = rtc_open, .close = rtc_close};
file_op_t fop_dir = {.read = read_dir, .write = write_dir, .open = open_dir, .close = close_dir};
file_op_t fop_file = {.read = read_file, .write = write_file, .open = open_file, .close = close_file};

/*
 * halt
 *   DESCRIPTION: Ends a process by clearing the fda, 
 *   checking if there is a parent process, if so 
 *   switch the pages and set the tss. Then return to execute.
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t halt (uint8_t status) {
    uint32_t parent_id;
    int i;
    uint8_t shellcmd[6] = "shell\0";
    uint32_t newEntry, pageHold; //basePointer;
    // uint8_t *buf;
    // dentry_t *dentry;
    pdir_entry_t kerntry;

    //uint8_t shellcmd[5] = "shell";
    
    if(programs_running > 0){
        pcb_array[curr_pid].in_use = 0;
        programs_running -= 1;
    }
    /*close any relevant FDs*/
    for(i = 0; i < FILE_MAX; i++) {
        close_file(i);
    }

    if(programs_running == 0){
        execute(shellcmd);
    }


    /*restore parent data*/
    parent_id = pcb_array[curr_pid].par_id;
    curr_pid = parent_id;

    /*base pointer?*/
    /*restore parent paging*/
    //Set new page (first addr at 0x400)
    pageHold = curr_pid;
    pageHold += PAGES_DEFAULT_USE;
    kerntry.p_addr = pageHold << KENTRY_SHIFT;
    kerntry.ps = 1;
    kerntry.a = 0;
    kerntry.pcd = 0;
    kerntry.pwt = 0;
    kerntry.us = 1;
    kerntry.rw = 1;
    kerntry.p = 1;
    newEntry = combine_dir_entry(kerntry);
    pagedir[USER_SPACE] = newEntry;
    
    flush_tlb();
    
    tss.esp0 = (uint32_t)pcb_array[curr_pid].stack_ptr;
    tss.ss0  = KERNEL_DS;
    
    /*Jump to execute return*/ 
    //also restores parent esp and ebp

    asm volatile(
                 "movl %0, %%esp;" 
                 "movl %1, %%ebp;" 
                 "movl %2, %%eax;"
                 "jmp  execute_return"
                 :
                 :"r"(pcb_array[curr_pid].stack_ptr), "r"(pcb_array[curr_pid].base_ptr), "r"((uint32_t)status)
                 :"eax"
                 );

    return -1;
}


/*
 * system_execute
 *   DESCRIPTION: Executes process, creates pcb and sets up paging for user space
 *   INPUTS: uint8_t - command 
 *   OUTPUTS: none
 *   RETURN VALUE: int32_t
 *   SIDE EFFECTS: none
 */
int32_t
execute(const uint8_t *command) {
    //printf("entering execute\n");
    uint8_t hold1[COMMAND_MAX];
    uint8_t *cmdHold;
    uint8_t *cmdArgs;
    uint8_t *file;
    uint32_t par_pid; 
    uint32_t i, newEntry, pageHold;
    uint8_t buf[4];
    //buf is an array, must initalize with a size
    dentry_t dentry; //= &(bbl->entries[0]);
    //dentry will cause page fault if null, always initialize it to something
    pdir_entry_t kerntry;

    asm volatile("movl %%ebp, %0;":"=r"(pcb_array[curr_pid].base_ptr));

    if(command == NULL) {
        return -1;
    }
    cmdHold = hold1;
    /*Parse Args*/
    strncpy((char*)cmdHold, (char*)command, COMMAND_MAX);
    //skip spaces
    for(i = 0; i < COMMAND_MAX; i++) {
        if(*cmdHold == ' ') {
            cmdHold++;
        } else { 
            break;
        }
    } 
    file = cmdHold;
    //go to end of filename
    while(*cmdHold != ' ' && *cmdHold != '\n' && *cmdHold != '\0') {
        //printf("%c", *cmdHold);
        cmdHold++;
    }
    //printf("\ngot to end of file\n");
 
    //set end of filename
    *cmdHold = '\0';
    
    //skip spaces (NOTE: maybe this goes to far? Maybe it will have infinite spaces? [when there are no args])
    for(i = 0; i < COMMAND_MAX; i++) {
        if(*cmdHold == ' ') {
            cmdHold++;
        } else { 
            break;
        }
    } 

    cmdArgs = cmdHold;
    //go to end of args
    while(*cmdHold != ' ' && *cmdHold != '\n' && *cmdHold != '\0') {
        cmdHold++;
    }
    //set end of args
    *cmdHold = '\0';

    // args will be passed into get args to the program
    //printf("file: ");
    //puts(file);
    //printf("\n");
    /*Executable Check*/
    //set dentry
    if(read_dentry_by_name(file, &dentry) == -1)
        return -1;
    if(read_data(dentry.inode_num,0,buf,4) == -1) 
        return -1;
    if(buf[0] != 0x7f || buf[1] != 0x45 || buf[2] != 0x4C || buf[3] != 0x46)
        return -1;
    if(dentry.filetype != 2)
        return -1;

    /*set up program paging*/
    par_pid = curr_pid;
    //Find first open process (0-5)
    //temp change to 3 prog_max
    for(i = 0; i < PROG_MAX; i++) {
        if(pcb_array[i].in_use == 0) {
            curr_pid = i;
            break;
        }
    }
    //FAILED TO RUN (could be another error return (-2))
    if(i == PROG_MAX)
        return -1;

    //pages in use
    pageHold = curr_pid;
    pageHold += PAGES_DEFAULT_USE;
    //Set new page (first addr at 0x400)
    kerntry.p_addr = pageHold << KENTRY_SHIFT;
    kerntry.ps = 1;
    kerntry.a = 0;
    kerntry.pcd = 0;
    kerntry.pwt = 0;
    kerntry.us = 1;
    kerntry.rw = 1;
    kerntry.p = 1;
    newEntry = combine_dir_entry(kerntry);
    pagedir[USER_SPACE] = newEntry;
    //flush tlb
    flush_tlb();

    /*load file into memory*/ /*aka User-level program loader*/
    //once have the page read_data
    
    // load from FS to program page (how to get process number)

    inode_t* file_inode = (inode_t*)(startinode + dentry.inode_num);

    read_data(dentry.inode_num, 0, (uint8_t*)PRGRM_IMG_START, file_inode->length);

    /*Create PCB*/
    if(curr_pid == 0) {
        par_pid = 0;
    }
    pcb_init(par_pid);
    programs_running += 1;
    strcpy((char *)pcb_array[curr_pid].arg_array, (char*)cmdArgs);
    
    // if(file == "shell\0") {
    //     pcb_array[curr_pid].shell_flag = 1;
    // } else {
    //     pcb_array[curr_pid].shell_flag = 0;
    // }


    
    
    /*context switch (IN x86)*/
    // create its own context switch stack 
    // read_data(dentry->inode_num, 24, buf, 4); // buf holds entry point in program

    uint8_t new_buf[4];
    read_data(dentry.inode_num, 24, new_buf, 4);   // only reads one byte for some reason

    /*tss*/
    //change esp0 to the value the stack pointer 
    tss.esp0 = (uint32_t)pcb_array[curr_pid].stack_ptr;
    //change ss0
    tss.ss0 = KERNEL_DS; 



    // this read data cannot go to the offset of 24, read data only works for first 4 bytes 

    // IRET 
    iret_context(*((uint32_t*)new_buf));
    // ^ gets lost in this function
    //after iret, program can't return to here
    //"cannot access memory at address 0x8400000"
//BOOT LOOPING BUG ^
//page fault now ??

    asm volatile("execute_return:");

    return 0;
}



/*
 * read
 *   DESCRIPTION: reads nbytes of the file and puts into buf
 *   INPUTS: index in fda, buf, nbytes
 *   OUTPUTS: bytes read, or -1 if fail
 *   RETURN VALUE: int
 *   SIDE EFFECTS: fills in buf 
 */
int32_t read (int32_t fd, void* buf, int32_t nbytes) {
    
    pcb_t* curpcb;
    //file_op_t fops;
    int32_t bread;  //bytes read from read function
    curpcb = &pcb_array[curr_pid];

    //check input valid
    if(fd < 0 || fd > 7 || buf == NULL){
        printf("read fail, invalid inputs");
        return -1;
    }

    if(curpcb->fda[fd].flags == 0){
        printf("read fail, file not open");
        return -1;
    }

//read broken, change 0=fd
    //fops = *(curpcb->fda[fd].file_op_ptr); //fops doesnt work, op ptrs wrong
    bread = curpcb->fda[fd].file_op_ptr->read(fd, buf, nbytes);
//when ls tries to do read dir it page faults
// because read is empty, why is file_op_ptr empty??

    //bread = (*(fops.read))(fd, buf, nbytes);
//printf("bread");
    return bread;
}

/*
 * write
 *   DESCRIPTION: does nothing, this is read-only file system
 *   INPUTS: fd, buf, nbytes
 *   OUTPUTS: num of bytes written, -1 if fail
 *   RETURN VALUE: int
 *   SIDE EFFECTS: might copy the input buffer
 */
int32_t write (int32_t fd, const void* buf, int32_t nbytes) {
    // printf("%s",buf);
    
    pcb_t* curpcb;
    //file_op_t fops;
    int32_t bitten;  //bytes written
    curpcb = &pcb_array[curr_pid];

    //check input valid
    if(fd < 0 || fd > 7 || buf == NULL){
        printf("read fail, invalid inputs");
        return -1;
    }
//printf("write");
    bitten = curpcb->fda[fd].file_op_ptr->write(fd, buf, nbytes);
//printf("kitten");
    return bitten;
}

/*
 * open
 *   DESCRIPTION: puts the file into the pcb's file array
 *   INPUTS: filename
 *   OUTPUTS: index in the file array, or -1 if fail
 *   RETURN VALUE: int
 *   SIDE EFFECTS: changes the fda of curpcb
 */
int32_t open (const uint8_t* filename) {
    uint32_t type;
    file_op_t fops;
    pcb_t* curpcb;

    //open_file will do most of the work because need dentry info
    int fdindex = open_file(filename);

    if(fdindex == -1){
        return -1;
    }

    //need to set the file operations depending on file type
    curpcb = &pcb_array[curr_pid];
    type = curpcb->fda[fdindex].file_type;


    switch(type){
        case 0: //if type 0, then rtc
            curpcb->fda[fdindex].file_op_ptr = &fop_rtc;

            //also need to do the rtc open
            rtc_open(filename);
            break;

        case 1: //if type 1, then boot block (dir)
            curpcb->fda[fdindex].file_op_ptr = &fop_dir;

            //need to do the directory open (does nothing)
            open_dir(filename);
//gdb here to see why file op ptr was empty
            break;

        case 2: //type 2 is normal files
            curpcb->fda[fdindex].file_op_ptr = &fop_file;
            break;

        default:
            printf("open fail, invalid file type");
            return -1;
    }

    curpcb->fda[fdindex].file_op_ptr = &fops;

    return fdindex;
}

/*
 * close
 *   DESCRIPTION: removes the file from the pcb's file array
 *   INPUTS: fd index of file
 *   OUTPUTS: 0 if success, -1 if fail
 *   RETURN VALUE: int
 *   SIDE EFFECTS: changes an entry in the file array
 */
int32_t close (int32_t fd) {
    pcb_t* curpcb;
    //file_op_t fops;
    
    //check input valid
    if(fd < 0 || fd > 7){
        printf("close fail, invalid fd");
        return -1;
    }

    //use the close operation from the file ops
    curpcb = &pcb_array[curr_pid]; //gets an unitialized pcb

    //fops does NOT work
    //fops = *(curpcb->fda[fd].file_op_ptr);
    //(*(fops.close))(fd);
//curpcb uninitialized here
    curpcb->fda[fd].file_op_ptr->close(fd);

    return 0;
}

/*
 * getargs
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t getargs (uint8_t* buf, int32_t nbytes) {
    /*If invalid arguments*/
    if(nbytes <= 0 || buf == NULL || pcb_array[curr_pid].arg_array[0] == '\0') {
        return -1;
    }
    /*Array is longer than length*/
    if(strlen((char*)pcb_array[curr_pid].arg_array) > nbytes) {
        return -1;
    }
    //Copies arguments to buf
    strcpy((char *)buf, (char*)pcb_array[curr_pid].arg_array);
    /*Success*/
    return 0;
}

/*
 * vidmap
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t vidmap (uint8_t** screen_start) {
    return -1;
}

/*
 * set_handler
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t set_handler(int32_t signum, void* handler_address) {
    return -1;
}

/*
 * sigreturn
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t sigreturn (void) {
    return -1;
}
