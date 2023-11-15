

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "lib.h"
#include "terminal.h"

#define NUM_SCANCODES 83
#define keyboard_buf_size 128

void keyboard_init();

void keyboard_handler();



#endif 
