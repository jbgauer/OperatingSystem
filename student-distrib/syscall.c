#include "syscall.h"

/*
 * halt
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t halt (uint8_t status) {
    return -1;
}


/*
 * system_execute
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t
execute(const uint8_t *command) {
        
    // uint8_t *cmdHold;
    // uint8_t *cmdArgs;
    // uint8_t *file;
    // uint32_t i, j, currPCB, newEntry;
    // uint8_t *buf;
    // dentry_t *dentry;
    // pdir_entry_t kerntry;
    // uint32_t basePointer;

    // //parent base pointer
    // basePointer = retrieveBasePointer();

    // if(command == NULL) {
    //     return -1;
    // }
    // /*Parse Args*/
    // cmdHold = command;
    // //skip spaces
    // for(int i = 0; i < COMMAND_MAX; i++) {
    //     if(cmdHold == ' ') {
    //         cmdHold++;
    //     } else { 
    //         break;
    //     }
    // } 
    // file = cmdHold;
    // //go to end of filename
    // while(cmdHold != ' ' || cmdHold != '\n' || cmdHold != '\0') {
    //     cmdHold++;
    // }
    // //set end of filename
    // *cmdHold = '\0';
    // //skip spaces (NOTE: maybe this goes to far? Maybe it will have infinite spaces? [when there are no args])
    // for(int i = 0; i < COMMAND_MAX; i++) {
    //     if(cmdHold == ' ') {
    //         cmdHold++;
    //     } else { 
    //         break;
    //     }
    // } 

    // cmdArgs = cmdHold;
    // //go to end of args
    // while(cmdHold != ' ' || cmdHold != '\n' || cmdHold != '\0') {
    //     cmdHold++;
    // }
    // //set end of args
    // *cmdHold = '\0';

    // // args will be passed into get args to the program

    // /*Executable Check*/
    // //set dentry
    // if(read_dentry_by_name(file, dentry) == -1)
    //     return -1;
    // if(read_data(dentry->inode_num,0,buf,4) == -1) 
    //     return -1;
    // if(buf != EXEC_VAL) 
    //     return -1;
    // if(dentry->filetype != 2)
    //     return -1;

    // /*set up program paging*/
    // //Find first open process (0-5)
    // for(i = 0; i < PROG_MAX; i++) {
    //     if(PCB[i].used == 0) {
    //         currPCB = i;
    //         break;
    //     }
    // }
    // //FAILED TO RUN (could be another error return (-2))
    // if(i == PROG_MAX)
    //     return -1;

    // //pages in use
    // currPCB += PAGES_DEFAULT_USE;
    // //Set new page (first addr at 0x400)
    // kerntry.p_addr = currPCB << KENTRY_SHIFT;
    // kerntry.ps = 1;
    // kerntry.a = 0;
    // kerntry.pcd = 0;
    // kerntry.pwt = 0;
    // kerntry.us = 1;
    // kerntry.rw = 1;
    // kerntry.p = 1;
    // newEntry = combine_kir_entry(kerntry);
    // pagedir[USER_SPACE] = newEntry;
    // //flush tlb
    // flush_tlb();

    // /*load file into memory*/ /*aka User-level program loader*/
    // //once have the page read_data
    
    // // load from FS to program page (how to get process number)

    

    // /*create pcb*/


    // /*fda*/

    // //stdin + stdout vals STILL NEED TO BE CHANGED!
    // //set filearray[0] == stdin
    // PCB[i]->filearray[0].file_op_ptr = NULL;
    // PCB[i]->filearray[0].inode = 0;
    // PCB[i]->filearray[0].file_position = 0;
    // PCB[i]->filearray[0].flags = 0;
    // PCB[i]->filearray[j].filename = "";     //not sure if we want this
    // PCB[i]->filearray[j].file_type = 0;     //not sure if we want this
    // //set filearray[1] == stdout 
    // PCB[i]->filearray[1].file_op_ptr = NULL;
    // PCB[i]->filearray[1].inode = 0;
    // PCB[i]->filearray[1].file_position = 0;
    // PCB[i]->filearray[1].flags = 0;
    // PCB[i]->filearray[j].filename = "";     //not sure if we want this
    // PCB[i]->filearray[j].file_type = 0;     //not sure if we want this

    // //set rest of array to 0
    // for(j = 2; j < FILE_MAX; j++) {
    //     PCB[i]->filearray[j].file_op_ptr = NULL;
    //     PCB[i]->filearray[j].inode = 0;
    //     PCB[i]->filearray[j].file_position = 0;
    //     PCB[i]->filearray[j].flags = 0;
    //     PCB[i]->filearray[j].filename = "";     //not sure if we want this
    //     PCB[i]->filearray[j].file_type = 0;     //not sure if we want this
    // }
    
    // /*tss*/
    // //change esp0 to the value the stack pointer 
    // tss.esp0 = PCB[i].stack_ptr;
    // //change ss0
    // tss.ss0 = KERNEL_DS; 

    
    /*context switch (IN x86)*/
        //create its own context switch stack
        //IRET

    return -1;
}



/*
 * read
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t read (int32_t fd, void* buf, int32_t nbytes) {
    return -1;
}

/*
 * write
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t write (int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/*
 * open
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t open (const uint8_t* filename) {
    return -1;
}

/*
 * close
 *   DESCRIPTION: 
 *   INPUTS:
 *   OUTPUTS: none
 *   RETURN VALUE: 
 *   SIDE EFFECTS: none
 */
int32_t close (int32_t fd) {
    return -1;
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
    return -1;
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



