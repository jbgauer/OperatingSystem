// /* pcb.c - Struct and helper functions for process control block
//  * 
//  */

// #include "pcb.h"
// #include "paging.h"

// /*
//  * pcb_init
//  *   DESCRIPTION: initializes a new process control block
//  *   INPUTS: none
//  *   OUTPUTS: new pcb struct with correct id based on which process
//  *   RETURN VALUE: pointer to pcb struct created
//  *   SIDE EFFECTS: loads pcb into kernel memory
//  */
// pcb_t* pcb_init(uint32_t process_num) {
//     pcb_t* new_pcb = PCB[process_num];
//     int j;
//     new_pcb->in_use = 1;
//     new_pcb->p_id = process_num;

//     //stdin + stdout vals STILL NEED TO BE CHANGED!
//     //set fda[0] == stdin
//     new_pcb->fda[0].file_op_ptr = NULL;
//     new_pcb->fda[0].inode = 0;
//     new_pcb->fda[0].file_position = 0;
//     new_pcb->fda[0].flags = 0;
//     new_pcb->fda[0].filename = "";     //not sure if we want this
//     new_pcb->fda[0].file_type = 0;     //not sure if we want this
//     //set fda[1] == stdout 
//     new_pcb->fda[1].file_op_ptr = NULL;
//     new_pcb->fda[1].inode = 0;
//     new_pcb->fda[1].file_position = 0;
//     new_pcb->fda[1].flags = 0;
//     new_pcb->fda[1].filename = "";     //not sure if we want this
//     new_pcb->fda[1].file_type = 0;     //not sure if we want this

//     //set rest of array to 0
//     for(j = 2; j < FILE_MAX; j++) {
//         new_pcb->fda[j].file_op_ptr = NULL;
//         new_pcb->fda[j].inode = 0;
//         new_pcb->fda[j].file_position = 0;
//         new_pcb->fda[j].flags = 0;
//         new_pcb->fda[j].filename = "";     //not sure if we want this
//         new_pcb->fda[j].file_type = 0;     //not sure if we want this
//     }
// }
