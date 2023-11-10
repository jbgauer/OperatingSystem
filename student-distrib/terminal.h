#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "lib.h"

#define KEYBOARD_BUF_LEN 128

int32_t term_open(const uint8_t* filename);

int32_t term_close(int32_t fd);

int32_t term_read(int32_t fd, void* buf, int32_t nbytes);

int32_t term_write(int32_t fd, const void* buf, int32_t nbytes);

int32_t bad_write (int32_t fd, const void* buf, int32_t nbytes);

int32_t bad_read (int32_t fd, void* buf, int32_t nbytes);

#endif
