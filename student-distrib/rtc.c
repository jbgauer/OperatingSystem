

#include "rtc.h"
#include "i8259.h"


volatile int interrupt_flag;

static int32_t log(int32_t n);

/*
 * rtc_init
 *   DESCRIPTION: initializes the rtc
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
// code modified from OSDEV wiki on RTC
void rtc_init(){
    cli();
    outb(REG_B, INDEX_PORT);		// select register B, and disable NMI
    char prev=inb(RW_PORT);	// read the current value of register B
    outb(REG_B, INDEX_PORT);		// set the index again (a read will reset the index to register D)
    outb(prev|0x40, RW_PORT);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    // default frequency set
    
    interrupt_flag = 1;     // set high

    enable_irq(8);     //8 is rtc interrupt number
    sti();
}

/*
 * rtc_handler
 *   DESCRIPTION: handler for rtc
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: 
 */
void rtc_handler(){
    
    // printf("RTC ");

    // test_interrupts();

    //from osdev wiki RTC, code to periodically send interrupts
    outb(REG_C, INDEX_PORT);	// select register C
    inb(RW_PORT);		// just throw away contents

    interrupt_flag = 0; // clear flag

    send_eoi(8);
}


int32_t rtc_open (const uint8_t* filename){

    rtc_change_freq(2); // set frequency to 2 Hz
    return 0;
}

int32_t rtc_close (int32_t fd){
    return 0;
}

int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes){
    
    interrupt_flag = 1;    //reset flag
    while(interrupt_flag);
    return 0;
}

int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes){
    
    //parameter validation
    if(buf == 0) return -1;
    if(nbytes != 4) return -1; // 4 bytes in 32 bit integer

    int32_t frequency;
    frequency = *((int*) buf);

    if(frequency > 1024 || frequency < 2) return -1;

    if(frequency & (frequency-1)) return -1;     //only one bit set at a time to be power of 2

    rtc_change_freq(frequency);
    return 0;
}


// code taken and modified from OSDEV Wiki
void rtc_change_freq(int32_t frequency){
    int32_t rate;

    rate = 16 - log(frequency);     //getting rate from frequency equation: frequency =  32768 >> (rate-1);
                                    //rate of 6 gives 1024 frequency
    
    rate &= 0x0F;                   // must be less than 15
    if(rate<3) return;              // must be at least 3
    
    cli();

    outb(REG_A, INDEX_PORT);		// select register A, and disable NMI
    char prev=inb(RW_PORT);	        // read the current value of register A
    outb(REG_A, INDEX_PORT);		// reset index to A
    outb((prev & 0xF0) | rate, RW_PORT);	// write only our rate to A

    sti();
}

static
int32_t log(int32_t n){
    int32_t i;
    while(n>1){
        n = n/2;
        i++;
    }
    return i;
}
