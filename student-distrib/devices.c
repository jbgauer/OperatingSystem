

#include "devices.h"
#include "i8259.h"
#include "lib.h"


char scancodeTranslator[83] =       // only putting alphabet and numbers for now, mapping all other keys to show nothing
// 82 is all entries included lowercase and all numbers

{0x0, 0x0, '1', '2', '3', '4', '5', '6', '7', '8', 
 '9', '0', 0x0, 0x0, 0x0, 0x0, 'q', 'w', 'e', 'r', 
 't', 'y', 'u', 'i', 'o', 'p', 0x0, 0x0, 0x0, 0x0, 
 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0x0, 
 0x0, 0x0, 0x0, 0x0, 'z', 'x', 'c', 'v', 'b', 'n', 
 'm', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
 0x0, '7', '8', '9', 0x0, '4', '5', '6', 0x0, '1', 
 '2', '3', '0' 
};



void keyboard_init(){
    enable_irq(1);     //1 is keyboard irq number
}


void keyboard_handler(){
    cli();

    uint8_t scan_code = inb(0x60); // port to receive scancodes

    if((scan_code < 83) && (scancodeTranslator[scan_code] != 0x0)){     // 0x0 is not mapped entry
        putc(scancodeTranslator[scan_code]);
    }

    send_eoi(1);
    sti();
}



// code from OSDEV wiki on RTC
void rtc_init(){
    cli();
    outb(0x8B, 0x70);		// select register B, and disable NMI
    char prev=inb(0x71);	// read the current value of register B
    outb(0x8B, 0x70);		// set the index again (a read will reset the index to register D)
    outb(prev | 0x40, 0x71);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    outb(0x8A, 0x70);       // set frequency to 0
    outb(0x0, 0x71);

    enable_irq(8);     //8 is rtc interrupt number
    sti();
}


void rtc_handler(){

    cli();

    // printf("RTC ");
    outb(0x8A, 0x70);       
    outb(0x06, 0x71);       // set default frequency: 1024 Hz

    test_interrupts();

    //from osdev wiki RTC, code to periodically send interrupts
    outb(0x8C, 0x70);	// select register C
    inb(0x71);		// just throw away contents

    send_eoi(8);
    sti();


}


