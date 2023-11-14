/* thread.h - definitions for struct and helper functions for individual threads
 * 
 */

#ifndef _THREAD_H
#define _THREAD_H

#include "lib.h"

#define keyboard_buf_size 128

typedef struct {
    int top_pid;
    char keyboard_buf[keyboard_buf_size];
    uint8_t keyboard_buf_index;
    uint8_t enter_flag;
} thread_t;

thread_t threads[3];

int curr_thread;

#endif
