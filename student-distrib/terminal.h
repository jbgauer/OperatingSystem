#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "lib.h"

#define KEYBOARD_BUF_LEN 128
#define MAX_TERMINALS 3

typedef struct {
    char key_buf[KEYBOARD_BUF_LEN];
    uint8_t buf_i;
    uint8_t enter_flag;
    uint8_t in_use;
    int scr_x;
    int scr_y;
} term_t;

term_t terminal[MAX_TERMINALS];

int curr_terminal;

void terminals_init();

void term_init(uint32_t num);

int32_t term_open(const uint8_t* filename);

int32_t term_close(int32_t fd);

int32_t term_read(int32_t fd, void* buf, int32_t nbytes);

int32_t term_write(int32_t fd, const void* buf, int32_t nbytes);

int32_t bad_write (int32_t fd, const void* buf, int32_t nbytes);

int32_t bad_read (int32_t fd, void* buf, int32_t nbytes);

#endif
