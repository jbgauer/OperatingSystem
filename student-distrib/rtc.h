
#ifndef _VGA_H
#define _VGA_H

#include "lib.h"

#define REG_A   0x8A
#define REG_B   0x8B
#define REG_C   0x8C

#define INDEX_PORT 0x70
#define RW_PORT 0x71

#define MAX 1024   //maximum frequency
#define MIN 2      //minimum frequency

#define MAXRATE 15 //maximum frequency

void rtc_init();

void rtc_handler();

int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes);
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes);

int32_t rtc_open (const uint8_t* filename);
int32_t rtc_close (int32_t fd);

void rtc_change_freq(int frequency);

// int32_t rtc_virt_read (int32_t fd, void* buf, int32_t nbytes);
// int32_t rtc_virt_write (int32_t fd, const void* buf, int32_t nbytes);

// int32_t rtc_virt_open (const uint8_t* filename);
// int32_t rtc_virt_close (int32_t fd);



#endif 
