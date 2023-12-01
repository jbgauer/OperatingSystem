/* thread.c - Struct and helper functions for individual threads
 * 
 */

#include "terminal.h"
#include "pit.h"

/*
 * switch_thread
 *   DESCRIPTION: switches process context from one thread to another
 *   INPUTS: 
 *   OUTPUTS: none 
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Context switch so that current process being executed switches 
 */
void switch_thread() {
    

    // not sure if using correct esp, ebp
    // not sure when to sent_eoi

    uint32_t terminal_pid = terminal[curr_thread].t_pid;
    // saving esp, ebp of finishing process
    asm volatile(
                 "movl %%esp, %0;" 
                 "movl %%ebp, %1;"
                 :
                 :"r"(pcb_array[terminal_pid].stack_ptr), "r"(pcb_array[terminal_pid].base_ptr)
                 :"%esp", "%ebp"
                 ); 
    
    
    curr_thread = (curr_thread+1)%3;
    terminal_pid = terminal[curr_thread].t_pid;

    // spawn shell if pcb is not in use
    if (pcb_array[curr_thread].in_use == 0) {

        uint8_t shellcmd[6] = "shell\0";
        execute(shellcmd);
       
    } else {

        // Context Switch


        // Store contents from TSS to enable restoration of Process state for the next time slice
        

    
        // update thread to the new one



        // Paging:
        // change virtual program image to point to next program in physical mem
        // redirect the program image
        uint32_t newEntry, pageHold; 
        pdir_entry_t kerntry;

        //Set new page (first addr at 0x400)
        pageHold = terminal_pid;
        pageHold += PAGES_DEFAULT_USE;
        kerntry.p_addr = pageHold << KENTRY_SHIFT;
        kerntry.ps = 1;
        kerntry.a = 0;
        kerntry.pcd = 0;
        kerntry.pwt = 0;
        kerntry.us = 1;
        kerntry.rw = 1;
        kerntry.p = 1;
        newEntry = combine_dir_entry(kerntry);
        pagedir[USER_SPACE] = newEntry;
    
        // Flush TLB on process switch
        flush_tlb();

    
        // Restore next process’ TSS

        // tss
        // Restore TSS contents relevant to Process and store in pcb

        // tss.esp0 = (uint32_t)pcb_array[terminal_pid].stack_ptr;
        tss.esp0 = ((0x00800000 - 4 - 0x200 * terminal_pid));
        tss.ss0  = KERNEL_DS;
        sti();
        // Switch ESP/EBP to next process’ kernel stack
        asm volatile(
                "movl %0, %%esp;" 
                "movl %1, %%ebp;" 
                "leave;"
                "ret;"
                :
                :"r"(pcb_array[terminal_pid].stack_ptr), "r"(pcb_array[terminal_pid].base_ptr)
                :"%esp", "%ebp"
                );
        
    }
    
    return;

}
