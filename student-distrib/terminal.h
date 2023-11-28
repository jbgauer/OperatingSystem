#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "lib.h"
#include "paging.h"

#define KEYBOARD_BUF_LEN 128
#define MAX_TERMINALS 3
#define PAGE_SIZE 4096
#define VIDEO_ADDR 0xB8000
#define PAGE_SHIFT 12

typedef struct {
    char key_buf[KEYBOARD_BUF_LEN];
    uint8_t buf_i;
    uint8_t enter_flag;
    int scr_x;
    int scr_y;
    int t_pid;
} term_t;

term_t terminal[MAX_TERMINALS];

int curr_terminal;  // the visible terminal

int curr_thread;    // terminal scheduler is updating

void switch_thread();

void change_terminal(int32_t term_id);

void term_init(uint32_t num, int screen_x, int screen_y);

int32_t term_open(const uint8_t* filename);

int32_t term_close(int32_t fd);

int32_t term_read(int32_t fd, void* buf, int32_t nbytes);

int32_t term_write(int32_t fd, const void* buf, int32_t nbytes);

int32_t bad_write (int32_t fd, const void* buf, int32_t nbytes);

int32_t bad_read (int32_t fd, void* buf, int32_t nbytes);



#endif
