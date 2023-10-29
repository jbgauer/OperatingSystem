// /* pcb.h - definitions for struct and helper functions for process control block
//  * 
//  */

// #ifndef _PCB_H
// #define _PCB_H

// #include "lib.h"
// #include "filesys_driver.h"
// #define MAX_FILENAME_LEN 32

// typedef struct {
//     uint32_t* stack_ptr;            // Holds current stack position of process
//     uint8_t p_id;                   // Number of process
//     uint32_t* inst_ptr;             // Holds next instruction to be executed for process
//     uint32_t p_eax;                 // Holds current registers of process
//     uint32_t p_ebx;
//     uint32_t p_ecx;
//     uint32_t p_edx;
//     uint32_t p_edi; 
//     uint32_t p_esi; 
//     dentry_t file_array[8];         // Array that holds dentrys of every file used by this program
// } pcb_t;

// typedef struct {
//     uint32_t* file_op_ptr;  
//     uint32_t inode;         //files inode
//     uint32_t file_position; //current position in file
//     uint32_t flags;         //current flags
//     uint8_t filename[MAX_FILENAME_LEN];     //filename
//     uint32_t file_type;                     //type of file
// } fda_t;


// /*
//  * pcb_init
//  *   DESCRIPTION: initializes a new process control block
//  *   INPUTS: none
//  *   OUTPUTS: new pcb struct with correct id based on which process
//  *   RETURN VALUE: pointer to pcb struct created
//  *   SIDE EFFECTS: loads pcb into kernel memory
//  */
// pcb_t* pcb_init();

// #endif
