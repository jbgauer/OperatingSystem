/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask; /* IRQs 8-15 */



/* Initialize the 8259 PIC */
void i8259_init(void) {

    /*set interupt masks*/
    master_mask = inb(MASTER_8259_IMR);
    slave_mask = inb(SLAVE_8259_IMR);
    /*mask all of master/slave PIC*/
    outb(0xFF, MASTER_8259_IMR);
    outb(0xFF, SLAVE_8259_IMR);

    /*master PIC control words*/
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW2_MASTER, MASTER_8259_PORT);
    outb(ICW3_MASTER, MASTER_8259_PORT);
    outb(ICW4, MASTER_8259_PORT);
    
    /*slave PIC control words*/
    outb(ICW1, SLAVE_8259_PORT);
    outb(ICW2_SLAVE, SLAVE_8259_PORT);
    outb(ICW3_SLAVE, SLAVE_8259_PORT);
    outb(ICW4, SLAVE_8259_PORT);
    /*restore PICs*/
    outb(master_mask, MASTER_8259_PORT);
    outb(slave_mask, MASTER_8259_PORT);
    

}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    /*Master or Slave PIC?*/
    if(irq_num & 8) {
        /*Slave PIC*/
        irq_num -= 8;
        slave_mask = inb(SLAVE_8259_PORT) & ~(1 << irq_num);
        outb(slave_mask, SLAVE_8259_PORT);
    } else {
        /*Master PIC*/
        master_mask = inb(MASTER_8259_PORT) & ~(1 << irq_num);
        outb(master_mask,MASTER_8259_PORT);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    /*Master or Slave PIC?*/
    if(irq_num & 8) {
        /*Slave PIC*/
        irq_num -= 8;
        slave_mask = inb(SLAVE_8259_PORT) | (1 << irq_num);
        outb(slave_mask, SLAVE_8259_PORT);
    } else {
        /*Master PIC*/
        master_mask = inb(MASTER_8259_PORT) | (1 << irq_num);
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

