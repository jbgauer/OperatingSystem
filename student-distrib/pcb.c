/* pcb.c - Struct and helper functions for process control block
 * 
 */

#include "pcb.h"
#include "paging.h"
#include "terminal.h"
#include "syscall.h"


/*
 * pcb_init
 *   DESCRIPTION: initializes a new process control block
 *   INPUTS: uint32_t - process number of parent
 *   OUTPUTS: changed entry in pcb array
 *   RETURN VALUE: none
 *   SIDE EFFECTS: loads pcb into array
 */
void pcb_init(uint32_t par_process_num) {
    file_op_t std_file_op;
    int curr_proc = curr_pid;
    int j;
    pcb_t* new_pcb = &pcb_array[curr_proc];
    new_pcb->in_use = 1;
    new_pcb->par_id = par_process_num;
    // new_pcb->inst_ptr = 0;

    //set fda[0] == stdin
    std_file_op.open = &term_open;
    std_file_op.read = &term_read;
    std_file_op.write = &term_write;
    std_file_op.close = &term_close;
    new_pcb->fda[0].file_op_ptr = &std_file_op;
    new_pcb->fda[0].inode = 0;
    new_pcb->fda[0].file_position = 0;
    new_pcb->fda[0].flags = 0;
    //*(new_pcb->fda[0].filename) = "";     //not sure if we want this
    new_pcb->fda[0].file_type = 0;     //not sure if we want this
    //set fda[1] == stdout 
    new_pcb->fda[1].file_op_ptr = &std_file_op;
    new_pcb->fda[1].inode = 0;
    new_pcb->fda[1].file_position = 0;
    new_pcb->fda[1].flags = 0;
    //*(new_pcb->fda[1].filename) = "";     //not sure if we want this
    new_pcb->fda[1].file_type = 0;     //not sure if we want this

    //set rest of array to 0
    for(j = 2; j < FILE_MAX; j++) {
        new_pcb->fda[j].file_op_ptr = NULL;
        new_pcb->fda[j].inode = 0;
        new_pcb->fda[j].file_position = 0;
        new_pcb->fda[j].flags = 0;
        //*(new_pcb->fda[j].filename) = "";   //not sure if we want this
        new_pcb->fda[j].file_type = 0;     //not sure if we want this
    }
}
