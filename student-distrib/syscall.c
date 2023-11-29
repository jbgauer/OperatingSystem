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
    
    cli();
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
    terminal[pcb_array[curr_pid].term_id].t_pid = parent_id;
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

    page_table_vmem[0] = 0x00000006;    // unmapping vid map page
    
    flush_tlb();
    
    tss.esp0 = (uint32_t)pcb_array[curr_pid].stack_ptr;
    tss.ss0  = KERNEL_DS;
    
    /*Jump to execute return*/ 
    //also restores parent esp and ebp
    putc('\n'); // newline after every executable

    sti();
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
    cli();
    int terminal_s = curr_terminal;
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

    // asm volatile("movl %%ebp, %0;":"=r"(pcb_array[curr_pid].base_ptr));

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
        cmdHold++;
    }
    //set end of filename
    *cmdHold = '\0';
    cmdHold++;
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
    // Changes terminal controlled pid
    terminal[terminal_s].t_pid = curr_pid;
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
    pcb_init(par_pid, terminal_s);
    programs_running += 1;
    strcpy((char *)pcb_array[curr_pid].arg_array, (char*)cmdArgs);
     
    /*context switch (IN x86)*/
    // create its own context switch stack 
    uint8_t new_buf[4];
    read_data(dentry.inode_num, 24, new_buf, 4);   // only reads one byte for some reason

    /*tss*/
    //change esp0 to the value the stack pointer 
    tss.esp0 = (uint32_t)pcb_array[curr_pid].stack_ptr;
    //change ss0
    tss.ss0 = KERNEL_DS; 

    sti();

    // IRET 
    iret_context(*((uint32_t*)new_buf));

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

    bread = curpcb->fda[fd].file_op_ptr->read(fd, buf, nbytes);

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
        printf("write fail, invalid inputs");
        return -1;
    }
    if(curpcb->fda[fd].flags == 0){
        printf("write fail, file not open");
        return -1;
    }

    bitten = curpcb->fda[fd].file_op_ptr->write(fd, buf, nbytes);

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
    pcb_t* curpcb;

    //open_file will do most of the work because need dentry info
    int fdindex = open_file(filename);

    if(fdindex == -1){
        return -1;
    }

    //open the file
    curpcb = &pcb_array[curr_pid];
    type = curpcb->fda[fdindex].file_type;
    //set file as open (just in case)
    curpcb->fda[fdindex].flags = 1;

    //need to set the file operations depending on file type
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
            break;

        case 2: //type 2 is normal files
            curpcb->fda[fdindex].file_op_ptr = &fop_file;
            break;

        default:
            printf("open fail, invalid file type");
            return -1;
    }

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
    curpcb = &pcb_array[curr_pid];

    //check input valid
    //should not be able to close stdin or stdout
    if(fd < 2 || fd > 7){
        printf("close fail, invalid fd");
        return -1;
    }
    //check that a file was open before trying to close
    if(curpcb->fda[fd].flags == 0){
        printf("close fail, file not open");
        return -1;
    }

    //use the close operation from the file ops
    curpcb->fda[fd].file_op_ptr->close(fd);
    //set file as closed
    curpcb->fda[fd].flags = 0;

    return 0;
}

/*
 * getargs
 *   DESCRIPTION: Copies arguments of current process into given user buffer.
 *   INPUTS: user buffer (buf), number of bytes to copy to (nbytes)
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if success, -1 if fail
 *   SIDE EFFECTS: copies arguments from exec into user buf.
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
 *   DESCRIPTION: Sets user level page to video memory, gives to program
 *   INPUTS: uint8_t** - Pointer to pointer to virtual address of user level video memory page
 *   OUTPUTS: uint8_t** - Sets this pointer to point to the new user level page
 *   RETURN VALUE: 0 if success, -1 if bad input
 *   SIDE EFFECTS: Creates new user level page
 */
int32_t vidmap (uint8_t** screen_start) {
    
    // uint32_t i;
    //Checks for bad input
    if(screen_start == NULL || screen_start < (uint8_t**)(EIGHT_MB)) {
        return -1;
    }
    
    //Creates new page for user level access to video memory
    uint32_t vidf;
    ptable_entry_t vidmem;

    vidmem.m_addr = 0xB8;
    if(curr_terminal != curr_thread) {
        vidmem.m_addr += curr_thread+1;
    }
    vidmem.g = 0;
    vidmem.pat = 0;
    vidmem.d = 0;
    vidmem.a = 0;
    vidmem.pcd = 0;
    vidmem.pwt = 0;
    vidmem.us = 1;
    vidmem.rw = 1;
    vidmem.p = 1;
    vidf = combine_table_entry(vidmem);

    //Sets page table entry 0 to video memory
    page_table_vmem[curr_thread] = vidf;

    //Sets page directory entry with user level set to 1, present set to 1, and r/w set to 1
    pagedir[VIRT_VID_MEM_DIR] = ((unsigned int)page_table_vmem) | 7;

    flush_tlb();
    //Sets screen_start pointer to virtual memory of new page
    *screen_start = (uint8_t*)(VIRT_VID_MEM+0x1000*(curr_thread)); //set at 1GB+1kB*thread_number in virtual memory

    return 0;
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


