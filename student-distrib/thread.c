/* thread.c - Struct and helper functions for individual threads
 * 
 */

#include "terminal.h"

/*
 * switch_thread
 *   DESCRIPTION: switches process context from one thread to another
 *   INPUTS: int - thread_id: index of thread in terminal array
 *   OUTPUTS: none 
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Context switch so that current process being executed switches 
 */
void switch_thread(int thread_id) {
    // Switch current process id
    curr_pid = terminal[thread_id].t_pid;

    // Context Switch


    // Store contents from TSS to enable restoration of Process state for the next time slice



    /*
    Utilize the kernel stack (think about what you did for HALT)
    You will be using assembly to do the context switch
    Switch ESP/EBP to next process’ kernel stack
    Restore next process’ TSS
    Flush TLB on process switch
    */

   // to switch to next process:
   // set up paging, change current process id

   // push current context information to its kernel stack

    // tss
    // Restore TSS contents relevant to Process and store in pcb
}
