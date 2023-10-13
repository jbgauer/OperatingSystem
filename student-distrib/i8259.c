/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/*Port Values for PIC*/
uint8_t irq_mask = 0xFFFF;   /*Mask for both PICs*/

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = irq_mask; /* IRQs 0-7  */
uint8_t slave_mask = irq_mask >> 8;  /* IRQs 8-15 */



/* Initialize the 8259 PIC */
void i8259_init(void) {
    /*mask all of master PIC*/
    //outb(0xFF, MASTER_8259_PORT);
    /*mask all of slave PIC*/
   // outb(0xFF, SLAVE_8259_PORT);
    /*write initialization control words*/
    //outb(ICW1, /*?*/);
   // outb(ICW2_MASTER, );
   // outb(ICW3_MASTER, );
    //outb(ICW4, /*?*/)

    /*slave PIC control words*/
    //outb(ICW1, /*?*/);
   // outb(ICW2_SLAVE, );
   // outb(ICW3_SLAVE, );
    //outb(ICW4, /*?*/)
   // outb(master_mask, MASTER8259_PORT);
  //  outb(slave_mask, SLAVE_8259_PORT);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    uint8_t mask = ~(1 << irq_num);
    irq_mask &= mask;
    /*Master or Slave PIC?*/
    if(irq_num & 8) {
        /*Slave PIC*/
        outb(slave_mask, SLAVE_8259_PORT);
    } else {
        /*Master PIC*/
        outb(master_mask,MASTER_8259_PORT);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    uint8_t mask = 1 << irq_num;
    irq_mask |= mask;
    /*Master or Slave PIC?*/
    if(irq_num & 8) {
        /*Slave PIC*/
        outb(slave_mask, SLAVE_8259_PORT);
    } else {
        /*Master PIC*/
        outb(master_mask,MASTER_8259_PORT);
    }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    
    if(irq_num & 8 ) {
        /*Slave PIC*/
        outb((slave_mask|EOI), SLAVE_8259_PORT);
    } else {
        /*Master PIC*/
        outb((master_mask|EOI), MASTER_8259_PORT);
    }

}

