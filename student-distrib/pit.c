
#include "pit.h"
#include "pcb.h"

uint32_t pit_count = 0;

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

    send_eoi(PIT_IRQ);
    // if(pit_count < 3) {
    //     pit_count++;
    //     if(pcb_array[0].in_use == 1) {
    //         curr_thread = (curr_thread+1)%3;
    //     }
    //     terminal[curr_thread].t_pid = curr_thread;
    //     uint8_t shellcmd[6] = "shell\0";
    //     execute(shellcmd);
        
    // } else {
    
    
   switch_thread();   //go to next thread
    // if(pit_count < 9) {
    //     printf(" curr_thread is : %d ", curr_thread);
    //     printf("pcb_array[curr_thread].stack_ptr = %d", pcb_array[curr_thread].stack_ptr);
    //     printf("\n");
    //     printf("pcb_array[curr_thread].base_ptr = %d", pcb_array[curr_thread].base_ptr);
    //     switch_thread();   //go to next thread
    //     printf(" switched thread to : %d", curr_thread);

    // }
    
    
    
    sti();
}
