

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "lib.h"

#define NUM_SCANCODES 83

char keyboard_buf[128];
uint8_t keyboard_buf_index;

void keyboard_init();

void keyboard_handler();



#endif 
