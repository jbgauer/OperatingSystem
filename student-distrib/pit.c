
#include "pit.h"


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
    
    switch_thread();   //go to next thread
    send_eoi(PIT_IRQ);
    
    sti();
}
