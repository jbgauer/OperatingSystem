
#include "pit.h"

uint32_t pit_counter = 0;
/*
 * pit_init
 *   DESCRIPTION: initializes the PIT
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: sets pit to channel 0, mode 3 (square wave), low/highbyte, irq0.
 */
void pit_init() {
    //set channel 0
    outb(0x36, PIT_CMD);
    //set PIT reload value
    //get top 8 of def_freq/our_freq (10 milliseconds)
    uint32_t div_freq = PIT_DEF_FREQ / PIT_FREQ;
    outb((div_freq & 0xFF), PIT_DATA);
    outb((div_freq >> 8), PIT_DATA);
    //enable irq0 on PIC
    enable_irq(PIT_IRQ);
}

/*
 * pit_handler
 *   DESCRIPTION: 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: 
 */
void pit_handler() {
    cli();
    uint32_t terminal_pid;
    uint32_t newEntry, pageHold; 
    uint8_t shellcmd[6] = "shell\0";
    pdir_entry_t kerntry;
    send_eoi(PIT_IRQ);
   // switch_thread();   //go to next thread
    
    // not sure if using correct esp, ebp
    // not sure when to sent_eoi
    //pit_counter++;
    if(curr_thread != -1) {
        terminal_pid = terminal[curr_thread].t_pid;
    
    // saving esp, ebp of finishing process   
        // asm volatile(
        //             "movl %%esp, %0;" 
        //             "movl %%ebp, %1;"
        //             :"=r"(pcb_array[terminal_pid].stack_ptr), "=r"(pcb_array[terminal_pid].base_ptr)
        //             :
        //             :"%esp", "%ebp"
        //             ); 

        asm volatile("movl %%esp, %0;":"=r"(pcb_array[terminal_pid].stack_ptr));
        asm volatile("movl %%ebp, %0;":"=r"(pcb_array[terminal_pid].base_ptr));
    } else { 
        // asm volatile(
        //             "movl %%esp, %0;" 
        //             "movl %%ebp, %1;"
        //             :
        //             :"=r"(pcb_array[0].stack_ptr), "=r"(pcb_array[0].base_ptr)
        //             :"%esp", "%ebp"
        //             ); 
        asm volatile("movl %%esp, %0;":"=r"(pcb_array[0].stack_ptr));
        asm volatile("movl %%ebp, %0;":"=r"(pcb_array[0].base_ptr));
    }
    
    curr_thread = (curr_thread+1)%3;
    terminal_pid = terminal[curr_thread].t_pid;
    
    // spawn shell if pcb is not in use
    if (pit_counter < 3) {
        curr_pid = curr_thread;
        pit_counter++;
        execute(shellcmd);
       
    } else {
        curr_pid = terminal_pid;
        // Context Switch

        // Paging:
        // change virtual program image to point to next program in physical mem
        // redirect the program image
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

        // tss.esp0 = (uint32_t)pcb_array[terminal_pid].stack_ptr;
        tss.esp0 = ((0x00800000 - 4 - 0x2000 * terminal_pid));
        //tss.ss0  = KERNEL_DS;
        
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
    sti();
}
