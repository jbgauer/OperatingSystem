

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
    outb(REG_B, INDEX_PORT);		// select register B, and disable NMI
    char prev=inb(RW_PORT);	// read the current value of register B
    outb(REG_B, INDEX_PORT);		// set the index again (a read will reset the index to register D)
    outb(prev|0x40, RW_PORT);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

    // default frequency set
    
    interrupt_flag = 1;     // set high
    
    terminal[curr_thread].count = 0;
    terminal[curr_thread].num_ticks = 1;

    enable_irq(8);     //8 is rtc interrupt number
}

/*
 * rtc_handler
 *   DESCRIPTION: handler for rtc
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void rtc_handler(){
    
    // printf("RTC ");

    // test_interrupts();

    //from osdev wiki RTC, code to periodically send interrupts
    outb(REG_C, INDEX_PORT);	// select register C
    inb(RW_PORT);		// just throw away contents

    interrupt_flag = 0; // clear flag

    terminal[curr_thread].count += 1;

    send_eoi(8);
}


//////////////////////////////////////////////
// VIRTUALIZED CODE:                        //
//////////////////////////////////////////////

/*
 * rtc_open (virtualized)
 *   DESCRIPTION: sets rtc to MAX Hz, and initializes
 *                number interrupts to trigger a read
 *   INPUTS: filename
 *   OUTPUTS: int32_t
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int32_t rtc_open (const uint8_t* filename){

    //rtc_change_freq(MAX); // set frequency to maximum
    rtc_change_freq(MAX); // set frequency to maximum
    terminal[curr_thread].num_ticks = 1;
    return 0;
}

/*
 * rtc_read (virtualized)
 *   DESCRIPTION: returns 0 only after num ticks
 *                of interrupts happened
 *   INPUTS: int32_t fd, void* buf, int32_t nbytes
 *   OUTPUTS: int32_t
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes){
    terminal[curr_thread].count = 0;
    while(terminal[curr_thread].count != terminal[curr_thread].num_ticks);
    return 0;
}

/*
 * rtc_write (virtualized)
 *   DESCRIPTION: changes num ticks to signal a read operation
 *   INPUTS: int32_t fd, void* buf, int32_t nbytes
 *   OUTPUTS: int32_t
 *   RETURN VALUE: 0 or -1 (if failed parameter validation)
 *   SIDE EFFECTS: none
 */
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes){
    
    //parameter validation
    if(buf == 0) return -1;

    int32_t frequency;
    frequency = *((int*) buf);

    if(frequency > MAX || frequency < MIN) return -1;

    terminal[curr_thread].num_ticks = MAX/frequency; // max/frequency ended up being off by a factor of 2

    return 0;
}


/*
 * rtc_close
 *   DESCRIPTION: returns 0
 *   INPUTS: int32_t fd (file descriptor)
 *   OUTPUTS: int32_t
 *   RETURN VALUE: 0
 *   SIDE EFFECTS: none
 */
int32_t rtc_close (int32_t fd){
    return 0;
}



/*
 * rtc_change_freq
 *   DESCRIPTION: changes rtc's frequency from input
 *                by modifing the device
 *   INPUTS: int32_t frequency
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
// code taken and modified from OSDEV Wiki
void rtc_change_freq(int32_t frequency){
    int32_t rate;
                                        // 15 is max rate, add 1 cuz (rate subtracted by 1 in equation on next line)
    rate = MAXRATE + 1 - log(frequency);     //getting rate from frequency equation: frequency =  32768 >> (rate-1);
                                        //rate of 6 gives MAX frequency
    
    rate &= 0x0F;                   // must be less than 15, setting to 15 if greater
    if(rate<3) return;              // must be at least 3, rate of 3 is 8192 (max freq of rtc)
    
    cli();

    outb(REG_A, INDEX_PORT);		// select register A, and disable NMI
    char prev=inb(RW_PORT);	        // read the current value of register A
    outb(REG_A, INDEX_PORT);		// reset index to A
    outb((prev & 0xF0) | rate, RW_PORT);	// write only our rate to A

    sti();
}

/*
 * log
 *   DESCRIPTION: binary logarithm operation 
 *   INPUTS: int32_t n
 *   OUTPUTS: int32_t i (binary log of n)
 *   RETURN VALUE: int32_t
 *   SIDE EFFECTS: none
 */
static
int32_t log(int32_t n){
    int32_t i;
    i=0;
    while(n>1){
        n = n/MIN;
        i++;
    }
    return i;
}


//////////////////////////////////////////////
// UNVIRTUALIZED CODE:                      //
//////////////////////////////////////////////

// /*
//  * rtc_open
//  *   DESCRIPTION: sets rtc to 2 Hz
//  *   INPUTS: filename
//  *   OUTPUTS: int32_t
//  *   RETURN VALUE: 0
//  *   SIDE EFFECTS: none
//  */
// int32_t rtc_open (const uint8_t* filename){

//     rtc_change_freq(MIN); // set frequency to 2 Hz
//     return 0;
// }

/*
//  * rtc_read
//  *   DESCRIPTION: returns 0 only after interrupt has occurred
//  *   INPUTS: int32_t fd, void* buf, int32_t nbytes
//  *   OUTPUTS: int32_t
//  *   RETURN VALUE: 0
//  *   SIDE EFFECTS: none
//  */
// int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes){
    
//     interrupt_flag = 1;    //reset flag
//     while(interrupt_flag);
//     return 0;
// }

// /*
//  * rtc_write
//  *   DESCRIPTION: changes rtc's frequency from user input
//  *                checks if provided a valid frequency
//  *   INPUTS: int32_t fd, void* buf, int32_t nbytes
//  *   OUTPUTS: int32_t
//  *   RETURN VALUE: 0 or -1 (if invalid frequency)
//  *   SIDE EFFECTS: none
//  */
// int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes){
    
//     //parameter validation
//     if(buf == 0) return -1;
//     if(nbytes != 4) return -1; // 4 bytes in 32 bit integer

//     int32_t frequency;
//     frequency = *((int32_t*) buf);

//     if(frequency > MAX || frequency < MIN) return -1;

//     if(frequency & (frequency-1)) return -1;     //only one bit set at a time to be power of 2

//     rtc_change_freq(frequency);
//     return 0;
// }


