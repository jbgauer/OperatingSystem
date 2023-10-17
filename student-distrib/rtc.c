

#include "rtc.h"
#include "i8259.h"
#include "lib.h"


// code modified from OSDEV wiki on RTC
void rtc_init(){
    cli();
    outb(REG_B, INDEX_PORT);		// select register B, and disable NMI
    char prev=inb(RW_PORT);	// read the current value of register B
    outb(REG_B, INDEX_PORT);		// set the index again (a read will reset the index to register D)
    outb(prev|0x40, RW_PORT);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    // default frequency set
    // outb(REG_A, INDEX_PORT);       // set frequency 
    // outb(1024, RW_PORT);

    enable_irq(8);     //8 is rtc interrupt number
    sti();
}


void rtc_handler(){
    // printf("RTC ");

    test_interrupts();

    //from osdev wiki RTC, code to periodically send interrupts
    outb(REG_C, INDEX_PORT);	// select register C
    inb(RW_PORT);		// just throw away contents

    send_eoi(8);
}


