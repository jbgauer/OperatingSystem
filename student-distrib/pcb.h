/* pcb.h - definitions for struct and helper functions for process control block
 * 
 */

#ifndef _PCB_H
#define _PCB_H

#include "lib.h"
#include "filesys_driver.h"
#define MAX_FILENAME_LEN 32
#define NUM_PCB 6
#define FILE_MAX 8
#define EIGHT_MB 0x800000
#define EIGHT_KB 0x2000

typedef struct {
    int32_t (*open)(const uint8_t*);
    int32_t (*read)(int32_t, void*, int32_t);
    int32_t (*write)(int32_t, const void*, int32_t);
    int32_t (*close)(int32_t);
} file_op_t;

typedef struct {
    file_op_t* file_op_ptr;  
    uint32_t inode;                         //files inode
    uint32_t file_position;                 //current position in file
    uint32_t flags;                         //current flags: 1 if in use, 0 if open
    //uint8_t filename[MAX_FILENAME_LEN];     //filename
    uint32_t file_type;                     //type of file
} fd_t;

typedef struct {
    uint32_t in_use;                // 1 if in use, 0 if not in use
    uint32_t stack_ptr;             // Holds current stack position of process
    uint32_t base_ptr;              // Holds current base position of process
    uint32_t par_id;                // parent process number

    // uint32_t* inst_ptr;             // Holds next instruction to be executed for process
    fd_t fda[FILE_MAX];             // Array that holds file descriptors of every file used by this program
} pcb_t;


/*
 * pcb_init
 *   DESCRIPTION: initializes a new process control block
 *   INPUTS: uint32_t - process number of parent
 *   OUTPUTS: changed entry in pcb array
 *   RETURN VALUE: none
 *   SIDE EFFECTS: loads pcb into array
 */
void pcb_init(uint32_t par_process_num);

pcb_t pcb_array[NUM_PCB];

#endif
