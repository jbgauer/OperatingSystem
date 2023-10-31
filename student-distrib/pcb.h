// /* pcb.h - definitions for struct and helper functions for process control block
//  * 
//  */

// #ifndef _PCB_H
// #define _PCB_H

// #include "lib.h"
// #include "filesys_driver.h"
// #define MAX_FILENAME_LEN 32
// #define NUM_PCB 6
// #define FILE_MAX 8

// pcb_t* PCB[NUM_PCB];

// typedef struct {
//     uint32_t in_use;                // 1 if in use, 0 if not in use
//     uint32_t* stack_ptr;            // Holds current stack position of process
//     uint32_t p_id;                  // Number of process
//     uint32_t* inst_ptr;             // Holds next instruction to be executed for process
//     uint32_t p_eax;                 // Holds current registers of process
//     uint32_t p_ebx;
//     uint32_t p_ecx;
//     uint32_t p_edx;
//     uint32_t p_edi; 
//     uint32_t p_esi; 
//     fd_t fda[FILE_MAX];             // Array that holds file descriptors of every file used by this program
// } pcb_t;

// typedef struct {
//     uint32_t* file_op_ptr;  
//     uint32_t inode;         //files inode
//     uint32_t file_position; //current position in file
//     uint32_t flags;         //current flags
//     uint8_t filename[MAX_FILENAME_LEN];     //filename
//     uint32_t file_type;                     //type of file
// } fd_t;


// /*
//  * pcb_init
//  *   DESCRIPTION: initializes a new process control block
//  *   INPUTS: uint32_t - Number of process to create new pcb at 
//  *   OUTPUTS: new pcb struct in correct process control block
//  *   RETURN VALUE: pointer to pcb struct created
//  *   SIDE EFFECTS: initializes pcb in kernel memory
//  */
// pcb_t* pcb_init(uint32_t process_num);

// #endif
