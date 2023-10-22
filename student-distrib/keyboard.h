

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "lib.h"

#define NUM_SCANCODES 83
#define keyboard_buf_size 128

char keyboard_buf[keyboard_buf_size];
uint8_t keyboard_buf_index;
uint8_t enter_flag;

void keyboard_init();

void keyboard_handler();



#endif 
