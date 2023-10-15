

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








void rtc_init(){
    enable_irq(8);     //1 is rtc irq number
}
