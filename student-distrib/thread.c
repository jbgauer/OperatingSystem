/* thread.c - Struct and helper functions for individual threads
 * 
 */

#include "terminal.h"

/*
 * switch_thread
 *   DESCRIPTION: switches process context from one thread to another
 *   INPUTS: 
 *   OUTPUTS: none 
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Context switch so that current process being executed switches 
 */
void switch_thread() {
    
    // curr_pid = terminal[curr_thread].t_pid;

    // // spawn shell if pcb is not in use
    // if (pcb_array[curr_pid].in_use == 0){

    //     uint8_t shellcmd[6] = "shell\0";
    //     execute(shellcmd);

    // }


    // // Context Switch


    // // Store contents from TSS to enable restoration of Process state for the next time slice

    // // saving esp, ebp of finishing process
    // asm volatile(
    //              "movl %%esp, %0;" 
    //              "movl %%ebp, %1;" 
    //              :
    //              :"r"(pcb_array[curr_pid].stack_ptr), "r"(pcb_array[curr_pid].base_ptr)
    //              :
    //              );   

    

    // // update thread to the new one
    // curr_thread = (curr_thread+1)%3;
    // curr_pid = terminal[curr_thread].t_pid;

    // // Switch ESP/EBP to next process’ kernel stack


    // asm volatile(
    //         "movl %0, %%esp;" 
    //         "movl %1, %%ebp;" 
    //         :
    //         :"r"(pcb_array[curr_pid].stack_ptr), "r"(pcb_array[curr_pid].base_ptr), "r"((uint32_t)status)
    //         :"eax"
    //         );

    // // Restore next process’ TSS

    // // tss
    // // Restore TSS contents relevant to Process and store in pcb

    // tss.esp0 = (uint32_t)pcb_array[curr_pid].stack_ptr;
    // tss.ss0  = KERNEL_DS;

    // // set up paging for new process

    // // Flush TLB on process switch





}
