
#ifndef _VGA_H
#define _VGA_H


#define REG_A   0x8A
#define REG_B   0x8B
#define REG_C   0x8C

#define INDEX_PORT 0x70
#define RW_PORT 0x71

void rtc_init();

void rtc_handler();



#endif 
