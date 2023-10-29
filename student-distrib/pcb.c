// /* pcb.c - Struct and helper functions for process control block
//  * 
//  */

// #include "pcb.h"


// /*
//  * pcb_init
//  *   DESCRIPTION: initializes a new process control block
//  *   INPUTS: none
//  *   OUTPUTS: new pcb struct with correct id based on which process
//  *   RETURN VALUE: pointer to pcb struct created
//  *   SIDE EFFECTS: loads pcb into kernel memory
//  */
// pcb_t* pcb_init() {
//     pcb_t new_pcb;
//     int i;

//     // Loads file array with dentry_t 
//     for(i = 0; i < 8; i++) {
//         new_pcb.file_array[i] = 0;
//     }
    
//     // Choose and initialize p_id

//     // 
// }
