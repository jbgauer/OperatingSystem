/* pcb.c - Struct and helper functions for process control block
 * 
 */

#include "pcb.h"
#include "paging.h"
#include "terminal.h"
#include "syscall.h"

file_op_t fop_stdin = {.read = term_read, .write = bad_write, .open = term_open, .close = term_close};
file_op_t fop_stdout = {.read = bad_read, .write = term_write, .open = term_open, .close = term_close};

/*
 * pcb_init
 *   DESCRIPTION: initializes a new process control block
 *   INPUTS: uint32_t - process number of parent
 *   OUTPUTS: changed entry in pcb array
 *   RETURN VALUE: none
 *   SIDE EFFECTS: loads pcb into array
 */
file_op_t std_file_op;
void pcb_init(uint32_t par_process_num, uint32_t terminal_id) {
    int curr_proc = terminal[curr_thread].t_pid;
    int j;
    pcb_t* new_pcb = &pcb_array[curr_proc];
    new_pcb->in_use = 1;
    new_pcb->par_id = par_process_num;
    new_pcb->stack_ptr = EIGHT_MB - (EIGHT_KB*curr_proc) - 4;
    new_pcb->base_ptr = EIGHT_MB - (EIGHT_KB*curr_proc) - 4;
    // new_pcb->sch_base_ptr = EIGHT_MB - (EIGHT_KB*curr_proc) - 4;
    // new_pcb->sch_stack_ptr = EIGHT_MB - (EIGHT_KB*curr_proc) - 4;
    new_pcb->term_id = terminal_id;

    //set fda[0] == stdin
    new_pcb->fda[0].file_op_ptr = &fop_stdin;
    new_pcb->fda[0].inode = 0;
    new_pcb->fda[0].file_position = 0;
    new_pcb->fda[0].flags = 1;
    new_pcb->fda[0].file_type = 3;     
    //set fda[1] == stdout 
    new_pcb->fda[1].file_op_ptr = &fop_stdout;
    new_pcb->fda[1].inode = 0;
    new_pcb->fda[1].file_position = 0;
    new_pcb->fda[1].flags = 1;
    new_pcb->fda[1].file_type = 3;     

    //set rest of array to 0
    for(j = 2; j < FILE_MAX; j++) {
        new_pcb->fda[j].file_op_ptr = NULL;
        new_pcb->fda[j].inode = 0;
        new_pcb->fda[j].file_position = 0;
        new_pcb->fda[j].flags = 0;
        new_pcb->fda[j].file_type = 0;     
    }
}

/*
 * initial_pcb_array
 *   DESCRIPTION: initializes pcb array to not in use
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets entire pcb array to not in use
 */
void initial_pcb_array() {
    int i;
    for(i = 0; i < NUM_PCB; i++) {
        pcb_array[i].in_use = 0;
    }
}
