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
}
